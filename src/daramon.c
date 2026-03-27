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

void do_subtletouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DARAMON) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->move < 250000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use the Subtle Touch on?\n\r",ch);
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

    WAIT_STATE(ch, 18);
    ch->move -= 250000;

    if ( victim->position != POS_FIGHTING )
       victim->agg = UMAX( victim->agg - 10, 0 );

    if ( IS_SET(victim->monkstuff, MONK_DEATH) )
       REMOVE_BIT(victim->monkstuff, MONK_DEATH);

    if ( ch == victim )
       {
          act("You place your hands on your head forcing the negative energy from within you.",ch,NULL,NULL,TO_CHAR);
          act("$n places $s hands on $s head forcing the negative energy from within $m.",ch,NULL,NULL,TO_ROOM);
       }
       else {
                act("You place your hands on $N's head forcing the negative energy from within $M.",ch,NULL,victim,TO_CHAR);
                act("$n places $s hands on your head forcing the negative energy from within you.",ch,NULL,victim,TO_VICT);
                act("$n places $s hands on $N's head forcing the negative energy from within $M.",ch,NULL,victim,TO_NOTVICT);
            }
}

void do_daracreate( CHAR_DATA *ch, char *argument )
{
   char      arg [MAX_INPUT_LENGTH];
   int cost = 1, vnum;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DARAMON) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Syntax: daracreate <item>\n\r", ch );
         send_to_char( "items are: shirt (1), skirt (1), sandals (1), sword (1)\n\r", ch );
         return;
      }

   if ( !str_cmp(arg,"shirt") )
      {
         vnum = 243;
      }
      else if ( !str_cmp(arg,"skirt") )
              {
                 vnum = 244;
              }
      else if ( !str_cmp(arg,"sandals") )
              {
                 vnum = 245;
              }
      else if ( !str_cmp(arg,"sword") )
              {
                 vnum = 246;
              }
      else {
              do_daracreate(ch,"");
              return;
           }

   if ( cost > ch->gold )
      {
         sprintf(arg,"It costs you #w%d #ygold#n piece to make this item.\n\r", cost);
         send_to_char(arg,ch);
         return;
      }

   ch->gold -= cost;

   if ( vnum == 0 || (pObjIndex = get_obj_index( vnum ) ) == NULL )
      {
         send_to_char("Missing object, please inform an Immortal.\n\r",ch);
         return;
      }

   obj = create_object(pObjIndex, 70);
   obj_to_char(obj, ch);
   act("You pray for the Emperor's honour as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
   act("$n prays for the Emperor's honour as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_empower( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 600000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }
      
         if ( ch->mana < 500000 )
            {
               send_to_char( "You don't have enough magical energy for that.\n", ch ); 
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to Empower?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->mana -= 500000;

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You yell '#rEmpower!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#rEmpower!#n'",ch,NULL,NULL,TO_ROOM);

   act("As you swirl your hand towards $N, a black wave zooms pass you.", ch,NULL,victim,TO_CHAR );
   act("As $n swirls $s hand towards you, a black wave zooms pass you.", ch,NULL,victim,TO_VICT );
   act("As $n swirls $s hand towards $N, a black wave zooms pass you.", ch,NULL,victim,TO_NOTVICT );

   if ( ch->dpower > 5 && ch->pkill < 10 )
      ch->dpower = 0;
      else if ( ch->dpower > 5 )
      {
          act("A 8-headed dragon skull ROARS straight into $N's body.", ch,NULL,victim,TO_CHAR );
          act("A 8-headed dragon skull ROARS straight into your body.", ch,NULL,victim,TO_VICT );
          act("A 8-headed dragon skull ROARS straight into $N's body.", ch,NULL,victim,TO_NOTVICT );

          hit_gain( ch, 850000 );
          move_gain( ch, 750000 );

          dam *= 2;
          ch->dpower = 0;
      }
      else {
              act("A 2-headed dragon skull ROARS straight into $N's body.", ch,NULL,victim,TO_CHAR );
              act("A 2-headed dragon skull ROARS straight into your body.", ch,NULL,victim,TO_VICT );
              act("A 2-headed dragon skull ROARS straight into $N's body.", ch,NULL,victim,TO_NOTVICT );

              if ( ch->fight_timer > 0 )
	           {
                    if ( !IS_NPC( ch ) )
                       ch->dpower++;

                    if ( ch->dpower == 6 )
                       stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
                 }
           }

   act("$N's body ripples as the Skull drains $S lifeforce.", ch,NULL,victim,TO_CHAR );
   act("Your body ripples as the Skull drains your lifeforce.", ch,NULL,victim,TO_VICT );
   act("$N's body ripples as the Skull drains $S lifeforce.", ch,NULL,victim,TO_NOTVICT );
   act("As the skull fades away, your body absorb the extracted energy.", ch,NULL,NULL,TO_CHAR );
   act("As the skull fades away, $n's body absorbs the extracted energy.", ch,NULL,NULL,TO_ROOM );

   victim->mana = UMAX( 0, victim->mana - 250000 );
   hit_gain( ch, 150000 );
   move_gain( ch, 250000 );

   ch->wait = 18;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_stunner( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch = NULL;
   CHAR_DATA *vch_next = NULL;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DARAMON) )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->ctimer[6] > 0 )
      {
         send_to_char("You are still recovering from the last one.\n\r", ch);
         return;
      }

   ch->ctimer[6] = 6;

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You say '#yDon't comprehend the universe, its wonders will destroy the faithless.#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yDon't comprehend the universe, its wonders will destroy the faithless.#n'",ch,NULL,NULL,TO_ROOM);
   act("You yell '#rStunner Blast!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#rStunner Blast!#n'",ch,NULL,NULL,TO_ROOM);

   act("You softly pray as the ethereal sword starts hovering in the air.", ch,NULL,NULL,TO_CHAR );
   act("$n softly prays as the ethereal sword hovering in the air.", ch,NULL,NULL,TO_ROOM );
   act("As you concentrate quietly, the sword radiates off palish energy.", ch,NULL,NULL,TO_CHAR );
   act("As $n concentrates quietly, the sword radiates off palish energy.", ch,NULL,NULL,TO_ROOM );
   act("Suddenly, you felt a smile from beyond as explosive light flashes throughout the vicinity.", ch,NULL,NULL,TO_CHAR );
   act("Suddenly, you felt a smile from beyond as explosive light flashes throughout the vicinity.", ch,NULL,NULL,TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( ( is_same_group(ch,vch) ) || ( is_safe(ch, vch) ) || ( vch == ch ) )
           continue;

        if ( ch->dpower > 5 )
           {
              SET_BIT(vch->more, MORE_BLIND);
              vch->blind_timer = 12;
  
              stc("The searing glare from the sword blinds you.\n\r",vch);
              act("$n is blinded from the searing glare of light from the sword.",vch,NULL,NULL,TO_ROOM);

              stop_fighting(vch,TRUE);
              vch->position = POS_STUNNED;

              if ( !is_martial( vch ) && !IS_CLASS( vch, CLASS_WISP ) )
                 vch->wait += 24;

              stc("You fall to the ground in pain as you felt an invisible force smashes its fist into you.\n\r",vch);
              act("$n falls to the ground clutching $s stomach in pain.",vch,NULL,NULL,TO_ROOM);

              continue;
           }

        if ( !IS_SET(vch->more, MORE_BLIND) && number_percent() < 51 )
           {
              SET_BIT(vch->more, MORE_BLIND);
              vch->blind_timer = 6;
              stc("The searing glare from the sword blinds you.\n\r",vch);
              act("$n is blinded from the searing glare of light from the sword.",vch,NULL,NULL,TO_ROOM);
              continue;
           }
           else if ( vch->position != POS_STUNNED )
                   {
                      stop_fighting(vch,TRUE);
                      vch->position = POS_STUNNED;

                      if ( !is_martial( vch ) && !IS_CLASS( vch, CLASS_WISP ) )
                         vch->wait += 12;

                      stc("You fall to the ground in pain as you felt an invisible force smashes it fist into you.\n\r",vch);
                      act("$n falls to the ground clutching $s stomach in pain.",vch,NULL,NULL,TO_ROOM);
                   }
   }

   if ( ch->dpower > 5 )
      ch->dpower = 0;
      else if ( ch->fight_timer > 0 )
              {
                 ch->dpower++;
       
                 if ( ch->dpower == 6 )
                    stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
	        }
}

void do_buddhastomp( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch = NULL;
   CHAR_DATA *vch_next = NULL;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
      }

   act("You mumble '#yAli nura kin...#n' as you rub your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n mumbles '#yAli nura kin...#n' as $e rubs $s hands together.",ch,NULL,NULL,TO_ROOM);

   act("A massive portal to another dimension fades into existence above your head.", ch,NULL,NULL,TO_CHAR );
   act("A massive portal to another dimension fades into existence above $n's head.", ch,NULL,NULL,TO_ROOM );

   act("A HUGE foot descends from the port SMASHING into the ground!", ch,NULL,NULL,TO_CHAR );
   act("A HUGE foot descends from the port SMASHING into the ground!", ch,NULL,NULL,TO_ROOM );
   act("A HUGE foot descends from the port SMASHING into the ground!", ch,NULL,NULL,TO_CHAR );
   act("A HUGE foot descends from the port SMASHING into the ground!", ch,NULL,NULL,TO_ROOM );
   act("There is a MASSIVE explosion as the foot fades away...", ch,NULL,NULL,TO_CHAR );
   act("There is a MASSIVE explosion as the foot fades away...", ch,NULL,NULL,TO_ROOM );

   if ( !IS_NPC( ch ) )
      {
         ch->wait = 6;
         subtract_stock( ch, 300 );
      }

   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;
  
        if ( ( is_same_group(ch,vch) ) || ( is_safe(ch, vch) ) || ( vch == ch ) )
           continue;

        ch->attack_type = ATTACK_MAGICAL;
        ch->attack_var = SOUL;
        ch->attack_focus = AERIAL;

        if ( !IS_NPC( ch ) )
           hurt_person( ch, vch, 3500000, TRUE );
           else hurt_person( ch, vch, 3000000, TRUE );
   }
}

void do_shock_wave( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 200 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to Shock Wave?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      {
         ch->wait = 12;
         subtract_stock( ch, 200 );
      }

   SET_BIT( ch->arena, AFF2_DM );

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You say '#yOh mighty Buddha, strike down the faithless with the beam of truth.#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yOh mighty Buddha, strike down the faithless with the beam of truth.#n'",ch,NULL,NULL,TO_ROOM);
   act("You yell '#rSHoCk WaVe!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#rSHoCk WaVe!#n'",ch,NULL,NULL,TO_ROOM);

   act("You raise your hand in the air as the ethereal sword leaps from your back.", ch,NULL,NULL,TO_CHAR );
   act("$n raises $s hand in the air as the ethereal sword leaps from $s back.", ch,NULL,NULL,TO_ROOM );

   act("You clutch your hand tightly as energy converges to the very tip of the sword.", ch,NULL,NULL,TO_CHAR );
   act("$n clutches $s hand tightly as energy converges to the very tip of the sword.", ch,NULL,NULL,TO_ROOM );
   act("The air splits in half as a transparent figure grabs the sword and swipes at it extremely viciously.", ch,NULL,NULL,TO_CHAR );
   act("The air splits in half as a transparent figure grabs the sword and swipes at it extremely viciously.", ch,NULL,NULL,TO_ROOM );
   act("You grin evilly as $N staggers from a MasSiVe ShOck WAve!!!", ch,NULL,victim,TO_CHAR );
   act("$n grins evilly as you staggers from a MasSiVe ShOck WAve!!!", ch,NULL,victim,TO_VICT );
   act("$n grins evilly as $N staggers from a MasSiVe ShOck WAve!!!", ch,NULL,victim,TO_NOTVICT );

   if ( ch->ctimer[15] == 0 )
      {
         victim->move = UMAX( 0, victim->move - 1000000 );
         victim->mana = UMAX( 0, victim->mana - 1000000 );
   
         if ( !is_martial( victim ) )
            victim->wait = 12;
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = LOW;

   if ( !IS_NPC( ch ) )
      hurt_person( ch, victim, 2000000, TRUE );
      else hurt_person( ch, victim, 3500000, TRUE );
}

void do_zinja_kicks( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DARAMON) )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to kick repeatively?\n\r", ch );
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
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You rush towards $N thrusting your foot in $S chest.", ch,NULL,victim,TO_CHAR );
   act("$n rushes towards you thrusting $s foot in your chest.", ch,NULL,victim,TO_VICT );
   act("$n rushes towards $N thrusting $s foot in $S chest.", ch,NULL,victim,TO_NOTVICT );
   act("You then spin around smashing your foot into the side of $N's face.", ch,NULL,victim,TO_CHAR );
   act("$n then spins around smashing $s foot into the side of your face.", ch,NULL,victim,TO_VICT );
   act("$n then spins around smashing $s foot into the side of $N's face.", ch,NULL,victim,TO_NOTVICT );
   act("You kick $N savagely in the stomach.", ch,NULL,victim,TO_CHAR );
   act("$n kicks you savagely in the stomach.", ch,NULL,victim,TO_VICT );
   act("$n kicks $N savagely in the stomach.", ch,NULL,victim,TO_NOTVICT );
   act("You kick $N savagely in the stomach.", ch,NULL,victim,TO_CHAR );
   act("$n kicks you savagely in the stomach.", ch,NULL,victim,TO_VICT );
   act("$n kicks $N savagely in the stomach.", ch,NULL,victim,TO_NOTVICT );
   act("Suddenly you lean back as you thrust your foot upwards $N's chin, causing $M to stagger.", ch,NULL,victim,TO_CHAR );
   act("Suddenly $n leans back as $e thrusts $s foot upwards your chin, causing you to stagger.", ch,NULL,victim,TO_VICT );
   act("Suddenly $n leans back as $e thrusts $s foot upwards $N's chin, causing $M to stagger.", ch,NULL,victim,TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_chakra( CHAR_DATA *ch, char *argument )
{
   int cost = 500000;
   int heal = 300000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DARAMON) )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->move < cost )
      {
         send_to_char( "You are too tired to heal yourself.\n", ch ); 
         return;
      }

   ch->move -= cost;
   ch->wait = 18;

   act("You mumble '#yNu azumi oi...#n' as you rub your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n mumbles '#yNu azumi oi...#n' as $e rubs $s hands together.",ch,NULL,NULL,TO_ROOM);

   act("A radiance of light shines off your body as you feel better.",ch,NULL,NULL,TO_CHAR);
   act("A radiance of light shines off $n's body as $e feel better.",ch,NULL,NULL,TO_ROOM);

   if ( IS_SET(ch->more, MORE_BLIND) )
      {
         REMOVE_BIT(ch->more, MORE_BLIND);
         ch->blind_timer = 0;
      }

   if ( IS_SET(ch->more, MORE_POISON) )
      REMOVE_BIT(ch->more, MORE_POISON);

   if ( is_affected( ch,skill_lookup("blindness") ) )
      affect_strip( ch, skill_lookup("blindness") );

   if ( is_affected( ch,skill_lookup("poison") ) )
      affect_strip( ch, skill_lookup("poison") );

   if ( is_affected( ch,skill_lookup("cloud kill") ) )
      affect_strip( ch, skill_lookup("cloud kill") );

   if ( is_affected( ch, skill_lookup("drow poison") ) )
      affect_strip( ch, skill_lookup("drow poison") );

   if ( ch->fight_timer > 0 && ch->dpower < 6 )
      {
         ch->dpower++;

         if ( ch->dpower == 6 )
            stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
      }

   hit_gain( ch, 300000 );
   mana_gain( ch, 500000 );
}

void do_f_uppercut( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 750000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }
      
         if ( ch->mana < 500000 )
            {
               send_to_char( "You don't have enough magical energy for that.\n", ch ); 
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to burst into flames?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->mana -= 500000;

   ch->wait = 18;

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You stand erect pushing your hand towards $N.", ch,NULL,victim,TO_CHAR );
   act("$n stands erect pushing $s hand towards you.", ch,NULL,victim,TO_VICT );
   act("$n stands erect pushing $s hand towards $N.", ch,NULL,victim,TO_NOTVICT );
   act("As the ethereal sword flies towards $N, a fiery spirit fades into existence.", ch,NULL,victim,TO_CHAR );
   act("As the ethereal sword flies towards you, a fiery spirit fades into existence.", ch,NULL,victim,TO_VICT );
   act("As the ethereal sword flies towards $N, a fiery spirit fades into existence.", ch,NULL,victim,TO_NOTVICT );
   act("Grabbing the sword, the spirit roars as it drives it viciously upwards $N's chest!", ch,NULL,victim,TO_CHAR );
   act("Grabbing the sword, the spirit roars as it drives it viciously upwards your chest!", ch,NULL,victim,TO_VICT );
   act("Grabbing the sword, the spirit roars as it drives it viciously upwards $N's chest!", ch,NULL,victim,TO_NOTVICT );

   if ( ch->dpower > 5 && ch->pkill < 10 )
      ch->dpower = 0;
      else if ( ch->dpower > 5 )
      {
         act("Suddenly, the spirit roars as it drives it viciously upwards $N's chest again!", ch,NULL,victim,TO_CHAR );
         act("Suddenly, the spirit roars as it drives it viciously upwards your chest again!", ch,NULL,victim,TO_VICT );
         act("Suddenly, the spirit roars as it drives it viciously upwards $N's chest again!", ch,NULL,victim,TO_NOTVICT );
         dam *= 2;
         ch->dpower = 0;
      }
      else if ( ch->fight_timer > 0 )
              {
                 if ( !IS_NPC( ch ) )
                    ch->dpower++;

                 if ( ch->dpower == 6 )
                    stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
              }

   act("You burst into flames as the spirit fades away...", victim,NULL,NULL,TO_CHAR );
   act("$n bursts into flames as the spirit fades away...", victim,NULL,NULL,TO_ROOM );

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) ) 
      SET_BIT( victim->affected_by, AFF_FLAMING );

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = UPPERCUT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_tremor( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 750000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }
      
         if ( ch->mana < 500000 )
            {
               send_to_char( "You don't have enough magical energy for that.\n", ch ); 
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to tremble?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->mana -= 500000;

   ch->wait = 18;

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You stand erect pushing your hand towards $N.", ch,NULL,victim,TO_CHAR );
   act("$n stands erect pushing $s hand towards you.", ch,NULL,victim,TO_VICT );
   act("$n stands erect pushing $s hand towards $N.", ch,NULL,victim,TO_NOTVICT );
   act("The ethereal sword hovers in the air, pulsing off bluish energy.", ch,NULL,NULL,TO_CHAR );
   act("The ethereal sword hovers in the air, pulsing off bluish energy.", ch,NULL,NULL,TO_ROOM );
   act("Suddenly, the sword SmAsH itself into the ground causing a HUGE earthquake.", ch,NULL,NULL,TO_CHAR );
   act("Suddenly, the sword SmAsH itself into the ground causing a HUGE earthquake.", ch,NULL,NULL,TO_ROOM );

   act("As you struggle to stay on your feet, a blurry beam BlAsT from the ground knocking you down!", victim,NULL,NULL,TO_CHAR );
   act("As $n struggles to stay on $s feet, a blurry beam BlAsT from the ground knocking $m down!", victim,NULL,NULL,TO_ROOM );

   if ( ch->dpower > 5 && ch->pkill < 10 )
      ch->dpower = 0;
      else if ( ch->dpower > 5 )
      {
         act("The blurry beam then turns around and SOARS through your defenseless body!", victim,NULL,NULL,TO_CHAR );
         act("The blurry beam then turns around and SOARS through $n's defenseless body!", victim,NULL,NULL,TO_ROOM );
         dam *= 2;
         ch->dpower = 0;
      }
      else if ( ch->fight_timer > 0 )
              {
                 if ( !IS_NPC( ch ) )
                    ch->dpower++;

                 if ( ch->dpower == 6 )
                    stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
              }

   if ( number_percent() > 74 - devil_luck( ch, victim, 5 ) ) 
      victim->move = UMAX( victim->move - 500000, 0 );

   ch->ele_attack = ATTACK_EARTH;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = BLUNT;
   ch->attack_focus = LOW;
   hurt_person( ch, victim, dam, TRUE );
}

void do_chill_blast( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 750000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 569 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }
      
         if ( ch->mana < 500000 )
            {
               send_to_char( "You don't have enough magical energy for that.\n", ch ); 
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to chill?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->mana -= 500000;

   ch->wait = 18;

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You stand erect pushing your hand towards $N.", ch,NULL,victim,TO_CHAR );
   act("$n stands erect pushing $s hand towards you.", ch,NULL,victim,TO_VICT );
   act("$n stands erect pushing $s hand towards $N.", ch,NULL,victim,TO_NOTVICT );
   act("As the ethereal sword flies towards $N, a freezing spirit fades into existence.", ch,NULL,victim,TO_CHAR );
   act("As the ethereal sword flies towards you, a freezing spirit fades into existence.", ch,NULL,victim,TO_VICT );
   act("As the ethereal sword flies towards $N, a freezing spirit fades into existence.", ch,NULL,victim,TO_NOTVICT );
   act("Grabbing the sword, the spirit blows through it softly sending a icestorm towards $N!", ch,NULL,victim,TO_CHAR );
   act("Grabbing the sword, the spirit blows through it softly sending a icestorm towards you!", ch,NULL,victim,TO_VICT );
   act("Grabbing the sword, the spirit blows through it softly sending a icestorm towards $N!", ch,NULL,victim,TO_NOTVICT );
   act("You shiver violently as the cold air freezes your body...", victim,NULL,NULL,TO_CHAR );
   act("$n shivers violently as the cold air freezes $s body...", victim,NULL,NULL,TO_ROOM );

   if ( ch->dpower > 5 && ch->pkill < 10 )
      ch->dpower = 0;
      else if ( ch->dpower > 5 )
      {
          ch->attack_var = BEAM;
          act("The spirit suddenly opens her mouth sending a beam of energy towards you!", victim,NULL,NULL,TO_CHAR );
          act("The spirit suddenly opens her mouth sending a beam of energy towards $n!", victim,NULL,NULL,TO_ROOM );
          act("The ice beam SMASHES through your defenseless body!", victim,NULL,NULL,TO_CHAR );
          act("The ice beam SMASHES through $n's defenseless body!", victim,NULL,NULL,TO_ROOM );

          dam *= 2;
          ch->dpower = 0;
      }
      else if ( ch->fight_timer > 0 )
              {
                 if ( !IS_NPC( ch ) )
                    ch->dpower++;

                 ch->attack_var = BLAST;

                 if ( ch->dpower == 6 )
                    stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
              }

   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, dam, TRUE );
}

void do_l_slash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 750000;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_DARAMON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }
      
         if ( ch->mana < 500000 )
            {
               send_to_char( "You don't have enough magical energy for that.\n", ch ); 
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to electricute?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->mana -= 500000;

   ch->wait = 18;

   act("You are surrounded by mystical energy.", ch,NULL,NULL,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,NULL,TO_ROOM );

   act("You stand erect pushing your hand towards $N.", ch,NULL,victim,TO_CHAR );
   act("$n stands erect pushing $s hand towards you.", ch,NULL,victim,TO_VICT );
   act("$n stands erect pushing $s hand towards $N.", ch,NULL,victim,TO_NOTVICT );
   act("As the ethereal sword flies towards $N, a lightning spirit fades into existence.", ch,NULL,victim,TO_CHAR );
   act("As the ethereal sword flies towards you, a lightning spirit fades into existence.", ch,NULL,victim,TO_VICT );
   act("As the ethereal sword flies towards $N, a lightning spirit fades into existence.", ch,NULL,victim,TO_NOTVICT );
   act("Grabbing the sword, the spirit slashes $N repeatively through $s chest!", ch,NULL,victim,TO_CHAR );
   act("Grabbing the sword, the spirit slashes you repeatively through your chest!", ch,NULL,victim,TO_VICT );
   act("Grabbing the sword, the spirit slashes $N repeatively through $s chest!", ch,NULL,victim,TO_NOTVICT );

   if ( ch->dpower > 5 && ch->pkill < 10 )
      ch->dpower = 0;
      else if ( ch->dpower > 5 )
      {
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_CHAR );
          act("Grabbing the sword, the spirit's image slashes you repeatively through your chest!", ch,NULL,victim,TO_VICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_NOTVICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_CHAR );
          act("Grabbing the sword, the spirit's image slashes you repeatively through your chest!", ch,NULL,victim,TO_VICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_NOTVICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_CHAR );
          act("Grabbing the sword, the spirit's image slashes you repeatively through your chest!", ch,NULL,victim,TO_VICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_NOTVICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_CHAR );
          act("Grabbing the sword, the spirit's image slashes you repeatively through your chest!", ch,NULL,victim,TO_VICT );
          act("Grabbing the sword, the spirit's image slashes $N repeatively through $s chest!", ch,NULL,victim,TO_NOTVICT );

          dam *= 2;
          ch->dpower = 0;
      }
      else if ( ch->fight_timer > 0 )
              { 
                 if ( !IS_NPC( ch ) )
                    ch->dpower++;

                 if ( ch->dpower == 6 )
                    stc("#y#zYou are now one with the universe monk.#n\n\r",ch);
              }

   act("Arches of electricity fill the room as you scream loudly from the pain.", victim,NULL,NULL,TO_CHAR );
   act("Arches of electricity fill the room as $n screams loudly from the pain.", victim,NULL,NULL,TO_ROOM );

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person(ch,victim,dam,TRUE);
}

void do_exploder( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DARAMON) )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->dpower < 6 )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( IS_SET(ch->more3, MORE3_ONE) )
      {
         stc("You are already releasing the true power of your inner being.\n\r",ch);
         return;
      }

   ch->daramon_timer = 10 + UMIN( ch->pkill / 2, 10 );
   ch->dpower = 0;

   SET_BIT(ch->more3, MORE3_ONE);
   act("You mumble '#yZatara Ummba Sei...#n' as you rub your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n mumbles '#yZatara Ummba Sei...#n' as $e rubs $s hands together.",ch,NULL,NULL,TO_ROOM);
   act("The Ethereal Sword leaps from your back as your body glows transparent.",ch,NULL,NULL,TO_CHAR);
   act("The Ethereal Sword leaps from $n's back as $s body glows transparent.",ch,NULL,NULL,TO_ROOM);
   ch->wait = 6;
}

void do_heavenly_gate( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DARAMON) )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 20 )
      {
	   send_to_char("#CYou #caren't one with the #wAllmighty #LBuddha #cyet.#n\n\r",ch);
	   return;
      }

   if ( ch->ctimer[1] > 0 )
      {
	   send_to_char("You are still recovering from the last one.\n\r",ch);
	   return;
      }

   if ( doubleheal > 0 )
      {
	   send_to_char("The gates have already been opened warrior monk.\n\r",ch);
	   return;
      }

   ch->wait = 24;
   doubleheal = 2;
   ch->ctimer[1] = 6;
   SET_BIT( ch->monkstuff, MONK_HEAL );
   act("You preach '#yOh mighty Buddha...#n' as you rub your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n preaches '#yOh mighty Buddha...#n' as $e rubs $s hands together.",ch,NULL,NULL,TO_ROOM);
   act("You then continue '#yPlease give the sufferers a taste of paradise!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n then continues '#yPlease give the sufferers a taste of paradise!#n'",ch,NULL,NULL,TO_ROOM);
   act("Tears run down your eyes as you witness the gates of heaven opening.",ch,NULL,NULL,TO_CHAR);
   do_info( ch, "#wThe #cgates #wto #yheavens #whave been opened #o#ztemporarily#n#w... #CRejoice#w!#n" );
}

void do_shining_soul( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    AFFECT_DATA af;
    int option, count, sn;

    if ( ( sn = skill_lookup( "shining soul" ) ) < 0 )
       return;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DARAMON) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 20 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->mana < 5000000 )
       {
          stc("You don't have enough magical energy to do this technique.\n\r",ch);
          return;
       }
    
    ch->mana -= 5000000;
    ch->wait = 6;

    act("You clench your hand as it radiates off holy energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it radiates off holy energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yMother of the Lands... Give us protection!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yMother of the Lands... Give us protection!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("The area brightens as a series of whirlwinds fill the vicinity.",ch,NULL,NULL,TO_CHAR);
    act("The area brightens as a series of whirlwinds fill the vicinity.",ch,NULL,NULL,TO_ROOM);
    act("Dust and pebbles beneath your feet swirl to the skies as the winds get stronger.",ch,NULL,NULL,TO_CHAR);
    act("Dust and pebbles beneath your feet swirl to the skies as the winds get stronger.",ch,NULL,NULL,TO_ROOM);

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

       act("You smile as the wind protects you from harm.",vch,NULL,NULL,TO_CHAR);
       act("$n smiles as the wind protects $m from harm!",vch,NULL,NULL,TO_ROOM);

       for ( count = 0; count < 3; count++ )
       {
          switch( number_range( 1, 8 ) )
          {
             default:
             case 1: option = APPLY_SAVING_ICE; break;
             case 2: option = APPLY_SAVING_THUNDER; break;
             case 3: option = APPLY_SAVING_HOLY; break;
             case 4: option = APPLY_SAVING_FIRE; break;
             case 5: option = APPLY_SAVING_EARTH; break;
             case 6: option = APPLY_SAVING_DARK; break;
             case 7: option = APPLY_SAVING_LUNAR; break;
             case 8: option = APPLY_SAVING_NATURAL; break;
          }

          af.type      = sn;
          af.duration  = 24;
          af.modifier  = 50;
          af.location  = option;
          af.bitvector = 0;
          affect_to_char( vch, &af );
       }
    }
}
