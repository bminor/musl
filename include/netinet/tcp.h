#ifndef _NETINET_TCP_H
#define _NETINET_TCP_H

#include <features.h>

#define TCP_NODELAY 1
#define TCP_MAXSEG	 2
#define TCP_CORK	 3
#define TCP_KEEPIDLE	 4
#define TCP_KEEPINTVL	 5
#define TCP_KEEPCNT	 6
#define TCP_SYNCNT	 7
#define TCP_LINGER2	 8
#define TCP_DEFER_ACCEPT 9
#define TCP_WINDOW_CLAMP 10
#define TCP_INFO	 11
#define	TCP_QUICKACK	 12
#define TCP_CONGESTION	 13
#define TCP_MD5SIG	 14
#define TCP_THIN_LINEAR_TIMEOUTS 16
#define TCP_THIN_DUPACK  17
#define TCP_USER_TIMEOUT 18
#define TCP_REPAIR       19
#define TCP_REPAIR_QUEUE 20
#define TCP_QUEUE_SEQ    21
#define TCP_REPAIR_OPTIONS 22
#define TCP_FASTOPEN     23
#define TCP_TIMESTAMP    24
#define TCP_NOTSENT_LOWAT 25

#define TCP_ESTABLISHED  1
#define TCP_SYN_SENT     2
#define TCP_SYN_RECV     3
#define TCP_FIN_WAIT1    4
#define TCP_FIN_WAIT2    5
#define TCP_TIME_WAIT    6
#define TCP_CLOSE        7
#define TCP_CLOSE_WAIT   8
#define TCP_LAST_ACK     9
#define TCP_LISTEN       10
#define TCP_CLOSING      11

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define SOL_TCP 6
#include <sys/types.h>
#include <sys/socket.h>
#endif

#ifdef _GNU_SOURCE
#include <endian.h>
struct tcphdr
{
	u_int16_t source;
	u_int16_t dest;
	u_int32_t seq;
	u_int32_t ack_seq;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	u_int16_t res1:4;
	u_int16_t doff:4;
	u_int16_t fin:1;
	u_int16_t syn:1;
	u_int16_t rst:1;
	u_int16_t psh:1;
	u_int16_t ack:1;
	u_int16_t urg:1;
	u_int16_t res2:2;
#else
	u_int16_t doff:4;
	u_int16_t res1:4;
	u_int16_t res2:2;
	u_int16_t urg:1;
	u_int16_t ack:1;
	u_int16_t psh:1;
	u_int16_t rst:1;
	u_int16_t syn:1;
	u_int16_t fin:1;
#endif
	u_int16_t window;
	u_int16_t check;
	u_int16_t urg_ptr;
};

#define TCPI_OPT_TIMESTAMPS	1
#define TCPI_OPT_SACK		2
#define TCPI_OPT_WSCALE		4
#define TCPI_OPT_ECN		8

#define TCP_CA_Open		0
#define TCP_CA_Disorder		1
#define TCP_CA_CWR		2
#define TCP_CA_Recovery		3
#define TCP_CA_Loss		4

struct tcp_info
{
	u_int8_t tcpi_state;
	u_int8_t tcpi_ca_state;
	u_int8_t tcpi_retransmits;
	u_int8_t tcpi_probes;
	u_int8_t tcpi_backoff;
	u_int8_t tcpi_options;
	u_int8_t tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;
	u_int32_t tcpi_rto;
	u_int32_t tcpi_ato;
	u_int32_t tcpi_snd_mss;
	u_int32_t tcpi_rcv_mss;
	u_int32_t tcpi_unacked;
	u_int32_t tcpi_sacked;
	u_int32_t tcpi_lost;
	u_int32_t tcpi_retrans;
	u_int32_t tcpi_fackets;
	u_int32_t tcpi_last_data_sent;
	u_int32_t tcpi_last_ack_sent;
	u_int32_t tcpi_last_data_recv;
	u_int32_t tcpi_last_ack_recv;
	u_int32_t tcpi_pmtu;
	u_int32_t tcpi_rcv_ssthresh;
	u_int32_t tcpi_rtt;
	u_int32_t tcpi_rttvar;
	u_int32_t tcpi_snd_ssthresh;
	u_int32_t tcpi_snd_cwnd;
	u_int32_t tcpi_advmss;
	u_int32_t tcpi_reordering;
	u_int32_t tcpi_rcv_rtt;
	u_int32_t tcpi_rcv_space;
	u_int32_t tcpi_total_retrans;
};

#define TCP_MD5SIG_MAXKEYLEN    80

struct tcp_md5sig
{
	struct sockaddr_storage tcpm_addr;
	u_int16_t __tcpm_pad1;
	u_int16_t tcpm_keylen;
	u_int32_t __tcpm_pad2;
	u_int8_t tcpm_key[TCP_MD5SIG_MAXKEYLEN];
};

#endif

#endif
