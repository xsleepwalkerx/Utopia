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
#if !defined(WIN32)
#include <unistd.h>
#endif
#include "merc.h"

void do_zsikuku(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *rch, *rch_next;
    int count;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum != 560 )
       return;

    if ( !IS_CLASS( ch, CLASS_NINJA ) )
       {
          send_to_char("Huh?\n\r", ch);
          return;
       }

    if ( !IS_NPC( ch ) && ch->move < 1000000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    act("You suddenly crouch forward as you whip out a boomerang and raise it back.",ch,NULL,NULL,TO_CHAR);
    act("$n suddenly crouches forward as $e whips out a boomerang and raises it back.",ch,NULL,NULL,TO_ROOM);
    act("You throw the boomerang viciously as you do a backwards flip into the air.",ch,NULL,NULL,TO_CHAR);
    act("$n throws the boomerang viciously as $e does a backwards flip into the air.",ch,NULL,NULL,TO_ROOM);

    if ( !IS_NPC( ch ) )
       ch->move -= 1000000;

    for ( count = 1; count < 4; count++ )
    {
       switch( count )
       {
          case 1: act("The Boomerang flashes through the room cutting everyone its path.",ch,NULL,NULL,TO_CHAR);
                  act("The Boomerang flashes through the room cutting everyone its path.",ch,NULL,NULL,TO_ROOM);
                  break;
          case 2: act("Then the Boomerang twirls around as it SlIcEs everyone its path.",ch,NULL,NULL,TO_CHAR);
                  act("Then the Boomerang twirls around as it SlIcEs everyone its path.",ch,NULL,NULL,TO_ROOM);
                  break;
          case 3: act("The Boomerang finally RiPs through everyone its path.",ch,NULL,NULL,TO_CHAR);
                  act("The Boomerang finally RiPs through everyone its path.",ch,NULL,NULL,TO_ROOM);
                  act("Midway in the air, you lean backwards as you catch the Boomerang and land on your feet.",ch,NULL,NULL,TO_CHAR);
                  act("Midway in the air, $n leans backwards as $e catches the Boomerang and land on $s feet.",ch,NULL,NULL,TO_ROOM);
                  break;
       }

       for( rch = ch->in_room->people; rch != NULL; rch = rch_next )
       {
          rch_next = rch->next_in_room;

          if ( is_same_group( ch, rch ) )
             continue;

          if ( is_safe( ch, rch ) )
             continue;

          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_focus = DIRECT;
          ch->attack_var = SLASH;

          if ( IS_NPC( ch ) )
             hurt_person( ch, rch, 333333, TRUE );
             else hurt_person( ch, rch, 75000, TRUE );
       }
    }
}

void do_principles( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg1[0] == '\0' && arg2[0] == '\0' )
       {
          sprintf(buf,"Principles: Sora (%d), Ningenno (%d).\n\r",
                      ch->pcdata->ninja_powers[NPOWER_SORA], ch->pcdata->ninja_powers[NPOWER_NINGENNO]);

          send_to_char(buf,ch);
          return;
       }

    if ( arg2[0] == '\0' )
       {
          if ( !str_cmp(arg1,"sora") )
             {
                send_to_char("Sora - The finding, observing, and locating principle.\n\r\n\r",ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] < 1 ) 
                   send_to_char("You have none of the Sora principles.\n\r",ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 0 ) 
                   send_to_char("Koryou -Consider- The read aura power, learn about your enemies.\n\r",ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 1 ) 
                   send_to_char("Uro-Uro -Silent Walk- You leave no footprints behind.\n\r",ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 2 ) 
                   send_to_char("Kanzuite -Aware- The truesight power.\n\r", ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 3 ) 
                   send_to_char("Bomuzite -Sleep Gas- By mixing an potion, you can put everyone in a room to sleep.\n\r", ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 4 )
                   send_to_char("Trip -Trip- You trip your opponent off balancing them.\n\r", ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 5 )
                   send_to_char("Mizotamouyo -Shadowwalk- You are able to walk the paths of darkness to reach another's location.\r\n", ch);
                if ( ch->pcdata->ninja_powers[NPOWER_SORA] > 6 )
                   send_to_char("Yoga -Cleanse- Exerts a chinese stance to cleanse your body.\r\n", ch);
                return;
             }
             else if ( !str_cmp(arg1,"ningenno") )
                     {
                        send_to_char("Ningenno - The battle, attacking and getting away.\n\r\n\r",ch);
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] < 1 ) 
                           send_to_char("You have none of the Ningenno principles.\n\r",ch);
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 1 )
                           send_to_char("Hakunetsu -First Strike- A deadly surprise attack with your katana.\n\r",ch);
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 2 )
                           send_to_char("Mienaku -Vanish- An enhanced fleeing ability.\n\r",ch);
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 3 ) 
                           send_to_char("Shiroken -Throwing Star- Added attacks per round, like headbutt for demons.\n\r",ch);
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 4 )
                           send_to_char("Dokuyaku -Poison- Adds poisons to the Shiroken\n\r", ch);
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 6 )
                           send_to_char("Dinichi -Nerve Touch- Stagger a foe with a powerful ki attack\n\r", ch );
                        if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] > 7 )
                           send_to_char("Orimasu -Heal Touch- Quickly heal your wounds with this useful tech\n\r", ch );
                        return;
                     }

       sprintf(buf,"Principles: Sora (%d), Ningenno (%d).\n\r",
       ch->pcdata->ninja_powers[NPOWER_SORA], ch->pcdata->ninja_powers[NPOWER_NINGENNO]);
       send_to_char(buf,ch);
       return;
    }

    if ( !str_cmp(arg2,"improve") )
       {
          int improve;
          int cost;
          int max = 7;

          if ( !str_cmp(arg1,"sora" ) )
             improve = NPOWER_SORA;
             else if ( !str_cmp(arg1,"chikyu" ) )
                     {
                        improve = NPOWER_CHIKYU;
                        max = 8;
                     }
             else if ( !str_cmp(arg1,"ningenno" ) )
                     {
                        improve = NPOWER_NINGENNO;
                        max = 8;
                     }
             else {
                     send_to_char("Principles: Sora, Chikyu, Ningenno.\n\r",ch);
                     return;
                  }							

          cost = ( ch->pcdata->ninja_powers[improve] + 1 ) * 1000;
          arg1[0] = UPPER(arg1[0]);

          if ( ch->pcdata->ninja_powers[improve] >= max )
             {
                sprintf(buf,"You have already gained all the powers of the %s principle.\n\r", arg1);
                send_to_char(buf,ch);
                return;
             }

          ch->pcdata->ninja_powers[improve] += 1;
          sprintf(buf,"You improve your ability in the %s principle.\n\r", arg1);
          send_to_char(buf,ch);
       }
       else send_to_char("To improve a principle, type: Principle <principle type> improve.\n\r",ch);
}

void do_kanzuite( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;
	
    if ( !IS_CLASS(ch, CLASS_NINJA) )
	 {
	    send_to_char("Huh?\n\r", ch);
	    return;
	 }
	
    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
       {
          REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
          send_to_char( "You're senses return to normal.\n\r", ch );
       }
      else {
              SET_BIT(ch->act, PLR_HOLYLIGHT);
              send_to_char( "You're senses increase into incredible proportions.\n\r", ch );
           }
}

void do_mienaku( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    EXIT_DATA *pexit;
    int door;
    CHAR_DATA *victim;
    int attempt;
    int cost = 750000;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_NIGHT_BLADE) && !IS_SET( ch->more3, MORE3_SOUL ) )
	 {
	    send_to_char("Huh?\n\r", ch);
	    return;
	 }

    if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_PRIDE && !IS_NPC( victim ) )
       {
          send_to_char("A proud fighter NEVER flees.\n\r",ch);
          return;
       }

    if ( ch->move < cost )
	 {
  	    send_to_char("You are too tired to execute this technique.\n\r", ch);
	    return;
	 } 

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
          send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	    return;
       }

  if ( IS_SET( ch->in_room->room_flags, ROOM_CATACOMB ) )
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
        ch->hit = UMAX( 0, ch->hit - number_range( 500000, 750000 ) );
        
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

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char("It is #c#zimpossible#n to flee from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
          return;
       }
	
    if ( ( victim = ch->fighting ) == NULL )
       {
          send_to_char( "You aren't fighting anyone.\n\r", ch );
          return;
       }

    if ( IS_AFFECTED(ch, AFF_WEBBED) )
       {
	    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more, MORE_NET) )
       {
	    send_to_char( "You are trapped in a spiritual net.\n\r", ch );
	    return;
       }

    if ( is_memb(ch) )
       return;

    was_in = ch->in_room;
    
    send_to_char("You move to vanish from combat!\n\r", ch );

    for ( attempt =  0; attempt < 6; attempt++ ) 
    {
	door = number_door( );

      if ( ( pexit = was_in->exit[door] ) == 0
           ||   pexit->to_room == NULL
           ||   IS_SET(pexit->exit_info, EX_CLOSED)
           || ( IS_NPC(ch) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
         continue;

	move_char( ch, door );

      if ( ( now_in = ch->in_room ) == was_in )
         continue;

      if ( IS_SET(ch->act, PLR_WIZINVIS) )
         REMOVE_BIT(ch->act, PLR_WIZINVIS);

      if ( IS_SET(ch->newbits, NEW_DARKNESS) )
	   {
		REMOVE_BIT(ch->newbits, NEW_DARKNESS);

            if ( ch->in_room->drows > 0 )
               {
                  ch->in_room->drows--;

    	            if ( ch->in_room->drows == 0 && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
                     REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
               }
         }

      ch->in_room = was_in;

      if ( number_percent() > 90 && !IS_SET( victim->extra, BLINDFOLDED ) )
         {
            SET_BIT( victim->extra, BLINDFOLDED );
            act("You start to move at super speed and #w#zblindfold#n $N.",ch,NULL,victim,TO_CHAR);
            act("$n starts to move at super speed and #w#zblindfolds#n you.",ch,NULL,victim,TO_VICT);
            act("$n starts to move at super speed and #w#zblindfolds#n $N.",ch,NULL,victim,TO_NOTVICT);
         }

      act( "$n has suddenly vanish from sight!", ch, NULL, NULL, TO_ROOM );
      ch->in_room = now_in;
	
      if ( !IS_NPC(ch) )
         send_to_char( "You escape from combat!\n\r", ch ); 

      stop_fighting( ch, TRUE );
      ch->move -= cost;

      if ( is_affected(ch, skill_lookup("curse of allah") ) )
         ch->wait = 36;
         else ch->wait = 6;

	if ( victim != NULL && IS_CLASS( victim, CLASS_WISP ) && victim->pkill >= 6 && !IS_NPC( ch )
           && victim->position > POS_STUNNED )
	   {
             act("You glow a brilliant #7white#n colour cooeing '#yPeeyo peeyo'#n as you follow your enemy.",victim,NULL,NULL,TO_CHAR);
		 act("$n glows a brilliant #7white#n colour cooeing '#yPeeyo peeyo'#n as $e follows $s enemy.",victim,NULL,NULL,TO_ROOM);

             stop_fighting( victim, TRUE );
		 move_char( victim, door );

		 if ( victim->position > POS_STUNNED && victim->stunned == 0 && !IS_SET( victim->more, MORE_FROZEN )
                  && can_see( victim, ch ) )
                multi_hit( victim, ch, TYPE_UNDEFINED );
	   }

      return;
    }
}

void do_bomuzite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
	 {
	    send_to_char("Huh?\n\r", ch);
	    return;
	 }

    if ( ch->pcdata->ninja_powers[NPOWER_SORA] < 4 )
	 {
	    send_to_char("You have not learned the Sora principle to 4.\n\r", ch);
	    return;
	 }

    if ( ch->fight_timer > 0 )
       {
	    send_to_char( "Not with a fight timer.\n\r", ch );
	    return;
       }
	
    if ( arg1[0] == '\0' )
       {
          send_to_char( "Bomb who?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( ch == victim )
       {
          send_to_char( "Not on yourself!\n\r", ch );
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    if ( victim->fight_timer > 0 )
       {
	    send_to_char( "They are moving too quickly to sneak behind them.\n\r", ch );
	    return;
       }
	
    if ( ch->move < 250000 )
       {
          send_to_char("You are too tired to execute this technique.\n\r", ch);
          return;
       } 
 
    act("You toss your bomb onto the floor and put $N to sleep.",ch,NULL,victim,TO_CHAR);
    act("$n tosses a bomb onto the floor. You feel sleepy.",ch,NULL,victim,TO_VICT);
    stop_fighting( victim, TRUE );
    victim->position = POS_SLEEPING;
    victim->wait = 12;
    ch->move -= 250000;
}

void do_tsume(CHAR_DATA *ch, char *argument) 
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->ninja_powers[NPOWER_NINGENNO] < 1 )
       {
          send_to_char("Huh?\n\r", ch );
          return;
       }
    
    if ( IS_VAMPAFF(ch, VAM_CLAWS) )
       {
          send_to_char("You remove the IronClaws from your wrists.\n\r", ch );
          REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
          return;
       }
  
    send_to_char("You attach IronClaws to your wrists.\n\r", ch );
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
}

void do_hara_kiri(CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->ninja_powers[NPOWER_CHIKYU] < 6 )
       {
          send_to_char("Huh?\n\r", ch );
          return;
       }

    if ( ch->pcdata->ninja_powers[HARA_KIRI] > 0 )
       {
          send_to_char("You are already experiencing the power of HaraKiri.\n\r", ch );
          return;
       }

    if ( ch->hit < ch->max_hit / 10 ) 
       {
          send_to_char("You are hurt too badly already.\n\r", ch );
          return;
       }

    ch->pcdata->ninja_powers[HARA_KIRI] = 100;

    ch->hit /= 2;
    ch->mana /= 2;
    ch->move /= 2;
    ch->agg = 0;
    send_to_char("You cut your finger and bleed profusely.\n\r", ch );
    act("$n cuts his finger and obtains the power of HaraKiri",ch,NULL,NULL,TO_ROOM);
}

void do_trip( CHAR_DATA *ch, char *argument ) 
{
    char  arg[MAX_STRING_LENGTH];
    char  buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
	
    argument = one_argument(argument, arg);
	
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->ninja_powers[NPOWER_SORA] < 5 ) 
       {
          send_to_char("Huh?\n\r", ch);
          return;
       }

    if ( ( victim = ch->fighting ) == NULL )
       {
          if ( arg[0] == '\0' )
		 {
                send_to_char("Trip whom?\r\n", ch);
                return;
             }

          if ( ( victim = get_char_room(ch, arg) ) == NULL )
             {
                send_to_char("How can you trip someone who is not here?\r\n", ch);
                return;
             }
	}
   
    ch->wait = 4; 

    if ( 85 < number_percent() )
       {
          sprintf(buf, "You skillfully trip %s, and they go down!\r\n", victim->name);
          send_to_char(buf, ch);
          sprintf(buf, "%s makes an impressive fighting move, and you land on your butt!\r\n", ch->name);
          send_to_char(buf, victim);
          ch->wait = 6; 

          if ( !is_martial( victim ) )
             WAIT_STATE(victim, 12);

          return;
	}

    send_to_char("You attempt an impressive fighting move, but instead land on your butt!\r\n", ch);
    sprintf(buf, "%s attempts to make an impressive fighting move, but lands on his butt!\r\n", ch->name);
    send_to_char(buf, victim);
}

void do_koryou( CHAR_DATA *ch, char *argument )
{
     CHAR_DATA *victim;
     OBJ_DATA  *obj;
     char      arg [MAX_INPUT_LENGTH];
     char      buf [MAX_INPUT_LENGTH];

     argument = one_argument( argument, arg );

     if ( IS_NPC(ch) )
        return;

   if ( !IS_CLASS(ch, CLASS_NINJA) )
      {
         send_to_char("Huh?", ch);
         return;
	}

     if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->ninja_powers[NPOWER_SORA] < 1 )
        {
           send_to_char( "Huh?\n\r", ch );
           return;
        }

     if ( arg[0] == '\0' )
        {
           send_to_char( "Read the aura on what?\n\r", ch );
           return;
        }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}

	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if ( !IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
       ( IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("#wYou #gare #eunable #gto read their #yaura#g.#n\n\r",ch);
	return;
    }

    if ( IS_NPC( victim ) )
       sprintf(buf, "#C%s #gis an #PNPC#g.#n\n\r",victim->short_descr);
       else sprintf(buf, "#C%s #gis a #yMortal#g.#n\n\r", victim->name);

    send_to_char(buf,ch);

    if ( !IS_NPC( victim ) )
       {
           sprintf( buf, "#wStr:#y%d#w, Vit:#y%d#w, Min:#y%d#w, Spi:#y%d#w, Dex:#y%d#w, Eva:#y%d#w.#n\n\r",
                         get_curr_str(victim), get_curr_con(victim), get_curr_int(victim), get_curr_wis(victim),
                         get_curr_dex(victim), get_curr_eva(victim) );
           send_to_char( buf, ch );
       }

    sprintf(buf,"#wHp:#C%d#w/#y%d#w, Mana:#C%d#w/#y%d#w, Move:#C%d#w/#y%d#w.#n\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);

    sprintf(buf,"#yAlignment:#P%d#w.#n\n\r",victim->alignment);
    send_to_char(buf,ch);

    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("#C$N #gis #Ppregnant#g.#n",ch,NULL,victim,TO_CHAR);
    return;
}

void do_hakunetsu( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj = NULL;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->ninja_powers[NPOWER_NINGENNO] < 2 )
       {
          send_to_char("Huh?\n\r", ch);
          return;
       }

    if ( ch->class_2 == 0 )
       if ( ( ( obj = get_eq_char( ch, WEAR_SCABBARD_R ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
          if ( ( ( obj = get_eq_char( ch, WEAR_SCABBARD_L ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
             if ( ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "You don't have a sheathed dagger.\n\r", ch );
             return;
          }

    if ( ch->ctimer[0] > 0 )
       {
          send_to_char("You are still recovering from the last one.\n\r", ch );
          return;
       }

    if ( ch->move < 250000 )
	 {
	    stc("You are too tired to do this!\n\r",ch);
	    return;
 	 }

    if ( arg[0] == '\0' && ( victim = ch->fighting ) == NULL )
       {
	    send_to_char( "Whom to you want to strike on the back?\n\r", ch );
	    return;
       }

    if ( victim != NULL && ( ( victim = get_char_area( ch, arg ) ) == NULL || victim->in_room == NULL ) )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }

    if ( ch == victim )
       {
          send_to_char("But you're already at yourself!\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    stc("You dash out of the room as you leap through bushes and roll under structures.\n\r",ch);

    if ( victim->in_room->vnum != ch->in_room->vnum )
       {
          char_from_room(ch);
          char_to_room(ch,victim->in_room);
          do_look(ch, "auto");
       }

    ch->move -= 250000;
    ch->wait = 4; 
    ch->ctimer[0] = 3;

    act("\n\rSomersaulting into the room, you twist your arm across $N's neck, choking $M.",ch,NULL,victim,TO_CHAR);
    act("Somersaulting into the room, $n twists $s arm across $N's neck, choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("Somersaulting into the room, $n twists $s arm across your neck, choking you viciously.",ch,NULL,victim,TO_VICT);

    act("As you struggle for breath, $n violently thrusts $s katana through your back.",ch,NULL,victim,TO_VICT);
    act("As you struggle for breath, $n violently thrusts $s katana through your back.",ch,NULL,victim,TO_VICT);
    act("As $N struggles for breath, you violently thrust your katana through $S back.",ch,NULL,victim,TO_CHAR);
    act("As $N struggles for breath, you violently thrust your katana through $S back.",ch,NULL,victim,TO_CHAR);
    act("As $N struggles for breath, $n violently thrusts $s katana through $S back.",ch,NULL,victim,TO_NOTVICT);
    act("As $N struggles for breath, $n violently thrusts $s katana through $S back.",ch,NULL,victim,TO_NOTVICT);
    act("Grinning evilly, you lean $N's head backwards as you slice rapidly across the neck.",ch,NULL,victim,TO_CHAR);
    act("Grinning evilly, $n leans your head backwards as $e slices rapidly across your neck.",ch,NULL,victim,TO_VICT);
    act("Grinning evilly, $n leans $N's head backwards as $e slices rapidly across the neck.",ch,NULL,victim,TO_NOTVICT);

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = OVERHEAD;
    hurt_person( ch, victim, 650000, TRUE );
}

void do_mizotamouyo( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_SHADOW) )
      {
         send_to_char("Huh?", ch);
         return;
	}

   if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->ninja_powers[NPOWER_SORA] < 7 )
      {
         send_to_char("Huh?", ch);
         return;
	}

   if ( ch->fight_timer > 0 )
      {
         send_to_char("Not until your fight timer expires.\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char("Who do you wish to walk to via the paths of darkness?\r\n", ch);
         return;
      }
	
   if ( ( victim = get_char_world( ch, arg ) ) == NULL || victim == ch || victim->in_room == NULL
        || ch->in_room == NULL )
      {
         send_to_char( "You attempt to walk through the shadows, but fail.\n\r", ch );
         return;
      } 

   if ( victim->in_room == ch->in_room )
      {
         send_to_char( "But you are already there!\n\r", ch );
         return;
      }

   if ( in_kingdom( ch, victim, victim->in_room ) )
      return;

   send_to_char("You dissipate into the shadows.\n\r",ch);
   act("$n dissipates into the shadows, slowly vanishing from sight.",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   do_look(ch,"");

   send_to_char("You coalesce from the darkness which surrounds you.\n\r",ch);

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );
}

void do_hakiznetu( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int dam = 500000;

    if ( IS_NPC( ch ) && ch->pIndexData->vnum != 559 && ch->pIndexData->vnum != 560 ) 
       return;

    argument = one_argument(argument, arg);

    if ( !IS_NPC( ch ) )
       {
          if ( !IS_CLASS(ch, CLASS_NINJA) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ch->move < 500000 )
             {
                send_to_char("You are too tired to execute this technique.\n\r", ch);
                return;
             }
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Whom to you want to strike in the back?\n\r", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }
 
    if ( is_safe(ch,victim) )
       return;

    act("Your body shimmers out of existence as you fade into the shadows.",ch,NULL,NULL,TO_CHAR);
    act("$n's body shimmers out of existence as $e fades into the shadows.",ch,NULL,NULL,TO_ROOM);
    act("You suddenly form behind $N's back as you place your dagger deep into $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly appears from behind as $e thrusts $s dagger deep into your back.",ch,NULL,victim,TO_VICT);
    act("$n suddenly appears behind $N's back as $e thrusts $s dagger deep into $M.",ch,NULL,victim,TO_NOTVICT);
    ch->wait = 12;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = DIRECT;

    if ( !IS_NPC( ch ) )
       {
          ch->move -= 500000;
          hurt_person( ch, victim, dam, TRUE );
       }
       else hurt_person( ch, victim, 500000, TRUE );
}

void do_dinichi( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) && ch->pIndexData->vnum != 559 && ch->pIndexData->vnum != 560 ) 
       return;

    argument = one_argument(argument, arg);

    if ( !IS_NPC( ch ) )
       {
          if ( !IS_CLASS(ch, CLASS_NINJA) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] < 7 ) 
             {
                send_to_char("Huh?\n\r", ch);
                return;
             }

          if ( ch->ctimer[6] > 0 )
             {
                send_to_char("You are still recovering from the last one.\n\r", ch);
                return;
             }
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Whom to you want to strike in the back?\n\r", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( !IS_NPC( ch ) )
       ch->ctimer[6] = 4;

    act("You twiddle your fingers as you cross one arm into a straight line, knees bent.",ch,NULL,NULL,TO_CHAR);
    act("$n twiddles $s fingers as $e crosses one arm into a straight line, knees bent.",ch,NULL,NULL,TO_ROOM);
    act("You mutter '#yAHHHHhhhh aHHhhHh AhhHHhh...#n'",ch,NULL,NULL,TO_CHAR);
    act("$n mutters '#yAHHHHhhhh aHHhhHh AhhHHhh...#n'",ch,NULL,NULL,TO_ROOM);
    act("The surroundings goes blurry as multiple images shimmers around your body.'",ch,NULL,NULL,TO_CHAR);
    act("The surroundings goes blurry as multiple images shimmers around $n's body.'",ch,NULL,NULL,TO_ROOM);
    act("As the images go closer and closer together, a huge spiral of energy forms into existence.'",ch,NULL,NULL,TO_CHAR);
    act("As the images go closer and closer together, a huge spiral of energy forms into existence.'",ch,NULL,NULL,TO_ROOM);
    act("You yell '#ySei!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n yells '#ySei!#n'",ch,NULL,NULL,TO_ROOM);
    act("You push your hand towards the right as $N stumbles back from the blast of the energy.",ch,NULL,victim,TO_CHAR);
    act("$n pushes $s hand towards the right as $N stumbles back from the blast of the energy.",ch,NULL,victim,TO_NOTVICT);
    act("$n pushes $s hand towards the right as you stumble back from the blast of the energy.",ch,NULL,victim,TO_VICT);

    if ( !is_martial( victim ) )
       victim->wait = UMIN( 18, victim->wait + 12 );
}

void do_orimasu( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int cost = 100000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_NINJA) )
      {
        send_to_char( "Huh?\n\r", ch );
        return;
      }

   if ( ch->pcdata->ninja_powers[NPOWER_NINGENNO] < 8 ) 
      {
         send_to_char("Huh?\n\r", ch);
         return;
      }

   if ( IS_SET( ch->cmbt[5], SKL2_HSOUL ) )
      cost += 100000;

   ch->wait = 4; 
   act( "You crouch silently on the ground whilst concentrating on your ki.", ch, NULL, NULL, TO_CHAR );
   act( "$n crouches silently on the ground whilst concentrating on $s ki.", ch, NULL, NULL, TO_ROOM );
   act( "Your body flickers for a second as some of your wounds vanish.", ch, NULL, NULL, TO_CHAR );
   act( "$n's body flickers for a second as some of $s wounds vanish.", ch, NULL, NULL, TO_ROOM );

   hit_gain( ch, cost );
   mana_gain( ch, cost );
   move_gain( ch, cost );
}

void do_circle( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_NINJA) )
     {
	  send_to_char("Huh?\n\r", ch);
	  return;
     }

  if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->ninja_powers[NPOWER_NINGENNO] < 6 ) 
     {
        send_to_char("Huh?\n\r", ch );
        return;
     }

  if ( ch->ctimer[15] > 0 )
     {
        stc( "#wYou are the #gO#wn#ge#w... You don't resort to these #ypetty #wmindless skills.#n\n\r", ch );
        return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
        if ( ch->fighting == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
           else victim = ch->fighting;
     }

  if ( victim == ch )
     {
        send_to_char( "How can you circle yourself?\n\r", ch );
        return;
     }

  if ( is_safe( ch, victim ) )
     return;

  if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You need to wield a piercing weapon.\n\r", ch );
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

        if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_DROW ) && IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
           {
              REMOVE_BIT( ch->pcdata->powers[MONK_SKILL], MONK_INVIN );
              ch->ctimer[8] = 0;
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

  ch->wait = 12; 

  check_killer( ch, victim );
  multi_hit( ch, victim, gsn_circle );
  show_total_damage( ch, victim );
}

void do_miezite(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *rch;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
	 {
	    send_to_char("Huh?\n\r", ch);
	    return;
       }

    if ( ch->fight_timer > 0 )
	 {
          send_to_char("Not with a fight timer.\n\r", ch);
          return;
       }

    if ( ch->move < 1000000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
       {
          stc("You can only do this under the covers of darkness.\n\r",ch);
          return;
       }

    REMOVE_BIT( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS );

    act("$n concentrate furiously on $s hatred as $e quickly raises $s hand into the air.",ch,NULL,NULL,TO_ROOM);
    act("The room is suddenly filled with blazing energy.",ch,NULL,NULL,TO_ROOM);
    act("You concentrate furiously on your hatred as you quickly raise your hand into the air.",ch,NULL,NULL,TO_CHAR);
    act("The room is suddenly filled with blazing energy.",ch,NULL,NULL,TO_CHAR);
    ch->wait = 12;
    ch->move -= 1000000;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
       if ( rch->fighting != NULL )
          stop_fighting(rch,TRUE);

       if ( IS_SET(rch->newbits, NEW_DARKNESS) )
          {
             REMOVE_BIT(rch->newbits, NEW_DARKNESS);
             rch->in_room->drows--;
          }

       if ( rch == ch )
          continue;

       if ( is_same_group(ch,rch) )
          continue;

       if ( is_safe(ch, rch) )
          continue;

       act("You scream as the explosion sears your eyes.",rch,NULL,NULL,TO_CHAR);
       act("$N is stunned from the huge explosion as $E crashes into the ground.",ch,NULL,rch,TO_NOTVICT);
       act("$N is stunned from the huge explosion as $E crashes into the ground.",ch,NULL,rch,TO_CHAR);
       rch->position = POS_STUNNED;
    }
}

void do_ninja_assassinate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
       return;
	
    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
          send_to_char("Huh?\n\r", ch);
          return;
	 }

    if ( ch->pkill < 10 )
       {
          send_to_char("You hadn't unlocked the secret of the scrolls for this technique yet.\n\r",ch);
          return;
       }

    if ( ch->fight_timer > 0 )
       {
          send_to_char("Not with a fight timer.\n\r",ch);
          return;
       }

    if ( ch->move < 500000 )
	 {
          send_to_char("You are too tired to do this technique.\n\r", ch);
          return;
	 } 

    if ( ch->class_2 == 0 )
       if ( ( ( obj = get_eq_char( ch, WEAR_SCABBARD_R ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
          if ( ( ( obj = get_eq_char( ch, WEAR_SCABBARD_L ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
             if ( ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "You don't have a sheathed dagger.\n\r", ch );
             return;
          }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
       {
          send_to_char( "assassinate whom?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( IS_NPC(victim) ) 
       {
          stc("Not on mobs!\n\r",ch);
          return;
       }

    if ( ( victim->position == POS_FIGHTING || victim->fighting != NULL ) )
       {
          stc("You can't sneak up to them.\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You leap from the shadows placing your weapon into $N's back...\n\r",ch,NULL,victim,TO_CHAR);
    act("$n leaps from the shadows placing $s weapon into your back!\n\r",ch,NULL,victim,TO_VICT);
    act("$n leaps from the shadows placing $s weapon into $N's back!\n\r",ch,NULL,victim,TO_NOTVICT);

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_michi( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;
	
   if ( !IS_CLASS(ch, CLASS_NINJA) )
	{
	   send_to_char("Huh?\n\r", ch);
 	   return;
	}
	
   if ( ch->rage >= 5000 )
	{
	   send_to_char("But you are already in the state of Michi.\n\r",ch);
	   return;
	}
	
   send_to_char("You are gifted positive energy while performing the michi.\n\r",ch);
   act("$n is gifted positives energy while performing the michi.",ch,NULL,NULL,TO_ROOM);

   ch->rage = 10000;

   WAIT_STATE(ch,12);
}

void do_yoga( CHAR_DATA *ch, char *argument )
{
    char  arg[MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) )
       return;

    if ( !IS_CLASS( ch, CLASS_NINJA ) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->pcdata->ninja_powers[NPOWER_SORA] < 4 )
	 {
          send_to_char("Huh?\n\r", ch);
          return;
	 }

    argument = one_argument(argument, arg);

    if ( IS_SET( ch->more, MORE_BLIND ) )
       {
          ch->blind_timer = 0;
          REMOVE_BIT( ch->more, MORE_BLIND );
       }

    if ( IS_SET( ch->more2, MORE2_NOARMS ) )
       REMOVE_BIT( ch->more2, MORE2_NOARMS );

    if ( IS_SET( ch->monkstuff, MONK_DEATH ) )
       REMOVE_BIT( ch->monkstuff, MONK_DEATH );

    if ( is_affected( ch,skill_lookup("chill touch") ) )
	 affect_strip( ch, skill_lookup("chill touch") );

    if ( is_affected( ch,skill_lookup("drowfire") ) )
	 affect_strip( ch, skill_lookup("drowfire") );

    if ( is_affected( ch,skill_lookup("infirmity") ) )
	 affect_strip( ch, skill_lookup("infirmity") );

    if ( is_affected( ch,skill_lookup("weaken") ) )
	 affect_strip( ch, skill_lookup("weaken") );

    act("You stretch forward in a Nichi stance as you close your eyes and tilt your arms.",ch,NULL,NULL,TO_CHAR);
    act("$n stretches forward in a Nichi stance as $e closes $s eyes and tilt $s arms.",ch,NULL,NULL,TO_ROOM);
    act("Greenish energy emits of you as you invert your chi inwards.",ch,NULL,NULL,TO_CHAR);
    act("Greenish energy emits of $n as $e inverts $s chi inwards.",ch,NULL,NULL,TO_ROOM);
    ch->wait = UMAX( 4, 12 - ( ch->pkill / 2.5 ) ); 
}

void do_dig( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_NINJA) )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

   if ( ch->ctimer[1] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->ctimer[1] = 4;

   SET_BIT( ch->more2, MORE2_DIG );
   act("You quickly dig yourself into the ground as you cover up all traces.",ch,NULL,NULL,TO_CHAR);
   act("$n quickly digs $mself into the ground as $e covers up all traces.",ch,NULL,NULL,TO_ROOM);
}

void do_inclimb( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int cost = 3000000;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 )
       {
          send_to_char( "Not in the arena.\n\r", ch );
          return;
       }

    if ( ch->pcdata->kingdom == 1 && ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000 )
       {
          stc("You don't see a need of inclimbing within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 2 && ch->in_room->vnum >= 3500 && ch->in_room->vnum < 4000 )
       {
          stc("You don't see a need of inclimbing within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 3 && ch->in_room->vnum >= 4500 && ch->in_room->vnum < 5000 )
       {
          stc("You don't see a need of inclimbing within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 4 && ch->in_room->vnum >= 16500 && ch->in_room->vnum < 17000 )
       {
          stc("You don't see a need of inclimbing within your own base.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char("It is #c#zimpossible#n to flee from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
          return;
       }

    if ( ch->move < cost )
       {
          send_to_char( "You are too tired to do this.\n\r", ch );
          return;
       }

    ch->move -= cost;
    stop_fighting( ch, TRUE );
   
    send_to_char( "You flip back a few feet as you scale a wall and exit the vicinity.\n\r", ch );
    act( "$n flips back a few feet, scaling a wall as $e exits the vicinity.", ch, NULL, NULL, TO_ROOM );
    get_random_room( ch );
    do_look( ch, "auto" );

    if ( IS_SET(ch->act, PLR_WIZINVIS) && ch->fight_timer > 0 )
       REMOVE_BIT(ch->act, PLR_WIZINVIS);
}

void do_incapitate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS( ch, CLASS_NINJA ) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->ctimer[5] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Who do you wish to incapacitate?\n", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe(ch,victim) ) return;

    if ( IS_SET( victim->more2, MORE2_NOARMS ) )
       {
	    send_to_char( "Their arms are already incapacitated gaiden.\n", ch );
	    return;
       }

    act("You leap towards $N landing on your knees.",ch,NULL,victim,TO_CHAR);
    act("$n leaps towards you landing on $s knees.",ch,NULL,victim,TO_VICT);
    act("$n leaps towards $N landing on $s knees.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly grab $N's arm, breaking it viciously with a LOUD snap.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly grabs your arm, breaking it viciously with a LOUD snap.",ch,NULL,victim,TO_VICT);
    act("$n suddenly grabs $N's arm, breaking it viciously with a LOUD snap.",ch,NULL,victim,TO_NOTVICT);
    act("You scream loudly from the pain soaring through your arms.",victim,NULL,NULL,TO_CHAR);
    act("$n screams loudly from the pain soaring through $s arms.",victim,NULL,NULL,TO_ROOM);

    SET_BIT(victim->more2, MORE2_NOARMS);

    ch->wait = 4; 
    ch->ctimer[5] = 2;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = DIRECT;

    hurt_person( ch, victim, 250000, TRUE );
}

void do_pain_touch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
 
    if ( IS_NPC(ch) && ch->pIndexData->vnum != 562 )
       return;

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use Pain Touch on?\n\r",ch);
	    return;
       }

    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe(ch,victim) ) 
       return;

    act("You circle behind $N as you place your palm on $S back.",ch,NULL,victim,TO_CHAR);
    act("$n circles behind you as $e places $s palm on your back.",ch,NULL,victim,TO_VICT);
    act("$n circles behind $N as $e places $s palm on $S back.",ch,NULL,victim,TO_NOTVICT);
    act("$N winces in pain as you viciously pinch $S nerve cells, having $S body ablazed.",ch,NULL,victim,TO_CHAR);
    act("You wince in pain as $n viciously pinches your nerve cells, having your body ablazed.",ch,NULL,victim,TO_VICT);
    act("$N winces in pain as $n viciously pinches $S nerve cells, having $S body ablazed.",ch,NULL,victim,TO_NOTVICT);

    SET_BIT( victim->affected_by, AFF_FLAMING );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = DIRECT;

    hurt_person( ch, victim, 500000, TRUE );
}

void do_split_strike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 559 ) 
       return;

    if ( !IS_NPC( ch ) )
       {
          if ( !IS_CLASS(ch, CLASS_NINJA) )
             {
                send_to_char( "Huh?\n\r", ch );
                return;
             }

          if ( get_stock( ch ) < 300 )
             {
                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                return;
             }

          ch->wait = 4;
          subtract_stock( ch, 300 );
       }

    SET_BIT( ch->arena, AFF2_DM );

    act( "Images leap out of your body dashing towards your foes.", ch, NULL, NULL, TO_CHAR );
    act( "Images leap out of $n's body dashing towards $s foes.", ch, NULL, NULL, TO_ROOM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( is_safe(ch, vch) )
           continue;

        act( "Your image thrust its dagger through $N's chest!", ch, NULL, vch, TO_CHAR );
        act( "$n's image thrusts its dagger through your chest!", ch, NULL, vch, TO_VICT );
        act( "$n's image thrusts its dagger through $N's chest!", ch, NULL, vch, TO_NOTVICT );
        act( "Your image slams its fist into $N's stomach causing $M to kneel down in pain.", ch, NULL, vch, TO_CHAR );
        act( "$n's image slams its fist into your stomach causing you to kneel down in pain.", ch, NULL, vch, TO_VICT );
        act( "$n's image slams its fist into $N's stomach causing $M to kneel down in pain.", ch, NULL, vch, TO_NOTVICT );
        act( "Your image repeatively pummels $N with its fists!!!", ch, NULL, vch, TO_CHAR );
        act( "$n's image repeatively pummels you with its fists!!!", ch, NULL, vch, TO_VICT );
        act( "$n's image repeatively pummels $N with its fists!!!", ch, NULL, vch, TO_NOTVICT );
        act( "Your image repeatively pummels $N with its fists!!!", ch, NULL, vch, TO_CHAR );
        act( "$n's image repeatively pummels you with its fists!!!", ch, NULL, vch, TO_VICT );
        act( "$n's image repeatively pummels $N with its fists!!!", ch, NULL, vch, TO_NOTVICT );
        act( "Your image repeatively pummels $N with its fists!!!", ch, NULL, vch, TO_CHAR );
        act( "$n's image repeatively pummels you with its fists!!!", ch, NULL, vch, TO_VICT );
        act( "$n's image repeatively pummels $N with its fists!!!", ch, NULL, vch, TO_NOTVICT );
        act( "$N screams as your image slashes across $S chest with its daggers, sending $M sprawling!", ch, NULL, vch, TO_CHAR );
        act( "You scream as $n's image slashes across your chest with its daggers, sending you sprawling!", ch, NULL, vch, TO_VICT );
        act( "$N screams as $n's image slashes across $S chest with its daggers, sending $M sprawling!", ch, NULL, vch, TO_NOTVICT );
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_var = SOUL;
        ch->attack_focus = DIRECT;

        if ( !IS_NPC( ch ) )
           hurt_person( ch, vch, 3500000, TRUE );
           else hurt_person( ch, vch, 1500000, TRUE );
    }

    act( "Your images suddenly leap back into your body.", ch, NULL, NULL, TO_CHAR );
    act( "$n's images suddenly leap back into $s body.", ch, NULL, NULL, TO_ROOM );
}

void do_hadouken( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 40000;

   if ( !IS_CLASS(ch, CLASS_NINJA) || ch->clannum != 4 )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

   if ( ch->ctimer[5] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Send the energy fireball to whom?\n\r", ch );
         return;
      }
    
   if ( arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
      }

   if ( ch->ninja_timer > 0 )
      {
         send_to_char( "You are recovering from the last one.\n\r", ch );
         return;
      }

   if ( victim == ch )
      {
         send_to_char( "You can't do this to yourself.\n\r", ch );
         return;
      }

   if ( is_safe(ch,victim) )
      return;
 
   ch->wait = 12;
   ch->ctimer[5] = 6;

   act( "You start to growl loudly as Dark energy surges out from you body blowing the dust away from your feet.", ch, NULL, NULL, TO_CHAR );
   act( "You start to shiver and slow step back as Dark energy surges out of $n blowing the dust away from $s feet.", ch, NULL, NULL, TO_ROOM );
   act( "Small dust and rock particles starts to rises and the boulders around you begins to shatter.", ch, NULL, NULL, TO_CHAR );
   act( "Small dust and rock particles starts to rises and the boulders around $n's body begins to shatter.", ch, NULL, NULL, TO_ROOM );
   act( "You drop into a Deep stance putting your hands at your side close together as energy particles starts to be suck in the vaccum of your hands.", ch, NULL, NULL, TO_CHAR );
   act( "$n drops into a Deep stance putting $s hands to $s side close together as energy particles starts to be suck in the vaccum of $s hands.", ch, NULL, NULL, TO_ROOM );
   act( "The energy particle slowly starts to form a energy ball and you concentrate more making the ball bigger.", ch, NULL, victim, TO_CHAR );
   act( "You notice more enery surges out from $n's body as well $s eyes turn #0dark #rred.", ch, NULL, NULL, TO_ROOM );
   act( "You shout out, '#RRRRRRRRRRRRRRRRRRRRAAAAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHH!!!#n'", ch, NULL, NULL, TO_CHAR );
   act( "You shout out, '#REAT THIS $N!!!!!!#n'", ch, NULL, victim, TO_CHAR );
   act( "$n shouts out, '#RRRRRRRRRRRRRRRRRRRRAAAAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHH!!!#n'", ch, NULL, NULL, TO_ROOM );
   act( "$n tells you, '#REAT THIS!!!!!!#n'", ch, NULL, victim, TO_VICT );
   act( "$n tells $N, '#REAT THIS!!!!!!#n'", ch, NULL, victim, TO_NOTVICT );
   act( "With a quick twist of your upper torso, you release the Dark Hadouken at $N.", ch, NULL, victim, TO_CHAR );
   act( "With a quick twist of $s's upper torso, $n releases the Dark Hadouken at you.", ch, NULL, victim, TO_VICT );
   act( "With a quick twist of $s's upper torso, $n releases the Dark Hadouken at $N.", ch, NULL, victim, TO_NOTVICT );
   act( "Unable to move, you take the full impact of the Fireball, sending you flying into the wall.", victim, NULL, NULL, TO_CHAR );
   act( "Unable to move, $n takes the full impact of the Fireball, sending $m flying into the wall.", victim, NULL, NULL, TO_ROOM );
   act( "You fall to your knee holding your chest while coughing out blood and gasping for air.", victim, NULL, NULL, TO_CHAR );
   act( "$n falls to $s knee holding $s chest while coughing out blood and gasping for air.", victim, NULL, NULL, TO_ROOM );

   hurt_person(ch,victim,dam,TRUE);
}

void do_choke( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  char      arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DROW) )
     {
        send_to_char( "Huh?\n\r", ch );
        return;
     }

  if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
     {
        stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
        return;
     }

  if ( ch->pkill < 12 )
     {
        stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
        return;
     }

  if ( ch->ctimer[4] > 0 )
     {
        stc("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( ch->holding != NULL )
     {
        ch->holding = NULL;
        ch->holding->holded = NULL;

	  send_to_char( "You release your hold on your target.\n\r", ch );
	  return;
     }

  argument = one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         send_to_char( "Who do you wish to hold?\n\r", ch );
	   return;
     }
    
  if ( arg[0] != '\0' )
     {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
     }

  if ( is_safe(ch,victim) )
     return;

  if ( victim == ch )
     {
	  send_to_char( "You can't do this to yourself.\n\r", ch );
	  return;
     }

  if ( victim->position < POS_STUNNED )
     {
	  send_to_char( "They are too weak for you to hold them all alone.\n\r", ch );
	  return;
     }

  if ( victim->holded != NULL )
     {
	  send_to_char( "Someone is already holding them.\n\r", ch );
	  return;
     }

  if ( is_martial( victim ) )
     {
	  act( "As you grasp $N, $E quickly slips out of your hold.", ch, NULL, victim, TO_CHAR );
	  return;
     }

  if ( ch->strike_timer == 0 )
     {
        ch->ctimer[4] = 5;
        ch->holding = victim;
        victim->holded = ch;
        ch->hunt_timer = 2;
     }

  act("You quickly go behind $N, tightly holding $M in a bear hug.",ch,NULL,victim,TO_CHAR);
  act("$n quickly goes behind you, tightly holding you in a bear hug.",ch,NULL,victim,TO_VICT);
  act("$n quickly goes behind $N, tightly holding $M in a bear hug.",ch,NULL,victim,TO_NOTVICT);
  act("As $N gasps for breath, you violently pull $M with you into the shadows.",ch,NULL,victim,TO_CHAR);
  act("As you gasp for breath, $n violently pulls you with $m into the shadows.",ch,NULL,victim,TO_VICT);
  act("As $N gasps for breath, $n violently pulls $M with $m into the shadows.",ch,NULL,victim,TO_NOTVICT);
}

void do_jigokute( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj = NULL;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) || ch->clannum != 2 )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Whom do you wish to strike?\n\r",ch);
	    return;
       }

    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( ch->ctimer[3] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->move < 10000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char(ch,WEAR_BACK) ) == NULL )
       || ( obj->pIndexData->vnum != 129 && obj->pIndexData->vnum != 91 ) )
       {
          stc("You need to have a sheathed katana on your back to perform this technique.\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) ) 
       return;

    ch->move -= 10000;
    ch->ctimer[3] = 4;

    act("You grin evilly as you lower your head silently.",ch,NULL,NULL,TO_CHAR);
    act("$n grins evilly as $e lowers $s head silently.",ch,NULL,NULL,TO_ROOM);
    act("Your body bursts with kiotic energy as you laugh loudly.",ch,NULL,NULL,TO_CHAR);
    act("$n's body bursts with kiotic energy as $e laughs loudly.",ch,NULL,NULL,TO_ROOM);
    act("You suddenly leap high into the air above $N's head.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly leaps high into the air above your head.",ch,NULL,victim,TO_VICT);
    act("$n suddenly leaps high into the air above $N's head.",ch,NULL,victim,TO_NOTVICT);
    act("You yell #y'Nokizura Den!!#n' as you rip out your sword SlAsHiNg down $N's body!!!",ch,NULL,victim,TO_CHAR);
    act("$n yells #y'Nokizura Den!!#n' as $e rips out $s sword SlAsHiNg down your body!!!",ch,NULL,victim,TO_VICT);
    act("$n yells #y'Nokizura Den!!#n' as $e rips out $s sword SlAsHiNg down $N's body!!!",ch,NULL,victim,TO_NOTVICT);
    act("As your sword RiPs $N's body aparts, it suddenly implodes with unbelievable energy.",ch,NULL,victim,TO_CHAR);
    act("As $n's sword RiPs your body apart, it suddenly implodes with unbelievable energy.",ch,NULL,victim,TO_VICT);
    act("As $n's sword RiPs $N's body aparts, it suddenly implodes with unbelievable energy.",ch,NULL,victim,TO_NOTVICT);

    if ( !IS_SET(victim->more, MORE_BLIND) )
       {
          SET_BIT(victim->more, MORE_BLIND);
          victim->blind_timer = 8;
       }

    hurt_person(ch,victim,30000,TRUE);
    ch->wait = 12;
}

void do_liakanitsu( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) || ch->clannum != 1 )
	 {
	    send_to_char("Huh?\n\r", ch);
	    return;
       }

    if ( !IS_SET(ch->more3, MORE3_MIRROR) )
       {
          send_to_char("You need to be hiding to get the full affect.\n\r", ch);
          return;
       }

    if ( ch->ctimer[1] > 0 ) 
       {
          send_to_char("You are still recovering from the last one.\n\r", ch );
          return;
       }	

    if ( ch->move < 10000 )
       {
          stc("You are too tired to do this!\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom to you want to surprise?\n\r", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }
 
    if ( ch == victim )
       {
          send_to_char("But you're already at yourself!\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    REMOVE_BIT(ch->more3, MORE3_MIRROR);

    ch->move -= 10000;
    ch->wait = 8;
    ch->ctimer[1] = 4;

    act("You slip a dagger in your hand as you suddenly throw the mirror into $N's face!",ch,NULL,victim,TO_CHAR);
    act("$n slips a dagger in $s hand as $e suddenly throws the mirror into your face!",ch,NULL,victim,TO_VICT);
    act("$n slips a dagger in $s hand as $e suddenly throws the mirror into $N's face!",ch,NULL,victim,TO_NOTVICT);
   
    if ( !IS_SET( victim->more, MORE_BLIND ) )
       {
          SET_BIT( victim->more, MORE_BLIND );
          victim->blind_timer = 5;
       }

    act("As $N staggers back from the blow, you rush to $N quickly thrusting your dagger into $S chest repeatively!",ch,NULL,victim,TO_CHAR);
    act("As you stagger back from the blow, $n rushes to you quickly thrusting $s dagger into $S chest repeatively!",ch,NULL,victim,TO_VICT);
    act("As $N staggers back from the blow, $n rushes to $N quickly thrusting $s dagger into $S chest repeatively!",ch,NULL,victim,TO_NOTVICT);

    hurt_person(ch,victim,40000,FALSE);

    if ( ch->fighting == NULL || victim == NULL || victim->position < POS_STUNNED ) 
       return;

    act("Suddenly, you push $N forward as you do a backflip onto your feet.",ch,NULL,victim,TO_CHAR);
    act("Suddenly, $n pushes you forward as $e does a backflip onto $s feet.",ch,NULL,victim,TO_VICT);
    act("Suddenly, $n pushes $N forward as $e does a backflip onto $s feet.",ch,NULL,victim,TO_NOTVICT);
    act("As $N staggers blindly, you rush towards $M for the attack.",ch,NULL,victim,TO_CHAR);
    act("As you stagger blindly, $n rushes towards you for the attack.",ch,NULL,victim,TO_VICT);
    act("As $N staggers blindly, $n rushes towards $M for the attack.",ch,NULL,victim,TO_NOTVICT);

    multi_hit( ch, victim, TYPE_UNDEFINED );
    multi_hit( ch, victim, TYPE_UNDEFINED );
}

void do_zinaki( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;
	
    if ( !IS_CLASS(ch, CLASS_NINJA) || ch->clannum != 1 )
       {
          send_to_char("Huh?\n\r", ch);
          return;
       }

    if ( ch->hit < 75000 ) 
       {
          send_to_char("You are too weak to hold the mirror properly.\n\r", ch );
          return;
       }	

    if ( IS_SET(ch->more3, MORE3_MIRROR) )
       {
          REMOVE_BIT(ch->more3, MORE3_MIRROR);
          send_to_char( "You appear from behind your mirror.\n\r", ch );
          act( "$n appears from behind $s mirror.", ch, NULL, NULL, TO_ROOM );
       }
       else {
               act( "$n pulls out a mirror in front of $m vanishing from sight.", ch, NULL, NULL, TO_ROOM );
               send_to_char( "You pull out a huge mirror from seemingly nowhere as you hide behind it.\n\r", ch );
               SET_BIT(ch->more3, MORE3_MIRROR);
            }
}

void do_screen(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_NINJA) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->ninja_powers[NPOWER_CHIKYU] < 70 ) 
      {
         send_to_char("Huh?\n\r", ch);
         return;
      }

   if ( ch->ctimer[2] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->move < 50000 )
      {
	   stc("You are too tired to release the smoke bomb.\n\r",ch);
	   return;
      }

   ch->ctimer[2] = 4;
   ch->move -= 50000;

   act("You flick your hands as you slam a small pill onto the ground.", ch, NULL, NULL, TO_CHAR);
   act("$n flicks $s hands as $e slams a small pill onto the ground.", ch, NULL, NULL, TO_ROOM);
   act("The room fills with deadly smoke as you flip back into the shadows.", ch, NULL, NULL, TO_CHAR);
   act("The room fills with deadly smoke as $e flips back into the shadows.", ch, NULL, NULL, TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      stop_fighting(vch,TRUE);

      if ( is_safe(ch,vch) )
         continue;

      if ( !IS_CLASS(vch, CLASS_NINJA) )
         vch->wait += 24;
   }
}

void do_sonic_strike( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int direction, range;
   bool pre_room;
   int count;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_NINJA) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->class_2 == 0 )
      if ( ( ( obj = get_eq_char( ch, WEAR_SCABBARD_R ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( ch, WEAR_SCABBARD_L ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
            if ( ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL ) || ( obj->value[3] != 11 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "You don't have a sheathed dagger.\n\r", ch );
             return;
          }

   if ( ch->move < 2000000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to strike?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   ch->move -= 2000000;
   ch->wait = 4; 

   act("You twist yourself around as you bring out your katana in a fluid motion.",ch,NULL,NULL,TO_CHAR);
   act("$n twists $mself around as $e brings out $s katana in a fluid motion.",ch,NULL,NULL,TO_ROOM);
   act("Before giving $N a chance to breathe, you slice $M ViCiOuSlY!",ch,NULL,victim,TO_CHAR);
   act("Before giving you a chance to breathe, $n slices you ViCiOuSlY!",ch,NULL,victim,TO_VICT);
   act("Before giving $N a chance to breathe, $n slices $M ViCiOuSlY!",ch,NULL,victim,TO_NOTVICT);
   act("You quickly sheath the weapon without pause as you glare around again.",ch,NULL,NULL,TO_CHAR);
   act("$n quickly sheathes the weapon without pause as $e glares around again.",ch,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 250000, TRUE );

   if ( victim->in_room == NULL || victim->position < POS_STUNNED || ch->hit > 2000000 || number_percent() < 70 )
      return;

   direction = number_range( 0, 3 );
   range = 1;

   do
   {
      pre_room = special_slam( victim, direction );
      count++;
   }
   while ( count < range && pre_room != TRUE );

   stop_fighting( victim, TRUE );
   victim->position = POS_STUNNED;
}
