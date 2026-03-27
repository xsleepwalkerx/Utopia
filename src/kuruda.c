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

void do_orochi_nagi( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC( ch ) && ch->pIndexData->vnum != 564 )
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( !IS_SET( ch->more3, MORE3_HAND ) )
            {
               send_to_char("Your hand isn't enlitted in flames.\n\r",ch);
               return;
            }

         if ( ch->ctimer[9] < 300 )
            {
               send_to_char("Your anger isn't sufficient enough to execute this technique.\n\r",ch);
               return;
            }

         if ( ch->move < 1000000 )
            {
               send_to_char( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   if ( !IS_NPC(ch) )
      {
         ch->move -= 1000000;
         ch->wait = 6;
         ch->ctimer[9] -= 300;
         SET_BIT( ch->arena, AFF2_DM );
      }

   act("You crouch forward as you raise your blazing fist in front of you.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches forward as $e raises $s blazing fist in front of $m.",ch,NULL,NULL,TO_ROOM);
   act("You mutter '#yArrrrrrrrrrghhhh...#n' as your entire body starts trembling.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yArrrrrrrrrrghhhh...#n' as $s entire body starts trembling.",ch,NULL,NULL,TO_ROOM);
   act("You are filled with the power of the Orochi as your hatred goes up a new level.",ch,NULL,NULL,TO_CHAR);
   act("$n seems to tremble with power as $s hatred becomes even worse than normal.",ch,NULL,NULL,TO_ROOM);
   act("Your eyes become radiant symbols of light as energy bursts out of your body.",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes become radiant symbols of light as energy bursts out of $s body.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly yell '#yOchi no ya Lei!#n' as you skid while releasing MaSsIvE FlAmEs!!!",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly yells '#yOchi no ya Lei!#n' as $e skids while releasing MaSsIvE FlAmEs!!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
         continue;

      act("You scream loudly in pain as the ferocious flames eradicates your flesh!",vch,NULL,NULL,TO_CHAR);
      act("$n screams loudly in pain as the ferocious flames eradicates $s flesh!",vch,NULL,NULL,TO_ROOM);
      ch->attack_type = ATTACK_MAGICAL;

      if ( !IS_NPC( ch ) )
         hurt_person( ch, vch, is_elemental( ATTACK_HOLY, vch, 1500000 ), TRUE );
         else hurt_person( ch, vch, is_elemental( ATTACK_HOLY, vch, 2500000 ), TRUE );
   }
}

void perform_shiki( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = 10000;
   bool pre_room;
   int direction;
   int count;
   int range;

   act( "You yell '#yOia De!#n' as you suddenly spin fluidly at $N!", ch, NULL, victim, TO_CHAR );
   act( "$n yells '#yOia De!#n' as $e suddenly spins fluidly at you!", ch, NULL, victim, TO_VICT );
   act( "$n yells '#yOia De!#n' as $e suddenly spins fluidly at $N!", ch, NULL, victim, TO_NOTVICT );
   act( "The air RoArS with POWER as you SmAsH your glowing fist into $N's facE!", ch, NULL, victim, TO_CHAR );
   act( "The air RoArS with POWER as $n SmAsHeS $s glowing fist into your facE!", ch, NULL, victim, TO_VICT );
   act( "The air RoArS with POWER as $n SmAsHeS $s glowing fist into $N's facE!", ch, NULL, victim, TO_NOTVICT );
   SET_BIT( ch->arena, AFF2_DM );

   if ( ch->pcdata->powers[MONK_SHIKI] > 1 )
      {
         direction = number_range( 0, 3 );
         count = 0;

         if ( ch->pcdata->powers[MONK_SHIKI] == 2 )
            {
               range = 3;
               dam = 1200000;
            }
            else {
                    dam = 2000000;
                    range = 10;
                 }

         do
         {
            pre_room = special_slam( victim, direction );
            count++;
         }
         while ( count < range && pre_room != TRUE );

         ch->attack_type = ATTACK_PHYSICAL;
         hurt_person( ch, victim, dam, TRUE );

         if ( victim != NULL && victim->position > POS_STUNNED )
            {
               stop_fighting( victim, TRUE ); 
               victim->wait = 12;
            }
      }

   REMOVE_BIT( ch->arena, AFF2_DM );
   ch->pcdata->powers[MONK_SHIKI] = 0;
   act( "You skid across the floor as you finish your powerful swing.", ch, NULL, NULL, TO_CHAR );
   act( "$n skids across the floor as $e finishes $s powerful swing.", ch, NULL, NULL, TO_ROOM );
}

void do_shiki( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   int move_cost = 500000;
   int cost = 100, shiki;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_MONK ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_HAND ) )
      {
         send_to_char("Your hand isn't enlitted in flames.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp( "semi", arg ) )
      {
         move_cost *= 2;
         cost = 200;
         shiki = 2;
      }
      else if ( !str_cmp( "pure", arg ) )
      {
         move_cost *= 3;
         cost = 300;
         shiki = 3;
      }
      else shiki = 1;

   if ( ch->ctimer[9] < cost )
      {
         send_to_char("Your anger isn't sufficient enough to execute this technique.\n\r",ch);
         return;
      }

   if ( ch->move < move_cost )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ch->strike_timer > 0 )
      {
         argument = one_argument( argument, arg );

         if ( arg[0] == '\0' )
            {
                stc("Whom do you wish to smash your fists into?\n\r",ch);
                return;
            }
    
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
      else {
              if ( ch->fighting == NULL )
                 {
                    stc( "But you aren't in a fight priest.\n\r", ch );
                    return;
                 }

              victim = ch->fighting;
           }

   ch->move -= move_cost;
   ch->ctimer[9] -= cost;

   act( "You twist yourself back as you start charging up your lifeforce.", ch, NULL, NULL, TO_CHAR );
   act( "$n twists $mself back as $e starts charging up $s lifeforce.", ch, NULL, NULL, TO_ROOM );
   act( "You glare at your hand as it starts trembling from the power.", ch, NULL, NULL, TO_CHAR );
   act( "$n glares at $s hand as it starts trembling from the power.", ch, NULL, NULL, TO_ROOM );
   act( "You growl '#yaaaaaarrrrrrgh...#n' as a spiral of flames encircle your feet.", ch, NULL, NULL, TO_CHAR );
   act( "$n growls '#yaaaaaarrrrrrgh...#n' as a spiral of flames encircle $s feet.", ch, NULL, NULL, TO_ROOM );

   if ( shiki == 1 )
      {
         act( "You yell '#yOia De!#n' as you suddenly spin fluidly at $N!", ch, NULL, victim, TO_CHAR );
         act( "$n yells '#yOia De!#n' as $e suddenly spins fluidly at you!", ch, NULL, victim, TO_VICT );
         act( "$n yells '#yOia De!#n' as $e suddenly spins fluidly at $N!", ch, NULL, victim, TO_NOTVICT );
         act( "The air RoArS with POWER as you SmAsH your glowing fist into $N's facE!", ch, NULL, victim, TO_CHAR );
         act( "The air RoArS with POWER as $n SmAsHeS $s glowing fist into your facE!", ch, NULL, victim, TO_VICT );
         act( "The air RoArS with POWER as $n SmAsHeS $s glowing fist into $N's facE!", ch, NULL, victim, TO_NOTVICT );
         ch->attack_type = ATTACK_PHYSICAL;
         SET_BIT( ch->arena, AFF2_DM );
         hurt_person( ch, victim, 500000, TRUE );

         act( "You skid across the floor as you finish your powerful swing.", ch, NULL, NULL, TO_CHAR );
         act( "$n skids across the floor as $e finishes $s powerful swing.", ch, NULL, NULL, TO_ROOM );
      }
      else {
              ch->pcdata->powers[MONK_SHIKI] = shiki;

              if ( ch->strike_timer > 0 )
                 perform_shiki( ch, victim );
                 else ch->nemesis = shiki - 1;
           }
}

void do_spirit_bomb( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   int dam = 500000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 563 ) 
      return;

   argument = one_argument( argument, arg );

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_KURUDA ) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
            dam = 3000000;
            else dam = 2500000;

         ch->ctimer[19] = 4;
         subtract_stock( ch, 300 );
         SET_BIT( ch->arena, AFF2_DM );

         if ( !IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
            SET_BIT( ch->pcdata->powers[MONK_SKILL], MONK_INVIN );
      }

   act("Your eyes glitter with rage as you stand proudly.",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes glitter with rage as $e stands proudly.",ch,NULL,NULL,TO_ROOM);
   act("You clench your fist as sparks of demonic energy fizzes from it.",ch,NULL,NULL,TO_CHAR);
   act("$n clenches $s fist as sparks of demonic energy fizzes from it.",ch,NULL,NULL,TO_ROOM);
   act("You sneer '#ySi!#n' as the energy waves suddenly become very rigid!",ch,NULL,NULL,TO_CHAR);
   act("$n sneers '#ySi!#n' as the energy waves suddenly become very rigid!",ch,NULL,NULL,TO_ROOM);
   act("A swirling vortex manifests from your hand as it starts trembling with intensity.",ch,NULL,NULL,TO_CHAR);
   act("A swirling vortex manifests from $n's hand as it starts trembling with intensity.",ch,NULL,NULL,TO_ROOM);
   act("Your hair flutters in the air as the vortex starts sucking in kiotic energy!",ch,NULL,NULL,TO_CHAR);
   act("$n's hair flutters in the air as the vortex starts sucking in kiotic energy!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
         continue;

      act("You spasm and shudder as a mass of kiotic energy flashes through your body!",vch,NULL,NULL,TO_CHAR);
      act("$n spasms and shudders as a mass of kiotic energy flashes through $s body!",vch,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
  
      if ( number_percent() > 49 )
         ch->attack_focus = OVERHEAD;
         else ch->attack_focus = LOW;

      if ( IS_NPC( ch ) )
         hurt_person( ch, vch, 2500000, TRUE );
         else hurt_person( ch, vch, dam, TRUE );
   }
}

void continue_sbomb( CHAR_DATA *ch )
{
   CHAR_DATA *vch, *vch_next;
   DESCRIPTOR_DATA *d;

   if ( IS_NPC( ch ) )
      return;

   act("Your hair flutters faster in the air as the vortex sucks in more kiotic energy!",ch,NULL,NULL,TO_CHAR);
   act("$n's hair flutters faster in the air as the vortex sucks in more kiotic energy!",ch,NULL,NULL,TO_ROOM);
   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( ch->pkill > 49 && !IS_NPC( vch ) )
         continue;

      if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
         continue;

      act("You spasm and shudder as a mass of kiotic energy flashes through your body!",vch,NULL,NULL,TO_CHAR);
      act("$n spasms and shudders as a mass of kiotic energy flashes through $s body!",vch,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
  
      if ( number_percent() > 49 )
         ch->attack_focus = OVERHEAD;
         else ch->attack_focus = LOW;

      hurt_person( ch, vch, 250000, TRUE );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
         {
            if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
               continue;

            if ( ch->in_room->area != d->character->in_room->area )
               continue;

            act("You spasm and shudder as a mass of kiotic energy flashes through your body!",vch,NULL,NULL,TO_CHAR);
            act("$n spasms and shudders as a mass of kiotic energy flashes through $s body!",vch,NULL,NULL,TO_ROOM);

            d->character->agg = 100;

            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_var = SOUL;
  
            if ( number_percent() > 49 )
               ch->attack_focus = OVERHEAD;
               else ch->attack_focus = LOW;

            hurt_person( ch, d->character, 250000, TRUE );
         }
   }

   REMOVE_BIT( ch->arena, AFF2_DM );
}

void do_ayanae( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 555 && ch->pIndexData->vnum != 563 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_KURUDA ) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 100 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to rip apart?\n\r",ch);
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

   act("You grin evilly as you lean forward, grabbing the edge of your kuruda armor.",ch,NULL,NULL,TO_CHAR);
   act("$n grins evilly as $e leans forward, grabbing the edge of $s kuruda armor.",ch,NULL,NULL,TO_ROOM);
   act("You feel the kiotic energy stirring within you as you leap in front of $N.",ch,NULL,victim,TO_CHAR);
   act("$n feels the kiotic energy stirring within $m as $e leaps in front of you.",ch,NULL,victim,TO_VICT);
   act("$n feels the kiotic energy stirring within $m as $e leaps in front of $N.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly wave your cape across $N's chest as your SpIkEs rip through it!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly waves $s cape across your chest as $s SpIkEs rip through it!",ch,NULL,victim,TO_VICT);
   act("$n suddenly waves $s cape across $N's chest as $s SpIkEs rip through it!",ch,NULL,victim,TO_NOTVICT);
   act("You cackle with glee as you start spinning yourself in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n cackles with glee as $e starts spinning $mself in the air.",ch,NULL,NULL,TO_ROOM);
   act("You snarl '#yGrrrrrr#n' as the spikes on your armor DeVaStAtE $N!",ch,NULL,victim,TO_CHAR);
   act("$n snarls '#yGrrrrrr#n' as the spikes on $s armor DeVaStAtE you!",ch,NULL,victim,TO_VICT);
   act("$n snarls '#yGrrrrrr#n' as the spikes on $s armor DeVaStAtE $N!",ch,NULL,victim,TO_NOTVICT);
   act("You then do a vicious somersault, causing your cape to RuSh up $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n then does a vicious somersault, causing $s cape to RuSh up your body!",ch,NULL,victim,TO_VICT);
   act("$n then does a vicious somersault, causing $s cape to RuSh up $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("$N is catapulted high into the sky as you sneer '#yAi nya Da!#n'",ch,NULL,victim,TO_CHAR);
   act("You are catapulted high into the sky as $n sneers '#yAi nya Da!#n'",ch,NULL,victim,TO_VICT);
   act("$N is catapulted high into the sky as $n sneers '#yAi nya Da!#n'",ch,NULL,victim,TO_NOTVICT);
   act("You silently land back on the ground as you give your cape a quick flick.",ch,NULL,NULL,TO_CHAR);
   act("$n silently lands back on the ground as $e gives $s cape a quick flick.",ch,NULL,NULL,TO_ROOM);

   ch->in_room->blood = UMIN( 1000, ch->in_room->blood + 300 );
   victim->hit -= 250000;

   stop_fighting( victim, TRUE );

   if ( !IS_NPC( ch ) )
      {
         if ( ch->ctimer[7] == 0 )
            victim->air_timer = 1;

         if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
              && ch->ctimer[9] != 100 ) )
            ch->ctimer[7] = 2;

         subtract_stock( ch, 100 );
         SET_BIT( ch->arena, AFF2_DM );
      }
      else {
              ch->fight_timer = 0;
              victim->air_timer = number_range( 1, 2 );
           }
}

void do_oniyaki( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam = 500000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 564 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_KURUDA ) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( ch->pkill < 10 )
            {
               send_to_char("You don't have Nests approval to do this technique yet.\n\r",ch);
               return;
            }

         if ( ch->ctimer[0] > 0 )
            {
               send_to_char( "You are still recovering from the last one.\n\r", ch );
               return;
            }

         if ( ch->move < 500000 )
            {
               send_to_char( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to burn with your fists?\n\r",ch);
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

   if ( !IS_NPC( ch ) )
      {
         ch->move -= 500000;
         victim->move = UMAX( 0, victim->move - 1000000 );
         dam = 1000000;
         ch->ctimer[0] = 4;
      }
      else victim->move = UMAX( 0, victim->move - 200000 );

   act("You crouch forward as you give $N a BlAzInG DrAgOn uppercut with a flick of your hand.",ch,NULL,victim,TO_CHAR);
   act("$n crouches forward as $e gives you a BlAzInG DrAgOn uppercut with a flick of $s hand.",ch,NULL,victim,TO_VICT);
   act("$n crouches forward as $e gives $N a BlAzInG DrAgOn uppercut with a flick of $s hand.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yOia Yah!#n' as you open your hands, DrIvInG the flames through $N.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yOia Yah!#n' as $e opens $s hands, DrIvInG the flames through you.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yOia Yah!#n' as $e opens $s hands, DrIvInG the flames through $N.",ch,NULL,victim,TO_NOTVICT);

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = UPPERCUT;

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
      {
         SET_BIT( victim->affected_by, AFF_FLAMING );
         hurt_person( ch, victim, dam * 1.5, TRUE );
      }
      else hurt_person( ch, victim, dam, TRUE );

   act("You land back on your feet gently as you glance around.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s feet gently as $e glances around.",ch,NULL,NULL,TO_ROOM);
}

void do_ki_charge( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA af;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 555 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_KURUDA ) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( ch->pkill < 5 )
            {
               send_to_char("You don't have Nests approval to do this technique yet.\n\r",ch);
               return;
            }

         if ( ch->move < 1000000 )
            {
               send_to_char( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to knock into the ground?\n\r",ch);
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

   act("You raise your hand in the air as energy radiates of it.",ch,NULL,NULL,TO_CHAR);
   act("$n raises $s hand in the air as energy radiates of it.",ch,NULL,NULL,TO_ROOM);
   act("You charge into $N viciously as you grab him by the neck!",ch,NULL,victim,TO_CHAR);
   act("$n charges into you viciously as $e grabs you by the neck!",ch,NULL,victim,TO_VICT);
   act("$n charges into $N viciously as $e grabs him by the neck!",ch,NULL,victim,TO_NOTVICT);
   act("Pushing your palm into $N's stomach, you SlAm $M into the ground!",ch,NULL,victim,TO_CHAR);
   act("Pushing $s palm into your stomach, $n's SlAmS you into the ground!",ch,NULL,victim,TO_VICT);
   act("Pushing $s palm into $N's stomach, $n's SlAmS $M into the ground!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yMua Wump...#n' as MaSsIvE explosions transcede around $N's body.",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yMua Wump...#n' as MaSsIvE explosions transcede around your body.",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yMua Wump...#n' as MaSsIvE explosions transcede around $N's body.",ch,NULL,victim,TO_NOTVICT);
   act("$N screams in agony as you release your ki into $S body, weakening $M.",ch,NULL,victim,TO_CHAR);
   act("You scream in agony as $n releases $s ki into your body, weakening you.",ch,NULL,victim,TO_VICT);
   act("$N screams in agony as $n releases $s ki into $S body, weakening $M.",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 64 - devil_luck( ch, victim, 15 ) && !IS_SET( victim->more, MORE_BLIND ) )
      {
         victim->blind_timer = 5;
         SET_BIT( victim->more, MORE_BLIND );
      }

   if ( number_percent() > 50 - devil_luck( ch, victim, 15 ) && !IS_SET( victim->more, MORE_PAIN ) )
      SET_BIT( victim->more, MORE_PAIN );

   if ( number_percent() > 50 - devil_luck( ch, victim, 15 ) && is_affected( victim,skill_lookup("slow") ) )
      {
         af.type      = skill_lookup("slow");
         af.duration  = 5;
         af.location  = APPLY_NONE;
         af.modifier  = -1;
         af.bitvector = 0;
         affect_to_char( victim, &af );
      }

   ch->wait = 12;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = LOW;

   if ( !IS_NPC( ch ) )
      {
         ch->move -= 1000000;
         hurt_person( ch, victim, 750000, TRUE );
      }
      else hurt_person( ch, victim, 1000000, TRUE );
}

void do_kyaku( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   bool pre_room;
   int direction;
   int count;
   int dam = 3250000;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 555 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_KURUDA ) )
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

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to go berserk on?\n\r",ch);
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
         if ( ch->ctimer[8] == 0 )
            victim->air_timer = number_range( 2, 3 );

         if ( ch->ctimer[15] > 0 )
            ch->ctimer[8] = 2;

         dam = 1250000;
         subtract_stock( ch, 200 );
         SET_BIT( ch->arena, AFF2_DM );
      }

   act("You spread your arms wide apart as flashes of light radiate through your armor.",ch,NULL,NULL,TO_CHAR);
   act("$n spreads $s arms wide apart as flashes of light radiate through $s armor.",ch,NULL,NULL,TO_ROOM);
   act("You dash towards $N as you twirl your foot into $S face.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you as $e twirls $s foot into your face.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N as $e twirls $s foot into $S face.",ch,NULL,victim,TO_NOTVICT);
   act("You push your palm viciously into $N's neck as your hands pulse with energy.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s palm viciously into your neck as $s hands pulse with energy.",ch,NULL,victim,TO_VICT);
   act("$n pushes $s palm viciously into $N's neck as $s hands pulse with energy.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly crouch on your knee as you RaM your palm into $N's stomach!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly crouches on $s knee as $e RaMs $s palm into your stomach!",ch,NULL,victim,TO_VICT);
   act("$n suddenly crouches on $s knee as $e RaMs $s palm into $N's stomach!",ch,NULL,victim,TO_NOTVICT);
   act("You punch $N viciously in the stomach as you mutter '#yShooooo...#n'",ch,NULL,victim,TO_CHAR);
   act("$n punches $N viciously in the stomach as $e mutters '#yShooooo...#n'",ch,NULL,victim,TO_VICT);
   act("$n punches $N viciously in the stomach as $e mutters '#yShooooo...#n'",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yBai!#n' as you SmAsH your fist down $N's head!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yBai!#n' as $e SmAsHeS $s fist down your head!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yBai!#n' as $e SmAsHeS $s fist down $N's head!",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You PuMmEl your glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_CHAR);
   act("$n PuMmElS $s glowing fists down your head as you screams with pain!",ch,NULL,victim,TO_VICT);
   act("$n PuMmElS $s glowing fists down $N's head as $E screams with pain!",ch,NULL,victim,TO_NOTVICT);
   act("You SmAsH your fist across $N's head as explosive energy flashes off you.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist across your head as explosive energy flashes off $m.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist across $N's head as explosive energy flashes off $m.",ch,NULL,victim,TO_NOTVICT);
   act("You spin yourself around sweeping $N off $S feet.",ch,NULL,victim,TO_CHAR);
   act("$n spins $mself around sweeping you off your feet.",ch,NULL,victim,TO_VICT);
   act("$n spins $mself around sweeping $N off $S feet.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yHa!#n' as you clench your right hand, punching $N viciously in the mouth!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yHa!#n' as $e clenches $s right hand, punching you viciously in the mouth!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yHa!#n' as $e clenches $s right hand, punching $N viciously in the mouth!",ch,NULL,victim,TO_NOTVICT);
   act("Your eyes flashes with intensity as you charge into $N, stunning $M!",ch,NULL,victim,TO_CHAR);
   act("$n's eyes flashes with intensity as $e charges into you, stunning you!",ch,NULL,victim,TO_VICT);
   act("$n's eyes flashes with intensity as $e charges into $N, stunning $M!",ch,NULL,victim,TO_NOTVICT);
   act("Flipping onto your hands, you yell '#yUmm Seia Ta!#n' as your foot SmAsH $N's face!",ch,NULL,victim,TO_CHAR);
   act("Flipping onto $s hands, $n yells '#yUmm Seia Ta!#n' as $s foot SmAsHeS your face!",ch,NULL,victim,TO_VICT);
   act("Flipping onto $s hands, $n yells '#yUmm Seia Ta!#n' as $s foot SmAsHeS $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You spread your feet as they pummel $N repeatively into the air.",ch,NULL,victim,TO_CHAR);
   act("$n spreads $s feet as they pummel you repeatively into the air.",ch,NULL,victim,TO_VICT);
   act("$n spreads $s feet as they pummel $N repeatively into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You land back on your knees as you clutch the ground with sheer hatred.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s knees as $e clutches the ground with sheer hatred.",ch,NULL,NULL,TO_ROOM);
   act("Flipping onto your hands, you yell '#yUmm Seia Ta!#n' as your foot SmAsH $N's face!",ch,NULL,victim,TO_CHAR);
   act("Flipping onto $s hands, $n yells '#yUmm Seia Ta!#n' as $s foot SmAsHeS your face!",ch,NULL,victim,TO_VICT);
   act("Flipping onto $s hands, $n yells '#yUmm Seia Ta!#n' as $s foot SmAsHeS $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You spread your feet as they pummel $N repeatively into the air.",ch,NULL,victim,TO_CHAR);
   act("$n spreads $s feet as they pummel you repeatively into the air.",ch,NULL,victim,TO_VICT);
   act("$n spreads $s feet as they pummel $N repeatively into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You land back on your knees as you clutch the ground with sheer hatred.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s knees as $e clutches the ground with sheer hatred.",ch,NULL,NULL,TO_ROOM);
   act("Flipping onto your hands, you yell '#yUmm Seia Ta!#n' as your foot SmAsH $N's face!",ch,NULL,victim,TO_CHAR);
   act("Flipping onto $s hands, $n yells '#yUmm Seia Ta!#n' as $s foot SmAsHeS your face!",ch,NULL,victim,TO_VICT);
   act("Flipping onto $s hands, $n yells '#yUmm Seia Ta!#n' as $s foot SmAsHeS $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You drill your feet upwards $N's body as you fly into the air.",ch,NULL,victim,TO_CHAR);
   act("$n drills $s feet upwards your body as $e flies into the air.",ch,NULL,victim,TO_VICT);
   act("$n drills $s feet upwards $N's body as $e flies into the air.",ch,NULL,victim,TO_NOTVICT);
   act("You spin backwards driving your blazing foot upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n spins backwards driving $s blazing foot upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n spins backwards driving $s blazing foot upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You land back on your knees as you clutch the ground with sheer hatred.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s knees as $e clutches the ground with sheer hatred.",ch,NULL,NULL,TO_ROOM);
   act("You scream with rage as your body explodes with ferocious energy!",ch,NULL,NULL,TO_CHAR);
   act("$n screams with rage as $s body explodes with ferocious energy!",ch,NULL,NULL,TO_ROOM);
   act("You dash towards $N yelling '#yAhhhhhhh!!!!#n'",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you yelling '#yAhhhhhhh!!!!#n'",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N yelling '#yAhhhhhhh!!!!#n'",ch,NULL,victim,TO_NOTVICT);
   act("The Entire ArEa ErUpTs wItH PaSsIoN as you SmAsH your shoulder into $N!",ch,NULL,victim,TO_CHAR);
   act("The Entire ArEa ErUpTs wItH PaSsIoN as $n SmAsHeS $s shoulder into you!",ch,NULL,victim,TO_VICT);
   act("The Entire ArEa ErUpTs wItH PaSsIoN as $n SmAsHeS $s shoulder into $N!",ch,NULL,victim,TO_NOTVICT);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) || vch == victim )
         continue;

      count = 0;
      direction = number_range( 0, 3 );

      do
      {
         pre_room = special_slam( vch, direction );
         count++;
      }
      while ( count < 5 && pre_room != TRUE );

      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = DIRECT;

      if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
         hurt_person( ch, vch, 3000000, TRUE );
         else hurt_person( ch, vch, 1500000, TRUE );

      if ( vch != NULL && vch->in_room != NULL && vch->position > POS_STUNNED )
         stop_fighting( vch, TRUE );
   }

   act("You mutter '#yTora...#n' as you viciously DrIvE your palm into $N's neck!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yTora...#n' as $e viciously DrIvEs $s palm into your neck!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yTora...#n' as $e viciously DrIvEs $s palm into $N's neck!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yZuus Ken...#n' as you leap into the air, SmAsHiNg your glowing fist into $N!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yZuus Ken...#n' as $e leaps into the air, SmAsHiNg $s glowing fist into you!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yZuus Ken...#n' as $e leaps into the air, SmAsHiNg $s glowing fist into $N!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#ySei!!!#n' as your glowing hands strikes $N viciously across the room!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#ySei!!!#n' as $s glowing hands strikes you viciously across the room!",ch,NULL,victim,TO_VICT);
   act("$n yells '#ySei!!!#n' as $s glowing hands strikes $N viciously across the room!",ch,NULL,victim,TO_NOTVICT); 

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = UPPERCUT;

   if ( ( ( IS_CLASS( victim, CLASS_ANGEL ) || IS_CLASS( victim, CLASS_SHADOW_WAR ) ) && victim->ctimer[8] > 0 )
        || !str_cmp( "N", victim->groove ) || !str_cmp( "A", victim->groove ) )
      dam += 750000;

   if ( IS_NPC( ch ) )
      SET_BIT( ch->arena, AFF2_UB );

   hurt_person( ch, victim, dam, TRUE );

   if ( IS_NPC( ch ) )
      REMOVE_BIT( ch->arena, AFF2_UB );

   stop_fighting( ch, TRUE );

   if ( victim != NULL && victim->in_room != NULL && victim->position > POS_STUNNED )
      {
         if ( IS_NPC( ch ) )
            victim->air_timer = 4;
            else victim->air_timer = 2;

         stop_fighting( victim, TRUE );
      }

   act("You softly land back on your knees out of breath.",ch,NULL,NULL,TO_CHAR);
   act("$n softly lands back on $s knees out of breath.",ch,NULL,NULL,TO_ROOM);
}

void do_ki_punch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( !IS_CLASS( ch, CLASS_KURUDA ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 250000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to go swipe your cape at?\n\r",ch);
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

   act( "You yell '#yHui!#n' as you crouches forward punching $N hard in the guts!", ch, NULL, victim, TO_CHAR );
   act( "$n yells '#yHui!#n' as $e crouches forward punching you hard in the guts!", ch, NULL, victim, TO_VICT );
   act( "$n yells '#yHui!#n' as $e crouches forward punching $N hard in the guts!", ch, NULL, victim, TO_NOTVICT );
   act( "Kiotic energy fizzes from your body as you crouch to your knees in agony.", victim, NULL, NULL, TO_CHAR );
   act( "Kiotic energy fizzes from $n's body as $e crouches to $s knees in agony.", victim, NULL, NULL, TO_ROOM );

   ch->wait = 12;
   ch->move -= 250000;

   if ( number_percent() > 94 - devil_luck( ch, victim, 5 ) )
      victim->stunned = 1;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLUNT;
   ch->attack_focus = LOW;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_swipe( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   bool pre_room;
   int direction;
   int count;

   argument = one_argument( argument, arg );

   if ( !IS_CLASS( ch, CLASS_KURUDA ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 500000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to go swipe your cape at?\n\r",ch);
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

   act("You grin evilly as you lean forward, grabbing the edge of your kuruda armor.",ch,NULL,NULL,TO_CHAR);
   act("$n grins evilly as $e leans forward, grabbing the edge of $s kuruda armor.",ch,NULL,NULL,TO_ROOM);
   act("You feel the kiotic energy stirring within you as you leap in front of $N.",ch,NULL,victim,TO_CHAR);
   act("$n feels the kiotic energy stirring within $m as $e leaps in front of you.",ch,NULL,victim,TO_VICT);
   act("$n feels the kiotic energy stirring within $m as $e leaps in front of $N.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly wave your cape across $N's chest as your SpIkEs rip through it!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly waves $s cape across your chest as $s SpIkEs rip through it!",ch,NULL,victim,TO_VICT);
   act("$n suddenly waves $s cape across $N's chest as $s SpIkEs rip through it!",ch,NULL,victim,TO_NOTVICT);
   act("You then SaVaGeLy swipe your cape to the right as $N screams in pain!",ch,NULL,victim,TO_CHAR);
   act("$n then SaVaGeLy swipes $s cape to the right as you scream in pain!",ch,NULL,victim,TO_VICT);
   act("$n then SaVaGeLy swipes $s cape to the right as $N screams in pain!",ch,NULL,victim,TO_NOTVICT);

   ch->wait = 18;
   ch->move -= 500000;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;

   if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) )
      special_slam( victim, number_range( 0, 3 ) );

   hurt_person( ch, victim, 1000000, TRUE );
}

void do_lockon( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int option;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 563 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_KURUDA ) || ch->class_2 != 0 )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( ch->pkill < 20 )
            {
               send_to_char("You don't have Nests approval to do this technique yet.\n\r",ch);
               return;
            }
      }

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         send_to_char("What element do you wish to focus your attacks on?\n\r", ch);
         return;
      }

   if ( !str_cmp(arg, "fire") )
      option = ATTACK_FIRE;
      else if ( !str_cmp(arg, "ice") )
              option = ATTACK_ICE;
      else if ( !str_cmp(arg, "thunder") )
              option = ATTACK_THUNDER;
      else if ( !str_cmp(arg, "earth") )
              option = ATTACK_EARTH;
      else if ( !str_cmp(arg, "dark") )
              option = ATTACK_DARK;
      else if ( !str_cmp(arg, "holy") )
              option = ATTACK_HOLY;
      else if ( !str_cmp(arg, "none") )
              option = 0;
              else stc("Syntax - #wLockOn #gFire/Ice/Thunder/Earth/Dark/Holy/None\n\r",ch);

   if ( ch->wfocus == option )
      {
         stc( "You are already focusing on that element Commander.\n\r", ch );
         return;
      }

   act( "An eye grid manifests in front of you as you stare coldly around the room.", ch, NULL, NULL, TO_CHAR );
   act( "An eye grid manifests in front of $n as $e stares coldly around the room.", ch, NULL, NULL, TO_ROOM );
   act( "You grin evilly as the gadget's computer calculates the benefits of your mental thoughts.", ch, NULL, NULL, TO_CHAR );
   act( "$n grins evilly as the gadget's computer calculates the benefits of $s thoughts.", ch, NULL, NULL, TO_ROOM );
   act( "You finally wave your cape aside as your body emits a powerful flash of light!", ch, NULL, NULL, TO_CHAR );
   act( "$n finally waves $s cape aside as $s body emits a powerful flash of light!", ch, NULL, NULL, TO_ROOM );
   stc( "You are now focusing your attacks on the new element Commander.\n\r", ch );
   ch->wfocus = option;
   ch->wait = 18;
}

void do_kuruda_armor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_KURUDA) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char("Please specify which piece of ninja eq you wish to make: Armour Boots Claws\n\r",ch);
          send_to_char("Leggings Bracers Tendrils ShoulderGuard.\n\r",ch);
          return;
       }

    if ( ch->gold < 1 )
       {
          send_to_char("You don't have the #w1 #ygold piece#n needed.\n\r",ch);
          return;
       }

    if (!str_cmp(arg,"armour" )) vnum = 424;
    else if (!str_cmp(arg,"boots" )) vnum = 425;
    else if (!str_cmp(arg,"claws" )) vnum = 426;
    else if (!str_cmp(arg,"leggings" )) vnum = 427;
    else if (!str_cmp(arg,"bracers" )) vnum = 428;
    else if (!str_cmp(arg,"tendrils" )) vnum = 429;
    else if (!str_cmp(arg,"shoulderguard" )) vnum = 430;
    else {
            do_kuruda_armor(ch,"");
            return;
         }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL )
       {
          send_to_char("Missing object, please inform a God .\n\r",ch);
          return;
       }

    ch->gold -= 1;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("You make $p from the shadows.",ch,obj,NULL,TO_CHAR);
    act("$n forms $p from the shadows.",ch,obj,NULL,TO_ROOM);
}

void do_kuruda_laugh( CHAR_DATA *ch )
{
   DESCRIPTOR_DATA *d;

   act("You place your hand on your hair as you cackle wildly, '#yMuHaHahaHa!!!#n'", ch, NULL, NULL, TO_CHAR);
   act("$n places $s hand on $s hair as $e cackles wildly, '#yMuHaHahaHa!!!#n'", ch, NULL, NULL, TO_ROOM);
   act("Falling to your knees, your eyes flashes with intensity as you can't control the rage burning within you.", ch, NULL, NULL, TO_CHAR);
   act("Falling to $s knees, $n's eyes flashes with intensity as $e can't control the rage burning within $m.", ch, NULL, NULL, TO_ROOM);

   act("Struggling to stand on your knees, you scream loudly, '#yMuHahahahaHaha!!!#n'", ch, NULL, NULL, TO_CHAR);

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO)
           && d->character->in_room->area == ch->in_room->area )
	{
         if ( ch->in_room == d->character->in_room )
            act("Struggling to stand on $s knees, $n screams loudly, '#yMuHahahahaHaha!!!#n'", ch, NULL, d->character, TO_VICT);
            else act("The hair on your neck stands up as you hear someone in the distance screaming, '#yMuHahahahaHaha!!!#n'", ch, NULL, d->character, TO_VICT);
      }
   }

   act("Placing a hand on your head, you chuckle softly on nothing in particular, your eyes burning with intensity.", ch, NULL, NULL, TO_CHAR);
   act("Placing a hand on $s head, $n chuckles softly on nothing in particular, $s eyes burning with intensity.", ch, NULL, NULL, TO_ROOM);
   act("You grin evilly as you stare wickedly around you in glee.", ch, NULL, NULL, TO_CHAR);
   act("$n grins evilly as $e stares wickedly around $m in glee.", ch, NULL, NULL, TO_ROOM);
   act("The sky goes dark as you cackle '#yMUhAHaHAhaHaHaHa!!!!!#n'", ch, NULL, NULL, TO_CHAR);

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO) )
	{
         if ( ch->in_room == d->character->in_room )
            act("The sky goes dark as $n cackles '#yMUhAHaHAhaHaHaHa!!!!!#n'", ch, NULL, d->character, TO_VICT);
            else if ( ch->in_room->area == d->character->in_room->area )
                    act("The sky goes dark as someone cackles '#yMUhAHaHAhaHaHaHa!!!!!#n'", ch, NULL, d->character, TO_VICT);
                    else act("A chill goes through your bones as you hear someone in the distance cackling, '#yMuHaHAHAhaHahA!!!!!#n'", ch, NULL, d->character, TO_VICT);
      }
   }
}

void do_ki_grasp( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int sn;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 555 ) 
      return;

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whose lifeforce do you wish to disrupt?\n\r",ch);
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

   if ( IS_CLASS(victim,CLASS_DROW) && victim->ctimer[1] == 0 && number_percent() > 74 )
      victim->ctimer[1] = 24;

   if ( IS_CLASS(victim,CLASS_DEMON) && victim->ctimer[2] == 0 && number_percent() > 74 )
      victim->ctimer[2] = 24;

   if ( IS_CLASS(victim,CLASS_PHOENIX) && victim->ctimer[1] == 0 && number_percent() > 74 )
      victim->ctimer[1] = 24;

   if ( IS_CLASS(victim,CLASS_PHOENIX) && victim->ctimer[3] == 0 && number_percent() > 74 )
      victim->ctimer[3] = 24;

   if ( IS_CLASS(victim,CLASS_VAMPIRE) && victim->obeah_timer == 0 && number_percent() > 74 )
      victim->obeah_timer = 100;

   act("You suddenly flip over $N tilting off $S head.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly flips over you tilting off your head.",ch,NULL,victim,TO_VICT);
   act("$n suddenly flips over $N tilting off $S head.",ch,NULL,victim,TO_NOTVICT);
   act("Landing on your feet, your body flares with demonic energy as you grasp $N by the neck.",ch,NULL,victim,TO_CHAR);
   act("Landing on $s feet, $n's body flares with demonic energy as $e grasps you by the neck.",ch,NULL,victim,TO_VICT);
   act("Landing on $s feet, $n's body flares with demonic energy as $e grasps $N by the neck.",ch,NULL,victim,TO_NOTVICT);
   act("You let go an evil grin as you transfuse kiotic energy into $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n lets go an evil grin as $e transfuses kiotic energy into your body!",ch,NULL,victim,TO_VICT);
   act("$n lets go an evil grin as $e transfuses kiotic energy into $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("You scream with intensity as the negative energy sears through your lifeforce!",victim,NULL,NULL,TO_CHAR);
   act("$n screams with intensity as the negative energy sears through $s lifeforce!",victim,NULL,NULL,TO_ROOM);
   act("The ground trembles as you release your full power onto $N!",ch,NULL,victim,TO_CHAR);
   act("The ground trembles as $n releases $s full power onto you!",ch,NULL,victim,TO_VICT);
   act("The ground trembles as $n releases $s full power onto $N!",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly RiP your hand from $N's face causing a huge explosion!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly RiPs $s hand from your face causing a huge explosion!",ch,NULL,victim,TO_VICT);
   act("$n suddenly RiPs $s hand from $N's face causing a huge explosion!",ch,NULL,victim,TO_NOTVICT);
   act("$N flies across the ground dazed as you spread your kiotic energy with a flick of your hand.",ch,NULL,victim,TO_CHAR);
   act("You fly across the ground dazed as $n spreads $s kiotic energy with a flick of $s hand.",ch,NULL,victim,TO_VICT);
   act("$N flies across the ground dazed as $n spreads $s kiotic energy with a flick of $s hand.",ch,NULL,victim,TO_NOTVICT);

   hurt_person( ch, victim, 1250000, TRUE );

   act("Your eyes darken with hatred as you glare at your trembling hand.",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes darken with hatred as $e glares at $s trembling hand.",ch,NULL,NULL,TO_ROOM);
   act("Swerving the dark energy around you, you place your hand on your waist looking discontented.",ch,NULL,NULL,TO_CHAR);
   act("Swerving the dark energy around $m, $n places $s hand on $s waist looking discontented.",ch,NULL,NULL,TO_ROOM);
}

void do_nanase( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 564 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to kick down?\n\r",ch);
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

   if ( !IS_SET( victim->more2, MORE2_NOLEGS ) && number_percent() > 50 )
      SET_BIT( victim->more2, MORE2_NOLEGS );

   act("You suddenly leap into the air as you twist your body around.",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly leaps into the air as $e twists $s body around.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yKyo Ka Sei!#n' as you DrIvE your foot over $N's head!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yKyo Ka Sei!#n' as $e DrIvEs $s foot over your head!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yKyo Ka Sei!#n' as $e DrIvEs $s foot over $N's head!",ch,NULL,victim,TO_NOTVICT);
   act("The impact of your attack SmAsHeS $N into the ground hard as you land on your feet.",ch,NULL,victim,TO_CHAR);
   act("The impact of $n's attack SmAsHeS you into the ground hard as $e lands on $s feet.",ch,NULL,victim,TO_VICT);
   act("The impact of $n's attack SmAsHeS $N into the ground hard as $e lands on $s feet.",ch,NULL,victim,TO_NOTVICT);

   ch->wait = 6;
   hurt_person( ch, victim, 500000, TRUE );
}
