#include <string.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <stdlib.h>

char *data;

int write_to_buf (char *buf)
{
  printf ("Updating buffer:\ncurrent value:\t%s\n", buf);
  memset (buf, 1, 8);
  printf ("updated value:\t%s\n", buf);
  return 0;
}

int update_ptr_to_payload (char *payload)
{
  int res = 0;
  memset (payload, 1, 5);
  return res;
}

char* malloc_ptr_to_payload (void)
{
  data = malloc(1);
  if (data == NULL)
    printf ("Error malloc(): failed to allocate memory.\n");
  return data;
}

void free_ptr_to_payload (void)
{
  free (data);
  printf ("Payload memory freed.\n");
}
