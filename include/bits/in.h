#define IP_TOS             1
#define IP_TTL             2
#define IP_HDRINCL         3
#define IP_OPTIONS         4
#define IP_ROUTER_ALERT    5
#define IP_RECVOPTS        6
#define IP_RETOPTS         7
//#define IP_PKTINFO         8
#define IP_PKTOPTIONS      9
#define IP_PMTUDISC        10
#define IP_MTU_DISCOVER    10
#define IP_RECVERR         11
#define IP_RECVTTL         12
#define IP_RECVTOS         13
#define IP_MULTICAST_IF    32
#define IP_MULTICAST_TTL   33
#define IP_MULTICAST_LOOP  34
#define IP_ADD_MEMBERSHIP  35
#define IP_DROP_MEMBERSHIP 36

#define IP_RECVRETOPTS IP_RETOPTS

#define IP_PMTUDISC_DONT   0
#define IP_PMTUDISC_WANT   1
#define IP_PMTUDISC_DO     2

#define SOL_IP 0

#define IP_DEFAULT_MULTICAST_TTL        1
#define IP_DEFAULT_MULTICAST_LOOP       1
#define IP_MAX_MEMBERSHIPS              20

struct ip_opts
{
	struct in_addr ip_dst;
	char ip_opts[40];
};

struct ip_mreq
{
	struct in_addr imr_multiaddr;
	struct in_addr imr_interface;
};

struct ip_mreqn
{
	struct in_addr imr_multiaddr;
	struct in_addr imr_address;
	int imr_ifindex;
};

struct in_pktinfo
{
	int ipi_ifindex;
	struct in_addr ipi_spec_dst;
	struct in_addr ipi_addr;
};

#define IPV6_ADDRFORM           1
#define IPV6_PKTINFO            2
#define IPV6_HOPOPTS            3
#define IPV6_DSTOPTS            4
#define IPV6_RXSRCRT            5
#define IPV6_PKTOPTIONS         6
#define IPV6_CHECKSUM           7
#define IPV6_HOPLIMIT           8
#define IPV6_NEXTHOP            9
#define IPV6_AUTHHDR            10
#define IPV6_UNICAST_HOPS       16
#define IPV6_MULTICAST_IF       17
#define IPV6_MULTICAST_HOPS     18
#define IPV6_MULTICAST_LOOP     19
#define IPV6_ADD_MEMBERSHIP     20
#define IPV6_DROP_MEMBERSHIP    21
#define IPV6_ROUTER_ALERT       22

#define SCM_SRCRT               IPV6_RXSRCRT

#define IPV6_RXHOPOPTS          IPV6_HOPOPTS
#define IPV6_RXDSTOPTS          IPV6_DSTOPTS


#define IPV6_PMTUDISC_DONT      0
#define IPV6_PMTUDISC_WANT      1
#define IPV6_PMTUDISC_DO        2


#define SOL_IPV6        41
#define SOL_ICMPV6      58
