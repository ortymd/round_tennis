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

#include "lib_send_receiv.h"

extern int socket_fd;
extern struct sockaddr_ll sock_addr;
extern char ether_frame_send [ETH_FRAME_LEN]; 
extern char ether_frame_receive [ETH_FRAME_LEN];
extern *char iface_name;    /* network interface used for connection */

int  initialize_socket()
{
  int res = -1;
  struct ifreq iface_req;

  socket_fd = socket (AF_PACKET, SOCK_RAW, ETHER_TYPE_PACKET);
  if (socket_fd != -1 )
  { 
    memset (&iface_req, 0, sizeof (iface_req));
    memset (&sock_addr, 0, sizeof(SA));
    strncpy( ifreq_temp.ifr_name, iface_name, 5);  /* .TO DO. add option to choose dflt interface */

    res = ioctl (socket_fd, SIOCGIFHWADDR, &iface_req);   /* here we get localhost mac */
    if (res == -1)
    {
      perror("Failed to initialize socket.\nError: ioctl()\t");
      return res;
    }

    memcpy (&sock_addr.sll_addr, &iface_req.ifr_hwaddr.sa_data,
            sizeof (ifreq_temp.ifr_hwaddr.sa_data));

    res = ioctl (socket_fd, SIOCGIFINDEX. &iface_req);   /* here we get device interface index */
    if (res == -1)
    {
      perror("Failed to initialize socket.\nError: ioctl()\t");
      return res;
    }

    memcpy (&sock_addr.sll_ifindex, &iface_req.ifr_ifindex,
            sizeof (iface_req,ifr_ifindex)); 

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
int send (char *data, size_t data_sz, char *mac_destination)
{
    if ( data_for_send && mac_destination && size_data_send )
       {
            if ( (size_data_send < (ETH_ZLEN - ETH_HLEN)) || (size_data_send  >  ETH_DATA_LEN) )
       	           errno  = EINVAL;
               else  
       	           {
       	               struct ethhdr * frame  = (struct ethhdr *)(ether_frame_send);
       	               memcpy(frame->h_dest, mac_destination, ETH_ALEN); /* set mac destination */
       	               memcpy(frame->h_source, socket_address.sll_addr, ETH_ALEN); /* set my mac address  */
       	               frame->h_proto =  htons(ETHER_TYPE_PACKET);               /* set type protocol */
                       memcpy(ether_frame_send + sizeof( struct ethhdr), data_for_send, size_data_send); /* copy data into buffer_send */
                       sendto(descriptor_socket,
                              ether_frame_send,  
                              size_data_send + ETH_HLEN,
                              0,
                             (struct sockaddr*)&socket_address, 
                             sizeof(struct sockaddr_ll));
       	           }
       }
    return errno;
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
    memcpy(buf, ether_frame_receive, size_frame_receiv);
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


