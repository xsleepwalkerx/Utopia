/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'licensedoc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
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

void do_swirling_smite( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 700000;

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Stun whom?\n\r", ch );
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

   if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
      dam += 100000;

   act( "You fall towards the ground thrusting your palm into $N's chin viciously!", ch, NULL, victim, TO_CHAR );
   act( "$n falls towards the ground thrusting $s palm into your chin viciously!", ch, NULL, victim, TO_VICT );
   act( "$n falls towards the ground thrusting $s palm into $N's chin viciously!", ch, NULL, victim, TO_NOTVICT );
   act( "Swirling your palms, a manifest of dark energy radiates you as you SmAsH $N's chest!", ch, NULL, victim, TO_CHAR );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS your chest!", ch, NULL, victim, TO_VICT );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS $N's chest!", ch, NULL, victim, TO_NOTVICT );

   if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
      dam /= 2.5;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );

   if ( victim != NULL && victim->position > POS_STUNNED && !is_martial( victim ) 
        && number_percent() > 90 - devil_luck( ch, victim, 5 ) )
      {
         stop_fighting( victim, FALSE );
         victim->position = POS_STUNNED;
      }
}

void do_flash_kick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 500000 )
      {
         send_to_char("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Kick whom?\n\r", ch );
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
 
   ch->wait = 18;
   ch->move -= 500000;

   act("Your foot glows with bright energy as you stagger backwards.",ch,NULL,NULL,TO_CHAR);
   act("$n's foot glows with bright energy as $e staggers backwards.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yHai!#n' as you SmAsH your foot upwards $N's face at the SPEED of LiGhT!!!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yHai!#n' as $e SmAsHeS $s foot upwards your face at the SPEED of LiGhT!!!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yHai!#n' as $e SmAsHeS $s foot upwards $N's face at the SPEED of LiGhT!!!",ch,NULL,victim,TO_NOTVICT);
   act("The air ROARS violently as the explosive SeArS through your chest!",victim,NULL,NULL,TO_CHAR);
   act("The air ROARS violently as the explosive SeArS through $n's chest!",victim,NULL,NULL,TO_ROOM);

   if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
      dam /= 2;

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = UPPERCUT;

   hurt_person( ch, victim, dam, TRUE );
}

void do_palm_tackle( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW_WAR) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
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

   if ( victim == ch )
      {
         send_to_char( "How can you strike yourself?\n\r", ch );
         return;
      }

   if ( is_safe(ch,victim) )
      return;
 
   act( "Swirling your palms, a manifest of dark energy radiates you as you SmAsH $N's chest!", ch, NULL, victim, TO_CHAR );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS your chest!", ch, NULL, victim, TO_VICT );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS $N's chest!", ch, NULL, victim, TO_NOTVICT );
   act( "Swirling your palms, a manifest of dark energy radiates you as you SmAsH $N's chest!", ch, NULL, victim, TO_CHAR );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS your chest!", ch, NULL, victim, TO_VICT );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS $N's chest!", ch, NULL, victim, TO_NOTVICT );
   act( "Swirling your palms, a manifest of dark energy radiates you as you SmAsH $N's chest!", ch, NULL, victim, TO_CHAR );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS your chest!", ch, NULL, victim, TO_VICT );
   act( "Swirling $s palms, a manifest of dark energy radiates of $n as $e SmAsHeS $N's chest!", ch, NULL, victim, TO_NOTVICT );
   act( "You then dash towards $N grasping $S arm in a vicious lock.", ch, NULL, victim, TO_CHAR );
   act( "$n then dashes towards you grasping your arm in a vicious lock.", ch, NULL, victim, TO_VICT );
   act( "$n then dashes towards $N grasping $S arm in a vicious lock.", ch, NULL, victim, TO_NOTVICT );
   act( "Twisting $N's arm with a loud snap, you RaM your elbow into $S face!", ch, NULL, victim, TO_CHAR );
   act( "Twisting your arm with a loud snap, $n RaMs $s elbow into your face!", ch, NULL, victim, TO_VICT );
   act( "Twisting $N's arm with a loud snap, $n RaMs $s elbow into $S face!", ch, NULL, victim, TO_NOTVICT );
   act( "As $N staggers back ground the blow, you connect your foot in $S face follow by a swift strike to the waistline!", ch, NULL, victim, TO_CHAR );
   act( "As you stagger back ground the blow, $n connects $s foot in your face follow by a swift strike to the waistline!", ch, NULL, victim, TO_VICT );
   act( "As $N staggers back ground the blow, $n connects $s foot in $S face follow by a swift strike to the waistline!", ch, NULL, victim, TO_NOTVICT );

   if ( !IS_SET( victim->more, MORE_PAIN ) )
      SET_BIT( victim->more, MORE_PAIN );

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = HIT;
   ch->attack_focus = LOW;

   if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
      hurt_person( ch, victim, 1000000, TRUE );
      else hurt_person( ch, victim, 800000, TRUE );
}

void do_chi_blast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         do_assassin_blast( ch, argument );
         return;
      }

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 10 )
      {
         send_to_char( "Your mastery of the shadows aren't high enough to do this technique.\n", ch ); 
         return;
      }

   if ( ch->ctimer[0] > 0 )
      {
         send_to_char( "You are still recovering from the last one.\n", ch ); 
         return;
      }

   ch->ctimer[0] = 6;
   ch->wait = 12;

   act("Inside of you, your dark chi starts to burn in the utmost bottom of your body",ch,NULL,NULL,TO_CHAR);
   act("A dark chi radiates from $n, creating a #pghastly aura#n",ch,NULL,NULL,TO_ROOM);
   act("You exclaim '#pMUHAHAHA.....feel my wrath, feel the power of my #yCHI BLAST#p'#n",ch,NULL,NULL,TO_CHAR);
   act("$n exclaims '#pMUHAHAHA.....feel my wrath, feel the power of my #yCHI BLAST#p'#n",ch,NULL,NULL,TO_ROOM);
   act("You crouch to one knee as your body flares with unbelievable power!",ch,NULL,NULL,TO_CHAR);
   act("$n crouches to one knee as $s body flares with unbelievable power!",ch,NULL,NULL,TO_ROOM);
   act("Pebbles start jumping all around you as you focus the powerful energy within you.",ch,NULL,NULL,TO_CHAR);
   act("Pebbles start jumping all around $n as $e focuses the powerful energy within $m.",ch,NULL,NULL,TO_ROOM);
   act("You exclaim '#yCCCHHHHHHIIIIIIII!!!#n' as you open your hand releasing an energy ball!",ch,NULL,NULL,TO_CHAR);
   act("$n exclaims '#yCCCHHHHHHIIIIIIII!!!#n' as $e opens $s hand releasing an energy ball!",ch,NULL,NULL,TO_ROOM);
   act("The energy ball hovers and electricity radiates from it.",ch,NULL,NULL,TO_CHAR);
   act("The energy ball hovers and electricity radiates from it.",ch,NULL,NULL,TO_ROOM);
   act("You release the energy ball into the air, high above everyone's head.",ch,NULL,NULL,TO_CHAR);
   act("$n release the energy ball into the air, high above everyone's head.",ch,NULL,NULL,TO_ROOM);
   act("You exclaim '#yBLAAAASSSSSSSSSTTT!!!#n' as you direct your hand in a downward motion!",ch,NULL,NULL,TO_CHAR);
   act("$n exclaims '#yBLAAAASSSSSSSSSTTT!!!#n' as $e direct $s hand in a downward motion!",ch,NULL,NULL,TO_ROOM);
   act("The energy ball ROARS through the air as it ZOOMS pass you at the speed of LiGht!",ch,NULL,NULL,TO_CHAR);
   act("The energy ball ROARS through the air as it ZOOMS pass you at the speed of LiGht!",ch,NULL,NULL,TO_ROOM);
   act("The energy ball turns around as it ZOOMS into the sky!",ch,NULL,NULL,TO_CHAR);
   act("The energy ball turns around as it ZOOMS into the sky!",ch,NULL,NULL,TO_ROOM);
   act("The energy ball suddenly SmAsHeS inTo tHe GroUnD aS tHe eNtIrE ViCiniTy ERUPTS vIoLeNtLY!!!",ch,NULL,NULL,TO_CHAR);
   act("The energy ball suddenly SmAsHeS inTo tHe GroUnD aS tHe eNtIrE ViCiniTy ERUPTS vIoLeNtLY!!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
  
      if ( is_safe(ch, vch) )
         continue;

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_var = BLAST;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 500000, TRUE);
   }

   act("MaSsIvE explosions fill the air as the room bursts into flames!!",ch,NULL,NULL,TO_CHAR);
   act("MaSsIvE explosions fill the air as the room bursts into flames!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
  
      if ( is_safe(ch, vch) )
         continue;

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_FIRE;
      ch->attack_var = BLAST;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 500000, TRUE);
   }

   act("The room goes white as a #wShOcKwAvE#n RiPs through EvErYtHiNg in its path!",ch,NULL,NULL,TO_CHAR);
   act("The room goes white as a #wShOcKwAvE#n RiPs through EvErYtHiNg in its path!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
  
      if ( is_safe(ch, vch) )
         continue;

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_var = BLUNT;
      ch->attack_focus = UPPERCUT;
      hurt_person( ch, vch, 500000, TRUE);
   }
}

void do_inner_chi( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 5 )
      {
         send_to_char( "Your mastery of the shadows aren't high enough to do this technique.\n", ch ); 
         return;
      }

   if ( ch->mana < 1500000 )
      {
         send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 1500000;
   ch->agg = UMAX( 0, ch->agg - 10 );
   ch->wait = 12;
   hit_gain( ch, 300000 );

   act("You close your eyes and stir your chi within.",ch,NULL,NULL,TO_CHAR);
   act("$n closes $s eyes and you feel a spark of chi stirring.",ch,NULL,NULL,TO_ROOM);
   act("A flash of #ppure chi#n emits from you as you invert your chi inwards.",ch,NULL,NULL,TO_CHAR);
   act("A flash of #ppure chi#n emits from $n as $e inverts $s chi inwards.",ch,NULL,NULL,TO_ROOM);
   clear_affects( ch );
}

void do_raging_demon( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) )
      {
         stc( "Huh?\n\r", ch );
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
         send_to_char( "Release your true rage onto whom?\n\r", ch );
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

   act( "The sky turn dark and lightning strikes the down around you.", ch, NULL, NULL, TO_CHAR );
   act( "The sky turn dark and lightning strikes the down around you.", ch, NULL, NULL, TO_ROOM );
   act( "You yell, '#RSHUN GOKU SATSU!!!!!!!!!!.", ch, NULL, NULL, TO_CHAR );
   act( "$n yells, '#RSHUN GOKU SATSU!!!!!!!!!!.", ch, NULL, NULL, TO_ROOM );
   act( "With one foot cluch in, you slide towards $N, leaving shadow images behind you.", ch, NULL, victim, TO_CHAR );
   act( "With one foot cluch in, $n slides towards you, leaving shadow images behind $m.", ch, NULL, victim, TO_VICT );
   act( "With one foot cluch in, $n slides towards $N, leaving shadow images behind $m.", ch, NULL, victim, TO_NOTVICT );
   act( "You grab $N, and upon contact, the entire area glows with #wBright White light!!!#n", ch, NULL, victim, TO_CHAR );
   act( "$n grabs you, and upon contact, the entire area glows with #wBright White light!!!#n", ch, NULL, victim, TO_VICT );
   act( "$n grabs $N, and upon contact, the entire area glows with #wBright White light!!!#n", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "You deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_CHAR );
   act( "$n deliver a series of unseen combos and techniques into you for serveral times!", ch, NULL, victim, TO_VICT );
   act( "$n deliver a series of unseen combos and techniques into $N for serveral times!", ch, NULL, victim, TO_NOTVICT );
   act( "When the light clears, $N is laying on the ground in a bloody pult as a symbols glows on your back.", ch, NULL, victim, TO_CHAR );
   act( "When the light clears, you are laying on the ground in a bloody pult as a symbols glows on $n's back.", ch, NULL, victim, TO_VICT );
   act( "When the light clears, $N is laying on the ground in a bloody pult as a symbols glows on $n's back.", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;

   if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
      hurt_person( ch, victim, 3500000, TRUE );
      else hurt_person( ch, victim, 2500000, TRUE );
}

void do_dark_hadou( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW_WAR) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Send the dark hadou to whom?\n\r", ch );
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
 
   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You start to growl loudly as Dark energy surges out from you body blowing the dust away from your feet.", ch, NULL, NULL, TO_CHAR );
   act( "You start to shiver and slow step back as Dark energy surges out of $n blowing the dust away from $s feet.", ch, NULL, NULL, TO_ROOM );
   act( "Small dust and rock particles starts to rises and the boulders around you begins to shatter.", ch, NULL, NULL, TO_CHAR );
   act( "Small dust and rock particles starts to rises and the boulders around $n's body begins to shatter.", ch, NULL, NULL, TO_ROOM );
   act( "You drop into a Deep stance putting your hands at your side close together as energy particles starts to be suck in the vaccum of your hands.", ch, NULL, NULL, TO_CHAR );
   act( "$n drops into a Deep stance putting $s hands to $s side close together as energy particles starts to be suck in the vaccum of $s hands.", ch, NULL, NULL, TO_ROOM );
   act( "The energy particle slowly starts to form a energy ball and you concentrate more making the ball bigger.", ch, NULL, victim, TO_CHAR );
   act( "You notice more enery surges out from $n's body as well $s eyes turn #0dark #rred.", ch, NULL, NULL, TO_ROOM );
   act( "You shout out, '#RRRRRRRRRRRRRRRRRRRRAAAAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHH!!!#n'", ch, NULL, NULL, TO_CHAR );
   act( "You shout out, '#REAT THIS $N!!!!!!#n'", ch, NULL, victim, TO_CHAR );
   act( "$n shouts out, '#RRRRRRRRRRRRRRRRRRRRAAAAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHH!!!#n'", ch, NULL, NULL, TO_ROOM );
   act( "$n tells you, '#REAT THIS!!!!!!#n'", ch, NULL, victim, TO_VICT );
   act( "$n tells $N, '#REAT THIS!!!!!!#n'", ch, NULL, victim, TO_NOTVICT );
   act( "With a quick twist of your upper torso, you release the Dark Hadouken at $N.", ch, NULL, victim, TO_CHAR );
   act( "With a quick twist of $s's upper torso, $n releases the Dark Hadouken at you.", ch, NULL, victim, TO_VICT );
   act( "With a quick twist of $s's upper torso, $n releases the Dark Hadouken at $N.", ch, NULL, victim, TO_NOTVICT );
   act( "Unable to move, you take the full impact of the Fireball, sending you flying into the wall.", victim, NULL, NULL, TO_CHAR );
   act( "Unable to move, $n takes the full impact of the Fireball, sending $m flying into the wall.", victim, NULL, NULL, TO_ROOM );
   act( "You fall to your knee holding your chest while coughing out blood and gasping for air.", victim, NULL, NULL, TO_CHAR );
   act( "$n falls to $s knee holding $s chest while coughing out blood and gasping for air.", victim, NULL, NULL, TO_ROOM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;

   SET_BIT( victim->more2, MORE2_NOLEGS );
   SET_BIT( victim->more2, MORE2_NOARMS );

   if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
      hurt_person( ch, victim, 2000000, TRUE );
      else hurt_person( ch, victim, 1500000, TRUE );
}

void do_shadowswap( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   AFFECT_DATA *paf, *paf_next;

   if ( !IS_CLASS(ch, CLASS_SHADOW_WAR) && !IS_SET( ch->cmbt[5], SKL2_SSWAP ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whose affects you want to switch with?\n\r", ch );
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

   if ( ch->mana < 500000 )
      {
         send_to_char( "You don't have enough magical energy for this.\n\r", ch );
         return;
      }

   if ( victim == ch )
      {
         send_to_char( "You can't do this to yourself.\n\r", ch );
         return;
      }

   if ( is_safe(ch,victim) )
      return;

   if ( is_affected( victim,skill_lookup("divine reflection") ) )
      {
         stc("Some mystical shield bounces of your spell.\n\r",ch);
         return;
      }

   if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
      {
         stc("They are protected by the holy saints.\n\r",ch);
         return;
      }

   give_align( ch, -25 );
   WAIT_STATE(ch,12);
   ch->mana -= 500000;

   act("You point a finger at $N as the shadows rise from the ground enveloping $M.",ch,NULL,victim,TO_CHAR);
   act("$n points a finger at you as the shadows rise from the ground enveloping you.",ch,NULL,victim,TO_VICT);
   act("$n points a finger at $N as the shadows rise from the ground enveloping $M.",ch,NULL,victim,TO_NOTVICT);

   if ( IS_SET( victim->cmbt[5], SKL2_MCONTROL ) )
      {
         act("You shrug as you flick your wrist dispersing the shadows as if they never exist.",victim,NULL,NULL,TO_CHAR);
         act("$n shrugs as $e flick $s wrist dispersing the shadows as if they never exist.",victim,NULL,NULL,TO_ROOM);
         return;
      }

   act("Suddenly the shadows leap from $N's body as it engulfs you.",ch,NULL,victim,TO_CHAR);
   act("Suddenly the shadows leap from your body as it engulfs $n.",ch,NULL,victim,TO_VICT);
   act("Suddenly the shadows leap from $N's body as it engulfs $n.",ch,NULL,victim,TO_NOTVICT);
   act("Your body trembles with an unearthly power as you use $N's blessings for your own.",ch,NULL,victim,TO_CHAR);
   act("$n's body trembles with an unearthly power as $e uses your blessings for $s own.",ch,NULL,victim,TO_VICT);
   act("$n's body trembles with an unearthly power as $e uses $N's blessings for $s own.",ch,NULL,victim,TO_NOTVICT);

   for ( paf = victim->affected; paf != NULL; paf = paf_next )
   {
      bool test;

      test = FALSE;
      paf_next = paf->next;

      if ( paf->type == skill_lookup("despondency")
           || paf->type == skill_lookup("web")
           || paf->type == skill_lookup("faerie fire")
           || paf->type == skill_lookup("blindness")
           || paf->type == skill_lookup("poison")
           || paf->type == skill_lookup("age")
           || paf->type == skill_lookup("slow")
           || paf->type == skill_lookup("curse of allah")
           || paf->type == skill_lookup("chill touch")
           || paf->type == skill_lookup("curse")
           || paf->type == skill_lookup("gods disfavor")
           || paf->type == skill_lookup("drowfire")
           || paf->type == skill_lookup("weaken")
           || paf->type == skill_lookup("last prayer")
           || paf->type == skill_lookup("fear")
           || paf->type == skill_lookup("royal flush")
           || paf->type == skill_lookup("infirmity") )
         test = TRUE;

      if ( test == FALSE && paf->duration != -1 )
         {
            if ( !is_affected( ch, paf->type ) )
               {
                  paf->type -= 10000;
                  affect_to_char( ch, paf );
               }

            affect_remove( victim, paf );
         }
   }

   for ( paf = ch->affected; paf != NULL; paf = paf->next )
   {
      if ( paf->type < 0 )
         paf->type += 10000;
   }

   for ( paf = ch->affected; paf != NULL; paf = paf_next )
   {
      bool test;

      test = TRUE;
      paf_next = paf->next;

      if ( paf->type != skill_lookup("despondency")
           && paf->type != skill_lookup("web")
           && paf->type != skill_lookup("faerie fire")
           && paf->type != skill_lookup("blindness")
           && paf->type != skill_lookup("poison")
           && paf->type != skill_lookup("age")
           && paf->type != skill_lookup("slow")
           && paf->type != skill_lookup("curse of allah")
           && paf->type != skill_lookup("chill touch")
           && paf->type != skill_lookup("curse")
           && paf->type != skill_lookup("gods disfavor")
           && paf->type != skill_lookup("drowfire")
           && paf->type != skill_lookup("weaken")
           && paf->type != skill_lookup("last prayer")
           && paf->type != skill_lookup("fear")
           && paf->type != skill_lookup("royal flush")
           && paf->type != skill_lookup("infirmity") )
         test = FALSE;

      if ( test == TRUE && paf->duration != -1 )
         {
            if ( ( paf->type == skill_lookup("overdrive") && is_affected( victim, skill_lookup("subsitance") ) )
                 || ( paf->type == skill_lookup("subsitance") && is_affected( victim, skill_lookup("overdrive") ) ) )
               ;
               else if ( !is_affected( victim, paf->type ) )
               {
                  paf->type -= 10000;
                  affect_to_char( victim, paf );
               }

            affect_remove( ch, paf );
         }
   }

   for ( paf = victim->affected; paf != NULL; paf = paf->next )
   {
      if ( paf->type < 0 )
         paf->type += 10000;
   }

   if ( IS_SET(ch->more, MORE_BLIND) )
      {
         SET_BIT(victim->more, MORE_BLIND);
         ch->blind_timer = victim->blind_timer;
         victim->blind_timer = 0;
         REMOVE_BIT(ch->more, MORE_BLIND);
      }

   if ( IS_SET(ch->more, MORE_PAIN) )
      {
         SET_BIT(victim->more, MORE_PAIN);
         REMOVE_BIT(ch->more, MORE_PAIN);
      }

   if ( IS_SET(ch->more, MORE_CONFUSION) )
      {
         SET_BIT(victim->more, MORE_CONFUSION);
         REMOVE_BIT(ch->more, MORE_CONFUSION);
      }

   if ( IS_SET(ch->more, MORE_PINCH) )
      {
         SET_BIT(victim->more, MORE_PINCH);
         REMOVE_BIT(ch->more, MORE_PINCH);
      }

   if ( IS_SET( ch->more, MORE_POISON ) )
      {
         SET_BIT( victim->more, MORE_POISON );
         REMOVE_BIT( ch->more, MORE_POISON );
      }

   if ( IS_SET( ch->more2, MORE2_NOLEGS ) )
      {
         SET_BIT( victim->more2, MORE2_NOLEGS );
         REMOVE_BIT( ch->more2, MORE2_NOLEGS );
      }

   if ( IS_SET( ch->more2, MORE2_NOARMS ) )
      {
         SET_BIT( victim->more2, MORE2_NOARMS );
         REMOVE_BIT( ch->more2, MORE2_NOARMS );
      }

   if ( IS_SET( ch->more3, MORE3_CONFUSE ) )
      {
         SET_BIT( victim->more3, MORE3_CONFUSE );
         REMOVE_BIT( ch->more3, MORE3_CONFUSE );
      }

   if ( IS_SET( victim->more, MORE_BARRIER ) )
      {
         SET_BIT( ch->more, MORE_BARRIER );
         REMOVE_BIT( victim->more, MORE_BARRIER );
      }

   if ( victim->gshield > 0 )
      {
         ch->gshield = victim->gshield;
         victim->gshield = 0;
      }

   if ( IS_SET(victim->itemaffect, ITEMA_FIREWALL) )
      {
         if ( !IS_SET(ch->itemaffect, ITEMA_FIREWALL) )
            SET_BIT(ch->itemaffect, ITEMA_FIREWALL);

         REMOVE_BIT(victim->itemaffect, ITEMA_FIREWALL);
      }

   if ( IS_SET( victim->elemental, ELE_NONE ) )
      {
         REMOVE_BIT( victim->elemental, ELE_NONE );
         SET_BIT( ch->elemental, ELE_NONE );
      }

   if ( IS_CLASS( victim, CLASS_ANGEL ) && IS_CLASS( ch, CLASS_SHADOW_WAR ) && victim->ctimer[17] > 0 )
      {
         int temp = ch->ctimer[17];

         ch->ctimer[17] = victim->ctimer[17];
         victim->ctimer[17] = temp;

         if ( temp == 0 )
            do_trueform( victim, "" );
      }
}

void do_shadow_fuse( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_SHADOW_WAR ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         send_to_char("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   act( "You open your hand drawing energy from the kiotic planes.", ch, NULL, NULL, TO_CHAR );
   act( "$n opens $s hand drawing energy from the kiotic planes.", ch, NULL, NULL, TO_ROOM );
   ch->wait = 6;
   ch->mana -= 1000000;

   argument = one_argument( argument, arg );

   if ( arg[0] != '\0' && ch->fight_timer == 0 )
      {
         if ( ( vch = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }

         if ( ( get_age(vch) - 17 ) < 2 )
            return;

         if ( vch->fight_timer > 0 )
            {
               stc("Their internal energy is too great for you to currently draw upon.\n\r", ch );
               return;
            }

         act( "The shadows beneath your feet rises claiming $N to the forbidden realm.", ch, NULL, vch, TO_CHAR );
         act( "The shadows beneath $n's feet rises claiming you to the forbidden realm.", ch, NULL, vch, TO_VICT );
         act( "The shadows beneath $n's feet rises claiming $N to the forbidden realm.", ch, NULL, vch, TO_NOTVICT );

         if ( !IS_AFFECTED(vch, AFF_SHADOWPLANE) )
            {
               send_to_char("You fade into the plane of shadows.\n\r",vch);
               act("The shadows flicker and swallow up $n.",vch,NULL,NULL,TO_ROOM);
               SET_BIT(vch->affected_by, AFF_SHADOWPLANE);
               vch->wait += 12;
            }

         return;
      }

   act( "The shadows beneath your feet rises claiming all to the forbidden realm.", ch, NULL, NULL, TO_CHAR );
   act( "The shadows beneath $n's feet rises claiming all to the forbidden realm.", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
       vch_next = vch->next_in_room;

       if ( ( get_age(vch) - 17 ) < 2 || vch->fight_timer > 0 )
          continue;

       if ( !IS_AFFECTED(vch, AFF_SHADOWPLANE) )
          {
             send_to_char("You fade into the plane of shadows.\n\r",vch);
             act("The shadows flicker and swallow up $n.",vch,NULL,NULL,TO_ROOM);
             SET_BIT(vch->affected_by, AFF_SHADOWPLANE);
             vch->wait += 12;
          }
    }
}
