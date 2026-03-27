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


/* Special Bits */
#define UNI_AFF		1

#define SPC_CHAMPION	1 /* PLR_CHAMPION 4 */
#define SPC_DEMON_LORD	2 /* OLD_DEMON Flag */
#define SPC_WOLFMAN	4 /* old PLR_WOLFMAN */
#define SPC_PRINCE      8
#define SPC_GUY_GIA	16
#define SPC_GUY_NIN	32
#define SPC_GUY_WAR	64
#define SPC_GUY_TUR	128
#define SPC_GUY_RPR	256
#define SPC_SOR         512
#define SPC_MONK        1024
#define SPC_KNIGHT      2048
#define SPC_RECLASS     4096
#define SPC_DOPPLE      8192
#define SPC_ULTRA       16384
#define SPC_ULTIMATE    32768
#define SPC_SWORD       131072

/* Class Bits */
#define CLASS_DEMON	     1
#define CLASS_MAGE	     2
#define CLASS_WEREWOLF	     3
#define CLASS_VAMPIRE	     4
#define CLASS_HIGHLANDER     5
#define CLASS_DROW	     6
#define CLASS_MONK 	     7
#define CLASS_NINJA          8
#define CLASS_ANGEL          9
#define CLASS_DRAGOON       10
#define CLASS_DRAGON        11
#define CLASS_PALADIN       12
#define CLASS_AMAZON        13
#define CLASS_SORCEROR      14
#define CLASS_DIVINE_KNIGHT 15
#define CLASS_HUNTER        16
#define CLASS_DARK_KNIGHT   17
#define CLASS_DAYWALKER     18
#define CLASS_UNDEAD_KNIGHT 19
#define CLASS_NIGHT_BLADE   20
#define CLASS_SAIYAN        21
#define CLASS_SAMURAI       22
#define CLASS_WIZARD        23
#define CLASS_SUMMONER      24
#define CLASS_NECROMANCER   25
#define CLASS_DARAMON       26
#define CLASS_ASSASSIN      27
#define CLASS_SHADOW        28
#define CLASS_KODO          29
#define CLASS_SHAOLIN       30
#define CLASS_KURUDA        31
#define CLASS_GUYVER        32
#define CLASS_GAROU         33
#define CLASS_SOULREAVER    34
#define CLASS_DARK_ELF      35
#define CLASS_DRAGON_KNIGHT 36
#define CLASS_FIEND         37
#define CLASS_HOBBIT        38
#define CLASS_SNOW_ELF      39
#define CLASS_PHOENIX       40
#define CLASS_WISP          41
#define CLASS_DUERGAR       42
#define CLASS_SHADOW_WAR    43
#define CLASS_MASTER        44
#define CLASS_BEING         49

#define NPOWER_SORA 1
#define NPOWER_CHIKYU 2
#define NPOWER_NINGENNO 3
#define BLACK_BELT 4
#define HARA_KIRI 5
#define NINJAKI 9
#define NINJA_EMBODY 10

#define DROW_POWER	8
#define DROW_TOTAL	9
#define DROW_MAGIC	6

#define DEMON_CURRENT		      8	/* POWER_CURRENT 0*/
#define DEMON_TOTAL		      9	/* POWER_TOTAL   1*/
#define DEMON_POWER		     10 /* TOTAL ARMOUR BOUNS */
#define DEMON_PPOWER		     7 /* ARMOUR BONUS FROM PLAYERS */


/*Powers*/
#define DPOWER_FLAGS		      4 /* C_POWERS  1 */
#define DPOWER_CURRENT		      5 /* C_CURRENT 2 */
#define DPOWER_HEAD		      6 /* C_HEAD    3 */
#define DPOWER_TAIL		      11 /* C_TAIL    4 */
#define DPOWER_OBJ_VNUM		      12 /* DISC[10]  5 */
#define VPOWER_CURRENT		     18
#define DEM_UNFOLDED		      1
#define DEM_FANGS		      1
#define DEM_CLAWS		      2
#define DEM_DAMN		      3
#define DEM_HORNS		      4
#define DEM_HOOVES		      8
#define DEM_EYES		     16
#define DEM_WINGS		     32
#define DEM_MIGHT		     64
#define DEM_TOUGH		    128
#define DEM_SPEED		    256
#define DEM_TRAVEL		    512
#define DEM_SCRY		   1024
#define DEM_SHADOWSIGHT		   2048

/*new demon powers*/
#define DEM_CONE		1
#define DEM_STRENGTH		2
#define DEM_FORM		3
#define DEM_STAKE		4
#define DEM_SPEEDY		5
#define DEM_TRUE		6

/* Object Powers */
#define DEM_MOVE		   4096   /* 1 Can roll as an object */
#define DEM_LEAP		   8192   /* 2 Can leap out of someone's hands */
#define DEM_MAGIC		  16384   /* 4 Can cast spells as an object */
#define DEM_LIFESPAN      32768   /* 8 Can change lifespan */
#define DEM_HEAD		 131072
#define DEM_TAIL		 262144
#define DEM_SHIELD       524288
#define DEM_TRUESIGHT   1048576
#define DEM_GRAFT       2097152
#define DEM_IMMOLATE    4194304
#define DEM_INFERNO     8388608
#define DEM_CAUST      16777216
#define DEM_ENTOMB     33554432
#define DEM_FREEZEWEAPON  67108864
#define DEM_UNNERVE      121601760
#define DEM_LEECH        243203520
#define DEM_BLINK        486407040

/* Drow Powers */
#define DPOWER_DROWFIRE		1
#define DPOWER_DARKNESS		2
#define DPOWER_LIGHTS		4
#define DPOWER_DROWSIGHT	8
#define DPOWER_LEVITATION	16
#define DPOWER_DROWSHIELD	32
#define DPOWER_DROWPOISON	64
#define DPOWER_SHADOWWALK	128
#define DPOWER_GAROTTE		256
#define DPOWER_ARMS			512
#define DPOWER_DROWHATE		1024
#define DPOWER_SPIDERFORM	2048
#define DPOWER_WEB			4096
#define DPOWER_DGAROTTE		8192
#define DPOWER_CONFUSE		16384
#define DPOWER_GLAMOUR		32768
#define DPOWER_EARTHSHATTER	65536
#define DPOWER_SPEED		131072
#define DPOWER_TOUGHSKIN	262144
#define DPOWER_DARKTENDRILS 524288
#define DPOWER_FIGHTDANCE	1048576
#define DPOWER_OLATH             2097152
#define DPOWER_FORGIVE           4194304
#define DPOWER_DREWST            8388608
#define DPOWER_LUTH              16777216
#define DPOWER_VELKYN            33554432
#define DPOWER_ELAMSHIN          67108864
#define DPOWER_SHADOWSTRENGTH    134217728


#define DRAGON_TOTAL	7

/*
 * Death Knights
 */
#define UNDEAD_SPIRIT	19
#define WEAPONSKILL	18
#define POWER_TICK	17
#define POWER_FLAME	16
#define POWER_WORD	15
#define POWER_FREEZE	14
#define POWER_BLIND	13
#define INVOCATION	12
#define POWER_CHILL	11
#define POWER_BLAST	10

/* Garou Bits */
#define WEREWOLF_CLASS     0
#define WEREWOLF_RAGE      4
#define WEREWOLF_SKILLS    0
#define WEREWOLF_DEFENSE   3
#define WEREWOLF_RAGING    4
#define WEREWOLF_TOKEN     5

#define WEREWOLF_HYBRID    1
#define WEREWOLF_NATURAL   2

#define WEREWOLF_AUTO      2

/* SoulReaver Bits */
#define VAMPIRE_CLASS     0
#define VAMPIRE_BLOOD     2
#define VAMPIRE_MAXBLOOD  3
#define VAMPIRE_FORM      4
#define VAMPIRE_SKILLS    5
#define VAMPIRE_REAVER    6
#define VAMPIRE_SPIRIT    7

#define VAMPIRE_DAGGER    1
#define VAMPIRE_ARCANE    2
#define VAMPIRE_TELE      3

#define VAMPIRE_REAVERON  1

#define VAMPIRE_BAT       1
#define VAMPIRE_WOLF      2
#define VAMPIRE_MIST      3
#define VAMPIRE_MALICE    4

#define REAVER_FIRE       1
#define REAVER_DARK       2
#define REAVER_LIGHT      3
#define REAVER_CHILL      4
#define REAVER_STONE      5
#define REAVER_SHOCK      6
#define REAVER_SOUL       7
#define REAVER_FAITH      8

/* Dark Elf Bits */
#define DROW_CLASS        7
#define DROW_LEVEL        2

#define DROW_MILITIA      1
#define DROW_CLERICAL     2
#define DROW_MAGI         3
#define DROW_AVATAR       4

/* Fiend Bits */
#define DEMON_SOULS       0
#define DEMON_TRIGGER     3
#define DEMON_MTRIGGER    3

/* Kuruda Bits */
#define MONK_STOCK         0
#define MONK_ARAGAMI       1
#define MONK_SHIKI         5
#define MONK_RAGE          6
#define MONK_SKILL         9

#define MONK_INVIN         1

/* Dragon Knight Bits */
#define DRAGOON_CLASS       0
#define DRAGOON_LEVEL       2
#define DRAGOON_SPIRIT      3
#define DRAGOON_SPS         4
#define DRAGOON_RCOMBO1     5
#define DRAGOON_RCOMBO2     6
#define DRAGOON_RCOMBO3     7
#define DRAGOON_LCOMBO1     8
#define DRAGOON_LCOMBO2     9
#define DRAGOON_LCOMBO3    10
#define DRAGOON_LCOMBO4    11
#define DRAGOON_SCOMBO1    12
#define DRAGOON_SCOMBO2    13
#define DRAGOON_SCOMBO3    14
#define DRAGOON_SCOMBO4    15
#define DRAGOON_SCOMBO5    16
#define DRAGOON_SCOMBO6    17

#define DRAGOON_FIRE        1
#define DRAGOON_JADE        2
#define DRAGOON_SEA         4
#define DRAGOON_HOLY        8
#define DRAGOON_DARK       16
#define DRAGOON_IMPROVE    32
#define DRAGOON_GUARD      64
#define DRAGOON_EVADE     128
#define DRAGOON_USER      256
#define DRAGOON_DIVINE    512
