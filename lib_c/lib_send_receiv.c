/* mem... */
#include <string.h> 


/* socket */
#include <sys/socket.h>
//#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>








#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* for socket */
#define ETHER_TYPE_PACKET 0x5555
#define SIZE_HEADER       0x0010  /* 16  */
#define SIZE_PACKET       0x0064  /* 100 */
#define ETH_ALEN               6  /* Octets in one ethernet addr */

static int                       id_socket;
static struct   sockaddr_ll socket_address;






int initialization_socket();
int socket_sent();
int socket_reciev();
const char * str_error();


/* return str error human readable */
const char * str_error()
{
  return strerror( errno ); 
}


/* return status errno if 0 success */
int  initialization_socket()
{

if ( ( id_socket = socket ( AF_PACKET, SOCK_RAW, htons( ETHER_TYPE_PACKET ))) != -1 )
   {
     struct   ifreq ifreq_temp;
     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
      strncpy( ifreq_temp.ifr_name, "lo" , 2);
     /* !!!!!!!!!! search way to get default  interface name and set  libpcap or something else   !!!!!!!*/
     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    */
        for( int i = 0; i < 2; ++i )
        {
         unsigned long request[ ] = { SIOCGIFHWADDR, SIOCGIFINDEX  };
          memset( &ifreq_temp, 0, sizeof( ifreq_temp ));
         if( ioctl( id_socket, request[i] , &ifreq_temp ) )  /*  ioctl  spoil data and that i am  eatch call copy  data into socket_add  */
             break;

         switch( i )
         {
           case 0 :
                memcpy( &socket_address.sll_addr,    &ifreq_temp.ifr_hwaddr,  sizeof( socket_address.sll_addr ));       break; 
           case 1 :
        	memcpy( &socket_address.sll_ifindex, &ifreq_temp.ifr_ifindex, sizeof( socket_address.sll_ifindex ));    break;
         }
        }
	socket_address.sll_halen = ETH_ALEN;
   }  
   return errno; 
}

