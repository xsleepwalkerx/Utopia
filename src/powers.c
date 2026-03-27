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

bool can_do_skill( CHAR_DATA *ch, int num )
{
   OBJ_DATA *obj;

   if ( ch->tier < UMIN( 5, ( ( num - 1 ) / 20 ) + 2 ) )
      return FALSE;

   if ( IS_CLASS( ch, CLASS_MASTER ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_BODY ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_ABOUT ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_HEAD ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_FINGER_L ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_FINGER_R ) ) != NULL && ( obj->value[1] == num || obj->value[2] == num )
        && ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR ) )
      return TRUE;

   return FALSE;
}

void do_subsitance( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn, sn2;
   AFFECT_DATA af;

   if ( ( sn = skill_lookup( "subsitance" ) ) < 0 )
      return;

   if ( ( sn2 = skill_lookup( "overdrive" ) ) < 0 )
      return;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 58 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 2500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   ch->wait = 4;
   ch->move -= 2500000;

   act("You crouch on your knees as you look around yourself cautiously.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches on $s knees as $e looks around $mself cautiously.",ch,NULL,NULL,TO_ROOM);
   act("#yYou #Cthen throw a pill on the ground as the room goes #w#zWHITE#n#C!!!#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n #Cthen throws a pill on the ground as the room goes #w#zWHITE#n#C!!!#n",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) )
         continue;

      if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
           && ch->pcdata->kingdom > 0 ) )
         continue;

      if ( is_affected( vch, sn ) || is_affected( vch, sn2 ) )
         continue;

      act( "You clutch your head in rage as the shinobi releases your inner powers!", vch, NULL, NULL, TO_CHAR );
      act( "$n clutches $s head in rage as the shinobi releases $s inner powers!", vch, NULL, NULL, TO_ROOM );

      af.type      = sn;
      af.duration  = 100;
      af.modifier  = 5;
      af.location  = APPLY_EVA;
      af.bitvector = 0;
      affect_to_char( vch, &af );
   }
}

void do_divine_might( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn;

   if ( ( sn = skill_lookup( "divine might" ) ) < 0 )
      return;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 36 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->hit > ch->max_hit * 0.25 )
      {
         send_to_char( "You are too healthy to perform this technique.\n\r", ch );
         return;
      }

   if ( ch->move < 2500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   ch->wait = 4;
   ch->ctimer[10] = 8;
   ch->move -= 2500000;

   act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);
   act("The ground starts trembling beneath your feet as you charge up.",ch,NULL,NULL,TO_CHAR);
   act("The ground starts trembling beneath $n's feet as $e charges up.",ch,NULL,NULL,TO_ROOM);
   act("#yYou #Cthen SMASH your foot on the ground as the room goes #w#zWHITE#n#C!!!#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n #Cthen SMASHES $s foot on the ground as the room goes #w#zWHITE#n#C!!!#n",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) )
         continue;

      if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
           && ch->pcdata->kingdom > 0 ) )
         continue;

      if ( is_affected( vch, sn ) )
         continue;

      act( "You clutch your head in rage as the dragon releases your inner powers!", vch, NULL, NULL, TO_CHAR );
      act( "$n clutches $s head in rage as the dragon releases $s inner powers!", vch, NULL, NULL, TO_ROOM );

      (*skill_table[sn].spell_fun) ( sn, 2, vch, vch );
   }
}

void do_over_drive( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn, sn2;
   AFFECT_DATA af;

   if ( ( sn = skill_lookup( "overdrive" ) ) < 0 )
      return;

   if ( ( sn2 = skill_lookup( "subsitance" ) ) < 0 )
      return;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 79 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 2500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   ch->wait = 4;
   ch->move -= 2500000;

   act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);
   act("You yells '#yHui!#n' as you focus on the lifeforce around you.",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yHui!#n' as $e focuses on the lifeforce around $m.",ch,NULL,NULL,TO_ROOM);
   act("#yYou #Cthen SMASH your foot on the ground as the room goes #w#zWHITE#n#C!!!#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n #Cthen SMASHES $s foot on the ground as the room goes #w#zWHITE#n#C!!!#n",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) )
         continue;

      if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
           && ch->pcdata->kingdom > 0 ) )
         continue;

      if ( is_affected( vch, sn ) || is_affected( vch, sn2 ) )
         continue;

      act( "You clutch your head in rage as the dragon releases your inner powers!", vch, NULL, NULL, TO_CHAR );
      act( "$n clutches $s head in rage as the dragon releases $s inner powers!", vch, NULL, NULL, TO_ROOM );

      af.type      = sn;
      af.duration  = 100;
      af.modifier  = 5;
      af.location  = APPLY_TENSION;
      af.bitvector = AFF_OVERDRIVE;
      affect_to_char( vch, &af );
   }
}

void do_gambler( CHAR_DATA *ch, char *argument )
{
   if ( !can_do_skill( ch, 11 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   spell_gambler(skill_lookup("gambler"), 10, ch, ch);
}

void do_royal_flush( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !can_do_skill( ch, 35 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->saiyan_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Place the Royal Flush curse on whom?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( is_safe( ch, victim ) )
       return;

    ch->saiyan_timer = 40;
    spell_rflush(skill_lookup("royal flush"), 5, ch, victim);
}

void do_ultra_heal( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 57 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->monk_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->monk_timer = 30;

   act( "You stand proudly as you place your hands on your waist.", ch, NULL, NULL, TO_CHAR );
   act( "$n stands proudly as $e places $s hands on $s waist.", ch, NULL, NULL, TO_ROOM );
   act( "You smile at the crowd as you exclaim '#yBelieve in yourself!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n smiles at the crowd as $e exclaims '#yBelieve in yourself!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The crowd ROARS for their champion as you suddenly look alot healthier.", ch, NULL, NULL, TO_CHAR );
   act( "The crowd ROARS for their champion as $n suddenly looks alot healthier.", ch, NULL, NULL, TO_ROOM );
   hit_gain( ch, 1500000 );
}

void do_absorb( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 77 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->monk_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->monk_timer = 30;
   ch->absorb_timer = 1;

   act( "You do a small leap into the air as a manifest of energy engulfs your weapon.", ch, NULL, NULL, TO_CHAR );
   act( "$n does a small leap into the air as a manifest of energy engulfs $s weapon.", ch, NULL, NULL, TO_ROOM );
   act( "You scream '#RAbsorb!#n' as you softly land back onto your knees...", ch, NULL, NULL, TO_CHAR );
   act( "$n screams '#RAbsorb!#n' as $e softly lands back onto $s knees...", ch, NULL, NULL, TO_ROOM );
   act( "Your body suddenly start pulsing with a mysterious light as you grin evilly.", ch, NULL, NULL, TO_CHAR );
   act( "$n's body suddenly start pulsing with a mysterious light as $e grins evilly.", ch, NULL, NULL, TO_ROOM );
}

void do_double_heal( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 34 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->monk_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->monk_timer = 30;

   act( "You stand proudly as you flex your arms.", ch, NULL, NULL, TO_CHAR );
   act( "$n stands proudly as $e flexes $s arms.", ch, NULL, NULL, TO_ROOM );
   act( "You exclaim '#yYou can't touch this!#n' as you continue posing.", ch, NULL, NULL, TO_CHAR );
   act( "$n exclaims '#yYou can't touch this!#n' as $e continues posing.", ch, NULL, NULL, TO_ROOM );
   act( "The crowd ROARS for their fighter as you suddenly look healthier.", ch, NULL, NULL, TO_CHAR );
   act( "The crowd ROARS for their fighter as $n suddenly looks healthier.", ch, NULL, NULL, TO_ROOM );
   hit_gain( ch, 1000000 );
}

void do_barrier( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) && !can_do_skill( ch, 2 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_BARRIER ) )
      {
         stc("You release your focus on the room as the invisible barrier vanishes.",ch);
         REMOVE_BIT( ch->in_room->room_flags, ROOM_BARRIER );
         return;
      }

   if ( ch->move < 2000000 )
      {
         stc("You are too tired to perform this technique.\n\r",ch);
         return;
      }

   ch->move -= 2000000;
   SET_BIT( ch->in_room->room_flags, ROOM_BARRIER );

   act("You put your finger on your forehead as you concentrate furiously!", ch, NULL, NULL, TO_CHAR);
   act("$n put $s finger on $s forehead as $e concentrates furiously!", ch, NULL, NULL, TO_ROOM);
   act("You summon a mystical barrier across the room as your hair blows through the wind!", ch, NULL, NULL, TO_CHAR);
   act("$n summons a mystical barrier across the room as $s hair blows through the wind!", ch, NULL, NULL, TO_ROOM);
   ch->wait = 6;
}

void do_double_attack( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim, *ally;
    int dam;

    if ( IS_NPC(ch) )
       return;

    if ( nogang != 0 )
       {
          send_to_char( "You can only execute this attack when group battles are on.\n\r", ch );
          return;
       }

    if ( ch->anger_level < 301 )
       {
	    send_to_char("Your rage isn't powerful enough to execute this attack warrior.\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
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

    for ( ally = ch->in_room->people; ally != NULL; ally = ally->next_in_room )
    {
       if ( IS_NPC( ally ) || ally == ch || !is_same_group( ch, ally ) )
          continue;
          else break;
    }

    if ( ally == NULL )
       {
          stc( "You have noone in your group to help assist with the attack!\n\r", ch );
          return;
       }
 
    if ( IS_CLASS( ch, CLASS_WISP ) )
       ch->wait = 4;
       else ch->wait = 12;

    ally->wait = 12;

    act( "#wThe room goes #edark as a #cgentle #yspotlight #eshines down upon #Pyou#w.#n", ch, NULL, NULL, TO_CHAR );
    act( "#wThe room goes #edark as a #cgentle #yspotlight #eshines down upon #P$n#w.#n", ch, NULL, NULL, TO_ROOM );

    switch( number_range( 1, 7 ) )
    {
       default:
       case 1: act( "You ask '#yAre you ok?#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n asks '#yAre you ok?#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;

       case 2: act( "You exclaim '#yLets finish this quick!#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n exclaims '#yLets finish this quick!#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;

       case 3: act( "You exclaim '#yWatch my back!#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n exclaims '#yWatch my back!#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;

       case 4: act( "You snarl '#yShow no mercy...#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n snarls '#yShow no mercy...#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;

       case 5: act( "You say '#yTo protect our loved ones.#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n says '#yTo protect over loved ones.#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;

       case 6: act( "You say '#yThis is the end for you.#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n says '#yThis is the end for you.#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;

       case 7: act( "You ask '#yCan you do this?#n' as you raise your weapon proudly.", ch, NULL, NULL, TO_CHAR );
               act( "$n asks '#yCan you do this?#n' as $e raises $s weapon proudly.", ch, NULL, NULL, TO_ROOM );
               break;
    }

    act( "#wSuddenly#e, another #cgentle #yspotlight #eshines down upon #Pyou#w.#n", ally, NULL, NULL, TO_CHAR );
    act( "#wSuddenly#e, another #cgentle #yspotlight #eshines down upon #P$n#w.#n", ally, NULL, NULL, TO_ROOM );

    switch( number_range( 1, 7 ) )
    {
       default:
       case 1: act( "You exclaim '#yWe got to win!#n' as you raise your weapon too.", ally, NULL, NULL, TO_CHAR );
               act( "$n exclaims '#yWe got to win!#n' as $e raises $s weapon too.", ally, NULL, NULL, TO_ROOM );
               break;

       case 2: act( "You exclaim '#yShut the hell up!#n' as you raise your weapon too.", ally, NULL, NULL, TO_CHAR );
               act( "$n exclaims '#yShut the hell up!#n' as $e raises $s weapon too.", ally, NULL, NULL, TO_ROOM );
               break;

       case 3: act( "You exclaim '#yGotta trust your buds!#n' as you raise your weapon too.", ally, NULL, NULL, TO_CHAR );
               act( "$n exclaims '#yGotta trust your buds!#n' as $e raises $s weapon too.", ally, NULL, NULL, TO_ROOM );
               break;

       case 4: act( "You snarl '#yDeath to the enemy dogs...#n' as you raise your weapon too.", ally, NULL, NULL, TO_CHAR );
               act( "$n snarls '#yDeath to the enemy dogs...#n' as $e raises $s weapon too.", ally, NULL, NULL, TO_ROOM );
               break;

       case 5: act( "You say '#yUnderstood.#n' as you raise your weapon too.", ally, NULL, NULL, TO_CHAR );
               act( "$n says '#yUnderstood.#n' as $e raises $s weapon too.", ally, NULL, NULL, TO_ROOM );
               break;

       case 6: act( "You say '#yI got your back.#n' as you raise your weapon too.", ally, NULL, NULL, TO_CHAR );
               act( "$n says '#yI got your back.#n' as $e raises $s weapon too.", ally, NULL, NULL, TO_ROOM );
               break;

       case 7: act( "You say '#yI will never let ya down $t#n' as you raise your weapon too.", ally, ch->pcdata->switchname, NULL, TO_CHAR );
               act( "$n says '#yI will never let ya down $t#n' as $e raises $s weapon too.", ally, ch->pcdata->switchname, NULL, TO_ROOM );
               break;
    }

    act( "#wYou scream '#yYai!#w' as you #Rswing #wyour weapon #C#zWiLdLy#n #wacross #e$N#w!#n", ch, NULL, victim, TO_CHAR );
    act( "#w$n screams '#yYai!#w' as $e #Rswings #w$s weapon #C#zWiLdLy#n #wacross #eyou#w!#n", ch, NULL, victim, TO_VICT );
    act( "#w$n screams '#yYai!#w' as $e #Rswings #w$s weapon #C#zWiLdLy#n #wacross #e$N#w!#n", ch, NULL, victim, TO_NOTVICT );

    act( "#wYou scream '#yThere!#w' as you #cfollow #wsuit with your #Rattack #won #e$N #walso!#n", ally, NULL, victim, TO_CHAR );
    act( "#w$n screams '#yThere!#w' as $e #cfollows #wsuit with $s #Rattack #won #eyou #walso!#n", ally, NULL, victim, TO_VICT );
    act( "#w$n screams '#yThere!#w' as $e #cfollows #wsuit with $s #Rattack #won #e$N #walso!#n", ally, NULL, victim, TO_NOTVICT );

    act( "#PYou #pScReAm in #RPAIN #pas your body is #wFeRoCiOuSlY #C#zRIPPED#n #papart!#n", victim, NULL, NULL, TO_CHAR );
    act( "#P$n #pScReAms in #RPAIN #pas $s body is #wFeRoCiOuSlY #C#zRIPPED#n #papart!#n", victim, NULL, NULL, TO_ROOM );

    dam = ( get_curr_str( ch ) + get_curr_str( ally ) + 2 ) * 25000;
    dam = UMAX( 2500000, dam );

    SET_BIT( ch->arena, AFF2_POWER );
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, dam, TRUE );

    ch->anger_level = 0;
}

void do_shadow_thrust( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 5 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You fade into the ground as your body blurs out of existence.", ch, NULL, NULL, TO_CHAR );
    act( "$n fades into the ground as $s body blurs out of existence.", ch, NULL, NULL, TO_ROOM );
    act( "You tunnel towards $N as your kiotic energy flashes with intensity.", ch, NULL, victim, TO_CHAR );
    act( "$n tunnels towards you as $e kiotic energy flashes with intensity.", ch, NULL, victim, TO_VICT );
    act( "$n tunnels towards $N as $e kiotic energy flashes with intensity.", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "You leap from the ground slashing across $N's chest as $E bursts into flames!", ch, NULL, victim, TO_CHAR );
    act( "$n leaps from the ground slashing across your chest as you burst into flames!", ch, NULL, victim, TO_VICT );
    act( "$n leaps from the ground slashing across $N's chest as $E bursts into flames!", ch, NULL, victim, TO_NOTVICT );
    act( "You leap from the ground slashing across $N's chest as $E bursts into flames!", ch, NULL, victim, TO_CHAR );
    act( "$n leaps from the ground slashing across your chest as you burst into flames!", ch, NULL, victim, TO_VICT );
    act( "$n leaps from the ground slashing across $N's chest as $E bursts into flames!", ch, NULL, victim, TO_NOTVICT );
    act( "You leap from the ground slashing across $N's chest as $E bursts into flames!", ch, NULL, victim, TO_CHAR );
    act( "$n leaps from the ground slashing across your chest as you burst into flames!", ch, NULL, victim, TO_VICT );
    act( "$n leaps from the ground slashing across $N's chest as $E bursts into flames!", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "Your shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_CHAR );
    act( "$n's shadow leaps from the ground slashing across your chest!", ch, NULL, victim, TO_VICT );
    act( "$n's shadow leaps from the ground slashing across $N's chest!", ch, NULL, victim, TO_NOTVICT );
    act( "You scream as a powerful force rip you across the vicinity!", victim, NULL, NULL, TO_CHAR );
    act( "$n screams as a powerful force rips $n across the vicinity!", victim, NULL, NULL, TO_ROOM );

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_DARK;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 1000000, TRUE );

    if ( number_percent() > 50 - devil_luck( ch, victim, 5 ) && victim != NULL && victim->position > POS_STUNNED )
       {
          act( "#eYour hand shimmers as you rapidly sheath your blade back in a blur.#n", ch, NULL, NULL, TO_CHAR );
          act( "#e$n#e's hand shimmers as $e rapidly sheathes $s blade back in a blur.#n", ch, NULL, NULL, TO_ROOM );
          special_slam( victim, number_range( 0, 4 ) );
          stop_fighting( victim, TRUE ); 
       }
}

void do_crescent_slash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 10 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You swerve your hands stylishly as you go on one knee.", ch, NULL, NULL, TO_CHAR );
    act( "$n swerves $s hands stylishly as $e goes on one knee.", ch, NULL, NULL, TO_ROOM );
    act( "You then slash a hand in a deadly spiral as an arc of energy appears.", ch, NULL, NULL, TO_CHAR );
    act( "$n then slashes a hand in a deadly spiral as an arc of energy appears.", ch, NULL, NULL, TO_ROOM );
    act( "You then twirl the other sending the deadly arc towards $N!", ch, NULL, victim, TO_CHAR );
    act( "$n then twirls the other sending the deadly arc towards you!", ch, NULL, victim, TO_VICT );
    act( "$n then twirls the other sending the deadly arc towards $N!", ch, NULL, victim, TO_NOTVICT );
    act( "Your entire body is suddenly blast away from the power of Luna!", victim, NULL, NULL, TO_CHAR );
    act( "$n's entire body is suddenly blast away from the power of Luna!", victim, NULL, NULL, TO_ROOM );

    if ( number_percent() > 50 - devil_luck( ch, victim, 5 ) )
       {
          act( "#oYou scream '#wHave a good day!#o' as the lunar eclipse absorbs $N's energy.#n", ch, NULL, victim, TO_CHAR );
          act( "#o$n screams '#wHave a good day!#o' as the lunar eclipse absorbs your energy.#n", ch, NULL, victim, TO_VICT );
          act( "#o$n screams '#wHave a good day!#o' as the lunar eclipse absorbs $N's energy.#n", ch, NULL, victim, TO_NOTVICT );
          victim->mana = UMAX( 0, victim->mana - ( victim->max_mana * 0.1 ) );
          victim->move = UMAX( 0, victim->move - ( victim->max_move * 0.1 ) );
       }

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_LUNAR;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_mirror_stance( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 61 ) )
      {
         send_to_char("Huh?\n\r",ch);
 	   return;
      }

   if ( ch->aragami_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->position != POS_FIGHTING )
      {
         send_to_char("But you aren't fighting anyone master.\n\r",ch);
         return;
      }

   ch->aragami_timer = 40;

   if ( IS_SET(ch->more2, MORE2_MIRROR) )
      {
         send_to_char("#yYou #ware already #Canticipating #wfor an incoming attack.#n\n\r",ch);
         return;
      }

   SET_BIT(ch->more2, MORE2_MIRROR);

   send_to_char("#yYou #wlean forward as you #Ccalmly #wraise your #ysword #win anticipation of an attack.#n\n\r",ch);
   act("#y$n #wleans forward as $e #Ccalmly #wraises $s #ysword #win anticipation of an attack.#n",ch,NULL,NULL,TO_ROOM);
}

void do_xbuster( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 62 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You strut a little in front of you as you push your right arm aside.", ch, NULL, NULL, TO_CHAR );
    act( "$n struts a little in front of $m as $e pushes $s right arm aside.", ch, NULL, NULL, TO_ROOM );
    act( "You mutter '#wCharging...#n' as everyone gets draw into a bluish vortex.", ch, NULL, NULL, TO_CHAR );
    act( "$n mutters '#wCharging...#n' as everyone gets draw into a bluish vortex.", ch, NULL, NULL, TO_ROOM );
    act( "You jolt back your head as your hair suddenly grows very long and silkly.", ch, NULL, NULL, TO_CHAR );
    act( "$n jolts back $s head as $s hair suddenly grows very long and silkly.", ch, NULL, NULL, TO_ROOM );
    act( "Your hair blows graciously in the wind as your back suddenly starts cracking!", ch, NULL, NULL, TO_CHAR );
    act( "$n's hair blows graciously in the wind as $s back suddenly starts cracking!", ch, NULL, NULL, TO_ROOM );
    act( "It suddenly opens like a flower petal as a sleek visor falls over your eyes.", ch, NULL, NULL, TO_CHAR );
    act( "It suddenly opens like a flower petal as a sleek visor falls over $n's eyes.", ch, NULL, NULL, TO_ROOM );
    act( "A spiral of light manifests on each leaf as a soft hum comes from your stomach.", ch, NULL, NULL, TO_CHAR );
    act( "A spiral of light manifests on each leaf as a soft hum comes from $n's stomach.", ch, NULL, NULL, TO_ROOM );
    act( "You then spread your arms apart as your back releases TONS of laser beams!!!", ch, NULL, NULL, TO_CHAR );
    act( "$n then spreads $s arms apart as $s back releases TONS of laser beams!!!", ch, NULL, NULL, TO_ROOM );
    act( "All the beams does a smooth curve as they fly into the centre of the room.", ch, NULL, NULL, TO_CHAR );
    act( "All the beams does a smooth curve as they fly into the centre of the room.", ch, NULL, NULL, TO_ROOM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       act( "The mysterious beams RIP through your body leaving holes and blood all over it.", vch, NULL, NULL, TO_CHAR );
       act( "The mysterious beams RIP through $n's body leaving holes and blood all over it.", vch, NULL, NULL, TO_ROOM );

       ch->ele_attack = ATTACK_HOLY;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BEAM;
       ch->attack_focus = number_range( 0, 4 );
       hurt_person( ch, vch, 3500000, TRUE );
    }

    act( "A grin appears on your face as the room EXPLODES with ferocious energy!!!", ch, NULL, NULL, TO_CHAR );
    act( "A grin appears on $n's face as the room EXPLODES with ferocious energy!!!", ch, NULL, NULL, TO_ROOM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) || IS_SET( vch->more, MORE_BLIND ) )
          continue;

       SET_BIT( vch->more, MORE_BLIND );
       vch->blind_timer = 10;
    }
}

void do_forever_zero( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 63 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You say '#CLets go!#n' as you tightly hold your trembling hand...", ch, NULL, NULL, TO_CHAR );
    act( "$n says '#CLets go!#n' as $e tightly holds $s trembling hand...", ch, NULL, NULL, TO_ROOM );
    act( "Your eyes start pusling with energy as sparks fizzing off your body.", ch, NULL, NULL, TO_CHAR );
    act( "$n's eyes start pusling with energy as sparks fizzing off $s body.", ch, NULL, NULL, TO_ROOM );
    act( "A manifest of blinding energy engulfs your hand as you strain your head away.", ch, NULL, NULL, TO_CHAR );
    act( "A manifest of blinding energy engulfs $n's hand as $e strains your head away.", ch, NULL, NULL, TO_ROOM );
    act( "Your feet starts sinking into the ground as a chilly wind encircles the room.", ch, NULL, NULL, TO_CHAR );
    act( "$n's feet starts sinking into the ground as a chilly wind encircles the room.", ch, NULL, NULL, TO_ROOM );
    act( "The energy mass turns crystal clear as it glows bigger and pulse faster.", ch, NULL, NULL, TO_CHAR );
    act( "The energy mass turns crystal clear as it glows bigger and pulse faster.", ch, NULL, NULL, TO_ROOM );
    act( "Suddenly you release it into the air as a shadowly form leaps after it!", ch, NULL, NULL, TO_CHAR );
    act( "Suddenly $n releases it into the air as a shadowly form leaps after it!", ch, NULL, NULL, TO_ROOM );
    act( "#wThe #eform #wmerges into #CShiva #was the entire surrounding is engulfed in #Cice#w!#n", ch, NULL, NULL, TO_CHAR );
    act( "#wThe #eform #wmerges into #CShiva #was the entire surrounding is engulfed in #Cice#w!#n", ch, NULL, NULL, TO_ROOM );
    act( "After touching the mass briefly, she pushes it straight towards the ground.", ch, NULL, NULL, TO_CHAR );
    act( "After touching the mass briefly, she pushes it straight towards the ground.", ch, NULL, NULL, TO_ROOM );
    act( "Still being sucked into the earth, you use ALL of your energy to bring your hands down...", ch, NULL, NULL, TO_CHAR );
    act( "Still being sucked into the earth, $n uses ALL of $s energy to bring $s hands down...", ch, NULL, NULL, TO_ROOM );
    act( "At the very last second, both your hand and the mass of energy connects at the ground.", ch, NULL, NULL, TO_CHAR );
    act( "At the very last second, both $n's hand and the mass of energy connects at the ground.", ch, NULL, NULL, TO_ROOM );
    act( "The vicinity goes dark as a blinding light shatters ALL!!!", ch, NULL, NULL, TO_CHAR );
    act( "The vicinity goes dark as a blinding light shatters ALL!!!", ch, NULL, NULL, TO_ROOM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       if ( !IS_SET( vch->more, MORE_FROZEN ) && !is_martial( vch ) )
          {
             SET_BIT( vch->more, MORE_FROZEN );
             vch->freeze_timer = 2;
          }

       ch->ele_attack = ATTACK_ICE;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BEAM;
       ch->attack_focus = UPPERCUT;
       hurt_person( ch, vch, 3000000, TRUE );
    }
}

void do_explode( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 66 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You do a small leap into the air as a manifest of energy engulfs your weapon.", ch, NULL, NULL, TO_CHAR );
    act( "$n does a small leap into the air as a manifest of energy engulfs $s weapon.", ch, NULL, NULL, TO_ROOM );
    act( "You scream '#RExplode!#n' as you softly land back onto your knees...", ch, NULL, NULL, TO_CHAR );
    act( "$n screams '#RExplode!#n' as $e softly lands back onto $s knees...", ch, NULL, NULL, TO_ROOM );
    act( "Mass of small explosions fill the room as you silently watch them.", ch, NULL, NULL, TO_CHAR );
    act( "Mass of small explosions fill the room as you try in vain to dodge them.", ch, NULL, NULL, TO_ROOM );
    act( "The ground starts trembling violently as the explosions get even BIGGER!", ch, NULL, NULL, TO_CHAR );
    act( "The ground starts trembling violently as the explosions get even BIGGER!", ch, NULL, NULL, TO_ROOM );
    SET_BIT( ch->arena, AFF2_POWER );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       act( "A TREMEDOUS explosion of flames sent you FLYING into the skies!", vch, NULL, NULL, TO_CHAR );
       act( "A TREMEDOUS explosion of flames sent $n FLYING into the skies!", vch, NULL, NULL, TO_ROOM );

       ch->ele_attack = ATTACK_FIRE;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BLAST;
       ch->attack_focus = UPPERCUT;
       hurt_person( ch, vch, 2500000, TRUE );

       if ( vch != NULL && vch->position > POS_STUNNED )
          {
             stop_fighting( vch, TRUE );
             vch->air_timer = number_range( 3, 5 );
          }
    }
}

void do_fire_bomb( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 67 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    victim->noskill = 2;

    act( "You strut a little in front of you as you push your right arm aside.", ch, NULL, NULL, TO_CHAR );
    act( "$n struts a little in front of $m as $e pushes $s right arm aside.", ch, NULL, NULL, TO_ROOM );
    act( "You mutter '#RCharging...#n' as your arm transforms into a fiery lance.", ch, NULL, NULL, TO_CHAR );
    act( "$n mutters '#RCharging...#n' as $s arm transforms into a fiery lance.", ch, NULL, NULL, TO_ROOM );
    act( "You slash violently across the air as you say '#RFire Bomb!#n'", ch, NULL, NULL, TO_CHAR );
    act( "$n slashes violently across the air as $e says '#RFire Bomb!#n'", ch, NULL, NULL, TO_ROOM );
    act( "You violently thrust your blade through $N's chest as it vanishes.", ch, NULL, victim, TO_CHAR );
    act( "$n violently thrusts $s blade through your chest as it vanishes.", ch, NULL, victim, TO_VICT );
    act( "$n violently thrusts $s blade through $N's chest as it vanishes.", ch, NULL, victim, TO_NOTVICT );
    act( "You sneer '#yMove and Die!#n' as you leap back into your battle posure.", ch, NULL, NULL, TO_CHAR );
    act( "$n sneers '#yMove and Die!#n' as $e leaps back into $s battle posure.", ch, NULL, NULL, TO_ROOM );
}

void do_rhammer( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 72 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You strut a little in front of you as you push your right arm aside.", ch, NULL, NULL, TO_CHAR );
    act( "$n struts a little in front of $m as $e pushes $s right arm aside.", ch, NULL, NULL, TO_ROOM );
    act( "You mutter '#yCharging...#n' as your arm transforms into a MASSIVE anvil.", ch, NULL, NULL, TO_CHAR );
    act( "$n mutters '#yCharging...#n' as $s arm transforms into a MASSIVE anvil.", ch, NULL, NULL, TO_ROOM );
    act( "You grin evilly saying '#yRHammer#n' as you smash it across $N's face!", ch, NULL, victim, TO_CHAR );
    act( "$n grins evilly saying '#yRHammer#n' as $e smashes it across your face!", ch, NULL, victim, TO_VICT );
    act( "$n grins evilly saying '#yRHammer#n' as $e smashes it across $N's face!", ch, NULL, victim, TO_NOTVICT );
    act( "As $N staggers from the blow, you drive the anvil up $s chest.", ch, NULL, victim, TO_CHAR );
    act( "As you staggers from the blow, $n drives the anvil up your chest.", ch, NULL, victim, TO_VICT );
    act( "As $N staggers from the blow, $n drives the anvil up $s chest.", ch, NULL, victim, TO_NOTVICT );
    act( "You scream '#yHai Hai!#n' while continue pummelling $N's body into a pulp!", ch, NULL, victim, TO_CHAR );
    act( "$n screams '#yHai Hai!#n' while continue pummelling your body into a pulp!", ch, NULL, victim, TO_VICT );
    act( "$n screams '#yHai Hai!#n' while continue pummelling $N's body into a pulp!", ch, NULL, victim, TO_NOTVICT );
    act( "You then leap high above $N's head as shard of light radiate of you.", ch, NULL, victim, TO_CHAR );
    act( "$n then leaps high above your head as shard of light radiate of $m.", ch, NULL, victim, TO_VICT );
    act( "$n then leaps high above $N's head as shard of light radiate of $m.", ch, NULL, victim, TO_NOTVICT );
    act( "You scream '#yHui!#n' as you DRIVE the anvil down into $N's head!!!", ch, NULL, victim, TO_CHAR );
    act( "$n screams '#yHui!#n' as $e DRIVES the anvil down into your head!!!", ch, NULL, victim, TO_VICT );
    act( "$n screams '#yHui!#n' as $e DRIVES the anvil down into $N's head!!!", ch, NULL, victim, TO_NOTVICT );

    if ( !is_martial( victim ) )
       victim->stunned += 2;

    act( "You scream in sheer pain as your body convulses from the HORRIFYING sets of shockwaves!", victim, NULL, NULL, TO_CHAR );
    act( "$n screams in sheer pain as $s body convulses from the HORRIFYING sets of shockwaves!", victim, NULL, NULL, TO_ROOM );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLUNT;
    ch->attack_focus = OVERHEAD;
    hurt_person( ch, victim, 3000000, TRUE );
}

void do_light_blade( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 73 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    if ( !IS_SET( victim->more3, MORE3_CONFUSE ) && ( victim->in_room->vnum < 33500 || victim->in_room->vnum > 33800 ) )
       SET_BIT( victim->more3, MORE3_CONFUSE );

    act("You suddenly grimaces as flashes of your skull laughs at your foes.",ch,NULL,NULL,TO_CHAR);
    act("$n suddenly grimaces as flashes of $s skull laughs at you.",ch,NULL,NULL,TO_ROOM);
    act("You spreads your arms as you cackle '#eAll of you shall suffer!!!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n spreads $s arms as $e cackles '#eAll of you shall suffer!!!#n'",ch,NULL,NULL,TO_ROOM);
    act("A menacing blade of light suddenly manifests into your hand.",ch,NULL,NULL,TO_CHAR);
    act("A menacing blade of light suddenly manifests into $n's hand.",ch,NULL,NULL,TO_ROOM);
    act("You then start dashing towards $N as your body split into unlimited images!",ch,NULL,victim,TO_CHAR);
    act("$n then starts dashing towards you as $s body split into unlimited images!",ch,NULL,victim,TO_CHAR);
    act("$n then starts dashing towards $N as $s body split into unlimited images!",ch,NULL,victim,TO_CHAR);

    act("An image of $n leaps in front of you while grinning evilly.",victim,NULL,NULL,TO_CHAR);
    act("It then slashes you savagely across the chest ripping it apart!",victim,NULL,NULL,TO_CHAR);
    act("It twirl its blade up your body sending you into the air.",victim,NULL,NULL,TO_CHAR);
    act("$n's features flickers into a skeletonly mask as it leap with you.",victim,NULL,NULL,TO_CHAR);
    act("The deadly blade of $s pulses with demonic energy as $e slams you on the ground.",victim,NULL,NULL,TO_CHAR);
    act("A voice screams '#eHui!#n' as the image SLASHES through you like BUTTER!!!",victim,NULL,NULL,TO_CHAR);

    act("#eAn image of #P$N #eleaps in front of #w$n #ewhile grinning #Pevilly#e.#n",victim,NULL,ch,TO_ROOM);
    act("In a #C#zflash#n of #ylight#n, $n is lying in a #Rbloody #eheap#n on the #oground#n.",victim,NULL,NULL,TO_ROOM);

    ch->attack_type = ATTACK_PHYSICAL;
    ch->ele_attack = ATTACK_DARK;
    ch->attack_var = SLASH;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 3000000, TRUE );
}

void do_satellite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 69 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act("You quickly take out your M.V.S unit as you activate its switches.",ch,NULL,NULL,TO_CHAR);
    act("$n quickly takes out $s M.V.S unit as $e activates its switches.",ch,NULL,NULL,TO_ROOM);
    act("Ready now, you scowl as you insert a cartridge into the device's slot.",ch,NULL,NULL,TO_CHAR);
    act("Ready now, $n scowls as $e inserts a cartridge into the device's slot.",ch,NULL,NULL,TO_ROOM);
    act("You draw back your weapon subtlely as you fire several rounds into space!!!",ch,NULL,NULL,TO_CHAR);
    act("$n draws back $s weapon subtlely as $e fires several rounds into space!!!",ch,NULL,NULL,TO_ROOM);

    act("You see a small twinkle in the sky.",ch,NULL,NULL,TO_ROOM);
    act("You finally release the huge manifest of energy as it ROARS TOWARDS THE EARTH!",ch,NULL,NULL,TO_CHAR);

    act("A HUGE EXPLOSION FROM ABOVE DEVASTATES the vicinity below you, BURSTING everything into OBLIVION!!!",ch,NULL,NULL,TO_CHAR);
    act("A HUGE EXPLOSION FROM ABOVE DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!",ch,NULL,NULL,TO_ROOM);
    SET_BIT( ch->arena, AFF2_POWER );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( !IS_NPC( vch ) || is_safe(ch,vch) )
          continue;

       act("#wYour #obody evaporates as the explosion #RANNIHILATES #oYOU!#n",d->character,NULL,NULL,TO_CHAR);
       act("#w$n#w's #obody evaporates as a tremedous explosion #RANNIHILATES #o$m!#n",d->character,NULL,NULL,TO_ROOM);
       ch->attack_type = ATTACK_PHYSICAL;
       ch->ele_attack = ATTACK_LUNAR;
       ch->attack_var = BEAM;
       ch->attack_focus = AERIAL;
       hurt_person( ch, vch, 3000000, TRUE );
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	 {
           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
              continue;

           act("#wYour #obody evaporates as the explosion #RANNIHILATES #oYOU!#n",d->character,NULL,NULL,TO_CHAR);
           act("#w$n#w's #obody evaporates as a tremedous explosion #RANNIHILATES #o$m!#n",d->character,NULL,NULL,TO_ROOM);
           ch->attack_type = ATTACK_PHYSICAL;
           ch->ele_attack = ATTACK_LUNAR;
           ch->attack_var = BEAM;
           ch->attack_focus = AERIAL;
           hurt_person( ch, d->character, 3000000, TRUE );
       }
    }
}

void do_divine_ball( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !can_do_skill( ch, 81 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act("You levitate a few feet in the air with your wings as you spread your arms.",ch,NULL,NULL,TO_CHAR);
    act("$n levitates a few feet in the air with $s wings as $e spread $s arms.",ch,NULL,NULL,TO_ROOM);
    act("You sneer as both your hands pulses with ferocious energy...",ch,NULL,NULL,TO_CHAR);
    act("$n sneers as both $s hands pulses with ferocious energy...",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("You then release a dazzling blast of energy towards your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a dazzling blast of energy towards $s foes!",ch,NULL,NULL,TO_ROOM);
    act("You cackle as you blast your foes with powerful energy!",ch,NULL,NULL,TO_CHAR);
    act("$n cackles as $e blasts $s foes with powerful energy!",ch,NULL,NULL,TO_ROOM);
    act("Admist the chaos, your body is suddenly shielded by a MASSIVE energy mass.",ch,NULL,NULL,TO_CHAR);
    act("Admist the chaos, $n's body is suddenly shielded by a MASSIVE energy mass.",ch,NULL,NULL,TO_ROOM);
    act("Using all of your might, you release the energy from your body...",ch,NULL,NULL,TO_CHAR);
    act("Using all of $s might, $n releases the energy from $s body...",ch,NULL,NULL,TO_ROOM);
    act("...and FlInG it towards the CeNtEr of the ViCiNiTy!!!",ch,NULL,NULL,TO_CHAR);
    act("...and FlInG it towards the CeNtEr of the ViCiNiTy!!!",ch,NULL,NULL,TO_ROOM);

    SET_BIT( ch->arena, AFF2_POWER );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( !IS_NPC( vch ) || is_safe(ch,vch) )
          continue;

       act("#yYour #wbody evaporates as the explosion #CANNIHILATES #wYOU!#n",vch,NULL,NULL,TO_CHAR);
       act("#y$n#y's #wbody evaporates as a tremedous explosion #CANNIHILATES #w$m!#n",vch,NULL,NULL,TO_ROOM);
       ch->attack_type = ATTACK_MAGICAL;
       ch->attack_var = BLAST;
       ch->attack_focus = AERIAL;
       hurt_person( ch, vch, 3000000, TRUE );
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	 {
           if ( is_safe( ch, d->character ) || ch->in_room->area != d->character->in_room->area )
              continue;

           act("#yYour #wbody evaporates as the explosion #CANNIHILATES #wYOU!#n",d->character,NULL,NULL,TO_CHAR);
           act("#y$n#y's #wbody evaporates as a tremedous explosion #CANNIHILATES #w$m!#n",d->character,NULL,NULL,TO_ROOM);
           ch->attack_type = ATTACK_MAGICAL;
           ch->attack_var = BLAST;
           ch->attack_focus = AERIAL;
           hurt_person( ch, d->character, 3000000, TRUE );
       }
    }
}

void do_yami_sugi( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC( ch ) && ch->pIndexData->vnum != 564 )
      return;

   argument = one_argument( argument, arg );

   if ( !IS_NPC(ch) )
      {
         if ( !can_do_skill( ch, 70 ) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( ch->throw_timer > 0 )
            {
               stc("You are still recovering from the last one.\n\r",ch);
               return;
            }
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to blast with flames?\n\r",ch);
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

   if ( !IS_NPC(ch) )
      {
         hit_gain( ch, 500000 );
         ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
         ch->throw_timer = 50;
      }

   act("You flick your wrist suddenly, grazing $N with a blast of flames.",ch,NULL,victim,TO_CHAR);
   act("$n flicks $s wrist suddenly, grazing you with a blast of flames.",ch,NULL,victim,TO_VICT);
   act("$n flicks $s wrist suddenly, grazing $N with a blast of flames.",ch,NULL,victim,TO_NOTVICT);
   act("You grin evilly as you release more flames into $N with your other hand.",ch,NULL,victim,TO_CHAR);
   act("$n grins evilly as $e releases more flames into you with $s other hand.",ch,NULL,victim,TO_VICT);
   act("$n grins evilly as $e releases more flames into $N with $s other hand.",ch,NULL,victim,TO_NOTVICT);
   act("As $N is catapulted into the air, you lean back as sheering pain seeps through you.",ch,NULL,victim,TO_CHAR);
   act("As you are catapulted into the air, $n leans back as sheering pain seeps through $m.",ch,NULL,victim,TO_VICT);
   act("As $N is catapulted into the air, $n leans back as sheering pain seeps through $m.",ch,NULL,victim,TO_NOTVICT);
   act("Your facial expression starts changing as you are filled with unatural rage.",ch,NULL,NULL,TO_CHAR);
   act("$n's facial expression starts changing as $e is filled with unatural rage.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yNo a Doi!#n' as you SlAm your arms downwards creating a MaSs of flames.",ch,NULL,NULL,TO_CHAR);
   act("A shadow screams '#yNo a Doi!#n' as it SlAms $s arms downwards creating a MaSs of flames.",ch,NULL,NULL,TO_ROOM);
   act("The demonic flames eats through your flesh as they blast you across the vicinity.",victim,NULL,NULL,TO_CHAR);
   act("The demonic flames eats through $n's flesh as they blast $m across the vicinity.",victim,NULL,NULL,TO_ROOM);
   SET_BIT( ch->arena, AFF2_POWER );

   hit_gain( ch, 500000 );

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
      SET_BIT( victim->affected_by, AFF_FLAMING );

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = BLAST;
   ch->attack_focus = UPPERCUT;
  
   if ( !IS_NPC(ch) )
      hurt_person( ch, victim, 2500000, TRUE );
      else if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
              hurt_person( ch, victim, 2500000, TRUE );
      else hurt_person( ch, victim, 1500000, TRUE );
}

void do_lunar_shield( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) && !can_do_skill( ch, 68 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( IS_CLASS( ch, CLASS_MASTER ) )
      {
         if ( ch->ctimer[7] > 0 )
            {
               stc("You are still recovering from the last one.\n\r",ch);
               return;
            }
      }

   if ( ch->saiyan_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( !IS_NPC( ch ) )
      ch->saiyan_timer = 40;

   ch->wait = 6;

   if ( IS_NPC( ch ) )
      ch->jedi_timer = 3;
      else if ( IS_CLASS( ch, CLASS_MASTER ) )
      {
         ch->ctimer[7] = 3;
         ch->jedi_timer = 4;
      }
      else ch->jedi_timer = 3;

   if ( !IS_NPC( ch ) && ch->pcdata->deity == DEI_LUNA )
      ch->jedi_timer++;

   act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
   act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
   act("You say, '#oLuna #wEnergy#n' as your sword shimmers intensely.",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#oLuna #wEnergy#n' as $s sword shimmers intensely.",ch,NULL,NULL,TO_ROOM);
   act("As the energy covers through your body, you suddenly feel protected.", ch, NULL, NULL, TO_CHAR);
   act("As the energy covers through $n's body, $e suddenly looks #P#zinvincible#n.", ch, NULL, NULL, TO_ROOM);
}

void do_rcannon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 64 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You strut a little in front of you as you push your right arm aside.", ch, NULL, NULL, TO_CHAR );
    act( "$n struts a little in front of $m as $e pushes $s right arm aside.", ch, NULL, NULL, TO_ROOM );
    act( "You mutter '#yCharging...#n' as your arm transforms into a powerful cannon.", ch, NULL, NULL, TO_CHAR );
    act( "$n mutters '#yCharging...#n' as $s arm transforms into a powerful cannon.", ch, NULL, NULL, TO_ROOM );
    act( "Dazzling light flashes through your cannon as a loud humming fills the air.", ch, NULL, NULL, TO_CHAR );
    act( "Dazzling light flashes through $n's cannon as a loud humming fills the air.", ch, NULL, NULL, TO_ROOM );
    act( "You grin evilly saying '#yRCannon#n' as you point it right at $N!", ch, NULL, victim, TO_CHAR );
    act( "$n grins evilly saying '#yRCannon#n' as $e points it right at you!", ch, NULL, victim, TO_VICT );
    act( "$n grins evilly saying '#yRCannon#n' as $e points it right at $N!", ch, NULL, victim, TO_NOTVICT );
    act( "A sleek visor covers your eyes as you release a MASSIVE BLAST towards $N!!!", ch, NULL, victim, TO_CHAR );
    act( "A sleek visor covers $n's eyes as $e releases a MASSIVE BLAST towards you!!!", ch, NULL, victim, TO_VICT );
    act( "A sleek visor covers $n's eyes as $e releases a MASSIVE BLAST towards $N!!!", ch, NULL, victim, TO_NOTVICT );

    if ( !is_martial( victim ) )
       victim->wait += 18;

    act( "You scream in sheer pain as your body convulses from the HORRIFYING sets of explosions!", victim, NULL, NULL, TO_CHAR );
    act( "$n screams in sheer pain as $s body convulses from the HORRIFYING sets of explosions!", victim, NULL, NULL, TO_ROOM );

    ch->ele_attack = ATTACK_THUNDER;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BEAM;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 3000000, TRUE );
}

void do_executioner( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 65 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) && mysafe == 0 && !armageddon )
       {
          send_to_char("You can't use this technique beneath this holy sanctuary.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You say '#yReady!#n' as you tightly hold your trembling hand...", ch, NULL, NULL, TO_CHAR );
    act( "$n says '#yReady!#n' as $e tightly holds $s trembling hand...", ch, NULL, NULL, TO_ROOM );
    act( "Beautiful spirals of energy encircle you as you grin evilly.", ch, NULL, NULL, TO_CHAR );
    act( "Beautiful spirals of energy encircle $n as $e grins evilly.", ch, NULL, NULL, TO_ROOM );
    act( "You then scream '#yExecutioner!#n' as you release a colourful wave into the air.", ch, NULL, NULL, TO_CHAR );
    act( "$n then screams '#yExecutioner!#n' as $e releases a colourful wave into the air.", ch, NULL, NULL, TO_ROOM );
    act( "Soft jolts fizzes all over the beam as you stay focused on your target.", ch, NULL, NULL, TO_CHAR );
    act( "Soft jolts fizzes all over the beam as $n stays focused on your target.", ch, NULL, NULL, TO_ROOM );
    act( "You sneer '#eDie!#n' as you coldly look to the side...", ch, NULL, NULL, TO_CHAR );
    act( "$n sneers '#eDie!#n' as $e coldly looks to the side...", ch, NULL, NULL, TO_ROOM );
    act( "Suddenly a MASSIVE Scythe blade falls head onto the room SLICING everything in HALF!!!", ch, NULL, NULL, TO_CHAR );
    act( "Suddenly a MASSIVE Scythe blade falls head onto the room SLICING everything in HALF!!!", ch, NULL, NULL, TO_ROOM );
    act( "#wYou #ylower your hand as you #oclose #yyour eyes from the #Rbloody #echaos#y.#n", ch, NULL, NULL, TO_CHAR );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       if ( !IS_SET( vch->cmbt[5], SKL2_AMIND ) && number_percent() > 49 )
          {
             stop_fighting( vch, TRUE );
             vch->hit = -2000010;
             vch->position = POS_MORTAL;
          }
    }

    act( "#w$n #ylowers $s hand as $e #ocloses #y$s eyes from the #Rbloody #echaos#y.#n", ch, NULL, NULL, TO_ROOM );
}

void do_shock_blade( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA af;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 25 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Strike whom?\n\r", ch );
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
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
 
   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;
   SET_BIT( ch->arena, AFF2_POWER );

   act("You quickly take out your M.V.S unit as you activate its switches.",ch,NULL,NULL,TO_CHAR);
   act("$n quickly takes out $s M.V.S unit as $e activates its switches.",ch,NULL,NULL,TO_ROOM);
   act("Ready now, you scowl as you insert a cartridge into the device's slot.",ch,NULL,NULL,TO_CHAR);
   act("Ready now, $n scowls as $e inserts a cartridge into the device's slot.",ch,NULL,NULL,TO_ROOM);
   act("You then leap towards $N as the M.V.S unit starts pulsing with energy.",ch,NULL,victim,TO_CHAR);
   act("$n then leaps towards you as the M.V.S unit starts pulsing with energy.",ch,NULL,victim,TO_VICT);
   act("$n then leaps towards $N as the M.V.S unit starts pulsing with energy.",ch,NULL,victim,TO_NOTVICT);
   act("You draw back your weapon subtlely as you mutter '#yShock...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n draws back $s weapon subtlely as $e mutters '#yShock...#n'",ch,NULL,NULL,TO_ROOM);
   act("You then scream '#yBLADE!!!#n' as you drive an energy blade through $N's chest!!!",ch,NULL,victim,TO_CHAR);
   act("$n then screams '#yBLADE!!!#n' as $e drives an energy blade through your chest!!!",ch,NULL,victim,TO_VICT);
   act("$n then screams '#yBLADE!!!#n' as $e drives an energy blade through $N's chest!!!",ch,NULL,victim,TO_NOTVICT);

   switch( number_range( 1, 6 ) )
   {
      case 1: act( "#wYou cover your eyes in pain as you realize you can't see anymore!#n", victim, NULL, NULL, TO_CHAR );
              act( "#w$n #wcovers $s eyes in pain as $e realizes $e can't see anymore!#n", victim, NULL, NULL, TO_ROOM );
              victim->blind_timer = 0;
              SET_BIT( victim->more, MORE_BLIND );
              break;

      case 2: act( "#PYou place a hand on your throat as you realize your casting is affected!#n", victim, NULL, NULL, TO_CHAR );
              act( "#P$n #Pplaces a hand on $s throat as $e realizes $s casting is affected!#n", victim, NULL, NULL, TO_ROOM );
              SET_BIT( victim->more, MORE_HEX );
              break;

      case 3: act( "#yYou place a hand on your throat as you realize you can't speak!#n", victim, NULL, NULL, TO_CHAR );
              act( "#y$n #yplaces a hand on $s throat as $e realizes $e can't speak!#n", victim, NULL, NULL, TO_ROOM );
              SET_BIT( victim->more3, MORE3_SILENCE );
              break;

      case 4: act( "#CYou collapse onto your knees as you realize you can't move!#n", victim, NULL, NULL, TO_CHAR );
              act( "#C$n #Ccollapses onto $s knees as $e realizes $e can't move!#n", victim, NULL, NULL, TO_ROOM );
              SET_BIT( victim->more2, MORE2_NOLEGS );
              break;

      case 5: act( "#gYou collapse onto your knees as you realize you can't fight properly!#n", victim, NULL, NULL, TO_CHAR );
              act( "#g$n #gcollapses onto $s knees as $e realizes $e can't fight properly!#n", victim, NULL, NULL, TO_ROOM );
              SET_BIT( victim->more2, MORE2_NOARMS );
              SET_BIT( victim->more, MORE_DISEMPOWER );
              SET_BIT( victim->garou1, GAROU_PAWS );
              SET_BIT( victim->more, MORE_PINCH );

              if ( !is_affected( victim, skill_lookup("slow") ) )
                 {
                     af.type      = skill_lookup("slow");
                     af.duration  = 20;
                     af.location  = APPLY_NONE;
                     af.modifier  = -1;
                     af.bitvector = 0;
                     affect_to_char( victim, &af );
                 }

              break;

      case 6: act( "#eYou clutch your stomach as you realize you were struck a draining blow!#n", victim, NULL, NULL, TO_CHAR );
              act( "#e$n $eclutches $s stomach as $e realizes $e was struck a draining blow!#n", victim, NULL, NULL, TO_ROOM );
              SET_BIT( victim->monkstuff, MONK_DEATH );
              victim->ctimer[9] /= 2;
              break;
   }

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = UPPERCUT;
   hurt_person( ch, victim, 1500000, TRUE );
}

void do_lightning_fist( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 26 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You say '#yLets go!#n' as you tightly hold your trembling hand...", ch, NULL, NULL, TO_CHAR );
    act( "$n says '#yLets go!#n' as $e tightly holds $s trembling hand...", ch, NULL, NULL, TO_ROOM );
    act( "Your eyes start pusling with energy as sparks fizzing off your body.", ch, NULL, NULL, TO_CHAR );
    act( "$n's eyes start pusling with energy as sparks fizzing off $s body.", ch, NULL, NULL, TO_ROOM );
    act( "A manifest of blinding energy engulfs your hand as you strain your head away.", ch, NULL, NULL, TO_CHAR );
    act( "A manifest of blinding energy engulfs $n's hand as $e strains your head away.", ch, NULL, NULL, TO_ROOM );
    act( "Focusing on $N intently, you scream loudly as you SMASH your fist into the ground!!!", ch, NULL, victim, TO_CHAR );
    act( "Focusing on you intently, $n screams loudly as $e SMASHES $s fist into the ground!!!", ch, NULL, victim, TO_VICT );
    act( "Focusing on $N intently, $n screams loudly as $e SMASHES $s fist into the ground!!!", ch, NULL, victim, TO_NOTVICT );
    act( "The ground trembles as a burst of energy flashes across the vicinity.", ch, NULL, NULL, TO_CHAR );
    act( "The ground trembles as a burst of energy flashes across the vicinity.", ch, NULL, NULL, TO_ROOM );

    if ( !is_martial( victim ) )
       victim->wait += 12;

    act( "You scream in sheer pain as your body convulses from the deadly sparks of electricity!", victim, NULL, NULL, TO_CHAR );
    act( "$n screams in sheer pain as $s body convulses from the deadly sparks of electricity!", victim, NULL, NULL, TO_ROOM );

    ch->ele_attack = ATTACK_THUNDER;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = LOW;
    hurt_person( ch, victim, 1500000, TRUE );
}

void do_meteor_kick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 31 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You say '#yLets go!#n' as you leap in front of $N suddenly.", ch, NULL, victim, TO_CHAR );
    act( "$n says '#yLets go!#n' as $e leaps in front of you suddenly.", ch, NULL, victim, TO_VICT );
    act( "$n says '#yLets go!#n' as $e leaps in front of $N suddenly.", ch, NULL, victim, TO_NOTVICT );
    act( "You raise your foot in the air as it bursts into ferocious flames.", ch, NULL, NULL, TO_CHAR );
    act( "$n raises $s foot in the air as it bursts into ferocious flames.", ch, NULL, NULL, TO_ROOM );
    act( "Leaning back, you suddenly kick $N viciously in the guts!", ch, NULL, victim, TO_CHAR );
    act( "Leaning back, $n suddenly kicks you viciously in the guts!", ch, NULL, victim, TO_VICT );
    act( "Leaning back, $n suddenly kicks $N viciously in the guts!", ch, NULL, victim, TO_NOTVICT );
    act( "As $N clutches $S stomach in pain, you kick $M in savagely in the face!", ch, NULL, victim, TO_CHAR );
    act( "As you clutch your stomach in pain, $n kicks you in savagely in the face!", ch, NULL, victim, TO_VICT );
    act( "As $N clutches $S stomach in pain, $n kicks $M in savagely in the face!", ch, NULL, victim, TO_NOTVICT );
    act( "You suddenly connect your foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly connects $s foot into your knee as you were pushed by an unseen force!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly connects $s foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_NOTVICT );
    act( "Leaning back, you suddenly kick $N viciously in the guts!", ch, NULL, victim, TO_CHAR );
    act( "Leaning back, $n suddenly kicks you viciously in the guts!", ch, NULL, victim, TO_VICT );
    act( "Leaning back, $n suddenly kicks $N viciously in the guts!", ch, NULL, victim, TO_NOTVICT );
    act( "As $N clutches $S stomach in pain, you kick $M in savagely in the face!", ch, NULL, victim, TO_CHAR );
    act( "As you clutch your stomach in pain, $n kicks you in savagely in the face!", ch, NULL, victim, TO_VICT );
    act( "As $N clutches $S stomach in pain, $n kicks $M in savagely in the face!", ch, NULL, victim, TO_NOTVICT );
    act( "You suddenly connect your foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly connects $s foot into your knee as you were pushed by an unseen force!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly connects $s foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_NOTVICT );
    act( "Leaning back, you suddenly kick $N viciously in the guts!", ch, NULL, victim, TO_CHAR );
    act( "Leaning back, $n suddenly kicks you viciously in the guts!", ch, NULL, victim, TO_VICT );
    act( "Leaning back, $n suddenly kicks $N viciously in the guts!", ch, NULL, victim, TO_NOTVICT );
    act( "As $N clutches $S stomach in pain, you kick $M in savagely in the face!", ch, NULL, victim, TO_CHAR );
    act( "As you clutch your stomach in pain, $n kicks you in savagely in the face!", ch, NULL, victim, TO_VICT );
    act( "As $N clutches $S stomach in pain, $n kicks $M in savagely in the face!", ch, NULL, victim, TO_NOTVICT );
    act( "You suddenly connect your foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly connects $s foot into your knee as you were pushed by an unseen force!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly connects $s foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_NOTVICT );
    act( "Leaning back, you suddenly kick $N viciously in the guts!", ch, NULL, victim, TO_CHAR );
    act( "Leaning back, $n suddenly kicks you viciously in the guts!", ch, NULL, victim, TO_VICT );
    act( "Leaning back, $n suddenly kicks $N viciously in the guts!", ch, NULL, victim, TO_NOTVICT );
    act( "As $N clutches $S stomach in pain, you kick $M in savagely in the face!", ch, NULL, victim, TO_CHAR );
    act( "As you clutch your stomach in pain, $n kicks you in savagely in the face!", ch, NULL, victim, TO_VICT );
    act( "As $N clutches $S stomach in pain, $n kicks $M in savagely in the face!", ch, NULL, victim, TO_NOTVICT );
    act( "You suddenly connect your foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly connects $s foot into your knee as you were pushed by an unseen force!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly connects $s foot into $N's knee as $E was pushed by an unseen force!", ch, NULL, victim, TO_NOTVICT );

    ch->ele_attack = ATTACK_FIRE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 1500000, TRUE );

    if ( victim != NULL && victim->position > POS_STUNNED )
       {
          act( "#RThe flames around $N #wintensify #Ras a barrage of energy engulfs your foot.#n", ch, NULL, victim, TO_CHAR );
          act( "#RThe flames around you #wintensify #Ras a barrage of energy engulfs $n#R's foot.#n", ch, NULL, victim, TO_VICT );
          act( "#RThe flames around $N #wintensify #Ras a barrage of energy engulfs $n#R's foot.#n", ch, NULL, victim, TO_NOTVICT );
          act( "#RYou then scream as you drive your foot into $N#R's chest with ALL of your #wMIGHT#R!#n", ch, NULL, victim, TO_CHAR );
          act( "#R$n #Rthen screams as $e drives $s foot into your chest with ALL of $s #wMIGHT#R!#n", ch, NULL, victim, TO_VICT );
          act( "#R$n #Rthen screams as $e drives $s foot into $N#R's chest with ALL of $s #wMIGHT#R!#n", ch, NULL, victim, TO_NOTVICT );

          special_slam( victim, number_range( 0, 4 ) );
          stop_fighting( victim, TRUE ); 
       }
}

void do_divine_cannon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam = 3500000;

    if ( IS_NPC(ch) )
       return;    

    if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !can_do_skill( ch, 82 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You sneer evilly as your eye scanner starts blinking.", ch, NULL, NULL, TO_CHAR );
    act( "$n sneers evilly as $s eye scanner starts blinking.", ch, NULL, NULL, TO_ROOM );
    act( "You then point your MASSIVE cannon at $N as you analyse $S data!", ch, NULL, victim, TO_CHAR );
    act( "$n then points $s MASSIVE cannon at you as $e analyses your data!", ch, NULL, victim, TO_VICT );
    act( "$n then points $s MASSIVE cannon at $N as $e analyses $S data!", ch, NULL, victim, TO_NOTVICT );
    act( "A loud hum fills the air as your cannon starts sucking in vital energy.", ch, NULL, NULL, TO_CHAR );
    act( "A loud hum fills the air as $n's cannon starts sucking in vital energy.", ch, NULL, NULL, TO_ROOM );
    act( "You struggle to stablize your arm as a manifest of ferocious energy grows...", ch, NULL, NULL, TO_CHAR );
    act( "$n struggles to stablize $s arm as a manifest of ferocious energy grows...", ch, NULL, NULL, TO_ROOM );
    act( "You then say '#CDivine #yCannon#n' as you release a #w#zTRMEDOUS BLAST#n at $N!!!", ch, NULL, victim, TO_CHAR );
    act( "$n then says '#CDivine #yCannon#n' as $e releases a #w#zTRMEDOUS BLAST#n at you!!!", ch, NULL, victim, TO_VICT );
    act( "$n then says '#CDivine #yCannon#n' as $e releases a #w#zTRMEDOUS BLAST#n at $N!!!", ch, NULL, victim, TO_NOTVICT );

    if ( victim->blocking > 0 )
       {
          dam *= 2;
          victim->blocking = victim->block_meter = 0;
          act( "#eSuddenly a #yviolent #Rforce #erips your arms apart as you realize your #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_CHAR );
          act( "#eSuddenly a #yviolent #Rforce #erips $n#e's arms apart as $e realizes $s #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_ROOM );
          victim->noblock = UMAX( 1, victim->noblock );
       }

    act( "You scream in sheer pain as your body convulses from the HORRIFYING sets of explosions!", victim, NULL, NULL, TO_CHAR );
    act( "$n screams in sheer pain as $s body convulses from the HORRIFYING sets of explosions!", victim, NULL, NULL, TO_ROOM );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BEAM;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, dam, TRUE );
}

void do_tenho_blast( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam = 1500000;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 29 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act( "You slam your foot onto the ground as you tilt your hand forward.", ch, NULL, NULL, TO_CHAR );
    act( "$n slams $s foot onto the ground as $e tilts $s hand forward.", ch, NULL, NULL, TO_ROOM );
    act( "You scream '#yKou Ken!#n' as you push your palm mightily at $N!", ch, NULL, victim, TO_CHAR );
    act( "$n screams '#yKou Ken!#n' as $e pushes $s palm mightily at you!", ch, NULL, victim, TO_VICT );
    act( "$n screams '#yKou Ken!#n' as $e pushes $s palm mightily at $N!", ch, NULL, victim, TO_NOTVICT );
    act( "You suddenly go on your knees as a VIOLENT force of energy SLAMS into you!", victim, NULL, NULL, TO_CHAR );
    act( "$n suddenly goes on $s knees as a VIOLENT force of energy SLAMS into $m!", victim, NULL, NULL, TO_ROOM );

    if ( victim->blocking > 0 )
       {
          dam = 2000000;
          victim->blocking = victim->block_meter = 0;
          act( "#eSuddenly a #yviolent #Rforce #erips your arms apart as you realize your #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_CHAR );
          act( "#eSuddenly a #yviolent #Rforce #erips $n#e's arms apart as $e realizes $s #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_ROOM );
          victim->noblock = UMAX( 1, victim->noblock );
       }

    ch->ele_attack = ATTACK_LUNAR;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, dam, TRUE );
}

void do_ice_blast( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 32 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You bring your hand to your mouth as you blow some ice dust towards $N!", ch, NULL, victim, TO_CHAR );
    act( "$n brings $s hand to $s mouth as $e blows some ice dust towards you!", ch, NULL, victim, TO_VICT );
    act( "$n brings $s hand to $s mouth as $e blows some ice dust towards $N!", ch, NULL, victim, TO_NOTVICT );
    act( "As the chilly breath flies by you, you suddenly find yourself frozen!", victim, NULL, NULL, TO_CHAR );
    act( "As the chilly breath flies by $n, $e suddenly finds $mself frozen!", victim, NULL, NULL, TO_ROOM );
    act( "You then spread your hands apart as shards of light engulfs you...", ch, NULL, NULL, TO_CHAR );
    act( "$n then spreads $s hands apart as shards of light engulfs $m...", ch, NULL, NULL, TO_ROOM );
    act( "A ferocious wind zooms past you as it BLASTS through $N's icy tomb!!!", ch, NULL, victim, TO_CHAR );
    act( "A ferocious wind zooms past $n as it BLASTS through your icy tomb!!!", ch, NULL, victim, TO_VICT );
    act( "A ferocious wind zooms past $n as it BLASTS through $N's icy tomb!!!", ch, NULL, victim, TO_NOTVICT );
    act( "You are flung aside as the shattered ice rips through your body!", victim, NULL, NULL, TO_CHAR );
    act( "$n is flung aside as the shattered ice rips through $s body!", victim, NULL, NULL, TO_ROOM );

    if ( !is_martial( victim ) && !IS_SET( victim->more, MORE_FROZEN ) )
       {
          victim->freeze_timer = 1;
          SET_BIT( victim->more, MORE_FROZEN );
       }

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_ICE;
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = OVERHEAD;
    hurt_person( ch, victim, 1500000, TRUE );
}

void do_angel_wings( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_ANGEL ) )
       {
          if ( !can_do_skill( ch, 28 ) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ch->throw_timer > 0 )
             {
                stc("You are still recovering from the last one.\n\r",ch);
                return;
             }
       }
       else {
               if ( ch->pkill < 4 )
                  {
                     stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
                     return;
                  }

               if ( ch->ctimer[6] > 0 )
                  {
                     stc("You are still recovering from the last one.\n\r",ch);
                     return;
                  }
            }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
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
 
    if ( !IS_CLASS( ch, CLASS_ANGEL ) )
       {
          ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
          ch->throw_timer = 50;
       }
       else {
               ch->wait = 6;
               ch->ctimer[6] = 4;
            }

    act( "You concentrate silently as two beautiful wings burst from your back.", ch, NULL, NULL, TO_CHAR );
    act( "$n concentrates silently as two beautiful wings burst from $s back.", ch, NULL, NULL, TO_ROOM );
    act( "You then fly towards $N as your angel feathers fall to the floor.", ch, NULL, victim, TO_CHAR );
    act( "$n then flies towards you as $s angel feathers fall to the floor.", ch, NULL, victim, TO_VICT );
    act( "$n then flies towards $N as $s angel feathers fall to the floor.", ch, NULL, victim, TO_NOTVICT );
    act( "You swerve your hands stylishly as a spiral of light surrounds $N.", ch, NULL, victim, TO_CHAR );
    act( "$n swerves $s hands stylishly as a spiral of light surrounds you.", ch, NULL, victim, TO_VICT );
    act( "$n swerves $s hands stylishly as a spiral of light surrounds $N.", ch, NULL, victim, TO_NOTVICT );
    act( "You yell '#wAngel Wings!#n' as you slam your fists into $N's chest!!!", ch, NULL, victim, TO_CHAR );
    act( "$n yells '#wAngel Wings!#n' as $e slams $s fists into your chest!!!", ch, NULL, victim, TO_VICT );
    act( "$n yells '#wAngel Wings!#n' as $e slams $s fists into $N's chest!!!", ch, NULL, victim, TO_NOTVICT );
    act( "As you stagger back into the energy field, a torrent of feathers rip through you!", victim, NULL, NULL, TO_CHAR );
    act( "As $n staggers back into the energy field, a torrent of feathers rip through $m!", victim, NULL, NULL, TO_ROOM );

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_HOLY;
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = UPPERCUT;

    if ( IS_CLASS( ch, CLASS_ANGEL ) )
       hurt_person( ch, victim, 500000, TRUE );
       else hurt_person( ch, victim, 1500000, TRUE );

    if ( victim != NULL && victim->position > POS_STUNNED )
       {
          act( "#yYou #wthen collapse onto the ground totally derived of energy.#n", victim, NULL, NULL, TO_CHAR );
          act( "#y$n #wthen collapses onto the ground totally derived of energy.#n", victim, NULL, NULL, TO_ROOM );
          stop_fighting( victim, TRUE ); 
          victim->position = POS_STUNNED;
       }
}

void do_roulette( CHAR_DATA *ch, char *argument )
{
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int count = 20, person = 0, num;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 27 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyan_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->saiyan_timer = 40;
   ch->wait = 6;

   act("You go on your knees silently as you pray for forgiveness...",ch,NULL,NULL,TO_CHAR);
   act("$n goes on $s knees silently as $e prays for forgiveness...",ch,NULL,NULL,TO_ROOM);
   act("#eSuddenly, a boney finger points at you before fading away.#n",ch,NULL,NULL,TO_CHAR);
   act("#eSuddenly, a boney finger points at $n before fading away.#n",ch,NULL,NULL,TO_ROOM);

   if ( ch->fighting != NULL )
      {
         count -= 2;
         act("Suddenly, a boney finger points at you before fading away.",ch->fighting,NULL,NULL,TO_CHAR);
         act("Suddenly, a boney finger points at $n before fading away.",ch->fighting,NULL,NULL,TO_ROOM);
      }
      else count--;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->character != NULL )
         person++;  
   }

   for ( count = count; count > 0; count-- )
   {
      num = number_range( 1, person );

      for ( d = descriptor_list; d != NULL; d = d->next )
      {
         if ( d->character != NULL )
            { 
               if ( --num == 0 )
                  {
                     victim = d->character;

                     if ( count > 1 )
                        {                            
                            act("#sSuddenly, a boney finger points at you before fading away.#n",victim,NULL,NULL,TO_CHAR);
                            act("#sSuddenly, a boney finger points at $n before fading away.#n",victim,NULL,NULL,TO_ROOM);
                        }
                  }
            }
      }
   }

   if ( victim == NULL || IS_IMMORTAL( victim ) )
      return;

   if ( number_percent() < 65 )
      victim = ch;

   act( "A loud cackle fills the air as the room explodes into demonic flames.", victim, NULL, NULL, TO_CHAR );
   act( "A loud cackle fills the air as the room explodes into demonic flames.", victim, NULL, NULL, TO_ROOM );
   act( "A pair of flaming eyes burn your soul as Death fades into existence.", victim, NULL, NULL, TO_CHAR );
   act( "A pair of flaming eyes burn $n's soul as Death fades into existence.", victim, NULL, NULL, TO_ROOM );
   act( "Death raise his hand above you as his Scythe appears in a burst of flames.", victim, NULL, NULL, TO_CHAR );
   act( "Death raise his hand above $n as his Scythe appears in a burst of flames.", victim, NULL, NULL, TO_ROOM );
   act( "Death cackles again as he leaps into the air with the deadly Scythe twirling...", victim, NULL, NULL, TO_CHAR );
   act( "Death cackles again as he leaps into the air with the deadly Scythe twirling...", victim, NULL, NULL, TO_ROOM );

   if ( IS_SET( victim->cmbt[5], SKL2_AMIND ) )
      {
         act( "At the LAST second, you suddenly duck to the ground dodging Death's fatal blow!", victim, NULL, NULL, TO_CHAR );
         act( "At the LAST second, $n suddenly ducks to the ground dodging Death's fatal blow!", victim, NULL, NULL, TO_ROOM );
      }
      else {
              stop_fighting( victim, TRUE );
              victim->hit = -2000010;
              victim->position = POS_MORTAL;
              act( "#eThe #CLAST #ething you see is your body being #wRIPPED #ein half!!!#n", victim, NULL, NULL, TO_CHAR );
              act( "#eThe #Cnext #ething you see is $n#e's body being #wRIPPED #ein half!!!#n", victim, NULL, NULL, TO_ROOM );
           }

   act( "Death raise his hand one more time before returning to the UnderWorld.", victim, NULL, NULL, TO_CHAR );
   act( "Death raise his hand one more time before returning to the UnderWorld.", victim, NULL, NULL, TO_ROOM );
}

void perform_arc( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act( "You scream loudly as you slash your arm violently in a deadly arc!", ch, NULL, NULL, TO_CHAR );
   act( "$n screams loudly as $e slashes $s arm violently in a deadly arc!", ch, NULL, NULL, TO_ROOM );
   act( "A flash of light nullifies $N's attack as your face flushes with intensity!", ch, NULL, victim, TO_CHAR );
   act( "A flash of light nullifies your attack as $n's face flushes with intensity!", ch, NULL, victim, TO_VICT );
   act( "A flash of light nullifies $N's attack as $n's face flushes with intensity!", ch, NULL, victim, TO_NOTVICT );
   act( "You snarl while clutching your stomach as $N is suddenly ripped apart!", ch, NULL, victim, TO_CHAR );
   act( "$n snarls while clutching $s stomach as you are suddenly ripped apart!", ch, NULL, victim, TO_VICT );
   act( "$n snarls while clutching $s stomach as $N is suddenly ripped apart!", ch, NULL, victim, TO_NOTVICT );

   ch->ninjastuff = 0;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_sonic_arc( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 21 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->blocking < 0 )
      {
         stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
         return;
      }

   if ( ch->aragami_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->aragami_timer = 40;
   ch->wait = 6;

   ch->blocking = 12;
   ch->ninjastuff = 90;
   SET_BIT( ch->arena, AFF2_AUTOGUARD );
   act("#CYou #wcautiously wait for your #Pfoe #wto execute a #ytechnique #won you.#n",ch,NULL,NULL,TO_CHAR);
   act("#C$n #wlooks like $e is cautiously waiting for #ysomething#w.#n",ch,NULL,NULL,TO_ROOM);
}

void do_anti_cross( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 42 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->saiyan_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) && mysafe == 0 && !armageddon )
       {
          send_to_char("You can't use this curse beneath this holy sanctuary.\n\r",ch);
          return;
       }

    ch->wait = 6;
    ch->saiyan_timer = 40;

    act("You start cackling with glee as you tilt your hands together.",ch,NULL,NULL,TO_CHAR);
    act("$n starts cackling with glee as $e tilt $s hands together.",ch,NULL,NULL,TO_ROOM);
    act("You then release a demonic pulse of energy as it slowly floats into the air.",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a demonic pulse of energy as it slowly floats into the air.",ch,NULL,NULL,TO_ROOM);
    act("Sparks of dark lightning fizzes from the energy as it grows bigger.",ch,NULL,NULL,TO_CHAR);
    act("Sparks of dark lightning fizzes from the energy as it grows bigger.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly it transforms into a vortex as a massive cross appears.",ch,NULL,NULL,TO_CHAR);
    act("Suddenly it transforms into a vortex as a massive cross appears.",ch,NULL,NULL,TO_ROOM);
    act("Looking closer, you see a lifeless woman cruxified on the holy cross.",ch,NULL,NULL,TO_CHAR);
    act("Looking closer, you see a lifeless woman cruxified on the holy cross.",ch,NULL,NULL,TO_ROOM);
    act("A grin then appears on her face as her flesh drips off her decaying body.",ch,NULL,NULL,TO_CHAR);
    act("A grin then appears on her face as her flesh drips off her decaying body.",ch,NULL,NULL,TO_ROOM);
    act("A loud screech fills the air as the unbearable light flashes through the room!!!",ch,NULL,NULL,TO_CHAR);
    act("A loud screech fills the air as the unbearable light flashes through the room!!!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_NPC( vch ) && IS_SET( vch->extra, EXTRA_ZOMBIE ) ) 
          continue;

       if ( !IS_NPC( vch ) && ( get_age( vch ) - 17 ) < 2 )
          continue;

       if ( vch != ch && vch->attacker != NULL && vch->attacker != ch )
          continue;

       if ( !IS_NPC( vch ) && !( ch == vch && IS_CLASS( ch, CLASS_MASTER ) ) )
          add_grove( vch, NULL, -150 );

       vch->hit = UMAX( 1, vch->hit - 2000000 );
    }
}

void perform_aragami( CHAR_DATA *ch, CHAR_DATA *victim )
{
   if ( IS_NPC( ch ) && ch->pIndexData->vnum != 564 )
      return;

   act("You yell '#yEia Ziek!#n' as you swing wildly across $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across your chest.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across $N's chest.",ch,NULL,victim,TO_NOTVICT);
   act("You then stagger $N with a burning uppercut up $S chin.",ch,NULL,victim,TO_CHAR);
   act("$n then staggers you with a burning uppercut up your chin.",ch,NULL,victim,TO_VICT);
   act("$n then staggers $N with a burning uppercut up $S chin.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yKen Za!#n' as you DrIvE your elbow down $N's back.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yKen Za!#n' as $e DrIvEs $s elbow down your back.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yKen Za!#n' as $e DrIvEs $s elbow down $N's back.",ch,NULL,victim,TO_NOTVICT);
   act("As $N bounces off the ground, you quickly grab $M by the neck.",ch,NULL,victim,TO_CHAR);
   act("As you bounce off the ground, $n quickly grabs you by the neck.",ch,NULL,victim,TO_VICT);
   act("As $N bounces off the ground, $n quickly grabs $M by the neck.",ch,NULL,victim,TO_NOTVICT);
   act("You clench your fist as $N is engulfed by a massive explosion!",ch,NULL,victim,TO_CHAR);
   act("$n clenches $s fist as you are engulfed by a massive explosion!",ch,NULL,victim,TO_VICT);
   act("$n clenches $s fist as $N is engulfed by a massive explosion!",ch,NULL,victim,TO_NOTVICT);
   act("You quickly spin around your body, giving $N a rapid roundhouse kick!",ch,NULL,victim,TO_CHAR);
   act("$n quickly spins around $s body, giving you a rapid roundhouse kick!",ch,NULL,victim,TO_VICT);
   act("$n quickly spins around $s body, giving $N a rapid roundhouse kick!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yOia Yah!#n' as you open your hands, DrIvInG the flames through $N.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yOia Yah!#n' as $e opens $s hands, DrIvInG the flames through you.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yOia Yah!#n' as $e opens $s hands, DrIvInG the flames through $N.",ch,NULL,victim,TO_NOTVICT);

   ch->ninjastuff = 0;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_focus = UPPERCUT;
   ch->attack_var = BLAST;

   if ( !IS_NPC( ch ) )
      hurt_person( ch, victim, 1000000, TRUE );
      else if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
              hurt_person( ch, victim, 1000000, TRUE );
              else hurt_person( ch, victim, 500000, TRUE );
}

void do_aragami( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 43 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->blocking < 0 )
      {
         stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
         return;
      }

   if ( ch->aragami_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->aragami_timer = 40;
   ch->wait = 6;

   ch->blocking = 12;
   ch->ninjastuff = 100;
   SET_BIT( ch->arena, AFF2_AUTOGUARD );
   send_to_char("#CYour #wflex your fingers as you #Pprepare #Cyourself for #yany #Cincoming attack.#n\n\r",ch);
   act("#C$n #wflexes $s fingers as $e #Pprepares #C$mself for #yany #Cincoming attack.#n",ch,NULL,NULL,TO_ROOM);
}

void perform_zanretsuken( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt your palm forward, blocking $N's attack as you focus on your chi.",ch,NULL,victim,TO_CHAR);
   act("$n tilts $s palms forward, blocking your attack as $e focuses on $s chi.",ch,NULL,victim,TO_VICT);
   act("$n tilts $s palms forward, blocking $N's attack as $e focuses on $s chi.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yZanretsu Ken!#n' as you punch $N viciously in the chest!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yZanretsu Ken!#n' as $e punches you viciously in the chest!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yZanretsu Ken!#n' as $e punches $N viciously in the chest!",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("You then send $N across the room with a swift jab to the guts!",ch,NULL,victim,TO_CHAR);
   act("$n then sends you across the room with a swift jab to the guts!",ch,NULL,victim,TO_VICT);
   act("$n then sends $N across the room with a swift jab to the guts!",ch,NULL,victim,TO_NOTVICT);

   special_slam( victim, number_range( 0, 3 ) );
   ch->ninjastuff = 0;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = HIT;

   hurt_person( ch, victim, 1500000, TRUE );
}

void do_zanretsuken( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 74 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->blocking < 0 )
      {
         stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
         return;
      }

   if ( ch->aragami_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->aragami_timer = 40;
   ch->wait = 6;

   ch->blocking = 12;
   ch->ninjastuff = 110;
   SET_BIT( ch->arena, AFF2_AUTOGUARD );
   send_to_char("#CYour #wflex your fingers as you #Pprepare #Cyourself for #yany #Cincoming attack.#n\n\r",ch);
   act("#C$n #wflexes $s fingers as $e #Pprepares #C$mself for #yany #Cincoming attack.#n",ch,NULL,NULL,TO_ROOM);
}

void do_anti_light( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 44 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET(ch->newbits, NEW_ALIGHT) ) 
      {
         send_to_char("#yYou #ebanish the #Pshroud #eof anti#wlight#e.#n\n\r", ch );
         REMOVE_BIT(ch->newbits, NEW_ALIGHT);

         if ( IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) )
            REMOVE_BIT( ch->in_room->area->bits, ROOM_ANTI_LIGHT );

         sta("#eThe #Pshroud #edampening the area is suddenly #wlifted#e.#n\n\r", ch->in_room->area );
         return;
	}

   if ( IS_AFFECTED( ch, AFF_MANA ) )
      {
	   send_to_char("#eYou have too much #Pmana #eto utilize the anti#wlight#e.#n\n\r",ch );
	   return;
      }

   if ( ch->mana < 5000000 )
      {
	   send_to_char("#eYou don't have enough #Pmana #eto summon the anti#wlight#e.#n\n\r",ch );
	   return;
      }

   send_to_char("#yYou #esummon the #Pshroud #eof anti#wlight#e.#n\n\r", ch );
   sta("#eA #Pshroud #esuddenly dampens the entire area.#n\n\r", ch->in_room->area );
   SET_BIT( ch->newbits, NEW_ALIGHT );

   if ( !IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) )
      SET_BIT( ch->in_room->area->bits, ROOM_ANTI_LIGHT );
}

void do_thunder_blade( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !can_do_skill( ch, 45 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );

    act("You cackle loudly as you pull out a menacing blade of lightning.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles loudly as $e pulls out a menacing blade of lightning.",ch,NULL,NULL,TO_ROOM);
    act("You swerve the blade around as it draws in the surrounding energy...",ch,NULL,NULL,TO_CHAR);
    act("$n swerves the blade around as it draws in the surrounding energy...",ch,NULL,NULL,TO_ROOM);
    act("You scream '#yThunder Blade!#n' as you twirl the sword over your head.",ch,NULL,NULL,TO_CHAR);
    act("$n screams '#yThunder Blade!#n' as $e twirls the sword over $s head.",ch,NULL,NULL,TO_ROOM);
    act("A powerful beam of energy follows the sword as you DRIVE it into the ground!!!",ch,NULL,NULL,TO_CHAR);
    act("A powerful beam of energy follows the sword as $n DRIVES it into the ground!!!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       if ( !is_martial( vch ) )
          vch->wait += 12;

       act("Your body fizzes and spasms as you are jolted VIOLENTLY into the air!",vch,NULL,NULL,TO_CHAR);
       act("$n's body fizzes and spasms as $e is jolted VIOLENTLY into the air!",vch,NULL,NULL,TO_ROOM);

       ch->ele_attack = ATTACK_THUNDER;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BEAM;
       ch->attack_focus = OVERHEAD;
       hurt_person( ch, vch, 2000000, TRUE );
    }
}

void do_omega_blade( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *victim_next;

   if ( !can_do_skill( ch, 48 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;
   SET_BIT( ch->arena, AFF2_POWER );

   act("You smirk evilly as a replica of the feared Omega Blade manifests into your hand.",ch,NULL,NULL,TO_CHAR);
   act("$n smirks evilly as a replica of the feared Omega Blade manifests into $s hand.",ch,NULL,NULL,TO_ROOM);
   act("You then leap into the air as you twirl the blade viciously.",ch,NULL,NULL,TO_CHAR);
   act("$n then leaps into the air as $e twirls the blade viciously.",ch,NULL,NULL,TO_ROOM);
   act("Shards of dark light radiate off your sword as you say '#eThe End is #wNow#e...#n'",ch,NULL,NULL,TO_CHAR);
   act("Shards of dark light radiate off the sword as $n says '#eThe End is #wNow#e...#n'",ch,NULL,NULL,TO_ROOM);
   act("The room blackens as your shadowy form slashes the ground MIGHTILY!!!",ch,NULL,NULL,TO_CHAR);
   act("The room blackens as a shadowy form slashes the ground MIGHTILY!!!",ch,NULL,NULL,TO_ROOM);
   act("A ferocious ATOMIC bomb BLINDS everyone as dark flames disrupt the vicinity!",ch,NULL,NULL,TO_CHAR);
   act("A ferocious ATOMIC bomb BLINDS everyone as dark flames disrupt the vicinity!",ch,NULL,NULL,TO_ROOM);
      
   for ( victim = ch->in_room->people; victim != NULL; victim = victim_next )
   {
      victim_next = victim->next_in_room;

      if ( is_safe( ch, victim ) )
         continue;

      act( "#eThe #Pevil energy #edisrupt your flesh as you are RiPpEd away!!!#n", victim, NULL, NULL, TO_CHAR );
      act( "#eThe #Pevil energy #edisrupt $n#e's flesh as $e is RiPpEd away!!!#n", victim, NULL, NULL, TO_ROOM );

      special_slam( victim, number_range( 0, 3 ) );
      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = BLAST;
      ch->attack_focus = LOW;
      hurt_person( ch, victim, 2000000, TRUE );  
   }
}

void do_rune_blade( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam = 2000000;
   bool pre_room;
   int direction = number_range( 0, 3 );
   int count = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 49 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Strike whom?\n\r", ch );
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
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
 
   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;
   SET_BIT( ch->arena, AFF2_POWER );

   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You draw a magical symbol with your hand in the air as your eyes blazes.", ch, NULL, NULL, TO_CHAR );
   act( "$n draws a magical symbol with $s hand in the air as $s eyes blazes.", ch, NULL, NULL, TO_ROOM );
   act( "The symbol suddenly erupts into flames as you unsheath a deadly sword.", ch, NULL, NULL, TO_CHAR );
   act( "The symbol suddenly erupts into flames as $n unsheathes a deadly sword.", ch, NULL, NULL, TO_ROOM );
   act( "You push the symbol into $N's chest as you mutter '#RRune Blade...#n'", ch, NULL, victim, TO_CHAR );
   act( "$n pushes the symbol into your chest as $e mutters '#RRune Blade...#n'", ch, NULL, victim, TO_VICT );
   act( "$n pushes the symbol into $N's chest as $e mutters '#RRune Blade...#n'", ch, NULL, victim, TO_NOTVICT );
   act( "You rip your sword across $N's chest as the fiery symbol sears it!", ch, NULL, victim, TO_CHAR );
   act( "$n rips $s sword across your chest as the fiery symbol sears it!", ch, NULL, victim, TO_VICT );
   act( "$n rips $s sword across $N's chest as the fiery symbol sears it!", ch, NULL, victim, TO_NOTVICT );
   act( "You then stagger $N with a deadly flaming upperslash!", ch, NULL, victim, TO_CHAR );
   act( "$n then staggers you with a deadly flaming upperslash!", ch, NULL, victim, TO_VICT );
   act( "$n then staggers $N with a deadly flaming upperslash!", ch, NULL, victim, TO_NOTVICT );
   act( "You swagger onto your knees saying '#yEat This!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n swaggers onto $s knees saying '#yEat This!#n'", ch, NULL, NULL, TO_ROOM );
   act( "Time seems to slow down as you draw back your charging blade...", ch, NULL, NULL, TO_CHAR );
   act( "Time seems to slow down as $n draws back $s charging blade...", ch, NULL, NULL, TO_ROOM );
   act( "You scream loudly as you DRIVE the rune blade into $N's chest!!!", ch, NULL, victim, TO_CHAR );
   act( "$n screams loudly as $e DRIVES the rune blade into your chest!!!", ch, NULL, victim, TO_VICT );
   act( "$n screams loudly as $e DRIVES the rune blade into $N's chest!!!", ch, NULL, victim, TO_NOTVICT );

   do
   {
      pre_room = special_slam( victim, direction );
      count++;
   }
   while ( count < 3 && pre_room != TRUE );

   if ( !pre_room )
      dam += 500000;

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_lunar_blade( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) && !can_do_skill( ch, 46 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Strike whom?\n\r", ch );
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
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
      {
         ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
         ch->throw_timer = 50;
      }

   SET_BIT( ch->arena, AFF2_POWER );

   act("You quickly take out your M.V.S unit as you activate its switches.",ch,NULL,NULL,TO_CHAR);
   act("$n quickly takes out $s M.V.S unit as $e activates its switches.",ch,NULL,NULL,TO_ROOM);
   act("Ready now, you scowl as twin blades expand from the device's slot.",ch,NULL,NULL,TO_CHAR);
   act("Ready now, $n scowls as twin blades expand from the device's slot.",ch,NULL,NULL,TO_ROOM);
   act("You then leap towards $N as the M.V.S unit starts pulsing with energy.",ch,NULL,victim,TO_CHAR);
   act("$n then leaps towards you as the M.V.S unit starts pulsing with energy.",ch,NULL,victim,TO_VICT);
   act("$n then leaps towards $N as the M.V.S unit starts pulsing with energy.",ch,NULL,victim,TO_NOTVICT);
   act("You draw back your weapon subtlely as you mutter '#ythe moon...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n draws back $s weapon subtlely as $e mutters '#ythe moon...#n'",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yLunar Blade!!!#n' as you slam the M.V.S unit across $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n screams '#yLunar Blade!!!#n' as $e slams the M.V.S unit across your body!",ch,NULL,victim,TO_VICT);
   act("$n screams '#yLunar Blade!!!#n' as $e slams the M.V.S unit across $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("Mystical letters slice through $N's body as you wrack $M with an uppercut!",ch,NULL,victim,TO_CHAR);
   act("Mystical letters slice through your body as $n wracks you with an uppercut!",ch,NULL,victim,TO_VICT);
   act("Mystical letters slice through $N's body as $n wracks $M with an uppercut!",ch,NULL,victim,TO_NOTVICT);
   act("A massive explosion fills the room as more letters slashes through you.",victim,NULL,NULL,TO_CHAR);
   act("A massive explosion fills the room as more letters slashes through $n.",victim,NULL,NULL,TO_ROOM);
   act("You then DRIVE the M.V.S. unit down $N's body as light pierces through $M!!!",ch,NULL,victim,TO_CHAR);
   act("$n then DRIVES the M.V.S. unit down your body as light pierces through you!!!",ch,NULL,victim,TO_VICT);
   act("$n then DRIVES the M.V.S. unit down $N's body as light pierces through $M!!!",ch,NULL,victim,TO_NOTVICT);

   ch->ele_attack = ATTACK_LUNAR;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;

   if ( IS_NPC( ch ) )
      hurt_person( ch, victim, 4000000, TRUE );
      else if ( ch->pcdata->deity == DEI_LUNA )
              hurt_person( ch, victim, 3000000, TRUE );
              else hurt_person( ch, victim, 2000000, TRUE );

   act("#yThe letters form into a haunting '#oLuna#n' #yas a #wblinding #ylight fills the room...#n",ch,NULL,NULL,TO_CHAR);
   act("#yThe letters form into a haunting '#oLuna#n' #yas a #wblinding #ylight fills the room...#n",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || vch == victim || is_safe( ch, vch ) )
         continue;

      if ( IS_NPC( ch ) )
         {
            ch->ele_attack = ATTACK_LUNAR;
            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_var = SOUL;
            ch->attack_focus = UPPERCUT;
            hurt_person( ch, vch, 4000000, TRUE );

            continue;
         }

      if ( !IS_CLASS( vch, CLASS_WISP ) )
         vch->wait += 36;

      stop_fighting( vch, TRUE );
      vch->position = POS_SLEEPING;
   }
}

void do_true_sacrifice( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   char buf [MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   int dam = 1500000;

   if ( IS_NPC( ch ) )
      return;

   if ( !can_do_skill( ch, 50 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyan_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = 6;
   ch->saiyan_timer = 40;
   SET_BIT( ch->arena, AFF2_POWER );

   act( "#yYou #wsuddenly twirl your sword in the air as dazzling light sparkles off it.#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #wsuddenly twirls $s sword in the air as dazzling light sparkles off it.#n", ch, NULL, NULL, TO_ROOM );
   act( "#yYou #wcackle '#yHeathen... may God forgive you#n' #was you RAM the sword into your guts!#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #wcackles '#yHeathen... may God forgive you#n' #was $e RAMS the sword into $s guts!#n", ch, NULL, NULL, TO_ROOM );
   act( "#yYou #wfall onto your knees as shards of very intense #ylight #Cblast #wfrom your body.#n", ch, NULL, NULL, TO_CHAR );
   act( "#y$n #wfalls onto $s knees as shards of very intense #ylight #Cblast #wfrom $s body.#n", ch, NULL, NULL, TO_ROOM );
   act( "#RBlood #etrails #wdown #yyour #wlips... leaving a grin on your face as the #ylight #wengulfs all...#n", ch, NULL, NULL, TO_CHAR );
   act( "#RBlood #etrails #wdown #y$n#w's lips... leaving a grin on $s face as the #ylight #wengulfs all...#n", ch, NULL, NULL, TO_ROOM );

   if ( nogang == 0 )
      {
         for ( d = descriptor_list; d != NULL; d = d->next )
         { 
            if ( d->character != NULL && d->character != ch && ( d->character->pcdata->kingdom
                 == ch->pcdata->kingdom || is_same_group( ch, d->character ) ) )
               {
                  act("#yYou #wfeel totally #Crefresh #was the soothing #ylight #wbathe your tired body.#n",d->character,NULL,NULL,TO_CHAR);
                  act("#y$n #wfeels totally #Crefresh #was the soothing #ylight #wbathe $s tired body.#n",d->character,NULL,NULL,TO_ROOM);
                  vch->hit = UMIN( d->character->max_hit, d->character->hit + 3000000 );
                  update_pos(d->character);
              }
         }
      }

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch )
         continue;

      if ( is_safe(ch, vch) )
         continue;

      ch->ele_attack = ATTACK_HOLY;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, dam, TRUE );
   }

   stop_fighting( ch, TRUE );

   ch->hit = -2000010;
   ch->position = POS_MORTAL;
}

void do_koho( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 53 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Strike whom?\n\r", ch );
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
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
 
   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;

   act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);

   act("You mutter '#CKoho...#n' as you punch $N ViCiOuSlY in the stomach, encasing $M in ice.",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#CKoho...#n' as $e punches you ViCiOuSlY in the stomach, encasing you in ice.",ch,NULL,victim,TO_VICT);
   act("$n mutters '#CKoho...#n' as $e punches $N ViCiOuSlY in the stomach, encasing $M in ice.",ch,NULL,victim,TO_NOTVICT);

   act("You suddenly smash your palms into $N making the ice around $M grow bigger.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly smashes $s palms into you making the ice around you grow bigger.",ch,NULL,victim,TO_VICT);
   act("$n suddenly smashes $s palms into $N making the ice around $M grow bigger.",ch,NULL,victim,TO_NOTVICT);

   act("You raise your palm upwards $N's body as it explodes with ferocious energy.",ch,NULL,victim,TO_CHAR);
   act("$n raise $s palm upwards your body as it explodes with ferocious energy.",ch,NULL,victim,TO_VICT);
   act("$n raise $s palm upwards $N's body as it explodes with ferocious energy.",ch,NULL,victim,TO_NOTVICT);

   act("You close your eyes suddenly as you push your right palm in front of you.",ch,NULL,NULL,TO_CHAR);
   act("$n closes $s eyes suddenly as $e pushes $s right palm in front of $m.",ch,NULL,NULL,TO_ROOM);

   act("A mass of energy begins to manifest in your hand as you push it towards $N.",ch,NULL,victim,TO_CHAR);
   act("A mass of energy begins to manifest in $n's hand as $e pushes it towards you.",ch,NULL,victim,TO_VICT);
   act("A mass of energy begins to manifest in $n's hand as $e pushes it towards $N.",ch,NULL,victim,TO_NOTVICT);

   act("You release the mass of energy as it ShAtTeRs the IcE encasing $N, ripping $S body apart!!!",ch,NULL,victim,TO_CHAR);
   act("$n releases the mass of energy as it ShAtTeRs the IcE encasing you, ripping your body apart!!!",ch,NULL,victim,TO_VICT);
   act("$n releases the mass of energy as it ShAtTeRs the IcE encasing $N, ripping $S body apart!!!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_SET( victim->more, MORE_FROZEN ) )
      {
         SET_BIT( victim->more, MORE_FROZEN );
         victim->freeze_timer = 1;
      }

   SET_BIT( ch->arena, AFF2_POWER );
   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 2000000, TRUE );
}

void do_entangle( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 52 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Strike whom?\n\r", ch );
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
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
 
   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;

   SET_BIT( victim->more2, MORE2_NOLEGS );
   SET_BIT( victim->more2, MORE2_NOARMS );

   act("You wrap a shimmering whip across $N's legs dragging $M down!",ch,NULL,victim,TO_CHAR);  
   act("$n wraps a shimmering whip across your legs dragging you down!",ch,NULL,victim,TO_VICT);  
   act("$n wraps a shimmering whip across $N's legs dragging $M down!",ch,NULL,victim,TO_NOTVICT);  
   act("You then twirl yourself into the air as you throw energy nets at $N!",ch,NULL,victim,TO_CHAR);  
   act("$n then twirls $mself into the air as $e throws energy nets at you!",ch,NULL,victim,TO_VICT);  
   act("$n then twirls $mself into the air as $e throws energy nets at $N!",ch,NULL,victim,TO_NOTVICT);  
   act("As you land on your feet, you wrap a grappling hook across $N's neck.",ch,NULL,victim,TO_CHAR);  
   act("As $n lands on $s feet, $e wraps a grappling hook across your neck.",ch,NULL,victim,TO_VICT);  
   act("As $n lands on $s feet, $e wraps a grappling hook across $N's neck.",ch,NULL,victim,TO_NOTVICT);  
   act("You then violently pull $N towards you as you JAB $M HARD in the guts!",ch,NULL,victim,TO_CHAR);  
   act("$n then violently pulls you towards $m as $e JABS you HARD in the guts!",ch,NULL,victim,TO_VICT);  
   act("$n then violently pulls $N towards $m as $e JABS $M HARD in the guts!",ch,NULL,victim,TO_NOTVICT);  
   act("Totally entangled, you collapse onto the ground in sheer pain.",victim,NULL,NULL,TO_CHAR);  
   act("Totally entangled, $n collapses onto the ground in sheer pain.",victim,NULL,NULL,TO_ROOM);  

   SET_BIT( ch->arena, AFF2_POWER );
   ch->ele_attack = ATTACK_EARTH;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 2000000, TRUE );

   if ( victim != NULL && victim->position > POS_STUNNED )
      {
         stop_fighting( victim, TRUE ); 
         victim->position = POS_STUNNED;
      }
}

void do_lightningblade( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 1 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "Your blade starts sparking of electricity as you dash towards $N!", ch, NULL, victim, TO_CHAR );
    act( "$n's blade starts sparking of electricity as $e dashes towards you!", ch, NULL, victim, TO_VICT );
    act( "$n's blade starts sparking of electricity as $e dashes towards $N!", ch, NULL, victim, TO_NOTVICT );

    if ( number_percent() > 50 - devil_luck( ch, victim, 5 ) )
       {
          act("You scream '#ySUPER Lightning Blade!!#n' as you RAM your sword straight through $N's guts!", ch, NULL, victim, TO_CHAR );
          act("$n screams '#ySUPER Lightning Blade!!#n' as $e RAMS $s sword straight through your guts!", ch, NULL, victim, TO_VICT );
          act("$n screams '#ySUPER Lightning Blade!!#n' as $e RAMS $s sword straight through $N's guts!", ch, NULL, victim, TO_NOTVICT );

          if ( !is_martial( victim ) )
             victim->wait += 12;
       }
       else {
               act("You scream '#yLightning Blade!!#n' as you RAM your sword straight through $N's guts!", ch, NULL, victim, TO_CHAR );
               act("$n screams '#yLightning Blade!!#n' as $e RAMS $s sword straight through your guts!", ch, NULL, victim, TO_VICT );
               act("$n screams '#yLightning Blade!!#n' as $e RAMS $s sword straight through $N's guts!", ch, NULL, victim, TO_NOTVICT );
            }

    act( "You scream in sheer pain as your body convulses from the deadly sparks of lightning!", victim, NULL, NULL, TO_CHAR );
    act( "$n screams in sheer pain as $s body convulses from the deadly sparks of lightning!", victim, NULL, NULL, TO_ROOM );

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_THUNDER;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_fire_dash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 7 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
    act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
    act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
    act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
    act("You say, '#yFire Dash!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yFire Dash!#n'",ch,NULL,NULL,TO_ROOM);
    act("As the explosive energy dances on your hand, you grab hold of your blade and FlAsH thrOuGh $N!!!",ch,NULL,victim,TO_CHAR);
    act("As the explosive energy dances on $n's hand, $e grabs hold of $s blade and FlAsHEs thrOuGh YOU!!!",ch,NULL,victim,TO_VICT);
    act("As the explosive energy dances on $n's hand, $e grabs hold of $s blade and FlAsHeS thrOuGh $N!!!",ch,NULL,victim,TO_NOTVICT);

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_FIRE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 1000000, TRUE );

    if ( number_percent() > 50 - devil_luck( ch, victim, 5 ) && victim != NULL && victim->position > POS_STUNNED )
       {
          act( "#RYou are suddenly catapulted into the air from the destructive force!#n", victim, NULL, NULL, TO_CHAR );
          act( "#R$n #Ris suddenly catapulted into the air from the destructive force!#n", victim, NULL, NULL, TO_ROOM );

          stop_fighting( victim, TRUE );
          victim->air_timer = number_range( 1, 2 );
       }
}

void do_ice_break( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 4 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act("You leap high into the air, as you swing your blade around.",ch,NULL,NULL,TO_CHAR);
    act("$n leaps high into the air, as $e swings $s blade around.",ch,NULL,NULL,TO_ROOM);
    act("You then twirl your body as you somersault down towards $N.",ch,NULL,victim,TO_CHAR);
    act("$n then twirls $s body as $e somersaults down towards you!",ch,NULL,victim,TO_VICT);
    act("$n then twirls $s body as $e somersaults down towards $N!",ch,NULL,victim,TO_NOTVICT);
    act("Explosive light emits off $N as your blade SlIcEs through $S body.",ch,NULL,victim,TO_CHAR);
    act("Explosive light emits off you as $n's blade SlIcEs through your body.",ch,NULL,victim,TO_VICT);
    act("Explosive light emits off $N as $n's blade SlIcEs through $S body.",ch,NULL,victim,TO_NOTVICT);

    if ( number_percent() > 85 - devil_luck( ch, victim, 10 ) && !is_martial( victim ) && !IS_SET( victim->more, MORE_FROZEN ) )
       {
          stc("#LThe explosive energy immobilizes your body, preventing movement.#n\n\r",victim);
          act("#LThe explosive energy immobilizes $n#L's body, preventing movement.#n",victim,NULL,NULL,TO_ROOM);
          victim->freeze_timer = 1;
          SET_BIT( victim->more, MORE_FROZEN );
       }

    act("You exclaim '#yIce Break#n' as you RIP your sword out of $N's body, shattering the ice around $M.",ch,NULL,victim,TO_CHAR);
    act("$n exclaims '#yIce Break#n' as $e RIPS $s sword out of your body, shattering the ice around you.",ch,NULL,victim,TO_VICT);
    act("$n exclaims '#yIce Break#n' as $e RIPS $s sword out of $N's body, shattering the ice around $M.",ch,NULL,victim,TO_NOTVICT);

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_ICE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = OVERHEAD;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_justice_dance( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 6 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You twirl your divine blade viciously as you leap into the air!", ch, NULL, NULL, TO_CHAR );   
    act( "$n twirls $s divine blade viciously as $e leaps into the air!", ch, NULL, NULL, TO_ROOM );   
    act( "You land back on the ground, hitting it hard as an energy wave flashes from it.", ch, NULL, NULL, TO_CHAR );
    act( "$n lands back on the ground, hitting it hard as an energy wave flashes from it.", ch, NULL, NULL, TO_ROOM );
    act( "You scream in agony as the blast of energy sends you flying.", victim, NULL, NULL, TO_CHAR );
    act( "$n screams in agony as the blast of energy sends $m flying.", victim, NULL, NULL, TO_ROOM );

    if ( number_percent() > 50 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more, MORE_BLIND ) )
       {
          act("#wYou exclaim '#yThis is Justice#w' as you throw your sword mightily at $N#w!#n",ch,NULL,victim,TO_CHAR);
          act("#w$n #wexclaims '#yThis is Justice#w' as $e throws $s sword mightily at you#w!#n",ch,NULL,victim,TO_VICT);
          act("#w$n #wexclaims '#yThis is Justice#w' as $e throws $s sword mightily at $N#w!#n",ch,NULL,victim,TO_NOTVICT);
          act("#wThe sword suddenly explodes in front of your eyes as you scream in pain.#n", victim, NULL, NULL, TO_CHAR );
          act("#wThe sword suddenly explodes in front of $n#w's eyes as $e screams in pain.#n", victim, NULL, NULL, TO_ROOM );

          victim->blind_timer = 8;
          SET_BIT( victim->more, MORE_BLIND );
       }

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_HOLY;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = LOW;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_voodoo( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 71 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Curse whom?\n\r", ch );
          return;
       }

    if ( ch->saiyan_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_world( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe( ch, victim ) )
       return;
 
    ch->wait = 6;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
       {
          send_to_char("#eWhich body part do you wish to affect? #C(#weyes/feet/skin/arms/mouth/heart#C)#n\n\r", ch);
          return;
       }

    if ( !str_cmp(arg, "eyes") )
       {
         ch->saiyan_timer = 40;

         act( "You cackle loudly as you swirl your hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_CHAR );
         act( "$n cackles loudly as $e swirls $s hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_ROOM );
         act( "The doll suddenly merge into a replica of $N as you pull out a deadly needle.", ch, NULL, victim, TO_CHAR );
         act( "The doll suddenly merge into a replica of $N as $n pulls out a deadly needle.", ch, NULL, victim, TO_ROOM );
         act( "Showing no mercy, you savagely poke the needle in each of the doll's eyes!", ch, NULL, NULL, TO_CHAR );
         act( "Showing no mercy, $n savagely pokes the needle in each of the doll's eyes!", ch, NULL, NULL, TO_ROOM );
         act( "You clutch your eyes in pain as you collapse to your knees blind and confused!!!", victim, NULL, NULL, TO_CHAR );
         act( "$n clutches $s eyes in pain as $e collapses to $s knees blind and confused!!!", victim, NULL, NULL, TO_ROOM );
         SET_BIT( victim->more, MORE_BLIND );
         
         if ( victim->position > POS_STUNNED )
            victim->position = POS_RESTING;

         victim->wait = 18;
       }
       else if ( !str_cmp(arg, "feet") )
       {
         ch->saiyan_timer = 40;

         act( "You cackle loudly as you swirl your hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_CHAR );
         act( "$n cackles loudly as $e swirls $s hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_ROOM );
         act( "The doll suddenly merge into a replica of $N as you flex your fingers.", ch, NULL, victim, TO_CHAR );
         act( "The doll suddenly merge into a replica of $N as $n flexes $s fingers.", ch, NULL, victim, TO_ROOM );
         act( "Showing no mercy, you savagely twist each of the doll's feet in opposite directions!", ch, NULL, NULL, TO_CHAR );
         act( "Showing no mercy, $n savagely twists each of the doll's feet in opposite directions!", ch, NULL, NULL, TO_ROOM );
         act( "You look puzzled as both your feet try to walk away in different directions!!!", victim, NULL, NULL, TO_CHAR );
         act( "$n looks puzzled as both $s feet try to walk away in different directions!!!", victim, NULL, NULL, TO_ROOM );
         SET_BIT( victim->more2, MORE2_REVWALK );
       }
       else if ( !str_cmp(arg, "heart") )
       {
         ch->saiyan_timer = 40;

         act( "You cackle loudly as you swirl your hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_CHAR );
         act( "$n cackles loudly as $e swirls $s hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_ROOM );
         act( "The doll suddenly merge into a replica of $N as you flex your fingers.", ch, NULL, victim, TO_CHAR );
         act( "The doll suddenly merge into a replica of $N as $n flexes $s fingers.", ch, NULL, victim, TO_ROOM );
         act( "Showing no mercy, you push a ball of negative energy right into the doll's heart!", ch, NULL, NULL, TO_CHAR );
         act( "Showing no mercy, $n pushes a ball of negative energy right into the doll's heart!", ch, NULL, NULL, TO_ROOM );
         act( "You sigh sadly to yourself as you lost the will to fight!!!", victim, NULL, NULL, TO_CHAR );
         act( "$n sighs sadly to $mself as $e losts the will to fight!!!", victim, NULL, NULL, TO_ROOM );

         if ( (victim->affected) )
            {
               AFFECT_DATA *affect_next, *affect;

               affect = victim->affected;

               while ( affect )
               {
                  affect_next = affect->next;

                  if ( affect->duration != -1 )
                     affect_remove( victim, affect );

                  affect = affect_next;
               }
            }

         victim->mana = UMAX( 0, victim->mana - ( victim->max_mana * 0.33 ) );
         victim->noprotection = 2;
       }
       else if ( !str_cmp(arg, "mouth") )
       {
         ch->saiyan_timer = 40;

         act( "You cackle loudly as you swirl your hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_CHAR );
         act( "$n cackles loudly as $e swirls $s hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_ROOM );
         act( "The doll suddenly merge into a replica of $N as you flex your fingers.", ch, NULL, victim, TO_CHAR );
         act( "The doll suddenly merge into a replica of $N as $n flexes $s fingers.", ch, NULL, victim, TO_ROOM );
         act( "Showing no mercy, you savagely rip off the doll's mouth in glee!", ch, NULL, NULL, TO_CHAR );
         act( "Showing no mercy, $n savagely rips off the doll's mouth in glee!", ch, NULL, NULL, TO_ROOM );
         act( "You open your mouth to scream in pain but nothing comes out!!!", victim, NULL, NULL, TO_CHAR );
         act( "$n opens $s mouth to scream in pain but nothing comes out!!!", victim, NULL, NULL, TO_ROOM );
         SET_BIT( victim->more3, MORE3_SILENCE );
       }
       else if ( !str_cmp(arg, "skin") )
       {
         ch->saiyan_timer = 40;

         act( "You cackle loudly as you swirl your hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_CHAR );
         act( "$n cackles loudly as $e swirls $s hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_ROOM );
         act( "The doll suddenly merge into a replica of $N as you flex your fingers.", ch, NULL, victim, TO_CHAR );
         act( "The doll suddenly merge into a replica of $N as $n flexes $s fingers.", ch, NULL, victim, TO_ROOM );
         act( "Showing no mercy, you push your hand at the doll as it bursts into flames!", ch, NULL, NULL, TO_CHAR );
         act( "Showing no mercy, $n pushes $s hand at the doll as it bursts into flames!", ch, NULL, NULL, TO_ROOM );
         act( "You suddenly burst into flames as you feel your energy being depleted!!!", victim, NULL, NULL, TO_CHAR );
         act( "$n suddenly burst into flames as $e feels $s energy being depleted!!!", victim, NULL, NULL, TO_ROOM );
         victim->move = UMAX( 0, victim->move - ( victim->max_move * 0.33 ) );

         if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
            SET_BIT( victim->affected_by, AFF_FLAMING );

         SET_BIT( ch->arena, AFF2_POWER );
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_type = ATTACK_MAGICAL;
         ch->attack_var = BLAST;
         ch->attack_focus = DIRECT;
         hurt_person( ch, victim, 1000000, TRUE );
       }
       else if ( !str_cmp(arg, "arms") )
       {
         OBJ_DATA *obj;

         ch->saiyan_timer = 40;

         act( "You cackle loudly as you swirl your hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_CHAR );
         act( "$n cackles loudly as $e swirls $s hands together, creating a doll of dark energy.", ch, NULL, NULL, TO_ROOM );
         act( "The doll suddenly merge into a replica of $N as you flex your fingers.", ch, NULL, victim, TO_CHAR );
         act( "The doll suddenly merge into a replica of $N as $n flexes $s fingers.", ch, NULL, victim, TO_ROOM );
         act( "Showing no mercy, you savagely rip off each of the doll's arms!", ch, NULL, NULL, TO_CHAR );
         act( "Showing no mercy, $n savagely rips off each of the doll's arms!", ch, NULL, NULL, TO_ROOM );
         act( "Tons of blood spills onto the floor as you find your arms being pulled apart!!!", victim, NULL, NULL, TO_CHAR );
         act( "Tons of blood spills onto the floor as $n finds $s arms being pulled apart!!!", victim, NULL, NULL, TO_ROOM );
         SET_BIT( victim->more2, MORE2_NOARMS );

         if ( !IS_ARM_R(victim,LOST_ARM) )
            {
               SET_BIT(victim->loc_hp[3],LOST_ARM);
               SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
               make_part(victim,"arm");

               if ( IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM) )
                  {
                     if ( ( obj = get_eq_char( victim, WEAR_ARMS ) ) != NULL )
                        take_item(victim,obj);
                  }

               if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) != NULL )
                  take_item(victim,obj);

               if ( ( obj = get_eq_char( victim, WEAR_HANDS ) ) != NULL )
                  take_item(victim,obj);

               if ( ( obj = get_eq_char( victim, WEAR_WRIST_R ) ) != NULL )
                  take_item(victim,obj);

               if ( ( obj = get_eq_char( victim, WEAR_FINGER_R ) ) != NULL )
                  take_item(victim,obj);
            }

         if ( !IS_ARM_L(victim,LOST_ARM) )
            {
               SET_BIT(victim->loc_hp[2],LOST_ARM);
               SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
               make_part(victim,"arm");

               if ( IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM) )
                  {
                     if ( ( obj = get_eq_char( victim, WEAR_ARMS ) ) != NULL )
                        take_item(victim,obj);
                  }

               if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) != NULL )
                  take_item(victim,obj);
   
               if ( ( obj = get_eq_char( victim, WEAR_HANDS ) ) != NULL )
                  take_item(victim,obj);

               if ( ( obj = get_eq_char( victim, WEAR_WRIST_L ) ) != NULL )
                  take_item(victim,obj);

               if ( ( obj = get_eq_char( victim, WEAR_FINGER_L ) ) != NULL )
                  take_item(victim,obj);
            }
       }
       else {
               send_to_char("#eWhich body part do you wish to affect? #C(#weyes/feet/skin/arms/mouth/heart#C)#n\n\r", ch);
               ch->saiyan_timer = 0;
            } 
}

void do_earth_smash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 3 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Strike whom?\n\r", ch );
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
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
 
    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act( "You twirl your rubbly blade viciously as you leap into the air!", ch, NULL, NULL, TO_CHAR );   
    act( "$n twirls $s rubbly blade viciously as $e leaps into the air!", ch, NULL, NULL, TO_ROOM );   
    act( "You land back on the ground, hitting it hard as shards of rock leap from it.", ch, NULL, NULL, TO_CHAR );
    act( "$n lands back on the ground, hitting it hard as shard of rock leap from it.", ch, NULL, NULL, TO_ROOM );
    act( "You are knocked away as the rocks pummel you HaRd!", victim, NULL, NULL, TO_CHAR );   
    act( "$n is knocked away as the rocks pummel $m HaRd!", victim, NULL, NULL, TO_ROOM );   

    SET_BIT( ch->arena, AFF2_POWER );
    ch->ele_attack = ATTACK_EARTH;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLUNT;
    ch->attack_focus = LOW;
    hurt_person( ch, victim, 1000000, TRUE );

    if ( victim == NULL || victim->in_room == NULL || victim->position < POS_STUNNED )
       return;

    if ( number_percent() > 50 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->mflags, MAGE_EMBRACED2 ) && !IS_SET( victim->mflags, MAGE_EMBRACED1 ) )
       {
          act("#gAs you crash back onto the ground, it suddenly envelops you!#n", victim, NULL, NULL, TO_CHAR );
          act("#gAs $n #gcrashes back onto the ground, it suddenly envelops $m.#n", victim, NULL, NULL, TO_ROOM );

          SET_BIT( victim->mflags, MAGE_EMBRACED2 );
          stop_fighting( victim, TRUE );
          victim->position = POS_STUNNED;
       }
}

void do_energysword( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 9 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->saiyan_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->saiyan_timer = 40;
    ch->wait = 6;

    act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
    act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
    act("You say, '#yEnergy#n' as your sword shimmers intensely.",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yEnergy#n' as $s sword shimmers intensely.",ch,NULL,NULL,TO_ROOM);
    act("As the energy goes through your body, you suddenly feel more refreshed.", ch, NULL, NULL, TO_CHAR);
    act("As the energy goes through $n's body, $e suddenly feels more refreshed.", ch, NULL, NULL, TO_ROOM);

    ch->move = UMIN( ch->max_move, ch->move + ( ch->max_move / ( 6 - ch->tier ) ) );
    ch->mana = UMIN( ch->max_mana, ch->mana + ( ch->max_mana / ( 6 - ch->tier ) ) );
}

void do_darkball( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 76 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->saiyan_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->saiyan_timer = 40;
    ch->wait = 6;

    act("You move your hands in a circular motion creating a #0dark #Pportal#n",ch,NULL,NULL,TO_CHAR);
    act("$n moves $s hands in a circular motion creating a #0dark #Pportal#n",ch,NULL,NULL,TO_ROOM);
    act("#yLightning #C#zsparks#n throughout the #0black hole#n as the #gearth#n RuMbLeS",ch,NULL,NULL,TO_CHAR);
    act("#yLightning #C#zsparks#n throughout the #0black hole#n as the #gearth#n RuMbLeS",ch,NULL,NULL,TO_ROOM);
    act("Your eyes #Rburn#n in #Ranger#n as you shield yourself from the mysterious #0darkness#n",ch,NULL,NULL,TO_CHAR);
    act("$n's eyes #Rburn#n in #Ranger#n as $e shields $mself from the mysterious #0darkness#n",ch,NULL,NULL,TO_ROOM);
    act("Suddenly the whole room starts to crumble as #ylight#n itself is sucked into the #0black hole#n",ch,NULL,NULL,TO_CHAR);
    act("Suddenly the whole room starts to crumble as #ylight#n itself is sucked into the #0black hole#n",ch,NULL,NULL,TO_ROOM);
    act("You grab onto what you can in fear of being sucked into the #Rdemonic #0black hole#n",ch,NULL,NULL,TO_ROOM);
    act("You stand silently and grin evilly at the #0C#Rh#0a#Ro#0s#n you have caused",ch,NULL,NULL,TO_CHAR);
    act("$n stands silently and grins evilly at the #0C#Rh#0a#Ro#0s#n $e has caused",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_same_group( ch, vch ) || is_safe( ch, vch ) )
          continue;    

       act("You fall to your knees as you are severely weakened by the strength of the #0black hole#n",vch,NULL,NULL,TO_CHAR);
       act("$n falls to $s knees as $e is severely weakened by the strength of the #0black hole#n",vch,NULL,NULL,TO_ROOM);
       act("You stare at the #Rdemonic #0black hole#n as it shrinks taking your very #Clife force#n and #ysoul#n with it",vch,NULL,NULL,TO_CHAR);
       act("$n stares at the #Rdemonic #0black hole#n as it shrinks taking $s very #Clife force#n and #ysoul#n with it",vch,NULL,NULL,TO_ROOM);

       vch->mana = UMAX( 0, vch->mana - ( vch->max_mana * 0.75 ) );
       vch->move = UMAX( 0, vch->move - ( vch->max_move * 0.75 ) );
    }
}

void do_summon_spirit( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 56 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyan_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to pull towards you?\n\r", ch );
         return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_world( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   if ( victim->fight_timer == 0 )
      {
         stc( "#eYou can't focus someone's lifeforce unless they have a #wfight timer#e.#n\n\r", ch );
         return;
      }

   if ( ch->in_room == victim->in_room )
      {
         send_to_char( "They are already near you.\n\r", ch );
         return;
      }

   ch->saiyan_timer = 40;
   ch->wait = 6;

   act("#eSensing #w$N#w's #efrantic, you sneer as you focus your energy on $M.#n",ch,NULL,victim,TO_CHAR);

   act("You hear a soft humming in the air as a mystical portal appears before you.",victim,NULL,NULL,TO_CHAR);
   act("#oSuddenly, some invisible force drags you into the portal!#n",victim,NULL,NULL,TO_ROOM);
   act("#oSuddenly, some invisible force drags $n #oaway into a shimmering portal!#n",victim,NULL,NULL,TO_ROOM);

   stop_fighting( victim, TRUE );
   char_from_room( victim );
   char_to_room( victim, ch->in_room );

   act("You snarl with rage as you find yourself in front of the summoner!",victim,NULL,NULL,TO_CHAR);
   act("$n snarls with rage as $e finds $mself in front of the people who summon $m!",victim,NULL,NULL,TO_ROOM);

   if ( !IS_CLASS( victim, CLASS_WISP ) )
      victim->wait = 18;
}

void do_assist( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    CHAR_DATA *mob;
    AFFECT_DATA af;

    if ( IS_NPC(ch) )
       return;

    if ( !can_do_skill( ch, 30 ) )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->position != POS_FIGHTING )
       {
          stc("But you aren't in the heat of battle.\n\r", ch );
          return;
       }
	
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( !IS_NPC( vch ) )
           continue;

        if ( vch->pIndexData->vnum == 89 )
           {
              stc("#yYou #Calready have assistance from your loyal golems #ycaller#C.#n\n\r", ch );
              return;
           }
    }

    if ( ( mob = create_mobile( get_mob_index( 89 ) ) ) == NULL )
       {
          stc("Your golems refuse to fight by your aid. Please inform an Immortal!\n\r",ch);
          return;
       }

    act("#yYou #Csuddenly take out a #wtrumpet #Cin the midst of battle and blow on it loudly.#n", ch, NULL, NULL, TO_CHAR );
    act("#y$n #Csuddenly takes out a #wtrumpet #Cin the midst of battle and blows on it loudly.#n", ch, NULL, NULL, TO_ROOM );
    act("#yYou #Csmile as you see your powerful golems coming to your aid.#n", ch, NULL, NULL, TO_CHAR );
    act("#y$n #Csmiles as $e sees $s powerful golems coming to $s aid.#n", ch, NULL, NULL, TO_ROOM );

    char_to_room( mob, ch->in_room );
    add_follower( mob, ch );
    do_group( ch, mob->name );

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( mob, &af );

    SET_BIT( mob->act, ACT_TIMER );
    mob->practice = 2;
    ch->wait = 12;

    if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
       SET_BIT( mob->affected_by, AFF_ETHEREAL );

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
       SET_BIT( mob->affected_by, AFF_SHADOWPLANE );

    if ( ch->pcdata->deity != DEI_GAIA )
       return;

    if ( ( mob = create_mobile( get_mob_index( 89 ) ) ) == NULL )
       {
          stc("Shit.  The Mob is missing.  Fuck.  Tell someone. Fuck!\n\r",ch);
          return;
       }

    char_to_room( mob, ch->in_room );
    add_follower( mob, ch );
    do_group( ch, mob->name );

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( mob, &af );

    SET_BIT( mob->act, ACT_TIMER );
    mob->practice = 2;

    if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
       SET_BIT( mob->affected_by, AFF_ETHEREAL );

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
       SET_BIT( mob->affected_by, AFF_SHADOWPLANE );
}

void do_rename(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int option;

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);
    strcpy( arg3, argument );

    if ( IS_NPC( ch ) )
       return;

    if ( arg1 == NULL || arg2 == NULL || arg3 == NULL )
       {
          send_to_char("#wSyntax: #CRename #yobject #C(name/short) #P(description)#n\n\r",ch);
          return;
       }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
       {
          send_to_char("You dont have that item.\n\r",ch);
          return;
       }

    if ( !str_cmp( "name", arg2 ) )
       option = 1;
       else if ( !str_cmp( "short", arg2 ) )
               option = 2;
               else {
                       do_rename( ch, "" );
                       return;
                    }

    if ( strlen(arg3) > 40 || strlen(arg3) < 3 )
       {
          send_to_char("From 3 to 40 characters please.\n\r",ch);
          return;
       }

    if ( !IS_SET( obj->extra_flags, ITEM_RENAME ) )
       {
          if ( ch->pcdata->faithpoints < 1 )
             {
                stc( "#yYou #wneed at least #C1 #olegend #ypoint #wto rename an object.#n\n\r", ch );
                return;
             }

          ch->pcdata->faithpoints -= 1;
          SET_BIT( obj->extra_flags, ITEM_RENAME );
       }

    if ( option == 1 )
       {
          free_string(obj->name);
          obj->name = str_dup(arg3);
       }

    if ( option == 2 )
       {
          free_string(obj->short_descr);
          obj->short_descr=str_dup(arg3);
       }

    obj->questmaker = str_dup(ch->name);
    send_to_char("Ok.\n\r",ch);
}
