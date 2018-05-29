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

extern int socket_fd;
extern SA_LL sock_addr;
extern char ether_frame_send [ETH_FRAME_LEN]; 
extern char ether_frame_receive [ETH_FRAME_LEN];
char *iface_name = "enps30";     /* network interface used for connection */

int socket_init()
{
  int res = -1;
  struct ifreq iface_req;   /* interface request */

  socket_fd = socket (AF_PACKET, SOCK_RAW, ETHER_TYPE_PACKET);
  if (socket_fd != -1 )
  { 
    memset (&iface_req, 0, sizeof (iface_req));
    memset (&sock_addr, 0, sizeof(SA_LL));
    strncpy( iface_req.ifr_name, iface_name, sizeof (iface_req));  /* .TO DO. add option to choose dflt interface */

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
  else
  {
    perror("Error: socket()\t");
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
int send_data (char *data, size_t data_sz, char *mac_destination)
{
  int res = 0;
  if (data && mac_destination && data_sz)
  {
    if ((data_sz < (ETH_ZLEN - ETH_HLEN)) || (data_sz  >  ETH_DATA_LEN) )
    {
      fprintf (stderr, "Invalid size of data_frame! Cannot send.");
      res = EINVAL;
    }
    else  
    {
        struct ethhdr * frame  = (struct ethhdr *)(ether_frame_send);
        memcpy(frame->h_dest, mac_destination, ETH_ALEN);         /* set mac destination */
        memcpy(frame->h_source, sock_addr.sll_addr, ETH_ALEN);    /* set my mac address  */
        frame->h_proto =  htons(ETHER_TYPE_PACKET);               /* set type protocol */
        memcpy (frame + sizeof( struct ethhdr), data, data_sz);   /* copy data into buffer_send */

        res = sendto (socket_fd, frame,  data_sz + ETH_HLEN, 0,   /* no flags specified */
                     (SA*)&sock_addr, sizeof(SA_LL));
        if (res == -1)
          perror("Error: sendto()\t");
    }
  }
    return res;
}

int receive (char *buf)  /* copy received frame into buffer  */
{  
  size_t res = -1;
  if (buf == NULL)
  {
    fprintf (stderr, "Buffer set to 0x0. Cannot copy to buffer.\n");
    return res;
  }

  res = recv (socket_fd, ether_frame_receive, ETH_DATA_LEN, 0); 

  if (res == -1)
  {
    perror ("Error: recv()\t");
  }
  else
  {
    memcpy(buf, ether_frame_receive, res);  /* copy number of bytes 
                                            that were succesfully received */
  }

  return res;
 
}

void close_socket()
{
    close (socket_fd);
}

/*
  return str error human readable
*/
const char * str_error()
{
    return strerror( errno ); 
}
