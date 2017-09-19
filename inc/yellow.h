// No Header
// 26/11/2015 - 14:49:51

#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#define CTRLD 4

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <ncurses.h>
#include <curses.h>
#include <menu.h>

#include <netdb.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>

#include <pcap/pcap.h>

#include <linux/icmp.h>

#include <arpa/inet.h>

struct pcap_sf {
  FILE *rfile;
  int swapped;
  int hdrsize;
  int version_major;
  int version_minor;
  u_char *base;
};

struct pcap_md {
  struct pcap_stat stat;
  /*XXX*/
  int use_bpf;    /* using kernel filter */
  u_long  TotPkts;  /* can't oflow for 79 hrs on ether */
  u_long  TotAccepted;  /* count accepted by filter */
  u_long  TotDrops; /* count of dropped packets */
  long  TotMissed;  /* missed by i/f during this run */
  long  OrigMissed; /* missed by i/f before this run */
#ifdef linux
  int sock_packet;  /* using Linux 2.0 compatible interface */
  int timeout;  /* timeout specified to pcap_open_live */
  int clear_promisc;  /* must clear promiscuous mode when we close */
  int cooked;   /* using SOCK_DGRAM rather than SOCK_RAW */
  int lo_ifindex; /* interface index of the loopback device */
  char  *device;  /* device name */
  struct pcap *next;  /* list of open promiscuous sock_packet pcaps */
#endif
};

struct pcap {
  int fd;
  int snapshot;
  int linktype;
  int tzoff;    /* timezone offset */
  int offset;   /* offset for proper alignment */

  struct pcap_sf sf;
  struct pcap_md md;

  /*
   * Read buffer.
   */
  int bufsize;
  u_char *buffer;
  u_char *bp;
  int cc;

  /*
   * Place holder for pcap_next().
   */
  u_char *pkt;

  
  /*
   * Placeholder for filter code if bpf not in kernel.
   */
  struct bpf_program fcode;

  char errbuf[PCAP_ERRBUF_SIZE];
};

typedef struct	s_inpt
{
  int		pos[2];
  char		*name;
  char		*buffer;
  int		buflen;
  int		index;
  struct s_inpt	*next;
  struct s_inpt	*prev;
}		t_input;

typedef struct	s_output
{
  int		pos[2];
  char		*data;
  int		idx_x;
  int		idx_y;
}		t_output;

typedef struct	s_form
{
  t_input	**input;
  int		intputlen;
}		t_form;

typedef struct	s_menu
{
  ITEM		**items;
  MENU		*menu;
  char		**entry;
  int		entrylen;
}		t_menu;

/* 
** Type :
** 0xD15 = Display
**
*/

/* typedef struct	s_win */
/* { */
/*   WINDOW	*win; */
/*   int		type; */
/*   int		pos[2]; */
/*   void		*display; */
/*   struct s_win	**child; */
/*   struct s_win	*next; */
/*   struct s_win	*prev; */
/* }		t_window; */

#define _WMAIN		0
#define _WINPUT		1
#define _WMENU		2
#define _WOUTPUT	3
#define _WFORGE		4
#define _WPACKET	5

typedef struct	s_win
{
  WINDOW	*win;
  int		type;
  int		pos[2];
  int		size[2];
  void		*data;
  void		*display;
  struct s_win	**child;
  struct s_win	*next;
  struct s_win	*prev;
}		t_display;

/* typedef struct	s_display */
/* { */
/*   t_window	**output; */
/*   t_window	*eff_o; */
/*   t_window	*input; */
/*   t_window	*menu; */
/* }		t_display; */

#endif /*! __DISPLAY_H_ */

#ifndef YELLOW_H__
#define YELLOW_H__

/* Header Ethernet */
typedef struct		s_ethernet {
  unsigned char		mac_dest[6];
  unsigned char		mac_source[6];
  union {
    struct {
      unsigned char	high;
      unsigned char	low;
    }			type;
    unsigned short	proto;
  };
}			t_ether;

/* Infos */
typedef struct		s_info {
  int			proto[3];
}			t_info;

typedef	struct		s_pac {
  
  t_info		*info; 
  struct s_pac		*prev;
  struct s_pac		*next;

  struct timeval  ts;

  /* Header IP */
  t_ether		*datalink_layer;

  void			*net_layer;
  void			*trans_layer;
  void			*app_layer;

  ssize_t		size;
  void			*data;
}			t_packet;

typedef struct s_misc t_yellow;
typedef int	(*action)(t_packet **, t_yellow *);

typedef struct		s_opt {
  char			*cmd;
  char			**argv;
  int			r;
}			t_opt;

typedef struct		s_fd {
  int			fd;
  action		read_on;
  action		write_on;
  struct s_fd		*prev;
  struct s_fd		*next;
}			t_fd;

typedef struct		s_net {
  t_fd			*head;
  fd_set		rfds;
  fd_set		wfds;
  int			sock;
  int			rsock;
  struct timeval        tv;
}			t_net;

typedef struct		s_misc {
  t_display		*disp;
  t_opt			*opt;
  t_net			*net;
}			t_yellow;

#define SIZE_BUFF ETH_FRAME_LEN

#define ERR_SOCK 2
#define ERR_MALLOC 3
#define ERR_RECV 4

//#define BUF_LEN	(COLS - 4)
#define BUF_LEN 255
#define TAB_LEN	5

#endif	/* !YELLOW_H__ */
