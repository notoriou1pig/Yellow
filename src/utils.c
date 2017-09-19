// No Header
// 26/11/2015 - 05:08:06

#include "prototypes.h"

void		*xmalloc(size_t size)
{
  void		*data;

  if ((data = malloc(size)) == NULL)
    err(ERR_MALLOC, "malloc");
  return (data);
}

void		*xrealloc(void *ptr, size_t size)
{
  if ((ptr = realloc(ptr, size)) == NULL)
    err(ERR_MALLOC, "realloc");
  return (ptr);
}

char		*xstrdup(char *s)
{
  char		*str;

  if ((str = strdup(s)) == NULL)
    err(ERR_MALLOC, "strdup");
  return (str);
}
