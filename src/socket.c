// No Header
// 26/11/2015 - 13:30:30

#include "yellow.h"
#include "prototypes.h"

//man netdevice !! IOCTL

static int	_set_promiscuous(int sock)
{
  struct ifreq	ifr;

  bzero(&ifr, sizeof(struct ifreq));
  //  strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ);
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
  if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
    perror("ioctl SIOCGIFINDEX");
    exit(1);
  }
  ifr.ifr_flags &= ~IFF_PROMISC;
  ifr.ifr_flags |= IFF_UP;
  if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
    perror("ioctl SIOCSIFINDEX");
    exit(1);
  }
  return (sock);
}

int		unset_promiscuous(int sock)
{
  struct ifreq	ifr;

  bzero(&ifr, sizeof(struct ifreq));
  //  strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ);
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
  if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
    perror("ioctl SIOCGIFINDEX");
    exit(1);
  }
  ifr.ifr_flags &= ~IFF_PROMISC;
  ifr.ifr_flags |= IFF_UP;
  if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
    perror("ioctl SIOCSIFINDEX");
    exit(1);
  }
  if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, NULL , 0) != 0)
    {
      perror("setsockopt");
      exit(1);
    }
  return (sock);
}

int		get_interface(int sock)
{
  struct ifconf conf;
  int		c;
  int		i;
  
  memset(&conf, 0, sizeof(conf));
  conf.ifc_len = sizeof(struct ifreq) * 10;
  conf.ifc_buf = (char*)xmalloc(conf.ifc_len);
  
  if (ioctl(sock, SIOCGIFCONF, &conf) == -1) {
    perror("ioctl SIOCSIFCONF");
    exit(1);
  }
  
  c = conf.ifc_len / sizeof(struct ifreq);
  i = 0;

  while (i < c)
    {
      if (setsockopt(sock,
		     SOL_SOCKET,
		     SO_BINDTODEVICE,
		     conf.ifc_req[i].ifr_name,
		     strlen(conf.ifc_req[i].ifr_name)+1) != 0)
	err(ERR_SOCK, "setsockopt");
      i++;
    }
  free(conf.ifc_buf);
  return (sock);
}

int		get_sock()
{
  int		sock;
  
  if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
    err(ERR_SOCK, "socket");
  sock = get_interface(sock);
  sock = _set_promiscuous(sock);

  return (sock);
}

int send_packet(t_packet **p, t_yellow *sock) {(void)p;(void)sock;return(1);}

int		recv_packet(t_packet **p, t_yellow *y)
{
  void		*data;
  int		size;
  static int	nbr_pac = 0;

  data = xmalloc(ETH_FRAME_LEN);
  mvwprintw(y->disp->child[0]->win, 40, 1,"Nbr Packet : %d\n", nbr_pac);
  if ((size = recvfrom(y->net->sock, data, ETH_FRAME_LEN, 0,	\
		       NULL, NULL)) == -1)
    err(ERR_RECV, "recvfrom");
  packet_process(p, y, data, size);
  nbr_pac++;
  return (0);
}

t_net		*register_sock(t_net *net, int fd)
{
  t_fd		*new_fd; 
  t_fd		*head;

  head = net->head;
  new_fd = init_fd(fd, &recv_packet, &send_packet);
  if (head->next != NULL)
    head->next->prev = new_fd;
  new_fd->next = head->next;
  new_fd->prev = head;
  head->next = new_fd;
  FD_SET(fd, &(net->rfds));
  FD_SET(fd, &(net->wfds));
  return (net);
}

void		unregister_sock(t_net *net, int fd)
{
  t_fd		*head;

  head = net->head;
  while (head->next != NULL && head->fd != fd)
    head = head->next;
  if (head->fd == fd)
    destroy_fd(net, head);
}
