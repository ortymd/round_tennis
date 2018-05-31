#include <string.h>
#include <stdio.h>

int write_to_buf (char *buf)
{
  printf ("Updating buffer:\ncurrent value:\t%s\n", buf);
  memset (buf, 1, 8);
  printf ("updated value:\t%s\n", buf);
  return 0;
}
