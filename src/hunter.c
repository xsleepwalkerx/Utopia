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

void do_huntercreate( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    int cost, vnum, value;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    bool summons = FALSE;
    bool auras = FALSE;
    bool points = FALSE;
    bool agil = FALSE;
    bool tens = FALSE;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( arg[0] == '\0' )
       {
          send_to_char( "         #y#uItems for sale#n\n\r\n\r", ch );

          send_to_char( "#wbetting        - #o1 gold piece per point\n\r", ch );
          send_to_char( "#wtransmigration - #o15 gold pieces per point\n\r", ch );
          send_to_char( "#wultratrans     - #o1 legend point per 50000 points\n\r", ch );
          send_to_char( "#whwater         - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wevial          - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wpvial          - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wcross          - #o250000 gold pieces\n\r", ch );
          send_to_char( "#worb            - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wknife          - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wmanatap        - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wsummons        - #o250000 gold pieces\n\r", ch );
          send_to_char( "#wprism          - #o750000 gold pieces\n\r", ch );
          send_to_char( "#wicetrap        - #o750000 gold pieces\n\r", ch );
          send_to_char( "#wagility        - #o2500000 gold pieces\n\r", ch );
          send_to_char( "#wtension        - #o2500000 gold pieces\n\r", ch );
          send_to_char( "#wlionheart      - #o2500000 gold pieces\n\r", ch );
          send_to_char( "#waura           - #o3000000 gold pieces\n\r", ch );

          send_to_char( "\n\r         #y#uItems for sale#n\n\r\n\r", ch );

          send_to_char( "Syntax: Buy <item>\n\r", ch );
          return;
       }

    cost = 250000;

    if ( !str_cmp(arg,"hwater") )
       vnum = 262;
       else if ( !str_cmp(arg,"knife") )
               vnum = 263;
       else if ( !str_cmp(arg,"pvial") )
               vnum = 264;
       else if ( !str_cmp(arg,"evial") )
               vnum = 265;
       else if ( !str_cmp(arg,"cross") )
               vnum = 266;
       else if ( !str_cmp(arg,"orb") )
               vnum = 267;
       else if ( !str_cmp(arg,"manatap") )
               vnum = 259;
       else if ( !str_cmp(arg,"summons") )
               {
                  if ( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) )
                     {
                        stc( "What use do you have for these summoner?\n\r", ch );
                        return;
                     }

                  summons = TRUE;
               }
       else if ( !str_cmp(arg,"aura") )
               {
                  cost = 3000000;
                  auras = TRUE;
               }
       else if ( !str_cmp(arg,"agility") )
               {
                  cost = 2500000;
                  agil = TRUE;
               }
       else if ( !str_cmp(arg,"tension") )
               {
                  cost = 2500000;
                  tens = TRUE;
               }
       else if ( !str_cmp(arg,"lionheart") )
               {
                   if ( 2500000 > ch->gold )
                      {
                         sprintf(arg,"It costs you #w%d #ygold#n piece to buy this item.\n\r", 2500000);
                         send_to_char(arg,ch);
                         return;
                      }

                  if ( ( pObjIndex = get_obj_index( 91 ) ) == NULL )
                     return;

                  obj = create_object(pObjIndex, 60);
                  obj_to_char(obj, ch);

                  value = number_range( UMAX( 2, ch->tier - 2 ), ch->tier );
                  obj->level = value;

                  switch( value )
                  {
                     default:
                     case 2: value = number_range( 1, 11 ); break;
                     case 3: value = number_range( 21, 36 ); break;
                     case 4: value = number_range( 41, 58 ); break;
                     case 5: value = number_range( 61, 80 ); break;
                  }

                  if ( value != 0 )
                     add_skill( obj, value );

                  ch->gold -= 2500000;
                  obj->cost = 3000000;
                  act("#wYou #gbuy a #eLion#PHeart#g.#n",ch,NULL,NULL,TO_CHAR);
                  act("#w$n #gbuys a #eLion#PHeart#g.#n",ch,NULL,NULL,TO_ROOM);
                  return;
               }
       else if ( !str_cmp(arg,"transmigration") )
               {
                  points = TRUE;

                  argument = one_argument( argument, arg );

                  if ( arg[0] == '\0' )
                     {
                        stc( "How many levels do you want to buy?\n\r", ch );
                        return;
                     }

                  value = atoi( arg );

                  if ( value < 1 )
                     {
                        stc( "You need to buy at least 1 transmigration level.\n\r", ch );
                        return;
                     }

                  cost = 15 * value;
               }
       else if ( !str_cmp(arg,"ultratrans") )
               {
                  argument = one_argument( argument, arg );

                  if ( arg[0] == '\0' )
                     {
                        stc( "How many legend points do you wish to spend?\n\r", ch );
                        return;
                     }

                  value = atoi( arg );

                  if ( value < 1 )
                     {
                        stc( "You need to spend at least 1 legend point.\n\r", ch );
                        return;
                     }

                  if ( ch->pcdata->faithpoints < value )
                     {
                        stc( "You don't have that many legend points warrior.\n\r", ch );
                        return;
                     }

                  ch->pcdata->faithpoints -= value;

                  sprintf( buf, "#wYou #gbuy #w%d #Ctransmigration levels#g.#n", value );
                  act( buf, ch, NULL, NULL, TO_CHAR );
                  act("#w$n #gbuys some #Ctransmigration levels#g.#n",ch,NULL,NULL,TO_ROOM);
                  add_transmigration( ch, value * 50000 );
                  return;
               }
       else if ( !str_cmp(arg,"betting") )
               {
                  if ( mybet == TRUE )
                     {
                        stc( "Betting is already on warrior.\n\r", ch );
                        return;
                     }

                  argument = one_argument( argument, arg );

                  if ( arg[0] == '\0' )
                     {
                        stc( "How much money you want to pool in for betting?\n\r", ch );
                        return;
                     }

                  value = atoi( arg );

                  if ( value < 1 )
                     {
                        stc( "You need to pool in at least 1 gold piece.\n\r", ch );
                        return;
                     }

                  if ( value + betting_points > 10000000 )
                     value = 10000000 - betting_points;

                  if ( value > ch->gold )
                     {
                        send_to_char( "You don't have enough money to pool this figure in.\n\r",ch);
                        return;
                     }

                  ch->gold -= value;
                  betting_points += value;

                  act("#wYou #gplace some money into the #wbetting #Cpool#g.#n",ch,NULL,NULL,TO_CHAR);
                  act("#w$n #gplaces some money into the #wbetting #Cpool#g.#n",ch,NULL,NULL,TO_ROOM);

                  if ( betting_points == 10000000 )
                     {
                        mybet = TRUE;
                        do_info( NULL, "#PPlayers #wcan now #ybet #win #Ctournaments #wand #gteam battles#w!!!#n" );
                     }

                  return;
               }
       else if ( !str_cmp(arg,"icetrap") )
               {
                  cost = 750000;
                  vnum = 260;
               }
       else if ( !str_cmp(arg,"prism") )
               {
                  cost = 750000;
                  vnum = 261;
               }
               else {
                        do_huntercreate(ch,"");
                        return;
                    }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold#n piece to buy this item.\n\r", cost);
	    send_to_char(arg,ch);
          return;
       }

    ch->gold -= cost;

    if ( points == TRUE )
       {
          sprintf( buf, "#wYou #gbuy #w%d #Ctransmigration levels#g.#n", value );
          act( buf, ch, NULL, NULL, TO_CHAR );
          act("#w$n #gbuys some #Ctransmigration levels#g.#n",ch,NULL,NULL,TO_ROOM);
          add_transmigration( ch, value );
          return;
       }

    if ( auras == TRUE )
       {
          act("#wYou #gbuy a random #CAura#g.#n",ch,NULL,NULL,TO_CHAR);
          act("#w$n #gbuys a random #CAura#g.#n",ch,NULL,NULL,TO_ROOM);
          give_random_aura( ch );
          return;
       }

    if ( points == TRUE )
       {
          act("#wYou #gbuy a #Ctraining point#g.#n",ch,NULL,NULL,TO_CHAR);
          act("#w$n #gbuys a #Ctraining point#g.#n",ch,NULL,NULL,TO_ROOM);
          ch->practice++;
          return;
       }

    if ( agil == TRUE )
       {
          act("#wYou #gbuy a #Cagility reroll#g.#n",ch,NULL,NULL,TO_CHAR);
          act("#w$n #gbuys a #Cagility reroll#g.#n",ch,NULL,NULL,TO_ROOM);
          ch->pcdata->perm_agi = number_range( 1, 10 );
          return;
       }

    if ( tens == TRUE )
       {
          act("#wYou #gbuy a #Ctension reroll#g.#n",ch,NULL,NULL,TO_CHAR);
          act("#w$n #gbuys a #Ctension reroll#g.#n",ch,NULL,NULL,TO_ROOM);
          ch->pcdata->perm_tension = number_range( 10, 25 );
          return;
       }

    if ( summons == TRUE )
       {
          ch->cmbt[0]++;
          act("#wYou #gbuy a #CSummon Point#g.#n",ch,NULL,NULL,TO_CHAR);
          act("#w$n #gbuys a #CSummon Point#g.#n",ch,NULL,NULL,TO_ROOM);
          return;
       }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
       {
	    send_to_char("Missing object, please inform KaVir.\n\r",ch);
	    return;
       }

    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("#wYou #gbuy #C$p#g.#n",ch,obj,NULL,TO_CHAR);
    act("#w$n #gbuys #C$p#g.#n",ch,obj,NULL,TO_ROOM);
}

void do_mega_jump( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( IS_SET( ch->more, MORE_HAMMER) )  
     {
	  send_to_char( "You are still preparing yourself for the next one.\n\r", ch );
	  return;
     }

  if ( ch->move < 300000 )
     {
	  stc("You are too tired to perform this skill.\n\r", ch);
	  return;
     }

  SET_BIT( ch->more, MORE_HAMMER);
  ch->move -= 300000;
  stc("You leap high into the air focusing your strength onto your feet.\n\r",ch);
  act("$n leaps high into the air focusing $s strength onto $s feet.",ch,NULL,NULL,TO_ROOM);
  stc("You SmAsH your feet onto the ground creating a MaSsIvE EARTHQUAKE!!!\n\r",ch);
  act("$n SmAsHeS $s feet onto the ground creating a MaSsIvE EARTHQUAKE!!!",ch,NULL,NULL,TO_ROOM);

  victim = ch->in_room->people;

  for ( ; victim ; victim = victim->next_in_room )
  {
     if ( victim == ch || is_safe(ch,victim) )
        continue;

     act("The ShOcKwAvE sent you flying across the room StUnNeD.",victim,NULL,NULL,TO_CHAR);
     act("The ShOcKwAvE sent $n flying across the room StUnNeD.",victim,NULL,NULL,TO_ROOM);
 
     victim->hit = UMAX( 0, victim->hit - is_elemental( ATTACK_THUNDER, victim, 1000000 ) );

     stop_fighting(victim,TRUE);

     if ( is_martial( victim ) )
        {
           act("You suddenly flip back onto your feet.",victim,NULL,NULL,TO_CHAR);
           act("$n suddenly flips back onto $s feet.",victim,NULL,NULL,TO_ROOM);
        }
        else victim->wait += 24;
  }

  ch->fighting = NULL;
  ch->position = POS_STANDING;
}

void do_artic_wave( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to blast?\n\r",ch);
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

  if ( is_safe(ch,victim) ) return;

  act("You open your hand towards $N as a massive beam of energy emits from it.",ch,NULL,victim,TO_CHAR);
  act("$n opens $s hand towards you as a massive beam of energy emits from it.",ch,NULL,victim,TO_VICT);
  act("$n opens $s hand towards $N as a massive beam of energy emits from it.",ch,NULL,victim,TO_NOTVICT);
  act("You stagger backward from the blast.",victim,NULL,NULL,TO_CHAR);
  act("$n staggers backward from the blast.",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  ch->ele_attack = ATTACK_ICE;
  hurt_person(ch,victim,500000,TRUE);
}

void do_harpoon( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj = NULL;

  if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) )
     {
        do_dragoon_harpoon( ch, argument );
        return;
     }

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to strike?\n\r",ch);
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

  if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
        if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
           if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
     {
         stc("You aren't using the spiritual lance.\n\r",ch);
         return;
     }

  if ( ch->move < 100000 )
     {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
     }

  if ( is_safe(ch,victim) ) return;

  act("You dash towards $N thrusting your lance through $N.",ch,NULL,victim,TO_CHAR);
  act("$n dashes towards $N thrusting $s lance through you.",ch,NULL,victim,TO_VICT);
  act("$n dashes towards $N thrusting $s lance through $N.",ch,NULL,victim,TO_NOTVICT);
  act("Draping your lance upwards $N's body, you leap back SlAsHiNg across $S face.",ch,NULL,victim,TO_CHAR);
  act("Draping $s lance upwards yout body, $n leaps back SlAsHiNg across your face.",ch,NULL,victim,TO_VICT);
  act("Draping $s lance upwards $N's body, $n leaps back SlAsHiNg across $S face.",ch,NULL,victim,TO_NOTVICT);
  act("You yell '#yHarpoon!#n' as you rapidly thrust your lance through $N's chest.",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yHarpoon!#n' as $e rapidly thrusts $s lance through your chest.",ch,NULL,victim,TO_VICT);
  act("$n yells '#yHarpoon!#n' as $e rapidly thrusts $s lance through $N's chest.",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 18;
  ch->move -= 100000;
  hurt_person(ch,victim,600000,TRUE);
}

void do_hand_freeze( CHAR_DATA *ch, char *argument )
{
     if ( IS_NPC(ch) )
        return;

     if ( !IS_CLASS(ch, CLASS_HUNTER) )
        {
	     send_to_char("Huh?\n\r",ch);
  	     return;
        }

     if ( ch->move < 100000 )
        {
          stc("You are too tired to perform this technique.\n\r",ch);
          return;
        }

     ch->move -= 100000;

     if ( !IS_SET(ch->in_room->room_flags, ROOM_ENTOMB) )
        SET_BIT(ch->in_room->room_flags, ROOM_ENTOMB);

     act("You clench your fist as you SmAsH the ground viciously!", ch, NULL, NULL, TO_CHAR);
     act("$n clenches $s fist as $e SmAsHeS the ground viciously!", ch, NULL, NULL, TO_ROOM);
     act("A flash of light zooms across the room as the vicinity turns radiant blue.", ch, NULL, NULL, TO_CHAR);
     act("A flash of light zooms across the room as the vicinity turns radiant blue.", ch, NULL, NULL, TO_ROOM);
     act("A blast of chilly air shivers your spine as everything is encoated in ice!", ch, NULL, NULL, TO_CHAR);
     act("A blast of chilly air shivers your spine as everything is encoated in ice!", ch, NULL, NULL, TO_ROOM);

     ch->wait = 12;
}

void do_death_beam( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *obj = NULL;
  int dam;

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
        if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
           if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 259 && obj->pIndexData->vnum != 91 ) )
     {
         stc("You aren't using the spiritual lance.\n\r",ch);
         return;
     }

  if ( ch->mana < 600000 )
     {
	  stc("You don't have enough magical energy for this.\n\r", ch);
	  return;
     }

  ch->mana -= 600000;
  stc("You point your spear towards the centre of the room as it starts pulsing.\n\r",ch);
  act("$n points $s spear towards the centre of the room as it starts pulsing.",ch,NULL,NULL,TO_ROOM);
  act("A mass of energy converges to the tip as you release your powers through the blade.",ch,NULL,NULL,TO_CHAR);
  act("A mass of energy converges to the tip as $n releases $s powers through the blade.",ch,NULL,NULL,TO_ROOM);
  act("A black beam FiReS out of the tip of your lance as it spreads across the room!",ch,NULL,NULL,TO_CHAR);
  act("A black beam FiReS out of the tip of your lance as it spreads across the room!",ch,NULL,NULL,TO_ROOM);

  victim = ch->in_room->people;
  ch->wait = 12;

  for ( ; victim ; victim = victim->next_in_room )
  {
     if ( victim == ch || is_safe(ch,victim) )
        continue;

     act("The beam SOARS through your body eating away at your very soul.",victim,NULL,NULL,TO_CHAR);
     act("The beam SOARS through $n's body eating away at $s very soul.",victim,NULL,NULL,TO_ROOM);

     if ( !IS_NPC(victim) )
        dam = 600000;
        else if ( victim->level < 950 )
                dam = victim->max_hit;
                else dam = 1500000;

     ch->ele_attack = ATTACK_ICE; 
     hurt_person(ch,victim,dam,TRUE);
  }
}

void do_artic_storm( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam = 500000;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to shatter?\n\r",ch);
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

  if ( ch->mana < 1500000 )
     {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
     }

  if ( is_safe(ch,victim) ) return;

  if ( IS_CLASS(victim, CLASS_DEMON) || IS_CLASS(victim, CLASS_FIEND) )
     dam = 1000000;

  act("You open your hand at $N as the room flashes with UnBeLiEvAbLe EnErGy!!!",ch,NULL,victim,TO_CHAR);
  act("$n open $s hand at $N as the room flashes with UnBeLiEvAbLe EnErGy!!!",ch,NULL,victim,TO_VICT);
  act("$n open $s hand at $N as the room flashes with UnBeLiEvAbLe EnErGy!!!",ch,NULL,victim,TO_NOTVICT);
  act("You cackle with glee as a ROAR of energy forces the room to ViBrRrRaaTtTeEe MIGHTILY!!!",ch,NULL,NULL,TO_CHAR);
  act("$n cackles with glee as a ROAR of energy forces the room to ViBrRrRaaTtTeEe MIGHTILY!!!",ch,NULL,NULL,TO_ROOM);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS you into the ground!!!",victim,NULL,NULL,TO_CHAR);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS $n into the ground!!!",victim,NULL,NULL,TO_ROOM);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS you into the ground!!!",victim,NULL,NULL,TO_CHAR);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS $n into the ground!!!",victim,NULL,NULL,TO_ROOM);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS you into the ground!!!",victim,NULL,NULL,TO_CHAR);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS $n into the ground!!!",victim,NULL,NULL,TO_ROOM);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS you into the ground!!!",victim,NULL,NULL,TO_CHAR);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS $n into the ground!!!",victim,NULL,NULL,TO_ROOM);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS you into the ground!!!",victim,NULL,NULL,TO_CHAR);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS $n into the ground!!!",victim,NULL,NULL,TO_ROOM);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS you into the ground!!!",victim,NULL,NULL,TO_CHAR);
  act("A #wMaSsIve BeAm#n of #bFrEeZiNg #yENERGY#n SmAsHeS $n into the ground!!!",victim,NULL,NULL,TO_ROOM);
  act("The sky goes dark as a hailstorm strikes down upon you!!!",victim,NULL,NULL,TO_CHAR);
  act("The sky goes dark as a hailstorm strikes down upon $n!!!",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  ch->mana -= 1000000;
  ch->ele_attack = ATTACK_ICE; 
  hurt_person(ch,victim,dam,TRUE);
}

void do_lunge( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj = NULL;
  int dam = 400000;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to strike?\n\r",ch);
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

  if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
        if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
           if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
     {
         stc("You aren't using the silver sword.\n\r",ch);
         return;
     }

  if ( ch->move < 50000 )
     {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
     }

  if ( is_safe(ch,victim) ) return;

  act("Tilting forward, you leap towards $N!",ch,NULL,victim,TO_CHAR);
  act("Tilting forward, $n leaps towards you!",ch,NULL,victim,TO_VICT);
  act("Tilting forward, $n leaps towards $N!",ch,NULL,victim,TO_NOTVICT);
  act("You exclaim '#yEn Guarde#n' thrusting your sword through $N's chest!",ch,NULL,victim,TO_CHAR);
  act("$n exclaims '#yEn Guarde#n' thrusting $s sword through your chest!",ch,NULL,victim,TO_VICT);
  act("$n exclaims '#yEn Guarde#n' thrusting $s sword through $N's chest!",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 12;
  ch->move -= 50000;

  if ( IS_CLASS(victim, CLASS_WEREWOLF) || IS_CLASS(victim, CLASS_GAROU) )
     dam = 650000;

  hurt_person(ch,victim,dam,TRUE);
}

void do_power_smash( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj = NULL;
  int dam = 600000;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to strike?\n\r",ch);
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

  if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
        if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
           if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
     {
         stc("You aren't using the silver sword.\n\r",ch);
         return;
     }

  if ( ch->move < 250000 )
     {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
     }

  if ( is_safe(ch,victim) ) return;

  act("The air vibrates loudly as you draw back your sword!",ch,NULL,NULL,TO_CHAR);
  act("The air vibrates loudly as $n draws back $s sword!",ch,NULL,NULL,TO_ROOM);
  act("You twirl your sword SmAsHiNg it into the ground causing the earth to shatter!",ch,NULL,NULL,TO_CHAR);
  act("$n twirls $s sword SmAsHiNg it into the ground causing the earth to shatter!",ch,NULL,NULL,TO_ROOM);
  act("The earth ERUPTS beneath your feet BlAsTiNg you into the air!",victim,NULL,NULL,TO_CHAR);
  act("The earth ERUPTS beneath $n's feet BlAsTiNg you into the air!",victim,NULL,NULL,TO_ROOM);
  act("You crash into the ground stunned.",victim,NULL,NULL,TO_CHAR);
  act("$n crashes into the ground stunned.",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  ch->move -= 250000;

  ch->ele_attack = ATTACK_EARTH; 
  hurt_person(ch,victim,dam,TRUE);
}

void do_rage_strike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj = NULL;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_HUNTER) )
       {
	    send_to_char("Huh?\n\r",ch);
	    return;
       }

    if ( IS_SET( ch->more3, MORE3_CLAN2) )  
       {
	    send_to_char( "You are still preparing yourself for the next one.\n\r", ch );
	    return;
       }

    argument = one_argument( argument, arg);

    if ( ch->move < 400000 )
       {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Whom do you wish to strike?\n\r",ch);
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

    if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
          if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
             if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 260 && obj->pIndexData->vnum != 91 ) )
       { 
         stc("You aren't using the silver sword.\n\r",ch);
         return;
       }

    act("You ChArGe dIrEcTlY into $N with sword ExTeNdEd!",ch,NULL,victim,TO_CHAR);
    act("$n ChArGeS dIrEcTlY into you with sword ExTeNdEd!",ch,NULL,victim,TO_VICT);
    act("$n ChArGeS dIrEcTlY into $N with sword ExTeNdEd!",ch,NULL,victim,TO_NOTVICT);

    SET_BIT( ch->more3, MORE3_CLAN2);
    hurt_person(ch,victim,1000000,TRUE);

    if ( victim == NULL )
       return;

    multi_hit( ch, victim, TYPE_UNDEFINED );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    ch->wait = 12;
}

void do_silver_rain( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int dam;

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( ch->mana < 1500000 )
     {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
     }

  ch->mana -= 1500000;
  ch->wait = 12;
  stc("You throw a mass of fine powder high above you.\n\r",ch);
  act("$n throws a mass of fine powder high above $m.",ch,NULL,NULL,TO_ROOM);
  act("The powder scatters through the clouds as they pulses with energy.",ch,NULL,NULL,TO_CHAR);
  act("The powder scatters through the clouds as they pulses with energy.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, a deadly rain of silver showers the area with a DOWNPOUR!",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, a deadly rain of silver showers the area with a DOWNPOUR!",ch,NULL,NULL,TO_ROOM);

  victim = ch->in_room->people;

  for ( ; victim ; victim = victim->next_in_room )
  {
     if ( victim == ch || is_safe(ch,victim) )
        continue;

     if ( IS_CLASS(victim, CLASS_WEREWOLF) || IS_CLASS(victim, CLASS_GAROU) )
        dam = 1000000;
        else dam = 500000;

     act("You scream loudly as the rain sears your flesh!",victim,NULL,NULL,TO_CHAR);
     act("$n screams loudly as the rain sears $s flesh!",victim,NULL,NULL,TO_ROOM);

     ch->ele_attack = ATTACK_ICE; 
     hurt_person(ch,victim,dam,TRUE);
  }
}

void do_energy_blast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int dam;
  OBJ_DATA *obj = NULL;

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
        if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
           if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
     { 
        stc("You aren't using the chain whip.\n\r",ch);
        return;
     }

  if ( ch->move < 600000 )
     {
	  stc("You are too tired to execute this technique.\n\r", ch);
	  return;
     }

  ch->move -= 600000;
  ch->wait = 12;
  stc("You flex your wrist crackling your whip through the air.\n\r",ch);
  act("$n flexes $s wrist crackling $s whip through the air.",ch,NULL,NULL,TO_ROOM);
  act("You yell '#yEnergy Blast!#n' as you RiP your whip across the air.",ch,NULL,NULL,TO_CHAR);
  act("$n yells '#yEnergy Blast!#n' as $n RiPs $s whip across the air.",ch,NULL,NULL,TO_ROOM);
  act("The air ROARS as a MaSsIvE EnErGy BaLl flashes through the vicinity!",ch,NULL,NULL,TO_CHAR);
  act("The air ROARS as a MaSsIvE EnErGy BaLl flashes through the vicinity!",ch,NULL,NULL,TO_ROOM);

  victim = ch->in_room->people;

  for ( ; victim ; victim = victim->next_in_room )
  {
     if ( victim == ch || is_safe(ch,victim) )
        continue;

     dam = 600000;
     ch->ele_attack = ATTACK_THUNDER; 
     act("You stagger from the energy blast!",victim,NULL,NULL,TO_CHAR);
     act("$n staggers from the energy blast!",victim,NULL,NULL,TO_ROOM);

     hurt_person(ch,victim,dam,TRUE);
  }
}

void do_flaming_strike( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj = NULL;
  int dam = 600000;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to strike?\n\r",ch);
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

  if ( ( ( obj = get_eq_char(ch,WEAR_WIELD) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
     if ( ( ( obj = get_eq_char(ch,WEAR_HOLD) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
        if ( ( ( obj = get_eq_char(ch,WEAR_THIRD) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
           if ( ( ( obj = get_eq_char(ch,WEAR_FOURTH) ) == NULL ) || ( obj->pIndexData->vnum != 261 && obj->pIndexData->vnum != 91 ) )
     {
         stc("You aren't using the chain whip.\n\r",ch);
         return;
     }

  if ( ch->move < 250000 )
     {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
     }

  if ( is_safe(ch,victim) ) return;

  act("The air vibrates loudly as you draw back your whip!",ch,NULL,NULL,TO_CHAR);
  act("The air vibrates loudly as $n draws back $s whip!",ch,NULL,NULL,TO_ROOM);
  act("You whirl your whip around your body as it burst into flames!",ch,NULL,NULL,TO_CHAR);
  act("$n whirls $s whip around $s body as it burst into flames!",ch,NULL,NULL,TO_ROOM);
  act("You SlAsH your whip across $N's chest yelling '#rFlaming Strike!#n'",ch,NULL,victim,TO_CHAR);
  act("$n SlAsHeS $s whip across your chest yelling '#rFlaming Strike!#n'",ch,NULL,victim,TO_VICT);
  act("$n SlAsHeS $s whip across $N's chest yelling '#rFlaming Strike!#n'",ch,NULL,victim,TO_NOTVICT);
  act("You SCREAM loudly as you burst into flames!",victim,NULL,NULL,TO_CHAR);
  act("$n SCREAMS loudly as $e bursts into flames!",victim,NULL,NULL,TO_ROOM);

  if ( !IS_AFFECTED(victim, AFF_FLAMING) ) 
     SET_BIT(victim->affected_by, AFF_FLAMING);

  ch->wait = 18;
  ch->move -= 250000;

  ch->ele_attack = ATTACK_FIRE; 
  hurt_person(ch,victim,dam,TRUE);
}

void do_ethereal_stake( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam = 500000;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_HUNTER) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to stake?\n\r",ch);
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

  if ( ch->mana < 1500000 )
     {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
     }

  if ( is_safe(ch,victim) ) return;

  if ( IS_CLASS(victim, CLASS_VAMPIRE) || IS_CLASS(victim, CLASS_SOULREAVER) )
     dam = 1000000;

  act("You raise your hand above you as an ethereal stake manifests into your hand.",ch,NULL,NULL,TO_CHAR);
  act("$n raises $s hand above $m as an ethereal stake manifests into $s hand.",ch,NULL,NULL,TO_ROOM);
  act("Your eyes glow red as flashes of energy pulses from the stake!",ch,NULL,NULL,TO_CHAR);
  act("$n's eyes glow red as flashes of energy pulses from the stake!",ch,NULL,NULL,TO_ROOM);
  act("You exclaim '#yEthereal Stake!#n' as you throw the stake at $N!",ch,NULL,victim,TO_CHAR);
  act("$n exclaims '#yEthereal Stake!#n' as $e throws the stake at you!",ch,NULL,victim,TO_VICT);
  act("$n exclaims '#yEthereal Stake!#n' as $e throws the stake at $N!",ch,NULL,victim,TO_NOTVICT);
  act("The stake FlIeS through your body SmAsHiNg your soul!",victim,NULL,NULL,TO_CHAR);
  act("The stake FlIeS through $n's body SmAsHiNg your soul!",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  ch->mana -= 1500000;
  hurt_person(ch,victim,dam,TRUE);
}

void do_sustain( CHAR_DATA *ch, char *argument )
{
  int obj_num;
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *vch, *vch_next, *victim;
  bool world = FALSE;
  OBJ_DATA *obj = NULL;

  if ( IS_NPC(ch) )
     return;

  if ( !can_do_skill( ch, 33 ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  argument = one_argument( argument, arg );

  if ( ch->use_timer > 0 )
     {
        stc("You are still preparing yourself for the next one.\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     {
        send_to_char( "Use what?\n\r", ch );
        return;
     }

  if ( !str_cmp(arg,"hwater") || !str_cmp(arg,"water") )
     obj_num = 262;
     else if ( !str_cmp(arg,"knife") )
             obj_num = 263;
     else if ( !str_cmp(arg,"pvial") || !str_cmp(arg,"poison") )
             obj_num = 264;
     else if ( !str_cmp(arg,"evial") || !str_cmp(arg,"explosive") )
             obj_num = 265;
     else if ( !str_cmp(arg,"cross") )
             obj_num = 266;
     else if ( !str_cmp(arg,"orb") )
             obj_num = 267;
     else if ( !str_cmp(arg,"prism") )
             {
                obj_num = 261;
                world = TRUE;
             }
     else if ( !str_cmp(arg,"icetrap") )
             obj_num = 260;
     else if ( !str_cmp(arg,"manatap") )
             obj_num = 259;
             else {
                     stc("You can't throw those type of items?\n\r",ch);
                     return;
                  }

  for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
  {
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) && obj->pIndexData->vnum == obj_num )
         break;
  }

  if ( obj == NULL )
     {
        if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != obj_num ) )
           if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != obj_num ) )
              {
	           send_to_char( "But you aren't holding that item.\n\r", ch );
	           return;
              }  
     }

  argument = one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use this on?\n\r",ch);
	   return;
     }

  if ( arg[0] != '\0' )
     {
        if ( world == TRUE )
           {
              if ( ( victim = get_char_world( ch, arg ) ) == NULL )
                 {
                    send_to_char( "They aren't here.\n\r", ch );
                    return;
                 }
           }
           else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
                   {
                      send_to_char( "They aren't here.\n\r", ch );
                      return;
                   }
     }

  ch->wait = 6;
  ch->use_timer = 25;

  if ( obj_num == 262 )
     {
        act("You crouch on your knee as you hurl the holy flask into the sky!",ch,NULL,NULL,TO_CHAR);
        act("$n crouches on $s knee as $e hurls the holy flask into the sky!",ch,NULL,NULL,TO_ROOM);
        act("The flask shatter in a flash of blinding light!",ch,NULL,NULL,TO_CHAR);
        act("The flask shatter in a flash of blinding light!",ch,NULL,NULL,TO_ROOM);
        act("Suddenly, the entire vicinity is pelted down with a holy shower!",ch,NULL,NULL,TO_CHAR);
        act("Suddenly, the entire vicinity is pelted down with a holy shower!",ch,NULL,NULL,TO_ROOM);

        for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next_in_room;

            if ( is_safe(ch, vch) )
               continue;

            act("You scream as the rain drops sears your flesh!",vch,NULL,NULL,TO_CHAR);
            act("$n screams as the rain drops sears $s flesh!",vch,NULL,NULL,TO_ROOM);

            ch->ele_attack = ATTACK_HOLY;
            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = AERIAL;
            ch->attack_var = HIT;

            if ( IS_CLASS( vch, CLASS_VAMPIRE ) )
               hurt_person( ch, vch, 1500000, TRUE );
               else hurt_person( ch, vch, 750000, TRUE );
        }
     }

  if ( obj_num == 266 )
     {
        act("You leap into the air spreading your arms apart!",ch,NULL,NULL,TO_CHAR);
        act("$n leaps into the air spreading $s arms apart!",ch,NULL,NULL,TO_ROOM);
        act("Your entire body pulses as a huge spiritual cross fades into existence.",ch,NULL,NULL,TO_CHAR);
        act("$n's entire body pulses as a huge spiritual cross fades into existence.",ch,NULL,NULL,TO_ROOM);
        act("The Holy Cross flashes as it rapidly hovers across the room!",ch,NULL,NULL,TO_CHAR);
        act("The Holy Cross flashes as it rapidly hovers across the room!",ch,NULL,NULL,TO_ROOM);

        for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
        {
           vch_next = vch->next_in_room;

           if ( is_safe(ch, vch) )
              continue;

           act("You shudder in revulsion as the cross sears your flesh!",vch,NULL,NULL,TO_CHAR);
           act("$n shudders in revulsion as the cross sears $s flesh!",vch,NULL,NULL,TO_ROOM);

           ch->ele_attack = ATTACK_HOLY;
           ch->attack_type = ATTACK_MAGICAL;
           ch->attack_focus = AERIAL;
           ch->attack_var = SOUL;

           if ( IS_CLASS( vch, CLASS_DEMON ) )
              hurt_person( ch, vch, 1500000,TRUE );
              else hurt_person( ch, vch, 500000, TRUE );
       }

        return;
     }

  if ( obj_num == 267 )
     {
        int dam = 500000;

        act("You throw the diamond orb into the air as it starts pulsing with energy!",ch,NULL,NULL,TO_CHAR);
        act("$n throws the diamond orb into the air as it starts pulsing with energy!",ch,NULL,NULL,TO_ROOM);

        act("A green crosspoint appears on you as the diamond orb hones in on you!",victim,NULL,NULL,TO_CHAR);
        act("A green crosspoint appears on $n as the diamond orb hones in on $m!",victim,NULL,NULL,TO_ROOM);
        act("The orb blasts a laser beam into your chest!",victim,NULL,NULL,TO_CHAR);
        act("The orb blasts a laser beam into $n's chest!",victim,NULL,NULL,TO_ROOM);
        act("The orb blasts a laser beam into your chest!",victim,NULL,NULL,TO_CHAR);
        act("The orb blasts a laser beam into $n's chest!",victim,NULL,NULL,TO_ROOM);
        act("The orb blasts a laser beam into your chest!",victim,NULL,NULL,TO_CHAR);
        act("The orb blasts a laser beam into $n's chest!",victim,NULL,NULL,TO_ROOM);
        act("The orb blasts a laser beam into your chest!",victim,NULL,NULL,TO_CHAR);
        act("The orb blasts a laser beam into $n's chest!",victim,NULL,NULL,TO_ROOM);
        act("The orb blasts a laser beam into your chest!",victim,NULL,NULL,TO_CHAR);
        act("The orb blasts a laser beam into $n's chest!",victim,NULL,NULL,TO_ROOM);

        if ( IS_CLASS( victim, CLASS_WEREWOLF ) )
           dam = 1500000;

        ch->ele_attack = ATTACK_THUNDER;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = AERIAL;
        ch->attack_var = BEAM;

        hurt_person( ch, victim, dam, TRUE );
        return;
     }

  if ( obj_num == 263 )
     {
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You are SLAMMED into the ground as the daggers strike you furiously across your body!",victim,NULL,NULL,TO_CHAR);
        act("$n is SLAMMED into the ground as the daggers strike $m furiously across $s body!",victim,NULL,NULL,TO_ROOM);

        ch->wait = 8;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = DIRECT;
        ch->attack_var = PIERCE;
        hurt_person(ch,victim,1000000,TRUE);
        return;
     }

  if ( obj_num == 261 )
     {
        CHAR_DATA *mount;

        ch->wait = 0;

        if ( victim->in_room->vnum < 100 )
           {
              stc("You cannot travel to that room\n\r",ch);
              return;
           }

        if ( ch->in_room == victim->in_room )
           {
              stc( "You are already in the same room as your intended victim.\n\r", ch );
              return;
           }

        stop_fighting( ch, TRUE );
        act("You SLAM the #sp#sr#si#ss#sm#n onto the ground as a shimmering vortex appear!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the #sp#sr#si#ss#sm#n onto the ground as a shimmering vortex appear!",ch,NULL,NULL,TO_ROOM);
        act("Before anyone can move, you quickly leap into the vortex as it disappears!",ch,NULL,NULL,TO_CHAR);
        act("Before anyone can move, $n quickly leaps into the vortex as it disappears!",ch,NULL,NULL,TO_ROOM);

        char_from_room( ch );
        char_to_room( ch, victim->in_room );

        do_look( ch, "" );
        act("$n quickly leaps out of a fading vortex as it disappears!",ch,NULL,NULL,TO_ROOM);

        if ( ( mount = ch->mount ) == NULL ) 
           return;

        char_from_room( mount );
        char_to_room( mount, ch->in_room );
     }

  if ( obj_num == 265 )
     {
        act("You SLAM a bunch of explosive vials onto the ground as they explode with divine energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS a bunch of explosive vials onto the ground as they explode with divine energy!",ch,NULL,NULL,TO_ROOM);
        act("The room ViBrAtEs with FEROICIOUS energy as MuLtIpLe flashes SOARS through!!!",ch,NULL,NULL,TO_CHAR);
        act("The room ViBrAtEs with FEROICIOUS energy as MuLtIpLe flashes SOARS through!!!",ch,NULL,NULL,TO_ROOM);
        ch->wait = 4;

        for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next_in_room;

            if ( is_safe(ch, vch) )
               continue;

            if ( !is_martial( vch ) )
               vch->wait = 24;

            act("A flash of light shines through your eyes blinding you!",victim,NULL,NULL,TO_CHAR);
            act("A flash of light shines through $n's eyes blinding $m!",victim,NULL,NULL,TO_ROOM);
            act("You clutch your hands in agony as you fall to the ground!",victim,NULL,NULL,TO_CHAR);
            act("$n clutches $s hands in agony as $e falls to the ground!",victim,NULL,NULL,TO_ROOM);
        }

        return;
     }

  if ( obj_num == 264 )
     {
        act("You SLAM the ancient vial onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the ancient vial onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_ROOM);
        act("Misty fumes seeps from the ground poisoning the air around you.",victim,NULL,NULL,TO_CHAR);
        act("Misty fumes seeps from the ground poisoning the air around you.",victim,NULL,NULL,TO_ROOM);
        ch->wait = 4;

        for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next_in_room;

            if ( ( IS_CLASS(victim, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
                 || IS_CLASS(victim,CLASS_DRAGON) || is_safe(ch, vch) )
               continue;

            if ( !IS_SET(vch->more, MORE_POISON) )
               SET_BIT(vch->more, MORE_POISON);

            if ( !IS_SET(vch->more, MORE_BLIND) )
               SET_BIT(vch->more, MORE_BLIND);

            vch->move = UMAX( 0, vch->move - 5000000 );
            vch->mana = UMAX( 0, vch->mana - 5000000 );
            
            act("You cough loudly as the fumes fills your lungs!",victim,NULL,NULL,TO_CHAR);
            act("$n coughs loudly as the fumes fills $s lungs!",victim,NULL,NULL,TO_ROOM);

            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = AERIAL;
            ch->attack_var = HIT;

            if ( IS_CLASS( vch, CLASS_DUERGAR ) )
               vch->hit = UMIN( vch->max_hit, vch->hit + ( 500000 * UMAX( 1, vch->pkill / 10 ) ) );
               else hurt_person( ch, vch, 500000, TRUE );
        }

        return;
     }

  if ( obj_num == 260 )
     {
        extract_obj( obj );

        act("You SLAM the ice trap onto the ground as it explodes with shivering energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the ice trap onto the ground as it explodes with shivering energy!",ch,NULL,NULL,TO_ROOM);

        for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next_in_room;

            if ( is_safe(ch, vch) )
               continue;

            act("Icy crystals suddenly encase $n's body as $e grimaces in pain.",vch,NULL,NULL,TO_ROOM);
            act("Icy crystals suddenly encase your body as you grimace in pain.",vch,NULL,NULL,TO_CHAR);

            if ( !IS_SET( vch->more, MORE_FROZEN ) && !is_martial( vch ) )
               {
                  SET_BIT( vch->more, MORE_FROZEN );
                  vch->freeze_timer = number_range( 2, 3 );
               }

            ch->ele_attack = ATTACK_ICE;
            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = DIRECT;
            ch->attack_var = BLAST;
            hurt_person( ch, vch, 750000, TRUE );
        }

        return;
     }

  if ( obj_num == 259 )
     {
        extract_obj( obj );

        act("You SLAM the mana trap onto the ground as it explodes with a mysterious energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the mana trap onto the ground as it explodes with a mysterious energy!",ch,NULL,NULL,TO_ROOM);
        ch->mana = UMIN( ch->max_mana, ch->mana + ( ch->max_mana * 0.25 ) );

        for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next_in_room;

            if ( is_safe(ch, vch) )
               continue;

            ch->ele_attack = ATTACK_DARK;
            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = AERIAL;
            ch->attack_var = SOUL;

            vch->mana = UMAX( 0, vch->mana - ( vch->max_mana * 0.25 ) );

            act("Your body warps out of existence as you feel your spiritful energy being drained.",vch,NULL,NULL,TO_CHAR);
            act("$n's body warps out of existence as $e feels $s spiritful energy being drained.",vch,NULL,NULL,TO_ROOM);
            hurt_person( ch, vch, 750000, TRUE );
        }

        return;
     }
}

void do_use( CHAR_DATA *ch, char *argument )
{
  int obj_num;
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj = NULL;
  bool world = FALSE;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( ch->use_timer > 0 )
     {
        send_to_char( "You are still recovering from the last one.\n\r", ch );
        return;
     }

  if ( arg[0] == '\0' )
     {
        send_to_char( "Use what?\n\r", ch );
        return;
     }

  if ( !str_cmp(arg,"hwater") || !str_cmp(arg,"water") )
     obj_num = 262;
     else if ( !str_cmp(arg,"knife") )
             obj_num = 263;
     else if ( !str_cmp(arg,"pvial") || !str_cmp(arg,"poison") )
             obj_num = 264;
     else if ( !str_cmp(arg,"evial") || !str_cmp(arg,"explosive") )
             obj_num = 265;
     else if ( !str_cmp(arg,"cross") )
             obj_num = 266;
     else if ( !str_cmp(arg,"orb") )
             obj_num = 267;
     else if ( !str_cmp(arg,"prism") )
             {
                obj_num = 261;
                world = TRUE;
             }
     else if ( !str_cmp(arg,"icetrap") )
             obj_num = 260;
     else if ( !str_cmp(arg,"manatap") )
             obj_num = 259;
             else {
                     stc("You can't throw those type of items?\n\r",ch);
                     return;
                  }

  for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
  {
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) && obj->pIndexData->vnum == obj_num )
         break;
  }

  if ( obj == NULL )
     {
        if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != obj_num ) )
           if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != obj_num ) )
              {
	           send_to_char( "But you aren't holding that item.\n\r", ch );
	           return;
              }  
     }

  argument = one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use this on?\n\r",ch);
	   return;
     }

  if ( arg[0] != '\0' )
     {
        if ( world == TRUE )
           {
              if ( ( victim = get_char_world( ch, arg ) ) == NULL )
                 {
                    send_to_char( "They aren't here.\n\r", ch );
                    return;
                 }
           }
           else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
                   {
                      send_to_char( "They aren't here.\n\r", ch );
                      return;
                   }
     }

  ch->wait = 6;
  ch->use_timer = 25;

  if ( obj_num == 262 )
     {
        extract_obj( obj );

        if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
           SET_BIT( victim->affected_by, AFF_FLAMING );

        act("You SLAM the flask of holy water onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the flask of holy water onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_ROOM);
        act("A spiral of flames ROARS across the floor engulfing you with its purified flames!",victim,NULL,NULL,TO_CHAR);
        act("A spiral of flames ROARS across the floor engulfing $n with its purified flames!",victim,NULL,NULL,TO_ROOM);
        act("You clutch your hands in agony as you fall to the ground!",victim,NULL,NULL,TO_CHAR);
        act("$n clutches $s hands in agony as $e falls to the ground!",victim,NULL,NULL,TO_ROOM);

        ch->ele_attack = ATTACK_FIRE;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = BLAST;
        ch->attack_var = LOW;

        hurt_person( ch, victim, 500000, TRUE );
        return;
     }

  if ( obj_num == 266 )
     {
        extract_obj( obj );

        act("You lower your hand towards your cross as it starts pulsing with holy energy!",ch,NULL,NULL,TO_CHAR);
        act("$n lowers $s hand towards $s cross as it starts pulsing with holy energy!",ch,NULL,NULL,TO_ROOM);
        act("An invisible force suddenly STRIKE you down!",victim,NULL,NULL,TO_CHAR);
        act("An invisible force suddenly STRIKE $n down!",victim,NULL,NULL,TO_ROOM);        

        ch->ele_attack = ATTACK_HOLY;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = OVERHEAD;
        ch->attack_var = SOUL;

        hurt_person( ch, victim, 500000, TRUE );

        if ( victim != NULL && victim->fighting != NULL && number_percent() > 80 )
           {
              stop_fighting( victim, TRUE );
              victim->position = POS_STUNNED;
           }

        return;
     }

  if ( obj_num == 267 )
     {
        extract_obj( obj );

        act("You SLAM the diamond orb onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the diamond orb onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_ROOM);
        act("A laser beam bounces across the room as it blasts through you.",victim,NULL,NULL,TO_CHAR);
        act("A laser beam bounces across the room as it blasts through $n.",victim,NULL,NULL,TO_ROOM);

        ch->ele_attack = ATTACK_THUNDER;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = UPPERCUT;
        ch->attack_var = BEAM;

        hurt_person( ch, victim, number_range( 400000, 700000 ), TRUE );
        return;
     }

  if ( obj_num == 263 )
     {
        extract_obj( obj );

        act("You flick a knife on your wrist throwing it at $N swiftly!",ch,NULL,victim,TO_CHAR);
        act("$n flicks a knife on $s wrist throwing it at you swiftly!",ch,NULL,victim,TO_VICT);
        act("$n flicks a knife on $s wrist throwing it at $N swiftly!",ch,NULL,victim,TO_NOTVICT);
        act("You wince as the dagger strikes you!",victim,NULL,NULL,TO_CHAR);
        act("$n winces as the dagger strikes $n!",victim,NULL,NULL,TO_ROOM);

        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = DIRECT;
        ch->attack_var = PIERCE;

        hurt_person( ch, victim, 600000, TRUE );
        return;
     }

  if ( obj_num == 265 )
     {
        extract_obj( obj );

        act("You SLAM the explosive vial onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the explosive vial onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_ROOM);
        act("A flash of light shines through your eyes blinding you!",victim,NULL,NULL,TO_CHAR);
        act("A flash of light shines through $n's eyes blinding $m!",victim,NULL,NULL,TO_ROOM);
        act("You clutch your hands in agony as you fall to the ground!",victim,NULL,NULL,TO_CHAR);
        act("$n clutches $s hands in agony as $e falls to the ground!",victim,NULL,NULL,TO_ROOM);

        if ( !IS_SET( victim->more, MORE_BLIND ) )
           {
              SET_BIT( victim->more, MORE_BLIND );
              victim->blind_timer = 5;
           }

        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = AERIAL;
        ch->attack_var = BLAST;
        hurt_person( ch, victim, 500000, TRUE );
        return;
     }

  if ( obj_num == 261 )
     {
        CHAR_DATA *mount;

        ch->wait = 0;

        if ( victim->in_room->vnum < 100 )
           {
              stc("You cannot travel to that room\n\r",ch);
              return;
           }

        if ( ch->in_room == victim->in_room )
           {
              stc( "You are already in the same room as your intended victim.\n\r", ch );
              return;
           }

        extract_obj( obj );

        ch->use_timer = 3;
        stop_fighting( ch, TRUE );
        act("You SLAM the #sp#sr#si#ss#sm#n onto the ground as a shimmering vortex appear!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the #sp#sr#si#ss#sm#n onto the ground as a shimmering vortex appear!",ch,NULL,NULL,TO_ROOM);
        act("Before anyone can move, you quickly leap into the vortex as it disappears!",ch,NULL,NULL,TO_CHAR);
        act("Before anyone can move, $n quickly leaps into the vortex as it disappears!",ch,NULL,NULL,TO_ROOM);

        char_from_room( ch );
        char_to_room( ch, victim->in_room );

        do_look( ch, "" );
        act("$n quickly leaps out of a fading vortex as it disappears!",ch,NULL,NULL,TO_ROOM);

        check_traps( ch );
 
        if ( ( mount = ch->mount ) == NULL ) 
           return;

        char_from_room( mount );
        char_to_room( mount, ch->in_room );
     }

  if ( obj_num == 264 )
     {
        if ( ( IS_CLASS(victim, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
             || IS_CLASS(victim,CLASS_DRAGON) )
           {
              send_to_char( "They are immuned to poison.\n\r", ch );
              return;
           }

        extract_obj( obj );

        act("You SLAM the poisonous vial onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the poisonous vial onto the ground as it explodes with divine energy!",ch,NULL,NULL,TO_ROOM);
        act("A cloud of deadly fumes rise from the ground covering you.",victim,NULL,NULL,TO_CHAR);
        act("A cloud of deadly fumes rise from the ground covering $n.",victim,NULL,NULL,TO_ROOM);
        act("You cough loudly as the fumes fills your lungs!",victim,NULL,NULL,TO_CHAR);
        act("$n coughs loudly as the fumes fills $s lungs!",victim,NULL,NULL,TO_ROOM);

        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = AERIAL;
        ch->attack_var = HIT;

        if ( !IS_SET( victim->more, MORE_POISON ) )
           SET_BIT( victim->more, MORE_POISON );

        if ( IS_CLASS( victim, CLASS_DUERGAR ) )
           victim->hit = UMIN( victim->max_hit, victim->hit + ( 500000 * UMAX( 1, victim->pkill / 10 ) ) );
           else hurt_person( ch, victim, 500000, TRUE );

        return;
     }

  if ( obj_num == 260 )
     {
        extract_obj( obj );

        act("You SLAM the ice trap onto the ground as it explodes with shivering energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the ice trap onto the ground as it explodes with shivering energy!",ch,NULL,NULL,TO_ROOM);
        act("Icy crystals suddenly encase $n's body as $e grimaces in pain.",victim,NULL,NULL,TO_ROOM);
        act("Icy crystals suddenly encase your body as you grimace in pain.",victim,NULL,NULL,TO_CHAR);

        if ( !IS_SET( victim->more, MORE_FROZEN ) && !is_martial( victim ) )
           {
              SET_BIT( victim->more, MORE_FROZEN );
              victim->freeze_timer = number_range( 1, 2 );
           }

        ch->ele_attack = ATTACK_ICE;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = LOW;
        ch->attack_var = BLAST;
        hurt_person( ch, victim, 500000, TRUE );

        return;
     }

  if ( obj_num == 259 )
     {
        extract_obj( obj );

        act("You SLAM the mana trap onto the ground as it explodes with a mysterious energy!",ch,NULL,NULL,TO_CHAR);
        act("$n SLAMS the mana trap onto the ground as it explodes with a mysterious energy!",ch,NULL,NULL,TO_ROOM);
        act("Your body warps out of existence as you feel your spiritful energy being drained.",victim,NULL,NULL,TO_CHAR);
        act("$n's body warps out of existence as $e feels $s spiritful energy being drained.",victim,NULL,NULL,TO_ROOM);

        ch->mana = UMIN( ch->max_mana, ch->mana + ( ch->max_mana * 0.1 ) );

        ch->ele_attack = ATTACK_DARK;
        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_focus = AERIAL;
        ch->attack_var = SOUL;
        hurt_person( ch, victim, 500000, TRUE );

        return;
     }
}

void do_spiritform( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_HUNTER) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET(ch->more3, MORE3_SPIRIT) )
      {
         stc("You go back into your human form.\n\r",ch);
         REMOVE_BIT(ch->more3, MORE3_SPIRIT);
         return;
      }

   SET_BIT(ch->more3, MORE3_SPIRIT);
   stc("#wYou #Crelease your #yinner#C spirit!#n\n\r",ch);
   ch->wait = 12;
}
