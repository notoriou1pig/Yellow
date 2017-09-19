// No Header
// 26/11/2015 - 14:00:23

#include "yellow.h"
#include "prototypes.h"

static action	_exec_action(t_net *net)
{
  t_fd		*fd;

  fd = net->head;
  while (fd != NULL && !FD_ISSET(fd->fd, &(net->rfds)))
    fd = fd->next;
  if (fd != NULL)
    {
      FD_CLR(fd->fd, &(net->rfds));
      net->sock = fd->fd;
      return (fd->read_on);
    }
  puts("FIXME!");
  return (NULL);
}

static int	_do_actions(t_packet **p, t_yellow *y, int act)
{
  if (act == 0)
    return (0);
  _exec_action(y->net)(p, y);
  return (_do_actions(p, y, act-1));
}

int		reset_select(t_net *net)
{
  t_fd		*head;
  int		high;

  high = 1;
  head = net->head;
  net->tv.tv_sec = 1;
  net->tv.tv_usec = 0;
  FD_ZERO(&(net->rfds));
  FD_ZERO(&(net->wfds));
  while (head != NULL)
    {
      /* if (head->write_on != NULL) */
      /* 	FD_SET(head->fd, &(net->wfds)); */
      if (head->read_on != NULL)
      	FD_SET(head->fd, &(net->rfds));
      if (head->fd > high)
	high = head->fd;
      head = head->next;
    }
  return (high + 1);
}

int		loop_io(t_packet **p, t_yellow *y)
{
  int		r;

  while (process_cmd(p, y) == 0)
    {      
      if ((r = select(reset_select(y->net),
		      &(y->net->rfds),
		      &(y->net->wfds),
		      NULL,
		      &y->net->tv)) == -1)
	err(ERR_SOCK, "select");
      _do_actions(p, y, r);
      glob_refresh(y->disp);
    }
  return (0);
}
