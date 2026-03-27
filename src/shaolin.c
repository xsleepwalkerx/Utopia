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

void show_trance( CHAR_DATA *ch )
{
   if ( ch->position > POS_STUNNED )
      {
         act( "#yYour #wbody levitates into the air as you raise your #elifeless #Ceyes #wabove you.#n", ch, NULL, NULL, TO_CHAR );
         act( "#y$n's #wbody levitates into the air as $e raises $s #elifeless #Ceyes #wabove $m.#n", ch, NULL, NULL, TO_ROOM );
         act( "#wa grin appears on #yyour #wface as your body explodes into #Rradiant #wenergy.#n", ch, NULL, NULL, TO_CHAR );
         act( "#wa grin appears on #y$n's #wface as $s body explodes into #Rradiant #wenergy.#n", ch, NULL, NULL, TO_ROOM );
         act( "#yYou #wland back gently on the ground as you stare around with #edaunting #Ceyes.#n", ch, NULL, NULL, TO_CHAR );
         act( "#y$n #wlands back gently on the ground as $e stares around with #edaunting #Ceyes.#n", ch, NULL, NULL, TO_ROOM );
      }

   stc( "#wYou are now in #wT#RR#yA#CN#gC#PE#w mode.#n\n\r", ch );
   ch->ctimer[4] = 105;
}

void show_sina_combo( CHAR_DATA *ch )
{
   char buf[MAX_INPUT_LENGTH];
   int option = number_range( 1, 15 );

   if ( IS_SET( ch->monkab[0], 8 ) )
      return;

   act("Your body freeze as everything around you turns white!", ch, NULL, NULL, TO_CHAR );
   act("$n's body suddenly freeze as $s eyes turn milky!", ch, NULL, NULL, TO_ROOM );
   act("You find yourself in a desolated land with two powerful combantants on it.", ch, NULL, NULL, TO_CHAR );

   sprintf( buf, "Time eventually shatter as you are filled with a vivid flashback of #Pmove#n #w#z%d#n!\n\r", option );
   stc( buf, ch );

   switch( option )
   {
      case 14:
      case 1 : act("#yKyo leans forward as $e kicks Iori viciously in the face.#n", ch, NULL, NULL, TO_CHAR );
               break;

      case 6:
      case 7:
      case 13:
      case 15:
      case 2 : act("#yKyo quickly punches Iori hard in the stomach.#n", ch, NULL, NULL, TO_CHAR );
               break;

      case 12:
      case 3 : act("#yKyo suddenly twirls $s foot under Iori's feet, tripping him down.#n", ch, NULL, NULL, TO_CHAR );
               break;

      case 9:
      case 4 : act("#yKyo twirls his body around StAgGeRiNg Iori with a vicious roundhouse kick.#n", ch, NULL, NULL, TO_CHAR);
               break;

      case 5 : act("#yKyo spins around catching Iori offguard with an elbow to the face.#n", ch, NULL, NULL, TO_CHAR );
               break;

      case 8 : act("#yKyo SmAsHeS Iori's chin with the palm of his hand as he leap into the air.#n", ch, NULL, NULL, TO_CHAR );
               break;

      case 11:
      case 10: act("#yKyo rams $s knee in Iori's guts.#n", ch, NULL, NULL, TO_CHAR );
               break;
   }
}

void do_monkdisarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
   OBJ_DATA *obj;
   int chance = 60;

   if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
      if ( ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
         if ( ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
            if ( ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
               return;

   if ( !IS_NPC( ch ) )
      {
         REMOVE_BIT(ch->more, MORE_BLADEGRASP);

         if ( IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_ASSASSIN ) || IS_SET( victim->cmbt[5], SKL2_GRIP ) )
            return;
      }
      else chance = 50;

   act("\n\rYou suddenly grab $N's weapon by grasping the bare blade of it.",ch,NULL,victim,TO_CHAR);
   act("\n\r$n suddenly grab your weapon by grasping the bare blade of it.",ch,NULL,victim,TO_VICT);
   act("\n\r$n suddenly grab $N's weapon by grasping the bare blade of it.",ch,NULL,victim,TO_NOTVICT);
   act("You then push your right palm in $N's face stunning $M.",ch,NULL,victim,TO_CHAR);
   act("$e then pushes $s right palm in your face stunning you.",ch,NULL,victim,TO_VICT);
   act("$e then pushes $s right palm in $N's face stunning $M.",ch,NULL,victim,TO_NOTVICT);
   act("As $N flips forward, you punch $M hard in the stomach taking $S weapon.\n\r",ch,NULL,victim,TO_CHAR);
   act("As you flip forward, $n punches you hard in the stomach taking away your weapon.\n\r",ch,NULL,victim,TO_VICT);
   act("As $N flips forward, $n punches $M hard in the stomach taking $S weapon.\n\r",ch,NULL,victim,TO_NOTVICT);

   obj_from_char( obj );
   obj_to_char( obj, victim );

   hurt_person( ch, victim, 250000, FALSE );

   if ( ch == NULL || victim == NULL || victim->position != POS_FIGHTING )
      return;

   if ( number_percent() < 60 )
      return;

   if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
      if ( ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
         if ( ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
            if ( ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
               return;

   act("Throwing $N's weapon in the air, you twirl your foot into $S face.",ch,NULL,victim,TO_CHAR);
   act("Throwing your weapon in the air, $n twirls $s foot into $S face.",ch,NULL,victim,TO_VICT);
   act("Throwing $N's weapon in the air, $n twirls $s foot into $S face.",ch,NULL,victim,TO_NOTVICT);
   act("You then SmAsH your palm into $N's chest as you hit away another weapon.",ch,NULL,victim,TO_CHAR);
   act("$n then SmAsHeS $s palm into your chest as $e hits away another weapon.",ch,NULL,victim,TO_VICT);
   act("$n then SmAsHeS $s palm into $N's chest as $e hits away another weapon.",ch,NULL,victim,TO_NOTVICT);
   act("Grabbing the weapon, you swing it viciously across $N's chest as you stagger $M.",ch,NULL,victim,TO_CHAR);
   act("Grabbing the weapon, $n swings it viciously across your chest as $e staggers $M.",ch,NULL,victim,TO_VICT);
   act("Grabbing the weapon, $n swings it viciously across $N's chest as $e staggers $M.",ch,NULL,victim,TO_NOTVICT);
   act("You then spin around sweeping $N off his feet.",ch,NULL,victim,TO_CHAR);
   act("$n then spins around sweeping you off his feet.",ch,NULL,victim,TO_VICT);
   act("$n then spins around sweeping $N off his feet.",ch,NULL,victim,TO_NOTVICT);

   obj_from_char( obj );
   obj_to_char( obj, victim );

   hurt_person( ch, victim, 250000, TRUE );
}

void do_blade_grasp( CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->monkab[BODY] < 4 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->focus[CURRENT] < 60 )
      {
         send_to_char("You don't have enough focus points for that.\n\r",ch);
         return;
      }

   if ( ch->fighting == NULL )
      {
         send_to_char("But you aren't fighting anyone.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->more, MORE_BLADEGRASP) )
      {
         send_to_char("Your are already concentrating on your opponent's weapon.\n\r",ch);
         return;
      }

   SET_BIT(ch->more, MORE_BLADEGRASP);
   send_to_char("You begin to concentrate on your opponent's weapon.\n\r",ch);
   ch->focus[CURRENT] -= 60;
}

void do_hand_hamedo( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) || ch->clannum != 2 )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->monk_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->position != POS_FIGHTING )
      {
         stc("But you aren't in a fight.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->more2, MORE2_GUARD) )
      {
         stc("You are already tilted in the hamedo fighting style.\n\r",ch);
         return;
      }

   SET_BIT(ch->more2, MORE2_GUARD);
   ch->monk_timer = 9;
   act("You softly close your eyes focusing on the auras around you.",ch,NULL,NULL,TO_CHAR);
}

void do_vulcanpinch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->monkab[BODY] < 2 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->focus[CURRENT] < 10 )
      {
         stc("You can't concentrate enough to use this power.\n\r", ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to use the Vulcan Pinch on?\n\r",ch);
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
         stc("That doesn't seem like a good idea.\n\r",ch);
         return;
      }

   if ( IS_SET(victim->more, MORE_PINCH) )
      {
         stc("They already have a crippling headache.\n\r",ch);
         return;
      }

   act("You circle around $N and pinch the nerves behind $S neck.",ch,NULL,victim,TO_CHAR);
   act("$n circles behind you and softly pinches you on the neck.",ch,NULL,victim,TO_VICT);
   act("$n circles around $N and pinches the nerves behind $S neck.",ch,NULL,victim,TO_NOTVICT);

   act("You stumble as you get a CrIpPlInG HeAdAcHe!.",ch,NULL,victim,TO_VICT);
   act("$n stumbles as $e gets a CrIpPlInG HeAdAcHe!.",victim,NULL,NULL,TO_ROOM);

   SET_BIT(victim->more, MORE_PINCH);

   ch->focus[CURRENT] -= 10;
}

void do_focus( CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->monkab[BODY] < 6 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->move < 10000000 )
      {
         send_to_char("You are too tired to do this technique.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->more, MORE_FOCUS) )
      {
         REMOVE_BIT(ch->more, MORE_FOCUS);
         send_to_char("You take a deep breath as you lower your concentration\n\r",ch);
         act("$n takes a deep breath as $e seems to be more calm.",ch,NULL,NULL,TO_ROOM);
         return;
      }

   SET_BIT(ch->more, MORE_FOCUS);
   send_to_char("You concentrate with all of your might as your focus achieve a new height.\n\r",ch);
   act("$n's body trembles as $e focuses on $s concentration.",ch,NULL,NULL,TO_ROOM);
}

void perform_sina( CHAR_DATA *ch, CHAR_DATA *victim )
{
   if ( IS_NPC( ch ) && ch->pIndexData->vnum != 564 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_SET(ch->more, MORE_FOCUS) || ch->ctimer[9] < 300 || !IS_SET( ch->more3, MORE3_HAND )
              || ch->move < 2500000 )
            return;

         if ( !IS_SET( ch->monkab[0], 8 ) )
            SET_BIT( ch->monkab[0], 8 );

         ch->move -= 2500000;
         ch->ctimer[9] = 0;
         SET_BIT( ch->arena, AFF2_DM );
      }

   act("You yell '#yEia Ziek!#n' as you swing wildly across $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across your chest.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across $N's chest.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yEia Ziek!#n' as you swing wildly across $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across your chest.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across $N's chest.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yEia Ziek!#n' as you swing wildly across $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across your chest.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yEia Ziek!#n' as $e swings wildly across $N's chest.",ch,NULL,victim,TO_NOTVICT);
   act("Your body flickers in the shadows for a second as your arm burst into flames!",ch,NULL,NULL,TO_CHAR);
   act("$n's body flickers in the shadows for a second as $s arm burst into flames!",ch,NULL,NULL,TO_ROOM);
   act("You flick your wrist up $N's body searing $M with ferocious flames!",ch,NULL,victim,TO_CHAR);
   act("$n flicks $s wrist up your body searing you with ferocious flames!",ch,NULL,victim,TO_VICT);
   act("$n flicks $s wrist up $N's body searing $M with ferocious flames!",ch,NULL,victim,TO_NOTVICT);
   act("#eYour shadow yells '#yEia Ziek!#e' as it swings wildly across $N#e's chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's shadow yells '#yEia Ziek!#e' as it swings wildly across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's shadow yells '#yEia Ziek!#e' as it swings wildly across $N#e's chest.#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour shadow flicks its wrist up $N#e's body searing $M with ferocious flames!#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's shadow flicks its wrist up your body searing you with ferocious flames!#n",ch,NULL,victim,TO_VICT);
   act("#e$n's shadow flicks its wrist up $N#e's body searing $M with ferocious flames!#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour shadow yells '#yEia Ziek!#e' as it swings wildly across $N#e's chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's shadow yells '#yEia Ziek!#e' as it swings wildly across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's shadow yells '#yEia Ziek!#e' as it swings wildly across $N#e's chest.#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour shadow yells '#yEia Ziek!#e' as it swings wildly across $N#e's chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's shadow yells '#yEia Ziek!#e' as it swings wildly across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's shadow yells '#yEia Ziek!#e' as it swings wildly across $N#e's chest.#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour shadow flicks its wrist up $N#e's body searing $M with ferocious flames!#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's shadow flicks its wrist up your body searing you with ferocious flames!#n",ch,NULL,victim,TO_VICT);
   act("#e$n's shadow flicks its wrist up $N#e's body searing $M with ferocious flames!#n",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yOOOOOOIAAAAA YAAAAAH!#n' as you SmAsH your glowing fist up $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yOOOOOOIAAAAA YAAAAAH!#n' as $e SmAsHeS $s glowing fist up your chin!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yOOOOOOIAAAAA YAAAAAH!#n' as $e SmAsHeS $s glowing fist up $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("#wThe room suddenly EXPLODES with instense energy as everything goes white!!!#n",ch,NULL,NULL,TO_CHAR);
   act("#wThe room suddenly EXPLODES with instense energy as everything goes white!!!#n",ch,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;

   if ( !IS_NPC( ch ) && !( ch->hit < ch->max_hit * 0.15 ) )
      hurt_person( ch, victim, 2000000, TRUE );
      else {
              SET_BIT( ch->arena, AFF2_UB );

              hurt_person( ch, victim, 5000000, TRUE );

              REMOVE_BIT( ch->arena, AFF2_UB );
           }
}

void do_gin_sentsu( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You charge into $N SmAsHiNg your elbow into $S face!",ch,NULL,victim,TO_CHAR);
   act("$n charges into you SmAsHiNg $s elbow into your face!",ch,NULL,victim,TO_VICT);
   act("$n charges into $N SmAsHiNg $s elbow into $S face!",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly twirl around spinning your foot into $N face!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly twirls around spinning $s foot into your face!",ch,NULL,victim,TO_VICT);
   act("$n suddenly twirls around spinning $s foot into $N face!",ch,NULL,victim,TO_NOTVICT);
   act("You push your palm into $N's chest viciously as you crouch on your knees.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s palm into your chest viciously as $e crouches on $s knees.",ch,NULL,victim,TO_VICT);
   act("$n pushes $s palm into $N's chest viciously as $e crouches on $s knees.",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#wAwwwhhh....#n' as you focus your chi on your fist, creating a ball of energy.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#wAwwwhhh....#n' as $e focuses $s chi on $s fist, creating a ball of energy.",ch,NULL,NULL,TO_ROOM);
   act("You open your palm as the ball sears $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n opens $s palm as the ball sears your chest!",ch,NULL,victim,TO_VICT);
   act("$n opens $s palm as the ball sears $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("MaSsIvE explosions fill the air as the energy ball RiPs through your body!",victim,NULL,NULL,TO_CHAR);
   act("MaSsIvE explosions fill the air as the energy ball RiPs through $n's body!",victim,NULL,NULL,TO_ROOM);
   act("You yell '#wZani Ken!#n' as you SmAsH your elbow into $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#wZani Ken!#n' as $e SmAsHeS $s elbow into your chest.",ch,NULL,victim,TO_VICT);
   act("$n yells '#wZani Ken!#n' as $e SmAsHeS $s elbow into $N's chest.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#wGin Sentsu!#n' as you twirl your fists into $N's body, sending $M sprawling.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#wGin Sentsu!#n' as $e twirls $s fists into your body, sending you sprawling.",ch,NULL,victim,TO_VICT);
   act("$n yells '#wGin Sentsu!#n' as $e twirls $s fists into $N's body, sending $M sprawling.",ch,NULL,victim,TO_NOTVICT);

   victim->mana = UMAX( 0, victim->mana - 500000 );

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   SET_BIT( ch->arena, AFF2_COMBO );

   if ( IS_CLASS( ch, CLASS_NINJA ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
      hurt_person(ch,victim,700000,TRUE);
      else if ( IS_SET(ch->more, MORE_FOCUS) )
              hurt_person(ch,victim,500000,TRUE);
      else hurt_person(ch,victim,350000,TRUE);
}

void do_vulcan_upper( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam_bonus = 0;

   act("You leap towards $N with your blazing hand stretch out in front of you.",ch,NULL,victim,TO_CHAR);
   act("$n leaps towards you with $s blazing hand stretch out in front of $m.",ch,NULL,victim,TO_VICT);
   act("$n leaps towards $N with $s blazing hand stretch out in front of $m.",ch,NULL,victim,TO_NOTVICT);
   act("Grabbing $N, you grimace with pain as you SMASH your fist into $N's stomach!",ch,NULL,victim,TO_CHAR);
   act("Grabbing you, $n grimaces with pain as $e SMASHES $s fist into your stomach!",ch,NULL,victim,TO_VICT);
   act("Grabbing $N, $n grimaces with pain as $e SMASHES $s fist into $N's stomach!",ch,NULL,victim,TO_NOTVICT);
   act("You grin as you mutter '#yAhhhhhhh...#n' softly as the flames within your stir.",ch,NULL,NULL,TO_CHAR);
   act("$n grins as $e mutters '#yAhhhhhhh...#n' softly as the flames around $s hands flickers.",ch,NULL,NULL,TO_ROOM);
   act("Releasing all of your energy, you grimace as you DRIVE your BURNING FIST into $N's FACE!!!",ch,NULL,victim,TO_CHAR);
   act("Releasing all of $s energy, $n grimaces as $e DRIVES $s BURNING FIST into your FACE!!!",ch,NULL,victim,TO_VICT);
   act("Releasing all of $s energy, $n grimaces as $e DRIVES $s BURNING FIST into $N's FACE!!!",ch,NULL,victim,TO_NOTVICT);
   act("You scream loudly as everything goes white!",victim,NULL,NULL,TO_CHAR);
   act("$n screams loudly as the entire vicinity TREMBLES from the impact!",victim,NULL,NULL,TO_ROOM);

   if ( !IS_AFFECTED(victim, AFF_FLAMING) ) 
      {
         SET_BIT(victim->affected_by, AFF_FLAMING);
         dam_bonus += number_range(50000,100000);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_var = BLUNT;
   ch->attack_focus = OVERHEAD;
   SET_BIT( ch->arena, AFF2_COMBO );

   if ( IS_CLASS( ch, CLASS_NINJA ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
      hurt_person(ch,victim,700000 + dam_bonus,TRUE);
      else if ( IS_SET(ch->more, MORE_FOCUS) )
              hurt_person(ch,victim,500000 + dam_bonus,TRUE);
      else hurt_person(ch,victim,350000 + dam_bonus,TRUE);
}

void do_nanpa( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You swirl your hands together as you lean forward.",ch,NULL,NULL,TO_CHAR);
   act("$n swirls $s hands together as $e leans forward.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yThoi!!#n' as you RaM your hands into $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yThoi!!#n' as $e RaMs $s hands into your chest!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yThoi!!#n' as $e RaMs $s hands into $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("Your hands explodes with feroicious energy as you SmAsH $N with an energy ball!",ch,NULL,victim,TO_CHAR);
   act("$n's hands explodes with feroicious energy as $e SmAsHeS you with an energy ball!",ch,NULL,victim,TO_VICT);
   act("$n's hands explodes with feroicious energy as $e SmAsHeS $N with an energy ball!",ch,NULL,victim,TO_NOTVICT);
   act("You scream as you are SlAmMeD towards the other side of the room.",victim,NULL,NULL,TO_CHAR);
   act("$n screams as $e is SlAmMeD towards the other side of the room.",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 39 && !IS_SET(victim->more, MORE_BLIND) )
      {
         SET_BIT(victim->more, MORE_BLIND);
         victim->blind_timer = 6;
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   SET_BIT( ch->arena, AFF2_COMBO );

   if ( IS_CLASS( ch, CLASS_NINJA ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
      hurt_person(ch,victim,700000,TRUE);
      else if ( IS_SET(ch->more, MORE_FOCUS) )
              hurt_person(ch,victim,500000,TRUE);
      else hurt_person(ch,victim,350000,TRUE);
}

void do_myo_suhaten( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You dash towards $N as you kick $M viciously in the stomach!",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you as $e kicks you viciously in the stomach!",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N as $e kicks $M viciously in the stomach!",ch,NULL,victim,TO_NOTVICT);
   act("You then raise your foot StRiKiNg $M across the face.",ch,NULL,victim,TO_CHAR);
   act("$n then raises $s foot StRiKiNg you across the face.",ch,NULL,victim,TO_VICT);
   act("$n then raises $s foot StRiKiNg $M across the face.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yNicashu!#n' as you repeatively strike your foot across $N's face.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yNicashu!#n' as $e repeatively strikes $s foot across your face.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yNicashu!#n' as $e repeatively strikes $s foot across $N's face.",ch,NULL,victim,TO_NOTVICT);
   act("You then spin around SmAsHiNg your foot into $N's face.",ch,NULL,victim,TO_CHAR);
   act("$n then spins around SmAsHiNg $s foot into your face.",ch,NULL,victim,TO_VICT);
   act("$n then spins around SmAsHiNg $s foot into $N's face.",ch,NULL,victim,TO_NOTVICT);
   act("You then lean backwards as you give $N a vicious FlAsH kIcK!!!",ch,NULL,victim,TO_CHAR);
   act("$n then leans backwards as $e gives you a vicious FlAsH kIcK!!!",ch,NULL,victim,TO_VICT);
   act("$n then leans backwards as $e gives $N a vicious FlAsH kIcK!!!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#wOOoo Gaw!!#n' as you SmAsH your foot into $N's face, sending $M into the air!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#wOOoo Gaw!!#n' as $e SmAsHeS $s foot into your face, sending you into the air!",ch,NULL,victim,TO_VICT);
   act("$n yells '#wOOoo Gaw!!#n' as $e SmAsHeS $s foot into $N's face, sending $M into the air!",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly you spin your body DrIvInG your foot into $N, sending $M sprawling onto the ground!",ch,NULL,victim,TO_CHAR);
   act("Suddenly $n spins $s body DrIvInG $s foot into you, sending you sprawling onto the ground!",ch,NULL,victim,TO_VICT);
   act("Suddenly $n spins $s body DrIvInG $s foot into $N, sending $M sprawling onto the ground!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 39 && !IS_SET( victim->more, MORE_PAIN ) )
      SET_BIT( victim->more, MORE_PAIN );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = OVERHEAD;
   SET_BIT( ch->arena, AFF2_COMBO );

   if ( IS_CLASS( ch, CLASS_NINJA ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
      hurt_person(ch,victim,700000,TRUE);
      else if ( IS_SET(ch->more, MORE_FOCUS) )
              hurt_person(ch,victim,500000,TRUE);
      else hurt_person(ch,victim,350000,TRUE);

   act("You land back on your feet softly.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s feet softly.",ch,NULL,NULL,TO_ROOM);
}

void do_mangetsu_zan( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You mutter '#yZiin...#n' as you SmAsH your foot upwards $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yZiin...#n' as $e SmAsHeS $s foot upwards your face!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yZiin...#n' as $e SmAsHeS $s foot upwards $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You flip backwards as you DrIvE your feet into $N's face again!",ch,NULL,victim,TO_CHAR);
   act("$n flips backwards as $e DrIvEs $s feet into your face again!",ch,NULL,victim,TO_VICT);
   act("$n flips backwards as $e DrIvEs $s feet into $N's face again!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yYa!!#n' as your foot strike $N's face at the speed of LiGhT!!!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yYa!!#n' as $s foot strikes your face at the speed of LiGhT!!!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yYa!!#n' as $s foot strikes $N's face at the speed of LiGhT!!!",ch,NULL,victim,TO_NOTVICT);

   victim->move = UMAX( 0, victim->move - 500000 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   SET_BIT( ch->arena, AFF2_COMBO );

   if ( IS_CLASS( ch, CLASS_NINJA ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
      hurt_person(ch,victim,700000,TRUE);
      else if ( IS_SET(ch->more, MORE_FOCUS) )
              hurt_person(ch,victim,500000,TRUE);
      else hurt_person(ch,victim,350000,TRUE);

   act("You land on the ground as you breath deeply.",ch,NULL,NULL,TO_CHAR);
   act("$n lands on the ground as $e breathes deeply.",ch,NULL,NULL,TO_ROOM);
}

void do_soshuu_jinn( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You mutter '#yUm Sei Do...#n' as you twirl your body around.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yUm Sei Do...#n' as $e twirls $s body around.",ch,NULL,NULL,TO_ROOM);
   act("You RaM your foot onto $N's head viciously as you land on the ground.",ch,NULL,victim,TO_CHAR);
   act("$n RaMs $s foot onto your head viciously as $e lands on the ground.",ch,NULL,victim,TO_VICT);
   act("$n RaMs $s foot onto $N's head viciously as $e lands on the ground.",ch,NULL,victim,TO_NOTVICT);
   act("Your image RaMs its foot onto $N's head viciously as it lands on the ground.",ch,NULL,victim,TO_CHAR);
   act("$n's image RaMs its foot onto your head viciously as it lands on the ground.",ch,NULL,victim,TO_VICT);
   act("$n's image RaMs its foot onto $N's head viciously as it lands on the ground.",ch,NULL,victim,TO_NOTVICT);
   act("Your image RaMs its foot onto $N's head viciously as it lands on the ground.",ch,NULL,victim,TO_CHAR);
   act("$n's image RaMs its foot onto your head viciously as it lands on the ground.",ch,NULL,victim,TO_VICT);
   act("$n's image RaMs its foot onto $N's head viciously as it lands on the ground.",ch,NULL,victim,TO_NOTVICT);
   act("Your image RaMs its foot onto $N's head viciously as it lands on the ground.",ch,NULL,victim,TO_CHAR);
   act("$n's image RaMs its foot onto your head viciously as it lands on the ground.",ch,NULL,victim,TO_VICT);
   act("$n's image RaMs its foot onto $N's head viciously as it lands on the ground.",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_SET( victim->more2, MORE2_NOLEGS ) && number_percent() > 49 )
      SET_BIT( victim->more2, MORE2_NOLEGS );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = SOUL;
   ch->attack_focus = AERIAL;
   SET_BIT( ch->arena, AFF2_COMBO );

   if ( IS_CLASS( ch, CLASS_NINJA ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
      hurt_person(ch,victim,700000,TRUE);
      else if ( IS_SET(ch->more, MORE_FOCUS) )
              hurt_person(ch,victim,500000,TRUE);
      else hurt_person(ch,victim,350000,TRUE);
}

bool is_combo( CHAR_DATA *ch, CHAR_DATA *victim, int move_num )
{
   if ( move_num == 8 )
      {
         /* Soshuu Jinn - Knee, Roundhouse, Sweep, Sidewinder, Sidewinder, Knee, Shin, Roundhouse */
         if ( ch->clist[0] == 8 && ch->clist[1] == 2 && ch->clist[2] == 7
              && ch->clist[3] == 4 && ch->clist[4] == 4 && ch->clist[5] == 8
              && ch->clist[6] == 1 && ch->clist[7] == 2 )
            {
                do_soshuu_jinn(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 42 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 42;

                return TRUE;
            }

         /* Mangetsu Zan - Sweep, Sidewinder, Sweep, Knee, Roundhouse, Sidewinder, Knee, Headbutt */
         if ( ch->clist[0] == 7 && ch->clist[1] == 4 && ch->clist[2] == 7
              && ch->clist[3] == 8 && ch->clist[4] == 2 && ch->clist[5] == 4
              && ch->clist[6] == 8 && ch->clist[7] == 3 )
            {
                do_mangetsu_zan(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 43 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 43;

                return TRUE;
            }

         /* Nanpa - Jab, Elbow, Shin, Elbow, Jab, Sidewinder, Knee, Headbutt */
         if ( ch->clist[0] == 5 && ch->clist[1] == 6 && ch->clist[2] == 1
              && ch->clist[3] == 6 && ch->clist[4] == 5 && ch->clist[5] == 4 
              && ch->clist[6] == 8 && ch->clist[7] == 3 )
            {
                do_nanpa(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 50 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 50;

                return TRUE;
            }
 
         /* Myo Suhaten - Knee, Roundhouse, Sidewinder, Sweep, Roundhouse, Sweep, Elbow, Jab */
         if ( ch->clist[0] == 8 && ch->clist[1] == 2 && ch->clist[2] == 4
              && ch->clist[3] == 7 && ch->clist[4] == 2 && ch->clist[5] == 7 
              && ch->clist[6] == 6 && ch->clist[7] == 5 )
            {
                do_myo_suhaten(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 58 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 58;

                return TRUE;
            }

         /* Gin Sentsu - Elbow, Jab, Roundhouse, Roundhouse, Headbutt, Shin, Sweep, SideWinder */
         if ( ch->clist[0] == 6 && ch->clist[1] == 5 && ch->clist[2] == 2
              && ch->clist[3] == 2 && ch->clist[4] == 3 && ch->clist[5] == 1 
              && ch->clist[6] == 7 && ch->clist[7] == 4 )
            {
                do_gin_sentsu(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 59 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 59;

                return TRUE;
            }

         /* Vulcan Upper - Elbow, Elbow, Sidewinder, Sweep, Shin, Jab, Roundhouse, Elbow */
         if ( ch->clist[0] == 6 && ch->clist[1] == 6 && ch->clist[2] == 4
              && ch->clist[3] == 7 && ch->clist[4] == 1 && ch->clist[5] == 5 
              && ch->clist[6] == 2 && ch->clist[7] == 6 )
            {
                do_vulcan_upper(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 66 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 66;

                return TRUE;
            }
      }

   return FALSE;
}

void show_random_move( CHAR_DATA *ch, CHAR_DATA *victim )
{
   switch( number_range(1,8) )
   {
      default:
      case 1 : act("You SmAsH $N's chin with the palm of your hand as you leap into the air.",ch,NULL,victim,TO_CHAR);
               act("$n SmAsHes your chin with the palm of $s hand as $e leaps into the air.",ch,NULL,victim,TO_VICT);
               act("$n SmAsHes $N's chin with the palm of your hand as you leap into the air.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 2 : act("You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_CHAR);
               act("$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.",ch,NULL,victim,TO_VICT);
               act("$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 3 : act("You SmAsH your head into $N stunning $M.",ch,NULL,victim,TO_CHAR);
               act("$n SmAsHes $s head into you stunning you.",ch,NULL,victim,TO_VICT);
               act("$n SmAsHes $s head into $N stunning $M.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 4 : act("You lean forward as you kick $N viciously in the face.",ch,NULL,victim,TO_CHAR);
               act("$n leans forward as $e kicks you viciously in the face.",ch,NULL,victim,TO_VICT);
               act("$n leans forward as $e kicks $N viciously in the face.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 5 : act("You quickly punch $N hard in the stomach.",ch,NULL,victim,TO_CHAR);
               act("$n quickly punches you hard in the stomach.",ch,NULL,victim,TO_VICT);
               act("$n quickly punches $N hard in the stomach.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 6 : act("You spin around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_CHAR);
               act("$n spins around catching you offguard with an elbow to the face.",ch,NULL,victim,TO_VICT);
               act("$n spins around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 7 : act("You suddenly twirl your foot under $N's feet, tripping $M down.",ch,NULL,victim,TO_CHAR);
               act("$n suddenly twirls $s foot under your feet, tripping you down.",ch,NULL,victim,TO_VICT);
               act("$n suddenly twirls $s foot under $N's feet, tripping $M down.",ch,NULL,victim,TO_NOTVICT);
               break;
      case 8 : act("You ram your knee in $N's guts.",ch,NULL,victim,TO_CHAR);
               act("$n rams $s knee in your guts.",ch,NULL,victim,TO_VICT);
               act("$n rams $s knee in $N's guts.",ch,NULL,victim,TO_NOTVICT);
               break;
   }
}

void do_rising_shin( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_shin( ch, argument );
         return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   one_argument( argument, arg );

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a rising shin to?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 1;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_roundhouse_kick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_roundhouse( ch, argument );
         return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a roundhouse kick to?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 2;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_headbutt( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_headbutt( ch, argument );
         return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a headbutt to?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 3;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_sidewinder_kick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_kick( ch, argument );
         return;
      }

   if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_MONK ) )
      {
          do_kuruda_sidewinder( ch, argument );
          return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a super kick to?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 4;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_jab( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_jab( ch, argument );
         return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a swift punch to?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 5;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_elbow_strike( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_elbow( ch, argument );
         return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to elbow in the face?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 6;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_sweep( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to trip down?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 7;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_knee( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to knee in the stomach?\n\r", ch );
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

   ch->clist[ ch->special_id  ] = 8;
   ch->special_id++;

   if ( ch->special_id < 16 )
      {
         if ( is_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   show_random_move( ch, victim );
}

void do_skid( CHAR_DATA *ch, char *argument )
{
   if ( !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_ASSASSIN) && !IS_CLASS(ch, CLASS_NINJA)
        && !IS_CLASS(ch, CLASS_SHADOW) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->special_id == 0 )
      {
         stc("But you aren't in the middle of a combo.\n\r",ch);
         return;
      }

   ch->wait = 4;
   ch->special_id = 0;
   ch->special_id_2 = 0;

   act("You suddenly skid across the ground gasping for breath.",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly skids across the ground gasping for breath.",ch,NULL,NULL,TO_ROOM);
}

void do_guidance( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->monkab[BODY] < 1 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->newbits, NEW_POWER) )
      {
         REMOVE_BIT(ch->newbits, NEW_POWER);
         send_to_char( "God stop focusing your attacks.\n\r", ch );
      } 
      else {
              if ( ch->move < 5000000 )
                 {
                    stc("You are too tired to perform this technique.\n\r",ch);
                    return;
                 }

              SET_BIT(ch->newbits, NEW_POWER);
              send_to_char( "Your attacks are now being focused in by God.\n\r", ch );
           }
}

void takeoff_stance( CHAR_DATA *ch )
{
   REMOVE_BIT( ch->more3, MORE3_MANTIS );
   REMOVE_BIT( ch->more3, MORE3_WOLF );
   REMOVE_BIT( ch->more3, MORE3_TIGER );
   REMOVE_BIT( ch->more3, MORE3_CRANE );
   REMOVE_BIT( ch->more3, MORE3_DRAGON );
   REMOVE_BIT( ch->more3, MORE3_FALCON );
   REMOVE_BIT( ch->more3, MORE3_SNAKE );
   REMOVE_BIT( ch->more3, MORE3_HYENA );
}

void do_stagger( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( !IS_SET( ch->cmbt[5], SKL2_STAGGER ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

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

   if ( is_safe( ch, victim ) )
      return;

   if ( IS_SET( victim->cmbt[5], SKL2_SWITCH ) || is_martial( victim ) )
      {
         send_to_char("They dodge your technique with ease.\n\r",ch);
         return;
      }

   act("You swerve around onto your knees as you spread your arms like wings.",ch,NULL,NULL,TO_CHAR);
   act("$n swerves around onto $s knees as $e spreads $s arms like wings.",ch,NULL,NULL,TO_ROOM);
   act("You then concentrate as a mass of energy manifests in front of you.",ch,NULL,NULL,TO_CHAR);
   act("$n then concentrates as a mass of energy manifests in front of $m.",ch,NULL,NULL,TO_ROOM);
   act("You push your fist into $N's chest causing $M to skid back.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s fist into your chest causing you to skid back.",ch,NULL,victim,TO_VICT);
   act("$n pushes $s fist into $N's chest causing $M to skid back.",ch,NULL,victim,TO_NOTVICT);
   act("#cYou collapse onto the ground realising you were destanced.#n",victim,NULL,NULL,TO_CHAR);
   act("#c$n #ccollapses onto the ground realising $e was destanced.#n",victim,NULL,NULL,TO_CHAR);

   takeoff_stance( victim );
}

void do_stance( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         send_to_char("Syntax: #wStance #wTiger#w/#CCrane#w/#RDragon#w/#LFalcon#w/#yMantis#w/#nNone\n\r",ch);
         return;
      }

   if ( !str_cmp( arg, "none" ) )
      {
         if ( !IS_SET( ch->more3, MORE3_CRANE ) && !IS_SET( ch->more3, MORE3_DRAGON )
              && !IS_SET( ch->more3, MORE3_TIGER ) && !IS_SET( ch->more3, MORE3_FALCON )
              && !IS_SET( ch->more3, MORE3_MANTIS ) && !IS_SET( ch->more3, MORE3_WOLF )
              && !IS_SET( ch->more3, MORE3_SNAKE ) && !IS_SET( ch->more3, MORE3_HYENA ) )
            {
               stc("But you aren't in a fighting stance.\n\r",ch);
               return;
            }

         if ( IS_SET( ch->more3, MORE3_CRANE ) )
            {
               act("#CYou take a deep breath as you calm down, relaxing from your fighting stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#C$n #Ctakes a deep breath as $e calms down, relaxing from $s fighting stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_CRANE );
            }

         if ( IS_SET( ch->more3, MORE3_DRAGON ) )
            {
               act("#RYou scowl menacing as you swerve your hands, relaxing from your fighting stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#R$n #Rscowls menacing as $e swerves $s hands, relaxing from $s fighting stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_DRAGON );
            }

         if ( IS_SET( ch->more3, MORE3_TIGER ) )
            {
               act("#wYour eyes stop burning with passion as you relax from your fighting stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#w$n#w's eyes stop burning with passion as $e relaxes from $s fighting stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_TIGER );
            }

         if ( IS_SET( ch->more3, MORE3_WOLF ) )
            {
               act("#oYour eyes stop burning with a fury as you relax from your fighting stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#o$n#o's eyes stop burning with a fury as $e relaxes from $s fighting stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_WOLF );
            }

         if ( IS_SET( ch->more3, MORE3_FALCON ) )
            {
               act("#LYou lower your anger as you relax from your fighting stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#L$n #Llowers $s anger as $e relaxes from $s fighting stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_FALCON );
            }

         if ( IS_SET( ch->more3, MORE3_SNAKE ) )
            {
               act("#gYou straighten your body as you relax your posure.#n", ch, NULL, NULL, TO_CHAR);
               act("#g$n #gstraightens $s body as $e relaxes $s posure.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_SNAKE );
            }

         if ( IS_SET( ch->more3, MORE3_MANTIS ) )
            {
               act("#yYour brush the dust out of your clothing as you relax from your fighting stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#y$n #ybrushes the dust out of $s clothing as $e relaxes from $s fighting stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_MANTIS );
            }

         if ( IS_SET( ch->more3, MORE3_HYENA ) )
            {
               act("#cYour stop grinning as you recompose a serious stance.#n", ch, NULL, NULL, TO_CHAR);
               act("#c$n #cstops grinning as $e recomposes a serious stance.#n", ch, NULL, NULL, TO_ROOM);
               REMOVE_BIT( ch->more3, MORE3_HYENA );
            }
      }
      else if ( !str_cmp( arg, "crane" ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) && !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }

               ch->wait = 12;
            }
            else takeoff_stance( ch );

         act("You tilt forward as you put your right hand behind your back.", ch, NULL, NULL, TO_CHAR);
         act("$n tilts forward as $e puts $s right hand behind $s back.", ch, NULL, NULL, TO_ROOM);
         act("You lean back as you kick rapidly at the air.", ch, NULL, NULL, TO_CHAR);
         act("$n leans back as $e kicks rapidly at the air.", ch, NULL, NULL, TO_ROOM);
         act("You then do a swirling kick around the room as your glasses reflects of the Light.", ch, NULL, NULL, TO_CHAR);
         act("$n then do a swirling kick around the room as $s glasses reflects of the Light.", ch, NULL, NULL, TO_ROOM);
         act("You mutter '#yNuus...#n' as you swing your head to the right, discharding your glasses of you.", ch, NULL, NULL, TO_CHAR);
         act("$n mutter '#yNuus...#n' as $e swings $s head to the right, discharding $s glasses of $m.", ch, NULL, NULL, TO_ROOM);
         act("You mutter '#yShaaa...#n' as you lean back, taking a deep breath.", ch, NULL, NULL, TO_CHAR);
         act("$n mutters '#yShaaa...#n' as $e leans back, taking a deep breath.", ch, NULL, NULL, TO_ROOM);
         act("#CYou are now leaning in the Crane Fighting Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#C$n #Cis now leaning in the Crane Fighting Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_CRANE );
      }
      else if ( !str_cmp( arg, "falcon" ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) && !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }

               ch->wait = 12;
            }
            else takeoff_stance( ch );

         act("You close your eyes as you concentrate on your surroundings.", ch, NULL, NULL, TO_CHAR);
         act("$n closes $s eyes as $e concentrates on $s surroundings.", ch, NULL, NULL, TO_ROOM);
         act("You lean back as you kick high in the air.", ch, NULL, NULL, TO_CHAR);
         act("$n leans back as $e kicks high in the air.", ch, NULL, NULL, TO_ROOM);
         act("You suddenly clench your fist as you open your eyes.", ch, NULL, NULL, TO_CHAR);
         act("$n suddenly clenches $s fist as $e opens $s eyes.", ch, NULL, NULL, TO_ROOM);
         act("Looking evilly around you, you mutter something inaudible as your eyes blazes.", ch, NULL, NULL, TO_CHAR);
         act("Looking evilly around $m, $n mutters something inaudible as $s eyes blazes.", ch, NULL, NULL, TO_ROOM);
         act("#LYou are now tilting in the Shinarui Ninjitsu Fighting Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#L$n #Lis now tilting in the Shinarui Ninjitsu Fighting Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_FALCON );
      }
      else if ( !str_cmp( arg, "snake" ) )
      {
         if ( !IS_CLASS( ch, CLASS_DEMON ) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( ch->pkill < 2 )
            {
               stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
               return;
            }

         if ( !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }
            }
            else takeoff_stance( ch );

         ch->wait = 12;

         act("Your eyes sparkle with unholy intelligence as you crouch forward on one knee.", ch, NULL, NULL, TO_CHAR);
         act("$n's eyes sparkle with unholy intelligence as $e crouches forward on one knee.", ch, NULL, NULL, TO_ROOM);
         act("You then start chuckling as you tilt your back in a horrendous arc.", ch, NULL, NULL, TO_CHAR);
         act("$n then starts chuckling as $e tilts $s back in a horrendous arc.", ch, NULL, NULL, TO_ROOM);
         act("You shape your hands into snake fangs as you sway from side to side.", ch, NULL, NULL, TO_CHAR);
         act("$n shapes $s hands into snake fangs as $e sways from side to side.", ch, NULL, NULL, TO_ROOM);
         act("#gYou are now withering back and forth in the forbidden Snake Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#g$n #gis now withering back and forth in the forbidden Snake Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_SNAKE );
      }
      else if ( !str_cmp( arg, "dragon" ) )
      {
         CHAR_DATA *vch, *vch_next;

         if ( !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) && !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }

               ch->wait = 12;
            }
            else takeoff_stance( ch );

         act("You yell '#yMueit!!!#n' as you deafen the room with a loud clap!", ch, NULL, NULL, TO_CHAR);
         act("$n yells '#yMueit!!!#n' as $e deafens the room with a loud clap!", ch, NULL, NULL, TO_ROOM);
         act("You yell '#yAii!!!#n' as you deafen the room with a loud clap!", ch, NULL, NULL, TO_CHAR);
         act("$n yells '#yAii!!!#n' as $e deafens the room with a loud clap!", ch, NULL, NULL, TO_ROOM);
         act("Bracing yourself, you yell '#yAhhhh!!!#n' as you bring your hands together with a ShAtTeRiNg ClAsP!!!", ch, NULL, NULL, TO_CHAR);
         act("Bracing $mself, $n yells '#yAhhhh!!!#n' as $e brings $s hands together with a ShAtTeRiNg ClAsP!!!", ch, NULL, NULL, TO_ROOM);
         act("Your body trembles with rage as you SmAsH your foot onto the ground!!!", ch, NULL, NULL, TO_CHAR);
         act("$n's body trembles with rage as $e SmAsHeS $s foot onto the ground!!!", ch, NULL, NULL, TO_ROOM);
         act("#RYou scowl loudly as you crouch into the Dragon Fighting Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#R$n #Rscowls loudly as $e crouches into the Dragon Fighting Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_DRAGON );
      }
      else if ( !str_cmp( arg, "mantis" ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) && !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }

               ch->wait = 12;
            }
            else takeoff_stance( ch );

         act("You tilt your fingers viciously as you swerve your hands around around.", ch, NULL, NULL, TO_CHAR);
         act("$n tilts $s fingers viciously as $e swerves $s hands around around.", ch, NULL, NULL, TO_ROOM);
         act("You suddenly bend your legs apart as you tighten your arms.", ch, NULL, NULL, TO_CHAR);
         act("$n suddenly bends $s legs apart as $e tighten $s arms.", ch, NULL, NULL, TO_ROOM);
         act("Moving back and forth on the spot, you close your eyes as you rubs your hands.", ch, NULL, NULL, TO_CHAR);
         act("Moving back and forth on the spot, $n closes $s eyes as $e rubs $s hands.", ch, NULL, NULL, TO_ROOM);
         act("You mutter '#yZu noi tu eiji sanada...#n' as you crouch on your knees.", ch, NULL, NULL, TO_CHAR);
         act("$n mutters '#yZu noi tu eiji sanada...#n' as $e crouches on $s knees.", ch, NULL, NULL, TO_ROOM);
         act("#yYou #ygrin softly as you start swerving your hands in the Mantis Fighting Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#y$n #ygrins softly as $e starts swerving $s hands in the Mantis Fighting Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_MANTIS );
      }
      else if ( !str_cmp( arg, "tiger" ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) && !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }

               ch->wait = 12;
            }
            else takeoff_stance( ch );

         act("You swerve your hand as you softly close your eyes.", ch, NULL, NULL, TO_CHAR);
         act("$n swerves $s hand as $e softly closes $s eyes.", ch, NULL, NULL, TO_ROOM);
         act("You start snickering softly as your rage builds up within you.", ch, NULL, NULL, TO_CHAR);
         act("$n starts snickering softly as $s rage builds up within $m.", ch, NULL, NULL, TO_ROOM);
         act("Suddenly, you cackle with insanity as your eyes burst wildly with demonic flames!!!", ch, NULL, NULL, TO_CHAR);
         act("Suddenly, $n cackles with insanity as $s eyes burst wildly with demonic flames!!!", ch, NULL, NULL, TO_ROOM);
         act("You do a beautiful roundhouse kick across the air as you punch rapidly in front of you!", ch, NULL, NULL, TO_CHAR);
         act("$n does a beautiful roundhouse kick across the air as $e punches rapidly in front of $m!", ch, NULL, NULL, TO_ROOM);
         act("You beckon an imaginery foe to challenge you as your body bursts with holy flames!", ch, NULL, NULL, TO_CHAR);
         act("$n beckons an imaginery foe to challenge $m as $s body bursts with holy flames!", ch, NULL, NULL, TO_ROOM);
         act("Clencing your hands like claws, you lower yourself slightly swaying back and forth to the beat.", ch, NULL, NULL, TO_CHAR);
         act("Clencing $s hands like claws, $n lowers $mself slightly swaying back and forth to the beat.", ch, NULL, NULL, TO_ROOM);
         act("#wYou are now swaying in the Tiger Fighting Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#w$n #wis now swaying in the Tiger Fighting Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_TIGER );
      }
      else if ( !str_cmp( arg, "wolf" ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) && !IS_CLASS( ch, CLASS_NINJA ) && !IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            {
               if ( IS_SET( ch->more3, MORE3_CRANE ) || IS_SET( ch->more3, MORE3_DRAGON )
                    || IS_SET( ch->more3, MORE3_TIGER ) || IS_SET( ch->more3, MORE3_FALCON ) 
                    || IS_SET( ch->more3, MORE3_MANTIS ) || IS_SET( ch->more3, MORE3_WOLF )
                    || IS_SET( ch->more3, MORE3_SNAKE ) || IS_SET( ch->more3, MORE3_HYENA ) )
                  {
                     stc("But you are already in a fighting stance.\n\r",ch);
                     return;
                  }

               ch->wait = 12;
            }
            else takeoff_stance( ch );

         act("You hop quickly a few times as you harness your powers from within.", ch, NULL, NULL, TO_CHAR);
         act("$n hops quickly a few times as $e harnesses $s powers from within.", ch, NULL, NULL, TO_ROOM);
         act("You then yell '#yHai#n as you punch wildly to your right and go in your battle pose.", ch, NULL, NULL, TO_CHAR);
         act("$n then yells '#yHai#n as $e punches wildly to $s right and go in $s battle pose.", ch, NULL, NULL, TO_ROOM);
         act("#oYou are now swaying in the Wolf Fighting Style.#n", ch, NULL, NULL, TO_CHAR);
         act("#o$n #ois now swaying in the Wolf Fighting Style.#n", ch, NULL, NULL, TO_ROOM);
         SET_BIT( ch->more3, MORE3_WOLF );
      }
      else send_to_char("Syntax: #wStance #wTiger#w/#CCrane#w/#RDragon#w/#LFalcon#w/#yMantis#w/#oWolf#w/#nNone\n\r",ch);
}

void do_twirl( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

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

   if ( ch->ctimer[2] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( is_safe(ch,victim) )
      return;

   act("You mutter '#yNus nira doraden...#n' as you unsheath your fans.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yNus nira doraden...#n' as $e unsheaths $s fans.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly spread open your fans as you crouch on your knees, arms folded.",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly spreads open $s fans as $e crouches on $s knees, arms folded.",ch,NULL,NULL,TO_ROOM);
   act("Swirling your fan across $N's chest, you spin around gliding the other fan through $S neck.",ch,NULL,victim,TO_CHAR);
   act("Swirling $s fan across your chest, $n spin around gliding the other fan through your neck.",ch,NULL,victim,TO_VICT);
   act("Swirling $s fan across $N's chest, $n spin around gliding the other fan through $S neck.",ch,NULL,victim,TO_NOTVICT);
   act("You then stand erect as you wave your fans in a circular motion.",ch,NULL,NULL,TO_CHAR);
   act("$n then stands erect as $e waves $s fans in a circular motion.",ch,NULL,NULL,TO_ROOM);
   act("Catching $N offguard with a kick, you twirl your fan through $S throat while gliding the other one aside.",ch,NULL,victim,TO_CHAR);
   act("Catching you offguard with a kick, $n twirls $s fan through your throat while gliding the other one aside.",ch,NULL,victim,TO_VICT);
   act("Catching $N offguard with a kick, $n twirls $s fan through $S throat while gliding the other one aside.",ch,NULL,victim,TO_NOTVICT);
   act("Crouching forward, you glide your other fan through $N's chest as your fans start glowing with energy.",ch,NULL,victim,TO_CHAR);
   act("Crouching forward, $n glides $s other fan through your chest as $s fans start glowing with energy.",ch,NULL,victim,TO_VICT);
   act("Crouching forward, $n glides $s other fan through $N's chest as $s fans start glowing with energy.",ch,NULL,victim,TO_NOTVICT);
   act("You move with a blur as you swirl both of your fans across $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n moves with a blur as $e swirls both of $s fans across your chest!",ch,NULL,victim,TO_VICT);
   act("$n moves with a blur as $e swirls both of $s fans across $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("You scream loudly as the fans discharge MaSsIvE energy through your veins!",victim,NULL,NULL,TO_CHAR);
   act("$n screams loudly as the fans discharge MaSsIvE energy through $s veins!",victim,NULL,NULL,TO_ROOM); 

   if ( number_percent() > 49 && !IS_SET( victim->more, MORE_HEX ) )
      SET_BIT( victim->more, MORE_HEX );

   ch->ctimer[2] = 4;
   ch->wait = 6;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;

   hurt_person( ch, victim, 500000, TRUE );

   act("Folding your fans, you lean forward placing your hands on your waist.",ch,NULL,NULL,TO_CHAR);
   act("Folding $s fans, $n leans forward placing $s hands on $s waist.",ch,NULL,NULL,TO_ROOM);
   act("Posing glumly, you mutter '#yNora sui Shin sai...#n'",ch,NULL,NULL,TO_CHAR);
   act("Posing glumly, $n mutters '#yNora sui Shin sai...#n'",ch,NULL,NULL,TO_ROOM);
}

void do_butterfly( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj = NULL;

   argument = one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_CRANE ) )
      {
         stc("But you aren't fighting in a crane stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to chop up?\n\r",ch);
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

   if ( ch->class_2 == 0 )
      if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      {
         stc("You need a sheathed sword on your back to perform this technique.\n\r",ch);
         return;
      }

   if ( is_safe(ch,victim) )
      return;

   act("You dash towards $N SmAsHiNg your palm up $S chin!",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you SmAsHiNg $s palm up your chin!",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N SmAsHiNg $s palm up $S chin!",ch,NULL,victim,TO_NOTVICT);
   act("You spin around as you strike $N's face viciously with your fist.",ch,NULL,victim,TO_CHAR);
   act("$n spins around as $e strikes your face viciously with $s fist.",ch,NULL,victim,TO_VICT);
   act("$n spins around as $e strikes $N's face viciously with $s fist.",ch,NULL,victim,TO_NOTVICT);
   act("A spiral of magical leaves flies of your hand as $N reels back from the blow.",ch,NULL,victim,TO_CHAR);
   act("A spiral of magical leaves flies of $n's hand as you reel back from the blow.",ch,NULL,victim,TO_VICT);
   act("A spiral of magical leaves flies of $n's hand as $N reels back from the blow.",ch,NULL,victim,TO_NOTVICT);
   act("You then lean back thrusting your foot upwards $N's face as you unsheath your sword!",ch,NULL,victim,TO_CHAR);
   act("$n then leans back thrusting $s foot upwards your face as $e unsheathes $s sword!",ch,NULL,victim,TO_VICT);
   act("$n then leans back thrusting $s foot upwards $N's face as $e unsheathes $s sword!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yNira...#n' as you twirl your sword across $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yNira...#n' as $e twirls $s sword across your chest!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yNira...#n' as $e twirls $s sword across $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("You then flip across the room SmAsHiNg your foot into $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n then flip across the room SmAsHiNg $s foot into your face!",ch,NULL,victim,TO_VICT);
   act("$n then flip across the room SmAsHiNg $s foot into $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yDania...#n' as you kneel down whirling your sword across $N's legs!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yDania...#n' as $e kneels down whirling $s sword across your legs!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yDania...#n' as $e kneels down whirling $s sword across $N's legs!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#ySei...#n' as you push your palm viciously into $N's stomach.",ch,NULL,victim,TO_CHAR);
   act("$n mutter '#ySei...#n' as $e pushes $s palm viciously into your stomach.",ch,NULL,victim,TO_VICT);
   act("$n mutter '#ySei...#n' as $e pushes $s palm viciously into $N's stomach.",ch,NULL,victim,TO_NOTVICT);
   act("You then tilt backwards flipping your foot across $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n then tilts backwards flipping $s foot across your face!",ch,NULL,victim,TO_VICT);
   act("$n then tilts backwards flipping $s foot across $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yLitu...#n' as you twirl your sword through $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yLitu...#n' as $e twirls $s sword through your chest!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yLitu...#n' as $e twirls $s sword through $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yNi...#n' as you slash $N across the face!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yNi...#n' as $e slashes you across the face!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yNi...#n' as $e slashes $N across the face!",ch,NULL,victim,TO_NOTVICT);
   act("Your hand pusles with energy as you strike $N viciously in the mouth!",ch,NULL,victim,TO_CHAR);
   act("$n's hand pusles with energy as $e strikes you viciously in the mouth!",ch,NULL,victim,TO_VICT);
   act("$n's hand pusles with energy as $e strikes $N viciously in the mouth!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yZora Ti Lumana Tei!!!#n' as you spin around leaping into the air!",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yZora Ti Lumana Tei!!!#n' as $e spins around leaping into the air!",ch,NULL,NULL,TO_ROOM);
   act("You twirl your blurry sword across $N's body searing $S blood across the vicinity!",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s blurry sword across your body searing your blood across the vicinity!",ch,NULL,victim,TO_VICT);
   act("$n twirls $s blurry sword across $N's body searing $S blood across the vicinity!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your glowing fist across $N's face making $M dizzy from the impact!",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s glowing fist across your face making you dizzy from the impact!",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s glowing fist across $N's face making $M dizzy from the impact!",ch,NULL,victim,TO_NOTVICT);
   act("You twirl your blurry sword across $N's body searing $S blood across the vicinity!",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s blurry sword across your body searing your blood across the vicinity!",ch,NULL,victim,TO_VICT);
   act("$n twirls $s blurry sword across $N's body searing $S blood across the vicinity!",ch,NULL,victim,TO_NOTVICT);

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

   act("You SmAsH your glowing fist across $N's face making $M dizzy from the impact!",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s glowing fist across your face making you dizzy from the impact!",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s glowing fist across $N's face making $M dizzy from the impact!",ch,NULL,victim,TO_NOTVICT);
   act("You twirl your blurry sword across $N's body searing $S blood across the vicinity!",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s blurry sword across your body searing your blood across the vicinity!",ch,NULL,victim,TO_VICT);
   act("$n twirls $s blurry sword across $N's body searing $S blood across the vicinity!",ch,NULL,victim,TO_NOTVICT);

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

   act("You SmAsH your glowing fist across $N's face making $M dizzy from the impact!",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s glowing fist across your face making you dizzy from the impact!",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s glowing fist across $N's face making $M dizzy from the impact!",ch,NULL,victim,TO_NOTVICT);
   act("The magical leaves encircle $N's body as you land back onto the ground.",ch,NULL,victim,TO_CHAR);
   act("The magical leaves encircle your body as $n lands back onto the ground.",ch,NULL,victim,TO_VICT);
   act("The magical leaves encircle $N's body as $n lands back onto the ground.",ch,NULL,victim,TO_NOTVICT);
   act("You crouch on one knee sheathing your sword as $N's body bursts with divine flames.",ch,NULL,victim,TO_CHAR);
   act("$n crouches on one knee sheathing $s sword as your body bursts with divine flames.",ch,NULL,victim,TO_VICT);
   act("$n crouches on one knee sheathing $s sword as $N's body bursts with divine flames.",ch,NULL,victim,TO_NOTVICT); 

   ch->wait = 8;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 200 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = UPPERCUT;

   hurt_person( ch, victim, 2000000, TRUE );
}

void do_swan_dive( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_CRANE ) )
      {
         stc("But you aren't fighting in a crane stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to devastate with holy energy?\n\r",ch);
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

   act("You leap into the air as you spread your arms apart!",ch,NULL,NULL,TO_CHAR);
   act("$n leaps into the air as $e spreads $s arms apart!",ch,NULL,NULL,TO_ROOM);

   if ( ch->in_room != victim->in_room )
      {
         char_from_room( ch );
         char_to_room( ch, victim->in_room );
      }

   act("Circling around the vicinity, you suddenly dive towards $N!",ch,NULL,victim,TO_CHAR);
   act("Circling around the vicinity, $n suddenly dives towards you!",ch,NULL,victim,TO_VICT);
   act("Circling around the vicinity, $n suddenly dives towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("You ram your body powerfully into $N dragging $M onto the ground!",ch,NULL,victim,TO_CHAR);
   act("$n rams $s body powerfully into you dragging you onto the ground!",ch,NULL,victim,TO_VICT);
   act("$n rams $s body powerfully into $N dragging $M onto the ground!",ch,NULL,victim,TO_NOTVICT);
   act("Shards of light emits of your body as you grasp $N by the throat.",ch,NULL,victim,TO_CHAR);
   act("Shards of light emits of $n's body as $e grasps you by the throat.",ch,NULL,victim,TO_VICT);
   act("Shards of light emits of $n's body as $e grasps $N by the throat.",ch,NULL,victim,TO_NOTVICT);
   act("Your skull flashes from the intensity as MaSsIvE energy radiates off you.",ch,NULL,NULL,TO_CHAR);
   act("$n's skull flashes from the intensity as MaSsIvE energy radiates off $m.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yYAAAAAAAAAAIIIIIIII!!#n' as a burst of energy SoArS across the room!!",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yYAAAAAAAAAAIIIIIIII!!#n' as a burst of energy SoArS across the room!!",ch,NULL,NULL,TO_ROOM);

   ch->wait = 8;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 300 );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( !IS_NPC( vch ) || is_safe( ch, vch ) ) 
         continue;

      ch->ele_attack = ATTACK_HOLY;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
      ch->attack_focus = UPPERCUT;

      act("You scream as the burst of energy sears you!!!",vch,NULL,NULL,TO_CHAR);
      act("$n screams as the burst of energy sears $m!!!",vch,NULL,NULL,TO_ROOM);
      hurt_person( ch, vch, 3500000, TRUE );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
         {
            if ( ch->in_room->area != d->character->in_room->area )
               {
                  act("You hear someone screaming '#yYAAAAAAAAAAIIIIIIII!!#n' in the distance.",d->character,NULL,NULL,TO_CHAR);
                  continue;
               }

            if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
               continue;

            ch->ele_attack = ATTACK_HOLY;
            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_var = SOUL;
            ch->attack_focus = UPPERCUT;
            act("You hear someone screaming '#yYAAAAAAAAAAIIIIIIII!!#n' as a burst of energy SeArS through you!!",d->character,NULL,NULL,TO_CHAR);
            act("$n hears someone screaming '#yYAAAAAAAAAAIIIIIIII!!#n' as a burst of energy SeArS through $m!!",d->character,NULL,NULL,TO_CHAR);
            hurt_person( ch, d->character, 3500000, TRUE );
	   }
   }

   act("You collapse onto the ground totally drained of energy.",ch,NULL,NULL,TO_CHAR);
   act("$n collapses onto the ground totally drained of energy.",ch,NULL,NULL,TO_ROOM);
}

void do_psycho_ball( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_CRANE ) )
      {
         stc("But you aren't fighting in a crane stance.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to blast?\n\r",ch);
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

   act("You lower your head as you place your hands behind your back.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s head as $e places $s hands behind $s back.",ch,NULL,NULL,TO_ROOM);
   act("You silently walk towards $N as your eyes darken with rage.",ch,NULL,victim,TO_CHAR);
   act("$n silently walks towards you as $s eyes darken with rage.",ch,NULL,victim,TO_VICT);
   act("$n silently walks towards $N as $s eyes darken with rage.",ch,NULL,victim,TO_NOTVICT);
   act("Your hands start glowing with chi energy as you focus on $N.",ch,NULL,victim,TO_CHAR);
   act("$n's hands start glowing with chi energy as $e focuses on you.",ch,NULL,victim,TO_VICT);
   act("$n's hands start glowing with chi energy as $e focuses on $N.",ch,NULL,victim,TO_NOTVICT);
   act("You grin evilly as you say softly '#yYai!#n' releasing the energy blast towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n grins evilly as $e says softly '#yYai!#n' releasing the energy blast towards you!",ch,NULL,victim,TO_VICT);
   act("$n grins evilly as $e says softly '#yYai!#n' releasing the energy blast towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("You stagger from the blow as everything goes white!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers from the blow as everything goes white!",victim,NULL,NULL,TO_ROOM);

   ch->wait = 18;
   ch->move -= 1500000;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 500000, TRUE );

   act("You smile softly, tilting your arms apart as you glide a hand through your hair.",ch,NULL,NULL,TO_CHAR);
   act("$n smiles softly, tilting $s arms apart as $e glides a hand through $s hair.",ch,NULL,NULL,TO_ROOM);
}

void do_psycho_rush( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_CRANE ) )
      {
         stc("But you aren't fighting in a crane stance.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ch->ctimer[3] > 0 )
      {
         send_to_char("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
      {
         send_to_char("It is #c#zimpossible#n to leave from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
         return;
      } 

   if ( arg[0] == '\0' )
      {
         send_to_char("Who do you wish to walk to via the psychic planes?\r\n", ch);
         return;
      }
	
   if ( ( victim = get_char_area( ch, arg ) ) == NULL || victim == ch || victim->in_room == NULL
        || ch->in_room == NULL )
      {
         send_to_char( "You attempt to walk through the psychic planes, but fail.\n\r", ch );
         return;
      } 

   if ( victim->in_room == ch->in_room )
      {
         send_to_char( "But you are already there!\n\r", ch );
         return;
      }

   if ( victim->fight_timer == 0 )
      {
         send_to_char("But they aren't facing battle dark one.\n\r", ch );
         return;
      }

   ch->ctimer[3] = 2;

   if ( !is_martial( victim ) )
      victim->wait = 12;

   stop_fighting( ch, TRUE );

   act("You mutter '#yNika...#n' as you move like a blur into the distance...",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yNika...#n' as $e moves like a blur into the distance...",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   do_look(ch,"");

   act("You then say '#ySui!#n' as you slam yourself into $N's body.",ch,NULL,victim,TO_CHAR);
   act("$n then say '#ySui!#n' as $e slams $mself into $N's body.",ch,NULL,victim,TO_VICT);
   act("$n then say '#ySui!#n' as $e slams $mself into $N's body.",ch,NULL,victim,TO_NOTVICT);

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );
}

void do_secret_fist( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA af;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->ctimer[7] > 0 )
      {
         send_to_char( "You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->move < 1500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

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

   if ( is_safe( ch, victim ) )
      return;

   act("You swerve around onto your knees as you spread your arms like wings.",ch,NULL,NULL,TO_CHAR);
   act("$n swerves around onto $s knees as $e spreads $s arms like wings.",ch,NULL,NULL,TO_ROOM);
   act("You then latch out at a vital part of $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n then latches out at a vital part of your body!",ch,NULL,victim,TO_VICT);
   act("$n then latches out at a vital part of $N's body!",ch,NULL,victim,TO_NOTVICT);

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

   ch->ctimer[7] = 6;
   ch->wait = 4;
   ch->move -= 1500000;
}

void do_rising_storm( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *victim_next;
   int dam = 1000000;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_DRAGON ) )
      {
         stc("But you aren't fighting in a dragon stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 12;
   SET_BIT( ch->arena, AFF2_DM );

   if ( get_stock( ch ) >= 300 )
      {
         subtract_stock( ch, 300 );
         dam = 3500000;
      }
      else if ( get_stock( ch ) >= 200 )
      {
         subtract_stock( ch, 200 );
         dam = 2000000;
      }
      else subtract_stock( ch, 100 );

   act("You mutter '#yRising...#n' as you raise your hands into the air.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yRising...#n' as $e raises $s hands into the air.",ch,NULL,NULL,TO_ROOM);
   act("Spirals of dark energy converges into your hands as your face twitches with pain.",ch,NULL,NULL,TO_CHAR);
   act("Spirals of dark energy converges into $n's hands as $s face twitches with pain.",ch,NULL,NULL,TO_ROOM);
   act("Your feet start sinking into the earth as the corrupted energy begins to take over you.",ch,NULL,NULL,TO_CHAR);
   act("$n's feet start sinking into the earth as the corrupted energy begins to take over $m.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly scream '#ySTORM!!!#n' as you DrIvE your fists into the earth!",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly screams '#ySTORM!!!#n' as $e DrIvEs $s fists into the earth!",ch,NULL,NULL,TO_ROOM);
      
   for ( victim = ch->in_room->people; victim != NULL; victim = victim_next )
   {
      victim_next = victim->next_in_room;

      if ( is_safe( ch, victim ) )
         continue;

      act( "#eThe #Pevil energy #edisrupt your flesh as you are RiPpEd away!!!#n", victim, NULL, NULL, TO_CHAR );
      act( "#eThe #Pevil energy #edisrupt $n#e's flesh as $e is RiPpEd away!!!#n", victim, NULL, NULL, TO_ROOM );

      if ( dam != 3500000 )
         special_slam( victim, number_range( 0, 3 ) );

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = BLAST;
      ch->attack_focus = UPPERCUT;
      hurt_person( ch, victim, dam, TRUE );

      if ( dam != 3500000 || victim == NULL || victim->in_room == NULL )
         continue;

      victim->air_timer = number_range( 2, 3 );
      stop_fighting( victim, TRUE );
   }
}

void do_heavenly_punch( CHAR_DATA *ch, char *argument )
{
   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_MANTIS ) )
      {
         stc("But you aren't fighting in a mantis stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_NOMOVE ) )
      {
         send_to_char( "The room is already surrounded by an antimovement shield.\n\r", ch );
         return;
      }

   ch->wait = 18;
   SET_BIT( ch->in_room->room_flags, ROOM_NOMOVE );
   subtract_stock( ch, 200 );

   act("You punch the air in front of you viciously as pitch black energy manifests into existence.",ch,NULL,NULL,TO_CHAR);
   act("$n punches the air in front of $m viciously as pitch black energy manifests into existence.",ch,NULL,NULL,TO_ROOM);
   act("You keep on punching the air in front of you as the ball of energy grows bigger and bigger.",ch,NULL,NULL,TO_CHAR);
   act("$n keeps on punching the air in front of $m as the ball of energy grows bigger and bigger.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly the ball flaShEs towards the center of the room as it pulses softly.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly the ball flaShEs towards the center of the room as it pulses softly.",ch,NULL,NULL,TO_ROOM);
   act("#wStrange #Pshards #wof #ylight #wradiate from the #Penergy #was it create an #C#zantimovement#n #wshield.#n",ch,NULL,NULL,TO_CHAR);
   act("#wStrange #Pshards #wof #ylight #wradiate from the #Penergy #was it create an #C#zantimovement#n #wshield.#n",ch,NULL,NULL,TO_ROOM);
}

void do_empty_blow( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_FALCON ) )
      {
         stc("But you aren't fighting in a falcon stance.\n\r",ch);
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc("You can't execute this technique in the heat of battle.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to strike?\n\r",ch);
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

   if ( ch->in_room != victim->in_room )
      {
         char_from_room( ch );
         char_to_room( ch, victim->in_room );
      }

   act("You twist your body around as you leap in front of $N subtlely.",ch,NULL,victim,TO_CHAR);
   act("$n twists $s body around as $e leaps in front of you subtlely.",ch,NULL,victim,TO_VICT);
   act("$n twists $s body around as $e leaps in front of $N subtlely.",ch,NULL,victim,TO_NOTVICT);
   act("You then open your palm as you push it viciously into $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n then opens $s palm as $e pushes it viciously into your chest!",ch,NULL,victim,TO_VICT);
   act("$n then opens $s palm as $e pushes it viciously into $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("You crouch on your knees as you whisper '#yChooooiii...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n crouches on $s knees as $e whispers '#yChooooiii...#n'",ch,NULL,NULL,TO_ROOM);
   act("Suddenly a powerful force of invisible energy flings you across the room!",ch,NULL,NULL,TO_CHAR);
   act("Suddenly a powerful force of invisible energy flings $n across the room!",ch,NULL,NULL,TO_ROOM);

   ch->wait = 8;
   ch->move -= 1500000;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_monk_fade( CHAR_DATA *ch, char *argument )
{
   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_FALCON ) )
      {
         stc("But you aren't fighting in a falcon stance.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
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
         act( "#w$n #egoes on $s feet as $e flickers from the #Pshadows#e.#n", ch, NULL, NULL, TO_ROOM );
         send_to_char( "#wYou #ego on your feet as you flicker from the #Pshadows#e.#n\n\r", ch );
      }
      else {
              act( "#w$n #ecrouches on $s knees silently as $e flickers into the #Pshadows#e.#n", ch, NULL, NULL, TO_ROOM );
              send_to_char( "#wYou #ecrouch silently on your knees as you flicker into the #Pshadows#e.#n\n\r", ch );
              SET_BIT( ch->act, PLR_WIZINVIS );
           }
}

void do_hishoken_rush( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_FALCON ) )
      {
         stc("But you aren't fighting in a falcon stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to chop up?\n\r",ch);
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

   act("You charge into $N SmAsHiNg your elbow into $S face!",ch,NULL,victim,TO_CHAR);
   act("$n charges into you SmAsHiNg $s elbow into your face!",ch,NULL,victim,TO_VICT);
   act("$n charges into $N SmAsHiNg $s elbow into $S face!",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly twirl around spinning your foot into $N face!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly twirls around spinning $s foot into your face!",ch,NULL,victim,TO_VICT);
   act("$n suddenly twirls around spinning $s foot into $N face!",ch,NULL,victim,TO_NOTVICT);
   act("You push your palm into $N's chest viciously as you crouch on your knees.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s palm into your chest viciously as $e crouches on $s knees.",ch,NULL,victim,TO_VICT);
   act("$n pushes $s palm into $N's chest viciously as $e crouches on $s knees.",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#wAwwwhhh....#n' as you focus your chi on your fist, creating a ball of energy.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#wAwwwhhh....#n' as $e focuses $s chi on $s fist, creating a ball of energy.",ch,NULL,NULL,TO_ROOM);
   act("You open your palm as the ball sears $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n opens $s palm as the ball sears your chest!",ch,NULL,victim,TO_VICT);
   act("$n opens $s palm as the ball sears $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("MaSsIvE explosions fill the air as the energy ball RiPs through your body!",victim,NULL,NULL,TO_CHAR);
   act("MaSsIvE explosions fill the air as the energy ball RiPs through $n's body!",victim,NULL,NULL,TO_ROOM);
   act("You yell '#wZani Ken!#n' as you SmAsH your elbow into $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#wZani Ken!#n' as $e SmAsHeS $s elbow into your chest.",ch,NULL,victim,TO_VICT);
   act("$n yells '#wZani Ken!#n' as $e SmAsHeS $s elbow into $N's chest.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly twirl around spinning your foot into $N face!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly twirls around spinning $s foot into your face!",ch,NULL,victim,TO_VICT);
   act("$n suddenly twirls around spinning $s foot into $N face!",ch,NULL,victim,TO_NOTVICT);
   act("You push your palm into $N's chest viciously as you crouch on your knees.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s palm into your chest viciously as $e crouches on $s knees.",ch,NULL,victim,TO_VICT);
   act("$n pushes $s palm into $N's chest viciously as $e crouches on $s knees.",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#wAwwwhhh....#n' as you focus your chi on your fist, creating DeVaStAtInG chi energy.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#wAwwwhhh....#n' as $e focuses $s chi on $s fist, creating DeVaStAtInG chi energy.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#wYEEEEESH!!!#n' as you ReLeAsE a BuRsT of holy energy into $N!",ch,NULL,victim,TO_CHAR);
   act("$n screams '#wYEEEEESH!!!#n' as $e ReLeAsEs a BuRsT of holy energy into you!",ch,NULL,victim,TO_VICT);
   act("$n screams '#wYEEEEESH!!!#n' as $e ReLeAsEs a BuRsT of holy energy into $N!",ch,NULL,victim,TO_NOTVICT);
   act("#wYour #ybody #wfizzes #yfrom #wthe #edestructive #wchi #yas #wyou #yare #wparalyzed #yin #wmidair#y!#n",victim,NULL,NULL,TO_CHAR);
   act("#w$n's #ybody #wfizzes #yfrom #wthe #edestructive #wchi #yas #w$e #yis #wparalyzed #yin #wmidair#y!#n",victim,NULL,NULL,TO_ROOM);

   if ( ch->ctimer[5] == 0 && !is_martial( victim ) )
      victim->wait = 18;

   if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
        && ch->ctimer[9] != 100 ) )
      ch->ctimer[5] = 2;

   ch->wait = 12;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 100 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 1000000, TRUE );
}

void start_innocence( CHAR_DATA *ch, CHAR_DATA *victim )
{
   stop_fighting( victim, TRUE );
   stop_fighting( ch, TRUE );
   ch->position = victim->position = POS_FIGHTING;
   ch->fighting = victim;
   victim->fighting = ch;
   ch->ctimer[8] = 4;
   victim->air_timer = 6;
   ch->ctimer[10] = 0;

   act( "#eYou #Lsuddenly grab #w$N #Las your eyes flashes with #Rrage...#n", ch, NULL, victim, TO_CHAR );
   act( "#e$n #Lsuddenly grabs #wyou #Las $s eyes flashes with #Rrage...#n", ch, NULL, victim, TO_VICT );
   act( "#e$n #Lsuddenly grabs #w$N #Las $s eyes flashes with #Rrage...#n", ch, NULL, victim, TO_NOTVICT );
   act( "#eYou #Lmutter '#yinnocence#L' as you leap with #w$N #Linto the air!", ch, NULL, victim, TO_CHAR );
   act( "#e$n #Lmutters '#yinnocence#L' as $e leaps with #wyou #Linto the air!", ch, NULL, victim, TO_VICT );
   act( "#e$n #Lmutters '#yinnocence#L' as $e leaps with #w$N #Linto the air!", ch, NULL, victim, TO_NOTVICT );
}

void perform_innocence( CHAR_DATA *ch, CHAR_DATA *victim )
{
   victim->air_timer = 0;
   act( "#eYou #LSLAM #w$N #LHARD into the ground as the #wentire #Lvicinity #PTREMBLES#L!!!#n", ch, NULL, victim, TO_CHAR );
   act( "#e$n #LSLAMS #wyou #LHARD into the ground as the #wentire #Lvicinity #PTREMBLES#L!!!#n", ch, NULL, victim, TO_VICT );
   act( "#e$n #LSLAMS #w$N #LHARD into the ground as the #wentire #Lvicinity #PTREMBLES#L!!!#n", ch, NULL, victim, TO_NOTVICT );

   if ( number_percent() < 90 - devil_luck( ch, victim, 5 ) || IS_SET( victim->cmbt[5], SKL2_AMIND ) )
      {
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = BLUNT;
         ch->attack_focus = AERIAL;
         hurt_person( ch, victim, 3500000, TRUE );
      }
      else {
              stop_fighting( victim, TRUE );
              victim->hit = -2000010;
              victim->position = POS_MORTAL;
           }

   act( "#eYou #Lflip back on your knees as you swerve your hands in a #Cninjitsu #Lpose.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n #Lflips back on $s knees as $e swerves $s hands in a #Cninjitsu #Lpose.#n", ch, NULL, NULL, TO_ROOM );
}

void do_innocence( CHAR_DATA *ch, char *argument )
{
   if ( !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_NINJA) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_FALCON ) )
      {
         stc("But you aren't fighting in a falcon stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 24;
   ch->ctimer[10] = 2;
   subtract_stock( ch, 300 );

   act("You breathe deeply as you store your #Cchi#n together, waiting for #wanyone#n to #ystrike#n...",ch,NULL,NULL,TO_CHAR);
   act("$n breathes deeply as $e stores $s #Cchi#n together, waiting for #wanyone#n to #ystrike#n...",ch,NULL,NULL,TO_ROOM);
}

void do_palm_strike( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA af;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_MANTIS ) )
      {
         stc("But you aren't fighting in a mantis stance.\n\r",ch);
         return;
      }

   if ( ch->move < 3500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to slow down?\n\r",ch);
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

   if ( !is_affected( victim, skill_lookup("slow") ) )
      {
         af.type      = skill_lookup("slow");
         af.duration  = 20;
         af.location  = APPLY_NONE;
         af.modifier  = -1;
         af.bitvector = 0;
         affect_to_char( victim, &af );
      }

   act("You push your palm into $N's chin as you mutter '#yShoi...#n'",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s palm into your chin as $e mutters '#yShoi...#n'",ch,NULL,victim,TO_VICT);
   act("$n pushes $s palm into $N's chin as $e mutters '#yShoi...#n'",ch,NULL,victim,TO_NOTVICT);
   act("Your face grimace with pain as flashes of energy surges through $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n's face grimaces with pain as flashes of energy surges through your body!",ch,NULL,victim,TO_VICT);
   act("$n's face grimaces with pain as flashes of energy surges through $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yThoi!#n' as you grasp your arm with your other arm, SmAsHiNg $N away!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yThoi!#n' as $e grasps $s arm with $s other arm, SmAsHiNg you away!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yThoi!#n' as $e grasps $s arm with $s other arm, SmAsHiNg $N away!",ch,NULL,victim,TO_NOTVICT);
   act("You are slammed to the ground from the force of the blow.",victim,NULL,NULL,TO_CHAR);
   act("$n is slammed to the ground from the force of the blow.",victim,NULL,NULL,TO_ROOM);

   if ( !IS_SET( victim->cmbt[5], SKL2_REMINISCE ) && number_percent() > 60 - devil_luck( ch, victim, 10 ) )
      {
         if ( victim->outer_aura != -1 )
            do_aura(victim,"outer none");

         if ( victim->inner_aura != -1 )
            do_aura(victim,"inner none");
      }

   ch->wait = 6;
   ch->move -= 3500000;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_soulbomb( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *victim_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_MANTIS ) )
      {
         stc("But you aren't fighting in a mantis stance.\n\r",ch);
         return;
      }

   if ( ch->move < 5000000 )
      {
         send_to_char( "You don't have enough magical energy to execute this technique.\n\r", ch );
         return;
      }

   if ( ch->hit < 500001 )
      {
         stc("Your body is too weak to sustain the potential damage.\n\r",ch);
         return;
      }

   ch->hit -= 500000;
   ch->move -= 5000000;
   ch->wait = 18;

   act("You yell, '#ySoul Bomb!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n yells, '#ySoul Bomb!#n'",ch,NULL,NULL,TO_ROOM);
   stc("Unlimited energy draws towards your body as you brace yourself.\n\r",ch); 
   act("Unlimited energy draws towards $n's body as $e braces $mself.\n\r",ch,NULL,NULL,TO_ROOM); 
   stc("You tremble violently as your body explodes with unholy energy.\n\r",ch); 
   act("$n trembles violently as $s body explodes with unholy energy.",ch,NULL,NULL,TO_ROOM); 
   stc("The Demonic atomic bomb rapidly expands and engulfs the area with dark energy!\n\r",ch); 
   act("The Demonic atomic bomb rapidly expands and engulfs the area with dark energy!",ch,NULL,NULL,TO_ROOM); 
      
   for ( victim = ch->in_room->people; victim != NULL; victim = victim_next )
   {
      victim_next = victim->next_in_room;

      if ( is_safe( ch, victim ) )
         continue;

      stc("You scream as the dark energy sears away at your flesh.\n\r",victim);
      act("$n screams as the dark energy sears away at $s flesh.",victim,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = BLAST;
      ch->attack_focus = DIRECT;
      hurt_person( ch, victim, 1500000, TRUE );  
   }
}

void do_soulless_touch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_MANTIS ) )
      {
         stc("But you aren't fighting in a mantis stance.\n\r",ch);
         return;
      }

   ch->wait = 18;

   if ( ch->hit > ch->max_hit * 0.1 )
      {
         send_to_char("You don't feel like throwing away your life atm.\n\r",ch);
         return;
      }

   if ( ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 ) || IS_SET(ch->arena, AFF2_INARENA) || ch->pcdata->team != 0 )
      {
         stc("Not in the arena!\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         stc("Whom do you wish to die with?\n\r",ch);
         return;
      }
    
   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( IS_NPC( victim ) )
      {
         send_to_char( "You can't do this to mobs.\n\r", ch );
         return;
      }

   if ( is_safe( ch, victim ) )
      return;

   if ( victim->position >= POS_STUNNED )
      {
         stc( "You can only do this technique to a foe who is in critical condition.\n\r", ch );
         return;
      }

   victim->position = POS_MORTAL;
   do_decapitate( ch, victim->name );

   if ( ch->position <= POS_STUNNED )
      return;

   act( "As you stand over your victim's corpse, your hands start pulsing with dark energy.", ch, NULL, NULL, TO_CHAR );
   act( "As $n stands over $s victim's corpse, $s hands start pulsing with dark energy.", ch, NULL, NULL, TO_ROOM );
   act( "You then snap a vital nerve with your finger as you collapse onto your knees.", ch, NULL, NULL, TO_CHAR );
   act( "$n then snaps a vital nerve with $s finger as $e collapses onto $s knees.", ch, NULL, NULL, TO_ROOM );
   act( "#eAs everything goes #Pbleak#e, you release one last smile...#n", ch, NULL, NULL, TO_CHAR );
   act( "#eAs everything goes #Pbleak#e, $n #ereleases one last smile...#n", ch, NULL, NULL, TO_ROOM );
   act( "#o$n #ois DEAD!!!#n", ch, NULL, NULL, TO_ROOM );

   stop_fighting( ch, TRUE );

   if ( ch->attacker != NULL )
      {
         restore_char( ch->attacker );
         ch->attacker->fight_timer = 0;
      }

   ch->fight_timer = 0;
   raw_kill(ch);
   ch->safe_timer = 5;
   ch->login_timer = 3;

   players_decap++;

   if ( ( players_decap % 25 ) == 0 && players_decap > 25 && !armageddon )
      do_nogang( ch, "" );

   if ( ( players_decap % 30 ) == 0 && arena == FIGHT_CLEAR )
      {
         arena = FIGHT_AUTO;
         do_info( NULL, "#wThe #PA#oren#Pa #gis open for #w#zEverybody#n#g! Use #yFtag #gto Join!!!#n" );
      }

   if ( ( players_decap % 50 ) == 0 && !armageddon )
      {
         start_scavenger();
         do_sinfo( NULL, "#w50 #yitems #ghas been #Pscattered #gthroughout the #Prealm#g! Use #yScavenger #gto Join!!!#n" );
      }
}

void do_neck_breaker( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_MANTIS ) )
      {
         stc("But you aren't fighting in a mantis stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to chop up?\n\r",ch);
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

   act("You leap onto of $N's head as you wrap your legs around $M!",ch,NULL,victim,TO_CHAR);
   act("$n leaps onto of your head as $e wraps $s legs around you!",ch,NULL,victim,TO_VICT);
   act("$n leaps onto of $N's head as $e wraps $s legs around $M!",ch,NULL,victim,TO_NOTVICT);
   act("Your eyes darken with rage as you savagely twist your legs to the right!",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes darken with rage as $e savagely twists $s legs to the right!",ch,NULL,NULL,TO_ROOM);
   act("You stagger back as a LOUD cracking noise shatters the vicinity!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers back as a LOUD cracking noise shatters the vicinity!",victim,NULL,NULL,TO_ROOM);
   act("You then send $N sprawling across the room as you land back silently on your feet.",ch,NULL,victim,TO_CHAR);
   act("$n then sends you sprawling across the room as $e lands back silently on $s feet.",ch,NULL,victim,TO_VICT);
   act("$n then sends $N sprawling across the room as $e lands back silently on $s feet.",ch,NULL,victim,TO_NOTVICT);

   ch->wait =128;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 100 );

   if ( number_percent() > 95 - devil_luck( ch, victim, 5 ) && ch->ctimer[15] == 0
        && !IS_SET( victim->cmbt[5], SKL2_AMIND ) )
      {
         stop_fighting( victim, TRUE );
         victim->hit = -2000010;
         victim->position = POS_MORTAL;
      }
      else {
              ch->attack_type = ATTACK_PHYSICAL;
              ch->attack_var = BLUNT;
              ch->attack_focus = AERIAL;
              hurt_person( ch, victim, 1000000, TRUE );
           }

   act( "You glance around angrilly as you mutter '#yWelcome...#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n glances around angrilly as $e mutters '#yWelcome...#n'", ch, NULL, NULL, TO_ROOM );
}

void do_dragon_roar( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_DRAGON ) )
      {
         stc("But you aren't fighting in a dragon stance.\n\r",ch);
         return;
      }

   if ( nogang != 0 )
      {
         stc("Your teachings forbid you to use this technique when ganging is illegal.\n\r",ch);
         return;
      }

   if ( ch->mana < 7500000 )
      {
         send_to_char( "You don't have enough magical energy to execute this technique.\n\r", ch );
         return;
      }

   ch->mana -= 7500000;
   ch->wait = 18;

   act( "You concentrate on your surroundings as you draw in your breath.", ch, NULL, NULL, TO_CHAR );
   act( "$n concentrates on $s surroundings as $e draws in $s breath.", ch, NULL, NULL, TO_ROOM );
   act( "Your eyes start glowing red as your body trembles with raw power!", ch, NULL, NULL, TO_CHAR );
   act( "$n's eyes start glowing red as $s body trembles with raw power!", ch, NULL, NULL, TO_ROOM );
   act( "A shadowy dragon flashes from your body as a TREMEDOUS roar fills the air!!!", ch, NULL, NULL, TO_CHAR );
   act( "A shadowy dragon flashes from $n's body as a TREMEDOUS roar fills the air!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( IS_NPC( vch ) || vch == ch || ( !is_same_group( ch, vch ) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
           && ch->pcdata->kingdom > 0 ) ) )
         continue;

      act( "You clutch your head in rage as the dragon releases your inner powers!", vch, NULL, NULL, TO_CHAR );
      act( "$n clutches $s head in rage as the dragon releases $s inner powers!", vch, NULL, NULL, TO_ROOM );
      add_grove( vch, NULL, -150 );
   }
}

void do_blazing_fury( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_DRAGON ) )
      {
         stc("But you aren't fighting in a dragon stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 8;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 200 );

   act( "You clench your fists as both of them burst into ferocious flames.", ch, NULL, NULL, TO_CHAR );
   act( "$n clenches $s fists as both of them burst into ferocious flames.", ch, NULL, NULL, TO_ROOM );
   act( "The entire vicinity trembles beneath your feet as you control the energy.", ch, NULL, NULL, TO_CHAR );
   act( "The entire vicinity trembles beneath $n's feet as $e controls the energy.", ch, NULL, NULL, TO_ROOM );
   act( "You then SmAsH your fist into the ground with ALL of your MIGHT!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n then SmAsHeS $s fist into the ground with ALL of $s MIGHT!!!", ch, NULL, NULL, TO_ROOM );
   act( "The area TREMBLES more as a stream of fire flashes across the vicinity!", ch, NULL, NULL, TO_CHAR );
   act( "The area TREMBLES more as a stream of fire flashes towards you!", ch, NULL, NULL, TO_ROOM );
   act( "As lava seeps out of the ground, the fire suddenly EXPLODES into INTENSE FLAMES!!!", ch, NULL, NULL, TO_CHAR );
   act( "As lava seeps out of the ground, the fire suddenly EXPLODES into INTENSE FLAMES!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe(ch, vch) )
         continue;

      if ( !IS_AFFECTED( vch, AFF_FLAMING ) ) 
         SET_BIT( vch->affected_by, AFF_FLAMING );

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_FIRE;
      ch->attack_var = BLAST;
      ch->attack_focus = UPPERCUT;

      if ( ( ( IS_CLASS( vch, CLASS_ANGEL ) || IS_CLASS( vch, CLASS_SHADOW_WAR ) ) && vch->ctimer[8] > 0 )
           || !str_cmp( "N", vch->groove ) || !str_cmp( "A", vch->groove ) )
         hurt_person( ch, vch, 2000000, TRUE );
         else hurt_person( ch, vch, 1250000, TRUE );

      if ( vch == NULL || vch->in_room == NULL )
         continue;

      vch->air_timer = number_range( 2, 3 );
      stop_fighting( vch, TRUE );
   }
}

void do_meteor_barret( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   bool pre_room;
   int direction;
   int count;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_TIGER ) )
      {
         stc("But you aren't fighting in a tiger stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 8;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You tighten your right hand as it starts pulsing with tremedous energy.", ch, NULL, NULL, TO_CHAR );
   act( "$n tightens $s right hand as it starts pulsing with tremedous energy.", ch, NULL, NULL, TO_ROOM );
   act( "Shards of light radiate from your hand as you leap into the air.", ch, NULL, NULL, TO_CHAR );
   act( "Shards of light radiate from $n's hand as $e leap into the air.", ch, NULL, NULL, TO_ROOM );
   act( "The light intensifies as powerful gravity makes it hard for you to turn.", ch, NULL, NULL, TO_CHAR );
   act( "The light intensifies as powerful gravity makes it hard for $n to turn.", ch, NULL, NULL, TO_ROOM );
   act( "A loud hum fills the air as you focus silently on your target...", ch, NULL, NULL, TO_CHAR );
   act( "A loud hum fills the air as $n focuses silently on you...", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly with the ROAR of a tiger you dive straight towards them!!!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly with the ROAR of a tiger $n dives straight towards you!!!", ch, NULL, NULL, TO_ROOM );
   act( "You SMASH the ground with ALL of your MIGHT as the entire area EXPLODES!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n SMASHES the ground with ALL of $s MIGHT as the entire area EXPLODES!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe(ch, vch) )
         continue;

      if ( !IS_SET( vch->more, MORE_BLIND ) )
         SET_BIT( vch->more, MORE_BLIND );

      count = 0;
      direction = number_range( 0, 3 );

      do
      {
         pre_room = special_slam( vch, direction );
         count++;
      }
      while ( count < 5 && pre_room != TRUE );

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_var = BLAST;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 3500000, TRUE );

      if ( vch != NULL && vch->in_room != NULL && vch->position > POS_STUNNED )          
         {
            stop_fighting( vch, TRUE );
            vch->position = POS_STUNNED;
         }
   }
}

void do_pummel( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int count;
   int direction;

   argument = one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_TIGER ) )
      {
         stc("But you aren't fighting in a tiger stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
        && ch->ctimer[9] != 100 ) )
      ch->ctimer[5] = 2;

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   for ( count = 0; count < 24; count++ )
   {
      show_random_move( ch, victim );
   }

   act( "#yYou #wfeel the #PPOWER #wof the #otiger #win you as you send #C$N #wflying with a powerful fist!!!#n", ch, NULL, victim, TO_CHAR );
   act( "#y$n #wfeels the #PPOWER #wof the #otiger #win $m as $e sends #Cyou #wflying with a powerful fist!!!#n", ch, NULL, victim, TO_VICT );
   act( "#y$n #wfeels the #PPOWER #wof the #otiger #win $m as $e sends #C$N #wflying with a powerful fist!!!#n", ch, NULL, victim, TO_NOTVICT );

   if ( ch->ctimer[5] == 0 )
      {
         direction = number_range( 0, 3 );
         special_slam( victim, direction );
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_tiger_rush( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int count;

   argument = one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_TIGER ) )
      {
         stc("But you aren't fighting in a tiger stance.\n\r",ch);
         return;
      }

   if ( ch->move < 7500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to knock down?\n\r",ch);
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

   ch->move -= 7500000;
   ch->wait = 12;

   act( "You release the ROAR of the tiger as you RAM yourself into $N like a blur!!!", ch, NULL, victim, TO_CHAR );
   act( "$n releases the ROAR of the tiger as $e RAMS $mself into you like a blur!!!", ch, NULL, victim, TO_VICT );
   act( "$n releases the ROAR of the tiger as $e RAMS $mself into $N like a blur!!!", ch, NULL, victim, TO_NOTVICT );
   act( "As $N staggers back from the blow, your images SMASH into $M viciously!", ch, NULL, victim, TO_CHAR );
   act( "As you stagger back from the blow, $n's images SMASH into you viciously!", ch, NULL, victim, TO_VICT );
   act( "As $N staggers back from the blow, $n's images SMASH into $M viciously!", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_kou_ken( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam = 3500000;

   argument = one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_DRAGON ) )
      {
         stc("But you aren't fighting in a dragon stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to blast?\n\r",ch);
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
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) ) 
      SET_BIT( victim->affected_by, AFF_FLAMING );

   act("You mutter '#yHaou...#n' as you tilt your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yHaou...#n' as $e tilts $s hands together.",ch,NULL,NULL,TO_ROOM);
   act("A loud humming fills the air as a sphere of energy manifests between your hands.",ch,NULL,NULL,TO_CHAR);
   act("A loud humming fills the air as a sphere of energy manifests between $n's hands.",ch,NULL,NULL,TO_ROOM);
   act("The energy flares with intensity as your eyes darken with concentration.",ch,NULL,NULL,TO_CHAR);
   act("The energy flares with intensity as $n's eyes darken with concentration.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yShou Kou Ken!#n' as you release the HuGe FiReBaLl towards $N!!!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yShou Kou Ken!#n' as $e releases the HuGe FiReBaLl towards you!!!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yShou Kou Ken!#n' as $e releases the HuGe FiReBaLl towards $N!!!",ch,NULL,victim,TO_NOTVICT);

   if ( victim->blocking > 0 )
      {
         dam = 5000000;
         victim->blocking = victim->block_meter = 0;
         act( "#eSuddenly a #yviolent #Rforce #erips your arms apart as you realize your #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_CHAR );
         act( "#eSuddenly a #yviolent #Rforce #erips $n#e's arms apart as $e realizes $s #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_ROOM );
         victim->noblock = UMAX( 1, victim->noblock );
      }

   act("The fireball RoArS into you as you are knocked across the room!",victim,NULL,NULL,TO_CHAR);
   act("The fireball RoArS into $n as $e is knocked across the room!",victim,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );

   act("You crouch on one knee as you spread your arms apart in a fighting stance.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches on one knee as $e spreads $s arms apart in a fighting stance.",ch,NULL,NULL,TO_ROOM);
}

void perform_final_heaven( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int direction;
    bool pre_room;
    int count;

    ch->pcdata->powers[MONK_SHIKI] = 0;
    act("#eRushing towards #y$N #eat the SPEED of #wLIGHT#e, #Cyour #ehand SMASHES $S face with #wHOLIOUS #RPOWER#e!!!#n\n\r",ch,NULL,victim,TO_CHAR);
    act("#eRushing towards #yYOU #eat the SPEED of #wLIGHT#e, #C$n#C's #ehand SMASHES YOUR face with #wHOLIOUS #RPOWER#e!!!#n\n\r",ch,NULL,victim,TO_VICT);
    act("#eRushing towards #y$N #eat the SPEED of #wLIGHT#e, #C$n#C's #ehand SMASHES $S face with #wHOLIOUS #RPOWER#e!!!#n\n\r",ch,NULL,victim,TO_NOTVICT);

    direction = number_range(0,3);
    count = 0;

    do
    {
       pre_room = special_slam( victim, direction );
       count++;
    }
    while ( count < 10 && pre_room != TRUE );

    if ( pre_room == victim->in_room->vnum )
       {
          act("#yYour #Cbody #wSHATTERS #Cas you CRASH into the WALL, BOUNCING off like #PRUBBER#C!#n",victim,NULL,NULL,TO_CHAR);
          act("#y$n#C's body #wSHATTERS #Cas $e CRASHES into the WALL, BOUNCING off like #PRUBBER#C!#n",victim,NULL,NULL,TO_ROOM);
       }
                                          
    if ( !IS_SET( victim->cmbt[5], SKL2_AMIND ) )
       {
          stop_fighting( victim, TRUE );
          victim->hit = -2000010;
          victim->position = POS_MORTAL;
          return;
       }

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 3500000, TRUE );
}

void do_final_heaven( CHAR_DATA *ch, char *argument )
{
   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_SET( ch->more3, MORE3_MANTIS ) )
      {
         stc("But you aren't fighting in a mantis stance.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->fighting == NULL )
      {
         stc( "But you aren't in a fight priest.\n\r", ch );
         return;
      }

   ch->pcdata->powers[MONK_SHIKI] = 1;
   ch->nemesis = 3;
   subtract_stock( ch, 300 );

   act("\n\r#yYou #etake a half-step forward as your right hand starts #ctrembling #ewith #wholy #Rpower#e.#n",ch,NULL,NULL,TO_CHAR);
   act("\n\r#y$n #etakes a half-step forward as $s right hand starts #ctrembling #ewith #wholy #Rpower#e.#n",ch,NULL,NULL,TO_ROOM);
}

void do_body_control( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 50 )
      {
         stc("#wOnly a monk #Cmaster #wcan execute this technique.#n\n\r",ch);
         return;
      }

   if ( ch->alignment == 0 )
      {
         stc("#wYou have a perfect balance between #ygood #wand #eevil #wpriest.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );
   
   if ( !str_cmp( "good", arg ) )
      {
         ch->wait = 12;
         ch->alignment = 1000;
         act( "#yYou #wfocus on your #Cchi #wenergy as #ygoodness #wradiates from within your #Csoul#w.#n", ch, NULL, NULL, TO_CHAR );
         act( "#y$n #wfocuses on $s #Cchi #wenergy as #ygoodness #wradiates from within $s #Csoul#w.#n", ch, NULL, NULL, TO_ROOM );
      }
      else if ( !str_cmp( "evil", arg ) )
              {
                 ch->wait = 12;
                 ch->alignment = -1000;
                 act( "#yYou #efocus on your #Pki #eenergy as #edarkness #eradiates from within your #Psoul#e.#n", ch, NULL, NULL, TO_CHAR );
                 act( "#y$n #efocuses on $s #Pki #eenergy as #edarkness #eradiates from within $s #Psoul#e.#n", ch, NULL, NULL, TO_ROOM );
              }
              else {
                      stc( "#CSyntax - #yBodycontrol #wgood#y/#eevil#n\n\r", ch );
                      return;
                   }
}
