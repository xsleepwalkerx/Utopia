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

#define SCREEN_WIDTH		79
int  col_str_len	args( ( char * text ) );

void show_skill( CHAR_DATA *ch, int num )
{
   switch( num )
   {
       case 1: stc( "#wLevel 2 Skill: #yLightningBlade#n\n\r", ch ); break;
       case 2: stc( "#wLevel 2 Skill: #yEnergyBarrier#n\n\r", ch ); break;
       case 3: stc( "#wLevel 2 Skill: #yEarthSmash#n\n\r", ch ); break;
       case 4: stc( "#wLevel 2 Skill: #yIceBreak#n\n\r", ch ); break;
       case 5: stc( "#wLevel 2 Skill: #yShadowThrust#n\n\r", ch ); break;
       case 6: stc( "#wLevel 2 Skill: #yJusticeDance#n\n\r", ch ); break;
       case 7: stc( "#wLevel 2 Skill: #yFireDash#n\n\r", ch ); break;
       case 9: stc( "#wLevel 2 Skill: #yEnergySword#n\n\r", ch ); break;
       case 10: stc( "#wLevel 2 Skill: #yCrescentSlash#n\n\r", ch ); break;
       case 11: stc( "#wLevel 2 Skill: #yGambler#n\n\r", ch ); break;
       case 21: stc( "#wLevel 3 Skill: #ySonicArc#n\n\r", ch ); break;
       case 22: stc( "#wLevel 3 Skill: #yGshield#n\n\r", ch ); break;
       case 23: stc( "#wLevel 3 Skill: #yRecharge #P(Auto)#n\n\r", ch ); break;
       case 24: stc( "#wLevel 3 Skill: #yGrim Reaper #P(Auto)#n\n\r", ch ); break;
       case 25: stc( "#wLevel 3 Skill: #yShockBlade#n\n\r", ch ); break;
       case 26: stc( "#wLevel 3 Skill: #yLightningFist#n\n\r", ch ); break;
       case 27: stc( "#wLevel 3 Skill: #yRoulette#n\n\r", ch ); break;
       case 28: stc( "#wLevel 3 Skill: #yAngelWings#n\n\r", ch ); break;
       case 29: stc( "#wLevel 3 Skill: #yTenhoBlast#n\n\r", ch ); break;
       case 30: stc( "#wLevel 3 Skill: #yGolem#n\n\r", ch ); break;
       case 31: stc( "#wLevel 3 Skill: #yMeteorKick#n\n\r", ch ); break;
       case 32: stc( "#wLevel 3 Skill: #yIceBlast#n\n\r", ch ); break;
       case 33: stc( "#wLevel 3 Skill: #ySustain#n\n\r", ch ); break;
       case 34: stc( "#wLevel 3 Skill: #yDoubleheal#n\n\r", ch ); break;
       case 35: stc( "#wLevel 3 Skill: #yRoyalFlush#n\n\r", ch ); break;
       case 36: stc( "#wLevel 3 Skill: #yDivineMight#n\n\r", ch ); break;
       case 41: stc( "#wLevel 4 Skill: #yGilgamesh #P(Auto)#n\n\r", ch ); break;
       case 42: stc( "#wLevel 4 Skill: #yAntiCross#n\n\r", ch ); break;
       case 43: stc( "#wLevel 4 Skill: #yAragami#n\n\r", ch ); break;
       case 44: stc( "#wLevel 4 Skill: #yAntiLight#n\n\r", ch ); break;
       case 45: stc( "#wLevel 4 Skill: #yThunderBlade#n\n\r", ch ); break;
       case 46: stc( "#wLevel 4 Skill: #yLunarBlade#n\n\r", ch ); break;
       case 47: stc( "#wLevel 4 Skill: #yHWrath#n\n\r", ch ); break;
       case 48: stc( "#wLevel 4 Skill: #yOmegaBlade#n\n\r", ch ); break;
       case 49: stc( "#wLevel 4 Skill: #yRuneBlade#n\n\r", ch ); break;
       case 50: stc( "#wLevel 4 Skill: #yTrueSacrifice#n\n\r", ch ); break;
       case 51: stc( "#wLevel 4 Skill: #yVengeance #P(Auto)#n\n\r", ch ); break;
       case 52: stc( "#wLevel 4 Skill: #yEntangle#n\n\r", ch ); break;
       case 53: stc( "#wLevel 4 Skill: #yKoho#n\n\r", ch ); break;
       case 54: stc( "#wLevel 4 Skill: #yLancelot #P(Auto)#n\n\r", ch ); break;
       case 55: stc( "#wLevel 4 Skill: #yEntomb#n\n\r", ch ); break;
       case 56: stc( "#wLevel 4 Skill: #ySummonSpirit#n\n\r", ch ); break;
       case 57: stc( "#wLevel 4 Skill: #yUltraheal#n\n\r", ch ); break;
       case 58: stc( "#wLevel 4 Skill: #ySubsitance#n\n\r", ch ); break;
       case 59: stc( "#wLevel 4 Skill: #yRevenga#n\n\r", ch ); break;
       case 60: stc( "#wLevel 4 Skill: #yMegaStrike#n\n\r", ch ); break;
       case 61: stc( "#wLevel 5 Skill: #yMirrorStance#n\n\r", ch ); break;
       case 62: stc( "#wLevel 5 Skill: #yXbuster#n\n\r", ch ); break;
       case 63: stc( "#wLevel 5 Skill: #yForeverZero#n\n\r", ch ); break;
       case 64: stc( "#wLevel 5 Skill: #yRCannon#n\n\r", ch ); break;
       case 65: stc( "#wLevel 5 Skill: #yExecutioner#n\n\r", ch ); break;
       case 66: stc( "#wLevel 5 Skill: #yExplode#n\n\r", ch ); break;
       case 67: stc( "#wLevel 5 Skill: #yFireBomb#n\n\r", ch ); break;
       case 68: stc( "#wLevel 5 Skill: #yLunarShield#n\n\r", ch ); break;
       case 69: stc( "#wLevel 5 Skill: #ySatellite#n\n\r", ch ); break;
       case 70: stc( "#wLevel 5 Skill: #yYamiSugi#n\n\r", ch ); break;
       case 71: stc( "#wLevel 5 Skill: #yVoodoo#n\n\r", ch ); break;
       case 72: stc( "#wLevel 5 Skill: #yRHammer#n\n\r", ch ); break;
       case 73: stc( "#wLevel 5 Skill: #yLightBlade#n\n\r", ch ); break;
       case 74: stc( "#wLevel 5 Skill: #yZanRetsuKen#n\n\r", ch ); break;
       case 75: stc( "#wLevel 5 Skill: #yLloth #P(Auto)#n\n\r", ch ); break;
       case 76: stc( "#wLevel 5 Skill: #yDarkBall#n\n\r", ch ); break;
       case 77: stc( "#wLevel 5 Skill: #yAbsorb#n\n\r", ch ); break;
       case 78: stc( "#wLevel 5 Skill: #yHighwind #P(Auto)#n\n\r", ch ); break;
       case 79: stc( "#wLevel 5 Skill: #yOverDrive#n\n\r", ch ); break;
       case 80: stc( "#wLevel 5 Skill: #yAntiStock#n\n\r", ch ); break;
       case 81: stc( "#wLevel 5 Skill: #yDivineBall#n\n\r", ch ); break;
       case 82: stc( "#wLevel 5 Skill: #yDivineCannon#n\n\r", ch ); break;
   }
}

void add_skill( OBJ_DATA *obj, int num )
{
   /*
    * 1 - LightningBlade
    * 2 - EnergyBarrier
    * 3 - EarthSmash
    * 4 - IceBreak
    * 5 - ShadowThrust
    * 6 - JusticeDance
    * 7 - FireDash
    * 9 - EnergySword
    * 10 - CrescentSlash
    * 11 - Gambler
    *
    * 21 - SonicArc
    * 22 - Gshield
    * 23 - Recharge
    * 24 - Grim Reaper
    * 25 - ShockBlade
    * 26 - LightningFist
    * 27 - Roulette
    * 28 - AngelWings
    * 29 - TenhoBlast
    * 30 - Golem
    * 31 - MeteorKick
    * 32 - IceBlast
    * 33 - Sustain
    * 34 - Doubleheal
    * 35 - Royalflush
    * 36 - DivineMight
    *
    * 41 - Gilgamesh
    * 42 - AntiCross
    * 43 - Aragami   
    * 44 - Antilight  
    * 45 - Thunderblade  
    * 46 - LunarBlade  
    * 47 - HWrath  
    * 48 - Omegablade  
    * 49 - RuneBlade  
    * 50 - TrueSacrifice  
    * 51 - Vengeance  
    * 52 - Entangle
    * 53 - Koho
    * 54 - Lancelot
    * 55 - Entomb
    * 56 - Summon
    * 57 - Ultraheal
    * 58 - Subsitance
    * 59 - Revenga
    * 60 - MegaStrike
    *
    * 61 - Mirrorstance
    * 62 - XBuster
    * 63 - ForeverZero
    * 64 - RCannon
    * 65 - Executioner
    * 66 - Explode
    * 67 - Firebomb
    * 68 - LunarShield
    * 69 - Satellite
    * 70 - YamiSugi
    * 71 - Voodoo
    * 72 - RHammer
    * 73 - Lightblade
    * 74 - ZanRetsuKen
    * 75 - Lloth
    * 76 - DarkBall
    * 77 - Absorb
    * 78 - Highwind
    * 79 - Overdrive
    * 80 - AntiStock

    * 81 - DivineBall
    * 82 - DivineCannon
    */
   if ( obj->value[1] == 0 )
      {
         obj->value[1] = num;
         obj->cost += ( 250000 * obj->level );
      }
      else if ( obj->value[2] == 0 && obj->value[1] != num )
              {
                 obj->value[2] = num;
                 obj->cost += ( 500000 * obj->level );
              }
}

void find_item( CHAR_DATA *ch, int obj_level )
{
   char buf[MAX_INPUT_LENGTH];
   char buf2[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   OBJ_INDEX_DATA *pObjIndex;
   AFFECT_DATA *paf;
   char *ore_name, *ore_list, *ele_name;
   int option, bonus = 1, level, counter = 1, count, num = 0, choice, modifier;
   int extra_bonus = 0;

   if ( IS_NPC( ch ) || ( ch->tier < 2 && obj_level == 0 ) )
      return;

   if ( obj_level > 0 )
      level = ch->tier + 1;
      else if ( obj_level == -1 )
              level = ch->tier;
      else level = number_range( ch->tier - 1, ch->tier );

   level = URANGE( 2, level, 5 );

   if ( obj_level == 2 )
      { 
         ore_name = str_dup( "#eD#Ra#ee#Rd#er#Ri#ec#C" );
         ore_list = str_dup( "daedric" );
         extra_bonus = 2;
      }
      else if ( obj_level == 1 )
              {
                 ore_name = str_dup( "#gG#el#ga#es#gs" );
                 ore_list = str_dup( "glass" );
                 extra_bonus = 1;
              }
              else switch( level )
                   {
                     case 2: if ( number_range( 1, 2 ) == 1 )
                                {
                                   ore_name = str_dup( "#RRuby" );
                                   ore_list = str_dup( "ruby" );
                                }
                                else {
                                        ore_name = str_dup( "#eSilver" );
                                        ore_list = str_dup( "silver" );
                                     }
                             break;

                     case 3: if ( number_range( 1, 2 ) == 1 )
                                {
                                   ore_name = str_dup( "#wDiamond" );
                                   ore_list = str_dup( "diamond" );
                                }
                                else {
                                        ore_name = str_dup( "#yGold" );
                                        ore_list = str_dup( "gold" );
                                     }
                             break;

                     case 4: ore_name = str_dup( "#CCrystal" );
                             ore_list = str_dup( "crystal" );
                             break;

                     case 5: ore_name = str_dup( "#wMith#eril#w" );
                             ore_list = str_dup( "mithril" );
                             break;
                   }

   if ( ( pObjIndex = get_obj_index( 820 ) ) == NULL )
      {
         send_to_char("Missing object, please inform an Administrator.\n\r",ch);
         return;
      }

   obj = create_object( pObjIndex, 60 );
   obj->questowner = str_dup( ch->pcdata->switchname );
   obj_to_char( obj, ch );

   obj->level = level;
   obj->cost = ( level * 25000 ) + ( extra_bonus * 500000 );

   if ( obj_level > 0 )
      SET_BIT( obj->extra_flags, ITEM_SPECIAL );

   if ( ch->pcdata->forge > 0 )
      {
         if ( ch->pcdata->forge > 100 )
            {
               bonus = 2;
               ch->pcdata->forge -= 100;
            }

         if ( ch->pcdata->forge < 11 )
            option = 1;
            else option = 2;
      }
      else option = number_range( 1, 2 );

   if ( option == 1 )
      {
         obj->item_type = ITEM_WEAPON;
         obj->value[1] = 0;
         obj->value[2] = 0;
         obj->wear_flags = ITEM_WIELD + 1;
         obj->weapflags = 0;

         do
         {
            option = number_range( 1, 20 );

            switch( option )
            {
               case 6:
               case 17: SET_BIT( obj->weapflags, TRUE_FIRE );
                        ele_name = str_dup( "#RFlaming " );
                        break;

               case 5:
               case 10: SET_BIT( obj->weapflags, TRUE_SAINT );
                        ele_name = str_dup( "#wDivine " );
                        break;
            
               default:
               case 20:
               case 2:
               case 18:
               case 11:
               case 15:
               case 1: ele_name = str_dup( "" );
                       break;

               case 19:
               case 3: SET_BIT( obj->weapflags, TRUE_DARK );
                       ele_name = str_dup( "#eUnholy " );
                       break;

               case 14:
               case 7: SET_BIT( obj->weapflags, TRUE_ICE );
                       ele_name = str_dup( "#CChilling " );
                       break;

               case 4:
               case 13: SET_BIT( obj->weapflags, TRUE_EARTH );
                        ele_name = str_dup( "#oStone " );
                        break;

               case 9:
               case 16: SET_BIT( obj->weapflags, TRUE_BOLT );
                        ele_name = str_dup( "#yThunder " );
                        break;

               case 12:
               case 8: SET_BIT( obj->weapflags, TRUE_MOON );
                       ele_name = str_dup( "#oLunar " );
                       break;
            }
         }
         while ( obj_level != 0 && obj->weapflags == 0 );

         if ( ( number_percent() > 75 || extra_bonus == 2 ) && extra_bonus != 1 )
            bonus = 2;

         if ( obj->weapflags == obj->weapflags )
            {
               if ( obj->weapflags > 0 )
                  SET_BIT( obj->weapflags, WEAPON_NOENCASE );

               obj->cost += 50000;

               if ( affect_free == NULL )
                  paf = alloc_perm( sizeof(*paf) );
                  else {
                          paf = affect_free;
                          affect_free = affect_free->next;
                       }

               paf->type = skill_lookup("reserved");
               paf->duration = -1;
               paf->bitvector = 0;
               paf->next = obj->affected;

               if ( extra_bonus > 0 )
                  paf->modifier = number_range( ( ch->tier * 5 ) + ( extra_bonus * 5 ), ( ch->tier * 10 ) + 5 ) * bonus;
                  else paf->modifier = number_range( ch->tier * 5, ch->tier * 9 ) * bonus;

               if ( extra_bonus == 2 || number_percent() > 93 - ( extra_bonus * 10 ) )
                  counter = 2;

               for ( count = 0; count < counter; count++ )
               {
                  switch( option )
                  {
                     default: paf->location = APPLY_NATURALS; count = counter; break;
                     case 9:
                     case 16: switch( level )
                              {
                                 case 5: switch( number_range( 1, 3 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 64 ); break;
                                            case 3: add_skill( obj, 65 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 45 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 3 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 25 ); break;
                                            case 3: add_skill( obj, 26 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 1 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_BOLTS;
                              break;

                     case 8:
                     case 12: switch( level )
                              {
                                 case 5: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 69 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 46 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 29 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 10 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_LUNARS;
                              break;

                     case 4:
                     case 13: switch( level )
                              {
                                 case 5: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 72 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 52 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 30 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 3 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_EARTHS;
                              break;

                     case 7:
                     case 14: switch( level )
                              {
                                 case 5: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 63 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 53 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 32 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 4 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_ICES;
                              break;

                     case 3:
                     case 19: switch( level )
                              {
                                 case 5: switch( number_range( 1, 4 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 70 ); break;
                                            case 4: add_skill( obj, 73 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 48 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 27 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 5 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_DARKS;
                              break;

                     case 5:
                     case 10: switch( level )
                              {
                                 case 5: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 62 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 47 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 28 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 6 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_HOLYS;
                              break;

                     case 6:
                     case 17: switch( level )
                              {
                                 case 5: switch( number_range( 1, 3 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 66 ); break;
                                            case 3: add_skill( obj, 67 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 4: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 49 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 case 3: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 31 ); break;
                                         }

                                         if ( ( count == 0 && obj->value[1] != 0 )
                                              || ( count == 1 && obj->value[2] != 0 ) )
                                            break;

                                 default:
                                 case 2: switch( number_range( 1, 2 ) )
                                         {
                                            case 1: break;
                                            case 2: add_skill( obj, 7 ); break;
                                         }
                                         break;
                              }

                              paf->location = APPLY_FIRES;
                              break;
                  }
               }

               obj->affected = paf;
            }

         if ( bonus == 2 )
            SET_BIT( obj->weapflags, WEAPON_TWO_HANDS );

         if ( ch->pcdata->forge > 0 )
            {
               switch( ch->pcdata->forge )
               {
                  default:
                  case 1: num = 8; break;
                  case 2: num = 1; break;
                  case 3: num = 2; break;
                  case 4: num = 4; break;
                  case 5: num = 3; break;
                  case 6: num = 5; break;
                  case 7: num = 6; break;
                  case 8: num = 7; break;
                  case 9: num = 9; break;
                  case 10: num = 10; break;
               }
            }
            else num = number_range( 1, 10 );

         switch( num )
         {
            default:
            case 1: if ( obj->weapflags > 0 )
                       sprintf( buf, "%s%s Blade#n", ele_name, ore_name );
                       else if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                               sprintf( buf, "%s%s Broadsword#n", ele_name, ore_name );
                               else sprintf( buf, "%s%s Sword#n", ele_name, ore_name );

                    if ( obj->weapflags > 0 )
                       sprintf( buf2, "%s blade", ore_list );
                       else if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                               sprintf( buf2, "%s broadsword", ore_list );
                               else sprintf( buf2, "%s sword", ore_list );

                    obj->value[3] = 3;
                    break;

            case 2: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Lance#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Spear#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s lance", ore_list );
                       else sprintf( buf2, "%s spear", ore_list );

                    obj->value[3] = 2;
                    break;

            case 3: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s SnakeWhip#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Whip#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s snakewhip", ore_list );
                       else sprintf( buf2, "%s whip", ore_list );

                    obj->value[3] = 4;
                    break;

            case 4: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Novel#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Book#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s novel", ore_list );
                       else sprintf( buf2, "%s book", ore_list );

                    obj->value[3] = 9;
                    break;

            case 5: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Katar#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Dagger#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s katar", ore_list );
                       else sprintf( buf2, "%s dagger", ore_list );

                    obj->value[3] = 11;
                    break;

            case 6: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Sceptor#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Rod#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s sceptor", ore_list );
                       else sprintf( buf2, "%s rod", ore_list );

                    obj->value[3] = 7;
                    break;

            case 7: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Long Bow#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Crossbow#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s long bow", ore_list );
                       else sprintf( buf2, "%s crossbow", ore_list );

                    obj->value[3] = 12;
                    break;

            case 8: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Bastard Axe#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Axe#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s bastard axe", ore_list );
                       else sprintf( buf2, "%s axe", ore_list );

                    obj->value[3] = 1;
                    break;

            case 9: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf, "%s%s Grand Staff#n", ele_name, ore_name );
                       else sprintf( buf, "%s%s Staff#n", ele_name, ore_name );

                    if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                       sprintf( buf2, "%s grand staff", ore_list );
                       else sprintf( buf2, "%s staff", ore_list );

                    obj->value[3] = 8;
                    break;

            case 10: if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                        sprintf( buf, "%s%s Claw#n", ele_name, ore_name );
                        else sprintf( buf, "%s%s Tooth#n", ele_name, ore_name );

                     if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
                        sprintf( buf2, "%s claw", ore_list );
                        else sprintf( buf2, "%s tooth", ore_list );

                     obj->value[3] = 5;
                     break;
         }

         free_string( obj->short_descr );
         obj->short_descr = str_dup( buf );
         free_string( obj->name );
         obj->name = str_dup( buf2 );

         num = number_percent();

         if ( num > 89 || extra_bonus == 2 )
            counter = 2;
            else if ( num > 59 || extra_bonus == 1 )
                    counter = 1;
                    else counter = 0;

         obj->cost += ( counter * 50000 );

         num = 0;

         modifier = number_range( ch->tier + extra_bonus, ( ch->tier * 2 ) + extra_bonus ) * bonus;

         if ( affect_free == NULL )
            paf = alloc_perm( sizeof(*paf) );
            else {
                    paf = affect_free;
                    affect_free = affect_free->next;
                 }

         paf->type = skill_lookup("reserved");
         paf->duration = -1;
         paf->modifier = modifier;
         paf->bitvector = 0;

         if ( obj->value[3] == 9 || obj->value[3] == 4 || obj->value[3] == 7 || obj->value[3] == 8 )
            {
               paf->location = APPLY_INT;
               SET_BIT( num, 4 );
            }
            else {
                    paf->location = APPLY_STR;
                    SET_BIT( num, 1 );
                 }

         paf->next = obj->affected;
         obj->affected = paf;

         for ( count = 0; count < counter; count++ )
         {
            switch( number_range( 1, 9 ) )
            {
               default:
               case 1: if ( IS_SET( num, 1 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_STR;

                       if ( obj->value[3] != 4 )
                          modifier = number_range( 1 + extra_bonus, ch->tier + extra_bonus ) * bonus;
                          else modifier = number_range( 2 + extra_bonus, ch->tier + 2 + extra_bonus ) * bonus;

                       SET_BIT( num, 1 );
                       break;

               case 2: if ( IS_SET( num, 2 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_DEX;
                       modifier = number_range( ch->tier + extra_bonus, ( ch->tier * 2 ) + extra_bonus ) * bonus;
                       SET_BIT( num, 2 );
                       break;

               case 3: if ( IS_SET( num, 4 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_INT;
                       modifier = number_range( 1 + extra_bonus, ch->tier + extra_bonus ) * bonus;
                       SET_BIT( num, 4 );
                       break;

               case 4: if ( IS_SET( num, 8 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_WIS;
                       modifier = number_range( 1 + extra_bonus, ch->tier + extra_bonus ) * bonus;
                       SET_BIT( num, 8 );
                       break;

               case 5: if ( IS_SET( num, 16 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_CON;
                       modifier = number_range( 1 + extra_bonus, ch->tier + extra_bonus ) * bonus;
                       SET_BIT( num, 16 );
                       break;

               case 6: if ( IS_SET( num, 32 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_EVA;
                       modifier = number_range( 1 + extra_bonus, ch->tier + extra_bonus ) * bonus;
                       SET_BIT( num, 32 );
                       break;

               case 7: if ( IS_SET( num, 64 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_HIT;
                       modifier = number_range( -50000, ( ch->tier + extra_bonus ) * 50000 ) * bonus;
                       SET_BIT( num, 64 );
                       break;

               case 8: if ( IS_SET( num, 128 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_MANA;
                       modifier = number_range( -50000, ( ch->tier + extra_bonus ) * 50000 ) * bonus;
                       SET_BIT( num, 128 );
                       break;

               case 9: if ( IS_SET( num, 256 ) )
                          {
                             count--;
                             continue;
                          }

                       choice = APPLY_MOVE;
                       modifier = number_range( -50000, ( ch->tier + extra_bonus ) * 50000 ) * bonus;
                       SET_BIT( num, 256 );
                       break;
            }

            if ( affect_free == NULL )
               paf = alloc_perm( sizeof(*paf) );
               else {
                       paf = affect_free;
                       affect_free = affect_free->next;
                    }

            paf->type = skill_lookup("reserved");
            paf->duration = -1;
            paf->modifier = ( modifier == 0 ) ? 1 : modifier;
            paf->bitvector = 0;
            paf->location = choice;
            paf->next = obj->affected;
            obj->affected = paf;
         }

         if ( number_percent() > 89 - ( extra_bonus * 10 ) )
            {
               choice = 0;
               obj->cost += 100000;

               switch( number_range( 1, 4 ) )
               {
                  case 3: if ( IS_SET( obj->weapflags, TRUE_FIRE ) )
                             choice = APPLY_SAVING_FIRE;
                             else if ( IS_SET( obj->weapflags, TRUE_ICE ) )
                                     choice = APPLY_SAVING_ICE;
                             else if ( IS_SET( obj->weapflags, TRUE_BOLT ) )
                                     choice = APPLY_SAVING_THUNDER;
                             else if ( IS_SET( obj->weapflags, TRUE_SAINT ) )
                                     choice = APPLY_SAVING_HOLY;
                             else if ( IS_SET( obj->weapflags, TRUE_DARK ) )
                                     choice = APPLY_SAVING_DARK;
                             else if ( IS_SET( obj->weapflags, TRUE_EARTH ) )
                                     choice = APPLY_SAVING_EARTH;
                             else if ( IS_SET( obj->weapflags, TRUE_MOON ) )
                                     choice = APPLY_SAVING_LUNAR;

                          if ( choice != 0 )
                             {
                                modifier = number_range( ( ch->tier * 2 ) + extra_bonus, ( ch->tier + extra_bonus ) * 5 ) * bonus;
                                break;
                             }
                  default:
                  case 1: choice = APPLY_AGI;
                          modifier = number_range( -2 + extra_bonus, 3 ) * bonus;
                          break;

                  case 2: choice = APPLY_LUCK;
                          modifier = number_range( -2 + extra_bonus, 2 ) * bonus;
                          break;

                  case 4: choice = APPLY_TENSION;
                          modifier = number_range( -2 + extra_bonus, 4 ) * bonus;
                          break;
               }

               if ( affect_free == NULL )
                  paf = alloc_perm( sizeof(*paf) );
                  else {
                          paf = affect_free;
                          affect_free = affect_free->next;
                       }

               paf->type = skill_lookup("reserved");
               paf->duration = -1;
               paf->modifier = ( modifier == 0 ) ? 1 : modifier;
               paf->bitvector = 0;
               paf->location = choice;
               paf->next = obj->affected;
               obj->affected = paf;
            }

         if ( bonus == 2 )
            obj->cost *= 2;
      }
      else
      {
         obj->value[1] = 0;
         obj->value[2] = 0;

         option = number_range( 1, 20 );

         if ( ch->pcdata->forge > 0 )
            {
               switch( ch->pcdata->forge )
               {
                  default:
                  case 11: choice = 1; break;
                  case 12: choice = 2; break;
                  case 13: choice = 3; break;
                  case 14: choice = 5; break;
                  case 15: choice = 4; break;
                  case 16: choice = 9; break;
                  case 17: choice = 10; break;
                  case 18: choice = 11; break;
               }
            }
            else choice = number_range( 1, 8 );

         switch( choice )
         {
             default:
             case 1: sprintf( buf, "%s Helmet#n", ore_name );
                     sprintf( buf2, "%s helmet", ore_list );
                     obj->wear_flags = ITEM_WEAR_HEAD + 1;                       
                     break;

             case 2: sprintf( buf, "%s Armour#n", ore_name );
                     sprintf( buf2, "%s armour", ore_list );
                     obj->wear_flags = ITEM_WEAR_BODY + 1;
                     break;

             case 3: sprintf( buf, "%s Robe#n", ore_name );
                     sprintf( buf2, "%s robe", ore_list );
                     SET_BIT( obj->extra_flags, ITEM_ROBE );
                     obj->wear_flags = ITEM_WEAR_BODY + 1;
                     break;

             case 4: if ( number_range( 1, 2 ) == 1 )
                        {
                           sprintf( buf, "%s Ring#n", ore_name );
                           sprintf( buf2, "%s ring", ore_list );
                        }
                        else {
                                sprintf( buf, "%s Signet#n", ore_name );
                                sprintf( buf2, "%s signet", ore_list );
                             }

                     obj->wear_flags = ITEM_WEAR_FINGER + 1;
                     break;

             case 5: if ( number_range( 1, 2 ) == 1 )
                        {
                           sprintf( buf, "%s Cloak#n", ore_name );
                           sprintf( buf2, "%s cloak", ore_list );
                        }
                        else {
                                sprintf( buf, "%s Cape#n", ore_name );
                                sprintf( buf2, "%s cape", ore_list );
                             }

                     obj->wear_flags = ITEM_WEAR_ABOUT + 1;
                     break;

             case 9: sprintf( buf, "%s Boots#n", ore_name );
                     sprintf( buf2, "%s boots", ore_list );

                     obj->wear_flags = ITEM_WEAR_FEET + 1;
                     obj->cost += 1500000;
                     break;

             case 10: sprintf( buf, "%s Belt#n", ore_name );
                      sprintf( buf2, "%s belt", ore_list );

                      obj->wear_flags = ITEM_WEAR_WAIST + 1;
                      obj->cost += 1500000;
                      break;

             case 11: if ( number_range( 1, 2 ) == 1 )
                         {
                            sprintf( buf, "%s Necklace#n", ore_name );
                            sprintf( buf2, "%s necklace", ore_list );
                         }
                         else {
                                 sprintf( buf, "%s Medallion#n", ore_name );
                                 sprintf( buf2, "%s medallion", ore_list );
                              }

                      obj->wear_flags = ITEM_WEAR_NECK + 1;
                      obj->cost += 1000000;
                      break;
         }

         obj->item_type = ITEM_ARMOR;
         obj->value[0] = 25;
         free_string( obj->short_descr );
         obj->short_descr = str_dup( buf );
         free_string( obj->name );
         obj->name = str_dup( buf2 );

         if ( number_percent() > 59 || extra_bonus > 0 )
            {
               if ( extra_bonus == 2 || number_percent() > 89 - ( extra_bonus * 30 ) )
                  counter = 2;
                  else counter = 1;

               for ( count = 0; count < counter; count++ )
               {
                  switch( level )
                  {
                     case 5: switch( number_range( 1, 21 ) )
                             {
                                default:
                                case 1: break;
                                case 2: add_skill( obj, 61 ); break;
                                case 4: add_skill( obj, 68 ); break;
                                case 6: add_skill( obj, 71 ); break;
                                case 8: add_skill( obj, 74 ); break;
                                case 10: add_skill( obj, 75 ); break;
                                case 12: add_skill( obj, 76 ); break;
                                case 14: add_skill( obj, 77 ); break;
                                case 16: add_skill( obj, 78 ); break;
                                case 18: add_skill( obj, 79 ); break;
                                case 20: add_skill( obj, 80 ); break;
                             }

                             if ( ( count == 0 && obj->value[1] != 0 )
                                  || ( count == 1 && obj->value[2] != 0 ) )
                                break;

                     case 4: switch( number_range( 1, 21 ) )
                             {
                                default:
                                case 1: break;
                                case 2: add_skill( obj, 41 ); break;
                                case 4: add_skill( obj, 42 ); break;
                                case 6: add_skill( obj, 43 ); break;
                                case 8: add_skill( obj, 44 ); break;
                                case 10: add_skill( obj, 50 ); break;
                                case 12: add_skill( obj, 51 ); break;
                                case 14: add_skill( obj, 55 ); break;
                                case 16: add_skill( obj, 56 ); break;
                                case 18: add_skill( obj, 57 ); break;
                                case 20: add_skill( obj, 58 ); break;
                             }

                             if ( ( count == 0 && obj->value[1] != 0 )
                                  || ( count == 1 && obj->value[2] != 0 ) )
                                break;

                     case 3: switch( number_range( 1, 17 ) )
                             {
                                default:
                                case 1: break;
                                case 2: add_skill( obj, 21 ); break;
                                case 4: add_skill( obj, 22 ); break;
                                case 6: add_skill( obj, 23 ); break;
                                case 8: add_skill( obj, 24 ); break;
                                case 10: add_skill( obj, 33 ); break;
                                case 12: add_skill( obj, 34 ); break;
                                case 14: add_skill( obj, 35 ); break;
                                case 16: add_skill( obj, 36 ); break;
                             }

                             if ( ( count == 0 && obj->value[1] != 0 )
                                  || ( count == 1 && obj->value[2] != 0 ) )
                                break;

                     default:
                     case 2: switch( number_range( 1, 7 ) )
                             {
                                default:
                                case 1: break;
                                case 2: add_skill( obj, 2 ); break;
                                case 4: add_skill( obj, 9 ); break;
                                case 6: add_skill( obj, 11 ); break;
                             }

                             if ( ( count == 0 && obj->value[1] != 0 )
                                  || ( count == 1 && obj->value[2] != 0 ) )
                                break;
                  }
               }
            }

         // if ( option != 20 && option != 2 && option != 18 && option != 11 && option != 15 && option != 1 )
         if ( option > 0 )
            {
               if ( affect_free == NULL )
                  paf = alloc_perm( sizeof(*paf) );
                  else {
                          paf = affect_free;
                          affect_free = affect_free->next;
                       }

               paf->type = skill_lookup("reserved");
               paf->duration = -1;
               paf->bitvector = 0;
               paf->next = obj->affected;

               obj->cost += 50000;

               if ( choice == 2 || choice == 3 )
                  paf->modifier = number_range( ( ch->tier * 10 ) + ( extra_bonus * 2 ), ( ch->tier * 18 ) + ( extra_bonus * 7.5 ) );
                  else paf->modifier = number_range( ( ch->tier * 2 ) + ( extra_bonus * 2 ), ( ch->tier * 5 ) + ( extra_bonus * 5 ) );

               switch( option )
               {
                  default:
                  case 9:
                  case 20:
                  case 16: paf->location = APPLY_SAVING_THUNDER;
                           break;

                  case 8:                 
                  case 12: paf->location = APPLY_SAVING_LUNAR;
                           break;

                  case 4:
                  case 13: paf->location = APPLY_SAVING_EARTH;
                           break;

                  case 7:
                  case 14: paf->location = APPLY_SAVING_ICE;
                           break;

                  case 3:
                  case 1:
                  case 19: paf->location = APPLY_SAVING_DARK;
                           break;

                  case 5:
                  case 18:
                  case 10: paf->location = APPLY_SAVING_HOLY;
                           break;

                  case 6:
                  case 11:
                  case 17: paf->location = APPLY_SAVING_FIRE;
                           break;

                  case 2:                 
                  case 15: paf->location = APPLY_SAVING_NATURAL;
                           break;
               }

               obj->affected = paf;
            }

         if ( number_percent() > 0 - ( extra_bonus * 10 ) )
            {
               if ( affect_free == NULL )
                  paf = alloc_perm( sizeof(*paf) );
                  else {
                          paf = affect_free;
                          affect_free = affect_free->next;
                       }

               paf->type = skill_lookup("reserved");
               paf->duration = -1;
               paf->bitvector = 0;
               paf->next = obj->affected;

               // obj->cost += 75000;

               if ( choice == 2 || choice == 3 )
                  paf->modifier = number_range( ( ch->tier * 10 ) + ( extra_bonus * 2 ), ( ch->tier * 18 ) + ( extra_bonus * 7.5 ) );
                  else paf->modifier = number_range( ( ch->tier * 2 ) + ( extra_bonus * 2 ), ( ch->tier * 5 ) + ( extra_bonus * 5 ) );

               switch( number_range( 1, 6 ) )
               {
                  default:
                  case 1: paf->location = APPLY_SAVING_SLASH;
                          break;

                  case 2: paf->location = APPLY_SAVING_PIERCE;
                          break;

                  case 3: paf->location = APPLY_SAVING_BLUNT;
                          break;

                  case 4: paf->location = APPLY_SAVING_BLAST;
                          break;

                  case 5: paf->location = APPLY_SAVING_BEAM;
                          break;

                  case 6: paf->location = APPLY_SAVING_SPIRIT;
                          break;
               }

               obj->affected = paf;
            }

         if ( number_percent() > 89 - ( extra_bonus * 5 ) )
            {
               if ( affect_free == NULL )
                  paf = alloc_perm( sizeof(*paf) );
                  else {
                          paf = affect_free;
                          affect_free = affect_free->next;
                       }

               paf->type = skill_lookup("reserved");
               paf->duration = -1;
               paf->bitvector = 0;
               paf->next = obj->affected;

               obj->cost += 250000;

               paf->modifier = number_range( ( ch->tier * 4 ) + ( extra_bonus * 5 ), ( ch->tier * 6 ) + ( extra_bonus * 5 ) );

               switch( number_range( 1, 2 ) )
               {
                  default:
                  case 1: paf->location = APPLY_SAVING_PHYSICAL;
                          break;

                  case 2: paf->location = APPLY_SAVING_MAGICAL;
                          break;
               }

               obj->affected = paf;
            }

         num = number_percent();

         if ( num > 89 || extra_bonus == 2 )
            counter = 2;
            else if ( num > 59 || extra_bonus == 1 )
                    counter = 1;
                    else counter = 0;
         num = 0;

         if ( choice == 2 || choice == 5 || choice == 3 )
            modifier = number_range( ch->tier + ( extra_bonus * 2 ), ( ch->tier * 4 ) + ( extra_bonus * 2 ) );
            else modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );

         if ( affect_free == NULL )
            paf = alloc_perm( sizeof(*paf) );
            else {
                    paf = affect_free;
                    affect_free = affect_free->next;
                 }

         paf->type = skill_lookup("reserved");
         paf->duration = -1;
         paf->modifier = ( modifier == 0 ) ? 1 : modifier;
         paf->bitvector = 0;

         if ( choice == 5 )
            {
               paf->location = APPLY_EVA;
               SET_BIT( num, 32 );
            }
            else if ( choice == 3 )
            {
               paf->location = APPLY_WIS;
               SET_BIT( num, 8 );
            }
            else {
                    paf->location = APPLY_CON;
                    SET_BIT( num, 16 );
                 }

         paf->next = obj->affected;
         obj->affected = paf;

         obj->cost += ( 50000 * counter );

         for ( count = 0; count < counter; count++ )
         {
            switch( number_range( 1, 9 ) )
            {
               default:
               case 1: if ( IS_SET( num, 1 ) )
                          {
                             count--;
                             continue;
                          }

                       bonus = APPLY_STR;
                       modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );
                       SET_BIT( num, 1 );
                       break;

               case 2: if ( IS_SET( num, 2 ) )
                          {
                             count--;
                             continue;
                          }

                       bonus = APPLY_DEX;
                       modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );
                       SET_BIT( num, 2 );
                       break;

               case 3: if ( IS_SET( num, 4 ) )
                          {
                             count--;
                             continue;
                          }

                       bonus = APPLY_INT;
                       modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );
                       SET_BIT( num, 4 );
                       break;

               case 4: if ( IS_SET( num, 8 ) )
                          {
                             count--;
                             continue;
                          }

                       bonus = APPLY_WIS;

                       if ( choice == 2 )
                          modifier = number_range( ch->tier + extra_bonus, ( ch->tier * 2 ) + extra_bonus );
                          else modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );

                       SET_BIT( num, 8 );
                       break;

               case 5: if ( IS_SET( num, 16 ) )
                          {
                             count--;
                             continue;
                          }

                       bonus = APPLY_CON;

                       if ( choice == 3 )
                          modifier = number_range( ch->tier + extra_bonus, ( ch->tier * 2 ) + extra_bonus );
                          else modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );

                       SET_BIT( num, 16 );
                       break;

               case 6: if ( IS_SET( num, 32 ) )
                          {
                             count--;
                             continue;
                          }

                       bonus = APPLY_EVA;
                       modifier = number_range( 1 + extra_bonus, 1 + ( ch->tier + extra_bonus ) / 2 );
                       SET_BIT( num, 32 );
                       break;
            }

            if ( affect_free == NULL )
               paf = alloc_perm( sizeof(*paf) );
               else {
                       paf = affect_free;
                       affect_free = affect_free->next;
                    }

            paf->type = skill_lookup("reserved");
            paf->duration = -1;
            paf->modifier = ( modifier == 0 ) ? 1 : modifier;
            paf->bitvector = 0;
            paf->location = bonus;
            paf->next = obj->affected;
            obj->affected = paf;
         }

         if ( extra_bonus > 0 || number_percent() > 49 )
         {
            obj->cost += 350000;

            switch( number_range( 1, 3 ) )
            {
               case 2: bonus = APPLY_HIT;

                       if ( choice == 2 || choice == 3 )
                          modifier = number_range( ( ch->tier + extra_bonus ) * 100000, ( ch->tier + extra_bonus ) * 200000 );
                          else modifier = number_range( ( ch->tier + extra_bonus ) * 37500, ( ch->tier + extra_bonus ) * 75000 );

                       break;

               case 3: bonus = APPLY_MANA;

                       if ( choice == 2 || choice == 3 )
                          modifier = number_range( ( ch->tier + extra_bonus ) * 100000, ( ch->tier + extra_bonus ) * 200000 );
                          else modifier = number_range( ( ch->tier + extra_bonus ) * 37500, ( ch->tier + extra_bonus ) * 75000 );

                       break;

               case 1: bonus = APPLY_MOVE;

                       if ( choice == 2 || choice == 3 )
                          modifier = number_range( ( ch->tier + extra_bonus ) * 100000, ( ch->tier + extra_bonus ) * 200000 );
                          else modifier = number_range( ( ch->tier + extra_bonus ) * 37500, ( ch->tier + extra_bonus ) * 75000 );

                       break;
            }

            if ( affect_free == NULL )
               paf = alloc_perm( sizeof(*paf) );
               else {
                       paf = affect_free;
                       affect_free = affect_free->next;
                    }

            paf->type = skill_lookup("reserved");
            paf->duration = -1;
            paf->modifier = ( modifier == 0 ) ? 1 : modifier;
            paf->bitvector = 0;
            paf->location = bonus;
            paf->next = obj->affected;
            obj->affected = paf;
         }

         if ( number_percent() > 89 - ( extra_bonus * 5 ) )
            {
               choice = 0;
               obj->cost += 100000;

               switch( number_range( 1, 4 ) )
               {
                  default:
                  case 3:
                  case 1: choice = APPLY_AGI;
                          modifier = number_range( -2 + extra_bonus, 2 );
                          break;

                  case 2: choice = APPLY_LUCK;
                          modifier = number_range( -1 + extra_bonus, 1 );
                          break;

                  case 4: choice = APPLY_TENSION;
                          modifier = number_range( -1 + extra_bonus, 2 );
                          break;
               }

               if ( affect_free == NULL )
                  paf = alloc_perm( sizeof(*paf) );
                  else {
                          paf = affect_free;
                          affect_free = affect_free->next;
                       }

               paf->type = skill_lookup("reserved");
               paf->duration = -1;
               paf->modifier = ( modifier == 0 ) ? 1 : modifier;
               paf->bitvector = 0;
               paf->location = choice;
               paf->next = obj->affected;
               obj->affected = paf;
            }
      }

   act( "#wYou #gfind a $P #glying down among the #Rbloodshed #gand #echaos.#n", ch, NULL, obj, TO_CHAR );

   if ( obj_level > 0 )
      {
         sprintf( buf, "#gIn #Preward #gfor their #wdedication#g, the #wimms #ggrant them %s#g!#n", obj->short_descr );
         do_sinfo( NULL, buf );
      }
      else act( "#w$n #gfinds a $P #glying down among the #Rbloodshed #gand #echaos.#n", ch, NULL, obj, TO_ROOM );
}

void do_rune_shift( CHAR_DATA *ch, char *argument )
{
   char arg1 [MAX_INPUT_LENGTH];
   char arg2 [MAX_INPUT_LENGTH];
   char arg3 [MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *obj;
   int value, cost = 1500000, option, limit = 10;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
      {
         send_to_char( "#7Syntax: runeshift <object> <field> <value>\n\r\n\r", ch );
         send_to_char( "#7Field being one of:\n\r", ch );
         send_to_char( "#3  str dex min spir vit eva agi luck\n\r", ch );
         send_to_char( "#3  fireres holyres darkres earthres thundres iceres natres lunares\n\r", ch );
         send_to_char( "#3  slashres pierceres blastres beamres bluntres spiritres\n\r", ch );
         return;
      }

   if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
      {
         send_to_char( "You don't have that item in your possession.\n\r", ch );
         return;
      }

   /*
    * Snarf the value (which need not be numeric).
    */
   value = atoi( arg3 );

   if ( !str_cmp( arg2, "str" ) || !str_cmp( arg2, "strength" ) )
      option = APPLY_STR;
      else if ( !str_cmp( arg2, "dex" ) || !str_cmp( arg2, "dexterity" ) )
              option = APPLY_DEX;
      else if ( !str_cmp( arg2, "min" ) || !str_cmp( arg2, "mind" ) )
              option = APPLY_INT;
      else if ( !str_cmp( arg2, "spi" ) || !str_cmp( arg2, "spirit" ) )
              option = APPLY_WIS;
      else if ( !str_cmp( arg2, "vit" ) || !str_cmp( arg2, "vitality" ) )
              option = APPLY_CON;
      else if ( !str_cmp( arg2, "eva" ) || !str_cmp( arg2, "evasion" ) )
              option = APPLY_EVA;
      else if ( !str_cmp( arg2, "agi" ) || !str_cmp( arg2, "agility" ) )
              option = APPLY_AGI;
      else if ( !str_cmp( arg2, "luc" ) || !str_cmp( arg2, "luck" ) )
              {
                 option = APPLY_LUCK;
                 cost = 10000000;
              }
      else if ( !str_cmp( arg2, "ten" ) || !str_cmp( arg2, "tension" ) )
              {
                 option = APPLY_TENSION;
                 cost = 3000000;
              }
      else if ( !str_cmp( arg2, "fireres" ) || !str_cmp( arg2, "fire resilience" ) )
              {
                 option = APPLY_SAVING_FIRE;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "iceres" ) || !str_cmp( arg2, "ice resilience" ) )
              {
                 option = APPLY_SAVING_ICE;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "thundres" ) || !str_cmp( arg2, "thunder resilience" ) )
              {
                 option = APPLY_SAVING_THUNDER;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "earthres" ) || !str_cmp( arg2, "earth resilience" ) )
              {
                 option = APPLY_SAVING_EARTH;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "holyres" ) || !str_cmp( arg2, "holy resilience" ) )
              {
                 option = APPLY_SAVING_HOLY;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "darkres" ) || !str_cmp( arg2, "dark resilience" ) )
              {
                 option = APPLY_SAVING_DARK;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "lunares" ) || !str_cmp( arg2, "lunar resilience" ) )
              {
                 option = APPLY_SAVING_LUNAR;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "natres" ) || !str_cmp( arg2, "natural resilience" ) )
              {
                 option = APPLY_SAVING_NATURAL;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "slashres" ) || !str_cmp( arg2, "slash resilience" ) )
              {
                 option = APPLY_SAVING_SLASH;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "pierceres" ) || !str_cmp( arg2, "pierce resilience" ) )
              {
                 option = APPLY_SAVING_PIERCE;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "blastres" ) || !str_cmp( arg2, "blast resilience" ) )
              {
                 option = APPLY_SAVING_BLAST;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "beamres" ) || !str_cmp( arg2, "beam resilience" ) )
              {
                 option = APPLY_SAVING_BEAM;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "bluntres" ) || !str_cmp( arg2, "blunt resilience" ) )
              {
                 option = APPLY_SAVING_BLUNT;
                 cost = 150000;
              }
      else if ( !str_cmp( arg2, "spiritres" ) || !str_cmp( arg2, "spirit resilience" ) )
              {
                 option = APPLY_SAVING_SPIRIT;
                 cost = 150000;
              }
      else {
              do_rune_shift( ch, "" );
              return;
           }

   if ( cost == 150000 )
      limit = 100;

   if ( !IS_SET( obj->extra_flags, ITEM_SPECIAL ) )
      {
         if ( obj->level == 5 )
            limit *= 0.9;
            else if ( obj->level == 4 )
                    limit *= 0.8;
            else if ( obj->level == 3 )
                    limit *= 0.7;
            else if ( obj->level == 2 )
                    limit *= 0.5;
                    else limit *= 0.3;
      }
      else {
              if ( obj->level == 5 )
                 limit *= 1.1;
                 else if ( obj->level == 4 )
                         limit *= 1.0;
                 else if ( obj->level == 3 )
                         limit *= 0.9;
                 else if ( obj->level == 2 )
                         limit *= 0.7;
                         else limit *= 0.5;
           }

   if ( value > limit )
      {
         sprintf( buf, "#wYou can only #yraise #wthis #Pitem #wby by #C%d#w units.#n\n\r", limit );
         stc( buf, ch );
         return;
      }

   if ( ch->gold < cost * value && ch->bank < cost * value )
      {
         sprintf( buf, "#wYou need to have #C%d #ygold #wto raise this item by #C%d#w.#n\n\r", cost * value, value );
         stc( buf, ch );
         return;
      }

   if ( ch->bank >= cost * value )
      ch->bank -= ( cost * value );
      else ch->gold -= ( cost * value );

   act( "#wYou #gfocus silently on your #Pmental powers #gas you align energy onto #e$P#g.#n", ch, NULL, obj, TO_CHAR );
   act( "#w$n #gfocuses silently on $s #Pmental powers #gas $e aligns energy onto #e$P#g.#n", ch, NULL, obj, TO_ROOM );

   obj->cost = UMIN( 500000000, obj->cost + ( cost * value * 0.5 ) );
   oset_affect( ch, obj, value, option, FALSE );
}

void do_forge(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *obj;
  int old_level, cost;
  bool rare = FALSE;

  if ( IS_NPC(ch) )
     return;

  argument = one_argument( argument, arg );

  if ( arg[0] == '\0' )
     {
        stc("Which item do you want to forge with?\n\r",ch);
        return;
     }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
     {
        stc("You are not carrying that object.\n\r",ch);
        return;
     }

  if ( obj->questowner == NULL || strlen(obj->questowner) < 2 ||
	str_cmp(obj->questowner,ch->pcdata->switchname) )
     {
        send_to_char( "You can only forge your own set of equipment warrior.\n\r", ch );
        return;
     }

  if ( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR )
     {
        act("$p is not a weapon or an armor.",ch,obj,NULL,TO_CHAR);
        return;
     }

  if ( obj->level < 2 )
     {
        act("The material of this object is too cheap to forge from.",ch,obj,NULL,TO_CHAR);
        return;
     }

  if ( ch->tier < obj->level )
     {
        act("Your knowledge of forge isn't great enough to work on this equipment.",ch,obj,NULL,TO_CHAR);
        return;
     }

  if ( IS_SET( obj->quest, QUEST_RELIC ) )
     {
        act("The material of this object is too powerful to forge from.",ch,obj,NULL,TO_CHAR);
        return;
     }

  cost = 500000 + ( obj->level * 500000 );

  argument = one_argument( argument, arg );

  if ( arg[0] == '\0' )
     {
        stc("#g#uForge List#n#g: #yAxe Sword Spear Book Whip Dagger Rod Crossbow Staff Tooth#n\n\r",ch);
        stc("#g#uForge List#n#g: #yHelmet Armour Robe Cape Ring#n\n\r",ch);
        return;
     }

  if ( !str_cmp( arg, "axe" ) )
     ch->pcdata->forge = 1;
     else if ( !str_cmp( arg, "sword" ) )
             ch->pcdata->forge = 2;
     else if ( !str_cmp( arg, "spear" ) )
             ch->pcdata->forge = 3;
     else if ( !str_cmp( arg, "book" ) )
             ch->pcdata->forge = 4;
     else if ( !str_cmp( arg, "whip" ) )
             ch->pcdata->forge = 5;
     else if ( !str_cmp( arg, "dagger" ) )
             ch->pcdata->forge = 6;
     else if ( !str_cmp( arg, "rod" ) )
             ch->pcdata->forge = 7;
     else if ( !str_cmp( arg, "crossbow" ) )
             ch->pcdata->forge = 8;
     else if ( !str_cmp( arg, "staff" ) )
             ch->pcdata->forge = 9;
     else if ( !str_cmp( arg, "tooth" ) )
             ch->pcdata->forge = 10;
     else if ( !str_cmp( arg, "helmet" ) )
             ch->pcdata->forge = 11;
     else if ( !str_cmp( arg, "armour" ) )
             ch->pcdata->forge = 12;
     else if ( !str_cmp( arg, "robe" ) )
             ch->pcdata->forge = 13;
     else if ( !str_cmp( arg, "cape" ) )
             ch->pcdata->forge = 14;
     else if ( !str_cmp( arg, "ring" ) )
             ch->pcdata->forge = 15;
     else if ( !str_cmp( arg, "boots" ) )
             {
                ch->pcdata->forge = 16;
                cost = 15;
                rare = TRUE;
             }
     else if ( !str_cmp( arg, "belt" ) )
             {
                ch->pcdata->forge = 17;
                cost = 15;
                rare = TRUE;
             }
     else if ( !str_cmp( arg, "necklace" ) )
             {
                ch->pcdata->forge = 18;
                cost = 10;
                rare = TRUE;
             }
             else {
                      stc("#g#uForge List#n#g: #yAxe Sword Spear Book Whip Dagger Rod Crossbow Staff Tooth#n\n\r",ch);
                      stc("#g#uForge List#n#g: #yHelmet Armour Robe Cape Ring Belt Boots Necklace#n\n\r",ch);
                      return;
                  }

  if ( rare == TRUE )
     {
        if ( ch->pcdata->faithpoints < cost )
           {
              stc( "#gYou #wdon't have enough #olegend #Ppoints #wto create this item.#n\n\r", ch );
              return;
           }

        ch->pcdata->faithpoints -= cost;
     }
     else {
             if ( ch->gold < cost )
                {
                   stc("You don't have enough #ygold pieces#n.\n\r",ch);
                   return;
                }

             ch->gold -= cost;  
          }

  old_level = ch->tier;
  ch->tier = obj->level;

  if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
     ch->pcdata->forge += 100;

  extract_obj( obj );

  act( "#wYou #gtinker away at a #Rhot #yshaze #gas you work on a #Pnew #gset of equipment.#n", ch, NULL, NULL, TO_CHAR );
  act( "#w$n #gtinkers away at a #Rhot #yshaze #gas $e works on a #Pnew #gset of equipment.#n", ch, NULL, NULL, TO_ROOM );

  if ( number_percent() > 95 )
     find_item( ch, number_range( 1, 2 ) );
     else find_item( ch, -1 );

  ch->tier = old_level;
  ch->pcdata->forge = 0;
}

void centre_text( char *text, CHAR_DATA *ch )
{
    int number;

    if ( ch == NULL ) return;
    if ( col_str_len( text ) > SCREEN_WIDTH-2 )
    {
        bug( "Centre Text - string too long!", 0 ); 
				send_to_char( text, ch );
        return;
    }
    for ( number = 0 ; number <  SCREEN_WIDTH ; number++ )
    {
        send_to_char( " ", ch );
        if ( number == ( ( SCREEN_WIDTH - col_str_len( text ) ) / 2 ) - 1
)
        {
            send_to_char( text, ch );
	    stc("\n\r",ch);
            return;
        }
    }
    return;
}

void line2( CHAR_DATA *ch )
{
    int number;

    if ( ch == NULL )
        return;
    for ( number = 0 ; number < SCREEN_WIDTH ; number++ )
        send_to_char( "*", ch );
    send_to_char( "\n\r", ch );
    return;
}

void line( CHAR_DATA *ch )
{
    int number;

    if ( ch == NULL )
        return;
    for ( number = 0 ; number < SCREEN_WIDTH ; number++ )
        send_to_char( "-", ch );
    send_to_char( "\n\r", ch );
    return;
}


void do_immune( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    send_to_char("                                -= Immunities =-\n\r",ch);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    /* Display weapon resistances */
    send_to_char("Weapons:",ch);
    if (!(!IS_IMMUNE(ch,IMM_SLASH) && !IS_IMMUNE(ch,IMM_STAB) && 
	  !IS_IMMUNE(ch,IMM_SMASH) && !IS_IMMUNE(ch,IMM_ANIMAL) &&
	  !IS_IMMUNE(ch,IMM_MISC )) )
    {
    	if (IS_IMMUNE(ch,IMM_SLASH )) send_to_char(" Slash Slice",ch);
    	if (IS_IMMUNE(ch,IMM_STAB  )) send_to_char(" Stab Pierce",ch);
    	if (IS_IMMUNE(ch,IMM_SMASH )) send_to_char(" Blast Crush Pound",ch);
    	if (IS_IMMUNE(ch,IMM_ANIMAL)) send_to_char(" Claw Bite",ch);
    	if (IS_IMMUNE(ch,IMM_MISC  )) send_to_char(" Grep Whip Suck",ch);
    }
    else
	send_to_char(" None",ch);
    send_to_char(".\n\r",ch);

    /* Display spell immunities */
    send_to_char("Spells :",ch);
    if (!(!IS_IMMUNE(ch,IMM_CHARM)  && !IS_IMMUNE(ch,IMM_HEAT)      && 
	  !IS_IMMUNE(ch,IMM_COLD)   && !IS_IMMUNE(ch,IMM_LIGHTNING) &&
	  !IS_IMMUNE(ch,IMM_ACID)   && !IS_IMMUNE(ch,IMM_SUMMON)    &&
	  !IS_IMMUNE(ch,IMM_VOODOO) && !IS_IMMUNE(ch,IMM_SLEEP)    &&
	  !IS_IMMUNE(ch,IMM_DRAIN)))
    {
    	if (IS_IMMUNE(ch,IMM_CHARM     )) send_to_char(" Charm",ch);
    	if (IS_IMMUNE(ch,IMM_HEAT      )) send_to_char(" Heat",ch);
    	if (IS_IMMUNE(ch,IMM_COLD      )) send_to_char(" Cold",ch);
    	if (IS_IMMUNE(ch,IMM_LIGHTNING )) send_to_char(" Lightning",ch);
    	if (IS_IMMUNE(ch,IMM_ACID      )) send_to_char(" Acid",ch);
    	if (!IS_IMMUNE(ch,IMM_SUMMON   )) send_to_char(" Summon",ch);
    	if (IS_IMMUNE(ch,IMM_VOODOO    )) send_to_char(" Voodoo",ch);
    	if (IS_IMMUNE(ch,IMM_SLEEP     )) send_to_char(" Sleep",ch);
    	if (IS_IMMUNE(ch,IMM_DRAIN     )) send_to_char(" Drain",ch);
    	if (IS_IMMUNE(ch,IMM_TRANSPORT )) send_to_char(" Transport",ch);
    }
    else
	send_to_char(" None",ch);
    send_to_char(".\n\r",ch);

    /* Display skill immunities */
    send_to_char("Skills :",ch);
    if (!(!IS_IMMUNE(ch,IMM_HURL) && !IS_IMMUNE(ch,IMM_BACKSTAB) && 
	  !IS_IMMUNE(ch,IMM_KICK) && !IS_IMMUNE(ch,IMM_DISARM)   &&
	  !IS_IMMUNE(ch,IMM_STEAL)))
    {
    	if (IS_IMMUNE(ch,IMM_HURL      )) send_to_char(" Hurl",ch);
    	if (IS_IMMUNE(ch,IMM_BACKSTAB  )) send_to_char(" Backstab",ch);
    	if (IS_IMMUNE(ch,IMM_KICK      )) send_to_char(" Kick",ch);
    	if (IS_IMMUNE(ch,IMM_DISARM    )) send_to_char(" Disarm",ch);
    	if (IS_IMMUNE(ch,IMM_STEAL     )) send_to_char(" Steal",ch);
    }
    else
	send_to_char(" None",ch);
    send_to_char(".\n\r",ch);

    /* Display vampire immunities */
    send_to_char("Vampire:",ch);
    if (!( IS_IMMUNE(ch,IMM_VAMPIRE) && !IS_IMMUNE(ch,IMM_STAKE) && 
	  !IS_IMMUNE(ch,IMM_SUNLIGHT)))
    {
    	if (!IS_IMMUNE(ch,IMM_VAMPIRE  )) send_to_char(" Bite",ch);
    	if  (IS_IMMUNE(ch,IMM_STAKE    )) send_to_char(" Stake",ch);
    	if  (IS_IMMUNE(ch,IMM_SUNLIGHT )) send_to_char(" Sunlight",ch);
    }
    else
	send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    return;
}


void do_spell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	show_spell(ch,0);
	show_spell(ch,1);
	show_spell(ch,2);
	show_spell(ch,3);
	show_spell(ch,4);
    }
    else if (!str_cmp(arg,"purple")) show_spell(ch,0);
    else if (!str_cmp(arg,"red"))    show_spell(ch,1);
    else if (!str_cmp(arg,"blue"))   show_spell(ch,2);
    else if (!str_cmp(arg,"green"))  show_spell(ch,3);
    else if (!str_cmp(arg,"yellow")) show_spell(ch,4);
    else send_to_char("You know of no such magic.\n\r",ch);
    return;
}

void show_spell( CHAR_DATA *ch, int dtype )
{
    char buf[MAX_INPUT_LENGTH];
    char bufskill[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if      (ch->spl[dtype] == 00 ) sprintf(bufskill,"untrained at");
    else if (ch->spl[dtype] <= 25 ) sprintf(bufskill,"an apprentice at");
    else if (ch->spl[dtype] <= 50 ) sprintf(bufskill,"a student at");
    else if (ch->spl[dtype] <= 75 ) sprintf(bufskill,"a scholar at");
    else if (ch->spl[dtype] <= 100) sprintf(bufskill,"a magus at");
    else if (ch->spl[dtype] <= 125) sprintf(bufskill,"an adept at");
    else if (ch->spl[dtype] <= 150) sprintf(bufskill,"a mage at");
    else if (ch->spl[dtype] <= 175) sprintf(bufskill,"a warlock at");
    else if (ch->spl[dtype] <= 199) sprintf(bufskill,"a master wizard at");
    else if (ch->spl[dtype] >= 300) sprintf(bufskill,"a lich master of");
    else if (ch->spl[dtype] >= 240) sprintf(bufskill,"the complete master of");
    else if (ch->spl[dtype] >= 200) sprintf(bufskill,"a grand sorcerer at");
 
    else return;
    if      (dtype == 0)
	sprintf(buf,"You are %s purple magic.\n\r",bufskill);
    else if (dtype == 1)
	sprintf(buf,"You are %s red magic.\n\r",bufskill);
    else if (dtype == 2)
	sprintf(buf,"You are %s blue magic.\n\r",bufskill);
    else if (dtype == 3)
	sprintf(buf,"You are %s green magic.\n\r",bufskill);
    else if (dtype == 4)
	sprintf(buf,"You are %s yellow magic.\n\r",bufskill);
    else return;
    send_to_char(buf,ch);
    return;
}

void do_scan( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    bool safe = FALSE;
    bool icy = FALSE;

    if ( IS_SET( ch->more, MORE_SAFE ) )
       safe = TRUE;

    if ( IS_SET( ch->in_room->room_flags, ROOM_ICETRAP ) && IS_CLASS( ch, CLASS_SNOW_ELF ) )
       icy = TRUE;

    location = ch->in_room;

    send_to_char("[North]\n\r",ch);
    do_spydirection(ch,"n");
    char_from_room(ch);
    char_to_room(ch,location);

    send_to_char("[East]\n\r",ch);
    do_spydirection(ch,"e");
    char_from_room(ch);
    char_to_room(ch,location);

    send_to_char("[South]\n\r",ch);
    do_spydirection(ch,"s");
    char_from_room(ch);
    char_to_room(ch,location);

    send_to_char("[West]\n\r",ch);
    do_spydirection(ch,"w");
    char_from_room(ch);
    char_to_room(ch,location);

    send_to_char("[Up]\n\r",ch);
    do_spydirection(ch,"u");
    char_from_room(ch);
    char_to_room(ch,location);

    send_to_char("[Down]\n\r",ch);
    do_spydirection(ch,"d");
    char_from_room(ch);
    char_to_room(ch,location);

    if ( safe == TRUE )
       SET_BIT( ch->more, MORE_SAFE );

    if ( icy == TRUE )
       SET_BIT( ch->in_room->room_flags, ROOM_ICETRAP );

    return;
}

void do_spy( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    char            arg1      [MAX_INPUT_LENGTH];
    int             door;

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
	send_to_char("Which direction do you wish to spy?\n\r", ch);
	return;
    }

    if      ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) )
	door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east" ) )
	door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) )
	door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west" ) )
	door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up" ) )
	door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down" ) )
	door = 5;
    else
    {
	send_to_char("You can only spy people north, south, east, west, up or down.\n\r", ch);
	return;
    }

    location = ch->in_room;

    send_to_char("[Short Range]\n\r",ch);
    do_spydirection(ch,arg1);
    send_to_char("\n\r",ch);
    send_to_char("[Medium Range]\n\r",ch);
    do_spydirection(ch,arg1);
    send_to_char("\n\r",ch);
    send_to_char("[Long Range]\n\r",ch);
    do_spydirection(ch,arg1);
    send_to_char("\n\r",ch);

    /* Move them back */
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void do_spydirection( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *pexit;
    CHAR_DATA       *vch;
    CHAR_DATA       *vch_next;
    char            arg      [MAX_INPUT_LENGTH];
    char            buf      [MAX_INPUT_LENGTH];
    int             door;
    bool            is_empty;

    argument = one_argument( argument, arg );

    is_empty = TRUE;

    if ( arg[0] == '\0' )
	return;

    if      ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else return;

    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
	send_to_char("   No exit.\n\r",ch);
	return;
    }
    pexit = ch->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) )
    {
	send_to_char("   Closed door.\n\r",ch);
	return;
    }
    char_from_room(ch);
    char_to_room(ch,to_room);
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;

	if ( vch->in_room == NULL )
	    continue;

      if ( vch->in_room->vnum >= 33500 && vch->in_room->vnum <= 33800 )
         break;

	if ( vch == ch )
	    continue;

	if ( !can_see(ch,vch) )
	    continue;

      if ( IS_CLASS(vch, CLASS_ANGEL) && vch->monkab[BODY] > 5 && IS_SET(vch->more, MORE_LIGHT) )
	    continue;

	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !IS_AFFECTED(vch,AFF_POLYMORPH))
		sprintf(buf,"   %s (Player)\n\r",vch->name);
	    else if (!IS_NPC(vch) && IS_AFFECTED(vch,AFF_POLYMORPH))
		sprintf(buf,"   %s (Player)\n\r",vch->morph);
            else if (IS_NPC(vch))
		sprintf(buf,"   %s\n\r",capitalize(vch->short_descr));
	    send_to_char(buf,ch);
	    is_empty = FALSE;
	    continue;
	}
    }
    if (is_empty)
    {
	send_to_char("   Nobody here.\n\r",ch);
	return;
    }
    return;
}


void take_item( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if (obj == NULL) return;
    obj_from_char(obj);
    obj_to_char(obj,ch);
    act("You wince in pain as $p falls from your body.",ch,obj,NULL,TO_CHAR);
    act("$n winces in pain as $p falls from $m.",ch,obj,NULL,TO_ROOM);
    return;
}

void birth_date( CHAR_DATA *ch, bool is_self )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char mstatus[20];
    char dstatus[20];
    char omin[3];
    char cmin[3];
    char ohour[3];
    char chour[3];
    char oday0[2];
    char cday0[2];
    char oday1[2];
    char cday1[2];
    char omonth[4];
    char cmonth[4];
    char oyear[5];
    char cyear[5];
    char *strtime;
    int oyear2 = 0;
    int cyear2 = 0;
    int omonth2 = 0;
    int cmonth2 = 0;
    int oday2 = 0;
    int cday2 = 0;
    int total = 0;
    int dd = 0;
    int mm = 0;
    int yy = 0;
    bool o_month = FALSE;
    bool c_month = FALSE;

    if (is_self)
	strcpy(buf,ch->createtime);
    else
	strcpy(buf,ch->pcdata->conception);
    if (strlen(buf) < 24) return;
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    strcpy(buf2,strtime);
    oyear[0] = buf[20];
    oyear[1] = buf[21];
    oyear[2] = buf[22];
    oyear[3] = buf[23];
    oyear[4] = '\0';
    omonth[0] = buf[4];
    omonth[1] = buf[5];
    omonth[2] = buf[6];
    omonth[3] = '\0';
    oday0[0] = buf[8];
    oday0[1] = '\0';
    oday1[0] = buf[9];
    oday1[1] = '\0';
    ohour[0] = buf[11];
    ohour[1] = buf[12];
    ohour[2] = '\0';
    omin[0] = buf[14];
    omin[1] = buf[15];
    omin[2] = '\0';

    cyear[0] = buf2[20];
    cyear[1] = buf2[21];
    cyear[2] = buf2[22];
    cyear[3] = buf2[23];
    cyear[4] = '\0';
    cmonth[0] = buf2[4];
    cmonth[1] = buf2[5];
    cmonth[2] = buf2[6];
    cmonth[3] = '\0';
    cday0[0] = buf2[8];
    cday0[1] = '\0';
    cday1[0] = buf2[9];
    cday1[1] = '\0';
    chour[0] = buf2[11];
    chour[1] = buf2[12];
    chour[2] = '\0';
    cmin[0] = buf2[14];
    cmin[1] = buf2[15];
    cmin[2] = '\0';

    if (!str_cmp(omonth,"Dec")) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Nov") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Oct") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Sep") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Aug") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Jul") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Jun") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"May") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Apr") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Mar") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Feb") || o_month) {omonth2 += 28;o_month = TRUE;}
    if (!str_cmp(omonth,"Jan") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!o_month) {send_to_char("Error! Please inform KaVir.\n\r",ch);return;}
    if (!str_cmp(cmonth,"Dec")) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Nov") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Oct") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Sep") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Aug") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jul") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jun") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"May") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Apr") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Mar") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Feb") || c_month) {cmonth2 += 28;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jan") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!c_month) {send_to_char("Error! Please inform KaVir.\n\r",ch);return;}
    if (is_number(oyear)) oyear2 += atoi(oyear);
    if (is_number(cyear)) cyear2 += atoi(cyear);
    if (is_number(oday0)) oday2 += (atoi(oday0) * 10);
    if (is_number(oday1)) oday2 += atoi(oday1);
    if (is_number(cday0)) cday2 += (atoi(cday0) * 10);
    if (is_number(cday1)) cday2 += atoi(cday1);
    total += ((cyear2 - oyear2) * 365);
    total += (cmonth2 - omonth2);
    total += (cday2 - oday2);
    total *= 24; /* Total playing time is now in hours */
    if (is_number(chour) && is_number(ohour))
	total += (atoi(chour) - atoi(ohour));
    total *= 60; /* Total now in minutes */
    if (is_number(cmin) && is_number(omin))
	total += (atoi(cmin) - atoi(omin));
    if (total < 1) total = 0;
	else total /= 12; /* Time now in game days */
    for ( ;; )
    {
	if (total >= 365)
	{
	    total -= 365;
	    yy += 1;
	}
	else if (total >= 30)
	{
	    total -= 30;
	    mm += 1;
	}
	else
	{
	    dd += total;
	    break;
	}
    }
    if (mm == 1) strcpy(mstatus,"month");
	else strcpy(mstatus,"months");
    if (dd == 1) strcpy(dstatus,"day");
	else strcpy(dstatus,"days");
    if (is_self)
    {
       if ( !IS_EXTRA(ch, EXTRA_BORN) ) 
          yy += 17;

       sprintf(buf,"You are %d years, %d %s, and %d %s old\n\r",yy,mm,mstatus,dd,dstatus);
       send_to_char(buf,ch);
    }
    else
    {
	if (yy > 0 || (dd > 0 && mm >= 9) || IS_EXTRA(ch, EXTRA_LABOUR))
	{
	    if (!IS_EXTRA(ch, EXTRA_LABOUR)) SET_BIT(ch->extra, EXTRA_LABOUR);
	    sprintf(buf,"You are ready to give birth.\n\r");
	    send_to_char(buf,ch);
	}
	else if (yy > 0 || mm > 0)
	{
	    sprintf(buf,"You are %d %s and %d %s pregnant.\n\r",
	    mm,mstatus,dd,dstatus);
	    send_to_char(buf,ch);
	}
    }
    return;
}

void other_age( CHAR_DATA *ch, int extra, bool is_preg, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char mstatus[20];
    char dstatus[20];
    char omin[3];
    char cmin[3];
    char ohour[3];
    char chour[3];
    char oday0[2];
    char cday0[2];
    char oday1[2];
    char cday1[2];
    char omonth[4];
    char cmonth[4];
    char oyear[5];
    char cyear[5];
    char *strtime;
    int oyear2 = 0;
    int cyear2 = 0;
    int omonth2 = 0;
    int cmonth2 = 0;
    int oday2 = 0;
    int cday2 = 0;
    int total = 0;
    int dd = 0;
    int mm = 0;
    int yy = 0;
    bool o_month = FALSE;
    bool c_month = FALSE;

    strcpy(buf,argument);
    if (strlen(buf) < 24) return;
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    strcpy(buf2,strtime);
    oyear[0] = buf[20];
    oyear[1] = buf[21];
    oyear[2] = buf[22];
    oyear[3] = buf[23];
    oyear[4] = '\0';
    omonth[0] = buf[4];
    omonth[1] = buf[5];
    omonth[2] = buf[6];
    omonth[3] = '\0';
    oday0[0] = buf[8];
    oday0[1] = '\0';
    oday1[0] = buf[9];
    oday1[1] = '\0';
    ohour[0] = buf[11];
    ohour[1] = buf[12];
    ohour[2] = '\0';
    omin[0] = buf[14];
    omin[1] = buf[15];
    omin[2] = '\0';

    cyear[0] = buf2[20];
    cyear[1] = buf2[21];
    cyear[2] = buf2[22];
    cyear[3] = buf2[23];
    cyear[4] = '\0';
    cmonth[0] = buf2[4];
    cmonth[1] = buf2[5];
    cmonth[2] = buf2[6];
    cmonth[3] = '\0';
    cday0[0] = buf2[8];
    cday0[1] = '\0';
    cday1[0] = buf2[9];
    cday1[1] = '\0';
    chour[0] = buf2[11];
    chour[1] = buf2[12];
    chour[2] = '\0';
    cmin[0] = buf2[14];
    cmin[1] = buf2[15];
    cmin[2] = '\0';

    if (!str_cmp(omonth,"Dec")) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Nov") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Oct") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Sep") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Aug") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Jul") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Jun") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"May") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Apr") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Mar") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Feb") || o_month) {omonth2 += 28;o_month = TRUE;}
    if (!str_cmp(omonth,"Jan") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!o_month) {send_to_char("Error! Please inform KaVir.\n\r",ch);return;}
    if (!str_cmp(cmonth,"Dec")) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Nov") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Oct") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Sep") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Aug") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jul") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jun") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"May") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Apr") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Mar") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Feb") || c_month) {cmonth2 += 28;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jan") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!c_month) {send_to_char("Error! Please inform KaVir.\n\r",ch);return;}
    if (is_number(oyear)) oyear2 += atoi(oyear);
    if (is_number(cyear)) cyear2 += atoi(cyear);
    if (is_number(oday0)) oday2 += (atoi(oday0) * 10);
    if (is_number(oday1)) oday2 += atoi(oday1);
    if (is_number(cday0)) cday2 += (atoi(cday0) * 10);
    if (is_number(cday1)) cday2 += atoi(cday1);
    total += ((cyear2 - oyear2) * 365);
    total += (cmonth2 - omonth2);
    total += (cday2 - oday2);
    total *= 24; /* Total playing time is now in hours */
    if (is_number(chour) && is_number(ohour))
	total += (atoi(chour) - atoi(ohour));
    total *= 60; /* Total now in minutes */
    if (is_number(cmin) && is_number(omin))
	total += (atoi(cmin) - atoi(omin));
    if (total < 1) total = 0;
	else total /= 12; /* Time now in game days */
    for ( ;; )
    {
	if (total >= 365)
	{
	    total -= 365;
	    yy += 1;
	}
	else if (total >= 30)
	{
	    total -= 30;
	    mm += 1;
	}
	else
	{
	    dd += total;
	    break;
	}
    }
    if (mm == 1) strcpy(mstatus,"month");
	else strcpy(mstatus,"months");
    if (dd == 1) strcpy(dstatus,"day");
	else strcpy(dstatus,"days");
    if (!is_preg)
    {
	if (!IS_EXTRA(ch, EXTRA_BORN)) yy += 17;
	sprintf(buf,"Age: %d years, %d %s and %d %s old.\n\r",
	    yy,mm,mstatus,dd,dstatus);
	send_to_char(buf,ch);
    }
    else
    {
	if (yy > 0 || (dd > 0 && mm >= 9) || IS_SET(extra, EXTRA_LABOUR))
	{
	    sprintf(buf,"She is ready to give birth.\n\r");
	    send_to_char(buf,ch);
	}
	else if (yy > 0 || mm > 0)
	{
	    sprintf(buf,"She is %d %s and %d %s pregnant.\n\r",
	    mm,mstatus,dd,dstatus);
	    send_to_char(buf,ch);
	}
    }
    return;
}

int years_old( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char omin[3];
    char cmin[3];
    char ohour[3];
    char chour[3];
    char oday0[2];
    char cday0[2];
    char oday1[2];
    char cday1[2];
    char omonth[4];
    char cmonth[4];
    char oyear[5];
    char cyear[5];
    char *strtime;
    int oyear2 = 0;
    int cyear2 = 0;
    int omonth2 = 0;
    int cmonth2 = 0;
    int oday2 = 0;
    int cday2 = 0;
    int total = 0;
    int yy = 0;
    bool o_month = FALSE;
    bool c_month = FALSE;

    if (IS_NPC(ch)) return 0;

    strcpy(buf,ch->createtime);
    if (strlen(buf) < 24) return 0;
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    strcpy(buf2,strtime);
    oyear[0] = buf[20];
    oyear[1] = buf[21];
    oyear[2] = buf[22];
    oyear[3] = buf[23];
    oyear[4] = '\0';
    omonth[0] = buf[4];
    omonth[1] = buf[5];
    omonth[2] = buf[6];
    omonth[3] = '\0';
    oday0[0] = buf[8];
    oday0[1] = '\0';
    oday1[0] = buf[9];
    oday1[1] = '\0';
    ohour[0] = buf[11];
    ohour[1] = buf[12];
    ohour[2] = '\0';
    omin[0] = buf[14];
    omin[1] = buf[15];
    omin[2] = '\0';

    cyear[0] = buf2[20];
    cyear[1] = buf2[21];
    cyear[2] = buf2[22];
    cyear[3] = buf2[23];
    cyear[4] = '\0';
    cmonth[0] = buf2[4];
    cmonth[1] = buf2[5];
    cmonth[2] = buf2[6];
    cmonth[3] = '\0';
    cday0[0] = buf2[8];
    cday0[1] = '\0';
    cday1[0] = buf2[9];
    cday1[1] = '\0';
    chour[0] = buf2[11];
    chour[1] = buf2[12];
    chour[2] = '\0';
    cmin[0] = buf2[14];
    cmin[1] = buf2[15];
    cmin[2] = '\0';

    if (!str_cmp(omonth,"Dec")) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Nov") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Oct") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Sep") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Aug") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Jul") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Jun") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"May") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Apr") || o_month) {omonth2 += 30;o_month = TRUE;}
    if (!str_cmp(omonth,"Mar") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!str_cmp(omonth,"Feb") || o_month) {omonth2 += 28;o_month = TRUE;}
    if (!str_cmp(omonth,"Jan") || o_month) {omonth2 += 31;o_month = TRUE;}
    if (!o_month) {send_to_char("Error! Please inform KaVir.\n\r",ch);return 0;}
    if (!str_cmp(cmonth,"Dec")) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Nov") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Oct") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Sep") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Aug") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jul") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jun") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"May") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Apr") || c_month) {cmonth2 += 30;c_month = TRUE;}
    if (!str_cmp(cmonth,"Mar") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!str_cmp(cmonth,"Feb") || c_month) {cmonth2 += 28;c_month = TRUE;}
    if (!str_cmp(cmonth,"Jan") || c_month) {cmonth2 += 31;c_month = TRUE;}
    if (!c_month) {send_to_char("Error! Please inform KaVir.\n\r",ch);return 0;}
    if (is_number(oyear)) oyear2 += atoi(oyear);
    if (is_number(cyear)) cyear2 += atoi(cyear);
    if (is_number(oday0)) oday2 += (atoi(oday0) * 10);
    if (is_number(oday1)) oday2 += atoi(oday1);
    if (is_number(cday0)) cday2 += (atoi(cday0) * 10);
    if (is_number(cday1)) cday2 += atoi(cday1);
    total += ((cyear2 - oyear2) * 365);
    total += (cmonth2 - omonth2);
    total += (cday2 - oday2);
    total *= 24; /* Total playing time is now in hours */
    if (is_number(chour) && is_number(ohour))
	total += (atoi(chour) - atoi(ohour));
    total *= 60; /* Total now in minutes */
    if (is_number(cmin) && is_number(omin))
	total += (atoi(cmin) - atoi(omin));
    if (total < 1) total = 0;
	else total /= 12; /* Time now in game days */
    for ( ;; )
    {
	if (total >= 365)
	{
	    total -= 365;
	    yy += 1;
	}
	else break;
    }
    if (!IS_EXTRA(ch, EXTRA_BORN)) yy += 17; /* Starting age for all players */
    return yy;
}

void do_huh( CHAR_DATA *ch, char *argument )
{
    send_to_char("Huh?\n\r",ch);
    return;
}

void do_consent( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
/*
	send_to_char("This has been temporarily disabled.\n\r",ch);
	return;


    if ( strlen(ch->pcdata->marriage) < 2 || !IS_EXTRA(ch, EXTRA_MARRIED) )
    {
	send_to_char("But you are not even married!\n\r",ch);
	return;
    }

*/
    if ( ( victim = get_char_room(ch, arg) ) == NULL )
    {
	send_to_char("They are not here.\n\r",ch);
	return;
    }

    if (IS_NPC(victim))
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }

    if (ch == victim)
    {
	send_to_char("Not on yourself!\n\r",ch);
	return;
    }
/*
    if (strlen(ch->pcdata->marriage) > 1 && 
	str_cmp(ch->pcdata->marriage,victim->name))
    {
	if (IS_EXTRA(ch, EXTRA_MARRIED))
	    send_to_char("But you are married to someone else!\n\r",ch);
	else
	    send_to_char("But you are engaged to someone else!\n\r",ch);
	return;
    }
    if (strlen(victim->pcdata->marriage) > 1 && 
	str_cmp(victim->pcdata->marriage,ch->name))
    {
	if (IS_EXTRA(victim, EXTRA_MARRIED))
	    send_to_char("But they are married to someone else!\n\r",ch);
	else
	    send_to_char("But they are engaged to someone else!\n\r",ch);
	return;
    }
*/
    ch->pcdata->partner = victim;
    act("You give $N permission to FUCK on you.",ch,NULL,victim,TO_CHAR);
    act("$n has given you permission to FUCK on $m.",ch,NULL,victim,TO_VICT);
    return;
}
