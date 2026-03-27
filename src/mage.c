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

void do_reveal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  int cost = 500000;

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_WISP) && !IS_CLASS(ch, CLASS_WIZARD ))
     {
        send_to_char("Huh?!?.\n\r",ch);
        return;
     }

  if ( IS_CLASS(ch, CLASS_WISP) && ch->pkill < 2 )
	 {
	    stc("You're not strong enough to know this technique, wisp.\n\r",ch);
		return;
	 }

  if ( ch->mana < cost )
  {
    send_to_char("You don't have the mystical energies to do this.\n\r",ch);
    return;
  } 

  if ( ch->fight_timer > 0 )
     {
        stc( "You are too distracted to perform this delicate technique.\n\r", ch );
        return;
     }

  act( "$n mumles a few words, and you are suddenly blinded by a flash.", ch, NULL, NULL, TO_ROOM );   
  send_to_char( "You reveal everything hidden in the room.\n\r", ch );

  if ( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
     {
         REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
         ch->in_room->drows = 0;
     }

  for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
  {
    if ( ich->trust > 6 ) 
       continue;

    affect_strip ( ich, gsn_invis );
    affect_strip ( ich, gsn_mass_invis );
    affect_strip ( ich, gsn_sneak );
    if (IS_SET(ich->affected_by, AFF_HIDE))      REMOVE_BIT(ich->affected_by, AFF_HIDE);
    if (IS_SET(ich->affected_by, AFF_INVISIBLE)) REMOVE_BIT(ich->affected_by, AFF_INVISIBLE);
    if (IS_SET(ich->affected_by, AFF_SNEAK))     REMOVE_BIT(ich->affected_by, AFF_SNEAK);
    if (IS_SET(ich->act, PLR_WIZINVIS))          REMOVE_BIT(ich->act, PLR_WIZINVIS);
    if (IS_SET(ich->act, AFF_HIDE))              REMOVE_BIT(ich->act, AFF_HIDE);

    if ( IS_SET(ich->newbits, NEW_DARKNESS) )
       REMOVE_BIT(ich->newbits, NEW_DARKNESS);
 
    send_to_char("You are suddenly very visibly.\n\r",ich);
  }

  ch->mana -= cost;
}

void do_magearmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) && !IS_CLASS(ch, CLASS_NECROMANCER) )
     {
        send_to_char("What?\n\r",ch);
        return;
     }

  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of mage armor you wish to make: Dagger Staff Ring Collar Robe Cap Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }

  if ( ch->gold < 1 )
  {
    send_to_char("It costs #w1 #ygold piece#n to create mage equipment.\n\r",ch);
    return;
  }

  if (!str_cmp(arg,"mask")) vnum = 296;
  else if (!str_cmp(arg,"belt")) vnum = 294;
  else if (!str_cmp(arg,"cape")) vnum = 293;
  else if (!str_cmp(arg,"ring")) vnum = 285;
  else if (!str_cmp(arg,"collar")) vnum = 286;
  else if (!str_cmp(arg,"robe")) vnum = 287;
  else if (!str_cmp(arg,"cap")) vnum = 288;
  else if (!str_cmp(arg,"leggings")) vnum = 289;
  else if (!str_cmp(arg,"boots")) vnum = 290;
  else if (!str_cmp(arg,"gloves")) vnum = 291;
  else if (!str_cmp(arg,"sleeves")) vnum = 292;
  else if (!str_cmp(arg,"bracer")) vnum = 295;
  else if (!str_cmp(arg,"staff")) vnum = 284;
  else if (!str_cmp(arg,"dagger")) vnum = 283;
  else
  {
    do_magearmor(ch,"");
    return;
  }

  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
  }

  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  ch->gold -= 1;
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  return;
}

void do_telekinetics( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) ) return;
 
  if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
  {
    stc("You relax your mental energies.\n\r",ch);
    REMOVE_BIT(ch->mflags, MAGE_TELEKINETICS);
    return;
  }
  else
  {
    stc("You focus your mental energies into your attacks.\n\r",ch);
    SET_BIT(ch->mflags, MAGE_TELEKINETICS);
    return;
  }
}

void do_layhands( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int i,test=0;
  int bonus = 0;

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) ) return;

  victim = ch;

  for( i = 0; i < 7; i++)
  {
    if ( victim->loc_hp[i] > 0 )
      test++;
  }

  if ( victim->hit == victim->max_hit && victim->mana == victim->max_mana && victim->move == victim->max_move &&
       test == 0 )
  {
    stc("You are in perfect condition!\n\r",ch);
    return;
  }

  if (victim->loc_hp[6] > 0)
  {
    stc("Your wounds close up and stop bleeding.\n\r",victim);
    victim->loc_hp[6] = 0;
  }
  
  if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + victim->loc_hp[3] + victim->loc_hp[4] +
victim->loc_hp[5]) != 0)
  {
    stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",victim);
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
  }

  stc("A warm feeling spreads through your body.\n\r",victim);

  if ( victim->fight_timer == 0 && !IS_NPC(victim) )
     {
        bonus = 5000000;

        victim->hit = UMIN( victim->max_hit, victim->hit + bonus );
        victim->mana = UMIN( victim->max_mana, victim->mana + bonus );
        victim->move = UMIN( victim->max_move, victim->move + bonus );
     }
     else {
              hit_gain( victim, 300000 );
              mana_gain( victim, 500000 );
              move_gain( victim, 500000 );
          }

  ch->wait = 12;

  if ( victim != ch )
     {
        act("You lay your hands on $N and rejuvenate $M.",ch,NULL,victim,TO_CHAR);
        act("$n lays $s hands on $N and rejuvenates $M.",ch,NULL,victim,TO_NOTVICT);
        act("$n lays $s hands on you and rejuvenates $M.",ch,NULL,victim,TO_VICT);
        return;
     }

  act("You lay your hands on yourself as a tingling sensation fills you.",ch,NULL,NULL,TO_CHAR);
  act("$n lays $s hands on $mself as a tingling sensation fills $m.",ch,NULL,NULL,TO_ROOM);
}

void do_haste( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Cast Haste on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->mana < 500000 )
    {
	send_to_char( "You don't have enough magical energy.\n\r", ch );
	return;
    }

    spell_haste(skill_lookup("haste"), 30, ch, victim);
    ch->mana -= 500000;
}

void do_slow( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	   send_to_char( "Cast Slow on whom?\n\r", ch );
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

    if ( ch->mana < 500000 )
    {
	send_to_char( "You don't have enough magical energy.\n\r", ch );
	return;
    }

    spell_slow(skill_lookup("slow"), 30, ch, victim);
    ch->mana -= 500000;
}

void do_quicken( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 3 && ch->clannum != 6 ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ch->pkill < 5 )
       {
          send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
          return;
       }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Cast Quicken on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->mana < 1000000 )
    {
	send_to_char( "You don't have enough magical energy.\n\r", ch );
	return;
    }

    spell_quicken(skill_lookup("quicken"), 30, ch, victim);
    ch->mana -= 1000000;
    ch->wait = 12;
}

void do_age( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 3 && ch->clannum != 6 ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( get_stock( ch ) < 300 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Cast age on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    ch->wait = 12;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 300 );

    spell_age( skill_lookup("age"), 30, ch, victim );
}

void do_timerlock( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->cmbt[7], SKL_TLOCK ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Place a Timer Lock on whom?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( IS_CLASS( victim, CLASS_WIZARD ) && victim->clannum != 3 )
       {
          send_to_char( "They are immuned to this technique.\n\r", ch );
          return;
       }

    if ( IS_CLASS( victim, CLASS_UNDEAD_KNIGHT ) )
       {
          send_to_char( "They are immuned to this technique.\n\r", ch );
          return;
       }

    if ( IS_SET( victim->extra, EXTRA_TLOCK ) )
       {
          send_to_char( "They are already affected by Timer Lock.\n\r", ch );
          return;
       }

    SET_BIT( victim->extra, EXTRA_TLOCK );
    act( "You suddenly dash towards $N as you grasp $M by the throat!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly dashes towards you as $e grasps you by the throat!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly dashes towards $N as $e grasps $M by the throat!", ch, NULL, victim, TO_NOTVICT );
    act( "Before $N can recover, you quickly mumble an ancient chant...", ch, NULL, victim, TO_CHAR );
    act( "Before you can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_VICT );
    act( "Before $N can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_NOTVICT );
    act( "It feels time itself has leave you, making it much harder to do anything.", victim, NULL, NULL, TO_CHAR);
    act( "It feels time itself has leave $n, making it much harder for $m to do anything.", victim, NULL, NULL, TO_ROOM);
}

void do_ltouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->cmbt[5], SKL2_LTOUCH ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Place Leeching Spirits on whom?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( IS_SET( victim->extra, EXTRA_LTOUCH ) )
       {
          send_to_char( "They are already affected by Leeching Spirits.\n\r", ch );
          return;
       }

    SET_BIT( victim->extra, EXTRA_LTOUCH );
    act( "You suddenly dash towards $N as you grasp $M by the throat!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly dashes towards you as $e grasps you by the throat!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly dashes towards $N as $e grasps $M by the throat!", ch, NULL, victim, TO_NOTVICT );
    act( "Before $N can recover, you quickly mumble an ancient chant...", ch, NULL, victim, TO_CHAR );
    act( "Before you can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_VICT );
    act( "Before $N can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_NOTVICT );
    act( "Suddenly spirits leech themselves onto your soul, dampening your healing abilities.", victim, NULL, NULL, TO_CHAR);
    act( "Suddenly spirits leech themselves onto $n's soul, dampening $s healing abilities.", victim, NULL, NULL, TO_ROOM);
}

void do_fbite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->cmbt[7], SKL_FBITE ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Place a frostbite on whom?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( IS_SET( victim->extra, EXTRA_FBITE ) )
       {
          send_to_char( "They are already affected by Leeching Spirits.\n\r", ch );
          return;
       }

    if ( !IS_SET( victim->cmbt[5], SKL2_THEART ) && !is_martial( victim ) && victim->freeze_timer == 0 )
       {
          SET_BIT( victim->more, MORE_FROZEN );
          victim->freeze_timer = 1;
       }

    SET_BIT( victim->extra, EXTRA_FBITE );
    act( "You suddenly dash towards $N as you grasp $M by the throat!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly dashes towards you as $e grasps you by the throat!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly dashes towards $N as $e grasps $M by the throat!", ch, NULL, victim, TO_NOTVICT );
    act( "Before $N can recover, you quickly mumble an ancient chant...", ch, NULL, victim, TO_CHAR );
    act( "Before you can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_VICT );
    act( "Before $N can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_NOTVICT );
    act( "Suddenly your toes feel clingy as a haunting chill shivers your body.", victim, NULL, NULL, TO_CHAR);
    act( "Suddenly $n's toes feel clingy as a haunting chill shivers $s body.", victim, NULL, NULL, TO_ROOM);
}

void do_subtle_pinch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_SET( ch->cmbt[5], SKL2_SPINCH ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Place Speed Down on whom?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( IS_SET( victim->cmbt[5], SKL2_RBODY ) )
       {
          send_to_char( "They easily dodge your technique.\n\r", ch );
          return;
       }

    if ( IS_SET( victim->more2, MORE2_SPINCH ) )
       {
          send_to_char( "They are already affected by Subtle Pinch.\n\r", ch );
          return;
       }

    SET_BIT( victim->more2, MORE2_SPINCH );
    act( "You suddenly dash towards $N as you grasp $M by the throat!", ch, NULL, victim, TO_CHAR );
    act( "$n suddenly dashes towards you as $e grasps you by the throat!", ch, NULL, victim, TO_VICT );
    act( "$n suddenly dashes towards $N as $e grasps $M by the throat!", ch, NULL, victim, TO_NOTVICT );
    act( "Before $N can recover, you quickly mumble an ancient chant...", ch, NULL, victim, TO_CHAR );
    act( "Before you can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_VICT );
    act( "Before $N can recover, $n quickly mumbles an ancient chant...", ch, NULL, victim, TO_NOTVICT );
    act( "Suddenly you feel less inclined to dish out your anger.", victim, NULL, NULL, TO_CHAR);
    act( "Suddenly $n feels less inclined to dish out $s anger.", victim, NULL, NULL, TO_ROOM);
}

void do_evocate(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ch->pkill < 5 )
       {
          send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
          return;
       }

    if ( ch->mana < 500000 )
    {
	send_to_char( "You don't have enough magical energy.\n\r", ch );
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to evocate?\n\r", ch);
	return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
	send_to_char("You are not carrying that item.\n\r", ch);
	return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That is not a weapon!\n\r", ch);
	return;
    }

    if (IS_WEAP(obj, WEAPON_EVOCATE))
    {
	act("$p is already encircled by greenish mystical flames.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("$p shimmers violently as swirling, green flames encircles it.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n, shimmers violently as swirling, green flames encircles it.", ch, obj, NULL, TO_ROOM);

    ch->mana -= 500000;
    WAIT_STATE(ch, 12);
    SET_BIT(obj->weapflags, WEAPON_EVOCATE);
}

void do_majorenchant( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg1[MAX_INPUT_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int number = 0;
  
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( arg1[0] == '\0' )
  {
    stc("What object do you wish to enchant?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
  {
    stc("You are not carrying that object.\n\r",ch);
    return;
  }

  if ( ch->gold < 2 )
  {
    stc("You don't have enough #ygold pieces#n.\n\r",ch);
    return;
  }

  if ( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR )
  {
    act("$p is not a weapon or an armor.",ch,obj,NULL,TO_CHAR);
    return;
  }

  if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
  {
    act("$p is too powerful for your quintessence.",ch,obj,NULL,TO_CHAR);
    return;
  }

  if ( !str_cmp(arg, "speed") )
     number = 17;
     else if ( !str_cmp(arg, "vanish") )
             number = 24;
     else if ( !str_cmp(arg, "curse") )
             {
                 if ( obj->item_type != ITEM_WEAPON )
                    {
                       stc("You can only put curse on weapons.\n\r",ch);
                       return;
                    }

                 act("$p glows #y#zradiantly#n as you release a power onto it.",ch,obj,NULL,TO_CHAR);
                 act("$p glows #g#zbrilliantly#n.",ch,obj,NULL,TO_ROOM);
                 obj->value[0] = 24;

                 return;
             }
     else if ( !str_cmp(arg, "fly") )
             {
                 if ( obj->item_type != ITEM_ARMOR )
                    {
                       stc("You can only put fly on armor.\n\r",ch);
                       return;
                    }

                 act("$p glows #y#zradiantly#n as you release a power onto it.",ch,obj,NULL,TO_CHAR);
                 act("$p glows #g#zbrilliantly#n.",ch,obj,NULL,TO_ROOM);
                 obj->value[3] = 3;

                 return;
             }
     else if ( !str_cmp(arg, "sneak") )
             {
                 if ( obj->item_type != ITEM_ARMOR )
                    {
                       stc("You can only put fly on armor.\n\r",ch);
                       return;
                    }

                 act("$p glows #y#zradiantly#n as you release a power onto it.",ch,obj,NULL,TO_CHAR);
                 act("$p glows #g#zbrilliantly#n.",ch,obj,NULL,TO_ROOM);
                 obj->value[3] = 9;

                 return;
             }
             else {
                     stc("Valid Choices: speed, vanish, curse, fly, sneak.\n\r",ch);
                     return;
                  }

  act("$p glows #y#zradiantly#n as you release a power onto it.",ch,obj,NULL,TO_CHAR);
  act("$p glows #g#zbrilliantly#n.",ch,obj,NULL,TO_ROOM);

  if ( obj->item_type == ITEM_WEAPON )
     obj->value[0] = number * 1000;
     else obj->value[3] = number;

  ch->gold -= 2;
}

void do_summon( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    CHAR_DATA *opponent;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( ch->fighting == NULL )
       {
	    send_to_char("But you aren't in a fight.\n\r",ch);
          return;
       }

    ch->wait = 12;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 200 );

    victim = create_mobile( get_mob_index( 40 ) );
    victim->sex = ch->sex;
    victim->level = 980;
    victim->armor = -100000;
    victim->hitroll = 2500000;
    victim->damroll = 2500000;
    victim->hit = victim->max_hit = 5000000;
    victim->mana = victim->max_mana = 15000;
    victim->move = victim->max_move = 15000;

    victim->practice = 2;
    SET_BIT( victim->act, ACT_TIMER );
    SET_BIT( victim->act, ACT_NINJA );

    if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
       SET_BIT( victim->affected_by, AFF_ETHEREAL );

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
       SET_BIT( victim->affected_by, AFF_SHADOWPLANE );

    af.type      = skill_lookup("dark shield");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    char_to_room( victim, ch->in_room );
    opponent = ch->fighting;

    act( "You suddenly mumble arabric incantations as you swirl your hand in a circular motion.", ch, NULL, NULL, TO_CHAR );
    act( "$n suddenly mumbles arabric incantations as $e swirls $s hand in a circular motion.", ch, NULL, NULL, TO_ROOM );
    act( "A shimmering portal appears in front of you as a huge barbed wyrm slides out of it!", ch, NULL, NULL, TO_CHAR );
    act( "A shimmering portal appears in front of you as a huge barbed wyrm slides out of it!", ch, NULL, NULL, TO_ROOM );
    act( "Grinning evilly, you point at $N as the Barbed Wyrm attacks $M!", ch, NULL, opponent, TO_CHAR );
    act( "Grinning evilly, $n points at you as the Barbed Wyrm attacks!", ch, NULL, opponent, TO_VICT );
    act( "Grinning evilly, $n points at $N as the Barbed Wyrm attacks $M!", ch, NULL, opponent, TO_NOTVICT );

    stop_fighting( opponent, TRUE );
    set_fighting( victim, opponent );
    set_fighting( opponent, victim );

    stop_fighting( ch, TRUE );
    multi_hit( ch, opponent, TYPE_UNDEFINED );

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = HIT;
    ch->attack_focus = DIRECT;
    hurt_person( ch, opponent, 2000000, TRUE );
}

void do_anti_magic( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int test = 0;
    int test2 = 0;
    AFFECT_DATA *affect_next, *affect;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( !IS_NPC(ch) )
       {
          if ( ch->mana < ch->max_mana / 3 )
             {
                stc("You don't have enough magical energy for that.\n\r",ch);
                return;
             }   
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Dispel whom?\n", ch );
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

    if ( nogang != 10 )
       {
          if ( victim->gshield > 0 )
             victim->gshield = 0;
       }

    if ( is_safe(ch,victim) )
       return;

    if ( is_affected(victim, skill_lookup("gods disfavor") ) )
       {
          send_to_char("God refuses to allow anything to disrupt this curse.\n\r",ch);
	    return;
       }

    if ( is_affected( victim, skill_lookup("blessing of the saints") ) )
       {
          send_to_char("They are protected by the holy saints.\n\r", ch );
          return;
       }

    if ( IS_CLASS(victim, CLASS_WIZARD) )
       {
          stc("Their mind is too strong.\n\r",ch);
          return;
       }

    if ( !IS_NPC(ch) )
       {
          ch->wait = 6;
          ch->mana -= ( ch->max_mana / 3 );
       }

    if ( IS_SET(victim->affected_by, AFF_POLYMORPH) )
       test = 1;

    if ( IS_SET(victim->monkstuff, MONK_HEAL) )
       REMOVE_BIT(victim->monkstuff, MONK_HEAL);

    if ( IS_AFFECTED(victim, AFF_CHARM) )
       test2 = 1;

    affect = victim->affected;

    while ( affect )
    {
       affect_next = affect->next;

       if ( affect->duration != -1 )
          {
             affect_remove( victim, affect );
             victim->wait++;
          }

       affect = affect_next;
    }

    act("You dischard all magical affects from $N.",ch,NULL,victim,TO_CHAR);
    act("$n dischards all magical affects from $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n dischards all magical affects from you.",ch,NULL,victim,TO_VICT);

    if ( IS_SET( victim->more, MORE_BARRIER ) )
       REMOVE_BIT( victim->more, MORE_BARRIER );

    if ( victim->binded != NULL && IS_CLASS( victim->binded, CLASS_ANGEL ) )
       do_gspirit( victim->binding, "" );
       else if ( victim->binding != NULL && IS_CLASS( victim, CLASS_ANGEL ) )
               do_gspirit( victim, "" );

    if ( test == 1 )
	 SET_BIT(victim->affected_by, AFF_POLYMORPH);

    if ( test2 == 1 )
       {
          AFFECT_DATA af;

          af.type      = skill_lookup("charm person");
          af.duration  = -1;
          af.location  = APPLY_NONE;
          af.modifier  = 0;
          af.bitvector = AFF_CHARM;
          affect_to_char( victim, &af );
       }
}

void do_anti_aura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    if ( !IS_SET( ch->cmbt[5], SKL2_ANTIAURA ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Anitaura whom?\n", ch );
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

    if ( IS_SET( victim->cmbt[5], SKL2_REMINISCE ) )
       {
          stc("#yTheir #Pmind #wis too strong.#n\n\r",ch);
          return;
       }

    act("#yYou #wdischard all #Paura #yaffects #wfrom #g$N#w.#n",ch,NULL,victim,TO_CHAR);
    act("#y$n #wdischards all #Paura #yaffects #wfrom #g$N#w.#n",ch,NULL,victim,TO_NOTVICT);
    act("#y$n #wdischards all #Paura #yaffects #wfrom #gyou.#n",ch,NULL,victim,TO_VICT);

    if ( victim->outer_aura != -1 )
       {
          do_aura(victim,"outer none");
          victim->wait += 6;
       }

    if ( victim->inner_aura != -1 )
       {
          do_aura(victim,"inner none");
          victim->wait += 6;
       }
}

void do_drain_power( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 33103 )
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_WIZARD) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
             {
                stc("Whose mana do you wish to drain?\n\r",ch);
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
       }
       else victim = ch->fighting;

    if ( victim == NULL )
       return;
 
    if ( ch == victim )
       {
          send_to_char("You can't do this to yourself!\n\r",ch);
	    return;
       }

   if ( is_safe(ch,victim) )
      return;

   ch->wait = 8;
   mana_gain( ch, ( ch->max_mana * 0.1 ) );
   victim->mana = UMAX( victim->mana - 500000, 0 );

   act("You open your hands drawing out $N's spiritual energy for your own use.",ch,NULL,victim,TO_CHAR);
   act("$n opens $s hands drawing out your spiritual energy for $s own use.",ch,NULL,victim,TO_VICT);
   act("$n opens $s hands drawing out $N's spiritual energy for $s own use.",ch,NULL,victim,TO_NOTVICT);
}

void do_gaseous(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if (IS_NPC(ch)) 
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

   argument = one_argument(argument, arg);

   if ( arg[0] == '\0' )
     {
      send_to_char( "Who do you wish to put in or out of Ethereal?\n\r", ch );
	return;
     }

   if ((victim = get_char_room( ch, arg)) == NULL)
      {
       send_to_char("They arent here.\n\r",ch);
       return;
      }

   if ( ch->fight_timer > 0 && IS_NPC( victim ) && victim->master != NULL )
   {
      stc("You can't do this on creatures like these with a fight timer!\n\r", ch );
      return;
   }

   if ( victim->fight_timer > 0 )
	{
	  send_to_char("They have a fight timer.\n\r",ch);
	  return;
      }

   if ( !IS_AFFECTED(victim, AFF_ETHEREAL))
      {
        act("You release the spiritual gases making $N Ethereal.",ch,NULL,victim,TO_CHAR);
        act("$n releases the spiritual gases making you Ethereal.",ch,NULL,victim,TO_VICT);
        act("$n releases the spiritual gases making $N Ethereal.",ch,NULL,victim,TO_NOTVICT);
        WAIT_STATE(ch, 8);     
        SET_BIT(victim->affected_by, AFF_ETHEREAL);
        return;
      }

   act("You draw back the spiritual gases unetherealing $N.",ch,NULL,victim,TO_CHAR);
   act("$n draws back the spiritual gases unetherealing you.",ch,NULL,victim,TO_VICT);
   act("$n draws back the spiritual gases unetherealing $N.",ch,NULL,victim,TO_NOTVICT);

   WAIT_STATE(ch, 8);     
   REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
}

void do_mage_clone( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    CHAR_DATA *target;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    AFFECT_DATA af;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[1] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->pcdata->followers > 2 )
       {
	    send_to_char("You have too many beings following you for this.\n\r",ch);
	    return;
       }

    if ( ch->mana < 30000 )
       {
	    send_to_char("You are too tired to execute this technique.\n\r",ch);
	    return;
       }

    argument = one_argument(argument, arg);

    if ( ( target = get_char_room( ch, arg) ) == NULL )
       {
          send_to_char("They arent here.\n\r",ch);
          return;
       }

    if ( IS_NPC(target) && ( target->pIndexData->vnum == 31 || target->pIndexData->vnum == 22 ||
         target->pIndexData->vnum == 55000 || target->pIndexData->vnum == 26005 || target->pIndexData->vnum == 21
         || target->pIndexData->vnum == 10329 || target->pIndexData->vnum == 20
         || ( target->pIndexData->vnum >= 22000 && target->pIndexData->vnum <= 23000 ) ) )
       {
          send_to_char("They are protected by their deity.\n\r", ch);
          return;
       }

    ch->ctimer[1] = 2;
    ch->pcdata->followers++;
    ch->mana -= 30000;

    victim = create_mobile( get_mob_index( 40 ) );
    victim->sex = target->sex;
    victim->level = 980;
    victim->armor = char_ac(target);
    victim->hitroll = char_hitroll(target);
    victim->damroll = char_damroll(target);
    victim->hit = victim->max_hit = target->max_hit;
    victim->mana = victim->max_mana = target->max_mana;
    victim->move = victim->max_move = target->max_move;
    victim->name = str_dup(target->name);

    if ( !IS_NPC(target) )
       {
          sprintf( buf, "%s is hovering here\n\r", target->name );
          victim->long_descr = str_dup(buf);
          victim->short_descr = str_dup(target->name);
       }
       else {
                victim->long_descr = str_dup(target->long_descr);
                victim->short_descr = str_dup(target->short_descr);
            }

    victim->itemaffect = target->itemaffect;
    victim->affected_by = target->affected_by;

    char_to_room( victim, ch->in_room );

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    act( "You point your finger at $N as a blurry image of $M fades into existence.", ch, NULL, victim, TO_CHAR );
    act( "$n points $s finger at you as a blurry image of yourself fades into existence.", ch, NULL, victim, TO_VICT );
    act( "$n points $s finger at $N as a blurry image of $M fades into existence.", ch, NULL, victim, TO_NOTVICT );
    add_follower( victim, ch );
    do_group(ch,victim->name);
}

void do_mknights( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    int count, max_count = 1;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->fighting == NULL )
       {
	    send_to_char("But you aren't in a fight.\n\r",ch);
          return;
       }

    if ( ch->mana < 100000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    ch->mana -= 100000;
    ch->ctimer[2] = 12;

    act( "You fall to your knees saying '#yKnights of the Sea, I need your help.#n'", ch, NULL, NULL, TO_CHAR );
    act( "$n falls to $s knees saying '#yKnights of the Sea, I need your help.#n'", ch, NULL, NULL, TO_ROOM );

    if ( IS_SET(ch->more3, MORE3_SOUL) )
       {
          max_count = 3;
          act( "Your image falls to its knees saying '#yKnights of the Sea, I need your help.#n'", ch, NULL, NULL, TO_CHAR );
          act( "$n's image falls to its knees saying '#yKnights of the Sea, I need your help.#n'", ch, NULL, NULL, TO_ROOM );
          act( "Your image falls to its knees saying '#yKnights of the Sea, I need your help.#n'", ch, NULL, NULL, TO_CHAR );
          act( "$n's image falls to its knees saying '#yKnights of the Sea, I need your help.#n'", ch, NULL, NULL, TO_ROOM );
       }

    act( "The ground explodes with ferocious energy as a gush of water spruts into the sky.", ch, NULL, NULL, TO_CHAR );
    act( "The ground explodes with ferocious energy as a gush of water spruts into the sky.", ch, NULL, NULL, TO_ROOM );
    
    for ( count = 0; count < max_count; count++ )
    {
       victim = create_mobile( get_mob_index( 29 ) );
       victim->sex = ch->sex;
       victim->level = 999;
       victim->armor = -40000;
       victim->hitroll = 30000;
       victim->damroll = 30000;
       victim->hit = victim->max_hit = 500000;
       victim->mana = victim->max_mana = 250000;
       victim->move = victim->max_move = 250000;

       af.type      = skill_lookup("haste");
       af.duration  = -1;
       af.location  = APPLY_NONE;
       af.modifier  = 0;
       af.bitvector = 0;
       affect_to_char( victim, &af );
       char_to_room( victim, ch->in_room );

       if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
          SET_BIT( victim->affected_by, AFF_ETHEREAL );

       if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
          SET_BIT( victim->affected_by, AFF_SHADOWPLANE );

       SET_BIT( victim->act, ACT_NINJA );
       SET_BIT( victim->act, ACT_TIMER );
       victim->practice = 2;

       switch( count )
       {
          default:
          case 0: act("$n exclaims '#yProtect the Magi!#n' as he raises his sword and goes for the attack!",victim,NULL,NULL,TO_ROOM);
                  break;
          case 1: act("Brandishing $s axe, $n exclaims '#yDie Enemy!!#n' as he goes for the attack!",victim,NULL,NULL,TO_ROOM);
                  break;
          case 2: act("Charging to his target, $n exclaims '#yHurry! Before it is too late!'",victim,NULL,NULL,TO_ROOM);
                  break;
          case 3: act("Charging to his target, $n exclaims '#y$N needs our help!'",victim,NULL,ch,TO_ROOM);
                  break;
       }

       set_fighting( victim, ch->fighting );
    }
}

void do_unsummon( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 250000 )
       {
	    send_to_char("You are too tired to execute this technique.\n\r",ch);
	    return;
       }

    argument = one_argument(argument, arg);

    if ( ( victim = get_char_room( ch, arg) ) == NULL )
       {
          send_to_char("They arent here.\n\r",ch);
          return;
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( !IS_NPC(victim) )
       {
	    send_to_char("Not on players!\n\r",ch);
	    return;
       }

    if ( victim->pIndexData->vnum == 4500 || victim->pIndexData->vnum == 4501
         || victim->pIndexData->vnum == 2500 || victim->pIndexData->vnum == 2501
         || victim->pIndexData->vnum == 16500 || victim->pIndexData->vnum == 16501
         || victim->pIndexData->vnum == 14500 || victim->pIndexData->vnum == 14501
         || victim->pIndexData->vnum == 3500 || victim->pIndexData->vnum == 3501 )
       {
	    send_to_char("Not on kingdom guards!\n\r",ch);
	    return;
       }

    if ( victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum < 571 )
       {
	    send_to_char("Do these look like mobs to you?\n\r",ch);
	    return;
       }

    ch->mana -= 250000;
    ch->wait = 4;

    act( "You clench your fist as you create a shimmering vortex.", ch, NULL, NULL, TO_CHAR );
    act( "$n clenches $s fist as $e creates a shimmering vortex.", ch, NULL, NULL, TO_ROOM );
    act( "Pointing your finger at $N, you banish him through the vortex forever.", ch, NULL, victim, TO_CHAR );
    act( "Pointing $s finger at $N, $n banishs $N through the vortex forever.", ch, NULL, victim, TO_NOTVICT );

    stop_fighting(victim,TRUE);
    die_follower( victim );

    if ( victim->mount != NULL )
       {
          victim->mount->mounted = IS_ON_FOOT;
          victim->mounted = IS_ON_FOOT;

          victim->mount->mount = NULL;
          victim->mount = NULL;
       }

    extract_char(victim,TRUE);
}

void do_gaea_blessing( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->mana < 500000 )
     {
	  send_to_char("You don't have enough magical energy for that.\n\r",ch);
	  return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "gaea blessing" ) ) < 0 ) return;

  if ( is_affected(victim,skill_lookup("gaea blessing")) )
  {
    stc("The person is already blessed by Gaea!\n\r",ch);
    return;
  }

  ch->mana -= 500000;
  level = 30;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 12 );
}

void do_briar_shield( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 2 && ch->clannum != 6 ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 5 )
     {
        send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
        return;
     }

  if ( ch->mana < 500000 )
     {
	  send_to_char("You don't have enough magical energy for that.\n\r",ch);
	  return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
        stc("They are not here.\n\r",ch);
        return;
     }

  if ( ( sn = skill_lookup( "briar shield" ) ) < 0 ) return;

  if ( is_affected(victim,skill_lookup("briar shield")) )
  {
    stc("The person is already engulfed in a shield of briar!\n\r",ch);
    return;
  }

  ch->mana -= 500000;
  level = 30;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 12 );
}

void do_primal_rage( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 3 && ch->clannum != 6 ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 10 )
     {
        send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
        return;
     }

  if ( IS_SET( ch->more2, MORE2_RAGE ) )
     {
        stc("Your spirit is already attuned to nature.\n\r",ch);
        return;
     }

  ch->wait = 12;
  act("You spread out your hands letting nature's suffering flows through you.",ch,NULL,NULL,TO_CHAR);
  act("$n spreads out $s hands letting nature's suffering flows through $m.",ch,NULL,NULL,TO_ROOM);
  act("Your spirit is now attuned with nature.",ch,NULL,NULL,TO_CHAR);
  SET_BIT( ch->more2, MORE2_RAGE );
}

void do_nature_revolt( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 3 && ch->clannum != 6 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 12;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 200 );

   act("You hover in the air silently as thunder clouds fill the sky.",ch,NULL,NULL,TO_CHAR);
   act("$n hovers in the air silently as thunder clouds fill the sky.",ch,NULL,NULL,TO_ROOM);
   act("Spreading your arms apart, you look above you as your body ExPlOdEs!!!",ch,NULL,NULL,TO_CHAR);
   act("Spreading $s arms apart, $n looks above $m as $s body ExPlOdEs!!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch,vch) ) 
          continue;

       if ( !is_safe(ch, vch) )
          {
              stc("You scream loudly as a HUGE explosion RiPs through you!\n\r",vch);
              act("$n screams loudly as a HUGE explosion RiPs through $s!",vch,NULL,NULL,TO_ROOM);
              stc("Suddenly the energy transforms into all the elements under the sun, DeVaStAtInG you mightily!\n\r",vch);
              act("Suddenly the energy transforms into all the elements under the sun, DeVaStAtInG $n mightily!",vch,NULL,NULL,TO_ROOM);
              ch->ele_attack = ATTACK_EARTH;
              ch->attack_type = ATTACK_MAGICAL;
              ch->attack_var = BLAST;
              ch->attack_focus = AERIAL;
              hurt_person( ch, vch, 2000000, TRUE );
	    }
   }
}

void do_cleanse( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->mana < 1500000 )
     {
	  send_to_char("You don't have enough magical energy for that.\n\r",ch);
	  return;
     }

  if ( arg[0] == '\0' )
     victim = ch;  
     else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
        stc("They are not here.\n\r",ch);
        return;
     }

  ch->mana -= 1500000;
  ch->wait = 6;

  if ( victim == ch )
     {
        act("You spread your arms out widely as a massive beam from the energy shines onto you.",ch,NULL,NULL,TO_CHAR);
        act("$n spreads $s arms out widely as a massive beam from the energy shines onto $m.",ch,NULL,NULL,TO_ROOM);
        act("You feel much healthier as your body is cleansed from evil.",ch,NULL,NULL,TO_CHAR);
        act("$n feels much healthier as $s body is cleansed from evil.",ch,NULL,NULL,TO_ROOM);
        clear_affects( ch );
     }
     else {
             act("You spread your arms out widely as a massive beam from the energy shines onto $N.",ch,NULL,victim,TO_CHAR);
             act("$n spreads $s arms out widely as a massive beam from the energy shines onto you.",ch,NULL,victim,TO_VICT);
             act("$n spreads $s arms out widely as a massive beam from the energy shines onto $N.",ch,NULL,victim,TO_NOTVICT);
             act("You feel much healthier as your body is cleansed from evil.",victim,NULL,NULL,TO_CHAR);
             act("$n feels much healthier as $s body is cleansed from evil.",victim,NULL,NULL,TO_ROOM);
             clear_affects( victim );
          }
}

void do_mangara_blessing( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->mana < 1000000 )
     {
	  send_to_char("You don't have enough magical energy for that.\n\r",ch);
	  return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "mangara blessing" ) ) < 0 ) return;

  ch->mana -= 1000000;
  level = 30;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 8 );
}

void do_great_wall( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 17501 && ch->pIndexData->vnum != 17502 ) 
     return;

  if ( !IS_NPC(ch) )
     {
          if ( !IS_CLASS(ch, CLASS_WIZARD) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }
 
          if ( ch->mana < 1000000 )
	       {
	          stc("You do not have enough magical energy to do that!\n\r",ch);
	          return;
	       }
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use this?\n\r",ch);
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

  if ( IS_SET(victim->more2, MORE2_NOARMS) )
     {
	  send_to_char("They are already struggling to attack.\n\r",ch);
	  return;
     }

  if ( !IS_NPC(ch) )
     ch->mana -= 1000000;

  act("You point your hand at the centre of the room as everything goes white!",ch,NULL,NULL,TO_CHAR);
  act("$n points $s hand at the centre of the room as everything goes white!",ch,NULL,NULL,TO_ROOM); 
  act("A huge wall wraps itself around your body, making it more difficult for you to attack!",victim,NULL,NULL,TO_CHAR);
  act("A huge wall wraps itself around $n's body, making it more difficult for $m to attack!",victim,NULL,NULL,TO_ROOM);

  SET_BIT( victim->more2, MORE2_NOARMS );
  hurt_person(ch,victim,300000,TRUE);
}

void do_disempower( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ch->clannum != 4 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
          return;
       }

     if ( ch->mana < 500000 )
     {
        stc("You don't have enough magical energy for that.\n\r",ch);
        return;
     }   

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to show the wrath of God to?\n\r",ch);
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
    
    if (is_safe(ch,victim) ) return;

    if ( ( victim == ch ) || ( IS_NPC(victim) ) )
       {
	   stc("That doesn't seem like a good idea.\n\r",ch);
	   return;
       }

    if ( IS_SET(victim->more, MORE_DISEMPOWER) )
       {
	   stc("They has already felt the wrath of God.\n\r",ch);
	   return;
       }

    act("Your hands glow with divine energy as you release a massive beam towards $N.",ch,NULL,victim,TO_CHAR);
    act("$n's hands glow with divine energy as $e releases a massive beam towards you.",ch,NULL,victim,TO_VICT);
    act("$n's hands glow with divine energy as $e releases a massive beam towards $N.",ch,NULL,victim,TO_NOTVICT);
    act("As the beam sears your flesh, your sins has come to haunt you.",victim,NULL,NULL,TO_CHAR);
    act("As the beam sears $s flesh, $n clutches $s head in agony.",victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->more, MORE_DISEMPOWER);
    ch->mana -= 500000;
}

void do_alabaster_potion( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->mana < 250000 )
     {
	  send_to_char("You don't have enough magical energy for that.\n\r",ch);
	  return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "alabaster potion" ) ) < 0 ) return;

  if ( is_affected(victim,skill_lookup("alabaster potion")) )
  {
    stc("The person is already feeling the affects of this mystical potion!\n\r",ch);
    return;
  }

  ch->mana -= 250000;
  level = 30;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 8 );
}

void do_aggression( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
          return;
       }

    if ( ch->mana < 100000 )
       {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
       }

    if ( arg[0] == '\0' )
       {
         stc("Whose heart you want to fill with rage?\n\r",ch);
	   return;
       }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
         send_to_char( "They aren't here.\n\r", ch );
         return;
       }

    if ( IS_SET(victim->more, MORE_TOUCH) )
       {
         send_to_char( "They are already filled with rage.\n\r", ch );
         return;
       }

    ch->mana -= 100000;
    ch->wait = 12;
    SET_BIT(victim->more, MORE_TOUCH);

    if ( victim == ch )
       {
           act("You release the rage within you!",ch,NULL,NULL,TO_CHAR);
           act("$n releases the rage within you!",ch,NULL,NULL,TO_ROOM);
           return;
       }

    act("You touch $N's forehead releasing the rage within $M!",ch,NULL,victim,TO_CHAR);
    act("$n touches your forehead releasing the rage within you!",ch,NULL,victim,TO_VICT);
    act("$n touches $N's forehead releasing the rage within $M!",ch,NULL,victim,TO_NOTVICT);
}

void do_soul_field( CHAR_DATA *ch, char *argument )
{
    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 5 )
       {
          send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
          return;
       }

    if ( ch->mana < 2000000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more3, MORE3_SOUL) )
       {
          act( "You laugh with insanity as you spread your right hand, snapping your fingers.", ch,NULL,NULL,TO_CHAR );
          act( "$n laughs with insanity as $e spreads $s right hand, snapping your fingers.", ch,NULL,NULL,TO_ROOM );
          act( "Your images merge back within your body as your soul connects again with your lifeforce.", ch,NULL,NULL,TO_CHAR );
          act( "$n's images merge back within $s body as $e soul connects again with $s lifeforce.", ch,NULL,NULL,TO_ROOM );
          REMOVE_BIT(ch->more3, MORE3_SOUL);
          return;
       }

    if ( !IS_NPC(ch) )
       ch->mana -= 2000000;

    ch->wait = 12;
    SET_BIT(ch->more3, MORE3_SOUL);

    act( "You laugh with insanity as you spread your right hand, snapping your fingers.", ch,NULL,NULL,TO_CHAR );
    act( "$n laughs with insanity as $e spreads $s right hand, snapping your fingers.", ch,NULL,NULL,TO_ROOM );
    act( "Wrapping a heavy sharf around your body, you becomes blurry.", ch,NULL,NULL,TO_CHAR );
    act( "Wrapping a heavy sharf around $s body, $n becomes blurry.", ch,NULL,NULL,TO_ROOM );
    act( "Multiple images hovers around your body mimicking every action you make.", ch,NULL,NULL,TO_CHAR );
    act( "Multiple images hovers around $n's body mimicking every action $e makes.", ch,NULL,NULL,TO_ROOM );
}

void do_fireball( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int cost = 400000;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_WIZARD) && !IS_CLASS(ch, CLASS_DROW) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( IS_CLASS(ch, CLASS_DROW) && ch->pcdata->powers[DROW_CLASS] != DROW_MAGI 
               && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
             {
                stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
                return;
             }

          if ( IS_CLASS(ch, CLASS_DROW) &&  ch->pkill < 12 )
             {
                stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
                return;
             }

          if ( IS_CLASS(ch, CLASS_DROW) )
             cost = 250000;

          if ( ch->mana < cost )
	       {
	          stc("You do not have enough magical energy to do that!\n\r",ch);
	          return;
	       }
       }

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to smash with a fireball?\n\r",ch);
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
    
    if (is_safe(ch,victim) ) return;

    ch->wait = 12;

    if ( !IS_NPC(ch) )
       ch->mana -= cost;

    act("You point your finger at $N as a massive fireball forms on the tip.",ch,NULL,victim,TO_CHAR);
    act("$n points $s finger at you as a massive fireball forms on the tip.",ch,NULL,victim,TO_VICT);
    act("$n points $s finger at $N as a massive fireball forms on the tip.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly flick your finger releasing the fireball at $N!",ch,NULL,victim,TO_CHAR);
    act("$n suddenly flicks $s finger releasing the fireball at you!",ch,NULL,victim,TO_VICT);
    act("$n suddenly flicks $s finger releasing the fireball at $N!",ch,NULL,victim,TO_NOTVICT);

    act("You stagger backwards from the fiery blast!",victim,NULL,NULL,TO_CHAR);
    act("$n staggers backwards from the fiery blast!",victim,NULL,NULL,TO_ROOM);

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = BLAST;
    ch->ele_attack = ATTACK_FIRE;
    hurt_person( ch, victim, 700000, TRUE );
}

void do_icelance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_WIZARD) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ch->mana < 400000 )
	       {
	          stc("You do not have enough magical energy to do that!\n\r",ch);
	          return;
	       }
       }

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to smash with a icelance?\n\r",ch);
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
    
    if (is_safe(ch,victim) ) return;

    ch->wait = 12;

    if ( !IS_NPC(ch) )
       ch->mana -= 400000;

    act("You gaze silently at your hand as shards of ice manifests on it.",ch,NULL,NULL,TO_CHAR);
    act("$n gazes silently at $s hand as shards of ice manifests on it.",ch,NULL,NULL,TO_ROOM);
    act("You then dash straight at $N as you drive an icy lance through $S body!",ch,NULL,victim,TO_CHAR);
    act("$n then dashes straight at you as $e drives an icy lance through your body!",ch,NULL,victim,TO_VICT);
    act("$n then dashes straight at $N as $e drives an icy lance through $S body!",ch,NULL,victim,TO_NOTVICT);
    act("Your body implodes as the ice seemingly rips your innards apart!",victim,NULL,NULL,TO_CHAR);
    act("$n's body implodes as the ice seemingly rips $s innards apart!",victim,NULL,NULL,TO_ROOM);

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_ICE;
    ch->attack_focus = DIRECT;
    ch->attack_var = PIERCE;
    hurt_person( ch, victim, 700000, TRUE );
}

void do_stop( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 3 && ch->clannum != 6 ) )
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
           send_to_char( "Freeze whom?\n\r", ch );
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

    if ( is_safe(ch,victim) )
       return;

    act("You point your hands at $N stopping time around $M.",ch,NULL,victim,TO_CHAR);
    act("$n points $s hands at you stopping time around you.",ch,NULL,victim,TO_VICT);
    act("$n points $s hands at $N stopping time around $M.",ch,NULL,victim,TO_NOTVICT);
    act("You can't move.",ch,NULL,victim,TO_VICT);

    if ( ch->ctimer[0] == 0 )
       {
          if ( number_percent() > 89 - devil_luck( ch, victim, 10 ) && !is_martial( victim ) )
             {
                victim->freeze_timer = 2;
                SET_BIT( victim->more, MORE_FROZEN );
             }
             else victim->wait = 12;
       }

    if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
         && ch->ctimer[9] != 100 ) )
       ch->ctimer[0] = 3;

    ch->wait = 12;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 100 );

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = SOUL;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_ultima( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 300 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    ch->wait = 12;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 300 );

    act("You say, '#yThe Entity of all times, never lost... Ultima!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yThe Entity of all times, never lost... Ultima!#n'",ch,NULL,NULL,TO_ROOM);
    act("$n's hands glows #yblazing gold#n as $e leans forward and point one of them at you.",ch,NULL,NULL,TO_ROOM);
    act("Your hands glows #yblazing gold#n as you lean forward and point one of them at the room.",ch,NULL,NULL,TO_CHAR);
    act("A small green ball erupts from $s hands as it spirals towards you.",ch,NULL,NULL,TO_ROOM);
    act("A small green ball erupts from $s hands as it spirals towards the center of the room.",ch,NULL,NULL,TO_CHAR);
    act("Suddenly the green ball ExPlOdEs ViOlEnTlY filling the room with blinding energy as the ground trembles loudly.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly the green ball ExPlOdEs ViOlEnTlY filling the room with blinding energy as the ground trembles loudly.",ch,NULL,NULL,TO_CHAR);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( is_same_group(ch,vch) ) 
           continue;

        if ( !IS_NPC(ch) && is_safe(ch, vch) )
           continue;

        stc("The searing heat of the ball blinds you.\n\r",vch);
        act("$n is blinded from the searing heat of the ball of energy.",vch,NULL,NULL,TO_ROOM);

        if ( !IS_SET( vch->more, MORE_BLIND ) )
           SET_BIT( vch->more, MORE_BLIND );
    }

    act("Through the explosion, greenish holy flames softly fades into existence around the room.",ch,NULL,NULL,TO_ROOM);
    act("Through the explosion, greenish holy flames softly fades into existence around the room.",ch,NULL,NULL,TO_CHAR);
    act("As if alive, the flames slowly start encircling themselves around their victims' body.",ch,NULL,NULL,TO_ROOM);
    act("As if alive, the flames slowly start encircling themselves around their victims' body.",ch,NULL,NULL,TO_CHAR);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( is_same_group(ch,vch) ) 
           continue;

        if ( !is_safe( ch, vch ) )
           {
              act("You scream loudly as the holy flames DESTROYS your body.",vch,NULL,NULL,TO_CHAR);
              act("$n screams loudly as the holy flames DESTROYS $s body.",vch,NULL,NULL,TO_ROOM);
              vch->agg = 100;
              ch->attack_type = ATTACK_MAGICAL;
              ch->attack_var = BLAST;
              ch->attack_focus = DIRECT;
              ch->ele_attack = ATTACK_HOLY;
              hurt_person( ch, vch, 3500000, TRUE );
	     }
    }
}

void do_whirlwind( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 1 && ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000 )
       {
          stc("You don't see a need of whirling away within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 2 && ch->in_room->vnum >= 3500 && ch->in_room->vnum < 4000 )
       {
          stc("You don't see a need of whirling away within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 3 && ch->in_room->vnum >= 4500 && ch->in_room->vnum < 5000 )
       {
          stc("You don't see a need of whirling away within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 4 && ch->in_room->vnum >= 16500 && ch->in_room->vnum < 17000 )
       {
          stc("You don't see a need of whirling away within your own base.\n\r",ch);
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 1500000 )
       {
          send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char("It is #c#zimpossible#n to flee from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
          return;
       }

    act("$n focuses intently as a massive whirlwind manifests in front of you.",ch,NULL,NULL,TO_ROOM);
    stc("You focus intently as a massive whirlwind manifests in front of you.\n\r",ch);
    ch->ctimer[2] = 8;
    ch->mana -= 1500000;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( IS_NPC( vch ) )
           continue;

        if ( is_same_group( ch, vch ) || ch->pcdata->kingdom == vch->pcdata->kingdom
             || vch->fighting == NULL ) 
           continue;

        vch->wait = 36;
    }

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( IS_NPC( vch ) )
           continue;

        if ( !is_same_group(ch,vch) && ( ch->pcdata->kingdom != vch->pcdata->kingdom ||
             ch->pcdata->kingdom == 0 ) )
           continue;

        if ( vch->fighting != NULL )
           stop_fighting( vch, TRUE );

        stc("You are swept away by the gale winds.\n\r",vch);
        act("$n is swept away by the gale winds.",vch,NULL,NULL,TO_ROOM);
        get_random_room( vch );

        do_look(vch,"auto");
        vch->wait = 12;
    }

    if ( ch->fighting != NULL )
       stop_fighting( ch, TRUE );

    stc("You are swept away by the gale winds.\n\r",ch);
    act("$n is swept away by the gale winds.",ch,NULL,NULL,TO_ROOM);
    get_random_room( ch );

    do_look(ch,"auto");
    ch->wait = 12;
}

void accelerate_tick( CHAR_DATA *ch, int level )
{
    int count;

    for ( count = 0; count < 18; count++ )
    {
       if ( count == 8 || count == 9 || count == 15 )
          continue;

       if ( ch->ctimer[count] > 0 )
          ch->ctimer[count]--;
    }

    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->ninja_powers[HARA_KIRI] > 0 ) 
       {
          if ( ch->pcdata->powers[HARA_KIRI] == 1 )
             send_to_char("The effects of the harakiri wear off.\n\r", ch );
             ch->pcdata->ninja_powers[HARA_KIRI]--;	
       }

    if ( is_affected( ch,skill_lookup("despondency") ) )
       {
          AFFECT_DATA af;

          af.type = skill_lookup("despondency");
          af.location  = APPLY_HITROLL;
          af.modifier  = -200000;
          af.duration  = 0;
          af.bitvector = 0;
          affect_join( ch, &af );

          af.type = skill_lookup("despondency");
          af.location  = APPLY_DAMROLL;
          af.modifier  = -200000;
          af.duration  = 0;
          af.bitvector = 0;
          affect_join( ch, &af );

          af.type = skill_lookup("despondency");
          af.location  = APPLY_STR;
          af.modifier  = -2;
          af.duration  = 0;
          af.bitvector = 0;
          affect_join( ch, &af );

          stc("You scream loudly as the Darkside devours your life and soul.\n\r",ch);
          act("$n screams loudly as a mystical force rips apart $s mortal body.",ch,NULL,NULL,TO_ROOM);
          ch->hit = UMAX( 1, ch->hit - ( ch->max_hit / 100 ) );
          update_pos(ch);
       }

    if ( IS_SET(ch->more, MORE_BLIND) )
       {
          REMOVE_BIT(ch->more, MORE_BLIND);
          ch->blind_timer = 0;
          send_to_char("Your vision returns!\n\r",ch);
          act("The white gaze surrounding $n's eyes clears.",ch,NULL,NULL,TO_ROOM);
       }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
          REMOVE_BIT(ch->more, MORE_IMPALE);
          send_to_char("The spear holding your foot dissolves into the ground!\n\r",ch);
          act("The spear holding $n's foot dissolves into the ground.",ch,NULL,NULL,TO_ROOM);
       }

    if ( IS_SET(ch->more, MORE_FROZEN) )
       {
          REMOVE_BIT(ch->more, MORE_FROZEN);
          ch->freeze_timer = 0;
          send_to_char("#bThe #wice #bencasing you melts away.#n\n\r",ch);
          act("#bThe #wice #bencasing $n #bmelts away.#n",ch,NULL,NULL,TO_ROOM);
       }

    if ( IS_SET(ch->more, MORE_CONFUSION) )
       REMOVE_BIT(ch->more, MORE_CONFUSION);

    if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
       {
          stc("The earth surrounding you starts to slowly crack.\n\r",ch);
          SET_BIT(ch->mflags, MAGE_EMBRACED1);
          REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
       }
       else if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
               {
                  stc("The earth surrounding you falls away.\n\r",ch);
                  REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
               }

    if ( is_affected( ch, skill_lookup("drow poison") ) )
       {
          send_to_char("You cough violently as the mystical poison rips through your vital systems.\n\r",ch);
          act("$n coughs violently as the mystical poison rips through $s vital systems.",ch,NULL,NULL,TO_ROOM); 
          ch->hit -= 300000;
       }

    if ( IS_SET(ch->more, MORE_HEX) )
       {
          REMOVE_BIT(ch->more, MORE_HEX);
          send_to_char("You can speak properly again.\n\r",ch);
       }

    if ( IS_SET(ch->more, MORE_POISON) && ch->hit > 250000 )
       {
          if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
             REMOVE_BIT(ch->more, MORE_POISON);
             else if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) )
                     REMOVE_BIT(ch->more, MORE_POISON);
             else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DRAGON) && IS_DRAGONPOWER(ch,DRAGON_IMMUNITY) )
                     REMOVE_BIT(ch->more, MORE_POISON);
             else {
                     send_to_char("You cough violently as the poison rips through your vital systems.\n\r",ch);
                     act("$n coughs violently as the poison rips through $s vital systems.",ch,NULL,NULL,TO_ROOM); 
                     ch->hit -= 200000;
                  }
       } 

    if ( level >= 6 )
       {
          if ( IS_SET(ch->garou1, GAROU_WHELP2) )
             {
                REMOVE_BIT(ch->garou1, GAROU_WHELP2);
                send_to_char("You feel better as the curse leaves you.\n\r",ch);
                ch->max_hit += 750000;
             }
       }

    if ( level >= 3 )
       {
          if ( IS_SET(ch->more2,MORE2_DESPONDENCY) ) 
             REMOVE_BIT(ch->more2,MORE2_DESPONDENCY);

          if ( IS_SET( ch->monkstuff, MONK_DEATH ) )
             REMOVE_BIT( ch->monkstuff, MONK_DEATH );

          if ( IS_SET( ch->extra, EXTRA_TLOCK ) && number_percent() > 50 )
             {
                send_to_char("Time has been restored to you as the air has been lifted.\n\r",ch);
                REMOVE_BIT( ch->extra, EXTRA_TLOCK );
             }

          if ( IS_SET(ch->more, MORE_POISON) )
             {
                REMOVE_BIT(ch->more, MORE_POISON);
                stc("Your body feels stronger as the deadly poison leaves you.\n\r",ch);
             }

          if ( IS_SET(ch->more, MORE_BLOOD) )
             {
                stc("Your wounds is healed as you pull the devastating weapon from your body.\n\r",ch);
                act("$n removes the devastating stiletto from $s body.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->more, MORE_BLOOD);
             }

          if ( IS_SET(ch->garou1, GAROU_PAWS) )
             {
                REMOVE_BIT(ch->garou1, GAROU_PAWS);
                stc("Your body feels stronger as the deadly curse leaves you.\n\r",ch);
             }

          if ( IS_SET(ch->more, MORE_DISEMPOWER) )
             REMOVE_BIT(ch->more, MORE_DISEMPOWER);
       }

    if ( level >= 2 )
       {
          if ( IS_SET(ch->more2, MORE2_NOLEGS) )
             {
                send_to_char("Your movement is restored.\n\r",ch);
                REMOVE_BIT(ch->more2, MORE2_NOLEGS);
             }

          if ( IS_SET(ch->more2, MORE2_NOARMS) )
             {
                send_to_char("You can move your arms at will again.\n\r",ch);
                REMOVE_BIT(ch->more2, MORE2_NOARMS);
             }
       }

    if ( IS_SET(ch->more, MORE_PINCH) )
       { 
          if ( 95 < number_percent() || ch->fight_timer == 0 )
             {
                stc("Your head suddenly feels clear as the headache disappears.\n\r",ch);
                act("$n has recovered from $s crippling headache.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->more, MORE_PINCH);
             }
       }

    if ( IS_SET(ch->more, MORE_PAIN) )
       { 
          if ( 50 < number_percent() )
             {
                stc("Your suddenly start moving properly again.\n\r",ch);
                act("$n suddenly starts moving properly again.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->more, MORE_PAIN);
             }
       }

    if ( ch->gshield > 0 )
       {
          ch->gshield--;

          if ( ch->gshield == 0 )
             stc("You are now vulnerable to multiple attacks.\n\r",ch);
       }
}

void do_accelerate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int level = 1, count;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 3 && ch->clannum != 6 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[6] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Accelerate whom?\n\r", ch );
          return;
       }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    argument = one_argument( argument, arg );

    if ( !str_cmp( arg, "one" ) || !str_cmp( arg, "1" ) )
       level = 1;
       else if ( !str_cmp( arg, "two" ) || !str_cmp( arg, "2" ) )
               level = 2;
       else if ( !str_cmp( arg, "three" ) || !str_cmp( arg, "3" ) )
               level = 3;
       else if ( !str_cmp( arg, "four" ) || !str_cmp( arg, "4" ) )
               level = 4;
       else if ( !str_cmp( arg, "five" ) || !str_cmp( arg, "5" ) )
               level = 5;
       else if ( !str_cmp( arg, "six" ) || !str_cmp( arg, "6" ) )
               level = 6;
               else {
                       send_to_char( "Accelerate for how long? [1-6]\n\r", ch );
                       return;
                    }

    if ( ch->mana < level * 2000000 )
       {
          send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    if ( ch == victim )
       {
          stc( "You can't use this mystical ability on yourself.\n\r", ch );
          return;

          act( "You swerve your hands as you warp time around yourself.", ch, NULL, NULL, TO_CHAR );
          act( "$n swerves $s hands as $e warps time around $mself.", ch, NULL, NULL, TO_ROOM );
       }
       else {
               act( "You swerve your hands as you warp time around $N.", ch, NULL, victim, TO_CHAR );
               act( "$n swerves $s hands as $e warps time around you.", ch, NULL, victim, TO_VICT );
               act( "$n swerves $s hands as $e warps time around $N.", ch, NULL, victim, TO_NOTVICT );
            }

    ch->mana -= ( level * 2000000 );
    ch->wait = 12;
    ch->ctimer[6] = 6;

    for ( count = 0; count < level; count++ )
        accelerate_tick( victim, level );
}

void do_lumica_barrier( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( IS_SET(ch->in_room->room_flags, ROOM_LUMICA) )
     {
        stc("The soft glow surrounding the room surrounding fades away.\n\r",ch);
        act("The soft glow surrounding the room surrounding fades away.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->in_room->room_flags, ROOM_LUMICA);
        return;
     }

  if ( ch->mana < ch->max_mana / 5 )
     {
        stc("You don't have enough magical energy for that.\n\r",ch);
        return;
     }   

  ch->mana -= ( ch->max_mana / 5 );
  SET_BIT(ch->in_room->room_flags, ROOM_LUMICA);

  act("You raise your hand in the air as energy manifests brightly on it.", ch, NULL, NULL, TO_CHAR);
  act("$n raises $s hand in the air as energy manifests brightly on it.", ch, NULL, NULL, TO_ROOM);
  act("Sparks of radiant energy flashes across the room forming a shocking shield.", ch, NULL, NULL, TO_CHAR);
  act("Sparks of radiant energy flashes across the room forming a shocking shield.", ch, NULL, NULL, TO_ROOM);
  act("Suddenly, the room vibrates as a lumica barrier negates all magical energy!", ch, NULL, NULL, TO_CHAR);
  act("Suddenly, the room vibrates as a lumica barrier negates all magical energy!", ch, NULL, NULL, TO_ROOM);
}

void do_enrage( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
       {
           send_to_char("Huh?\n\r",ch);
           return;
       }

    if ( ch->mana < 1500000 )
       {
           stc("You do not have enough magical energy to do that!\n\r",ch);
           return;
       }

    if ( arg[0] == '\0' )
       {
           stc("Who do you wish to enrage?\n\r",ch);
	     return;
       }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
           send_to_char( "They aren't here.\n\r", ch );
           return;
       }
    
    ch->wait = 12;
    ch->mana -= 1500000;

    if ( ch == victim )
       {
          act( "You cackle with glee as your eyes burn with rage!", ch, NULL, NULL, TO_CHAR );
          act( "$n cackles with glee as $s eyes burn with rage!", ch, NULL, NULL, TO_ROOM );
          return;
       }

    act("You push your hand at $N as the air pulses from a mysterious force!", ch, NULL, victim, TO_CHAR );
    act("$n pushes $s hand at you as the air pulses from a mysterious force!", ch, NULL, victim, TO_VICT );
    act("$n pushes $s hand at $N as the air pulses from a mysterious force!", ch, NULL, victim, TO_NOTVICT );
    act("You get a wild look in your eyes, as you try in vain to control $s temper...",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, as $e tries in vain to control $s temper...",ch,NULL,NULL,TO_ROOM);

    if ( IS_CLASS( victim, CLASS_WEREWOLF ) && !IS_SET( victim->special, SPC_WOLFMAN ) )
       do_werewolf( victim, "" );
       else do_berserk( victim, "" );
}

void do_mwall( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 1000000 )
       {
          send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    ch->mana -= 1000000;
    spell_wall(skill_lookup("magical wall"), 30, ch, ch);
    spell_earth_wall(skill_lookup("earth wall"), 30, ch, ch);
    ch->wait = 12;
}

void do_specialize( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_CLASS( ch, CLASS_WIZARD ) ) 
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }	

   if ( ch->clannum != 0 )
      {
         send_to_char("You have already specialized wizard.\n\r", ch );
         return;
      }	

   if ( !str_cmp( "summoner", arg ) )
      {
         ch->clannum = 2;
         ch->cmbt[0] = 0;
         ch->pcdata->deity = 0;
         stc( "#cYou place your trust in deities... and are now a #pSummoner#n.\n\r", ch );
         return;
      }

   if ( !str_cmp( "battlemage", arg ) || !str_cmp( "battle mage", arg ) )
      {
         ch->clannum = 1;
         stc( "#cYou place your trust in magick... and are now a #pBattle Mage#n.\n\r", ch );
         return;
      }

   if ( !str_cmp( "timemage", arg ) || !str_cmp( "time mage", arg ) )
      {
         ch->clannum = 3;
         stc( "#cYou place your trust in time... and are now a #pTime Mage#n.\n\r", ch );

         if ( !IS_SET( ch->cmbt[7], SKL_TLOCK ) )
            SET_BIT( ch->cmbt[7], SKL_TLOCK );

         if ( !IS_SET( ch->cmbt[7], SKL_ACCELERATE ) )
            SET_BIT( ch->cmbt[7], SKL_ACCELERATE );

         return;
      }

   if ( !str_cmp( "bookmage", arg ) || !str_cmp( "book mage", arg ) )
      {
         ch->clannum = 4;
         stc( "#cYou place your trust in the books... and are now a #pBook Mage#n.\n\r", ch );
         return;
      }

   stc( "#wSyntax - #cSpecialize #psummoner#c/#ptimemage#c/#pbattlemage#c/#pbookmage#n\n\r", ch );
}

void do_timewarp( CHAR_DATA *ch, char *argument )
{
    int count;

	if ( IS_NPC( ch ) )
      return;

	if ( !IS_CLASS( ch, CLASS_WIZARD ) || ( ch->clannum != 3 && ch->clannum != 6 ) )
	   {
            stc("Huh?\n\r",ch);
            return;
	   }

      if ( ch->pkill < 10 )
         {
            send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
            return;
         }

	if ( ch->mana < 500000 )
	   {
            stc("You haven't got enough mana for this technique\n\r",ch);
            return;
	   }

	if ( ch->ctimer[4] > 0 )
	   {
            stc("You cannot do this tech so fast after your last one!\n\r",ch);
            return;
	   }
	
	ch->mana -= 500000;

	act("You wave your hands as time stops for everyone but you",ch,NULL,NULL,TO_CHAR);
	act("$n waves $s hands as time stops for everyone but $m",ch,NULL,NULL,TO_ROOM);
	act("#zYou fall asleep and are ready to fight even harder!#n",ch,NULL,NULL,TO_CHAR);
	act("#z$n falls asleep and is ready to fight even harder!#n",ch,NULL,NULL,TO_ROOM);
	act("You wake up and immediately commence battle, rejuvenated!",ch,NULL,NULL,TO_CHAR);
	act("$n wakes up and immediately commences battle, rejuvenated!",ch,NULL,NULL,TO_ROOM);
	act("Everyone pales in comparison to how powerful you are...",ch,NULL,NULL,TO_CHAR);
	act("You seem to pale in comparison to how powerful $n is...",ch,NULL,NULL,TO_ROOM);
	act("You SnAp your fingers and time resumes a normal #ypace#n",ch,NULL,NULL,TO_CHAR);
	act("$n SnApS $s fingers and time resumes a normal #ypace#n",ch,NULL,NULL,TO_ROOM);

    ch->wait = 6;

    for ( count = 0; count < 18; count++ )
    {
       if ( count == 8 || count == 9 || count == 15 )
          continue;

       if ( !( IS_CLASS( ch, CLASS_ANGEL ) && count == 4 ) )
          ch->ctimer[count] = 0;
    }

    ch->control_timer = 0;
    ch->dragoon_timer = 0;
    ch->dragon_timer = 0;
    ch->divine_timer = 0;
    ch->monk_timer = 0;
    ch->defiance_timer = 0;
    ch->power_timer = 0;
    ch->use_timer = 0;
    ch->ninja_timer = 0;
    ch->guarding = 0;
    ch->mage_timer = 0;
    ch->freeze_timer = 0;
    ch->ctimer[4] = 8;
}

void do_meteor_shower( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam = 1000000;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_WIZARD) )
      {
	   send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->pkill < 10 )
      {
         send_to_char( "#yYou #wneed to further your studies before being allowed to use this ability.#n\n\r", ch );
         return;
      }

   if ( ch->ctimer[5] > 0 )
      {
	   send_to_char("You are still recovering from the last one.\n\r",ch);
	   return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   ch->ctimer[5] = 4;
   ch->wait = 12;
   ch->mana -= 1000000;

   act("Your body explodes with demonic energy as you levitate into the air!",ch,NULL,NULL,TO_CHAR);
   act("$n's body explodes with demonic energy as $e levitates into the air!",ch,NULL,NULL,TO_ROOM);
   act("You say '#yDeadly shards, wreck havoc on my foes!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yDeadly shards, wreck havoc on my foes!#n'",ch,NULL,NULL,TO_ROOM);
   act("The sky goes dark as MaSsIvE MeTeOrS stones descend from the heavens!",ch,NULL,NULL,TO_CHAR);
   act("The sky goes dark as MaSsIvE MeTeOrS stones descend from the heavens!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_safe(ch, vch) )
         {
            stc("You get struck down by a massive meteor!\n\r",vch);
            act("$n gets struck down by a massive meteor!",vch,NULL,NULL,TO_ROOM);
            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_focus = AERIAL;
            ch->attack_var = BLUNT;
            ch->ele_attack = ATTACK_FIRE;
            hurt_person( ch, vch, dam, TRUE );
         }
   }
}

