#define _GNU_SOURCE     /* for syscall() interface */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib_send_receive.h>

#include <unistd.h>
#include <sys/syscall.h>


#define INCLUDE_SEND 1
#define INCLUDE_RECEIVE 1

typedef struct      /* need to put all here in one structure in order to pass to thread */
{
  char *payload;
  unsigned payload_sz;
  char mac_dest[6];
} frame;

void* test_send_payload (void *test_frame)
{
#if INCLUDE_SEND == 1
  int res = 0;
  
  frame *test_frame_ptr = (frame*)test_frame;

  printf("Starting send_payload thread.\n"); 
  for (unsigned i = 0; i < 10; ++i)
  {
    res = send_payload (test_frame_ptr->payload, test_frame_ptr->payload_sz,
                    test_frame_ptr->mac_dest);
    if (res == -1)
      perror ("Error: send()\t");
    else
      printf ("***Message sent. ID:\t%u Sent bytes:\t%d\n", i, res);
  }
#endif
  return test_frame;
}

void* test_receive (void* buf)
{
#if INCLUDE_RECEIVE == 1
  int res = 0;
  int num_of_messages = 0;
  printf("Starting receive thread.\n"); 

  while (1)
  {
    res = receive (buf);

    if (res == -1)
      perror ("Error: receive()\t");
    else
    {
      printf ("\n***Message received. Received bytes:\t%d\n", res);
      printf ("Data in message:\n%s\n", (char*)buf + ETH_HLEN);   /* print only payload. no header */
      ++num_of_messages;
      printf ("\nThread ID:\t%ld\nTotal received:\t%d\n",
              syscall(__NR_gettid), num_of_messages);
    }
  }
#endif
  return buf;
}


int main()
{
  int res = 0;
  char *payload = "this is a test messsage";
  unsigned payload_sz = strlen (payload);

  frame test_frame;                        /* set up payload */
  test_frame.payload = malloc (ETH_DATA_LEN);   /* get part of memory that we can modify (append 0 to payload) */
  memcpy (test_frame.payload, payload, payload_sz);
  test_frame.payload_sz = payload_sz;
  memset (test_frame.mac_dest, 0xff, ETH_ALEN);

  char *buf = malloc (ETH_DATA_LEN);
  memset (buf, 0, ETH_DATA_LEN);

  pthread_t send_payload_thread;
  pthread_t receive_thread_1;
  pthread_t receive_thread_2;

  res = socket_init();    /* initialize common socket for both threads */
  if (res == -1)
    perror("Error: socket_init()\t");

  res = pthread_create (&send_payload_thread, 0, test_send_payload, (void*)&test_frame);
  if (res == -1)
    perror("Error: pthread_create()\t");

  res = pthread_create (&receive_thread_1, 0, test_receive, (void*)buf);
  if (res == -1)
    perror("Error: pthread_create()\t");

  res = pthread_create (&receive_thread_2, 0, test_receive, (void*)buf);
  if (res == -1)
    perror("Error: pthread_create()\t");

  pthread_join (send_payload_thread, NULL);
  pthread_join (receive_thread_1, NULL);
  pthread_join (receive_thread_2, NULL);

  socket_close();

  exit(0);
}
