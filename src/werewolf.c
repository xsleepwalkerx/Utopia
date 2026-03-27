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

void do_wolf_graft( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int option = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_GAROU) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[WEREWOLF_CLASS] != 0 )
      {
         stc("You have already discovered your lineage.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp("natural",arg) )
      option = WEREWOLF_NATURAL;
      else if ( !str_cmp("hybrid",arg) )
              {
                 option = WEREWOLF_HYBRID;

                 if ( !IS_SET( ch->pcdata->stats[UNI_AFF], VAM_CLAWS ) )
                    SET_BIT( ch->pcdata->stats[UNI_AFF], VAM_CLAWS );

                 if ( !IS_SET( ch->pcdata->stats[UNI_AFF], VAM_FANGS ) )
                    SET_BIT( ch->pcdata->stats[UNI_AFF], VAM_FANGS );
              }
              else {
                      stc( "#yChoices for #wlineage #yare: #cNatural#w, #RHybrid#w.#n\n\r", ch );
                      return;
                   }

   stc( "#yYou #whave #yfound your #wlineage#y. May your #cvictims #yfear you even more.#n\n\r", ch );
   ch->pcdata->powers[WEREWOLF_CLASS] = option;
}

void do_raging_slash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_GAROU) || ch->pcdata->powers[WEREWOLF_CLASS] != WEREWOLF_HYBRID )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 100000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash viciously?\n\r", ch );
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

   if ( ch->pcdata->powers[WEREWOLF_RAGING] < 11 )
      ch->pcdata->powers[WEREWOLF_RAGING]++;
      else ch->pcdata->powers[WEREWOLF_RAGING] = 1;

   dam = 500000 + ( ch->pcdata->powers[WEREWOLF_RAGING] * 10000 );
   ch->wait = UMAX( 12, 18 - ch->pcdata->powers[WEREWOLF_RAGING] );
   ch->move -= 100000;

   act( "You howl wildly at the sky as your anger boils within you.", ch, NULL, NULL, TO_CHAR );
   act( "$n howls wildly at the sky as $s anger boils within $m.", ch, NULL, NULL, TO_ROOM );
   act( "Trembling with rage, you extract your claws as you dash towards $N!", ch, NULL, victim, TO_CHAR );
   act( "Trembling with rage, $n extracts $s claws as $e dashes towards you!", ch, NULL, victim, TO_VICT );
   act( "Trembling with rage, $n extracts $s claws as $e dashes towards $N!", ch, NULL, victim, TO_NOTVICT );

   switch( number_range( 1, 4 ) )
   {
      default:
      case 3: act( "You quickly roll over slashing violently at $N's legs!", ch, NULL, victim, TO_CHAR );
              act( "$n quickly rolls over slashing violently at your legs!", ch, NULL, victim, TO_VICT );
              act( "$n quickly rolls over slashing violently at $N's legs!", ch, NULL, victim, TO_NOTVICT );
              act( "$N clutches $S legs in pain as you leap away slashing again.", ch, NULL, victim, TO_CHAR );
              act( "You clutch your legs in pain as $n leaps away slashing again.", ch, NULL, victim, TO_VICT );
              act( "$N clutches $S legs in pain as $n leaps away slashing again.", ch, NULL, victim, TO_NOTVICT );
             
              if ( number_percent() > 75 && !IS_SET( victim->more2, MORE2_NOLEGS ) )
                 SET_BIT( victim->more2, MORE2_NOLEGS );
              break;

      case 2: act( "You leap into the air as you slash wildly across $N's chest!", ch, NULL, victim, TO_CHAR );
              act( "$n leaps into the air as $e slashes wildly across your chest!", ch, NULL, victim, TO_VICT );
              act( "$n leaps into the air as $e slashes wildly across $N's chest!", ch, NULL, victim, TO_NOTVICT );
              act( "As $N reels back from the blow, you suddenly slice through $S arms.", ch, NULL, victim, TO_CHAR );
              act( "As you reel back from the blow, $n suddenly slices through your arms.", ch, NULL, victim, TO_VICT );
              act( "As $N reels back from the blow, $n suddenly slices through $S arms.", ch, NULL, victim, TO_NOTVICT );

              spell_weaken( skill_lookup( "weaken" ), 2, ch, victim );
              break;

      case 1: act( "You leap into the air as you slash savagely through $N's face!", ch, NULL, victim, TO_CHAR );
              act( "$n leaps into the air as $e slashes savagely through your face!", ch, NULL, victim, TO_VICT );
              act( "$n leaps into the air as $e slashes savagely through $N's face!", ch, NULL, victim, TO_NOTVICT );
              act( "You subtively roll over as you SlAm your claws into $N's stomach!", ch, NULL, victim, TO_CHAR );
              act( "$n subtively rolls over as $e SlAmS $s claws into your stomach!", ch, NULL, victim, TO_VICT );
              act( "$n subtively rolls over as $e SlAmS $s claws into $N's stomach!", ch, NULL, victim, TO_NOTVICT );

              if ( number_percent() > 50 )
                 dam += number_range( 25000, 50000 );
              break;

      case 4: act( "You leap into the air as you land upon $N viciously!", ch, NULL, victim, TO_CHAR );
              act( "$n leaps into the air as $e lands upon you viciously!", ch, NULL, victim, TO_VICT );
              act( "$n leaps into the air as $e lands upon $N viciously!", ch, NULL, victim, TO_NOTVICT );
              act( "Your eyes fill with glee as you savagely slash and crack $N's head.", ch, NULL, victim, TO_CHAR );
              act( "$n's eyes fill with glee as $e savagely slashes and cracks your head.", ch, NULL, victim, TO_VICT );
              act( "$n's eyes fill with glee as $e savagely slashes and cracks $N's head.", ch, NULL, victim, TO_NOTVICT );

              victim->move = UMAX( 0, victim->move - 200000 );
              ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );
              break;
   }

   hurt_person( ch, victim, dam, TRUE );
}

void do_tornado_slice( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   bool ganged = FALSE;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to break up?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You quickly leap into $N grabbing $M painfully by the neck.", ch, NULL, victim, TO_CHAR );
   act( "$n quickly leaps into you grabbing you painfully by the neck.", ch, NULL, victim, TO_VICT );
   act( "$n quickly leaps into $N grabbing $M painfully by the neck.", ch, NULL, victim, TO_NOTVICT );
   act( "You gaze into $N's eyes evilly as you lift $M above yourself easily.", ch, NULL, victim, TO_CHAR );
   act( "$n gazes into your eyes evilly as $e lifts you above $mself easily.", ch, NULL, victim, TO_VICT );
   act( "$n gazes into $N's eyes evilly as $e lifts $M above $mself easily.", ch, NULL, victim, TO_NOTVICT );
   act( "You then lash $N across the room wildly knocking things aside!", ch, NULL, victim, TO_CHAR );
   act( "$n then lashes you across the room wildly knocking things aside!", ch, NULL, victim, TO_VICT );
   act( "$n then lashes $N across the room wildly knocking things aside!", ch, NULL, victim, TO_NOTVICT );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == victim || is_safe( ch, vch ) || vch->master == NULL || !is_same_group( vch, victim ) )
         continue;

      ganged = TRUE;

      act( "You become dazed as the impact of $N sends you flying across the room!", vch, NULL, victim, TO_CHAR );
      act( "$n becomes dazed as the impact of you sends $m flying across the room!", vch, NULL, victim, TO_VICT );
      act( "$n becomes dazed as the impact of $N sends $m flying across the room!", vch, NULL, victim, TO_NOTVICT );

      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = BLUNT;
      ch->attack_focus = OVERHEAD;

      if ( IS_NPC( vch ) && !( vch->pIndexData->vnum >= 550 && vch->pIndexData->vnum < 571 ) )
         hurt_person( ch, vch, vch->max_hit, TRUE );
         else hurt_person( ch, vch, 1000000, TRUE );

      if ( vch->in_room == NULL || vch->position <= POS_STUNNED )
         continue;

      if ( vch->in_room->vnum >= 33500 && vch->in_room->vnum <= 33800 )
         continue;

      if ( vch->master != NULL )
         stop_follower( vch );

      stop_fighting( vch, TRUE );
      vch->position = POS_STUNNED;
      vch->stunned = 2;
   }

   if ( ganged == TRUE && !( victim->in_room->vnum >= 33500 && victim->in_room->vnum <= 33800 ) )
      {
         if ( victim->master != NULL )
            stop_follower( victim );
  
         victim->stunned = 2;
         victim->position = POS_STUNNED;
      }

   act( "You throw $N above you as you slash wildly across $S stomach!", ch, NULL, victim, TO_CHAR );
   act( "$n throws you above $m as $e slashes wildly across your stomach!", ch, NULL, victim, TO_VICT );
   act( "$n throws $N above $m as $e slashes wildly across $S stomach!", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, you grab $N again SlAmMiNg $M HaRd into the ground!!!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n grabs you again SlAmMiNg you HaRd into the ground!!!", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n grabs $N again SlAmMiNg $M HaRd into the ground!!!", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLUNT;
   ch->attack_focus = OVERHEAD;

   if ( IS_NPC( victim ) && !( victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum < 571 ) )
      hurt_person( ch, victim, victim->max_hit, TRUE );
      else hurt_person( ch, victim, 1000000, TRUE );
}

void do_twisting_moon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_GAROU) || ch->pcdata->powers[WEREWOLF_CLASS] != WEREWOLF_HYBRID )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->mana < 150000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to kick hard?\n\r", ch );
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

   ch->wait = 18;
   ch->mana -= 150000;
   ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );

   act( "As you run towards $N, your body starts flashes with a silver light.", ch, NULL, victim, TO_CHAR );
   act( "As $n runs towards you, $n's body starts flashes with a silver light.", ch, NULL, victim, TO_VICT );
   act( "As $n runs towards $N, $n's body starts flashes with a silver light.", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, you backflip kick $N in the face stylishly, knocking $M off $S face.", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n backflip kicks you in the face stylishly, knocking you off your face.", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n backflip kicks $N in the face stylishly, knocking $M off $S face.", ch, NULL, victim, TO_NOTVICT );

   ch->ele_attack = ATTACK_THUNDER;
   hurt_person( ch, victim, 550000, TRUE );
}

void do_banshee( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         send_to_char( "You are still recovering from the last one.\n", ch );
         return;
      }

   if ( ch->move < 1000000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   ch->ctimer[4] = 6;
   ch->wait = 4;
   ch->move -= 1000000;

   act("A look of concentration crosses your face as you suddenly cackle with glee.",ch,NULL,NULL,TO_CHAR);
   act("A look of concentration crosses $n face as $e suddenly cackles with glee.",ch,NULL,NULL,TO_ROOM);
   act("You howl '#yAaaawoooOOOoH!!!#n' as you let go a MASSIVE shout across the vicinity!",ch,NULL,NULL,TO_CHAR);
   act("$n howl '#yAaaawoooOOOoH!!!#n' as $e lets go a MASSIVE shout across the vicinity!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
         continue;

      stc("You hear an ear poping howl and are suddenly attacked from all sides.\n\r",vch);
      act("You hear an ear poping howl, $n is suddenly attacked from all sides.",vch,NULL,NULL,TO_ROOM);
      act("You clutch your ears in sheer pain as you roll over on the ground dazed.",vch,NULL,NULL,TO_CHAR);
      act("$n clutches $s ears in sheer pain as $e rolls over on the ground dazed.",vch,NULL,NULL,TO_ROOM);

      vch->wait = 18;
      stop_fighting( vch, TRUE );
   }
}

void do_faded_moon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_GAROU) || ch->pcdata->powers[WEREWOLF_CLASS] != WEREWOLF_HYBRID )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 50000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash hard?\n\r", ch );
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

   ch->wait = 18;
   ch->move -= 50000;

   act( "You growl in rage as you get into a crouched position...", ch, NULL, NULL, TO_CHAR );
   act( "$n growls in rage as $e gets into a crouched position...", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, you rush at $N with your claws extended.", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n rushes at you with $s claws extended.", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n rushes at $N with $s claws extended.", ch, NULL, victim, TO_NOTVICT );
   act( "You claw and slash across $N's body as $E screams loudly in pain!", ch, NULL, victim, TO_CHAR );
   act( "$n claws and slashes across your body as you scream loudly in pain!", ch, NULL, victim, TO_VICT );
   act( "$n claws and slashes across $N's body as $E screams loudly in pain!", ch, NULL, victim, TO_NOTVICT );
   act( "Towards the end, you lick the blood off your claw as you grin evilly.", ch, NULL, NULL, TO_CHAR );
   act( "Towards the end, $n licks the blood off $s claw as $e grins evilly.", ch, NULL, NULL, TO_ROOM );

   hurt_person( ch, victim, 600000, TRUE );
}

void do_power_dunk( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_GAROU ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( ch->move < 750000 )
            {
               stc( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to smash into?\n\r", ch );
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

   ch->wait = 18;
   
   if ( !IS_NPC( ch ) )
      ch->move -= 750000;

   act( "You run towards $N screaming '#yPoooowerrrr#n' as your hands start pulsing.", ch, NULL, victim, TO_CHAR );
   act( "$n runs towards you screaming '#yPoooowerrrr#n' as $s hands start pulsing.", ch, NULL, victim, TO_VICT );
   act( "$n runs towards $N screaming '#yPoooowerrrr#n' as $s hands start pulsing.", ch, NULL, victim, TO_NOTVICT );
   act( "You then leap high above $N as your hands get surrounded by a wispy blue aura.", ch, NULL, victim, TO_CHAR );
   act( "$n then leaps high above you as $s hands get surrounded by a wispy blue aura.", ch, NULL, victim, TO_VICT );
   act( "$n then leaps high above $N as your hands get surrounded by a wispy blue aura.", ch, NULL, victim, TO_NOTVICT );
   act( "You scream '#yDunk!!!#n' as you DrIvE your fist into the top of $N's skull.", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yDunk!!!#n' as $e DrIvEs $s fist into the top of your skull.", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yDunk!!!#n' as $e DrIvEs $s fist into the top of $N's skull.", ch, NULL, victim, TO_NOTVICT );

   if ( !IS_SET( victim->more, MORE_PAIN ) )
      SET_BIT( victim->more, MORE_PAIN );

   if ( !IS_CLASS( victim, CLASS_WISP ) )
      ch->ele_attack = ATTACK_THUNDER;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = OVERHEAD;

   hurt_person( ch, victim, 500000, TRUE );
}

void do_bare_knuckle( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_GAROU ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( ch->move < 750000 )
            {
               stc( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to disarm?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->move -= 750000;

   ch->wait = 18;

   act( "You run towards $N while bringing your fists low across the ground.", ch, NULL, victim, TO_CHAR );
   act( "$n runs towards you while bringing $s fists low across the ground.", ch, NULL, victim, TO_VICT );
   act( "$n runs towards $N while bringing $s fists low across the ground.", ch, NULL, victim, TO_NOTVICT );
   act( "You yell '#yBARE KNUCKLLUUEEE!#n' as you swing your fist into the air!", ch, NULL, NULL, TO_CHAR );
   act( "$n yells '#yBARE KNUCKLLUUEEE!#n' as $e swings $s fist into the air!", ch, NULL, NULL, TO_ROOM );
   act( "On its way, your fist lifts $N slightly into the air causing $M to fall to the ground.", ch, NULL, victim, TO_CHAR );
   act( "On its way, $n's fist lifts you slightly into the air causing you to fall to the ground.", ch, NULL, victim, TO_VICT );
   act( "On its way, $n's fist lifts $N slightly into the air causing $M to fall to the ground.", ch, NULL, victim, TO_NOTVICT );

   if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON || IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_ASSASSIN ) )
      && ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL || obj->item_type != ITEM_WEAPON || IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_ASSASSIN ) ) )
      obj = NULL;

   if ( !IS_NPC( victim ) && victim->pcdata->kingdom == 1 && victim->pcdata->clanrank > 1 )
      obj = NULL;

   if ( obj != NULL && number_percent() > 75 )
      {
         obj_from_char( obj );
         obj_to_char( obj, victim );
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 1000000, TRUE );

   act( "You tilt forward as you raise your fists up in victory.", ch, NULL, NULL, TO_CHAR );
   act( "$n tilts forward as $e raises $s fists up in victory.", ch, NULL, NULL, TO_ROOM );
}

void do_burn_knuckle( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_GAROU ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( ch->move < 750000 )
            {
               stc( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
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

   if ( !IS_NPC( ch ) )
      ch->move -= 750000;

   ch->wait = 18;

   act( "You quickly extend your body out, gathering energy around you.", ch, NULL, NULL, TO_CHAR );
   act( "$n quickly extends $s body out, gathering energy around $mself.", ch, NULL, NULL, TO_ROOM );
   act( "As the energy swirls around you, you clench your fist as you leap towards $N!", ch, NULL, victim, TO_CHAR );
   act( "As the energy swirls around $n, $e clenches $s fist as $e leaps towards you!", ch, NULL, victim, TO_VICT );
   act( "As the energy swirls around $n, $e clenches $s fist as $e leaps towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You yell '#yBURN KNUCKLLUUEEE!#n' as you DrIvE your fist into $N's chest.", ch, NULL, victim, TO_CHAR );
   act( "$n yells '#yBURN KNUCKLLUUEEE!#n' as $e DrIvEs $s fist into your chest.", ch, NULL, victim, TO_VICT );
   act( "$n yells '#yBURN KNUCKLLUUEEE!#n' as $e DrIvEs $s fist into $N's chest.", ch, NULL, victim, TO_NOTVICT );
   act( "The impact of your fist sends $N sprawling mid-air onto the ground.", ch, NULL, victim, TO_CHAR );
   act( "The impact of $n's fist sends you sprawling mid-air onto the ground.", ch, NULL, victim, TO_VICT );
   act( "The impact of $n's fist sends $N sprawling mid-air onto the ground.", ch, NULL, victim, TO_NOTVICT );

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) && number_percent() > 75 )
      SET_BIT( victim->affected_by, AFF_FLAMING );

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_power_charge( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_MONK) )
            {
	         stc("Huh?\n\r",ch);
               return;
            }

         if ( !IS_SET( ch->more3, MORE3_WOLF ) )
            {
               stc("But you aren't fighting in a wolf stance.\n\r",ch);
               return;
            }

         if ( ch->pkill < 20 )
            {
               stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
               return;
            }

         if ( ch->move < 2500000 )
            {
               send_to_char( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to stun?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      ch->move -= 2500000;

   ch->wait = 12;
   perform_charge( ch, victim );
}

void perform_charge( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = ( !melee ) ? 500000 : 150000;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLUNT;
   ch->attack_focus = DIRECT;

   if ( number_percent() < 90 - devil_luck( ch, victim, 5 ) || IS_NPC( ch ) )
      {
         act( "You begin to lower your body as you mutter '#yPower...#n'", ch, NULL, NULL, TO_CHAR );
         act( "$n begins to lower $s body as $e mutters '#yPower...#n'", ch, NULL, NULL, TO_ROOM );
         act( "There is a brief pause as you ram your shoulders into $N screaming '#yCharge!#n'", ch, NULL, victim, TO_CHAR );
         act( "There is a brief pause as $n rams $s shoulders into you screaming '#yCharge!#n'", ch, NULL, victim, TO_VICT );
         act( "There is a brief pause as $n rams $s shoulders into $N screaming '#yCharge!#n'", ch, NULL, victim, TO_NOTVICT );
         act( "You stumble backwards slightly, but appear nearly unharmed.", victim, NULL, NULL, TO_CHAR );
         act( "$n stumbles backwards slightly, but appear nearly unharmed.", victim, NULL, NULL, TO_ROOM );
         hurt_person( ch, victim, dam, TRUE );
         return;
      }

   ch->wait = 0;
   act( "As $N begins to attack you, you lower your body muttering '#yPower...#n'", ch, NULL, victim, TO_CHAR );
   act( "As you begin to attack $n, $e lowers $s body muttering '#yPower...#n'", ch, NULL, victim, TO_VICT );
   act( "As $N begins to attack $n, $e lowers $s body muttering '#yPower...#n'", ch, NULL, victim, TO_NOTVICT );
   act( "You ram your shoulders into $N screaming '#yCharge!#n' as you knock $M off $S feet.", ch, NULL, victim, TO_CHAR );
   act( "$n rams $s shoulders into you screaming '#yCharge!#n' as $e knocks you off your feet.", ch, NULL, victim, TO_VICT );
   act( "$n rams $s shoulders into $N screaming '#yCharge!#n' as $e knocks $M off $S feet.", ch, NULL, victim, TO_NOTVICT );

   special_slam( victim, number_range( 0, 3 ) );
   hurt_person( ch, victim, dam * 2, TRUE );
}

void perform_tackle( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act( "You dash towards $N as you suddenly leap onto your hands.", ch, NULL, victim, TO_CHAR );
   act( "$n dashes towards you as $e suddenly leaps onto $s hands.", ch, NULL, victim, TO_VICT );
   act( "$n dashes towards $N as $e suddenly leaps onto $s hands.", ch, NULL, victim, TO_NOTVICT );
   act( "Spinning yourself rapidly, you are pushed into the air screaming '#yRising...#n'", ch, NULL, NULL, TO_CHAR );
   act( "Spinning $mself rapidly, $n is pushed into the air screaming '#yRising...#n'", ch, NULL, NULL, TO_ROOM );
   act( "Your fists SmAsH into $N repeatively as $E is driven into the air as well.", ch, NULL, victim, TO_CHAR );
   act( "$n's fists SmAsH into you repeatively as you are driven into the air as well.", ch, NULL, victim, TO_VICT );
   act( "$n's fists SmAsH into $N repeatively as $E is driven into the air as well.", ch, NULL, victim, TO_NOTVICT );
   act( "You scream '#yTackle!#n' as the impact of your fists sent $N flying!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yTackle!#n' as the impact of $s fists sent you flying!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yTackle!#n' as the impact of $s fists sent $N flying!", ch, NULL, victim, TO_NOTVICT );
   act( "You flip backwards as you land on your feet gently.", ch, NULL, NULL, TO_CHAR );
   act( "$n flips backwards as $e lands on $s feet gently.", ch, NULL, NULL, TO_ROOM );

   if ( number_percent() > 79 && victim->wait < 12 )
      victim->wait = 12;

   if ( number_percent() > 49 && !IS_SET( victim->more, MORE_CONFUSION ) )
      SET_BIT( victim->more, MORE_CONFUSION );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = UPPERCUT;

   if ( IS_NPC( ch ) )
      hurt_person( ch, victim, 1000000, TRUE );
      else hurt_person( ch, victim, 500000, TRUE );
}

void perform_geyser( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = 3000000;

   act( "You cautiously wait until $N attempts an attack, then slam your fist into the ground.", ch, NULL, victim, TO_CHAR );
   act( "$n cautiously waits until you attempt an attack, then slams $s fist into the ground.", ch, NULL, victim, TO_VICT );
   act( "$n cautiously waits until $N attempts an attack, then slams $s fist into the ground.", ch, NULL, victim, TO_NOTVICT );
   act( "The ground trembles from your impact as a blast of ferocious energy flashes into $N!", ch, NULL, victim, TO_CHAR );
   act( "The ground trembles from $n's impact as a blast of ferocious energy flashes into you!", ch, NULL, victim, TO_VICT );
   act( "The ground trembles from $n's impact as a blast of ferocious energy flashes into $N!", ch, NULL, victim, TO_NOTVICT );
   act( "As $N is catapulted into the air, you exclaim '#yTRIPLE#n' punching the ground again.", ch, NULL, victim, TO_CHAR );
   act( "As You are catapulted into the air, $n exclaims '#yTRIPLE#n' punching the ground again.", ch, NULL, victim, TO_VICT );
   act( "As $N is catapulted into the air, $n exclaims '#yTRIPLE#n' punching the ground again.", ch, NULL, victim, TO_NOTVICT );
   act( "You are completely engulfed by the powerful flames as you are blasted even higher in the air.", victim, NULL, NULL, TO_CHAR );
   act( "$n is completely engulfed by the powerful flames as $e is blasted even higher in the air.", victim, NULL, NULL, TO_ROOM );
   act( "You scream '#yGEEEEEEYSEEER!#n' as you SmAsH your fist into the ground one last time.", ch, NULL, NULL, TO_CHAR );
   act( "$n screams '#yGEEEEEEYSEEER!#n' as $e SmAsHeS $s fist into the ground one last time.", ch, NULL, NULL, TO_ROOM );
   act( "You become dazed as the final blast knocks you clearly into the air!", victim, NULL, NULL, TO_CHAR );
   act( "$n becomes dazed as the final blast knocks $m clearly into the air!", victim, NULL, NULL, TO_ROOM );

   ch->ninjastuff = 0;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_var = BLAST;
   ch->attack_focus = UPPERCUT;

   SET_BIT( ch->arena, AFF2_DM );

   if ( ( ( IS_CLASS( victim, CLASS_ANGEL ) || IS_CLASS( victim, CLASS_SHADOW_WAR ) ) && victim->ctimer[8] > 0 )
        || !str_cmp( "N", victim->groove ) || !str_cmp( "A", victim->groove ) )
      dam += 750000;

   if ( IS_NPC( ch ) )
      hurt_person( ch, victim, dam, TRUE );
      else hurt_person( ch, victim, dam + 1000000, TRUE );

   REMOVE_BIT( ch->arena, AFF2_DM );

   if ( victim == NULL || victim->in_room == NULL )
      return;

   stop_fighting( victim, TRUE );
   victim->air_timer = number_range( 1, 4 );

   if ( IS_NPC( ch ) )
      ch->fight_timer = 0;
}

void do_triple_geyser( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( !IS_SET( ch->more3, MORE3_WOLF ) )
            {
               stc("But you aren't fighting in a wolf stance.\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         if ( ch->blocking < 0 )
            {
               stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
               return;
            }

         ch->blocking = 12;
         ch->ninjastuff = 70;
         subtract_stock( ch, 300 );
         SET_BIT( ch->arena, AFF2_AUTOGUARD );
         act("#CYou #wcautiously wait for your #Pfoe #wto execute a #ytechnique #won you.#n",ch,NULL,NULL,TO_CHAR);
         act("#C$n #wlooks like $e is cautiously waiting for #ysomething#w.#n",ch,NULL,NULL,TO_ROOM);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to blast?\n\r", ch );
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

   ch->wait = 12;
   perform_geyser( ch, victim );
}

void do_rising_tackle( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_MONK) )
            {
	         stc("Huh?\n\r",ch);
               return;
            }

         if ( !IS_SET( ch->more3, MORE3_WOLF ) )
            {
               stc("But you aren't fighting in a wolf stance.\n\r",ch);
               return;
            }

         if ( ch->pkill < 20 )
            {
               stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
               return;
            }

         if ( ch->null_timer > 0 )
            {
               stc("You can't do a rising tackle for another few pulses as it is exhausting.\n\r",ch);
               return;
            }

         if ( ch->blocking < 0 )
            {
               stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
               return;
            }

         ch->blocking = 12;
         ch->null_timer = 10;
         SET_BIT( ch->arena, AFF2_AUTOGUARD );
         act("#CYou #wcautiously wait for your #Pfoe #wto execute a #ytechnique #won you.#n",ch,NULL,NULL,TO_CHAR);
         act("#C$n #wlooks like $e is cautiously waiting for #ysomething#w.#n",ch,NULL,NULL,TO_ROOM);

         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slam your fists into?\n\r", ch );
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

   ch->wait = 18;
   perform_tackle( ch, victim );
}

void do_power_wave( CHAR_DATA *ch, char *argument )
{
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA *pexit;
   CHAR_DATA *victim;
   char arg1 [MAX_INPUT_LENGTH];
   char arg2 [MAX_INPUT_LENGTH];
   int door, count, dam = 500000;

   if ( IS_NPC(ch) ) return;

   if ( IS_CLASS(ch, CLASS_MASTER) )
      {
         do_master_wave( ch, argument );
         return;
      }

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_WOLF ) )
      {
         stc("But you aren't fighting in a wolf stance.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for this.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if ( arg1[0] == '\0' )
      {
         send_to_char("#wSyntax - #RPower#yWave #C<Direction> #e<Victim>#w.#n\n\r", ch);
         return;
      }

   if ( arg2[0] == '\0' )
      {
         send_to_char("Please specify a target to send the projectile at.\n\r", ch);
         return;
      }

   if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) )
      door = 0;
      else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east" ) )
              door = 1;
      else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) )
              door = 2;
      else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west" ) )
              door = 3;
              else {
                      send_to_char("You can only do this north, south, east, or west.\n\r", ch);
                      return;
                   }

   location = ch->in_room;
   ch->wait = 12;
   ch->mana -= 1500000;

   act("You yell '#yPoweer Waave!#n' as you suddenly SmAsH your fist onto the ground!",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yPoweer Waave!#n' as $e suddenly SmAsHeS $s fist onto the ground!",ch,NULL,NULL,TO_ROOM);

   if ( ( victim = get_char_room( ch, arg2 ) ) != NULL && !is_safe( ch, victim ) )
      {
         act("You stagger back in pain as a MaSsIvE energy wave SLAMS into you.",victim,NULL,NULL,TO_CHAR);
         act("$n staggers back in pain as a MaSsIvE energy wave SLAMS into $m.",victim,NULL,NULL,TO_ROOM);

         ch->ele_attack = ATTACK_THUNDER;
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = BLAST;
         ch->attack_focus = LOW;
         hurt_person( ch, victim, 500000, TRUE );
      }

   act("A rippling effect flashes across the room as the energy wave goes $T!",ch,NULL,dir_name[door],TO_CHAR);
   act("A rippling effect flashes across the room as the energy wave goes $T!",ch,NULL,dir_name[door],TO_ROOM);

   for ( count = 0; count < 10; count++ )
   {
      if ( ( pexit = ch->in_room->exit[door] ) == NULL || ( to_room = pexit->to_room ) == NULL )
         {
             act("#yThe wave implodes against the wall as it shatters into particles.#n",ch,NULL,NULL,TO_CHAR);
             act("#yThe wave implodes against the wall as it shatters into particles.#n",ch,NULL,NULL,TO_ROOM);
             char_from_room(ch);
             char_to_room(ch,location);
             return;
         }

      char_from_room(ch);
      char_to_room(ch,to_room);

      if ( ( victim = get_char_room( ch, arg2 ) ) != NULL && !is_safe( ch, victim ) )
         {
             act("You stagger back in pain as a MaSsIvE energy wave SLAMS into you.",victim,NULL,NULL,TO_CHAR);
             act("$n staggers back in pain as a MaSsIvE energy wave SLAMS into $m.",victim,NULL,NULL,TO_ROOM);

             ch->ele_attack = ATTACK_THUNDER;
             ch->attack_type = ATTACK_PHYSICAL;
             ch->attack_var = BLAST;
             ch->attack_focus = LOW;
             hurt_person( ch, victim, dam, TRUE );

             char_from_room( ch );
             char_to_room( ch, location );

             act("$n staggers back in pain as a MaSsIvE energy wave SLAMS into $m.",victim,NULL,NULL,TO_ROOM);
             return;
         }

      act("#wA loud crackle fills the room as an energy #Pwave #wflies past you!#n",ch,NULL,NULL,TO_ROOM);
   }

   char_from_room(ch);
   char_to_room(ch,location);
   act("#yThe wave fades away into the atmosphere from the velocity loss.#n",ch,NULL,NULL,TO_CHAR);
   act("#yThe wave fades away into the atmosphere from the velocity loss.#n",ch,NULL,NULL,TO_ROOM);
}

void do_morph( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   bool night = FALSE;
   bool automorph = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_GAROU) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         act( "You clutch your head in agony as you fall to your knees.", ch, NULL, NULL, TO_CHAR );
         act( "$n clutches $s head in agony as $e falls to $s knees.", ch, NULL, NULL, TO_ROOM );
         act( "As you roll over the ground in pain, your facial expressions start to soften.", ch, NULL, NULL, TO_CHAR );
         act( "As $n rolls over the ground in pain, $s facial expressions start to soften.", ch, NULL, NULL, TO_ROOM );
         act( "Your fangs and claws retract into your body as your furry hair drops to the ground.", ch, NULL, NULL, TO_CHAR );
         act( "$n's fangs and claws retract into $s body as $s furry hair drops to the ground.", ch, NULL, NULL, TO_ROOM );

         ch->wait = 4;
         ch->pcdata->powers[WEREWOLF_RAGE] = 0;

         if ( IS_SET( ch->pcdata->stats[UNI_AFF], VAM_CLAWS ) )
            REMOVE_BIT( ch->pcdata->stats[UNI_AFF], VAM_CLAWS );

         if ( IS_SET( ch->pcdata->stats[UNI_AFF], VAM_FANGS ) )
            REMOVE_BIT( ch->pcdata->stats[UNI_AFF], VAM_FANGS );

         free_string( ch->morph );
         ch->morph = str_dup( "" );

         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

         act( "You raise your face with great effort as you struggle back onto your feet.", ch, NULL, NULL, TO_CHAR );
         act( "$n raises $s face with great effort as $e struggles back onto $s feet.", ch, NULL, NULL, TO_ROOM );

         return;
      }

   if ( IS_SET( ch->pcdata->powers[WEREWOLF_SKILLS], WEREWOLF_AUTO ) )
      automorph = TRUE;

   if ( time_info.hour < 6 || time_info.hour > 18 )
      night = TRUE;

   if ( !IS_SET( ch->pcdata->stats[UNI_AFF], VAM_CLAWS ) )
      SET_BIT( ch->pcdata->stats[UNI_AFF], VAM_CLAWS );

   if ( !IS_SET( ch->pcdata->stats[UNI_AFF], VAM_FANGS ) )
      SET_BIT( ch->pcdata->stats[UNI_AFF], VAM_FANGS );

   if ( night || automorph )
      ch->pcdata->powers[WEREWOLF_RAGE] = 1000000;
      else ch->pcdata->powers[WEREWOLF_RAGE] = 500000;

   act( "You clutch your head in agony as you fall to your knees.", ch, NULL, NULL, TO_CHAR );
   act( "$n clutches $s head in agony as $e falls to $s knees.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, your nails get longer as a mass of thick hair covers your body.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, $n's nails get longer as a mass of thick hair covers $s body.", ch, NULL, NULL, TO_ROOM );
   act( "As sharp canine teeth slide out further from your gums, your expression changes.", ch, NULL, NULL, TO_CHAR );
   act( "As sharp canine teeth slide out further from $n's gums, $s expression changes.", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #yWere#owolf#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   if ( night )
      {
         act( "You stand out quickly, spreading your arms apart as you howl at the moon!", ch, NULL, NULL, TO_CHAR );
         act( "$n stands out quickly, spreading $s arms apart as $e howls at the moon!", ch, NULL, NULL, TO_ROOM );
      }
      else {
              act( "You stand out quickly, spreading your arms apart as your eyes glow red with hatred!", ch, NULL, NULL, TO_CHAR );
              act( "$n stands out quickly, spreading $s arms apart as $s eyes glow red with hatred!", ch, NULL, NULL, TO_ROOM );
           }

   act( "Your hair becomes bristly as you crouch forward and growl sinisterly at your target.", ch, NULL, NULL, TO_CHAR );
   act( "$n's hair becomes bristly as $e crouches forward and growls sinisterly at $s target.", ch, NULL, NULL, TO_ROOM );
}

void do_overheat_geyser( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( !IS_SET( ch->more3, MORE3_WOLF ) )
            {
               stc("But you aren't fighting in a wolf stance.\n\r",ch);
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
	   send_to_char( "Whom do you want to stun?\n\r", ch );
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

   ch->wait = 12;

   if ( !IS_NPC( ch ) )
      {
         subtract_stock( ch, 200 );
         SET_BIT( ch->arena, AFF2_DM );
      }

   act( "You bump your shoulder into $N which catches $M offguard.", ch, NULL, victim, TO_CHAR );
   act( "$n bumps $s shoulder into you which catches you offguard.", ch, NULL, victim, TO_VICT );
   act( "$n bumps $s shoulder into $N which catches $M offguard.", ch, NULL, victim, TO_NOTVICT );
   act( "You leap slightly and raise your arm above your head saying '#yOverHeat...#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n leaps slightly and raises $s arm above $s head saying '#yOverHeat...#n'", ch, NULL, NULL, TO_ROOM );
   act( "The impact of your momentum drives $N into the air with you as your hand starts pulsing.", ch, NULL, victim, TO_CHAR );
   act( "The impact of $n's momentum drives you into the air with $m as $s hand starts pulsing.", ch, NULL, victim, TO_VICT );
   act( "The impact of $n's momentum drives $N into the air with $m as $s hand starts pulsing.", ch, NULL, victim, TO_NOTVICT );
   act( "You land back slamming your fist into the ground screaming '#yGeyser!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n lands back slamming $s fist into the ground screaming '#yGeyser!#n'", ch, NULL, NULL, TO_ROOM );
   act( "A wave of yellowish energy blasts from the ground sending you flying into the air.", victim, NULL, NULL, TO_CHAR );
   act( "A wave of yellowish energy blasts from the ground sending $n flying into the air.", victim, NULL, NULL, TO_ROOM );
   act( "You crash back hard onto the ground in a dazed as everything goes bleak.", victim, NULL, NULL, TO_CHAR );
   act( "$n crashes back hard onto the ground in a dazed as everything goes bleak.", victim, NULL, NULL, TO_ROOM );

   if ( ch->ctimer[5] == 0 && !is_martial( victim ) && victim->stunned == 0 )
      victim->stunned = 2;

   if ( ch->ctimer[15] > 0 )
      ch->ctimer[5] = 2;

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_PHYSICAL;   
   ch->attack_var = BLAST;
   ch->attack_focus = UPPERCUT;

   if ( !IS_NPC( ch ) )
      hurt_person( ch, victim, 2000000, TRUE );
      else hurt_person( ch, victim, 1500000, TRUE );
}

void do_buster_wolf( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int bonus = 0;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 554 ) 
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_MONK ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( !IS_SET( ch->more3, MORE3_WOLF ) )
            {
               stc("But you aren't fighting in a wolf stance.\n\r",ch);
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
	   send_to_char( "Whom do you want to burn?\n\r", ch );
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

   ch->wait = 12;

   if ( !IS_NPC( ch ) )
      {
         if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
              && ch->ctimer[9] != 100 ) )
            ch->ctimer[5] = 2;

         subtract_stock( ch, 100 );
         SET_BIT( ch->arena, AFF2_DM );
      }

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) ) 
      SET_BIT( victim->affected_by, AFF_FLAMING );

   act( "You hold your fist to the side as you mutter '#yArr... Ryuuu... ohhh... Kaaaaynnn.#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n holds $s fist to the side as $e mutters '#yArr... Ryuuu... ohhh... Kaaaaynnn.#n'", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly bump into $N with a shoulder movement making it seems you slide into $M.", ch, NULL, victim, TO_CHAR );
   act( "$n suddenly bumps into you with a shoulder movement making it seems $e slides into you.", ch, NULL, victim, TO_VICT );
   act( "$n suddenly bumps into $N with a shoulder movement making it seems $e slides into $M.", ch, NULL, victim, TO_NOTVICT );
   act( "You skid painfully across the ground as you hold your fist right at $N's face.", ch, NULL, victim, TO_CHAR );
   act( "$n skids painfully across the ground as $e holds $s fist right at your face.", ch, NULL, victim, TO_VICT );
   act( "$n skids painfully across the ground as $e holds $s fist right at $N's face.", ch, NULL, victim, TO_NOTVICT );
   act( "You exclaim '#yBUSTAAAA WOOLFFF!#n' as you release a wave of yellowish energy at $N!", ch, NULL, victim, TO_CHAR );
   act( "$n exclaims '#yBUSTAAAA WOOLFFF!#n' as $e releases a wave of yellowish energy at you!", ch, NULL, victim, TO_VICT );
   act( "$n exclaims '#yBUSTAAAA WOOLFFF!#n' as $e releases a wave of yellowish energy at $N!", ch, NULL, victim, TO_NOTVICT );
   act( "The flames eat through your flesh hungrily as you are blast away from the force!", victim, NULL, NULL, TO_CHAR );
   act( "The flames eat through $n's flesh hungrily as $e is blast away from the force!", victim, NULL, NULL, TO_ROOM );

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;   
   ch->attack_var = BLAST;
   ch->attack_focus = OVERHEAD;

   if ( ch->ctimer[5] == 0 )
      {
         special_slam( victim, number_range( 0, 3 ) );
         victim->move = UMAX( 0, victim->move - 1500000 );
      }

   if ( !IS_NPC( ch ) )
      hurt_person( ch, victim, 1000000, TRUE );
      else hurt_person( ch, victim, 2000000, TRUE );

   if ( IS_NPC( ch ) )
      ch->fight_timer = 0;
}

void do_omega_rave( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int dam = number_range( 1400000, 1600000 );

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_GAROU ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               stc("You can only do this technique in your human form.\n\r",ch);
               return;
            }

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               stc( "You aren't strong enough to execute this technique yet.\n\r", ch );
               return;
            }

         if ( ch->ctimer[10] > 0 )
            {
               stc( "You are still recovering from the last one.\n\r", ch );
               return;
            }

         if ( ch->move < 1500000 )
            {
               stc( "You are too tired to execute this technique.\n\r", ch );
               return;
            }
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to rip apart?\n\r", ch );
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

   if ( !IS_NPC( ch ) )
      {
         ch->ctimer[10] = 4;
         ch->move -= 1500000;
      }

   ch->wait = 12;

   if ( time_info.hour < 6 || time_info.hour > 18 )
      dam += 500000;

   SET_BIT( ch->pcdata->powers[WEREWOLF_SKILLS], WEREWOLF_AUTO );

   do_morph( ch, "" );

   REMOVE_BIT( ch->pcdata->powers[WEREWOLF_SKILLS], WEREWOLF_AUTO );
   hurt_person( ch, victim, dam, TRUE );
}

void do_wolf_pounce( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_GAROU ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
      {
         stc( "You aren't strong enough to execute this technique yet.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can only do this technique in your true form.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         stc( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      {
	   send_to_char( "Whom do you want to pounce onto?\n\r", ch );
	   return;
      }
    
   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }
   
   if ( is_safe( ch, victim ) )
      return;

   ch->wait = 18;
   ch->move -= 1500000;

   if ( number_percent() < 55 )
      {
         act( "As you leap towards $N with claws extended, $E barely rolls out of the way.", ch, NULL, victim, TO_CHAR );
         act( "As $n leaps towards you with claws extended, you barely roll out of the way.", ch, NULL, victim, TO_VICT );
         act( "As $n leaps towards $N with claws extended, $E barely rolls out of the way.", ch, NULL, victim, TO_NOTVICT );
         act( "Growling in anger, you quickly jump back into an attacking position.", ch, NULL, NULL, TO_CHAR );
         act( "Growling in anger, $n quickly jumps back into an attacking position.", ch, NULL, NULL, TO_ROOM );
         return;
      }

   act("You jump onto $N and pin $S arms to the ground with a sinister look on your face.",ch, NULL, victim, TO_CHAR );
   act("$n jumps onto you and pins your arms to the ground with a sinister look on $s face.",ch, NULL, victim, TO_VICT );
   act("$n jumps onto $N and pins $S arms to the ground with a sinister look on $s face.",ch, NULL, victim, TO_NOTVICT );
   act("You then clamp onto $N's windpipe and start ripping it out with your teeth ferociously.",ch, NULL, victim, TO_CHAR );
   act("$n then clamps onto your windpipe and start ripping it out with $s teeth ferociously.",ch, NULL, victim, TO_VICT );
   act("$n then clamps onto $N's windpipe and start ripping it out with $s teeth ferociously.",ch, NULL, victim, TO_NOTVICT );
   act("After awhile, you toss it out of your mouth as if you were wiping your face.",ch, NULL, NULL, TO_CHAR );
   act("After awhile, $n tosses it out of $s mouth as if $e was wiping $s face.",ch, NULL, NULL, TO_ROOM );

   ch->attack_type = ATTACK_PHYSICAL; 
   hurt_person( ch, victim, 1500000, TRUE );
}

void do_neck_throttle( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_GAROU ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
      {
         stc( "You aren't strong enough to execute this technique yet.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can only do this technique in your true form.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         stc( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to choke?\n\r", ch );
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

   ch->wait = 18;
   ch->move -= 1500000;

   act("You viciously grab $N by the throat as you start tossing $M around like a ragged doll!",ch, NULL, victim, TO_CHAR );
   act("$n viciously grabs you by the throat as $e starts tossing you around like a ragged doll!",ch, NULL, victim, TO_VICT );
   act("$n viciously grabs $N by the throat as $e starts tossing $M around like a ragged doll!",ch, NULL, victim, TO_NOTVICT );
   act("As $N hangs lifelessly before you, you suddenly breathe a disgusting stench onto $S face.",ch, NULL, victim, TO_CHAR );
   act("As you hang lifelessly before $n, $e suddenly breathes a disgusting stench onto your face.",ch, NULL, victim, TO_VICT );
   act("As $N hangs lifelessly before $n, $e suddenly breathes a disgusting stench onto $S face.",ch, NULL, victim, TO_NOTVICT );
   act("$N reeks badly as you slam $M hard into the ground!",ch, NULL, victim, TO_CHAR );
   act("You reek badly as $n slams you hard into the ground!",ch, NULL, victim, TO_VICT );
   act("$N reeks badly as $n slams $M hard into the ground!",ch, NULL, victim, TO_NOTVICT );

   if ( number_percent() > 25 && !IS_SET( victim->more, MORE_HEX ) )
      SET_BIT( victim->more, MORE_HEX );

   if ( number_percent() > 50 && !IS_SET( victim->more, MORE_BLIND ) )
      {
         SET_BIT( victim->more, MORE_BLIND );
         victim->blind_timer = 5;
      }

   if ( number_percent() > 25 && !IS_SET( victim->more, MORE_POISON ) )
      SET_BIT( victim->more, MORE_POISON );
 
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_burning_rave( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_GAROU ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
      {
         stc( "You aren't strong enough to execute this technique yet.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can only do this technique in your true form.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         stc( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release your rage on?\n\r", ch );
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

   ch->pcdata->powers[WEREWOLF_RAGE] = UMIN( 1000000, ch->pcdata->powers[WEREWOLF_RAGE] + 100000 );
   ch->wait = 18;
   ch->move -= 1500000;

   act("Using your natural rage, you howl as you beat $N into submission...",ch, NULL, victim, TO_CHAR );
   act("Using $n's natural rage, $e howls as $e beats you into submission...",ch, NULL, victim, TO_VICT );
   act("Using $n's natural rage, $e howls as $e beats $N into submission...",ch, NULL, victim, TO_NOTVICT );
   act("You feel fresh energy flowing through your vein as your swipes become more vicious.",ch, NULL, NULL, TO_CHAR );
   act("$n feels fresh energy flowing through $s vein as $s swipes become more vicious.",ch, NULL, NULL, TO_ROOM );
   act("After slashing $N's chest, you backhand $N into the ground, grinning evilly.",ch, NULL, victim, TO_CHAR );
   act("After slashing your chest, $n backhands you into the ground, grinning evilly.",ch, NULL, victim, TO_VICT );
   act("After slashing $N's chest, $n backhands $N into the ground, grinning evilly.",ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_moon_eclipse( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int timer = 10;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_GAROU ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill > 49 )
      timer = 11;

   if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
      {
         stc( "You aren't strong enough to execute this technique yet.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can only do this technique in your true form.\n\r",ch);
         return;
      }

   if ( ch->ctimer[timer] > 0 )
      {
         stc( "You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->move < 3000000 )
      {
         stc( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release your rage on?\n\r", ch );
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

   ch->ctimer[timer] = 4;
   ch->move -= 3000000;

   act("Your body shimmers with moon energy as you howl wildly at the moon.",ch, NULL, NULL, TO_CHAR );
   act("$n's body shimmers with moon energy as $e howls wildly at the moon.",ch, NULL, NULL, TO_ROOM );
   act("Suddenly, you rush towards $N as your claws sparkle in the light.",ch, NULL, victim, TO_CHAR );
   act("Suddenly, $n rushes towards you as $s claws sparkle in the light.",ch, NULL, victim, TO_VICT );
   act("Suddenly, $n rushes towards $N as $s claws sparkle in the light.",ch, NULL, victim, TO_NOTVICT );
   act("Your body moves like a blur as you beat $N down to a pulp.",ch, NULL, victim, TO_CHAR );
   act("$n's body moves like a blur as $e beats you down to a pulp.",ch, NULL, victim, TO_VICT );
   act("$n's body moves like a blur as $e beats $N down to a pulp.",ch, NULL, victim, TO_NOTVICT );
   act("You then hit $N with an explosive uppercut with sends $M into the atmosphere.",ch, NULL, victim, TO_CHAR );
   act("$n then hits you with an explosive uppercut with sends you into the atmosphere.",ch, NULL, victim, TO_VICT );
   act("$n then hits $N with an explosive uppercut with sends $M into the atmosphere.",ch, NULL, victim, TO_NOTVICT );
   act("As $N starts falling back towards the earth, all of your energy moves to your elbow.",ch, NULL, victim, TO_CHAR );
   act("As you start falling back towards the earth, all of $n's energy moves to $s elbow.",ch, NULL, victim, TO_VICT );
   act("As $N starts falling back towards the earth, all of $n's energy moves to $s elbow.",ch, NULL, victim, TO_NOTVICT );

   do_morph( ch, "" );
   ch->wait = 6;

   act("You then DrIvE your glowing elbow into $N, SmAsHInG $M through the ground!",ch, NULL, victim, TO_CHAR );
   act("$n then DrIvEs $s glowing elbow into you, SmAsHInG you through the ground!",ch, NULL, victim, TO_VICT );
   act("$n then DrIvEs $s glowing elbow into $N, SmAsHInG $M through the ground!",ch, NULL, victim, TO_NOTVICT );

   victim->mana = UMAX( 0, victim->mana - ( victim->max_mana / 4 ) );
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 2500000, TRUE );
}

void do_moon_dance( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   int dam = 500000;
   int bonus = 0;
   int timer = 10;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_GAROU ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill > 49 )
      timer = 11;

   if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
      {
         stc( "You aren't strong enough to execute this technique yet.\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can only do this technique in your true form.\n\r",ch);
         return;
      }

   if ( ch->ctimer[timer] > 0 )
      {
         stc( "You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->move < 3000000 )
      {
         stc( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release your rage on?\n\r", ch );
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

   ch->move -= 3000000;
   ch->ctimer[timer] = 4;
   ch->wait = 6;

   act("Your body shimmers with moon energy as you howl wildly at the moon.",ch, NULL, NULL, TO_CHAR );
   act("$n's body shimmers with moon energy as $e howls wildly at the moon.",ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( ch == vch || victim == vch || !is_same_group( victim, vch ) )
         continue;

      bonus += 1000000;

      act( "You turn around briskly growling at $N as you make a subtle calculation.", ch, NULL, vch, TO_CHAR );
      act( "$n turns around briskly growling at you as $e makes a subtle calculation.", ch, NULL, vch, TO_VICT );
      act( "$n turns around briskly growling at $N as $e makes a subtle calculation.", ch, NULL, vch, TO_NOTVICT );
   }

   bonus = UMIN( bonus, 4000000 );

   act("Surrounded, you beckon all to attack you as foam drips from your mouth.",ch, NULL, NULL, TO_CHAR );
   act("Surrounded, $n beckons all to attack $m as foam drips from $s mouth.",ch, NULL, NULL, TO_ROOM );
   act("At the same time, you howl loudly causing everyone to cover their ears in pain!",ch, NULL, NULL, TO_CHAR );
   act("At the same time, $n howls loudly causing you to cover your ears in pain!",ch, NULL, NULL, TO_ROOM );
   act("You then dash towards $N as your body turns into liquid silver!",ch, NULL, victim, TO_CHAR );
   act("$n then dashes towards you as $s body turns into liquid silver!",ch, NULL, victim, TO_VICT );
   act("$n then dashes towards $N as $s body turns into liquid silver!",ch, NULL, victim, TO_NOTVICT );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
         continue;

      act("You slash wildly all over $N as the wind SlIcEs through $S flesh!",ch, NULL, vch, TO_CHAR );
      act("$n slashes wildly all over you as the wind SlIcEs through $S flesh!",ch, NULL, vch, TO_VICT );
      act("$n slashes wildly all over $N as the wind SlIcEs through $S flesh!",ch, NULL, vch, TO_NOTVICT );

      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_PHYSICAL;
      hurt_person( ch, vch, dam + bonus, TRUE );
   }
}
