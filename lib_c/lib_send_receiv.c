/* mem... */
#include <string.h> 


/* socket */
#include <sys/socket.h>
#include <linux/if_packet.h>
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

static int      id_socket     ;
static u_char   mac_device[6] ;

static struct   sockaddr_ll socket_add;
static struct   ifreq ifreq_;






int initialization_socket();
int socket_sent();
int socket_reciev();
char * str_error();


/* return str error human readable */
const chat * str_error()
{
  return strerror( errno ); 
}


/* return status errno if 0 success */
int  initialization_socket()
{

if ( ( id_socket = socket ( AF_PACKET, SOCK_RAW, htons( ETHER_TYPE_PACKET ))) != -1 )
   {
     struct   ifreq ifreq_temp;

     /* !!!!!!!!!! search way to get default  interface name and set  libpcap or something else   !!!!!!!*/
     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    */
        for( int i = 0; i < 2; ++i )
        {
         unsigned long request[ ] = { SIOCGIFHWADDR, SIOCGIFINDEX  };
          memset( &ifreq_temp, 0, sizeof( ifreq_temp ));
         if( ioctl( sock_raw, request[i] , &ifreq_temp ) )  /*  ioctl  spoil data and that i am  eatch call copy  data into socket_add  */
             break;
         if( !i ) // first iter...
          memcpy( socket_address.sll_addr, ifreq_temp.ifr_hwaddr, sizeof( ocket_address.sll_addr )); 
         else    // second iter...
          memcpy( socket_address.sll_ifindex , ifreq_temp.ifr_ifindex , sizeof( socket_address.sll_ifindex ));
        } 
        /* Address length*/
	socket_address.sll_halen = ETH_ALEN;
   }  
   return errno; 
}

