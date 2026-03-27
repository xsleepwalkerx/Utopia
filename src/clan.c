/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,
 *
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

/*
 * Local functions.
 */

void	adv_spell_damage	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_affect	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_action	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	birth_write		args( ( CHAR_DATA *ch, char *argument ) );
bool	birth_ok		args( ( CHAR_DATA *ch, char *argument ) );

/* Need to get rid of those flames somehow - KaVir */

void do_level( CHAR_DATA *ch, char *argument )
{
    char arg       [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char skill  [20];  
    one_argument( argument, arg );

    if (IS_NPC(ch)) return;                     
    sprintf(buf, "#w   -=+=-=+=-=+=-=+=-=+=-#r[ #c   D . O . A   #r ]#w-=+=-=+=-=+=-=+=-=+=-#n\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "                           #cWeapons Skills#n\n\r\n\r");
    send_to_char(buf,ch);
    sprintf(buf, " #eUnarmed:#w %3d  #ySlash:#w %3d  #cSlice:#w %3d    #pStab:#w %3d  #bWhip:#w %3d\n\r",
                     ch->wpn[0],         ch->wpn[3],       ch->wpn[1],       ch->wpn[2],       ch->wpn[4]);
    send_to_char(buf,ch);
    sprintf(buf, "   #rBlast:#w %3d   #cClaw:#w %3d  #gPound:#w %3d   #oCrush:#w %3d  #wGrep:#w %3d\n\r",
                       ch->wpn[6],        ch->wpn[5],      ch->wpn[7],        ch->wpn[8],       ch->wpn[9]);
    send_to_char(buf,ch);
    sprintf(buf, "                #yBite:#w %3d   #wSuck:#w %3d  #pPierce:#w %3d\n\r",
                                          ch->wpn[10],     ch->wpn[12],     ch->wpn[11]);
    send_to_char(buf,ch);

    sprintf(buf, " #w-=+=-=+=-=+=-=+=-=+=-=+=-=+=-=+=***=+=-=+=-=+=-=+=-=+=-=+=-=+=-=+=-#n\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "                              #cSpells#n\n\r\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "   #eTempest#w: %3d   #rRed#w: %3d   #g Green#w: %3d   #wWhite#w: %3d   #bBlue#w: %3d\n\r",
                        ch->spl[0],       ch->spl[1],     ch->spl[3],       ch->spl[2],      ch->spl[4]);
    send_to_char(buf,ch);
    sprintf(buf, " #w-=+=-=+=-=+=-=+=-=+=-=+=-=+=-=+=***=+=-=+=-=+=-=+=-=+=-=+=-=+=-=+=-\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "                           #cMisc and Class#n\n\r\n\r");
    send_to_char(buf,ch);
    sprintf(skill, " Recall Room : %d          \n\r",ch->home);
    send_to_char( skill, ch );

    sprintf(buf, "#w   -=+=-=+=-=+=-=+=-=+=-#r[ #c   D . O . A   #r ]#w-=+=-=+=-=+=-=+=-=+=-#n\n\r");
    send_to_char(buf,ch);
}

void do_smother( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Smother whom?\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(victim, AFF_FLAMING))
    {
	send_to_char( "But they are not on fire!\n\r", ch );
	return;
    }

    act( "You manage to smother the flames around $M!",  ch, NULL, victim, TO_CHAR    );
    act( "$n manages to smother the flames around you!", ch, NULL, victim, TO_VICT    );
    act( "$n manages to smother the flames around $N!",  ch, NULL, victim, TO_NOTVICT );
    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
}

void horn args((CHAR_DATA *ch));

/* claw and fang commands */

void declaw(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_CLAWS)) return;

    send_to_char("You retract your claws.\n\r",ch);
    act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
}

void dehorn(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_HORNS)) return;

    send_to_char("Your horns burrow back into your skull.\n\r",ch);
    act("$n's horns retract into $s skull.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void horn(CHAR_DATA *ch)
{
    OBJ_DATA *obj;
  
    if (IS_VAMPAFF(ch, VAM_HORNS)) return;

    /* Only certain demons may use their horns */
    if (ch->power[DISC_DAEM_ATTA] < 4) return;

    send_to_char("Sharp horns grow out of your skull.\n\r",ch);
    act("Sharp horns grow out of $n's skull.",ch, NULL, NULL, TO_ROOM);

    if ( (obj = get_eq_char(ch, WEAR_HEAD)) != NULL) 
    { 
        act("Your horns shred $p.", ch, obj, NULL, TO_CHAR); 
        act("$n's horns shred $p.", ch, obj, NULL, TO_ROOM); 
        extract_obj(obj); 
    } 

    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void defang(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_FANGS)) return;

    send_to_char("Your fangs slide back into your gums.\n\r",ch);
    act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
}

void do_claws(CHAR_DATA *ch, char *argument)
{
   if ( !IS_CLASS(ch, CLASS_DRAGON) && !IS_CLASS(ch, CLASS_VAMPIRE)
      && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS( ch, CLASS_WEREWOLF ) )
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_CLAWS))
   {
	send_to_char("You retract your claws.\n\r",ch);
	act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
   else
   {
	send_to_char("Wicked claws extend from your fingers.\n\r",ch);
	act("Long sharp claws extend from $n's fingers.",ch, NULL, NULL, TO_ROOM);
	remove_obj( ch, WEAR_WIELD, TRUE );
	remove_obj( ch, WEAR_HOLD, TRUE );
	remove_obj( ch, WEAR_THIRD, TRUE );
	remove_obj( ch, WEAR_FOURTH, TRUE );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
}

void do_fangs(CHAR_DATA *ch, char *argument)
{
    if (ch->cur_form != 0 && !form_data[ch->cur_form].can_use_claws )
    {
	send_to_char("You cannot use your fangs in this form.\n\r", ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_FANGS) && ch->power[DISC_DAEM_ATTA] < 2)
	{
	    send_to_char("You haven't been granted the gift of fangs.\n\r",ch);
	    return;
	}
    }

   else if ( !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DRAGON)
             && !IS_CLASS( ch, CLASS_WEREWOLF ) )
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_FANGS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your fangs while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("Your fangs slide back into your gums.\n\r",ch);
	act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
   else
   {
	send_to_char("Sharp fangs slide out of your gums.\n\r",ch);
	act("Sharp fangs slide out of $n's gums.",ch, NULL, NULL, TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
}

void do_nightsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_EYES))
	{
	    send_to_char("You haven't been granted the gift of nightsight.\n\r",ch);
	    return;
	}
    }
    else if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->gifts[OWL_TOTEM] < 1 )
    {
      stc("You must obtain level one in Owl to use nightsight.\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
	return;
    }
    send_to_char("Your eyes start glowing red.\n\r",ch);
    act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    return;
}

void do_shadowsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
    {
	send_to_char("You can no longer see between planes.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 10)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->gifts[OWL_TOTEM] < 2 )
    {
      stc("You must obtain level two in Owl to use shadowsight.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(5,10);
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}

void do_generation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int gen;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: generation <char> <generation>.\n\r", ch );
      send_to_char("Generation 1 is a Master <Class> and 2 is clan leader.\n\r", ch);
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    
    gen = is_number( arg2 ) ? atoi( arg2 ) : -1;

    send_to_char("Generation Set.\n\r",ch);
    victim->generation = gen;
    return;
}

void reset_weapon( CHAR_DATA *ch, int dtype ) {
	if ( ch->wpn[dtype] > 200 )
		ch->wpn[dtype] = 200;
	return;
}

void reset_spell( CHAR_DATA *ch, int dtype )
{
	if ( ch->spl[dtype] > 200 )
		ch->spl[dtype] = 200;
	return;
}

void do_shadowplane( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
   
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_HOBBIT) && !IS_CLASS(ch, CLASS_SHADOW_WAR) )
    {
       if ( !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_SHADOW) && !IS_SET( ch->cmbt[7], SKL_PLANAR ) )
          {
             stc("Huh?\n\r",ch);
             return;
          }
    }

   if ( IS_CLASS(ch, CLASS_HOBBIT) && is_oring( ch ) == FALSE )
      {
         send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
         return;
      }

    if ( ch->fight_timer > 0 )
       {
          stc("Not with a fight timer.\n\r",ch);
          return;
       }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 75)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(65,75);

    if ( arg[0] == '\0' )
    {
    	if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	{
            if (has_timer(ch)) return;
	    send_to_char("You fade into the plane of shadows.\n\r",ch);
	    act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    do_look(ch,"auto");
	    return;
    	}
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You fade back into the real world.\n\r",ch);
	act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
    	send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}


void do_darkheart( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_SERP] < 1  )
    {
        send_to_char("You must obtain at least level 1 in Serpentis to use Darkheart.\n\r",ch);
	return;
    }
    if (IS_IMMUNE(ch,IMM_STAKE) )
    {
	send_to_char("But you've already torn your heart out!\n\r",ch);
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 1000 / (10 - ch->generation) )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= 100;
    send_to_char("You rip your heart from your body and toss it to the ground.\n\r",ch);
    act("$n rips $s heart out and tosses it to the ground.", ch, NULL, NULL, TO_ROOM);
    make_part( ch, "heart" );
    ch->hit = ch->hit - number_range(10,20);
    update_pos(ch);
    if (ch->position == POS_DEAD && !IS_HERO(ch))
    {
	send_to_char( "You have been KILLED!!\n\r\n\r", ch );
	raw_kill(ch);
	return;
    }
    SET_BIT(ch->immune, IMM_STAKE);
    return;
}

void do_truesight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses increase to incredible proportions.\n\r", ch );
    }

    return;
}

void do_scry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];
    int rand_room;
    bool chaosvalue = FALSE;
    bool safe = FALSE;
    bool icy = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->trust > 6 )
       {
          send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
          send_to_char("Someone is scrying you.\n\r",victim);
          return;
       }

    if ( IS_SET(victim->more3, MORE3_DAMNATION) )
       {
          act("You scream in agony as a flare of energy ROARS through your defenseless mind!",ch,NULL,NULL,TO_CHAR);
          act("$n screams in agony as a flare of energy ROARS through $s defenseless mind!",ch,NULL,NULL,TO_ROOM);

          if ( ch->outer_aura != A_RTHUNDER && ch->inner_aura != A_RTHUNDER && !IS_SET(ch->group_aura, G_RTHUNDER) )
             {
                ch->hit = UMAX( 100, ch->hit - 2500000 );
                ch->agg = 100;
             }

          return;
       }

    if (!IS_NPC(victim) && IS_IMMUNE(victim,IMM_SHIELDED))
    {
       send_to_char( "You cannot find them.\n\r",ch);
       return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2)) chaosvalue = TRUE;

    if (!IS_NPC(ch) && IS_SET(ch->more, MORE_SAFE)) safe = TRUE;

    if ( IS_SET( ch->in_room->room_flags, ROOM_ICETRAP ) && IS_CLASS( ch, CLASS_SNOW_ELF ) )
       icy = TRUE;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 25 
	&& !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if (!IS_ITEMAFF(ch, ITEMA_VISION) && IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(15,25);

    chroom = ch->in_room;
    victimroom = victim->in_room;
    if (chaosvalue)
      for ( ; ; ) 
      {
        rand_room = number_range( 0, 65535 );
        victimroom = get_room_index( rand_room );
        if (victimroom != NULL) break;   
      }

    char_from_room(ch);
    char_to_room(ch,victimroom);

    do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    
    if ( ( IS_CLASS(victim,CLASS_DRAGON) ) && ( IS_DRAGONPOWER(victim,DRAGON_INTUITION) ) )
       act("Your senses tickle as you feel $N scrying you.\n\r",victim,NULL,ch,TO_CHAR);

    if ( safe )
       SET_BIT(ch->more, MORE_SAFE);

    if ( icy == TRUE )
       SET_BIT( ch->in_room->room_flags, ROOM_ICETRAP );
}

void do_readaura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}

	act("#w$n #gexamines #P$p #gintently.#n",ch,obj,NULL,TO_ROOM);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if ( !IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
       ( IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("#wYou #gare #eunable #gto read their #yaura#g.#n\n\r",ch);
	return;
    }

    act("#w$n #gexamines #C$N #gintently.#n",ch,NULL,victim,TO_NOTVICT);
    act("#w$n #gexamines #Cyou #gintently.#n",ch,NULL,victim,TO_VICT);

    if ( IS_NPC( victim ) )
       sprintf(buf, "#C%s #gis an #PNPC#g.#n\n\r",victim->short_descr);
       else sprintf(buf, "#C%s #gis a #yMortal#g.#n\n\r", victim->name);

    send_to_char(buf,ch);

    if ( !IS_NPC( victim ) )
       {
           sprintf( buf, "#wStr:#y%d#w, Vit:#y%d#w, Min:#y%d#w, Spi:#y%d#w, Dex:#y%d#w, Eva:#y%d#w.#n\n\r",
                         get_curr_str(victim), get_curr_con(victim), get_curr_int(victim), get_curr_wis(victim),
                         get_curr_dex(victim), get_curr_eva(victim) );
           send_to_char( buf, ch );
       }

    sprintf(buf,"#wHp:#C%d#w/#y%d#w, Mana:#C%d#w/#y%d#w, Move:#C%d#w/#y%d#w.#n\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);

    sprintf(buf,"#yAlignment:#P%d#w.#n\n\r",victim->alignment);
    send_to_char(buf,ch);

    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("#C$N #gis #Ppregnant#g.#n",ch,NULL,victim,TO_CHAR);
    return;
}

void do_shield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
	if (!IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_WEREWOLF) )
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}

	if (!IS_DEMPOWER( ch, DEM_SHIELD) && IS_CLASS(ch,CLASS_DEMON))
	{
        send_to_char("You must inpart it first!\n\r",ch);
        return;
    }
	if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSHIELD))
	{  send_to_char("You don't have that power.\n\r", ch );
		return;}

    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You shield your aura from those around you.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
	return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
}

void do_regenerate( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->rage > 0)
    {
	send_to_char("You cannot control your regenerative powers while the beast is so strong.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char("You cannot regenerate while fighting.\n\r",ch);
	return;
    }

    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
	send_to_char("But you are already completely regenerated!\n\r",ch);
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 5 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(10,20);
    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
    	send_to_char("Your body has completely regenerated.\n\r",ch);
    	act("$n's body completely regenerates itself.",ch,NULL,NULL,TO_ROOM);
    }
    else
    	send_to_char("Your body slowly regenerates itself.\n\r",ch);
    if (ch->hit < 1 )
    {
	ch->hit = ch->hit + 1;
	update_pos(ch);
	WAIT_STATE( ch, 24 );
    }
    else
    {
    	ch->hit = UMIN(ch->hit + 500, ch->max_hit);
    	ch->mana = UMIN(ch->mana + 500, ch->max_mana);
    	ch->move = UMIN(ch->move + 500, ch->max_move);
	WAIT_STATE(ch,8);
        update_pos(ch);
    }
    return;
}


void reg_mend( CHAR_DATA *ch )
{
    int ribs = 0;
    int teeth = 0;

    if (IS_BODY(ch,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(ch,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(ch,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(ch,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(ch,BROKEN_RIBS_16)) ribs += 16;
    if (IS_HEAD(ch,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(ch,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(ch,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(ch,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(ch,LOST_TOOTH_16 )) teeth += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(ch,BROKEN_RIBS_1 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(ch,BROKEN_RIBS_2 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(ch,BROKEN_RIBS_4 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(ch,BROKEN_RIBS_8 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(ch,BROKEN_RIBS_16))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",ch,NULL,NULL,TO_ROOM);
	act("One of your ribs snap back into place.",ch,NULL,NULL,TO_CHAR);
    }
    else if (IS_HEAD(ch,LOST_EYE_L))
    {
	act("An eyeball appears in $n's left eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your left eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_L);
    }
    else if (IS_HEAD(ch,LOST_EYE_R))
    {
	act("An eyeball appears in $n's right eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your right eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_R);
    }
    else if (IS_HEAD(ch,LOST_EAR_L))
    {
	act("An ear grows on the left side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the left side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_L);
    }
    else if (IS_HEAD(ch,LOST_EAR_R))
    {
	act("An ear grows on the right side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the right side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_R);
    }
    else if (IS_HEAD(ch,LOST_NOSE))
    {
	act("A nose grows on the front of $n's face.",ch,NULL,NULL,TO_ROOM);
	act("A nose grows on the front of your face.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_NOSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(ch,LOST_TOOTH_1 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(ch,LOST_TOOTH_2 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(ch,LOST_TOOTH_4 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(ch,LOST_TOOTH_8 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(ch,LOST_TOOTH_16))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("A missing tooth grows in your mouth.",ch,NULL,NULL,TO_CHAR);
	act("A missing tooth grows in $n's mouth.",ch,NULL,NULL,TO_ROOM);
    }
    else if (IS_HEAD(ch,BROKEN_NOSE))
    {
	act("$n's nose snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your nose snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(ch,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your jaw snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(ch,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your skull knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(ch,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your spine knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(ch,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your neck snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's left shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your left shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
    }
    else if (IS_ARM_R(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's right shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your right shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
    }
    else if (IS_LEG_L(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's left hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your left hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's right hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your right hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,BROKEN_ARM))
    {
	act("$n's left arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(ch,BROKEN_ARM))
    {
	act("$n's right arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(ch,BROKEN_LEG))
    {
	act("$n's left leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(ch,BROKEN_LEG))
    {
	act("$n's right leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's left wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your left wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's right wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your right wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
    }
    else if (IS_LEG_L(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's left ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your left ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's right ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your right ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,BROKEN_THUMB))
    {
	act("$n's left thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,BROKEN_THUMB))
    {
	act("$n's right thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if ( IS_ARM_R(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if ( IS_ARM_R(ch,BROKEN_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(ch,CUT_THROAT))
    {
    	if (IS_SET(ch->loc_hp[6], BLEEDING_THROAT)) return;
	act("The wound in $n's throat closes up.",ch,NULL,NULL,TO_ROOM);
	act("The wound in your throat closes up.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],CUT_THROAT);
    }
    return;
}

void vamp_rage( CHAR_DATA *ch )
{
    if ( IS_NPC(ch) ) return;

    if ( ch->rage > ch->beast )
       return;

    send_to_char("You scream with rage as the beast within consumes you!\n\r",ch);
    act("$n screams with rage as $s inner beast consumes $m!.", ch, NULL, NULL, TO_ROOM);

    ch->rage += number_range(500,1000);

    if ( ch->rage > ch->beast )
       ch->rage = ch->beast;

    return;
}

void knightupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#wPaladin Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      stc("You are the Avatar.\n\r",ch);

   if ( ch->protecting != NULL )
      act("You are covering $N with your body right now.", ch, NULL, ch->protecting, TO_CHAR );

   if ( IS_SET(ch->more2, MORE2_GUARD) )
      stc("You are currently in a defensive mode.\n\r",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Subdue in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[11] > 0 )
      {
         sprintf(buf, "You can do another Enhanced Divine Tech in #w%d#n ticks.\n\r", ch->ctimer[11] );
         stc(buf,ch);
      }
}

void ninjaupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#bNinja Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      stc("You are hiding in the shadows.\n\r",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Hakunetsu in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Dig in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another Dinichi in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Incapitate in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[12] > 0 )
      {
         sprintf(buf, "You can do another Caltrops in #w%d#n ticks.\n\r", ch->ctimer[12] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Illusion in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Finale Chance in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another Hishoken Slam in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc(buf,ch);
      }
}

void gaidenupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#bGaiden Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      stc("You are hiding in the shadows.\n\r",ch);

   if ( ch->pcdata->ninja_powers[NINJAKI] > 0 )
      sprintf(buf, "Your ninja ki is currently at #w%d#n%%.\n\r", ch->pcdata->ninja_powers[NINJAKI] );

   if ( ch->ninja_timer > 0 )
      {
         sprintf(buf, "You can do another Somer Strike in #w%d#n pulses.\n\r", ch->ninja_timer );
         stc(buf,ch);
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Rising Tiger in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Star Dance in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }
}

void demonupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#RDemon Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->pcdata->powers[DPOWER_FLAGS] < 1 )
      send_to_char("You have no demonic powers.\n\r",ch);

   if ( IS_DEMPOWER(ch,DEM_FANGS) )
      {
         if ( IS_VAMPAFF(ch, VAM_FANGS) )
            send_to_char("You have a pair of long pointed fangs extending from your gums.\n\r",ch);
            else send_to_char("You have a pair of long pointed fangs, but they are not currently extended.\n\r",ch);
      }

   if ( IS_DEMPOWER(ch,DEM_CLAWS) )
      {
         if ( IS_VAMPAFF(ch, VAM_CLAWS) )
            send_to_char("You have a pair of razor sharp claws extending from your fingers.\n\r",ch);
            else send_to_char("You have a pair of razor sharp claws, but they are not currently extended.\n\r",ch);
      }

   if ( IS_DEMPOWER(ch,DEM_HORNS) )
      {
         if ( IS_DEMAFF(ch, DEM_HORNS) )
            send_to_char("You have a pair of curved horns extending from your forehead.\n\r",ch);
            else send_to_char("You have a pair of curved horns, but they are not currently extended.\n\r",ch);
      }

   if ( IS_DEMPOWER(ch,DEM_TAIL) )
      {
         if ( IS_DEMAFF(ch, DEM_TAIL) )
            send_to_char("You have a huge tail extended from your backside.\n\r",ch);
            else send_to_char("You have a tail, but it is  not currently extended.\n\r",ch);
      }

   if ( IS_DEMPOWER(ch,DEM_HOOVES) )
      {
         if ( IS_DEMAFF(ch, DEM_HOOVES) )
            send_to_char("You have hooves instead of feet.\n\r",ch);
            else send_to_char("You are able to transform your feet into hooves at will.\n\r",ch);
	}

   if ( IS_DEMPOWER(ch,DEM_EYES) )
      {
         if ( IS_VAMPAFF(ch, VAM_NIGHTSIGHT) )
            send_to_char("Your eyes are glowing bright red, allowing you to see in the dark.\n\r",ch);
            else send_to_char("You are able to see in the dark, although that power is not currently activated.\n\r",ch);
	}

   if ( IS_DEMPOWER(ch,DEM_WINGS) )
      {
         if ( !IS_DEMAFF(ch, DEM_WINGS) )
            send_to_char("You have the ability to extend wings from your back.\n\r",ch);
            else if ( IS_DEMAFF(ch, DEM_UNFOLDED) )
                    send_to_char("You have a pair of large leathery wings unfolded behind your back.\n\r",ch);
                   else send_to_char("You have a pair of large leathery wings folded behind your back.\n\r",ch);
	}

   if ( IS_DEMPOWER(ch,DEM_MIGHT) )
      send_to_char("Your muscles ripple with supernatural strength.\n\r",ch);

   if ( IS_DEMPOWER(ch,DEM_TOUGH) )
      send_to_char("Your skin reflects blows with supernatural toughness.\n\r",ch);

   if ( IS_DEMPOWER(ch,DEM_SPEED) )
      send_to_char("You move with supernatural speed and grace.\n\r",ch);

   if ( IS_SET(ch->more3, MORE3_DAMNATION) )
      stc("You are protected by a psisonic shield.\n\r",ch);

   if ( IS_SET(ch->extra, EXTRA_OSWITCH) )
      stc("You are in weapon form.\n\r",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Gust in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Unnerve in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Inferno in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Frost Breath in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Dark Force in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc(buf,ch);
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can go into weaponform again in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc(buf,ch);
      }

   if ( ch->ctimer[7] > 0 )
      {
         sprintf(buf, "You can do another shed in #w%d#n ticks.\n\r", ch->ctimer[7] );
         stc(buf,ch);
      }

   if ( ch->ctimer[11] > 0 )
      {
         sprintf(buf, "You can do another crucifix in #w%d#n ticks.\n\r", ch->ctimer[11] );
         stc(buf,ch);
      }

   if ( ch->ctimer[12] > 0 )
      {
         sprintf(buf, "You can do another endozone in #w%d#n ticks.\n\r", ch->ctimer[12] );
         stc(buf,ch);
      }
}

void dragonupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#pDragon Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      stc("You are in dragon form.\n\r",ch);

   if ( ch->dragon_timer > 0 )
      {
          sprintf(buf, "You will recover your senses in #e%d#n more pulses.\n\r", ch->dragon_timer );
          stc(buf,ch);
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Dragon Swoop in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Mega Flare in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }
}

void dragoonupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#cHighwind Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->dragoon_timer > 0 )
      {
          sprintf(buf, "You can do another Jump attack in #e%d#n more pulses.\n\r", ch->dragoon_timer );
          stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Ultra Jump in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Virtual Spears in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }
}

void mageupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#yWizard Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
      stc("You are focusing on your telekinetics.\n\r",ch);

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Whirlwind in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Meteor Shower in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc(buf,ch);
      }

   if ( ch->clannum == 1 || ch->clannum == 6 )
      {
         stc("\n\r",ch);
         cent_to_char("-----------------------------------------------------------------",ch);
         centre_text("#CBattleMage Upkeep#n",ch);
         cent_to_char("-----------------------------------------------------------------",ch);

         if ( IS_SET( ch->more3, MORE3_SOUL ) )
            stc("Your soul and body are two different entities.\n\r",ch);
      }

   if ( ch->clannum == 2 || ch->clannum == 6 )
      {
         stc("\n\r",ch);
         cent_to_char("-----------------------------------------------------------------",ch);
         centre_text("#CSummoner Upkeep#n",ch);
         cent_to_char("-----------------------------------------------------------------",ch);
      }

   if ( ch->clannum == 3 || ch->clannum == 6 )
      {
         stc("\n\r",ch);
         cent_to_char("-----------------------------------------------------------------",ch);
         centre_text("#CTimeMage Upkeep#n",ch);
         cent_to_char("-----------------------------------------------------------------",ch);
 
         if ( IS_SET( ch->more2, MORE2_RAGE ) )
            stc("Your spirit is already attuned to nature.\n\r",ch);

         if ( ch->ctimer[6] > 0 )
            {
               sprintf(buf, "You can do another Accelerate in #w%d#n ticks.\n\r", ch->ctimer[6] );
               stc(buf,ch);
            }

         if ( ch->ctimer[4] > 0 )
            {
               sprintf(buf, "You can do another TimeWarp in #w%d#n ticks.\n\r", ch->ctimer[4] );
               stc(buf,ch);
            }
      }

   if ( ch->clannum == 4 )
      {
         stc("\n\r",ch);
         cent_to_char("-----------------------------------------------------------------",ch);
         centre_text("#CBookMage Upkeep#n",ch);
         cent_to_char("-----------------------------------------------------------------",ch);

         if ( ch->ctimer[0] > 0 )
            {
               sprintf(buf, "You can do another Teleport Chant in #w%d#n ticks.\n\r", ch->ctimer[0] );
               stc(buf,ch);
            }

         if ( ch->ctimer[6] > 0 )
            {
               sprintf(buf, "You can do another Blaze Chant in #w%d#n ticks.\n\r", ch->ctimer[6] );
               stc(buf,ch);
            }

		 if ( ch->ctimer[7] > 0 )
			{
               sprintf(buf, "You can do another Dark Ball in #w%d#n ticks.\n\r", ch->ctimer[7] );
			   stc(buf,ch);
			}
      }
}

void saiyanupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#rSaiyan Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->saiyanab[BODY] == 500 )
   	stc("You are in Oozaruu form.\n\r",ch);
      else if ( ch->saiyanab[BODY] > 0 )
              stc("You are a Super Saiyan.\n\r",ch);

   sprintf(buf, "Your current powerlevel is #w%d#n.\n\r\n\r", ch->saiyan_powerlevel );
   stc(buf,ch);

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Taiyouken in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc(buf,ch);
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Power Ball in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Genki Dama in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Mouth Blast in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Ryuken in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another Chou Kamehameha in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc(buf,ch);
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another Galick Hou in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc(buf,ch);
      }
}

void necroupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eNecromancer Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Gas Cloud in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Seek in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another AntiCross in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another SoulStop in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another SoulChill in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Madness in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc(buf,ch);
      }
}

void undeadupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eUndead Knight Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->pcdata->powers[POWER_FREEZE] > 0 )
      {
          sprintf(buf, "You can use powerword stun in another %d pulses.\n\r", ch->pcdata->powers[POWER_FREEZE] );
          stc(buf,ch);
      }

   if ( ch->lightning_timer > 0 )
      {
          sprintf(buf, "You can use powerword lightning in another %d pulses.\n\r", ch->lightning_timer );
          stc(buf,ch);
      }

   if ( ch->pcdata->powers[POWER_WORD] > 0 )
      {
          sprintf(buf, "You can use powerword kill in another %d pulses.\n\r", ch->pcdata->powers[POWER_WORD] );
          stc(buf,ch);
      }

   if ( ch->pcdata->powers[POWER_BLIND] > 0 )
      {
          sprintf(buf, "You can use powerword blind in another %d pulses.\n\r", ch->pcdata->powers[POWER_BLIND] );
          stc(buf,ch);
      }

   if ( ch->pcdata->powers[POWER_FLAME] > 0 )
      {
          sprintf(buf, "You can use powerword flames in another %d pulses.\n\r", ch->pcdata->powers[POWER_FLAME] );
          stc(buf,ch);
      }

   if ( ch->pcdata->powers[POWER_CHILL] > 0 )
      {
          sprintf(buf, "You can use powerword chill in another %d pulses.\n\r", ch->pcdata->powers[POWER_CHILL] );
          stc(buf,ch);
      }

   if ( ch->pcdata->powers[POWER_BLAST] > 0 )
      {
          sprintf(buf, "You can use powerword blast in another %d pulses.\n\r", ch->pcdata->powers[POWER_BLAST] );
          stc(buf,ch);
      }

   if ( ch->fear_timer > 0 )
      {
          sprintf(buf, "You can use powerword fear in another %d pulses.\n\r", ch->fear_timer );
          stc(buf,ch);
      }

   if ( ch->poison_timer > 0 )
      {
          sprintf(buf, "You can use powerword venom in another %d pulses.\n\r", ch->poison_timer );
          stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Genocide in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }
}

void angelupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#wAngel Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_AFFECTED(ch, AFF_ETHEREAL) )
	stc("You are in your spiritual form.\n\r",ch);

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION) )
      send_to_char("Your wings are already spread out.\n\r",ch);

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      stc("You are in your true form.\n\r",ch);

   if ( IS_SET(ch->more, MORE_LIGHT) )
      stc("Holy Light is bursting from within you.\n\r",ch);

   if ( IS_SET(ch->more, MORE_MSHIELD) )
      stc("Your body is engulfed by a yellow ball of energy.\n\r",ch);

   if ( IS_SET( ch->more3, MORE3_HAND ) )
      stc("Your hand is covering a blast of holy flames.\n\r",ch);

   if ( ch->guarding > 0 )
      {
         sprintf(buf, "You can do another Gshield in #w%d#n ticks.\n\r", ch->guarding );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Holy Ball in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Heavenly Cloak in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Seraph Curse in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Divine Judgement in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Holy Word in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another Genesis in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc(buf,ch);
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another Angel Wings in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc(buf,ch);
      }
}

void werewolfupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#oWerewolf Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->garou1, GAROU_CLENCHED) )
      stc("You will clench onto your foes.\n\r",ch);

   if ( IS_SET(ch->newbits, NEW_QUILLS) )
      stc("Your fur is very bristly and sharp.\n\r",ch);

   if ( IS_GAR1(ch, WOLF_RAZORCLAWS ) )
      stc("Your claws are very sharp.\n\r",ch);

   if ( ch->ctimer[0] > 0 )
      {
          sprintf(buf, "You can do another Cannibal in #w%d#n ticks.\n\r", ch->ctimer[0] );
          stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
          sprintf(buf, "You can do another Banshee in #w%d#n ticks.\n\r", ch->ctimer[4] );
          stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
          sprintf(buf, "You can do another Dazzle in #w%d#n ticks.\n\r", ch->ctimer[5] );
          stc(buf,ch);
      }
}

void vampupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#RVampire Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->polyaff, POLY_SERPENT) )
      stc("You are in serpent form.\n\r",ch);

   if ( IS_SET(ch->polyaff, POLY_WOLF) )
      stc("You are in wolf form.\n\r",ch);

   if ( IS_SET(ch->polyaff, POLY_BAT) )
      stc("You are in bat form.\n\r",ch);

   if ( IS_SET(ch->polyaff, POLY_SOULREAVER) )
      stc("You are in soulreaver form.\n\r",ch);

   if ( IS_SET(ch->polyaff, POLY_MIST) )
      stc("You are in mist form.\n\r",ch);

   if ( IS_SET(ch->flag2, AFF_SPIRITGUARD) )
      stc("You have awaken your guardian spirit.\n\r",ch);

   if ( IS_EXTRA(ch, EXTRA_PLASMA) )
      stc("You are a pool of blood.\n\r",ch);

   if ( IS_EXTRA(ch, EXTRA_FLASH) )
      stc("You are moving very swiftly.\n\r",ch);

   if ( IS_SET(ch->more2, MORE2_SHIELD) )
      send_to_char("You are surrounded by a death shield.\n\r", ch );

   if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
      stc( "Your right arm is infused with the #RSoul#eReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_LIGHT )
              stc( "Your right arm is infused with the #wLight#eReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_DARK )
              stc( "Your right arm is infused with the #eDarkReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_STONE )
              stc( "Your right arm is infused with the #oStone#eReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_CHILL )
              stc( "Your right arm is infused with the #LChill#eReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_FIRE )
              stc( "Your right arm is infused with the #RFlame#eReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SHOCK )
              stc( "Your right arm is infused with the #yShock#eReaver#n.\n\r", ch );
      else if ( ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_FAITH )
              stc( "Your right arm is infused with the #PFaith#eReaver#n.\n\r", ch );

   if ( ch->vampirestuff > 0 )
      stc("You are still recovering from the last shroud.\n\r",ch);

   if ( ch->vlife[CURRENT] > 0 )
      {
         sprintf(buf, "You have #R%d#n life force remaining.\n\r", ch->vlife[CURRENT] );
         stc(buf,ch);
      }

   if ( ch->obeah_timer > 0 )
      {
         sprintf(buf, "You can't use your obeah powers for another #R%d#n pulses.\n\r", ch->obeah_timer );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Spirit Blast in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Fear in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another Stigmata in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc(buf,ch);
      }
}

void samuraiupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];
   int sam_max = UMIN( 28, 10 + ch->pkill );

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#cSamurai Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another SDraw in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Elemental Strike in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }
}

void jediupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#oJedi Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->newbits, NEW_BLADESPIN) )
      stc("You are grasping your blade firmly.\n\r",ch);

   if ( IS_AFFECTED(ch, AFF_FLYING) ) 
      stc("You are levitating yourself with the force.\n\r",ch);

   if ( IS_SET(ch->more, MORE_FORCE) )
      stc("You are summoning the force.\n\r",ch);

   if ( ch->clannum == 1 )
      {
         if ( ch->ctimer[0] > 0 )
            {
               sprintf(buf, "You can do another Meteor Strike in #w%d#n ticks.\n\r", ch->ctimer[0] );
               stc(buf,ch);
            }

         if ( ch->ctimer[1] > 0 )
            {
               sprintf(buf, "You can do another Force Faze in #w%d#n ticks.\n\r", ch->ctimer[1] );
               stc(buf,ch);
            }
      }

   if ( ch->clannum == 2 )
      {
         if ( ch->ctimer[0] > 0 )
            {
               sprintf(buf, "You can do another Jedi Destruction in #w%d#n ticks.\n\r", ch->ctimer[0] );
               stc(buf,ch);
            }
      }

   if ( ch->jedi_timer > 0 )
      {
         sprintf(buf, "You can't push a person for another #e%d#n more pulses.\n\r", ch->jedi_timer );
         stc(buf,ch);
      }
}

void drgnupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#PDragoon Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      stc( "You are currently in your #Pdragoon #eform#n.\n\r", ch );

   sprintf( buf, "Your current #Pdragoon #ylevel#n is at #w%d#n.\n\r", URANGE( 1, ch->pkill / 2, 10 ) );
   stc( buf, ch );

   sprintf( buf, "You currently have #w%d #y/ #w%d #Pdragoon#n spirit.\n\r", ch->pcdata->powers[DRAGOON_SPIRIT], URANGE( 100, ch->pkill * 50, 1000 ) );
   stc( buf, ch );
}

void duergarupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#gDuergar Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET( ch->more, MORE_CLOUD ) )
      stc( "You currently have a #edark #gcloud#n around you...\n\r", ch );

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another axe knockout in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc( buf, ch );
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another tunnel in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc( buf, ch );
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another vine of evil in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc( buf, ch );
      }
}

void phoenixupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#RPhoenix Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another phoenix rush in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc( buf, ch );
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another phoenix wish in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc( buf, ch );
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another phoenix cry in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc( buf, ch );
      }
}

void snowupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#CSnow Elf Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->divine_timer > 0 )
      {
         sprintf(buf, "You can block another attack in #w%d#n pulses.\n\r", ch->divine_timer );
         stc( buf, ch );
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another winter swams in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc( buf, ch );
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another carpe jugulum in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc( buf, ch );
      }
}

void hobbitupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   bool ring = TRUE;

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#wHob#ebit #CUpkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ( ( obj = get_eq_char( ch, WEAR_FINGER_L ) ) == NULL ) || obj->pIndexData->vnum != 437 )
      if ( ( ( obj = get_eq_char( ch, WEAR_FINGER_R ) ) == NULL ) || obj->pIndexData->vnum != 437 )
         ring = FALSE;

   if ( ring )
      stc( "You are currently wearing the #ering#n and are consumed with #wpure #ehatred#n.\n\r", ch );
      else stc( "You are currently not wearing the #ering#n, but its #ecorruption #whaunts#n you.\n\r", ch );    

   if ( IS_SET( ch->more3, MORE3_HOUSE ) )
      stc( "You are currently being watched by a guardian angel.\n\r", ch );

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another hobbit magic in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc( buf, ch );
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another shadow dancer in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc( buf, ch );
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another daintly dark in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc( buf, ch );
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another crystal dreams in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc( buf, ch );
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another frogget in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc( buf, ch );
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another wraithe knights in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc( buf, ch );
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another famed archangel in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc( buf, ch );
      }
}

void wispupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#yWisp Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another heat shield in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc( buf, ch );
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another call lightning in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc( buf, ch );
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another thunderstorm in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc( buf, ch );
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another icarus in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc( buf, ch );
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another mind tap in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc( buf, ch );
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can turn into a plasma blade again in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc(buf,ch);
      }
}

void drowupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eDrow Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   sprintf( buf, "You are a Level #w%d#n Elf.\n\r", ( ch->pkill < 4 ) ? 1 : UMIN( ch->pkill / 2, 10 ) );
   stc( buf, ch );

   if ( IS_SET(ch->newbits, NEW_DARKTENDRILS) )
      stc("Your dark tendrils are assisting you in battle.\n\r",ch);

   if ( IS_SET(ch->newbits, NEW_DARKDODGE) )
      stc("Your Dark Nature is enhancing your movements.\n\r",ch);

   if ( IS_SET(ch->immune, IMM_SHIELDED) )
      stc("You are shielding yourself from those around you.\n\r",ch);

   if ( IS_SET(ch->newbits, NEW_FIGHTDANCE) )
      stc("You are mimicking the dance of Lloth.\n\r",ch);

   if ( IS_SET(ch->newbits, NEW_DARKNESS) ) 
      stc("You have a cloak of darkness around yourself.\n\r",ch);

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Veil of Death in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->pcdata->powers[DROW_CLASS] == DROW_CLERICAL || ch->pcdata->powers[DROW_CLASS] == DROW_AVATAR )
      {
         stc("#eYou are a #Pfully pledged member #eof the #wClerical #eHouse.#n\n\r",ch);

         if ( ch->ctimer[1] > 0 )
            {
               sprintf(buf, "You can do another Hold Person in #w%d#n ticks.\n\r", ch->ctimer[1] );
               stc(buf,ch);
            }

         if ( ch->ctimer[0] > 0 )
            {
               sprintf(buf, "You can do another Cloud Kill in #w%d#n ticks.\n\r", ch->ctimer[0] );
               stc(buf,ch);
            }

         if ( ch->ctimer[12] > 0 )
            {
               sprintf(buf, "You can do another Raise Dead in #w%d#n ticks.\n\r", ch->ctimer[12] );
               stc(buf,ch);
            }
      }

   if ( ch->pcdata->powers[DROW_CLASS] == DROW_MAGI || ch->pcdata->powers[DROW_CLASS] == DROW_AVATAR )
      {
         stc("#eYou are a #Pfully pledged member #eof the #wMagi #eHouse.#n\n\r",ch);

         if ( IS_SET(ch->act, PLR_WIZINVIS) )
            stc("#eYou are hiding in the #Pshadows#e.#n\n\r",ch);

         if ( ch->ctimer[10] > 0 )
            {
               sprintf(buf, "You can do another Horror in #w%d#n ticks.\n\r", ch->ctimer[10] );
               stc(buf,ch);
            }

         if ( ch->ctimer[12] > 0 )
            {
               sprintf(buf, "You can do another Shadow Door in #w%d#n ticks.\n\r", ch->ctimer[12] );
               stc(buf,ch);
            }

         if ( ch->ctimer[3] > 0 )
            {
               sprintf(buf, "You can do another Ghoul's Touch in #w%d#n ticks.\n\r", ch->ctimer[3] );
               stc(buf,ch);
            }
      }

   if ( ch->pcdata->powers[DROW_CLASS] == DROW_MILITIA || ch->pcdata->powers[DROW_CLASS] == DROW_AVATAR )
      {
         stc("#eYou are a #Pfully pledged member #eof the #wMilitia #eHouse.#n\n\r",ch);

         if ( ch->cmbt[3] > 0 )
            {
               sprintf(buf, "You can shoot #e%d#n more arrows.\n\r", ch->cmbt[3] );
               stc(buf,ch);
            }

         if ( ch->ctimer[5] > 0 )
            {
               sprintf(buf, "You can do another Elghinn Wu'suul in #w%d#n ticks.\n\r", ch->ctimer[5] );
               stc(buf,ch);
            }

         if ( ch->ctimer[4] > 0 )
            {
               sprintf(buf, "You can do another Choke Hold in #w%d#n ticks.\n\r", ch->ctimer[4] );
               stc(buf,ch);
            }
      }
}

void shadowupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eAssassin Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   sprintf(buf, "Your #wchi#n energy currently at #w%d#n%%.\n\r", ch->focus[CURRENT] );
   stc(buf,ch);
   sprintf(buf, "Your #ekiotic#n energy currently at #e%d#n%%.\n\r", ch->chi[CURRENT] / 2 );
   stc(buf,ch);

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      stc( "You are currently disguised.\n\r", ch );

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Finale Chance in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Ryujin in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[4] > 0 )
      {
         sprintf(buf, "You can do another Tenbu in #w%d#n ticks.\n\r", ch->ctimer[4] );
         stc(buf,ch);
      }

   if ( ch->ctimer[5] > 0 )
      {
         sprintf(buf, "You can do another Koho in #w%d#n ticks.\n\r", ch->ctimer[5] );
         stc(buf,ch);
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another Ziden in #w%d#n ticks.\n\r", ch->ctimer[6] );
         stc(buf,ch);
      }

   if ( ch->ctimer[10] > 0 )
      {
         sprintf(buf, "You can do another Fukio in #w%d#n ticks.\n\r", ch->ctimer[10] );
         stc(buf,ch);
      }

   if ( ch->ctimer[11] > 0 )
      {
         sprintf(buf, "You can do another Ki Pinch in #w%d#n ticks.\n\r", ch->ctimer[11] );
         stc(buf,ch);
      }
}

void warriorupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eShadow Warrior Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->ninja_timer > 0 )
      {
         sprintf(buf, "You can do another Dark Hadou in #w%d#n pulses.\n\r", ch->ninja_timer );
         stc(buf,ch);
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Chi Blast in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Palm Tackle in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Raging Demon in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }
}

void assassinupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eClone Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->more, MORE_BLITZ) )
      stc("You are releasing the spirit of the blade.\n\r",ch);

   if ( IS_SET(ch->more3, MORE3_TURBO) )
      stc("You are focusing your energy on your attacks.\n\r",ch);

   if ( IS_SET(ch->itemaffect, ITEMA_FIREWALL) )
      stc("You are surrounded by a fire wall.\n\r",ch);

   if ( IS_SET( ch->special, SPC_SOR ) )
      {
         if ( ch->ctimer[0] > 0 )
            {
               sprintf(buf, "You can do another Earth Grasp in #w%d#n ticks.\n\r", ch->ctimer[0] );
               stc(buf,ch);
            }
      }

   if ( IS_SET( ch->special, SPC_KNIGHT ) )
      {
         if ( ch->ctimer[0] > 0 )
            {
               sprintf(buf, "You have the ability to merge for #w%d#n pulses.\n\r", ch->ctimer[0] );
               stc(buf,ch);
            }

         if ( ch->ctimer[1] > 0 )
            {
               sprintf(buf, "You can do another Shadow Stitch in #w%d#n ticks.\n\r", ch->ctimer[1] );
               stc(buf,ch);
            }

         if ( ch->ctimer[2] > 0 )
            {
               sprintf(buf, "You can do another Stunner Combo in #w%d#n ticks.\n\r", ch->ctimer[2] );
               stc(buf,ch);
            }

         if ( ch->ctimer[3] > 0 )
            {
               sprintf(buf, "You can do another Meteorian Combo in #w%d#n ticks.\n\r", ch->ctimer[3] );
               stc(buf,ch);
            }

         if ( ch->ctimer[4] > 0 )
            {
               sprintf(buf, "You can do another Absolute Zero in #w%d#n ticks.\n\r", ch->ctimer[4] );
               stc(buf,ch);
            }
      }
}

void statusupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#wEquipment #RPowers #CUpk#yeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET( ch->more2, MORE2_WARCRY ) )
	stc("You are releasing the true power of a warrior.\n\r",ch);
}

void kingdomupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#CKingdom #yPowers #wUpk#eeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->pcdata->kingdom == 1 )
      {
         if ( ch->ctimer[18] > 0 )
            {
               sprintf( buf, "You can do another Mirror Stance in #w%d#n ticks.\n\r", ch->ctimer[18] );
               stc( buf, ch );
            }

         if ( ch->ctimer[17] > 0 )
            {
               sprintf( buf, "You can do another True Sacrifice in #w%d#n ticks.\n\r", ch->ctimer[17] );
               stc( buf, ch );
            }

         if ( ch->ctimer[16] > 0 )
            {
               sprintf( buf, "You can do another Time Stop in #w%d#n ticks.\n\r", ch->ctimer[16] );
               stc( buf, ch );
            }
      }

   if ( ch->pcdata->kingdom == 2 )
      {
         if ( ch->ctimer[18] > 0 )
            {
               sprintf( buf, "You can do another Destroyer in #w%d#n ticks.\n\r", ch->ctimer[18] );
               stc( buf, ch );
            }

         if ( ch->ctimer[17] > 0 )
            {
               sprintf( buf, "You can do another Blaster in #w%d#n ticks.\n\r", ch->ctimer[17] );
               stc( buf, ch );
            }
      }

   if ( ch->pcdata->kingdom == 3 )
      {
         if ( IS_SET(ch->newbits, NEW_ALIGHT) ) 
            stc( "You have the entire area shrouded by an antilight.\n\r", ch );

         if ( ch->ctimer[18] > 0 )
            {
               sprintf( buf, "You can do another Backstab in #w%d#n ticks.\n\r", ch->ctimer[18] );
               stc( buf, ch );
            }

         if ( ch->ctimer[17] > 0 )
            {
               sprintf( buf, "You can do another Blaster in #w%d#n ticks.\n\r", ch->ctimer[17] );
               stc( buf, ch );
            }
      }
}

void monkupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#yMonk Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->newbits,NEW_MONKFLAME) )
	stc("Your hands are engulfed in flames.\n\r",ch);
 
   if ( IS_SET(ch->newbits,NEW_MONKADAM) )
  	stc("Your hands are as hard as adamantium.\n\r",ch);

   if ( IS_SET(ch->newbits,NEW_MONKSKIN) )
   	stc("Your skin is as hard as steel.\n\r\n\r",ch);

   if ( IS_SET(ch->newbits,NEW_POWER) )
      stc("The Light is directing your attacks!\n\r",ch);

   if ( IS_SET(ch->more,MORE_FOCUS) )
      stc("You are concentrating on your combos!\n\r",ch);

   if ( IS_SET(ch->more, MORE_BLADEGRASP) )
      stc("You are concentrating on your foe's weapon!\n\r",ch);

   sprintf(buf,"Your current focus points :   %d%%\n\r",ch->focus[CURRENT]);
   stc(buf,ch);
   sprintf(buf,"Your maximum focus points :   100%%\n\r");
   stc(buf,ch);

   if ( ch->ctimer[4] > 99 )
      {
         sprintf( buf, "You are in #wT#RR#yA#CN#gC#PE#n mode for %d more rounds.\n\r", ch->ctimer[4] - 99 );
         stc( buf, ch );
      }

   if ( ch->demmak > 0 )
      stc("You are performing the Dem Mak.\n\r",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Kakeio in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another Divine Ray in #w%d#n ticks.\n\r", ch->ctimer[1] );
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Twirl in #w%d#n ticks.\n\r", ch->ctimer[2] );
         stc(buf,ch);
      }

   if ( ch->ctimer[3] > 0 )
      {
         sprintf(buf, "You can do another Psycho Rush in #w%d#n ticks.\n\r", ch->ctimer[3] );
         stc(buf,ch);
      }

   if ( ch->ctimer[7] > 0 )
      {
         sprintf(buf, "You can do another Secret Fist in #w%d#n ticks.\n\r", ch->ctimer[7] );
         stc(buf,ch);
      }

   if ( ch->ctimer[10] > 0 )
      {
         sprintf(buf, "You can counter any technique with innocence for #w%d#n rounds.\n\r", ch->ctimer[10] );
         stc(buf,ch);
      }

   if ( ch->ctimer[10] > 0 )
      {
         sprintf(buf, "You can do another Divine Might in #w%d#n ticks.\n\r", ch->ctimer[10] );
         stc(buf,ch);
      }
}

void daramonupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#PDaramon Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->more3,MORE3_ONE) )
      stc("You are one with the universe.\n\r",ch);

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another stunner in #w%d#n ticks.\n\r", ch->ctimer[6]);
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another heavenly gate in #w%d#n ticks.\n\r", ch->ctimer[1]);
         stc(buf,ch);
      }
}

void masterupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eBlade #PLord Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->more3,MORE3_ONE) )
      stc("You are one with the universe.\n\r",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another 'Power' tech in #w%d#n ticks.\n\r", ch->ctimer[0]);
         stc(buf,ch);
      }

   if ( ch->ctimer[1] > 0 )
      {
         sprintf(buf, "You can do another air blade in #w%d#n ticks.\n\r", ch->ctimer[1]);
         stc(buf,ch);
      }

   if ( ch->ctimer[6] > 0 )
      {
         sprintf(buf, "You can do another vampire fang in #w%d#n ticks.\n\r", ch->ctimer[6]);
         stc(buf,ch);
      }

   if ( ch->ctimer[7] > 0 )
      {
         sprintf(buf, "You can do another lunar shield in #w%d#n ticks.\n\r", ch->ctimer[7]);
         stc(buf,ch);
      }
}

void daywalkerupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#RDaywalker Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Psionic Blast in #w%d#n ticks.\n\r", ch->ctimer[0]);
         stc(buf,ch);
      }

   if ( ch->ctimer[2] > 0 )
      {
         sprintf(buf, "You can do another Statis in #w%d#n ticks.\n\r", ch->ctimer[2]);
         stc(buf,ch);
      }
}

void garouupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#oGarou Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   sprintf( buf, "You are a Level #w%d#n Garou.\n\r", ch->pcdata->powers[DRAGOON_LEVEL] );
   stc( buf, ch );

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      stc( "You are in your true form.\n\r", ch );

   if ( ch->ctimer[10] > 0 )
      {
         sprintf(buf, "You can do another Desperation Attack in #w%d#n ticks.\n\r", ch->ctimer[10] );
         stc(buf,ch);
      }

   if ( ch->ctimer[11] > 0 )
      {
         sprintf(buf, "You can do another Wolverine Desperation Attack in #w%d#n ticks.\n\r", ch->ctimer[11] );
         stc(buf,ch);
      }
}

void kurudaupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eNests Commander Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->wfocus != 0 )
      {
         if ( ch->wfocus == ATTACK_FIRE )
            stc( "You are focusing your attacks to be of a #Rfiery#n type.\n\r", ch );
            else if ( ch->wfocus == ATTACK_ICE )
                    stc( "You are focusing your attacks to be of a #Cicy#n type.\n\r", ch );
            else if ( ch->wfocus == ATTACK_THUNDER )
                    stc( "You are focusing your attacks to be of a #ythunder like#n type.\n\r", ch );
            else if ( ch->wfocus == ATTACK_HOLY )
                    stc( "You are focusing your attacks to be of a #wholy#n type.\n\r", ch );
            else if ( ch->wfocus == ATTACK_DARK )
                    stc( "You are focusing your attacks to be of a #edemonic#n type.\n\r", ch );
            else if ( ch->wfocus == ATTACK_EARTH )
                    stc( "You are focusing your attacks to be of an #gearth like#n type.\n\r", ch );
      }

   if ( ch->ctimer[0] > 0 )
      {
         sprintf(buf, "You can do another Oniyaki in #w%d#n ticks.\n\r", ch->ctimer[0] );
         stc(buf,ch);
      }
}

void fiendupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#RFiend Upkeep#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      stc( "#RYou #eare in your #wtrue #eform.#n\n\r", ch );

   sprintf( buf, "Your Demon Trigger is currently #w%d#o/#e%d#n.\n\r", ch->pcdata->powers[DEMON_TRIGGER], URANGE( 100, ch->pkill * 50, 1000 ) );
   stc( buf, ch );

   if ( IS_SET( ch->more, MORE_WALL ) )
      stc( "You are currently focusing on your lifespan.\n\r", ch );
}

void equipupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#CGeneral #PPowers#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   if ( ch->mage_timer > 0 )
      {
         sprintf(buf, "You can't summon your deity for #w%d#n pulses.\n\r", ch->mage_timer );
         stc(buf,ch);
      }
      else if ( ch->cmbt[0] > 0 )
              {
                 sprintf(buf, "You currently have #w%d#n summon counters.\n\r", ch->cmbt[0] );
                 stc(buf,ch);
              }

   if ( ch->use_timer > 0 )
      {
         sprintf(buf, "You can't use another item for #w%d#n pulses.\n\r", ch->use_timer );
         stc(buf,ch);
      }

   if ( ch->ctimer[14] > 0 )
      {
         sprintf(buf, "You can do another pelt in #w%d#n ticks.\n\r", ch->ctimer[14] );
         stc(buf,ch);
      }

   if ( ch->ctimer[13] > 0 )
      {
         sprintf(buf, "You can do another Wind Technique in #w%d#n ticks.\n\r", ch->ctimer[13] );
         stc(buf,ch);
      }

   if ( ch->saiyan_timer > 0 )
      {
         sprintf( buf, "You can do another Armour Tech in #w%d#n pulses.\n\r", ch->saiyan_timer );
         stc( buf, ch );
      }

   if ( ch->throw_timer > 0 )
      {
         sprintf( buf, "You can do another Weapon Tech in #w%d#n pulses.\n\r", ch->throw_timer );
         stc( buf, ch );
      }

   if ( ch->break_timer > 0 )
      {
         sprintf( buf, "You can do another Breaker in #w%d#n pulses.\n\r", ch->break_timer );
         stc( buf, ch );
      }

   if ( ch->stunning_timer > 0 )
      {
         sprintf( buf, "You can do another Stunning Blow in #w%d#n pulses.\n\r", ch->stunning_timer );
         stc( buf, ch );
      }

   if ( ch->aragami_timer > 0 )
      {
         sprintf( buf, "You can do another Counter Tech in #w%d#n pulses.\n\r", ch->aragami_timer );
         stc( buf, ch );
      }

   if ( ch->achi > 0 )
      {
         sprintf( buf, "You can do another Greater Summon in #w%d#n pulses.\n\r", ch->achi );
         stc( buf, ch );
      }

   if ( ch->monk_timer > 0 )
      {
         sprintf( buf, "You can do another Healing Tech in #w%d#n pulses.\n\r", ch->monk_timer );
         stc( buf, ch );
      }
}

void elfupkeep(CHAR_DATA *ch)
{
   char buf[MAX_INPUT_LENGTH];

   stc("\n\r",ch);
   cent_to_char("-----------------------------------------------------------------",ch);
   centre_text("#eDark #cElf#n",ch);
   cent_to_char("-----------------------------------------------------------------",ch);

   sprintf( buf, "You are a Level #w%d#n Elf.\n\r", ch->pcdata->powers[DRAGOON_LEVEL] );
   stc( buf, ch );

   if ( ch->pcdata->powers[DROW_CLASS] == DROW_CLERICAL )
      {
         stc("#eYou are a #Pfully pledged member #eof the #wClerical #eHouse.#n\n\r",ch);

         if ( ch->ctimer[10] > 0 )
            {
               sprintf(buf, "You can do another Cura in #w%d#n ticks.\n\r", ch->ctimer[10] );
               stc(buf,ch);
            }

         if ( ch->ctimer[11] > 0 )
            {
               sprintf(buf, "You can do another Flame Strike in #w%d#n ticks.\n\r", ch->ctimer[11] );
               stc(buf,ch);
            }

         if ( ch->ctimer[12] > 0 )
            {
               sprintf(buf, "You can do another Raise Dead in #w%d#n ticks.\n\r", ch->ctimer[12] );
               stc(buf,ch);
            }
      }

   if ( ch->pcdata->powers[DROW_CLASS] == DROW_MAGI )
      {
         stc("#eYou are a #Pfully pledged member #eof the #wMagi #eHouse.#n\n\r",ch);

         if ( IS_SET(ch->act, PLR_WIZINVIS) )
            stc("#eYou are hiding in the #Pshadows#e.#n\n\r",ch);

         if ( ch->ctimer[10] > 0 )
            {
               sprintf(buf, "You can do another Horror in #w%d#n ticks.\n\r", ch->ctimer[10] );
               stc(buf,ch);
            }

         if ( ch->ctimer[11] > 0 )
            {
               sprintf(buf, "You can do another Confusion in #w%d#n ticks.\n\r", ch->ctimer[11] );
               stc(buf,ch);
            }

         if ( ch->ctimer[12] > 0 )
            {
               sprintf(buf, "You can do another Shadow Door in #w%d#n ticks.\n\r", ch->ctimer[12] );
               stc(buf,ch);
            }

         if ( ch->ctimer[13] > 0 )
            {
               sprintf(buf, "You can do another Ghoul's Touch in #w%d#n ticks.\n\r", ch->ctimer[13] );
               stc(buf,ch);
            }
      }

   if ( ch->pcdata->powers[DROW_CLASS] == DROW_MILITIA )
      {
         stc("#eYou are a #Pfully pledged member #eof the #wMilitia #eHouse.#n\n\r",ch);

         if ( ch->ctimer[10] > 0 )
            {
               sprintf(buf, "You can do another Demi Drow in #w%d#n ticks.\n\r", ch->ctimer[10] );
               stc(buf,ch);
            }

         if ( ch->ctimer[11] > 0 )
            {
               sprintf(buf, "You can do another Series Slash in #w%d#n ticks.\n\r", ch->ctimer[11] );
               stc(buf,ch);
            }

         if ( ch->ctimer[12] > 0 )
            {
               sprintf(buf, "You can do another Fallen Angel in #w%d#n ticks.\n\r", ch->ctimer[12] );
               stc(buf,ch);
            }

         if ( ch->ctimer[13] > 0 )
            {
               sprintf(buf, "You can do another Elghinn Wu'suul in #w%d#n ticks.\n\r", ch->ctimer[13] );
               stc(buf,ch);
            }
      }
}

void add_transmigration( CHAR_DATA *ch, int value )
{
   int old_tp, new_tp;

   if ( IS_NPC( ch ) || get_translevel( ch ) >= 25000000 )
      return;

   old_tp = get_translevel( ch ) / 100000;

   ch->pcdata->transmigration += value;

   new_tp = get_translevel( ch ) / 100000;

   if ( new_tp > old_tp )
      {
         new_tp -= old_tp;
         ch->practice += new_tp;
      }

   ch->max_hit += value;
   ch->max_mana += ( value / 2 );
   ch->max_move += ( value / 2 );
}

int get_translevel( CHAR_DATA *ch )
{
   if ( IS_NPC( ch ) )
      return 0;

   return UMIN( ch->pcdata->transmigration + ( ( ch->pcdata->clanrank - 1 ) * 100000 ), 25000000 );
}

void do_upkeep( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    cent_to_char("-----------------------------------------------------------------",ch);
    centre_text("#PGeneral Upkeep#n",ch);
    cent_to_char("-----------------------------------------------------------------",ch);

    if ( ch->inner_aura > -1 )
       stc("You body is encovered by a #wbri#ylli#want#n aura of #rd#ea#cz#bz#yl#pi#rn#wg#n energy.\n\r",ch);

    if ( ch->outer_aura > -1 )
       stc("You are surrounded by a #cpalish#n aura of #ehazing#n energy.\n\r",ch);

    sprintf( buf, "%s #wBlock Meter: #y%d%%#n\n\r", show_groove( ch ), ch->block_meter );
    stc( buf, ch );

    sprintf(buf, "\n\rYou are Level #w%d#n, #y%d#n generation and #p%d#n rank.\n\r", ch->tier, ch->generation,
                 ch->pcdata->clanrank );
    stc(buf,ch);

    sprintf( buf, "#PTransmigration Level#c: #y%d#w, #oLegend Points#c: #y%d#w, ", get_translevel( ch ), ch->pcdata->faithpoints );
    stc( buf, ch );

    if ( ch->anger_level > 300 )
       sprintf( buf, "#wTension Level#c: #sV#si#sO#sl#sE#sn#sT#n\n\r" );
       else sprintf( buf, "#wTension Level#c: #w%d#y%%#n\n\r", ch->anger_level / 3 );
    stc(buf,ch);

    if ( scavenger_time > 0 )
       {
          sprintf(buf, "You have found #w%d#n items in the #PScAvEnGeR #oHuNt#n so far.\n\r", ch->scavenger_item );
          stc( buf, ch );
       }

    if ( ch->cmbt[4] > 0 )
       {
          sprintf(buf, "You have #w%d#n free skillbuy points remaining.\n\r", ch->cmbt[4] );
          stc( buf, ch );
       }

    if ( IS_SET( ch->more3, MORE3_TIGER ) )
       stc("You are swaying in the #wTiger#n Fighting Style.\n\r",ch);

    if ( IS_SET( ch->more3, MORE3_SNAKE ) )
       stc( "You are withering in the #gsnake#n stance.\n\r", ch );

    if ( IS_SET( ch->more3, MORE3_HYENA ) )
       stc( "You are grinning to yourself in the #chyena#n stance.\n\r", ch );

    if ( IS_SET( ch->more3, MORE3_DRAGON ) )
       stc("You are scowling in the #RDragon#n Fighting Style.\n\r",ch);

    if ( IS_SET( ch->more3, MORE3_CRANE ) )
       stc("You are leaning in the #CCrane#n Fighting Style.\n\r",ch);

    if ( IS_SET( ch->more3, MORE3_FALCON ) )
       stc("You are tilting in the #LFalcon#n Fighting Style.\n\r",ch);

    if ( IS_SET( ch->more3, MORE3_MANTIS ) )
       stc("You are crouching in the #yMantis#n Fighting Style.\n\r",ch);

    if ( IS_SET( ch->more3, MORE3_WOLF ) )
       stc("You are swaying in the #oWolf#n Fighting Style.\n\r",ch);

    if ( IS_SET( ch->more, MORE_SAFE ) && ch->fight_timer == 0 )
       stc( "#PYou are currently protected from attacks.#n\n\r", ch );

    if ( ch->binding != NULL && IS_CLASS( ch, CLASS_ANGEL ) )
       act("#wYour soul is binded with #L$N#w.#n",ch,NULL,ch->binding,TO_CHAR);

    if ( ch->binded != NULL )
       act("#L$N #wsoul is binded with you.#n",ch,NULL,ch->binded,TO_CHAR);

    if ( IS_SET( ch->more, MORE_BARRIER ) )
       stc("#CYou are protected by a damage absorbing barrier.#n\n\r", ch );

    if ( ch->retaliate != NULL && ch->retaliate->pkill < ch->pkill )
       act( "#yYou can #Rretaliate #yon #w$N#y.#n", ch, NULL, ch->retaliate, TO_CHAR );

    if ( IS_SET( ch->extra, EXTRA_DETERMINE ) )
       stc("#PYou #eare currently #wanalysing #eattacks...#n\n\r", ch );

    if ( ch->pcdata->blood_meter > 0 && ch->pcdata->blood_power > 0 )
       {
          switch( ch->pcdata->blood_power )
          {
             case 1: sprintf( buf, "You have #eDestructive #RRage#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                     break;
             case 2: sprintf( buf, "You have #wSuper #CSonic #cspeed#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                     break;
             case 3: sprintf( buf, "You can #ystrike#n like a #CBlUr#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                     break;
             case 4: sprintf( buf, "You have #yProtection#n like a #CTank#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                     break;
             case 5: sprintf( buf, "You have #yEnhanced #PTech #RDamage#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                     break;
             case 6: sprintf( buf, "You can #RReGen#n like a #gSnake#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                     break;
             default: sprintf( buf, "You are #wquite #eb#pu#eg#pg#ey#n for #w%d#n pulses.\n\r", ch->pcdata->blood_meter );
                      break;
          }

          stc( buf, ch );
       }

    if ( ch->gshield > 0 )
       stc("#CYou are protected from being attacked by multiple persons.#n\n\r",ch);

    if ( IS_SET( ch->more, MORE_TOUCH ) )
       stc("#RYour aggression is fully released.#n\n\r",ch);

    if ( ch->control_timer > 0 )
       {
          sprintf(buf, "You can do another control in #w%d#n pulses.\n\r", ch->control_timer );
          stc(buf,ch);
       }  

    if ( IS_CLASS(ch, CLASS_PALADIN) )
       knightupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DRAGON) )
       dragonupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DROW) )
       drowupkeep(ch);

    if ( IS_CLASS(ch, CLASS_WEREWOLF) )
       werewolfupkeep(ch);

    if ( IS_CLASS(ch, CLASS_ANGEL) )
       angelupkeep(ch);

    if ( IS_CLASS(ch, CLASS_NINJA) )
       ninjaupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DEMON) )
       demonupkeep(ch);

    if ( IS_CLASS(ch, CLASS_VAMPIRE) )
       vampupkeep(ch);

    if ( IS_CLASS(ch, CLASS_MONK) )
       monkupkeep(ch);

    if ( IS_CLASS(ch, CLASS_MASTER) )
       masterupkeep(ch);

    if ( IS_CLASS(ch, CLASS_WIZARD) )
       mageupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DRAGOON) )
       dragoonupkeep(ch);

    if ( IS_CLASS(ch, CLASS_ASSASSIN) )
       assassinupkeep(ch);

    if ( IS_CLASS(ch, CLASS_SHADOW) )
       shadowupkeep(ch);

    if ( IS_CLASS(ch, CLASS_SHADOW_WAR) )
       warriorupkeep(ch);

    if ( IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
       undeadupkeep(ch);

    if ( IS_CLASS(ch, CLASS_NECROMANCER) )
       necroupkeep(ch);

    if ( IS_CLASS(ch, CLASS_SAMURAI) )
       samuraiupkeep(ch);

    if ( IS_CLASS(ch, CLASS_SAIYAN) )
       saiyanupkeep(ch);

    if ( IS_CLASS(ch, CLASS_SNOW_ELF) )
       snowupkeep(ch);

    if ( IS_CLASS(ch, CLASS_WISP) )
       wispupkeep(ch);

    if ( IS_CLASS(ch, CLASS_HOBBIT) )
       hobbitupkeep(ch);

    if ( IS_CLASS(ch, CLASS_PHOENIX) )
       phoenixupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DUERGAR) )
       duergarupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DARAMON ) )
       daramonupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DAYWALKER ) )
       daywalkerupkeep(ch);

    if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) )
       drgnupkeep(ch);

    if ( IS_CLASS(ch, CLASS_KURUDA) )
       kurudaupkeep(ch);

    if ( IS_CLASS(ch, CLASS_FIEND) )
       fiendupkeep(ch);

    equipupkeep(ch);
}

void do_werewolf( CHAR_DATA *ch, char *argument )
{
    char       buf[MAX_INPUT_LENGTH];
    char       arg[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int countup, max;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) ) return;
    if (IS_SET(ch->special, SPC_WOLFMAN)) return;
    SET_BIT(ch->special, SPC_WOLFMAN);

    send_to_char("You throw back your head and howl with rage!\n\r",ch);
    act("$n throws back $s head and howls with rage!.", ch, NULL, NULL, TO_ROOM);

    if (!IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("Your eyes start glowing red.\n\r",ch);
	act("$n's eyes start glowing red.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }

    if (!IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("A pair of long fangs extend from your mouth.\n\r",ch);
	act("A pair of long fangs extend from $n's mouth.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Razor sharp talons extend from your fingers.\n\r",ch);
	act("Razor sharp talons extend from $n's fingers.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }

    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);

    sprintf(buf,"%s the werewolf", ch->name);

    free_string(ch->morph);
    ch->morph = str_dup(buf);
    ch->rage = 10;

    act("You throw back your head and howl with rage!", ch, NULL, NULL, TO_CHAR);
    act("$n throws back $s head and howls with rage!", ch, NULL, NULL, TO_ROOM);
    act("You suddenly swipe a POWERFUL claw across the room RIPPING apart EVERYTHING!!!", ch, NULL, NULL, TO_CHAR);
    act("$n suddenly swipes a POWERFUL claw across the room RIPPING apart EVERYTHING!!!", ch, NULL, NULL, TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;
       
       if ( is_same_group(ch,vch) || is_safe(ch,vch) ) 
          continue;

       ch->attack_type = ATTACK_PHYSICAL;
       ch->ele_attack = ATTACK_LUNAR;
       ch->attack_var = SLASH;
       ch->attack_focus = DIRECT;

       if ( ch->class_2 != 0 )
          hurt_person( ch, vch, 500000, TRUE );
          else hurt_person( ch, vch, 1000000 + UMIN( 1000000, ch->pkill * 50000 ), TRUE );
    }

    ch->wait = 0;
}

void do_unwerewolf( CHAR_DATA *ch, char *argument )
{
    char       arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) ) return;
    if (!IS_SET(ch->special, SPC_WOLFMAN)) return;
    REMOVE_BIT(ch->special, SPC_WOLFMAN);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string(ch->morph);
    ch->morph = str_dup("");

    if (IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Your talons slide back into your fingers.\n\r",ch);
	act("$n's talons slide back into $s fingers.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if (IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("Your fangs slide back into your mouth.\n\r",ch);
	act("$n's fangs slide back into $s mouth.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }

    ch->rage = 0;

    send_to_char("Your coarse hair shrinks back into your body.\n\r",ch);
    act("$n's coarse hair shrinks back into $s body.",ch,NULL,NULL,TO_ROOM);
}

void do_possession( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_DOMI] < 3 && IS_CLASS(ch, CLASS_MAGE) )
    {
        send_to_char("You must obtain at least level 3 in Dominate to use Possession.\n\r",ch);
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
	sprintf(buf,"You break your hold over %s.\n\r",familiar->short_descr);
	send_to_char( buf, ch );
	familiar->wizard = NULL;
	ch->pcdata->familiar = NULL;
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to Possess?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "What an intelligent idea!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to possess them.\n\r", ch );
	return;
    }

    if (victim->level > (ch->spl[RED_MAGIC] * 0.25))
    {
	send_to_char( "They are too powerful.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 50 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= 50;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
    return;
}

void do_familiar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_fcommand( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && (ch->power[DISC_VAMP_DOMI] < 3) )
    {
	send_to_char("You are not trained in the Dominate discipline.\n\r",ch);
	return;
    }

    if ( ( victim = ch->pcdata->familiar ) == NULL )
    {
	send_to_char( "But you don't have a familiar!\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar do?\n\r", ch );
	return;
    }
    interpret(victim,argument);
    return;
}

void do_flex( CHAR_DATA *ch, char *argument )
{
    act("You flex your bulging muscles.",ch,NULL,NULL,TO_CHAR);
    act("$n flexes $s bulging muscles.",ch,NULL,NULL,TO_ROOM);

    ch->wait = 8;

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
          send_to_char("You rip away the spear which was pinning your feet down.\n\r",ch);
          REMOVE_BIT(ch->more, MORE_IMPALE); 
       }

    if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
	act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->extra, TIED_UP);
    }

    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	affect_strip(ch, gsn_web);
    }

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }

    if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
       {
          stc("#gThe earth surrounding you starts to slowly crack.#n\n\r",ch);
          SET_BIT(ch->mflags, MAGE_EMBRACED1);
          REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
       }
       else if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
               {
                  stc("#gThe earth surrounding you falls away.#n\n\r",ch);
                  REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
               }

    if ( IS_SET(ch->more, MORE_NET) )
    {
       send_to_char( "The spiritual net trapping you is ripped to shreds.\n\r", ch );
       REMOVE_BIT(ch->more, MORE_NET);
    }

	if ( IS_SET(ch->more, MORE_HAMMER) )
	   {
	   send_to_char( "You flex and the hammer #0axe#n comes out your arm.\n\r",ch);
	   REMOVE_BIT(ch->more, MORE_HAMMER);
	   }
}

void do_beastlike( CHAR_DATA *ch, char *argument )
{
}

void do_rage( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

  if ( IS_CLASS(ch, CLASS_WEREWOLF) )
  {
    if ( !IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You start snarling angrilly.\n\r",ch);
	act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM);

      if ( ch->class_2 != 0 )
         ch->rage = 10;
         else ch->rage += number_range( 5, 12 );

	if (ch->rage >= 10) do_werewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }

    send_to_char("But you are already in a rage!\n\r",ch);
  }

    if (IS_CLASS(ch, CLASS_DEMON))
    {
     if (ch->beast < 1)
	ch->beast = 1;

        if (ch->beast > ch->rage || ch->rage < 125)
	{
	    ch->rage += number_range(ch->generation,25);
	    stc("You scream as you plunge into a mad attacking frenzy.\n\r",ch);
	    act("$n screams in frenzied rage.",ch,NULL,NULL,TO_ROOM);
	    if (number_percent() < (ch->beast + 1))
	    {
		stc("You feel the beast take more control.\n\r",ch);
		ch->beast += 1;
		if (ch->beast > 100)
		  ch->beast = 100;
	    }
	    WAIT_STATE(ch, 12);
	    return;
	}
       
        stc("You have reached the pinnacle of your frenzy.\n\r",ch);
    }
}

void do_calm( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

 if (IS_CLASS(ch, CLASS_WEREWOLF) )
 {
    if (IS_SET(ch->special, SPC_WOLFMAN))
    {
      if ( ch->fight_timer > 0 )
         {
            stc( "#w#zNot with a fight timer.#n\n\r", ch );
            return;
         }

	send_to_char("You take a deep breath and calm yourself.\n\r",ch);
	act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
      ch->rage = 0;

	if (ch->rage < 100) do_unwerewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are not in crinos form!\n\r",ch);
 }

if ( IS_CLASS(ch, CLASS_NINJA) )
   {
send_to_char("You take a deep breath and calm yourself.\n\r",ch);
        act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
ch->rage = 0;
WAIT_STATE(ch,12);
        return;
    }
   if (IS_CLASS(ch, CLASS_DEMON))
   {
    if (ch->beast >= 100) ch->beast = 100;
    if (ch->beast <= 0) ch->beast = 0;

    ch->rage -= (40 - (ch->beast / 3) +dice (1, 10));
    if (ch->rage <= 0)
    {
	ch->rage = 0;
	send_to_char("You have calmed out of your rage completely.\n\r", ch);
    }
    else
    {
      send_to_char("You hold your breath and try to calm your frenzy.\n\r", ch);
      WAIT_STATE(ch, 8);
    }
    return;
  } 
    return;
}

void do_web( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DROW) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->gifts[SPIDER_TOTEM] < 2 )
       {
          stc("You must obtain level two in Spider to use Web.\n\r",ch);
          return;
       }

    if ( IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_WEB) )
 	 { 
          send_to_char("You don't have that power yet.\n\r", ch );
	    return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot web yourself.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You cannot web an ethereal person.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

void do_birth( CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];
    bool raped = FALSE;

    if (IS_NPC(ch)) return;

    if (!IS_EXTRA(ch, EXTRA_PREGNANT))
    {
	send_to_char("But you are not even pregnant!\n\r",ch);
	return;
    }

    if (!IS_EXTRA(ch, EXTRA_LABOUR))
    {
	send_to_char("You're not ready to give birth yet.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
	if (ch->pcdata->genes[4] == SEX_MALE)
	    send_to_char( "What do you wish to name your little boy?\n\r", ch );
	else if (ch->pcdata->genes[4] == SEX_FEMALE)
	    send_to_char( "What do you wish to name your little girl?\n\r", ch );
	else
	    send_to_char( "What do you wish to name your child?\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( char_exists(FALSE,argument) )
    {
	send_to_char( "That player already exists.\n\r", ch );
	return;
    }

    strcpy(buf2,ch->pcdata->cparents);
    strcat(buf2," ");
    strcat(buf2,argument);

    if (!birth_ok(ch, buf2))
    {
	send_to_char( "Bug - please inform KaVir.\n\r", ch );
	return;
    }
    argument[0] = UPPER(argument[0]);

    if(!raped) birth_write( ch, argument );

    ch->pcdata->genes[9] += 1;
    REMOVE_BIT(ch->extra, EXTRA_PREGNANT);
    REMOVE_BIT(ch->extra, EXTRA_LABOUR);
    save_char_obj(ch);
    return;
}

bool birth_ok( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
    char mum [MAX_INPUT_LENGTH];
    char dad [MAX_INPUT_LENGTH];
    char child [MAX_INPUT_LENGTH];

    argument = one_argument( argument, mum );
    argument = one_argument( argument, dad );
    argument = one_argument( argument, child );

    if (dad[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }

    if (child[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    dad[0] = UPPER(dad[0]);
    if (!str_cmp(dad,"Kavir")) strcpy(dad,"KaVir");
    child[0] = UPPER(child[0]);
    if (ch->pcdata->genes[4] == SEX_FEMALE)
    {
	send_to_char("You give birth to a little girl!\n\r",ch);

      if ( IS_IMMORTAL(ch) )
         sprintf(buf,"An Immortal has given birth to her daughter, named %s!",child);
         else sprintf(buf,"%s has given birth to %s's daughter, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    else if (ch->pcdata->genes[4] == SEX_MALE)
    {
	send_to_char("You give birth to a little boy!\n\r",ch);

      if ( IS_IMMORTAL(ch) )
         sprintf(buf,"An Immortal has given birth to her son, named %s!",child);
         else sprintf(buf,"%s has given birth to %s's son, named %s!",ch->name,dad,child);

	do_info(ch,buf);
	return TRUE; 
    }
    return FALSE;
}


bool char_exists( bool backup, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    bool found = FALSE;

    fclose( fpReserve );
    if (backup)
    	sprintf( buf, "%sbackup/%s", PLAYER_DIR, capitalize( argument ) );
    else
    	sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( buf, "r" ) ) != NULL )
    {
	found = TRUE;
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}

void birth_write( CHAR_DATA *ch, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    char *strtime;

    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';

    sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen(buf, "w") ) != NULL) 
    {  
       int sn;
       int i;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYERS"                );

    fprintf( fp, "Name         %s~\n",  capitalize( argument )          );
    fprintf( fp, "Version      %d\n",  ch->version  );
    fprintf( fp, "Switchname   %s~\n",  capitalize( argument )  );
    fprintf( fp, "Lastname     %s~\n",  ch->pcdata->lastname  );
    fprintf( fp, "Bios         15 2 2\n" );
    fprintf( fp, "ShortDescr   ~\n" );
    fprintf( fp, "LongDescr    ~\n" );
    fprintf( fp, "ObjDesc      ~\n"	);
    fprintf( fp, "Description  ~\n" );
    fprintf( fp, "Lord         ~\n" );
    fprintf( fp, "Kingdom      0\n"	);

    fprintf( fp, "Class        0\n" );
    fprintf( fp, "Morph        ~\n" );

    fprintf( fp, "Createtime   %s~\n",  str_dup( strtime )      );
    fprintf( fp, "Lasttime     %s~\n",  ch->lasttime            );
    fprintf( fp, "Lasthost     %s~\n",  ch->lasthost            );
    fprintf( fp, "Poweraction  ~\n" );
    fprintf( fp, "Powertype    ~\n" );
    fprintf( fp, "Prompt       ~\n" );
    fprintf( fp, "Cprompt      ~\n" );
    fprintf( fp, "Sex          %d\n",   ch->pcdata->genes[4]  );
    fprintf( fp, "Immune       0\n" );
    fprintf( fp, "Polyaff      0\n" );
    /* Dh Crap */
    fprintf( fp, "Gnosis       0\n"	);
    fprintf( fp, "Clan         ~\n"	);
    fprintf( fp, "CurrentForm  0\n" );
    fprintf( fp, "Rage         0\n" );
    fprintf( fp, "Generation   5\n" );
    fprintf( fp, "Flag2       0\n" );
    fprintf( fp, "Flag3       0\n" );
    fprintf( fp, "Flag4       0\n" ); 
    fprintf( fp, "SilTol      0\n" );
    fprintf( fp, "Souls	      0\n" );
/* Lala */
    fprintf( fp, "Itemaffect   0\n" );
    fprintf( fp, "Beast        0\n" );
    fprintf( fp, "Spectype     0\n" );
    fprintf( fp, "Specpower    0\n" );
    fprintf( fp, "Home         3001\n" );
    fprintf( fp, "More         0\n" );
    fprintf( fp, "Level        2\n" );
    fprintf( fp, "Trust        2\n" );
    fprintf( fp, "Security     0\n" );  /* OLC */

    fprintf( fp, "Paradox      0 0 0\n" ); 
    fprintf( fp, "Played       0\n" );
    fprintf( fp, "Room         3001\n" );

    fprintf( fp, "PkPdMkMd     0 0 5 0\n" );
    fprintf( fp, "Alos         0\n" );
    fprintf( fp, "Awin         0\n" ); 
    fprintf( fp, "Weapons      0 0 0 0 0 0 0 0 0 0 0 0 0\n" );
    fprintf( fp, "Spells       0 0 0 0 0\n" );
    fprintf( fp, "Combat       %d %d %d %d %d %d %d %d\n",
	ch->cmbt[0], ch->cmbt[1], ch->cmbt[2], ch->cmbt[3],
	ch->cmbt[4], ch->cmbt[5], ch->cmbt[6], ch->cmbt[7] );
    fprintf( fp, "Locationhp   0 0 0 0 0 0 0\n" );
    fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n", ch->max_hit/2, ch->max_hit/2, ch->max_mana/2, ch->max_mana/2, ch->max_move/2, ch->max_move/2 );
    fprintf( fp, "Gold         %d\n",   ch->gold                );

    fprintf( fp, "Explevel     0\n" );
    fprintf( fp, "Expgained    0\n" );
    fprintf( fp, "Exp          0\n" );
    fprintf( fp, "Levelexp     0\n" );
    fprintf( fp, "Act          %d\n",   ch->act                 );
    fprintf( fp, "Special      0\n" );
    fprintf( fp, "Newbits      0\n" );

    fprintf( fp, "Extra        0\n" );
    fprintf( fp, "AffectedBy   0\n" );
       fprintf( fp, "MageFlags    0\n" );
	fprintf( fp, "Spheres      0 0 0 0 0 0 0 0 0\n" );
	fprintf( fp, "Mimmunity    0 0 0\n" );
	fprintf( fp, "Quint        0 0 0\n" );

    fprintf( fp, "Position     9\n" );

    fprintf( fp, "Practice     0\n" );
    fprintf( fp, "SavingThrow  0\n" );
    fprintf( fp, "Alignment    1000\n" );
    fprintf( fp, "XHitroll     0\n" );
    fprintf( fp, "XDamroll     0\n" );
    fprintf( fp, "Hitroll      0\n" );
    fprintf( fp, "Damroll      0\n" );
    fprintf( fp, "Armor        100\n" );
    fprintf( fp, "Wimpy        0\n" );
    fprintf( fp, "Deaf         0\n" );


    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum         %d\n",       ch->pIndexData->vnum    );
    }
    else
    {
	fprintf( fp, "Password     %s~\n",      ch->pcdata->pwd         );
	fprintf( fp, "Bamfin       %s~\n",      ch->pcdata->bamfin      );
	fprintf( fp, "Bamfout      %s~\n",      ch->pcdata->bamfout     );
	fprintf( fp, "Questsrun    0\n"	);
	fprintf( fp, "Queststotal  0\n"	);
	fprintf( fp, "Title        ~\n"  );
	fprintf( fp, "Bounty	   0\n"	);
	fprintf( fp, "Conception   ~\n"  );
	fprintf( fp, "Parents      %s~\n",      ch->pcdata->parents     );
	fprintf( fp, "Cparents     %s~\n",      ch->pcdata->cparents    );
	fprintf( fp, "Marriage     ~\n" );
	fprintf( fp, "Email        ~\n" );
	fprintf( fp, "AttrPerm     %d %d %d %d %d\n",
	    ch->pcdata->perm_str,
	    ch->pcdata->perm_int,
	    ch->pcdata->perm_wis,
	    ch->pcdata->perm_dex,
	    ch->pcdata->perm_con );

	fprintf( fp, "AttrMod      0 0 0 0 0\n" );

	fprintf( fp, "Quest        0\n" );
	fprintf( fp, "Wolf         0\n" );

	fprintf( fp, "Stage        0 0 0\n" );
	fprintf( fp, "Score        %d %d %d %d %d %d\n",
	    ch->pcdata->score[0],
	    ch->pcdata->score[1],
	    ch->pcdata->score[2],
	    ch->pcdata->score[3],
	    ch->pcdata->score[4],
	    ch->pcdata->score[5]);
	fprintf( fp, "Genes        0 0 0 0 0 0 0 0 0 0\n" );
	fprintf( fp, "Power        ");
	    for (sn=0; sn < 20; sn++)
	    {
		fprintf(fp, "0 " );
	    }
	    fprintf(fp, "\n");

	fprintf( fp, "Stats        ");
	    for (sn=0; sn < 12; sn++)
	    {
		fprintf(fp, "0 " );
	    }
	    fprintf(fp, "\n");

	fprintf( fp, "Condition    0 100 100\n" );

	fprintf( fp, "StatAbility  0 0 0 0\n" );

	fprintf( fp, "StatAmount   0 0 0 0\n" );

	fprintf( fp, "StatDuration 0 0 0 0\n" );

	fprintf( fp, "Exhaustion   %d\n", ch->pcdata->exhaustion );

	/* Save note board status */
	/* Save number of boards in case that number changes */
	fprintf (fp, "Boards       %d ", MAX_BOARD);
	for (i = 0; i < MAX_BOARD; i++)
	fprintf (fp, "%s %ld ", boards[i].short_name, ch->pcdata->last_note[i]);
	fprintf (fp, "\n");
	
      fprintf( fp, "End\n\n" ); 
	fprintf( fp, "#END\n" );
      fclose( fp );
    }
    }
}
