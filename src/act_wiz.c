#define crypt(s1, s2)	(s1)

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
#include <unistd.h>



/*
 * Local functions.
 */
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );
void			call_all	args( ( CHAR_DATA *ch ) );
char			buf		[MAX_STRING_LENGTH];
char			arg		[MAX_INPUT_LENGTH];
bool     write_to_descriptor     args( ( DESCRIPTOR_DATA *d, char *txt, int length ) );
void init_descriptor args ((DESCRIPTOR_DATA *dnew, int desc));

void give_align( CHAR_DATA *ch, int number )
{
   int old_algn = ch->alignment;

   ch->alignment = URANGE( -1000, ch->alignment + number, 1000 );

   if ( ch->alignment == 1000 && old_algn != 1000 )
      stc( "#cWell done... #yYou #care now #wangelic#c!#n", ch );
      else if ( ch->alignment == -1000 && old_algn != -1000 )
              stc( "#pWell done... #RYou #pare now #esatanic#p!#n", ch );

   if ( ch->class_2 != 0 )
      return;

   if ( IS_CLASS( ch, CLASS_DEMON ) && ch->alignment > -350 )
      {
         int fgt = ch->fight_timer;

         stc( "#eThe #Pdarkness #eROARS at you in anger as you are suddenly #w#zstripped#n #eof your powers!#n", ch );
         ch->fight_timer = 0;
         do_detox( ch, "" );
         ch->fight_timer = fgt;
         ch->safe_timer = ch->login_timer = 0;
      }
      else if ( IS_CLASS( ch, CLASS_ANGEL ) && ch->alignment < 350 )
      {
         int fgt = ch->fight_timer;

         stc( "#wThe #ylight #wsadly leaves your body respecting #eyour #wdecision to the fullest...#n", ch );
         ch->fight_timer = 0;
         do_detox( ch, "" );
         ch->fight_timer = fgt;
         ch->safe_timer = ch->login_timer = 0;
      }
}

void do_ftag( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim, *gch;

    argument = one_argument( argument, arg1 );

    if ( arena == FIGHT_AUTO )
       {
          if ( IS_NPC( ch ) )
             return;

          if ( ( get_age(ch) - 17 ) < 2 )
             {
                send_to_char( "But you are a newbie.\n\r", ch );
                return;
             }

          if ( IS_SET( ch->extra, EXTRA_HUNT ) )
             {
                send_to_char( "Not while you are in the hunt.\n\r", ch );
                return;
             }

          if ( ch->pcdata->team > 0 )
             {
                send_to_char( "You have already joined the slaughter... Good Luck.\n\r", ch );
                return;
             }

          if ( ch->fight_timer > 0 )  
             {
                send_to_char("Not until your fight timer expires.\n\r", ch );
                return;
             }
	
          stop_fighting( ch, TRUE );
          restore_char( ch );
          ch->pcdata->team = 9;
          stc("You have joined the #rR#yA#wI#eN#bB#pO#CW #yteam#n!!!\n\r",ch);
          sprintf( arg1, "#w%s #ghave #o#zventured#n #ginto the #Rcollesium #gto test their #Rmight#g!#n", ch->pcdata->switchname );
          do_ainfo( ch, arg1 );

          char_from_room(ch);
          char_to_room( ch, get_room_index( number_range(550,629) ) );

          return;
       }

    if ( ch->level < 7 )
       {
          stc( "Huh?\n\r", ch );
          return;
       }   

    if ( arena != FIGHT_CLEAR && arena != FIGHT_QUEST && arena != FIGHT_AUTO
         && arena != FIGHT_ARENA )
    {
       stc("The arena is not ready for a fight at the moment.\n\r",ch);
       return;
    }

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax:  ftag reset\n\r", ch );
	send_to_char( "         ftag start\n\r", ch );
	send_to_char( "         ftag survial (makes a free for all quest)\n\r", ch );
	send_to_char( "         ftag winners\n\r", ch );
	send_to_char( "         ftag scavenger\n\r", ch );
	send_to_char( "         ftag dungeon\n\r", ch );
	send_to_char( "         ftag arena (opens/closes the arena for one on one)\n\r", ch );
	send_to_char( "         ftag auto (splits everyone in room onto teams)\n\r", ch );
	send_to_char( "         ftag red/blue/yellow/grey/purple/white/green/cyan <player>\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "reset" ) )
    {
      for ( gch = char_list; gch != NULL; gch = gch->next )
	{
        if ( IS_NPC(gch) )
           continue;

        if ( gch->pcdata->team == 0 )
           continue;

        if ( gch->pcdata->team == first_team )
           {
               gch->gold = UMIN( 500000000, gch->gold + number_range( 5000, 10000 ) );
               gch->race = gch->race + number_range( 1, 2 );
               send_to_char( "#sCong#srat#sulat#sions#w! Your team had #Rdestroyed #wall #cchallengers#w!\n\r", gch );
           }

        gch->pcdata->team = 0;
        gch->fight_timer = 0;
        gch->safe_timer = 5;
        gch->hit = gch->max_hit;
        gch->mana = gch->max_mana;
        gch->move = gch->max_move;
        send_to_char( "#e<<<<<#wThe #PQuest #wfor #gSurvival #whas been terminated!#e>>>>>#n\n\r", gch );
	}

      first_team = -1;
	send_to_char( "All players reset.\n\r", ch );

      arena = FIGHT_CLEAR;
	return;
    }

    if ( !str_cmp( arg1, "scavenger" ) )
    {
      do_sinfo( NULL, "#w50 #yitems #ghas been #Pscattered #gthroughout the #Prealm#g! Use #yScavenger #gto Join!!!#n" );
      start_scavenger();
      return;
    }

    if ( !str_cmp( arg1, "dungeon" ) )
    {
      if ( is_dungeon == -1 )
         create_dungeon();
         else {
                 do_info( NULL, "#eThe #CDungeon #eof #RDeath #eis now closed...#n" );
                 do_info( NULL, "#eFor #wall #ewho had tried their #R#zbest#n#e... Await for the #creturn#e...#n" );
                 clear_dungeon();
              }

      return;
    }

    if ( !str_cmp( arg1, "start" ) )
    {
      do_info( ch, "#wThe #CQ#yu#ee#Rs#Lt #gfor #eSurvival #ghas #w#zStarted#n#g!!!#n" );
	send_to_char( "You have started the PK Quest.\n\r", ch );
      first_team = -1;
      second_team = -1;
      third_team = -1;
      arena = FIGHT_QUEST;
	return;
    }

    if ( !str_cmp( arg1, "arena" ) )
    {
       if ( arena != FIGHT_ARENA && arena != FIGHT_CLEAR )
          {
             stc( "The arena isn't free for changing arena usage at this time.\n\r", ch );
             return;
          }

       if ( arena != FIGHT_ARENA )
          {
             arena = FIGHT_ARENA;
             do_info( ch, "#wThe #PA#oren#Pa #gis now open for #Cone #yon #oone #Rcombat#g!#n" );
             send_to_char( "You have opened the Arena.\n\r", ch );
          }
          else {
                  arena = FIGHT_CLEAR;
                  do_info( ch, "#wThe #PA#oren#Pa #gis now #Pclosed #gfor #erenovations...#n" );
                  send_to_char( "You have closed the Arena.\n\r", ch );
               }

       return;
    }

    if ( !str_cmp( arg1, "winners" ) )
    {
       int count = 1;
       int check;

       for ( count = 1; count < 4; count++ )
       {
          switch( count )
          {
             default:
             case 1: sprintf( buf, "#y1st Team: "); check = first_team; break;
             case 2: sprintf( buf, "#C2nd Team: "); check = second_team; break;
             case 3: sprintf( buf, "#w3rd Team: "); check = third_team; break;
          }

          switch( check )
          {
             default: strcat( buf, "#eNone#n\n\r" ); break;
             case 1: strcat( buf, "#rRED team#n\n\r" ); break;
             case 2: strcat( buf, "#bBLUE team#n\n\r" ); break;
             case 3: strcat( buf, "#yYELLOW team#n\n\r" ); break;
             case 4: strcat( buf, "#eGREY team#n\n\r" ); break;
             case 5: strcat( buf, "#pPURPLE team#n\n\r" ); break;
             case 6: strcat( buf, "#wWHITE team#n\n\r" ); break;
             case 7: strcat( buf, "#gGREEN team#n\n\r" ); break;
             case 8: strcat( buf, "#CCYAN team#n\n\r" ); break;
          }

          stc( buf, ch );
       }
       stc("\n\r",ch);
       return;
    }

    if ( !str_cmp( arg1, "auto" ) )
    {
      int count = 1;

      for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	  if ( gch == ch || IS_NPC(gch) )
	    continue;

        if ( gch->level < 3 || IS_IMMORTAL(gch) )
           continue;

        if ( (get_age(gch)-17) < 2 )
           continue;

        if ( IS_EXTRA(gch,EXTRA_OSWITCH) )
           continue;

        gch->pcdata->team = count;

        switch( count )
        {
           default:
           case 1: stc("You have joined the #rRED#n team!!!\n\r",gch); break;
           case 2: stc("You have joined the #bBLUE#n team!!!\n\r",gch); break;
           case 3: stc("You have joined the #yYELLOW#n team!!!\n\r",gch); break;
           case 4: stc("You have joined the #eGREY#n team!!!\n\r",gch); break;
           case 5: stc("You have joined the #pPURPLE#n team!!!\n\r",gch); break;
           case 6: stc("You have joined the #wWHITE#n team!!!\n\r",gch); break;
           case 7: stc("You have joined the #gGREEN#n team!!!\n\r",gch); break;
           case 8: stc("You have joined the #CCYAN#n team!!!\n\r",gch); break;
        }

        char_from_room(gch);

        switch( count )
        {
           default:
           case 1: char_to_room( gch,get_room_index(550) ); break;
           case 2: char_to_room( gch,get_room_index(565) ); break;
           case 3: char_to_room( gch,get_room_index(554) ); break;
           case 4: char_to_room( gch,get_room_index(569) ); break;
           case 5: char_to_room( gch,get_room_index(610) ); break;
           case 6: char_to_room( gch,get_room_index(625) ); break;
           case 7: char_to_room( gch,get_room_index(629) ); break;
           case 8: char_to_room( gch,get_room_index(614) ); break;
        }

        count++;

        if ( count == 4 )
           count = 1;
	}
	return;
    }

    if ( !str_cmp( arg1, "survival" ) )
    {
      for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	  if ( gch == ch || IS_NPC(gch) )
	    continue;

        if ( gch->level < 3 || IS_IMMORTAL(gch) )
           continue;

        if ( (get_age(gch)-17) < 2 )
           continue;

        if ( IS_EXTRA(gch,EXTRA_OSWITCH) )
           continue;

        gch->pcdata->team = 9;

        stc("You have joined the #rR#yA#wI#eN#bB#pO#CW #yteam#n!!!\n\r",gch);

        char_from_room(gch);
        char_to_room( gch,get_room_index( number_range(550,629) ) );
	}
	return;
    }

    argument = one_argument( argument, arg2 );
    if ( arg2[0] == '\0' )
    {
	send_to_char( "Syntax:  ftag reset\n\r", ch );
	send_to_char( "         ftag start\n\r", ch );
	send_to_char( "         ftag winners\n\r", ch );
	send_to_char( "         ftag arena (opens/closes the arena for one on one)\n\r", ch );
	send_to_char( "         ftag auto (splits everyone in room onto teams)\n\r", ch );
	send_to_char( "         ftag survial (makes a free for all quest)\n\r", ch );
	send_to_char( "         ftag red/blue/yellow/grey/purple/white/green/cyan <player>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( (get_age(victim)-17) < 2 )
    {
	send_to_char( "But they are a newbie.\n\r", ch );
	return;
    }

    if ( IS_EXTRA(victim,EXTRA_OSWITCH) )
    {
	send_to_char( "But they are an object.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "red" ) )
    {
      victim->pcdata->team = 1;
	act( "You are on the #1RED#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #1RED#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #1RED#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(550) );
    }
    else if ( !str_cmp( arg1, "blue" ) )
    {
      victim->pcdata->team = 2;
	act( "You are on the #4BLUE#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #4BLUE#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #4BLUE#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(565) );
    }
    else if ( !str_cmp( arg1, "yellow" ) )
    {
      victim->pcdata->team = 3;
	act( "You are on the #yYELLOW#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #yYELLOW#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #yYELLOW#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(554) );
    }
    else if ( !str_cmp( arg1, "grey" ) )
    {
      victim->pcdata->team = 4;
	act( "You are on the #eGREY#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #eGREY#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #eGREY#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(569) );
    }
    else if ( !str_cmp( arg1, "purple" ) )
    {
      victim->pcdata->team = 5;
	act( "You are on the #pPURPLE#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #pPURPLE#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #pPURPLE#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(610) );
    }
    else if ( !str_cmp( arg1, "white" ) )
    {
      victim->pcdata->team = 6;
	act( "You are on the #wWHITE#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #wWHITE#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #wWHITE#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(625) );
    }
    else if ( !str_cmp( arg1, "green" ) )
    {
      victim->pcdata->team = 7;
	act( "You are on the #gGREEN#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #gGREEN#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #gGREEN#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(629) );
    }
    else if ( !str_cmp( arg1, "cyan" ) )
    {
      victim->pcdata->team = 8;
	act( "You are on the #CCYAN#n team!", ch, NULL, victim, TO_VICT );
	act( "$N is on the #CCYAN#n team!",   ch, NULL, victim, TO_NOTVICT );
	act( "$N is on the #CCYAN#n team!",   ch, NULL, victim, TO_CHAR    );
      char_from_room(victim);
      char_to_room( victim, get_room_index(614) );
    }

    return;
}

void do_destroy( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( IS_IMMORTAL( victim ) )
      {
         stc("Nests won't dare oppose those from the higher dimensions...\n\r", ch );
         return;
      }

   if ( IS_NPC( victim ) )
      {
         stc("Nests don't waste their time killing inferior beings...\n\r", ch );
         return;
      }

   if ( ch->in_room != victim->in_room )
      stc("#eYou silently commend Zero to destroy your target as he fades away into the shadows...#n\n\r", ch );

   act("The room suddenly fill with an intense light as you find yourself in some eerie base.", victim, NULL, NULL, TO_CHAR );
   act("The room suddenly fill with an intense light as you find yourself in some eerie base.", victim, NULL, NULL, TO_ROOM );
   act("Suddenly, you are standing face to face against #eZero#n...", victim, NULL, NULL, TO_CHAR );
   act("Suddenly, you are standing face to face against #eZero#n...", victim, NULL, NULL, TO_ROOM );
   act("Zero sneers '#yToday is one of your unlucky days... Krizalid?#n'", victim, NULL, NULL, TO_CHAR );
   act("Zero sneers '#yToday is one of your unlucky days... Krizalid?#n'", victim, NULL, NULL, TO_ROOM );
   act("A shadowy figure suddenly lands beside Zero as he faces you.", victim, NULL, NULL, TO_CHAR );
   act("A shadowy figure suddenly lands beside Zero as he faces $n.", victim, NULL, NULL, TO_ROOM );
   act("Krizalid screams '#eDie!#n as he releases a #eMASSIVE #Pburst#n of #Rflames#n at you!", victim, NULL, NULL, TO_CHAR );
   act("Krizalid screams '#eDie!#n as he releases a #eMASSIVE #Pburst#n of #Rflames#n at $n!", victim, NULL, NULL, TO_ROOM );
   act("#rThe last thing you notice is the hungry flames eating through your flesh...#n", victim, NULL, NULL, TO_CHAR );
   act("#r$n didn't even have time to scream as the hungry flames AnNhIlAtE $m!#n", victim, NULL, NULL, TO_ROOM );

   if ( victim->pcdata->kingdom > 0 && king_table[victim->pcdata->kingdom].members > 0 )
      {
         king_table[victim->pcdata->kingdom].members--;
         save_kingdoms();
         victim->pcdata->kingdom = 0;
         save_char_obj( victim );
      }

   remove_member(victim);

   do_update_class_count( victim->class, FALSE );
   do_update_class_count( victim->class_2, FALSE );
   do_update_class_count( victim->class_3, FALSE );
   do_update_class_count( victim->class_4, FALSE );
   do_update_class_count( victim->class_5, FALSE );

   sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
   stop_fighting(victim,TRUE);

   victim->login_timer = 0;
   victim->safe_timer = 0;
   victim->fight_timer = 0;
   victim->position = POS_STANDING;
   victim->level = 1;

   do_quit(victim,"");
   unlink(strsave);
}

void do_resetarea(CHAR_DATA *ch, char *argument)
{
    send_to_char("You patiently twiddle your thumbs, waiting for the reset.\n\r",ch);
    area_update();
}

void do_nosafe(CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
       return;

    if ( mysafe != 0 )
       {
          mysafe = 0;
          do_info(ch,"Safes are back in!!!");
          send_to_char("Safe Rooms are back in operational.\n\r",ch);
          return;
       }

    mysafe = 10;
    do_info(ch,"Safes are out!!!");
    send_to_char("Safe Rooms are removed.\n\r",ch);
}

void do_kingdom_wars(CHAR_DATA *ch, char *argument)
{
    int count;

    if ( IS_NPC(ch) )
       return;

    if ( arena == FIGHT_WAR || arena == FIGHT_WAIT )
       {
          armageddon = FALSE;
          arena = FIGHT_CLEAR;
          mud_version++;

          do_echo( ch, "#wKrizalid chats '#PAnd that concludes the war... Please check #y#zmudstat#n #Pfor winners!#n" );
          return;
       }

    if ( arena != FIGHT_CLEAR )
       {
          stc("#wYou can't start a kingdom war until the arena is free.\n\r#n", ch );
          return;
       }

    for ( count = 0; count < 4; count++ )
    {
       king_table[count].wk = 0;
    }

    free_string( man_match );
    man_match = str_dup( "" );
    armageddon = TRUE;
    nogang = 0;
    man_kills = 0;
    w1_kills = 0;
    w2_kills = 0;
    w1_kdg = -1;
    w2_kdg = -1;
    time_info.hour = 0;
    arena = FIGHT_WAIT;
    do_echo( ch, "#wKrizalid chats '#PBy order of Lord Zero himself, the annual Kingdom Wars shall begin shortly!#n" );
}


void do_armageddon(CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
       return;

    if ( armageddon )
       {
          if ( arena == FIGHT_WAIT || arena == FIGHT_WAR )
             {
                stc("#wYou can't turn off armageddon when the realm is at war.\n\r#n", ch );
                return;
             }

          armageddon = FALSE;
          armageddon_timer = 0;
          do_info(ch,"#wThe #gWorld #yhas #rregained its #w#zSanity!!!#n");
          send_to_char("Armageddon state is over.\n\r",ch);
          return;
       }

    armageddon = TRUE;
    nogang = 0;
    do_info(ch,"#wRun #gfor #yyour #Clives!!! #wArmageddon #ehas #R#zarrived!!!#n");
    send_to_char("The mud is now in armageddon mode.\n\r",ch);
}

void do_nogang(CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
       return;

    if ( nogang != 0 )
       {
          nogang = 0;
          do_info(NULL,"#wGroup #RBattles#g are back #y#zin#n#g!!!#n");
          return;
       }

    nogang = 10;
    do_info(NULL,"#wGanging#e is #R#zOuT#n#e!!!#n");
}


void do_tick(CHAR_DATA *ch, char *argument)
{
    send_to_char("TICK!  Now wasn't that fun for you.\n\r",ch);
    weather_update();
    char_update();
    obj_update();
    area_update();
    room_update();
    update_pos(ch);
}

/*
void do_reseteq( CHAR_DATA *ch, char *argument )
{
do_forceauto(ch,"rem all");
do_forceauto(ch,"drop all");
do_purge(ch,"");
do_autosave(ch,"");
send_to_char("Your eq has been wiped ",ch);
return;
}
*/
void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level > LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}


void do_testarm( CHAR_DATA *ch, char *argument )
{
   SET_BIT(ch->newbits, THIRD_HAND);
   SET_BIT(ch->newbits, FOURTH_HAND);
   stc("Ohh. You grow some arms.\n\r",ch);
   return;
}
void do_noarm( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
char arg1[100];

one_argument(argument, arg1);

  if ((victim = get_char_world(ch, arg1)) == NULL) {
    stc("They are not here.\r\n", ch);
    return;
  }

	
  if (!IS_SET(victim->newbits, THIRD_HAND) && !IS_SET(victim->newbits,
      FOURTH_HAND)) {
    return;
   } else {
    if (IS_SET(victim->newbits, THIRD_HAND) &&
!IS_CLASS(ch, CLASS_DEMON)) {
     REMOVE_BIT(victim->newbits, THIRD_HAND);
     stc("Third arm removed.\r\n", ch);}
 
    if (IS_SET(victim->newbits, FOURTH_HAND) && !IS_CLASS(ch,
CLASS_DEMON))
{    REMOVE_BIT(victim->newbits, FOURTH_HAND);
    stc("Fourth arm removed.\r\n", ch);
 }   
}
 return; 
} 

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );
	free_string( ch->pcdata->bamfin );
	ch->pcdata->bamfin = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );
	free_string( ch->pcdata->bamfout );
	ch->pcdata->bamfout = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}

void do_nosummon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch summon ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_SUMMON);
	send_to_char("You now cant be the target of summon and portal.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_SUMMON);
	send_to_char("You now can be the target of summon and portal.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_norescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch summon ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_RESCUE) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_RESCUE);
	send_to_char("You can no longer be rescued by your allies.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_RESCUE) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_RESCUE) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_RESCUE);
	send_to_char("You can now be rescued by your allies.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_RESCUE) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_transport( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch transport ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_TRANSPORT);
	send_to_char("You can no longer be the target of transport spells.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_TRANSPORT);
	send_to_char("You can now be the target of transport spells.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    sprintf(buf,"%s: Deny %s",ch->name,argument);

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    send_to_char( "OK.\n\r", ch );

    stop_fighting(victim,TRUE);
    victim->fight_timer = 0;
    victim->safe_timer = 0;
    victim->login_timer = 0;
    do_quit( victim, "" );
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Disconnect %s",ch->name,argument);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}

void do_info( CHAR_DATA *ch, char *argument )
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
          send_to_char( "#p[#w<#gInfo#w>#p]#c ",d->character );
          send_to_char( argument, d->character );
          send_to_char( " #p[#w<#gInfo#w>#p]#n\n\r",   d->character );
	}
    }
    return;
}

void do_sinfo( CHAR_DATA *ch, char *argument )
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
          send_to_char( "#p[#e<#yScAvEnGeR#e>#p]#c ",d->character );
          send_to_char( argument, d->character );
          send_to_char( " #p[#e<#yScAvEnGeR#e>#p]#n\n\r",   d->character );
	}
    }
    return;
}

void logchan( char *argument )
{
   CHAR_DATA *ch;
   DESCRIPTOR_DATA *d;
    
   if ( argument[0] == '\0' )
      return;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( ( ch = d->character ) == NULL )
         continue;

      if ( ( d->connected == CON_PLAYING ) && IS_JUDGE( ch )
           && !IS_SET( ch->deaf,CHANNEL_LOG ) )
         {
            send_to_char( "[", ch );
            send_to_char( argument, ch );
            send_to_char( "]\n\r",  ch );
         }
   }
}

void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Echo %s",ch->name,argument);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Echo what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Recho %s",ch->name,argument);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Recho what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL)
	if( can_see(ch, victim) )
	    return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL && obj->in_room != NULL)
	return obj->in_room;

    if ( obj != NULL && obj->carried_by != NULL && obj->carried_by->in_room != NULL)
	return obj->carried_by->in_room;

    if ( obj != NULL && obj->in_obj != NULL && obj->in_obj->in_room != NULL)
	return obj->in_obj->in_room;

    if ( obj != NULL && obj->in_obj != NULL && obj->in_obj->carried_by && obj->in_obj->carried_by->in_room != NULL)
	return obj->in_obj->carried_by->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    CHAR_DATA *mount;

    sprintf(buf,"%s: Transfer %s",ch->name,argument);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}

	if ( room_is_private( location ) )
	{
	    send_to_char( "That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( "$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( "$n has transferred you.", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
    if ( (mount = victim->mount) == NULL) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    if ( ch != mount )
	act( "$n has transferred you.", ch, NULL, mount, TO_VICT );
    do_look( mount, "auto" );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    CHAR_DATA *wch;
    
    sprintf(buf,"%s: At %s",ch->name,argument);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    break;
	}
    }

    return;
}



void do_goto( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *in_room;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	stc("No such location.\n\r",ch);
	return;
    }

    in_room = ch->in_room;
    if ( ch->fighting )
	stop_fighting( ch, TRUE );

    char_from_room( ch );
    char_to_room( ch, location );

    do_look( ch, "auto" );

    if ( ch->in_room == in_room )
      return;
}



void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( ch->in_room != location && room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: '%s.'\n\rArea: '%s'.\n\r",
	location->name,
	location->area->name );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %d.  Sector: %d.  Light: %d.\n\r",
	location->vnum,
	location->sector_type,
	location->light );
    send_to_char( buf, ch );

    sprintf( buf,
	"Room flags: %d.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	send_to_char( " ", ch );
	one_argument( rch->name, buf );
	send_to_char( buf, ch );
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",

		door,
		pexit->to_room != NULL ? pexit->to_room->vnum : 0,
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char nm1[40];
    char nm2[40];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Ostat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
	sprintf(nm1,obj->questmaker); else sprintf(nm1,"None");
    if (obj->questowner != NULL && strlen(obj->questowner) > 1)
	sprintf(nm2,obj->questowner); else sprintf(nm2,"None");

    sprintf( buf, "Name: %s.\n\r",
	obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d.  Type: %s.\n\r",
	obj->pIndexData->vnum, item_type_name( obj ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );

    sprintf( buf, "Object creator: %s.  Object owner: %s.  Quest points: %d.\n\r", nm1, nm2, obj->points );
    send_to_char( buf, ch );
    if (obj->quest != 0)
    {
    	send_to_char( "Quest selections:", ch );
    	if (IS_SET(obj->quest,QUEST_STR)) send_to_char(" Str",ch);
    	if (IS_SET(obj->quest,QUEST_DEX)) send_to_char(" Dex",ch);
    	if (IS_SET(obj->quest,QUEST_INT)) send_to_char(" Int",ch);
    	if (IS_SET(obj->quest,QUEST_WIS)) send_to_char(" Wis",ch);
    	if (IS_SET(obj->quest,QUEST_CON)) send_to_char(" Con",ch);
    	if (IS_SET(obj->quest,QUEST_HIT)) send_to_char(" Hp",ch);
    	if (IS_SET(obj->quest,QUEST_MANA)) send_to_char(" Mana",ch);
    	if (IS_SET(obj->quest,QUEST_MOVE)) send_to_char(" Move",ch);
    	if (IS_SET(obj->quest,QUEST_HITROLL)) send_to_char(" Hit",ch);
    	if (IS_SET(obj->quest,QUEST_DAMROLL)) send_to_char(" Dam",ch);
    	if (IS_SET(obj->quest,QUEST_AC)) send_to_char(" Ac",ch);
    	send_to_char( ".\n\r", ch );
    }
    sprintf( buf, "Wear bits: %d.  Extra bits: %s.\n\r",
	obj->wear_flags, extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Weight: %d/%d.\n\r",
	obj->weight, get_obj_weight( obj ) );
    send_to_char( buf, ch );

    sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.\n\r",
	obj->cost, obj->timer, obj->level );
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" : obj->carried_by->name,
	obj->wear_loc );
    send_to_char( buf, ch );
    
    sprintf( buf, "Values: %d %d %d %d.\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    send_to_char( buf, ch );

    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	send_to_char( "'.\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    return;
}

void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Mstat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    sprintf( buf, "#wName: #y%s#w.\n\r", victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "#wVnum: #y%d#w.  Sex: #y%s#w.  Room: #y%d#w.\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	victim->sex == SEX_MALE    ? "male"   :
	victim->sex == SEX_FEMALE  ? "female" : "neutral",
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    sprintf( buf, "#wStr: #y%d#w.  Vit: #y%d#w.  Min: #y%d#w.  Spi: #y%d#w.  Dex: #y%d#w.  Eva: #y%d#w.  Agi: #y%d#w.  Ten: #y%d#w.  Luck: #y%d#w.\n\r",
	get_curr_str(victim),
	get_curr_con(victim),
	get_curr_int(victim),
	get_curr_wis(victim),
	get_curr_dex(victim),
	get_curr_eva(victim),
	get_curr_agi(victim),
	get_curr_tens(victim),
	get_curr_luck(victim) );
    send_to_char( buf, ch );

    sprintf( buf, "#wHp: #C%d#w/#y%d#w.  Mana: #C%d#w/#y%d#w.  Move: #C%d#w/#y%d#w.\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move );
    send_to_char( buf, ch );
	
    sprintf( buf,
	"#wLv: #y%d#w.  Align: #y%d#w.  Position: #y%d#w.  Gold: #y%d#w.  Bank: #y%d#w.\n\r",
	( IS_NPC( victim ) ) ? victim->level : victim->tier,       victim->alignment,
	victim->position,     victim->gold,         victim->bank );
    send_to_char( buf, ch );

    if ( IS_NPC( victim ) )
       {
          sprintf( buf, "#wHitroll: #y%d#w.  Damroll: #y%d#w.\n\r", char_hitroll( victim ), char_damroll( victim ) );
          send_to_char( buf, ch );
	 }

    sprintf( buf, "#wFighting: #y%s#w.\n\r",
	victim->fighting ? victim->fighting->name : "(none)" );
    send_to_char( buf, ch );

    sprintf( buf, "#wMaster: #y%s#w.  Leader: #y%s#w.  Affected by: #y%s#w.\n\r",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	affect_bit_name( victim->affected_by ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none).\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
	send_to_char( "#wMobile has #Cspec #yfun#w.\n\r", ch );

    if ( victim->affected != NULL )
       {
          send_to_char( "You are affected by:\n\r", ch );
          for ( paf = victim->affected; paf != NULL; paf = paf->next )
          {
              if ( paf->duration != -1 )
                 {
                     sprintf( buf, "#wSpell: #o'%s'", skill_table[paf->type].name );
                     send_to_char( buf, ch );

                     if ( str_cmp( "none", affect_loc_name( paf->location ) ) )
                        {
                            sprintf( buf, " #wgives %s #y%s%d #wfor #y%d #whours.#n\n\r",
                                          affect_loc_name( paf->location ),
                                          ( paf->modifier > -1 ) ? "+":"",
                                          paf->modifier,                    
                                          paf->duration );
                        }
                        else sprintf( buf, " #wfor #y%d #whours.#n\n\r", paf->duration );

                     send_to_char( buf, ch );
                  }
          }
       }
}



void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mfind whom?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Ofind what?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mwhere whom?\n\r", ch );
	return;
    }

    found = FALSE;
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( IS_NPC(victim)
	&&   victim->in_room != NULL
	&&   is_name( arg, victim->name ) )
	{
	    found = TRUE;
	    sprintf( buf, "[%5d] %-28s [%5d] %s\n\r",
		victim->pIndexData->vnum,
		victim->short_descr,
		victim->in_room->vnum,
		victim->in_room->name );
	    send_to_char( buf, ch );
	}
    }

    if ( !found )
	act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );

    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}



void do_reboot( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    sprintf( buf, "\033[1;24r\033[2JReboot by An Immortal." );
    do_echo( ch, buf );
    do_restore(ch,"all");
    do_forceauto(ch,"call all");
    call_all(ch);
    do_forceauto(ch,"save");
    do_autosave(ch,"");
    merc_down = TRUE;
    return;
}


void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    sprintf( buf, "Shutdown by An Immortal." );
    append_file( ch, SHUTDOWN_FILE, buf );
    strcat( buf, "\n\r" );
    do_echo( ch, buf );
    do_forceauto(ch,"save");
    do_autosave(ch,"");
    merc_down = TRUE;
    return;
}



void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Snoop %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_WATCHER))
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_oswitch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *mount;

    sprintf(buf,"%s: Oswitch %s",ch->name,argument);

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into what?\n\r", ch );
	return;
    }

    if ( IS_EXTRA(ch, EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD) )
    {
	send_to_char( "You are already oswitched.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_SWITCH) )
    {
	send_to_char( "Not while switched.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( obj->chobj != NULL )
    {
	send_to_char( "Object in use.\n\r", ch );
	return;
    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oreturn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    sprintf(buf,"%s: Oreturn",ch->name);

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;
    
    if ( !IS_EXTRA(ch, EXTRA_OSWITCH) && !IS_HEAD(ch,LOST_HEAD) )
    {
	send_to_char( "You are not oswitched.\n\r", ch );
	return;
    }
    if ( ( obj = ch->pcdata->chobj ) != NULL ) obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0], LOST_HEAD);
    free_string(ch->morph);
    ch->morph = str_dup("");
    char_from_room(ch);
    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_return( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Return %s",ch->name,argument);

    if ( ch->desc == NULL )
       return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( "You return to your original body.\n\r", ch );
    REMOVE_BIT(ch->desc->original->extra, EXTRA_SWITCH);
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc; 
    ch->desc                  = NULL;
    return;
}



void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Mload %s",ch->name,argument);
    
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: mload <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
    act( "You have created $N!", ch, NULL, victim, TO_CHAR );
    return;
}


void do_pload( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *in_room;
    bool fOld;

    if ( IS_NPC(ch) || ch->desc == NULL || ch->in_room == NULL ) return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax: pload <name>.\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( !char_exists(FALSE,argument) )
    {
	send_to_char( "That player doesn't exist.\n\r", ch );
	return;
    }

    sprintf(buf,"%s: Pload %s",ch->name,argument);
    
    argument[0] = UPPER(argument[0]);

    sprintf(buf,"You transform into %s.\n\r",argument);
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms into %s.",argument);
    act(buf,ch,NULL,NULL,TO_ROOM);

    d = ch->desc;
    do_autosave(ch,"");
    in_room = ch->in_room;
    extract_char(ch, TRUE);
    d->character = NULL;
    fOld = load_char_obj( d, argument );
    ch   = d->character;
    ch->next = char_list;
    char_list = ch;
    char_to_room(ch,in_room);
    return;
}

void do_preturn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool fOld;

    if (IS_NPC(ch)) {send_to_char("Huh?\n\r",ch);return;}

    if (ch->pload == NULL) {send_to_char("Huh?\n\r",ch);return;}
    sprintf(arg,ch->pload);
    if (strlen(arg) < 3 || strlen(arg) > 8) 
	{send_to_char("Huh?\n\r",ch);return;}

    if (!str_cmp(ch->name,arg)) {send_to_char("Huh?\n\r",ch);return;}

    d = ch->desc;

    sprintf(buf,"You transform back into %s.\n\r",capitalize(ch->pload));
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms back into %s.",capitalize(ch->pload));
    act(buf,ch,NULL,NULL,TO_ROOM);
    do_autosave(ch,"");
    if (ch != NULL && ch->desc != NULL)
	extract_char(ch,TRUE);
    else if (ch != NULL)
	extract_char(ch,TRUE);
    if (ch->desc) ch->desc->character = NULL;
/*
    ch->next = char_list;
    char_list = ch;
*/
    fOld = load_char_obj(d, capitalize(arg));
    if (ch->in_room != NULL)
    	char_to_room(ch,ch->in_room);
    else
    	char_to_room(ch,get_room_index(3001));
    free_string(ch->pload);
    ch->pload = str_dup("");
    return;
}


void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    sprintf(buf,"%s: Oload %s",ch->name,argument);
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
        return;
    }
 
    if ( arg2[0] == '\0' )
    {
	level = get_trust( ch );
    }
    else
    {
	/*
	 * New feature from Alander.
	 */
        if ( !is_number( arg2 ) )
        {
	    send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	    return;
        }
        level = atoi( arg2 );
	if ( level < 0 || level > get_trust( ch ) )
        {
	    send_to_char( "Limited to your trust level.\n\r", ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
    {
	obj_to_char( obj, ch );
	act( "$p appears in $n's hands!", ch, obj, NULL, TO_ROOM );
    }
    else
    {
	obj_to_room( obj, ch->in_room );
	act( "$n has created $p!", ch, obj, NULL, TO_ROOM );
    }
    act( "You create $p.", ch, obj, NULL, TO_CHAR );
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    return;
}



void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if (IS_NPC(victim) && victim->desc == NULL && (mount = victim->mount) == NULL)
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	if (obj->item_type != ITEM_KINGDOM_POWER)
	    extract_obj( obj );
	}

	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on PC's.\n\r", ch );
	return;
    }
    if ( victim->desc != NULL )
    {
	send_to_char( "Not on switched players.\n\r", ch );
	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE );
    return;
}



void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int level;

    sprintf(buf,"%s: Trust %s",ch->name,argument);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: trust <char> <trust>.\n\r", ch );
	send_to_char( "Trust being one of: None, Builder, Questmaker, Enforcer, Judge, or Highjudge.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

         if ( !str_cmp( arg2, "none"       ) ) level = 0;
    else if ( !str_cmp( arg2, "builder"    ) ) level = 7;
    else if ( !str_cmp( arg2, "questmaker" ) ) level = 8;
    else if ( !str_cmp( arg2, "enforcer"   ) ) level = 9;
    else if ( !str_cmp( arg2, "judge"      ) ) level = 10;
    else if ( !str_cmp( arg2, "highjudge"  ) ) level = 11;
    else
    {
	send_to_char( "Please enter: None, Builder, Questmaker, Enforcer, Judge, or Highjudge.\n\r", ch );
	return;
    }

    if ( level >= get_trust( ch ) )
    {
	send_to_char( "Limited to below your trust.\n\r", ch );
	return;
    }
    send_to_char("Ok.\n\r",ch);
    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;

    sprintf(buf,"%s: Restore %s",ch->name,argument);

    one_argument( argument, arg );

    if ( get_trust(ch) >= MAX_LEVEL - 2 && !str_cmp(arg,"all") )
    {
       for ( d = descriptor_list; d != NULL; d = d->next )
       {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;
    
          restore_char(victim);
          update_pos( victim);

          if ( victim->in_room != NULL )
             act("$n has restored you.",ch,NULL,victim,TO_VICT);
       }

       send_to_char("All active players restored.\n\r",ch);
       return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
       send_to_char( "They aren't here.\n\r", ch );
       return;
    }

    restore_char(victim);
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
    victim->loc_hp[6] = 0;
    update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    send_to_char( "Ok.\n\r", ch );
}

void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Freeze %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Bitchslap whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You stop crying.\n\r", victim );
	send_to_char( "Crybaby removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You recieve a BITCHSLAP, and start crying like the bitch you are!\n\r", victim );
	send_to_char( "Crybaby set.\n\r", ch );
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Log %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Noemote %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }


    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Notell %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_NO_TELL) )
    {
	REMOVE_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NO_TELL removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NO_TELL set.\n\r", ch );
    }

    return;
}


void do_undeny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    	char *oldname;
	CHAR_DATA *victim;
	DESCRIPTOR_DATA *d;
	ROOM_INDEX_DATA *in_room;

    sprintf(buf,"%s: Undeny %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Undeny whom?", ch );
	return;
    }

	if ((victim = get_char_world(ch, arg)) != NULL) {
	send_to_char("They are already online.\n\r", ch );
	return;}

    if (!char_exists(FALSE,arg))
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	oldname = str_dup(ch->pcdata->switchname);
	d = ch->desc;
	do_autosave(ch,"");
	in_room = ch->in_room;
	extract_char(ch, TRUE);
	d->character = NULL;
	load_char_obj(d, arg);
	ch = d->character;
	ch->next = char_list;
	char_list = ch;
	char_to_room(ch,in_room);    
	
 
    if ( IS_SET(ch->act, PLR_DENY) )
    {
	REMOVE_BIT(ch->act, PLR_DENY);
	send_to_char( "DENY removed.\n\r", ch );
    } else {
	send_to_char("They are not DENIED.\n\r", ch ); }

	d = ch->desc;
	do_autosave(ch,"");
	in_room = ch->in_room;
	extract_char(ch, TRUE);
	d->character = NULL;
	load_char_obj(d, oldname);
	ch = d->character;
	ch->next = char_list;
	char_list = ch;
	char_to_room(ch,in_room);    


    return;
}

void do_silence( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Silence %s",ch->name,argument);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Silence whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_SILENCE) )
    {
	REMOVE_BIT(victim->act, PLR_SILENCE);
	send_to_char( "You can use channels again.\n\r", victim );
	send_to_char( "SILENCE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_SILENCE);
	send_to_char( "You can't use channels!\n\r", victim );
	send_to_char( "SILENCE set.\n\r", ch );
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Peace %s",ch->name,argument);

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
         {
	      stop_fighting( rch, TRUE );
            rch->fight_timer = 0;
         }
    }

    if ( !IS_CLASS(ch, CLASS_ANGEL) ) send_to_char( "Ok.\n\r", ch );
    return;
}

void do_wizlock( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Wizlock %s",ch->name,argument);
    wizlock = !wizlock;

    if ( wizlock )
	send_to_char( "Game wizlocked.\n\r", ch );
    else
	send_to_char( "Game un-wizlocked.\n\r", ch );

    return;
}


void do_closemud( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Wizlock %s",ch->name,argument);
    wizlock = !wizlock;

    if ( wizlock )
        send_to_char( "Game wizlocked.\n\r", ch );
    else
        send_to_char( "Game un-wizlocked.\n\r", ch );

    return;
}


void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Slookup what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
		sn, skill_table[sn].slot, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].slot, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}



void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    sprintf(buf,"%s: Sset %s",ch->name,argument);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "#7Syntax: sset <victim> <skill> <value>\n\r",	ch
);
	send_to_char( "#7or:     sset <victim> all     <value>\n\r",	ch
);
	send_to_char( "#3Skill being any skill or spell.\n\r",		ch
);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL )
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    send_to_char("Ok.\n\r",ch);
    return;
}


void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;

    sprintf(buf,"%s: Mset %s",ch->name,argument);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( !str_cmp(arg1,"prophet") )
       {
          do_eipheal( 1 );
          return;
       }

    if ( !str_cmp(arg1,"conan") )
       {
          do_conan( 1 );
          return;
       }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "#7Syntax: mset <victim> <field>  <value>\n\r",		ch );
	send_to_char( "#7or:     mset <victim> <string> <value>\n\r",		ch );
	send_to_char( "\n\r",							ch );

	send_to_char( "#7Field being one of:\n\r",				ch );
	send_to_char( "#3  str vit min spi dex eva agi sex level sp\n\r",		ch );
	send_to_char( "#3  gen hp mana move points align status prophet tlvl\n\r",			ch );
	send_to_char( "#3  thirst drunk full hit dam ac dp safetimer tier lpoints\n\r",		ch );
	send_to_char( "#3  security tension rune extra bounty hours faith aura conan\n\r",		ch );
	send_to_char( "#3  rank mkill exp bank helper pkill summon equip skillbuy\n\r\n\r", ch );

	send_to_char( "#7String being one of:\n\r",				ch );
	send_to_char( "#3  name short long description title spec clan#n\n\r",	ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */

    if(!str_cmp(arg2,"aura"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

      give_random_aura( victim );
	stc( "Ok.\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"equip"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

      find_item( victim, 0 );
	stc( "Ok.\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"helper"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

      if ( IS_SET( victim->act, ACT_HELPER ) )
         {
            stc("They are no longer a #Pnewbie #whelper#n.\n\r", ch );
            stc("You are no longer a #Pnewbie #whelper#n.\n\r", victim );
            REMOVE_BIT( victim->act, ACT_HELPER );
         }
         else {
                 stc("They are now a #Pnewbie #whelper#n.\n\r", ch );
                 stc("You are now a #Pnewbie #whelper#n.\n\r", victim );
                 SET_BIT( victim->act, ACT_HELPER );
              }

	return;
    }

    if ( !str_cmp(arg2, "extra") )
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}
	if( !str_cmp(arg3,"pregnant"))
	{
	    if( IS_EXTRA(victim,EXTRA_PREGNANT))
		REMOVE_BIT(victim->extra, EXTRA_PREGNANT);
	    else
		SET_BIT(victim->extra,EXTRA_PREGNANT);
	    stc( "Ok.\n\r",ch);
	    return;
	}
	else if( !str_cmp(arg3,"poly"))
	{
	    if( IS_SET(victim->affected_by, AFF_POLYMORPH) )
		REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	    else
		SET_BIT(victim->affected_by, AFF_POLYMORPH);
	    stc( "Ok.\n\r",ch);
	    return;
	}
	else if( !str_cmp(arg3,"zuloform"))
	{
	    if( IS_SET(victim->polyaff, POLY_ZULOFORM))
		REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
	    else
		SET_BIT(victim->polyaff, POLY_ZULOFORM);
	    stc("Ok.\n\r",ch);
	    return;
	}
	else if( !str_cmp(arg3,"pdragon"))
	{
	    if( IS_SET(victim->polyaff, POLY_DRAGON))
		REMOVE_BIT(victim->polyaff, POLY_DRAGON);
	    else
		SET_BIT(victim->polyaff, POLY_DRAGON);
	    stc("Ok.\n\r",ch);
	    return;
	}
	else
	{
	   stc( "Pregnant, pdragon, zuloform, poly.\n\r",ch);
	   return;
	}
    }

    if(!str_cmp(arg2,"bounty"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}
	if( value < 0) value = 0;
	victim->pcdata->bounty = value;
	sprintf(buf, "%s bounty is now at %d.\n\r",victim->name,victim->pcdata->bounty);
	stc(buf, ch);
	return;
    }

    if(!str_cmp(arg2,"skillbuy"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}
	if( value < 0) value = 0;
	victim->cmbt[4] = value;
      stc( "#wOk.#n\n\r", ch );
	return;
    }

    if(!str_cmp(arg2,"tier"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}
	if( value < 1) value = 1;

	if( value > 5) value = 5;
	victim->tier = value;
      stc("#wOk.#n\n\r", ch );
	return;
    }

    if(!str_cmp(arg2,"lpoints"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

	if( value < 0) value = 0;

	victim->pcdata->faithpoints = value;
      stc("#wOk.#n\n\r", ch );
	return;
    }

    if(!str_cmp(arg2,"tlvl"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

	if( value < 0) value = 0;

      add_transmigration( victim, value );
      stc("#wOk.#n\n\r", ch );
	return;
    }

    if(!str_cmp(arg2,"pkill"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}
	if( value < 0) value = 0;

      if ( IS_CLASS( victim, CLASS_PALADIN ) && victim->clannum != 1 )
         {
            int count;

            for ( count = 8; count < 16; count++ )
                victim->aura[count] = UMIN( 10, value / 2 );

            stc("#wYour mastery of the #Cancient auras #whave improved warrior.\n\r", victim );
         }

      if ( IS_CLASS( victim, CLASS_DRAGON_KNIGHT ) )
         victim->pcdata->powers[DRAGOON_LEVEL] = UMIN( 10, value / 2 );

      if ( IS_CLASS( victim, CLASS_UNDEAD_KNIGHT ) )
         {
            int count;

            for ( count = 28; count < 35; count++ )
                victim->aura[count] = UMIN( 10, value / 2 );

            stc("#eYour mastery of the #Pancient auras #ehave improved warrior.\n\r", victim );
         }

      if ( IS_CLASS( victim, CLASS_SAIYAN ) )
         victim->saiyan_powerlevel = UMIN( 100000, value * 5000 );

	victim->pkill = value;
	stc( "Ok.\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"summon"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}
	if( value < 0) value = 0;

	victim->cmbt[0] = value;
	stc( "Ok.\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"rank"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

	if( value < 1 || value > 6) value = 1;
	victim->pcdata->clanrank = value;
	stc("Ok\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"mkill"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

	if( value < 0 || value > 1000000) value = 0;
	victim->mkill = value;
	stc("Ok\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"faith"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

	if( value < 0 || value > 600) value = 0;
	victim->pcdata->faithpoints = value;
	stc("Ok\n\r", ch);
	return;
    }

    if(!str_cmp(arg2,"sp"))
    {
	if( IS_NPC(victim))
	{
	    stc( "Not on mobs.\n\r",ch);
	    return;
	}

	if( value < 0 || value > 100) value = 0;
	victim->aura[A_POINTS] = value;
	stc("Ok\n\r", ch);
	return;
    }

    if (!str_cmp(arg2, "hours"))
    {
        if ( IS_NPC( victim ) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }
        
        if ( !is_number( arg3 ) )
        {
            send_to_char( "Value must be numeric.\n\r", ch );
            return;
        }
        
        value = atoi( arg3 );
        
        if ( value < 0 || value > 999 )
        {
            send_to_char( "Value must be betwen 0 and 999.\n\r", ch );
            return;
        }
            
        value *= 3600;
        victim->played = value;
        return;
    }

    if ( !str_cmp( arg2, "str" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 99 )
	{
	    send_to_char( "Strength range is 5 to 99.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_str = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "dp" ))
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r",ch);
	    return;
	}

	if ( value < 1 || value > 50000000 )
	{
	    send_to_char("Valid range is 1 to 50000000.\n\r",ch);
	    return;
	}
   
	victim->pcdata->stats[DEMON_CURRENT] = value;
	victim->pcdata->stats[DEMON_TOTAL]   = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "safetimer" ))
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r",ch);
	    return;
	}

	if ( value < 1 || value > 1000 )
	{
	    send_to_char("Valid range is 1 to 1000.\n\r",ch);
	    return;
	}
   
	victim->safe_timer = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }


    if ( !str_cmp( arg2, "min" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 99 )
	{
	    send_to_char( "Mind range is 5 to 99.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_int = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "spi" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 99 )
	{
	    send_to_char( "Spirit range is 5 to 99.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_wis = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 99 )
	{
	    send_to_char( "Dexterity range is 5 to 99.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_dex = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "vit" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 99 )
	{
	    send_to_char( "Vitality range is 5 to 99.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_con = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "tens" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 33 )
	{
	    send_to_char( "Tension range is 5 to 33.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_tension = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "eva" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 5 || value > 99 )
	{
	    send_to_char( "Evasion range is 5 to 99.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_eva = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "agi" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value > 10 )
	{
	    send_to_char( "Agility range is 1 to 10.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_agi = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	victim->sex = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	if ( IS_NPC(victim) && ( value < 1 || value > 999 ) )
	{
	    send_to_char( "Level range is 1 to 999 for mobs.\n\r", ch );
	    return;
	}
	else if ( !IS_JUDGE(ch) )
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}

           if ( !str_cmp( arg3, "mortal"     ) ) value = 3;
      else if ( !str_cmp( arg3, "avatar"     ) ) value = 3;
      else if ( !str_cmp( arg3, "apprentice" ) ) value = 4;
      else if ( !str_cmp( arg3, "mage"       ) ) value = 5;
      else if ( !str_cmp( arg3, "monk"       ) ) value = 6;          
      else if ( !str_cmp( arg3, "archmage"   ) ) value = 6;
      else if ( !str_cmp( arg3, "builder"   ) ) value = 7;
      else if ( !str_cmp( arg3, "immortal"   ) ) value = 12;
      else if ( !IS_NPC(victim) )
              {
                 send_to_char( "Level should be one of the following:\n\rMortal, Avatar, Apprentice, Mage, Archmage, Builder or Immortal.\n\r", ch );
                 return;
              }
	
      if ( value > ch->level && !IS_NPC(victim) ) 
        send_to_char( "Sorry, no can do...\n\r", ch );
        else {
                victim->level = value;
                send_to_char("Ok.\n\r",ch);
             }

      if ( !IS_NPC(victim) )
         victim->trust = value;

	return;
    }

if ( !str_cmp( arg2, "hitroll" ) || !str_cmp(arg2, "hit" ) )
    {
	if ( !IS_NPC(victim) && ( value < 0 || value > 50 ) )
	{
	    send_to_char( "Hitroll range is 0 to 50.\n\r", ch );
	    return;
	}
	else if ( IS_NPC(victim) && ( value < 0 || value > 250 ) )
	{
	    send_to_char( "Hitroll range is 0 to 250.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->hitroll = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "damroll" ) || !str_cmp( arg2, "dam" ) )
    {
	if ( !IS_NPC(victim) && ( value < 0 || value > 50 ) )
	{
	    send_to_char( "Damroll range is 0 to 50.\n\r", ch );
	    return;
	}
	else if ( IS_NPC(victim) && ( value < 0 || value > 250 ) )
	{
	    send_to_char( "Damroll range is 0 to 250.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->damroll = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

if ( !str_cmp( arg2, "Xhitroll" ) || !str_cmp( arg2, "hit" ) )
    {
	if ( !IS_NPC(victim) && ( value < 0 || value > 50 ) )
	{
	    send_to_char( "Xhitroll range is 0 to 50.\n\r", ch );
	    return;
	}
	else if ( IS_NPC(victim) && ( value < 0 || value > 250 ) )
	{
	    send_to_char( "Xhitroll range is 0 to 250.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->xhitroll = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "Xdamroll" ) || !str_cmp( arg2, "dam" ) )
    {
	if ( !IS_NPC(victim) && ( value < 0 || value > 50 ) )
	{
	    send_to_char( "Xdamroll range is 0 to 50.\n\r", ch );
	    return;
	}
	else if ( IS_NPC(victim) && ( value < 0 || value > 250 ) )
	{
	    send_to_char( "Xdamroll range is 0 to 250.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->xdamroll = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "armor" ) || !str_cmp( arg2, "ac" ) )
    {
	if ( !IS_NPC(victim) && ( value < -200 || value > 200 ) )
	{
	    send_to_char( "Armor class range is -200 to 200.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->armor = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "exp" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 )
	{
	    send_to_char( "Exp must be at least 0.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch) || (ch == victim)) {
		victim->exp = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "gold" ) )
    {
	if (value > 10000000 && !IS_JUDGE(ch))
		send_to_char("Don't be so damn greedy!\n\r",ch);
	else
		{victim->gold = value; send_to_char("Ok.\n\r",ch);}
	return;
    }

    if ( !str_cmp( arg2, "bank" ) )
    {
	if (value > 10000000 && !IS_JUDGE(ch))
		send_to_char("Don't be so damn greedy!\n\r",ch);
	else
		{victim->bank = value; send_to_char("Ok.\n\r",ch);}
	return;
    }

    if ( !str_cmp( arg2, "hp" ) )
    {
	if ( value < 1 || value > 25000000 )
	{
	    send_to_char( "Hp range is 1 to 25,000,000 hit points.\n\r", ch );
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim) || (IS_NPC(victim)) ) {
		victim->max_hit = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "mana" ) )
    {
	if ( value < 0 || value > 20000000 )
	{
	    send_to_char( "Mana range is 0 to 20,000,000 mana points.\n\r", ch );
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim) || (IS_NPC(victim)) ) {
		victim->max_mana = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "move" ) )
    {
	if ( value < 0 || value > 20000000 )
	{
	    send_to_char( "Move range is 0 to 20,000,000 move points.\n\r", ch );
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim) || (IS_NPC(victim)) ) {
		victim->max_move = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "points" ) )
    {
	if ( value < 0 )
	{
	    send_to_char( "Training Points range is 0 to infinate.\n\r", ch );
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim)) {
		victim->practice = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = ( value == -1 ) ? -1000 : value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 3000 )
	{
	    send_to_char( "Thirst range is 0 to 2000.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Drunk range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 1200 )
	{
	    send_to_char( "Full range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }


    if ( !str_cmp( arg2, "name" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	free_string( victim->name );
	victim->name = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}
	free_string( victim->short_descr );
	victim->short_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}
	free_string( victim->long_descr );
	victim->long_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "clan" ) )
    {
	free_string( victim->clan );
	victim->clan = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if ( !str_cmp( arg2, "title" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	set_title( victim, arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	{
	    send_to_char( "No such spec fun.\n\r", ch );
	    return;
	}

	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "gen" ) )
    {
	if ( IS_NPC( victim ) )
	return;

	victim->generation = UMAX( 1, value );
	send_to_char("Ok.\n\r",ch);
	return;
    }   

    if ( !str_cmp( arg2, "status" ) )
    {
	if ( IS_NPC( victim ) )
	return;

	victim->race = UMAX( 0, value );
	send_to_char("Ok.\n\r",ch);
	return;
    }   

    if ( !str_cmp( arg2, "security" ) ) /* OLC */
    {
        if ( IS_NPC( victim ) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }
     
        if ( value > ch->pcdata->security || value < 0 )
        {
            if ( ch->pcdata->security != 0 )
            {
                sprintf( buf, "Valid security is 0-%d.\n\r",
                    ch->pcdata->security );
                send_to_char( buf, ch );
                send_to_char( buf, ch );
            }
            else
            {
                send_to_char( "Valid security is 0 only.\n\r", ch );
            }
            return;
        }
        victim->pcdata->security = value;
        return;
    }

 
    /*
     * Generate usage message.
     */

    do_mset( ch, "" );
    return;
}

void do_pack(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  OBJ_DATA *obj2;
  char arg[MAX_STRING_LENGTH];
  one_argument(argument,arg);
  if (arg[0] == '\0')
  {send_to_char("Newbiepack Who?\n\r",ch);
  return;}
  if ((victim=get_char_world(ch,arg)) == NULL)
  {send_to_char("They arent here.\n\r",ch);
  return;}
  obj2 = create_object(get_obj_index(388),0);
  obj  = create_object(get_obj_index(389),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(390),0);
  obj = NULL;
  obj_to_char(obj,victim);

  obj  = create_object(get_obj_index(391),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(392),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(393),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(397),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(397),0);
  obj_to_char(obj,victim);
  obj = NULL;
 
  obj  = create_object(get_obj_index(394),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(395),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(386),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(386),0);
  obj_to_char(obj,victim);
  obj = NULL;
 
  obj  = create_object(get_obj_index(387),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(387),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(396),0);
  obj_to_char(obj,victim);

  obj = NULL;
  obj  = create_object(get_obj_index(396),0);
  obj_to_char(obj,victim);
 
  obj_to_char(obj2,victim);
  send_to_char(
 "Tons of stuff appears in your hands in a blast of flames.\n\r",victim);
  send_to_char("You newbiepack them.\n\r",ch);
  return;
  }
 
 

void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *morph;
    OBJ_INDEX_DATA *pObjIndex;
    int value;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    sprintf(buf,"%s: Oset %s",ch->name,argument);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "#7Syntax: oset <object> <field>  <value>\n\r",	ch);
	send_to_char( "#7or:     oset <object> <string> <value>\n\r",	ch);
	send_to_char( "#7or:     oset <object> <affect> <value>\n\r",	ch);
	send_to_char( "\n\r",						ch );
	send_to_char( "#7Field being one of:\n\r",			ch);
	send_to_char( "#3  value0 value1 value2 value3 skill\n\r",		ch);
	send_to_char( "#3  level weight cost timer morph\n\r",		ch);
	send_to_char( "\n\r",						ch );
	send_to_char( "#7String being one of:\n\r",			ch);
	send_to_char( "#3  name short long ed type extra wear owner\n\r",ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "#7Affect being one of:\n\r",			ch);
	send_to_char( "#3  str dex min spir vit eva luck\n\r",		ch);
	send_to_char( "#3  hp mana move\n\r",ch );
	stc("  #3attackgood\n\r",ch);
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker)))
    {
	send_to_char("You don't have permission to change that item.\n\r", ch);
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
	if (obj->item_type == ITEM_WEAPON && !IS_JUDGE(ch)) {
		send_to_char("You are not authorised to create spell weapons.\n\r", ch);
		return;}
/*	else if (obj->item_type == ITEM_QUEST) {
		send_to_char("You cannot change a quest tokens value with oset.\n\r", ch);
		return;}
	else if (obj->item_type == ITEM_ARMOR && value > 15)
		obj->value[0] = 15;*/
	else
		obj->value[0] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
/*	if (obj->item_type == ITEM_WEAPON && value > 10)
		obj->value[1] = 10;
	else*/
		obj->value[1] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
/*	if (obj->item_type == ITEM_WEAPON && value > 20)
		obj->value[2] = 20;
	else*/
		obj->value[2] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	if (obj->item_type == ITEM_ARMOR && !IS_JUDGE(ch))
		send_to_char("You are not authorised to create spell armour.\n\r", ch);
	else
		{obj->value[3] = value; send_to_char("Ok.\n\r",ch);}
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "skill" ) )
    {
        add_skill( obj, value );
        return;
    }

    if ( !str_cmp( arg2, "morph" ) )
    {
	int mnum;
	if ( IS_SET(obj->spectype, SITEM_MORPH) )
    	{
	    send_to_char( "This item can already morph.\n\r", ch );
	    return;
    	}
    	if ( ( pObjIndex = get_obj_index( value ) ) == NULL )
    	{
	    send_to_char( "No object has that vnum.\n\r", ch );
	    return;
    	}
    	morph = create_object( pObjIndex, obj->level );
    	if ( !CAN_WEAR(morph, ITEM_TAKE) )
	    SET_BIT(morph->wear_flags, ITEM_TAKE);
	if ( !IS_SET(obj->spectype, SITEM_MORPH) )
	    SET_BIT(obj->spectype, SITEM_MORPH);
	if ( !IS_SET(morph->spectype, SITEM_MORPH) )
	    SET_BIT(morph->spectype, SITEM_MORPH);
	obj_to_char( morph, ch );
	act( "$p morphs into $P in $n's hands!", ch, obj, morph, TO_ROOM );
	act( "$p morphs into $P in your hands!", ch, obj, morph, TO_CHAR );
	mnum = obj->wear_loc;
	obj_from_char( obj );
	obj_to_obj( obj, morph );
	if ( morph->wear_flags == obj->wear_flags && mnum != WEAR_NONE )
	    equip_char( ch, morph, mnum );
    	if (morph->questmaker != NULL) free_string(morph->questmaker);
    	morph->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "extra" ) )
    {
	if      ( !str_cmp( arg3, "glow"         )) value = ITEM_GLOW;
	else if ( !str_cmp( arg3, "hum"          )) value = ITEM_HUM;
	else if ( !str_cmp( arg3, "thrown"       )) value = ITEM_THROWN;
	else if ( !str_cmp( arg3, "vanish"       )) value = ITEM_VANISH;
	else if ( !str_cmp( arg3, "invis"        )) value = ITEM_INVIS;
	else if ( !str_cmp( arg3, "magic"        )) value = ITEM_MAGIC;
	else if ( !str_cmp( arg3, "nodrop"       )) value = ITEM_NODROP;
	else if ( !str_cmp( arg3, "bless"        )) value = ITEM_BLESS;
	else if ( !str_cmp( arg3, "anti-good"    )) value = ITEM_ANTI_GOOD;
	else if ( !str_cmp( arg3, "anti-evil"    )) value = ITEM_ANTI_EVIL;
	else if ( !str_cmp( arg3, "anti-neutral" )) value = ITEM_ANTI_NEUTRAL;
	else if ( !str_cmp( arg3, "noremove"     )) value = ITEM_NOREMOVE;
	else if ( !str_cmp( arg3, "inventory"    )) value = ITEM_INVENTORY;
	else if ( !str_cmp( arg3, "loyal"        )) value = ITEM_LOYAL;
	else {
	    send_to_char("Extra flag can be from the following: Glow, Hum, Thrown, Vanish, Invis, Magic, Nodrop, Bless, Anti-Good, Anti-Evil, Anti-Neutral, Noremove, Inventory, Loyal.\n\r",ch); return;}

	/* Removing magic flag allows multiple enchants */
	if (IS_SET(obj->extra_flags,value) && value == ITEM_MAGIC && !IS_JUDGE(ch))
	{send_to_char("Sorry, no can do...\n\r",ch); return;}

	if (IS_SET(obj->extra_flags,value))
	    REMOVE_BIT(obj->extra_flags, value);
	else
	    SET_BIT(obj->extra_flags, value);
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "wear" ) )
    {
	if      ( !str_cmp( arg3, "none" ) || !str_cmp( arg3, "clear" ))
	{
	    obj->wear_flags = 0;
	    send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);
	    return;
	}
	else if ( !str_cmp( arg3, "take"   ))
	{
	    if (IS_SET(obj->wear_flags,ITEM_TAKE))
		REMOVE_BIT(obj->wear_flags, ITEM_TAKE);
	    else
		SET_BIT(obj->wear_flags, ITEM_TAKE);
	    send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);
	    return;
	}
	else if ( !str_cmp( arg3, "finger" )) value = ITEM_WEAR_FINGER;
	else if ( !str_cmp( arg3, "neck"   )) value = ITEM_WEAR_NECK;
	else if ( !str_cmp( arg3, "body"   )) value = ITEM_WEAR_BODY;
	else if ( !str_cmp( arg3, "head"   )) value = ITEM_WEAR_HEAD;
	else if ( !str_cmp( arg3, "legs"   )) value = ITEM_WEAR_LEGS;
	else if ( !str_cmp( arg3, "feet"   )) value = ITEM_WEAR_FEET;
	else if ( !str_cmp( arg3, "hands"  )) value = ITEM_WEAR_HANDS;
	else if ( !str_cmp( arg3, "arms"   )) value = ITEM_WEAR_ARMS;
	else if ( !str_cmp( arg3, "about"  )) value = ITEM_WEAR_ABOUT;
	else if ( !str_cmp( arg3, "waist"  )) value = ITEM_WEAR_WAIST;
	else if ( !str_cmp( arg3, "wrist"  )) value = ITEM_WEAR_WRIST;
	else if ( !str_cmp( arg3, "hold"   )) value = ITEM_WIELD;
	else if ( !str_cmp( arg3, "face"   )) value = ITEM_WEAR_FACE;
	else {
	    send_to_char("Wear location can be from: None, Take, Finger, Neck, Body, Head, Legs, Hands, Arms, About, Waist, Hold, Face.\n\r",ch); return;}
	if (IS_SET(obj->wear_flags,ITEM_TAKE) ) value += 1;
	obj->wear_flags = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	if (value < 1) value = 1; else if (value > 50) value = 50;
	if (!IS_JUDGE(ch))
	    send_to_char("You are not authorised to change an items level.\n\r", ch);
	else
	    {obj->level = value; send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);}
	return;
    }
	
    if ( !str_cmp( arg2, "weight" ) )
    {
	obj->weight = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "cost" ) )
    {
	if (value > 100000 && !IS_JUDGE(ch))
		send_to_char("Don't be so damn greedy!\n\r",ch);
	else
		{obj->cost = value; send_to_char("Ok.\n\r",ch);
	    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    		obj->questmaker = str_dup(ch->name);}
	return;
    }

    if ( !str_cmp( arg2, "timer" ) )
    {
	obj->timer = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }
	
    if ( !str_cmp( arg2, "hitpoints" ) || !str_cmp( arg2, "hp" ) )
	{oset_affect(ch,obj,value,APPLY_HIT,FALSE);return;}
    else if ( !str_cmp( arg2, "mana" ) )
	{oset_affect(ch,obj,value,APPLY_MANA,FALSE);return;}
    else if ( !str_cmp( arg2, "move" ) || !str_cmp( arg2, "movement" ) )
	{oset_affect(ch,obj,value,APPLY_MOVE,FALSE);return;}
    else if ( !str_cmp( arg2, "str" ) || !str_cmp( arg2, "strength" ) )
	{oset_affect(ch,obj,value,APPLY_STR,FALSE);return;}
    else if ( !str_cmp( arg2, "dex" ) || !str_cmp( arg2, "dexterity" ) )
	{oset_affect(ch,obj,value,APPLY_DEX,FALSE);return;}
    else if ( !str_cmp( arg2, "min" ) || !str_cmp( arg2, "mind" ) )
	{oset_affect(ch,obj,value,APPLY_INT,FALSE);return;}
    else if ( !str_cmp( arg2, "spi" ) || !str_cmp( arg2, "spirit" ) )
	{oset_affect(ch,obj,value,APPLY_WIS,FALSE);return;}
    else if ( !str_cmp( arg2, "vit" ) || !str_cmp( arg2, "vitality" ) )
	{oset_affect(ch,obj,value,APPLY_CON,FALSE);return;}
    else if ( !str_cmp( arg2, "eva" ) || !str_cmp( arg2, "evasion" ) )
	{oset_affect(ch,obj,value,APPLY_EVA,FALSE);return;}
    else if ( !str_cmp( arg2, "luc" ) || !str_cmp( arg2, "luck" ) )
	{oset_affect(ch,obj,value,APPLY_LUCK,FALSE);return;}
    else if ( !str_cmp( arg2, "ten" ) || !str_cmp( arg2, "tension" ) )
	{oset_affect(ch,obj,value,APPLY_TENSION,FALSE);return;}

    if( !str_cmp( arg2, "quint") )
    {
	if ( obj->item_type != ITEM_WEAPON )
	{
	    act("$p is not a weapon.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	if( !str_cmp(arg3, "off") )
	{
	    REMOVE_BIT(obj->extra_flags,ITEM_MENCHANT);
	    stc( "Removed from weapon\n\r", ch);
	}
	else if(!str_cmp(arg3, "on") )
	{
	    SET_BIT(obj->extra_flags,ITEM_MENCHANT);
	    stc( "Set on weapon\n\r", ch);
	}
	else
	{
	    stc( "On or off please\n\r", ch);
	    return;
	}
	return;
    }

    if ( !str_cmp( arg2, "type" ) )
    {
	if (!IS_JUDGE(ch))
	{
	    send_to_char("You are not authorised to change an item type.\n\r", ch);
	    return;
	}
	if      (!str_cmp( arg3, "light")) obj->item_type = 1;
	else if (!str_cmp( arg3, "scroll")) obj->item_type = 2;
	else if (!str_cmp( arg3, "wand")) obj->item_type = 3;
	else if (!str_cmp( arg3, "staff")) obj->item_type = 4;
	else if (!str_cmp( arg3, "weapon")) obj->item_type = 5;
	else if (!str_cmp( arg3, "treasure")) obj->item_type = 8;
	else if (!str_cmp( arg3, "armor")) obj->item_type = 9;
	else if (!str_cmp( arg3, "armour")) obj->item_type = 9;
	else if (!str_cmp( arg3, "potion")) obj->item_type = 10;
	else if (!str_cmp( arg3, "furniture")) obj->item_type = 12;
	else if (!str_cmp( arg3, "trash")) obj->item_type = 13;
	else if (!str_cmp( arg3, "container")) obj->item_type = 15;
	else if (!str_cmp( arg3, "drink")) obj->item_type = 17;
	else if (!str_cmp( arg3, "key")) obj->item_type = 18;
	else if (!str_cmp( arg3, "food")) obj->item_type = 19;
	else if (!str_cmp( arg3, "money")) obj->item_type = 20;
	else if (!str_cmp( arg3, "boat")) obj->item_type = 22;
	else if (!str_cmp( arg3, "corpse")) obj->item_type = 23;
	else if (!str_cmp( arg3, "fountain")) obj->item_type = 25;
	else if (!str_cmp( arg3, "pill")) obj->item_type = 26;
	else if (!str_cmp( arg3, "instrument")) obj->item_type = ITEM_INSTRUMENT;
	else if (!str_cmp( arg3, "portal")) obj->item_type = 27;
	else if (!str_cmp( arg3, "stake")) obj->item_type = 30;
	else {send_to_char("Type can be one of: Light, Scroll, Wand, Staff, Weapon, Treasure, Armor, Potion, Furniture, Trash, Container, Drink, Key, Food, Money, Boat, Corpse, Fountain, Pill, Portal, Stake.\n\r", ch);
	     return;}
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "owner" ) )
    {
	if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch);return;}
	if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker)))
	{send_to_char("Someone else has already changed this item.\n\r",ch);return;}
	if ( ( victim = get_char_world( ch, arg3 ) ) == NULL )
	{send_to_char("You cannot find any player by that name.\n\r",ch);return;}
	if (IS_NPC(victim)) {send_to_char("Not on NPC's.\n\r",ch);return;}
	if (obj->questmaker != NULL) free_string(obj->questmaker);
	obj->questmaker = str_dup(ch->name);
	if (obj->questowner != NULL) free_string(obj->questowner);
	obj->questowner = str_dup(victim->name);
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	free_string( obj->name );
	obj->name = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	free_string( obj->short_descr );
	obj->short_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	free_string( obj->description );
	obj->description = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "ed" ) )
    {
	EXTRA_DESCR_DATA *ed;

	argument = one_argument( argument, arg3 );
	if ( argument == NULL )
	{
	    send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		ch );
	    return;
	}

	if ( extra_descr_free == NULL )
	{
	    ed			= alloc_perm( sizeof(*ed) );
	}
	else
	{
	    ed			= extra_descr_free;
	    extra_descr_free	= ed;
	}

	ed->keyword		= str_dup( arg3     );
	ed->description		= str_dup( argument );
	ed->next		= obj->extra_descr;
	obj->extra_descr	= ed;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    sprintf(buf,"%s: Rset %s",ch->name,argument);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "#7Syntax: rset <location> <field> value\n\r",	ch
);
	send_to_char( "\n\r",						ch );
	send_to_char( "#7Field being one of:\n\r",			ch
);
	send_to_char( "#3  flags sector\n\r",				ch
);
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_cmp( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}


void do_omni(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if (IS_NPC(ch)) return;

      stc("#gThe #wPlayers of #eUtopia:#n\n\r\n\r",ch);

	sprintf(buf," #w         Name#C|#wLevel#C|#wClass#C|#wAlign#C|#wSta#C|     #wHit#C|    #wMana#C|    #wMove#C|#n\n\r");
	send_to_char(buf,ch);
      sprintf(buf,"#C--------------|-----|-----|-----|---|--------|--------|--------|#n\n\r");
	send_to_char(buf,ch);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
         CHAR_DATA *wch;

		if (d->connected != CON_PLAYING) continue;
		wch = ( d->original != NULL) ? d->original : d->character;

            sprintf(buf,"#y%14s#C|#y%5d#C|#y%5d#C|#y%5d#C|#y%3d#C|#y%8d#C|#y%8d#C|#y%8d#C|#n\n\r",
		wch->pcdata->switchname,
		wch->tier,
		wch->class,
		wch->alignment,
		wch->pkill,
		wch->max_hit,
		wch->max_mana,
		wch->max_move
		);	

		send_to_char(buf,ch);
	}

	return;
}

void is_multiplaying( )
{
  DESCRIPTOR_DATA *d;
  DESCRIPTOR_DATA *d2;

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character == NULL )
       continue;

    if ( IS_SET( d->character->extra, EXTRA_MULTI ) )
       REMOVE_BIT( d->character->extra, EXTRA_MULTI );
  }
      
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->connected != CON_PLAYING) continue;  
    if ( d->character == NULL || IS_IMMORTAL(d->character) ) continue;

    for ( d2 = d->next; d2 != NULL; d2 = d2->next )
    {
      if (!str_cmp(d->host, d2->host))
      {
        if (d2->connected != CON_PLAYING) continue;
        if (d2->character == NULL || d->character == NULL || IS_IMMORTAL(d2->character)) continue;

        if ( !IS_SET( d2->character->extra, EXTRA_MULTI ) )
           SET_BIT( d2->character->extra, EXTRA_MULTI );

        if ( !IS_SET( d->character->extra, EXTRA_MULTI ) )
           SET_BIT( d->character->extra, EXTRA_MULTI );
      }
    }
  }
}

void check_multi( CHAR_DATA *ch )
{
  DESCRIPTOR_DATA *d;
      
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
     if ( d->character == NULL || IS_IMMORTAL( d->character ) || d->character == ch )
        continue;

     if ( !str_cmp( d->host, ch->desc->host ) )
        {
           FILE *fp;

           if ( ( fp = fopen( "../multi.txt", "a" ) ) != NULL )
              {
                 fprintf( fp, "%s and %s are multiplaying from %s.\n\r", ch->pcdata->switchname, 
                              d->character->pcdata->switchname, d->host );
                 fclose( fp );

                 sprintf( log_buf, "%s and %s are multiplaying from %s.", ch->pcdata->switchname, 
                                   d->character->pcdata->switchname, d->host );
                 log_string( log_buf );
              }

           return;
        }
  }
}

void do_users( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int count;
    char *st;

    count	= 0;
    buf[0]	= '\0';

    is_multiplaying( );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character != NULL && can_see( ch, d->character ) )
	{
	    count++;

          if ( IS_SET( d->character->extra, EXTRA_MULTI ) )
             st = "#y MULTIPLAYING #n ";
             else switch ( d->connected ) 
                  {
                      case CON_PLAYING:              st = "#1    PLAYING   #n ";    break;
                      case CON_GET_NAME:             st = "#3   Get Name   #n ";    break;
                      case CON_GET_OLD_PASSWORD:     st = "#3Get Old Passwd#n ";    break;
                      case CON_CONFIRM_NEW_NAME:     st = "#3 Confirm Name #n ";    break;
                      case CON_GET_LAST_NAME:        st = "#3Get Last Name #n ";    break;
                      case CON_GET_NEW_PASSWORD:     st = "#3Get New Passwd#n ";    break;
                      case CON_CONFIRM_NEW_PASSWORD: st = "#3Confirm Passwd#n ";    break;
                      case CON_GET_NEW_SEX:          st = "#3  Get New Sex #n ";    break;
                      case CON_GET_NEW_VT102:        st = "#3 Picking VT102#n ";    break;
                      case CON_GET_NEW_ANSI:         st = "#6 Picking ANSI #n ";    break;
                      case CON_ROLL_STATS:           st = "#6Rolling Stats #n ";    break;
                      case CON_GET_HEIGHT:           st = "#6Getting Height#n ";    break;
                      case CON_GET_WEIGHT:           st = "#6Getting Weight#n ";    break;
                      case CON_GET_AGE:              st = "#6 Getting Age  #n ";    break;
                      case CON_READ_MOTD:            st = "#7 Reading MOTD #n ";    break;
                      case CON_GET_NEW_CLASS:        st = "#5 Picking Class#n ";    break;
                      case CON_NOT_PLAYING:          st = "#2 *Not* Playing#n ";    break;
                      case CON_NOTE_TO:
                      case CON_NOTE_SUBJECT:
                      case CON_NOTE_EXPIRE:
                      case CON_NOTE_TEXT:
                      case CON_NOTE_FINISH:          st = "#2 Writing Note #n ";	break;
                      default:                       st = "#4   !UNKNOWN!#n   ";    break;
                  }

         if ( strlen(d->character->lasthost) < 2 )
	      {
	          sprintf( buf + strlen(buf), "#3[#7%3d %16s#3]  #1%s#2@#3%s\n\r",
		             d->descriptor, st,
                         d->original  ? d->original->pcdata->switchname  :
                         d->character ? d->character->pcdata->switchname : "(none)",
                         d->host );
	      }
	      else {
                    sprintf( buf + strlen(buf), "#3[#7%3d %16s#3]  #1%s#2@#3%s\n\r",
                             d->descriptor, st,
                             d->original  ? d->original->pcdata->switchname  :
                             d->character ? d->character->pcdata->switchname : "(none)",
                             d->character->lasthost );
	           }
	}
    }

    sprintf( buf2, "#w%d #cuser%s#n\n\r", count, count == 1 ? "" : "s" );
    send_to_char( buf2, ch );
    send_to_char( buf, ch );
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Force %s",ch->name,argument);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (get_trust(ch) < MAX_LEVEL - 3)
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}
	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
	    {
		if ( !str_cmp(argument,"Die") )
	         {
		      raw_kill(vch);
		      stc("Hmm. These players need to die.\n\r",ch);
		      stc("Haha. Die Fucker.\n\r",vch);
                  continue;
	         }
              
		interpret( vch, argument );
	    }
	}
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	act( "$n forces you to '$t'.", ch, argument, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_forceauto( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) && vch != ch )
	{
	    act( "Autocommand: $t.", ch, argument, vch, TO_VICT );
	    interpret( vch, argument );
	}
    }
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
	SET_BIT(ch->act, PLR_WIZINVIS);
    }

    return;
}

void do_incog( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_INCOG) )
    {
	REMOVE_BIT(ch->act, PLR_INCOG);
	act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
	SET_BIT(ch->act, PLR_INCOG);
    }

    return;
}


void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#3Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#3Holy light mode on.\n\r", ch );
    }

    return;
}

void do_safe( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( mysafe != 0 || armageddon == TRUE )
       send_to_char( "You are not safe from player attacks in this room.\n\r", ch );
       else if ( IS_SET(ch->in_room->bits,NON_SAFE) )
               send_to_char( "You are not safe from player attacks in this room.\n\r", ch );
       else if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
     	         send_to_char( "You cannot be attacked by other players here.\n\r", ch );
               else send_to_char( "You are not safe from player attacks in this room.\n\r", ch );

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
	return;

    if (ch->in_room->sector_type == SECT_INSIDE)
    {
	send_to_char( "You are inside, which means you are safe from sunlight.\n\r", ch );
	return;
    }

    if (weather_info.sunlight == SUN_DARK)
    {
	send_to_char( "It is not yet light out, so you are safe from the sunlight...for now.\n\r", ch );
	return;
    }

    if ( room_is_dark(ch->in_room) )
    {
	send_to_char( "This room is dark, and will protect you from the sunlight.\n\r", ch );
	return;
    }

    send_to_char( "This room will provide you no protection from the sunlight.\n\r", ch );
}


void do_qstat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char("Which item?\n\r",ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    sprintf(buf,"Object %s.\n\r",obj->short_descr);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when worn: %s\n\r",obj->chpoweron);
    send_to_char(buf,ch);
    sprintf(buf,"Other when worn: %s\n\r",obj->victpoweron);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when removed: %s\n\r",obj->chpoweroff);
    send_to_char(buf,ch);
    sprintf(buf,"Other when removed: %s\n\r",obj->victpoweroff);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when used: %s\n\r",obj->chpoweruse);
    send_to_char(buf,ch);
    sprintf(buf,"Other when used: %s\n\r",obj->victpoweruse);
    send_to_char(buf,ch);
    send_to_char("Type:",ch);
    if ( IS_SET(obj->spectype, SITEM_ACTIVATE))
	send_to_char(" Activate",ch);
    if ( IS_SET(obj->spectype, SITEM_TWIST))
	send_to_char(" Twist",ch);
    if ( IS_SET(obj->spectype, SITEM_PRESS))
	send_to_char(" Press",ch);
    if ( IS_SET(obj->spectype, SITEM_PULL))
	send_to_char(" Pull",ch);
    if ( IS_SET(obj->spectype, SITEM_TARGET))
	send_to_char(" Target",ch);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
	send_to_char(" Spell",ch);
    if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
	send_to_char(" Transporter",ch);
    if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
	send_to_char(" Teleporter",ch);
    if ( IS_SET(obj->spectype, SITEM_OBJECT))
	send_to_char(" Object",ch);
    if ( IS_SET(obj->spectype, SITEM_MOBILE))
	send_to_char(" Mobile",ch);
    if ( IS_SET(obj->spectype, SITEM_ACTION))
	send_to_char(" Action",ch);
    if ( IS_SET(obj->spectype, SITEM_DELAY1))
	send_to_char(" Delay1",ch);
    if ( IS_SET(obj->spectype, SITEM_DELAY2))
	send_to_char(" Delay2",ch);
    if (   !IS_SET(obj->spectype, SITEM_ACTIVATE    )
    	&& !IS_SET(obj->spectype, SITEM_TWIST       )
    	&& !IS_SET(obj->spectype, SITEM_PRESS       )
    	&& !IS_SET(obj->spectype, SITEM_PULL        )
    	&& !IS_SET(obj->spectype, SITEM_TARGET      )
    	&& !IS_SET(obj->spectype, SITEM_SPELL       )
    	&& !IS_SET(obj->spectype, SITEM_TELEPORTER  )
    	&& !IS_SET(obj->spectype, SITEM_DELAY1      )
    	&& !IS_SET(obj->spectype, SITEM_DELAY2      )
    	&& !IS_SET(obj->spectype, SITEM_OBJECT      )
    	&& !IS_SET(obj->spectype, SITEM_MOBILE      )
    	&& !IS_SET(obj->spectype, SITEM_ACTION      )
    	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER ) )
	send_to_char(" No flags set",ch);
    send_to_char(".\n\r",ch);
    sprintf(buf,"Power: %d.\n\r",obj->specpower);
    send_to_char(buf,ch);
    return;
}

void do_qset( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '0' )
    {
    	send_to_char("You can change the following fields...\n\r", ch);
	send_to_char("chwear   = Message to owner when item is worn.\n\r",ch);
	send_to_char("chrem    = Message to owner when item is removed.\n\r",ch);
	send_to_char("chuse    = Message to owner when item is used.\n\r",ch);
	send_to_char("victwear = Message to others in room when item is worn.\n\r",ch);
	send_to_char("victrem  = Message to others in room when item is removed.\n\r",ch);
	send_to_char("victuse  = Message to others in room when item is used.\n\r",ch);
	send_to_char("type       activate     = Item can be activated.\n\r",ch);
	send_to_char("           twist        = Item can be twisted.\n\r",ch);
	send_to_char("           press        = Item can be pressed.\n\r",ch);
	send_to_char("           pull         = Item can be pulled.\n\r",ch);
	send_to_char("           target       = Item can target people (for spell, etc).\n\r",ch);
	send_to_char("           spell        = Item can cast spells.\n\r",ch);
	send_to_char("           transporter  = Owner can transport freely between two locations.\n\r",ch);
	send_to_char("           teleporter   = Owner can transport to target location at will.\n\r",ch);
	send_to_char("           object       = Owner can create the specified object.\n\r",ch);
	send_to_char("           mobile       = Owner can create the specified mobile.\n\r",ch);
	send_to_char("           action       = Target must perform an action.\n\r",ch);
	send_to_char("           delay1       = Sets a delay of half a round on spell items.\n\r",ch);
	send_to_char("           delay2       = Sets a delay of one round on spell items.\n\r",ch);
	send_to_char("power      <value>      = Spell number/transporter room number.\n\r",ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    if (!str_cmp(arg2,"chwear") )
    {
	if (obj->chpoweron != NULL) strcpy(buf,obj->chpoweron);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweron );
	    obj->chpoweron = str_dup( "(null)" );
	}
	else if (obj->chpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweron );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweron = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweron );
	    obj->chpoweron = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"chrem") )
    {
	if (obj->chpoweroff != NULL) strcpy(buf,obj->chpoweroff);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweroff );
	    obj->chpoweroff = str_dup( "(null)" );
	}
	else if (obj->chpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweroff );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweroff = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweroff );
	    obj->chpoweroff = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"chuse") )
    {
	if (obj->chpoweruse != NULL) strcpy(buf,obj->chpoweruse);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweruse );
	    obj->chpoweruse = str_dup( "(null)" );
	}
	else if (obj->chpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweruse );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweruse = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweruse );
	    obj->chpoweruse = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victwear") )
    {
	if (obj->victpoweron != NULL) strcpy(buf,obj->victpoweron);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweron );
	    obj->victpoweron = str_dup( "(null)" );
	}
	else if (obj->victpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweron );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweron = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweron );
	    obj->victpoweron = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victrem") )
    {
	if (obj->victpoweroff != NULL) strcpy(buf,obj->victpoweroff);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweroff );
	    obj->victpoweroff = str_dup( "(null)" );
	}
	else if (obj->victpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweroff );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweroff = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweroff );
	    obj->victpoweroff = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victuse") )
    {
	if (obj->victpoweruse != NULL) strcpy(buf,obj->victpoweruse);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweruse );
	    obj->victpoweruse = str_dup( "(null)" );
	}
	else if (obj->victpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweruse );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweruse = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweruse );
	    obj->victpoweruse = str_dup( arg3 );
	}
    }
    else if ( !str_cmp( arg2, "type" ) )
    {
	if ( !str_cmp( arg3, "activate" ) && IS_SET(obj->spectype, SITEM_ACTIVATE) )
	    REMOVE_BIT(obj->spectype, SITEM_ACTIVATE);
	else if ( !str_cmp( arg3, "activate" ) && !IS_SET(obj->spectype, SITEM_ACTIVATE) )
	    SET_BIT(obj->spectype, SITEM_ACTIVATE);
	else if ( !str_cmp( arg3, "twist" ) && IS_SET(obj->spectype, SITEM_TWIST) )
	    REMOVE_BIT(obj->spectype, SITEM_TWIST);
	else if ( !str_cmp( arg3, "twist" ) && !IS_SET(obj->spectype, SITEM_TWIST) )
	    SET_BIT(obj->spectype, SITEM_TWIST);
	else if ( !str_cmp( arg3, "press" ) && IS_SET(obj->spectype, SITEM_PRESS) )
	    REMOVE_BIT(obj->spectype, SITEM_PRESS);
	else if ( !str_cmp( arg3, "press" ) && !IS_SET(obj->spectype, SITEM_PRESS) )
	    SET_BIT(obj->spectype, SITEM_PRESS);
	else if ( !str_cmp( arg3, "pull" ) && IS_SET(obj->spectype, SITEM_PULL) )
	    REMOVE_BIT(obj->spectype, SITEM_PULL);
	else if ( !str_cmp( arg3, "pull" ) && !IS_SET(obj->spectype, SITEM_PULL) )
	    SET_BIT(obj->spectype, SITEM_PULL);
	else if ( !str_cmp( arg3, "target" ) && IS_SET(obj->spectype, SITEM_TARGET) )
	    REMOVE_BIT(obj->spectype, SITEM_TARGET);
	else if ( !str_cmp( arg3, "target" ) && !IS_SET(obj->spectype, SITEM_TARGET) )
	    SET_BIT(obj->spectype, SITEM_TARGET);
	else if ( !str_cmp( arg3, "spell" ) && IS_SET(obj->spectype, SITEM_SPELL) )
	    REMOVE_BIT(obj->spectype, SITEM_SPELL);
	else if ( !str_cmp( arg3, "spell" ) && !IS_SET(obj->spectype, SITEM_SPELL) )
	    SET_BIT(obj->spectype, SITEM_SPELL);
	else if ( !str_cmp( arg3, "transporter" ) && IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	    REMOVE_BIT(obj->spectype, SITEM_TRANSPORTER);
	else if ( !str_cmp( arg3, "transporter" ) && !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	    SET_BIT(obj->spectype, SITEM_TRANSPORTER);
	else if ( !str_cmp( arg3, "teleporter" ) && IS_SET(obj->spectype, SITEM_TELEPORTER) )
	    REMOVE_BIT(obj->spectype, SITEM_TELEPORTER);
	else if ( !str_cmp( arg3, "teleporter" ) && !IS_SET(obj->spectype, SITEM_TELEPORTER) )
	    SET_BIT(obj->spectype, SITEM_TELEPORTER);
	else if ( !str_cmp( arg3, "object" ) && IS_SET(obj->spectype, SITEM_OBJECT) )
	    REMOVE_BIT(obj->spectype, SITEM_OBJECT);
	else if ( !str_cmp( arg3, "object" ) && !IS_SET(obj->spectype, SITEM_OBJECT) )
	    SET_BIT(obj->spectype, SITEM_OBJECT);
	else if ( !str_cmp( arg3, "mobile" ) && IS_SET(obj->spectype, SITEM_MOBILE) )
	    REMOVE_BIT(obj->spectype, SITEM_MOBILE);
	else if ( !str_cmp( arg3, "mobile" ) && !IS_SET(obj->spectype, SITEM_MOBILE) )
	    SET_BIT(obj->spectype, SITEM_MOBILE);
	else if ( !str_cmp( arg3, "action" ) && IS_SET(obj->spectype, SITEM_ACTION) )
	    REMOVE_BIT(obj->spectype, SITEM_ACTION);
	else if ( !str_cmp( arg3, "action" ) && !IS_SET(obj->spectype, SITEM_ACTION) )
	    SET_BIT(obj->spectype, SITEM_ACTION);
	else if ( !str_cmp( arg3, "delay1" ) && IS_SET(obj->spectype, SITEM_DELAY1) )
	    REMOVE_BIT(obj->spectype, SITEM_DELAY1);
	else if ( !str_cmp( arg3, "delay1" ) && !IS_SET(obj->spectype, SITEM_DELAY1) )
	    SET_BIT(obj->spectype, SITEM_DELAY1);
	else if ( !str_cmp( arg3, "delay2" ) && IS_SET(obj->spectype, SITEM_DELAY2) )
	    REMOVE_BIT(obj->spectype, SITEM_DELAY2);
	else if ( !str_cmp( arg3, "delay2" ) && !IS_SET(obj->spectype, SITEM_DELAY2) )
	    SET_BIT(obj->spectype, SITEM_DELAY2);
	else
	{
	    send_to_char("No such flag to set.\n\r",ch);
	    return;
	}
	sprintf(buf,"%s flag toggled.\n\r",capitalize(arg3));
	send_to_char(buf,ch);
	return;
    }
    else if ( !str_cmp( arg2, "power" ) )
	obj->specpower = value;
    else
    {
	send_to_char("No such flag.\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    return;
}


void do_oclone( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;

    argument = one_argument( argument, arg1 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Make a clone of what object?\n\r", ch );
        return;
    }
 
    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker) || strlen(obj->questmaker) < 2))
    {
	send_to_char("You can only clone your own creations.\n\r", ch);
	return;
    }

    pObjIndex = get_obj_index( obj->pIndexData->vnum);
    obj2 = create_object( pObjIndex, obj->level );
    /* Copy any changed parts of the object. */
    free_string(obj2->name);
    obj2->name=str_dup(obj->name);
    free_string(obj2->short_descr);
    obj2->short_descr=str_dup(obj->short_descr);
    free_string(obj2->description);
    obj2->description=str_dup(obj->description);

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	free_string(obj2->questmaker);
	obj2->questmaker=str_dup(obj->questmaker);
    }

    if (obj->chpoweron != NULL)
    {
    	free_string(obj2->chpoweron);
    	obj2->chpoweron=str_dup(obj->chpoweron);
    }
    if (obj->chpoweroff != NULL)
    {
    	free_string(obj2->chpoweroff);
    	obj2->chpoweroff=str_dup(obj->chpoweroff);
    }
    if (obj->chpoweruse != NULL)
    {
    	free_string(obj2->chpoweruse);
    	obj2->chpoweruse=str_dup(obj->chpoweruse);
    }
    if (obj->victpoweron != NULL)
    {
    	free_string(obj2->victpoweron);
    	obj2->victpoweron=str_dup(obj->victpoweron);
    }
    if (obj->victpoweroff != NULL)
    {
    	free_string(obj2->victpoweroff);
    	obj2->victpoweroff=str_dup(obj->victpoweroff);
    }
    if (obj->victpoweruse != NULL)
    {
    	free_string(obj2->victpoweruse);
    	obj2->victpoweruse=str_dup(obj->victpoweruse);
    }
    obj2->item_type 	= obj->item_type;
    obj2->extra_flags 	= obj->extra_flags;
    obj2->wear_flags 	= obj->wear_flags;
    obj2->weight 	= obj->weight;
    obj2->spectype 	= obj->spectype;
    obj2->specpower 	= obj->specpower;
    obj2->condition 	= obj->condition;
    obj2->toughness 	= obj->toughness;
    obj2->resistance 	= obj->resistance;
    obj2->quest 	= obj->quest;
    obj2->points 	= obj->points;
    obj2->cost 		= obj->cost;
    obj2->value[0] 	= obj->value[0];
    obj2->value[1] 	= obj->value[1];
    obj2->value[2] 	= obj->value[2];
    obj2->value[3] 	= obj->value[3];
    /*****************************************/
    obj_to_char(obj2,ch);

    if (obj->affected != NULL)
    {
    	for ( paf = obj->affected; paf != NULL; paf = paf->next )
    	{
	    if (affect_free == NULL)
	        paf2 = alloc_perm( sizeof(*paf) );
	    else
	    {
		paf2 = affect_free;
		affect_free = affect_free->next;
	    }
	    paf2->type  	= 0;
	    paf2->duration	= paf->duration;
	    paf2->location	= paf->location;
	    paf2->modifier	= paf->modifier;
	    paf2->bitvector	= 0;
	    paf2->next  	= obj2->affected;
	    obj2->affected	= paf2;
    	}
    }

    act( "You create a clone of $p.", ch, obj, NULL, TO_CHAR );
    return;
}

void do_evileye( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int value;
 
    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );
 
    if (ch->power[DISC_DAEM_DISC] < 2)
    {
	stc("You must obtain a level two Mastery of Discord.\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Format is: evileye <option> <value>\n\r", ch );
        send_to_char( "Option ACTION is a text string action performed by you or the viewer.\n\r", ch );
        send_to_char( "Option MESSAGE is a text string shown to the person looking at you.\n\r", ch );
        send_to_char( "Option TOGGLE has values: spell, self, other.\n\r\n\r", ch );
	if ( ch->poweraction != NULL) {
	    sprintf(buf,"Current action: %s.\n\r",ch->poweraction);
	    send_to_char(buf,ch);}
	if ( ch->powertype != NULL) {
	    sprintf(buf,"Current message: %s.\n\r",ch->powertype);
	    send_to_char(buf,ch);}
	send_to_char("Current flags:",ch);
	if (IS_SET(ch->spectype,EYE_SPELL)) send_to_char(" Spell",ch);
	if (IS_SET(ch->spectype,EYE_SELFACTION)) send_to_char(" Self",ch);
	if (IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" Other",ch);
	if (!IS_SET(ch->spectype,EYE_SPELL) &&
	    !IS_SET(ch->spectype,EYE_SELFACTION) &&
	    !IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" None",ch);
	send_to_char(".\n\r",ch);
        return;
    }
    value = is_number( arg2 ) ? atoi( arg2 ) : -1;
    if ( !str_cmp( arg1, "action" ) )
    {
	free_string( ch->poweraction );
	ch->poweraction = str_dup( arg2 );
	return;
    }
    else if ( !str_cmp( arg1, "message" ) )
    {
	free_string( ch->powertype );
	ch->powertype = str_dup( arg2 );
	return;
    }
    else if ( !str_cmp( arg1, "toggle" ) )
    {
	if ( !str_cmp( arg2, "spell" ) && IS_SET(ch->spectype,EYE_SPELL))
	    REMOVE_BIT(ch->spectype,EYE_SPELL);
	else if ( !str_cmp( arg2, "spell" ) && !IS_SET(ch->spectype,EYE_SPELL))
	    SET_BIT(ch->spectype,EYE_SPELL);
	else if ( !str_cmp( arg2, "self" ) && IS_SET(ch->spectype,EYE_SELFACTION))
	    REMOVE_BIT(ch->spectype,EYE_SELFACTION);
	else if ( !str_cmp( arg2, "self" ) && !IS_SET(ch->spectype,EYE_SELFACTION))
	    SET_BIT(ch->spectype,EYE_SELFACTION);
	else if ( !str_cmp( arg2, "other" ) && IS_SET(ch->spectype,EYE_ACTION))
	    REMOVE_BIT(ch->spectype,EYE_ACTION);
	else if ( !str_cmp( arg2, "other" ) && !IS_SET(ch->spectype,EYE_ACTION))
	    SET_BIT(ch->spectype,EYE_ACTION);
	else {
	    send_to_char("TOGGLE flag should be one of: spell, self, other.\n\r",ch);
	    return;}
	sprintf(buf,"%s flag toggled.\n\r",capitalize(arg2));
	send_to_char(buf,ch);
	return;
    }
    else
    {
        send_to_char( "Format is: evileye <option> <value>\n\r", ch );
        send_to_char( "Option ACTION is a text string action performed by you or the viewer.\n\r", ch );
        send_to_char( "Option MESSAGE is a text string shown to the person looking at you.\n\r", ch );
        send_to_char( "Option TOGGLE has values: spell, self, other.\n\r\n\r", ch );
	if ( ch->poweraction != NULL) {
	    sprintf(buf,"Current action: %s.\n\r",ch->poweraction);
	    send_to_char(buf,ch);}
	if ( ch->powertype != NULL) {
	    sprintf(buf,"Current message: %s.\n\r",ch->powertype);
	    send_to_char(buf,ch);}
	send_to_char("Current flags:",ch);
	if (IS_SET(ch->spectype,EYE_SPELL)) send_to_char(" Spell",ch);
	if (IS_SET(ch->spectype,EYE_SELFACTION)) send_to_char(" Self",ch);
	if (IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" Other",ch);
	if (!IS_SET(ch->spectype,EYE_SPELL) &&
	    !IS_SET(ch->spectype,EYE_SELFACTION) &&
	    !IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" None",ch);
	send_to_char(".\n\r",ch);
    }
    return;
}

void do_artifact( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !IS_SET(obj->quest, QUEST_ARTIFACT) ) continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "There are no artifacts in the game.\n\r", ch );

    return;
}

void do_locate( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || obj->questowner == NULL || 
	    strlen(obj->questowner) < 2 || str_cmp( ch->pcdata->switchname, obj->questowner ))
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL
		    ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "You cannot locate any items belonging to you.\n\r", ch );

    return;
}

void do_claim( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( arg[0] == '\0' )
    {
	send_to_char( "What object do you wish to claim ownership of?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}

    if ( obj->item_type == ITEM_AMMO ||
        obj->item_type == ITEM_EGG   || obj->item_type == ITEM_VOODOO ||
        obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE ||
        obj->item_type == ITEM_PAGE )
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }
 else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }

    if ( obj->questowner != NULL && strlen(obj->questowner) > 1 )
    {
	if (!str_cmp(ch->pcdata->switchname,obj->questowner))
	    send_to_char("But you already own it!\n\r",ch);
	else
	    send_to_char("Someone else has already claimed ownership to it.\n\r",ch);
	return;
    }

    if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
       {
          send_to_char( "You can't claim artifacts.\n\r", ch );
          return;
       }

    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(ch->pcdata->switchname);
    act("You are now the owner of $p.",ch,obj,NULL,TO_CHAR);
    act("$n is now the owner of $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_gift( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Make a gift of which object to whom?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg1) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{send_to_char("Nobody here by that name.\n\r",ch); return;}

    if ( IS_NPC(victim) )
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }
    if ( obj->questowner == NULL || strlen(obj->questowner) < 2 )
    {
	send_to_char("That item has not yet been claimed.\n\r",ch);
	return;
    }
    if (str_cmp(ch->pcdata->switchname,obj->questowner))
    {
	send_to_char("But you don't own it!\n\r",ch);
	return;
    }

    if ( !IS_IMMORTAL( ch ) && ( obj->item_type == ITEM_PKTOKEN || obj->item_type == ITEM_DTOKEN 
         || obj->item_type == ITEM_QUEST || IS_SET( obj->quest, QUEST_RELIC ) ) )
	{
	stc("You can't gift one of these objects to someone else!\n\r",ch);
	return;
	}

    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(victim->pcdata->switchname);
    act("You grant ownership of $p to $N.",ch,obj,victim,TO_CHAR);
    act("$n grants ownership of $p to $N.",ch,obj,victim,TO_NOTVICT);
    act("$n grants ownership of $p to you.",ch,obj,victim,TO_VICT);
    return;
}

void do_create( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int itemtype = 13;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' )               itemtype = ITEM_TRASH;
    else if (!str_cmp(arg1,"light"    )) itemtype = ITEM_LIGHT;
    else if (!str_cmp(arg1,"scroll"   )) itemtype = ITEM_SCROLL;
    else if (!str_cmp(arg1,"wand"     )) itemtype = ITEM_WAND;
    else if (!str_cmp(arg1,"staff"    )) itemtype = ITEM_STAFF;
    else if (!str_cmp(arg1,"weapon"   )) itemtype = ITEM_WEAPON;
    else if (!str_cmp(arg1,"treasure" )) itemtype = ITEM_TREASURE;
    else if (!str_cmp(arg1,"armor"    )) itemtype = ITEM_ARMOR;
    else if (!str_cmp(arg1,"armour"   )) itemtype = ITEM_ARMOR;
    else if (!str_cmp(arg1,"potion"   )) itemtype = ITEM_POTION;
    else if (!str_cmp(arg1,"furniture")) itemtype = ITEM_FURNITURE;
    else if (!str_cmp(arg1,"trash"    )) itemtype = ITEM_TRASH;
    else if (!str_cmp(arg1,"container")) itemtype = ITEM_CONTAINER;
    else if (!str_cmp(arg1,"drink"    )) itemtype = ITEM_DRINK_CON;
    else if (!str_cmp(arg1,"key"      )) itemtype = ITEM_KEY;
    else if (!str_cmp(arg1,"food"     )) itemtype = ITEM_FOOD;
    else if (!str_cmp(arg1,"money"    )) itemtype = ITEM_MONEY;
    else if (!str_cmp(arg1,"boat"     )) itemtype = ITEM_BOAT;
    else if (!str_cmp(arg1,"corpse"   )) itemtype = ITEM_CORPSE_NPC;
    else if (!str_cmp(arg1,"fountain" )) itemtype = ITEM_FOUNTAIN;
    else if (!str_cmp(arg1,"pill"     )) itemtype = ITEM_PILL;
    else if (!str_cmp(arg1,"portal"   )) itemtype = ITEM_PORTAL;
    else if (!str_cmp(arg1,"egg"      )) itemtype = ITEM_EGG;
    else if (!str_cmp(arg1,"stake"    )) itemtype = ITEM_STAKE;
    else if (!str_cmp(arg1,"missile"  )) itemtype = ITEM_MISSILE;
    else                                 itemtype = ITEM_TRASH;

    if ( arg2[0] == '\0' || !is_number( arg2 ) )
    {
	level = 0;
    }
    else
    {
        level = atoi( arg2 );
	if ( level < 1 || level > 50 )
        {
	    send_to_char( "Level should be within range 1 to 50.\n\r", ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform KaVir.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    obj->level = level;
    obj->item_type = itemtype;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);

    act( "You reach up into the air and draw out a ball of protoplasm.", ch, obj, NULL, TO_CHAR );
    act( "$n reaches up into the air and draws out a ball of protoplasm.", ch, obj, NULL, TO_ROOM );
    return;
}

void do_token( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( IS_NPC(ch) || (ch->gold < 1 && !IS_JUDGE(ch)) )
    {
	send_to_char("You are unable to make a coin token.\n\r",ch);
	return;
    }
    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char("Please specify a value for the coin token.\n\r",ch);
	return;
    }
    else
    {
        value = atoi( arg1 );
	if ( value < 1 || value > 100000 )
        {
	    send_to_char( "Coin tokens should have a value between 1 and 100000.\n\r", ch );
	    return;
        }
	else if ( value > ch->gold && !IS_JUDGE(ch) )
        {
	    sprintf(buf,"You only have #w%d #ygold#n pieces left to put into tokens.\n\r",ch->pcdata->quest);
	    send_to_char( buf, ch );
	    return;
        }
    }

    victim = get_char_room( ch, arg2 );

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform Trace.\n\r",
ch );
	return;
    }

    ch->gold -= value;
    if (ch->gold < 0) ch->gold = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
    obj->item_type = ITEM_QUEST;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "coin token" );
    sprintf(buf,"a #w%d #ycoin #etoken#n",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d coin token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (victim != NULL && victim != ch)
    {
    	act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
    	act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
    	act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT );
    }
    else
    {
    	act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
    	act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );
    }
    return;
}

void do_ctoken( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( !IS_IMMORTAL(ch) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char("Please specify a value for the status token.\n\r",ch);
	return;
    }
    else
    {
        value = atoi( arg1 );
	if ( value < 1 || value > 100 )
        {
	    send_to_char( "Status tokens should have a value between 1 and 100.\n\r", ch );
	    return;
        }
    }

    victim = get_char_room( ch, arg2 );

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform Trace.\n\r",ch );
	return;
    }

    ch->gold -= value;
    if (ch->gold < 0) ch->gold = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
    obj->item_type = ITEM_DTOKEN;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "TP token" );
    sprintf(buf,"a #C%d #PTP #etoken#n",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d TP token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (victim != NULL && victim != ch)
    {
    	act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
    	act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
    	act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT );
    }
    else
    {
    	act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
    	act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );
    }
}

void do_pktoken( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( !IS_IMMORTAL(ch) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char("Please specify a value for the PK token.\n\r",ch);
	return;
    }
    else
    {
        value = atoi( arg1 );
	if ( value < 1 || value > 50 )
        {
	    send_to_char( "PK tokens should have a value between 1 and 50.\n\r", ch );
	    return;
        }
    }

    victim = get_char_room( ch, arg2 );

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform Trace.\n\r",ch );
	return;
    }

    ch->gold -= value;
    if (ch->gold < 0) ch->gold = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
    obj->item_type = ITEM_PKTOKEN;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "pk token" );
    sprintf(buf,"a #P%d #CPK #ytoken#n",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d PK token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (victim != NULL && victim != ch)
    {
    	act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
    	act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
    	act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT );
    }
    else
    {
    	act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
    	act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );
    }
}

void do_qtrust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: qtrust <char> <on/off>.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "Do you wish to set qtrust ON or OFF?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (str_cmp(arg2,"on") && str_cmp(arg2,"off"))
    {
	send_to_char( "Do you want to set their qtrust ON or OFF?\n\r", ch);
	return;
    }
    if (!str_cmp(arg2,"off"))
    {
	if (!IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their qtrust is already off.\n\r",ch);
	    return;
	}
	REMOVE_BIT(victim->extra, EXTRA_TRUSTED);
	send_to_char("Quest trust OFF.\n\r",ch);
	send_to_char("You are no longer quest trusted.\n\r",victim);
	return;
    }
    else if (!str_cmp(arg2,"on"))
    {
	if (IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their qtrust is already on.\n\r",ch);
	    return;
	}
	SET_BIT(victim->extra, EXTRA_TRUSTED);
	send_to_char("Quest trust ON.\n\r",ch);
	send_to_char("You are now quest trusted.\n\r",victim);
	return;
    }
    return;
}

/*
void do_ntrust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: ntrust <char> <on/off>.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "Do you wish to set ntrust ON or OFF?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (str_cmp(arg2,"on") && str_cmp(arg2,"off"))
    {
	send_to_char( "Do you want to set their ntrust ON or OFF?\n\r",ch);
	return;
    }
    if (!str_cmp(arg2,"off"))
    {
	if (!IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their ntrust is already off.\n\r",ch);
	    return;
	}
	REMOVE_BIT(victim->extra, EXTRA_NOTE_TRUST);
	send_to_char("Note trust OFF.\n\r",ch);
	send_to_char("You are no longer note trusted.\n\r",victim);
	return;
    }
    else if (!str_cmp(arg2,"on"))
    {
	if (IS_EXTRA(victim, EXTRA_NOTE_TRUST))
	{
	    send_to_char("Their ntrust is already on.\n\r",ch);
	    return;
	}
	SET_BIT(victim->extra, EXTRA_NOTE_TRUST);
	send_to_char("Note trust ON.\n\r",ch);
	send_to_char("You are now note trusted.\n\r",victim);
	return;
    }
    return;
}
*/

void do_mclear( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: mclear <char>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (IS_NPC(victim))
    {
	send_to_char( "Not on NPC's.\n\r", ch);
	return;
    }

    for ( obj = victim->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->wear_loc != WEAR_NONE ) 
	{obj_from_char(obj); obj_to_char(obj,victim);}
    }
    while ( victim->affected )
	affect_remove( victim, victim->affected );
    if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    {
    	victim->affected_by	= AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    	victim->affected_by	= AFF_POLYMORPH;
    else if (IS_AFFECTED(victim,AFF_ETHEREAL))
    	victim->affected_by	= AFF_ETHEREAL;
    else
    	victim->affected_by	= 0;
    victim->armor	 = 100;
    victim->hit		 = UMAX( 1, victim->hit  );
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->pcdata->mod_str = 0;
    victim->pcdata->mod_int = 0;
    victim->pcdata->mod_wis = 0;
    victim->pcdata->mod_dex = 0;
    victim->pcdata->mod_con = 0;

    if ( IS_CLASS(victim, CLASS_DRAGON) )
       victim->powerlevel = 0;

    save_char_obj( victim );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    return;
}

void do_clearstats( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    if (IS_NPC(ch)) return;

    if ((ch->in_room == NULL || ch->in_room->vnum != 800))
    {
	send_to_char("You can only clear your stats at the Centre of the Crystal Cathedral.\n\r",ch);
	return;
    }

    if ( ch->fight_timer > 0 )
       {
          stc( "Not with a fight timer!\n\r", ch );
          return;
       }

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->wear_loc != WEAR_NONE ) 
	{obj_from_char(obj); obj_to_char(obj,ch);}
    }
    
    while ( ch->affected )
      affect_remove( ch, ch->affected );

    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    if (IS_SET(ch->affected_by, AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

    ch->affected_by	 = 0;
    ch->armor		 = 100;
    ch->hit		 = UMAX( 1, ch->hit  );
    ch->mana		 = UMAX( 1, ch->mana );
    ch->move		 = UMAX( 1, ch->move );
    ch->hitroll		 = 0;
    ch->damroll		 = 0;
    ch->saving_throw	 = 0;
    ch->pcdata->mod_str	 = 0;
    ch->pcdata->mod_int	 = 0;
    ch->pcdata->mod_wis	 = 0;
    ch->pcdata->mod_dex	 = 0;
    ch->pcdata->mod_con	 = 0;
    ch->itemaffect       = 0;

    if ( IS_SET(ch->special, SPC_WOLFMAN) )
       {
          ch->rage_timer = 0;
          do_unwerewolf( ch, "" );
       }
       else ch->rage = 0;

    if ( IS_CLASS(ch, CLASS_DRAGON) )
       ch->dragonform = 0;

    if (IS_SET(ch->newbits, NEW_DFORM)) REMOVE_BIT(ch->newbits, NEW_DFORM);
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

void do_otransfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Otransfer which object?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' ) victim = ch;
    else if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || 
	str_cmp(ch->name,obj->questmaker) || strlen(obj->questmaker) < 2))
    {
	send_to_char("You don't have permission to otransfer that item.\n\r", ch);
	return;
    }

    if (obj->carried_by != NULL)
    {
	act("$p vanishes from your hands in an explosion of energy.",obj->carried_by,obj,NULL,TO_CHAR);
	act("$p vanishes from $n's hands in an explosion of energy.",obj->carried_by,obj,NULL,TO_ROOM);
	obj_from_char(obj);
    }
    else if (obj->in_obj     != NULL) obj_from_obj(obj);
    else if (obj->in_room != NULL)
    {
    	chroom = ch->in_room;
    	objroom = obj->in_room;
    	char_from_room(ch);
    	char_to_room(ch,objroom);
    	act("$p vanishes from the ground in an explosion of energy.",ch,obj,NULL,TO_ROOM);
	if (chroom == objroom) act("$p vanishes from the ground in an explosion of energy.",ch,obj,NULL,TO_CHAR);
    	char_from_room(ch);
    	char_to_room(ch,chroom);
	obj_from_room(obj);
    }
    else
    {
	send_to_char( "You were unable to get it.\n\r", ch );
	return;
    }
    obj_to_char(obj,victim);
    act("$p appears in your hands in an explosion of energy.",victim,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in an explosion of energy.",victim,obj,NULL,TO_ROOM);
    return;
}

void bind_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *location;

    if ( IS_NPC(ch) || ch->pcdata->obj_vnum < 1 )
	return;

    if ( ( pObjIndex = get_obj_index( ch->pcdata->obj_vnum ) ) == NULL )
	return;

    if ( ch->in_room == NULL || ch->in_room->vnum == ROOM_VNUM_IN_OBJECT )
    {
	location = get_room_index(ROOM_VNUM_ALTAR);
    	char_from_room(ch);
    	char_to_room(ch,location);
    }
    else location = ch->in_room;

    obj = create_object( pObjIndex, 50 );
    obj_to_room(obj,location);
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    send_to_char( "You reform yourself.\n\r", ch );
    act("$p fades into existance on the floor.",ch,obj,NULL,TO_ROOM);
    do_look(ch,"auto");
    return;
}

void do_bind( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: bind <player> <object>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    if (ch == victim)
    {
	send_to_char( "You can't do this to yourself.\n\r", ch);
	return;
    }
    if (IS_NPC(victim))
    {
	send_to_char( "Not on NPC's.\n\r", ch);
	return;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    {
	send_to_char( "You cannot do this while they are polymorphed.\n\r", ch);
	return;
    }
    else if (IS_IMMORTAL(victim))
    {
	send_to_char( "Only on mortals or avatars.\n\r", ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg2) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if (obj->chobj != NULL)
    {
	send_to_char( "That item already has someone bound in it.\n\r", ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    act("$n transforms into a white vapour and pours into $p.",victim,obj,NULL,TO_ROOM);
    act("You transform into a white vapour and pour into $p.",victim,obj,NULL,TO_CHAR);
    victim->pcdata->obj_vnum = obj->pIndexData->vnum;
    obj->chobj = victim;
    victim->pcdata->chobj = obj;
    SET_BIT(victim->affected_by, AFF_POLYMORPH);
    SET_BIT(victim->extra, EXTRA_OSWITCH);
    free_string(victim->morph);
    victim->morph = str_dup(obj->short_descr);
    return;
}

void do_contraception( CHAR_DATA *ch, char *argument)
{
   if(IS_NPC(ch)) return;
   if( ch->sex != SEX_FEMALE) return;
    if( IS_SET(ch->affected_by2, AFF_CONTRACEPTION))
    {
	stc( "You remove your contraception.\n\r", ch);
	REMOVE_BIT(ch->affected_by2, AFF_CONTRACEPTION);
	return;
    }
    else
    {
	stc( "You use your contraception.\n\r", ch);
	SET_BIT(ch->affected_by2, AFF_CONTRACEPTION);
	return;
    }
    return;
}

/*----------------------------------------------------------------
 * functions for RELOAD command.  --Zarniwoop@Dutch Mountains  (4-28-97)
 * Snippet intended for Merc 2.0, 2.1 and 2.2 
 *
 * ready to go, except for two things:
 * 1. make the usual modifications for do_reload (merc.h, interp.c)
 * 2. in merc.h, after the declaration of help_first, declare help_last
 *    so it is global.
 *
 * Bugs, comments: remmelt@kosterix.icce.rug.nl
 */
bool free_helps( CHAR_DATA *ch, char *arg )
{
    char buf[MAX_STRING_LENGTH];
    HELP_DATA *h, *h_next;
    HELP_DATA *prev = NULL;
    bool found = FALSE;

    prev = help_first;
    for ( h = help_first; h; h = h_next )
    {
	h_next = h->next;

        if ( !str_cmp( h->keyword, arg ) )
        {
            free_string( h->keyword );
            free_string( h->text );
            if ( h == help_first )
                help_first = h->next;
            else
            prev->next = h->next;
            if ( h == help_last )
               help_last = prev;
            free_mem( h, sizeof(*h) );
            found = TRUE;
        }
        prev = h;
    }

    if ( !found )
    {
        sprintf( buf, "Help entry %s not found.\n\r", arg );
        send_to_char( buf, ch );
        return FALSE;
    }

    return TRUE;
}


bool read_entry( CHAR_DATA *ch, FILE *fp, char *filename, char *arg )
{
    char buf[MAX_STRING_LENGTH];
    HELP_DATA *new;
    HELP_DATA *debug;
    char *test_keyword = 0;
    char *test_text = 0;
    int test_level = 0;

    if ( fread_letter( fp ) != '#' )
    {
        sprintf( buf, "read_entry: # not in %s.\n\r", filename );
        send_to_char( buf, ch );
        return FALSE;
    }

    fread_word( fp ); /* read the word HELPS */

    new          = alloc_mem( sizeof(*new) );
    new->next    = NULL;

    for ( ; ; )
    {
        test_level   = fread_number( fp );
        test_keyword = fread_string( fp );

        if ( !str_cmp( test_keyword, "$" )) /* end of file */
        {
            free_string( test_keyword );
            free_string( test_text );
            free_mem( new, sizeof(*new) );
            return FALSE;
        }

        test_text    = fread_string( fp );

        if ( !str_cmp( test_keyword, arg ))  /* match */
        {
            new->keyword = test_keyword;
            new->level   = test_level;
            new->text    = test_text;

            debug = help_last;
            if ( help_last ) 
                help_last->next = new;
            help_last = new;
            
            sprintf( buf, "entry %s updated.\n\r", new->keyword );
            send_to_char( buf, ch );
            return TRUE;
        }
        else
        {
            free_string( test_keyword );
            free_string( test_text );
        }
    }

    return FALSE;
}


void do_hreload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
    bool found = FALSE;
    int i;

    /* in order of 'importance' */
    char *helpfile[10] = { "../area/help.are", "", "", "", "", "", "", "", "", "" };    

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Usage: reload <help-keyword>\n\r", ch );
        return;
    }

    /* remove help entry from the list */
    if ( !free_helps( ch, arg ))
        return;

    /* here we go */
    fclose( fpReserve );

    for ( i = 0; i < 9; i++ )
    {
        if ( ( fp = fopen( helpfile[i], "r" ) ) == NULL )
        {
            sprintf( buf, "do_reload: couldn't open %s.\n\r", helpfile[i] );
            send_to_char( buf, ch );
        }
        else
        {
            if ( ( found = read_entry( ch, fp, helpfile[i], arg ) ) == TRUE )
            {
                fclose( fp );
                break;
            }
            fclose( fp );
        }
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

static char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
};

#define MAX_SPECTATORS 100

bool check_airmail_dir(CHAR_DATA* ch, ROOM_INDEX_DATA* to_room, EXIT_DATA* pexit)
{
	   /* impassable door*/
return !(IS_SET(pexit->exit_info, EX_CLOSED) && 
	   (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))) ||
	   /* unenterable room */
       (room_is_private(to_room)) ||
	   /* elemental difficulties.. */
       ((to_room->sector_type == SECT_AIR ||
         to_room->sector_type == SECT_WATER_NOSWIM)
         && !IS_AFFECTED(ch,AFF_FLYING));
}

void airmail_damage(CHAR_DATA* ch, CHAR_DATA* victim, int num)
{
	/* Make the damage depend on the force of the throw and how far flown */
	int amount = number_range(5 * victim->size, 5 * victim->size * (num + 1));

	WAIT_STATE(ch, PULSE_VIOLENCE);
	damage(ch,victim, amount, TYPE_UNDEFINED);
}

void airmail_char(CHAR_DATA *ch, CHAR_DATA *victim, int door, CHAR_DATA* lst[], int* spectator_count, int num)
{
   ROOM_INDEX_DATA *in_room;
   ROOM_INDEX_DATA *to_room = NULL;
   EXIT_DATA *pexit;
   CHAR_DATA* target;
   int s;

   in_room = victim->in_room;
   pexit = in_room->exit[door];

   if(pexit)
   {
	   to_room = pexit->to_room;
   }

   /* End of the line */
   if(num <= 0)
   {
	   /* It didn't go far..  */
	  if(in_room == ch->in_room)
	  {
		  act("With a mighty heave you throw $M $T.", ch, victim, dir_name[door], TO_CHAR);
		  act("With a mighty heave $n throws $N into the air!", ch, NULL, victim, TO_ROOM);
	  }

	  act("$n lands in an unceremonious heap.", 
		  victim, NULL, NULL, TO_ROOM);
	  act("You hear a confused racket as $N completes $S erstwhile journey.", 
		  ch, NULL, victim, TO_CHAR);
      send_to_char("Your journey is suddenly arrested by the floor.\n\r", victim);

	  /* Keep the spectators informed.. */
	  for(s = 0; s < *spectator_count; s++)
	  {
		  act("You hear a confused racket as $N completes $S erstwhile journey.", 
			  lst[s], NULL, victim, TO_CHAR);
	  }

	  airmail_damage(ch, victim, num);
   }
   else if(pexit == NULL || to_room == NULL)
   {
	   /* It didn't go far..  */
	  if(in_room == ch->in_room)
	  {
		  act("With a mighty heave you throw $M $T.", ch, victim, dir_name[door], TO_CHAR);
		  act("With a mighty heave $n throws $N into the air!", ch, NULL, victim, TO_ROOM);
	  }

	  act("$n crashes into the wall and lands in a disgruntled heap.", victim, NULL, NULL, TO_ROOM);
      send_to_char("*CRUNCH* is the sound of you bouncing.\n\r", victim);

	  if(in_room == ch->in_room)
	  {
		  act("*CRUNCH* is the sound of $N bouncing.", ch, NULL, victim, TO_CHAR);
	  }
	  else
	  {
		  act("You hear a confused racket as $N completes $S erstwhile journey.", 
			  ch, NULL, victim, TO_CHAR);

		  /* Keep the spectators informed.. */
		  for(s = 0; s < *spectator_count; s++)
		  {
			  act("You hear a confused racket as $N completes $S erstwhile journey.", 
				  lst[s], NULL, victim, TO_CHAR);
		  }
	  }

	  airmail_damage(ch, victim, num);
   }
   else if(!check_airmail_dir(ch, to_room, pexit))
   {
	  act("$n gets a physics lesson from the $d and lands in a disgruntled heap.", victim, NULL,
pexit->keyword, TO_ROOM);
      act("OWCH! is the sound of you missing the $d.", victim, NULL, pexit->keyword, TO_CHAR);

	  if(in_room == ch->in_room)
	  {
		  act("Ooooh!  With a neato crunch $N bounced!", ch, NULL, victim, TO_CHAR);
	  }
	  else
	  {
		  act("You hear a confused racket as $N completes $S erstwhile journey.", 
			  ch, NULL, victim, TO_CHAR);

		  /* Keep the spectators informed.. */
		  for(s = 0; s < *spectator_count; s++)
		  {
			  act("You hear a confused racket as $N completes $S erstwhile journey.", 
				  lst[s], NULL, victim, TO_CHAR);
		  }
	  }

	  airmail_damage(ch, victim, num);
   }
   else 
   {
      if(ch->fighting != NULL)
      {
         stop_fighting(ch, TRUE);
         if(ch->position == POS_FIGHTING)
         {
            ch->position = POS_STANDING;
         }
      }

	  /* only way to tell if this is the initial throw or not */
	  if(in_room == ch->in_room)
	  {
		  act("With a mighty heave you throw $M $T.", ch, victim, dir_name[door], TO_CHAR);
		  act("With a mighty heave $n throws $N into the air!", ch, NULL, victim, TO_ROOM);

		  /* First time through?  Add spectators in this room */
		  target = in_room->people;
		  while(target && *spectator_count < MAX_SPECTATORS)
		  {
			  if(target != ch && target != victim)
			  {
				lst[(*spectator_count)++] = target;
			  }

			  target = target->next_in_room;
		  }
	  }
	  else
	  {
		  /* This one to the player only, to cut down on spam  */
		  act("You hear swooshing noises to the $T.", ch, victim, dir_name[door], TO_CHAR);
	  }


      act("$n cuts a perfect arc as $E sails $T.", victim, NULL, dir_name[door], TO_ROOM);
      char_from_room(victim);
      char_to_room(victim, to_room);

	  act("In a swoosh of air $n sails in looking quite confused.", victim, NULL, NULL, TO_ROOM);
      do_look(victim, "auto");

	  /* Figure collisions */
	  target = to_room->people;
	  while(target)
	  {
		  /* Hm. */
		  if(target == victim)
		  {
			  target = target->next_in_room;
			  continue;
		  }

		  if((target->size + victim->size) * 10 > number_range(0, 200))
		  {
			  /* They collide, and a fight ensues */
			  act("The $n smashes headlong into $N!", victim, NULL, target, TO_ROOM);
			  act("You smash into $N!", victim, NULL, target, TO_CHAR);
			  act("$n crashes into you!", victim, NULL, target, TO_VICT);
			  act("There is a confused shriek as the $N crashes into someone.", ch, NULL, victim,
TO_CHAR);

			  /* Keep the spectators informed.. */
			  for(s = 0; s < *spectator_count; s++)
			  {
				  act("There is a confused shriek as the $N crashes into someone.", lst[s], NULL,
victim, TO_CHAR);
			  }

			  airmail_damage(victim, target, num);
			  airmail_damage(target, victim, num);
			  break;
		  }
		  
		  target = target->next_in_room;
	  }

	  if(!target)
	  {
  		  /* Add spectators in this room */
		  target = to_room->people;
		  while(target && *spectator_count < MAX_SPECTATORS)
		  {
			  if(target != ch && target != victim)
			  {
				lst[(*spectator_count)++] = target;
			  }
			  target = target->next_in_room;
		  }

		  /* recurse to keep going */
		  airmail_char(ch, victim, door, lst, spectator_count, num - 1);
	  }
   }

   return;
}

void do_airmail(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim = NULL;
	char* tmp;
	int door;
	int force = 3;
	CHAR_DATA* lst[MAX_SPECTATORS];
	int spectator_count = 0;

	/* Get a victim */
    tmp = one_argument(argument,arg);
    victim = get_char_room(ch, arg);
	if (victim == NULL)
	{
		send_to_char("Um, airmail -who-?\n\r",ch);
		return;
	}

	/* Figure out how far we can throw this sucker */
	/* Heavily weighted for physical size */
	force = URANGE(0, ch->pcdata->perm_str / 2, 10 );

	/* Heh, not very far at all.. */
	if(force <= 0 || !IS_IMMORTAL(ch))
	{
		/* Oops, ya done pissed 'em off */
		send_to_char("You grunt and heave but they refuse to budge!\r\n", ch);
		return;
	}

	/* Get a direction or pick one.. */
    one_argument(tmp, arg);
	if(arg[0] == '\0') door = number_door();
    if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;	
	else door = number_door();

	airmail_char(ch, victim, door, lst, &spectator_count, force);
}

void do_showchar( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *wch;
    int count = 0;
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if( IS_NPC(wch)) continue;
	sprintf( buf, "Name: %s.\n\r",wch->name);
	stc( buf, ch);
	count++;
    }
	sprintf( buf,"%d PCs found.\n\r",count);
	stc(buf,ch);
    return;
}

void do_resetpassword( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *pwdnew;

    if ( IS_NPC(ch) )
    return;

    argument=one_argument( argument, arg1 );
    argument=one_argument( argument, arg2 );

    victim = get_char_world(ch, arg1);

    if (  ( ch->pcdata->pwd != '\0' )
    && ( arg1[0] == '\0' || arg2[0] == '\0')  )
    {
        send_to_char( "Syntax: password <char> <new>.\n\r", ch );
        return;
    }
    if( victim == '\0' )
    {
		send_to_char( "That person isn't here, they have to be here to reset pwd's.\n\r", ch);
	 	return;
    }
    if ( IS_NPC( victim ) )
    {
		send_to_char( "You cannot change the password of NPCs!\n\r",ch);
		return;
    }

    if (( victim->level > LEVEL_IMMORTAL ) || ( get_trust(victim) > LEVEL_IMMORTAL ))
	
	{
		send_to_char( "You can't change the password of that player.\n\r",ch);
		return;
    }
    
    if ( strlen(arg2) < 5 )
    {
        send_to_char( "New password must be at least five characters long.\n\r", ch );
        return;
    }
	
    pwdnew = crypt( arg2, victim->name );
    free_string( victim->pcdata->pwd );
    victim->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( victim );
    send_to_char( "Ok.\n\r", ch );
    sprintf( buf, "Your password has been changed to %s.", arg2 );
    send_to_char( buf, victim);
    return;
}

#define CH(descriptor)  ((descriptor)->original ? (descriptor)->original : (descriptor)->character)
#define COPYOVER_FILE "copyover.data"

/* This is the executable file */
#define EXE_FILE	  "../src/starwars"

extern int port,control; /* db.c */

void do_copyover (CHAR_DATA *ch, char * argument)
{
	FILE *fp;
	DESCRIPTOR_DATA *d, *d_next;
      extern int port,control; /* db.c */
	char buf[100], buf2[100];
      OBJ_DATA *obj, *obj_next;

	fp = fopen (COPYOVER_FILE, "w");

	if (!fp)
	{
		send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
		perror ("do_copyover:fopen");
		return;
	}

      do_restore(ch,"all");
	
	
    sprintf (buf, "\n\r <*>         It is a time of changes         <*>\n\r");	

	/* For each playing descriptor, save its state */
	for (d = descriptor_list; d ; d = d_next)
	{
		CHAR_DATA * och = CH (d);
		d_next = d->next; /* We delete from the list , so need to save this */
	
		if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
		{
			write_to_descriptor (d, "\n\rSorry, we are rebooting. Come back in 30 seconds.\n\r", 0);
			close_socket (d); /* throw'em out */
		}
		else
		{
                  if ( d->out_compress )
                     compressEnd( d );

                  fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
                  call_all(och);

                  for ( obj = och->carrying; obj != NULL; obj = obj_next )
                  {
                     obj_next = obj->next_content;

                     if ( !IS_SET( obj->quest, QUEST_ARTIFACT ) )
                        continue;
  
                     obj_from_char( obj );
                     obj_to_room( obj, och->in_room );
                  }

                  save_char_obj( och );
		}
	}
	
	fprintf (fp, "-1\n");
	fclose (fp);
	
	/* Close reserve and other always-open files and release other resources */
	
	fclose (fpReserve);

	sprintf (buf, "%d", port);
	sprintf (buf2, "%d", control);
	execl (EXE_FILE, "starwars", buf, "copyover", buf2, (char *) NULL);
	
	/* Failed - sucessful exec will not return */
	
	perror ("do_copyover: execl");
	send_to_char ("Copyover FAILED!\n\r",ch);
}

void copyover ( )
{
	FILE *fp;
	DESCRIPTOR_DATA *d, *d_next;
      extern int port,control; /* db.c */
	char buf[100], buf2[100];
      OBJ_DATA *obj, *obj_next;

        if ( ( fp = fopen(LAST_COMMAND,"a") ) == NULL )
           bug("Error in do_auto_save opening last_command.txt",0);
   
        fprintf(fp,"Last Command: %s\n", last_command);
        fclose( fp );

	fp = fopen (COPYOVER_FILE, "w");

	if (!fp)
	{
		perror ("do_copyover:fopen");
		return;
	}

	/* For each playing descriptor, save its state */
	for (d = descriptor_list; d ; d = d_next)
	{
		CHAR_DATA * och = CH (d);
		d_next = d->next; /* We delete from the list , so need to save this */
	
		if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
		{
			write_to_descriptor (d, "\n\rSorry, we are rebooting. Come back in 30 seconds.\n\r", 0);
			close_socket (d); /* throw'em out */
		}
		else
		{
                  if ( d->out_compress )
                     compressEnd( d );

                  fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
                  call_all(och);

                  for ( obj = och->carrying; obj != NULL; obj = obj_next )
                  {
                     obj_next = obj->next_content;

                     if ( !IS_SET( obj->quest, QUEST_ARTIFACT ) )
                        continue;
  
                     obj_from_char( obj );
                     obj_to_room( obj, och->in_room );
                  }

                  restore_char( och );
                  save_char_obj( och );
		}
	}
	
	fprintf (fp, "-1\n");
	fclose (fp);
	
	/* Close reserve and other always-open files and release other resources */
	
	fclose (fpReserve);
	
	sprintf (buf, "%d", port);
	sprintf (buf2, "%d", control);
	execl (EXE_FILE, "starwars", buf, "copyover", buf2, (char *) NULL);
}

/* Recover from a copyover - load players */
void copyover_recover ()
{
	DESCRIPTOR_DATA *d;
	FILE *fp;
	char name [100];
	char host[MSL];
	int desc;
	bool fOld;
	
	fp = fopen (COPYOVER_FILE, "r");
	
	if (!fp) /* there are some descriptors open which will hang forever then ? */
	{
		perror ("copyover_recover:fopen");
		exit (1);
	}

	unlink (COPYOVER_FILE); /* In case something crashes - doesn't prevent reading	*/
	
	for (;;)
	{
		fscanf (fp, "%d %s %s\n", &desc, name, host);
		if (desc == -1)
			break;

		d = alloc_perm (sizeof(DESCRIPTOR_DATA));
		init_descriptor (d,desc); /* set up various stuff */
		
		d->host = str_dup (host);
		d->next = descriptor_list;
		descriptor_list = d;
		d->connected = CON_COPYOVER_RECOVER; /* -15, so close_socket frees the char */
	
            fOld = load_char_obj (d, name);
                d->character->login_timer = 0;

		if (!fOld) /* Player file not found?! */
		{
			write_to_descriptor (d, "\n\r#wYou didn't kill enough mobs so your pfile wasn't saved!#n\n\r", 0);
			close_socket (d);			
		}
		else /* ok! */
		{
                  /* Just In Case */
			if (!d->character->in_room)
				d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);

			/* Insert in the char_list */
			d->character->next = char_list;
			char_list = d->character;

                  if ( IS_CLASS(d->character,CLASS_DRAGON) && d->character->dragonform > 0 )
                     check_dragon_form( d->character, "" );

                  if ( IS_SET( d->character->arena, AFF2_COMPETITOR ) )
                     REMOVE_BIT( d->character->arena, AFF2_COMPETITOR );

                  if ( IS_SET( d->character->extra, EXTRA_HUNT ) )
                     REMOVE_BIT( d->character->extra, EXTRA_HUNT );

			char_to_room (d->character, d->character->in_room);

			d->connected = CON_PLAYING;
                  stc ( "#wThe #Csky #ygoes #edark #yas a #w#zCopyover#n #yor #R#zCrash#n #ytook place!#n\n\r", d->character );
                  stc ( "#cYour body #Lflows #cwith the #wblessings #cof the #PGods #cas you are faced with more challenges!#n.\n\r", d->character );
			act ("#w$n #ghave #w#zflickered#n#g right before your #Peyes.#n", d->character, NULL, NULL, TO_ROOM);
		}
		
	}
	
	fclose (fp);
}

void do_implag(CHAR_DATA *ch, char *argument)
{
   char      arg[MAX_INPUT_LENGTH];
   char      buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument(argument, arg);

   if (arg[0] == '\0')
      {
      send_to_char("Who you wanna lag?\n\r", ch);
      return;
      }

	if ((victim = get_char_world( ch, arg ) )==NULL )
      {
      send_to_char("They aren't playing you dufus!\n\r", ch);
      return;
      }
   else
      {
      if (IS_NPC(victim))
         {
         send_to_char("Not on NPC's!\n\r", ch);
         return;
         }
      else if (IS_AFF2(victim, PLR_IMPLAG))
         {
         REMOVE_BIT(victim->affected_by2, PLR_IMPLAG);
         sprintf(buf, "You make %s's life better.\n\r", victim->name);
         send_to_char(buf, ch);
         sprintf(buf, "You suddenly look at the world normally.\n\r");
         send_to_char(buf, victim);
         return;
         }
      else
         {
         SET_BIT(victim->affected_by2, PLR_IMPLAG);
         sprintf(buf, "You make %s's life totally suck.\n\r", victim->name);
         send_to_char(buf, ch);
         return;
         }
      }
}
void do_gflags( CHAR_DATA *ch, char *argument )
{
int temp = doxlimit;
char buf[MIL];
	sprintf(buf, "Paradoxlimit is %d\n\r", 10+temp);
	send_to_char(buf, ch);
    if (global_exp)
	send_to_char("Double exp is ON!\n\r",ch);
	else
	send_to_char("Double exp is off\n\r",ch);
    if (global_qp)
	send_to_char("All Gold is ON!\n\r",ch);
	else
	send_to_char("All Gold is off\n\r",ch);
     sprintf(buf, "number of players logged on this copyover: %d\n\r", players_logged);
	send_to_char(buf, ch);
return;
} 

bool is_sound( CHAR_DATA *ch )
{
   if ( IS_NPC(ch) )
      return FALSE;

   if ( !IS_SET( ch->p_bit, P_MUSIC ) )
      return FALSE;

   return TRUE;
}

void do_sound( CHAR_DATA *ch, char *argument )
{
    char arg[MIL];

    argument = one_argument( argument, arg );

    if ( !str_cmp(arg, "on") )
       {
                if ( IS_SET( ch->p_bit, P_MUSIC ) )
                   {
                      send_to_char("Sound/Music is already in affect!\n\r",ch);
                      return;
                   }

                SET_BIT( ch->p_bit, P_MUSIC );

                send_to_char("You can now hear Sound and Music!\n\r",ch);
                return;
       }

    if ( !str_cmp(arg, "off") )
       {
                stc("!!MUSIC(Off)\n\r",ch);
                if ( !IS_SET( ch->p_bit, P_MUSIC ) )
                   {
                      send_to_char("Sound/Music is already off!\n\r",ch);
                      return;
                   }

                REMOVE_BIT( ch->p_bit, P_MUSIC );

                send_to_char("You can no longer hear Sound and Music!\n\r",ch);
                return;
       }

    send_to_char("Syntax: sound <on|off>.\n\r",ch);
}               

void do_doublexp( CHAR_DATA *ch, char *argument )
{
    char arg[MIL];
    char buf[MSL];
    char arg2[MIL];             

    argument = one_argument( argument, arg );
    argument = one_argument(argument, arg2);

    if (!strcmp(argument, "qp") && !strcmp(argument, "exp") && !strcmp(argument, "extra_log"))
        {
            send_to_char("Syntax: double <qp|exp> <on|off>.\n\r",ch);
	      send_to_char("Syntax: double extra_log on/off.\n\r",ch);
	      return;
        }      

    if (!str_cmp(arg, "exp"))
    {
      if (!str_cmp(arg2, "on"))  
	{
	if (global_exp)
        {
            send_to_char("Double exp is already in affect!\n\r",ch);
            return;
        }
        global_exp = TRUE;
        sprintf( buf, "An Immortal has declared double exp for everyone." );
	do_info( ch, buf );

        send_to_char("Double exp is now in affect!\n\r",ch);
        return;
    }
    if (!str_cmp(arg2, "off"))
    {
        if (!global_exp)
        {
            send_to_char("The global exp flag isn't on!.\n\r",ch);
            return;
        }
        global_exp = FALSE; 
        sprintf(buf, "An Immortal has removed double experience!" );
	do_info( ch, buf );
        send_to_char( "The double exp flag is now off!\n\r", ch );  
        return;
    }}
   if (!str_cmp(arg, "extra_log"))
    {
      if (!str_cmp(arg2, "on"))
        {
        if (extra_log==TRUE)
        {
            send_to_char("Already on!\n\r",ch);
            return;
        }
        extra_log = TRUE;
        send_to_char("Logging as a little bastard!\n\r",ch);
        return;
      }
     if (!str_cmp(arg2, "off"))
        {
        if (extra_log==FALSE)
        {
            send_to_char("Already of!\n\r",ch);
            return;
        }
        extra_log = FALSE;
        send_to_char("OK, stopped logging!\n\r",ch);
        return;
      }
    }
    if (!str_cmp(arg, "qp"))
    {
      if (!str_cmp(arg2, "on"))  
	{
	if (global_qp==TRUE)
        {
            send_to_char("All Gold is already in affect!\n\r",ch);
            return;
        }
        global_qp = TRUE;
        sprintf( buf, "An Immortal has declared #yALL gold#n for everyone." );
	do_info( ch, buf );

        send_to_char("All gold is now in affect!\n\r",ch);
        return;
    }
    if (!str_cmp(arg2, "off"))
    {
        if (global_qp==FALSE)
        {
            send_to_char("The All gold flag isn't on!.\n\r",ch);
            return;
        }
        global_qp = FALSE; 
        sprintf(buf, "An Immortal has removed #yAll gold#n!" );
	do_info( ch, buf );
        send_to_char( "The All gold flag is now off!\n\r", ch );  
        return;
    }}

send_to_char( "Syntax: double <exp|qp> <on|off>\n\r", ch );
send_to_char( "Syntax: double doxlimit <number>\n\r", ch);
}               

void do_setdisciplines(CHAR_DATA *ch, char *argument){    CHAR_DATA *victim;    char
arg1[MAX_STRING_LENGTH];    char arg2[MAX_STRING_LENGTH];    char arg3[MAX_STRING_LENGTH];    int val;
int i;    argument = one_argument(argument, arg1);    argument = one_argument(argument, arg2);
argument = one_argument(argument, arg3);    if (arg1[0] == '\0')    {	
send_to_char("Set whose disciplines?\n\r",ch);	return;    }    if ((victim = get_char_world(ch, arg1)) == NULL)    {
send_to_char("They aren't here.\n\r",ch);	return;    }    val = atoi(arg3);    if (val < -2 || val
> 10)    {	send_to_char("Values between -2 and 10 please.\n\r",ch);	return;    }    if
(!str_cmp(arg2, "all"))    {	for (i = 0; i < MAX_DISCIPLINES; i++)	    victim->power[i] = val;
send_to_char("All disciplines set.\n\r",ch);	return;    }    for (i = 0; i < MAX_DISCIPLINES; i++)
{	if (!str_prefix(arg2, discipline[i]))	{	    send_to_char("Discipline set.\n\r",ch);
victim->power[i] = val;	    return;	}    }    send_to_char("No such discipline.\n\r",ch);}

