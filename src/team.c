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

void do_form( CHAR_DATA *ch, char *argument )
{
   int count; 
   int hash = 0;
   int point;
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   TEAM_DATA *my_team;
   TEAM_DATA *prev_team = NULL;

   if ( IS_NPC(ch) )
	return;

   if ( IS_SET( ch->arena, AFF2_COMPETITOR ) || IS_SET( ch->arena, AFF2_TEAM1 )
        || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
      {
         stc("You must not be in a team nor in the KOF tournament to use this.\n\r",ch);
         return;
      }

   for ( count = 0; count < strlen(argument); count++ )
   {
      if ( count < strlen(argument) && argument[count] == '#' && argument[count + 1] != '#' )
         hash++;
   }

   if ( count - ( hash * 2 ) > 14 )
      {
         stc("You can only have a max of 14 letters for your team name.\n\r",ch);
         return;
      }

   if ( count - ( hash * 2 ) < 3 )
      {
         stc( "Your team must have a minimum of 3 letters in it.\n\r", ch );
         return;
      }

   count = 0;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group( ch, vch ) || IS_NPC( vch ) || vch == ch
           || IS_SET( vch->arena, AFF2_COMPETITOR ) || IS_SET( vch->arena, AFF2_TEAM1 )
           || IS_SET( vch->arena, AFF2_TEAM2 ) ) 
         continue;

      if ( ++count == 3 )
         break;
   }

   strcat( argument, "#n" );
   smash_tilde( argument );

   my_team = team_list;

   while ( my_team != NULL && my_team->striker_points[1] != -1 )
   {
      prev_team = my_team;
      my_team = my_team->next;
   }

   if ( my_team == NULL )
      {
         if ( team_free == NULL )
            my_team = alloc_perm( sizeof(*my_team) );
            else {
                    my_team = team_free;
                    team_free = team_free->next;
                 }

         free_string( my_team->team_name[0] );
         my_team->team_name[0] = str_dup( argument );
         my_team->team_numbers[0] = count + 1;
         my_team->team_power[0] = 0;
         my_team->striker_points[0] = 3;
         my_team->power_mode[0] = 0;
         my_team->power_duration[0] = 0;
         my_team->team_eliminated[0] = 0;
         free_string( my_team->win_type[0] );
         my_team->win_type[0] = str_dup("    ");

         my_team->challenger[0] = ch;
         count = 1;
         SET_BIT( ch->arena, AFF2_TEAM1 );
 
         for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
         {
            vch_next = vch->next_in_room;

            if ( !is_same_group( ch, vch ) || IS_NPC( vch ) || vch == ch
                 || IS_SET( vch->arena, AFF2_COMPETITOR ) || IS_SET( vch->arena, AFF2_TEAM1 )
                 || IS_SET( vch->arena, AFF2_TEAM2 ) ) 
               continue; 

            SET_BIT( vch->arena, AFF2_TEAM1 );

            if ( vch->generation < 5 )
               my_team->team_power[0] += ( ( 6 - vch->generation ) * 5 );

            my_team->team_power[0] += vch->race;
            my_team->team_power[0] += ( vch->pcdata->clanrank - 1 ) * 5;
            my_team->team_power[0] += vch->pkill;
            my_team->team_power[0] += ( vch->pcdata->legend * 2 );

            my_team->challenger[count] = vch;

            if ( ++count == 4 )
               break;
         }

         my_team->striker_points[1] = -1;
         my_team->next = NULL;

         if ( prev_team == NULL )
            team_list = my_team;
            else prev_team->next = my_team;
      }
      else {
              free_string( my_team->team_name[1] );
              my_team->team_name[1] = str_dup( argument );
              my_team->team_numbers[1] = count + 1;
              my_team->team_power[1] = 0;
              my_team->striker_points[1] = 3;
              my_team->power_mode[1] = 0;
              my_team->power_duration[1] = 0;
              my_team->team_eliminated[1] = 0;
              free_string( my_team->win_type[1] );
              my_team->win_type[1] = str_dup("    ");

              my_team->opponent[0] = ch;
              count = 1;
              SET_BIT( ch->arena, AFF2_TEAM2 );
 
              for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
              {
                 vch_next = vch->next_in_room;

                 if ( !is_same_group( ch, vch ) || IS_NPC( vch ) || vch == ch
                      || IS_SET( vch->arena, AFF2_COMPETITOR ) || IS_SET( vch->arena, AFF2_TEAM1 )
                      || IS_SET( vch->arena, AFF2_TEAM2 ) ) 
                    continue;

                 SET_BIT( vch->arena, AFF2_TEAM2 );

                 if ( vch->generation < 5 )
                    my_team->team_power[1] += ( ( 6 - vch->generation ) * 5 );

                 my_team->team_power[1] += vch->race;
                 my_team->team_power[1] += ( vch->pcdata->clanrank - 1 ) * 5;
                 my_team->team_power[1] += vch->pkill;
                 my_team->team_power[1] += ( vch->pcdata->legend * 2 );

                 my_team->opponent[count] = vch;

                 if ( ++count == 4 )
                    break;
              }

              my_team->round = 0;
           }

    stc( "#yWell done #wwarrior. Your #Cteam #whas been formed.#n\n\r", ch );

}

void do_adjust( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   int choice = -1;
   TEAM_DATA *my_team;

   argument = one_argument(argument, arg);

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
      {
         send_to_char("But you aren't in a team warrior.\n\r", ch );
         return;
      }	

   if ( !str_cmp( "first", arg ) || !str_cmp( "one", arg ) || !str_cmp( "1", arg ) )
      choice = 0;
      else if ( !str_cmp( "second", arg ) || !str_cmp( "two", arg ) || !str_cmp( "2", arg ) )
              choice = 1;
      else if ( !str_cmp( "third", arg ) || !str_cmp( "three", arg ) || !str_cmp( "3", arg ) )
              choice = 2;
      else if ( !str_cmp( "fourth", arg ) || !str_cmp( "four", arg ) || !str_cmp( "4", arg ) )
              choice = 3;
              else {
                      stc( "#wWhich #yposition #wdo you wish to #Cstart #win? #P[1-4]#n\n\r", ch );
                      return;
                   }

   my_team = team_list;

   while ( my_team != NULL )
   {
      int position = 0;

      position = 0;

      do
      {
         if ( my_team->challenger[position] == ch )
            {
               if ( choice == position )
                  {
                     stc( "You are already starting in that position warrior.\n\r", ch );
                     return;
                  }
               
               if ( choice == 3 )
                  my_team->challenger[position] = my_team->challenger[3];
                  else {
                          int count;

                          if ( choice < position )
                             {
                                for ( count = position; count > choice; count-- )
                                {
                                    my_team->challenger[count] = my_team->challenger[count - 1];
                                }
                             }
                             else {
                                     for ( count = position; count < choice; count++ )
                                     {
                                        my_team->challenger[count] = my_team->challenger[count + 1];
                                     }
                                  }
                       }

               my_team->challenger[choice] = ch;
               stc( "#wYour #ystarting #wposition in the team is now as you requested.#n\n\r", ch );
               return;
            }

         position++;
      }
      while ( position < 4 );

      position = 0;

      do
      {
         if ( my_team->opponent[position] == ch )
            {
               if ( choice == position )
                  {
                     stc( "You are already starting in that position warrior.\n\r", ch );
                     return;
                  }
               
               if ( choice == 3 )
                  my_team->opponent[position] = my_team->opponent[3];
                  else {
                          int count;

                          if ( choice < position )
                             {
                                for ( count = position; count > choice; count-- )
                                {
                                    my_team->opponent[count] = my_team->opponent[count - 1];
                                }
                             }
                             else {
                                     for ( count = position; count < choice; count++ )
                                     {
                                        my_team->opponent[count] = my_team->opponent[count + 1];
                                     }
                                  }
                       }

               my_team->opponent[choice] = ch;
               stc( "#wYour #ystarting #wposition in the team is now as you requested.#n\n\r", ch );
               return;
            }

         position++;
      }
      while ( position < 4 );

      my_team = my_team->next;
   }

   stc( "You don't seem to be in a team warrior...\n\r", ch );
}

void remove_team_player( CHAR_DATA *ch, int team )
{
   TEAM_DATA *my_team;
   int team_members = 0;

   my_team = team_list;

   while ( my_team != NULL )
   {
      int position = 0;
      int count;

      position = 0;

      do
      {
         if ( team == 0 )
            {
               if ( my_team->challenger[position] == ch )
                  {
                     for ( count = 0; count < 4; count++ )
                     {
                        if ( my_team->challenger[count] != NULL )
                           team_members++;
                     }

                     my_team->challenger[position] = NULL;

                     if ( team_members >= my_team->team_numbers[0] )
                        my_team->team_numbers[0]--;

                     return;
                  }
            }
            else {
                     if ( my_team->opponent[position] == ch )
                        {
                           for ( count = 0; count < 4; count++ )
                           {
                              if ( my_team->opponent[count] != NULL )
                                 team_members++;
                           }

                           my_team->opponent[position] = NULL;

                           if ( team_members >= my_team->team_numbers[1] )
                              my_team->team_numbers[1]--;

                           return;
                        }
                 }

         position++;
      }
      while ( position < 4 );
   }
}

void remove_team( void )
{
   TEAM_DATA *my_team;
   int count;

   if ( team_list == NULL )
      return;

   my_team = team_list;
   team_list = team_list->next;

   my_team->striker_points[0] = my_team->striker_points[1] = -1;
   my_team->round = 0;

   for ( count = 0; count < 4; count++ )
   {
      if ( my_team->challenger[count] != NULL )
         {
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_TEAM1 );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_CHALLENGED );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_CHALLENGER );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_INARENA );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_INTROC );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_WINNER );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_ASSIST );
            REMOVE_BIT( my_team->challenger[count]->arena, AFF2_DM );
            my_team->challenger[count] = NULL;
         }

      if ( my_team->opponent[count] != NULL )
         {
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_TEAM2 );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_CHALLENGED );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_CHALLENGER );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_INARENA );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_INTROC );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_WINNER );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_ASSIST );
            REMOVE_BIT( my_team->opponent[count]->arena, AFF2_DM );
            my_team->opponent[count] = NULL;
         }
   }

   if ( team_free == NULL )
      my_team->next = NULL;
      else my_team->next = team_free;

   team_free = my_team;
}

void assign_pose_limit( CHAR_DATA *ch )
{
   if ( IS_NPC(ch) && ch->pIndexData->vnum == 554 && pose_limit < 2 )
      pose_limit = 2;
      else if ( IS_NPC(ch) && ( ch->pIndexData->vnum == 559 || 
                ch->pIndexData->vnum == 560 || ch->pIndexData->vnum == 550 ||
                ch->pIndexData->vnum == 552 || ch->pIndexData->vnum == 557 ||
                ch->pIndexData->vnum == 556 || ch->pIndexData->vnum == 561 )
                && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_NPC(ch) && ( ch->pIndexData->vnum == 551 ||
                ch->pIndexData->vnum == 555 ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_MONK ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_DRAGOON ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_SHADOW_WAR ) && pose_limit < 2 )
              pose_limit = 2;
      else if ( IS_CLASS( ch, CLASS_SHADOW ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_DRAGOON ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_DROW ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_NINJA ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_WEREWOLF ) && pose_limit < 2 )
              pose_limit = 2;
      else if ( IS_CLASS( ch, CLASS_VAMPIRE ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_PALADIN ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_DEMON ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_SAIYAN ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_DARAMON ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_ASSASSIN ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_SAMURAI ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_DRAGON ) && pose_limit < 2 )
              pose_limit = 2;
      else if ( IS_CLASS( ch, CLASS_DAYWALKER ) && pose_limit < 3 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_NECROMANCER ) && pose_limit < 2 )
              pose_limit = 2;
      else if ( IS_CLASS( ch, CLASS_ANGEL ) && pose_limit < 4 )
              pose_limit = 4;
      else if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) && pose_limit < 2 )
              pose_limit = 3;
      else if ( IS_CLASS( ch, CLASS_WIZARD ) && pose_limit < 2 )
              pose_limit = 2;
      else if ( ( IS_CLASS( ch, CLASS_SNOW_ELF ) || IS_CLASS( ch, CLASS_HOBBIT )
                || IS_CLASS( ch, CLASS_PHOENIX ) || IS_CLASS( ch, CLASS_DUERGAR )
                || IS_CLASS( ch, CLASS_WISP ) ) && pose_limit < 2 )
              pose_limit = 2;
              else if ( pose_limit < 1 )
                      pose_limit = 1;
}

void show_monk( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You silently shake the dust out of your left leg as you prepare for battle.", ch, NULL, NULL, TO_CHAR );
              act("$n silently shakes the dust out of $s left leg as $e prepares for battle.", ch, NULL, NULL, TO_ROOM );
              break;
      case 1: act("Looking carefully at your clothes, you quickly shake the dust out of your right leg.", ch, NULL, NULL, TO_CHAR );
              act("Looking carefully at $s clothes, $n quickly shakes the dust out of $s right leg.", ch, NULL, NULL, TO_ROOM );
              break;
      case 2: act("You then do a beautiful set of kicks as you feel the adrenaline rushes through you.", ch, NULL, NULL, TO_CHAR );
              act("$n then does a beautiful set of kicks as $e feels the adrenaline rushes through $m.", ch, NULL, NULL, TO_ROOM );
              break;
      case 3: act("You mutter '#yZu noi tu eiji sanada...#n' as you crouch on your knees.", ch, NULL, NULL, TO_CHAR);
              act("$n mutters '#yZu noi tu eiji sanada...#n' as $e crouches on $s knees.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_highwind( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You lean back on your spear as you take a long puff off your cigarette.", ch, NULL, NULL, TO_CHAR );
              act("$n leans back on $s spear as $e takes a long puff off $s cigarette.", ch, NULL, NULL, TO_ROOM );
              break;
      case 1: act("You then blow a ring of smoke into $N's face as you tighten the grip on your spear.", ch, NULL, ch->challenged, TO_CHAR );
              act("$n then blows a ring of smoke into your face as $e tightens the grip on $s spear.", ch, NULL, ch->challenged, TO_VICT );
              act("$n then blows a ring of smoke into $N's face as $e tightens the grip on $s spear.", ch, NULL, ch->challenged, TO_NOTVICT );
              break;
      case 2: act("You exclaim '#yTime to take you to school punk!#n'", ch, NULL, NULL, TO_CHAR);
              act("$n exclaims '#yTime to take you to school punk!#n'", ch, NULL, NULL, TO_ROOM);
              break;
      case 3: act("You then swipe your spear viciously at $N's face as you are ready to kick some ass.", ch, NULL, ch->challenged, TO_CHAR );
              act("$n then swipes $s spear viciously at your face as $e looks ready to kick some ass.", ch, NULL, ch->challenged, TO_VICT );
              act("$n then swipes $s spear viciously at $N's face as $e looks ready to kick some ass.", ch, NULL, ch->challenged, TO_NOTVICT );
              break;
      default: break;
   }
}

void show_saiyan( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You do a variety of pushups and exercises as your muscles ripple.", ch, NULL, NULL, TO_CHAR );
              act("$n does a variety of pushups and exercises as $s muscles ripple.", ch, NULL, NULL, TO_ROOM );
              break;
      case 1: act("You scream loudly as you charge up your powerlevel.", ch, NULL, NULL, TO_CHAR );
              act("$n screams loudly as $e charges up $s powerlevel.", ch, NULL, NULL, TO_ROOM );
              break;
      case 2: act("You then smash your foot into the ground as the entire area trembles.", ch, NULL, NULL, TO_CHAR);
              act("$n then smashes $s foot into the ground as the entire area trembles.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_demon( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You savagely tear a hole through reality as you step in from the vortex.", ch, NULL, NULL, TO_CHAR );
              act("A hole through reality is savagely ripped down as $n steps out from it.", ch, NULL, NULL, TO_CHAR );
              break;
      case 1: act("You cackle with glee '#yMuhahahahaha! No competition!#n' as you flex your muscles.", ch, NULL, NULL, TO_CHAR );
              act("$n cackles with glee '#yMuhahahahaha! No competition!#n' as $e flexes $s muscles.", ch, NULL, NULL, TO_ROOM );
              break;
      case 2: act("You give $N one more look as you sneer '#yDie!#n'", ch, NULL, ch->challenged, TO_CHAR );
              act("$n gives you one more look as $e sneers '#yDie!#n'", ch, NULL, ch->challenged, TO_VICT );
              act("$n gives $N one more look as $e sneers '#yDie!#n'", ch, NULL, ch->challenged, TO_NOTVICT );
              break;
      default: break;
   }
}

void show_drow( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You manifest slowly into existence curled up in a bluish ball.", ch, NULL, NULL, TO_CHAR );
              act("$n manifests slowly into existence curled up in a bluish ball.", ch, NULL, NULL, TO_ROOM );
              break;
      case 1: act("Suddenly, the ball shatters as you levitate slowly onto the ground.", ch, NULL, NULL, TO_CHAR );
              act("Suddenly, the ball shatters as $n levitates slowly onto the ground.", ch, NULL, NULL, TO_ROOM );
              act("As you let go a soft smile, blinding light fills the room as Lolth comes behind you.", ch, NULL, NULL, TO_CHAR );
              act("As $n lets go a soft smile, blinding light fills the room as Lolth comes behind $m.", ch, NULL, NULL, TO_ROOM );
              break;
      case 2: act("Lolth points towards you with her lightblade saying '#yUn soi se ni da...#n'", ch->challenged, NULL, NULL, TO_CHAR );
              act("Lolth points towards $n with her lightblade saying '#yUn soi se ni da...#n'", ch->challenged, NULL, NULL, TO_ROOM );
              break;
      case 3: act("You sneer '#yHoi!#n' as Lolth touches you softly, then vanishing back into the darkness.", ch, NULL, NULL, TO_CHAR);
              act("$n sneers '#yHoi!#n' as Lolth touches $m softly, then vanishing back into the darkness.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_ninja( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You tilt your fingers viciously as you swerve your hands around around.", ch, NULL, NULL, TO_CHAR);
              act("$n tilts $s fingers viciously as $e swerves $s hands around around.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You suddenly bend your legs apart as you tighten your arms.", ch, NULL, NULL, TO_CHAR);
              act("$n suddenly bends $s legs apart as $e tighten $s arms.", ch, NULL, NULL, TO_ROOM);
              break;
      case 2: act("Moving back and forth on the spot, you close your eyes as you rubs your hands.", ch, NULL, NULL, TO_CHAR);
              act("Moving back and forth on the spot, $n closes $s eyes as $e rubs $s hands.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_knight( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You raise your weapon in the air as your heart is filled with pride.", ch, NULL, NULL, TO_CHAR);
              act("$n raises $s weapon in the air as $s heart is filled with pride.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You scream '#yEnguarde!#n' as you gaze intently at $N.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n screams '#yEnguarde!#n' as $e gazes intently at you.", ch, NULL, ch->challenged, TO_VICT);
              act("$n screams '#yEnguarde!#n' as $e gazes intently at $N.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 2: act("You then move back and forth in your deadly battle pose, waiting in anticipation.", ch, NULL, NULL, TO_CHAR);
              act("$n then moves back and forth in $s deadly battle pose, waiting in anticipation.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_shadow( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("The shadows disperse around you as you raise your head slowly...", ch, NULL, NULL, TO_CHAR);
              act("The shadows disperse around $n as $e raises $s head slowly...", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You then switch into your shokotan stance as you gaze at $N silently.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n then switches into $s shokotan stance as $e gazes at you silently.", ch, NULL, ch->challenged, TO_VICT);
              act("$n then switches into $s shokotan stance as $e gazes at $N silently.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      default: break;
   }
}

void show_daramon( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You clutch the prized sword as you sigh sadly.", ch, NULL, NULL, TO_CHAR);
              act("$n clutches the prized sword as $e sighs sadly.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You then say to $N '#yAren't you tired of all this death and decay?#n'", ch, NULL, ch->challenged, TO_CHAR);
              act("$n then says to you '#yAren't you tired of all this death and decay?#n'", ch, NULL, ch->challenged, TO_VICT);
              act("$n then says to $N '#yAren't you tired of all this death and decay?#n'", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 2: act("Your sword trembles slightly as you snarl '#yThe cleansing will start with you!#n'", ch, NULL, NULL, TO_CHAR);
              act("$n's sword trembles slightly as you snarl '#yThe cleansing will start with you!#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_clone( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You activate your eye scanner as you focus intently on $N.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n activates $s eye scanner as $e focuses intently on you.", ch, NULL, ch->challenged, TO_VICT);
              act("$n activates $s eye scanner as $e focuses intently on $N.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 1: act("You say '#yTarget scanned... subject confirmed...#n'", ch, NULL, NULL, TO_CHAR);
              act("$n says '#yTarget scanned... subject confirmed...#n'", ch, NULL, NULL, TO_ROOM);
              break;
      case 2: act("You say '#yMode Option... Kill...#n' as you lean into an offensive stance.", ch, NULL, NULL, TO_CHAR);
              act("$n says '#yMode Option... Kill...#n' as $e leans into an offensive stance.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_assassin( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You collapse to your knees as you start cackling wildly.", ch, NULL, NULL, TO_CHAR);
              act("$n collapses to $s knees as $e starts cackling wildly.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("Your eyes then burn with rage as flashes of your past haunt you.", ch, NULL, NULL, TO_CHAR);
              act("$n's eyes then burn with rage as flashes of $s past haunt $m.", ch, NULL, NULL, TO_ROOM);
              break;
      case 2: act("You suddenly leap to your feet pushing $N away from you savagely.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n suddenly leaps to $s feet pushing you away from $m savagely.", ch, NULL, ch->challenged, TO_VICT);
              act("$n suddenly leaps to $s feet pushing $N away from $m savagely.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 3: act("You then chuckle saying '#yGive me a reason why you should live...#n'", ch, NULL, NULL, TO_CHAR);
              act("$n then chuckles saying '#yGive me a reason why you should live...#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_wolf( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You hop quickly a few times as you harness your powers from within.", ch, NULL, NULL, TO_CHAR);
              act("$n hops quickly a few times as $e harnesses $s powers from within.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You then yell '#yHai#n as you punch wildly to your right and go in your battle pose.", ch, NULL, NULL, TO_CHAR);
              act("$n then yells '#yHai#n as $e punches wildly to $s right and go in $s battle pose.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_dragon( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You release a deafening screech as you flap your wings noisily.", ch, NULL, NULL, TO_CHAR);
              act("$n releases a deafening screech as $e flaps $s wings noisily.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You then eye $N hungrily as you lip your mouth in anticipation of a meal.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n then eyes you hungrily as $e lips $s mouth in anticipation of a meal.", ch, NULL, ch->challenged, TO_VICT);
              act("$n then eyes $N hungrily as $e lips $s mouth in anticipation of a meal.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      default: break;
   }
}

void show_samurai( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You grip your katana tightly as you frown at $N.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n grips $s katana tightly as $e frowns at you.", ch, NULL, ch->challenged, TO_VICT);
              act("$n grips $s katana tightly as $e frowns at $N.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 1: act("You then grasp $N by the neck as you raise $M above you.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n then grasps you by the neck as $e raises you above $m.", ch, NULL, ch->challenged, TO_VICT);
              act("$n then grasps $N by the neck as $e raises $M above $m.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 2: act("You then mutter '#yYour presence insults the Emperor... DIE!#n'", ch, NULL, NULL, TO_CHAR);
              act("$n then mutters '#yYour presence insults the Emperor... DIE!#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_necro( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("Your zombies swarm all around you as you clutch your body seductively.", ch, NULL, NULL, TO_CHAR);
              act("$n's zombies swarm all around $m as $e clutches $s body seductively.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("You then cackle '#yIts going to be a fun night heheheheheee#n'", ch, NULL, NULL, TO_CHAR);
              act("$n then cackles '#yIts going to be a fun night heheheheheee#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_daywalker( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You snarl your fangs angrily at $N as you quickly reach into your pockets.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n snarl $s fangs angrily at you as $e quickly reaches into $s pockets.", ch, NULL, ch->challenged, TO_VICT);
              act("$n snarl $s fangs angrily at $N as $e quickly reaches into $s pockets.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 1: act("Pulling out the serum, you inject yourself with it savagely!", ch, NULL, NULL, TO_CHAR);
              act("Pulling out the serum, $n injects $mself with it savagely!", ch, NULL, NULL, TO_ROOM);
              break;
      case 2: act("You then swerve into a battle pose saying '#yTime to end this!#n'", ch, NULL, NULL, TO_CHAR);
              act("$n then swerves into a battle pose saying '#yTime to end this!#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_angel( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You start singing for the glory of God as you spread your wings widely.", ch, NULL, NULL, TO_CHAR);
              act("$n starts singing for the glory of God as $e spreads $s wings widely.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("A holious chorus accompanies your singing as a bright light shines on you.", ch, NULL, NULL, TO_CHAR);
              act("A holious chorus accompanies $n's singing as a bright light shines on $m.", ch, NULL, NULL, TO_ROOM);
              break;
      case 2: act("Feeling totally at peace now, you open your hand allowing your sword to fly into it.", ch, NULL, NULL, TO_CHAR);
              act("Feeling totally at peace now, $n opens $s hand allowing $s sword to fly into it.", ch, NULL, NULL, TO_ROOM);
              break;
      case 3: act("You say with infinite kindness '#yRepent now and save your soul.#n'", ch, NULL, NULL, TO_CHAR);
              act("$n says with infinite kindness '#yRepent now and save your soul.#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_undead( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You place your hand on your demonic mask as time seems to stand still.", ch, NULL, NULL, TO_CHAR);
              act("$n places $s hand on $s demonic mask as time seems to stand still.", ch, NULL, NULL, TO_CHAR);
              break;
      case 1: act("$N shields $S eyes in terror as you start chuckling.", ch, NULL, ch->challenged, TO_CHAR);
              act("You shield your eyes in terror as $n starts chuckling.", ch, NULL, ch->challenged, TO_VICT);
              act("$N shields $S eyes in terror as $n starts chuckling.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 2: act("You then grasp your axe saying '#yTHIS is what you should be scared of#n'", ch, NULL, NULL, TO_CHAR);
              act("$n then grasps $s axe saying '#yTHIS is what you should be scared of#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_ele( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You hover silently before $N as a sihoulette of your deity appears behind you.", ch, NULL, ch->challenged, TO_CHAR);
              act("$n hovers silently before you as a sihoulette of $s deity appears behind $m.", ch, NULL, ch->challenged, TO_VICT);
              act("$n hovers silently before $N as a sihoulette of $s deity appears behind $m.", ch, NULL, ch->challenged, TO_NOTVICT);
              break;
      case 1: act("You then summon your elemental powers as your body radiates intensively.", ch, NULL, NULL, TO_CHAR);
              act("$n then summons $s elemental powers as $s body radiates intensively.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_wizard( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You appear in a poof of smoke as you start chuckling.", ch, NULL, NULL, TO_CHAR);
              act("$n appear in a poof of smoke as you start chuckling.", ch, NULL, NULL, TO_CHAR);
              break;
      case 1: act("You then say confidently '#yI promise to end this quick!#n'", ch, NULL, NULL, TO_CHAR);
              act("$n then says confidently '#yI promise to end this quick!#n'", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_vampire( CHAR_DATA *ch )
{
   switch( pose_timer )
   {
      case 0: act("You tilt your hands in an ancient ritual as you levitate into the air.", ch, NULL, NULL, TO_CHAR);
              act("$n tilt $s hands in an ancient ritual as $e levitates into the air.", ch, NULL, NULL, TO_ROOM);
              break;
      case 1: act("Your capes start fluttering in the wind as your eyes fill with demonic energy.", ch, NULL, NULL, TO_CHAR);
              act("$n's capes start fluttering in the wind as $s eyes fill with demonic energy.", ch, NULL, NULL, TO_ROOM);
              act("You mutter '#yVampi nosra xu bien natu...#n' as a sword manifests beyond you.", ch, NULL, NULL, TO_CHAR);
              act("$n mutters '#yVampi nosra xu bien natu...#n' as a sword manifests beyond $m.", ch, NULL, NULL, TO_ROOM);
              break;
      case 3: act("You open your fists as the sword melts into a puddle of blood.", ch, NULL, NULL, TO_CHAR);
              act("$n opens $s fists as the sword melts into a puddle of blood.", ch, NULL, NULL, TO_ROOM);
              act("You shudder in pleasure as the blood suddenly washes your powerful body.", ch, NULL, NULL, TO_CHAR);
              act("$n shudders in pleasure as the blood suddenly washes $s powerful body.", ch, NULL, NULL, TO_ROOM);
              break;
      default: break;
   }
}

void show_poses( CHAR_DATA *ch )
{
   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      return;

   if ( ch->challenged == NULL )
      return;

   if ( IS_SET( ch->arena, AFF2_INTROC ) )
      return;

   if ( pose_timer == 0 )
      assign_pose_limit( ch );

   if ( IS_CLASS( ch, CLASS_MONK ) )
      show_monk( ch );
      else if ( IS_CLASS( ch, CLASS_DROW ) )
              show_drow( ch );
      else if ( IS_CLASS( ch, CLASS_NINJA ) || IS_CLASS( ch, CLASS_NIGHT_BLADE ) )
              show_ninja( ch );
      else if ( IS_CLASS( ch, CLASS_WEREWOLF ) )
              show_wolf( ch );
      else if ( IS_CLASS( ch, CLASS_DEMON ) )
              show_demon( ch );
      else if ( IS_CLASS( ch, CLASS_VAMPIRE ) )
              show_vampire( ch );
      else if ( IS_CLASS( ch, CLASS_SAIYAN ) )
              show_saiyan( ch );
      else if ( IS_CLASS( ch, CLASS_DRAGOON ) )
              show_highwind( ch );
      else if ( IS_CLASS( ch, CLASS_SHADOW_WAR ) )
              show_shadow( ch );
      else if ( IS_CLASS( ch, CLASS_SHADOW ) )
              show_assassin( ch );
      else if ( IS_CLASS( ch, CLASS_ASSASSIN ) )
              show_clone( ch );
      else if ( IS_CLASS( ch, CLASS_DARAMON ) )
              show_daramon( ch );
      else if ( IS_CLASS( ch, CLASS_DRAGON ) )
              show_dragon( ch );
      else if ( IS_CLASS( ch, CLASS_SAMURAI ) )
              show_samurai( ch );
      else if ( IS_CLASS( ch, CLASS_DAYWALKER ) )
              show_daywalker( ch );
      else if ( IS_CLASS( ch, CLASS_NECROMANCER ) )
              show_necro( ch );
      else if ( IS_CLASS( ch, CLASS_WIZARD ) )
              show_wizard( ch );
      else if ( IS_CLASS( ch, CLASS_ANGEL ) )
              show_angel( ch );
      else if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
              show_undead( ch );
      else if ( IS_CLASS( ch, CLASS_SNOW_ELF ) || IS_CLASS( ch, CLASS_HOBBIT )
                || IS_CLASS( ch, CLASS_PHOENIX ) || IS_CLASS( ch, CLASS_DUERGAR )
                || IS_CLASS( ch, CLASS_WISP ) )
              show_ele( ch );
      else if ( IS_CLASS( ch, CLASS_PALADIN ) || ( IS_NPC(ch) && ( ch->pIndexData->vnum == 550 
                || ch->pIndexData->vnum == 552 || ch->pIndexData->vnum == 557 ) ) )
              show_knight( ch );
}

void show_intro( void )
{
   CHAR_DATA *ch = NULL, *victim = NULL;
   int count;
   int found;
   bool chmsg = FALSE, vchmsg = FALSE;
   char *cname, *vname;
   char buf[MAX_STRING_LENGTH];

   count = 0;
   found = 0;

   do
   {
      if ( team_list->challenger[count] != NULL )
         {
            if ( found == team_list->team_eliminated[0] )
               ch = team_list->challenger[count];

            found++;
         }

      count++;
   }
   while ( ch == NULL && count < 4 );

   count = 0;
   found = 0;

   do
   {
      if ( team_list->opponent[count] != NULL )
         {
            if ( found == team_list->team_eliminated[1] )
               victim = team_list->opponent[count];

            found++;
         }

      count++;
   }
   while ( victim == NULL && count < 4 );

   if ( ch == NULL || victim == NULL )
      {
         do_tinfo( NULL, NULL, "#wThe #ycurrent #Pchallenge #wis disbarred dued to #c#zmissing#n #wfighters.#n" );
         remove_team();
         arena = FIGHT_CLEAR;
         return;
      }

   if ( !IS_NPC( ch ) && ch->pcdata->watching != NULL )
      do_arena_watch( ch, "" );

   if ( !IS_NPC( victim ) && victim->pcdata->watching != NULL )
      do_arena_watch( victim, "" );

   REMOVE_BIT( ch->arena, AFF2_ASSIST );
   REMOVE_BIT( ch->arena, AFF2_WINNER );
   REMOVE_BIT( ch->arena, AFF2_DM );

   pose_timer = 0;
   pose_limit = -1;
   team_list->power_duration[0] = team_list->power_duration[1] = 0;
   team_list->power_mode[0] = team_list->power_mode[1] = 0;

   if ( IS_NPC( ch ) )
      cname = str_dup( ch->short_descr );
      else cname = str_dup( ch->pcdata->switchname );

   if ( IS_NPC( victim ) )
      vname = str_dup( victim->short_descr );
      else vname = str_dup( victim->pcdata->switchname );

   if ( ++team_list->round != 7 )
      sprintf( buf, "#wRound #y%d #w- #C%s #yVs #p%s", team_list->round, cname, vname );
      else sprintf( buf, "#CFinal #yRound #w- #C%s #yVs #p%s", cname, vname );

   do_tinfo( ch, victim, buf );

   stop_fighting( ch, TRUE );
   ch->arena_timer = 40;
   ch->fight_timer = 0;
   ch->nemesis = 0;
   SET_BIT( ch->arena, AFF2_INARENA );

   if ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 )
      {
         restore_char( ch );

         if ( team_list->round > 1 )
            {
               chmsg = TRUE;
               team_list->striker_points[0] = UMIN( 5, team_list->striker_points[0]++ );
            }
      }
      else {
              ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit / 4 ) );
              ch->mana = UMIN( ch->max_mana, ch->mana + ( ch->max_mana / 4 ) );
              ch->move = UMIN( ch->max_move, ch->move + ( ch->max_move / 4 ) );
           }

   char_from_room( ch );
   char_to_room( ch, get_room_index( 570 ) );
   ch->challenged = victim;

   if ( IS_SET( ch->extra, EXTRA_AFK ) )
      REMOVE_BIT( ch->extra, EXTRA_AFK );

   stop_fighting( victim, TRUE );
   victim->fight_timer = 0;
   victim->nemesis = 0;
   victim->arena_timer = 40;
   SET_BIT( victim->arena, AFF2_INARENA );

   if ( victim->in_room->vnum < 550 || victim->in_room->vnum >= 630 )
      {
         restore_char( victim );

         if ( team_list->round > 1 )
            {
               vchmsg = TRUE;
               team_list->striker_points[1] = UMIN( 5, team_list->striker_points[1]++ );
            }
      }
      else {
              victim->hit = UMIN( victim->max_hit, victim->hit + ( victim->max_hit / 4 ) );
              victim->mana = UMIN( victim->max_mana, victim->mana + ( victim->max_mana / 4 ) );
              victim->move = UMIN( victim->max_move, victim->move + ( victim->max_move / 4 ) );
           }

   char_from_room( victim );
   char_to_room( victim, get_room_index( 570 ) );
   victim->challenged = ch;

   if ( IS_SET( victim->extra, EXTRA_AFK ) )
      REMOVE_BIT( victim->extra, EXTRA_AFK );

   if ( chmsg == TRUE )
      {
         act( "#yYou #wquickly leap in front of #C$N #wbeing full of vigor and anticipation.#n", ch, NULL, ch->challenged, TO_CHAR );
         act( "#y$n #wquickly leaps in front of #Cyou #wbeing full of vigor and anticipation.#n", ch, NULL, ch->challenged, TO_VICT );
         act( "#y$n #wquickly leaps in front of #C$N #wbeing full of vigor and anticipation.#n", ch, NULL, ch->challenged, TO_NOTVICT );
      }

   if ( vchmsg == TRUE )
      {
         act( "#yYou #wquickly leap in front of #C$N #wbeing full of vigor and anticipation.#n", victim, NULL, victim->challenged, TO_CHAR );
         act( "#y$n #wquickly leaps in front of #Cyou #wbeing full of vigor and anticipation.#n", victim, NULL, victim->challenged, TO_VICT );
         act( "#y$n #wquickly leaps in front of #C$N #wbeing full of vigor and anticipation.#n", victim, NULL, victim->challenged, TO_NOTVICT );
      }

   do_look( ch, "" );
   do_look( victim, "" );
}

void show_round( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      return;

   stc( "\n\r", ch );

   switch( team_list->round )
   {
      case 1: stc( "               #w*****          ****\n\r", ch );
              stc( "               #w*     *          *\n\r", ch );
              stc( "               #w* * *           *\n\r", ch );
              stc( "               #w*     *        *\n\r", ch );
              stc( "               #w*       *  *********\n\r", ch );
              stc( "               #C     ROUND ONE\n\r", ch );
              break;

      case 2: stc( "               #w*****       ******\n\r", ch );
              stc( "               #w*     *    *    *\n\r", ch );
              stc( "               #w* * *        ***\n\r", ch );
              stc( "               #w*     *     *   \n\r", ch );
              stc( "               #w*       *  *******  \n\r", ch );
              stc( "               #C     ROUND TWO\n\r", ch );
              break;

      case 3: stc( "               #w*****         ******\n\r", ch );
              stc( "               #w*     *           *\n\r", ch );
              stc( "               #w* * *          ***\n\r", ch );
              stc( "               #w*     *         *   \n\r", ch );
              stc( "               #w*       *  *****  \n\r", ch );
              stc( "               #C    ROUND THREE\n\r", ch );
              break;
 
      case 4: stc( "               #w*****        *  *  \n\r", ch );
              stc( "               #w*     *     *  *  \n\r", ch );
              stc( "               #w* * *      ******\n\r", ch );
              stc( "               #w*     *      *     \n\r", ch );
              stc( "               #w*       *   *    \n\r", ch );
              stc( "               #C    ROUND FOUR \n\r", ch );
              break;

      case 5: stc( "               #w*****          *****  \n\r", ch );
              stc( "               #w*     *       *    *  \n\r", ch );
              stc( "               #w* * *        ****\n\r", ch );
              stc( "               #w*     *        *     \n\r", ch );
              stc( "               #w*       *  ****    \n\r", ch );
              stc( "               #C    ROUND FIVE\n\r", ch );
              break;

      case 6: stc( "               #w*****          *****\n\r", ch );
              stc( "               #w*     *       *     \n\r", ch );
              stc( "               #w* * *        ****  \n\r", ch );
              stc( "               #w*     *     *   *     \n\r", ch );
              stc( "               #w*       *  *****    \n\r", ch );
              stc( "               #C    ROUND SIX  \n\r", ch );
              break;

      case 7: stc( "               #w*****      ******\n\r", ch );
              stc( "               #w*     *        *\n\r", ch );
              stc( "               #w* * *         *\n\r", ch );
              stc( "               #w*     *      *\n\r", ch );
              stc( "               #w*       *   *     \n\r", ch );
              stc( "               #C   FINAL ROUND\n\r", ch );
              break;
   }

   stc( "\n\r", ch );
}

void show_start( CHAR_DATA *ch )
{
   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      return;

   SET_BIT( ch->arena, AFF2_INTROC );

   stc( "#y******  ***  ********   **   **  ********  #C** **\n\r", ch );
   stc( "#y******  ***  ********   **   **  ********  #C** **\n\r", ch );
   stc( "#y**      ***  **         **   **     **     #C** **\n\r", ch );
   stc( "#y******  ***  ** ******  *******     **     #C** **\n\r", ch );
   stc( "#y******  ***  ** ******  *******     **     #C** **\n\r", ch );
   stc( "#y**      ***  **   **    **   **     **     #C** **\n\r", ch );
   stc( "#y**      ***  *******    **   **     **     \n\r", ch );
   stc( "#y**      ***  *******    **   **     **     #C** **\n\r\n\r", ch );
}

void show_power_meter( CHAR_DATA *ch )
{
   int team, count;
   char buf[MAX_STRING_LENGTH];
   char *name1, *name2;
   char line[MAX_STRING_LENGTH];
   int stars, cur_hit, max_hit;

   if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
      team = 0;
      else team = 1;

   if ( ch->challenged != NULL )
      {
        sprintf( line, "#o______________________/#yR" );

        sprintf( buf, "%d", team_list->round );
        strcat( line, buf );

        strcat( line, "#o\\______________________\n\r" );
        stc( line, ch );

        sprintf( line, "#o\\" );

        if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
           {
              cur_hit = ch->hit / 10;
              max_hit = UMAX( 1, ch->max_hit / 10 );

              if ( IS_NPC( ch ) )
                 name1 = str_dup( ch->short_descr );
                 else name1 = str_dup( ch->pcdata->switchname );

              if ( IS_NPC( ch ) )
                 name2 = str_dup( ch->challenged->short_descr );
                 else name2 = str_dup( ch->challenged->pcdata->switchname );
           }
           else {
                   cur_hit = ch->challenged->hit / 10;
                   max_hit = UMAX( 1, ch->challenged->max_hit / 10 );

                   if ( IS_NPC( ch ) )
                      name2 = str_dup( ch->short_descr );
                      else name2 = str_dup( ch->pcdata->switchname );

                   if ( IS_NPC( ch->challenged ) )
                      name1 = str_dup( ch->challenged->short_descr );
                      else name1 = str_dup( ch->challenged->pcdata->switchname );
                }

        stars = ( cur_hit * 20 ) / max_hit;

        for ( count = 0; count < 20 - stars; count++ )
        {
           strcat( line, " " );
        }

        strcat( line, "#R" );

        for ( count = 0; count < stars; count++ )
        {
           strcat( line, "+" );
        }

        strcat( line, "#o| " );

        if ( ch->arena_timer > 9 )
           strcat( line, "#w" );
           else strcat( line, "#P" );

        sprintf( buf, "%d  #o|#R", ch->arena_timer / 10 );
        strcat( line, buf );

        if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
           {
              cur_hit = ch->challenged->hit / 10;
              max_hit = ch->challenged->max_hit / 10;
           }
           else {
                   cur_hit = ch->hit / 10;
                   max_hit = ch->max_hit / 10;
                }

        stars = ( cur_hit * 20 ) / max_hit;

        for ( count = 0; count < stars; count++ )
        {
           strcat( line, "+" );
        }

        for ( count = 0; count < 20 - stars; count++ )
        {
           strcat( line, " " );
        }

        strcat( line, "#o/\n\r" );
        stc( line, ch );

        sprintf( line, " #w%s", name1 ); 

        for ( count = strlen(name1); count < 17; count++ )
        {
           strcat( line, " " );
        }

        strcat( line, "#C" );
        strcat( line, team_list->win_type[0] );
        strcat( line, "  " );

        if ( ch->arena_timer > 9 )
           strcat( line, "#w" );
           else strcat( line, "#P" );

        sprintf( buf, "%d #C", ch->arena_timer % 10 );
        strcat( line, buf );

        strcat( line, team_list->win_type[1] );

        for ( count = strlen(name2); count < 17; count++ )
        {
           strcat( line, " " );
        }

        sprintf( buf, "#w%s\n\r", name2 ); 
        strcat( line, buf );
        stc( line, ch );
        stc( "\n\r", ch );
      }

   if ( team_list->power_mode[team] != 0 )
      {
          switch( team_list->power_mode[team] )
          {
             default: team_list->power_mode[team] = 0;
                      break;
             case 1: stc( "#o<+< #RHyper Mode #o>+>#n ", ch );
                     return;

             case 2: stc( "#o<+< #yArmor Mode #o>+>#n ", ch );
                     return;

             case 3: stc( "#o<+< #PCounter Mode #o>+>#n ", ch );
                     return;

             case 4: stc( "#o<+< #CChain Mode #o>+>#n ", ch );
                     return;

             case 5: stc( "#o<+< #wInfinity Mode #o>+>#n ", ch );
                     return;
          }
      }

   sprintf( line, "#o<+< #wPow #C%d #o(#y", get_stock( ch ) / 100 );

   if ( get_stock( ch ) == 300 )
      strcat( line, " #wMAX " );
      else {
              for ( count = 0; count < 5; count++ )
              {
                 if ( ( get_stock( ch ) % 100 ) > count * 20 )
                    strcat( line, "*" );
                    else strcat( line, " " );
              }
           }

   if ( team_list->striker_points[team] == 5 )
      sprintf( buf, "#o) #PS: #CMax #o>+>#n ", team_list->striker_points[team] );
      else sprintf( buf, "#o) #PS: #C%d #o>+>#n ", team_list->striker_points[team] );

   strcat( line, buf );
   stc( line, ch );
}

void add_win( char letter, int team )
{
   int count = 0;

   if ( team == 1 )
      {
         while( count < 4 )
         {
            if ( team_list->win_type[team][count] == ' ' )
               {
                  team_list->win_type[team][count] = letter;
                  return;
               }

            count++;
         }
      }
      else {
              count = 3;
              while( count > -1 )
              {
                 if ( team_list->win_type[team][count] == ' ' )
                    {
                       team_list->win_type[team][count] = letter;
                       return;
                    }

                 count--;
              }
           }
}

void check_round( CHAR_DATA *winner, CHAR_DATA *loser )
{
   char *cname, *vname;
   DESCRIPTOR_DATA *d;
   int lteam, wteam;
   char buf[MAX_STRING_LENGTH];

   REMOVE_BIT(winner->arena, AFF2_WINNER);

   if ( IS_NPC( winner ) )
      cname = str_dup( winner->short_descr );
      else cname = str_dup( winner->pcdata->switchname );

   if ( IS_NPC( loser ) )
      vname = str_dup( loser->short_descr );
      else vname = str_dup( loser->pcdata->switchname );

   if ( IS_SET( winner->arena, AFF2_TEAM2 ) )
      {
        lteam = 0;
        wteam = 1;
      }
      else { 
              lteam = 1;
              wteam = 0;
           }

   if ( --team_list->team_numbers[lteam] == 0 )
      {
         bool bonus = FALSE;

         REMOVE_BIT(winner->arena, AFF2_CHALLENGED);
         REMOVE_BIT(winner->arena, AFF2_CHALLENGER);
         REMOVE_BIT(winner->arena, AFF2_INARENA);
         REMOVE_BIT(winner->arena, AFF2_INTROC);
         char_from_room( winner );

         if ( IS_NPC( winner ) )
            {
               char_to_room(winner, get_room_index(651));
               winner->cmbt[0] = 0;
            }
            else char_to_room(winner, get_room_index(631));

         sprintf( buf, "#y%s #Pteam #whas defeated #y%s #Pteam!#n", team_list->team_name[wteam], team_list->team_name[lteam] );
         do_tinfo( winner, loser, buf );

         if ( mybet == TRUE && number_percent() > 74 )
            {
               sprintf(buf,"#wThe #gpreceding #ywas #Ca #z#RB#wo#RN#wu#RS#n #cMaTcH#w!!!#n");
               do_tinfo( NULL, NULL, buf );
               bonus = TRUE;
            }

         for ( d = descriptor_list; d != NULL; d = d->next )
         {
            if ( d == NULL || d->character == NULL )
               continue;

            if ( d->character->team != 0 )
               {
                  if ( d->character->team == wteam + 1 )
                     {
                        stc("#gYou have #wwon#g your #y#zbet#n#g, #Cwell #wdone #ekid#g!#n\n\r",d->character);
                        d->character->gold = UMIN( 500000000, d->character->gold + ( d->character->pcdata->plr_wager * 2 ) );

                        if ( bonus == TRUE && d->character->pcdata->plr_wager >= 50000 )
                           give_random_prize(d->character);
                     }
                     else stc("#eToo #r#zbad#n#b chump#e, better #Cluck#e next time eh.#n\n\r",d->character);

                  d->character->pcdata->plr_wager = 0;
                  d->character->team = 0;
               }

            if ( d->character->pcdata->watching == loser )
               {
                  act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
                  d->character->pcdata->watching = NULL;
               }

            if ( d->character->pcdata->watching == winner && arena == FIGHT_CLEAR )
               {
                  act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
                  d->character->pcdata->watching = NULL;
               }
         }

         remove_team();
         arena = FIGHT_CLEAR;
         return;
      }
      else team_list->team_eliminated[lteam]++;

   if ( arena == FIGHT_START )
      {
         if ( IS_SET( winner->arena, AFF2_ASSIST ) )
            add_win( 'A', wteam );
            else if ( IS_SET( loser->arena, AFF2_GIVEUP ) )
                    add_win( 'G', wteam );
            else add_win( 'K', wteam );
      }
      else if ( arena == FIGHT_SPECIAL_KO )
              add_win( 'S', wteam );
      else if ( arena == FIGHT_BATTLEOVER )
              add_win( 'T', wteam );
      else if ( arena != FIGHT_CLEAR )
              add_win( 'K', wteam );

   if ( arena == FIGHT_START || arena == FIGHT_SPECIAL_KO )
      sprintf( buf, "#y%s #wdefeated #y%s #wby a #CK#RO#w!#n", cname, vname );
      else if ( arena == FIGHT_BATTLEOVER )
              sprintf( buf, "#y%s #wdefeated #y%s #wfrom a #cTime#eover#w...#n", cname, vname );

   do_tinfo( NULL, NULL, buf );

   if ( arena != FIGHT_CLEAR )
      arena = FIGHT_RECESS;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
       if ( d == NULL || d->character == NULL )
          continue;

       if ( d->character->pcdata->watching == loser )
          {
             act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
             d->character->pcdata->watching = NULL;
          }

       if ( d->character->pcdata->watching == winner && arena == FIGHT_CLEAR )
          {
             act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
             d->character->pcdata->watching = NULL;
          }
   }
}

void do_tabulate( CHAR_DATA *ch, char *argument )
{
   int team;

   if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
      {
         send_to_char("But you aren't in a team warrior.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      {
         send_to_char("Only active fighters can use this command warrior.\n\r", ch );
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

   if ( get_stock( ch ) < 100 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
        || !str_cmp( "A", ch->groove ) ) && ch->ctimer[9] > 32 ) )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
      team = 0;
      else team = 1;

   if ( team_list->striker_points[team] == 5 )
      {
         stc( "#wYour #Cstriker #ypoints #ware at max warrior.#n\n\r", ch );
         return;
      }

   ch->wait = 6;
   team_list->striker_points[team]++;

   if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove ) ) && ch->ctimer[9] < 100 )
      ch->ctimer[9] -= 33;
      else subtract_stock( ch, 100 );

   act( "#yYou #wfocus intently for a moment a flash of #ccyanish #ylight #wradiates off your body!#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #wfocuses intently for a moment a flash of #ccyanish #ylight #wradiates off $s body!#n", ch, NULL, NULL, TO_ROOM );
}

void do_dodge( CHAR_DATA *ch, char *argument )
{
   int cost = 33;

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

   if ( !str_cmp( "S", ch->groove ) )
      cost = 66;

   if ( get_stock( ch ) < 100 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
        || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > cost - 1 ) )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( IS_SET( ch->more3, MORE3_DODGE ) )
      {
         stc( "#wYou are already in a #cdefensive mode #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 6;

   if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
          || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 100 )
      ch->ctimer[9] -= cost;
      else ch->ctimer[9] -= 100;

   if ( ch->challenged != NULL )      
      act( "#yYou #wconcentrate softly in #Panticipation #wof #c$N#w's next attack...#n", ch, NULL, ch->challenged, TO_CHAR );
      else act( "#yYou #wconcentrate softly in #Panticipation #wof #cthe next attack...#n", ch, NULL, NULL, TO_CHAR );

   act( "#y$n #wseems to be concentrating in #Panticipation #wof #csomething#w...#n", ch, NULL, NULL, TO_ROOM );
   SET_BIT( ch->more3, MORE3_DODGE );
}

void do_retort( CHAR_DATA *ch, char *argument )
{
   int cost = 66;

   if ( !str_cmp( "S", ch->groove ) )
      cost = 100;

   if ( get_stock( ch ) < 200 && !( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) 
        || !str_cmp( "A", ch->groove ) || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] > cost - 1 ) )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->ctimer[15] > 0 )
      {
         send_to_char("#wThe church has banned the use of #CTimeStop #wfor this technique.#n\n\r",ch);
	   return;
      }

   if ( IS_SET( ch->more3, MORE3_RETORT ) )
      {
         stc( "#wYou are already in a #pretort mode #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->challenged != NULL )      
      act( "#yYou #wfocus intently in #Panticipation #wof #c$N#w's next attack...#n", ch, NULL, ch->challenged, TO_CHAR );
      else act( "#yYou #wfocus intently in #Panticipation #wof #cthe next attack...#n", ch, NULL, NULL, TO_CHAR );

   act( "#y$n #wseems to be focusing in #Panticipation #wof #csomething#w...#n", ch, NULL, NULL, TO_ROOM );

   ch->wait = 6;

   if ( !str_cmp( "S", ch->groove ) )
      ch->ctimer[9] -= cost;
      else if ( ( !str_cmp( "K", ch->groove ) || !str_cmp( "S", ch->groove ) || !str_cmp( "A", ch->groove )
                || !str_cmp( "P", ch->groove ) ) && ch->ctimer[9] < 100 )
              ch->ctimer[9] -= cost;
      else ch->ctimer[9] -= 200;

   SET_BIT( ch->more3, MORE3_RETORT );
}

void do_hyper_mode( CHAR_DATA *ch, char *argument )
{
   int team;

   if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
      {
         send_to_char("But you aren't in a team warrior.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      {
         send_to_char("Only active fighters can use this command warrior.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
      team = 0;
      else team = 1;

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( team_list->power_mode[team] > 0 )
      {
         stc( "#wYou are already in some #Rmode #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 6;
   subtract_stock( ch, 300 );
   ch->ctimer[15] = 0;
   team_list->power_mode[team] = 1;
   team_list->power_duration[team] = 10;

   act( "#RYou #Rplace your feet on the ground firmly as you clench your fist with rage.#n", ch, NULL, NULL, TO_CHAR );
   act( "#R$n #Rplaces $s feet on the ground firmly as $e clenches $s fist with rage.#n", ch, NULL, NULL, TO_ROOM );
   act( "#RA spiral of reddish energy radiates intently off your body!#n", ch, NULL, NULL, TO_CHAR );
   act( "#RA spiral of reddish energy radiates intently off $n as $e activates #wHyper Mode#R!#n", ch, NULL, NULL, TO_ROOM );
}

void do_armor_mode( CHAR_DATA *ch, char *argument )
{
   int team;

   if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
      {
         send_to_char("But you aren't in a team warrior.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      {
         send_to_char("Only active fighters can use this command warrior.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
      team = 0;
      else team = 1;

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( team_list->power_mode[team] > 0 )
      {
         stc( "#wYou are already in some #Rmode #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 6;
   subtract_stock( ch, 300 );
   ch->ctimer[15] = 0;
   team_list->power_mode[team] = 2;
   team_list->power_duration[team] = 10;

   act( "#yYou #yplace your feet on the ground firmly as you clench your fist with rage.#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #yplaces $s feet on the ground firmly as $e clenches $s fist with rage.#n", ch, NULL, NULL, TO_ROOM );
   act( "#yA spiral of yellowish energy radiates intently off your body!#n", ch, NULL, NULL, TO_CHAR );
   act( "#yA spiral of yellowish energy radiates intently off $n as $e activates #wArmor Mode#y!#n", ch, NULL, NULL, TO_ROOM );
}

void do_counter_mode( CHAR_DATA *ch, char *argument )
{
   int team;

   if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
      {
         send_to_char("But you aren't in a team warrior.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      {
         send_to_char("Only active fighters can use this command warrior.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
      team = 0;
      else team = 1;

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( team_list->power_mode[team] > 0 )
      {
         stc( "#wYou are already in some #Rmode #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 6;
   subtract_stock( ch, 300 );
   ch->ctimer[15] = 0;
   team_list->power_mode[team] = 3;
   team_list->power_duration[team] = 10;
   act( "#PYou #Pplace your feet on the ground firmly as you clench your fist with rage.#n", ch, NULL, NULL, TO_CHAR );
   act( "#P$n #Pplaces $s feet on the ground firmly as $e clenches $s fist with rage.#n", ch, NULL, NULL, TO_ROOM );
   act( "#PA spiral of yellowish energy radiates intently off your body!#n", ch, NULL, NULL, TO_CHAR );
   act( "#PA spiral of yellowish energy radiates intently off $n as $e activates #wCounter Mode#P!#n", ch, NULL, NULL, TO_ROOM );
}

void do_chain_mode( CHAR_DATA *ch, char *argument )
{
   int team;

   if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
      {
         send_to_char("But you aren't in a team warrior.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      {
         send_to_char("Only active fighters can use this command warrior.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
      team = 0;
      else team = 1;

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( team_list->team_numbers[team] > 2 )
      {
         send_to_char("You can only activate this mode with 2 or less active members remaining.\n\r", ch );
         return;
      }

   if ( team_list->power_mode[team] > 0 )
      {
         stc( "#wYou are already in some #Rmode #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 6;
   subtract_stock( ch, 300 );
   ch->ctimer[15] = 0;
   team_list->power_mode[team] = 4;
   team_list->power_duration[team] = 100;
   act( "#CYou #Cplace your feet on the ground firmly as you clench your fist with rage.#n", ch, NULL, NULL, TO_CHAR );
   act( "#C$n #Cplaces $s feet on the ground firmly as $e clenches $s fist with rage.#n", ch, NULL, NULL, TO_ROOM );
   act( "#CA spiral of yellowish energy radiates intently off your body!#n", ch, NULL, NULL, TO_CHAR );
   act( "#CA spiral of yellowish energy radiates intently off $n as $e activates #wChain Mode#C!#n", ch, NULL, NULL, TO_ROOM );
}

void do_infinity_mode( CHAR_DATA *ch, char *argument )
{
    int team, count;

    if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But you aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( !IS_SET( ch->arena, AFF2_INARENA ) )
       {
          send_to_char("Only active fighters can use this command warrior.\n\r", ch );
          return;
       }

    if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
       team = 0;
       else team = 1;

    if ( get_stock( ch ) < 300 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( team_list->team_numbers[team] > 1 )
       {
          send_to_char("You can only activate this mode when you are the last fighter.\n\r", ch );
          return;
       }

    if ( team_list->power_mode[team] > 0 )
       {
          stc( "#wYou are already in some #Rmode #wwarrior.#n\n\r", ch );
          return;
       }

    for ( count = 0; count < 18; count++ )
    {
       if ( !( IS_CLASS( ch, CLASS_ANGEL ) && count == 4 ) && count != 8 && count != 9 )
          ch->ctimer[count] = 0;
    }

    ch->tick_timer[TIMER_CAN_GUST] = 0;
    ch->tick_timer[TIMER_INFERNO] = 0;
    ch->tick_timer[TIMER_CAN_BREATHE_FROST] = 0;

    if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
       {
          ch->pcdata->powers[POWER_FREEZE] = 0;
          ch->pcdata->powers[POWER_WORD] = 0;
          ch->pcdata->powers[POWER_FLAME] = 0;
          ch->pcdata->powers[POWER_BLIND] = 0;
       }

    ch->lightning_timer = 0;
    ch->fear_timer = 0;
    ch->poison_timer = 0;

    ch->sam_chi[CURRENT] = ch->sam_chi[MAXIMUM];

    ch->focus[CURRENT] = 100;
    ch->chi[CURRENT] = 200;
    ch->dlife[CURRENT] = 100;
    ch->vlife[CURRENT] = 100;
    ch->force = 100;

    ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM];
    ch->quint[BODY] = ch->quint[AVATAR] = ch->spheres[MPRI];
    ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

    if ( IS_CLASS(ch, CLASS_DROW) )
       ch->cmbt[3] = 5;

    ch->saiyan_timer = 0;
    ch->control_timer = 0;
    ch->obeah_timer = 0;
    ch->vampirestuff = 0;
    ch->dragoon_timer = 0;
    ch->dragon_timer = 0;
    ch->divine_timer = 0;
    ch->monk_timer = 0;
    ch->throw_timer = 0;
    ch->defiance_timer = 0;
    ch->power_timer = 0;
    ch->use_timer = 0;
    ch->jedi_timer = 0;
    ch->ninja_timer = 0;
    ch->aragami_timer = 0;
    ch->guarding = 0;
    ch->mage_timer = 0;
    ch->plasma_timer = 0;
    ch->freeze_timer = 0;
    ch->stunned = 0;
    ch->achi = 0;

    ch->wait = 6;
    subtract_stock( ch, 300 );
    ch->ctimer[15] = 0;
    team_list->power_mode[team] = 5;
    team_list->power_duration[team] = 10;
    act( "#wYou #wplace your feet on the ground firmly as you clench your fist with rage.#n", ch, NULL, NULL, TO_CHAR );
    act( "#w$n #wplaces $s feet on the ground firmly as $e clenches $s fist with rage.#n", ch, NULL, NULL, TO_ROOM );
    act( "#wA spiral of bright energy radiates intently off your body!#n", ch, NULL, NULL, TO_CHAR );
    act( "#wA spiral of bright energy radiates intently off $n as $e activates #yInfinity Mode#w!#n", ch, NULL, NULL, TO_ROOM );
}

void do_strike( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But you aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( ch->strike_timer == 0 )
       {
          send_to_char("Only strikers can use this command warrior.\n\r", ch );
          return;
       }

    if ( arg[0] == '\0' || argument[0] == '\0' )
       {
          send_to_char( "#CStrike #wwhere warrior?#n\n\r", ch );
          return;
       }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }

    if ( victim == ch )
       {
          send_to_char( "Boggle... strike yourself? Ummm right.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->arena, AFF2_TEAM1 ) && !IS_SET( victim->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But they aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->arena, AFF2_INARENA ) )
       {
          send_to_char("But they aren't an active fighter.\n\r", ch );
          return;
       }

    sprintf( argument, "%s %s", argument, victim->name );

    location = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, victim->in_room );
    act( "#wGetting the #Pcue#w, #yyou #wquickly leap to #g$N #was you prepare your technique.\n\r#n", ch, NULL, victim, TO_CHAR );
    act( "#wGetting the #Pcue#w, #y$n #wquickly leaps to #gyou #was $e prepares $s technique.\n\r#n", ch, NULL, victim, TO_VICT );
    act( "#wGetting the #Pcue#w, #y$n #wquickly leaps to #g$N #was $e prepares $s technique.\n\r#n", ch, NULL, victim, TO_NOTVICT );

    ch->ctimer[9] = 300;
    interpret( ch, argument );
    ch->strike_timer = 0;

    act( "\n\r#wSnaring at your #gfoe #wonce more, #yyou #wsuddenly #Pleap #waway back to your waiting position.#n", ch, NULL, NULL, TO_CHAR );
    act( "\n\r#wSnaring at $s #gfoe #wonce more, #y$n #wsuddenlys #Pleap #waway back to $s waiting position.#n", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );

    stop_fighting( ch, TRUE );
    ch->fight_timer = 0;
    restore_char( ch );
    ch->caller = NULL;
}

void do_taunt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;

    if ( IS_NPC( ch ) )
       return;

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But you aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( ch->strike_timer == 0 )
       {
          send_to_char("Only strikers can use this command warrior.\n\r", ch );
          return;
       }

    if ( arg[0] == '\0' || argument[0] == '\0' )
       {
          send_to_char( "#CTaunt #wwhom warrior?#n\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }

    if ( victim == ch )
       {
          send_to_char( "Boggle... taunt yourself? Ummm right.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->arena, AFF2_TEAM1 ) && !IS_SET( victim->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But they aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->arena, AFF2_INARENA ) )
       {
          send_to_char("But they aren't an active fighter.\n\r", ch );
          return;
       }

    act( "You go all over $N calling $M vile names as $S face boils with anger!", ch, NULL, victim, TO_CHAR );
    act( "$n goes all over you calling you vile names as your face boils with anger!", ch, NULL, victim, TO_VICT );
    act( "$n goes all over $N calling $M vile names as $S face boils with anger!", ch, NULL, victim, TO_NOTVICT );
    act( "Just when $N thinks your are done, you suddenly give $M the finger.", ch, NULL, victim, TO_CHAR );
    act( "Just when you think $n is done, $e suddenly gives you the finger.", ch, NULL, victim, TO_VICT );
    act( "Just when $N thinks $n is done, $e suddenly gives $M the finger.", ch, NULL, victim, TO_NOTVICT );
    act( "You then turn around raising your arms in victory!", ch, NULL, NULL, TO_CHAR );
    act( "$n then turns around raising $s arms in victory!", ch, NULL, NULL, TO_ROOM );

    add_grove( victim, NULL, -66 );
}

void do_cheer( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;

    if ( IS_NPC( ch ) )
       return;

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->arena, AFF2_TEAM1 ) && !IS_SET( ch->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But you aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( ch->strike_timer == 0 )
       {
          send_to_char("Only strikers can use this command warrior.\n\r", ch );
          return;
       }

    if ( arg[0] == '\0' || argument[0] == '\0' )
       {
          send_to_char( "#CCheer #wwhom warrior?#n\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }

    if ( victim == ch )
       {
          send_to_char( "Boggle... cheer yourself? Ummm right.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->arena, AFF2_TEAM1 ) && !IS_SET( victim->arena, AFF2_TEAM2 ) )
       {
          send_to_char("But they aren't in a team warrior.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->arena, AFF2_INARENA ) )
       {
          send_to_char("But they aren't an active fighter.\n\r", ch );
          return;
       }

    act( "You go slapping $N behind the back as you smile at $M proudly.", ch, NULL, victim, TO_CHAR );
    act( "$n goes slapping you behind the back as $e smiles at you proudly.", ch, NULL, victim, TO_VICT );
    act( "$n goes slapping $N behind the back as $e smiles at $M proudly.", ch, NULL, victim, TO_NOTVICT );
    act( "You exclaim '#yBelieve in yourself man!#n' as $E is filled with pride!", ch, NULL, victim, TO_CHAR );
    act( "$n exclaims '#yBelieve in yourself man!#n' as you are filled with pride!", ch, NULL, victim, TO_VICT );
    act( "$n exclaims '#yBelieve in yourself man!#n' as $E is filled with pride!", ch, NULL, victim, TO_NOTVICT );

    add_grove( victim, NULL, -33 );
}

void show_special_ko( CHAR_DATA *ch )
{
    stc( "\n\r", ch );
    stc( "#w*****                                          ******\n\r", ch );
    stc( "#w*****                                        ***    ***\n\r", ch );
    stc( "#w*****                                       ***      ***\n\r", ch );
    stc( "#y*****                                      ***        ***\n\r", ch );
    stc( "#y*****           *****                     ***          ***\n\r", ch );
    stc( "#y*****          ********                  ***            ***\n\r", ch );
    stc( "#y*****         *****  ***                ***              ***\n\r", ch );
    stc( "#o*****        *****    ***              ***                ***\n\r", ch );
    stc( "#o*****       *****     ***             ***                  ***\n\r", ch );
    stc( "#o*******   ******                     ***                    ***\n\r", ch );
    stc( "#o******** ******                     ***                      ***\n\r", ch );
    stc( "#e**************                     ***                        ***\n\r", ch );
    stc( "#e******** ******                    ***                        ***\n\r", ch );
    stc( "#o*******   ******                    ***                      ***\n\r", ch );
    stc( "#o*****       ******                   ***                    ***\n\r", ch );
    stc( "#o*****        ******                   ***                  ***\n\r", ch );
    stc( "#o*****         ******                   ***                ***\n\r", ch );
    stc( "#y*****          ******                   ***              ***\n\r", ch );
    stc( "#y*****           ******                   ***            ***\n\r", ch );
    stc( "#y*****            ******                   ***          ***\n\r", ch );
    stc( "#y*****             ******                   ***        ***\n\r", ch );
    stc( "#w*****              ******      ****         ***      ***\n\r", ch );
    stc( "#w*****               ******     ****          ***    ***\n\r", ch );
    stc( "#w*****                ******    ****            ******\n\r", ch );
    stc( "\n\r", ch );
}

void show_ko( CHAR_DATA *ch )
{
    stc( "\n\r", ch );
    stc( "#o            **    ***      #P*********\n\r", ch );
    stc( "#o            **   ***       #P*********\n\r", ch );
    stc( "#o            **  **         #P**     **\n\r", ch );
    stc( "#o            *****          #P**     **\n\r", ch );
    stc( "#o            *****          #P**     **\n\r", ch );
    stc( "#o            **  **         #P**     **\n\r", ch );
    stc( "#o            **   ***   #y**  #P*********\n\r", ch );
    stc( "#o            **    ***  #y**  #P*********\n\r\n\r", ch );
}

void finish_round( CHAR_DATA *victim, CHAR_DATA *ch )
{
   stop_fighting( ch, TRUE );
   stop_fighting( victim, TRUE );
   ch->saiyanlevel = 0;
   victim->saiyanlevel = 0;

   if ( ch->strike_timer > 0 && ch->caller != NULL )
      {
         SET_BIT( ch->caller->arena, AFF2_ASSIST );
         show_ko( ch->caller );
         show_ko( victim );
         clean_arena( victim, ch->caller );
      }
      else if ( IS_SET( ch->arena, AFF2_DM ) )
      {
         SET_BIT( ch->arena, AFF2_WINNER );

         clear_affects( ch );
         clear_affects( victim );
         act( "#yYou #wscream in #Rpain #was the powerful attack causes your body to #Cfizz #win the air#e!!#n", victim, NULL, NULL, TO_CHAR );
         act( "#y$n #wscreams in #Rpain #was the powerful attack causes $s body to #Cfizz #win the air#e!!#n", victim, NULL, NULL, TO_ROOM );

         show_special_ko( ch );
         show_special_ko( victim );
         arena = FIGHT_SPECIAL_KO;
         arena_wait = 2;
      }
      else {
              show_ko( ch );
              show_ko( victim );
              clean_arena( victim, ch );
           }
}
