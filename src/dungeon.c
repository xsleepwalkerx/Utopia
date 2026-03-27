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
#include "player.h"

void clear_dungeon( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;

    is_dungeon = -1;
    dungeon_timer = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
       ch_next = ch->next;

       if ( ch->in_room == NULL )
          continue;

       if ( ch->in_room == NULL || ch->in_room->vnum < 33500
            || ch->in_room->vnum > 33800 )
          continue;

       if ( IS_NPC( ch ) )
          extract_char( ch, TRUE );
          else {
                  char_from_room( ch );
                  char_to_room( ch, get_room_index( 800 ) );
               }
    }
}

void load_boss( int level )
{
   ROOM_INDEX_DATA *pRoomIndex;
   int vnum, m1num = -1, m2num = -1, m3num = -1, m4num = -1, counter;
   CHAR_DATA *mob, *mob_next;

   switch( level )
   {
      default: return;
      case 1: vnum = 33519; m1num = 560; m2num = 559; break;
      case 2: vnum = 33539; m1num = 562; m2num = 554; break;
      case 3: vnum = 33559; m1num = 557; m2num = 552; m3num = 550; break;
      case 4: vnum = 33579; m1num = 568; m2num = 568; m3num = 556; break;
      case 5: vnum = 33599; m1num = 553; break;
      case 6: vnum = 33619; m1num = 559; m2num = 555; m3num = 563; break;
      case 7: vnum = 33639; m1num = 569; break;
      case 8: vnum = 33659; m1num = 551; m2num = 567; m3num = 561; m4num = 558; break;
      case 9: vnum = 33679; m1num = 570; break;
      case 10: vnum = 33699; m1num = 566; m2num = 565; m3num = 564; break;
   }

   if ( ( pRoomIndex = get_room_index( vnum ) ) == NULL )
      return;

   for ( mob = pRoomIndex->people; mob != NULL; mob = mob_next )
   {
      mob_next = mob->next_in_room;

      if ( !IS_NPC( mob ) || ( mob->pIndexData->vnum != m1num && mob->pIndexData->vnum != m2num
           && mob->pIndexData->vnum != m3num && mob->pIndexData->vnum != m4num ) )
         continue;

      extract_char( mob, TRUE );
   }

   for ( counter = 1; counter < 5; counter++ )
   {
       switch( counter )
       {
          default: continue;
          case 1: vnum = m1num; break;
          case 2: vnum = m2num; break;
          case 3: vnum = m3num; break;
          case 4: vnum = m4num; break;
       }

       if ( vnum == -1 )
          continue;

       mob = create_mobile( get_mob_index( vnum ) );

       if ( mob == NULL )
          continue;

       char_to_room( mob, pRoomIndex );

       SET_BIT( mob->act, ACT_AGGRESSIVE );

       if ( mob->pIndexData->vnum == 553 )
          mob->hit = mob->max_hit = 120000000;
          else if ( mob->pIndexData->vnum == 569 )
                  mob->hit = mob->max_hit = 120000000;
          else if ( mob->pIndexData->vnum == 570 )
                  mob->hit = mob->max_hit = 40000000;
          else if ( mob->pIndexData->vnum == 551 || mob->pIndexData->vnum == 567
                    || mob->pIndexData->vnum == 561 || mob->pIndexData->vnum == 558 )
                  mob->hit = mob->max_hit = 50000000;
                  else mob->hit = mob->max_hit = 65000000;
   }
}

void create_dungeon( void )
{
   ROOM_INDEX_DATA *pRoomIndex;
   int vnum, counter, counter2, door, realexit;
   EXIT_DATA *pexit;
   bool ishigher = FALSE;

   clear_dungeon();
   dungeon_timer = 7;
   is_dungeon = 0;

   do_info( NULL, "#eThe #CDungeon #eof #RDeath #eis now open for #wpeople #ewho want to #R#zdie#n#e...#n" );
   do_info( NULL, "#eForm your #wgroup#e, then type #C#zComeOn#n #eif you #Pdare #etake the #ychallenge#e...#n" );

   for ( counter = 0; counter < 10; counter++ )
   {
      vnum = 33500 + ( counter * 20 );

      if ( number_range( 1, 4 ) > 2 )
         ishigher = TRUE;
         else ishigher = FALSE;

      for ( counter2 = vnum + 1; counter2 < vnum + 18; counter2++ )
      {
         if ( ( pRoomIndex = get_room_index( counter2 ) ) == NULL )
            continue;

         if ( number_percent() > 64 )
            pRoomIndex->trap = number_range( 1, 7 );
            else pRoomIndex->trap = 0;

         if ( counter2 == vnum + 8 || counter2 == vnum + 17 )
            realexit = ( number_range( 0, 3 ) > 1 ) ? 1 : 3;
            else realexit = number_range( 0, 3 );

         for ( door = 0; door < 4; door++ )
         {
            if ( ( counter2 == vnum + 8 || counter2 == vnum + 17 )
                 && ( door == 0 || door == 2 ) )
               continue;

            if ( pRoomIndex->exit[door] == NULL )
               {
                  pexit = alloc_perm( sizeof(*pexit) );
                  pexit->description = str_dup( "" );
                  pexit->keyword = str_dup( "" );
                  pexit->exit_info = 0;
                  pexit->rs_flags = 0;
                  pexit->key = -1;
                  pexit->vnum = 0;
                  pRoomIndex->exit[door] = pexit;
               }

            if ( door == realexit )
               {
                  if ( ishigher != TRUE )
                     {
                        if ( counter2 == vnum + 8 )
                           pRoomIndex->exit[door]->vnum = vnum + 18;
                           else if ( counter2 == vnum + 17 )
                                   pRoomIndex->exit[door]->vnum = vnum;
                                   else pRoomIndex->exit[door]->vnum = counter2 + 1;
                     }
                     else {
                             if ( counter2 == vnum + 17 )
                                pRoomIndex->exit[door]->vnum = vnum + 18;
                                else if ( counter2 == vnum + 8 )
                                        pRoomIndex->exit[door]->vnum = vnum;
                                        else pRoomIndex->exit[door]->vnum = counter2 + 1;
                          }
               }
               else pRoomIndex->exit[door]->vnum = vnum;

            pRoomIndex->exit[door]->to_room = get_room_index( pRoomIndex->exit[door]->vnum );
            top_exit++;
         }
      }
   }

   for ( counter = 1; counter < 11; counter++ )
   {
       load_boss( counter );
   }
}

void give_lprize( CHAR_DATA *ch )
{
   int option = number_range( 0, 2 ), counter;

   if ( IS_NPC( ch ) )
      return;

   if ( option == 0 )
      option = -1;

   ch->pcdata->forge = number_range( 16, 18 );
   find_item( ch, option );
   ch->pcdata->forge = 0;

   for ( counter = 0; counter < 31; counter++ )
   {
      if ( counter > 22 )
         counter -= 22;
         else switch( number_range( 1, 8 ) )
              {
                 default:
                 case 1: option = 1; break;
                 case 2: option = 2; break;
                 case 3: option = 4; break;
                 case 4: option = 8; break;
                 case 5: option = 16; break;
                 case 6: option = 32; break;
                 case 7: option = 64; break;
                 case 8: option = 128; break;
              }

      if ( IS_SET( ch->pcdata->hidden_skill, option ) )
         continue;

      SET_BIT( ch->pcdata->hidden_skill, option );
      stc( "#yYou #whave obtained a #ohidden #Pskill#w!!#n\n\r", ch );
      return;
   }
}

void check_level( CHAR_DATA *ch )
{
   CHAR_DATA *mob, *mob_next, *player, *player_next, *leader = NULL;
   int level, group_members = 0;
   char buf[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;

   if ( !IS_NPC( ch ) || ch->pIndexData->vnum < 550 || ch->pIndexData->vnum > 570 )
      return;

   for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
   {
      mob_next = mob->next_in_room;

      if ( !IS_NPC( mob ) || mob == ch )
         continue;

      if ( IS_NPC( mob ) && mob->pIndexData->vnum >= 550 && mob->pIndexData->vnum <= 570 )
         {
            // Death phrases go here
            act( "You scream '#yGaaa#n' as you are flung unconscious across the room!", ch, NULL, NULL, TO_CHAR );
            act( "$n screams '#yGaaa#n' as $e is flung unconscious across the room!", ch, NULL, NULL, TO_ROOM );

            return;
         }
   }

   act( "#yYou #wscream in #Rpain #was the powerful attack causes your body to #Cfizz #win the air#e!!#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #wscreams in #Rpain #was the powerful attack causes $s body to #Cfizz #win the air#e!!#n", ch, NULL, NULL, TO_ROOM );
   act( "#wAs the #ysparks #wof #Penergy #Rrips #waway from #Cyour #wbody, you are suddenly slammed onto the floor.\n\r#n", ch, NULL, NULL, TO_CHAR );

   level = ( ( ch->in_room->vnum - 33500 ) / 20 ) + 1;

   for ( player = ch->in_room->people; player != NULL; player = player_next )
   {
      player_next = player->next_in_room;

      if ( IS_NPC( player ) )
         continue;

      if ( leader == NULL )
         leader = player;

      if ( leader == player || is_same_group( leader, player ) )
         {
            group_members++;
            player->pcdata->faithpoints += ( level == 10 ) ? 15 : level;
            show_special_ko( player );
            act( "\n\r#wAs the #ysparks #wof #Penergy #Rrips #waway from #C$n#C's #wbody, $e is suddenly slammed onto the floor.\n\r#n", ch, NULL, player, TO_VICT );

            if ( level == 10 )
               {
                  give_lprize( player );
                  sprintf( buf, "\n\r#gYou #whave won #C%d #olegend #ppoints #was you overcome #C#zall#n #Rperils#w. Well done!#n", 15 );
               }
               else sprintf( buf, "\n\r#gYou #whave won #C%d #olegend #ppoints #was you transceed to the #Rnext #ylevel#w!#n", level );

            act( buf, player, NULL, NULL, TO_CHAR );
            save_char_obj( player );
         }
         else act( "\n\r#wAs the #ysparks #wof #Penergy #Rrips #waway from #C$n#C's #wbody, $e is suddenly slammed onto the floor.\n\r#n", ch, NULL, player, TO_VICT );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( leader == NULL )
         break;

      if ( d->character == NULL || d->character->in_room == NULL || !is_same_group( d->character, leader ) )
         continue;

      if ( d->character->in_room != ch->in_room )
         {
            group_members++;
            d->character->pcdata->faithpoints += ( level == 10 ) ? 15 : level;

            if ( level == 10 )
               {
                  give_lprize( d->character );
                  sprintf( buf, "\n\r#gYour #w group has won #C%d #olegend #ppoints #was you overcome #C#zall#n #Rperils#w. Well done!#n", 15 );
               }
               else sprintf( buf, "\n\r#gYour #wgroup has won #C%d #olegend #ppoints #was you transceed to the #Rnext #ylevel#w!#n", level );

            act( buf, d->character, NULL, NULL, TO_CHAR );
            save_char_obj( d->character );
         }

      char_from_room( d->character );

      if ( level == 10 )
         char_to_room( d->character, get_room_index( 800 ) );
         else char_to_room( d->character, get_room_index( 33500 + ( 20 * level ) ) );
   }

   if ( group_members < 2 && leader != NULL )
      {
         stc( "#yYou #wslam your #ofist #win the ground in #Rfrustration #was you realize your group is no more...#n\n\r", leader );
         char_from_room( leader );
         char_to_room( leader, get_room_index( 800 ) );
      }

   if ( level != 10 )
      load_boss( level );
      else {
              if ( leader != NULL )
                 {
                    sprintf( buf, "#g%s's #wteam has overcome #C#zall#n #Rperils#w in the #Rdungeon #wof #edeath#w!#n", leader->pcdata->switchname );
                    do_info( NULL, buf );
                 }

              clear_dungeon();
           }
}

void check_boss( CHAR_DATA *ch )
{
   CHAR_DATA *mob, *mob_next, *player, *player_next;
   int num = 0, level;

   for ( player = ch->in_room->people; player != NULL; player = player_next )
   {
      player_next = player->next_in_room;

      if ( IS_NPC( player ) )
         continue;

      num++;
   }

   if ( num > 1 )
      return;

   for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
   {
      mob_next = mob->next_in_room;

      if ( !IS_NPC( mob ) )
         continue;

      extract_char( mob, TRUE );
   }

   level = ( ( ch->in_room->vnum - 33500 ) / 20 ) + 1;
   load_boss( level );
}

void do_come_on( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *player, *player_next;
   int num = 0;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      {
         stc( "#wCome on! #yYou #ware already in the #Rdungeon #edying #w:p#n\n\r", ch );
         return;
      }

   if ( dungeon_timer == 0 )
      {
         stc( "#wThe #Centrance #wto #Rhell #wis #eclosed #wkid... Try again another time.#n\n\r", ch );
         return;
      }

   for ( player = ch->in_room->people; player != NULL; player = player_next )
   {
      player_next = player->next_in_room;

      if ( IS_NPC( player ) || !is_same_group( ch, player ) )
         continue;

      if ( player->pkill < 30 )
         {
            stc( "#wOnly big boys are allowed in the dungeon of death. Remove the kid.#n\n\r", ch );
            return;
         }

      num++;
   }

   if ( num < 2 )
      {
         stc( "#yAlthough #wyou #C#zreally#n #wwant to #edie#w, people like Ron prefer a #C-little- #wchallenge.#n\n\r", ch );
         return;
      }

   if ( num > 3 )
      {
         stc( "#yAlthough #wyou #C#zreally#n #wwant to #ebeat this#w, the fights have to be #Cfairly #weven.#n\n\r", ch );
         return;
      }

   for ( player = ch->in_room->people; player != NULL; player = player_next )
   {
      player_next = player->next_in_room;

      if ( IS_NPC( player ) || !is_same_group( ch, player ) )
         continue;

      char_from_room( player );
      char_to_room( player, get_room_index( 33500 ) );

      do_look( player, "" );
   }
}
