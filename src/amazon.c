/***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
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

void do_amazoncreate( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    cost = 1;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_AMAZON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Syntax: amazoncreate <item>\n\r", ch );
          send_to_char( "items are: sword (1), shield (1), skirt (1), breastplate (1)\n\r", ch );
          send_to_char( "           helmet (1), bracer (1), boot (1), warbow (1)\n\r", ch );
          return;
       }

    if (!str_cmp(arg,"sword"))
       {
          vnum = 235;
       }
    else if (!str_cmp(arg,"shield"))
            {
               vnum = 236;
            }
    else if (!str_cmp(arg,"skirt"))
            {
               vnum = 237;
            }
    else if (!str_cmp(arg,"breastplate"))
            {
               cost = 10;
               vnum = 238;
            }
    else if (!str_cmp(arg,"helmet"))
            {
               vnum = 239;
            }
    else if (!str_cmp(arg,"bracer"))
            {
               vnum = 240;
            }
    else if (!str_cmp(arg,"boot"))
            {
               vnum = 241;
            }
    else if (!str_cmp(arg,"warbow"))
            {
               vnum = 242;
            }
    else {
            do_amazoncreate(ch,"");
            return;
         }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold#n pieces to make this item.\n\r", cost);
	    send_to_char(arg,ch);
          return;
       }

    ch->gold -= cost;

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
       {
	    send_to_char("Missing object, please inform KaVir.\n\r",ch);
	    return;
       }

    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("You yell loudly for the Legendary Sisters as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$n yells loudly as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_drill( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost;
    char buf[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_AMAZON) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
       centre_text("#b[-----(<#g Drills of The Mighty Jungle #b>)-----]#n",ch);
       centre_text("#b\\^<>^/#n",ch);
       centre_text("#b \\/ #n",ch);
       send_to_char("\n\r",ch);
       if ( ch->drill >= 1 )
          centre_text("Your can heighten your senses.",ch);
       if ( ch->drill >= 2 )
          centre_text("You have the ability to sense them from afar.",ch);
       if ( ch->drill >= 3 )
          centre_text("You can now shoot a barrage of arrows at your foes.",ch);
       if ( ch->drill >= 4 )
          centre_text("You can block with a shield.",ch);
       if ( ch->drill >= 5 )
          centre_text("You have the ability to throw a net over a victim.",ch);
       if ( ch->drill >= 6 )
          centre_text("You have the ability to hide in your surroundings.",ch);
       if ( ch->drill >= 7 )
          centre_text("You have the ability to turn your skin into a very hard bark.",ch);
       if ( ch->drill >= 8 )
          centre_text("You can release the anger of an amazon.",ch);
       if ( ch->drill >= 9 )
          centre_text("You can summon a devastating tornado.",ch);
       if ( ch->drill >= 10 )
          centre_text("You can turn your fists into steel, parrying with bare hands.",ch);
       if ( ch->drill >= 11 )
          centre_text("Your arrows now always strike on target.",ch);
       if ( ch->drill >= 12 )
          centre_text("You have the strength to continue fighting where others would have crumble.",ch);

       send_to_char("\n\r",ch);
       if ( ch->drill >= 12 )
          centre_text("You are a true warrior.",ch);
          else {
	             cost = ( ch->drill + 1 ) * 10;
	             sprintf(buf,"It costs you %d primal to improve your drill.", cost);
	             centre_text(buf,ch);
               }

       send_to_char("\n\r",ch);
       centre_text("#b \\/ #n",ch);
       centre_text("#b\\^<>^/#n",ch);
       centre_text("#b[-----------------------------------------------]#n\n",ch);
       return;
    }

    if (!str_cmp(arg,"improve"))
    {
	cost = ( ch->drill + 1 ) * 10;

	if ( ch->drill >= 12 )
	{
	    send_to_char("You had already finish your training.\n\r",ch);
	    return;
	}

	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your drill.\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}

	ch->drill++;
	ch->practice -= cost;
	sprintf(buf,"You drill improve.\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a drill, type: drill improve.\n\r",ch);
}

void do_camouflage( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;
	
   if ( !IS_CLASS(ch, CLASS_AMAZON) )
      {
         send_to_char("Huh?\n\r", ch);
         return;
	}

   if ( ch->drill < 7 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }
	
   if ( ch->fight_timer > 0 ) 
      {
         send_to_char("Not until your fight timer expires.\n\r", ch );
         return;
      }	

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      {
         REMOVE_BIT(ch->act, PLR_WIZINVIS);
         send_to_char( "You appear from seemingly nowhere.\n\r", ch );
         act( "$n appears from seemingly nowhere.", ch, NULL, NULL, TO_ROOM );
      }
      else {
              act( "$n fades into $s surroundings.", ch, NULL, NULL, TO_ROOM );
              send_to_char( "You fade into your surroundings.\n\r", ch );
              SET_BIT(ch->act, PLR_WIZINVIS);
           }
}

void do_amazon_shoot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg [MAX_INPUT_LENGTH];
    int count, dam, range = 11;
 
    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_AMAZON) )
       {
	     send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ch->drill < 3 )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->drill < 3 )
       range = number_range(1, 11);

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 242 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 242 && obj->pIndexData->vnum != 91 ) )
          if ( ( ( obj = get_eq_char( ch, WEAR_THIRD ) ) == NULL ) || ( obj->pIndexData->vnum != 242 && obj->pIndexData->vnum != 91 ) )
             if ( ( ( obj = get_eq_char( ch, WEAR_FOURTH ) ) == NULL ) || ( obj->pIndexData->vnum != 242 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need the Huge War-Bow to utilize this technique.\n\r", ch );
          return;
       }

    if ( ( victim = get_char_area( ch, arg ) ) == NULL )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }

    if ( ch == victim )
       {
	    send_to_char( "Dont be foolish.\n\r", ch );
	    return;
       }

    if ( is_safe(ch, victim) ) return;

    ch->wait = 18;
    dam = char_damroll( ch );

    for ( count = 1; count <= range; count ++ )
    {
        damage( ch, victim, dam, gsn_arrows );

        if ( victim->in_room == NULL || victim->position == POS_MORTAL )
           {
              show_total_damage( ch, victim );
              return;
           }

        if ( number_percent() > 94 )
           {
              if ( !IS_SET( victim->act, PLR_BRIEF2 ) )
                 act("The arrow suddenly explodes searing your chest.",victim,NULL,NULL,TO_CHAR);

              if ( !IS_SET( ch->act, PLR_BRIEF2 ) )
                 act("The arrow suddenly explodes searing $n's chest.",victim,NULL,ch,TO_VICT);

              ch->ele_attack = ATTACK_FIRE;
              hurt_person( ch, victim, 250000, FALSE );
           }

        if ( victim->in_room == NULL || victim->position == POS_MORTAL )
           {
              show_total_damage( ch, victim );
              return;
           }
    } 

    show_total_damage( ch, victim );
}

void do_barkskin( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_AMAZON) )
       {
	     send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ch->drill < 8 )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( IS_SET(ch->newbits, NEW_BARKSKIN) )
       {
	    send_to_char( "You smile weakly as your skin turns back to normal.\n\r", ch );
          REMOVE_BIT(ch->newbits, NEW_BARKSKIN);
       }
       else {
	         send_to_char( "You concentrate furiously as your skin turns into hard bark!\n\r", ch );
               SET_BIT(ch->newbits, NEW_BARKSKIN);
            }

    WAIT_STATE( ch, 11 );
}

void do_steelfist( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_AMAZON) )
       {
	     send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ch->drill < 11 )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( IS_SET(ch->more, MORE2_STEELFIST) )
       {
	    send_to_char( "Your hands return to normal.\n\r", ch );
          REMOVE_BIT(ch->more, MORE2_STEELFIST);
       }
       else {
	         send_to_char( "Your fists turn into steel!\n\r", ch );
               SET_BIT(ch->more, MORE2_STEELFIST);
            }

    WAIT_STATE( ch, 11 );
}

void do_tornado( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( !IS_CLASS(ch, CLASS_AMAZON) )
       {
	     send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ch->drill < 10 )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->mana < 900000 )
       {
          send_to_char("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    act("$n's hands glows #gpassionately green#n as $e leans forward and point one of them at you.",ch,NULL,NULL,TO_ROOM);
    act("A huge tornado devastates the area, throwing everyone high into the air.",ch,NULL,NULL,TO_ROOM);
 
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;
   
        if ( is_same_group(ch,vch) ) 
           continue;

        if ( !is_safe(ch, vch) && vch != ch )
	     {
              ch->ele_attack = ATTACK_THUNDER;
              hurt_person(ch,vch,800000,TRUE);
              act("You fall hundreds of feet crashing into the ground.",vch,NULL,NULL,TO_CHAR);
              act("$n falls hundreds of feet crashing into the ground.",vch,NULL,NULL,TO_ROOM);

              if ( number_percent() > 85 && !IS_CLASS(vch, CLASS_MONK) )
                 vch->wait = 22;
	     }
    }

    ch->mana -= 900000;
    ch->wait = 18;
}

void do_vacuum_spear( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *victim_next;
   OBJ_DATA *obj;
   int dam;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_AMAZON) || ch->generation != 1 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET(ch->more, MORE_BOMB) )
      {
         stc("Your body is still recovering from the last one.\n\r",ch);
         return;
      }

   SET_BIT(ch->more, MORE_BOMB);
   ch->wait = 12;

   act("You twirl your spear viciously as you lean forward.", ch,NULL,NULL,TO_CHAR );
   act("$n twirls $s spear viciously as $s leans forward.", ch,NULL,NULL,TO_ROOM );
   act("You give a small wink as you flash your spear around you.", ch,NULL,NULL,TO_CHAR );
   act("$n gives a small wink as $e flashes $s spear around $m.", ch,NULL,NULL,TO_ROOM );
   act("You yell '#wArrrw!#n' as you grip your spear releasing a series of vacuums!!!", ch,NULL,NULL,TO_CHAR );
   act("$n yells '#wArrrw!#n' as $e grips $s spear releasing a series of vacuums!!!", ch,NULL,NULL,TO_ROOM );

   for ( victim = ch->in_room->people; victim != NULL; victim = victim_next )
   {
      victim_next = victim->next_in_room;

      if ( victim->in_room == NULL )
         continue;

      if ( victim == ch )
         continue;

      if ( is_safe(ch,victim) || is_same_group(ch,victim) )
         continue;

      stc("You scream as the vacuum soars through you.\n\r",victim);
      act("$n screams as the vacuum soars through $m.",victim,NULL,NULL,TO_ROOM);

      victim->wait += 18;
      dam = 2500000;
      ch->ele_attack = ATTACK_THUNDER;
      hurt_person(ch,victim,dam,FALSE);  
  }
}
