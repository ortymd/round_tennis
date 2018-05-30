#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib_send_receive.h>

#define EXCLUDE_SEND 0
#define EXCLUDE_RECEIVE 1

typedef struct 
{
  char *data;
  unsigned data_sz;
  char mac_dest[6];
} payload;

void* test_send_data (void *test_payload)
{
#if EXCLUDE_SEND == 0
  int res = 0;
  
  payload *test_payload_ptr = (payload*)test_payload;

  printf("Starting send_data thread.\n"); 
  res = send_data (test_payload_ptr->data, test_payload_ptr->data_sz,
                 test_payload_ptr->mac_dest);
  if (res == -1)
    perror ("Error: send()\t");
  else
    printf ("***Message sent. Sent bytes:\t%d\n", res);
#endif
  return test_payload;
}

void* test_receive (void* buf)
{
#if EXCLUDE_RECEIVE == 0
  int res = 0;
  printf("Starting receive thread.\n"); 
  sleep (3);
  res = receive (buf);
  if (res == -1)
    perror ("Error: receive()\t");
  else
  {
    printf ("***Message received. Received bytes:\t%d\n", res);
    printf ("Data in message:\n%s\n", (char*)buf + ETH_HLEN);   /* print only payload. no header */
  }
#endif
  return buf;
}


int main()
{
  int res = 0;

  char buf[50];

  const int sz = 50;
  char mac_dest[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  payload test_payload;

  test_payload.data = "this is a test messsage";  /* setup test payload */
  test_payload.data_sz = sz;
  memcpy (test_payload.mac_dest, mac_dest, 6);

  pthread_t send_data_thread;
  pthread_t receive_thread;

  res = socket_init();    /* initialize common socket for both threads */
  if (res == -1)
    perror("Error: socket_init()\t");

  res = pthread_create (&send_data_thread, 0, test_send_data, (void*)&test_payload);
  if (res == -1)
    perror("Error: pthread_create()\t");

  res = pthread_create (&receive_thread, 0, test_receive, (void*)buf);
  if (res == -1)
    perror("Error: pthread_create()\t");

  pthread_join (send_data_thread, NULL);
  pthread_join (receive_thread, NULL);

  exit(0);

}
