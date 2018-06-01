/*
   this file allows to send and receive DATA  on protocol  Level 2 
   example
   const int size = 100
   char buffer_data[size] ; where size >= (64 - 14) && size <= 1500
   char mac_address[6] = {0xff,0xff,0xff,0xff,0xff,0xff }; broadcast
   first call initialize_socket(); 

    if returned  value 0 call finished fine
    else retun state error and it is same that errno
    or you can call func str_error() and get string value error
  socket_send(buffer_data, sizeof(buffer_data), mac);
    if error return value != 0
  cahr buff_receive[size];
  socket_receive(buff_receive);
    buffer size must be enought to get frame 
    other  unbehavior
  close_socket();
*/

#include "lib_send_receive.h"
int  socket_fd;
SA_LL sock_addr;
char ether_frame_send [ETH_FRAME_LEN]; 
char ether_frame_receive [ETH_FRAME_LEN];


#define TEST_INTERFACE 0    /* choose here which interface to test */
#if TEST_INTERFACE == 0
char *iface_name = "lo";     /* network interface used for connection */
#elif TEST_INTERFACE == 1
char *iface_name = "vboxnet2";     /* network interface used for connection */
#elif TEST_INTERFACE == 2
char *iface_name = "eth2";     /* network interface used for connection */
#elif TEST_INTERFACE == 3
char *iface_name = "enp3s0";     /* network interface used for connection */
#endif

int socket_init()
{
  int res = -1;
  struct ifreq iface_req;   /* interface request */

  socket_fd = socket (AF_PACKET, SOCK_RAW, ETHER_TYPE_PACKET);
  if (socket_fd == -1 )
  {
    perror("Error: socket_init()\t");       /* end program if failed to init socket */
    exit (res);
  }
  else
  { 
    memset (&iface_req, 0, sizeof (iface_req));
    memset (&sock_addr, 0, sizeof(SA_LL));
    strncpy( iface_req.ifr_name, iface_name, sizeof (iface_req.ifr_name));  /* .TO DO. add option to choose dflt interface */

    res = ioctl (socket_fd, SIOCGIFHWADDR, &iface_req);   /* here we get localhost mac */
    if (res == -1)
    {
      perror("Failed to initialize socket.\nError: ioctl()\t");
      return res;
    }

    memcpy (&sock_addr.sll_addr, &iface_req.ifr_hwaddr.sa_data,
            sizeof (iface_req.ifr_hwaddr.sa_data));

    res = ioctl (socket_fd, SIOCGIFINDEX, &iface_req);   /* here we get device interface index */
    if (res == -1)
    {
      perror("Failed to initialize socket.\nError: ioctl()\t");
      return res;
    }

    memcpy (&sock_addr.sll_ifindex, &iface_req.ifr_ifindex,
            sizeof (iface_req.ifr_ifindex)); 
    sock_addr.sll_halen = ETH_ALEN;

    return res; 
  }
}

/*
   data for send must be more ((60 min ether_frame) - (14 head ether_frame)) => 46 byte
   copy data from data into ether_frame_send
   set mac address destination
   set mac address source
   set EtherType
   sendto()                                
*/
int send_data (const char *data, unsigned data_sz, const char *mac_dest)
{
  int res = 0;
  unsigned total_sz = ETH_HLEN;     /* this is total size of frame (header + payload) */
  EH *frame_hdr  = (EH*)(ether_frame_send); /* just for convenience */
                                                
  if (data_sz > ETH_DATA_LEN)            /* if payload is larger then allowed */
  {
    fprintf (stderr, "Size of data > ETH_DATA_LEN! Cannot send.\n");
    res = -1;
  }
  else
  {
    memcpy (frame_hdr->h_dest, mac_dest, ETH_ALEN);         /* put mac dest to header */
    memcpy (frame_hdr->h_source, sock_addr.sll_addr, ETH_ALEN);    /* set my mac address  */
    frame_hdr->h_proto =  ETHER_TYPE_PACKET;               /* set type protocol */

    memcpy (frame_hdr + 1, data, data_sz);   /* copy data into buffer_send. 
                                                  we add 1 as frame_hdr is of size ETH_HLEN, thus adding 1
                                                  actually increases address by ETH_HLEN bytes (14) */
    if (data_sz < ETH_ZLEN)                 /* here we check min octests in frame */
    {
      unsigned append_sz = ETH_ZLEN - data_sz;  /* store how many zeros to append to data */
      const char *append_offset = data + data_sz;     /* store offset, where zeros are appended */

      memset ((void*)append_offset, 0, append_sz);     /* append zeros to data */
      total_sz += append_sz;                    /* add appended zeros to total frame sz */
    }

    res = sendto (socket_fd, frame_hdr, total_sz, 0,   /* no flags specified */
                 (SA*)&sock_addr, sizeof(SA_LL));
    if (res == -1)
      perror("Error: sendto()\t");
  }
  memset ((void*)data, 0, data_sz);        /* clear data from frame no matter
                                            whether it was sent or not */
  return res;
}

int discover_server (void)    /* broadcast message with request for game server */
{
    int res = 0;
    const char *const data = "SERVER_DISCOVERY";
    const char const mac_dest [ETH_ALEN] = {
                0xff,0xff,0xff,0xff,0xff,0xff   /* use broadcast address */
                }; 
    unsigned data_sz = strlen (data);
    res = send_data (data, data_sz, mac_dest);
    if (res == -1)
      perror("Error: discover_server()\t");
    return res;
}

int receive (char *buf)                     /* copy received frame into buffer  */
{  
  int res = -1;
  if (buf == NULL)
  {
    fprintf (stderr, "Buffer set to 0x0. Cannot copy to buffer.\n");
    return res;
  }

  printf ("Receiving data...\n");
  res = recv (socket_fd, ether_frame_receive, ETH_DATA_LEN, 0); 
  printf ("Succesfully received:\t%d bytes\n", res);

  if (res == -1)
  {
    perror ("Error: recv()\t");
  }
  else
  {
    memcpy(buf, ether_frame_receive, res);  /* copy number of bytes 
                                            that were successfully received */
  }

  return res;
 
}

void socket_close (void)
{
    close (socket_fd);
}

const char *str_error (void) /* return str error human readable */
{
    return strerror (errno); 
}
