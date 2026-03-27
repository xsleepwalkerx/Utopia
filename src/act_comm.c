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
#include "merc.h"


/*
 * Local functions.
 */
void	talk_channel	args( ( CHAR_DATA *ch, char *argument,
			    int channel, const char *verb ) );
void remove_group_auras args( ( CHAR_DATA *ch ) );
bool is_in		args( (char *, char *) );
bool all_in		args( (char *, char *) );
char * socialc		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );
char * socialv		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );
char * socialn		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );

void game_in_play(CHAR_DATA *ch, char *argument);

bool in_kingdom( CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *pRoomIndex )
{
   if ( IS_NPC(ch) )
      return FALSE;

   if ( pRoomIndex->vnum < 4 )
      {
          stc("You can't use this ability to get access to that room.\n\r",ch);
          return TRUE;
      }

   if ( pRoomIndex->vnum >= 550 && pRoomIndex->vnum <= 629 )
      {
         if ( ch->in_room->vnum < 550 || ch->in_room->vnum > 629 )
            {
               stc("You can't use this ability to get access to that room.\n\r",ch);
               return TRUE;
            }
      }

   if ( ch->in_room->vnum >= 550 && ch->in_room->vnum <= 629 )
      {
         if ( pRoomIndex->vnum < 550 || pRoomIndex->vnum > 629 )
            {
               stc("You can't use this ability to get access to that room.\n\r",ch);
               return TRUE;
            }
      }

   if ( ch->in_room->vnum >= 90 && ch->in_room->vnum < 100 )
      {
         stc("You can't use this ability in this room.\n\r",ch);
         return TRUE;
      }

   if ( pRoomIndex->vnum >= 90 && pRoomIndex->vnum < 100 )
      {
         stc("You can't use this ability to get access to that room.\n\r",ch);
         return TRUE;
      }

   if ( pRoomIndex->vnum == 651 )
      {
         stc("You can't use this ability to get access to that room.\n\r",ch);
         return TRUE;
      }

   if ( weather_info.sky == SKY_APOCALYPSE )
      return FALSE;

   if ( !armageddon && mysafe != 10 )
      {
         if ( IS_NPC( ch ) && IS_SET( pRoomIndex->room_flags, ROOM_SHIFT ) )
            {
               stc("You can't use this ability to get access to that room.\n\r",ch);
               return TRUE;
            }

         if ( ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000
              && !( ch->in_room->area->land == ch->pcdata->deity && ch->pcdata->deity > 0 ) )
            {
               if ( !IS_NPC( ch ) && ch->pcdata->kingdom == 1 && pRoomIndex->vnum >= 2500 && pRoomIndex->vnum < 3000 )
                  return FALSE;

               stc("You can't use this ability in this room.\n\r",ch);
               return TRUE;
            }

         if ( ch->in_room->vnum >= 3500 && ch->in_room->vnum < 4000
              && !( ch->in_room->area->land == ch->pcdata->deity && ch->pcdata->deity > 0 ) )
            {
               if ( !IS_NPC( ch ) && ch->pcdata->kingdom == 3 && pRoomIndex->vnum >= 3500 && pRoomIndex->vnum < 4000 )
                  return FALSE;

               stc("You can't use this ability in this room.\n\r",ch);
               return TRUE;
            }

         if ( ch->in_room->vnum >= 4500 && ch->in_room->vnum < 5000
              && !( ch->in_room->area->land == ch->pcdata->deity && ch->pcdata->deity > 0 ) )
            {
               if ( !IS_NPC( ch ) && ch->pcdata->kingdom == 2 && pRoomIndex->vnum >= 4500 && pRoomIndex->vnum < 5000 )
                  return FALSE;

               stc("You can't use this ability in this room.\n\r",ch);
               return TRUE;
            }

         if ( ch->in_room->vnum >= 16500 && ch->in_room->vnum < 17000 &&
              !( ch->in_room->area->land == ch->pcdata->deity && ch->pcdata->deity > 0 ) )
            {
               if ( !IS_NPC( ch ) && ch->pcdata->kingdom == 4 && pRoomIndex->vnum >= 16500 && pRoomIndex->vnum < 17000 )
                  return FALSE;

               stc("You can't use this ability in this room.\n\r",ch);
               return TRUE;
            }

         if ( !IS_NPC( ch ) && pRoomIndex->area->land == ch->pcdata->deity && ch->pcdata->deity > 0 )
            return FALSE;

         if ( ( pRoomIndex->vnum >= 3500 && pRoomIndex->vnum < 4000 )
            || ( pRoomIndex->vnum >= 4500 && pRoomIndex->vnum < 5000 )
            || ( pRoomIndex->vnum >= 16500 && pRoomIndex->vnum < 17000 )
            || ( pRoomIndex->vnum >= 2500 && pRoomIndex->vnum < 3000 ) )
            {
                stc("You can't use this ability to get access to that room.\n\r",ch);
                return TRUE;
            }
      }

   return FALSE;
}

/* Trace's Bounty code */
void do_bounty( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    
        if ( arg1[0] == '\0' || arg2[0] == '\0' )
        {
             send_to_char( "Place a bounty on who's head?\n\rSyntax:  Bounty <victim> <amount>\n\r", ch );
             return;
        }
        
        if ( ( victim = get_char_world( ch, arg1 ) ) == NULL)
        {
           send_to_char( "They are currently not logged in!", ch );
           return;
        }
  
      if (IS_NPC(victim))
      {
        send_to_char( "You cannot put a bounty on NPCs!", ch );
        return;
      }
      
      if (victim->pcdata->bounty > 1000000)
      {
        send_to_char( "They already have too much bounty on their head.",ch);
        return;
      }

      if( victim == ch)
      {
	stc( "Not on yourself\n\r",ch);
	return;
      }
     
      if (victim->level >= 7)
      {
        send_to_char( "You can't put a bounty on an immortal.",ch);
        return;
      }
 
   if ( is_number( arg2 ) )
      {
         int amount;
         amount   = atoi(arg2);
	
         if ( amount < 1 )
   	      {
		  stc( "Needs to be at least #w1 #ygold piece#n, less than that you gotta be kidding.\n\r",ch);
		  return;
	      }

         if ( !IS_IMMORTAL(ch) && ch->gold < amount )
            {
                send_to_char( "You don't have that many #ygold pieces#n!", ch );
                return;
            }

         ch->gold -= amount;
         victim->pcdata->bounty += amount;
         sprintf( buf, "You have placed a %d QP bounty on %s.\n\r",
         amount,victim->name );
         send_to_char(buf,ch);

         if ( IS_IMMORTAL(ch) )
            sprintf(buf, "#w%s#g now has a bounty of #w%d #ygold pieces#g on their head placed by an #pImmortal#g.", victim->name, amount );
            else sprintf(buf, "#w%s#g now has a bounty of #w%d #ygold pieces#g on their head placed by #p%s#g.", victim->name, amount, ch->name);
	
         do_info(ch, buf);        
         return;
     }
}

/*
 * Generic channel function.
 */
void talk_channel( CHAR_DATA *ch, char *argument, int channel, const char *verb )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf5[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int pos, image_check = 0;
    bool ignored;
    int position;

    if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_STEALTHY && ch->fight_timer > 0 )
       {
          stc("#w#zYou are too sneaky to use anything but #ysay #wat this critical moment.#n\n\r", ch );
          return;
       }

	if ( IS_SET( ch->more3, MORE3_SILENCE ) )
       {
          stc("#w#zYou have lost your voice!#n\n\r", ch );
          return;
       }

    if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
    {
     send_to_char("Something prevents you from speaking in this room.\n\r", ch);
     return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "%s what?\n\r", verb );
	buf[0] = UPPER(buf[0]);
	return;
    }

    if ( IS_HEAD(ch, LOST_TONGUE) )
    {
	sprintf( buf, "You can't %s without a tongue!\n\r", verb );
	send_to_char( buf, ch );
	return;
    }
    
    if ( IS_EXTRA(ch, GAGGED) )
    {
	sprintf( buf, "You can't %s with a gag on!\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	sprintf( buf, "You can't %s.\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    REMOVE_BIT(ch->deaf, channel);

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_HEX) )
       argument = talk_garbage(ch,argument);

    switch ( channel )
    {
    default:

if (channel == CHANNEL_CHAT)
{
   sprintf( buf, "#wYou %s '#P%s#w'.#n\n\r", verb, argument );
   send_to_char( buf, ch );

   if ( IS_SET(ch->more3, MORE3_SOUL) )
      {
         sprintf( buf, "#wYour image %s '#P%s#w'.#n\n\r", verb, argument );
         send_to_char( buf, ch );
         send_to_char( buf, ch );
      }

   if ( ch->trust > 6 ) 
      {
          sprintf( buf, "#cAn Immortal#n #y%ss#n '#1$t#n'.", verb );
          sprintf( buf2,"#wAn Immortal#n #y%ss#n '#1$t#n'.", verb);
          sprintf( buf5,"#w(- #C$n #w-)#n #y%ss#n '#1$t#n'.", verb);
      }
      else {
               sprintf( buf, "#w$n %ss '#P$t#w'.#n",     verb );
               sprintf( buf2, "#w$n %ss '#P$t#w'.#n",     verb );

               if ( IS_SET(ch->more3, MORE3_SOUL) )
                  {
                     sprintf( buf3, "#w$n's image %ss '#P$t#w'.#n",     verb );
                     sprintf( buf4, "#w$n's image %ss '#P$t#w'.#n",     verb );
                     image_check = 1;
                  }
           }
}

else
{
     sprintf( buf, "You %s '#1%s#n'.\n\r", verb, argument );
     send_to_char( buf, ch );

     sprintf( buf, "$n %ss '#1$t#n'.",     verb );
     sprintf( buf2, "$n %ss '#1$t#n'.",     verb );

     if ( IS_SET(ch->more3, MORE3_SOUL) )
        {
            sprintf( buf3, "$n's image %ss '#1$t#w'.#n",     verb );
            sprintf( buf4, "$n's image %ss '#1$t#w'.#n",     verb );
            image_check = 1;
        }
}
break;
    case CHANNEL_IMMTALK:
      switch( number_range(1,4) )
      {
         case 1: sprintf( buf, "#0(( #L%s#0 ))#C $t.#n",ch->pcdata->switchname );
	           sprintf( buf2, "#0(( #L%s#0 ))#C $t.#n",ch->pcdata->switchname );
                 break;
         case 2: sprintf( buf, "#sI#sm#sm#sy #y%s screams #C$t.#n",ch->pcdata->switchname );
	           sprintf( buf2, "#sI#sm#sm#sy #y%s screams #C$t.#n",ch->pcdata->switchname );
                 break;
         case 3: sprintf( buf, "#R))#w*#y----#e%s #R|-:#C $t.#n",ch->pcdata->switchname );
	           sprintf( buf2, "#R))#w*#y----#e%s #R|-:#C $t.#n",ch->pcdata->switchname );
                 break;
         case 4: sprintf( buf, "#w/-~~~ #R%s#w ~~~-\\ #C$t.#n",ch->pcdata->switchname );
	           sprintf( buf2, "#w/-~~~ #R%s#w ~~~-\\ #C$t.#n",ch->pcdata->switchname );
                 break;
      }
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

    case CHANNEL_ANGEL:
        sprintf( buf, "#0[#7$n#0]#C '$t'.#n" );
        sprintf( buf2, "#0[#7$n#0]#C '$t'.#n" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

    case CHANNEL_PALADIN:

      sprintf( buf, "#c(:#w$n#c:) '#y$t#c'.#n" );
	sprintf( buf2, "#c(:#w$n#c:) '#y$t#c'.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

    case CHANNEL_CLANTALK:
	sprintf( buf, "#g}#e<#R:#y$n#C:#w>#g{ #c'#w$t#c'.#n" );
	sprintf( buf2, "#g}#e<#R:#y$n#C:#w>#g{ #c'#w$t#c'.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

    case CHANNEL_PRAY:
	sprintf( buf, "#0[#R$n#0]#C '$t'.#n" );
	sprintf( buf2, "#0[#R$n#0]#C '$t'.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

    case CHANNEL_AMAZON:
      sprintf( buf, "#g[0o: #pWarrior $n#g :o0]#w $t.#n" );
      sprintf( buf2, "#g[0o: #pWarrior $n#g :o0]#w $t.#n" );
      position        = ch->position;
      ch->position    = POS_STANDING;
      act( buf, ch, argument, NULL, TO_CHAR );
      ch->position    = position;
      break; 

    case CHANNEL_MAGETALK:
	sprintf( buf, "#n{{#0$n#n}}#C '$t'.#n" );
	sprintf( buf2, "#n{{#0$n#n}}#C '$t'.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

   case CHANNEL_ROAR:
        sprintf( buf, "#r(($n)) roars #w--> '#y$t#w'.#n" );
        sprintf( buf2, "#r(($n)) roars #w--> '#y$t#w'.#n" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

   case CHANNEL_DRAGTALK:
        sprintf( buf, "#w|-_#r$n#w--) '#c$t#w'.#n" );
        sprintf( buf2, "#w|-_#r$n#w--) '#c$t#w'.#n" );  
        position        = ch->position;
        ch->position    = POS_STANDING;

        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

    case CHANNEL_COMMUNICATE:
        sprintf( buf, "#e^^#pSon $n#e^^#r: '#c$t#r'.#n" );
        sprintf( buf2, "#e^^#pSon $n#e^^#r: '#c$t#r'.#n" );  
        position        = ch->position;
        ch->position    = POS_STANDING;

        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

    case CHANNEL_HOWL:
	sprintf( buf, "#y((#L$n#y))#C '$t'.#n" );
	sprintf( buf2, "#y((#L$n#y))#C '$t'.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;

      if ( is_sound( ch ) )
         stc("!!SOUND(howl.wav V=100 L=1 P=40)\n\r",ch);

	break;

	case CHANNEL_MIKTALK:
		sprintf(buf, "#C***#y$n#C*** '$t'.#n" );
		sprintf(buf2, "#C***#y$n#C*** '$t'.#n" );
		position = ch->position;
		act(buf, ch, argument, NULL, TO_CHAR);
		ch->position = position;
	break;

	case CHANNEL_HIGHTALK:
		sprintf( buf, "#C-=#R$n#C=-  '$t'.#n" );
		sprintf(buf2, "#C-=#R$n#C=-  '$t'.#n" );
		position = ch->position;
		act( buf, ch, argument, NULL, TO_CHAR );
		ch->position = position;
		break;

    case CHANNEL_KNIGHTTALK:
        sprintf( buf, "#0.x.#7$n#0.x.#C '$t'.#n");
        sprintf( buf2, "#0.x.#7$n#0.x.#C '$t'.#n");
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

    case CHANNEL_SIGN:
        sprintf( buf, "#e.o0#P$n#w0o.#C '$t'.#n");
        sprintf( buf2, "#e.o0#P$n#w0o.#C '$t'.#n");
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

    case CHANNEL_PROBE:
		sprintf(buf, "#R]x#b$n#Rx[ '$t'.#n" );
		sprintf(buf2, "#R]x\\/#b$n#R\\/x[ '$t'.#n" );
		position = ch->position;
		act(buf, ch, argument, NULL, TO_CHAR);
		ch->position = position;
	break;

    case CHANNEL_KTALK:
	sprintf( buf, "#g.x[#w$n#g]x. #n'#y$t#n'." );
	sprintf(buf2, "#g.x[#w$n#g]x. #n'#y$t#n'." );
	position = ch->position;
	ch->position = POS_STANDING;
	act(buf, ch, argument, NULL, TO_CHAR);
	ch->position = position;
	break;

    case CHANNEL_MONK:
        sprintf( buf, "#c$n #ghums #n'#c$t#n'." );
        sprintf( buf2, "#c$n #ghums #n'#c$t#n'." );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break; 

    case CHANNEL_VAMPTALK:
      sprintf( buf, "#R<<#0$n#R>>#C $t.#n" );
	sprintf( buf2, "#R<<#0$n#R>>#C $t.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *och;
	CHAR_DATA *vch;

	och = d->original ? d->original : d->character;
	vch = d->character;

	ignored = FALSE;

	if ( d->connected == CON_PLAYING && vch != ch && !IS_SET(och->deaf, channel) )
	{
        for ( pos = 0; pos < MAX_FORGET; pos++ )
        {
           if ( och->pcdata->forget[pos] == NULL )
              break;

           if ( !str_cmp(ch->name, och->pcdata->forget[pos]) )
              ignored = TRUE;
        }

	if (ignored) continue;
	
	    if ( channel == CHANNEL_IMMTALK && !IS_IMMORTAL(och) )
		continue;
	if (channel== CHANNEL_MIKTALK && (!IS_NPC(och) && och->class != CLASS_NINJA && och->class != CLASS_SHADOW
          && och->class != CLASS_ASSASSIN && och->class != CLASS_SHADOW_WAR && och->class != CLASS_NIGHT_BLADE 
          && och->class != CLASS_KURUDA && !IS_IMMORTAL(och)))
	continue;
	    if ( channel == CHANNEL_HIGHTALK && (!IS_NPC(och) &&		
		och->class != CLASS_HIGHLANDER && !IS_IMMORTAL(och)))
		continue;
	if (channel == CHANNEL_KTALK && (!IS_NPC(och) && 
	!IS_IMMORTAL(och) && ch->pcdata->kingdom != och->pcdata->kingdom))
	continue;
	    if ( channel == CHANNEL_SIGN && (!IS_NPC(och) &&
                och->class != CLASS_DROW && och->class != CLASS_MASTER && och->class != CLASS_DARK_ELF && !IS_IMMORTAL(och)))
                continue;

            if ( channel == CHANNEL_MONK && (!IS_NPC(och) && och->class != CLASS_SHADOW &&
                och->class != CLASS_MONK && och->class != CLASS_ASSASSIN 
                && och->class != CLASS_KURUDA && !IS_IMMORTAL(och)))
                continue; 

            if ( channel == CHANNEL_PROBE && (!IS_NPC(och) &&
               och->class != CLASS_GUYVER && !IS_IMMORTAL(och)))
               continue;

	    if ( channel == CHANNEL_VAMPTALK && (!IS_NPC(och) && 
		och->class != CLASS_VAMPIRE && och->class != CLASS_SOULREAVER && och->class != CLASS_DAYWALKER && !IS_IMMORTAL(och)))
		continue;

            if ( channel == CHANNEL_MAGETALK && (!IS_NPC(och) &&
	        och->class != CLASS_MAGE && och->class != CLASS_SORCEROR && och->class != CLASS_NECROMANCER
              && och->class != CLASS_WIZARD && och->class != CLASS_SUMMONER && !IS_IMMORTAL(och)))
		continue;

            if ( channel == CHANNEL_DRAGTALK && (!IS_NPC(och) &&
	        och->class != CLASS_DRAGOON && och->class != CLASS_DRAGON_KNIGHT && !IS_IMMORTAL(och)))
		continue;

            if ( channel == CHANNEL_PRAY && (!IS_NPC(och) &&
	        och->class != CLASS_DEMON && och->class != CLASS_FIEND && !IS_IMMORTAL(och)))
		continue;

            if ( channel == CHANNEL_COMMUNICATE && (!IS_NPC(och) &&
               !IS_IMMORTAL(och) && och->class != CLASS_SAIYAN ))
               continue;

            if ( channel == CHANNEL_PALADIN && ( !IS_NPC(och) &&
	        och->class != CLASS_PALADIN && och->class != CLASS_DARK_KNIGHT 
                && och->class != CLASS_DIVINE_KNIGHT 
                && och->class != CLASS_HOBBIT && !IS_IMMORTAL(och)))
		continue;

            if ( channel == CHANNEL_ANGEL && (!IS_NPC(och) &&
                och->class != CLASS_ANGEL && !IS_IMMORTAL(och)))
                continue;

            if ( channel == CHANNEL_KNIGHTTALK && (!IS_NPC(och) &&
                och->class != CLASS_UNDEAD_KNIGHT && !IS_IMMORTAL(och)))
                continue;

            if ( channel == CHANNEL_AMAZON && (!IS_NPC(och) &&
                och->class != CLASS_AMAZON && och->class != CLASS_SAMURAI
                && och->class != CLASS_HUNTER && och->class != CLASS_KODO && !IS_IMMORTAL(och)))
                continue;

			if ( channel == CHANNEL_ROAR && (!IS_NPC(och) &&
			och->class != CLASS_DRAGON && !IS_IMMORTAL(och)))
		continue;

			if ( channel == CHANNEL_CLANTALK && (!IS_NPC(och) &&
			och->class != CLASS_PHOENIX && och->class != CLASS_SNOW_ELF
                  && och->class != CLASS_DUERGAR && och->class != CLASS_HOBBIT
                  && och->class != CLASS_WISP && !IS_IMMORTAL(och)))
		continue;

            if ( channel == CHANNEL_HOWL && (!IS_NPC(och) &&
	        och->class != CLASS_WEREWOLF && och->class != CLASS_GAROU && !IS_POLYAFF(och,POLY_WOLF)
		&& !IS_IMMORTAL(och)))
	    {
	        if ((och->in_room) && (ch->in_room))
	    	{
                  if ( is_sound( och ) )
                     stc("!!SOUND(howl.wav V=100 L=1 P=40)\n\r",och);

			if ((och->in_room == ch->in_room))
			{
		    		act("$n throws back $s head and howls loudly.", ch , argument , och, TO_VICT);
		    		continue;
			} 
			else if ((och->in_room->area == ch->in_room->area ))
			{
		    		act("You hear a loud howl nearby.", ch , NULL , och, TO_VICT);
		    		continue;
			}
			else 
			{
		    		act("You hear a loud howl in the distance.", ch , NULL , och, TO_VICT);
	            		continue;
			}
	    	}
	    }
            
	    if ( channel == CHANNEL_YELL
	    &&   vch->in_room->area != ch->in_room->area )
		continue;

	    position		= vch->position;
	    if ( channel != CHANNEL_SHOUT && channel != CHANNEL_YELL )
		vch->position	= POS_STANDING;

	    if ( !IS_NPC(vch) )
	    {
            if ( IS_IMMORTAL(vch) && IS_IMMORTAL(ch) && channel == CHANNEL_CHAT )
               act( buf5, ch, argument, vch, TO_VICT );
               else act( buf2, ch, argument, vch, TO_VICT );

            if ( image_check == 1 )
               {
	    	      act( buf3, ch, argument, vch, TO_VICT );
	    	      act( buf3, ch, argument, vch, TO_VICT );
               }

	    	vch->position	= position;
		continue;
	    }

	    act( buf, ch, argument, vch, TO_VICT );

          if ( image_check == 1 )
             {
    	          act( buf4, ch, argument, vch, TO_VICT );
    	          act( buf4, ch, argument, vch, TO_VICT );
             }

	    vch->position	= position;
	}
    }

    return;
}

void room_message(ROOM_INDEX_DATA *room, char *message)
{
    CHAR_DATA *rch;

    if ( (rch = room->people) == NULL) return;

    act(message, rch, NULL, NULL, TO_ROOM);
    act(message, rch, NULL, NULL, TO_CHAR);
}

void do_chat( CHAR_DATA *ch, char *argument )
{
   talk_channel( ch, argument, CHANNEL_CHAT, "chat" );
   return;
}


/*
 * Alander's new channels.
 */
void do_music( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_MUSIC, "sing" );

    return;
}



void do_clantalk( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       {
           stc("Huh?\n\r", ch);
           return;
       }

    if ( !IS_CLASS( ch, CLASS_WISP ) && !IS_CLASS( ch, CLASS_PHOENIX ) && !IS_CLASS( ch, CLASS_DUERGAR )
         && !IS_CLASS( ch, CLASS_SNOW_ELF ) && !IS_CLASS( ch, CLASS_HOBBIT )
         && !IS_IMMORTAL( ch ) )
       {
           stc("Huh?\n\r", ch);
           return;
       }

    talk_channel( ch, argument, CHANNEL_CLANTALK, "clantalks" );
    return;
}

void do_roar( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch) || ( !IS_IMMORTAL(ch) && ch->class != CLASS_DRAGON ))
        {
           stc("Huh?\n\r", ch);
           return;
        }
        talk_channel(ch, argument, CHANNEL_ROAR, "roar");
        return;
}

void do_dragtalk( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC( ch ) || ( !IS_IMMORTAL(ch) && ch->class != CLASS_DRAGOON
        && ch->class != CLASS_DRAGON_KNIGHT ))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_DRAGTALK, "dragtalk" );
    return;
}

void do_communicate( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) ) 
      return;

   if ( ch->class != CLASS_SAIYAN && !IS_IMMORTAL(ch) ) 
      {
        stc("Huh?\n\r",ch);
        return;
      }

   if ( !IS_IMMORTAL(ch) && is_eye_scouter(ch) == FALSE )
      return; 

   act("$n's Eye Scouter flashes red as $e mumbles something into it.",ch,NULL,NULL,TO_ROOM);
   talk_channel( ch, argument, CHANNEL_COMMUNICATE, "communicate" );
   return;
}

void do_knighttalk( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC( ch ) || (!IS_IMMORTAL(ch) && ch->class != CLASS_UNDEAD_KNIGHT ))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_KNIGHTTALK, "knighttalk" );
    return;
}

void do_shout( CHAR_DATA *ch, char *argument )
{

    talk_channel( ch, argument, CHANNEL_SHOUT, "shout" );

    WAIT_STATE( ch, 12 );
    return;
}

void do_yell( CHAR_DATA *ch, char *argument )
{

    talk_channel( ch, argument, CHANNEL_YELL, "yell" );

    return;
}



void do_immtalk( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_IMMTALK, "immtalk" );
    return;
}

void do_monktalk( CHAR_DATA *ch, char *argument ) {
	if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && ch->class != CLASS_MONK
          && ch->class != CLASS_KURUDA && ch->class != CLASS_ASSASSIN && ch->class != CLASS_SHADOW && ch->class != CLASS_DARAMON ) ) {
	send_to_char("Huh?\n\r", ch);
	return;}
	talk_channel(ch, argument, CHANNEL_MONK, "monk");
	return;
}

void do_amaztalk( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) || (!IS_IMMORTAL(ch) && ch->class != CLASS_AMAZON && ch->class != CLASS_SAMURAI
         && ch->class != CLASS_KODO && ch->class != CLASS_HUNTER ))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_AMAZON, "samurai" );
    return;
}

void do_miktalk( CHAR_DATA *ch, char *argument )
{

	if ( IS_NPC(ch) || ( !IS_IMMORTAL(ch) && ch->class != CLASS_NINJA && ch->class != CLASS_ASSASSIN
           && ch->class != CLASS_NIGHT_BLADE && ch->class != CLASS_KURUDA && ch->class != CLASS_SHADOW && ch->class != CLASS_SHADOW_WAR ) )
	{send_to_char("Huh?\n\r", ch);
	return;}
	talk_channel(ch, argument, CHANNEL_MIKTALK, "miktalk");
	return;
}

void do_hightalk( CHAR_DATA *ch, char *argument )
{
      if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && ch->class != CLASS_HIGHLANDER ))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	talk_channel( ch, argument, CHANNEL_HIGHTALK, "hightalk" );
	return;
}

void do_sign( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && ch->class != CLASS_DROW 
        && ch->class != CLASS_DARK_ELF && ch->class != CLASS_MASTER ))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    talk_channel( ch, argument, CHANNEL_SIGN, "sign" );
    return;
}

void do_ktalk( CHAR_DATA *ch, char *argument ) 
{
	if (IS_NPC(ch)) return;
	if (ch->pcdata->kingdom == 0 || ch->pcdata->kingdom == 5) {
	send_to_char("Huh?\n\r", ch );
	return;}
	talk_channel(ch, argument, CHANNEL_KTALK, "ktalk" );
}


void do_vamptalk( CHAR_DATA *ch, char *argument )
{

    if ( IS_NPC(ch) || ( !IS_IMMORTAL(ch) && ch->class != CLASS_VAMPIRE && ch->class != CLASS_DAYWALKER
         && ch->class != CLASS_SOULREAVER ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    talk_channel( ch, argument, CHANNEL_VAMPTALK, "vamptalk" );
}

void do_magetalk( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) || ( !IS_IMMORTAL(ch) && ch->class != CLASS_MAGE && ch->class != CLASS_SORCEROR
         && ch->class != CLASS_SUMMONER && ch->class != CLASS_WIZARD && ch->class != CLASS_NECROMANCER ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_MAGETALK, "magetalk" );
}

void do_powers( CHAR_DATA *ch, char *argument )
{
	if ( IS_CLASS ( ch, CLASS_PALADIN ) )
	{
		stc("              Paladin Powers               \n\r",ch);
		stc(" Praise Holyarmor Pgrant Aura Avatar       \n\r",ch);
		stc(" Justicecutter Soulhammer Flashsword Charge\n\r",ch);
		stc(" Nemesis Genesis Cover Godlysight Godsheal \n\r",ch);
		stc(" Godssearch Stasissword Lightningstab      \n\r",ch);
		stc(" Splitpunch Holyexplosion                  \n\r",ch);
	
		return;
	}

	if ( IS_CLASS ( ch, CLASS_DEMON ) )
	{
		stc("              Demon Powers                     \n\r",ch);
		stc(" Inpart Demonarmor Fangs Horns Caust Leap      \n\r",ch);
		stc(" Nightsight Toughness Scry Immolate Shield     \n\r",ch);
		stc(" Unnerve Claws Hooves Wings Truesight Speed    \n\r",ch);
		stc(" Magic Longsword Inferno Demonform Freezeweapon\n\r",ch);
		stc(" Tail Graft Might Travel Lifespan Move Entomb  \n\r",ch);
		stc(" Rage Blink Eyespy Hellfire Gust               \n\r",ch);
		stc(" Hellhound Sin Damnation Lavapit Frostbreah    \n\r",ch);
		stc(" Darkholy Desanctify Darkforce Firewall DCurse \n\r",ch);
		stc(" Soulheal Soullink                             \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_MONK ) )
	{
		stc("              Monk Powers                  \n\r",ch);
		stc(" Godly Hum Guidance Vulcanpinch Blazinghand\n\r",ch);
		stc(" Bladegrasp Target Focus Kakeio Aragami    \n\r",ch);
		stc(" Ironhammer Cloak HolyRetrieval Curseofall.\n\r",ch);
		stc(" Blessingofsai. Divineray Subtletouch      \n\r",ch);
		stc(" Demmak chitilak healingtouch deathtouch   \n\r",ch);
		stc(" skid Knee Roundhouse Jab Shin Sidewinder  \n\r",ch);
		stc(" Elbow Sweep Headbutt Shiki Yamisugi Nisina\n\r",ch);
		stc(" OrochiNagi                                \n\r",ch);
		
		return;
	}

	if ( IS_CLASS ( ch, CLASS_NINJA ) )
	{
		stc("              Ninja Powers                   \n\r",ch);
		stc(" Miktalk Ninjaarmor Assassinate Kakusu       \n\r",ch);
		stc(" Koryou Kanzuite Bomuzite Trip Shadowwalk    \n\r",ch);
		stc(" Yoga Hunt Harakiri Hakiznetu Tsume Hakunetsu\n\r",ch);
		stc(" Mienaku Dinichi Circle Orimasu Inclimb      \n\r",ch);
		stc(" Miezite Dig Splitstrike Incapacitate        \n\r",ch);
		stc(" Sonicstrike Zsikuku                         \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_VAMPIRE ) )
	{
		stc("              Vampire Powers                 \n\r",ch);
		stc(" Claws Fangs Vamptalk Vamparmor Abduction    \n\r",ch);
		stc(" Lamprecy Grab Vsilence Flash Infimity Unveil\n\r",ch);
		stc(" Change Shroud Spiritblast Majesty Renew     \n\r",ch);
		stc(" Drain Mirror Spit Tears Stigmata Maelstrom  \n\r",ch);
		stc(" Physchiccrush Soulreaver Soulrreaper        \n\r",ch);
		stc(" Dark Light Chill Fire Stone Shock Soul Faith\n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_DRAGON ) )
	{
		stc("              Dragon Powers                  \n\r",ch);
		stc(" Claw Fangs Dragonswoop Breach Dragonbreath  \n\r",ch);
		stc(" Highclaw Tailsweep Shoulderrush Megaflare   \n\r",ch);
		stc(" Stomp Dragonshift Roar Dragongift           \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_WEREWOLF ) )
	{
		stc("              Werewolf Powers                 \n\r",ch);
		stc(" Howl Moonarmor Shadowplane Vanish Moonbeam   \n\r",ch);
		stc(" Clapofthunder Paws Dazzle Spiralmoon Disquiet\n\r",ch);
		stc(" Quills Gnaw Claws Fangs Razorclaws Whelp     \n\r",ch);
		stc(" Klaive Clenched Barrage Avalanche Cannibal   \n\r",ch);
		stc(" Quicksand Waste Lambent Cheapshot Camoflauge \n\r",ch);
		stc(" Tornadoslice Banshee Rage Calm               \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_DROW ) )
	{
		stc("              Drow Powers                      \n\r",ch);
		stc(" Grant Drowcreate Sign Drowpowers Drowsight    \n\r",ch);
		stc(" Drowshield Darkness Drowfire Mount Dgarrotte  \n\r",ch);
		stc(" Drowhate Spiderform Web Confuse Glamour       \n\r",ch);
		stc(" Fightdance Drewst Velkyn Darkdodge Choke      \n\r",ch);
		stc(" Chaosblast Venom Bloodlet Earthblast Cloudkill\n\r",ch);
		stc(" Poison Hex Holdperson Shoot Reload Unload     \n\r",ch);
		stc(" Dischard Pandorabox                           \n\r",ch);
		
		return;
	}

	if ( IS_CLASS ( ch, CLASS_WIZARD ) )
	{
		stc("              Wizard Powers                    \n\r",ch);
		stc(" Magearmor Magetalk Fireball Enrage Drainpower \n\r",ch);
		stc(" Gaseous Greatwall Reveal Layhands Telekinetic \n\r",ch);
		stc(" Antimagic Ultima Unsummon Whirlwind Mwall     \n\r",ch);
		stc(" Levitate Aggression Alabaster Gaeab Haste     \n\r",ch);
		stc(" Cleanse Mangarab Divinewisdom Briarshield     \n\r",ch);
		stc(" Slow Lumica                                   \n\r",ch);

		if ( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 1 || ch->clannum == 6 ) )
		{
			stc(" \n\r",ch);
			stc("             Battle Mage Powers                  \n\r",ch);
			stc(" Evocate Elementaltouch Soulfield                \n\r",ch);
			
			return;
		}

		if ( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) )
		{
			stc(" \n\r",ch);
			stc("             Summoner Powers                     \n\r",ch);
			stc(" Titan Shiva Ifrit Diablos Siren Quezacotl Odin  \n\r",ch);
			stc(" Carburkle Salamander                            \n\r",ch);

			return;
		}

		if ( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 3 || ch->clannum == 6 ) )
		{
			stc(" \n\r",ch);
			stc("             Time Mage Powers                    \n\r",ch);
			stc(" Quicken Stop Age Accelerate Timewarp            \n\r",ch);
			
			return;
		}

		if ( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 )
		{
			stc(" \n\r",ch);
			stc("             Book Mage Powers                    \n\r",ch);
			stc(" Chant Disempower                                \n\r",ch);

			return;
		}

		return;
	}

	if ( IS_CLASS ( ch, CLASS_ANGEL ) )
	{
		stc("              Angel Powers                 \n\r",ch);
		stc(" Pray Angelcreate Enlighten Spread Fold    \n\r",ch);
		stc(" Etherealform Travel Godsheal Manashield   \n\r",ch);
		stc(" Fisttakedown Redemption Smite Trueform    \n\r",ch);
		stc(" Gspirit Gshield Holyball Godpeace Fheaven \n\r",ch);
		stc(" Holyword Megastrike Godswrath Martyr      \n\r",ch);
		stc(" Genesis                                   \n\r",ch);
		
		return;
	}

	if ( IS_CLASS ( ch, CLASS_DRAGOON ) )
	{
		stc("              Highwind Powers              \n\r",ch);
		stc(" Jump Ujump Vspears Whiskeybreak Higharmor \n\r",ch);
		stc(" Dragtalk Impale Dynamite Hyperjump Dragon \n\r",ch);
		stc(" Dragondive Highwind Dragoonthrow          \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_UNDEAD_KNIGHT ) )
	{
		stc("              Undead Knight Powers         \n\r",ch);
		stc(" Knighttalk Knightarmor Gain Weaponpractice\n\r",ch);
		stc(" Powerword Unholyrite Genocide Aura Mount  \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_NECROMANCER ) )
	{
		stc("              Necromancer Powers              \n\r",ch);
		stc(" Order Necrodrain Magearmor Magetalk Reduce   \n\r",ch);
		stc(" Gascloud corpseexplosion Soulblight Darkside \n\r",ch);
	    stc(" Soulbolt Createzombie Despondency Darkkinesis\n\r",ch);
		stc(" Darkshield Darkpath                          \n\r",ch);
		
		return;
	}

	if ( IS_CLASS ( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_SOR ) )
	{
		stc("              Sorceror Powers                 \n\r",ch);
		stc(" Darksword Chaossword Deathfield Firewall     \n\r",ch);
		stc(" Firesword DancingFlames Earthsword Earthgrasp\n\r",ch);
		stc(" Rockblast Aurawave Icesword Icewall Frigidcut\n\r",ch);
		stc(" Blitzsword Thunderbeam Blitztouch Blitzstrike\n\r",ch);
		stc(" Moonsword Quadraslice Lunabeam Magiwall      \n\r",ch);
		stc(" Ragedash Despairstrike Blaze                 \n\r",ch);
		stc(" Moonslash Piercingone                        \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_KNIGHT ) )
	{
		stc("              Dark Knight Powers                 \n\r",ch);
		stc(" DRoundhouse Duppercut Swordtakedown Dsweep Slash\n\r",ch);
		stc(" Analyse Darkbomb Hyper Mergance Swordrain Hwrath\n\r",ch);
		stc(" Stitch Psychicbarrier AbsoluteZero Moonwave     \n\r",ch);
		stc(" Darkstorm Renzokuken Finishingtouch Confuser    \n\r",ch);

		return;
	}

	if ( IS_CLASS ( ch, CLASS_NIGHT_BLADE ) )
	{
		stc("             Gaiden Powers                       \n\r",ch);
		stc(" Research Embody Uppercut Nknee Backfist Reverse \n\r",ch);
		stc(" NSweep Thrust Spin Lowkick Kipinch Nervaltouch  \n\r",ch);
		stc(" HishokenSlam Hunt Kakusu Mienaku Gunaya Alert   \n\r",ch);
		stc(" Throw Izunaken Jakouha Somerstrike Caltrops     \n\r",ch);
		stc(" Stardance Illusion Risingtiger                  \n\r",ch);
	
		return;
	}

	if ( IS_CLASS ( ch, CLASS_SAIYAN ) )
	{
		stc("              Saiyan Powers                      \n\r",ch);
		stc(" Saiyancreate Charging Imbreed Communicate       \n\r",ch);
		stc(" Powerread Powersearch Bigbang Kamehameha Kaioken\n\r",ch);
		stc(" Ryuken Taiyouken Genkidama Galickhou Kienzan    \n\r",ch);
		stc(" Kakusan Powerball Choukamehameha Mouthblast     \n\r",ch);
		stc(" Tailswipe Scatterpunch Pounce ssj               \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_SAMURAI ) )
	{
		stc("             Samurai Powers                      \n\r",ch);
		stc(" Samuraicreate Samutalk Hardslash Mediumslash    \n\r",ch);
		stc(" Lightslash Combo Sdraw Withdraw Elementalstrike \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_SHADOW ) )
	{
		stc("             Assassin Powers                               \n\r",ch);
		stc(" Miktalk Hum Ninjaarmor Learn Switch Study Chiblast        \n\r",ch);
		stc(" Innershield Innerfury Innerrage Yin Yang Innerhealing     \n\r",ch);
		stc(" Guidedshot Levitate Target Painless Handblast Chinsmite   \n\r",ch);
		stc(" Legdispatcher Armgrab Bodyrip Kipinch Dispel Kiwrath      \n\r",ch);
		stc(" Disguise Vanish Dispose Handcircua Triheal Divine Strength\n\r",ch);
		stc(" Midrain Darkmatter Luminaire Possess Hunt Throw Mitsu     \n\r",ch);
		stc(" Flashgrenade PiercingDasher Somerstrike Jab Roundhouse    \n\r",ch);
		stc(" Sidewinder Shin Elbow Headbutt                            \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_HOBBIT ) )
	{
		stc("             Hobbit Powers                                 \n\r",ch);
		stc(" Windtalk Hobbyarmor Siren Diablos Holy Flare GrandLove    \n\r",ch);
		stc(" Truthful Tillymint Famedarchangel Loathdemon Shadowdancer \n\r",ch);
		stc(" Shadowplane Demoneye Shadowdemon Daintydark Catseye       \n\r",ch);
		stc(" Darkhell Crystaldreams Frogget Wraitheknight              \n\r",ch);
	
		return;
	}

	if ( IS_CLASS( ch, CLASS_DUERGAR ) )
	{
		stc("             Duergar Powers                               \n\r",ch);
		stc(" Windtalk Titan Duergararmor Axeslam Duergardance Swing   \n\r",ch);
		stc(" Axeddrain Axeknockout Darkcloud Tunnel Vineofevil        \n\r",ch);
		stc(" Earthquake Slasheraxe Stainedaxe                         \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_SNOW_ELF ) )
	{
		stc("             Snow Elf Powers                              \n\r",ch);
		stc(" Windtalk Shiva Snowyarmor Icetrap Ambush Iceslice        \n\r",ch);
		stc(" Icepunch Iceshield Iceslash Carpejugulum Icedust         \n\r",ch);
		stc(" Winterswans Icearrow Freezingblast Slitherice            \n\r",ch);
		
		return;
	}

	if ( IS_CLASS( ch, CLASS_PHOENIX ) )
	{
		stc("             Phoenix Powers                               \n\r",ch);
		stc(" Windtalk Ifrit Firebreath Flametalon Phoenixrush Volcano \n\r",ch);
		stc(" Phoenixwrath Phoenixcry                                  \n\r",ch);
		
		return;
	}

	if ( IS_CLASS( ch, CLASS_WISP ) )
	{
		stc("             Wisp Powers                                  \n\r",ch);
		stc(" Windtalk Quezacotl Electricshield Colourspray Innerlight \n\r",ch);
		stc(" Reveal Heatshield Calllightning Thunderstorm Plasmablade \n\r",ch);
		stc(" Icarus Mindtap                                           \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_SHADOW_WAR ) )
	{
		stc("             Shadow Warrior Powers                        \n\r",ch);
		stc(" Miktalk Ninjaarmor Barrier Shadowplane Shadowfuse        \n\r",ch);
		stc(" Shadowswap Target Innerchi Darkhadou Chiblast Ragingdemon\n\r",ch);
		stc(" Swirlingsmite Flashkick Palmtackle                       \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_DARAMON ) )
	{
		stc("             Daramon Powers                     \n\r",ch);
		stc(" Chakra Hum BuddhaStomp Shockwave Stunner       \n\r",ch);
		stc(" Daracreate Zinjakicks Empower Tremor Chillblast\n\r",ch);
		stc(" Lightningslash Flaminguppercut Exploder        \n\r",ch);

		return;
	}

	if ( IS_CLASS( ch, CLASS_DAYWALKER ) )
	{
		stc("             Daywalker Powers                        \n\r",ch);
		stc(" Jiamar Sonicthrust Recover Cgrab Egrab Statis       \n\r",ch);
		stc(" Shatter Daydream Nightmare Stakewall Piercingrain   \n\r",ch);
		stc(" Desparation Rathi Despoticseptor Deathgrip Souldrain\n\r",ch);
		stc(" Darkritual Armorthorns Barbedfoliage Naturewrath    \n\r",ch);
		stc(" Sunbeam Sunaura Shadowsight Dtaste Psionicblast     \n\r",ch);
		stc(" Dcreate Inquest Insight                             \n\r",ch);

		return;
	}

	else
	{
		stc("This is not in for your class yet, please inform an immortal\n\r",ch);
		return;
	}

	return;
}

void do_probe( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && ch->class != CLASS_GUYVER ))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    talk_channel( ch, argument, CHANNEL_PROBE, "probe" );
    return;
}

void do_monk( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC( ch ) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch,CLASS_MONK) && !IS_CLASS(ch,CLASS_SHAOLIN)
         && !IS_CLASS(ch,CLASS_DARAMON) && !IS_CLASS(ch,CLASS_KURUDA) ))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    talk_channel( ch, argument, CHANNEL_MONK, "monk" );
}

void do_pray( CHAR_DATA *ch, char *argument )
{
    act("You mutter a few prayers.",ch,NULL,NULL,TO_CHAR);
    act("$n mutters a quick prayer.",ch,NULL,NULL,TO_ROOM);

    if ( ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_ALTAR &&
         ch->class == 0 && ch->position != POS_FIGHTING)
       {
                send_to_char("You glow bright blue as you are restored.\n\r",ch);
                ch->hit  = ch->max_hit;
                ch->mana = ch->max_mana;
                ch->move = ch->max_move;
                ch->loc_hp[0] = 0;
                ch->loc_hp[1] = 0;
                ch->loc_hp[2] = 0;
                ch->loc_hp[3] = 0;
                ch->loc_hp[4] = 0;
                ch->loc_hp[5] = 0;
                ch->loc_hp[6] = 0;
                update_pos(ch);
       }

    if ( IS_NPC(ch) || ( ch->class != CLASS_DEMON && ch->class != CLASS_FIEND && ch->class != CLASS_ANGEL && !IS_IMMORTAL(ch) ) ) 
	 return;

    if ( argument[0] == '\0' )
    {
	send_to_char("What do you wish to pray?\n\r",ch);
	return;
    }

    if ( IS_CLASS(ch, CLASS_ANGEL) )
       talk_channel( ch, argument, CHANNEL_ANGEL, "angel" );
       else if ( IS_CLASS(ch, CLASS_DEMON) || IS_CLASS(ch, CLASS_FIEND) )
               talk_channel( ch, argument, CHANNEL_PRAY, "pray" );
               else {
                       talk_channel( ch, argument, CHANNEL_ANGEL, "angel" );
                       talk_channel( ch, argument, CHANNEL_PRAY, "pray" );
                    }
}

void do_paladintalk( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) || ( !IS_IMMORTAL(ch) && ch->class != CLASS_PALADIN && ch->class != CLASS_DARK_KNIGHT
         && ch->class != CLASS_DIVINE_KNIGHT && ch->class != CLASS_HOBBIT ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    talk_channel( ch, argument, CHANNEL_PALADIN, "paladin" );
}

void do_howl( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC( ch ) || (!IS_IMMORTAL(ch) && ch->class != CLASS_WEREWOLF && ch->class != CLASS_GAROU 
	&& !IS_POLYAFF(ch, POLY_WOLF)))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_HOWL, "howls" );
    return;
}



void do_say( CHAR_DATA *ch, char *argument )
{
    char name   [80];
    char poly   [MAX_STRING_LENGTH];
    char speak  [10];
    char speaks [10];
    char endbit [2];
    char secbit [2];
    CHAR_DATA *to;
    bool is_ok;

	if ( IS_SET( ch->more3, MORE3_SILENCE ) )
       {
          stc("#w#zYou have lost your voice!#n\n\r", ch );
          return;
       }

    if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
    {
     send_to_char("Something prevents you from speaking in this room.\n\r", ch);
     return;
    }
    if ( IS_HEAD(ch, LOST_TONGUE) )
    {
	send_to_char( "You can't speak without a tongue!\n\r", ch );
	return;
    }
    if ( IS_EXTRA(ch, GAGGED) )
    {
	send_to_char( "You can't speak with a gag on!\n\r", ch );
	return;
    }

    if (strlen(argument) > MAX_INPUT_LENGTH)
    {
	send_to_char( "Line too long.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }

    endbit[0] = argument[strlen(argument)-1];
    endbit[1] = '\0';

    if (strlen(argument) > 1) secbit[0] = argument[strlen(argument)-2];
	else secbit[0] = '\0';
    secbit[1] = '\0';

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_HEX) )
       argument = talk_garbage(ch,argument);

    if (IS_BODY(ch,CUT_THROAT))
    {
	sprintf(speak,"rasp");
	sprintf(speaks,"rasps");
    }
    else if ( !IS_NPC(ch) && IS_SET(ch->special,SPC_WOLFMAN) )
    {
	if (number_percent() > 50)
	{
	    sprintf(speak,"growl");
	    sprintf(speaks,"growls");
	}
	else
	{
	    sprintf(speak,"snarl");
	    sprintf(speaks,"snarls");
	}
    }
    else if (!IS_NPC(ch) && IS_CLASS( ch, CLASS_WISP ) )
    {
	sprintf(speak,"cooe");
	sprintf(speaks,"cooes");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_BAT))
    {
	sprintf(speak,"squeak");
	sprintf(speaks,"squeaks");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_SERPENT))
    {
	sprintf(speak,"hiss");
	sprintf(speaks,"hisses");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_FROG))
    {
	sprintf(speak,"croak");
	sprintf(speaks,"croaks");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_RAVEN))
    {
	sprintf(speak,"squark");
	sprintf(speaks,"squarks");
    }
	else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_CHEESE))
	{
    sprintf(speak,"melt");
	sprintf(speaks,"melts");
	}
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_FROG)
    {
	sprintf(speak,"croak");
	sprintf(speaks,"croaks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_RAVEN)
    {
	sprintf(speak,"squark");
	sprintf(speaks,"squarks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_CAT)
    {
	sprintf(speak,"purr");
	sprintf(speaks,"purrs");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_DOG)
    {
	sprintf(speak,"bark");
	sprintf(speaks,"barks");
    }
    else if (!str_cmp(endbit,"!"))
    {
	sprintf(speak,"exclaim");
	sprintf(speaks,"exclaims");
    }
    else if (!str_cmp(endbit,"?"))
    {
	sprintf(speak,"ask");
	sprintf(speaks,"asks");
    }
    else if (secbit[0] != '\0' && str_cmp(secbit,".") && !str_cmp(endbit,"."))
    {
	sprintf(speak,"state");
	sprintf(speaks,"states");
    }
    else if (secbit[0] != '\0' && !str_cmp(secbit,".") && !str_cmp(endbit,"."))
    {
	sprintf(speak,"mutter");
	sprintf(speaks,"mutters");
    }
    else if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
    {
	sprintf(speak,"slur");
	sprintf(speaks,"slurs");
    }
    else
    {
	sprintf(speak,"say");
	sprintf(speaks,"says");
    }

    sprintf(poly,"You %s '#3$T#n'.", speak);

    act( poly, ch, NULL, argument, TO_CHAR );

    if ( IS_SET(ch->more3, MORE3_SOUL) )
       {
           sprintf(poly,"Your image %s '#3$T#n'.", speak);
           act( poly, ch, NULL, argument, TO_CHAR );
           act( poly, ch, NULL, argument, TO_CHAR );
       }

    sprintf(poly,"$n %s '#3$T#n'.", speaks);

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
        act( poly, ch, NULL, argument, TO_ROOM );

        if ( IS_SET(ch->more3, MORE3_SOUL) )
           {
               sprintf(poly,"$n's image %s '#3$T#n'.", speak);
               act( poly, ch, NULL, argument, TO_ROOM );
               act( poly, ch, NULL, argument, TO_ROOM );
           }

    	room_text( ch, strlower(argument) );
	return;
    }


    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    ch->pcdata->chobj->in_room != NULL &&
	    !IS_NPC(to) && to->pcdata->chobj != NULL && 
	    to->pcdata->chobj->in_room != NULL &&
	    ch->in_room == to->in_room)
		is_ok = TRUE; else is_ok = FALSE;

	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    ch->pcdata->chobj->in_obj != NULL &&
	    !IS_NPC(to) && to->pcdata->chobj != NULL && 
	    to->pcdata->chobj->in_obj != NULL &&
	    ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		is_ok = TRUE; else is_ok = FALSE;

	if (!is_ok) continue;

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
	else if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
	    sprintf(name, ch->morph);
        else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s '%s'.\n\r", name,speaks,argument);
	send_to_char(poly,to);
    }

    room_text( ch, strlower(argument) );
    return;
}

void room_text( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA     *vch;
    CHAR_DATA     *vch_next;
    CHAR_DATA     *mob;
    OBJ_DATA      *obj;
    ROOMTEXT_DATA *rt;
    bool           mobfound;
    bool           hop;

    char           arg  [MAX_INPUT_LENGTH];
    char           arg1 [MAX_INPUT_LENGTH];
    char           arg2 [MAX_INPUT_LENGTH];

    for ( rt = ch->in_room->roomtext; rt != NULL; rt = rt->next )
    {
	if (!strcmp(argument,rt->input)
	 || is_in(argument, rt->input)
	 || all_in(argument, rt->input)) 
	{
	    if ( rt->name != NULL         && rt->name != '\0'
	    &&   str_cmp(rt->name,"all")  && str_cmp(rt->name,"|all*") ) 
	    	if (!is_in(ch->name, rt->name) ) continue;
	    mobfound = TRUE;
	    if (rt->mob != 0)
	    {
		mobfound = FALSE;
	    	for ( vch = char_list; vch != NULL; vch = vch_next )
	    	{
		    vch_next	= vch->next;
		    if ( vch->in_room == NULL ) continue;
		    if ( !IS_NPC(vch)         ) continue;
		    if ( vch->in_room == ch->in_room
			&& vch->pIndexData->vnum == rt->mob )
		    {
			mobfound = TRUE;
		    	break;
		    }
	    	}
	    }
	    if (!mobfound) continue;
	    hop = FALSE;
	    switch(rt->type % RT_RETURN)
	    {
		case RT_SAY:
		    break;
		case RT_LIGHT:
		    REMOVE_BIT(ch->in_room->room_flags, ROOM_DARK);
		    break;
		case RT_DARK:
		    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
		    break;
		case RT_OBJECT:
		    if ( get_obj_index(rt->power) == NULL ) return;
		    obj = create_object(get_obj_index(rt->power), ch->level);
		    if (IS_SET(rt->type, RT_TIMER) ) obj->timer = 1;
		    if (CAN_WEAR(obj,ITEM_TAKE)) obj_to_char(obj,ch);
		    else obj_to_room(obj,ch->in_room);
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, obj, NULL, TO_CHAR );
		    else
		    	act( rt->choutput, ch, obj, NULL, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, obj, NULL, TO_ROOM );
		    hop = TRUE;
		    break;
		case RT_MOBILE:
		    if ( get_mob_index(rt->power) == NULL ) return;
		    mob = create_mobile(get_mob_index(rt->power));
		    char_to_room(mob,ch->in_room);
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, NULL, mob, TO_CHAR );
		    else
		    	act( rt->choutput, ch, NULL, mob, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, NULL, mob, TO_ROOM );
		    hop = TRUE;
		    break;
		case RT_SPELL:
		    (*skill_table[rt->power].spell_fun) ( rt->power, number_range(20,30), ch, ch );
		    break;
		case RT_PORTAL:
		    if ( get_obj_index(OBJ_VNUM_PORTAL) == NULL ) return;
		    obj = create_object(get_obj_index(OBJ_VNUM_PORTAL), 0);
		    obj->timer = 1;
		    obj->value[0] = rt->power;
		    obj->value[1] = 1;
		    obj_to_room(obj,ch->in_room);
		    break;
		case RT_TELEPORT:

		    if ( get_room_index(rt->power) == NULL ) return;

                if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
                   {
                       ROOM_INDEX_DATA *pRoomIndex;

                       pRoomIndex = get_room_index( rt->power );

                       for ( vch = pRoomIndex->people; vch != NULL; vch = vch_next )
                       {
                           vch_next = vch->next_in_room;

                           if ( !IS_NPC( vch ) )
                              {
                                 stc( "#wThere is already a #Rbattle #wgoing on in that #Croom #wwarrior. Wait a minute.#n\n\r", ch );
                                 return;
                              }
                       }

                       for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
                       {
                           vch_next = vch->next_in_room;

                           if ( is_same_group( ch, vch ) )
                              {
                                 if ( !str_cmp(rt->choutput,"copy") )
                                    act( rt->output, vch, NULL, NULL, TO_CHAR );
                                    else act( rt->choutput, vch, NULL, NULL, TO_CHAR );

                                 if ( !IS_SET(rt->type, RT_PERSONAL) ) 
                                    act( rt->output, vch, NULL, NULL, TO_ROOM );

                                 char_from_room(vch);
                                 char_to_room(vch,get_room_index(rt->power));
                                 act("#c$n #wappears in a #P#zflash #wof #ylight#w.#n",vch,NULL,NULL,TO_ROOM);
                                 do_look(vch,"auto");
                              }
                       }

                       return;
                   }
                   else if ( !IS_NPC(ch) && ch->fight_timer > 0 )
                           return;

		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, NULL, NULL, TO_CHAR );
		    else
		    	act( rt->choutput, ch, NULL, NULL, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, NULL, NULL, TO_ROOM );
		    char_from_room(ch);
		    char_to_room(ch,get_room_index(rt->power));
		    act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
		    do_look(ch,"auto");
                check_traps( ch );
		    hop = TRUE;
		    break;
		case RT_ACTION:
		    sprintf(arg,argument);
		    argument = one_argument( arg, arg1 );
		    argument = one_argument( arg, arg2 );
		    if ( (mob = get_char_room(ch, arg2) ) == NULL ) continue;
		    interpret( mob, rt->output );
		    break;
		case RT_OPEN_LIFT:
		    open_lift(ch);
		    break;
		case RT_CLOSE_LIFT:
		    close_lift(ch);
		    break;
		case RT_MOVE_LIFT:
		    move_lift(ch,rt->power);
		    break;
		default:
		    break;
	    }
	    if (hop && IS_SET(rt->type, RT_RETURN) ) return;
	    else if (hop) continue;
	    if (!str_cmp(rt->choutput,"copy") && !IS_SET(rt->type, RT_ACTION))
	    	act( rt->output, ch, NULL, NULL, TO_CHAR );
	    else if (!IS_SET(rt->type, RT_ACTION))
	    	act( rt->choutput, ch, NULL, NULL, TO_CHAR );
	    if (!IS_SET(rt->type, RT_PERSONAL) && !IS_SET(rt->type, RT_ACTION) )
		act( rt->output, ch, NULL, NULL, TO_ROOM );
	    if (IS_SET(rt->type, RT_RETURN) ) return;
	}
    }
    return;
}

char *strlower(char *ip)
{
	static char buffer[MAX_INPUT_LENGTH];
	int pos;

	for (pos = 0; pos < (MAX_INPUT_LENGTH - 1) && ip[pos] != '\0'; pos++) {
		buffer[pos] = tolower(ip[pos]);
	}	
	buffer[pos] = '\0';
	return buffer;
}

bool is_in(char *arg, char *ip)
{
	char *lo_arg;
	char cmp[MAX_INPUT_LENGTH];
	int fitted;

	if (ip[0] != '|')
		return FALSE;
	cmp[0] = '\0';
	lo_arg = strlower(arg);
	do {
		ip += strlen(cmp) + 1;
		fitted = sscanf(ip, "%[^*]", cmp);
		if (strstr(lo_arg, cmp) != NULL) {
			return TRUE;
		}
	} while (fitted > 0);
	return FALSE;
}

bool all_in(char *arg, char *ip)
{
	char *lo_arg;
	char cmp[MAX_INPUT_LENGTH];
	int fitted;

	if (ip[0] != '&')
		return FALSE;
	cmp[0] = '\0';
	lo_arg = strlower(arg);
	do {
		ip += strlen(cmp) + 1;
		fitted = sscanf(ip, "%[^*]", cmp);
		if (strstr(lo_arg, cmp) == NULL) {
			return FALSE;
		}
	} while (fitted > 0);
	return TRUE;
}	

void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char poly [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_STEALTHY && ch->fight_timer > 0 )
       {
          stc("#w#zYou are too sneaky to use anything but #ysay #wat this critical moment.#n\n\r", ch );
          return;
       }

	if ( IS_SET( ch->more3, MORE3_SILENCE ) )
       {
          stc("#w#zYou have lost your voice!#n\n\r", ch );
          return;
       }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( IS_SET(victim->deaf, CHANNEL_TELL) && !IS_IMMORTAL(ch) )
    {
	if (IS_NPC(victim) || IS_NPC(ch) || strlen(victim->pcdata->marriage) < 2
	    || str_cmp(ch->name, victim->pcdata->marriage))
	{
	    act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_HEX) )
       argument = talk_garbage(ch,argument);

    sprintf(poly,"You tell $N '#C$t#n'.");
    act(poly, ch, argument, victim, TO_CHAR );

    position		= victim->position;
    victim->position	= POS_STANDING;

    sprintf(poly,"$n tells you '#C$t#n'.");
    act( poly, ch, argument, victim, TO_VICT );

    victim->position	= position;
    victim->reply	= ch;

    return;
}



void do_whisper( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_STEALTHY && ch->fight_timer > 0 )
       {
          stc("#w#zYou are too sneaky to use anything but #ysay #wat this critical moment.#n\n\r", ch );
          return;
       }

	if ( IS_SET( ch->more3, MORE3_SILENCE ) )
       {
          stc("#w#zYou have lost your voice!#n\n\r", ch );
          return;
       }

    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Not with a gag on!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Syntax: whisper <person> <message>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_AWAKE(victim) )
    {
	act( "$E cannot hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_HEX) )
       argument = talk_garbage(ch,argument);

    act( "You whisper to $N '$t'.", ch, argument, victim, TO_CHAR );
    act( "$n whispers to you '$t'.", ch, argument, victim, TO_VICT );
    act( "$n whispers something to $N.", ch, NULL, victim, TO_NOTVICT );

    return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    char poly [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_STEALTHY && ch->fight_timer > 0 )
       {
          stc("#w#zYou are too sneaky to use anything but #ysay #wat this critical moment.#n\n\r", ch );
          return;
       }

	if ( IS_SET( ch->more3, MORE3_SILENCE ) )
       {
          stc("#w#zYou have lost your voice!#n\n\r", ch );
          return;
       }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_HEX) )
       argument = talk_garbage(ch,argument);

    sprintf(poly,"You reply to $N '#C$t#n'.");

    act( poly, ch, argument, victim, TO_CHAR );
    position		= victim->position;
    victim->position	= POS_STANDING;

    sprintf(poly,"$n replies to you '#C$t#n'.");
    act( poly, ch, argument, victim, TO_VICT );

    victim->position	= position;
    victim->reply	= ch;

    return;
}



void do_emote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *plast;

    char name   [80];
    char poly   [MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    bool is_ok;


    if ( IS_HEAD(ch, LOST_TONGUE) || IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )

   {
	send_to_char( "Pose what?\n\r", ch );
	return;
    }

    for ( plast = argument; *plast != '\0'; plast++ )
	;

    strcpy( buf, argument );
    if ( isalpha(plast[-1]) )
	strcat( buf, "." );

    act( "$n $T", ch, NULL, buf, TO_CHAR );

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
    	act( "$n $T", ch, NULL, buf, TO_ROOM );
	return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    ch->pcdata->chobj->in_room != NULL &&
	    !IS_NPC(to) && to->pcdata->chobj != NULL && 
	    to->pcdata->chobj->in_room != NULL &&
	    ch->in_room == to->in_room)
		is_ok = TRUE; else is_ok = FALSE;

	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    ch->pcdata->chobj->in_obj != NULL &&
	    !IS_NPC(to) && to->pcdata->chobj != NULL && 
	    to->pcdata->chobj->in_obj != NULL &&
	    ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		is_ok = TRUE; else is_ok = FALSE;

	if (!is_ok) continue;

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
	else if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
	    sprintf(name, ch->morph);
        else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s\n\r", name,buf);
	send_to_char(poly,to);
    }
    return;
}



void do_xemote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char oldarg[MAX_STRING_LENGTH];
    char *plast;

    char name   [80];
    char you    [80];
    char them   [80];
    char poly   [MAX_INPUT_LENGTH];
    char arg    [MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *victim;
    bool is_ok;

    argument = one_argument( argument, arg );


    if ( IS_HEAD(ch, LOST_TONGUE) || IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if (strlen(argument) > MAX_INPUT_LENGTH)
    {
	send_to_char( "Line too long.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' || arg[0] == '\0' )
    {
	send_to_char( "Syntax: emote <person> <sentence>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_NPC(ch)) strcpy(you, ch->short_descr);
	else strcpy(you, ch->name);
    if (IS_NPC(victim)) strcpy(you, victim->short_descr);
	else strcpy(you, victim->name);
/*
oldarg = argument;
*/
    strcpy( oldarg,argument );
    strcpy( buf, argument );
    for ( plast = argument; *plast != '\0'; plast++ )
	;

    if ( isalpha(plast[-1]) )
	strcat( buf, "." );
    argument = socialc(ch, buf, you, them);

    strcpy( buf, argument );
    strcpy( buf2, "You ");
    buf[0] = LOWER(buf[0]);
    strcat( buf2, buf );
    capitalize( buf2 );
    act( buf2, ch, NULL, victim, TO_CHAR );

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
	strcpy( buf, oldarg );
	for ( plast = argument; *plast != '\0'; plast++ )
	    ;
	if ( isalpha(plast[-1]) )
	    strcat( buf, "." );

	argument = socialn(ch, buf, you, them);

	strcpy( buf, argument );
	strcpy( buf2, "$n ");
	buf[0] = LOWER(buf[0]);
	strcat( buf2, buf );
	capitalize( buf2 );
    	act( buf2, ch, NULL, victim, TO_NOTVICT );

	strcpy( buf, oldarg );
	for ( plast = argument; *plast != '\0'; plast++ )
	    ;
	if ( isalpha(plast[-1]) )
	    strcat( buf, "." );

	argument = socialv(ch, buf, you, them);

	strcpy( buf, argument );
	strcpy( buf2, "$n ");
	buf[0] = LOWER(buf[0]);
	strcat( buf2, buf );
	capitalize( buf2 );
    	act( buf2, ch, NULL, victim, TO_VICT );
	return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    ch->pcdata->chobj->in_room != NULL &&
	    !IS_NPC(to) && to->pcdata->chobj != NULL && 
	    to->pcdata->chobj->in_room != NULL &&
	    ch->in_room == to->in_room)
		is_ok = TRUE; else is_ok = FALSE;

	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    ch->pcdata->chobj->in_obj != NULL &&
	    !IS_NPC(to) && to->pcdata->chobj != NULL && 
	    to->pcdata->chobj->in_obj != NULL &&
	    ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		is_ok = TRUE; else is_ok = FALSE;

	if (!is_ok) continue;

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
	else if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
	    sprintf(name, ch->morph);
        else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s\n\r", name,buf);
	send_to_char(poly,to);
    }
    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_idea( CHAR_DATA *ch, char *argument )
{
    append_file( ch, IDEA_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here.  Just save and quit.\n\r", ch );
    return;
}



void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}

char *talk_garbage( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;

    struct spk_type
    {
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ "a" }, { "b" }, { "c" }, { "d" },
	{ "e" }, { "f" }, { "g" }, { "h" },
	{ "i" }, { "j" }, { "k" }, { "l" },
	{ "m" }, { "n" }, { "o" }, { "p" },
	{ "q" }, { "r" }, { "s" }, { "t" },
	{ "u" }, { "v" }, { "w" }, { "x" },
	{ "y" }, { "z" }
    };

    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName++ )
    {
        if ( *pName == ' ' )
           strcat( buf, " " );
           else strcat( buf, spk_table[number_range(0,25)].new );
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);
    return argument;
}

void do_char_exit( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) || IS_IMMORTAL(ch) )
       return;

    if ( ch->pcdata->logout[0] != '\0' )
       {
             DESCRIPTOR_DATA *d;
             int test = 0;

             if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
                {
                   REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                   test = 1;
                }

             buf[0] = '\0';
             strcat( buf, "#p[#w<#gInfo#w>#p]#c " );
             strcat( buf, ch->pcdata->logout );
             strcat( buf, " #p[#w<#gInfo#w>#p]#n" );
    
             for ( d = descriptor_list; d != NULL; d = d->next )
             {
                if ( d->connected != CON_PLAYING || d->character == ch )
                   continue;

                act( buf,ch,NULL,d->character,TO_VICT );
             }

             if ( test == 1 )
                SET_BIT(ch->affected_by, AFF_POLYMORPH);

             return;
       }
       else if ( ch->class == 0 )
 	         sprintf( buf, "#2%s #7has left for #2new adventures#7.#n", ch->name );
       else {
               switch( ch->class )
               {
                  case CLASS_FIEND:
                  case CLASS_DEMON: sprintf(buf,"#rSatan beckons #w%s #rback to hell to collect his souls!#n", ch->name );
                                    break;
                  case CLASS_DARAMON:
                  case CLASS_MONK: sprintf(buf,"#C%s #wstarts meditating for future battles.#n", ch->name );
                                   break;
                  case CLASS_PALADIN: sprintf(buf,"#g%s #whas returned to the the castle!#n", ch->name );
                                      break;
                  case CLASS_WEREWOLF: sprintf(buf,"#r%s #Rdashes into the forest to rest their wounds.#n", ch->name );
                                       break;
                  case CLASS_DRAGON: sprintf(buf,"#pFull from their last meal, #g%s #pgoes to slumber inside a dark cave.#n", ch->name );
                                     break;
                  case CLASS_ANGEL: sprintf(buf,"#wSpreading their wings, #y%s #wflies back towards Heaven.#n", ch->name );
                                    break;
                  case CLASS_WIZARD: sprintf(buf,"#c%s #bgone back researching the musty old tomes for answers.#n", ch->name );
                                     break;
                  case CLASS_VAMPIRE: sprintf(buf,"#r%s #Rgoes back into their coffin for a well deserved rest.#n", ch->name );
                                      break;
                  case CLASS_NIGHT_BLADE:
                  case CLASS_SHADOW:
                  case CLASS_SHADOW_WAR:
                  case CLASS_NINJA: sprintf(buf,"#eTheir tasks done, #b%s #efades back in the shadows they came from.#n", ch->name );
                                    break;
                  case CLASS_DROW: sprintf(buf,"#eMission accomplish, #w%s #ereturns to Lloth to report.#n", ch->name );
                                   break;
                  case CLASS_PHOENIX: sprintf(buf,"#RPleased with the results of their life, #y%s #Rslumbers in the flames.#n", ch->name );
                                      break;
                  case CLASS_DUERGAR: sprintf(buf,"#gTotally drunk now and with nothing else to do, #e%s #gdecides to go home.#n", ch->name );
                                      break;
                  case CLASS_HOBBIT: sprintf(buf,"#wLuckily, #e%s #wresists the temptation of the ring... till next time...#n", ch->name );
                                     break;
                  case CLASS_SAMURAI: sprintf(buf,"#CSheathing their katana, #w%s #Cconcentrates on the next battle!#n", ch->name );
                                      break;
                  case CLASS_SAIYAN: sprintf(buf,"#rNothing left to destroy, #p%s #rreturns to their home planet!#n", ch->name );
                                     break;
                  case CLASS_KURUDA:
                  case CLASS_ASSASSIN: sprintf(buf,"#eTheir rage satisfied for now, #p%s #ereturns to Nests to report.#n", ch->name );
                                       break;
                  case CLASS_MASTER:
                  case CLASS_DRAGOON: sprintf(buf,"#cProud of their accomplishments, #P%s #cleaves for some more cigarettes!#n", ch->name );
                                      break;
                  case CLASS_DRAGON_KNIGHT: sprintf(buf,"#gReturning to their #7human#g form #c%s #gslumbers in the #cDragon Shrine.#n", ch->name );
                                            break;
                  case CLASS_UNDEAD_KNIGHT: sprintf(buf,"#eStill checking salvation, #y%s #eprays for a cure to his curse.#n", ch->name );
                                            break;
                  case CLASS_NECROMANCER: sprintf(buf,"#w%s #ehas left with their undead army.#n", ch->name );
                                          break;
                  case CLASS_SNOW_ELF: sprintf(buf,"#CSatisfied for now, #g%s #Cburrows themselves into the snow.#n", ch->name );
                                       break;
                  case CLASS_WISP: sprintf(buf,"#w%s #ycooes 'Mia gono' as they zip away into the night.#n", ch->name );
                                   break;
                  case CLASS_DAYWALKER: sprintf(buf, "#R%s #esheaths his blade and fades away.#n", ch->name );
                                        break;
                  default: sprintf( buf, "#2%s #7has left for #2new adventures#7.#n", ch->name );
                           break;
               }
            }

    do_info(ch,buf);
}

void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mount;
    OBJ_DATA *obj, *obj_next;

    if ( IS_NPC(ch) )
	return;

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( ch->fight_timer > 0 )
       {
	    send_to_char("#P#zNot until your fight timer expires.#n\n\r", ch );
	    return;
       }

    if ( ch->login_timer > 0 )
       {
	    send_to_char("#w#zYou can't leave so quickly after being capped.#n\n\r", ch );
	    return;
       }

    if ( ch->seeself > 0 || ch->revdam > 0 || ch->noprotection > 0 || ch->revregen > 0 )
       {
	    send_to_char("#w#zYou can't leave while inflicted with #Pmadness#w.#n\n\r", ch );
	    return;
       }

    if (IS_SET(ch->extra, EXTRA_OSWITCH)) do_humanform(ch,"");
    if (ch->gladiator  != NULL)
    ch->gladiator       = NULL; /* set player to bet on to NULL */

    if ( ch->holded != NULL )
       {
          act("You release your hold on your target.",ch->holded,NULL,NULL,TO_CHAR);
          ch->holded->holding = NULL;
       }

    if ( ch->holding != NULL )
       {
          act("You break free of the hold on you.",ch->holding,NULL,NULL,TO_CHAR);
          ch->holding->holded = NULL;
       }

    if ( ch->protecting != NULL )
       {
          act("$n stops protecting you from all attacks.",ch,NULL,ch->protecting,TO_VICT);
          ch->protecting->protector = NULL;
       }

    if ( ch->protector != NULL )
       {
          act("You stop protecting $n from all attacks.",ch,NULL,ch->protector,TO_VICT);
          ch->protector->protecting = NULL;
       }

    if ( auction_list != NULL && auction_list->buyer == ch )
       {
          auction_list->seller->bank = UMIN( 500000000, auction_list->seller->bank + ( auction_list->buyer->bank / 2 ) +
                                                        ( auction_list->buyer->gold / 2 ) );
          auction_list->buyer->bank /= 2;
          auction_list->buyer->gold /= 2;
          sprintf( buf, "#CBids on %s #Cremoved due to cheating from #y%s#C...#n", auction_list->object->short_descr,
                         auction_list->buyer->pcdata->switchname );
          do_acinfo( NULL, buf );
          free_auction();
       }
       else remove_auction( ch );

    if ( IS_SET( ch->arena, AFF2_COMPETITOR ) )
       {
          int num = 0;

          num_contestants--;

          while ( num < 16 && participants[ num ] != ch )
            num++;

          if ( num < 16 )
             participants[ num ] = NULL;
       }

    if ( ch->attacker != NULL )
       ch->attacker->attacker = NULL;

    if ( ch->retaliate != NULL )
       ch->retaliate->retaliate = NULL;

    if ( ch->binding != NULL )
       ch->binding->binded = NULL;

    if ( ch->binded != NULL )
       ch->binded->binding = NULL;

    if ( IS_SET( ch->newbits, NEW_ALIGHT ) ) 
       do_anti_light( ch, "" );

    if ( ch->unveil != NULL )
       {
          if ( !IS_CLASS( ch, CLASS_VAMPIRE ) )
             {
                sprintf( buf, "You lose your mental link with %s.\n\r", ch->name );
                stc( buf, ch->unveil );
             }

          ch->unveil->unveil = NULL;
          ch->unveil = NULL;
       }

    // Remove non-fighter from the team battle
    if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
       remove_team_player( ch, 0 );
       else if ( IS_SET( ch->arena, AFF2_TEAM2 ) )
               remove_team_player( ch, 1 );

    if ( IS_SET( ch->arena, AFF2_INARENA ) )
       do_give_up( ch, "" );
       else undo_arena( ch );

    if ( (mount = ch->mount) != NULL ) do_dismount(ch,"");

    send_to_char( "\n\r           #rA kiss of promise;\n\r",ch);
    send_to_char( "           #bfrom the adventures of your past;\n\r",ch);
    send_to_char( "           #rA brush with true destiny;\n\r",ch);
    send_to_char( "           #bso fuitile, for it can never #elast...#n\n\r\n\r",ch);
  
    /*
     * After extract_char the ch is no longer valid!
     */

	if (ch->pcdata->in_progress)
		free_note(ch->pcdata->in_progress);

    d = ch->desc;

    if ( ch->level != 1 )
       {
          if ( ch->pcdata->obj_vnum != 0 )
             act( "$n slowly fades out of existance.", ch, NULL, NULL, TO_ROOM );
             else act( "$n has left the game.", ch, NULL, NULL, TO_ROOM );
       }

    if ( IS_CLASS(ch, CLASS_ANGEL) && angels > 0 )
       angels--;
    
    if( IS_SET(ch->act, PLR_TAG) )
    {
      REMOVE_BIT(ch->act, PLR_TAG);
      sprintf(buf, "TAG Game update:- %s has quit the TAG Game so no one is IT.", ch->name );
      do_info(ch, buf);
    }

    if ( d != NULL )
	close_socket2( d, FALSE );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
       obj_next = obj->next_content;

       if ( !IS_SET( obj->quest, QUEST_ARTIFACT ) )
          continue;

       obj_from_char( obj );
       obj_to_room( obj, ch->in_room );
    }

    save_char_obj( ch );

    if (ch->in_room != NULL) char_from_room(ch);
    char_to_room(ch,get_room_index(3));

    sprintf( log_buf, "%s has quit.", ch->name );
    log_string( log_buf );

    if ( ch->pcdata->chobj != NULL )
       extract_obj( ch->pcdata->chobj );

    if ( ch->level != 1 )
       do_char_exit( ch );

    if ( arena == FIGHT_WAR && number_percent() > 70 )
       {
          CHAR_DATA *mob;

          mob = get_mob( 101 );

          if ( mob != NULL )
             do_chat( mob, "Coward! People like that deserves to die." );
       }

    extract_char( ch, TRUE );
    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    check_leaderboard(ch,"");
    save_char_obj( ch );
    send_to_char( "Saved.\n\r", ch );
    return;
}



void do_autosave( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    save_char_obj( ch );
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( !IS_NPC(ch) && (get_age(ch)-17) < 2 )
    {
       stc("But you are a newbie.\n\r",ch);
       return;
    }

    if ( !IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
       {
          send_to_char( "#wSorry #ychump#w. You are stuck with your #Rteam #wwhile in this area.#n\n\r", ch );
          return;
       }
	
    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

	if ( IS_SET( victim->extra, EXTRA_HUNT ) )
      {
         stc( "You can't follow someone that is in a scavenger hunt.\n\r", ch );
         return;
      }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower( ch );
	return;
    }

/*
    if ( !IS_NPC(ch) && !IS_NPC(victim) && ( ch->pcdata->kingdom == 0 || ch->pcdata->kingdom != victim->pcdata->kingdom ) )
       {
         stc("But they aren't in the same kingdom as you.\n\r",ch);
         return;
       }
*/

    if ( ch->master != NULL )
       stop_follower( ch );

    add_follower( ch, victim );
    return;
}



void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    DESCRIPTOR_DATA *d;

    if ( ch->master == NULL )
       {
          bug( "Stop_follower: null master.", 0 );
          return;
       }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
       {
          REMOVE_BIT( ch->affected_by, AFF_CHARM );
          affect_strip( ch, gsn_charm_person );
       }

    if ( !IS_NPC(ch->master) && ch->master->pcdata->followers > 0 && IS_NPC( ch ) )
       ch->master->pcdata->followers--;

    if ( can_see( ch->master, ch ) )
       act( "$n stops following you.", ch, NULL, ch->master, TO_VICT );

    act( "You stop following $N.", ch, NULL, ch->master, TO_CHAR    );
    remove_group_auras( ch );

    ch->master = NULL;

    if ( ch->leader != NULL )
       {
          for ( d = descriptor_list; d != NULL; d = d->next )
          {
             if ( d->character == NULL )
                continue;

             if ( !is_same_group( ch, d->character ) )
                continue;

             if ( d->character->pcdata->personality == PER_COMPASSION )
                d->character->cmbt[1]--;
           }

          if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_COMPASSION )
             ch->cmbt[1] = 0;

          ch->leader = NULL;
       }

    return;
}



void die_follower( CHAR_DATA *ch )
{
   CHAR_DATA *fch, *fch_next, *mount;

   if ( ch->master != NULL )
      stop_follower( ch );

   if ( ( mount = ch->mount ) != NULL )
      {
         mount->mount    = NULL;
         ch->mount   = NULL;
         mount->mounted  = IS_ON_FOOT;
         ch->mounted = IS_ON_FOOT;
      }

   ch->leader = NULL;

   for ( fch = char_list; fch != NULL; fch = fch_next )
   {
      fch_next = fch->next;

      if ( fch->master == ch )
         {
            stop_follower( fch );

            if ( IS_NPC( fch ) )
               {
                  act( "#pWithout $s master, $n #pslowly fades out of existence...#n", fch, NULL, NULL, TO_ROOM );
                  extract_char( fch, TRUE );
               }
         }
   }
}

void do_order( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );

    if ( IS_SET(ch->loc_hp[0],LOST_HEAD) || !IS_CLASS( ch, CLASS_NECROMANCER ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( och == ch || ( fAll && och->master != ch ) || !IS_AFFECTED( och, AFF_CHARM ) )
         continue;

	if ( fAll || och == victim )
         {
            found = TRUE;
            act( "$n orders you to '$t'.", ch, argument, och, TO_VICT );
        
            interpret( och, argument );
         }
    }

    if ( found ) 
       send_to_char( "Ok.\n\r", ch );
       else	send_to_char( "You have no followers here.\n\r", ch );
}

void remove_group_auras( CHAR_DATA *ch )
{
   int count;

   for ( count = 0; count < 34; count++ )
   {
      ch->special_aura[count] = 0;
   }

   if ( ch->group_aura != 0 )
      {
         ch->group_aura = 0;
         if ( ch->outer_aura == -1 && ch->inner_aura == -1 )
            {
                stc("The humming fades as your aura vanish.\n\r",ch);
                act("The humming fades as $n's aura vanishes.",ch,NULL,NULL,TO_ROOM);
            }
      }

   if ( ch->outer_aura == -1 )
      return;

   aura_remove( ch );
}

void add_group_auras( CHAR_DATA *ch )
{
   int ginpart;
   int count;
   bool test = FALSE;

   if ( ch->group_aura == 0 && ( ch->leader->group_aura != 0 || ch->leader->outer_aura != -1 )
        && ch->outer_aura == -1 && ch->inner_aura == -1 )
      {
          stc("The humming increases to a deafening rate as you are surrounded by an aura.\n\r",ch);
          act("The humming increases to a deafening rate as $n is surrounded by an aura.",ch,NULL,NULL,TO_ROOM);
      }

   if ( ch->leader->outer_aura != -1 )
      {
         if ( ch->leader->outer_aura == A_PRAYER )
            ginpart = G_PRAYER;
            else if ( ch->leader->outer_aura == A_MEDITATION )
                    ginpart = G_MEDITATION;
            else if ( ch->leader->outer_aura == A_REARTH )
                    ginpart = G_REARTH;
            else if ( ch->leader->outer_aura == A_RFIRE )
                    ginpart = G_RFIRE;
            else if ( ch->leader->outer_aura == A_RCOLD )
                    ginpart = G_RCOLD;
            else if ( ch->leader->outer_aura == A_RTHUNDER )
                    ginpart = G_RTHUNDER;
            else if ( ch->leader->outer_aura == A_VIGOR )
                    ginpart = G_VIGOR;
            else if ( ch->leader->outer_aura == A_FANATICISM )
                    ginpart = G_FANATICISM;
            else if ( ch->leader->outer_aura == A_MIGHT )
                    ginpart = G_MIGHT;
            else if ( ch->leader->outer_aura == A_CONCENTRATION )
                    ginpart = G_CONCENTRATION;
            else if ( ch->leader->outer_aura == A_HFIRE )
                    ginpart = G_HFIRE;
            else if ( ch->leader->outer_aura == A_HFREEZE )
                    ginpart = G_HFREEZE;
            else if ( ch->leader->outer_aura == A_HSHOCK )
                    ginpart = G_HSHOCK;
            else if ( ch->leader->outer_aura == A_THORNS )
                    ginpart = G_THORNS;
            else if ( ch->leader->outer_aura == A_SANCTUARY )
                    ginpart = G_SANCTUARY;
            else if ( ch->leader->outer_aura == A_CONVICTION )
                    ginpart = A_CONVICTION;
            else if ( ch->leader->outer_aura == A_HEALING )
                    ginpart = G_HEALING;
            else if ( ch->leader->outer_aura == A_AIM )
                    ginpart = G_AIM;
            else if ( ch->leader->outer_aura == A_CLEANSING )
                    ginpart = G_CLEANSING;
            else if ( ch->leader->outer_aura == A_STRENGTH )
                    ginpart = G_STRENGTH;
            else if ( ch->leader->outer_aura == A_SPEED )
                    ginpart = G_SPEED;
            else if ( ch->leader->outer_aura == A_RHOLY )
                    ginpart = G_RHOLY;
            else if ( ch->leader->outer_aura == A_RDARK )
                    ginpart = G_RDARK;
            else if ( ch->leader->outer_aura == A_FAITH )
                    ginpart = G_FAITH;
            else if ( ch->leader->outer_aura == A_LTAP )
                    ginpart = G_LTAP;
            else if ( ch->leader->outer_aura == A_DVISION )
                    ginpart = G_DVISION;
            else if ( ch->leader->outer_aura == A_BSHIELD )
                    ginpart = G_BSHIELD;
            else if ( ch->leader->outer_aura == A_ADAMAGE )
                    ginpart = G_ADAMAGE;
            else if ( ch->leader->outer_aura == A_WEAKEN )
                    ginpart = G_WEAKEN;
            else if ( ch->leader->outer_aura == A_IMADIEN )
                    ginpart = G_IMADIEN;
            else if ( ch->leader->outer_aura == A_RAGE )
                    ginpart = G_RAGE;
                    else ginpart = 0;

         ch->group_aura = ginpart;
      }

   ch->group_aura += ch->leader->group_aura;

   for ( count = 0; count < 34; count++ )
   {
      if ( count == ch->leader->outer_aura && ch->leader->outer_aura != -1 )
         ch->special_aura[count] = ch->leader->aura[count];
         else ch->special_aura[count] = ch->leader->special_aura[count];

      if ( ch->special_aura[count] > 0 )
         test = TRUE;
   }

   if ( ch->outer_aura == A_PRAYER )
      ginpart = G_PRAYER;
      else if ( ch->outer_aura == A_MEDITATION )
              ginpart = G_MEDITATION;
      else if ( ch->outer_aura == A_REARTH )
              ginpart = G_REARTH;
      else if ( ch->outer_aura == A_RFIRE )
              ginpart = G_RFIRE;
      else if ( ch->outer_aura == A_RCOLD )
              ginpart = G_RCOLD;
      else if ( ch->outer_aura == A_RTHUNDER )
              ginpart = G_RTHUNDER;
      else if ( ch->outer_aura == A_VIGOR )
              ginpart = G_VIGOR;
      else if ( ch->outer_aura == A_FANATICISM )
              ginpart = G_FANATICISM;
      else if ( ch->outer_aura == A_MIGHT )
              ginpart = G_MIGHT;
      else if ( ch->outer_aura == A_CONCENTRATION )
              ginpart = G_CONCENTRATION;
      else if ( ch->outer_aura == A_HFIRE )
              ginpart = G_HFIRE;
      else if ( ch->outer_aura == A_HFREEZE )
              ginpart = G_HFREEZE;
      else if ( ch->outer_aura == A_HSHOCK )
              ginpart = G_HSHOCK;
      else if ( ch->outer_aura == A_THORNS )
              ginpart = G_THORNS;
      else if ( ch->outer_aura == A_SANCTUARY )
              ginpart = G_SANCTUARY;
      else if ( ch->outer_aura == A_CONVICTION )
              ginpart = A_CONVICTION;
      else if ( ch->outer_aura == A_HEALING )
              ginpart = G_HEALING;
      else if ( ch->outer_aura == A_AIM )
              ginpart = G_AIM;
      else if ( ch->outer_aura == A_CLEANSING )
              ginpart = G_CLEANSING;
      else if ( ch->outer_aura == A_STRENGTH )
              ginpart = G_STRENGTH;
      else if ( ch->outer_aura == A_SPEED )
              ginpart = G_SPEED;
      else if ( ch->outer_aura == A_RHOLY )
              ginpart = G_RHOLY;
      else if ( ch->outer_aura == A_RDARK )
              ginpart = G_RDARK;
      else if ( ch->outer_aura == A_FAITH )
              ginpart = G_FAITH;
      else if ( ch->outer_aura == A_LTAP )
              ginpart = G_LTAP;
      else if ( ch->outer_aura == A_DVISION )
              ginpart = G_DVISION;
      else if ( ch->outer_aura == A_BSHIELD )
              ginpart = G_BSHIELD;
      else if ( ch->outer_aura == A_ADAMAGE )
              ginpart = G_ADAMAGE;
      else if ( ch->outer_aura == A_WEAKEN )
              ginpart = G_WEAKEN;
      else if ( ch->outer_aura == A_IMADIEN )
              ginpart = G_IMADIEN;
      else if ( ch->outer_aura == A_RAGE )
              ginpart = G_RAGE;
              else return;

   aura_add( ch, ginpart );
}

void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	
	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );
	
	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( ch == gch || is_same_group( gch, ch ) )
	    {
		sprintf( buf,
		"[%-16s] %4d/%4d hp %4d/%4d mana %4d/%4d mv %5d xp\n\r",
		    capitalize( PERS(gch, ch) ),
		    gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp    );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC( victim ) && victim->in_room->vnum >= 33500 && victim->in_room->vnum <= 33800 )
       {
          send_to_char( "#wSorry #ychump#w. You are stuck with your #Rteam #wwhile in this area.#n\n\r", ch );
          return;
       }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
         if ( d->character == NULL )
            continue;

         if ( !is_same_group( ch, d->character ) )
            continue;

         if ( d->character->pcdata->personality == PER_COMPASSION )
            d->character->cmbt[1]--;
      }

	act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
	act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
	act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );
      remove_group_auras( victim );
	victim->leader = NULL;

      if ( !IS_NPC( victim ) && victim->pcdata->personality == PER_COMPASSION )
         victim->cmbt[1] = 0;

	return;
    }

    victim->leader = ch;
    act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
    act( "You join $n's group.", ch, NULL, victim, TO_VICT    );
    act( "$N joins your group.", ch, NULL, victim, TO_CHAR    );
    add_group_auras( victim );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->character == NULL )
          continue;

       if ( !is_same_group( ch, d->character ) )
          continue;

       if ( d->character->pcdata->personality == PER_COMPASSION )
          d->character->cmbt[1]++;
    }
}

void do_team( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    int count;

    if ( IS_NPC(ch) )
       return;

    if ( IS_SET( ch->arena, AFF2_TEAM1 ) && team_list != NULL )
       {
          sprintf( buf, "%s:\n\r", team_list->team_name[0] );
          stc( buf, ch );  

          for ( count = 0; count < 4; count++ )
          {
             if ( team_list->challenger[count] == NULL )
                continue;
                else gch = team_list->challenger[count];

		 sprintf( buf, "#g[#y%-16s#g] #R%4d/%4d hp #C%4d/%4d mana #e%4d/%4d mv#n\n\r",
		          gch->pcdata->switchname,
		          gch->hit,   gch->max_hit,
		          gch->mana,  gch->max_mana,
		          gch->move,  gch->max_move );

		 send_to_char( buf, ch );
          }
   
          return;
       }

    if ( IS_SET( ch->arena, AFF2_TEAM2 ) && team_list != NULL )
       {
          sprintf( buf, "%s:\n\r", team_list->team_name[1] );
          stc( buf, ch );  

          for ( count = 0; count < 4; count++ )
          {
             if ( team_list->opponent[count] == NULL )
                continue;
                else gch = team_list->opponent[count];

		 sprintf( buf, "#g[#y%-16s#g] #R%4d/%4d hp #C%4d/%4d mana #e%4d/%4d mv#n\n\r",
		          gch->pcdata->switchname,
		          gch->hit,   gch->max_hit,
		          gch->mana,  gch->max_mana,
		          gch->move,  gch->max_move );

		 send_to_char( buf, ch );
          }
   
          return;
       }

    if ( ch->pcdata->team == 0 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->team > 8 )
       {
          stc("You are by yourself dude.\n\r",ch);
          return;
       }

    switch( ch->pcdata->team )
    {
       default:
       case 1: stc(" #rRED#n team:\n\r", ch); break;
       case 2: stc(" #bBLUE#n team:\n\r", ch); break;
       case 3: stc(" #yYELLOW#n team:\n\r", ch); break;
       case 4: stc(" #eGREY#n team:\n\r", ch); break;
       case 5: stc(" #pPURPLE#n team:\n\r", ch); break;
       case 6: stc(" #wWHITE#n team:\n\r", ch); break;
       case 7: stc(" #gGREEN#n team:\n\r", ch); break;
       case 8: stc(" #CCYAN#n team:\n\r", ch); break;
    }

    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
       if ( IS_NPC(gch) )
          continue;

	 if ( gch->pcdata->team == ch->pcdata->team )
	    {
		 sprintf( buf, "#g[#y%-16s#g] #R%4d/%4d hp #C%4d/%4d mana #e%4d/%4d mv#n\n\r",
		          gch->pcdata->switchname,
		          gch->hit,   gch->max_hit,
		          gch->mana,  gch->max_mana,
		          gch->move,  gch->max_move );
		 send_to_char( buf, ch );
	    }
    }
}

/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount;
    int share;
    int extra;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }
    
    amount = atoi( arg );

    if ( amount < 0 )
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    if ( ch->gold < amount )
    {
	send_to_char( "You don't have that much gold.\n\r", ch );
	return;
    }
  
    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) )
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
    }
	    
    share = amount / members;
    extra = amount % members;

    if ( share == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

    ch->gold -= amount;
    ch->gold += share + extra;

    sprintf( buf,
	"You split %d gold coins.  Your share is %d gold coins.\n\r",
	amount, share + extra );
    send_to_char( buf, ch );

    sprintf( buf, "$n splits %d gold coins.  Your share is %d gold coins.",
	amount, share );

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group( gch, ch ) )
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->act, PLR_NO_TELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
    sprintf( buf, "#G%s tells the group #R'#G%s#R'\n\r#n", ch->name, argument );
//    for ( gch = char_list; gch != NULL; gch = gch->next )
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->character != NULL) gch = d->character;
        else continue;
	if ( is_same_group( gch, ch ) )
	    send_to_char( buf, gch );
    }

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( endozone > 0 )
       return FALSE;

    if ( ach == bch )
       return TRUE;

    if ( IS_SET( ach->more3, MORE3_CONFUSE ) || IS_SET( bch->more3, MORE3_CONFUSE ) )
       return FALSE;

    if ( !IS_NPC( ach ) && ach->pcdata->personality == PER_PSYCHO && ach->cmbt[1] > 0 )
       return FALSE;

    if ( !IS_NPC( bch ) && bch->pcdata->personality == PER_PSYCHO && bch->cmbt[1] > 0 )
       return FALSE;

    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

void do_changelight(CHAR_DATA *ch, char *argument )
{
    if (IS_SET(ch->in_room->room_flags, ROOM_DARK))
    {
	REMOVE_BIT(ch->in_room->room_flags, ROOM_DARK);
	act("The room is suddenly filled with light!",ch,NULL,NULL,TO_CHAR);
	act("The room is suddenly filled with light!",ch,NULL,NULL,TO_ROOM);
	return;
    }
    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
    act("The lights in the room suddenly go out!",ch,NULL,NULL,TO_CHAR);
    act("The lights in the room suddenly go out!",ch,NULL,NULL,TO_ROOM);
    return;
}

void open_lift( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (is_open(ch)) return;

    act("The doors open.",ch,NULL,NULL,TO_CHAR);
    act("The doors open.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_ROOM);
    if (!same_floor(ch,in_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_ROOM);
    if (is_open(ch)) act("The doors open.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    open_door(ch,TRUE);
    move_door(ch);
    open_door(ch,TRUE);
    thru_door(ch,in_room);
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}


void close_lift( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (!is_open(ch)) return;
    act("The doors close.",ch,NULL,NULL,TO_CHAR);
    act("The doors close.",ch,NULL,NULL,TO_ROOM);
    open_door(ch,FALSE);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void move_lift( CHAR_DATA *ch, int to_room )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_CHAR);
    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_ROOM);
    if (!same_floor(ch,to_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_CHAR);
    if (!same_floor(ch,to_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    open_door(ch,FALSE);
    thru_door(ch,to_room);
    return;
}

bool same_floor( CHAR_DATA *ch, int cmp_room )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[0] == cmp_room ) return TRUE;
	else return FALSE;
    }
    return FALSE;
}

bool is_open( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[2] == 0 ) return TRUE;
	else return FALSE;
    }
    return FALSE;
}

void move_door( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *pRoomIndex;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	pRoomIndex = get_room_index(obj->value[0]);
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	return;
    }
    return;
}

void thru_door( CHAR_DATA *ch, int doorexit )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	obj->value[0] = doorexit;
	return;
    }
    return;
}

void open_door( CHAR_DATA *ch, bool be_open )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[2] == 0 && !be_open ) obj->value[2] = 3;
	else if ( obj->value[2] == 3 && be_open ) obj->value[2] = 0;
	return;
    }
    return;
}

char *socialc( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "you are",	"$E is"		},
	{ "you.",	"$M."		},
	{ "you,",	"$M,"		},
	{ "you ",	"$M "		},
	{ " you",	" $M"		},
	{ "your ",	"$S "		},
	{ " your",	" $S"		},
	{ "yours.",	"theirs."	},
	{ "yours,",	"theirs,"	},
	{ "yours ",	"theirs "	},
	{ " yours",	" theirs"	},
	{ "begins",	"begin"		},
	{ "caresses",	"caress"	},
	{ "gives",	"give"		},
	{ "glares",	"glare"		},
	{ "grins",	"grin"		},
	{ "licks",	"lick"		},
	{ "looks",	"look"		},
	{ "loves",	"love"		},
	{ "plunges",	"plunge"	},
	{ "presses",	"press"		},
	{ "pulls",	"pull"		},
	{ "runs",	"run"		},
	{ "slaps",	"slap"		},
	{ "slides",	"slide"		},
	{ "smashes",	"smash"		},
	{ "squeezes",	"squeeze"	},
	{ "stares",	"stare"		},
	{ "sticks",	"stick"		},
	{ "strokes",	"stroke"	},
	{ "tugs",	"tug"		},
	{ "thinks",	"think"		},
	{ "thrusts",	"thrust"	},
	{ "whistles",	"whistle"	},
	{ "wraps",	"wrap"		},
	{ "winks",	"wink"		},
	{ "wishes",	"wish"		},
	{ " winks",	" wink"		},
	{ " his",	" your"		},
	{ "his ",	"your "		},
	{ " her",	" your"		},
	{ "her ",	"your "		},
	{ " him",	" your"		},
	{ "him ",	"your "		},
	{ "the",	"the"		},
	{ " he",	" you"		},
	{ "he ",	"you "		},
	{ " she",	" you"		},
	{ "she ",	"you "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}


	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *socialv( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ " his",	" $s"		},
	{ "his ",	"$s "		},
	{ " her",	" $s"		},
	{ "her ",	"$s "		},
	{ " him",	" $m"		},
	{ "him ",	"$m "		},
	{ " he",	" $e"		},
	{ "he ",	"$e "		},
	{ " she",	" $e"		},
	{ "she ",	"$e "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *socialn( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "you are",	"$N is"		},
	{ "you.",	"$N."		},
	{ "you,",	"$N,"		},
	{ "you ",	"$N "		},
	{ " you",	" $N"		},
	{ "your.",	"$N's."		},
	{ "your,",	"$N's,"		},
	{ "your ",	"$N's "		},
	{ " your",	" $N's"		},
	{ "yourself",	"$Mself"	},
	{ " his",	" $s"		},
	{ "his ",	"$s "		},
	{ " her",	" $s"		},
	{ "her ",	"$s "		},
	{ " him",	" $m"		},
	{ "him ",	"$m "		},
	{ " he",	" $e"		},
	{ "he ",	"$e "		},
	{ " she",	" $e"		},
	{ "she ",	"$e "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}


void do_ignore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int pos;
    bool found = FALSE;


    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;

    if (IS_NPC(rch))
	return;

    smash_tilde( argument );

    argument = one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
	if (rch->pcdata->forget[0] == NULL)
	{
	    send_to_char("You are not ignoring anyone.\n\r",ch);
	    return;
	}
	send_to_char("You are currently ignoring:\n\r",ch);

	for (pos = 0; pos < MAX_FORGET; pos++)
	{
	    if (rch->pcdata->forget[pos] == NULL)
		break;

	    sprintf(buf,"    %s\n\r",rch->pcdata->forget[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;

	if (!str_cmp(arg,rch->pcdata->forget[pos]))
	{
	    send_to_char("You have already ignored that person.\n\r",ch);
	    return;
	}
    }

    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;
	
	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if (!str_cmp(arg,wch->name))
	{
	    found = TRUE;
	    if (wch == ch)
	    {
		send_to_char("You can't ignore yourself!\n\r", ch );
		return;
	    }
	    if (wch->level >= LEVEL_IMMORTAL)
	    {
		send_to_char("That person is very hard to ignore.\n\r",ch);
		return;
	    }
	}
    }

    if (!found)
    {
	send_to_char("No one by that name is playing.\n\r",ch);
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;
     }

     if (pos >= MAX_FORGET)
     {
	send_to_char("Sorry, you have reached the ignore limit.\n\r",ch);
	return;
     }
  
     /* make a new forget */
     rch->pcdata->forget[pos]		= str_dup(arg);
     sprintf(buf,"You are now ignoring %s.\n\r",arg);
     send_to_char(buf,ch);
}

void do_unignore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    int pos;
    bool found = FALSE;
 
    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;
 
    if (IS_NPC(rch))
	return;
 
    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	if (rch->pcdata->forget[0] == NULL)
	{
	    send_to_char("You are not ignoring anyone.\n\r",ch);
	    return;
	}
	send_to_char("You are currently ignoring:\n\r",ch);

	for (pos = 0; pos < MAX_FORGET; pos++)
	{
	    if (rch->pcdata->forget[pos] == NULL)
		break;

	    sprintf(buf,"    %s\n\r",rch->pcdata->forget[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;

	if (found)
	{
	    rch->pcdata->forget[pos-1]		= rch->pcdata->forget[pos];
	    rch->pcdata->forget[pos]		= NULL;
	    continue;
	}

	if(!strcmp(arg,rch->pcdata->forget[pos]))
	{
	    send_to_char("Ignore removed.\n\r",ch);
	    free_string(rch->pcdata->forget[pos]);
	    rch->pcdata->forget[pos] = NULL;
	    found = TRUE;
	}
    }

    if (!found)
	send_to_char("No one by that name is ignored.\n\r",ch);
}


void do_notag(CHAR_DATA *ch, char *argument) /*Removes a players TAG flag if set... */
{
        if (IS_SET (ch->act, PLR_NOTAG ) )
           {
           send_to_char("You now allow people to tag you. (YAY!)\n\r", ch);
           REMOVE_BIT( ch->act, PLR_NOTAG );
           return;
           }
        else
           {
           send_to_char("You nolonger let people tag you... (Spoil Sport!)\n\r", ch);
           SET_BIT( ch->act, PLR_NOTAG );
           return;
           }

}

/********************************************************************/

void game_in_play(CHAR_DATA *ch, char *name)
{
	CHAR_DATA       *on_it;
	DESCRIPTOR_DATA	*d;

	for ( d = descriptor_list; d; d = d->next )
	  {
	  if ( d->connected == CON_PLAYING && IS_SET(d->character->act, PLR_TAG) )
		  {
		  on_it = d->character;
		  strcpy(name, d->character->name);
		  }
	  }
	 return;
}

