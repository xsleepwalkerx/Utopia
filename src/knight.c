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

void do_moon_wave( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;

  argument = one_argument( argument, arg );

  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 100 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a sword.\n\r", ch );
           return;
        }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to confuse?\n\r",ch);
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

  if ( ch->ctimer[7] == 0 )
     hit_gain( ch, 500000 );

  if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
       && ch->ctimer[9] != 100 ) )
     ch->ctimer[7] = 2;

  ch->wait = 12;
  subtract_stock( ch, 100 );
  SET_BIT( ch->arena, AFF2_DM );

  act("Shimmering energy radiates of you as you raise your sword into the air.",ch,NULL,NULL,TO_CHAR);
  act("Shimmering energy radiates of $n as $e raises $s sword into the air.",ch,NULL,NULL,TO_ROOM);
  act("You dash towards $N DRIVING your sword through $S chest!",ch,NULL,victim,TO_CHAR);
  act("$n dashes towards you DRIVING $s sword through your chest!",ch,NULL,victim,TO_VICT);
  act("$n dashes towards $N DRIVING $s sword through $S chest!",ch,NULL,victim,TO_NOTVICT);
  act("You whisper '#yMoonWave#n' as you RiP your sword through $N's body leaping into the air!",ch,NULL,victim,TO_CHAR);
  act("$n whispers '#yMoonWave#n' as $e RiPs $s sword through your body leaping into the air!",ch,NULL,victim,TO_VICT);
  act("$n whispers '#yMoonWave#n' as $e RiPs $s sword through $N's body leaping into the air!",ch,NULL,victim,TO_NOTVICT);
  act("You scream loudly as a soft beam engulfs your body!",victim,NULL,NULL,TO_CHAR);
  act("$n screams loudly as a soft beam engulfs $s body!",victim,NULL,NULL,TO_ROOM);

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = UPPERCUT;
  ch->ele_attack = ATTACK_LUNAR;
  hurt_person( ch, victim, 1000000, TRUE );

  act("You softly land back on your feet.",ch,NULL,NULL,TO_CHAR);
  act("$n softly lands back on $s feet.",ch,NULL,NULL,TO_ROOM);
}

void do_dark_storm( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   OBJ_DATA *obj;
   int count, direction;
   bool pre_room;

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You exclaim '#yDark Blade... Fill the skies with thy anger!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n exclaims '#yDark Blade... Fill the skies with thy anger!#n'",ch,NULL,NULL,TO_ROOM);
   act("You crouch to one knee as your body flares with unbelievable power!",ch,NULL,NULL,TO_CHAR);
   act("$n crouches to one knee as $s body flares with unbelievable power!",ch,NULL,NULL,TO_ROOM);
   act("Pebbles start jumping all around you as you focus the powerful energy within your blade.",ch,NULL,NULL,TO_CHAR);
   act("Pebbles start jumping all around $n as $e focuses the powerful energy within $s blade.",ch,NULL,NULL,TO_ROOM);
   act("You exclaim '#yGo Sword!!!#n' as you open your hand releasing your sword into the air!",ch,NULL,NULL,TO_CHAR);
   act("$n exclaims '#yGo Sword!!!#n' as $e opens $s hand releasing $s sword into the air!",ch,NULL,NULL,TO_ROOM);
   act("Your sword flashes into the distance leaving behind a trail of sparkling stardust!",ch,NULL,NULL,TO_CHAR);
   act("$n's sword flashes into the distance leaving behind a trail of sparkling stardust!",ch,NULL,NULL,TO_ROOM);
   act("You see a sparkle in the distance.",ch,NULL,NULL,TO_CHAR);
   act("You see a sparkle in the distance.",ch,NULL,NULL,TO_ROOM);
   act("Your sword ROARS through the air as it ZOOMS pass you at the speed of LiGht!",ch,NULL,NULL,TO_CHAR);
   act("$n's sword ROARS through the air as it ZOOMS pass you at the speed of LiGht!",ch,NULL,NULL,TO_ROOM);
   act("Your sword turn turns around as it ZOOMS into the sky!",ch,NULL,NULL,TO_CHAR);
   act("$n's sword turn turns around as it ZOOMS into the sky!",ch,NULL,NULL,TO_ROOM);
   act("Your sword suddenly SmAsHeS inTo tHe GroUnD aS tHe eNtIrE ViCiniTy ERUPTS vIoLeNtLY!!!",ch,NULL,NULL,TO_CHAR);
   act("$n's sword suddenly SmAsHeS inTo tHe GroUnD aS tHe eNtIrE ViCiniTy ERUPTS vIoLeNtLY!!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( is_same_group(ch,vch) || is_safe(ch, vch) )
           continue;

        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_var = BLAST;
        ch->attack_focus = AERIAL;

        if ( ch->ctimer[15] == 0 )
           hurt_person( ch, vch, 250000, TRUE);
   }

   act("MaSsIvE explosions fill the air as the room bursts into flames!!",ch,NULL,NULL,TO_CHAR);
   act("MaSsIvE explosions fill the air as the room bursts into flames!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( is_same_group(ch,vch) || is_safe(ch, vch) )
           continue;

        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_FIRE;
        ch->attack_var = BLAST;
        ch->attack_focus = DIRECT;

        if ( ch->ctimer[15] == 0 )
           hurt_person( ch, vch, 250000, TRUE);
   }

   act("The room goes white as a #wSuperNova#n RiPs through EvErYtHiNg in its path!",ch,NULL,NULL,TO_CHAR);
   act("The room goes white as a #wSuperNova#n RiPs through EvErYtHiNg in its path!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( is_same_group(ch,vch) || is_safe(ch, vch) )
           continue;

        count = 0;
        direction = number_range( 0, 3 );

        if ( ch->ctimer[15] == 0 )
           {
              do
              {
                 pre_room = special_slam( vch, direction );
                 count++;
              }
              while ( count < 5 && pre_room != TRUE );
           }

        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_THUNDER;
        ch->attack_var = BLAST;
        ch->attack_focus = UPPERCUT;

        hurt_person( ch, vch, 500000, TRUE);

        if ( ch->ctimer[15] == 0 )
           if ( vch != NULL && vch->in_room != NULL && IS_NPC( vch ) && vch->position > POS_STUNNED )
              stop_fighting( vch, TRUE );
   }
}

void do_finishing_touch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   OBJ_DATA *obj;

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You swerve your hand around as you raise your sword in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n swerves $s hand around as $e raises $s sword in the air.",ch,NULL,NULL,TO_ROOM);
   act("You twirl your sword across youself as energy converges to the tip.",ch,NULL,NULL,TO_CHAR);
   act("$n twirls $s sword across $mself as energy converges to the tip.",ch,NULL,NULL,TO_ROOM);
   act("Crouching on one knee, you slash the air viciously creating a massive tornado!",ch,NULL,NULL,TO_CHAR);
   act("Crouching on one knee, $n slashes the air viciously creating a massive tornado!",ch,NULL,NULL,TO_ROOM);
   act("The ToRnAdO sPiRaLs throughout the room in a frenzy!",ch,NULL,NULL,TO_CHAR);
   act("The ToRnAdO sPiRaLs throughout the room in a frenzy!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
  
      if ( is_same_group(ch,vch) || is_safe(ch, vch) )
         continue;

      act("The tornado flings you into the air as you wildly try to grasp on to something!",vch,NULL,NULL,TO_CHAR);
      act("The tornado flings $n into the air as $e wildly tries to grasp on to something!",vch,NULL,NULL,TO_ROOM);
      vch->air_timer = number_range( 3, 5 );

      if ( ( IS_CLASS( vch, CLASS_ANGEL ) || IS_CLASS( vch, CLASS_SHADOW_WAR ) ) && vch->ctimer[8] > 0 )
         ;
         else if ( !str_cmp( "N", vch->groove ) || !str_cmp( "A", vch->groove ) )
                 vch->hit -= 2000000;
                 else vch->hit -= 1250000;

      stop_fighting( vch, TRUE );
   }
}

void do_confuser( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;

  argument = one_argument( argument, arg );

  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a sword.\n\r", ch );
           return;
        }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to confuse?\n\r",ch);
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

  act("You take one step forward clutching your sword tightly.",ch,NULL,NULL,TO_CHAR);
  act("$n takes one step forward clutching $s sword tightly.",ch,NULL,NULL,TO_ROOM);
  act("Your sword flares with energy as you softly close your eyes.",ch,NULL,NULL,TO_CHAR);
  act("$n's sword flares with energy as $e softly closes $s eyes.",ch,NULL,NULL,TO_ROOM);
  act("You mutter '#yConfuser...#n' as the ground starts trembling beneath your feet.",ch,NULL,NULL,TO_CHAR);
  act("$n mutters '#yConfuser...#n' as the ground starts trembling beneath your feet.",ch,NULL,NULL,TO_ROOM);
  act("You dash towards $N thrusting your sword through $S chest as you StAgGeR $M with an uppercut.",ch,NULL,victim,TO_CHAR);
  act("$n dashes towards you thrusting $s sword through your chest as $e StAgGeRs you with an uppercut.",ch,NULL,victim,TO_VICT);
  act("$n dashes towards $N thrusting $s sword through $S chest as $e StAgGeRs $M with an uppercut.",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image mutter '#yConfuser...#n' as the ground trembles loudly beneath your feet.#n",ch,NULL,NULL,TO_CHAR);
  act("#e$n's image mutters '#yConfuser...#n' as the ground trembles loudly beneath your feet.#n",ch,NULL,NULL,TO_ROOM);
  act("You twirl your sword upwards $N's chest as a wave of energy sears $M viciously!",ch,NULL,victim,TO_CHAR);
  act("$n twirls $s sword upwards your chest as a wave of energy sears you viciously!",ch,NULL,victim,TO_VICT);
  act("$n twirls $s sword upwards $N's chest as a wave of energy sears $M viciously!",ch,NULL,victim,TO_NOTVICT);
  act("#eYour image dashes towards $N thrusting its sword through $S chest as it StAgGeRs $M with an uppercut.#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image dashes towards you thrusting its sword through your chest as it StAgGeRs you with an uppercut.#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image dashes towards $N thrusting its sword through $S chest as it StAgGeRs $M with an uppercut.#n",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image mutter '#yConfuser...#n' as the ground trembles loudly beneath your feet.#n",ch,NULL,NULL,TO_CHAR);
  act("#e$n's image mutters '#yConfuser...#n' as the ground trembles loudly beneath your feet.#n",ch,NULL,NULL,TO_ROOM);
  act("You crouch down RiPpInG your sword across $N's waist!",ch,NULL,victim,TO_CHAR);
  act("$n crouches down RiPpInG $s sword across your waist!",ch,NULL,victim,TO_VICT);
  act("$n crouches down RiPpInG $s sword across $N's waist!",ch,NULL,victim,TO_NOTVICT);
  act("#eYour image twirls its sword upwards $N's chest as a wave of energy sears $M viciously!#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image twirls its sword upwards your chest as a wave of energy sears you viciously!#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image twirls its sword upwards $N's chest as a wave of energy sears $M viciously!#n",ch,NULL,victim,TO_NOTVICT);
  act("#eYour image dashes towards $N thrusting its sword through $S chest as it StAgGeRs $M with an uppercut.#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image dashes towards you thrusting its sword through your chest as it StAgGeRs you with an uppercut.#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image dashes towards $N thrusting its sword through $S chest as it StAgGeRs $M with an uppercut.#n",ch,NULL,victim,TO_NOTVICT);

  act("You yell '#yStrike!#n' as you leap into the air StRiKiNg $N viciously!",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yStrike!#n' as $e leaps into the air StRiKiNg you viciously!",ch,NULL,victim,TO_VICT);
  act("$n yells '#yStrike!#n' as $e leaps into the air StRiKiNg $N viciously!",ch,NULL,victim,TO_NOTVICT);
  act("#eYour image crouches down RiPpInG its sword across $N's waist!#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image crouches down RiPpInG its sword across your waist!#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image crouches down RiPpInG its sword across $N's waist!#n",ch,NULL,victim,TO_NOTVICT);
  act("#eYour image twirls its sword upwards $N's chest as a wave of energy sears $M viciously!#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image twirls its sword upwards your chest as a wave of energy sears you viciously!#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image twirls its sword upwards $N's chest as a wave of energy sears $M viciously!#n",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image yells '#yStrike!#n' as it leaps into the air StRiKiNg $N viciously!#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image yells '#yStrike!#n' as it leaps into the air StRiKiNg you viciously!#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image yells '#yStrike!#n' as it leaps into the air StRiKiNg $N viciously!#n",ch,NULL,victim,TO_NOTVICT);
  act("#eYour image crouches down RiPpInG its sword across $N's waist!#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image crouches down RiPpInG its sword across your waist!#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image crouches down RiPpInG its sword across $N's waist!#n",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image yells '#yStrike!#n' as it leaps into the air StRiKiNg $N viciously!#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image yells '#yStrike!#n' as it leaps into the air StRiKiNg you viciously!#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image yells '#yStrike!#n' as it leaps into the air StRiKiNg $N viciously!#n",ch,NULL,victim,TO_NOTVICT);

  act("#yYou scream as the light VaPoRiZeS you!!!#n",victim,NULL,NULL,TO_CHAR);
  act("#y$n screams as the light VaPoRiZeS $m!!!#n",victim,NULL,NULL,TO_ROOM);

  if ( !IS_SET( victim->more3, MORE3_CONFUSE ) && ( victim->in_room->vnum < 33500 || victim->in_room->vnum > 33800 ) )
     SET_BIT( victim->more3, MORE3_CONFUSE );

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SOUL;
  ch->attack_focus = UPPERCUT;
  ch->ele_attack = ATTACK_LUNAR;
  hurt_person( ch, victim, 2000000, TRUE );
}

void do_ultimonia( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off your body as you raise your sword behind your back.",ch,NULL,NULL,TO_CHAR);
   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off $n's body as $e raises $s sword behind $s back.",ch,NULL,NULL,TO_ROOM);
   act("You whisper softly, 'Ultomina...'",ch,NULL,NULL,TO_CHAR);
   act("$n whispers softly, 'Ultomina...'",ch,NULL,NULL,TO_ROOM);
   act("Your shadow whisper softly, 'Ultomina...'",ch,NULL,NULL,TO_CHAR);
   act("$n's shadow whispers softly, 'Ultomina...'",ch,NULL,NULL,TO_ROOM);
   act("Your shadow whisper softly, 'Ultomina...'",ch,NULL,NULL,TO_CHAR);
   act("$n's shadow whispers softly, 'Ultomina...'",ch,NULL,NULL,TO_ROOM);

   act("You charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n's shadow charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n's shadow charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );

   act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's shadow slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's shadow slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );

   act("You then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n's shadow then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n's shadow then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );

   act("The palish energy sears through $N's flesh as you twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );
   act("The palish energy sears through $N's flesh as your shadow twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n's shadow twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n's shadow twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );
   act("The palish energy sears through $N's flesh as your shadow twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n's shadow twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n's shadow twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );

   act("Your sword burst into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's sword bursts into explosive flames, scorching your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's sword bursts into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_NOTVICT );

   act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's shadow slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's shadow slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );

   act("You then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n then rapidly turn around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n then rapidly turn around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n's shadow then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n's shadow then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );

   act("The palish energy sears through $N's flesh as you twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );
   act("The palish energy sears through $N's flesh as your shadow twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n's shadow twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n's shadow twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );
   act("The palish energy sears through $N's flesh as your shadow twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n's shadow twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n's shadow twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );

   act("You then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n then rapidly turn around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n then rapidly turn around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n's shadow then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow then rapidly turns around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n's shadow then rapidly turns around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );

   act("You charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n's shadow charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n's shadow charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );

   act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's shadow slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("Your shadow slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's shadow slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's shadow slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 4500000, TRUE );

   act("You collapse out of breath as you skid backwards on your feet.",ch,NULL,NULL,TO_CHAR);
   act("$n collapses out of breath as $e skids backwards on $s feet.",ch,NULL,NULL,TO_ROOM);
}

void do_rough_divide( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int count; 
   int dam = 3500000;

   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off your body as you raise your sword behind your back.",ch,NULL,NULL,TO_CHAR);
   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off $n's body as $e raises $s sword behind $s back.",ch,NULL,NULL,TO_ROOM);
   act("You mutter, 'Rough...' as you rush towards $N swinging your sword backwards as palish energy dazzles the atompshere.",ch,NULL,victim,TO_CHAR); 
   act("$n mutters, 'Rough...' as $e rushes towards you swinging $s sword backwards as palish energy dazzles the atompshere.",ch,NULL,victim,TO_VICT); 
   act("$n mutters, 'Rough...' as $e rushes towards $N swinging $s sword backwards as palish energy dazzles the atompshere.",ch,NULL,victim,TO_NOTVICT);
   act("You yell, 'Divide!' as you leap high into the heavens thrusting your sword VICIOUSLY upwards $N's body.",ch,NULL,victim,TO_CHAR);   
   act("$n yells, 'Divide!' as $e leaps high into the heavens thrusting $s sword VICIOUSLY upwards your body.",ch,NULL,victim,TO_VICT);   
   act("$n yells, 'Divide!' as $e leaps high into the heavens thrusting $s sword VICIOUSLY upwards $N's body.",ch,NULL,victim,TO_NOTVICT);   
   act("You scream loudly as the palish energy from $N's sword SEARS your flesh.",victim,NULL,ch,TO_CHAR);
   act("$n screams loudly as the palish energy from $N's sword SEARS $s flesh.",victim,NULL,ch,TO_ROOM);
   act("Suddenly the ground beneath your feet StArT sHakInG VIoLenTlY!!!",victim,NULL,NULL,TO_CHAR);
   act("Suddenly the ground beneath $n's feet StArTs sHakInG VIoLenTlY!!!",victim,NULL,NULL,TO_ROOM);
   act("The very earth SHATTER apart as you are completely engulfed by blinding light.",victim,NULL,NULL,TO_CHAR);
   act("The very earth SHATTERS apart as $n is completely engulfed by blinding light.",victim,NULL,NULL,TO_ROOM);

   for ( count = 1; count < 9; count++ )
   {
      act("A huge boulder SMASHES into YOU from underneath your very feet!",victim,NULL,NULL,TO_CHAR);
      act("A huge boulder SMASHES into $n from underneath $s feet!",victim,NULL,NULL,TO_ROOM);
   }

   if ( ch->ctimer[6] == 0 )
      {
         victim->move = UMAX( 0, victim->move - 5000000 );
         victim->mana = UMAX( 0, victim->mana - 5000000 );
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_EARTH;
   ch->attack_var = BLUNT;
   ch->attack_focus = UPPERCUT;
   hurt_person( ch, victim, dam, TRUE );

   act("You land back onto your knees, clutching your sword as you gasp for breath.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back onto $s knees, clutching $s sword as $e gasps for breath.",ch,NULL,NULL,TO_ROOM);
}

void do_blazingfury( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int count;
   int dam = 4000000;

   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off your body as you raise your sword behind your back.",ch,NULL,NULL,TO_CHAR);
   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off $n's body as $e raises $s sword behind $s back.",ch,NULL,NULL,TO_ROOM);
   act("You yell, 'BLAZING FURY!!!' as you rush towards $N swinging your sword backwards as palish energy dazzles the atompshere.",ch,NULL,victim,TO_CHAR); 
   act("$n yells, 'BLAZING FURY!!!' as $e rushes towards you swinging $s sword backwards as palish energy dazzles the atompshere.",ch,NULL,victim,TO_VICT); 
   act("$n yells, 'BLAZING FURY!!!' as $e rushes towards $N swinging $s sword backwards as palish energy dazzles the atompshere.",ch,NULL,victim,TO_NOTVICT);

   for ( count = 1; count < 5; count++ )
   {
      act("#rYour sword burst into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_CHAR );
      act("#r$n's sword bursts into explosive flames, scorching your flesh.", ch,NULL,victim,TO_VICT );
      act("#r$n's sword bursts into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_NOTVICT );
      act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
      act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
      act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
      act("#rYour sword burst into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_CHAR );
      act("#r$n's sword bursts into explosive flames, scorching your flesh.", ch,NULL,victim,TO_VICT );
      act("#r$n's sword bursts into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_NOTVICT );
   }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_fated_circle( CHAR_DATA *ch, CHAR_DATA *victim )
{
   CHAR_DATA *vch = NULL;
   CHAR_DATA *vch_next = NULL;
   int dam = 3000000;

   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off your body as you raise your sword behind your back.",ch,NULL,NULL,TO_CHAR);
   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off $n's body as $e raises $s sword behind $s back.",ch,NULL,NULL,TO_ROOM);
   act("You whisper softly, 'Fated Circle...'",ch,NULL,NULL,TO_CHAR);
   act("$n whispers softly, 'Fated Circle...'",ch,NULL,NULL,TO_ROOM);
   act("You leap high into the heavens emitting off a blazing beam of palish energy.",ch,NULL,NULL,TO_CHAR);
   act("$n leaps high into the heavens emitting off a blazing beam of palish energy.",ch,NULL,NULL,TO_ROOM);
   act("Your sword starts trembling viciously as gorgeous energy converges to the very tip.",ch,NULL,NULL,TO_CHAR);
   act("$n's sword starts trembling viciously as gorgeous energy converges to the very tip.",ch,NULL,NULL,TO_ROOM);
   act("You then swing your sword wildly, then again with all of your MIGHT!",ch,NULL,NULL,TO_CHAR);
   act("$n then swings $s sword wildly, then again with all of $s MIGHT!",ch,NULL,NULL,TO_ROOM);
   act("A huge circular wave flashes from your body as it ROARS through the vicinity!",ch,NULL,NULL,TO_CHAR);
   act("A huge circular wave flashes from $n's body as it ROARS through the vicinity!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( is_same_group(ch,vch) || is_safe(ch, vch) )
           continue;

        if ( ch->ctimer[6] == 0 && !IS_SET(vch->more, MORE_FROZEN) && !IS_CLASS( vch, CLASS_SNOW_ELF ) )
           {
              vch->freeze_timer = number_range( 1, 2 );
              SET_BIT( vch->more, MORE_FROZEN );
           }

        act("You scream loudly as the shockwave disrupts your nerval system.",vch,NULL,NULL,TO_CHAR);
        act("$n screams loudly as the shockwave disrupts $s nerval system.",vch,NULL,NULL,TO_ROOM);
        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_ICE;
        ch->attack_var = SOUL;
        ch->attack_focus = AERIAL;
        hurt_person( ch, vch, dam, TRUE );
   }

   act("You land back onto your knees, clutching your sword as you gasp for breath.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back onto $s knees, clutching $s sword as $e gasps for breath.",ch,NULL,NULL,TO_ROOM);
}

void do_blasting_zone( CHAR_DATA *ch, CHAR_DATA *victim )
{
   CHAR_DATA *vch = NULL;
   CHAR_DATA *vch_next = NULL;
   int dam = 3500000;

   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off your body as you raise your sword behind your back.",ch,NULL,NULL,TO_CHAR);
   act("\n\r#wM#ru#cl#gt#pi#e-#ycoloured #wenergy#n swirls off $n's body as $e raises $s sword behind $s back.",ch,NULL,NULL,TO_ROOM);
   act("You whisper softly, 'Blasting Zone...'",ch,NULL,NULL,TO_CHAR);
   act("$n whispers softly, 'Blasting Zone...'",ch,NULL,NULL,TO_ROOM);
   act("#wBlazing energy emits from your body as you raise your sword high above you.", ch,NULL,NULL,TO_CHAR );
   act("#wBlazing energy emits from $n's body as $e raises $s sword high above $eself.", ch,NULL,NULL,TO_ROOM );
   act("Spirals of energy swirls around your body as your sword charges up.", ch,NULL,NULL,TO_CHAR );
   act("Spirals of energy swirls around $n's body as $s sword charges up.", ch,NULL,NULL,TO_ROOM );
   act("Unbelievable energy blazes from your sword firing off into space!", ch,NULL,NULL,TO_CHAR );
   act("Unbelievable energy blazes from $n's sword firing off into space!", ch,NULL,NULL,TO_ROOM );
   act("Clutching your trembling sword tightly, you bring down the BEAM of light into the room!", ch,NULL,NULL,TO_CHAR );
   act("Clutching $s trembling sword tightly, $n brings down the BEAM of light into the room!", ch,NULL,NULL,TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
       vch_next = vch->next_in_room;
  
       if ( is_same_group(ch,vch) || is_safe(ch, vch) )
          continue;

       act("You scream loudly as the blinding beam SMASHES your body to ribbons!",vch,NULL,NULL,TO_CHAR);
       act("$n screams loudly as the blinding beam SMASHES $s body to ribbons!",vch,NULL,NULL,TO_ROOM);

       if ( !IS_SET( vch->more, MORE_BLIND ) )
          SET_BIT( vch->more, MORE_BLIND );

       if ( ch->ctimer[6] == 0 && !is_martial( vch ) && !IS_CLASS( vch, CLASS_WISP ) )
          vch->wait += 24;

       ch->attack_type = ATTACK_MAGICAL;
       ch->ele_attack = ATTACK_THUNDER;
       ch->attack_var = BEAM;
       ch->attack_focus = OVERHEAD;
       hurt_person( ch, vch, dam, TRUE );
   }
}

void do_renzokuken( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int number;
   OBJ_DATA *obj;
   bool bonus = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( !( ch->pkill > 29 && ch->throw_timer == 0 && ch->tier == 5 ) )
      {
         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
      }
      else bonus = TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Renzokuken on?\n\r", ch );
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

   if ( is_safe(ch, victim) )
      return;

   act("#wBlazing energy emits from your body as you raise your sword towards your face.", ch,NULL,NULL,TO_CHAR );
   act("#wBlazing energy emits from $n's body as $e raises $s sword towards $s face.", ch,NULL,NULL,TO_ROOM );

   act("You yell '#yRenzokuken!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yRenzokuken!!!#n'",ch,NULL,NULL,TO_ROOM);

   act("You charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );
 
   act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("You then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n then rapidly turn around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n then rapidly turn around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("The palish energy sears through $N's flesh as you twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );
   act("Your sword burst into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_CHAR );
   act("$n's sword bursts into explosive flames, scorching your flesh.", ch,NULL,victim,TO_VICT );
   act("$n's sword bursts into explosive flames, scorching $N's flesh.", ch,NULL,victim,TO_NOTVICT );
   act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );
   act("You then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n then rapidly turn around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n then rapidly turn around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("The palish energy sears through $N's flesh as you twirl your sword viciously through $N's arms.", ch,NULL,victim,TO_CHAR );
   act("The palish energy sears through your flesh as $n twirls $s sword viciously through your arms.", ch,NULL,victim,TO_VICT );
   act("The palish energy sears through $N's flesh as $n twirls $s sword viciously through $N's arms.", ch,NULL,victim,TO_NOTVICT );
   act("You then rapidly turn around as you strike through $N's midsection.", ch,NULL,victim,TO_CHAR );
   act("$n then rapidly turn around as $e strikes through your vital midsection.", ch,NULL,victim,TO_VICT );
   act("$n then rapidly turn around as $e strikes through $N's midsection.", ch,NULL,victim,TO_NOTVICT );
   act("You charge into $N as palish energy radiates off your sword.", ch,NULL,victim,TO_CHAR );
   act("$n charges into you as palish energy radiates off $s sword.", ch,NULL,victim,TO_VICT );
   act("$n charges into $N as palish energy radiates off $s sword.", ch,NULL,victim,TO_NOTVICT );
   act("You slash upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_CHAR );
   act("$n slashes upwards your body viciously as the palish energy sears your flesh.", ch,NULL,victim,TO_VICT );
   act("$n slashes upwards $N's body viciously as the palish energy sears $S flesh.", ch,NULL,victim,TO_NOTVICT );

   SET_BIT( ch->arena, AFF2_DM );

   number = number_range(1,10);
   switch(number)
   {
      case 2: do_ultimonia(ch,victim); break;
      case 9:
      case 10: do_fated_circle(ch,victim); break;
      case 5:
      case 1:
      case 4: do_rough_divide(ch,victim); break;
      case 3:
      case 7: do_blazingfury(ch,victim); break;
      case 6:
      case 8: do_blasting_zone(ch,victim); break;    
   }

   ch->wait = 12;
  
   if ( !bonus )
      {
         if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
              && ch->ctimer[9] != 100 ) )
            ch->ctimer[6] = 2;
      }
      else ch->throw_timer = 50;

   subtract_stock( ch, 300 );
}

void do_braver_strike( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("Shimmering energy radiates of you as you raise your sword into the air.",ch,NULL,NULL,TO_CHAR);
   act("Shimmering energy radiates of $n as $e raises $s sword into the air.",ch,NULL,NULL,TO_ROOM);
   act("You dash towards $N landing on your feet.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you landing on $s feet.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N landing on $s feet.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#wBraver Strike!#n' as you leap high into the air!",ch,NULL,NULL,TO_CHAR);
   act("You yell '#wBraver Strike!#n' as $n leaps high into the air!",ch,NULL,NULL,TO_ROOM);
   act("Your sword pulses with energy as you bring it down down $N's body!!!",ch,NULL,victim,TO_CHAR);
   act("$n's sword pulses with energy as $e brings it down down your body!!!",ch,NULL,victim,TO_VICT);
   act("$n's sword pulses with energy as $e brings it down down $N's body!!!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 70 - devil_luck( ch, victim, 10 ) && !IS_SET(victim->more, MORE_PAIN) )
      SET_BIT(victim->more, MORE_PAIN);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_LUNAR;
   ch->attack_var = SLASH;
   ch->attack_focus = AERIAL;
   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person( ch, victim, 500000, TRUE);
}

void do_quadra_slam( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("Shimmering energy radiates of you as you raise your sword into the air.",ch,NULL,NULL,TO_CHAR);
   act("Shimmering energy radiates of $n as $e raises $s sword into the air.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#wQuadra Slam!#n' as you rush towards $N, slicing your sword through $S body!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#wQuadra Slam!#n' as $e rushes towards you, slicing $s sword through your body!",ch,NULL,victim,TO_VICT);
   act("$n yells '#wQuadra Slam!#n' as $e rushes towards $N, slicing $s sword through $S body!",ch,NULL,victim,TO_NOTVICT);
   act("Your image circles behind $N thrusting its blade through $S back!",ch,NULL,victim,TO_CHAR);
   act("$n's image circles behind you thrusting its blade through your back!",ch,NULL,victim,TO_VICT);
   act("$n's image circles behind $N thrusting its blade through $S back!",ch,NULL,victim,TO_NOTVICT);
   act("You grip your sword slashing $N's waist viciously!",ch,NULL,victim,TO_CHAR);
   act("$n grips $s sword slashing your waist viciously!",ch,NULL,victim,TO_VICT);
   act("$n grips $s sword slashing $N's waist viciously!",ch,NULL,victim,TO_NOTVICT);
   act("The air ROARS as you SmAsH your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("The air ROARS as $n SmAsHeS $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("The air ROARS as $n SmAsHeS $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 70 - devil_luck( ch, victim, 5 ) && !IS_SET(victim->more, MORE_CONFUSION) )
      SET_BIT(victim->more, MORE_CONFUSION);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person( ch, victim, 500000, TRUE);
}

void do_knight_stunner( CHAR_DATA *ch, CHAR_DATA *victim )
{
   CHAR_DATA *vch, *vch_next;

   ch->ctimer[2] = 1;

   act("Shimmering energy radiates of you as you raise your sword into the air.",ch,NULL,NULL,TO_CHAR);
   act("Shimmering energy radiates of $n as $e raises $s sword into the air.",ch,NULL,NULL,TO_ROOM);
   act("You whisper '#wStunner!#n' as you release your sword, watching it levitates in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n whispers '#wStunner!#n' as $e releases $s sword, watching it levitates in the air.",ch,NULL,NULL,TO_ROOM);
   act("Your sword pulses with bright energy as it turns transulent.",ch,NULL,NULL,TO_CHAR);
   act("$n's sword pulses with bright energy as it turns transulent.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly an ViOlEnT outburst of #wLIGHT#n flashes through the vicinity!!!",ch,NULL,NULL,TO_CHAR);
   act("Suddenly an ViOlEnT outburst of #wLIGHT#n flashes through the vicinity!!!",ch,NULL,NULL,TO_ROOM);
   SET_BIT( ch->arena, AFF2_COMBO );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( is_same_group(ch,vch) || is_safe(ch, vch) )
           continue;

        stc("The brilliant flash from the sword blinds you.\n\r",vch);
        act("$n is blinded from the brilliant flash of light from the sword.",vch,NULL,NULL,TO_ROOM);

        if ( !is_martial( vch ) )
           vch->wait = UMIN( 18, vch->wait + 12 );

        if ( !IS_SET( vch->more, MORE_BLIND ) )
           {
              SET_BIT( vch->more, MORE_BLIND );
              vch->blind_timer = 4;
           }

        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_HOLY;
        ch->attack_var = SOUL;
        ch->attack_focus = AERIAL;
        hurt_person( ch, vch, 500000, TRUE);
   }
}

void do_meteorian( CHAR_DATA *ch, CHAR_DATA *victim )
{
   CHAR_DATA *vch, *vch_next;

   ch->ctimer[3] = 1;

   act("You twirl your sword over your head as the air forms a protective shield over you.",ch,NULL,NULL,TO_CHAR);
   act("$n twirls $s sword over $s head as the air forms a protective shield over $m.",ch,NULL,NULL,TO_ROOM);
   act("You whisper '#yMeteorian#n' as you leap into the air.",ch,NULL,NULL,TO_CHAR);
   act("$n whispers '#yMeteorian#n' as $e leaps into the air.",ch,NULL,NULL,TO_ROOM);
   act("Your sword pulses with energy as it flares #rred#n.",ch,NULL,NULL,TO_CHAR);
   act("$n's sword pulses with energy as it flares #rred#n.",ch,NULL,NULL,TO_ROOM);
   act("You swipe the air viciously as the sky goes dark with hatred!",ch,NULL,NULL,TO_CHAR);
   act("$n swipes the air viciously as the sky goes dark with hatred!",ch,NULL,NULL,TO_ROOM);
   SET_BIT( ch->arena, AFF2_COMBO );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( is_same_group(ch,vch) || is_safe(ch, vch) )
           continue;

        act("#wMasSiVe #smeteors#n SLAM into you cRuShInG you into the ground!",vch,NULL,NULL,TO_CHAR);
        act("#wMasSiVe #smeteors#n SLAM into $n cRuShInG $m into the ground!",vch,NULL,NULL,TO_ROOM);
        act("#wMasSiVe #smeteors#n SLAM into you cRuShInG you into the ground!",vch,NULL,NULL,TO_CHAR);
        act("#wMasSiVe #smeteors#n SLAM into $n cRuShInG $m into the ground!",vch,NULL,NULL,TO_ROOM);
        ch->attack_type = ATTACK_PHYSICAL;
        ch->ele_attack = ATTACK_EARTH;
        ch->attack_var = BLUNT;
        ch->attack_focus = AERIAL;
        hurt_person( ch, vch, 600000, TRUE);

        if ( number_percent() > 89 - devil_luck( ch, victim, 5 ) && vch != NULL && vch->position > POS_STUNNED )
           {
              stop_fighting( vch, TRUE );
              vch->position = POS_STUNNED;
           }
   }
}

void do_knight_wave( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("Shimmering energy radiates of you as you raise your sword into the air.",ch,NULL,NULL,TO_CHAR);
   act("Shimmering energy radiates of $n as $e raises $s sword into the air.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#wShock Wave!#n' as you SmAsH your sword onto the ground ViCiOuSlY!",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#wShock Wave!#n' as $e SmAsHeS $s sword onto the ground ViCiOuSlY!",ch,NULL,NULL,TO_ROOM);
   act("A Wave of SpIkEy EnErGy ROARS across the room with a BlAzE!!!",victim,NULL,NULL,TO_CHAR);
   act("A Wave of SpIkEy EnErGy ROARS across the room with a BlAzE!!!",victim,NULL,NULL,TO_ROOM);
   act("You stagger backwards from the MaSsIvE ShOcKwAvE!!!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers backwards from the MaSsIvE ShOcKwAvE!!!",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 96 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
      victim->wait += 12;

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_var = BLUNT;
   ch->attack_focus = LOW;
   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person( ch, victim, 500000, TRUE);
}

void do_dispatch( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("Shimmering energy radiates of you as you raise your sword into the air.",ch,NULL,NULL,TO_CHAR);
   act("Shimmering energy radiates of $n as $e raises $s sword into the air.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#wDispatcher!#n' as your body splits into images.",ch,NULL,NULL,TO_CHAR);
   act("You yell '#wDispatcher!#n' as your body splits into images.",ch,NULL,NULL,TO_ROOM);
   act("You dash towards $N striking your sword across $S chest.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you striking $s sword across your chest.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N striking $s sword across $S chest.",ch,NULL,victim,TO_NOTVICT);
   act("#eYour image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's image dashes towards you striking its sword across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's image dashes towards you striking its sword across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's image dashes towards you striking its sword across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYour image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_CHAR);
   act("#e$n's image dashes towards you striking its sword across your chest.#n",ch,NULL,victim,TO_VICT);
   act("#e$n's image dashes towards $N striking its sword across $S chest.#n",ch,NULL,victim,TO_NOTVICT);

   mana_gain( ch, 500000 );

   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;
   ch->ele_attack = ATTACK_DARK;
   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person( ch, victim, 500000, TRUE);

   act("Your images blur back together as you clutch your sword tightly.",ch,NULL,NULL,TO_CHAR);
   act("$n's images blur back together as $e clutches $s sword tightly.",ch,NULL,NULL,TO_ROOM);
}

bool is_knight_combo( CHAR_DATA *ch, CHAR_DATA *victim, int move_num )
{
   if ( move_num == 6 )
      {
         /* Dispatcher - RisingUpper, Sweep, RoundHouse, Slash, SwordTakedown, RisingUpper */
         if ( ch->clist[0] == 2 && ch->clist[1] == 3 && ch->clist[2] == 1
              && ch->clist[3] == 4 && ch->clist[4] == 5 && ch->clist[5] == 2 )
            {
               do_dispatch(ch,victim);

               if ( ch->ninjastuff == 100 )
                  ch->wait = 12;
                  else ch->wait = 4;

               ch->special_id = 0;
               ch->ninjastuff = 100;

               return TRUE;
            }
      }

   if ( move_num == 7 )
      {
         /* Braver - Sweep, RisingUpper, Roundhouse, Slash, RisingUpper, Roundhouse, SwordTakedown */
         if ( ch->clist[0] == 3 && ch->clist[1] == 2 && ch->clist[2] == 1
              && ch->clist[3] == 4 && ch->clist[4] == 2 && ch->clist[5] == 1 && ch->clist[6] == 5 )
            {
                do_braver_strike(ch,victim);

                if ( ch->ninjastuff == 101 )
                  ch->wait = 12;
                  else ch->wait = 4;

                ch->special_id = 0;
                ch->ninjastuff = 101;

                return TRUE;
            }

         /* Quadra Slam - RisingUpper, Sweep, Slash, Roundhouse, SwordTakedown, Roundhouse, RisingUpper */
         if ( ch->clist[0] == 2 && ch->clist[1] == 3 && ch->clist[2] == 4
              && ch->clist[3] == 1 && ch->clist[4] == 5 && ch->clist[5] == 1 && ch->clist[6] == 2 )
            {
                do_quadra_slam(ch,victim);

                if ( ch->ninjastuff == 105 )
                  ch->wait = 12;
                  else ch->wait = 4;

                ch->special_id = 0;
                ch->ninjastuff = 105;

                return TRUE;
            }

         /* Stunner - Sweep, Roundhouse, SwordTakedown, RisingUpper, Slash, Roundhouse, RisingUpper */
         if ( ch->clist[0] == 3 && ch->clist[1] == 1 && ch->clist[2] == 5
              && ch->clist[3] == 2 && ch->clist[4] == 4 && ch->clist[5] == 1 && ch->clist[6] == 2 )
            {
                if ( ch->ctimer[2] == 0 )
                   do_knight_stunner(ch,victim);

                if ( ch->ninjastuff == 102 )
                  ch->wait = 12;
                  else ch->wait = 4;

                ch->special_id = 0;
                ch->ninjastuff = 102;

                return TRUE;
            }

         /* ShockWave - Swordtakedown, RisingUpper, Sweep, Swordtakedown, Roundhouse, Roundhouse, Sweep */
         if ( ch->clist[0] == 5 && ch->clist[1] == 2 && ch->clist[2] == 3
              && ch->clist[3] == 5 && ch->clist[4] == 1 && ch->clist[5] == 1 && ch->clist[6] == 3 )
            {
                do_knight_wave(ch,victim);

                if ( ch->ninjastuff == 103 )
                  ch->wait = 12;
                  else ch->wait = 4;

                ch->special_id = 0;
                ch->ninjastuff = 103;

                return TRUE;
            }

         /* Meteorian - RisingUpper, Roundhouse, Sweep, Swordtakedown, Roundhouse, Slash, Sweep */
         if ( ch->clist[0] == 2 && ch->clist[1] == 1 && ch->clist[2] == 3
              && ch->clist[3] == 5 && ch->clist[4] == 1 && ch->clist[5] == 4 && ch->clist[6] == 3 )
            {
                if ( ch->ctimer[3] == 0 )
                   do_meteorian(ch,victim);

                if ( ch->ninjastuff == 104 )
                  ch->wait = 12;
                  else ch->wait = 4;

                ch->special_id = 0;
                ch->ninjastuff = 104;

                return TRUE;
            }
      }

   return FALSE;
}

void do_knight_round( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a roundhouse kick to?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING || arg[0] != '\0')
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

   if ( ch->special_id < 12 )
      {
         if ( is_knight_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   act("You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.",ch,NULL,victim,TO_VICT);
   act("$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_NOTVICT);
}

void do_knight_upper( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a uppercut?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING || arg[0] != '\0')
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

   if ( ch->special_id < 12 )
      {
         if ( is_knight_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   act("You dash towards $N as your hands pulses with energy.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you as $s hands pulses with energy.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N as $s hands pulses with energy.",ch,NULL,victim,TO_NOTVICT);
   act("The air ROARS as you SmAsH your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("The air ROARS as $n SmAsHeS $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("The air ROARS as $n SmAsHeS $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
}

void do_knight_sweep( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to trip?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING || arg[0] != '\0')
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

   if ( ch->special_id < 12 )
      {
         if ( is_knight_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   act("You spin around on the ground tripping $N with your foot.",ch,NULL,victim,TO_CHAR);
   act("$n spins around on the ground tripping you with $s foot.",ch,NULL,victim,TO_VICT);
   act("$n spins around on the ground tripping $N with $s foot.",ch,NULL,victim,TO_NOTVICT);
}

void do_slash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to slash?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING || arg[0] != '\0')
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

   if ( ch->special_id < 12 )
      {
         if ( is_knight_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   act("You dash across the room slashing your sword through $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n dashes across the room slashing $s sword through your chest.",ch,NULL,victim,TO_VICT);
   act("$n dashes across the room slashing $s sword through $N's chest.",ch,NULL,victim,TO_NOTVICT);
}

void do_knight_takedown( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to trip?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING || arg[0] != '\0')
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

   if ( ch->special_id < 12 )
      {
         if ( is_knight_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   act("You swipe your sword across $N's legs tripping $M off $S feet.",ch,NULL,victim,TO_CHAR);
   act("$n swipes $s sword across your legs tripping you off your feet.",ch,NULL,victim,TO_VICT);
   act("$n swipes $s sword across $N's legs tripping $M off $S feet.",ch,NULL,victim,TO_NOTVICT);
}

void do_dark_bomb( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 4 )
     {
        send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
        return;
     }

  if ( ch->mana < 500000 )
     {
        send_to_char("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to cast Dark Bomb on?\n\r",ch);
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

  ch->mana -= 500000;

  act("You slide your right hand across your body as you quickly raise a finger on your left one.",ch,NULL,NULL,TO_CHAR);
  act("$n slides $s right hand across $s body as $e quickly raises a finger on $s left one.",ch,NULL,NULL,TO_ROOM);

  act("Mumbling the ancient language, you form a circle of chaotic energy with your left hand.",ch,NULL,NULL,TO_CHAR);
  act("Mumbling the ancient language, $n forms a circle of chaotic energy with $s left hand.",ch,NULL,NULL,TO_ROOM);

  act("A look of severe concentration fills your face as you refocus the dark energy.",ch,NULL,NULL,TO_CHAR);
  act("A look of severe concentration fills $n's face as $e refocuses the energy surrounding $m.",ch,NULL,NULL,TO_ROOM);

  act("With your hair blowing in the wind, you point your hand at $N with absolute hatred.",ch,NULL,victim,TO_CHAR);
  act("With $s hair blowing in the wind, $n points $s hand at you with absolute hatred.",ch,NULL,victim,TO_VICT);
  act("With $s hair blowing in the wind, $n points $s hand at $N with absolute hatred.",ch,NULL,victim,TO_NOTVICT);

  act("The ground beneath your feet shakes violently as a mass of explosive energy rips from within your body.",victim,NULL,NULL,TO_CHAR);
  act("The ground beneath $n's feet shakes violently as a mass of explosive energy rips from within $s body.",victim,NULL,NULL,TO_ROOM);

  if ( ch->class_2 == 0 )
     ch->wait = 18;
     else ch->wait = 12;

  ch->ele_attack = ATTACK_DARK;
  ch->attack_type = ATTACK_MAGICAL;
  ch->attack_var = BLAST;
  ch->attack_focus = DIRECT;
  hurt_person( ch, victim, 600000, TRUE );
}

void do_hyper( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 6 )
     {
        send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
        return;
     }

  if ( ch->mana < 500000 )
     {
        send_to_char("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  victim = ch;

  if ( is_affected( victim, skill_lookup("hyper") ) )
     {
        send_to_char( "That person is already moving faster than the eye can see.\n\r", ch );
        return;
     }

  spell_hyper( skill_lookup("hyper"), 30, ch, victim );
  ch->mana -= 500000;
}

void do_sword_rain( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 8 )
     {
        send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
        return;
     }

  if ( ch->mana < 1000000 )
     {
        send_to_char("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to cast Swords on?\n\r",ch);
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

  if ( ch->in_room != victim->in_room && IS_SET( ch->more, MORE_HEX ) )
     {
        stc( "You open your mouth to say something but nothing comes out.\n\r", ch );
        return;
     }

  ch->mana -= 1000000;

  if ( IS_SET( ch->act, PLR_WIZINVIS ) )
     REMOVE_BIT( ch->act, PLR_WIZINVIS );

  act("You raise your right hand in the air as a shimmering sword manifests above it.",ch,NULL,NULL,TO_CHAR);
  act("$n raises $s right hand in the air as a shimmering sword manifests above it.",ch,NULL,NULL,TO_ROOM);
  act("Focusing on your hatred, you hurl the sword towards $N!",ch,NULL,victim,TO_CHAR);
  act("Focusing on $s hatred, $n hurls the sword towards you!",ch,NULL,victim,TO_VICT);
  act("Focusing on $s hatred, $n hurls the sword towards $N!",ch,NULL,victim,TO_NOTVICT);
  act("$N screams loudly as the sword drives through $S chest!",ch,NULL,victim,TO_CHAR);
  act("You screams loudly as the sword drives through your chest!",ch,NULL,victim,TO_VICT);
  act("$N screams loudly as the sword drives through $S chest!",ch,NULL,victim,TO_NOTVICT);
  act("Suddenly a barrage of swords fade into existence, flying towards $n!",victim,NULL,NULL,TO_ROOM);
  act("Suddenly a barrage of swords fade into existence, flying towards you!",victim,NULL,NULL,TO_CHAR);
  act("$N screams loudly as the swords DRIVE through $S chest!",ch,NULL,victim,TO_CHAR);
  act("You screams loudly as the swords DRIVE through your chest!",ch,NULL,victim,TO_VICT);
  act("$N screams loudly as the swords DRIVE through $S chest!",ch,NULL,victim,TO_NOTVICT);

  if ( ch->class_2 == 0 )
     ch->wait = 18;
     else ch->wait = 12;

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = AERIAL;
  hurt_person( ch, victim, 600000, TRUE );
}

void do_mergance( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg [MAX_INPUT_LENGTH];
  int temp;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 10 )
     {
        send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
        return;
     }

  if ( ch->fighting == NULL )
     {
        send_to_char("You can only do this technique in the heat of battle.\n\r",ch);
        return;
     }

  if ( ch->mergence == 0 )
     {
        send_to_char("You hadn't been vultured as yet dark knight.\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     victim = ch->fighting;
     else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
        send_to_char( "They aren't here.\n\r", ch );
        return;
     }

  if ( is_safe( ch, victim ) )
     return;

  act("You grin evilly at $N as your body suddenly shift into fading images.",ch,NULL,victim,TO_CHAR);
  act("$n grins evilly at you as $s body suddenly shifts into fading images.",ch,NULL,victim,TO_VICT);
  act("$n grins evilly at $N as $s body suddenly shifts into fading images.",ch,NULL,victim,TO_NOTVICT);
  act("Concentrating on your dark powers, you lock onto $N's mind severely.",ch,NULL,victim,TO_CHAR);
  act("Concentrating on $s dark powers, $n locks onto your mind severely.",ch,NULL,victim,TO_VICT);
  act("Concentrating on $s dark powers, $n locks onto $N's mind severely.",ch,NULL,victim,TO_NOTVICT);
  act("Before you realize what happened, you feel something different about you.",victim,NULL,NULL,TO_CHAR);
  act("Before $n realizes what happened, $e feels something different about $mself.",victim,NULL,NULL,TO_ROOM);

  ch->mergence = 0;
  temp = ch->hit;
  ch->hit = victim->hit;
  victim->hit = temp;

  temp = ch->mana;
  ch->mana = victim->mana;
  victim->mana = temp;

  temp = ch->move;
  ch->move = victim->move;
  victim->move = temp;
}

void do_hwrath( CHAR_DATA *ch, char *argument)
{
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA       *pexit;
   CHAR_DATA       *vch, *vch_next;
   char            arg1      [MAX_INPUT_LENGTH];
   int             door;
   int             count, dam = 700000;
   bool            kng = FALSE;

   argument = one_argument( argument, arg1 );

   if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && !IS_SET( ch->special, SPC_SWORD ) )
      kng = TRUE;      

   if ( can_do_skill( ch, 47 ) )
      {
         if ( ch->throw_timer > 0 )
            {
               if ( !kng )
                  {
                     stc("You are still recovering from the last one.\n\r",ch);
                     return;
                  }
            }
            else dam = 2000000;
      }
      else if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
              {
                 send_to_char("Huh?\n\r",ch);
                 return;
              }

   if ( kng && ch->pkill < 12 && dam < 2000000 )
      {
         send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 && dam < 2000000 )
      {
         send_to_char("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( arg1[0] == '\0' )
      {
	   send_to_char("Please specify a direction to release your wrath.\n\r", ch);
         return;
      }

   if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) )
	{ door = 0; }
      else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east" ) )
              { door = 1; }
      else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) )
              { door = 2; }
      else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west" ) )
              { door = 3; }
      else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up" ) )
              { door = 4; }
      else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down" ) )
              { door = 5; }
      else {
              send_to_char("You can only release your wrath north, south, east, west, up or down.\n\r", ch);
              return;
           }

   location = ch->in_room;

   if ( dam == 2000000 )
      {
         ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
         ch->throw_timer = 50;
      }
      else {
              ch->mana -= 1000000;

              if ( ch->class_2 == 0 )
                ch->wait = 18;
                else ch->wait = 12;
           }

   if ( IS_SET( ch->act, PLR_WIZINVIS ) )
      REMOVE_BIT( ch->act, PLR_WIZINVIS );

   act("Spirals of dark energy encircle your hands as you gaze down silently at them.",ch,NULL,NULL,TO_CHAR);
   act("Spirals of dark energy encircle $n's hands as $e gaze down silently at them.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly you grin evilly as you ask '#yReady?#n'",ch,NULL,NULL,TO_CHAR);
   act("Suddenly $n grins evilly as $e asks '#yReady?#n'",ch,NULL,NULL,TO_ROOM);
   act("A loud humming fills the air as you raise your hands high above you.",ch,NULL,NULL,TO_CHAR);
   act("A loud humming fills the air as $e raises $s hands high above you.",ch,NULL,NULL,TO_ROOM);
   act("Sparks of divine energy fizzes from your body as you gaze into the sky.",ch,NULL,NULL,TO_CHAR);
   act("Sparks of divine energy fizzes from $n's body as $e gazes into the sky.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yHeaven's Wrath!#n' as you release a MASSIVE shock wave to the $T!",ch,NULL,dir_name[door],TO_CHAR);
   act("$n screams '#yHeaven's Wrath!#n' as $e releases a MASSIVE shock wave to the $T!",ch,NULL,dir_name[door],TO_ROOM);

   for ( count = 0; count < 5; count++ )
   {
      if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
         {
            act("The wall trembles slightly as it absorbs a ferocious blast.",ch,NULL,NULL,TO_CHAR);
            act("The wall trembles slightly as it absorbs a ferocious blast.",ch,NULL,NULL,TO_ROOM);

            if ( ch->in_room != location )
               {
                  char_from_room( ch );
                  char_to_room( ch, location );
               }

            stop_fighting( ch, TRUE );
            return;
         }

      char_from_room( ch );
      char_to_room( ch, to_room );

      for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next_in_room;

         if ( is_same_group( ch, vch ) || is_safe( ch, vch ) )
            continue;

         act("A TREMEDOUS force sends you flying as the wind ROARS mightily!",vch,NULL,NULL,TO_CHAR);
         act("A TREMEDOUS force sends $n flying as the wind ROARS mightily!",vch,NULL,NULL,TO_ROOM);

         vch->agg = 100;

         ch->attack_type = ATTACK_MAGICAL;
         ch->ele_attack = ATTACK_HOLY;
         ch->attack_var = BLAST;
         ch->attack_focus = LOW;

         if ( IS_NPC( vch ) )
            hurt_person( ch, vch, dam * 2, TRUE );
            else hurt_person( ch, vch, dam, TRUE );

         if ( vch != NULL && vch->fighting == ch )
            stop_fighting( vch, TRUE );
      }

      if ( dam < 2000000 )
         dam += 100000;
   }

   stop_fighting( ch, TRUE );
   char_from_room( ch );
   char_to_room( ch, location );
}

void do_stitch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 14 )
      {
         send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[1] > 0 )
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

   if ( victim->fight_timer == 0 )
      {
         send_to_char("But they aren't facing battle dark one.\n\r", ch );
         return;
      }

   ch->ctimer[1] = 2;

   if ( !is_martial( victim ) )
      victim->wait = 12;

   stop_fighting( ch, TRUE );

   act("You mutter '#yHmph...#n' as the shadows leap from the ground engulfing you...",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yHmph...#n' as the shadows leap from the ground engulfing $m...",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   do_look(ch,"");

   act("You then concentrate on your dark powers as you lock onto $N's shadow.",ch,NULL,victim,TO_CHAR);
   act("Your shadow suddenly rips apart from your body as it slowly forms into $n.",ch,NULL,victim,TO_VICT);
   act("$N's shadow suddenly rips apart from $S body as it slowly forms into $n.",ch,NULL,victim,TO_NOTVICT);
   check_traps( ch );

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );
}

void do_psychic_barrier( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || ch->class_2 != 0 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 3000000 )
      {
         send_to_char("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   act("You raise your finger on your forehead as you concentrate furiously!", ch, NULL, NULL, TO_CHAR);
   act("$n raises $s finger on $s forehead as $e concentrates furiously!", ch, NULL, NULL, TO_ROOM);
   act("You summon a mystical barrier across the room as your hair blows through the wind!", ch, NULL, NULL, TO_CHAR);
   act("$n summons a mystical barrier across the room as $s hair blows through the wind!", ch, NULL, NULL, TO_ROOM);
   ch->wait = 6;
   ch->mana -= 1000000;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
       vch_next = vch->next_in_room;

       if ( IS_NPC( vch ) || IS_SET( vch->more, MORE_BARRIER ) )
          continue;

       if ( is_same_group( ch, vch ) || ( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom != 0 && ch->pcdata->kingdom != 5 ) )
          {
             act("Your body shimmers with dark energy.", vch, NULL, NULL, TO_CHAR );
             act("$n's body shimmers with dark energy.", vch, NULL, NULL, TO_ROOM );
             SET_BIT( vch->more, MORE_BARRIER );
          }
    }
}

void do_absolute_zero( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   DESCRIPTOR_DATA *d;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 18 )
      {
         send_to_char("Your mastery of the forbidden magic isn't great enough as yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         send_to_char("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   ch->ctimer[4] = 6;

   if ( ch->class_2 == 0 )
      ch->wait = 18;
      else ch->wait = 12;

   act("Spirals of dark energy encircle your hands as you gaze down silently at them.",ch,NULL,NULL,TO_CHAR);
   act("Spirals of dark energy encircle $n's hands as $e gaze down silently at them.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly you grin evilly as you ask '#yReady?#n'",ch,NULL,NULL,TO_CHAR);
   act("Suddenly $n grins evilly as $e asks '#yReady?#n'",ch,NULL,NULL,TO_ROOM);
   act("A menacing breeze chill your bones as a frosty ball of energy manifest in your hands.",ch,NULL,NULL,TO_CHAR);
   act("A menacing breeze chill your bones as a frosty ball of energy manifest in $n's hands.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly throw it into the air as you crouch onto your knees.",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly throws it into the air as $e crouches onto $s knees.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yAbsolute Zero!#n' as you leap into the air and SMASH the ball to bits!",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yAbsolute Zero!#n' as $e leaps into the air and SMASHES the ball to bits!",ch,NULL,NULL,TO_ROOM);
   sta("#eThe #sc#so#sl#so#su#sr #efalls off your body as time becomes still, like a greyscale snapshot.#n\n\r", ch->in_room->area );

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != ch )
	{
          if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
             continue;

          if ( ch->in_room->area != d->character->in_room->area || IS_SET( d->character->more, MORE_FROZEN )
               || ch->in_room == d->character->in_room )
             continue;

          SET_BIT( d->character->more, MORE_FROZEN );
          d->character->freeze_timer = 2;
	}
   }
}

void show_dline( char * name, int num, char * str, CHAR_DATA *ch, int div )
{
    char buf [MAX_INPUT_LENGTH];
    int count, per = ( ( num < 0 ) ? -num : num ) / div;

    sprintf( buf, "   #w| %10s #w|#n", name );

    if ( num < 0 )
       {
          num = -num / 10;

          for ( count = 10; count > 0; count-- )
          {
             if ( IS_SET( ch->act, PLR_ANSI ) )
                {
                   if ( count == num )
                      strcat( buf, str );

                   strcat( buf, " " );
                }
                else {
                         if ( count <= num )
                            strcat( buf, "*" );
                            else strcat( buf, " " );
                     }
          }

          num = -num;
       }
       else strcat( buf, "          " );

    strcat( buf, "#n#w|#n" );

    if ( num > 0 )
       {
          num /= 10;

          if ( IS_SET( ch->act, PLR_ANSI ) )
             strcat( buf, str );

          for ( count = 0; count < 10; count++ )
          {
             if ( IS_SET( ch->act, PLR_ANSI ) )
                {
                   if ( count == num )
                      strcat( buf, "#n" );

                   strcat( buf, " " );
                }
                else {
                        if ( count <= num )
                           strcat( buf, "*" );
                           else strcat( buf, " " );
                     }
          }
       }
       else strcat( buf, "          " );

    stc( buf, ch );

    sprintf( buf, "#n#w| #C%2d#y%% #w|#n\n\r", per );
    stc( buf, ch );
}

void do_analyse( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   char buf [MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC( ch ) )
      return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Analyse whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_area( ch, arg ) ) == NULL )
    {
	send_to_char( "Analyse whom?\n\r", ch );
	return;
    }

    act("You analyse $N carefully, but silently from a distance...",ch,NULL,victim,TO_CHAR);
    act("You can't help but have the most sinister feeling you are being watched...",victim,NULL,NULL,TO_CHAR);

    if ( IS_NPC( victim ) )
       sprintf(buf, "#C%s #gis an #PNPC#g.#n\n\r",victim->short_descr);
       else sprintf(buf, "#C%s #gis a #yMortal#g.#n\n\r", victim->name);

    send_to_char(buf,ch);

    if ( !IS_NPC( victim ) )
       {
           sprintf( buf, "#wStr:#y%d#w, Vit:#y%d#w, Min:#y%d#w, Spi:#y%d#w, Dex:#y%d#w, Eva:#y%d#w, ",
                         get_curr_str(victim), get_curr_con(victim), get_curr_int(victim), get_curr_wis(victim),
                         get_curr_dex(victim), get_curr_eva(victim) );
           send_to_char( buf, ch );

           if ( victim->pkill > 19 )
              sprintf( buf, "Tens:#y%d#w, Agi:#y%d#w.#n\n\r", get_curr_tens(victim), get_curr_agi(victim) );
              else sprintf( buf, "Tens:#y??#w, Agi:#y??#w.#n\n\r" );

           send_to_char( buf, ch );
       }

    sprintf(buf,"#wHp:#C%d#w/#y%d#w, Mana:#C%d#w/#y%d#w, Move:#C%d#w/#y%d#w.#n\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);

    sprintf(buf,"#yAlignment:#P%d#w.#n\n\r",victim->alignment);
    send_to_char(buf,ch);

    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("#C$N #gis #Ppregnant#g.#n",ch,NULL,victim,TO_CHAR);

    stc( "   #w----------------------------------------\n\r", ch );
    stc( "   #w| #oVariant  #w| #oWeakness #w| #oReslient #w| #oVal #w|\n\r", ch );
    stc( "   #w----------------------------------------\n\r", ch );

    show_dline( "#Rfire", get_fire_prof( victim ), "#!", ch, 2 );
    show_dline( "#Cice", get_ice_prof( victim ), "#%", ch, 2 );
    show_dline( "#ythunder", get_thunder_prof( victim ), "#$", ch, 2 );
    show_dline( "#gearth", get_earth_prof( victim ), "#@", ch, 2 );
    show_dline( "#wholy", get_holy_prof( victim ), "#*", ch, 2 );
    show_dline( "#edark", get_dark_prof( victim ), "#^", ch, 2 );
    show_dline( "#Cnatural", get_natural_prof( victim ), "#%", ch, 2 );
    show_dline( "#olunar", get_lunar_prof( victim ), "#$", ch, 2 );
    show_dline( "#cslash", get_slash_prof( victim ), "#&", ch, 2 );
    show_dline( "#cpierce", get_pierce_prof( victim ), "#&", ch, 2 );
    show_dline( "#cblast", get_blast_prof( victim ), "#&", ch, 2 );
    show_dline( "#cbeam", get_beam_prof( victim ), "#&", ch, 2 );
    show_dline( "#cblunt", get_blunt_prof( victim ), "#&", ch, 2 );
    show_dline( "#Pspirit", get_spirit_prof( victim ), "#^", ch, 2 );
    show_dline( "#wphysical", get_physical_prof( victim ), "#*", ch, 4 );
    show_dline( "#Pmagical", get_magical_prof( victim ), "#^", ch, 4 );

    stc( "   #w----------------------------------------\n\r", ch );
}

void do_determine( CHAR_DATA *ch, char *argument )
{
   if ( IS_SET( ch->extra, EXTRA_DETERMINE ) )
      {
         stc("#PYou #ebreath deeply as you stop #wanalysing #eattacks...#n\n\r", ch );
         REMOVE_BIT( ch->extra, EXTRA_DETERMINE );
         return;
      }

   SET_BIT( ch->extra, EXTRA_DETERMINE );
   stc("#PYou #ebreath deeply as you start #wanalysing #eattacks...#n\n\r", ch );
}

void do_advise( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   char arg[MAX_INPUT_LENGTH];
   int sn;

   if ( ( sn = skill_lookup( "battle tactics" ) ) < 0 )
      return;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         send_to_char("Your mastery of the forbidden arts isn't great enough as yet.\n\r",ch);
         return;
      }

   ch->wait = 12;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
       vch_next = vch->next_in_room;

       if ( vch == ch || IS_NPC( vch ) || is_affected( vch, sn ) )
          continue;

       if ( is_same_group( ch, vch ) || ( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom != 0 && ch->pcdata->kingdom != 5 ) )
          {
             act("You silently give $N pointers on how to fight better.", ch, NULL, vch, TO_CHAR );
             act("$n silently gives you pointers on how to fight better.", ch, NULL, vch, TO_VICT );
             act("$n silently speaks to $N as $E suddenly looks WAY smarter.", ch, NULL, vch, TO_NOTVICT );
             SET_BIT( vch->more3, MORE3_RETORT );
             (*skill_table[sn].spell_fun) ( sn, 50, vch, vch );
          }
    }
}

void do_anti_stock( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !can_do_skill( ch, 80 ) )
      if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( !can_do_skill( ch, 80 ) )
      if ( ch->pkill < 25 )
         {
            send_to_char("Your mastery of the forbidden arts isn't great enough as yet.\n\r",ch);
            return;
         }

   if ( IS_SET( ch->in_room->room_flags, ROOM_NOSTOCK ) )
      {
         stc("You release your focus on the room as the antistock barrier vanishes.",ch);
         REMOVE_BIT( ch->in_room->room_flags, ROOM_NOSTOCK );
         return;
      }

   SET_BIT( ch->in_room->room_flags, ROOM_NOSTOCK );

   act("You put your finger on your forehead as you concentrate furiously!", ch, NULL, NULL, TO_CHAR);
   act("$n put $s finger on $s forehead as $e concentrates furiously!", ch, NULL, NULL, TO_ROOM);
   act("You summon a mystical barrier across the room as stock seems to be dampened!", ch, NULL, NULL, TO_CHAR);
}
