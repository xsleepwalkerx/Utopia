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

void do_paralysis( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int chance, gain = 0;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_BAT
        && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MALICE )
      {
         stc("You can't do this mystical ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] >= ch->pcdata->powers[VAMPIRE_MAXBLOOD] )
      {
         stc("Your bloodlust is already sated.\n\r",ch);
         return;
      }

   if ( ch->move < 100000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to paralyse?\n\r", ch );
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

   ch->move -= 100000;
   ch->wait = 18;

   if ( ch->fighting == NULL )
      {
         if ( ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
            chance = number_range( 35, 50 );
            else chance = 65;
      }
      else {
              if ( ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
                 chance = 35;
                 else chance = number_range( 35, 50 );
           }

   if ( IS_NPC( victim ) )
      chance += number_range( 10, 25 );

   if ( chance < number_percent() )
      {
         act( "You try to focus on $N's mind but fail from the lack of concentration.", ch, NULL, victim, TO_CHAR );
         return;
      }

   ch->wait = 0;
   act( "You sigh deeply as you attempt to take over $N's mind.", ch, NULL, victim, TO_CHAR );
   act( "$n sighs deeply as $e attempts to take over your mind.", ch, NULL, victim, TO_VICT );
   act( "$n sighs deeply as $e attempts to take over $N's mind.", ch, NULL, victim, TO_NOTVICT );
   act( "As $N averts $S eyes from you, $E suddenly stands motionless.", ch, NULL, victim, TO_CHAR );
   act( "As you avert your eyes from $n, your body freeze with fear as you breathe gently.", ch, NULL, victim, TO_VICT );
   act( "As $N averts $S eyes from you, $E suddenly stands motionless.", ch, NULL, victim, TO_NOTVICT );
   act( "You glare straight into $N's eyes as you disrupt $S concentration.", ch, NULL, victim, TO_CHAR );
   act( "$n glares straight into your eyes as $e disrupts your concentration.", ch, NULL, victim, TO_VICT );
   act( "$n glares straight into $N's eyes as $e disrupts $S concentration.", ch, NULL, victim, TO_NOTVICT );
   act( "You then sink your teeth into $N's neck as you slurp away on $S fresh blood.", ch, NULL, victim, TO_CHAR );
   act( "$n then sinks $s teeth into your neck as $e slurps away on your fresh blood.", ch, NULL, victim, TO_VICT );
   act( "$n then sinks $s teeth into $N's neck as $e slurps away on $S fresh blood.", ch, NULL, victim, TO_NOTVICT );

   if ( IS_NPC( victim ) )
      {
         gain += number_range( 25, 100 );

         if ( ch->pcdata->powers[VAMPIRE_FORM] == VAMPIRE_BAT )
            gain = 25;

         if ( ch->fight_timer == 0 )
            gain *= number_range( 2, 4 );
      }
      else {
              if ( ch->pcdata->powers[VAMPIRE_FORM] == VAMPIRE_BAT )
                 gain = 150;
                 else gain += 100;
           }

   if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_DAGGER )
      gain *= 2;

   ch->pcdata->powers[VAMPIRE_BLOOD] = UMIN( ch->pcdata->powers[VAMPIRE_BLOOD] + gain, ch->pcdata->powers[VAMPIRE_MAXBLOOD] );
   ch->embracing = victim;
   victim->embraced = ch;
   
   if ( number_percent() > 89 && victim->stunned == 0 )
      victim->stunned = 1;
      else victim->wait = 12;
}

void do_belot( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int option = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_CLASS] != 0 )
      {
         stc("you have already chosen a specialization.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp("dagger",arg) )
      option = VAMPIRE_DAGGER;
      else if ( !str_cmp("arcane",arg) )
              {
                 option = VAMPIRE_ARCANE;
                 ch->pcdata->powers[VAMPIRE_BLOOD] = ch->pcdata->powers[VAMPIRE_MAXBLOOD] = 4000;
              }
      else if ( !str_cmp("telekinetic",arg) )
              {
                 option = VAMPIRE_TELE;
                 ch->pcdata->powers[VAMPIRE_BLOOD] = ch->pcdata->powers[VAMPIRE_MAXBLOOD] = 2000;
              }
              else {
                      stc( "#RChoices for #wspecialization #Rare: #eDagger#w, #PArcane#w, #CTelekinetic#w.#n\n\r", ch );
                      return;
                   }

   stc( "#RYou #whave #Rchosen a #wspecialization#R. May your #evictims #ytaste #Reven more beautiful.#n\n\r", ch );
   ch->pcdata->powers[VAMPIRE_CLASS] = option;
}

/* Change Forms */
void do_soul_change( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "#RYou can #ychange #Rbetween #e'human'#R, #e'bat'#R, #e'mist' #Rand #e'wolf' #yforms#R.#n\n\r", ch );
         return;
      }

   if ( !str_cmp(arg,"bat") )
      {
         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               do_change( ch, "human" );
               return;
            }

         if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 30 )
            {
               send_to_char("You have insufficient blood.\n\r",ch);
               return;
            }

         if ( ch->mounted == IS_RIDING )
            do_dismount(ch,"");

         ch->pcdata->powers[VAMPIRE_BLOOD] -= 30;
         act( "#RYou bring your #ycloak #Rtowards your face and disappear in a #ypuff #Rof #esmoke#R.#n", ch, NULL, NULL, TO_CHAR );
         act( "#R$n brings $s #ycloak #Rtowards $s face and disappears in a #ypuff #Rof #esmoke#R.#n", ch, NULL, NULL, TO_ROOM );
         act( "#RAs the #esmoke #Rfades away, a large #ebat #Ris there flapping its wings #yloudly#R.#n", ch, NULL, NULL, TO_ROOM );

         SET_BIT( ch->affected_by, AFF_POLYMORPH );
         ch->pcdata->powers[VAMPIRE_FORM] = VAMPIRE_BAT;

         if ( !IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
            SET_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );

         free_string( ch->morph );
         ch->morph = str_dup( "#ea #Rvampire #ebat#n" );
         return;
      }
      else if ( !str_cmp(arg,"mist") )
      {
         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               do_change( ch, "human" );
               return;
            }

         if ( ch->fight_timer > 0 )
            {
               stc( "The surroundings is too hostile for you to transform into a #wsilvery #emist#n.\n\r", ch );
               return;
            }

         if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
            {
               send_to_char("You have insufficient blood.\n\r",ch);
               return;
            }

         if ( ch->mounted == IS_RIDING )
            do_dismount(ch,"");

         ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;
         act( "#RYou bring your #ycloak #Rtowards your face and disappear in a #wlight silvery #emist#R.#n", ch, NULL, NULL, TO_CHAR );
         act( "#R$n brings $s #ycloak #Rtowards $s face and disappears in a #wlight silvery #emist#R.#n", ch, NULL, NULL, TO_ROOM );

         SET_BIT( ch->affected_by, AFF_POLYMORPH );
         ch->pcdata->powers[VAMPIRE_FORM] = VAMPIRE_MIST;

         if ( !IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
            SET_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );

         if ( !IS_AFFECTED(ch, AFF_PASS_DOOR) )
            SET_BIT( ch->affected_by, AFF_PASS_DOOR );

         free_string( ch->morph );
         ch->morph = str_dup( "#wsome foggy #emist#n" );
         return;
      }
      else if ( !str_cmp(arg,"malice") )
      {
         if ( ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               do_change( ch, "human" );
               return;
            }

         if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 200 )
            {
               send_to_char("You have insufficient blood.\n\r",ch);
               return;
            }

         if ( ch->mounted == IS_RIDING )
            do_dismount(ch,"");

         ch->pcdata->powers[VAMPIRE_BLOOD] -= 200;
         act( "#RYour #yfacial #Rexpressions darken as you bring your #ycloak #Rtowards your face.#n", ch, NULL, NULL, TO_CHAR );
         act( "#R$n's #yfacial #Rexpressions darken as $e brings $s #ycloak #Rtowards $s face.#n", ch, NULL, NULL, TO_ROOM );
         act( "#RAll the #ycolour #Rdrains from your flesh leaving a #eshadowy #wfigure #Rbehind...#n", ch, NULL, NULL, TO_CHAR );
         act( "#RAll the #ycolour #Rdrains from $n's flesh leaving a #eshadowy #wfigure #Rbehind...#n", ch, NULL, NULL, TO_ROOM );

         SET_BIT( ch->affected_by, AFF_POLYMORPH );
         ch->pcdata->powers[VAMPIRE_FORM] = VAMPIRE_MALICE;

         if ( !IS_AFFECTED(ch, AFF_PASS_DOOR) )
            SET_BIT( ch->affected_by, AFF_PASS_DOOR );

         free_string( ch->morph );
         ch->morph = str_dup( "#ethe black monster#n" );
         return;
      }
      else if ( !str_cmp(arg,"wolf") )
      {
         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               do_change( ch, "human" );
               return;
            }

         if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 50 )
            {
               send_to_char("You have insufficient blood.\n\r",ch);
               return;
            }

         if ( ch->mounted == IS_RIDING )
            do_dismount(ch,"");

         ch->pcdata->powers[VAMPIRE_BLOOD] -= 50;
         act( "#RYou #ycrawl #Ron the ground as a mass of #elong, #odense hair #Rgrows all over your body.#n", ch, NULL, NULL, TO_CHAR );
         act( "#R$n #ycrawls #Ron the ground as a mass of #elong, #odense hair #Rgrows all over $s body.#n", ch, NULL, NULL, TO_ROOM );
         act( "#RIn a few seconds, the #eformer #Rvampire now #yhowls #Rat the moon as a menacing #ewolf#R.#n", ch, NULL, NULL, TO_ROOM );

         SET_BIT( ch->affected_by, AFF_POLYMORPH );
         ch->pcdata->powers[VAMPIRE_FORM] = VAMPIRE_WOLF;

         free_string( ch->morph );
         ch->morph = str_dup( "#ea #odire #ywolf#n" );
         return;
      }
      else if ( !str_cmp(arg,"human") || !str_cmp(arg,"none") )
      {
         if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            {
               stc( "But you are already in your human form.\n\r", ch );
               return;
            }

         act( "#RYou #ytransform #Rback into your #Phuman #Rform.#n", ch, NULL, NULL, TO_CHAR );
         act( "#R$n #ytransforms #Rback into $s #Phuman #Rform.#n", ch, NULL, NULL, TO_ROOM );

         ch->pcdata->powers[VAMPIRE_FORM] = 0;

         if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            REMOVE_BIT( ch->affected_by, AFF_POLYMORPH );

         if ( IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
            REMOVE_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );

         if ( IS_SET( ch->more3, MORE3_MIST ) )
            REMOVE_BIT( ch->more3, MORE3_MIST );

         if ( IS_AFFECTED(ch, AFF_PASS_DOOR) )
            REMOVE_BIT( ch->affected_by, AFF_PASS_DOOR );

         free_string( ch->morph );
         ch->morph = str_dup( "" );
         return;
      }
      else do_soul_change( ch, "" );
}

void do_wing_smash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   bool travel = FALSE;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_BAT )
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
	   send_to_char( "Whom do you want to smash yourself into?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_area( ch, arg ) ) == NULL )
            {
                send_to_char( "They aren't here.\n\r", ch );
                return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   if ( ch->fighting != NULL )
      {
         stc( "You need free movement out of the hassle of battle to travel that far.\n\r", ch );
         return;
      }

   if ( victim->in_room != ch->in_room )
      travel = TRUE;

   ch->move -= 250000;
   ch->wait = 6;

   act( "Your features turn bleak as you fly straight into the air.", ch, NULL, NULL, TO_CHAR );
   act( "$n features turn bleak as $e flies straight into the air.", ch, NULL, NULL, TO_ROOM );
   act( "You circle slowly around the vicinity as you search for your target.", ch, NULL, NULL, TO_CHAR );
   act( "You see $n circling slowly above you as in search for something.", ch, NULL, NULL, TO_ROOM );
   act( "Catching $N with a glance of your eyes, you burst into flames as you ZoOm towards $M!", ch, NULL, victim, TO_CHAR );
   act( "Catching you with a glance of $s eyes, $n bursts into flames as $e ZoOmS towards you!", ch, NULL, victim, TO_VICT );

   if ( travel )
      {
         act( "Catching something with a glance of $s eyes, $n ZoOm away!", ch, NULL, NULL, TO_ROOM );

         char_from_room( ch );
         char_to_room( ch, victim->in_room );
      }
      else act( "Catching $N with a glance of $s eyes, $n bursts into flames as $e ZoOmS towards $M!", ch, NULL, victim, TO_NOTVICT );

   act( "You SlAm yourself HaRd into $N sending $M SpRaWlInG!", ch, NULL, victim, TO_CHAR );
   act( "$n SlAmS $mself HaRd into you sending you SpRaWlInG!", ch, NULL, victim, TO_VICT );
   act( "$n SlAmS $mself HaRd into $N sending $M SpRaWlInG!", ch, NULL, victim, TO_NOTVICT );
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 400000, TRUE );
}

void do_sonic_pierce( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_BAT )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more, MORE_HAMMER ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->move < 300000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   ch->move -= 300000;
   ch->wait = 8;
   SET_BIT( ch->more, MORE_HAMMER );

   act( "You fly above the room flapping your wings noisly.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies above the room flapping $s wings noisly.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly let out a HiGh-PiTcHeD screech causing everyone to cover their ears in pain.", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly lets out a HiGh-PiTcHeD screech causing you to cover your ears in pain.", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      if ( number_percent() > 70 && !IS_SET( vch->more, MORE_CONFUSION ) )
         SET_BIT( vch->more, MORE_CONFUSION );

      act( "The sonic waves RiP your body apart as they cause you to tremble backwards!", vch, NULL, NULL, TO_CHAR );
      act( "The sonic waves RiP $n's body apart as they cause $m to tremble backwards!", vch, NULL, NULL, TO_ROOM );
      ch->attack_type = ATTACK_MAGICAL;
      hurt_person( ch, vch, 750000, TRUE );
   }
}

void do_bat_fire( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( ch->mana < 200000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to blast with flames?\n\r", ch );
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

   ch->mana -= 200000;
   ch->wait = 12;

   act( "You open your mouth as it glows with fierce intensity.", ch, NULL, NULL, TO_CHAR );
   act( "$n opens $s mouth as it glows with fierce intensity.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, you shoot a blast of flames right towards $N!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n shoots a blast of flames right towards you!", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n shoots a blast of flames right towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream in agony as you are engulfed in the mighty blast.", victim, NULL, NULL, TO_CHAR );
   act( "$n screams in agony as $e is engulfed in the mighty blast.", victim, NULL, NULL, TO_ROOM );

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_FIRE;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_bat_blast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int bonus = 0;

   act( "You hold your breath painfully as your body starts growing enormously...", ch, NULL, NULL, TO_CHAR );
   act( "$n holds $s breath painfully as $s body starts growing enormously...", ch, NULL, NULL, TO_ROOM );
   act( "Now as big as the entire vicinity, your body starts trembling from the added weight.", ch, NULL, NULL, TO_CHAR );
   act( "Now as big as the entire vicinity, $n's body starts trembling from the added weight.", ch, NULL, NULL, TO_ROOM );
   act( "Your eyes flashes gold as you ExPlOdE violently into hundrends of miniature bats!", ch, NULL, NULL, TO_CHAR );
   act( "$n's eyes flashes gold as $e ExPlOdEs violently into hundrends of miniature bats!", ch, NULL, NULL, TO_ROOM );

   ch->wait = 12;
   subtract_stock( ch, 100 );

   REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
   REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
   REMOVE_BIT(ch->polyaff, POLY_BAT);
   REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
   REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
   free_string( ch->morph );
   ch->morph = str_dup( "" );

   act( "The bats wreck total havoc all throughout the room as you shift back into your human form.", ch, NULL, NULL, TO_CHAR );
   act( "The bats wreck total havoc all throughout the room as $e shifts back into $s human form.", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      ch->pcdata->condition[COND_THIRST] = UMIN( 10000, ch->pcdata->condition[COND_THIRST] + 500 );
      bonus += 100000;

      act( "You shriek loudly as a bunch of bats clenches themselves onto you sucking your blood!", vch, NULL, NULL, TO_CHAR );
      act( "$n shrieks loudly as a bunch of bats clenches themselves onto $m sucking $s blood!", vch, NULL, NULL, TO_ROOM );
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = HIT;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 750000, TRUE );
   }

   bonus = UMIN( bonus, 500000 );

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      bonus *= 2;

   ch->hit = UMIN( ch->max_hit, ch->hit + bonus );
   act( "You cackle in glee at your handiwork as you draw your cape towards your face.", ch, NULL, NULL, TO_CHAR );
   act( "$n cackles in glee at $s handiwork as $e draws $s cape towards $s face.", ch, NULL, NULL, TO_ROOM );
}

void do_wolf_charge( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_WOLF )
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
	   send_to_char( "Whom do you want to charge into?\n\r", ch );
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

   ch->move -= 50000;
   ch->wait = 12;

   act( "Your body flashes as you dash headfirst straight into $N!", ch, NULL, victim, TO_CHAR );
   act( "$n's body flashes as $e dashes headfirst straight into you!", ch, NULL, victim, TO_VICT );
   act( "$n's body flashes as $e dashes headfirst straight into $N!", ch, NULL, victim, TO_NOTVICT );
   act( "At the very last second, you fling your heavy weight into $N!", ch, NULL, victim, TO_CHAR );
   act( "At the very last second, $n flings $s heavy weight into you!", ch, NULL, victim, TO_VICT );
   act( "At the very last second, $n flings $s heavy weight into $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You clutch your stomach in pain as the wind is taken out of you.", victim, NULL, NULL, TO_CHAR );
   act( "$n clutches $s stomach in pain as the wind is taken out of $m.", victim, NULL, NULL, TO_ROOM );

   victim->move = UMAX( 0, victim->move - 250000 );
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_lunge_bite( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_WOLF )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 400000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to bite viciously?\n\r", ch );
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

   ch->move -= 400000;
   ch->wait = 18;

   act( "You pounce onto $N as you snap your jaws into $S arms!", ch, NULL, victim, TO_CHAR );
   act( "$n pounces onto you as $e snaps $s jaws into your arms!", ch, NULL, victim, TO_VICT );
   act( "$n pounces onto $N as $e snaps $s jaws into $S arms!", ch, NULL, victim, TO_NOTVICT );
   act( "You savagely try to rip $N's arm off $S body as $E fights back against you!", ch, NULL, victim, TO_CHAR );
   act( "$n savagely tries to rip your arm off your body as you fight back against $m!", ch, NULL, victim, TO_VICT );
   act( "$n savagely tries to rip $N's arm off $S body as $E fights back against $m!", ch, NULL, victim, TO_NOTVICT );
   act( "As you leap away from $N, you sneer as $E bleeds badly onto the ground!", ch, NULL, victim, TO_CHAR );
   act( "As $n leaps away from you, $e sneers as you bleed badly onto the ground!", ch, NULL, victim, TO_VICT );
   act( "As $n leaps away from $N, $e sneers as $E bleeds badly onto the ground!", ch, NULL, victim, TO_NOTVICT );

   ch->in_room->blood = UMIN( 1000, ch->in_room->blood + 200 );
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_wolf_blast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int limb = number_range( 1, 4 );

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 556 ) 
      return;

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast into?\n\r", ch );
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

   act( "You leap high into the air as your body bursts into a deadly light.", ch, NULL, NULL, TO_CHAR );
   act( "$n leaps high into the air as $s body bursts into a deadly light.", ch, NULL, NULL, TO_ROOM );
   act( "You swerve around $N as you suddenly rush towards $M!", ch, NULL, victim, TO_CHAR );
   act( "The dark light swerves around you as it suddenly rushes towards you!", ch, NULL, victim, TO_VICT );
   act( "The dark light swerves around $N as it suddenly rushes towards $M!", ch, NULL, victim, TO_NOTVICT );

   if ( !IS_NPC(ch) )
      {
         ch->wait = 12;
         subtract_stock( ch, 100 );
         REMOVE_BIT(ch->polyaff, POLY_WOLF);
         REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
         free_string( ch->morph );
         ch->morph = str_dup( "" );
      }

   act( "As you rip through $M, you change back into your human form as the light keeps on merging.", ch, NULL, victim, TO_CHAR );
   act( "As the light rips through you, it changes back into $n as more of it keeps on merging.", ch, NULL, victim, TO_VICT );
   act( "As the light rips through $M, it changes back into $n as more of it keeps on merging.", ch, NULL, victim, TO_NOTVICT );
   act( "You sneer as the light blasts out of $N's body then pours itself into $S mouth.", ch, NULL, victim, TO_CHAR );
   act( "$n sneers as the light blasts out of your body then pours itself into your mouth.", ch, NULL, victim, TO_VICT );
   act( "$n sneers as the light blasts out of $N's body then pours itself into $S mouth.", ch, NULL, victim, TO_NOTVICT );

   if ( !IS_ARM_R(victim,LOST_ARM) && limb == 1 && !IS_CLASS( victim, CLASS_ASSASSIN ) && !IS_CLASS( victim, CLASS_PALADIN ) )
      {
         act( "You scream in agony as your arm is suddenly RiPpEd apart from your body.", victim, NULL, NULL, TO_CHAR );
         act( "$n screams in agony as $s arm is suddenly RiPpEd apart from $s body.", victim, NULL, NULL, TO_ROOM );
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
      else if ( !IS_ARM_L(victim,LOST_ARM) && limb == 2 && !IS_CLASS( victim, CLASS_ASSASSIN ) && !IS_CLASS( victim, CLASS_PALADIN ) )
              {
                 act( "You scream in agony as your arm is suddenly RiPpEd apart from your body.", victim, NULL, NULL, TO_CHAR );
                 act( "$n screams in agony as $s arm is suddenly RiPpEd apart from $s body.", victim, NULL, NULL, TO_ROOM );
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
      else if ( !IS_LEG_L(victim,LOST_LEG) && limb == 3 && !IS_CLASS( victim, CLASS_ASSASSIN ) && !IS_CLASS( victim, CLASS_PALADIN ) )
              {
                 act( "You scream in agony as your leg is suddenly RiPpEd apart from your body.", victim, NULL, NULL, TO_CHAR );
                 act( "$n screams in agony as $s leg is suddenly RiPpEd apart from $s body.", victim, NULL, NULL, TO_ROOM );
                 SET_BIT(victim->loc_hp[4],LOST_LEG);
                 SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
                 make_part(victim,"leg");

                 if ( IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM) )
                    {
                       if ( ( obj = get_eq_char( victim, WEAR_LEGS ) ) != NULL )
                          take_item(victim,obj);
                    }

                 if ( ( obj = get_eq_char( victim, WEAR_FEET ) ) != NULL )
                    take_item(victim,obj);
              }
      else if ( !IS_LEG_R(victim,LOST_LEG) && limb == 4 && !IS_CLASS( victim, CLASS_ASSASSIN ) && !IS_CLASS( victim, CLASS_PALADIN ) )
              {
                 act( "You scream in agony as your leg is suddenly RiPpEd apart from your body.", victim, NULL, NULL, TO_CHAR );
                 act( "$n screams in agony as $s leg is suddenly RiPpEd apart from $s body.", victim, NULL, NULL, TO_ROOM );
                 SET_BIT(victim->loc_hp[5],LOST_LEG);
                 SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
                 make_part(victim,"leg");

                 if ( IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG) )
                    {
                       if ( ( obj = get_eq_char( victim, WEAR_LEGS ) ) != NULL )
                          take_item(victim,obj);
                    }

                 if ( ( obj = get_eq_char( victim, WEAR_FEET ) ) != NULL )
                    take_item(victim,obj);
              }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BEAM;
   ch->attack_focus = DIRECT;

   if ( !IS_NPC(ch) )
      hurt_person( ch, victim, 1000000, TRUE );
      else hurt_person( ch, victim, 1500000, TRUE );
}

void do_reaver_blast( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   SET_BIT( ch->arena, AFF2_DM );

   if ( IS_POLYAFF(ch, POLY_BAT) )
      do_bat_blast( ch, argument );
      else if ( IS_POLYAFF(ch, POLY_WOLF) )
              do_wolf_blast( ch, argument );
              else {
                      stc("You need to be in bat or wolf form.\n\r",ch);
                      return;
                   }
}

void do_spear_kick( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->move < 50000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to kick in the stomach?\n\r", ch );
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

   ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;
   ch->move -= 50000;
   ch->wait = 18;

   act( "You crouch forward as you bring your daggers up towards your face.", ch, NULL, NULL, TO_CHAR );
   act( "$n crouches forward as $e brings $s daggers up towards $s face.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly leap into the air giving $N a savage kick into $S midsection.", ch, NULL, victim, TO_CHAR );
   act( "$n suddenly leaps into the air giving you a savage kick into your midsection.", ch, NULL, victim, TO_VICT );
   act( "$n suddenly leaps into the air giving $N a savage kick into $S midsection.", ch, NULL, victim, TO_NOTVICT );

   if ( !is_martial( victim ) && victim->leader != NULL )
      {
         act( "The force of the blow sends you flying into your group's arms, causing them to fall.", victim, NULL, NULL, TO_CHAR );
         act( "The force of the blow sends $n flying into $s group's arms, causing them to fall.", victim, NULL, NULL, TO_ROOM );
         victim->wait = UMIN( 12, victim->wait + 8 );
      }
      else {
              act( "The force of the blow sends you sprawling across the room.", victim, NULL, NULL, TO_CHAR );
              act( "The force of the blow sends $n sprawling across the room.", victim, NULL, NULL, TO_ROOM );
           }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_dagger_pounce( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more, MORE_NINJA ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 75 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->move < 250000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to strike from the back?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_area( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   SET_BIT( ch->more, MORE_NINJA );
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 75;
   ch->move -= 250000;
   ch->wait = 4;

   if ( victim->in_room->vnum != ch->in_room->vnum )
      {
         stc("You dash out of the room as you leap through bushes and roll under structures.\n\r",ch);
         char_from_room( ch );
         char_to_room( ch, victim->in_room );
         do_look( ch, "" );
      }

   act( "Grinning evilly, you jump out of the dark leaping upon $N's shoulders.", ch, NULL, victim, TO_CHAR );
   act( "Grinning evilly, $n jumps out of the dark leaping upon your shoulders.", ch, NULL, victim, TO_VICT );
   act( "Grinning evilly, $n jumps out of the dark leaping upon $N's shoulders.", ch, NULL, victim, TO_NOTVICT );
   act( "Before $N can recover, you deftly plunge your dagger viciously into $S back!", ch, NULL, victim, TO_CHAR );
   act( "Before you can recover, $n deftly plunges $s dagger viciously into your back!", ch, NULL, victim, TO_VICT );
   act( "Before $N can recover, $n deftly plunges $s dagger viciously into $S back!", ch, NULL, victim, TO_NOTVICT );
   act( "You then use your daggers as a handle as you flip off $N's shoulders.", ch, NULL, victim, TO_CHAR );
   act( "$n then uses $s daggers as a handle as $e flips off your shoulders.", ch, NULL, victim, TO_VICT );
   act( "$n then uses $s daggers as a handle as $e flips off $N's shoulders.", ch, NULL, victim, TO_NOTVICT );
   act( "You savagely kick $N's body away from your daggers as you jump in for the attack!", ch, NULL, victim, TO_CHAR );
   act( "$n savagely kicks your body away from $s daggers as $e jumps in for the attack!", ch, NULL, victim, TO_VICT );
   act( "$n savagely kicks $N's body away from $s daggers as $e jumps in for the attack!", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 650000, TRUE );
   multi_hit( ch, victim, TYPE_UNDEFINED );
   multi_hit( ch, victim, TYPE_UNDEFINED );
}

void do_sense_blood( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   bool found;

   one_argument( argument, arg );

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_WOLF )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 25 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   ch->pcdata->powers[VAMPIRE_BLOOD] -= 25;
   act("You place your hand on the earth as you sense for trails of dried up blood.",ch,NULL,NULL,TO_CHAR);
   act("$n places $s hand on the earth as $e softly concentrates on $s surroundings.",ch,NULL,NULL,TO_ROOM);

   sprintf( buf, "#rHeart#ebeats #cnear you (#w%s#e)#n:\n\r", ch->in_room->area->name );
   send_to_char( buf, ch );
   found = FALSE;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
           && ( victim = d->character ) != NULL
           && !IS_NPC(victim)
           && !IS_IMMORTAL(victim)
           && victim != ch
           && victim->in_room != NULL
           && victim->in_room->area == ch->in_room->area )
         {
            found = TRUE;
            sprintf( buf, "%-28s %s\n\r", victim->name, victim->in_room->name );
            send_to_char( buf, ch );
         }
   }

   if ( !found )
      send_to_char( "You are the only being throughout the surrounding vicinity.\n\r", ch );
}

void do_mirage( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SOULREAVER ) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->act, PLR_WIZINVIS ) )
      {
         send_to_char("You fade into existence as you step from within the shadows.\n\r",ch);
         act("$n fades into existence as $e steps from within the shadows.",ch, NULL, NULL, TO_ROOM);
         REMOVE_BIT( ch->act, PLR_WIZINVIS );
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc("You are way too distracted to fade within the shadows at this time.\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_WOLF )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   ch->wait = 4;
   send_to_char("You step back into the shadows as you calmly wait for your kill.\n\r",ch);
   act("$n steps back into the shadows as $e fades away from sight.",ch, NULL, NULL, TO_ROOM);
   SET_BIT( ch->act, PLR_WIZINVIS );
}

void do_double_twist( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   bool disarm = FALSE;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->move < 200000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
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

   if ( !IS_CLASS( victim, CLASS_PALADIN ) && !IS_CLASS( victim, CLASS_DARK_KNIGHT ) &&
        !IS_CLASS( victim, CLASS_SORCEROR ) && !IS_CLASS( victim, CLASS_DIVINE_KNIGHT ) )
      disarm = TRUE;

   if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON )
      && ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL || obj->item_type != ITEM_WEAPON )
      && ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL || obj->item_type != ITEM_WEAPON )
      && ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL || obj->item_type != ITEM_WEAPON ) )
      obj = NULL;

   if ( number_percent() < 70 )
      disarm = FALSE;

   ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;
   ch->move -= 200000;
   ch->wait = 18;

   act( "You twirl your body towards $N slashing $M once with your dagger.", ch, NULL, victim, TO_CHAR );
   act( "$n twirls $s body towards you slashing you once with $s dagger.", ch, NULL, victim, TO_VICT );
   act( "$n twirls $s body towards $N slashing $M once with $s dagger.", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;

   if ( disarm && obj != NULL )
      {
         act( "You rapidly knock away $N's weapon as your body spins through the air.", ch, NULL, victim, TO_CHAR );
         act( "$n rapidly knocks away your weapon as $s body spins through the air.", ch, NULL, victim, TO_VICT );
         act( "$n rapidly knocks away $N's weapon as $s body spins through the air.", ch, NULL, victim, TO_NOTVICT );
         obj_from_char( obj );
         obj_to_char( obj, victim );
         hurt_person( ch, victim, 650000, TRUE );
      }
      else {
              act( "You rapidly slice into $N's abdomen as your body spins through the air.", ch, NULL, victim, TO_CHAR );
              act( "$n rapidly slices into your abdomen as $s body spins through the air.", ch, NULL, victim, TO_VICT );
              act( "$n rapidly slices into $N's abdomen as $s body spins through the air.", ch, NULL, victim, TO_NOTVICT );
              hurt_person( ch, victim, 400000, TRUE );
           }
}

void do_neck_slash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int gain = 20;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->move < 5 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash?\n\r", ch );
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

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      gain *= number_range( 2, 4 );

   ch->pcdata->powers[VAMPIRE_BLOOD] = UMIN( ch->pcdata->powers[VAMPIRE_BLOOD] + gain, ch->pcdata->powers[VAMPIRE_MAXBLOOD] );
   ch->move -= 5;
   ch->wait = 12;

   act( "You move yourself side by side dodging $N's attacks as you wait for the right moment.", ch, NULL, victim, TO_CHAR );
   act( "$n moves $mself side by side dodging your attacks as $e waits for the right moment.", ch, NULL, victim, TO_VICT );
   act( "$n moves $mself side by side dodging $N's attacks as $e waits for the right moment.", ch, NULL, victim, TO_NOTVICT );
   act( "At the last second, you leap by $N and slice into $S neck!", ch, NULL, victim, TO_CHAR );
   act( "At the last second, $n leaps by you and slice into your neck!", ch, NULL, victim, TO_VICT );
   act( "At the last second, $n leaps by $N and slice into $S neck!", ch, NULL, victim, TO_NOTVICT );
   act( "As your mouth passes the wound, you shudder in esctasy from the taste of fresh blood.", ch, NULL, victim, TO_CHAR );
   act( "As $n's mouth passes the wound, $e shudders in esctasy from the taste of fresh blood.", ch, NULL, victim, TO_VICT );
   act( "As $n's mouth passes the wound, $e shudders in esctasy from the taste of fresh blood.", ch, NULL, victim, TO_NOTVICT );

   ch->in_room->blood = UMIN( 1000, ch->in_room->blood + 50 );
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_soul_steal( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more, MORE_BOMB ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 250 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   ch->wait = 12;
   ch->mana -= 500000;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 250;
   SET_BIT( ch->more, MORE_BOMB );

   act( "Your eyes taint with evil as you watch your hands burst into flames.", ch, NULL, NULL, TO_CHAR );
   act( "$n's eyes taint with evil as $e watches $s hands burst into flames.", ch, NULL, NULL, TO_ROOM );
   act( "You raise your arms and say in a stern demonic voice '#ySoul Steal!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n raises $s arms and says in a stern demonic voice '#ySoul Steal!#n'", ch, NULL, NULL, TO_ROOM );
   act( "Energy from all over the room gathers into one spot and flies into you.", ch, NULL, NULL, TO_CHAR );
   act( "Energy from all over the room gathers into one spot and flies into $n.", ch, NULL, NULL, TO_ROOM );
   act( "Your body trembles with glee as the energy around you becomes more tense.", ch, NULL, NULL, TO_CHAR );
   act( "$n's body trembles with glee as the energy around $m becomes more tense.", ch, NULL, NULL, TO_ROOM );
   ch->hit = UMIN( ch->max_hit, ch->hit + 1500000 );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "You moan in pain as your lifeforce rips from your body going into $N.", vch, NULL, ch, TO_CHAR );
      act( "$n moans in pain as $s lifeforce rips from $s body going into you.", vch, NULL, ch, TO_VICT );
      act( "$n moans in pain as $s lifeforce rips from $s body going into $N.", vch, NULL, ch, TO_NOTVICT );

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      hurt_person( ch, vch, 1000000, TRUE );
   }
}

void do_blood_cleanse( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SOULREAVER ) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_WOLF )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 250 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 500000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   ch->wait = 6;
   ch->mana -= 500000;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 250;
   ch->hit = UMIN( ch->max_hit, ch->hit + 1000 );
   
   act("You close your eyes as you relax your body.",ch, NULL, NULL, TO_CHAR);
   act("$n closes $s eyes as $e relaxes $s body.",ch, NULL, NULL, TO_ROOM);
   act("Large gashes and wounds seal themselves up on your peaceful body.",ch, NULL, NULL, TO_CHAR);
   act("Large gashes and wounds seal themselves up on $n's peaceful body.",ch, NULL, NULL, TO_ROOM);
   clear_affects( ch );
}

void do_blood_ritual( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_WOLF )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 500 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to cleanse?\n\r", ch );
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
   
   ch->wait = 6;
   ch->mana -= 1000000;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 500;

   act( "You glide towards $N taking $S finger towards your lips.", ch, NULL, victim, TO_CHAR );
   act( "$n glides towards you taking your finger towards $s lips.", ch, NULL, victim, TO_VICT );
   act( "$n glides towards $N taking $S finger towards $s lips.", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly you bite down on $N's finger.. spitting out blood in intervals.", ch, NULL, victim, TO_CHAR );
   act( "Suddenly $n bites down on your finger.. spitting out blood in intervals.", ch, NULL, victim, TO_VICT );
   act( "Suddenly $n bites down on $N's finger.. spitting out blood in intervals.", ch, NULL, victim, TO_NOTVICT );
   act( "Afterwards, you glide your hand across the wound making it disappear.", ch, NULL, NULL, TO_CHAR );
   act( "Afterwards, $n glides $s hand across the wound making it disappear.", ch, NULL, NULL, TO_ROOM );
   act( "You feel alot more relieved as fresh blood flows throughout your body.", victim, NULL, NULL, TO_CHAR );
   act( "$n feels alot more relieved as fresh blood flows throughout $s body.", victim, NULL, NULL, TO_ROOM );
   clear_affects( victim );
}

void do_soul_assassinate( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int chance = 95;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_DAGGER )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc("You are too excited to have the cold precision required for this.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->act, PLR_WIZINVIS ) )
      {
         stc("You need to be within the shadows to move in for the kill.\n\r",ch);
         return;
      }

   if ( ch->move < 500000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to assassinate?\n\r", ch );
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

   ch->move -= 500000;
   ch->wait = 12;
   REMOVE_BIT( ch->act, PLR_WIZINVIS );

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      chance = 91;

   if ( number_percent() > chance )
      {
         act("You fade from the shadows as you softly place your dagger into $N's back.",ch,NULL,victim,TO_CHAR);
         act("$n fades from the shadows as $e softly places $s dagger into $N's back.",ch,NULL,victim,TO_NOTVICT);
         act("You feel a #rt#yi#bn#gg#rl#Ci#pn#wg#n sensation soars through your body.",ch,NULL,victim,TO_VICT);
         stop_fighting( victim, TRUE );
         victim->hit = -2000010;
         victim->position = POS_MORTAL;
         ch->fight_timer = 20;
         return;
      }

   act("You leap from the shadows as you merge behind $N's back.",ch,NULL,victim,TO_CHAR);
   act("$n leaps from the shadows as $e merges behind your back.",ch,NULL,victim,TO_VICT);
   act("$n leaps from the shadows as $e merges behind $N's back.",ch,NULL,victim,TO_NOTVICT);
   act("As you slash across $N's neck, $E barely moves to the side glancing the blow.",ch,NULL,victim,TO_CHAR);
   act("As $n slashes across your neck, you barely move to the side glancing the blow.",ch,NULL,victim,TO_VICT);
   act("As $n slashes across $N's neck, $E barely moves to the side glancing the blow.",ch,NULL,victim,TO_NOTVICT);
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_soul_fire( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_ARCANE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 150000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to blast with fireballs?\n\r", ch );
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

   ch->mana -= 150000;
   ch->wait = 12;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;

   act("You concentrate furiously as you disappear in a silvery mist.",ch,NULL,NULL,TO_CHAR);
   act("$n concentrates furiously as $e disappears in a silvery mist.",ch,NULL,NULL,TO_ROOM);
   act("Reappearing a few feet away, you suddenly move your arm to one side.",ch,NULL,NULL,TO_CHAR);
   act("Reappearing a few feet away, $n suddenlys move $s arm to one side.",ch,NULL,NULL,TO_ROOM);
   act("You mutter '#yHell Fire...#n' as your cape starts pulsing with demonic energy.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yHell Fire...#n' as $s cape starts pulsing with demonic energy.",ch,NULL,NULL,TO_ROOM);
   act("Three MaSsIvE fireballs shoots from the cape as they blast straight into you.",victim,NULL,NULL,TO_CHAR);
   act("Three MaSsIvE fireballs shoots from the cape as they blast straight into $n.",victim,NULL,NULL,TO_ROOM);
   act("You then wrap the cape back around your body as the flames engulf $N!",ch,NULL,victim,TO_CHAR);
   act("$n then wraps the cape back around $s body as the flames engulf you!",ch,NULL,victim,TO_VICT);
   act("$n then wraps the cape back around $s body as the flames engulf $N!",ch,NULL,victim,TO_NOTVICT);

   ch->hit = UMIN( ch->hit + 150000, ch->max_hit );
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_FIRE;
   hurt_person( ch, victim, 600000, TRUE );
}

void do_dark_fire( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_ARCANE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast with fireballs?\n\r", ch );
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

   ch->mana -= 50000;
   ch->wait = 12;

   act("You concentrate furiously as you disappear in a silvery mist.",ch,NULL,NULL,TO_CHAR);
   act("$n concentrates furiously as $e disappears in a silvery mist.",ch,NULL,NULL,TO_ROOM);
   act("Reappearing a few feet away, you suddenly move your arm to one side.",ch,NULL,NULL,TO_CHAR);
   act("Reappearing a few feet away, $n suddenlys move $s arm to one side.",ch,NULL,NULL,TO_ROOM);
   act("You mutter '#yDark Fire...#n' as your cape starts pulsing with demonic energy.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yDark Fire...#n' as $s cape starts pulsing with demonic energy.",ch,NULL,NULL,TO_ROOM);
   act("A MaSsIvE fireball shoots from the cape as it flies straight into you.",victim,NULL,NULL,TO_CHAR);
   act("A MaSsIvE fireball shoots from the cape as it flies straight into $n.",victim,NULL,NULL,TO_ROOM);
   act("You then wrap the cape back around your body as the flames blast $N away!",ch,NULL,victim,TO_CHAR);
   act("$n then wraps the cape back around $s body as the flames blast you away!",ch,NULL,victim,TO_VICT);
   act("$n then wraps the cape back around $s body as the flames blast $N away!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 50 )
      ch->hit = UMIN( ch->hit + 50000, ch->max_hit );

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_FIRE;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_tetra_spirit( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_ARCANE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to send a spirit after?\n\r", ch );
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

   ch->mana -= 100000;
   ch->wait = 12;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;

   act("Your eyes change colours as four misty spirits fly out from beneath the ground.",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes change colours as four misty spirits fly out from beneath the ground.",ch,NULL,NULL,TO_ROOM);
   act("With a single hand motion, you send them circling around $N slowly.",ch,NULL,victim,TO_CHAR);
   act("With a single hand motion, $n sends them circling around you slowly.",ch,NULL,victim,TO_VICT);
   act("With a single hand motion, $n sends them circling around $N slowly.",ch,NULL,victim,TO_NOTVICT);
   act("As you focus onto the spirits, they start floating aimlessly into the air.",ch,NULL,NULL,TO_CHAR);
   act("As $n focuses onto the spirits, they start floating aimlessly into the air.",ch,NULL,NULL,TO_ROOM);
   act("You say sternly '#yTetra Spirits!#n' as you point a glowing finger at $N!",ch,NULL,victim,TO_CHAR);
   act("$n says sternly '#yTetra Spirits!#n' as $e points a glowing finger at you!",ch,NULL,victim,TO_VICT);
   act("$n says sternly '#yTetra Spirits!#n' as $e points a glowing finger at $N!",ch,NULL,victim,TO_NOTVICT);
   act("A wispy spirit blast through your body viciously as it knocks you down!.",victim,NULL,NULL,TO_CHAR);
   act("A wispy spirit blast through $n's body viciously as it knocks $m down!.",victim,NULL,NULL,TO_ROOM);
   act("A wispy spirit blast through your body viciously as it knocks you down!.",victim,NULL,NULL,TO_CHAR);
   act("A wispy spirit blast through $n's body viciously as it knocks $m down!.",victim,NULL,NULL,TO_ROOM);
   act("A wispy spirit blast through your body viciously as it knocks you down!.",victim,NULL,NULL,TO_CHAR);
   act("A wispy spirit blast through $n's body viciously as it knocks $m down!.",victim,NULL,NULL,TO_ROOM);
   act("A wispy spirit blast through your body viciously as it knocks you down!.",victim,NULL,NULL,TO_CHAR);
   act("A wispy spirit blast through $n's body viciously as it knocks $m down!.",victim,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->hit = UMIN( ch->hit + 100000, ch->max_hit );
   hurt_person( ch, victim, 600000, TRUE );
}

void do_blood_wave( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_VAMPIRE) || ch->clannum != 1 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->ctimer[2] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   ch->wait = 12;
   ch->mana -= 100000;
   ch->ctimer[2] = 4;

   act( "You softly chant '#yLudricri Jatusm!#n' as the blood around you starts trembling.", ch, NULL, NULL, TO_CHAR );
   act( "$n softly chants '#yLudricri Jatusm!#n' as the blood around $m starts trembling.", ch, NULL, NULL, TO_ROOM );
   act( "As your chanting continues, the pool of blood turns into a gigantic tidal wave.", ch, NULL, NULL, TO_CHAR );
   act( "As $n's chanting continues, the pool of blood turns into a gigantic tidal wave.", ch, NULL, NULL, TO_ROOM );
   act( "You then bring your arms into the air, clapping them mightly with sheer force.", ch, NULL, NULL, TO_CHAR );
   act( "$n then brings $s arms into the air, clapping them mightly with sheer force.", ch, NULL, NULL, TO_ROOM );
   act( "The wave towers above everything as it RoaRs across the room!", ch, NULL, NULL, TO_CHAR );
   act( "The wave towers above everything as it RoaRs across the room!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "You grasp the air wildly as the blood waves sweep you away.", vch, NULL, NULL, TO_CHAR );
      act( "$n grasps the air wildly as the blood waves sweep $m away.", vch, NULL, NULL, TO_ROOM );

      if ( vch->fighting != NULL )
         stop_fighting( vch, TRUE );

      if ( special_slam( vch, number_range( 0, 3 ) ) == TRUE )
         hurt_person( ch, vch, 75000, TRUE );
         else hurt_person( ch, vch, 50000, TRUE );
   }
}

void do_soul_fear( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int mygroup = 0, theirgroup = 0;
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->ctimer[3] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to bring fear to?\n\r", ch );
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

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_NPC( ch ) && ( vch == ch || is_same_group( ch, vch ) ) )
         mygroup++;

      if ( !IS_NPC( vch ) && ( vch == victim || is_same_group( ch, victim ) ) )
         theirgroup++;
   }

   if ( mygroup > theirgroup )
      {
         stc("Your numbers are much greater than theirs vampire. Don't be a coward.\n\r", ch);
         return;
      }

   ch->wait = 12;
   ch->mana -= 1000000;
   ch->ctimer[3] = 6;

   act( "You scream '#yDASHIER NI!#n' as you attempt to make $N flee in fear.", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yDASHIER NI!#n' as $e attempts to make you flee in fear.", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yDASHIER NI!#n' as $e attempts to make $N flee in fear.", ch, NULL, victim, TO_NOTVICT );
   act( "Your body is filled with the harshest of fear as you tremble on the ground.", victim, NULL, NULL, TO_CHAR );
   act( "$n's body is filled with the harshest of fear as $e trembles on the ground.", victim, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch != victim && !is_same_group(victim,vch) )
         continue;

      if ( vch->fight_timer == 0 )
         continue;

      do_flee( vch, "" );

      if ( !is_martial( vch ) )
         vch->wait = 36;
   }
}

void do_poison_mist( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SOULREAVER ) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_ARCANE )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MIST )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   ch->wait = 12;

   if ( IS_SET( ch->more3, MORE3_MIST ) )
      {
         send_to_char("Your poisonous fumes vaporize into the air as you turn back misty.\n\r",ch);
         act("$n's poisonous fumes vaporize into the air as $e turns back misty.",ch, NULL, NULL, TO_ROOM);
         REMOVE_BIT( ch->more3, MORE3_MIST );
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;

   send_to_char("You slowly turn more yellowish as you are filled with poisonous fumes.\n\r",ch);
   act("$n slowly turns more yellowish as $e is filled with poisonous fumes.",ch, NULL, NULL, TO_ROOM);
   SET_BIT( ch->more3, MORE3_MIST );
}

void do_psy_reaver( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int option = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MALICE )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp("fire",arg) )
      option = REAVER_FIRE;
      else if ( !str_cmp("dark",arg) )
              option = REAVER_DARK;
      else if ( !str_cmp("light",arg) )
              option = REAVER_LIGHT;
      else if ( !str_cmp("chill",arg) )
              option = REAVER_CHILL;
      else if ( !str_cmp("stone",arg) )
              option = REAVER_STONE;
      else if ( !str_cmp("shock",arg) )
              option = REAVER_SHOCK;
      else if ( !str_cmp("soul",arg) )
              option = REAVER_SOUL;
      else if ( !str_cmp("faith",arg) )
              option = REAVER_FAITH;
      else if ( !str_cmp("none",arg) )
              option = 0;
              else {
                      stc( "#RSyntax: #wPsyReaver #RFire#w/#eDark#w/#yLight#w/#LChill#w/#oStone#w/#yShock#w/#PSoul#w/#CFaith#w/#gNone#w.#n\n\r", ch );
                      return;
                   }

   if ( IS_SET( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON ) )
      REMOVE_BIT( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON );

   ch->wait = 12;

   if ( option != 0 )
      {
         act( "You look down at your arm in deep concentration as it starts trembling with power.", ch, NULL, NULL, TO_CHAR );
         act( "$n looks down at $s arm in deep concentration as it starts trembling with power.", ch, NULL, NULL, TO_ROOM );
         act( "Your arm suddenly starts pulsing with a pinkish energy as it forms into palish beam.", ch, NULL, NULL, TO_CHAR );
         act( "$n's arm suddenly starts pulsing with a pinkish energy as it forms into palish beam.", ch, NULL, NULL, TO_ROOM );
      }
      else {
              act( "You relax your muscles as the palish energy around your arm vanishes.", ch, NULL, NULL, TO_CHAR );
              act( "$n relaxes $s muscles as the palish energy around $s arm vanishes.", ch, NULL, NULL, TO_ROOM );
           }

   ch->pcdata->powers[VAMPIRE_REAVER] = option;
}

void do_psy_slice( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int chance = 96;
   int dam = 500000;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == 0 )
      {
         stc("You need a reaver weapon to use this deadly technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MALICE )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 50 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->move < 15 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to kill?\n\r", ch );
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

   ch->move -= 15;
   ch->wait = 12;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 50;

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      chance = 92;

   if ( ch->fight_timer == 0 && ch->position == POS_STANDING && number_percent() > chance )
      {
         act("You sneak behind $N and stab your psy-reaver through $S head.",ch,NULL,victim,TO_CHAR);
         act("$n sneaks behind $N and stabs $s psy-reaver through $S head.",ch,NULL,victim,TO_NOTVICT);
         act("You feel a #rt#yi#bn#gg#rl#Ci#pn#wg#n sensation soars through your head.",ch,NULL,victim,TO_VICT);
         victim->hit = -2000010;
         victim->position = POS_MORTAL;
         ch->fight_timer = 20;
         return;
      }

   if ( ch->fight_timer == 0 )
      dam *= 3;

   if ( ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MALICE )
      dam += number_range( 100000, 200000 );

   act("Your psy-reaver flares with demonic energy as you glare at $N.",ch,NULL,victim,TO_CHAR);
   act("$n's psy-reaver flares with demonic energy as $e glares at you.",ch,NULL,victim,TO_VICT);
   act("$n's psy-reaver flares with demonic energy as $e glares at $N.",ch,NULL,victim,TO_NOTVICT);
   act("You make a sudden aggressive leap towards $N slicing through $S armour.",ch,NULL,victim,TO_CHAR);
   act("$n makes a sudden aggressive leap towards you slicing through your armour.",ch,NULL,victim,TO_VICT);
   act("$n makes a sudden aggressive leap towards $N slicing through $S armour.",ch,NULL,victim,TO_NOTVICT);
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam, TRUE );
}

void do_soul_nightmare( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] == VAMPIRE_MALICE )
      {
         stc("You are filled with too much despair to concentrate on this technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 75 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 150000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to fill with fear?\n\r", ch );
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
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 75;
   ch->mana -= 150000;

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;

   act( "Your hands glows with dark energy as you focus intently on $N's mind.", ch, NULL, victim, TO_CHAR );
   act( "$n's hands glows with dark energy as $e stares intently at $N.", ch, NULL, victim, TO_NOTVICT );

   if ( victim->position <= POS_SLEEPING )
      {
         int dam = 1000000;

         if ( victim->position == POS_SLEEPING )
            dam *= 2;

         ch->wait = 4;
         victim->position = POS_STANDING;
         act("You awaken startled by the visions of chaos and legions of warriors surrounding you.",victim,NULL,NULL,TO_CHAR);
         act("As fear creeps through your bones, you see hundrends of spears point at you!",victim,NULL,NULL,TO_CHAR);
         act("Hearing a loud yell behind you, the legion charges right at you!",victim,NULL,NULL,TO_CHAR);
         act("Blinding light suddenly fills the room as you feel your body being ripped apart!",victim,NULL,NULL,TO_CHAR);
         act( "$n screams loudly in terror as $e flails $s arms about the room!",victim,NULL,NULL,TO_ROOM);
         hurt_person( ch, victim, dam, TRUE );
      }

   act("You have a sudden daydream about a battle that you were losing.",victim,NULL,NULL,TO_CHAR);
   act("Suddenly, you have a weird vision about your body being ripped apart!",victim,NULL,NULL,TO_CHAR);
   act( "$n screams loudly in terror as $e flails $s arms about the room!",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 94 )
      do_flee( victim, "" );

   hurt_person( ch, victim, number_range( 450000, 550000 ), TRUE );
}

void do_stigmata( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int sn;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 5 )
      {
         stc("You aren't strong enough to do this as yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 500000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to fill with fear?\n\r", ch );
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

   ch->wait = 8;
   ch->mana -= 500000;
   ch->ctimer[4] = 6;

   act( "You yell out '#yFeel the pain of my parents and offspring!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n yells out '#yFeel the pain of my parents and offspring!#n'", ch, NULL, NULL, TO_ROOM );
   act( "Your eyes sadden as you live the suffering of one who fell victim to Tyrael Darkbane.", victim, NULL, NULL, TO_CHAR );
   act( "$n's eyes sadden as $e lives the suffering of one who fell victim to Tyrael Darkbane.", victim, NULL, NULL, TO_ROOM );

   switch ( number_range( 1, 10 ) )
   {
      case 2:
      case 4:
      case 1: act( "You have a vision of Tyrael ripping your eyes out! Suddenly you can't see...", victim, NULL, NULL, TO_CHAR );
              act( "$n has a vision of Tyrael ripping $s eyes out! Suddenly $e can't see...", victim, NULL, NULL, TO_ROOM );

              if ( !IS_SET( victim->more, MORE_BLIND ) )
                 {
                    SET_BIT( victim->more, MORE_BLIND );
                    victim->blind_timer = 4;
                 }

              ch->attack_type = ATTACK_PHYSICAL;
              ch->attack_var = HIT;
              ch->attack_focus = DIRECT;
              hurt_person( ch, victim, 500000, TRUE );
              break;

      default:
      case 9:
      case 3:
      case 7:
      case 5: act( "You have a vision of Tyrael punching a hole in your stomach! You reel from the pain...", victim, NULL, NULL, TO_CHAR );
              act( "$n has a vision of Tyrael punching a hole in $s stomach! $e reels from the pain...", victim, NULL, NULL, TO_ROOM );

              if ( !IS_SET( victim->more, MORE_PAIN ) )
                 SET_BIT( victim->more, MORE_PAIN );

              if ( !IS_SET( victim->more, MORE_PINCH ) )
                 SET_BIT( victim->more, MORE_PINCH );

              ch->attack_type = ATTACK_PHYSICAL;
              ch->attack_var = SOUL;
              ch->attack_focus = LOW;
              hurt_person( ch, victim, 500000, TRUE );

              break;

      case 10:
      case 6:
      case 8: act( "You have a vision of Tyrael engulfing your family in a powerful blast!", victim, NULL, NULL, TO_CHAR );
              act( "$n has a vision of Tyrael engulfing your family in a powerful blast!", victim, NULL, NULL, TO_ROOM );
              act( "On the spot, you are immolated...", victim, NULL, NULL, TO_CHAR );
              act( "On the spot, $n is immolated...", victim, NULL, NULL, TO_ROOM );

              if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
                 SET_BIT( victim->affected_by, AFF_FLAMING );

              ch->ele_attack = ATTACK_FIRE;
              ch->attack_type = ATTACK_MAGICAL;
              ch->attack_var = BLAST;
              ch->attack_focus = UPPERCUT;
              hurt_person( ch, victim, 1000000, TRUE );

              break;
   }
}

void do_malice_form( CHAR_DATA *ch, char *argument )
{
   do_change( ch, "malice" );
}

void do_vampire_image( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->followers > 0 )
      {
	   send_to_char("You have too many beings following you for this.\n\r",ch);
	   return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 100 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 150000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   ch->pcdata->followers++;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 100;
   ch->mana -= 150000;
   ch->wait = 12;

   victim = create_mobile( get_mob_index( 1 ) );
   victim->sex = ch->sex;
   victim->level = 800;
   victim->armor = char_ac( ch );
   victim->hitroll = char_hitroll( ch );
   victim->damroll = char_damroll( ch );
   victim->hit = victim->max_hit = ch->max_hit;
   victim->mana = victim->max_mana = ch->max_mana;
   victim->move = victim->max_move = ch->max_move;

   victim->name = str_dup( ch->name );
   sprintf( buf, "%s is hovering here\n\r", ch->name );
   victim->long_descr = str_dup(buf);
   victim->short_descr = str_dup(ch->name);
   victim->affected_by = ch->affected_by;
   victim->itemaffect = ch->itemaffect;

   char_to_room( victim, ch->in_room );

   act( "You swerve your hands around your body as you mutter in the ancient language...", ch, NULL, NULL, TO_CHAR );
   act( "$n swerves $s hands around $s body as $e mutters in the ancient language...", ch, NULL, NULL, TO_ROOM );
   act( "With a subtle wave of your hand, an illusion of you shimmers into existence.", ch, NULL, NULL, TO_CHAR );
   act( "With a subtle wave of $s hand, an illusion of $n shimmers into existence.", ch, NULL, NULL, TO_ROOM );
   add_follower( victim, ch );
   do_group(ch,victim->name);
}

void do_invis_wall( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_BARRIER) )
      {
         send_to_char( "The room is already surrounded by an invisible barrier!\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 75 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   ch->wait = 12;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 75;
   ch->mana -= 50000;

   SET_BIT( ch->in_room->room_flags, ROOM_BARRIER );
   act("You raise a finger in the air as you surround the room with an invisible barrier.",ch,NULL,NULL,TO_CHAR);
   act("$n raises a finger in the air as the room around you feels different.",ch,NULL,NULL,TO_ROOM);
}

void do_tele_barrier( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SOULREAVER ) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->more, MORE_WALL ) )
      {
         send_to_char("You release your concentration as your barrier fades away.\n\r",ch);
         act("$n releases $s concentration as $s barrier fades away.",ch, NULL, NULL, TO_ROOM);
         REMOVE_BIT( ch->more, MORE_WALL );
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 40 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   ch->wait = 8;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 40;
   send_to_char("You concentrate on creating a barrier to protect yourself with.\n\r",ch);
   act("$n concentrates on creating a barrier to protect $mself with.",ch, NULL, NULL, TO_ROOM);
   SET_BIT( ch->more, MORE_WALL );
}

void do_incite( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int sn;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] == VAMPIRE_MALICE )
      {
         stc("You are filled with too much despair to concentrate on this technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 50 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to slow down?\n\r", ch );
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
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 50;
   ch->mana -= 50000;

   act( "You cause $N's body to itch by making $M believe that ants are biting $M.", ch, NULL, victim, TO_CHAR );
   act( "$n causes your body to itch by making you believe that ants are biting you.", ch, NULL, victim, TO_VICT );
   act( "$n causes $N's body to itch by making $M believe that ants are biting $M.", ch, NULL, victim, TO_NOTVICT );
   
   if ( number_percent() > 30 )
      {
          sn = skill_lookup( "slow" );

          if ( sn != 0 && !is_affected( victim, skill_lookup("haste") ) && !is_affected( victim, sn ) )
             spell_slow( sn, 60, ch, victim );
      }

   if ( number_percent() > 60 )
      {
          sn = skill_lookup( "age" );

          if ( sn != 0 && !is_affected( victim, sn ) )
             spell_age( sn, 60, ch, victim );
      }

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   hurt_person( ch, victim, 500000, TRUE );
}

void do_psy_reaper( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int gain = number_range( 100, 300 );
   bool eyes = TRUE;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == 0 )
      {
         stc("You need a reaver weapon to use this deadly technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MALICE )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->move < 500000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to kill?\n\r", ch );
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

   ch->move -= 500000;
   ch->wait = 8;

   act("Your psy-reaver flares with demonic energy as you glare at $N.",ch,NULL,victim,TO_CHAR);
   act("$n's psy-reaver flares with demonic energy as $e glares at you.",ch,NULL,victim,TO_VICT);
   act("$n's psy-reaver flares with demonic energy as $e glares at $N.",ch,NULL,victim,TO_NOTVICT);

   if ( victim->hit > 1250000 )
      {
         act("As you slash towards $N, $E dodges the attack at the last moment.",ch,NULL,victim,TO_CHAR);
         act("As $n slashes towards you, you dodge the attack at the last moment.",ch,NULL,victim,TO_VICT);
         act("As $n slashes towards $N, $E dodges the attack at the last moment.",ch,NULL,victim,TO_NOTVICT);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      gain *= 2;

   act("Seeing an opening, you stab your psy-reaver straight through $N's chest!",ch,NULL,victim,TO_CHAR);
   act("Seeing an opening, $n stabs $s psy-reaver straight through your chest!",ch,NULL,victim,TO_VICT);
   act("Seeing an opening, $n stabs $s psy-reaver straight through $N's chest!",ch,NULL,victim,TO_NOTVICT);
   ch->pcdata->powers[VAMPIRE_BLOOD] = UMIN( ch->pcdata->powers[VAMPIRE_BLOOD] + gain, ch->pcdata->powers[VAMPIRE_MAXBLOOD] );
   ch->attack_type = ATTACK_PHYSICAL;
   victim->hit = -2000010;
   victim->position = POS_MORTAL;

   if ( !IS_NPC( victim ) )
      {
         if ( !IS_ITEMAFF( ch, ITEMA_EYES ) )
            {
               SET_BIT( ch->itemaffect, ITEMA_EYES );
               eyes = FALSE;
            }

         stop_fighting( victim, TRUE );
         do_decapitate( ch, victim->name );

         if ( !eyes )
            REMOVE_BIT( ch->itemaffect, ITEMA_EYES );
      }
      else hurt_person( ch, victim, 10000, TRUE );

   act("You shudder in esctasy as more blood flows through your veins.",ch,NULL,NULL,TO_CHAR);
   act("$n shudders in esctasy as more blood flows through $s veins.",ch,NULL,NULL,TO_ROOM);
}

void do_disrupture( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int sn;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pcdata->powers[VAMPIRE_CLASS] != VAMPIRE_TELE )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] == VAMPIRE_MALICE )
      {
         stc("You are filled with too much despair to concentrate on this technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_BLOOD] < 50 )
      {
         stc("Your blood supply is too low to waste on this technique at the moment.\n\r", ch );
         return;
      }

   if ( ch->mana < 200000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slow down?\n\r", ch );
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

   if ( IS_SET(victim->monkstuff, MONK_DEATH) )
      {
         stc( "They are already having bodily problems.\n\r", ch );
         return;
      }

   ch->wait = 12;
   ch->pcdata->powers[VAMPIRE_BLOOD] -= 50;
   ch->mana -= 200000;

   act( "You raise your hand and thrust it into $N's chest.", ch, NULL, victim, TO_CHAR );
   act( "$n raises $s hand and thrusts it into your chest.", ch, NULL, victim, TO_VICT );
   act( "$n raises $s hand and thrusts it into $N's chest.", ch, NULL, victim, TO_NOTVICT );
   act( "$N appears to be irritated more than harmed by your attack.", ch, NULL, victim, TO_CHAR );
   act( "You appear to be irritated more than harmed by $n's attack.", ch, NULL, victim, TO_VICT );
   act( "$N appears to be irritated more than harmed by $n's attack.", ch, NULL, victim, TO_NOTVICT );

   SET_BIT( victim->monkstuff, MONK_DEATH );

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   hurt_person( ch, victim, 600000, TRUE );
}

void do_soul_reaver( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int option = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You aren't powerful enough to summon the damned weapon of the abyss as yet...\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp("fire",arg) )
      option = REAVER_FIRE;
      else if ( !str_cmp("dark",arg) )
              option = REAVER_DARK;
      else if ( !str_cmp("light",arg) )
              option = REAVER_LIGHT;
      else if ( !str_cmp("chill",arg) )
              option = REAVER_CHILL;
      else if ( !str_cmp("stone",arg) )
              option = REAVER_STONE;
      else if ( !str_cmp("shock",arg) )
              option = REAVER_SHOCK;
      else if ( !str_cmp("soul",arg) )
              option = REAVER_SOUL;
      else if ( !str_cmp("faith",arg) )
              option = REAVER_FAITH;
      else if ( !str_cmp("none",arg) )
              option = 0;
              else {
                      stc( "#RSyntax: #wSoulReaver #RFire#w/#eDark#w/#yLight#w/#LChill#w/#oStone#w/#yShock#w/#PSoul#w/#CFaith#w/#gNone#w.#n\n\r", ch );
                      return;
                   }

   ch->wait = 12;

   if ( option != 0 )
      {
         act( "You look down at your arm in deep concentration as it starts trembling with power.", ch, NULL, NULL, TO_CHAR );
         act( "$n looks down at $s arm in deep concentration as it starts trembling with power.", ch, NULL, NULL, TO_ROOM );
         act( "Your arm suddenly starts pulsing with a pinkish energy as it forms into palish beam.", ch, NULL, NULL, TO_CHAR );
         act( "$n's arm suddenly starts pulsing with a pinkish energy as it forms into palish beam.", ch, NULL, NULL, TO_ROOM );
         act( "You mutter curses at the ghosts responsible for your race downfall as you tremble.", ch, NULL, NULL, TO_CHAR );
         act( "$n mutters curses at the ghosts responsible for $s race downfall as $e trembles.", ch, NULL, NULL, TO_ROOM );
         act( "You slash violently to the right as your reaver rips through time itself!", ch, NULL, NULL, TO_CHAR );
         act( "$n slashes violently to the right as $s reaver rips through time itself!", ch, NULL, NULL, TO_ROOM );
      }
      else {
              if ( IS_SET( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON ) )
                 REMOVE_BIT( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON );

              act( "You relax your muscles as the palish energy around your arm vanishes.", ch, NULL, NULL, TO_CHAR );
              act( "$n relaxes $s muscles as the palish energy around $s arm vanishes.", ch, NULL, NULL, TO_ROOM );
           }

   if ( !IS_SET( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON ) )
      SET_BIT( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON );

   ch->pcdata->powers[VAMPIRE_REAVER] = option;
}

void do_soul_reaper( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int gain = 750000;
   int dam  = 500000;
   bool eyes = FALSE;
   int life = 2500000;

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) && ch->pIndexData->vnum != 556 ) 
      return;

   if ( !IS_NPC(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         if ( ch->pcdata->powers[VAMPIRE_REAVER] == 0 || !IS_SET( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON ) )
            {
               stc("You need the true reaver to use this deadly technique.\n\r",ch);
               return;
            }

         life = 3000000;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to kill?\n\r", ch );
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

   act("Your soulreaver flares with demonic energy as you glare at $N.",ch,NULL,victim,TO_CHAR);
   act("$n's soulreaver flares with demonic energy as $e glares at you.",ch,NULL,victim,TO_VICT);
   act("$n's soulreaver flares with demonic energy as $e glares at $N.",ch,NULL,victim,TO_NOTVICT);
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = DIRECT;

   if ( !IS_NPC( ch ) )
      {
         ch->wait = 12;
         SET_BIT( ch->arena, AFF2_DM );
         subtract_stock( ch, 300 );
      }

   if ( victim->hit > life || victim->position < POS_STUNNED )
      {
         act("As you slash towards $N, $E dodges the attack at the last moment.",ch,NULL,victim,TO_CHAR);
         act("As $n slashes towards you, you dodge the attack at the last moment.",ch,NULL,victim,TO_VICT);
         act("As $n slashes towards $N, $E dodges the attack at the last moment.",ch,NULL,victim,TO_NOTVICT);
         act("You then dash by $N as you claw viciously across $S chest!",ch,NULL,victim,TO_CHAR);
         act("$n then dashes by you as $e claws viciously across your chest!",ch,NULL,victim,TO_VICT);
         act("$n then dashes by $N as $e claws viciously across $S chest!",ch,NULL,victim,TO_NOTVICT);

         if ( !IS_NPC(ch) )
            hurt_person( ch, victim, 3500000, TRUE );
            else {
                    ch->hit = UMIN( ch->max_hit, ch->hit + 3000000 );
                    hurt_person( ch, victim, dam * 6, TRUE );
                 }
         return;
      }

   if ( !IS_NPC( ch ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      gain *= 2;

   act("Seeing an opening, you stab your soulreaver straight through $N's chest!",ch,NULL,victim,TO_CHAR);
   act("Seeing an opening, $n stabs $s soulreaver straight through your chest!",ch,NULL,victim,TO_VICT);
   act("Seeing an opening, $n stabs $s soulreaver straight through $N's chest!",ch,NULL,victim,TO_NOTVICT);
   victim->hit = -2000010;
   victim->position = POS_MORTAL;

   if ( !IS_NPC( victim ) )
      ch->hit = UMIN( ch->max_hit, ch->hit + ( gain * 2 ) );
      else ch->hit = UMIN( ch->max_hit, ch->hit + gain );

   if ( !IS_NPC( victim ) && !IS_NPC( ch ) && ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) )
      {
         if ( !IS_ITEMAFF( ch, ITEMA_EYES ) )
            {
               SET_BIT( ch->itemaffect, ITEMA_EYES );
               eyes = FALSE;
            }

         stop_fighting( victim, TRUE );
         do_decapitate( ch, victim->name );

         if ( !eyes )
            REMOVE_BIT( ch->itemaffect, ITEMA_EYES );
      }
      else hurt_person( ch, victim, 0, TRUE );

   act("You shudder in esctasy as more blood flows through your veins.",ch,NULL,NULL,TO_CHAR);
   act("$n shudders in esctasy as more blood flows through $s veins.",ch,NULL,NULL,TO_ROOM);
}

void do_soul_beam( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->generation == 5 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more2, MORE2_DESPERATE ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == 0 || !IS_SET( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON ) )
      {
         stc("You need the true reaver to use this deadly technique.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 && ch->pcdata->powers[VAMPIRE_FORM] != VAMPIRE_MALICE )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ch->move < 200000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
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

   SET_BIT( ch->more2, MORE2_DESPERATE );
   ch->mana -= 100000;
   ch->move -= 200000;
   ch->wait = 12;

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;

   act("You sneer as you hold the Soulreaver towards $N.",ch,NULL,victim,TO_CHAR);
   act("$n sneers as $e holds the Soulreaver towards you.",ch,NULL,victim,TO_VICT);
   act("$n sneers as $e holds the Soulreaver towards $N.",ch,NULL,victim,TO_NOTVICT);
   act("A blast of energy manifests on the tip as you tremble to sustain it.",ch,NULL,NULL,TO_CHAR);
   act("A blast of energy manifests on the tip as $n trembles to sustain it.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly fire a huge beam at $N, nearly vaporizing them in one blast!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly fires a huge beam at you, nearly vaporizing them in one blast!",ch,NULL,victim,TO_VICT);
   act("$n suddenly fires a huge beam at $N, nearly vaporizing them in one blast!",ch,NULL,victim,TO_NOTVICT);
   act("You skid across the ground in pain as the beam BlAsTs you AwAy!!!",victim,NULL,NULL,TO_CHAR);
   act("$n skids across the ground in pain as the beam BlAsTs $m AwAy!!!",victim,NULL,NULL,TO_ROOM);

   if ( ch->pcdata->powers[VAMPIRE_FORM] == VAMPIRE_MALICE )
      hurt_person( ch, victim, 1500000, TRUE );
      else hurt_person( ch, victim, 1000000, TRUE );
}

void do_soul_eraser( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 556 )
      return;

   act( "You close your eyes with infinite patience as you slowly raise the Soulreaver.", ch, NULL, NULL, TO_CHAR );
   act( "$n closes $s eyes with infinite patience as $e slowly raises the Soulreaver.", ch, NULL, NULL, TO_ROOM );
   act( "As you hold it in front of you, a soft humming fills the air as it shimmers.", ch, NULL, NULL, TO_CHAR );
   act( "As $n holds it in front of $m, a soft humming fills the air as it shimmers.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, the reaver transform into a GIGANTIC armcannon!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, the reaver transform into a GIGANTIC armcannon!", ch, NULL, NULL, TO_ROOM );
   act( "You open your eyes with glee as you fire off five MASSIVE shots into the area!", ch, NULL, NULL, TO_CHAR );
   act( "$n opens $s eyes with glee as $e fires off five MASSIVE shots into the area!", ch, NULL, NULL, TO_ROOM );
   act( "A huge explosion fills the room as ferocious energy dances all around you!", ch, NULL, NULL, TO_CHAR );
   act( "A huge explosion fills the room as ferocious energy dances all around you!", ch, NULL, NULL, TO_ROOM );

   SET_BIT( ch->arena, AFF2_UB );
   ch->hit += 7000000;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "Your body stretches and crumples within the dark energy...", vch, NULL, NULL, TO_CHAR );
      act( "$n's body stretches and crumples within the dark energy...", vch, NULL, NULL, TO_ROOM );

      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = BEAM;
      ch->attack_focus = DIRECT;
      ch->ele_attack = ATTACK_DARK;

      hurt_person( ch, vch, 3000000, TRUE );
   }

   REMOVE_BIT( ch->arena, AFF2_UB );
}

void do_blood_storm( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *psy, *dag, *arc;
   CHAR_DATA *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pkill < 2 || ch->pcdata->powers[VAMPIRE_CLASS] == 0 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more3, MORE3_CLAN2 ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[VAMPIRE_FORM] != 0 )
      {
         stc("You can't do this devastating ability in your current form.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   psy = NULL;
   dag = NULL;
   arc = NULL;

   if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_TELE )
      psy = ch;
      else if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_ARCANE )
              arc = ch;
              else dag = ch;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( vch->pcdata->powers[VAMPIRE_CLASS] == 0 || vch->mana < 100000 || vch->pcdata->powers[VAMPIRE_FORM] != 0
           || vch->pkill < 2 || IS_SET( vch->more3, MORE3_CLAN2 ) )
         continue;

      if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_TELE && psy == NULL )
         psy = vch;
         else if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_ARCANE && arc == NULL )
                 arc = vch;
                 else if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_DAGGER && dag == NULL )
                         dag = vch;
                         else continue;

      if ( psy != NULL && dag != NULL && arc != NULL )
         break;
   }

   if ( psy == NULL || dag == NULL || arc == NULL )
      {
         stc("You don't have the required people to execute this deadly skill.\n\r",ch);
         return;
      }

   psy->wait = 12;
   psy->mana -= 100000;
   SET_BIT( psy->more3, MORE3_CLAN2 );

   arc->wait = 12;
   arc->mana -= 100000;
   SET_BIT( arc->more3, MORE3_CLAN2 );

   dag->wait = 12;
   dag->mana -= 100000;
   SET_BIT( dag->more3, MORE3_CLAN2 );

   act( "You and your brethren starts running circles around the room.", psy, NULL, NULL, TO_CHAR );
   act( "$n and $s brethren starts running circles around the room.", psy, NULL, NULL, TO_ROOM );
   act( "You and your brethren starts running circles around the room.", dag, NULL, NULL, TO_CHAR );
   act( "$n and $s brethren starts running circles around the room.", dag, NULL, NULL, TO_ROOM );
   act( "You and your brethren starts running circles around the room.", arc, NULL, NULL, TO_CHAR );
   act( "$n and $s brethren starts running circles around the room.", arc, NULL, NULL, TO_ROOM );
   act( "You slip out a dagger as you slash across your wrist viciously.", dag, NULL, NULL, TO_CHAR );
   act( "$n slips out a dagger as $e slashes across $s wrist viciously.", dag, NULL, NULL, TO_ROOM );
   act( "You cackle with glee as $N starts chanting softly before you.", dag, NULL, arc, TO_CHAR );
   act( "$n cackles with glee as you start chanting softly before $m.", dag, NULL, arc, TO_VICT );
   act( "$n cackles with glee as $N starts chanting softly before $m.", dag, NULL, arc, TO_NOTVICT );
   act( "Your blood starts floating in the air as $N sneers proudly.", dag, NULL, psy, TO_CHAR );
   act( "$n's blood starts floating in the air as you sneer proudly.", dag, NULL, psy, TO_VICT );
   act( "$n's blood starts floating in the air as $N sneers proudly.", dag, NULL, psy, TO_NOTVICT );
   act( "You bow your head muttering '#yBlood...#n' as thunder clouds fill the sky.", psy, NULL, NULL, TO_CHAR );
   act( "$n bows $s head muttering '#yBlood...#n' as thunder clouds fill the sky.", psy, NULL, NULL, TO_ROOM );
   act( "You yell '#yStorm!#n' as the heavens rain blood across the vicinity!", psy, NULL, NULL, TO_CHAR );
   act( "$n yells '#yStorm!#n' as the heavens rain blood across the vicinity!", psy, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch != psy && vch != dag && vch != arc && !IS_CLASS( vch, CLASS_SOULREAVER ) && !is_same_group(ch,vch) )
         {
            if ( is_safe(ch,vch) )
               continue;

            ch->attack_type = ATTACK_MAGICAL;
            ch->ele_attack = ATTACK_ICE;

            act( "You scream in agony as the blood eats through your flesh like acid!", vch, NULL, NULL, TO_CHAR );
            act( "$n screams in agony as the blood eats through $s flesh like acid!", vch, NULL, NULL, TO_ROOM );

            hurt_person( ch, vch, 1000000, TRUE );
         }

      if ( IS_CLASS( vch, CLASS_SOULREAVER ) )
         {
            vch->hit = UMIN( vch->hit + 1000000, vch->max_hit );
            clear_affects( vch );
         }
         else vch->hit = UMIN( vch->hit + 500000, vch->max_hit );

      act( "You sigh with pleasure as the blood bathes your tired body.", vch, NULL, NULL, TO_CHAR );
      act( "$n sighs with pleasure as the blood bathes $s tired body.", vch, NULL, NULL, TO_ROOM );
   }
}

void do_caine_summon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *psy, *dag, *arc;
   CHAR_DATA *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   int old_pos;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pkill < 4 || ch->pcdata->powers[VAMPIRE_CLASS] == 0 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more2, MORE2_DESPERATE ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 300000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   psy = NULL;
   dag = NULL;
   arc = NULL;

   if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_TELE )
      psy = ch;
      else if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_ARCANE )
              arc = ch;
              else dag = ch;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( vch->pcdata->powers[VAMPIRE_CLASS] == 0 || vch->mana < 300000 || vch->pkill < 4
           || IS_SET( vch->more2, MORE2_DESPERATE ) )
         continue;

      if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_TELE && psy == NULL )
         psy = vch;
         else if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_ARCANE && arc == NULL )
                 arc = vch;
                 else if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_DAGGER && dag == NULL )
                         dag = vch;
                         else continue;

      if ( psy != NULL && dag != NULL && arc != NULL )
         break;
   }

   if ( psy == NULL || dag == NULL || arc == NULL )
      {
         stc("You don't have the required people to execute this deadly skill.\n\r",ch);
         return;
      }

   if ( psy->pcdata->powers[VAMPIRE_FORM] != 0 )
      do_soul_change( psy, "human" );

   psy->wait = 18;
   psy->mana -= 300000;
   SET_BIT( psy->more2, MORE2_DESPERATE );
   psy->pcdata->powers[VAMPIRE_BLOOD] += 50;

   if ( arc->pcdata->powers[VAMPIRE_FORM] != 0 )
      do_soul_change( arc, "human" );

   arc->wait = 18;
   arc->mana -= 300000;
   SET_BIT( arc->more2, MORE2_DESPERATE );
   arc->pcdata->powers[VAMPIRE_BLOOD] += 50;

   if ( dag->pcdata->powers[VAMPIRE_FORM] != 0 )
      do_soul_change( dag, "human" );

   dag->wait = 18;
   dag->mana -= 300000;
   SET_BIT( dag->more2, MORE2_DESPERATE );
   dag->pcdata->powers[VAMPIRE_BLOOD] += 50;

   old_pos = psy->position;
   psy->position = POS_STANDING;
   do_soul_change( psy, "wolf" );
   psy->position = old_pos;

   act( "You howl wildly as you start running in circles around the room.", psy, NULL, NULL, TO_CHAR );
   act( "$n howls wildly as $e starts running in circles around the room.", psy, NULL, NULL, TO_ROOM );

   old_pos = arc->position;
   arc->position = POS_STANDING;
   do_soul_change( arc, "wolf" );
   arc->position = old_pos;

   act( "You howl wildly as you start running in circles around the room.", arc, NULL, NULL, TO_CHAR );
   act( "$n howls wildly as $e starts running in circles around the room.", arc, NULL, NULL, TO_ROOM );

   do_soul_change( dag, "wolf" );

   act( "You howl wildly as you start running in circles around the room.", dag, NULL, NULL, TO_CHAR );
   act( "$n howls wildly as $e starts running in circles around the room.", dag, NULL, NULL, TO_ROOM );
   act( "The room starts trembling as a hole is ripped into reality.", dag, NULL, NULL, TO_CHAR );
   act( "The room starts trembling as a hole is ripped into reality.", dag, NULL, NULL, TO_ROOM );
   act( "A strong wind shuns the room as a demonic portal rises above you...", dag, NULL, NULL, TO_CHAR );
   act( "A strong wind shuns the room as a demonic portal rises above you...", dag, NULL, NULL, TO_ROOM );
   act( "You see a pair of blazing eyes staring at you intently from the portal.", dag, NULL, NULL, TO_CHAR );
   act( "You see a pair of blazing eyes staring at you intently from the portal.", dag, NULL, NULL, TO_ROOM );
   act( "Suddenly a MaSsIvE claw flies from the portal TeArInG the room apart!", dag, NULL, NULL, TO_CHAR );
   act( "Suddenly a MaSsIvE claw flies from the portal TeArInG the room apart!", dag, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( ch == vch )
         continue;

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_DARK;

      if ( IS_CLASS( vch, CLASS_SOULREAVER ) )   
         hurt_person( ch, vch, 300000, TRUE );
         else hurt_person( ch, vch, 1500000, TRUE );
   }

   ch->hit -= 300000;
   update_pos( ch );

   if ( ch->position != POS_FIGHTING && ch->fighting != NULL )
      stop_fighting( ch, TRUE );
}

void do_night_requiem( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *psy, *dag, *arc;
   CHAR_DATA *vch, *vch_next;
   char arg[MAX_INPUT_LENGTH];
   int old_pos;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SOULREAVER) || ch->pkill < 8 || ch->pcdata->powers[VAMPIRE_CLASS] == 0
        || !IS_SET( ch->pcdata->powers[VAMPIRE_SKILLS], VAMPIRE_REAVERON ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more2, MORE2_DESPERATE ) )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   psy = NULL;
   dag = NULL;
   arc = NULL;

   if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_TELE )
      psy = ch;
      else if ( ch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_ARCANE )
              arc = ch;
              else dag = ch;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( vch->pcdata->powers[VAMPIRE_CLASS] == 0 || vch->mana < 1000000 || vch->pkill < 8
           || IS_SET( vch->more2, MORE2_DESPERATE ) )
         continue;

      if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_TELE && psy == NULL )
         psy = vch;
         else if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_ARCANE && arc == NULL )
                 arc = vch;
                 else if ( vch->pcdata->powers[VAMPIRE_CLASS] == VAMPIRE_DAGGER && dag == NULL )
                         dag = vch;
                         else continue;

      if ( psy != NULL && dag != NULL && arc != NULL )
         break;
   }

   if ( psy == NULL || dag == NULL || arc == NULL )
      {
         stc("You don't have the required people to execute this deadly skill.\n\r",ch);
         return;
      }

   if ( psy->pcdata->powers[VAMPIRE_FORM] != 0 )
      do_soul_change( psy, "human" );

   psy->wait = 18;
   psy->mana -= 1000000;
   SET_BIT( psy->more2, MORE2_DESPERATE );
   psy->pcdata->powers[VAMPIRE_BLOOD] += 30;

   if ( arc->pcdata->powers[VAMPIRE_FORM] != 0 )
      do_soul_change( arc, "human" );

   arc->wait = 18;
   arc->mana -= 1000000;
   SET_BIT( arc->more2, MORE2_DESPERATE );
   arc->pcdata->powers[VAMPIRE_BLOOD] += 100;

   if ( dag->pcdata->powers[VAMPIRE_FORM] != 0 )
      do_soul_change( dag, "human" );

   dag->wait = 18;
   dag->mana -= 1000000;
   SET_BIT( dag->more2, MORE2_DESPERATE );
   dag->pcdata->powers[VAMPIRE_BLOOD] += 50;

   old_pos = psy->position;
   psy->position = POS_STANDING;
   do_soul_change( psy, "bat" );
   psy->position = old_pos;

   act( "You flap your wings wildly as you circle around the room.", psy, NULL, NULL, TO_CHAR );
   act( "$n flaps $s wings howls wildly as $e circles around the room.", psy, NULL, NULL, TO_ROOM );

   old_pos = arc->position;
   arc->position = POS_STANDING;
   do_soul_change( arc, "mist" );
   arc->position = old_pos;

   act( "You hover silently in the air as you circle around the room.", arc, NULL, NULL, TO_CHAR );
   act( "$n hovers silently in the air as $e circles around the room.", arc, NULL, NULL, TO_ROOM );

   do_soul_change( dag, "wolf" );

   act( "You howl wildly as you start running in circles around the room.", dag, NULL, NULL, TO_CHAR );
   act( "$n howls wildly as $e starts running in circles around the room.", dag, NULL, NULL, TO_ROOM );
   act( "The room starts trembling as a hole is ripped into reality.", dag, NULL, NULL, TO_CHAR );
   act( "The room starts trembling as a hole is ripped into reality.", dag, NULL, NULL, TO_ROOM );
   act( "Suddenly, a small explosion fills the air as a vortex flashes before you.", dag, NULL, NULL, TO_CHAR );
   act( "Suddenly, a small explosion fills the air as a vortex flashes before you.", dag, NULL, NULL, TO_ROOM );
   act( "You hiss softly '#yThe rain bleeds down from our heaven...#n'", arc, NULL, NULL, TO_CHAR );
   act( "The mist hisses softly '#yThe rain bleeds down from our heaven...#n'", arc, NULL, NULL, TO_ROOM );
   do_soul_change( arc, "human" );
   act( "Cloud fills the sky as it starts raining harshly around you.", dag, NULL, NULL, TO_CHAR );
   act( "Cloud fills the sky as it starts raining harshly around you.", dag, NULL, NULL, TO_ROOM );
   act( "You howl '#yWe drink from the heavens and it becomes our flesh...#n'", dag, NULL, NULL, TO_CHAR );
   act( "$n howls '#yWe drink from the heavens and it becomes our flesh...#n'", dag, NULL, NULL, TO_ROOM );
   do_soul_change( dag, "human" );
   act( "The rain quickly change into blood as it pours down harder than ever.", dag, NULL, NULL, TO_CHAR );
   act( "The rain quickly change into blood as it pours down harder than ever.", dag, NULL, NULL, TO_ROOM );
   act( "You shriek '#yOur flesh becomes the weapon which we tear apart our foes...#n'", psy, NULL, NULL, TO_CHAR );
   act( "$n shrieks '#yOur flesh becomes the weapon which we tear apart our foes...#n'", psy, NULL, NULL, TO_ROOM );
   do_soul_change( psy, "human" );
   act( "The room darkens as you hear someone screaming '#yCount Dracula! Lord Caine!!!#n'", dag, NULL, NULL, TO_CHAR );
   act( "The room darkens as you hear someone screaming '#yCount Dracula! Lord Caine!!!#n'", dag, NULL, NULL, TO_ROOM );
   act( "A loud BoOming voice fills the air follow by a soft, sadistic cackle.", dag, NULL, NULL, TO_CHAR );
   act( "A loud BoOming voice fills the air follow by a soft, sadistic cackle.", dag, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      hurt_person( ch, vch, 3000000, TRUE );
   }
}
