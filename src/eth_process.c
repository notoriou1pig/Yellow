// No Header
// 26/11/2015 - 06:13:10

#include "yellow.h"
#include "fptr.h"

void		eth_process_ip(t_packet *p, t_yellow *y)
{
  struct iphdr	*ip;
  int	i;

  i = 0; 
  ip = (struct iphdr*)p->net_layer;
  //  print_ip_header(ip, y);
  p->info->proto[1] = ip->protocol;
  p->trans_layer = p->net_layer + sizeof(struct iphdr);
  while (ip_idx_fptr[i] != 0 && ip->protocol != ip_idx_fptr[i])
    i++;
  ip_tab_fptr[i](p, y);
}

void		eth_process_ipv6(t_packet *p, t_yellow *y)
{
  /* write(1, "I'm processing IPV6 in ETH field\n", */
  /* 	strlen("I'm processing IPV6 in ETH field\n")); */
  (void)p;
  (void)y;
}

void		eth_process_arp(t_packet *p,  t_yellow *y)
{
  /* write(1, "I'm processing ARP in ETH field\n", */
  /* 	strlen("I'm processing ARP in ETH field\n")); */
  (void)p;
  (void)y;
}

t_packet	*process_ether(t_packet *p, t_yellow *y)
{
  int	i;

  i = 0; 
  p->info->proto[0] = p->datalink_layer->proto;
  p->net_layer = p->data + sizeof(*p->datalink_layer);
  while (eth_idx_fptr[i] != 0 && p->datalink_layer->proto != eth_idx_fptr[i])
    i++;
  eth_tab_fptr[i](p, y);
  /* process_layer2(p); */
  return (p);
}
