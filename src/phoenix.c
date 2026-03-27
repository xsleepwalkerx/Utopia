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

void do_phoenix_rush( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch,CLASS_PHOENIX) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          send_to_char("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Whom to you want to swoop on?\n\r", ch );
          return;
       }

    if ( ( ( victim = get_char_area( ch, arg ) ) == NULL ) || ( victim->in_room == NULL ) )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }
 
    if ( victim->in_room->vnum == ch->in_room->vnum )
       {
          send_to_char("But you're already there!\n\r",ch);
          return;
       }

    stc("You flap your wings noisily as you zoom upwards in the sky.\n\r",ch);
    act("$n flaps $s wings noisily as $e zooms upwards in the sky.\n\r",ch,NULL,NULL,TO_ROOM);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    do_look(ch, "auto");
    stc("Circling around the vicinity, you charge straight down onto your target.\n\r",ch);
    act("$n charges from the sky straight into $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n charges from the sky straight into you.",ch,NULL,victim,TO_VICT);

    ch->ctimer[0] = 2;
    ch->wait = 12;

    if ( ch->position < POS_STANDING || IS_SET( ch->more, MORE_FROZEN ) )
       return;

    multi_hit( ch, victim, TYPE_UNDEFINED );
}

void do_firebreath( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj, *obj_next;
    int obj_dam;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_PHOENIX ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to breathe upon?\n\r",ch);
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

    if ( ch->move < 250000 )
       {
          send_to_char( "You are too tired to execute this technique.\n\r", ch );
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You take in a deep breath as shards of light manifests into your mouth.",ch,NULL,NULL,TO_CHAR);
    act("$n takes in a deep breath as shards of light manifests into $s mouth.",ch,NULL,NULL,TO_ROOM);
    act("#RYou then #ybreathe #Ra barrage of #eDeVaSTaTiNg #yflames #Rall over $N#R!#n",ch,NULL,victim,TO_CHAR);
    act("#R$n #Rthen #ybreathes #Ra barrage of #eDeVaSTaTiNg #yflames #Rall over you!#n",ch,NULL,victim,TO_VICT);
    act("#R$n #Rthen #ybreathes #Ra barrage of #eDeVaSTaTiNg #yflames #Rall over $N#R!#n",ch,NULL,victim,TO_NOTVICT);

    if ( ch->pkill > 14 && ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !( !IS_NPC( victim ) 
         && victim->pcdata->capture_timer > 0 ) && victim->position == POS_FIGHTING && ch->ctimer[15] == 0
         && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       {
          for ( obj = victim->carrying; obj != NULL; obj = obj_next )
          {
             obj_next = obj->next_content;

             if ( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
                  || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91 
                  || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
                  || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
                  || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
                  || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
                  || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
                  || obj->item_type == ITEM_PKTOKEN )
                continue;

             obj_dam = number_range( 1, 100 );

             if ( obj_dam > obj->resistance )
                obj_dam = 10;

             obj->condition -= obj_dam;

             if ( obj->condition < 1 )
                {
                   if ( obj->points > 0 )
                      ch->bank = UMIN( 500000000, ch->bank + obj->points );

                   act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                   act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                   obj_from_char( obj );
                   extract_obj( obj );
               }
          }
       }

    if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
       SET_BIT( victim->affected_by, AFF_FLAMING );

    ch->move -= 250000;
    ch->wait = 18;

    ch->ele_attack = ATTACK_FIRE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = BLAST;

    hurt_person( ch, victim, 1000000, TRUE );
}

void do_flame_talons( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj, *obj_next;
    int dam = 900000, obj_dam;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_PHOENIX ) )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to strike?\n\r",ch);
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

    if ( ch->move < 250000 )
       {
          send_to_char( "You are too tired to execute this technique.\n\r", ch );
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You raise a claw infront of your face as shards of light manifests between them.",ch,NULL,NULL,TO_CHAR);
    act("$n raises a claw infront of $s face as shards of light manifests between them.",ch,NULL,NULL,TO_ROOM);
    act("#RYou then #ystrike #Rviciously at $N#R, giving $M a #eReNdInG #Rwound!#n",ch,NULL,victim,TO_CHAR);
    act("#R$n #Rthen #ystrikes #Rviciously at you, giving you a #eReNdInG #Rwound!#n",ch,NULL,victim,TO_VICT);
    act("#R$n #Rthen #ystrikes #Rviciously at $N#R, giving $M a #eReNdInG #Rwound!#n",ch,NULL,victim,TO_NOTVICT);

    if ( ch->pkill > 14 && ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && !( !IS_NPC( victim ) 
         && victim->pcdata->capture_timer > 0 ) && victim->position == POS_FIGHTING && ch->ctimer[15] == 0
         && !IS_SET( victim->cmbt[5], SKL2_MAINTENANCE ) )
       {
          for ( obj = victim->carrying; obj != NULL; obj = obj_next )
          {
             obj_next = obj->next_content;

             if ( IS_SET( obj->quest, QUEST_ARTIFACT ) || obj->pIndexData->vnum == 437
                  || obj->pIndexData->vnum == 182 || obj->pIndexData->vnum == 91 
                  || obj->pIndexData->vnum == 92 || obj->pIndexData->vnum == 95
                  || obj->pIndexData->vnum == 439 || obj->pIndexData->vnum == 246
                  || obj->pIndexData->vnum == 168 || obj->pIndexData->vnum == 370
                  || obj->pIndexData->vnum == 310 || obj->pIndexData->vnum == 122
                  || obj->item_type == ITEM_QUEST || obj->item_type == ITEM_DTOKEN
                  || obj->item_type == ITEM_PKTOKEN )
                continue;

             obj_dam = number_range( 1, 100 );

             if ( obj_dam > obj->resistance )
                obj_dam = 10;

             obj->condition -= obj_dam;

             if ( obj->condition < 1 )
                {
                   if ( obj->points > 0 )
                      ch->bank = UMIN( 500000000, ch->bank + obj->points );

                   act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_CHAR);
                   act("#c$p #yexplodes #Rviolently #gin a #w#zflash#n #gof #ylight#g!#n",ch,obj,NULL,TO_ROOM);
                   obj_from_char( obj );
                   extract_obj( obj );
               }
          }
       }

    if ( !IS_SET( victim->cmbt[5], SKL2_GRIP ) && number_percent() > 89 - devil_luck( ch, victim, 5 ) )
       {
          if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
             if ( ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
                if ( ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
                   if ( ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
                      obj = NULL;

          if ( obj != NULL && !( IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_ASSASSIN )
                || IS_SET( victim->cmbt[5], SKL2_GRIP ) ) )
             {
                obj_from_char( obj );
                obj_to_char( obj, victim );
        
                act("#RYou #yslash #Ragain at the side, suddenly #eDiSaRmInG #R$M!#n",ch,NULL,victim,TO_CHAR);
                act("#R$n #yslashes #Ragain at the side, suddenly #eDiSaRmInG #Ryou!#n",ch,NULL,victim,TO_VICT);
                act("#R$n #yslashes #Ragain at the side, suddenly #eDiSaRmInG #R$M!#n",ch,NULL,victim,TO_NOTVICT);
             }
       }
       else if ( number_percent() > 89 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more, MORE_BLIND ) )
               {
                  SET_BIT( victim->more, MORE_BLIND );
                  victim->blind_timer = number_range( 3, 5 );

                  act("#RYou #yslash #Ragain at the side, suddenly #wBlInDiNg #R$M!#n",ch,NULL,victim,TO_CHAR);
                  act("#R$n #yslashes #Ragain at the side, suddenly #wBlInDiNg #Ryou!#n",ch,NULL,victim,TO_VICT);
                  act("#R$n #yslashes #Ragain at the side, suddenly #wBlInDiNg #R$M!#n",ch,NULL,victim,TO_NOTVICT);
               }

    if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
       SET_BIT( victim->affected_by, AFF_FLAMING );

    ch->move -= 250000;
    ch->wait = 18;

    ch->ele_attack = ATTACK_FIRE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = SLASH;
    hurt_person( ch, victim, dam, TRUE );
}

void do_phoenix_wrath( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) ) 
      return;

    if ( !IS_CLASS(ch,CLASS_PHOENIX) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 5 )
       {
          send_to_char("You aren't powerful enough yet to do this technique.\n\r",ch);
          return;
       }

   if ( ch->mana < 5000000 )
      {
         send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
      {
         send_to_char( "You cannot do this in a safe room.\n\r", ch );
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_FLAME) )
      {
         stc("The room is already enveloped in flames.\n\r",ch);
         return;
      }

   ch->mana -= 5000000;
   ch->wait = 6;

   act("You suddenly open your wings wide apart as spruts of flames fall upon the room.",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly opens $s wings wide apart as spruts of flames fall upon the room.",ch,NULL,NULL,TO_ROOM);
   act("#RThe #yflames #Rstart #eeating #Raway at #weverything #Rin its path as the vicinity turns into an inferno.#n",ch,NULL,NULL,TO_CHAR);
   act("#RThe #yflames #Rstart #eeating #Raway at #Weverything #Rin its path as the vicinity turns into an inferno.#n",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->in_room->room_flags, ROOM_FLAME);
}

void do_phoenix_cry(CHAR_DATA *ch,char *argument)
{
    DESCRIPTOR_DATA *d;

    if ( !IS_CLASS(ch,CLASS_PHOENIX) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 20 )
       {
          send_to_char("You aren't powerful enough yet to do this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[3] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->ctimer[3] = 6;
    ch->wait = 12;
    
    act( "You suddenly tilt your head to the sky as you release a MASSIVE screech!!", ch, NULL, NULL, TO_CHAR );
    act( "$n suddenly tilts $s head to the sky as $e releases a MASSIVE screech!!", ch, NULL, NULL, TO_ROOM );
    act( "A fiery shockwave flashes from your body as it scatters throughout the vicinity!", ch, NULL, NULL, TO_CHAR );
    act( "A fiery shockwave flashes from $n's body as it scatters throughout the vicinity!", ch, NULL, NULL, TO_ROOM );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	 {
          if ( ch->in_room->area != d->character->in_room->area || !can_heal( ch, d->character )
               || d->character->position >= POS_STUNNED )
             continue;

          give_align( ch, 75 );

          d->character->position = POS_STANDING;
          d->character->hit = d->character->max_hit * 0.67;

	    act("#RYou suddenly feel alot better as the #yflames #wpurifies #Ryour #ewounds#R!#n",d->character,NULL,NULL,TO_CHAR);
	    act("#R$n #Rsuddenly feels alot better as the #yflames #wpurifies #R$s #ewounds#R!#n",d->character,NULL,NULL,TO_ROOM);
 	 }
    }
}

void do_blaze( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch,CLASS_PHOENIX) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
      {
         send_to_char( "You cannot do this in a safe room.\n\r", ch );
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You suddenly tilt your head to the sky as you release a MASSIVE screech!!", ch, NULL, NULL, TO_CHAR );
   act("$n suddenly tilts $s head to the sky as $e releases a MASSIVE screech!!", ch, NULL, NULL, TO_ROOM );
   act("You then release a burst of flames across the room as your eyes burn red.",ch,NULL,NULL,TO_CHAR); 
   act("$n then releases a burst of flames across the room as $s eyes burn red.",ch,NULL,NULL,TO_CHAR); 
   act("#RThe room suddenly #yExPlOdEs #Ras a MaNiFeSt oF SeArInG EnErGy EnLiTs the ViCINiTy!!!#n",ch,NULL,NULL,TO_CHAR); 
   act("#RThe room suddenly #yExPlOdEs #Ras a MaNiFeSt oF SeArInG EnErGy EnLiTs the ViCINiTy!!!#n",ch,NULL,NULL,TO_ROOM); 
   SET_BIT(ch->in_room->room_flags, ROOM_BLAZE);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_focus = UPPERCUT;
      ch->attack_var = BLAST;
      hurt_person( ch, vch, 1500000, TRUE );
   }
}

void do_volcano(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *vch, *vch_next;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch,CLASS_PHOENIX) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    act("#RYou #Rfocus on your innate powers as a #gVolcano#R spruts up and erupts!!#n",ch,NULL,NULL,TO_CHAR);
    act("#R$n #Rfocuses on $s innate powers as a #gVolcano#R spruts up and erupts!!#n",ch,NULL,NULL,TO_ROOM);

    ch->wait = 12;
    subtract_stock( ch, 100 );
    SET_BIT( ch->arena, AFF2_DM );

    if ( ch->pkill > 11 )
       SET_BIT( ch->in_room->area->bits, MAGMA );

    act("Suddenly.. The #gearth#n #zcrackles#n beneath everyone.. UH OH!",ch,NULL,NULL,TO_CHAR);
    act("Suddenly.. The #gearth#n #zcrackles#n beneath everyone.. UH OH!",ch,NULL,NULL,TO_ROOM);
    act("#RHot magma spills everywhere as everyone frantically runs about!!!#n",ch,NULL,NULL,TO_CHAR);
    act("#RHot magma spills everywhere as everyone frantically runs about!!!#n",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
          continue;

       act("#RYou fall in between the cracks of earth and get burned.. OUCH!#n",vch,NULL,NULL,TO_CHAR);
       act("#R$n #Rfalls in between the cracks of earth and gets burned.. OUCH!#n",vch,NULL,NULL,TO_ROOM);
       ch->ele_attack = ATTACK_FIRE;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_focus = LOW;
       ch->attack_var = HIT;
       hurt_person( ch, vch, 1000000, TRUE );
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	 {
           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) || IS_CLASS( d->character, CLASS_DUERGAR ) )
              continue;

           if ( ch->in_room->area != d->character->in_room->area )
              continue;

           act("#RYou fall in between the cracks of earth and get burned.. OUCH!#n",d->character,NULL,NULL,TO_CHAR);
           act("#R$n #Rfalls in between the cracks of earth and gets burned.. OUCH!#n",d->character,NULL,NULL,TO_ROOM);
           ch->ele_attack = ATTACK_FIRE;
           ch->attack_type = ATTACK_PHYSICAL;
           ch->attack_focus = LOW;
           ch->attack_var = HIT;
           hurt_person( ch, d->character, 1000000, TRUE );
       }
    }
}

void do_fire_cannon( CHAR_DATA *ch, char *argument)
{
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA       *pexit;
   CHAR_DATA       *vch, *vch_next;
   char            arg1      [MAX_INPUT_LENGTH];
   int             door;
   int             count;

   argument = one_argument( argument, arg1 );

   if ( !IS_CLASS(ch,CLASS_PHOENIX) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( arg1[0] == '\0' )
      {
	   send_to_char("Please specify a direction to release your breath.\n\r", ch);
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

   ch->wait = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   if ( IS_SET( ch->act, PLR_WIZINVIS ) )
      REMOVE_BIT( ch->act, PLR_WIZINVIS );

   act("You suddenly fly a few feet above your foes as you glance around you angrilly.",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly flies a few feet above $s foes as $e glances around $m angrilly.",ch,NULL,NULL,TO_ROOM);
   act("Time seem to stand still as a circle of passive flames encircle the vicinity...",ch,NULL,NULL,TO_CHAR);
   act("Time seem to stand still as a circle of passive flames encircle the vicinity...",ch,NULL,NULL,TO_ROOM);
   act("You open your beak as a spiral of ferocious energy manifests between it.",ch,NULL,NULL,TO_CHAR);
   act("$n opens $s beak as a spiral of ferocious energy manifests between it.",ch,NULL,NULL,TO_ROOM);
   act("You struggle to control the energy as a loud hum fills the air...",ch,NULL,NULL,TO_CHAR);
   act("$n struggles to control the energy as a loud hum fills the air...",ch,NULL,NULL,TO_ROOM);
   act("#RYou #ythen release a #RMASSIVE #yfire beam to the #R$T#y!#n",ch,NULL,dir_name[door],TO_CHAR);
   act("#R$n #ythen releases a #RMASSIVE #yfire beam to the #R$T#y!#n",ch,NULL,dir_name[door],TO_ROOM);

   for ( count = 0; count < 10; count++ )
   {
      for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next_in_room;

         if ( is_same_group( ch, vch ) || is_safe( ch, vch ) )
            continue;

         act("A TREMEDOUS beam sends you flying as the flames ROARS hungrily!",vch,NULL,NULL,TO_CHAR);
         act("A TREMEDOUS beam sends $n flying as the flames ROARS hungrily!",vch,NULL,NULL,TO_ROOM);

         vch->agg = 100;

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = DIRECT;
         ch->attack_var = BEAM;
         hurt_person( ch, vch, 3500000 , TRUE );
      }

      if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
         {
            act("#yThe #Rwall #ytrembles slightly as it #Rabsorbs #ya ferocious #Rbeam#y.#n",ch,NULL,NULL,TO_CHAR);
            act("#yThe #Rwall #ytrembles slightly as it #Rabsorbs #ya ferocious #Rbeam#y.#n",ch,NULL,NULL,TO_ROOM);

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
   }

   stop_fighting( ch, TRUE );
   char_from_room( ch );
   char_to_room( ch, location );
}
