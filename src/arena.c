/* Hmm. An attempt..
     At a godwars battle arena
     Da dun dun dun da..
*/
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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "arena.h"

void  clean_arena	args( ( CHAR_DATA *loser, CHAR_DATA *winner ) );

bool is_inarena( CHAR_DATA *ch )
{
  if ( IS_SET(ch->arena, AFF2_INARENA) )
  {
    stc("You cannot do that while in the arena!\n\r",ch);
    return TRUE;
  }
  else
    return FALSE;
}

void do_arena_watch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC( ch ) )
       return;

    one_argument( argument, arg );

    if ( ch->pcdata->watching != NULL )
       {
	    act( "You stop watching $N.", ch, NULL, ch->pcdata->watching, TO_CHAR );
          ch->pcdata->watching = NULL;
  	    return;
       }

    if ( IS_SET(ch->arena, AFF2_INARENA) || ch->pcdata->team != 0 )
       {
          stc("You can't arena watch someone at this time.\n\r",ch);
       }

    if ( arg[0] == '\0' )
       {
	    send_to_char( "Watch whom?\n\r", ch );
	    return;
       }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }

    if ( victim == ch || ch->pcdata->watching != NULL )
       {
	    act( "You stop watching $N.", ch, NULL, ch->pcdata->watching, TO_CHAR );
          ch->pcdata->watching = NULL;
  	    return;
       }

    if ( !IS_NPC( victim ) && victim->pcdata->watching != NULL )
       {
	    stc( "#wThey #gare already arenawatching someone else... #eToo bad.#n", ch );
  	    return;
       }

    if ( !IS_SET(victim->arena, AFF2_INARENA) )
       {
          stc("They aren't in the arena.\n\r",ch);
          return;
       }

    act( "You start watching $N.", ch, NULL, victim, TO_CHAR );
    ch->pcdata->watching = victim;
}

void do_decline( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];

  if ( IS_NPC(ch) ) return;

  ch->arena_timer = 0;

  if ( ch->challenged == NULL )
  {
    stc("You have not been challenged.\n\r",ch);
    return;
  }

  if ( !IS_SET(ch->arena, AFF2_CHALLENGED) )
  {
    stc("You have not been challenged.\n\r",ch);
    return;
  }

  if ( arena == FIGHT_START )
  {
    stc("A fight is already in process.\n\r",ch);
    return;
  }

  victim = ch->challenged;

  act( "#wYou #yhave declined #c$N's #ychallenge...#n", ch, NULL, victim, TO_CHAR );
  sprintf(buf,"%s has declined %s's challenge.",ch->name, victim->name);
  do_ainfo(ch,buf);
  victim->arena_timer = 0;

  arena = FIGHT_CLEAR;
  undo_arena(ch);
}

void do_ainfo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( argument[0] == '\0' )
    {
        return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO) )
        {
            send_to_char( "#g[#y<#wArena#y>#g]#p ",d->character );
            send_to_char( argument, d->character );
            send_to_char( " #g[#y<#wArena#y>#g]#n\n\r",   d->character );
        }
    }
}

void do_challenge( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
 
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( (get_age(ch)-17) < 2 )
     {
       stc("But you are a newbie.\n\r",ch);
       return;
     }

  if ( arena != FIGHT_CLEAR )
  {
    stc("The arena is not ready for a fight at the moment.\n\r",ch);
    return;
  }

  if ( IS_SET( ch->arena, AFF2_COMPETITOR ) )
     {
        stc( "Kof fighters are banned from challenging people.\n\r", ch );
        return;
     }

  if ( ch->safe_timer > 0 )
  {
     stc( "Not with a Safe Timer.\n\r", ch );
     return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Who do you want to challenge?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    stc("They aren't even on the mud.\n\r",ch);
    return;
  }                         

  if ( IS_NPC(victim) )
  {
    stc("Challenge a mobile? You're pathetic.\n\r",ch);
    return;
  }

  if ( (get_age(victim)-17) < 2 )
     {
       stc("But they are a newbie.\n\r",ch);
       return;
     }
 
  if ( victim == ch )
  {
    stc("Oh, kill yourself. Fun.\n\r",ch);
    return;
  }

  if ( ch->hit < ch->max_hit )
  {
    stc("You must be fully healed to use the arena.\n\r",ch);
    return;
  }

  if ( victim->safe_timer > 0 )
  {
     stc( "But they have a Safe Timer.\n\r", ch );
     return;
  }

  if ( victim->hit < victim->max_hit )
  {
    stc("Your victim isn't fully healed.\n\r",ch);
    return;
  }

  if ( victim->level < 3 || ch->level < 3 )
  {
    stc("Only avatars may use the Arena.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->arena, AFF2_CHALLENGED)
       || IS_SET(ch->arena, AFF2_CHALLENGER) )
  {
    stc("Uh.  You're already involved in an arena fight.\n\r",ch);
    return;
  }

  if ( IS_SET(victim->arena, AFF2_CHALLENGED)
       || IS_SET(victim->arena, AFF2_CHALLENGER) )
  {
    stc("They are already involved in an arena fight.\n\r",ch);
    return;
  }

  if ( victim->fight_timer > 0 )
  {
    stc("They have a fight timer.\n\r",ch);
    return;
  }

  act( "#wYou #ychallenge #c$N #yto a death match!#n", ch, NULL, victim, TO_CHAR );

  ch->challenged = victim;
  victim->challenged = ch;
  ch->arena_timer = 14;
  victim->arena_timer = 14;
  sprintf(buf,"%s has challenged %s to a death match.",ch->name,victim->name);
  do_ainfo(ch,buf);
  SET_BIT(ch->arena, AFF2_CHALLENGER);
  SET_BIT(victim->arena, AFF2_CHALLENGED);
  arena = FIGHT_PRE;
}

void do_arenaagree( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victroom;
  char buf[MAX_STRING_LENGTH];

  if ( IS_NPC(ch) ) return;

  if ( ch->fight_timer > 0 )
  {
    stc("Not with a fight timer.\n\r",ch);
    return;
  }

  if ( arena == FIGHT_START )
  {
    stc("A fight is already in process.\n\r",ch);
    return;
  }

  if ( !IS_SET(ch->arena, AFF2_CHALLENGED) )
  {
    stc("You weren't even challenged.\n\r",ch);
    return;
  }

  if ( ch->challenged == NULL )
  {
    stc("Oddly enough, You have been challenged by someone, yet.. You haven't.\n\r",ch);
    REMOVE_BIT(ch->arena, AFF2_CHALLENGED);
    arena = FIGHT_CLEAR;

    sprintf(buf,"#wThe #PA#yren#Pa #wis #CClear#w.#n");
    do_ainfo(ch,buf);
    return;
  }

  if ( IS_SET( ch->arena, AFF2_COMPETITOR ) )
     {
        stc( "Kof fighters are banned from accepting arena challenges.\n\r", ch );
        return;
     }

  if ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
     {
        stc( "team members are banned from accepting arena challenges.\n\r", ch );
        return;
     }

  if ( arena == FIGHT_CLEAR )
     {
        undo_arena(ch);
        return;
     }

  if ( IS_SET( ch->challenged->arena, AFF2_COMPETITOR ) )
     {
        stc( "Kof fighters are banned from challenging people.\n\r", ch );
        return;
     }

  if ( IS_SET( ch->challenged->arena, AFF2_TEAM1 ) || IS_SET( ch->challenged->arena, AFF2_TEAM2 ) ) 
     {
        stc( "team members are banned from accepting arena challenges.\n\r", ch );
        return;
     }

  if ( ch->challenged->fight_timer > 0 )
     {
        stc("#wThey currently are in a fight #P#zOUTSIDE#n #wthe arena!#n\n\r",ch);
        return;
     }

  if ( ch->challenged->pcdata->capture_timer > 0 )
     {
        stc("#wThey currently are in jail awaiting #P#zexecution#n#w... Why not kill them?#n\n\r",ch);
        return;
     }

  victim = ch->challenged;
  arena = FIGHT_START;
  SET_BIT(ch->arena, AFF2_INARENA);
  SET_BIT(victim->arena, AFF2_INARENA);
  REMOVE_BIT(ch->arena, AFF2_CHALLENGER);
  REMOVE_BIT(victim->arena, AFF2_CHALLENGED);
  chroom = get_room_index(number_range(550,629));
  victroom = get_room_index(number_range(550,629));
  if ( victroom == chroom )
     chroom = get_room_index(number_range(550,629));
  char_from_room(ch);
  char_from_room(victim);
  char_to_room(ch, chroom);
  char_to_room(victim, victroom);
  stc("The fight has begun! Good luck!\n\r",ch);
  stc("The fight has begun! Good luck!\n\r",victim);
  do_look(ch,"");
  do_look(victim,"");

  act( "#wYou #yhave accepted #c$N's #ychallenge!#n", ch, NULL, victim, TO_CHAR );

  sprintf(buf,"#w%s #ghas accepted #y%s's #gchallenge.#n",ch->name, victim->name);
  do_ainfo(ch,buf);
  ch->arena_timer = 60;
  victim->arena_timer = 60;
}

void give_random_prize( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;
   int value;

   if ( IS_NPC( ch ) )
      return;

   switch( number_range(1,12) )
   {
      case 11: if ( ( pObjIndex = get_obj_index( 91 ) ) == NULL )
                 return;

               obj = create_object(pObjIndex, 60);
               obj_to_char(obj, ch);

               value = number_range( UMAX( 2, ch->tier - 2 ), ch->tier );
               obj->level = value;

               switch( value )
               {
                  default:
                  case 2: value = number_range( 1, 11 ); break;
                  case 3: value = number_range( 21, 36 ); break;
                  case 4: value = number_range( 41, 58 ); break;
                  case 5: value = number_range( 61, 80 ); break;
               }

               if ( value != 0 )
                  add_skill( obj, value );

               act("#wA #ysmall #gfairy #wgives you a Lionheart!#n",ch,NULL,NULL,TO_CHAR);
               act("#wA #ysmall #gfairy #wgives #P$n#w a Lionheart!#n",ch,NULL,NULL,TO_ROOM);
               break;
      case 3:
      case 7: if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
                 return;

              obj = create_object(pObjIndex, 60);
              obj_to_char(obj, ch);

              value = number_range( 1, 5 );
              obj->cost = 0;
              obj->value[0] = value;
              obj->item_type = ITEM_DTOKEN;

              if ( obj->questmaker != NULL )
                 free_string(obj->questmaker);

              obj->questmaker = str_dup(ch->name);
              free_string( obj->name );
              obj->name = str_dup( "TP token" );
              sprintf(buf,"a #C%d #PTP #etoken#n",value);
              free_string( obj->short_descr );
              obj->short_descr = str_dup( buf );
              sprintf(buf,"A %d TP token lies on the floor.",value);
              free_string( obj->description );
              obj->description = str_dup( buf );

		  if ( obj->questowner != NULL )
                 free_string( obj->questowner );

              obj->questowner = str_dup( ch->pcdata->switchname );

              act("#wA #ysmall #gfairy #wgives you a TP Token!#n",ch,NULL,NULL,TO_CHAR);
              act("#wA #ysmall #gfairy #wgives #P$n#w a TP Token!#n",ch,NULL,NULL,TO_ROOM);
              break;
      case 12:
      case 5: if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
                 return;

              obj = create_object(pObjIndex, 60);
              obj_to_char(obj, ch);

              value = number_range( 1, 5 );
              obj->cost = 0;
              obj->value[0] = value;
              obj->item_type = ITEM_PKTOKEN;

              if ( obj->questmaker != NULL )
                 free_string(obj->questmaker);

              obj->questmaker = str_dup(ch->name);
              free_string( obj->name );
              obj->name = str_dup( "PK token" );
              sprintf(buf,"a #P%d #CPK #ytoken#n",value);
              free_string( obj->short_descr );
              obj->short_descr = str_dup( buf );
              sprintf(buf,"A %d PK token lies on the floor.",value);
              free_string( obj->description );
              obj->description = str_dup( buf );
			  if ( obj->questowner != NULL )
                 free_string( obj->questowner );

              obj->questowner = str_dup( ch->pcdata->switchname );


              act("#wA #ysmall #gfairy #wgives you a PK Token!#n",ch,NULL,NULL,TO_CHAR);
              act("#wA #ysmall #gfairy #wgives #P$n#w a PK Token!#n",ch,NULL,NULL,TO_ROOM);
              break;
      case 10:
      case 6: find_item( ch, 0 );
              break;

      case 9: act("#wA #ysmall #gfairy #wgives you a #P#zFREE#n #wskillbuy!#n",ch,NULL,NULL,TO_CHAR);
              act("#wA #ysmall #gfairy #wgives #P$n#w a #P#zFREE#n #wskillbuy!#n",ch,NULL,NULL,TO_ROOM);
              ch->cmbt[4]++;
              break;
      case 2:
      case 8: act("#wA #ysmall #gfairy #wgives you #P#zFREE#n #wtransmigration levels!#n",ch,NULL,NULL,TO_CHAR);
              act("#wA #ysmall #gfairy #wgives #P$n#w #P#zFREE#n #wtransmigration levels!#n",ch,NULL,NULL,TO_ROOM);
              add_transmigration( ch, number_range( 25000, 50000 ) );
              break;
      default:
      case 1:
      case 4: if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
                 return;

              obj = create_object(pObjIndex, 60);
              obj_to_char(obj, ch);

              value = number_range( 250000, 500000 );
              obj->cost = value;
              obj->value[0] = value;
              obj->item_type = ITEM_QUEST;

              if ( obj->questmaker != NULL )
                 free_string(obj->questmaker);

              obj->questmaker = str_dup(ch->name);
              free_string( obj->name );
              obj->name = str_dup( "coin token" );
              sprintf(buf,"a #w%d #ycoin #etoken#n",value);
              free_string( obj->short_descr );
              obj->short_descr = str_dup( buf );
              sprintf(buf,"A %d coin token lies on the floor.",value);
              free_string( obj->description );
              obj->description = str_dup( buf );

              if ( obj->questowner != NULL )
                 free_string( obj->questowner );

              obj->questowner = str_dup( ch->pcdata->switchname );

              act("#wA #ysmall #gfairy #wgives you a Coin Token!#n",ch,NULL,NULL,TO_CHAR);
              act("#wA #ysmall #gfairy #wgives #P$n#w a Coin Token!#n",ch,NULL,NULL,TO_ROOM);
              break;
   }
}

void clean_arena( CHAR_DATA *loser, CHAR_DATA *winner )
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];
  bool bonus = FALSE;

  if ( arena == FIGHT_ANNOUNCE || arena == FIGHT_BATTLEOVER )
     {
        if ( IS_CLASS( loser, CLASS_MONK ) )
           {
              act( "\n\rYou fold your arms silently as you stare blankly into the distance...", loser, NULL, NULL, TO_CHAR );
              act( "\n\r$n folds $s arms silently as $e stares blankly into the distance...", loser, NULL, NULL, TO_ROOM );
           }
           else if ( IS_CLASS( loser, CLASS_PALADIN ) || IS_CLASS( loser, CLASS_SAMURAI ) )
           {
              act( "\n\rYou fall to your knees in shame, disgusted at the way you lose this battle.", loser, NULL, NULL, TO_CHAR );
              act( "\n\r$n falls to $s knees in shame, disgusted at the way $e loses this battle.", loser, NULL, NULL, TO_ROOM );
           }
           else {
                   act( "\n\rYou SmAsH your fist into the ground with fury as you shake your head sadly.", loser, NULL, NULL, TO_CHAR );
                   act( "\n\r$n SmAsHeS $s fist into the ground with fury as $e shakes $s head sadly.", loser, NULL, NULL, TO_ROOM );
                }
      }
      else if ( arena == FIGHT_SPECIAL_KO )
              {
                 act( "#wAs the #ysparks #wof #Penergy #Rrips #waway from #Cyour #wbody, you are suddenly slammed onto the floor.\n\r#n", loser, NULL, NULL, TO_CHAR );
                 act( "#wAs the #ysparks #wof #Penergy #Rrips #waway from #C$n#C's #wbody, $e is suddenly slammed onto the floor.\n\r#n", loser, NULL, NULL, TO_ROOM );
              }

  if ( IS_SET(winner->arena, AFF2_TEAM1 ) || IS_SET(winner->arena, AFF2_TEAM2 ) 
       || ( winner->round > 0 && winner->round < 5 ) )
     show_victory_pose( winner );

  stop_fighting( winner, TRUE );
  stop_fighting( loser, TRUE );

  if ( loser->retaliate != NULL )
     {
        loser->retaliate->retaliate = NULL;
        loser->retaliate = NULL;
     }

  REMOVE_BIT( loser->more3, MORE3_RETORT );
  REMOVE_BIT( loser->more3, MORE3_DODGE );
  REMOVE_BIT( winner->more3, MORE3_RETORT );
  REMOVE_BIT( winner->more3, MORE3_DODGE );
  REMOVE_BIT( loser->arena, AFF2_CHAIN );
  REMOVE_BIT( winner->arena, AFF2_CHAIN );

  REMOVE_BIT(loser->arena, AFF2_CHALLENGED);
  REMOVE_BIT(loser->arena, AFF2_CHALLENGER);
  REMOVE_BIT(loser->arena, AFF2_INARENA);
  REMOVE_BIT(loser->arena, AFF2_COMPETITOR);
  REMOVE_BIT(loser->arena, AFF2_INTROC);
  loser->challenged = NULL;
  restore_char(loser);
  loser->fight_timer = 0;
  loser->round = 0;
  loser->arena_timer = 0;
  loser->attacker = NULL;
  char_from_room(loser);

  if ( IS_NPC( loser ) )
     {
        char_to_room(loser, get_room_index(651));
        loser->cmbt[0] = 0;
     }
     else char_to_room(loser, get_room_index(630));

  do_look(loser,"");
  loser->safe_timer = 1;

  winner->challenged = NULL;
  winner->arena_timer = 0;
  winner->fight_timer = 0;
  winner->attacker = NULL;

  if ( IS_SET(winner->arena, AFF2_TEAM1 ) || IS_SET(winner->arena, AFF2_TEAM2 ) )
     {
        check_round( winner, loser );
        return;
     }

  REMOVE_BIT(winner->arena, AFF2_CHALLENGED);
  REMOVE_BIT(winner->arena, AFF2_CHALLENGER);
  REMOVE_BIT(winner->arena, AFF2_INARENA   );
  restore_char(winner);

  char_from_room(winner);

  if ( IS_NPC( winner ) )
     {
        char_to_room(winner, get_room_index(651));
        winner->cmbt[0] = 0;
     }
     else char_to_room(winner, get_room_index(631));
  
  do_look(winner,"");

  winner->safe_timer = 1;

  if ( winner->round > 0 )
     {
       winner->round++;

       if ( winner->round >= 5 )
          {
             for ( d = descriptor_list; d != NULL; d = d->next )
             {
                if ( d == NULL || d->character == NULL )
                   continue;

                if ( d->character->pcdata->watching != NULL )
                   {
                      act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
                      d->character->pcdata->watching = NULL;
                   }
             }

             show_winner( winner );

             if ( mybet == TRUE )
                find_item( winner, number_range( 1, 2 ) );
          }
          else {
                  if ( !IS_NPC( winner ) )
                     {
                        free_string( winner->short_descr );
                        winner->short_descr = str_dup( winner->name );
                     }

                  if ( !IS_NPC( loser ) )
                     {
                        free_string( loser->short_descr );
                        loser->short_descr = str_dup( loser->name );
                     }

                  sprintf(buf,"#w%s #ghas #Rdefeated #C%s #gand #yadvanced #gto the next #o#zRoUnD#n#g!#n", winner->short_descr, loser->short_descr );
                  do_tinfo( winner, loser, buf );
                  arena = FIGHT_INITWAIT;

                  if ( mybet == TRUE )
                     {
                        if ( number_percent() > 59 )
                           give_random_prize(winner);
                           else {
                                   act("#gYou won #w100000 #ygold#g pieces!!!#n",winner,NULL,NULL,TO_CHAR);
                                   winner->gold = UMIN( 500000000, winner->gold + 100000 );
                                }
                     }
               }

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

          if ( d->character->gladiator == NULL )
             continue;

          if ( d->character->gladiator == winner )
             {
               stc("#gYou have #wwon#g your #y#zbet#n#g, #Cwell #wdone #ekid#g!#n\n\r",d->character);
               d->character->gold = UMIN( 500000000, d->character->gold + ( d->character->pcdata->plr_wager * 2 ) );

               if ( bonus == TRUE && d->character->pcdata->plr_wager >= 50000 )
                  give_random_prize(d->character);
             }
             else stc("#eToo #r#zbad#n#b chump#e, better #Cluck#e next time eh.#n\n\r",d->character);

          d->character->pcdata->plr_wager = 0;
          d->character->gladiator = NULL;
       }
     }
     else {
             sprintf(buf,"%s has defeated %s!",winner->name, loser->name);
             do_ainfo(winner,buf);
             winner->pcdata->awins++;
             loser->pcdata->alosses++;
             arena = FIGHT_CLEAR;

             for ( d = descriptor_list; d != NULL; d = d->next )
             {
                if ( d == NULL || d->character == NULL )
                   continue;

                if ( d->character->pcdata->watching != NULL )
                   {
                      act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
                      d->character->pcdata->watching = NULL;
                   }

                if ( d->character->gladiator == NULL )
                   continue;

                d->character->gladiator = NULL;
             }
          }
}

void still_survival( CHAR_DATA *victim )
{
   int count = 0;
   CHAR_DATA *gch, *winner;
   char buf [MAX_INPUT_LENGTH];

   victim->pcdata->team = 0;

   sprintf( buf, "#y%s #ghas #wbeen #p#zElImINaTeD#n#y!!!#n", victim->pcdata->switchname );
   do_info( victim, buf );

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
      if ( IS_NPC(gch) )
         continue;

      if ( gch->pcdata->team < 9 )
          continue;
 
      winner = gch;
      count++;
   }

   if ( count != 1 || winner == NULL )
      return;

   count = winner->level;
   winner->level = 12;
   winner->gold = UMIN( 500000000, winner->gold + 1000000 );
   winner->practice += number_range( 1, 3 );

   if ( !( winner->generation < 3 && winner->tier == 5 ) )
      {
         winner->generation -= 3;

         if ( winner->generation < 1 && winner->tier == 5 )
            winner->generation = 1;
            else if ( winner->generation < 1 )
            {
               stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",winner);
               winner->generation = 5 + winner->generation;
               winner->tier++;
               winner->max_hit += 1000000;
               winner->max_mana += 1000000;
               winner->max_move += 1000000;
            }
      }

   sprintf(buf,"#y%s #ghas #wsurvived the #p#zSlAuGhTeR#n#y!!!#n",winner->pcdata->switchname);
   do_info(winner,buf);
   stc("By some miracle... you had won the battle.\n\r",winner);
   do_ftag(winner,"reset");
   winner->level = count;
   return;
}

void still_team( CHAR_DATA *victim )
{
   int count = 0, team[8];
   CHAR_DATA *gch, *winner;
   char buf [MAX_INPUT_LENGTH];

   for ( count = 0; count < 8; count++ )
       team[count] = 0;

   count = 0;

   sprintf( buf, "#y%s #ghas #wbeen #p#zElImINaTeD#n#y!!!#n", victim->pcdata->switchname );
   do_info( victim, buf );

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
      if ( IS_NPC(gch) )
         continue;

      if ( gch->pcdata->team == 0 )
         continue;

      if ( gch->pcdata->team != victim->pcdata->team )
         winner = gch;

      if ( gch->pcdata->team == 1 )
         {
            team[0]++;
 
            if ( team[0] == 1 )
               count++;
 
            continue;
         }

      if ( gch->pcdata->team == 2 )
         {
            team[1]++;

            if ( team[1] == 1 )
               count++;

            continue;
         }

      if ( gch->pcdata->team == 3 )
         {
            team[2]++;

            if ( team[2] == 1 )
               count++;
 
            continue;
         }

      if ( gch->pcdata->team == 4 )
         {
            team[3]++;
 
            if ( team[3] == 1 )
               count++;

            continue;
          }

       if ( gch->pcdata->team == 5 )
          {
             team[4]++;

             if ( team[4] == 1 )
                count++;

             continue;
          }

       if ( gch->pcdata->team == 6 )
          {
             team[5]++;

             if ( team[5] == 1 )
                count++;

             continue;
           }

       if ( gch->pcdata->team == 7 )
          {
             team[6]++;

             if ( team[6] == 1 )
                count++;

             continue;
          }

       if ( gch->pcdata->team == 8 )
          {
             team[7]++;

             if ( team[7] == 1 )
                count++;

             continue;
          }
   }

   if ( count == 3 && team[victim->pcdata->team -1] == 1 )
      {
          third_team = victim->pcdata->team;
          victim->pcdata->team = 0;
          return;
      }
      else if ( count == 2 && team[victim->pcdata->team -1] == 1 )
              {
                 second_team = victim->pcdata->team;
                 first_team = winner->pcdata->team;
                 stc("#cYour team had won the SlAuGhTeR!!!#n\n\r",winner);
                 sprintf(buf,"#yThe PK #gQuest #phas #CEnDeD!!!#n" );
                 do_info(winner,buf);
                 count = winner->level;
                 winner->level = 12;
                 do_ftag(winner,"reset");
                 winner->level = count;
                 victim->pcdata->team = 0;
                 return;
              }
}
  
void undo_arena( CHAR_DATA *ch )
{
  CHAR_DATA *victim;
  char name[MAX_STRING_LENGTH]; 

  if ( ch->attacker != NULL )
     {
        ch->attacker->attacker = NULL;
        ch->attacker = NULL;
     }
 
  if ( ch->challenged != NULL )
  {
    victim = ch->challenged;

    if ( victim->in_room->vnum >= 550 && victim->in_room->vnum < 630 )
       {
          int chit, cmhit, vhit, vmhit;

          if ( ch->max_hit > 999999 )
             {
                chit = ch->hit;
                cmhit = ch->max_hit / 100;
             }
             else {
                     chit = ch->hit * 100;
                     cmhit = ch->max_hit;
                  }

          if ( victim->max_hit > 999999 )
             {
                vhit = victim->hit;
                vmhit = victim->max_hit / 100;
             }
             else {
                     vhit = victim->hit * 100;
                     vmhit = victim->max_hit;
                  }

          if ( IS_SET( ch->arena, AFF2_WINNER ) )
             clean_arena( victim, ch );
             else if ( IS_SET( victim->arena, AFF2_WINNER ) )
                     clean_arena( ch, victim );
                     else if ( vhit / vmhit > chit / cmhit )
                             clean_arena( ch, victim );
                             else clean_arena( victim, ch );
       }
       else {
               REMOVE_BIT(ch->arena, AFF2_CHALLENGED);
               REMOVE_BIT(ch->arena, AFF2_CHALLENGER);
               REMOVE_BIT(ch->arena, AFF2_INARENA);
               REMOVE_BIT(victim->arena, AFF2_CHALLENGED);
               REMOVE_BIT(victim->arena, AFF2_CHALLENGER);
               REMOVE_BIT(victim->arena, AFF2_INARENA);
               ch->challenged = NULL;    
               victim->challenged = NULL;
            }

    if ( arena == FIGHT_BATTLEOVER || arena == FIGHT_SPECIAL_KO || arena == FIGHT_RECESS )
       return;

    if ( arena != FIGHT_CLEAR && arena != FIGHT_INITWAIT && arena != FIGHT_BATTLEOVER
        && arena != FIGHT_TIMEOVER && arena != FIGHT_SPECIAL_KO && arena != FIGHT_RECESS )
       {
          char buf [MAX_INPUT_LENGTH];

          arena = FIGHT_CLEAR;
          sprintf(buf,"#wThe #PA#yren#Pa #wis no longer #Cbusy#w!#n");
          do_ainfo(victim,buf);
          stc("You waited to long, arena status cleared!\n\r", victim);
       }
  }
  else
  {
    if ( IS_SET(ch->arena, AFF2_CHALLENGED) )
      REMOVE_BIT(ch->arena, AFF2_CHALLENGED);
    if ( IS_SET(ch->arena, AFF2_CHALLENGER) )
      REMOVE_BIT(ch->arena, AFF2_CHALLENGER);
    if ( IS_SET(ch->arena, AFF2_INARENA) )
      REMOVE_BIT(ch->arena, AFF2_INARENA);

     if ( !IS_NPC( ch ) )
        {
           if ( ch->pcdata->team == 9 )
              still_survival( ch );
              else if ( ch->pcdata->team > 0 )
                      still_team( ch );
        }
  }

  if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 )
  {
    char_from_room(ch);
    char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
  }
}

void do_bet(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH]; 
  char buf[MAX_STRING_LENGTH]; 
  CHAR_DATA *fighter; 
  int wager; 

  argument = one_argument(argument, arg); 

  if ( IS_IMMORTAL( ch ) )
     {
        if ( !str_cmp( arg, "on" ) )
           {
              if ( mybet == TRUE )
                 {
                    stc( "#yBets #ware already allowed in the tournaments and team battles.#n\n\r", ch );
                    return;
                 }

              mybet = TRUE;
              do_info( NULL, "#PPlayers #wcan now #ybet #win #Ctournaments #wand #gteam battles#w!!!#n" );
              return;
           }
           else if ( !str_cmp( arg, "off" ) )
           {
              if ( mybet == FALSE )
                 {
                    stc( "#yBets #ware already not allowed in the tournaments and team battles.#n\n\r", ch );
                    return;
                 }

              mybet = FALSE;
              do_info( NULL, "#ePlayers #wcan no longer #ybet #win #Ctournaments #wand #gteam battles#w!!!#n" );
              return;
           }
           else if ( argument[0] == '\0' )
                   {
                      send_to_char("#wSyntax: #yBET #Con#w/#eoff#n\n\r",ch); 
                      return;
                   }
     }

  if ( mybet == FALSE )
     {
        send_to_char("Betting has been disabled for now.\n\r",ch); 
        return;
     }

  if ( betting_time == 0 )
     {
        send_to_char("The time allocated for betting in this match is not now.\n\r",ch); 
        return; 
     }

  if ( argument[0] == '\0' || !is_number(arg) )
     {
        send_to_char("Syntax: BET [amount] [player]\n\r",ch); 
        return;
     }

  if ( ch->gladiator != NULL || ch->team != 0 )
     {
        send_to_char("You have already placed a bet on this fight.\n\r",ch);
        return;
     }

  if ( IS_SET(ch->act,PLR_CHALLENGER) || IS_SET(ch->act,PLR_CHALLENGED) || IS_SET(ch->arena, AFF2_INARENA)
       || IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) )
     {
        send_to_char("You can't bet on this battle.\n\r",ch); 
        return; 
     }

  fighter = get_char_world(ch,argument);

  if ( fighter == NULL )
     {
        send_to_char("That player is not logged in.\n\r",ch);
        return;
     }

  if ( !IS_SET(fighter->arena, AFF2_INARENA) )
     {
        send_to_char("That player is not in the arena.\n\r",ch); 
        return; 
     }

  if ( fighter->round == 0 )

  if ( !IS_SET( fighter->arena, AFF2_TEAM1 ) && !IS_SET( fighter->arena, AFF2_TEAM2 )
       && !IS_SET( fighter->arena, AFF2_COMPETITOR ) )
     {
        send_to_char("You can only bet on tournament and team participants.\n\r",ch); 
        return; 
     }

  if ( !str_prefix("-",arg) )
     {
        send_to_char("Error: Invalid argument!\n\r",ch); 
        return; 
     }

  wager = atoi(arg);

  if ( wager > 100000 || wager < 1 )
     {
        send_to_char("Wager range is between #w1#n and #w100,000 #ygold pieces#n.\n\r",ch);
        return; 
     }

  if ( wager > ch->gold )
     {
        send_to_char("You don't have that many #ygold pieces#n to wager!\n\r",ch); 
        return; 
     }

  ch->pcdata->plr_wager = wager;
  ch->gold -= ch->pcdata->plr_wager;

  if ( IS_SET( fighter->arena, AFF2_TEAM1 ) || IS_SET( fighter->arena, AFF2_TEAM2 ) )
     {
        if ( IS_SET( fighter->arena, AFF2_TEAM1 ) )
           {
              ch->team = 1;
              sprintf( buf,"You have placed a #w%d #ygold piece#n wager on #P%s#n.\n\r", wager, team_list->team_name[0] );
           }
           else {
                   ch->team = 2;
                   sprintf( buf,"You have placed a #w%d #ygold piece#n wager on #P%s#n.\n\r", wager, team_list->team_name[1] );
                }
     }
     else {
             ch->gladiator = fighter;

             if ( !IS_NPC( ch ) )
                sprintf(buf,"You have placed a #w%d #ygold piece#n wager on #P%s#n.\n\r", wager,fighter->name);
                else sprintf(buf,"You have placed a #w%d #ygold piece#n wager on #P%s#n.\n\r", wager,fighter->short_descr);
          }

  send_to_char(buf,ch);
}

void do_no_watch( CHAR_DATA *ch, char *argument ) 
{
   if ( IS_NPC(ch) )
      return;

   if ( IS_SET(ch->newbits, NEW_NOWATCH) )
      {
         REMOVE_BIT(ch->newbits, NEW_NOWATCH);
         send_to_char("People can once again watch you in arena matches.\n\r", ch );
         return;
      }

   SET_BIT(ch->newbits, NEW_NOWATCH);
   send_to_char("People can no longer watch you in arena matches.\n\r", ch );
}
