// No Header
// 26/11/2015 - 14:58:10

#include "yellow.h"
#include "prototypes.h"
//#include "fptr.h"

t_opt		*set_opt(t_opt *opt, int c, char **tab)
{
  (void)c;
  (void)tab;
  return (opt);
}

int		release_packet(t_packet *p)
{
  t_packet	*tmp;

  if (p == NULL)
    return (0);
  while (p->prev != NULL)
    p = p->prev;
  while (p != NULL)
    {
      tmp = p;
      p = p->next;
      free(tmp->info);
      free(tmp->data);
      free(tmp);
    }
  return (0);
}

void		process_dump(t_packet *p, t_display *d)
{
  print_eth_header(p->datalink_layer, d);
  if (p->datalink_layer->proto == 0x0800)
    print_ip_header(p->net_layer, d);
  if (((struct iphdr *)p->net_layer)->protocol == 6)
    print_tcp_header(p->trans_layer, d);
  if (((struct iphdr *)p->net_layer)->protocol == 1)
    print_icmp_header(p->trans_layer, d);
}

int		packet_dump(t_packet *p, t_display *d)
{
  if (p != NULL)
    process_dump(p, d);
  return (0);
}

void		no_op(t_packet *p, t_yellow*y)
{
  (void)p;
  (void)y;
  //  write(1, "no op\n", 6);
}

t_display		*init_packet_win(int x, int y, int sizex, int sizey);
t_menu		*update_menu(t_menu *m, char *entry, t_display *);
t_menu		*init_menu(char *entry, t_display*);
void			add_packet_child(t_packet *p, t_display *d)
{
  static int	size = 2;
  char		*tmp;
  
  tmp = xmalloc(sizeof(char) * (COLS / 2));
  snprintf(tmp, (COLS / 2),
	   "Ethernet Frame %d", size - 1);
  size++;
  if (d->data == NULL)
    d->data = (void*)init_menu(tmp, d);
  else
    d->data = (void*)update_menu((t_menu*)d->data, tmp, d);
  (void)p;
}

void		register_packet(t_packet *p, t_yellow *y)
{
  /* (void)p; */
  /* (void)y; */
   add_packet_child(p, y->disp->child[0]);
  //  print_eth_header(p->datalink_layer, y);
  //  print_ip_header(ip, y);
}

void		data_process(t_packet *p, t_yellow *y)
{
  p->datalink_layer = p->data;
#if __BYTE_ORDER == __LITTLE_ENDIAN
  p->datalink_layer->type.low = p->datalink_layer->type.low +
    p->datalink_layer->type.high;
  p->datalink_layer->type.high = p->datalink_layer->type.low -
    p->datalink_layer->type.high;
  p->datalink_layer->type.low = p->datalink_layer->type.low -
    p->datalink_layer->type.high;
#endif 
  p->net_layer = p->data + sizeof(*p->datalink_layer);
  process_ether(p, y);
}

int		packet_process(t_packet **p, t_yellow *y, void *data, int size)
{
  if (*p == NULL)
    { 
      *p = xmalloc(sizeof(**p));
      (*p)->prev = NULL;
      (*p)->next = NULL;
    }
  else
    {
      (*p)->next = xmalloc(sizeof(**p));
      (*p)->next->prev = (*p);
      (*p) = (*p)->next;
      (*p)->next = NULL;
    }
  (*p)->info = xmalloc(sizeof(*(*p)->info));
  (*p)->data = xmalloc(ETH_FRAME_LEN);
  (*p)->size = size;
  (*p)->data = memcpy((*p)->data, data, ETH_FRAME_LEN);
  gettimeofday(&(*p)->ts, NULL);
  data_process(*p, y);
  register_packet(*p, y);
  return (0);
}

int		read_cli(t_packet **p, t_yellow *sock)
{
  static int	i = 0;
  int		buf;
  
  buf = getch();
  sock->opt->cmd[i] = buf;
  if (buf == 263)
    {
      sock->opt->cmd[i] = 0;
      mvwprintw(sock->disp->child[2]->win, 1, i," ");
      if (i > 0)
	i = i - 2;
      else
	i = -1;
    }
  else if (buf == '\n')
    {
      sock->opt->cmd[i] = 0;
      while (i > 0)
	mvwprintw(sock->disp->child[2]->win, 1, i--," ");
      i = -1;
      sock->opt->r = 1;
    }
  else if (buf == KEY_DOWN)
    {
      sock->opt->cmd[i] = 0;
      menu_driver(((t_menu*)sock->disp->child[0]->data)->menu, REQ_DOWN_ITEM);
      i--;
    }
  else if (buf == KEY_UP)
    {
      sock->opt->cmd[i] = 0;
      menu_driver(((t_menu*)sock->disp->child[0]->data)->menu, REQ_UP_ITEM);
      i--;
    }
  else if (buf == ' ')
    {
      sock->opt->cmd[i] = 0;
      i--;
    }
  else
    mvwprintw(sock->disp->child[2]->win, 1, 1,"%s", sock->opt->cmd);
  //  mvwprintw(sock->disp->child[2]->win, 1, 1,"%d", buf);
  i++;
  (void)p;
  return(0);
}

int		write_out(t_packet **p, t_yellow *y)
{
  (void)p;
  glob_refresh(y->disp);
  return(1);
}

int		exit_func(t_packet **p, t_yellow *y)
{
  (void)p;
  (void)y;
  return (1);
}

int		display_func(t_packet **p, t_yellow *y)
{
  void (*ptr)();

  werase(y->disp->child[1]->win);
  ptr = item_userptr(current_item(((t_menu*)y->disp->child[0]->data)->menu));
  ptr(p, y->disp->child[1], item_index(current_item(((t_menu*)y->disp->child[0]->data)->menu)));
  return (0);
}

int		start_func(t_packet **p, t_yellow *y)
{
  y->net->sock = get_sock();
  y->net->rsock =  y->net->sock;
  /* y->net->sock = get_sock("eth0");  */
  y->net = register_sock(y->net, y->net->sock);
  (void)p;
  return (0);
}

int		stop_func(t_packet **p, t_yellow *y)
{
  //  printf("%d\n", y->net->sock);
  if (y->net->rsock < 2)
    return (1);
  unregister_sock(y->net, y->net->rsock);
  /* y->net = register_sock(y->net, y->net->sock); */
  (void)p;
  return (0);
}

int		export_func(t_packet **p, t_yellow *y)
{
  pcap_export("yellow.export.pcap", *p);
  (void)y;
  return (0);
}

int		help_func(t_packet **p, t_yellow *y)
{
  (void)p;
  werase(y->disp->child[1]->win);
  box(y->disp->child[1]->win, 0, 0);
  mvwprintw(y->disp->child[1]->win, 1, 2,"/start   : start capture on any device\n");
  mvwprintw(y->disp->child[1]->win, 2, 2,"/stop    : stop capture\n");
  mvwprintw(y->disp->child[1]->win, 3, 2,"/export  : export packets to yellow.export.pcap\n");
  mvwprintw(y->disp->child[1]->win, 4, 2,"/display : display selected packet information\n");
  mvwprintw(y->disp->child[1]->win, 5, 2,"/exit    : exit program\n");
  mvwprintw(y->disp->child[1]->win, 6, 2,"/help    : display this help\n");

  return (0);
}

int		no_cli(t_packet **p, t_yellow *y)
{
  (void)p;
  (void)y;
  return (0);
}

typedef int (*cli_ptr)(t_packet**,t_yellow*);

static char		*cli_idx_fptr[] =
  {
    "/exit",
    "/display",
    "/start",
    "/stop",
    "/export",
    "/help",
    NULL
  };

static cli_ptr		cli_tab_fptr[] =
  {
    &exit_func,
    &display_func,
    &start_func,
    &stop_func,
    &export_func,
    &help_func,
    &no_cli,
    NULL
  };

int		process_cmd(t_packet **p, t_yellow *y)
{
  int		ret = 0;
  int		i = 0;

  (void)p;
  if (y->opt->r != 1)
    return (0);
  y->opt->r = 0;
  while (cli_idx_fptr[i] != NULL && strcmp(y->opt->cmd, cli_idx_fptr[i]) != 0)
    i++;
  ret = cli_tab_fptr[i](p, y);
  bzero(y->opt->cmd, BUF_LEN);
  return (ret);
}
