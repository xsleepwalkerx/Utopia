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

void do_hobby_armor( CHAR_DATA *ch, char *argument )
{
   char arg [MAX_INPUT_LENGTH];
   int cost, vnum;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   cost = 1;

   argument = one_argument( argument, arg );

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_CLASS(ch, CLASS_HOBBIT) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Syntax: hobbyarmor <item>\n\r", ch );
         send_to_char( "items are: belt (1), robe (1), sandals (1), cloak (1), ring (1)\n\r", ch );
         return;
      }

   if ( !str_cmp(arg,"belt") )
      vnum = 433;
      else if ( !str_cmp(arg,"robe") )
              vnum = 434;
      else if ( !str_cmp(arg,"sandals") )
              vnum = 435;
      else if ( !str_cmp(arg,"cloak") )
              vnum = 436;
      else if ( !str_cmp(arg,"ring") )
              vnum = 437;
              else {
                      do_hobby_armor(ch,"");
                      return;
                   }

   if ( cost > ch->gold )
      {
         sprintf(arg,"It costs you #w%d #ygold piece#n to make this item.\n\r", cost);
         send_to_char(arg,ch);
         return;
      }

   ch->gold -= cost;

   if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
      {
         send_to_char("Missing object, please inform Gogeta.\n\r",ch);
         return;
      }

   obj = create_object(pObjIndex, 60);
   obj_to_char(obj, ch);
   act("You solemnly take $p out of a backpack.",ch,obj,NULL,TO_CHAR);
   act("$n solemnly takes $p out of a backpack.",ch,obj,NULL,TO_ROOM);
}

bool is_oring( CHAR_DATA *ch )
{
   OBJ_DATA *obj;

   if ( ( ( obj = get_eq_char( ch, WEAR_FINGER_L ) ) == NULL ) || obj->pIndexData->vnum != 437 )
      if ( ( ( obj = get_eq_char( ch, WEAR_FINGER_R ) ) == NULL ) || obj->pIndexData->vnum != 437 )
         return FALSE;

   return TRUE;
}

void do_holy( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == TRUE )
      {
         send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to annihilate?\n\r", ch );
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
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   act("Your hands pulses with holy energy as you leap swiftly in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n's hands pulses with holy energy as $e leaps swiftly in the air.",ch,NULL,NULL,TO_ROOM);
   act("As you land back on the ground, you suddenly SmAsH your fist into it MiGhTilY!!!",ch,NULL,NULL,TO_CHAR);
   act("As $n lands back on the ground, $e suddenly SmAsHeS $s fist into it MiGhTilY!!!",ch,NULL,NULL,TO_ROOM);
   act("A blast of divine energy shoots in the air as the entire room starts swirling.",ch,NULL,NULL,TO_CHAR);
   act("A blast of divine energy shoots in the air as the entire room starts swirling.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, everything begins to age younger as mystical comets encircle you.",victim,NULL,NULL,TO_CHAR);
   act("Suddenly, everything begins to age younger as mystical comets encircle $n.",victim,NULL,NULL,TO_ROOM);
   act("A loud chorus then fills the air as you are engulfed by a beam from the heavens!",victim,NULL,NULL,TO_CHAR);
   act("A loud chorus then fills the air as $n is engulfed by a beam from the heavens!",victim,NULL,NULL,TO_ROOM);
   act("A comet then CrAsHeS directly into you as you ScReAm in agony!",victim,NULL,NULL,TO_CHAR);
   act("A comet then CrAsHeS directly into $n as $e ScReAmS in agony!",victim,NULL,NULL,TO_ROOM);
   act("You fall onto your knees as the explosions reach a new height of pain.",victim,NULL,NULL,TO_CHAR);
   act("$n falls onto $s knees as the explosions reach a new height of pain.",victim,NULL,NULL,TO_ROOM);
   act("A soft humming floats through the air as the room explodes with HoLiOuS PoWeR!!!",victim,NULL,NULL,TO_CHAR);
   act("A soft humming floats through the air as the room explodes with HoLiOuS PoWeR!!!",victim,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_var = SOUL;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 3500000, TRUE );
}

void do_flare( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You sneer '#yHmph...#n' as you tilt your petite hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n sneers '#yHmph...#n' as $e tilts $s petite hands together.",ch,NULL,NULL,TO_ROOM);
   act("The #eone #wring#n crackles with electricity as a mass of energy manifests between your hands.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as a mass of energy manifests between $n's hands.",ch,NULL,NULL,TO_ROOM);
   act("You then push your palm forward as the energy ball floats towards the centre of the room.",ch,NULL,NULL,TO_CHAR);
   act("$n then pushes $s palm forward as the energy ball floats towards the centre of the room.",ch,NULL,NULL,TO_ROOM);
   act("The ball starts pulsing violently as your eyes radiates the intense desire to kill.",ch,NULL,NULL,TO_CHAR);
   act("The ball starts pulsing violently as $n's eyes radiates the intense desire to kill.",ch,NULL,NULL,TO_ROOM);
   act("You sneer '#yJust Die!#n' as you fold your arms while turning around to the distance.",ch,NULL,NULL,TO_CHAR);
   act("$n sneers '#yJust Die!#n' as $e folds $s arms while turning around to the distance.",ch,NULL,NULL,TO_ROOM);
   act("A MaSsIvE explosion fills the air as the energy ball BuRsTs into dark ferocious flames!",ch,NULL,NULL,TO_CHAR);
   act("A MaSsIvE explosion fills the air as the energy ball BuRsTs into dark ferocious flames!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("A spiral of flames RiP through your body SlAmMiNg you HARD into the GrOuNd!!!#n",vch,NULL,NULL,TO_CHAR);
      act("A spiral of flames RiP through $n's body SlAmMiNg $m HARD into the GrOuNd!!!#n",vch,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = BLAST;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 2000000, TRUE );
   }

   act("Just as the flames tone down, the energy ball suddenly emits out a loud humming.",ch,NULL,NULL,TO_CHAR);
   act("Just as the flames tone down, the energy ball suddenly emits out a loud humming.",ch,NULL,NULL,TO_ROOM);
   act("You let out a soft, evil smirk as the room ExPlOdEs with DeMoNiC PoWeR!!!",ch,NULL,NULL,TO_CHAR);
   act("$n lets out a soft, evil smirk as the room ExPlOdEs with DeMoNiC PoWeR!!!",ch,NULL,NULL,TO_ROOM);
}   

void do_shadow_dancer( CHAR_DATA *ch, char *argument )
{
   ROOM_INDEX_DATA *location;
   bool test = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 )
      {
         send_to_char( "Not in the arena.\n\r", ch );
         return;
      }

   if ( IS_CLASS( ch, CLASS_HOBBIT ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc( "The Corrupt forces refuse to let you use this mystical ability.\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   if ( ch->pcdata->kingdom == 1 && ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000 )
      {
         stc("You don't see a need of shadow dancing within your own base.\n\r",ch);
         return;
      }

   if ( ch->pcdata->kingdom == 2 && ch->in_room->vnum >= 3500 && ch->in_room->vnum < 4000 )
      {
         stc("You don't see a need of shadow dancing within your own base.\n\r",ch);
         return;
      }

   if ( ch->pcdata->kingdom == 3 && ch->in_room->vnum >= 4500 && ch->in_room->vnum < 5000 )
      {
         stc("You don't see a need of shadow dancing within your own base.\n\r",ch);
         return;
      }

   if ( ch->pcdata->kingdom == 4 && ch->in_room->vnum >= 16500 && ch->in_room->vnum < 17000 )
      {
         stc("You don't see a need of shadow dancing within your own base.\n\r",ch);
         return;
      }

   if ( ch->ctimer[1] > 0 )
      {
         stc( "You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->fighting == NULL )
      {
         stc( "But you aren't in the heat of battle.\n\r", ch );
         return;
      }

   if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
      {
         stc( "This dance is forbbiden within the Shadowy planes.\n\r", ch );
         return;
      }

   if ( ch->move < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   ch->move -= 1000000;
   ch->wait = 12;
   ch->ctimer[1] = 4;

   act("You laugh loudly as you start moving your body in an ancient swaying motion.",ch,NULL,NULL,TO_CHAR);
   act("$n laughs loudly as $e starts moving $s body in an ancient swaying motion.",ch,NULL,NULL,TO_ROOM);
   act("As you twirl and leap across the room, the shadows beneath your feet suddenly rise.",ch,NULL,NULL,TO_CHAR);
   act("As $n twirls and leaps across the room, the shadows beneath $s feet suddenly rise.",ch,NULL,NULL,TO_ROOM);
   act("Grabbing the shadows like a cloak, you drape it across your body as you twirl one last time.",ch,NULL,NULL,TO_CHAR);
   act("Grabbing the shadows like a cloak, $n drapes it across $s body as $e twirls one last time.",ch,NULL,NULL,TO_ROOM);
   act("The #eone #wring#n crackles with electricity as you suddenly fade away.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as $n suddenly fades away.",ch,NULL,NULL,TO_ROOM);

   stop_fighting( ch, TRUE );
   get_random_room( ch );
   do_look( ch, "auto" );

   if ( IS_SET(ch->newbits, NEW_DARKNESS) )
      {
         REMOVE_BIT(ch->newbits, NEW_DARKNESS);

         if ( ch->in_room->drows > 0 )
            {
               ch->in_room->drows--;

               if ( ch->in_room->drows == 0 && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
                  REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
            }
      }

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      REMOVE_BIT(ch->act, PLR_WIZINVIS);

   if ( number_percent() > 85 )
      SET_BIT( ch->affected_by, AFF_SHADOWPLANE );
}

void do_demon_eye( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to hurt badly?\n\r", ch );
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

   if ( IS_SET( victim->more, MORE_PAIN ) && IS_SET( victim->monkstuff, MONK_DEATH ) )
      {
         stc("They are already in pain!\n\r",ch);
         return;
      }

   ch->mana -= 250000;
   act("You fold your arms as you glare at $N extremely coldly.",ch,NULL,victim,TO_CHAR);
   act("$n folds $s arms as $e glares at you extremely coldly.",ch,NULL,victim,TO_VICT);
   act("$n folds $s arms as $e glares at $N extremely coldly.",ch,NULL,victim,TO_NOTVICT);
   act("The #eone #wring#n crackles with electricity as your eyes pulses with dark energy.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as $n's eyes pulses with dark energy.",ch,NULL,NULL,TO_ROOM);
   act("Mystical forces suddenly rip through $N's body as you gaze through $S soul.",ch,NULL,victim,TO_CHAR);
   act("Mystical forces suddenly rip through your body as $n gazes through your soul.",ch,NULL,victim,TO_VICT);
   act("Mystical forces suddenly rip through $N's body as $n gazes through $S soul.",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_SET( victim->more, MORE_PAIN ) )
      SET_BIT( victim->more, MORE_PAIN );

   if ( !IS_SET( victim->monkstuff, MONK_DEATH ) )
      SET_BIT( victim->monkstuff, MONK_DEATH );

   ch->wait = 12;
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;

   hurt_person( ch, victim, 700000, TRUE );
}

void do_shadow_demon( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         act("The shadows engulfing you suddenly fades away into the night.",ch,NULL,NULL,TO_CHAR);
         act("The shadows engulfing $n suddenly fades away into the night.",ch,NULL,NULL,TO_ROOM);

         free_string( ch->morph );
         ch->morph = str_dup( "" );

         REMOVE_BIT( ch->affected_by, AFF_POLYMORPH );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 300000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   ch->mana -= 300000;
   act("Your facial expressions darken as you fall to the ground in pain.",ch,NULL,NULL,TO_CHAR);
   act("$n's facial expressions darken as $e falls to the ground in pain.",ch,NULL,NULL,TO_ROOM);
   act("The #eone #wring#n crackles with electricity as the colour drains from your skin.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as the colour drains from $n's skin.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, the shadows beneath your feet rise up and engulf your petite body.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, the shadows beneath $n's feet rise up and engulf $s petite body.",ch,NULL,NULL,TO_ROOM);

   sprintf( buf, "#ea Shadowy Figure#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   SET_BIT( ch->affected_by, AFF_POLYMORPH );

   act("Feeling the demonic powers flowing through you, your eyes burn with intense rage.",ch,NULL,NULL,TO_CHAR);
   act("Feeling the demonic powers flowing through $m, $n's eyes burn with intense rage.",ch,NULL,NULL,TO_ROOM);
}

void do_daintly_dark( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You wrap your cloak around your face as your eyes darken with rage.",ch,NULL,NULL,TO_CHAR);
   act("$n wraps $s cloak around $s face as $s eyes darken with rage.",ch,NULL,NULL,TO_ROOM);
   act("You sneer '#yFollowers of the Light, disperse or Die!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n sneers '#yFollowers of the Light, disperse or Die!#n'",ch,NULL,NULL,TO_ROOM);
   act("The #eone #wring#n crackles with electricity as you clench your fist angrilly.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as $n clenches $s fist angrilly.",ch,NULL,NULL,TO_ROOM);
   act("The sun aboves you suddenly sets as everything around you blackens out.",ch,NULL,NULL,TO_CHAR);
   act("The sun aboves you suddenly sets as everything around you blackens out.",ch,NULL,NULL,TO_ROOM);

   if ( !IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
      SET_BIT( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      if ( !IS_SET( vch->more, MORE_BLIND ) )
         {
            SET_BIT( vch->more, MORE_BLIND );
            vch->blind_timer = 4;
         }

      act("An unseen force SlAsHeS vIoLeNtLy at you as your BlOoD splatters all over the ground!",vch,NULL,NULL,TO_CHAR);
      act("An unseen force SlAsHeS vIoLeNtLy at $n as $s BlOoD splatters all over the ground!",vch,NULL,NULL,TO_ROOM);
      vch->in_room->blood = UMIN( 1000, vch->in_room->blood + 100 );
      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SLASH;
      ch->attack_focus = LOW;
      hurt_person( ch, vch, 1000000, TRUE );
   }
}

void do_cats_eye( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 750000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   ch->mana -= 750000;
   ch->wait = 12;

   act("Your eyes shimmer with intense evil as you suddenly leap high into air.",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes shimmer with intense evil as $e suddenly leaps high into air.",ch,NULL,NULL,TO_ROOM);
   act("The #eone #wring#n crackles with electricity as palish energy fades from your hands.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as palish energy fades from $n's hands.",ch,NULL,NULL,TO_ROOM);
   act("Your fingers transform into talons as you swipe across the air viciously.",ch,NULL,NULL,TO_CHAR);
   act("$n's fingers transform into talons as $e swipes across the air viciously.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("A flash of light SlAsHeS vIoLeNtLy at you as your innards feel ripped apart!",vch,NULL,NULL,TO_CHAR);
      act("An unseen force SlAsHeS vIoLeNtLy at $n as $s BlOoD splatters all over the ground!",vch,NULL,NULL,TO_ROOM);
      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SLASH;
      ch->attack_focus = UPPERCUT;
      hurt_person( ch, vch, 600000, TRUE );
   }

   act("Your talons shatter like glass as you land back silently on the ground.",ch,NULL,NULL,TO_CHAR);
   act("$n's talons shatter like glass as $e lands back silently on the ground.",ch,NULL,NULL,TO_ROOM);
}

void do_dark_hell( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int dam = 700000;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to burn with the eternal flames?\n\r", ch );
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

   ch->mana -= 250000;
   act("You stare at $N coldly as you raise a quivering finger at $M.",ch,NULL,victim,TO_CHAR);
   act("$n stares at you coldly as $e raises a quivering finger at you.",ch,NULL,victim,TO_VICT);
   act("$n stares at $N coldly as $e raises a quivering finger at $M.",ch,NULL,victim,TO_NOTVICT);
   act("The #eone #wring#n crackles with electricity as it hums with demonic energy.",ch,NULL,NULL,TO_CHAR);
   act("The #eone #wring#n crackles with electricity as it hums with demonic energy.",ch,NULL,NULL,TO_ROOM);
   act("You then fire an intense beam at $N as it sizzles through $S defenses.",ch,NULL,victim,TO_CHAR);
   act("$n then fires an intense beam at you as it sizzles through your defenses.",ch,NULL,victim,TO_VICT);
   act("$n then fires an intense beam at $N as it sizzles through $S defenses.",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
      {
         dam += 100000;
         SET_BIT( victim->affected_by, AFF_FLAMING );
      }

   if ( ch->in_room != NULL && ch->in_room->vnum >= 700 && ch->in_room->vnum <= 711 )
      dam += 250000;

   ch->wait = 12;
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = BEAM;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_grand_love( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == TRUE )
      {
         send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      victim = ch;
      else if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
      }

   ch->wait = 12;

   if ( victim == ch )
      {
         act("You swoon as you hug yourself tightly, showing all the true meaning of love.",ch,NULL,NULL,TO_CHAR);
         act("$n swoons as $e hugs $mself tightly, showing all the true meaning of love.",ch,NULL,NULL,TO_ROOM);
         act("A gentle light suddenly shines down upon you from the heavens as your wounds vanish.",ch,NULL,NULL,TO_CHAR);
         act("A gentle light suddenly shines down upon $m from the heavens as $s wounds vanish.",ch,NULL,NULL,TO_ROOM);
         hit_gain( ch, 300000 );
         mana_gain( ch, 300000 );
         return;
      }

   if ( !can_heal( ch, victim ) )
      return;
   
   act("You giggle '#yWheee!#n' as you prance playfully towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n giggles '#yWheee!#n' as $e prances playfully towards you!",ch,NULL,victim,TO_VICT);
   act("$n giggles '#yWheee!#n' as $e prances playfully towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("Giving $N a big hug, you tickle $S nose saying '#yGod loves all you know.#n'",ch,NULL,victim,TO_CHAR);
   act("Giving you a big hug, $n tickles your nose saying '#yGod loves all you know.#n'",ch,NULL,victim,TO_VICT);
   act("Giving $N a big hug, $n tickles $S nose saying '#yGod loves all you know.#n'",ch,NULL,victim,TO_NOTVICT);
   act("A gentle light suddenly shines down upon you from the heavens as your wounds vanish.",victim,NULL,NULL,TO_CHAR);
   act("A gentle light suddenly shines down upon $m from the heavens as $s wounds vanish.",victim,NULL,NULL,TO_ROOM);

   hit_gain( victim, 500000 );
   move_gain( victim, 500000 );
   mana_gain( victim, 500000 );
}

void do_tilly_mint( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == TRUE )
      {
         send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 750000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   ch->mana -= 750000;
   ch->wait = 12;

   act("You flutter your eyes playfully as you roll over the ground.",ch,NULL,NULL,TO_CHAR);
   act("$n flutters $s eyes playfully as $e rolls over the ground.",ch,NULL,NULL,TO_ROOM);
   act("Taking out a lollipop, you start sucking on it cutely while opening your eyes widely.",ch,NULL,NULL,TO_CHAR);
   act("Taking out a lollipop, $n starts sucking on it cutely while opening $s eyes widely.",ch,NULL,NULL,TO_ROOM);
   act("You then blow up a MaSsIvE bubble letting it POP loudly thoughout the room.",ch,NULL,NULL,TO_CHAR);
   act("$n then blows up a MaSsIvE bubble letting it POP loudly thoughout the room.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("The blast of air sent you flying into the ground as you feel bones crunching!",vch,NULL,NULL,TO_CHAR);
      act("The blast of air sent $n flying into the ground as $e feels bones crunching!",vch,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_HOLY;
      ch->attack_var = BLAST;
      ch->attack_focus = OVERHEAD;
      hurt_person( ch, vch, 600000, TRUE );
   }

   act("You look around shyly as you whimper '#yDid I do that?#n'",ch,NULL,NULL,TO_CHAR);
   act("$n looks around shyly as $e whimpers '#yDid I do that?#n'",ch,NULL,NULL,TO_ROOM);
}

void do_truthful( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int dam = 700000;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == TRUE )
      {
         send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to insult?\n\r", ch );
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

   ch->mana -= 250000;
   act("You calmly walk to a surprised $N as you place your arms around $S shoulders.",ch,NULL,victim,TO_CHAR);
   act("$n calmly walks to a surprised you as $e places $s arms around your shoulders.",ch,NULL,victim,TO_VICT);
   act("$n calmly walks to a surprised $N as $e places $s arms around $S shoulders.",ch,NULL,victim,TO_NOTVICT);
   act("You say '#yDon't feel bad for your actions. This was ordeal from the beginning of times.#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yDon't feel bad for your actions. This was ordeal from the beginning of times.#n'",ch,NULL,NULL,TO_ROOM);
   act("You continue reciting to $N as $S face turns red in shame.",ch,NULL,victim,TO_CHAR);
   act("$n continues reciting to you as your face turns red in shame.",ch,NULL,victim,TO_VICT);
   act("$n continues reciting to $N as $S face turns red in shame.",ch,NULL,victim,TO_NOTVICT);
   act("Unable to take it anymore, you scream loudly as you knock yourself out.",victim,NULL,NULL,TO_CHAR);
   act("Unable to take it anymore, $n screams loudly as $e knocks $mself out.",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 75 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more, MORE_HEX ) )   
      {
         SET_BIT( victim->more, MORE_HEX );
         dam += 100000;
      }

   ch->wait = 12;
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_loath_demon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int dam = 700000;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( is_oring( ch ) == TRUE )
      {
         send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to detect?\n\r", ch );
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
   ch->mana -= 250000;
   act("You place a weird device over your eye as you focus intently on $N.",ch,NULL,victim,TO_CHAR);
   act("$n place a weird device over $s eye as $e focuses intently on you.",ch,NULL,victim,TO_VICT);
   act("$n place a weird device over $s eye as $e focuses intently on $N.",ch,NULL,victim,TO_NOTVICT);
   act("Some strange symbols fill the read out as the device flashes alot.",ch,NULL,NULL,TO_CHAR);
   act("Some strange symbols fill the read out as the device flashes alot.",ch,NULL,NULL,TO_ROOM);

   if ( victim->alignment <= -200 && !IS_SET( victim->more, MORE_CONFUSION ) )   
      {
         act("A soft voice says '#yConfirmed...#n' as you suddenly stagger from an invisible force.",victim,NULL,NULL,TO_CHAR);
         act("A soft voice says '#yConfirmed...#n' as $n suddenly staggers from an invisible force.",victim,NULL,NULL,TO_ROOM);
         SET_BIT( victim->more, MORE_CONFUSION );
         ch->attack_type = ATTACK_MAGICAL;
         ch->ele_attack = ATTACK_HOLY;
         ch->attack_var = BLAST;
         ch->attack_focus = DIRECT;
         hurt_person( ch, victim, dam, TRUE );
         return;
      }

   act("A soft voice says '#yNegative...#n' as the device automatically shuts off.",victim,NULL,NULL,TO_CHAR);
   act("A soft voice says '#yNegative...#n' as the device automatically shuts off.",victim,NULL,NULL,TO_ROOM);
}

void do_famed_archangel( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->more3, MORE3_HOUSE ) )
      {
         act("You sulken as you no longer feels God's presence beside you.",ch,NULL,NULL,TO_CHAR);
         act("$n sulkens as $e no longer feels God's presence beside $m.",ch,NULL,NULL,TO_ROOM);
         REMOVE_BIT( ch->more3, MORE3_HOUSE );
         return;
      }

   if ( is_oring( ch ) == TRUE )
      {
         send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
         return;
      }

   if ( ch->move < 1000000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   ch->move -= 1000000;
   SET_BIT( ch->more3, MORE3_HOUSE );
   act("You go on your knees as you pray to God for assistance.",ch,NULL,NULL,TO_CHAR);
   act("$n goes on $s knees as $e prays to God for assistance.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, tears of joy fills your eyes as a blurry winged figure hugs you from behind.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, tears of joy fills $n's eyes as a blurry winged figure hugs $m from behind.",ch,NULL,NULL,TO_ROOM);
   act("Feeling protected, you stand up proudly to fight evil once more.",ch,NULL,NULL,TO_CHAR);
   act("Feeling protected, $n stands up proudly to fight evil once more.",ch,NULL,NULL,TO_ROOM);
}

void do_crystal_dreams( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 5 )
      {
         stc( "You don't have enough mastery over the ring as yet to execute this technique.\n\r", ch );
         return;
      }

   if ( ch->ctimer[3] > 0 )
      {
         stc( "You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to detect?\n\r", ch );
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

   ch->mana -= 1000000;
   ch->ctimer[3] = 6;
   act("You goes on your knees as you carefully pull out a crystal harp.",ch,NULL,NULL,TO_CHAR);
   act("$n goes on $s knees as $e carefully pulls out a crystal harp.",ch,NULL,NULL,TO_ROOM);
   act("You start playing very soothing music of a long forgotten melody while smiling happily.",ch,NULL,NULL,TO_CHAR);
   act("$n starts playing very soothing music of a long forgotten melody while smiling happily.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == victim || is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("Your eyes become droopy as you drop down your weapons and curl up to sleep peacefully.",vch,NULL,NULL,TO_CHAR);
      act("$n eyes become droopy as $e drops down $s weapons and curls up to sleep peacefully.",vch,NULL,NULL,TO_ROOM);
      stop_fighting( vch, TRUE );
      vch->position = POS_SLEEPING;
      vch->agg = 100;
      vch->wait = 24;
   }

   act("Just as $N starts falling asleep too, you let out a soft reassuring smirk.",ch,NULL,victim,TO_CHAR);
   act("Just as you start falling asleep too, $n lets out a soft reassuring smirk.",ch,NULL,victim,TO_VICT);
   act("Just as $N starts falling asleep too, $n lets out a soft reassuring smirk.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly snap a string as $N screams loudly in a world of unimaginable nightmares!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly snap a string as you scream loudly in a world of unimaginable nightmares!",ch,NULL,victim,TO_VICT);
   act("$n suddenly snap a string as $N screams loudly in a world of unimaginable nightmares!",ch,NULL,victim,TO_NOTVICT);
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_var = SOUL;
   ch->attack_focus = LOW;
   hurt_person( ch, victim, 500000, TRUE );

   if ( victim != NULL && victim->in_room != NULL && victim->position > POS_STUNNED )
      {
         stop_fighting( victim, TRUE );
         victim->position = POS_SLEEPING;
         victim->wait = 24;
      }
}

void do_hobbit_cocoon( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) || ch->pcdata->legend < 3 )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->more2, MORE2_DIG ) )
      {
         act("You burst from beneath the ground as you gasp loudly for breath.",ch,NULL,NULL,TO_CHAR);
         act("$n bursts from beneath the ground as $e gasps loudly for breath.",ch,NULL,NULL,TO_ROOM);

         REMOVE_BIT( ch->more2, MORE2_DIG );
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc("You can't hide like this while having pursuers.\n\r",ch);
         return;
      }

   SET_BIT( ch->more2, MORE2_DIG );
   act("You quickly dig yourself into the ground as you cover up all traces.",ch,NULL,NULL,TO_CHAR);
   act("$n quickly digs $mself into the ground as $e covers up all traces.",ch,NULL,NULL,TO_ROOM);
}

void do_hidden_destiny( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
    
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) || ch->pcdata->legend < 3 )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->more2, MORE2_DIG ) )
      {
         send_to_char( "You need to be fully hidden to make use of this skill.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to surprised?\n\r", ch );
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

   ch->wait = 12;
   REMOVE_BIT( ch->more2, MORE2_DIG );
   act("You yell '#yBANZAI!!#n' as you burst from the ground into a fiery blast!",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yBANZAI!!#n' as $e bursts from the ground into a fiery blast!",ch,NULL,NULL,TO_ROOM);
   act("Before $N realizes what is going on, you catapult yourself straight into $M!",ch,NULL,victim,TO_CHAR);
   act("Before you realize what is going on, $n catapults $mself straight into you!",ch,NULL,victim,TO_VICT);
   act("Before $N realizes what is going on, $n catapults $mself straight into $M!",ch,NULL,victim,TO_NOTVICT);
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_frogget( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int count;
    
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 15 )
      {
         stc( "You don't have enough mastery over the ring as yet to execute this technique.\n\r", ch );
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         stc( "You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->move < 2000000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to make look stupid?\n\r", ch );
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

   ch->move -= 2000000;
   ch->wait = 6;
   ch->ctimer[4] = 12;
   act("You giggle '#yPOI!#n' as you suddenly touch $N going into a fit.",ch,NULL,victim,TO_CHAR);
   act("$n giggles '#yPOI!#n' as $e suddenly touches you going into a fit.",ch,NULL,victim,TO_VICT);
   act("$n giggles '#yPOI!#n' as $e suddenly touches $N going into a fit.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly start hopping on all four going '#yRibbit #oRibbit!#n'",victim,NULL,NULL,TO_CHAR);
   act("$n suddenly starts hopping on all four going '#yRibbit #oRibbit!#n'",victim,NULL,NULL,TO_ROOM);
   act("You roll on the ground laughing as you succeeded in making $N looks the dullard.",ch,NULL,victim,TO_CHAR);
   act("$n rolls on the ground laughing as $e succeeded in making you look the dullard.",ch,NULL,victim,TO_VICT);
   act("$n rolls on the ground laughing as $e succeeded in making $N looks the dullard.",ch,NULL,victim,TO_NOTVICT);

   for ( count = 0; count < 18; count++ )
   {
      if ( count == 9 )
         continue;

      if ( number_percent() > 74 )
         victim->ctimer[count] = 6;
   }
}

void do_wknights( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    int count, max_count = 3;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS( ch, CLASS_HOBBIT ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    if ( ch->pkill < 20 )
       {
          stc( "You don't have enough mastery over the ring as yet to execute this technique.\n\r", ch );
          return;
       }

    if ( ch->ctimer[6] > 0 )
       {
          stc( "You are still recovering from the last one.\n\r", ch );
          return;
       }

    if ( ch->pcdata->followers > 4 )
       {
          send_to_char("You already have too many people around you lord ringholder.\n\r",ch);
          return;
       }

    if ( ch->fighting == NULL )
       {
	    send_to_char("But you aren't in a fight.\n\r",ch);
          return;
       }

    if ( ch->mana < 2000000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    ch->mana -= 2000000;
    ch->ctimer[6] = 8;

    act( "You fall to your knees saying '#yKnights of the Shadow World, I need your help.#n'", ch, NULL, NULL, TO_CHAR );
    act( "$n falls to $s knees saying '#yKnights of the Shadow World, I need your help.#n'", ch, NULL, NULL, TO_ROOM );
    act( "The ground explodes with ferocious energy as a haunting mist fills the room.", ch, NULL, NULL, TO_CHAR );
    act( "The ground explodes with ferocious energy as a haunting mist fills the room.", ch, NULL, NULL, TO_ROOM );
    
    for ( count = 0; count < max_count; count++ )
    {
       victim = create_mobile( get_mob_index( 29 ) );
       victim->sex = ch->sex;
       victim->level = 999;
       victim->armor = -40000;
       victim->hitroll = 3000000;
       victim->damroll = 3000000;
       victim->hit = victim->max_hit   = 10000000;
       victim->mana = victim->max_mana = 10000000;
       victim->move = victim->max_move = 10000000;

       af.type      = skill_lookup("haste");
       af.duration  = -1;
       af.location  = APPLY_NONE;
       af.modifier  = 0;
       af.bitvector = 0;
       affect_to_char( victim, &af );
       char_to_room( victim, ch->in_room );
       add_follower( victim, ch );

       if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
          SET_BIT( victim->affected_by, AFF_ETHEREAL );

       if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
          SET_BIT( victim->affected_by, AFF_SHADOWPLANE );

       SET_BIT( victim->act, ACT_NINJA );
       SET_BIT( victim->act, ACT_TIMER );
       victim->practice = 2;
       ch->pcdata->followers++;

       switch( count )
       {
          default:
          case 0: act("$n exclaims '#yProtect the Ring Holder!#n' as he raises his sword and goes for the attack!",victim,NULL,NULL,TO_ROOM);
                  break;
          case 1: act("Brandishing $s axe, $n exclaims '#yDie Enemy!!#n' as he goes for the attack!",victim,NULL,NULL,TO_ROOM);
                  break;
          case 2: act("Charging to his target, $n exclaims '#y$N needs our help!'",victim,NULL,ch,TO_ROOM);
                  break;
       }

       set_fighting( victim, ch->fighting );
    }
}
