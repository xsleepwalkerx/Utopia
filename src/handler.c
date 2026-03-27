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
#include <string.h>
#include <time.h>
#include "merc.h"


AFFECT_DATA *		affect_free;



/*
 * Local functions.
 */
int get_fire_prof( CHAR_DATA *ch )
{
   int number = ch->fire_prof;

   if ( IS_SET( ch->elemental, ELE_WFIRE ) )
      return -100;

   if ( ch->outer_aura == A_RFIRE || ch->inner_aura == A_RFIRE || IS_SET(ch->group_aura, G_RFIRE) )
      number += 66;

   if ( ch->outer_aura == A_RCOLD || ch->inner_aura == A_RCOLD || IS_SET(ch->group_aura, G_RCOLD) )
      number -= 66;

   number += UMIN( 50, king_table[DEI_IFRIT].members * 2 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      return URANGE( -100, number, 100 );
}

int get_natural_prof( CHAR_DATA *ch )
{
   int number = ch->natural_prof;

   number += UMIN( 50, king_table[DEI_GAIA].members * 2 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      return URANGE( -100, number, 100 );
}

int get_lunar_prof( CHAR_DATA *ch )
{
   int number = ch->lunar_prof;

   if ( IS_SET( ch->elemental, ELE_WLUNAR ) )
      return -100;

   number += UMIN( 50, king_table[DEI_LUNA].members * 2 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      return URANGE( -100, number, 100 );
}

int get_ice_prof( CHAR_DATA *ch )
{
   int number = ch->ice_prof;

   if ( IS_SET( ch->elemental, ELE_WWATER ) )
      return -100;

   if ( ch->outer_aura == A_RFIRE || ch->inner_aura == A_RFIRE || IS_SET(ch->group_aura, G_RFIRE) )
      number -= 66;

   if ( ch->outer_aura == A_RCOLD || ch->inner_aura == A_RCOLD || IS_SET(ch->group_aura, G_RCOLD) )
      number += 66;

   number += UMIN( 50, king_table[DEI_SHIVA].members * 2 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      return URANGE( -100, number, 100 );
}

int get_thunder_prof( CHAR_DATA *ch )
{
   int number = ch->bolt_prof;

   if ( IS_SET( ch->elemental, ELE_WTHUNDER ) )
      return -100;

   if ( ch->outer_aura == A_REARTH || ch->inner_aura == A_REARTH || IS_SET(ch->group_aura, G_REARTH) )
      number -= 66;

   if ( ch->outer_aura == A_RTHUNDER || ch->inner_aura == A_RTHUNDER || IS_SET(ch->group_aura, G_RTHUNDER) )
      number += 66;

   number += UMIN( 50, king_table[DEI_BIRD].members * 2 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      return URANGE( -100, number, 100 );
}

int get_earth_prof( CHAR_DATA *ch )
{
   int number = ch->earth_prof;

   if ( IS_SET( ch->elemental, ELE_WEARTH ) )
      return -100;

   if ( ch->outer_aura == A_REARTH || ch->inner_aura == A_REARTH || IS_SET(ch->group_aura, G_REARTH) )
      number += 66;

   if ( ch->outer_aura == A_RTHUNDER || ch->inner_aura == A_RTHUNDER || IS_SET(ch->group_aura, G_RTHUNDER) )
      number -= 66;

   number += UMIN( 50, king_table[DEI_TITAN].members * 2 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      return URANGE( -100, number, 100 );
}

int get_holy_prof( CHAR_DATA *ch )
{
   int number = ch->holy_prof;

   if ( IS_SET( ch->elemental, ELE_WHOLY ) )
      return -100;

   if ( ch->outer_aura == A_RHOLY || ch->inner_aura == A_RHOLY || IS_SET(ch->group_aura, G_RHOLY) )
      number += 66;

   if ( ch->outer_aura == A_RDARK || ch->inner_aura == A_RDARK || IS_SET(ch->group_aura, G_RDARK) )
      number -= 66;

   number += UMIN( 50, king_table[DEI_SIREN].members * 2 );

//   number += ( ch->alignment / 10 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      else if ( is_affected( ch, skill_lookup( "demi protection" ) ) && number < 0 )
           return 0;
           else return URANGE( -100, number, 100 );
}

int get_dark_prof( CHAR_DATA *ch )
{
   int number = ch->dark_prof;

   if ( IS_SET( ch->elemental, ELE_WDARK ) )
      return -100;

   if ( ch->outer_aura == A_RHOLY || ch->inner_aura == A_RHOLY || IS_SET(ch->group_aura, G_RHOLY) )
      number -= 66;

   if ( ch->outer_aura == A_RDARK || ch->inner_aura == A_RDARK || IS_SET(ch->group_aura, G_RDARK) )
      number += 66;

   number += UMIN( 50, king_table[DEI_DIABLOS].members * 2 );

//   number -= ( ch->alignment / 10 );

   if ( IS_SET( ch->elemental, ELE_BTHROUGH ) && number > 0 )
      return 0;
      else if ( is_affected( ch, skill_lookup( "protection from evil" ) ) && number < 0 )
           return 0;
           else return URANGE( -100, number, 100 );
}

int get_slash_prof( CHAR_DATA *ch )
{
   int number = ch->slash_prof;

   return URANGE( -100, number, 100 );
}

int get_pierce_prof( CHAR_DATA *ch )
{
   int number = ch->pierce_prof;

   return URANGE( -100, number, 100 );
}

int get_blast_prof( CHAR_DATA *ch )
{
   int number = ch->blast_prof;

   return URANGE( -100, number, 100 );
}

int get_beam_prof( CHAR_DATA *ch )
{
   int number = ch->beam_prof;

   return URANGE( -100, number, 100 );
}

int get_blunt_prof( CHAR_DATA *ch )
{
   int number = ch->blunt_prof;

   return URANGE( -100, number, 100 );
}

int get_spirit_prof( CHAR_DATA *ch )
{
   int number = ch->spirit_prof;

   return URANGE( -100, number, 100 );
}

int get_physical_prof( CHAR_DATA *ch )
{
   int number = ch->physical_prof;

   return URANGE( -100, number, 100 );
}

int get_magical_prof( CHAR_DATA *ch )
{
   int number = ch->magical_prof;

   return URANGE( -100, number, 100 );
}


/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    if ( ch->trust != 0 )
	return ch->trust;

    if ( IS_NPC(ch) && ch->level >= LEVEL_HERO )
	return LEVEL_HERO - 1;
    else
	return ch->level;
}



/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
    return 17 + ( ch->played + (int) (current_time - ch->logon) ) / 7200;
}



/*
 * Retrieve character's current strength.
 */
int get_curr_str( CHAR_DATA *ch )
{
    int max = 99, mod = 0;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
       {
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             return 99;
             else return 75;
       }

    if ( IS_CLASS( ch, CLASS_MONK ) || IS_CLASS( ch, CLASS_NINJA ) )
       {
          if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
             return 1;
       }

    mod += ch->cmbt[2];

    if ( IS_SET( ch->more3, MORE3_DRAGON ) )
       mod += 5;
       else if ( IS_SET( ch->more3, MORE3_TIGER ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_MANTIS ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_FALCON ) || IS_SET( ch->more3, MORE3_CRANE ) )
               mod -= 2;
       else if ( IS_SET( ch->more3, MORE3_SNAKE ) )
               mod -= 3;

    if ( ch->ctimer[15] > 0 )
       mod -= 15;

    if ( weather_info.sky == SKY_APOCALYPSE && IS_CLASS( ch, CLASS_DEMON ) )
       mod += 15;
       else if ( weather_info.sky == SKY_ECLIPSE && IS_CLASS( ch, CLASS_DROW ) )
               mod += 15;

    if ( IS_CLASS( ch, CLASS_SHADOW ) && ch->gifts[CURRENT_METER] == AMI )
       mod -= 10;
       else if ( IS_CLASS( ch, CLASS_NECROMANCER ) && ch->class_2 == 0 )
               mod -= 5;

    if ( IS_SET( ch->more3, MORE3_YANG ) )
       mod -= 5;

    if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) && IS_SET( ch->affected_by, AFF_POLYMORPH ) 
         && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
       mod += 10;
       else if ( IS_SET( ch->more3, MORE3_YIN ) )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_ANGEL ) && ch->ctimer[8] > 0 )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_WEREWOLF ) )
               mod += ch->rage;
       else if ( IS_CLASS( ch, CLASS_PHOENIX ) )
               mod += ch->ctimer[8];
       else if ( IS_CLASS( ch, CLASS_FIEND ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_DRAGOON ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
               mod += UMIN( 6, 1 + ( ch->pkill / 4 ) );
       else if ( IS_CLASS( ch, CLASS_HOBBIT ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
               mod += 5;
       else if ( IS_CLASS( ch, CLASS_DRAGON ) )
               mod += 5;
       else if ( !IS_NPC(ch) && IS_CLASS( ch, CLASS_VAMPIRE ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_FIRE )
               mod += 5;

    if ( ch->outer_aura == A_STRENGTH || ch->inner_aura == A_STRENGTH || IS_SET(ch->group_aura, G_STRENGTH) )
       mod += 5;
       else if ( ch->outer_aura == A_RAGE || ch->inner_aura == A_RAGE || IS_SET(ch->group_aura, G_RAGE) )
               mod += 4;

    if ( IS_SET( ch->garou1, GAROU_PAWS ) )
       mod -= 4;

    if ( IS_EXTRA( ch, EXTRA_FLASH ) )
       mod += 2;

    if ( IS_SET( ch->polyaff, POLY_WOLF ) )
       mod += 3;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    if ( IS_CLASS( ch, CLASS_DUERGAR ) )
       {
          if ( IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
             mod += 10;
             else mod += 5;
       }

    if ( IS_SET( ch->more, MORE_TOUCH ) )
       mod += 3;

    if ( IS_NPC(ch) )
       return 10 + mod;

    if ( ch->pcdata->deity == DEI_DIABLOS || ch->pcdata->deity == DEI_IFRIT )
       mod += 5 + king_table[DEI_DIABLOS].members + king_table[DEI_IFRIT].members + king_table[DEI_LUNA].members
                + king_table[DEI_TITAN].members + king_table[DEI_ODIN].members;
       else if ( ch->pcdata->deity == DEI_SIREN )
               mod += -5 - king_table[DEI_SIREN].members - king_table[DEI_SHIVA].members - king_table[DEI_GAIA].members
                         - king_table[DEI_BIRD].members - king_table[DEI_CARB].members;
       else if ( ch->pcdata->deity == DEI_TITAN )
               mod += -5 - king_table[DEI_DIABLOS].members - king_table[DEI_IFRIT].members - king_table[DEI_LUNA].members
                      - king_table[DEI_TITAN].members - king_table[DEI_ODIN].members;

    mod += ch->pcdata->perm_str + ch->pcdata->mod_str;

    if ( ch->blocking == -10 )
       mod *= 1.25;

    return URANGE( 3, mod, max );
}

/*
 * Retrieve character's current evasion.
 */
int get_curr_eva( CHAR_DATA *ch )
{
    int max = 99, mod = 0;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
       {
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             return 99;
             else return 75;
       }

    if ( IS_SET( ch->more3, MORE3_FALCON ) )
       mod += 5;
       else if ( IS_SET( ch->more3, MORE3_SNAKE ) )
               mod += 7;
       else if ( IS_SET( ch->more3, MORE3_MANTIS ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_DRAGON ) )
               mod -= 5;

    if ( IS_SET( ch->newbits, NEW_DARKDODGE ) )
       mod += 5;

    if ( IS_SET( ch->more3, MORE3_SOUL ) )
       mod += 5;

    if ( IS_CLASS( ch, CLASS_WISP ) )
       mod += 15;
       else if ( IS_SET( ch->more, MORE_BLITZ ) )
               mod += 10;
       else if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) && IS_SET( ch->affected_by, AFF_POLYMORPH ) 
                 && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_ASSASSIN ) && ch->gifts[CURRENT_METER] == ALI )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_FIEND ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_NINJA ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_MONK ) )
               mod += 5;
       else if ( IS_CLASS( ch, CLASS_HOBBIT ) && is_oring( ch ) == TRUE )
               mod += 5;
       else if ( !IS_NPC(ch) && IS_CLASS( ch, CLASS_VAMPIRE ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SHOCK )
               mod += 3;

    if ( ch->outer_aura == A_DVISION || ch->inner_aura == A_DVISION || IS_SET( ch->group_aura, G_DVISION ) )
       mod += 5;

    if ( IS_SET( ch->cmbt[5], SKL2_SSTRIKE ) )
       mod++;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    if ( ch->outer_aura == A_RAGE || ch->inner_aura == A_RAGE || IS_SET(ch->group_aura, G_RAGE) )
       mod -= 5;

    if ( IS_SET( ch->polyaff, POLY_BAT ) )
       mod += 5;

    if ( ch->ctimer[15] > 0 )
       mod += 25;

    if ( IS_NPC(ch) )
	 return 10 + mod;

    if ( ch->pcdata->deity == DEI_SHIVA )
       mod += 5 + king_table[DEI_SIREN].members + king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members
              + king_table[DEI_BIRD].members + king_table[DEI_CARB].members;
       else if ( ch->pcdata->deity == DEI_BIRD )
               mod += -5 - king_table[DEI_SIREN].members - king_table[DEI_SHIVA].members - king_table[DEI_GAIA].members
                         - king_table[DEI_BIRD].members - king_table[DEI_CARB].members;

    return URANGE( 3, ch->pcdata->perm_eva + ch->pcdata->mod_eva + mod, max );
}

/*
 * Retrieve character's current agility.
 */
int get_curr_agi( CHAR_DATA *ch )
{
    int max = 20, mod = 0;

    if ( IS_SET( ch->more3, MORE3_MANTIS ) )
       mod += 2;
       else if ( IS_SET( ch->more3, MORE3_SNAKE ) )
               mod += 1;
       else if ( IS_SET( ch->more3, MORE3_DRAGON ) )
               mod -= 1;

    if ( ch->outer_aura == A_SPEED || ch->inner_aura == A_SPEED || IS_SET(ch->group_aura, G_SPEED) )
       mod += 8;

    if ( weather_info.sky == SKY_APOCALYPSE && IS_CLASS( ch, CLASS_DEMON ) )
       mod += 4;
       else if ( weather_info.sky == SKY_ECLIPSE && IS_CLASS( ch, CLASS_DROW ) )
               mod += 4;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    if ( IS_NPC(ch) )
	 return 5 + mod;

    if ( ch->pcdata->deity == DEI_ODIN )
       mod += 4 + king_table[DEI_DIABLOS].members + king_table[DEI_IFRIT].members + king_table[DEI_LUNA].members
                + king_table[DEI_TITAN].members + king_table[DEI_ODIN].members;
       else if ( ch->pcdata->deity == DEI_CARB )
               mod += -4 - king_table[DEI_SIREN].members - king_table[DEI_SHIVA].members - king_table[DEI_GAIA].members
                         - king_table[DEI_BIRD].members - king_table[DEI_CARB].members;

    mod += ch->pcdata->perm_agi + ch->pcdata->mod_agi;

    return URANGE( 1, mod, max );
}

/*
 * Retrieve character's current luck.
 */
int get_curr_luck( CHAR_DATA *ch )
{
    int max = 10, mod = 0;

    if ( IS_SET( ch->more3, MORE3_WOLF ) )
       mod -= 1;

    if ( IS_NPC(ch) )
	 return 5 + mod;

    if ( ch->pcdata->deity == DEI_ODIN )
       mod += -2 - king_table[DEI_DIABLOS].members - king_table[DEI_IFRIT].members - king_table[DEI_LUNA].members
                - king_table[DEI_TITAN].members - king_table[DEI_ODIN].members;
       else if ( ch->pcdata->deity == DEI_CARB )
               mod += 2 + king_table[DEI_SIREN].members + king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members
                         + king_table[DEI_BIRD].members + king_table[DEI_CARB].members;

    return URANGE( 0, ch->pcdata->perm_luck + ch->pcdata->mod_luck + mod, max );
}

/*
 * Retrieve character's current tension
 */
int get_curr_tens( CHAR_DATA *ch )
{
    int max = 33, mod = 0;

    if ( IS_NPC(ch) )
	 return 15 + mod;

    if ( IS_SET( ch->cmbt[5], SKL2_RBODY ) )
       mod += 5;

    return URANGE( 0, ch->pcdata->perm_tension + ch->pcdata->mod_tension + mod, max );
}

/*
 * Retrieve character's current intelligence.
 */
int get_curr_int( CHAR_DATA *ch )
{
    int max = 99, mod = 0;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
       {
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             return 99;
             else return 75;
       }

    if ( IS_SET( ch->more3, MORE3_WOLF ) )
       mod += 5;
       else if ( IS_SET( ch->more3, MORE3_MANTIS ) )
               mod += 3;

    if ( IS_NPC(ch) )
	 return 10 + mod;

    mod += ch->cmbt[2];

    if ( ch->pcdata->deity == DEI_DIABLOS || ch->pcdata->deity == DEI_LUNA )
       mod += -5 - king_table[DEI_DIABLOS].members - king_table[DEI_IFRIT].members - king_table[DEI_LUNA].members
                - king_table[DEI_TITAN].members - king_table[DEI_ODIN].members;
       else if ( ch->pcdata->deity == DEI_SIREN )
               mod += 5 + king_table[DEI_SIREN].members + king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members
                         + king_table[DEI_BIRD].members + king_table[DEI_CARB].members;

    if ( IS_SET( ch->polyaff, POLY_SERPENT ) )
       mod += 5;

    if ( ch->ctimer[15] > 0 )
       mod += 25;

    if ( weather_info.sky == SKY_APOCALYPSE && IS_CLASS( ch, CLASS_DEMON ) )
       mod += 15;
       else if ( weather_info.sky == SKY_ECLIPSE && IS_CLASS( ch, CLASS_DROW ) )
               mod += 15;

    if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) && IS_SET( ch->affected_by, AFF_POLYMORPH ) 
         && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
       mod += 10;
       else if ( IS_CLASS( ch, CLASS_ANGEL ) && ch->ctimer[8] > 0 )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_SHADOW ) && ch->gifts[CURRENT_METER] == AMI )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_KNIGHT ) )
               mod += 7;

    if ( ch->outer_aura == A_CONCENTRATION || ch->inner_aura == A_CONCENTRATION || IS_SET(ch->group_aura, G_CONCENTRATION) )
       mod += 5;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    mod += ch->pcdata->perm_int + ch->pcdata->mod_int;

    if ( IS_SET( ch->more2, MORE2_RAGE ) )
       mod += 4;

    if ( ch->blocking == -10 )
       mod *= 1.25;

    return URANGE( 3, mod, max );
}


/*
 * Retrieve character's current wisdom.
 */
int get_curr_wis( CHAR_DATA *ch )
{
    int max = 99, mod = 0;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
       {
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             return 99;
             else return 75;
       }

    if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) && IS_SET( ch->affected_by, AFF_POLYMORPH ) 
         && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
       mod += 10;
       else if ( IS_SET( ch->more3, MORE3_YANG ) )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_DARAMON ) )
               mod += 10;

    mod -= ch->cmbt[2];

    if ( IS_SET( ch->more3, MORE3_YIN ) )
       mod -= 5;

    if ( ch->outer_aura == A_SALVATION || ch->inner_aura == A_SALVATION )
       mod += 5;

    if ( IS_SET( ch->polyaff, POLY_SERPENT ) )
       mod += 5;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    if ( IS_NPC(ch) )
	 return 10 + mod;

    if ( ch->pcdata->deity == DEI_LUNA )
       mod += 5 + king_table[DEI_DIABLOS].members + king_table[DEI_IFRIT].members + king_table[DEI_LUNA].members
                + king_table[DEI_TITAN].members + king_table[DEI_ODIN].members;
       else if ( ch->pcdata->deity == DEI_IFRIT )
               mod += -5 - king_table[DEI_DIABLOS].members - king_table[DEI_IFRIT].members - king_table[DEI_LUNA].members
                      - king_table[DEI_TITAN].members - king_table[DEI_ODIN].members;

    if ( IS_SET( ch->more3, MORE3_CRANE ) )
       mod += 5;
       else if ( IS_SET( ch->more3, MORE3_MANTIS ) )
               mod += 3;

    return URANGE( 3, ch->pcdata->perm_wis + ch->pcdata->mod_wis + mod, max );
}



/*
 * Retrieve character's current dexterity.
 */
int get_curr_dex( CHAR_DATA *ch )
{
    int max = 99, mod = 0;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
       {
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             return 99;
             else return 75;
       }
       else if ( IS_NPC( ch ) && ch->pIndexData->vnum == 89 )
               return 50;

    if ( IS_SET( ch->more3, MORE3_TIGER ) )
       mod += 5;
       else if ( IS_SET( ch->more3, MORE3_SNAKE ) )
               mod += 5;
       else if ( IS_SET( ch->more3, MORE3_MANTIS ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_WOLF ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_FALCON ) || IS_SET( ch->more3, MORE3_CRANE ) )
               mod -= 2;

    if ( weather_info.sky == SKY_APOCALYPSE && IS_CLASS( ch, CLASS_DEMON ) )
       mod += 15;
       else if ( weather_info.sky == SKY_ECLIPSE && IS_CLASS( ch, CLASS_DROW ) )
               mod += 15;

    if ( IS_CLASS( ch, CLASS_DROW ) && IS_SET( ch->pcdata->powers[1], DPOWER_ELAMSHIN )
         && ch->hit < ch->max_hit * 0.25 )
       mod += 5;

    if ( ch->ctimer[15] > 0 )
       mod += 25;

    if ( IS_SET( ch->more, MORE_TOUCH ) )
       mod += 3;

    if ( IS_SET( ch->polyaff, POLY_WOLF ) )
       mod += 3;

    if ( IS_SET( ch->more3, MORE3_CLAWS ) )
       mod += 4;

    if ( ch->outer_aura == A_CONVICTION || ch->inner_aura == A_CONVICTION || IS_SET(ch->group_aura, G_CONVICTION) )
       mod += 5;
       else if ( ch->outer_aura == A_RAGE || ch->inner_aura == A_RAGE || IS_SET(ch->group_aura, G_RAGE) )
               mod += 5;
       else if ( ch->outer_aura == A_FANATICISM || ch->inner_aura == A_FANATICISM || IS_SET(ch->group_aura, G_FANATICISM) )
               mod += 4;
       else if ( ch->outer_aura == A_AIM || ch->inner_aura == A_AIM || IS_SET(ch->group_aura, G_AIM) )
               mod += 4;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    if ( IS_CLASS( ch, CLASS_SNOW_ELF ) )
       mod += 10;
       else if ( IS_SET( ch->more3, MORE3_YIN ) )
               mod += 10;
       else if ( IS_SET( ch->more3, MORE3_TURBO ) )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_ANGEL ) && ch->ctimer[8] > 0 )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_WEREWOLF ) )
               mod += ch->rage;
       else if ( IS_CLASS( ch, CLASS_PHOENIX ) )
               mod += ch->ctimer[8];
       else if ( IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_KNIGHT ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_DRAGOON ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_FIEND ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
               mod += 7;
       else if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
               mod += UMIN( 6, 1 + ( ch->pkill / 4 ) );
       else if ( IS_CLASS( ch, CLASS_MONK ) )
               mod += 5;
       else if ( IS_CLASS( ch, CLASS_HOBBIT ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
               mod += 5;
       else if ( !IS_NPC(ch) && IS_CLASS( ch, CLASS_VAMPIRE ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SHOCK )
               mod += 3;

    if ( IS_EXTRA( ch, EXTRA_FLASH ) )
       mod += 2;

    if ( IS_SET( ch->cmbt[5], SKL2_SSTRIKE ) )
       mod++;

    if ( IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) && IS_CLASS( ch, CLASS_DUERGAR ) )
       mod += 10;

    if ( IS_SET( ch->more3, MORE3_YANG ) )
       mod -= 5;

    if ( IS_NPC(ch) )
	 return 10 + mod;

    if ( ch->pcdata->deity == DEI_GAIA )
       mod += -5 - king_table[DEI_SIREN].members - king_table[DEI_SHIVA].members - king_table[DEI_GAIA].members
              - king_table[DEI_BIRD].members - king_table[DEI_CARB].members;
       else if ( ch->pcdata->deity == DEI_BIRD )
               mod += 5 + king_table[DEI_SIREN].members + king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members
                      + king_table[DEI_BIRD].members + king_table[DEI_CARB].members;

    return URANGE( 3, ch->pcdata->perm_dex + ch->pcdata->mod_dex + mod, max );
}



/*
 * Retrieve character's current constitution.
 */
int get_curr_con( CHAR_DATA *ch )
{
    int max = 99, mod = 0;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
       {
          if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
             return 99;
             else return 75;
       }

    if ( IS_SET( ch->more3, MORE3_CRANE ) )
       mod += 5;
       else if ( IS_SET( ch->more3, MORE3_DRAGON ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_MANTIS ) )
               mod += 3;
       else if ( IS_SET( ch->more3, MORE3_TIGER ) )
               mod -= 5;
       else if ( IS_SET( ch->more3, MORE3_SNAKE ) )
               mod -= 3;

    mod -= ch->cmbt[2];

    if ( ch->outer_aura == A_BSHIELD || ch->inner_aura == A_BSHIELD || IS_SET(ch->group_aura, G_BSHIELD) )
       mod += 5;

    if ( ch->outer_aura == A_VIGOR || ch->inner_aura == A_VIGOR || IS_SET(ch->group_aura, G_VIGOR) )
       mod += 5;

    if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
       {
          if ( ch->hit < ch->max_hit * 0.25 )
             mod += 4;
             else if ( ch->hit < ch->max_hit * 0.5 )
                     mod += 3;
             else if ( ch->hit < ch->max_hit * 0.75 )
                     mod += 2;
       }

    if ( IS_SET( ch->more3, MORE3_YIN ) )
       mod -= 5;

    if ( IS_CLASS( ch, CLASS_NECROMANCER ) && ch->class_2 == 0 )
       mod -= 10;

    if ( IS_CLASS( ch, CLASS_DUERGAR ) )
       mod += 10;
       else if ( IS_CLASS( ch, CLASS_PALADIN ) )
               mod += UMIN( 15, 5 + ( ch->pkill / 2 ) );
       else if ( IS_SET( ch->more3, MORE3_YANG ) )
               mod += 10;
       else if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) && IS_SET( ch->affected_by, AFF_POLYMORPH ) 
                 && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_DRAGON ) )
               mod += 10;
       else if ( IS_CLASS( ch, CLASS_DARAMON ) )
               mod += 5;

    if ( IS_NPC(ch) )
	 return 10 + mod;

    if ( ch->pcdata->deity == DEI_TITAN )
       mod += 5 + king_table[DEI_DIABLOS].members + king_table[DEI_IFRIT].members + king_table[DEI_LUNA].members
                + king_table[DEI_TITAN].members + king_table[DEI_ODIN].members;
       else if ( ch->pcdata->deity == DEI_SHIVA )
               mod += -5 - king_table[DEI_SIREN].members - king_table[DEI_SHIVA].members - king_table[DEI_GAIA].members
                         - king_table[DEI_BIRD].members - king_table[DEI_CARB].members;
       else if ( ch->pcdata->deity == DEI_GAIA )
               mod += 5 + king_table[DEI_SIREN].members + king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members
                         + king_table[DEI_BIRD].members + king_table[DEI_CARB].members;

    return URANGE( 3, ch->pcdata->perm_con + ch->pcdata->mod_con + mod, max );
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 100000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return MAX_WEAR + 2 * get_curr_dex( ch ) / 3;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return str_app[get_curr_str(ch)].carry;
}




/*
 * See if a string is one of the names of an object.
 */

bool is_name ( char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;

    /* fix crash on NULL namelist */
    if (namelist == NULL || namelist[0] == '\0')
        return FALSE;

    /* fixed to prevent is_name on "" returning TRUE */
    if (str[0] == '\0')
        return FALSE;

    string = str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
        str = one_argument(str,part);

        if (part[0] == '\0' )
            return TRUE;

        /* check to see if this is part of namelist */
        list = namelist;
        for ( ; ; )  /* start parsing namelist */
        {
            list = one_argument(list,name);
            if (name[0] == '\0')  /* this name was not found */
                return FALSE;

            if (!str_prefix(string,name))
                return TRUE; /* full pattern match */

            if (!str_prefix(part,name))
                break;
        }
    }
}


bool is_full_name( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
	if ( !str_cmp( str, name ) )
	    return TRUE;
    }
}



/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, OBJ_DATA *obj )
{
    OBJ_DATA *wield;
    int mod;

    mod = paf->modifier;

    if ( fAdd )
    {
	SET_BIT( ch->affected_by, paf->bitvector );
    }
    else
    {
	REMOVE_BIT( ch->affected_by, paf->bitvector );
	mod = 0 - mod;
    }

    if ( IS_NPC(ch) )
    {
    	switch ( paf->location )
    	{
	    default: break;
	    case APPLY_NONE:						break;
	    case APPLY_MANA:          ch->max_mana		+= mod;	break;
	    case APPLY_HIT:           ch->max_hit		+= mod;	break;
	    case APPLY_MOVE:          ch->max_move		+= mod;	break;
	    case APPLY_AC:            ch->armor			+= mod;	break;
	    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
	    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
	    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
	    case APPLY_POLY:	      ch->polyaff		+= mod;	break;
          case APPLY_SAVING_LUNAR: ch->lunar_prof += mod; ch->natural_prof -= mod; break;
          case APPLY_SAVING_NATURAL: ch->lunar_prof -= mod; ch->natural_prof += mod; break;
          case APPLY_SAVING_FIRE: ch->fire_prof += mod; ch->ice_prof -= mod; break;
          case APPLY_SAVING_ICE: ch->fire_prof -= mod; ch->ice_prof += mod; break;
          case APPLY_SAVING_THUNDER: ch->bolt_prof += mod; ch->earth_prof -= mod; break;
          case APPLY_SAVING_EARTH: ch->bolt_prof -= mod; ch->earth_prof += mod; break;
          case APPLY_SAVING_HOLY: ch->holy_prof += mod; ch->dark_prof -= mod; break;
          case APPLY_SAVING_DARK: ch->holy_prof -= mod; ch->dark_prof += mod; break;
          case APPLY_SAVING_SLASH: ch->slash_prof += mod; ch->pierce_prof -= mod; break;
          case APPLY_SAVING_PIERCE: ch->slash_prof -= mod; ch->pierce_prof += mod; break;
          case APPLY_SAVING_BLAST: ch->blast_prof += mod; ch->beam_prof -= mod; break;
          case APPLY_SAVING_BEAM: ch->blast_prof -= mod; ch->beam_prof += mod; break;
          case APPLY_SAVING_BLUNT: ch->blunt_prof += mod; ch->spirit_prof -= mod; break;
          case APPLY_SAVING_SPIRIT: ch->blunt_prof -= mod; ch->spirit_prof += mod; break;
          case APPLY_SAVING_NORMAL: ch->normal_prof += mod; break;
          case APPLY_SAVING_PHYSICAL: ch->physical_prof += mod; ch->magical_prof -= mod; break;
          case APPLY_SAVING_MAGICAL: ch->physical_prof -= mod; ch->magical_prof += mod; break;
          case APPLY_LUNARS: ch->lunar_bonus += mod; break;
          case APPLY_NATURALS: ch->natural_bonus += mod; break;
          case APPLY_DARKS: ch->dark_bonus += mod; break;
          case APPLY_HOLYS: ch->holy_bonus += mod; break;
          case APPLY_FIRES: ch->fire_bonus += mod; break;
          case APPLY_ICES: ch->ice_bonus += mod; break;
          case APPLY_BOLTS: ch->bolt_bonus += mod; break;
          case APPLY_EARTHS: ch->earth_bonus += mod; break;
	}
	return;
    }

    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->pcdata->mod_str	+= mod;	break;
    case APPLY_DEX:           ch->pcdata->mod_dex	+= mod;	break;
    case APPLY_INT:           ch->pcdata->mod_int	+= mod;	break;
    case APPLY_WIS:           ch->pcdata->mod_wis	+= mod;	break;
    case APPLY_CON:           ch->pcdata->mod_con	+= mod;	break;
    case APPLY_EVA:           ch->pcdata->mod_eva	+= mod;	break;
    case APPLY_AGI:           ch->pcdata->mod_agi	+= mod;	break;
    case APPLY_LUCK:          ch->pcdata->mod_luck	+= mod;	break;
    case APPLY_TENSION:       ch->pcdata->mod_tension	+= mod;	break;
    case APPLY_SEX:						break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:            ch->armor			+= mod;	break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_POLY:	      ch->polyaff		+= mod;	break;

    case APPLY_SAVING_LUNAR: ch->lunar_prof += mod; ch->natural_prof -= mod; break;
    case APPLY_SAVING_NATURAL: ch->lunar_prof -= mod; ch->natural_prof += mod; break;
    case APPLY_SAVING_FIRE: ch->fire_prof += mod; ch->ice_prof -= mod; break;
    case APPLY_SAVING_ICE: ch->fire_prof -= mod; ch->ice_prof += mod; break;
    case APPLY_SAVING_THUNDER: ch->bolt_prof += mod; ch->earth_prof -= mod; break;
    case APPLY_SAVING_EARTH: ch->bolt_prof -= mod; ch->earth_prof += mod; break;
    case APPLY_SAVING_HOLY: ch->holy_prof += mod; ch->dark_prof -= mod; break;
    case APPLY_SAVING_DARK: ch->holy_prof -= mod; ch->dark_prof += mod; break;
    case APPLY_SAVING_SLASH: ch->slash_prof += mod; ch->pierce_prof -= mod; break;
    case APPLY_SAVING_PIERCE: ch->slash_prof -= mod; ch->pierce_prof += mod; break;
    case APPLY_SAVING_BLAST: ch->blast_prof += mod; ch->beam_prof -= mod; break;
    case APPLY_SAVING_BEAM: ch->blast_prof -= mod; ch->beam_prof += mod; break;
    case APPLY_SAVING_BLUNT: ch->blunt_prof += mod; ch->spirit_prof -= mod; break;
    case APPLY_SAVING_SPIRIT: ch->blunt_prof -= mod; ch->spirit_prof += mod; break;
    case APPLY_SAVING_NORMAL: ch->normal_prof += mod; break;
    case APPLY_SAVING_PHYSICAL: ch->physical_prof += mod; ch->magical_prof -= mod; break;
    case APPLY_SAVING_MAGICAL: ch->physical_prof -= mod; ch->magical_prof += mod; break;
    case APPLY_LUNARS: ch->lunar_bonus += mod; break;
    case APPLY_DARKS: ch->dark_bonus += mod; break;
    case APPLY_HOLYS: ch->holy_bonus += mod; break;
    case APPLY_FIRES: ch->fire_bonus += mod; break;
    case APPLY_ICES: ch->ice_bonus += mod; break;
    case APPLY_BOLTS: ch->bolt_bonus += mod; break;
    case APPLY_EARTHS: ch->earth_bonus += mod; break;
    case APPLY_NATURALS: ch->natural_bonus += mod; break;
    }

    return;
}

/*
 * Remove an exit from a room					-Thoric
 */
void extract_exit( ROOM_INDEX_DATA *room, EXIT_DATA *pexit )
{
    UNLINK( pexit, room->first_exit, room->last_exit, next, prev );
    if ( pexit->rexit )
      pexit->rexit->rexit = NULL;
    STRFREE( pexit->keyword );
    STRFREE( pexit->description );
    DISPOSE( pexit );
}


void set_learnable_disciplines (CHAR_DATA *ch)
{
}



/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    if ( IS_SET(ch->more2, MORE2_SHIELD) )
       return;

    if ( affect_free == NULL )
    {
	paf_new		= alloc_perm( sizeof(*paf_new) );
    }
    else
    {
	paf_new		= affect_free;
	affect_free	= affect_free->next;
    }

    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE, NULL );
    return;
}



/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE, NULL );

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }

    paf->next	= affect_free;
    affect_free	= paf->next;
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;
    bool found;

    found = FALSE;
    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type && paf_old->location == paf->location )
	{
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    affect_to_char( ch, paf );
    return;
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if ( ch->in_room == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }

 //   if ( !IS_NPC(ch) )
//	--ch->in_room->area->nplayer;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;
    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->in_room->room_flags, ROOM_KIOTIC) )
        REMOVE_BIT(ch->in_room->room_flags, ROOM_KIOTIC);

    if ( IS_SET( ch->newbits, NEW_DARKNESS ) )
       {
          if ( ch->in_room->drows > 0 )
             ch->in_room->drows--;
       }

    if ( IS_SET( ch->more, MORE_CLOUD ) )
       REMOVE_BIT( ch->more, MORE_CLOUD );

    if ( IS_SET(ch->newbits, NEW_ALIGHT) && IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) ) 
       REMOVE_BIT( ch->in_room->area->bits, ROOM_ANTI_LIGHT );
  
    if ( ch->in_room->drows == 0 && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )  
       REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);  

    if ( IS_SET(ch->in_room->room_flags, ROOM_FLAMING) && ( IS_CLASS( ch, CLASS_DEMON ) || 
         IS_CLASS( ch, CLASS_PHOENIX ) ) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_FLAMING);

    if ( IS_SET(ch->in_room->room_flags, ROOM_FLAME) && IS_CLASS( ch, CLASS_PHOENIX ) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_FLAME);

    if ( IS_SET(ch->in_room->room_flags, ROOM_ASSASSIN) && IS_CLASS( ch, CLASS_ASSASSIN )
         && IS_SET( ch->special, SPC_SOR ) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_ASSASSIN);

    if ( IS_SET( ch->in_room->room_flags, ROOM_BLAZE ) && ( IS_CLASS( ch, CLASS_PHOENIX )
         || ( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 ) ) )
       REMOVE_BIT( ch->in_room->room_flags, ROOM_BLAZE );

    if ( IS_SET(ch->in_room->room_flags, ROOM_NOSTOCK) && IS_CLASS( ch, CLASS_ASSASSIN )
         && IS_SET( ch->special, SPC_KNIGHT ) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_NOSTOCK);

    if ( IS_SET(ch->in_room->room_flags, ROOM_ICYSWORD) && IS_CLASS( ch, CLASS_ASSASSIN )
         && IS_SET( ch->special, SPC_SOR ) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_ICYSWORD);

    if ( IS_SET( ch->in_room->room_flags, ROOM_ICETRAP ) && IS_CLASS( ch, CLASS_SNOW_ELF ) )
       REMOVE_BIT( ch->in_room->room_flags, ROOM_ICETRAP );

    if ( ch->in_room->trap > 0 )
       ch->in_room->trap = 0;

    if ( IS_SET(ch->more, MORE_SAFE) )
       {
         send_to_char( "The mystical force protecting your body leaves you.\n\r", ch );
         REMOVE_BIT(ch->more, MORE_SAFE);
       }

    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }

    ch->old_room     = ch->in_room;
    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    return;
}



/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    OBJ_DATA *obj;

    if ( !pRoomIndex )
    {
	char buf[MAX_STRING_LENGTH];

	sprintf( buf, "Char_to_room: %s -> NULL room!  Putting char in Crystal Cathedral (%d)", ch->name, ROOM_VNUM_ALTAR );
	bug( buf, 0 );
        /* This used to just return, but there was a problem with crashing
           and I saw no reason not to just put the char in limbo. -Narn */
        pRoomIndex = get_room_index( ROOM_VNUM_ALTAR );
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;
    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;

    if (ch->loc_hp[6] > 0 && ch->in_room->blood < 1000) ch->in_room->blood += 1;

    if ( IS_SET(ch->newbits, NEW_DARKNESS) )
       {
           ch->in_room->drows++;
  
           if ( !IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )  
              SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);  
       }

    if ( IS_SET(ch->newbits, NEW_ALIGHT) && !IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) ) 
       SET_BIT( ch->in_room->area->bits, ROOM_ANTI_LIGHT );

    if ( is_sound( ch ) )
       {
          if ( ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000 )
             stc("!!MUSIC(camelot.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 2000 && ch->in_room->vnum < 2500 )
                     stc("!!MUSIC(jad.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 800 && ch->in_room->vnum < 900 )
                     stc("!!MUSIC(cathedral.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 15000 && ch->in_room->vnum < 16000 )
                     stc("!!MUSIC(ancilla.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 14000 && ch->in_room->vnum < 15000 )
                     stc("!!MUSIC(cave.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 20300 && ch->in_room->vnum < 20400 )
                     stc("!!MUSIC(valley.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 20000 && ch->in_room->vnum < 20100 )
                     stc("!!MUSIC(groad.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 20200 && ch->in_room->vnum < 20300 )
                     stc("!!MUSIC(forest.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 20100 && ch->in_room->vnum < 20200 )
                     stc("!!MUSIC(mountain.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 17000 && ch->in_room->vnum < 18000 )
                     stc("!!MUSIC(academy.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 7000 && ch->in_room->vnum < 8000 )
                     stc("!!MUSIC(mystargia.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 3000 && ch->in_room->vnum < 4000 )
                     stc("!!MUSIC(apocalyse.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 4000 && ch->in_room->vnum < 5000 )
                     stc("!!MUSIC(fortress.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 16000 && ch->in_room->vnum < 17000 )
                     stc("!!MUSIC(nevarre.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 650 )
                     stc("!!MUSIC(arena.mid V=100 L=-1 C=1)\n\r",ch);
             else if ( ch->in_room->vnum >= 0 && ch->in_room->vnum < 100 )
                     stc("!!MUSIC(Off)\n\r",ch);
                     else stc("!!MUSIC(default.mid V=100 L=-1 C=1)\n\r",ch);
       }

    if ( ch->raiding > 0 && ch->in_room->area != ch->old_room->area )
       do_obtain( ch, "" );

    return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    if (obj == NULL) return;
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;

    obj->in_obj		 = NULL;
    ch->carry_number	+= 1;
    ch->carry_weight	+= get_obj_weight( obj );

    if ( IS_SET( obj->quest, QUEST_ARTIFACT ) )
       SET_BIT( ch->itemaffect, ITEMA_ARTIFACT );
       else if ( obj->chobj == NULL )
               obj->timer = -1;
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
    CHAR_DATA *ch;

    if (obj == NULL) return;
    if ( ( ch = obj->carried_by ) == NULL )
    {
	log_string( "Obj_from_char: null ch.");
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
	unequip_char( ch, obj );

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	  
  log_string( "Obj_from_char: obj not in list.");
    }

    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= 1;
    ch->carry_weight	-= get_obj_weight( obj );

    if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
       REMOVE_BIT(ch->itemaffect, ITEMA_ARTIFACT);
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear )
{
    if ( obj->item_type != ITEM_ARMOR )
	return 0;

    switch ( iWear )
    {
    case WEAR_BODY:	return 3 * obj->value[0];
    case WEAR_HEAD:	return 2 * obj->value[0];
    case WEAR_LEGS:	return 2 * obj->value[0];
    case WEAR_FEET:	return     obj->value[0];
    case WEAR_HANDS:	return     obj->value[0];
    case WEAR_ARMS:	return     obj->value[0];
    case WEAR_SHIELD:	return     obj->value[0];
    case WEAR_FINGER_L:	return     obj->value[0];
    case WEAR_FINGER_R: return     obj->value[0];
    case WEAR_NECK_1:	return     obj->value[0];
    case WEAR_NECK_2:	return     obj->value[0];
    case WEAR_ABOUT:	return 2 * obj->value[0];
    case WEAR_WAIST:	return     obj->value[0];
    case WEAR_WRIST_L:	return     obj->value[0];
    case WEAR_WRIST_R:	return     obj->value[0];
    case WEAR_HOLD:	return     obj->value[0];
    case WEAR_FACE:	return     obj->value[0];
    case WEAR_SCABBARD_L:return 0;
    case WEAR_SCABBARD_R:return 0;
    }

    return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == iWear )
	    return obj;
    }

    return NULL;
}



/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    AFFECT_DATA *paf;
    int sn;

    if (obj->item_type == ITEM_ARMOR)
      sn = obj->value[3];
    else
      sn = obj->value[0] / 1000;

    if ( IS_CLASS( ch, CLASS_HOBBIT ) && obj->pIndexData->vnum == 437
         && IS_SET( ch->more3, MORE3_HOUSE ) )
       do_famed_archangel( ch, "" );

    if ( obj->level > ch->tier )
       {
          act( "You are zapped by $p as it goes back in your inventory.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p as it goes back in $s inventory.", ch, obj, NULL, TO_ROOM);
          obj_from_char( obj );
          obj_to_char( obj, ch );
          do_autosave( ch, "" );
          return;
       }

    if ( obj->chobj != NULL && ( IS_NPC(ch) || IS_NPC(obj->chobj) || ch->pcdata->kingdom == 0 || ch->pcdata->kingdom
         != obj->chobj->pcdata->kingdom ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( IS_SET( obj->quest, QUEST_RELIC ) && ( IS_NPC( ch ) ||
        ( obj->questowner != NULL && str_cmp( obj->questowner, ch->pcdata->switchname ) ) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 168 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_PALADIN) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 370 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_UNDEAD_KNIGHT) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 95 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_DRAGON_KNIGHT) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 182 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_ANGEL) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 92 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_DEMON) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 310 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_MONK) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 246 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_DARAMON) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( obj->pIndexData->vnum == 122 && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_DRAGOON) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( ( obj->pIndexData->vnum == 439 )
           && ( IS_NPC(ch) || !IS_CLASS(ch,CLASS_ASSASSIN) || IS_SET(ch->special, SPC_MONK) ) )
       {
          act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR);
          act( "$n is zapped by $p and drops it.", ch, obj, NULL, TO_ROOM);
          obj_from_char(obj);
          obj_to_room(obj, ch->in_room);
          do_autosave(ch, "");
          return;
       }

    if ( IS_NPC(ch) && IS_SET(obj->quest,QUEST_RELIC) )
    {
	act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
	act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	do_autosave( ch, "" );
	return;
    }

    if ( iWear == WEAR_SCABBARD_L ||
    	iWear == WEAR_SCABBARD_R ) 
    {
	obj->wear_loc = iWear;
	return;
    }

    obj->wear_loc = iWear;

    ch->armor -= apply_ac(obj, obj->wear_loc);

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, TRUE, obj );
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, TRUE, obj );

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL )
	++ch->in_room->light;

    if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	&& str_cmp(obj->chpoweron,"(null)")
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	{
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
	    if (IS_SET(obj->spectype, SITEM_ACTION))
		kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_ROOM);
	}
    if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	&& str_cmp(obj->victpoweron,"(null)")
	&& !IS_SET(obj->spectype, SITEM_ACTION)
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);

    if ( obj->wear_loc == WEAR_NONE )
	return;

    if ( can_do_skill( ch, 24 ) )
       SET_BIT( ch->itemaffect, ITEMA_REAPER );

    if (   ((obj->item_type == ITEM_ARMOR ) && (obj->value[3] >= 1   ))
	|| ((obj->item_type == ITEM_WEAPON) && (obj->value[0] >= 1000))
	|| IS_SET(obj->spectype, SITEM_SILVER)
	|| IS_SET(obj->spectype, SITEM_DEMONIC)
	|| IS_SET(obj->spectype, SITEM_DROW)
	|| IS_SET(obj->quest, QUEST_ARTIFACT) )
    {

	if      ((sn ==  4) && (IS_AFFECTED(ch, AFF_BLIND)))        return;
	else if ((sn == 27) && (IS_AFFECTED(ch, AFF_DETECT_INVIS))) return;
	else if ((sn == 39) && (IS_AFFECTED(ch, AFF_FLYING)))       return;
	else if ((sn == 45) && (IS_AFFECTED(ch, AFF_INFRARED)))     return;
	else if ((sn == 46) && (IS_AFFECTED(ch, AFF_INVISIBLE)))    return;
	else if ((sn == 52) && (IS_AFFECTED(ch, AFF_PASS_DOOR)))    return;
	else if ((sn == 54) && (IS_AFFECTED(ch, AFF_PROTECT)))      return;
      else if ((sn == 139) && (IS_AFFECTED(ch, AFF_PROTECT_GOOD))) return;
	else if ((sn == 57) && (IS_AFFECTED(ch, AFF_SANCTUARY)))    return;
	else if ((sn == 2 ) && (IS_AFFECTED(ch, AFF_DETECT_INVIS))) return;
	else if ((sn == 3 ) && (IS_AFFECTED(ch, AFF_FLYING)))       return;
	else if ((sn == 1 ) && (IS_AFFECTED(ch, AFF_INFRARED)))     return;
	else if ((sn == 5 ) && (IS_AFFECTED(ch, AFF_INVISIBLE)))    return;
	else if ((sn == 6 ) && (IS_AFFECTED(ch, AFF_PASS_DOOR)))    return;
	else if ((sn == 7 ) && (IS_AFFECTED(ch, AFF_PROTECT)))      return;
	else if ((sn == 8 ) && (IS_AFFECTED(ch, AFF_SANCTUARY)))    return;
	else if ((sn == 9 ) && (IS_AFFECTED(ch, AFF_SNEAK)))        return;
	else if ((sn == 10) && (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))) return;
	else if ((sn == 11) && (IS_ITEMAFF(ch, ITEMA_FIRESHIELD)))  return;
	else if ((sn == 12) && (IS_ITEMAFF(ch, ITEMA_ICESHIELD)))   return;
	else if ((sn == 13) && (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD)))  return;
	else if ((sn == 15) && (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))) return;
	else if ((sn == 16) && (IS_ITEMAFF(ch, ITEMA_REGENERATE)))  return;
	else if ((sn == 17) && (IS_ITEMAFF(ch, ITEMA_SPEED)))       return;
	else if ((sn == 18) && (IS_ITEMAFF(ch, ITEMA_VORPAL)))      return;
	else if ((sn == 20) && (IS_ITEMAFF(ch, ITEMA_REFLECT)))     return;
	else if ((sn == 21) && (IS_ITEMAFF(ch, ITEMA_RESISTANCE)))  return;
	else if ((sn == 22) && (IS_ITEMAFF(ch, ITEMA_VISION)))      return;
	else if ((sn == 14) && (IS_ITEMAFF(ch, ITEMA_VISION)))      return;
	else if ((sn == 23) && (IS_ITEMAFF(ch, ITEMA_STALKER)))     return;
	else if ((sn == 24) && (IS_ITEMAFF(ch, ITEMA_VANISH)))      return;
	else if ((sn == 25) && (IS_ITEMAFF(ch, ITEMA_RAGER)))       return;
	else if ((sn == 60) && (IS_AFFECTED(ch, AFF_DETECT_HIDDEN)))return;
	else if ((sn == 29) && (IS_ITEMAFF(ch, ITEMA_AFFEYE)))      return;
	else if ((sn == 30) && (IS_ITEMAFF(ch, ITEMA_MONKCHI)))     return;
	else if ((sn == 50) && (IS_ITEMAFF(ch, ITEMA_PLASMA)))      return;
	else if ((sn == 61) && (IS_ITEMAFF(ch, ITEMA_DBUSTER)))     return;
	else if ((sn == 62) && (IS_ITEMAFF(ch, ITEMA_DSTAFF)))      return;
	else if ((sn == 63) && (IS_ITEMAFF(ch, ITEMA_GRAHF)))       return;
	if (sn == 4)
	{
	    SET_BIT(ch->affected_by, AFF_BLIND);
	    send_to_char( "You cannot see a thing!\n\r", ch );
	    act("$n seems to be blinded!",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 27 || sn == 2)
	{
	    SET_BIT(ch->affected_by, AFF_DETECT_INVIS);
	    send_to_char( "Your eyes tingle.\n\r", ch );
	    act("$n's eyes flicker with light.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 39 || sn == 3)
	{
	    SET_BIT(ch->affected_by, AFF_FLYING);
	    send_to_char( "Your feet rise off the ground.\n\r", ch );
	    act("$n's feet rise off the ground.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 45 || sn == 1)
	{
	    SET_BIT(ch->affected_by, AFF_INFRARED);
	    send_to_char( "Your eyes glow red.\n\r", ch );
	    act("$n's eyes glow red.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 46 || sn == 5)
	{
	    SET_BIT(ch->affected_by, AFF_INVISIBLE);
	    send_to_char( "You fade out of existance.\n\r", ch );
	    act("$n fades out of existance.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 52 || sn == 6)
	{
	    SET_BIT(ch->affected_by, AFF_PASS_DOOR);
	    send_to_char( "You turn translucent.\n\r", ch );
	    act("$n turns translucent.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 60)
	{
	    SET_BIT(ch->affected_by, AFF_DETECT_HIDDEN);
	    send_to_char( "You awarenes improves.\n\r", ch );
	    act("$n eyes tingle.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 54 || sn == 7)
	{
	    SET_BIT(ch->affected_by, AFF_PROTECT);
	    send_to_char( "You are surrounded by a divine aura.\n\r", ch );
	    act("$n is surrounded by a divine aura.",ch,NULL,NULL,TO_ROOM);
	}
        else if (sn == 139)
        {
            SET_BIT(ch->affected_by, AFF_PROTECT_GOOD);
            send_to_char( "You are surrounded by an unholy aura.\n\r", ch ); 
            act("$n is surrounded by an unholy aura.",ch,NULL,NULL,TO_ROOM);
        }
	else if (sn == 57 || sn == 8)
	{
          SET_BIT(ch->affected_by, AFF_SANCTUARY);
          send_to_char( "You are surrounded by a white aura.\n\r", ch );
          act("$n is surrounded by a white aura.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 9)
	{
	    SET_BIT(ch->affected_by, AFF_SNEAK);
	    send_to_char( "Your footsteps stop making any sound.\n\r", ch );
	    act("$n's footsteps stop making any sound.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 10)
	{
	    SET_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);
	    send_to_char( "You are surrounded by a crackling shield of lightning.\n\r", ch );
	    act("$n is surrounded by a crackling shield of lightning.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 11)
	{
	    SET_BIT(ch->itemaffect, ITEMA_FIRESHIELD);
	    send_to_char( "You are surrounded by a burning shield of flames.\n\r", ch );
	    act("$n is surrounded by a burning shield of flames.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 12)
	{
	    SET_BIT(ch->itemaffect, ITEMA_ICESHIELD);
	    send_to_char( "You are surrounded by a shimmering shield of ice.\n\r", ch );
	    act("$n is surrounded by a shimmering shield of ice.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 13)
	{
	    SET_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);
	    send_to_char( "You are surrounded by a bubbling shield of acid.\n\r", ch );
	    act("$n is surrounded by a bubbling shield of acid.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 30)
	{
	    SET_BIT(ch->itemaffect, ITEMA_MONKCHI);
	    send_to_char( "You feel the craving of Ultimate Destruction pulse through you.\n\r", ch );
	}
	else if (sn == 15)
	{
	    SET_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);
	    send_to_char( "You are surrounded by a swirling shield of chaos.\n\r", ch );
	    act("$n is surrounded by a swirling shield of chaos.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 16)
	    SET_BIT(ch->itemaffect, ITEMA_REGENERATE);
	else if (sn == 17)
	{
	    SET_BIT(ch->itemaffect, ITEMA_SPEED);
	    send_to_char( "You start moving faster than the eye can follow.\n\r", ch );
	    act("$n starts moving faster than the eye can follow.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 18)
	    SET_BIT(ch->itemaffect, ITEMA_VORPAL);
	else if (sn == 20)
	{
	    SET_BIT(ch->itemaffect, ITEMA_REFLECT);
	    send_to_char( "You are surrounded by flickering shield of darkness.\n\r", ch );
	    act("$n is surrounded by a flickering shield of darkness.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 21)
	    SET_BIT(ch->itemaffect, ITEMA_RESISTANCE);
	else if (sn == 22 || sn == 14)
	{
	    SET_BIT(ch->itemaffect, ITEMA_VISION);
	    send_to_char( "Your eyes begin to glow bright white.\n\r", ch );
	    act("$n's eyes begin to glow bright white.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 23)
	    SET_BIT(ch->itemaffect, ITEMA_STALKER);
	else if (sn == 24)
	{
	    SET_BIT(ch->itemaffect, ITEMA_VANISH);
	    send_to_char( "You blend into the shadows.\n\r", ch );
	    act("$n gradually fades into the shadows.",ch,NULL,NULL,TO_ROOM);
	}

	else if (sn == 29)
	{
	   SET_BIT(ch->itemaffect, ITEMA_AFFEYE);
	}
	else if (sn == 50)
	{
	   SET_BIT(ch->itemaffect, ITEMA_PLASMA);
	}

	else if (sn == 25 && !IS_NPC(ch))
	{
	    SET_BIT(ch->itemaffect, ITEMA_RAGER);
	    if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->rage < 100 && ch->fight_timer == 0 )
	    {
		ch->rage = 300;
		do_werewolf(ch,"");
	    }
	    else if (IS_CLASS(ch, CLASS_WEREWOLF))
		ch->rage = 300;
	    else if (IS_CLASS(ch, CLASS_VAMPIRE))
		ch->rage = 125;
	    else if (IS_CLASS(ch, CLASS_DEMON))
		ch->rage = 125;
	}
	else if ( sn == 80 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_PROTECTION);
	    send_to_char( "#CYou are #wprotected #yfully #Cby the #wSaint #LArjora#C.#n\n\r", ch );
	    act("#C$n#C's #ybody #w#zflickers#n #Cas a #wdivine #Lspirit #yshields #C$m.#n",ch,NULL,NULL,TO_ROOM);
	}
	else if ( sn == 81 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_SHIELD);
	}
	else if ( sn == 82 && ch->fight_timer == 0 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_DARKNESS);
          send_to_char("#PYou #ewrap a #ccloak #eof darkness around yourself.#n\n\r", ch );
	    act("#P$n #ewraps a #ccloak #eof darkness around $mself.#n",ch,NULL,NULL,TO_ROOM);

          if ( !IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
	       SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);

          if ( !IS_SET(ch->newbits, NEW_DARKNESS) )
             {
	          SET_BIT(ch->newbits, NEW_DARKNESS);
                ch->in_room->drows++;
             }
	}
	else if ( sn == 83 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_WEREWOLF);
          send_to_char("#yYour #Ceyes #Rfill with #wrage#R.#n\n\r", ch );
	    act("#y$n#y's #Ceyes #Rfill with #wrage#R.#n",ch,NULL,NULL,TO_ROOM);
	}
	else if ( sn == 84 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_EYES);
          send_to_char("#wYour eyes #Cglows #ybright yellow #was everything is shown before you.#n\n\r", ch );
	    act("#w$n#w's eyes #Cglows #ybright yellow#w.#n",ch,NULL,NULL,TO_ROOM);
	}
	else if ( sn == 85 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_REGEN);
	}
	else if ( sn == 61 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_DBUSTER);
	}
	else if ( sn == 62 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_DSTAFF);
	}
	else if ( sn == 63 )
	{
	    SET_BIT(ch->itemaffect, ITEMA_GRAHF);
	}

	if (IS_SET(obj->spectype, SITEM_DEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] < 15)
	    ch->pcdata->stats[DEMON_POWER] += 1;
	if( IS_SET(obj->spectype, SITEM_DROW) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] < 15)
            ch->pcdata->stats[DEMON_POWER] += 1;
	if (IS_SET(obj->spectype, SITEM_PDEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_PPOWER] < 15)
	    ch->pcdata->stats[DEMON_PPOWER] += 1;
    }

    if ( IS_CLASS( ch, CLASS_HOBBIT ) && obj->pIndexData->vnum == 437 )
       {
          ch->pcdata->deity = DEI_DIABLOS;

          if ( ch->fight_timer == 0 && !IS_SET( ch->act, PLR_WIZINVIS ) )
             {
                SET_BIT( ch->act, PLR_WIZINVIS );
                send_to_char( "You blend into the darkness.\n\r", ch );
                act("$n gradually fades into the darkness.",ch,NULL,NULL,TO_ROOM);
             }
       }

    return;
}

/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    AFFECT_DATA *paf;
    int sn;
    int oldpos = obj->wear_loc;

    if ( obj->wear_loc == WEAR_NONE )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

    if ( IS_CLASS( ch, CLASS_HOBBIT ) && obj->pIndexData->vnum == 437
         && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
       do_shadow_demon( ch, "" );

    obj->wear_loc = -1;

    ch->armor += apply_ac(obj, obj->wear_loc);

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE, obj );
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE, obj );

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if (!IS_NPC(ch) && (ch->desc != NULL && ch->desc->connected != CON_PLAYING)) return;

    if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	&& str_cmp(obj->chpoweroff,"(null)")
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	{
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
	    if (IS_SET(obj->spectype, SITEM_ACTION))
		kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_ROOM);
	}
    if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	&& str_cmp(obj->victpoweroff,"(null)")
	&& !IS_SET(obj->spectype, SITEM_ACTION)
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);

    if ( ((obj->item_type == ITEM_ARMOR ) && (obj->value[3] >= 1   ))
        || ((obj->item_type == ITEM_WEAPON) && (obj->value[0] >= 1000))
	|| IS_SET(obj->spectype, SITEM_SILVER)
	|| IS_SET(obj->spectype, SITEM_DEMONIC)
	|| IS_SET(obj->spectype, SITEM_DROW)
	|| IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	if (obj->item_type == ITEM_ARMOR)
	    sn = obj->value[3];
	else
	    sn = obj->value[0] / 1000;
	if (IS_AFFECTED(ch, AFF_BLIND) && (sn == 4)
		&& !is_affected(ch, 4))
	{
	    REMOVE_BIT(ch->affected_by, AFF_BLIND);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You can see again.\n\r", ch );
                act("$n seems to be able to see again.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_DETECT_INVIS) && (sn == 27 || sn == 2)
		&& !is_affected(ch, 27))
	{
	    REMOVE_BIT(ch->affected_by, AFF_DETECT_INVIS);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "Your eyes stop tingling.\n\r", ch );
                act("$n's eyes stop flickering with light.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_FLYING) && (sn == 39 || sn == 3)
		&& !is_affected(ch, 39))
	{
	    REMOVE_BIT(ch->affected_by, AFF_FLYING);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You slowly float to the ground.\n\r", ch );
                act("$n slowly floats to the ground.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_INFRARED) && (sn == 45 || sn == 1)
		&& !is_affected(ch, 45))
	{
	    REMOVE_BIT(ch->affected_by, AFF_INFRARED);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "Your eyes stop glowing red.\n\r", ch );
                act("$n's eyes stop glowing red.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_INVISIBLE) && (sn == 46 || sn == 5)
		&& !is_affected(ch, 46))
	{
	    REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You fade into existance.\n\r", ch );
                act("$n fades into existance.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_PASS_DOOR) && (sn == 52 || sn == 6)
		&& !is_affected(ch, 52))
	{
	    REMOVE_BIT(ch->affected_by, AFF_PASS_DOOR);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You feel solid again.\n\r", ch );
                act("$n is no longer translucent.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_PROTECT) && (sn == 54 || sn == 7)
		&& !is_affected(ch, 54))
	{
	    REMOVE_BIT(ch->affected_by, AFF_PROTECT);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The divine aura around you fades.\n\r", ch );
                act("The divine aura around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
        else if (IS_AFFECTED(ch, AFF_PROTECT_GOOD) && sn == 139
                && !is_affected(ch, 139))
        {
            REMOVE_BIT(ch->affected_by, AFF_PROTECT_GOOD);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The unholy aura around you fades.\n\r", ch );
                act("The unholy aura around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
        }
	else if (IS_AFFECTED(ch, AFF_SANCTUARY) && (sn == 57 || sn == 8)
		&& !is_affected(ch, 57))
	{
	    REMOVE_BIT(ch->affected_by, AFF_SANCTUARY);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The white aura around your body fades.\n\r", ch );
                act("The white aura about $n's body fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_SNEAK) && (sn == 9)
		&& !is_affected(ch, 80))
	{
	    REMOVE_BIT(ch->affected_by, AFF_SNEAK);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You are no longer moving so quietly.\n\r", ch );
                act("$n is no longer moving so quietly.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_AFFECTED(ch, AFF_DETECT_HIDDEN) && (sn == 60))
	{
	    REMOVE_BIT(ch->affected_by, AFF_DETECT_HIDDEN);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You feel less aware of your surrondings.\n\r",ch );
                act("$n eyes tingle.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD) && (sn == 10))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The crackling shield of lightning around you fades.\n\r", ch );
                act("The crackling shield of lightning around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD) && (sn == 11))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_FIRESHIELD);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The burning shield of fire around you fades.\n\r", ch );
                act("The burning shield of fire around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_ITEMAFF(ch, ITEMA_ICESHIELD) && (sn == 12))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_ICESHIELD);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The shimmering shield of ice around you fades.\n\r", ch );
                act("The shimmering shield of ice around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD) && (sn == 13))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The bubbling shield of acid around you fades.\n\r", ch );
                act("The bubbling shield of acid around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (IS_ITEMAFF(ch, ITEMA_PLASMA) && (sn == 50))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_PLASMA);
      }
	else if (IS_ITEMAFF(ch, ITEMA_MONKCHI) && (sn == 30))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_MONKCHI);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You no longer feel the need to destroy all.\n\r", ch );
             }
	}
	else if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD) && (sn == 15))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The swirling shield of chaos around you fades.\n\r", ch );
                act("The swirling shield of chaos around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (sn == 16)
	    REMOVE_BIT(ch->itemaffect, ITEMA_REGENERATE);
	else if (IS_ITEMAFF(ch, ITEMA_SPEED) && (sn == 17))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_SPEED);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "Your actions slow down to normal speed.\n\r", ch );
                act("$n stops moving at supernatural speed.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (sn == 29)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_AFFEYE);
	}

	else if (sn == 18)
	    REMOVE_BIT(ch->itemaffect, ITEMA_VORPAL);
	else if (IS_ITEMAFF(ch, ITEMA_REFLECT) && sn == 20)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_REFLECT);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "The flickering shield of darkness around you fades.\n\r", ch );
                act("The flickering shield of darkness around $n fades.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (sn == 21)
	    REMOVE_BIT(ch->itemaffect, ITEMA_RESISTANCE);
	else if (IS_ITEMAFF(ch, ITEMA_VISION) && ( sn == 22 || sn == 14 ) )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_VISION);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "Your eyes stop glowing bright white.\n\r", ch );
                act("$n's eyes stop glowing bright white.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (sn == 23)
	    REMOVE_BIT(ch->itemaffect, ITEMA_STALKER);
	else if (IS_ITEMAFF(ch, ITEMA_VANISH) && sn == 24)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_VANISH);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "You emerge from the shadows.\n\r", ch );
                act("$n gradually fades out of the shadows.",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if (sn == 25 && !IS_NPC(ch) && IS_ITEMAFF(ch, ITEMA_RAGER))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_RAGER);
	}
	else if ( IS_ITEMAFF(ch, ITEMA_PROTECTION) && sn == 80 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_PROTECTION);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char( "#CYou are no #ylonger #wprotected #Cby the #wSaint #LArjora#C.#n\n\r", ch );
                act("#C$n#C's #ybody stops #w#zflickering#n #Cas the #wdivine #Lspirit #Cleaves $m.#n",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if ( IS_ITEMAFF(ch, ITEMA_SHIELD) && sn == 81 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_SHIELD);
	}
	else if ( IS_ITEMAFF(ch, ITEMA_DARKNESS) && sn == 82 )
	{
          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char("#PYou banish your #ccloak #eof darkness.#n\n\r", ch );
                act("#P$n #ebanishes $s #ccloak #eof darkness.#n",ch,NULL,NULL,TO_ROOM);
             }

          if ( IS_SET(ch->itemaffect, ITEMA_DARKNESS) )
             REMOVE_BIT(ch->itemaffect, ITEMA_DARKNESS);

          if ( IS_SET(ch->newbits, NEW_DARKNESS) )
             {
	          REMOVE_BIT(ch->newbits, NEW_DARKNESS);

                if ( ch->in_room->drows > 0 )
                   {
                      ch->in_room->drows--;

                      if ( ch->in_room->drows == 0 )
                         {
                            if ( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
	                         REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
                         }
                   }
             }
	}
	else if ( IS_ITEMAFF(ch, ITEMA_WEREWOLF) && sn == 83 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_WEREWOLF);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char("#yYour #Ceyes #Rlose your #wrage#R.#n\n\r", ch );
                act("#y$n#y's #Ceyes #Rlose its #wrage#R.#n",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if ( IS_ITEMAFF(ch, ITEMA_EYES) && sn == 84 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_EYES);

          if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
             {
                send_to_char("#wYour eyes #wstops #gglowing yellow#w.#n\n\r", ch );
                act("#w$n#w's eyes #wstops #yglowing yellow#w.#n",ch,NULL,NULL,TO_ROOM);
             }
	}
	else if ( IS_ITEMAFF(ch, ITEMA_REGEN) && sn == 85 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_REGEN);
	}
	else if ( IS_ITEMAFF(ch, ITEMA_DBUSTER) && sn == 61 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_DBUSTER);
	}
	else if ( IS_ITEMAFF(ch, ITEMA_DSTAFF) && sn == 62 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_DSTAFF);
	}
	else if ( IS_ITEMAFF(ch, ITEMA_GRAHF) && sn == 63 )
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_GRAHF);
	}

	if (IS_SET(obj->spectype, SITEM_DEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] > 0)
	    ch->pcdata->stats[DEMON_POWER] -= 1;
	if( IS_SET(obj->spectype, SITEM_DROW) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] > 0)
            ch->pcdata->stats[DEMON_POWER] -= 1;
	if (IS_SET(obj->spectype, SITEM_PDEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_PPOWER] > 0)
	    ch->pcdata->stats[DEMON_PPOWER] -= 1;
    }

    if ( IS_CLASS( ch, CLASS_HOBBIT ) && obj->pIndexData->vnum == 437 )
       {
          ch->pcdata->deity = DEI_SIREN;

          if ( IS_SET( ch->act, PLR_WIZINVIS ) )
             {
                REMOVE_BIT( ch->act, PLR_WIZINVIS );

                if ( !IS_SET( ch->more3, MORE3_NOSHOW ) )
                   {
                       send_to_char( "You blend from the darkness.\n\r", ch );
                       act("$n gradually fades from the darkness.",ch,NULL,NULL,TO_ROOM);
                   }
             }
       }

    if ( IS_SET( ch->itemaffect, ITEMA_REAPER ) && !can_do_skill( ch, 24 ) )
       {
          REMOVE_BIT( ch->itemaffect, ITEMA_REAPER );
          ch->cmbt[2] = 0;
       }
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;

    if (obj == NULL) return;
    if ( ( in_room = obj->in_room ) == NULL )
    {
	bug( "obj_from_room: NULL.", 0 );
	return;
    }

    if ( obj == in_room->contents )
    {
	in_room->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = in_room->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_room: obj not found.", 0 );
	    return;
	}
    }

    obj->in_room      = NULL;
    obj->next_content = NULL;
    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
    if (obj == NULL) return;
    if (pRoomIndex == NULL) return;
    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;
    return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
    if ( obj == NULL )
       return;

    obj->next_content		= obj_to->contains;
    obj_to->contains		= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;

    if ( obj->item_type == ITEM_BOMB && !IS_SET( obj_to->extra_flags, ITEM_DETONATE ) )
       SET_BIT( obj_to->extra_flags, ITEM_DETONATE );

    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )
    {
	if ( obj_to->carried_by != NULL && !IS_SET(obj_to->spectype,SITEM_MORPH))
	    obj_to->carried_by->carry_weight += get_obj_weight( obj );
    }
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;

    if (obj == NULL) return;
    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: null obj_from.", 0 );
	return;
    }

    if ( obj == obj_from->contains )
    {
	obj_from->contains = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->contains; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )
    {
	if ( obj_from->carried_by != NULL && !IS_SET(obj_from->spectype,SITEM_MORPH))
	    obj_from->carried_by->carry_weight -= get_obj_weight( obj );
    }

    return;
}



/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA *obj )
{
    CHAR_DATA *ch;
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;

    if ( IS_SET( obj->extra_flags, ITEM_AUCTION ) )
       remove_auction_o( obj );

    if ( obj == NULL ) return;
    if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );

    if ( ( ch = obj->chobj ) != NULL
    &&   !IS_NPC(ch)
    &&   ch->pcdata->chobj == obj
    &&   IS_HEAD(ch,LOST_HEAD))
    {
	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	free_string(ch->morph);
	ch->morph = str_dup("");

      if ( arena == FIGHT_WAR )
         {
            ch->safe_timer = 2;
            ch->login_timer = 2;
         }
         else {
                 ch->safe_timer = 3;
                 ch->login_timer = 2;
              }

      if ( nogang == 10 && !armageddon )
         restore_char( ch );

      ch->pcdata->capture_timer = 0;
     	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	ch->pcdata->chobj = NULL;
	obj->chobj = NULL;
	send_to_char("You have been KILLED!!\n\r",ch);
	do_look(ch,"auto");

      if ( IS_CLASS( ch, CLASS_WISP ) )
         {
            char buf[MAX_STRING_LENGTH];

            sprintf(buf,"#w%s the #yWisp#n", ch->pcdata->switchname );
            free_string( ch->morph );
            ch->morph = str_dup(buf);
            SET_BIT(ch->affected_by, AFF_POLYMORPH);
         }
         else if ( IS_CLASS( ch, CLASS_PHOENIX ) )
         {
            char buf[MAX_STRING_LENGTH];

            sprintf(buf,"#R%s #ethe #Rf#ri#ye#Rr#ry #RP#rh#eo#ye#en#Ri#rx#n", ch->pcdata->switchname );
            free_string( ch->morph );
            ch->morph = str_dup(buf);
            SET_BIT(ch->affected_by, AFF_POLYMORPH);
         }
    }
    else if ( ( ch = obj->chobj ) != NULL
    &&   !IS_NPC(ch)
    &&   ch->pcdata->chobj == obj
    &&   (IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0))
    {
       if ( IS_EXTRA( ch, EXTRA_OSWITCH ) )
          REMOVE_BIT(ch->extra,EXTRA_OSWITCH);

       REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
       free_string(ch->morph);
       ch->morph = str_dup("");
       ch->pcdata->chobj = NULL;
       obj->chobj = NULL;
       send_to_char("You return to your body.\n\r",ch);
       ch->pcdata->obj_vnum = 0;

      if ( IS_CLASS( ch, CLASS_WISP ) )
         {
            char buf[MAX_STRING_LENGTH];

            sprintf(buf,"#w%s the #yWisp#n", ch->pcdata->switchname );
            free_string( ch->morph );
            ch->morph = str_dup(buf);
            SET_BIT(ch->affected_by, AFF_POLYMORPH);
         }
    }
    else if ( obj->chobj != NULL )
    {
	if (!IS_NPC(obj->chobj)) obj->chobj->pcdata->chobj = NULL;
	obj->chobj = NULL;
	bug( "Extract_obj: obj %d chobj invalid.", obj->pIndexData->vnum );
    }

    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
    {
	obj_next = obj_content->next_content;
	if (obj->contains != NULL) extract_obj( obj->contains );
    }

    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }

    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	for ( paf = obj->affected; paf != NULL; paf = paf_next )
	{
	    paf_next    = paf->next;
	    paf->next   = affect_free;
	    affect_free = paf;
	}
    }

    {
	EXTRA_DESCR_DATA *ed;
	EXTRA_DESCR_DATA *ed_next;

	for ( ed = obj->extra_descr; ed != NULL; ed = ed_next )
	{
	    ed_next		= ed->next;
	    free_string( ed->description );
	    free_string( ed->keyword     );
	    extra_descr_free	= ed;
	}
    }

    free_string( obj->name        );
    free_string( obj->description );
    free_string( obj->short_descr );
    if (obj->chpoweron    != NULL) free_string( obj->chpoweron   );
    if (obj->chpoweroff   != NULL) free_string( obj->chpoweroff  );
    if (obj->chpoweruse   != NULL) free_string( obj->chpoweruse  );
    if (obj->victpoweron  != NULL) free_string( obj->victpoweron );
    if (obj->victpoweroff != NULL) free_string( obj->victpoweroff);
    if (obj->victpoweruse != NULL) free_string( obj->victpoweruse);
    if (obj->questmaker   != NULL) free_string( obj->questmaker  );
    if (obj->questowner   != NULL) free_string( obj->questowner  );
    --obj->pIndexData->count;
    obj->next	= obj_free;
    obj_free	= obj;
    return;
}



/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA *ch, bool fPull )
{
    CHAR_DATA *wch;
    CHAR_DATA *familiar;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( ch == NULL ) return;

    if ( ch->in_room == NULL )
    {
	bug( "Extract_char: NULL.", 0 );
	return;
    }

    if ( fPull )
	die_follower( ch );

    stop_fighting( ch, TRUE );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	extract_obj( obj );
    }
    
    char_from_room( ch );

    if ( IS_NPC(ch) )
	--ch->pIndexData->count;
    else if ( ch->pcdata->chobj != NULL )
    {
	ch->pcdata->chobj->chobj = NULL;
	ch->pcdata->chobj = NULL;
    }

    if ( !fPull )
    {
	char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ) );
	return;
    }

    if ( ch->desc != NULL && ch->desc->original != NULL )
	do_return( ch, "" );

    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch->reply == ch )
	    wch->reply = NULL;
    }

    if ( ch == char_list )
    {
       char_list = ch->next;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = char_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == ch )
	    {
		prev->next = ch->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc )
	ch->desc->character = NULL;

    if ( (wizard = ch->wizard) != NULL)
    {
	if (!IS_NPC(wizard)) wizard->pcdata->familiar = NULL;
	ch->wizard = NULL;
    }
    if ( !IS_NPC(ch) )
    {
	if ((familiar = ch->pcdata->familiar) != NULL)
	{
	    familiar->wizard = NULL;
	    ch->pcdata->familiar = NULL;
	    if (IS_NPC(familiar))
	    {
		act("$n slowly fades away to nothing.",familiar,NULL,NULL,TO_ROOM);
		extract_char(familiar,TRUE);
	    }
	}
	if ((familiar = ch->pcdata->partner) != NULL)
	    ch->pcdata->partner = NULL;
	if ((familiar = ch->pcdata->propose) != NULL)
	    ch->pcdata->propose = NULL;
	for ( familiar = char_list; familiar != NULL; familiar = familiar->next)
	{
	    if ( !IS_NPC(familiar) && familiar->pcdata->propose != NULL && 
		familiar->pcdata->propose == ch )
		familiar->pcdata->propose = NULL;
	    if ( !IS_NPC(familiar) && familiar->pcdata->partner != NULL && 
		familiar->pcdata->partner == ch )
		familiar->pcdata->partner = NULL;
	}
    }
    else if (IS_NPC(ch) && strlen(ch->lord) > 1)
    {
	for ( wch = char_list; wch != NULL ; wch = wch->next )
	{
	    if (IS_NPC(wch)) continue;
	    if (str_cmp(wch->name, ch->lord)) continue;
	    if (wch->pcdata->followers > 0) wch->pcdata->followers--;
	}
    }

    free_char( ch );
    return;
}


/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_area( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ch->seeself > 0 )
       return get_your_name( ch, argument );

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( !IS_NPC(wch) && IS_HEAD(wch, LOST_HEAD) ) continue;
	else if ( !IS_NPC(wch) && IS_EXTRA(wch, EXTRA_OSWITCH) ) continue;
	else if ( !can_see( ch, wch ) || ( !is_name( arg, wch->name ) &&
	 (IS_NPC(wch) || !is_name(arg, wch->pcdata->switchname)) &&
	    ( IS_NPC(wch) || !is_name( arg, wch->morph ))))
	    continue;

      if ( ch->in_room->area != wch->in_room->area )
         continue;

	if ( ++count == number )
	    return wch;
    }

    return NULL;
}

/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch = NULL;
    int number;
    int count;

    number = number_argument( argument, arg );
    count = 0;

    if ( ch->seeself > 0 )
       return get_your_name( ch, argument );

    if ( !str_cmp( arg, "self" ) && (IS_NPC( ch ) || ch->pcdata->chobj == NULL ) )
       return ch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
       if ( !IS_NPC(rch) && IS_HEAD(rch, LOST_HEAD) )
          continue;
          else if ( !IS_NPC(rch) && IS_EXTRA(rch, EXTRA_OSWITCH) )
                  continue;
          else if ( !can_see( ch, rch ) || ( !is_name( arg, rch->name ) &&
                    ( IS_NPC(rch) || !is_name(arg, rch->pcdata->switchname) ) &&
                    ( IS_NPC(rch) || !is_name( arg, rch->morph ) ) ) )
                  continue;

	if ( ++count == number )
	   break;
    }

    if ( rch == NULL )
       return rch;

    if ( IS_SET( ch->more3, MORE3_CONFUSE ) && number_percent() > 60 && rch != ch )
       {
          count = 0;

          stc( "#wIn your #yconfusion#w, #Cyou #wswing #yblindly #wat a random target.#n\n\r", ch );

          for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
          {
             count++;
          }

          number = number_range( 1, count );
          count = 0;

          for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
          {
             count++;

             if ( !IS_NPC(rch) && IS_HEAD(rch, LOST_HEAD) ) continue;
                else if ( !IS_NPC(rch) && IS_EXTRA(rch, EXTRA_OSWITCH) ) continue;
                else if ( !can_see( ch, rch ) ) continue;

             if ( count == number )
                return rch;
          }
       }
       else if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_PSYCHO && number_percent() > 90
                 && ch->fight_timer > 0 && rch != ch )
       {
          count = 0;
          ch->cmbt[1] = 5;

          stc( "#wIn your #Rrage#w, #Cyou #wswing #Rwildly #wat a random target.#n\n\r", ch );

          for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
          {
             count++;
          }

          number = number_range( 1, count );
          count = 0;

          for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
          {
             count++;

             if ( !IS_NPC(rch) && IS_HEAD(rch, LOST_HEAD) ) continue;
                else if ( !IS_NPC(rch) && IS_EXTRA(rch, EXTRA_OSWITCH) ) continue;
                else if ( !can_see( ch, rch ) ) continue;

             if ( count == number )
                return rch;
          }
       }

    return rch;
}

/*
 * Find a char in the world in corresponde to your name, like get 2.jax, 3.jax etc.
 */
CHAR_DATA *get_your_name( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *wch;
   int number;
   int count;

   number = number_argument( argument, arg );
   count = 0;

   if ( !IS_NPC( ch ) && !is_name( arg, ch->pcdata->switchname ) )
      return NULL;

   if ( IS_NPC( ch ) && !is_name( arg, ch->name ) )
      return NULL;

   for ( wch = ch->in_room->people; wch != NULL; wch = wch->next_in_room )
   {
     if ( !IS_NPC(wch) && IS_HEAD(wch, LOST_HEAD) )
        continue;
        else if ( !IS_NPC(wch) && IS_EXTRA(wch, EXTRA_OSWITCH) ) continue;
	  else if ( ch == wch || !can_see( ch, wch ) )
               continue;

     if ( ++count == number )
        return wch;
   }

   count = 0;

   for ( wch = char_list; wch != NULL ; wch = wch->next )
   {
     if ( !IS_NPC(wch) && IS_HEAD(wch, LOST_HEAD) )
        continue;
        else if ( !IS_NPC(wch) && IS_EXTRA(wch, EXTRA_OSWITCH) ) continue;
	  else if ( !can_see( ch, wch ) )
               continue;

     if ( ++count == number )
        return wch;
   }

   return NULL;
}

CHAR_DATA *get_mob( int vnum )
{
   CHAR_DATA *mob;

   if ( vnum == 100 )
      {
         if ( auron != NULL )
            return auron;

         for ( mob = char_list; mob != NULL ; mob = mob->next )
         {
            if ( IS_NPC( mob ) && mob->pIndexData->vnum == vnum )
               break;
         }

         if ( mob == NULL )
             mob = create_mobile( get_mob_index( 100 ) );

         auron = mob;
         return auron;
      }

   if ( vnum == 101 )
      {
         if ( krizalid != NULL )
            return krizalid;

         for ( mob = char_list; mob != NULL ; mob = mob->next )
         {
            if ( IS_NPC( mob ) && mob->pIndexData->vnum == vnum )
               break;
         }

         if ( mob == NULL )
             mob = create_mobile( get_mob_index( 101 ) );

         krizalid = mob;
         return krizalid;
      }

   if ( vnum == 102 )
      {
         if ( eiphealm != NULL )
            return eiphealm;

         for ( mob = char_list; mob != NULL ; mob = mob->next )
         {
            if ( IS_NPC( mob ) && mob->pIndexData->vnum == vnum )
               break;
         }

         if ( mob == NULL )
             mob = create_mobile( get_mob_index( 102 ) );

         eiphealm = mob;
         return eiphealm;
      }

   if ( vnum == 103 )
      {
         if ( jaggarm != NULL )
            return jaggarm;

         for ( mob = char_list; mob != NULL ; mob = mob->next )
         {
            if ( IS_NPC( mob ) && mob->pIndexData->vnum == vnum )
               break;
         }

         if ( mob == NULL )
             mob = create_mobile( get_mob_index( 103 ) );

         jaggarm = mob;
         return jaggarm;
      }

   if ( vnum == 104 )
      {
         if ( altimam != NULL )
            return altimam;

         for ( mob = char_list; mob != NULL ; mob = mob->next )
         {
            if ( IS_NPC( mob ) && mob->pIndexData->vnum == vnum )
               break;
         }

         if ( mob == NULL )
             mob = create_mobile( get_mob_index( 104 ) );

         altimam = mob;
         return altimam;
      }

   if ( vnum == 105 )
      {
         if ( conanm != NULL )
            return conanm;

         for ( mob = char_list; mob != NULL ; mob = mob->next )
         {
            if ( IS_NPC( mob ) && mob->pIndexData->vnum == vnum )
               break;
         }

         if ( mob == NULL )
             mob = create_mobile( get_mob_index( 105 ) );

         conanm = mob;
         return conanm;
      }

   return NULL;
}

/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ch->seeself > 0 )
       return get_your_name( ch, argument );

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    if ( ( wch = get_char_area( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( !IS_NPC(wch) && IS_HEAD(wch, LOST_HEAD) ) continue;
	else if ( !IS_NPC(wch) && IS_EXTRA(wch, EXTRA_OSWITCH) ) continue;
      else if ( !IS_IMMORTAL( ch ) && wch->raiding > 0 ) continue;
	else if ( !can_see( ch, wch ) || ( !is_name( arg, wch->name ) &&
	 (IS_NPC(wch) || !is_name(arg, wch->pcdata->switchname)) &&

	    ( IS_NPC(wch) || !is_name( arg, wch->morph ))))
	    continue;

	if ( ++count == number )
	    return wch;
    }

    return NULL;
}



/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an object within the object you are in.
 */
OBJ_DATA *get_obj_in_obj( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj->in_obj == NULL)
	return NULL;
    obj = ch->pcdata->chobj;
    if (obj->in_obj->item_type != ITEM_CONTAINER && obj->in_obj->item_type != ITEM_CORPSE_NPC &&
    	obj->in_obj->item_type != ITEM_CORPSE_PC ) return NULL;

    for ( obj2 = obj->in_obj->contains; obj2 != NULL; obj2 = obj2->next_content )
    {
	if (obj != obj2
	&&  is_name( arg, obj2->name ) )
	{
	    if ( ++count == number )
		return obj2;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == WEAR_NONE
	&&   can_see_obj( ch, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc != WEAR_NONE
	&&   can_see_obj( ch, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    if ( ( obj = get_obj_carry( ch, argument ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_in_obj( ch, argument ) ) != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_room( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int amount )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( amount <= 0 )
    {
	bug( "Create_money: zero or negative money %d.", amount );
	amount = 1;
    }

    if ( amount == 1 )
    {
	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_ONE ), 0 );
    }
    else
    {
	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ), 0 );
	sprintf( buf, obj->short_descr, amount );
	free_string( obj->short_descr );
	obj->short_descr	= str_dup( buf );
	obj->value[0]		= amount;
    }

    return obj;
}



/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;

    weight = obj->weight;
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
	weight += get_obj_weight( obj );

    return weight;
}



/*
 * True if room is dark.
 */
bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex )
{
    if ( pRoomIndex->light > 0 )
	return FALSE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
	return FALSE;

    if ( weather_info.sunlight == SUN_SET
    ||   weather_info.sunlight == SUN_DARK )
	return TRUE;

    return FALSE;
}



/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;

    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;

    return FALSE;
}



/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if ( ch == victim )
    return TRUE;

  if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) && IS_SET( victim->arena, AFF2_TEAM1 ) )
       || ( IS_SET( ch->arena, AFF2_TEAM2 ) && IS_SET( victim->arena, AFF2_TEAM2 ) ) )
     return TRUE;

  if ( victim->in_room == NULL || victim == NULL )
     return FALSE;

  if ( IS_IMMORTAL(ch) )
     return TRUE;

  if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_INCOG) && IS_IMMORTAL(victim)
       && victim->in_room == ch->in_room )
     return TRUE;

  if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_INCOG) && IS_IMMORTAL(victim)
       && victim->in_room != ch->in_room )
      return FALSE;

  if ( !IS_NPC(victim) && IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_WIZINVIS) )
     return FALSE;

  if ( ( victim->in_room->vnum == 2 || victim->in_room->vnum == 651 ) && ch->in_room != victim->in_room )
     return FALSE;

  if ( ( victim->in_room->vnum >= 93 && victim->in_room->vnum <= 99 ) && ch->in_room != victim->in_room )
     return FALSE;

  if ( ( victim->in_room->vnum >= 33500 && victim->in_room->vnum <= 33800 ) && ch->in_room != victim->in_room )
     return FALSE;

  if ( victim->air_timer > 0 )
     return FALSE;

  if ( ( IS_CLASS( victim, CLASS_NINJA ) || IS_CLASS( victim, CLASS_DRAGOON ) || IS_CLASS( victim, CLASS_MONK ) ) && victim->ctimer[8] > 0 )
     return FALSE;

  if ( IS_ITEMAFF(ch, ITEMA_EYES) )
     return TRUE;

  if ( IS_SET(victim->more2, MORE2_DIG) )
     return FALSE;

  if ( armageddon == TRUE )
     return TRUE;

  if ( IS_SET( victim->in_room->area->bits, ROOM_ANTI_LIGHT ) && !can_do_skill( ch, 44 ) ) 
     return FALSE;

  if ( IS_SET(ch->more, MORE_BLIND) && !IS_SET( ch->cmbt[5], SKL2_SSIGHT ) )
     return FALSE;

  if ( !IS_NPC(victim) && victim->in_room->vnum >= 550 && victim->in_room->vnum < 630 )
     return TRUE;

  if ( IS_EXTRA(ch, BLINDFOLDED) )
     return FALSE;

  if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
     return FALSE;

  if ( !IS_NPC(victim) && victim->pcdata->team > 0 )
     return TRUE;

  if ( weather_info.sky == SKY_ECLIPSE && !IS_CLASS( ch, CLASS_DROW ) && number_percent() > 60 )
     return FALSE;

  if ( IS_SET( victim->more3, MORE3_FALCON ) && number_percent() > 74 && victim->fight_timer > 0 )
     return;

  if ( IS_SET( ch->cmbt[7], SKL_EYES ) )
     return TRUE;

  if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && IS_CLASS( victim, CLASS_NINJA )
       && !IS_CLASS( ch, CLASS_NINJA ) )
     return FALSE;

  if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && IS_CLASS( victim, CLASS_MONK )
       && !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) )
     return FALSE;

  if ( ch->level == 999 || IS_CLASS( ch, CLASS_DUERGAR ) )
     return TRUE;

  if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && ( IS_SET( victim->cmbt[7], SKL_KAKUSU )
       || IS_CLASS( victim, CLASS_NIGHT_BLADE ) ) && !IS_CLASS( ch, CLASS_NINJA ) )
     return FALSE;

  if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && IS_CLASS( victim, CLASS_DROW ) 
       && victim->pcdata->powers[DROW_CLASS] == DROW_MILITIA )
     return FALSE;

  if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && IS_CLASS( victim, CLASS_SHADOW )
       && !IS_CLASS( ch, CLASS_SHADOW ) && !IS_CLASS( ch, CLASS_NINJA ) )
     return FALSE;

  if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && IS_CLASS( victim, CLASS_HOBBIT )
       && !IS_CLASS( ch, CLASS_HOBBIT ) && !IS_CLASS( ch, CLASS_NINJA ) )
     return FALSE;

  if ( IS_ITEMAFF( ch, ITEMA_VISION ) )
     return TRUE;

  if ( IS_SET( victim->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
     {
        if ( !IS_IMMORTAL( ch ) && !IS_CLASS( ch, CLASS_DROW ) && !IS_ITEMAFF( ch, ITEMA_DARKNESS )
             && !IS_SET( ch->more2, MORE2_LEOPARD ) )
           return FALSE;
     }

  if ( IS_SET( victim->in_room->area->bits, ROOM_TOTAL_DARKNESS ) && !IS_CLASS( ch, CLASS_VAMPIRE )
       && !IS_CLASS( ch, CLASS_DROW ) )
     return FALSE;

  if ( IS_SET( ch->extra, EXTRA_ZOMBIE ) )
     return TRUE;

  if ( IS_NPC(ch) && get_kingdom_num(ch) != -1 )
     return TRUE;

  if ( !IS_NPC(victim) && IS_NPC(ch) && ( IS_SET(victim->act, PLR_WIZINVIS) || IS_SET(victim->itemaffect, ITEMA_VANISH) ) )
     return FALSE;

  if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
     return TRUE;

  if ( IS_AFFECTED(victim, AFF_INVISIBLE) && !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
     return FALSE;

  if ( IS_AFFECTED(victim, AFF_HIDE) && !IS_AFFECTED(ch, AFF_DETECT_HIDDEN) )
     return FALSE;

  return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
  CHAR_DATA *gch;

    if ((gch = obj->carried_by) != NULL)
    {
      if (gch->desc != NULL)
      {
        if (gch->desc->connected != CON_PLAYING) return FALSE;
      }
    }

    if ( IS_ITEMAFF(ch, ITEMA_EYES) )
       return TRUE;

    if ( IS_SET(ch->more, MORE_BLIND) && !IS_SET( ch->cmbt[5], SKL2_SSIGHT ) )
       return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

    if (( IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)
    &&   obj->carried_by == NULL)
    &&   !IS_AFFECTED(ch, AFF_SHADOWSIGHT)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE))
	return FALSE;

    if (( !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)
    &&   obj->carried_by == NULL)
    &&   !IS_AFFECTED(ch, AFF_SHADOWSIGHT)
    &&   IS_AFFECTED(ch, AFF_SHADOWPLANE))
	return FALSE;

    if ( !IS_NPC(ch) && IS_VAMPAFF(ch, VAM_SONIC) )
	return TRUE;

    if ( obj->item_type == ITEM_POTION )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
	return FALSE;

    if ( IS_EXTRA(ch, BLINDFOLDED) )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_BLIND ) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT) )
	return FALSE;

    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED) 
	&& (!IS_NPC( ch ) && !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)))
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	return FALSE;

    if ( !IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA *obj )
{
    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return "light";
    case ITEM_SCROLL:		return "scroll";
    case ITEM_WAND:		return "wand";
    case ITEM_STAFF:		return "staff";
    case ITEM_WEAPON:		return "weapon";
    case ITEM_TREASURE:		return "treasure";
    case ITEM_INSTRUMENT:	return "instrument";
    case ITEM_ARMOR:		return "armor";
    case ITEM_POTION:		return "potion";
    case ITEM_FURNITURE:	return "furniture";
    case ITEM_TRASH:		return "trash";
    case ITEM_CONTAINER:	return "container";
    case ITEM_DRINK_CON:	return "drink container";
    case ITEM_KEY:		return "key";
    case ITEM_FOOD:		return "food";
    case ITEM_MONEY:		return "money";
    case ITEM_BOAT:		return "boat";
    case ITEM_CORPSE_NPC:	return "npc corpse";
    case ITEM_CORPSE_PC:	return "pc corpse";
    case ITEM_FOUNTAIN:		return "fountain";
    case ITEM_PILL:		return "pill";
    case ITEM_PORTAL:		return "portal";
    case ITEM_EGG:		return "egg";
    case ITEM_VOODOO:		return "voodoo doll";
    case ITEM_STAKE:		return "stake";
    case ITEM_MISSILE:		return "missile";
    case ITEM_AMMO:		return "ammo";
    case ITEM_QUESTCARD:	return "quest card";
    case ITEM_QUESTMACHINE:	return "quest generator";
    case ITEM_SYMBOL:		return "magical symbol";
    case ITEM_BOOK:		return "book";
    case ITEM_PAGE:		return "page";
    case ITEM_TOOL:		return "tool";
    case ITEM_COPPER:         return "copper";    
    case ITEM_IRON:           return "iron";
    case ITEM_STEEL:          return "steel";
    case ITEM_ADAMANTITE:     return "adamantite";
    case ITEM_GEMSTONE:       return "gemstone";
    case ITEM_HILT:           return "hilt";
    case ITEM_DTOKEN:         return "TP token";
    case ITEM_PKTOKEN:        return "PK token";
    case ITEM_QUEST:		return "coin token";
}

    return "(unknown)";
}



/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:		return "none";
    case APPLY_STR:		return "#Rstrength";
    case APPLY_DEX:		return "#Cdexterity";
    case APPLY_INT:		return "#Lmind";
    case APPLY_WIS:		return "#Pspirit";
    case APPLY_CON:		return "#yvitality";
    case APPLY_EVA:		return "#cevasion";
    case APPLY_AGI:		return "#wagility";
    case APPLY_LUCK:		return "#gluck";
    case APPLY_TENSION:		return "#Rtension";
    case APPLY_SEX:		return "sex";
    case APPLY_CLASS:		return "class";
    case APPLY_LEVEL:		return "level";
    case APPLY_AGE:		return "age";
    case APPLY_MANA:		return "#ymana";
    case APPLY_HIT:		return "#Chp";
    case APPLY_MOVE:		return "#Pmoves";
    case APPLY_GOLD:		return "gold";
    case APPLY_EXP:		return "experience";
    case APPLY_AC:		return "armor class";
    case APPLY_HITROLL:		return "hit roll";
    case APPLY_DAMROLL:		return "damage roll";
    case APPLY_SAVING_PARA:	return "save vs paralysis";
    case APPLY_SAVING_ROD:	return "save vs rod";
    case APPLY_SAVING_PETRI:	return "save vs petrification";
    case APPLY_SAVING_BREATH:	return "save vs breath";
    case APPLY_SAVING_SPELL:	return "save vs spell";
    case APPLY_POLY:		return "polymorph form";
    case APPLY_SAVING_LUNAR:  return "#olunar resilience";
    case APPLY_SAVING_FIRE:   return "#Rfire resilience";
    case APPLY_SAVING_ICE:    return "#Cice resilience";
    case APPLY_SAVING_THUNDER: return "#ythunder resilience";
    case APPLY_SAVING_EARTH: return "#gearth resilience";
    case APPLY_SAVING_HOLY: return "#wholy resilience";
    case APPLY_SAVING_DARK: return "#edark resilience";
    case APPLY_SAVING_SLASH: return "#cslash resilience";
    case APPLY_SAVING_PIERCE: return "#cpierce resilience";
    case APPLY_SAVING_BLAST: return "#cblast resilience";
    case APPLY_SAVING_BEAM: return "#cbeam resilience";
    case APPLY_SAVING_BLUNT: return "#cblunt resilience";
    case APPLY_SAVING_SPIRIT: return "#Pspirit resilience";
    case APPLY_SAVING_NORMAL: return "#cnormal resilience";
    case APPLY_SAVING_NATURAL: return "#Cnatural resilience";
    case APPLY_SAVING_PHYSICAL: return "#wphysical resilience";
    case APPLY_SAVING_MAGICAL: return "#Pmagical resilience";
    case APPLY_LUNARS: return "#olunar attack bonus";
    case APPLY_DARKS: return "#edark attack bonus";
    case APPLY_HOLYS: return "#wholy attack bonus";
    case APPLY_FIRES: return "#Rfire attack bonus";
    case APPLY_ICES: return "#Cice attack bonus";
    case APPLY_BOLTS: return "#ythunder attack bonus";
    case APPLY_EARTHS: return "#gearth attack bonus";
    case APPLY_NATURALS: return "#Cnatural attack bonus";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) strcat( buf, " invisible"     );
    if ( vector & AFF_DETECT_EVIL   ) strcat( buf, " detect_evil"   );
    if ( vector & AFF_DETECT_INVIS  ) strcat( buf, " detect_invis"  );
    if ( vector & AFF_DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & AFF_SHADOWPLANE   ) strcat( buf, " shadowplane"   );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_FLAMING       ) strcat( buf, " flaming"       );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_PROTECT       ) strcat( buf, " protect"       );
    if ( vector & AFF_ETHEREAL      ) strcat( buf, " ethereal"      );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CHARM         ) strcat( buf, " charm"         );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) strcat( buf, " pass_door"     );
    if ( vector & AFF_POLYMORPH     ) strcat( buf, " polymorph"     );
    if ( vector & AFF_SHADOWSIGHT   ) strcat( buf, " shadowsight"   );
    if ( vector & AFF_WEBBED        ) strcat( buf, " web"           );
    if ( vector & AFF_INFIRMITY     ) strcat( buf, " infirmity" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}



/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );
    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );
    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );
    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );
    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );
    if ( extra_flags & ITEM_LOYAL        ) strcat( buf, " loyal"        );
    if ( extra_flags & ITEM_SHADOWPLANE  ) strcat( buf, " shadowplane"  );
    if ( extra_flags & ITEM_THROWN       ) strcat( buf, " thrown"       );
    if ( extra_flags & ITEM_KEEP         ) strcat( buf, " keep"         );
    if ( extra_flags & ITEM_VANISH       ) strcat( buf, " vanish"       );
    if ( extra_flags & ITEM_TWOHANDS     ) strcat( buf, " twohanded"    );
    if ( extra_flags & ITEM_ROBE         ) strcat( buf, " light armor"  );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

CHAR_DATA *get_char_world2( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;

    if (argument[0] == '\0') return NULL;

    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( IS_NPC(wch) && !str_cmp( argument, wch->short_descr ) ) return wch;
    }

    return NULL;
}

OBJ_DATA *get_obj_world2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int vnum = 0;

    if (argument[0] == '\0') return NULL;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !str_cmp( argument, obj->short_descr ) )
	{
	    if ((vnum = obj->pIndexData->vnum) == 30037 || vnum == 30041)
		continue;
	    return obj;
	}
    }

    return NULL;
}

