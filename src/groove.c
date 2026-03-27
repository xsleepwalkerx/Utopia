/***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
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
#include "player.h"

void do_autoguard( CHAR_DATA *ch, CHAR_DATA *victim )
{
   ch->blocking = 0;

   if ( ch == NULL || victim == NULL )
      {
         ch->ninjastuff = 0;
         return;
      }

   if ( ch->ninjastuff == 90 )
      perform_arc( ch, victim );
      else if ( ch->ninjastuff == 100 )
              perform_aragami( ch, victim );
      else if ( ch->ninjastuff == 110 )
              perform_zanretsuken( ch, victim );
      else if ( ch->ninjastuff == 120 )
              perform_solkatti( ch, victim );
      else if ( IS_CLASS( ch, CLASS_WISP ) )
              perform_trigger( ch, victim );
      else if ( IS_CLASS( ch, CLASS_MONK ) )
              {
                 if ( ch->ninjastuff == 70 )
                    perform_geyser( ch, victim );
                    else perform_tackle( ch, victim );
              }
}

char * show_fbar( int num, int max_num )
{
   static char buf[13];
   int digit, count, cur_hit, max_hit;

   buf[0] = '\0';

   if ( num == 0 )
      return buf;

   if ( num > 10000000 || max_num > 10000000 )
      {
         cur_hit = num / 10;
         max_hit = max_num / 10;
      }
      else {
              cur_hit = num;
              max_hit = max_num;
           }

   if ( cur_hit * 100 / max_hit < 25 )
      sprintf( buf, "#!" );
      else if ( cur_hit * 100 / max_hit < 50 )
              sprintf( buf, "#%" );
      else if ( cur_hit * 100 / max_hit < 75 )
              sprintf( buf, "#@" );
      else if ( cur_hit * 100 / max_hit < 100 )
              sprintf( buf, "#$" );
              else sprintf( buf, "#&" );
  
   digit = UMAX( 1, UMIN( num * 10 / max_num, 10 ) );

   for ( count = 1; count < digit + 1; count++ )
      strcat( buf, " " );

   strcat( buf, "#n" );

   return buf;
}

char * show_groove( CHAR_DATA *ch )
{
   static char buf[23];
   int count, bonus = 0;

   buf[0] = '\0';

   if ( !str_cmp( "", ch->groove ) )
      strcpy( buf, "#wno #egroove#n" );
      else if ( !str_cmp( "C", ch->groove ) )
              {
                  if ( IS_AFFECTED( ch, AFF_OVERDRIVE ) )
                     bonus = 200;

                  if ( ch->ctimer[9] == 300 + bonus )
                     sprintf( buf, "#w+Lvl #C%d   #o<#wMAX#o>  #w+", ( 300 + bonus ) / 100 );
                     else {
                             int num = ( ch->ctimer[9] % 100 ) / 10;

                             sprintf( buf, "#w+Lvl #C%d #y", ch->ctimer[9] / 100 );

                             for ( count = 0; count < num; count++ )
                                 strcat( buf, ">" );

                             strcat( buf, "#e" );

                             for ( count = 0; count < 9 - num; count++ )
                             {
                                if ( IS_SET( ch->act, PLR_ANSI ) )
                                   strcat( buf, ">" );
                                   else strcat( buf, " " );
                             }

                             strcat( buf, "#w+#n" );
                          }
              }
      else if ( !str_cmp( "M", ch->groove ) )
              {
                  if ( IS_AFFECTED( ch, AFF_OVERDRIVE ) )
                     bonus = 200;

                  if ( ch->ctimer[18] > 0 )
                     {
                        int num = ch->ctimer[18] / 2;

                        sprintf( buf, "#w||" );

                        for ( count = 0; count < 5; count++ )
                        {
                           if ( count > num )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) )
                                    strcat( buf, "-" );
                                    else break;

                                 continue;
                              }
                              else if ( count == num )
                                      strcat( buf, "#e" );

                            strcat( buf, "-" );
                        }

                        strcat( buf, "#y> #CPOWER #y<#e" );

                        for ( count = 5; count > 0; count-- )
                        {
                           if ( count > num )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) )
                                    strcat( buf, "-" );

                                 continue;
                              }
                              else if ( count == num )
                                      strcat( buf, "#w" );

                            strcat( buf, "-" );
                        }

                        strcat( buf, "||#n" );
                     }
                     else {
                             int num = ( ch->ctimer[9] % 100 ) / 10;

                             sprintf( buf, "#w[#o|#C%d#o", ch->ctimer[9] / 100 );

                             for ( count = 0; count < 4; count++ )
                             {
                                if ( count > num )
                                   {
                                      if ( IS_SET( ch->act, PLR_ANSI ) )
                                         strcat( buf, "_" );
                                         else strcat( buf, " " );

                                      continue;
                                   }
                                   else if ( count == num )
                                           strcat( buf, "#e" );

                                strcat( buf, "_" );
                             }

                             for ( count = 4; count < 7; count++ )
                             {
                                if ( count > num )
                                   {
                                      if ( IS_SET( ch->act, PLR_ANSI ) )
                                         strcat( buf, "*" );
                                         else strcat( buf, " " );

                                      continue;
                                   }
                                   else if ( count == num )
                                           strcat( buf, "#e" );

                                strcat( buf, "*" );
                             }

                             for ( count = 7; count < 10; count++ )
                             {
                                if ( count > num )
                                   {
                                      if ( IS_SET( ch->act, PLR_ANSI ) )
                                         strcat( buf, "-" );
                                         else strcat( buf, " " );

                                      continue;
                                   }
                                   else if ( count == num )
                                           strcat( buf, "#e" );

                                strcat( buf, "-" );
                             }

                             strcat( buf, "#w)#g" );

                             num = ch->ctimer[9] / 100;

                             for ( count = 0; count < num; count++ )
                             {
                                strcat( buf, ">" );
                             }

                             strcat( buf, "#n" );
                          }
              }
      else if ( !str_cmp( "S", ch->groove ) ) 
              {
                  if ( ch->ctimer[18] > 0 )
                     {
                        int num = ch->ctimer[18];

                        strcpy( buf, "#w[#R" );

                        for ( count = 0; count < 10; count++ )
                        {
                           if ( count < 3 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "+" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 3 )
                              {
                                 if ( num > 3 )
                                    strcat( buf, "#yP" );
                                    else if ( num == 3 )
                                            strcat( buf, "#eP" );
                                            else strcat( buf, "P" );
                              }

                           if ( count == 4 )
                              {
                                 if ( num == 4 )
                                    strcat( buf, "#eM" );
                                    else strcat( buf, "M" );
                              }

                           if ( count == 5 )
                              {
                                 if ( num == 5 )
                                    strcat( buf, "#eA" );
                                    else strcat( buf, "A" );
                              }

                           if ( count == 6 )
                              {
                                 if ( num > 6 )
                                    strcat( buf, "X#R" );
                                    else if ( num == 6 )
                                            strcat( buf, "#eX" );
                                            else strcat( buf, "X" );
                              }

                           if ( count > 6 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "+" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }
                        }

                        strcat( buf, "#w]#n" );
                     }
                     else {
                             int num = ch->ctimer[9] / 10;

                             strcpy( buf, "#w[#y" );

                             for ( count = 0; count < num; count++ )
                                 strcat( buf, "+" );

                             strcat( buf, "#e" );

                             for ( count = 0; count < 10 - num; count++ )
                             {
                                if ( IS_SET( ch->act, PLR_ANSI ) )
                                   strcat( buf, "+" );
                                   else strcat( buf, " " );
                             }

                             strcat( buf, "#w]#n" );
                          }
              }
      else if ( !str_cmp( "N", ch->groove ) ) 
              {
                  if ( ch->ctimer[18] > 0 )
                     {
                        int num = ch->ctimer[18];

                        strcpy( buf, "#w[POW #C" );

                        for ( count = 0; count < num; count++ )
                            strcat( buf, "-" );

                        strcat( buf, "#e" );

                        for ( count = 0; count < 9 - num; count++ )
                        {
                           if ( IS_SET( ch->act, PLR_ANSI ) )
                              strcat( buf, "-" );
                              else strcat( buf, " " );
                        }

                        strcat( buf, "#w]#n" );
                     }
                     else {
                             int num = ch->ctimer[9] / 100;

                             strcpy( buf, "#w[" );

                             if ( num > 3 )
                                strcat( buf, "#C" );

                             for ( count = 0; count < 3; count++ )
                                 {
                                    if ( count < num )
                                       strcat( buf, "*" );
                                       else strcat( buf, " " );

                                    if ( count == 0 && num == 4 || count == 1 && num == 5 )
                                       strcat( buf, "#w" );
                                 }

                             strcat( buf, "#y" );
                             num = ( ch->ctimer[9] % 100 ) / 10;

                             for ( count = 0; count < num; count++ )
                                 strcat( buf, "|" );

                             strcat( buf, "#e" );

                             for ( count = 0; count < 9 - num; count++ )
                             {
                                if ( IS_SET( ch->act, PLR_ANSI ) )
                                   strcat( buf, "|" );
                                   else strcat( buf, " " );
                             }

                             strcat( buf, "#w]#n" );
                          }
              }
      else if ( !str_cmp( "P", ch->groove ) ) 
              {
                  if ( ch->ctimer[18] > 0 )
                     {
                        int num = ch->ctimer[18];

                        strcpy( buf, "#w||-#s" );

                        for ( count = 0; count < 10; count++ )
                        {
                           if ( count == 0 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "S" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 2 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "U" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 4 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "P" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 6 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "E" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 8 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "R" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 1 || count == 3 || count == 5 || count == 7 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "=" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }
                        }

                        strcat( buf, "#w->#n" );
                     }
                     else {
                             int num = ch->ctimer[9] / 10;

                             strcpy( buf, "#w<-#C" );

                             for ( count = 0; count < num; count++ )
                                 strcat( buf, ")" );

                             strcat( buf, "#e" );

                             for ( count = 0; count < 10 - num; count++ )
                             {
                                if ( IS_SET( ch->act, PLR_ANSI ) )
                                   strcat( buf, ")" );
                                   else strcat( buf, " " );
                             }

                             strcat( buf, "#w->#n" );
                          }
              }
      else if ( !str_cmp( "A", ch->groove ) )
              {
                  if ( ch->ctimer[15] > 0 )
                     {
                        int num = ch->ctimer[15] / 10;

                        strcpy( buf, "#w[T#C" );

                        for ( count = 0; count < num; count++ )
                            strcat( buf, "<" );

                        strcat( buf, "#e" );

                        for ( count = 0; count < 10 - num; count++ )
                        {
                           if ( IS_SET( ch->act, PLR_ANSI ) )
                              strcat( buf, "<" );
                              else strcat( buf, " " );
                        }

                        strcat( buf, "#wS]#n" );
                     }
                     else {
                             int num = ch->ctimer[9] / 10;

                             strcpy( buf, "#w[+#y" );

                             for ( count = 0; count < num; count++ )
                                 strcat( buf, ">" );

                             strcat( buf, "#e" );

                             for ( count = 0; count < 10 - num; count++ )
                             {
                                if ( IS_SET( ch->act, PLR_ANSI ) )
                                   strcat( buf, ">" );
                                   else strcat( buf, " " );
                             }

                             strcat( buf, "#w+]#n" );
                          }
              }
      else if ( !str_cmp( "K", ch->groove ) )
              {
                  if ( ch->ctimer[18] > 0 )
                     {
                        int num = ch->ctimer[18];

                        strcpy( buf, "#w<<#R" );

                        for ( count = 0; count < 10; count++ )
                        {
                           if ( count < 3 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "~" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }

                           if ( count == 3 )
                              {
                                 if ( num > 3 )
                                    strcat( buf, "#y#uR" );
                                    else if ( num == 3 )
                                            strcat( buf, "#e#uR" );
                                            else strcat( buf, "#uR" );
                              }

                           if ( count == 4 )
                              {
                                 if ( num == 4 )
                                    strcat( buf, "#eA" );
                                    else strcat( buf, "A" );
                              }

                           if ( count == 5 )
                              {
                                 if ( num == 5 )
                                    strcat( buf, "#eG" );
                                    else strcat( buf, "G" );
                              }

                           if ( count == 6 )
                              {
                                 if ( num > 6 )
                                    strcat( buf, "E#n#R" );
                                    else if ( num == 6 )
                                            strcat( buf, "#eE#n#e" );
                                            else strcat( buf, "E#n#e" );
                              }

                           if ( count > 6 )
                              {
                                 if ( IS_SET( ch->act, PLR_ANSI ) || count < num )
                                    strcat( buf, "~" );
                                    else strcat( buf, " " );

                                 if ( count == num - 1 )
                                    strcat( buf, "#e" );
                              }
                        }

                        strcat( buf, "#w>>#n" );
                     }
                     else {
                             int num = ch->ctimer[9] / 10;

                             strcpy( buf, "#w<<#y" );

                             for ( count = 0; count < num; count++ )
                                 strcat( buf, "~" );

                             strcat( buf, "#e" );

                             for ( count = 0; count < 10 - num; count++ )
                             {
                                if ( IS_SET( ch->act, PLR_ANSI ) )
                                   strcat( buf, "~" );
                                   else strcat( buf, " " );
                             }

                             strcat( buf, "#w>>#n" );
                          }
              }
              else strcpy( buf, "#wno #egroove#n" ); 

   return buf;
}

int get_stock( CHAR_DATA *ch )
{
   if ( !str_cmp( "C", ch->groove ) || !str_cmp( "M", ch->groove ) )
      return ch->ctimer[9];
      else if ( !str_cmp( "N", ch->groove ) )
              {
                 if ( ch->ctimer[18] > 0 )
                    {
                       if ( ch->ctimer[9] > 99 )
                          return 100 + UMIN( 200, ch->pkill * 10 );
                          else return 100;
                    }
                    return ch->ctimer[9];
              }      
      else if ( !str_cmp( "P", ch->groove ) )
              {
                 if ( IS_SET( ch->arena, AFF2_SC ) )
                    return 100;
                    else return ch->ctimer[9];
              }
      else if ( !str_cmp( "A", ch->groove ) )
              {
                 if ( ch->ctimer[15] > 0 )
                    return 100 + UMIN( 200, ch->pkill * 10 );
                    else if ( ch->ctimer[9] > 49 || IS_SET( ch->arena, AFF2_SC ) )
                            return 100;
                            else return 0;
              }
      else if ( !str_cmp( "K", ch->groove ) )
              {
                 if ( ch->ctimer[9] > 99 || IS_SET( ch->arena, AFF2_SC ) )
                    return 100 + UMIN( 200, ch->pkill * 10 );
                    else return 0;
              }
      else if ( !str_cmp( "S", ch->groove ) )
              {
                 if ( ch->ctimer[9] > 99 )
                    {
                       if ( ch->hit < ch->max_hit * 0.25 || IS_SET( ch->arena, AFF2_SC ) )
                          return 100 + UMIN( 200, ch->pkill * 10 );
                          else return 100;
                    }
                    else if ( ch->hit < ch->max_hit * 0.25 || IS_SET( ch->arena, AFF2_SC ) )
                            return 100;
                            else return 0;
              }
}

void subtract_stock( CHAR_DATA *ch, int stock )
{
   if ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) )
      {
         if ( !str_cmp( "S", ch->groove ) && ch->ctimer[9] < 100 )
            ch->wait += 6;

         if ( is_affected( ch, skill_lookup( "subsitance" ) ) )
            ch->ctimer[9] = UMAX( 0, ch->ctimer[9] - 50 );
            else ch->ctimer[9] = UMAX( 0, ch->ctimer[9] - 100 );

         ch->ctimer[18] = 0;
         return;
      }
      else if ( !str_cmp( "A", ch->groove ) && ch->ctimer[15] > 0 )
              return;

   if ( stock > 100 && is_affected( ch, skill_lookup( "subsitance" ) ) )
      stock -= 100;

   if ( ch->ctimer[18] > 0 && !str_cmp( "N", ch->groove ) )
      stock = 100;

   ch->ctimer[9] = UMAX( 0, ch->ctimer[9] - stock );
}

void add_grove( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
   CHAR_DATA *user = ch;

   if ( IS_NPC( ch ) && IS_SET( ch->extra, EXTRA_ZOMBIE ) && ch->master != NULL ) 
      user = ch->master;

   if ( dam == 0 || user->ctimer[15] > 0 || IS_SET( user->arena, AFF2_DM ) || IS_SET( user->arena, AFF2_SC ) || IS_SET( user->arena, AFF2_POWER ) )
      return;

   if ( user->ctimer[18] > 0 && ( !IS_AFFECTED( user, AFF_OVERDRIVE ) || ( user->ctimer[9] >= 200 
        &&  str_cmp( "N", user->groove ) ) ) )
      return;

   if ( dam < 0 )
      {
         int bonus = IS_AFFECTED( user, AFF_OVERDRIVE ) ? 2 : 1;
         int max = IS_AFFECTED( user, AFF_OVERDRIVE ) ? 200 : 0;

         if ( dam == -150 )
            {
               if ( !str_cmp( "K", user->groove ) || !str_cmp( "S", user->groove ) || !str_cmp( "A", user->groove )
                    || !str_cmp( "P", user->groove ) )
                  user->ctimer[9] = UMIN( 100, user->ctimer[9] + 50 * bonus );
                  else if ( user->pkill > 19 )
                          user->ctimer[9] = UMIN( 300 + max, user->ctimer[9] + 150 * bonus );
                  else if ( user->pkill > 9 )
                          user->ctimer[9] = UMIN( 200 + max, user->ctimer[9] + 100 * bonus );
                          else user->ctimer[9] = UMIN( 100 + max, user->ctimer[9] + 50 * bonus );
            }
            else if ( dam == -33 )
            {
               if ( !str_cmp( "K", user->groove ) || !str_cmp( "S", user->groove ) || !str_cmp( "A", user->groove )
                    || !str_cmp( "P", user->groove ) )
                  user->ctimer[9] = UMIN( 100, user->ctimer[9] + 34 * bonus );
                  else if ( user->pkill > 19 )
                          user->ctimer[9] = UMIN( 300 + max, user->ctimer[9] + 100 * bonus );
                  else if ( user->pkill > 9 )
                          user->ctimer[9] = UMIN( 200 + max, user->ctimer[9] + 68 * bonus );
                          else user->ctimer[9] = UMIN( 100 + max, user->ctimer[9] + 34 * bonus );
            }
            else if ( dam == -66 )
            {
               if ( !str_cmp( "K", user->groove ) || !str_cmp( "S", user->groove ) || !str_cmp( "A", user->groove )
                    || !str_cmp( "P", user->groove ) )
                  user->ctimer[9] = UMAX( 0, user->ctimer[9] - 25 );
                  else if ( user->pkill > 19 )
                          user->ctimer[9] = UMAX( 0, user->ctimer[9] - 75 );
                  else if ( user->pkill > 9 )
                          user->ctimer[9] = UMAX( 0, user->ctimer[9] - 50 );
                          else user->ctimer[9] = UMAX( 0, user->ctimer[9] - 25 );
            }
            else if ( dam == -40 && !str_cmp( "S", user->groove ) )
                    user->ctimer[9] = UMIN( 100, user->ctimer[9] + 40 );
            else if ( dam == -25 )
            {
               if ( !str_cmp( "K", user->groove ) || !str_cmp( "S", user->groove ) || !str_cmp( "A", user->groove )
                    || !str_cmp( "P", user->groove ) )
                  user->ctimer[9] = UMIN( 100, user->ctimer[9] + 25 * bonus );
                  else if ( user->pkill > 19 )
                          user->ctimer[9] = UMIN( 300 + max, user->ctimer[9] + 75 * bonus );
                  else if ( user->pkill > 9 )
                          user->ctimer[9] = UMIN( 200 + max, user->ctimer[9] + 50 * bonus );
                          else user->ctimer[9] = UMIN( 100 + max, user->ctimer[9] + 25 * bonus );
            }

         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_NOSTOCK ) || victim == NULL )
      return;

   if ( !str_cmp( "K", user->groove ) )
      {
         int dam_bond = user->max_hit / 100;
         int adder = 0;
         float multi = 1.7;
         int max = 100;

         if ( user->ctimer[18] > 0 )
            return;

         if ( dam >= dam_bond )
            {
               adder = dam / dam_bond;

               if ( IS_CLASS( user, CLASS_ANGEL ) && user->ctimer[8] > 0 )
                  multi += 0.5;

               if ( IS_AFFECTED( user, AFF_OVERDRIVE ) )
                  multi += 0.5;
                  else if ( can_do_skill( user, 23 ) )
                          multi += 0.5;

               if ( is_affected( user, skill_lookup( "hyper drive" ) ) )
                  multi += 0.5;

               if ( IS_SET( user->more2, MORE2_SPINCH ) )
                  multi *= 0.67;

               adder *= multi;

               user->ctimer[9] = UMIN( max, user->ctimer[9] + adder );

               if ( user->ctimer[9] >= 100 && user->ctimer[18] == 0 )
                  {
                     act( "#RYou #yscream #wloudly #yas your body #CBOILS #ywith #utremedous#n #RPOWER#y!!!#n", user, NULL, NULL, TO_CHAR );
                     act( "#R$n #yscreams #wloudly #yas $s body #CBOILS #ywith #utremedous#n #RPOWER#y!!!#n", user, NULL, NULL, TO_ROOM );
                     user->ctimer[18] = 10;
                  }
            }
      }

   if ( !str_cmp( "C", user->groove ) || !str_cmp( "N", user->groove ) || !str_cmp( "P", user->groove )
        || !str_cmp( "M", user->groove ) )
      {
         int adder = 0;
         int dam_bond = ( dam < 450000 ) ? 100000: victim->max_hit / 100;
         int max;
         float multi = 1.5;

         if ( !str_cmp( "P", user->groove ) )            
            max = 100 + UMIN( 50, user->pkill * 2.5 );
            else max = 100 + UMIN( 200, user->pkill * 10 );

         if ( dam >= dam_bond )
            {
               adder = dam / dam_bond;

               if ( !str_cmp( "C", user->groove ) )
                  multi = 1.75;
                  else multi = 1.5;

               if ( IS_CLASS( user, CLASS_ANGEL ) && user->ctimer[8] > 0 )
                  multi += 0.5;

               if ( IS_AFFECTED( user, AFF_OVERDRIVE ) )
                  {
                     if ( str_cmp( "P", user->groove ) )
                        max += 200;

                     multi += 0.5;
                  }
                  else if ( can_do_skill( user, 23 ) )
                          multi += 0.5;

               if ( is_affected( user, skill_lookup( "hyper drive" ) ) )
                  multi += 0.5;

               if ( IS_SET( user->more2, MORE2_SPINCH ) )
                  multi *= 0.67;

               adder *= multi * ( max * 0.01 );

               if ( !str_cmp( "P", user->groove ) )
                  {
                     user->ctimer[9] = UMIN( max, user->ctimer[9] + adder );

                     if ( user->ctimer[9] >= 100 && user->ctimer[18] == 0 )
                        {
                           act( "#yYou #wsuddenly #Cclench #wyour fist as you #Ractivate #wyour groove mode!#n", user, NULL, NULL, TO_CHAR );
                           act( "#y$n #wsuddenly #Cclenches #w$s fist as $e #Ractivates #w$s groove mode!#n", user, NULL, NULL, TO_ROOM );
                           user->ctimer[18] = 10;
                        }
                  }
                  else user->ctimer[9] = UMIN( max, user->ctimer[9] + adder );
            }
      }

   if ( !str_cmp( "A", user->groove ) || !str_cmp( "S", user->groove ) )
      {
         int adder = 0;
         int dam_bond = ( dam < 450000 ) ? 100000: victim->max_hit / 100;
         float multi = 1.5;
         int max = 100;

         if ( dam >= dam_bond )
            {
               adder = dam / dam_bond;

               if ( IS_CLASS( user, CLASS_ANGEL ) && user->ctimer[8] > 0 )
                  multi += 0.5;

               if ( is_affected( user, skill_lookup( "hyper drive" ) ) )
                  multi += 0.5;

               if ( IS_AFFECTED( user, AFF_OVERDRIVE ) )
                  multi += 0.5;
                  else if ( can_do_skill( user, 23 ) )
                          multi += 0.5;

               if ( IS_SET( user->more2, MORE2_SPINCH ) )
                  multi *= 0.67;

               adder *= multi;

               user->ctimer[9] = UMIN( max, user->ctimer[9] + adder );

               if ( user->ctimer[9] >= 100 && !str_cmp( "S", user->groove ) && user->ctimer[18] == 0 )
                  {
                     act( "#yYou #wsuddenly #Cclench #wyour fist as you #Ractivate #wyour groove mode!#n", user, NULL, NULL, TO_CHAR );
                     act( "#y$n #wsuddenly #Cclenches #w$s fist as $e #Ractivates #w$s groove mode!#n", user, NULL, NULL, TO_ROOM );
                     user->ctimer[18] = 10;
                  }
            }
      }
}

void do_emergency_roll( CHAR_DATA *ch, char *argument )
{
   if ( ch->position != POS_STUNNED && ch->position != POS_SLEEPING && ch->stunned == 0 )
      {
         stc( "#wYou currently aren't #cstunned #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25 && ch->ctimer[9] != 100 )
      {
         send_to_char("#wDue to being in awful condition, you need #yfull #wstock to do this technique.#n\n\r",ch);
	   return;
      }

   if ( ch->ctimer[15] > 0 )
      {
         send_to_char("#wThe church has banned the use of #CTimeStop #wfor this technique.#n\n\r",ch);
	   return;
      }

   if ( !IS_NPC( ch ) )
      {
         if ( get_stock( ch ) < 100 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
              || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > 32 ) )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
              || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 100 )
            ch->ctimer[9] -= 33;
            else ch->ctimer[9] -= 100;
      }

   ch->stunned = 0;
   
   if ( ch->position == POS_STUNNED || ch->position == POS_SLEEPING )
      ch->position = POS_STANDING;

   act( "#wA #Cspiral #wof #Cbluish light #wradiates off #gyou #was you suddenly roll back onto your feet!#n", ch, NULL, NULL, TO_CHAR );
   act( "#wA #Cspiral #wof #Cbluish light #wradiates off #g$n #was $e suddenly rolls back onto $s feet!#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou then #gtwirl #wyourself around #Pgracefully #was you shift into a deadly pose...#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n then #gtwirls #w$mself around #Pgracefully #was $e shifts into a deadly pose...#n", ch, NULL, NULL, TO_ROOM );
}

void do_emergency_flip( CHAR_DATA *ch, char *argument )
{
   if ( ch->position != POS_MORTAL && ch->air_timer == 0 )
      {
         stc( "#wYou currently don't have a need for this #ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25 && ch->ctimer[9] != 100 )
      {
         send_to_char("#wDue to being in awful condition, you need #yfull #wstock to do this technique.#n\n\r",ch);
	   return;
      }

   if ( ch->ctimer[15] > 0 )
      {
         send_to_char("#wThe church has banned the use of #CTimeStop #wfor this technique.#n\n\r",ch);
	   return;
      }

   if ( ch->air_timer > 0 )
      {
         if ( ch->position < POS_FIGHTING )
            {
               stc( "#wYou are unable to do this #ctechnique #wdue to your condition warrior.#n\n\r", ch );
               return;
            }

         if ( !IS_NPC( ch ) )
            {
               if ( get_stock( ch ) < 100 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
                    || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > 32 ) )
                  {
                     stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                     return;
                  }

               if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
                    || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 100 )
                  ch->ctimer[9] -= 33;
                  else ch->ctimer[9] -= 100;
            }

         ch->air_timer = 0;
         act( "#wA #gspiral #wof #ggreenish light #wradiates off #Cyou #was you quickly land back onto your knees!#n", ch, NULL, NULL, TO_CHAR );
         act( "#wA #gspiral #wof #ggreenish light #wradiates off #C$n #was $e quickly lands back onto $s knees!#n", ch, NULL, NULL, TO_ROOM );
         act( "#wYou #Ccoldly #wleap back onto your #ofeet #was you swerve your hands for #Raction#w.#n", ch, NULL, NULL, TO_CHAR );
         act( "#w$n #Ccoldly #wleaps back onto $s #ofeet #was $e swerves $s hands for #Raction#w.#n", ch, NULL, NULL, TO_ROOM );
      }
      else {
              if ( !IS_NPC( ch ) )
                 {
                    if ( get_stock( ch ) < 200 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
                         || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > 65 ) )
                       {
                          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                          return;
                       }

                    if ( str_cmp( "N", ch->groove ) && str_cmp( "P", ch->groove ) && ch->ctimer[18] > 0 )
                       {
                          ch->ctimer[9] = 0;
                          ch->ctimer[18] = 0;
                       }
                       else if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
                                  || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 200 )
                               ch->ctimer[9] -= 66;
                               else ch->ctimer[9] -= 200;
                 }

              ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * 0.1 ) );
              ch->position = POS_STANDING;
              act( "#wYou #Ccoldly #wleap back onto your #ofeet #was you swerve your hands for #Raction#w.#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n #Ccoldly #wleaps back onto $s #ofeet #was $e swerves $s hands for #Raction#w.#n", ch, NULL, NULL, TO_ROOM );
              act( "#wYou mutter '#yToo slow...#n' as the dazzling lights rejuvenate you!#n", ch, NULL, NULL, TO_CHAR );
              act( "#w$n mutters '#yToo slow...#n' as the dazzling lights rejuvenate $m!#n", ch, NULL, NULL, TO_ROOM );
           }
}

void apply_cpush( CHAR_DATA *ch, CHAR_DATA *victim )
{
    bool pre_room;
    int direction;
    int count;

    act( "#wYour #oarm #y#zshimmers#n #wwith #Rferocious #wenergy as you #Pabsorb #g$N#g's #wswift attack!#n", ch, NULL, victim, TO_CHAR );
    act( "#w$n#w's #oarm #y#zshimmers#n #wwith #Rferocious #wenergy as $e #Pabsorbs #gyour #wswift attack!#n", ch, NULL, victim, TO_VICT );
    act( "#w$n#w's #oarm #y#zshimmers#n #wwith #Rferocious #wenergy as $e #Pabsorbs #g$N#g's #wswift attack!#n", ch, NULL, victim, TO_NOTVICT );
    act( "#CTime #eslows down as #Preplicas #eof you draw back your #RUnStAbLe #earms...#n", ch, NULL, NULL, TO_CHAR );
    act( "#CTime #eslows down as #Preplicas #eof $e draws back $s #RUnStAbLe #earms...#n", ch, NULL, NULL, TO_ROOM );
    act( "#wYou scream '#yHAI!#w' as you ViOlEnTlY push $N #waway from you!!!#n", ch, NULL, victim, TO_CHAR );
    act( "#w$n screams '#yHAI!#w' as $e ViOlEnTlY pushes you #waway from $m!!!#n", ch, NULL, victim, TO_NOTVICT );
    act( "#w$n screams '#yHAI!#w' as $e ViOlEnTlY pushes $N #waway from $m!!!#n", ch, NULL, victim, TO_VICT );

    count = 0;
    direction = number_range( 0, 3 );

    do
    {
       pre_room = special_slam( victim, direction );
       count++;
    }
    while ( count < 5 && pre_room != TRUE );

    if ( victim != NULL && victim->in_room != NULL && victim->position > POS_STUNNED )
       {
          stop_fighting( victim, TRUE );
          victim->position = POS_STUNNED;
       }

    ch->anger_level = UMIN( 300, ch->anger_level + 60 );

    if ( ch->anger_level == 300 )
       {
          act( "#PYour #warms #Cburst #winto #Rflames #was you swing your weapon #g#zviolently#n#w!#n", ch, NULL, NULL, TO_CHAR );
          act( "#P$n's #warms #Cburst #winto #Rflames #was $e swings $s weapon #g#zviolently#n#w!#n", ch, NULL, NULL, TO_ROOM );
          ch->anger_level = 306;
       }
}

void do_super_cancel( CHAR_DATA *ch, char *argument )
{
   int cost = 33;

   if ( IS_NPC(ch) )
      return;

   if ( argument[0] == '\0' )
      {
         send_to_char( "Which command do you wish to supercancel?\n\r", ch );
         return;
      }

   if ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25 && ch->ctimer[9] != 100 )
      {
         send_to_char("#wDue to being in awful condition, you need #yfull #wstock to do this technique.#n\n\r",ch);
	   return;
      }

   if ( ch->ctimer[15] > 0 )
      {
         send_to_char("#wThe church has banned the use of #CTimeStop #wfor this technique.#n\n\r",ch);
	   return;
      }

   if ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
        || !str_cmp( "P", ch->groove ) )
      {
          if ( ch->pkill < 5 )
             cost = 100;
             else if ( ch->pkill < 10 )
                     cost = 66;
      }

   if ( get_stock( ch ) < 100 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
        || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > cost - 1 ) )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->ctimer[18] > 0 && !str_cmp( "N", ch->groove ) )
      ch->ctimer[18] = 0;
      else if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
                || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 100 )
              ch->ctimer[9] -= cost;
              else ch->ctimer[9] -= 100;

   SET_BIT( ch->arena, AFF2_SC );

   act( "#gYou #wclench your #ofists #was #edeadly #Crays #wof #ylight #R#zAnNiHiLaTe#n #wthe ViCiNiTy!#n", ch, NULL, NULL, TO_CHAR );
   act( "#g$n #wclenches $s #ofists #was #edeadly #Crays #wof #ylight #R#zAnNiHiLaTe#n #wthe ViCiNiTy!#n", ch, NULL, NULL, TO_ROOM );
   act( "#gYou #wquickly dash forward as the #yLiGhT #P#zRIPS#n #wthe #oground #wbeneath your feet!!!#n", ch, NULL, NULL, TO_CHAR );
   act( "#g$n #wquickly dashes forward as the #yLiGhT #P#zRIPS#n #wthe #oground #wbeneath your feet!!!#n", ch, NULL, NULL, TO_ROOM );
   act( "#gYou #wthen #yharness #wthe #yenergy #waround #Cyou #was you release your #RFuRy#w!#n", ch, NULL, NULL, TO_CHAR );
   act( "#g$n #wthen #yharnesses #wthe #yenergy #waround #C$m #was $e releases $s #RFuRy#w!#n", ch, NULL, NULL, TO_ROOM );

   interpret( ch, argument );
   ch->wait = 0;
   ch->blocking = 0;
   REMOVE_BIT( ch->arena, AFF2_DD );
   REMOVE_BIT( ch->arena, AFF2_SC );
}

void do_counter_push( CHAR_DATA *ch, char *argument )
{
   int cost = 33;

   if ( IS_NPC(ch) )
      return;

   if ( ch->blocking < 0 )
      {
         stc( "#wYou can #yblock #wagain in a moment warrior.#n\n\r", ch );
         return;
      }

   if ( !str_cmp( "S", ch->groove ) )
      cost = 66;

   if ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25 && ch->ctimer[9] != 100 )
      {
         send_to_char("#wDue to being in awful condition, you need #yfull #wstock to do this technique.#n\n\r",ch);
	   return;
      }

   if ( ch->ctimer[15] > 0 )
      {
         send_to_char("#wThe church has banned the use of #CTimeStop #wfor this technique.#n\n\r",ch);
	   return;
      }

   if ( get_stock( ch ) < 100 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
        || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > cost - 1 ) )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "A", ch->groove ) || !str_cmp( "S", ch->groove )
        || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 100 )
      ch->ctimer[9] -= cost;
      else ch->ctimer[9] -= 100;

   ch->blocking = 12;
   ch->counta = TRUE;
   act( "#yYou beckon #R#zall#n #yto attack you as you #wshift #oyour body #pdefensively.#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n beckons #R#zall#n #yto attack $m as $e #wshifts #o$s body #pdefensively.#n", ch, NULL, NULL, TO_ROOM );
}

void do_charging( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) ) 
      return;

   if ( str_cmp( "S", ch->groove ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->ctimer[9] >= 100 )
      {
         stc("#yYou #ware already #ycharged #wup.#n\n\r",ch);
         return;
      }

   if ( ch->saiyanlevel > 0 )
      {
         ch->saiyanlevel = 0;
         stc("#yYou #wrelease the #cswiral #wof #Pblazing #yenergy #was you stop charging!#n\n\r",ch);
         act("#y$n #wreleases the #cswiral #wof #Pblazing #yenergy #was $e stop charging!#n",ch,NULL,NULL,TO_ROOM);
         return;
      }

   ch->saiyanlevel = 1;
   stc("#yYou #ware surrounded by a #cswiral #wof #Pblazing #yenergy #was you charge up!#n\n\r",ch);
   act("#y$n #wis surrounded by a #cswiral #wof #Pblazing #yenergy #was $e charges up!#n",ch,NULL,NULL,TO_ROOM);
}

void do_run( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( str_cmp( "K", ch->groove ) && str_cmp( "N", ch->groove ) && str_cmp( "S", ch->groove )
        && str_cmp( "M", ch->groove ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 5000000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
      {
         send_to_char("It is #c#zimpossible#n to leave from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
         return;
      } 

   if ( arg[0] == '\0' )
      {
         send_to_char("Who do you wish to run to?\r\n", ch);
         return;
      }
	
   if ( ( victim = get_char_area( ch, arg ) ) == NULL || victim == ch || victim->in_room == NULL
        || ch->in_room == NULL )
      {
         send_to_char( "You attempt to find your target, but fail.\n\r", ch );
         return;
      }

   if ( victim->in_room == ch->in_room )
      {
         send_to_char( "But you are already there!\n\r", ch );
         return;
      }

   if ( in_kingdom( ch, victim, victim->in_room ) )
      return;

   ch->move -= 5000000;

   if ( !is_martial( victim ) && !IS_CLASS( victim, CLASS_WISP ) )
      victim->wait = 12;

   stop_fighting( ch, TRUE );

   act("#yYou #wgrit your teeth as you charge toward your #gtarget!#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n #wgrits $s teeth as $e runs into the #gdistance#w!#n",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   do_look(ch,"");

   act("#yYou #wthen #Rslam #wyourself into #g$N#g's #wbody.#n",ch,NULL,victim,TO_CHAR);
   act("#y$n #wsuddenly #cruns #winto the room #Rslamming #w$mself into #gyour #wbody.#n",ch,NULL,victim,TO_VICT);
   act("#y$n #wsuddenly #cruns #winto the room #Rslamming #w$mself into #g$N#g's #wbody.#n",ch,NULL,victim,TO_NOTVICT);

   check_traps( ch );

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );
}

void do_dash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   sh_int vnum;
   int dam = 500000;
   int move = 1000000;

   if ( IS_NPC( ch ) )
      return;

   if ( str_cmp( "C", ch->groove ) && str_cmp( "A", ch->groove ) && str_cmp( "M", ch->groove ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ch->move < move )
      { 
         stc("You are too tired to use this skill.\n\r",ch);
         return;
      }

   vnum = ch->in_room->vnum; 

   if (!str_cmp(arg,"n") || !str_cmp(arg,"north")) do_north(ch,"");
      else if (!str_cmp(arg,"s") || !str_cmp(arg,"south")) do_south(ch,"");
      else if (!str_cmp(arg,"e") || !str_cmp(arg,"east" )) do_east(ch,"");
      else if (!str_cmp(arg,"w") || !str_cmp(arg,"west" )) do_west(ch,"");
      else {
              send_to_char("#wDo #yyou #wwish to #Cdash #wnorth, south, east, or west?#n\n\r",ch);
              return;
           }

   if ( vnum == ch->in_room->vnum )
      {
         stc("You can't charge into that direction.\n\r",ch);
         return;
      }

   ch->move -= move; 
   ch->wait = 12;

   act("#yYou #wquickly #Cdash #winto the next room in a #Rfury#w!#n",ch,NULL,NULL,TO_CHAR); 

   if ( IS_SET( ch->more, MORE_FROZEN ) )
      return;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( !is_safe(ch, vch) )
         {
            if ( !str_cmp( "P", vch->groove ) || !str_cmp( "M", vch->groove ) )
               {
                  act( "#CAs #yyou #Ccharge towards #g$N#C, $E violently pushes you aside.#n", ch, NULL, vch, TO_CHAR );
                  act( "#CAs #y$n #Ccharges towards #gyou#C, you violently push $m aside.#n", ch, NULL, vch, TO_VICT );
                  act( "#CAs #y$n #Ccharges towards #g$N#C, $E violently pushes $m aside.#n", ch, NULL, vch, TO_NOTVICT );
                  continue;
               }

            act("#yYou #Cshoulder dash #winto #g$N #wknocking $M onto the ground.#n",ch,NULL,vch,TO_CHAR);
            act("#y$n #Cshoulder dash #winto #gyou #wknocking you right off your feet.#n",ch,NULL,vch,TO_VICT);
            act("#y$n #Cshoulder dash #winto #g$N #wknocking $M onto the ground.#n",ch,NULL,vch,TO_NOTVICT);

            ch->attack_type = ATTACK_PHYSICAL;
            hurt_person( ch, vch, dam, TRUE );

            if ( vch == NULL || vch->position <= POS_STUNNED )
               continue;

            if ( is_martial( vch ) )
               continue;

            vch->wait = number_range( 12, 18 );
         }
    }
}

void do_block( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( ch->blocking < 0 )
      {
         stc( "#wYou can #yblock #wagain in a moment warrior.#n\n\r", ch );
         return;
      }

   ch->blocking = 12;
   act( "#yYou #Cshift #gyour body into a #pdefensive #gposure.#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #Cshifts #g$s body into a #pdefensive #gposure.#n", ch, NULL, NULL, TO_ROOM );
}

void do_time_stop( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( str_cmp( "A", ch->groove ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->ctimer[9] < 100 )
      {
         stc("#wYou don't have enough #ystock #wto use this technique #ywarrior#w.#n\n\r",ch);
         return;
      }

   ch->ctimer[15] = 100;
   ch->ctimer[9] = 100;

   act("#PTime #wstands still as everything is drawn into a #ccomputerized #gm#wa#rt#wr#gi#wx.#n",ch,NULL,NULL,TO_CHAR);
   act("#PTime #wstands still as everything is drawn into a #ccomputerized #gm#wa#rt#wr#gi#wx.#n",ch,NULL,NULL,TO_ROOM);
   act("#wEverything suddenly #eslows down #wto a #Rdead stop #was infinite numbers flashes by #yyou#w.#n",ch,NULL,NULL,TO_CHAR);
   act("#wEverything suddenly #eslows down #wto a #Rdead stop #was infinite numbers flashes by #y$n#w.#n",ch,NULL,NULL,TO_ROOM);
   act("#yYour #whands starts trembling with #Cunbelievable power #was a whisper floats by you.#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n's #whands starts trembling with #Cunbelievable power #was a whisper floats by $m.#n",ch,NULL,NULL,TO_ROOM);
   act("#o'#gY#wo#gu #wa#gr#we #gt#wh#ge #wO#gn#we#g.#w.#g.#o'#n",ch,NULL,NULL,TO_CHAR);
   act("#o'#g$e #wi#gs #wt#gh#we #gO#wn#ge#w.#g.#w.#o'#n",ch,NULL,NULL,TO_ROOM);
}

void do_activate( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( ch->topmode > 0 )
      {
         if ( !str_cmp( "N", ch->groove ) || !str_cmp( "M", ch->groove ) )
            {
               if ( ch->ctimer[18] > 0 )
                  {
                     stc( "#wYou already have your #ygroove #Pactivated #wwarrior.#n\n\r", ch );
                     return;
                  }

               if ( get_stock( ch ) < 100 )
                  {
                     stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                     return;
                  }

               act( "#yYou #wsuddenly #Cclench #wyour fist as you #Ractivate #wyour groove mode!#n", ch, NULL, NULL, TO_CHAR );
               act( "#y$n #wsuddenly #Cclenches #w$s fist as $e #Ractivates #w$s groove mode!#n", ch, NULL, NULL, TO_ROOM );
               ch->ctimer[18] = 10;
               ch->ctimer[9] = UMAX( 0, ch->ctimer[9] - 100 );
               return;
            }

         send_to_char("You have already chosen your T.O.P. mode warrior.\n\r",ch);
         return;
      }

   argument = one_argument(argument, arg);

   if ( !str_cmp( "critical", arg ) )
      ch->topmode = 33;
      else if ( !str_cmp( "focused", arg ) )
              ch->topmode = 66;
      else if ( !str_cmp( "healthy", arg ) )
              ch->topmode = 100;
              else {
                      stc( "Which level do you want to set it at? #RCritical, #CFocused, #wHealthy#n\n\r", ch );
                      return;
                   }

   ch->max_hit += 2000000;
   stc( "#gYour #RT#e.#CO#e.#wP#e. #gmode is set warrior... Well done.#n\n\r", ch );
}
