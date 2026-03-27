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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h> /* unlink() */
#endif
#include "merc.h"
#include "garou.h"

char    last_command[MAX_STRING_LENGTH];

bool	check_social  args( ( CHAR_DATA *ch, char *command, char *argument ) );
bool	check_xsocial args( ( CHAR_DATA *ch, char *command, char *argument ) );
void	make_preg     args( ( CHAR_DATA *mother, CHAR_DATA *father ) );

void do_klaive( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS( ch, CLASS_GAROU ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( 1 > ch->gold )
      {
         send_to_char("It costs #w6 #ygold piece#n to create a Lesser Klaive.\n\r",ch);
         return;
      }

   ch->gold   -= 1;
   obj = create_object(get_obj_index(353)  ,0 );
   SET_BIT(obj->quest, QUEST_RELIC);
   obj->questowner = str_dup(ch->pcdata->switchname);
   obj_to_char(obj, ch);
   act("A Lesser Klaive appears in your hands in a flash of light.",ch,NULL,NULL,TO_CHAR);
   act("A Lesser Klaive appears in $n's hands in a flash of light.",ch,NULL,NULL,TO_ROOM);
}

void do_katana( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( 1 > ch->gold )
      {
         send_to_char("It costs #w1 #ygold piece#n to create a Lightsaber.\n\r",ch);
         return;
      }

   ch->gold -= 1;
   obj = create_object( get_obj_index(354), 50 );
   SET_BIT( obj->quest, QUEST_RELIC );
   obj->questowner = str_dup( ch->pcdata->switchname );
   obj_to_char( obj, ch );
   act("A lightsaber flashes into existance.",ch,NULL,NULL,TO_CHAR);
   act("A lightsaber suddenly appears in $n's hands.",ch,NULL,NULL,TO_ROOM);
}

/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2

/*
 * Log-all switch.
 */
bool	fLogAll         = FALSE;
bool	global_exp      = FALSE;
bool 	global_qp       = FALSE;
bool  extra_log	    = FALSE;
int   players_logged  = 0;
int   players_decap   = 0;
int   players_gstolen = 0;

bool  check_disabled ( const struct cmd_type *command );
DISABLED_DATA *disabled_first;

#define END_MARKER    "END"

/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    /* Name             Function           Min Position     Level  Log */

    { "north",          do_north,          POS_STANDING,     0,  LOG_NORMAL },
    { "east",           do_east,           POS_STANDING,	 0,  LOG_NORMAL },
    { "south",          do_south,          POS_STANDING,	 0,  LOG_NORMAL },
    { "west",           do_west,           POS_STANDING,	 0,  LOG_NORMAL },
    { "up",             do_up,             POS_STANDING,	 0,  LOG_NORMAL },
    { "down",           do_down,           POS_STANDING,	 0,  LOG_NORMAL },

    { "transfer",       do_transfer,       POS_DEAD,         7,  LOG_ALWAYS },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "specialbuy",     do_huntercreate,   POS_FIGHTING,	 0,  LOG_NORMAL },
    { "auction",		do_auction,        POS_SLEEPING,	 0,  LOG_NORMAL },
    { "bid",		do_bid,            POS_SLEEPING,	 0,  LOG_NORMAL },
    { "cast",		do_cast,           POS_FIGHTING,	 0,  LOG_NORMAL },
    { "call",		do_call,	       POS_SLEEPING,	 0,  LOG_NORMAL },
    { "class",          do_classself,      POS_STANDING,     7,  LOG_NEVER  },
    { "consider",		do_consider,       POS_SITTING,	 0,  LOG_NORMAL },
    { "diagnose",	      do_diagnose,       POS_FIGHTING,	 0,  LOG_NORMAL },
    { "dismount",	      do_dismount,       POS_STANDING,	 0,  LOG_NORMAL },
    { "enter",		do_enter,	       POS_STANDING,	 0,  LOG_NORMAL },
    { "exits",		do_exits,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "get",            do_get,            POS_SITTING,	 0,  LOG_NORMAL },
    { "follow",         do_follow,	       POS_STANDING,	 0,  LOG_NORMAL },
    { "group",		do_group,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "inventory",	do_inventory,      POS_DEAD,	 	 0,  LOG_NORMAL },
    { "kill",		do_kill,	       POS_FIGHTING,	 0,  LOG_NORMAL },
    { "look",		do_look,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "ls",			do_look,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "mount",		do_mount,	       POS_STANDING,	 0,  LOG_NORMAL },
    { "make",		do_make,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "order",		do_order,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "rest",		do_rest,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "sit",		do_sit,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "stand",		do_stand,	       POS_SLEEPING,	 0,  LOG_NORMAL },
    { "tail",           do_tail,           POS_RESTING,      0,  LOG_NORMAL },
    { "tell",		do_tell,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "version",		do_version,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "whisper",		do_whisper,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "wield",		do_wear,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "wizhelp",		do_wizhelp,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "where",		do_where,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "enquire",		do_enquire,        POS_SLEEPING,	 0,  LOG_NORMAL },

    /*
     * Informational commands.
     */
    { "affects",        do_affects,        POS_DEAD,         0,  LOG_NORMAL },
    { "areas",		do_areas,          POS_DEAD,	 	 0,  LOG_NORMAL },
    { "bug",		do_bug,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "credits",		do_credits,        POS_DEAD,	 	 0,  LOG_NORMAL },
    { "equipment",	do_equipment,      POS_DEAD,	 	 0,  LOG_NORMAL },
    { "examine",		do_examine,	       POS_SITTING,	 0,  LOG_NORMAL },
    { "exp",            do_level,          POS_FIGHTING,     0,  LOG_NORMAL },
    { "help",		do_help,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "idea",		do_idea,           POS_DEAD,	  	 0,  LOG_NORMAL },
    { "mudstat",	      do_mudstat,        POS_DEAD,         2,  LOG_NORMAL },
    { "reply",          do_reply,          POS_MEDITATING,   0,  LOG_NORMAL },
    { "report",		do_report,         POS_DEAD,	 	 0,  LOG_NORMAL },
    { "rename",		do_rename,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "score",		do_score,          POS_DEAD,	 	 0,  LOG_NORMAL },
    { "skill",		do_skill,          POS_MEDITATING,	 0,  LOG_NORMAL },
    { "socials",		do_socials,	       POS_DEAD,	       0,  LOG_NORMAL },
    { "spells",		do_spell,	       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "time",		do_time,           POS_DEAD,	       0,  LOG_NORMAL },
    { "typo",		do_typo,           POS_DEAD,	       0,  LOG_NORMAL },
    { "who",		do_who,            POS_DEAD,	       0,  LOG_NORMAL },
    { "wizlist",		do_wizlist,        POS_DEAD,	       0,  LOG_NORMAL },
    { "xemot",		do_huh,	       POS_DEAD,         1,  LOG_NORMAL },
    { "xemote",		do_xemote,         POS_SITTING,      1,  LOG_NORMAL },
    { "xsocial",		do_huh,            POS_DEAD,         1,  LOG_NORMAL },
    { "xsocials",		do_xsocials,       POS_DEAD,         1,  LOG_NORMAL },
	{ "powers",         do_powers,         POS_DEAD,         0,  LOG_NORMAL },

	/*
     * Configuration commands.
     */
    { "ansi",		do_ansi,           POS_DEAD,         0,  LOG_NORMAL },
    { "autoexit",		do_autoexit,       POS_DEAD,         0,  LOG_NORMAL },
    { "autoloot",		do_autoloot,       POS_DEAD,         0,  LOG_NORMAL },
    { "autosac",		do_autosac,        POS_DEAD,         0,  LOG_NORMAL },
    { "block",          do_block,          POS_FIGHTING,     0,  LOG_NORMAL },
    { "brief",		do_brief,          POS_DEAD,         0,  LOG_NORMAL },
    { "shortfight",     do_brief2,         POS_DEAD,         0,  LOG_NORMAL },
    { "channels",		do_channels,       POS_DEAD,         0,  LOG_NORMAL },
    { "compress",       do_compress,       POS_DEAD,         0,  LOG_NORMAL },
    { "comeon",         do_come_on,        POS_STANDING,     0,  LOG_NORMAL },
    { "config",		do_config,         POS_DEAD,         0,  LOG_NORMAL },
    { "cprompt",		do_cprompt,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "description",	do_description,    POS_DEAD,         0,  LOG_NORMAL },
    { "password",		do_password,       POS_DEAD,         0,  LOG_NEVER  },
    { "prompt",		do_prompt,         POS_DEAD,	 	 0,  LOG_NORMAL },
    { "title",		do_title,          POS_DEAD,         0,  LOG_NORMAL },

    /*
     * Communication commands.
     */
    { ".",              do_chat,           POS_DEAD,         2,  LOG_NORMAL },
    { ",",              do_xemote,         POS_SITTING,      0,  LOG_NORMAL },
    { ";",			do_gtell,          POS_DEAD,         0,  LOG_NORMAL },
    { "'",              do_say,            POS_MEDITATING,	 0,  LOG_NORMAL },
    { "amaztalk",       do_amaztalk,       POS_DEAD,         0,  LOG_NORMAL },
    { "board",          do_board,          POS_DEAD,         0,  LOG_NORMAL },
    { "chat",           do_chat,	       POS_DEAD,	 	 0,  LOG_NORMAL },
    { "windtalk",       do_clantalk,       POS_DEAD,         0,  LOG_NORMAL },
    { "communicate",    do_communicate,    POS_DEAD,	 	 0,  LOG_NORMAL },
    { "dragtalk",       do_dragtalk,       POS_DEAD,         1,  LOG_NORMAL },
    { "emote",          do_xemote,         POS_SITTING,      0,  LOG_NORMAL },
    { "gtell",		do_gtell,          POS_DEAD,         0,  LOG_NORMAL },
    { "hightalk",       do_hightalk,       POS_DEAD,         1,  LOG_NORMAL },
    { "howl",		do_howl,           POS_DEAD,         1,  LOG_NORMAL },
    { "hum",            do_monktalk,       POS_DEAD,         1,  LOG_NORMAL },
    { "ignore",         do_ignore,         POS_SITTING,      0,  LOG_NORMAL },
    { "knighttalk",     do_knighttalk,     POS_DEAD,         0,  LOG_NORMAL },
    { "ktalk",          do_ktalk,          POS_DEAD,         0,  LOG_NORMAL },
    { "magetalk",       do_magetalk,       POS_DEAD,         1,  LOG_NORMAL },
    { "miktalk",        do_miktalk,        POS_DEAD,         1,  LOG_NORMAL },
    { "music",		do_music,          POS_SLEEPING,     2,  LOG_NORMAL },
    { "note",		do_note,           POS_DEAD,         0,  LOG_NORMAL },
    { "praise",         do_paladintalk,    POS_DEAD,         1,  LOG_NORMAL },
    { "pray",           do_pray,           POS_MEDITATING,   1,  LOG_NORMAL },
    { "pose",           do_emote,          POS_SITTING,      0,  LOG_NORMAL },
    { "roar",           do_roar,           POS_DEAD,         1,  LOG_NORMAL },
    { "probe",          do_probe,          POS_DEAD,         1,  LOG_NORMAL },
    { "say",		do_say,            POS_MEDITATING,	 0,  LOG_NORMAL },
    { "sign",           do_sign,           POS_DEAD,         1,  LOG_NORMAL },
    { "shout",          do_shout,          POS_SITTING,      2,  LOG_NORMAL },
    { "talk",           do_talk,           POS_SITTING,      0,  LOG_NORMAL },
    { "unignore",       do_unignore,       POS_SITTING,      0,  LOG_NORMAL },
    { "vamptalk",       do_vamptalk,       POS_DEAD,         1,  LOG_NORMAL },
    { "yell",           do_yell,           POS_SITTING,      2,  LOG_NORMAL },

    /*
     * Object manipulation commands.
     */
    { "buy",            do_buy,            POS_SITTING,      0,  LOG_NORMAL },
    { "sell",           do_sell,           POS_SITTING,      0,  LOG_NORMAL },
    { "list",           do_list,           POS_SITTING,      0,  LOG_NORMAL },
    { "value",          do_value,          POS_SITTING,      0,  LOG_NORMAL },
    { "close",          do_close,          POS_SITTING,      0,  LOG_NORMAL },
    { "draw",           do_draw,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "drink",          do_drink,          POS_SITTING,      0,  LOG_NORMAL },
    { "drop",           do_drop,           POS_SITTING,      0,  LOG_NORMAL },
    { "eat",            do_eat,            POS_SITTING,      0,  LOG_NORMAL },
    { "give",           do_give,           POS_SITTING,      0,  LOG_NORMAL },
    { "gift",           do_gift,           POS_STANDING,     0,  LOG_NEVER  },
    { "hold",           do_wear,           POS_SITTING,      0,  LOG_NORMAL },
    { "junk",           do_sacrifice,      POS_STANDING,     3,  LOG_NORMAL },
    { "lock",           do_lock,           POS_SITTING,      0,  LOG_NORMAL },
    { "open",           do_open,           POS_SITTING,      0,  LOG_NORMAL },
    { "pick",           do_pick,           POS_SITTING,      0,  LOG_NORMAL },
    { "put",            do_put,            POS_SITTING,      0,  LOG_NORMAL },
    { "quaff",          do_quaff,          POS_SITTING,      0,  LOG_NORMAL },
    { "reload",         do_reload,         POS_STANDING,     0,  LOG_NORMAL },
    { "remove",         do_remove,         POS_SITTING,      0,  LOG_NORMAL },
    { "sacrifice",      do_sacrifice,      POS_SITTING,      1,  LOG_NORMAL },
    { "sheath",         do_sheath,         POS_FIGHTING,     0,  LOG_NORMAL },
    { "shoot",          do_shoot,          POS_FIGHTING,     0,  LOG_NORMAL },
    { "take",           do_get,            POS_SITTING,      0,  LOG_NORMAL },
    { "turn",           do_turn,           POS_SITTING,      0,  LOG_NORMAL },
    { "unload",         do_unload,         POS_STANDING,     0,  LOG_NORMAL },
    { "unlock",         do_unlock,         POS_SITTING,      0,  LOG_NORMAL },
    { "wear",           do_wear,           POS_SITTING,      0,  LOG_NORMAL },
    { "zap",            do_zap,            POS_SITTING,      0,  LOG_NORMAL },

    /* 
     * Other commands 
     */
    { "aura",           do_aura,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "autoclass",      do_autoclass,      POS_STANDING,     3,  LOG_NORMAL },
    { "reclass",        do_reclass,        POS_STANDING,     3,  LOG_NORMAL },
    { "backstab",       do_backstab,       POS_STANDING,     0,  LOG_NORMAL },
    { "berserk",        do_berserk,        POS_FIGHTING,     0,  LOG_NORMAL },
    { "bs",             do_backstab,       POS_STANDING,     0,  LOG_NORMAL },
    { "classcount",     do_class_count,    POS_STANDING,     0,  LOG_NORMAL },
    { "claws",          do_claws,          POS_FIGHTING,     0,  LOG_NORMAL },
    { "crack",          do_crack,          POS_FIGHTING,     0,  LOG_NORMAL },
    { "decapitate",     do_decapitate,     POS_FIGHTING,     0,  LOG_NORMAL },
    { "disarm",         do_disarm,         POS_FIGHTING,     0,  LOG_NORMAL },
    { "escape",         do_escape,         POS_DEAD,         0,  LOG_NORMAL },
    { "fangs",          do_fangs,          POS_FIGHTING,     0,  LOG_NORMAL },
    { "flee",           do_flee,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "hurl",           do_hurl,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "kick",           do_kick,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "majorquest",     do_majorenchant,   POS_RESTING,      0,  LOG_NORMAL },
    { "punch",          do_punch,          POS_STANDING,     0,  LOG_NORMAL },
    { "rescue",         do_rescue,         POS_FIGHTING,     0,  LOG_NORMAL },
    { "remort",         do_remort,         POS_STANDING,     3,  LOG_NORMAL },
    { "scry",           do_scry,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "shield",         do_shield,         POS_STANDING,     0,  LOG_NORMAL },
    { "tear",           do_decapitate,     POS_STANDING,     0,  LOG_NORMAL },
    { "truesight",      do_truesight,      POS_FIGHTING,     0,  LOG_NORMAL },

    /*
     * Miscellaneous commands.
     */
    { "accep",          do_huh,            POS_STANDING,     1,  LOG_NORMAL },
    { "accept",         do_accept,         POS_STANDING,     1,  LOG_NORMAL },
    { "agree",          do_arenaagree,	 POS_STANDING,     2,  LOG_NORMAL },
    { "arenawatch",     do_arena_watch,	 POS_MEDITATING,   2,  LOG_NORMAL },
    { "artifact",       do_artifact,       POS_STANDING,     0,  LOG_NORMAL },
    { "birth",          do_birth,          POS_STANDING,     1,  LOG_NORMAL },
    { "blindfold",      do_blindfold,      POS_STANDING,     0,  LOG_NORMAL },
    { "bet",            do_bet,            POS_SLEEPING,     2,  LOG_NORMAL },
    { "bounty",         do_bounty,         POS_STANDING,     0,  LOG_NORMAL },
    { "bountylist",     do_bountylist,     POS_STANDING,     0,  LOG_NORMAL },
    { "breaku",         do_huh,            POS_STANDING,     1,  LOG_NORMAL },
    { "breakup",        do_breakup,        POS_STANDING,     1,  LOG_NORMAL },
    { "bribe",          do_bribe,          POS_STANDING,     3,  LOG_NORMAL },
    { "challenge",      do_challenge,	 POS_STANDING,     2,  LOG_NORMAL },
    { "claim",          do_claim,          POS_STANDING,     0,  LOG_NORMAL },
    { "clearstats",     do_clearstats,     POS_STANDING,     0,  LOG_NORMAL },
    { "choose",         do_choose,         POS_STANDING,     1,  LOG_NORMAL },
    { "pelt",           do_pelt,           POS_FIGHTING,     1,  LOG_NORMAL },
    { "target",         do_assassin_focus,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "consen",         do_huh,            POS_STANDING,     1,  LOG_NORMAL },
    { "consent",        do_consent,        POS_STANDING,     1,  LOG_NORMAL },
    { "contraception",  do_contraception,  POS_DEAD,         0,  LOG_NORMAL },
    { "decline",        do_decline, 	 POS_STANDING,     2,  LOG_NORMAL },
    { "delet",          do_delet,          POS_DEAD,         0,  LOG_NORMAL },
    { "delete",         do_delete,         POS_DEAD,         0,  LOG_NORMAL },
    { "depositgold",    do_depositqps,     POS_DEAD,         3,  LOG_NORMAL },
    { "deto",           do_deto,           POS_DEAD,         3,  LOG_NORMAL },
    { "detox",          do_detox,          POS_DEAD,         3,  LOG_NORMAL },
    { "fade",           do_fade,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "finger",         do_finger,         POS_SITTING,      0,  LOG_NORMAL },
    { "fire",           do_amazon_shoot,   POS_FIGHTING,     0,  LOG_NORMAL },
    { "flex",           do_flex,           POS_SITTING,      0,  LOG_NORMAL },
    { "gag",            do_gag,            POS_STANDING,	 0,  LOG_NORMAL },
    { "giveup",         do_give_up,        POS_FIGHTING,     2,  LOG_NORMAL },
    { "gsocial",        do_gsocial,        POS_DEAD,         0,  LOG_NORMAL },
    { "hide",           do_hide,           POS_STANDING,     0,  LOG_NORMAL },
    { "home",           do_home,           POS_STANDING,     0,  LOG_NORMAL },
    { "immune",         do_immune,         POS_DEAD,         0,  LOG_NORMAL },
    { "kdeposit",       do_kdeposit,       POS_STANDING,     3,  LOG_NORMAL },
    { "leader",         do_leader,         POS_STANDING,     2,  LOG_NORMAL },
    { "locate",         do_locate,         POS_STANDING,     0,  LOG_NORMAL },
    { "nightsight",     do_nightsight,     POS_SITTING,      1,  LOG_NORMAL },
    { "nosummon",       do_nosummon,       POS_DEAD,         0,  LOG_NORMAL },
    { "pknow",          do_pknow,          POS_DEAD,         3,  LOG_NORMAL },
    { "propos",         do_huh,            POS_STANDING,     2,  LOG_NORMAL },
    { "propose",        do_propose,        POS_STANDING,     2,  LOG_NORMAL },
    { "qui",            do_qui,            POS_DEAD,         0,  LOG_NORMAL },
    { "quit",           do_quit,           POS_SLEEPING,     0,  LOG_NORMAL },
    { "recall",         do_recall,         POS_FIGHTING,     0,  LOG_NORMAL },
    { "rent",           do_rent,           POS_DEAD,         0,  LOG_NORMAL },
    { "safe",           do_safe,           POS_STANDING,     0,  LOG_NORMAL },
    { "save",           do_save,           POS_DEAD,         0,  LOG_NORMAL },
    { "scan",		do_scan,           POS_SITTING,      0,  LOG_NORMAL },
    { "scavenger",	do_scavenger,      POS_SITTING,      0,  LOG_NORMAL },
    { "setfatality",    do_setfatality,    POS_DEAD,         0,  LOG_ALWAYS },
    { "setfatality2",   do_setfatality2,   POS_DEAD,         0,  LOG_ALWAYS },
    { "setlogin",       do_setlogin,       POS_DEAD,         0,  LOG_ALWAYS },
    { "setlogout",      do_setlogout,      POS_DEAD,         0,  LOG_ALWAYS },
    { "settitle",       set_title,         POS_DEAD,         0,  LOG_NORMAL },
    { "shadowsight",    do_shadowsight,    POS_SITTING,      1,  LOG_NORMAL },
    { "skillbuy",       do_skillbuy,       POS_SLEEPING,     0,  LOG_NORMAL },
    { "sleep",          do_sleep,          POS_SLEEPING,     0,  LOG_NORMAL },
    { "smother",        do_smother,        POS_FIGHTING,     0,  LOG_NORMAL },
    { "sneak",          do_sneak,          POS_STANDING,     0,  LOG_NORMAL },
    { "sound",          do_sound,          POS_DEAD,         1,  LOG_NORMAL },
    { "split",          do_split,          POS_SITTING,      0,  LOG_NORMAL },
    { "spy",            do_spy,            POS_SITTING,      0,  LOG_NORMAL },
    { "steal",          do_steal,          POS_STANDING,     0,  LOG_NORMAL },
    { "superheal",	do_superheal,      POS_STANDING,     0,  LOG_NORMAL },
    { "team",           do_team,           POS_SLEEPING,     3,  LOG_NORMAL },
    { "tie",            do_tie,            POS_FIGHTING,     3,  LOG_NORMAL },
    { "track",          do_track,          POS_STANDING,     0,  LOG_NORMAL },
    { "train",          do_train,          POS_STANDING,     0,  LOG_NORMAL },
    { "transport",      do_transport,      POS_DEAD,         0,  LOG_NORMAL },
    { "travel",         do_travel,         POS_STANDING,     0,  LOG_NORMAL },
    { "untie",          do_untie,          POS_STANDING,     0,  LOG_NORMAL },
    { "upkeep",         do_upkeep,         POS_DEAD,	       1,  LOG_NORMAL },
    { "visible",        do_visible,        POS_SLEEPING,     0,  LOG_NORMAL },
    { "wake",           do_wake,           POS_SLEEPING,     0,  LOG_NORMAL },
    { "warp",           do_mystic_warp,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "web",            do_web,            POS_FIGHTING,     0,  LOG_NORMAL },
    { "withdraw",       do_withdraw,       POS_FIGHTING,     3,  LOG_NORMAL },
    { "withdrawgold",   do_withdrawqps,    POS_DEAD,         3,  LOG_NORMAL },
    { "dragon",         do_dragon,          POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Priest commands.
     */
    { "learn",          do_learn,           POS_MEDITATING,  0,  LOG_NORMAL },
    { "cloak",          do_monkcloak,       POS_MEDITATING,  0,  LOG_NORMAL },

    /*
     * Champion commands.
     */
    { "chitilak",       do_chitilak,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "deathtouch",     do_deathtouch,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "demmak",         do_demmak,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "divineray",      do_divineray,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "heal",           do_cure,            POS_STANDING,    3,  LOG_NORMAL },
    { "healingtouch",   do_healingtouch,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "subtletouch",    do_subtletouch,     POS_STANDING,    0,  LOG_NORMAL },

    { "kakeio",         do_kakeio,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "powershot",      do_power_shot,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "ragingsoul",     do_raging_soul,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "sigmarage",      do_sigma_rage,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "vampirefang",    do_vampire_fang,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "entice",         do_entice,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "riposte",        do_riposte,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "forestall",      do_forestall,       POS_FIGHTING,    0,  LOG_NORMAL },

    { "airblade",          do_air_blade,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "breaker",           do_breaker,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "fangbreakerblade",  do_fangbreaker_blade,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "justicestarfist",   do_justice_starfist,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "divineeradication", do_divine_eradication, POS_FIGHTING,    0,  LOG_NORMAL },

    { "secretfist",     do_secret_fist,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "stagger",        do_stagger,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "risingstorm",    do_rising_storm,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "heavenlypunch",  do_heavenly_punch,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "hishokenrush",   do_hishoken_rush,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "innocence",      do_innocence,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "emptyblow",      do_empty_blow,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "stance",         do_stance,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "twirl",          do_twirl,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "butterfly",      do_butterfly,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "swandive",       do_swan_dive,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "psychoball",     do_psycho_ball,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "psychorush",     do_psycho_rush,     POS_STANDING,    0,  LOG_NORMAL },
    { "palmstrike",     do_palm_strike,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "soulbomb",       do_soulbomb,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "finalheaven",    do_final_heaven,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "neckbreaker",    do_neck_breaker,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "dragonroar",     do_dragon_roar,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "blazingfury",    do_blazing_fury,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "haoukokouken",   do_kou_ken,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "flaminghands",   do_flaminghands,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "tigerrush",      do_tiger_rush,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "meteorbarret",   do_meteor_barret,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "pummel",         do_pummel,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "divinemight",    do_divine_might,    POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Shaolin Monk commands.
     */
    { "bladegrasp",     do_blade_grasp,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "elbow",          do_elbow_strike,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "focus",          do_focus,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "guidance",       do_guidance,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "handhamedo",     do_hand_hamedo,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "headbutt",       do_headbutt,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "jab",            do_jab,             POS_FIGHTING,    0,  LOG_NORMAL },
    { "knee",           do_knee,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "roundhouse",     do_roundhouse_kick, POS_FIGHTING,    0,  LOG_NORMAL },
    { "shin",           do_rising_shin,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "shiningsoul",    do_shining_soul,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "sidewinder",     do_sidewinder_kick, POS_FIGHTING,    0,  LOG_NORMAL },
    { "skid",           do_skid,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "sweep",          do_sweep,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "vulcanpinch",    do_vulcanpinch,     POS_FIGHTING,    0,  LOG_NORMAL },

	/*
	 * KOF Kuruda Commands.
	 */
	{ "khighkick",       do_highkick,        POS_FIGHTING,    0,  LOG_NORMAL },
	{ "kjumpkick",       do_jumpkick,        POS_FIGHTING,    0,  LOG_NORMAL },
	{ "ksnapkick",       do_snapkick,        POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Angel commands.
     */
    { "enlighten",      do_enlighten,       POS_RESTING,     0,  LOG_NORMAL },
    { "fheaven",        do_fist_heaven,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "fisttakedown",   do_fist_takedown,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "fold",           do_angel_fold,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "godpeace",       do_gpeace,          POS_STANDING,    0,  LOG_NORMAL },
    { "gshield",        do_gshield,         POS_STANDING,    0,  LOG_NORMAL },
    { "gspirit",        do_gspirit,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "holy",           do_holy,            POS_FIGHTING,    3,  LOG_NORMAL },
    { "holyball",       do_holy_ball,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "holyword",       do_holyword,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "hope",           do_hope,            POS_FIGHTING,    3,  LOG_NORMAL },
    { "godswrath",      do_wrathofgod,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "manashield",     do_mana_shield,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "martyr",         do_martyr,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "megastrike",     do_mega_strike,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "redemption",     do_redemption,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "smite",          do_angel_smite,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "spread",         do_angel_spread,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "trueform",       do_trueform,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "purification",   do_purification,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "heavenlycloak",  do_heavenly_cloak,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "divinejudgement",do_divine_judgement,POS_FIGHTING,    0,  LOG_NORMAL },
    { "seraphcurse",    do_seraph_curse,    POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Werewolf commands.
     */
    { "avalanche",      do_avalanche,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "luna",           do_luna,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "gaia",           do_gaia,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "lunarpower",     do_lunar_power,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "lunartouch",     do_lunar_touch,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "lunareclipse",   do_lunar_eclipse,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "barrage",        do_barrage,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "banshee",        do_banshee,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "calm",           do_calm,            POS_STANDING,    0,  LOG_NORMAL },
    { "cannibal",       do_cannibalspirit,  POS_STANDING,    0,  LOG_NORMAL },
    { "clapofthunder",  do_clapofthunder,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "clenched",       do_clenched,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "dazzle",         do_dazzle,          POS_STANDING,    0,  LOG_NORMAL },
    { "disquiet",       do_disquiet,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "gnaw",           do_gnaw,            POS_DEAD,        0,  LOG_NORMAL },
    { "invis",          do_invis,           POS_DEAD,        7,  LOG_NORMAL },
    { "lambent",        do_lambent,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "moonbeam",       do_moonbeam,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "paws",           do_paws,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "quicksand",      do_quicksand,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "quills",         do_quills,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "rage",           do_rage,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "razorclaws",     do_razorclaws,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "spiralmoon",     do_spiral_moon,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "vanish",         do_vanish,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "waste",          do_waste,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "whelp",          do_whelp,           POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Vampire commands.
     */
    { "assassinate",    do_assassinate,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "change",         do_change,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "drainlife",      do_drain,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "embrace",        do_embrace,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "flash",          do_flash,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "infirmity",      do_infirmity,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "inherit",        do_inherit,         POS_RESTING,     0,  LOG_NORMAL },
    { "lamprey",        do_lamprey,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "maelstrom",      do_maelstrom,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "majesty",        do_majesty,         POS_STANDING,    0,  LOG_NORMAL },
    { "mirror",         do_mirror,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "psychiccrush",   do_physic_crush,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "renew",          do_renew,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "shroud",         do_shroud,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "spiritblast",    do_spiritblast,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "spit",           do_spit,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "tears",          do_tears_reality,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "unveil",         do_unveil,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "vsilence",       do_death,           POS_FIGHTING,    0,  LOG_NORMAL },

    { "etherealform",   do_ether_form,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "gaseous",        do_gaseous,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "shadowplane",    do_shadowplane,     POS_STANDING,    0,  LOG_NORMAL },
    { "shadowfuse",     do_shadow_fuse,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "grab",           do_grab,            POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Dark Elf general commands.
     */
    { "transfuse",      do_transfuse,      POS_RESTING,      3, LOG_NORMAL },

    /*
     * Dark Elf Militia commands.
     */
    { "blackwidow",     do_black_widow,    POS_FIGHTING,     3, LOG_NORMAL },
    { "catacomb",       do_catacomb,       POS_FIGHTING,     3, LOG_NORMAL },
    { "deathsmell",     do_death_smell,    POS_FIGHTING,     3, LOG_NORMAL },
    { "demidrow",       do_demi_drow,      POS_FIGHTING,     3, LOG_NORMAL },
    { "elghinnwusuul",  do_elghinn_wusuul, POS_FIGHTING,     3, LOG_NORMAL },
    { "fallenangel",    do_fallen_angel,   POS_FIGHTING,     3, LOG_NORMAL },
    { "lolthdance",     do_lolth_dance,    POS_FIGHTING,     3, LOG_NORMAL },
    { "lolthshoot",     do_lolth_shoot,    POS_FIGHTING,     3, LOG_NORMAL },
    { "lolththalack",   do_lolth_thalack,  POS_FIGHTING,     3, LOG_NORMAL },
    { "olplynirdro",    do_olplynir_dro,   POS_FIGHTING,     3, LOG_NORMAL },
    { "orbblance",      do_orbb_lance,     POS_FIGHTING,     3, LOG_NORMAL },
    { "seriesslash",    do_series_slash,   POS_FIGHTING,     3, LOG_NORMAL },
    { "shadowshoot",    do_shadow_shoot,   POS_FIGHTING,     3, LOG_NORMAL },
    { "virynsmace",     do_viryns_mace,    POS_FIGHTING,     3, LOG_NORMAL },
    { "underworld",     do_under_world,    POS_FIGHTING,     3, LOG_NORMAL },
    { "waver",          do_waver,          POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Dark Elf Magi commands.
     */
    { "acidarrow",      do_acid_arrow,     POS_FIGHTING,     3, LOG_NORMAL },
    { "blur",           do_blur,           POS_RESTING,      3, LOG_NORMAL },
    { "cloudkill",      do_cloudkill,      POS_FIGHTING,     3, LOG_NORMAL },
    { "confusion",      do_confusion,      POS_FIGHTING,     3, LOG_NORMAL },
    { "darkstrike",     do_dark_strike,    POS_FIGHTING,     3, LOG_NORMAL },
    { "holyshame",      do_holy_shame,     POS_FIGHTING,     3, LOG_NORMAL },
    { "horror",         do_horror,         POS_FIGHTING,     3, LOG_NORMAL },
    { "icedust",        do_ice_dust,       POS_FIGHTING,     3, LOG_NORMAL },
    { "icestorm",       do_ice_storm,      POS_FIGHTING,     3, LOG_NORMAL },
    { "shadowflare",     do_shadow_flare,  POS_FIGHTING,     3, LOG_NORMAL },
    { "invulnerability", do_invunerability, POS_FIGHTING,     3, LOG_NORMAL },
    { "lolthcharm",     do_lolth_charm,    POS_FIGHTING,     3, LOG_NORMAL },
    { "roaringskies",   do_roaring_skies,  POS_FIGHTING,     3, LOG_NORMAL },
    { "shadowdoor",     do_shadow_door,    POS_FIGHTING,     3, LOG_NORMAL },
    { "shockinggrasp",  do_shocking_grasp, POS_FIGHTING,     3, LOG_NORMAL },
    { "spidersummon",   do_spider_summon,  POS_FIGHTING,     3, LOG_NORMAL },
    { "swiftwave",      do_swift_wave,     POS_FIGHTING,     3, LOG_NORMAL },
    { "titan",          do_titan,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "titansrage",     do_titans_rage,    POS_FIGHTING,     3, LOG_NORMAL },
    { "vampirictouch",  do_vampiric_touch, POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Dark Elf Clerical commands.
     */
    { "aid",            do_aid,            POS_RESTING,      3, LOG_NORMAL },
    { "bless",          do_bless,          POS_RESTING,      3, LOG_NORMAL },
    { "cura",           do_cura,           POS_RESTING,      3, LOG_NORMAL },
    { "flameblade",     do_flame_blade,    POS_RESTING,      3, LOG_NORMAL },
    { "flamestrike",    do_flame_strike,   POS_FIGHTING,     3, LOG_NORMAL },
    { "holdperson",     do_hold_person,    POS_FIGHTING,     3, LOG_NORMAL },
    { "lolthstrength",  do_lolth_strength, POS_RESTING,      3, LOG_NORMAL },
    { "lolthwrath",     do_lolth_wrath,    POS_FIGHTING,     3, LOG_NORMAL },
    { "neutralize",     do_neutralize,     POS_RESTING,      3, LOG_NORMAL },
    { "poison",         do_poison,         POS_FIGHTING,     3, LOG_NORMAL },
    { "demiprot",       do_protection,     POS_RESTING,      3, LOG_NORMAL },
    { "lolthprayer",    do_lolth_prayer,   POS_RESTING,      3, LOG_NORMAL },
    { "raisedead",      do_raise_dead,     POS_RESTING,      3, LOG_NORMAL },
    { "shillelagh",     do_shillelagh,     POS_RESTING,      3, LOG_NORMAL },
    { "unparalyse",     do_unparalyse,     POS_RESTING,      3, LOG_NORMAL },

    /*
     * Fiend commands.
     */
    { "stinger",        do_stinger,        POS_FIGHTING,     3, LOG_NORMAL },
    { "falsepride",     do_false_pride,    POS_FIGHTING,     3, LOG_NORMAL },
    { "roundtrip",      do_round_trip,     POS_FIGHTING,     3, LOG_NORMAL },
    { "airraid",        do_air_raid,       POS_FIGHTING,     3, LOG_NORMAL },
    { "vortex",         do_vortex,         POS_FIGHTING,     3, LOG_NORMAL },
    { "magmadrive",     do_magma_drive,    POS_FIGHTING,     3, LOG_NORMAL },
    { "lungekick",      do_lunge_kick,     POS_FIGHTING,     3, LOG_NORMAL },
    { "meteorblast",    do_meteor_blast,   POS_FIGHTING,     3, LOG_NORMAL },
    { "hellswrath",     do_fiend_inferno,  POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Soulreaver general commands.
     */
    { "belot",          do_belot,          POS_RESTING,      3, LOG_NORMAL },
    { "bloodstorm",     do_blood_storm,    POS_FIGHTING,     3, LOG_NORMAL },
    { "cainesummon",    do_caine_summon,   POS_FIGHTING,     3, LOG_NORMAL },
    { "nightrequiem",   do_night_requiem,  POS_FIGHTING,     3, LOG_NORMAL },
    { "paralysis",      do_paralysis,      POS_FIGHTING,     3, LOG_NORMAL },
    { "soulbeam",       do_soul_beam,      POS_FIGHTING,     3, LOG_NORMAL },
    { "souleraser",     do_soul_eraser,    POS_FIGHTING,     3, LOG_NORMAL },
    { "soulreaper",     do_soul_reaper,    POS_FIGHTING,     3, LOG_NORMAL },
    { "soulstrike",     do_soul_strike,    POS_FIGHTING,     3, LOG_NORMAL },
    { "soulreaver",     do_soul_reaver,    POS_FIGHTING,     3, LOG_NORMAL },

    { "doublexp",       do_doublexp,       POS_DEAD,         9,  LOG_ALWAYS },

    /*
     * Soulreaver forms commands.
     */
    { "lunge",          do_lunge,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "lungebite",      do_lunge_bite,     POS_FIGHTING,     3, LOG_NORMAL },
    { "sonicpierce",    do_sonic_pierce,   POS_FIGHTING,     3, LOG_NORMAL },
    { "wings",          do_wings,           POS_RESTING,     0,  LOG_NORMAL },
    { "wingsmash",      do_wing_smash,     POS_FIGHTING,     3, LOG_NORMAL },
    { "wolfcharge",     do_wolf_charge,    POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Soulreaver D. Specialists commands.
     */
    { "bloodcleanse",   do_blood_cleanse,  POS_FIGHTING,     3, LOG_NORMAL },
    { "bloodritual",    do_blood_ritual,   POS_FIGHTING,     3, LOG_NORMAL },
    { "bloodsense",     do_sense_blood,    POS_FIGHTING,     3, LOG_NORMAL },
    { "daggerpounce",   do_dagger_pounce,  POS_FIGHTING,     3, LOG_NORMAL },
    { "doubletwist",    do_double_twist,   POS_FIGHTING,     3, LOG_NORMAL },
    { "doubleattack",   do_double_attack,  POS_FIGHTING,     3, LOG_NORMAL },
    { "absorb",         do_absorb,         POS_FIGHTING,     3, LOG_NORMAL },
    { "doubleheal",     do_double_heal,    POS_FIGHTING,     3, LOG_NORMAL },
    { "ultraheal",      do_ultra_heal,     POS_FIGHTING,     3, LOG_NORMAL },
    { "mirage",         do_mirage,         POS_FIGHTING,     3, LOG_NORMAL },
    { "neckslash",      do_neck_slash,     POS_FIGHTING,     3, LOG_NORMAL },
    { "spearkick",      do_spear_kick,     POS_FIGHTING,     3, LOG_NORMAL },
    { "soulsteal",      do_soul_steal,     POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Soulreaver A. Specialists commands.
     */
    { "bloodwave",      do_blood_wave,   POS_FIGHTING,     3, LOG_NORMAL },
    { "darkfire",       do_dark_fire,    POS_FIGHTING,     3, LOG_NORMAL },
    { "earthgrasp",     do_earth_grasp,  POS_FIGHTING,     3, LOG_NORMAL },
    { "fear",           do_fear,         POS_FIGHTING,     3, LOG_NORMAL },
    { "hellfire",       do_hellfire,     POS_FIGHTING,     3, LOG_NORMAL },
    { "poisonmist",     do_poison_mist,  POS_FIGHTING,     3, LOG_NORMAL },
    { "summon",         do_summon,       POS_FIGHTING,     1, LOG_NORMAL },
    { "summonspirit",   do_summon_spirit,POS_FIGHTING,     3, LOG_NORMAL },
    { "tetraspirit",    do_tetra_spirit, POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Soulreaver T. Specialists commands.
     */
    { "disrupture",     do_disrupture,   POS_FIGHTING,     3, LOG_NORMAL },
    { "incite",         do_incite,       POS_FIGHTING,     3, LOG_NORMAL },
    { "invisiblewall",  do_invis_wall,   POS_FIGHTING,     3, LOG_NORMAL },
    { "maliceform",     do_malice_form,  POS_FIGHTING,     3, LOG_NORMAL },
    { "mirrorimage",    do_vampire_image,POS_FIGHTING,     3, LOG_NORMAL },
    { "nightmare",      do_nightmare,    POS_FIGHTING,     3, LOG_NORMAL },
    { "psyreaper",      do_psy_reaper,   POS_FIGHTING,     3, LOG_NORMAL },
    { "psyreaver",      do_psy_reaver,   POS_FIGHTING,     3, LOG_NORMAL },
    { "psyslice",       do_psy_slice,    POS_FIGHTING,     3, LOG_NORMAL },
    { "stigmata",       do_stigmata,     POS_FIGHTING,     3, LOG_NORMAL },
    { "telebarrier",    do_tele_barrier, POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Knight commands.
     */
    { "avatar",         do_avatar,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "charge",         do_charge,          POS_STANDING,    0,  LOG_NORMAL },
    { "crushpunch",     do_crush_punch,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "eruptionsword",  do_eruption_sword,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "flashsword",     do_flash_sword,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "genesis",        do_genesis,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "godlysight",     do_godlysight,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "godsheal",       do_godsheal,        POS_STANDING,    0,  LOG_NORMAL },
    { "godsread",       do_godsread,        POS_MEDITATING,  0,  LOG_NORMAL },
    { "godssearch",     do_godssearch,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "holyexplosion",  do_holy_explosion,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "justicecutter",  do_justice_cutter,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "lightningstab",  do_lightning_stab,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "nemesis",        do_nemesis,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "pgrant",         do_pgrant,          POS_RESTING,     0,  LOG_NORMAL },
    { "soulhammer",     do_soul_hammer,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "subdue",         do_subdue,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "splitpunch",     do_split_punch,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "stasissword",    do_stasis_sword,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "nomercy",        do_no_mercy,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "demonslice",     do_demon_slice,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "durandal",       do_durandal,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "vengeance",      do_vengeance,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "holyshield",     do_holy_shield,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "shellblaststab", do_shellblast_stab, POS_FIGHTING,    0,  LOG_NORMAL },
    { "hellcrypunch",   do_hellcry_punch,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "blasterpunch",   do_blaster_punch,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "icewolfbite",    do_icewolf_bite,    POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Divine Knight commands.
     */
    { "cover",          do_cover,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "guard",          do_guard,           POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Nests Commander commands.
     */
    { "ayanae",          do_ayanae,         POS_FIGHTING,     3, LOG_NORMAL },
    { "spiritbomb",      do_spirit_bomb,    POS_FIGHTING,     3, LOG_NORMAL },
    { "swipe",           do_swipe,          POS_FIGHTING,     3, LOG_NORMAL },
    { "kipunch",         do_ki_punch,       POS_FIGHTING,     3, LOG_NORMAL },
    { "kicharge",        do_ki_charge,      POS_FIGHTING,     3, LOG_NORMAL },
    { "oniyaki",         do_oniyaki,        POS_FIGHTING,     3, LOG_NORMAL },
    { "lockon",          do_lockon,         POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Kuruda commands.

    { "orochinagi",      do_orochi_nagi,    POS_FIGHTING,     3, LOG_NORMAL },
    { "shiki",           do_shiki,          POS_FIGHTING,     3, LOG_NORMAL },
     */

    { "aragami",         do_aragami,        POS_FIGHTING,     3, LOG_NORMAL },
    { "blazinghand",     do_blazing_hand,   POS_FIGHTING,     3, LOG_NORMAL },
    { "ironhammer",      do_ironhammer,     POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Drow commands.
     */
    { "bloodlet",       do_bloodlet,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "chaosblast",     do_chaosblast,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkdodge",      do_darkdodge,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkness",       do_darkness,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "darktendrils",   do_darktendrils,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "dischard",       do_dischard,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "drewst",         do_drewst,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "drowfire",       do_drowfire,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "drowpowers",     do_drowpowers,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "drowsight",      do_drowsight,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "drowshield",     do_drowshield,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "earthblast",     do_earth_blast,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "fightdance",     do_fightdance,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "grant",          do_grant,           POS_RESTING,     0,  LOG_NORMAL },
    { "hex",            do_hex,             POS_FIGHTING,    0,  LOG_NORMAL },
    { "olath",          do_olath,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "pandorabox",     do_pandora_box,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "shoot",          do_shoot,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "velkyn",         do_velkyn,          POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Dark Knight commands.
     */
    { "analyse",        do_analyse,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "advise",         do_advise,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "absolutezero",   do_absolute_zero,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "confuser",       do_confuser,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkbomb",       do_dark_bomb,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "antistock",      do_anti_stock,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkstorm",      do_dark_storm,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "determine",      do_determine,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "droundhouse",    do_knight_round,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "dsweep",         do_knight_sweep,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "duppercut",      do_knight_upper,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "finishingtouch", do_finishing_touch, POS_FIGHTING,    0,  LOG_NORMAL },
    { "hyper",          do_hyper,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "hypercharge",    do_kyaku,           POS_FIGHTING,     3, LOG_NORMAL },
    { "hyperdrive",     do_hyper_drive,     POS_FIGHTING,     3, LOG_NORMAL },
    { "speedup",        do_speed_up,        POS_FIGHTING,     3, LOG_NORMAL },
    { "hwrath",         do_hwrath,          POS_STANDING,    0,  LOG_NORMAL },
    { "mergance",       do_mergance,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "moonwave",       do_moon_wave,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "psychicbarrier", do_psychic_barrier, POS_FIGHTING,    0,  LOG_NORMAL },
    { "renzokuken",     do_renzokuken,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "slash",          do_slash,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "stitch",         do_stitch,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "swordrain",      do_sword_rain,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "swordtakedown",  do_knight_takedown, POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Team commands.
     */
    { "adjust",         do_adjust,          POS_MEDITATING,  0,  LOG_NORMAL },
    { "dodge",          do_dodge,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "counterpush",    do_counter_push,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "emergencyroll",  do_emergency_roll,  POS_STUNNED,     0,  LOG_NORMAL },
    { "emergencyflip",  do_emergency_flip,  POS_MORTAL,      0,  LOG_NORMAL },
    { "eroll",          do_emergency_roll,  POS_STUNNED,     0,  LOG_NORMAL },
    { "eflip",          do_emergency_flip,  POS_MORTAL,      0,  LOG_NORMAL },
    { "supercancel",    do_super_cancel,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "form",           do_form,            POS_MEDITATING,  0,  LOG_NORMAL },
    { "retort",         do_retort,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "tabulate",       do_tabulate,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "armormode",      do_armor_mode,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "hypermode",      do_hyper_mode,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "countermode",    do_counter_mode,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "chainmode",      do_chain_mode,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "infinitymode",   do_infinity_mode,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "strike",         do_strike,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "cheer",          do_cheer,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "taunt",          do_taunt,           POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Highwind commands.
     */
    { "dragondive",     do_dragon_dive,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "dragoonthrow",   do_dragoon_throw,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "dynamite",       do_dynamite,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "highwind",       do_highwind,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "hyperjump",      do_hyper_jump,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "impale",         do_impale,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "jump",           do_jump,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "ujump",          do_ultra_jump,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "vspears",        do_virtual_spears,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "whiskeybreak",   do_whiskey_break,   POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Dragon commands.
     */
    { "dragonbreath",   do_dragonbreath,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "dragongift",     do_dragongift,      POS_RESTING,     0,  LOG_NORMAL },
    { "dragonshift",    do_dragonshift,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "dragonswoop",    do_dragonswoop,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "highclaw",       do_highclaw,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "hyperclaw",      do_hyper_claw,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "finalflare",     do_final_flare,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "megaflare",      do_dragon_flare,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "shoulderrush",   do_shoulderrush,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "stomp",          do_stomp,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "tailsweep",      do_tailsweep,       POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Highlander commands.
     */
    { "bladespin",      do_bladespin,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "jcatapult",      do_jcatapult,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "jchoke",         do_jchoke,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "jdestruction",   do_jdestruction,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "jheal",          do_jheal,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "jinvis",         do_jinvis,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "jlightning",     do_jlightning,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "jpull",          do_jpull,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "jpush",          do_jpush,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "jrush",          do_jrush,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "jsummon",        do_jsummon,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "jtaunt",         do_jtaunt,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "jtouch",         do_jtouch,          POS_STANDING,    0,  LOG_NORMAL },
    { "levitate",       do_levitate,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "lightsaber",     do_katana,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "meteorstrike",   do_meteor_strike,   POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Demon commands.
     */
    { "blink",          do_blink,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "damnation",      do_damnation,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkholy",       do_dark_holy,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkforce",      do_dark_force,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "battlelust",     do_battle_lust,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "shed",           do_shed,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "hailstorm",      do_hail_storm,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "dcurse",         do_curse,           POS_RESTING,     0,  LOG_NORMAL },
    { "desanctify",     do_desanctify,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "demonform",      do_demonform,       POS_SLEEPING,    0,  LOG_NORMAL },
    { "entomb",         do_entomb,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "eyespy",         do_eyespy,          POS_RESTING,     0,  LOG_NORMAL },
    { "frostbreath",    do_frostbreath,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "frostbite",      do_fbite,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "gust",           do_gust,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "hooves",         do_hooves,          POS_RESTING,     0,  LOG_NORMAL },
    { "horns",          do_horns,           POS_RESTING,     0,  LOG_NORMAL },
    { "humanform",      do_humanform,       POS_SLEEPING,    0,  LOG_NORMAL },
    { "inferno",        do_dinferno,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "inpart",         do_inpart,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "lavapit",        do_lava_pit,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "leap",           do_leap,            POS_SLEEPING,    0,  LOG_NORMAL },
    { "lifespan",       do_lifespan,        POS_DEAD,        0,  LOG_NORMAL },
    { "roll",           do_roll,            POS_SLEEPING,    0,  LOG_NORMAL },
    { "seed",           do_seed,            POS_RESTING,     0,  LOG_NORMAL },
    { "sin",            do_sin,             POS_FIGHTING,    0,  LOG_NORMAL },
    { "soulheal",       do_soulheal,        POS_RESTING,     0,  LOG_NORMAL },
    { "soullink",       do_soullink,        POS_RESTING,     0,  LOG_NORMAL },
    { "unnerve",        do_unnerve,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "weaponform",     do_weaponform,      POS_STANDING,    0,  LOG_NORMAL },
    { "zombie",         do_zombie,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "endozone",       do_endo_zone,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "crucifix",       do_crucifix,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "demoncradle",    do_demon_cradle,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "revenga",        do_revenga,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "yamiurasugi",    do_yami_urasugi,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "breakthrough",   do_break_through,   POS_FIGHTING,    0,  LOG_NORMAL },


    /*
     * Mage Commands.
     */
    { "aureola",        do_aureola,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "abduction",      do_abduction,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "accelerate",     do_accelerate,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "age",            do_age,             POS_FIGHTING,    1,  LOG_NORMAL },
    { "aggression",     do_aggression,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "alabasterpotion",do_alabaster_potion,POS_MEDITATING,  1,  LOG_NORMAL },
    { "antimagic",      do_anti_magic,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "antiaura",       do_anti_aura,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "briarshield",    do_briar_shield,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "cleanse",        do_cleanse,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "disempower",     do_disempower,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "drainpower",     do_drain_power,     POS_FIGHTING,    1,  LOG_NORMAL },
    { "enrage",         do_enrage,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "evocate",        do_evocate,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "elementaltouch", do_elemental_touch, POS_FIGHTING,    1,  LOG_NORMAL },
    { "fireball",       do_fireball,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "gaeablessing",	do_gaea_blessing,   POS_FIGHTING,    1,  LOG_NORMAL },
    { "greatwall",      do_great_wall,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "haste",          do_haste,           POS_FIGHTING,    1,  LOG_NORMAL },
    { "icelance",       do_icelance,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "layhands",       do_layhands,        POS_STANDING,    3,  LOG_NORMAL },
    { "mangarablessing",do_mangara_blessing,POS_FIGHTING,	 1,  LOG_NORMAL },
    { "reveal",         do_reveal,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "primalrage",     do_primal_rage,     POS_STANDING,    0,  LOG_NORMAL },
    { "naturerevolt",   do_nature_revolt,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "quicken",        do_quicken,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "soulfield",      do_soul_field,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "specialize",     do_specialize,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "slow",           do_slow,            POS_FIGHTING,    1,  LOG_NORMAL },
    { "stop",           do_stop,            POS_FIGHTING,    1,  LOG_NORMAL },
    { "summon",         do_summon,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "telekinetics",   do_telekinetics,    POS_FIGHTING,	 1,  LOG_NORMAL },
    { "tempest",        do_tempest,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "timerlock",      do_timerlock,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "leechingtouch",  do_ltouch,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "gambler",        do_gambler,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "royalflush",     do_royal_flush,     POS_FIGHTING,    1,  LOG_NORMAL },
    { "subtlepinch",    do_subtle_pinch,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "ultima",         do_ultima,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "unsummon",       do_unsummon,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "whirlwind",      do_whirlwind,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "whirlwindcrusher",  do_whirlwind_crusher,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "read",           do_read,            POS_RESTING,     1,  LOG_NORMAL },
    { "write",          do_write,           POS_RESTING,     1,  LOG_NORMAL },
    { "chant",          do_chant,           POS_FIGHTING,    1,  LOG_NORMAL },
    { "glamour",        do_glamour,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "mlearn",         do_mlearn,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "darkball",       do_darkball,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "timewarp",       do_timewarp,        POS_FIGHTING,    1,  LOG_NORMAL },

    // Kingdom Powers
    { "timestop",       do_time_stop,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "destroy",        do_destroy,         POS_DEAD,       12,  LOG_NORMAL },
    { "destroyer",      do_destroyer,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "blaster",        do_blaster,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "shroudstrike",   do_shroud_strike,   POS_FIGHTING,    1,  LOG_NORMAL },
    { "scout",          do_scout,           POS_FIGHTING,    1,  LOG_NORMAL },
    { "shadowwalk",     do_shadow_walk,     POS_FIGHTING,    1,  LOG_NORMAL },

    /*
     * Necromancer Commands.
     */
    { "ereshkigal",     do_ereshkigal,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "madness",        do_madness,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "seek",           do_seek,            POS_FIGHTING,    1,  LOG_NORMAL },
    { "reanimate",      do_reanimate,       POS_DEAD,        1,  LOG_NORMAL },
    { "bindingagony",   do_binding_agony,   POS_FIGHTING,    1,  LOG_NORMAL },
    { "calling",        do_calling,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "corpseexplosion",do_corpse_explosion,POS_FIGHTING,    1,  LOG_NORMAL },
    { "gascloud",       do_gas_cloud,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "necrodrain",     do_necro_drain,     POS_FIGHTING,    1,  LOG_NORMAL },
    { "reduce",         do_reduce,          POS_FIGHTING,    1,  LOG_NORMAL },
    { "shadowgrasp",    do_shadowgrasp,     POS_STANDING,    1,  LOG_NORMAL },
    { "venom",          do_venom,           POS_STANDING,    1,  LOG_NORMAL },

    /*
     * Summoner Commands.
     */
    { "bahamut",        do_bahamut,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "carburkle",      do_reflect_beam,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "diablos",        do_ddarkside,       POS_FIGHTING,    1,  LOG_NORMAL },
    { "ifrit",          do_ifrit,           POS_FIGHTING,    1,  LOG_NORMAL },
    { "mwall",          do_mwall,           POS_FIGHTING,    1,  LOG_NORMAL },
    { "moogle",         do_sun_bath,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "odin",           do_zantetsuken,     POS_FIGHTING,    1,  LOG_NORMAL },
    { "quezacotl",      do_thunder_storm,   POS_FIGHTING,    1,  LOG_NORMAL },
    { "shiva",          do_diamond_dust,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "siren",          do_silent_voice,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "salamander",     do_salamander,      POS_FIGHTING,    1,  LOG_NORMAL },

    /*
     * Ninja commands.
     */
    { "choke",          do_choke,           POS_FIGHTING,    1,  LOG_NORMAL },
    { "dig",            do_dig,             POS_STANDING,    0,  LOG_NORMAL },
    { "inclimb",        do_inclimb,         POS_FIGHTING,    1,  LOG_NORMAL },
    { "kakusu",         do_kakusu,          POS_STANDING,    0,  LOG_NORMAL },
    { "kanzuite",       do_kanzuite,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "mitsukeru",      do_mitsukeru,       POS_FIGHTING,    1,  LOG_NORMAL },

    /*
     * Night Blade commands.
     */
    { "bomuzite",       do_bomuzite,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "hakunetsu",      do_hakunetsu,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "hunt",           do_hunt,            POS_STANDING,    0,  LOG_NORMAL },
    { "koryou",         do_koryou,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "mienaku",        do_mienaku,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "mizotamouyo",    do_mizotamouyo,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "principles",     do_principles,      POS_SLEEPING,    0,  LOG_NORMAL },
    { "sonicstrike",    do_sonic_strike,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "trip",           do_trip,            POS_FIGHTING,    0,  LOG_NORMAL },
    { "dinichi",        do_dinichi,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "gunaya",         do_net,             POS_FIGHTING,    0,  LOG_NORMAL },
    { "incapitate",     do_incapitate,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "orimasu",        do_orimasu,         POS_STANDING,    0,  LOG_NORMAL },
    { "yoga",           do_yoga,            POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Gaiden commands.
     */
    { "alert",          do_alertness,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "backfist",       do_backfist,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "caltrops",       do_caltrops,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "gunaya",         do_net,             POS_FIGHTING,    0,  LOG_NORMAL },
    { "hishokenslam",   do_hishoken_slam,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "illusion",       do_illusion,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "nknee",          do_nknee,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "kipinch",        do_ki_pinch,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "lowkick",        do_lowkick,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "reverse",        do_reverse,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "risingtiger",    do_rising_tiger,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "somerstrike",    do_somer_strike,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "spinkick",       do_spin_kick,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "stardance",      do_stardance,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "nsweep",         do_nsweep,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "throw",          do_throw,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "thrust",         do_thrust_kick,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "uppercut",       do_uppercut,        POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Sorceror commands.
     */
    { "manikatti",      do_mani_katti,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "solkatti",       do_sol_katti,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "aurawave",       do_aura_wave,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "blaze",          do_blaze,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "blitzstrike",    do_blitzstrike,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "blitzsword",     do_blitzsword,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "blitztouch",     do_blitztouch,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "chaossword",     do_chaos_sword,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "dancingflames",  do_dancingflames,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "darksword",      do_darksword,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "deathfield",     do_deathfield,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "despairstrike",  do_despairstrike,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "earthsword",     do_earthsword,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "encase",         do_encase,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "firesword",      do_firesword,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "firewall",       do_firewall,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "frigidcut",      do_frigidcut,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "icesword",       do_icesword,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "icewall",        do_icewall,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "lunabeam",       do_lunabeam,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "moonslash",      do_moonslash,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "moonsword",      do_moonsword,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "piercingone",    do_piercing_one,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "quadraslice",    do_quadraslice,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "ragedash",       do_ragedash,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "rockblast",      do_rockblast,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "thunderbeam",    do_thunderbeam,     POS_FIGHTING,    0,  LOG_NORMAL },

    { "breach",         do_breach,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "lumicabarrier",  do_lumica_barrier,  POS_MEDITATING,  0,  LOG_NORMAL },

    /*
     * Undead Knight commands.
     */
    { "armads",         do_armads,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "gain",           do_gain,            POS_RESTING,     0,  LOG_NORMAL },
    { "genocide",       do_genocide,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "powerword",      do_powerword,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "unholyrite",     do_unholyrite,      POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Saiyan commands.
     */
    { "bigbang",        do_big_bang,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "charging",       do_charging,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "choukamehameha", do_chou_kamehameha, POS_FIGHTING,    0,  LOG_NORMAL },
    { "galickhou",      do_gallet_gun,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "kakusan",        do_kakusan,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "kienzan",        do_distrucodisc,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "kamehameha",     do_kamehameha,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "kaioken",        do_kaioken,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "mouthblast",     do_mouthblast,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "pounce",         do_pounce,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "powerball",      do_power_ball,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "powerread",      do_power_read,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "powersearch",    do_power_search,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "ryuken",         do_ryuken,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "scatterpunch",   do_scatterpunch,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "taiyouken",      do_solarflare,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "genkidama",      do_spiritbomb,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "ssj",            do_supersaiyan,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "tailswipe",      do_tailswipe,       POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Daywalker commands.
     */
    { "cgrab",          do_cgrab,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "daydream",       do_day_dream,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "deathgrip",      do_death_grip,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "desparation",    do_desparation,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "despoticseptor", do_despotic_septor, POS_FIGHTING,    0,  LOG_NORMAL },
    { "egrab",          do_egrab,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "inquest",        do_inquest,         POS_RESTING,     0,  LOG_NORMAL },
    { "insight",        do_insight,         POS_RESTING,     0,  LOG_NORMAL },
    { "jiamar",         do_jiamar,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "naturecleansing",do_nature_cleansing,POS_FIGHTING,    0,  LOG_NORMAL },
    { "naturewrath",    do_nature_wrath,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "nightmare",      do_nightmare,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "piercingrain",   do_piercing_rain,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "psionicblast",   do_psionicblast,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "rathi",          do_rathi,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "recover",        do_recover,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "shakar",         do_shakar,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "shatter",        do_shatter,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "sonicthrust",    do_sonic_thrust,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "souldrain",      do_soul_drain,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "stakewall",      do_stakewall,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "statis",         do_statis,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "sunbeam",        do_sunbeam,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "transform",      do_transform,       POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Assassin commands.
     */
    { "control",        do_control,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "handcircua",     do_hand_circua,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "switch",         do_switch,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkmatter",     do_dark_matter,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "chiblast",       do_chi_blast,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "luminaire",      do_luminaire,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "piercingdasher", do_piercing_dasher,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "midrain",        do_mi_drain,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "triheal",        do_tri_heal,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "flashgrenade",   do_flash_grenade,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "dispose",        do_dispose,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "disguise",       do_disguise,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "painless",       do_painless,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "possess",        do_possess,          POS_MORTAL,      0,  LOG_NORMAL },
    { "guidedshot",     do_assassin_guided,  POS_FIGHTING,    0,  LOG_NORMAL },
    { "innerhealing",   do_inner_healing,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "innerrage",      do_inner_rage,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "yang",           do_yang_power,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "yin",            do_yin_power,        POS_FIGHTING,    0,  LOG_NORMAL },
    { "innershield",    do_inner_shield,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "innerfury",      do_inner_fury,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "dispel",         do_dispel,           POS_FIGHTING,    0,  LOG_NORMAL },
    { "handblast",      do_hand_blast,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "chinsmite",      do_chin_smite,       POS_FIGHTING,    0,  LOG_NORMAL },
    { "legdispatcher",  do_leg_dispatcher,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "armgrab",        do_arm_grab,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "bodyrip",        do_body_rip,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "kiwrath",        do_ki_wrath,         POS_FIGHTING,    0,  LOG_NORMAL },
    { "study",          do_study,            POS_DEAD,        0,  LOG_NORMAL },

    /*
     * Samurai commands.
     */
    { "combo",           do_combo,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "elementalstrike", do_elemental_slash,POS_FIGHTING,    0,  LOG_NORMAL },
    { "knowledge",       do_knowledge,      POS_RESTING,     0,  LOG_NORMAL },
    { "lightslash",      do_light_slash,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "mediumslash",     do_samurai_mslash, POS_FIGHTING,    0,  LOG_NORMAL },
    { "hardslash",       do_samurai_hslash, POS_FIGHTING,    0,  LOG_NORMAL },
    { "sdraw",           do_sdraw,          POS_FIGHTING,    0,  LOG_NORMAL },
    { "seppuko",         do_seppuko,        POS_RESTING,     0,  LOG_NORMAL },
    { "quicklash",       do_quick_lash,     POS_FIGHTING,    0,  LOG_NORMAL },

    { "hakiznetu",       do_hakiznetu,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "splitstrike",     do_split_strike,   POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * Shadow Warrior commands.
     */
    { "shadowswap",      do_shadowswap,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "ragingdemon",     do_raging_demon,   POS_FIGHTING,    0,  LOG_NORMAL },
    { "darkhadou",       do_dark_hadou,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "innerchi",        do_inner_chi,      POS_FIGHTING,    0,  LOG_NORMAL },
    { "palmtackle",      do_palm_tackle,    POS_FIGHTING,    0,  LOG_NORMAL },
    { "flashkick",       do_flash_kick,     POS_FIGHTING,    0,  LOG_NORMAL },
    { "swirlingsmite",   do_swirling_smite, POS_FIGHTING,    0,  LOG_NORMAL },

    /*
     * H. Werewolf commands.
     */
    { "fadedmoon",       do_faded_moon,     POS_FIGHTING,     3, LOG_NORMAL },
    { "fadedstrike",     do_fadedstrike,    POS_FIGHTING,     3, LOG_NORMAL },
    { "ragingslash",     do_raging_slash,   POS_FIGHTING,     3, LOG_NORMAL },
    { "tornadoslice",    do_tornado_slice,  POS_FIGHTING,     3, LOG_NORMAL },
    { "twistingmoon",    do_twisting_moon,  POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Hobbits commands.
     */
    { "catseye",         do_cats_eye,       POS_FIGHTING,     3, LOG_NORMAL },
    { "crystaldreams",   do_crystal_dreams, POS_FIGHTING,     3, LOG_NORMAL },
    { "daintlydark",     do_daintly_dark,   POS_FIGHTING,     3, LOG_NORMAL },
    { "darkhell",        do_dark_hell,      POS_FIGHTING,     3, LOG_NORMAL },
    { "demoneye",        do_demon_eye,      POS_FIGHTING,     3, LOG_NORMAL },
    { "famedarchangel",  do_famed_archangel,POS_FIGHTING,     3, LOG_NORMAL },
    { "flare",           do_flare,          POS_FIGHTING,     3, LOG_NORMAL },
    { "frogget",         do_frogget,        POS_FIGHTING,     3, LOG_NORMAL },
    { "grandlove",       do_grand_love,     POS_FIGHTING,     3, LOG_NORMAL },
    { "loathdemon",      do_loath_demon,    POS_FIGHTING,     3, LOG_NORMAL },
    { "shadowdancer",    do_shadow_dancer,  POS_FIGHTING,     3, LOG_NORMAL },
    { "shadowdemon",     do_shadow_demon,   POS_FIGHTING,     3, LOG_NORMAL },
    { "tillymint",       do_tilly_mint,     POS_FIGHTING,     3, LOG_NORMAL },
    { "truthful",        do_truthful,       POS_FIGHTING,     3, LOG_NORMAL },
    { "wraitheknight",   do_wknights,       POS_FIGHTING,     3, LOG_NORMAL },


    /*
     * N. Werewolf commands.
     */
    { "bareknuckle",     do_bare_knuckle,   POS_FIGHTING,     3, LOG_NORMAL },
    { "burnknuckle",     do_burn_knuckle,   POS_FIGHTING,     3, LOG_NORMAL },
    { "burningrave",     do_burning_rave,   POS_FIGHTING,     3, LOG_NORMAL },
    { "busterwolf",      do_buster_wolf,    POS_FIGHTING,     3, LOG_NORMAL },
    { "moondance",       do_moon_dance,     POS_FIGHTING,     3, LOG_NORMAL },
    { "mooneclipse",     do_moon_eclipse,   POS_FIGHTING,     3, LOG_NORMAL },
    { "morph",           do_morph,          POS_FIGHTING,     3, LOG_NORMAL },
    { "neckthrottle",    do_neck_throttle,  POS_FIGHTING,     3, LOG_NORMAL },
    { "omegarave",       do_omega_rave,     POS_FIGHTING,     3, LOG_NORMAL },
    { "overheatgeyser",  do_overheat_geyser,POS_FIGHTING,     3, LOG_NORMAL },
    { "overdrive",       do_over_drive,     POS_FIGHTING,     3, LOG_NORMAL },
    { "subsitance",      do_subsitance,     POS_FIGHTING,     3, LOG_NORMAL },
    { "powerdunk",       do_power_dunk,     POS_FIGHTING,     3, LOG_NORMAL },
    { "powercharge",     do_power_charge,   POS_FIGHTING,     3, LOG_NORMAL },
    { "powerwave",       do_power_wave,     POS_FIGHTING,     3, LOG_NORMAL },
    { "triplegeyser",    do_triple_geyser,  POS_FIGHTING,     3, LOG_NORMAL },

	/*
	 * Snow Elf Commands.
	 */
    { "icearrow",        do_ice_arrow,      POS_FIGHTING,     3, LOG_NORMAL },
    { "freezingblast",   do_freezing_blast, POS_FIGHTING,     3, LOG_NORMAL },
    { "slitherice",      do_slither_ice,    POS_FIGHTING,     3, LOG_NORMAL },
    { "iceslash",        do_ice_slash,      POS_FIGHTING,     3, LOG_NORMAL },
    { "iceslice",        do_ice_slice,      POS_FIGHTING,     3, LOG_NORMAL },
    { "winterswans",     do_winter_swans,   POS_RESTING,      3, LOG_NORMAL },
    { "carpejuglum",     do_carpe_jugulum,  POS_FIGHTING,     3, LOG_NORMAL },
    { "icepunch",        do_ice_punch,      POS_FIGHTING,     3, LOG_NORMAL },
    { "iceshield",       do_ice_shield,     POS_FIGHTING,     3, LOG_NORMAL },
    { "ambush",          do_ambush,         POS_FIGHTING,     3, LOG_NORMAL },
    { "icetrap",         do_ice_trap,       POS_FIGHTING,     3, LOG_NORMAL },
    { "multiarrow",      do_multi_arrow,    POS_FIGHTING,     3, LOG_NORMAL },
    { "arrowdrain",      do_arrow_drain,    POS_FIGHTING,     3, LOG_NORMAL },
    { "demonicshot",     do_demonic_shot,   POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Wisp Commands.
     */
    { "electricshield",  do_electric_shield,POS_RESTING,      3, LOG_NORMAL },
    { "colourspray",     do_colour_spray,   POS_FIGHTING,     3, LOG_NORMAL },
    { "innerlight",      do_inner_light,    POS_FIGHTING,     3, LOG_NORMAL },
    { "heatshield",      do_heat_shield,    POS_FIGHTING,     3, LOG_NORMAL },
    { "calllightning",   do_call_lightning, POS_RESTING,      3, LOG_NORMAL },
    { "thunderstorm",    do_thunderstorm,   POS_FIGHTING,     3, LOG_NORMAL },
    { "plasmablade",     do_plasma_blade,   POS_RESTING,      3, LOG_NORMAL },
    { "icarus",          do_icarus,         POS_FIGHTING,     3, LOG_NORMAL },
    { "mindtap",         do_mindtap,        POS_FIGHTING,     3, LOG_NORMAL },
    { "colourbeam",      do_colourbeam,     POS_FIGHTING,     3, LOG_NORMAL },
    { "rainblade",       do_rain_blade,     POS_FIGHTING,     3, LOG_NORMAL },
    { "electrotrigger",  do_electrotrigger, POS_FIGHTING,     3, LOG_NORMAL },

	/*
	 * Duergar Commands.
	 */
	{ "swing",           do_swing,          POS_FIGHTING,     3, LOG_NORMAL },
	{ "slasheraxe",      do_slasher_axe,    POS_FIGHTING,     3, LOG_NORMAL },
	{ "stainedaxe",      do_stained_axe,    POS_FIGHTING,     3, LOG_NORMAL },
	{ "axeslam",         do_axe_slam,       POS_FIGHTING,     3, LOG_NORMAL },
	{ "duergardance",    do_duergar_dance,  POS_FIGHTING,     3, LOG_NORMAL },
	{ "axeddrain",       do_axed_drain,     POS_FIGHTING,     3, LOG_NORMAL },
	{ "axeknockout",     do_axe_knockout,   POS_FIGHTING,     3, LOG_NORMAL },
	{ "darkcloud",       do_dark_cloud,     POS_FIGHTING,     3, LOG_NORMAL },
	{ "earthquake",      do_earthquake,     POS_FIGHTING,     3, LOG_NORMAL },
	{ "forge",           do_forge,          POS_FIGHTING,     3, LOG_NORMAL },
	{ "tunnel",          do_tunnel,         POS_FIGHTING,     3, LOG_NORMAL },
	{ "vineofevil",      do_vineofevil,     POS_FIGHTING,     3, LOG_NORMAL },

	/*
	 * Phoenix Commands.
	 */
	{ "firebreath",      do_firebreath,     POS_FIGHTING,     3, LOG_NORMAL },
	{ "flametalons",     do_flame_talons,   POS_FIGHTING,     3, LOG_NORMAL },
	{ "volcano",         do_volcano,        POS_FIGHTING,     3, LOG_NORMAL },
	{ "phoenixcry",      do_phoenix_cry,    POS_FIGHTING,     3, LOG_NORMAL },
	{ "phoenixrush",     do_phoenix_rush,   POS_STANDING,     3, LOG_NORMAL },
	{ "phoenixwrath",    do_phoenix_wrath,  POS_FIGHTING,     3, LOG_NORMAL },
	{ "firecannon",      do_fire_cannon,    POS_FIGHTING,     3, LOG_NORMAL },

	/*
	 * Daramon Commands.
	 */
	{ "empower",         do_empower,        POS_FIGHTING,     3, LOG_NORMAL },
	{ "stunner",         do_stunner,        POS_FIGHTING,     3, LOG_NORMAL },
	{ "stunningblow",    do_stunning_blow,  POS_FIGHTING,     3, LOG_NORMAL },
	{ "buddhastomp",     do_buddhastomp,    POS_FIGHTING,     3, LOG_NORMAL },
	{ "shockwave",       do_shock_wave,     POS_FIGHTING,     3, LOG_NORMAL },
	{ "zinjakicks",      do_zinja_kicks,    POS_FIGHTING,     3, LOG_NORMAL },
	{ "flaminguppercut", do_f_uppercut,     POS_FIGHTING,     3, LOG_NORMAL },
      { "tremor",          do_tremor,         POS_FIGHTING,     3, LOG_NORMAL },
	{ "chillblast",      do_chill_blast,    POS_FIGHTING,     3, LOG_NORMAL },
	{ "lightningslash",  do_l_slash,        POS_FIGHTING,     3, LOG_NORMAL },
      { "explode",         do_explode,        POS_FIGHTING,    1,  LOG_NORMAL },
	{ "exploder",        do_exploder,       POS_FIGHTING,     3, LOG_NORMAL },
	{ "heavenlygate",    do_heavenly_gate,  POS_FIGHTING,     3, LOG_NORMAL },
	{ "chakra",          do_chakra,         POS_FIGHTING,     3, LOG_NORMAL },

    /*
     * Dragoon commands.
     */
    { "astraldrain",    do_astral_drain,   POS_FIGHTING,     3, LOG_NORMAL },
    { "blueseadragon",  do_bluesea_dragon, POS_FIGHTING,     3, LOG_NORMAL },
    { "darkdragon",     do_dark_dragon,    POS_FIGHTING,     3, LOG_NORMAL },
    { "darkshrine",     do_dark_shrine,    POS_FIGHTING,     3, LOG_NORMAL },
    { "deathdimension", do_death_dimension,POS_FIGHTING,     3, LOG_NORMAL },
    { "demonsgate",     do_demon_gate,     POS_FIGHTING,     3, LOG_NORMAL },
    { "diamonddust",    do_dragoon_dust,   POS_FIGHTING,     3, LOG_NORMAL },
    { "explosion",      do_explosion,      POS_FIGHTING,     3, LOG_NORMAL },
    { "flameshot",      do_flame_shot,     POS_FIGHTING,     3, LOG_NORMAL },
    { "flamingtwirl",   do_flaming_twirl,  POS_FIGHTING,     3, LOG_NORMAL },
    { "finalburst",     do_final_burst,    POS_FIGHTING,     3, LOG_NORMAL },
    { "freezingrain",   do_freezing_rain,  POS_FIGHTING,     3, LOG_NORMAL },
    { "gaspless",       do_gaspless,       POS_FIGHTING,     3, LOG_NORMAL },
    { "handcannon",     do_hand_cannon,    POS_FIGHTING,     3, LOG_NORMAL },
    { "heavensgate",    do_heavens_gate,   POS_FIGHTING,     3, LOG_NORMAL },
    { "jadedragon",     do_jade_dragon,    POS_FIGHTING,     3, LOG_NORMAL },
    { "lastprayer",     do_last_prayer,    POS_FIGHTING,     3, LOG_NORMAL },
    { "merton",         do_merton,         POS_FIGHTING,     3, LOG_NORMAL },
    { "moonlight",      do_moonlight,      POS_FIGHTING,     3, LOG_NORMAL },
    { "rainbowbreath",  do_rainbow_breath, POS_FIGHTING,     3, LOG_NORMAL },
    { "redeyeddragon",  do_redeyed_dragon, POS_FIGHTING,     3, LOG_NORMAL },
    { "rosestorm",      do_rose_storm,     POS_FIGHTING,     3, LOG_NORMAL },
    { "regeneration",   do_regeneration,   POS_FIGHTING,     3, LOG_NORMAL },
    { "shift",          do_shift,          POS_FIGHTING,     3, LOG_NORMAL },
    { "divinecannon",   do_divine_cannon,  POS_FIGHTING,     3, LOG_NORMAL },
    { "divineball",     do_divine_ball,    POS_FIGHTING,     3, LOG_NORMAL },
    { "silverdragon",   do_silver_dragon,  POS_FIGHTING,     3, LOG_NORMAL },
    { "sonicbomb",      do_sonic_bomb,     POS_FIGHTING,     3, LOG_NORMAL },
    { "starchildren",   do_star_children,  POS_FIGHTING,     3, LOG_NORMAL },
    { "swirlingdance",  do_swirling_dance, POS_FIGHTING,     3, LOG_NORMAL },
    { "whirlpool",      do_whirlpool,      POS_FIGHTING,     3, LOG_NORMAL },
    { "wingblaster",    do_wing_blaster,   POS_FIGHTING,     3, LOG_NORMAL },
    { "demondance",     do_demon_dance,    POS_FIGHTING,     3, LOG_NORMAL },
    { "flowerstorm",    do_flower_storm,   POS_FIGHTING,     3, LOG_NORMAL },
    { "blazingdynamo",  do_blazing_dynamo, POS_FIGHTING,     3, LOG_NORMAL },


    /*
     * Extra class commands.
     */
    { "activate",       do_activate,       POS_FIGHTING,     3,  LOG_NORMAL },
    { "dash",           do_dash,           POS_STANDING,     3,  LOG_NORMAL },
    { "run",            do_run,            POS_STANDING,     3,  LOG_NORMAL },
    { "clannable",      do_seeking,        POS_DEAD,         3,  LOG_NORMAL },
    { "capture",        do_capture,        POS_FIGHTING,     3,  LOG_NORMAL },
//    { "kingdomlist",    do_kingdomlist,    POS_DEAD,         3,  LOG_NORMAL },
    { "kingnum",        do_kingnum,        POS_DEAD,         7,  LOG_NORMAL },
    { "kingset",        do_kingset,        POS_DEAD,         7,  LOG_NORMAL },
    { "krecall",        do_krecall,        POS_FIGHTING,     3,  LOG_NORMAL },
    { "lands",          do_lands,          POS_DEAD,         0,  LOG_NORMAL },
    { "obtain",         do_obtain,         POS_STANDING,     0,  LOG_NORMAL },
    { "induct",         do_induct,         POS_FIGHTING,     3,  LOG_NORMAL },
    { "memberlist",     do_memberlist,     POS_DEAD,         0,  LOG_NORMAL },
    { "readaura",       do_readaura,       POS_FIGHTING,     0,  LOG_NORMAL },
    { "release",        do_release,        POS_FIGHTING,     3,  LOG_NORMAL },
    { "viewkingdom",    do_kingdom,        POS_DEAD,         3,  LOG_NORMAL },
    { "outcast",        do_outcast,        POS_DEAD,         3,  LOG_NORMAL },
    { "prince",         do_prince,         POS_FIGHTING,     3,  LOG_NORMAL },
//    { "rogue",          do_rogue,          POS_FIGHTING,     3,  LOG_NORMAL },

    /* kingdom powers */
    { "golem",          do_assist,         POS_FIGHTING,     0,  LOG_NORMAL },
    { "energysword",    do_energysword,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "crescentslash",  do_crescent_slash, POS_FIGHTING,     0,  LOG_NORMAL },
    { "energybarrier",  do_barrier,        POS_FIGHTING,     0,  LOG_NORMAL },
    { "lightningblade", do_lightningblade, POS_FIGHTING,     0,  LOG_NORMAL },
    { "firedash",       do_fire_dash,      POS_FIGHTING,     0,  LOG_NORMAL },
    { "icebreak",       do_ice_break,      POS_FIGHTING,     0,  LOG_NORMAL },
    { "justicedance",   do_justice_dance,  POS_FIGHTING,     0,  LOG_NORMAL },
    { "earthsmash",     do_earth_smash,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "shadowthrust",   do_shadow_thrust,  POS_FIGHTING,     0,  LOG_NORMAL },

    { "shockblade",     do_shock_blade,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "lightningfist",  do_lightning_fist, POS_FIGHTING,     0,  LOG_NORMAL },
    { "meteorkick",     do_meteor_kick,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "tenhoblast",     do_tenho_blast,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "angelwings",     do_angel_wings,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "iceblast",       do_ice_blast,      POS_FIGHTING,     0,  LOG_NORMAL },
    { "roulette",       do_roulette,       POS_FIGHTING,     0,  LOG_NORMAL },
    { "sonicarc",       do_sonic_arc,      POS_FIGHTING,     0,  LOG_NORMAL },

    { "thunderblade",   do_thunder_blade,  POS_FIGHTING,     0,  LOG_NORMAL },
    { "lunarblade",     do_lunar_blade,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "omegablade",     do_omega_blade,    POS_FIGHTING,     0,  LOG_NORMAL },
    { "entangle",       do_entangle,       POS_FIGHTING,     0,  LOG_NORMAL },
    { "koho",           do_koho,           POS_FIGHTING,     0,  LOG_NORMAL },
    { "runeblade",      do_rune_blade,     POS_FIGHTING,     0,  LOG_NORMAL },
    { "runeshift",      do_rune_shift,     POS_FIGHTING,     0,  LOG_NORMAL },
    { "anticross",      do_anti_cross,     POS_FIGHTING,     0,  LOG_NORMAL },
    { "truesacrifice",  do_true_sacrifice, POS_FIGHTING,    1,  LOG_NORMAL },
    { "antilight",      do_anti_light,     POS_FIGHTING,    1,  LOG_NORMAL },


    { "yamisugi",       do_yami_sugi,      POS_FIGHTING,     3, LOG_NORMAL },
    { "mirrorstance",   do_mirror_stance,  POS_FIGHTING,    1,  LOG_NORMAL },
    { "rcannon",        do_rcannon,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "xbuster",        do_xbuster,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "foreverzero",    do_forever_zero,   POS_FIGHTING,    1,  LOG_NORMAL },
    { "firebomb",       do_fire_bomb,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "satellite",      do_satellite,      POS_FIGHTING,    1,  LOG_NORMAL },
    { "lunarshield",    do_lunar_shield,   POS_FIGHTING,    1,  LOG_NORMAL },
    { "executioner",    do_executioner,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "zanretsuken",    do_zanretsuken,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "rhammer",        do_rhammer,        POS_FIGHTING,    1,  LOG_NORMAL },
    { "lightblade",     do_light_blade,    POS_FIGHTING,    1,  LOG_NORMAL },
    { "voodoo",         do_voodoo,         POS_FIGHTING,    1,  LOG_NORMAL },

    /*
     * Immortal commands.
     */
    { "addlag",         do_addlag,         POS_DEAD,         11, LOG_NORMAL },
    { "allow",          do_allow,          POS_DEAD,         11, LOG_ALWAYS },
    { "armageddon",     do_armageddon,     POS_DEAD,         11, LOG_NORMAL },
    { "at",             do_at,             POS_DEAD,         8,  LOG_NORMAL },
    { "bamfin",         do_bamfin,         POS_DEAD,         7,  LOG_NORMAL },
    { "bamfout",        do_bamfout,        POS_DEAD,         7,  LOG_NORMAL },
    { "ban",            do_ban,            POS_DEAD,         7,  LOG_ALWAYS },
    { "bind",           do_bind,           POS_DEAD,         10, LOG_ALWAYS },
    { "bitchslap",      do_freeze,         POS_DEAD,         9,  LOG_ALWAYS },
    { "copyover",       do_copyover,       POS_DEAD,         9,  LOG_ALWAYS },
    { "create",         do_create,         POS_STANDING,     8,  LOG_NORMAL },
    { "stoken",         do_ctoken,         POS_DEAD,	       7,  LOG_NORMAL },
    { "pktoken",        do_pktoken,        POS_DEAD,	       7,  LOG_NORMAL },
    { "deny",           do_deny,           POS_DEAD,         10, LOG_ALWAYS },
    { "disable",        do_disable,        POS_DEAD,         11, LOG_ALWAYS },
    { "disconnect",	do_disconnect,     POS_DEAD,         10, LOG_NEVER  },
    { "divorce",        do_divorce,        POS_DEAD,         9,  LOG_ALWAYS },
    { "echo",           do_echo,           POS_DEAD,         8,  LOG_ALWAYS },
    { "force",          do_force,          POS_DEAD,         9,  LOG_ALWAYS },
    { "for",            do_for,            POS_DEAD,         10, LOG_NORMAL },
    { "freeze",         do_freeze,         POS_DEAD,		 9,  LOG_ALWAYS },
    { "ftag",           do_ftag,           POS_SLEEPING,     3,  LOG_NORMAL },
    { "goto",           do_goto,           POS_DEAD,         7,  LOG_NORMAL },
    { "holylight",      do_holylight,      POS_DEAD,	       7,  LOG_NORMAL },
    { "hlist",          do_hlist,          POS_DEAD,	       7,  LOG_NORMAL },
    { "immreturn",      do_return,         POS_DEAD,         8,  LOG_NORMAL },
    { "immtalk",        do_immtalk,        POS_DEAD,         7,  LOG_NORMAL },
    { "incog",          do_incog,          POS_DEAD,         7,  LOG_NORMAL },
    { "info",           do_info,           POS_DEAD,         7,  LOG_NORMAL },
    { "kingdomwars",    do_kingdom_wars,   POS_DEAD,         11, LOG_NORMAL },
    { "leaderclear",    do_leaderclear,    POS_DEAD,         7,  LOG_NORMAL },
    { "linkdead",       do_linkdead,       POS_DEAD,         7,  LOG_NORMAL },
    { "log",            do_log,            POS_DEAD,         12, LOG_ALWAYS },
    { "marry",          do_marry,          POS_DEAD,         9,  LOG_ALWAYS },
    { "medit",          do_medit,          POS_DEAD,         7,  LOG_NORMAL },
    { "meditate",		do_meditate,       POS_MEDITATING,	 0,  LOG_NORMAL },
    { "memory",         do_memory,         POS_DEAD,         8,  LOG_NORMAL },
    { "mfind",          do_mfind,          POS_DEAD,         7,  LOG_NORMAL },
    { "mload",          do_mload,          POS_DEAD,         7,  LOG_ALWAYS },
    { "mset",           do_mset,           POS_DEAD,	       8,  LOG_NORMAL },
    { "mstat",          do_mstat,          POS_DEAD,         9,  LOG_NORMAL },
    { "multicheck",     do_multicheck,     POS_DEAD,	       7,  LOG_NORMAL },
    { "mwhere",         do_mwhere,         POS_DEAD,         8,  LOG_NORMAL },
    { "noemote",        do_noemote,        POS_DEAD,         9,  LOG_NORMAL },
    { "nogang",         do_nogang,         POS_DEAD,         7,  LOG_NORMAL },
    { "nosafe",         do_nosafe,         POS_DEAD,         7,  LOG_NORMAL },
    { "notell",         do_notell,         POS_DEAD,         9,  LOG_NORMAL },
    { "oclone",         do_oclone,         POS_DEAD,	       8,  LOG_ALWAYS },
    { "ofind",          do_ofind,          POS_DEAD,         7,  LOG_NORMAL },
    { "oload",          do_oload,          POS_DEAD,         8,  LOG_ALWAYS },
    { "omni",           do_omni,           POS_DEAD,         7,  LOG_NORMAL },
    { "oreturn",        do_oreturn,        POS_DEAD,         8,  LOG_NORMAL },
    { "oset",           do_oset,           POS_DEAD,         8,  LOG_ALWAYS },
    { "ostat",          do_ostat,          POS_DEAD,         7,  LOG_NORMAL },
    { "oswitch",        do_oswitch,        POS_DEAD,         8,  LOG_NORMAL },
    { "otransfer",      do_otransfer,      POS_DEAD,	       8,  LOG_ALWAYS },
    { "pack",           do_pack,           POS_DEAD,         7,  LOG_ALWAYS },
    { "paradox",        do_paradox,        POS_DEAD,         7,  LOG_ALWAYS },
    { "peace",          do_peace,          POS_DEAD,         7,  LOG_NORMAL },
    { "permban",        do_permban,        POS_DEAD,         7,  LOG_ALWAYS },
    { "pfile",          do_pfile,          POS_DEAD,         7,  LOG_NORMAL },
    { "pload",          do_pload,          POS_DEAD,         12, LOG_ALWAYS },
    { "pset",           do_pset,           POS_DEAD,         8,  LOG_NORMAL },
    { "pstat",          do_pstat,          POS_DEAD,         10, LOG_ALWAYS },
    { "purge",          do_purge,          POS_DEAD,         7,  LOG_NORMAL },
    { "qset",           do_qset,           POS_DEAD,         8,  LOG_ALWAYS },
    { "qstat",          do_qstat,          POS_DEAD,         8,  LOG_ALWAYS },
    { "qtrust",         do_qtrust,         POS_DEAD,         10, LOG_ALWAYS },
    { "reboo",          do_reboo,          POS_DEAD,         10, LOG_NORMAL },
    { "reboot",         do_reboot,         POS_DEAD,         9,  LOG_ALWAYS },
    { "recho",          do_recho,          POS_DEAD,         8,  LOG_ALWAYS },
    { "resetarea",      do_resetarea,      POS_DEAD,         10, LOG_NORMAL },
    { "resetpassword",	do_resetpassword,  POS_DEAD,	       12, LOG_ALWAYS },
    { "restore",        do_restore,        POS_DEAD,         8,  LOG_ALWAYS },
    { "rset",           do_rset,           POS_DEAD,         8,  LOG_ALWAYS },
    { "rstat",          do_rstat,          POS_DEAD,         7,  LOG_NORMAL },
    { "silence",        do_silence,        POS_DEAD,         9,  LOG_NORMAL },
    { "shutdow",        do_shutdow,        POS_DEAD,         10, LOG_NORMAL },
    { "shutdown",       do_shutdown,       POS_DEAD,         12, LOG_ALWAYS },
    { "sla",            do_sla,            POS_DEAD,         10, LOG_NORMAL },
    { "slay",           do_slay,           POS_DEAD,         10, LOG_ALWAYS },
    { "slookup",        do_slookup,        POS_DEAD,         8,  LOG_NORMAL },
    { "snoop",          do_snoop,          POS_DEAD,         8,  LOG_NORMAL },
    { "special",        do_special,        POS_DEAD,         10, LOG_ALWAYS },
    { "sset",           do_sset,           POS_DEAD,         10, LOG_NORMAL },
    { "sustain",        do_sustain,        POS_FIGHTING,     3,  LOG_NORMAL },
    { "tick",           do_tick,           POS_DEAD,         7,  LOG_NORMAL },
    { "token",          do_token,          POS_DEAD,	       7,  LOG_NORMAL },
    { "trust",          do_trust,          POS_DEAD,         11, LOG_ALWAYS },
    { "undeny",         do_undeny,         POS_DEAD,         12, LOG_ALWAYS },
    { "use",            do_use,            POS_FIGHTING,     3,  LOG_NORMAL },
    { "users",          do_users,          POS_DEAD,         8,  LOG_NORMAL },
    { "wizlock",	      do_wizlock,        POS_DEAD,         11, LOG_ALWAYS },

    /*
     * OLC 1.1b
     */
    { "aedit",          do_aedit,          POS_DEAD,         7,  LOG_NORMAL },
    { "alist",          do_alist,          POS_DEAD,         7,  LOG_NORMAL },
    { "asave",          do_asave,          POS_DEAD,         7,  LOG_NORMAL },
    { "oedit",          do_oedit,          POS_DEAD,         7,  LOG_NORMAL },
    { "redit",          do_redit,          POS_DEAD,         7,  LOG_NORMAL },
    { "relearn",        do_relearn,        POS_DEAD,         0,  LOG_NORMAL },
    { "resets",         do_resets,         POS_DEAD,         7,  LOG_NORMAL },

    /*
     * End of list.
     */
    { "",               0,                 POS_DEAD,         0,  LOG_NORMAL }
};



/*
 * The social table.
 * Add new socials here.
 * Alphabetical order is not required.
 */
const	struct	social_type	social_table [] =
{
    {   "jewel",
        "confess your obsession of jewel to WHO?!?!",
        "$n confesses their love of JEWEL KILCHER to you.",
        "You confess your obession of love to $M",
        "$n gets on one knee and confesses their love of Jewel Kilcher.",
        "$n tells you how much they love Jewel Kilcher.",
        "Don't you already know?",
        "$n thinks happy thoughts about Jewel Kilcher.",
        "$n gets on one knee and confesses their love of Jewel Kilcher.",
    },

    {
	"accuse",
	"Accuse whom?",
	"$n is in an accusing mood.",
	"You look accusingly at $M.",
	"$n looks accusingly at $N.",
	"$n looks accusingly at you.",
	"You accuse yourself.",
	"$n seems to have a bad conscience.",
	"$n looks accusingly at $t.",
    },

    {
	"ack",
	"You gasp and say 'ACK!' at your mistake.",
	"$n ACKS at $s big mistake.",
	"You ACK $M.",
	"$n ACKS $N.",
	"$n ACKS you.",
	"You ACK yourself.",
	"$n ACKS $mself.  Must be a bad day.",
	"$n ACKS $t.",
    },
	
    {
	"addict",
	"You stand and admit to all in the room, 'Hi, I'm $n, and I'm a mud addict.'",
	"$n stands and says, 'Hi, I'm $n, and I'm a mud addict.'",
	"You tell $M that you are addicted to $S love.",
	"$n tells $N that $e is addicted to $S love.",
	"$n tells you that $e is addicted to your love.",
	"You stand and admit to all in the room, 'Hi, I'm $n, and I'm a mud addict.'",
	"$n stands and says, 'Hi, I'm $n, and I'm a mud addict.'",
	"$n tells $t that $e is addicted to their love.",
    },

    {
	"afk",
	"You announce that you are going away from keyboard.",
	"$n announces that $e is going away from keyboard.",
	"You announce that you are going away from keyboard.",
	"$n informs $N that $e is going away from keyboard.",
	"$n informs you that $e is going away from keyboard.",
	"You announce that you are going away from keyboard.",
	"$n announces that $e is going away from keyboard.",
	"$n informs $t that $e is going away from keyboard.",
    },

    {
	"imawhore",
	"You seem to be in an agreeable mood.",
	"$n seems to agree.",
	"You agree with $M.",
	"$n agrees with $N.",
	"$n agrees with you.",
	"Well I hope you would agree with yourself!",
	"$n agrees with $mself, of course.",
	"$n agrees with $t.",
    },

    {
	"airguitar",
	"You sizzle the air with your BITCHIN' guitar playing!",
	"$n air-guitars like Jimi Hendrix!",
	"You sizzle the air with your BITCHIN' guitar playing!",
	"$n air-guitars like Jimi Hendrix!",
	"$n air-guitars like Jimi Hendrix, just for you!",
	"You forget all else as you sizzle the air with your BITCHIN' guitar playing!",
	"$n forgets all else as $e air-guitars like Jimi Hendrix.",
	"$n air-guitars like Jimi Hendrix!",
    },
    
    {
	"apologize",
	"You apologize for your behavior.",
	"$n apologizes for $s rude behavior.",
	"You apologize to $M.",
	"$n apologizes to $N.",
	"$n apologizes to you.",
	"You apologize to yourself.",
	"$n apologizes to $mself.  Hmmmm.",
	"$n apologizes to $t.",
    },

    {
	"applaud",
	"Clap, clap, clap.",
	"$n gives a round of applause.",
	"You clap at $S actions.",
	"$n claps at $N's actions.",
	"$n gives you a round of applause.  You MUST'VE done something good!",
	"You applaud at yourself.  Boy, are we conceited!",
	"$n applauds at $mself.  Boy, are we conceited!",
	"$n claps at $t's actions.",
    },

    {
	"banzai",
	"You scream 'BANZAI!!!!' and charge into the fray.",
	"$n screams 'BANZAI!!!!' and charges into the fray.",
	"You scream 'BANZAI!!!!' and drag $M into the fray with you.",
	"$n screams 'BANZAI!!!!' and drags $N into the fray with $m!",
	"$n screams 'BANZAI!!!!' and drags you into the fray with $m!",
	"You scream 'BANZAI!!!!' and brandish your weapon for battle.",
	"$n screams 'BANZAI!!!!' and brandishes $s weapon for battle.",
	"$n screams 'BANZAI!!!!' and drags $t into the fray with $m!",
    },


    {
	"bark",
	"Woof!  Woof!",
	"$n barks like a dog.",
	"You bark at $M.",
	"$n barks at $N.",
	"$n barks at you.",
	"You bark at yourself.  Woof!  Woof!",
	"$n barks at $mself.  Woof!  Woof!",
	"$n barks at $t.",
    },

    {
	"bbl",
	"You announce that you will be back later.",
	"$n announces that $e'll be back later.",
	"You inform $M that you will be back later.",
	"$n informs $N that $e will be back later",
	"$n informs you that $e will be back later",
	"You mumble to yourself that you'll be back later.",
	"$n mumbles to $mself that $e'll be back later.",
	"$n informs $t that $e will be back later",
    },

    {
	"bearhug",
	"You hug a grizzly bear.",
	"$n hugs a flea-infested grizzly bear.",
	"You bearhug $M.",
	"$n bearhugs $N.  Some ribs break.",
	"$n bearhugs you.  Wonder what's coming next?",
	"You bearhug yourself.",
	"$n bearhugs $mself.",
	"$n bearhugs $t.  Some ribs break.",
    },

    {
        "beef",
        "You loudly exclaim 'WHERE'S THE BEEF??????'",
        "$n loudly exclaims 'WHERE'S THE BEEF??????'",
        "You poke $N and exclaim, 'WHERE'S THE BEEF?????'",
        "$n pokes $N, and exclaims, 'WHERE'S THE BEEF????'",
        "$n pokes you and exclaims, 'WHERE'S THE BEEF????'",
        "You poke your fat rolls and exclaim, 'Oh, THERE'S THE BEEF!'",
        "$n pokes $s fat rolls, and exclaims, 'Oh, THERE'S THE BEEF!'",
        "$n pokes $t, and exclaims, 'WHERE'S THE BEEF????'",
    },
   
    {
	"beer",
	"You down a cold, frosty beer.",
	"$n downs a cold, frosty beer.",
	"You draw a cold, frosty beer for $N.",
	"$n draws a cold, frosty beer for $N.",
	"$n draws a cold, frosty beer for you.",
	"You draw yourself a beer.",
	"$n draws $mself a beer.",
	"$n draws a cold, frosty beer for $t.",
    },

    {
	"beg",
	"You beg the for mercy.",
	"$n falls to the ground and begs for mercy.",
	"You desperately beg $M for mercy.",
	"$n begs $N for mercy!",
	"$n begs you for mercy.",
	"Begging yourself for mercy doesn't help.",
	"$n begs $mself for mercy.",
	"$n begs $t for mercy!",
    },

    {
	"whimper",
	"You whimper softly.",
	"$n whimpers pathetically.",
	"You look at $N and whimper sadly.",
	"$n looks at $N and whimpers sadly.",
	"$n looks at you and whimpers sadly.",
	"You whimper to yourself.",
	"$n whimpers to $mself.",
	"$n looks at $t and whimpers sadly.",
    },

    {
	"behead",
	"You look around for some heads to cut off.",
	"$n looks around for some heads to cut off.",
	"You grin evilly at $N and brandish your weapon.",
	"$n grins evilly at $N, while branding $s weapon!",
	"$n grins evilly at you, brandishing $s weapon.",
	"I really don't think you want to do that...",
	"$n is so desperate for exp that $e tries to decapitate $mself!",
	"$n grins evilly at $t, while branding $s weapon!",
    },

    {
	"bkiss",
	"Blow a kiss to whom?",
	"$n blows at $s hand.",
	"You blow a kiss to $M.",
	"$n blows a kiss to $N.  Touching, ain't it?",
	"$n blows a kiss to you.  Not as good as a real one, huh?",
	"You blow a kiss to yourself.",
	"$n blows a kiss to $mself.  Wierd.",
	"$n blows a kiss to $t.  Touching, ain't it?",
    },
  
    {
	"bleed",
	"You bleed all over the room!",
	"$n bleeds all over the room!  Get out of $s way!",
	"You bleed all over $M!",
	"$n bleeds all over $N.  Better leave, you may be next!",
	"$n bleeds all over you!  YUCK!",
	"You bleed all over yourself!",
	"$n bleeds all over $mself.",
	"$n bleeds all over $t.  Better leave, you may be next!",
    },

    {
	"blink",
	"You blink in utter disbelief.",
	"$n blinks in utter disbelief.",
	"You blink at $M in confusion.",
	"$n blinks at $N in confusion.",
	"$n blinks at you in confusion.",
	"You are sooooooooooooo confused",
	"$n blinks at $mself in complete confusion.",
	"$n blinks at $t in confusion.",
    },

	/* This social is for Stephen of ACK mud :) */
    {
	"blownose",
	"You blow your nose loudly.",
	"$n blows $s nose loudly.",
	"You blow your nose on $S shirt.",
	"$n blows $s nose on $N's shirt.",
	"$n blows $s nose on your shirt.",
	"You blow your nose on your shirt.",
	"$n blows $s nose on $s shirt.",
	"$n blows $s nose on $t's shirt.",
    },

    {
	"blush",
	"Your cheeks are burning.",
	"$n blushes.",
	"You get all flustered up seeing $M.",
	"$n blushes as $e sees $N here.",
	"$n blushes as $e sees you here.  Such an effect on people!",
	"You blush at your own folly.",
	"$n blushes as $e notices $s boo-boo.",
	"$n blushes as $e sees $t here.",
    },

    {
	"boggle",
	"You boggle at all the loonies around you.",
	"$n boggles at all the loonies around $m.",
	"You boggle at $S ludicrous idea.",
	"$n boggles at $N's ludicrous idea.",
	"$n boggles at your ludicrous idea.",
	"BOGGLE.",
	"$n wonders what BOGGLE means.",
	"$n boggles at $t's ludicrous idea.",
    },

    {
	"bonk",
	"BONK.",
	"$n spells 'potato' like Dan Quayle: 'B-O-N-K'.",
	"You bonk $M for being a numbskull.",
	"$n bonks $N.  What a numbskull.",
	"$n bonks you.  BONK BONK BONK!",
	"You bonk yourself.",
	"$n bonks $mself.",
	"$n bonks $t.  What a numbskull.",
    },

    {
	"boogie",
	"You boogie down!",
	"$n gets down and boogies!!",
	"You grab $M and boogie down!",
	"$n grabs $N and they boogie down!",
	"$n grabs you and boogies with you!",
	"You boogie with yourself.  What a great dancer you are!",
	"$n boogies with $mself.  Guess no one will dance with $m.",
	"$n grabs $t and they boogie down!",
    },
    
    {
	"bottle",
	"You open up a cold bottle of brew.",
	"$n opens a cold bottle of brew.",
	"You open up a cold bottle of brew for $M.",
	"$n opens a cold bottle of brew for $N.",
	"$n opens a cold bottle of brew for you.",
	"You open a cold bottle of brew for yourself.",
	"$n opens a cold bottle of brew for $mself." ,
	"$n opens a cold bottle of brew for $t.",
    },
 
    {
	"bounce",
	"BOIINNNNNNGG!",
	"$n bounces around.",
	"You bounce onto $S lap.",
	"$n bounces onto $N's lap.",
	"$n bounces onto your lap.",
	"You bounce your head like a basketball.",
	"$n plays basketball with $s head.",
	"$n bounces onto $t's lap.",
    },

    {
	"bow",
	"You bow deeply.",
	"$n bows deeply.",
	"You bow before $M.",
	"$n bows before $N.",
	"$n bows before you.",
	"You kiss your toes.",
	"$n folds up like a jack knife and kisses $s own toes.",
	"$n bows before $t.",
    },

    {
	"brb",
	"You announce that you will be right back.",
	"$n says in a stern voice, 'I'll be back!'",
	"You announce to $M that you will be right back.",
	"$n says to $N in a stern voice, 'I'll be back!'",
	"$n says to you in a stern voice, 'I'll be right back!'",
	"You mumble to yourself, 'I'll be right back'",
	"$n mumbles to $mself, 'I'll be right back, won't I?'",
	"$n says to $t in a stern voice, 'I'll be back!'",
    },

    {
	"brush",
	"Brush what? Who? Where?",
	NULL,
	"You brush out $S hair for $M.  Very thoughtful.",
	"$n brushes $N's hair for $M.  Looks better now.",
	"$n brushes out your hair.  How nice of $m.",
	"You brush out your hair.  There - much better.",
	"$n brushes out $s hair.  Looks much better now.",
	"$n brushes $t's hair for $M.  Looks better now.",
    },

    {
	"bully",
	"You growl and demand everyone's lunch money NOW!",
	"$n growls and demands everyone's lunch money.",  
	"You growl and demand $S lunch money NOW!",
	"$n growls and demands $N's lunch money.",
	"$n growls and demands your lunch money.",
	"You bully yourself, and take your own lunch money.",
	"$n bullies $mself, and takes $s lunch money away from $mself???",
	"$n growls and demands $t's lunch money.",
    },
 
    {  
	"bungy",
	"You tie a bungy cord to the mud and jump into internet.",
	"$n ties a bungy cord to the mud and jumps into internet.",
	"You tie a bungy cord to $N and throw $M off the mud.",
	"$n ties a bungy cord to $N and throws $M off the mud.",
	"$n ties a bungy cord to you and throws you off the mud.",
	"You tie a bungy cord to yourself and jump off the mud.",
	"$n ties a bungy cord to $mself and jumps off the mud.",
	"$n ties a bungy cord to $t and throws them off the mud.",
    },

    {
	"burp",
	"You burp loudly.",
	"$n burps loudly.",
	"You burp loudly to $M in response.",
	"$n burps loudly in response to $N's remark.",
	"$n burps loudly in response to your remark.",
	"You burp at yourself.",
	"$n burps at $mself.  What a sick sight.",
	"$n burps loudly in response to $t's remark.",
    },

    {
	"bye",
	"You say goodbye to all in the room.",
	"$n says goodbye to everyone in the room.",
	"You say goodbye to $N.",
	"$n says goodbye to $N.",
	"$n says goodbye to you.",
	"You say goodbye to yourself.  Contemplating suicide?",
	"$n says goodbye to $mself.  Is $e contemplating suicide?",
	"$n says goodbye to $t.",
    },

    {
	"byron",
	"You recite romantic poems to yourself.",
	"$n mutters romantic poems to himself, must be practicing for someone.",
	"You recite a romantic poem to $N.",
	"$n recites a beautiful romantic poem for $N.",
	"$n recites a heartstoppingly beautiful romantic poem for you.",
	"You read yourself romantic poems.  Falling in love with yourself?",
	"$n reads $mself a romantic poem.  Perhaps he loves $mself?",
	"$n recites a beautiful romantic poem for $t.",
    },

    {
	"cackle",
	"You throw back your head and cackle with insane glee!",
	"$n throws back $s head and cackles with insane glee!",
	"You cackle gleefully at $N",
	"$n cackles gleefully at $N.",
	"$n cackles gleefully at you.  Better keep your distance from $m.",
	"You cackle at yourself.  Now, THAT'S strange!",
	"$n is really crazy now!  $e cackles at $mself.",
	"$n cackles gleefully at $t.",
    },

    {
	"cannonball",
	"You cannonball into the pool.   *SPLOOSH*",
	"$n cannonballs into the pool.   *SPLOOSH*",
	"You cannonball into the pool and splash water over $N.",
	"$n cannonballs into the pool and splashes $N.",
	"$n cannonballs into the pool and sends water over you.  You are WET!",
	"You bounce cannonballs off your head to show your strength.",
	"$n bounces cannonballs off $s head.  Can you say 'nutcase'?",
	"$n cannonballs into the pool and splashes $t.",
    },

    {
	"catnap",
	"You curl into a tiny ball and go to sleep.",
	"$n curls $mself into a tiny ball and goes to sleep.",
	"You curl up in $S lap and go to sleep.",
	"$n curls up in $N's lap and goes to sleep.",
	"$n curls up in your lap and goes to sleep.",
	"You curl into a tiny ball and go to sleep.",
	"$n curls $mself into a tiny ball and goes to sleep.",
	"$n curls up in $t's lap and goes to sleep.",
    },

    {
	"cheer",
	"ZIS BOOM BAH!  BUGS BUNNY BUGS BUNNY RAH RAH RAH!",
	"$n cheers 'BUGS BUNNY BUGS BUNNY RAH RAH RAH!'",
	"You cheer loudly: 'Go $N Go!'",
	"$n cheers loudly: 'Go $N Go!'",
	"$n cheers you on!",
	"You cheer yourself up.",
	"$n cheers $mself on.",
	"$n cheers loudly: 'Go $t Go!'",
    },

    {
	"chekov",
	"You speak your best Russian in hopes of finding a nuclear wessel.",
	"$n asks sheepishly, 'Ver are the nuclear wessels?'",
	"You ask $M about the nuclear wessels.",
	"$n asks $N sheepishly, 'Ver are the nuclear wessels?'",
	"$n asks you sheepishly, 'Ver are the nuclear wessels?'",
	"You look around, muttering, 'Ver are the nuclear wessels?'",
	"$n looks around, muttering, 'Ver are the nuclear wessels?'",
	"$n asks $t sheepishly, 'Ver are the nuclear wessels?'",
    },
    
    {
	"chortle",
	"You chortle with glee.",
	"$n chortles with glee.",
	"You chortle loudly at $M.",
	"$n chortles loudly at $N.",
	"$n chortles loudly at you.",
	"You chortle loudly to yourself.",
	"$n chortles loudly to $mself.",
	"$n chortles loudly at $t.",
    },

    {
	"chuckle",
	"You chuckle politely.",
	"$n chuckles politely.",
	"You chuckle at $S joke.",
	"$n chuckles at $N's joke.",
	"$n chuckles at your joke.",
	"You chuckle at your own joke, since no one else would.",
	"$n chuckles at $s own joke, since none of you would.",
	"$n chuckles at $t's joke.",
    },

    {
	"clap",
	"You clap your hands together.",
	"$n shows $s approval by clapping $s hands together.",
	"You clap at $S performance.",
	"$n claps at $N's performance.",
	"$n claps at your performance.",
	"You clap at your own performance.",
	"$n claps at $s own performance.",
	"$n claps at $t's performance.",
    },

    { 
        "clue",
        "You mumble 'DUM-DUM-DUM-DUM ... ah, a clue!'",
        "$n mumbles 'DUM-DUM-DUM-DUM ... ah, a clue!'",
        "You mumble 'DUM-DUM-DUM-DUM ... ah, a clue!'",
        "$n mumbles 'DUM-DUM-DUM-DUM ... ah, a clue!'",
        "$n mumbles 'DUM-DUM-DUM-DUM ... $N, aha, a clue!'",
        "You mumble to yourself, 'DUM-DUM-DUM-DUM ... ah, a clue!'",
        "$n mumbles to $mself, 'DUM-DUM-DUM-DUM ... ah, a clue!'",
        "$n mumbles 'DUM-DUM-DUM-DUM ... $t, aha, a clue!'",
    },
   
    {
	"clueless",
	"You chastise everyone in the room for being clueless.",
	"$n chastises everyone for being clueless.",
	"You chastise $M for being clueless.",
	"$n chastises $N for being clueless.",
	"$n chastises you for being clueless.",
	"You chastise yourself for being clueless.  What a dweeb you are!",
	"$n chastises $mself for being clueless.  What a dweeb!",
	"$n chastises $t for being clueless.",
    },

    {
	"comb",
	"You comb your hair - perfect.",
	"$n combs $s hair, how dashing!",
	"You patiently untangle $N's hair - what a mess!",
	"$n tries patiently to untangle $N's hair.",
	"$n pulls your hair in an attempt to comb it.",
	"You pull your hair, but it will not be combed.",
	"$n tries to comb $s tangled hair.",
	"$n tries patiently to untangle $t's hair.",
    },

    {
	"comfort",
	"Do you feel uncomfortable?",
	NULL,
	"You comfort $M.",
	"$n comforts $N.",
	"$n comforts you.",
	"You make a vain attempt to comfort yourself.",
	"$n has no one to comfort $m but $mself.",
	"$n comforts $t.",
    },

    {
	"cough",
	"You cough to clear your throat and eyes and nose and....",
	"$n coughs loudly.",
	"You cough loudly.  It must be $S fault, $E gave you this cold.",
	"$n coughs loudly, and glares at $N, like it is $S fault.",
	"$n coughs loudly, and glares at you.  Did you give $m that cold?",
	"You cough loudly.  Why don't you take better care of yourself?",
	"$n coughs loudly.  $n should take better care of $mself." ,
	"$n coughs loudly, and glares at $t, like it is their fault.",
    },

    { 
	"cower",
	"What are you afraid of?",
	"$n cowers in the corner from claustrophobia.",
	"You cower in the corner at the sight of $M.",
	"$n cowers in the corner at the sight of $N.",
	"$n cowers in the corner at the sight of you.",
	"You cower in the corner at the thought of yourself.  You scaredy cat!",
	"$n cowers in the corner.  What is wrong with $m now?",
	"$n cowers in the corner at the sight of $t.",
    },
    
    {
	"cringe",
	"You cringe in terror.",
	"$n cringes in terror!",
	"You cringe away from $M.",
	"$n cringes away from $N in mortal terror.",
	"$n cringes away from you.",
	"I beg your pardon?",
	"$n cringes in terror!",
	"$n cringes away from $t in mortal terror.",
    },

    {
	"crush",
	"You squint and hold two fingers up, saying 'I'm crushing your heads!'",
	"$n squints and holds two fingers up, saying 'I'm crushing your heads!'",
	"You hold two fingers up at $M and say, 'I'm crushing your head!'",
	"$n holds two fingers up at $N and says, 'I'm crushing your head!'",
	"$n holds two fingers up at you and says, 'I'm crushing your head!'",
	"You crush yourself.  YEEEEOOOUUUUCH!",
	"$n crushes $mself into the ground.  OUCH!",
	"$n holds two fingers up at $t and says, 'I'm crushing your head!'",
    },

    {
	"cry",
	"Waaaaah ...",
	"$n bursts into tears.",
	"You cry on $S shoulder.",
	"$n cries on $N's shoulder.",
	"$n cries on your shoulder.",
	"You cry to yourself.",
	"$n sobs quietly to $mself.",
	"$n cries on $t's shoulder.",
    },

    {
	"cuddle",
	"Whom do you feel like cuddling today?",
	NULL,
	"You cuddle $M.",
	"$n cuddles $N.",
	"$n cuddles you.",
	"You must feel very cuddly indeed ... :)",
	"$n cuddles up to $s shadow.  What a sorry sight.",
	"$n cuddles $t.",
    },

    {
	"curse",
	"You swear loudly for a long time.",
	"$n swears: @*&^%@*&!",
	"You swear at $M.",
	"$n swears at $N.",
	"$n swears at you!  Where are $s manners?",
	"You swear at your own mistakes.",
	"$n starts swearing at $mself.  Why don't you help?",
	"$n swears at $t.",
    },

    {
	"curtsey",
	"You curtsey to your audience.",
	"$n curtseys gracefully.",
	"You curtsey to $M.",
	"$n curtseys gracefully to $N.",
	"$n curtseys gracefully for you.",
	"You curtsey to your audience (yourself).",
	"$n curtseys to $mself, since no one is paying attention to $m.",
	"$n curtseys gracefully to $t.",
    },

    {
	"dance",
	"Feels silly, doesn't it?",
	"$n tries to break dance, but nearly breaks $s neck!",
	"You sweep $M into a romantic waltz.",
	"$n sweeps $N into a romantic waltz.",
	"$n sweeps you into a romantic waltz.",
	"You skip and dance around by yourself.",
	"$n dances a pas-de-une.",
	"$n sweeps $t into a romantic waltz.",
    },

    {
	"dive",
	"You dive into the ocean.",
	"$n dives into the ocean.",
	"You dive behind $M and hide.",
	"$n dives behind $N and hides.",
	"$n dives behind you and hides.",
	"You take a dive.",
	"$n takes a dive.",
	"$n dives behind $t and hides.",
    },
    
    {
        "dizzy",
        "You are so dizzy from all this chatter.",
        "$n spins twice and hits the ground, dizzy from all this chatter.",
        "You are dizzy from all of $N's chatter.",
        "$n spins twice and hits the ground, dizzy from all $N's chatter.",
        "$n spins twice and hits the ground, dizzy from all your chatter.",
        "You are dizzy from lack of air.  Don't talk so much!",
        "$n spins twice and falls to the ground from lack of air.",
        "$n spins twice and hits the ground, dizzy from all $t's chatter.",
    },

    {
	"doc",
	"You nibble on a carrot and say 'Eh, what's up Doc?'",
	"$n nibbles on a carrot and says 'Eh, what's up Doc?'",
	"You nibble on a carrot and say to $M, 'Eh, what's up Doc?'",
	"$n nibbles on a carrot and says to $N, 'Eh, what's up Doc?'",
	"$n nibbles on a carrot and says to you, 'Eh, what's up Doc?'",
	"You nibble on a carrot and say to yourself, 'Eh, what's up $n?'",
	"$n nibbles on a carrot and says 'Eh, what's up Doc?'" ,
	"$n nibbles on a carrot and says to $t, 'Eh, what's up Doc?'",
    },
    
    {
	"doh",
	"You say, 'Doh!!' and hit your forehead.  What an idiot you are!",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"You say, 'Doh!!' and hit your forehead.  What an idiot you are!",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"You hit yourself in the forehead and say, 'Doh!!!'",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
    },
    
    /*
     * This one's for Baka, Penn, and Onethumb!
     */
    {
	"drool",
	"You drool on yourself.",
	"$n drools on $mself.",
	"You drool all over $N.",
	"$n drools all over $N.",
	"$n drools all over you.",
	"You drool on yourself.",
	"$n drools on $mself.",
	"$n drools all over $t.",
    },

    {
	"duck",
	"Whew!  That was close!",
	"$n is narrowly missed by a low-flying dragon.",
	"You duck behind $M.  Whew!  That was close!",
	"$n ducks behind $N to avoid the fray.",
	"$n ducks behind you to avoid the fray.",
	"You duck behind yourself.  Oww that hurts!",
	"$n tries to duck behind $mself.  $n needs help getting untied now.",
	"$n ducks behind $t to avoid the fray.",
    },

    {   
	"sembrace",
	"Who do you want to hold?",
	"$n looks around for someone to hold close to $m.",
	"You hold $M in a warm and loving embrace.",
	"$n holds $N in a warm and loving embrace.",
	"$n holds you in a warm and loving embrace.",
	"You hold yourself in a warm and loving embrace.  Feels silly doesn't it?",
	"$n holds $mself in a warm and loving embrace.  $e looks pretty silly.",
	"$n holds $t in a warm and loving embrace.",
    },

    {
	"eskimo",
	"Who do you want to eskimo kiss with?",
	"$n is looking for someone to rub noses with.  Any volunteers?",
	"You rub noses with $M.",
	"$n rubs noses with $N.  You didn't know they were eskimos!",
	"$n rubs noses with you.",
	"You ponder the difficulties involved in rubbing noses with yourself.",
	"$n ponders the difficulties involved in rubbing noses with $mself.",
	"$n rubs noses with $t.  You didn't know they were eskimos!",
    },
    
    {
	"evilgrin",
	"You grin so evilly that everyones alignment drops to -1000.",
	"$n grins evilly that everyones alignment drops to -1000.",
	"You grin so evilly at $M that $S alignment drops to -1000.",
	"$n grins so evilly at $N that $S alignment drops to -1000.",
	"$n grins so evilly at you that your alignment drops to -1000.",
	"You grin so evilly at yourself that your alignment drops to -1000.",
	"$n grins so evilly that $s alignment drops to -1000.",
	"$n grins so evilly at $t that their alignment drops to -1000.",
    },

    {
	"eyebrow",
	"You raise an eyebrow.",
	"$n raises an eyebrow.",
	"You raise an eyebrow at $M.",
	"$n raises an eyebrow at $N.",
	"$n raises an eyebrow at you.",
	"You raise an eyebrow at yourself.  That hurt!",
	"$n raises an eyebrow at $mself.  That must have hurt!",
	"$n raises an eyebrow at $t.",
    },

    {
	"faint",
	"You feel dizzy and hit the ground like a board.",
	"$n's eyes roll back in $s head and $e crumples to the ground.",
	"You faint into $S arms.",
	"$n faints into $N's arms.",
	"$n faints into your arms.  How romantic.",
	"You look down at your condition and faint.",
	"$n looks down at $s condition and faints dead away.",
	"$n faints into $t's arms.",
    },
   
    {  
	"fakerep",
	"You report: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"$n reports: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"You report: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"$n reports: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"$n reports: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"You report: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"$n reports: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
	"$n reports: 12874/13103 hp 9238/10230 mana 2483/3451 mv 2.31E13 xp.",
    },

    {
	"fart",
	"Where are your manners?",
	"$n lets off a real rip-roarer ... a greenish cloud envelops $n!",
	"You fart at $M.  Boy, you are sick.",
	"$n farts in $N's direction.  Better flee before $e turns to you!",
	"$n farts in your direction.  You gasp for air.",
	"You fart at yourself.  You deserve it.",
	"$n farts at $mself.  Better $m than you.",
	"$n farts in $t's direction.  Better flee before $e turns to you!",
    },

    {
	"flash",
	"You flash your naked body at the gawking crowd.",
	"$n flashes $s naked body at everyone.  Gasp!",
	"You flash your naked body at $M.",
	"$n flashes $s naked body at $N.  Aren't you jealous?",
	"$n flashes $s naked body at you.  Everyone else is jealous.",
	"You flash your naked body at yourself.  How strange.",
	"$n is looking down $s shirt and grinning.  Very wierd!",
	"$n flashes $s naked body at $t.  Aren't you jealous?",
    },

    {
	"flip",
	"You flip head over heels.",
	"$n flips head over heels.",
	"You flip $M over your shoulder.",
	"$n flips $N over $s shoulder.",
	"$n flips you over $s shoulder.  Hmmmm.",
	"You tumble all over the room.",
	"$n does some nice tumbling and gymnastics.",
	"$n flips $t over $s shoulder.",
    },

    {
	"flirt",
	"Wink wink!",
	"$n flirts -- probably needs a date, huh?",
	"You flirt with $M.",
	"$n flirts with $N.",
	"$n wants you to show some interest and is flirting with you.",
	"You flirt with yourself.",
	"$n flirts with $mself.  Hoo boy.",
	"$n flirts with $t.",
    },
    
    {
	"flutter",
	"You flutter your eyelashes.",
	"$n flutters $s eyelashes.",
	"You flutter your eyelashes at $M.",
	"$n flutters $s eyelashes in $N's direction.",
	"$n looks at you and flutters $s eyelashes.",
	"You flutter your eyelashes at the thought of yourself.",
	"$n flutters $s eyelashes at no one in particular." ,
	"$n flutters $s eyelashes in $t's direction.",
    },

    {
	"fondle",
	"Who needs to be fondled?",
	NULL,
	"You fondly fondle $M.",
	"$n fondly fondles $N.",
	"$n fondly fondles you.",
	"You fondly fondle yourself, feels funny doesn't it?",
	"$n fondly fondles $mself - this is going too far!!",
	"$n fondly fondles $t.",
    },

    {
	"french",
	"Kiss whom?",
	NULL,
	"You give $N a long and passionate kiss.",
	"$n kisses $N passionately.",
	"$n gives you a long and passionate kiss.",
	"You gather yourself in your arms and try to kiss yourself.",
	"$n makes an attempt at kissing $mself.",
	"$n kisses $t passionately.",
    },

    {
	"frown",
	"What's bothering you ?",
	"$n frowns.",
	"You frown at what $E did.",
	"$n frowns at what $E did.",
	"$n frowns at what you did.",
	"You frown at yourself.  Poor baby.",
	"$n frowns at $mself.  Poor baby.",
	"$n frowns at what $t did.",
    },

    {
	"fume",
	"You grit your teeth and fume with rage.",
	"$n grits $s teeth and fumes with rage.",
	"You stare at $M, fuming.",
	"$n stares at $N, fuming with rage.",
	"$n stares at you, fuming with rage!",
	"That's right - hate yourself!",
	"$n clenches $s fists and stomps his feet, fuming with anger.",
	"$n stares at $t, fuming with rage.",
    },

    {
        "garth",
        "You will give your weapons away....NOT.",
	"$n yells 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
	"You yell 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
	"$n yells 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
	"$n yells 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
	"You yell 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
	"$n yells 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
	"$n yells 'WAYNE'S WORLD WAYNE'S WORLD -- PARTY TIME!  EXCELLENT!'",
    },
    
    {
	"gasp",
	"You gasp in astonishment.",
	"$n gasps in astonishment.",
	"You gasp as you realize what $E did.",
	"$n gasps as $e realizes what $N did.",
	"$n gasps as $e realizes what you did.",
	"You look at yourself and gasp!",
	"$n takes one look at $mself and gasps in astonisment!",
	"$n gasps as $e realizes what $t did.",
    },

    {
	"gawk",
	"You gawk at evryone around you.",
	"$n gawks at everyone in the room.",
	"You gawk at $M.",
	"$n gawks at $N.",
	"$n gawks at you.",
	"You gawk as you think what you must look like to others.",
	"$n is gawking again.  What is on $s mind?",
	"$n gawks at $t.",
    },
 
    {
	"german",
	"You speak your best German in hopes of getting a beer.",
	"$n says 'Du bist das hundchen!'  What the hell?",
	"You speak your best German to $M in hopes of getting a beer.",
	"$n says to $N, 'Du bist das hundchen!'  What the hell?",
	"$n says to you, 'Du bist das hundchen!'  What the hell?",
	"You speak your best German in hopes of getting a beer.",
	"$n says 'Du bist das hundchen!'  What the hell?",
	"$n says to $t, 'Du bist das hundchen!'  What the hell?",
    },
    
    {
	"ghug",
	"GROUP HUG!  GROUP HUG!",
	"$n hugs you all in a big group hug.  How sweet!",
	"GROUP HUG!  GROUP HUG!",
	"$n hugs you all in a big group hug.  How sweet!",
	"$n hugs you all in a big group hug.  How sweet!",
	"GROUP HUG!  GROUP HUG!",
	"$n hugs you all in a big group hug.  How sweet!",
	"$n hugs you all in a big group hug.  How sweet!",
    },
    
    {
	"giggle",
	"You giggle.",
	"$n giggles.",
	"You giggle in $S's presence.",
	"$n giggles at $N's actions.",
	"$n giggles at you.  Hope it's not contagious!",
	"You giggle at yourself.  You must be nervous or something.",
	"$n giggles at $mself.  $e must be nervous or something.",
	"$n giggles at $t's actions.",
    },

    {
	"glare",
	"You glare at nothing in particular.",
	"$n glares around $m.",
	"You glare icily at $M.",
	"$n glares at $N.",
	"$n glares icily at you, you feel cold to your bones.",
	"You glare icily at your feet, they are suddenly very cold.",
	"$n glares at $s feet, what is bothering $m?",
	"$n glares at $t.",
    },

    {
	"goose",
	"You honk like a goose.",
	"$n honks like a goose.",
	"You goose $S luscious bottom.",
	"$n gooses $N's soft behind.",
	"$n squeezes your tush.  Oh my!",
	"You goose yourself.",
	"$n gooses $mself.  Yuck.",
	"$n gooses $t's soft behind.",
    },

    {
	"grimace",
	"You contort your face in disgust.",
	"$n grimaces is disgust.",
	"You grimace in disgust at $M.",
	"$n grimaces in disgust at $N.",
	"$n grimaces in disgust at you.",
	"You grimace at yourself in disgust.",
	"$n grimaces at $mself in disgust.",
	"$n grimaces in disgust at $t.",
    },

    {
	"grin",
	"You grin evilly.",
	"$n grins evilly.",
	"You grin evilly at $M.",
	"$n grins evilly at $N.",
	"$n grins evilly at you.  Hmmm.  Better keep your distance.",
	"You grin at yourself.  You must be getting very bad thoughts.",
	"$n grins at $mself.  You must wonder what's in $s mind.",
	"$n grins evilly at $t.",
    },

    {
	"groan",
	"You groan loudly.",
	"$n groans loudly.",
	"You groan at the sight of $M.",
	"$n groans at the sight of $N.",
	"$n groans at the sight of you.",
	"You groan as you realize what you have done.",
	"$n groans as $e realizes what $e has done.",
	"$n groans at the sight of $t.",
    },

    {
	"grope",
	"Whom do you wish to grope?",
	NULL,
	"Well, what sort of noise do you expect here?",
	"$n gropes $N.",
	"$n gropes you.",
	"You grope yourself - YUCK.",
	"$n gropes $mself - YUCK.",
	"$n gropes $t.",
    },

    {
	"grovel",
	"You grovel in the dirt.",
	"$n grovels in the dirt.",
	"You grovel before $M.",
	"$n grovels in the dirt before $N.",
	"$n grovels in the dirt before you.",
	"That seems a little silly to me.",
	"$n grovels in the dirt.",
	"$n grovels in the dirt before $t.",
    },

    {
	"growl",
	"Grrrrrrrrrr ...",
	"$n growls.",
	"Grrrrrrrrrr ... take that, $N!",
	"$n growls at $N.  Better leave the room before the fighting starts.",
	"$n growls at you.  Hey, two can play it that way!",
	"You growl at yourself.  Boy, do you feel bitter!",
	"$n growls at $mself.  This could get interesting...",
	"$n growls at $t.  Better leave the room before the fighting starts.",
    },

    {
	"grumble",
	"You grumble.",
	"$n grumbles.",
	"You grumble to $M.",
	"$n grumbles to $N.",
	"$n grumbles to you.",
	"You grumble under your breath.",
	"$n grumbles under $s breath.",
	"$n grumbles to $t.",
    },

    {
	"grunt",
	"GRNNNHTTTT.",
	"$n grunts like a pig.",
	"GRNNNHTTTT.",
	"$n grunts to $N.  What a pig!",
	"$n grunts to you.  What a pig!",
	"GRNNNHTTTT.",
	"$n grunts to nobody in particular.  What a pig!",
	"$n grunts to $t.  What a pig!",
    },

    {
	"hand",
	"Kiss whose hand?",
	NULL,
	"You kiss $S hand.",
	"$n kisses $N's hand.  How continental!",
	"$n kisses your hand.  How continental!",
	"You kiss your own hand.",
	"$n kisses $s own hand.",
	"$n kisses $t's hand.  How continental!",
    },

    {
	"hangover",
	"You pop a few aspirin and put on your sunglasses.  Ow, your head hurts!",
	"$n holds $s head and says 'Quit breathing so loud!'",
	"Won't $N be quiet?  Your head is gonna split in two if $E keeps talking!",
	"$n complains to $N 'Be quiet!  I have a hangover!",
	"$n complains to you 'Be quiet!  I have a hangover!",
	"You shoosh yourself.  Be quiet!  You have a hangover!",
	"$n shooshes $mself.  Wow, what a hangover that must be!",
	"$n complains to $t 'Be quiet!  I have a hangover!",
    },

    {
	"happy",
	"You smile wide like Cindy Brady.",
	"$n is as happy as a maggot on a piece of rotten meat.",
	"You smile wide at $M like Cindy Brady.",
	"$n is as happy as a maggot on a piece of rotten meat.",
	"$n is as happy as a maggot on a piece of rotten meat.",
	"You smile wide like Cindy Brady.",
	"$n is as happy as a maggot on a piece of rotten meat.",
	"$n is as happy as a maggot on a piece of rotten meat.",
    },

    {
	"heal",
	"You start yelling for a heal!",
	"$n yells 'Hey, how about a heal? I'm DYING here!'",
	"You start yelling at $N for a heal!",
	"$n yells 'Hey $N, how about a heal? I'm DYING here!'",
	"$n yells 'Hey $N, how about a heal? I'm DYING here!'",
	"You start yelling for a heal!",
	"$n yells 'Hey, how about a heal? I'm DYING here!'",
	"$n yells 'Hey $t, how about a heal? I'm DYING here!'",
    },

    {   
	"hello",
	"You say hello to everyone in the room.",
	"$n says hello to everyone in the room.",
	"You tell $M how truly glad you are to see $M.",
	"$n tells $N 'Hi!'",
	"$n tells you how truly glad $e is that you are here.",
	"You greet yourself enthusiastically.",
	"$n greets $mself enthusiastically.  How odd.",
	"$n tells $t 'Hi!'",
    },

    {
	"highfive",
	"You jump in the air...oops, better get someone else to join you.",
	"$n jumps in the air by $mself.  Is $e a cheerleader, or just daft?",
	"You jump in the air and give $M a big highfive!",
	"$n jumps in the air and gives $N a big highfive!",
	"$n jumps in the air and gives you a big highfive!",
	"You jump in the air and congratulate yourself!",
	"$n jumps in the air and gives $mself a big highfive!  Wonder what $e did?",
	"$n jumps in the air and gives $t a big highfive!",
    },

    {
	"hmm",
	"You Hmmmm out loud.",
	"$n thinks, 'Hmmmm.'",
	"You gaze thoughtfully at $M and say 'Hmmm.'",
	"$n gazes thoughtfully at $N and says 'Hmmm.'",
	"$n gazes thoughtfully at you and says 'Hmmm.'",
	"You Hmmmm out loud.",
	"$n thinks, 'Hmmmm.'",
	"$n gazes thoughtfully at $t and says 'Hmmm.'",
    },

    {
        "hologram",
        "You snap your fingers and create an illusion.",
        "$n leaves south.",
        "You snap your fingers and create an illusion.",
        "$n leaves south.",
        "$n waves at you and leaves south.",
        "You snap your fingers and create an illusion of yourself.",
        "Suddenly, there are 2 $n's standing here!",
        "$n leaves south.",
    },
    
    {
	"hop",
	"You hop around like a little kid.",
	"$n hops around like a little kid.",
        "You hop into $N's lap.",
        "$n hops into $N's lap.",
        "$n hops into your lap.",
	"You hop around like a little kid.",
	"You hop around like a little kid.",
      "You hop into $t's lap.",
    },

    {
	"hug",
	"Hug whom?",
	NULL,
	"You hug $M.",
	"$n hugs $N.",
	"$n hugs you.",
	"You hug yourself.",
	"$n hugs $mself in a vain attempt to get friendship.",
	"$n hugs $t.",
    },

    {
	"hum",
	"Hmm Hmm Hmm Hmmmmmmm.",
	"$n hums like a bee with a chest cold.",
	"You hum a little ditty for $M.  Hmm Hmm Hmm Hmmmmmm.",
	"$n hums a little ditty for $N.  Hmm Hmm Hmm Hmmmmmm.",
	"$n hums a little ditty for you.  Hmm Hmm Hmm Hmmmmmm.",
	"Hmm Hmm Hmmmmmmm.",
	"$n hums like a bee with a chest cold.",
	"$n hums a little ditty for $t.  Hmm Hmm Hmm Hmmmmmm.",
    }, 

    {
	"invite",
	"You speak your best French in hopes of getting lucky.",
	"$n tells you, 'Voulez-vous couche avec moi ce soir?'",  
	"You speak your best French to $M in hopes of getting lucky.",
	"$n tells $N, 'Voulez-vous couche avec moi ce soir?'",
	"$n tells you, 'Voulez-vous couche avec moi ce soir?'",
	"You speak your best French in hopes of getting lucky, with yourself???",
	"$n says to $mself, 'Voulez-vous couche avec moi ce soir?'",
	"$n tells $t, 'Voulez-vous couche avec moi ce soir?'",
    },
  
    {
        "jsave",
        "You profess 'Jesus saves!  But Gretsky recovers...he scores!'",
        "$n announces 'Jesus saves!  But Gretsky recovers...he scores!'",
        "You profess 'Jesus saves!  But Gretsky recovers...he scores!'",
        "$n announces 'Jesus saves!  But Gretsky recovers...he scores!'",
        "$n announces to you 'Jesus saves!  But Gretsky recovers...he scores!'",
        "You profess 'Jesus saves!  But Gretsky recovers...he scores!'",
        "$n announces 'Jesus saves!  But Gretsky recovers...he scores!'",
        "$n announces 'Jesus saves!  But Gretsky recovers...he scores!'",
    },

    {
	"kiss",
	"Isn't there someone you want to kiss?",
	NULL,
	"You kiss $M.",
	"$n kisses $N.",
	"$n kisses you.",
	"All the lonely people :(",
	NULL,
	"$n kisses $t.",
    },

    {
	"laugh",
	"You laugh.",
	"$n laughs.",
	"You laugh at $N mercilessly.",
	"$n laughs at $N mercilessly.",
	"$n laughs at you mercilessly.  Hmmmmph.",
	"You laugh at yourself.  I would, too.",
	"$n laughs at $mself.  Let's all join in!!!",
	"$n laughs at $t mercilessly.",
    },

    {
	"lag",
	"You complain about the terrible lag.",
	"$n starts complaining about the terrible lag.",
	"You complain to $N about the terrible lag.",
	"$n complains to $N about the terrible lag.",
	"$n complains to you about the terrible lag.",
	"You start muttering about the awful lag.",
	"$n starts muttering about the awful lag.",
	"$n complains to $t about the terrible lag.",
    },

    {
	"lick",
	"You lick your lips and smile.",
	"$n licks $s lips and smiles.",
	"You lick $M.",
	"$n licks $N.",
	"$n licks you.",
	"You lick yourself.",
	"$n licks $mself - YUCK.",
	"$n licks $t.",
    },

    {
	"love",
	"You love the whole world.",
	"$n loves everybody in the world.",
	"You tell your true feelings to $N.",
	"$n whispers softly to $N.",
	"$n whispers to you sweet words of love.",
	"Well, we already know you love yourself (lucky someone does!)",
	"$n loves $mself, can you believe it ?",
	"$n whispers softly to $t.",
    },

    {
	"lust",
	"You are getting lusty feelings!",
	"$n looks around lustily.",
	"You stare lustily at $N.",
	"$n stares lustily at $N.",
	"$n stares lustily at you.",
	"You stare lustily at...youself?",
	"$n looks $mself up and down lustily.",
	"$n stares lustily at $t.",
    },

    {
	"maim",
	"Who do you want to maim?",
	"$n is looking for someone to maim.",
	"You maim $M with your dull fingernails.",
	"$n raises $s hand and tries to maim $N to pieces.",
	"$n raises $s hand and paws at you.  You've been maimed!",
	"You maim yourself with your dull fingernails.",
	"$n raises $s hand and maims $mself to pieces.",
	"$n raises $s hand and tries to maim $t to pieces.",
    },

    {
	"marvelous",
	"You say 'Ah dahling, you look MAHVELLOUS!'",
	"$n says 'Ah dahling, you look MAHVELLOUS!'",
	"You say to $M, 'Ah dahling, you look MAHVELLOUS!'",
	"$n says to $N, 'Ah dahling, you look MAHVELLOUS!'",
	"$n says to you, 'Ah dahling, you look MAHVELLOUS!'",
	"You say 'Ah dahling, I look MAHVELLOUS!'",
	"$n says 'Ah dahling, I look MAHVELLOUS!'",
	"$n says to $t, 'Ah dahling, you look MAHVELLOUS!'",
    },
    
    {
	"massage",
	"Massage what?  Thin air?",
	NULL,
	"You gently massage $N's shoulders.",
	"$n massages $N's shoulders.",
	"$n gently massages your shoulders.  Ahhhhhhhhhh ...",
	"You practice yoga as you try to massage yourself.",
	"$n gives a show on yoga positions, trying to massage $mself.",
	"$n massages $t's shoulders.",
    },

    {
	"meow",
	"MEOW.",
	"$n meows.  What's $e going to do next, wash $mself with $s tongue?",
	"You meow at $M, hoping $E will give you some milk.",
	"$n meows at $N, hoping $E will give $m some milk. ",
	"$n meows at you.  Maybe $e wants some milk.",
	"You meow like a kitty cat.",
	"$n meows like a kitty cat."
	"$n meows at $t, hoping they will give $m some milk. ",
    },

    {
	"mmm",
	"You go mmMMmmMMmmMMmm.",
	"$n says 'mmMMmmMMmmMMmm.'",
	"You go mmMMmmMMmmMMmm.",
	"$n says 'mmMMmmMMmmMMmm.'",
	"$n thinks of you and says, 'mmMMmmMMmmMMmm.'",
	"You think of yourself and go mmMMmmMMmmMMmm.",
	"$n thinks of $mself and says 'mmMMmmMMmmMMmm.'",
	"$n says 'mmMMmmMMmmMMmm.'",
    },
    
    {
	"moan",
	"You start to moan.",
	"$n starts moaning.",
	"You moan for the loss of $m.",
	"$n moans for the loss of $N.",
	"$n moans at the sight of you.  Hmmmm.",
	"You moan at yourself.",
	"$n makes $mself moan.",
	"$n moans for the loss of $t.",
    },

    {
        "mooch",
        "You beg for money, weapons, coins.",
        "$n says 'Spare change?'",
        "You beg $N for money, weapons, coins.",
        "$n begs you for favors of the insidious type...",
        "$n begs you for favors of the insidious type...",
        "You beg for money, weapons, coins.",
        "$n says 'Spare change?'",
        "You beg $t for money, weapons, coins.",
    },
    
    {
	"moocow",
	"You make cow noises.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises at $M.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow at $N.",
	"$n looks at you and Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises at $t.  Mooooooooooooooooooo!",
    },
    
    {
	"smoon",
	"Gee your butt is big.",
	"$n moons the entire room.",
	"You show your big butt to $M.",
	"$n shows $s big butt to $N.  Find a paddle, quick!",
	"$n shows $s big ugly butt to you.  How do you like it?",
	"You moon yourself.",
	"$n moons $mself.  Ugliest butt you ever saw.",
	"$n shows $s big butt to $t.  Find a paddle, quick!",
    },
    
    {
	"mosh",
	"You MOSH insanely about the room.",
	"$n MOSHES insanely about the room.",
	"You MOSH $M into a pile of Jello pudding.  Ewww!",
	"$n MOSHES $N into Jello pudding.  Ah, the blood!",
	"$n MOSHES you into the ground.",
	"You MOSH yourself.  Ah the blood!",
	"$n MOSHES and MOSHES and MOSHES and MOSHES...",
	"$n MOSHES $t into Jello pudding.  Ah, the blood!",
    },
    
    {
	"muhaha",
	"You laugh diabolically.  MUHAHAHAHAHAHA!.",
	"$n laughs diabolically.  MUHAHAHAHAHAHA!..",
	"You laugh at $M diabolically.  MUHAHAHAHAHAHA!..",
	"$n laughs at $N diabolically.  MUHAHAHAHAHAHA!..",
	"$n laughs at you diabolically.  MUHAHAHAHAHAHA!..",
	"Muhaha at yourself??  Wierd.",
	NULL,
	"$n laughs at $t diabolically.  MUHAHAHAHAHAHA!..",
    },

    {
	"mwalk",
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.",      
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
    },

    {
        "nail",
        "You nibble nervously on your nails.",
        "$n nibbles nervously on $s fingernails.",
        "You nibble nervously on your nails.",
        "$n nibbles nervously on $s fingernails.",
        "$n nibbles nervously on your fingernails.  Yuck!",
        "You nibble nervously on your nails.",
        "$n nibbles nervously on $s fingernails.",
        "$n nibbles nervously on $s fingernails.",
    },
    
    {
	"nasty",
	"You do the best imitation of the nasty you can.",
	"$n does the nasty solo? -- wow.",
	"You do the nasty with $M.",
	"$n does the nasty with $N.  Find a firehose quick!",
	"$n does the nasty with you.  How do you like it?",
	"You do the nasty with yourself.",
	"$n does the nasty with $mself.  Hoo boy.",
	"$n does the nasty with $t.  Find a firehose quick!",
    },
    
    {
	"newbie",
	"You inform everyone that you're a newbie.",
	"$n says 'Hi, I'm a newbie, please help!'",
	"You ask $N to help you because you're a newbie.",
	"$n asks $N to help $m, because $e's a newbie.",
	"$n asks you to help $m, because $e's a newbie.",
	NULL,
	NULL,
	"$n asks $t to help $m, because $e's a newbie.",
    },

    {
	"nibble",
	"Nibble on whom?",
	NULL,
	"You nibble on $N's ear.",
	"$n nibbles on $N's ear.",
	"$n nibbles on your ear.",
	"You nibble on your OWN ear.",
	"$n nibbles on $s OWN ear.",
	"$n nibbles on $t's ear.",
    },

    {
	"nod",
	"You nod affirmative.",
	"$n nods affirmative.",
	"You nod in recognition to $M.",
	"$n nods in recognition to $N.",
	"$n nods in recognition to you.  You DO know $m, right?",
	"You nod at yourself.  Are you getting senile?",
	"$n nods at $mself.  $e must be getting senile.",
	"$n nods in recognition to $t.",
    },

    {
	"nose",
	"You wiggle your nose.",
	"$n wiggles $s nose.",
	"You tweek $S nose.",
	"$n tweeks $N's nose.",
	"$n tweeks your nose.",
	"You tweek your own nose!",
	"$n tweeks $s own nose!",
	"$n tweeks $t's nose.",
    },

    {
	"nudge",
	"Nudge whom?",
	NULL,
	"You nudge $M.",
	"$n nudges $N.",
	"$n nudges you.",
	"You nudge yourself, for some strange reason.",
	"$n nudges $mself, to keep $mself awake.",
	"$n nudges $t.",
    },

    {
	"nuke",
	"Who do you want to nuke?",
	"$n tries to put something in the microwave.",
	"You envelop $M in a mushroom cloud.",
	"$n detonates a nuclear warhead at $N.  Uh oh!",
	"$n envelops you in a mushroom cloud.",
	"You nuke yourself.",
	"$n puts $mself in the microwave.",
	"$n detonates a nuclear warhead at $t.  Uh oh!",
    },

    {
	"nuzzle",
	"Nuzzle whom?",
	NULL,
	"You nuzzle $S neck softly.",
	"$n softly nuzzles $N's neck.",
	"$n softly nuzzles your neck.",
	"I'm sorry, friend, but that's impossible.",
	NULL,
	"$n softly nuzzles $t's neck.",
    },

    {
	"ogle",
	"Whom do you want to ogle?",
	NULL,
	"You ogle $M like $E was a piece of meat.",
	"$n ogles $N.  Maybe you should leave them alone for awhile?",
	"$n ogles you.  Guess what $e is thinking about?",
	"You ogle yourself.  You may just be too wierd for this mud.",
	"$n ogles $mself.  Better hope that $e stops there.",
	"$n ogles $t.  Maybe you should leave them alone for awhile?",
    },

    {
	"ohno",
	"Oh no!  You did it again!",
	"Oh no!  $n did it again!",
	"You exclaim to $M, 'Oh no!  I did it again!'",
	"$n exclaims to $N, 'Oh no!  I did it again!'",
	"$n exclaims to you, 'Oh no!  I did it again!'",
	"You exclaim to yourself, 'Oh no!  I did it again!'",
	"$n exclaims to $mself, 'Oh no!  I did it again!'",
	"$n exclaims to $t, 'Oh no!  I did it again!'",
    },
    
    {
	"oink",
	"You make pig noises.  OINK!",
	"$n oinks 'OINK OINK OINK!'",
	"You make pig noises at $M.  OINK!",
	"$n oinks at $N: 'OINK OINK OINK!'",
	"$n oinks at you: 'OINK OINK OINK!'",
	"You make pig noises.  OINK!",
	"$n oinks 'OINK OINK OINK!'",
	"$n oinks at $t: 'OINK OINK OINK!'",
    },
 
    {
        "ooo",
        "You go ooOOooOOooOOoo.",
        "$n says, 'ooOOooOOooOOoo.'",
        "You go ooOOooOOooOOoo.",
        "$n says, 'ooOOooOOooOOoo.'",
        "$n thinks of you and says, 'ooOOooOOooOOoo.'",
        "You go ooOOooOOooOOoo.",
        "$n says, 'ooOOooOOooOOoo.'",
        "$n says, 'ooOOooOOooOOoo.'",
    },

    {
	"pat",
	"Pat whom?",
	NULL,
	"You pat $N on $S head.",
	"$n pats $N on $S head.",
	"$n pats you on your head.",
	"You pat yourself on your head.",
	"$n pats $mself on the head.",
	"$n pats $t on their head.",
    },

    {
	"peck",
	"You peck for seeds on the ground.",
	"$n pecks for seeds on the ground.",
	"You give $M a little peck on the cheek.",
	"$n gives $N a small peck on the cheek.",
	"$n gives you a sweet peck on the cheek.",
	"You kiss your own pectoral muscles.",
	"$n pecks $mself on $s pectoral muscles.",
	"$n gives $t a small peck on the cheek.",
    },

    {
	"peer",
	"You peer intently about your surroundings.",
	"$n peers intently about the area, looking for thieves no doubt.",
	"You peer at $M quizzically.",
	"$n peers at $N quizzically.",
	"$n peers at you quizzically.",
	"You peer intently about your surroundings.",
	"$n peers intently about the area, looking for thieves no doubt.",
	"$n peers at $t quizzically.",
    },

    {
	"pinch",
	"You toss a pinch of salt over your shoulder.",
	"$n tosses a pinch of salt over $s shoulder.",
	"You pinch $S rosy cheeks.",
	"$n pinches $N's rosy cheeks.",
	"$n pinches your chubby cheeks.",
	"You need a pinch of salt.",
	"$n needs a pinch of salt.",
	"$n pinches $t's rosy cheeks.",
    },

    {
        "ping",
        "Ping who?",
        NULL,
        "You ping $N.  $N is unreachable.",
        "$n pings $N.  $N is unreachable.",
        "$n pings you.  Is your server dead or alive?",
        "You ping yourself. Ooof!",
        "$n pings $mself. Ooof!",
        "$n pings $t.  $t is unreachable.",
    },

    {
	"point",
	"Point at whom?",
	NULL,
	"You point at $M accusingly.",
	"$n points at $N accusingly.",
	"$n points at you accusingly.",
	"You point proudly at yourself.",
	"$n points proudly at $mself.",
	"$n points at $t accusingly.",
    },

    {
	"poke",
	"Poke whom?",
	NULL,
	"You poke $M in the ribs.",
	"$n pokes $N in the ribs.",
	"$n pokes you in the ribs.",
	"You poke yourself in the ribs, feeling very silly.",
	"$n pokes $mself in the ribs, looking very sheepish.",
	"$n pokes $t in the ribs.",
    },

    {
	"ponder",
	"You ponder the question.",
	"$n sits down and thinks deeply.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
    },

    {
	"possum",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"$n hits the ground... DEAD.",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"$n hits the ground... DEAD.",
    },

    {
	"poupon",
	"You say 'Pardon me, do you have any Grey Poupon?'.",
	"$n says 'Pardon me, do you have any Grey Poupon?'",
	"You say to $M, 'Pardon me, do you have any Grey Poupon?'.",
	"$n says to $N, 'Pardon me, do you have any Grey Poupon?'",
	"$n says to you, 'Pardon me, do you have any Grey Poupon?'",
	"You say 'Pardon me, do you have any Grey Poupon?'.",
	"$n says 'Pardon me, do you have any Grey Poupon?'",
	"$n says to $t, 'Pardon me, do you have any Grey Poupon?'",
    },

    {
	"pout",
	"Ah, don't take it so hard.",
	"$n pouts.",
	"You pout at $M.",
	"$n pouts at $N.",
	"$n pouts at you.",
	"Ah, don't take it so hard.",
	"$n pouts.",
	"$n pouts at $t.",
    },

    {   
	"pretend",
	"You pretend you are a GOD, and slay everyone in sight!",
	"$n is pretending $e is an implementor again.  *sigh*",
	"You pretend you are a GOD, and demote $M to level 1.",
	"$n pretends $e is a GOD, and says, '$N, you're demoted to level 1!'",
	"$n pretends $e is a GOD, and says, 'You are demoted to level 1!'",
	"You pretend you are an implementor, and you demote yourself to level 1.",
	"$n pretends $e is a GOD, and demotes $mself to level 1.",
	"$n pretends $e is a GOD, and says, '$t, you're demoted to level 1!'",
    },

    {
	"puke",
	"You puke ... chunks everywhere!",
	"$n pukes.",
	"You puke on $M.",
	"$n pukes on $N.",
	"$n spews vomit and pukes all over your clothing!",
	"You puke on yourself.",
	"$n pukes on $s clothes.",
	"$n pukes on $t.",
    },

    {
	"purr",
	"MMMMEEEEEEEEOOOOOOOOOWWWWWWWWWWWW.",
	"$n purrs contentedly.",
	"You purr contentedly in $M lap.",
	"$n purrs contentedly in $N's lap.",
	"$n purrs contentedly in your lap.",
	"You purr at yourself.",
	"$n purrs at $mself.  Must be a cat thing.",
	"$n purrs contentedly in $t's lap.",
    },

    {
	"raise",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"$n raises $s hand in response to you.",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"$n raises $s hand in response.",
    },
   
    {   
	"renandstimpy",
	"You say, 'Oh Happy Happy, Joy Joy!'",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!'",
	"You exclaim, 'Oh Happy Happy, Joy Joy!' at the mere thought of $M.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' as $e sees $N enter the room.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' when $e sees you approach.",
	"You exclaim, 'Oh Happy Happy, Joy Joy!' at the thought of yourself.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' at the thought of $mself.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' as $e sees $t enter the room.",
    },  

    {
	"rofl",
	"You roll on the floor laughing hysterically.",
	"$n rolls on the floor laughing hysterically.",
	"You laugh your head off at $S remark.",
	"$n rolls on the floor laughing at $N's remark.",
	"$n can't stop laughing at your remark.",
	"You roll on the floor and laugh at yourself.",
	"$n laughs at $mself.  Join in the fun.",
	"You laugh your head off at $t's remark.",
    },
    
    {
	"rolleyes",
	"You roll your eyes.",
	"$n rolls $s eyes.",
	"You roll your eyes at $M.",
	"$n rolls $s eyes at $N.",
	"$n rolls $s eyes at you.",
	"You roll your eyes at yourself.",
	"$n rolls $s eyes at $mself.",
	"$n rolls $s eyes at $t.",
    },

    {
	"roll2",
	"You roll your eyes and make a repetative motion near your crotch.",
	"$n rolls $s eyes and makes a repetative motion near $s crotch.",
	"You roll your eyes at $M and make a repetative motion near your crotch.",
	"$n rolls $s eyes at $N and makes a repetative motion near $s crotch.",
	"$n rolls $s eyes at you and makes a repetative motion near $s crotch.",
	"You make a repetative motion near your crotch...you find you quite like it!",
	"$n starts making a repetative motion near $s crotch...how strange...",
	"$n rolls $s eyes at $t and makes a repetative motion near $s crotch.",
    },

    {
	"rub",
	"You rub your eyes.  How long have you been at this?",
	"$n rubs $s eyes.  $n must have been playing all day.",

"You rub your eyes.  Has $N been playing as long as you have?",
	"$n rubs $s eyes.  $n must have been playing all day.",
	"$n rubs $s eyes.  Have you been playing as long as $m?",
	"You rub your eyes.  How long have you been at this?",
	"$n rubs $s eyes.  $n must have been playing all day.",
	"$n rubs $s eyes.  $n must have been playing all day.",
    },

    {
	"ruffle",
	"You've got to ruffle SOMEONE.",
	NULL,
	"You ruffle $N's hair playfully.",
	"$n ruffles $N's hair playfully.",
	"$n ruffles your hair playfully.",
	"You ruffle your hair.",
	"$n ruffles $s hair.",
	"$n ruffles $t's hair playfully.",
    },

    {
	"runaway",
	"You scream 'RUN AWAY! RUN AWAY!'.",
	"$n screams 'RUN AWAY! RUN AWAY!'.",
	"You scream '$N, QUICK! RUN AWAY!'.",
	"$n screams '$N, QUICK! RUN AWAY!'.",
	"$n screams '$N, QUICK! RUN AWAY!'.",
	"You desperately look for somewhere to run to!",
	"$n looks like $e's about to run away.",
	"$n screams '$t, QUICK! RUN AWAY!'.",
    },

    {
	"russian",
	"You speak Russian.  Yeah, right.  Dream on.",
	"$n says 'Ya horosho stari malenky koshka.'  Huh?",
	"You speak Russian to $M.  Yeah, right.  Dream on.",
	"$n says to $N 'Ya horosho stari malenky koshka.'  Huh?",
	"$n says to you 'Ya horosho stari malenky koshka.'  Huh?",
	"You speak Russian.  Yeah, right.  Dream on.",
	"$n says 'Ya horosho stari malenky koshka.'  Huh?",
	"$n says to $t 'Ya horosho stari malenky koshka.'  Huh?",
    },

    {
	"sad",
	"You put on a glum expression.",
	"$n looks particularly glum today.  *sniff*",
	"You give $M your best glum expression.",
	"$n looks at $N glumly.  *sniff*  Poor $n.",
	"$n looks at you glumly.  *sniff*   Poor $n.",
	"You bow your head and twist your toe in the dirt glumly.",
	"$n bows $s head and twists $s toe in the dirt glumly.",
	"$n looks at $t glumly.  *sniff*  Poor $n.",
    },

    {
	"salute",
	"You salute smartly.",
	"$n salutes smartly.",
	"You salute $M.",
	"$n salutes $N.",
	"$n salutes you.",
	"Huh?",
	NULL,
	"$n salutes $t.",
    },

    {
	"scowl",
	"You scowl angrilly.",
	"$n scowls angrilly.",
	"You scowl angrilly at $M.",
	"$n scowls angrilly at $N.",
	"$n scowls angrilly at you.",
	"You scowl angrilly at yourself.",
	"$n scowls angrilly at $mself.",
	"$n scowls angrilly at $t.",
    },

    {
	"scream",
	"ARRRRRRRRRRGH!!!!!",
	"$n screams loudly!",
	"ARRRRRRRRRRGH!!!!!  Yes, it MUST have been $S fault!!!",
	"$n screams loudly at $N.  Better leave before $n blames you, too!!!",
	"$n screams at you!  That's not nice!  *sniff*",
	"You scream at yourself.  Yes, that's ONE way of relieving tension!",
	"$n screams loudly at $mself!  Is there a full moon up?",
	"$n screams loudly at $t.  Better leave before $n blames you, too!!!",
    },

    {
	"shake",
	"You shake your head.",
	"$n shakes $s head.",
	"You shake $S hand.",
	"$n shakes $N's hand.",
	"$n shakes your hand.",
	"You are shaken by yourself.",
	"$n shakes and quivers like a bowl full of jelly.",
	"$n shakes $t's hand.",
    },

    {
	"shiver",
	"Brrrrrrrrr.",
	"$n shivers uncomfortably.",
	"You shiver at the thought of fighting $M.",
	"$n shivers at the thought of fighting $N.",
	"$n shivers at the suicidal thought of fighting you.",
	"You shiver to yourself?",
	"$n scares $mself to shivers.",
	"$n shivers at the thought of fighting $t.",
    },

    {
	"shrug",
	"You shrug.",
	"$n shrugs helplessly.",
	"You shrug in response to $s question.",
	"$n shrugs in response to $N's question.",
	"$n shrugs in respopnse to your question.",
	"You shrug to yourself.",
	"$n shrugs to $mself.  What a strange person.",
	"$n shrugs in response to $t's question.",
    },

    {
	"sigh",
	"You sigh.",
	"$n sighs loudly.",
	"You sigh as you think of $M.",
	"$n sighs at the sight of $N.",
	"$n sighs as $e thinks of you.  Touching, huh?",
	"You sigh at yourself.  You MUST be lonely.",
	"$n sighs at $mself.  What a sorry sight.",
	"$n sighs at the sight of $t.",
    },

    {
	"sing",
	"You raise your clear voice towards the sky.",
	"$n has begun to sing.",
	"You sing a ballad to $m.",
	"$n sings a ballad to $N.",
	"$n sings a ballad to you!  How sweet!",
	"You sing a little ditty to yourself.",
	"$n sings a little ditty to $mself.",
	"$n sings a ballad to $t.",
    },

    {
	"slap",
	"Slap whom?",
	NULL,
	"You slap $M across the face.",
	"$n slaps $N across the face for $S stupidity.",
	"$n slaps you across the face. Don't take it from $m.",
	"You slap yourself.  You deserve it.",
	"$n slaps $mself.  Why don't you join in?",
	"$n slaps $t across the face for their stupidity.",
    },

    {
	"slobber",
	"You slobber all over the floor.",
	"$n slobbers all over the floor.",
	"You slobber all over $M.",
	"$n slobbers all over $N.",
	"$n slobbers all over you.",
	"You slobber all down your front.",
	"$n slobbers all over $mself.",
	"$n slobbers all over $t.",
    },

    {
	"slut",
	"You act like a total slut.",
	"$n lounges about looking like a total slut.",
	"You come on to $N, breathing sensuality.",
	"$n comes on to $N, perhaps he's trying to take Rosario's reputation?",
	"$n comes on to you, $n is like a living embodiment of sensuality.",
	"You sigh, and say 40 partners is average for a 18 yr old, right?",
	"$n sighs, and says 40 partners is about right for an 18 yr old right?",
	"$n comes on to $t, perhaps he's trying to take Rosario's reputation?",
    },

    {
	"smile",
	"You smile happily.",
	"$n smiles happily.",
	"You smile at $M.",
	"$n beams a smile at $N.",
	"$n smiles at you.",
	"You smile at yourself.",
	"$n smiles at $mself.",
	"$n beams a smile at $t.",
    },

    {
	"smirk",
	"You smirk.",
	"$n smirks.",
	"You smirk at $S saying.",
	"$n smirks at $N's saying.",
	"$n smirks at your saying.",
	"You smirk at yourself.  Okay ...",
	"$n smirks at $s own 'wisdom'.",
	"$n smirks at $t's saying.",
    },

    {
	"smoke",
	"You calmly light a cigarette and take a puff.",
	"$n calmly lights a cigarette and take a puff.",
	"You blow smoke into $S eyes.",
	"$n blows smoke into $N's eyes.",
	"$n blows smoke rings into your eyes.",
	"You call down lightning and SMOKE yourself.",
	"$n calls down lightning and SMOKES $mself.",
	"$n blows smoke into $t's eyes.",
    },

    {
	"smooch",
	"You are searching for someone to smooch.",
	"$n is looking for someone to smooch.",
	"You give $M a nice, wet smooch.",
	"$n and $N are smooching in the corner.",
	"$n smooches you passionately on the lips.",
	"You smooch yourself.",
	"$n smooches $mself.  Yuck.",
	"$n and $t are smooching in the corner.",
    },

    {
	"snap",
	"PRONTO ! You snap your fingers.",
	"$n snaps $s fingers.",
	"You snap back at $M.",
	"$n snaps back at $N.",
	"$n snaps back at you!",
	"You snap yourself to attention.",
	"$n snaps $mself to attention.",
	"$n snaps back at $t.",
    },

    {
	"snarl",
	"You grizzle your teeth and look mean.",
	"$n snarls angrily.",
	"You snarl at $M.",
	"$n snarls at $N.",
	"$n snarls at you, for some reason.",
	"You snarl at yourself.",
	"$n snarls at $mself.",
	"$n snarls at $t.",
    },

    {
	"sneer",
	"You sneer in contempt.",
	"$n sneers in contempt.",
	"You sneer at $M in contempt.",
	"$n sneers at $N in contempt.",
	"$n sneers at you in contempt.",
	"You sneer at yourself in contempt.",
	"$n sneers at $mself in contempt.",
	"$n sneers at $t in contempt.",
    },

    {
	"sneeze",
	"Gesundheit!",
	"$n sneezes.",
	"You sneeze all over $M.",
	"$n sneezes all over $N.",
	"$n sneezes all over you.",
	NULL,
	NULL,
	"$n sneezes all over $t.",
    },

    {
	"snicker",
	"You snicker softly.",
	"$n snickers softly.",
	"You snicker with $M about your shared secret.",
	"$n snickers with $N about their shared secret.",
	"$n snickers with you about your shared secret.",
	"You snicker at your own evil thoughts.",
	"$n snickers at $s own evil thoughts.",
	"$n snickers with $t about their shared secret.",
    },

    {
	"sniff",
	"You sniff sadly. *SNIFF*",
	"$n sniffs sadly.",
	"You sniff sadly at the way $E is treating you.",
	"$n sniffs sadly at the way $N is treating $m.",
	"$n sniffs sadly at the way you are treating $m.",
	"You sniff sadly at your lost opportunities.",
	"$n sniffs sadly at $mself.  Something MUST be bothering $m.",
	"$n sniffs sadly at the way $t is treating $m.",
    },

    {
	"snore",
	"Zzzzzzzzzzzzzzzzz.",
	"$n snores loudly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"snort",
	"You snort in disgust.",
	"$n snorts in disgust.",
	"You snort at $M in disgust.",
	"$n snorts at $N in disgust.",
	"$n snorts at you in disgust.",
	"You snort at yourself in disgust.",
	"$n snorts at $mself in disgust.",
	"$n snorts at $t in disgust.",
    },

    {
	"snowball",
	"Whom do you want to throw a snowball at?",
	NULL,
	"You throw a snowball in $N's face.",
	"$n throws a snowball at $N.",
	"$n throws a snowball at you.",
	"You throw a snowball at yourself.",
	"$n throws a snowball at $mself.",
	"$n throws a snowball at $t.",
    },

    {
	"snuggle",
	"Who?",
	NULL,
	"You snuggle up contentedly in $M lap.",
	"$n snuggles up contentedly in $N's lap.",
	"$n snuggles up contentedly in your lap.",
	"You snuggle up, getting ready to sleep.",
	"$n snuggles up, getting ready to sleep.",
	"$n snuggles up contentedly in $t's lap.",
    },

    {
	"spam",
	"You mutter 'SPAM' quietly to yourself.",
	"$n sings 'SPAM SPAM SPAM SPAM SPAM SPAM....'",
	"You hurl a can of SPAM at $M.",
	"$n hurls a can of SPAM at $N.",
	"$n SPAMS you viciously.",
	"You mutter 'SPAM' in the corner quietly.",
	"$n mutters 'SPAM SPAM SPAM SPAM'.  Join in!",
	"$n hurls a can of SPAM at $t.",
    },
    
    {
	"spanish",
	"You speak Spanish or at least you think you do.",
	"$n says 'Naces pendejo, mueres pendejo.'  Uh oh.",
	"You speak Spanish or at least you think you do.",
	"$n says 'Naces pendejo, mueres pendejo.'  Uh oh.",
	"$n says 'Naces pendejo, mueres pendejo.'  Uh oh.",
	"You speak Spanish or at least you think you do.",
	"$n says to $mself 'Naces pendejo, mueres pendejo.'  Uh oh.",
	"$n says 'Naces pendejo, mueres pendejo.'  Uh oh.",
    },

    {
	"spank",
	"Spank whom?",
	NULL,
	"You spank $M playfully.",
	"$n spanks $N playfully.",
	"$n spanks you playfully.  OUCH!",
	"You spank yourself.  Kinky!",
	"$n spanks $mself.  Kinky!",
	"$n spanks $t playfully.",
    },

    {
	"spin",
	"You twirl in a graceful pirouette.",
	"$n twirls in a graceful pirouette.",
	"You spin $M on one finger.",
	"$n spins $N on $s finger.",
	"$n spins you around on $s finger.",
	"You spin yourself around and around and around....",
	"$n spins $mself around and around and around...",
	"$n spins $t on $s finger.",
    },
    
    {
	"squeeze",
	"Where, what, how, whom?",
	NULL,
	"You squeeze $M fondly.",
	"$n squeezes $N fondly.",
	"$n squeezes you fondly.",
	"You squeeze yourself - try to relax a little!",
	"$n squeezes $mself.",
	"$n squeezes $t fondly.",
    },

    {
	"squeak",
	"You squeak like a mouse.",
	"$n squeaks like a mouse.",
	"You squeak at $M.",
	"$n squeaks at $N.  Is $e a man or a mouse?",
	"$n squeaks at $N.  Is $e a man or a mouse?",
	"$n squeaks at you.  Is $e a man or a mouse?",
	"You squeak at yourself like a mouse.",
	"$n squeaks at $t.  Is $e a man or a mouse?",
    },

    {
	"squeal",
	"You squeal with delight.",
	"$n squeals with delight.",
	"You squeal at $M.",
	"$n squeals at $N.  Wonder why?",
	"$n squeals at you.  You must be doing something good.",
	"You squeal at yourself.",
	"$n squeals at $mself.",
	"$n squeals at $t.  Wonder why?",
    },

    {
	"squirm",
	"You squirm guiltily.",
	"$n squirms guiltily.  Looks like $e did it.",
	"You squirm in front of $M.",
	"$n squirms in front of $N.",
	"$n squirms in front of you.  You make $m nervous.",
	"You squirm and squirm and squirm....",
	"$n squirms and squirms and squirm.....",
	"$n squirms in front of $t.",
    },
    
    {
	"squish",
	"You squish your toes into the sand.",
	"$n squishes $s toes into the sand.",
	"You squish $M between your legs.",
	"$n squishes $N between $s legs.",
	"$n squishes you between $s legs.",
	"You squish yourself.",
	"$n squishes $mself.  OUCH.",
	"$n squishes $t between $s legs.",
    },

    {
	"stare",
	"You stare at the sky.",
	"$n stares at the sky.",
	"You stare dreamily at $N, completely lost in $S eyes..",
	"$n stares dreamily at $N.",
	"$n stares dreamily at you, completely lost in your eyes.",
	"You stare dreamily at yourself - enough narcissism for now.",
	"$n stares dreamily at $mself - NARCISSIST!",
	"$n stares dreamily at $t.",
    },

    {
	"stickup",
	"You don a mask and ask for everyone's gold.",
	"$n says 'This is a stickup.  Gimme yer gold, NOW!'",
	"You don a mask and ask for $S gold.",
	"$n says to $N, 'This is a stickup.  Gimme yer gold, NOW!'",
	"$n says to you, 'This is a stickup.  Gimme yer gold, NOW!'",
	"You extort money from yourself.",
	"$n holds $s weapon to $s throat and says 'Ok, me, give me all my money!'",
	"$n says to $t, 'This is a stickup.  Gimme yer gold, NOW!'",
    },
   
    {
	"stretch",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
    },
    
    {
	"strip",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively around the room.",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively at $N.",
	"$n shows $s bare shoulder and glances seductively at you.",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively around the room.",
	"$n shows $s bare shoulder and glances seductively at $t.",
    },

    {
	"strut",
	"Strut your stuff.",
	"$n struts proudly.",
	"You strut to get $S attention.",
	"$n struts, hoping to get $N's attention.",
	"$n struts, hoping to get your attention.",
	"You strut to yourself, lost in your own world.",
	"$n struts to $mself, lost in $s own world.",
	"$n struts, hoping to get $t's attention.",
    },

    {
	"suffer",
	"No xp again?  You suffer at the hands of fate.",
	"$n is suffering.  Looks like $e can't seem to level.",
	"You tell $M how you suffer whenever you're away from $M.",
	"$n tells $N that $e suffers whenever they're apart.",
	"$n tells you that $e suffers whenever you're apart.",
	"No xp again?  You suffer at the hands of fate.",
	"$n is suffering.  Looks like $e can't seem to level.",
	"$n tells $t that $e suffers whenever they're apart.",
    },  
    
    {
	"sulk",
	"You sulk.",
	"$n sulks in the corner.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"surf",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
    },
    
    {
	"swoon",
	"You swoon in ecstacy.",
	"$n swoons in ecstacy.",
	"You swoon in ecstacy at the thought of $M.",
	"$n swoons in ecstacy at the thought of $N.",
	"$n swoons in ecstacy as $e thinks of you.",
	"You swoon in ecstacy.",
	"$n swoons in ecstacy.",
	"$n swoons in ecstacy at the thought of $t.",
    },

    {
	"tackle",
	"You can't tackle the AIR!",
	"$n tries tackle the AIR! Goof!",
	"You run over to $M and bring $M down!",
	"$n runs over to $N and tackles $M to the ground!",
	"$n runs over to you and tackles you to the ground!",
	"You wrap your arms around yourself, and throw yourself to the ground.",
	"$n wraps $s arms around $mself and brings $mself down!?",
	"$n runs over to $t and tackles them to the ground!",
    },
    
    {
	"tap",
	"You tap your foot impatiently.",
	"$n taps $s foot impatiently.",
	"You tap your foot impatiently.  Will $E ever be ready?",
	"$n taps $s foot impatiently as $e waits for $N.",
	"$n taps $s foot impatiently as $e waits for you.",
	"You tap yourself on the head.  Ouch!",
	"$n taps $mself on the head.",
	"$n taps $s foot impatiently as $e waits for $t.",
    },

    {
	"tender",
	"You will enjoy it more if you choose someone to kiss.",
	NULL,
	"You give $M a soft, tender kiss.",
	"$n gives $N a soft, tender kiss.",
	"$n gives you a soft, tender kiss.",
	"You'd better not, people may start to talk!",
	NULL,
	"$n gives $t a soft, tender kiss.",
     },
     
    {
	"thank",
	"Thank you too.",
	NULL,
	"You thank $N heartily.",
	"$n thanks $N heartily.",
	"$n thanks you heartily.",
	"You thank yourself since nobody else wants to !",
	"$n thanks $mself since you won't.",
	"$n thanks $t heartily.",
    },

    {
	"think",
	"You think about times past and friends forgotten.",
	"$n thinks deeply and is lost in thought.",
	"You think about times past and friends forgotten.",
	"$n thinks deeply and is lost in thought.",
	"$n thinks deeply and is lost in thought.  Maybe $e is thinking of you?",
	"You think about times past and friends forgotten.",
	"$n thinks deeply and is lost in thought.",
	"$n thinks deeply and is lost in thought.",
    },
   
    {
	"throttle",
	"Whom do you want to throttle?",
	NULL,
	"You throttle $M till $E is blue in the face.",
	"$n throttles $N about the neck, until $E passes out.  THUNK!",
	"$n throttles you about the neck until you pass out.  THUNK!",
	"That might hurt!  Better not do it!",
	"$n is getting a crazy look in $s eye again.",
	"$n throttles $t about the neck, until they passes out.  THUNK!",
    },

    {
	"tickle",
	"Whom do you want to tickle?",
	NULL,
	"You tickle $N.",
	"$n tickles $N.",
	"$n tickles you - hee hee hee.",
	"You tickle yourself, how funny!",
	"$n tickles $mself.",
	"$n tickles $t.",
    },

    {
	"timeout",
	"You take a 'T' and drink some Gatorade (tm).",
	"$n takes a 'T' and drinks some Gatorade (tm).",
	"You take a 'T' and offer $M some Gatorade (tm).",
	"$n takes a 'T' and offers $N some Gatorade (tm).",
	"$n takes a 'T' and offers you some Gatorade (tm).",
	"You take a 'T' and drink some Gatorade (tm).",
	"$n takes a 'T' and drinks some Gatorade (tm).",
	"$n takes a 'T' and offers $t some Gatorade (tm).",
    },
    
    {
	"tongue",
	"You stick out your tongue.",
	"$n sticks out $s tongue.",
	"You stick your tongue out at $M.",
	"$n sticks $s tongue out at $N.",
	"$n sticks $s tongue out at you.",
	"You stick out your tongue and touch your nose.",
	"$n sticks out $s tongue and touches $s nose.",
	"$n sticks $s tongue out at $t.",
    },

    {
	"torture",
	"You have to torture someone!",
	NULL,
	"You torture $M with rusty weapons, Mwaahhhhh!!",
	"$n tortures $N with rusty weapons, $E must have been REAL bad!",
	"$n tortures you with rusty weapons!  What did you DO!?!",
	"You torture yourself with rusty weapons.  Was it good for you?",
	"$n tortures $mself with rusty weapons.  Looks like $e enjoys it!?",
	"$n tortures $t with rusty weapons, they must have been REAL bad!",
    },

    {   
	"tummy",
	"You rub your tummy and wish you'd bought a pie at the bakery.",
	"$n rubs $s tummy and wishes $e'd bought a pie at the bakery.",
	"You rub your tummy and ask $M for some food.",
	"$n rubs $s tummy and asks $N for some food.",
	"$n rubs $s tummy and asks you for some food.  Please?",
	"You rub your tummy and wish you'd bought a pie at the bakery.",
	"$n rubs $s tummy and wishes $e'd bought a pie at the bakery.",
	"$n rubs $s tummy and asks $t for some food.",
    },

    {
	"tweety",
	"You exclaim 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims 'I TAWT I TAW A PUTTY TAT!!'",
	"You exclaim to $M, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $N, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to you, 'I TAWT I TAW A PUTTY TAT!!'",
	"You exclaim to yourself, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $mself, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $t, 'I TAWT I TAW A PUTTY TAT!!'",
    },
    
    {
	"twiddle",
	"You patiently twiddle your thumbs.",
	"$n patiently twiddles $s thumbs.",
	"You twiddle $S ears.",
	"$n twiddles $N's ears.",
	"$n twiddles your ears.",
	"You twiddle your ears like Dumbo.",
	"$n twiddles $s own ears like Dumbo.",
	"$n twiddles $t's ears.",
    },

    {
	"type",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"$n couldn't type a period if there was only one key on the keyboard.",
    },

    {
	"wager",
	"You wager you can name that tune in ONE NOTE.",
	"$n bets $e can name that tune in ONE NOTE.",
	"You wager $M that you can name that tune in ONE NOTE.",
	"$n bets $N that $e can name that tune in ONE NOTE.",
	"$n bets you that $e can name that tune in ONE NOTE.",
	"You wager you can name that tune in ONE NOTE.",
	"$n bets $e can name that tune in ONE NOTE.",
	"$n bets $t that $e can name that tune in ONE NOTE.",
    },
    
    {
	"wave",
	"You wave.",
	"$n waves happily.",
	"You wave goodbye to $N.",
	"$n waves goodbye to $N.",
	"$n waves goodbye to you.  Have a good journey.",
	"Are you going on adventures as well?",
	"$n waves goodbye to $mself.",
	"$n waves goodbye to $t.",
    },

    {
	"wedgie",
	"You look around for someone to wedgie.",
	"$n is looking around for someone to wedgie!  Run!",
	"You wedgie $M.  Must be fun! ",
	"$n wedgies $N to the heavens.",
	"$n wedgies you!  Ouch!",
	"You delight in pinning your underwear to the sky.",
	"$n wedgies $mself and revels with glee.",
	"$n wedgies $t to the heavens.",
    },

    {
	"whine",
	"You whine like the great whiners of the century.",
	"$n whines 'I want to be a god already.  I need more hitpoints..I...'",
	"You whine to $M like the great whiners of the century.",
	"$n whines to $N 'I want to be an immortal already.  I need more hp...I..'",
	"$n whines to you 'I want to be an immortal already.  I need more hp...I...'",
	"You whine like the great whiners of the century.",
	"$n whines 'I want to be a god already.  I need more hitpoints..I...'",
	"$n whines to $t 'I want to be an immortal already.  I need more hp...I..'",
    },

    {
	"whistle",
	"You whistle appreciatively.",
	"$n whistles appreciatively.",
	"You whistle at the sight of $M.",
	"$n whistles at the sight of $N.",
	"$n whistles at the sight of you.",
	"You whistle a little tune to yourself.",
	"$n whistles a little tune to $mself.",
	"$n whistles at the sight of $t.",
    },

    {
	"wiggle",
	"Your wiggle your bottom.",
	"$n wiggles $s bottom.",
	"You wiggle your bottom toward $M.",
	"$n wiggles $s bottom toward $N.",
	"$n wiggles $s bottom toward you.",
	"You wiggle about like a fish.",
	"$n wiggles about like a fish.",
	"$n wiggles $s bottom toward $t.",
    },

    {
	"wince",
	"You wince.  Ouch!",
	"$n winces.  Ouch!",
	"You wince at $M.",
	"$n winces at $N.",
	"$n winces at you.",
	"You wince at yourself.  Ouch!",
	"$n winces at $mself.  Ouch!",
	"$n winces at $t.",
    },

    {
	"wink",
	"You wink suggestively.",
	"$n winks suggestively.",
	"You wink suggestively at $N.",
	"$n winks at $N.",
	"$n winks suggestively at you.",
	"You wink at yourself ?? - what are you up to ?",
	"$n winks at $mself - something strange is going on...",
	"$n winks at $t.",
    },

    {
	"worship",
	"You worship the powers that be.",
	"$n worships the powers that be.",
	"You drop to your knees in homage of $M.",
	"$n prostrates $mself before $N.",
	"$n believes you are all powerful.",
	"You worship yourself.",
	"$n worships $mself - ah, the conceitedness of it all.",
	"$n prostrates $mself before $t.",
    },

    {
	"wrap",
	"You wrap a present for your love.",
	"$n wraps a present for someone special.",
	"You wrap your legs around $M.",
	"$n wraps $s legs around $N.",
	"$n wraps $s legs around you.  Wonder what's coming next?",
	"You wrap yourself with some paper.",
	"$n wraps $mself with some wrapping paper.  Is it Christmas?",
	"$n wraps $s legs around $t.",
    },

    {
	"yabba",
	"YABBA-DABBA-DOO!",
	"$n hollers 'Hey Wilma -- YABBA DABBA DOO!'",
	"You holler 'Hey $N -- YABBA DABBA DOO!'",
	"$n hollers 'Hey $N -- YABBA DABBA DOO!'",
	"$n hollers 'Hey $N -- YABBA DABBA DOO!'",
	"YABBA-DABBA-DOO!",
	"$n hollers 'Hey Wilma -- YABBA DABBA DOO!'",
	"$n hollers 'Hey $t -- YABBA DABBA DOO!'",
    },
   
    {
	"yahoo",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, kid, that was one in a million!'",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, $N, that was one in a million!'",
	"$n exclaims, 'YAHOO!  Great shot, $N, that was one in a million!'",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, $n, that was one in a million!'",
	"$n exclaims, 'YAHOO!  Great shot, $t, that was one in a million!'",
    },
    
    {
	"yawn",
	"You must be tired.",
	"$n yawns.",
	"You must be tired.",
	"$n yawns at $N.  Maybe you should let them go to bed?",
	"$n yawns at you.  Maybe $e wants you to go to bed with $m?",
	"You must be tired.",
	"$n yawns.",
	"$n yawns at $t.  Maybe you should let them go to bed?",
    },
	 
    {
	"yeehaw",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
    },

    {
	"fear",
	"You fear.",
	"$n fears.",
	"You fear $M.",
	"$n fears $N.",
	"$n fears you.  You RULE!",
	"You fear yourself.  Kind of paranoid?",
	"$n fears $mself.  $e must psycho.",
	"$n fears $t.",
    },

    {
	"flaf",
	"You fucking laf.",
	"$n fucking lafs.",
	"You fucking laf at $M.",
	"$n fucking lafs at $N.",
	"$n fucking lafs at you.  You should pound $m, no doubt about it!",
	"You fucking laf at yourself.  Are you whacked!?",
	"$n fucking lafs at $mself.  $e must be whacked!",
	"$n fucking lafs at $t.",
    },

    {
	"fnog",
	"You fucking nog.",
	"$n fucking nogs.",
	"You fucking nog at $M.",
	"$n fucking nogs at $N.",
	"$n fucking nogs at you.  You agree with $m, right?",
	"You fucking nog at yourself.  Are you whacked!?",
	"$n fucking nogs at $mself.  $e must be whacked!",
	"$n fucking nogs at $t.",
    },

    {
	"laf",
	"You laf.",
	"$n lafs.",
	"You laf at $M.",
	"$n lafs at $N.",
	"$n lafs at you.  You should pound $m, right?",
	"You laf at yourself.  Are you whacked!?",
	"$n lafs at $mself.  $e must be whacked!",
	"$n lafs at $t.",
    },

    {
	"mfear",
	"You take a moment fear!.",
	"$n takes a moment fear!.",
	"You take a moment fear $M!",
	"$n takes a moment fear $N!",
	"$n takes a moment fear you!.  Enjoy it while it lasts.",
	"You take a moment fear yourself.  You're whacked dude!",
	"$n takes a moment fear $mself.  $e is really whacked!",
	"$n takes a moment fear $t!",
    },

    {
	"mhfnog",
	"You take a moment to let out some hard f__king nogs!.",
	"$n takes a moment to let out some hard f__king nogs!.",
	"You take a moment to let out some hard f__king nogs at $M.",
	"$n takes a moment to let out some hard f__king nogs at $N.",
	"$n takes a moment to let out some hard f__king nogs at you.  You better agree with $m!",
	"You take a moment to let out some hard f__king nogs at yourself.  You're whacked dude!",
	"$n takes a moment to let out some hard f__king nogs at $mself.  $e is really whacked!",
	"$n takes a moment to let out some hard f__king nogs at $t.",
    },

    {
	"nog",
	"You nog.",
	"$n nogs.",
	"You nog at $M.",
	"$n nogs at $N.",
	"$n nogs at you.  You agree with $m, right?",
	"You nog at yourself.  Are you whacked!?",
	"$n nogs at $mself.  $e must be whacked!",
	"$n nogs at $t.",
    },

    {
	"",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
};


/*
 * The X-social table.
 * Add new X-socials here.
 * Alphabetical order is not required.
 */
const	struct	xsocial_type	xsocial_table [] =
{

    {
	"x-earlobe",
	"On whom do you wish to do this?",
	NULL,
	"You gently tug on $M earlobe with your teeth.",
	"$n gently tugs on $N's earlobe with $s teeth.",
	"$n gently tugs on your earlobe with $s teeth.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

{
"x-kneel",
"Drop to your knees before who?",
NULL,
"You drop to your knees before $M, begging for mercy.",
"$n drops to $s knees before $N, begging for $M mercy.",
"$n drops to $s knees before you, begging for mercy.",
"Nope.",
NULL,
0, 0, 0, 7, 7, FALSE
},
    {
	"x-french",
	"On whom do you wish to do this?",
	NULL,
	"You give $M a deep throbbing kiss, rolling your tongue around $S.",
	"$n gives $N a deep throbbing kiss.",
	"$n gives you a deep throbbing kiss, rolling $s tongue around yours.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

    {
	"x-kissneck",
	"On whom do you wish to do this?",
	NULL,
	"You slowly and softly kiss and nuzzle $M neck.",
	"$n slowly and softly kisses and nuzzles $N's neck.",
	"$n slowly and softly kisses and nuzzles your neck.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

{
"x-wcream",
"On whom do you wish to do this?",
NULL,
"You spray whipped cream all over $M body, licking it up with your tongue.",
"$n coats $N's body in whipped cream and then licks it up with their tongue.",
"$n covers your body in whipped cream, and you moan as it gets licked up with their tongue.",
"No.",
NULL,
0, 0, 0, 5, 5, FALSE
},
{
"x-ice",
"On who?",
NULL,
"You run a cold piece of ice down $M back, following up with your tongue.",
"$n runs a piece of ice over $N's back, following up with their tongue.",
"$n runs a piece of ice over your burning hot flesh, following up with their tongue.",
"No.",
NULL,
0, 0, 0, 6, 8, FALSE
},

{
"x-whip",
"Who's been bad?",
NULL,
"You smirk slightly as you crack a whip over $N's tender skin, making $M whimper.",
"$n grins slightly as he whips $N's tender flesh, making $M whimper.",
"$n grins wickedly as they whip your tender flesh, making you whimper slightly in fear.",
"No.",
NULL,
0, 0, 0, 9, 2, FALSE
},
    {
	"x-moan",
	"On whom do you wish to do this?",
	NULL,
	"You start moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"$n starts moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"$n starts moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"Not on yourself!",
	NULL,
	0, 2, 0, 1, 1, FALSE
    },

    {
	"x-nuttella",
	"On whom do you wish to do this?",
	NULL,
	"You cover $N's naked flesh with a popular choc spread, before licking it off.",
	"$n smears a chocolate spread over $N's body, licking $M clean with relish",
	"$n smears a popular choc spread on your naked flesh, licking it off you",
	"Not on yourself!",
	NULL,
	0, 0, 0, 5, 20, FALSE
    },

    {
	"x-stroke",
	"On whom do you wish to do this?",
	NULL,
	"You lightly run your fingers along the insides of $S thighs.",
	"$n lightly runs $s fingers along the insides of $N's thighs.",
	"$n lightly runs $s fingers along the insides of your thighs.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 5, 10, FALSE
    },

    {
	"x-tender",
	"On whom do you wish to do this?",
	NULL,
	"You run your fingers through $S hair and kiss $M tenderly on the lips.",
	"$n runs $s fingers through $N's hair and kisses $M tenderly on the lips.",
	"$n runs $s fingers through your hair and kisses you tenderly on the lips.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

    {
	"x-tie",
	"On whom do you wish to do this?",
	NULL,
	"You tie $N to the bed posts!",
	"$n ties $N's body, fixing $M helplessly on the bed.  Oh!! Kinky!",
	"$n ties you to the bed post, leaving you completely at $s mercy",
	"Not on yourself!",
	NULL,
	0, 0, 0, 15, 10, FALSE
    },

	{
	"x-gag",
	"Gag whom?",
	NULL,
	"You tear off a small strip of $N's clothing, and stuff it into their mouth, tying it back behind their head to secure it.",
	"$n reaches down and tears off a small strip of $N's clothing, stuffing it into $S mouth and tying it behind $S head, effectively gagging $S.",
	"$n tears off a strip of your clothing, and as you look up at $s, $s shoves it into your mouth and ties it behind your head, gagging you.",
	"No.",
	NULL,
	0, 0, 0, 5, 2, FALSE
	},

	{
	"x-blindfold",
	"Blindfold who?",
	NULL,
	"You take out a black silk blindfold, and place it over $N's eyes, tying it securely.",
	"$n takes out a black silk blindfold, and places it over $N's eyes, tying it securely behind $S.",
	"$n takes out a black silk blindfold, and grins as $s places it over your eyes, tying it behind your head.",
	"No.",
	NULL,
	0, 0, 0, 7, 2, FALSE
	},

	{
    "x-knife",
	"On whom do you wish to do this?",
    NULL,
    "You lightly run a cool knife over $N's skin, letting it slide across their body and make weak cuts in their flesh.",
    "$n lightly traces a knife over $N's skin, smiling as it makes a few shallow cuts, drawing a bit of blood.",
    "$n lightly runs a knife over your skin, and you shiver a bit as it makes a few shallow cuts, drawing a bit of your blood.",
    "No.",
    NULL,
    0, 0, 0, 15, 3, FALSE
	}, 

    {
	"x-withdraw",
	"On whom do you wish to do this?",
	NULL,
	"You gently pull yourself free of $M.",
	"$n gently pulls $mself free of $N.",
	"$n gently pulls $mself free of you.",
	"Not on yourself!",
	NULL,
	0, 1, 0, 0, 0, FALSE
    },

	{
	"xl-candle",
	"On who?",
	NULL,
	"You produce a thick white candle, and you step behind $N with it, grinning a bit as you run it up and down her pussy lips.  After a moment you push it in and twist it, starting to slowly pump it in and out.",
	"$n produces a thick white candle, and walks behind $N, holding it in her hands.  She grins a bit as she pushes it to the girl's wet pussy lips, and pushes it in, twisting it as she begins to pump it in and out.",
	"$n produces a thick white candle, and steps behind you as you whimper a bit.  After a moment, you feel it running up and down your cunt lips, and you let out a moan as it slips inside you, twisting as it slides in and out.",
	"No.",
	NULL,
	3, 0, 0, 17, 39, FALSE
	},
{
"xl-afinger",
"Use this on who?",
NULL,
"You take your finger to $N's mouth and have her lick it before taking it to her puckered ass and slowly slipping it in.",
"$N sucks lightly on $n's finger a moment before she takes it between $N's ass cheeks and pushes it in to the knuckle, twisting.",
"$n brings her finger to your lips, and you suck on it a bit before she pulls it away.  A moment later, she presses it to your tight little ass and pushes it in, twisting as you whimper softly.",
"No.",
NULL,
3, 0, 0, 20, 25, FALSE
},

	{
    "xl-finger",
	"On whom do you wish to do this?",
    NULL,
    "You spread $N's thighs and slip your finger between them, tracing it over her lips a few times before pushing it past and into her hot and wet pink folds.",
    "$n spreads $N's thighs and plunges her finger into her twat, pumping it out a few times as sweet, sticky juice spills over it.",
    "$n spreads your legs and begins to trace her finger over your moistened lips, and you let out a soft moan as she slips it past them and into your hot snatch, plunging it in and out as it collects your juices.",
    "No.",
    NULL,
    3, 0, 250, 19, 30, FALSE
	},

	{
	"xl-gag",
	"Gag who?",
	NULL,
	"You pick up your soaked panties, and wad them up into a ball, stuffing them into $N's mouth to gag her.",
	"$n picks up her drenched panties, wadding them into a ball and stuffing them into $N's mouth, gagging her.",
	"$n picks up her panties, soaked through with her cunt juice, and wads them into a ball, stuffing them into your mouth to gag you.",
	"No.",
	NULL,
	3, 0, 0, 7, 9, FALSE
	},

	{
    "xl-stroke",
	"On whom do you wish to do this?",
    NULL,
    "You slip your finger between $N's legs and gently stroke her throbbing clit, making her squirm and moan.",
    "$n slips her finger between $N's damp thighs and begins to softly stroke her clit, making the poor girl squirm and moan in pleasure.",
    "$n slips her finger between your thighs and begins to softly stroke your throbbing clit, making you shut your eyes and moan in pleasure.",
    "No.",
    NULL,
    3, 0, 250, 10, 17, FALSE
	},

	{
    "xl-undress",
	"On whom do you wish to do this?",
    NULL,
    "You reach back and unhook your bra, letting it slide to the floor with your panties.",
    "$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",
    "$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",
    "Pick a valid target.",
    NULL,
    3, 0, 250, 15, 15, FALSE
	}, 

	{
    "xl-lick",
	"On whom do you wish to do this?",
    NULL,
    "You bury your face between $N's thighs, letting your tongue slide over her swollen pussy lips and gently tease her clit.",
    "$n pushes $N to the floor and buries her face between her thighs, letting her tongue slide over $N's pussy lips and caress her clit.",
    "$n pushes you to the floor, burying her face between your damp thighs as she lets her tongue caress your swollen pussy lips, and you squirm as it lightly runs over your clit.",
    "No.",
    NULL,
    3, 0, 250, 25, 37, FALSE 
	},

	{ 
    "xl-69",
	"On whom do you wish to do this?",
    NULL, 
    "You lie down on your back and motion for $N to straddle your face, moaning as she licks at you while you hungrily devour her wet snatch.", 
    "$N lowers her pussy over $n's face and places her face between her thighs, both of them gasping as their tongues dart in and out of each other.",
    "$n pulls you on top of her, placing your hot, wet pussy right over her face, and you do the same, both of you panting and moaning as you devour each other.", 
    "No.", 
    NULL, 
    3, 1, 250, 40, 40, FALSE 
	},

	{ 
    "xl-breasts",
	"On whom do you wish to do this?",
    NULL, 
    "You lean forward and place your hands over $N's supple breasts, kneading them softly as she moans.",
    "$n places her hands over $N's soft breasts and begins to gently knead them, smiling as she begins to moan.", 
    "$n places her hands over your soft breasts and begins to knead them, a smile playing over her face as you start moaning.", 
    "No.",
    NULL,
    3, 0, 250, 10, 18, FALSE 
	}, 
	
	{
	"xf-oral",
	"On whom do you wish to do this?",
	NULL,
	"You take $N's hot member in your hands, licking his cock from base to tip.",
	"$n takes $N's penis in $s hands, licking $N's cock from base to tip.",
    "You let out a small moan as $n takes your cock into her hands, licking it from your balls to the tip.",
	"On yourself? I'm impressed!",
	NULL,
	2, 0, 0, 5, 15, FALSE
    },

	{
	"xf-spank",
	"Who's been naughty?",
	NULL,
	"You take $N over your knee and spank him hard, making him squirm.",
	"$n bends $N over her knees and spanks him hard on the ass, making him squirm.",
	"You whimper softly as $n takes you over her knee, spanking you hard on the ass, making it sting.",
	"Nah.",
	NULL,
	2, 0, 0, 25, 4, FALSE
	},

	{
	"xf-urine",
	"On whom do you wish to do this?",
	NULL,
	"You maliciously lean $N's head back and stand over him with his mouth open before letting a hard stream of piss shoot down his throat.",
	"$n shoots a golden stream of urine down $N's throat, making him choke and swallow.",
	"You eyes widen as your mistress shoots a stream of piss odown your throat, making you swallow it all.",
	"Nope.",
	NULL,
	2, 0, 0, 37, -10, FALSE
	},
    
	{
	"xf-beg",
	"Beg who, you stupid slut?",
	NULL,
	"You drop to your kneees before $N, begging for him to abuse you and fuck you hard.",
	"$n drops to her knees and begs for $N to take her and use her for his pleasure.",
	"$n drops to her knees before you, begging for you to abuse her worthless slut body.",
	"It doesn't work that way, you stupid whore.",
	NULL,
	2, 0, 0, -10, 12, FALSE
	},

	{
	"xf-blowjob",
	"On whom do you wish to do this?",
	NULL,
	"You take $N's hot member in your mouth, sucking $S shaft.",
	"$n takes $N's throbbing penis in $s mouth, sucking $N's cock.",
	"You gasp as $n takes your penis in $s mouth, licking your head.",
	"On yourself? I'm impressed!",
	NULL,
	2, 1, 250, 10, 25, FALSE
    },

	{
       "xf-tip",
       "On whom do you wish to do this?",
       NULL,
       "You gently run the tip of your tongue along the underside of $N's hot cock.",
       "$n runs the tip of $s tongue along the bottom of $N's pulsing cock.",
       "You let out a low moan as $n runs the tip of her tongue along the underside of your cock.",
       "I think not.",
       NULL,
         2, 0, 0, 5, 59, FALSE
	},

	{
	"xf-handjob",
	"On who?",
	NULL,
	"You take his hot cock in your hand and wrap your fingers around it, slowly pumping up and down.",
	"$n takes $N's hot cock in her hands, wrapping her fingers around it as she slowly pumps up and down.",
	"$n takes your hot cock into her hands, and you moan softly as she wraps her fingers around it, slowly pumping up and down.",
	"No.",
	NULL,
	2, 0, 0, 9, 14, FALSE
	},

	{
	"xf-sitface",
	"Who gets to taste your sweet juice?",
	NULL,
	"You push $N onto his back and crawl up his body, sitting down on his face as he licks at your pussy.",
	"$n pushes $N onto his back and straddles his face, moaning as he licks her hot pussy.",
	"$n pushes you onto your back and straddles your face, moaning and squirming as you eagerly lap at her hot, wet cunt.",
	"No.",
	NULL,
	2, 1, 250, 15, 0, FALSE
	},

	{
	"xf-69",
	"Who?",
	NULL,
	"You crawl on top of $N and takes his hot cock between your lips, moaning as he laps at your cunt.",
	"$n pushes $N onto his back and crawls on top of him, sucking his cock while he wriggles his tongue about in her cunt.",
	"$n pushes you onto your back, crawling on top of you as she sucks on your cock, moaning while you lick her pussy.",
	"No.",
	NULL,
	2, 1, 250, 20, 20, TRUE
	},

    {
	"xf-breasts",
	"On whom do you wish to do this?",
	NULL,
	"You take $S hands and press them to your breasts.",
	"$n takes $N's hands and presses them to $m breasts.",
	"$n takes your hands and presses them to $m breasts.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 5, 10, FALSE
    },

{
"xf-cum",
"Wrong...",
NULL,
"You feel your muscles tighten for a moment, and you let out a loud moan and arch your back as you release, cumming hard as your juices run down your thighs.",
"$n arches her back and lets out a loud moan, gasping and writhing in pleasure as she cums, her juices running down her thighs.",
"$n arches her back and lets out a loud moan, gasping and writhing in pleasure as her juices gush down her thighs.",
"No.",
NULL,
2, 1, 0, 500, 25, FALSE
},
{ 
"xf-cface",
"Wrong...",
NULL,
"You bite down hard on your lip, letting out a loud moan as you climax, gushing your sticky, wet cum all over $N's face, glazing it over.",
"$n bites down on her lip and lets out a loud cry of pleasure, gushing her juices all over $N's face as he sucks and slurps at her dripping wet pussy.",
"$n bites down on her lip and lets out a loud moan as you slurp at her dripping wet cunt, and she shudders before gushing her sweet juices all over your face, glazing it completely over.",
"No.",
NULL,
2, 1, 0, 500, 30, FALSE
},
{
"xf-chands",
"No...",
NULL,
"You keep sliding your hands back and forth over $N's cock as it grows hotter and hotter, not stopping until he lets out a low groan and fills your hand with his hot, sticky cum.",
"$n slides her hands back and forth over $N's cock as he moans, eventually gritting his teeth and shutting his eyes as he fills her hand full of his hot sticky load of cum, most of it oozing out from between her fingertips.", 
"$n slides her hands over your cock as it grows hotter and hotter, and you let out a low moan as you grit your teeth and shoot a hot, sticky load of cum into her closed hands, filling it up as most of your gooey sperm drips out from between her fingertips.",
"No.",
NULL,
2, 1, 0, 30, 500, FALSE
},

	{
"xf-cbreasts",
"No..",
NULL,
"You take $N's twitching cock between your hands, and aim it straight at your heaving tits, jerking him off until you feel stream after stream of hot, white cum splatter against them and trickle over your stiff nipples.",
"$n takes $N's stiff cock in her hands and aims it at her soft tits, giving him a few quick jerks to finish him off as he shoots out a jet of hot, sticky cum all over her ample mounds and hard nipples.",
"$n takes your cock into her hands and aims it straight at her tits, giving you a few quick jerks to finish you off, and you moan as you shoot your sticky white seed all over her soft breasts, your cum oozing down and trickling over her nipples.",
"No.",
NULL,
1, 1, 0, 37, 500, FALSE
},

    {
	"xf-ride",
	"On whom do you wish to do this?",
	NULL,
	"You moan loudly as you cling onto $N tightly, thrusting yourself up and down $N's stiff cock.",
	"$n moans loudly as $e clings onto $N tightly, thrusting $mself up and down $N's stiff cock.",
	"$n moans loudly as $e clings onto you tightly, thrusting $mself up and down your stiff cock.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

	{
      "xf-rub",
      "Who gets to feel the magic touch?",
	  NULL,
      "You gently run your fingers over $N's hardening cock.",
      "$n gently runs her fingers over $N's hardening cock.",
      "$n gently runs her fingers over your hardening cock.",
      "You don't have a penis.  At least, I hope not.",
      NULL,
      2, 0, 0, 15, 20, FALSE
	},
    {
	"xf-contract",
	"On whom do you wish to do this?",
	NULL,
	"You contract your vaginal muscles, driving $M wild.",
	"$n contracts $m vaginal muscles, driving $N wild.",
	"$n contracts $m vaginal muscles, driving you wild.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 10, 15, TRUE
    },

{
"xf-afinger",
"Use this on who?",
NULL,
"You lick your finger and press it to $N's ass, pushing lightly until it pops in and he lets out a small moan.",
"$n licks her finger and takes it to $N's ass, popping it in and gently pumping it back and forth, making him moan.",
"$n licks her finger and takes it to your ass, pushing it in and pumping it in and out, making your dick rock-hard as you moan.",
"No.",
NULL,
2, 1, 0, 20, 23, FALSE
},

    {
	"xf-finger",
	"On whom do you wish to do this?",
	NULL,
	"You put your hands between your legs and begin to masterbate for $N.",
	"$n puts $m hands between $m legs and begins to masterbate for $N.",
	"$n puts $m hands between $m legs and begins to masterbate for your viewing pleasure.  What a turn on!.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 20, 10, FALSE
    },
    {
	"xf-floor",
	"On whom do you wish to do this?",
	NULL,
	"You lie on your back, and pull $M between your parted legs.",
	"$n lies on $m back, and pulls $N between $m parted legs.",
	"$n lies on $m back, and pulls you between $m parted legs.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

    {
	"xf-grind",
	"On whom do you wish to do this?",
	NULL,
	"You grind your hips up to meet $S thrusts.",
	"$n grinds $m hips up to meet $N's thrusts.",
	"$n grinds $m hips up to meet your thrusts.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 15, 10, TRUE
    },

    {
	"xf-mount",
	"On whom do you wish to do this?",
	NULL,
	"You push $M onto $S back, and slowly lower yourself onto $S erection.",
	"$n pushes $N onto $S back, and slowly lowers $mself onto $S erection.",
	"$n pushes you onto your back, and slowly lowers $mself onto your erection.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

    {
	"xf-nails",
	"On whom do you wish to do this?",
	NULL,
	"You scratch your nails down $S back.",
	"$n scratches $m nails down $N's back.",
	"$n scratches $m nails down your back.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 5, 1, TRUE
    },

    {
	"xf-pull",
	"On whom do you wish to do this?",
	NULL,
	"You wrap your arms and legs around $M and pull $M into you.",
	"$n wraps $m arms and legs around $N and pulls $M into $m.",
	"$n wraps $m arms and legs around you and pulls you into $m.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 15, 10, TRUE
    },

    {
	"xf-squeeze",
	"On whom do you wish to do this?",
	NULL,
	"Your wrap your legs around $M and squeeze tightly.",
	"$n wraps $m legs around $N and squeezes tightly.",
	"$n wraps $m legs around you and squeezes tightly.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 15, 10, TRUE
    },

    {
	"xf-titfuck",
	"Whom do you wish to treat to this?",
	NULL,
	"You take $N's cock and place it between your breasts, as $N gently thrusts.",
	"$n takes $N's penis, places it between $s breasts, and wanks $M off.",
	"$n takes your cock, places it between her breasts, and wanks you gently with them.",
	"Not on yourself!",
	NULL,
	2, 1, 0, 10, 20, FALSE
    },

{
"xf-fondle",
"On whom do you wish to perform this?",
NULL,
"You reach down and gently fondle $N's warm penis.",
"$n takes $N's cock into her hands, fondling it gently.",
"$n takes your warm cock into her hands, fondling you gently.",
"No.",
NULL,
2, 0, 0, 5, 13, FALSE
},

{
"xf-bob",
"On whom do you wish to perform this?",
NULL,
"You wrap your lips tight around $N's cock and bob your head back and forth, forcing him into your mouth.",
"$n wraps her lips over $N's cock and bobs her head back and forth, making him fuck her mouth.",
"$n wraps her lips tight around your cock and bobs her head back and forth, pumping your length in and out of her mouth.",
"No.",
NULL,
2, 1, 0, 20, 25, TRUE
},

    {
	"xf-undress",
	"On whom do you wish to do this?",
	NULL,
	"You push $N onto the floor, a gleam in your eyes, and tear $S clothes off.",
	"$n pushes $N to the floor, tears $S clothes from $S body.",
	"$n pushes you to the floor, grins, and rips your clothes from your body.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 1, 1, FALSE
    },

    {
	"xm-anal",
	"On whom do you wish to do this?",
	NULL,
	"$N screams in extacy as you come from behind and penetrate $M tight ass from behind.",
	"$n comes from behind $N and penetrates $M tight ass from behind.",
	"You scream as $n comes from behind and penetrates your tight ass from behind!",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 25, FALSE
    },

    {
	"xm-spank",
	"Who's the naughty whore?",
	NULL,
	"You grab $N by the hair and throw her over your knees, slapping her hard on the ass.",
	"$n visciously throws $N over his knee, spanking her relentlessly on her tight ass.",
	"You cry out softly as $n visciously takes you over his knee, spanking you mercilessly.",
	"Nah.",
	NULL,
	1, 0, 0, 33, -14, FALSE
	},

	{
	"xm-69",
	"Whom do you wish to perform this on?",
	NULL,
	"You pull $N on top of you and spread her legs, burying your face in her pussy as she sucks on your hard cock.",
	"$n pulls $N on top of him and buries his face between her legs, eating her out as she goes down on his cock.",
	"$n pulls you on top of him and spreads your legs, burying his face between your legs and sucking on your clit as you takes his cock into your mouth.",
	"No.",
	NULL,
	1, 1, 0, 24, 24, TRUE
	},

	{
	"xm-urine",
	"Who gets to taste it?",
	NULL,
	"You force open $N's slutty mouth and shoot a hot stream of piss into it, making her swallow.",
	"$n forces open $Ns mouth and shoots a hot stream of golden piss into it, making her swallow.",
	"$n forces open your slutty mouth and shoots a hot, golden stream of his piss into it, making you swallow.",
	"No.",
	NULL,
	1, 0, 0, 40, -19, FALSE
	},

	{
	"xm-beg",
	"Beg who, you worthless bitchboy?",
	NULL,
	"You drop to your knees, tears in your eyes, and beg for $N to take you as hers.",
	"$n drops to his knees before $N, begging for her to take him as hers.",
	"$n drops to his knees before you with tears in his eyes, sobbing as he begs for you to take him as your own personal toy.",
	"You worthless slut, you can't even do this right.",
	NULL,
	1, 0, 0, -15, 30, FALSE
	},

	{
	"xm-facial",
	"Who gets to have their slutty face done over?",
	NULL,
	"You groan as you shoot your load across her delicate features, your hot cum coating her face and dripping from her chin.",
	"$n grits his teeth as he shoots his sticky cum over $N's face, moaning as she licks her lips and lets it drip down.",
	"$n's eyes widen as he shoots his hot, sticky cum over your face, letting it drip from your chin and trickle over your lips.",
	"Nope.",
	NULL,
	1, 1, 0, 300, 20, TRUE
	},
	
	{
	"xm-cum",
	"So close, yet, so far away?",
	NULL,
	"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
	"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
	"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
	"Nopers.",
	NULL,
	1, 2, 0, 300, 20, TRUE
	},

{
"xm-chair",
"Cum in whose hair?",
NULL,
"You pull your hard cock out of $N's hole, and force her to her knees before you as you finish yourself off with your hand, shooting your gooey wad straight into her hair and using a handfull of it to wipe off the sticky head.",
"$n pulls his cock out of $N's hole, and forces her to her knees, jerking himself off for a moment before shooting his hot, sticky sperm into her hair, and he takes a handfull of it to wipe off the head of his still-stiff meat.",
"$n pulls himself out of your hole, and forces you to your knees before him, jerking off into your hair as you look up.  A hot, gooey splatter of cum smacks against your scalp, and he takes a handfull of your hair to wipe off the still-sticky head of his cock.",
"No.",
NULL,
1, 1, 0, 500, 47, FALSE
},

{
"xm-canal",
"Cum in whose ass?",
NULL,
"You pump your cock in and out of $N's ass a few more times before grunting and shooting your sticky load of cum right up it, feeling it fill and trickle over your still-hard cock and down her thighs.",
"$n pumps into $N's tight ass a few more times before he grunts, holding his cock there as he shoots her full of cum, and she lets out a tiny gasp as a little bit oozes out of her tight ass and trickles down her thighs.",
"$n shoves his cock into your tight little ass a few more times, and you hear a grunt from behind you as he starts to cum, shooting a sticky white jet of his cream straight up your ass, and you feel a bit of it trickle out and run down your thighs.",
"No.",
NULL,
1, 1, 0, 500, 47, FALSE
},


{
"xm-cbreasts",
"So close, yet, so far away?",
NULL,
"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
"Nopers.",
NULL,
1, 1, 0, 300, 20, TRUE
},
	{
	"xm-chand",
	"So close, yet, so far away?",
	NULL,
	"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
	"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
	"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
	"Nopers.",
	NULL,
	1, 1, 0, 300, 20, TRUE
	},

	{
	"xm-stomach",
	"Cum on whose stomach?",
	NULL,
	"You pull out of $N and stand up, pushing her to her back.  After a few quick jerks on your stiff cock, you let out a moan and start to cum, spraying your hot, gooey cum all over her smooth body.",
	"$n pulls out of $N and stands up, pushing her to her back.  After a few quick jerks on his stiff cock, he closes his eyes and lets out a moan, a stream of gooey whip cum spraying from the swollen head of his cock and all over $N's smooth skin.",
	"$n pulls out of you and pushes you onto your back.  As you look up, he gives his hard cock a few quick jerks, and he moans as he starts to cum, a spray of hot, gooey white cum splattering all over your body, coating you from head to toe.",
	"No.",
	NULL,
	1, 1, 0, 500, 50, TRUE
	},

	{
	"xm-breasts",
	"On whom do you wish to do this?",
	NULL,
	"You gently run the tip of your tongue across $M naked breasts.",
	"$n gently runs the tip of $s tongue across $N's naked breasts.",
	"$n gently runs the tip of $s tongue across your naked breasts.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-cup",
	"On whom do you wish to do this?",
	NULL,
	"You cup $N's breasts in your hands, and caress $S nipples.",
	"$n cups $N's breasts in $s hands and caress' $S nipples.",
	"$n cups your breasts in $s palms and caress' your nipples.",
	"ON YOURSELF??",
	NULL,
	1, 0, 0, 1, 5, FALSE
    },

    {
	"xm-doggy",
	"On whom do you wish to do this?",
	NULL,
	"You roll $M onto all fours and penetrate $M hot flesh from behind.",
	"$n rolls $N onto all fours and penetrates $M hot flesh from behind.",
	"$n rolls you onto all fours and penetrates your hot flesh from behind.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 15, FALSE
    },

    {
	"xm-finger",
	"On whom do you wish to do this?",
	NULL,
	"You slide your fingers between $M legs, gently stroking $M clitoris.",
	"$n slides $s fingers between $N's legs, gently stroking $M clitoris.",
	"$n slides $s fingers between your legs, gently stroking your clitoris.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 0, 10, FALSE
    },

    {
	"xm-fist",
	"On whom do you wish to do this? I hope you asked!",
	NULL,
	"You make a fist and thrust it up $N's spread wide vagina as she screams with pleasure.",
	"$N gasps as $n pushes $s hand up between $S legs into $S vagina.",
	"$n spreads your legs wide, and thrusts $s hand up your vagina making you cry out in pleasure.",
	"C'est non possible.. I hope...",
	NULL,
        1, 1, 0, 5, 15, FALSE
    },

    {
	"xm-floor",
	"On whom do you wish to do this?",
	NULL,
	"You lower $M to the floor, and slide your body between $M parted legs.",
	"$n lowers $N to the floor, and slides $s body between $M parted legs.",
	"$n lowers you to the floor, and slides $s body between your parted legs.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 15, 10, TRUE
    },

    {
	"xm-jerkoff",
	"On whom do you wish to do this?",
	NULL,
	"You grab your penis and begin to jerkoff for $N.",
	"$n grabs your penis and begins to jerkoff for $N.",
	"$n grab his penis and begin to jerkoff for your viewing pleasure.  What a turn on!.",
	"Not on yourself!",
	NULL,
	1, 1, 0, 25, 5, FALSE
    },

    {
	"xm-nipple",
	"On whom do you wish to do this?",
	NULL,
	"You gently twist $M nipple between your thumb and forefinger.",
	"$n gently twists $N's nipple between $s thumb and forefinger.",
	"$n gently twists your nipple between $s thumb and forefinger.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-oral",
	"On whom do you wish to do this?",
	NULL,
	"$N squirms in delight as you bend down and run your tongue along and into $S vagina.",
	"$N squirms ecstatically as $n licks and kisses $S loveslit.",
	"$n wriggles $s tongue about between your legs, making you squirm in ecstacy.",
	"Biologically impossible I think you'll find!",
	NULL,
	1, 0, 0, 10, 25, FALSE
    },

    {
	"xm-press",
	"On whom do you wish to do this?",
	NULL,
	"You press $M against the wall, pulling $M legs around your hips.",
	"$n presses $N against the wall, pulling $M legs around $s hips.",
	"$n presses you against the wall, pulling your legs around $s hips.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 20, TRUE
    },

    {
	"xm-pull",
	"On whom do you wish to do this?",
	NULL,
	"You grab $M around the hips and pull $M firmly onto your erection.",
	"$n grabs $N around the hips and pull $M firmly onto $s erection.",
	"$n grabs you around the hips and pulls you firmly onto $s erection.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 10, 10, TRUE
    },

    {
	"xm-spoon",
	"On whom do you wish to do this?",
	NULL,
	"You roll $M onto $S side and penetrate $M hot flesh from behind.",
	"$n rolls $N onto $S side and penetrates $M hot flesh from behind.",
	"$n rolls you onto your side and penetrates your hot flesh from behind.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 20, 20, TRUE
    },

    {
	"xm-suck",
	"On whom do you wish to do this?",
	NULL,
	"You suck slowly on $M nipple, feeling it harden between your lips.",
	"$n suck slowly on $N's nipple.",
	"$n sucks slowly on your nipple, and you feel it harden between $s lips.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-thrust",
	"On whom do you wish to do this?",
	NULL,
	"You thrust deeply between $M warm, slippery thighs.",
	"$n thrusts deeply between $N's warm, slippery thighs.",
	"$n thrusts deeply between your warm, slippery thighs.",
	"Not on yourself!",
	NULL,
	1, 2, 0, 15, 10, TRUE
    },
{
"xm-hump",
"On whom do you wish to do this?",
NULL,
"You push $M onto all fours and mount her, madly shoving your hard cock in and out of her tiny hole.",
"$n mounts $N like she was a bitch in heat, madly pumping in and out of her hole.",
"$n pushes you onto all fours like you were a bitch and heat and mounts you as he rapidly shoves his cock in and out of your slutty hole.",
"No!.",
NULL,
1, 2, 0, 35, 65, TRUE
},

{
"xm-mfuck",
"Fuck who?",
NULL,
"You have $N wrap her lips tight around your cock, and you thrust gently, pumping your cock in and out of her warm, wet mouth.",
"$N wraps her lips tight aroudn $n's cock, and he thrusts gently, pumping in and out from between her lips.",
"You wrap your lips tight around $n's cock and he thrusts gently, pumping in and out from between your ruby red lips.",
"No.",
NULL,
1, 1, 0, 12, 20, TRUE
},

{
"xm-feed",
"Feed who?",
NULL,
"You gently stick your fingers in $N's hot cunt and bring them to her lips, making her suck them clean.",
"$n sticks his fingers in $N's twat and then brings them to her lips, making her lick them clean.",
"$n sticks his fingers in your dripping wet pussy and then brings them to your lips, where you suck them clean.",
"No.",
NULL,
1, 1, 0, 5, 10, FALSE
},

{
"xm-gag",
"Who gets to taste it?",
NULL,
"You ram your cock deep into $M mouth, making her gag and try to swallow.",
"$n rams his cock deep into $N's throat, making her gag and try to swallow.",
"$n shoves his cock deep into your throat, making you gag and try to swallow.",
"No.",
NULL,
1, 1, 0, 5, 13, FALSE
},
    {
	"xm-tug",
	"On whom do you wish to do this?",
	NULL,
	"You gently tug $M nipple between your teeth.",
	"$n gently tugs $N's nipple between $s teeth.",
	"$n gently tugs your nipple between $s teeth.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

{
"xm-nibble",
"On whom do you wish to do this?",
NULL,
"You push $M onto her back and bury your face in her cunt, nibbling gently on her clit.",
"$n pushes $N onto her back and buries his face between her legs, nibbling gently on her clit.",
"$n pushes you onto your back and buries his face in your womanhood, nibbling gently on your clit.",
"Nope.",
NULL,
1, 1, 0, 10, 45, FALSE
},
    {
	"xm-undress",
	"Who do you wish to undress?",
	NULL,
	"You gently tug at $N's garments with your teeth, until $E stands naked before you.",
	"$n gently tugs at $N's clothes with his teeth, until $E stands naked infront of $m.",
	"$n gently tugs at your clothing with his teeth, you stand naked before $m.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 1, 1, FALSE
    },

    {
	"",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, FALSE
    }
};

/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *unveil;
    char arg[MAX_STRING_LENGTH];
    char argu[MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char command[MAX_STRING_LENGTH];
    char logline[MAX_STRING_LENGTH];
    int cmd;
    int trust;
    bool found;

    sprintf(argu,"%s %s",arg,one_argument( argument, arg));

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You can't do anything while crying like a bitch!\n\r", ch );
	return;
    }

    if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
         && IS_SET( ch->arena, AFF2_INARENA ) && arena > 13 )
      return;

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_FROZEN) )
    {
	send_to_char( "You are frozen!\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_STONE) )
    {
	send_to_char( "#yYou are #opetrified#y!#n\n\r", ch );
	return;
    }

    if ( ch->nemesis > 0 || ( IS_CLASS( ch, CLASS_KURUDA ) && ch->ctimer[19] > 0 )
         || ( IS_CLASS( ch, CLASS_DRAGOON ) && ch->ctimer[8] > 0 )
         || ( IS_CLASS( ch, CLASS_MONK ) && ch->ctimer[8] > 0 ) )
       {
	    send_to_char( "Concentration. You're in the middle of a complicated attack.\n\r", ch );
	    return;
       }

    if ( IS_SET( ch->more2, MORE2_DIG ) && !IS_CLASS( ch, CLASS_SNOW_ELF ) )
       {
	    send_to_char( "You can't do anything while buried so deep inside of the earth.\n\r", ch );
	    return;
       }

    if ( ch->noskill > 0 )
       {
           ch->agg = 100;
           ch->noskill = 0;

           act( "#wUnable to #Cwithstand #wyour movement, #Pshards #wof #RFEROCIOUS #yenergy #wERUPTS from your body!!!#n", ch, NULL, NULL, TO_CHAR );
           act( "#wUnable to #Cwithstand #w$s movement, #Pshards #wof #RFEROCIOUS #yenergy #wERUPTS from $n's body!!!#n", ch, NULL, NULL, TO_ROOM );

           if ( IS_NPC( ch ) )
              ch->hit = UMAX( 0, ch->hit - 3000000 );
              else ch->hit -= 3000000;

           special_slam( ch, number_range( 0, 3 ) );
           stop_fighting( ch, TRUE );
           ch->position = POS_STUNNED;
           return;
       }

    if ( !IS_IMMORTAL( ch ) && IS_SET( ch->in_room->room_flags, ROOM_NOMOVE ) && ch->position > POS_STUNNED )
       {
           ch->agg = 100;

           act( "#wSensing #yyour #Canticipated #wmovement, #Pshards #wof #yenergy #wsudddenly SLAMS itself into your body!!!#n", ch, NULL, NULL, TO_CHAR );
           act( "#wSensing #y$s #Canticipated #wmovement, #Pshards #wof #yenergy #wsudddenly SLAMS itself into $n#w's body!!!#n", ch, NULL, NULL, TO_ROOM );

           if ( IS_NPC( ch ) )
              ch->hit = UMAX( 0, ch->hit - 2000000 );
              else ch->hit -= 2000000;

           special_slam( ch, number_range( 0, 3 ) );
           stop_fighting( ch, TRUE );
           ch->position = POS_STUNNED;
           return;
       }

    if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
         && IS_SET( ch->arena, AFF2_INARENA ) )
       {
          int team;

          if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
             team = 0;
             else team = 1;

          switch( team_list->power_mode[team] )
          {
             case 4: if ( number_percent() > 65 )
                        {
                           SET_BIT( ch->arena, AFF2_CHAIN );
                           act( "#CYou move like a blur as a flash of cyanish #ylight #Cshines off you.#n", ch, NULL, NULL, TO_CHAR );
                           act( "#C$n #Cmoves like a blur as a flash of cyanish #ylight #Cshines off you.#n", ch, NULL, NULL, TO_ROOM );
                        }

                     break;
          }
       }

    ch->attack_type = ch->ele_attack = 0;

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );

    /*Lets see who is doing what? -Ferric*/
    strcpy( buf, argument);
    sprintf(last_command,"%s in room[%d]: %s.",ch->name, ch->in_room->vnum, buf);

    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immtalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ":"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "emote"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "oreturn" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "roll" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "leap" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "lifespan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "truesight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "horns" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "cast" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "entomb" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "dcurse" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "soulheal" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "soullink" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "desanctify" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "clantalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ";" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "reply" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "hellfire" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "plasma" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "plasmablade" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "reanimate" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "ashes" )) found = TRUE;
                else if (!str_cmp(cmd_table[cmd].name,"obj")&&
                  !IS_NPC(ch) && ch->pcdata->obj_vnum!=0)found=TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "quit" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "humanform" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else {
		    send_to_char( "Not without a body!\n\r", ch ); return; }
	    }
          else if ( ch->holding != NULL )
          {
		if      (!str_cmp( cmd_table[cmd].name, "say"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ";" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "ktalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "flex" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "hold" )) found = TRUE;
		else {
		    send_to_char( "You are holding someone!\n\r", ch ); return; }
          }
          else if ( ch->holded != NULL )
          {
		if      (!str_cmp( cmd_table[cmd].name, "say"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ";" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "ktalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "flex" )) found = TRUE;
		else {
		    send_to_char( "Not with someone holding you!\n\r", ch ); return; }
          }
          else if ( ch->saiyanlevel > 0 )
          {
             if ( !str_cmp( cmd_table[cmd].name, "say"  ) ) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "charging"    )) found = TRUE;
		    else {
		            send_to_char( "#wNot while #ycharging #wup!#n\n\r", ch ); 
                        return; 
                     }
          }
          else if ( ch->stunned > 0 )
          {
             if ( !str_cmp( cmd_table[cmd].name, "eroll"  ) ) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "emergencyroll"    )) found = TRUE;
                else {
                        send_to_char( "#wYou are #yf#oeelin#wg #Rwoozy#w.#n\n\r", ch );
                        return;
                     }
          }
          else if ( ch->air_timer > 0 )
          {
             if ( !str_cmp( cmd_table[cmd].name, "eflip"  ) ) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "emergencyflip"    )) found = TRUE;
		    else return; 
          }
          else if ( !IS_NPC(ch) && ch->pcdata->capture_timer > 0 )
          {
             if ( !str_cmp( cmd_table[cmd].name, "say"  ) ) found = TRUE;
                else if ( !str_cmp( cmd_table[cmd].name, "ktalk"  ) ) found = TRUE;
                else if ( !str_cmp( cmd_table[cmd].name, "bribe"  ) ) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "flex"    )) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "who"    )) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "look"    )) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "blindfold" )) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "gag"    )) found = TRUE;
		    else {
		            send_to_char( "Not tied in chains!\n\r", ch ); 
                        return; 
                     }
          }
          else if ( IS_SET( ch->more2, MORE2_DIG ) )
          {
             if ( !str_cmp( cmd_table[cmd].name, "ambush"  ) ) found = TRUE;
                else if ( !str_cmp( cmd_table[cmd].name, "ktalk"  ) ) found = TRUE;
                else if ( !str_cmp( cmd_table[cmd].name, "look"  ) ) found = TRUE;
  		    else if ( !str_cmp( cmd_table[cmd].name, "who"    )) found = TRUE;
                else    {
                           send_to_char( "You can't do anything while buried so deep under snow.\n\r", ch );
                           return;
                        }
          }
            else if (IS_EXTRA(ch,TIED_UP) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "yell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shout")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "restore" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "introduce" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "darkness" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "sleep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "wake" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "claws" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowplane" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "regenerate" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shield" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "vclan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "upkeep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immune" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "consent" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "flex" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "gnaw" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "change" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "drink" )) found = TRUE;
		else {
		    send_to_char( "Not while tied up.\n\r", ch );
		    if (ch->position > POS_STUNNED)
		    	act("$n strains against $s bonds.",ch,NULL,NULL,TO_ROOM);
		    return; }
	    }
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );
    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS )
    {
	if (!IS_CREATOR(ch) && !IS_NPC(ch))
	{
	sprintf( log_buf, "Log %s: %s", ch->pcdata->switchname, logline );
	log_string( log_buf );
	}
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( ch != NULL && ch->unveil != NULL && !IS_CLASS( ch, CLASS_VAMPIRE ) )
    {
	unveil = ch->unveil;
	
      if ( unveil->in_room->area != ch->in_room->area )
	   {
	      sprintf(buf, "You lose your mental link with %s.\n\r",ch->name);
	      stc(buf,unveil);
            ch->unveil = NULL;
	   }
	   else
	   {
	      stc("% ", unveil);
	      stc(logline, unveil);
	      stc("\n\r", unveil);
	   }
    }

       if (ch->desc != NULL)
       write_to_buffer(ch->desc,"\n\r",2);	
    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	{
	    if ( !check_xsocial( ch, command, argument ) )
             send_to_char( "Huh?\n\r", ch );
	}

	return;
    }
    else /* a normal valid command.. check if it is disabled */
      if (check_disabled (&cmd_table[cmd]))
      {
              send_to_char ("This command has been temporarily disabled.\n\r", ch );
             return;
      }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{

	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_MEDITATING:
	case POS_SITTING:
	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */

    (*cmd_table[cmd].do_fun) ( ch, argument );

    if ( IS_SET( ch->arena, AFF2_DM ) )
       {
          if ( ch->ctimer[18] > 0 )
             ch->ctimer[18] = 0;

          REMOVE_BIT( ch->arena, AFF2_DM );
       }

    ch->ctimer[9] = UMAX( 0, ch->ctimer[9] );

    REMOVE_BIT( ch->arena, AFF2_POWER );
    REMOVE_BIT( ch->arena, AFF2_COMBO );

    if ( IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_KNIGHT )
         && ch->class_2 == 0 && is_affected( ch,skill_lookup("hyper") ) )
       {
          if ( ch->wait > 12 )
             ch->wait = 12;
       }

    if ( IS_SET( ch->more3, MORE3_ONE ) )
       ch->wait /= 2;
       else if ( is_affected( ch, skill_lookup("quicken") ) )
               ch->wait *= 0.75;
       else if ( is_affected( ch, skill_lookup("age") ) && ch->wait > 4 )
               ch->wait += 6;

    if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
         && IS_SET( ch->arena, AFF2_INARENA ) )
       {
          int team;

          if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
             team = 0;
             else team = 1;

          switch( team_list->power_mode[team] )
          {
             case 1: ch->wait *= 0.75;
             case 4: if ( IS_SET( ch->arena, AFF2_CHAIN ) )
                        {
                            ch->wait = 0;
                            if ( team_list->power_duration[team] > 75 )
                               team_list->power_duration[team] = 75;
                               else if ( team_list->power_duration[team] > 50 )
                                       team_list->power_duration[team] = 50;
                               else if ( team_list->power_duration[team] > 25 )
                                       team_list->power_duration[team] = 25;
                                       else {
                                               team_list->power_duration[team] = 0;
                                               team_list->power_mode[team] = 0;
                                            }
                        }
          }
       }

    if ( !IS_NPC( ch ) && ch->ctimer[15] > 0 )
       ch->wait = 0;
       else if ( !IS_NPC( ch ) && ch->topmode == 100 && ch->hit > ch->max_hit * 0.66
                 && number_percent() > 84 )
               ch->wait *= 0.75;
       else if ( ch->topmode == 33 && ch->hit < ch->max_hit * 0.34 && number_percent() > 84 )
               ch->wait *= 0.75;

    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }


    if ( !found )
	return FALSE;

    switch ( ch->position )
    {

    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;

    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim) )
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }
    return TRUE;
}



bool check_xsocial( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *partner = NULL;
    int cmd;
    int stage;
    int amount;
    bool is_ok = FALSE;
    bool found = FALSE;
    bool one = FALSE;
    bool two = FALSE;

    if (IS_NPC(ch)) return FALSE;

    for ( cmd = 0; xsocial_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == xsocial_table[cmd].name[0]
	&&   !str_prefix( command, xsocial_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    switch ( ch->position )
    {

    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return TRUE;
    }
    if (IS_NPC(victim))
    {
	send_to_char("You can only perform xsocials on players.\n\r",ch);
	return TRUE;
    }

	if( !str_cmp(ch->pcdata->switchname, "") )
	{
		victim->pcdata->partner = ch;
	}
  else if (IS_SET(victim->extra, TIED_UP))
         { victim->pcdata->partner = ch;
         }

    
	if (IS_EXTRA(ch, TIED_UP))
    {
	send_to_char("You wiggle and strain but the ropes only tighten.\n\r",ch);
        act("$n strains helplessly against $m bonds.",ch,NULL,NULL,TO_ROOM);
	return FALSE;
    }
    else if ( arg[0] == '\0' )
    {
	act( xsocial_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( xsocial_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( victim == ch )
    {
	act( xsocial_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM );
	act( xsocial_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR );
    }
/*    else if (ch->sex != SEX_MALE && ch->sex != SEX_FEMALE)
    {
	send_to_char("You must be either male or female to use these socials.\n\r",ch);
    }
    else if (victim->sex != SEX_MALE && victim->sex != SEX_FEMALE)
    {
	send_to_char("They must be either male or female for these socials.\n\r",ch);
    }
    else if (ch->sex == victim->sex)
    {
	send_to_char("Please stick to people of the opposite gender.\n\r",ch);
    }
*/    else
    {
	if (xsocial_table[cmd].gender == SEX_MALE && ch->sex != SEX_MALE)
	{
	    send_to_char("Only men can perform this type of social.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_FEMALE && ch->sex != SEX_FEMALE)
	{
	    send_to_char("Only women can perform this type of social.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_MALE && victim->sex != SEX_FEMALE)
	{
	    send_to_char("You can only perform this social on a woman.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_FEMALE && victim->sex != SEX_MALE)
	{
	    send_to_char("You can only perform this social on a man.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == 3 && ch->sex != SEX_FEMALE)
	{
		stc( "Only females may preform this command.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == 3 && victim->sex != SEX_FEMALE)
	{
		stc( "You can only preform this command on a female.\n\r",ch);
	}
	else if ( ( partner = victim->pcdata->partner ) == NULL || partner != ch )
  	{
         	stc("You cannot perform an xsocial on someone without their consent.\n\r",ch);
	 }
	else if (xsocial_table[cmd].stage == 0 && ch->pcdata->stage[0] < 1
	    && ch->pcdata->stage[2] > 0 && ch->sex == 5)
	    send_to_char("You have not yet recovered from last time!\n\r",ch);
	else if (xsocial_table[cmd].stage == 0 && victim->pcdata->stage[0] < 1
	    && victim->pcdata->stage[2] > 0 && victim->sex == 5)
	    send_to_char("They have not yet recovered from last time!\n\r",ch);
	else if (xsocial_table[cmd].stage > 0 && ch->pcdata->stage[0] < 100)
	    send_to_char("You are not sufficiently aroused.\n\r",ch);
	else if (xsocial_table[cmd].stage > 0 && victim->pcdata->stage[0] < 100)
	    send_to_char("They are not sufficiently aroused.\n\r",ch);
	else if (xsocial_table[cmd].stage > 1 && ch->pcdata->stage[1] < 1)
	    send_to_char("You are not in the right position.\n\r",ch);
	else if (xsocial_table[cmd].stage > 1 && victim->pcdata->stage[1] < 1)
	    send_to_char("They are not in the right position.\n\r",ch);
	else
	{
	    act(xsocial_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT);
	    act(xsocial_table[cmd].char_found,    ch, NULL, victim, TO_CHAR   );
	    act(xsocial_table[cmd].vict_found,    ch, NULL, victim, TO_VICT   );
	    if (xsocial_table[cmd].chance)
	    {
		if (ch->sex == SEX_FEMALE && 
		    !IS_EXTRA(ch, EXTRA_PREGNANT) && number_range(1,3) == 1) 
		make_preg(ch,victim);
		else if (victim->sex == SEX_FEMALE && 
		    !IS_EXTRA(victim, EXTRA_PREGNANT) && 
		    number_range(1,3) == 1) 
		make_preg(victim,ch);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-tie"))
	    {
		SET_BIT(victim->extra, TIED_UP);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-gag"))
	    {
		SET_BIT(victim->extra, GAGGED);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-blindfold"))
	    {
		SET_BIT(victim->extra, BLINDFOLDED);
	    }
	    if (xsocial_table[cmd].stage == 1)
	    {
		ch->pcdata->stage[1] = xsocial_table[cmd].position;
		victim->pcdata->stage[1] = xsocial_table[cmd].position;
		if (!IS_SET(ch->extra, EXTRA_DONE))
		{
		    SET_BIT(ch->extra, EXTRA_DONE);
		    if (ch->sex == SEX_FEMALE)
		    {
			act("You feel $n bleed as you enter $m.",ch,NULL,victim,TO_VICT);
			act("You feel yourself bleed as $N enters you.",ch,NULL,victim,TO_CHAR);
			ch->in_room->blood += 1;
		    }
		}
		if (!IS_SET(victim->extra, EXTRA_DONE))
		{
		    SET_BIT(victim->extra, EXTRA_DONE);
		    if (victim->sex == SEX_FEMALE)
		    {
			act("You feel $N bleed as you enter $M.",ch,NULL,victim,TO_CHAR);
			act("You feel yourself bleed as $n enters you.",ch,NULL,victim,TO_VICT);
			ch->in_room->blood += 1;
		    }
		}
		stage = 2;
	    }
	    else stage = xsocial_table[cmd].stage;
	    if (stage == 2) amount = ch->pcdata->stage[1];
		else amount = 100;
	    if (xsocial_table[cmd].self > 0)
	    {
		is_ok = FALSE;
		if (ch->pcdata->stage[stage] >= amount) is_ok = TRUE;
		ch->pcdata->stage[stage] += xsocial_table[cmd].self;
		if (!is_ok && ch->pcdata->stage[stage] >= amount) 
		{
		    stage_update(ch,victim,stage,xsocial_table[cmd].name);
		    one = TRUE;
		}
	    }
	    if (xsocial_table[cmd].other > 0)
	    {
		is_ok = FALSE;
		if (victim->pcdata->stage[stage] >= amount) is_ok = TRUE;
		victim->pcdata->stage[stage] += xsocial_table[cmd].other;
		if (!is_ok && victim->pcdata->stage[stage] >= amount) 
		{
		    stage_update(victim,ch,stage,xsocial_table[cmd].name);
		    two = TRUE;
		}
	    }
	    if ( one && two )
	    {
		    ch->pcdata->stage[0] = 0;
		    victim->pcdata->stage[0] = 0;

		if ( number_percent() > 60 )
		{
		    send_to_char("Congratulations on achieving a simultanious orgasm!\n\r",ch);
		    send_to_char("Congratulations on achieving a simultanious orgasm!\n\r",victim);
		}

	    }
	}
    }
    return TRUE;
}

void stage_update( CHAR_DATA *ch, CHAR_DATA *victim, int stage,char *argument )
{
    if (IS_NPC(ch) || IS_NPC(victim)) return;
    if (stage == 0)
    {
	if (ch->sex == SEX_MALE)
	{
	    send_to_char("You get a boner.\n\r",ch);
	    act("You feel $n get a boner.",ch,NULL,victim,TO_VICT);
	    return;
	}
	else if (ch->sex == SEX_FEMALE)
	{
	    send_to_char("You get wet.\n\r",ch);
	    act("You feel $n get wet.",ch,NULL,victim,TO_VICT);
	    return;
	}
    }
    else if (stage == 2)
    {
	if (ch->sex == SEX_MALE)
	{
	    if( str_cmp(argument,"xm-cum")   && str_cmp(argument,"xm-facial") && str_cmp(argument,"xm-canal") &&
		str_cmp(argument,"xm-canal") && str_cmp(argument,"xm-cbreasts") && str_cmp(argument,"xm-chair") &&
		str_cmp(argument,"xm-chand") && str_cmp(argument,"xm-cstomach") && str_cmp(argument,"xf-chands") &&
		str_cmp(argument,"xf-cbreasts") )
	    {
		act("You grit your teeth as you shoot your creamy load inside of $M.",ch,NULL,victim,TO_CHAR);
		act("$n grits his teeth as he shoots his load inside of you.",ch,NULL,victim,TO_VICT);
		act("$n grits his teeth as he shoots a load of cum inside of $N.",ch,NULL,victim,TO_NOTVICT);
	    }
	    ch->pcdata->genes[8] += 1;
	    victim->pcdata->genes[8] += 1;
	    save_char_obj(ch);
	    save_char_obj(victim);
	    if (victim->pcdata->stage[2] < 1 || victim->pcdata->stage[2] >= 250)
	    {
		ch->pcdata->stage[2] = 0;
		if (ch->pcdata->stage[0] >= 200) ch->pcdata->stage[0] -= 100;
	    }
	    else ch->pcdata->stage[2] = 200;
	    if (victim->sex == SEX_FEMALE && 
		!IS_EXTRA(victim, EXTRA_PREGNANT) && number_percent() <= 8) 
	    make_preg(victim,ch);
	    return;
	}
	else if (ch->sex == SEX_FEMALE)
	{
	    if( str_cmp(argument,"xf-cum") && str_cmp(argument,"xf-cface") )
	    {
		act("You whimper as you cum.",ch,NULL,victim,TO_CHAR);
		act("$n whimpers as $e cums.",ch,NULL,victim,TO_ROOM);
	    }
	    if (victim->pcdata->stage[2] < 1 || victim->pcdata->stage[2] >= 250)
	    {
		ch->pcdata->stage[2] = 0;
		if (ch->pcdata->stage[0] >= 200) ch->pcdata->stage[0] -= 100;
	    }
	    else ch->pcdata->stage[2] = 200;
	    return;
	}
    }
    return;
}

void make_preg( CHAR_DATA *mother, CHAR_DATA *father )
{
    char *strtime;
    char buf [MAX_STRING_LENGTH];

    if (IS_NPC(mother) || IS_NPC(father)) return;

    if (IS_SET(mother->affected_by2, AFF_CONTRACEPTION)) return;

  strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    free_string(mother->pcdata->conception);
    mother->pcdata->conception = str_dup(strtime);
    sprintf(buf,"%s %s",mother->name,father->name);
    free_string(mother->pcdata->cparents);
    mother->pcdata->cparents = str_dup(buf);
    SET_BIT(mother->extra, EXTRA_PREGNANT);
    mother->pcdata->genes[0] = (mother->max_hit + father->max_hit) * 0.5;
    mother->pcdata->genes[1] = (mother->max_mana + father->max_mana) * 0.5;
    mother->pcdata->genes[2] = (mother->max_move + father->max_move) * 0.5;
    if (IS_IMMUNE(mother, IMM_SLASH) && IS_IMMUNE(father, IMM_SLASH))
	SET_BIT(mother->pcdata->genes[3], IMM_SLASH);
    if (IS_IMMUNE(mother, IMM_STAB) && IS_IMMUNE(father, IMM_STAB))
	SET_BIT(mother->pcdata->genes[3], IMM_STAB);
    if (IS_IMMUNE(mother, IMM_SMASH) && IS_IMMUNE(father, IMM_SMASH))
	SET_BIT(mother->pcdata->genes[3], IMM_SMASH);
    if (IS_IMMUNE(mother, IMM_ANIMAL) && IS_IMMUNE(father, IMM_ANIMAL))
	SET_BIT(mother->pcdata->genes[3], IMM_ANIMAL);
    if (IS_IMMUNE(mother, IMM_MISC) && IS_IMMUNE(father, IMM_MISC))
	SET_BIT(mother->pcdata->genes[3], IMM_MISC);
    if (IS_IMMUNE(mother, IMM_CHARM) && IS_IMMUNE(father, IMM_CHARM))
	SET_BIT(mother->pcdata->genes[3], IMM_CHARM);
    if (IS_IMMUNE(mother, IMM_HEAT) && IS_IMMUNE(father, IMM_HEAT))
	SET_BIT(mother->pcdata->genes[3], IMM_HEAT);
    if (IS_IMMUNE(mother, IMM_COLD) && IS_IMMUNE(father, IMM_COLD))
	SET_BIT(mother->pcdata->genes[3], IMM_COLD);
    if (IS_IMMUNE(mother, IMM_LIGHTNING) && IS_IMMUNE(father, IMM_LIGHTNING))
	SET_BIT(mother->pcdata->genes[3], IMM_LIGHTNING);
    if (IS_IMMUNE(mother, IMM_ACID) && IS_IMMUNE(father, IMM_ACID))
	SET_BIT(mother->pcdata->genes[3], IMM_ACID);
    if (IS_IMMUNE(mother, IMM_VOODOO) && IS_IMMUNE(father, IMM_VOODOO))
	SET_BIT(mother->pcdata->genes[3], IMM_VOODOO);
    if (IS_IMMUNE(mother, IMM_HURL) && IS_IMMUNE(father, IMM_HURL))
	SET_BIT(mother->pcdata->genes[3], IMM_HURL);
    if (IS_IMMUNE(mother, IMM_BACKSTAB) && IS_IMMUNE(father, IMM_BACKSTAB))
	SET_BIT(mother->pcdata->genes[3], IMM_BACKSTAB);
    if (IS_IMMUNE(mother, IMM_KICK) && IS_IMMUNE(father, IMM_KICK))
	SET_BIT(mother->pcdata->genes[3], IMM_KICK);
    if (IS_IMMUNE(mother, IMM_DISARM) && IS_IMMUNE(father, IMM_DISARM))
	SET_BIT(mother->pcdata->genes[3], IMM_DISARM);
    if (IS_IMMUNE(mother, IMM_STEAL) && IS_IMMUNE(father, IMM_STEAL))
	SET_BIT(mother->pcdata->genes[3], IMM_STEAL);
    if (IS_IMMUNE(mother, IMM_SLEEP) && IS_IMMUNE(father, IMM_SLEEP))
	SET_BIT(mother->pcdata->genes[3], IMM_SLEEP);
    if (IS_IMMUNE(mother, IMM_DRAIN) && IS_IMMUNE(father, IMM_DRAIN))
	SET_BIT(mother->pcdata->genes[3], IMM_DRAIN);
    mother->pcdata->genes[4] = number_range(1,2);
    return;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit(*arg) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/* Syntax is:
disable - shows disabled commands
disable <command> - toggles disable status of command
*/

void do_disable (CHAR_DATA *ch, char *argument)
{
	int i;
	DISABLED_DATA *p,*q;
	char buf[100];
	
	if (IS_NPC(ch))
	{
		send_to_char ("RETURN first.\n\r",ch);
		return;
	}

	if (!argument[0]) /* Nothing specified. Show disabled commands. */
	{
		if (!disabled_first) /* Any disabled at all ? */
		{
			send_to_char ("There are no commands disabled.\n\r",ch);
			return;
		}

		send_to_char ("Disabled commands:\n\r"
		              "Command      Level   Disabled by\n\r",ch);
		                
		for (p = disabled_first; p; p = p->next)
		{
			sprintf (buf, "%-12s %5d   %-12s\n\r",p->command->name, p->level, p->disabled_by);
			send_to_char (buf,ch);
		}
		return;
	}
	
	/* command given */

	/* First check if it is one of the disabled commands */
	for (p = disabled_first; p ; p = p->next)
		if (!str_cmp(argument, p->command->name))
			break;
			
	if (p) /* this command is disabled */
	{
	/* Optional: The level of the imm to enable the command must equal or exceed level
	   of the one that disabled it */
		if (get_trust(ch) < p->level)
		{
			send_to_char ("This command was disabled by a higher power.\n\r",ch);
			return;
		}
		
		/* Remove */
		
		if (disabled_first == p) /* node to be removed == head ? */
			disabled_first = p->next;
		else /* Find the node before this one */
		{
			for (q = disabled_first; q->next != p; q = q->next); /* empty for */
			q->next = p->next;
		}
		
		free_string (p->disabled_by); /* free name of disabler */
		free_mem (p,sizeof(DISABLED_DATA)); /* free node */
		
		save_disabled(); /* save to disk */
		send_to_char ("Command enabled.\n\r",ch);
	}
	else /* not a disabled command, check if that command exists */
	{
		/* IQ test */
		if (!str_cmp(argument,"disable"))
		{
			send_to_char ("You cannot disable the disable command.\n\r",ch);
			return;
		}

		/* Search for the command */
		for (i = 0; cmd_table[i].name[0] != '\0'; i++)
			if (!str_cmp(cmd_table[i].name, argument))
				break;

		/* Found? */				
		if (cmd_table[i].name[0] == '\0')
		{
			send_to_char ("No such command.\n\r",ch);
			return;
		}

		/* Can the imm use this command at all ? */				
		if (cmd_table[i].level > get_trust(ch))
		{
			send_to_char ("You dot have access to that command; you cannot disable it.\n\r",ch);
			return;
		}
		
		/* Disable the command */
		
		p = alloc_mem (sizeof(DISABLED_DATA));

		p->command = &cmd_table[i];
		p->disabled_by = str_dup (ch->pcdata->switchname); /* save name of disabler */
		p->level = get_trust(ch); /* save trust */
		p->next = disabled_first;
		disabled_first = p; /* add before the current first element */
		
		send_to_char ("Command disabled.\n\r",ch);
		save_disabled(); /* save to disk */
	}
}

/* Check if that command is disabled 
   Note that we check for equivalence of the do_fun pointers; this means
   that disabling 'chat' will also disable the '.' command
*/   
bool check_disabled (const struct cmd_type *command)
{
	DISABLED_DATA *p;
	
	for (p = disabled_first; p ; p = p->next)
		if (p->command->do_fun == command->do_fun)
			return TRUE;

	return FALSE;
}

/* Load disabled commands */
void load_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	char *name;
	int i;
	
	disabled_first = NULL;
	
	fp = fopen (DISABLED_FILE, "r");
	
	if (!fp) /* No disabled file.. no disabled commands : */
		return;
		
	name = fread_word (fp);
	
	while (str_cmp(name, END_MARKER)) /* as long as name is NOT END_MARKER :) */
	{
		/* Find the command in the table */
		for (i = 0; cmd_table[i].name[0] ; i++)
			if (!str_cmp(cmd_table[i].name, name))
				break;
				
		if (!cmd_table[i].name[0]) /* command does not exist? */
		{
			bug ("Skipping uknown command in " DISABLED_FILE " file.",0);
			fread_number(fp); /* level */
			fread_word(fp); /* disabled_by */
		}
		else /* add new disabled command */
		{
			p = alloc_mem(sizeof(DISABLED_DATA));
			p->command = &cmd_table[i];
			p->level = fread_number(fp);
			p->disabled_by = str_dup(fread_word(fp)); 
			p->next = disabled_first;
			
			disabled_first = p;

		}
		
		name = fread_word(fp);
	}

	fclose (fp);		
}

/* Save disabled commands */
void save_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	
	if (!disabled_first) /* delete file if no commands are disabled */
	{
		unlink (DISABLED_FILE);
		return;
	}
	
	fp = fopen (DISABLED_FILE, "w");
	
	if (!fp)
	{
		bug ("Could not open " DISABLED_FILE " for writing",0);
		return;
	}
	
	for (p = disabled_first; p ; p = p->next)
		fprintf (fp, "%s %d %s\n", p->command->name, p->level, p->disabled_by);
		
	fprintf (fp, "%s\n",END_MARKER);
		
	fclose (fp);
}
