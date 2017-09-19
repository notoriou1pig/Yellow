// No Header
// 26/11/2015 - 14:49:53

#include "yellow.h"
#include "prototypes.h"

t_fd			*init_fd(int fd, action read_on, action write_on)
{
  t_fd			*head;

  head = xmalloc(sizeof(*head));
  head->fd = fd;
  head->read_on = read_on;
  head->write_on = write_on;
  head->prev = NULL;
  head->next = NULL;
  return (head);
}

static t_net		*_init_net()
{
  t_net			*input;
  t_fd			*head;
  
  input = xmalloc(sizeof(*input)); 
  head = init_fd(0, &read_cli, NULL);
  head->next = init_fd(1, NULL, &write_out);
  head->next->prev = head;
  input->head = head;
  input->rsock = 0;
  return (input);
}

static t_display	*_init_win(int x, int y, int sizex, int sizey)
{
  t_display		*w;

  w = xmalloc(sizeof(*w));
  w->win = newwin(sizex, sizey, x, y);
  w->pos[0] = x;
  w->pos[1] = y;
  w->size[0] = sizex;
  w->size[1] = sizey;
  box(w->win, 0, 0);
  wrefresh(w->win);
  return (w);
}

static t_display	*_init_input_win()
{
  t_display		*w;

  w = _init_win(LINES - 3, 0, 3, COLS);
  w->data = NULL;
  w->display = NULL;
  w->child = NULL;
  w->next = NULL;
  w->prev = NULL;
  return (w);
}

static t_display	*_init_display_win(int type)
{
  t_display		*w;

  if (type == _WOUTPUT)
    w = _init_win(0, 0, LINES - 2, COLS / 2);
  else
    w = _init_win(0, COLS / 2, LINES - 2, COLS / 2);
  w->type = type;
  w->data = NULL;
  w->display = NULL;
  w->child = NULL;
  w->next = NULL;
  w->prev = NULL;
  return (w);
}

t_packet	*get_packet_by_idx(t_packet **p, int idx)
{
  while ((*p)->prev != NULL)
    *p = (*p)->prev;
  while (idx != 0)
    {
      *p = (*p)->next;
      idx--;
    }
  return (*p);
}

void		display_packet(t_packet **p, t_display *d, int idx)
{
  packet_dump(get_packet_by_idx(p, idx), d);
}

t_menu		*init_menu(char *entry, t_display *d)
{
  t_menu		*m;
  int			i;
  WINDOW *tmp;

  m = xmalloc(sizeof(*m));
  m->entrylen = 1;
  m->entry = xmalloc(sizeof(char *) * (m->entrylen + 1));
  m->entry[m->entrylen - 1] = entry;
  m->items = (ITEM **)xmalloc((1024) * sizeof(ITEM *));
  for(i = 0; i < m->entrylen; ++i)
    {
      m->items[i] = new_item(m->entry[i], m->entry[i]);
      set_item_userptr(m->items[i], display_packet);
    }
  m->items[m->entrylen] = (ITEM *)NULL;
  m->menu = new_menu((ITEM **)m->items);
  menu_opts_off(m->menu, O_SHOWDESC);
  set_menu_format(m->menu, LINES - 4, 1);
  set_menu_win(m->menu, d->win);
  tmp = derwin(d->win, 0, 0, 1, 1);
  set_menu_sub(m->menu, tmp);
  box(d->win, 0, 0);
  box(tmp, 0, 0);
  wrefresh(d->win);
  wrefresh(tmp);
  return (m);
}

t_menu		*update_menu(t_menu *m, char *entry, t_display *d)
{
  int		i;
  int		p;

  p = item_index(current_item(m->menu));
  unpost_menu(m->menu);
  m->entrylen++;
  m->entry = xrealloc(m->entry, sizeof(char *) * (m->entrylen + 1));
  m->entry[m->entrylen - 1] = entry;
  i = 0;
  //  puts("toto");
  while (i < m->entrylen)
    {
      m->items[i] = new_item(m->entry[i], m->entry[i]);
      set_item_userptr(m->items[i], display_packet);
      i++;
    }
  m->items[i] = NULL;
  set_menu_items(m->menu, m->items);
  post_menu(m->menu);
  while (p > 0)
    {
      menu_driver(m->menu, REQ_DOWN_ITEM);
      p--;
    }
  
  box(d->win, 0, 0);
  //  box(tmp, 0, 0);
  wrefresh(d->win);
  //  wrefresh(tmp);
  refresh();
  return (m);
}

static t_display	*_init_list_win(int type)
{
  t_display		*w;

  w = _init_display_win(type);
  //  w->data = (void*)_init_menu();
  w->data = NULL;
  w->next = NULL;
  w->prev = NULL;
  return (w);
}

static t_display	*_init_output_win(int type)
{
  t_display		*w;

  w = _init_display_win(type);
  w->next = NULL;
  w->prev = NULL;
  return (w);
}
  
t_display		*init_packet_win(int x, int y, int sizex, int sizey)
{
  t_display		*w;

  w = _init_win(x, y, sizex, sizey);
  w->type = _WPACKET;
  w->data = NULL;
  w->display = strdup("toto");
  w->child = NULL;
  w->next = NULL;
  w->prev = NULL;
  return (w);
}
  
static t_display	**_init_main_child()
{
  t_display		**child;

  child = xmalloc(sizeof(*child) * 4);
  child[0] = _init_list_win(_WOUTPUT);
  child[1] = _init_output_win(_WFORGE);
  child[2] = _init_input_win();
  child[3] = NULL;
  return (child);
}

static t_display	*_init_main_display()
{
  t_display		*d;

  initscr();
  start_color();
  cbreak();
  noecho();  
  keypad(stdscr, TRUE);
  refresh();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
  d = xmalloc(sizeof(*d));
  d->win = NULL;
  d->type = _WMAIN;
  d->pos[0] = 0;
  d->pos[1] = 0;
  d->size[0] = LINES;
  d->size[1] = COLS;
  d->data = NULL;
  d->display = NULL;
  d->child = _init_main_child();
  d->next = NULL;
  d->prev = NULL;
  return (d);
}

static t_opt		*_init_opt()
{
  t_opt			*opt;

  opt = xmalloc(sizeof(*opt)); 
  opt->cmd = xmalloc(sizeof(char) * BUF_LEN);
  bzero(opt->cmd, BUF_LEN);
  opt->argv = NULL;
  opt->r = 0;
  return (opt);
}

int			init(t_yellow *y, int argc, char **argv)
{
  (void)argc;
  (void)argv;
  y->net = _init_net();
  y->disp = _init_main_display();
  y->opt = _init_opt();
  return (0);
}

