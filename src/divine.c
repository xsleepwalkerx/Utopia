/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
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

void do_charge( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  sh_int vnum;
  int dam = 1000000;
  int move = 2000000;

  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS( ch, CLASS_PALADIN ) || IS_SET( ch->special, SPC_SWORD ) )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->pkill < 10 )
     { 
        stc("#yYour #Cfaith #win the #yLight #wisn't strong enough to do this technique.#n\n\r",ch);
        return;
     }

  if ( ch->fight_timer > 0 )
     {
        stc( "#wHonour dictates for you to use this as an #Cinitiating attack #yknight#w.#n\n\r", ch );
        return;
     }

  argument = one_argument( argument, arg );

  if ( ch->move < move )
     { 
        stc("You are too tired to use this skill.\n\r",ch);
        return;
     }

  vnum = ch->in_room->vnum; 

  if      (!str_cmp(arg,"n") || !str_cmp(arg,"north")) do_north(ch,"");
    else if (!str_cmp(arg,"s") || !str_cmp(arg,"south")) do_south(ch,"");
    else if (!str_cmp(arg,"e") || !str_cmp(arg,"east" )) do_east(ch,"");
    else if (!str_cmp(arg,"w") || !str_cmp(arg,"west" )) do_west(ch,"");
    else
    {
	send_to_char("Do you wish to charge north, south, east, or west?\n\r",ch);
	return;
    }

  if ( vnum == ch->in_room->vnum )
     {
        stc("You can't charge into that direction.\n\r",ch);
        return;
     }

  ch->move -= move; 
  ch->wait = 8;

  act("You rush quickly into the room.",ch,NULL,NULL,TO_CHAR); 

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;
       
        if ( !is_safe(ch, vch) )
	     {
              if ( !str_cmp( "P", vch->groove ) || !str_cmp( "M", vch->groove ) )
                 {
                    act( "As you charge towards $N, $E violently pushes you aside.", ch, NULL, vch, TO_CHAR );
                    act( "As $n charges towards you, you violently push $m aside.", ch, NULL, vch, TO_VICT );
                    act( "As $n charges towards $N, $E violently pushes $m aside.", ch, NULL, vch, TO_NOTVICT );
                    continue;
                 }

              act("You shoulder charge into $N knocking $M onto the ground.",ch,NULL,vch,TO_CHAR);
              act("$n shoulder charge into you knocking you right off your feet.",ch,NULL,vch,TO_VICT);
              act("$n shoulder charge into $N knocking $M onto the ground.",ch,NULL,vch,TO_NOTVICT);

              hurt_person(ch,vch,dam,TRUE);

              if ( vch == NULL || vch->position <= POS_STUNNED )
                 continue;

              if ( !IS_NPC(vch) && is_martial( vch ) )
                 {
                    act("You leap back onto your feet unscratched.",vch,NULL,NULL,TO_CHAR);
                    act("$n leaps back onto $s feet unscratched.",vch,NULL,NULL,TO_ROOM);
                    continue;
                 }

              vch->wait += number_range( 12, 24 );

              if ( number_percent() > 60 && vch->position > POS_STUNNED )
                 vch->position = POS_STUNNED;
	     }
    }
}

void do_guard( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DIVINE_KNIGHT ) || IS_SET( ch->special, SPC_SWORD ) )
      if ( !IS_ITEMAFF( ch, ITEMA_SHIELD ) )
      {
         stc( "But you aren't using the #bIllumina Shield#n.\n\r", ch );
         return;
      }

   if ( ch->position != POS_FIGHTING )
      {
         send_to_char("But you aren't in a fight.\n\r",ch);
         return;
      }

   if ( ch->divine_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more2, MORE2_GUARD ) )
      {
         stc("You are already in defensive mode.\n\r",ch);
         return;
      }

   SET_BIT( ch->more2, MORE2_GUARD );
   ch->divine_timer = 15;

   act("You crouch forward protecting your body with your #Cshield#n.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches forward protecting $s body with $s #Cshield#n.",ch,NULL,NULL,TO_ROOM);
}

void do_cover( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_PALADIN ) && ch->pcdata->kingdom != 1 )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    if ( ch->pcdata->kingdom == 0 )
       {
          stc("#wYou can only do this skill if you are in a #ykingdom#w.#n\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 1 && !IS_CLASS( ch, CLASS_PALADIN ) && ch->pcdata->clanrank < 4 )
       {
          stc("#wYou aren't #ypowerful #wenough to cover #yknight#w.#n\n\r",ch);
          return;
       }

    if ( ch->protecting != NULL )
       {
          ch->dpower = 0;
          act("You stop protecting $N from all attacks.",ch,NULL,ch->protecting,TO_CHAR);
          act("$n stops protecting you from all attacks.",ch,NULL,ch->protecting,TO_VICT);
          ch->protecting->protector = NULL;
          ch->protecting = NULL;
          return;
       }

    if ( ch->safe_timer > 0 )
       {
          stc("You can't cover people until your safe timer is gone!",ch);
          return;
       }

    if ( nogang != 0 )
       {
          stc("You can't cover anyone if group battles are off!",ch);
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
	    send_to_char("They are not here.\n\r", ch);
          return;
       }

    if ( IS_NPC(victim) )
       {
	    send_to_char("You can't protect mobs.\n\r", ch);
          return;
       }

    if ( victim == ch )
       {
	    send_to_char("Don't be silly.\n\r", ch);
          return;
       }

    if ( !IS_NPC(ch) && !IS_NPC(victim) && ( ch->pcdata->kingdom == 0 || ch->pcdata->kingdom != victim->pcdata->kingdom ) )
       {
          stc("But they aren't in the same kingdom as you.\n\r",ch);
          return;
       }

    if ( victim->protector != NULL )
       {
	    send_to_char("They are already being protecting.\n\r", ch);
          return;
       }

    if ( victim->protecting != NULL )
       {
	    send_to_char("You can't protect a protector.\n\r", ch);
          return;
       }

    if ( !is_same_plane( ch, victim ) )
       {
          stc("But you aren't in the same plane as them.\n\r",ch);
          return;
       }

    give_align( ch, 25 );
    ch->wait = 12;

    act("You circle around $N acting as $S personal shield.",ch,NULL,victim,TO_CHAR);
    act("$n circles around you acting as your personal shield.",ch,NULL,victim,TO_VICT);
    act("$n circles around $N acting as $S personal shield.",ch,NULL,victim,TO_NOTVICT);

    stop_fighting( victim, TRUE );       
    victim->protector = ch;
    ch->protecting = victim;
}

void do_shellblast_stab( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int dam = 1000000, obj_dam = number_range( 1, 100 );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_DIVINE_KNIGHT ) && !IS_SET( ch->special, SPC_SWORD ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
       if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "But you are not holding a sword.\n\r", ch );
             return;
          }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to use Shellblast Stab on?\n\r",ch);
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

    if ( ch->mana < 50000 )
       {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You say, '#yArmor won't help the heart stay sharp, ShellBlast Stab!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yArmor won't help the heart stay sharp, ShellBlast Stab!#n'",ch,NULL,NULL,TO_ROOM);
    act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
    act("A huge spiral blade descends from the heavens SmAsHiNg into $N vIcIoUsLy!",ch,NULL,victim,TO_CHAR);
    act("A huge spiral blade descends from the heavens SmAsHiNg into you vIcIoUsLy!",ch,NULL,victim,TO_VICT);
    act("A huge spiral blade descends from the heavens SmAsHiNg into $N vIcIoUsLy!",ch,NULL,victim,TO_NOTVICT);

    if ( ch->pkill > 9 && ch->ctimer[11] == 0 )
       {
          dam *= 2;
          ch->ctimer[11] = 4;
       }

    if ( ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       {
          if ( !IS_CLASS( victim, CLASS_PALADIN ) && ( obj = get_eq_char(victim,WEAR_BODY) ) != NULL
               && !( !IS_NPC( victim ) && victim->pcdata->capture_timer > 0 )
               && !( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
                     || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91 
                     || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
                     || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
                     || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
                     || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
                     || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
                     || obj->item_type == ITEM_PKTOKEN ) )
             {
                if ( obj_dam > obj->resistance )
                   obj_dam = 35;

                obj->condition -= obj_dam;

                if ( obj->condition < 1 )
                   {
                      if ( obj->points > 0 )
                         ch->bank = UMIN( 500000000, ch->bank + obj->points );

                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                      victim->wait += 24;
                      obj_from_char( obj );
                      extract_obj( obj );
                  }
             }
       }

    ch->mana -= 500000;
    ch->wait = 18;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_LUNAR;
    ch->attack_var = BLUNT;
    ch->attack_focus = AERIAL;

    hurt_person(ch,victim,dam,TRUE);
}

void do_hellcry_punch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int dam = 1000000, obj_dam = number_range( 1, 100 );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_DIVINE_KNIGHT ) && !IS_SET( ch->special, SPC_SWORD ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
       if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "But you are not holding a sword.\n\r", ch );
             return;
          }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
           stc("Who do you wish to use HellCry Punch on?\n\r",ch);
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

    if ( ch->mana < 50000 )
       {
	     send_to_char( "You don't have enough magical energy for that.\n\r", ch );
           return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You say, '#yMay the Demons rip fear through your mortal heart, HellCry Punch!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yMay the Demons rip fear through your mortal heart, HellCry Punch!#n'",ch,NULL,NULL,TO_ROOM);
    act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
    act("$n staggers back as a wave of energy slashes through $s body!",victim,NULL,NULL,TO_ROOM);
    act("You stagger back as a wave of energy slashes through your body!",victim,NULL,NULL,TO_CHAR);
    act("Suddenly the energy wave SOARS through your brain causing you to stumble in agony!",victim,NULL,NULL,TO_CHAR);
    act("Suddenly the energy wave SOARS through $n's brain causing $m to stumble in agony!",victim,NULL,NULL,TO_ROOM);

    if ( ch->pkill > 9 && ch->ctimer[11] == 0 )
       {
          dam *= 2;
          ch->ctimer[11] = 4;
       }

    if ( ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       {
          if ( !IS_CLASS( victim, CLASS_PALADIN ) && ( obj = get_eq_char(victim,WEAR_HEAD) ) != NULL
               && !( !IS_NPC( victim ) && victim->pcdata->capture_timer > 0 )
               && !( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
                     || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91 
                     || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
                     || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
                     || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
                     || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
                     || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
                     || obj->item_type == ITEM_PKTOKEN ) )
             {
                if ( obj_dam > obj->resistance )
                   obj_dam = 35;

                obj->condition -= obj_dam;

                if ( obj->condition < 1 )
                   {
                      if ( obj->points > 0 )
                         ch->bank = UMIN( 500000000, ch->bank + obj->points );

                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                      victim->wait += 24;
                      obj_from_char( obj );
                      extract_obj( obj );
                  }
             }
       }

    ch->mana -= 500000;
    ch->wait = 18;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_DARK;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;

    hurt_person(ch,victim,dam,TRUE);
}

void do_blaster_punch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int dam = 1000000, obj_dam = number_range( 1, 100 );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_DIVINE_KNIGHT ) && !IS_SET( ch->special, SPC_SWORD ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
       if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "But you are not holding a sword.\n\r", ch );
             return;
          }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to use Blaster Punch on?\n\r",ch);
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

    if ( ch->mana < 50000 )
       {
	     send_to_char( "You don't have enough magical energy for that.\n\r", ch );
           return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You say, '#yThy weapon is meaningless in the true fight, Blaster Punch!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yThy weapon is meaningless in the true fight, Blaster Punch!#n'",ch,NULL,NULL,TO_ROOM);
    act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
    act("$n's weapon starts shaking violently as explosive energy radiates from it!",victim,NULL,NULL,TO_ROOM);
    act("Your weapon starts shaking violently as explosive energy radiates from it!",victim,NULL,NULL,TO_CHAR);
    act("Everything feels heavier as you collapse onto the ground for breath!",victim,NULL,NULL,TO_CHAR);
    act("$n collapses onto the ground for breath!",victim,NULL,NULL,TO_ROOM);

    if ( ch->pkill > 9 && ch->ctimer[11] == 0 )
       {
          dam *= 2;
          ch->ctimer[11] = 4;
       }

    if ( ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       {
          if ( ( obj = get_eq_char(victim,WEAR_WIELD) ) == NULL || obj->item_type != ITEM_WEAPON || obj->pIndexData->vnum == 91 || IS_SET(obj->quest, QUEST_ARTIFACT) )
             if ( ( obj = get_eq_char(victim,WEAR_HOLD) ) == NULL || obj->item_type != ITEM_WEAPON || obj->pIndexData->vnum == 91 || IS_SET(obj->quest, QUEST_ARTIFACT) )
                obj = NULL;

          if ( obj != NULL && ( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
               || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91 
               || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
               || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
               || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
               || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
               || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
               || obj->item_type == ITEM_PKTOKEN ) )
             obj = NULL;

          if ( !IS_CLASS( victim, CLASS_PALADIN ) && obj != NULL 
               && !( !IS_NPC( victim ) && victim->pcdata->capture_timer > 0 ) )
             {
                if ( obj_dam > obj->resistance )
                   obj_dam = 35;

                obj->condition -= obj_dam;

                if ( obj->condition < 1 )
                   {
                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                      victim->wait += 24;
                      obj_from_char( obj );
                      extract_obj( obj );
                  }
             }
       }

    ch->mana -= 500000;
    ch->wait = 18;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_FIRE;
    ch->attack_var = BLAST;
    ch->attack_focus = AERIAL;

    hurt_person(ch,victim,dam,TRUE);
}

void do_icewolf_bite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int dam = 1000000, obj_dam = number_range( 1, 100 );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_DIVINE_KNIGHT ) && !IS_SET( ch->special, SPC_SWORD ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
       if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "But you are not holding a sword.\n\r", ch );
             return;
          }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use IceWolf Bite on?\n\r",ch);
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
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You say, '#yFeel the sting of the IceWolf Bite!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yFeel the sting of the IceWolf Bite!#n'",ch,NULL,NULL,TO_ROOM);
    act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
    act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
    act("A mAsSiVe beam of ENERGY RIPS through the GroUnD ThrUsTiNg through Your DefEnsElEsS BoDy!!!",victim,NULL,NULL,TO_CHAR);
    act("A mAsSiVe beam of ENERGY RIPS through the GroUnD ThrUsTiNg through $n's DefEnsElEsS BoDy!!!",victim,NULL,NULL,TO_ROOM);

    if ( ch->pkill > 9 && ch->ctimer[11] == 0 )
       {
          dam *= 2;
          ch->ctimer[11] = 4;
       }

    if ( ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       {
          if ( ( obj = get_eq_char(victim,WEAR_NECK_1) ) == NULL || IS_SET(obj->quest, QUEST_ARTIFACT) )
             if ( ( obj = get_eq_char(victim,WEAR_NECK_2) ) == NULL || IS_SET(obj->quest, QUEST_ARTIFACT) )
                obj = NULL;

          if ( obj != NULL && ( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
               || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91 
               || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
               || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
               || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
               || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
               || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
               || obj->item_type == ITEM_PKTOKEN ) )
             obj = NULL;

          if ( !IS_CLASS( victim, CLASS_PALADIN ) && obj != NULL 
               && !( !IS_NPC( victim ) && victim->pcdata->capture_timer > 0 ) )
             {
                if ( obj_dam > obj->resistance )
                   obj_dam = 35;

                obj->condition -= obj_dam;

                if ( obj->condition < 1 )
                   {
                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                      act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                      victim->wait += 24;
                      obj_from_char( obj );
                      extract_obj( obj );
                  }
             }
       }

    ch->mana -= 500000;
    ch->wait = 18;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_ICE;
    ch->attack_var = BEAM;
    ch->attack_focus = UPPERCUT;

    hurt_person( ch, victim, dam, TRUE );
}

void do_store( CHAR_DATA *ch, char *argument )
{
   int dt;
   OBJ_DATA *wield;
   CHAR_DATA *victim = ch->fighting;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_PALADIN ) || ch->clannum != 2 )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->ctimer[1] > 0 )
      {
         send_to_char("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->position != POS_FIGHTING )
      {
         stc("But you aren't in a fight.\n\r",ch);
         return;
      }

   if ( victim == NULL )
      {
         stc("But you aren't in a fight.\n\r",ch);
         return;
      }

   if ( ch->move < 100000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   ch->move -= 100000;

   act("You crouch forward as you start concentrating furiously.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches forward as $e starts concentrating furiously.",ch,NULL,NULL,TO_ROOM);
   ch->ctimer[1] = 12;
   ch->ctimer[2] = 1;
}

void divine_strike( CHAR_DATA *ch )
{
   int dt;
   OBJ_DATA *wield;
   CHAR_DATA *victim = ch->fighting;

   if ( victim == NULL )
      return;

   stc("You release your stored energy driving your might into your blow.\n\r",ch);
   act("$n releases $s stored energy driving $s might into $s blow.",ch,NULL,NULL,TO_ROOM);

   if ( ( wield = get_eq_char( ch, WEAR_FOURTH ) ) == NULL || wield->item_type != ITEM_WEAPON )
      if ( ( wield = get_eq_char( ch, WEAR_THIRD ) ) == NULL || wield->item_type != ITEM_WEAPON )
         if ( ( wield = get_eq_char( ch, WEAR_HOLD ) ) == NULL || wield->item_type != ITEM_WEAPON )
            wield = get_eq_char( ch, WEAR_WIELD );

   dt = TYPE_HIT;
   if ( wield != NULL && wield->item_type == ITEM_WEAPON )
      dt += wield->value[3];

   SET_BIT(ch->newbits, NEW_MONKCLOAK);
   dam_message( ch, victim, 150000, dt );

   show_total_damage( ch, victim );
   hurt_person(ch,victim,150000,TRUE);
}
