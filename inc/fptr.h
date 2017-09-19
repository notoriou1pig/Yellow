// No Header
// 16/11/2015 - 14:34:30

#ifndef __FPTR_H_
#define __FPTR_H_

#include "prototypes.h"

typedef void (*func_ptr)(t_packet*,t_yellow*);

static int		eth_idx_fptr[] =
  {
    ETH_P_IP,
    ETH_P_IPV6,
    ETH_P_ARP,
    0
  };

static func_ptr		eth_tab_fptr[] =
  {
    &eth_process_ip,
    &eth_process_ipv6,
    &eth_process_arp,
    &no_op,
    NULL
  };

/* IP Layer */
/* RFC 790 : http://tools.ietf.org/html/rfc790 */
#define IP_P_ICMP	1
#define IP_P_TCP	6
#define IP_P_SECURE	9
#define IP_P_UDP	17

static char		ip_idx_fptr[] =
  {
    IP_P_ICMP,
    IP_P_TCP,
    IP_P_SECURE,
    IP_P_UDP,
    0
  };

static func_ptr		ip_tab_fptr[] =
  {
    &ip_process_icmp,
    &ip_process_tcp,
    &ip_process_secure,
    &ip_process_udp,
    &no_op,
    NULL
  };




#endif /* !FPTR_H__ */
