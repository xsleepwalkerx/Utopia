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


/**************************
 * Undead knights by Jobo * 
 **************************/

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

void do_knightarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
  {
    send_to_char("You are not one of the undead!!!\n\r",ch);
    return;
  }

  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of unholy armor you wish to make: plate ring bracer collar helmet leggings boots gauntlets chains cloak belt mask axe.\n\r",ch);
    return;
  }

  if ( ch->gold < 1 )
  {
    send_to_char("It costs #w1 #ygold#n piece to create a piece of unholy armor.\n\r",ch);
    return;
  }

  if (!str_cmp(arg,"plate")) vnum = 369;
  else if (!str_cmp(arg,"axe")) vnum = 370;
  else if (!str_cmp(arg,"ring")) vnum = 372;
  else if (!str_cmp(arg,"bracer")) vnum = 373;
  else if (!str_cmp(arg,"collar")) vnum = 374;
  else if (!str_cmp(arg,"helmet")) vnum = 375;
  else if (!str_cmp(arg,"leggings")) vnum = 376;
  else if (!str_cmp(arg,"boots")) vnum = 377;
  else if (!str_cmp(arg,"gauntlets")) vnum = 378;
  else if (!str_cmp(arg,"chains")) vnum = 379;
  else if (!str_cmp(arg,"cloak")) vnum = 380;
  else if (!str_cmp(arg,"belt")) vnum = 381;
  else if (!str_cmp(arg,"mask")) vnum = 382;
  else
  {
    send_to_char("Please specify which piece of unholy armor you wish to make: plate ring bracer collar helmet leggings boots gauntlets chains cloak belt mask axe.\n\r",ch);
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform Jobo.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  ch->gold -=15;
}

void do_unholyrite (CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) ) return;

   if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 5 )
     {
        send_to_char("#yYour #efaith in the #PGod #eof #RDeath #eisn't high enough to do this technique.#n\n\r",ch);
        return;
     }

  if ( ch->mana < 400000 )
     {
        send_to_char("You don't have the mystical energies to make the rite.\n\r",ch);
        return;
     }

  hit_gain( ch, 300000 );
  send_to_char("#RYou make a blood sacrifice to the #wgod #Rof #eDeath#R.#n\n\r",ch);

  ch->mana -= 400000;
  WAIT_STATE( ch, 12 );
}

void do_armads( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   bool pre_room;
   int direction;
   int count;
   DESCRIPTOR_DATA *d;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 25 || ch->tier < 5 )
      {
         send_to_char("#yYour #efaith in the #PGod #eof #RDeath #eisn't high enough to do this technique.#n\n\r",ch);
         return;
      }

   if ( ch->alignment < 750 )
      {
         stc("#wOnly the hands of #ygood #wcan use this holy weapon.#n\n\r",ch);
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;

   act( "You tighten the grip around your cloak as the room is suddenly fill with light.", ch, NULL, NULL, TO_CHAR );
   act( "$n tightens the grip around $s cloak as the room is suddenly fill with light.", ch, NULL, NULL, TO_ROOM );
   act( "Finding yourself admist the heavens, you hear soft gentle music in the background.", ch, NULL, NULL, TO_CHAR );
   act( "Finding yourself admist the heavens, you hear soft gentle music in the background.", ch, NULL, NULL, TO_ROOM );
   act( "A beam of light suddenly shines down upon you, revealing a MASSIVE axe.", ch, NULL, NULL, TO_CHAR );
   act( "A beam of light suddenly shines down upon $n, revealing a MASSIVE axe.", ch, NULL, NULL, TO_ROOM );
   act( "Strapping it behind your sholders, you look coldly around you...", ch, NULL, NULL, TO_CHAR );
   act( "Strapping it behind $s sholders, $n looks coldly around you...", ch, NULL, NULL, TO_ROOM );
   act( "You then push a hand forward as it erupt into ferocious flames!", ch, NULL, NULL, TO_CHAR );
   act( "$n then pushes a hand forward as it erupt into ferocious flames!", ch, NULL, NULL, TO_ROOM );
   act( "You then SMASH the axe on the ground releasing a TREMEDOUS shockwave!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n then SMASHES the axe on the ground releasing a TREMEDOUS shockwave!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
 
      stop_fighting( vch, TRUE );
   }

   act( "Sparks of electricity fizzes over the axe as you crouch forward subtively.", ch, NULL, NULL, TO_CHAR );
   act( "Sparks of electricity fizzes over the axe as $n crouches forward subtively.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly SWIPE across the room at the SPEED OF LIGHT!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly SWIPES across the room at the SPEED OF LIGHT!!!", ch, NULL, NULL, TO_ROOM );

   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) )
         continue;

      act( "#wYour #obody #Rconvulses #p#zViOlEnTlY#n #was the #Cdazzling #yenergy #wBlAsTs you AWAY!!!#n", vch, NULL, NULL, TO_CHAR );
      act( "#w$n#w's #obody #Rconvulses #p#zViOlEnTlY#n #was the #Cdazzling #yenergy #wBlAsTs $m AWAY!!!#n", vch, NULL, NULL, TO_ROOM );

      count = 0;
      direction = number_range( 0, 3 );

      do
      {
         pre_room = special_slam( vch, direction );
         count++;
      }
      while ( count < 10 && pre_room != TRUE );

      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = SLASH;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 3000000, TRUE );

      if ( vch == NULL || vch->in_room == NULL || IS_CLASS( vch, CLASS_WISP ) )
         continue;

      vch->stunned = 3;
   }

   act( "Gripping back your axe, you sneer as you leap high into the air!", ch, NULL, NULL, TO_CHAR );
   act( "Gripping back $s axe, $n sneer as you leaps high into the air!", ch, NULL, NULL, TO_ROOM );
   act( "You twirl your axe rapidly as the room spins and turns with it...", ch, NULL, NULL, TO_CHAR );
   act( "$n twirls $s axe rapidly as the room spins and turns with it...", ch, NULL, NULL, TO_ROOM );
   act( "You SCREAM loudly as you SMASH the axe MIGHTILY into the ground!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n SCREAMS loudly as $e SMASHES the axe MIGHTILY into the ground!!!", ch, NULL, NULL, TO_ROOM );
   act( "#y#zThe room goes dark as a TREMEDOUS EXPLOSION fills the air!!!#n", ch, NULL, NULL, TO_CHAR );
   act( "#y#zThe room goes dark as a TREMEDOUS EXPLOSION fills the air!!!#n", ch, NULL, NULL, TO_ROOM );

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
         {
            if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
               continue;

            if ( ch->in_room->area == d->character->in_room->area )
               continue;

            act( "#yThe ground beneath your feet suddenly explodes with FEROCIOUS energy!#n", d->character, NULL, NULL, TO_CHAR );
            act( "#yThe ground beneath $n#y's feet suddenly explodes with FEROCIOUS energy!#n", d->character, NULL, NULL, TO_CHAR );

            ch->ele_attack = ATTACK_THUNDER;
            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_var = BLAST;
            ch->attack_focus = UPPERCUT;
            hurt_person( ch, d->character, 3000000, TRUE );

            if ( d->character == NULL || d->character->in_room == NULL )
               continue;

            d->character->air_timer = 4;
            stop_fighting( d->character, TRUE );
         }
   }
}

void do_genocide( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
     {
         stc("Huh?\n\r",ch);
         return;
     }

  if ( ch->pkill < 10 )
     {
        send_to_char("#yYour #efaith in the #PGod #eof #RDeath #eisn't high enough to do this technique.#n\n\r",ch);
        return;
     }

  if ( ch->ctimer[1] > 0 )
     {
	   send_to_char( "You are still recovering from the last one.\n\r", ch );
         return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding an axe.\n\r", ch );
           return;
        }

  if ( ch->move < 2000000 )
     {
       stc("You are too tired to do this!\n\r",ch);
       return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use your rage on?\n\r",ch);
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

  act("You firmly grab your axe with both hands as you savagely take a step forward.",ch,NULL,NULL,TO_CHAR);
  act("$n firmly grabs $s axe with both hands as $e savagely takes a step forward.",ch,NULL,NULL,TO_ROOM);
  act("The ground beneath your feet ShAtTeRs as your foot SmAsH into it.",ch,NULL,NULL,TO_CHAR);
  act("The ground beneath $n's feet ShAtTeRs as $s foot SmAsHeS into it.",ch,NULL,NULL,TO_ROOM);
  act("Reddish energy soars from your body as your eyes blazes with demonic intensity.",ch,NULL,NULL,TO_CHAR);
  act("Reddish energy soars from $n's body as $s eyes blazes with demonic intensity.",ch,NULL,NULL,TO_ROOM);
  act("You suddenly DrIvE your axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly DrIvEs $s axe across your chest, spraying blood all over the place.",ch,NULL,victim,TO_VICT);
  act("$n suddenly DrIvEs $s axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_NOTVICT);
  act("You then twirl your axe across $N's waist, staggering $M backwards.",ch,NULL,victim,TO_CHAR);
  act("$n then twirls $s axe across your waist, staggering you backwards.",ch,NULL,victim,TO_VICT);
  act("$n then twirls $s axe across $N's waist, staggering $M backwards.",ch,NULL,victim,TO_NOTVICT);
  act("Finally, you SmAsH your axe downwards $N's body causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_CHAR);
  act("Finally, $n SmAsHeS $s axe downwards your body causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_VICT);
  act("Finally, $n SmAsHeS $s axe downwards $N's body causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 12;
  ch->ctimer[1] = 4;
  ch->move -= 2000000;

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SLASH;
  ch->attack_focus = OVERHEAD;
  hurt_person( ch, victim, 1500000, TRUE );
}

void do_tetra_slice( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) || ch->generation != 1 )
     {
         stc("Huh?\n\r",ch);
         return;
     }

  if ( IS_SET(ch->more, MORE_BOMB) )
     {
	   send_to_char( "You are still recovering from the last one.\n\r", ch );
         return;
     }

  if ( ch->move < 300000 )
     {
       stc("You are too tired to do this!\n\r",ch);
       return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use your rage on?\n\r",ch);
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

  act("You firmly grab your axe with both hands as you savagely take a step forward.",ch,NULL,NULL,TO_CHAR);
  act("$n firmly grabs $s axe with both hands as $e savagely takes a step forward.",ch,NULL,NULL,TO_ROOM);
  act("The ground beneath your feet ShAtTeRs as your foot SmAsH into it.",ch,NULL,NULL,TO_CHAR);
  act("The ground beneath $n's feet ShAtTeRs as $s foot SmAsHeS into it.",ch,NULL,NULL,TO_ROOM);
  act("Reddish energy soars from your body as your eyes blazes with demonic intensity.",ch,NULL,NULL,TO_CHAR);
  act("Reddish energy soars from $n's body as $s eyes blazes with demonic intensity.",ch,NULL,NULL,TO_ROOM);
  act("You suddenly DrIvE your axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly DrIvEs $s axe across your chest, spraying blood all over the place.",ch,NULL,victim,TO_VICT);
  act("$n suddenly DrIvEs $s axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_NOTVICT);
  act("You suddenly DrIvE your axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly DrIvEs $s axe across your chest, spraying blood all over the place.",ch,NULL,victim,TO_VICT);
  act("$n suddenly DrIvEs $s axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_NOTVICT);
  act("You suddenly DrIvE your axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly DrIvEs $s axe across your chest, spraying blood all over the place.",ch,NULL,victim,TO_VICT);
  act("$n suddenly DrIvEs $s axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_NOTVICT);
  act("You suddenly DrIvE your axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly DrIvEs $s axe across your chest, spraying blood all over the place.",ch,NULL,victim,TO_VICT);
  act("$n suddenly DrIvEs $s axe across $N's chest, spraying blood all over the place.",ch,NULL,victim,TO_NOTVICT);
  act("You then twirl your axe across $N's waist, staggering $M backwards.",ch,NULL,victim,TO_CHAR);
  act("$n then twirls $s axe across your waist, staggering you backwards.",ch,NULL,victim,TO_VICT);
  act("$n then twirls $s axe across $N's waist, staggering $M backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You then twirl your axe across $N's waist, staggering $M backwards.",ch,NULL,victim,TO_CHAR);
  act("$n then twirls $s axe across your waist, staggering you backwards.",ch,NULL,victim,TO_VICT);
  act("$n then twirls $s axe across $N's waist, staggering $M backwards.",ch,NULL,victim,TO_NOTVICT);
  act("Finally, you SmAsH your axe downwards $N's body causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_CHAR);
  act("Finally, $n SmAsHeS $s axe downwards your body causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_VICT);
  act("Finally, $n SmAsHeS $s axe downwards $N's body causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 12;
  ch->move -= 300000;
  SET_BIT(ch->more, MORE_BOMB);
  hurt_person(ch,victim,1500000,TRUE);
}

void do_gain (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;

  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }

  if (arg[0] == '\0')
  {
    sprintf(buf, "Current powers : Invocation [#r%d#L]         Spirit [#r%d#L]#n\n\r",
      ch->pcdata->powers[INVOCATION], ch->pcdata->powers[UNDEAD_SPIRIT]);
    send_to_char(buf,ch);
    send_to_char("what path of power do you wish to gain power from? [invocation,spirit]\n\r",ch);
    return;
  }

  if (!str_cmp(arg,"invocation"))  /* attacks like powerwords */
  {
    if (ch->pcdata->powers[INVOCATION]>4)
    {
      send_to_char("You have mastered the art of invocation.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("Your mastery of the ancient arts increase.\n\r",ch);
      ch->pcdata->powers[INVOCATION] += 1;
      return;
    }
  }
  else if (!str_cmp(arg,"spirit"))  /* toughness */
  {
    if (ch->pcdata->powers[UNDEAD_SPIRIT] > 9)
    {
      send_to_char("You have completely bound your spirit to this vessel.\n\r",ch);
      return;
    }
    else  
    {
      send_to_char("You channel more of your spirit from the abyss into this body.\n\r",ch);
      ch->pcdata->powers[UNDEAD_SPIRIT] += 1;
      return;
    }
  }
  else stc("Syntax: Gain Invocation/Spirit\n\r",ch);
}

void do_weaponpractice (CHAR_DATA *ch, char *argument)
{
  int count;

  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
     {
        send_to_char("You are not one of the undead!!!\n\r",ch);
        return;
     }

  if ( ch->pcdata->powers[WEAPONSKILL] > 9 )
     {
        send_to_char("You have already mastered the art of combat.\n\r",ch);
        return;
     }

  if ( ch->pkill / 2 <= ch->pcdata->powers[WEAPONSKILL] )
     {
        send_to_char("#eYou need to be #ymore powerful #ebefore training your weapons further.#n\n\r",ch);
        return;
     }

  send_to_char("#eYou feel your skills with #yweapons #eincrease as you make your bloodsacrifice.#n\n\r",ch);
  ch->pcdata->powers[WEAPONSKILL] += 1;
}

void do_powerword (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  CHAR_DATA *ich;
  CHAR_DATA *victim;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
     {
         stc("Huh?\n\r",ch);
         return;
     }

  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2 );

  if ( arg[0] == '\0' )
     {
        send_to_char("#eValid #Rpowerwords #eare #wkill, stun, blind, fear, lightning, venom, flames, chill, blast, terror, leech and oblivion.#n\n\r",ch);
        return;
     }

  if ( !str_cmp(arg,"stun") )
     {
        if ( !( arg2[0] == '\0' ) || ch->fighting != NULL )
           {
              if ( arg2[0] == '\0' )
                 victim = ch->fighting;
                 else if ( ( victim = get_char_room(ch, arg2) ) == NULL )
                         {
                            send_to_char("They are not here.\n\r", ch );
                            return;
                         }

              if ( ch->pcdata->powers[POWER_FREEZE] > 0 )
                 {
                    send_to_char("You cannot use this powerword yet.\n\r",ch);
                    return;
                 }

              if ( is_safe(ch, victim) )
                 return;

              act("$n points his finger at $N and says '#CFREEZE!!!#n'",ch,NULL,victim,TO_NOTVICT);
              act("You point your finger at $N and say '#CFREEZE!!!#n'",ch,NULL,victim,TO_CHAR);
              act("$n points his finger at you and says '#CFREEZE!!!#n'",ch,NULL,victim,TO_VICT);

              if ( !is_martial( victim ) )
                 victim->wait = 18;

              ch->pcdata->powers[POWER_FREEZE] = 25;
              return;
           }
           else {
                   send_to_char("Stun whom?\n\r",ch);
                   return;
                }
     }
     else if ( !str_cmp(arg,"lightning") )
             {
                int dam;

                if ( !( arg2[0] == '\0' ) || ch->fighting != NULL )
                   {
                      if ( ch->lightning_timer > 0 )
                         {
                            send_to_char("You cannot use this powerword yet.\n\r",ch);
                            return;
                         }

                      if ( arg2[0] == '\0' )
                         victim = ch->fighting;
                         else if ( ( victim = get_char_room(ch, arg2) ) == NULL )
                                 {
                                    send_to_char("They are not here.\n\r", ch );
                                    return;
                                 }

                      if ( is_safe(ch, victim) )
                         return;

                      act("$n looks at $N and shouts '#yLIGHTNING!!!#n'",ch,NULL,victim,TO_NOTVICT);
                      act("You look at $N and shouts '#yLIGHTNING!!!#n'",ch,NULL,victim,TO_CHAR);
                      act("$n looks at you and shouts '#yLIGHTNING!!!#n'",ch,NULL,victim,TO_VICT);

                      ch->ele_attack = ATTACK_THUNDER;
                      ch->attack_type = ATTACK_MAGICAL;
                      ch->attack_var = BLUNT;
                      ch->attack_focus = DIRECT;

                      dam = 750000 + UMIN( 150000, ch->pkill * 7500 );

                      dam = is_elemental( ATTACK_THUNDER, victim, dam );
                      dam = is_attack_type( ch, victim, dam );

                      sprintf(buf1,"$n's lightning bolt strikes $N [#C%d#n]", dam);
                      sprintf(buf2,"Your lightning bolt strikes $N [#C%d#n]", dam);
                      sprintf(buf3,"$n's lightning bolt strikes you [#C%d#n]", dam);
 
                      dam = 750000 + UMIN( 150000, ch->pkill * 7500 );

                      act(buf1,ch,NULL,victim, TO_NOTVICT);
                      act(buf2,ch,NULL,victim, TO_CHAR);
                      act(buf3,ch,NULL,victim, TO_VICT);

                      ch->lightning_timer = 3;
                      ch->wait = 6;
                      hurt_person( ch, victim, dam, TRUE );
                   }
                   else {
                           send_to_char("Struck whom with lightning?\n\r",ch);
                           return;
                        }
             }
     else if ( !str_cmp(arg,"kill") )
             {
                if ( !( arg2[0] == '\0' ) || ch->fighting != NULL )
                   {
                      int dam;

                      if ( arg2[0] == '\0' )
                         victim = ch->fighting;
                         else if ( ( victim = get_char_room(ch, arg2) ) == NULL )
                                 {
                                    send_to_char("They are not here.\n\r", ch );
                                    return;
                                 }

                      if ( ch->pcdata->powers[POWER_WORD] > 0 )
                         {
                            send_to_char("You cannot use this powerword yet.\n\r",ch);
                            return;
                         }

                      if ( is_safe(ch, victim) )
                         return;

                      act("$n looks at $N and utters '#eDIE!!!#n'",ch,NULL,victim,TO_NOTVICT);
                      act("You look at $N and say '#eDIE!!!#n'",ch,NULL,victim,TO_CHAR);
                      act("$n looks at you and utters '#eDIE!!!#n'",ch,NULL,victim,TO_VICT);

                      ch->attack_type = ATTACK_MAGICAL;
                      ch->ele_attack = ATTACK_DARK;
                      ch->attack_var = SOUL;
                      ch->attack_focus = DIRECT;

                      dam = 1000000;
                      dam = is_attack_type( ch, victim, dam );

                      sprintf(buf1,"$n's powerword strikes $N [#C%d#n]", dam);
                      sprintf(buf2,"Your powerword strikes $N [#C%d#n]", dam);
                      sprintf(buf3,"$n's powerword strikes you [#C%d#n]", dam);

                      dam = 1000000;

                      act(buf1,ch,NULL,victim, TO_NOTVICT);
                      act(buf2,ch,NULL,victim, TO_CHAR);
                      act(buf3,ch,NULL,victim, TO_VICT);
                      ch->pcdata->powers[POWER_WORD] = 4;
                      ch->wait = 18;

                      hurt_person(ch,victim,dam,TRUE);
                      return;
                   }
                   else {
                           send_to_char("Kill whom?\n\r",ch);
                           return;
                        }
             }
     else if ( !str_cmp(arg,"flames") )
             {
                CHAR_DATA *ich_next;
                int count;

                if ( ch->pcdata->powers[POWER_FLAME] > 0 )
                   {
                      send_to_char("You cannot use this powerword yet.\n\r",ch);
                      return;
                   }

                ch->wait = 12;
                ch->pcdata->powers[POWER_FLAME] = 15;

                for ( ich = ch->in_room->people; ich != NULL; ich = ich_next )
                {
                   ich_next = ich->next_in_room;

                   if ( is_safe(ch,ich) )
                      continue;

                   act("$n looks at $N and utters '#RFLAMES!!!#n'",ch,NULL,ich,TO_NOTVICT);
                   act("You look at $N and say '#RFLAMES!!!#n'",ch,NULL,ich,TO_CHAR);
                   act("$n looks at you and utters '#RFLAMES!!!#n'",ch,NULL,ich,TO_VICT);

                   for ( count = 0; count < 15 + UMIN( 10, ch->pkill / 10 ); count++ )
                   {
                      ch->ele_attack = ATTACK_FIRE;
                      ch->attack_type = ATTACK_MAGICAL;
                      ch->attack_var = BLAST;
                      ch->attack_focus = DIRECT;
                      one_hit(ch,ich,gsn_fireball,1);
                   }

                   show_total_damage( ch, ich );
                }

                return;
             }
     else if ( !str_cmp(arg,"chill") )
             {
                CHAR_DATA *ich_next;
                int count;

                if ( ch->pkill < 15 )
                   {
                      send_to_char("#yYour #efaith in the #PGod #eof #RDeath #eisn't high enough to do this technique.#n\n\r",ch);
                      return;
                   }

                if ( ch->pcdata->powers[POWER_CHILL] > 0 )
                   {
                      send_to_char("You cannot use this powerword yet.\n\r",ch);
                      return;
                   }

                ch->wait = 12;
                ch->pcdata->powers[POWER_CHILL] = 15;

                for ( ich = ch->in_room->people; ich != NULL; ich = ich_next )
                {
                   ich_next = ich->next_in_room;

                   if ( is_safe(ch,ich) )
                      continue;

                   act("$n looks at $N and utters '#CCHILL!!!#n'",ch,NULL,ich,TO_NOTVICT);
                   act("You look at $N and say '#CCHILL!!!#n'",ch,NULL,ich,TO_CHAR);
                   act("$n looks at you and utters '#CCHILL!!!#n'",ch,NULL,ich,TO_VICT);

                   for ( count = 0; count < 15; count++ )
                   {
                      if ( !IS_SET( ich->more, MORE_FROZEN ) && !is_martial( ich ) )
                         {
                            ich->freeze_timer = 1;
                            SET_BIT( ich->more, MORE_FROZEN );
                         }

                      ch->ele_attack = ATTACK_ICE;
                      ch->attack_type = ATTACK_MAGICAL;
                      ch->attack_var = PIERCE;
                      ch->attack_focus = DIRECT;
                      one_hit(ch,ich,gsn_fireball,1);
                   }

                   show_total_damage( ch, ich );
                }

                return;
             }
     else if ( !str_cmp(arg,"blast") )
             {
                CHAR_DATA *ich_next;

                if ( ch->pkill < 20 )
                   {
                      send_to_char("#yYour #efaith in the #PGod #eof #RDeath #eisn't high enough to do this technique.#n\n\r",ch);
                      return;
                   }

                if ( ch->pcdata->powers[POWER_BLAST] > 0 )
                   {
                      send_to_char("You cannot use this powerword yet.\n\r",ch);
                      return;
                   }

                ch->wait = 6;
                ch->pcdata->powers[POWER_BLAST] = 10;

                for ( ich = ch->in_room->people; ich != NULL; ich = ich_next )
                {
                   ich_next = ich->next_in_room;

                   if ( is_safe(ch,ich) )
                      continue;

                   act("$n looks at $N and utters '#yPOWER BLAST!!!#n'",ch,NULL,ich,TO_NOTVICT);
                   act("You look at $N and say '#yPOWER BLAST!!!#n'",ch,NULL,ich,TO_CHAR);
                   act("$n looks at you and utters '#yPOWER BLAST!!!#n'",ch,NULL,ich,TO_VICT);
                   act("A ferocious bolt of energy rips your body from below!",ich,NULL,NULL,TO_CHAR);
                   act("A ferocious bolt of energy rips $n's body from below!",ich,NULL,NULL,TO_ROOM);

                   ch->ele_attack = ATTACK_THUNDER;
                   ch->attack_type = ATTACK_MAGICAL;
                   ch->attack_var = PIERCE;
                   ch->attack_focus = UPPERCUT;
                   hurt_person( ch, ich, 750000, TRUE );
                }

                return;
             }
     else if ( !str_cmp(arg,"fear") )
             {
                if ( !( arg2[0] == '\0' ) || ch->fighting != NULL )
                   {
                      int count = 0;                      

                      if ( arg2[0] == '\0' )
                         victim = ch->fighting;
                         else if ( ( victim = get_char_room(ch, arg2) ) == NULL )
                                 {
                                    send_to_char("They are not here.\n\r", ch );
                                    return;
                                 }

                      if ( ch->fear_timer > 0 )
                         {
                            send_to_char("You cannot use this powerword yet.\n\r",ch);
                            return;
                         }

                      if ( is_safe(ch, victim) )
                         return;

                      act("$n touches $N's forehead and screams '#wFEAR!!!#n'",ch,NULL,victim,TO_NOTVICT);
                      act("You touch $N's forehead and screams '#wFEAR!!!#n'",ch,NULL,victim,TO_CHAR);
                      act("$n touches your forhead and screams '#wFEAR!!!#n'",ch,NULL,victim,TO_VICT);
                      act("You scramble to flee in total fear!",victim,NULL,NULL,TO_CHAR);
                      act("$n scrambles to flee in total fear!",victim,NULL,NULL,TO_ROOM);

                      do
                      {
                         do_flee( victim, "" );
                         count++;
                      }
                      while ( ch->position == POS_FIGHTING && count < 4 );

                      if ( !is_martial( victim ) )
                         victim->wait = 18;

                      ch->fear_timer = 20;
                      return;
                   }
                   else {
                           send_to_char("Scare whom?\n\r",ch);
                           return;
                        }
             }
     else if ( !str_cmp(arg,"venom") )
             {
                if ( !( arg2[0] == '\0' ) || ch->fighting != NULL )
                   {
                      if ( arg2[0] == '\0' )
                         victim = ch->fighting;
                         else if ( ( victim = get_char_room(ch, arg2) ) == NULL )
                                 {
                                    send_to_char("They are not here.\n\r", ch );
                                    return;
                                 }

                      if ( ch->poison_timer > 0 )
                         {
                            send_to_char("You cannot use this powerword yet.\n\r",ch);
                            return;
                         }

                      if ( IS_CLASS( victim, CLASS_DROW ) || IS_CLASS( victim, CLASS_DRAGON ) ) 
                         {
                            stc( "They are immuned to your poison.\n\r", ch );
                            return;
                         }

                      if ( is_safe(ch, victim) )
                         return;

                      act("$n looks at $N with hatred shouting '#gVENOM!!!#n'",ch,NULL,victim,TO_NOTVICT);
                      act("You look at $N's with hatred shouting '#gVENOM!!!#n'",ch,NULL,victim,TO_CHAR);
                      act("$n looks at you with hatred shouting '#gVENOM!!!#n'",ch,NULL,victim,TO_VICT);

                      act("You scream as #gdeadly poison#n seeps into your blood!",victim,NULL,NULL,TO_CHAR);
                      act("$n screams as #gdeadly poison#n seeps into $s blood!",victim,NULL,NULL,TO_ROOM);

                      ch->poison_timer = 15;

                      if ( !IS_SET(victim->more, MORE_POISON) )
                         SET_BIT(victim->more, MORE_POISON);

                      if ( !IS_CLASS( victim, CLASS_DUERGAR ) )
                         {
                            ch->attack_type = ATTACK_MAGICAL;
                            ch->attack_var = HIT;
                            ch->attack_focus = DIRECT;
                            hurt_person( ch, victim, 1000000 + UMIN( 250000, ch->pkill * 12500 ), TRUE );
                         }
                         else victim->hit = UMIN( victim->max_hit, victim->hit + 1000000 + UMIN( 250000, ch->pkill * 12500 ) );

                      ch->wait = 12;
                      return;
                   }
                   else {
                           send_to_char("Poison whom?\n\r",ch);
                           return;
                        }
             }
     else if ( !str_cmp(arg,"blind") )
             {
                if ( !( arg2[0] == '\0' ) || ch->fighting != NULL )
                   {
                      if ( arg2[0] == '\0' )
                         victim = ch->fighting;
                         else if ( ( victim = get_char_room(ch, arg2) ) == NULL )
                                 {
                                    send_to_char("They are not here.\n\r", ch );
                                    return;
                                 }
 
                      if ( ch->pcdata->powers[POWER_BLIND] > 0 )
                         {
                            send_to_char("You cannot use this powerword yet.\n\r",ch);
                            return;
                         }

                      if ( is_safe(ch, victim) )
                         return;

                      act("$n touches $N's forehead and screams '#eMIDNIGHT!!!#n'",ch,NULL,victim,TO_NOTVICT);
                      act("You touch $N's forehead and screams '#eMIDNIGHT!!!#n'",ch,NULL,victim,TO_CHAR);
                      act("$n touches your forhead and screams '#eMIDNIGHT!!!#n'",ch,NULL,victim,TO_VICT);
                      act("Everything goes #edark#n around you as your worst #wnightmares#n come to life!",victim,NULL,NULL,TO_CHAR);
                      act("Everything goes #edark#n around $n as $s worst #wnightmares#n come to life!",victim,NULL,NULL,TO_ROOM);

                      if ( !IS_SET( victim->more, MORE_BLIND ) )
                         {
                            SET_BIT( victim->more, MORE_BLIND );
                            victim->blind_timer = 5;
                         }

                      hit_gain( ch, 500000 );

                      ch->wait = 6;
                      ch->pcdata->powers[POWER_BLIND] = 25;
                      return;
                   }
                   else {
                           send_to_char("Blind whom?\n\r",ch);
                           return;
                        }
             }
  else if ( !str_cmp(arg,"terror") )
  {
    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if (!(arg2[0] == '\0') || ch->fighting != NULL)
    {
      if (arg2[0] == '\0') victim = ch->fighting;
      else if ((victim = get_char_room(ch, arg2)) == NULL)
      {
        send_to_char("They are not here.\n\r", ch );
        return;
      }
      if (is_safe(ch, victim)) return;

      ch->wait = 12;
      subtract_stock( ch, 100 );
      SET_BIT( ch->arena, AFF2_DM );

      act("$n touches $N's forehead and screams '#eT#wE#eR#wR#eO#wR#e!!!#n'",ch,NULL,victim,TO_NOTVICT);
      act("You touch $N's forehead and screams '#eT#wE#eR#wR#eO#wR#e!!!#n'",ch,NULL,victim,TO_CHAR);
      act("$n touches your forhead and screams '#eT#wE#eR#wR#eO#wR#e!!!#n'",ch,NULL,victim,TO_VICT);
      act("#PYou #ecower on your knees #wgiving #eup all will to #yfight#e.#n",victim,NULL,NULL,TO_CHAR);
      act("#P$n #ecowers on $s knees #wgiving #eup all will to #yfight#e.#n",victim,NULL,NULL,TO_ROOM);
 
      if ( ch->ctimer[4] == 0 )
         victim->noblock = 1;

      if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
           && ch->ctimer[9] != 100 ) )
         ch->ctimer[4] = 2;

      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, victim, 1000000, TRUE );

      return;
    }
    else
    {
      send_to_char("Scare whom?\n\r",ch);
      return;
    }
  }
  else if ( !str_cmp(arg,"leech") )
  {
    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if (!(arg2[0] == '\0') || ch->fighting != NULL)
    {
      if (arg2[0] == '\0') victim = ch->fighting;
      else if ((victim = get_char_room(ch, arg2)) == NULL)
      {
        send_to_char("They are not here.\n\r", ch );
        return;
      }
      if (is_safe(ch, victim)) return;

      ch->wait = 12;
      subtract_stock( ch, 200 );
      SET_BIT( ch->arena, AFF2_DM );

      act("$n touches $N's forehead and screams '#eL#wE#eE#wE#eC#wH#e!!!#n'",ch,NULL,victim,TO_NOTVICT);
      act("You touch $N's forehead and screams '#eL#wE#eE#wE#eC#wH#e!!!#n'",ch,NULL,victim,TO_CHAR);
      act("$n touches your forhead and screams '#eL#wE#eE#wE#eC#wH#e!!!#n'",ch,NULL,victim,TO_VICT);
      act("#yYou #eswoon in esctacy as you feel #P$N#P'n #wlife#eforce sucked into your body!#n",ch,NULL,victim,TO_CHAR);
      act("#y$n #eswoons in esctacy as $e feels #Pyour #wlife#eforce sucked into $s body!#n",ch,NULL,victim,TO_VICT);
      act("#y$n #eswoons in esctacy as $e feels #P$N#P'n #wlife#eforce sucked into $s body!#n",ch,NULL,victim,TO_NOTVICT);

      hit_gain( ch, 1000000 );

      if ( ch->ctimer[5] == 0 )
         {
            victim->mana = UMAX( 0, victim->mana - 1500000 );
            victim->move = UMAX( 0, victim->move - 1500000 );
         }

      if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
           && ch->ctimer[9] != 100 ) )
         ch->ctimer[5] = 2;

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, victim, 2000000, TRUE );

      return;
    }
    else
    {
      send_to_char("Leech whom?\n\r",ch);
      return;
    }
  }
  else if ( !str_cmp(arg,"oblivion") )
  {
    CHAR_DATA *ich_next;

    if ( get_stock( ch ) < 300 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    ch->wait = 12;
    subtract_stock( ch, 300 );
    SET_BIT( ch->arena, AFF2_DM );

    act("You clench your fists as you scream '#eO#wB#eL#wI#eV#wI#eO#wN#e!!!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n clench $s fists as $e screams '#eO#wB#eL#wI#eV#wI#eO#wN#e!!!#n'",ch,NULL,NULL,TO_ROOM);
    act("The room is suddenly engulfed by a mysterious dark light.",ch,NULL,NULL,TO_CHAR);
    act("The room is suddenly engulfed by a mysterious dark light.",ch,NULL,NULL,TO_ROOM);
    act("You take off your #emask#n showing all your #C#zfearful#n, eyes of #Rfire#n!",ch,NULL,NULL,TO_CHAR);
    act("$n takes off $s #emask#n showing you $s #C#zfearful#n, eyes of #Rfire#n!",ch,NULL,NULL,TO_ROOM);

    for (ich = ch->in_room->people; ich != NULL; ich = ich_next )
    {
      ich_next = ich->next_in_room;
      if (ich != ch)
      {
         if (is_safe(ch,ich)) continue;

         act("$n looks at $N and utters '#RPRAY!!!#n'",ch,NULL,ich,TO_NOTVICT);
         act("You look at $N and say '#RPRAY!!!#n'",ch,NULL,ich,TO_CHAR);
         act("$n looks at you and utters '#RPRAY!!!#n'",ch,NULL,ich,TO_VICT);

         ch->ele_attack = ATTACK_DARK;
         ch->attack_type = ATTACK_MAGICAL;
         ch->attack_var = SOUL;
         ch->attack_focus = UPPERCUT;
         hurt_person( ch, ich, 3500000, TRUE );
      }
    }
  }
  else {
          send_to_char("You have not learned that powerword yet.\n\r",ch);
          return;
       }
}

