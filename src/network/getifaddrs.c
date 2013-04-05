/* (C) 2013 John Spencer. released under musl's standard MIT license. */
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <ifaddrs.h>
#include <stdlib.h>
#include <net/if.h> /* IFNAMSIZ, ifreq, ifconf */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h> /* inet_pton */
#include <unistd.h>
#include <sys/ioctl.h>

static struct ifaddrs* list_add(struct ifaddrs** list, struct ifaddrs** head, char* ifname)
{
	struct ifaddrs* curr = calloc(1, sizeof(struct ifaddrs));
	if(curr) {
		curr->ifa_name = strdup(ifname);
		if(!curr->ifa_name) {
			free(curr);
			curr = 0;
			goto out;
		}
		if(*head) (*head)->ifa_next = curr;
		*head = curr;
		if(!*list) *list = curr;
	}
	out:
	return curr;
}

void freeifaddrs(struct ifaddrs *ifp)
{
	struct ifaddrs *head = ifp;
	while(head) {
		free(head->ifa_name);
		free(head->ifa_addr);
		free(head->ifa_netmask);
		free(head->ifa_ifu.ifu_dstaddr);
		free(head->ifa_data);
		void *p = head;
		head = head->ifa_next;
		free(p);
	}
}

static struct sockaddr *sockaddr_in_dup(struct sockaddr_in *src)
{
	struct sockaddr_in *nu = malloc(sizeof(struct sockaddr_in));
	if(nu) *nu = *src;
	return (struct sockaddr*) nu;
}

static struct sockaddr *sockaddr_in6_dup(struct sockaddr_in6 *src)
{
	struct sockaddr_in6 *nu = malloc(sizeof(struct sockaddr_in6));
	if(nu) *nu = *src;
	return (struct sockaddr*) nu;
}

static void ipv6netmask(unsigned prefix_length, struct sockaddr_in6 *sa)
{
	// FIXME: left for bit-wizard rich
	memset(&sa->sin6_addr, -1, sizeof(sa->sin6_addr));
}

static void dealwithipv6(struct ifaddrs **list, struct ifaddrs** head)
{
	FILE* f = fopen("/proc/net/if_inet6", "r");
	/* 00000000000000000000000000000001 01 80 10 80 lo
	   A                                B  C  D  E  F
	   all numbers in hex
	   A = addr B=netlink device#, C=prefix length,
	   D = scope value (ipv6.h) E = interface flags (rnetlink.h, addrconf.c)
	   F = if name */
	char v6conv[32 + 7 + 1], *v6;
	char *line, linebuf[512];
	if(!f) return;
	while((line = fgets(linebuf, sizeof linebuf, f))) {
		v6 = v6conv;
		size_t i = 0;
		for(; i < 8; i++) {
			memcpy(v6, line, 4);
			v6+=4;
			*v6++=':';
			line+=4;
		}
		--v6; *v6 = 0;
		line++;
		unsigned b, c, d, e;
		char name[IFNAMSIZ+1];
		if(5 == sscanf(line, "%x %x %x %x %s", &b, &c, &d, &e, name)) {
			struct sockaddr_in6 sa = {0};
			if(1 == inet_pton(AF_INET6, v6conv, &sa.sin6_addr)) {
				sa.sin6_family = AF_INET6;
				struct ifaddrs* curr = list_add(list, head, name);
				if(!curr) goto out;
				curr->ifa_addr = sockaddr_in6_dup(&sa);
				ipv6netmask(c, &sa);
				curr->ifa_netmask = sockaddr_in6_dup(&sa);
				/* find ipv4 struct with the same interface name to copy flags */
				struct ifaddrs* scan = *list;
				for(;scan && strcmp(name, scan->ifa_name);scan=scan->ifa_next);
				if(scan) curr->ifa_flags=scan->ifa_flags;
				else curr->ifa_flags = 0;
			} else errno = 0;
		}
	}
	out:
	fclose(f);
}

int getifaddrs(struct ifaddrs **ifap)
{
	FILE* f = fopen("/proc/net/dev", "r");
	/* the alternative to parsing /proc.. seems to be iterating
	   through the interfaces using an index number in ifreq.ifr_ifindex
	   until we get some error code back. the kernel will fill ifr_name field
	   for valid ifindices (SIOCGIFINDEX) */
	if(!f) return -1;
	struct ifaddrs *list = 0, *head = 0;

	char* line; char linebuf[512];
	while((line = fgets(linebuf, sizeof linebuf, f))) {
		while(isspace(*line) && *line) line++;
		char* start = line;
		while(*line && isalnum(*line)) line++;
		if(line > start && *line == ':') {
			// found interface
			*line = 0;
			struct ifaddrs* curr = list_add(&list, &head, start);
			if(!curr) {
				fclose(f);
				goto err2;
			}
		}
	}
	fclose(f);

	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(sock == -1) goto err2;
	struct ifreq reqs[32]; /* arbitrary chosen boundary */
	struct ifconf conf = {.ifc_len = sizeof reqs, .ifc_req = reqs};
	if(-1 == ioctl(sock, SIOCGIFCONF, &conf)) goto err;
	else {
		size_t reqitems = conf.ifc_len / sizeof(struct ifreq);
		for(head = list; head; head=head->ifa_next) {
			size_t i;
			for(i = 0; i < reqitems; i++) {
				// get SIOCGIFADDR of active interfaces.
				if(!strcmp(reqs[i].ifr_name, head->ifa_name)) {
					head->ifa_addr = sockaddr_in_dup((struct sockaddr_in*) &reqs[i].ifr_addr);
					break;
				}
			}
			struct ifreq req;
			snprintf(req.ifr_name, sizeof req.ifr_name, "%s", head->ifa_name);
			if(-1 == ioctl(sock, SIOCGIFFLAGS, &req)) goto err;

			head->ifa_flags = req.ifr_flags;
			if(head->ifa_addr) {
				/* or'ing flags with IFF_LOWER_UP on active interfaces to mimic glibc */
				head->ifa_flags |= IFF_LOWER_UP; 
				if(-1 == ioctl(sock, SIOCGIFNETMASK, &req)) goto err;
				head->ifa_netmask = sockaddr_in_dup((struct sockaddr_in*) &req.ifr_netmask);
		
				if(head->ifa_flags & IFF_POINTOPOINT) {
					if(-1 == ioctl(sock, SIOCGIFDSTADDR, &req)) goto err;
					head->ifa_ifu.ifu_dstaddr = sockaddr_in_dup((struct sockaddr_in*) &req.ifr_dstaddr);
				} else {
					if(-1 == ioctl(sock, SIOCGIFBRDADDR, &req)) goto err;
					head->ifa_ifu.ifu_broadaddr = sockaddr_in_dup((struct sockaddr_in*) &req.ifr_broadaddr);
				}
			}
		}
	}
	close(sock);
	void* last = 0;
	for(head = list; head; head=head->ifa_next) last=head;
	head = last;
	dealwithipv6(&list, &head);
	*ifap = list;
	return 0;
	err:
	close(sock);
	err2:
	freeifaddrs(list);
	return -1;
}

