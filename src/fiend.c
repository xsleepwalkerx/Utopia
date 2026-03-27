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

void do_fiend_damnation( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_FIEND) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( ch->mana < 5000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         stc("They are not here.\n\r",ch);
         return;
      }

   if ( ( sn = skill_lookup( "damnation" ) ) < 0 )
      return;

   if ( is_affected( victim, skill_lookup("damnation") ) )
      {
         stc("The person is already protected by a reflective shield!\n\r",ch);
         return;
      }

   ch->mana -= 5000000;

   (*skill_table[sn].spell_fun) ( sn, 60, ch, victim );
   WAIT_STATE( ch, 12 );
}

void do_false_pride( CHAR_DATA *ch, char *argument )
{
   int sn, level, cost;
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_FIEND) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' || !is_number(arg) )
      {
         send_to_char( "#rSyntax: #eFalsePride #r<value> #e<Target>#r.#n\n\r", ch );
         return;
      }

   level = atoi( arg );

   if ( level < 1 || level > 10 )
      {
         stc( "#rThis is an #einvalid #rnumber. #eThe range that can be used is #r1 - 10#e.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         stc("They are not here.\n\r",ch);
         return;
      }

   cost = level * 500000;

   if ( ch->mana < cost )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( sn = skill_lookup( "false pride" ) ) < 0 )
      return;

   if ( is_affected( victim, skill_lookup("false pride") ) )
      {
         stc("The person's pride is already too good as it is.\n\r",ch);
         return;
      }

   ch->mana -= cost;

   (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
   WAIT_STATE( ch, 12 );
}

void do_fiend_lifespan( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_FIEND) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more, MORE_WALL ) )
      {
         act( "You release the dark energy surrounding your body as your lifeforce dims.", ch, NULL, NULL, TO_CHAR );
         act( "$n releases the dark energy surrounding $s body as $s lifeforce dims.", ch, NULL, NULL, TO_ROOM );
         REMOVE_BIT( ch->more, MORE_WALL );
         return;
      }

   act( "You swerve your hand in the air as you draw a demonic symbol.", ch, NULL, NULL, TO_CHAR );
   act( "$n swerves $s hand in the air as $e draws a demonic symbol.", ch, NULL, NULL, TO_ROOM );
   act( "The symbol suddenly flashes with dark energy as it merge with your body.", ch, NULL, NULL, TO_CHAR );
   act( "The symbol suddenly flashes with dark energy as it merge with $n's body.", ch, NULL, NULL, TO_ROOM );
   act( "Your lifeforce now flares with unholy power as you cackle with glee.", ch, NULL, NULL, TO_CHAR );
   act( "$n's lifeforce now flares with unholy power as $e cackles with glee.", ch, NULL, NULL, TO_ROOM );
   SET_BIT( ch->more, MORE_WALL );
}

void do_stinger( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 561 ) 
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_FIEND) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( ch->move < 250000 )
            {
               stc("You are too tired to execute this technique.\n\r",ch);
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to thrust your blade into?\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   act("You quickly dash towards $N as palish energy shimmers behind you.",ch,NULL,victim,TO_CHAR);
   act("$n quickly dashes towards you as palish energy shimmers behind $m.",ch,NULL,victim,TO_VICT);
   act("$n quickly dashes towards $N as palish energy shimmers behind $m.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yHai!#n' as you DrIvE your blade through $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yHai!#n' as $e DrIvEs $s blade through your chest!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yHai!#n' as $e DrIvEs $s blade through $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly leap back slashing your blade across $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly leaps back slashing $s blade across your chest!",ch,NULL,victim,TO_VICT);
   act("$n suddenly leaps back slashing $s blade across $N's chest!",ch,NULL,victim,TO_NOTVICT);
  
   ch->wait = 12;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = DIRECT;
   ch->ele_attack = ATTACK_THUNDER;

   if ( IS_NPC(ch) )
      hurt_person( ch, victim, 1000000, TRUE );
      else {
              ch->move -= 250000;
              ch->pcdata->powers[DEMON_TRIGGER] = UMIN( URANGE( 100, ch->pkill * 50, 1000 ), ch->pcdata->powers[DEMON_TRIGGER] + 50 );

              hurt_person( ch, victim, 700000, TRUE );
           }

   if ( number_percent() > 50 - devil_luck( ch, victim, 10 ) && victim->position != POS_DEAD )
      {
         act("The blood marks on your body glows #yyellow#n as sparks of electricity fizzes off you.",victim,NULL,NULL,TO_CHAR);
         act("The blood marks on $n's body glows #yyellow#n as sparks of electricity fizzes off $m.",victim,NULL,NULL,TO_ROOM);

         for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
         {
            vch_next = vch->next_in_room;

            if ( is_same_group(ch,vch) || is_safe(ch,vch) || vch == victim )
               continue;

            act("You scream loudly as you are struck by a deadly bolt of energy sizzling from $N's body.",vch,NULL,victim,TO_CHAR);
            act("$n screams loudly as $e is struck by a deadly bolt of energy sizzling from your body.",vch,NULL,victim,TO_VICT);
            act("$n screams loudly as $e is struck by a deadly bolt of energy sizzling from $N's body.",vch,NULL,victim,TO_NOTVICT);

            if ( number_percent() > 74 - devil_luck( ch, vch, 5 ) && !is_martial( vch ) )
               vch->wait += 12;

            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_var = BLAST;
            ch->attack_focus = DIRECT;
            ch->ele_attack = ATTACK_THUNDER;
            hurt_person( ch, vch, 500000, TRUE );
         }
      }
}

void do_round_trip( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 561 ) 
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_FIEND) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 100 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to throw your blade into?\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   act("You curl yourself abruptly as you lean forward holding your blade.",ch,NULL,NULL,TO_CHAR);
   act("$n curls $mself abruptly as $e leans forward holding $s blade.",ch,NULL,NULL,TO_ROOM);
   act("You focus on your demonic powers as your blade sparkles in the light.",ch,NULL,NULL,TO_CHAR);
   act("$n focuses on $s demonic powers as $s blade sparkles in the light.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly snarl '#yHui!#n' as you throw the blade towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly snarls '#yHui!#n' as $e throws the blade towards you!",ch,NULL,victim,TO_VICT);
   act("$n suddenly snarls '#yHui!#n' as $e throws the blade towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("The Sparda Blade spins like a hurricane as deadly arcs of lightning flashes from it!",ch,NULL,NULL,TO_CHAR);
   act("The Sparda Blade spins like a hurricane as deadly arcs of lightning flashes from it!",ch,NULL,NULL,TO_ROOM);
   act("You scream in agony as the blade rips through your chest like a scythe!",victim,NULL,NULL,TO_CHAR);
   act("$n screams in agony as the blade rips through $s chest like a scythe!",victim,NULL,NULL,TO_ROOM);
   act("As you stagger backwards from the pain, you are suddenly struck by an arc of lightning!",victim,NULL,NULL,TO_CHAR);
   act("As $n staggers backwards from the pain, $e is suddenly struck by an arc of lightning!",victim,NULL,NULL,TO_ROOM);
   act("You scream in agony as the blade rips through your chest like a scythe!",victim,NULL,NULL,TO_CHAR);
   act("$n screams in agony as the blade rips through $s chest like a scythe!",victim,NULL,NULL,TO_ROOM);
   act("You scream in agony as the blade rips through your chest like a scythe!",victim,NULL,NULL,TO_CHAR);
   act("$n screams in agony as the blade rips through $s chest like a scythe!",victim,NULL,NULL,TO_ROOM);
   act("As you stagger backwards from the pain, you are suddenly struck by an arc of lightning!",victim,NULL,NULL,TO_CHAR);
   act("As $n staggers backwards from the pain, $e is suddenly struck by an arc of lightning!",victim,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_focus = DIRECT;
   ch->attack_var = SLASH;

   if ( IS_NPC(ch) )
      hurt_person( ch, victim, 1500000, TRUE );
      else {
              ch->wait = 12;
              subtract_stock( ch, 100 );
              SET_BIT( ch->arena, AFF2_DM );

              ch->pcdata->powers[DEMON_TRIGGER] = UMIN( URANGE( 100, ch->pkill * 50, 1000 ), ch->pcdata->powers[DEMON_TRIGGER] + 100 );
              hurt_person( ch, victim, 1000000, TRUE );
           }

   if ( victim == NULL || victim->position <= POS_STUNNED || number_percent() < 60 - devil_luck( ch, victim, 20 ) )
      {
         act( "You extend your hand forward catching your blade as it spins back to you.", ch, NULL, NULL, TO_CHAR );
         act( "$n extends $s hand forward catching $s blade as it spins back to $m.", ch, NULL, NULL, TO_ROOM );
         return;
      }

   one_hit( ch, victim, gsn_punch, 0 );
   one_hit( ch, victim, gsn_kick, 0 );
   one_hit( ch, victim, gsn_punch, 0 );

   show_total_damage( ch, victim );

   act( "You extend your hand forward catching your blade as it spins back to you.", ch, NULL, NULL, TO_CHAR );
   act( "$n extends $s hand forward catching $s blade as it spins back to $m.", ch, NULL, NULL, TO_ROOM );
}

void do_true_demon( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         act( "#eYour eyes becomes dim as you suddenly fall onto your knees.#n", ch, NULL, NULL, TO_CHAR );
         act( "#e$n#e's eyes becomes dim as $e suddenly falls onto $s knees.#n", ch, NULL, NULL, TO_ROOM );
         act( "#eYou look up at the Heavens as blinding light emits off your shadowy body.#n", ch, NULL, NULL, TO_CHAR );
         act( "#e$n#e looks up at the Heavens as blinding light emits off $s shadowy body.#n", ch, NULL, NULL, TO_ROOM );
         act( "#eAs you feel your strength dissolving away, you merge back into your human form.#n", ch, NULL, NULL, TO_CHAR );
         act( "#eAs $n#e feels $s strength dissolving away, $e merges back into $s human form.#n", ch, NULL, NULL, TO_ROOM );

         ch->wait = 12;

         free_string( ch->morph );
         ch->morph = str_dup( "" );

         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

         if ( IS_SET( ch->pcdata->stats[UNI_AFF], VAM_CLAWS ) )
            REMOVE_BIT( ch->pcdata->stats[UNI_AFF], VAM_CLAWS );

         if ( IS_SET( ch->pcdata->stats[UNI_AFF], VAM_FANGS ) )
            REMOVE_BIT( ch->pcdata->stats[UNI_AFF], VAM_FANGS );

         if ( IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
            REMOVE_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );

         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[DEMON_TRIGGER] < 100 )
      {
         stc("You don't have enough demonic energy within you to transform into your true form.\n\r",ch);
         return;
      }

   act( "#eYour eyes #yblaze #ewith #Rdemonic fire #eas you fall to your knees in agony.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n's eyes #yblaze #ewith #Rdemonic fire #eas $e falls to $s knees in agony.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eYou #rscream #eloudly clutching your head in #wpain #eas #ysparks of electricity fizzes #eoff you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n #rscreams #eloudly clutching $s head in #wpain #eas #ysparks of electricity fizzes #eoff $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eThe #ysparks of electricity #wintensifies #ecausing you to tremble as you become #cblurry#e.#n", ch, NULL, NULL, TO_CHAR );
   act( "#eThe #ysparks of electricity #wintensifies #ecausing $n to tremble as $e becomes #cblurry#e.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eYour eyes roll over as the colour quickly drains off your rapidly expanding body.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n's eyes roll over as the colour quickly drains off $s rapidly expanding body.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eYou grin evilly as sharp fangs jolt from the edge of your shadowy mouth.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n grins evilly as sharp fangs jolt from the edge of $s shadowy mouth.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eTwo small stubbles rise from your forehead as they flare with demonic energy.#n", ch, NULL, NULL, TO_CHAR );
   act( "#eTwo small stubbles rise from $n's forehead as they flare with demonic energy.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eA long pair of pitch-black wings grow from your back as you flap them noisily.#n", ch, NULL, NULL, TO_CHAR );
   act( "#eA long pair of pitch-black wings grow from $n's back as $e flaps them noisily.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eA long shadowy tail slides down your back as it fades in and out of existence.#n", ch, NULL, NULL, TO_CHAR );
   act( "#eA long shadowy tail slides down $n's back as it fades in and out of existence.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eYou clench your fists in #ysheer #Rrage #eas you fill the room with deadly sparks of dark energy.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n clenches $s fists in #ysheer #Rrage #eas $e fills the room with deadly sparks of dark energy.#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #eShadow #rDemon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   if ( !IS_SET( ch->pcdata->stats[UNI_AFF], VAM_CLAWS ) )
      SET_BIT( ch->pcdata->stats[UNI_AFF], VAM_CLAWS );

   if ( !IS_SET( ch->pcdata->stats[UNI_AFF], VAM_FANGS ) )
      SET_BIT( ch->pcdata->stats[UNI_AFF], VAM_FANGS );

   if ( !IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
      SET_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );
}

void do_air_raid( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_FIEND) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("But you aren't in your demonic form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[DEMON_TRIGGER] < 50 )
      { 
         stc("You don't have enough demonic energy within you to release the deadly sparks.\n\r",ch);
         return;
      }

   ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit / 200 ) );
   ch->mana = UMIN( ch->max_mana, ch->mana + ( ch->max_mana / 100 ) );
   ch->move = UMIN( ch->max_move, ch->move + ( ch->max_move / 100 ) );
   ch->pcdata->powers[DEMON_TRIGGER] -= 50;
   ch->wait = 6;

   act("You spread your wings as you fly high into air above everyone.",ch,NULL,NULL,TO_CHAR);
   act("$n spreads $s wings as $e flies high into air above everyone.",ch,NULL,NULL,TO_ROOM);
   act("Your eyes glow yellow with intensity as electricity fizzes on your fingertips.",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes glow yellow with intensity as electricity fizzes on $s fingertips.",ch,NULL,NULL,TO_ROOM);
   act("You then spread your fingers as you release deadly arcs of electricity across the room.",ch,NULL,NULL,TO_CHAR);
   act("$n then spreads $s fingers as $e releases deadly arcs of electricity across the room.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) || number_percent() < 20 )
         continue;

      act("You scream loudly as you are struck by a deadly bolt of energy sizzling from $N's body.",vch,NULL,ch,TO_CHAR);
      act("$n screams loudly as $e is struck by a deadly bolt of energy sizzling from your body.",vch,NULL,ch,TO_VICT);
      act("$n screams loudly as $e is struck by a deadly bolt of energy sizzling from $N's body.",vch,NULL,ch,TO_NOTVICT);

      if ( number_percent() > 80 - devil_luck( ch, vch, 5 ) && !is_martial( vch ) )
         vch->wait = UMIN( 18, vch->wait + 8 );

      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_focus = AERIAL;
      ch->attack_var = BLUNT;
      hurt_person( ch, vch, 400000, TRUE );
   }
}

void do_vortex( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_FIEND ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("But you aren't in your demonic form.\n\r",ch);
         return;
      }

   if ( ch->move < 4000000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slam yourself into?\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   act("You spread your wings as you fly high into air above everyone.",ch,NULL,NULL,TO_CHAR);
   act("$n spreads $s wings as $e flies high into air above everyone.",ch,NULL,NULL,TO_ROOM);
   act("Focusing your dark energy on $N, you suddenly fly towards $M at full speed!",ch,NULL,victim,TO_CHAR);
   act("Focusing $s dark energy on you, $n suddenly flies towards you at full speed!",ch,NULL,victim,TO_VICT);
   act("Focusing $s dark energy on $N, $n suddenly flies towards $M at full speed!",ch,NULL,victim,TO_NOTVICT);
   act("At the last second, you curl yourself into a ball as you slam into $N!",ch,NULL,victim,TO_CHAR);
   act("At the last second, $n curls $mself into a ball as $e slams into you!",ch,NULL,victim,TO_VICT);
   act("At the last second, $n curls $mself into a ball as $e slams into $N!",ch,NULL,victim,TO_NOTVICT);
   act("The ground explodes with a flash of brilliant light as you are flung viciously into the ground!",victim,NULL,NULL,TO_CHAR);
   act("The ground explodes with a flash of brilliant light as $n is flung viciously into the ground!",victim,NULL,NULL,TO_ROOM);

   ch->move -= 4000000;
   ch->wait = 12;

   ch->ele_attack = ATTACK_DARK;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = AERIAL;
   ch->attack_var = HIT;
   hurt_person( ch, victim, 1250000, TRUE );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) || vch == victim )
         continue;

      if ( vch->fighting == NULL )
         continue;

      stop_fighting( vch, TRUE );

      special_slam( vch, number_range( 0, 3 ) );
      vch->position = POS_STUNNED;
   }
}

void do_magma_drive( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 561 ) 
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_FIEND) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( ch->move < 500000 )
            {
               stc("You are too tired to execute this technique.\n\r",ch);
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to drive your fist into?\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   act("Your fists burst into demonic flames as you leap towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n's fists burst into demonic flames as $e leaps towards you!",ch,NULL,victim,TO_VICT);
   act("$n's fists burst into demonic flames as $e leaps towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yYa!#n' as you punch $N viciously across the face!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yYa!#n' as $e punches you viciously across the face!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yYa!#n' as $e punches $N viciously across the face!",ch,NULL,victim,TO_NOTVICT);
   act("As $N staggers back from the blazing blow, you crouch on your knees abruptly.",ch,NULL,victim,TO_CHAR);
   act("As you stagger back from the blazing blow, $n crouches on $s knees abruptly.",ch,NULL,victim,TO_VICT);
   act("As $N staggers back from the blazing blow, $n crouches on $s knees abruptly.",ch,NULL,victim,TO_NOTVICT);
   act("You clench your flaming fist as you mutter '#yAhhhhhhhh...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n clenches $s flaming fist as $e mutters '#yAhhhhhhhh...#n'",ch,NULL,NULL,TO_ROOM);
   act("You suddenly DrIvE your fist upwards $N's face as a blast of MaGmA sents $M flying!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly DrIvEs $s fist upwards your face as a blast of MaGmA sents you flying!",ch,NULL,victim,TO_VICT);
   act("$n suddenly DrIvEs $s fist upwards $N's face as a blast of MaGmA sents $M flying!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_NPC( ch ) )
      {
         ch->move -= 500000;
         ch->pcdata->powers[DEMON_TRIGGER] = UMIN( URANGE( 100, ch->pkill * 50, 1000 ), ch->pcdata->powers[DEMON_TRIGGER] + 75 );
      }

   ch->wait = 18;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = UPPERCUT;
   ch->attack_var = BEAM;
   hurt_person( ch, victim, 900000, TRUE );

   if ( victim == NULL || victim->position <= POS_STUNNED || number_percent() < 50 - devil_luck( ch, victim, 5 ) )
      return;

   act("You leap into the air as you kick $N savagely in the stomach!",ch,NULL,victim,TO_CHAR);
   act("$n leaps into the air as $e kicks you savagely in the stomach!",ch,NULL,victim,TO_VICT);
   act("$n leaps into the air as $e kicks $N savagely in the stomach!",ch,NULL,victim,TO_NOTVICT);
   act("You then punch $N hard in the chest as you spin yourself around!",ch,NULL,victim,TO_CHAR);
   act("$n then punches you hard in the chest as $e spins $mself around!",ch,NULL,victim,TO_VICT);
   act("$n then punches $N hard in the chest as $e spins $mself around!",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = AERIAL;
   ch->attack_var = HIT;
   hurt_person( ch, victim, 100000, TRUE );

   act("You land back softly on your feet.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back softly on $s feet.",ch,NULL,NULL,TO_ROOM);
}

void do_lunge_kick( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_FIEND ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 5 )
      {
         stc("You aren't strong enough to use this technique.\n\r",ch);
         return;
      }

   if ( ch->move < 1000000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to drive your foot into?\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   act("You quickly dash towards $N as palish energy shimmers behind you.",ch,NULL,victim,TO_CHAR);
   act("$n quickly dashes towards you as palish energy shimmers behind $m.",ch,NULL,victim,TO_VICT);
   act("$n quickly dashes towards $N as palish energy shimmers behind $m.",ch,NULL,victim,TO_NOTVICT);
   act("You punch $N hard in the guts as you suddenly do a backflip onto your feet.",ch,NULL,victim,TO_CHAR);
   act("$n punches you hard in the guts as $e suddenly does a backflip onto $s feet.",ch,NULL,victim,TO_VICT);
   act("$n punches $N hard in the guts as $e suddenly does a backflip onto $s feet.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yHai!#n' as you DrIvE your blazing foot into $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yHai!#n' as $e DrIvEs $s blazing foot into your chest!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yHai!#n' as $e DrIvEs $s blazing foot into $N's chest!",ch,NULL,victim,TO_NOTVICT);

   ch->pcdata->powers[DEMON_TRIGGER] = UMIN( URANGE( 100, ch->pkill * 50, 1000 ), ch->pcdata->powers[DEMON_TRIGGER] + 50 );

   ch->move -= 1000000;
   ch->wait = 12;

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;

   hurt_person( ch, victim, 700000, TRUE );

   if ( victim == NULL || victim->position <= POS_STUNNED || number_percent() < 50 - devil_luck( ch, victim, 5 ) )
      return;

   act("You scream loudly as #edemonic #Rflames#n suddenly burst from your body scattering all over the vicinity.",victim,NULL,NULL,TO_CHAR);
   act("$n screams loudly as #edemonic #Rflames#n suddenly burst from $s body scattering all over the vicinity.",victim,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) || vch == victim )
         continue;

      act("The ferocious flames from $N's body eats through your flesh as you sizzle.",vch,NULL,victim,TO_CHAR);
      act("The ferocious flames from your body eats through $n's flesh as $e sizzles.",vch,NULL,victim,TO_VICT);
      act("The ferocious flames from $N's body eats through $n's flesh as $e sizzles.",vch,NULL,victim,TO_NOTVICT);

      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_focus = DIRECT;
      ch->attack_var = BLAST;
      hurt_person( ch, vch, 500000, TRUE );
   }
}

void do_meteor_blast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 561 ) 
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_FIEND) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( ch->pkill < 10 )
            {
               stc("You aren't strong enough to use this technique.\n\r",ch);
               return;
            }

         if ( ch->mana < 1500000 && get_stock( ch ) < 200 )
            {
               stc("You are too tired to execute this technique.\n\r",ch);
               return;
            }
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to send a fiery blast at?\n\r", ch );
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

   if ( is_safe(ch,victim) )
      return;

   act( "You softly push your right palm in front of you as you clench your other fist.", ch, NULL, NULL, TO_CHAR );
   act( "$n softly pushes $s right palm in front of $m as $e clenches $s other fist.", ch, NULL, NULL, TO_ROOM );
   act( "Your hand burst into flames as a blast of ferocious energy manifests into existence.", ch, NULL, NULL, TO_CHAR );
   act( "$n's hand bursts into flames as a blast of ferocious energy manifests into existence.", ch, NULL, NULL, TO_ROOM );
   act( "You sneer evilly as you release the blast of energy towards $N!", ch, NULL, victim, TO_CHAR );
   act( "$n sneers evilly as $e releases the blast of energy towards you!", ch, NULL, victim, TO_VICT );
   act( "$n sneers evilly as $e releases the blast of energy towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "The flames sears through your flesh as it sents you slamming onto the ground!", victim, NULL, NULL, TO_CHAR );
   act( "The flames sears through $n's flesh as it sents $m slamming onto the ground!", victim, NULL, NULL, TO_ROOM );

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_focus = AERIAL;
   ch->attack_var = BLAST;

   ch->wait = 12;

   if ( !IS_NPC(ch) && get_stock( ch ) < 200 )
      {
         ch->mana -= 1500000;
         ch->pcdata->powers[DEMON_TRIGGER] = UMIN( URANGE( 100, ch->pkill * 50, 1000 ), ch->pcdata->powers[DEMON_TRIGGER] + 100 );
         hurt_person( ch, victim, 700000, TRUE );
         return;
      }

   act( "You spread your wings as you suddenly fly right above $N!", ch, NULL, victim, TO_CHAR );
   act( "$n spreads $s wings as $e suddenly flies right above you!", ch, NULL, victim, TO_VICT );
   act( "$n spreads $s wings as $e suddenly flies right above $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream '#yKai!#n' as you release a barrage of fireballs onto $N!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yKai!#n' as $e releases a barrage of fireballs onto you!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yKai!#n' as $e releases a barrage of fireballs onto $N!", ch, NULL, victim, TO_NOTVICT );
   act( "Everything becomes white as the fireballs SmAsHeS into your defenseless body!", victim, NULL, NULL, TO_CHAR );
   act( "Everything becomes white as the fireballs SmAsHeS into $n's defenseless body!", victim, NULL, NULL, TO_ROOM );

   if ( IS_NPC(ch) )
      hurt_person( ch, victim, 1000000, TRUE );
      else {
              subtract_stock( ch, 200 );
              SET_BIT( ch->arena, AFF2_DM );

              hurt_person( ch, victim, 2000000, TRUE );
           }
}

void do_fiend_inferno( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next; 
   int dam = 1500000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 561 ) 
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_FIEND) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               stc("But you aren't in your demonic form.\n\r",ch);
               return;
            }

         dam = 3500000;

         ch->wait = 12;
         subtract_stock( ch, 300 );
         SET_BIT( ch->arena, AFF2_DM );
      }
      else dam = 2500000;

   act( "You clench your fists tightly as your eyes suddenly burst with demonic energy.", ch, NULL, NULL, TO_CHAR );
   act( "$n clenches $s fists tightly as $s's eyes suddenly burst with demonic energy.", ch, NULL, NULL, TO_ROOM );
   act( "You mutter '#yAhhhhhh...#n' as the Unholy powers causes your body to tremble.", ch, NULL, NULL, TO_CHAR );
   act( "$n mutters '#yAhhhhhh...#n' as the Unholy powers causes $s body to tremble.", ch, NULL, NULL, TO_ROOM );
   act( "You grin evilly as your body suddenly radiates of blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "$n grin evilly as $s body suddenly radiates of blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "You then mutter '#yDai...#n' as you raise your trembling right fist above you.", ch, NULL, NULL, TO_CHAR );
   act( "$n then mutters '#yDai...#n' as $e raises $s trembling right fist above $m.", ch, NULL, NULL, TO_ROOM );
   act( "You yell '#yWai!#n' as you SmAsH the ground ViCiOuSlY while releasing your demonic powers.", ch, NULL, NULL, TO_CHAR );
   act( "$n yells '#yWai!#n' as $e SmAsHeS the ground ViCiOuSlY while releasing $s demonic powers.", ch, NULL, NULL, TO_ROOM );
   act( "The grounds tremble slightly as alot of smoke starts rising from various parts of the vicinity.", ch, NULL, NULL, TO_CHAR );
   act( "The grounds tremble slightly as alot of smoke starts rising from various parts of the vicinity.", ch, NULL, NULL, TO_ROOM );
   act( "The RoOm SuDdEnLy ExPlOdEs as HuNgRy FlAmEs aNd mEtEoRs FlY vIcIouSly eVeRyWhEre!!!", ch, NULL, NULL, TO_CHAR );
   act( "The RoOm SuDdEnLy ExPlOdEs as HuNgRy FlAmEs aNd mEtEoRs FlY vIcIouSly eVeRyWhEre!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) || IS_CLASS( vch, CLASS_DEMON ) || IS_CLASS( vch, CLASS_FIEND ) )
         continue;

      act( "The demonic hail SlAmS into your body repeatively as you are sent crashing across the room!", vch, NULL, NULL, TO_CHAR );
      act( "The demonic hail SlAmS into $n's body repeatively as $e is sent crashing across the room!", vch, NULL, NULL, TO_ROOM );
      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = AERIAL;
      ch->attack_var = BLAST;
      hurt_person( ch, vch, dam, TRUE );
   }
}
