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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define MAX_SLAY_TYPES 3

/*
 * Local functions.
 */
void  dropinvis       args((CHAR_DATA *ch));
bool  check_dodge     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void  check_killer    args((CHAR_DATA *ch, CHAR_DATA *victim));
bool  check_parry     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void  death_cry       args((CHAR_DATA *ch));
void  group_gain      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   xp_compute      args((CHAR_DATA *gch, CHAR_DATA *victim));
void  set_fighting    args((CHAR_DATA *ch, CHAR_DATA *victim));
int   number_attacks  args((CHAR_DATA *ch, CHAR_DATA *victim));
void dragoon_trip args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_illusionblur args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool  check_weapon_guard args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool check_dodge2 args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool check_shield_block args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void do_hell_hound args( ( CHAR_DATA *ch, char *argument ) );
void do_spider args( ( CHAR_DATA *ch, char *argument ) );
bool show_shaolin_ipose args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

int is_attack_type( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
   CHAR_DATA *protector = victim->protecting;
   float adder = 1.0;
   int str = get_curr_str( ch ), con = get_curr_con( victim ) + 1;
   int min = get_curr_int( ch ), spi = get_curr_wis( victim ) + 1;

   if ( ch->anger_level > 300 )
      dam *= 1.5;

   if ( IS_SET( victim->extra, EXTRA_LOWDEF ) )
      dam *= 1.15;

   if ( IS_SET( ch->extra, EXTRA_LOWOFF ) )
      dam *= 0.85;

   dam *= ch->dam_boost;

   if ( victim->dam_boost > 1.0 )
      {
         float reduc = victim->dam_boost - 1.0;

         dam *= ( 1.0 - ( reduc * 0.5 ) );
      }

   if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
      dam /= 5;

   if ( IS_CLASS( victim, CLASS_SAIYAN ) && victim->saiyanstuff == 1 && victim->nemesis > 0 )
      dam /= 3;

   if ( ch->attack_type == ATTACK_MAGICAL && conan == 7 )
      dam *= 1.25;
      else if ( ch->attack_type == ATTACK_PHYSICAL && conan == 6 )
              dam *= 1.25;

   if ( dam < 150000 )
      return dam;

   if ( ch->ctimer[18] > 0 )
      {
        if ( !str_cmp( "K", ch->groove ) )
           dam *= 1.5;
           else if ( !str_cmp( "M", ch->groove ) )
                   dam *= 1.25;
           else if ( !str_cmp( "S", ch->groove ) )
                   dam *= 1.2;
           else if ( !str_cmp( "N", ch->groove ) || !str_cmp( "P", ch->groove ) )
                   dam *= 1.05;
      }

   if ( !IS_NPC( ch ) && ch->in_room->area->land == ch->pcdata->deity && ch->pcdata->deity > 0 )
      dam *= 1.1;

   if ( !IS_NPC( victim ) && victim->in_room->area->land == victim->pcdata->deity && victim->pcdata->deity > 0 )
      dam *= 0.9;

   if ( ch->attack_type == ATTACK_MAGICAL )
      {
        if ( IS_SET( victim->in_room->room_flags, ROOM_WHIRLWIND ) && dam > 0
             && !( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_DRAGON_KNIGHT ) 
             && IS_SET( victim->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) ) )
           {
              if ( number_percent() > 74 )
                 {
                    act( "#wThe #Cattack #ysparks #gand fizzes in the whirlpool as its powers gets sapped...#n", victim, NULL, NULL, TO_CHAR );
                    act( "#wThe #Cattack #ysparks #gand fizzes in the whirlpool as its powers gets sapped...#n", victim, NULL, NULL, TO_ROOM );
                 }

              return 0;
           }

        if ( number_percent() > 74 && is_affected( victim, skill_lookup("invunerability") ) )
           {
              act( "#PYou #ecackle with #Lglee #eas the #Pmagical #eattack bounces off you.#n", victim, NULL, NULL, TO_CHAR );
              act( "#P$n #ecackles with #Lglee #eas the #Pmagical #eattack bounces off $m.#n", victim, NULL, NULL, TO_ROOM );
              return 0;
           }

        adder -= ( 0.01 * URANGE( -25, get_magical_prof( victim ) / 4, 25 ) );

        if ( is_affected( victim, skill_lookup("fear") ) )
           adder += 0.15;
           else if ( is_affected( victim, skill_lookup("magiwall") ) )
                   adder -= 0.25;
           else if ( is_affected( victim, skill_lookup("sun aura") ) )
                   adder -= 0.1;

        adder += ( min * 0.002 );
      }

   if ( ch->attack_type == ATTACK_PHYSICAL )
      {
        adder += ( str * 0.002 );
        adder -= ( 0.01 * URANGE( -25, get_physical_prof( victim ) / 4, 25 ) );

        if ( IS_ITEMAFF( ch, ITEMA_MONKCHI ) && IS_SET( ch->itemaffect, ITEMA_ARTIFACT ) )
           adder += 0.15;

        if ( is_affected( ch, skill_lookup("rose storm") ) )
           adder -= 0.1;
      }

   if ( victim->position == POS_FIGHTING && victim->binded != NULL && is_same_plane( victim, victim->binded ) )
      dam /= 2;

   if ( ch->protecting != NULL && ch->protecting->in_room == ch->in_room && is_same_plane( ch, ch->protecting ) )
      dam /= 2;
      else if ( protector != NULL && protector->in_room == victim->in_room && victim->attacker == NULL
                && is_same_plane( victim, protector ) )
              dam /= 2;

   if ( !IS_NPC( ch ) && !IS_NPC( victim ) && victim != ch && ch->strike_timer == 0 )
      {
        if ( victim->fighting != ch && victim->fighting != NULL )
           {
              int divider = 1;
              CHAR_DATA *vch;

              for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
              {
                 if ( vch->fighting != victim || vch == victim || vch == ch || IS_NPC( vch ) )
                    continue;

                 divider++;
              }

              dam = dam / divider;
           }
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
            case 1: dam *= 1.5;
                    break;

            case 2: dam *= 1.1;
                    break;
         }
      }

   if ( ( IS_SET( victim->arena, AFF2_TEAM1 ) || IS_SET( victim->arena, AFF2_TEAM2 ) ) 
        && IS_SET( victim->arena, AFF2_INARENA ) )
      {
         int team;

         if ( IS_SET( victim->arena, AFF2_TEAM1 ) )
            team = 0;
            else team = 1;

         switch( team_list->power_mode[team] )
         {
            case 2: dam *= 0.5;
                    break;
         }
      }

   if ( IS_SET( ch->arena, AFF2_CHAIN ) )
      dam *= 0.75;

   if ( ch->topmode == 66 && ch->hit > ch->max_hit * 0.33
        && ch->hit < ch->max_hit * 0.67 && number_percent() > 94 )
      adder += 0.25;
      else if ( ch->topmode == 33 && ch->hit < ch->max_hit * 0.34 )
              adder += 0.1;

   if ( ch->cmbt[2] > 0 )
      adder += 0.1;

   if ( !IS_NPC( ch ) && ch->pcdata->deity == DEI_ODIN )
      adder += 0.05;

   if ( !IS_NPC( victim ) && victim->pcdata->deity == DEI_CARB )
      adder -= 0.05;

   if ( IS_ITEMAFF( victim, ITEMA_PROTECTION ) )
      adder -= 0.15;

   if ( ch->blocking == -10 )
      adder += 0.5;

   if ( ch->attack_var == SLASH )
      {
         if ( conan == 1 )
            dam *= 1.5;

         adder -= ( 0.01 * URANGE( -50, get_slash_prof( victim ) / 2, 50 ) );
      }
      else if ( ch->attack_var == PIERCE )
              {
                 if ( conan == 2 )
                    dam *= 1.5;

                 adder -= ( 0.01 * URANGE( -50, get_pierce_prof( victim ) / 2, 50 ) );
              }
      else if ( ch->attack_var == BEAM )
              {
                 if ( conan == 5 )
                    dam *= 1.5;

                 adder -= ( 0.01 * URANGE( -50, get_beam_prof( victim ) / 2, 50 ) );
              }
      else if ( ch->attack_var == BLUNT )
              {
                 if ( conan == 4 )
                    dam *= 1.5;

                 adder -= ( 0.01 * URANGE( -50, get_blunt_prof( victim ) / 2, 50 ) );
              }
      else if ( ch->attack_var == SOUL )
              adder -= ( 0.01 * URANGE( -50, get_spirit_prof( victim ) / 2, 50 ) );
      else if ( ch->attack_var == BLAST )
              {
                 if ( conan == 3 )
                    dam *= 1.5;

                 adder -= ( 0.01 * URANGE( -50, get_blast_prof( victim ) / 2, 50 ) );
              }

   if ( !IS_NPC( ch ) && !IS_NPC( victim ) )
      {
        if ( IS_SET( ch->in_room->room_flags, ROOM_SHIFT ) )
           {
              if ( IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
                 adder += 0.15;

              if ( IS_CLASS( victim, CLASS_DRAGON_KNIGHT ) && IS_SET( victim->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
                 adder -= 0.15;
           }

        if ( IS_CLASS( ch, CLASS_DROW ) )
           {
              if ( time_info.hour > 6 && time_info.hour < 19 )
                 adder -= 0.05;
                 else adder += 0.05;

              if ( !IS_SET( ch->newbits, NEW_DARKNESS ) )
                 adder -= 0.1;
           }
           else if ( IS_CLASS( ch, CLASS_DUERGAR ) && IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
                   adder += 0.05;

        if ( IS_SET( ch->cmbt[7], SKL_PRACTICE ) )
           adder += ( number_range( 1, 10 ) * 0.01 );

        if ( IS_SET( victim->cmbt[7], SKL_KNOWLEDGE ) )
           adder -= ( number_range( 1, 10 ) * 0.01 );

        if ( ch->outer_aura == A_ADAMAGE || ch->inner_aura == A_ADAMAGE || IS_SET(ch->group_aura, G_ADAMAGE) )
           adder += 0.1;
           else if ( ch->outer_aura == A_STRENGTH || ch->inner_aura == A_STRENGTH || IS_SET(ch->group_aura, G_STRENGTH) )
                   adder += 0.05;

        if ( victim->outer_aura == A_FAITH || victim->inner_aura == A_FAITH || IS_SET(victim->group_aura, G_FAITH) )
           {
              if ( victim->hit < victim->max_hit * 0.25 )
                 adder -= 0.1;
                 else if ( victim->hit < victim->max_hit * 0.5 )
                         adder -= 0.08;
                 else if ( victim->hit < victim->max_hit * 0.75 )
                         adder -= 0.06;
                 else if ( victim->hit < victim->max_hit )
                         adder -= 0.02;
           }

        if ( ch->outer_aura == A_FAITH || ch->inner_aura == A_FAITH || IS_SET(ch->group_aura, G_FAITH) )
           {
              if ( ch->hit < ch->max_hit * 0.25 )
                 adder += 0.1;
                 else if ( ch->hit < ch->max_hit * 0.5 )
                         adder += 0.08;
                 else if ( ch->hit < ch->max_hit * 0.75 )
                         adder += 0.06;
                 else if ( ch->hit < ch->max_hit )
                         adder += 0.02;
           }

        if ( IS_CLASS( ch, CLASS_VAMPIRE ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_DARK )
           adder += 0.05;
           else if ( IS_CLASS( ch, CLASS_VAMPIRE ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_FAITH )
           {
              if ( ch->hit < ch->max_hit * 0.25 )
                 adder += 0.1;
                 else adder -= 0.05;
           }

        if ( IS_CLASS( victim, CLASS_VAMPIRE ) && victim->pcdata->powers[VAMPIRE_REAVER] == REAVER_STONE )
           adder -= 0.1;
      }

   if ( ch->attack_type == ATTACK_MAGICAL )
      adder = UMAX( 1.0, adder - ( spi * 0.005 ) );
      else if ( ch->attack_type == ATTACK_PHYSICAL )
              adder = UMAX( 1.0, adder - ( con * 0.005 ) );

   return dam * UMIN( 1.5, adder );
}

int is_elemental( int ele_attack, CHAR_DATA *victim, int dam )
{
   float adder = 1.0;

   if ( victim->fighting != NULL )
      {
         if ( IS_SET( victim->fighting->elemental, ELE_LUNAR ) )
            ele_attack = ATTACK_LUNAR;
            else if ( IS_SET( victim->fighting->elemental, ELE_NONE ) )
                    return dam;
      }

   if ( ( eipheal == 4 && ele_attack == ATTACK_HOLY )
        || ( eipheal == 5 && ele_attack == ATTACK_FIRE )
        || ( eipheal == 8 && ele_attack == ATTACK_EARTH )
        || ( eipheal == 7 && ele_attack == ATTACK_THUNDER )
        || ( eipheal == 6 && ele_attack == ATTACK_ICE ) )
      return 0;
      else if ( eipheal == 9 && ele_attack == ATTACK_LUNAR )
              dam *= 2;
      else if ( conan == 11 && ele_attack == 0 )
              dam *= 2;

   if ( ( time_info.day == 0 && ele_attack == ATTACK_HOLY )
        || ( time_info.day == 1 && ele_attack == ATTACK_FIRE )
        || ( time_info.day == 2 && ele_attack == ATTACK_EARTH )
        || ( time_info.day == 3 && ele_attack == ATTACK_LUNAR )
        || ( time_info.day == 4 && ele_attack == ATTACK_THUNDER )
        || ( time_info.day == 5 && ele_attack == ATTACK_ICE )
        || ( time_info.day == 6 && ele_attack == ATTACK_DARK ) )
      dam *= 1.25;

   if ( ( ele_attack == ATTACK_HOLY && is_affected( victim, skill_lookup( "holy seal" ) ) )
        || ( ele_attack == ATTACK_DARK && is_affected( victim, skill_lookup( "demonic seal" ) ) )
        || ( ele_attack == ATTACK_FIRE && is_affected( victim, skill_lookup( "fiery seal" ) ) )
        || ( ele_attack == ATTACK_ICE && is_affected( victim, skill_lookup( "icy seal" ) ) )
        || ( ele_attack == ATTACK_THUNDER && is_affected( victim, skill_lookup( "thunder seal" ) ) )
        || ( ele_attack == ATTACK_EARTH && is_affected( victim, skill_lookup( "earth seal" ) ) )
        || ( ele_attack == ATTACK_LUNAR && is_affected( victim, skill_lookup( "lunar seal" ) ) )
        || ( ele_attack == 0 && is_affected( victim, skill_lookup( "natural seal" ) ) ) )
      return -dam;

   if ( IS_CLASS( victim, CLASS_DEMON ) && !IS_CLASS( victim, CLASS_ANGEL ) )
      {
         if ( ele_attack == ATTACK_HOLY && !is_affected( victim, skill_lookup( "demi protection" ) ) )
            return dam * 2;
            else if ( ele_attack == ATTACK_DARK && endozone == 0 )
                    {
                       if ( IS_CLASS( victim, CLASS_FIEND ) && IS_CLASS( victim, CLASS_SHADOW_WAR ) )
                          return -dam;
                          else return 0;
                    }
      }

   if ( endozone == 0 && victim->class_2 == 0 )
      {
         if ( IS_CLASS( victim, CLASS_ANGEL ) )
            {
               if ( ele_attack == ATTACK_HOLY )
                  return 0;
                  else if ( ele_attack == ATTACK_DARK && !is_affected( victim, skill_lookup( "protection from evil" ) ) )
                          return dam * 2;
            }

         if ( ele_attack == ATTACK_FIRE && IS_CLASS( victim, CLASS_PHOENIX ) && dam > 0 )
            return -dam * UMAX( 1, victim->pkill / 10 );

         if ( ele_attack == ATTACK_ICE && IS_CLASS( victim, CLASS_SNOW_ELF ) && dam > 0 )
            return -dam * UMAX( 1, victim->pkill / 10 );

         if ( ele_attack == ATTACK_THUNDER && IS_CLASS( victim, CLASS_WISP ) && dam > 0 )
            return -dam;

         if ( ele_attack == ATTACK_EARTH && IS_CLASS( victim, CLASS_DUERGAR ) && dam > 0 )
            return -dam * UMAX( 1, victim->pkill / 10 );
      }

   if ( ele_attack == ATTACK_FIRE )
      adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->fire_bonus ) + UMIN( 100, king_table[DEI_IFRIT].members * 4 ) ) / 4 ) - get_fire_prof( victim ) ) / 2, 50 ) );
      else if ( ele_attack == ATTACK_ICE )
              adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->ice_bonus ) + UMIN( 100, king_table[DEI_SHIVA].members * 4 ) ) / 4 ) - get_ice_prof( victim ) ) / 2, 50 ) );
      else if ( ele_attack == ATTACK_THUNDER )
              adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->bolt_bonus ) + UMIN( 100, king_table[DEI_BIRD].members * 4 ) ) / 4 ) - get_thunder_prof( victim ) ) / 2, 50 ) );
      else if ( ele_attack == ATTACK_EARTH )
              adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->earth_bonus ) + UMIN( 100, king_table[DEI_TITAN].members * 4 ) ) / 4 ) - get_earth_prof( victim ) ) / 2, 50 ) );
      else if ( ele_attack == ATTACK_HOLY )
              adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->holy_bonus ) + UMIN( 100, king_table[DEI_SIREN].members * 4 ) ) / 4 ) - get_holy_prof( victim ) ) / 2, 50 ) );
      else if ( ele_attack == ATTACK_DARK )
              adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->dark_bonus ) + UMIN( 100, king_table[DEI_DIABLOS].members * 4 ) ) / 4 ) - get_dark_prof( victim ) ) / 2, 50 ) );
      else if ( ele_attack == ATTACK_LUNAR )
              adder += ( 0.01 * URANGE( -50, ( ( victim->fighting == NULL ? 0 : ( ( IS_SET( victim->fighting->elemental, ELE_LUNARP ) ? 100 : 0 ) + victim->fighting->lunar_bonus + UMIN( 100, king_table[DEI_LUNA].members * 4 ) ) / 4 ) - get_lunar_prof( victim ) ) / 2, 50 ) );
              else adder += ( 0.01 * URANGE( -50, ( ( ( ( victim->fighting == NULL ? 0 : victim->fighting->natural_bonus ) + UMIN( 100, king_table[DEI_GAIA].members * 4 ) ) / 4 ) - get_natural_prof( victim ) ) / 2, 50 ) );

   if ( eipheal == 10 && adder > 1.0 )
      adder = 1.0 + ( ( adder - 0.9 ) * 3 );

   return dam * adder;
}

bool is_same_plane( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( victim == NULL || ch == NULL )
       return FALSE;

    if ( mysafe != 0 || armageddon == TRUE )
       return TRUE;

    if ( victim->in_room != NULL && victim->in_room->vnum >= 550 && victim->in_room->vnum < 630 )
       return TRUE;

    if ( victim->in_room != NULL && victim->in_room->vnum >= 33500 && victim->in_room->vnum < 33800 )
       return TRUE;

    if ( IS_AFFECTED(ch, AFF_ETHEREAL) && !IS_AFFECTED(victim, AFF_ETHEREAL) )
       return FALSE;

    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE) )
       return FALSE;

    if ( !IS_AFFECTED(ch, AFF_ETHEREAL) && IS_AFFECTED(victim, AFF_ETHEREAL) )
       return FALSE;

    if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE) )
       return FALSE;

    return TRUE;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch, *fch_next;

    one_argument( argument, arg );

    if ( !IS_NPC( ch ) && nogang == 10 )
    {
       stc("You can't rescue other people unless ganging is allowed.\n\r",ch);
       return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Rescue whom?\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && (get_age(ch)-17) < 2 )
    {
       stc("But you are a newbie.\n\r",ch);
       return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "What about fleeing instead?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && (get_age(victim)-17) < 2 )
    {
       stc("But they are a newbie.\n\r",ch);
       return;
    }

    if ( ch->fighting == victim )
    {
	send_to_char( "Too late.\n\r", ch );
	return;
    }

    if ( !armageddon )
       {
          if ( fch->attacker != NULL )
             {
                stc("You can't rescue other people unless ganging is allowed.\n\r",ch);
                return;
             }

          if ( victim->attacker != NULL )
             {
                stc("You can't rescue other people unless ganging is allowed.\n\r",ch);
                return;
             }
       }

    if ( ch->safe_timer > 0 )
    {
	send_to_char( "Not with a safe timer!\n\r", ch );
	return;
    }

    if ( victim->safe_timer > 0 )
    {
	send_to_char( "They have a safe timer!\n\r", ch );
	return;
    }

    if ( !is_same_plane(ch,victim) )
       return;

    if ( ch->gshield > 0 )
       ch->gshield = 0;

    for ( fch = ch->in_room->people; fch != NULL; fch = fch_next )
    {
       fch_next = fch->next_in_room;

       if ( fch->fighting != victim )
          continue;

       if ( !can_see( ch, fch ) )
          continue;

       act( "#RYou #wrescue #R$N!",  ch, NULL, victim, TO_CHAR );
       act( "#R$n #wrescues #Ryou!", ch, NULL, victim, TO_VICT );
       act( "#R$n #wrescues #R$N!",  ch, NULL, victim, TO_NOTVICT );

       stop_fighting( ch, FALSE );
       set_fighting( ch, fch );
       fch->fighting = ch;
       return;
    }

    stc( "#RRescue #wthem from #ywhom#w?#n\n\r", ch );
}

void do_redemption_aura( CHAR_DATA *ch )
{
   int people = 0;
   CHAR_DATA *vch;

   if ( ch->outer_aura == A_REDEMPTION || ch->inner_aura == A_REDEMPTION )
      {
         int level = 0;

         if ( ch->inner_aura == A_REDEMPTION )
            level += 1;

         level += ch->aura[A_REDEMPTION];
 
         if ( level > 10 )
            level = 10;

         if ( number_percent() > level * 10 )
            return;
      }
      else return;

   for( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
   {
      if ( vch == ch || vch->fighting != ch )
         continue;

      if ( ++people == 10 )
         break;    
   }

   if ( people < 1 )
      return;

   act("You mutter prayers for the souls of your aggressors.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters prayers for the souls of $s aggressors.",ch,NULL,NULL,TO_ROOM);
   act("You are blessed from the divine one as these times of turmoil.",ch,NULL,NULL,TO_CHAR);
   act("$n is blessed from the divine one in these times of turmoil.",ch,NULL,NULL,TO_ROOM);

   hit_gain( ch, ( 50000 * people ) );
   mana_gain( ch, ( 25000 * people ) );
   move_gain( ch, ( 25000 * people ) );
}

void do_cleansing( CHAR_DATA *ch )
{
   if ( number_percent() < 65 )
      return;

   if ( ch->outer_aura == A_CLEANSING || ch->inner_aura == A_CLEANSING || IS_SET(ch->group_aura, G_CLEANSING) )
      {
         int level = 0;

         if ( !IS_SET(ch->group_aura, G_CLEANSING) )
            {
               if ( ch->inner_aura == A_CLEANSING )
                  level += level + 1;

               level += ch->aura[A_CLEANSING];
            }
            else level = ch->special_aura[A_CLEANSING];
 
         if ( level > 10 )
            level = 10;

         if ( number_percent() > level * 10 )
            return;

         act("You meditate softly cleansing all impurities from your body.",ch,NULL,NULL,TO_CHAR);
         act("$n meditates softly cleansing all impurities from $s body.",ch,NULL,NULL,TO_ROOM);
      }
      else return;

    clear_affects( ch );
}

void scorch_equip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj, *obj_next;

    act("#RYou #ybreathe #Ra barrage of #eDeVaSTaTiNg #yflames #Rover $N#R's #yequipment!#n",ch,NULL,victim,TO_CHAR);
    act("#R$n #ybreathes #Ra barrage of #eDeVaSTaTiNg #yflames #Rover your #yequipment!#n",ch,NULL,victim,TO_VICT);
    act("#R$n #ybreathes #Ra barrage of #eDeVaSTaTiNg #yflames #Rover $N#R's #yequipment!#n",ch,NULL,victim,TO_NOTVICT);

    if ( ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !( !IS_NPC( victim ) 
         && victim->pcdata->capture_timer > 0 ) && victim->position == POS_FIGHTING )
       {
          for ( obj = victim->carrying; obj != NULL; obj = obj_next )
          {
             obj_next = obj->next_content;

             if ( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
                  || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91
                  || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
                  || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
                  || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
                  || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
                  || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
                  || obj->item_type == ITEM_PKTOKEN )
                continue;

             obj->condition -= 10;

             if ( obj->condition < 1 )
                {
                   if ( obj->points > 0 )
                      ch->bank = UMIN( 500000000, ch->bank + obj->points );

                   act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                   act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                   obj_from_char( obj );
                   extract_obj( obj );
               }
          }
       }
}

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *emb;
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    CHAR_DATA *rch_next;
    CHAR_DATA *mount;
    int ilovenima = 0;

    /* check lightnings throughout the world */
    beware_lightning();

    /* check icestorms throughout the world */
    beware_icestorm();

    if ( betting_time > 0 )
       {
          betting_time--;

          if ( betting_time == 0 )
             {
                char      buf [MAX_INPUT_LENGTH];
                DESCRIPTOR_DATA *d;

                strcpy(buf,"#eNo more #wbetting #ecan #ytake #bplace#e for the current #pmatch#e.#n");

                for ( d = descriptor_list; d != NULL; d = d->next )
                {
                   if ( d->connected == CON_PLAYING && !IS_SET(d->character->deaf, CHANNEL_INFO) )
                      {
                          send_to_char( "#g[#y<#wArena#y>#g]#p ",d->character );
                          send_to_char( buf, d->character );
                          send_to_char( " #g[#y<#wArena#y>#g]#n\n\r",   d->character );
                      }
                }
             }
         }
  
    melee = TRUE;

    if ( nofighting > 0 )
       {
          if ( --nofighting == 0 )
             do_info( NULL, "#yThe #wLight #yhas removed its #Pprotection #yfrom the world... #RCombat #yis allowed again.#n" );
       }

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next	= ch->next;

      ilovenima++;

      if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) && arena_wait == 0
           && IS_SET( ch->arena, AFF2_INARENA ) && ( arena == FIGHT_BATTLEOVER || arena == FIGHT_SPECIAL_KO ) )
         undo_arena( ch );

      if ( ch->in_room == NULL )
         continue;

      if ( IS_CLASS( ch, CLASS_MONK ) && ch->ctimer[8] > 0 )
         continue;

      if ( ch->air_timer > 0 )
         {
            ch->air_timer--;

            if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 && number_percent() > 70 )
               do_emergency_flip( ch, "" );
               else if ( ch->air_timer == 0 )
               {
                  act( "You CRASH loudly into tho ground causing the vicinity to tremble.", ch, NULL, NULL, TO_CHAR );
                  act( "$n CRASHES loudly into tho ground causing the vicinity to tremble.", ch, NULL, NULL, TO_ROOM );
                  act( "You bounce of the floor like a RaGgEd DoLl as you are slammed across the room.", ch, NULL, NULL, TO_CHAR );
                  act( "$n bounces of the floor like a RaGgEd DoLl as $e is slammed across the room.", ch, NULL, NULL, TO_ROOM );

                  if ( !( ( IS_CLASS( ch, CLASS_ANGEL ) || IS_CLASS( ch, CLASS_SHADOW_WAR ) ) && ch->ctimer[17] > 0 )
                       && str_cmp( "N", ch->groove ) && str_cmp( "A", ch->groove ) && str_cmp( "M", ch->groove ) )
                     ch->hit -= 750000;

                  update_pos( ch );

                  if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571
                       && ch->position == POS_DEAD )
                     ch->position = POS_STUNNED;
                     else if ( ch->position == POS_DEAD )
                     {
                        raw_kill( ch );
                        continue;
                     }    
               }
               else stc( "The wind ROARS through you as you fall rapidly towards the earth.\n\r", ch );

            continue;
         }

      if ( !IS_NPC( ch ) )
         {
            if ( ch->pcdata->blood_meter > 0 )
               {
                  ch->pcdata->blood_meter--;
 
                  if ( ch->pcdata->blood_meter == 0 )
                     {
                        ch->pcdata->blood_power = 0;
                        act( "#wYou #rsigh #gas your #ysupernatural #Rpowers #edisappear...#n", ch, NULL, NULL, TO_CHAR );
                        act( "#w$n #rsighs #gas $s #ysupernatural #Rpowers #edisappear...#n", ch, NULL, NULL, TO_ROOM );
                     }
               }

            if ( !str_cmp( "A", ch->groove ) || !str_cmp( "M", ch->groove ) || ch->roll_timer > 1 ) 
               {
                  if ( ch->position == POS_STUNNED && ch->hit > 0 )
                     {
                        ch->position = POS_STANDING;
                        act("You suddenly roll backwards as you flip onto your feet.",ch,NULL,NULL,TO_CHAR);
                        act("$n suddenly rolls backwards as $e flips onto $s feet.",ch,NULL,NULL,TO_ROOM);
                        ch->wait = 0;
                     }

                  ch->roll_timer = 0;
               }

            if ( ch->position == POS_STUNNED && IS_SET( ch->cmbt[5], SKL2_ROLL ) )
               ch->roll_timer++;
         }
         else if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
                 {
                     if ( ( ch->position == POS_STUNNED || ch->position == POS_SLEEPING ) && ch->hit > 0
                          && number_percent() > 60 )
                        {
                           do_emergency_roll( ch, "" );
                           ch->fight_timer = 0;
                        }
                 }

      check_arena( ch );

      if ( ch->holding != NULL )
         {
            if ( ch->in_room != ch->holding->in_room )
               {
                  stc("You release your hold on your target.\n\r",ch);
                  stc("someone releases their hold on you.\n\r",ch->holding);
                  ch->holding->holded = NULL;
                  ch->holding = NULL;
               }
               else if ( ch->hunt_timer > 0 )
                       {
                           act("You tighten your arms around $N's throat choking the hell out of $M!",ch,NULL,ch->holding,TO_CHAR);
                           act("$n tightens $s arms around your throat choking the hell out of you!",ch,NULL,ch->holding,TO_VICT);
                           act("$n tightens $s arms around $N's throat choking the hell out of $M!",ch,NULL,ch->holding,TO_NOTVICT);
                           ch->hunt_timer--;
                           hurt_person(ch,ch->holding,100000,TRUE);
                       }
                       else {
                               act("You release your hold on your target.",ch,NULL,NULL,TO_CHAR);
                               act("$n releases $s hold on you.",ch,NULL,ch->holding,TO_VICT);

                               if ( ch->holding != NULL )
                                  {
                                     ch->holding->holded = NULL;
                                     ch->holding = NULL;
                                  }
      
                               if ( ch == NULL )
                                  continue;
                            }
         }

     if ( ( IS_CLASS( ch, CLASS_ANGEL ) || IS_CLASS( ch, CLASS_SHADOW_WAR ) ) && ch->ctimer[17] > 0 )
        {
           if ( --ch->ctimer[17] == 0 )
              {
                 if ( IS_CLASS( ch, CLASS_ANGEL ) )
                    do_trueform( ch, "" );

                 act( "You collapse onto your knees unable to withstand the pain any longer.", ch, NULL, NULL, TO_CHAR );
                 act( "$n collapses onto $s knees unable to withstand the pain any longer.", ch, NULL, NULL, TO_ROOM );
                 
                 ch->hit = UMAX( 1, ch->hit - ch->ctimer[16] );
                 ch->ctimer[16] = 0;
                 update_pos( ch );
              }
        }

      do_cleansing(ch);
      do_redemption_aura(ch);

      if ( !IS_NPC( ch ) && number_percent() > 49 && ( ch->outer_aura == A_HFIRE || ch->inner_aura == A_HFIRE || IS_SET(ch->group_aura, G_HFIRE) ) )
         {
            int level = 0;
            int dam;
            CHAR_DATA *vch, *vch_next;
            bool veng = ( can_do_skill( ch, 51 ) ? TRUE : FALSE );

            if ( !IS_SET(ch->group_aura, G_HFIRE) )
               {
                  if ( ch->inner_aura == A_HFIRE )
                     level += level + 1;

                  level += ch->aura[A_HFIRE];
               }
               else level = ch->special_aura[A_HFIRE];
 
            if ( level > 10 )
               level = 10;

            if ( IS_NPC( ch ) )
               dam = ( level * 3000 );
               else dam = ( level * 30000 );

            act("The flames around $n's body intensifies scorching everyone in the room.",ch,NULL,NULL,TO_ROOM);
            act("The flames around your body intensifies scorching everyone in the room.",ch,NULL,NULL,TO_CHAR);

            for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
            {
                 vch_next = vch->next_in_room;
  
                 if ( vch->fighting != ch && ch->fighting != vch )
                    continue;

                 if ( is_same_group(ch,vch) || is_safe(ch, vch) || vch == ch )
                    continue;

                 act("You scream loudly as the holy flames sears your flesh.",vch,NULL,NULL,TO_CHAR);
                 act("$n screams loudly as the holy flames sears $s flesh.",vch,NULL,NULL,TO_ROOM);
                 ch->ele_attack = ATTACK_FIRE;
                 ch->attack_type = ATTACK_MAGICAL;
                 ch->attack_var = BLAST;
                 ch->attack_focus = DIRECT;

                 if ( veng && number_percent() > 74 )
                    {
                       if ( !IS_AFFECTED( vch, AFF_FLAMING ) )
                          SET_BIT( vch->affected_by, AFF_FLAMING );

                       hurt_person( ch, vch, dam * 2, TRUE );
                    }
                    else hurt_person( ch, vch, dam, TRUE );
            }
         }

      if ( !IS_NPC( ch ) && number_percent() > 49 && ( ch->outer_aura == A_HFREEZE || ch->inner_aura == A_HFREEZE || IS_SET(ch->group_aura, G_HFREEZE) ) )
         {
            int level = 0;
            int dam;
            CHAR_DATA *vch, *vch_next;
            bool veng = ( can_do_skill( ch, 51 ) ? TRUE : FALSE );

            if ( !IS_SET(ch->group_aura, G_HFREEZE) )
               {
                   if ( ch->inner_aura == A_HFREEZE )
                      level += level + 1;

                   level += ch->aura[A_HFREEZE];
               }
               else level = ch->special_aura[A_HFREEZE];
 
            if ( level > 10 )
               level = 10;

            act("The icy aura around $n's body explodes into countless icicles freezing everyone in the room.",ch,NULL,NULL,TO_ROOM);
            act("The icy aura around your body explodes into countless icicles freezing everyone in the room.",ch,NULL,NULL,TO_CHAR);

            if ( IS_NPC( ch ) )
               dam = ( level * 3000 );
               else dam = ( level * 30000 );

            for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
            {
                 vch_next = vch->next_in_room;
  
                 if ( vch->fighting != ch && ch->fighting != vch )
                    continue;

                 if ( is_same_group(ch,vch) || is_safe(ch, vch) || vch == ch )
                    continue;

                 act("Your body stiffens as the holy ice stops all of your movements.",vch,NULL,NULL,TO_CHAR);
                 act("$n's body stiffens as the holy ice stops all of $s movements.",vch,NULL,NULL,TO_ROOM);
                 ch->ele_attack = ATTACK_ICE;
                 ch->attack_type = ATTACK_MAGICAL;
                 ch->attack_var = HIT;
                 ch->attack_focus = DIRECT;

                 if ( veng && number_percent() > 84 )
                    {
                       if ( !is_martial( vch ) && !IS_SET( vch->more, MORE_FROZEN ) )
                          {
                             vch->freeze_timer = 1;
                             SET_BIT( vch->more, MORE_FROZEN );
                          }

                       hurt_person( ch, vch, dam * 1.5, TRUE );
                    }
                    else hurt_person( ch, vch, dam, TRUE );
            }
         }

      if ( !IS_NPC( ch ) && number_percent() > 49 && ( ch->outer_aura == A_HSHOCK || ch->inner_aura == A_HSHOCK || IS_SET(ch->group_aura, G_HSHOCK) ) )
         {
            int level = 0;
            int dam;
            CHAR_DATA *vch, *vch_next;
            bool veng = ( can_do_skill( ch, 51 ) ? TRUE : FALSE );

            if ( !IS_SET(ch->group_aura, G_HSHOCK) )
               {
                  if ( ch->inner_aura == A_HSHOCK )
                     level += level + 1;

                  level += ch->aura[A_HSHOCK];
               }
               else level = ch->special_aura[A_HSHOCK];
 
            if ( level > 10 )
               level = 10;

            act("The sparkles of holy energy around $n's body flashes through the room shocking everyone.",ch,NULL,NULL,TO_ROOM);
            act("The sparkles of holy energy around your body flashes through the room shocking everyone.",ch,NULL,NULL,TO_CHAR);

            if ( IS_NPC( ch ) )
               dam = ( level * 3000 );
               else dam = ( level * 30000 );

            for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
            {
                 vch_next = vch->next_in_room;
  
                 if ( vch->fighting != ch && ch->fighting != vch )
                    continue;

                 if ( is_same_group(ch,vch) || is_safe(ch, vch) || vch == ch )
                    continue;

                 act("You shake violently as the holy electricity shock circuited your mortal body.",vch,NULL,NULL,TO_CHAR);
                 act("$n shakes violently as the holy electricity shock circuited $s mortal body.",vch,NULL,NULL,TO_ROOM);
                 ch->ele_attack = ATTACK_THUNDER;
                 ch->attack_type = ATTACK_MAGICAL;
                 ch->attack_var = BLUNT;
                 ch->attack_focus = DIRECT;

                 if ( veng && number_percent() > 84 )
                    {
                       if ( !is_martial( vch ) && vch->stunned == 0 )
                          vch->stunned = 1;

                       hurt_person( ch, vch, dam * 1.5, TRUE );
                    }
                    else hurt_person( ch, vch, dam, TRUE );
            }
         }

      if ( !IS_NPC( ch ) && number_percent() > 49 && ( ch->outer_aura == A_SANCTUARY || ch->inner_aura == A_SANCTUARY || IS_SET(ch->group_aura, G_SANCTUARY) ) )
         {
            int level = 0;
            int dam;
            CHAR_DATA *vch, *vch_next;

            if ( !IS_SET(ch->group_aura, G_SANCTUARY) )
               {
                  if ( ch->inner_aura == A_SANCTUARY )
                     level += level + 1;

                  level += ch->aura[A_SANCTUARY];
               }
               else level = ch->special_aura[A_SANCTUARY];
 
            if ( level > 10 )
               level = 10;

            if ( IS_NPC( ch ) )
               dam = ( level * 3000 );
               else dam = ( level * 30000 );

            act("$n meditates softly as blinding light rises from the earth.",ch,NULL,NULL,TO_ROOM);
            act("You meditate softly as blinding light rises from the earth.",ch,NULL,NULL,TO_CHAR);

            for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
            {
                 vch_next = vch->next_in_room;

                 if ( vch->fighting != ch && ch->fighting != vch )
                    continue;

                 if ( is_safe(ch, vch) || vch == ch )
                    continue;

                 act("You stumble onto your knees as the light manifests you.",vch,NULL,NULL,TO_CHAR);
                 act("$n stumbles onto $s knees as the light manifests $m.",vch,NULL,NULL,TO_ROOM);
                 ch->ele_attack = ATTACK_HOLY;              
                 ch->attack_type = ATTACK_MAGICAL;
                 ch->attack_var = SOUL;
                 ch->attack_focus = DIRECT;

                 hurt_person( ch, vch, dam, TRUE );

                 if ( vch->in_room == NULL || number_percent() < 90 || vch->position <= POS_STUNNED )
                    continue;

                 if ( !IS_NPC( vch ) && !IS_SET( vch->more, MORE_BLIND ) )
                    {
                       vch->blind_timer = 6;
                       SET_BIT( vch->more, MORE_BLIND );
                       act("#w$n currently can't see as $s eyes is filled with the intense light.#n",vch,NULL,NULL,TO_ROOM);
                       continue;
                    }

                 if ( !IS_NPC( vch ) )
                    continue;

                 stop_fighting( vch, TRUE );
                 vch->position = POS_STUNNED;                 
            }
         }

         if ( ch->position > POS_INCAP && ( ch->outer_aura == A_PRAYER || ch->inner_aura == A_PRAYER || IS_SET(ch->group_aura, G_PRAYER) ) )
         {
            int level = 0;

            if ( !IS_SET(ch->group_aura, G_PRAYER) )
               {
                  if ( ch->inner_aura == A_PRAYER )
                     level += level + 1;

                  level += ch->aura[A_PRAYER];
               }
               else level = ch->special_aura[A_PRAYER];
 
            if ( level > 10 )
               level = 10;

            hit_gain( ch, ( 7500 * level ) );
         }
         
         if ( ch->hit > 0 && ch->hit < ch->max_hit && ( ch->outer_aura == A_HEALING || ch->inner_aura == A_HEALING || IS_SET(ch->group_aura, G_HEALING) ) )
         {
            int level = 0;

            if ( !IS_SET(ch->group_aura, G_HEALING) )
               {
                  if ( ch->inner_aura == A_HEALING )
                     level += level + 1;

                  level += ch->aura[A_HEALING];
               }
               else level = ch->special_aura[A_HEALING];
 
            if ( level > 10 )
               level = 10;

            hit_gain( ch, ( 5000 * level ) );
            move_gain( ch, ( 10000 * level ) );
            mana_gain( ch, ( 10000 * level ) );
        }
         
       if ( ch->position > POS_INCAP && ch->fighting != NULL && ch->fighting->hit > 50000 && ( ch->outer_aura == A_LTAP || ch->inner_aura == A_LTAP || IS_SET(ch->group_aura, G_LTAP) ) )
          {
             int level = 0;
             int bonus = 1;
             CHAR_DATA *victim = ch->fighting;

             if ( !IS_SET(ch->group_aura, G_LTAP) )
                {
                   if ( ch->inner_aura == A_LTAP )
                      level += level + 1;

                   level += ch->aura[A_LTAP];
                }
                else level = ch->special_aura[A_LTAP];
 
             if ( level > 10 )
                level = 10;

             hit_gain( ch, ( 5000 * level ) );
             move_gain( ch, ( 7500 * level ) );
             mana_gain( ch, ( 7500 * level ) );
 
             victim->hit = UMAX( 1, victim->hit - ( level * 5000 ) );
             victim->mana = UMAX( 0, victim->mana - ( level * 5000 ) );
             victim->move = UMAX( 0, victim->move - ( level * 5000 ) );
          }

       if ( ch->outer_aura == A_VIGOR || ch->inner_aura == A_VIGOR || IS_SET(ch->group_aura, G_VIGOR) )
          {
              int level = 0;

              if ( !IS_SET(ch->group_aura, G_VIGOR) )
                 {
                    if ( ch->inner_aura == A_VIGOR )
                       level += level + 1;

                    level += ch->aura[A_VIGOR];
                 }
                 else level = ch->special_aura[A_VIGOR];
 
              if ( level > 10 )
                 level = 10;

              move_gain( ch, ( 20000 * level ) );
          }

      if ( ch->outer_aura == A_MEDITATION || ch->inner_aura == A_MEDITATION || IS_SET(ch->group_aura, G_MEDITATION) )
         {
            int level = 0;

            if ( !IS_SET(ch->group_aura, G_MEDITATION) )
               {
                  if ( ch->inner_aura == A_MEDITATION )
                     level += level + 1;

                  level += ch->aura[A_MEDITATION];
               }
               else level = ch->special_aura[A_MEDITATION];
 
            if ( level > 10 )
               level = 10;

            mana_gain( ch, ( 20000 * level ) );
         }

	if ( ch->fight_timer > 0 )
         {
            ch->fight_timer--;

            if ( ch->fight_timer == 0 )
               {
                  if ( is_affected( ch, skill_lookup( "battle tactics" ) ) )
                     SET_BIT( ch->more3, MORE3_RETORT );

                  if ( ch->attacker != NULL )
                     {
                        if ( nogang != 0 )
                           {
                              restore_char( ch->attacker );
                              restore_char( ch );
                           }

                        if ( ch->attacker->attacker != NULL )
                           ch->attacker->attacker = NULL;

                        ch->attacker = NULL;
                     }
               }
         }

     if ( ch == NULL || ch->in_room == NULL )
        continue;

    if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_KURUDA ) )
       {
           if ( IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
              continue;
       }
  
    if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL || victim->in_room == NULL )
       continue;

    if ( ch->holding != NULL || ch->holded != NULL )
       continue;

    if ( !IS_NPC(ch) && ch->pcdata->capture_timer > 0 )
       continue;

    if ( IS_SET( ch->cmbt[5], SKL2_SCORCHING ) && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       scorch_equip( ch, victim );

    if ( IS_SET( ch->cmbt[7], SKL_POWERB ) && !IS_SET( victim->cmbt[7], SKL_STUNA )
         && number_percent() > 74 && ch->power_timer == 0 && !is_martial( victim )
         && victim->stunned == 0 )
       {
          act("The air vibrates loudly as you draw back your weapon!",ch,NULL,NULL,TO_CHAR);
          act("The air vibrates loudly as $n draws back $s weapon!",ch,NULL,NULL,TO_ROOM);
          act("You twirl your weapon SmAsHiNg it into the ground causing the earth to shatter!",ch,NULL,NULL,TO_CHAR);
          act("$n twirls $s weapon SmAsHiNg it into the ground causing the earth to shatter!",ch,NULL,NULL,TO_ROOM);
          act("You crash into the ground stunned.",victim,NULL,NULL,TO_CHAR);
          act("$n crashes into the ground stunned.",victim,NULL,NULL,TO_ROOM);
          ch->power_timer = 25;

          if ( !IS_NPC( ch ) && ch->pcdata->deity == DEI_GAIA )
             {
                victim->stunned = 2;
                ch->attack_type = ATTACK_PHYSICAL;
                ch->attack_var = BLUNT;
                ch->attack_focus = OVERHEAD;
                hurt_person( ch, victim, 500000, TRUE );
             }
             else victim->stunned = 1;
       }

    if ( IS_AWAKE(ch) && IS_AWAKE(victim) && ch->in_room == victim->in_room ) 
       {
            if ( IS_NPC(ch) ) 
               {
                 melee = FALSE;

                 if ( ch->pIndexData->vnum == 16 )
                    do_hell_hound(ch,"");

                 if ( ch->pIndexData->vnum == 14 )
                    do_spider(ch,"");

                 if ( ch->pIndexData->vnum == 550 || ch->pIndexData->vnum == 557 || ch->pIndexData->vnum == 552 ) 
                    do_knight(ch,"");

                 if ( ch->pIndexData->vnum == 551 )
                    do_drow(ch,"");

                 if ( ch->pIndexData->vnum == 565 )
                    do_ryo(ch,"");

                 if ( ch->pIndexData->vnum == 564 )
                    do_kyo(ch,"");

                 if ( ch->pIndexData->vnum == 563 )
                    do_zero(ch,"");

                 if ( ch->pIndexData->vnum == 553 )
                    do_angel(ch,"");

                 if ( ch->pIndexData->vnum == 561 )
                    do_sparda(ch,"");

                 if ( ch->pIndexData->vnum == 556 )
                    do_vampire(ch,"");

                 if ( ch->pIndexData->vnum == 559 || ch->pIndexData->vnum == 560 )
                    do_ninja(ch,"");

                 if ( ch->pIndexData->vnum == 554 )
                    do_terry(ch,"");

                 if ( ch->pIndexData->vnum == 562 )
                    do_andy(ch,"");

                 if ( ch->pIndexData->vnum == 566 )
                    do_kk(ch,"");

                 if ( ch->pIndexData->vnum == 558 )
                    do_joe(ch,"");

                 if ( ch->pIndexData->vnum == 570 )
                    do_killer(ch,"");

                 if ( ch->pIndexData->vnum == 4507 )
                    do_soldier(ch,"");

                 if ( ch->pIndexData->vnum == 555 )
                    do_krizalid(ch,"");
				 
                 if ( ch->pIndexData->vnum == 567 )
                    do_robbie(ch,"");

                 if ( ch->pIndexData->vnum == 568 )
                    do_sdemon(ch,"");

                 if ( ch->pIndexData->vnum == 569 )
                    do_guardian(ch,"");

                 melee = TRUE;
                 REMOVE_BIT( ch->arena, AFF2_DM );

                 if ( ( victim = ch->fighting ) == NULL )
                    continue;
               }

            multi_hit( ch, victim, TYPE_UNDEFINED );
      } 
      else stop_fighting( ch, FALSE );

      if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_KURUDA ) && IS_SET( victim->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
         continue;

	if ( ( victim = ch->fighting ) == NULL )
	    continue;

      if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && IS_SET(ch->polyaff, POLY_SERPENT) && number_percent() > 50 )
         {
            do_vbite(ch,"");        
	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

      if ( ch->hit < ch->max_hit * 0.25 && number_percent() > 84 && can_do_skill( ch, 41 ) && ch->achi == 0 )
         {
            do_gilgamesh( ch, "" );

	      if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( ch->hit < ch->max_hit * 0.25 && number_percent() > 84 && can_do_skill( ch, 78 ) && ch->achi == 0 )
         {
            do_mywind( ch, "" );

	      if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( ch->hit < ch->max_hit * 0.25 && number_percent() > 84 && can_do_skill( ch, 54 ) && ch->achi == 0 )
         {
            do_lancelot( ch, "" );

	      if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( ch->hit < ch->max_hit * 0.25 && number_percent() > 84 && IS_CLASS( ch, CLASS_ANGEL ) && ch->pkill > 19
           && ch->achi == 0 )
         {
            do_tyrael( ch, "" );

	      if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( ch->hit < ch->max_hit * 0.25 && number_percent() > 84 && can_do_skill( ch, 75 ) && ch->achi == 0 )
         {
            do_lloth( ch, "" );

	      if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( IS_SET( ch->more3, MORE3_WOLF ) && ch->hit < ch->max_hit * 0.25 && number_percent() > 80 
           - devil_luck( ch, victim, 10 ) )
         {
            perform_charge( ch, victim );

	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

	  if ( IS_CLASS(ch, CLASS_WISP ) && ch->pkill >= 12 && IS_SET(ch->more, MORE_LIGHT) && number_percent() > 89 && !IS_SET( victim->more, MORE_BLIND ) )
           {
              act("You turn bright #7white#n and blind $N!",ch,NULL,victim,TO_CHAR);
              act("$n turns bright #7white#n and blinds $N!",ch,NULL,victim,TO_NOTVICT);
              act("$n turns bright #7white#n and blinds you!",ch,NULL,victim,TO_VICT);
              victim->blind_timer = number_range( 2, 5 );
              SET_BIT( victim->more, MORE_BLIND );
           }

	  if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WIZARD) && ( ch->clannum == 1 || ch->clannum == 6 ) && number_percent() > 70 )
         {
            act( "You dance swiftly in and out of $N's range while slashing $M with an energy blade.", ch, NULL,victim, TO_CHAR );
            act( "$n dances swiftly in and out of your range while slashing you with an energy blade.", ch, NULL, victim, TO_VICT );
            act( "$n dances swiftly in and out of $N's range while slashing $M with an energy blade.", ch, NULL, victim, TO_NOTVICT );

            if ( number_percent() > 89 && ch->hit <= ch->max_hit * 0.1 && !IS_SET( victim->cmbt[5], SKL2_AMIND ) )
               {
                  act( "You suddenly twirl around in a stylish pose as your blade disappears!", ch, NULL, NULL, TO_CHAR );
                  act( "$n suddenly twirls around in a stylish pose as $s blade disappears!", ch, NULL, NULL, TO_ROOM );
                  act( "You exclaim '#yFarewell...#n' as your blade rips out of $N's body!!!", ch, NULL, victim, TO_CHAR );
                  act( "$n exclaims '#yFarewell...#n' as $s blade rips out of your body!!!", ch, NULL, victim, TO_VICT );
                  act( "$n exclaims '#yFarewell...#n' as $s blade rips out of $N's body!!!", ch, NULL, victim, TO_NOTVICT );

                  stop_fighting( victim, TRUE );
                  victim->hit = -2000010;
                  victim->position = POS_MORTAL;
                  continue;
               }
              
            ch->attack_type = ATTACK_PHYSICAL;
            hurt_person( ch, victim, 100000, TRUE );
         }

      if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
         {
            OBJ_DATA *obj;
            int damage = 50000;

            if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
                 && obj->value[3] == 2 && number_percent() > 86 - devil_luck( ch, victim, 10 ) )
               {
                  damage = 75000;

                  act("You rush to $N as sparkling energy converges on your spear.", ch,NULL,victim,TO_CHAR );
                  act("$n rushes to you as sparkling energy converges on $s spear.", ch,NULL,victim,TO_VICT );
                  act("$n rushes to $N as sparkling energy converges on $s spear.", ch,NULL,victim,TO_NOTVICT );

                  act("You stab $N viciously in the stomach as you brace for your next attack.", ch,NULL,victim,TO_CHAR );
                  act("$n stabs you viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_VICT );
                  act("$n stabs $N viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_NOTVICT );

                  act("You stab $N rapidly in the face as your spear becomes a blur.", ch,NULL,victim,TO_CHAR );
                  act("$n stabs you rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_VICT );
                  act("$n stabs $N rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_NOTVICT );

                  act("You stab $N viciously in the stomach as you brace for your next attack.", ch,NULL,victim,TO_CHAR );
                  act("$n stabs you viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_VICT );
                  act("$n stabs $N viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_NOTVICT );

                  act("You stab $N rapidly in the face as your spear becomes a blur.", ch,NULL,victim,TO_CHAR );
                  act("$n stabs you rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_VICT );
                  act("$n stabs $N rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_NOTVICT );

                  act("You stab $N viciously in the stomach as you leap back a few feet.", ch,NULL,victim,TO_CHAR );
                  act("$n stabs you viciously in the stomach as $e leaps back a few feet.", ch,NULL,victim,TO_VICT );
                  act("$n stabs $N viciously in the stomach as $e leaps back a few feet.", ch,NULL,victim,TO_NOTVICT );

                  ch->attack_type = ATTACK_PHYSICAL;
                  ch->attack_focus = DIRECT;
                  ch->attack_var = PIERCE;

                  hurt_person( ch, victim, damage, TRUE );

                  if ( ( victim = ch->fighting ) == NULL )
                     continue;
               }

            if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
                 && obj->value[3] == 2 && number_percent() > 70 - devil_luck( ch, victim, 10 ) )
               {
                  act( "You suddenly dash towards $N as you trip $M swiftly with your spear.", ch, NULL, victim, TO_CHAR );
                  act( "$n suddenly dashes towards you as $e trips you swiftly with $s spear.", ch, NULL, victim, TO_VICT );
                  act( "$n suddenly dashes towards $N as $e trips $M swiftly with $s spear.", ch, NULL, victim, TO_NOTVICT );

                  if ( number_percent() > 40 - devil_luck( ch, victim, 5 ) )
                     {
                        damage = 75000;
                        act( "You then stand above $N as you drive your spear into $S body!", ch, NULL, victim, TO_CHAR );
                        act( "$n then stands above you as $e drives $s spear into your body!", ch, NULL, victim, TO_VICT );
                        act( "$n then stands above $N as $e drives $s spear into $S body!", ch, NULL, victim, TO_NOTVICT );
                        ch->attack_focus = OVERHEAD;
                     }
                     else ch->attack_focus = LOW;

                  ch->attack_type = ATTACK_PHYSICAL;
                  ch->attack_var = PIERCE;
                  
                  hurt_person( ch, victim, damage, TRUE );
               }

            if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
                 && obj->value[3] == 3 && number_percent() > 86 - devil_luck( ch, victim, 10 ) )
               {
                  damage = 75000;
                  act("You grip your rapier tightly as you swipe beautifully across the air!",ch,NULL,NULL,TO_CHAR);
                  act("$n grips $s rapier tightly as $e swipes beautifully across the air!",ch,NULL,NULL,TO_ROOM);
                  act("You scream '#yKill!#n' as your blurry rapier singes $N's flesh!",ch,NULL,victim,TO_CHAR);
                  act("$n screams '#yKill!#n' as $s blurry rapier singes your flesh!",ch,NULL,victim,TO_VICT);
                  act("$n screams '#yKill!#n' as $s blurry rapier singes $N's flesh!",ch,NULL,victim,TO_NOTVICT);
                  act("As you are SmAsHEd away from the impact, demonic energy rips through your body!",victim,NULL,NULL,TO_CHAR);
                  act("As $n is SmAsHEd away from the impact, demonic energy rips through $s body!",victim,NULL,NULL,TO_ROOM);

                  ch->ele_attack = ATTACK_DARK;
                  ch->attack_type = ATTACK_PHYSICAL;
                  ch->attack_focus = UPPERCUT;
                  ch->attack_var = SLASH;
                  hurt_person( ch, victim, damage, TRUE );

                  if ( ( victim = ch->fighting ) == NULL )
                     continue;
               }
               else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
                         && obj->value[3] == 3 && number_percent() > 86 - devil_luck( ch, victim, 10 ) )
               {
                  act("You drive your sword into the ground viciously as you grip the blade tightly!",ch,NULL,NULL,TO_CHAR);
                  act("$n drives $s sword into the ground viciously as $e grips the blade tightly!",ch,NULL,NULL,TO_ROOM);
                  act("You concentrate softly as a soft humming fills the air.",ch,NULL,NULL,TO_CHAR);
                  act("$n concentrates softly as a soft humming fills the air.",ch,NULL,NULL,TO_ROOM);
                  act("The air around you roars as palish energy from $N is sucked into your blade.",ch,NULL,victim,TO_CHAR);
                  act("The air around $n roars as palish energy from you is sucked into $s blade.",ch,NULL,victim,TO_VICT);
                  act("The air around $n roars as palish energy from $N is sucked into $s blade.",ch,NULL,victim,TO_NOTVICT);
                  act("You feel weak as your lifeforce is taken from you.",victim,NULL,NULL,TO_CHAR);
                  act("$n feels weaker as $s lifeforce is taken from $m.",victim,NULL,NULL,TO_ROOM);

                  ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );
                  ch->attack_type = ATTACK_MAGICAL;
                  ch->ele_attack = ATTACK_LUNAR;
                  ch->attack_focus = DIRECT;
                  ch->attack_var = SOUL;
                  hurt_person( ch, victim, 25000, TRUE );
               }
               else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
                         && obj->value[3] == 3 && number_percent() > 70 - devil_luck( ch, victim, 10 ) )
               {
                  damage = 50000;
                  act("Tilting forward, you leap towards $N!",ch,NULL,victim,TO_CHAR);
                  act("Tilting forward, $n leaps towards you!",ch,NULL,victim,TO_VICT);
                  act("Tilting forward, $n leaps towards $N!",ch,NULL,victim,TO_NOTVICT);
                  act("You exclaim '#yEn Guarde#n' thrusting your blade through $N's chest!",ch,NULL,victim,TO_CHAR);
                  act("$n exclaims '#yEn Guarde#n' thrusting $s blade through your chest!",ch,NULL,victim,TO_VICT);
                  act("$n exclaims '#yEn Guarde#n' thrusting $s blade through $N's chest!",ch,NULL,victim,TO_NOTVICT);

                  ch->attack_type = ATTACK_PHYSICAL;
                  ch->attack_focus = DIRECT;
                  ch->attack_var = PIERCE;
                  hurt_person( ch, victim, damage, TRUE );

                  if ( ( victim = ch->fighting ) == NULL )
                     continue;
               }

            if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
                 && obj->value[3] == 3 && !IS_SET( victim->more, MORE_BLIND ) && number_percent() > 90 )
               {
                  act("You crouch on your knees tilting your sword in the light.",ch,NULL,NULL,TO_CHAR);
                  act("$n crouches on $s knees tilting $s sword in the light.",ch,NULL,NULL,TO_ROOM);
                  act("A blast of searing light strikes your face as you can no longer see.",victim,NULL,NULL,TO_CHAR);
                  act("A blast of searing light strikes $n's face as $e can no longer see.",victim,NULL,NULL,TO_ROOM);
                  SET_BIT(victim->more, MORE_BLIND);
                  victim->blind_timer = number_range( 1, 3 );
                  
                  if ( !is_martial( victim ) && victim->wait < 12 )
                     victim->wait = 12;
               }

            if ( ( victim = ch->fighting ) == NULL )
               continue;
         }
		    
      if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_ANGEL) && number_percent() > 70 - devil_luck( ch, victim, 5 ) 
           && IS_SET( ch->more3, MORE3_HAND) )
         {
            act("You stand subtively as you place your hands on your waist.",ch,NULL,NULL,TO_CHAR);                        
            act("$n stands subtively as $e places $s hands on $s waist.",ch,NULL,NULL,TO_ROOM);       
            act("You suddenly push your hand across you releasing a burst of flames onto $N.",ch,NULL,victim,TO_CHAR);       
            act("$n suddenly pushes $s hand across $m releasing a burst of flames onto you.",ch,NULL,victim,TO_VICT);
            act("$n suddenly pushes $s hand across $m releasing a burst of flames onto $N.",ch,NULL,victim,TO_NOTVICT);

            ch->ele_attack = ATTACK_HOLY;
            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = DIRECT;
            ch->attack_var = BLAST;
            hurt_person( ch, victim, 150000, TRUE );

 	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

      if ( !IS_NPC( ch ) && is_affected( ch, skill_lookup("ice dust") ) && number_percent() > 60 - devil_luck( ch, victim, 15 ) )
         {
            act("You let go a soft smile as you raise your palm into the air.",ch,NULL,NULL,TO_CHAR);                        
            act("$n lets go a soft smile as $e raises $s palm into the air.",ch,NULL,NULL,TO_ROOM);                        
            act("The swirling dust around you suddenly converges into an #Lice #wshard#n.",ch,NULL,NULL,TO_CHAR);
            act("The swirling dust around $n suddenly converges into an #Lice #wshard#n.",ch,NULL,NULL,TO_ROOM);
            act("You then throw the shard at $N as it penetrates $S chest.",ch,NULL,victim,TO_CHAR);
            act("$n then throws the shard at you as it penetrates your chest.",ch,NULL,victim,TO_VICT);
            act("$n then throws the shard at $N as it penetrates $S chest.",ch,NULL,victim,TO_NOTVICT);

            if ( IS_CLASS( victim, CLASS_SNOW_ELF ) )
               victim->hit = UMIN( victim->max_hit, victim->hit + UMIN( 250000, is_elemental( ATTACK_ICE, victim, 50000 ) * -1 ) );
               else {
                       ch->ele_attack = ATTACK_ICE;
                       ch->attack_type = ATTACK_MAGICAL;
                       ch->attack_focus = DIRECT;
                       ch->attack_var = PIERCE;

                       if ( IS_CLASS( ch, CLASS_SNOW_ELF ) && ch->class_2 == 0 )
                          hurt_person( ch, victim, 400000, TRUE );
                          else hurt_person( ch, victim, 100000, TRUE );
                    }

            if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && IS_SET(ch->polyaff, POLY_SERPENT) 
           && number_percent() > 75 - devil_luck( ch, victim, 5 ) )
         {
            do_tongue_lash(ch,"");        
	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

      if ( !IS_NPC(ch) && ( IS_CLASS(ch,CLASS_SAMURAI) || IS_SET( ch->cmbt[5], SKL2_DISARM ) ) 
           && !IS_SET( victim->cmbt[5], SKL2_GRIP ) && number_percent() > 89 - devil_luck( ch, victim, 5 ) )
         {
            OBJ_DATA *obj;
            bool weapon_test = TRUE;

            if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
               if ( ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
                  weapon_test = FALSE;

            if ( IS_CLASS(victim, CLASS_PALADIN) || IS_CLASS(victim, CLASS_ASSASSIN) )
               weapon_test = FALSE;

            if ( weapon_test == TRUE )
               {
                  act("\n\rYou suddenly grab $N by the neck and lift $M in the air.",ch,NULL,victim,TO_CHAR);
                  act("\n\r$n suddenly grabs you by the neck and lifts you in the air.",ch,NULL,victim,TO_VICT);
                  act("\n\r$n suddenly grabs $N by the neck and lift $M in the air.",ch,NULL,victim,TO_NOTVICT);
                  act("You snarl '#yEia... Soi Thi Zun!#n' as you punch $N viciously in the stomach.",ch,NULL,victim,TO_CHAR);
                  act("$n snarls '#yEia... Soi Thi Zun!#n' as $e punches you viciously in the stomach.",ch,NULL,victim,TO_VICT);
                  act("$n snarls '#yEia... Soi Thi Zun!#n' as $epunches $N viciously in the stomach.",ch,NULL,victim,TO_NOTVICT);
                  act("You spin your body around sending $N flying with a roundhouse kick.",ch,NULL,victim,TO_CHAR);
                  act("$n spins $s body around sending you flying with a roundhouse kick.",ch,NULL,victim,TO_VICT);
                  act("$n spins $s body around sending $N flying with a roundhouse kick.",ch,NULL,victim,TO_NOTVICT);
                  act("At the last second, you slash across $N's arms disarming $M.",ch,NULL,victim,TO_CHAR);
                  act("At the last second, $n slashes across your arms disarming you.",ch,NULL,victim,TO_VICT);
                  act("At the last second, $n slashes across $N's arms disarming $M.",ch,NULL,victim,TO_NOTVICT);

                  obj_from_char( obj );
                  obj_to_char( obj, victim );

                  ch->attack_type = ATTACK_PHYSICAL;
                  ch->attack_focus = OVERHEAD;
                  ch->attack_var = HIT;
                  hurt_person( ch, victim, 250000, TRUE );

                  if ( ( victim = ch->fighting ) == NULL )
                     continue;
               }
         }

      if ( !IS_NPC(ch) && ch->pcdata->kingdom == 1 && ch->pcdata->clanrank > 4 && number_percent() > 74
           && IS_EVIL( victim ) && !IS_CLASS( victim, CLASS_ANGEL ) )
         {
            act( "#yYou #wdash straight towards #R$N #was you open your palm silently.#n", ch, NULL, victim, TO_CHAR );
            act( "#y$n #wdashes straight towards #Ryou #was $e opens $s palm silently.#n", ch, NULL, victim, TO_VICT );
            act( "#y$n #wdashes straight towards #R$N #was $e opens $s palm silently.#n", ch, NULL, victim, TO_NOTVICT );
            act( "#wA replica of an #yenergy #Cblade #wforms in your hand as you draw it back...#n", ch, NULL, NULL, TO_CHAR );
            act( "#wA replica of an #yenergy #Cblade #wforms in $n#w's hand as $e draws it back...#n", ch, NULL, NULL, TO_ROOM );
            act( "#yYou #wscream loudly as you drive the energy through #R$N#w's innards!!!#n", ch, NULL, victim, TO_CHAR );
            act( "#y$n #wscreams loudly as $e drives the energy through #Ryour #winnards!!!#n", ch, NULL, victim, TO_VICT );
            act( "#y$n #wscreams loudly as $e drives the energy through #R$N#w's innards!!!#n", ch, NULL, victim, TO_NOTVICT );

            ch->attack_type = ATTACK_PHYSICAL;
            ch->ele_attack = ATTACK_HOLY;
            hurt_person( ch, victim, 500000, TRUE );
         }

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WIZARD) && number_percent() > 70 )
         {
            act("The Earth beneath your feet suddenly jolt high into the sky, forming into a hand of stone.",victim,NULL,NULL,TO_CHAR);                        
            act("The Earth beneath your feet suddenly jolt high into the sky, forming into a hand of stone.",victim,NULL,NULL,TO_ROOM);                        
            act("Pointing your finger at $N, the hand SmAsHeS itself into $S stomach!",ch,NULL,victim,TO_CHAR);     
            act("As $n points $s finger at you, the hand SmAsHeS itself into your stomach!",ch,NULL,victim,TO_VICT);     
            act("As $n points $s finger at $N, the hand SmAsHeS itself into $S stomach!",ch,NULL,victim,TO_NOTVICT);     

            ch->ele_attack = ATTACK_EARTH;
            ch->attack_type = ATTACK_MAGICAL;
            hurt_person(ch,victim,150000,TRUE);

 	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

      if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WEREWOLF) && number_percent() > 90 && !IS_SET(victim->more, MORE_POISON)
           && !IS_CLASS( victim, CLASS_DRAGON ) && !IS_CLASS( victim, CLASS_DROW ) && !IS_CLASS( victim, CLASS_DUERGAR ) )
         {
            act("You slash your claws across $N's chest viciously as your poison mingles with $S blood.",ch,NULL,victim,TO_CHAR);                        
            act("$n slashes $s claws across your chest viciously as $s poison mingles with your blood.",ch,NULL,victim,TO_VICT);                        
            act("$n slashes $s claws across $N's chest viciously as $s poison mingles with $S blood.",ch,NULL,victim,TO_NOTVICT);                        

            SET_BIT(victim->more, MORE_POISON);
            ch->attack_type = ATTACK_PHYSICAL;
            hurt_person( ch, victim, 50000, TRUE );

 	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && ( ch->pcdata->powers[DROW_CLASS] == DROW_MILITIA 
           || ch->pcdata->powers[DROW_CLASS] == DROW_AVATAR ) && ch->pkill > 3 && number_percent() > 50 )
         {
            int dam = 50000;

            act("You suddenly flick your hand causing a small dagger to slide down into it.",ch,NULL,NULL,TO_CHAR);
            act("$n suddenly flicks $s hand causing a small dagger to slide down into it.",ch,NULL,NULL,TO_ROOM);
            act("Before $N gets a chance to move, you RAM the dagger through $S chest!",ch,NULL,victim,TO_CHAR);
            act("Before you get a chance to move, $n RAMS the dagger through your chest!",ch,NULL,victim,TO_VICT);
            act("Before $N gets a chance to move, $n RAMS the dagger through $S chest!",ch,NULL,victim,TO_NOTVICT);

            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = DIRECT;
            ch->attack_var = PIERCE;
            hurt_person(ch,victim,dam,TRUE);

            if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) && ch->gifts[AZI] > 5 && ch->gifts[CURRENT_METER] == AZI
           && number_percent() > 59 - devil_luck( ch, victim, 5 ) )
         {
            int dam = 250000;

            act("You suddenly flick your hand causing a small dagger to slide down into it.",ch,NULL,NULL,TO_CHAR);
            act("$n suddenly flicks $s hand causing a small dagger to slide down into it.",ch,NULL,NULL,TO_ROOM);
            act("Before $N gets a chance to move, you RAM the dagger through $S chest!",ch,NULL,victim,TO_CHAR);
            act("Before you get a chance to move, $n RAMS the dagger through your chest!",ch,NULL,victim,TO_VICT);
            act("Before $N gets a chance to move, $n RAMS the dagger through $S chest!",ch,NULL,victim,TO_NOTVICT);

            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = DIRECT;
            ch->attack_var = PIERCE;
            hurt_person(ch,victim,dam,TRUE);

            if ( ( victim = ch->fighting ) == NULL )
               continue;
         }

      if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON) && number_percent() > 90
           && victim->hit < 2000000 )
         {
             do_flee(ch,"");

             if ( ch->fighting == NULL )
                continue;
         }

      if ( !IS_NPC(ch) && ch->demmak > 0 )
         {
            if ( number_percent() < 50 )
               {
                  stc("#yYou concentrate harder!#n\n\r",ch);
                  act("#y$n concentrates harder!#n",ch,NULL,NULL,TO_ROOM);
                  ch->move -= 50;
               }
               else {
                       char buf[MAX_STRING_LENGTH];
                       int dam = number_range(10000,15000);

                       ch->demmak--;
                       ch->ele_attack = ATTACK_THUNDER;
                       ch->attack_type = ATTACK_PROJECTILE;
                       switch(ch->demmak)
                       {
                          case 2: dam = dam * 2;
                                  sprintf( buf, "You concentrate hard as you release a #ymass#n of #cenergy#n towards $N!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_CHAR);
                                  sprintf( buf, "$n concentrates hard as $e releases a #ymass#n of #cenergy#n towards you!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_VICT);
                                  sprintf( buf, "$n concentrates hard as $e releases a #ymass#n of #cenergy#n towards $N!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_NOTVICT);
                                  stc("You start to focus on the next part of the Dem Mak technique.\n\r",ch);
                                  hurt_person(ch,victim,dam,FALSE);
                                  break;
                          case 1: dam = dam * 5;
                                  sprintf( buf, "Focusing mightily, you release a #yhuge mass#n of #cenergy#n towards $N!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_CHAR);
                                  sprintf( buf, "Focusing mightily, $n releases a #yhuge mass#n of #cenergy#n towards you!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_VICT);
                                  sprintf( buf, "Focusing mightily, $n releases a #yhuge mass#n of #cenergy#n towards $N!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_NOTVICT);
                                  stc("You start to focus on the final part of the Dem Mak technique.\n\r",ch);
                                  hurt_person(ch,victim,dam,FALSE);
                                  break;
                          case 0: dam = dam * 10;
                                  act("You yell '#yOoooo Huden!!!#n'",ch,NULL,NULL,TO_CHAR);
                                  act("$n yells '#yOoooo Huden!!!#n'",ch,NULL,NULL,TO_ROOM);
                                  sprintf( buf, "You push your palms towards $N, releasing a #yMEGA BLAST#n of #cenergy#n towards $N!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_CHAR);
                                  sprintf( buf, "$n pushes $s palms towards you, releasing a #yMEGA BLAST#n of #cenergy#n towards you!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_VICT);
                                  sprintf( buf, "$n pushes $s palms towards $N, releasing a #yMEGA BLAST#n of #cenergy#n towards $N!!! [#r#z%d#n]", dam);
                                  act(buf,ch,NULL,victim,TO_NOTVICT);
                                  hurt_person(ch,victim,dam,TRUE);
                                  break;
                       }
                    }

 	      if ( ( victim = ch->fighting ) == NULL )
	         continue;
         }

	if ( ch == NULL || ch->in_room == NULL )
	    continue;

	/*
	 * Fun for the whole family!
	 */
	for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
	{
	    rch_next = rch->next_in_room;

          if ( victim->attacker != NULL && !IS_NPC( rch ) )
             continue;

	    if ( IS_AWAKE(rch) && rch->fighting == NULL )
	    {
		/*
		 * Mount's auto-assist their riders and vice versa.
		 */
		if ( (mount = rch->mount) != NULL )
               {
                  if ( mount == ch )
                     multi_hit( rch, victim, TYPE_UNDEFINED );

                  continue;
               }

		/*
		 * PC's auto-assist others in their group.
		 */
		if ( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) )
		{
		    if ( ( !IS_NPC(rch) || IS_AFFECTED(rch, AFF_CHARM) )
                   && is_same_group(ch, rch) )
			multi_hit( rch, victim, TYPE_UNDEFINED );
		    continue;
		}

		/*
		 * Players' mobs auto-assist others in their group.
		 */
            if ( !IS_NPC(ch) && IS_NPC(rch) )
               {
                   if ( rch->master == ch && is_same_group(ch, rch) )
                      multi_hit( rch, victim, TYPE_UNDEFINED );

                   continue;
               }

		/*
		 * Team members auto-assist others.
		 */
		if ( !IS_NPC(ch) )
               {
                   if ( !IS_NPC(rch) && rch->pcdata->team == ch->pcdata->team && ch->pcdata->team > 0 )
                      multi_hit( rch, victim, TYPE_UNDEFINED );

                   continue;
               }
	    }
	}
    }

  melee = FALSE;

  if ( arena_wait > 0 )
     arena_wait = UMAX( 0, arena_wait-- );

  if ( arena == FIGHT_BEGINS )
     arena = FIGHT_START;

  if ( arena == FIGHT_ROUNDS )
     arena = FIGHT_BEGINS;

  if ( arena == FIGHT_POSE )
     {
        pose_timer++;

        if ( pose_limit > 0 )
           {
              if ( --pose_limit == 0 )
                 arena = FIGHT_ROUNDS;
           }
     }

  if ( arena == FIGHT_INTRO )
     {
        show_intro();

        if ( arena != FIGHT_CLEAR )
           arena = FIGHT_POSE;
     }

  if ( arena == FIGHT_RECESS )
     arena = FIGHT_INTRO;

  if ( arena == FIGHT_BEGIN )
     arena = FIGHT_START;

  if ( arena == FIGHT_ROUND )
     arena = FIGHT_BEGIN;

  if ( arena == FIGHT_TIMEOVER )
     arena = FIGHT_ANNOUNCE;
}

void do_true_rush( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;

  if ( !IS_NPC(ch) ) return;

  if ( ch->fighting != NULL ) 
     victim = ch->fighting;
     else return;

  act("\n\rA beam of pure energy sHaTTeRs the entire room as $n SmAsHeS iNto YOU!!!\n\r",ch,NULL,victim,TO_VICT);
  act("\n\rA beam of pure energy sHaTTeRs the entire room as $n SmAsHeS iNto $N!!!\n\r",ch,NULL,victim,TO_NOTVICT);

  hurt_person(ch,victim,number_range(50000,100000),TRUE);
}

void do_true_blaster (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int dam = 150000;

  if ( !IS_NPC(ch) ) return;

  if ( ch->fighting != NULL ) 
     victim = ch->fighting;
     else return;

    act("You breathe out a huge gout of abyssal flame and incinerate $N.", ch, NULL, victim, TO_CHAR);
    act("$n breathes out a huge gout of abyssal flame and incinerates you.", ch, NULL, victim, TO_VICT);
    act("$n breathes out a huge gout of flame and incinerates $N.", ch, NULL, victim, TO_NOTVICT);

  ch->ele_attack = ATTACK_FIRE;
  ch->attack_type = ATTACK_PHYSICAL;
  hurt_person(ch,victim,dam,TRUE);
}

void do_d_pounce (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if ( !IS_NPC(ch) ) return;

  if ( ch->fighting != NULL ) 
     victim = ch->fighting;
     else return;

  act("You leap high into the air, pouncing down into $N.", ch, NULL, victim, TO_CHAR);
  act("$n leaps high into the air, pouncing down into you.", ch, NULL, victim, TO_VICT);
  act("$n leaps high into the air, pouncing down into $N.", ch, NULL, victim, TO_NOTVICT);

  hurt_person( ch, victim, number_range( 100000, 150000 ),TRUE);
}

void do_spider_heal ( CHAR_DATA *ch, char *argument ) 
{
    CHAR_DATA *victim;

    victim = ch->mount;

    if ( victim == NULL )
       victim = ch;

    hit_gain( victim, number_range( 40000, 60000 ) );

    send_to_char("Lloth heals you.\n\r",victim);
    act("$n is healed by Lloth.",victim,NULL,NULL,TO_ROOM);
}

void do_hell_hound( CHAR_DATA *ch, char *argument )
{
   int choice;

   if ( number_percent() > 92 )
      {
         do_true_blaster(ch,"");
         return;
      }

   choice = number_range(1,3);

   switch(choice)
   {
      case 2: do_true_rush(ch,""); break;
      case 3:
      case 1: spell_curse( skill_lookup("curse"), 60, ch, ch->fighting ); break;
      default: break;
   }
}

void do_true_flare( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  int dam = 200000;

  if ( !IS_NPC(ch) ) 
     return;

  if ( ch->fighting != NULL ) 
     victim = ch->fighting;
     else return;

  if ( IS_CLASS( victim, CLASS_DRAGON ) )
     dam = 500000;

  act("$n grins evilly as $e waves $s arm in a circular motion and points it at you.",ch,NULL,victim,TO_VICT);
  act("$n grins evilly as $e waves $s arm in a circular motion and points it at $N.",ch,NULL,victim,TO_NOTVICT);

  act("\n\rBalls of pitch-black energy is drawn inside of your body.",ch,NULL,victim,TO_VICT);
  act("\n\rBalls of pitch-black energy is drawn inside of $n's body.",victim,NULL,NULL,TO_ROOM);
  act("You suddenly turn into a shadowly outline as a demonic cackle fills the room.",ch,NULL,victim,TO_VICT);
  act("$n suddenly turns into a shadowly outline as a demonic cackle fills the room.",victim,NULL,NULL,TO_ROOM);
  act("The Demonic Cackles deafens your ears as the balls of Pitch-Black energy rips apart your body.",ch,NULL,victim,TO_VICT);
  act("The Demonic Cackles deafens your ears as the balls of Pitch-Black energy rips apart $n's body.",victim,NULL,NULL,TO_ROOM);
  act("The room is filled with blinding energy as the pitch-black substance ROARS from within you!!!",ch,NULL,victim,TO_VICT);
  act("The room is filled with blinding energy as the pitch-black substance ROARS from within $n!!!",victim,NULL,NULL,TO_ROOM);

  if ( !IS_AFFECTED(victim, AFF_FLAMING) )
     SET_BIT(victim->affected_by, AFF_FLAMING);

  ch->ele_attack = ATTACK_FIRE;
  ch->attack_type = ATTACK_MAGICAL;
  hurt_person( ch, victim, dam, TRUE );
}

void do_spider( CHAR_DATA *ch, char *argument )
{
   int choice;

   if ( number_percent() > 84 )
      {
         do_true_flare(ch,"");
         return;
      }

   choice = number_range(1,3);

   switch(choice)
   {
      default: break;
      case 3: do_spider_heal(ch,""); break;
      case 2: spell_web( skill_lookup("web"), 999, ch, ch->fighting ); break;
      case 1: do_d_pounce(ch,""); break;
   }
}

bool in_arena( CHAR_DATA *ch, CHAR_DATA *victim )
{
   if ( IS_IMMORTAL(victim) )
      return FALSE;

   if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630
        && victim->in_room->vnum >= 550 && victim->in_room->vnum < 630 ) 
      return TRUE;

   if ( victim->in_room->vnum >= 33500 && victim->in_room->vnum < 33800 ) 
      return TRUE;

   return FALSE;
}

/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  OBJ_DATA *wield;
  char buf[MAX_STRING_LENGTH];
  int sn, unarmed, maxcount,countup,i;
  AFFECT_DATA af;

  if ( ch == NULL || victim == NULL || ch->in_room == NULL || victim->in_room == NULL || ch->position <= POS_SLEEPING || victim->position == POS_DEAD || ch->in_room != victim->in_room )
     return;

  if ( IS_SET(ch->more, MORE_FROZEN) || IS_SET(ch->more, MORE_STONE) || ch->blocking > 0 )
     return;

  if ( ch->stunned > 0 )
     return;

  if ( ch->holding != NULL || ch->holded != NULL )
     return;

  if ( !IS_NPC(ch) && ch->pcdata->capture_timer > 0 )
     return;

  if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_KURUDA ) && IS_SET( victim->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
     return;

  if ( IS_CLASS( ch, CLASS_MONK ) && ch->pcdata->powers[MONK_SHIKI] > 0 )
     return;

  if ( IS_NPC(victim) && victim->mount != NULL && IS_SET(victim->mounted,IS_MOUNT) )
     return;

  if ( !is_same_plane(ch,victim) )
     {
        stop_fighting(ch,FALSE);
        return;
     }

  if ( dt == gsn_headbutt )
     {
        one_hit( ch, victim, dt, 1 );
        return;
     }

  if ( dt == gsn_hooves )
     {
        one_hit( ch, victim, dt, 1 );
        return;
     }

  if ( dt == gsn_shiroken )
     {
        for ( i = number_range(0,1); i > 0 ;i-- )
            one_hit(ch, victim, dt, 1); 

        return;
     }

  if ( dt == gsn_claws )
     {
        one_hit( ch, victim, dt, 1 );
        return;
     }

  if ( dt == gsn_fangs )
     {
        one_hit( ch, victim, dt, 1 );
        return;
     } 

  if ( dt == gsn_cheapshot )
     {
        for ( i = number_range(0,1); i > 0 ;i-- )
            one_hit(ch,victim,dt,1);    

        return;
     } 

  if ( dt == gsn_darktendrils && IS_CLASS(ch, CLASS_DROW) )
     {
        for ( i = number_range(0,1); i > 0 ;i-- )
            one_hit(ch,victim,dt,1);

        return;
     }

  if ( dt == gsn_venomtong )
     {
        one_hit(ch,victim,dt,1);
        return;
     }

  if ( dt == gsn_buffet )
     {
        one_hit( ch, victim, dt, 1 );
        return;
     } 

  if ( dt == gsn_sweep )
     {
        one_hit( ch, victim, dt, 1 );
        return;
     }

  if ( dt == gsn_rfangs )
     {
        one_hit( ch, victim, dt, 1);
        return;
     }

   switch( number_range(1,4) )
   {
      case 4: if ( ( wield = get_eq_char( ch, WEAR_FOURTH ) ) != NULL && wield->item_type == ITEM_WEAPON )
                 break;
      case 3: if ( ( wield = get_eq_char( ch, WEAR_THIRD ) ) != NULL && wield->item_type == ITEM_WEAPON )
                 break;
      case 2: if ( ( wield = get_eq_char( ch, WEAR_HOLD ) ) != NULL && wield->item_type == ITEM_WEAPON )
                 break;
      default: wield = get_eq_char( ch, WEAR_WIELD );
               break;
   }

  unarmed = 0;

  one_hit( ch, victim, dt, 1 );

  if ( victim == NULL || victim->position != POS_FIGHTING ) return;

  if ( dt == TYPE_UNDEFINED )
     {
        dt = TYPE_HIT;

        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
           {
              dt += wield->value[3];

              if ( wield->value[0] >= 1 )
                 {
                    /* Look, just don't ask...   KaVir */
                    if ( wield->value[0] >= 1000 )
                       sn = wield->value[0] - ( ( wield->value[0] / 1000 ) * 1000 );
                       else sn = wield->value[0];

                    if ( ( sn != 0 && sn != 71 && sn != 158 && sn != 61 ) && victim->position == POS_FIGHTING )
                    (*skill_table[sn].spell_fun) (sn,wield->level,ch,victim);

                    /* sn.71 is gas breath and sn.158 is desanct and sn.61 is sleep */
                 }
           }
     }

  if ( victim == NULL || victim->in_room == NULL || victim->position != POS_FIGHTING ) return;
  if ( dt == gsn_backstab || dt == gsn_shiroken || dt == gsn_headbutt ) return;

  maxcount = number_attacks(ch, victim);

  if ( dt == gsn_circle )
     maxcount /= 2;

  for ( countup = 0; countup < maxcount; countup++ )
  {
     one_hit( ch, victim, -1, 1 );

     if ( victim == NULL || victim->in_room == NULL || victim->position != POS_FIGHTING )
        {
           show_total_damage( ch, victim );           
           return;
        }
  }

  if ( !IS_NPC(ch) && get_curr_dex(ch) >= get_curr_eva(victim) )
     {
        if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 3 )
           multi_hit(ch, victim, gsn_shiroken);

        if ( IS_CLASS( ch, CLASS_VAMPIRE ) && IS_VAMPAFF(ch,VAM_FANGS) )
           one_hit( ch, victim, (TYPE_HIT + 10), 1 );

        if ( IS_CLASS(ch, CLASS_DRAGON) && IS_DRAGONPOWER(ch, DRAGON_TAIL) && number_range(1,2) == 1 )
           one_hit( ch, victim, gsn_tail, 0);

        if ( IS_SET(ch->newbits, NEW_DARKTENDRILS) && IS_CLASS(ch, CLASS_DROW) )
           multi_hit(ch,victim,gsn_darktendrils);

        if ( IS_CLASS(ch,CLASS_WEREWOLF) )
           multi_hit(ch,victim,gsn_cheapshot); 

        if ( IS_CLASS(ch, CLASS_SAIYAN) )
           {
               one_hit( ch, victim, gsn_punch, 0 );

               if ( number_range(1,2) == 1 )
                  one_hit( ch, victim, gsn_tail, 0);
           }
     }

  if ( victim == NULL || victim->position != POS_FIGHTING
       || ch == NULL || ch->position != POS_FIGHTING )
     {
        show_total_damage( ch, victim );           
        return;
     }

  if ( !IS_NPC(ch) && IS_SET(ch->newbits, NEW_MONKFLAME) && !IS_AFFECTED(victim, AFF_FLAMING) )
     {
        if ( number_percent() > 95 )
           {
              SET_BIT(victim->affected_by, AFF_FLAMING);

              if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DRAGON) )
                 {
                    if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                       act("Your flaming hands catch $n on fire!", ch, NULL, victim, TO_VICT);

                    if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                       act("$N's flaming hands catch you on fire!.", ch, NULL, victim, TO_CHAR);
                 }
                 else {
                         if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                            act("Your flaming claws catch $n on fire!", ch, NULL, victim, TO_VICT);

                         if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                            act("$N's flaming claws catch you on fire!.", ch, NULL, victim, TO_CHAR);
                      }
           }
     }

  show_total_damage( ch, victim );           

  if ( victim->outer_aura == A_IMADIEN || victim->inner_aura == A_IMADIEN || IS_SET(victim->group_aura, G_IMADIEN) )
     {
        one_hit(victim,ch,gsn_deathaura,0);
        one_hit(victim,ch,gsn_deathaura,0);

        if ( ch->position != POS_FIGHTING || victim->in_room == NULL )
           {
              show_total_damage( victim, ch );           
              return;
           }
     }

  if ( victim->outer_aura == A_WEAKEN || victim->inner_aura == A_WEAKEN || IS_SET(victim->group_aura, G_WEAKEN) )
     {
        int level = 0;

        if ( !IS_SET(victim->group_aura, G_WEAKEN) )
           {
              if ( victim->inner_aura == A_WEAKEN )
                 level += level + 1;

              level += victim->aura[A_WEAKEN];
           }
           else level = victim->special_aura[A_WEAKEN];

        if ( number_percent() < level * 10 && !is_affected( ch,skill_lookup("weaken") ) )
           {
               af.type      = skill_lookup("weaken");
               af.duration  = 1;                      
               af.location  = APPLY_STR;
               af.modifier  = get_curr_str( ch ) * -0.1;
               af.bitvector = 0;
               affect_to_char(ch, &af);

               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  act("You cower in the presence of $N.",ch,NULL,victim,TO_CHAR);

               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  act("$n cowers in the presence of you.",ch,NULL,victim,TO_VICT);
           }

        if ( number_percent() >= 50 && !IS_SET( ch->more, MORE_PAIN ) )
           SET_BIT( ch->more, MORE_PAIN );

        if ( number_percent() >= 50 && !IS_SET( ch->more, MORE_PINCH ) )
           SET_BIT( ch->more, MORE_PINCH );
     }

  if ( IS_CLASS( victim, CLASS_WIZARD ) && number_percent() > 70 && IS_SET( victim->mflags, MAGE_TELEKINETICS ) )
     {
        if ( ch->position == POS_FIGHTING )
           if ( ( sn = skill_lookup( "acid blast" ) ) > 0 )
              (*skill_table[sn].spell_fun) (sn,60,victim,ch);
     }

  if ( IS_CLASS( victim, CLASS_DROW ) && number_percent() > 70 )
     {
        if ( ch->position == POS_FIGHTING )
           if ( ( sn = skill_lookup( "chaos blast" ) ) > 0 )
              (*skill_table[sn].spell_fun) (sn,60,victim,ch);
     }

  show_total_damage( victim, ch );           

  if ( IS_CLASS( victim, CLASS_PHOENIX ) && victim->pkill > 1 && ch->position == POS_FIGHTING )
     {
        int dam = 150000;
        
        if ( victim->fighting != ch )
           dam = 500000;

        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
           {
              sprintf( buf, "#RThe #yfiery #Rflames surrounding $n#R's body burns your flesh!#n" );
              act( buf, victim, NULL, ch, TO_VICT );
           }

        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
           {
              sprintf( buf, "#RThe #yfiery #Rflames surrounding your body burns $N#R's flesh!#n" );
              act( buf, victim, NULL, ch, TO_CHAR );
           }

        victim->ele_attack = ATTACK_FIRE;
        victim->attack_type = ATTACK_PHYSICAL;
        victim->attack_var = BLAST;
        victim->attack_focus = DIRECT;
        hurt_person( victim, ch, dam, TRUE );
     }
     else if ( ( is_affected( victim, skill_lookup("fire shield") ) || IS_ITEMAFF(victim, ITEMA_FIREWALL) )
                 && ch->position == POS_FIGHTING && number_percent() > 60 )
     {
        int dam = 100000;

        victim->ele_attack = ATTACK_FIRE;
        victim->attack_type = ATTACK_PHYSICAL;
        victim->attack_var = BLAST;
        victim->attack_focus = DIRECT;

        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
           act("The explosive energy surrounding $n's body burns your flesh!", victim,NULL,ch,TO_VICT);

        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
           act("The explosive energy surrounding your body burns $N's flesh!", victim,NULL,ch,TO_CHAR);

        hurt_person( victim, ch, dam, TRUE );
     }
}

void show_total_damage( CHAR_DATA *ch, CHAR_DATA *victim )
{
   char buf1[512], buf2[512], buf3[512], buf4[512];
   const char *vs;
   const char *vp;
   int dam;
   CHAR_DATA *vch, *vch_next;
   bool rev_dam = FALSE;
   CHAR_DATA *attacker = ( IS_NPC( ch ) && ch->master != NULL ) ? ch->master : ch;

   if ( ch == NULL || ch->in_room == NULL || ch->total_damage == 0 
        || victim == NULL || victim->in_room == NULL )
      {
         ch->total_hits = 0;
         ch->total_damage = 0;      
         return;
      }

   dam = ch->total_damage;

   if ( str_cmp( "", ch->groove ) && str_cmp( "K", ch->groove ) && dam > 0
        && !( IS_NPC( victim ) && ( victim->pIndexData->vnum < 550 
        || victim->pIndexData->vnum > 570 ) ) )
      add_grove( ch, victim, dam );

   if ( !str_cmp( "K", victim->groove ) && dam > 0
        && !( IS_NPC( ch ) && ( ch->pIndexData->vnum < 550 
        || ch->pIndexData->vnum > 570 ) ) )
      add_grove( victim, ch, dam );

   if ( dam < 0 )
      {
         dam = -dam;
         rev_dam = TRUE;
      }

   if ( dam <= 5000 )
      { 
         vs = " like a #csch#yool #bgirl#n";
         vp = " like a #csch#yool #bgirl#n";
      }
      else if ( dam <= 10000 )
              {
                 vs = " like #ra BEAST#n";
                 vp = " like #ra BEAST#n";
              }
      else if ( dam <= 20000 ) 
              {
                 vs = " with #eInCaLuLaTeD #yFoRcE#n";
                 vp = " with #eInCaLuLaTeD #yFoRcE#n";
              }
      else if ( dam <= 30000 )
              {
                 vs = " into #pOblivion#n";
                 vp = " into #pOblivion#n";
              }
      else if ( dam <= 50000 )
              {
                 vs = " with #wNO #yMERCY#n";
                 vp = " with #wNO #yMERCY#n";
              }
      else if ( dam <= 70000 )
              {
                 vs = " #cbeyond reasoning#n";
                 vp = " #cbeyond reasoning#n";
              }  
      else if ( dam <= 80000 )
              {
                 vs = " with #rShAt#eTeRiNg DeAf#rnESs#n";
                 vp = " with #rShAt#eTeRiNg DeAf#rnESs#n";
              }
      else if ( dam <= 90000 )
              {
                 vs = " into the #eDARK #bABYSS#n";
                 vp = " into the #eDARK #bABYSS#n";
              }
      else if ( dam <= 100000 )
              {
                 vs = " #rLIKE A WYRM#n";
                 vp = " #rLIKE A WYRM#n";
              }
      else if ( dam <= 150000 )
              {
                 vs = " #wWiTh ImMoRtAl PaSsIoN#n";
                 vp = " #wWiTh ImMoRtAl PaSsIoN#n";
              }
      else if ( dam <= 200000 )
              {
                 vs = " into #pthe #cnext #wDiMeNsIoN#n";
                 vp = " into #pthe #cnext #wDiMeNsIoN#n";
              }
      else if ( dam <= 300000 )
              {
                 vs = " like A #rrAbId #eWoLvErViNe#n";
                 vp = " like A #rrAbId #eWoLvErViNe#n";
              }
      else if ( dam <= 400000 )
              { 
                 vs = " like a #wtrue #rD#ee#bm#ci#wgod#n";
                 vp = " like a #wtrue #rD#ee#bm#ci#wgod#n";
              }
      else if ( dam <= 500000 )
              {
                 vs = " FoR sOmE #wiNsAnE#n dAmAgE!";
                 vp = " FoR sOmE #wiNsAnE#n dAmAgE!";
              }
      else if ( dam < 750000 )
              {
                 vs = " making them seeing #rST#wA#rRS#n!";
                 vp = " making them seeing #rST#wA#rRS#n!";
              }
      else { 
              vs = " with absolute #yPer#yfec#wtion#n!";
              vp = " with absolute #yPer#wfect#yion#n!";
           }

   if ( ch->total_hits > 1 )
      {
         if ( rev_dam == TRUE )
            {
               sprintf( buf1, "$n heals $v %d times%s! [%d]", ch->total_hits, vp, dam );
               sprintf( buf2, "You heal $N %d times%s! [%d]", ch->total_hits, vp, dam );
               sprintf( buf3, "$n heals you %d times%s! [%d]", ch->total_hits, vp, dam );
               sprintf( buf4, "$N heals $N %d times%s! [%d]", ch->total_hits, vp, dam );
            }
            else {
                    sprintf( buf1, "$n hits $v %d times%s! [%d]", ch->total_hits, vp, dam );
                    sprintf( buf2, "You hit $N %d times%s! [%d]", ch->total_hits, vp, dam );
                    sprintf( buf3, "$n hits you %d times%s! [%d]", ch->total_hits, vp, dam );
                    sprintf( buf4, "$N hits $N %d times%s! [%d]", ch->total_hits, vp, dam );
                 }
      }
      else {
              if ( rev_dam == TRUE )
                 {
                    sprintf( buf1, "$n heals $v one time%s! [%d]", vp, dam );
                    sprintf( buf2, "You heal $N one time%s! [%d]", vp, dam );
                    sprintf( buf3, "$n heals you one time%s! [%d]", vp, dam );
                    sprintf( buf4, "$N heals $N one time%s! [%d]", vp, dam );
                 }
                 else {
                         sprintf( buf1, "$n hits $v one time%s! [%d]", vp, dam );
                         sprintf( buf2, "You hit $N one time%s! [%d]", vp, dam );
                         sprintf( buf3, "$n hits you one time%s! [%d]", vp, dam );
                         sprintf( buf4, "$N hits $N one time%s! [%d]", vp, dam );
                      }
           }

   if ( IS_SET(ch->act, PLR_BRIEF2) )
     act( buf2, ch, NULL, victim, TO_CHAR );

   if ( IS_SET(victim->act, PLR_BRIEF2) )
     act( buf3, ch, NULL, victim, TO_VICT );

   for ( vch = victim->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( victim != vch && IS_SET(vch->act, PLR_BRIEF2) )
         {
            if ( vch->seeself > 0 )
               act( buf4, ch, NULL, vch, TO_VICT );
               else act( buf1, ch, victim, vch, TO_VICT );
         }
   }

   ch->total_hits = 0;
   ch->total_damage = 0;      
}

int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int count = 2;

    if ( !IS_NPC( ch ) && ch->pcdata->blood_power == 3 )
       count += 7;
       else if ( IS_ITEMAFF( ch, ITEMA_MONKCHI ) )
               count += 5;

    if ( ch->topmode == 100 && ch->hit > ch->max_hit * 0.66 )
       count += 3;

    if ( IS_SET( ch->cmbt[7], SKL_CRITICAL ) )
       count += 2;

    if ( !IS_NPC( ch ) && ch->pcdata->deity == DEI_BIRD )
       count += 3;

    if ( is_affected( ch, skill_lookup("prayer") ) )
       count += 2;

    count -= number_range( 0, get_curr_luck( victim ) ) + number_range( 0, get_curr_luck( ch ) / 2 );

    if ( is_affected( ch, skill_lookup("divine strength") ) )
       count += 2;

    if ( IS_NPC( ch ) )
       {
          if ( IS_SET( ch->extra, EXTRA_ZOMBIE ) )
             {
                if ( ch->pIndexData->vnum == 23 )
                   count += 11;
                   else count += 7;
             }
             else if ( ( ch->pIndexData->vnum >= 558 && ch->pIndexData->vnum < 561 )
                       || ch->pIndexData->vnum == 567 || ch->pIndexData->vnum == 89 )
                     return 10;
             else if ( ch->pIndexData->vnum == 999 )
                     count = ch->hitroll;
                     else count += 6;
       }
       else count += ( get_curr_dex( ch ) / 5 );

    if ( IS_ITEMAFF( victim, ITEMA_PROTECTION ) )
       count -= 5;
       else if ( is_affected( victim, skill_lookup("blessed soul") ) && is_affected( victim, skill_lookup("aided soul") ) )
               count -= number_range( 0, 5 );
       else if ( is_affected( victim, skill_lookup("blessed soul") ) )
               count -= number_range( 0, 3 );

    if ( IS_SET( victim->cmbt[7], SKL_ANTICIPATE ) )
       count -= 4;

    if ( is_affected( ch, skill_lookup("haste") ) )
       count *= 1.25;

    if ( is_affected( ch, skill_lookup("last prayer") ) )
       count /= 2;
       else if ( is_affected( ch, skill_lookup("slow") ) )
               count *= 0.75;
       else if ( IS_SET( ch->more2, MORE2_NOARMS ) )
               count *= 0.85;
       else if ( IS_SET( ch->more, MORE_PINCH ) )
               count *= 0.9;

    if ( IS_CLASS( victim, CLASS_SNOW_ELF ) && victim->pkill >= 8 )
       {
          if ( IS_SET( victim->more, MORE_FROZEN ) )
	       count = 0;
             else count -= UMAX( 4, count * 0.25 );
       }

    if ( count < 1 )
       count = 1;

    return UMIN( count, 14 );
}


/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype )
{
  OBJ_DATA *wield;
  int wdam, level;

  /*
   * Can't beat a dead char!
   * Guard against weird room-leavings.
   */
  if ( ch->position <= POS_SLEEPING || victim->position == POS_DEAD || ch->in_room != victim->in_room
       || ch->blocking > 0 )
     return;

  if ( IS_CLASS( ch, CLASS_MONK ) && ch->pcdata->powers[MONK_SHIKI] > 0 )
     return;

  if ( IS_NPC(victim) && victim->mount != NULL && IS_SET(victim->mounted,IS_MOUNT) )
     stop_fighting(victim,TRUE);

  if ( IS_NPC(ch) && ch->mount != NULL && ch->mount->position < POS_FIGHTING && IS_SET(ch->mounted,IS_MOUNT) )
     stop_fighting(ch,TRUE);

  if ( IS_NPC(ch) && ( ch->pIndexData->vnum == 20 || ch->pIndexData->vnum == 21 ) )
     return;

  if ( IS_SET(ch->more, MORE_FROZEN) || IS_SET(ch->more, MORE_STONE) )
     return;

  if ( ch->stunned > 0 )
     return;

  if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_KURUDA ) && IS_SET( victim->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
     return;

  if ( ch->holding != NULL || ch->holded != NULL )
     return;

  if ( ch->holding != NULL || ch->holded != NULL )
     return;

  if ( IS_SET(ch->more, MORE_CONFUSION) && number_percent() > 50 )
     {
        if ( IS_CLASS( ch, CLASS_DROW ) )
           {
              act("#PYou #ecackle with glee as #PLady Lloth #egives you #wstrength #eadmist the chaos.#n", ch, NULL, NULL, TO_CHAR );
              act("#P$n #ecackles with glee as #P$e #elooks much #wstronger #eadmist the chaos.#n", ch, NULL, NULL, TO_ROOM );
              REMOVE_BIT( ch->more, MORE_CONFUSION );
              ch->hit = UMIN( ch->max_hit, ch->hit + 1000000 );
           }
           else if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                   {
                      stc("In your confusion you swing wildly hitting nothing but air.\n\r",ch);
                      return;
                   }
     }

  switch( number_range(1,4) )
  {
     case 4: if ( ( wield = get_eq_char( ch, WEAR_FOURTH ) ) != NULL && wield->item_type == ITEM_WEAPON )
                break;
     case 3: if ( ( wield = get_eq_char( ch, WEAR_THIRD ) ) != NULL && wield->item_type == ITEM_WEAPON )
                break;
     case 2: if ( ( wield = get_eq_char( ch, WEAR_HOLD ) ) != NULL && wield->item_type == ITEM_WEAPON )
                break;
     default: wield = get_eq_char( ch, WEAR_WIELD );
              break;
  }

  if ( dt == TYPE_UNDEFINED )
     {
        dt = TYPE_HIT;

        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
           dt += wield->value[3];
     }

  if ( !IS_NPC( ch ) )
     {
       if ( wield != NULL && wield->item_type == ITEM_WEAPON )
          {
             if ( IS_WEAP( wield, TRUE_FIRE ) )
                ch->ele_attack = ATTACK_FIRE;
                else if ( IS_WEAP( wield, TRUE_SAINT ) )
                        ch->ele_attack = ATTACK_HOLY;
                else if ( IS_WEAP( wield, TRUE_ICE ) )
                        ch->ele_attack = ATTACK_ICE;
                else if ( IS_WEAP( wield, TRUE_EARTH ) )
                        ch->ele_attack = ATTACK_EARTH;
                else if ( IS_WEAP( wield, TRUE_DARK ) )
                        ch->ele_attack = ATTACK_DARK;
                else if ( IS_WEAP( wield, TRUE_BOLT ) )
                        ch->ele_attack = ATTACK_THUNDER;

             ch->attack_type = ATTACK_PHYSICAL;
          }
     }

  if ( !IS_SET( victim->more, MORE_FROZEN ) && !IS_SET(victim->more, MORE_STONE) && victim->stunned == 0 && victim->position > POS_STUNNED &&
       !( ch->outer_aura == A_AIM || ch->inner_aura == A_AIM || IS_SET( ch->group_aura, G_AIM ) ) )
  { 
      if ( IS_CLASS( victim, CLASS_DEMON ) && IS_SET( victim->more3, MORE3_SNAKE ) 
           && victim->pkill > 15 && number_percent() > 75 - devil_luck( victim, ch, 10 ) )
         {
            if ( ( IS_CLASS( ch, CLASS_DROW ) && IS_SET( ch->pcdata->powers[1], DPOWER_DROWPOISON ) )
                 || IS_CLASS( ch, CLASS_DRAGON ) || is_affected( ch, skill_lookup("drow poison") )
                 || IS_CLASS( ch, CLASS_DUERGAR ) || number_percent() < 89 - devil_luck( victim, ch, 10 ) )
               ;
               else spell_dpoison( skill_lookup( "drow poison" ), 60, victim, ch );

            one_hit( victim, ch, gsn_venom_strike, 1 );
            return;
         }

      if ( IS_CLASS( victim, CLASS_DUERGAR ) || is_affected( victim, skill_lookup("earth wall") )
           || ( !IS_NPC( victim ) && victim->pcdata->deity == DEI_TITAN ) )
      {
         int ddodge = number_range(1,50);
 
         if ( ddodge == 10 || ddodge == 50 || ddodge == 20 || ddodge == 30 || ddodge == 40 )
            {
               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  stc("#gThe Earth around you absorbs your damage#n.\n\r",victim);

               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  stc("#gThe Earth around your opponent absorbs your damage#n.\n\r",ch);
               return;
            }
      }
      else if ( IS_CLASS( victim, CLASS_ANGEL ) )
      {
         int ddodge = number_range(1,50);

         if ( ddodge == 50 || ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 )
            {
               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  stc("#wYour tendrils absorbs your damage#n.\n\r",victim);

               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  stc("#wYour opponent's tendrils absorbs your damage#n.\n\r",ch);
               return;
            }
      }
      else if ( IS_CLASS( victim, CLASS_DROW ) )
      {
         int ddodge = number_range(1,50);

         if ( ddodge == 50 || ddodge == 20 || ddodge == 30 || ddodge == 40 )
            {
               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  stc("#eYour tendrils absorbs your damage#n.\n\r",victim);

               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  stc("#eYour opponent's tendrils absorbs your damage#n.\n\r",ch);
               return;
            }
      }
      else if ( IS_CLASS( victim, CLASS_NINJA ) && victim->pkill > 19 )
      {
         int ddodge = number_range(1,50);
 
         if ( ddodge == 20 || ddodge == 10 || ddodge == 30 || ddodge == 40 || ddodge == 50 )
            {
               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  stc("#eYou vanish into the ground as your shadow circles around your attacker's blow#n.\n\r",victim);


               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  stc("#eYour opponent's vanishes into the ground as their shadow circles around your blow#n.\n\r",ch);

               return;
            }
      }
      else if ( IS_SET( victim->cmbt[7], SKL_SWERVE ) )
      {
         int ddodge = number_range(1,50);
 
         if ( ddodge == 20 || ddodge == 10 || ddodge == 30 || ddodge == 40 || ddodge == 50 )
            {
               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  stc("#cYou suddenly push your attacker's attack to the side with your hands#n.\n\r",victim);

               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  stc("#cYour opponent suddenly push your attack to the side with their hands#n.\n\r",ch);

               return;
            }
      }

   if ( is_affected( victim, skill_lookup("blur") ) )
      {
         int ddodge = number_range( 1, 50 );

         if ( ddodge == 1 || ddodge == 50 || ddodge == 20 || ddodge == 30 || ddodge == 40
              || ddodge == 5 || ddodge == 45 || ddodge == 25 || ddodge == 35 || ddodge == 15
              || ddodge == 3 || ddodge == 43 || ddodge == 23 || ddodge == 33 || ddodge == 13
              || ddodge == 7 || ddodge == 47 || ddodge == 27 || ddodge == 37 || ddodge == 17 )
            {
               if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                  act("Your body shimmers as you fade around $N's attack.", victim, NULL, ch, TO_CHAR );

               if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                  act("$n's body shimmers as $e fades around your attack.", victim, NULL, ch, TO_VICT );

               return;
            }
      }

   if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_DRAGON_KNIGHT ) )
      {
         OBJ_DATA *weapon;

         if ( ( ( weapon = get_eq_char( victim, WEAR_WIELD ) ) != NULL ) && weapon->value[3] == 2 )
            {
               int ddodge = number_range( 1, 50 );

               if ( ddodge == 1 || ddodge == 50 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 5 )
                  {
                     if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                        act("You savagely poke your spear into $N's chest keeping $M at bay.", victim, NULL, ch, TO_CHAR );

                     if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                        act("$n savagely pokes $s spear into your chest keeping you at bay.", victim, NULL, ch, TO_VICT );

                     return;
                  }
            }
      }

   if ( victim->outer_aura == A_BSHIELD || victim->inner_aura == A_BSHIELD || IS_SET(victim->group_aura, G_BSHIELD) )
      {
         int level = 0;

         if ( !IS_SET(victim->group_aura, G_BSHIELD) )
            {
               if ( victim->inner_aura == A_BSHIELD )
                  level += level + 1;

               level += victim->aura[A_BSHIELD];
            }
            else level = victim->special_aura[A_BSHIELD];
 
         if ( number_percent() < level * 10 )
            {
	         int ddodge = number_range(1,50);
	           
               if ( ddodge == 1 || ddodge == 50 || ddodge == 20 || ddodge == 30 || ddodge == 40
                    || ddodge == 5 || ddodge == 45 || ddodge == 25 || ddodge == 35 || ddodge == 15 )
                  {
                        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
	                   stc("#rYour dark aura absorbs your damage.#n\n\r",victim);

                        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
  	                   stc("#rYour opponent's dark aura absorbs your damage#n.\n\r",ch);

                      return;
                  }
            }
      }
  }

  if ( number_range(1,10) == 5 )
     {
        if ( IS_CLASS( ch, CLASS_ANGEL ) )
           {
              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 {
                    act("$n's body glow with a heavenly aura.",ch,NULL,victim,TO_VICT);
                    act("$n's aura sears your flesh!",ch,NULL,victim,TO_VICT);
                 }

              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("Your heavenly aura sears $N's flesh.",ch,NULL,victim,TO_CHAR);

              victim->hit -= is_elemental( ATTACK_HOLY, victim, 20000 );
           }
     }

  if ( !IS_NPC( ch ) && ch->pcdata->kingdom == 3 && ch->pcdata->clanrank == 6
       && number_range(1,10) == 5 )
     {
       if ( ch->fighting == victim )
          {
              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 act("#y$n#e's blade draws energy from your soul as it slices through #wyou#e.#n",ch,NULL,victim,TO_VICT);

              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("#yYour #eblade draws energy from #w$n#e's soul as it slices through $m.#n",ch,NULL,victim,TO_CHAR);

              hit_gain( ch, 25000 );
              victim->hit -= is_elemental( ATTACK_DARK, victim, 50000 );
         }
     }

  if ( !IS_NPC(ch) && IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && wield != NULL && wield->value[3] == 3
       && number_range(1,10) == 5 )
     {
        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
           act("#CYou #ysneer as the #wwind #yfrom your slash #PStRiKeS #y$N #yviciously!#n",ch,NULL,victim,TO_CHAR);

        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
           act("#C$n #ysneers as the #wwind #yfrom $s slash #PStRiKeS #yyou viciously!#n",ch,NULL,victim,TO_VICT);

        victim->hit -= is_elemental( ATTACK_THUNDER, victim, 25000 );
     }

  if ( number_range(1,10) == 5 )
     {
        if ( IS_CLASS( ch, CLASS_VAMPIRE ) )
           {
              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 act("$n sinks $s teeth into your flesh sucking your health away.",ch,NULL,victim,TO_VICT);

              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("You sink your teeth into $N's flesh sucking $S health away.",ch,NULL,victim,TO_CHAR);

              victim->hit -= 10000;
              hit_gain( ch, 20000 );
           }
     }

  if ( IS_CLASS( ch, CLASS_DROW ) && wield != NULL && number_range(1,10) == 5
       && IS_WEAP(wield,WEAPON_NPOISON) && !IS_CLASS( victim, CLASS_DRAGON )
       && !IS_CLASS( victim, CLASS_DROW ) )
     {
       if ( ch->fighting == victim )
          {
              wdam = number_range(10000,25000) + UMIN( 25000, ( ch->pkill * 1000 ) );

              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("You scream as the poison from $N's weapon enters your body.",victim,NULL,ch,TO_CHAR);

              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 act("$n screams as the poison from your weapon enters $s body.",victim,NULL,ch,TO_VICT);

              if ( !IS_SET(victim->more, MORE_POISON) )
                 SET_BIT(victim->more, MORE_POISON);

              if ( !IS_CLASS( victim, CLASS_DUERGAR ) )
                 victim->hit -= wdam;
                 else victim->hit = UMIN( victim->max_hit, victim->hit + ( wdam * UMAX( 1, victim->pkill / 10 ) ) );
         }
     }

  if ( wield != NULL && number_range(1,10) == 5 && IS_ITEMAFF(ch, ITEMA_PLASMA) )
     {
       if ( ch->fighting == victim )
          {
              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 act("$n's body gets #PStRuCk#n by #ysparks#n of #wenergy#n as your weapon strikes $m!",victim,NULL,ch,TO_VICT);

              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("Your body get #PStRuCk#n by #ysparks#n of #wenergy#n as $N's weapon strikes you.",victim,wield,ch,TO_CHAR);

              if ( number_percent() > 74 && ch->plasma_timer == 0 && !is_martial( victim ) )
                 {
                    ch->plasma_timer = 20;
                    victim->wait = 24;
                 }

              victim->hit -= is_elemental( ATTACK_THUNDER, victim, 50000 );
         }
     }

  if ( is_affected( ch, skill_lookup("flame blade") ) && number_range(1,10) == 5 )
     {
        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
           act("#yYou #Rpoint your finger at $N #Ras the #yfiery #Cblade #Raround you #wsears #R$M.#n",ch,NULL,victim,TO_CHAR);

        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
           act("#y$n #Rpoints $s finger at you as the #yfiery #Cblade #Raround $m #wsears #Ryou.#n",ch,NULL,victim,TO_VICT);

        victim->hit -= is_elemental( ATTACK_FIRE, victim, 30000 );
     }

  if ( !IS_NPC(ch) && IS_CLASS( ch, CLASS_VAMPIRE ) && number_range(1,10) == 5 
       && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_CHILL )
     {
        int damage = 30000;

        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
           act("$n #Lshivers#n as your #LChill#PReaver #LSliVeRs#n through $s chest!",victim,NULL,ch,TO_VICT);

        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
           act("You #Lshiver#n as $N's #LChill#PReaver #LSliVeRs#n through your chest.",victim,wield,ch,TO_CHAR);

        if ( ch->ctimer[5] == 0 && number_percent() > 70 && !IS_SET( victim->more, MORE_FROZEN ) && !is_martial( victim ) )
           {
              ch->ctimer[5] = 3;
              SET_BIT( victim->more, MORE_FROZEN );
              victim->freeze_timer = 1;
           }
 
        victim->hit -= is_elemental( ATTACK_ICE, victim, damage );
     }

  if ( wield != NULL && number_range(1,10) == 5 && IS_WEAP( wield, WEAPON_EVOCATE ) )
     {
        int damage = 50000;

        if ( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 1 || ch->clannum == 6 ) )
           damage = 125000;

        act("$n's body shivers as your weapon sears $s flesh.",victim,NULL,ch,TO_VICT);
        act("$n's body shivers as $N's weapon sears $s flesh.",victim,NULL,ch,TO_NOTVICT);
        act("Your body shivers as $N's weapon sears your flesh.",victim,NULL,ch,TO_CHAR);
        victim->hit -= damage;
        ch->mana = UMIN( ch->max_mana, ch->mana + 100000 );
        ch->move = UMIN( ch->max_move, ch->move + 100000 );
     }

  if ( !IS_NPC( ch ) && wield != NULL && number_range( 1, 10 ) == 5 && IS_WEAP( wield, TRUE_FIRE )
       && IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_SOR ) )
     {
        act("You focus on your blade as it explodes in a manifest of flames, searing $N's flesh.",ch,NULL,victim,TO_CHAR);
        act("$n focuses on $s blade as it explodes in a manifest of flames, searing your flesh.",ch,NULL,victim,TO_VICT);
        act("$n focuses on $s blade as it explodes in a manifest of flames, searing $N's flesh.",ch,NULL,victim,TO_NOTVICT);
        victim->hit -= is_elemental( ATTACK_FIRE, victim, 30000 );
     }

  if ( wield != NULL && number_range(1,10) == 5 && ( IS_CLASS( ch, CLASS_DEMON ) 
       || IS_CLASS( ch, CLASS_ASSASSIN ) || IS_CLASS( ch, CLASS_ANGEL )
       || IS_SET( ch->cmbt[7], SKL_ENCASE ) ) && !IS_SET( victim->cmbt[7], SKL_ASHIELD ) )
     {
       if ( ch->fighting == victim && IS_WEAP(wield,TRUE_FIRE) )
	    {
              if ( !IS_NPC(ch) )
                 {
                    if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                       act("$n screams as your weapon SeArS through $s chest!",victim,NULL,ch,TO_VICT);

                    if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                       act("You scream as $N's weapon SeArS through your chest.",victim,wield,ch,TO_CHAR);

                    victim->hit -= is_elemental( ATTACK_FIRE, victim, 50000 );
                 }
          }
          else if ( ch->fighting == victim && IS_WEAP(wield,TRUE_ICE) )
                  {
                     if ( !IS_NPC(ch) )
                        {
                           if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                              act("$n's body stiffens as your weapon FrEeZeS $s cells!",victim,NULL,ch,TO_VICT);

                           if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                              act("Your body stiffen as $N's weapon FrEeZeS your cells.",victim,wield,ch,TO_CHAR);

                           victim->hit -= is_elemental( ATTACK_ICE, victim, 50000 );
                        }
                  }
          else if ( ch->fighting == victim && IS_WEAP(wield,TRUE_EARTH) )
                  {
                     if ( !IS_NPC(ch) )
                        {
                           if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                              act("$n's body gets CrUsHeD by the EaRtH as your weapon thrusts through $m!",victim,NULL,ch,TO_VICT);

                           if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                              act("Your body get CrUsHeD by the EaRtH as $N's weapon thrusts through you.",victim,wield,ch,TO_CHAR);

                           victim->hit -= is_elemental( ATTACK_EARTH, victim, 50000 );
                        }
                 }
         else if ( ch->fighting == victim && IS_WEAP(wield,TRUE_DARK) )
                 {
                   if ( !IS_NPC(ch) )
                      {
                         if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                            act("$n screams as your weapon scorches $s body with dark energy!",victim,NULL,ch,TO_VICT);

                         if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                            act("You scream as $N's weapon scorches your body with dark energy!",victim,NULL,ch,TO_CHAR);

                         victim->hit -= is_elemental( ATTACK_DARK, victim, 50000 );
                      }
                 }
         else if ( ch->fighting == victim && IS_WEAP(wield,TRUE_SAINT) )
                 {
                   if ( !IS_NPC(ch) )
                      {
                         if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                            act("$n screams as your weapon bathes $s body with holy energy!",victim,NULL,ch,TO_VICT);

                         if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                            act("You scream as $N's weapon bathes your body with holy energy!",victim,NULL,ch,TO_CHAR);

                         victim->hit -= is_elemental( ATTACK_HOLY, victim, 50000 );
                      }
                 }
         else if ( ch->fighting == victim && IS_WEAP(wield,TRUE_MOON) )
                 {
                   if ( !IS_NPC(ch) )
                      {
                         if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                            act("$n becomes paler as your weapon drains energy from $s body!",victim,NULL,ch,TO_VICT);

                         if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                            act("You become paler as $N's weapon drains energy from your body!",victim,NULL,ch,TO_CHAR);

                         ch->hit = UMIN( ch->max_hit, ch->hit + 25000 );
                         victim->hit -= is_elemental( ATTACK_LUNAR, victim, 30000 );
                      }
                 }
         else if ( ch->fighting == victim && IS_WEAP(wield,TRUE_BOLT) )
                 {
                   if ( !IS_NPC(ch) )
                      {
                         if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                            act("$n's body gets StRuCk by MEGA lightning as your weapon strikes $m!",victim,NULL,ch,TO_VICT);

                         if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                            act("Your body get StRuCk by MEGA lightning $N's weapon strikes you.",victim,wield,ch,TO_CHAR);

                         victim->hit -= is_elemental( ATTACK_THUNDER, victim, 50000 );
                      }
                }
     }

  damage( ch, victim, 0, dt );

  tail_chain( );
}

int randomize_damage( CHAR_DATA *ch, int dam, int am )
{
  dam = dam * (am + 50) / 100;
  return dam;
}

int cap_dam( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int dam = char_damcap( ch );

  if ( !IS_NPC( ch ) )
     {
        if ( dam < 50 )
           dam *= 800;
           else {
                   dam -= 49;
                   dam = 40000 + ( dam * 200 );
                }
     }
     else dam *= 1000;

  if ( IS_SET( ch->cmbt[7], SKL_CRITICAL ) && number_percent() > 89
       && !IS_SET( victim->cmbt[7], SKL_ANTICIPATE ) )
     dam *= 1.25;
     else if ( is_affected( ch, skill_lookup("shillelagh") ) )
             dam *= 1.15;
     else if ( ch->topmode == 100 && ch->hit > ch->max_hit * 0.66 )
             dam *= 1.1;

  if ( !IS_NPC( ch ) && ch->pcdata->deity == DEI_IFRIT )
     dam *= 1.15;

  if ( !IS_NPC( victim ) && victim->pcdata->deity == DEI_TITAN )
     dam *= 0.9;

  if ( victim->noprotection > 0 )
     dam *= 1.5;

  if ( victim->blocking > 0 )
     dam *= 0.2;

  if ( !IS_NPC( victim ) && victim->pcdata->blood_power == 4 )
     dam /= 3;

  return dam;
}

int char_damcap( CHAR_DATA *ch )
{
  int str = get_curr_str( ch );
  int bonus = 0;

  if ( IS_CLASS( ch, CLASS_NECROMANCER ) )
     return 1;

  if ( IS_CLASS(ch, CLASS_ANGEL) && str < 25 && ch->angelab[COMBAT] > 3 )
     str = 25;

  if ( ch->fighting != NULL )
     {
        CHAR_DATA *victim = ch->fighting;
        int con;

        con = get_curr_con( victim );

        if ( con > str )
           str -= 10 - UMIN( 25, con - str );

        if ( IS_CLASS( victim, CLASS_SHADOW ) && victim->gifts[ALI] > 3 && victim->gifts[CURRENT_METER] == ALI )
           str *= 0.9;
     }

  if ( IS_NPC( ch ) )
     {
        if ( IS_SET( ch->extra, EXTRA_ZOMBIE ) )
           {
              if ( ch->pIndexData->vnum == 23 )
                 str = 50;
                 else str = 15;
           }
           else if ( ch->pIndexData->vnum >= 558 && ch->pIndexData->vnum < 561 )
                   str = 90;
           else if ( ch->pIndexData->vnum == 567 )
                   str = 65;
           else if ( ch->pIndexData->vnum == 89 )
                   str = 35;
           else if ( ch->pIndexData->vnum == 1 )
                   str = ch->damroll;
                   else str = 25;
     }

  if ( IS_ITEMAFF( ch, ITEMA_WEREWOLF ) )
     str += 25;
     else if ( !IS_NPC( ch ) && ch->pcdata->blood_power == 1 )
             str += 15;
     else if ( ch->outer_aura == A_MIGHT || ch->inner_aura == A_MIGHT || IS_SET( ch->group_aura, G_MIGHT ) )
             str += 10;
     else if ( ch->outer_aura == A_STRENGTH || ch->inner_aura == A_STRENGTH || IS_SET(ch->group_aura, G_STRENGTH) )
             str += 5;

  if ( is_affected( ch, skill_lookup("vision destroyer") ) )
     str -= 12;
     else if ( IS_SET( ch->more, MORE_DISEMPOWER ) )
             str -= 7;
     else if ( IS_SET( ch->garou1, GAROU_PAWS ) )
             str -= 5;

  return str;
}

int check_immune(CHAR_DATA *ch, int dam_type)
{
  int immune, def;
  int bit;

  immune = -1;
  def = IS_NORMAL;

  if (dam_type == DAM_NONE) return immune;

  if (ch->class == CLASS_VAMPIRE)
  {
    if (dam_type == DAM_COLD) immune = IS_RESISTANT;
    if (dam_type == DAM_DROWNING) immune = IS_VULNERABLE;
    if (dam_type == DAM_FIRE) immune = IS_VULNERABLE;
  }

  if (ch->power[DISC_VAMP_PROT] >= 7 && dam_type == DAM_FIRE) immune = IS_RESISTANT;
  if (dam_type == DAM_POISON && ch->power[DISC_DAEM_CORR] >= 3) immune = IS_DIMMUNE;
  if (dam_type == DAM_POISON && ch->power[DISC_DAEM_CORR] >= 1) immune = IS_RESISTANT;

  if (dam_type == DAM_FIRE && ch->power[DISC_DAEM_HELL] >= 2) immune = IS_DIMMUNE;
  if (dam_type == DAM_FIRE && ch->power[DISC_DAEM_HELL] >= 1) immune = IS_RESISTANT;

  if (dam_type == DAM_COLD && ch->power[DISC_DAEM_GELU] >= 4) immune = IS_DIMMUNE;
  if (dam_type == DAM_COLD && ch->power[DISC_DAEM_GELU] >= 1) immune = IS_RESISTANT;

  if (dam_type == DAM_POISON && ch->power[DISC_VAMP_FORT] >= 1) immune = IS_RESISTANT;

  if (immune != -1) return immune;

    
 /* set bits to check -- VULN etc. must ALL be the same or this will fail */
  switch (dam_type)
  {
  case(DAM_BASH):         bit = DIMM_BASH;         break;
  case(DAM_PIERCE):       bit = DIMM_PIERCE;       break;
  case(DAM_SLASH):        bit = DIMM_SLASH;        break;
  case(DAM_FIRE):         bit = DIMM_FIRE;         break;
  case(DAM_COLD):         bit = DIMM_COLD;         break;
  case(DAM_LIGHTNING):    bit = DIMM_LIGHTNING;    break;
  case(DAM_ACID):         bit = DIMM_ACID;         break;
  case(DAM_POISON):       bit = DIMM_POISON;       break;
  case(DAM_NEGATIVE):     bit = DIMM_NEGATIVE;     break;
  case(DAM_HOLY):         bit = DIMM_HOLY;         break;
  case(DAM_ENERGY):       bit = DIMM_ENERGY;       break;
  case(DAM_MENTAL):       bit = DIMM_MENTAL;       break;
  case(DAM_DISEASE):      bit = DIMM_DISEASE;      break;
  case(DAM_DROWNING):     bit = DIMM_DROWNING;     break;
  case(DAM_LIGHT):        bit = DIMM_LIGHT;        break;
  case(DAM_CHARM):        bit = DIMM_CHARM;        break;
  case(DAM_SOUND):        bit = DIMM_SOUND;        break;
  default:                return def;
  }

  if (immune == -1) return def;
  else return immune;
}


/*
 * Inflict damage from a hit.
 */
void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
  CHAR_DATA *emb;
  bool spell = FALSE, block = TRUE;

  if ( victim->position == POS_DEAD || victim->in_room == NULL || ch->in_room == NULL )
     return;

  if ( ch->embracing != NULL )
  {
    if ( ( emb = ch->embracing ) != NULL ) stop_embrace(ch,emb);
    else stop_embrace(ch,NULL);
  }

  if ( dt < -1 )
     {
        dt += 2000;
        spell = TRUE;
 
        if ( IS_SET(victim->extra, EXTRA_ZOMBIE) ) 
           dam = 0;

        if ( IS_CLASS(victim, CLASS_DEMON) )
           {
              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("Your spell harmlessly bounces off $N's skin.",ch,NULL,victim,TO_CHAR);

              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 act("$n's spell harmlessly bounces off your skin.",ch,NULL,victim,TO_VICT);

              return;
           }
     }

  if ( dam != -123456 )
     dam = cap_dam( ch, victim );
     else dam = 50000;

  /*
   * Certain attacks are forbidden.
   * Most other attacks are returned.
   */
  if ( victim != ch )
  {
    if (is_safe( ch, victim )) return;
    check_killer( ch, victim );

    if ( ch->fighting == NULL )
       {
          if ( ch->fighting == NULL )
             {
                if ( ch->fight_timer == 0 && !IS_NPC( ch ) && !IS_NPC( victim ) && IS_SET( ch->more3, MORE3_FALCON ) )
                   {
                      dam += 500000;
                      block = FALSE;
                   }

                set_fighting( ch, victim );
             }
       }

    if ( victim->position > POS_STUNNED )
       {
          if ( victim->fighting == NULL )
             set_fighting( victim, ch );
       }

    if ( victim->master == ch ) stop_follower( victim );

    if ( IS_SET( ch->garou1, GAROU_BLISSFUL ) )
       REMOVE_BIT( ch->garou1, GAROU_BLISSFUL );

    if ( IS_SET( ch->more2, MORE2_DIG ) )
       REMOVE_BIT( ch->more2, MORE2_DIG );

    if ( IS_SET( ch->act, PLR_WIZINVIS ) && !( IS_NPC( ch ) && IS_CLASS( ch, CLASS_DROW ) 
         && ( ch->pcdata->powers[DROW_CLASS] == DROW_MILITIA || ch->pcdata->powers[DROW_CLASS] == DROW_AVATAR ) ) )
	 {
	    REMOVE_BIT( ch->act, PLR_WIZINVIS );
          send_to_char("You leap from the shadows.\n\r", ch);
	    act( "$n leaps from the shadows.", ch, NULL, NULL, TO_ROOM );
	 }

    if ( IS_AFFECTED(ch, AFF_HIDE) )
       REMOVE_BIT( ch->affected_by, AFF_HIDE );

    if ( spell && IS_CLASS( victim, CLASS_ANGEL ) && number_percent() > 20 )
       {
          if ( IS_NPC(victim) || !( IS_SET(victim->act, PLR_BRIEF) || IS_SET( victim->act, PLR_BRIEF2 ) ) )
             act("Your shield turns blue and reflects back the attack.",victim,NULL,ch,TO_CHAR);

          if ( IS_NPC(ch) || !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
             act("$n's holy shield turns blue and reflects back the attack.",victim,NULL,ch,TO_VICT);

          if ( IS_CLASS( ch, CLASS_DEMON ) )
             {
                if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                   act("Your spell harmlessly bounces off your skin.",ch,NULL,NULL,TO_CHAR);

                if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                   act("$n's spell harmlessly bounces off $s skin.",ch,NULL,NULL,TO_ROOM);

                return;
             }

          victim = ch;
       }
       else if ( spell && is_affected( victim,skill_lookup("divine reflection") ) && victim != ch )
               {
                  if ( IS_NPC(victim) || !( IS_SET(victim->act, PLR_BRIEF) || IS_SET( victim->act, PLR_BRIEF2 ) ) )
                     act("Your shield turns white and reflects back the attack.",victim,NULL,ch,TO_CHAR);

                  if ( IS_NPC(ch) || !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
                     act("$n's shield turns white and reflects back the attack.",victim,NULL,ch,TO_VICT);

                  if ( IS_CLASS( ch, CLASS_DEMON ) )
                     {
                        if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                           act("Your spell harmlessly bounces off your skin.",ch,NULL,NULL,TO_CHAR);

                        if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                           act("$n's spell harmlessly bounces off $s skin.",ch,NULL,NULL,TO_ROOM);

                        return;
                     }

                  victim = ch;
               }

    if ( dam < 0 ) dam = 0;

    if ( block )
       {
          if ( dt != -1 && check_illusionblur( ch, victim ) )
             return;

          if ( dt != -1 && check_dodge2( ch, victim, dt ) )
             return;

          if ( dt != -1 && check_parry( ch, victim, dt ) )
	       return;

          if ( dt != -1 && check_dodge( ch, victim, dt ) )
             return;
       }

    /*
     * Check for disarm, trip, parry, and dodge.
     */
    if ( dt >= TYPE_HIT && block )
    {
       if ( !IS_NPC(victim) && IS_CLASS(victim,CLASS_MONK) )
          {
             if ( IS_SET(victim->more,MORE_BLADEGRASP) )
                do_monkdisarm(victim,ch);
          } 

       if ( check_weapon_guard( ch, victim, dt ) )
          return;

       if ( check_shield_block( ch, victim ) )
          return;
    }

    if ( dt >= 1000 && dt <= 1012 && IS_CLASS( victim, CLASS_WISP ) )
       {
          if ( dt == 1011 || dt == 1002 )
             dam = 0;
             else if ( dt == 1001 || dt == 1003 )
                     dam /= 2;
             else if ( dt == 1012 )
                     dam *= 1.25;
       }

    if ( dt != -1 )
       dam_message( ch, victim, dam, dt );
  }

  hurt_person(ch,victim,dam,TRUE);
}

bool show_shaolin_ipose( CHAR_DATA *ch, CHAR_DATA *victim )
{
}

void do_kinfo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
       return;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
      if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO)
           && d->character->in_room != ch->in_room )
	{
          send_to_char( "#y[#w<#RFatality#w>#y]#c ", d->character );
          send_to_char( argument, d->character );
          send_to_char( " #y[#w<#RFatality#w>#y]#n\n\r", d->character );
	}
    }
}

void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument )
{
   CHAR_DATA *victim;
   MOB_INDEX_DATA *pMobIndex; 
   char arg1 [MAX_INPUT_LENGTH];
   char arg2 [MAX_INPUT_LENGTH];
   char buf [MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if ( str_cmp(arg2,"mob") && obj->chobj != NULL && !IS_NPC(obj->chobj) && IS_AFFECTED(obj->chobj,AFF_POLYMORPH) )
      {
         victim = obj->chobj;
         make_part(victim,"cracked_head");
         make_part(victim,"brain");
         sprintf(buf,"the quivering brain of %s",victim->name);
         free_string(victim->morph);
         victim->morph = str_dup(buf);
         return;
      }
      else if ( !str_cmp(arg2,"mob") )
              {
                 if ( ( pMobIndex = get_mob_index( obj->value[1] ) ) == NULL ) 
                    return;

                 victim = create_mobile( pMobIndex );
                 char_to_room(victim,ch->in_room);
                 make_part(victim,"cracked_head");
                 make_part(victim,"brain");
                 extract_char(victim,TRUE);
                 return;
              }
              else {
                      if ( ( pMobIndex = get_mob_index( 30002 ) ) == NULL ) 
                         return;

                      victim = create_mobile( pMobIndex );
                      sprintf( buf, capitalize(arg2) );
                      free_string( victim->short_descr );
                      victim->short_descr = str_dup( buf );
                      char_to_room(victim,ch->in_room);
                      make_part(victim,"cracked_head");
                      make_part(victim,"brain");
                      extract_char(victim,TRUE);
                      return;
                   }
}

void do_crack( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->pIndexData->vnum != 12 )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || obj->pIndexData->vnum != 12 )
         if ( ( ( obj = get_eq_char( ch, WEAR_THIRD ) ) == NULL ) || obj->pIndexData->vnum != 12 )
            if ( ( ( obj = get_eq_char( ch, WEAR_FOURTH ) ) == NULL ) || obj->pIndexData->vnum != 12 )
               {
                  send_to_char("You are not holding any heads.\n\r",ch);
                  return;
               }

   act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR);
   act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM);
   act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_CHAR);

   if ( obj->chobj != NULL )
      {
         act("$p cracks open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_NOTVICT);
         act("$p crack open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_VICT);
      }
      else act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_ROOM);

   crack_head(ch,obj,obj->name);
   obj_from_char(obj);
   extract_obj(obj);
}

void behead( CHAR_DATA *victim )
{
   char buf [MAX_STRING_LENGTH];

   if ( IS_NPC(victim) )
      return;

   stop_fighting( victim, TRUE );
   make_corpse( victim );
   make_part( victim, "head" );

   if ( victim->attacker != NULL )
      {
         victim->attacker->attacker = NULL;
         victim->attacker = NULL;
      }

   while ( victim->affected )
     affect_remove( victim, victim->affected );

   victim->affected_by = 0;

   if ( victim->protecting != NULL )
      {
         victim->protecting->protector = NULL;
         victim->protecting = NULL;
      }

   if ( victim->protector != NULL )
      {
         act("You stop protecting $N from all attacks.",victim->protector,NULL,victim,TO_CHAR);
         victim->protector->protecting = NULL;
         victim->protector = NULL;
      }

   if ( victim->binding != NULL )
      {
         victim->binding->binded = NULL;
         victim->binding = NULL;
      }

   if ( victim->binded != NULL )
      {
         victim->binded->binding = NULL;
         victim->binded = NULL;
      }

   if ( IS_SET( victim->more, MORE_BARRIER ) )
      REMOVE_BIT( victim->more, MORE_BARRIER );

   if ( IS_CLASS(victim,CLASS_SAIYAN) )
      victim->saiyanab[BODY] = 0;

   free_string( victim->morph );
   victim->morph = str_dup( "" );

   if ( IS_CLASS(victim,CLASS_DRAGON) )
      victim->dragonform = 0;

   die_follower( victim );

   REMOVE_BIT(victim->immune, IMM_STAKE);
   REMOVE_BIT(victim->extra, TIED_UP);
   REMOVE_BIT(victim->extra, GAGGED);
   REMOVE_BIT(victim->extra, BLINDFOLDED);
   victim->pcdata->stats[DEMON_POWER] = 0;
   victim->itemaffect   = 0;
   victim->loc_hp[0]    = 0;
   victim->loc_hp[1]    = 0;
   victim->loc_hp[2]    = 0;
   victim->loc_hp[3]    = 0;
   victim->loc_hp[4]    = 0;
   victim->loc_hp[5]    = 0;
   victim->loc_hp[6]    = 0;
   victim->raiding = 0;
   victim->armor  = 100;
   victim->position   = POS_RESTING;
   victim->hit    = UMAX( 1, victim->hit  );
   victim->mana   = UMAX( 1, victim->mana );
   victim->move   = UMAX( 1, victim->move );
   victim->hitroll  = 0;
   victim->damroll  = 0;
   victim->saving_throw = 0;
   victim->carry_weight = 0;
   victim->carry_number = 0;

   SET_BIT(victim->loc_hp[0],LOST_HEAD);
   SET_BIT(victim->affected_by,AFF_POLYMORPH);
   sprintf(buf,"the severed head of %s",victim->name);
   free_string(victim->morph);
   victim->morph = str_dup(buf);

   do_call(victim,"all");
   save_char_obj( victim );
}

void do_decapitate( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA  *weapon;
   int message = 0;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   bool sparadox = FALSE, reverse = FALSE, murder = FALSE;
   int range = 20, sn = skill_lookup("vengeance");

   if ( IS_NPC(ch) )
      return;

   one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         send_to_char( "Decapitate whom?\n\r", ch );
         return;
      }

   if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 ) 
      {
         send_to_char( "Not in here.\n\r", ch );
         return;
      }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( IS_NPC(victim) )
      {
         send_to_char( "You can only decapitate other players.\n\r", ch );
         return;
      }

   if ( ch == victim )
      {
         send_to_char( "That might be a bit tricky...\n\r", ch );
         return;
      }

   if ( victim->pcdata->team > 0 )
      {
         send_to_char( "You cann't decapitate quest participants.\n\r", ch );
         return;
      }

   if ( victim->position != POS_MORTAL )
      {
         send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
         return;
      }

   if ( is_safe(ch,victim) )
      return;

   if ( number_percent() > 94 )
      {
         CHAR_DATA *mob;
         int option = number_range( 1, 5 );

         mob = get_mob( 104 );

         if ( mob != NULL )
            {
               switch( option )
               {
                  default:
                  case 1: do_chat( mob, "Even if the world forsakes you, I shall save you..." );
                          break;
                  case 2: do_chat( mob, "The power to restore all wounds." );
                          break;
                  case 3: do_chat( mob, "Praise the Light, for your guardian angel has answered." );
                          break;
                  case 4: do_chat( mob, "Come behind me children of man, for I shall protect you." );
                          break;
                  case 5: do_chat( mob, "*sighs* Will this fighting ever cease...?" );
                          break;
               }

               victim->hit = UMIN( victim->max_hit, victim->hit + ( victim->max_hit * 0.15 ) );
               REMOVE_BIT( victim->extra, TIED_UP );

               sprintf( buf, "#P%s #whas been saved by #CSaint Altima#w... The #CGuardian #yAngel#w...", victim->pcdata->switchname );
               do_info( mob, buf );
               
               if ( victim->position < POS_FIGHTING )
                  victim->position = POS_STANDING;

               if ( !IS_CLASS( ch, CLASS_WISP ) )
                  ch->wait = 18;
                  else ch->position = POS_STUNNED;

               return;
            }
      }

   if ( IS_SET(ch->more3, MORE3_CONFUSE) )
      REMOVE_BIT(ch->more3, MORE3_CONFUSE);

   if ( !IS_NPC( ch ) && ch->fight_timer == 0 )
      give_align( ch, -100 );
      else if ( IS_GOOD( victim ) )
      {      
         if ( IS_EVIL( ch ) )
            give_align( ch, -50 );
            else if ( IS_GOOD( ch ) )
                    give_align( ch, -25 );
      }
      else if ( IS_EVIL( victim ) )
      {      
         if ( IS_GOOD( ch ) )
            give_align( ch, 50 );
            else if ( IS_EVIL( ch ) )
                    give_align( ch, -25 );
      }

   if ( endozone > 0 )
      {
         if ( number_percent() > 65 )
            {
               CHAR_DATA *temp = victim, *mount;

               sprintf( buf, "#eA loud demonic cackle fills the air as #w%s's #cweapon #R#zturns#n #eagainst them!#n", ch->name );
               do_info( NULL, buf);

               ch->position = POS_STANDING;
               reverse = TRUE;

               victim = ch;
               ch = temp;

               if ( ( mount = victim->mount ) != NULL )
                  {
                     if ( victim->mounted == IS_RIDING )
                        {
                           act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
                           act("You fall off $N.",victim,NULL,mount,TO_CHAR);
                        }

                     mount->mount    = NULL;
                     victim->mount   = NULL;
                     mount->mounted  = IS_ON_FOOT;
                     victim->mounted = IS_ON_FOOT;
                     stop_fighting( mount, TRUE );
                     extract_char( mount, TRUE );
                  }
            }
      }

   if ( ch->binding != NULL )
      {
         ch->binding->binded = NULL;
         ch->binding = NULL;
      }

   if ( ch->binded != NULL )
      {
         ch->binded->binding = NULL;
         ch->binded = NULL;
      }

   if ( ( weapon = get_eq_char( ch, WEAR_WIELD ) ) != NULL )
      {
         act( "You bring your weapon down upon $N's neck!",  ch, NULL, victim, TO_CHAR    );
         send_to_char( "Your head is sliced from your shoulders!\n\r", victim);
         act( "$n swings $s weapon down towards $N's neck!",  ch, NULL, victim, TO_NOTVICT );
         act( "$n's head is sliced from $s shoulders!", victim, NULL, NULL, TO_ROOM);
      }
      else {
              send_to_char( "Your head is ripped from your shoulders!\n\r", victim);
              act( "You rip $N's head from $S shoulders!", ch, NULL, victim, TO_CHAR);
              act( "$n rips $N's head from $S shoulders!", ch, NULL, victim, TO_NOTVICT);
           }

   act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, TO_CHAR    );
   act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, TO_NOTVICT );
   act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, NULL, TO_CHAR    );
   act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, TO_NOTVICT );

   if ( reverse )
      ch->position = POS_MORTAL;

   if ( arena != FIGHT_WAR )
      {
         if ( victim->pkill < 20 )
            range = 5;
            else if ( victim->pkill < 40 )
                    range = 10;

         if ( armageddon == TRUE || victim->pkill + range >= ch->pkill )
            {
               if ( armageddon )
                  give_bm_affect( ch );
            }
            else sparadox = TRUE;

         if ( !armageddon )
            {
               if ( IS_SET(ch->newbits, NEW_DARKNESS) )
                  {
                     REMOVE_BIT(ch->newbits, NEW_DARKNESS);
                     ch->in_room->drows--;
                  }
 
               if ( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) && ch->in_room->drows == 0 )
                  REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
            }
      }
      else give_bm_affect( ch );

   if ( ch->pcdata->deity != 0 && !sparadox && !IS_CLASS( ch, CLASS_SNOW_ELF ) && !IS_CLASS( ch, CLASS_PHOENIX )
        && !IS_CLASS( ch, CLASS_DUERGAR ) && !IS_CLASS( ch, CLASS_WISP ) && !IS_CLASS( ch, CLASS_HOBBIT )
        && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
      {
          if ( ch->pcdata->deity == victim->pcdata->deity )
             ch->cmbt[0] = UMAX( 0, ch->cmbt[0] - 2 );
             else if ( ch->pcdata->deity == DEI_SIREN && victim->pcdata->deity == DEI_DIABLOS )
                     ch->cmbt[0] += 2;
             else if ( victim->pcdata->deity == DEI_SIREN && ch->pcdata->deity == DEI_DIABLOS )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_TITAN && victim->pcdata->deity == DEI_BIRD )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_LUNA && victim->pcdata->deity == DEI_GAIA )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_GAIA && victim->pcdata->deity == DEI_LUNA )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_BIRD && victim->pcdata->deity == DEI_TITAN )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_IFRIT && victim->pcdata->deity == DEI_SHIVA )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_SHIVA && victim->pcdata->deity == DEI_IFRIT )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_CARB && victim->pcdata->deity == DEI_ODIN )
                     ch->cmbt[0] += 2;
             else if ( ch->pcdata->deity == DEI_ODIN && victim->pcdata->deity == DEI_CARB )
                     ch->cmbt[0] += 2;
             else if ( ( ch->pcdata->deity == DEI_SIREN || ch->pcdata->deity == DEI_SHIVA
                       || ch->pcdata->deity == DEI_CARB || ch->pcdata->deity == DEI_BIRD
                       || ch->pcdata->deity == DEI_GAIA )
                       && ( victim->pcdata->deity == DEI_ODIN || victim->pcdata->deity == DEI_TITAN
                       || victim->pcdata->deity == DEI_IFRIT || victim->pcdata->deity == DEI_DIABLOS
                       || victim->pcdata->deity == DEI_LUNA ) )
                     ch->cmbt[0] += 1;
             else if ( ( victim->pcdata->deity == DEI_SIREN || victim->pcdata->deity == DEI_SHIVA
                       || victim->pcdata->deity == DEI_CARB || victim->pcdata->deity == DEI_BIRD
                       || victim->pcdata->deity == DEI_GAIA )
                       && ( ch->pcdata->deity == DEI_ODIN || ch->pcdata->deity == DEI_TITAN
                       || ch->pcdata->deity == DEI_IFRIT || ch->pcdata->deity == DEI_DIABLOS
                       || ch->pcdata->deity == DEI_LUNA ) )
                     ch->cmbt[0] += 1;
             else if ( ( victim->pcdata->deity == DEI_SIREN || victim->pcdata->deity == DEI_SHIVA
                       || victim->pcdata->deity == DEI_CARB || victim->pcdata->deity == DEI_BIRD
                       || victim->pcdata->deity == DEI_GAIA )
                       && ( ch->pcdata->deity == DEI_CARB || ch->pcdata->deity == DEI_BIRD
                       || ch->pcdata->deity == DEI_SIREN || ch->pcdata->deity == DEI_SHIVA
                       || ch->pcdata->deity == DEI_GAIA ) )
                     ch->cmbt[0] = UMAX( 0, ch->cmbt[0] - 1 );
             else if ( ( victim->pcdata->deity == DEI_ODIN || victim->pcdata->deity == DEI_TITAN
                       || victim->pcdata->deity == DEI_IFRIT || victim->pcdata->deity == DEI_DIABLOS
                       || victim->pcdata->deity == DEI_LUNA )
                       && ( ch->pcdata->deity == DEI_ODIN || ch->pcdata->deity == DEI_TITAN
                       || ch->pcdata->deity == DEI_IFRIT || ch->pcdata->deity == DEI_DIABLOS
                       || ch->pcdata->deity == DEI_LUNA ) )
                     ch->cmbt[0] = UMAX( 0, ch->cmbt[0] - 1 );
      }

   if ( !sparadox && IS_CLASS( victim, CLASS_ASSASSIN ) && IS_SET( victim->special, SPC_SOR ) )
      {
         act( "A grin appears on your face as your sword starts pulsing with dark energy.", victim, NULL, NULL, TO_CHAR );
         act( "A grin appears on $n's face as $s sword starts pulsing with dark energy.", victim, NULL, NULL, TO_ROOM );
         act( "Your sword slowly hovers in the air as sparks of energy flitters to the ground.", victim, NULL, NULL, TO_CHAR );
         act( "$n's sword slowly hovers in the air as sparks of energy flitters to the ground.", victim, NULL, NULL, TO_ROOM );
         act( "Suddenly the sword explodes in a flash of light as intense pain soars through you!", ch, NULL, NULL, TO_CHAR );
         act( "Suddenly the sword explodes in a flash of light as intense pain soars through $n!", ch, NULL, NULL, TO_ROOM );
         ch->hit -= is_elemental( ATTACK_DARK, ch, number_range( 250000, 1500000 ) );
         update_pos( ch );
         ch->wait = number_range( 24, 36 );
      }

   if ( !sparadox && IS_CLASS( victim, CLASS_NECROMANCER ) && victim->pkill > 19 )
      {
         act( "Your corpse trembles as it suddenly explodes in $N's face!", victim, NULL, ch, TO_CHAR );
         act( "$n's corpse trembles as it suddenly explodes in your face!", victim, NULL, ch, TO_VICT );
         act( "$n's corpse trembles as it suddenly explodes in $N's face!", victim, NULL, ch, TO_NOTVICT );
         ch->blind_timer = 2;
         SET_BIT( ch->more, MORE_BLIND );
      }

   if ( IS_CLASS( victim, CLASS_DEMON ) && victim->pkill > 19 && number_percent() > 70 
        && weather_info.sky != SKY_ECLIPSE && weather_info.sky != SKY_APOCALYPSE )
      {
         act( "A loud demonic cackle fills the air as the sky suddenly goes dark...", ch, NULL, NULL, TO_CHAR );
         act( "A loud demonic cackle fills the air as the sky suddenly goes dark...", ch, NULL, NULL, TO_ROOM );
         act( "The voice screams '#yFOOL!#n' as hundrends of red blood portals appear around you.", ch, NULL, NULL, TO_CHAR );
         act( "The voice screams '#yFOOL!#n' as hundrends of red blood portals appear around you.", ch, NULL, NULL, TO_ROOM );
         act( "'#yThe end is NEAR!!! Hahahaha!!#n' it continues as Apocalypse has arised...", ch, NULL, NULL, TO_CHAR );
         act( "'#yThe end is NEAR!!! Hahahaha!!#n' it continues as Apocalypse has arised...", ch, NULL, NULL, TO_ROOM );
         weather_info.sky = SKY_APOCALYPSE;
      }

   ch->bank = UMIN( 500000000, 25000 + ch->bank );

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      REMOVE_BIT(ch->act, PLR_WIZINVIS);

   if ( victim->pcdata->bounty > 0 )
      {
         stc("#gYou #rhave #ereceived #Rblood #Gmoney#g for killing this scum. Excellent.#n\n\r",ch);
         ch->gold = UMIN( 500000000, victim->pcdata->bounty + ch->gold );
         victim->pcdata->bounty = 0;
      }

   if ( ch->pcdata->decap[0] != '\0' )
      {
         int test = 0;
         DESCRIPTOR_DATA *d;

         if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
            {
               REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
               test = 1;
            }

         buf[0] = '\0';

         strcat( buf, "#y[#w<#RFatality#w>#y]#c " );
         strcat( buf, ch->pcdata->decap );
         strcat( buf, " #y[#w<#RFatality#w>#y]#n" );
    
         for ( d = descriptor_list; d != NULL; d = d->next )
         {
            if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO)
                 && d->character->in_room != ch->in_room )
                act( buf,ch,victim->name,d->character,TO_VICT );
         }

         if ( test == 1 )
            SET_BIT(ch->affected_by, AFF_POLYMORPH);

         message = 1;
      }

   if ( ch->pcdata->decap_2[0] != '\0' )
      {
         int test = 0;
         DESCRIPTOR_DATA *d;

         if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
            {
               REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
               test = 1;
            }

         buf[0] = '\0';

         strcat( buf, "#y[#w<#RFatality#w>#y]#c " );
         strcat( buf, ch->pcdata->decap_2 );
         strcat( buf, " #y[#w<#RFatality#w>#y]#n" );
    
         for ( d = descriptor_list; d != NULL; d = d->next )
         {
            if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO)
                 && d->character->in_room != ch->in_room )
               act( buf,ch,victim->name,d->character,TO_VICT );
         }

         if ( test == 1 )
            SET_BIT(ch->affected_by, AFF_POLYMORPH);

         message = 1;
      }

   if ( message != 1 )
      {
         switch( ch->class )
         {
            case CLASS_DARAMON:
            case CLASS_MONK: sprintf(buf,"#c%s silently prays for the salvation of %s's soul.#n",ch->name,victim->name);
                             do_kinfo(ch,buf);
                             break;
            case CLASS_WISP: sprintf(buf,"#y%s transforms into a massive mass of energy as they engulf %s.#n",ch->name,victim->name);
                             do_kinfo(ch,buf);
                             break;
            case CLASS_PHOENIX: sprintf(buf,"#R%s blasts %s to ashes with their fiery flames.#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
            case CLASS_DUERGAR: sprintf(buf,"#g%s screams 'FOR TITAN!' as they lop %s's head off with their axe.#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
            case CLASS_HOBBIT: sprintf(buf,"#e%s concentrates on the #wone #ering as %s gets erased out of existence.#n",ch->name,victim->name);
                               do_kinfo(ch,buf);
                               break;
            case CLASS_DROW: sprintf(buf,"#e%s has sacrficed %s to LLoth.#n",ch->name,victim->name);
                             do_kinfo(ch,buf);
                             sprintf(buf,"#eLLoth is pleased.#n");
                             do_kinfo(ch,buf);
                             break;
            case CLASS_SHADOW_WAR: sprintf(buf,"#e%s screams in terror as %s violently pulls them into the shadows.#n",victim->name,ch->name);
                                   do_kinfo(ch,buf);
                                   break;
            case CLASS_SNOW_ELF: sprintf(buf,"#C%s has sacrficed %s to Shiva by smashing them to bits.#n",ch->name,victim->name);
                                 do_kinfo(ch,buf);
                                 sprintf(buf,"#CShiva is pleased.#n");
                                 do_kinfo(ch,buf);
                                 break;
            case CLASS_KURUDA:
            case CLASS_ASSASSIN: sprintf(buf,"#e%s's body cackles with ferocious energy as they dash towards %s.#n",ch->name,victim->name);
                                 do_kinfo(ch,buf);
                                 sprintf(buf,"#eBefore %s can recover, %s grasps their head letting it explodes.#n",victim->name,ch->name);
                                 do_kinfo(ch,buf);
                                 break;
            case CLASS_DRAGOON: sprintf(buf,"#g%s #ccowers on their knees begging #p%s #cfor mercy like a woman.#n",victim->name,ch->name);
                                do_kinfo(ch,buf);
                                sprintf(buf,"#y'$^*%% you!#c' #p%s #csays as they shove a spear down #g%s #cthroat!#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
            case CLASS_NIGHT_BLADE:
            case CLASS_SHADOW:
            case CLASS_MASTER:
            case CLASS_NINJA: sprintf(buf,"#e%s has plunged a dagger in %s's back ending their life.#n",ch->name,victim->name);
                              do_kinfo(ch,buf);
                              break;
            case CLASS_DRAGON_KNIGHT: sprintf(buf,"#p%s #csummons the #wDivine #oDragon #cas they #RInCiReNaTe #y%s #cin one blast!#n",ch->name,victim->name);
                                      do_kinfo(ch,buf);
                                      break;
            case CLASS_FIEND:
            case CLASS_DEMON: sprintf(buf,"#r%s's soul have been stolen by %s.#n",victim->name,ch->name);
                              do_kinfo(ch,buf);
                              break;
            case CLASS_WIZARD: sprintf(buf,"#g%s grins as %s's body got shattered into millions of pieces.#n",ch->name,victim->name);
                               do_kinfo(ch,buf);
                               break;
            case CLASS_DRAGON:
            case CLASS_SAIYAN:
            case CLASS_WEREWOLF: sprintf(buf,"#R%s savagely RIPS of %s's head from the body.#n",ch->name,victim->name);
                                 do_kinfo(ch,buf);
                                 break;
            case CLASS_VAMPIRE: sprintf(buf,"#R%s sucks all the blood from %s's body.#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
            case CLASS_SAMURAI: sprintf(buf,"#y%s drives %s face into the ground as they hacks of their head viciously.#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
            case CLASS_ANGEL: sprintf(buf,"#w%s raises back their sword as they viciously cut %s in half.#n",ch->name,victim->name);
                              do_kinfo(ch,buf);
                              break;
            case CLASS_UNDEAD_KNIGHT: sprintf(buf,"#e%s removes their mask scaring %s to death.#n",ch->name,victim->name);
                                      do_kinfo(ch,buf);
                                      break;
            case CLASS_NECROMANCER: sprintf(buf,"#e%s drains the life energy from %s's rotting corpse.#n",ch->name,victim->name);
                                    do_kinfo(ch,buf);
                                    break;
            case CLASS_PALADIN: sprintf(buf,"#w%s shouts 'Good will always triumph over evil!!'#n",ch->name);
                                do_kinfo(ch,buf);
                                sprintf(buf,"#w%s slices off %s's head executioner style!!#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
		case CLASS_DAYWALKER: sprintf(buf,"#R%s pounces on %s ripping them completely apart!#n",ch->name,victim->name);
                                do_kinfo(ch,buf);
                                break;
         }
      }

   ch->pcdata->bounty = UMIN( 500000000, ch->pcdata->bounty + number_range( 25000, 50000 ) * number_range( 1, UMAX( 1, ch->pkill / 3 ) ) );

   if ( ch->pcdata->kingdom != 0 && victim->pcdata->kingdom != 0 )
      {
         king_table[ch->pcdata->kingdom].pk += 1;
         king_table[victim->pcdata->kingdom].pd += 1;

         if ( arena == FIGHT_WAR )
            {
                king_table[ch->pcdata->kingdom].wk += 1;   
                ch->wkill++;

                if ( king_table[ch->pcdata->kingdom].wk > w1_kills )
                   {
                      if ( w1_kdg != -1 && w1_kdg != ch->pcdata->kingdom )
                         {
                            w2_kdg = w1_kdg;
                            w2_kills = w1_kills;
                         }

                      w1_kdg = ch->pcdata->kingdom;
                      w1_kills = king_table[ch->pcdata->kingdom].wk;
                   }
                   else if ( king_table[ch->pcdata->kingdom].wk > w2_kills && w1_kdg != ch->pcdata->kingdom )
                   {
                      w2_kdg = ch->pcdata->kingdom;
                      w2_kills = king_table[ch->pcdata->kingdom].wk;
                   }

                if ( ch->wkill > man_kills )
                   {
                      free_string( man_match );
                      man_match = str_dup( ch->pcdata->switchname );
                      man_kills = ch->wkill;
                   }
            }
      }

   if ( IS_CLASS( ch, CLASS_ASSASSIN ) && number_percent() > 50 )
      {
         act( "Suddenly, a dark figure in a furry coat leaps silently beside you.", ch, NULL, NULL, TO_CHAR );
         act( "Suddenly, a dark figure in a furry coat leaps silently beside $n.", ch, NULL, NULL, TO_ROOM );
         act( "Krizalid sneers '#yWell done assassin...#n' as you coldly turn away.", ch, NULL, NULL, TO_CHAR );
         act( "Krizalid sneers '#yWell done assassin...#n' as $n coldly turns away.", ch, NULL, NULL, TO_ROOM );
         act( "Your eyes burn with rage as you mutter '#yA waste of my time... Where is Iori?#n'", ch, NULL, NULL, TO_CHAR );
         act( "$n's eyes burn with rage as $e mutters '#yA waste of my time... Where is Iori?#n'", ch, NULL, NULL, TO_ROOM );
         act( "Krizalid flexes his fingers as he says '#yWe are still looking for him...#n'", ch, NULL, NULL, TO_CHAR );
         act( "Krizalid flexes his fingers as he says '#yWe are still looking for him...#n'", ch, NULL, NULL, TO_ROOM );
         act( "You go '#yHmph...#n' as Krizalid leaps away into the darkness...", ch, NULL, NULL, TO_CHAR );
         act( "$n goes '#yHmph...#n' as Krizalid leaps away into the darkness...", ch, NULL, NULL, TO_ROOM );
      }

   if ( is_affected( victim, sn ) )
      spell_godsdisfavor( skill_lookup("gods disfavor"), 10, victim, ch );

   save_kingdoms();

   check_leaderboard(ch,"");
   check_leaderboard(victim,"");

   behead( victim );

   ch->fight_timer = 20;
   victim->fight_timer = 0;
   victim->pcdata->capture_timer = 0;
   ch->attacker = NULL;
   victim->attacker = NULL;

   sprintf( log_buf, "%s decapitated by %s at %d.", victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
   log_string( log_buf );

   players_decap++;

   if ( ( players_decap % 25 ) == 0 && players_decap > 25 && !armageddon )
      do_nogang( ch, "" );

   if ( ( players_decap % 30 ) == 0 && arena == FIGHT_CLEAR )
      {
         arena = FIGHT_AUTO;
         do_info( NULL, "#wThe #PA#oren#Pa #gis open for #w#zEverybody#n#g! Use #yFtag #gto Join!!!#n" );
      }

   if ( ( players_decap % 50 ) == 0 && !armageddon )
      {
         start_scavenger();
         do_sinfo( NULL, "#w50 #yitems #ghas been #Pscattered #gthroughout the #Prealm#g! Use #yScavenger #gto Join!!!#n" );
      }

   if ( nogang == 10 && !armageddon )
      {
         restore_char( ch );
         ch->fight_timer = 0;
      }

   if ( sparadox )
      {
         if ( ch->retaliate != victim )
            paradox( ch );
      }
      else {
              ch->agg /= 2;
              ch->pkill++;
              ch->practice += 2;
              lucky_token--;

              if ( ch->pkill > 30 )
                 add_transmigration( ch, number_range( 45000, 60000 ) );
                 else add_transmigration( ch, number_range( 25000, 35000 ) );

              if ( ch->tier < 4 )
                 { 
                    if ( ch->generation != 1 )
                       ch->generation--;
                       else {
                               stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",ch);
                               ch->generation = 5;
                               ch->tier++;
                               ch->max_hit += 1000000;
                               ch->max_mana += 1000000;
                               ch->max_move += 1000000;
                            }
                 }

              if ( number_percent() > 94 )
                 {
                    stc( "#wYou #yhave #gwon #Ra #P#zfree#n #oskill#ebuy #cpoint#L!#y!#C!#n\n\r", ch );
                    sprintf( buf, "#w%s #yhas #gwon #Ra #P#zfree#n #oskill#ebuy #cpoint#L!#y!#C!#n", ch->pcdata->switchname );
                    do_info( ch, buf );
                    ch->cmbt[4]++;
                 }

              if ( lucky_token == 5 )
                 do_info( NULL, "#gOnly #y5 #gmore #Rpkills #gbefore a #sl#su#sc#sk#sy#n #wtoken #gis given away!!!#n" );

              if ( lucky_token == 0 )
                 {
                    OBJ_INDEX_DATA *pObjIndex;
                    OBJ_DATA *obj;
                    int value;

                    lucky_token = number_range( 10, 25 );
                    value = number_range( 500000, 2000000 );

                    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) != NULL )
                       {
                          obj = create_object( pObjIndex, 60 );
                          obj_to_char( obj, ch );

                          obj->cost = 0;
                          obj->value[0] = value;
                          obj->item_type = ITEM_QUEST;
  
                          if ( obj->questmaker != NULL )
                             free_string(obj->questmaker);

                          obj->questmaker = str_dup(ch->name);
                          free_string( obj->name );
                          obj->name = str_dup( "gold token" );
                          sprintf(buf,"a #w%d #ygold #etoken#n",value);
                          free_string( obj->short_descr );
                          obj->short_descr = str_dup( buf );
                          sprintf(buf,"A #w%d #ygold#n token lies on the floor.",value);
                          free_string( obj->description );
                          obj->description = str_dup( buf );

                          obj->questowner = str_dup( ch->pcdata->switchname );
                       }

                    act("#gYou #whave won the #s#zl#s#zu#s#zc#s#zk#s#zy#n #ytoken#w!!!#n",ch,NULL,NULL,TO_CHAR);
                    sprintf( buf, "#g%s #whas won a #w%d #s#zl#s#zu#s#zc#s#zk#s#zy#n #ytoken#w!!!#n", ch->pcdata->switchname, value );
                    do_info( ch, buf );
                 }

              if ( IS_CLASS( ch, CLASS_WEREWOLF ) )
                 do_unwerewolf( ch, "" );

              if ( !armageddon )
                 victim->pdeath++;

              add_grove( ch, NULL, -150 );

              if ( IS_CLASS( ch, CLASS_PALADIN ) && ch->clannum != 1 && ch->pkill < 21 && ( ( ch->pkill % 2 ) == 0 ) )
                 {
                    int count;

                    for ( count = 8; count < 16; count++ )
                        ch->aura[count]++;

                    stc("#wYour mastery of the #Cancient auras #whave improved warrior.\n\r", ch );
                 }

              if ( IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && ch->pkill > 2 && ch->pkill < 21 && ( ( ch->pkill % 2 ) == 0 ) )
                 ch->pcdata->powers[DRAGOON_LEVEL]++;

              if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) && ch->pkill < 21 && ( ( ch->pkill % 2 ) == 0 ) )
                 {
                    int count;

                    for ( count = 28; count < 35; count++ )
                        ch->aura[count]++;

                    stc("#eYour mastery of the #Pancient auras #ehave improved warrior.\n\r", ch );
                 }

              if ( IS_CLASS( ch, CLASS_DARAMON ) && ch->pkill < 16 )
                 {
                    switch( ch->pkill )
                    {
                       default: break;
                       case 2: ch->aura[A_RHOLY] = 10; break;
                       case 4: ch->aura[A_REARTH] = 10; break;
                       case 6: ch->aura[A_RCOLD] = 10; break;
                       case 8: ch->aura[A_RTHUNDER] = 10; break;
                       case 10: ch->aura[A_RDARK] = 10; break;
                       case 15: ch->aura[A_RFIRE] = 10; break;
                    }

                    if ( ch->pkill == 2 || ch->pkill == 4 || ch->pkill == 6 || ch->pkill == 8
                         || ch->pkill == 10 || ch->pkill == 15 )
                       stc("#wYour mastery of the #Cancient auras #whave improved warrior.#n\n\r", ch );
                 }

              if ( IS_CLASS( ch, CLASS_WIZARD ) && ch->pkill < 21 )
                 ch->max_mana += 250000;

              if ( IS_CLASS( ch, CLASS_SAIYAN ) && ch->pkill < 21 )
                 ch->saiyan_powerlevel += 5000;

              if ( ( IS_CLASS( ch, CLASS_PHOENIX ) || IS_CLASS( ch, CLASS_SNOW_ELF )
                     || IS_CLASS( ch, CLASS_HOBBIT ) || IS_CLASS( ch, CLASS_DUERGAR )
                     || IS_CLASS( ch, CLASS_WISP ) || IS_CLASS( ch, CLASS_DRAGON ) ) && ch->pkill < 11 )
                 ch->max_hit += 100000;

              if ( ch->pkill < 21 )
                 ch->max_hit += 350000;

              if ( ( ch->pkill % 10 ) == 0 )
                 give_random_aura( ch );

              if ( ch->pkill + 4 <= victim->pkill )
                 find_item( ch, 1 );
                 else if ( ch->pkill + 9 <= victim->pkill ) 
                         find_item( ch, 2 );
                         else find_item( ch, 0 );

              if ( --jaggar == 0 )
                 {
                    murder = TRUE;
                    do_jaggar( ch );
                 }
           }

    if ( arena == FIGHT_WAR && number_percent() > 75 && !murder )
       {
           CHAR_DATA *mob, *mob2;
           int option = number_range( 1, 5 );
           DESCRIPTOR_DATA *d;

           mob = get_mob( 100 );
           mob2 = get_mob( 101 );

           for ( d = descriptor_list; d != NULL; d = d->next )
           {
              if ( d->connected == CON_PLAYING && d->character != NULL )
                 stc( "\n\r", d->character );
           }

           switch( option )
           {
              case 1: if ( mob != NULL )
                         {
                            do_chat( mob, "Heh. hehehe. Hahahahahahaha!" );
                            break;
                         }                       

              case 2: if ( mob != NULL && mob2 != NULL )
                         {
                            sprintf( buf, "Wow! They are really trying to stop %s now!", ch->pcdata->switchname );
                            do_chat( mob2, buf );

                            sprintf( buf, "Let them use whatever they want. They still won't defeat %s.", ch->pcdata->switchname );
                            do_chat( mob, buf );
                            break;
                         }

              case 3: if ( mob2 != NULL )
                         {
                            sprintf( buf, "Slaughter ALL %s! Hahaha, make the earth become red.", ch->pcdata->switchname );
                            do_chat( mob2, buf );
                            break;
                         }                       

              case 4: if ( mob != NULL && mob2 != NULL )
                         {
                            sprintf( buf, "What the hell?!? %s lost?!?", victim->pcdata->switchname );
                            do_chat( mob2, buf );

                            do_chat( mob, "That is correct." );
                            break;
                         }

              case 5: if ( mob != NULL )
                         {
                            sprintf( buf, "Come on %s. We got no time to lose.", ch->pcdata->switchname );
                            do_chat( mob, buf );
                            break;
                         }
           }
       }
       else if ( arena == FIGHT_WAR && number_percent() > 65 && !murder )
               {
                  sprintf( buf, "#wThe #Pc#er#Co#yw#Ld #gcheers #o%s #gon!#n", ch->pcdata->switchname );
                  do_info( NULL, buf );
               }

   ch->retaliate = NULL;
   victim->retaliate = NULL;
   save_char_obj( ch );
}

void give_bm_affect( CHAR_DATA *ch )
{
   if ( IS_NPC( ch ) )
      return;

   ch->pcdata->blood_power = number_range( 1, 6 );
   ch->pcdata->blood_meter = 25;
   ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * 0.25 ) );
   ch->mana = UMIN( ch->max_mana, ch->mana + 5000000 );
   ch->move = UMIN( ch->max_move, ch->move + 5000000 );

   switch( ch->pcdata->blood_power )
   {
      case 1: act( "#wYour #gare suddenly surrounded by a very #edestructive #yaura#g.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n #gis suddenly surrounded by a very #edestructive #yaura#g.#n", ch, NULL, NULL, TO_ROOM );
              break;

      case 2: act( "#wYou #gsuddenly start #Pmoving #glike a #Cblur#g.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n #gsuddenly starts #Pmoving #glike a #Cblur#g.#n", ch, NULL, NULL, TO_ROOM );
              break;

      case 3: act( "#wYou #gcackle with glee as #w10 #yshimmering #oarms #gattach onto you.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n #gcackles with glee as #w10 #yshimmering #oarms #gattach onto $m.#n", ch, NULL, NULL, TO_ROOM );
              break;

      case 4: act( "#wYou #gfrown silently as your #oskin #gturns into #etitanium#g.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n #gfrowns silently as $s #oskin #gturns into #etitanium#g.#n", ch, NULL, NULL, TO_ROOM );
              break;

      case 5: act( "#wYour hands #Cpulses #gwith #edemonic #yenergy #gas your knowledge improves#g.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n's hands #Cpulses #gwith #edemonic #yenergy #gas $s knowledge improves#g.#n", ch, NULL, NULL, TO_ROOM );
              break;

      case 6: act( "#wYour #Rwounds #gstart disappearing #wrapidly#g, right before your very eyes#g.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n's #Rwounds #gstart disappearing #wrapidly#g, right before your very eyes#g.#n", ch, NULL, NULL, TO_ROOM );
              ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * 0.1 ) );
              ch->agg /= 2;
              break;
   }
}


void remove_stuff( CHAR_DATA *victim )
{
    if ( IS_SET(victim->newbits, NEW_DARKNESS) )
       {
          REMOVE_BIT(victim->newbits, NEW_DARKNESS);
          victim->in_room->drows--;
       }

    if ( IS_SET( victim->act, PLR_WIZINVIS ) )
       REMOVE_BIT( victim->act, PLR_WIZINVIS );

    if ( IS_SET( victim->more2, MORE2_WARCRY ) )
       REMOVE_BIT( victim->more2, MORE2_WARCRY );

    if ( victim->outer_aura != -1 )
       do_aura(victim,"outer none");

    if ( victim->inner_aura != -1 )
       do_aura(victim,"inner none");

    if ( IS_CLASS( victim, CLASS_DRAGON_KNIGHT ) && IS_SET( victim->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
       do_dragoon_transform( victim, "" );

    if ( IS_CLASS( victim, CLASS_FIEND ) && IS_SET( victim->affected_by, AFF_POLYMORPH ) )
       do_true_demon( victim, "" );

    if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
       victim->pcdata->powers[MONK_SHIKI] = 0;

    victim->raiding = 0;
    victim->saiyanlevel = 0;
    victim->nemesis = 0;
    victim->agg = 100;
    victim->stunned = 0;
    victim->dpower = 0;
    REMOVE_BIT( victim->arena, AFF2_AUTOGUARD );
}

int counter_type( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int bonus = 0;

   if ( ( !str_cmp( "P", victim->groove ) || !str_cmp( "M", victim->groove ) 
        || ( !str_cmp( "C", victim->groove ) && str_cmp( "P", ch->groove ) ) ) 
        && victim->attack_focus == ch->attack_focus )
      return 10;

   switch( ch->attack_focus )
   {
      case DIRECT: if ( victim->attack_focus == LOW )
                      {
                         if ( eipheal == 3 )
                            return 200;
                            else return 10;
                      }

                   break;

      case LOW: if ( ( !str_cmp( "S", victim->groove ) || !str_cmp( "K", victim->groove )
                       || !str_cmp( "M", victim->groove ) ) && number_percent() > 69 )
                   bonus = 10;

                if ( victim->attack_focus == AERIAL )
                   {
                      if ( eipheal == 3 )
                         return 200;
                         else return 10 + bonus;
                   }

                break;

      case AERIAL: if ( victim->attack_focus == OVERHEAD && ( !str_cmp( "C", victim->groove )
                        || !str_cmp( "M", victim->groove ) ) )
                      {
                         if ( eipheal == 3 )
                            return 200;
                            else return 10;
                      }

                    break;

      case OVERHEAD: if ( victim->attack_focus == UPPERCUT )
                        {
                           if ( eipheal == 3 )
                              return 200;
                              else return 10;
                        }

                     break;

      case UPPERCUT: if ( victim->attack_focus == DIRECT )
                        {
                           if ( eipheal == 3 )
                              return 200;
                              else return 10;
                        }

                     break;
   }

   return 0;
}

int eva_bonus( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int luck2 = number_range( 0, get_curr_luck( victim ) );
   int luck = number_range( 0, get_curr_luck( ch ) );
   int counter = counter_type( ch, victim );

   if ( victim->topmode == 33 && victim->hit < victim->max_hit * 0.34 )
      luck2 += 10;

   // Paladins Hand of God's Skill
   if ( IS_CLASS( ch, CLASS_PALADIN ) && ch->pkill > 9 && get_curr_con( ch ) > get_curr_dex( ch ) )
      {
         if ( get_curr_con( ch ) <= get_curr_eva( victim ) )
            return UMIN( 15 + counter, luck2 - luck + counter );
            else return UMIN( 15 + counter, get_curr_eva( victim ) + luck2 - luck - get_curr_con( ch ) + counter );
      }
      else if ( get_curr_eva( victim ) + luck2 + counter < get_curr_dex( ch ) + luck )
              return 0;
      else return UMIN( 15 + counter, get_curr_eva( victim ) + luck2 - luck - get_curr_dex( ch ) + counter );
}

int counter_bonus( CHAR_DATA *ch, CHAR_DATA *victim, int type )
{
   int counter = counter_type( ch, victim );
   int def_bon = ( ( get_curr_dex( victim ) + get_curr_int( victim ) ) / 10 );
   int luck = number_range( 0, get_curr_luck( ch ) );
   int luck2 = number_range( 0, get_curr_luck( victim ) );
   int atk_bon;

   if ( !str_cmp( "P", victim->groove ) && victim->ctimer[18] > 0 )
      counter += 20;

   if ( conan == 9 )
      counter += 25;

   if ( IS_SET( ch->cmbt[5], SKL2_SBLOW ) )
      counter -= 7;
      else if ( IS_SET( victim->cmbt[5], SKL2_SSTRIKE ) )
              counter += 5;

   if ( IS_SET( victim->more3, MORE3_SNAKE ) )
      counter += 7;
      else if ( IS_SET( victim->more3, MORE3_WOLF ) )
              counter += 5;

   // Paladins Hand of God's Skill
   if ( IS_CLASS( ch, CLASS_PALADIN ) && ch->pkill > 9 && get_curr_con( ch ) > get_curr_dex( ch ) )
      {
         atk_bon = ( ( get_curr_con( ch ) + get_curr_int( ch ) ) / 10 );

         if ( atk_bon <= def_bon )
            return UMIN( 15 + counter, luck2 - luck + counter );
            else return URANGE( 0, def_bon - atk_bon + counter, 15 + counter );
      }
      else {
              atk_bon = ( ( get_curr_dex( ch ) + get_curr_int( ch ) ) / 10 );
              return URANGE( 0, def_bon + luck2 - atk_bon - luck + counter, 15 + counter );
           }
}

int block_bonus( CHAR_DATA *ch, CHAR_DATA *victim, int type )
{
   int counter = counter_type( ch, victim );
   int def_bon = get_curr_con( victim ) + number_range( 0, get_curr_luck( victim ) );
   int atk_bon = get_curr_dex( ch ) + number_range( 0, get_curr_luck( ch ) );

   if ( IS_SET( victim->cmbt[5], SKL2_IBODY ) )
      counter += 5;

   if ( IS_AFFECTED( victim, AFF_SHIELD ) )
      counter += 5;

   return URANGE( 0, def_bon - atk_bon + counter, 15 + counter );
}

char * show_type( int value )
{
   static char buf[23];

   buf[0] = '\0';

   switch( value )
   {
      default: strcpy( buf, "#oNone" );
               break;
      case ATTACK_PHYSICAL: strcpy( buf, "#wPhysical" );
                            break;
      case ATTACK_MAGICAL: strcpy( buf, "#PMagical" );
                           break;
   }

   return buf;
}

char * show_elem( int value )
{
   static char buf[23];

   buf[0] = '\0';

   switch( value )
   {
      default: strcpy( buf, "#oNone" );
               break;
      case ATTACK_HOLY: strcpy( buf, "#wHoly" );
                        break;
      case ATTACK_FIRE: strcpy( buf, "#RFire" );
                        break;
      case ATTACK_ICE: strcpy( buf, "#CIce" );
                       break;
      case ATTACK_EARTH: strcpy( buf, "#oEarth" );
                         break;
      case ATTACK_THUNDER: strcpy( buf, "#yThunder" );
                           break;
      case ATTACK_DARK: strcpy( buf, "#eDark" );
                        break;
      case ATTACK_LUNAR: strcpy( buf, "#oLunar" );
                         break;
   }

   return buf;
}

char * show_var( int value )
{
   static char buf[23];

   buf[0] = '\0';

   switch( value )
   {
      default: strcpy( buf, "#wNormal" );
               break;
      case HIT: strcpy( buf, "#wNormal" );
                break;
      case SLASH: strcpy( buf, "#wSlash" );
                  break;
      case PIERCE: strcpy( buf, "#wPierce" );
                   break;
      case BLAST: strcpy( buf, "#wBlast" );
                  break;
      case BEAM: strcpy( buf, "#wBeam" );
                 break;
      case BLUNT: strcpy( buf, "#wBlunt" );
                  break;
      case SOUL: strcpy( buf, "#PSpirit" );
                 break;
   }

   return buf;
}

char * show_focus( int value )
{
   static char buf[23];

   buf[0] = '\0';

   switch( value )
   {
      default: strcpy( buf, "#wDirect" );
               break;
      case DIRECT: strcpy( buf, "#wDirect" );
                   break;
      case LOW: strcpy( buf, "#wLow" );
                break;
      case AERIAL: strcpy( buf, "#wAerial" );
                    break;
      case UPPERCUT: strcpy( buf, "#wUppercut" );
                     break;
      case OVERHEAD: strcpy( buf, "#wOverhead" );
                     break;
   }

   return buf;
}

void hurt_person( CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool agg_dam )
{
  char buf[MAX_STRING_LENGTH];
  int total = -conaneffect, num_attackers = 1;
  CHAR_DATA *attacker = ( IS_NPC( ch ) && ch->master != NULL ) ? ch->master : ch;
  bool armor = FALSE;
  bool counter = FALSE;
  bool chain = FALSE;
  bool mode = FALSE;
  int old_hp = victim->hit;
  bool block = FALSE;
  int at = ch->attack_type;
  int ele = IS_SET( ch->elemental, ELE_LUNAR ) ? ATTACK_LUNAR : ch->ele_attack;
  int mydam = dam;
  bool rm = ( ch->in_room == victim->in_room ) ? TRUE : FALSE;

  if ( ch == NULL || victim == NULL || ch->in_room == NULL || victim->in_room == NULL
       || victim->position == POS_DEAD )
     return;

  if ( IS_SET( ch->arena, AFF2_SC ) )
     {
        if ( IS_SET( ch->arena, AFF2_DM ) )
           {
              if ( ch->blocking == -10 )
                 {
                    dam = UMIN( 6000000, dam * 2 );
                    SET_BIT( ch->arena, AFF2_DD );
                 }
                 else dam = UMIN( 6000000, dam * 1.5 );
           }
           else dam /= 2;
     }

  if ( dam > 0 && ( ch->nodamage > 0 || ( victim->jedi_timer > 0 && ele != ATTACK_LUNAR ) ) )
     dam = 0;
     else if ( eipheal == 1 )
             dam *= 2;
     else if ( eipheal == 2 )
             dam /= 2;

  if ( victim->raiding > 0 )
     dam *= 2;

  if ( !IS_SET( ch->arena, AFF2_UB ) && ch->absorb_timer > 0 && dam > 0 )
     {
        dam = -dam;
        ch->absorb_timer = 0;
     }

  if ( ch->wfocus > 0 )
     ele = ch->ele_attack = ch->wfocus;

  if ( !IS_SET( ch->arena, AFF2_UB ) )
     dam = is_elemental( ele, victim, dam );

  ch->ele_attack = 0;

  if ( dam < 0 )
     {
        victim->hit = UMIN( victim->max_hit, victim->hit - dam );
        return;
     }

  if ( !IS_SET( ch->arena, AFF2_UB ) )
     {
        if ( victim->position == POS_FIGHTING && victim->binded != NULL && IS_CLASS( victim->binded, CLASS_ANGEL )
             && is_same_plane( victim, victim->binded ) && nogang == 0 && ch->attacker == NULL && victim->attacker == NULL )
           {
              if ( victim->binded->pkill > 11 && !( victim->in_room->vnum >= 33500 && victim->in_room->vnum < 33800 ) )
                 {
                    if ( !IS_SET( victim->act, PLR_BRIEF2 ) && number_percent() > 85 )
                       {
                          sprintf( buf, "You feel $N's soul flicking dimly as $E suffers along with you!" );
                          act( buf, victim->binded, NULL, ch, TO_CHAR );
                       }

                    if ( !IS_SET( ch->act, PLR_BRIEF2 ) && number_percent() > 85 )
                       {
                          sprintf( buf, "Suddenly, you clutch your head in agony as you endure the wrath of $n's pain! [#r%d#n]", dam );
                          act( buf, victim->binded, NULL, ch, TO_VICT );
                       }

                    if ( victim->binded->pkill > 15 )
                       victim->binded->hit = UMIN( victim->binded->max_hit, victim->binded->hit + ( dam / 2 ) );

                    victim->binded->attack_type = ATTACK_MAGICAL;
                    victim->binded->attack_focus = AERIAL;
                    victim->binded->attack_var = SOUL;
                    hurt_person( victim->binded, ch, dam / 3, TRUE );
                 }
                 else {
                         if ( !IS_SET( victim->act, PLR_BRIEF2 ) && number_percent() > 85 )
                            {
                               sprintf( buf, "You feel $N's soul flicking dimly as $E suffers along with you!" );
                               act( buf, victim, NULL, victim->binded, TO_CHAR );
                            }

                         if ( !IS_SET( ch->act, PLR_BRIEF2 ) && number_percent() > 85 )
                            {
                               sprintf( buf, "Suddenly, you clutch your head in agony as you endure the wrath of $n's pain! [#r%d#n]", dam );
                               act( buf, victim, NULL, victim->binded, TO_VICT );
                            }

                         ch->attack_type = at;
                         hurt_person( ch, victim->binded, dam, TRUE );
                      }
           }

       if ( victim != ch && ( IS_CLASS( victim, CLASS_NINJA ) || IS_SET( victim->cmbt[5], SKL2_FATTACK ) ) && !IS_CLASS( ch, CLASS_NINJA ) 
            && !IS_CLASS( ch, CLASS_NIGHT_BLADE ) && victim->position != POS_FIGHTING && !IS_SET( ch->cmbt[5], SKL2_FATTACK )
            && rm && victim->fight_timer == 0 && !block )
          {
             act("As $N draws in for the attack, you spring to your feet as you unsheath a small dagger.",victim,NULL,ch,TO_CHAR);
             act("As you draw in for the attack, $n springs to $s feet as $e unsheathes a small dagger.",victim,NULL,ch,TO_VICT);
             act("As $N draws in for the attack, $n springs to $s feet as $e unsheathes a small dagger.",victim,NULL,ch,TO_NOTVICT);
             act("Your dagger glitters as you quickly dash towards $N in a fury.",victim,NULL,ch,TO_CHAR);
             act("$n's dagger glitters as $e quickly dashes towards you in a fury.",victim,NULL,ch,TO_VICT);
             act("$n's dagger glitters as $e quickly dashes towards $N in a fury.",victim,NULL,ch,TO_NOTVICT);

             victim->attack_type = ATTACK_PHYSICAL;
             hurt_person( victim, ch, 500000, TRUE );
             return;
          }

       if ( is_affected( victim, skill_lookup("damnation") ) && at == ATTACK_MAGICAL && dam > 149999
            && number_percent() > 94 - devil_luck( ch, victim, 5 ) )
          {
             CHAR_DATA *temp;

             act( "#RYour #ymagical shield turns #wwhite #yas it #Creflects #ythe attack!#n", victim, NULL, NULL, TO_CHAR );
             act( "#R$n's #ymagical shield turns #wwhite #yas it #Creflects #ythe attack!#n", victim, NULL, ch, TO_VICT );

             temp = victim;
             victim = ch;
             ch = temp;
          }

       if ( ( IS_CLASS( victim, CLASS_MONK ) || IS_CLASS( victim, CLASS_NINJA ) ) && victim->ctimer[10] > 0 && dam > 149999 )
          {
             start_innocence( victim, ch );
             return;
          }

       if ( IS_SET( victim->arena, AFF2_AUTOGUARD ) && dam > 149999 )
          {
             REMOVE_BIT( victim->arena, AFF2_AUTOGUARD );
 
             do_autoguard( victim, ch );
             return;
          }

       if ( victim->blocking > 0 && dam > 149999 && !melee && !( victim->counta == TRUE && victim->blocking < 9 ) )
          {
             block = TRUE;
             int difficulty = ( IS_SET( ch->arena, AFF2_COMBO ) ) ? 50 : 0;        
             int dam_null = 0;

             if ( victim->counta == TRUE )
                {
                   apply_cpush( victim, ch );
                   return;
                }

             if ( victim->topmode == 66 && victim->hit > victim->max_hit * 0.33
                  && victim->hit < victim->max_hit * 0.67 )
                dam_null = 2;
                else if ( !str_cmp( "P", victim->groove ) )
                        dam_null = 1;

             if ( conan == 8 )
                dam_null += 3;

             if ( victim->blocking + ( victim->cmbt[2] < 0 ? 1 : 0 ) + dam_null > 10 && number_percent() > difficulty )
                {
                   if ( !str_cmp( "K", victim->groove ) )
                      {
                         victim->blocking = -10;
                         victim->ctimer[9] = UMIN( 100, victim->ctimer[9] + 25 );
                         victim->hit = UMIN( victim->max_hit, victim->hit + ( victim->max_hit * 0.05 ) );
                         act( "#wYour #ybody #Pflashes a #w#zbrilliant#n #Lblue #yas it completely #Pabsorb #C$N#C's #yattack!!#n", victim, NULL, ch, TO_CHAR );
                         act( "#w$n's #ybody #Pflashes a #w#zbrilliant#n #Lblue #yas it completely #Pabsorb #Cyour #yattack!!#n", victim, NULL, ch, TO_VICT );
                         act( "#w$n's #ybody #Pflashes a #w#zbrilliant#n #Lblue #yas it completely #Pabsorb #C$N#C's #yattack!!#n", victim, NULL, ch, TO_NOTVICT );
                         return;
                      }
                      else if ( !str_cmp( "P", victim->groove ) )
                              {
                                 victim->blocking = -10;

                                 if ( IS_SET( ch->arena, AFF2_DM ) )                      
                                    victim->ctimer[9] = UMIN( 100, victim->ctimer[9] + 33 );

                                 switch( number_range( 1, 3 ) )
                                 {
                                    default:
                                    case 1: act( "#PYou #csay '#wToo slow#c' as you #P#zbrilliantly#n #csidestep #g$N#g's #cvicious attack!!#n", victim, NULL, ch, TO_CHAR );
                                            act( "#P$n #csays '#wToo slow#c' as $e #P#zbrilliantly#n #csidesteps #gyour #cvicious attack!!#n", victim, NULL, ch, TO_VICT );
                                            act( "#P$n #csays '#wToo slow#c' as $e #P#zbrilliantly#n #csidesteps #g$N#g's #cvicious attack!!#n", victim, NULL, ch, TO_NOTVICT );
                                            break;
                                    case 2: act( "#PYou #csnarl '#wWhat was that?#c' as you dodge #g$N#g's #csudden attack #P#zbrilliantly!!#n", victim, NULL, ch, TO_CHAR );
                                            act( "#P$n #csnarls '#wWhat was that?#c' as $e dodge #gyour #csudden attack #P#zbrilliantly!!#n", victim, NULL, ch, TO_VICT );
                                            act( "#P$n #csnarls '#wWhat was that?#c' as $e dodge #g$N#g's #csudden attack #P#zbrilliantly!!#n", victim, NULL, ch, TO_NOTVICT );
                                            break;
                                    case 3: act( "#PYou #cyell '#wForget it!!#c' as you #P#zbrilliantly#n #cpush #g$N #caway from you!!#n", victim, NULL, ch, TO_CHAR );
                                            act( "#P$n #cyells '#wForget it!!#c' as $e #P#zbrilliantly#n #cpushes #gyou #caway from $M!!#n", victim, NULL, ch, TO_VICT );
                                            act( "#P$n #cyells '#wForget it!!#c' as $e #P#zbrilliantly#n #cpushes #g$N #caway from $M!!#n", victim, NULL, ch, TO_NOTVICT );
                                            break;
                                  }

                                  act( "#CYou #othen #Psavagely #ocounter with a #Rflaming #ofist in the #w#zguts#n#o!!!#n", victim, NULL, NULL, TO_CHAR );
                                  act( "#C$n #othen #Psavagely #ocounters with a #Rflaming #ofist in the #w#zguts#n#o!!!#n", victim, NULL, NULL, TO_ROOM );

                                  if ( victim->pkill > 19 )
                                     dam /= 2;
                                     else if ( victim->pkill > 9 )
                                             dam /= 3;
                                             else dam /= 4;
 
                                  victim->attack_type = ATTACK_PHYSICAL;
                                  victim->attack_var = HIT;
                                  victim->attack_focus = DIRECT;
                                  hurt_person( victim, ch, dam, TRUE );
                                  return;
                               }
                               else {
                                       act( "#CYour #gbody #yshimmers #w#zbrilliantly#n #gas you #Rcancel #e$N#e's #gattack swiftly!#n", victim, NULL, ch, TO_CHAR );
                                       act( "#C$n#C's #gbody #yshimmers #w#zbrilliantly#n #gas $e #Rcancels #eyour#e's #gattack swiftly!#n", victim, NULL, ch, TO_VICT );
                                       act( "#C$n#C's #gbody #yshimmers #w#zbrilliantly#n #gas $e #Rcancels #e$N#e's #gattack swiftly!#n", victim, NULL, ch, TO_NOTVICT );
                                       dam /= 2;
                                       victim->blocking = -10;
                                    }
                }
                else {
                        int extra = ( IS_SET( ch->cmbt[5], SKL2_SFIST ) ) ? 10 : 0;

                        if ( !str_cmp( "P", victim->groove ) && IS_SET( ch->arena, AFF2_DM ) )
                           victim->ctimer[9] = UMIN( 100, victim->ctimer[9] + 33 );

                        act( "#yYou #gcrouch on your knees using your arms to #wabsorb #gsome of #C$N#C's #gattack.#n", victim, NULL, ch, TO_CHAR );
                        act( "#y$n #gcrouches on $s knees using $s arms to #wabsorb #gsome of #Cyour #gattack.#n", victim, NULL, ch, TO_VICT );
                        act( "#y$n #gcrouches on $s knees using $s arms to #wabsorb #gsome of #C$N#C's #gattack.#n", victim, NULL, ch, TO_NOTVICT );

                        if ( dam < 500000 )                      
                           victim->block_meter -= URANGE( 5 + ( ( extra == 10 ) ? 10 : 0 ), dam / 20000, 25 );
                           else {
                                   victim->block_meter -= URANGE( 35 + ( extra == 10 ? 35 : 0 ), dam / 20000, 75 );
                                   victim->blocking = 0;
                                }

                        if ( victim->block_meter <= 0 )
                           {
                              act( "#eSuddenly a #yviolent #Rforce #erips your arms apart as you realize your #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_CHAR );
                              act( "#eSuddenly a #yviolent #Rforce #erips $n#e's arms apart as $e realizes $s #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_ROOM );
                              dam *= 2;
                              victim->block_meter = 0;
                              victim->noblock = UMAX( 1, victim->noblock );
                           }
                           else dam /= 2;
                     }
          }

       if ( !IS_NPC( victim ) && victim->ctimer[15] > 0 && dam > 149999 && number_percent() > 70 - eva_bonus( ch, victim ) 
            && !block && rm )
          {
              act( "#wYou #gsuddenly #wfade #gbehind #w$N #gas #wif #gyou #wwere #galways #wthere#g.#n", victim, NULL, ch, TO_CHAR );
              act( "#w$n #gsuddenly #wfades #gbehind #wyou #gas #wif #g$e #wwas #galways #wthere#g.#n", victim, NULL, ch, TO_VICT );
              act( "#w$n #gsuddenly #wfades #gbehind #w$N #gas #wif #gyou #wwas #galways #wthere#g.#n", victim, NULL, ch, TO_NOTVICT );

              switch( number_range( 1, 3 ) )
              {
                 default:
                 case 1: act( "#wTime #gsuddenly #wstops #gas #wyou #gsmash #wyour #gfist #winto #g$S #wback#g!#w!#g!#n", victim, NULL, ch, TO_CHAR );
                         act( "#wTime #gsuddenly #wstops #gas #w$e #gsmashes #w$s #gfist #winto #gyour #wback#g!#w!#g!#n", victim, NULL, ch, TO_VICT );
                         act( "#wTime #gsuddenly #wstops #gas #w$e #gsmashes #w$s #gfist #winto #g$S #wback#g!#w!#g!#n", victim, NULL, ch, TO_NOTVICT );
                         break;
                 case 2: act( "#wTime #gsuddenly #wstops #gas #wyou #gslam #w$M #gonto #wthe #gground #whard#g!#w!#g!#n", victim, NULL, ch, TO_CHAR );
                         act( "#wTime #gsuddenly #wstops #gas #w$e #gslams #wyou #gonto #wthe #gground #whard#g!#w!#g!#n", victim, NULL, ch, TO_VICT );
                         act( "#wTime #gsuddenly #wstops #gas #w$e #gslams #w$M #gonto #wthe #gground #whard#g!#w!#g!#n", victim, NULL, ch, TO_NOTVICT );
                         break;
                 case 3: act( "#wTime #gsuddenly #wstops #gas #wyou #gkick #w$M #gstraight #win #gthe #whead#g!#w!#g!#n", victim, NULL, ch, TO_CHAR );
                         act( "#wTime #gsuddenly #wstops #gas #w$e #gkicks #wyou #gstraight #win #gthe #whead#g!#w!#g!#n", victim, NULL, ch, TO_VICT );
                         act( "#wTime #gsuddenly #wstops #gas #w$e #gkicks #wyou #gstraight #win #gthe #whead#g!#w!#g!#n", victim, NULL, ch, TO_NOTVICT );
                         break;
              }

              victim->hit -= 250000;
              hurt_person( victim, ch, 0, TRUE );
              return;
          }

       if ( IS_NPC( victim ) && victim->pIndexData->vnum == 565 && dam > 149999 && IS_SET( victim->cmbt[0], 4 )
            && !IS_SET( victim->cmbt[0], 2 ) && !block )
          {
             do_tenchi( victim, ch );
             return;
          }

       if ( number_percent() > 84 && dam > 149999 && IS_NPC( victim ) && victim->pIndexData->vnum >= 562
            && victim->pIndexData->vnum < 567 && !block )
          {
             do_mob_counter( victim, ch );
             return;
          }

       if ( number_percent() > 84 && IS_NPC( victim ) && victim->pIndexData->vnum == 566 && dam > 149999 && !block )
          {
             act( "As $N moves in to strike, you suddenly slide towards $M.", victim, NULL, ch, TO_CHAR );
             act( "As you move in to strike, $n suddenly slides towards you.", victim, NULL, ch, TO_VICT );
             act( "As $N moves in to strike, $n suddenly slides towards $M.", victim, NULL, ch, TO_NOTVICT );
             act( "You mutter '#ygracias...#n' as you fade out of sight.", victim, NULL, NULL, TO_CHAR );
             act( "$n mutters '#ygracias...#n' as $e fades out of sight.", victim, NULL, NULL, TO_ROOM );
             act( "You then reappear behind $N as you completely dodge $S attack.", victim, NULL, ch, TO_CHAR );
             act( "$n then reappears behind you as $e completely dodges $S attack.", victim, NULL, ch, TO_VICT );
             act( "$n then reappears behind $N as $e completely dodges $S attack.", victim, NULL, ch, TO_NOTVICT );
             return;
          }

       if ( number_percent() > 84 && IS_NPC( victim ) && victim->pIndexData->vnum == 566 && dam > 149999 && !block
            && victim->in_room->vnum >= 33500 && victim->in_room->vnum <= 33800 )
          {
             act( "As $N moves in to strike, you suddenly slide towards $M.", victim, NULL, ch, TO_CHAR );
             act( "As you move in to strike, $n suddenly slides towards you.", victim, NULL, ch, TO_VICT );
             act( "As $N moves in to strike, $n suddenly slides towards $M.", victim, NULL, ch, TO_NOTVICT );
             act( "You mutter '#yHmph...#n' as you fade out of sight.", victim, NULL, NULL, TO_CHAR );
             act( "$n mutters '#yHmph...#n' as $e fades out of sight.", victim, NULL, NULL, TO_ROOM );
             act( "You then reappear behind $N as you grab $M by the neck!", victim, NULL, ch, TO_CHAR );
             act( "$n then reappears behind you as $e grabs you by the neck!", victim, NULL, ch, TO_VICT );
             act( "$n then reappears behind $N as $e grabs $M by the neck!", victim, NULL, ch, TO_NOTVICT );
             act( "You say '#ePunk#n' as you SLAM your elbow down $N's back!!", victim, NULL, ch, TO_CHAR );
             act( "$n says '#ePunk#n' as $e SLAMS $s elbow down your back!!", victim, NULL, ch, TO_VICT );
             act( "$n says '#ePunk#n' as $e SLAMS $s elbow down $N's back!!", victim, NULL, ch, TO_NOTVICT );

             victim->attack_type = ATTACK_PHYSICAL;
             victim->attack_focus = OVERHEAD;
             victim->attack_var = BLUNT;

             hurt_person( victim, ch, 500000, TRUE );
             return;
          }

       if ( number_percent() > 84 && IS_NPC( victim ) && victim->pIndexData->vnum == 563 && dam > 149999 && !block )
          {
             act( "As $N moves in to strike, you fold your arms as you glare straight at $M.", victim, NULL, ch, TO_CHAR );
             act( "As you move in to strike, $n folds $s arms as $e glares straight at you.", victim, NULL, ch, TO_VICT );
             act( "As $N moves in to strike, $n folds $s arms as $e glares straight at $M.", victim, NULL, ch, TO_NOTVICT );
             act( "$N finds $Mself stuck in midway as you blur towards $M!", victim, NULL, ch, TO_CHAR );
             act( "You finds yourself paralysed in midway as $n blurs towards you!", victim, NULL, ch, TO_VICT );
             act( "$N finds $Mself stuck in midway as $n blurs towards $M!", victim, NULL, ch, TO_NOTVICT );
             act( "You yell '#ySi!#n' kicking $M in the ankle as your images merge back together.", victim, NULL, ch, TO_CHAR );
             act( "$n yells '#ySi!#n' kicking you in the ankle as $s images merge back together.", victim, NULL, ch, TO_VICT );
             act( "$n yells '#ySi!#n' kicking $M in the ankle as $s images merge back together.", victim, NULL, ch, TO_NOTVICT );
             act( "You then crouch forward, staggering $N with a savage punch to the guts.", victim, NULL, ch, TO_CHAR );
             act( "$n then crouches forward, staggering you with a savage punch to the guts.", victim, NULL, ch, TO_VICT );
             act( "$n then crouches forward, staggering $N with a savage punch to the guts.", victim, NULL, ch, TO_NOTVICT );
             act( "Leaning back with a grin, you DrIvE your foot into $N's mouth!", victim, NULL, ch, TO_CHAR );
             act( "Leaning back with a grin, $n DrIvEs $s foot into your mouth!", victim, NULL, ch, TO_VICT );
             act( "Leaning back with a grin, $n DrIvEs $s foot into $N's mouth!", victim, NULL, ch, TO_NOTVICT );
             act( "You yell '#ySinn Ne Das!#n' as you furiously KiCk $N away from you!", victim, NULL, ch, TO_CHAR );
             act( "$n yells '#ySinn Ne Das!#n' as $e furiously KiCks you away from $m!", victim, NULL, ch, TO_VICT );
             act( "$n yells '#ySinn Ne Das!#n' as $e furiously KiCks $N away from $m!", victim, NULL, ch, TO_NOTVICT );

             victim->attack_type = ATTACK_PHYSICAL;
             hurt_person( victim, ch, 750000, TRUE );
             return;
          }

       if ( IS_SET(victim->more2, MORE2_GUARD) && dam > 149999 && !block )
          {
             if ( IS_NPC( victim ) && victim->pIndexData->vnum == 559 ) 
                {
                   act("#eThe #ycrest #earound #Lyou #eshimmers brightly as it nullifies $N#e's attack.\n\r",victim,NULL,ch,TO_CHAR);
                   act("#eThe #ycrest #earound #L$n #eshimmers brightly as it nullifies your #eattack.\n\r",victim,NULL,ch,TO_VICT);
                   act("#eThe #ycrest #earound #L$n #eshimmers brightly as it nullifies $N#e's attack.\n\r",victim,NULL,ch,TO_NOTVICT);
                }
                else if ( !IS_ITEMAFF( victim, ITEMA_SHIELD ) && !IS_CLASS( victim, CLASS_DIVINE_KNIGHT ) )
                {
                   act("#CThe #Lice#C shatters around you, revealing you unscathed.\n\r",victim,NULL,NULL,TO_CHAR);
                   act("#CThe #Lice#C shatters around $n, revealing $m unscathed.\n\r",victim,NULL,NULL,TO_ROOM);
                }
                else {
                        act("#yYou #wcompletely block $N's attack with your #Cshield#w.#n\n\r",victim,NULL,ch,TO_CHAR);
                        act("#y$n #wcompletely blocks your attack with $s #Cshield#w.#n\n\r",victim,NULL,ch,TO_VICT);
                        act("#y$n #wcompletely blocks $N's attack with $s #Cshield#w.#n\n\r",victim,NULL,ch,TO_NOTVICT);

                        if ( is_affected( victim, skill_lookup("holy shield") ) )
                           {
                              AFFECT_DATA af;
     
                              af.type = skill_lookup("holy shield bonus");
                              af.location  = APPLY_CON;
                              af.modifier  = 15;
                              af.duration  = 0;
                              af.bitvector = 0;
                              affect_join( victim, &af );
                           }
                     }

             REMOVE_BIT(victim->more2, MORE2_GUARD);
             return;
          }

       if ( IS_CLASS( victim, CLASS_MONK ) && IS_SET( victim->more3, MORE3_DRAGON ) && dam > 149999 
            && number_percent() > 84 - counter_bonus( ch, victim, at ) && victim->fighting != ch && !block
            && rm )
          {
             act("#yYou #Rclench your body viciously as you absorb $N's blow!#n\n\r",victim,NULL,ch,TO_CHAR);
             act("#y$n #Rclenches $s body viciously as $e absorbs your blow!#n\n\r",victim,NULL,ch,TO_VICT);
             act("#y$n #Rclenches $s body viciously as $e absorbs $N's blow!#n\n\r",victim,NULL,ch,TO_NOTVICT);

             special_slam( ch, number_range( 0, 3 ) );
             victim->attack_type = ATTACK_PHYSICAL;
             hurt_person( victim, ch, 750000, TRUE );

             if ( ch != NULL && ch->in_room != NULL && ch->position > POS_STUNNED )          
                {
                   stop_fighting( ch, TRUE );
                   ch->position = POS_STUNNED;
                }

             return;
          }

       if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_KURUDA ) && IS_SET( victim->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
          {
             act("You look puzzled as you realize that your attack has no effect on $N!",ch,NULL,victim,TO_CHAR);
             act("You grin evilly as $n realizes that $s attack has no effect on you!",ch,NULL,victim,TO_VICT);
             act("$n looks puzzled as $e realizes that $s attack has no effect on $N!",ch,NULL,victim,TO_NOTVICT);
             act("Suddenly, you spasm and shudder as a mass of kiotic energy knocks you away!",ch,NULL,NULL,TO_CHAR);
             act("Suddenly, $n spasms and shudder as a mass of kiotic energy knocks $m away!",ch,NULL,NULL,TO_ROOM);

             ch->wait = 24;

             if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_KURUDA ) && IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
                {
                   REMOVE_BIT( ch->pcdata->powers[MONK_SKILL], MONK_INVIN );
                   ch->ctimer[19] = 0;
                }

             if ( ch->desc != NULL )
                ch->desc->inbuf[0] = '\0';

             victim->attack_type = ATTACK_MAGICAL;
             victim->attack_var = SOUL;
  
             if ( number_percent() > 49 )
                victim->attack_focus = OVERHEAD;
                else victim->attack_focus = LOW;

             hurt_person( victim, ch, 250000, TRUE );
             return;
          }

       if ( IS_CLASS( victim, CLASS_SHADOW_WAR ) && victim->fighting != NULL && victim->fighting != ch && dam > 99999
            && number_percent() > 94 - counter_bonus( ch, victim, at ) && !block && rm )
          {
             act( "You take the full blunt of the blow as the shadows swirl around your hands.", victim, NULL, NULL, TO_CHAR );
             act( "$n takes the full blunt of the blow as the shadows swirl around $s hands.", victim, NULL, NULL, TO_ROOM );
             act( "You scream '#eHadou!#n' as you tilt forward releasing a fireball at $N!", victim, NULL, victim->fighting, TO_CHAR );
             act( "$n screams '#eHadou!#n' as $e tilts forward releasing a fireball at you!", victim, NULL, victim->fighting, TO_VICT );
             act( "$n screams '#eHadou!#n' as $e tilts forward releasing a fireball at $N!", victim, NULL, victim->fighting, TO_NOTVICT );

             victim->attack_type = ATTACK_PHYSICAL;
             victim->attack_var = BLAST;
             victim->attack_focus = DIRECT;

             hurt_person( victim, victim->fighting, dam, TRUE );
             return;
          }

       if ( IS_SET(victim->more2, MORE2_MIRROR) && dam > 0 && !block && rm )
          {
             REMOVE_BIT( victim->more2, MORE2_MIRROR );
             act( "#yYou #wquickly parry #R$N#w's attack as you skip back a foot.#n", victim, NULL, ch, TO_CHAR );
             act( "#y$n #wquickly parries #Ryour#w's attack as $e skips back a foot.#n", victim, NULL, ch, TO_VICT );
             act( "#y$n #wquickly parries #R$N#w's attack as $e skips back a foot.#n", victim, NULL, ch, TO_NOTVICT );
             act( "#yYou #wthen ram your body into #R$N #wsending $M flying!#n", victim, NULL, ch, TO_CHAR );
             act( "#y$n #wthen rams $s body into #Ryou #wsending you flying!#n", victim, NULL, ch, TO_VICT );
             act( "#y$n #wthen rams $s body into #R$N #wsending $M flying!#n", victim, NULL, ch, TO_NOTVICT );
             act( "#yYou #wthen #C#zcleave#n #R$N #wmightily as your sword slices through $S flesh!!!#n", victim, NULL, ch, TO_CHAR );
             act( "#y$n #wthen #C#zcleaves#n #Ryou #wmightily as $s sword slices through your flesh!!!#n", victim, NULL, ch, TO_VICT );
             act( "#y$n #wthen #C#zcleaves#n #R$N #wmightily as $s sword slices through $S flesh!!!#n", victim, NULL, ch, TO_NOTVICT );

             victim->attack_type = ATTACK_PHYSICAL;
             victim->attack_var = SLASH;
             victim->attack_focus = OVERHEAD;

             hurt_person( victim, ch, dam, TRUE );
             return;
          }

       if ( ( ( IS_SET( victim->more3, MORE3_RETORT ) && !chain ) || ( counter && number_percent() > 74 )
            || ( counter && ch->strike_timer > 0 && number_percent() > 34 )
            || ( ch->strike_timer > 0 && number_percent() > 85 - counter_bonus( ch, victim, at ) )
            || ( IS_NPC( victim ) && victim->pIndexData->vnum == 570 && number_percent() > 44 )
            || ( number_percent() > 104 - counter_bonus( ch, victim, at ) ) && !block ) 
            && dam > 150000 && rm )
          {
             switch( number_range( 1, 3 ) )
             {
                default:
                case 1: act( "#yYou #wclench your fists tightly as you brace yourself against #g$N#w's blow.#n", victim, NULL, ch, TO_CHAR );
                        act( "#y$n #wclenches $s fists tightly as $e braces $mself against #gyour #wblow.#n", victim, NULL, ch, TO_VICT );
                        act( "#y$n #wclenches $s fists tightly as $e braces $mself against #g$N's #wblow.#n", victim, NULL, ch, TO_NOTVICT );
                        act( "#yYou #wsuddenly ram your head into $S stomach sending $M sprawling!#n", victim, NULL, ch, TO_CHAR );
                        act( "#y$n #wsuddenly rams $s head into your stomach sending you sprawling!#n", victim, NULL, ch, TO_VICT );
                        act( "#y$n #wsuddenly rams $s head into $S stomach sending $M sprawling!#n", victim, NULL, ch, TO_NOTVICT );
                        victim->attack_focus = LOW;
                        break;
                case 2: act( "#wAs #g$N #wmoves in after the attack, #yyou #wsuddenly spin around kicking $M up the chin!#n", victim, NULL, ch, TO_CHAR );
                        act( "#wAs #gyou #wmove in after the attack, #y$n #wsuddenly spins around kicking you up the chin!#n", victim, NULL, ch, TO_VICT );
                        act( "#wAs #g$N #wmoves in after the attack, #y$n #wsuddenly spins around kicking $M up the chin!#n", victim, NULL, ch, TO_NOTVICT );
                        act( "#yYou #wthen give $M an overhead kick causing $M to crash into the ground hard!#n", victim, NULL, ch, TO_CHAR );
                        act( "#y$n #wthen gives you an overhead kick causing you to crash into the ground hard!#n", victim, NULL, ch, TO_VICT );
                        act( "#y$n #wthen gives $M an overhead kick causing $M to crash into the ground hard!#n", victim, NULL, ch, TO_NOTVICT );
                        victim->attack_focus = OVERHEAD;
                        break;
                case 3: act("#yYou #wsuddenly yell '#CUMMM!#w' as you tilt forward blocking #g$N#w's attack with your shoulder.#n",victim,NULL,ch,TO_CHAR);
                        act("#y$n #wsuddenly yells '#CUMMM!#w' as $e tilts forward blocking #gyour #wattack with $s shoulder.#n",victim,NULL,ch,TO_VICT);
                        act("#y$n #wsuddenly yells '#CUMMM!#w' as $e tilts forward blocking #g$N#w's attack with $s shoulder.#n",victim,NULL,ch,TO_NOTVICT);
                        act("#yYou #wexclaim '#CJackpai!#w' as you dash forward smashing your elbow into $S stomach.#n",victim,NULL,ch,TO_CHAR);
                        act("#y$n #wexclaims '#CJackpai!#w' as $e dashes forward smashing $s elbow into your stomach.#n",victim,NULL,ch,TO_VICT);
                        act("#y$n #wexclaims '#CJackpai!#w' as $e dashes forward smashing $s elbow into $S stomach.#n",victim,NULL,ch,TO_NOTVICT);
                        victim->attack_focus = DIRECT;
                        break;
             }

             if ( IS_NPC( victim ) && victim->pIndexData->vnum == 570 )
                ; 
                else if ( IS_SET( victim->more3, MORE3_RETORT ) )
                {
                   REMOVE_BIT( victim->more3, MORE3_RETORT );
                   dam = 500000 + ( dam / 2 );
                }
                else dam = 500000;

             victim->attack_type = ATTACK_PHYSICAL;
             victim->attack_var = HIT;
             hurt_person( victim, ch, dam, TRUE );
             return;
          }
     }

  if ( ch->fighting == NULL )
     {
        if ( ch->fight_timer == 0 && !IS_NPC( ch ) && !IS_NPC( victim ) && IS_SET( ch->more3, MORE3_FALCON ) )
           dam += 500000;

        set_fighting( ch, victim );
     }
 
  if ( victim->fighting == NULL )
     set_fighting( victim, ch );

  if ( IS_CLASS(victim, CLASS_ASSASSIN) && IS_SET( victim->special, SPC_KNIGHT ) && victim->pkill > 19 )
     total -= 1000000;

  if ( IS_CLASS(victim, CLASS_FIEND) && IS_SET( victim->more, MORE_WALL ) )
     total -= 1000000;

  if ( IS_SET( victim->more3, MORE3_DRAGON ) )
     total -= 750000;

  if ( victim->position == POS_STUNNED && victim->hit > 0 - total && ( !str_cmp( "C", victim->groove ) 
       || !str_cmp( "K", victim->groove ) || !str_cmp( "M", victim->groove ) ) )
     return;

  if ( !IS_SET( ch->arena, AFF2_UB ) && ch->attack_type != 0 )
     {
        dam = is_attack_type( ch, victim, dam );
        ch->attack_type = 0;
     }

  if ( !IS_SET( ch->arena, AFF2_UB ) && ( ch->revdam > 0 || victim->revdam > 0 ) )
     {
        CHAR_DATA *temp;

        temp = ch;
        victim = ch;
        ch = temp;
     }


  if ( ( ( IS_SET( victim->more3, MORE3_DODGE ) && !chain ) 
       || ( ch->strike_timer > 0 && number_percent() > 85 - eva_bonus( ch, victim ) )
       || ( number_percent() > 95 - eva_bonus( ch, victim ) )
       || ( IS_NPC( victim ) && victim->pIndexData->vnum == 570 && number_percent() > 89 ) )
       && dam > 149999 && !block )
     {
        REMOVE_BIT( victim->more3, MORE3_DODGE );
 
        if ( number_range( 1, 2 ) == 1 )
           {
              act( "#pAt the very last second, #cyou #proll away to the side dodging #y$N#p's attack!#n", victim, NULL, ch, TO_CHAR );
              act( "#pAt the very last second, #c$n #prolls away to the side dodging #yyour #pattack!#n", victim, NULL, ch, TO_VICT );
              act( "#pAt the very last second, #c$n #prolls away to the side dodging #y$N's #pattack!#n", victim, NULL, ch, TO_NOTVICT );
           }
           else {
                   act( "#cYou #pquickly step aside as #y$N#p's attack glances by you!#n", victim, NULL, ch, TO_CHAR );
                   act( "#c$n #pquickly steps aside as #yyour #pattack glances by $m!#n", victim, NULL, ch, TO_VICT );
                   act( "#c$n #pquickly steps aside as #y$N#p's attack glances by $m!#n", victim, NULL, ch, TO_NOTVICT );
                }

        return;
     }

  if ( !IS_NPC( victim ) && victim->pcdata->deity == DEI_SHIVA && number_percent() > 84 - devil_luck( victim, ch, 5 )
       && dam > 149999 && !block && victim->null_timer == 0 )
     {
        victim->null_timer = 20;
        act( "#CYou #wtransform into a #ysplash #wof #Cwater #was you #Pswish #wthrough $N#w's attack!#n", victim, NULL, ch, TO_CHAR );
        act( "#C$n #wtransforms into a #ysplash #wof #Cwater #was $e #Pswishes #wthrough your attack!#n", victim, NULL, ch, TO_VICT );
        act( "#C$n #wtransforms into a #ysplash #wof #Cwater #was $e #Pswishes #wthrough $N#w's attack!#n", victim, NULL, ch, TO_NOTVICT );
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
           case 0: break;

           default: mode = TRUE;
                    break;

           case 1: mode = TRUE;
                   break;

           case 2: mode = TRUE;
                   break;
        }
     }

  if ( ( IS_SET( victim->arena, AFF2_TEAM1 ) || IS_SET( victim->arena, AFF2_TEAM2 ) ) 
       && IS_SET( victim->arena, AFF2_INARENA ) )
     {
        int team;

        if ( IS_SET( victim->arena, AFF2_TEAM1 ) )
           team = 0;
           else team = 1;

        switch( team_list->power_mode[team] )
        {
           case 2: armor = TRUE;                   
                   break;

           case 3: counter = TRUE;
                   break;

           case 4: chain = TRUE;
                   break;

           case 5: if ( team_list->power_duration[team] > 7 )
                      dam = 0;
        }
     }

  if ( agg_dam == FALSE && number_percent() > 60 )
     agg_dam = TRUE;

  if ( IS_SET( ch->arena, AFF2_POWER ) && IS_CLASS( victim, CLASS_MASTER ) && !IS_CLASS( ch, CLASS_MASTER ) )
     dam /= 2;

  if ( !IS_SET( ch->arena, AFF2_UB ) && IS_SET( victim->cmbt[7], SKL_BLOCK ) && dam >= 900000 )
     {
        if ( !IS_NPC( victim ) && victim->pcdata->deity == DEI_SIREN )
           {
              if ( dam >= 2500000 )
                 dam -= 750000;
                 else if ( dam >= 2000000 )
                         dam -= 450000;
                 else if ( dam >= 1500000 )
                         dam -= 300000;
                         else dam -= 150000;
           }
           else {
                   if ( dam >= 2500000 )
                      dam -= 500000;
                      else if ( dam >= 2000000 )
                              dam -= 300000;
                      else if ( dam >= 1500000 )
                              dam -= 200000;
                              else dam -= 100000;
                }
     }

  if ( dam >= 500000 && ( is_affected( ch, skill_lookup( "divine might" ) ) || ( IS_SET( ch->more3, MORE3_TIGER ) 
       && number_percent() > 84 ) ) )
     {
        act("#yYou #wscream as the #Cpower #wof the #yHALO #wrips your body apart!#n",victim,NULL,NULL,TO_CHAR);
        act("#y$n #wscreams as the #Cpower #wof the #yHALO #wrips $s body apart!#n",victim,NULL,NULL,TO_ROOM);
        dam *= 1.25;
     }

  if ( dam >= 500000 && number_percent() > 100 - jaggareffect )
     {
        act("#P$N #escreams as #wyou #eSeAr through $S #oflesh #ewith a #y#zcritical#n #eattack!!#n",ch,NULL,victim,TO_CHAR);
        act("#PYou #escream as #w$n #eSeArS through your #oflesh #ewith a #y#zcritical#n #eattack!!#n",ch,NULL,victim,TO_VICT);
        act("#P$N #escreams as #w$n #eSeArS through $S #oflesh #ewith a #y#zcritical#n #eattack!!#n",ch,NULL,victim,TO_NOTVICT);
        dam *= 1.33;
     }

  if ( !IS_NPC( ch ) && ch->pcdata->blood_power == 5 && dam > 149999 )
     dam += UMIN( 500000, dam / 2 );

  if ( ch->strike_timer > 0 && dam > victim->max_hit * 0.075 )
     dam = victim->max_hit * 0.075;

  if ( !IS_SET( ch->arena, AFF2_UB ) && !IS_SET( ch->arena, AFF2_DD ) )
     {
        if ( armor == TRUE && dam > 1000000 )
           dam = 1000000;
           else if ( IS_NPC( victim ) && victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum < 571 && dam > 3500000 )
                   dam = 3500000;
           else if ( ( IS_SET( victim->more3, MORE3_CRANE ) || IS_CLASS( victim, CLASS_DRAGON ) ) && dam > 5000000 )
                   dam = 5000000;
           else if ( IS_SET( victim->more, MORE_BARRIER ) && dam > 4999999 )
                   {
                      if ( IS_CLASS( victim, CLASS_ASSASSIN ) && IS_SET( victim->special, SPC_KNIGHT )
                           && victim->pkill > 29 && victim->class_2 == 0 )
                         {
                            act( "#wYou #Lcoldly #eturn your head to the side as #g$N#g's #eattack grazes by you!#n", victim, NULL, ch, TO_CHAR );
                            act( "#w$n #Lcoldly #eturns $s head to the side as #gyour #eattack grazes by $m!#n", victim, NULL, ch, TO_VICT );
                            act( "#w$n #Lcoldly #eturns $s head to the side as #g$N#g's #eattack grazes by $m!#n", victim, NULL, ch, TO_NOTVICT );
                            return;
                         }

                      dam = 5000000;
                   }
           else if ( ch->pkill > 29 && IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_KNIGHT )
                    && IS_SET( ch->more, MORE_BARRIER ) && dam > 4750000 )
                   dam = 4750000;
     }

  if ( !IS_SET( ch->arena, AFF2_UB ) && IS_SET( ch->extra, EXTRA_SUBDUE ) && !melee && mydam > 149999 )
     {
        REMOVE_BIT( ch->extra, EXTRA_SUBDUE );
        dam = 1;
     }
     else if ( !IS_NPC( ch ) && ch->ctimer[15] > 0 && dam > 100000 )
             dam = UMIN( 150000, 100000 + ( ch->pkill * 2500 ) );

  if ( ( IS_CLASS( victim, CLASS_ANGEL ) || IS_CLASS( victim, CLASS_SHADOW_WAR ) ) && victim->ctimer[17] > 0 )
     {
        if ( victim->hit > 5000000 )
           victim->ctimer[16] += ( dam * 1.5 );
           else if ( victim->hit > 2500000 )
                   victim->ctimer[16] += ( dam * 1.25 );
                   else victim->ctimer[16] += ( dam * 0.75 );

        return;
     }

  if ( str_cmp( "", ch->groove ) && str_cmp( "K", ch->groove )
       && !( IS_NPC( victim ) && ( victim->pIndexData->vnum < 550 
       || victim->pIndexData->vnum > 570 ) ) )
     add_grove( ch, victim, dam );

  if ( !IS_SET( ch->arena, AFF2_UB ) && dam > 299999 && --altima == 0 )
     {
        do_altima( victim, ch );
        return;
     } 

  if ( !IS_SET( ch->arena, AFF2_UB ) && !block && victim->noblock == 0 && dam > 149999 && number_percent() > 89 - block_bonus( ch, victim, at ) )
     {
        if ( !str_cmp( "P", victim->groove ) && IS_SET( ch->arena, AFF2_DM ) )
           victim->ctimer[9] = UMIN( 100, victim->ctimer[9] + 33 );

        act( "#yYou #gquickly use your arms to #wdampen #gsome of #C$N#C's #gattack.#n", victim, NULL, ch, TO_CHAR );
        act( "#y$n #gquickly uses $s arms to #wdampen #gsome of #Cyour #gattack.#n", victim, NULL, ch, TO_VICT );
        act( "#y$n #gquickly uses $s arms to #wdampen #gsome of #C$N#C's #gattack.#n", victim, NULL, ch, TO_NOTVICT );
        dam /= 2;
     }

  if ( IS_SET( ch->extra, EXTRA_DETERMINE ) && !melee && mydam > 149999 )
     {
        sprintf( buf, "#g<+<Attack Type: %s|#gElemental: %s|#gVariant: %s|#gFocus: %s|#gDamage: (#R%d#g)>+>#n\n\r",
                 show_type( at ), show_elem( ele ), show_var( ch->attack_var ),
                 show_focus( ch->attack_focus ), dam );
        stc( buf, ch );
     }

  if ( IS_SET( victim->extra, EXTRA_DETERMINE ) && !melee && mydam > 149999 )
     {
        sprintf( buf, "#g<+<Attack Type: %s|#gElemental: %s|#gVariant: %s|#gFocus: %s|#gDamage: (#R%d#g)>+>#n\n\r",
                 show_type( at ), show_elem( ele ), show_var( ch->attack_var ),
                 show_focus( ch->attack_focus ), dam );
        stc( buf, victim );
     }

  if ( !IS_SET( ch->arena, AFF2_UB ) && IS_CLASS(victim, CLASS_ANGEL) && IS_SET(victim->more, MORE_MSHIELD) )
     {
        victim->mana = UMAX( 0, victim->mana - ( dam * 20 ) );

        if ( victim->mana == 0 || IS_AFFECTED( victim, AFF_MANA ) )
           do_mana_shield( victim, "" );
     }
     else { 
             if ( !str_cmp( "K", victim->groove )
                  && !( IS_NPC( ch ) && ( ch->pIndexData->vnum < 550 
                  || ch->pIndexData->vnum > 570 ) ) )
                add_grove( victim, ch, dam );

             victim->hit -= dam;

             if ( ( victim->topmode == 33 && victim->hit < victim->max_hit * 0.34
                  && old_hp >= victim->max_hit * 0.34 ) || ( victim->topmode == 66 
                  && victim->hit < victim->max_hit * 0.67 && old_hp >= victim->max_hit * 0.67 ) )
                {
                    stc( "#wT#e.#CO#e.#RP#e. #P#zIN#n#g!!!#n\n\r", victim );
                    act( "#c$n #gclenches $s fists as $e #P#zactivates#n #g$s #wT#e.#CO#e.#RP#e. #gmode!#n", victim, NULL, NULL, TO_ROOM );
                }

             if ( victim->hit > total && victim->anger_level < 300 && dam > 199999 )
                {
                   int gang_bonus = ( victim->fighting != ch && !IS_NPC( ch ) ) ? 3 : 1;
                   int adder;
                   
                   adder = UMAX( 1, dam / 1000000 ) * ( get_curr_tens( victim ) / 3 ) * gang_bonus;
                   victim->anger_level = UMIN( 300, victim->anger_level + adder );

                   if ( victim->anger_level == 300 )
                      {
                         act( "#PYour #warms #Cburst #winto #Rflames #was you swing your weapon #g#zviolently#n#w!#n", victim, NULL, NULL, TO_CHAR );
                         act( "#P$n's #warms #Cburst #winto #Rflames #was $e swings $s weapon #g#zviolently#n#w!#n", victim, NULL, NULL, TO_ROOM );
                         victim->anger_level = 306;
                      }
                }
          }

  if ( !IS_NPC( victim ) && victim->pcdata->watching != NULL )
     {
        act( "You stop watching $N.", victim, NULL, victim->pcdata->watching, TO_CHAR );
        victim->pcdata->watching = NULL;
     }

  if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER )
       && ch->pcdata->powers[DRAGOON_SPIRIT] < ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
     {
        int drg_bonus = 0;

        drg_bonus = UMIN( dam * 0.000005 * ch->pcdata->powers[DRAGOON_LEVEL], ch->pcdata->powers[DRAGOON_LEVEL] * 5 );
        ch->pcdata->powers[DRAGOON_SPIRIT] = UMIN( ch->pcdata->powers[DRAGOON_LEVEL] * 100, ch->pcdata->powers[DRAGOON_SPIRIT] + drg_bonus );
     }

  if ( IS_CLASS( victim, CLASS_ASSASSIN ) && IS_SET( victim->special, SPC_SOR ) && victim->pkill > 10
       && victim->hit < victim->max_hit * 0.2 && !is_affected( victim, skill_lookup("magiwall") ) )
     spell_magi_wall( skill_lookup("magiwall"), 3, victim, victim );

  if ( IS_SET( victim->more2, MORE2_DIG ) )
     REMOVE_BIT( victim->more2, MORE2_DIG );

  if( IS_NPC(victim) )
  {
    switch( victim->pIndexData->vnum )
    {
      case 20:
      case 21:
      case 17505:
      case 4505:
      case 7505:
      case 16505:
      case 14505:
      case 15505:
      case 3505:
      case 2505: if ( victim->hit <= 0 ) 
                    victim->hit = victim->max_hit;
    }
  }

  if ( !IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL && victim->hit < total + 1 )
     victim->hit = total + 1;
     else if ( victim == ch && !IS_NPC(ch) && ch->in_room->vnum >= 550 && ch->in_room->vnum < 630
               && ch->hit < total + 1 )
             ch->hit = total + 1;

  if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DRAGON) && IS_DRAGONPOWER(ch,DRAGON_CRUSH) )
     victim->agg = 100;  
     else if ( get_kingdom_num( ch ) != -1 || victim->agg > 100 )
             victim->agg = 100;
     else if ( agg_dam && victim->agg < 100 )
             victim->agg = UMIN( 100, victim->agg + 30 );

  if ( IS_NPC( victim ) && victim->pIndexData->vnum == 569 && victim->hit < total
       && !( IS_SET( ch->arena, AFF2_DM ) && dam > 1999999 ) )
     {
        victim->hit = total + 1;

        if ( dam > 999999 )
           {
              act( "#CYou #ggrin #eevilly #gas you stand up and #Pswerve #gyour arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#C$n #ggrins #eevilly #gas $e stands up and #Pswerves #g$s arms upwards.#n", victim, NULL, NULL, TO_ROOM );
              act( "#eA replica of you grins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#eA replica of $n #egrins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_ROOM );
              act( "#eA replica of you grins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#eA replica of $n #egrins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_ROOM );
              act( "#eA replica of you grins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#eA replica of $n #egrins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_ROOM );
           }

        return;
     }

  if ( !IS_NPC( victim ) && IS_SET( victim->pcdata->hidden_skill, 16 ) && victim->hit < total
       && !IS_SET( ch->arena, AFF2_DM ) && !IS_SET( victim->in_room->room_flags, ROOM_NOSTOCK ) )
     {
        victim->hit = total + 1;

        if ( dam > 999999 )
           {
              act( "#CYou #ggrin #eevilly #gas you stand up and #Pswerve #gyour arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#C$n #ggrins #eevilly #gas $e stands up and #Pswerves #g$s arms upwards.#n", victim, NULL, NULL, TO_ROOM );
              act( "#eA replica of you grins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#eA replica of $n #egrins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_ROOM );
              act( "#eA replica of you grins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#eA replica of $n #egrins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_ROOM );
              act( "#eA replica of you grins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_CHAR );
              act( "#eA replica of $n #egrins evilly as it stands up and swerves its arms upwards.#n", victim, NULL, NULL, TO_ROOM );
           }

        return;
     }

  if ( !IS_NPC(victim) && IS_CLASS(victim,CLASS_PHOENIX) && victim->pkill > 9 && victim->ctimer[1] == 0 
       && victim->hit <= total && victim->pcdata->capture_timer == 0 && victim->position >= POS_STUNNED )
     {
          CHAR_DATA *vch, *vch_next;

          act("You suddenly fly a few feet above your foes as you glance around you angrilly.",victim,NULL,NULL,TO_CHAR);
          act("$n suddenly flies a few feet above $s foes as $e glances around $m angrilly.",victim,NULL,NULL,TO_ROOM);
          act("Time seem to stand still as a circle of passive flames encircle the vicinity...",victim,NULL,NULL,TO_CHAR);
          act("Time seem to stand still as a circle of passive flames encircle the vicinity...",victim,NULL,NULL,TO_ROOM);
          act("You then frighten everyone with a #CLOUD #wSCREECH#n as the #yFLAMES#n blast everyone away!!",victim,NULL,NULL,TO_CHAR);
          act("$n then frightens everyone with a #CLOUD #wSCREECH#n as the #yFLAMES#n blast everyone away!!",victim,NULL,NULL,TO_ROOM);

          victim->hit = 1000000 + total;
          victim->ctimer[1] = 4;

          for ( vch = victim->in_room->people; vch != NULL; vch = vch_next )
          {
             vch_next = vch->next_in_room;

             if ( is_safe( victim, vch ) )
                continue;

             stop_fighting( vch, TRUE );
             vch->air_timer = 1;
          }

          stop_fighting( ch, TRUE );
          return;
     }

  if ( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DROW) && victim->ctimer[2] == 0 && victim->hit <= total
       && victim->pcdata->capture_timer == 0 && victim->position >= POS_STUNNED )
     {
          ROOM_INDEX_DATA *was_in;
          ROOM_INDEX_DATA *now_in;
          int attempt;
  	    EXIT_DATA *pexit;
	    int door;

          was_in = victim->in_room;
          victim->hit = 1000000 + total;
          victim->ctimer[2] = 4;

          if ( IS_SET(victim->more, MORE_IMPALE) )
             {
                send_to_char("You try to move but your foot is impaled into the ground.\n\r",victim);
                return;
             }

          if ( IS_AFFECTED(victim, AFF_WEBBED) )
             {
                send_to_char("You are unable to move with all this sticky webbing on.\n\r",victim);
                return;
             }

          if ( IS_SET(victim->more, MORE_NET) )
             {
                send_to_char( "You are trapped in a spiritual net.\n\r", victim );
                return;
             }

          if ( is_memb(victim) )
             return;

          for ( attempt =  0; attempt < 6; attempt++ ) 
	    {
	       door = number_door( );
	       if ( ( pexit = was_in->exit[door] ) == 0
	       ||   pexit->to_room == NULL
	       ||   IS_SET(pexit->exit_info, EX_CLOSED)
	       || ( IS_NPC(victim)
	       &&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
	       continue;

	       move_char( victim, door );
	       if ( ( now_in = victim->in_room ) == was_in )
	          continue;

             victim->in_room = was_in;

 	       act( "$n quickly jumps back to $s feet as $e staggers away .", victim, NULL, NULL, TO_ROOM );
  	       add_tracks( victim, door );
             victim->in_room = now_in;
             attempt = 9;
          }
	
          if ( !IS_NPC(victim) )
	       send_to_char( "You quickly jump to your feet as you stagger away.\n\r", victim ); 

          stop_fighting( victim, TRUE );

	  if ( IS_SET(victim->newbits, NEW_DARKNESS) )
             {
 	        REMOVE_BIT(victim->newbits, NEW_DARKNESS);
                victim->in_room->drows--;

                if ( victim->in_room->drows == 0 )
                   REMOVE_BIT(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS);
             }

          return;
      }

  if ( ( IS_CLASS( victim, CLASS_NINJA ) || IS_CLASS( victim, CLASS_SHADOW ) ) && victim->hit <= total
       && victim->ctimer[3] == 0 && victim->pkill > 19 && victim->position > POS_STUNNED
       && victim->pcdata->capture_timer == 0 )
     {
        act("As you slump towards the ground, you suddenly flick out a dagger throwing it at $N's throat!",victim,NULL,ch,TO_CHAR);
        act("As $n slumps towards the ground, $e suddenly flicks out a dagger throwing it at your throat!",victim,NULL,ch,TO_VICT);
        act("As $n slumps towards the ground, $e suddenly flicks out a dagger throwing it at $N's throat!",victim,NULL,ch,TO_NOTVICT);

        victim->ctimer[3] = 4;
        victim->attack_type = ATTACK_PHYSICAL;
        victim->attack_var = PIERCE;
        victim->attack_focus = DIRECT;

        if ( !IS_CLASS( victim, CLASS_NINJA ) )
           hurt_person( victim, ch, 750000, TRUE );
           else hurt_person( victim, ch, 250000, TRUE );
          
        victim->hit = 1000000 + total;
        act("You suddenly backflip onto your feet as your body radiates greenish energy.",victim,NULL,NULL,TO_CHAR);
        act("$n suddenly backflips onto $s feet as $s body radiates greenish energy.",victim,NULL,NULL,TO_ROOM);
        return;
     }

  if ( IS_SET( victim->cmbt[5], SKL2_FBLOW ) && !IS_SET( ch->cmbt[5], SKL2_FBLOW ) && victim->hit <= total 
       && victim->position > POS_STUNNED && victim->pcdata->capture_timer == 0 )
     {
        act("As you slump towards the ground, you suddenly flick out a dagger throwing it at $N's throat!",victim,NULL,ch,TO_CHAR);
        act("As $n slumps towards the ground, $e suddenly flicks out a dagger throwing it at your throat!",victim,NULL,ch,TO_VICT);
        act("As $n slumps towards the ground, $e suddenly flicks out a dagger throwing it at $N's throat!",victim,NULL,ch,TO_NOTVICT);

        victim->attack_type = ATTACK_PHYSICAL;
        victim->attack_var = PIERCE;
        victim->attack_focus = DIRECT;

        if ( !IS_NPC( victim ) && victim->pcdata->deity == DEI_DIABLOS )
           hurt_person( victim, ch, 1000000, TRUE );
           else hurt_person( victim, ch, 500000, TRUE );
     }

  if ( IS_CLASS( victim, CLASS_SHADOW_WAR ) && !IS_CLASS( ch, CLASS_SHADOW_WAR ) && victim->hit <= total 
       && victim->pkill > 9 && victim->position >= POS_STUNNED && victim->pcdata->capture_timer == 0 )
     {
        act("As you slump towards the ground, you suddenly release a dark flash of light at $N!",victim,NULL,ch,TO_CHAR);
        act("As $n slumps towards the ground, $e suddenly releases a dark flash of light at you!",victim,NULL,ch,TO_VICT);
        act("As $n slumps towards the ground, $e suddenly releases a dark flash of light at $N!",victim,NULL,ch,TO_NOTVICT);

        if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_KURUDA ) && IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
           {
              REMOVE_BIT( ch->pcdata->powers[MONK_SKILL], MONK_INVIN );
              ch->ctimer[19] = 0;
           }

        victim->attack_type = ATTACK_PHYSICAL;
        victim->ele_attack = ATTACK_DARK;
        victim->attack_var = BLAST;
        victim->attack_focus = UPPERCUT;
        hurt_person( victim, ch, 750000, TRUE );

        if ( ch == NULL || ch->in_room == NULL || ch->position <= POS_STUNNED )
           {
              victim->hit = 1000000 + total;
              return;
           }
     }

  if ( ( victim->outer_aura == A_SALVATION || victim->inner_aura == A_SALVATION )
       && !IS_NPC( victim ) && victim->hit <= total && victim->position >= POS_STUNNED && victim->defiance_timer == 0 )
     {
        int level = 0;

        if ( victim->inner_aura == A_SALVATION )
           level += 1;

        level += victim->aura[A_SALVATION];

        if ( level > 10 )
           level = 10;

        victim->hit = ( 100000 * level ) + total;
        victim->defiance_timer = 100;
        stop_fighting( victim, TRUE );
        act( "As you collapse to the ground from $N's finishing blow, your aura suddenly radiates intensely.", victim, NULL, ch, TO_CHAR );
        act( "As $n collapses to the ground from your finishing blow, $s aura suddenly radiates intensely.", victim, NULL, ch, TO_VICT );
        act( "As $n collapses to the ground from $N's finishing blow, $s aura suddenly radiates intensely.", victim, NULL, ch, TO_NOTVICT );
        act( "You see a flicker of a being hovering before you as it speaks.", victim, NULL, NULL, TO_CHAR );
        act( "You see a flicker of a being hovering before $n as it speaks.", victim, NULL, NULL, TO_ROOM );
        act( "'#yThou must continue to walk the path of good... and defy evil.#n'", victim, NULL, NULL, TO_CHAR );
        act( "'#yThou must continue to walk the path of good... and defy evil.#n'", victim, NULL, NULL, TO_ROOM );
        act( "Suddenly, you feel tremedous energy soaring through your body, reviving you.", victim, NULL, NULL, TO_CHAR );
        act( "Suddenly, $n feels tremedous energy soaring through $s body, reviving $m.", victim, NULL, NULL, TO_ROOM );
        return;
     }

  if ( IS_NPC( victim ) && victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum < 571 && victim->hit < total
       && number_percent() > 89 )
     {
        victim->position = POS_MORTAL;
        do_emergency_flip( victim, "" );
        return;
     }

  if ( IS_SET(victim->arena, AFF2_INARENA) && victim->hit < total && ( IS_NPC( victim )
       || ( !IS_NPC(victim) && victim->pcdata->team == 0 ) ) )
     {
        if ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) )
           finish_round( victim, ch );
           else if ( !IS_NPC(ch) || IS_SET( ch->arena, AFF2_INARENA ) )
                   clean_arena( victim, ch );
           else if ( IS_NPC(ch) && ch->master != NULL && !IS_NPC(ch->master) )
                   clean_arena( victim, ch->master );
                   else victim->hit = 1;

        return;
     }

  update_pos( victim );

  switch( victim->position )
  {
    case POS_MORTAL:

    if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->mergence == 0 )
       ch->mergence = 10;

    if ( !armageddon )
       {
          if ( IS_SET(ch->newbits, NEW_DARKNESS) )
             {
                 REMOVE_BIT(ch->newbits, NEW_DARKNESS);
                 ch->in_room->drows--;
             }

          if ( IS_SET(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
             REMOVE_BIT(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS);
       }

    remove_stuff( victim );

    act( "$n is mortally wounded, and spraying blood everywhere.", victim, NULL, NULL, TO_ROOM );
    send_to_char("You are mortally wounded, and spraying blood everywhere.\n\r", victim );

    if ( get_kingdom_num( ch ) != -1 && ch->in_room == victim->in_room )
       capture_person( ch, victim );

    break;
    case POS_INCAP:

    if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->mergence == 0 )
       ch->mergence = 10;

    if ( !armageddon )
       {
          if ( IS_SET(ch->newbits, NEW_DARKNESS) )
             {
                 REMOVE_BIT(ch->newbits, NEW_DARKNESS);
                 ch->in_room->drows--;
             }

          if ( IS_SET(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
             REMOVE_BIT(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS);
       }

    remove_stuff( victim );

    act( "$n is incapacitated, and bleeding badly.",victim, NULL, NULL, TO_ROOM );
    send_to_char("You are incapacitated, and bleeding badly.\n\r",victim );

    if ( get_kingdom_num( ch ) != -1 && ch->in_room == victim->in_room )
       capture_person( ch, victim );

    break;
    case POS_STUNNED:
    act( "$n is stunned, but will soon recover.",victim, NULL, NULL, TO_ROOM );
    send_to_char("You are stunned, but will soon recover.\n\r", victim );
    break;
    case POS_DEAD:

    if ( !( IS_NPC( victim ) && victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum <= 570 ) )
       {
          act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
          send_to_char( "You have been KILLED!!\n\r\n\r", victim );
       }
    break;
    default:
    if ( ch != victim && !IS_NPC(ch) && !IS_NPC(victim) )
       {
          ch->fight_timer = 20;
          victim->fight_timer = 20;
          ch->safe_timer = 0;
       }

    if ( !IS_SET( victim->act, PLR_BRIEF2 ) && dam > victim->max_hit / 4 )
      send_to_char( "That really did HURT!\n\r", victim );
    if ( !IS_SET( victim->act, PLR_BRIEF2 ) && victim->hit < victim->max_hit / 10 && dam > 0 )
       send_to_char( "You sure are BLEEDING!\n\r", victim );

    break;
  }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) )
	stop_fighting( victim, TRUE );

    if ( victim->in_room != ch->in_room && victim->fighting == ch )
       stop_fighting( victim, TRUE );

    if ( ch->fighting == ch )
       stop_fighting( ch, FALSE );

    if ( !IS_NPC( victim ) && victim->position == POS_FIGHTING && number_percent() > 79 && dam > 499999 )
       {
          switch( number_range( 1, 7 ) )
          {
             default:
             case 1: act( "You snarl '#yArrrgh#n' as you skid back a few feet.", victim, NULL, NULL, TO_CHAR );
                     act( "$n snarls '#yArrrgh#n' as $e skids back a few feet.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 2: act( "You exclaim '#yYou son of a ...!#n' as you clutch your arm in pain.", victim, NULL, NULL, TO_CHAR );
                     act( "$n exclaims '#yYou son of a ...!#n' as $e clutches $s arm in pain.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 3: act( "You mutter '#yHrmph...#n' as you glance around for incoming danger.", victim, NULL, NULL, TO_CHAR );
                     act( "$n mutters '#yHrmph...#n' as $e glances around for incoming danger.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 4: act( "You ask '#yWhat was that?#n' as you check yourself for injuries.", victim, NULL, NULL, TO_CHAR );
                     act( "$n asks '#yWhat was that?#n' as $e checks $mself for injuries.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 5: act( "You snarl '#yWhat impulence!#n' as you shrug away $N's attack.", victim, NULL, ch, TO_CHAR );
                     act( "$n snarls '#yWhat impulence!#n' as $e shrugs away your attack.", victim, NULL, ch, TO_VICT );
                     act( "$n snarls '#yWhat impulence!#n' as $e shrugs away $N's attack.", victim, NULL, ch, TO_NOTVICT );
                     break;

             case 6: act( "You ask '#yIs that all you got?#n' as you check yourself for injuries.", victim, NULL, NULL, TO_CHAR );
                     act( "$n asks '#yIs that all you got?#n' as $e checks $mself for injuries.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 7: act( "You exclaim '#yMuhahaaaa too weak!#n' as you stand proudly for battle.", victim, NULL, NULL, TO_CHAR );
                     act( "$n exclaims '#yMuhahaaaa too weak!#n' as $e stands proudly for battle.", victim, NULL, NULL, TO_ROOM );
                     break;
          }

          switch( number_range( 1, 7 ) )
          {
             default:
             case 1: act( "You exclaim '#yDon't get so excited about that!#n' as you leap back in the fray.", victim, NULL, NULL, TO_CHAR );
                     act( "$n exclaims '#yDon't get so excited about that!#n' as $e leaps back in the fray.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 2: act( "You snarl '#yHit harder...#n' as you beg for more pain.", victim, NULL, NULL, TO_CHAR );
                     act( "$n snarls '#yHit harder...#n' as $e begs for more pain.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 3: act( "You mutter '#yNext time you are dead...#n' as your anger rise.", victim, NULL, NULL, TO_CHAR );
                     act( "$n mutters '#yNext time you are dead...#n' as $s anger rises.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 4: act( "You say '#yYou are NOTHING without power#n' as you point proudly at your muscles.", victim, NULL, NULL, TO_CHAR );
                     act( "$n says '#yYou are NOTHING without power#n' as $e points proudly at $s muscles.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 5: act( "You ask $N '#yWanna be my servant?#n' as you make lewd remarks to $M.", victim, NULL, ch, TO_CHAR );
                     act( "$n asks you '#yWanna be my servant?#n' as $e makes lewd remarks to you.", victim, NULL, ch, TO_VICT );
                     act( "$n asks $N '#yWanna be my servant?#n' as $e makes lewd remarks to $M.", victim, NULL, ch, TO_NOTVICT );
                     break;

             case 6: act( "You mutter '#yStop playing around...#n' as you show you mean business.", victim, NULL, NULL, TO_CHAR );
                     act( "$n mutters '#yStop playing around...#n' as $e shows $e means business.", victim, NULL, NULL, TO_ROOM );
                     break;

             case 7: act( "You ask '#yTake it easy will ya?#n' as you step cautiously.", victim, NULL, NULL, TO_CHAR );
                     act( "$n asks '#yTake it easy will ya?#n' as $e steps cautiously.", victim, NULL, NULL, TO_ROOM );
                     break;
          }
       }

    if ( victim->topmode == 100 && victim->hit > victim->max_hit * 0.66
         && number_percent() > 94 && dam > 149999 && !block )
       {
          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             act("You quickly parry $N's attack and go for the counter attack.",victim,NULL,ch,TO_CHAR);

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             act("$n suddenly parries your attack and goes for the counter attack.",victim,NULL,ch,TO_VICT);

          victim->attack_type = ATTACK_PHYSICAL;
          hurt_person( victim, ch, 300000, TRUE );

          if ( ch == NULL || ch->in_room == NULL || ch->position < POS_STUNNED )
             return;
       }

    if ( victim->position == POS_FIGHTING && dam > 999999 && IS_ITEMAFF( victim, ITEMA_ELECSHIELD ) )
       {
          dam /= 20;

          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             {
                sprintf( buf, "The #ylightning#n crackles and sparks stunning $n! [#r%d#n]", dam );
                act( buf, ch, NULL, victim, TO_VICT );
             }

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             {
                sprintf( buf, "The #ylightning#n crackles and sparks, stunning you! [#r%d#n]", dam );
                act( buf, ch, NULL, NULL, TO_CHAR );
             }

          if ( !is_martial( ch ) )
             ch->wait = UMIN( 18, ch->wait + ( dam / 200000 ) );

          hurt_person( victim, ch, dam, agg_dam );
          return;
       }

    if ( victim->position == POS_FIGHTING && number_percent() > 89 && ( victim->outer_aura == A_THORNS || victim->inner_aura == A_THORNS || IS_SET(victim->group_aura, G_THORNS) ) )
       {
          int level = 0;

          if ( !IS_SET(victim->group_aura, G_THORNS) )
             {
                if ( victim->inner_aura == A_THORNS )
                   level += level + 1;

                level += victim->aura[A_THORNS];
             }
             else level = victim->special_aura[A_THORNS];
 
          if ( level > 10 )
             level = 10;

          dam *= number_range( 0, level * 20 );
          dam /= 100;

          if ( dam > 500000 )
             dam = 500000;

          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             {
                sprintf( buf, "The heavens rejoice in harmony as an unseen force SmAsHeS $n! [#r%d#n]", dam );
                act( buf, ch, NULL, victim, TO_VICT );
             }

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             {
                sprintf( buf, "The heavens rejoice in harmony as an unseen force SmAsHeS you! [#r%d#n]", dam );
                act( buf, ch, NULL, NULL, TO_CHAR );
             }

          victim->attack_type = ATTACK_MAGICAL;
          hurt_person(victim,ch,dam,agg_dam);
          return;
       }

    if ( victim->position == POS_FIGHTING && number_percent() > 91 - counter_bonus( ch, victim, at )
         && IS_SET( victim->more3, MORE3_MANTIS ) )
       {
          dam *= 0.01 * number_range( 0, 100 );

          if ( dam > 500000 )
             dam = 500000;

          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             {
                sprintf( buf, "You sneer as you open your hand scatching $n with dark energy! [#e%d#n]", dam );
                act( buf, ch, NULL, victim, TO_VICT );
             }

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             {
                sprintf( buf, "$N sneers as $e opens $s hand scatching you with dark energy! [#e%d#n]", dam );
                act( buf, ch, NULL, victim, TO_CHAR );
             }

          victim->attack_type = ATTACK_PHYSICAL;
          victim->ele_attack = ATTACK_DARK;
          hurt_person(victim,ch,dam,agg_dam);
          return;
       }
 
    if ( number_percent() > 91 && is_affected( ch, skill_lookup("vampires touch") ) && dam > 0 )
       {
          dam /= 4;

          sprintf( buf, "You shudder with glee as you float towards $N and suck $S blood!" );
          act( buf, victim, NULL, ch, TO_CHAR );

          sprintf( buf, "$n shudders with glee as $e floats towards you and sucks your blood! [#r%d#n]", dam );
          act( buf, victim, NULL, ch, TO_VICT );

          sprintf( buf, "$n shudder with glee as $e floats towards $N and sucks $S blood! [#r%d#n]", dam );
          act( buf, victim, NULL, ch, TO_NOTVICT );

          hit_gain( victim, dam );

          if ( dam > 250000 )
             dam = 250000;

          victim->attack_type = ATTACK_PHYSICAL;
          hurt_person( victim, ch, dam, TRUE );
          return;
       }

    if ( number_percent() > 91 && !IS_NPC( victim ) && IS_CLASS( victim, CLASS_DROW ) 
         && ch->ctimer[3] == 0 && ( victim->pcdata->powers[DROW_CLASS] == DROW_MAGI 
         || victim->pcdata->powers[DROW_CLASS] == DROW_AVATAR ) && victim->pkill > 3 && dam > 0 && !is_martial( ch ) )
       {
          act( "#w$N#o's skin goes pale with #Rfear #ofrom the brief touch of #eyour #oghoul like skin.#n", victim, NULL, ch, TO_CHAR );
          act( "#wYour #oskin goes pale with #Rfear #ofrom the brief touch of #e$n#o's ghoul like skin.#n", victim, NULL, ch, TO_VICT );
          act( "#w$N#o's skin goes pale with #Rfear #ofrom the brief touch of #e$n#o's ghoul like skin.#n", victim, NULL, ch, TO_NOTVICT );

          ch->wait = 12;
          ch->ctimer[3] = 6;

          if ( number_percent() > 70 && ch->stunned == 0 )
             ch->stunned = 1;
      }
 
    if ( victim->position == POS_FIGHTING && IS_SET( victim->cmbt[7], SKL_RETORT ) && !IS_SET( ch->cmbt[7], SKL_SWERVE )
         && number_percent() > 65 - counter_bonus( ch, victim, at ) && dam < 150000 )
       {
          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             act("You quickly parry $N's attack and go for the counter attack.",victim,NULL,ch,TO_CHAR);

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             act("$n suddenly parries your attack and goes for the counter attack.",victim,NULL,ch,TO_VICT);

          one_hit( victim, ch, TYPE_UNDEFINED, 1 );
          return;
       }

    if ( victim->position == POS_FIGHTING && IS_CLASS( victim, CLASS_SHADOW ) && victim->gifts[CURRENT_METER] == ACHI
         && number_percent() > 89 - counter_bonus( ch, victim, at ) )
       {
          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             act("You quickly parry $N's attack and go for the counter attack.",victim,NULL,ch,TO_CHAR);

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             act("$n suddenly parries your attack and goes for the counter attack.",victim,NULL,ch,TO_VICT);

          one_hit( victim, ch, TYPE_UNDEFINED, 1 );
          return;
       }
       else if ( victim->position == POS_FIGHTING && IS_CLASS( victim, CLASS_SHADOW_WAR ) && number_percent() > 
                 89 - counter_bonus( ch, victim, at ) )
       {
          if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
             act("A log appears in a puff of smoke as you drop above $N.",victim,NULL,ch,TO_CHAR);

          if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
             act("A log appears in a puff of smoke as $n drops above you.",victim,NULL,ch,TO_VICT);

          one_hit( victim, ch, TYPE_UNDEFINED, 1 );
          return;
       }

  if ( victim->position == POS_DEAD )
  {
      if ( IS_NPC(victim) && !IS_NPC(ch) )
         {
           if ( !IS_SET(victim->act, ACT_MOUNT) )
              {
                 if ( number_percent() < 96 && global_qp == FALSE )
                    ch->gold = UMIN( ch->gold + 1, 500000000 );
                    else ch->gold = UMIN( ch->gold + 10, 500000000 );
              }

           ch->mkill += 1;

           if ( IS_CLASS(ch, CLASS_DEMON) || IS_CLASS(ch, CLASS_DRAGON) || IS_CLASS(ch, CLASS_DROW) )
              {
                 if ( IS_CLASS(ch, CLASS_DEMON) ) 
                    {
                       ch->pcdata->stats[DEMON_CURRENT] +=victim->level*100;
                       ch->pcdata->stats[DEMON_TOTAL] +=victim->level*100;
                    }

                 if ( IS_CLASS(ch, CLASS_DROW) ) 
                    ch->pcdata->stats[DROW_POWER] += victim->level*100;
             }
         }

      if ( !IS_NPC(victim) && IS_NPC(ch) && ch->master == NULL )
         victim->mdeath = victim->mdeath + 1;

      raw_kill( victim );

      if ( IS_SET(ch->act, PLR_AUTOLOOT) )
         do_get( ch, "all corpse" );
         else do_look( ch, "in corpse" );

      if ( !IS_NPC(ch) && IS_NPC(victim) )
         {
           if ( IS_SET(ch->act, PLR_AUTOSAC) )
              do_sacrifice( ch, "corpse" );
         }

      return;
  }

  if ( victim == ch )
    return;

  tail_chain( );
}

bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
  CHAR_DATA *mount;
  bool no_group = FALSE;
  CHAR_DATA *opponent = ( IS_NPC( victim ) && victim->master != NULL ) ? victim->master : victim;
  CHAR_DATA *attacker = ( IS_NPC( ch ) && ch->master != NULL ) ? ch->master : ch;

  if ( attacker->in_room == NULL || opponent->in_room == NULL )
     return TRUE;

  if ( opponent->in_room->vnum < 5 || ( opponent->in_room->vnum >= 93 && opponent->in_room->vnum <= 99 ) )
     return TRUE;

  if ( opponent->in_room->vnum >= 33500 && opponent->in_room->vnum <= 33800 && opponent->in_room
       != attacker->in_room )
     return TRUE;

  if ( ( IS_CLASS( opponent, CLASS_DRAGOON ) || IS_CLASS( opponent, CLASS_MONK )
         || IS_CLASS( opponent, CLASS_NINJA ) ) && opponent->ctimer[8] > 0 )
     return TRUE;

  if ( ( endozone == 0 && opponent == attacker ) || opponent->air_timer > 0 )
     return TRUE;

  if ( attacker->caller == opponent )
     return TRUE;

  if ( IS_NPC( opponent ) && IS_SET( opponent->act, ACT_JAILER ) )
     return TRUE;

  if ( IS_NPC( victim ) && IS_NPC( ch ) && victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum < 571
       && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 && victim->in_room->vnum >= 33500
       && victim->in_room->vnum <= 33800 )
     return TRUE;

  if ( nofighting > 0 )
     {
        stc("#yYou don't see any need of #Rfighting #yright now.#n\n\r", attacker );
        return TRUE;
     }

  if ( !IS_NPC(attacker) && opponent->raiding == 0 )
     {
        if ( attacker->desc != NULL && ( attacker->desc->connected == CON_NOTE_TO || attacker->desc->connected == CON_NOTE_SUBJECT
             || attacker->desc->connected == CON_NOTE_EXPIRE || attacker->desc->connected == CON_NOTE_TEXT
             || attacker->desc->connected == CON_NOTE_FINISH ) )
           return TRUE;

        if ( !armageddon && attacker->fight_timer == 0 && !attacker->desc && !( attacker->in_room->vnum >= 550 
             && attacker->in_room->vnum < 630 ) )
           return TRUE;
     }

  if ( arena == FIGHT_WAIT )
     {
        stc("#yYou can't start until Auron gives the word warrior.#n\n\r", attacker );
        return TRUE;
     }

  if ( attacker->fighting == opponent )
     return FALSE;

  if ( !IS_NPC(opponent) && !IS_SET( opponent->arena, AFF2_COMPETITOR ) && opponent->raiding == 0
       && !IS_SET( opponent->arena, AFF2_TEAM1 ) && !IS_SET( opponent->arena, AFF2_TEAM2 ) )
     {
        if ( !armageddon && opponent->fight_timer == 0 && !opponent->desc && opponent->pcdata->capture_timer == 0
            && !( opponent->in_room->vnum >= 550 && opponent->in_room->vnum < 630 ) )
           {
              stc("You can't attack linkdead people!\n\r",attacker);
              return TRUE;
           }

        if ( opponent->desc != NULL && ( opponent->desc->connected == CON_NOTE_TO || opponent->desc->connected == CON_NOTE_SUBJECT
             || opponent->desc->connected == CON_NOTE_EXPIRE || opponent->desc->connected == CON_NOTE_TEXT
             || opponent->desc->connected == CON_NOTE_FINISH ) && !( opponent->in_room->vnum >= 550 
             && opponent->in_room->vnum < 630 ) )
           return TRUE;
     }

  if ( IS_HEAD(attacker,LOST_HEAD) || IS_EXTRA(attacker,EXTRA_OSWITCH) )
     {
        send_to_char( "Objects cannot fight!\n\r", attacker );
        return TRUE;
     }

  if ( IS_HEAD(opponent,LOST_HEAD) || IS_EXTRA(opponent,EXTRA_OSWITCH))
     {
        send_to_char( "You cannot attack an object.\n\r", attacker );
        return TRUE;
     }

  if ( IS_NPC(opponent) && opponent->level == 666 )
     {
        stc("They look uninterested of you.\n\r",attacker);
        return TRUE;
     }

  if ( IS_NPC(opponent) && ( armageddon == TRUE || nogang == 10 || mysafe == 10 ) &&
       get_kingdom_num( opponent ) != -1 )
     return TRUE;

  if ( IS_SET( attacker->more3, MORE3_CONFUSE ) || ( !IS_NPC( attacker ) 
       && attacker->pcdata->personality == PER_PSYCHO && attacker->cmbt[1] > 0 ) || endozone > 0 )
     no_group = TRUE;

  if ( !IS_NPC( attacker ) && IS_NPC( opponent ) && get_kingdom_num( opponent ) == attacker->pcdata->kingdom
       && !no_group )
     return TRUE;

  if ( IS_NPC(attacker) && attacker->level == 666 )
     return TRUE;

  if ( opponent->protector != NULL && opponent->protector->in_room == opponent->in_room 
       && opponent->attacker == NULL && is_same_plane( opponent->protector, opponent ) )
     {
        if ( attacker->fighting == opponent )
           {
              stop_fighting( attacker, FALSE );
              set_fighting( attacker, opponent->protector );
           }
           else {
                   act( "They are protected by $N.", attacker, NULL, opponent->protector, TO_CHAR );
                   return TRUE;
                }
     }

  if ( is_same_group(attacker,opponent) || is_same_kingdom( attacker, opponent ) )
     return TRUE;

  if ( attacker->safe_timer > 0 )
     {
        send_to_char( "Not with a safe timer.\n\r", attacker );
        return TRUE;
     }

  if ( opponent->safe_timer > 0 )
     {
        send_to_char( "They have a safe timer.\n\r", attacker );
        return TRUE;
     }

  if ( IS_NPC(opponent) && ( mount = opponent->mount ) != NULL && opponent->mounted == IS_MOUNT )
     return TRUE;

  if ( ( attacker->attacker != NULL || opponent->attacker != NULL ) && !armageddon )
     {
        bool arena = FALSE;

        if ( !IS_NPC(opponent) && opponent->pcdata->team > 0 )
           arena = TRUE;

        if ( !IS_NPC(attacker) && !IS_NPC(opponent) && opponent->attacker != NULL && opponent->attacker != attacker
             && !arena )
           {
              stc("But they are already in a fight.\n\r",attacker);
              return TRUE;
           }

        if ( !IS_NPC(attacker) && attacker->attacker != NULL && attacker->attacker != opponent && !IS_NPC(opponent)
             && !arena )
           {
              act("But you are already in a fight with $N.",attacker,NULL,attacker->attacker,TO_CHAR);
              return TRUE;
           }
     }

  if ( opponent->in_room->vnum >= 550 && opponent->in_room->vnum < 630 && ( attacker->in_room->vnum < 550 ||
       attacker->in_room->vnum >= 630 ) && attacker->strike_timer == 0 )
     {
        stc("You can't attack anyone participating in an arena battle.\n\r", attacker );
        return TRUE;
     }

  if ( !IS_NPC(opponent) && !IS_NPC(attacker) )
     {
        if ( (get_age(attacker)-17) < 2 )
           {
              stc("But you are a newbie.\n\r",attacker);
              return TRUE;
           }

        if ( (get_age(opponent)-17) < 2 )
           {
              stc("But they are a newbie.\n\r",attacker);
              return TRUE;
           }
      }

  if ( !IS_NPC(attacker) && !IS_NPC(opponent) )
     {
        if ( attacker->pcdata->team > 0 && attacker->pcdata->team < 9 && attacker->pcdata->team == opponent->pcdata->team )
           return TRUE;

        if ( attacker->pcdata->team > 0 && arena != FIGHT_QUEST )
           {
              stc("You can't attack anyone until the quest STARTS!\n\r",attacker);
              return TRUE;
           }

        if ( attacker->pcdata->team > 0 && opponent->pcdata->team == 0 )
           {
              stc("You can't attack people who isn't in a team.\n\r",attacker);
              return TRUE;
           }

        if ( opponent->pcdata->team > 0 && attacker->pcdata->team == 0 )
           {
              stc("You can't attack people who is in a team.\n\r",attacker);
              return TRUE;
           }
     }

  if ( in_arena(attacker, opponent) )
     return FALSE;

/*
  if ( !IS_NPC( attacker ) && !IS_NPC( opponent ) )
     {
        if ( !armageddon && IS_SET( attacker->special, SPC_RECLASS ) && !IS_SET( opponent->special, SPC_RECLASS ) )
           {
              stc( "You are forbidden from attacking normal classes.\n\r", ch );
              return TRUE;
           }

        if ( !armageddon && !IS_SET( attacker->special, SPC_RECLASS ) && IS_SET( opponent->special, SPC_RECLASS ) )
           {
              stc( "It is suicide for you to attack a special class.\n\r", ch );
              return TRUE;
           }
     }
*/

  if ( !no_group )
     {
        if ( !IS_NPC( attacker ) && !IS_NPC( opponent ) && opponent->pcdata->kingdom == attacker->pcdata->kingdom
             && opponent->pcdata->kingdom > 0 )
           return TRUE;
     }

  if ( IS_NPC(opponent) && opponent->pIndexData->vnum == 712 )
     {
         stc("Foolish One... How do you expect to hurt God?\n\r",attacker);
         return TRUE;
     }

  if ( IS_ITEMAFF(opponent, ITEMA_ARTIFACT) )
     return FALSE;

  if ( armageddon == TRUE || mysafe != 0 || ( victim->in_room->vnum >= 33500 && victim->in_room->vnum <= 33800 ) )
     return FALSE;

  if ( IS_SET(opponent->extra, EXTRA_AFK) )
     {
        send_to_char( "They are AFK.\n\r", attacker );
        return TRUE;
     }

  if ( IS_AFFECTED(attacker, AFF_ETHEREAL) && !IS_AFFECTED(opponent, AFF_ETHEREAL) )
     {
        send_to_char( "You cannot while ethereal.\n\r", attacker );
        return TRUE;
     }

  if ( IS_AFFECTED(opponent, AFF_ETHEREAL) && !IS_AFFECTED(attacker, AFF_ETHEREAL) )
     {
        send_to_char( "You cannot fight an ethereal person.\n\r", attacker );
        return TRUE;
     }

  if ( IS_AFFECTED(attacker, AFF_SHADOWPLANE) && !IS_AFFECTED(opponent, AFF_SHADOWPLANE) )
     {
        act( "You are too insubstantial!", attacker, NULL, opponent, TO_CHAR );
        return TRUE;
     }

  if ( IS_AFFECTED(opponent, AFF_SHADOWPLANE) && !IS_AFFECTED(attacker, AFF_SHADOWPLANE) )
     {
        act( "$E is too insubstantial!", attacker, NULL, opponent, TO_CHAR );
        return TRUE;
     }

  if ( opponent->fight_timer > 0 || opponent->raiding > 0 )
     return FALSE;

  if ( opponent->fighting == attacker )
     return FALSE;

  if ( IS_SET( opponent->in_room->room_flags,ROOM_SAFE ) )
     {
        send_to_char( "The room is currently a safe haven from attackers.\n\r", attacker );
        return TRUE;
     }

  if ( IS_SET( attacker->more, MORE_SAFE ) )
     {
        send_to_char( "You are unable to attack them.\n\r", attacker );
        return TRUE;
     }

  if ( IS_SET( opponent->more, MORE_SAFE ) )
     {
        send_to_char( "You can't seem to attack them.\n\r", attacker );
        return TRUE;
     }

  return FALSE;
}

/*
 * See if an attack justifies a KILLER flag.  ** This function does NOTHING - JOBO **
 */
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
  return;
}

/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  OBJ_DATA *obj = NULL, *wpn = NULL;
  int chance = 75;
  bool claws = FALSE;
  bool hands = FALSE;
  bool hands2 = FALSE;

  if ( !IS_NPC( victim ) )
     {
        if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
           return FALSE;
     }

  if ( IS_SET(victim->more, MORE_FROZEN) || IS_SET(victim->more, MORE_STONE) )
     return FALSE;

  if ( victim->stunned > 0 )
     return FALSE;

  if ( victim->holding != NULL || victim->holded != NULL || victim->position <= POS_STUNNED )
     return FALSE;

  if ( victim->noblock > 0 || victim->blocking > 0 )
     return FALSE;

  if (!IS_AWAKE(victim)) return FALSE;

  if (IS_NPC(victim))  obj = NULL;
  else if ( ( IS_CLASS(victim, CLASS_WEREWOLF) || IS_CLASS(victim, CLASS_NINJA)
  || IS_CLASS(victim, CLASS_GAROU) )
  && IS_VAMPAFF(victim, VAM_CLAWS) && ( get_eq_char(victim, WEAR_WIELD) == NULL 
  || get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    claws = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_MONK) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_MASTER) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_NINJA) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_SAIYAN) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_SHADOW) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_PHOENIX) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    claws = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_DARAMON) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_WISP) && ( get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL) )
  {
    obj = NULL;
    hands = TRUE;
  }
  else if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_DRAGON)	&& 
	victim->dragonform > 0		&& 
	( get_eq_char(victim, WEAR_WIELD) == NULL	||
	get_eq_char(victim, WEAR_HOLD) == NULL) )
    {
	obj = NULL;
	claws = TRUE;
    }
  else
  {
    if ( (obj = get_eq_char(victim, WEAR_WIELD)) == NULL || obj->item_type != ITEM_WEAPON )
      if ( (obj = get_eq_char(victim, WEAR_HOLD)) == NULL || obj->item_type != ITEM_WEAPON ) 
         if ( (obj = get_eq_char(victim, WEAR_THIRD)) == NULL || obj->item_type != ITEM_WEAPON ) 
            if ( (obj = get_eq_char(victim, WEAR_FOURTH)) == NULL || obj->item_type != ITEM_WEAPON ) 
               return FALSE;
  }

  if ( ( dt < 1000 || dt > 1012) && !IS_CLASS( victim, CLASS_MONK ) && !IS_CLASS( victim, CLASS_WISP ) )
     return FALSE;

  if ( is_affected( victim, skill_lookup("magical wall") ) )
     {
        if ( number_percent() > 90 )
           {
              if ( IS_NPC(victim) || !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
	           act( "A shield of energy rapidly parries $n's attack.",  ch, NULL, victim, TO_VICT    );

              if ( IS_NPC(ch) || !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
 	           act( "A shield of energy rapidly parries your attack.", ch, NULL, victim, TO_CHAR    );

              return TRUE;
           }
     }

  chance += number_range( 0, get_curr_luck( ch ) * 2 ) - number_range( 0, get_curr_luck( victim ) * 2 );

  if ( !IS_NPC( ch ) && ch->pcdata->blood_power == 3 )
     chance += number_range( 10, 15 );

  if ( IS_SET( ch->cmbt[7], SKL_HITRATE ) )
     chance += 25;

  if ( IS_SET( victim->cmbt[7], SKL_DODGE ) )
     chance -= 50;

  if ( is_affected( victim, skill_lookup("last prayer") ) )
     chance += 100;
     else if ( IS_SET( victim->more, MORE_SIN ) ) 
             chance += 10;
     else if ( is_affected( ch, skill_lookup("prayer") ) )
             chance += 10;

  if ( ch->outer_aura == A_FANATICISM || ch->inner_aura == A_FANATICISM || IS_SET(ch->group_aura, G_FANATICISM) )
     chance += 10;
     else if ( ch->outer_aura == A_CONVICTION || ch->inner_aura == A_CONVICTION || IS_SET(ch->group_aura, G_CONVICTION) )
             chance += 10;

  if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 558 && ch->pIndexData->vnum < 560 )
     chance += 33;

  chance = URANGE( 25, chance, 90 );

  if ( number_percent( ) <= chance )
     return FALSE;

  if ( IS_SET(ch->newbits, NEW_FIGHTDANCE) && IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(victim, CLASS_MONK)
       && number_percent() >= 87 )
     {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
           act( "$n's dark weapon dances around your defences, making a strange sound.",  ch, NULL, victim, TO_VICT );

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
           act( "Your sacrificial weapon hums in the air as it dances $N's defences.", ch, obj, victim, TO_CHAR );

        return FALSE;
     }

  if ( ( ( wpn = get_eq_char( ch, WEAR_WIELD ) ) != NULL ) && wpn->item_type == ITEM_WEAPON
       && wpn->value[3] == 3 && IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && !IS_CLASS( victim, CLASS_NINJA ) 
       && !IS_CLASS( victim, CLASS_MONK ) && number_percent() > 87 )
     {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
           act( "As you raise your weapon to block $n's blow, $n diverts in midswing.", ch, NULL, victim, TO_VICT );

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
           act( "As $N raises $S weapon to block your blow, you divert in midswing.", ch, NULL, victim, TO_CHAR );

        return FALSE;
     }

  if ( hands && IS_CLASS(victim, CLASS_WISP) )
     {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
           act( "Your body shimmers as it absorbs $n's blow.", ch,NULL,victim, TO_VICT);

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
           act( "$N's body shimmers as it absorbs your blow.", ch,NULL,victim, TO_CHAR);

	  return TRUE;
     }

  if ( hands )
     {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
           act( "You quickly parry $n's blow with your bare hands.", ch,NULL,victim, TO_VICT);

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
           act( "$N quickly parries your blow with $S bare hands.", ch,NULL,victim, TO_CHAR);

	  return TRUE;
     }

  if (claws)
  {
    if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
        act( "You parry $n's blow with your claws.",  ch, NULL, victim, TO_VICT );

    if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
      act( "$N parries your blow with $S claws.", ch, NULL, victim, TO_CHAR );

    return TRUE;
  }

  if ( !IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0 && obj->value[3] <= 12 )
     {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
           act( "You parry $n's blow with $p.",  ch, obj, victim, TO_VICT );

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
           act( "$N parries your blow with $p.", ch, obj, victim, TO_CHAR );

        return TRUE;
     }

  if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
     act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT );

  if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
     act( "$N parries your attack.", ch, NULL, victim, TO_CHAR );

  return TRUE;
}

/*
 * Leighton - Check for shield block. so put this here
 */
bool check_shield_block( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj = NULL;
    int chance = 0;

    if ( IS_SET(victim->more, MORE_FROZEN) || IS_SET(victim->more, MORE_STONE) )
       return FALSE;

    if ( victim->stunned > 0 )
       return FALSE;

    if ( victim->holding != NULL || victim->holded != NULL || victim->position <= POS_STUNNED )
       return FALSE;

    if ( !IS_NPC( victim ) )
       {
          if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
             return FALSE;
       }

    if ( victim->noblock > 0 || victim->blocking > 0 || victim->position <= POS_STUNNED )
       return FALSE;

    if ( !IS_AWAKE(victim) )
       return FALSE;

    if ( !IS_ITEMAFF( victim, ITEMA_SHIELD ) && !IS_AFFECTED( victim, AFF_SHIELD ) )
       return FALSE;

    chance -= number_range( 0, get_curr_luck( victim ) ) + number_range( 0, get_curr_luck( ch ) );

    if ( number_percent() < 75 - chance )
       return FALSE;

    if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
       act( "You block $n's attack with your shield.",  ch, NULL, victim, TO_VICT );

    if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
	 act( "$N blocks your attacks with $S shield.", ch, NULL, victim, TO_CHAR );

    if ( is_affected( victim, skill_lookup("holy shield") ) )
       {
          AFFECT_DATA af;

          af.type = skill_lookup("holy shield bonus");
          af.location  = APPLY_CON;
          af.modifier  = 1;
          af.duration  = 0;
          af.bitvector = 0;
          affect_join( victim, &af );
       }

    return TRUE;
}

/*
 * Check for dodge.
 */
bool check_dodge2( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance = 0;

    if ( IS_NPC( victim ) || !( IS_CLASS( victim, CLASS_MONK ) || IS_CLASS( victim, CLASS_SHADOW ) ) )
      return FALSE;

    if ( victim->noblock > 0 || victim->blocking > 0 || victim->position <= POS_STUNNED )
       return FALSE;

    if ( IS_SET(victim->more, MORE_FROZEN) || IS_SET(victim->more, MORE_STONE) )
       return FALSE;

    if ( victim->stunned > 0 )
       return FALSE;

    if ( victim->holding != NULL || victim->holded != NULL )
       return FALSE;

    if ( !IS_NPC( victim ) )
       {
         if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
            return FALSE;

         if ( IS_CLASS( victim, CLASS_MONK ) && !IS_SET( victim->more3, MORE3_FALCON )
              && !IS_SET( victim->more3, MORE3_TIGER ) )
            return FALSE;

         if ( IS_CLASS( victim, CLASS_SHADOW ) && victim->gifts[CURRENT_METER] != ANI )
            return FALSE;
       }
 
    chance -= number_range( 0, get_curr_luck( victim ) ) + number_range( 0, get_curr_luck( ch ) );

    if ( number_percent() <= 90 - chance )
       return FALSE;

    chance = number_range(1,4);

    if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
    {
       switch(chance)
       {
          case 1:	act( "You swerve to the right dodging $n's attack.", ch, NULL, victim, TO_VICT );
                  break;
          case 2:	act( "You swerve to the left dodging $n's attack.", ch, NULL, victim, TO_VICT );
                  break;
          case 3:	act( "You duck to the ground avoiding $n's attack.", ch, NULL, victim, TO_VICT );
                  break;
          case 4:	act( "You leap away from $n's attack.", ch, NULL, victim, TO_VICT );
                  break;
       }
    }

    if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
    {
       switch(chance)
       {
          case 1:	act( "$N swerves to the right dodging your attack.", ch, NULL, victim, TO_CHAR );
                  break;
          case 2:	act( "$N swerves to the left dodging your attack.", ch, NULL, victim, TO_CHAR );
                  break;
          case 3:	act( "$N ducks to the ground avoiding your attack.", ch, NULL, victim, TO_CHAR );
                  break;
          case 4:	act( "$N leaps away from your attack.", ch, NULL, victim, TO_CHAR );
                  break;
       }
    }
    return TRUE;
}

/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  int chance = 75;
  OBJ_DATA *wpn = NULL;

  if ( IS_NPC( victim ) && victim->pIndexData->vnum == 570 )
     {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
           act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT );

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
           act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR );

        return TRUE;
     }

  if ( IS_SET(victim->more, MORE_FROZEN) || IS_SET(victim->more, MORE_STONE) )
     return FALSE;

  if ( victim->noblock > 0 || victim->blocking > 0 )
     return FALSE;

  if ( victim->stunned > 0 )
     return FALSE;

  if ( victim->holding != NULL || victim->holded != NULL || victim->position <= POS_STUNNED )
     return FALSE;

  if ( !IS_NPC( victim ) )
     {
        if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
           return FALSE;
     }

  if ( !IS_AWAKE(victim) ) return FALSE;

  if ( ( dt < 1000 || dt > 1012) && !IS_CLASS( victim, CLASS_NINJA ) )
     return FALSE;

  chance += number_range( 0, get_curr_luck( ch ) * 2 ) - number_range( 0, get_curr_luck( victim ) * 2 );

  if ( !IS_NPC( ch ) && ch->pcdata->blood_power == 3 )
     chance += number_range( 10, 15 );

  if ( IS_SET( ch->cmbt[7], SKL_HITRATE ) )
     chance += 25;

  if ( IS_SET( victim->cmbt[7], SKL_DODGE ) )
     chance -= 50;

  if ( is_affected( victim, skill_lookup("last prayer") ) )
     chance += 100;
     else if ( IS_SET( victim->more, MORE_SIN ) ) 
             chance += 10;
     else if ( is_affected( ch, skill_lookup("prayer") ) )
             chance += 10;

  if ( ch->outer_aura == A_FANATICISM || ch->inner_aura == A_FANATICISM || IS_SET(ch->group_aura, G_FANATICISM) )
     chance += 10;
     else if ( ch->outer_aura == A_CONVICTION || ch->inner_aura == A_CONVICTION || IS_SET(ch->group_aura, G_CONVICTION) )
             chance += 10;

  if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 558 && ch->pIndexData->vnum < 560 )
     chance += 33;

  chance = URANGE( 25, chance, 90 );

  if ( number_percent( ) <= chance )
     return FALSE;

  if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
     act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT );

  if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
     act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR );

  return TRUE;
}

bool check_illusionblur( CHAR_DATA *ch, CHAR_DATA *victim )
{
  OBJ_DATA *obj = NULL;
  int chance = 0;

  if ( !IS_CLASS(victim,CLASS_VAMPIRE) && !IS_CLASS(victim,CLASS_SHADOW) && !IS_SET( victim->more, MORE_BLITZ )
       && !IS_CLASS(victim,CLASS_WISP) && !IS_CLASS( victim, CLASS_SHADOW_WAR ) && !IS_SET( victim->cmbt[7], SKL_ILLUSION ) )
       return FALSE;

  if ( victim->noblock > 0 || victim->blocking > 0 || victim->position <= POS_STUNNED )
     return FALSE;

  if ( IS_SET(victim->more, MORE_FROZEN) || IS_SET(victim->more, MORE_STONE) )
     return FALSE;

  if ( victim->noblock > 0 || victim->blocking > 0 )
     return FALSE;

  if ( victim->stunned > 0 )
     return FALSE;

  if ( victim->holding != NULL || victim->holded != NULL )
     return FALSE;

  if ( !IS_NPC( victim ) )
     {
        if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
           return FALSE;
     }

    chance -= number_range( 0, get_curr_luck( victim ) ) + number_range( 0, get_curr_luck( ch ) );

    if ( number_percent() <= 90 - chance )
       return FALSE;

    if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
       act( "You shimmer as $n's attack goes through you.",  ch, NULL, victim, TO_VICT );

    if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
	 act( "$N shimmers as your attacks appear to go through $M.", ch, NULL, victim, TO_CHAR );
  
    return TRUE;
}

bool check_weapon_guard( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *obj = NULL;
    int chance = 0;

    if ( IS_NPC(victim) )
       return FALSE;

    if ( !IS_AWAKE(victim) )
	return FALSE;

  if ( victim->noblock > 0 || victim->blocking > 0 || victim->position <= POS_STUNNED )
     return FALSE;

  if ( IS_SET(victim->more, MORE_FROZEN) || IS_SET(victim->more, MORE_STONE) )
     return FALSE;

  if ( victim->stunned > 0 )
     return FALSE;

  if ( victim->holding != NULL || victim->holded != NULL )
     return FALSE;

    if ( !IS_CLASS( victim, CLASS_WEREWOLF ) && !IS_CLASS( victim, CLASS_DRAGOON )
         && !IS_CLASS( victim, CLASS_SAMURAI ) && !IS_SET( victim->cmbt[7], SKL_GUARD )
         && !( IS_CLASS( victim, CLASS_ASSASSIN ) && IS_SET( victim->special, SPC_KNIGHT ) ) )
       return FALSE;  

  if ( !IS_NPC( victim ) )
     {
        if ( IS_CLASS( victim, CLASS_MONK ) && victim->pcdata->powers[MONK_SHIKI] > 0 )
           return FALSE;
     }

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON )
       if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL || obj->item_type != ITEM_WEAPON ) 
          if ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL || obj->item_type != ITEM_WEAPON ) 
             if ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL || obj->item_type != ITEM_WEAPON ) 
                return FALSE;

    chance -= number_range( 0, get_curr_luck( victim ) ) + number_range( 0, get_curr_luck( ch ) );

    if ( number_percent() <= 90 - chance )
       return FALSE;

    if (!IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON &&
	obj->value[3] >= 0 && obj->value[3] <= 12)
    {
        if ( !( IS_SET(victim->act, PLR_BRIEF) || IS_SET(victim->act, PLR_BRIEF2) ) )
	    act( "You quickly parry $n's attacks with $p.",  ch, obj, victim, TO_VICT );

        if ( !( IS_SET(ch->act, PLR_BRIEF) || IS_SET(ch->act, PLR_BRIEF2) ) )
	    act( "$N rapidly parries your attacks with $p.", ch, obj, victim, TO_CHAR );
	return TRUE;
    }

    return TRUE;
}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
  CHAR_DATA *mount;
  int new_total = conaneffect;

  if ( IS_CLASS(victim, CLASS_ASSASSIN) && IS_SET( victim->special, SPC_KNIGHT ) && victim->pkill > 19 )
     new_total += 1000000;

  if ( IS_CLASS(victim, CLASS_FIEND) && IS_SET( victim->more, MORE_WALL ) )
     new_total += 1000000;

  if ( IS_SET( victim->more3, MORE3_DRAGON ) )
     new_total += 750000;

  if ( victim->hit > 0 - new_total )
  {
      if ( victim->position < POS_STUNNED && victim->position != POS_MORTAL )
         {
            victim->position = POS_STANDING;

            if ( IS_NPC(victim) || victim->max_hit * 0.25 > victim->hit )
               {
                  act( "$n clambers back to $s feet.", victim, NULL, NULL, TO_ROOM);
                  act( "You clamber back to your feet.", victim, NULL, NULL, TO_CHAR);
               }
               else {
                       act( "$n flips back up to $s feet.", victim, NULL, NULL, TO_ROOM);
                       act( "You flip back up to your feet.", victim, NULL, NULL, TO_CHAR);
                    }
         }

      return;
  }
  else if ( ( mount = victim->mount ) != NULL )
  {
    if ( victim->mounted == IS_MOUNT )
       {
          victim->hit = victim->max_hit;
          return;
       }

    if ( victim->mounted == IS_RIDING )
       {
          act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
          act("You fall off $N.",victim,NULL,mount,TO_CHAR);
       }

    mount->mount    = NULL;
    victim->mount   = NULL;
    mount->mounted  = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;
    stop_fighting( mount, TRUE );
    extract_char( mount, TRUE );
  }

  if ( IS_NPC(victim) && victim->hit < 1 - new_total )
     {
        if ( !IS_SET( victim->arena, AFF2_INARENA ) )
           victim->position = POS_DEAD;
           else victim->position = POS_STUNNED;

        return;
     }

  if ( !IS_NPC(victim) && victim->hit < 0 - new_total )
     {
        victim->position = POS_MORTAL;
        victim->hit = -10 - new_total;
        return;
     }
}    

/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
  CHAR_DATA *opponent = ( IS_NPC( victim ) && victim->master != NULL ) ? victim->master : victim;
  CHAR_DATA *attacker = ( IS_NPC( ch ) && ch->master != NULL ) ? ch->master : ch;

  if ( ch->fighting != NULL || victim == ch )
     return;

  ch->fighting = victim;
  ch->position = POS_FIGHTING;

  if ( !IS_NPC( ch ) && !IS_NPC( victim ) && ch->pcdata->blood_meter > 0 && victim->fighting != ch )
     {
        act( "#wYou #rsigh #gas your #ysupernatural #Rpowers #edisappear...#n", ch, NULL, NULL, TO_CHAR );
        act( "#w$n #rsighs #gas $s #ysupernatural #Rpowers #edisappear...#n", ch, NULL, NULL, TO_ROOM );
        ch->pcdata->blood_power = 0;
        ch->pcdata->blood_meter = 0;
     }

  if ( !IS_NPC( attacker ) && !IS_NPC( opponent ) && attacker->fight_timer == 0 && opponent->fight_timer > 0 )
     give_align( attacker, -10 );

  if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->mergence > 0
       && ch->fighting->fighting != ch )
     ch->mergence = 0;

  if ( !IS_NPC(attacker) && !IS_NPC(opponent) ) 
     {
        if ( !( attacker->in_room->vnum >= 550 && attacker->in_room->vnum < 630 )
             && !( attacker->in_room->vnum >= 33500 && attacker->in_room->vnum < 33800 )
             && ( nogang == 10 || attacker->gshield > 0 || opponent->gshield > 0 ) 
             && !armageddon && opponent->hit > opponent->max_hit * 0.85 
             && opponent->fighting == NULL )
           {
              if ( attacker->attacker == NULL )
                 attacker->attacker = opponent;

              if ( opponent->attacker == NULL )
                 opponent->attacker = attacker;
           }

        if ( opponent->retaliate == NULL && attacker->fight_timer == 0 )
           {
              int range = 20;

              if ( opponent->pkill < 20 )
                 range = 5;
                 else if ( opponent->pkill < 40 )
                         range = 10;

              if ( attacker->pkill + range < opponent->pkill )
                 {
                    attacker->retaliate = opponent;
                    opponent->retaliate = attacker;
                 }
           }

        if ( attacker->fight_timer == 0 )
           {
              if ( IS_SET( attacker->special, SPC_RECLASS ) && !IS_SET( opponent->special, SPC_RECLASS ) )
                 {
                    opponent->anger_level = UMIN( 300, opponent->anger_level + 150 );

                    if ( opponent->anger_level == 300 )
                       {
                          act( "#PYour #warms #Cburst #winto #Rflames #was you swing your weapon #g#zviolently#n#w!#n", opponent, NULL, NULL, TO_CHAR );
                          act( "#P$n's #warms #Cburst #winto #Rflames #was $e swings $s weapon #g#zviolently#n#w!#n", opponent, NULL, NULL, TO_ROOM );
                          opponent->anger_level = 306;
                       }

                    add_grove( opponent, NULL, -150 );
                 }
                 else add_grove( attacker, NULL, -25 );
           }

        if ( attacker->fight_timer == 0 && get_translevel( attacker ) >= get_translevel( opponent ) + 1000000 )
           opponent->dam_boost = 1.00 + ( ( get_translevel( attacker ) - get_translevel( opponent ) ) * 0.0000001 );

        attacker->fight_timer = opponent->fight_timer = 20;
     }
}

/*
 * Stop fights.
 */
void stop_embrace( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if ( victim == NULL)
  {
    ch->embracing=NULL;
    return;
  }
  victim->embraced=NULL;
  ch->embracing=NULL;
}

void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
  CHAR_DATA *fch, *target = ( IS_CLASS( ch, CLASS_NECROMANCER ) ) ? ch->fighting : NULL;

  if ( !IS_NPC( ch ) && IS_SET( ch->mounted, IS_RIDING ) && ch->mount != NULL )
     stop_fighting( ch->mount, FALSE );

  for ( fch = char_list; fch != NULL; fch = fch->next )
  {
    if ( fch == ch || ( fBoth && fch->fighting == ch )
         || ( fBoth && IS_NPC( fch ) && fch->master == ch && target != NULL && fch->fighting == target ) )
    {
      if ( IS_CLASS( fch,CLASS_MONK ) || IS_CLASS( fch, CLASS_ASSASSIN ) || IS_CLASS( fch, CLASS_NINJA )
           || IS_CLASS( fch, CLASS_SHADOW ) )
         {
             fch->special_id = 0;
             fch->special_id_2 = 0;
             fch->monkstuff = 0;
         } 

      fch->total_hits = 0;
      fch->total_damage = 0;

      if ( IS_SET( fch->more2, MORE2_GUARD ) )
         REMOVE_BIT( fch->more2, MORE2_GUARD );

      if ( IS_SET( fch->more2, MORE2_MIRROR ) )
         REMOVE_BIT( fch->more2, MORE2_MIRROR );

      if ( IS_SET( fch->more, MORE_BLADEGRASP ) )
         REMOVE_BIT( fch->more, MORE_BLADEGRASP );

      fch->fighting = NULL;

      if ( fch->position == POS_FIGHTING )
         fch->position = POS_STANDING;
    }
  }

  update_pos(ch);
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *corpse;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  char *name;

  if ( IS_NPC(ch) )
  {
    name = ch->short_descr;
    corpse = create_object( get_obj_index(OBJ_VNUM_CORPSE_NPC), 0 );
    corpse->timer = 1;
    corpse->value[2]= ch->pIndexData->vnum;

    if ( ch->gold > 0 )
    {
      obj_to_obj( create_money( ch->gold ), corpse );
      ch->gold = 0;
    }
  }
  else {
          name = ch->name;
          corpse = create_object( get_obj_index(OBJ_VNUM_CORPSE_PC), 0 );
          corpse->timer = 10;

          if ( ch->gold > 0 )
             {
                obj = create_money( ch->gold );

                if ( IS_AFFECTED(ch,AFF_SHADOWPLANE) && ( !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
                   SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);

                obj_to_obj( obj, corpse );
                ch->gold = 0;
             }
       }

  sprintf( buf, corpse->short_descr, name );
  free_string( corpse->short_descr );
  corpse->short_descr = str_dup( buf );
  sprintf( buf, corpse->description, name );
  free_string( corpse->description );
  corpse->description = str_dup( buf );

  for ( obj = ch->carrying; obj != NULL; obj = obj_next )
  {
    obj_next = obj->next_content;
    obj_from_char( obj );

    if ( IS_SET( obj->extra_flags, ITEM_VANISH ) )
       extract_obj( obj );
       else {
               if ( IS_AFFECTED(ch,AFF_SHADOWPLANE) && !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
                  SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);

               if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
                  obj_to_room( obj, ch->in_room );
                  else obj_to_obj( obj, corpse );
            }
  }

  if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
     SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);

  obj_to_room( corpse, ch->in_room );
}

void death_cry( CHAR_DATA *ch )
{
}

void make_part( CHAR_DATA *ch, char *argument )
{
  char  arg [MAX_INPUT_LENGTH];
  int vnum;

  argument = one_argument(argument,arg);
  vnum = 0;

  if (arg[0] == '\0') return;
  if (!str_cmp(arg,"head")) vnum = OBJ_VNUM_SEVERED_HEAD;
  else if (!str_cmp(arg,"arm")) vnum = OBJ_VNUM_SLICED_ARM;
  else if (!str_cmp(arg,"leg")) vnum = OBJ_VNUM_SLICED_LEG;
  else if (!str_cmp(arg,"heart")) vnum = OBJ_VNUM_TORN_HEART;
  else if (!str_cmp(arg,"turd")) vnum = OBJ_VNUM_TORN_HEART;
  else if (!str_cmp(arg,"entrails")) vnum = OBJ_VNUM_SPILLED_ENTRAILS;
  else if (!str_cmp(arg,"brain")) vnum = OBJ_VNUM_QUIVERING_BRAIN;
  else if (!str_cmp(arg,"eyeball")) vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
  else if (!str_cmp(arg,"blood")) vnum = OBJ_VNUM_SPILT_BLOOD;
  else if (!str_cmp(arg,"face")) vnum = OBJ_VNUM_RIPPED_FACE;
  else if (!str_cmp(arg,"windpipe")) vnum = OBJ_VNUM_TORN_WINDPIPE;
  else if (!str_cmp(arg,"cracked_head")) vnum = OBJ_VNUM_CRACKED_HEAD;
  else if (!str_cmp(arg,"ear")) vnum = OBJ_VNUM_SLICED_EAR;
  else if (!str_cmp(arg,"nose")) vnum = OBJ_VNUM_SLICED_NOSE;
  else if (!str_cmp(arg,"tooth")) vnum = OBJ_VNUM_KNOCKED_TOOTH;
  else if (!str_cmp(arg,"tongue")) vnum = OBJ_VNUM_TORN_TONGUE;
  else if (!str_cmp(arg,"hand")) vnum = OBJ_VNUM_SEVERED_HAND;
  else if (!str_cmp(arg,"foot")) vnum = OBJ_VNUM_SEVERED_FOOT;
  else if (!str_cmp(arg,"thumb")) vnum = OBJ_VNUM_SEVERED_THUMB;
  else if (!str_cmp(arg,"index")) vnum = OBJ_VNUM_SEVERED_INDEX;
  else if (!str_cmp(arg,"middle")) vnum = OBJ_VNUM_SEVERED_MIDDLE;
  else if (!str_cmp(arg,"ring")) vnum = OBJ_VNUM_SEVERED_RING;
  else if (!str_cmp(arg,"little")) vnum = OBJ_VNUM_SEVERED_LITTLE;
  else if (!str_cmp(arg,"toe")) vnum = OBJ_VNUM_SEVERED_TOE;

  if ( vnum != 0 )
  {
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    char *name;

    name = IS_NPC(ch) ? ch->short_descr : ch->name;
    obj = create_object( get_obj_index( vnum ), 0 );

    obj->timer = 1;

    if (!str_cmp(arg,"head") && IS_NPC(ch))
      obj->value[1] = ch->pIndexData->vnum;
    else if (!str_cmp(arg,"head") && !IS_NPC(ch))
      {ch->pcdata->chobj = obj; obj->chobj = ch;obj->timer = 2; obj->item_type = ITEM_HEAD;}
    else if (!str_cmp(arg,"arm"))
      SET_BIT(obj->extra_flags2, ITEM_ARM);
    else if (!str_cmp(arg,"brain") && !IS_NPC(ch) &&  IS_AFFECTED(ch,AFF_POLYMORPH) && IS_HEAD(ch, LOST_HEAD))
    {
      if (ch->pcdata->chobj != NULL) ch->pcdata->chobj->chobj = NULL;
      ch->pcdata->chobj = obj;
      obj->chobj = ch;
      obj->timer = 2;
      obj->item_type = ITEM_HEAD;
    }

    if (!IS_NPC(ch))
    {
      sprintf( buf, obj->name, name );
      free_string( obj->name );
      obj->name = str_dup( buf );
    }
    else
    {
      sprintf( buf, obj->name, "mob" );
      free_string( obj->name );
      obj->name = str_dup( buf );
    }
    sprintf( buf, obj->short_descr, name );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf( buf, obj->description, name );
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
      SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
    if( ch->in_room != NULL )
      obj_to_room( obj, ch->in_room );
  }
}

void raw_kill( CHAR_DATA *victim )
{
  CHAR_DATA *mount;

  if ( IS_NPC( victim ) && victim->pIndexData->vnum > 549 && victim->pIndexData->vnum < 562
       && IS_SET( victim->arena, AFF2_INARENA ) )
     {
        victim->position = POS_STANDING;
        victim->hit = -1000010;
        undo_arena( victim );
        return;
     }

  if ( victim->embraced != NULL )
     {
        victim->embraced->embracing = NULL;
        victim->embraced = NULL;
     }

  stop_fighting( victim, TRUE );
  make_corpse( victim );

  if ( IS_SET(victim->newbits, NEW_DARKNESS) )
     {
         REMOVE_BIT(victim->newbits, NEW_DARKNESS);
         victim->in_room->drows = UMAX( 0, victim->in_room->drows-- );

         if ( victim->in_room->drows == 0 && IS_SET(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
            REMOVE_BIT(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS);
     }

  if ( (mount = victim->mount) != NULL)
  {
    if (victim->mounted == IS_MOUNT)
    {
      act("$n rolls off the corpse of $N.",mount,NULL,victim,TO_ROOM);
      act("You roll off the corpse of $N.",mount,NULL,victim,TO_CHAR);
    }
    else if (victim->mounted == IS_RIDING)
    {
      act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
      act("You fall off $N.",victim,NULL,mount,TO_CHAR);
    }
    mount->mount    = NULL;
    victim->mount   = NULL;
    mount->mounted  = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;
  }

  if ( !IS_SET( victim->more3, MORE3_NOSHOW ) )
     SET_BIT( victim->more3, MORE3_NOSHOW );

  if ( IS_NPC(victim) )
     {
        if ( IS_NPC( victim ) && victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum <= 570 )
           {
              check_level( victim );

              if ( victim == NULL || victim->in_room == NULL )
                 {
                    REMOVE_BIT( victim->more3, MORE3_NOSHOW );
                    return;
                 }
           }

        extract_char( victim, TRUE );
        REMOVE_BIT( victim->more3, MORE3_NOSHOW );

        return;
     }

  if ( victim->attacker != NULL )
     {
        victim->attacker->attacker = NULL;
        victim->attacker = NULL;
     }

  extract_char( victim, FALSE );
  REMOVE_BIT( victim->more3, MORE3_NOSHOW );

  if ( IS_SET( victim->more2, MORE2_TIE ) )
     REMOVE_BIT( victim->more2, MORE2_TIE );
 
  while ( victim->affected )
    affect_remove( victim, victim->affected );

  victim->affected_by = 0;

  if ( victim->binding != NULL )
     {
        victim->binding->binded = NULL;
        victim->binding = NULL;
     }

  if ( victim->binded != NULL )
     {
        victim->binded->binding = NULL;
        victim->binded = NULL;
     }

  if ( IS_CLASS(victim,CLASS_SAIYAN) )
     victim->saiyanab[BODY] = 0;

  free_string( victim->morph );
  victim->morph = str_dup( "" );

  if ( victim->outer_aura != -1 )
     do_aura(victim,"outer none");

  if ( victim->inner_aura != -1 )
      do_aura(victim,"inner none");

  if ( IS_CLASS(victim,CLASS_DRAGON) )
     victim->dragonform = 0;

  if ( IS_SET( victim->elemental, ELE_SHIELD ) )
     REMOVE_BIT( victim->elemental, ELE_SHIELD );

  die_follower( victim );

  REMOVE_BIT(victim->immune, IMM_STAKE);
  REMOVE_BIT(victim->extra, TIED_UP);
  REMOVE_BIT(victim->extra, GAGGED);
  REMOVE_BIT(victim->extra, BLINDFOLDED);
  victim->pcdata->stats[DEMON_POWER] = 0;
  victim->itemaffect   = 0;
  victim->loc_hp[0]    = 0;
  victim->loc_hp[1]    = 0;
  victim->loc_hp[2]    = 0;
  victim->loc_hp[3]    = 0;
  victim->loc_hp[4]    = 0;
  victim->loc_hp[5]    = 0;
  victim->loc_hp[6]    = 0;
  victim->armor  = 100;
  victim->position   = POS_RESTING;
  victim->hit    = UMAX( 1, victim->hit  );
  victim->mana   = UMAX( 1, victim->mana );
  victim->move   = UMAX( 1, victim->move );
  victim->hitroll  = 0;
  victim->damroll  = 0;
  victim->saving_throw = 0;
  victim->carry_weight = 0;
  victim->carry_number = 0;
  do_call(victim,"all");
  victim->safe_timer = 5;
  save_char_obj( victim );
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;
  CHAR_DATA *lch;
  CHAR_DATA *mount;
  int xp;
  int members;

  /*
   * Monsters don't get kill xp's or alignment changes.
   * P-killing doesn't help either.
   * Dying of mortal wounds or poison doesn't give xp to anyone!
   */
  if ((IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch) return;
  members = 0;
  for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
  {
    if ( ch == gch || is_same_group( gch, ch ) )
      members++;
  }
  if ( members == 0 )
  {
    bug( "Group_gain: members.", members );
    members = 1;
  }
  lch = (ch->leader != NULL) ? ch->leader : ch;
  for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
  {
    if ( !is_same_group( gch, ch ) )
       continue;

    xp = xp_compute( gch, victim );

    if ( IS_EVIL(gch) && IS_GOOD(victim) )
    {
      xp *= 1.25;
      send_to_char("#RYES! #GAnother victory for the forces of Evil.#n\n\r",gch);
    }

    if ( IS_GOOD(gch) && IS_EVIL(victim) )
    {
      xp *= 1.25;
      send_to_char("#RYES! #GAnother victory for the forces of Good.#n\n\r",gch);
    }

    if ( IS_EVIL(gch) && IS_EVIL(victim) )
    {
      xp *= 0.75;
      send_to_char("#ROH NO! #GThe forces of evil are diminished.#n\n\r",gch);
    }

    if ( IS_GOOD(gch) && IS_GOOD(victim) )
    {
      xp *= 0.75;
      send_to_char("#ROH NO! #GThe forces of good are diminished.#n\n\r",gch);
    }

    if ( global_exp )
    {
      xp *= 2;
      send_to_char("#sDouble exp!!!\n\r#n",gch);
    }

    if ( (get_age(ch)-17) < 2 )
    {
      xp *= 2;
      send_to_char("#snewbie exp!!!\n\r#n",gch);
    }

    if ( gch->exp > 500000000 || gch->exp < 0 )
       {
          xp = 0;

          if ( gch->exp < 0 )
             gch->exp = 0;
             else gch->exp = 500000000;
       }

    sprintf( buf, "You receive %d experience points.\n\r", xp );
    send_to_char( buf, gch );
    if ((mount = gch->mount) != NULL) send_to_char( buf, mount );
    gain_exp( gch, xp );
  }
}

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
  double xp;

  if ( victim->level > 999 )
     victim->level = 999;
  
  xp = victim->level * 6400;

  /*
   * Adjust for popularity of target:
   *   -1/8 for each target over  'par' (down to -100%)
   *   +1/8 for each target under 'par' (  up to + 25%)
   */
  xp -= xp * number_range( -2, 2 ) / 8;
  xp  = number_range( xp * 3 / 4, xp * 5 / 4 );
  xp  = UMAX( 0, xp );

  if ( !IS_NPC(gch) )
     {
        gch->pcdata->score[SCORE_TOTAL_LEVEL] += victim->level;

        if ( victim->level > gch->pcdata->score[SCORE_HIGH_LEVEL] )
           gch->pcdata->score[SCORE_HIGH_LEVEL] += victim->level;

        gch->pcdata->score[SCORE_TOTAL_XP] += xp;

        if ( xp > gch->pcdata->score[SCORE_HIGH_XP] )
           gch->pcdata->score[SCORE_HIGH_XP] += xp;
     }

  if ( xp < 90000 ) 
     xp = 90000;

  if ( IS_NPC(victim) && ( IS_SET(victim->act, ACT_NOEXP) ) )
     return 0;

  return (int) xp;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
  static char * const attack_table[] =
  {
  "hit", "slice",  "stab",  "slash", "whip", "claw",
  "blast",  "pound", "whack", "chant", "bite", "pierce", "pierce"
  };
  static char * const attack_table2[] =
  {
  "hits", "slices",  "stabs",  "slashes", "whips", "claws",
  "blasts",  "pounds", "whacks", "chants", "bites", "pierces", "pierces"
  };
  char buf1[512], buf2[512], buf3[512], buf4[512];
  const char *vs;
  const char *vp;
  const char *attack;
  const char *attack2;
  int damp;
  int bodyloc;
  char punct;
  int old_dam;
  CHAR_DATA *vch, *vch_next;
  bool rev_dam = FALSE;

  /* If victim's hp are less/equal to 'damp', attacker gets a death blow */
  if ( IS_NPC(victim) )
     damp = -conaneffect;
     else damp = -conaneffect - 10;

  if ( IS_CLASS(victim, CLASS_ASSASSIN) && IS_SET( victim->special, SPC_KNIGHT ) && victim->pkill > 19 )
     damp -= 1000000;

  if ( IS_CLASS(victim, CLASS_FIEND) && IS_SET( victim->more, MORE_WALL ) )
     damp -= 1000000;

  if ( IS_SET( victim->more3, MORE3_DRAGON ) )
     damp -= 750000;

  if ( dam > 0 && ( ch->nodamage > 0 || ( victim->jedi_timer > 0 && ch->ele_attack != ATTACK_LUNAR ) ) )
     dam = old_dam = 0;
     else old_dam = dam;

  if ( ch->absorb_timer > 0 && dam > 0 )
     dam = -dam;

  if ( eipheal == 1 )
     dam *= 2;
     else if ( eipheal == 2 )
             dam /= 2;

  if ( victim->raiding > 0 )
     dam *= 2;

  if ( ch->wfocus > 0 )
     ch->ele_attack = ch->wfocus;

  dam = is_elemental( ch->ele_attack, victim, dam );

  if ( ( IS_SET( victim->arena, AFF2_TEAM1 ) || IS_SET( victim->arena, AFF2_TEAM2 ) ) 
       && IS_SET( victim->arena, AFF2_INARENA ) )
     {
        int team;

        if ( IS_SET( victim->arena, AFF2_TEAM1 ) )
           team = 0;
           else team = 1;

        switch( team_list->power_mode[team] )
        {
           case 5: if ( team_list->power_duration[team] > 7 )
                      dam = 0;
        }
     }

  if ( dam < 0 )
     {
        dam = -dam;
        rev_dam = TRUE;
     }

         if ( dam ==   0 ) { vs = " miss";	vp = " misses";		}
    else if ( dam <= 500 ) { vs = " like a #csch#yool #bgirl#n";	vp = " like a #csch#yool #bgirl#n";	}
    else if ( dam <= 1000 ) { vs = " like #ra BEAST#n"; vp = " like #ra BEAST#n";	}
    else if ( dam <= 2000 ) { vs = " with #eInCaLuLaTeD #yFoRcE#n";	vp = " with #eInCaLuLaTeD #yFoRcE#n";		}
    else if ( dam <= 3000 ) { vs = " into #pOblivion#n";	vp = " into #pOblivion#n";	}
    else if ( dam <= 5000 ) { vs = " with #wNO #yMERCY#n";  vp = " with #wNO #yMERCY#n";}
    else if ( dam <= 6000 ) { vs = " #cbeyond reasoning#n";  vp = " #cbeyond reasoning#n";}  
    else if ( dam <= 7000 ) { vs = " with #rShAt#eTeRiNg DeAf#rnESs#n";  vp = " with #rShAt#eTeRiNg DeAf#rnESs#n";}
    else if ( dam <= 8000 ) { vs = " into the #eDARK #bABYSS#n";  vp = " into the #eDARK #bABYSS#n";}
    else if ( dam <= 9000 ) { vs = " #rLIKE A WYRM#n";  vp = " #rLIKE A WYRM#n";}
    else if ( dam <= 10000 ) { vs = " #wWiTh ImMoRtAl PaSsIoN#n";  vp = " #wWiTh ImMoRtAl PaSsIoN#n";}
    else if ( dam <= 20000 ) { vs = " into #pthe #cnext #wDiMeNsIoN#n";  vp = " into #pthe #cnext #wDiMeNsIoN#n";}
    else if ( dam <= 30000 ) { vs = " like A #rrAbId #eWoLvErViNe#n";  vp = " like A #rrAbId #eWoLvErViNe#n";}
    else if ( dam <= 40000 ) { vs = " like a #wtrue #rD#ee#bm#ci#wgod#n";  vp = " like a #wtrue #rD#ee#bm#ci#wgod#n";}
    else if ( dam <= 50000 ) { vs = " FoR sOmE #wiNsAnE#n dAmAgE!";  vp = " FoR sOmE #wiNsAnE#n dAmAgE!";}
    else if ( dam < 75000 ) { vs = " making them seeing #rST#wA#rRS#n!";  vp = " making them seeing #rST#wA#rRS#n!";}
    else { vs = " with absolute #yPer#yfec#wtion#n!";  vp = " with absolute #yPer#wfect#yion#n!";}

  if ( rev_dam == TRUE )
     dam = -dam;

  if ((victim->hit - dam > damp ) || (dt >= 0 && dt < MAX_SKILL) ||
     (IS_NPC(victim) && IS_SET(victim->act, ACT_NOPARTS)))
  {
    punct   = (dam <= 2500) ? ' ' : ' ';

    if ( dam < 0 )
       {  
          dam = -dam;
          sprintf( buf1, "$n heals $v%s%c[%d]",  vp, punct, dam );
          sprintf( buf2, "You heal $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n heals you%s%c[%d]", vp, punct, dam );
          sprintf( buf4, "$N heals $N%s%c[%d]", vp, punct, dam );
          dam = -dam;
       }
    else if ( dt == TYPE_HIT && !IS_NPC(ch) && !IS_VAMPAFF(ch,VAM_CLAWS) )
    {
      if (dam == 0)
      {
        sprintf( buf1, "$n%s $v%c[%d]",  vp, punct, dam );
        sprintf( buf2, "You%s $N%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n%s you%c", vp, punct );
        sprintf( buf4, "$N%s $N%c", vp, punct );
      }
      else
      {
        sprintf( buf1, "$n hits $v%s%c[%d]",  vp, punct, dam );
        sprintf( buf2, "You hit $N%s%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n hits you%s%c[%d]", vp, punct, dam );
        sprintf( buf4, "$N hits $N%s%c[%d]", vp, punct, dam );
      }
    }
    else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_SET(ch->newbits, NEW_MONKFLAME))
    {
      if (dam >= 1)
      {
        if (!IS_CLASS(ch, CLASS_WEREWOLF))
        {
          sprintf( buf1, "$n's flaming hands hit $v%s%c[%d]", vp, punct, dam );
          sprintf( buf2, "Your flaming hands hit $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n's flaming hands hit you%s%c[%d]", vp, punct, dam );
          sprintf( buf4, "$N's flaming hands hit $N%s%c[%d]", vp, punct, dam );
        }
        else
        {
          sprintf( buf1, "$n's flaming claws hit $v%s%c[%d]", vp, punct, dam );
          sprintf( buf2, "Your flaming claws hit $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n's flaming claws hit you%s%c[%d]", vp, punct, dam );
          sprintf( buf4, "$N's flaming claws hit $N%s%c[%d]", vp, punct, dam );
        }
      }
      else 
      {
        sprintf( buf1, "$n's hit%s $v%c[%d]",  vp, punct, dam );
        sprintf( buf2, "Your hit%s $N%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n hit%s you%c[%d]", vp, punct, dam );
        sprintf( buf4, "$N hit%s $N%c[%d]", vp, punct, dam );
      }
    }
    else
    {
      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && dt >= TYPE_HIT && number_percent() > 75 )
         {
             attack  = attack_table[10];
             attack2 = attack_table2[10];
             ch->pcdata->condition[COND_THIRST] += number_range(100,500);

             if ( ch->pcdata->condition[COND_THIRST] > 10000 )
                ch->pcdata->condition[COND_THIRST] = 10000;

             dt = TYPE_HIT + 10;
         }
      else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_CLAWS) )
      {
        attack  = attack_table[dt - TYPE_HIT + 5];
        attack2 = attack_table2[dt - TYPE_HIT + 5];
      }
      else if ( dt >= 0 && dt < MAX_SKILL )
      {
        attack = skill_table[dt].noun_damage;
        attack2 = skill_table[dt].noun_damage;
      }
      else if (dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
      {
        attack  = attack_table[dt - TYPE_HIT];
        attack2 = attack_table2[dt - TYPE_HIT];
      }
      else
      {
        dt  = TYPE_HIT;
        attack  = attack_table[0];
        attack2 = attack_table2[0];
      }
      if (dam == 0)
      {
        sprintf( buf1, "$n's %s%s $v%c[%d]",  attack, vp, punct, dam );
        sprintf( buf2, "Your %s%s $N%c[%d]",  attack, vp, punct, dam );
        sprintf( buf3, "$n's %s%s you%c[%d]", attack, vp, punct, dam );
        sprintf( buf4, "$N's %s%s $N%c[%d]", attack, vp, punct, dam );
      }
      else if (IS_SET(ch->newbits, NEW_MONKFLAME) && (dam == 0))
      {
        sprintf( buf1, "$n's flaming %s%s $v%c[%d]", attack2, vp, punct, dam );
        sprintf( buf2, "Your flaming %s%s $N%c[%d]",  attack, vp, punct, dam );
        sprintf( buf3, "$n's flaming %s%s you%c[%d]", attack2, vp, punct, dam );
        sprintf( buf4, "$N's flaming %s%s $N%c[%d]", attack2, vp, punct, dam );
      }
      else
      {
        if ( dt >= 0 && dt < MAX_SKILL )
        {
          sprintf( buf1, "$n's %s strikes $v%s%c[%d]",  attack2, vp, punct, dam );
          sprintf( buf2, "Your %s strikes $N%s%c[%d]",  attack, vp, punct, dam );
          sprintf( buf3, "$n's %s strikes you%s%c[%d]", attack2, vp, punct, dam );
          sprintf( buf4, "$N's %s strikes $N%s%c[%d]", attack2, vp, punct, dam );
        }
        else if (IS_SET(ch->newbits, NEW_MONKFLAME))
        {
          sprintf( buf1, "$n's flaming %s hits $v%s%c[%d]", attack2, vp, punct, dam);
          sprintf( buf2, "Your flaming %s hits $N%s%c[%d]", attack, vp, punct, dam);
          sprintf( buf3, "$n's flaming %s hits you%s%c[%d]", attack2, vp, punct, dam);
          sprintf( buf4, "$N's flaming %s hits $N%s%c[%d]", attack2, vp, punct, dam);
        }
        else
        {
          sprintf( buf1, "$n %s $v%s%c[%d]",  attack2, vp, punct, dam );
          sprintf( buf2, "You %s $N%s%c[%d]",  attack, vp, punct, dam );
          sprintf( buf3, "$n %s you%s%c[%d]", attack2, vp, punct, dam );
          sprintf( buf4, "$N %s $N%s%c[%d]", attack2, vp, punct, dam );
        }
      }
    }

    for ( vch = victim->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( victim != vch && !IS_SET(vch->act, PLR_BRIEF2) && !( IS_SET(vch->act, PLR_BRIEF) 
            && ( dt == 37 || dt == 48 || dt == 12 || dt == 1 || dam == 0 ) ) )
          {
             if ( vch->seeself > 0 )
                act( buf4, ch, NULL, vch, TO_VICT );
                else act( buf1, ch, victim, vch, TO_VICT );
          }
    }

    if ( !IS_SET(ch->act, PLR_BRIEF2) && !( IS_SET(ch->act, PLR_BRIEF) 
         && ( dt == 37 || dt == 48 || dt == 12 || dt == 1 || dam == 0 ) ) )
      act( buf2, ch, NULL, victim, TO_CHAR );

    if ( !IS_SET(victim->act, PLR_BRIEF2) && !( IS_SET(victim->act, PLR_BRIEF) 
         && ( dt == 37 || dt == 48 || dt == 12 || dt == 1 || dam == 0 ) ) )
      act( buf3, ch, NULL, victim, TO_VICT );

    if ( dam != 0 )
       {
          ch->total_hits++;
          ch->total_damage = UMIN( 1000000000, ch->total_damage + dam );
       }

    dam = old_dam;
    return;
  }

  show_total_damage( ch, victim );
  dam = old_dam;

  if ( dt == TYPE_HIT && !IS_NPC( ch ) && !IS_VAMPAFF(ch,VAM_CLAWS) &&!IS_VAMPAFF(ch,VAM_FANGS))
  {
    damp=number_range(1,5);
    if ( damp == 1 )
    {
      act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR);
      act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT);
      act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT);
      make_part(victim,"face");
    }
    else if ( damp == 2 )
    {
      act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR);
      act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT);
      act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT);
      make_part(victim,"windpipe");
    }
    else if ( damp == 3 )
    {
      act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR);
      act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT);
      act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT);
      make_part(victim,"entrails");
    }
    else if ( damp == 4 )
    {
      if (!IS_BODY(victim,BROKEN_SPINE))
      SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
      act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR);
      act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT);
      act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 5 )
    {
      act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR);
      act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT);
      act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT);
      if (!IS_BODY(victim,BROKEN_NECK))
      {
        act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR);
        act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM);
        SET_BIT(victim->loc_hp[1],BROKEN_NECK);
      }
    }
    return;
  }
  if (dt >= 0 && dt < MAX_SKILL) attack = skill_table[dt].noun_damage;
  else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
    attack = attack_table[dt - TYPE_HIT];
  else
  {
    dt  = TYPE_HIT;
    attack  = attack_table[0];
  }
  if ( attack == "slash" || attack == "slice" )
  {
    damp=number_range(1,8);
    if ( damp == 1 )
    {
      act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR);
      act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT);
      act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT);
      make_part(victim,"entrails");
    }
    else if ( damp == 2 )
    {
      act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
      act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
      act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 3 )
    {
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
      act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR);
      act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT);
      act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
    }
    else if ( damp == 4 )
    {
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
      act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
      act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
      act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
    }
    else if ( damp == 5 )
    {
      if (!IS_HEAD(victim,BROKEN_SKULL))
      {
        act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT);
        make_part(victim,"brain");
        SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
      }
    }
    else if ( damp == 6 )
    {
      act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR);
      act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT);
      act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 7 )
    {
      if (!IS_ARM_L(victim,LOST_ARM))
      {
        act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
        make_part(victim,"arm");
        SET_BIT(victim->loc_hp[2],LOST_ARM);
        if (!IS_BLEEDING(victim,BLEEDING_ARM_L)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
        if (IS_BLEEDING(victim,BLEEDING_HAND_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
      }
      else if (!IS_ARM_R(victim,LOST_ARM))
      {
        act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
        make_part(victim,"arm");
        SET_BIT(victim->loc_hp[3],LOST_ARM);
        if (!IS_BLEEDING(victim,BLEEDING_ARM_R)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
        if (IS_BLEEDING(victim,BLEEDING_HAND_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
      }
    }
    else if ( damp == 8 )
    {
      if (!IS_LEG_L(victim,LOST_LEG))
      {
        act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
        make_part(victim,"leg");
        SET_BIT(victim->loc_hp[4],LOST_LEG);
        if (!IS_BLEEDING(victim,BLEEDING_LEG_L)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
      }
      else if (!IS_LEG_R(victim,LOST_LEG))
      {
        act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
        make_part(victim,"leg");
        SET_BIT(victim->loc_hp[5],LOST_LEG);
        if (!IS_BLEEDING(victim,BLEEDING_LEG_R)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
      }
    }
  }
  else if ( attack == "stab" || attack == "pierce" )
  {
    damp=number_range(1,5);
    if ( damp == 1 )
    {
      act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR);
      act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT);
      act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
    }
    else if ( damp == 2 )
    {
      act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
      act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
      act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 3 )
    {
      act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR);
      act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT);
      act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 4 )
    {
      act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR);
      act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT);
      act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 5 )
    {
      act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR);
      act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT);
      act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT);
      if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      else if (!IS_HEAD(victim,LOST_EYE_R)) SET_BIT(victim->loc_hp[0],LOST_EYE_R);
      else if (!IS_HEAD(victim,LOST_EYE_L)) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
    }
  }
  else if ( attack == "blast" || attack == "pound" || attack == "whack" )
  {
    damp=number_range(1,3);
    bodyloc = 0;
    if ( damp == 1)
    {
      act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR);
      act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT);
      act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT);
      if (IS_BODY(victim,BROKEN_RIBS_1 )) {bodyloc += 1; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
      if (IS_BODY(victim,BROKEN_RIBS_2 )) {bodyloc += 2; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
      if (IS_BODY(victim,BROKEN_RIBS_4 )) {bodyloc += 4; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
      if (IS_BODY(victim,BROKEN_RIBS_8 )) {bodyloc += 8; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
      if (IS_BODY(victim,BROKEN_RIBS_16)) {bodyloc += 16; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
      bodyloc += number_range(1,3);
      if (bodyloc > 24) bodyloc = 24;
      if (bodyloc >= 16) {bodyloc -= 16; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
      if (bodyloc >= 8) {bodyloc -= 8; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
      if (bodyloc >= 4) {bodyloc -= 4; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
      if (bodyloc >= 2) {bodyloc -= 2; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
      if (bodyloc >= 1) {bodyloc -= 1; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
    }
    else if ( damp == 2)
    {
      act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_CHAR);
      act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_NOTVICT);
      act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, victim, TO_VICT);
      if (!IS_BODY(victim,BROKEN_SPINE)) SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
    }
    else if ( damp == 3)
    {
      if (!IS_HEAD(victim,BROKEN_SKULL))
      {
        act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT);
        make_part(victim,"brain");
        SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
      }
      else
      {
        act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, TO_CHAR);
        act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, TO_NOTVICT);
        act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, TO_VICT);
      }
    }
  }
  else if ( !IS_NPC( ch ) && (attack == "bite" ||IS_VAMPAFF(ch,VAM_FANGS)))
  {
    act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR);
    act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT);
    act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT);
    make_part(victim,"blood");
    if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
    if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
  }
  else if ( !IS_NPC(ch) && (attack == "claw" || IS_VAMPAFF(ch,VAM_CLAWS)))
  {
    damp=number_range(1,2);
    if ( damp == 1 )
    {
      act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
      act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
      act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    if ( damp == 2 )
    {
      if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      }
      else if (!IS_HEAD(victim,LOST_EYE_R))
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_R);
      }
      else if (!IS_HEAD(victim,LOST_EYE_L))
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      }
      else
      {
        act("You claw open $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n claws open $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n claws open $N's chest.", ch, NULL, victim, TO_VICT);
      }
    }
  }
  else if ( attack == "whip" )
  {
    act("You entangle $N around the neck, and squeeze out $S life.", ch, NULL, victim, TO_CHAR);
    act("$n entangles $N around the neck, and squeezes out $S life.", ch, NULL, victim, TO_NOTVICT);
    act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, victim, TO_VICT);
    if (!IS_BODY(victim,BROKEN_NECK)) SET_BIT(victim->loc_hp[1],BROKEN_NECK);
  }
  else if ( attack == "chant" )
  {
    act("You push your palm at $N as $E explodes into a million pieces.", ch, NULL, victim, TO_CHAR);
    act("$n pushes $s palm at you as you explode into a million pieces.", ch, NULL, victim, TO_VICT);
    act("$n pushes $s palm at $N as $E explodes into a million pieces.", ch, NULL, victim, TO_NOTVICT);
    if (!IS_BODY(victim,BROKEN_NECK)) SET_BIT(victim->loc_hp[1],BROKEN_NECK);
  }
  else if ( attack == "suck" || attack == "grep" )
  {
    act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_CHAR);
    act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_NOTVICT);
    act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, TO_VICT);
  }
  else
  {
  /*bug( "Dam_message: bad dt %d.", dt );*/
  }
  return;
}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
  OBJ_DATA *obj;
  char buf [MAX_STRING_LENGTH];

  if (IS_NPC(ch) && victim->level > 2 && number_percent() > 10)
    return;
  if (!IS_IMMUNE(ch, IMM_DISARM))
    return;
  if(((obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
  {
    if(((obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
      return;
  }
  sprintf(buf,"$n disarms you!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf, ch, NULL, victim, TO_VICT    );
  sprintf(buf,"You disarm $N!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf,  ch, NULL, victim, TO_CHAR    );
  sprintf(buf,"$n disarms $N!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf,  ch, NULL, victim, TO_NOTVICT );
  obj_from_char( obj );
  if (IS_SET(obj->extra_flags, ITEM_LOYAL) && ( !IS_NPC(victim) ) )
  {
    act( "$p leaps back into your hand!", victim, obj, NULL, TO_CHAR );
    act( "$p leaps back into $n's hand!",  victim, obj, NULL, TO_ROOM );
    obj_to_char( obj, victim );
    do_wear( victim, obj->name);
  }
  else if (IS_NPC(victim))
    obj_to_char(obj, victim);
  else
    obj_to_room(obj, victim->in_room);
  return;
}

/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf [MAX_STRING_LENGTH];

  if (IS_AFFECTED(victim,AFF_FLYING)) return;
  if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5) return;
  if (!IS_NPC(victim))  /* hmmm, gotta check all class fly powers - JOBO */
  {
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_FLYING))
      return;
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMAFF(victim, DEM_UNFOLDED))
      return;
    if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)) 
      return;
    if (IS_CLASS(victim, CLASS_DRAGON) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)) return;
    if (IS_CLASS(victim, CLASS_ANGEL) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)) return;
    if ( is_martial( victim ) )
       return;
  }
  if ( victim->wait == 0)
  {
    sprintf(buf,"$n trips you and you go down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf, ch, NULL, victim, TO_VICT    );
    sprintf(buf,"You trip $N and $E goes down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_CHAR    );
    sprintf(buf,"$n trips $N and $E goes down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
    victim->position = POS_RESTING;
  }
  return;
}

void dragoon_trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];

    if ( is_martial( victim ) )
       {
          act("You backflip over $n's wings landing on your feet softly.",ch,NULL,victim,TO_VICT);
          act("$N backflips over your wings landing on $S feet softly.",ch,NULL,victim,TO_CHAR);
          act("$N backflips over $n's wings landing on $S feet softly.",ch,NULL,victim,TO_NOTVICT);
          return;
       }

    sprintf(buf,"$n's wings sweep you off your feet and you go down!");
    act( buf, ch, NULL, victim, TO_VICT    );
    sprintf(buf,"Your wings sweep $N off $S feet and $E goes down!");
    act( buf,  ch, NULL, victim, TO_CHAR    );
    sprintf(buf,"$n wings sweep $N off $S feet and $E goes down!");
    act( buf, ch, NULL, victim, TO_NOTVICT );

    victim->wait = 12;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  
  one_argument( argument, arg );

  if ( arg[0] == '\0' )
  {
    send_to_char( "Kill whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
        send_to_char( "They aren't here.\n\r", ch );
        return;
     }

  if ( victim == ch )
     {
        send_to_char( "You cannot kill yourself!\n\r", ch );
        return;
     }

  if ( is_safe( ch, victim ) ) return;

  if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
     {
         act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
         return;
     }

  if ( ch->position == POS_FIGHTING )
     {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
     }

  if ( IS_CLASS(victim, CLASS_DUERGAR ) )
	 {
	    int chance = 70;
          DESCRIPTOR_DATA *d;

         for ( d = descriptor_list; d != NULL; d = d->next )
		 {
         if ( d->character != NULL && d->character != victim && IS_CLASS( d->character, CLASS_DUERGAR ) )
             chance -= 10;
		 }

	   if ( weather_info.sky == SKY_ECLIPSE )
            chance -= 20;

        if ( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) || IS_SET (ch->in_room->area->bits,ROOM_TOTAL_DARKNESS) )
           chance -= 10;

		 chance = UMAX( chance, 30 );

		 if ( number_percent() > chance )
			{
			   act("You grunt and turn around quickly to face $N startling $M!",victim,NULL,ch,TO_CHAR);
			   act("$n grunts and turns around quickly to face $N startling $M!",victim,NULL,ch,TO_NOTVICT);
			   act("$n grunts and turns around quickly to face you startling you!",victim,NULL,ch,TO_VICT);
			   act("You go on the immediate counter attack stopping $N from hitting you first!",victim,NULL,ch,TO_CHAR);
			   act("$n goes on the immediate counter attack stopping $N from hitting $m first!",victim,NULL,ch,TO_NOTVICT);
			   act("$n goes on the immediate counter attack stopping you from hitting $m first!",victim,NULL,ch,TO_VICT);
			
                     set_fighting( ch, victim );

                     if ( victim->fighting != NULL )
                        set_fighting( victim, ch );

                     return;
			}
	 }

  multi_hit( ch, victim, TYPE_UNDEFINED );
}

void do_flee( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *was_in;
  ROOM_INDEX_DATA *now_in;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];    
  int attempt;
  int cost = 0;
  bool noaffect = FALSE;

  if ( IS_SET( ch->more3, MORE3_SOUL ) && ch->move > 750000 )
     {
        do_mienaku( ch, "" );
        return;
     }

  if ( ( victim = ch->fighting ) == NULL )
     {
        if ( ch->position == POS_FIGHTING )
           ch->position = POS_STANDING;

        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
     }

  if ( IS_CLASS( ch, CLASS_DRAGOON ) && !IS_NPC( victim ) )
     {
        send_to_char("You tell yourself in anger '#yI am no $%^$ wuss!#n' as you continue the attack.\n\r",ch);
        return;
     }

  if ( IS_CLASS( ch, CLASS_SAIYAN ) && !IS_NPC( victim ) )
     {
        send_to_char("Do you want to shame yourself to Vegeta and Son Goku? FIGHT like a TRUE warrior!\n\r",ch);
        return;
     }

  if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_PRIDE && !IS_NPC( victim ) )
     {
        send_to_char("A proud fighter NEVER flees.\n\r",ch);
        return;
     }

  if ( IS_SET( ch->in_room->room_flags, ROOM_CATACOMB ) && !( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_DROW ) 
       && ( ch->pcdata->powers[DROW_CLASS] == DROW_MILITIA || ch->pcdata->powers[DROW_CLASS] == DROW_AVATAR ) ) )
     {
        act( "In your frantic attempt to flee, you RuSh straight ONTO an encavement!", ch, NULL, NULL, TO_CHAR );
        act( "In $s frantic attempt to flee, $n RuSheS straight ONTO an encavement!", ch, NULL, NULL, TO_ROOM );

        if ( is_martial( ch ) )
           {
              act( "At the very last second, you spring off the wall gently landing back on your feet.", ch, NULL, NULL, TO_CHAR );
              act( "At the very last second, $n springs off the wall gently landing back on $s feet.", ch, NULL, NULL, TO_ROOM );
              return;
           }

        act( "You stagger backwards from the sheer pain as you collapse onto the ground!", ch, NULL, NULL, TO_CHAR );
        act( "$n staggers backwards from the sheer pain as $e collapses onto the ground!", ch, NULL, NULL, TO_ROOM );
        ch->hit = UMAX( 0, ch->hit - is_elemental( ATTACK_EARTH, ch, number_range( 500000, 750000 ) ) );
        
        update_pos( ch );        

        if ( ch->position <= POS_STUNNED )
           {
              ch->position = POS_STUNNED;
              stop_fighting( ch, TRUE );
           }

        return;
     }

  if ( IS_SET(ch->in_room->room_flags, ROOM_BARRIER) )
     {
        send_to_char("As you try to flee, a powerful blast SmAsH through your mind.\n\r",ch);
        act("As $n tries to flee, a powerful blast SmAsHeS through $s mind.",ch,NULL,NULL,TO_ROOM);
        return;
     }

  if ( IS_SET(ch->more, MORE_IMPALE) )
     {
        send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
        return;
     }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
     {
        send_to_char("It is #c#zimpossible#n to flee from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
        return;
     }

  if ( ch->fight_timer > 0 )
     cost = ch->max_move / 5;

  if ( ch->move < cost )
     {
        send_to_char("You can't find the energy to leave.\n\r",ch);
        return;
     }

  if ( IS_AFFECTED(ch, AFF_WEBBED) )
     {
        send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
        return;
     }

  if ( is_memb(ch) )
     return;

  if ( IS_SET(ch->more, MORE_NET) )
     {
	  send_to_char( "You are trapped in a spiritual net.\n\r", ch );
	  return;
     }

  was_in = ch->in_room;
  for ( attempt = 0; attempt < 6; attempt++ )
  {
    EXIT_DATA *pexit;
    int door;

    if ( IS_CLASS( ch, CLASS_HOBBIT ) && is_oring( ch ) == FALSE )
       door = attempt;
       else door = number_door( );

    if ((pexit = was_in->exit[door] ) == 0 || pexit->to_room == NULL
      || IS_SET(pexit->exit_info, EX_CLOSED) || ( IS_NPC(ch)
      &&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
      continue;
    move_char( ch, door );

    if ( IS_SET(ch->newbits, NEW_DARKNESS) && !armageddon )
	 {
		REMOVE_BIT(ch->newbits, NEW_DARKNESS);

            if ( ch->in_room->drows > 0 )
               {
                  ch->in_room->drows--;

    	            if ( ch->in_room->drows == 0 && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
                     REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
               }
	 }

    if (( now_in = ch->in_room ) == was_in)
      continue;

    char_from_room( ch );
    char_to_room( ch, was_in );

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
       REMOVE_BIT(ch->act, PLR_WIZINVIS);

    if ( IS_CLASS( ch, CLASS_HOBBIT ) && IS_SET( ch->more3, MORE3_HOUSE ) && !IS_CLASS( victim, CLASS_ANGEL )
         && ch->hit <= ch->max_hit * 0.25 && ch->ctimer[5] == 0 )
       {
          act( "Completely Terrified, you cower to the ground allowing $N to do $S worst.", ch, NULL, victim, TO_CHAR );
          act( "Completely Terrified, $n cowers to the ground allowing you to do your worst.", ch, NULL, victim, TO_VICT );
          act( "Completely Terrified, $n cowers to the ground allowing $N to do $S worst.", ch, NULL, victim, TO_NOTVICT );
          act( "Suddenly, an beautiful winged figure swoops down from the heavens in front of you.", ch, NULL, NULL, TO_CHAR );
          act( "Suddenly, an beautiful winged figure swoops down from the heavens in front of $n.", ch, NULL, NULL, TO_ROOM );
          act( "Bading you to leave, the angel suddenly SlAmS itself into $N sending $M sprawling.", ch, NULL, victim, TO_CHAR );
          act( "Bading $n to leave, the angel suddenly SlAmS itself into you sending you sprawling.", ch, NULL, victim, TO_VICT );
          act( "Bading $n to leave, the angel suddenly SlAmS itself into $N sending $M sprawling.", ch, NULL, victim, TO_NOTVICT );
          act( "As you run from the vicinity, you see the angel smiting $N with a mortal blow!", ch, NULL, victim, TO_CHAR );
          act( "As $n runs from the vicinity, $n sees the angel smiting you with a mortal blow!", ch, NULL, victim, TO_VICT );
          act( "As $n runs from the vicinity, $n sees the angel smiting $N with a mortal blow!", ch, NULL, victim, TO_NOTVICT );

          noaffect = TRUE;
          ch->ctimer[5] = 4;
          hurt_person( ch, victim, is_elemental( ATTACK_HOLY, victim, 1000000 ), TRUE );
       }

    stop_fighting( ch, TRUE );

    if ( !noaffect )
       {
          if ( IS_CLASS( ch, CLASS_WISP ) )
             ch->wait = 4;
             else if ( is_affected( ch, skill_lookup("last prayer") ) )
                     ch->wait = 36;
             else if ( is_affected(ch, skill_lookup("curse of allah") ) )
                     ch->wait = 24;
             else if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_COWARD )
                     ch->wait = 0;
             else if ( arena == FIGHT_WAR )
                     ch->wait = 6;
                     else ch->wait = 12;

          if ( IS_CLASS( ch, CLASS_NINJA ) && ch->wait > 4 )
             ch->wait = UMAX( 4, ch->wait - ( ch->pkill / 2.5 ) ); 

          ch->move = UMAX( 0, ch->move - cost );
       }
       
    if ( ch->fight_timer > 0 )
       ch->agg = 100;

    if ( victim != NULL && IS_CLASS( victim, CLASS_DRAGON_KNIGHT ) && number_percent() > 49
         && victim->position > POS_STUNNED )
       {
          OBJ_DATA *obj;

          if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) != NULL ) && obj->item_type == ITEM_WEAPON
               && obj->value[3] == 2 )
             {
                 act( "You lean back subtively as you place all of your strength into your arms.", victim, NULL, NULL, TO_CHAR );
                 act( "$n leans back subtively as $e places all of $s strength into $s arms.", victim, NULL, NULL, TO_ROOM );
                 act( "You yell '#yYiieah!#n' as you throw your spear towards $N's fleeing figure.", victim, NULL, ch, TO_CHAR );
                 act( "$n yells '#yYiieah!#n' as $e throws $s spear towards $N's fleeing figure.", victim, NULL, ch, TO_NOTVICT );
                 act( "You scream in pain as a spear descends from the sky impaling your chest!", victim, NULL, ch, TO_CHAR );
                 act( "$n screams in pain as a spear descends from the sky impaling $s chest!", victim, NULL, ch, TO_ROOM );
                 victim->attack_type = ATTACK_PHYSICAL;
                 victim->attack_var = PIERCE;
                 victim->attack_focus = OVERHEAD;
                 hurt_person( victim, ch, 400000, TRUE );

                 if ( ch == NULL || ch->position < POS_FIGHTING )
                    return;
             }
       }
       else if ( victim != NULL && IS_CLASS( victim, CLASS_NINJA ) && !is_martial( ch ) && number_percent() > 49
                 && victim->position > POS_STUNNED )
       {
          act("As you flee to the $t, $N quickly slips out a dagger throwing it at your throat!",ch,dir_name[door],victim,TO_CHAR);
          act("As $n flees to the $t, you quickly slip out a dagger throwing it at $s throat!",ch,dir_name[door],victim,TO_VICT);
          act("As $n flees to the $t, $N quickly slips out a dagger throwing it at $s throat!",ch,dir_name[door],victim,TO_NOTVICT);

          victim->attack_type = ATTACK_PHYSICAL;
          victim->attack_var = PIERCE;
          victim->attack_focus = DIRECT;
          hurt_person( victim, ch, 300000, TRUE );

          if ( ch == NULL || ch->position < POS_FIGHTING )
             return;
       }
       else act( "$n has fled to the $T!", ch, NULL, dir_name[door], TO_ROOM );

    add_tracks( ch, door );

    char_from_room( ch );
    char_to_room( ch, now_in );

    if ( !IS_NPC(ch) )
      send_to_char( "You flee from combat!  Coward!\n\r", ch );

	if ( victim != NULL && IS_CLASS( victim, CLASS_WISP ) && victim->pkill >= 6 && !IS_NPC( ch )
           && victim->position > POS_STUNNED )
	   {
             act("You glow a brilliant #7white#n colour cooeing '#yPeeyo peeyo'#n as you follow your enemy.",victim,NULL,NULL,TO_CHAR);
		 act("$n glows a brilliant #7white#n colour cooeing '#yPeeyo peeyo'#n as $e follows $s enemy.",victim,NULL,NULL,TO_ROOM);

             stop_fighting( victim, TRUE );
		 move_char( victim, door );

		 if ( victim->position > POS_STUNNED && victim->stunned == 0 && !IS_SET( victim->more, MORE_FROZEN )
                  && !IS_SET( victim->more, MORE_STONE ) && can_see( victim, ch ) )
                multi_hit( victim, ch, TYPE_UNDEFINED );
	   }

    return;
  }

  send_to_char( "You were unable to escape!\n\r", ch );
}

void do_kick( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *boots;
  int dam;
  int hitpoints;

  if (!IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
  {
    send_to_char("First you should learn to kick.\n\r", ch );
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_kick].beats );
  if ((IS_NPC(ch) && ch->level<1800))
  {
    dam=50000;
    damage(ch,victim,dam,gsn_kick);
    return;
  }
  if ((IS_NPC(ch)) ||  number_percent( ) < ch->pcdata->learned[gsn_kick]) dam = number_range(1,4);
  else
  {
    dam = 0;
    damage( ch, victim, dam, gsn_kick );
    return;
  }
  dam += char_damroll(ch);
  if (dam == 0) dam = 1;
  if (!IS_AWAKE(victim)) dam *= 2;
  if(IS_NPC(ch)) dam *= 100;
  if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
  {
    if (IS_SET(victim->special, SPC_WOLFMAN)) dam *= 0.8;
    if ((boots = get_eq_char( ch, WEAR_FEET ) ) != NULL 
    && IS_SET(boots->spectype, SITEM_SILVER)) dam *= 2;
  }
  if (!IS_NPC(ch)) dam = dam + (dam * ((ch->wpn[0]+1) / 100));

  if (!IS_NPC(ch) && !IS_NPC(victim) && dam > 750000) dam = 750000;

  if (dam <= 0) dam = 2;

  hitpoints = victim->hit;

  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES) )
  {
    dam = char_damroll(ch);
    damage( ch, victim, dam, gsn_kick );
  }
  else if ( IS_IMMUNE(victim, IMM_KICK) )
          damage( ch, victim, 0, gsn_kick );
          else damage( ch, victim, dam, gsn_kick );
}


void do_punch( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam;
  int store;
  bool broke = FALSE;

  one_argument(argument,arg);
  if ( IS_NPC(ch) ) return;
  if ( ch->level < skill_table[gsn_punch].skill_level)
  {
    send_to_char( "First you should learn to punch.\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "You cannot punch yourself!\n\r", ch );
    return;
  }
  if (is_safe(ch,victim)) return;
  if ( victim->hit < victim->max_hit )
  {
    send_to_char( "They are hurt and suspicious.\n\r", ch );
    return;
  }
  if ( victim->position < POS_FIGHTING )
  {
    send_to_char( "You can only punch someone who is standing.\n\r", ch );
    return;
  }
  act("You draw your fist back and aim a punch at $N.",ch,NULL,victim,TO_CHAR);
  act("$n draws $s fist back and aims a punch at you.",ch,NULL,victim,TO_VICT);
  act("$n draws $s fist back and aims a punch at $N.",ch,NULL,victim,TO_NOTVICT);
  WAIT_STATE( ch, skill_table[gsn_punch].beats );
  if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_punch] )
    dam = number_range(1,4);
  else
  {
    dam = 0;
    damage( ch, victim, dam, gsn_punch );
    return;
  }
  dam += char_damroll(ch);
  if (dam == 0) dam = 1;
  if (!IS_AWAKE(victim)) dam *= 2;
  if ( !IS_NPC(ch) ) dam = dam + (dam * (ch->wpn[0] / 100));
  if ( dam <= 0 ) dam = 1;
  if (dam > 1000000) dam = 1000000;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
  {
    store = victim->hit;
    victim->hit += dam;
    damage( ch, victim, dam, gsn_punch );
    victim->hit = store;
    if (number_percent() <= 25 && !IS_ARM_L(ch,LOST_ARM) && !IS_ARM_L(ch,LOST_HAND))
    {
      if (!IS_ARM_L(ch, LOST_FINGER_I) && !IS_ARM_L(ch, BROKEN_FINGER_I))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_M) && !IS_ARM_L(ch, BROKEN_FINGER_M))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_R) && !IS_ARM_L(ch, BROKEN_FINGER_R))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_L) && !IS_ARM_L(ch, BROKEN_FINGER_L))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);broke = TRUE;}
      if (broke)
      {
        act("The fingers on your left hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
        act("The fingers on $n's left hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
      }
    }
    else if (number_percent() <= 25 && !IS_ARM_R(ch,LOST_ARM) && !IS_ARM_R(ch,LOST_HAND))
    {
      if (!IS_ARM_R(ch, LOST_FINGER_I) && !IS_ARM_R(ch, BROKEN_FINGER_I))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_M) && !IS_ARM_R(ch, BROKEN_FINGER_M))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_R) && !IS_ARM_R(ch, BROKEN_FINGER_R))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_L) && !IS_ARM_R(ch, BROKEN_FINGER_L))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);broke = TRUE;}
      if (broke)
      {
        act("The fingers on your right hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
        act("The fingers on $n's right hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
      }
    }
    stop_fighting(victim,TRUE);
    return;
  }
  damage( ch, victim, dam, gsn_punch );
  if (victim == NULL || victim->position == POS_DEAD || dam < 1) return;
  if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
  if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
  {
    act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
  }
  else if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_JAW))
  {
    act("Your jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    act("$n's jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
  }
  act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
  act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
  victim->position = POS_STUNNED;
  if( IS_NPC(ch) )
  {
    switch( ch->pIndexData->vnum ){
    case 30003:
    case 90500:
    case 90501:
    case 20050:
    case 30704:
    case 30703:
    case 10000:
    case 10001:
    case 10002:
    case 10003:
    case 10004:
    break;
    default:
    {
      victim->position = POS_STANDING;
      victim->hit = victim->max_hit -100;
    }}
  }
}

/* Hurl skill by KaVir */
void do_hurl( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    CHAR_DATA       *mount;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *pexit;
    EXIT_DATA       *pexit_rev;
    char            buf       [MAX_INPUT_LENGTH];
    char            direction [MAX_INPUT_LENGTH];
    char            arg1      [MAX_INPUT_LENGTH];
    char            arg2      [MAX_INPUT_LENGTH];
    int             door;
    int             rev_dir;
    int             dam, level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1 )
    {
	send_to_char("Maybe you should learn the skill first?\n\r",ch);
	return;
    }

    if ( ch->position != POS_STANDING && !( IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1],DPOWER_LUTH) ) )
    {
	send_to_char("But you are in a fight.\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' )
    {
	send_to_char("Who do you wish to hurl?\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char("They are not here.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you hurl yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
    {
	send_to_char("But they have someone on their back!\n\r",ch);
	return;
    }
    else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	send_to_char("But they are riding!\n\r",ch);
	return;
    }

    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
    {
        send_to_char("You are unable to get their feet of the ground.\n\r",ch);
        return;
    }

    if ( IS_CLASS( ch, CLASS_DROW ) )
       level = 998;
       else level = 300;

    if ( IS_NPC(victim) && victim->level > level && !IS_SET(victim->extra, EXTRA_ZOMBIE) )
    {
      if ( IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1],DPOWER_LUTH) )
         ;
         else {
                 send_to_char("You are unable to get their feet of the ground.\n\r",ch);
                 return; 
              }
    }

    WAIT_STATE( ch, 6 );

    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_hurl] )
    {
	    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
	    multi_hit( victim, ch, TYPE_UNDEFINED );
	    return;
    }

    rev_dir = 0;

    if ( arg2[0] == '\0' )
	door = number_range(0,3);
    else
    {
    	if      ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "north" ) )
	    door = 0;
    	else if ( !str_cmp( arg2, "e" ) || !str_cmp( arg2, "east" ) )
	    door = 1;
    	else if ( !str_cmp( arg2, "s" ) || !str_cmp( arg2, "south" ) )
	    door = 2;
    	else if ( !str_cmp( arg2, "w" ) || !str_cmp( arg2, "west" ) )
	    door = 3;
    	else
    	{
	    send_to_char("You can only hurl people north, south, east or west.\n\r", ch);
	    return;
    	}
    }

    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}

    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
	sprintf(buf,"$n hurls $N into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You hurl $N into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n hurls you into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
	dam = 100000;
	victim->hit = victim->hit - dam;
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	if (victim->position == POS_DEAD) {raw_kill(victim);return;}

      victim->position = POS_STUNNED;
	return;
    }

    pexit = victim->in_room->exit[door];
    if( IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
    {
	sprintf(buf,"$n hurls $N into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You hurl $N into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n hurls you into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
	dam = 100000;
	victim->hit = victim->hit - dam;
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	if (victim->position == POS_DEAD) {raw_kill(victim);return;}

      victim->position = POS_STUNNED;
	return;
    }
    if (IS_SET(pexit->exit_info, EX_CLOSED) && 
	!IS_AFFECTED(victim,AFF_PASS_DOOR ) &&
	!IS_AFFECTED(victim,AFF_ETHEREAL  ) )
    {
	if (IS_SET(pexit->exit_info, EX_LOCKED))
	    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	if (IS_SET(pexit->exit_info, EX_CLOSED))
	    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	sprintf(buf,"$n hoists $N in the air and hurls $M %s.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You hoist $N in the air and hurl $M %s.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n hurls you %s, smashing you through the %s.", direction,pexit->keyword);
	act(buf,ch,NULL,victim,TO_VICT);
	sprintf(buf,"There is a loud crash as $n smashes through the $d.");
	act(buf,victim,NULL,pexit->keyword,TO_ROOM);

	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir] ) != NULL
	&&   pexit_rev->to_room == ch->in_room
	&&   pexit_rev->keyword != NULL )
	{
	    if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
		REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
		REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    if (door == 0) sprintf(direction,"south");
	    if (door == 1) sprintf(direction,"west");
	    if (door == 2) sprintf(direction,"north");
	    if (door == 3) sprintf(direction,"east");
	    char_from_room(victim);
	    char_to_room(victim,to_room);
	    sprintf(buf,"$n comes smashing in through the %s $d.", direction);
	    act(buf,victim,NULL,pexit->keyword,TO_ROOM);
          dam = 100000;
	    victim->hit = victim->hit - dam;
	    update_pos(victim);
    	    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	    if (victim->position == POS_DEAD) {raw_kill(victim);return;}

          victim->position = POS_STUNNED;
	}
    }
    else
    {
	sprintf(buf,"$n hurls $N %s.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You hurl $N %s.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n hurls you %s.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
	if (door == 0) sprintf(direction,"south");
	if (door == 1) sprintf(direction,"west");
	if (door == 2) sprintf(direction,"north");
	if (door == 3) sprintf(direction,"east");
	char_from_room(victim);
	char_to_room(victim,to_room);
	sprintf(buf,"$n comes flying in from the %s.", direction);
	act(buf,victim,NULL,NULL,TO_ROOM);
      dam = 100000;
	victim->hit = victim->hit - dam;
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	if (victim->position == POS_DEAD) {raw_kill(victim);return;}

      victim->position = POS_STUNNED;
    }
}

void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_disarm].skill_level)
    {
  send_to_char( "You don't know how to disarm opponents.\n\r", ch );
  return;
    }

    if ( ( get_eq_char( ch, WEAR_WIELD ) == NULL )
    &&   ( get_eq_char( ch, WEAR_HOLD  ) == NULL ) )
    {
  send_to_char( "You must wield a weapon to disarm.\n\r", ch );
  return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
  send_to_char( "You aren't fighting anyone.\n\r", ch );
  return;
    }

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL ) )
    {
  send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
  return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + victim->level - ch->level;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
  send_to_char( "You failed.\n\r", ch );
    else if ( IS_NPC(ch) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
  disarm( ch, victim );
    else
  send_to_char( "You failed.\n\r", ch );
    return;
}



void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}

/* Had problems with people not dying when POS_DEAD...KaVir */
void do_killperson( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument( argument, arg );
  if ( arg[0] == '\0' ) return;
  if ( ( victim = get_char_room( ch, arg ) ) == NULL ) return;
  send_to_char( "You have been KILLED!!\n\r\n\r", victim );
  if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
  if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
  raw_kill( victim );
  return;
}

bool has_timer(CHAR_DATA *ch) 
{
  if (ch->fight_timer > 0 && !IS_NPC(ch)) 
  {
    send_to_char("Not until your fight timer runs out!\n\r", ch );
    return TRUE;
  }
  return FALSE;
}

void dropinvis(CHAR_DATA *ch)
{
  if (ch->level < 7 && IS_SET(ch->act, AFF_HIDE)) REMOVE_BIT(ch->act, AFF_HIDE);
  if (ch->level < 7 && IS_SET(ch->act, PLR_WIZINVIS)) REMOVE_BIT(ch->act, PLR_WIZINVIS);
}

