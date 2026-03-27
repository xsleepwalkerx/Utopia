// #define unix
// #define NOCRYPT

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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#include <stdlib.h>
#include <limits.h>
#if !defined(WIN32)
#include <sys/cdefs.h>
#endif
#include <sys/time.h>  
#include "monk.h"
#include "warlock.h"
#include "arena.h"
#include "garou.h"
#include "zlib.h"

/* system calls */
int unlink();
int system();

#define TELOPT_COMPRESS 85
#define COMPRESS_BUF_SIZE 16384

/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#endif


/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef short int			sh_int;
typedef unsigned char			bool;
#endif
#include "player.h"
//#include "old.h"
/*
 * Structure types.
 */
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct	char_data		CHAR_DATA;
typedef struct	auction_data      AUCTION_DATA;

// Structure for Team Battle
typedef struct	team_data         TEAM_DATA;

typedef struct  l_board			LEADER_BOARD;

typedef struct  config_data		CONFIG_DATA;
typedef	struct	editor_data		EDITOR_DATA;

typedef struct  balance_data		BALANCE_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct	kill_data		KILL_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct  kingdom_data		KINGDOM_DATA;
typedef struct  member_data		MEMBER_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;

typedef struct  disabled_data                 DISABLED_DATA;

/* one disabled command */
struct disabled_data
{
      DISABLED_DATA *next; /* pointer to next node */
      struct cmd_type const *command; /* pointer to the command struct*/
      char *disabled_by; /* name of disabler */
      sh_int level; /* level of disabler */
};
extern                        DISABLED_DATA     *disabled_first;

struct form_type
{
    char * 	lookname;
    char *      name;
    int         hit;
    int         dam;
    int         ac;
    int         hit_mod;
    bool        squishable;
    sh_int	    size;
    bool	    can_cast;
    bool	    can_fly;
    bool	    can_wear;
    int         carry_max;
    int         max_items;
    char *	    say_word;
    char *	    move_word;
    int	    xp_bonus;
    int	    polymorph_level;
    char *	    in_room_desc;
    char *	    short_desc;
    bool	    can_use_claws;
};


struct clanrank_type
{
    char *      rank;                   /* rank name */
    int         min_level;              /* minimum level to gain that rank */
    char *	male_name;		/* rank name for males */
    char * 	female_name;		/* use some imagination */
};

struct race_type
{
    char *      name;                   /* call name of the race */
    char *      ishname;                /* elvish, gnomish etc... */
    bool        pc_race;                /* can be chosen by pcs */
    long        act;                    /* act bits for the race */
    long        aff;                    /* aff bits for the race */
    long        off;                    /* off bits for the race */
    long        imm;                    /* imm bits for the race */
    long        res;                    /* res bits for the race */
    long        vuln;                   /* vuln bits for the race */
    long        form;                   /* default form flag for the race */
    long        parts;                  /* default parts for the race */
};

/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch ) );
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );


/* Oni's plaything
 * To show how rigging is truly done
 * Just Kidding
 */
bool jope_load_char             args (( CHAR_DATA *ch, char *arg ));
void jope_free_char             args (( CHAR_DATA *ch ));
void jope_interp                args (( CHAR_DATA *ch, char *argument ));

extern  const   struct  jope_type       jope_table      [];

struct jope_type
{
  char * const        name;
  DO_FUN *            do_fun;
  sh_int              level;
};

struct bit_type
{ 
  char *const         name;
  int                 bit_value;
};


/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 8192
#define MAX_INPUT_LENGTH	  400

/*
 * Rotains Gobal Procedures
 */
void clear_stats    args( (CHAR_DATA *ch) );
void room_is_total_darkness args( (ROOM_INDEX_DATA *pRoomIndex) );
void improve_wpn    args( (CHAR_DATA *ch,int dtype, int right_hand) );
void show_spell     args( (CHAR_DATA *ch, int dtype) );
void critical_hit   args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam) );

void take_item      args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void raw_kill       args( (CHAR_DATA *victim) );
void trip           args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void disarm         args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void make_corpse    args( (CHAR_DATA *ch) );
void one_hit        args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype) );
void make_part      args( (CHAR_DATA *ch, char *argument) );

void home_write             args( ( ) );

void behead                 args( (CHAR_DATA *victim) );
void paradox                args( (CHAR_DATA *ch) );

void load_kingshit args( (void) );
void save_kingshit args( (void) );
void load_kingdoms args( (void) );
void save_kingdoms args( (void) );

void load_leaderboard args( (void) );
void save_leaderboard args( (void) );

void load_bans	args( (void) );
void save_bans	args( (void) );
void load_members  args( (void) );
void save_members  args( (void) );
void add_member  args( (CHAR_DATA *ch) );
void remove_member  args( (CHAR_DATA *ch) );

/* 
 * Godwars Game Parameters
 * By Rotain
 */
#define SKILL_ADEPT               100
#define SKILL_THAC0_32             18
#define SKILL_THAC0_00              6
#define VERSION_NUMBER              1
#define DONATION_ROOM_WEAPON     3207
#define DONATION_ROOM_ARMOR      3207
#define DONATION_ROOM_REST       3207
#define MAX_VAMPIRE_POWER           3 
#define MAX_CLAN                   11
#define MAX_DISCIPLINES		   44
#define MAX_ART                    12
#define MAX_FORGET		   10
#define MAX_KINGDOM		   11
#define MAX_WAR			   45 /* MAX_KINGDOM-1 * MAX_KINGDOM /2 total combos*/
#define MAX_SONGS		    1

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */

#define PARADOX_TICK               30
#define MAX_SKILL		  300
#define MAX_SPELL		   71
#define MAX_LEVEL		   12
#define NO_WATCH		   10
#define LEVEL_HERO		   (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 5)

#define LEVEL_MORTAL		   (MAX_LEVEL - 10)
#define LEVEL_AVATAR		   (MAX_LEVEL - 9)
#define LEVEL_APPRENTICE	   (MAX_LEVEL - 8)
#define LEVEL_MAGE		   (MAX_LEVEL - 7)
#define LEVEL_ARCHMAGE		   (MAX_LEVEL - 6)
#define LEVEL_NINJA                (MAX_LEVEL - 6)
#define LEVEL_MONK                 (MAX_LEVEL - 6)
#define LEVEL_BUILDER		   (MAX_LEVEL - 5)
#define LEVEL_QUESTMAKER	   (MAX_LEVEL - 4)
#define LEVEL_ENFORCER		   (MAX_LEVEL - 3)
#define LEVEL_JUDGE		   (MAX_LEVEL - 2)
#define LEVEL_HIGHJUDGE		   (MAX_LEVEL - 1)
#define LEVEL_IMPLEMENTOR	   (MAX_LEVEL)

#define PULSE_PER_SECOND	    3
#define PULSE_VIOLENCE		  ( 5 * PULSE_PER_SECOND)
#define OLOAD_30333		    1
/* Save the database - OLC 1.1b */
#define PULSE_DB_DUMP             (1800* PULSE_PER_SECOND ) /* 30 minutes  */


/* Status levels, change here to modify - Vic */
/*
#define STAT_AVATAR                      1
#define STAT_IMMORTAL                   20
#define STAT_GODLING                    40
#define STAT_DEMIGOD                    60
#define STAT_LESSER                     80
#define STAT_GREATER                   100
#define STAT_SUPREME                   100
*/

#define PULSE_EMBRACE             ( 5 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  ( 5 * PULSE_PER_SECOND)
#define PULSE_PLAYERS	 	  ( 5 * PULSE_PER_SECOND)
#define PULSE_TICK		  (50 * PULSE_PER_SECOND)
#define PULSE_AREA		  (50 * PULSE_PER_SECOND)
#define PULSE_WW                  ( 5 * PULSE_PER_SECOND)

struct artifact_type
{
  char * player_name;
  int    object_vnum;
};

#include "board.h"

/*
 * Site ban structure.
 */
struct	ban_data
{
    BAN_DATA *	next;
    int ban_flags, level;
    char *	name;
};



/*
 * Time and weather stuff.
 */
#define SUN_DARK                  0
#define SUN_RISE                  1
#define SUN_LIGHT                 2
#define SUN_SET                   3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3
#define SKY_SNOWY                 4
#define SKY_ICESTORM              5
#define SKY_ECLIPSE               6
#define SKY_APOCALYPSE            7

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};



/*
 * Directions.
 * Used in #ROOMS.
 */
typedef enum
{
  DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_UP, DIR_DOWN,
  DIR_NORTHEAST, DIR_NORTHWEST, DIR_SOUTHEAST, DIR_SOUTHWEST,
DIR_SOMEWHERE
} dir_types;
#define SUB_NORTH DIR_NORTH
#define SUB_EAST  DIR_EAST
#define SUB_SOUTH DIR_SOUTH
#define SUB_WEST  DIR_WEST
#define SUB_UP    DIR_UP
#define SUB_DOWN  DIR_DOWN
#define SUB_NE    DIR_NORTHEAST
#define SUB_NW    DIR_NORTHWEST
#define SUB_SE    DIR_SOUTHEAST
#define SUB_SW    DIR_SOUTHWEST


/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_SEX			 6
#define CON_GET_NEW_CLASS		 7
#define CON_GET_NEW_VT102            8
#define CON_GET_NEW_ANSI             9 
#define CON_READ_MOTD			 10
#define CON_NOT_PLAYING			 11
#define CON_EDITING			 12
#define CON_COPYOVER_RECOVER         13
#define CON_GET_LAST_NAME		 19
#define CON_ROLL_STATS               20
#define CON_GET_HEIGHT               21
#define CON_GET_WEIGHT               22
#define CON_GET_AGE                  23
#define CON_PFILE                    24


/*
 * Character substates
 */
typedef enum
{
  SUB_NONE, SUB_PAUSE, SUB_PERSONAL_DESC, SUB_OBJ_SHORT, SUB_OBJ_LONG,
  SUB_OBJ_EXTRA, SUB_MOB_LONG, SUB_MOB_DESC, SUB_ROOM_DESC, SUB_ROOM_EXTRA,
  SUB_ROOM_EXIT_DESC, SUB_WRITING_NOTE, SUB_MPROG_EDIT, SUB_HELP_EDIT,
  SUB_WRITING_MAP, SUB_PERSONAL_BIO, SUB_REPEATCMD, SUB_RESTRICTED,
  SUB_DEITYDESC,
  /* timer types ONLY below this point */
  SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;

/*
 * Timer macros.
 */

#define TIMER(ch, tmr)		((ch)->tick_timer[(tmr)])
#define SET_TIMER(ch, tmr, tm)	((ch)->tick_timer[(tmr)] = (tm))
#define ADD_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] += (tm))
#define SUB_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] -= (tm))
#define TIME_UP(ch, tmr)	((ch)->tick_timer[(tmr)] == 0 ? TRUE : FALSE)

#define RTIMER(room, rtmr)	 ((room)->tick_timer[(rtmr)])
#define SET_RTIMER(room, rtmr, rtm) ((room)->tick_timer[(rtmr)] = (rtm))
#define ADD_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] += (rtm))
#define SUB_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] -= (rtm))
#define RTIME_UP(room, rtmr)	 ((room)->tick_timer[(rtmr)] == 0 ? TRUE : FALSE)

#define TIMER_LAYONHANDS	0
#define TIMER_WRENCH		1
#define TIMER_WRENCHED 		2
#define TIMER_VAMPCALL 		3
#define TIMER_UNCONCIOUS 	4
#define TIMER_VAMP_INHABIT 	5
#define TIMER_DAEMONIC_TRANSFORM 6
#define TIMER_MESMERISE		7
#define TIMER_MESMERISED	8
#define TIMER_FORAGE		9
#define TIMER_NO_ARBOREA	11
#define TIMER_TREE_WALK		12
#define TIMER_CAN_PRAC		13
#define TIMER_CAN_EAT_ARMS	14
#define TIMER_THIRD_ARM_GROWING	15
#define TIMER_FOURTH_ARM_GROWING 16
#define TIMER_THIRD_ARM_GOING	17
#define TIMER_FOURTH_ARM_GOING  18
#define TIMER_SCALPED		19
#define TIMER_CAN_CALL_ROCKS    20
#define TIMER_CANT_BE_TURNED    21
#define TIMER_CANT_TURN    	22
#define TIMER_FIGHT_LAG    	23
#define TIMER_CAN_CHANGE_HAWK   24
#define TIMER_CAN_CREATE_SHARD  25
#define TIMER_CAN_GUST	        26
#define TIMER_CAN_ENTER_STASIS  27
#define TIMER_MAKE_SNOWMAN      28
#define TIMER_ENTOMB            29
#define TIMER_CAN_BREATHE_FROST 30
#define TIMER_HELLFIRE_SUMMON   31
#define TIMER_ON_SPEED	   	32
#define TIMER_ON_LSD		33
#define TIMER_ON_HASH		34
#define TIMER_CAN_CALL_WAR_HORSE 35
#define TIMER_WAR_HORSE_GO      36
#define TIMER_CAN_SPIT_VENOM    37
#define TIMER_CAN_GAIN_VOODOO   38
#define TIMER_CAN_FEATHER       39
#define TIMER_CAN_SHRIEK        40
#define TIMER_CAN_POLYMORPH     41
#define TIMER_DRAGON_GROW       42
#define TIMER_VAMPIRE_GROW      43
#define TIMER_SKILL_LEV1        44
#define TIMER_SKILL_LEV2        45
#define TIMER_CANT_SWARM        46
#define TIMER_CANT_BORROWLIFE   47
#define TIMER_TREE		48
#define TIMER_NEXUS_STUNNED	49
#define TIMER_GOLEM		50
#define TIMER_TAINT		51
#define TIMER_NEWBIE_IMM	52
#define TIMER_CAN_DO_NEXUS	53
#define TIMER_CAN_USE_HEALER	54
#define TIMER_DISCORD		55
#define TIMER_SPHINX_ROAR	56
#define TIMER_INFERNO		57
#define TIMER_CHAOSPORT		58
#define TIMER_CANMAJESTY	59
#define TIMER_MAJESTY		60
#define TIMER_DSLEEP		61

#define MAX_TIMER		62


#define RTIMER_STINKING_CLOUD	0
#define RTIMER_LIFE_VORTEX	1
#define RTIMER_DEATH_VORTEX	2
#define RTIMER_GLYPH_PROTECTION	3
#define RTIMER_HIDE_ROOM	4
#define RTIMER_SWARM_BEES	5
#define RTIMER_SWARM_RATS	6
#define RTIMER_SWARM_BATS	7
#define RTIMER_GHOST_LIGHT	8
#define RTIMER_NEXUS_FLAME	9
#define RTIMER_NEXUS_WATER	10
#define RTIMER_NEXUS_AIR	11
#define RTIMER_NEXUS_EARTH	12
#define RTIMER_NEXUS_ENTROPY	13
#define RTIMER_WALL_NORTH	14
#define RTIMER_WALL_EAST	15
#define RTIMER_WALL_SOUTH	16
#define RTIMER_WALL_WEST	17
#define RTIMER_WALL_UP		18
#define RTIMER_WALL_DOWN	19
#define RTIMER_DISCORD		20
#define RTIMER_DARK_ROOM	21
#define RTIMER_SILENCE		22
#define MAX_RTIMER		30



/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    char *		host;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    bool                vt102;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    char *              showstr_head;  /* From ENVY code to compile */
    char *              showstr_point; /* From ENVY code to compile */
    char *		outbuf;
    int			outsize;
    int			outtop; 
    z_stream *          out_compress;		/* mccp: support data */
    unsigned char *     out_compress_buf;
    void *              pEdit;          /* OLC */
    char **             pString;        /* OLC */
    int                 editor;         /* OLC */	
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_ALL		    4

/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    HELP_DATA * prev;
    AREA_DATA * area;
    sh_int	level;
    char *	keyword;
    char *	text;
};



/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    int		keeper;			/* Vnum of shop keeper mob	*/
    sh_int	buy_type [MAX_TRADE];	/* Item types shop will buy	*/
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
};



/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA *	next;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
    int     kingdom;
    time_t	date_stamp;
    time_t	expire;
};



/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    int                 modifier;
    int			bitvector;
};



/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    sh_int		number;
    sh_int		killed;
};



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/


/* ASCII conversions -- used so we can have letters in this file */

#define A                       1
#define B                       2
#define C                       4
#define D                       8
#define E                       16
#define F                       32
#define G                       64
#define H                       128
#define I                       256
#define J                       512
#define K                       1024
#define L                       2048
#define M                       4096
#define N                       8192
#define O                       16384
#define P                       32768
#define Q                       65536
#define R                       131072
#define S                       262144
#define T                       524288
#define U                       1048576
#define V                       2097152
#define W                       4194304
#define X                       8388608
#define Y                       16777216
#define Z                       33554432
#define aa                      67108864        /* doubled due to conflicts */
#define bb                      134217728
#define cc                      268435456    
#define ddd                     536870912
#define ee                      1073741824
#define ff                      2147483648
#define gg                      4294967256


/* These are the good old dragon/werewolf types - same variable */
#define WERE_WOLF		          1
#define WERE_RAT		          2
#define WERE_TIGER		    3
#define WERE_BEAR		          4
#define WERE_SHARK		    5

#define DRAGON_RED		    1
#define DRAGON_WHITE		    2
#define DRAGON_BLACK		    3
#define DRAGON_GREEN		    4
#define DRAGON_BLUE		    5
#define DRAGON_SILVER		    6
#define DRAGON_GOLD		    7
#define DRAGON_SHADOW		    8

/* size */
#define SIZE_TINY                       0
#define SIZE_SMALL                      1
#define SIZE_MEDIUM                     2
#define SIZE_LARGE                      3
#define SIZE_HUGE                       4
#define SIZE_GIANT                      5

/*
#define FRM_WOLF               1
#define FRM_BAT                2
#define FRM_MIST               3
#define FRM_DOLPHIN            4
#define FRM_FLAME              5
#define FRM_SPIDER             6
#define FRM_LIGHT              7
#define FRM_COCKROACH          8
#define FRM_RABBIT             9
#define FRM_BLOB               10
#define FRM_FROG               11
#define FRM_IDIOT              12
#define FRM_WHIRLWIND          13
#define FRM_CRIMSON            14
#define FRM_HAWK               15
#define FRM_WEREWOLF           16
#define FRM_FOX	               17
#define FRM_WYVERN             18
#define FRM_GARGANTUA          19
#define FRM_PEASANT            20
#define FRM_WARRIOR            21
#define FRM_BARBARIAN          22
#define FRM_GIRL               23
*/

/* The demonic forms */
#define FRM_LEMURE             1
#define FRM_NUPPERIBO          2
#define FRM_SPINAGON           3
#define FRM_BARBAZU            4
#define FRM_ABISHAI            5
#define FRM_OSYLUTH            6
#define FRM_HAMATULA           7
#define FRM_ERINYES            8
#define FRM_AMNIZU             9
#define FRM_CORNUGON           10
#define FRM_GELUGON            11
#define FRM_PIT_FIEND          12
#define FRM_PIT_LORD           13
#define FRM_DROWSPIDER	       14

/*
#define FRM_TREE               37

#define FRM_DRAGON_1           38
#define FRM_DRAGON_13          50
*/

#define FRM_WERE_RAT	       1
#define FRM_WERE_BEAR	       1
#define FRM_WERE_TIGER	       1
#define FRM_WERE_SHARK	       1
#define FRM_GOLEM	       1
#define FRM_PIXIE	       1

#define MAX_FORMS	       15

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES. flags2
 */

#define VAMP_ASHES		(A)
#define VAMP_CLONE		(B)
#define VAMP_OBJMASK		(C)
#define AFF_TOTALBLIND		(D)
#define AFF_SPIRITGUARD		(E)
#define AFF_CDONE		(F)

#define AFF_ALLOW_VAMP          (P)
#define AFF_ALLOW_WERE          (Q)
#define AFF_ALLOW_ELAD          (R)
#define AFF_ALLOW_DEMON         (S)
#define AFF_ALLOW_WARLOCK       (T)
#define AFF_CLAW                (L)
#define AFF_BITE                (M)
#define AFF_TAIL                (N)
#define AFF_WING                (O)

// flag3
#define AFF3_BLINK_1ST_RD	(A)
#define AFF3_BLINK_2ND_RD	(B)

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	   3060
#define MOB_VNUM_VAMPIRE	   3404
#define MOB_VNUM_CANNIBAL	   15

/*
 * Immunities, for players.  KaVir.
 */
#define IMM_SLASH	      1	/* Resistance to slash, slice. 		*/
#define IMM_STAB	      2	/* Resistance to stab, pierce. 		*/
#define IMM_SMASH	      4	/* Resistance to blast, pound, crush. 	*/
#define IMM_ANIMAL	      8	/* Resistance to bite, claw. 		*/
#define IMM_MISC	     16	/* Resistance to grep, suck, whip. 	*/
#define IMM_CHARM	     32	/* Immune to charm spell. 		*/
#define IMM_HEAT	     64	/* Immune to fire/heat spells. 		*/
#define IMM_COLD	    128	/* Immune to frost/cold spells.		*/
#define IMM_LIGHTNING	    256	/* Immune to lightning spells.		*/
#define IMM_ACID	    512	/* Immune to acid spells.		*/
#define IMM_SUMMON	   1024	/* Immune to being summoned.		*/
#define IMM_VOODOO	   2048	/* Immune to voodoo magic.		*/
#define IMM_VAMPIRE	   4096	/* Allow yourself to become a vampire.	*/
#define IMM_STAKE	   8192	/* Immune to being staked (vamps only). */
#define IMM_SUNLIGHT	  16384	/* Immune to sunlight (vamps only).	*/
#define IMM_SHIELDED	  32768 /* For Obfuscate. Block scry, etc.	*/
#define IMM_HURL	  65536 /* Cannot be hurled.			*/
#define IMM_BACKSTAB	 131072 /* Cannot be backstabbed.		*/
#define IMM_KICK	 262144 /* Cannot be kicked.			*/
#define IMM_DISARM	 524288 /* Cannot be disarmed.			*/
#define IMM_STEAL	1048576 /* Cannot have stuff stolen.		*/
#define IMM_SLEEP	2097152 /* Immune to sleep spell.		*/
#define IMM_DRAIN       4194304 /* Immune to energy drain.		*/
#define IMM_SHIELD2	8388608 /* Chaotic shield			*/
#define IMM_TRANSPORT  16777216 /* Objects can't be transported to you.	*/
#define IMM_TRAVEL     33554432
#define IMM_RESCUE     67108864 /* Immune to rescue */

/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		      (A)	/* Auto set for mobs	*/
#define ACT_SENTINEL		    (B)		/* Stays in one room	*/
#define ACT_SCAVENGER		    (C)		/* Picks up objects	*/
#define ACT_AGGRESSIVE		    (D)		/* Attacks PC's		*/
#define ACT_STAY_AREA		    (E)		/* Won't leave area	*/
#define ACT_WIMPY		    (F)		/* Flees when hurt	*/
#define ACT_PET			    (G)		/* Auto set for pets	*/
#define ACT_TRAIN		    (H)		/* Can train PC's	*/
#define ACT_PRACTICE		    (I)		/* Can practice PC's	*/
#define ACT_MOUNT		    (J)		/* Can be mounted	*/
#define ACT_NOPARTS		    (K)		/* Dead = no body parts	*/
#define ACT_NOEXP		    (L)		/* No exp for killing   */
#define ACT_PROTOTYPE		    (M)
#define ACT_NOAUTOKILL			(N)
#define ACT_NOEXP2			(O)
#define ACT_JAILER			(P)
#define ACT_NINJA			      (Q)
#define ACT_TIMER                   (R)
#define ACT_HELPER                  (S)

/*
 * Thingers for Demon Warps
 */

#define WARP_CBODY				1
#define WARP_SBODY				2
#define WARP_STRONGARMS				4
#define WARP_STRONGLEGS				8
#define WARP_VENOMTONG				16
#define WARP_SPIKETAIL				32
#define WARP_BADBREATH				64
#define WARP_QUICKNESS				128
#define WARP_STAMINA				256
#define WARP_HUNT				512
#define WARP_DEVOUR				1024
#define WARP_TERROR				2048
#define WARP_REGENERATE				4096
#define WARP_STEED				8192
#define WARP_WEAPON				16384
#define WARP_INFIRMITY				32768
#define WARP_GBODY				65536
#define WARP_SCARED				131072
#define WARP_MAGMA				262144
#define WARP_WEAK				524288
#define WARP_SLOW				1048576
#define WARP_VULNER				2097152
#define WARP_SHARDS				4194304
#define WARP_WINGS				8388608
#define WARP_CLUMSY				16777216
#define WARP_STUPID				33554432
#define WARP_SPOON				67108864
#define WARP_FORK				134217728
#define WARP_KNIFE				268435456
#define WARP_SALADBOWL				536870912



/* Bits for the Discie thing Numbers.. really.. not bits */

#define DISC_VAMP_CELE			2
#define DISC_VAMP_FORT			3
#define DISC_VAMP_OBTE			4
#define DISC_VAMP_PRES			5
#define DISC_VAMP_QUIE			6
#define DISC_VAMP_THAU			7
#define DISC_VAMP_AUSP			8
#define DISC_VAMP_DOMI			9
#define DISC_VAMP_OBFU			10
#define DISC_VAMP_POTE			11
#define DISC_VAMP_PROT			12
#define DISC_VAMP_SERP			13
#define DISC_VAMP_VICI			14
#define DISC_VAMP_DAIM			15
#define DISC_VAMP_ANIM			16

#define DISC_WERE_BEAR			18
#define DISC_WERE_LYNX			19
#define DISC_WERE_BOAR			20
#define DISC_WERE_OWL			21
#define DISC_WERE_SPID			22
#define DISC_WERE_WOLF			23
#define DISC_WERE_HAWK			24
#define DISC_WERE_MANT			25
#define DISC_WERE_RAPT			26
#define DISC_WERE_LUNA			27
#define DISC_WERE_PAIN			28
#define DISC_WERE_CONG			29

#define DISC_DAEM_HELL			30
#define DISC_DAEM_ATTA			31
#define DISC_DAEM_TEMP			32
#define DISC_DAEM_MORP			33
#define DISC_DAEM_CORR			34
#define DISC_DAEM_GELU			35
#define DISC_DAEM_DISC			36
#define DISC_DAEM_NETH			37
#define DISC_DAEM_IMMU			38
#define DISC_VAMP_CHIM			39
#define DISC_VAMP_THAN			40
#define DISC_VAMP_OBEA			41
#define DISC_VAMP_NECR			42
#define DISC_VAMP_MELP			43

#define DISC_SHIFTINANIMENT     44
#define DISC_SHIFTANIMENT       45
#define DISC_ARMWEAPONS         46
#define DISC_POOLFORMS          47


#define DRAGON_WINGS       1
#define DRAGON_FURY        2
#define DRAGON_TAIL        4
#define DRAGON_SIGHT       8
#define DRAGON_BREATH      16
#define DRAGON_IMMUNITY    32
#define DRAGON_SWOOP       64
#define DRAGON_INTUITION   128
#define DRAGON_CRUSH       256

#define ATTACK_NONE        0
#define ATTACK_THUNDER     1
#define ATTACK_ICE         2
#define ATTACK_EARTH       4
#define ATTACK_FIRE        8
#define ATTACK_HOLY        16
#define ATTACK_DARK        32
#define ATTACK_LUNAR       64

#define ATTACK_PHYSICAL    1
#define ATTACK_MAGICAL     2
#define ATTACK_MENTAL      3
#define ATTACK_PROJECTILE  4

#define HIT                0
#define SLASH              1
#define PIERCE             2
#define BLAST              3
#define BEAM               4
#define SOUL               5
#define BLUNT              6

#define DIRECT             0
#define LOW                1
#define AERIAL            2
#define OVERHEAD           3
#define UPPERCUT           4

#define DEI_SHIVA          1
#define DEI_IFRIT          2
#define DEI_SIREN          3
#define DEI_DIABLOS        4
#define DEI_TITAN          5
#define DEI_BIRD           6
#define DEI_ODIN           7
#define DEI_CARB           8
#define DEI_LUNA           9
#define DEI_GAIA          10

#define PER_PRIDE          1
#define PER_HONOUR         2
#define PER_COWARD         3
#define PER_COMPASSION     4
#define PER_RESERVED       5
#define PER_PSYCHO         6
#define PER_STEALTHY       7

#define MORE_DISEMPOWER   1
#define MORE_NET        2
#define MORE_STONE      4
#define MORE_NINJA      8
#define MORE_SMOKE      16
#define MORE_BLIND      32
#define MORE_VEIL       64
#define MORE_HEX        128
#define MORE_FROZEN     256
#define MORE_CONFUSION  512
#define MORE_TAUNT      1024
#define MORE_CLOUD      2048
#define MORE_SIN        4096
#define MORE_PINCH      8192
#define MORE_FOCUS      16384
#define MORE_BLADEGRASP 32768
#define MORE_WALL       65536
#define MORE_BOMB       131072
#define MORE_SAFE       262144
#define MORE_BARRIER    524288
#define MORE_IMPALE	1048576
#define MORE_HAMMER	2097152
#define MORE_DVEIL	4194304
#define MORE_BLITZ	8388608
#define MORE_LIGHT      16777216
#define MORE_MSHIELD    33554432
#define MORE_PAIN       67108864
#define MORE_FORCE      134217728
#define MORE_POISON     268435456
#define MORE_BLOOD      536870912
#define MORE_TOUCH      1073741824

#define GSUIT           1
#define IN_CLAN         2
#define MUD_VERSION     7

/* Daywalker powers */
#define DMAT   0
#define DASH   1
#define DONI   2
#define DAUS   3
#define DYOG   4
#define DTOU   5
#define DALU   6

#define MORE3_DAMNATION 1
#define MORE3_SPIRIT    2
#define MORE3_WOLF      4
#define MORE3_HOUSE     8
#define MORE3_DASH      16
#define MORE3_RAGE      32
#define MORE3_BLAST     64
#define MORE3_BOMB      128
#define MORE3_HYENA     256
#define MORE3_MIST      512
#define MORE3_DODGE     1024
#define MORE3_RETORT    2048
#define MORE3_CLAWS     4096
#define MORE3_MIRROR    8192
#define MORE3_YIN       16384
#define MORE3_YANG      32768
#define MORE3_NOSHOW    65536
#define MORE3_HAND     131072
#define MORE3_TIGER    262144
#define MORE3_SOUL     524288
#define MORE3_CLAN1    1048576
#define MORE3_CLAN2    2097152
#define MORE3_SILENCE  4194304
#define MORE3_DRAGON   8388608
#define MORE3_CRANE    16777216
#define MORE3_SNAKE    33554432
#define MORE3_TURBO    67108864
#define MORE3_FALCON   134217728
#define MORE3_MANTIS   268435456
#define MORE3_CONFUSE  536870912
#define MORE3_ONE      1073741824

/* This deals with elements */
#define ELE_FIRE        1
#define ELE_THUNDER     2
#define ELE_WATER       4
#define ELE_EARTH       8
#define ELE_WFIRE       16
#define ELE_WTHUNDER    32
#define ELE_WWATER      64
#define ELE_WEARTH      128
#define ELE_WHOLY       256
#define ELE_WDARK       512
#define ELE_SHIELD      1024
#define ELE_WLUNAR      2048
#define ELE_NONE        4096
#define ELE_BTHROUGH    8192
#define ELE_LUNAR       16384
#define ELE_LUNARP      32768

/* This deals with skillbuy */
#define SKL_KAKUSU      1
#define SKL_EYES        2
#define SKL_HEAL        4
#define SKL_BLOCK       8
#define SKL_HITRATE     16
#define SKL_DODGE       32
#define SKL_ACCELERATE  64
#define SKL_TLOCK       128
#define SKL_HUNT        256
#define SKL_STEALTH     512
#define SKL_ENCASE      1024
#define SKL_ASHIELD     2048
#define SKL_PLANAR      4096
#define SKL_PRACTICE    8192
#define SKL_KNOWLEDGE   16384
#define SKL_GRASP       32768
#define SKL_GODPEACE    65536
#define SKL_DISCHARD    131072
#define SKL_CRITICAL    262144
#define SKL_ANTICIPATE  524288
#define SKL_RETORT      1048576
#define SKL_SWERVE      2097152
#define SKL_POWERB      4194304
#define SKL_STUNA       8388608
#define SKL_MAJESTY     16777216
#define SKL_DESANCTIFY  33554432
#define SKL_FOCUS       67108864
#define SKL_BALANCE     134217728
#define SKL_ILLUSION    268435456
#define SKL_GUARD       536870912
#define SKL_FBITE       1073741824

#define SKL2_SCORCHING   1
#define SKL2_MAINTENANCE 2
#define SKL2_DLUCK       4
#define SKL2_AMIND       8
#define SKL2_ANTIAURA    16
#define SKL2_REMINISCE   32
#define SKL2_THROW       64
#define SKL2_TRIP        128
#define SKL2_MIME        256
#define SKL2_SSIGHT      512
#define SKL2_STEAL       1024
#define SKL2_PRESENCE    2048
#define SKL2_SSWAP       4096
#define SKL2_MCONTROL    8192
#define SKL2_HSOUL       16384
#define SKL2_LTOUCH      32768
#define SKL2_STBLOW      65536
#define SKL2_ROLL        131072
#define SKL2_SPINCH      262144
#define SKL2_RBODY       524288
#define SKL2_SFIST       1048576
#define SKL2_IBODY       2097152
#define SKL2_FATTACK     4194304
#define SKL2_FBLOW       8388608
#define SKL2_STAGGER     16777216
#define SKL2_SWITCH      33554432
#define SKL2_DISARM      67108864
#define SKL2_GRIP        134217728
#define SKL2_SSTRIKE     268435456
#define SKL2_SBLOW       536870912
#define SKL2_THEART      1073741824

#define MORE2_YIN       1
#define MORE2_TIE       2  
#define MORE2_RAGE      4
#define MORE2_HEAD      8
#define MORE2_NOLEGS    16
#define MORE2_NOARMS    32
#define MORE2_COMBO1    64
#define MORE2_COMBO2    128
#define MORE2_COMBO3    256
#define MORE2_COMBO4    512
#define MORE2_COMBO5    1024
#define MORE2_REVWALK   2048
#define MORE2_SPINCH    4096
#define MORE2_LEOPARD   8192
#define MORE2_TIGER     16384
#define MORE2_DRAGON    32768
#define MORE2_DIG       65536
#define MORE2_PHOENIX   131072
#define MORE2_SHIELD    262144
#define MORE2_DESPERATE 524288
#define MORE2_DESPONDENCY 1048576
#define MORE2_CHANCE      2097152
#define MORE2_GUARD       4194304
#define MORE2_STEELFIST   8388608
#define MORE2_SHADOW      16777216
#define MORE2_VORTEX      33554432
#define MORE2_MIRROR      67108864
#define MORE2_VSWORD      134217728
#define MORE2_DSMASHER    268435456
#define MORE2_WARCRY      536870912
#define MORE2_RETALIATE  1073741824

#define P_MUSIC           1

/* Assassin Bits */
#define ACHI            0
#define AKI             1
#define ANI             2
#define ALI             3
#define AZI             4
#define AMI             5
#define ACHI_METER      6
#define AKI_METER       7
#define ANI_METER       8
#define ALI_METER       9
#define AZI_METER       10
#define AMI_METER       11
#define CURRENT_METER   12

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		      1
#define AFF_INVISIBLE		      2
#define AFF_DETECT_EVIL		      4
#define AFF_DETECT_INVIS	      8
#define AFF_DETECT_MAGIC	     16
#define AFF_DETECT_HIDDEN	     32
#define AFF_SHADOWPLANE		     64 /* Creatures in shadow plane - KaVir */
#define AFF_SANCTUARY		    128
#define AFF_FAERIE_FIRE		    256
#define AFF_INFRARED		    512
#define AFF_CURSE		   1024
#define AFF_FLAMING		   2048 /* For burning creatures - KaVir */
#define AFF_POISON		   4096
#define AFF_PROTECT		   8192
#define AFF_ETHEREAL		  16384 /* For ethereal creatures - KaVir */
#define AFF_SNEAK		  32768
#define AFF_HIDE		  65536
#define AFF_OVERDRIVE	 131072
#define AFF_CHARM		 262144
#define AFF_FLYING		 524288
#define AFF_PASS_DOOR		1048576
#define AFF_POLYMORPH		2097152 /* For polymorphed creatures - KaVir */
#define AFF_SHADOWSIGHT		4194304 /* Can see between planes - KaVir */
#define AFF_WEBBED		8388608 /* Cannot move - KaVir */
#define AFF_PROTECT_GOOD       16777216
#define AFF_DROWFIRE           33554432 /* Drow Darkness - Rotain */   
#define AFF_MANA               67108864
#define AFF_MOVE              134217728
#define AFF_SHIELD            268435456
#define AFF_INFIRMITY         536870912



/* Affected_by 2 */
#define AFF_CONTRACEPTION   1
#define AFF_BEASTIE		      2
#define AFF_SEVERED		      4
#define PLR_IMPLAG		      8
#define EXTRA_BLINKY       16
#define AFF2_BALOR	   32

#define AFF2_CHALLENGED			1
#define AFF2_CHALLENGER			2
#define AFF2_INARENA			4
#define AFF2_COMPETITOR             8
#define AFF2_TEAM1                 16
#define AFF2_TEAM2                 32
#define AFF2_INTROC                64
#define AFF2_CHAIN                128
#define AFF2_WINNER               256
#define AFF2_ASSIST               512
#define AFF2_DM                  1024
#define AFF2_GIVEUP              2048
#define AFF2_AUTOGUARD           4096
#define AFF2_COMBO               8192
#define AFF2_POWER              16384
#define AFF2_DD                 32768
#define AFF2_SC                 65536
#define AFF2_UB                131072

#define OBJ_VNUM_LEGS		     46
DECLARE_DO_FUN( do_resetpassword);
DECLARE_DO_FUN( do_pstat        );

/*
 * Bits for 'itemaffect'.
 * Used in #MOBILES.
 */
#define ITEMA_SHOCKSHIELD	      1
#define ITEMA_FIRESHIELD	      2
#define ITEMA_ICESHIELD		      4
#define ITEMA_ACIDSHIELD	      8
#define ITEMA_MONKCHI                 16
#define ITEMA_CHAOSSHIELD            32
#define ITEMA_ARTIFACT               64
#define ITEMA_REGENERATE            128
#define ITEMA_SPEED                 256
#define ITEMA_VORPAL                512
#define ITEMA_PEACE                1024
#define ITEMA_PLASMA               2048
#define ITEMA_DBUSTER              4096
#define ITEMA_REFLECT              8192
#define ITEMA_RESISTANCE          16384
#define ITEMA_VISION              32768
#define ITEMA_STALKER             65536
#define ITEMA_VANISH             131072
#define ITEMA_RAGER              262144
#define ITEMA_DSTAFF             524288
#define ITEMA_GRAHF             1048576
#define ITEMA_EYES	        2097152
#define ITEMA_REGEN   	        4194304 
#define ITEMA_AFFEYE	        8388608
#define ITEMA_FIREWALL	       16777216
#define ITEMA_PROTECTION       33554432
#define ITEMA_WEREWOLF         67108864
#define ITEMA_DARKNESS        134217728
#define ITEMA_SHIELD          268435456
#define ITEMA_REAPER          536870912
#define ITEMA_ELECSHIELD     1073741824

/*
 * Newbie Pack Crap
 */
#define ITEM_NP_BREASTPLATE	30333
#define ITEM_NP_HELMET		30334
#define ITEM_NP_SLEEVES		30335
#define ITEM_NP_LEGGINGS	30336
#define ITEM_NP_GAUNTLETS	30337
#define ITEM_NP_BOOTS		30338
#define ITEM_NP_LONGSWORD	30340
#define ITEM_NP_BUCKLER		30341
#define ITEM_NP_RING		30342
#define ITEM_NP_COLLAR		30343
#define ITEM_NP_LANTERN		30344		
 





/*
 * Rune, Glyph and Sigil bits.
 */
#define RUNE_FIRE	      1
#define RUNE_AIR	      2
#define RUNE_EARTH	      4
#define RUNE_WATER	      8
#define RUNE_DARK	      16
#define RUNE_LIGHT	      32
#define RUNE_LIFE	      64
#define RUNE_DEATH	      128
#define RUNE_MIND	      256
#define RUNE_SPIRIT	      512
#define RUNE_MASTER	      1024
#define GLYPH_CREATION	      1
#define GLYPH_DESTRUCTION     2
#define GLYPH_SUMMONING	      4
#define GLYPH_TRANSFORMATION  8
#define GLYPH_TRANSPORTATION  16
#define GLYPH_ENHANCEMENT     32
#define GLYPH_REDUCTION	      64
#define GLYPH_CONTROL	      128
#define GLYPH_PROTECTION      256
#define GLYPH_INFORMATION     512
#define SIGIL_SELF	      1
#define SIGIL_TARGETING       2
#define SIGIL_AREA	      4
#define SIGIL_OBJECT	      8


/*
 * Advanced spells.
 */
#define ADV_DAMAGE	      1
#define ADV_AFFECT	      2
#define ADV_ACTION	      4
#define ADV_AREA_AFFECT       8
#define ADV_VICTIM_TARGET    16
#define ADV_OBJECT_TARGET    32
#define ADV_GLOBAL_TARGET    64
#define ADV_NEXT_PAGE       128
#define ADV_PARAMETER       256
#define ADV_SPELL_FIRST     512
#define ADV_NOT_CASTER     1024
#define ADV_NO_PLAYERS     2048
#define ADV_SECOND_VICTIM  4096
#define ADV_SECOND_OBJECT  8192
#define ADV_REVERSED      16384
#define ADV_STARTED	  32768
#define ADV_FINISHED	  65536
#define ADV_FAILED	 131072
#define ADV_MESSAGE_1	 262144
#define ADV_MESSAGE_2	 524288
#define ADV_MESSAGE_3    1048576	
#define ADV_SUPER_DAMAGE 2097152
#define ADV_ULTRA_DAMAGE 4194304

/*
 * Advanced spell actions.
 */
#define ACTION_NONE	      0
#define ACTION_MOVE	      1
#define ACTION_MOB	      2
#define ACTION_OBJECT	      3

/*
 * Advanced spell affects.
 */
#define ADV_STR			      1
#define ADV_DEX			      2
#define ADV_INT			      4
#define ADV_WIS			      8
#define ADV_CON			     16
#define ADV_SEX			     32
#define ADV_MANA		     64
#define ADV_HIT			    128
#define ADV_MOVE		    256
#define ADV_AC			    512
#define ADV_AGI                  2048
#define ADV_SAVING_SPELL	   4096
#define ADV_EVA                  8192
#define ADV_TEN                 16384



/* Colour scale macros - added 12th Aug 1995 by Calamar */

#define NO_COLOUR	""		/* Blank */
#define GREY		"[0;1;30m"	/* Dark Grey */
#define D_RED		"[0;0;31m"	/* Dark Red */
#define L_RED		"[0;1;31m"	/* Light Red */
#define D_GREEN		"[0;0;32m"	/* Dark Green */
#define L_GREEN		"[0;1;32m"	/* Light Green */
#define BROWN		"[0;0;33m"	/* Brown */
#define YELLOW		"[0;1;33m"	/* Yellow */
#define D_BLUE		"[0;0;34m"	/* Dark Blue */
#define L_BLUE		"[0;1;34m"	/* Light Blue */
#define MAGENTA		"[0;0;35m"	/* Magenta */
#define PINK		"[0;1;35m"	/* Pink */
#define D_CYAN		"[0;0;36m"	/* Dark Cyan */
#define L_CYAN		"[0;1;36m"	/* Light Cyan */
#define NORMAL		"[0;0;37m"	/* Light Grey */
#define WHITE		"[0;0;38m"	/* White */

#define ADD_COLOUR(_player,_str,_col) {char swh_temp[255]; if (!IS_NPC(_player) && IS_SET(_player->act, PLR_ANSI)) {swh_temp[0] = '\0'; strcpy(swh_temp, _col); strcat(swh_temp, _str); strcat(swh_temp, NORMAL); strcpy(_str, swh_temp);}} 

#define SCALE_COLS 4

#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) ADD_COLOUR(_swh_ch, _swh_str, (_swh_curr < 1) ? L_RED : (_swh_curr < _swh_max) ? scale[(SCALE_COLS * _swh_curr) / ((_swh_max > 0) ? _swh_max : 1)] : L_CYAN)

extern char *scale[SCALE_COLS];

/*
 * Bits for 'vampire'.
 * Used for player vampires.
 */
#define VAM_FANGS		      1
#define VAM_CLAWS		      2
#define VAM_NIGHTSIGHT		      4
#define VAM_FLYING		      8 /* For flying creatures */
#define VAM_SONIC		     16 /* For creatures with full detect */
#define VAM_CHANGED		     32 /* Changed using a vampire power */

#define VAM_PROTEAN		     64 /* Claws, nightsight, and change */
#define VAM_CELERITY		    128 /* 66%/33% chance 1/2 extra attacks */
#define VAM_FORTITUDE		    256 /* 5 hp less per hit taken */
#define VAM_POTENCE		    512 /* Deal out 1.5 times normal damage */
#define VAM_OBFUSCATE		   1024 /* Disguise and invis */
#define VAM_AUSPEX		   2048 /* Truesight, etc */
#define VAM_OBTENEBRATION	   4096 /* Shadowplane/sight and shadowbody */
#define VAM_SERPENTIS		   8192 /* Eyes/serpent, heart/darkness, etc */

#define VAM_DISGUISED		  16384 /* For the Obfuscate disguise ability */
#define VAM_MORTAL		  32768 /* For Obfuscate mortal ability. */

#define VAM_DOMINATE		  65536 /* Evileye, command */

#define VAM_EVILEYE		 131072 /* Evileye, command */

#define VAM_PRESENCE		 262144 /* Presence discipline */
#define VAM_VICISSITUDE          524288 /* Vicissitude discipline */
#define VAM_THAU                1048576 /* Thaumaturgy discipline */
#define VAM_ANIMAL              2097152 /* Animalism discipline */
#define VAM_SHIFTED             4194304 /* Non-poly shift */
#define VAM_QUIETUS             8388608 /* Quietus discipline */
#define VAM_HEAD	       16777216
#define VAM_TAIL	       33554432
#define VAM_EXOSKELETON        67108864
#define VAM_HORNS	      134217728
#define VAM_WINGS	      268435456


/*
 * Bits for 'polymorph'.
 * Used for players.
 */
#define POLY_BAT		      1
#define POLY_WOLF		      2
#define POLY_MIST		      4
#define POLY_SERPENT		      8
#define POLY_RAVEN		     16
#define POLY_FISH		     32
#define POLY_FROG		     64
#define POLY_CHEESE          32768
#define POLY_ZULOFORM               128
#define POLY_SHIFT                  256
#define POLY_SPIDER		    512
#define POLY_DRAGON		   1024
#define POLY_ZULO 		   2048
#define POLY_SPIRIT 		   4096
#define POLY_PHOENIX 		   8192
#define POLY_SOULREAVER		   16384

/*
 * Languages.
 */
#define LANG_COMMON		      0
#define DIA_OLDE		      1
#define DIA_BAD			      2
#define LANG_DARK		      4



/*
 * Score.
 */
#define SCORE_TOTAL_XP		      0
#define SCORE_HIGH_XP		      1
#define SCORE_TOTAL_LEVEL	      2
#define SCORE_HIGH_LEVEL	      3
#define SCORE_QUEST		      4
#define SCORE_NUM_QUEST		      5



/*
 * Zombie Lord.
 */
#define ZOMBIE_NOTHING		      0
#define ZOMBIE_TRACKING		      1
#define ZOMBIE_ANIMATE		      2
#define ZOMBIE_CAST		      3
#define ZOMBIE_REST		      4



/*
 * Damcap values.
 */
#define DAM_CAP		      0
#define DAM_CHANGE	      1


/* return values for check_imm */
#define IS_NORMAL               0
#define IS_DIMMUNE              1
#define IS_RESISTANT            2
#define IS_VULNERABLE           3

/* damage classes */
#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4
#define DAM_COLD                5
#define DAM_LIGHTNING           6
#define DAM_ACID                7
#define DAM_POISON              8
#define DAM_NEGATIVE            9
#define DAM_HOLY                10
#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT               15
#define DAM_OTHER               16
#define DAM_HARM                17
#define DAM_CHARM               18
#define DAM_SOUND               19

/* IMM bits for mobs */
#define DIMM_SUMMON              (A)
#define DIMM_CHARM               (B)
#define DIMM_MAGIC               (C)
#define DIMM_WEAPON              (D)
#define DIMM_BASH                (E)
#define DIMM_PIERCE              (F)
#define DIMM_SLASH               (G)
#define DIMM_FIRE                (H)
#define DIMM_COLD                (I)
#define DIMM_LIGHTNING           (J)
#define DIMM_ACID                (K)
#define DIMM_POISON              (L)
#define DIMM_NEGATIVE            (M)
#define DIMM_HOLY                (N)
#define DIMM_ENERGY              (O)
#define DIMM_MENTAL              (P)
#define DIMM_DISEASE             (Q)
#define DIMM_DROWNING            (R)
#define DIMM_LIGHT               (S)
#define DIMM_SOUND               (T)
#define DIMM_WOOD                (X)
#define DIMM_SILVER              (Y)
#define DIMM_IRON                (Z)
 
/* RES bits for mobs */
#define RES_SUMMON              (A)
#define RES_CHARM               (B)
#define RES_MAGIC               (C)
#define RES_WEAPON              (D)
#define RES_BASH                (E)
#define RES_PIERCE              (F)
#define RES_SLASH               (G)
#define RES_FIRE                (H)
#define RES_COLD                (I)
#define RES_LIGHTNING           (J)
#define RES_ACID                (K)
#define RES_POISON              (L)
#define RES_NEGATIVE            (M)
#define RES_HOLY                (N)
#define RES_ENERGY              (O)
#define RES_MENTAL              (P)
#define RES_DISEASE             (Q)
#define RES_DROWNING            (R)
#define RES_LIGHT               (S)
#define RES_SOUND               (T)
#define RES_WOOD                (X)
#define RES_SILVER              (Y)
#define RES_IRON                (Z)
 
/* VULN bits for mobs */
#define VULN_SUMMON             (A)
#define VULN_CHARM              (B)
#define VULN_MAGIC              (C)
#define VULN_WEAPON             (D)
#define VULN_BASH               (E)
#define VULN_PIERCE             (F)
#define VULN_SLASH              (G)
#define VULN_FIRE               (H)
#define VULN_COLD               (I)
#define VULN_LIGHTNING          (J)
#define VULN_ACID               (K)
#define VULN_POISON             (L)
#define VULN_NEGATIVE           (M)
#define VULN_HOLY               (N)
#define VULN_ENERGY             (O)
#define VULN_MENTAL             (P)
#define VULN_DISEASE            (Q)
#define VULN_DROWNING           (R)
#define VULN_LIGHT              (S)
#define VULN_SOUND              (T)
#define VULN_WOOD               (X)
#define VULN_SILVER             (Y)
#define VULN_IRON               (Z)


/*
 * Mounts
 */
#define IS_ON_FOOT		      0
#define IS_MOUNT		      1
#define IS_RIDING		      2
#define IS_CARRIED		      4
#define IS_CARRYING		      8




/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2



/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3

/* Demonic Transformation */

#define OBJ_VNUM_DHELM		     27924
#define OBJ_VNUM_DGREAVES            27925
#define OBJ_VNUM_DCLOAK	             27926
#define OBJ_VNUM_DARMOR              27927
#define OBJ_VNUM_DRING               27928
#define OBJ_VNUM_DSWORD              27929
#define OBJ_VNUM_DSHIELD             27930

#define OBJ_VNUM_BROD		     30073
#define OBJ_VNUM_LKLAIVE             33114
#define OBJ_VNUM_STAKE		     30011
#define OBJ_VNUM_MEDAL               29521
#define OBJ_VNUM_KHORNE		     29664
#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_FINAL_TURD	     16

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_BLOOD_SPRING	     23

#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716

/* For KaVir's stuff */
#define KINGDOM_OBJ_VNUM_START                          90000
#define OBJ_VNUM_SOULBLADE	  30000
#define OBJ_VNUM_GATE             30042
#define OBJ_VNUM_GATE2		  30072
#define OBJ_VNUM_PORTAL		  30001
#define OBJ_VNUM_EGG		  30002
#define OBJ_VNUM_EMPTY_EGG	  30003
#define OBJ_VNUM_SPILLED_ENTRAILS 30004
#define OBJ_VNUM_QUIVERING_BRAIN  30005
#define OBJ_VNUM_SQUIDGY_EYEBALL  30006
#define OBJ_VNUM_SPILT_BLOOD      30007
#define OBJ_VNUM_VOODOO_DOLL      30010
#define OBJ_VNUM_RIPPED_FACE      30012
#define OBJ_VNUM_TORN_WINDPIPE    30013
#define OBJ_VNUM_CRACKED_HEAD     30014
#define OBJ_VNUM_SLICED_EAR	  30025
#define OBJ_VNUM_SLICED_NOSE	  30026
#define OBJ_VNUM_KNOCKED_TOOTH	  30027
#define OBJ_VNUM_TORN_TONGUE	  30028
#define OBJ_VNUM_SEVERED_HAND	  30029
#define OBJ_VNUM_SEVERED_FOOT	  30030
#define OBJ_VNUM_SEVERED_THUMB	  30031
#define OBJ_VNUM_SEVERED_INDEX	  30032
#define OBJ_VNUM_SEVERED_MIDDLE	  30033
#define OBJ_VNUM_SEVERED_RING	  30034
#define OBJ_VNUM_SEVERED_LITTLE	  30035
#define OBJ_VNUM_SEVERED_TOE	  30036
#define OBJ_VNUM_PROTOPLASM	  30037
#define OBJ_VNUM_QUESTCARD	  30039
#define OBJ_VNUM_QUESTMACHINE	  30040
#define OBJ_VNUM_COPPER           30049
#define OBJ_VNUM_IRON             30050
#define OBJ_VNUM_STEEL            30051
#define OBJ_VNUM_ADAMANTITE       30052

#define MOB_VNUM_GUARDIAN	  13
#define MOB_VNUM_SERVANT          13
#define MOB_VNUM_MOUNT		  6
#define MOB_VNUM_FROG		  7
#define MOB_VNUM_RAVEN		  8
#define MOB_VNUM_CAT		  9
#define MOB_VNUM_DOG		  10
#define MOB_VNUM_EYE		  12
#define MOB_VNUM_FIRE             36  
#define MOB_VNUM_STONE            38  
#define MOB_VNUM_IRON             37  
#define MOB_VNUM_CLAY             200004

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT            1
#define ITEM_SCROLL           2
#define ITEM_WAND             3
#define ITEM_STAFF            4
#define ITEM_WEAPON           5
#define ITEM_TREASURE         8
#define ITEM_ARMOR            9
#define ITEM_POTION           10
#define ITEM_FURNITURE        12
#define ITEM_TRASH            13
#define ITEM_CONTAINER        15
#define ITEM_DRINK_CON        17
#define ITEM_KEY              18
#define ITEM_FOOD             19
#define ITEM_MONEY            20
#define ITEM_BOAT	            22
#define ITEM_CORPSE_NPC       23
#define ITEM_CORPSE_PC        24
#define ITEM_FOUNTAIN         25
#define ITEM_PILL             26
#define ITEM_PORTAL           27
#define ITEM_EGG              28
#define ITEM_VOODOO           29
#define ITEM_STAKE            30
#define ITEM_MISSILE          31 /* Ammo vnum, cur, max, type */
#define ITEM_AMMO             32 /* ???, dam min, dam max, type */
#define ITEM_QUEST            33
#define ITEM_QUESTCARD        34
#define ITEM_QUESTMACHINE     35
#define ITEM_SYMBOL           36
#define ITEM_BOOK             37
#define ITEM_PAGE             38
#define ITEM_TOOL	            39
#define ITEM_WALL             40
#define ITEM_COPPER           41
#define ITEM_IRON	            42
#define ITEM_STEEL            43
#define ITEM_ADAMANTITE	      44
#define ITEM_GEMSTONE         45
#define ITEM_HILT             46
#define ITEM_DTOKEN           47
#define ITEM_HEAD             48
#define ITEM_KINGDOM_POWER   	50
#define ITEM_DRAGONGEM        51
#define ITEM_WGATE            52
#define ITEM_INSTRUMENT       53
#define ITEM_BOMB             54
#define ITEM_HUNT             55
#define ITEM_PKTOKEN          56

/*
 * Weapon Stats
 */

#define WEAPON_FLAMING          (A)
#define WEAPON_FROST            (B)
#define WEAPON_VAMPIRIC         (C)
#define WEAPON_SHARP            (D)
#define WEAPON_VORPAL           (E)
#define WEAPON_TWO_HANDS        (F)
#define WEAPON_SHOCKING         (G)
#define WEAPON_POISON           (H)
#define WEAPON_NOENCASE         (I)
#define WEAPON_JAGGED           (J)
#define WEAPON_SILVER           (K)
#define WEAPON_EVOCATE          (L)
#define TRUE_FIRE               (M)
#define TRUE_BOLT               (N)
#define TRUE_ICE                (O)
#define TRUE_EARTH              (P)
#define WEAPON_ELE_FLAME        (Q)
#define WEAPON_ELE_WATER        (R)
#define WEAPON_ELE_EARTH        (S)
#define WEAPON_ELE_AIR          (T)
#define WEAPON_NPOISON          (U)
#define WEAPON_ICEBURST         (X)
#define WEAPON_HOLYWATER        (V)
#define TRUE_MOON               (W)
#define TRUE_SAINT              (Y)
#define TRUE_DARK               (Z)

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		      1
#define ITEM_HUM		      2
#define ITEM_THROWN		      4
#define ITEM_KEEP		      8
#define ITEM_VANISH		     16
#define ITEM_INVIS		     32
#define ITEM_MAGIC		     64
#define ITEM_NODROP		    128
#define ITEM_BLESS		    256
#define ITEM_ANTI_GOOD		    512
#define ITEM_ANTI_EVIL		   1024
#define ITEM_ANTI_NEUTRAL	   2048
#define ITEM_NOREMOVE		   4096
#define ITEM_INVENTORY		   8192
#define ITEM_LOYAL              16384
#define ITEM_SHADOWPLANE        32768
#define ITEM_PROTOTYPE            (L)
#define ITEM_MENCHANT		  65536
#define ITEM_DETONATE          131072
#define ITEM_AUCTION           262144
#define ITEM_TWOHANDS          524288
#define ITEM_ROBE             1048576
#define ITEM_SPECIAL          2097152
#define ITEM_RENAME           4194304

/* Item extra flags II, the return of the item flags!  */

#define ITEM_ARM		  (A)
#define ITEM_NYSTUL               (B)
#define ITEM_NO_INTERRUPT         (C)
#define ITEM_DAEMONSEED		  (F)
#define ITEM_VOO_HEAD		  (G)
#define ITEM_VOO_DEAD		  (H)
#define ITEM_VOO_BODY		  (I)
#define ITEM_VOO_THREAD		  (J)
#define ITEM_INGRED_GLOIN	  (K)
#define ITEM_INGRED_FROGBREATH    (L)
#define ITEM_INGRED_PAPACOCO	  (M)
#define ITEM_INGRED_MULDALEAF	  (N)
#define ITEM_INGRED_SCULLYWEED    (O)
#define ITEM_INGRED_WORMWART	  (P)
#define ITEM_INGRED_TILLIFREEN    (Q)
#define ITEM_INGRED_BAJUJU        (R)
#define ITEM_ATTACK_GOOD          (S)
#define ITEM_ITEMHIDE	          (T)
#define ITEM_ICE                  (U)

/* artifact and relic flags */

#define ITEM_TELEPORTS		   (W)  /* teleports upon owner death */
#define ITEM_DESTROYED		   (X)  /* destroyed upon owner death */
#define ITEM_UNIQUE		   (Y)  /* only 1 in game ever */
#define ITEM_DESIRED		   (Z)  /* you can't let go of it ever */
#define ITEM_INDESTRUCTABLE	   (aa) /* guess */
#define ITEM_TELEPORT_PROTECTION   (bb) /* teleports when attempts to destroy it */
#define ITEM_KNOW_OWNER 	   (cc) /* owner displayed on artifact command */

#define ITEM_FLYING	           (dd)
#define ITEM_FORM_MELDED	   (ee)



/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		      1
#define ITEM_WEAR_FINGER	      2
#define ITEM_WEAR_NECK		      4
#define ITEM_WEAR_BODY		      8
#define ITEM_WEAR_HEAD		     16
#define ITEM_WEAR_LEGS		     32
#define ITEM_WEAR_FEET		     64
#define ITEM_WEAR_HANDS		    128 
#define ITEM_WEAR_ARMS		    256
#define ITEM_WEAR_SHIELD	    512
#define ITEM_WEAR_ABOUT		   1024 
#define ITEM_WEAR_WAIST		   2048
#define ITEM_WEAR_WRIST		   4096
#define ITEM_WIELD		   8192
#define ITEM_HOLD		  16384
#define ITEM_WEAR_FACE		  32768



/*
 * Special types.
 * Used in #OBJECTS for special items - KaVir.
 */
#define SITEM_ACTIVATE		      1
#define SITEM_TWIST		      2
#define SITEM_PRESS		      4
#define SITEM_PULL		      8
#define SITEM_TARGET		      16
#define SITEM_SPELL		      32
#define SITEM_TRANSPORTER	      64
#define SITEM_TELEPORTER	      128
#define SITEM_DELAY1		      256
#define SITEM_DELAY2		      512
#define SITEM_OBJECT		     1024
#define SITEM_MOBILE		     2048
#define SITEM_ACTION		     4096
#define SITEM_MORPH		     8192
#define SITEM_SILVER		    16384
#define SITEM_WOLFWEAPON	    32768
#define SITEM_DROW		    65536
#define SITEM_CHAMPWEAPON	   131072
#define SITEM_DEMONIC		   262144
#define SITEM_HIGHLANDER	   524288
#define SITEM_COPPER              1048576
#define SITEM_MAGE		  2097152
#define SITEM_STEEL		  4194304
#define SITEM_ADAMANTITE	  8388608
#define SITEM_GEMSTONE           16777216
#define SITEM_HILT               33554432
#define SITEM_PDEMONIC		 67108864
#define SITEM_MONK		134217728 
#define SITEM_IRON		268435456
/*
 * Apply types (for quest affects).
 * Used in #OBJECTS.
 */
#define QUEST_STR		      1
#define QUEST_DEX		      2
#define QUEST_INT		      4
#define QUEST_WIS		      8
#define QUEST_CON		     16
#define QUEST_HITROLL		     32
#define QUEST_DAMROLL		     64
#define QUEST_HIT		    128
#define QUEST_MANA		    256
#define QUEST_MOVE		    512
#define QUEST_AC		   1024

#define QUEST_NAME		   2048
#define QUEST_SHORT		   4096
#define QUEST_LONG		   8192
#define QUEST_FREENAME		  16384

#define QUEST_ENCHANTED		  32768
#define QUEST_SPELLPROOF	  65536
#define QUEST_ARTIFACT		 131072
#define QUEST_IMPROVED		 262144
#define QUEST_MASTER_RUNE	 524288
#define QUEST_RELIC             1048576
#define QUEST_BLOODA            2097152
#define QUEST_CLONED            4194304
#define QUEST_ZOMBIE            8388608
#define QUEST_FORGE            16777216
#define ITEM_EQUEST	       33554432
#define QUEST_MAGEENCHANT      67108864
#define QUEST_MALLEABLE       134217728 

/*
 * Tool types.
 */
#define TOOL_PEN		      1
#define TOOL_PLIERS		      2
#define TOOL_SCALPEL		      4



/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_POLY		     25
#define APPLY_EVA                  26
#define APPLY_AGI                  27
#define APPLY_LUCK                 28
#define APPLY_SAVING_FIRE          29
#define APPLY_SAVING_THUNDER       30
#define APPLY_SAVING_HOLY          31
#define APPLY_SAVING_SLASH         32
#define APPLY_SAVING_PIERCE        33
#define APPLY_SAVING_BLUNT         34
#define APPLY_SAVING_BLAST         35
#define APPLY_SAVING_BEAM          36
#define APPLY_SAVING_SPIRIT        37
#define APPLY_SAVING_NORMAL        38
#define APPLY_SAVING_PHYSICAL      39
#define APPLY_SAVING_MAGICAL       40
#define APPLY_SAVING_ICE           41
#define APPLY_SAVING_EARTH         42
#define APPLY_SAVING_DARK          43
#define APPLY_LUNARS               44
#define APPLY_DARKS                45
#define APPLY_HOLYS                46
#define APPLY_FIRES                47
#define APPLY_ICES                 48
#define APPLY_BOLTS                49
#define APPLY_EARTHS               50
#define APPLY_SAVING_LUNAR         51
#define APPLY_SAVING_NATURAL       52
#define APPLY_NATURALS             53
#define APPLY_TENSION              54


/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8



/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_HELL		   700
#define ROOM_VNUM_TEMPLE	   800
#define ROOM_VNUM_ALTAR		   800
#define ROOM_VNUM_SCHOOL	   33000
#define ROOM_VNUM_DISCONNECTION	  3
#define ROOM_VNUM_IN_OBJECT	  700

#define NON_SAFE 1

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_TRAP_CALTROPS    1
#define ROOM_TRAP_LANDMINE    2
#define ROOM_TRAP_BOULDERS    3
#define ROOM_TRAP_NIGHTMARE   4
#define ROOM_TRAP_JAGGAR      5
#define ROOM_TRAP_TRANSPORT   6
#define ROOM_TRAP_STAKE       7

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		      1
#define ROOM_NOSTOCK          2
#define ROOM_NO_MOB		4
#define ROOM_INDOORS		8
#define ROOM_ASSASSIN         16
#define ROOM_QUICKSAND        32
#define ROOM_WHIRLWIND        64
#define ROOM_SHIFT            256
#define ROOM_PRIVATE		512
#define ROOM_SAFE		      1024
#define ROOM_SOLITARY		2048
#define ROOM_PET_SHOP		4096
#define ROOM_NO_RECALL		8192
#define ROOM_NO_TELEPORT	16384
#define ROOM_TOTAL_DARKNESS   32768
#define ROOM_ICYSWORD         65536
#define ROOM_ARENA            131072
#define ROOM_FLAMING          262144
#define ROOM_SILENCE          524288
#define ROOM_CATACOMB         1048576
#define ROOM_PROTOTYPE		2097152
#define ROOM_ENTOMB           4194304
#define ROOM_BLAZE            8388608
#define ROOM_KIOTIC           16777216
#define ROOM_LAVAPIT          33554432
#define ROOM_FLAME            67108864
#define ROOM_BARRIER          134217728 
#define ROOM_LUMICA 	      268435456
#define ROOM_NOMOVE           536870912
#define ROOM_ICETRAP          1073741824

/*
 * Room text flags (KaVir).
 * Used in #ROOMS.
 */
#define RT_LIGHTS		      1 /* Toggles lights on/off */
#define RT_SAY			      2 /* Use this if no others powers */
#define RT_ENTER		      4
#define RT_CAST			      8
#define RT_THROWOUT		     16 /* Erm...can't remember ;) */
#define RT_OBJECT		     32 /* Creates an object */
#define RT_MOBILE		     64 /* Creates a mobile */
#define RT_LIGHT		    128 /* Lights on ONLY */
#define RT_DARK			    256 /* Lights off ONLY */
#define RT_OPEN_LIFT		    512 /* Open lift */
#define RT_CLOSE_LIFT		   1024 /* Close lift */
#define RT_MOVE_LIFT		   2048 /* Move lift */
#define RT_SPELL		   4096 /* Cast a spell */
#define RT_PORTAL		   8192 /* Creates a one-way portal */
#define RT_TELEPORT		  16384 /* Teleport player to room */

#define RT_ACTION		  32768
#define RT_BLANK_1		  65536
#define RT_BLANK_2		 131072

#define RT_RETURN		1048576 /* Perform once */
#define RT_PERSONAL		2097152 /* Only shows message to char */
#define RT_TIMER		4194304 /* Sets object timer to 1 tick */



/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      1
#define EX_CLOSED		      2
#define EX_LOCKED		      4
#define EX_PICKPROOF		     32
#define EX_NOPASS                    64
#define EX_EASY                     128
#define EX_HARD                     256
#define EX_INFURIATING              512  
#define EX_NOCLOSE                 1024
#define EX_NOLOCK                  2048
#define EX_ICE_WALL                4096
#define EX_FIRE_WALL               8192
#define EX_SWORD_WALL             16384
#define EX_PRISMATIC_WALL         32768
#define EX_IRON_WALL              65536	
#define EX_MUSHROOM_WALL         131072
#define EX_CALTROP_WALL          262144
#define EX_ASH_WALL              524288
#define EX_WARDING              1048576
#define EX_ICE_SWORDS           2097152
#define EX_STAKE_WALL           4194304

#define MAX_EXFLAG		      20
#define MAX_WALL		       8



/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNUSED		      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_MAX		     11



/*
 * Equipment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FINGER_L		      1
#define WEAR_FINGER_R		      2
#define WEAR_NECK_1		      3
#define WEAR_NECK_2		      4
#define WEAR_BODY		      5
#define WEAR_HEAD		      6
#define WEAR_LEGS		      7
#define WEAR_FEET		      8
#define WEAR_HANDS		      9
#define WEAR_ARMS		     10
#define WEAR_SHIELD		     11
#define WEAR_ABOUT		     12
#define WEAR_WAIST		     13
#define WEAR_WRIST_L		     14
#define WEAR_WRIST_R		     15
#define WEAR_WIELD		     16
#define WEAR_HOLD		     17
#define WEAR_THIRD		     18
#define WEAR_FOURTH		     19
#define WEAR_FACE		     20
#define WEAR_SCABBARD_L		     21
#define WEAR_SCABBARD_R		     22
#define WEAR_BACK 		     23
#define MAX_WEAR		     24




/*
 * Locations for damage.
 */
#define LOC_HEAD		      0
#define LOC_BODY		      1
#define LOC_ARM_L		      2
#define LOC_ARM_R		      3
#define LOC_LEG_L		      4
#define LOC_LEG_R		      5

/*
 * For Head
 */
#define LOST_EYE_L		       1
#define LOST_EYE_R		       2
#define LOST_EAR_L		       4
#define LOST_EAR_R		       8
#define LOST_NOSE		      16
#define BROKEN_NOSE		      32
#define BROKEN_JAW		      64
#define BROKEN_SKULL		     128
#define LOST_HEAD		     256
#define LOST_TOOTH_1		     512 /* These should be added..... */
#define LOST_TOOTH_2		    1024 /* ...together to caculate... */
#define LOST_TOOTH_4		    2048 /* ...the total number of.... */
#define LOST_TOOTH_8		    4096 /* ...teeth lost.  Total..... */
#define LOST_TOOTH_16		    8192 /* ...possible is 31 teeth.   */
#define LOST_TONGUE		   16384

/*
 * For Body
 */
#define BROKEN_RIBS_1		       1 /* Remember there are a total */
#define BROKEN_RIBS_2		       2 /* of 12 pairs of ribs in the */
#define BROKEN_RIBS_4		       4 /* human body, so not all of  */
#define BROKEN_RIBS_8		       8 /* these bits should be set   */
#define BROKEN_RIBS_16		      16 /* at the same time.          */
#define BROKEN_SPINE		      32
#define BROKEN_NECK		      64
#define CUT_THROAT		     128
#define CUT_STOMACH		     256
#define CUT_CHEST		     512

/*
 * For Arms
 */
#define BROKEN_ARM		       1
#define LOST_ARM		       2
#define LOST_HAND		       4
#define LOST_FINGER_I		       8 /* Index finger */
#define LOST_FINGER_M		      16 /* Middle finger */
#define LOST_FINGER_R		      32 /* Ring finger */
#define LOST_FINGER_L		      64 /* Little finger */
#define LOST_THUMB		     128
#define BROKEN_FINGER_I		     256 /* Index finger */
#define BROKEN_FINGER_M		     512 /* Middle finger */
#define BROKEN_FINGER_R		    1024 /* Ring finger */
#define BROKEN_FINGER_L		    2048 /* Little finger */
#define BROKEN_THUMB		    4096

/*
 * For Legs
 */
#define BROKEN_LEG		       1
#define LOST_LEG		       2
#define LOST_FOOT		       4
#define LOST_TOE_A		       8
#define LOST_TOE_B		      16
#define LOST_TOE_C		      32
#define LOST_TOE_D		      64 /* Smallest toe */
#define LOST_TOE_BIG		     128
#define BROKEN_TOE_A		     256
#define BROKEN_TOE_B		     512
#define BROKEN_TOE_C		    1024
#define BROKEN_TOE_D		    2048 /* Smallest toe */
#define BROKEN_TOE_BIG		    4096

/*
 * For Bleeding
 */
#define BLEEDING_HEAD		       1
#define BLEEDING_THROAT		       2
#define BLEEDING_ARM_L		       4
#define BLEEDING_ARM_R		       8
#define BLEEDING_HAND_L		      16
#define BLEEDING_HAND_R		      32
#define BLEEDING_LEG_L		      64
#define BLEEDING_LEG_R		     128
#define BLEEDING_FOOT_L		     256
#define BLEEDING_FOOT_R		     512


/*
 * For Spec powers on players
 */
#define EYE_SPELL		       1 /* Spell when they look at you */
#define EYE_SELFACTION		       2 /* You do action when they look */
#define EYE_ACTION		       4 /* Others do action when they look */




/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2



/*
 * Stats - KaVir.
 */
#define STAT_STR		      0
#define STAT_END		      1
#define STAT_REF		      2
#define STAT_FLE		      2



/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_MEDITATING		      5
#define POS_SITTING		      6
#define POS_RESTING		      7
#define POS_FIGHTING                  8
#define POS_STANDING                  9
/* Status of Arena */

// Bits for Areas ((area)->bits)
#define WASTE (A)
#define UPDATEWASTE (B)
#define MAGMA (C)
#define ROOM_ANTI_LIGHT (D)

/*
 * ACT bits for players.
 */
/*
#define PLR_EMBRACING		      4	
#define PLR_EMBRACED		    256	
Seriosly bugged bits for some reason dont enable them they arent used*/
#define PLR_IS_NPC		      1		/* Don't EVER set.	*/
#define PLR_AUTOEXIT		      8
#define PLR_AUTOLOOT		     16
#define PLR_AUTOSAC                  32
#define PLR_BLANK		     64
#define PLR_BRIEF		    128
#define PLR_COMBINE		    512
#define PLR_PROMPT		   1024
#define PLR_TELNET_GA		   2048
#define PLR_HOLYLIGHT		   4096
#define PLR_WIZINVIS		   8192
#define PLR_ANSI		  16384
#define	PLR_SILENCE		  32768
#define PLR_VT102       	  65536
#define PLR_INCOG                131072
#define PLR_NO_TELL		 262144
#define PLR_LOG			 524288
#define PLR_DENY		1048576
#define PLR_FREEZE		2097152
#define PLR_BRIEF2	       16777216
#define PLR_WATCHER	       33554432
#define PLR_ACID               67108864
#define PLR_CHALLENGER        134217728
#define PLR_CHALLENGED        268435456
#define PLR_TAG               536870912
#define PLR_NOTAG            1073741824

#define TAG_PLAYING		(A)
#define TAG_FROZEN		(B)
#define TAG_RED			(C)
#define TAG_BLUE		(D)

/*New bits for playrs (Infidel)*/
#define NEW_SLAM       1
#define NEW_QUILLS     2
#define NEW_JAWLOCK    4
#define NEW_PERCEPTION 8
#define NEW_SKIN       16
#define NEW_SHADSTR    32
#define NEW_ALIGHT     64
#define NEW_REND       128
#define NEW_MONKFLAME  256
#define NEW_SCLAWS     512
#define NEW_NOWATCH   1024
#define NEW_MONKCLOAK 2048
#define NEW_MONKADAM  4096
#define NEW_MONKSKIN  8192
#define NEW_BARKSKIN  16384
#define NEW_CLOAK     32768
#define NEW_DROWHATE  65536
#define NEW_DARKNESS  131072
#define THIRD_HAND    262144
#define FOURTH_HAND   524288
#define NEW_CLANNABLE 1048576
#define NEWBIE_PACK   2097152 // Sigh Sage you make things hard :)
#define NEW_NATURAL   4194304
#define NEW_POWER     8388608
#define NEW_DFORM     16777216
#define NEW_MASTERY     33554432
#define NEW_DARKDODGE   33554432
#define NEW_DARKTENDRILS 67108864
#define NEW_MULTIARMS   134217728
#define NEW_BLADESPIN   268435456
#define NEW_FIGHTDANCE	536870912
#define NEW_CUBEFORM	1073741824  //Please work dammit

/*
 * EXTRA bits for players. (KaVir)
 */
#define EXTRA_LOWDEF                1
#define FROUND				2
#define EXTRA_TRUSTED		      4
#define EXTRA_NEWPASS		      8
#define EXTRA_OSWITCH		     16
#define EXTRA_SWITCH		     32
#define EXTRA_FBITE		     64
#define TIED_UP			    128
#define GAGGED			    256
#define BLINDFOLDED		    512
#define EXTRA_MULTI		   1024 
#define EXTRA_DONE		   2048
#define EXTRA_HUNT               4096
#define EXTRA_PREGNANT		   8192
#define EXTRA_LABOUR		  16384
#define EXTRA_BORN		  32768
#define EXTRA_PROMPT		  65536
#define EXTRA_MARRIED		 131072
#define EXTRA_AFK        	 262144
#define EXTRA_LTOUCH           524288 
#define EXTRA_CALL_ALL	      1048576
#define EXTRA_DETERMINE       2097152
#define EXTRA_BSD	            4194304
#define EXTRA_SUBDUE          8388608
#define EXTRA_PLASMA         16777216
#define EXTRA_DANCE          33554432
#define EXTRA_LOWOFF         67108864
#define EXTRA_ROT           134217728
#define EXTRA_ZOMBIE        268435456
#define EXTRA_TLOCK         536870912
#define EXTRA_FLASH        1073741824
/* 2147483648 */

/* 
 * AGE Bits.
 */
#define AGE_CHILDE		      0
#define AGE_NEONATE		      1
#define AGE_ANCILLA		      2
#define AGE_ELDER		            3
#define AGE_METHUSELAH		      4
#define AGE_LA_MAGRA		      5
#define AGE_TRUEBLOOD               6


#define BAN_PERMANENT               1
#define BAN_ALL                     2
#define BAN_PERMIT                  4
#define BAN_NEWBIES                 8
#define BAN_PREFIX                  16
#define BAN_SUFFIX                  32


/* other shit */
#define COMM_NOARENA		0
#define COMM_AFK		1
#define COMM_NORESTORE		2
#define SHL_INVISIBLE		3

/*
 * Obsolete bits.
 */
#if 0
#define PLR_NO_SHOUT		 131072	/* Obsolete	*/
#endif



/*
 * Channel bits.
 */
#define	CHANNEL_AUCTION		      1
#define	CHANNEL_CHAT		      2
#define	CHANNEL_ANGEL		      4
#define	CHANNEL_IMMTALK		      8
#define	CHANNEL_MUSIC		     16
#define	CHANNEL_CLANTALK  	     32
#define	CHANNEL_SHOUT		     64
#define	CHANNEL_YELL		    128
#define	CHANNEL_VAMPTALK	    256
#define	CHANNEL_HOWL		    512
#define	CHANNEL_LOG		       1024
#define	CHANNEL_PRAY		   2048
#define	CHANNEL_INFO		   4096
#define	CHANNEL_DRAGTALK		   8192
#define	CHANNEL_TELL		  16384
#define	CHANNEL_MAGETALK      32768
#define CHANNEL_HIGHTALK	  65536
#define CHANNEL_KTALK		 131072
#define CHANNEL_SIGN		 262144
#define	CHANNEL_MONK         524288
#define CHANNEL_MIKTALK		1048576	
#define CHANNEL_NECRO	2097152
#define CHANNEL_COMMUNICATE	4194304
#define CHANNEL_KNIGHTTALK      8388608
#define CHANNEL_ROAR	16777216
#define CHANNEL_PALADIN 33554432
#define CHANNEL_SAMURAI 134217728
#define CHANNEL_AMAZON 268435456
#define CHANNEL_PROBE 536870912

struct l_board
{
  char *pk_name;
  int pk_number;
  char *pd_name;
  int pd_number;
  char *tt_name;
  int tt_number;
  char *qc_name;
  int qc_number;
  char *mk_name;
  int mk_number;
  char *md_name;
  int md_number;
  char *bestpk_name;
  int bestpk_number;
  char *pkpd_name;
  int pkpd_number;
  char *t_name;
  int t_number;
};

struct kingdom_data
{
	OBJ_DATA *object;
	int num;
	char *name;
	char *who_name;
	char *leader_name;
	char *leader2_name;
	int pk;
	int pd;
	int wk;
      int members;
	int recall;
	int altar;
	int money;
};

struct member_data
{
	char *name;
	int remort;
	int trimort;
	int quamort;
	int quimort;
      int class;
};

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    SHOP_DATA *		pShop;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    AREA_DATA *         area;           /* OLC */
    char *		hunting;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		morph;
    char *		createtime;
    char *		pload;
    char *		lasttime;
    char *		lasthost;
    char *		powertype;
    char *		poweraction;
    char *		prompt;
    char *		cprompt;
    sh_int		spectype;
    sh_int		specpower;
    int		        loc_hp	[7];
    int			vnum;
    sh_int		count;
    sh_int		killed;
    sh_int		sex;
    sh_int		mounted;
    sh_int		home;
    int		        level;
    int			immune;
    int			polyaff;
    int			vampaff;
    int			itemaffect;
    int			vamppass;
    int			form;
    int			act;
    int			extra;
    int	affected_by;
    int	affected_by2;
    int		alignment;
    int		hitroll;		/* Unused */
    int		ac;			/* Unused */
    int		hitnodice;		/* Unused */
    int		hitsizedice;		/* Unused */
    int		hitplus;		/* Unused */
    int		damnodice;		/* Unused */
    int		damsizedice;		/* Unused */
    int		damplus;		/* Unused */
    int			gold;			/* Unused */
   /*int                 special;
    int                 class; */
};

struct balance_data
{
    int			pkills;
    int			pkilled;
    int			members;
    int			total_score;
    int			position;
    int			total_gen;
    int			num_tertiary;
    int			total_alignment;
};

struct	editor_data
{
    sh_int		numlines;
    sh_int		on_line;
    sh_int		size;
    char		line[49][81];
};

struct config_data
{
    int		demons;
    int		mages;
    int		vampires;
    int		werewolves;
    int		drow;
    int		monks;  
    int		ninjas;
};

// Structure for Team Battle
struct	team_data
{
    TEAM_DATA *      next;
    char *           win_type[2];         // Type of wins for each round. S - Special KO, K - KO etc
    char *           team_name[2];        // Name of team
    int              team_power[2];       // A quantitive indicator of the team's strength
    int              striker_points[2];   // How many Striker points available to the team
    int              power_mode[2];       // Which Power mode the fighter is in
    int              power_duration[2];   // How much time left the fighter has in the Power mode
    int              round;               // The current round the fight is in
    CHAR_DATA *      challenger[4];       // The ch pointers for the challenging team
    CHAR_DATA *      opponent[4];         // The ch pointers for the challenged team
    int              team_numbers[2];     // No. of members in each team remaining
    int              team_eliminated[2];  // Shows how many team mates had been knocked out
};

/*
 * Auction structure
 */
struct	auction_data
{
    AUCTION_DATA *      next;
    OBJ_DATA *          object;
    int                 current_bid;
    int                 bidding_time;
    CHAR_DATA *         buyer;
    CHAR_DATA *         seller;
};

/*
 * One character (PC or NPC).
 */
struct	char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		prev_in_room;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *         attacker;
    CHAR_DATA *         caller;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *         embracing;
    CHAR_DATA *         embraced;
    CHAR_DATA *		blinkykill;
    CHAR_DATA *		reply;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    CHAR_DATA *		binding;
    CHAR_DATA *		binded;
    CHAR_DATA *		retaliate;
    CHAR_DATA *		holding;
    CHAR_DATA *		holded;
    CHAR_DATA *		protector;
    CHAR_DATA *		protecting;
    CHAR_DATA *         challenger; /*  person who challenged you */
    CHAR_DATA *         challenged; /*  person who you challenged */
    CHAR_DATA *         gladiator;  /*  ARENA player wagered on */
    int                 team;  /*  TEAM player wagered on */
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;
    OBJ_DATA *		carrying;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    ROOM_INDEX_DATA *	old_room;
    PC_DATA *		pcdata;
    DO_FUN *		last_cmd;
    DO_FUN *		prev_cmd;   /* mapping */
    int		tag_flags;
    float         dam_boost;
    char *		hunting;
    char *		name;
    char *	      pload;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		morph;
    char *		createtime;
    char *		lasttime;
    char *		lasthost;
    char *		poweraction;
    char *		powertype;
    char *		prompt;
    char *		cprompt;
    char *		prefix;
    sh_int		sex;
    sh_int		race;
    sh_int			class, class_2, class_3, class_4, class_5;
    int			armweaps;
    int			shiftdrag;
    int			immune;
    int 		mflags;
    int 		polyaff;
    int			vampaff_a;
    int			fight_timer;
    int                 gang_factor;
    int			safe_timer;
    int			login_timer;
    int			itemaffect;
    int			vamppass_a;
    int			form;
    int			warp;
    int			explevel;
    int			expgained;
    int			levelexp;
    int                 power[MAX_DISCIPLINES];
    int			xhitroll;
    int			xdamroll;
    int                 lunar_bonus, dark_bonus, holy_bonus, fire_bonus, ice_bonus, bolt_bonus, earth_bonus, natural_bonus;
    int                 fire_prof, ice_prof, bolt_prof, earth_prof, holy_prof, dark_prof, physical_prof, magical_prof,
                        slash_prof, pierce_prof, blast_prof, beam_prof, spirit_prof, blunt_prof, normal_prof, lunar_prof,
                        natural_prof;

/* SMAUUUUUUUUUUUG */
    void *		dest_buf;
    void *		spare_ptr;
    int			tempnum;
    EDITOR_DATA *	editor;
    sh_int		substate;
    int			pagelen;                        /* BUILD INTERFACE */
    sh_int		inter_page;                     /* BUILD INTERFACE */
    sh_int		inter_type;                     /* BUILD INTERFACE */
    char  		*inter_editing;                 /* BUILD INTERFACE */
    int			inter_editing_vnum;             /* BUILD INTERFACE */
    sh_int		inter_substate;                 /* BUILD INTERFACE */
/* End O' Smaug */

    /* Dh Flags */
    int		cclan;
    int		flag2;
    int		flag3;
    int		flag4;
    sh_int 	      generation;
    sh_int		primary;
    sh_int		proper_size;
    sh_int		size;
    sh_int		cur_form;
    sh_int	      dragtype;
    int           rage;
    sh_int		siltol;
/* end */
    sh_int		tick_timer[MAX_TIMER];
    sh_int		warpcount;
    sh_int		vampgen_a;
    sh_int		spectype;
    sh_int		specpower;
    sh_int		loc_hp	[7];
    sh_int		wpn	[13];
    sh_int		spl	[8];
    int           cmbt  [8];
    int		beast;
    sh_int		mounted;
    int		home;
    int		timer;
    int           force;
    int           version;
    int		clannum;
    int           nemesis;
    int           powerlevel, saiyanlevel;
    int           dark_thinking;
    int           dragonform;
    int           special_id, special_id_2;
    int           aura[40];
    int           mlist[12];
    int           nlist[12];
    int           clist[20];
    int           demmak;
    int           special_aura[40];
    int           inner_aura, outer_aura, group_aura;
    int           drill, guarded, guarding;
    int           arena;
    int           arena_timer;
    int           gshield;
    int           more;
    int           more2;
    int           more3;
    int           p_bit;
    int           air_timer;
    int           ctimer[20];
    int           mergence;
    char  		*groove;
    bool          counta;
    int           tier, raiding, anger_level;
    int           block_meter, blocking, stunning_timer, roll_timer;
    int           obeah_timer, monk_timer, saiyan_timer, ninja_timer, mage_timer, throw_timer, daramon_timer, aragami_timer, null_timer,
                  dragoon_timer, divine_timer, dragon_timer, vampirestuff, rage_timer, jedi_timer, plasma_timer, noblock, nodamage,
                  fear_timer, lightning_timer, control_timer, poison_timer, freeze_timer, hunt_timer, blind_timer, stunned,
                  seeself, noprotection, revdam, revregen, noskill, absorb_timer, stone_timer, break_timer;
    int           scavenger_item, power_timer, use_timer, defiance_timer, mort_timer;
    int           strike_timer, topmode;
    int           agg;
    int		level;
    sh_int		trust;
    int           played;
    time_t		logon;
    time_t		save_time;
    sh_int		wait;
    int           pkill;
    int           wkill;
    int           pdeath;
    int           mkill;
    int           mdeath;
    int           hit;
    int           ele_attack, elemental, attack_type, attack_focus, attack_var;
    int           max_hit;
    int           round;
    int           total_damage, total_hits;
    int           achi;
    int		mana;
    int		max_mana;
    int		move;
    int		max_move;
    int           gold;
    int           bank;
    int           exp;
    int           act;
    int	      extra;
    int           newbits;
    int           special;
    int 	      affected_by;
    int 	      affected_by2;
    int		position;
    int		practice;
    int		carry_weight;
    int		carry_number;
    int		saving_throw;
    int		alignment;
    int		hitroll;
    int		damroll;
    int		armor;
    int		wimpy;
    int		deaf;
    int		paradox		[3];
    int		damcap		[2];
    int			monkstuff, ninjastuff, saiyanstuff;
    int			monkcrap, ninjacrap;
    int			monkab [4];
    int			angelab [4];
    int			paladinab [4];
    int			sorcerorab [7];
    int			arrowab [5];
    int			saiyanab [4];
    int			saiyan_powerlevel, saiyan_drill;
    int           dpower;
    sh_int		chi [2];
    int           energy[2];
    sh_int		sam_ki [2], sam_chi [2];
    sh_int		vlife [2];
    sh_int		dlife [2];
    char *		clan;
    int			gifts[21], dskills[21];
    int			garou1;
    int			garou2;
    sh_int		gnosis[2];
    CHAR_DATA *		unveil;
    char *		objdesc;
    sh_int		monkblock;
    sh_int		focus[2];
    int           wfocus;
    int           squest[2];
	int			spheres[9];/*mages*/
	int			imms[3];/*mages*/
	int			quint[3];/*mages*/
};



/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    CHAR_DATA *         pfile;
    CHAR_DATA *		familiar;
    CHAR_DATA *		partner;
    CHAR_DATA *		propose;
    CHAR_DATA *		watching;
    OBJ_DATA *		chobj;
    OBJ_DATA *		memorised;
    BOARD_DATA *	board; /* The current board */
    time_t		last_note[MAX_BOARD]; /* last note for the boards */
    NOTE_DATA *		in_progress;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
    char *		conception;
    char *		parents;
    char *		cparents;
    char *		marriage;
    char *		email;
    char *		forget[MAX_FORGET];
    char *		switchname;
    char *		lastname;
    char *        	custom_enter_room;
    char *        	enter_room;
    char *		subprompt;	/* Substate prompt */
    char *		login;
    char *		logout;
    char *		decap;
    char *		decap_2;
    sh_int		souls;
    sh_int		perm_str;
    sh_int		perm_con;
    sh_int		perm_int;
    sh_int		perm_wis;
    sh_int		perm_dex;
    sh_int		perm_eva;
    sh_int		perm_agi;
    sh_int		perm_luck;
    sh_int		perm_tension;
    sh_int		mod_str;
    sh_int		mod_con;
    sh_int		mod_int;
    sh_int		mod_wis;
    sh_int		mod_dex;
    sh_int		mod_eva;
    sh_int		mod_agi;
    sh_int		mod_luck;
    sh_int		mod_tension;
    bool                confirm_delete;
    int                 age;
    int                 height;
    int                 weight;
    int                 capture_timer;
    int 		questsrun;   
    int			questtotal;
    int			quest;
    int	 		kingdom;
    int                 faithpoints;
    int 		pagelen;
    int			powers[20];
    int			ninja_powers[20];
    int 		stats[12];
    int			disc_points;
    int			disc_research;
    sh_int		stage[3];
    int			score[6];
    int			genes[10];
    int			obj_vnum;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int		stat_ability	[4];
    sh_int		stat_amount	[4];
    sh_int		stat_duration	[4];
    sh_int		exhaustion;
    sh_int              followers;
    int               clanrank;
    int               transmigration;
    int               team;
    int                 plr_wager; /*  ARENA amount wagered     */
    int                 awins;     /*  ARENA number of wins     */
    int                 twins;     /*  TOURNAMENT number of wins     */
    int                 alosses;   /*  ARENA number of losses  */
    int			comm;
    int                 security;       /* OLC - Builder security */
    int			bounty;
    int          dragon_age;
    int          hidden_skill;
    int          forge;
    int          deity;
    int          personality;
    int          dragon_hit, dragon_magic, dragon_armor, dragon_dam, dragon_ac, dragon_damcap, dragon_attack, dragon_power, dragon_toughness;
    int          dragon_power_bonus, dragon_toughness_bonus;
    int			magejunk[10];
    int          legend;
    int          blood_meter, blood_power;
};

LEADER_BOARD leader_board;
MEMBER_DATA member[1000];
KINGDOM_DATA king_table[MAX_KINGDOM + 1];

/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		19

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    EXTRA_DESCR_DATA *prev;	/* Previous			    */
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};



/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect; 
    AFFECT_DATA *	affected;
    AREA_DATA *         area;           /* OLC */
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int			vnum;
    sh_int		item_type;
    int           extra_flags;
    int           extra_flags2;
    sh_int		wear_flags;
    sh_int		count;
    sh_int		weight;
    sh_int		weapflags;
    int			spectype;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int           quest;
    int           points;
    int           cost;			/* Unused */
    int           value[4];
};



/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    CHAR_DATA *		carried_by;
    CHAR_DATA *		chobj;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int		item_type;
    int		extra_flags;
    int		extra_flags2;
    int		wear_flags;
    int		wear_loc;
    int		weight;
    int		weapflags;
    int			spectype;
    int			specpower;
    int		condition;
    int		toughness;
    int		resistance;
    int           quest;
    int           points;
    int           cost;
    int		level;
    int		timer;
    int			value	[4];
};



/*
 * Exit data.
 */
struct	exit_data
{
    EXIT_DATA *		prev;		/* previous exit in linked list	*/
    EXIT_DATA *		next;		/* next exit in linked list	*/
    EXIT_DATA *		rexit;		/* Reverse exit pointer		*/
    ROOM_INDEX_DATA *	to_room;	/* Pointer to destination room	*/
    char *		keyword;	/* Keywords for exit or door	*/
    char *		description;	/* Description of exit		*/
    int			vnum;		/* Vnum of room exit leads to	*/
    int			rvnum;		/* Vnum of room in opposite dir	*/
    int			exit_info;	/* door states & other flags	*/
    int			key;		/* Key vnum			*/
    sh_int		vdir;		/* 0,5 N\E\S\W\U\D shit		*/
    int                 rs_flags;       /* OLC */
    int                 orig_door;      /* OLC */

};

/*
 * Room text checking data.
 */
typedef struct roomtext_data
{
    int				type;
    int				power;
    int				mob;
    char *			input;
    char *			output;
    char *			choutput;
    char *			name;
    struct roomtext_data	*next;
} ROOMTEXT_DATA;



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    int		arg1;
    int		arg2;
    int		arg3;
};



/*
 * Area definition.
 */
struct  area_data   
{
    AREA_DATA *         next;
    char *              name;
    int                 recall;
    int                 land;
    int                 age;
    int                 nplayer;
    char *              filename;       /* OLC */
    char *              builders;       /* OLC - Listing of builders */
    int                 security;       /* OLC - Value 0-infinity  */
    int                 lvnum;          /* OLC - Lower vnum */
    int                 uvnum;          /* OLC - Upper vnum */
    int                 vnum;           /* OLC - Area vnum  */
    int                 area_flags;     /* OLC */
    int			bits;
};


/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    ROOM_INDEX_DATA *   next_room;
    CHAR_DATA *		people;
    CHAR_DATA *		first_person;
    CHAR_DATA *		last_person;
    OBJ_DATA *		contents;
    OBJ_DATA *		first_content;
    OBJ_DATA *		last_content;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AREA_DATA *		area;
    EXIT_DATA *		exit	  [6];
    EXIT_DATA *		first_exit;
    EXIT_DATA *		last_exit;
    ROOMTEXT_DATA *	roomtext;
    RESET_DATA *        reset_first;    /* OLC */
    RESET_DATA *        reset_last;     /* OLC */

    char *		track     [5];
    char *		name;
    char *		description;
    int			vnum;
    int		        room_flags;
    int		        bits;
    int                   drows;
    int           trap;
    sh_int		light;
    sh_int		blood;
    sh_int		track_dir [5];
    sh_int		sector_type;
    sh_int		tick_timer[MAX_RTIMER];
};



/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000



/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4

#define TAR_OBJ_CHAR_DEF            5
#define TAR_OBJ_CHAR_OFF            6
#define TAR_OBJ_ROOM		    7
#define TAR_EXIT		    8
#define TAR_CHAR_WORLD		    9

#define TARGET_CHAR                 0
#define TARGET_OBJ                  1
#define TARGET_ROOM                 2
#define TARGET_NONE                 3

#define PURPLE_MAGIC		    0
#define RED_MAGIC		    1
#define BLUE_MAGIC		    2
#define GREEN_MAGIC		    3
#define YELLOW_MAGIC		    4



/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    sh_int	skill_level;	        /* Level needed by class	*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	slot;			/* Slot for #OBJECT loading	*/
    sh_int	min_mana;		/* Minimum mana used		*/
    sh_int	beats;			/* Waiting time after use	*/
    char *	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
};



/*
 * These are skill_lookup return values for common skills and spells.
 */
extern bool             armageddon;
extern bool             melee;
extern bool             mybet;
extern sh_int		mysafe;
extern sh_int		angels;
extern sh_int		first_team;
extern int              arena_wait;
extern int              pose_timer;
extern int              pose_limit;
extern int              scavenger_items;
extern int              scavenger_time;
extern int              lucky_token;
extern int              dungeon_timer;
extern int              betting_points;
extern int              is_dungeon;
extern sh_int		second_team;
extern sh_int		third_team;
extern sh_int		betting_time;
extern int              armageddon_timer;
extern sh_int		nogang;
extern int	            nofighting;
extern int	            doubleheal;
extern int	            eipheal;
extern int	            altima;
extern int	            jaggar;
extern int	            conan;
extern int	            conaneffect;
extern int	            jaggareffect;
extern int	            endozone;
extern CHAR_DATA        *participants[ 16 ];
extern CHAR_DATA        *temp[ 16 ];
extern char             *battle_name[ 16 ];
extern int              num_contestants;
extern int              battle_num;


extern CHAR_DATA        *auron;
extern CHAR_DATA        *krizalid;
extern CHAR_DATA        *jaggarm;
extern CHAR_DATA        *altimam;
extern CHAR_DATA        *conanm;
extern CHAR_DATA        *eiphealm;
extern char             *man_match;
extern int              man_kills;
extern int              w1_kills;
extern int              w2_kills;
extern int              w1_kdg;
extern int              w2_kdg;
extern int              mud_version;

extern sh_int      gsn_arrows;
extern sh_int      gsn_holy_bonus;
extern sh_int      gsn_concentration;

extern sh_int		gsn_stuntubes;
extern sh_int		gsn_laser;
extern sh_int   gsn_stinger;
extern sh_int	  gsn_music;
extern  sh_int    gsn_quills;
extern  sh_int    gsn_cheapshot;
extern  sh_int    gsn_shred;
extern  sh_int    gsn_heavenlyaura;
extern  sh_int    gsn_bladespin;
extern	sh_int	gsn_fiery;
extern	sh_int	gsn_tail;
extern	sh_int	gsn_breath;
extern  sh_int  gsn_hooves;
extern  sh_int	gsn_claws;
extern  sh_int    gsn_fireball;
extern  sh_int    gsn_iceshard;
extern  sh_int    gsn_deathaura;
extern  sh_int    gsn_lightningslash;
extern  sh_int    gsn_wrathofgod;
extern sh_int  gsn_darktendrils;
extern sh_int  gsn_mageshield;
extern  sh_int  gsn_venomtong;
extern  sh_int  gsn_spiketail;
extern  sh_int  gsn_badbreath;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_hellfire;
extern  sh_int  gsn_shards;
extern  sh_int  gsn_spiderform;
extern  sh_int  gsn_garotte;
extern	sh_int	gsn_backstab;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern  sh_int	gsn_godbless;  /* Vic - Monks */
extern  sh_int	gsn_totalblind; /* Vic - Monks */
extern	sh_int	gsn_tendrils;
extern	sh_int	gsn_berserk;
extern	sh_int	gsn_punch;
extern	sh_int	gsn_headbutt;
extern  sh_int  gsn_spiket;
extern  sh_int  gsn_venomt;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_shards;
extern sh_int gsn_shiroken;
extern  sh_int  gsn_blinky;
extern  sh_int  gsn_inferno;
extern  sh_int  gsn_fangs;
extern  sh_int  gsn_buffet;
extern  sh_int  gsn_venom_strike;
extern  sh_int  gsn_rfangs;
extern	sh_int	gsn_sweep;
extern	sh_int	gsn_knee;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_hurl;
extern	sh_int	gsn_kick;
extern  sh_int  gsn_chillhand;
extern sh_int gsn_circle;
extern sh_int gsn_booming;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_track;
extern	sh_int	gsn_polymorph;
extern	sh_int	gsn_web;
extern  sh_int  gsn_infirmity;
extern sh_int	gsn_drowfire;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_mass_invis;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;
extern	sh_int	gsn_paradox;
extern sh_int gsn_spew;
extern	sh_int	gsn_darkness;
extern      sh_int      gsn_multiplearms;
extern      sh_int      gsn_tail_image;
extern      sh_int	gsn_darts;


/*
 * Utility macros.
 */
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)	((var) ^= (bit))

#define COLOUR(ch,color,bit)    (ADD_COLOUR((ch),(bit),(color));)


/*
 * Memory allocation macros.
 */

#define CREATE(result, type, number) do { if (!((result) = (type *) calloc ((number), sizeof(type))))	{ perror("malloc failure"); abort(); } } while(0)

#define RECREATE(result,type,number) do { if (!((result) = (type *) realloc ((result), sizeof(type) * (number))))	{ perror("realloc failure"); abort(); } } while(0)

#define DISPOSE(point) do { if (!(point))  { bug( "Freeing null pointer",0 );	fprintf( stderr, "DISPOSEing NULL in %s, line %d\n", __FILE__, __LINE__ ); } else free(point); point = NULL; } while(0)

#ifdef HASHSTR
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	quick_link((point))
#define QUICKMATCH(p1, p2)	(int) (p1) == (int) (p2)
#define STRFREE(point) do { if (!(point))  { bug( "Freeing null pointer",0 ); } else if (str_free((point))==-1) fprintf( stderr, "STRFREEing bad pointer in %s, line %d\n", __FILE__, __LINE__ ); } while(0)
#else
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	str_dup((point))
#define QUICKMATCH(p1, p2)	strcmp((p1), (p2)) == 0
#define STRFREE(point) do { if (point == NULL) { bug( "Freeing null pointer",0 ); } else free_string((point)); } while(0)
#endif

/* double-linked list handling macros -Thoric */

#define LINK(link, first, last, next, prev) do { if ( !(first) ) (first)			= (link); else (last)->next		= (link); (link)->next		= NULL; (link)->prev		= (last); (last)			= (link); } while(0)

#define INSERT(link, insert, first, next, prev) do { (link)->prev		= (insert)->prev; if ( !(insert)->prev ) (first)			= (link); else (insert)->prev->next	= (link); (insert)->prev		= (link); (link)->next		= (insert); } while(0)

#define UNLINK(link, first, last, next, prev) do { if ( !(link)->prev ) (first)			= (link)->next; else (link)->prev->next	= (link)->next; if ( !(link)->next ) (last)			= (link)->prev; else (link)->next->prev	= (link)->prev; } while(0)


#define CHECK_LINKS(first, last, next, prev, type) do { type *ptr, *pptr = NULL; if ( !(first) && !(last) )  break; if ( !(first) ) { bug( "CHECK_LINKS: last with NULL first!  %s.", __STRING(first) ); for ( ptr = (last); ptr->prev; ptr = ptr->prev ); (first) = ptr; } else if ( !(last) ) {  bug( "CHECK_LINKS: first with NULL last!  %s.", __STRING(first) ); for ( ptr = (first); ptr->next; ptr = ptr->next ); (last) = ptr; } if ( (first) ) { for ( ptr = (first); ptr; ptr = ptr->next )  {  if ( ptr->prev != pptr ) {  bug( "CHECK_LINKS(%s): %p:->prev != %p.  Fixing.", __STRING(first), ptr, pptr ); ptr->prev = pptr; }  if ( ptr->prev && ptr->prev->next != ptr ) { bug( "CHECK_LINKS(%s): %p:->prev->next != %p.  Fixing.", __STRING(first), ptr, ptr ); ptr->prev->next = ptr;  } pptr = ptr; } pptr = NULL; } if ( (last) ) { for ( ptr = (last); ptr; ptr = ptr->prev ) { if ( ptr->next != pptr ) { bug( "CHECK_LINKS (%s): %p:->next != %p.  Fixing.", __STRING(first), ptr, pptr ); ptr->next = pptr; } if ( ptr->next && ptr->next-


#define ASSIGN_GSN(gsn, skill) do { if ( ((gsn) = skill_lookup((skill))) == -1 ) fprintf( stderr, "ASSIGN_GSN: Skill %s not found.\n", (skill) ); } while(0)

#define CHECK_SUBRESTRICTED(ch) do { if ( (ch)->substate == SUB_RESTRICTED ) { send_to_char( "You cannot use this command from within another command.\n\r", ch ); return; } } while(0)


/*
 * Character macros.
 */

#define IS_COMB(ch, sn)		(IS_SET((ch)->monkcrap, (sn)))
#define IS_FS(ch, sn)		(IS_SET((ch)->monkstuff, (sn)))
#define IS_NEWFLAG(ch, sn)      (IS_SET((ch)->flag2, (sn)))
#define IS_CREATOR(ch)		(get_trust(ch) >= MAX_LEVEL)
#define IS_BASE_FORM(ch)	(((ch)->cur_form >= 38 && (ch)->cur_form <= 50) || ((ch)->cur_form >=23 && (ch)->cur_form <= 36) || ((ch)->cur_form >= FRM_WERE_RAT && (ch)->cur_form <= FRM_WERE_SHARK) || (ch)->cur_form == FRM_PIXIE)
#define GET_FORM(ch)            ((form_data[(ch)->cur_form].short_desc == NULL || form_data[(ch)->cur_form].short_desc[0] == '\0') ? form_data[(ch)->cur_form].name : "")
#define GET_PROPER_NAME(ch)     (IS_NPC((ch)) ? (ch)->short_descr : (ch)->pcdata->switchname)
#define GET_PC_NAME(ch)     	(IS_NPC((ch)) ? "<npc>" : (ch)->pcdata->switchname)

#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_JUDGE(ch)		(get_trust(ch) >= LEVEL_JUDGE)
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)		(get_trust(ch) >= LEVEL_HERO)

#define IS_DAY()	    	((weather_info.sunlight == SUN_RISE || weather_info.sunlight == SUN_LIGHT))
#define IS_NIGHT()	    	((!IS_DAY()))

#define CAN_PK(ch)		(get_trust(ch)>= 3 && get_trust(ch)<= 12)
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
#define IS_AFF2(ch, sn)		(IS_SET((ch)->affected_by2, (sn)))
#define IS_SPEAKING(ch, sn)	(IS_SET((ch)->pcdata->language[0], (sn)))
#define CAN_SPEAK(ch, sn)	(IS_SET((ch)->pcdata->language[1], (sn)))
#define IS_ITEMAFF(ch, sn)	(IS_SET((ch)->itemaffect, (sn)))
#define IS_IMMUNE(ch, sn)	(IS_SET((ch)->immune, (sn)))
#define IS_VAMPAFF(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_AFF], (sn)))
#define IS_VAMPPASS(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_CURRENT], (sn))) 
#define IS_FORM(ch, sn)		(IS_SET((ch)->form, (sn)))
#define IS_POLYAFF(ch, sn)	(IS_SET((ch)->polyaff, (sn)))
#define IS_EXTRA(ch, sn)	(IS_SET((ch)->extra, (sn)))
#define IS_CURSED(whome)                                        (IS_AFFECTED(whome, AFF_CURSE))
#define IS_DEMPOWER(ch, sn)	( IS_SET((ch)->pcdata->powers[DPOWER_FLAGS], (sn)))
#define IS_DEMAFF(ch, sn)	( IS_SET((ch)->pcdata->powers[DPOWER_CURRENT], (sn)))
#define IS_DRAGONPOWER(ch, sn)( IS_SET((ch)->pcdata->powers[9], (sn)) )
#define IS_CLASS(ch, CLASS)	( (ch)->class == CLASS || (ch)->class_2 == CLASS || (ch)->class_3 == CLASS || (ch)->class_4 == CLASS || (ch)->class_5 == CLASS )
#define IS_HEAD(ch, sn)		(IS_SET((ch)->loc_hp[0], (sn)))
#define IS_BODY(ch, sn)		(IS_SET((ch)->loc_hp[1], (sn)))
#define IS_ARM_L(ch, sn)	(IS_SET((ch)->loc_hp[2], (sn)))
#define IS_ARM_R(ch, sn)	(IS_SET((ch)->loc_hp[3], (sn)))
#define IS_LEG_L(ch, sn)	(IS_SET((ch)->loc_hp[4], (sn)))
#define IS_LEG_R(ch, sn)	(IS_SET((ch)->loc_hp[5], (sn)))
#define IS_BLEEDING(ch, sn)	(IS_SET((ch)->loc_hp[6], (sn)))

#define IS_PLAYING( d )         (d->connected==CON_PLAYING)
 
#define IS_GOOD(ch)		(ch->alignment >= 1)
#define IS_EVIL(ch)		(ch->alignment <= -1)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define GET_AC(ch)		((ch)->armor + ( IS_AWAKE(ch) ? dex_app[get_curr_dex(ch)].defensive : 0 ))
#define GET_HITROLL(ch)		((ch)->hitroll+str_app[get_curr_str(ch)].tohit)
#define GET_DAMROLL(ch)		((ch)->damroll+str_app[get_curr_str(ch)].todam)

#define IS_OUTSIDE(ch)		(!IS_SET((ch)->in_room->room_flags,ROOM_INDOORS))

#define WAIT_STATE(ch, npulse) if (!IS_CREATOR((ch))) ((ch)->wait = UMAX((ch)->wait, (npulse)))



/*
 * Object Macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))
#define IS_OBJ_STAT2(obj, stat) (IS_SET((obj)->extra_flags2,(stat)))
#define IS_WEAP(obj, stat)      (IS_SET((obj)->weapflags,  (stat)))


/*
 * Description macros.
 */
#define PERS(ch, looker)	( IS_SET( looker->more3, MORE3_CONFUSE ) || IS_SET( looker->more, MORE_CONFUSION ) ? "Someone familar" : ( can_see( looker, (ch) ) ? ( IS_NPC(ch) ? (ch)->short_descr : ( IS_AFFECTED( (ch), AFF_POLYMORPH) ?	(ch)->morph : (ch)->name ) ) : "someone" ) )



/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *	do_fun;
    sh_int		position;
    sh_int		level;
    sh_int		log;
};



/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
    char * const	others_world;
};


/*
 * Structure for an X-social in the socials table.
 */
struct	xsocial_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
    sh_int		gender;
    sh_int		stage;
    sh_int		position;
    sh_int		self;
    sh_int		other;
    sh_int        extra;
    bool		chance;
};


/*
 * Global constants.
 */
extern	const	struct str_app_type	str_app		[36];
extern	const	struct int_app_type	int_app		[36];
extern	const	struct wis_app_type	wis_app		[36];
extern	const	struct dex_app_type	dex_app		[36];
extern	const	struct con_app_type	con_app		[36];
extern	const	struct cmd_type	cmd_table	[];
extern	const	struct liq_type	liq_table	[LIQ_MAX];
extern	const	struct skill_type	skill_table	[MAX_SKILL];
extern		struct artifact_type	artifact_table	[MAX_ART];
extern      const   char *discipline	[MAX_DISCIPLINES];
extern      const   char *wwgift          [MAX_GIFTS];
extern	const	struct social_type	social_table	[];
extern	const	struct xsocial_type	xsocial_table	[];
extern      const struct race_type       race_table      [];
extern      const char   *colour_name                   [8];
extern      const char	 *circlesymbol			[7];
extern      const struct clanrank_type	clanrank	[9][14];
extern      const struct form_type       form_data       [MAX_FORMS];

/*
 * Global variables.
 */
// Structure for Team Battle
extern		TEAM_DATA         *team_list;
extern		TEAM_DATA         *team_free;

extern		HELP_DATA	      *help_first;
extern		HELP_DATA	      *help_last;
extern		SHOP_DATA	      *shop_first;
extern		HELP_DATA	      *first_help;
extern		HELP_DATA	      *last_help;
extern		BAN_DATA	      *ban_list;
extern		CHAR_DATA	      *char_list;
extern		AUCTION_DATA      *auction_list;
extern		AUCTION_DATA      *auction_free;
extern		DESCRIPTOR_DATA   *descriptor_list;
extern		NOTE_DATA	      *note_list;
extern		OBJ_DATA	      *object_list;
extern		BALANCE_DATA	balance_info[7];
extern		CONFIG_DATA	      *config_info;
extern            ROOM_INDEX_DATA   *room_list;
extern		AFFECT_DATA	      *affect_free;
extern		CHAR_DATA	      *char_free;
extern		DESCRIPTOR_DATA	*descriptor_free;
extern		EXTRA_DESCR_DATA  *extra_descr_free;
extern		ROOMTEXT_DATA     *roomtext_free;
extern		OBJ_DATA	      *obj_free;
extern		PC_DATA		*pcdata_free;
extern		char			bug_buf		[];
extern		time_t		current_time;
extern		bool			fLogAll;
extern		FILE *		fpReserve;
extern		KILL_DATA		kill_table	[];
extern		char			log_buf	[];
extern		TIME_INFO_DATA	time_info;
extern		WEATHER_DATA	weather_info;
extern            char              last_command[MAX_STRING_LENGTH];
extern            int               arena;
extern		bool			global_exp;
extern		bool			global_qp;
extern		bool			extra_log;
extern		int			doxlimit;
extern		int			players_logged;
extern            int               players_decap;
extern            int               players_gstolen;
extern            int               auction_timer;

NOTE_DATA	  *	note_free;


/*
 * Common movement commands.
 */
DECLARE_DO_FUN( do_north );
DECLARE_DO_FUN( do_east );
DECLARE_DO_FUN( do_south );
DECLARE_DO_FUN( do_west );
DECLARE_DO_FUN( do_up );
DECLARE_DO_FUN( do_down );

/*
 * Common other commands.
 * Placed here so one and two letter abbreviations work.
 */
DECLARE_DO_FUN( do_cast );
DECLARE_DO_FUN( do_call );
DECLARE_DO_FUN( do_consider );
DECLARE_DO_FUN( do_crack );
DECLARE_DO_FUN( do_diagnose );
DECLARE_DO_FUN( do_dismount );
DECLARE_DO_FUN( do_enter );
DECLARE_DO_FUN( do_exits );
DECLARE_DO_FUN( do_get );
DECLARE_DO_FUN( do_follow );
DECLARE_DO_FUN( do_group );
DECLARE_DO_FUN( do_inventory );
DECLARE_DO_FUN( do_kill );
DECLARE_DO_FUN( do_look );
DECLARE_DO_FUN( do_look );
DECLARE_DO_FUN( do_meditate );
DECLARE_DO_FUN( do_mount );
DECLARE_DO_FUN( do_order );
DECLARE_DO_FUN( do_readaura );
DECLARE_DO_FUN( do_rest );
DECLARE_DO_FUN( do_sit );
DECLARE_DO_FUN( do_stand );
DECLARE_DO_FUN( do_tell );
DECLARE_DO_FUN( do_version );
DECLARE_DO_FUN( do_whisper );
DECLARE_DO_FUN( do_wizhelp );
DECLARE_DO_FUN( do_where );

/*
 * Informational commands.
 */
DECLARE_DO_FUN( do_affects );
DECLARE_DO_FUN( do_areas );
DECLARE_DO_FUN( do_bug );
DECLARE_DO_FUN( do_commands );
DECLARE_DO_FUN( do_credits );
DECLARE_DO_FUN( do_equipment );
DECLARE_DO_FUN( do_examine );
DECLARE_DO_FUN( do_level );
DECLARE_DO_FUN( do_help );
DECLARE_DO_FUN( do_huh );
DECLARE_DO_FUN( do_idea );
DECLARE_DO_FUN( do_level );
DECLARE_DO_FUN( do_mudstat );
DECLARE_DO_FUN( do_reply );
DECLARE_DO_FUN( do_report );
DECLARE_DO_FUN( do_score );
DECLARE_DO_FUN( do_skill );
DECLARE_DO_FUN( do_socials );
DECLARE_DO_FUN( do_spell );
DECLARE_DO_FUN( do_time );
DECLARE_DO_FUN( do_typo );
DECLARE_DO_FUN( do_who );
DECLARE_DO_FUN( do_wizlist );
DECLARE_DO_FUN( do_xemote );
DECLARE_DO_FUN( do_xsocials );
DECLARE_DO_FUN( do_powers );

/*
 * Configuration commands.
 */
DECLARE_DO_FUN( do_alignment );
DECLARE_DO_FUN( do_ansi );
DECLARE_DO_FUN( do_autoexit );
DECLARE_DO_FUN( do_autoloot );
DECLARE_DO_FUN( do_autosac );
DECLARE_DO_FUN( do_blank );
DECLARE_DO_FUN( do_block );
DECLARE_DO_FUN( do_brief );
DECLARE_DO_FUN( do_brief2 );
DECLARE_DO_FUN( do_channels );
DECLARE_DO_FUN( do_compress );
DECLARE_DO_FUN( do_config );
DECLARE_DO_FUN( do_cprompt );
DECLARE_DO_FUN( do_description );
DECLARE_DO_FUN( do_password );
DECLARE_DO_FUN( do_prompt );
DECLARE_DO_FUN( do_title );

/*
 * Communication commands.
 */
DECLARE_DO_FUN( do_auction );
DECLARE_DO_FUN( do_enquire );
DECLARE_DO_FUN( do_make );
DECLARE_DO_FUN( do_bid );


/*
 * Communication commands.
 */
DECLARE_DO_FUN( do_amaztalk );
DECLARE_DO_FUN( do_board );
DECLARE_DO_FUN( do_chat );
DECLARE_DO_FUN( do_clantalk );
DECLARE_DO_FUN( do_communicate );
DECLARE_DO_FUN( do_dragtalk );
DECLARE_DO_FUN( do_emote );
DECLARE_DO_FUN( do_gtell );
DECLARE_DO_FUN( do_hightalk );
DECLARE_DO_FUN( do_howl );
DECLARE_DO_FUN( do_ignore );
DECLARE_DO_FUN( do_knighttalk );
DECLARE_DO_FUN( do_ktalk );
DECLARE_DO_FUN( do_magetalk );
DECLARE_DO_FUN( do_monktalk );
DECLARE_DO_FUN( do_miktalk );
DECLARE_DO_FUN( do_music );
DECLARE_DO_FUN( do_note );
DECLARE_DO_FUN( do_paladintalk );
DECLARE_DO_FUN( do_pray );
DECLARE_DO_FUN( do_quest );
DECLARE_DO_FUN( do_roar );
DECLARE_DO_FUN( do_probe );
DECLARE_DO_FUN( do_say );
DECLARE_DO_FUN( do_sign );
DECLARE_DO_FUN( do_shout );
DECLARE_DO_FUN( do_talk );
DECLARE_DO_FUN( do_unignore );
DECLARE_DO_FUN( do_vamptalk );
DECLARE_DO_FUN( do_xemote );
DECLARE_DO_FUN( do_yell );

/*
 * Object manipulation commands.
 */
DECLARE_DO_FUN( do_buy );
DECLARE_DO_FUN( do_sell );
DECLARE_DO_FUN( do_list );
DECLARE_DO_FUN( do_value );
DECLARE_DO_FUN( do_activate );
DECLARE_DO_FUN( do_dash );
DECLARE_DO_FUN( do_run );
DECLARE_DO_FUN( do_brandish );
DECLARE_DO_FUN( do_close );
DECLARE_DO_FUN( do_draw );
DECLARE_DO_FUN( do_drink );
DECLARE_DO_FUN( do_drop );
DECLARE_DO_FUN( do_eat );
DECLARE_DO_FUN( do_empty );
DECLARE_DO_FUN( do_fill );
DECLARE_DO_FUN( do_get );
DECLARE_DO_FUN( do_give );
DECLARE_DO_FUN( do_gift );
DECLARE_DO_FUN( do_sacrifice );
DECLARE_DO_FUN( do_lock );
DECLARE_DO_FUN( do_newbiepack );
DECLARE_DO_FUN( do_open );
DECLARE_DO_FUN( do_pick );
DECLARE_DO_FUN( do_press );
DECLARE_DO_FUN( do_pull );
DECLARE_DO_FUN( do_put );
DECLARE_DO_FUN( do_quaff );
DECLARE_DO_FUN( do_recite );
DECLARE_DO_FUN( do_reload );
DECLARE_DO_FUN( do_remove );
DECLARE_DO_FUN( do_sacrifice );
DECLARE_DO_FUN( do_sheath );
DECLARE_DO_FUN( do_shoot );
DECLARE_DO_FUN( do_unload );
DECLARE_DO_FUN( do_turn );
DECLARE_DO_FUN( do_twist );
DECLARE_DO_FUN( do_unlock );
DECLARE_DO_FUN( do_wear );
DECLARE_DO_FUN( do_zap );

/* 
 * Other commands 
 */
DECLARE_DO_FUN( do_autoclass );
DECLARE_DO_FUN( do_reclass );
DECLARE_DO_FUN( do_remort );
DECLARE_DO_FUN( do_backstab );
DECLARE_DO_FUN( do_berserk );
DECLARE_DO_FUN( do_classself );
DECLARE_DO_FUN( do_class_count );
DECLARE_DO_FUN( do_crack );
DECLARE_DO_FUN( do_decapitate );
DECLARE_DO_FUN( do_disarm );
DECLARE_DO_FUN( do_escape );
DECLARE_DO_FUN( do_flee );
DECLARE_DO_FUN( do_heal );
DECLARE_DO_FUN( do_double_heal );
DECLARE_DO_FUN( do_double_attack );
DECLARE_DO_FUN( do_ultra_heal );
DECLARE_DO_FUN( do_absorb );
DECLARE_DO_FUN( do_hurl );
DECLARE_DO_FUN( do_kick );
DECLARE_DO_FUN( do_majorenchant );
DECLARE_DO_FUN( do_majorbuy );
DECLARE_DO_FUN( do_punch );
DECLARE_DO_FUN( do_rescue );
DECLARE_DO_FUN( do_scavenger );
DECLARE_DO_FUN( do_scry );
DECLARE_DO_FUN( do_shield );
DECLARE_DO_FUN( do_truesight );

/*
 * Kingdom commands.
 */
DECLARE_DO_FUN( do_capture );
DECLARE_DO_FUN( do_choose );
DECLARE_DO_FUN( do_pelt );
DECLARE_DO_FUN( do_skillbuy );
DECLARE_DO_FUN( do_release );
DECLARE_DO_FUN( do_induct );
DECLARE_DO_FUN( do_rogue );
DECLARE_DO_FUN( do_kingdomlist );
DECLARE_DO_FUN( do_kingdom );
DECLARE_DO_FUN( do_prince );
DECLARE_DO_FUN( do_kingnum );
DECLARE_DO_FUN( do_kingset );
DECLARE_DO_FUN( do_krecall );
DECLARE_DO_FUN( do_lands );
DECLARE_DO_FUN( do_obtain );
DECLARE_DO_FUN( do_memberlist );
DECLARE_DO_FUN( do_outcast );
DECLARE_DO_FUN( do_seeking );

/*
 * Team commands.
 */
DECLARE_DO_FUN( do_form );
DECLARE_DO_FUN( do_come_on );
DECLARE_DO_FUN( do_cheer );
DECLARE_DO_FUN( do_taunt );
DECLARE_DO_FUN( do_dodge );
DECLARE_DO_FUN( do_counter_push );
DECLARE_DO_FUN( do_emergency_roll );
DECLARE_DO_FUN( do_emergency_flip );
DECLARE_DO_FUN( do_super_cancel );
DECLARE_DO_FUN( do_retort );
DECLARE_DO_FUN( do_tabulate );
DECLARE_DO_FUN( do_adjust );
DECLARE_DO_FUN( do_armor_mode );
DECLARE_DO_FUN( do_hyper_mode );
DECLARE_DO_FUN( do_counter_mode );
DECLARE_DO_FUN( do_chain_mode );
DECLARE_DO_FUN( do_infinity_mode );
DECLARE_DO_FUN( do_strike );


/*
 * Miscellaneous commands.
 */
DECLARE_DO_FUN( do_accept );
DECLARE_DO_FUN( do_arenaagree );
DECLARE_DO_FUN( do_give_up );
DECLARE_DO_FUN( do_arena_watch );
DECLARE_DO_FUN( do_artifact );
DECLARE_DO_FUN( do_autosave );
DECLARE_DO_FUN( do_birth );
DECLARE_DO_FUN( do_blindfold );
DECLARE_DO_FUN( do_bestow );
DECLARE_DO_FUN( do_bet );
DECLARE_DO_FUN( do_bounty );
DECLARE_DO_FUN( do_bountylist );
DECLARE_DO_FUN( do_breakup );
DECLARE_DO_FUN( do_bribe );
DECLARE_DO_FUN( do_challenge );
DECLARE_DO_FUN( do_claim );
DECLARE_DO_FUN( do_clearstats );
DECLARE_DO_FUN( do_consent );
DECLARE_DO_FUN( do_contraception );
DECLARE_DO_FUN( do_clan_recall );
DECLARE_DO_FUN( do_decline );
DECLARE_DO_FUN( do_delet );
DECLARE_DO_FUN( do_delete );
DECLARE_DO_FUN( do_depositqps );
DECLARE_DO_FUN( do_kdeposit );
DECLARE_DO_FUN( do_deto );
DECLARE_DO_FUN( do_detox );
DECLARE_DO_FUN( do_finger );
DECLARE_DO_FUN( do_flex );
DECLARE_DO_FUN( do_gag );
DECLARE_DO_FUN( do_gsocial );
DECLARE_DO_FUN( do_hide );
DECLARE_DO_FUN( do_home );
DECLARE_DO_FUN( do_immune );
DECLARE_DO_FUN( do_leader );
DECLARE_DO_FUN( do_locate );
DECLARE_DO_FUN( do_mystic_warp );
DECLARE_DO_FUN( do_nightsight );
DECLARE_DO_FUN( do_norescue );
DECLARE_DO_FUN( do_nosummon );
DECLARE_DO_FUN( do_notravel );
DECLARE_DO_FUN( do_no_watch );
DECLARE_DO_FUN( do_pknow );
DECLARE_DO_FUN( do_practice );
DECLARE_DO_FUN( do_propose );
DECLARE_DO_FUN( do_qui );
DECLARE_DO_FUN( do_quit );
DECLARE_DO_FUN( do_recall );
DECLARE_DO_FUN( do_rent );
DECLARE_DO_FUN( do_safe );
DECLARE_DO_FUN( do_save );
DECLARE_DO_FUN( do_scan );
DECLARE_DO_FUN( do_setfatality );
DECLARE_DO_FUN( do_setfatality2 );
DECLARE_DO_FUN( do_setlogin );
DECLARE_DO_FUN( set_title );
DECLARE_DO_FUN( do_setlogout );
DECLARE_DO_FUN( do_shadowsight );
DECLARE_DO_FUN( do_sleep );
DECLARE_DO_FUN( do_smother );
DECLARE_DO_FUN( do_sneak );
DECLARE_DO_FUN( do_sound );
DECLARE_DO_FUN( do_split );
DECLARE_DO_FUN( do_spy );
DECLARE_DO_FUN( do_steal );
DECLARE_DO_FUN( do_superheal );
DECLARE_DO_FUN( do_teach );
DECLARE_DO_FUN( do_team );
DECLARE_DO_FUN( do_tie );
DECLARE_DO_FUN( do_track );
DECLARE_DO_FUN( do_train );
DECLARE_DO_FUN( do_transport );
DECLARE_DO_FUN( do_travel );
DECLARE_DO_FUN( do_untie );
DECLARE_DO_FUN( do_unwerewolf );
DECLARE_DO_FUN( do_upkeep );
DECLARE_DO_FUN( do_visible );
DECLARE_DO_FUN( do_wake );
DECLARE_DO_FUN( do_web );
DECLARE_DO_FUN( do_werewolf );
DECLARE_DO_FUN( do_withdrawqps );

/*
 * Priest commands.
 */
DECLARE_DO_FUN( do_adamantium );
DECLARE_DO_FUN( do_learn );
DECLARE_DO_FUN( do_monkcloak );
DECLARE_DO_FUN( do_flaminghands );
DECLARE_DO_FUN( do_monkarmor );
DECLARE_DO_FUN( do_godsbless );
DECLARE_DO_FUN( do_steelskin );

/*
 * KOF Kuruda Commands.
 */
DECLARE_DO_FUN( do_highkick );
DECLARE_DO_FUN( do_jumpkick );
DECLARE_DO_FUN( do_snapkick );
DECLARE_DO_FUN( do_kuruda_spinkick );
DECLARE_DO_FUN( do_kuruda_sidewinder );

/*
 * Guyver commands.
 */
DECLARE_DO_FUN( do_dark_smasher );
DECLARE_DO_FUN( do_fuse );
DECLARE_DO_FUN( do_gcreate );
DECLARE_DO_FUN( do_gweapon );
DECLARE_DO_FUN( do_guyver );
DECLARE_DO_FUN( do_guyver_heal );
DECLARE_DO_FUN( do_head_beam );
DECLARE_DO_FUN( do_mega_smasher );
DECLARE_DO_FUN( do_power_punch );
DECLARE_DO_FUN( do_pressure_cannon );
DECLARE_DO_FUN( do_sonic_emitter );
DECLARE_DO_FUN( do_sense );
DECLARE_DO_FUN( do_single_smasher );
DECLARE_DO_FUN( do_stealth );
DECLARE_DO_FUN( do_turbo_mode );
DECLARE_DO_FUN( do_vswords );

/*
 * Garou - N. Werewolf commands.
 */
DECLARE_DO_FUN( do_bare_knuckle );
DECLARE_DO_FUN( do_burn_knuckle );
DECLARE_DO_FUN( do_burning_rave );
DECLARE_DO_FUN( do_buster_wolf );
DECLARE_DO_FUN( do_chain );
DECLARE_DO_FUN( do_moon_dance );
DECLARE_DO_FUN( do_moon_eclipse );
DECLARE_DO_FUN( do_morph );
DECLARE_DO_FUN( do_neck_throttle );
DECLARE_DO_FUN( do_omega_rave );
DECLARE_DO_FUN( do_overheat_geyser );
DECLARE_DO_FUN( do_over_drive );
DECLARE_DO_FUN( do_subsitance );
DECLARE_DO_FUN( do_rising_tackle );
DECLARE_DO_FUN( do_wolf_pounce );
DECLARE_DO_FUN( do_power_dunk );
DECLARE_DO_FUN( do_power_charge );
DECLARE_DO_FUN( do_power_wave );
DECLARE_DO_FUN( do_triple_geyser );

/*
 * Garou - H. Werewolf commands.
 */
DECLARE_DO_FUN( do_wolf_graft );
DECLARE_DO_FUN( do_faded_moon );
DECLARE_DO_FUN( do_raging_slash );
DECLARE_DO_FUN( do_tornado_slice );
DECLARE_DO_FUN( do_twisting_moon );
DECLARE_DO_FUN( do_wolf_banshee );

/*
 * Assassin commands.
 */
DECLARE_DO_FUN( do_hand_circua );
DECLARE_DO_FUN( do_control );
DECLARE_DO_FUN( do_dark_bomb );
DECLARE_DO_FUN( do_dark_matter );
DECLARE_DO_FUN( do_luminaire );
DECLARE_DO_FUN( do_piercing_dasher );
DECLARE_DO_FUN( do_somer_strike );
DECLARE_DO_FUN( do_sword_rain );
DECLARE_DO_FUN( do_mi_drain );
DECLARE_DO_FUN( do_tri_heal );
DECLARE_DO_FUN( do_flash_grenade );
DECLARE_DO_FUN( do_dispose );
DECLARE_DO_FUN( do_dual_image );
DECLARE_DO_FUN( do_disguise );
DECLARE_DO_FUN( do_fade );
DECLARE_DO_FUN( do_painless );
DECLARE_DO_FUN( do_possess );
DECLARE_DO_FUN( do_assassin_guided );
DECLARE_DO_FUN( do_inner_healing );
DECLARE_DO_FUN( do_inner_rage );
DECLARE_DO_FUN( do_yang_power );
DECLARE_DO_FUN( do_yin_power );
DECLARE_DO_FUN( do_inner_shield );
DECLARE_DO_FUN( do_inner_fury );
DECLARE_DO_FUN( do_dispel );
DECLARE_DO_FUN( do_hand_blast );
DECLARE_DO_FUN( do_chin_smite );
DECLARE_DO_FUN( do_leg_dispatcher );
DECLARE_DO_FUN( do_arm_grab );
DECLARE_DO_FUN( do_body_rip );
DECLARE_DO_FUN( do_ki_pinch );
DECLARE_DO_FUN( do_ki_field );
DECLARE_DO_FUN( do_ki_wrath );
DECLARE_DO_FUN( do_study );
DECLARE_DO_FUN( do_assassin_focus );
DECLARE_DO_FUN( do_assassin_throw );
DECLARE_DO_FUN( do_assassin_vanish );
DECLARE_DO_FUN( do_assassin_blast );
DECLARE_DO_FUN( do_assassin_shin );
DECLARE_DO_FUN( do_assassin_roundhouse );
DECLARE_DO_FUN( do_assassin_headbutt );
DECLARE_DO_FUN( do_assassin_kick );
DECLARE_DO_FUN( do_assassin_jab );
DECLARE_DO_FUN( do_assassin_elbow );
DECLARE_DO_FUN( do_switch );


/*
 * Champion commands.
 */
DECLARE_DO_FUN( do_deathtouch );
DECLARE_DO_FUN( do_demmak );
DECLARE_DO_FUN( do_divineray );
DECLARE_DO_FUN( do_fadedstrike );
DECLARE_DO_FUN( do_healingtouch );
DECLARE_DO_FUN( do_inner_shield );
DECLARE_DO_FUN( do_kakeio );
DECLARE_DO_FUN( do_chitilak );
DECLARE_DO_FUN( do_reflection );
DECLARE_DO_FUN( do_soulbomb );
DECLARE_DO_FUN( do_subtletouch );

/*
 * Weapons Master commands.
 */
DECLARE_DO_FUN( do_entice );
DECLARE_DO_FUN( do_riposte );
DECLARE_DO_FUN( do_forestall );
DECLARE_DO_FUN( do_power_shot );
DECLARE_DO_FUN( do_master_wave );
DECLARE_DO_FUN( do_air_blade );
DECLARE_DO_FUN( do_breaker );
DECLARE_DO_FUN( do_fangbreaker_blade );
DECLARE_DO_FUN( do_whirlwind_crusher );
DECLARE_DO_FUN( do_justice_starfist );
DECLARE_DO_FUN( do_divine_eradication );
DECLARE_DO_FUN( do_raging_soul );
DECLARE_DO_FUN( do_hyper_drive );
DECLARE_DO_FUN( do_speed_up );
DECLARE_DO_FUN( do_shining_soul );
DECLARE_DO_FUN( do_sigma_rage );
DECLARE_DO_FUN( do_vampire_fang );


/*
 * Shaolin Monk commands.
 */
DECLARE_DO_FUN( do_blade_grasp );
DECLARE_DO_FUN( do_butterfly );
DECLARE_DO_FUN( do_elbow_strike );
DECLARE_DO_FUN( do_focus );
DECLARE_DO_FUN( do_guidance );
DECLARE_DO_FUN( do_hand_hamedo );
DECLARE_DO_FUN( do_headbutt );
DECLARE_DO_FUN( do_jab );
DECLARE_DO_FUN( do_ki_grasp );
DECLARE_DO_FUN( do_kou_ken );
DECLARE_DO_FUN( do_knee );
DECLARE_DO_FUN( do_palm_strike );
DECLARE_DO_FUN( do_roundhouse_kick );
DECLARE_DO_FUN( do_rising_shin );
DECLARE_DO_FUN( do_sidewinder_kick );
DECLARE_DO_FUN( do_skid );
DECLARE_DO_FUN( do_soul_fusion );
DECLARE_DO_FUN( do_stance );
DECLARE_DO_FUN( do_sweep );
DECLARE_DO_FUN( do_blazing_hand );
DECLARE_DO_FUN( do_twirl );
DECLARE_DO_FUN( do_vulcanpinch );


/*
 * Angel commands.
 */
DECLARE_DO_FUN( do_angel_create );
DECLARE_DO_FUN( do_blade_dance );
DECLARE_DO_FUN( do_enlighten );
DECLARE_DO_FUN( do_ether_form );
DECLARE_DO_FUN( do_fist_heaven );
DECLARE_DO_FUN( do_fist_takedown );
DECLARE_DO_FUN( do_angel_fold );
DECLARE_DO_FUN( do_gpeace );
DECLARE_DO_FUN( do_gshield );
DECLARE_DO_FUN( do_gspirit );
DECLARE_DO_FUN( do_harmony );
DECLARE_DO_FUN( do_holy_ball );
DECLARE_DO_FUN( do_holyword );
DECLARE_DO_FUN( do_hope );
DECLARE_DO_FUN( do_houseofgod );
DECLARE_DO_FUN( do_wrathofgod );
DECLARE_DO_FUN( do_mana_shield );
DECLARE_DO_FUN( do_mega_strike );
DECLARE_DO_FUN( do_redemption );
DECLARE_DO_FUN( do_angel_smite );
DECLARE_DO_FUN( do_purification );
DECLARE_DO_FUN( do_heavenly_cloak );
DECLARE_DO_FUN( do_divine_judgement );
DECLARE_DO_FUN( do_seraph_curse );
DECLARE_DO_FUN( do_angelic_genesis );
DECLARE_DO_FUN( do_martyr );
DECLARE_DO_FUN( do_angel_spread );
DECLARE_DO_FUN( do_trueform );
DECLARE_DO_FUN( do_touchofgod );

/*
 * Knight commands.
 */
DECLARE_DO_FUN( do_avatar );
DECLARE_DO_FUN( do_charge );
DECLARE_DO_FUN( do_crush_punch );
DECLARE_DO_FUN( do_eruption_sword );
DECLARE_DO_FUN( do_subdue );
DECLARE_DO_FUN( do_flash_sword );
DECLARE_DO_FUN( do_genesis );
DECLARE_DO_FUN( do_godlysight );
DECLARE_DO_FUN( do_godsheal );
DECLARE_DO_FUN( do_godsread );
DECLARE_DO_FUN( do_godssearch );
DECLARE_DO_FUN( do_holyarmor );
DECLARE_DO_FUN( do_holy_shield );
DECLARE_DO_FUN( do_holy_explosion );
DECLARE_DO_FUN( do_demon_slice );
DECLARE_DO_FUN( do_no_mercy );
DECLARE_DO_FUN( do_justice_cutter );
DECLARE_DO_FUN( do_durandal );
DECLARE_DO_FUN( do_vengeance );
DECLARE_DO_FUN( do_lightning_stab );
DECLARE_DO_FUN( do_my_final_heaven );
DECLARE_DO_FUN( do_nemesis );
DECLARE_DO_FUN( do_pgrant );
DECLARE_DO_FUN( do_soul_hammer );
DECLARE_DO_FUN( do_split_punch );
DECLARE_DO_FUN( do_stasis_sword );

/*
 * Dragon Knight commands.
 */
DECLARE_DO_FUN( do_demon_dance );
DECLARE_DO_FUN( do_flower_storm );
DECLARE_DO_FUN( do_blazing_dynamo );
DECLARE_DO_FUN( do_astral_drain );
DECLARE_DO_FUN( do_dragoon_avalanche );
DECLARE_DO_FUN( do_blazing_dynamo );
DECLARE_DO_FUN( do_bluesea_dragon );
DECLARE_DO_FUN( do_burning_rush );
DECLARE_DO_FUN( do_crush_dance );
DECLARE_DO_FUN( do_dark_dragon );
DECLARE_DO_FUN( do_dark_shrine );
DECLARE_DO_FUN( do_death_dimension );
DECLARE_DO_FUN( do_demon_dance );
DECLARE_DO_FUN( do_demon_gate );
DECLARE_DO_FUN( do_dragoon_dust );
DECLARE_DO_FUN( do_double_slash );
DECLARE_DO_FUN( do_explosion );
DECLARE_DO_FUN( do_flame_shot );
DECLARE_DO_FUN( do_flaming_twirl );
DECLARE_DO_FUN( do_flower_storm );
DECLARE_DO_FUN( do_final_burst );
DECLARE_DO_FUN( do_freezing_rain );
DECLARE_DO_FUN( do_gaspless );
DECLARE_DO_FUN( do_gust_wind );
DECLARE_DO_FUN( do_hand_cannon );
DECLARE_DO_FUN( do_hard_blade );
DECLARE_DO_FUN( do_dragoon_harpoon );
DECLARE_DO_FUN( do_heavens_gate );
DECLARE_DO_FUN( do_jade_dragon );
DECLARE_DO_FUN( do_last_prayer );
DECLARE_DO_FUN( do_madness_hero );
DECLARE_DO_FUN( do_dragoon_merton );
DECLARE_DO_FUN( do_more_and_more );
DECLARE_DO_FUN( do_moonlight );
DECLARE_DO_FUN( do_moon_strike );
DECLARE_DO_FUN( do_rainbow_breath );
DECLARE_DO_FUN( do_redeyed_dragon );
DECLARE_DO_FUN( do_regeneration );
DECLARE_DO_FUN( do_rod_typhoon );
DECLARE_DO_FUN( do_rose_storm );
DECLARE_DO_FUN( do_shift );
DECLARE_DO_FUN( do_divine_ball );
DECLARE_DO_FUN( do_divine_cannon );
DECLARE_DO_FUN( do_silver_dragon );
DECLARE_DO_FUN( do_sonic_bomb );
DECLARE_DO_FUN( do_spinning_cane );
DECLARE_DO_FUN( do_star_children );
DECLARE_DO_FUN( do_swirling_dance );
DECLARE_DO_FUN( do_dragoon_transform );
DECLARE_DO_FUN( do_volcano );
DECLARE_DO_FUN( do_weapon_weld );
DECLARE_DO_FUN( do_whip_smack );
DECLARE_DO_FUN( do_whirlwind );
DECLARE_DO_FUN( do_whirlpool );
DECLARE_DO_FUN( do_wing_blaster );

/*
 * hobbit commands.
 */
DECLARE_DO_FUN( do_cats_eye );
DECLARE_DO_FUN( do_crystal_dreams );
DECLARE_DO_FUN( do_daintly_dark );
DECLARE_DO_FUN( do_dark_hell );
DECLARE_DO_FUN( do_demon_eye );
DECLARE_DO_FUN( do_famed_archangel );
DECLARE_DO_FUN( do_flare );
DECLARE_DO_FUN( do_frogget );
DECLARE_DO_FUN( do_grand_love );
DECLARE_DO_FUN( do_hobby_armor );
DECLARE_DO_FUN( do_holy );
DECLARE_DO_FUN( do_hidden_destiny );
DECLARE_DO_FUN( do_hobbit_cocoon );
DECLARE_DO_FUN( do_loath_demon );
DECLARE_DO_FUN( do_shadow_dancer );
DECLARE_DO_FUN( do_shadow_demon );
DECLARE_DO_FUN( do_tilly_mint );
DECLARE_DO_FUN( do_truthful );
DECLARE_DO_FUN( do_wknights );


/*
 * Kuruda commands.
 */
DECLARE_DO_FUN( do_ironhammer );
DECLARE_DO_FUN( do_aragami );
DECLARE_DO_FUN( do_ayanae );
DECLARE_DO_FUN( do_kyaku );
DECLARE_DO_FUN( do_blazing_hand );
DECLARE_DO_FUN( do_ki_charge );
DECLARE_DO_FUN( do_ki_grasp );
DECLARE_DO_FUN( do_kuruda_armor );
DECLARE_DO_FUN( do_nanase );
DECLARE_DO_FUN( do_oniyaki );
DECLARE_DO_FUN( do_orochi_nagi );
DECLARE_DO_FUN( do_shiki );
DECLARE_DO_FUN( do_spirit_bomb );
DECLARE_DO_FUN( do_yami_sugi );
DECLARE_DO_FUN( do_swipe );
DECLARE_DO_FUN( do_ki_punch );
DECLARE_DO_FUN( do_lockon );
DECLARE_DO_FUN( do_kyaku );
DECLARE_DO_FUN( do_stance );
DECLARE_DO_FUN( do_twirl );
DECLARE_DO_FUN( do_butterfly );
DECLARE_DO_FUN( do_swan_dive );
DECLARE_DO_FUN( do_psycho_ball );
DECLARE_DO_FUN( do_psycho_rush );
DECLARE_DO_FUN( do_palm_strike );
DECLARE_DO_FUN( do_soulbomb );
DECLARE_DO_FUN( do_soulless_touch );
DECLARE_DO_FUN( do_final_heaven );
DECLARE_DO_FUN( do_neck_breaker );
DECLARE_DO_FUN( do_dragon_roar );
DECLARE_DO_FUN( do_blazing_fury );
DECLARE_DO_FUN( do_kou_ken );
DECLARE_DO_FUN( do_tiger_rush );
DECLARE_DO_FUN( do_meteor_barret );
DECLARE_DO_FUN( do_pummel );
DECLARE_DO_FUN( do_divine_might );
DECLARE_DO_FUN( do_hishoken_rush );
DECLARE_DO_FUN( do_innocence );
DECLARE_DO_FUN( do_empty_blow );
DECLARE_DO_FUN( do_body_control );
DECLARE_DO_FUN( do_secret_fist );
DECLARE_DO_FUN( do_stagger );
DECLARE_DO_FUN( do_rising_storm );
DECLARE_DO_FUN( do_heavenly_punch );


/*
 * Snow Elf Commands.
 */
DECLARE_DO_FUN( do_snowyarmor );
DECLARE_DO_FUN( do_ice_arrow );
DECLARE_DO_FUN( do_slither_ice );
DECLARE_DO_FUN( do_freezing_blast );
DECLARE_DO_FUN( do_ice_slice );
DECLARE_DO_FUN( do_ice_slash );
DECLARE_DO_FUN( do_winter_swans );
DECLARE_DO_FUN( do_carpe_jugulum );
DECLARE_DO_FUN( do_ice_punch );
DECLARE_DO_FUN( do_ice_shield );
DECLARE_DO_FUN( do_ambush );
DECLARE_DO_FUN( do_ice_trap );

/*
 * Wisp Commands.
 */
DECLARE_DO_FUN( do_electric_shield );
DECLARE_DO_FUN( do_colour_spray );
DECLARE_DO_FUN( do_inner_light );
DECLARE_DO_FUN( do_heat_shield );
DECLARE_DO_FUN( do_call_lightning );
DECLARE_DO_FUN( do_thunderstorm );
DECLARE_DO_FUN( do_plasma_blade );
DECLARE_DO_FUN( do_icarus );
DECLARE_DO_FUN( do_mindtap );
DECLARE_DO_FUN( do_colourbeam );
DECLARE_DO_FUN( do_rain_blade );
DECLARE_DO_FUN( do_electrotrigger );

/*
 * Duergar Commands.
 */
DECLARE_DO_FUN( do_duergar_armor );
DECLARE_DO_FUN( do_swing );
DECLARE_DO_FUN( do_slasher_axe );
DECLARE_DO_FUN( do_stained_axe );
DECLARE_DO_FUN( do_axe_slam );
DECLARE_DO_FUN( do_forge );
DECLARE_DO_FUN( do_rename );
DECLARE_DO_FUN( do_rune_shift );
DECLARE_DO_FUN( do_duergar_dance );
DECLARE_DO_FUN( do_axed_drain );
DECLARE_DO_FUN( do_axe_knockout );
DECLARE_DO_FUN( do_dark_cloud );
DECLARE_DO_FUN( do_earthquake );
DECLARE_DO_FUN( do_tunnel );
DECLARE_DO_FUN( do_vineofevil );

/*
 * Phoenix Commands.
 */
DECLARE_DO_FUN( do_firebreath );
DECLARE_DO_FUN( do_flame_talons );
DECLARE_DO_FUN( do_volcano );
DECLARE_DO_FUN( do_phoenix_cry );
DECLARE_DO_FUN( do_fire_cannon );
DECLARE_DO_FUN( do_phoenix_rush );
DECLARE_DO_FUN( do_phoenix_wrath );


/*
 * Kodo commands.
 */
DECLARE_DO_FUN( do_kodocreate );
DECLARE_DO_FUN( do_arrowlearn );
DECLARE_DO_FUN( do_hells_barrage );
DECLARE_DO_FUN( do_multi_arrow );
DECLARE_DO_FUN( do_arrow_drain );
DECLARE_DO_FUN( do_demonic_shot );
DECLARE_DO_FUN( do_fire_arrow );
DECLARE_DO_FUN( do_detonative_arrow );
DECLARE_DO_FUN( do_explosive_arrow );
DECLARE_DO_FUN( do_poison_arrow );
DECLARE_DO_FUN( do_poison_confusion );
DECLARE_DO_FUN( do_poison_paralyse );

/*
 * Divine Knight commands.
 */
DECLARE_DO_FUN( do_blaster_punch );
DECLARE_DO_FUN( do_cover );
DECLARE_DO_FUN( do_guard );
DECLARE_DO_FUN( do_hellcry_punch );
DECLARE_DO_FUN( do_icewolf_bite );
DECLARE_DO_FUN( do_shellblast_stab );
DECLARE_DO_FUN( do_store );

/*
 * Drow commands.
 */
DECLARE_DO_FUN( do_bloodlet );
DECLARE_DO_FUN( do_chaosblast );
DECLARE_DO_FUN( do_confuse );
DECLARE_DO_FUN( do_darkdodge );
DECLARE_DO_FUN( do_darkness );
DECLARE_DO_FUN( do_darktendrils );
DECLARE_DO_FUN( do_dark_garotte );
DECLARE_DO_FUN( do_dark_thrust );
DECLARE_DO_FUN( do_dischard );
DECLARE_DO_FUN( do_drewst );
DECLARE_DO_FUN( do_drowcreate );
DECLARE_DO_FUN( do_drowfire );
DECLARE_DO_FUN( do_drowhate );
DECLARE_DO_FUN( do_drowpowers );
DECLARE_DO_FUN( do_drowsight );
DECLARE_DO_FUN( do_drowshield );
DECLARE_DO_FUN( do_earth_blast );
DECLARE_DO_FUN( do_earthshatter );
DECLARE_DO_FUN( do_fightdance );
DECLARE_DO_FUN( do_garotte );
DECLARE_DO_FUN( do_glamour );
DECLARE_DO_FUN( do_grant );
DECLARE_DO_FUN( do_hex );
DECLARE_DO_FUN( do_llothbless );
DECLARE_DO_FUN( do_lloth_throw );
DECLARE_DO_FUN( do_olath );
DECLARE_DO_FUN( do_pandora_box );
DECLARE_DO_FUN( do_shadow_flare );
DECLARE_DO_FUN( do_shadowstrength );
DECLARE_DO_FUN( do_shoot );
DECLARE_DO_FUN( do_spiderform );
DECLARE_DO_FUN( do_unsatisfy );
DECLARE_DO_FUN( do_velkyn );

/*
 * Drow general commands.
 */
DECLARE_DO_FUN( do_transfuse );

/*
 * Dark Elf Militia commands.
 */
DECLARE_DO_FUN( do_black_widow );
DECLARE_DO_FUN( do_catacomb );
DECLARE_DO_FUN( do_death_smell );
DECLARE_DO_FUN( do_demi_drow );
DECLARE_DO_FUN( do_elghinn_wusuul );
DECLARE_DO_FUN( do_fallen_angel );
DECLARE_DO_FUN( do_lolth_dance );
DECLARE_DO_FUN( do_lolth_shoot );
DECLARE_DO_FUN( do_lolth_thalack );
DECLARE_DO_FUN( do_olplynir_dro );
DECLARE_DO_FUN( do_orbb_lance );
DECLARE_DO_FUN( do_series_slash );
DECLARE_DO_FUN( do_shadow_shoot );
DECLARE_DO_FUN( do_viryns_mace );
DECLARE_DO_FUN( do_under_world );
DECLARE_DO_FUN( do_waver );

/*
 * Dark Elf Magi commands.
 */
DECLARE_DO_FUN( do_acid_arrow );
DECLARE_DO_FUN( do_blur );
DECLARE_DO_FUN( do_cloudkill );
DECLARE_DO_FUN( do_confusion );
DECLARE_DO_FUN( do_dancing_flames );
DECLARE_DO_FUN( do_dark_strike );
DECLARE_DO_FUN( do_holy_shame );
DECLARE_DO_FUN( do_horror );
DECLARE_DO_FUN( do_ice_dust );
DECLARE_DO_FUN( do_ice_storm );
DECLARE_DO_FUN( do_invunerability );
DECLARE_DO_FUN( do_lolth_charm );
DECLARE_DO_FUN( do_lolth_shield );
DECLARE_DO_FUN( do_roaring_skies );
DECLARE_DO_FUN( do_shadow_door );
DECLARE_DO_FUN( do_shocking_grasp );
DECLARE_DO_FUN( do_spider_summon );
DECLARE_DO_FUN( do_swift_wave );
DECLARE_DO_FUN( do_titans_rage );
DECLARE_DO_FUN( do_vampiric_touch );

/*
 * Dark Elf Clerical commands.
 */
DECLARE_DO_FUN( do_aid );
DECLARE_DO_FUN( do_bless );
DECLARE_DO_FUN( do_cura );
DECLARE_DO_FUN( do_cure );
DECLARE_DO_FUN( do_drow_blast );
DECLARE_DO_FUN( do_flame_blade );
DECLARE_DO_FUN( do_flame_strike );
DECLARE_DO_FUN( do_fireshield );
DECLARE_DO_FUN( do_hold_person );
DECLARE_DO_FUN( do_lolth_strength );
DECLARE_DO_FUN( do_lolth_wrath );
DECLARE_DO_FUN( do_lightning_blast );
DECLARE_DO_FUN( do_neutralize );
DECLARE_DO_FUN( do_poison );
DECLARE_DO_FUN( do_protection );
DECLARE_DO_FUN( do_lolth_prayer );
DECLARE_DO_FUN( do_raise_dead );
DECLARE_DO_FUN( do_rigid_thinking );
DECLARE_DO_FUN( do_shame );
DECLARE_DO_FUN( do_shammer );
DECLARE_DO_FUN( do_shillelagh );
DECLARE_DO_FUN( do_unparalyse );

/*
 * Dragon commands.
 */
DECLARE_DO_FUN( check_dragon_form );
DECLARE_DO_FUN( do_dragonbreath );
DECLARE_DO_FUN( do_dragongift );
DECLARE_DO_FUN( do_dragonshift );
DECLARE_DO_FUN( do_dragonswoop );
DECLARE_DO_FUN( do_final_flare );
DECLARE_DO_FUN( do_hyper_claw );
DECLARE_DO_FUN( do_highclaw );
DECLARE_DO_FUN( do_dragon_flare );
DECLARE_DO_FUN( do_shoulderrush );
DECLARE_DO_FUN( do_stomp );
DECLARE_DO_FUN( do_tailsweep );

/*
 * Highlander commands.
 */
DECLARE_DO_FUN( do_bladespin );
DECLARE_DO_FUN( do_jcatapult );
DECLARE_DO_FUN( do_jchoke );
DECLARE_DO_FUN( do_jdestruction );
DECLARE_DO_FUN( do_jheal );
DECLARE_DO_FUN( do_jinvis );
DECLARE_DO_FUN( do_jrush );
DECLARE_DO_FUN( do_jlightning );
DECLARE_DO_FUN( do_jpull );
DECLARE_DO_FUN( do_jpush );
DECLARE_DO_FUN( do_jsummon );
DECLARE_DO_FUN( do_jtaunt );
DECLARE_DO_FUN( do_jtouch );
DECLARE_DO_FUN( do_levitate );
DECLARE_DO_FUN( do_meteor_strike );

/*
 * Soulreavers general commands.
 */
DECLARE_DO_FUN( do_belot );
DECLARE_DO_FUN( do_blood_storm );
DECLARE_DO_FUN( do_caine_summon );
DECLARE_DO_FUN( do_soul_change );
DECLARE_DO_FUN( do_night_requiem );
DECLARE_DO_FUN( do_paralysis );
DECLARE_DO_FUN( do_soul_beam );
DECLARE_DO_FUN( do_soul_eraser );
DECLARE_DO_FUN( do_soul_reaper );
DECLARE_DO_FUN( do_soul_reaver );


/*
 * Soulreaver forms commands.
 */
DECLARE_DO_FUN( do_bat_fire );
DECLARE_DO_FUN( do_lunge_bite );
DECLARE_DO_FUN( do_sonic_pierce );
DECLARE_DO_FUN( do_reaver_blast );
DECLARE_DO_FUN( do_wing_smash );
DECLARE_DO_FUN( do_wolf_charge );

/*
 * Soulreaver D. Specialists commands.
 */
DECLARE_DO_FUN( do_soul_assassinate );
DECLARE_DO_FUN( do_blood_cleanse );
DECLARE_DO_FUN( do_blood_ritual );
DECLARE_DO_FUN( do_dagger_pounce );
DECLARE_DO_FUN( do_double_twist );
DECLARE_DO_FUN( do_mirage );
DECLARE_DO_FUN( do_neck_slash );
DECLARE_DO_FUN( do_sense_blood );
DECLARE_DO_FUN( do_spear_kick );
DECLARE_DO_FUN( do_soul_steal );

/*
 * Soulreaver A. Specialists commands.
 */
DECLARE_DO_FUN( do_blood_wave );
DECLARE_DO_FUN( do_dark_fire );
DECLARE_DO_FUN( do_earth_grasp );
DECLARE_DO_FUN( do_soul_fear );
DECLARE_DO_FUN( do_soul_fire );
DECLARE_DO_FUN( do_poison_mist );
DECLARE_DO_FUN( do_summon_spirit );
DECLARE_DO_FUN( do_tetra_spirit );

/*
 * Soulreaver T. Specialists commands.
 */
DECLARE_DO_FUN( do_disrupture );
DECLARE_DO_FUN( do_incite );
DECLARE_DO_FUN( do_invis_wall );
DECLARE_DO_FUN( do_malice_form );
DECLARE_DO_FUN( do_nightmare );
DECLARE_DO_FUN( do_psy_reaper );
DECLARE_DO_FUN( do_psy_reaver );
DECLARE_DO_FUN( do_psy_slice );
DECLARE_DO_FUN( do_stigmata );
DECLARE_DO_FUN( do_tele_barrier );
DECLARE_DO_FUN( do_vampire_image );


/*
 * Vampire commands.
 */
DECLARE_DO_FUN( do_assassinate );
DECLARE_DO_FUN( do_baal );
DECLARE_DO_FUN( do_bloodwall );
DECLARE_DO_FUN( do_change );
DECLARE_DO_FUN( do_coffin );
DECLARE_DO_FUN( do_drain );
DECLARE_DO_FUN( do_earthmeld );
DECLARE_DO_FUN( do_embrace );
DECLARE_DO_FUN( do_evolve );
DECLARE_DO_FUN( do_fear );
DECLARE_DO_FUN( do_feed );
DECLARE_DO_FUN( do_flash );
DECLARE_DO_FUN( do_frenzy );
DECLARE_DO_FUN( do_glide );
DECLARE_DO_FUN( do_grab );
DECLARE_DO_FUN( do_infirmity );
DECLARE_DO_FUN( do_inherit );
DECLARE_DO_FUN( do_lamprey );
DECLARE_DO_FUN( do_maelstrom );
DECLARE_DO_FUN( do_majesty );
DECLARE_DO_FUN( do_mindblast );
DECLARE_DO_FUN( do_mirror );
DECLARE_DO_FUN( do_physic_crush );
DECLARE_DO_FUN( do_physic_drain );
DECLARE_DO_FUN( do_plasma );
DECLARE_DO_FUN( do_renew );
DECLARE_DO_FUN( do_rot );
DECLARE_DO_FUN( do_shadowgaze );
DECLARE_DO_FUN( do_shroud );
DECLARE_DO_FUN( do_soul_strike );
DECLARE_DO_FUN( do_spiritguard );
DECLARE_DO_FUN( do_spit );
DECLARE_DO_FUN( do_tears_reality );
DECLARE_DO_FUN( do_tongue );
DECLARE_DO_FUN( do_tongue_lash );
DECLARE_DO_FUN( do_unveil );
DECLARE_DO_FUN( do_vampirearmor );
DECLARE_DO_FUN( do_vbite );
DECLARE_DO_FUN( do_vinferno );
DECLARE_DO_FUN( do_death );
DECLARE_DO_FUN( do_withering );

/*
 * Highwind commands.
 */
DECLARE_DO_FUN( do_dragon );
DECLARE_DO_FUN( do_dragon_dive );
DECLARE_DO_FUN( do_dragoon_throw );
DECLARE_DO_FUN( do_dynamite );
DECLARE_DO_FUN( do_higharmor );
DECLARE_DO_FUN( do_highwind );
DECLARE_DO_FUN( do_hyper_jump );
DECLARE_DO_FUN( do_impale );
DECLARE_DO_FUN( do_jump );
DECLARE_DO_FUN( do_ultra_jump );
DECLARE_DO_FUN( do_virtual_spears );
DECLARE_DO_FUN( do_whiskey_break );

/*
 * Demon commands.
 */
DECLARE_DO_FUN( do_blink );
DECLARE_DO_FUN( do_caust );
DECLARE_DO_FUN( do_chaosport );
DECLARE_DO_FUN( do_damnation );
DECLARE_DO_FUN( do_dark_force );
DECLARE_DO_FUN( do_endo_zone );
DECLARE_DO_FUN( do_crucifix );
DECLARE_DO_FUN( do_demon_cradle );
DECLARE_DO_FUN( do_revenga );
DECLARE_DO_FUN( do_yami_urasugi );
DECLARE_DO_FUN( do_break_through );
DECLARE_DO_FUN( do_battle_lust );
DECLARE_DO_FUN( do_shed );
DECLARE_DO_FUN( do_zombie );
DECLARE_DO_FUN( do_hail_storm );
DECLARE_DO_FUN( do_dark_holy );
DECLARE_DO_FUN( do_curse );
DECLARE_DO_FUN( do_demonarmour );
DECLARE_DO_FUN( do_demonform );
DECLARE_DO_FUN( do_desanctify );
DECLARE_DO_FUN( do_entomb );
DECLARE_DO_FUN( do_eyespy );
DECLARE_DO_FUN( do_frostbreath );
DECLARE_DO_FUN( do_fbite );
DECLARE_DO_FUN( do_graft );
DECLARE_DO_FUN( do_gust );
DECLARE_DO_FUN( do_hellfire );
DECLARE_DO_FUN( do_hooves );
DECLARE_DO_FUN( do_horns );
DECLARE_DO_FUN( do_humanform );
DECLARE_DO_FUN( do_immolate );
DECLARE_DO_FUN( do_dinferno );
DECLARE_DO_FUN( do_inpart );
DECLARE_DO_FUN( do_lava_pit );
DECLARE_DO_FUN( do_leap );
DECLARE_DO_FUN( do_leech );
DECLARE_DO_FUN( do_lifespan );
DECLARE_DO_FUN( do_roll );
DECLARE_DO_FUN( do_seed );
DECLARE_DO_FUN( do_sin );
DECLARE_DO_FUN( do_soul_dream );
DECLARE_DO_FUN( do_soulheal );
DECLARE_DO_FUN( do_soullink );
DECLARE_DO_FUN( do_tail );
DECLARE_DO_FUN( do_unnerve );
DECLARE_DO_FUN( do_weaponform );
DECLARE_DO_FUN( do_wfreeze );
DECLARE_DO_FUN( do_wings );

/*
 * Demon commands.
 */
DECLARE_DO_FUN( do_air_raid );
DECLARE_DO_FUN( do_fiend_damnation );
DECLARE_DO_FUN( do_true_demon );
DECLARE_DO_FUN( do_false_pride );
DECLARE_DO_FUN( do_fiend_inferno );
DECLARE_DO_FUN( do_fiend_lifespan );
DECLARE_DO_FUN( do_lunge_kick );
DECLARE_DO_FUN( do_magma_drive );
DECLARE_DO_FUN( do_meteor_blast );
DECLARE_DO_FUN( do_round_trip );
DECLARE_DO_FUN( do_shadow_gate );
DECLARE_DO_FUN( do_stinger );
DECLARE_DO_FUN( do_vortex );


/*
 * Mage Commands.
 */
DECLARE_DO_FUN( do_abduction );
DECLARE_DO_FUN( do_age );
DECLARE_DO_FUN( do_aggression );
DECLARE_DO_FUN( do_anti_magic );
DECLARE_DO_FUN( do_anti_aura );
DECLARE_DO_FUN( do_alabaster_potion );
DECLARE_DO_FUN( do_banshee );
DECLARE_DO_FUN( do_blessing );
DECLARE_DO_FUN( do_briar_shield );
DECLARE_DO_FUN( do_reflect_beam );
DECLARE_DO_FUN( do_chain_lightning );
DECLARE_DO_FUN( do_cleanse );
DECLARE_DO_FUN( do_mage_clone );
DECLARE_DO_FUN( do_mage_drain );
DECLARE_DO_FUN( do_mage_ritual );
DECLARE_DO_FUN( do_disempower );
DECLARE_DO_FUN( do_doomsday );
DECLARE_DO_FUN( do_drain_power );
DECLARE_DO_FUN( do_evocate );
DECLARE_DO_FUN( do_fireball );
DECLARE_DO_FUN( do_icelance );
DECLARE_DO_FUN( do_gaea_blessing );
DECLARE_DO_FUN( do_gaseous );
DECLARE_DO_FUN( do_great_wall );
DECLARE_DO_FUN( do_haste );
DECLARE_DO_FUN( do_layhands );
DECLARE_DO_FUN( do_magearmor );
DECLARE_DO_FUN( do_mangara_blessing );
DECLARE_DO_FUN( do_melt );
DECLARE_DO_FUN( do_meteor_shower );
DECLARE_DO_FUN( do_mknights );
DECLARE_DO_FUN( do_nature_revolt );
DECLARE_DO_FUN( do_objectgate );
DECLARE_DO_FUN( do_primal_rage );
DECLARE_DO_FUN( do_quicken );
DECLARE_DO_FUN( do_timerlock );
DECLARE_DO_FUN( do_ltouch );
DECLARE_DO_FUN( do_gambler );
DECLARE_DO_FUN( do_royal_flush );
DECLARE_DO_FUN( do_subtle_pinch );
DECLARE_DO_FUN( do_specialize );
DECLARE_DO_FUN( do_reveal );
DECLARE_DO_FUN( do_slow );
DECLARE_DO_FUN( do_soul_field );
DECLARE_DO_FUN( do_stone_rain );
DECLARE_DO_FUN( do_summon );
DECLARE_DO_FUN( do_unsummon );

/*
 * Wizard Commands.
 */
DECLARE_DO_FUN( do_accelerate );
DECLARE_DO_FUN( do_analyze );
DECLARE_DO_FUN( do_advise );
DECLARE_DO_FUN( do_alterweight );
DECLARE_DO_FUN( do_alterstate );
DECLARE_DO_FUN( do_betterbody );
DECLARE_DO_FUN( do_call_lightning );
DECLARE_DO_FUN( do_callspirit );
DECLARE_DO_FUN( do_chant );
DECLARE_DO_FUN( do_aureola );
DECLARE_DO_FUN( do_control_weather );
DECLARE_DO_FUN( do_dimmak );
DECLARE_DO_FUN( do_discharge );
DECLARE_DO_FUN( do_earthembrace );
DECLARE_DO_FUN( do_enrage );
DECLARE_DO_FUN( do_elemental_touch );
DECLARE_DO_FUN( do_enchant );
DECLARE_DO_FUN( do_enchantarmor );
DECLARE_DO_FUN( do_objectgate );
DECLARE_DO_FUN( do_hotienchi );
DECLARE_DO_FUN( do_hover );
DECLARE_DO_FUN( do_littledeath );
DECLARE_DO_FUN( do_mage_breach );
DECLARE_DO_FUN( do_mlearn );
DECLARE_DO_FUN( do_mshadow );
DECLARE_DO_FUN( do_perception );
DECLARE_DO_FUN( do_polyappear );
DECLARE_DO_FUN( do_purification );
DECLARE_DO_FUN( do_qblast );
DECLARE_DO_FUN( do_quintessence );
DECLARE_DO_FUN( do_read );
DECLARE_DO_FUN( do_rubbing );
DECLARE_DO_FUN( do_shapechange );
DECLARE_DO_FUN( do_spheres );
DECLARE_DO_FUN( do_spiritblast );
DECLARE_DO_FUN( do_spiritkiss );
DECLARE_DO_FUN( do_stop );
DECLARE_DO_FUN( do_telekinetics );
DECLARE_DO_FUN( do_tempest );
DECLARE_DO_FUN( do_ultima );
DECLARE_DO_FUN( do_write );
DECLARE_DO_FUN( do_darkball );
DECLARE_DO_FUN( do_timewarp );

DECLARE_DO_FUN( do_mirror_stance );
DECLARE_DO_FUN( do_true_sacrifice );
DECLARE_DO_FUN( do_time_stop );
DECLARE_DO_FUN( do_blaster );
DECLARE_DO_FUN( do_destroyer );
DECLARE_DO_FUN( do_shadow_walk );
DECLARE_DO_FUN( do_scout );
DECLARE_DO_FUN( do_shroud_strike );
DECLARE_DO_FUN( do_anti_light );
DECLARE_DO_FUN( do_zanretsuken );
DECLARE_DO_FUN( do_rhammer );
DECLARE_DO_FUN( do_light_blade );
DECLARE_DO_FUN( do_voodoo );


/*
 * Necromancer Commands.
 */
DECLARE_DO_FUN( do_anti_cross );
DECLARE_DO_FUN( do_madness );
DECLARE_DO_FUN( do_seek );
DECLARE_DO_FUN( do_reanimate );
DECLARE_DO_FUN( do_binding_agony );
DECLARE_DO_FUN( do_calling );
DECLARE_DO_FUN( do_ereshkigal );
DECLARE_DO_FUN( do_corpse_explosion );
DECLARE_DO_FUN( do_gas_cloud );
DECLARE_DO_FUN( do_necro_drain );
DECLARE_DO_FUN( do_reduce );
DECLARE_DO_FUN( do_shadowgrasp );
DECLARE_DO_FUN( do_venom );

/*
 * Summoner Commands.
 */
DECLARE_DO_FUN( do_bahamut );
DECLARE_DO_FUN( do_ddarkside );
DECLARE_DO_FUN( do_diamond_dust );
DECLARE_DO_FUN( do_gilgamesh );
DECLARE_DO_FUN( do_lloth );
DECLARE_DO_FUN( do_mywind );
DECLARE_DO_FUN( do_lancelot );
DECLARE_DO_FUN( do_tyrael );
DECLARE_DO_FUN( do_ifrit );
DECLARE_DO_FUN( do_merton );
DECLARE_DO_FUN( do_mwall );
DECLARE_DO_FUN( do_silent_voice );
DECLARE_DO_FUN( do_sun_bath );
DECLARE_DO_FUN( do_thunder_storm );
DECLARE_DO_FUN( do_titan );
DECLARE_DO_FUN( do_luna );
DECLARE_DO_FUN( do_gaia );
DECLARE_DO_FUN( do_zantetsuken );
DECLARE_DO_FUN( do_salamander );


/*
 * Werewolf commands.
 */
DECLARE_DO_FUN( do_aeolus );
DECLARE_DO_FUN( do_avalanche );
DECLARE_DO_FUN( do_lunar_eclipse );
DECLARE_DO_FUN( do_lunar_power );
DECLARE_DO_FUN( do_lunar_touch );
DECLARE_DO_FUN( do_brew );
DECLARE_DO_FUN( do_balorsgaze );
DECLARE_DO_FUN( do_barrage );
DECLARE_DO_FUN( do_blissful );
DECLARE_DO_FUN( do_bodywrack );
DECLARE_DO_FUN( do_bridgegate );
DECLARE_DO_FUN( do_cannibalspirit );
DECLARE_DO_FUN( do_chillfrost );
DECLARE_DO_FUN( do_clapofthunder );
DECLARE_DO_FUN( do_clenched );
DECLARE_DO_FUN( do_cocoon );
DECLARE_DO_FUN( do_cuttingwind );
DECLARE_DO_FUN( do_darkshroud );
DECLARE_DO_FUN( do_dazzle );
DECLARE_DO_FUN( do_disquiet );
DECLARE_DO_FUN( do_elementalgift );
DECLARE_DO_FUN( do_flamespirit );
DECLARE_DO_FUN( do_feral );
DECLARE_DO_FUN( do_garouserenity );
DECLARE_DO_FUN( do_gifts );
DECLARE_DO_FUN( do_gnaw );
DECLARE_DO_FUN( do_halosun );
DECLARE_DO_FUN( do_heartofice );
DECLARE_DO_FUN( do_heightened );
DECLARE_DO_FUN( do_icychill );
DECLARE_DO_FUN( do_invisibility );
DECLARE_DO_FUN( do_lambent );
DECLARE_DO_FUN( do_livingwood );
DECLARE_DO_FUN( do_lunasavenger );
DECLARE_DO_FUN( do_lunasblessing );
DECLARE_DO_FUN( do_malleable );
DECLARE_DO_FUN( do_merciful );
DECLARE_DO_FUN( do_moonarmor );
DECLARE_DO_FUN( do_moonbeam );
DECLARE_DO_FUN( do_mothers );
DECLARE_DO_FUN( do_paws );
DECLARE_DO_FUN( do_quicksand );
DECLARE_DO_FUN( do_quills );
DECLARE_DO_FUN( do_wither );
DECLARE_DO_FUN( do_rage );
DECLARE_DO_FUN( do_ragesong );
DECLARE_DO_FUN( do_razorclaws );
DECLARE_DO_FUN( do_reshape );
DECLARE_DO_FUN( do_shadowplane );
DECLARE_DO_FUN( do_snarl );
DECLARE_DO_FUN( do_songbeast );
DECLARE_DO_FUN( do_spiral_moon );
DECLARE_DO_FUN( do_giant );
DECLARE_DO_FUN( do_spiritdrain );
DECLARE_DO_FUN( do_staredown );
DECLARE_DO_FUN( do_vanish );
DECLARE_DO_FUN( do_vengeance );
DECLARE_DO_FUN( do_wallofgranite );
DECLARE_DO_FUN( do_waste );
DECLARE_DO_FUN( do_whelp );

/*
 * Ninja commands.
 */
DECLARE_DO_FUN( do_choke );
DECLARE_DO_FUN( do_dig );
DECLARE_DO_FUN( do_inclimb );
DECLARE_DO_FUN( do_incapitate );
DECLARE_DO_FUN( do_kakusu );
DECLARE_DO_FUN( do_kanzuite );
DECLARE_DO_FUN( do_michi );
DECLARE_DO_FUN( do_orimasu );
DECLARE_DO_FUN( do_mitsukeru );
DECLARE_DO_FUN( do_ninjaarmor );
DECLARE_DO_FUN( do_ninja_assassinate );

/*
 * Night Blade commands.
 */
DECLARE_DO_FUN( do_bomuzite );
DECLARE_DO_FUN( do_circle );
DECLARE_DO_FUN( do_hakunetsu );
DECLARE_DO_FUN( do_hara_kiri );
DECLARE_DO_FUN( do_hunt );
DECLARE_DO_FUN( do_liakanitsu );
DECLARE_DO_FUN( do_koryou );
DECLARE_DO_FUN( do_mienaku );
DECLARE_DO_FUN( do_miezite );
DECLARE_DO_FUN( do_mizotamouyo );
DECLARE_DO_FUN( do_principles );
DECLARE_DO_FUN( do_sonic_strike );
DECLARE_DO_FUN( do_screen );
DECLARE_DO_FUN( do_trip );
DECLARE_DO_FUN( do_tsume );
DECLARE_DO_FUN( do_zinaki );
DECLARE_DO_FUN( do_zsikuku );

/*
 * Gaiden commands.
 */
DECLARE_DO_FUN( do_alertness );
DECLARE_DO_FUN( do_backfist );
DECLARE_DO_FUN( do_caltrops );
DECLARE_DO_FUN( do_dinichi );
DECLARE_DO_FUN( do_illusion );
DECLARE_DO_FUN( do_incapacitator );
DECLARE_DO_FUN( do_izunaken );
DECLARE_DO_FUN( do_jakouha );
DECLARE_DO_FUN( do_jigokute );
DECLARE_DO_FUN( do_nknee );
DECLARE_DO_FUN( do_lowkick );
DECLARE_DO_FUN( do_lsamu );
DECLARE_DO_FUN( do_nappaku );
DECLARE_DO_FUN( do_pain_touch );
DECLARE_DO_FUN( do_research );
DECLARE_DO_FUN( do_nerval_touch );
DECLARE_DO_FUN( do_hishoken_slam );
DECLARE_DO_FUN( do_reverse );
DECLARE_DO_FUN( do_rising_tiger );
DECLARE_DO_FUN( do_spin_kick );
DECLARE_DO_FUN( do_ssaki );
DECLARE_DO_FUN( do_stardance );
DECLARE_DO_FUN( do_nsweep );
DECLARE_DO_FUN( do_throw );
DECLARE_DO_FUN( do_thrust_kick );
DECLARE_DO_FUN( do_uppercut );
DECLARE_DO_FUN( do_yoga );

/*
 * Amazon commands.
 */
DECLARE_DO_FUN( do_amazoncreate );
DECLARE_DO_FUN( do_barkskin );
DECLARE_DO_FUN( do_camouflage );
DECLARE_DO_FUN( do_decoy );
DECLARE_DO_FUN( do_drill );
DECLARE_DO_FUN( do_amazon_fend );
DECLARE_DO_FUN( do_impale );
DECLARE_DO_FUN( do_lightning_fury );
DECLARE_DO_FUN( do_net );
DECLARE_DO_FUN( do_phoenix_strike );
DECLARE_DO_FUN( do_power_strike );
DECLARE_DO_FUN( do_amazon_shoot );
DECLARE_DO_FUN( do_amazon_jabs );
DECLARE_DO_FUN( do_steelfist );
DECLARE_DO_FUN( do_tornado );
DECLARE_DO_FUN( do_vacuum_spear );

/*
 * Kingdom Powers
 */
DECLARE_DO_FUN( do_warcry );
DECLARE_DO_FUN( do_assist );
DECLARE_DO_FUN( do_dragonbuster );
DECLARE_DO_FUN( do_energysword );
DECLARE_DO_FUN( do_lightningblade );
DECLARE_DO_FUN( do_crescent_slash );

 /*
  * Sorceror commands.
  */
DECLARE_DO_FUN( do_aura_wave );
DECLARE_DO_FUN( do_mani_katti );
DECLARE_DO_FUN( do_sol_katti );
DECLARE_DO_FUN( do_aurawhirl );
DECLARE_DO_FUN( do_blaze );
DECLARE_DO_FUN( do_blitzstrike );
DECLARE_DO_FUN( do_blitzsword );
DECLARE_DO_FUN( do_blitztouch );
DECLARE_DO_FUN( do_chaos_sword );
DECLARE_DO_FUN( do_cushionwhirl );
DECLARE_DO_FUN( do_dancingflames );
DECLARE_DO_FUN( do_darksword );
DECLARE_DO_FUN( do_deathfield );
DECLARE_DO_FUN( do_despairstrike );
DECLARE_DO_FUN( do_divinewave );
DECLARE_DO_FUN( do_earthsword );
DECLARE_DO_FUN( do_earthwall );
DECLARE_DO_FUN( do_eleshield );
DECLARE_DO_FUN( do_encase );
DECLARE_DO_FUN( do_finalflame );
DECLARE_DO_FUN( do_firesword );
DECLARE_DO_FUN( do_firewall );
DECLARE_DO_FUN( do_freezingrain );
DECLARE_DO_FUN( do_frigidcut );
DECLARE_DO_FUN( do_holysword );
DECLARE_DO_FUN( do_icearmor );
DECLARE_DO_FUN( do_iceshield );
DECLARE_DO_FUN( do_icesword );
DECLARE_DO_FUN( do_icewall );
DECLARE_DO_FUN( do_lunabeam );
DECLARE_DO_FUN( do_magiwall );
DECLARE_DO_FUN( do_moonslash );
DECLARE_DO_FUN( do_moonsword );
DECLARE_DO_FUN( do_piercing_one );
DECLARE_DO_FUN( do_pulsar_drill );
DECLARE_DO_FUN( do_quadraslice );
DECLARE_DO_FUN( do_ragedash );
DECLARE_DO_FUN( do_ravingeyes );
DECLARE_DO_FUN( do_renzokuken );
DECLARE_DO_FUN( do_rockblast );
DECLARE_DO_FUN( do_shiningwind );
DECLARE_DO_FUN( do_swordcreate );
DECLARE_DO_FUN( do_swordlearn );
DECLARE_DO_FUN( do_thunderbeam );
DECLARE_DO_FUN( do_vengenceslash );

/*
 * Hunter commands.
 */
DECLARE_DO_FUN( do_artic_storm );
DECLARE_DO_FUN( do_artic_wave );
DECLARE_DO_FUN( do_death_beam );
DECLARE_DO_FUN( do_entangle );
DECLARE_DO_FUN( do_energy_blast );
DECLARE_DO_FUN( do_ethereal_stake );
DECLARE_DO_FUN( do_flaming_strike );
DECLARE_DO_FUN( do_hold );
DECLARE_DO_FUN( do_hand_freeze );
DECLARE_DO_FUN( do_harpoon );
DECLARE_DO_FUN( do_huntercreate );
DECLARE_DO_FUN( do_hunter_lunge );
DECLARE_DO_FUN( do_use );
DECLARE_DO_FUN( do_lunge );
DECLARE_DO_FUN( do_mega_jump );
DECLARE_DO_FUN( do_power_smash );
DECLARE_DO_FUN( do_rage_strike );
DECLARE_DO_FUN( do_silver_rain );
DECLARE_DO_FUN( do_spiritform );
DECLARE_DO_FUN( do_sustain );

/*
 * Dark Knight commands.
 */
DECLARE_DO_FUN( do_analyse );
DECLARE_DO_FUN( do_anti_stock );
DECLARE_DO_FUN( do_breach );
DECLARE_DO_FUN( do_confuser );
DECLARE_DO_FUN( do_darkcreate );
DECLARE_DO_FUN( do_dark_storm );
DECLARE_DO_FUN( do_determine );
DECLARE_DO_FUN( do_finishing_touch );
DECLARE_DO_FUN( do_lumica_barrier );
DECLARE_DO_FUN( do_knight_round );
DECLARE_DO_FUN( do_knight_sweep );
DECLARE_DO_FUN( do_knight_takedown );
DECLARE_DO_FUN( do_knight_upper );
DECLARE_DO_FUN( do_mind_barrier );
DECLARE_DO_FUN( do_slash );
DECLARE_DO_FUN( do_thought );
DECLARE_DO_FUN( do_absolute_zero );
DECLARE_DO_FUN( do_hyper );
DECLARE_DO_FUN( do_hwrath );
DECLARE_DO_FUN( do_mergance );
DECLARE_DO_FUN( do_moon_wave );
DECLARE_DO_FUN( do_psychic_barrier );
DECLARE_DO_FUN( do_stitch );

/*
 * Undead Knight commands.
 */
DECLARE_DO_FUN( do_gain );
DECLARE_DO_FUN( do_genocide );
DECLARE_DO_FUN( do_knightarmor );
DECLARE_DO_FUN( do_powerword );
DECLARE_DO_FUN( do_armads );
DECLARE_DO_FUN( do_tetra_slice );
DECLARE_DO_FUN( do_unholyrite );
DECLARE_DO_FUN( do_weaponpractice );

/*
 * Saiyan commands.
 */
DECLARE_DO_FUN( do_bakuhatsuha );
DECLARE_DO_FUN( do_big_bang );
DECLARE_DO_FUN( do_charging );
DECLARE_DO_FUN( do_chou_kamehameha );
DECLARE_DO_FUN( do_distrucodisc );
DECLARE_DO_FUN( do_gallet_gun );
DECLARE_DO_FUN( do_imbreed );
DECLARE_DO_FUN( do_kakusan );
DECLARE_DO_FUN( do_kamehameha );
DECLARE_DO_FUN( do_kaioken );
DECLARE_DO_FUN( do_mouthblast );
DECLARE_DO_FUN( do_pounce );
DECLARE_DO_FUN( do_power_ball );
DECLARE_DO_FUN( do_power_read );
DECLARE_DO_FUN( do_power_search );
DECLARE_DO_FUN( do_rage_invoke );
DECLARE_DO_FUN( do_ryuken );
DECLARE_DO_FUN( do_saiyancreate );
DECLARE_DO_FUN( do_scatterpunch );
DECLARE_DO_FUN( do_solarflare );
DECLARE_DO_FUN( do_soukidan );
DECLARE_DO_FUN( do_spiritbomb );
DECLARE_DO_FUN( do_supersaiyan );
DECLARE_DO_FUN( do_tailswipe );
DECLARE_DO_FUN( perform_gallet_gun );
DECLARE_DO_FUN( perform_kamehameha );

/*
 * Daywalker commands.
 */
DECLARE_DO_FUN( do_armor_thorns );
DECLARE_DO_FUN( do_cgrab );
DECLARE_DO_FUN( do_cursedblood );
DECLARE_DO_FUN( do_day_dream );
DECLARE_DO_FUN( do_daywalker_ritual );
DECLARE_DO_FUN( do_death_grip );
DECLARE_DO_FUN( do_desparation );
DECLARE_DO_FUN( do_despotic_septor );
DECLARE_DO_FUN( do_dcreate );
DECLARE_DO_FUN( do_dtaste );
DECLARE_DO_FUN( do_egrab );
DECLARE_DO_FUN( do_holy_water );
DECLARE_DO_FUN( do_iceburst );
DECLARE_DO_FUN( do_inquest );
DECLARE_DO_FUN( do_insight );
DECLARE_DO_FUN( do_jiamar );
DECLARE_DO_FUN( do_karma );
DECLARE_DO_FUN( do_nature_cleansing );
DECLARE_DO_FUN( do_nature_wrath );
DECLARE_DO_FUN( do_nightmare );
DECLARE_DO_FUN( do_piercing_rain );
DECLARE_DO_FUN( do_psionicblast );
DECLARE_DO_FUN( do_rathi );
DECLARE_DO_FUN( do_recover );
DECLARE_DO_FUN( do_serum );
DECLARE_DO_FUN( do_shakar );
DECLARE_DO_FUN( do_shatter );
DECLARE_DO_FUN( do_sonic_thrust );
DECLARE_DO_FUN( do_soul_drain );
DECLARE_DO_FUN( do_stakewall );
DECLARE_DO_FUN( do_statis );
DECLARE_DO_FUN( do_sunbeam );
DECLARE_DO_FUN( do_transform );
DECLARE_DO_FUN( do_unstable );
DECLARE_DO_FUN( do_vortex );

/*
 * Samurai commands.
 */
DECLARE_DO_FUN( do_combo );
DECLARE_DO_FUN( do_quick_lash );
DECLARE_DO_FUN( do_elemental_slash );
DECLARE_DO_FUN( do_knowledge );
DECLARE_DO_FUN( do_light_slash );
DECLARE_DO_FUN( do_samurai_mslash );
DECLARE_DO_FUN( do_samurai_hslash );
DECLARE_DO_FUN( do_samuricreate );
DECLARE_DO_FUN( do_sdraw );
DECLARE_DO_FUN( do_seppuko );
DECLARE_DO_FUN( do_withdraw );

/*
 * Daramon Monk commands.
 */
DECLARE_DO_FUN( do_buddhastomp );
DECLARE_DO_FUN( do_chakra );
DECLARE_DO_FUN( do_chill_blast );
DECLARE_DO_FUN( do_daracreate );
DECLARE_DO_FUN( do_empower );
DECLARE_DO_FUN( do_exploder );
DECLARE_DO_FUN( do_heavenly_gate );
DECLARE_DO_FUN( do_f_uppercut );
DECLARE_DO_FUN( do_l_slash );
DECLARE_DO_FUN( do_shock_wave );
DECLARE_DO_FUN( do_stunner );
DECLARE_DO_FUN( do_stunning_blow );
DECLARE_DO_FUN( do_tremor );
DECLARE_DO_FUN( do_zinja_kicks );

/*
 * Assassin commands.
 */
DECLARE_DO_FUN( do_barrier );
DECLARE_DO_FUN( do_chi_blast );
DECLARE_DO_FUN( do_chi_cleanse );
DECLARE_DO_FUN( do_flash_kick );
DECLARE_DO_FUN( do_dark_hadou );
DECLARE_DO_FUN( do_hakiznetu );
DECLARE_DO_FUN( do_inner_chi );
DECLARE_DO_FUN( do_palm_tackle );
DECLARE_DO_FUN( do_raging_demon );
DECLARE_DO_FUN( do_shadow_fuse );
DECLARE_DO_FUN( do_shadow_thrust );
DECLARE_DO_FUN( do_shadowswap );
DECLARE_DO_FUN( do_split_strike );
DECLARE_DO_FUN( do_swirling_smite );
DECLARE_DO_FUN( do_target );
DECLARE_DO_FUN( do_aura_all );
DECLARE_DO_FUN( do_earth_smash );
DECLARE_DO_FUN( do_justice_dance );
DECLARE_DO_FUN( do_ice_break );
DECLARE_DO_FUN( do_fire_dash );
DECLARE_DO_FUN( do_shock_blade );
DECLARE_DO_FUN( do_lightning_fist );
DECLARE_DO_FUN( do_meteor_kick );
DECLARE_DO_FUN( do_tenho_blast );
DECLARE_DO_FUN( do_angel_wings );
DECLARE_DO_FUN( do_ice_blast );
DECLARE_DO_FUN( do_roulette );
DECLARE_DO_FUN( do_sonic_arc );
DECLARE_DO_FUN( do_thunder_blade );
DECLARE_DO_FUN( do_lunar_blade );
DECLARE_DO_FUN( do_omega_blade );
DECLARE_DO_FUN( do_entangle );
DECLARE_DO_FUN( do_koho );
DECLARE_DO_FUN( do_rune_blade );
DECLARE_DO_FUN( do_rcannon );
DECLARE_DO_FUN( do_xbuster );
DECLARE_DO_FUN( do_forever_zero );
DECLARE_DO_FUN( do_explode );
DECLARE_DO_FUN( do_fire_bomb );
DECLARE_DO_FUN( do_satellite );
DECLARE_DO_FUN( do_lunar_shield );
DECLARE_DO_FUN( do_executioner );


/*
 * Immortal commands.
 */
DECLARE_DO_FUN( do_ainfo );
DECLARE_DO_FUN( do_addlag );
DECLARE_DO_FUN( do_allow );
DECLARE_DO_FUN( do_armageddon );
DECLARE_DO_FUN( do_kingdom_wars );
DECLARE_DO_FUN( do_at );
DECLARE_DO_FUN( do_bamfin );
DECLARE_DO_FUN( do_bamfout );
DECLARE_DO_FUN( do_ban );
DECLARE_DO_FUN( do_bind );
DECLARE_DO_FUN( do_copyover );
DECLARE_DO_FUN( do_create );
DECLARE_DO_FUN( do_ctoken );
DECLARE_DO_FUN( do_pktoken );
DECLARE_DO_FUN( do_deny );
DECLARE_DO_FUN( do_destroy );
DECLARE_DO_FUN( do_disable );
DECLARE_DO_FUN( do_disconnect );
DECLARE_DO_FUN( do_divorce );
DECLARE_DO_FUN( do_doublexp );
DECLARE_DO_FUN( do_echo );
DECLARE_DO_FUN( do_for );
DECLARE_DO_FUN( do_force );
DECLARE_DO_FUN( do_forceauto );
DECLARE_DO_FUN( do_freeze );
DECLARE_DO_FUN( do_ftag );
DECLARE_DO_FUN( do_goto );
DECLARE_DO_FUN( do_holylight );
DECLARE_DO_FUN( do_hlist );
DECLARE_DO_FUN( do_return );
DECLARE_DO_FUN( do_immtalk );
DECLARE_DO_FUN( do_incog );
DECLARE_DO_FUN( do_info );
DECLARE_DO_FUN( do_acinfo );
DECLARE_DO_FUN( do_sinfo );
DECLARE_DO_FUN( do_invis );
DECLARE_DO_FUN( do_kingnum );
DECLARE_DO_FUN( do_kingset );
DECLARE_DO_FUN( do_leaderclear );
DECLARE_DO_FUN( do_linkdead );
DECLARE_DO_FUN( do_log );
DECLARE_DO_FUN( do_marry );
DECLARE_DO_FUN( do_medit );
DECLARE_DO_FUN( do_memory );
DECLARE_DO_FUN( do_mfind );
DECLARE_DO_FUN( do_mload );
DECLARE_DO_FUN( do_mset );
DECLARE_DO_FUN( do_mstat );
DECLARE_DO_FUN( do_multicheck );
DECLARE_DO_FUN( do_mwhere );
DECLARE_DO_FUN( do_noemote );
DECLARE_DO_FUN( do_nogang );
DECLARE_DO_FUN( do_nosafe );
DECLARE_DO_FUN( do_notell );
DECLARE_DO_FUN( do_oclone );
DECLARE_DO_FUN( do_ofind );
DECLARE_DO_FUN( do_oload );
DECLARE_DO_FUN( do_omni );
DECLARE_DO_FUN( do_oreturn );
DECLARE_DO_FUN( do_oset );
DECLARE_DO_FUN( do_ostat );
DECLARE_DO_FUN( do_oswitch );
DECLARE_DO_FUN( do_otransfer );
DECLARE_DO_FUN( do_pack );
DECLARE_DO_FUN( do_paradox );
DECLARE_DO_FUN( do_peace );
DECLARE_DO_FUN( do_permban );
DECLARE_DO_FUN( do_pfile );
DECLARE_DO_FUN( do_pload );
DECLARE_DO_FUN( do_pset );
DECLARE_DO_FUN( do_pstat );
DECLARE_DO_FUN( do_purge );
DECLARE_DO_FUN( do_qset );
DECLARE_DO_FUN( do_qstat );
DECLARE_DO_FUN( do_qtrust );
DECLARE_DO_FUN( do_reboo );
DECLARE_DO_FUN( do_reboot );
DECLARE_DO_FUN( do_recho );
DECLARE_DO_FUN( do_relevel );
DECLARE_DO_FUN( do_resetarea );
DECLARE_DO_FUN( do_resetpassword );
DECLARE_DO_FUN( do_restore );
DECLARE_DO_FUN( do_rset );
DECLARE_DO_FUN( do_rstat );
DECLARE_DO_FUN( do_silence );
DECLARE_DO_FUN( do_shutdow );
DECLARE_DO_FUN( do_shutdown );
DECLARE_DO_FUN( do_sla );
DECLARE_DO_FUN( do_slay );
DECLARE_DO_FUN( do_slookup );
DECLARE_DO_FUN( do_snoop );
DECLARE_DO_FUN( do_special );
DECLARE_DO_FUN( do_sset );
DECLARE_DO_FUN( do_tick );
DECLARE_DO_FUN( do_token );
DECLARE_DO_FUN( do_transfer );
DECLARE_DO_FUN( do_transform );
DECLARE_DO_FUN( do_trust );
DECLARE_DO_FUN( do_undeny );
DECLARE_DO_FUN( do_users );
DECLARE_DO_FUN( do_wizlock );
DECLARE_DO_FUN( show_sex );

/*
 * OLC 1.1b
 */
DECLARE_DO_FUN( do_aedit );
DECLARE_DO_FUN( do_alist );
DECLARE_DO_FUN( do_asave );
DECLARE_DO_FUN( do_oedit );
DECLARE_DO_FUN( do_redit );
DECLARE_DO_FUN( do_relearn );
DECLARE_DO_FUN( do_resets );
DECLARE_DO_FUN( do_quest_check );


/* New Commands */
DECLARE_DO_FUN( do_pfile        );
DECLARE_DO_FUN( jope_done       );
DECLARE_DO_FUN( jope_exp        );
DECLARE_DO_FUN( jope_gold       );
DECLARE_DO_FUN( jope_addaffect  );
DECLARE_DO_FUN( jope_removeaffect );
DECLARE_DO_FUN( jope_show       );
DECLARE_DO_FUN( jope_list       );
DECLARE_DO_FUN( jope_spells     );
DECLARE_DO_FUN( jope_weapons    );
DECLARE_DO_FUN( jope_action     );
DECLARE_DO_FUN( jope_inventory  );
DECLARE_DO_FUN( jope_drop       );
DECLARE_DO_FUN( jope_equipment  );
DECLARE_DO_FUN( jope_get        );
DECLARE_DO_FUN( jope_look       );
DECLARE_DO_FUN( jope_remove     );
DECLARE_DO_FUN( jope_wear       );
DECLARE_DO_FUN( jope_newbits    );
DECLARE_DO_FUN( jope_act        );
DECLARE_DO_FUN( jope_primal     );
DECLARE_DO_FUN( jope_duration   );
DECLARE_DO_FUN( jope_hit        );
DECLARE_DO_FUN( jope_move       );
DECLARE_DO_FUN( jope_mana       );
DECLARE_DO_FUN( jope_pkill      );
DECLARE_DO_FUN( jope_pdeath     );
DECLARE_DO_FUN( jope_mkill      );
DECLARE_DO_FUN( jope_mdeath     );


DECLARE_DO_FUN( do_aura );
DECLARE_DO_FUN( do_worship );
DECLARE_DO_FUN( do_beastlike );
DECLARE_DO_FUN( do_calm );
DECLARE_DO_FUN( do_claws );
DECLARE_DO_FUN( do_fangs );
DECLARE_DO_FUN( do_killperson );
DECLARE_DO_FUN( do_majorenchant );
DECLARE_DO_FUN( do_spydirection );


/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_briar_shield );
DECLARE_SPELL_FUN(	spell_gaea_blessing );
DECLARE_SPELL_FUN(	spell_earth_wall );
DECLARE_SPELL_FUN(	spell_fire_wall );
DECLARE_SPELL_FUN(	spell_mangara_blessing );
DECLARE_SPELL_FUN(	spell_alabaster_potion );
DECLARE_SPELL_FUN(	spell_rflush );
DECLARE_SPELL_FUN(	spell_gambler );

DECLARE_SPELL_FUN(	spell_haste 	);
DECLARE_SPELL_FUN(	spell_hyper 	);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_wall		);
DECLARE_SPELL_FUN(	spell_barbed_foliage );
DECLARE_SPELL_FUN(	spell_sun_aura );
DECLARE_SPELL_FUN(	spell_dark_holy );
DECLARE_SPELL_FUN(	spell_divine_might );
DECLARE_SPELL_FUN(	spell_lunar_eclipse );
DECLARE_SPELL_FUN(	spell_battle_tactics );
DECLARE_SPELL_FUN(	spell_mani_katti );
DECLARE_SPELL_FUN(	spell_sol_katti );
DECLARE_SPELL_FUN(	spell_vengeance );
DECLARE_SPELL_FUN(	spell_holy_shield );
DECLARE_SPELL_FUN(	spell_demonic_might );
DECLARE_SPELL_FUN(	spell_battle_lust	);
DECLARE_SPELL_FUN(	spell_hope	);
DECLARE_SPELL_FUN(	spell_tide	);
DECLARE_SPELL_FUN(	spell_hell_hound	);
DECLARE_SPELL_FUN(	spell_age	);
DECLARE_SPELL_FUN(	spell_quicken );
DECLARE_SPELL_FUN(	spell_divine_shield );
DECLARE_SPELL_FUN(	spell_eyes_heavens );
DECLARE_SPELL_FUN(	spell_holy_word	);
DECLARE_SPELL_FUN(	spell_magi_wall	);
DECLARE_SPELL_FUN(	spell_curse_allah	);
DECLARE_SPELL_FUN(	spell_holy_retrieval	);
DECLARE_SPELL_FUN(	spell_kaoken	);
DECLARE_SPELL_FUN(	spell_blessing		);
DECLARE_SPELL_FUN(	spell_full_cure		);
DECLARE_SPELL_FUN(	spell_divine_reflection	);
DECLARE_SPELL_FUN(	spell_divine_shield	);
DECLARE_SPELL_FUN(	spell_cloak_life	);
DECLARE_SPELL_FUN(	spell_resurrection	);
DECLARE_SPELL_FUN(	spell_godsdisfavor	);
DECLARE_SPELL_FUN(	spell_darkside	);
DECLARE_SPELL_FUN(	spell_despondency	);
DECLARE_SPELL_FUN(	spell_darkpath	);
DECLARE_SPELL_FUN(	spell_dark_kinesis );
DECLARE_SPELL_FUN(	spell_soulblight	);
DECLARE_SPELL_FUN(	spell_create_zombie );
DECLARE_SPELL_FUN(	spell_soulbolt	);
DECLARE_SPELL_FUN(	spell_dark_forge	);
DECLARE_SPELL_FUN(	spell_soulburn	);
DECLARE_SPELL_FUN(	spell_soulstop    );
DECLARE_SPELL_FUN(	spell_soulchill   );
DECLARE_SPELL_FUN(	spell_dark_kinesis );
DECLARE_SPELL_FUN(	spell_dark_shield	);
DECLARE_SPELL_FUN(	spell_unholy_strength	);
DECLARE_SPELL_FUN(	spell_divine_strength	);
DECLARE_SPELL_FUN(	spell_blazing_strikes	);
DECLARE_SPELL_FUN(	spell_demonic_speed	);
DECLARE_SPELL_FUN(	spell_protection_evil	);
DECLARE_SPELL_FUN(	spell_icearmor	);
DECLARE_SPELL_FUN(	spell_llothbless	);

DECLARE_SPELL_FUN(	spell_contraception	);
DECLARE_SPELL_FUN(      spell_spew              );
DECLARE_SPELL_FUN(	spell_infirmity		);
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(      spell_make_bag          );
DECLARE_SPELL_FUN(	spell_acid_blast	);

DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_godbless		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(	spell_darkness		);
DECLARE_SPELL_FUN(	spell_lunas_blessing  );
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_drowfire	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_enchant_armor	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_desanct		);
DECLARE_SPELL_FUN(      spell_imp_heal		);
DECLARE_SPELL_FUN(      spell_imp_fireball	);
DECLARE_SPELL_FUN(      spell_imp_faerie_fire	);
DECLARE_SPELL_FUN(      spell_imp_teleport	);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_general_purpose	);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_group_heal	);
DECLARE_SPELL_FUN(	spell_high_explosive	);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_readaura		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_protection	);
DECLARE_SPELL_FUN(	spell_protection_vs_good);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_godbless		);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_cone		);
DECLARE_SPELL_FUN(	spell_guardian		);
DECLARE_SPELL_FUN(	spell_soulblade		);
DECLARE_SPELL_FUN(	spell_mana		);
DECLARE_SPELL_FUN(	spell_frenzy		);
DECLARE_SPELL_FUN(	spell_darkblessing	);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_energyflux	);
DECLARE_SPELL_FUN(	spell_voodoo		);
DECLARE_SPELL_FUN(	spell_transport		);
DECLARE_SPELL_FUN(	spell_regenerate	);
DECLARE_SPELL_FUN(	spell_clot		);
DECLARE_SPELL_FUN(	spell_mend		);
DECLARE_SPELL_FUN(	spell_copy  	);
DECLARE_SPELL_FUN(	spell_insert_page );
DECLARE_SPELL_FUN(	spell_remove_page );
DECLARE_SPELL_FUN(	spell_major_creation	);
DECLARE_SPELL_FUN(	spell_minor_creation	);
DECLARE_SPELL_FUN(	spell_spiritkiss	);
DECLARE_SPELL_FUN(	spell_brew		);
DECLARE_SPELL_FUN( 	spell_jailwater		);
DECLARE_SPELL_FUN(	spell_scribe		);
DECLARE_SPELL_FUN(	spell_carve		);
DECLARE_SPELL_FUN(	spell_engrave		);
DECLARE_SPELL_FUN(	spell_bake		);
DECLARE_SPELL_FUN(	spell_mount		);
DECLARE_SPELL_FUN(	spell_scan		);
DECLARE_SPELL_FUN(	spell_repair		);
DECLARE_SPELL_FUN(	spell_spellproof	);
DECLARE_SPELL_FUN(	spell_preserve		);
DECLARE_SPELL_FUN(	spell_major_creation	);
DECLARE_SPELL_FUN(	spell_copy		);
DECLARE_SPELL_FUN(	spell_insert_page	);
DECLARE_SPELL_FUN(	spell_chaos_blast	);
DECLARE_SPELL_FUN(	spell_resistance	);
DECLARE_SPELL_FUN(	spell_web		);
DECLARE_SPELL_FUN(	spell_polymorph		);
DECLARE_SPELL_FUN(	spell_contraception	);
DECLARE_SPELL_FUN(	spell_find_familiar	);
DECLARE_SPELL_FUN(	spell_improve		);
DECLARE_SPELL_FUN(      spell_clay              );
DECLARE_SPELL_FUN( spell_aid );
DECLARE_SPELL_FUN( spell_drow_bless );
DECLARE_SPELL_FUN( spell_blur );
DECLARE_SPELL_FUN( spell_ckill );
DECLARE_SPELL_FUN( spell_dpoison );
DECLARE_SPELL_FUN( spell_fblade );
DECLARE_SPELL_FUN( spell_fshield );
DECLARE_SPELL_FUN( spell_idust );
DECLARE_SPELL_FUN( spell_invun );
DECLARE_SPELL_FUN( spell_lcharm );
DECLARE_SPELL_FUN( spell_lolthst );
DECLARE_SPELL_FUN( spell_lshield );
DECLARE_SPELL_FUN( spell_lwrath );
DECLARE_SPELL_FUN( spell_prayer );
DECLARE_SPELL_FUN( spell_prot );
DECLARE_SPELL_FUN( spell_shammer );
DECLARE_SPELL_FUN( spell_shille );
DECLARE_SPELL_FUN( spell_vtouch );
DECLARE_SPELL_FUN( spell_false_pride );
DECLARE_SPELL_FUN( spell_damnation );
DECLARE_SPELL_FUN( spell_fear );
DECLARE_SPELL_FUN( spell_rose_storm );
DECLARE_SPELL_FUN( spell_regeneration );
DECLARE_SPELL_FUN( spell_last_prayer );

/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(_AIX)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(apollo)
int	atoi		args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(interactive)
#endif

#if	defined(linux)
char *	crypt		args( ( const char *key, const char *salt
) );
#endif

#if	defined(macintosh) || defined(WIN32)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *	crypt		args( ( const char *key, const char *salt
) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
#if 	defined(SYSV)
size_t 	fread		args( ( void *ptr, size_t size, size_t n, 
				FILE *stream ) );
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif



/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)

#define PLAYER_DIR	""		/* Player files			*/
#define NULL_FILE	"proto.are"	/* To reserve one stream	*/
#define BACKUP_DIR      "" /*reimburse shit -Infidel*/
#endif

#if defined(MSDOS)
#define PLAYER_DIR	""		/* Player files                 */
#define NULL_FILE	"nul"		/* To reserve one stream	*/
#define BAN_FILE	      ""
#define LAST_COMMAND    ""  /*For the signal handler.*/
#endif

#if defined( WIN32 )
#define PLAYER_DIR       "..\\player\\"	/* Player files                 */
#define NULL_FILE        "nul"		/* To reserve one stream        */
#define BACKUP_DIR	"..\\backup\\"
#define LAST_COMMAND    ""  /*For the signal handler.*/
#endif

#if defined(unix)
#define LAST_COMMAND    "../last_command.txt"  /*For the signal handler.*/
#define PLAYER_DIR	"../player/"	/* Player files			*/
#define BACKUP_DIR      "../player/backup/" /*reimb shit*/
#define BAN_FILE  	"../area/ban.txt"	/* Player files			*/

#define NULL_FILE	"/dev/null"	/* To reserve one stream	*/
#endif

#define AREA_LIST	"area.lst"	/* List of areas		*/
#define CLAN_LIST	"clan1.txt"	/* List of clans		*/
#define ART_LIST	"art1.txt"	/* List of artifacts            */
#define HOME_AREA	"../area/homes.are" /* Mages towers, etc        */
#define BAN_LIST	"../area/ban.txt"	/* baaan. */
#define BUG_FILE	"bugs.txt"      /* For 'bug' and bug( )		*/
#define IDEA_FILE	"ideas.txt"	/* For 'idea'			*/
#define TYPO_FILE	"typos.txt"     /* For 'typo'			*/
#define NOTE_FILE	"notes.txt"	/* For 'notes'			*/
#define SHUTDOWN_FILE	"shutdown.txt"	/* For 'shutdown'		*/
#define QUEST_TXT	"quest.txt"     /* Show quests hehe whee	*/

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define ED	EXIT_DATA

/* monk2.c */
void aura_add args ( ( CHAR_DATA *ch, int ginpart ) );
void aura_remove args( ( CHAR_DATA *ch ) );

/* kingdom.c */
void remove_member args ( ( CHAR_DATA *ch ) );
void add_member args ( ( CHAR_DATA *ch ) );
void start_scavenger args( ( void ) );
void stop_scavenger args( ( void ) );


/* act_comm.c */
bool in_kingdom         args( ( CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *pRoomIndex ) );
bool is_eye_scouter     args( ( CHAR_DATA *ch ) );
char *talk_garbage      args( ( CHAR_DATA *ch, char *argument ) );
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void	room_text	args( ( CHAR_DATA *ch, char *argument ) );
char    *strlower       args( ( char * ip ) );
void	excessive_cpu	args( ( int blx ) );
bool	check_parse_name args( ( char *name ) );
bool is_unique_name    args( ( DESCRIPTOR_DATA *d, char *argument ) );

void    room_message    args( ( ROOM_INDEX_DATA *room, char *message ) );

/* act_info.c */
char * get_stars        args( ( int num, int max_num ) );
char * show_fbar        args( ( int num, int max_num ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
int	char_hitroll	args( ( CHAR_DATA *ch ) );
int	char_damroll	args( ( CHAR_DATA *ch ) );
int	char_damcap  	args( ( CHAR_DATA *ch ) );
int	char_ac		args( ( CHAR_DATA *ch ) );

/* darkheart.c */

MID   * get_random_mob	args( ( ) );
bool    is_demonic_form  args( ( int form ) );
char  * dragon_colour   args( (sh_int dragtype) );
char  * dragon_colour2   args( (sh_int dragtype) );
int     get_size	args( (CHAR_DATA *ch) );
void    set_enter_room_text args( ( CHAR_DATA *ch, char *argument ) );
void    set_form        args( ( CHAR_DATA *ch , sh_int form ) );
void    look_name	args( ( CHAR_DATA *ch ) );
void    set_pc_name     args( ( CHAR_DATA *ch, char *title ) );
void    set_switchname  args( ( CHAR_DATA *ch, char *title ) );
char *  get_who_race    args( ( CHAR_DATA *ch, CHAR_DATA *wch) );
bool    is_clan		args( ( CHAR_DATA *ch ) );
void    form_in_room_desc args( ( CHAR_DATA *ch, char *buf ) );
bool    is_vowel	args( ( char arg ) );
int     get_normal_form   args( (CHAR_DATA *ch) );

/* act_move.c */
bool is_memb      args( ( CHAR_DATA *ch ) );
void	move_char	args( ( CHAR_DATA *ch, int door ) );
void	open_lift	args( ( CHAR_DATA *ch ) );
void	close_lift	args( ( CHAR_DATA *ch ) );
void	move_lift	args( ( CHAR_DATA *ch, int to_room ) );
void	move_door	args( ( CHAR_DATA *ch ) );
void	thru_door	args( ( CHAR_DATA *ch, int doorexit ) );
void	open_door	args( ( CHAR_DATA *ch, bool be_open ) );
bool	is_open		args( ( CHAR_DATA *ch ) );
bool	same_floor	args( ( CHAR_DATA *ch, int cmp_room ) );
void	add_tracks	args( ( CHAR_DATA *ch, int direction ) );
int check_hunt args( ( CHAR_DATA *ch ) );
void check_traps  args( ( CHAR_DATA *ch ) );

int   disc_points_needed args( (CHAR_DATA *ch) );
void  gain_disc_points  args( (CHAR_DATA *ch, int points ) );
void  get_random_room   args ( (CHAR_DATA *ch) );
RID  *get_rand_room   args ( ( ) );
RID  *get_treemeld_room   args ( ( ) );
RID  *get_rand_room_by_sect   args ( (int sect ) );


/* act_obj.c */
bool	is_ok_to_wear	args( ( CHAR_DATA *ch, bool wolf_ok, char *argument ) );
void	quest_object	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	remove_obj	args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );

/* ban.c */
bool check_ban args( (char *site,int type) );

/* act_wiz.c */
void   	get_reboot_string args( ( void ) );
void	bind_char	args( ( CHAR_DATA *ch ) );
void	logchan		args( ( char *argument ) );
bool is_sound args( ( CHAR_DATA *ch ) );

/* build.c */
ED *	get_exit	args( ( ROOM_INDEX_DATA *room, sh_int dir ) );
ED *	get_exit_to	args( ( ROOM_INDEX_DATA *room, sh_int dir,int vnum ) );
ED *	get_exit_num	args( ( ROOM_INDEX_DATA *room, sh_int count ) );

char *	copy_buffer	args( ( CHAR_DATA *ch ) );
void	edit_buffer	args( ( CHAR_DATA *ch, char *argument ) );
char *	strip_cr	args( ( char *str  ) );
void    start_editing   args( ( CHAR_DATA *ch, char *data ) );  
void    stop_editing    args( ( CHAR_DATA *ch ) );

/* comm.c */
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	close_socket2	args( ( DESCRIPTOR_DATA *dclose, bool kickoff ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	act2		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	kavitem		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );

void    banner_to_char  args( ( char *txt, CHAR_DATA *ch ) );
void    banner2_to_char args( ( char *txt, CHAR_DATA *ch ) );
void    divide_to_char  args( ( CHAR_DATA *ch ) );
void    divide2_to_char args( ( CHAR_DATA *ch ) );
void    divide3_to_char args( ( CHAR_DATA *ch ) );
void    divide4_to_char args( ( CHAR_DATA *ch ) );
void    divide5_to_char args( ( CHAR_DATA *ch ) );
void    divide6_to_char args( ( CHAR_DATA *ch ) );
void    sta         args( (const char *txt,AREA_DATA *area) );
void    stc	        args( ( const char *txt, CHAR_DATA *ch ) );
void    cent_to_char    args( ( char *txt, CHAR_DATA *ch ) );
void    check_multi args( ( CHAR_DATA *ch ) );

#define DISABLED_FILE "disabled.txt"  /* disabled commands */

/* prototypes from db.c */
void  load_disabled   args( ( void ) );
void  save_disabled   args( ( void ) );

/* db.c */
int strlen2 args( ( const char *s ) );
void	boot_db		args( (bool fCopyOver ) );
void	area_update	args( ( void ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
void	free_char	args( ( CHAR_DATA *ch ) );
char *	get_extra_descr	args( ( char *name, EXTRA_DESCR_DATA *ed ) );
char *	get_roomtext	args( ( const char *name, ROOMTEXT_DATA    *rt ) );
MID *	get_mob_index	args( ( int vnum ) );
OID *	get_obj_index	args( ( int vnum ) );
RID *	get_room_index	args( ( int vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
void	free_mem	args( ( void *pMem, int sMem ) );
char *	str_dup		args( ( const char *str ) );
void	free_string	args( ( char *pstr ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
int	number_mm	args( ( void ) );
int	dice		args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
void	smash_tilde	args( ( char *str ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void	bug		args( ( const char *str, int param ) );
void	log_string	args( ( const char *str ) );
void	tail_chain	args( ( void ) );

void	add_help	args( ( HELP_DATA *pHelp ) );
char *	strupper	args( ( const char *str ) );
/*
RID *	make_room	args( ( int vnum ) );
OID *	make_object	args( ( int vnum, int cvnum, char *name ) );
MID *	make_mobile	args( ( int vnum, int cvnum, char *name ) );
ED  *	make_exit	args( ( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, int door ) );
*/

/* daemon.c */

ROOM_INDEX_DATA * locate_obj	args ( ( OBJ_DATA *obj ) );
void shock_effect  args( ( void *vo,int level, int dam, int target ) );
void cold_effect   args( ( void *vo,int level, int dam, int target ) );
void acid_effect   args( ( void *vo,int level, int dam, int target ) );
void fire_effect   args( ( void *vo,int level, int dam, int target ) );
void make_wall     args( ( ROOM_INDEX_DATA *room, int dir, int wall ) );

/* special.c */
void capture_person args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool is_same_kingdom args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int get_kingdom_num args( ( CHAR_DATA *ch ) );

/* mccp.c */
bool compressStart(DESCRIPTOR_DATA *desc);
bool compressEnd(DESCRIPTOR_DATA *desc);
bool processCompressed(DESCRIPTOR_DATA *desc);
bool writeCompressed(DESCRIPTOR_DATA *desc, char *txt, int length);


/* kuruda.c */
void do_psychic_rush    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void do_air_blast       args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void perform_aragami    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void perform_zanretsuken args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void perform_shiki      args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void perform_final_heaven args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void continue_sbomb     args( ( CHAR_DATA *ch ) );
void do_kuruda_laugh    args( ( CHAR_DATA *ch ) );

/* highwind.c */
void update_jumping     args( ( CHAR_DATA *ch ) );
void lloth_curse        args( ( CHAR_DATA *ch ) );
void lloth_bless        args( ( CHAR_DATA *ch ) );


/* monk.c */
void do_monkdisarm args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void start_innocence args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void perform_innocence args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void special_monkslam args( ( CHAR_DATA *ch, int direction ) );
void show_trance args( ( CHAR_DATA *ch ) );

/* kingdom.c */
void do_update_class_count args( ( int class, bool add ) );

/* mage.c */
void mage_damage        args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type ) );
void mage_message       args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type ) );
int  mage_immunity      args( ( CHAR_DATA *ch, int dam, const char *msg, int type ) );
void add_quint          args( ( CHAR_DATA *ch, int number ) );
bool is_memb		args( ( CHAR_DATA *ch ) );
bool is_mage		args( ( CHAR_DATA *ch ) );
void show_page          args( ( CHAR_DATA *ch, OBJ_DATA *book, int pnum, bool pagefalse ) );
void show_runes         args( ( CHAR_DATA *ch, OBJ_DATA *page, bool endline ) );

/* torunament.c */
void announce_fight     args( ( CHAR_DATA *ch ) );
void announce_round     args( ( CHAR_DATA *ch ) );
void announce_match     args( ( void ) );
void do_initialize_tournament args( ( CHAR_DATA *ch ) );
void show_victory_pose  args( ( CHAR_DATA *ch ) );
void show_winner        args( ( CHAR_DATA *champion ) );
void check_arena        args( ( CHAR_DATA *ch ) );
void do_knight args( ( CHAR_DATA *ch, char *argument ) );
void do_killer args( ( CHAR_DATA *ch, char *argument ) );
void do_ryo args( ( CHAR_DATA *ch, char *argument ) );
void do_kyo args( ( CHAR_DATA *ch, char *argument ) );
void do_zero args( ( CHAR_DATA *ch, char *argument ) );
void do_andy args( ( CHAR_DATA *ch, char *argument ) );
void do_terry args( ( CHAR_DATA *ch, char *argument ) );
void do_andy args( ( CHAR_DATA *ch, char *argument ) );
void do_kk args( ( CHAR_DATA *ch, char *argument ) );
void do_sdemon args( ( CHAR_DATA *ch, char *argument ) );
void do_guardian args( ( CHAR_DATA *ch, char *argument ) );
void do_joe args( ( CHAR_DATA *ch, char *argument ) );
void do_soldier args( ( CHAR_DATA *ch, char *argument ) );
void do_drow args( ( CHAR_DATA *ch, char *argument ) );
void do_angel args( ( CHAR_DATA *ch, char *argument ) );
void do_sparda args( ( CHAR_DATA *ch, char *argument ) );
void do_vampire args( ( CHAR_DATA *ch, char *argument ) );
void do_ninja args( ( CHAR_DATA *ch, char *argument ) );
void do_krizalid args( ( CHAR_DATA *ch, char *argument ) );
void do_mob_counter args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void do_tenchi args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* aura.c */
void give_random_aura   args( ( CHAR_DATA *ch ) );

/* aunction.c */
void free_auction args( ( void ) );
void auction_progess args( ( void ) );
void remove_auction args( ( CHAR_DATA *ch ) );
void remove_auction_o args( ( OBJ_DATA *obj ) );

int get_translevel args( ( CHAR_DATA *ch ) );
void add_transmigration args( ( CHAR_DATA *ch, int value ) );


/* team.c */
void remove_team_player args( ( CHAR_DATA *ch, int team ) );
void show_poses args( ( CHAR_DATA *ch ) );
void show_round args( ( CHAR_DATA *ch ) );
void show_start args( ( CHAR_DATA *ch ) );
void show_intro args( ( void ) );
void remove_team args( ( void ) );
void check_round args( ( CHAR_DATA *winner, CHAR_DATA *loser ) );
void show_power_meter args( ( CHAR_DATA *ch ) );
void finish_round args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );

/* powers.c */
bool can_do_skill       args( ( CHAR_DATA *ch, int num ) );

/* fight.c */
void do_jaggar          args( ( CHAR_DATA *ch ) );
void do_altima          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void do_eipheal         args( ( int nocheck ) );
void do_conan           args( ( int nocheck ) );
void add_grove          args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
bool is_same_plane      args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int is_elemental        args( ( int ele_attack, CHAR_DATA *victim, int dam ) );
bool is_martial         args( ( CHAR_DATA *ch ) );
char * show_style       args( ( CHAR_DATA *ch ) );
char * show_groove      args( ( CHAR_DATA *ch ) );
void subtract_stock     args( ( CHAR_DATA *ch, int dam ) );
int get_stock           args( ( CHAR_DATA *ch ) );
int devil_luck          args( ( CHAR_DATA *ch, CHAR_DATA *victim, int num ) );
void clear_affects      args( ( CHAR_DATA *ch ) );
void restore_char       args( ( CHAR_DATA *ch ) );
void  dam_message       args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void clean_arena        args( ( CHAR_DATA *loser, CHAR_DATA *winner ) );
void do_tinfo           args( ( CHAR_DATA *ch, CHAR_DATA *victim, char *argument ) );
void undo_arena         args( ( CHAR_DATA *ch ) );
int cap_dam             args( ( CD *ch, CD *victim ) );
int randomize_damage    args( ( CHAR_DATA *ch, int dam, int am ) );
int sorceror_sword      args( ( CHAR_DATA *ch ) );
int sorceror_magic      args( ( CHAR_DATA *ch ) );
void give_bm_affect     args( ( CHAR_DATA *ch ) );
bool can_heal           args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void show_total_damage  args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool is_oring           args( ( CHAR_DATA *ch ) );
void divine_strike      args( ( CHAR_DATA *ch ) );
void update_timers      args( ( CHAR_DATA *ch ) );
void show_sina_combo    args( ( CHAR_DATA *ch ) );

int is_attack_type      args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void	violence_update	args( ( void ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void  stop_embrace    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	hurt_person	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool agg_dam ) );
void  set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
void  apply_cpush    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  give_align     args( ( CHAR_DATA *ch, int number ) );
bool  has_timer	args( ( CHAR_DATA *ch ) );
bool  has_bad_chars   args( ( CHAR_DATA *ch, char *argument ) );
void  check_leaderboard  args( ( CHAR_DATA *ch, char *argument ) );
void  do_autoguard    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  perform_trigger args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  perform_solkatti args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  perform_tackle args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  perform_geyser args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  perform_arc args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void  perform_charge args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* dungeon.c */
void create_dungeon args( ( void ) );
void clear_dungeon args( ( void ) );
void check_boss args( ( CHAR_DATA *ch ) );
void check_level args( ( CHAR_DATA *ch ) );

/* update.c */
void show_skill   args( ( CHAR_DATA *ch, int num ) );
void find_item    args( ( CHAR_DATA *ch, int obj_level ) );

void	hit_gain	args( ( CHAR_DATA *ch, int heal ) );
void	mana_gain	args( ( CHAR_DATA *ch, int heal ) );
void	move_gain	args( ( CHAR_DATA *ch, int heal ) );


/* handler.c */

void	extract_exit	args( ( ROOM_INDEX_DATA *room, EXIT_DATA *pexit ) );

int   get_fire_prof args( ( CHAR_DATA *ch ) );
int   get_natural_prof args( ( CHAR_DATA *ch ) );
int   get_ice_prof args( ( CHAR_DATA *ch ) );
int   get_thunder_prof args( ( CHAR_DATA *ch ) );
int   get_lunar_prof args( ( CHAR_DATA *ch ) );
int   get_earth_prof args( ( CHAR_DATA *ch ) );
int   get_holy_prof args( ( CHAR_DATA *ch ) );
int   get_dark_prof args( ( CHAR_DATA *ch ) );
int   get_physical_prof args( ( CHAR_DATA *ch ) );
int   get_magical_prof args( ( CHAR_DATA *ch ) );
int   get_slash_prof args( ( CHAR_DATA *ch ) );
int   get_pierce_prof args( ( CHAR_DATA *ch ) );
int   get_blast_prof args( ( CHAR_DATA *ch ) );
int   get_beam_prof args( ( CHAR_DATA *ch ) );
int   get_blunt_prof args( ( CHAR_DATA *ch ) );
int   get_spirit_prof args( ( CHAR_DATA *ch ) );

int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_age		args( ( CHAR_DATA *ch ) );
int	get_curr_str	args( ( CHAR_DATA *ch ) );
int	get_curr_int	args( ( CHAR_DATA *ch ) );
int	get_curr_eva	args( ( CHAR_DATA *ch ) );
int	get_curr_luck	args( ( CHAR_DATA *ch ) );
int	get_curr_tens	args( ( CHAR_DATA *ch ) );
int	get_curr_agi	args( ( CHAR_DATA *ch ) );
int	get_curr_wis	args( ( CHAR_DATA *ch ) );
int	get_curr_dex	args( ( CHAR_DATA *ch ) );
int	get_curr_con	args( ( CHAR_DATA *ch ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( char *str, char *namelist ) );
bool	is_full_name	args( ( const char *str, char *namelist ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
CD *  get_mob           args( ( int vnum ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *  get_your_name     args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_area	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_in_obj	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_room	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	create_money	args( ( int amount ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *	item_type_name	args( ( OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *	extra_bit_name	args( ( int extra_flags ) );
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, OBJ_DATA *obj ) );


void    set_learnable_disciplines args( ( CHAR_DATA *ch ) );
void    update_disc	args( ( CHAR_DATA *ch) );

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
void	stage_update	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int stage, char *argument ) );
void    make_preg       args( ( CHAR_DATA *mother, CHAR_DATA *father ) );

/* magic.c */
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );
void	enhance_stat	args( ( int sn, int level, CHAR_DATA *ch, 
				    CHAR_DATA *victim, int apply_bit,
				    int bonuses, int affect_bit ) );

/* stat_sav.c */

void    initialise_config	args( ( void ) );
int check_class_max args ( ( CHAR_DATA *ch, int number ) );

/* save.c */
void	save_char_obj		args( ( CHAR_DATA *ch ) );
void	save_char_obj_backup	args( ( CHAR_DATA *ch ) );
bool	load_char_obj		args( ( DESCRIPTOR_DATA *d, char *name ) );
bool	load_char_short		args( ( DESCRIPTOR_DATA *d, char *name ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );

/* update.c */
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );
void    mobile_update   args( ( void ) );
void    weather_update  args( ( void ) );
void    char_update     args( ( void ) );
void    obj_update      args( ( void ) );
void    aggr_update     args( ( void ) );
void    ww_update       args( ( void ) );
void    parse_who       args( ( void ) );

void    room_update	args( ( void ) );

/* kav_fight.c */
void	special_move	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* kav_info.c */
void	birth_date	args( ( CHAR_DATA *ch, bool is_self ) );
void	other_age	args( ( CHAR_DATA *ch, int extra, bool is_preg, 
				char *argument ) );
void centre_text  args( ( char *text, CHAR_DATA *ch ) );
int	years_old	args( ( CHAR_DATA *ch ) );

/* kav_wiz.c */
void oset_affect args( ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest) );

/* clan.c */
void	reg_mend	args( ( CHAR_DATA *ch ) );
void	vamp_rage	args( ( CHAR_DATA *ch ) );
bool	char_exists	args( ( bool backup, char *argument ) );
OD *	get_page	args( ( OBJ_DATA *book, int page_num ) );

/* vic.c */
DECLARE_DO_FUN( do_relevel2 );
void reset_weapon   args( (CHAR_DATA *ch, int dtype) );
void reset_spell    args( (CHAR_DATA *ch, int dtype) );

/* update.c */
void beware_lightning args( (void) ); 


#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef  ED

/*****************************************************************************
 *                                    OLC                                    *
 *****************************************************************************/

/*
 * This structure is used in special.c to lookup spec funcs and
 * also in olc_act.c to display listings of spec funcs.
 */
struct spec_type
{
    char *	spec_name;
    SPEC_FUN *	spec_fun;
};



/*
 * This structure is used in bit.c to lookup flags and stats.
 */
struct flag_type
{
    char * name;
    int  bit;
    bool settable;
};



/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
#define OBJ_VNUM_DUMMY	1



/*
 * Area flags.
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define 	AREA_VERBOSE	8


#define MAX_DIR	6
#define NO_FLAG -99	/* Must not be used in flags or stats. */



/*
 * Interp.c
 */
DECLARE_DO_FUN( do_aedit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_redit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_oedit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_medit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_asave	);
DECLARE_DO_FUN( do_alist	);
DECLARE_DO_FUN( do_resets	);


/*
 * Global Constants
 */
extern	char *	const	dir_name        [];
extern	const	sh_int	rev_dir         [];
extern	const	struct	spec_type	spec_table	[];



/*
 * Global variables
 */

extern		int			nAllocPerm;
extern		int			nAllocString;

extern          AREA_DATA *             area_first;
extern          AREA_DATA *             area_last;
extern  	SHOP_DATA *             shop_last;

extern          int                     top_affect;
extern          int                     top_area;
extern          int                     top_ed;
extern          int                     top_exit;
extern          int                     top_help;
extern          int                     top_mob_index;
extern          int                     top_obj_index;
extern          int                     top_reset;
extern          int                     top_room;
extern          int                     top_shop;

extern          int                     top_vnum_mob;
extern          int                     top_vnum_obj;
extern          int                     top_vnum_room;

extern          char                    str_empty       [1];

extern  MOB_INDEX_DATA *        mob_index_hash  [MAX_KEY_HASH];
extern  OBJ_INDEX_DATA *        obj_index_hash  [MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *       room_index_hash [MAX_KEY_HASH];



/* db.c */
void	reset_area      args( ( AREA_DATA * pArea ) );
void	reset_room	args( ( ROOM_INDEX_DATA *pRoom ) );

/* string.c */
void	string_edit	args( ( CHAR_DATA *ch, char **pString ) );
void    string_append   args( ( CHAR_DATA *ch, char **pString ) );
char *	string_replace	args( ( char * orig, char * old, char * new ) );
void    string_add      args( ( CHAR_DATA *ch, char *argument ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );
char *	string_unpad	args( ( char * argument ) );
char *	string_proper	args( ( char * argument ) );
char * all_capitalize	args( ( const char *str) );

/* olc.c */
bool	run_olc_editor	args( ( DESCRIPTOR_DATA *d ) );
char	*olc_ed_name	args( ( CHAR_DATA *ch ) );
char	*olc_ed_vnum	args( ( CHAR_DATA *ch ) );

/* special.c */
char *	spec_string	args( ( SPEC_FUN *fun ) );	/* OLC */


/* bit.c */
extern const struct flag_type 	area_flags[];
extern const struct flag_type	sex_flags[];
extern const struct flag_type	exit_flags[];
extern const struct flag_type	door_resets[];
extern const struct flag_type	room_flags[];
extern const struct flag_type	sector_flags[];
extern const struct flag_type	type_flags[];
extern const struct flag_type	extra_flags[];
extern const struct flag_type	wear_flags[];
extern const struct flag_type	act_flags[];
extern const struct flag_type	affect_flags[];
extern const struct flag_type	apply_flags[];
extern const struct flag_type	wear_loc_strings[];
extern const struct flag_type	wear_loc_flags[];
extern const struct flag_type	weapon_flags[];
extern const struct flag_type	container_flags[];
extern const struct flag_type	liquid_flags[];


/*
 * Bits for dragons, their 25 skills
 */
#define DRA_WINGS			1
#define DRA_FANGS			2
#define DRA_LICK			4
#define DRA_FROST_BREATH		8
#define DRA_FIRE_BREATH			16
#define DRA_LIGHTNING_BREATH		32
#define DRA_ACID_BREATH			64
#define DRA_GAS_BREATH			128
#define DRA_TAIL			256
#define DRA_TERROR			512
#define DRA_CONFUSE			1024
#define DRA_MASK			2048
#define DRA_VENOM			4096
#define DRA_MAGIC			8192
#define DRA_POLYMORPH			16384
#define DRA_LUCK			32768
#define DRA_FLY				65536
#define DRA_SCRY			131072
#define DRA_PEER			262144
#define DRA_TRUESIGHT			524288
#define DRA_TOUGHNESS			1048576	


/* NOTE: This file only includes two examples! write your own! */

struct slay_type
{
  char *        owner;		/* only this player can use this option */
  char *        title;		/* each one must have a unique title!   */
  char *        char_msg;	/* is sent to the "slayer"		*/
  char *        vict_msg;	/* is sent to the "slayee" (poor sucker)*/
  char *        room_msg;	/* is sent to everyone else in the room */
};

/* I am lazy :) */
#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH

void copyover_recover args((void));

DECLARE_DO_FUN( do_implag	);
DECLARE_DO_FUN( do_doublexp	);
