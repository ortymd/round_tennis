/* mem... */
#include <string.h> 
/* socket */
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
//#include <netpacket/packet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <linux/if.h>		/* for ifreq. somehow net/if.h gives error 
												lib_test.c:57:19: error: storage size of ‘ifreq_temp’ isn’t known
												*/
#include <sys/types.h> 	/* for u_char */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* for socket */
#define ETHER_TYPE_PACKET 0x5555

static int                       id_socket;
static struct   sockaddr_ll socket_address;
static char buffer_for_send   [ ETH_FRAME_LEN ], 
            buffer_for_reciev [ ETH_FRAME_LEN ]; /* 1514 */





void * fun2(void * a);
int initialize_socket();
int socket_send( char * data , size_t  size , char * mac_destination );
int socket_reciev( char * buffer_user  );
const char * str_error();


/* return str error human readable */
const char * str_error()
{
  return strerror( errno ); 
}


/* return status errno if 0 success */
int  initialize_socket()
{

if ( ( id_socket = socket ( AF_PACKET, SOCK_RAW, htons( ETHER_TYPE_PACKET ))) != -1 )
   {
     struct   ifreq ifreq_temp;
      memset( &ifreq_temp, 0, sizeof( ifreq_temp ));

     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
      strncpy( ifreq_temp.ifr_name, "wlo1" , 5);
     /* !!!!!!!!!! search way to get default  interface name and set  libpcap or something else   !!!!!!!*/
     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    */
        for( int i = 0; i < 2; ++i )
        {
         unsigned long request[ ] = { SIOCGIFHWADDR, SIOCGIFINDEX  };
         if( ioctl( id_socket, request[i] , &ifreq_temp ) )  /*  ioctl  spoil data and that i am  eatch call copy  data into socket_add  */
             break;

         switch( i )
         {
           case 0 :
                memcpy( &socket_address.sll_addr,    &ifreq_temp.ifr_hwaddr.sa_data,  sizeof( ifreq_temp.ifr_hwaddr.sa_data )); break; 
           case 1 :
        	memcpy( &socket_address.sll_ifindex, &ifreq_temp.ifr_ifindex,         sizeof( socket_address.sll_ifindex ));    break;
         }
        }
	socket_address.sll_halen = ETH_ALEN;
   
   }  
   return errno; 
}

/* look if type args corrected for ctype  */
/* max send 1500 bite */
int socket_send( char * data , size_t  size , char * mac_destination  )
{
     if( data && mac_destination && size )
       {
      	 if(  ( size < ( ETH_ZLEN - ETH_HLEN ) ) || ( size  >  ETH_DATA_LEN ) ) 
       	   errno  = EINVAL;
         else  
       	  {
       	   struct ethhdr * frame  = (struct ethhdr *)( buffer_for_send );
       	   memcpy( frame->h_dest,           mac_destination, ETH_ALEN ); /* set mac destination */
       	   memcpy( frame->h_source, socket_address.sll_addr, ETH_ALEN ); /* set my mac address  */

       	   frame->h_proto =  htons( ETHER_TYPE_PACKET );               /* set type protocol */

           memcpy( buffer_for_send + sizeof( struct ethhdr), data, size); /* copy data into buffer_send */
 
           sendto( id_socket, buffer_for_send , size + ETH_HLEN , 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));

       	  }
       }
        return errno;
}



/* copy reciev frame into  buffer  */
int socket_reciev( char * buffer_user )
{
  
   size_t size_mes = recv(  id_socket, buffer_for_reciev, ETH_DATA_LEN, 0); 
    if( !errno )
       memcpy( buffer_user, buffer_for_reciev, size_mes );
 
    return errno;
}

void * fun2(void * a)
{
	char b[] = "HELLO WORLD **************************************************************************************************************************";
	u_char mac[] = { 0xff,0xff,0xff,0xff,0xff,0xff};

	for( int i = 0; i < 3; ++i)
	{
	printf("send\n");
	int c = socket_send( b, sizeof(b), mac );
	sleep(1);
	}
}
