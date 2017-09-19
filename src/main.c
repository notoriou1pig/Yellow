// No Header
// 11/04/2017 - 16:08:35

#include "yellow.h"
#include "prototypes.h"

int	main(int argc, char **argv)
{
  t_yellow	y;

  init(&y, argc, argv);
  refresh();
  display(&y);
  destroy_the_yell(&y);
  return (0);
}
