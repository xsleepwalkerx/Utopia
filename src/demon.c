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

void frost_breath args(( CHAR_DATA *ch, CHAR_DATA *victim, bool all));

void do_leech(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim = NULL;
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 568 )
       return;

    if ( !IS_NPC( ch ) )
       {    
          if ( !IS_CLASS(ch, CLASS_DEMON) )
             {
                stc("Huh?\n\r",ch);
                return;
             }

          if ( argument[0] == '\0' )
             {
                stc("Who's life do you wish to leech off of?\n\r", ch);
                return;
             }

          if ( ch->mana < 250000 )
             {
                stc("You don't have enough mana for that.\n\r", ch);
                return;
             }

          if ( ( victim = get_char_room(ch, argument) ) == NULL )
             {
                stc("They aren't here.\n\r", ch);
                return;
             }

          if ( is_safe(ch, victim) )
             return;

          ch->mana -= 250000;
       }
       else if ( ( victim = ch->fighting ) == NULL )
               return;

    WAIT_STATE(ch, 12);

    act("$n stares intently at $N.", ch, NULL, victim, TO_NOTVICT);
    act("You stare intently at $N.", ch, NULL, victim, TO_CHAR);
    act("$n stares intently at you.", ch, NULL, victim, TO_VICT);
    sprintf(buf,"#1You absorb %d hitpoints.#n\n\r",100000);
    stc(buf,ch);
    sprintf(buf,"#1%s absorbed %d of your hitpoints!#n\n\r",ch->name,100000);
    stc(buf,victim);

    hit_gain( ch, 100000 );
    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_DARK;
    ch->attack_focus = DIRECT;
    ch->attack_var = SOUL;
    hurt_person( ch, victim, 600000, TRUE );
}

void do_entomb (CHAR_DATA *ch, char *argument)
{
     OBJ_DATA *obj;

     if ( IS_NPC(ch) )
        return;

     if ( !can_do_skill( ch, 55 ) )
        {
           if ( !IS_CLASS( ch, CLASS_DEMON ) )
              {
                 stc("Huh?\n\r",ch);
                 return;
              }

           if ( !IS_DEMPOWER(ch, DEM_ENTOMB) )
              {
                 stc("You have not been granted that power.\n\r",ch);
                 return;
              }
        }

    if ( ( obj = ch->pcdata->chobj ) != NULL && ch->pcdata->obj_vnum != 92 )
       {
	    send_to_char("You are not Zeik Tuvai... The Revenge Blade.\n\r",ch);
	    return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_ENTOMB) )
       {
	    send_to_char("The room is already entombed.\n\r",ch);
	    return;
       }

    SET_BIT(ch->in_room->room_flags, ROOM_ENTOMB);

    act("$n calls forth sheets of ice, blocking your path in every direction.", ch, NULL, NULL, TO_ROOM);
    send_to_char("You call forth sheets of ice to entrap your prey.\n\r", ch);
    send_to_char("You grin wickedly.\n\r",ch);

    act("You say '#yPrepare to die! Muhahahahaha!#n'", ch,NULL,NULL, TO_CHAR);
    act("$n says '#yPrepare to die! Muhahahahaha!#n'", ch,NULL,NULL, TO_ROOM);

    WAIT_STATE(ch, 6);
}

void do_gust( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    ROOM_INDEX_DATA *to_room = NULL;
    EXIT_DATA       *pexit;
    EXIT_DATA       *pexit_rev;
    char            buf       [MAX_INPUT_LENGTH];
    char            direction [MAX_INPUT_LENGTH];
    char            arg      [MAX_INPUT_LENGTH];
    int             door;
    int             rev_dir;
     
    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_DEMON) )
       {
          send_to_char("Huh?.\n\r", ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          send_to_char("You are out of breath.\n\r", ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
  	    stc("Who do you wish to unnerve?\n\r", ch);
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
     
    ch->ctimer[0] = 3;

    act("You breathe a gust of cold air at $N.", ch, NULL, victim, TO_CHAR);
    act("$n breathes a gust of cold air at $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n breathes a gust of cold air at you.", ch, NULL, victim, TO_VICT);

    if ( IS_SET(ch->in_room->room_flags, ROOM_ENTOMB) )
       {
          act("The frigid air pushes you into a wall as you land back on your feet.",victim,NULL,NULL,TO_CHAR);
          act("The frigid air pushes $n into a wall as $e lands back on $s feet.",victim,NULL,NULL,TO_ROOM);
          return;
       }

    door = number_range(0,3);
     
    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}
     
    if (( pexit = ch->in_room->exit[door]) == NULL)
    {
	sprintf(buf,"$N flies into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"$N flies into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"You smash into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
      victim->wait += 6;
	return;
    }
    
    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) )
    {
	if (IS_SET(pexit->exit_info, EX_LOCKED))
	    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	if (IS_SET(pexit->exit_info, EX_CLOSED))
	    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	sprintf(buf,"$N is blown into the air and flies %s.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"$N is blown into the air and flies %s.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"You are blown into the air and fly %s.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
	sprintf(buf,"There is a loud crash as $n smashes through the $d.");
	act(buf,victim,NULL,pexit->keyword,TO_ROOM);
     
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir] ) != NULL
	&&   pexit_rev->to_room == ch->in_room
	&&   pexit_rev->keyword != NULL )
	{
	    if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
		REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
		REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    if (door == 0) sprintf(direction,"south");
	    if (door == 1) sprintf(direction,"west");
	    if (door == 2) sprintf(direction,"north");
	    if (door == 3) sprintf(direction,"east");
	    char_from_room(victim);
	    char_to_room(victim,to_room);
	    sprintf(buf,"$n comes smashing in through the %s $d.", direction);
	    act(buf,victim,NULL,pexit->keyword,TO_ROOM);
          victim->wait += 6;
	}
    }
    else
    {
	sprintf(buf,"$N flies %s.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"$N flies %s.", direction);   
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"You fly %s.", direction);  
	act(buf,ch,NULL,victim,TO_VICT);
	if (door == 0) sprintf(direction,"south");
	if (door == 1) sprintf(direction,"west");
	if (door == 2) sprintf(direction,"north");
	if (door == 3) sprintf(direction,"east");
	  char_from_room(victim);
	char_to_room(victim,pexit->to_room);
	sprintf(buf,"$n comes flying in from the %s.", direction);
	act(buf,victim,NULL,NULL,TO_ROOM);
      victim->wait += 6;
    }

    if ( ch == NULL || victim == NULL )
       return;

    if ( ch->in_room != victim->in_room )
       stop_fighting(ch,TRUE);
}

void do_caust(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if (!IS_DEMPOWER(ch, DEM_CAUST) )
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to caust?\n\r", ch);
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

    if (dice(1, 100) == 1)
    {
	act("$p is disintegrated by $n's poison.", ch,obj, NULL, TO_ROOM);
	act("Your poison eats through $p.", ch,obj, NULL, TO_CHAR);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_POISON))
    {
	act("$p is already coated with deadly poison.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("You run your tongue along $p, coating it with a sickly venom.", ch, obj, NULL, TO_CHAR);
    act("$n runs $m tongue along $p, coating it with a sickly venom.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_POISON);
    obj->value[1] += 1000;
    obj->value[2] += 1000;    
}

void do_unnerve(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (!IS_DEMPOWER(ch, DEM_UNNERVE) )
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
  	    stc("Who do you wish to unnerve?\n\r", ch);
	    return;
       }

    if ( ch->ctimer[1] > 0 )
       {
          send_to_char("You are still recovering from the last one.\n\r", ch);
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
	stc("You don't have enough movements for that.\n\r", ch);
	return;
    }

    if ( is_safe(ch,victim) )
       return;

    if ( is_affected( victim, skill_lookup("blessing of the saints") ) )
       {
	    stc("They are protected by the holy saints.\n\r", ch);
	    return;
       }

    act("You say '#y#1Xeus Dominus Mortai!#n'", ch,NULL,NULL, TO_CHAR);
    act("$n says '#y#1Xeus Dominus Mortai!#n'", ch,NULL,NULL, TO_ROOM);
    act("Your body trembles as you are suddenly paralysed!", victim,NULL,NULL, TO_CHAR);
    act("$n's body trembles as $e is suddenly paralysed!", victim,NULL,NULL, TO_ROOM);

    ch->ctimer[1] = 6;
    victim->wait = 18;
    ch->mana -= 1000000;
}

void do_wfreeze(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if (!IS_DEMPOWER(ch, DEM_FREEZEWEAPON) )
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to freeze?\n\r", ch);
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

    if (dice(1, 100) == 1)
    {
	act("$p freezes and shatters.", ch,obj, NULL, TO_ROOM);
	act("$p freezes and shatters.", ch,obj, NULL, TO_CHAR);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_FROST))
    {
	act("$p is already frozen.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("$p is surrounded by ice crystals.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n is surrounded by ice crystals.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_FROST);
}

void do_hellfire (CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_PHOENIX) )
	return;

    if ( ( obj = ch->pcdata->chobj ) != NULL && ch->pcdata->obj_vnum != 92 )
       {
	    send_to_char("You are not Zeik Tuvai... The Revenge Blade.\n\r",ch);
	    return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_FLAMING) )
       {
	    send_to_char("The room is already in flames.\n\r",ch);
	    return;
       }

    SET_BIT(ch->in_room->room_flags, ROOM_FLAMING);

    if ( IS_SET(ch->in_room->room_flags, ROOM_ICYSWORD) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_ICYSWORD);

    stc("#yYou #Rsummon the #yfires #Rof hell!#n\n\r",ch);
    act("#y$n #Rsummons the #eAbyssal #yfires #Rof Hell!#n",ch,NULL,NULL,TO_ROOM);
}

void do_seed(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to plant with Deamonseed?\n\r", ch);
	return;
    }

    if ((obj = get_obj_carry(ch, argument) ) == NULL)
    {
	send_to_char("You do not have that item in your inventory.\n\r",ch);
	return;
    }
  
    if (IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
    {
	send_to_char("That item is already set to explode.\n\r", ch);
	return;
    }

    if (IS_OBJ_STAT(obj, ITEM_BLESS)
	|| IS_OBJ_STAT(obj, ITEM_ANTI_EVIL) )
    {
	send_to_char("You cannot affect such a holy item.\n\r", ch);
	return;
    }

    if (IS_SET(obj->quest, QUEST_ARTIFACT)
	|| IS_SET(obj->quest, QUEST_RELIC) )
    {
	send_to_char("You cannot affect such a powerful item.\n\r", ch);
	return;
    }

    act("$p starts to shudder and vibrate in your hands.", ch, obj, NULL, TO_CHAR);
    act("$n lays $s hands on $p.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 24);
    SET_BIT(obj->extra_flags2, ITEM_DAEMONSEED);
    obj->timer = 3;
}

void do_immolate(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (!IS_DEMPOWER(ch, DEM_IMMOLATE))
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to immolate?\n\r", ch);
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

    if (dice(1, 100) == 1)
    {
	act("$p explodes in a burst of flames.", ch,obj, NULL, TO_ROOM);
	act("$p explodes in a burst of flames.", ch,obj, NULL, TO_CHAR);
	damage(ch,ch,500,gsn_inferno);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_FLAMING))
    {
	act("$p is already flaming.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("$p bursts into flames.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n bursts into flames.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_FLAMING);
}

void do_dinferno(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch, *vch_next;
    int count;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
       {
	    send_to_char( "Huh?\n\r", ch );
	    return;
       }

    if ( !IS_DEMPOWER(ch, DEM_INFERNO) )
       {
          send_to_char("You have not been granted that power.\n\r", ch);
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          send_to_char("You are still recovering from the last one.\n\r", ch);
          return;
       }

	if ( ch->mana < 300000 )
       {
          stc("You are too tired to do this technique.\n\r", ch);
          return;
       }

    if ( ch->hit > ch->max_hit * 0.25 )
       {
          send_to_char("#RThis is only used as a last resort.#n\n\r",ch);
          return;
       }

	ch->mana -= 300000;

    ch->ctimer[2] = 6;
    act("$n explodes, sending forth a massive fireballs in all directions.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You explode in a huge blast of abyssal flame.\n\r",ch);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch,vch) || is_safe(ch,vch) || vch == ch )
          continue;

       for ( count = 0; count < UMIN( 15, ch->pkill ); count++ )
       {
          ch->ele_attack = ATTACK_FIRE;
          ch->attack_type = ATTACK_MAGICAL;
          ch->attack_var = BLAST;
          ch->attack_focus = number_range( 0, 4 );
          one_hit( ch, vch, gsn_fireball, 1 );
       }
    }

    if ( ch->pkill > 9 )
       clear_affects( ch );

    hit_gain( ch, 750000 + UMIN( 750000, ch->pkill * 37500 ) );
}   

void do_blink(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch,CLASS_DEMON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_DEMPOWER( ch, DEM_BLINK) )
       {
          stc("You need to get blink first.\n\r",ch);
          return;
       }

    if ( ch->ctimer[15] > 0 )
       {
          stc( "#wYou are the #gO#wn#ge#w... You don't resort to these #ypetty #wmindless skills.#n\n\r", ch );
          return;
       }

    if ( ch->move < 250000 )
       {
          stc("You are too tired to do this technique.\n\r", ch);
          return;
       }

    argument = one_argument(argument, arg);

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to blink into combat with?\n\r", ch);
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

    ch->move -= 250000;
    WAIT_STATE(ch, 12);

    stc("You pop out of existance.\n\r", ch);
    act("$n pops out of existance.", ch, NULL, NULL, TO_ROOM);
    act("$n pops back into existence next to $N.", ch,NULL,victim, TO_NOTVICT);
    act("You pop back into existence next to $N.", ch, NULL,victim, TO_CHAR);
    act("$n pops back into existence next to you.", ch, NULL,victim, TO_VICT);
    act("You say '#yMuhahahahaha!#n'", ch,NULL,NULL, TO_CHAR);
    act("$n says '#yMuhahahahaha!#n'", ch,NULL,NULL, TO_ROOM);

    multi_hit( ch, victim, TYPE_UNDEFINED );
    show_total_damage( ch, victim );
}

void do_graft(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return; 

    if ( IS_CLASS(ch,CLASS_GAROU) )
       {
          do_wolf_graft( ch, argument );
          return;
       }

    if (!IS_CLASS(ch,CLASS_DEMON))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }

    if ( !IS_DEMPOWER( ch, DEM_GRAFT) )
    {
	stc("You need to get graft first.\n\r",ch);
	return;
    }

    if (IS_SET(ch->newbits, THIRD_HAND) && IS_SET(ch->newbits, FOURTH_HAND))
    {
	stc("You already have four arms!\n\r",ch);
	return;
    }

    act("You cackle with glee as you graft two arms onto your body.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $e grafts two arm onto $s body.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->newbits, FOURTH_HAND);
    SET_BIT(ch->newbits, THIRD_HAND);
}

void do_frostbreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 568 )
       return;

    if ( !IS_NPC( ch ) )
       {
          if ( !IS_CLASS(ch,CLASS_DEMON) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ch->ctimer[3] > 0 )
             {
                send_to_char("You are out of breath.\n\r", ch);
                return;
             }

          if ( arg[0] == '\0' )
             {
                send_to_char("Who do you wish to breathe on?\n\r", ch);
                return;
             }
       }

    if ( IS_NPC( ch ) || !str_cmp(arg, "all") )
       {
          CHAR_DATA *vch, *vch_next;

          for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
          {
             vch_next = vch->next_in_room;

             if ( vch == ch )
                continue;

             if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
                continue;

             frost_breath( ch, vch, TRUE );
          }

          WAIT_STATE( ch, 12 );

          if ( !IS_NPC( ch ) )
             ch->ctimer[3] = 4;

          return;
       }

    if ( ( victim = get_char_room(ch, arg) ) == NULL )
       {
          send_to_char("They aren't here.\n\r", ch);
          return;
       }

    WAIT_STATE( ch, 12 );
    ch->ctimer[3] = 3;
    frost_breath(ch, victim, FALSE);
}

void frost_breath(CHAR_DATA *ch, CHAR_DATA *victim, bool all)
{
    CHAR_DATA *vch, *vch_next;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
    {
       stc("Huh?\n\r",ch);
       return;
    }

    if ( all )
    {
	act("$n breathes forth a stream of frost over you.",ch,NULL,NULL,TO_ROOM);
	act("You breath forth a stream of frost.",ch,NULL,NULL,TO_CHAR);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next_in_room;

	    if ( is_safe(ch,vch) || is_same_group(ch,vch) )
		continue;

          ch->ele_attack = ATTACK_ICE;
          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = BLAST;
          ch->attack_focus = DIRECT;

          if ( IS_NPC( ch ) )
             hurt_person( ch, vch, 500000, TRUE );
             else hurt_person( ch, vch, 750000, TRUE );
	}
	return;
    }

    if (is_safe(ch, victim)) return;

    act("$n breathes forth a stream of frost over $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a stream of frost over you!",ch,NULL,victim,TO_VICT);
    act("You breath forth a stream of frost over $N.",ch,NULL,victim,TO_CHAR);

    ch->ele_attack = ATTACK_ICE;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 750000, TRUE );
}

void do_sin( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 3501 && ch->pIndexData->vnum != 3502 ) 
       return;

    if ( ch->in_room == NULL )
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_DEMON) )
             {
                stc("Huh?\n\r",ch);
                return;
             }

          if ( ch->mana < 500000 )
             {
                stc("You dont have enough mana to sacrifice to Satan.\n\r",ch);
                return;
             }
       }

    if ( ch->fighting == NULL )
       {
          stc("You are not in a fight.\n\r",ch);
          return;
       }

    victim = ch->fighting;

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       {
            stc("They are protected by the holy saints.\n\r",ch);
            return;
       }

    if ( IS_SET(victim->more, MORE_SIN) )
       {
          stc("The victim is already cowering in sin.\n\r",ch);
          return;
       }

    if ( !IS_NPC(ch) )
       ch->mana -= 500000;
 
    if ( number_percent() < 30 )
       {
           act("You pray to the Devil to show his wrath on $N but it laughs at you.",ch,NULL,victim,TO_CHAR);
           ch->wait = 12;
           return;
       }

    act("You pray to the Devil to show his wrath on $N.",ch,NULL,victim,TO_CHAR);
    act("The Devil cackles loudly as it corrupts $N's soul.\n\r",ch,NULL,victim,TO_CHAR);
    act("You see a glince of the hideous afterlife which awaits you.\n\r",victim,NULL,NULL,TO_CHAR);
    act("Your soul is corrupted with the grace of sin.\n\r",victim,NULL,NULL,TO_CHAR);
    SET_BIT(victim->more, MORE_SIN);
}

void do_lava_pit( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DEMON) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( IS_SET(ch->in_room->room_flags, ROOM_LAVAPIT) )
     {
        act("The lava seeps back into the ground as the hole beneath your feet vanish.",ch,NULL,NULL,TO_CHAR);
        act("The lava seeps back into the ground as the hole beneath your feet vanish.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->in_room->room_flags, ROOM_LAVAPIT);
        return;
     }

  if ( ch->mana < 800000 )
     {
        stc("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  act("You cackle with glee as you SmAsH your fist against the ground.",ch,NULL,NULL,TO_CHAR);
  act("$n cackle with glee as $e SmAsHeS $s fist against the ground.",ch,NULL,NULL,TO_ROOM);
  act("A huge spiral of lava bursts from the ground seeping across the room.",ch,NULL,NULL,TO_CHAR);
  act("A huge spiral of lava bursts from the ground seeping across the room.",ch,NULL,NULL,TO_ROOM);

  SET_BIT(ch->in_room->room_flags, ROOM_LAVAPIT);
  ch->wait = 6;
  ch->mana -= 800000;
}

void do_damnation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_CLASS(ch, CLASS_FIEND) )
       {
          do_fiend_damnation( ch, argument );
          return;
       }

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( IS_SET(victim->more3, MORE3_DAMNATION) )
       {
          stc("They are protected by a psisonic shield.\n\r",ch);
          return;
       }

    act("You touch $N as you surround $M with a psisonic shield.",ch,NULL,victim,TO_CHAR);
    act("$n touchs you as $e surrounds you with a psisonic shield.",ch,NULL,victim,TO_VICT);
    act("$n touchs $N as $e surrounds $M with a psisonic shield.",ch,NULL,victim,TO_NOTVICT);
    SET_BIT(victim->more3, MORE3_DAMNATION);
}

void do_inpart( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DEMON))
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Inpart <person> <power>\n\r", ch );
	send_to_char(" Fangs        [ 2500] Claws        [ 2500] Tail        [ 5000]\n\r",ch);
      send_to_char(" Horns        [ 2500] Hooves       [ 1500] Graft       [20000]\n\r",ch);
	send_to_char(" NightSight   [ 3000] Wings        [ 1000] Might       [ 7500]\n\r",ch);
      send_to_char(" Toughness    [ 7500] Speed        [ 7500] Travel      [ 1500]\n\r",ch);
	send_to_char(" Scry         [ 7500] Truesight    [ 7500] Move        [  500]\n\r",ch);
	send_to_char(" Leap         [  500] Magic        [ 1000] Lifespan    [  100]\n\r",ch);
	send_to_char(" Shield       [20000] Longsword    [    0] Shortsword  [    0]\n\r",ch);
	send_to_char(" Inferno      [20000] Unnerve      [ 5000] Entomb      [20000]\n\r",ch);
	send_to_char(" Demonform    [25000] Blink        [15000]\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }


    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }


    if (!str_cmp(arg2,"longsword"))
    {
	send_to_char("You have been granted the power to transform into a demonic longsword!\n\r",victim);
	send_to_char("You grant them the power to transform into a demonic longsword.\n\r",ch);
	victim->pcdata->powers[DPOWER_OBJ_VNUM] = 33120;
	save_char_obj(victim);
	return;
    }

    if (!str_cmp(arg2,"fangs")) 
	{inpart = DEM_FANGS; cost = 2500;}
    else if (!str_cmp(arg2,"inferno"))
	{inpart = DEM_INFERNO; cost = 20000;}
    else if (!str_cmp(arg2,"unnerve"))
	{inpart = DEM_UNNERVE; cost = 5000;}
    else if (!str_cmp(arg2,"entomb"))
	{inpart = DEM_ENTOMB; cost = 20000;}
    else if (!str_cmp(arg2,"claws")) 
	{inpart = DEM_CLAWS; cost = 2500;}
    else if (!str_cmp(arg2,"horns")) 
	{inpart = DEM_HORNS; cost = 2500;}
   else if (!str_cmp(arg2, "demonform"))
	{inpart = DEM_FORM; cost = 25000;} 
   else if (!str_cmp(arg2,"tail")) 
	{inpart = DEM_TAIL; cost = 5000;}
    else if (!str_cmp(arg2,"hooves")) 
	{inpart = DEM_HOOVES; cost = 1500;}
    else if (!str_cmp(arg2,"shield")) 
	{inpart = DEM_SHIELD; cost = 20000;}
    else if (!str_cmp(arg2,"nightsight")) 
	{inpart = DEM_EYES; cost = 3000;}
    else if (!str_cmp(arg2,"wings")) 
	{inpart = DEM_WINGS; cost = 1000;}
    else if (!str_cmp(arg2,"might")) 
	{inpart = DEM_MIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"toughness")) 
	{inpart = DEM_TOUGH; cost = 7500;}
    else if (!str_cmp(arg2,"speed"))
	{inpart = DEM_SPEED; cost = 7500;}
    else if (!str_cmp(arg2,"travel")) 
	{inpart = DEM_TRAVEL; cost = 1500;}
    else if (!str_cmp(arg2,"scry")) 
	{inpart = DEM_SCRY; cost = 7500;}
    else if (!str_cmp(arg2,"move")) 
	{inpart = DEM_MOVE; cost = 500;}
    else if (!str_cmp(arg2,"leap")) 
	{inpart = DEM_LEAP; cost = 500;}
    else if (!str_cmp(arg2,"magic")) 
	{inpart = DEM_MAGIC; cost = 1000;}
    else if (!str_cmp(arg2,"truesight"))
        {inpart = DEM_TRUESIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"graft"))
	{inpart = DEM_GRAFT; cost = 20000;}
    else if (!str_cmp(arg2,"blink"))
        {inpart = DEM_BLINK; cost = 15000;}
    else if (!str_cmp(arg2,"lifespan")) 
	{inpart = DEM_LIFESPAN; cost = 100;}
    else
    {
	do_inpart(ch,"");
	return;
    }

    if (IS_DEMPOWER(victim, inpart))
    {
	send_to_char("They have already got that power.\n\r",ch);
	return;
    }

    if( !IS_CLASS(victim, CLASS_DEMON))
    {
	stc( "You can not inpart this onto nonDemons.\n\r",ch);
	return;
    }

    cost = 0;

    if (ch->pcdata->stats[DEMON_TOTAL] < cost || 
	ch->pcdata->stats[DEMON_CURRENT] < cost)
    {
	send_to_char("You have insufficient power to inpart that gift.\n\r",ch);
	return;
    }

    SET_BIT(victim->pcdata->powers[DPOWER_FLAGS], inpart);
    ch->pcdata->stats[DEMON_CURRENT] -= cost;
    if (victim != ch) send_to_char("You have been granted a demonic gift from your patron!\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    if (victim != ch) save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_demonarmour( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    int vnum = 0;
    int cost = 1;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS( ch, CLASS_FIEND ) )
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
       {
          send_to_char("Please specify which piece of demon armor you wish to make: Ring Collar\n\r", ch );
          send_to_char("Plate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor chaosweapon shortsword.\n\r",ch);
          return;
       }

    else if (!str_cmp(arg,"ring"     ))   vnum = 357;
    else if (!str_cmp(arg,"collar"   ))   vnum = 358;
    else if (!str_cmp(arg,"plate"    ))   vnum = 360;
    else if (!str_cmp(arg,"helmet"   ))   vnum = 361;
    else if (!str_cmp(arg,"leggings" ))   vnum = 362;
    else if (!str_cmp(arg,"boots"    ))   vnum = 363;
    else if (!str_cmp(arg,"gauntlets"))   vnum = 364;
    else if (!str_cmp(arg,"sleeves"  ))   vnum = 365;
    else if (!str_cmp(arg,"cape"     ))   vnum = 366;
    else if (!str_cmp(arg,"belt"     ))   vnum = 367;
    else if (!str_cmp(arg,"bracer"   ))   vnum = 359;
    else if (!str_cmp(arg,"visor"    ))   vnum = 368;
    else if (!str_cmp(arg,"chaosweapon")) { vnum = 355; cost = 10; }
    else if (!str_cmp(arg,"shortsword"))  vnum = 356;
    else
    {
        do_demonarmour(ch,"");
	return;
    }

    if ( ch->gold < cost )
       {
          sprintf( buf, "It costs #w%d #ygold pieces#n to create a piece of demon armour.\n\r", cost );
          stc( buf, ch );
          return;
       }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Jobo.\n\r",ch);
	return;
    }

    ch->gold -= cost;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);

    if ( vnum == 355 )
       {
          obj->value[3] = number_range(0,12);

          switch( number_range( 1, 15 ) )
          {
             case 7:
             case 1: obj->value[0] = 15000; break;
             case 10: obj->value[0] = 16000; break;
             case 14:
             case 2:
             case 3: obj->value[0] = 17000; break;
             case 8:
             case 4: obj->value[0] = 18000; break;
             case 9:
             case 5: obj->value[0] = 20000; break;
             case 12: obj->value[0] = 22000; break;
             case 6: obj->value[0] = 23000; break;
          }

          if ( number_percent() > 50 )
             {
                switch( number_range( 1, 7 ) )
                {
                   case 1: SET_BIT(obj->weapflags, TRUE_FIRE); break;
                   case 2: SET_BIT(obj->weapflags, TRUE_ICE); break;
                   case 3: SET_BIT(obj->weapflags, TRUE_MOON); break;
                   case 4: SET_BIT(obj->weapflags, TRUE_SAINT); break;
                   case 5: SET_BIT(obj->weapflags, TRUE_DARK); break;
                   case 6: SET_BIT(obj->weapflags, TRUE_BOLT); break;
                   case 7: SET_BIT(obj->weapflags, TRUE_EARTH); break;
                }
             }

          if ( number_percent() > 50 )
             {
                AFFECT_DATA *paf;
                int apply_type, modifier;

                switch( number_range( 1, 5 ) )
                {
                   case 1: apply_type = APPLY_STR; modifier = number_range( 1, 3 ); break;
                   case 2: apply_type = APPLY_WIS; modifier = number_range( 1, 3 ); break;
                   case 3: apply_type = APPLY_INT; modifier = number_range( 1, 3 ); break;
                   case 4: apply_type = APPLY_DEX; modifier = number_range( 1, 3 ); break;
                   case 5: apply_type = APPLY_CON; modifier = number_range( 1, 3 ); break;
                }

                if ( affect_free == NULL )
                   paf = alloc_perm( sizeof(*paf) );
                   else {
                           paf = affect_free;
                           affect_free = affect_free->next;
                        }

                paf->type = skill_lookup("enchant weapon");
                paf->duration = -1;
                paf->location = apply_type;
                paf->modifier = modifier;
                paf->bitvector = 0;
                paf->next = obj->affected;
                obj->affected = paf;
             }

          if ( number_percent() > 50 )
             {
                AFFECT_DATA *paf;
                int apply_type, modifier;

                switch( number_range( 1, 3 ) )
                {
                   case 2: apply_type = APPLY_HIT; modifier = number_range( 100000, 300000 ); break;
                   case 3: apply_type = APPLY_MANA; modifier = number_range( 100000, 300000 ); break;
                   case 1: apply_type = APPLY_MOVE; modifier = number_range( 100000, 300000 ); break;
                }

                if ( affect_free == NULL )
                   paf = alloc_perm( sizeof(*paf) );
                   else {
                           paf = affect_free;
                           affect_free = affect_free->next;
                        }

                paf->type = skill_lookup("enchant weapon");
                paf->duration = -1;
                paf->location = apply_type;
                paf->modifier = modifier;
                paf->bitvector = 0;
                paf->next = obj->affected;
                obj->affected = paf;
             }

          obj->value[0] += 24;
       }
}

void do_horns( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch,CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_HORNS) )
    {
	send_to_char("Your horns slide back into your head.\n\r",ch);
	act("$n's horns slide back into $s head.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	return;
    }
    send_to_char("Your horns extend out of your head.\n\r",ch);
    act("A pair of pointed horns extend from $n's head.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
    return;
}

void do_demonform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 

    if (IS_NPC(ch)) return;

    if ( IS_CLASS( ch, CLASS_FIEND ) )
       {
          do_true_demon( ch, "" );
          return;
       }

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg ); 

    if ( !IS_DEMPOWER( ch, DEM_FORM ) && IS_CLASS(ch,CLASS_DEMON) )
    {
	send_to_char("You haven't been granted the gift of demonform.\n\r",ch);
	return;
    }
    
    if ( IS_AFFECTED(ch,AFF_POLYMORPH) )
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM);
	  free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
  
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        act( "You transform into a huge demon.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body grows and distorts into a huge demon.", ch, NULL, NULL, TO_ROOM );

        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        sprintf(buf, "#R%s #ythe #Chuge #yh#Culkin#yg #Rdemon#n", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
}

void do_tail( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_TAIL))
    {
	send_to_char("You haven't been granted the gift of a tail.\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_TAIL) )
    {
	send_to_char("Your tail slides back into your back.\n\r",ch);
	act("$n's tail slides back into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
	return;
    }

    send_to_char("Your tail slides out of your back side.\n\r",ch);
    act("A huge tail extends from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
    return;
}

/*moved*/
void do_hooves( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_HOOVES))
    {
	send_to_char("You haven't been granted the gift of horns.\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_HOOVES) )
    {
	send_to_char("Your hooves transform into feet.\n\r",ch);
	act("$n's hooves transform back into $s feet.", ch, NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
	return;
    }
    send_to_char("Your feet transform into hooves.\n\r",ch);
    act("$n's feet transform into hooves.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    return;
}

void do_wings( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

   if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (!IS_DEMPOWER( ch, DEM_WINGS) && ch->power[DISC_DAEM_ATTA] < 5 && IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("You haven't been granted the gift of wings.\n\r",ch);
	return;
    }

    if (arg[0] != '\0')
    {
	if (!IS_DEMAFF(ch,DEM_WINGS) )
	{
	    send_to_char("First you better get your wings out!\n\r",ch);
	    return;
	}
	if (!str_cmp(arg,"unfold") || !str_cmp(arg,"u"))
	{
	    if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	    {
		send_to_char("But your wings are already unfolded!\n\r",ch);
		return;
	    }
	    send_to_char("Your wings unfold from behind your back.\n\r",ch);
	    act("$n's wings unfold from behind $s back.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	    return;
	}
	else if (!str_cmp(arg,"fold") || !str_cmp(arg,"f"))
	{
	    if (!IS_DEMAFF(ch,DEM_UNFOLDED) )
	    {
		send_to_char("But your wings are already folded!\n\r",ch);
		return;
	    }
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	    return;
	}
	else
	{
	    send_to_char("Do you want to FOLD or UNFOLD your wings?\n\r",ch);
	    return;
	}
    }

    if (IS_DEMAFF(ch,DEM_WINGS) )
    {
	if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
	send_to_char("Your wings slide into your back.\n\r",ch);
	act("$n's wings slide into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
	return;
    }
    send_to_char("Your wings extend from your back.\n\r",ch);
    act("A pair of wings extend from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    return;
}

void do_lifespan( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;

    if ( IS_CLASS( ch, CLASS_FIEND ) )
       {
          do_fiend_lifespan( ch, argument );
          return;
       }

    if (!IS_CLASS(ch,CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if (!IS_DEMPOWER( ch, DEM_LIFESPAN))
    {
	send_to_char("You haven't been granted the gift of lifespan.\n\r",ch);
	return;
    }

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( obj->chobj == NULL || obj->chobj != ch )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_HEAD(ch,LOST_HEAD))
    {
	send_to_char("You cannot change your lifespan in this form.\n\r",ch);
	return;
    }

         if (!str_cmp(arg,"l") || !str_cmp(arg,"long" )) obj->timer = 0;
    else if (!str_cmp(arg,"s") || !str_cmp(arg,"short")) obj->timer = 1;
    else
    {
	send_to_char("Do you wish to have a long or short lifespan?\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_weaponform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int obj_num = 356;

    if (IS_NPC(ch)) return;
   

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION))
    {
	send_to_char( "Huh?\n\r", ch);
	return;
    }

    if (ch->pcdata->powers[DPOWER_OBJ_VNUM] < 1)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }

    if ( ch->pkill > 19 )
       obj_num = 92;

    if ((obj = create_object(get_obj_index(obj_num),60)) == NULL)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_WEBBED))
    {
	send_to_char( "Not with all this sticky webbing on.\n\r", ch);
	return;
    }

    if (ch->fight_timer>0)
    {
       send_to_char("Not with a fighttimer.\n\r",ch);
       return;
    }

	   if ( ch->ctimer[6] > 0 )
	   {
		  stc("You can't do another weaponform yet..\n\r",ch);
	   return;
	   }

    obj_to_room( obj, ch->in_room );
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);

	ch->ctimer[6] = 100;

    ch->pcdata->obj_vnum = obj_num;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    obj->timer = 12;
    SET_BIT( ch->affected_by, AFF_POLYMORPH );
    SET_BIT( ch->extra, EXTRA_OSWITCH );
    free_string( ch->morph );
    ch->morph = str_dup(obj->short_descr);
}

void do_curse( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch,CLASS_DEMON) )
      return;

   one_argument( argument, arg );

   if ( ( obj = ch->pcdata->chobj ) == NULL )
      {
	   send_to_char("You are not in weaponform demon.\n\r",ch);
	   return;
      }

   if ( ch->pcdata->obj_vnum != 92 )
      {
	   send_to_char("You are not Zeik Tuvai... The Revenge Blade.\n\r",ch);
	   return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && arg[0] == '\0' )
      {
	   send_to_char( "Who do you want to curse demon?\n\r", ch );
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

   REMOVE_BIT(ch->extra, EXTRA_OSWITCH);

   if ( is_safe(ch,victim) )
      {
         SET_BIT(ch->extra, EXTRA_OSWITCH);
         return;
      }

   SET_BIT(ch->extra, EXTRA_OSWITCH);

   if ( IS_AFFECTED(victim, AFF_CURSE) )
      {
	   send_to_char( "They are already cursed demon.\n\r", ch );
	   return;
      }

   if ( is_affected( victim, skill_lookup("blessing of the saints") ) )
      {
	   send_to_char( "They are protected by the holy saints.\n\r", ch );
	   return;
      }

   act("You say '#yWhere you think you are going child? Muhahahahaha!!!#n'", ch,NULL,NULL, TO_CHAR);
   act("$n says '#yWhere you think you are going child? Muhahahahaha!!!#n'", ch,NULL,NULL, TO_ROOM);
   act("You glow red as $N's body bursts into flames.", ch, NULL, victim, TO_CHAR);
   act("$n glows red as your body bursts into flames.", ch, NULL, victim, TO_VICT);
   act("$n glows red as $N's body bursts into flames.", ch, NULL, victim, TO_NOTVICT);
  
   if ( !is_martial( victim ) )
      victim->wait += 12;

   SET_BIT( victim->affected_by, AFF_CURSE );

   if ( !IS_AFFECTED(victim, AFF_FLAMING) ) 
      SET_BIT(victim->affected_by, AFF_FLAMING);
}

void do_soulheal( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch,CLASS_DEMON) )
      return;

   one_argument( argument, arg );

   if ( ( obj = ch->pcdata->chobj ) == NULL )
      {
	   send_to_char("You are not in weaponform demon.\n\r",ch);
	   return;
      }

   if ( ch->pcdata->obj_vnum != 92 )
      {
	   send_to_char("You are not Zeik Tuvai... The Revenge Blade.\n\r",ch);
	   return;
      }

   if ( armageddon == FALSE && nogang == 10 )
      {
	   send_to_char( "You can only use this command in group battles and armageddon.\n\r", ch );
	   return;
      }

   if ( arg[0] == '\0' )
      {
	   send_to_char( "Who do you want to heal demon?\n\r", ch );
	   return;
      }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( IS_SET(victim->more, MORE_POISON) )
      REMOVE_BIT(victim->more, MORE_POISON);

   if ( IS_SET(victim->more, MORE_BLIND) )
      {
         REMOVE_BIT( victim->more, MORE_BLIND );
         victim->blind_timer = 0;
      }

   if ( IS_SET(victim->more, MORE_HEX) )
      REMOVE_BIT(victim->more, MORE_HEX);

   act("You say '#yFeeble Creature, Use my life to conqueror all! Muhahahahaha!!!#n'", ch,NULL,NULL, TO_CHAR);
   act("$n says '#yFeeble Creature, Use my life to conqueror all! Muhahahahaha!!!#n'", ch,NULL,NULL, TO_ROOM);
   act("You glow blue as $N's body shimmers with spirals of energy.", ch, NULL, victim, TO_CHAR);
   act("$n glows blue as your body shimmers with spirals of energy.", ch, NULL, victim, TO_VICT);
   act("$n glows blue as $N's body shimmers with spirals of energy.", ch, NULL, victim, TO_NOTVICT);

   ch->wait = 12;
   hit_gain( victim, 250000 );
   mana_gain( victim, 500000 );
   move_gain( victim, 500000 );
}

void do_soullink( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *holder;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch,CLASS_DEMON) )
      return;

   one_argument( argument, arg );

   if ( ( obj = ch->pcdata->chobj ) == NULL )
      {
	   send_to_char("You are not in weaponform demon.\n\r",ch);
	   return;
      }

   if ( ch->pcdata->obj_vnum != 92 )
      {
	   send_to_char("You are not Zeik Tuvai... The Revenge Blade.\n\r",ch);
	   return;
      }

   if ( arg[0] == '\0' )
      {
	   send_to_char( "Who do you want to travel to demon?\n\r", ch );
	   return;
      }

   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( in_kingdom( ch, victim, victim->in_room ) )
      return;

   if ( ch->in_room == victim->in_room )
      {
         send_to_char( "But you are already there.\n\r", ch );
         return;
      }

   act("You say '#yXin zjha durakis bai! Muhahahahaha!!!#n'", ch,NULL,NULL, TO_CHAR);
   act("$n says '#yXin zjha durakis bai! Muhahahahaha!!!#n'", ch,NULL,NULL, TO_ROOM);
   act("You glow white as you fade out of existence.", ch, NULL, NULL, TO_CHAR);
   act("$n glows white as $e fades out of existence.", ch, NULL, NULL, TO_ROOM);

   if ( ( holder = obj->carried_by ) != NULL )
      {
         if ( obj->wear_loc != WEAR_NONE )
            unequip_char( holder, obj );

         obj_from_char(obj);
      }
      else obj_from_room(obj);

   obj_to_char(obj,victim);
}

void do_humanform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("You are already in human form.\n\r",ch);
	return;
    }

    if ( IS_CLASS( ch, CLASS_WISP ) )
       {
          do_plasma_blade( ch, "" );
          return;
       }

    ch->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup("");
    act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
    act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
    extract_obj(obj);
    if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    {
	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_HELL));
    }
}

void do_eyespy( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_DEMON))
    {
      send_to_char("huh.\n\r",ch);
      return;
    }
    if (IS_HEAD(ch,LOST_EYE_L) && IS_HEAD(ch,LOST_EYE_R))
    {
	send_to_char( "But you don't have any more eyes to pluck out!\n\r", ch );
	return;
    }
    if (!IS_HEAD(ch,LOST_EYE_L) && number_range(1,2) == 1)
    {
	act( "You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    else if (!IS_HEAD(ch,LOST_EYE_R))
    {
	act( "You pluck out your right eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s right eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    else
    {
	act( "You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
    {
	make_part(ch,"eyeball");
	return;
    }

    victim = create_mobile( get_mob_index( MOB_VNUM_EYE ) );
    SET_BIT(victim->act, ACT_NOEXP);
    if (victim == NULL)
    {send_to_char("Error - please inform KaVir.\n\r",ch); return;}

    char_to_room( victim, ch->in_room );

    ch->pcdata->familiar = victim;
    victim->wizard = ch;
}

void do_shadow_gate( CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];
   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
	return;

   if ( !IS_CLASS(ch, CLASS_DEMON) || ch->clannum != 2 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 10000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Summon whom?\n\r", ch );
         return;
      }

   if ( ( victim = get_char_area( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }
 
   if ( ch == victim )
      {
         send_to_char("But you're already at yourself!\n\r",ch);
         return;
      }

   if ( IS_NPC( victim ) )
      {
         send_to_char("Not on NPCs!!!\n\r",ch);
         return;
      }

   if ( victim->in_room->vnum == ch->in_room->vnum )
      {
         send_to_char("But they are already here!\n\r",ch);
         return;
      }

   if ( victim->fight_timer == 0 || victim->position == POS_FIGHTING )
      {
         send_to_char("But they aren't currently running for their lives.\n\r",ch);
         return;
      }

   ch->mana -= 10000;
   ch->ctimer[4] = 4;

   act( "You swerve your hand in the air as you draw a demonic symbol.", ch, NULL, NULL, TO_CHAR );
   act( "$n swerves $s hand in the air as $e draws a demonic symbol.", ch, NULL, NULL, TO_ROOM );
   act( "The symbol suddenly pulses with dark energy as it molds into a dimenionsal doorway.", ch, NULL, NULL, TO_CHAR );
   act( "The symbol suddenly pulses with dark energy as it molds into a dimenionsal doorway.", ch, NULL, NULL, TO_ROOM );

   act( "A strange symbol suddenly appears in front of you molding into a dimenionsal doorway.", victim, NULL, NULL, TO_CHAR );
   act( "A strange symbol suddenly appears in front of $n molding into a dimenionsal doorway.", victim, NULL, NULL, TO_ROOM );
   act( "You gasp in terror as you see an image of $N cackling before being sucked into the gate.", victim, NULL, ch, TO_CHAR );
   act( "$n gasps in terror as $e sees an image of $N cackling before being sucked into the gate.", victim, NULL, ch, TO_ROOM );

   char_from_room( victim );
   char_to_room( victim, ch->in_room );

   do_look( victim, "auto");

   if ( ( mount = victim->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );

   if ( is_martial( victim ) )
      victim->wait = 12;
      else victim->wait = 36;

   act( "$n suddenly flies from the swirling vortex as $e crashes into the ground in a heap.", victim, NULL, NULL, TO_ROOM );

   if ( number_percent() > 60 && victim->position > POS_STUNNED )
      victim->position = POS_STUNNED;
}

void do_chaosport( CHAR_DATA *ch, char *argument )
{
  if ( !IS_CLASS(ch, CLASS_DEMON) || ch->clannum != 2 )
     {
         stc("Huh?\n\r",ch);
         return;
     }

  if ( is_affected(ch, skill_lookup("curse of allah") ) )
     {
         stc("God prevents you from using this mystical ability.\n\r",ch);
         return;
     }

  if ( ch->ctimer[6] > 0 )
     {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
     }

  if ( ch->fighting && number_range(1,4) == 3 )
     {	
	   stc("The powers of Chaos fail you.\n\r",ch);
	   act("$n tries in vain to summon a chaos portal.",ch,NULL,NULL,TO_ROOM);
	   WAIT_STATE(ch, 12);
	   return;
     }

  act("$n disappears through a rift in time.",ch,NULL,NULL,TO_ROOM);
  stc("You are swept away through a rift in time.\n\r",ch);

  if ( ch->fighting != NULL )
     stop_fighting( ch, TRUE );

  ch->ctimer[6] = 12;
  get_random_room( ch );

  stc("You reappear in a swirling storm of chaos.\n\r",ch);
  act("$n appears in a swirling wind of Chaos.",ch,NULL,NULL,TO_ROOM);

  do_look(ch,"auto");
  WAIT_STATE(ch, 12);
}

void do_dark_holy(CHAR_DATA *ch, char *argument) 
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 568 ) 
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_DEMON) )
             {
                stc("Huh?\n\r",ch);
                return;
             }

          if ( ch->mana < 800000 )
             {
                stc("You dont have enough mana to sacrifice to Satan.\n\r",ch);
                return;
             }
       }

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

   if ( is_safe(ch,victim) )
      return;

   if ( ( sn = skill_lookup( "dark holy" ) ) < 0 ) 
      return;

   if ( !IS_NPC( ch ) )
      ch->mana -= 800000;

   spelltype = skill_table[sn].target;
   level = ch->spl[spelltype] * 2.7;

   (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
   WAIT_STATE( ch, 18 );
}

void do_hail_storm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 568 )
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS( ch, CLASS_DEMON ) )
             {
                stc("Huh?\n\r",ch);
                return;
             }

          if ( get_stock( ch ) < 100 )
             {
                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                return;
             }

          ch->wait = 12;
          SET_BIT( ch->arena, AFF2_DM );
          subtract_stock( ch, 100 );
       }

    act("A look of concentration crosses your face as you suddenly cackle with glee.",ch,NULL,NULL,TO_CHAR);
    act("A look of concentration crosses $n face as $e suddenly cackles with glee.",ch,NULL,NULL,TO_ROOM);
    act("As you stare at the sky, it suddenly goes red as endless hailstones fall onto the earth.",ch,NULL,NULL,TO_CHAR);
    act("As $n stares at the sky, it suddenly goes red as endless hailstones fall onto the earth.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       stc("You scream loudly as a HUGE explosion RiPs everything in its path!\n\r",vch);
       act("$n screams loudly as a HUGE explosion RiPs through $s body!",vch,NULL,NULL,TO_ROOM);

       ch->ele_attack = ATTACK_FIRE;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BLUNT;
       ch->attack_focus = AERIAL;
       hurt_person( ch, vch, 1000000, TRUE );
   }
}

void do_dark_force( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_DEMON ) )
       {
          stc("Huh?\n\r",ch);
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

    act("You open your hands as a flash of dark energy fills the room!",ch,NULL,NULL,TO_CHAR);
    act("$n opens $s hands as a flash of dark energy fills the room!",ch,NULL,NULL,TO_ROOM);
    act("MaSsIvE eXpLoSiOnS erupts all around you as even darker energy slowly fills the room.",ch,NULL,NULL,TO_CHAR);
    act("MaSsIvE eXpLoSiOnS erupts all around you as even darker energy slowly fills the room.",ch,NULL,NULL,TO_ROOM);
    act("Endless Swords rain down from the heavens striking all in their path!!!",ch,NULL,NULL,TO_CHAR);
    act("Endless Swords rain down from the heavens striking all in their path!!!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( is_same_group(ch,vch) ) 
           continue;

        if ( is_safe(ch, vch) )
           continue;

        act("Your body vibrates as DeAdLy lIgHt implodes within you!!!",vch,NULL,NULL,TO_CHAR);
        act("$n's body vibrates as DeAdLy lIgHt implodes within $m!!!",vch,NULL,NULL,TO_ROOM);

        ch->ele_attack = ATTACK_DARK;
        ch->attack_type = ATTACK_MAGICAL;
        ch->attack_var = BEAM;
        ch->attack_focus = UPPERCUT;
        hurt_person( ch, vch, 2000000, TRUE );
    }

    act("Suddenly, the dark energy explodes with a flash of brilliant light!!!",ch,NULL,NULL,TO_CHAR);
    act("Suddenly, the dark energy explodes with a flash of brilliant light!!!",ch,NULL,NULL,TO_ROOM);
    act("The room bursts into demonic flames as your eyes glow red!",ch,NULL,NULL,TO_CHAR);
    act("The room bursts into demonic flames as $n's eyes glow red!",ch,NULL,NULL,TO_ROOM);
}

void do_soul_dream( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_DEMON) || ch->clannum != 2 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_KIOTIC) )
       {
          act("You banish the souls back to the depths of hell.",ch,NULL,NULL,TO_CHAR);
          act("You sigh a breath of relief as $n banishes the souls to the depths of hell.",ch,NULL,NULL,TO_ROOM);
          REMOVE_BIT(ch->in_room->room_flags, ROOM_KIOTIC);
          return;
       }

    if ( ch->mana < 20000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
    act("You exclaim 'Foul creatures from the deepest pits, fill this room with Chaos!'",ch,NULL,NULL,TO_CHAR);
    act("$n exclaims 'Foul creatures from the deepest pits, fill this room with Chaos!'",ch,NULL,NULL,TO_CHAR);
    act("The room suddenly bursts with an outpour of demonic energy, blankening everything.",ch,NULL,NULL,TO_CHAR);
    act("The room suddenly bursts with an outpour of demonic energy, blankening everything.",ch,NULL,NULL,TO_ROOM);
    act("You breathe in deeply as transparent spirits rise from the ground, floating all around you.",ch,NULL,NULL,TO_CHAR);
    act("$n breathes in deeply as transparent spirits rise from the ground, floating all around you.",ch,NULL,NULL,TO_ROOM);

    SET_BIT(ch->in_room->room_flags, ROOM_KIOTIC);
    ch->wait = 12;
    ch->mana -= 20000;
}

void do_firewall( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_DEMON) && !( IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_SOR ) ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( IS_SET(ch->itemaffect, ITEMA_FIREWALL) )
      {
         REMOVE_BIT(ch->itemaffect, ITEMA_FIREWALL);
	   send_to_char( "#RThe fire surrounding your body fades away...#n\n\r", ch );
         return;
      }

   if ( ch->mana < 2500000 )
      {
	   send_to_char( "You dont have enough magical energy for that.\n\r", ch );
         return;
      }

   act("You say, '#yJue Mani Blaz Ze!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yJue Mani Blaz Ze!!#n'",ch,NULL,NULL,TO_ROOM);
   act("#RFocusing on your magick, you are suddenly surrounded by a massive wall of #yblazing #Rfire.#n",ch,NULL,NULL,TO_CHAR);
   act("#RFocusing on $s magick, $n #Ris suddenly surrounded by a massive wall of #yblazing #Rfire.#n",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->itemaffect, ITEMA_FIREWALL);
   ch->wait = 12;
}

void do_protection( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DEMON ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 18 )
      {
         stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
         return;
      }

   if ( ch->mana < 5000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( sn = skill_lookup( "demi protection" ) ) < 0 )
      return;

   if ( !str_cmp("group",arg) )
      {
         ch->wait = 8;

         for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
         {
            vch_next = vch->next_in_room;

            if ( ch != vch && !is_same_group( ch, vch ) )
               continue;

            if ( !is_affected( vch, sn ) )
               spell_prot( sn, 200, ch, vch );
         }

         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to protect?\n\r", ch );
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

   ch->mana -= 5000000;
   ch->wait = 8;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already protected.\n\r",ch);
         return;
      }

   spell_prot( sn, 200, ch, victim );
}

void do_desanctify( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if ( ch->in_room == NULL )
       return;

    if ( !IS_CLASS(ch, CLASS_DEMON) && !IS_SET( ch->cmbt[7], SKL_DESANCTIFY ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 2000000 )
       {
          stc("You dont have enough mana to sacrifice to Satan.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->bits,NON_SAFE) )
    {
	send_to_char( "The room is already non-safe.\n\r", ch );
	return;
    }

    stc("You fall to your knees and pray silently to the Devil to curse this holy room.\n\r",ch);
    stc("The room is no longer a safe haven.\n\r",ch);
    SET_BIT(ch->in_room->bits,NON_SAFE);

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
       REMOVE_BIT(ch->in_room->room_flags,ROOM_SAFE);

    ch->mana -= 2000000;
}

void do_zombie( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 4 )
      {
         stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You dont have enough mana to sacrifice to Satan.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to turn into a zombie?\n\r", ch );
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

   if ( IS_SET( victim->extra, EXTRA_ZOMBIE ) ) 
      {
         stc( "But they are already one of the undead.\n\r", ch );
         return;
      }


   ch->mana -= 1500000;
   SET_BIT( victim->extra, EXTRA_ZOMBIE );

   act("You say '#1Thi Mortai Cor!#n'", ch,NULL,NULL, TO_CHAR);
   act("$n says '#1Thi Mortai Cor!#n'", ch,NULL,NULL, TO_ROOM);
   act("Your skin becomes bloated as your eyes turn silky!", victim,NULL,NULL, TO_CHAR);
   act("$n's skill becomes bloated as $s eyes turn silky!", victim,NULL,NULL, TO_ROOM);
   act("#oYou are now one of the undead!#n", victim,NULL,NULL, TO_CHAR);
   act("#o$n is now one of the undead!#n", victim,NULL,NULL, TO_ROOM);
}

void do_shed( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    CHAR_DATA *opponent = NULL;
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    EXIT_DATA *pexit;
    int door;
    int attempt;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 6 )
       {
          stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
          return;
       }

    if ( ch->move < 5000000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[7] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->ctimer[7] = 6;
    ch->move -= 5000000;

    victim = create_mobile( get_mob_index( 1 ) );
    victim->sex = ch->sex;
    victim->level = 980;
    victim->damroll = get_curr_str( ch );
    victim->max_hit = ch->max_hit;
    victim->hit = ch->hit;
    victim->mana = victim->max_mana = ch->max_mana;
    victim->move = victim->max_move = ch->max_move;
    victim->name = str_dup(ch->name);
    victim->short_descr = str_dup(ch->name);

    victim->practice = 2;
    SET_BIT(victim->act, ACT_TIMER);
    SET_BIT( victim->act, ACT_NINJA );
    sprintf( buf, "%s is hovering here.\n\r", ch->name );

    victim->long_descr = str_dup(buf);
    victim->itemaffect = ch->itemaffect;
    victim->affected_by = ch->affected_by;

    char_to_room( victim, ch->in_room );

    send_to_char("#eYou #Rshed #eyour skin creating a #wperfect #Cduplicate #eof yourself.#n\n\r", ch );
    act("#e$n #Rsheds #e$s skin creating a #wperfect #Cduplicate #eof $mself.#n", ch, NULL, NULL, TO_ROOM );

    if ( ch->fighting != NULL )
       {
           opponent = ch->fighting;

           stop_fighting( opponent, FALSE );

           set_fighting( opponent, victim );
           set_fighting( victim, opponent );

           was_in = ch->in_room;

           for ( attempt = 0; attempt < 6; attempt++ ) 
           {
              door = number_door( );

              if ( ( pexit = was_in->exit[door] ) == 0
                   ||   pexit->to_room == NULL
                   ||   IS_SET(pexit->exit_info, EX_CLOSED)
                   || ( IS_NPC(ch) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
                 continue;

              move_char( ch, door );

              if ( ( now_in = ch->in_room ) == was_in )
                 continue;

              clear_affects( ch );
              send_to_char("#wCackling #ewith #Pglee#e, you quickly #Rflee#e from battle.#n\n\r", ch );

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

              ch->in_room = was_in;
              add_tracks( ch, door );

              ch->in_room = now_in;
              stop_fighting( ch, TRUE );

              if ( !IS_CLASS( opponent, CLASS_WISP ) )
                 opponent->wait = 12;

              SET_BIT( opponent->more, MORE_BLIND );
              SET_BIT( opponent->more, MORE_HEX );
              return;
           }
       }
}

void do_battle_lust( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_DEMON) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 8 )
     {
        stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
        return;
     }

  if ( ch->mana < 5000000 )
  {
    stc("You don't have enough magical energy for this.\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "battle lust" ) ) < 0 ) return;

  if ( is_affected(victim,skill_lookup("battle lust")) )
  {
    stc("The person is already filled with battle lust!\n\r",ch);
    return;
  }

  ch->mana -= 5000000;
  level = 30;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 12 );
}

void do_demon_cradle( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->more3, MORE3_SNAKE ) )
      {
         stc("But you aren't in the snake stance.\n\r",ch);
         return;
      }

   if ( ch->move < 1000000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slam into?\n\r", ch );
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

   ch->move -= 1000000;
   ch->wait = 12;

   act("You float towards $N as you fade out of sight...", ch, NULL, victim, TO_CHAR);
   act("$n floats towards you as $e fades out of sight...", ch, NULL, victim, TO_VICT);
   act("$n floats towards $N as $e fades out of sight...", ch, NULL, victim, TO_NOTVICT);
   act("Reappearing in front of $N, you quickly punch $M hard in the guts!", ch, NULL, victim, TO_CHAR);
   act("Reappearing in front of you, $n quickly punches you hard in the guts!", ch, NULL, victim, TO_VICT);
   act("Reappearing in front of $N, $n quickly punches $M hard in the guts!", ch, NULL, victim, TO_NOTVICT);
   act("You then twirl your body as you slam yourself up $N's chest!!!", ch, NULL, victim, TO_CHAR);
   act("$n then twirls $s body as $e slams $mself up your chest!!!", ch, NULL, victim, TO_VICT);
   act("$n then twirls $s body as $e slams $mself up $N's chest!!!", ch, NULL, victim, TO_NOTVICT);
   act("You finally rip a dark spiral of light across $N's face as $E spawls away!", ch, NULL, victim, TO_CHAR);
   act("$n finally rips a dark spiral of light across your face as you spawl away!", ch, NULL, victim, TO_VICT);
   act("$n finally rips a dark spiral of light across $N's face as $E spawls away!", ch, NULL, victim, TO_NOTVICT);

   if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more3, MORE3_CONFUSE )
        && ( victim->in_room->vnum < 33500 || victim->in_room->vnum > 33800 ) )
      SET_BIT( victim->more3, MORE3_CONFUSE );

   if ( number_percent() > 70 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more, MORE_CONFUSION ) )
      SET_BIT( victim->more, MORE_CONFUSION );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = UPPERCUT;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_break_through( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;

   if ( IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 14 )
      {
         stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ch->ctimer[10] > 0 )
      {
	   send_to_char("You are still recovering from the last one.\n\r",ch);
	   return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 92 && obj->pIndexData->vnum != 91 ) )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 92 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You need the Revenge Blade to utilize this technique.\n\r", ch );
            return;
         }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to curse?\n\r", ch );
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

   if ( IS_SET( victim->elemental, ELE_BTHROUGH ) )
      {
         stc("Their elemental resistance had already been nullified dark one.\n\r", ch );
         return;
      }

   ch->ctimer[10] = 6;

   send_to_char("You concentrate a moment.\n\r",ch);
   act("$n stops and concentrates a moment.\n\r",ch,NULL,NULL, TO_ROOM);

   act("You say, '#yFeel the might of the cursed blade...'#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yFeel the might of the cursed blade...'#n'",ch,NULL,NULL,TO_ROOM);

   act("You snarl at $N, '#yTremble before the destroyer of the heavens!'#n'",ch,NULL,victim,TO_CHAR);
   act("$n snarls at you, '#yTremble before the destroyer of the heavens!'#n'",ch,NULL,victim,TO_VICT);
   act("$n snarls at $N, '#yTremble before the destroyer of the heavens!'#n'",ch,NULL,victim,TO_NOTVICT);

   act("You raise the Revenge Blade above your head as it gets struck by a lightning bolt.",ch,NULL,NULL,TO_CHAR);
   act("$n raises the Revenge Blade above $s head as it gets struck by a lightning bolt.",ch,NULL,NULL,TO_ROOM);

   act("The heavens roar with fury as you thrust your blade through $N's chest!!!",ch,NULL,victim,TO_CHAR);
   act("The heavens roar with fury as $n thrusts $s blade through your chest!!!",ch,NULL,victim,TO_VICT);
   act("The heavens roar with fury as $n thrusts $s blade through $N's chest!!!",ch,NULL,victim,TO_NOTVICT);

   act("#eThe darkness cackles at you as you no longer are blessed with #Pelemental #Rresistance#w.#n",victim,NULL,NULL,TO_CHAR);

   SET_BIT( victim->elemental, ELE_BTHROUGH );
}

void do_revenga( CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( !can_do_skill( ch, 59 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   act("You quickly unsheath your blade as you snarl '#eNo mercy...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n quickly unsheathes $s blade as $e snarls '#eNo mercy...#n'",ch,NULL,NULL,TO_ROOM);
   act("You then cackle with glee as you clench your fists above you.",ch,NULL,NULL,TO_CHAR);
   act("$n then cackles with glee as $e clenches $s fists above $m.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yI am going to KILL YOU ALL!!!! AHHHHhhHHhHHHhh!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yI am going to KILL YOU ALL!!!! AHHHHhhHHhHHHhh!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("The room suddenly goes dark as you dash towards your foes...",ch,NULL,NULL,TO_CHAR);
   act("The room suddenly goes dark as $n dashes towards $s foes...",ch,NULL,NULL,TO_ROOM);

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;
   SET_BIT( ch->arena, AFF2_DM );

   if ( !IS_SET( ch->in_room->area->bits, ROOM_TOTAL_DARKNESS ) )
      SET_BIT( ch->in_room->area->bits, ROOM_TOTAL_DARKNESS );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
  
      if ( is_safe(ch, vch) )
         continue;

      act("You scream in pain as a #Pviolent #rarc#n of #slight#n RiPs you apart!!!",vch,NULL,NULL,TO_CHAR);
      act("$n screams in pain as a #Pviolent #rarc#n of #slight#n RiPs $m apart!!!",vch,NULL,NULL,TO_ROOM);

      ch->ele_attack = ATTACK_DARK;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = number_range( 0, 4 );
      ch->attack_var = SLASH;
      hurt_person( ch, vch, 2500000, TRUE);
   }

   act("You quickly sheath your blade as you scream '#yDragon Fang Sword of AWAKENING!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n quickly sheathes $s blade as $e screams '#yDragon Fang Sword of AWAKENING!!!#n'",ch,NULL,NULL,TO_ROOM);
}

void do_crucifix( CHAR_DATA *ch, char *argument )
{
   DESCRIPTOR_DATA *d;
   int bonus = 0;
   CHAR_DATA *vch;

   if ( IS_NPC(ch) )
      return;
    
   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 12 )
      {
         stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
         return;
      }

   if ( ch->ctimer[11] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->hit > 3000000 )
      {
         stc("You aren't in times of need dark one.\n\r",ch);
         return;
      }

   act("You clutch your fist in rage screaming '#yI will save the demons!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n clutches $s fist in rage screaming '#yI will save the demons!#n'",ch,NULL,NULL,TO_ROOM);
   act("You then swirl your hand around your body forming a mystical aura.",ch,NULL,NULL,TO_CHAR);
   act("$n then swirls $s hand around $s body forming a mystical aura.",ch,NULL,NULL,TO_ROOM);
   act("The aura merge into a cross as you find yourself being slammed onto it.",ch,NULL,NULL,TO_CHAR);
   act("The aura merge into a cross as $n finds $mself being slammed onto it.",ch,NULL,NULL,TO_ROOM);
   act("A loud voice booms from above '#wLost one... The time to repent is now!#n'",ch,NULL,NULL,TO_CHAR);
   act("A loud voice booms from above '#wLost one... The time to repent is now!#n'",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yKILL ALL THE DOGS!#n' as you are struck by a MASSIVE BOLT!",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yKILL ALL THE DOGS!#n' as $e is struck by a MASSIVE BOLT!",ch,NULL,NULL,TO_ROOM);

   ch->ctimer[11] = 8;
   ch->wait = 12;
   ch->hit -= 3000000;
   update_pos( ch );

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->character == NULL )
         continue;

      vch = d->character;

      if ( IS_CLASS( vch, CLASS_DEMON ) )
         bonus += 5;
         else if ( IS_CLASS( vch, CLASS_DROW ) || IS_CLASS( vch, CLASS_FIEND ) )
                 bonus += 4;
         else if ( IS_CLASS( vch, CLASS_VAMPIRE ) || IS_CLASS( vch, CLASS_UNDEAD_KNIGHT ) )
                 bonus += 3;
         else if ( IS_CLASS( vch, CLASS_NECROMANCER ) || IS_CLASS( vch, CLASS_SHADOW_WAR ) )
                 bonus += 2;
         else if ( vch->alignment < -100 )
                 bonus += 1;
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->character == NULL || d->character == ch || d->character->alignment > -350 )
         continue;

      act( "A voice tells you '#eAvenge $N!#n' as you are filled with DeMoNiC power!", d->character, NULL, ch, TO_CHAR );
      act( "$n is suddenly filled with DeMoNiC power!", d->character, NULL, NULL, TO_ROOM );
      spell_demonic_might( skill_lookup("demonic might"), bonus, d->character, d->character );
   }
}

void do_endo_zone( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;
    
   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "#eYour #Pfaith #ein the #pdarkness #eisn't powerful enough as yet dark one.#n\n\r", ch );
         return;
      }

   if ( endozone > 0 )
      {
         stc("The endozone has already been summoned dark one.\n\r",ch);
         return;
      }

   if ( ch->ctimer[12] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( IS_IMMORTAL( ch ) )
      {
         act("You clutch your fist in rage screaming '#yThey shall know my fury!#n'",ch,NULL,NULL,TO_CHAR);
         endozone = 12;
         return;
      }

   ch->ctimer[12] = 24;

   endozone = 4;
   act("You clutch your fist in rage screaming '#yYou shall know my fury!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n clutches $s fist in rage screaming '#yYou shall know my fury!#n'",ch,NULL,NULL,TO_ROOM);
   act("You then swirl your hand around your body forming a mystical aura.",ch,NULL,NULL,TO_CHAR);
   act("$n then swirls $s hand around $s body forming a mystical aura.",ch,NULL,NULL,TO_ROOM);
   act("You push your palm into the skies as the earth trembles beneath you.",ch,NULL,NULL,TO_CHAR);
   act("$n pushes $s palm into the skies as the earth trembles beneath you.",ch,NULL,NULL,TO_ROOM);
   act("Cackles of glee fill the air as the ground seeps with blood.",ch,NULL,NULL,TO_CHAR);
   act("Cackles of glee fill the air as the ground seeps with blood.",ch,NULL,NULL,TO_ROOM);
   act("Gazing at the heavens, you see two glowing eyes of fury...",ch,NULL,NULL,TO_CHAR);
   act("Gazing at the heavens, you see two glowing eyes of fury...",ch,NULL,NULL,TO_ROOM);

   do_info( NULL, "#eThe #CEndo #PZone #ehas been summoned... #w#zNoone#n #eis #Psafe#e...#n" );
}

void do_yami_urasugi( CHAR_DATA *ch, char *argument)
{
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA       *pexit;
   CHAR_DATA       *vch, *vch_next;
   char            arg1      [MAX_INPUT_LENGTH];
   int             door;
   int             count;

   argument = one_argument( argument, arg1 );

   if ( !IS_CLASS(ch, CLASS_DEMON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( arg1[0] == '\0' )
      {
	   send_to_char("Please specify a direction to release your wrath.\n\r", ch);
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

   act("You clutch your fist in rage snarling '#yIf only... I have... more power...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n clutches $s fist in rage snarling '#yIf only... I have... more power...#n'",ch,NULL,NULL,TO_ROOM);
   act("You collapse onto your knees as your body convulses with pure fury.",ch,NULL,NULL,TO_CHAR);
   act("$n collapses onto $s knees as $s body convulses with pure fury.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yI SHALL DESTROY YOU ALL!!!#n' as your body burst into demonic flames.",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yI SHALL DESTROY YOU ALL!!!#n' as $s body burst into demonic flames.",ch,NULL,NULL,TO_ROOM);
   act("You then open your right fist as it releases a sprout of flames!",ch,NULL,NULL,TO_CHAR);
   act("$n then opens $s right fist as it releases a sprout of flames!",ch,NULL,NULL,TO_ROOM);
   act("You snarl '#yThis is the hand that shall save the demons...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n snarls '#yThis is the hand that shall save the demons...#n'",ch,NULL,NULL,TO_ROOM);
   act("#PYou #esay '#RTAKE THIS!!!#e' as you release a #RMASSIVE #ywave of #Rflames #eto the $T#e!#n",ch,NULL,dir_name[door],TO_CHAR);
   act("#P$n #esays '#RTAKE THIS!!!#e' as $e releases a #RMASSIVE #ywave of #Rflames #eto the $T#e!#n",ch,NULL,dir_name[door],TO_ROOM);

   for ( count = 0; count < 4; count++ )
   {
      for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next_in_room;

         if ( is_same_group( ch, vch ) || is_safe( ch, vch ) )
            continue;

         act("A TREMEDOUS burst of demonic flames sends you CATAPULTING into the air!",vch,NULL,NULL,TO_CHAR);
         act("A TREMEDOUS burst of demonic flames sends $n CATAPULTING into the air!",vch,NULL,NULL,TO_ROOM);

         vch->agg = 100;

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_DARK;
         ch->attack_focus = UPPERCUT;
         ch->attack_var = BLAST;
         hurt_person( ch, vch, 3000000 , TRUE );

         if ( vch == NULL || vch->in_room == NULL )
            continue;

         vch->air_timer = 3;
         stop_fighting( vch, TRUE );
      }

      if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
         {
            act("#eThe #Rwall #etrembles slightly as it #Rabsorbs #ea ferocious #Rblast#e.#n",ch,NULL,NULL,TO_CHAR);
            act("#eThe #Rwall #etrembles slightly as it #Rabsorbs #ea ferocious #Rblast#e.#n",ch,NULL,NULL,TO_ROOM);

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
