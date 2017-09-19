// No Header
// 26/11/2015 - 12:55:59

#include "yellow.h"
#include "prototypes.h"

static void	_destroy_display(t_display *);
static void	_destroy_child(t_display **c)
{
  int		i;

  i = 0;
  if (c == NULL)
    return ;
  while (c[i] != NULL)
    {
      _destroy_display(c[i]);
      i++;
    }
}

static void	_destroy_win(WINDOW *w)
{
  if (w != NULL)
    delwin(w);
}

static void	_destroy_data(t_display *data)
{
  /* if (data->type > _WINPUT) */
  /*   free(data); */
  (void)data;
}

static void	_destroy_display(t_display *d)
{
  t_display	*tmp;

  endwin();
  while (d != NULL)
    {
      tmp = d->next;
      _destroy_child(d->child);
      _destroy_data(d);
      _destroy_win(d->win);
      free(d);
      d = tmp;
    }
}

void		destroy_fd(t_net *net, t_fd *head)
{
  if (head->prev != NULL)
    head->prev->next = head->next;
  if (head->next != NULL)
    head->next->prev = head->prev;
  FD_CLR(head->fd, &(net->rfds));
  FD_CLR(head->fd, &(net->wfds));
  //  printf("%d\n", head->fd);
  if (head->fd > 1)
    unset_promiscuous(head->fd);
  close(head->fd);
  free(head);
}

static void	_destroy_net(t_net *net)
{
  t_fd		*tmp;

  while (net->head != NULL)
    {
      tmp = net->head;
      net->head = net->head->next;
      destroy_fd(net, tmp);
    }
  free(net);
}

int		destroy_the_yell(t_yellow *y)
{
  _destroy_display(y->disp);
  _destroy_net(y->net);
  /* _destroy_opt(y->opt); */
  return (0);
}
