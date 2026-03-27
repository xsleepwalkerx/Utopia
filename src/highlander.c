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

void do_bladespin( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    if ( ch->wpn[1] < 1000 )
    {
        stc("You understanding of the Katana is not great enough.\n\rYou need a higher slice weapon level first.\n\r",ch);
        return;
    }          

    if ( IS_SET(ch->newbits, NEW_BLADESPIN) )
    {
        stc("You relax your grip on your lightsabre.\n\r",ch);
	  act("$n's grip on their lightsabre relaxes.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->newbits, NEW_BLADESPIN);
	return;
    }

    if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
    {
        stc("Your grip on your lightsabre grows stronger.\n\r",ch);
	  act("$n's grip on thier lightsabre grows stronger.",ch,NULL,NULL,TO_ROOM);
	  SET_BIT(ch->newbits, NEW_BLADESPIN);
	  return;
    }
}

void do_jheal ( CHAR_DATA *ch, char *argument ) 
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 1 )
      {
         stc("Huh?\n\r",ch);
         return;
      } 

   if ( ch->force < 50 )
      {
         stc("You don't have enough power to use the Force.\n\r", ch );
         return;
      }

   if ( ch->mana < 50000 )
      {
         send_to_char("You don't have enough mana.\n\r", ch );
         return;
      }

   ch->mana = ch->mana - 50000;
   ch->hit += 50000;
   ch->force -= 50;

   if ( ch->hit > ch->max_hit ) 
      ch->hit = ch->max_hit;

   send_to_char("You concentrate on the Force healing your wounds.\n\r",ch);
   act("$n concentrates on the Force healing $s wounds.",ch,NULL,NULL,TO_ROOM);
   WAIT_STATE(ch, 12);
}

void do_levitate( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_HIGHLANDER) && !IS_CLASS( ch, CLASS_WIZARD ) )
      {
         stc("Huh?\n\r",ch);
         return;
      } 

   if ( IS_AFFECTED(ch, AFF_FLYING) ) 
      {
          send_to_char("You gently lower yourself down with the Force.\r\n", ch);
          act("$n gently lowers $mself down with the Force.", ch, NULL, NULL, TO_ROOM);
          REMOVE_BIT(ch->affected_by, AFF_FLYING);
          return;
      }

   send_to_char("You let the Force flow through you and you begin to levitate.\r\n", ch);
   act("$n uses the Force and begins to levitate.", ch, NULL, NULL, TO_ROOM);
   SET_BIT(ch->affected_by, AFF_FLYING);
}

void do_jpull( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
       {
          send_to_char( "Pull whose weapon?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( victim == ch ) 
       {
          stc("Not on yourself!\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON )
       && ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL || obj->item_type != ITEM_WEAPON )
       && ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL || obj->item_type != ITEM_WEAPON )
       && ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL || obj->item_type != ITEM_WEAPON ) )
       {
          send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
          return;
       }

    ch->wait = 18;

    if ( number_percent() < 40 || IS_CLASS(victim, CLASS_SORCEROR) )
       {
          act("You try to pull $N's weapon away from $M but $S mind was too powerful.",ch,NULL,victim,TO_CHAR);
          return;
       }
  
    act("You open your hand as you wave it at $N, causing $S weapon to leap into it, then fall to the ground.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly opens $s hand as $e mystically pulls your weapon and let it drop to the floor.",ch,NULL,victim,TO_VICT);
    act("$n opens $s hand as $e wave it at $N, causing $S weapon to leap into it, then fall to the ground.",ch,NULL,victim,TO_NOTVICT);

    obj_from_char( obj );
    obj_to_char( obj, victim );
}

void do_jsummon( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    if ( IS_SET(ch->more, MORE_FORCE) )
       {
          stc("You release your hold on the Force as it leaves your body.\n\r",ch);
          REMOVE_BIT(ch->more, MORE_FORCE); 
          return;
       }

    if ( ch->force < 10 )
       {
          stc("You are too tired to concentrate on the Force.\n\r",ch);
          return;
       } 

    stc("You concentrate on the Force as it flows throughout your body.\n\r",ch);
    act("$n concentrates on the Force as it flows throughout $s body.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->more, MORE_FORCE); 
}

void do_jtouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    one_argument( argument, arg );

    if ( ch->move < 50000 )
       {
          stc("You are too tired to use this skill.\n\r",ch);
          return;
       }  

    if ( arg[0] == '\0' )
    {
	send_to_char( "Who to you wish to stun?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->fight_timer > 0 )
       {
          send_to_char("but they have a fight timer.\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return; 

    if ( victim->position <= POS_STUNNED )
       {
          send_to_char("It will be stupid to do that.\n\r",ch);
          return;
       }

    ch->wait = 4;
    ch->move -= 50000;

    act("You point your finger at $N as you focus the force on $M.",ch,NULL,victim,TO_CHAR);
    act("$n points $s finger at you as you feel a presence in your head.",ch,NULL,victim,TO_VICT);
    act("$n points $s finger at $N as $e focuses the force on $M.",ch,NULL,victim,TO_NOTVICT);

    send_to_char( "You collapse to the ground stunned.\n\r", victim );
    act( "$n collapses to the ground stunned.", victim, NULL, NULL, TO_ROOM );
    victim->position = POS_STUNNED;
}

void do_jcatapult( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 1 )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    one_argument( argument, arg );

    if ( ch->move < 30000 )
       {
          stc("You are too tired to use this skill.\n\r",ch);
          return;
       }  

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
	   send_to_char( "Who to you wish to stun?\n\r", ch );
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

    if ( victim == ch )
       {
          send_to_char("Don't be stupid.\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return; 

    ch->wait = 18;
    ch->move -= 30000;

    act("You dash towards $N sidestepping $S attacks.",ch,NULL,victim,TO_CHAR);
    act("$n dashes towards you sidestepping your attacks.",ch,NULL,victim,TO_VICT);
    act("$n dashes towards $N sidestepping $S attacks.",ch,NULL,victim,TO_NOTVICT);
    act("You then leap off $N landing back on your feet.",ch,NULL,victim,TO_CHAR);
    act("$n then leaps off you landing back on $s feet.",ch,NULL,victim,TO_VICT);
    act("$n then leaps off $N landing back on $s feet.",ch,NULL,victim,TO_NOTVICT);
    act("Suddenly, you concentrate on the force sending $N into the air.",ch,NULL,victim,TO_CHAR);
    act("Suddenly, $n concentrates on the force sending you into the air.",ch,NULL,victim,TO_VICT);
    act("Suddenly, $n concentrates on the force sending $N into the air.",ch,NULL,victim,TO_NOTVICT);
    send_to_char( "You fall to the ground stunned.\n\r", victim );
    act( "$n falls to the ground stunned.", victim, NULL, NULL, TO_ROOM );

    hurt_person(ch,victim,50000,TRUE);
}

void do_meteor_strike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 1 )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    one_argument( argument, arg );

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
       && ( ( obj = get_eq_char( ch, WEAR_HOLD  ) ) == NULL )
       && ( ( obj = get_eq_char( ch, WEAR_THIRD ) ) == NULL )
       && ( ( obj = get_eq_char( ch, WEAR_FOURTH ) ) == NULL ) )
       {
          send_to_char( "You need to be using a weapon for this skill.\n\r", ch );
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          send_to_char( "You are still recovering from the last one.\n\r", ch );
          return;
       }

    if ( ch->mana < 100000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }  

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
  	   send_to_char( "Who to you wish to strike?\n\r", ch );
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

    ch->wait = 12;
    ch->mana -= 100000;
    ch->ctimer[0] = 4;

    act("You dash towards $N sidestepping $S attacks.",ch,NULL,victim,TO_CHAR);
    act("$n dashes towards you sidestepping your attacks.",ch,NULL,victim,TO_VICT);
    act("$n dashes towards $N sidestepping $S attacks.",ch,NULL,victim,TO_NOTVICT);
    act("You spin in a low arc swiping them in an upward motion across $N's chest.",ch,NULL,victim,TO_CHAR);
    act("$n spins in a low arc swiping you in an upward motion across your chest.",ch,NULL,victim,TO_VICT);
    act("$n spins in a low arc swiping $N in an upward motion across $S chest.",ch,NULL,victim,TO_NOTVICT);
    act("Suddenly you twirl your weapon across $N's arm searing $S flesh.",ch,NULL,victim,TO_CHAR);
    act("Suddenly $n twirls $s weapon across your arm searing your flesh.",ch,NULL,victim,TO_VICT);
    act("Suddenly $n twirls $s weapon across $N's arm searing $S flesh.",ch,NULL,victim,TO_NOTVICT);
    act("The force flows through your body as you mutter '#yMeteor Strike...#n'.",ch,NULL,NULL,TO_CHAR);
    act("Greenish energy emits of $n's body as $e mutters '#yMeteor Strike...#n'.",ch,NULL,NULL,TO_ROOM);
    act("You twirl your weapon across $N's chest searing $S flesh.",ch,NULL,victim,TO_CHAR);
    act("$n twirls $s weapon across your chest searing your flesh.",ch,NULL,victim,TO_VICT);
    act("$n twirls $s weapon across $N's chest searing $S flesh.",ch,NULL,victim,TO_NOTVICT);
    act("You twirl your weapon across $N's chest searing $S flesh.",ch,NULL,victim,TO_CHAR);
    act("$n twirls $s weapon across your chest searing your flesh.",ch,NULL,victim,TO_VICT);
    act("$n twirls $s weapon across $N's chest searing $S flesh.",ch,NULL,victim,TO_NOTVICT);
    act("You twirl your weapon across $N's chest searing $S flesh.",ch,NULL,victim,TO_CHAR);
    act("$n twirls $s weapon across your chest searing your flesh.",ch,NULL,victim,TO_VICT);
    act("$n twirls $s weapon across $N's chest searing $S flesh.",ch,NULL,victim,TO_NOTVICT);
    act("You leap into the air SmAsHiNg your weapon into $N face, making $M staggers backwards.",ch,NULL,victim,TO_CHAR);
    act("$n leaps into the air SmAsHiNg $s weapon into your face, making you stagger backwards.",ch,NULL,victim,TO_VICT);
    act("$n leaps into the air SmAsHiNg $s weapon into $N face, making $M staggers backwards.",ch,NULL,victim,TO_NOTVICT);

    hurt_person(ch,victim,150000,TRUE);
}

void do_jpush( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *pexit;
    EXIT_DATA       *pexit_rev;
    char            buf       [MAX_INPUT_LENGTH];
    char            direction [MAX_INPUT_LENGTH];
    int             door;
    int             rev_dir;
    int             dam = 75000;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
       {
          stc("Huh?\n\r",ch);
          return;
       } 

    if ( ch->jedi_timer > 0 )
       {
           send_to_char("Your mind is still focusing for the next one.\n\r",ch);
           return;
       }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
       {
          stc("Who do you wish to push?\n\r",ch);
          return;
       }

    if ( ch->mana < 50000 )
       {
          stc("You don't have enough magical energy to summon the Force.\n\r",ch);
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          stc("They are not here.\n\r",ch);
          return;
       }

    if ( victim == ch )
       return;

    if ( is_safe(ch,victim) )
       return;

    ch->mana -= 50000;
    ch->jedi_timer = 30;
   
    act("You push your hand towards $N.",ch,NULL,victim,TO_CHAR);
    act("$n pushes $s hand towards you.",ch,NULL,victim,TO_VICT);
    act("$n pushes $s hand towards $N.",ch,NULL,victim,TO_NOTVICT);

    if ( is_martial( victim ) )
       {
          act("An unseen force pushes you aside as you flip around landing on your feet.",victim,NULL,NULL,TO_CHAR);
          act("An unseen force pushes $n aside as $e flips around landing on $s feet.",victim,NULL,NULL,TO_ROOM);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) || IS_SET(ch->in_room->room_flags, ROOM_ENTOMB)
         || IS_SET(ch->in_room->room_flags, ROOM_BARRIER) || IS_SET(ch->in_room->room_flags, ROOM_CATACOMB) )
       {
          hurt_person(ch, victim, 75000, TRUE);
          return;
       }

    rev_dir = 0;

    door = number_range(0,3);

    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}

    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
	sprintf(buf,"$n is suddenly hurled into the %s wall.", direction);
	act(buf,victim,NULL,NULL,TO_ROOM);
	sprintf(buf,"You are suddenly hurled into the %s wall.", direction);
	act(buf,victim,NULL,NULL,TO_VICT);
	victim->hit = UMAX( victim->hit - dam, 0 );
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;

	if ( victim->position == POS_DEAD ) 
         raw_kill(victim);
         else if ( victim->position >= POS_FIGHTING )
                 victim->position = POS_STUNNED;
     
      return;
    }

    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) && 
	!IS_AFFECTED(victim,AFF_PASS_DOOR ) &&
	!IS_AFFECTED(victim,AFF_ETHEREAL  ) )
    {
	if (IS_SET(pexit->exit_info, EX_LOCKED))
	    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	if (IS_SET(pexit->exit_info, EX_CLOSED))
	    REMOVE_BIT(pexit->exit_info, EX_CLOSED);

	sprintf(buf,"There is a loud crash as $n is flung through the $d.");
	act(buf,victim,NULL,pexit->keyword,TO_ROOM);
	sprintf(buf,"An unseen force hurls you %s, smashing you through the %s.", direction,pexit->keyword);
	act(buf,ch,NULL,victim,TO_VICT);

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
          victim->hit = UMAX( victim->hit - dam, 0 );
	    update_pos(victim);
    	    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;

          if ( victim->position == POS_DEAD ) 
             raw_kill(victim);
             else if ( victim->position >= POS_FIGHTING )
                     victim->position = POS_STUNNED;
	}
    }
    else
    {
	sprintf(buf,"$n is suddenly hurled to the %s.", direction);
	act(buf,victim,NULL,NULL,TO_ROOM);
	sprintf(buf,"Toy are suddenly hurled to the %s.", direction);
	act(buf,victim,NULL,NULL,TO_CHAR);

	if (door == 0) sprintf(direction,"south");
	if (door == 1) sprintf(direction,"west");
	if (door == 2) sprintf(direction,"north");
	if (door == 3) sprintf(direction,"east");
	char_from_room(victim);
	char_to_room(victim,to_room);
	sprintf(buf,"$n comes flying in from the %s.", direction);
	act(buf,victim,NULL,NULL,TO_ROOM);
      victim->hit = UMAX( victim->hit - dam, 0 );
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;

      if ( victim->position == POS_DEAD ) 
         raw_kill(victim);
         else if ( victim->position >= POS_FIGHTING )
                 victim->position = POS_STUNNED;
    }
}

void do_jtaunt( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 2 )
     {
        stc("Huh?\n\r",ch);
        return;
     } 

  if ( ( arg[0] == '\0' ) && ( ch->fighting == NULL ) )
     {
        stc("Who do you wish to taunt?\n\r",ch);
        return;
     }

  if ( ch->mana < 20000 )
  {
    stc("You don't have enough magical energy to summon the Force.\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( ch->fighting != NULL ) 
     victim = ch->fighting;

  if ( is_safe(ch,victim) ) return;

  if ( IS_SET(victim->more,MORE_TAUNT) )
     {
        stc("Their anger is already boiled.\n\r",ch);
        return;
     }
 
  ch->wait = 12; 
  ch->mana -= 20000;

  if ( number_percent() < 60 )
     {
        stc("You fail to raise their anger.\n\r",ch);
        return;
     } 

  act("You sneer at $N as you call $M all types of names, making $S anger boils.",ch,NULL,victim,TO_CHAR);
  act("Your face turns red as $n call you all types of names.",ch,NULL,victim,TO_VICT);
  act("$N's face turns red.",ch,NULL,victim,TO_NOTVICT);

  SET_BIT(victim->more,MORE_TAUNT);
}

void do_jchoke( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 2 )
     {
        stc("Huh?\n\r",ch);
        return;
     } 

  if ( ch->fighting == NULL )
     {
        stc("You aren't in a fight.\n\r",ch);
        return;
     }

  if ( ch->mana < 20000 )
  {
    stc("You don't have enough magical energy to summon the Force.\n\r",ch);
    return;
  }

  victim = ch->fighting;

  ch->wait = 12; 
  ch->mana -= 20000;

  act("You calmly focus the Force on $N choking $M.",ch,NULL,victim,TO_CHAR);
  act("You suddenly cant breathe as you feel a presence choking you.",ch,NULL,victim,TO_VICT);
  act("$N suddenly start gasping for breath.",ch,NULL,victim,TO_NOTVICT);
}

void do_jlightning( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  char arg[MAX_INPUT_LENGTH];
  int dam = 50000;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
     {
        stc("Huh?\n\r",ch);
        return;
     } 

  if ( !IS_NPC(ch) )
     {
             if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
                {
  	              send_to_char( "Whom do you cant bolts of lightning on?\n\r", ch );
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

             if ( ch->mana < 30000 )
                {
                    stc("You don't have enough magical energy to summon the Force.\n\r",ch);
                    return;
                }
          }

  if ( is_safe(ch,victim) ) return;

  act("You point your finger at $N as lightning bolts flashes from it and strikes $M.",ch,NULL,victim,TO_CHAR); 
  act("$n points $s finger at you as lightning bolts flashes from it and strikes you.",ch,NULL,victim,TO_VICT); 
  act("$n points $s finger at $N as lightning bolts flashes from it and strikes $M.",ch,NULL,victim,TO_NOTVICT); 

  if ( number_percent() > 92 && !is_martial( victim ) )
     {
        stc("You are stunned from the cackling electricity.\n\r",victim);
        victim->wait = 12;
     }

  if ( !IS_NPC( ch ) )
     ch->mana -= 30000;

  ch->wait = 18;
  hurt_person(ch,victim,dam,TRUE);
}

void do_jdestruction( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam = 150000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 2 )
     {
        stc("Huh?\n\r",ch);
        return;
     } 

  argument = one_argument( argument, arg );

  if ( ch->ctimer[0] > 0 )
     {
        send_to_char( "You are still recovering from the last one.\n\r", ch );
        return;
     }

  if ( ( victim = ch->fighting ) == NULL && arg[0] == '\0' )
     {
        stc("Whose body do you wish to disrupt?\n\r",ch);
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

  if ( ch->mana < 30000 )
     {
        stc("You don't have enough magical energy to summon the Force.\n\r",ch);
        return;
     }

  if ( is_safe(ch,victim) ) return;

  act("You grin evilly as you point your finger at $N.",ch,NULL,victim,TO_CHAR); 
  act("$n grins evilly as $e points $s finger at you.",ch,NULL,victim,TO_VICT); 
  act("$n grins evilly as $e points $s finger at $N.",ch,NULL,victim,TO_NOTVICT); 
  act("$N screams loudly as the Force explodes within $S body.",ch,NULL,victim,TO_CHAR); 
  act("You scream loudly as a presence explodes within your body.",ch,NULL,victim,TO_VICT); 
  act("$N screams loudly as a presence explodes within $S body.",ch,NULL,victim,TO_NOTVICT); 

  ch->ctimer[0] = 4;
  ch->mana -= 30000;
  ch->wait = 12;
  victim->mana = UMAX( 0, victim->mana - 150000 );
  victim->move = UMAX( 0, victim->move - 150000 );

  hurt_person(ch,victim,dam,TRUE);
}

void do_jinvis( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;
	
   if ( !IS_CLASS(ch, CLASS_HIGHLANDER) || ch->clannum != 1 )
      {
         send_to_char("Huh?\n\r", ch);
         return;
	}
	
   if ( ch->fight_timer > 0 ) 
      {
         send_to_char("Not until your fight timer expires.\n\r", ch );
         return;
      }	

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      {
         REMOVE_BIT(ch->act, PLR_WIZINVIS);
         send_to_char( "You remove your masking allowing people to see you again.\n\r", ch );
         act( "$n suddenly appears from nowhere.", ch, NULL, NULL, TO_ROOM );
      }
      else {
              act( "$n suddenly vanishes from sight as $e waves $s hands.", ch, NULL, NULL, TO_ROOM );
              send_to_char( "You calmly concentrate on the force fading from view.\n\r", ch );
              SET_BIT(ch->act, PLR_WIZINVIS);
           }
}

void do_jrush( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
     {
	  send_to_char("Huh?\n\r", ch);
	  return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    if (ch->fighting == NULL) 
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
    else victim = ch->fighting;
  }

  if (is_safe( ch, victim )) return;

  if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You need to wield a slicing weapon.\n\r", ch );
           return;
        }

  act( "You dash towards $N with a flashy glint from your eyes!", ch, NULL, victim, TO_CHAR );
  act( "$n dashes towards you with a flashy glint from $s eyes!", ch, NULL, victim, TO_VICT );
  act( "$n dashes towards $N with a flashy glint from $s eyes!", ch, NULL, victim, TO_NOTVICT );
  act( "You concentrate purely on the force as you scream '#yBANZAI!#n", ch, NULL, NULL, TO_CHAR );
  act( "$n concentrates purely on the force as $e screams '#yBANZAI!#n", ch, NULL, NULL, TO_ROOM );

  ch->wait = 12;
  multi_hit( ch, victim, TYPE_UNDEFINED );
  show_total_damage( ch, victim );
}
