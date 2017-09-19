// No Header
// 26/11/2015 - 06:13:58

#include "yellow.h"
#include "prototypes.h"

void		ip_process_icmp(t_packet *p, t_yellow *y)
{
  struct icmphdr *icmp;

  icmp = (struct icmphdr *)(p->trans_layer);
  (void)icmp;
  (void)y;
  //  print_icmp_header(icmp, y);
}

void		ip_process_tcp(t_packet *p, t_yellow *y)
{
  struct icmphdr *icmp;

  icmp = (struct icmphdr *)(p->trans_layer);
  (void)icmp;
  (void)y;
  //  print_tcp_header((unsigned char *)icmp, y);
}

void		ip_process_secure(t_packet *p, t_yellow *y)
{
  /* write(1, "I'm processing SECURE in IP field\n", */
  /* 	strlen("I'm processing SECURE in IP field\n")); */
 (void)p;
 (void)y;
}

void		ip_process_udp(t_packet *p, t_yellow *y)
{
  /* write(1, "I'm processing UDP in IP field\n", */
  /* 	strlen("I'm processing UDP in IP field\n")); */
  (void)p;
  (void)y;
}
