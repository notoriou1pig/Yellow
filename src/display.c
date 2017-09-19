// No Header
// 26/11/2015 - 14:55:04

#include "yellow.h"
#include "prototypes.h"

int		display(t_yellow *y)
{
  t_packet	*packet;
  
  packet = NULL;
  loop_io(&packet, y);
  release_packet(packet);
  return (0);
}

int		glob_refresh(t_display *disp)
{
  wrefresh(disp->child[2]->win);
  wrefresh(disp->child[0]->win);
  box(disp->child[1]->win, 0, 0);
  wrefresh(disp->child[1]->win);
  refresh();
  return (0);
}

void		print_icmp_header(struct icmphdr *icmp, t_display *d)
{
  mvwprintw(d->win, 17, 2,"ICMP Header\n");
  mvwprintw(d->win, 18, 2,"   |-Type : %d",(unsigned int)(icmp->type));
  if((unsigned int)(icmp->type) == 11)
    mvwprintw(d->win, 19, 1,"  (TTL Expired)\n");
  else if((unsigned int)(icmp->type) == ICMP_ECHOREPLY)
    mvwprintw(d->win, 19, 1,"  (ICMP Echo Reply)\n");
  mvwprintw(d->win, 20, 1,"   |-Code : %d\n",(unsigned int)(icmp->code));
  mvwprintw(d->win, 21, 1,"   |-Checksum : %d\n",ntohs(icmp->checksum));
  mvwprintw(d->win, 22, 1,"\n");
}

void		print_ip_header(struct iphdr *ip, t_display *d)
{
  struct sockaddr_in source,dest;

  source.sin_addr.s_addr = ip->saddr;
  dest.sin_addr.s_addr = ip->daddr;
  mvwprintw(d->win, 7, 1,"IP Version           : %d",
	    (unsigned int)ip->version);
  mvwprintw(d->win, 8, 1,"IP Header Length     : %d DWORDS or %d Bytes",
	    (unsigned int)ip->ihl,
	    ((unsigned int)(ip->ihl))*4);
  mvwprintw(d->win, 9, 1,"IP Type Of Service   : %d\n",
	    (unsigned int)ip->tos);
  mvwprintw(d->win, 10, 1,"IP Total Length      : %d  Bytes(Size of Packet)",
	    ntohs(ip->tot_len));
  mvwprintw(d->win, 11, 1,"IP Identification    : %d",
	    ntohs(ip->id));
  mvwprintw(d->win, 12, 1,"IP TTL               : %d",
	    (unsigned int)ip->ttl);
  mvwprintw(d->win, 13, 1,"IP Protocol          : %d",
	    (unsigned int)ip->protocol);
  mvwprintw(d->win, 14, 1,"IP Checksum          : %d",
	    ntohs(ip->check));
  mvwprintw(d->win, 15, 1,"IP Source IP         : %s",
	    inet_ntoa(source.sin_addr));
  mvwprintw(d->win, 16, 1,"IP Destination IP    : %s",
	    inet_ntoa(dest.sin_addr));
}

void		print_tcp_header(unsigned char* Buffer, t_display *d)
{
  struct tcphdr *tcph=(struct tcphdr*)(Buffer);

  mvwprintw(d->win, 17, 1,"TCP SOURCE %u\n",
	    ntohs(tcph->source));
  mvwprintw(d->win, 18, 1,"TCP Dest Port %u\n",
	    ntohs(tcph->dest));
  if(ntohs(tcph->dest) == 80 || ntohs(tcph->dest) == 8080)
    mvwprintw(d->win, 19, 20,"PORT HTTP");
  if (ntohs(tcph->dest) == 443)
    mvwprintw(d->win, 19, 20,"PORT HTTPS");
  mvwprintw(d->win, 20, 1,"TCP Sequence Number %u\n",
	    ntohl(tcph->seq));
  mvwprintw(d->win, 21, 1,"TCP Acknoledge %u\n",
	    ntohl(tcph->ack_seq));
  mvwprintw(d->win, 22, 1,"TCP header length %d DWORDS or %d BYTES\n" ,
	    (unsigned int)tcph->doff,
	    (unsigned int)tcph->doff*4);
  mvwprintw(d->win, 23, 1,"TCP Urgent Flag %dn",
	    (unsigned int)tcph->urg);
  mvwprintw(d->win, 24, 1,"TCP Ackno flaf %dn",
	    (unsigned int)tcph->ack);
  mvwprintw(d->win, 25, 1,"TCP Push Flag %dn",
	    (unsigned int)tcph->psh);
  mvwprintw(d->win, 26, 1,"TCP Reset Flag %dn",
	    (unsigned int)tcph->rst);
  mvwprintw(d->win, 27, 1,"TCP Sync Flag %dn",
	    (unsigned int)tcph->syn);
  mvwprintw(d->win, 28, 1,"TCP Finish Flag %dn",
	    (unsigned int)tcph->fin);
  mvwprintw(d->win, 29, 1,"TCP Window         : %d\n",ntohs(tcph->window));
  
  mvwprintw(d->win, 30, 1,"TCP Checksum       : %d\n",ntohs(tcph->check));

  mvwprintw(d->win, 31, 1,"TCP Urgent Pointer : %d\n",tcph->urg_ptr);
}

void		print_eth_header(t_ether *ether, t_display *d)
{

  mvwprintw(d->win, 2, 2,"Start Etherned Header\n");
  mvwprintw(d->win, 3, 2,
	    "Dest   ADDR : %02x:%02x:%02x:%02x:%02x:%02x",
	    (unsigned char)*(ether->mac_dest + 0),
	    (unsigned char)*(ether->mac_dest + 1),
	    (unsigned char)*(ether->mac_dest + 2),
	    (unsigned char)*(ether->mac_dest + 3),
	    (unsigned char)*(ether->mac_dest + 4),
	    (unsigned char)*(ether->mac_dest + 5));
  mvwprintw(d->win, 4, 2,
	    "Source ADDR : %02x:%02x:%02x:%02x:%02x:%02x",
	    (unsigned char)*(ether->mac_source + 0),
	    (unsigned char)*(ether->mac_source + 1),
	    (unsigned char)*(ether->mac_source + 2),
	    (unsigned char)*(ether->mac_source + 3),
	    (unsigned char)*(ether->mac_source + 4),
	    (unsigned char)*(ether->mac_source + 5));
  mvwprintw(d->win, 5, 2,"Protocol    : %04x",
  	    (unsigned short)(ether->proto));
  mvwprintw(d->win, 6, 2,"End Ethernet Header");
}
