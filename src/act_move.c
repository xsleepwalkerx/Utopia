/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license._doc' as well the Merc       *
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


void horn args( ( CHAR_DATA *ch ) );


char *	const	dir_name	[]		=
{
    "#rnorth#n", "#geast#n", "#csouth#n", "#bwest#n", "#yup#n", "#pdown#n"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );
int	count_imms	args( ( CHAR_DATA *ch ) );
bool	check_track	args( ( CHAR_DATA *ch, int direction ) );
void	add_tracks	args( ( CHAR_DATA *ch, int direction ) );
void	drow_hate	args( ( CHAR_DATA *ch) );

void check_traps( CHAR_DATA *ch )
{
   OBJ_DATA *obj, *obj_next;

   if ( ch->in_room->trap == ROOM_TRAP_CALTROPS )
      {
         stc("\n\r#gYou #yscream loudly #gas hundreds of #wneedles #gpierces your feet.#n\n\r",ch);
         stc("#gYou #ycollapse #gto the ground from the effects of the #edeadly #gpoison.#n\n\r",ch);
         act("#g$n #yscreams loudly #gas hundreds of #wneedles #gpierces $s feet.#n",ch,NULL,NULL,TO_ROOM);
         act("#g$n #ycollapses #gto the ground from the effects of the #edeadly #gpoison.#n",ch,NULL,NULL,TO_ROOM);

         ch->hit /= 2;
         ch->agg = 100;

         if ( !IS_SET( ch->more, MORE_POISON ) )
            SET_BIT( ch->more, MORE_POISON );

         return;
      }

   if ( ch->in_room->trap == ROOM_TRAP_LANDMINE )
      {
         act("\n\r#yYou #wscream loudly #yas a #C#zMASSIVE#n #yexplosion devastates all!#n",ch,NULL,NULL,TO_CHAR);
         act("#yYou #wclutch #P#zwildly#n #wat nothing a you are flung into the #Cskies#w!#n",ch,NULL,NULL,TO_CHAR);
         act("\n\r#y$n #wscreams loudly #yas a #C#zMASSIVE#n #yexplosion devastates all!#n",ch,NULL,NULL,TO_ROOM);
         act("#yYou #wclutches #P#zwildly#n #wat nothing a $e is flung into the #Cskies#w!#n",ch,NULL,NULL,TO_ROOM);

         ch->air_timer = 15;
         return;
      }

   if ( ch->in_room->trap == ROOM_TRAP_BOULDERS )
      {
         act("\n\r#oYou #gscream loudly #oas boulders #Crain #odown from the skies.#n",ch,NULL,NULL,TO_CHAR);
         act("#oYou #gstagger #oonto your knees as you are #wknocked #eout #gof commission!#n",ch,NULL,NULL,TO_CHAR);
         act("\n\r#o$n #gscreams loudly #oas boulders #Crain #odown from the skies.#n",ch,NULL,NULL,TO_ROOM);
         act("#o$n #gstaggers #oonto $s knees as $e is #wknocked #eout #gof commission!#n",ch,NULL,NULL,TO_ROOM);

         ch->position = POS_STUNNED;
         return;
      }

   if ( ch->in_room->trap == ROOM_TRAP_JAGGAR )
      {
         act( "\n\rA cloak man suddenly leap in front of you as his eyes pierce through your soul.", ch, NULL, NULL, TO_CHAR );
         act( "\n\rA cloak man suddenly leap in front of $n as his eyes pierce through $s soul.", ch, NULL, NULL, TO_ROOM );
         act( "Sensing the end near, you frantically try to cut the assassin in half!", ch, NULL, NULL, TO_CHAR );
         act( "Sensing the end near, $n frantically tries to cut the assassin in half!", ch, NULL, NULL, TO_ROOM );
         act( "No matter what you do, your weapons seem to go through his body.", ch, NULL, NULL, TO_CHAR );
         act( "No matter what $n does, $s weapons seem to go through his body.", ch, NULL, NULL, TO_ROOM );
         act( "Unsheathing his daggers, the last thing you see is Jaggar fading away...", ch, NULL, NULL, TO_CHAR );
         act( "Unsheathing his daggers, the last thing $n sees is Jaggar fading away...", ch, NULL, NULL, TO_ROOM );

         stop_fighting( ch, TRUE );
         ch->hit = -1000010;
         ch->position = POS_MORTAL;

         if ( number_percent() > 60 )
            ch->in_room->trap = 0;

         return;
      }

   if ( ch->in_room->trap == ROOM_TRAP_TRANSPORT )
      {
         int vnum;

         act("\n\r#CYour #weyes open wide in #Rsurprise #was you are engulfed in #yblinding #wlight.#n",ch,NULL,NULL,TO_CHAR);
         act("#CBefore #wyou can #precover#w, you are suddenly #P#zwarped#n #waway to another room!#n",ch,NULL,NULL,TO_CHAR);
         act("\n\r#C$n's #weyes opens wide in #Rsurprise #was $e is engulfed in #yblinding #wlight.#n",ch,NULL,NULL,TO_ROOM);
         act("#CBefore #w$n can #precover#w, $e is suddenly #P#zwarped#n #waway to another room!#n",ch,NULL,NULL,TO_ROOM);

         if ( number_percent() > 60 )
            ch->in_room->trap = 0;

         vnum = ( ch->in_room->vnum - 33500 ) / 20;
         vnum *= 20;
         vnum = number_range( vnum, vnum + 17 );

         char_from_room( ch );
         char_to_room( ch, get_room_index( 33500 + vnum ) );
         do_look( ch, "" );
         return;
      }

   if ( ch->in_room->trap == ROOM_TRAP_STAKE )
      {
         act("\n\r#rYou #cscream loudly #ras the #oground #rbeneath you #y#zdisappears#n#r!#n",ch,NULL,NULL,TO_CHAR);
         act("#rYou #ggrasp #w#zwildly#n #rat thin air as you fall down into a #Csea #rof #ostakes#r!#n",ch,NULL,NULL,TO_CHAR);
         act("\n\r#r$n #cscreams loudly #ras the #oground #rbeneath $m #y#zdisappears#n#r!#n",ch,NULL,NULL,TO_ROOM);
         act("#r$n #ggrasps #w#zwildly#n #rat thin air as $e falls down into a #Csea #rof #ostakes#r!#n",ch,NULL,NULL,TO_ROOM);

         SET_BIT( ch->more2, MORE2_NOLEGS );
         SET_BIT( ch->more, MORE_IMPALE );

         return;
      }

   if ( ch->in_room->trap == ROOM_TRAP_NIGHTMARE )
      {
         act("\n\r#wYou #ego #won #eyour #wknees #estruggling #wfor #ebreath #was #ea #wloud #ecackle #wfills #ethe #wair#e!#n",ch,NULL,NULL,TO_CHAR);
         act("#eThe last thing you hear was #y'#wN#ei#wG#eh#wT#em#wA#er#wE#e!#y' #eas everything #Rblackens #eout...#n",ch,NULL,NULL,TO_CHAR);
         act("\n\r#w$n #egoes #won #e$s #wknees #estruggling #wfor #ebreath #was #ea #wloud #ecackle #wfills #ethe #wair#e!#n",ch,NULL,NULL,TO_ROOM);
         act("#eThe last thing $n hears was #y'#wN#ei#wG#eh#wT#em#wA#er#wE#e!#y' #eas everything #Rblackens #eout...#n",ch,NULL,NULL,TO_ROOM);

         ch->position = POS_SLEEPING;
         ch->wait = 120;

         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_BLAZE ) && !IS_CLASS( ch, CLASS_DEMON ) && !IS_CLASS( ch, CLASS_PHOENIX ) )
      {
         act("#RYou #yscream #Rin agony as the #yhungry flames #Reat through your flesh!#n", ch, NULL, NULL, TO_CHAR );
         act("#R$n #yscreams #Rin agony as the #yhungry flames #Reat through $s flesh!#n", ch, NULL, NULL, TO_ROOM );

         if ( ch->hit > 0 )
            ch->hit = UMAX( 0, ch->hit - is_elemental( ATTACK_FIRE, ch, 1500000 ) );

         update_pos( ch );
         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_ICETRAP ) && !IS_CLASS( ch, CLASS_SNOW_ELF ) && !IS_SET( ch->more, MORE_FROZEN ) )
      {
         act("The #Cice#n trap snaps at your feet and #Lfreezes#n you!",ch,NULL,NULL,TO_CHAR);
         act("The #Cice#n trap snaps at $n's feet and #Lfreezes#n $m!",ch,NULL,NULL,TO_ROOM);
		  
         SET_BIT( ch->more, MORE_FROZEN );
         ch->freeze_timer = number_range( 2, 3 );
      }

   for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
   {
      obj_next = obj->next_content;

      if ( !IS_CLASS( ch, CLASS_NINJA ) && !IS_CLASS( ch, CLASS_NIGHT_BLADE )
           && !IS_CLASS( ch, CLASS_DRAGON ) && obj->pIndexData->vnum == 127 && obj->item_type != ITEM_HUNT )
         {
            stc("\n\rYou scream loudly as hundrends of needles pierces your feet.\n\r",ch);
            stc("You collapse to the ground from the effects of the deadly poison.\n\r",ch);
            act("$n screams loudly as hundrends of needles pierces $s feet.",ch,NULL,NULL,TO_ROOM);
            act("$n collapses to the ground from the effects of the deadly poison.",ch,NULL,NULL,TO_ROOM);

            if ( IS_CLASS( ch, CLASS_DUERGAR ) )
               ch->hit = UMIN( ch->max_hit, ch->hit + 1000000 );
               else if ( ch->hit > 500000 )
                       ch->hit -= 500000;
                       else ch->hit = 0;

            if ( !IS_SET( ch->more, MORE_POISON ) )
               SET_BIT( ch->more, MORE_POISON );

            update_pos( ch );

            if ( ch->position > POS_SLEEPING )
               ch->position = POS_SLEEPING;

            extract_obj(obj);
            return;
         }
   }
}

void move_char( CHAR_DATA *ch, int door )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    CHAR_DATA *mount;
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    DESCRIPTOR_DATA *d;
    char wall[MAX_STRING_LENGTH];
    char buf  [MAX_STRING_LENGTH];
    char poly [MAX_STRING_LENGTH];
    char mount2 [MAX_INPUT_LENGTH];
    char leave [20];
    int revdoor;

    if ( IS_SET(ch->more2, MORE2_DIG) )
       return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       return;

    if ( IS_SET(ch->more2, MORE2_NOLEGS) )
    {
        send_to_char("Your legs are disabled, preventing all movement.\n\r",ch);
        return;
    }

    if(IS_SET(ch->affected_by2,AFF_SEVERED))
    {
        send_to_char("Move without legs? How?\n\r",ch);
        return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char("It is impossible to move from within this colourful vortex.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
       }

    if ( is_memb(ch) ) return;

    if ( door < 0 || door > 5 )

    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->to_room      ) == NULL )
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
	return;
    }

    sprintf(wall,"walln");
    obj = get_obj_list(ch,wall,ch->in_room->contents);
    if ( obj != NULL && obj->pIndexData->vnum == 30043 )
       {
          send_to_char("\n\rYou cant pass the flowing wall of blood.\n\r",ch);
          return;
       }

    sprintf(wall,"walle");
    obj = get_obj_list(ch,wall,ch->in_room->contents);
    if ( obj != NULL && obj->pIndexData->vnum == 30045 )
       {
          send_to_char("\n\rYou cant pass the flowing wall of blood.\n\r",ch);
          return;
       }

    sprintf(wall,"walls");
    obj = get_obj_list(ch,wall,ch->in_room->contents);
    if ( obj != NULL && obj->pIndexData->vnum == 30044 )
       {
          send_to_char("\n\rYou cant pass the flowing wall of blood.\n\r",ch);
          return;
       }

    sprintf(wall,"wallw");
    obj = get_obj_list(ch,wall,ch->in_room->contents);
    if ( obj != NULL && obj->pIndexData->vnum == 30046 )
       {
          send_to_char("\n\rYou cant pass the flowing wall of blood.\n\r",ch);
          return;
       }

    sprintf(wall,"wallu");
    obj = get_obj_list(ch,wall,ch->in_room->contents);
    if ( obj != NULL && obj->pIndexData->vnum == 30048 )
       {
          send_to_char("\n\rYou cant pass the flowing wall of blood.\n\r",ch);
          return;
       }

    sprintf(wall,"walld");
    obj = get_obj_list(ch,wall,ch->in_room->contents);
    if ( obj != NULL && obj->pIndexData->vnum == 30047 )
       {
          send_to_char("\n\rYou cant pass the flowing wall of blood.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_ENTOMB) )
       {
          send_to_char("A huge wall of ice blocks your way.\n\r", ch);
          return;
       }

   if ( IS_SET(ch->in_room->room_flags, ROOM_CATACOMB) )
      {
         send_to_char( "A huge wall of granite blocks your way.\n\r", ch );
         return;
      }

    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    &&   !IS_AFFECTED(ch, AFF_PASS_DOOR)
    &&   !IS_AFFECTED(ch, AFF_ETHEREAL)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE) )
    {
	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) &&
	    ch->power[DISC_WERE_BOAR] > 0 && !IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
	{
	    act( "You smash open the $d.", ch, NULL, pexit->keyword, TO_CHAR );
	    act( "$n smashes open the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	}
	else
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	    return;
	}
    }

    if (IS_SET(pexit->exit_info, EX_PRISMATIC_WALL) &&  IS_SET(pexit->exit_info, EX_CLOSED))
    {
	stc( "The prismatic wall prevents movement through this closed exit.\n\r",ch);
	return;
    }

    if (IS_NPC(ch) && (mount=ch->mount) != NULL && IS_SET(ch->mounted,IS_MOUNT))
    {
	send_to_char( "You better wait for instructions from your rider.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_FLAMING) && !IS_CLASS( ch, CLASS_DEMON )
         && !IS_CLASS( ch, CLASS_PHOENIX ) )
       {
          act("$n bursts through the wall of fire.", ch, NULL, NULL, TO_ROOM);
          send_to_char("You jump through the flames.\n\r", ch);
          stc("The flames sear your flesh.\n\r",ch);

          if ( ch->hit > 0 )
             ch->hit = UMAX( 0, ch->hit - is_elemental( ATTACK_FIRE, ch, 750000 ) );
             else ch->position = POS_STUNNED;

          if ( ch->hit == 0 )
             ch->position = POS_STUNNED;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_ICYSWORD) && !IS_CLASS( ch, CLASS_ASSASSIN ) )
       {
          act("$n screams loudly as $e jumps through the wall of icy swords.", ch, NULL, NULL, TO_ROOM);
          send_to_char("You scream as you jump through the wall of icy swords.\n\r", ch);

          if ( ch->hit > 0 )
             ch->hit = URANGE( 0, ch->hit - is_elemental( ATTACK_ICE, ch, 750000 ), ch->max_hit );
             else ch->position = POS_STUNNED;

          if ( ch->hit == 0 )
             ch->position = POS_STUNNED;
       }

    if ( room_is_private( to_room ))
    {
	if ( IS_NPC( ch ) || ch->trust < MAX_LEVEL)
	{
		send_to_char( "That room is private right now.\n\r", ch );
		return;
	}
	else 
		send_to_char( "That room is private (Access granted).\n\r", ch);
    }

    if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_LEG)) &&
	 (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_LEG)) &&
	 (IS_ARM_L(ch,BROKEN_ARM) || IS_ARM_L(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_HOLD) != NULL) &&
	 (IS_ARM_R(ch,BROKEN_ARM) || IS_ARM_R(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	send_to_char( "You need at least one free arm to drag yourself with.\n\r", ch );
	return;
    }
    else if ( IS_BODY(ch,BROKEN_SPINE) &&
	 (IS_ARM_L(ch,BROKEN_ARM) || IS_ARM_L(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_HOLD) != NULL) &&
	 (IS_ARM_R(ch,BROKEN_ARM) || IS_ARM_R(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	send_to_char( "You cannot move with a broken spine.\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) )
    {
	int move = 0;
	if ( in_room->sector_type == SECT_AIR
      || IS_SET(ch->in_room->room_flags, ROOM_QUICKSAND)
	||   to_room->sector_type == SECT_AIR )
	{
	    if ( !IS_AFFECTED(ch, AFF_FLYING) &&
	( !IS_NPC(ch) && ( !IS_CLASS(ch, CLASS_DROW) ||
!IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))) &&
	( !IS_NPC(ch) && ( !IS_CLASS(ch, CLASS_SOULREAVER) ||
!IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))) &&
	( !IS_NPC(ch) && ( !IS_CLASS(ch, CLASS_FIEND) ||
!IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))) &&
	( !IS_NPC(ch) && ( !IS_CLASS(ch, CLASS_DRAGON_KNIGHT) ||
!IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))) &&
	(!IS_NPC(ch) && (!IS_CLASS(ch, CLASS_DRAGON) ||
!IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))) &&
	(!IS_NPC(ch) && (!IS_CLASS(ch, CLASS_ANGEL) ||
!IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))) &&
!IS_CLASS(ch, CLASS_DRAGOON) &&
		 (!IS_NPC(ch) && !IS_VAMPAFF(ch, VAM_FLYING) &&
		 (!IS_NPC(ch) && !IS_DEMAFF(ch,DEM_UNFOLDED)))
	    &&   !((mount=ch->mount) != NULL && IS_SET(ch->mounted,IS_RIDING) &&
		 IS_AFFECTED(mount, AFF_FLYING) ) )
	    {
		send_to_char( "You can't fly.\n\r", ch );
		return;
	    }
	}

	if ( in_room->sector_type == SECT_WATER_NOSWIM
	||   to_room->sector_type == SECT_WATER_NOSWIM )
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;
	    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
	    {
		if ( IS_VAMPAFF(ch, VAM_FLYING) )
		    found = TRUE;
		else if ( IS_POLYAFF(ch, POLY_SERPENT) )
		    found = TRUE;
		else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
		    found = TRUE;
   	      else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		    found = TRUE;
   	      else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_FIEND) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		    found = TRUE;
   	      else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DRAGON_KNIGHT) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		    found = TRUE;
   	      else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_SOULREAVER) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		    found = TRUE;
	      else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_ANGEL) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		    found = TRUE;
	      else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		    found = TRUE;
            else if ( IS_CLASS(ch, CLASS_DRAGOON) )
		    found = TRUE;
		else if ((mount=ch->mount) != NULL && IS_SET(ch->mounted, IS_RIDING) 
		    && IS_AFFECTED(mount, AFF_FLYING))
		    found = TRUE;
		else
		{
		    send_to_char( "You are unable to cross running water.\n\r", ch );
		    return;
		}
	    }
	    if ( IS_AFFECTED(ch, AFF_FLYING) )
		found = TRUE;
	    else if ( !IS_NPC( ch) && IS_DEMAFF(ch, DEM_UNFOLDED) )
		found = TRUE;
	else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DROW) &&
IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		found = TRUE;
	else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_FIEND) &&
IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		found = TRUE;
	else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DRAGON_KNIGHT) &&
IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		found = TRUE;
	else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_SOULREAVER) &&
IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
		found = TRUE;
	    if ( !found )
	    {
	    	for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    	{
		    if ( obj->item_type == ITEM_BOAT )
		    {
		    	found = TRUE;
		    	break;
		    }
	    	}
	    	if ( !found )
	    	{
		    send_to_char( "You need a boat to go there.\n\r", ch );
		    return;
	    	}
	    }
	}
	else if ( !IS_AFFECTED(ch, AFF_FLYING) && IS_POLYAFF(ch, POLY_FISH) )
	{
	    bool from_ok = FALSE;
	    bool to_ok = FALSE;
	    if ( in_room->sector_type == SECT_WATER_NOSWIM ) from_ok = TRUE;
	    if ( in_room->sector_type == SECT_WATER_SWIM   ) from_ok = TRUE;
	    if ( to_room->sector_type == SECT_WATER_NOSWIM ) to_ok   = TRUE;
	    if ( to_room->sector_type == SECT_WATER_SWIM   ) to_ok   = TRUE;
	    if ( !from_ok || !to_ok )
	    {
		send_to_char("You cannot cross land.\n\r",ch);
		return;
	    }
	}

	if ( IS_SET( ch->extra, EXTRA_FBITE ) )
         move = 300000;

	if ( ch->move < move && !IS_SET( ch->mounted, IS_RIDING ) )
         {
            send_to_char("#yYou #ware too #CExhausted#w.#n\n\r",ch);
            return;
         }

        if (IS_SET(pexit->exit_info, EX_IRON_WALL)
	&& !IS_AFFECTED(ch, AFF_PASS_DOOR) )
    {
	send_to_char("A towering wall of iron blocks your path.\n\r", ch);
	act("$n's path is blocked by the wall of iron.", ch, NULL, NULL, TO_ROOM);
	return;
    }

    if (IS_SET(pexit->exit_info, EX_MUSHROOM_WALL))
    {
	send_to_char("The mushrooms block your path.\n\r", ch);
	return;
    }

    if (IS_SET(pexit->exit_info, EX_SWORD_WALL)
	&& ch->class == 0)
    {
	act("$n bursts through the wall of swords.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You jump through the swords and are unaffected.\n\r", ch);
    }

    else if ( IS_SET(pexit->exit_info, EX_SWORD_WALL)	&& ch->class != 0 && !IS_NPC(ch) && ch->level > 2)
    {
	act("$n jumps through the wall of swords.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You jump through the swords.\n\r", ch);
	send_to_char("Aaaaaaaaarghhhhhhh! That hurt!\n\r", ch);
	ch->hit -= dice(6, 70);
    }

    if ( IS_SET(pexit->exit_info, EX_STAKE_WALL) && ch->level > 2 && !( IS_CLASS(ch, CLASS_DAYWALKER) ) )
    {
	act("$n screams loudly as $e jumps through the thorny stakes.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You scream as you jump through the thorny stakes.\n\r", ch);

      if ( ch->hit < 0 )
         ch->position = POS_STUNNED;
         else if ( IS_CLASS( ch, CLASS_VAMPIRE ) )
                 ch->hit = UMAX( 0, ch->hit - 3000000 );
                 else ch->hit = UMAX( 0, ch->hit - 1500000 );

      if ( ch->hit == 0 )
         ch->position = POS_STUNNED;
    }

    if ( IS_SET(pexit->exit_info, EX_ASH_WALL) )
    {
	send_to_char("You scream in agony as the wall of ash rips apart your life force.\n\r", ch);
	act("$n screams in agony as the wall of ash rips $s life force apart.", ch, NULL, NULL, TO_ROOM);
	ch->hit /= 2;
	ch->move /= 2;
    }

	WAIT_STATE( ch, 1 );
	if (!IS_SET(ch->mounted, IS_RIDING)) ch->move -= move;
    }


    /* Check for mount message - KaVir */
    if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
    {
	if (IS_NPC(mount))
	    sprintf(mount2," on %s.",mount->short_descr);
	else
	    sprintf(mount2," on %s.",mount->name);
    }
    else
	sprintf(mount2,".");

    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	sprintf(leave,"rolls");
    else if ( IS_SET(ch->more3, MORE3_SOUL) )
	sprintf(leave,"flashes");
    else if ( IS_AFFECTED(ch,AFF_ETHEREAL) )
	sprintf(leave,"floats");
    else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
	sprintf(leave,"swims");
    else if ( IS_SET(ch->polyaff,POLY_SERPENT) )
	sprintf(leave,"slithers");
    else if ( IS_SET(ch->polyaff,POLY_WOLF) )
	sprintf(leave,"stalks");
    else if ( IS_SET(ch->polyaff,POLY_FROG) )
	sprintf(leave,"hops");
    else if ( IS_SET(ch->polyaff,POLY_FISH) )
	sprintf(leave,"swims");
    else if ( !IS_NPC(ch) && IS_DEMAFF(ch,DEM_UNFOLDED) )
	sprintf(leave,"flies");
    else if ( IS_BODY(ch,BROKEN_SPINE) )
	sprintf(leave,"drags $mself");
    else if ( IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG) )
	sprintf(leave,"drags $mself");
    else if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_LEG) || IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"crawls");
    else if ( ch->hit < (ch->max_hit/4) )
	sprintf(leave,"crawls");
    else if ( (IS_LEG_R(ch,LOST_LEG) || IS_LEG_R(ch,LOST_FOOT)) &&
	 (!IS_LEG_L(ch,BROKEN_LEG) && !IS_LEG_L(ch,LOST_LEG) && 
	  !IS_LEG_L(ch,LOST_FOOT)) )
	sprintf(leave,"hops");
    else if ( (IS_LEG_L(ch,LOST_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (!IS_LEG_R(ch,BROKEN_LEG) && !IS_LEG_R(ch,LOST_LEG) &&
	  !IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"hops");
    else if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (!IS_LEG_R(ch,BROKEN_LEG) && !IS_LEG_R(ch,LOST_LEG) &&
	  !IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"limps");
    else if ( (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_FOOT)) &&
	 (!IS_LEG_L(ch,BROKEN_LEG) && !IS_LEG_L(ch,LOST_LEG) &&
	  !IS_LEG_L(ch,LOST_FOOT)) )
	sprintf(leave,"limps");
    else if ( ch->hit < (ch->max_hit/3) )
	sprintf(leave,"limps");
    else if ( ch->hit < (ch->max_hit/2) )
	sprintf(leave,"staggers");
    else if ( !IS_NPC(ch) )
    {
    
	if (ch->pcdata->condition[COND_DRUNK] > 10)
	    sprintf(leave,"staggers");
	else
	    sprintf(leave,"walks");
    }
    else
	sprintf(leave,"walks");

    if (form_data[ch->cur_form].move_word[0] != '\0')
    {
	leave[0] = '\0';
	sprintf(leave, form_data[ch->cur_form].move_word);
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	if ( ( victim = d->character ) == NULL )
	    continue;

        if ( ch->in_room == NULL || victim->in_room == NULL ) continue;

        if ( ch == victim || ch->in_room != victim->in_room) continue;

	if ( d->connected != CON_PLAYING || !can_see( ch, victim ) )
	    continue;

	if ( !IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK) && IS_AFFECTED(ch,AFF_POLYMORPH)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) )
	&& can_see(victim,ch))
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf(poly,"%s flies $T%s",ch->morph,mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"%s rides $T%s",ch->morph,mount2);
	    else
		sprintf(poly,"%s %s $T%s",ch->morph,leave,mount2);
	    act( poly, victim, NULL, dir_name[door], TO_CHAR );
	}
	else if ( !IS_AFFECTED(ch, AFF_SNEAK) 
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) )
	&& can_see(victim,ch))
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf(poly,"$n flies %s%s",dir_name[door],mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"$n rides %s%s",dir_name[door],mount2);
	    else
		sprintf(poly,"$n %s %s%s",leave,dir_name[door],mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
    }
    char_from_room( ch );
    char_to_room( ch, to_room );
    if      ( door == 0 ) {revdoor = 2;sprintf(buf,"the south");}
    else if ( door == 1 ) {revdoor = 3;sprintf(buf,"the west");}
    else if ( door == 2 ) {revdoor = 0;sprintf(buf,"the north");}
    else if ( door == 3 ) {revdoor = 1;sprintf(buf,"the east");}
    else if ( door == 4 ) {revdoor = 5;sprintf(buf,"below");}
    else                  {revdoor = 4;sprintf(buf,"above");}

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	if ( ( victim = d->character ) == NULL )
	    continue;

        if ( ch->in_room == NULL || victim->in_room == NULL ) continue;

        if ( ch == victim || ch->in_room != victim->in_room) continue;

	if ( d->connected != CON_PLAYING || !can_see( ch, victim ) )
	    continue;

	if ( !IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK) && IS_AFFECTED(ch,AFF_POLYMORPH) 
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) && can_see(victim,ch))
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf(poly,"%s flies in from %s%s",ch->morph,buf,mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"%s rides in from %s%s",ch->morph,buf,mount2);
	    else
		sprintf(poly,"%s %s in from %s%s",ch->morph,leave,buf,mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
	else if ( !IS_AFFECTED(ch, AFF_SNEAK) && can_see(victim,ch) 
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) )
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf( poly, "$n flies in from %s%s",buf,mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"$n rides in from %s%s",buf,mount2);
	    else
		sprintf( poly, "$n %s in from %s%s",leave,buf,mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
    }

    do_look( ch, "auto" );

    if ( !IS_CLASS( ch, CLASS_WEREWOLF ) && ( IS_SET( ch->in_room->area->bits,WASTE ) || IS_SET( ch->in_room->area->bits,UPDATEWASTE ) )
         && !( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 ) )
       ch->wait = 12;

    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;
	if ( (mount = fch->mount) != NULL && mount == ch && IS_SET(fch->mounted,IS_MOUNT))
	{
	    act( "$N digs $S heels into you.", fch, NULL, ch, TO_CHAR );
	    char_from_room(fch);
	    char_to_room(fch,ch->in_room);
	}

	if ( fch->master == ch && fch->position == POS_STANDING
		&& fch->in_room != ch->in_room )
	{
	    act( "You follow $N.", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door );
	}
    }

    room_text(ch,">ENTER<");

    check_traps( ch );

    return;
}

bool is_memb( CHAR_DATA *ch )
{
  if ( IS_SET(ch->mflags, MAGE_EMBRACED2) || IS_SET(ch->mflags, MAGE_EMBRACED1) )
     {
        stc("You struggle to move, but the earth surrounding you won't budge.\n\r",ch);
        return TRUE;
     }

  return FALSE;
}

void do_north( CHAR_DATA *ch, char *argument )
{
   ROOM_INDEX_DATA *in_room;

   if ( IS_AFFECTED(ch,AFF_WEBBED) )
      {
	    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	    return;
      }

   if ( IS_SET(ch->more, MORE_NET) )
      {
         send_to_char("You are unable to move while tangled in a spiritual net.\n\r",ch);
  	   return;
      }

   if ( IS_SET(ch->more, MORE_IMPALE) )
      {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
      }

   in_room = ch->in_room;

   if ( IS_SET( ch->more2, MORE2_REVWALK ) )
      move_char( ch, DIR_SOUTH );
      else move_char( ch, DIR_NORTH );

   if ( !IS_NPC(ch) && ch->in_room != in_room )
      {
	   ROOM_INDEX_DATA *old_room;
	   old_room = ch->in_room;
	   char_from_room(ch);
	   char_to_room(ch, in_room);

         if ( IS_SET( ch->more2, MORE2_REVWALK ) )
            add_tracks( ch, DIR_SOUTH );
            else add_tracks( ch, DIR_NORTH );

	   char_from_room(ch);
	   char_to_room(ch, old_room);
      }
}



void do_east( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if ( IS_AFFECTED(ch, AFF_WEBBED) )
    {
	 send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	 return;
    }

    if ( IS_SET(ch->more, MORE_NET) )
    {
       send_to_char("You are unable to move while tangled in a spiritual net.\n\r",ch);
	 return;
    }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
       }

    in_room = ch->in_room;

    if ( IS_SET( ch->more2, MORE2_REVWALK ) )
       move_char( ch, DIR_WEST );
       else move_char( ch, DIR_EAST );

    if ( !IS_NPC(ch) && ch->in_room != in_room )
    {
	 ROOM_INDEX_DATA *old_room;
	 old_room = ch->in_room;
	 char_from_room(ch);
	 char_to_room(ch, in_room);

       if ( IS_SET( ch->more2, MORE2_REVWALK ) )
          add_tracks( ch, DIR_WEST );
          else add_tracks( ch, DIR_EAST );

	 char_from_room(ch);
	 char_to_room(ch, old_room);
    }
}

void do_south( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if (IS_SET(ch->more, MORE_NET))
    {
      send_to_char("You are unable to move while tangled in a spiritual net.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
       }

    in_room = ch->in_room;

    if ( IS_SET( ch->more2, MORE2_REVWALK ) )
       move_char( ch, DIR_NORTH );
       else move_char( ch, DIR_SOUTH );

    if ( !IS_NPC(ch) && ch->in_room != in_room )
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);

      if ( IS_SET( ch->more2, MORE2_REVWALK ) )
         add_tracks( ch, DIR_NORTH );
         else add_tracks( ch, DIR_SOUTH );

	char_from_room(ch);
	char_to_room(ch, old_room);
    }
}


void do_west( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if ( IS_AFFECTED(ch, AFF_WEBBED) )
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if (IS_SET(ch->more, MORE_NET))
    {
      send_to_char("You are unable to move while tangled in a spiritual net.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
       }

    in_room = ch->in_room;

    if ( IS_SET( ch->more2, MORE2_REVWALK ) )
       move_char( ch, DIR_EAST );
       else move_char( ch, DIR_WEST );

    if ( !IS_NPC(ch) && ch->in_room != in_room )
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);

      if ( IS_SET( ch->more2, MORE2_REVWALK ) )
         add_tracks( ch, DIR_EAST );
         else add_tracks( ch, DIR_WEST );

	char_from_room(ch);
	char_to_room(ch, old_room);
    }
}



void do_up( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;
    
    if ( IS_AFFECTED(ch, AFF_WEBBED) )
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if (IS_SET(ch->more, MORE_NET))
    {
      send_to_char("You are unable to move while tangled in a spiritual net.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
       }

    in_room = ch->in_room;

    if ( IS_SET( ch->more2, MORE2_REVWALK ) )
       move_char( ch, DIR_DOWN );
       else move_char( ch, DIR_UP );

    if ( !IS_NPC(ch) && ch->in_room != in_room )
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);

      if ( IS_SET( ch->more2, MORE2_REVWALK ) )
         add_tracks( ch, DIR_DOWN );
         else add_tracks( ch, DIR_UP );

	char_from_room(ch);
	char_to_room(ch, old_room);
    }
}


void do_down( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if ( IS_AFFECTED(ch, AFF_WEBBED) )
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if (IS_SET(ch->more, MORE_NET))
    {
      send_to_char("You are unable to move while tangled in a spiritual net.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
         send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
  	   return;
       }

    in_room = ch->in_room;

    if ( IS_SET( ch->more2, MORE2_REVWALK ) )
       move_char( ch, DIR_UP );
       else move_char( ch, DIR_DOWN );

    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);

      if ( IS_SET( ch->more2, MORE2_REVWALK ) )
         add_tracks( ch, DIR_UP );
         else add_tracks( ch, DIR_DOWN );

	char_from_room(ch);
	char_to_room(ch, old_room);
    }
}

int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}

/* random room generation procedure */
void get_random_room( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *room;

    if ( ( ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000 )
         || ( ch->in_room->vnum >= 3500 && ch->in_room->vnum < 4000 )
         || ( ch->in_room->vnum >= 4500 && ch->in_room->vnum < 5000 )
         || ( ch->in_room->vnum >= 16500 && ch->in_room->vnum < 17000 ) )
       {
          for ( ; ; )
          { 
             room = get_room_index( number_range( ch->in_room->area->lvnum, ch->in_room->area->uvnum ) );
 
             if ( room != NULL )
                break;
          }

          char_from_room( ch );
          char_to_room( ch, room );
          return;
       }

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );

        if ( room == NULL )
           continue;

        if ( room->vnum < 100 || room->vnum == 651 )
           continue;

        if ( ( room->vnum >= 2500 && room->vnum < 3000 )
             || ( room->vnum >= 3500 && room->vnum < 4000 )
             || ( room->vnum >= 4500 && room->vnum < 5000 )
             || ( room->vnum >= 16500 && room->vnum < 17000 ) )
           continue;

        break;
    }

   char_from_room( ch );
   char_to_room( ch, room );
}

ROOM_INDEX_DATA  *get_rand_room()
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL )
        if ( !IS_SET(room->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(room->room_flags, ROOM_SOLITARY) 
	&&   !IS_SET(room->room_flags, ROOM_SAFE) 
	&&  room->vnum > 300)
            break;
    }

    return room;
}

ROOM_INDEX_DATA  *get_treemeld_room()
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL
	    && room->sector_type == SECT_FOREST ) break;
    }

    return room;
}

ROOM_INDEX_DATA  *get_rand_room_by_sect( int sect )
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL
	    && room->sector_type == sect ) break;
    }

    return room;
}

/* Designed for the portal spell, but can also have other uses...KaVir
 * V0 = Where the portal will take you.
 * V1 = Number of uses (0 is infinate).
 * V2 = if 2, cannot be entered.
 * V3 = The room the portal is currently in.
 */
void do_enter( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    CHAR_DATA *mount;
    bool found;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Enter what?\n\r", ch );
	return;
    }

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return;
    }
    if ( obj->item_type != ITEM_PORTAL && obj->item_type != ITEM_WGATE)
    {
	act( "You cannot enter that.", ch, NULL, arg, TO_CHAR );
	return;
    }

	if( obj->item_type == ITEM_PORTAL) {
    if ( obj->value[2] == 2 || obj->value[2] == 3 )
    {
	act( "It seems to be closed.", ch, NULL, arg, TO_CHAR );
	return;
    }

    pRoomIndex = get_room_index(obj->value[0]);
    location = ch->in_room;

    if ( pRoomIndex == NULL )
    {
	act( "You are unable to enter.", ch, NULL, arg, TO_CHAR );
	return;
    }

    act( "You step into $p.", ch, obj, NULL, TO_CHAR );
    act( "$n steps into $p.", ch, obj, NULL, TO_ROOM );

    char_from_room(ch);
    char_to_room(ch,pRoomIndex);

    act( "$n steps out of $p.", ch, obj, NULL, TO_ROOM );
    char_from_room(ch);
    char_to_room(ch,location);

    if (obj->value[1] != 0)
    {
	obj->value[1] = obj->value[1] - 1;
	if (obj->value[1] < 1)
	{
	    act( "$p vanishes.",  ch, obj, NULL, TO_CHAR );
	    act( "$p vanishes.",  ch, obj, NULL, TO_ROOM );
	    extract_obj(obj);
	}
    }
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    found = FALSE;
    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
    {
	portal_next = portal->next_content;
	if ( ( obj->value[0] == portal->value[3]  )
	    && (obj->value[3] == portal->value[0]) )
	{
	    found = TRUE;

    	    if (portal->value[1] != 0)
    	    {
		portal->value[1] = portal->value[1] - 1;
		if (portal->value[1] < 1)
		{
		    act( "$p vanishes.",  ch, portal, NULL, TO_CHAR );
		    act( "$p vanishes.",  ch, portal, NULL, TO_ROOM );
		    extract_obj(portal);
		}
	    }
    	}
    }
    do_look(ch,"auto");
    check_traps( ch );

    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
	}
	else
	{
    pRoomIndex = get_room_index(obj->value[0]);
    location = ch->in_room;

    if ( pRoomIndex == NULL )
    {
	act( "You are unable to enter.", ch, NULL, arg, TO_CHAR );
	return;
    }
    act( "You step into $p.",  ch, obj, NULL, TO_CHAR );
    act( "$n steps into $p.", ch, obj, NULL, TO_ROOM );

    char_from_room(ch);
    char_to_room(ch,pRoomIndex);

    act( "$n steps out of $p.", ch, obj, NULL, TO_ROOM );
    
    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
    {
	portal_next = portal->next_content;
	if ( ( obj->value[0] == portal->value[3]  )
	    && (obj->value[3] == portal->value[0]) )
	{
	    found = TRUE;
    	}
    }
    do_look(ch,"auto");
    check_traps( ch );

    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );

    return;
	}
}


void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_BOOK)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) && obj->item_type != ITEM_BOOK)
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_BOOK)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) && obj->item_type != ITEM_BOOK )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	if (obj->item_type == ITEM_BOOK) obj->value[2] = 0;
	send_to_char( "Ok.\n\r", ch );
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_turn( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Turn <book> <forward/back>.\n\r", ch );
	return;
    }

    if (is_number(arg2)) value = atoi(arg2);

    if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
    {
	send_to_char("You don't have that book.\n\r",ch);
	return;
    }

    if ( obj->item_type != ITEM_BOOK)
    { send_to_char( "That's not a book.\n\r", ch ); return; }
    if ( IS_SET(obj->value[1], CONT_CLOSED) )
    { send_to_char( "First you should open it.\n\r",    ch ); return; }

    if (!str_cmp(arg2,"f") || !str_cmp(arg2,"forward"))
    {
	if (obj->value[2] >= obj->value[3])
	{
	    send_to_char("But you are already at the end of the book.\n\r",ch);
	    return;
	}
	obj->value[2] += 1;
	act( "You flip forward a page in $p.", ch, obj, NULL, TO_CHAR );
	act( "$n flips forward a page in $p.", ch, obj, NULL, TO_ROOM );
    }
    else if (!str_cmp(arg2,"b") || !str_cmp(arg2,"backward"))
    {
	if (obj->value[2] <= 0)
	{
	    send_to_char("But you are already at the beginning of the book.\n\r",ch);
	    return;
	}
	obj->value[2] -= 1;
	act( "You flip backward a page in $p.", ch, obj, NULL, TO_CHAR );
	act( "$n flips backward a page in $p.", ch, obj, NULL, TO_ROOM );
    }
    else if (is_number(arg2) && value >= 0 && value <= obj->value[3])
    {
	if (value == obj->value[2])
	{
	    act("$p is already open at that page.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	else if (value < obj->value[2])
	{
	    act( "You flip backwards through $p.", ch, obj, NULL, TO_CHAR );
	    act( "$n flips backwards through $p.", ch, obj, NULL, TO_ROOM );
	}
	else
	{
	    act( "You flip forwards through $p.", ch, obj, NULL, TO_CHAR );
	    act( "$n flips forwards through $p.", ch, obj, NULL, TO_ROOM );
	}
	obj->value[2] = value;
    }
    else
	send_to_char("Do you wish to turn forward or backward a page?\n\r",ch);
    return;
}

bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_pick_lock] )
    {
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* pick the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You wake and stand up.\n\r", ch );
	act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_RESTING:
    case POS_SITTING:
	send_to_char( "You stand up.\n\r", ch );
	act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_MEDITATING:
	send_to_char( "You uncross your legs and stand up.\n\r", ch );
	act( "$n uncrosses $s legs and stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
    case POS_SITTING:
    case POS_STANDING:
	send_to_char( "You rest.\n\r", ch );
	act( "$n rests.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_RESTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_sit( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You are already sitting.\n\r", ch );
	break;

    case POS_STANDING:
	send_to_char( "You sit down.\n\r", ch );
	act( "$n sits down.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SITTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_meditate( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You cross your legs and start meditating.\n\r", ch );
	act( "$n crosses $s legs and starts meditating.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_STANDING:
	send_to_char( "You sit down, cross your legs and start meditating.\n\r", ch );
	act( "$n sits down, crosses $s legs and starts meditating.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}

void do_healing( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch) || (!IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PROT] < 8))
    {
	send_to_char("You are unable to assume a state of healing.\n\r",ch);
	return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already in a healing position.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You close your eyes and become completely relaxed.\n\r", ch );
	act( "$n closes $m eyes and looks completely relaxed.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_STANDING:
	send_to_char( "You close your eyes and become completely relaxed.\n\r", ch );
	act( "$n closes $m eyes and looks completely relaxed.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}

void do_sleep( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_SITTING:
    case POS_MEDITATING:
    case POS_RESTING:
    case POS_STANDING: 
	send_to_char( "You sleep.\n\r", ch );
	act( "$n sleeps.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SLEEPING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if (victim->position < POS_SLEEPING)
	{ act( "$E doesn't respond!",   ch, NULL, victim, TO_CHAR );  return; }

    act( "You wake $M.", ch, NULL, victim, TO_CHAR );
    act( "$n wakes you.", ch, NULL, victim, TO_VICT );
    victim->position = POS_STANDING;
    return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    send_to_char( "You attempt to move silently.\n\r", ch );
    affect_strip( ch, gsn_sneak );

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_sneak] )
    {
	af.type      = gsn_sneak;
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
    }

    return;
}



void do_hide( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You attempt to hide.\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_hide] )
	SET_BIT(ch->affected_by, AFF_HIDE);

    return;
}



/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    affect_strip ( ch, gsn_invis			);
    affect_strip ( ch, gsn_mass_invis			);
    affect_strip ( ch, gsn_sneak			);
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_unpolymorph( CHAR_DATA *ch, char *argument )
{
    if (!is_affected(ch, gsn_polymorph) )
    {
	send_to_char("But you are not polymorphed!\n\r",ch);
	return;
    }
    act("$n's body begins to distort.",ch,NULL,NULL,TO_ROOM);
    affect_strip ( ch, gsn_polymorph );
    act("$n resumes $s normal form.",ch,NULL,NULL,TO_ROOM);
    send_to_char( "You resume your normal form.\n\r", ch );
    return;
}



void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;
    int cost = 0;

    if ( !IS_NPC(ch) && ch->pcdata->team > 0 )
       {
          stc("You can't recall in the middle of a quest!!!\n\r",ch);
          return;
       }

    if ( armageddon == TRUE )
       {
	    send_to_char("You can't recall while the world is in #rC#eh#Ra#eo#rs#n!!\n\r", ch );
	    return;
       }

    if ( !IS_NPC(ch) && ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 ) 
       {
          send_to_char( "Not in the arena.\n\r", ch );
          return;
       }

    if ( IS_CLASS( ch, CLASS_DRAGOON ) && ch->fight_timer > 0 )
       {
          send_to_char("You tell yourself in anger '#yI am no $%^$ wuss!#n' as you continue the attack.\n\r",ch);
          return;
       } 

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char( "As hard as you try, there is no way to escape #Ct#yh#Ri#Ls #evortex#n.\n\r", ch );
          return;
       }

    if ( ch->fight_timer > 0 )
       cost = ch->max_move / 5;

    if ( ch->move < cost )
       {
          send_to_char("You can't find the energy to return to the sanctuary.\n\r",ch);
          return;
       }

    if ( is_affected(ch, skill_lookup("curse of allah") ) )
       {
           act( "#e$n#e's body flickers with black energy.#n", ch, NULL, NULL, TO_ROOM );
           act( "#eYour body flickers with black energy.#n", ch, NULL, NULL, TO_CHAR );
           send_to_char( "#yGod #wrefuse to make you use this #Pmystical ability#w.#n\n\r", ch );
           ch->wait = 18;
           return;
       }

    act( "$n's body flickers with green energy.", ch, NULL, NULL, TO_ROOM );
    act( "Your body flickers with green energy.", ch, NULL, NULL, TO_CHAR );

    if ( ch->fight_timer > 0 )
       location = get_room_index( 800 );
       else location = get_room_index( ch->home );

    if ( in_kingdom( ch, NULL, location ) )
       return;

    if ( location == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "You are unable to recall.\n\r", ch );
	return;
    }
	
    if ( ( victim = ch->fighting ) != NULL )
    {
	if ( number_range( 1, 2 ) == 1 )
	{
          ch->wait = 12;
	    sprintf( buf, "You failed!\n\r" );
	    send_to_char( buf, ch );
	    return;
	}
	sprintf( buf, "You recall from combat!\n\r");
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
    }

    ch->move = UMAX( 0, ch->move - cost );

    if ( ch->fight_timer > 0 )
       ch->agg = 100;

    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );

    if ( ch->fight_timer > 0 && IS_SET(ch->act, PLR_WIZINVIS) )
       REMOVE_BIT(ch->act, PLR_WIZINVIS);

    if ( ch->fight_timer > 0 && IS_SET(ch->newbits, NEW_DARKNESS) ) 
       {
          send_to_char("You banish your cloak of darkness.\n\r", ch );
          REMOVE_BIT(ch->newbits, NEW_DARKNESS);
          act("The cloak of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);

          if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) && ch->in_room->drows == 1 )
	       REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);

          ch->in_room->drows--;
       }

    if ( is_affected( ch, skill_lookup("last prayer") ) )
       ch->wait = 36;

    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
}

void do_home( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' || str_cmp(arg,"here") )
    {
	send_to_char( "If you wish this to be your room, you must type 'home here'.\n\r", ch );
	return;
    }

    if ( ch->in_room->vnum >= 33200 )
    {
	send_to_char( "#yYou #Ccan't make your #whome #Chere!#n\n\r", ch );
	return;
    }

    if ( ch->in_room->vnum == ch->home )
    {
	send_to_char( "But this is already your home!\n\r", ch );
	return;
    }

    ch->home = ch->in_room->vnum;
    send_to_char( "This room is now your home.\n\r", ch );

    return;
}


void do_escape( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if ( IS_NPC(ch) )
       return;
  
    if ( ch->position >= POS_SLEEPING )
       {
          send_to_char( "You can only do this if you are dying.\n\r", ch );
          return;
       }

    if ( !IS_NPC(ch) && ch->pcdata->team > 0 )
       {
          stc("You can't recall in the middle of a quest!!!\n\r",ch);
          return;
       }

    if ( ch->fight_timer > 0 )
       {
          stc("Not with an active fight timer!\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags,ROOM_ARENA) )
       {
          send_to_char( "Not in the arena.\n\r", ch );
          return;
       }

    if ( ( location = get_room_index( 800 ) ) == NULL )
       {
          send_to_char( "You are completely lost.\n\r", ch );
          return;
       }

    if ( ch->in_room == location )
	return;

    ch->move = 0;
    ch->mana = 0;
    act( "$n fades out of existance.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n fades into existance.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    sprintf(buf,"%s has escaped defenceless from a fight.",ch->pcdata->switchname);
    do_info(ch,buf);

    if ( IS_SET(ch->newbits, NEW_DARKNESS) )
       {
           REMOVE_BIT(ch->newbits, NEW_DARKNESS);

           if ( ch->in_room != NULL )
              if ( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
                 REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
       }
}


int disc_points_needed(CHAR_DATA *ch)
{
    
    if (IS_NPC(ch)) return 1;

    return (ch->power[ch->pcdata->disc_research] + 1) * 10;
}

void gain_disc_points(CHAR_DATA *ch, int points)
{   
    if (IS_NPC(ch)) return;

    if (ch->pcdata->disc_points == 666) return;
    if (ch->pcdata->disc_research == -1) return;

    ch->pcdata->disc_points += points;

    if (ch->pcdata->disc_points < 0)
    {
	ch->pcdata->disc_points = 0;
	return;
    }

    if (ch->pcdata->disc_points >= 
	    disc_points_needed(ch) )
    {
	ch->pcdata->disc_points = 999;
	WAIT_STATE(ch, 6);
	stc("#4You have finished researching your discipline.\n\r", ch);
	stc("You may now use the 'train' command.#n\n\r", ch);
	return;
    }
}

const  char * wwgift [MAX_GIFTS] =
{
	"",
	"Homid",		"Metis",
	"Lupus",		"Ragabash",
	"Theurge",		"Philodox",
	"Galliard",		"Ahroun",
	"Black Furies",		"Bone Gnawers",
	"Children of Gaia",	"Fianna",
	"Get of Fenris",	"Glass Walkers",
	"Red Talons",		"Shadow Lords",
	"Silent Striders",	"Silver Fangs",
	"Stargazers",		"Uktena",
	"Wendigo"
};

const  char * discipline [MAX_DISCIPLINES] =
{
	  "",			"",
        "celerity",		"fortitude",
        "obtenebration",	"presence",
        "quietus",		"thaumaturgy",
        "auspex",		"dominate",
	"obfuscate",	        "potence",
	"protean",	        "serpentis",
	"vicissitude",          "daimoinon",	    
        "animalism",		"",

/* Werewolf */

	"bear",			"lynx",
        "boar",			"owl",
        "spider",		"wolf",
	"hawk",			"mantis",
	"raptor",	        "luna",
	"pain",			"congregation",

/* Daemon */
	"hellfire",		"attack",
	"temptation",		"morphosis",
	"corruption",		"geluge",
	"discord",		"nether",
	"immunae",		"chimerstry",
        "thanatosis",           "obeah",
	"necromancy",    	"melpominee"

};

void do_disciplines(CHAR_DATA *ch, char *argument)    
{
    char buf[MAX_INPUT_LENGTH];
    char buf2[MAX_INPUT_LENGTH];
    int loop;
    int percent = 0, i;
    int indent = 0;

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DEMON))
    {
      send_to_char("Huh.\n\r",ch);
      return;
    }

    if (IS_CLASS(ch, CLASS_WEREWOLF))
        sprintf(buf, "---===[ Guardians ]===---");
    else
        sprintf(buf, "---===[ Disciplines ]===---");


    cent_to_char(buf, ch);
    send_to_char("\n\r", ch);

    for (loop = 0; loop < MAX_DISCIPLINES; loop++)
    {
	if (discipline[loop][0] != '\0'
	&& strlen(discipline[loop]) != 0
	&& ch->power[loop] >= 0 )
	{
	    sprintf(buf2, discipline[loop]);
	    buf2[0] = UPPER(buf2[0]);

	    sprintf(buf,"     %-14s: %-2d",
		buf2,
		ch->power[loop]);
	    send_to_char(buf,ch);
	    indent++;
	    if (indent == 3)
	    {
		stc("\n\r", ch);
		indent = 0;
	    }
	}
    }
    if (indent != 0) stc("\n\r", ch);

    /* This tells them if they are currently researching */
    if (ch->pcdata->disc_research < 0)
    {
	send_to_char("\n\r", ch);
	divide6_to_char(ch);
	return;
    }

    if (ch->power[ch->pcdata->disc_research] < 0)
    {
	send_to_char("\n\r", ch);
	divide6_to_char(ch);
	return;
    }

    if (ch->pcdata->disc_points == 999)
    {
	stc("\n\r", ch);
	sprintf(buf, "You have finished researching %s.",
	    discipline[ch->pcdata->disc_research]);
	cent_to_char(buf, ch);
	stc("\n\r", ch);
	divide6_to_char(ch);
	return;
    }

    if (ch->pcdata->disc_research < MAX_DISCIPLINES)
    {
	send_to_char("\n\r", ch);
	sprintf(buf, "You are currently researching %s.",
	    discipline[ch->pcdata->disc_research]);
    	cent_to_char(buf, ch);
    }

    percent = ch->pcdata->disc_points * 40 / disc_points_needed(ch);

    sprintf(buf, "-[#1");

    for( i = 0 ; i < percent ; i++)
	strcat(buf, "*");

    for( i = percent ; i < 40 ; i++)
	strcat(buf, " ");

    strcat(buf, "#n]-");

    cent_to_char(buf, ch);
}

void do_relearn(CHAR_DATA *ch,char *argument )
{
	char arg1[MAX_STRING_LENGTH];
        argument = one_argument( argument, arg1 );
	
      if (IS_NPC(ch)) return;
/*
	if ( arg1[0] == '\0' )
	{
		send_to_char("You are currently resistant to:\n\r",ch);
		
		int immune_counter = 0;
		if (IS_IMMUNE(ch,IMM_SLASH))
		{
			immune_counter+=1;
			send_to_char("Slash & Slice.\n",ch);
		}
	if (IS_IMMUNE(ch,IMM_STAB))
		{
			immune_counter+=1;
			send_to_char("Stab & Pierce.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_SMASH))
		{
			immune_counter+=1;
			send_to_char("Pound, Blast & Crush.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_ANIMAL))
		{
			immune_counter+=1;
			send_to_char("Claw & Bite.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_MISC))
{
			immune_counter+=1;
			send_to_char("Grep, Whip & Suck.\n",ch);
		}
if (immune_counter==0)
		send_to_char("None.\n\r",ch);
immune_counter=0;
send_to_char("You are immune to:\n\r",ch);
if (IS_IMMUNE(ch,IMM_CHARM))
		{
			immune_counter+=1;
			send_to_char("Charm.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_HEAT))
		{
			immune_counter+=1;
			send_to_char("Heat.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_COLD))
		{
			immune_counter+=1;
			send_to_char("Cold.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_LIGHTNING))
		{
			immune_counter+=1;
			send_to_char("Lightning.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_ACID))
		{
			immune_counter+=1;
			send_to_char("Acid.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_DRAIN))
		{
			immune_counter+=1;
			send_to_char("Drain.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_VOODOO))
		{
			immune_counter+=1;
			send_to_char("Voodoo.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_HURL))
		{
			immune_counter+=1;
			send_to_char("Hurl.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_BACKSTAB))
		{
			immune_counter+=1;
			send_to_char("Backstab.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_DISARM))
		{
			immune_counter+=1;
			send_to_char("Disarm.\n",ch);
		}
if (IS_IMMUNE(ch,IMM_STEAL))
		{
			immune_counter+=1;
			send_to_char("Steal.\n",ch);
		}
	if (immune_counter==0)
		send_to_char("None.\n\r",ch);

	return;
	}

*/
	if (!str_cmp(arg1,"slash") || !str_cmp(arg1,"slice"))
	{
        if (IS_IMMUNE(ch,IMM_SLASH))
	{
		REMOVE_BIT(ch->immune, IMM_SLASH);
		send_to_char("You forget Slash & Slice resistances.\n\r",ch); return;
	}
        }
	if (!strcmp(arg1,"stab") || !str_cmp(arg1,"pierce"))
        {
	if (IS_IMMUNE(ch,IMM_STAB))
	{
		REMOVE_BIT(ch->immune, IMM_STAB);
		send_to_char("You forget Stab & Pierce resistances.\n\r",ch); return;
	}
        }
        if (!strcmp(arg1,"smash") || !str_cmp(arg1,"pound") || !str_cmp(arg1,"blast") || !str_cmp(arg1,"crush"))
        {
        if (IS_IMMUNE(ch,IMM_SMASH))
        {
                REMOVE_BIT(ch->immune, IMM_SMASH);
                send_to_char("You forget Pound, Blast & Crush resistances.\n\r",ch); return;
        }
        }
         if (!strcmp(arg1,"beast") || !str_cmp(arg1,"claw") || !str_cmp(arg1,"bite"))
        {
        if (IS_IMMUNE(ch,IMM_ANIMAL))
        {
                REMOVE_BIT(ch->immune, IMM_ANIMAL);
                send_to_char("You forget Claw & Bite resistances.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"grab") || !str_cmp(arg1,"grep") || !str_cmp(arg1,"whip") || !str_cmp(arg1,"suck"))
        {
        if (IS_IMMUNE(ch,IMM_MISC))
        {
                REMOVE_BIT(ch->immune, IMM_MISC);
                send_to_char("You forget Grep, Whip & Suck resistances.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"charm"))
        {
        if (IS_IMMUNE(ch,IMM_CHARM))
        {
                REMOVE_BIT(ch->immune, IMM_CHARM);
                send_to_char("You forget Charm immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"heat"))
        {
        if (IS_IMMUNE(ch,IMM_HEAT))
        {
                REMOVE_BIT(ch->immune, IMM_HEAT);
                send_to_char("You forget Heat immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"cold"))
        {
        if (IS_IMMUNE(ch,IMM_COLD))
        {
                REMOVE_BIT(ch->immune, IMM_COLD);
                send_to_char("You forget Cold immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"lightning"))
        {
        if (IS_IMMUNE(ch,IMM_LIGHTNING))
        {
                REMOVE_BIT(ch->immune, IMM_LIGHTNING);
                send_to_char("You forget Lightning immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"acid"))
        {
        if (IS_IMMUNE(ch,IMM_ACID))
        {
                REMOVE_BIT(ch->immune, IMM_ACID);
                send_to_char("You forget Acid immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"cold"))
        {
        if (IS_IMMUNE(ch,IMM_COLD))
        {
                REMOVE_BIT(ch->immune, IMM_COLD);
                send_to_char("You forget Cold immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"drain"))
        {
        if (IS_IMMUNE(ch,IMM_DRAIN))
        {
                REMOVE_BIT(ch->immune, IMM_DRAIN);
                send_to_char("You forget Drain immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"voodoo"))
        {
        if (IS_IMMUNE(ch,IMM_VOODOO))
        {
                REMOVE_BIT(ch->immune, IMM_VOODOO);
                send_to_char("You forget Voodoo immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"hurl"))
        {
        if (IS_IMMUNE(ch,IMM_HURL))
        {
                REMOVE_BIT(ch->immune, IMM_HURL);
                send_to_char("You forget Hurl immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"backstab"))
        {
        if (IS_IMMUNE(ch,IMM_BACKSTAB))
        {
                REMOVE_BIT(ch->immune, IMM_BACKSTAB);
                send_to_char("You forget Backstab immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"kick"))
        {
        if (IS_IMMUNE(ch,IMM_KICK))
        {
                REMOVE_BIT(ch->immune, IMM_KICK);
                send_to_char("You forget Kick immunity.\n\r",ch); return;
        }
        }
if (!strcmp(arg1,"disarm"))
        {
        if (IS_IMMUNE(ch,IMM_DISARM))
        {
                REMOVE_BIT(ch->immune, IMM_DISARM);
                send_to_char("You forget Disarm immunity.\n\r",ch);
                return;
        }
        }
if (!strcmp(arg1,"steal"))
        {
        if (IS_IMMUNE(ch,IMM_STEAL))
        {
                REMOVE_BIT(ch->immune, IMM_STEAL);
                send_to_char("You forget Steal immunity.\n\r",ch);
                return;
        }
        }
else               
    {
    send_to_char("Command: relearn an already learned immunity/resistance.\n\r", ch);
    return;
    }
}

void do_train( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int amount = 0, min_stat;

    if ( IS_NPC( ch ) )
       return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( !str_cmp(arg1,"hp") )
    {
	int urin = 0;
	int urin_counter = 0;
      int cost = 100;
      int max = 10000000;

      if ( IS_SET( ch->garou1, GAROU_WHELP2 ) || IS_SET( ch->garou1, GAROU_WHELP1 ) )
         {
            send_to_char("#PYou are under the influence of the Whelp's curse.#n\n\r",ch);
            return;
         }

      if ( is_affected( ch, skill_lookup("weaken") ) )
         {
            stc( "#PYou can't train stats while weakened.#n\n\r", ch );
            return;
         }

      if ( ch->max_hit > max )
         {
            send_to_char("You've reached the statcap.\n\r",ch);
            return;
         }
         
	if ( !is_number(arg2) && strcmp(arg2,"all") )
  	   {
	      send_to_char("Please enter a numeric value.\n\r",ch);
	      return;
	   }

	if ( is_number(arg2) )
	   {
	      amount = atoi( arg2 );

 	      if ( amount < 1 || amount > 1000000 )
	         {
	            send_to_char("Please enter a value between 1 and 1000000.\n\r",ch);
	            return;
	         }
	   }
	   else amount = 1000000;

        if ( amount > ch->max_hit - ch->hit )
           amount -= ch->max_hit - ch->hit;

	for ( urin = 0; urin < amount ; urin++ )
	{
	         ch->max_hit +=1;
	         urin_counter +=1;

           if ( ch->max_hit >= max )
              break;  
        }	

	if ( urin_counter==0 )
	{
	    send_to_char( "You need more exp to gain any hps.\n\r", ch );
	}

	if ( urin_counter == 1 )
	{
	    send_to_char( "You gain 1 hp.\n\r", ch );
	}

	if ( urin_counter > 1 )
	{
	    sprintf( buf, "You gain %d hps.\n\r", urin_counter );
	    send_to_char( buf, ch );
	}

        return;
    }

    if ( !str_cmp(arg1,"mana") )
    {
	int urin = 0;
	int urin_counter = 0;
      int cost = 100;
      int max = 10000000;

      if ( IS_SET( ch->garou1, GAROU_WHELP2 ) || IS_SET( ch->garou1, GAROU_WHELP1 ) )
         {
            send_to_char("#PYou are under the influence of the Whelp's curse.#n\n\r",ch);
            return;
         }

      if ( is_affected( ch, skill_lookup("weaken") ) )
         {
            stc( "#PYou can't train stats while weakened.#n\n\r", ch );
            return;
         }

      if ( ch->max_mana > max )
         {
            send_to_char("You've reached the statcap.\n\r",ch);
            return;
         }
         
	if ( !is_number(arg2) && strcmp(arg2,"all") )
  	   {
	      send_to_char("Please enter a numeric value.\n\r",ch);
	      return;
	   }

	if ( is_number(arg2) )
	   {
	      amount = atoi( arg2 );

 	      if ( amount < 1 || amount > 1000000 )
	         {
	            send_to_char("Please enter a value between 1 and 1000000.\n\r",ch);
	            return;
	         }
	   }
	   else amount = 1000000;

        if ( amount > ch->max_mana - ch->mana )
           amount -= ch->max_mana - ch->mana;

	for ( urin = 0; urin < amount ; urin++ )
	{
	         ch->max_mana +=1;
	         urin_counter +=1;

           if ( ch->max_mana > max )
              break;  
        }	

	if ( urin_counter==0 )
	{
	    send_to_char( "You need more exp to gain any mana points.\n\r", ch );
	}

	if ( urin_counter == 1 )
	{
	    send_to_char( "You gain 1 mana.\n\r", ch );
	}

	if ( urin_counter > 1 )
	{
	    sprintf( buf, "You gain %d mana points.\n\r", urin_counter );
	    send_to_char( buf, ch );
	}

        return;
    }

    if ( !str_cmp(arg1,"move") )
    {
	int urin = 0;
	int urin_counter = 0;
      int cost = 100;
      int max = 10000000;

      if ( IS_SET( ch->garou1, GAROU_WHELP2 ) || IS_SET( ch->garou1, GAROU_WHELP1 ) )
         {
            send_to_char("#PYou are under the influence of the Whelp's curse.#n\n\r",ch);
            return;
         }

      if ( is_affected( ch, skill_lookup("weaken") ) )
         {
            stc( "#PYou can't train stats while weakened.#n\n\r", ch );
            return;
         }

      if ( ch->max_move > max )
         {
            send_to_char("You've reached the statcap.\n\r",ch);
            return;
         }
         
	if ( !is_number(arg2) && strcmp(arg2,"all") )
  	   {
	      send_to_char("Please enter a numeric value.\n\r",ch);
	      return;
	   }

	if ( is_number(arg2) )
	   {
	      amount = atoi( arg2 );

 	      if ( amount < 1 || amount > 1000000 )
	         {
	            send_to_char("Please enter a value between 1 and 1000000.\n\r",ch);
	            return;
	         }
	   }
	   else amount = 1000000;

        if ( amount > ch->max_move - ch->move )
           amount -= ch->max_move - ch->move;

	for ( urin = 0; urin < amount ; urin++ )
	{
	         ch->max_move +=1;
	         urin_counter +=1;

           if ( ch->max_move > max )
              break;  
        }	

	if ( urin_counter==0 )
	{
	    send_to_char( "You need more exp to gain any move points.\n\r", ch );
	}

	if ( urin_counter == 1 )
	{
	    send_to_char( "You gain 1 move.\n\r", ch );
	}

	if ( urin_counter > 1 )
	{
	    sprintf( buf, "You gain %d move points.\n\r", urin_counter );
	    send_to_char( buf, ch );
	}

        return;
    }

    min_stat = UMIN( ch->pcdata->perm_eva, UMIN( ch->pcdata->perm_dex, UMIN( ch->pcdata->perm_wis, UMIN( ch->pcdata->perm_con, UMIN( ch->pcdata->perm_str, ch->pcdata->perm_int ) ) ) ) );

    if ( !str_cmp(arg1,"str") )
       {
         if ( ch->pcdata->perm_str >= 99 )
            {
               stc( "You can't train this anymore as it is at max.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_str > min_stat + 24 )
            {
               stc( "You can't train this anymore until you train up your lowest stat.\n\r", ch );
               return;
            }

         if ( !is_number(arg2) && strcmp(arg2,"all") )
            {
	         send_to_char("Please enter a numeric value.\n\r",ch);
	         return;
            }

         if ( is_number(arg2) )
            {
	         amount = atoi( arg2 );

 	         if ( amount < 1 || amount > 99 )
                  {
                     send_to_char("Please enter a value between 1 and 99.\n\r",ch);
	               return;
	            }

               ( amount + ch->pcdata->perm_str > 99 ) ? 99 - ch->pcdata->perm_str : amount;
            }
            else amount = 99 - ch->pcdata->perm_str;


         if ( ch->practice < amount )
            {
               stc( "You don't have enough #ytraining points#n to train this attribute.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_str + amount > min_stat + 25 )
            amount -= ( amount + ch->pcdata->perm_str - min_stat - 25 );

         ch->pcdata->perm_str += amount;
         ch->practice -= amount;
         stc( "#wOk#e.#n\n\r", ch );
         return;
       }

    if ( !str_cmp(arg1,"min") )
       {
         if ( ch->pcdata->perm_int >= 99 )
            {
               stc( "You can't train this anymore as it is at max.\n\r", ch );
               return;
            }

         if ( !is_number(arg2) && strcmp(arg2,"all") )
            {
	         send_to_char("Please enter a numeric value.\n\r",ch);
	         return;
            }

         if ( is_number(arg2) )
            {
	         amount = atoi( arg2 );

 	         if ( amount < 1 || amount > 99 )
                  {
                     send_to_char("Please enter a value between 1 and 99.\n\r",ch);
	               return;
	            }

               ( amount + ch->pcdata->perm_int > 99 ) ? 99 - ch->pcdata->perm_int : amount;
            }
            else amount = 99 - ch->pcdata->perm_int;


         if ( ch->practice < amount )
            {
               stc( "You don't have enough #ytraining points#n to train this attribute.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_int + amount > min_stat + 25 )
            amount -= ( amount + ch->pcdata->perm_int - min_stat - 25 );

         ch->pcdata->perm_int += amount;
         ch->practice -= amount;
         stc( "#wOk#e.#n\n\r", ch );
         return;
       }

    if ( !str_cmp(arg1,"spi") )
       {
         if ( ch->pcdata->perm_wis >= 99 )
            {
               stc( "You can't train this anymore as it is at max.\n\r", ch );
               return;
            }

         if ( !is_number(arg2) && strcmp(arg2,"all") )
            {
	         send_to_char("Please enter a numeric value.\n\r",ch);
	         return;
            }

         if ( is_number(arg2) )
            {
	         amount = atoi( arg2 );

 	         if ( amount < 1 || amount > 99 )
                  {
                     send_to_char("Please enter a value between 1 and 99.\n\r",ch);
	               return;
	            }

               ( amount + ch->pcdata->perm_wis > 99 ) ? 99 - ch->pcdata->perm_wis : amount;
            }
            else amount = 99 - ch->pcdata->perm_wis;


         if ( ch->practice < amount )
            {
               stc( "You don't have enough #ytraining points#n to train this attribute.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_wis + amount > min_stat + 25 )
            amount -= ( amount + ch->pcdata->perm_wis - min_stat - 25 );

         ch->pcdata->perm_wis += amount;
         ch->practice -= amount;
         stc( "#wOk#e.#n\n\r", ch );
         return;
       }

    if ( !str_cmp(arg1,"dex") )
       {
         if ( ch->pcdata->perm_dex >= 99 )
            {
               stc( "You can't train this anymore as it is at max.\n\r", ch );
               return;
            }

         if ( !is_number(arg2) && strcmp(arg2,"all") )
            {
	         send_to_char("Please enter a numeric value.\n\r",ch);
	         return;
            }

         if ( is_number(arg2) )
            {
	         amount = atoi( arg2 );

 	         if ( amount < 1 || amount > 99 )
                  {
                     send_to_char("Please enter a value between 1 and 99.\n\r",ch);
	               return;
	            }

               ( amount + ch->pcdata->perm_dex > 99 ) ? 99 - ch->pcdata->perm_dex : amount;
            }
            else amount = 99 - ch->pcdata->perm_dex;


         if ( ch->practice < amount )
            {
               stc( "You don't have enough #ytraining points#n to train this attribute.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_dex + amount > min_stat + 25 )
            amount -= ( amount + ch->pcdata->perm_dex - min_stat - 25 );

         ch->pcdata->perm_dex += amount;
         ch->practice -= amount;
         stc( "#wOk#e.#n\n\r", ch );
         return;
       }

    if ( !str_cmp(arg1,"vit") )
       {
         if ( ch->pcdata->perm_con >= 99 )
            {
               stc( "You can't train this anymore as it is at max.\n\r", ch );
               return;
            }

         if ( !is_number(arg2) && strcmp(arg2,"all") )
            {
	         send_to_char("Please enter a numeric value.\n\r",ch);
	         return;
            }

         if ( is_number(arg2) )
            {
	         amount = atoi( arg2 );

 	         if ( amount < 1 || amount > 99 )
                  {
                     send_to_char("Please enter a value between 1 and 99.\n\r",ch);
	               return;
	            }

               ( amount + ch->pcdata->perm_con > 99 ) ? 99 - ch->pcdata->perm_con : amount;
            }
            else amount = 99 - ch->pcdata->perm_con;


         if ( ch->practice < amount )
            {
               stc( "You don't have enough #ytraining points#n to train this attribute.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_con + amount > min_stat + 25 )
            amount -= ( amount + ch->pcdata->perm_con - min_stat - 25 );

         ch->pcdata->perm_con += amount;
         ch->practice -= amount;
         stc( "#wOk#e.#n\n\r", ch );
         return;
       }

    if ( !str_cmp(arg1,"eva") )
       {
         if ( ch->pcdata->perm_eva >= 99 )
            {
               stc( "You can't train this anymore as it is at max.\n\r", ch );
               return;
            }

         if ( !is_number(arg2) && strcmp(arg2,"all") )
            {
	         send_to_char("Please enter a numeric value.\n\r",ch);
	         return;
            }

         if ( is_number(arg2) )
            {
	         amount = atoi( arg2 );

 	         if ( amount < 1 || amount > 99 )
                  {
                     send_to_char("Please enter a value between 1 and 99.\n\r",ch);
	               return;
	            }

               ( amount + ch->pcdata->perm_eva > 99 ) ? 99 - ch->pcdata->perm_eva : amount;
            }
            else amount = 99 - ch->pcdata->perm_eva;


         if ( ch->practice < amount )
            {
               stc( "You don't have enough #ytraining points#n to train this attribute.\n\r", ch );
               return;
            }

         if ( ch->pcdata->perm_eva + amount > min_stat + 25 )
            amount -= ( amount + ch->pcdata->perm_eva - min_stat - 25 );

         ch->pcdata->perm_eva += amount;
         ch->practice -= amount;
         stc( "#wOk#e.#n\n\r", ch );
         return;
       }

    sprintf( buf, "#wYou have #C%d #wtraining points and can #ytrain #wthe following:\n\r", ch->practice );
    send_to_char( buf, ch );

    sprintf( buf, "#oHp               - #w1 #gmill per level.\n\r" );
    send_to_char( buf, ch );

    sprintf( buf, "#oMana             - #w1 #gmill per level.\n\r" );
    send_to_char( buf, ch );

    sprintf( buf, "#oMove             - #w1 #gmill per level.\n\r" );
    send_to_char( buf, ch );

    send_to_char( "#oStats            -", ch );

    if ( ch->pcdata->perm_str < 99 ) send_to_char( " #RStr", ch );
    if ( ch->pcdata->perm_int < 99 ) send_to_char( " #CMin", ch );
    if ( ch->pcdata->perm_wis < 99 ) send_to_char( " #wSpi", ch );
    if ( ch->pcdata->perm_dex < 99 ) send_to_char( " #eDex", ch );
    if ( ch->pcdata->perm_con < 99 ) send_to_char( " #yVit", ch );
    if ( ch->pcdata->perm_eva < 99 ) send_to_char( " #PEva", ch );

    if ( ( ch->pcdata->perm_str >= 99 ) && ( ch->pcdata->perm_wis >= 99 )
         && ( ch->pcdata->perm_int >= 99 ) && ( ch->pcdata->perm_dex >= 99 )
         && ( ch->pcdata->perm_con >= 99 ) && ( ch->pcdata->perm_eva >= 99 ) )
       send_to_char( " #wNone #gleft to train.\n\r", ch );
       else send_to_char( "#n.\n\r", ch );
}

void do_mount( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Mount what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_VAMPAFF(ch, VAM_DISGUISED) 
         && !IS_CLASS(ch, CLASS_PALADIN) && !IS_CLASS(ch, CLASS_DROW)
         && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_DAYWALKER) )
    {
	send_to_char( "You cannot ride in this form.\n\r",ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r",ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot ride on your own back!\n\r",ch );
	return;
    }

    if ( ch->mounted > 0 )
    {
	send_to_char( "You are already riding.\n\r",ch );
	return;
    }

    if( !IS_NPC(victim) )
    {
	send_to_char( "You cannot mount them.\n\r",ch );
	return;
    }

    if ( IS_NPC(victim) && victim->pIndexData->vnum == 14 )
    {
       if ( !IS_CLASS(ch, CLASS_DROW) && victim->master != ch )
          {
             stc("Only drow clerics can mount these hideous beasts.\n\r",ch);
             return;
          }
    }

    if ( IS_NPC(victim) && victim->pIndexData->vnum == 16 )
    {
       if ( !IS_CLASS(ch, CLASS_DEMON) && victim->master != ch )
          {
             stc("Only hell spawns can mount these hideous beasts.\n\r",ch);
             return;
          }
    }

    if( victim->mounted > 0 )
    {
	send_to_char( "You cannot mount them.\n\r",ch );
	return;
    }
    if( IS_NPC(victim) && !IS_SET(victim->act, ACT_MOUNT) && !IS_IMMORTAL(ch) )
    {
	send_to_char( "You cannot mount them.\n\r",ch );
	return;
    }
  
    if ( victim->position < POS_STANDING )
    {
	if ( victim->position < POS_SLEEPING )
	    act( "$N is too badly hurt for that.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SLEEPING )
	    act( "First you better wake $m up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_RESTING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_MEDITATING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SITTING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SLEEPING )
	    act( "First you better wake $m up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_FIGHTING )
	    act( "Not while $e's fighting.", ch, NULL, victim, TO_CHAR );
	return;
    }

    ch->mounted     = IS_RIDING;
    victim->mounted = IS_MOUNT;
    ch->mount = victim;
    victim->mount = ch;

    act( "You clamber onto $N's back.", ch, NULL, victim, TO_CHAR );
    act( "$n clambers onto $N's back.", ch, NULL, victim, TO_ROOM );
    return;
}

void do_dismount( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
if (IS_SET(ch->mounted,IS_RIDING))
{ 
if ((victim = ch->mount) == NULL)
{ ch->mounted = 0;send_to_char("You stop riding the air.\n\r",ch);return;}
}
    if ( ch->mounted == 0 )
    {
	send_to_char( "But you are not riding!\n\r",ch );
	return;
    }

    if ( (victim = ch->mount) == NULL )
    {
	send_to_char( "But you are not riding!\n\r",ch );
	return;
    }

    act( "You clamber off $N's back.", ch, NULL, victim, TO_CHAR );
    act( "$n clambers off $N's back.", ch, NULL, victim, TO_ROOM );
    act( "Its task done, $n quickly trots into the distance.", victim, NULL, NULL, TO_ROOM );

    ch->mounted     = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;

    ch->mount = NULL;
    victim->mount = NULL;
    extract_char( victim, TRUE );
}

void check_survival( CHAR_DATA *ch )
{
   int count = 0;
   CHAR_DATA *gch;
   char buf [MAX_INPUT_LENGTH];

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
      if ( IS_NPC(gch) )
         continue;

      if ( gch->pcdata->team < 9 )
          continue;
 
      count++;
   }

   if ( count == 1 )
      {
          count = ch->level;
          ch->level = 12;
          ch->gold = UMIN( 500000000, ch->gold + 1000000 );
          ch->practice += number_range( 1, 3 );

          if ( !( ch->generation == 1 && ch->tier == 5 ) )
             {
                ch->generation -= 3;

                if ( ch->generation < 1 && ch->tier == 5 )
                   ch->generation = 1;
                   else if ( ch->generation < 1 )
                   {
                      stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",ch);
                      ch->generation = 5 + ch->generation;
                      ch->tier++;
                      ch->max_hit += 1000000;
                      ch->max_mana += 1000000;
                      ch->max_move += 1000000;
                   }
             }

          sprintf(buf,"#y%s #ghas #osurvived #gthe #p#zSlAuGhTeR#n#g!#n", ch->pcdata->switchname );
          do_info(ch,buf);
          stc("By some miracle... you had won the battle.\n\r",ch);

          do_ftag(ch,"reset");
          ch->level = count;
          restore_char( ch );
      }
}

void do_tie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *gch;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( (get_age(ch)-17) < 2 )
    {
       stc("But you are a newbie.\n\r",ch);
       return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim))
    {
        send_to_char( "You cannot tie a mob up!\n\r", ch );
        return;
    }

    if (victim == ch)
    {
	 send_to_char( "You cannot tie yourself up!\n\r", ch );
	 return;
    }

    if ( (get_age(victim)-17) < 2 )
    {
       stc("But they are a newbie.\n\r",ch);
       return;
    }

    if ( IS_EXTRA(victim,TIED_UP) )
    {
	 send_to_char( "But they are already tied up!\n\r", ch );
	 return;
    }

    if ( victim->position > POS_STUNNED || victim->hit > 0 )
    {
	 send_to_char( "You can only tie up a defenceless person.\n\r", ch );
	 return;
    }

    act("You quickly tie up $N.",ch,NULL,victim,TO_CHAR);
    act("$n quickly ties up $N.",ch,NULL,victim,TO_ROOM);
    send_to_char("You have been tied up!\n\r",victim);

    if ( ch->pcdata->team > 0 && victim->pcdata->team > 0 )
       {
            int count, team[8];

            for ( count = 0; count < 8; count++ )
            {
                team[count] = 0;
            }

            count = 0;

           sprintf(buf,"#y%s #ghas #wbeen #p#zElImINaTeD#n#y!!!#n",victim->pcdata->switchname);
           do_info(victim,buf);
           restore_char( victim );
           victim->position = POS_STANDING;
           ch->fight_timer = 20;
           victim->fight_timer = 0;
           victim->safe_timer = 5;
           char_from_room(victim);
           char_to_room(victim,get_room_index(632));

           if ( victim->pcdata->team == 9 )
              {
                 victim->gold = UMIN( 500000000, victim->gold + number_range( 250000, 750000 ) );
                 victim->pcdata->team = 0;
                 victim->practice += number_range( 1, 3 );

                 if ( !( victim->generation == 1 && victim->tier == 5 ) )
                    {
                       victim->generation--;

                       if ( victim->generation < 1 )
                          {
                             stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",victim);
                             victim->generation = 5;
                             victim->tier++;
                             victim->max_hit += 1000000;
                             victim->max_mana += 1000000;
                             victim->max_move += 1000000;
                          }
                    }

                 check_survival( ch );

                 if ( ch->pcdata->team == 0 )
                    {
                        if ( !( victim->generation == 1 && victim->tier == 5 ) )
                           {
                              victim->generation--;

                              if ( victim->generation < 1 )
                                 {
                                    stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",victim);
                                    victim->generation = 5;
                                    victim->tier++;
                                    victim->max_hit += 1000000;
                                    victim->max_mana += 1000000;
                                    victim->max_move += 1000000;
                                 }
                           }
                    }

                 return;
              }

           for ( gch = char_list; gch != NULL; gch = gch->next )
	     {
               if ( IS_NPC(gch) )
                  continue;

               if ( gch->pcdata->team == 0 )
                  continue;

               if ( gch->pcdata->team == 1 )
                  {
                     team[0]++;
                     if ( team[0] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 2 )
                  {
                     team[1]++;
                     if ( team[1] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 3 )
                  {
                     team[2]++;
                     if ( team[2] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 4 )
                  {
                     team[3]++;
                     if ( team[3] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 5 )
                  {
                     team[4]++;
                     if ( team[4] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 6 )
                  {
                     team[5]++;
                     if ( team[5] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 7 )
                  {
                     team[6]++;
                     if ( team[6] == 1 )
                        count++;
                     continue;
                  }

               if ( gch->pcdata->team == 8 )
                  {
                     team[7]++;
                     if ( team[7] == 1 )
                        count++;
                     continue;
                  }
           }

           if ( count == 3 && team[victim->pcdata->team -1] == 1 )
              third_team = victim->pcdata->team;
              else if ( count == 2 && team[victim->pcdata->team -1] == 1 )
                      {
                         second_team = victim->pcdata->team;
                         first_team = ch->pcdata->team;
                         stc("#cYour team had won the SlAuGhTeR!!!#n\n\r",ch);
                         sprintf(buf,"#yThe PK #gQuest #phas #CEnDeD!!!#n" );
                         do_info(ch,buf);
                         count = ch->level;
                         ch->level = 12;
                         do_ftag(ch,"reset");
                         ch->level = count;
                      }

           victim->pcdata->team = 0;
           return;
       }

    if ( is_safe( ch, victim ) )
       return;

    if ( endozone > 0 )
       {
          if ( number_percent() > 65 )
             {
                CHAR_DATA *temp = victim;
 
                sprintf( buf, "#eA loud demonic cackle fills the air as #w%s's #chands #R#zturns#n #eagainst them!#n", ch->name );
                do_info( NULL, buf);

                victim = ch;
                ch = temp;
            }
       }

    SET_BIT(victim->extra,TIED_UP);
    sprintf(buf,"#C%s has been tied up by %s.#n",victim->name,ch->name);
    do_info(ch,buf);

    if ( !( ch->generation == 1 && ch->tier == 5 ) && ( ch->in_room->vnum < 550
         || ch->in_room->vnum >= 630 ) && !IS_SET( victim->more2, MORE2_TIE )
         && ch->tier == victim->tier )
       {
          if ( ch->generation >= victim->generation )
             {
                if ( victim->generation < 5 && !armageddon )
                   victim->generation++;
                   else if ( victim->tier > 1 )
                           {
                              victim->max_hit -= 1000000;
                              victim->max_mana -= 1000000;
                              victim->max_move -= 1000000;
                              victim->generation = 1;
                              victim->tier--;
                              stc("#wYou #ghave gone #ydown#g the #Cranks#g... #wP#ea#wt#eh#we#et#wi#ec#g...#n\n\r",victim);
                           }

                SET_BIT( victim->more2, MORE2_TIE );

                act( "#wYou #P#zsteal#n #y$N#w's generation right before $S very eyes.#n", ch, NULL, victim, TO_CHAR );
                sprintf( buf, "#c%s's generation was #w#zstolen#n #cby %s.#n",victim->name,ch->name);
                do_info( ch, buf );

                if ( ch->generation != 1 )
                   ch->generation--;
                   else {
                           stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",ch);
                           ch->generation = 5;
                           ch->tier++;
                           ch->max_hit += 1000000;
                           ch->max_mana += 1000000;
                           ch->max_move += 1000000;
                        }
             }
       }
}

void do_untie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "But they are not tied up!\n\r", ch );
	return;
    }
    if (victim == ch)
    {
	send_to_char( "You cannot untie yourself!\n\r", ch );
	return;
    }
    act("You quickly untie $N.",ch,NULL,victim,TO_CHAR);
    act("$n quickly unties $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n quickly unties you.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,TIED_UP);
    return;
}

void do_gag( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch && !IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "You cannot gag yourself!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP) && !IS_EXTRA(victim,GAGGED))
    {
	send_to_char( "You can only gag someone who is tied up!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,GAGGED))
    {
    	act("You place a gag over $N's mouth.",ch,NULL,victim,TO_CHAR);
    	act("$n places a gag over $N's mouth.",ch,NULL,victim,TO_NOTVICT);
    	act("$n places a gag over your mouth.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->extra,GAGGED);
	return;
    }
    if (ch == victim)
    {
    	act("You remove the gag from your mouth.",ch,NULL,victim,TO_CHAR);
    	act("$n removes the gag from $s mouth.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->extra,GAGGED);
	return;
    }
    act("You remove the gag from $N's mouth.",ch,NULL,victim,TO_CHAR);
    act("$n removes the gag from $N's mouth.",ch,NULL,victim,TO_NOTVICT);
    act("$n removes the gag from your mouth.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,GAGGED);
    return;
}

void do_blindfold( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch && !IS_EXTRA(victim,BLINDFOLDED) && IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "You cannot blindfold yourself!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP) && !IS_EXTRA(victim,BLINDFOLDED))
    {
	send_to_char( "You can only blindfold someone who is tied up!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,BLINDFOLDED))
    {
    	act("You place a blindfold over $N's eyes.",ch,NULL,victim,TO_CHAR);
    	act("$n places a blindfold over $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    	act("$n places a blindfold over your eyes.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->extra,BLINDFOLDED);
	return;
    }
    if (ch == victim)
    {
    	act("You remove the blindfold from your eyes.",ch,NULL,victim,TO_CHAR);
    	act("$n removes the blindfold from $s eyes.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->extra,BLINDFOLDED);
	return;
    }
    act("You remove the blindfold from $N's eyes.",ch,NULL,victim,TO_CHAR);
    act("$n removes the blindfold from $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    act("$n removes the blindfold from your eyes.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,BLINDFOLDED);
    return;
}


int count_imms( CHAR_DATA *ch )
{
    int count = 0;

    if ( IS_IMMUNE(ch, IMM_SLASH)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_STAB)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_SMASH)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_ANIMAL)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_MISC)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_CHARM)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_HEAT)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_COLD)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_LIGHTNING) ) count += 1;
    if ( IS_IMMUNE(ch, IMM_ACID)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_SLEEP)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_DRAIN)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_VOODOO)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_HURL)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_BACKSTAB)  ) count += 1;
    if ( IS_IMMUNE(ch, IMM_KICK)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_DISARM)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_STEAL)     ) count += 1;
    return ( ( count * 10000 ) + 10000 );
}

void do_track( CHAR_DATA *ch, char *argument )
{
    bool found = FALSE;

    if ( IS_SET(ch->in_room->area->bits,WASTE) && !IS_CLASS(ch,CLASS_WEREWOLF) )
    {
	send_to_char("Its impossible to find any tracks in this devoid terrain.\n\r",ch);
	return;
    }

    if (check_track(ch,0)) found = TRUE;
    if (check_track(ch,1)) found = TRUE;
    if (check_track(ch,2)) found = TRUE;
    if (check_track(ch,3)) found = TRUE;
    if (check_track(ch,4)) found = TRUE;

    if (found == FALSE)
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	return;
    }
    act("$n carefully examines the ground for tracks.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_hunt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int check, count = 0;
    ROOM_INDEX_DATA *first_room,*in_room;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) ) 
       if ( !IS_CLASS(ch, CLASS_VAMPIRE) || !IS_SET(ch->polyaff, POLY_WOLF) )
             if ( !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_NIGHT_BLADE) && !IS_CLASS(ch, CLASS_SHADOW) )
                if ( !IS_SET( ch->cmbt[7], SKL_HUNT ) )
                {
                   stc("Huh?\n\r",ch);
                   return;
                }

    if ( IS_SET(ch->in_room->area->bits,WASTE) && !IS_CLASS(ch,CLASS_WEREWOLF) )
    {
	send_to_char("Its impossible to find any tracks in this devoid terrain.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	if (strlen(ch->hunting) > 1)
	{
	    free_string(ch->hunting);
	    ch->hunting = str_dup( "" );
	    send_to_char( "You stop hunting your prey.\n\r", ch );
	}
	else send_to_char( "Who do you wish to hunt?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, ch->name ) )
       {
          send_to_char("How can you hunt yourself?\n\r",ch);
          return;
       }

    ch->hunting = str_dup( arg );
    send_to_char( "You start hunting your quarry!\n\r", ch );
    first_room = ch->in_room;
  
    do
    {
       count++;
       in_room = ch->in_room;
       check = check_hunt(ch);

       if ( check == 0 && ch->in_room != in_room && ch->in_room != first_room )
          stc("\n\r",ch);
    }
    while ( check == 0 && ch->in_room != in_room && ch->in_room != first_room && count < 50 );

    free_string(ch->hunting);
    ch->hunting = str_dup( "" );

    if ( check != -1 && ( ch->in_room == in_room || ch->in_room == first_room ) )
       send_to_char( "You lost track of your quarry!\n\r", ch );

    ch->wait = 0;
    return;
}

int check_hunt( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    bool found = FALSE;
    int direction = 0;
    ROOM_INDEX_DATA *in_room;

    in_room = ch->in_room;

    if (check_track(ch,0)) {found = TRUE;direction = ch->in_room->track_dir[0];}
    else if (check_track(ch,1)) {found = TRUE;direction = ch->in_room->track_dir[1];}
    else if (check_track(ch,2)) {found = TRUE;direction = ch->in_room->track_dir[2];}
    else if (check_track(ch,3)) {found = TRUE;direction = ch->in_room->track_dir[3];}
    else if (check_track(ch,4)) {found = TRUE;direction = ch->in_room->track_dir[4];}
    else if (check_track(ch,5)) {found = TRUE;direction = ch->in_room->track_dir[5];}
    else {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
	return -1;
         }
    if (strlen(ch->hunting) < 2) return -1;

    if ( ( victim = get_char_room( ch, ch->hunting ) ) != NULL ) 
       {
          stc("You have found your quarry!",ch);
          return -1;
       }

    act("$n carefully examines the ground for tracks.\n\r",ch,NULL,NULL,TO_ROOM);

    move_char(ch,direction);

    if (in_room == ch->in_room || victim != NULL)
    {
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
      return -1;
    }

    return 0;
}

void add_tracks( CHAR_DATA *ch, int direction )
{
    int loop;

    if ( IS_NPC(ch) ) return;

    if ( IS_IMMORTAL( ch ) )
       return;

    if ( weather_info.sky != SKY_SNOWY )
       {
          if ( weather_info.sky == SKY_RAINING )
             return;

          if ( IS_CLASS(ch, CLASS_NINJA) )
             return;

          if ( IS_SET( ch->cmbt[7], SKL_STEALTH ) )
             return;

          if ( IS_CLASS( ch, CLASS_DEMON ) && IS_ITEMAFF( ch, ITEMA_STALKER ) )
             return;

          if ( ch->pcdata->personality == PER_STEALTHY )
             return;
       }

    for ( loop = 0; loop <= 4; loop++ )
    {
    	if (ch->in_room->track[loop] != NULL && !str_cmp(ch->in_room->track[loop],ch->name))
    	{
	    free_string(ch->in_room->track[loop]);
	    ch->in_room->track[loop] = str_dup("");
    	}
    }

    if (ch->in_room->track[0] != NULL && strlen(ch->in_room->track[0]) < 2)
    {
	free_string(ch->in_room->track[0]);
	ch->in_room->track[0]     = str_dup(ch->pcdata->switchname);
	ch->in_room->track_dir[0] = direction;
    }
    else if (ch->in_room->track[1] != NULL && strlen(ch->in_room->track[1]) < 2)
    {
	free_string(ch->in_room->track[1]);
	ch->in_room->track[1]     = str_dup(ch->pcdata->switchname);
	ch->in_room->track_dir[1] = direction;
    }
    else if (ch->in_room->track[2] != NULL && strlen(ch->in_room->track[2]) < 2)
    {
	free_string(ch->in_room->track[2]);
	ch->in_room->track[2]     = str_dup(ch->pcdata->switchname);
	ch->in_room->track_dir[2] = direction;
    }
    else if (ch->in_room->track[3] != NULL && strlen(ch->in_room->track[3]) < 2)
    {
	free_string(ch->in_room->track[3]);
	ch->in_room->track[3]     = str_dup(ch->pcdata->switchname);
	ch->in_room->track_dir[3] = direction;
    }
    else if (ch->in_room->track[4] != NULL && strlen(ch->in_room->track[4]) < 2)
    {
	free_string(ch->in_room->track[4]);
	ch->in_room->track[4]     = str_dup(ch->pcdata->switchname);
	ch->in_room->track_dir[4] = direction;
    }
    else
    {
	free_string(ch->in_room->track[0]);
	ch->in_room->track[0]     = str_dup(ch->in_room->track[1]);
	ch->in_room->track_dir[0] = ch->in_room->track_dir[1];
	free_string(ch->in_room->track[1]);
	ch->in_room->track[1]     = str_dup(ch->in_room->track[2]);
	ch->in_room->track_dir[1] = ch->in_room->track_dir[2];
	free_string(ch->in_room->track[2]);
	ch->in_room->track[2]     = str_dup(ch->in_room->track[3]);
	ch->in_room->track_dir[2] = ch->in_room->track_dir[3];
	free_string(ch->in_room->track[3]);
	ch->in_room->track[3]     = str_dup(ch->in_room->track[4]);
	ch->in_room->track_dir[3] = ch->in_room->track_dir[4];
	free_string(ch->in_room->track[4]);
	ch->in_room->track[4]     = str_dup(ch->name);
	ch->in_room->track_dir[4] = direction;
    }
    return;
}

bool check_track( CHAR_DATA *ch, int direction )
{
    CHAR_DATA *victim;
    char buf [MAX_INPUT_LENGTH];
    char vict [MAX_INPUT_LENGTH];
    int door;

    strcpy(vict,ch->hunting);
    if (!str_cmp(ch->hunting,vict))
    {
	if ( ( victim = get_char_room( ch, vict ) ) != NULL )
	{
	    act("You have found $N!",ch,NULL,victim,TO_CHAR);
	    free_string(ch->hunting);
	    ch->hunting = str_dup( "" );
	    return TRUE;
	}
    }
    if (strlen(ch->in_room->track[direction]) < 2) return FALSE;
    if (!str_cmp(ch->in_room->track[direction],ch->name)) return FALSE;
    if (strlen(ch->hunting) > 1 && str_cmp(ch->in_room->track[direction],ch->hunting)) return FALSE;
    door = ch->in_room->track_dir[direction];
    sprintf(buf,"You sense the trail of %s leading $T from here.",ch->in_room->track[direction]);
    act( buf, ch, NULL, dir_name[door], TO_CHAR );
    return TRUE;
}

void do_roll( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DEMON))
    {
	stc("Huh?\n\r",ch);
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

    if ( obj->in_room == NULL )
    {
	send_to_char("You are unable to move.\n\r",ch);
	return;
    }

    if      (!str_cmp(arg,"n") || !str_cmp(arg,"north")) do_north(ch,"");
    else if (!str_cmp(arg,"s") || !str_cmp(arg,"south")) do_south(ch,"");
    else if (!str_cmp(arg,"e") || !str_cmp(arg,"east" )) do_east(ch,"");
    else if (!str_cmp(arg,"w") || !str_cmp(arg,"west" )) do_west(ch,"");
    else if (!str_cmp(arg,"u") || !str_cmp(arg,"up"   )) do_up(ch,"");
    else if (!str_cmp(arg,"d") || !str_cmp(arg,"down" )) do_down(ch,"");
    else
    {
	send_to_char("Do you wish to roll north, south, east, west, up or down?\n\r",ch);
	return;
    }
    obj_from_room(obj);
    obj_to_room(obj,ch->in_room);
    return;
}

void do_leap( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *container;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch,CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
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

    if ( obj->in_room != NULL && arg[0] == '\0' )
    {
	send_to_char("Where do you want to leap?\n\r",ch);
	return;
    }

    if ( obj->in_room != NULL )
    {
	if ( ( victim = get_char_room( ch, arg ) ) != NULL )
	{
	    act("$p leaps into your hands.",victim,obj,NULL,TO_CHAR);
	    act("$p leaps into $n's hands.",victim,obj,NULL,TO_ROOM);
	    obj_from_room(obj);
	    obj_to_char(obj,victim);
	    return;
	}
	else if ( ( container = get_obj_room( ch, arg ) ) != NULL )
	{
	    if (container->item_type != ITEM_CONTAINER &&
		container->item_type != ITEM_CORPSE_NPC &&
		container->item_type != ITEM_CORPSE_PC)
	    {
		send_to_char("You cannot leap into that sort of object.\n\r",ch);
		return;
	    }
	    act("$p leap into $P.",ch,obj,container,TO_CHAR);
	    act("$p leaps into $P.",ch,obj,container,TO_ROOM);
	    obj_from_room(obj);
	    obj_to_obj(obj,container);
	    return;
	}
	else send_to_char( "Nothing here by that name.\n\r",ch );
	return;
    }
    if ( obj->carried_by != NULL )
    {
	act("$p leaps from your hands.",obj->carried_by,obj,NULL,TO_CHAR);
	act("$p leaps from $n's hands.",obj->carried_by,obj,NULL,TO_ROOM);

      if ( obj->wear_loc != WEAR_NONE )
         unequip_char( obj->carried_by, obj );

	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	return;
    }
    else if ( (container = obj->in_obj) != NULL && container->in_room != NULL )
    {
	obj_from_obj(obj);
	obj_to_room(obj,container->in_room);
	char_from_room(ch);
	char_to_room(ch,container->in_room);
	act("$p leap from $P.",ch,obj,container,TO_CHAR);
	act("$p leaps from $P.",ch,obj,container,TO_ROOM);
	return;
    }
    if (obj->in_room != NULL)
	send_to_char("You seem unable to leap anywhere.\n\r",ch);
    else
	send_to_char("You seem to be stuck!\n\r",ch);
    return;
}


void drow_hate( CHAR_DATA *ch)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->newbits, NEW_DROWHATE)) return;
	
	if (number_percent() < 25) {

	for (vch = char_list; vch != NULL; vch = vch_next)
	{
	vch_next = vch->next;
	if ( ch == vch) continue;
	if ( vch->in_room == NULL) continue;
	if ( vch->in_room == ch->in_room) {
		send_to_char("You scream out in hatred and attack!\n\r", ch );
		act("$n screams out in hatred and attacks!",ch,NULL,vch,TO_ROOM);
		do_kill(ch,vch->name);
		return;}
	}
	}

	return;
}
