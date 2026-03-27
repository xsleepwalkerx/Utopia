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

void do_snowyarmor( CHAR_DATA *ch, char *argument )
{
   char	 arg [MAX_INPUT_LENGTH];
   int cost = 1, vnum;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Syntax: snowyarmor <item>\n\r", ch );
         send_to_char( "items are: armor(1), leggings(1), sandals(1), bow (1), helmet (1), bracer(1),\n\r", ch );
         send_to_char( "           robe (1), gloves(1), ring(1), necklace (1), sword (1).\n\r", ch );
         return;
      }

   if ( !str_cmp(arg,"armor") )
      {
         vnum = 411;
      }
      else if ( !str_cmp(arg,"leggings") )
              {
                 vnum = 412;
              }
      else if ( !str_cmp(arg,"sandals") )
              {
                 vnum = 413;
              }
	else if ( !str_cmp(arg,"helmet") )
	        {
 	     	     vnum = 415;
		  }
	else if ( !str_cmp(arg,"bracer") )
	        {
     		     vnum = 416;
              }
	else if ( !str_cmp(arg,"robe") )
	        {
		     vnum = 417;
		  }
	else if ( !str_cmp(arg,"gloves") )
	        {
		     vnum = 418;
		  }
	else if ( !str_cmp(arg,"ring") )
		  {
		     vnum = 419;
		  }
	else if ( !str_cmp(arg,"necklace") )
		  {
		     vnum = 420;
	        }
	else if ( !str_cmp(arg,"bow") )
		  {
		     vnum = 423;
              }
	else if ( !str_cmp(arg, "sword") )
		  {
		     vnum = 422;
	}
      else {
              do_kodocreate(ch,"");
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
  act("You carve your $p from cattle.",ch,obj,NULL,TO_CHAR);
  act("$n carves $s $p from cattle and materialises in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_ice_arrow( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 700000;
  OBJ_DATA *obj;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a crossbow.\n\r", ch );
           return;
        }

  if ( ch->move < 250000 )
     {
	  send_to_char("You don't have enough energy to do this.\n\r",ch);
	  return;
     }

  one_argument( argument, arg);

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
	   send_to_char( "Whom do you wish to pummel with arrows?\n\r", ch );
	   return;
     }

  if ( ch->position == POS_STANDING )
     {
        if ( ( victim = get_char_area( ch, arg ) ) == NULL )
           {
             send_to_char( "They aren't here.\n\r", ch );
	       return;
           }
     }      

  if ( is_safe( ch, victim ) )
     return;

  if ( ch->in_room != victim->in_room && IS_SET( ch->in_room->room_flags, ROOM_BARRIER ) )
     {
        act("You can't attack other people from afar in this safe haven due to your honour.",ch,NULL,NULL,TO_CHAR);
        return;
     }

  ch->move -= 250000;
  ch->wait = 12;

  if ( IS_SET(ch->act, PLR_WIZINVIS) )
     REMOVE_BIT(ch->act, PLR_WIZINVIS);

  act("Your #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #CIcE#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #CIcE#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #CIcE#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #CIcE#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #CIcE#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #CIcE#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #CIcE#n arrows strike you hard!",ch,NULL,victim,TO_VICT);

  if ( number_percent() > 89 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
     victim->wait += 12;

  victim->agg += 20;
  ch->ele_attack = ATTACK_ICE;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = OVERHEAD;
  hurt_person(ch,victim,dam,TRUE);
}

void do_freezing_blast( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 700000;
  OBJ_DATA *obj;
 
  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a crossbow.\n\r", ch );
           return;
        }

  if ( ch->mana < 250000 )
     {
	  send_to_char("You don't have enough magical energy to do this.\n\r",ch);
	  return;
     }

  one_argument( argument, arg);

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you wish to pummel with arrows?\n\r", ch );
        return;
     }

  if ( ch->position == POS_STANDING )
     {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
     }

  if ( is_safe(ch,victim) )
     return;

  ch->mana -= 250000;
  ch->wait = 12;
   
  act("You load your bow with an arrow of FrEeZiNg cold ice.",ch,NULL,NULL,TO_CHAR);
  act("$n loads $s bow with an arrow of FrEeZiNg cold ice.",ch,NULL,NULL,TO_ROOM);
  act("Making sure everything is in place with great accuracy you aim and fire at $N.",ch,NULL,victim,TO_CHAR);
  act("Making sure everything is in place with great accuracy $n aims and fires at $N.",ch,NULL,victim,TO_NOTVICT);
  act("As your ice arrow impacts it encases $N in an ice shard completely #CFrEeZiNg#n all action.",ch,NULL,victim,TO_CHAR);
  act("As $n ice arrow impacts it encases $N in an ice shard completely #CFrEeZiNg#n all action.",ch,NULL,victim,TO_NOTVICT);
  act("As $n ice arrow impacts it encases you in an ice shard completely #CFrEeZiNg#n all your actions.",ch,NULL,victim,TO_VICT);
  act("As your ice arrow impacts it encases $N in an ice shard completely #CFrEeZiNg#n all action.",ch,NULL,victim,TO_CHAR);
  act("As $n ice arrow impacts it encases $N in an ice shard completely #CFrEeZiNg#n all action.",ch,NULL,victim,TO_NOTVICT);
  act("As $n ice arrow impacts it encases you in an ice shard completely #CFrEeZiNg#n all your actions.",ch,NULL,victim,TO_VICT);
  act("As your ice arrow impacts it encases $N in an ice shard completely #CFrEeZiNg#n all action.",ch,NULL,victim,TO_CHAR);
  act("As $n ice arrow impacts it encases $N in an ice shard completely #CFrEeZiNg#n all action.",ch,NULL,victim,TO_NOTVICT);
  act("As $n ice arrow impacts it encases you in an ice shard completely #CFrEeZiNg#n all your actions.",ch,NULL,victim,TO_VICT);

  if ( number_percent() > 89 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) && !IS_SET( victim->more, MORE_FROZEN ) )
     {
        SET_BIT( victim->more, MORE_FROZEN );
        victim->freeze_timer = 1;
     }

  if ( number_percent() > 70 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more, MORE_BLIND ) )
     {
        SET_BIT( victim->more, MORE_BLIND );
        victim->blind_timer = number_range( 2, 4 );
     }

  ch->ele_attack = ATTACK_ICE;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = LOW;
  hurt_person(ch,victim,dam,TRUE);
}

void do_slither_ice( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 700000;
  OBJ_DATA *obj;
 
  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a crossbow.\n\r", ch );
           return;
        }

  if ( ch->mana < 250000 )
     {
	  send_to_char("You don't have enough magical energy to do this.\n\r",ch);
	  return;
     }

  one_argument( argument, arg);

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
	   send_to_char( "Whom do you wish to pummel with arrows?\n\r", ch );
	   return;
     }

  if ( ch->position == POS_STANDING )
     {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
     }

  if ( is_safe(ch,victim) )
     return;

  if ( !IS_SET( victim->more2, MORE2_NOARMS ) && number_percent() > 49 - devil_luck( ch, victim, 10 ) )
     SET_BIT( victim->more2, MORE2_NOARMS );

  ch->mana -= 250000;
  ch->wait = 12;

  act("You softly load your bow with another arrow EnCaSeD in ice.",ch,NULL,NULL,TO_CHAR);
  act("$n softly loads $s bow with another arrow EnCaSeD in ice.",ch,NULL,NULL,TO_ROOM);
  act("You quietly take your spot in the distance away from your enemy and wait patiently.",ch,NULL,NULL,TO_CHAR);
  act("$n quietly takes $s spot in the distance away from $s enemy and waits patiently.",ch,NULL,NULL,TO_ROOM);
  act("You suddenly jump high into the air frightening all wildlife around you and ShOoT your arrow at $N!",ch,NULL,victim,TO_CHAR);
  act("$n suddenly jumps high into the air frightening all wildlife around $m and ShOoTs $s arrow at $N!",ch,NULL,victim,TO_NOTVICT);
  act("$n suddenly jumps high into the air frightening all wildlife around $m and ShOoTs $s arrow at you!",ch,NULL,victim,TO_VICT);
  act("You scream '#CSlither Ice!#n' and the arrow slithers like a snake towards $N!",ch,NULL,victim,TO_CHAR);
  act("$n screams '#CSlither Ice!#n' and the arrow slithers like a snake towards $N!",ch,NULL,victim,TO_NOTVICT);
  act("$n screams '#CSlither Ice!#n' and the arrow slithers like a snake towards you!",ch,NULL,victim,TO_VICT);
  act("Your arrow softly slithers towards $N and as it impacts it creates blocks of ice around $N's arms!",ch,NULL,victim,TO_CHAR);
  act("$n arrow softly slithers towards $N and as it impacts it creates blocks of ice around $N's arms!",ch,NULL,victim,TO_NOTVICT);
  act("$n arrow softly slithers towards you and as it impacts it creates blocks of ice around your arms!",ch,NULL,victim,TO_VICT);

  ch->ele_attack = ATTACK_ICE;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = DIRECT;
  hurt_person(ch,victim,dam,TRUE);
}

void do_ice_trap (CHAR_DATA *ch, char *argument)
{
     if ( IS_NPC(ch) )
        return;

     if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
     {
	  stc("Huh?\n\r",ch);
	  return;
     }

     if ( ch->fight_timer > 0 && ch->position != POS_FIGHTING )
        {
	     send_to_char("In the midst of battle, you can only do this in a fight currently.\n\r",ch);
	     return;
        }

    if ( IS_SET(ch->in_room->room_flags, ROOM_ICETRAP) )
       {
	    send_to_char("The room is already trapped.\n\r",ch);
	    return;
       }

    SET_BIT(ch->in_room->room_flags, ROOM_ICETRAP);

    act("You set up a trap to trap your foes",ch,NULL,NULL,TO_CHAR);
    act("$n sets up a trap to trap $s foes",ch,NULL,NULL,TO_ROOM);
}

void do_ambush( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
      {
         send_to_char( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->more2, MORE2_DIG ) )
	  {
	     argument = one_argument( argument, arg );

           if ( arg[0] == '\0' )
              {
                 REMOVE_BIT( ch->more2, MORE2_DIG );
                 act( "You quickly burst out of the ground as the snow around you scatters.", ch, NULL, NULL, TO_CHAR );
                 return;
              }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	   {
         send_to_char( "They aren't here.\n\r", ch );
         return;
       }

    if ( is_safe( ch, victim ) )
       return;

      REMOVE_BIT( ch->more2, MORE2_DIG );
	act("You jump out from the snow and slash $N ViCiOuSlY!",ch,NULL,victim,TO_CHAR);
	act("$n jumps out from the snow and slashes $N ViCiOuSlY!",ch,NULL,victim,TO_NOTVICT);
	act("$n jumps out from the snow and slashes you ViCiOuSlY!",ch,NULL,victim,TO_VICT);

      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = SLASH;
      ch->attack_focus = UPPERCUT;
      hurt_person(ch,victim,1000000,TRUE);

	if ( victim == NULL || victim->in_room == NULL || !IS_SET( victim->more, MORE_FROZEN ) )
         return;

	act("You unexpectedly attack $N like true #ylightning#n breaking the #Cice#n encasing!",ch,NULL,victim,TO_CHAR);
	act("$n unexpectedly attacks $N like true #ylightning#n breaking the #Cice#n encasing!",ch,NULL,victim,TO_NOTVICT);
	act("$n unexpectedly attacks you like true #ylightning#n breaking the #Cice#n encasing!",ch,NULL,victim,TO_VICT);

      ch->ele_attack = ATTACK_ICE;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = SLASH;
      ch->attack_focus = AERIAL;
	hurt_person(ch,victim,500000,TRUE);
	return;
	  }

   if ( ch->fight_timer > 0 )
	  {
	     stc("You cannot use this with a fight timer, elf\n\r", ch);
		 return;
	  }
  
   if ( weather_info.sky != SKY_SNOWY && weather_info.sky != SKY_ICESTORM )
	  {
		  stc("You cannot bury yourself in the snow WITHOUT snow!\n\r", ch);
		  return;
	  }

   SET_BIT( ch->more2, MORE2_DIG );
   act("You curl up throwing snow all over your body.",ch,NULL,NULL,TO_CHAR);
}

void do_ice_slice( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];
  int dam = 700000;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS (ch, CLASS_SNOW_ELF ) )
     {
	  send_to_char("Huh?\n\r", ch);
        return;
     }

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

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a sword.\n\r", ch );
           return;
        }

        if ( ch->move < 250000 )
           { 
              stc("You aren't atheletic enough for that.\n\r",ch);
              return;
           }

  ch->move -= 250000;
  ch->wait = 12;
  victim->move = UMAX( 0, victim->move - 250000 );

  act("You swing your sword as great stallectites of #Cice#n fly off it",ch,NULL,NULL,TO_CHAR);
  act("$n swings $s sword as great atallectites of #Cice#n fly off it",ch,NULL,NULL,TO_ROOM);
  act("You slice downwards onto $N causing $M to #CFrEeZe Up!#n",ch,NULL,victim,TO_CHAR);
  act("$n slices downwards onto $N causing $M to #CFrEeZe Up!#n",ch,NULL,victim,TO_NOTVICT);
  act("$n slices downwards onto you causing you to #CFrEeZe Up!#n",ch,NULL,victim,TO_VICT);
  act("You shiver as the #Lfreezing #Ccold#n hits you where it HuRtS!",victim,NULL,NULL,TO_CHAR);
  act("$n shivers as the #Lfreezing #Ccold#n hits $m where it HuRtS!",victim,NULL,NULL,TO_ROOM);

  if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) 
       && !IS_SET( victim->more, MORE_FROZEN ) )
     {
        stc("#LThe #Cfreezing #yenergy #Limmobilizes your body, preventing movement.#n\n\r",victim);
        act("#LThe #Cfreezing #yenergy #Limmobilizes $n#L's body, preventing movement.#n",victim,NULL,NULL,TO_ROOM);
        victim->freeze_timer = 1;
        SET_BIT( victim->more, MORE_FROZEN );
     }

  ch->attack_type = ATTACK_PHYSICAL;
  ch->ele_attack = ATTACK_ICE;
  ch->attack_var = SLASH;
  ch->attack_focus = OVERHEAD;
  hurt_person( ch, victim, dam, TRUE );
}

void do_ice_shield( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SNOW_ELF ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 15 )
	  {
          stc("You are not strong enough to know this yet, elf\n\r",ch);
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

   if ( IS_SET(ch->more2, MORE2_GUARD) )
      {
         stc("You are already surrounded by a shattering ice protection.\n\r",ch);
         return;
      }

   SET_BIT(ch->more2, MORE2_GUARD);
   ch->divine_timer = 10;

   act("#CYou surround your body with a #LICE#C covering#n.",ch,NULL,NULL,TO_CHAR);
   act("#C$n surrounds $s body with a #LICE#C covering.#n",ch,NULL,NULL,TO_ROOM);
}

void do_ice_punch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int direction;
    bool pre_room;
    int count;
    int attempt;
    int dam = 750000;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SNOW_ELF ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

	if ( ch->pkill < 18 )
	   {
          stc("You are not strong enough to know this yet, elf\n\r",ch);
          return;
       }

    if ( ch->fighting == NULL )
       {
          stc("Who do you wish to punch?\n\r",ch);
          return;
       }

	if ( arg[0] == '\0' )
       {
         send_to_char( "Whom do you wish to send flying?\n\r", ch );
         return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	   {
         send_to_char( "They aren't here.\n\r", ch );
         return;
       }

	if ( victim->fighting != ch || ch->fighting == victim )
	   {
		stc("They are not ganging you.\n\r",ch);
		return;
	   }

    for ( attempt = 0; attempt < 12; attempt++ )
  {
    EXIT_DATA *pexit;

    if ( direction > 6 )
       direction = attempt - 6;
       else direction = number_range( 0, 5 );

    if ( ( pexit = ch->in_room->exit[direction] ) == 0 || pexit->to_room == NULL
           || IS_SET( pexit->exit_info, EX_CLOSED ) || ( IS_NPC( victim )
           && IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB ) ) )
       {
          direction = -1;
          continue;
       }

    break;
  }

  if ( direction == -1 )
     {
        stc("There are no available exits for you to send your foe departing elf.\n\r", ch );
        return;
     }

    count = 0;

	act("You sheath your sword behind your back and draw your fists outward, you mean business!",ch,NULL,NULL,TO_CHAR);
	act("$n sheaths $s sword behind $s back and draws $s fists outward, $e means business!",ch,NULL,NULL,TO_ROOM);
	act("Ancient words flow from your mouth '#yToko jik mahari #LShiva#y mahit!#n'",ch,NULL,NULL,TO_CHAR);
	act("Ancient words flow from $n's mouth '#yToko jik mahari #LShiva#y mahit!#n'",ch,NULL,NULL,TO_ROOM);
	act("Suddenly you clench your fist and it #Renlights#n itself in #Lblue #Rflames#n of #Cice#n",ch,NULL,NULL,TO_CHAR);
	act("Suddenly $n clenches $s fist and it #Renlights#n itself in #Lblue #Rflames#n of #Cice#n",ch,NULL,NULL,TO_ROOM);
	act("You punch forwards causing the #Lblue #Rflames#n to spread as giant #Cstallactites#n form.",ch,NULL,NULL,TO_CHAR);
	act("$n punches fowards causing the #Lblue #Rflames#n to spread as giant #Cstallactites#n form.",ch,NULL,NULL,TO_ROOM);
	act("The entire ground #7CrAcKlEs#n as the #Cstallactites#n grow further and further out the ground.",ch,NULL,NULL,TO_CHAR);
	act("The entire ground #7CrAcKlEs#n as the #Cstallactites#n grow further and further out the ground.",ch,NULL,NULL,TO_ROOM);
	act("You mutter '#yIts been a blast.. But I think you out stayed your welcome.. tata!#n'",ch,NULL,NULL,TO_CHAR);
	act("$n mutters '#yIts been a blast.. But I think you out stayed your welcome.. tata!#n'",ch,NULL,NULL,TO_ROOM);
	act("You hear a sound underneath you as a #Cstallactite#n impales you and throws you into the next room!",victim,NULL,NULL,TO_CHAR);
      act("$n hears a sound underneath $m as a #Cstallactite#n impales $m and throws $m into the next room!",victim,NULL,NULL,TO_ROOM);

      if ( victim->blocking > 0 )
         {
            dam *= 2;
            victim->blocking = victim->block_meter = 0;
            act( "#eSuddenly a #yviolent #Rforce #erips your arms apart as you realize your #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_CHAR );
            act( "#eSuddenly a #yviolent #Rforce #erips $n#e's arms apart as $e realizes $s #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_ROOM );
            victim->noblock = UMAX( 1, victim->noblock );
         }

    ch->wait = 18;

    do
    {
       pre_room = special_slam( victim, direction );
       count++;
    }
    while ( count < 5 && pre_room != TRUE );

    ch->ele_attack = ATTACK_ICE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLUNT;
    ch->attack_focus = UPPERCUT;

    if ( pre_room == FALSE )
       {
		 act("You suddenly SmAsH against the wall and fall face first WiNdInG you SeVeReLy!",victim,NULL,NULL,TO_CHAR);
		 act("$n suddenly SmAsHeS against the wall and falls face first WiNdInG $m SeVeReLy!",victim,NULL,NULL,TO_ROOM);
		 hurt_person(ch,victim,dam * 3,TRUE);
       }

	else hurt_person(ch,victim,dam,TRUE);

    if ( victim == NULL || victim->position <= POS_STUNNED )
       return;

    stop_fighting( victim, TRUE );

    if ( victim->position > POS_STUNNED )                          
       victim->position = POS_STUNNED;
}

void do_ice_slash( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];
  int dam = 1000000;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS (ch, CLASS_SNOW_ELF ) )
     {
         send_to_char("Huh?\n\r", ch);
         return;
     }

  if ( ch->pkill < 20 )
      {
         send_to_char("You haven't trained hard enough yet, elf.\n\r",ch);
         return;
      }

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

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
           send_to_char( "You are not holding a sword.\n\r", ch );
           return;
        }

        if ( ch->move < 500000 )
           { 
              stc("You don't have enough magical energy for that.\n\r",ch);
              return;
           }

  ch->move -= 500000;

  act("Great colours of #7light#n emit from your sword as you slash downwards towards $N",ch,NULL,victim,TO_CHAR);
  act("Great colours of #7light#n emit from $n's sword as $e slashes downwards towards $N",ch,NULL,victim,TO_NOTVICT);
  act("Great colours of #7light#n emit from $n's sword as $e slashes downwards towards you",ch,NULL,victim,TO_VICT);
  act("You keep smashing away on $N, using your #Cice #7sword#n as a pick axe",ch,NULL,victim,TO_CHAR);
  act("$n keeps smashing away on $N, using $s #Cice #7sword#n as a pick axe",ch,NULL,victim,TO_NOTVICT);
  act("$n keeps smashing away on you, using $s #Cice #7sword#n as a pick axe",ch,NULL,victim,TO_VICT);
  act("You shriek as the #Cice #Rburns#n your #Lfreezing #Ccold#n body!",victim,NULL,NULL,TO_CHAR);
  act("$n shrieks as the #Cice #Rburns#n $s #Lfreezing #Ccold#n body!",victim,NULL,NULL,TO_ROOM);
  act("You struggle against the #Lcold #Cice#n rushing down your spine",victim,NULL,NULL,TO_CHAR);
  act("$n struggles against the #Lcold #Cice#n rushing down $s spine",victim,NULL,NULL,TO_ROOM);

  if ( IS_SET(victim->more, MORE_FROZEN) && number_percent() > 90 - devil_luck( ch, victim, 5 )
       && !IS_SET( victim->cmbt[5], SKL2_AMIND ) && ch->ctimer[15] == 0 )
     {
         act("You slam your sword down towards $N slicing the #Cice#n casing surrounding $M",ch,NULL,victim,TO_CHAR);
	   act("$n slams $s sword down towards $N slicing the #Cice#n casing surrounding $M",ch,NULL,victim,TO_NOTVICT);
         act("$n slams $s sword down towards you slicing the #Cice#n casing surrounding you",ch,NULL,victim,TO_VICT);
         act("You clutch your #Cicy #7sword#n to your chest and whisper '#yWelcome to death...#n'",ch,NULL,NULL,TO_CHAR);
         act("$n clutches $s #Cicy #7sword#n to $s chest and whispers '#yWelcome to death...#n'",ch,NULL,NULL,TO_ROOM);
         act("$n's #Cice#n casing shatters into a thousand pieces, killing $m!",victim,NULL,NULL,TO_ROOM);
         act("Your #Cice#n casing shatters into a thousand pieces, killing you!",victim,NULL,NULL,TO_CHAR);

         stop_fighting( victim, TRUE );
         victim->hit = -1000010;
         victim->position = POS_MORTAL;
         return;
     }

  ch->wait = 18;

  ch->ele_attack = ATTACK_ICE;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SLASH;
  ch->attack_focus = OVERHEAD;
  hurt_person( ch, victim, dam, TRUE );
}

void do_carpe_jugulum( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS (ch, CLASS_SNOW_ELF ) )
     {
        send_to_char("Huh?\n\r", ch);
        return;
     }

  if ( ch->pkill < 2 )
     {
        send_to_char("You aren't powerful enough to use this snow elf magic yet.\n\r",ch);
        return;
     }

  if ( ch->ctimer[1] > 0 )
     {
        stc("You are still recovering from the last one.\n\r",ch);
        return;
     }

  one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to silence?\n\r", ch );
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

        if ( ch->mana < 1000000 )
           { 
              stc("You don't have enough mana for that.\n\r",ch);
              return;
           }

		if ( IS_SET( victim->more3, MORE3_SILENCE ) )
		{
			stc("But they are already silenced elf.\n\r",ch);
			return;
		}

  ch->mana -= 1000000;

  act("You leap towards $N and grasp $S throat tightly.",ch,NULL,victim,TO_CHAR);
  act("$n leaps towards $N and graps $S throat tightly.",ch,NULL,victim,TO_NOTVICT);
  act("$n leaps towards you and grasps your throat tightly.",ch,NULL,victim,TO_VICT);
  act("Your hand changes to #ybright coloured #Cice#n.",ch,NULL,NULL,TO_CHAR);
  act("$n's hand changes to #ybright coloured #Cice#n.",ch,NULL,NULL,TO_ROOM);
  act("The #Cice#n from your hand spreads to $N's throat as $s neck turns #Lblue#n in #Rsevere #Ccold#n.",ch,NULL,victim,TO_CHAR);
  act("The #Cice#n from $n's hand spreads to $N's thorat as $s neck turns #Lblue#n in #Rsevere #Ccold#n.",ch,NULL,victim,TO_NOTVICT);
  act("The #Cice#n from $n's hand spreads to your throat as your neck turns #Lblue#n in #Rsevere #Ccold#n.",ch,NULL,victim,TO_VICT);
  act("You feel numb as the #Cice#n spreads to your tongue and you realise you can't speak!",victim,NULL,NULL,TO_CHAR);
  act("$n feels numb as the #Cice#n spreads to $s tongue and $e realises $e can't speak!",victim,NULL,NULL,TO_ROOM);
  ch->ctimer[1] = 6;

  SET_BIT( victim->more3, MORE3_SILENCE );
}

void do_ice_dust( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SNOW_ELF ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 4 )
      {
         send_to_char("You haven't trained hard enough yet, elf.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to give ice protection?\n\r", ch );
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
   
   if ( ( sn = skill_lookup( "ice dust" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person already has ice sparkles surrounding them.\n\r",ch);
         return;
      }

   if ( IS_NPC( victim ) )
      {
         stc("The snow elf is too proud to do this.\n\r",ch);
         return;
      }

   spell_idust( sn, 60, ch, victim );
}

void do_winter_swans( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
	 {
          send_to_char("Huh?\n\r",ch);
          return;
	 }

    if ( ch->pkill < 10 )
       {
          send_to_char("You aren't powerful enough to use this snow elf magic yet.\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

	if ( ch->mana < 500000 )
           { 
              stc("You don't have enough magic energy for that.\n\r",ch);
              return;
           }

	if ( weather_info.sky == SKY_SNOWY || weather_info.sky == SKY_ICESTORM )
	{
		stc("The #7swans#n return; it is already winter...\n\r",ch);
		return;
	}

	ch->mana -= 500000;

    act("You call out to #LShiva#n gently to lend her powers.",ch,NULL,NULL,TO_CHAR);
	act("$n calls out to #LShiva#n gently to lend her powers.",ch,NULL,NULL,TO_ROOM);
	act("#LShiva#n appears before you, sending out dozens of #7swans#n.",ch,NULL,NULL,TO_CHAR);
	act("#LShiva#n appears before $n, sending out dozens of #7swans#n",ch,NULL,NULL,TO_ROOM);
	act("#LShiva#n says '#yGo now, #CChildren#n #yof the #7Loom#n'",ch,NULL,NULL,TO_CHAR);
	act("#LShiva#n says '#yGo now, #CChildren#n #yof the #7Loom#n'",ch,NULL,NULL,TO_ROOM);
	act("Led by #7Bobbin Threadbare#n and his mother the #7swans#n hastily fly away..",ch,NULL,NULL,TO_CHAR);
	act("Led by #7Bobbin Threadbare#n and his mother the #7swans#n hastily fly away..",ch,NULL,NULL,TO_ROOM);
    ch->ctimer[0] = 12;

    weather_info.sky = SKY_SNOWY;

    if ( IS_SET( ch->in_room->area->bits, MAGMA ) )
       REMOVE_BIT( ch->in_room->area->bits, MAGMA );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->character == NULL )
          continue;

       stc( "#7Swans#n appear tearing a rift in #0space#n.\n\r", d->character );

       if ( d->character != ch )
          stc( "Suddenly #CWinter#n appears...\n\r", d->character );
	}

    ch->wait = 12;

	if ( number_percent() > 49 )
	{
	   sta("The #7snow#n worsens as an #CIcestorm#n brews...#n\n\r", ch->in_room->area );
	   weather_info.sky = SKY_ICESTORM;
	}
}

void do_multi_arrow( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a crossbow.\n\r", ch );
            return;
         }

   one_argument( argument, arg);

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to pummel with arrows?\n\r", ch );
         return;
      }
 
   if ( ch->position == POS_STANDING )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
      } 

   if ( is_safe(ch,victim) )
      return;

   ch->wait = 8;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You draw back on your bow and scout for your victim as you hold five arrows in your hand.",ch,NULL,NULL,TO_CHAR);
   act("$n draws back on $s bow holding five arrows.",ch,NULL,NULL,TO_ROOM);
   act("You carefully place all the arrows onto your bow and take aim...",ch,NULL,NULL,TO_CHAR);
   act("$n loads $s bow with all five arrows and takes aim...",ch,NULL,NULL,TO_ROOM);
   act("You draw back and FiRe your arrows as you shriek '#yMulti arrow!!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n draws back and FiReS $s arrows as $n shrieks '#YMulti arrow!!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You watch carefully as your arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
   act("$n watches carefully as $s arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
   act("#eYou move like a blur as an image of you draws back FiRiNg more arrows towards $N!#n",ch,NULL,victim,TO_CHAR);
   act("#e$n moves like a blur as an image of $m draws back FiRiNg more arrows towards you!#n",ch,NULL,victim,TO_VICT);
   act("#e$n moves like a blur as an image of $m draws back FiRiNg more arrows towards $N!#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYou move like a blur as an image of you draws back FiRiNg more arrows towards $N!#n",ch,NULL,victim,TO_CHAR);
   act("#e$n moves like a blur as an image of $m draws back FiRiNg more arrows towards you!#n",ch,NULL,victim,TO_VICT);
   act("#e$n moves like a blur as an image of $m draws back FiRiNg more arrows towards $N!#n",ch,NULL,victim,TO_NOTVICT);
   act("#eYou move like a blur as an image of you draws back FiRiNg more arrows towards $N!#n",ch,NULL,victim,TO_CHAR);
   act("#e$n moves like a blur as an image of $m draws back FiRiNg more arrows towards you!#n",ch,NULL,victim,TO_VICT);
   act("#e$n moves like a blur as an image of $m draws back FiRiNg more arrows towards $N!#n",ch,NULL,victim,TO_NOTVICT);
   act("You yelp in pain as a barrage of arrows pierces all over your body.",victim,NULL,NULL,TO_CHAR);
   act("$n yelps in pain as a barrage of arrows pierces all over $s body.",victim,NULL,NULL,TO_ROOM);

   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_arrow_drain( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a crossbow.\n\r", ch );
            return;
         }

   one_argument( argument, arg);

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to drain?\n\r", ch );
         return;
      }
 
   if ( ch->position == POS_STANDING )
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
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You carefully add your saliva to your arrow as you place it carefully on your bow.",ch,NULL,NULL,TO_CHAR);
   act("$n licks $s arrow and places it on $s bow.",ch,NULL,NULL,TO_ROOM);
   act("You draw back, accurately take your aim and fire, you whisper softly like a mantra '#PA#Rr#Pr#Ro#Pw #RD#Pr#Ra#Pi#Rn#n...'",ch,NULL,NULL,TO_CHAR);
   act("$n draws back, takes $s aim and shoots. $e whispers softly '#PA#Rr#Pr#Ro#Pw #RD#Pr#Ra#Pi#Rn#n...'",ch,NULL,NULL,TO_ROOM);
   act("You silently celebrate as your arrow strike $N hard and saps them of vital life.",ch,NULL,victim,TO_CHAR);
   act("$n watches carefully as $s arrows strike $N hard.",ch,NULL,victim,TO_NOTVICT);
   act("You examine the arrow placed inside of you as it starts to pulsate draining you of life.",ch,NULL,victim,TO_VICT);
   act("You watch the pulsating arrow stuck inside of $N and feel healthier as energy is transferred to you.",ch,NULL,victim,TO_CHAR);
   act("$n looks revitalized as the pulsating arrow transfers energy.",ch,NULL,NULL,TO_ROOM);

   hit_gain( ch, 1000000 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 2000000, TRUE );
}

void do_demonic_shot( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SNOW_ELF) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a crossbow.\n\r", ch );
            return;
         }

   one_argument( argument, arg);

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to pummel with arrows?\n\r", ch );
         return;
      }

   if ( ch->position == POS_STANDING )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
      }

   if ( is_safe(ch,victim) )
      return;

   act("You look around grinning as you slide a fire, ice and poison arrow onto your bow.",ch,NULL,NULL,TO_CHAR);
   act("$n beams a smile as they arm $s bow with three different arrows.",ch,NULL,NULL,TO_ROOM);
   act("Carefully you eye your opponent and with great accuracy you let off your three arrows.",ch,NULL,NULL,TO_CHAR);
   act("$n eyes $s opponent and lets off three arrows.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#rDemonic Shot!#n' as the arrows strike $N with great precision!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#rDemonic Shot!#n' as the arrows strike $N with great precision!",ch,NULL,victim,TO_NOTVICT);
   act("$n yells '#rDemonic Shot!#n' as the arrows strike you with great precision!",ch,NULL,victim,TO_VICT);
   act("$N screams in ViCiOUs pain as #g#zpoison#n seeps into $S system!!!",ch,NULL,victim,TO_CHAR);
   act("$N screams in ViCiOUs pain as #g#zpoison#n seeps into $S system!!!",ch,NULL,victim,TO_NOTVICT);
   act("You scream in ViCiOUs pain as #g#zpoison#n seeps into your system!!!",ch,NULL,victim,TO_VICT);
   act("$N screams in ViCiOUs pain as #gpoison#n seeps into $S system!!!",ch,NULL,victim,TO_CHAR);
   act("$N screams in ViCiOUs pain as #gpoison#n seeps into $S system!!!",ch,NULL,victim,TO_NOTVICT);
   act("You scream in ViCiOUs pain as #gpoison#n seeps into your system!!!",ch,NULL,victim,TO_VICT);
   act("$N screams in ViCiOUs pain as #g#zpoison#n seeps into $S system!!!",ch,NULL,victim,TO_CHAR);
   act("$N screams in ViCiOUs pain as #g#zpoison#n seeps into $S system!!!",ch,NULL,victim,TO_NOTVICT);
   act("You scream in ViCiOUs pain as #g#zpoison#n seeps into your system!!!",ch,NULL,victim,TO_VICT);
   act("$N twirls around is suddenly STrUcK by a #r#zfire#n arrow!!",ch,NULL,victim,TO_CHAR);
   act("$N twirls around is suddenly STrUcK by a #r#zfire#n arrow!!",ch,NULL,victim,TO_NOTVICT);
   act("You twirl around and are suddenly STrUcK by a #r#zfire#n arrow!!",ch,NULL,victim,TO_VICT);
   act("$N twirls around is suddenly STrUcK by a #rfire#n arrow!!",ch,NULL,victim,TO_CHAR);
   act("$N twirls around is suddenly STrUcK by a #rfire#n arrow!!",ch,NULL,victim,TO_NOTVICT);
   act("You twirl around and are suddenly STrUcK by a #rfire#n arrow!!",ch,NULL,victim,TO_VICT);
   act("$N twirls around is suddenly STrUcK by a #r#zfire#n arrow!!",ch,NULL,victim,TO_CHAR);
   act("$N twirls around is suddenly STrUcK by a #r#zfire#n arrow!!",ch,NULL,victim,TO_NOTVICT);
   act("You twirl around and are suddenly STrUcK by a #r#zfire#n arrow!!",ch,NULL,victim,TO_VICT);
   act("Your final arrow hits $N and $S is suddenly ENcASeD in freezing cold #C#zice#n!!",ch,NULL,victim,TO_CHAR);
   act("$n final arrow hits $N and $S is suddenly ENcASeD in freezing cold #C#zice#n!!",ch,NULL,victim,TO_NOTVICT);
   act("$n final arrow hits you and you are suddenly ENcASeD in freezing cold #C#zice#n!!",ch,NULL,victim,TO_VICT);
   act("Your final arrow hits $N and $S is suddenly ENcASeD in freezing cold #Cice#n!!",ch,NULL,victim,TO_CHAR);
   act("$n final arrow hits $N and $S is suddenly ENcASeD in freezing cold #Cice#n!!",ch,NULL,victim,TO_NOTVICT);
   act("$n final arrow hits you and you are suddenly ENcASeD in freezing cold #Cice#n!!",ch,NULL,victim,TO_VICT);
   act("Your final arrow hits $N and $S is suddenly ENcASeD in freezing cold #C#zice#n!!",ch,NULL,victim,TO_CHAR);
   act("$n final arrow hits $N and $S is suddenly ENcASeD in freezing cold #Cice#n!!",ch,NULL,victim,TO_NOTVICT);
   act("$n final arrow hits you and you are suddenly ENcASeD in freezing cold #C#zice#n!!",ch,NULL,victim,TO_VICT);
   act("You watch as the #Cice#n shatters around $N.",ch,NULL,victim,TO_CHAR);
   act("$n stares intently as #Cice#m shatters around $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n stares intently as #Cice#m shatters around you.",ch,NULL,victim,TO_VICT);
   act("You grin evilly as the #rfire#n spreads amongst $N body.",ch,NULL,victim,TO_CHAR);
   act("$n grins as the #rfire#n spreads amongst $N body.",ch,NULL,victim,TO_NOTVICT);
   act("$n grins as the #rfire#n spreads amongst your body.",ch,NULL,victim,TO_VICT);
   act("You cackle as $N screams in pain from the #gpoison#n spreading...",ch,NULL,victim,TO_CHAR);
   act("$n cackles wildly as $N screams from all the #gpoison#n spreading...",ch,NULL,victim,TO_NOTVICT);
   act("$n cackles wildly as you scream from all the #gpoison#n spreading inside you...",ch,NULL,victim,TO_VICT);

   ch->wait = 6;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   victim->agg = 100;

   SET_BIT( victim->more, MORE_POISON );

   if ( IS_CLASS( victim, CLASS_DUERGAR ) )
      victim->hit = UMIN( victim->max_hit, victim->hit + ( 1000000 * UMIN( 1, victim->pkill / 10 ) ) );
      else {
              ch->attack_type = ATTACK_PHYSICAL;
              ch->attack_var = PIERCE;
              ch->attack_focus = DIRECT;
              hurt_person( ch, victim, dam, TRUE );
           }

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam, TRUE );

   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, dam + 500000, TRUE );
}
