#define crypt(s1, s2)	(s1)

/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#if !defined( WIN32 )
#include <sys/time.h>
#endif
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h>
#endif

#if defined( WIN32 )
#include <winsock.h>
#include <sys/timeb.h> /*for _ftime(), uses _timeb struct*/
#endif

#include "merc.h"

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif


void check_class args(( CHAR_DATA *ch ));
void save_config args(( void ));


/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix) || defined(WIN32)
#include <signal.h>
#if !defined(WIN32)
#include <unistd.h>
#include <sys/resource.h>	/* for RLIMIT_NOFILE */
#endif
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
const   char    compress_will   [] = { '\0' };
const   char    compress_do     [] = { '\0' };
const   char    compress_dont   [] = { '\0' };
const   char    compress_start  [] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
/* mccp: compression negotiation strings */
const   char    compress_will   [] = { IAC, WILL, TELOPT_COMPRESS, '\0' };
const   char    compress_do     [] = { IAC, DO, TELOPT_COMPRESS, '\0' };
const   char    compress_dont   [] = { IAC, DONT, TELOPT_COMPRESS, '\0' };
void show_string args((DESCRIPTOR_DATA *d, char *input ));

#endif

#if	defined( WIN32 )
const   char echo_off_str	[] = { '\0' };
const   char echo_on_str	[] = { '\0' };
const   char go_ahead_str	[] = { '\0' };
void    gettimeofday    args( ( struct timeval *tp, void *tzp ) );
void show_string args((DESCRIPTOR_DATA *d, char *input ));
#endif

/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
/* mercpoint: commented out to enable compiling
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
*/
int	close		args( ( int fd ) );
//int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
//int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
//int	listen		args( ( int s, int backlog ) );
/* kavirpoint: put this in for nin site.
int	read		args( ( int fd, char *buf, int nbyte ) );
*/
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
/* kavirpoint: put this in for nin site.
int	write		args( ( int fd, char *buf, int nbyte ) );
*/
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int	setsockopt	args( ( int s, int level, int optname, 
                            const char *optval, int optlen ) );
#else 
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
char		    str_boot_time[MAX_INPUT_LENGTH];
char		    crypt_pwd[MAX_INPUT_LENGTH];
time_t		    current_time;	/* Time of this pulse		*/
int		    arena;
int		    doxlimit=0 ;
/* Colour scale char list - Calamar */

char *scale[SCALE_COLS] = {
	L_RED,
	L_BLUE,
	L_GREEN,
	YELLOW
};

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( DESCRIPTOR_DATA *d, char *txt, int length ) );
bool	write_to_descriptor_2	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix) || defined( WIN32 )
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( DESCRIPTOR_DATA *d, char *txt, int length ) );
bool	write_to_descriptor_2	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_kickoff		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void	bust_a_prompt		args( ( DESCRIPTOR_DATA *d ) );
void    bust_a_header           args((DESCRIPTOR_DATA *d));
void    init_signals            args( (void) );
void    copyover             args( (void) );

int port, control;

int main( int argc, char **argv )
{
    struct timeval now_time;
    bool fCopyOver = FALSE;
    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

#ifdef RLIMIT_NOFILE
#ifndef min
# define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif
        { 
        struct  rlimit rlp;
        (void)getrlimit(RLIMIT_NOFILE, &rlp);
        rlp.rlim_cur=min(rlp.rlim_max,FD_SETSIZE);
        (void)setrlimit(RLIMIT_NOFILE, &rlp);
	}
#endif

    /*
     * Init time and encryption.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );
    strcpy( crypt_pwd, "Don't bother." );

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 4321;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
    }

 	if (argv[2] && argv[2][0])
 	{
 		fCopyOver = TRUE;
 		control = atoi(argv[3]);
 	}
 		
 	else
 		fCopyOver = FALSE;

    /*
     * Run the game.
     */
#if defined(macintosh) || defined(MSDOS)
    boot_db( );
    log_string( "Dystopia is ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined(unix) || defined(WIN32)
    if ( !fCopyOver ) /* We have already the port if copyover'ed */
       control = init_socket (port);

    init_signals();
    boot_db (fCopyOver);

    arena = FIGHT_CLEAR;
    sprintf( log_buf, "Dystopia is ready to rock on port %d.", port );
    log_string( log_buf );
    game_loop_unix( control );
#if !defined( WIN32 )
    close( control );
#else
    closesocket( control );
    WSACleanup();
#endif
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix) || defined(WIN32)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x;
    int fd;

#if !defined(WIN32)
    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }
#else
    WORD    wVersionRequested = MAKEWORD( 1, 1 );
    WSADATA wsaData;
    int err = WSAStartup( wVersionRequested, &wsaData ); 
    if ( err != 0 )
    {
	perror( "No useable WINSOCK.DLL" );
	exit( 1 );
    }

    if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "Init_socket: socket" );
	exit( 1 );
    }
#endif

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close( fd );
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;

#if !defined(WIN32)
    sa.sin_family   = AF_INET;
#else
    sa.sin_family   = PF_INET;
#endif
    sa.sin_port	    = htons( port );
    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror( "Init_socket: bind" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.next		= descriptor_list;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	extern char * help_greeting_2;
	extern char * help_greeting_3;
	extern char * help_greeting_4;
	extern char * help_greeting_5;
	extern char * help_greeting_6;
      int rnd = 1;

       if ( rnd == 1 )
          {
	       if ( help_greeting[0] == '.' )
  	          write_to_buffer( &dcon, help_greeting+1, 0 );
                else write_to_buffer( &dcon, help_greeting, 0 );
          }
          else if ( rnd == 2 )
                  {
	                if ( help_greeting_2[0] == '.' )
  	                   write_to_buffer( &dcon, help_greeting_2+1, 0 );
  	                   else write_to_buffer( &dcon, help_greeting_2, 0 );
                  }
          else if ( rnd == 3 )
                  {
	                if ( help_greeting_3[0] == '.' )
  	                   write_to_buffer( &dcon, help_greeting_3+1, 0 );
  	                   else write_to_buffer( &dcon, help_greeting_3, 0 );
                  }
          else if ( rnd == 4 )
                  {
	                if ( help_greeting_4[0] == '.' )
  	                   write_to_buffer( &dcon, help_greeting_4+1, 0 );
  	                   else write_to_buffer( &dcon, help_greeting_4, 0 );
                  }
          else if ( rnd == 5 )
                  {
	                if ( help_greeting_5[0] == '.' )
  	                   write_to_buffer( &dcon, help_greeting_5+1, 0 );
  	                   else write_to_buffer( &dcon, help_greeting_5, 0 );
                  }
          else {
	                if ( help_greeting_6[0] == '.' )
  	                   write_to_buffer( &dcon, help_greeting_6+1, 0 );
  	                   else write_to_buffer( &dcon, help_greeting_6, 0 );
               }
    }

    /* Main loop */
    while ( !merc_down )

    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
            if ( d->character != NULL )
               d->character->timer = 0;

		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

          if ( d->character != NULL && d->character->ctimer[15] > 0 && d->character->position >= POS_STUNNED )
             {
                d->character->ctimer[15] -= 2;

                if ( d->character->ctimer[15] < 6 )
                   {
                      d->character->ctimer[15] = 0;
                      d->character->ctimer[9] = 0;

                      if ( d->character->position >= POS_STUNNED )
                         {
                            act( "#gTime #wresumes #gnormal #wspeed #gagain #was #geverything #wspeeds #gup#w.#n", d->character, NULL, NULL, TO_CHAR );
                            act( "#gTime #wresumes #gnormal #wspeed #gagain #was #geverything #wspeeds #gup#w.#n", d->character, NULL, NULL, TO_ROOM );
                         }
                   }
             }

	    if ( d->character != NULL && d->character->blocking > 0 )
             {
                --d->character->blocking;

                if ( d->character->blocking == 6 && IS_SET( d->character->arena, AFF2_AUTOGUARD ) )
                   {
                      REMOVE_BIT( d->character->arena, AFF2_AUTOGUARD );
 
                      do_autoguard( d->character, d->character->fighting );
                   }

                if ( d->character->blocking == 0 && d->character->counta == TRUE )
                   d->character->counta = FALSE;

                d->character->wait = 0;
                continue;
             }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
                if ( !IS_NPC( d->character ) && d->character->ctimer[15] > 0 )
                   d->character->wait = 0;
                   else if ( is_martial( d->character ) && d->character->wait > 18 )
                           d->character->wait = 18;

                if ( IS_CLASS( d->character, CLASS_WISP ) && d->character->class_2 == 0 && d->character->pkill >= 4 && d->character->wait > 4 )
                   d->character->hit = UMIN( d->character->max_hit, d->character->hit - is_elemental( ATTACK_THUNDER, d->character, 100000 ) );

		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

                /* OLC */
                if ( d->showstr_point )
                    show_string( d, d->incomm );
                else
                if ( d->pString )
                    string_add( d->character, d->incomm );
                else
		switch( d->connected )
		{
	     	        default:
 			nanny( d, d->incomm );
			break;
		   case CON_PLAYING:
                        if ( !run_olc_editor( d ) )
                            interpret( d->character, d->incomm );
			break;
		   case CON_EDITING:
			edit_buffer( d->character, d->incomm );
			break;
		  }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;

		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL )
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix) || defined(WIN32)

void excessive_cpu(int blx)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *player = char_list;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	  vch_next = vch->next;

        if ( IS_NPC(vch) )
           continue;

        player = vch;

        stc("#w<<#yMUD INFO#w>> #gMud has crashed... Attempting to copyover... #w<<#yMUD INFO#w>>#n\n\r", player);
    }

    do_copyover ( player, "" );
}

void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

#if !defined(WIN32)
    signal( SIGPIPE, SIG_IGN );
#endif
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
/* kavirpoint
	maxdesc	= control * 2;
*/
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

          if ( d->character != NULL )
             d->character->timer = 0;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

          if ( d->character != NULL && d->character->ctimer[15] > 0 && d->character->position >= POS_STUNNED )
             {
                d->character->ctimer[15] -= 2;

                if ( d->character->ctimer[15] < 6 )
                   {
                      d->character->ctimer[15] = 0;
                      d->character->ctimer[9] = 0;

                      if ( d->character->position >= POS_STUNNED )
                         {
                            act( "#gTime #wresumes #gnormal #wspeed #gagain #was #geverything #wspeeds #gup#w.#n", d->character, NULL, NULL, TO_CHAR );
                            act( "#gTime #wresumes #gnormal #wspeed #gagain #was #geverything #wspeeds #gup#w.#n", d->character, NULL, NULL, TO_ROOM );
                         }
                   }
             }

	    if ( d->character != NULL && d->character->blocking > 0 )
             {
                --d->character->blocking;

                if ( d->character->blocking == 6 && IS_SET( d->character->arena, AFF2_AUTOGUARD ) )
                   {
                      REMOVE_BIT( d->character->arena, AFF2_AUTOGUARD );
 
                      do_autoguard( d->character, d->character->fighting );
                   }

                if ( d->character->blocking == 0 && d->character->counta == TRUE )
                   d->character->counta = FALSE;

                d->character->wait = 0;
                continue;
             }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
             if ( !IS_NPC( d->character ) && d->character->ctimer[15] > 0 )
                d->character->wait = 0;
                else if ( is_martial( d->character ) && d->character->wait > 18 )
                        d->character->wait = 18;

             if ( IS_CLASS( d->character, CLASS_WISP ) && d->character->class_2 == 0 && d->character->pkill >= 4 && d->character->wait > 4 )
                d->character->hit = UMIN( d->character->max_hit, d->character->hit - is_elemental( ATTACK_THUNDER, d->character, 100000 ) );

		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

                /* OLC */
                if ( d->showstr_point )
                    show_string( d, d->incomm );
                else
                if ( d->pString )
                    string_add( d->character, d->incomm );
                else 
		switch( d->connected )
			  {
			   default:
 				nanny( d, d->incomm );
				break;
			   case CON_PLAYING:
                        if ( !run_olc_editor( d ) )
				interpret( d->character, d->incomm );
				break;
			   case CON_EDITING:
				edit_buffer( d->character, d->incomm );
				break;
                           case CON_PFILE:
                                jope_interp( d->character, d->incomm );
                                break;
		         }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );

	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

          if ( ( d->fcommand || d->outtop > 0 || d->out_compress )
	       && FD_ISSET(d->descriptor, &out_set) )
             {
                  bool ok = TRUE;
          
                  if ( d->fcommand || d->outtop > 0 )
                     ok = process_output( d, TRUE );

                  if (ok && d->out_compress)
                     ok = processCompressed(d);
                
		      if (!ok)
		         {
		           if ( d->character != NULL )
		              save_char_obj( d->character );

                       d->outtop	= 0;
		           close_socket( d );
		         }
	       }
	 }

	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
#if !defined(WIN32)
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}
#else
	{
	    int times_up;
	    int nappy_time;
	    struct _timeb start_time;
	    struct _timeb end_time;
	    _ftime( &start_time );
	    times_up = 0;

	    while( times_up == 0 )
	    {
		_ftime( &end_time );
		if ( ( nappy_time =
		      (int) ( 1000 *
			     (double) ( ( end_time.time - start_time.time ) +
				       ( (double) ( end_time.millitm -
						   start_time.millitm ) /
					1000.0 ) ) ) ) >=
		    (double)( 1000 / PULSE_PER_SECOND ) )
		  times_up = 1;
		else
		{
		    Sleep( (int) ( (double) ( 1000 / PULSE_PER_SECOND ) -
				  (double) nappy_time ) );
		    times_up = 1;
		}
	    }
	}
#endif

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



 void init_descriptor (DESCRIPTOR_DATA *dnew, int desc)
 {
 	static DESCRIPTOR_DATA d_zero;
 
 	*dnew = d_zero;
 	dnew->descriptor = desc;
 	dnew->character = NULL;
 	dnew->connected = CON_GET_NAME;
 	dnew->showstr_head = str_dup ("");
 	dnew->showstr_point = 0;
 	dnew->pEdit = NULL;			/* OLC */
 	dnew->pString = NULL;		/* OLC */
 	dnew->editor = 0;			/* OLC */
 	dnew->outsize = 2000;
 	dnew->outbuf = alloc_mem (dnew->outsize);
 	
 }

#if defined(unix) || defined(WIN32)
void new_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;


    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

#if !defined(WIN32)
    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }
#endif

    /*
     * Cons a new descriptor.
     */
    if ( descriptor_free == NULL )
    {
	dnew		= alloc_perm( sizeof(*dnew) );
    }
    else
    {
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
    }

    init_descriptor (dnew, desc);

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
/*	log_string( log_buf ); */
        if (str_cmp("150.113.68.212", buf)) {
        from = gethostbyaddr( (char *) &sock.sin_addr,
            sizeof(sock.sin_addr), AF_INET );
        dnew->host = str_dup( from ? from->h_name : buf );
        }
        else
          dnew->host = str_dup(buf);
    }

    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    if ( check_ban(dnew->host,BAN_ALL))
    {
	write_to_descriptor_2( desc, "Your site has been banned from this mud.\n\r", 0 );
      sprintf( log_buf, "Banning all characters from %s.", dnew->host );
      log_string( log_buf );
	close( desc );
	return;
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /* mccp: tell the client we support compression */
    write_to_buffer( dnew, compress_will, 0 );

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	extern char * help_greeting_2;
	extern char * help_greeting_3;
	extern char * help_greeting_4;
	extern char * help_greeting_5;
	extern char * help_greeting_6;
      int rnd = 1;

       if ( rnd == 1 )
          {
	       if ( help_greeting[0] == '.' )
  	          write_to_buffer( dnew, help_greeting+1, 0 );
                else write_to_buffer( dnew, help_greeting, 0 );
          }
          else if ( rnd == 2 )
                  {
	                if ( help_greeting_2[0] == '.' )
  	                   write_to_buffer( dnew, help_greeting_2+1, 0 );
  	                   else write_to_buffer( dnew, help_greeting_2, 0 );
                  }
          else if ( rnd == 3 )
                  {
	                if ( help_greeting_3[0] == '.' )
  	                   write_to_buffer( dnew, help_greeting_3+1, 0 );
  	                   else write_to_buffer( dnew, help_greeting_3, 0 );
                  }
          else if ( rnd == 4 )
                  {
	                if ( help_greeting_4[0] == '.' )
  	                   write_to_buffer( dnew, help_greeting_4+1, 0 );
  	                   else write_to_buffer( dnew, help_greeting_4, 0 );
                  }
          else if ( rnd == 5 )
                  {
	                if ( help_greeting_5[0] == '.' )
  	                   write_to_buffer( dnew, help_greeting_5+1, 0 );
  	                   else write_to_buffer( dnew, help_greeting_5, 0 );
                  }
          else {
	                if ( help_greeting_6[0] == '.' )
  	                   write_to_buffer( dnew, help_greeting_6+1, 0 );
  	                   else write_to_buffer( dnew, help_greeting_6, 0 );
               }
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	   write_to_buffer( dclose->snoop_by, "Your victim has left the game.\n\r", 0 );
    }


    if (dclose->connected == CON_PFILE) jope_done(dclose->character, "");

    if ( dclose->character != NULL && 
	(dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING) &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;

          if ( d->character == NULL || d->character->pcdata->watching == NULL )
             continue;

          if ( d->character->pcdata->watching == dclose->character )
             d->character->pcdata->watching = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
 	/* If ch is writing note or playing, just lose link otherwise clear char */
 	if ( (dclose->connected == CON_PLAYING) || 
 	  ((dclose->connected >= CON_NOTE_TO) && 
 	   (dclose->connected <= CON_NOTE_FINISH)))
	{
            if (IS_SET(ch->extra, EXTRA_OSWITCH)) do_humanform(ch,"");
	    if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
	    	act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
            if ( IS_SET(ch->act,PLR_CHALLENGER) && arena == FIGHT_START )
            {
               char buf[MAX_STRING_LENGTH];

               REMOVE_BIT(ch->act,PLR_CHALLENGER);
               REMOVE_BIT(ch->challenged->act,PLR_CHALLENGED);
               ch->challenged->challenger = NULL;
               ch->challenged = NULL;
               arena = FIGHT_CLEAR;
     sprintf(buf, "[Arena] %s has lost $s link. Arena is OPEN.", ch->name);
 {
 DESCRIPTOR_DATA *d;
 for ( d = descriptor_list; d; d = d->next )
  
     if( d->connected == CON_PLAYING || d->connected == CON_EDITING)
     {
        send_to_char( buf, d->character );
     }
  }
            }
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    if ( dclose->out_compress )
       {
          deflateEnd(dclose->out_compress);
          free_mem(dclose->out_compress_buf, COMPRESS_BUF_SIZE);
          free_mem(dclose->out_compress, sizeof(z_stream));
       }

#if !defined( WIN32 )
    close( dclose->descriptor );
#else
    closesocket( dclose->descriptor );
#endif
    free_string( dclose->host );

    /* RT socket leak fix */
    free_mem( dclose->outbuf, dclose->outsize );

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}

/* For a better kickoff message :) KaVir */
void close_socket2( DESCRIPTOR_DATA *dclose, bool kickoff )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if (dclose->connected == CON_PFILE) jope_done(dclose->character, "");

    if ( dclose->character != NULL && 
       (dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING )&&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;

          if ( d->character == NULL || d->character->pcdata->watching == NULL )
             continue;

          if ( d->character->pcdata->watching == dclose->character )
             d->character->pcdata->watching = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	if ( dclose->connected == CON_PLAYING
		|| dclose->connected == CON_EDITING )
	{
	    if (kickoff)
		act( "$n doubles over in agony and $s eyes roll up into $s head.", ch, NULL, NULL, TO_ROOM );
 	    save_char_obj( ch );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }
#if !defined(WIN32)
    close( dclose->descriptor );
#else
    closesocket( dclose->descriptor );
#endif
    free_string( dclose->host );
    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	if (d != NULL && d->character != NULL)
	    sprintf( log_buf, "%s input overflow!", d->character->lasthost );
	else
	    sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );

	write_to_descriptor( d,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix) || defined(WIN32)
    for ( ; ; )
    {
	int nRead;

#if !defined(WIN32)
	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
#else
	nRead = recv( d->descriptor, d->inbuf + iStart,
		     sizeof( d->inbuf ) - 10 - iStart, 0 );
#endif
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
#if !defined(WIN32)
	else if ( errno == EWOULDBLOCK )
	    break;
#endif
#if defined( WIN32 )
        else if ( WSAGetLastError() == WSAEWOULDBLOCK || errno == EAGAIN )
	    break;
#endif
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
          else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	            d->incomm[k++] = d->inbuf[i];
          else if (d->inbuf[i] == (signed char)IAC) 
                  {
                     if (!memcmp(&d->inbuf[i], compress_do, strlen(compress_do))) 
                        {
                           i += strlen(compress_do) - 1;
                           compressStart(d);
                        }
                        else if (!memcmp(&d->inbuf[i], compress_dont, strlen(compress_dont))) 
                                {
                                    i += strlen(compress_dont) - 1;
                                    compressEnd(d);
                                }
                  }
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 40 )
	    {
		if (d != NULL && d->character != NULL)
		    sprintf( log_buf, "%s input spamming!", d->character->lasthost );
		else
		    sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		write_to_descriptor( d,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
/*
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;
    DESCRIPTOR_DATA *e;

    if ( fPrompt && !merc_down && (d->connected == CON_PLAYING || d->connected == CON_PFILE))
        if ( d->showstr_point )
            write_to_buffer( d,
  "[Please type (c)ontinue, (r)efresh, (b)ack, (h)elp, (q)uit, or RETURN]:  ",
                            0 );
        else
        if ( d->pString )
            write_to_buffer( d, "> ", 2 );
        else
 
{
	CHAR_DATA *ch;

	ch = d->original ? d->original : d->character;
	if ( IS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

	if (IS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT))
	    bust_a_prompt( d );
	else if ( IS_SET(ch->act, PLR_PROMPT) )
	{
	    char buf[MAX_STRING_LENGTH];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];
	    int per;

	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);

	        sprintf( buf, "<[%sX] [?H ?M ?V]> ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING)
	    {
              int max_hit, cur_hit;

	        victim = ch->fighting;

           if ( victim != NULL )
              {
                 if ( victim->hit > 10000000 || victim->max_hit > 10000000 )
                    {
                       cur_hit = victim->hit / 10;
                       max_hit = victim->max_hit / 10;
                    }
                    else {
                            cur_hit = victim->hit;
                            max_hit = victim->max_hit;
                         }
              }

           if (victim == NULL || max_hit < 1)
              {
                 strcpy(cond, "UNKNOWN");
                 ADD_COLOUR(ch, cond, L_RED);
              }
		else if ( cur_hit * 100 / max_hit < 25 )
		{
		    strcpy(cond, "Awful");
		    ADD_COLOUR(ch, cond, L_RED);
		}
		else if ( cur_hit * 100 / max_hit < 50 )
		{
		    strcpy(cond, "Poor");
		    ADD_COLOUR(ch, cond, L_BLUE);
		}
		else if ( cur_hit * 100 / max_hit < 75 )
		{
		    strcpy(cond, "Fair");
		    ADD_COLOUR(ch, cond, L_GREEN);
		}
		else if ( cur_hit * 100 / max_hit < 100 )
		{
		    strcpy(cond, "Good");
		    ADD_COLOUR(ch, cond, YELLOW);
		}
		else {
                     strcpy(cond, "Perfect");
                     ADD_COLOUR(ch, cond, L_CYAN);
                 }

		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf( buf, "<%s [%sH %sM %sV]> ", cond, hit_str, mana_str, move_str );
	    }
	    else
	    {
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);
	        sprintf( buf, "<[%s] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, buf, 0 );
	}

	if ( IS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    if ( d->outtop == 0 )
	return TRUE;

    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    if ( !write_to_descriptor( d, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }

}
*/

bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;
    DESCRIPTOR_DATA *e;

    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
	CHAR_DATA *ch;
	CHAR_DATA *victim;

	ch = d->original ? d->original : d->character;
	if ( IS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

      if ( ch->pcdata->watching != NULL && IS_NPC( ch->pcdata->watching ) )
         {
            char prompt[MAX_STRING_LENGTH];
            char cond[MAX_INPUT_LENGTH];
            char hit_str[MAX_INPUT_LENGTH];
            char mana_str[MAX_INPUT_LENGTH];
            char move_str[MAX_INPUT_LENGTH];
            CHAR_DATA *to = ch->pcdata->watching;
            int max_hit, cur_hit;

            if ( to->position == POS_FIGHTING )
               victim = to->fighting;
               else victim = NULL;

            if ( victim != NULL )
               {
                  if ( to->hit > 10000000 || to->max_hit > 10000000 )
                     {
                        cur_hit = victim->hit / 10;
                        max_hit = victim->max_hit / 10;
                     }
                     else {
                             cur_hit = victim->hit;
                             max_hit = victim->max_hit;
                          }
               }

		sprintf(hit_str, "%d", to->hit);
		COL_SCALE(hit_str, to, to->hit, to->max_hit);
		sprintf(mana_str, "%d", to->mana);
		COL_SCALE(mana_str, to, to->mana, to->max_mana);
		sprintf(move_str, "%d", to->move);
		COL_SCALE(move_str, to, to->move, to->max_move);

            if (victim == NULL || max_hit < 1)
               strcpy(cond, "#RN#y/#RA#e");
               else if ( cur_hit * 100 / max_hit < 25 )
                       strcpy(cond, "#RAwful#e");
               else if ( cur_hit * 100 / max_hit < 50 )
                       strcpy(cond, "#LPoor#e");
               else if ( cur_hit * 100 / max_hit < 75 )
                       strcpy(cond, "#gFair#e");
               else if ( cur_hit * 100 / max_hit < 100 )
                       strcpy(cond, "#yGood#e");
               else strcpy(cond, "#CPerfect#e");

            sprintf( prompt, "#e<[%s] [%sH %sM %sV]>#n\n\r", cond, hit_str, mana_str, move_str );
            send_to_char( prompt, d->character );
         }

      if ( IS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT) )
          bust_a_prompt( d );
          else if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
                    && IS_SET( ch->arena, AFF2_INARENA ) )
                  show_power_meter( ch );
	else if ( IS_SET(ch->act, PLR_PROMPT) )
	{
	    char buf[MAX_STRING_LENGTH];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];

	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);
            sprintf( buf, "#7<[#4%sX#7] [#3?#1H #3?#6M #3?#2V#7]>#n ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING)
	    {
              int max_hit, cur_hit;

	        victim = ch->fighting;

              if ( victim != NULL )
              {
                 if ( victim->hit > 10000000 || victim->max_hit > 10000000 )
                    {
                       cur_hit = victim->hit / 10;
                       max_hit = victim->max_hit / 10;
                    }
                    else {
                            cur_hit = victim->hit;
                            max_hit = victim->max_hit;
                         }
              }

            if (victim == NULL || max_hit < 1)
              {
                 strcpy(cond, "N/A");
                 ADD_COLOUR(ch, cond, L_RED);
              }
		else if ( cur_hit * 100 / max_hit < 25 )
		{
		    strcpy(cond, "Awful");
		    ADD_COLOUR(ch, cond, L_RED);
		}
		else if ( cur_hit * 100 / max_hit < 50 )
		{
		    strcpy(cond, "Poor");
		    ADD_COLOUR(ch, cond, L_BLUE);
		}
		else if ( cur_hit * 100 / max_hit < 75 )
		{
		    strcpy(cond, "Fair");
		    ADD_COLOUR(ch, cond, L_GREEN);
		}
		else if ( cur_hit * 100 / max_hit < 100 )
		{
		    strcpy(cond, "Good");
		    ADD_COLOUR(ch, cond, YELLOW);
		}
		else {
                     strcpy(cond, "Perfect");
                     ADD_COLOUR(ch, cond, L_CYAN);
                 }

		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf( buf, "#7<[%s] [%sH %sM %sV]> ", cond,hit_str, mana_str, move_str );
	    }
	    else
	    {
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);
            sprintf( buf, "#7<[%s] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, buf, 0 );
	}

	if ( IS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	 write_to_buffer( d->snoop_by, "% ", 2 );
	 write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    for ( e = descriptor_list; e != NULL; e = e->next )
    {
       if ( e == NULL || e->character == NULL || e->character->pcdata->watching == NULL || e->character->pcdata->watching != d->character )
          continue;

	 write_to_buffer( e, "%%% ", 2 );
	 write_to_buffer( e, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}

void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length)
{
    char tb[MAX_STRING_LENGTH], ccode;
    int i,j;
    bool ansi, grfx = FALSE;


    /*
     * Find length in case caller didn't.
     */                                                                        
    if ( length <= 0 )
        length = strlen(txt);

    if ( length >= MAX_STRING_LENGTH )
    {
        bug( "Write_to_buffer: Way too big. Closing.", 0 );
        return;
    }

    if (d->character == NULL ||
        (d->connected != CON_PLAYING))
        ansi = FALSE;
    else ansi = (IS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;
    if (d->connected == CON_GET_NAME) ansi = TRUE;

    /*
     * Initial \n\r if needed.
     */                                                                        
    if ( d->outtop == 0 && !d->fcommand )
    {
        d->outbuf[0]    = '\n';
        d->outbuf[1]    = '\r';
        d->outtop       = 2;
    }

    /*
     * Expand d->outbuf for ansi info
     */

    j = 0;
    ccode = '3';
    tb[0] = '\0';
    for (i = 0; i < length ; i++)
    {
        if ( txt[i] == '#' && txt[i+1] != 'I' && txt[i+1] != 'N' )               
        {
            if (txt[++i] == 'Z')
            {
                ccode = '3';
                i++;
            }
            else ccode = '3';

            switch (txt[i])
            {
                default : break;
                case '#': tb[j++] = '#'; break;
                case '-': tb[j++] = '~'; break;
                case '+': tb[j++] = '%'; break;

                case '!':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case '@':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case '$':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case '%':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case '^':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case '&':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case '*':if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '7'; tb[j++] = 'm';
                    break;

                case 'I':
                case 'i': tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = 'm'; break;                                
                case 'u': tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] ='m'; break;
                case 't': if (!ansi) continue;
                  tb[j++] = '['; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'T': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] = '1';
                    break;
                case 'd': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = '1';
                    break;
                case 'D': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '6'; tb[j++] = '1';
                    break;
                case 'N':
                case 'n':
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case 'e':
                case '0': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '1': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'g':
                case '2': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case 'o': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case 'y':
                case '3': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case 'b':
                case 'B':
                case '4': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case '5': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case '6': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 'w':
                case '7': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';             
                    break;
                case 'r': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'R': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'G': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;                                                     
                case 'l': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'L': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'p': if (!ansi) continue;                                 
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case 'P': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case 'c': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 'q': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
   	          case 'z': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '5';
                    tb[j++] = 'm';
                    break;
                case 'C': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 's': if (!ansi) continue;
                     switch(number_range(1,15))                                
                     {
                        case  1:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  2:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                        case  3:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    break;
                        case  4:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;                                                     
                        case  5:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                        case  6:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                     break;
                       case  7:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                        case  8:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  9:                                               
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                        case  10:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                        case  11:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  12:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                        case  13:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                        case  14:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                        case  15:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;

                     }
                        break;
            }
        }                                                                      
        else tb[j++] = txt[i];
    }

    {
        tb[j++] = 27;
        tb[j++] = '[';
        tb[j++] = '0';
        tb[j++] = 'm';
        if (grfx)
        {
                tb[j++] = 27;
                tb[j++]='(';
                tb[j++]='B';
        }
    }
    tb[j]   = '\0';                                                            

    length = j;

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
        char *outbuf;

        if (d->outsize >= 32000)
        {
            bug("Buffer overflow. Closing.\n\r",0);
            close_socket(d);
            return;
        }
        outbuf      = alloc_mem( 2 * d->outsize );
        strncpy( outbuf, d->outbuf, d->outtop );                               
        free_mem( d->outbuf, d->outsize );
        d->outbuf   = outbuf;
        d->outsize *= 2;
    }

    /*
     * Copy.
     */
    strncpy( d->outbuf + d->outtop, tb, length);
    d->outtop += length;
    return;
}

/* Try to send any pending compressed-but-not-sent data in `desc' */
bool processCompressed(DESCRIPTOR_DATA *desc)
{
    int iStart, nBlock, nWrite, len;

    if (!desc->out_compress)
        return TRUE;
    
    /* Try to write out some data.. */
    len = desc->out_compress->next_out - desc->out_compress_buf;
    if (len > 0) {
        /* we have some data to write */

        for (iStart = 0; iStart < len; iStart += nWrite)
        {
            nBlock = UMIN (len - iStart, 4096);

            if ((nWrite = write (desc->descriptor, desc->out_compress_buf + iStart, nBlock)) < 0)
            {
                if (errno == EAGAIN ||
                    errno == ENOSR)
                    break;

                return FALSE; /* write error */
            }

            if (nWrite <= 0)
                break;
        }

        if (iStart) {
            /* We wrote "iStart" bytes */
            if (iStart < len)
                memmove(desc->out_compress_buf, desc->out_compress_buf+iStart, len - iStart);

            desc->out_compress->next_out = desc->out_compress_buf + len - iStart;
        }
    }

    return TRUE;
}


/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor_2( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
#if !defined(WIN32)
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
#else
	if ( ( nWrite = send( desc, txt + iStart, nBlock , 0) ) < 0 )
#endif
	    { perror( "Write_to_descriptor" ); return
FALSE; }
    } 

    return TRUE;
}

/* mccp: write_to_descriptor wrapper */
bool write_to_descriptor(DESCRIPTOR_DATA *d, char *txt, int length)
{
    if (d->out_compress)
       return writeCompressed(d, txt, length);
       else return write_to_descriptor_2(d->descriptor, txt, length);
}

void do_introduction( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->more3, MORE3_NOSHOW) )
       REMOVE_BIT(ch->more3, MORE3_NOSHOW);

    if ( IS_NPC(ch) || IS_IMMORTAL(ch) )
       return;

    if ( ch->version < mud_version )
       {
          ch->wkill = 0;
          ch->version = mud_version;
       }

    check_leaderboard(ch,"");
    check_multi( ch );

    if ( IS_SET(ch->more2, MORE2_TIE) )
       REMOVE_BIT(ch->more2, MORE2_TIE);

    if ( IS_SET( ch->more2, MORE2_RETALIATE ) )
       REMOVE_BIT( ch->more2, MORE2_RETALIATE );

    if ( ch->pcdata->login[0] != '\0' )
       {
             DESCRIPTOR_DATA *d;
             int test = 0;

             if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
                {
                   REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                   test = 1;
                }

             buf[0] = '\0';
             strcat( buf, "#p[#w<#gInfo#w>#p]#c " );
             strcat( buf, ch->pcdata->login );
             strcat( buf, " #p[#w<#gInfo#w>#p]#n" );
    
             for ( d = descriptor_list; d != NULL; d = d->next )
             {
                if ( d->connected != CON_PLAYING || d->character == ch )
                   continue;

                act( buf,ch,NULL,d->character,TO_VICT );
             }

             if ( test == 1 )
                SET_BIT(ch->affected_by, AFF_POLYMORPH);

             return;
       }
       else if ( ch->level == 1 )
       sprintf(buf,"#gPlease welcome #w%s#g, the newest player to the #RValley #gof #eDeath#g.#n", ch->name );
       else if ( ch->class == 0 )
 	         sprintf(buf,"#2%s #7enters #2Valley of Death#7.#n", ch->name );
       else {
               switch( ch->class )
               {
                  case CLASS_FIEND:
                  case CLASS_DEMON: sprintf(buf,"#rSatan releases #w%s #rfrom their bond to capture more souls!#n", ch->name );
                                    break;
                  case CLASS_DARAMON:
                  case CLASS_MONK: sprintf(buf,"#C%s #wcompletes their meditation and confronts the darkness once more.#n", ch->name );
                                   break;
                  case CLASS_PALADIN: sprintf(buf,"#g%s #whas left the castle to battle evil!#n", ch->name );
                                      break;
                  case CLASS_WISP: sprintf(buf,"#w%s #ycooes 'wooooooah mia pom pom' as they hover above utopia.#n", ch->name );
                                   break;
                  case CLASS_SNOW_ELF: sprintf(buf,"#g%s #Cslowly emerges from the snow as they seek a victim.#n", ch->name );
                                       break;
                  case CLASS_DRAGON_KNIGHT: sprintf(buf,"#gTransforming into a majestic #7dragoon #c%s #graces away from the #cDragon Shrine.#n", ch->name );
                                            break;
                  case CLASS_WEREWOLF: sprintf(buf,"#r%s #Rcomes in a berserking #yr#ba#wg#ge #Rthroughout Utopia!#n", ch->name );
                                       break;
                  case CLASS_DRAGON: sprintf(buf,"#pWaking from their long slumber, #g%s #plooks for their next meal.#n", ch->name );
                                     break;
                  case CLASS_ANGEL: sprintf(buf,"#wDescending from Heaven, #y%s #warrived to save ManKind.#n", ch->name );
                                    break;
                  case CLASS_MASTER:
                  case CLASS_WIZARD: sprintf(buf,"#c%s #bappears in a flash of light!#n", ch->name );
                                     break;
                  case CLASS_DRAGOON: sprintf(buf,"#cSERIOUSLY pissed now, #p%s #cis here to KICK some ass!#n", ch->name );
                                      break;
                  case CLASS_SAIYAN: sprintf(buf,"#rRoaring with rage, #p%s #rcharges into battle!#n", ch->name );
                                     break;
                  case CLASS_PHOENIX: sprintf(buf,"#y%s #Rawakes from their fiery tomb to begin life anew!#n", ch->name );
                                      break;
                  case CLASS_DUERGAR: sprintf(buf,"#gLooking for amusement and good beer, #e%s #ghas arrived.#n", ch->name );
                                      break;
                  case CLASS_HOBBIT: sprintf(buf,"#wWill #e%s #wgive in to the corrupted powers of the ring this time?#n", ch->name );
                                     break;
                  case CLASS_SAMURAI: sprintf(buf,"#CWielding their katana, #w%s #Cprepares for battle!#n", ch->name );
                                      break;
                  case CLASS_VAMPIRE: sprintf(buf,"#r%s #Rwakes from their rest and begins the search for blood.#n", ch->name );
                                      break;
                  case CLASS_NIGHT_BLADE:
                  case CLASS_SHADOW:
                  case CLASS_SHADOW_WAR:
                  case CLASS_NINJA: sprintf(buf,"#eFading from the shadows, #b%s #eprepares for their merciless deeds.#n", ch->name );
                                    break;
                  case CLASS_DROW: sprintf(buf,"#eLloth has sent #w%s #eon a new mission.#n", ch->name );
                                   break;
                  case CLASS_UNDEAD_KNIGHT: sprintf(buf,"#eForever cursed on the astral planes, #p%s #esearches for salvation.#n", ch->name );
                                            break;
                  case CLASS_NECROMANCER: sprintf(buf,"#w%s #ehas arrived with their undead army.#n", ch->name );
                                          break;
                  case CLASS_KURUDA:
                  case CLASS_ASSASSIN: sprintf(buf,"#eNests has release #p%s #eto create havoc among utopia.#n", ch->name );
                                       break;
				  case CLASS_DAYWALKER: sprintf(buf,"#eFading into existence, #R%s #eseeks the next vampire.#n", ch->name );
                                    break;
                  default: sprintf(buf,"#2%s #7enters #2Valley of Death#7.#n", ch->name );
                           break;
               }
            }

    do_info(ch,buf);

    if ( arena == FIGHT_WAR && number_percent() > 70 )
       {
           CHAR_DATA *mob;

           mob = get_mob( 100 );

           if ( mob != NULL )
              do_chat( mob, "Hey, new guy." );
       }
}

void show_motd( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];

	do_help( ch, "motd" );

	stc("\n\r#3**#6Welcome to #7Valley of Death#3**\n\r#3**#6Where #1Death#6 is a part of #7Life#3**\n\r", ch );
	ch->next	= char_list;
	char_list	= ch;


	if (ch->mounted == IS_RIDING) do_dismount(ch,"");

	if ( !IS_SET(ch->extra, EXTRA_TRUSTED) )
		SET_BIT(ch->extra, EXTRA_TRUSTED);

	if ( ch->level < 3 )
	{
	    ch->pcdata->plr_wager    = 0; /* arena betting amount */
          ch->pcdata->awins        = 0; /* arena wins           */
          ch->pcdata->alosses      = 0; /* arena losses         */
          ch->pcdata->board = &boards[0];
          ch->gladiator       = NULL; /* set player to bet on to NULL */
	    ch->challenger = NULL;
	    ch->challenged = NULL;
	    ch->exp	= 0;
	    ch->hit	= ch->max_hit = 10000001;
	    ch->mana	= ch->max_mana = 10000001;
	    ch->move	= ch->max_move = 10000001;
          ch->gold      = 10000;
          ch->home      = 800;
          ch->special   = 0;
	    set_switchname(ch, ch->name);
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    do_look( ch, "auto" );
          do_introduction(ch);
	    ch->level	= 3;
	}
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	{
          ch->pcdata->obj_vnum = 0;
	    if (ch->in_room != NULL) char_to_room( ch, ch->in_room );
	    else char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
          do_introduction(ch);
          ch->wait = 12;
	}
	else if ( ch->in_room != NULL )
	{
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
             else char_to_room( ch, ch->in_room );

	    do_look( ch, "auto" );
          do_introduction(ch);
          ch->wait = 12;
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	    do_look( ch, "auto" );
          do_introduction(ch);
          ch->wait = 12;
	}

      players_logged++;
      ch->fight_timer = 0;

      if ( IS_SET(ch->more2, MORE2_HEAD) )
         {
             REMOVE_BIT(ch->more2, MORE2_HEAD);
             REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	       REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	       free_string(ch->morph);
	       ch->morph = str_dup("");
         }

      act( "With a flash of energy, $n has arrived into our world.", ch, NULL, NULL, TO_ROOM );

      if ( arena == FIGHT_WAR )
         {
            stc("#wA #PMASSIVE #CKINGDOM #RWAR #sI#sS #sO#sN#w!!!#n #wA #PMASSIVE #CKINGDOM #RWAR #sI#sS #sO#sN#w!!!#n\n\r",ch);
            stc("#wA #PMASSIVE #CKINGDOM #RWAR #sI#sS #sO#sN#w!!!#n #wA #PMASSIVE #CKINGDOM #RWAR #sI#sS #sO#sN#w!!!#n\n\r",ch);
         }
         else if ( mysafe != 0 )
                 {
                    stc("#wSAFES #yARE #cOUT!!! #bSAFES #pARE #wOUT!!!#n\n\r",ch);
                    stc("#wSAFES #yARE #cOUT!!! #bSAFES #pARE #wOUT!!!#n\n\r",ch);
                 }

      if ( nogang == 0 )
         {
            stc("#wGANGING #yIS #cIN!!! #bGANGING #pIS #wIN!!!#n\n\r",ch);
            stc("#wGANGING #yIS #cIN!!! #bGANGING #pIS #wIN!!!#n\n\r",ch);
         }

      if ( IS_CLASS(ch, CLASS_ANGEL) )
         angels++;

	if ( !IS_NPC(ch) )
       {
       for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
       {
	int value = 0;
	if( obj->item_type == ITEM_WEAPON )
	{
	    value = obj->value[0];
	    if( value >= 1000 )
	    {
		int temp = value / 1000;
		temp = temp * 1000;
		value -= temp;
	    }
	    if( value == 81 )
	    { do_remove(ch,obj->name);extract_obj(obj);}
	}
    }
  }

	if( !IS_NPC(ch) && ch->pcdata->bounty > 0 ){
	    sprintf(buf,"%s has a #w%d #ygold piece#n bounty on their head.",ch->name,ch->pcdata->bounty);
	    do_info(ch,buf);
	}
	room_text(ch,">ENTER<");

      if ( IS_CLASS(ch,CLASS_DRAGON) && ch->dragonform > 0 )
         check_dragon_form( ch, "" );

      if ( IS_SET( ch->extra, EXTRA_HUNT ) )
         REMOVE_BIT( ch->extra, EXTRA_HUNT );

	ch->embraced=NULL; 
      ch->embracing=NULL;
}

/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
    char *strtime;
    int char_age = 17;
    bool fOld = FALSE; 

	if (d->connected != CON_NOTE_TEXT)
    while ( isspace(*argument) )
	argument++;

    ch = d->character;


    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rEnter thy name brave traveler: ", 0 );
	    return;
	}

	sprintf(kav,"%s trying to connect.", argument);
	log_string( kav );
	fOld = load_char_short( d, argument );
	ch   = d->character;

        char_age = years_old(ch);
	if ( IS_SET(ch->act, PLR_DENY))	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( check_ban(d->host,BAN_PERMIT) || check_ban(d->host,BAN_ALL) )
	{
	    write_to_buffer(d,"Your site has been banned from this mud.\n\r",0);
          sprintf( log_buf, "Banning all characters from %s.", d->host );
          log_string( log_buf );
	    close_socket(d);
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    /* Check max number of players - KaVir */

	    DESCRIPTOR_DATA *dcheck;
	    DESCRIPTOR_DATA *dcheck_next;
	    int countdesc = 0;
	    int max_players = 150;

	    for (dcheck = descriptor_list; dcheck != NULL; dcheck = dcheck_next)
	    {
		dcheck_next = dcheck->next;
		countdesc++;
	    }

	    if ( countdesc > max_players && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "Too many players connected, please try again in a couple of minutes.\n\r", 0 );
		close_socket( d );
		return;
	    }

	    if ( wizlock && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "The game is currently wizlocked. Come back a bit later.\n\r", 0 );
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Please enter password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
          if ( !is_unique_name( d, argument ) )
             {
                write_to_buffer( d, "Illegal name, try another.\n\rEnter thy name brave traveler: ", 0 );
                return;
             }

	    if (check_ban(d->host,BAN_NEWBIES))
	    {
            sprintf( log_buf, "Banning new characters from %s.", d->host );
	      log_string( log_buf );
		write_to_buffer(d,"New players are not allowed from your site.\n\r",0);
		close_socket(d);
		return;
	    }

	    /* New player */
	    sprintf( buf, "You want %s engraved on your tombstone (Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif


	if ( ch == NULL || (!IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( argument, ch->pcdata->pwd ) &&
	    strcmp( crypt ( argument, ch->pcdata->pwd ),ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}
	else if ( ch == NULL || (IS_EXTRA(ch,EXTRA_NEWPASS)  &&
	    strcmp( crypt ( argument, ch->pcdata->pwd ), ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_reconnect( d, GET_PC_NAME(ch), TRUE ) )
	    return;

	if ( check_playing( d, GET_PC_NAME(ch) ) )
	    return;

	if (ch->level > 1)
	{
	    sprintf(kav,ch->pcdata->switchname);
            free_char(d->character);
            d->character = NULL;
            fOld = load_char_obj( d, kav );
            ch   = d->character;
	}

	if ( !IS_EXTRA(ch,EXTRA_NEWPASS) && strlen(argument) > 1) {
		sprintf(kav,"%s %s",argument,argument);
		do_password(ch,kav);}

	if (ch->lasthost != NULL) free_string(ch->lasthost);
	if (ch->desc != NULL && ch->desc->host != NULL)
	{
        ch->lasthost = str_dup(ch->desc->host);
	}
	else 
	{
	    ch->lasthost = str_dup("(unknown)");
	}

	strtime = ctime( &current_time );
	strtime[strlen(strtime)-1] = '\0';
	free_string(ch->lasttime);
	ch->lasttime = str_dup( strtime );
	sprintf( log_buf, "%s@%s has connected.",ch->name, ch->lasthost );
	log_string( log_buf );

	if (IS_SET(ch->newbits, NEW_CLANNABLE)) 
	REMOVE_BIT(ch->newbits, NEW_CLANNABLE);

	d->connected	= CON_PLAYING;
      show_motd(ch);
	break;


    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "\n\rGive the Surname of your character: " );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_LAST_NAME;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "\n\rOk, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "\n\rPlease type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_LAST_NAME:
	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "\n\rIllegal surname, try another.\n\rGive the Surname of your character: ", 0 );
	    return;
	}

      ch->pcdata->lastname = str_dup(argument);

	sprintf( buf, "\n\rGive me a password for %s: %s", ch->name, echo_off_str );
	write_to_buffer( d, buf, 0 );
	d->connected = CON_GET_NEW_PASSWORD;
      break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,	"\n\rPassword must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt ( argument, ch->name );

	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d, "\n\rNew password not acceptable, try again.\n\rPassword: ", 0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );

	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

  if ( strcmp( crypt ( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "What is your sex (M/F)? ", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}

      write_to_buffer( d, "Do you wish to use ANSI (y/n)? ", 0 );
      d->connected = CON_GET_NEW_ANSI;
      break;

    case CON_GET_NEW_ANSI:
	switch ( argument[0] )
	{
	case 'y': case 'Y': SET_BIT(ch->act,PLR_ANSI);    break;
	case 'n': case 'N': break;
	default:
	    write_to_buffer( d, "Do you wish to use ANSI (y/n)? ", 0 );
	    return;
	}

	ch->class = 0;
	sprintf( log_buf, "%s@%s new player.",ch->name, d->host );
	log_string( log_buf );
	write_to_buffer( d, "\n\r", 2 );
	d->connected	= CON_PLAYING;
      show_motd(ch);
	break;

    case CON_READ_MOTD:
      show_motd(ch);
      d->connected = CON_PLAYING;
	break;

	/* states for new note system, (c)1995-96 erwin@pip.dknet.dk */
	/* ch MUST be PC here; have nwrite check for PC status! */
		
	case CON_NOTE_TO:
		handle_con_note_to (d, argument);
		break;
		
	case CON_NOTE_SUBJECT:
		handle_con_note_subject (d, argument);
		break; /* subject */
	
	case CON_NOTE_EXPIRE:
		handle_con_note_expire (d, argument);
		break;

	case CON_NOTE_TEXT:
		handle_con_note_text (d, argument);
		break;
		
	case CON_NOTE_FINISH:
		handle_con_note_finish (d, argument);
		break;

    }

    return;
}

bool is_unique_name( DESCRIPTOR_DATA *d, char *argument )
{
   DESCRIPTOR_DATA *e;

   for ( e = descriptor_list; e != NULL; e = e->next )
   {    
      if ( e->character != NULL && e != d && !strcmp( e->character->pcdata->switchname, argument ) )
         return FALSE;
   }

   return TRUE;
}


/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto immortal self someone gaia none save" ) )
	return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen(name) <  3 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix) || defined(WIN32)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) && !isdigit(*pc) )
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ))
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)  && !IS_EXTRA(ch, EXTRA_SWITCH)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character), GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "Reconnecting.\n\r", ch );
	    	if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
		    act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s reconnected.",ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
 		/* Inform the character of a note in progress and the possbility of continuation! */		
 		if (ch->pcdata->in_progress)
 			send_to_char ("You have a note in progress. Type NWRITE to continue it.\n\r",ch);

            if ( ch->pcdata->watching != NULL )
               {
                  act( "You stop watching $N.", ch, NULL, ch->pcdata->watching, TO_CHAR );
                  ch->pcdata->watching = NULL;
                  return;
               }
	    }
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Kick off old connection.  KaVir.
 */
bool check_kickoff( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character),GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "You take over your body, which was already in use.\n\r", ch );
		act( "...$n's body has been taken over by another spirit!", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s kicking off old link.", ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing - KaVir.
 * Using kickoff code from Malice, as mine is v. dodgy.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold != NULL; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? GET_PC_NAME(dold->original) : GET_PC_NAME(dold->character) ) )
	{
	    char	buf [MAX_STRING_LENGTH];
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    send_to_char("This body has been taken over!\n\r",dold->character);
	    d->connected = CON_PLAYING;
	    d->character = dold->character;
	    d->character->desc = d;
	    send_to_char( "You take over your body, which was already in use.\n\r", d->character );
	    act( "$n doubles over in agony and $s eyes roll up into $s head.", d->character, NULL, NULL, TO_ROOM );
	    act( "...$n's body has been taken over by another spirit!", d->character, NULL, NULL, TO_ROOM );
	    dold->character=NULL;
	    
	    sprintf(buf,"Kicking off old connection %s@%s",d->character->name,d->host);
	    log_string(buf);
	    close_socket(dold);	/*Slam the old connection into the ether*/
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   (ch->desc->connected != CON_PLAYING && ch->desc->connected != CON_EDITING)
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    if ( IS_CLASS( ch, CLASS_MONK ) && ch->nemesis > 0 )
       return;

    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    ch->timer = 0;
    return;
}

void sta(const char *txt,AREA_DATA *area)
{
  CHAR_DATA *rch;

  for(rch=char_list;rch!=NULL;rch=rch->next)
  {
    if ( rch->in_room->area == area )
	 stc(txt,rch);
  }
}

void stc( const char *txt, CHAR_DATA *ch )
{
    send_to_char( txt, ch );
    return;
}

int col_str_len(char *txt)
{
    int pos, len;

    len = 0;
    for (pos = 0; txt[pos] != '\0'; pos++)
    {
	if ( txt[pos] != '#' )
	{
	    len++;
	    continue;
	}

	pos++;
	if (txt[pos] == '\0') return len;
	if (txt[pos] == '#' || txt[pos] == '-') len++;
    }

    return len;
}

void cent_to_char(char *txt, CHAR_DATA *ch)
{
    int len,pos;
    char buf[MAX_STRING_LENGTH];

    len = (80 - col_str_len(txt)) / 2;
    for (pos = 0; pos < len; pos++)
    {
	buf[pos] = ' ';
    }
    buf[pos]= '\0';
    send_to_char(buf, ch);
    send_to_char(txt, ch);
    send_to_char("\n\r",ch);
}

void divide_to_char(CHAR_DATA *ch)
{
send_to_char("-------------------------------------------------------------------------------\r\n",
ch); }

void divide2_to_char(CHAR_DATA *ch)
{
send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",
ch); }

void divide3_to_char(CHAR_DATA *ch)
{
send_to_char("===============================================================================\r\n",ch);
}

void divide4_to_char(CHAR_DATA *ch)
{
send_to_char("#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6***#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-\r\n",ch);
}

void divide5_to_char(CHAR_DATA *ch)
{
cent_to_char("#4-=[#6***********#4]=-------------=[#6***********#4]=-#n",ch);
}

void divide6_to_char(CHAR_DATA *ch)
{
cent_to_char("#4-    -   -  - - -#6- ---====#7*#6====--- -#4- - -  -   - -",ch); }

void banner_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[                   ]#4=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-#n"); 
	wdth = (17 - ln) / 2 + 20;
}
else
{
	sprintf(buf,
"#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[              ] #L=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-#n");
	wdth = (11 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 22] = txt[loop];
cent_to_char(buf,ch);
}

void banner2_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#4    -   -  - - -#6- ---===#7                               #6===--- -#4- - -  -   -\r\n"); 
	wdth = (31 - ln) / 2 + 24;
}
else
{
	sprintf(buf, "#4     -    -   -  - - -#6- ---====#7                #6====--- -#4- - -  -   -    -\r\n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 6] = txt[loop];
send_to_char(buf,ch);
}


/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    CHAR_DATA *wizard;
    CHAR_DATA *familiar;

    if ( ch->desc == NULL && IS_NPC(ch) && (wizard = ch->wizard) != NULL )
    {
	   if (!IS_NPC(wizard) && (familiar = wizard->pcdata->familiar) != NULL 
	       && familiar == ch && ch->in_room != wizard->in_room)
	      {
                send_to_char("[ ",wizard);

    	         if ( txt != NULL && wizard->desc != NULL )
		      write_to_buffer( wizard->desc, txt, strlen(txt) );
	      }
    }

    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}



/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    CHAR_DATA *ech = (CHAR_DATA *) arg1;

    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;

    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    if ( ch == NULL || ch->in_room == NULL)
       return;

    to = ch->in_room->people;
    if ( type == TO_VICT || type == TO_NOTVICT )
    {
	if ( vch == NULL || vch->in_room == NULL )
         return;

	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;

      if ( to->air_timer > 0 && type != TO_CHAR ) continue;

	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
         if ( !IS_NPC(wizard) && ( (familiar=wizard->pcdata->familiar) != NULL ) && familiar == to )
	      {
                  if ( to->in_room == ch->in_room && wizard->in_room != to->in_room )
		      {
		          to = wizard;
		          is_fam = TRUE;
		      }
	      }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{
          if ( !IS_NPC( to ) || to->pIndexData->vnum < 550 || to->pIndexData->vnum > 570 )
             {
                if ( is_fam )
                   to = to_old;
                continue;
             }
	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': if ( to->seeself > 0 )
                         i = PERS( to,  to  );
                         else i = PERS( ch,  to  );
                      break;
		case 'N': if ( to->seeself > 0 )
                         i = PERS( to, to  );
                         else i = PERS( vch, to  );
                      break;
		case 'e': i = he_she  [URANGE(0, ch->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch->sex, 2)];	break;
		case 'v': if ( ech == NULL )
                             return;
                          i = PERS( ech, to );                          break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	if (is_fam)
	{
	    if ( to->in_room != ch->in_room && familiar != NULL && familiar->in_room == ch->in_room )
		 send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	if (to->desc && (to->desc->connected == CON_PLAYING))
	write_to_buffer( to->desc, buf, point - buf );

      if ( IS_NPC( to ) && to->pIndexData->vnum >= 550 && to->pIndexData->vnum <= 570 )
      {
          DESCRIPTOR_DATA *d;

          for ( d = descriptor_list; d != NULL; d = d->next )
          {
             if ( d == NULL || d->character == NULL )
                continue;

             if ( d->character->pcdata->watching == to )
                write_to_buffer( d, buf, point - buf );
          }
      }

	if (is_fam) to = to_old;

    }
    return;
}

void act2( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;

	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{

	    if (is_fam) to = to_old;

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {

		if (is_fam) to = to_old;

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {

/*		bug( "Act: missing arg2 for code %d.", *str );
*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  i = " "; break;
		case 'n': if ( ch != NULL ) i = PERS( ch,  to  );
			  else i = " ";
			  break;
		case 'N': if ( vch != NULL ) i = PERS( vch,  to  );
			  else i = " ";
			  break;
		case 'e': if ( ch != NULL ) i=he_she  [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'E': if (vch != NULL ) i=he_she  [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'm': if ( ch != NULL ) i=him_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'M': if (vch != NULL ) i=him_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 's': if ( ch != NULL ) i=his_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'S': if (vch != NULL ) i=his_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'p':
		    if (obj1 != NULL)
		    {
			i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;

		case 'P':
		    if (obj2 != NULL)
		    {
			i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );

	if (is_fam) to = to_old;

    }
    return;
}



void kavitem( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char kav[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    const char *str;
    const char *i;
    char *point;
    bool is_ok;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok) continue;
    	}
	if ( type == TO_CHAR && to != ch )
	    continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
		i = "";
	    else
	    {
		switch ( *str )
		{
		default:  i = "";					break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'o':
		    if (obj1 != NULL) sprintf(kav,"%s's",obj1->short_descr);
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "your" : kav)
			    : "something's";
		    break;

		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}
void bust_a_header(DESCRIPTOR_DATA *d)
{
char class[MAX_STRING_LENGTH];
char class2[MAX_STRING_LENGTH];
char header[MAX_STRING_LENGTH];
char header1[MAX_STRING_LENGTH];
char blanklin[MAX_STRING_LENGTH];
CHAR_DATA *ch;
char cls[MAX_STRING_LENGTH];
sprintf(cls," ");

ch=d->character;
if (ch==NULL) return;
ADD_COLOUR(ch,cls,NORMAL);

if (IS_CLASS(ch,CLASS_VAMPIRE)) sprintf(class,"Vampire");
else if (IS_CLASS(ch,CLASS_DEMON)) sprintf(class,"Demon");
else if (IS_CLASS(ch,CLASS_WEREWOLF)) sprintf(class,"Werewolf");
else sprintf(class,"Classless"); 
sprintf(class2,"%s the %s",ch->name,class);
sprintf(blanklin," ");
sprintf(header1,"%-30s Align:%-4d",class2,ch->alignment);
sprintf(header,
"\0337\033[1;1H\033[1;44m\033[1;37m%-79s%s\0338",header1,cls);
send_to_char(header,ch);
return;
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
         CHAR_DATA *ch;
         CHAR_DATA *victim;
         CHAR_DATA *tank;
   const char      *str;
   const char      *i;
         char      *point;
         char       buf  [ MAX_STRING_LENGTH ];
         char       buf2 [ MAX_STRING_LENGTH ];
	 bool       is_fighting = TRUE;

   if ( ( ch = d->character ) == NULL ) return;
   if ( ch->pcdata == NULL )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }
   if ( ch->position == POS_FIGHTING && ch->cprompt[0] == '\0' )
   {
      if ( ch->prompt[0] == '\0' )
      {
         send_to_char( "\n\r\n\r", ch );
         return;
      }
      is_fighting = FALSE;
   }
   else if ( ch->position != POS_FIGHTING && ch->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }

   point = buf;
   if ( ch->position == POS_FIGHTING && is_fighting )
      str = d->original ? d->original->cprompt : d->character->cprompt;
   else
      str = d->original ? d->original->prompt : d->character->prompt;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit );
            COL_SCALE(buf2, ch, ch->hit, ch->max_hit);
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana                              );
            COL_SCALE(buf2, ch, ch->mana, ch->max_mana);
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move                              ); 
            COL_SCALE(buf2, ch, ch->move, ch->max_move);
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp                               );
            COL_SCALE(buf2, ch, ch->exp, 1000);
            i = buf2; break;
         case 'g' :
            sprintf( buf2, "%s", show_groove( ch ) );
            i = buf2; break;
         case 'G' :
            if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_WEREWOLF ) )
               sprintf( buf2, "%d", ch->gnosis[GCURRENT] );
               else strcpy( buf2, "0" );
            i = buf2; break;
         case 'p' :
            sprintf( buf2, "%s", get_stars( ch->hit, ch->max_hit ) );
            i = buf2; break;
         case 'P' :
            sprintf( buf2, "%s", show_fbar( ch->hit, ch->max_hit ) );
            i = buf2; break;
         case 'q' :
            sprintf( buf2, "%s", get_stars( ch->mana, ch->max_mana ) );
            i = buf2; break;
         case 'Q' :
            sprintf( buf2, "%s", show_fbar( ch->mana, ch->max_mana ) );
            i = buf2; break;
         case 'c' :
            sprintf( buf2, "%s", get_stars( ch->move, ch->max_move ) );
            i = buf2; break;
         case 'C' :
            sprintf( buf2, "%s", show_fbar( ch->move, ch->max_move ) );
            i = buf2; break;
         case 'f' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               int max_hit, cur_hit;

               if ( victim->hit > 10000000 || victim->max_hit > 10000000 )
                  {
                     cur_hit = victim->hit / 10;
                     max_hit = victim->max_hit / 10;
                  }
                  else {
                          cur_hit = victim->hit;
                          max_hit = victim->max_hit;
                       }

               if ((cur_hit*100/max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((cur_hit*100/max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((cur_hit*100/max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((cur_hit*100/max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((cur_hit*100/max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'F' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else if ( ( tank = victim->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               int max_hit, cur_hit;

               if ( tank->hit > 10000000 || tank->max_hit > 10000000 )
                  {
                     cur_hit = tank->hit / 10;
                     max_hit = tank->max_hit / 10;
                  }
                  else {
                          cur_hit = tank->hit;
                          max_hit = tank->max_hit;
                       }

               if ((cur_hit*100/max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((cur_hit*100/max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((cur_hit*100/max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((cur_hit*100/max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((cur_hit*100/max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'a' :
            sprintf( buf2, "%s", IS_GOOD( ch ) ? "good"
		                  : IS_EVIL( ch ) ? "evil" : "neutral" );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'A' :
            sprintf( buf2, "%d", ch->alignment                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 't' :
            sprintf(buf2, "%d", ch->fight_timer			  );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'r' :
            if( ch->in_room )
               sprintf( buf2, "%s", ch->in_room->name                  );
            else
               sprintf( buf2, " "                                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'R' :
            if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_WEREWOLF) || IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_NINJA) ) )
            {
               sprintf( buf2, "%d", ch->rage);
               ADD_COLOUR(ch, buf2, D_RED);
            }
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'b' :
            sprintf( buf2, "%d", ch->block_meter );
            COL_SCALE(buf2, ch, ch->block_meter, 100);
            i = buf2; break;
         case 'B' :
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
            {
               sprintf( buf2, "%d", ch->pcdata->condition[COND_THIRST] );
               ADD_COLOUR(ch, buf2, D_RED);
            }
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'T' : if ( !IS_NPC( ch ) )
                       {
                          if ( ch->anger_level > 300 )
                             sprintf( buf2, "#sV#si#sO#sl#sE#sn#sT#n" );
                             else sprintf( buf2, "%d", ch->anger_level / 3 );
                       }
                       else strcpy( buf2, "0" );
                    i = buf2; break;
         case 'y' : if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
                       sprintf( buf2, "%d", ch->pcdata->powers[DRAGOON_SPIRIT] );
                       else if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_FIEND ) )
                               sprintf( buf2, "%d", ch->pcdata->powers[DEMON_TRIGGER] );
                               else strcpy( buf2, "0" );
                    i = buf2; break;
         case 'Y' : if ( !IS_NPC( ch ) && ( IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || IS_CLASS( ch, CLASS_FIEND ) ) )
                       sprintf( buf2, "%d", ch->pcdata->powers[DRAGOON_LEVEL] * 100 );
                       else strcpy( buf2, "0" );
                    i = buf2; break;
         case 's' :
         case 'S' :
            sprintf( buf2, "%s", show_style( ch ) );
            i = buf2; break;
         case 'O' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "no" );
            else if (!IS_NPC(victim) && victim != NULL && victim->pcdata->stage[1] > 0
		&& victim->pcdata->stage[2]+25 >= victim->pcdata->stage[1])
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'l' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "Nobody" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'd' :
         case 'D' :
           if ( IS_SET(ch->newbits, NEW_DARKNESS) ) 
              {
                 sprintf( buf2, "(@)" );
                 ADD_COLOUR(ch, buf2, GREY);
              }
              else sprintf( buf2, "" );
            i = buf2;
            break;
         case 'k' :
            if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_NIGHT_BLADE ) )
               sprintf( buf2, "%d", ch->pcdata->ninja_powers[NINJAKI] );
               else strcpy( buf2, "0" );
            i = buf2; break;
         case 'K' :
            if ( IS_SET(ch->act, PLR_WIZINVIS) )
               {
                  sprintf( buf2, "(!)" );
                  ADD_COLOUR(ch, buf2, GREY);
               }
               else sprintf( buf2, "" );
            i = buf2;
            break;
        case '%' :
            sprintf( buf2, "%%"                                        );
            i = buf2; break;
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, buf, point - buf );
   return;
}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif
/*

int len(char s)
{
	int l = 0;
	int x;

	if (s[0] == '\0') return 0;

	for (x = 0; x < length(s); x++)
	{
		if (s[x] == '#')
		{
			if s[x+1
		}

	}

	return l;
}
*/

/* OLC, new pager for editing long descriptions. */
/* ========================================================================= */
/* - The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom for porting  */
/*   this SillyMud code for MERC 2.0 and laying down the groundwork.         */
/* - Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which the improvements*/
/*   to the pager was modeled from.  - Kahn                                  */
/* - Safer, allows very large pagelen now, and allows to page while switched */
/*   Zavod of jcowan.reslife.okstate.edu 4000.                               */
/* ========================================================================= */

void show_string( DESCRIPTOR_DATA *d, char *input )
{
    char               *start, *end;
    char                arg[MAX_INPUT_LENGTH];
    int                 lines = 0, pagelen;

    /* Set the page length */
    /* ------------------- */

    pagelen = d->original ? d->original->pcdata->pagelen
                          : d->character->pcdata->pagelen;

    /* Check for the command entered */
    /* ----------------------------- */

    one_argument( input, arg );

    switch( UPPER( *arg ) )
    {
        /* Show the next page */

        case '\0':
        case 'C': lines = 0;
                  break;
        
        /* Scroll back a page */

        case 'B': lines = -2 * pagelen;
                  break;

        /* Help for show page */

        case 'H': write_to_buffer( d, "B     - Scroll back one page.\n\r", 0 );
                  write_to_buffer( d, "C     - Continue scrolling.\n\r", 0 );
                  write_to_buffer( d, "H     - This help menu.\n\r", 0 );
                  write_to_buffer( d, "R     - Refresh the current page.\n\r",
                                   0 );
                  write_to_buffer( d, "Enter - Continue Scrolling.\n\r", 0 );
                  return;

        /* refresh the current page */

        case 'R': lines = -1 - pagelen;
                  break;

        /* stop viewing */

        default:  free_string( d->showstr_head );
                  d->showstr_head  = NULL;
                  d->showstr_point = NULL;
                  return;
    }

    /* do any backing up necessary to find the starting point */
    /* ------------------------------------------------------ */

    if ( lines < 0 )
    {
        for( start= d->showstr_point; start > d->showstr_head && lines < 0;
             start-- )
            if ( *start == '\r' )
                lines++;
    }
    else
        start = d->showstr_point;

    /* Find the ending point based on the page length */
    /* ---------------------------------------------- */

    lines  = 0;

    for ( end= start; *end && lines < pagelen; end++ )
        if ( *end == '\r' )
            lines++;

    d->showstr_point = end;

    if ( end - start )
        write_to_buffer( d, start, end - start );

    /* See if this is the end (or near the end) of the string */
    /* ------------------------------------------------------ */

    for ( ; isspace( *end ); end++ );

    if ( !*end )
    {
        free_string( d->showstr_head );
        d->showstr_head  = NULL;
        d->showstr_point = NULL;
    }

    return;
}

void sig_handler(int sig)
{
  switch( sig )
  {
     case SIGBUS: bug("Sig handler SIGBUS.",0);
                  copyover();
                  break;
     case SIGTERM: bug("Sig handler SIGTERM.",0);
                   copyover();
                   break;
     case SIGABRT: bug("Sig handler SIGABRT",0);  
                   copyover();
                   break;
     case SIGSEGV: bug("Sig handler SIGSEGV",0);
                   copyover();
                   break;
  }
}

void init_signals()
{
   // return;

   signal(SIGBUS,sig_handler);
   signal(SIGTERM,sig_handler);
   signal(SIGABRT,sig_handler);
   signal(SIGSEGV,sig_handler);
}

#if defined( WIN32 )
void gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif
