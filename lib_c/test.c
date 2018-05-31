#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib_send_receive.h>

#define INCLUDE_SEND 1
#define INCLUDE_RECEIVE 1

typedef struct      /* need to put all here in one structure in order to pass to thread */
{
  char *data;
  unsigned data_sz;
  char mac_dest[6];
} payload;

void* test_send_data (void *test_payload)
{
#if INCLUDE_SEND == 1
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
#if INCLUDE_RECEIVE == 1
  int res = 0;
  printf("Starting receive thread.\n"); 
  sleep (1);
  res = receive (buf);
  if (res == -1)
    perror ("Error: receive()\t");
  else
  {
    printf ("\n***Message received. Received bytes:\t%d\n", res);
    printf ("Data in message:\n%s\n", (char*)buf + ETH_HLEN);   /* print only payload. no header */
  }
#endif
  return buf;
}


int main()
{
  int res = 0;
  char *data = "this is a test messsage";
  unsigned data_sz = strlen (data);

  payload test_payload;                        /* set up payload */
  test_payload.data = malloc (ETH_DATA_LEN);   /* get part of memory that we can modify (append 0 to payload) */
  memcpy (test_payload.data, data, data_sz);
  test_payload.data_sz = data_sz;
  memset (test_payload.mac_dest, 0xff, ETH_ALEN);

  char *buf = malloc (ETH_DATA_LEN);
  memset (buf, 0, ETH_DATA_LEN);

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
  free (test_payload.data);

  socket_close();

  exit(0);
}
