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

void do_duergar_armor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_DUERGAR) )
  {
    send_to_char("You are not a Duergar.\n\r",ch);
    return;
  }

  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of unholy armor you wish to make: plate ring bracer collar helmet leggings boots gauntlets chains cloak belt mask axe.\n\r",ch);
    return;
  }

  if ( ch->gold < 1 )
  {
    send_to_char("It costs #w1 #ygold#n piece to create a piece of Duergar armor.\n\r",ch);
    return;
  }

  if (!str_cmp(arg,"plate")) vnum =  440;
  else if (!str_cmp(arg,"axe")) vnum = 452;
  else if (!str_cmp(arg,"ring")) vnum = 441;
  else if (!str_cmp(arg,"bracer")) vnum = 442;
  else if (!str_cmp(arg,"collar")) vnum = 443;
  else if (!str_cmp(arg,"helmet")) vnum = 444;
  else if (!str_cmp(arg,"leggings")) vnum = 445;
  else if (!str_cmp(arg,"boots")) vnum = 446;
  else if (!str_cmp(arg,"gauntlets")) vnum = 447;
  else if (!str_cmp(arg,"chains")) vnum = 448;
  else if (!str_cmp(arg,"cloak")) vnum = 449;
  else if (!str_cmp(arg,"belt")) vnum = 450;
  else if (!str_cmp(arg,"mask")) vnum = 451;
  else
  {
    send_to_char("Please specify which piece of unholy armor you wish to make: plate ring bracer collar helmet leggings boots gauntlets chains cloak belt mask axe.\n\r",ch);
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform Jobo.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  ch->gold -=1;
}

void do_swing( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];
  int dam = 700000;

  if ( IS_NPC(ch)  ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DUERGAR) )
     return;

  one_argument( argument, arg );

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

  if ( is_safe(ch,victim) )
     return;

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding an axe.\n\r", ch );
           return;
        }

  if ( ch->move < 250000 )
     { 
        stc("You aren't feeling athletic enough for that.\n\r",ch);
        return;
     }

  ch->move -= 250000;

  act("You grunt and you lift your heavy #0axe#n into the air as you #7swing#n for $N",ch,NULL,victim,TO_CHAR);
  act("$n grunts and $e lifts $s heavy #0axe#n into the air as $e #7swings#n for $N",ch,NULL,victim,TO_NOTVICT);
  act("$n grunts and $e lifts $s heavy #0axe#n into the air as $e #7swings#n for you",ch,NULL,victim,TO_VICT);
  act("You slice through $N ViCiOuSlY making $M scream in terrifying pain!",ch,NULL,victim,TO_CHAR);
  act("$n slices through $N ViCiOuSlY making $M scream in terrifying pain!",ch,NULL,victim,TO_NOTVICT);
  act("$n slices through you ViCiOuSlY making you scream in terrifying pain!",ch,NULL,victim,TO_VICT);
  act("You scream uncontrollably, #Rblood#n flying everywhere",victim,NULL,NULL,TO_CHAR);
  act("$n screams uncontrollably, #Rblood#n flying everywhere",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SLASH;
  ch->attack_focus = DIRECT;
  hurt_person( ch, victim, dam, TRUE );
}

void do_slasher_axe( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  int dam = 1000000;
  char      arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  argument = one_argument( argument, arg );

  if ( !IS_CLASS(ch, CLASS_DUERGAR) )
     return;

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding an axe.\n\r", ch );
           return;
        }

  if ( ch->move < 1000000 )
     { 
        stc("You are too tired to do this technique.\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     {
	    send_to_char( "Do this on whom?\n\r", ch );
	    return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
          send_to_char( "They aren't here.\n\r", ch );
          return;
     }

  if ( is_safe(ch,victim) )
     return;

  if ( IS_ARM_L(victim,LOST_ARM) )
     {
        stc("Your victim dont have a left arm.\n\r",ch);
        return;
     }

  ch->move -= 1000000;
  ch->wait = 18;

  act("You grunt in a deep voice '#yMEET THE SLASHER AXE!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n gruns in a deep voice '#yMEET THE SLASHER AXE!#n'",ch,NULL,NULL,TO_ROOM);
  act("You lurch forward giving another satisfying grunt as you slash at $N's arms",ch,NULL,victim,TO_CHAR);
  act("$n lurches forward giving another satisfying grunt as $e slashes at $N's arms",ch,NULL,victim,TO_NOTVICT);
  act("$n lurches forward giving another satisfying grunt as $e slashes at your arms",ch,NULL,victim,TO_VICT);
  act("You scream in absolute agony as your arms fall, and your left with #Rbleeding#n stumps!",victim,NULL,NULL,TO_CHAR);
  act("$n screams in absolute agony as $s arms fall, and $e's left with #Rbleeding#n stumps!",victim,NULL,NULL,TO_ROOM);

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SLASH;
  ch->attack_focus = DIRECT;
  hurt_person(ch,victim,dam,TRUE);

  if ( !IS_ARM_L(victim,LOST_ARM) )
     {
         SET_BIT(victim->loc_hp[2],LOST_ARM);
         SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	   make_part(victim,"arm");
         if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
            {
               if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
               take_item(victim,obj);
	      }
         if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
            take_item(victim,obj);
     }

  if ( !IS_LEG_R(victim,LOST_LEG) )
     {
	   SET_BIT(victim->loc_hp[5],LOST_LEG);
	   SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
  	   make_part(victim,"leg");
	   if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	      {
	         if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
		      take_item(victim,obj);
	      }
	   if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	      take_item(victim,obj);
     }

   if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4) )
      SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);

   if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2) )
      SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);

   if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1) )
      SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);
}

void do_axe_slam( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;   
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DUERGAR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->pkill < 2 )
      {
	   stc("You aren't strong enough to do this technique Duergar.\n\r",ch);
	   return;
	}

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Axe Slam on?\n\r", ch );
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

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding an axe.\n\r", ch );
            return;
         }

   ch->mana -= 500000;

   act("You grunt and slam your axe deep into the #qground#n",ch,NULL,NULL,TO_CHAR);
   act("$n grunts and slams $s axe deep into the #qground#n",ch,NULL,NULL,TO_ROOM);
   act("The #gearth#n crumbles before you as your #0axe#n gives off a huge #7shockwave#n!",ch,NULL,NULL,TO_CHAR);
   act("The #gearth#n crumbles before $n as $s #0axe#n gives off a huge #7shockwave#n#!",ch,NULL,NULL,TO_ROOM);

   if ( number_percent() > 60 && !IS_SET( victim->more, MORE_BLIND ) )
      {
         act("The #gearthly #7shockwave#n causes you to wince as you are #0blinded..#n",victim,NULL,NULL,TO_CHAR);
         act("The #gearthly #7shockwave#n causes $n to wince as $e is #0blinded..#n",victim,NULL,NULL,TO_ROOM);
         SET_BIT( victim->more, MORE_BLIND );
         victim->blind_timer = 4;
      }

   ch->wait = 18;

   ch->ele_attack = ATTACK_EARTH;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLUNT;
   ch->attack_focus = LOW;
   hurt_person(ch,victim,dam,TRUE);
}

void do_axed_drain( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
 
   argument = one_argument( argument, arg );

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_DUERGAR) )
      {
         send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 6 )
      {
         stc("You are not strong enough to do this yet Duergar.\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding an axe.\n\r", ch );
            return;
         }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to use axed drain on?\n\r",ch);
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

   if ( ch->mana < 1000000 )
      {
         send_to_char( "You don't have enough mana to do this\n\r", ch );
         return;
      }

   ch->mana -= 1000000;

   if ( is_safe(ch, victim) ) return;

   if ( IS_SET( victim->more, MORE_HAMMER ) )
      {
         send_to_char( "You are already draining there energy.\n\r", ch );
	   return;
      }

   ch->wait = 8;

   if ( number_percent() < 40 )
      {
         act("You grunt and throw your axe into $N but it bounces clean off $M!",ch,NULL,victim,TO_CHAR);
         act("$n grunts and throws $s axe into $N but it bounces clean off $M!",ch,NULL,victim,TO_NOTVICT);
         act("$n grunts and throws $s axe into you but it bounces clean off you!",ch,NULL,victim,TO_VICT);
         act("You mumble '#yI'll get you next time!#n'",ch,NULL,NULL,TO_CHAR);
         act("$n mumbles '#yI'll get you next time!#n'",ch,NULL,NULL,TO_ROOM);
         return;
      }

   act("You grunt and throw your axe into $N, it hits $M in the arm!",ch,NULL,victim,TO_CHAR);
   act("$n grunts and throws $s axe into $N, it hits $M in the arm!",ch,NULL,victim,TO_NOTVICT);
   act("$n grunts and throws $s axe into you, it hits you in the arm",ch,NULL,victim,TO_VICT);
   act("You mumble '#yDirect hit!#n' as you start to look #Phealthier!#n",ch,NULL,NULL,TO_CHAR);
   act("$n mumbles '#yDirect hit!#n' as $e starts to look #Phealthier!#n",ch,NULL,NULL,TO_ROOM);
   SET_BIT( victim->more, MORE_HAMMER );
}

void do_axe_knockout( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DUERGAR) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You are not strong enough to do this yet Duergar.\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding an axe.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ch->ctimer[0] > 0 )
      {
         stc("You're still recovering from your last one.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to use Whiskey Break on?\n\r", ch );
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

   if ( ch->move < 1000000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
	   return;
      }

   if ( is_safe(ch, victim) ) return;

   ch->move -= 1000000;
   ch->wait = 18;

   act("You swing your axe towards $N's head!",ch,NULL,victim,TO_CHAR);
   act("$n swings $s axe towards $N's head!",ch,NULL,victim,TO_NOTVICT);
   act("$n swings $s axe towards your head!",ch,NULL,victim,TO_VICT);
   act("You barely move out of the way of the attack!",victim,NULL,NULL,TO_CHAR);
   act("$n barely moves out the way of the attack!",victim,NULL,NULL,TO_ROOM);

   ch->ctimer[0] = 4;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = OVERHEAD;
   hurt_person(ch,victim,1000000,TRUE);

   if ( victim != NULL && victim->position > POS_STUNNED && number_percent() > 70 - devil_luck( ch, victim, 10 ) )
      {
         stop_fighting( victim, TRUE );
         stop_fighting( ch, TRUE );

         act("You swing your axe one final time at $N's head!",ch,NULL,victim,TO_CHAR);
         act("$n swings $s axe one final time at $N's head!",ch,NULL,victim,TO_NOTVICT);
         act("$n swings $s axe one final time at your head!",ch,NULL,victim,TO_VICT);
         act("The axe strikes you as a clear blow, knocking you cold!",victim,NULL,NULL,TO_CHAR);
         act("The axe strikes $n as a clear blow, knocking $m cold!",victim,NULL,NULL,TO_ROOM);

         if ( victim->position == POS_STUNNED && number_percent() > 84 - devil_luck( ch, victim, 5 ) )
            {
               act("You are quietly dragged off into a different room.",victim,NULL,NULL,TO_CHAR);
               act("$n is quietly dragged off into a different room.",victim,NULL,NULL,TO_ROOM);
               get_random_room( victim );
            }

         victim->position = POS_STUNNED;
      }
}

void do_dark_cloud(CHAR_DATA *ch, char *argument) 
{
	if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DUERGAR ) )
         {
           stc("Huh?\n\r",ch);
           return;
         }

    if ( ch->pkill < 10 )
	   {
		 stc("You are not strong enough to do this yet Duergar.\n\r",ch);
		 return;
	   }

	if ( IS_SET( ch->more, MORE_CLOUD ) ) 
         {
	      REMOVE_BIT( ch->more, MORE_CLOUD );

  	      send_to_char("You banish the dark cloud that is among you.\n\r", ch );
	      act("The dark cloud around $n disappears.",ch,NULL,NULL,TO_ROOM);

	      if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) && ch->in_room->drows == 0 )
	         REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);

	      return;
	}

	if (ch->mana < 2000000) {
	send_to_char("You don't have enough mana to summon the dark cloud.\n\r",ch );
	return;}

	send_to_char("You summon a dark cloud around yourself.\n\r", ch );
	act("$n summons a dark cloud around $mself.",ch,NULL,NULL,TO_ROOM);

      SET_BIT( ch->more, MORE_CLOUD );
	if (!IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
	SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
}

void do_tunnel( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *room;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DUERGAR) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 13 )
       {
          stc("You are not strong enough to do this yet Duergar.\n\r",ch);
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    one_argument( argument, arg );

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( IS_IMMORTAL( victim ) )
       {
          send_to_char( "Not on immortals.\n\r", ch );
          return;
       }

    if ( IS_NPC( victim ) )
       {
          send_to_char( "Not on creatures.\n\r", ch );
          return;
       }

    if ( victim->fight_timer == 0 )
       {
          send_to_char( "But they aren't fighting.\n\r", ch );
          return;
       }

    if ( ( victim->in_room->vnum >= 2500 && victim->in_room->vnum < 3000 )
         || ( victim->in_room->vnum >= 3500 && victim->in_room->vnum < 4000 ) )
       {
          for ( ; ; )
          { 
             room = get_room_index( number_range( victim->in_room->area->lvnum, victim->in_room->area->uvnum ) );
 
             if ( room != NULL )
                break;
          }
       }
       else for ( ; ; )
            {
                room = get_room_index( number_range( 0, 32000 ) );

                if ( room == NULL )
                   continue;

                if ( ( room->vnum >= 2500 && room->vnum < 3000 )
                     || ( room->vnum >= 3500 && room->vnum < 4000 )
                     || room->vnum < 100 )
                   continue;

                break;
            }

   ch->ctimer[2] = 1;
   act( "Taking a gulp of Duergar's finest brew, you roar '#yYOU THINK YOU CAN HIDE FROM ME?!?#n'", ch, NULL, NULL, TO_CHAR );
   act( "Taking a gulp of Duergar's finest brew, $n roars '#yYOU THINK YOU CAN HIDE FROM ME?!?#n'", ch, NULL, NULL, TO_ROOM );
   act( "Really pissed off now, you take your axe and SMASH it hard into the ground.", ch, NULL, NULL, TO_CHAR );
   act( "Really pissed off now, $n takes $s axe and SMASHES it hard into the ground.", ch, NULL, NULL, TO_ROOM );
   act( "You then jump into the large gaping hole you had just created.", ch, NULL, NULL, TO_CHAR );
   act( "$n then jumps into the large gaping hole $e had just created.", ch, NULL, NULL, TO_ROOM );

   char_from_room( ch );

   if ( number_percent() < 50 + devil_luck( ch, victim, 10 ) )
      char_to_room( ch, room );
      else char_to_room( ch, victim->in_room );

   act( "You burst out from within a tunnel saying '#ySon of a... where is that MIDGET?!?#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n bursts out from within a tunnel saying '#ySon of a... where is that MIDGET?!?#n'", ch, NULL, NULL, TO_ROOM );
}

void do_vineofevil( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_DUERGAR) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 16 )
       {
          stc("You are not strong enough to do this yet Duergar.\n\r",ch);
          return;
       }

    if ( ch->ctimer[3] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->move < 1500000 )
       {
	    send_to_char( "You are too tired to execute this technique.\n\r", ch );
	    return;
       }

    ch->move -= 1500000;
    ch->ctimer[3] = 4;

    act("You grunt then call out '#yTitan!! I need your help to do my evil bidding!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n grunts then calls out '#yTitan!! I need your help to do my evil bidding!#n'",ch,NULL,NULL,TO_ROOM);
    act("#GTitan#n appears ripping up all the #gearth#n with him as a thousand vines appear below.",ch,NULL,NULL,TO_CHAR);
    act("#GTitan#n appears ripping up all the #gearth#n with him as a thousand vines appear below.",ch,NULL,NULL,TO_ROOM);
    act("#GTitan#n stares at you then says '#yYou know what to do Duergar..#n'",ch,NULL,NULL,TO_CHAR);
    act("#GTitan#n stares at $n then says '#yYou know what to do Duergar..#n'",ch,NULL,NULL,TO_ROOM);
    act("You call the vines into your power and start to entangle your foes!",ch,NULL,NULL,TO_CHAR);
    act("$n calls the vines into $s power and starts to entangle $s foes!",ch,NULL,NULL,TO_ROOM);
	
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch,vch) || is_safe(ch,vch) )
          continue;

       act("A massive vine wraps itself around your throat and leaves you hanging in the air!",ch,NULL,vch,TO_CHAR);
       act("A massive vine wraps itself around $n's throat and leaves $m hanging in the air!",ch,NULL,vch,TO_ROOM);
       stop_fighting( vch, TRUE );

       if ( !is_martial( vch ) )
          vch->stunned = number_range( 1, 2 );
          else vch->position = POS_STUNNED;
    }

    stop_fighting( ch, TRUE );
}

void do_stained_axe( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch)  ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DUERGAR) )
     return;

  one_argument( argument, arg );

  if ( get_stock( ch ) < 100 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding an axe.\n\r", ch );
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

  if ( is_safe(ch,victim) )
     return;

  act("You grunt and stain your axe with the evilest #gpoison#n!",ch,NULL,NULL,TO_CHAR);
  act("$n gruns and stains $s axe with the evilest #gpoison#n!",ch,NULL,NULL,TO_ROOM);
  act("You swing your axe down towards $N as the #gpoison#n infiltrates $S wounds!",ch,NULL,victim,TO_CHAR);
  act("$n swings $s axe down towards $N as the #gpoison#n infiltrates $S wounds!",ch,NULL,victim,TO_NOTVICT);
  act("$n swings $s axe down towards you as the #gpoison#n infiltrates $S wounds!",ch,NULL,victim,TO_VICT);
  act("You cry in #Ragony#n as the #gpoison#n #0eats#n its way through you!",victim,NULL,NULL,TO_CHAR);
  act("$n cries in #Ragony#n as the #gpoison#n #0eats#n its way through $m!",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  subtract_stock( ch, 100 );
  SET_BIT( ch->arena, AFF2_DM );

  if ( !IS_CLASS(victim, CLASS_DROW ) && !IS_CLASS(victim, CLASS_DRAGON ) )
     SET_BIT(victim->more, MORE_POISON);

  if ( !IS_CLASS( victim, CLASS_DUERGAR ) )
     {
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_var = SLASH;
        ch->attack_focus = OVERHEAD;
        hurt_person( ch, victim, 1250000, TRUE );
     }
     else victim->hit = UMIN( victim->max_hit, victim->hit + ( 1000000 * UMAX( 1, victim->pkill / 10 ) ) );
}

void do_duergar_dance( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch, *vch_next;
  OBJ_DATA *obj;
  int count = 0;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_DUERGAR) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 1 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding an axe.\n\r", ch );
           return;
        }

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  act("You grunt then call out '#yTitan!! I need your help to do my evil bidding!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n grunts then calls out '#yTitan!! I need your help to do my evil bidding!#n'",ch,NULL,NULL,TO_ROOM);
  act("#GTitan#n appears ripping up all the #gearth#n with him as a thousand rocks appear overhead.",ch,NULL,NULL,TO_CHAR);
  act("#GTitan#n appears ripping up all the #gearth#n with him as a thousand rocks appear overhead.",ch,NULL,NULL,TO_ROOM);
  act("#GTitan#n stares at you then says '#yYou know what to do Duergar..#n'",ch,NULL,NULL,TO_CHAR);
  act("#GTitan#n stares at $n then says '#yYou know what to do Duergar..#n'",ch,NULL,NULL,TO_ROOM);
  act("You call the stones into your power and start to throw them around!",ch,NULL,NULL,TO_CHAR);
  act("$n calls the stones into $s power and starts to throw them around!",ch,NULL,NULL,TO_ROOM);
	
  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) || is_safe(ch,vch) )
        continue;

     for ( count = 0; count < 3; count++ )
     {
       if ( number_percent() > 84 - devil_luck( ch, vch, 5 ) )
          {
             act("A huge stone hits you winding you completely!",vch,NULL,NULL,TO_CHAR);
             act("A huge stone hits $n winding $m completely!",vch,NULL,NULL,TO_ROOM);

             ch->ele_attack = ATTACK_EARTH;
             ch->attack_type = ATTACK_MAGICAL;
             ch->attack_var = BLUNT;
             ch->attack_focus = AERIAL;
             hurt_person( ch, vch, 1000000, TRUE );

             if ( vch->in_room == NULL || vch->position <= POS_STUNNED )
                count = 6;
          }
          else {
                  act("A smaller stone hits you, not winding you as bad.",vch,NULL,NULL,TO_CHAR);
                  act("A smaller stone hits $n, not winding $m as bad.",vch,NULL,NULL,TO_ROOM);

                  ch->ele_attack = ATTACK_EARTH;
                  ch->attack_type = ATTACK_MAGICAL;
                  ch->attack_var = BLUNT;
                  ch->attack_focus = AERIAL;
                  hurt_person( ch, vch, 500000, TRUE );

                  if ( vch->in_room == NULL || vch->position <= POS_STUNNED )
                     count = 6;
               }
     }
  }
}

void do_earthquake(CHAR_DATA *ch,char *argument)
{
  CHAR_DATA *vch, *vch_next;
  DESCRIPTOR_DATA *d;

  if ( IS_NPC(ch) )
     return;
    
  if ( !IS_CLASS(ch, CLASS_DUERGAR) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 300 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  act("You grunt and dig your fists into the #gearth#n",ch,NULL,NULL,TO_CHAR);
  act("$n grunts and digs $s fists into the #gearth#n",ch,NULL,NULL,TO_ROOM);
  act("With a huge roar you start to lift the earth up beneath everyone's feet!",ch,NULL,NULL,TO_CHAR);
  act("With a huge roar $n starts to lift the earth up beneath everyone's feet!",ch,NULL,NULL,TO_ROOM);
  act("Suddenly.. The #gearth#n #zcrackles#n beneath everyone.. UH OH!",ch,NULL,NULL,TO_CHAR);
  act("Suddenly.. The #gearth#n #zcrackles#n beneath everyone.. UH OH!",ch,NULL,NULL,TO_ROOM);
  act("You finally get your fists out of the #gearth#n and say '#yDon't make us dwarfs mad, we are STRONG!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n finally gets $s fists out of the #gearth#n and says '#yDon't make us dwarfs mad, we are STRONG!#n'",ch,NULL,NULL,TO_ROOM);
  act("The #gearth#n begins to crack underneath everyone's feet.. as there's a giant #gRUMBLE#n and #RSHAKE#n",ch,NULL,NULL,TO_CHAR);
  act("The #gearth#n begins to crack underneath everyone's feet.. as there's a giant #gRUMBLE#n and #RSHAKE#n",ch,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  subtract_stock( ch, 300 );
  SET_BIT( ch->arena, AFF2_DM );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
        continue;

     act("#gYou fall in between the cracks of earth and get squashed.. OUCH!#n",vch,NULL,NULL,TO_CHAR);
     act("#g$n falls in between the cracks of earth and gets squashed.. OUCH!#n",vch,NULL,NULL,TO_ROOM);

     ch->attack_type = ATTACK_PHYSICAL;
     ch->ele_attack = ATTACK_EARTH;
     ch->attack_var = BLUNT;
     ch->attack_focus = UPPERCUT;
     hurt_person( ch, vch, 4000000, TRUE );
  }

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
     if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
        {
           if ( ch->in_room->area != d->character->in_room->area )
              continue;

           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) || IS_CLASS( d->character, CLASS_DUERGAR ) )
              continue;

           act("#gYou fall in between the cracks of earth and get squashed.. OUCH!#n",d->character,NULL,NULL,TO_CHAR);
	     act("#g$n falls in between the cracks of earth and gets squashed.. OUCH!#n",d->character,NULL,NULL,TO_ROOM);

           ch->attack_type = ATTACK_PHYSICAL;
           ch->ele_attack = ATTACK_EARTH;
           ch->attack_var = BLUNT;
           ch->attack_focus = UPPERCUT;
           hurt_person( ch, d->character, 4000000, TRUE );
        }
   }

   if ( number_percent() > 74 && !IS_SET( ch->in_room->area->bits, MAGMA ) )
      {
         act("#gThe earthquake causes a #RVolcano#g to erupt!!#n",ch,NULL,NULL,TO_CHAR);
         act("#gThe earthquake causes a #RVolcano#g to erupt!!#n",ch,NULL,NULL,TO_ROOM);
	   act("#RHot magma spills everywhere as everyone frantically runs about.#n",ch,NULL,NULL,TO_CHAR);
         act("#RHot magma spills everywhere as everyone frantically runs about.#n",ch,NULL,NULL,TO_ROOM);
         SET_BIT( ch->in_room->area->bits, MAGMA );
      }
}
