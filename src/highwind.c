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

void do_higharmor( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int cost, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    cost = 1;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: higharmor <item>\n\r", ch );
	send_to_char( "items are: spear (1), breastplate (1), boots (1), gauntlets (1), leggings (1)\n\r", ch );
	send_to_char( "           robe (1), helmet (1), bracer (1), ring (1), pendant (1)\n\r", ch );
	send_to_char( "           belt (1), sleeves (1), cigarette (1)", ch );
	return;
    }

	if (!str_cmp(arg,"spear"))
    {
       vnum = 122;
    }
    else if (!str_cmp(arg,"breastplate"))
            {
               vnum = 111;
            }
    else if (!str_cmp(arg,"boots"))
            {
               vnum = 112;
            }
    else if (!str_cmp(arg,"gauntlets"))
            {
               vnum = 113;
            }
    else if (!str_cmp(arg,"leggings"))
            {
               vnum = 114;
            }
    else if (!str_cmp(arg,"helmet"))
            {
               vnum = 116;
            }
    else if (!str_cmp(arg,"robe"))
            {
               vnum = 115;
            }
    else if (!str_cmp(arg,"bracer"))
            {
               vnum = 117;
            }
    else if (!str_cmp(arg,"ring"))
            {
               vnum = 118;
            }
    else if (!str_cmp(arg,"pendant"))
            {
               vnum = 119;
            }
    else if (!str_cmp(arg,"belt"))
            {
               vnum = 120;
            }
    else if (!str_cmp(arg,"sleeves"))
            {
               vnum = 121;
            }
	else if (!str_cmp(arg,"cigarette"))
	{
               vnum = 438;
	}
    else {
            do_higharmor(ch,"");
            return;
         }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold piece#n to make this item.\n\r", cost);
	    send_to_char(arg,ch);
          return;
       }

    ch->gold -= cost;

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
       {
	    send_to_char("Missing object, please inform Gogeta.\n\r",ch);
	    return;
       }

    obj = create_object(pObjIndex, 60);
    obj_to_char(obj, ch);
    act("You chew on your cigarette as $p appears, %£*^ happens.",ch,obj,NULL,TO_CHAR);
    act("$n chews on there cigarette as $p appears in $s hands. £*^& happens.",ch,obj,NULL,TO_ROOM);
}

void perform_jump( CHAR_DATA *ch )
{
    CHAR_DATA *victim = ch->binding;

    ch->binding = NULL;

    if ( victim == NULL || victim->in_room == NULL || ch->in_room->area != victim->in_room->area )
       {
          act("Going downwards you realise there is no stopping you from falling into the ground, '#yCrap!#n' you mutter.",ch,NULL,NULL,TO_CHAR);
          act("Going downwards $n realises there is no stopping $m from falling into the ground, '#yCrap!#n' $e mutters.",ch,NULL,NULL,TO_ROOM);
          return;
       }

    if ( victim->in_room != ch->in_room )
       {
          stop_fighting( ch, TRUE );
          char_from_room( ch );
          char_to_room( ch, victim->in_room );
       }

    act("Going downwards you realise there is no stopping you from falling straight into $N, '#yCrap!#n' you mutter.",ch,NULL,victim,TO_CHAR);
    act("Going downwards $n realises there is no stopping $m from falling straight into you, '#yCrap!#n' $e mutters.",ch,NULL,victim,TO_VICT);
    act("Going downwards $n realises there is no stopping $m from falling straight into $N '#yCrap!#n' $e mutters.",ch,NULL,victim,TO_NOTVICT);

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = AERIAL;
    ch->attack_var = PIERCE;
    hurt_person( ch, victim, 750000, TRUE );
}

void do_jump(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
    	    stc("Who do you wish to jump into?\n\r", ch);
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe(ch, victim) )
       return;

    stc("You jump up into the air and prepare for a crash landing spear first.\n\r", ch);
    act("$n jumps up into the air and prepares for a crash landing spear first.", ch, NULL, NULL, TO_ROOM);

    ch->wait = 12;
    ch->binding = victim;
    perform_jump( ch );
}

void perform_ujump( CHAR_DATA *ch )
{
    CHAR_DATA *victim = ch->binding;

    ch->binding = NULL;

    if ( victim == NULL || victim->in_room == NULL )
       {
          act("Going downwards you realise there is no stopping you from falling into the ground, '#yCrap!#n' you mutter.",ch,NULL,NULL,TO_CHAR);
          act("Going downwards $n realises there is no stopping $m from falling into the ground, '#yCrap!#n' $e mutters.",ch,NULL,NULL,TO_ROOM);
          return;
       }

    if ( victim->in_room != ch->in_room )
       {
          char_from_room( ch );
          char_to_room( ch, victim->in_room );
       }

    stc("\n\rYou sore through the air like a stiff board and then land spear first into your victim\n\r",ch);
    act("$n sores through the air like a stiff board and lands spear first into $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n sores through the air like a stiff board and lands spear first into you.",ch,NULL,victim,TO_VICT);
    act("Twirling your spear around, you dash towards $N thrusting it through $S stomach.",ch,NULL,victim,TO_CHAR);
    act("Twirling $s spear around, $n dashes towards you thrusting it through your stomach.",ch,NULL,victim,TO_VICT);
    act("Twirling $s spear around, $n dashes towards $N thrusting it through $S stomach.",ch,NULL,victim,TO_NOTVICT);
    act("You then punch $N hard in the chest RiPpInG your spear upwards $S body.",ch,NULL,victim,TO_CHAR);
    act("$n then punches you hard in the chest RiPpInG $s spear upwards your body.",ch,NULL,victim,TO_VICT);
    act("$n then punches $N hard in the chest RiPpInG $s spear upwards $S body.",ch,NULL,victim,TO_NOTVICT);
    act("The air DEAFENS as you swiftly spin your spear through $N waist, making $M stagger.",ch,NULL,victim,TO_CHAR);
    act("The air DEAFENS as $n swiftly spins $s spear through your waist, making you stagger.",ch,NULL,victim,TO_VICT);
    act("The air DEAFENS as $n swiftly spins $s spear through $N waist, making $M stagger.",ch,NULL,victim,TO_NOTVICT);
    act("You say proudly '#yRemember the name Highwind! But it ain't got £*%^ all to do with YOU!#n'.",ch,NULL,NULL,TO_CHAR);
    act("$n says proudly '#yRemember the name Highwind! But it ain't got £*%^ all to do with YOU!#n'.",ch,NULL,NULL,TO_ROOM);
    victim->agg = 100;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = UPPERCUT;
    ch->attack_var = SLASH;
    hurt_person( ch, victim, 1500000, TRUE );

    if ( ch->in_room == NULL || ch->position <= POS_STUNNED )
       return;

    act("You then light up another cigarette and smile.",ch,NULL,NULL,TO_CHAR);
    act("$n then lights up another cigarette and smiles.",ch,NULL,NULL,TO_ROOM);
}

void perform_hjump( CHAR_DATA *ch )
{
   CHAR_DATA *vch, *vch_next;
   bool pre_room;
   int direction;
   int count;

   act("Floating in mid-air you throw the #CVenus Gospel#n onto your enemies!",ch,NULL,NULL,TO_CHAR);
   act("Floating in mid-air $n throws the #CVenus Gospel#n onto $s enemies!",ch,NULL,NULL,TO_ROOM);
   act("The #CVenus Gospel#n begins to shake as you say '#yYeah shake it like a stripper baby!#n'",ch,NULL,NULL,TO_CHAR);
   act("The #CVenus Gospel#n begins to shake as $n says '#YYeah shake it like a stripper baby!#n'",ch,NULL,NULL,TO_ROOM);
   act("Suddenly a bright blue light fills the room, blinding everyone as you say '#yYeah! What an explosion, that felt good#n'",ch,NULL,NULL,TO_CHAR);
   act("Suddenly a bright blue light fills the room, blinding everyone as $n says '#yYeah! What an explosion, that felt good#n'",ch,NULL,NULL,TO_ROOM);
   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("You are blasted away from the scene!",vch,NULL,NULL,TO_CHAR);
      act("$n is blasted away from the scene!",vch,NULL,NULL,TO_ROOM);

      if ( !IS_SET( vch->more, MORE_BLIND ) )
         {
            SET_BIT( vch->more, MORE_BLIND );
            vch->blind_timer = 6;
         }

      count = 0;
      direction = number_range( 0, 3 );

      do
      {
         pre_room = special_slam( vch, direction );
         count++;
      }
      while ( count < 5 && pre_room != TRUE );

      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = LOW;
      ch->attack_var = BLAST;
      hurt_person( ch, vch, 2000000, TRUE );

      if ( vch != NULL && vch->in_room != NULL && vch->position > POS_STUNNED )
         stop_fighting( vch, TRUE );
   }

   REMOVE_BIT( ch->arena, AFF2_DM );
}

void update_jumping( CHAR_DATA *ch )
{
   ch->ctimer[8]--;

   if ( ch->ctimer[8] == 0 )
      {
         switch( ch->special_id )
         {
            default:
            case 1: perform_jump( ch ); break;
            case 2: perform_ujump( ch ); break;
            case 3: perform_hjump( ch ); break;
         }
      }
}

void do_ultra_jump( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch,CLASS_DRAGOON))
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char("You don't feel like dubbing the cigarette to use this technique yet.\n\r",ch);
          return;
       }

    if ( ch->ctimer[1] > 0 )
       {
          send_to_char( "You are still recovering from the last one.\n\r", ch );
          return;
       }

    if (ch->move < 1000000)
	 {
	    stc("You are too tired to do this!\n\r",ch);
	    return;
	 }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
    	    stc("Who do you wish to jump into?\n\r", ch);
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
           if ( ( victim = get_char_area( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
       }

    if ( is_safe(ch, victim) )
       return;

    stc("Your cigarette drops from your mouth as you dash across the room.\n\r",ch);
    act("$n's cigarette drops from $s mouth as $e dashes across the room.",ch,NULL,NULL,TO_ROOM);
    stc("You jump high into the heavens shouting '#yI'm too old for this £*£%#n'.\n\r\n\r",ch);
    act("$n jumps high into the heavens shouting '#yI'm too old for this £*£%#n'.\n\r",ch,NULL,NULL,TO_ROOM);

    stop_fighting( ch, TRUE );
    ch->binding = victim;
    ch->ctimer[1] = 6;
    ch->special_id = 2;
    ch->move -= 1000000;

    if ( ch->strike_timer > 0 )
       perform_ujump( ch );
       else ch->ctimer[8] = 1;
}

void do_impale( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
 
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
	     send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use to impale?\n\r",ch);
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

    if ( is_safe(ch, victim) ) return;

    if ( IS_SET(victim->more, MORE_IMPALE) )
       {
	    send_to_char( "Their feet is already impaled to the ground.\n\r", ch );
	    return;
       }

    ch->wait = 12;

    if ( number_percent() < 50 )
       {
          act("$N quickly leaps away from your spear as you thrust it into the ground.",ch,NULL,victim,TO_CHAR);
          act("You quickly leap away from $n's spear as $e thrusts it into the ground.",ch,NULL,victim,TO_VICT);
          act("$N  quickly leaps away from $n's spear as $e thrusts it into the ground.",ch,NULL,victim,TO_NOTVICT);
          act("You say in frustration '#yWhat the hell?! You stingy *%^&*%#n'.",ch,NULL,NULL,TO_CHAR);
          act("$n says in frustration '#yWhat the hell?! You stingy *%^&*%#n'.",ch,NULL,NULL,TO_ROOM);
          ch->wait = 6;
          return;
       }

    act("You twirl around your spear viciously as you thrust it down onto $N's foot.",ch,NULL,victim,TO_CHAR);
    act("$n twirls around $s spear viciously as $e thrusts it down onto your foot.",ch,NULL,victim,TO_VICT);
    act("$n twirls around $s spear viciously as $e thrusts it down onto $N foot.",ch,NULL,victim,TO_NOTVICT);
    act("You say triumphantly '#yGuess who's back in town...#n'.",ch,NULL,NULL,TO_CHAR);
    act("$n says triumphantly '#yGuess who's back in town...#n'.",ch,NULL,NULL,TO_ROOM);
    act("You continue '#yAnd guess who stays to see me? How sweet!#n'.",ch,NULL,NULL,TO_CHAR);
    act("$n continues '#yAnd guess who stays to see me? How sweet!#n'.",ch,NULL,NULL,TO_ROOM);
    SET_BIT( victim->more, MORE_IMPALE );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = OVERHEAD;
    ch->attack_var = PIERCE;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_virtual_spears( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 5 )
       {
          send_to_char("You don't feel like dubbing the cigarette to use this technique yet.\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          send_to_char( "You are still recovering from the last one.\n\r", ch );
          return;
       }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Who do you want to use Virtual Spears on?\n\r", ch );
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

    if ( ch->move < 1000000 )
       {
	    send_to_char( "You are too tired to execute this technique.\n\r", ch );
	    return;
       }

    if ( is_safe(ch, victim) ) return;

    ch->move -= 1000000;
    ch->ctimer[2] = 4;

    act("You scream in anger '#yShut up, sit your ass down and drink your goddamn tea, ARRGH! I mean..#n'.",ch,NULL,NULL,TO_CHAR);
    act("$n screams in anger '#yShut up, sit your ass down and drink your goddamn tea, ARRGH! I mean..#n'.",ch,NULL,NULL,TO_ROOM);
    act("You say with a smirk '#yStay still.. And let me beat the £*%^ out of you!#n'.",ch,NULL,NULL,TO_CHAR);
    act("$n says with a smirk '#yStay still.. And let me beat the £*%^ out of you!#n'.",ch,NULL,NULL,TO_ROOM);
    act("You rush to $N as sparkling energy converges on your spear.", ch,NULL,victim,TO_CHAR );
    act("$n rushes to you as sparkling energy converges on $s spear.", ch,NULL,victim,TO_VICT );
    act("$n rushes to $N as sparkling energy converges on $s spear.", ch,NULL,victim,TO_NOTVICT );

    act("You stab $N viciously in the stomach as you brace for your next attack.", ch,NULL,victim,TO_CHAR );
    act("$n stabs you viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_VICT );
    act("$n stabs $N viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_NOTVICT );

    act("You stab $N rapidly in the face as your spear becomes a blur.", ch,NULL,victim,TO_CHAR );
    act("$n stabs you rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_VICT );
    act("$n stabs $N rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_NOTVICT );

    act("You stab $N viciously in the stomach as you brace for your next attack.", ch,NULL,victim,TO_CHAR );
    act("$n stabs you viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_VICT );
    act("$n stabs $N viciously in the stomach as $e braces for the next attack.", ch,NULL,victim,TO_NOTVICT );

    act("You stab $N rapidly in the face as your spear becomes a blur.", ch,NULL,victim,TO_CHAR );
    act("$n stabs you rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_VICT );
    act("$n stabs $N rapidly in the face as $s spear becomes a blur.", ch,NULL,victim,TO_NOTVICT );

    act("You stab $N viciously in the stomach as you leap back a few feet.", ch,NULL,victim,TO_CHAR );
    act("$n stabs you viciously in the stomach as $e leaps back a few feet.", ch,NULL,victim,TO_VICT );
    act("$n stabs $N viciously in the stomach as $e leaps back a few feet.", ch,NULL,victim,TO_NOTVICT );

    act("You say in anger again '#yGODDAMNIT i'm so *^&&£*#n'. as you drink tea.",ch,NULL,NULL,TO_CHAR);
    act("$n says in anger again '#yGODDAMNIT i'm so *^&&£*#n'. as $e drinks tea.",ch,NULL,NULL,TO_ROOM);

    ch->wait = 6;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = PIERCE;
    hurt_person( ch, victim, 1000000, TRUE );
}

void do_whiskey_break( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    one_argument( argument, arg );

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

    if ( is_safe(ch, victim) ) return;

    act("Drunk as a lord, you suddenly appear out of a dark alleyway",ch,NULL,NULL,TO_CHAR);
    act("Drunk as a lord, $n suddenly appears out of a dark alleyway",ch,NULL,NULL,TO_ROOM);
    act("You then turn towards $N '#yYou like whiskey shonny boy?#n'",ch,NULL,victim,TO_CHAR);
    act("$n then turns toward $N '#yYou like whiskey shonny boy?#n'",ch,NULL,victim,TO_NOTVICT);
    act("$n then turns toward you '#yYou like whiskey shonny boy?#n'",ch,NULL,victim,TO_VICT);
    act("You don't give $N a chance to answer as you say '#yYou don't like whiskey?! Best be off then..#n'",ch,NULL,victim,TO_CHAR);
    act("$n doesn't give $N a chance to answer as $e says '#yYou don't like whiskey?! Best be off then..#n'",ch,NULL,victim,TO_NOTVICT);
    act("$n doesn't give you a chance to answer as $e says '#yYou don't like whiskey?! Best be off then..#n'",ch,NULL,victim,TO_VICT);
    act("You then say in a temper '#yGet outta my £%^&^&' way!!!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n then says in a temper '#yGet outta my £%^&^&' way!!!#n'",ch,NULL,NULL,TO_ROOM);
    act("$N doesn't have a chance to move as you smash the whiskey bottle over $M!",ch,NULL,victim,TO_CHAR);
    act("$N doesn't have a chance to move as $n smashes the whiskey bottle over $M!",ch,NULL,victim,TO_NOTVICT);
    act("You don't have a chance to move as $n smashes the whiskey bottle over you!",ch,NULL,victim,TO_VICT);

    ch->wait = 12;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 100 );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = OVERHEAD;
    ch->attack_var = BLUNT;
    hurt_person( ch, victim, 1000000, TRUE );

    if ( victim != NULL && victim->position > POS_STUNNED && number_percent() > 84 - devil_luck( ch, victim, 10 ) )
       {
          stop_fighting( victim, TRUE );
          victim->position = POS_STUNNED;
       }

    act("You then shout with a drunken cackle '#yTold you the hell so!'",ch,NULL,NULL,TO_CHAR);
    act("$n then shouts with a drunken cackle '#yTold you the hell so!'",ch,NULL,NULL,TO_ROOM);
}

void do_dynamite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    OBJ_DATA *obj;

     if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    subtract_stock( ch, 100 );

    act("You calmly draw out a stick of dynamite and light it with a cigarette",ch,NULL,NULL,TO_CHAR);
    act("$n calmly draws out a stick of dynamite and lights it with a cigarette",ch,NULL,NULL,TO_ROOM);
    act("You then throw the dynamite and smoke your cigarette calmly",ch,NULL,NULL,TO_CHAR);
    act("$n then throws the dynamite and smokes on $s cigarette calmly",ch,NULL,NULL,TO_ROOM);
    act("You smirk and say '#yStart runnin' cos my ^&*% is explosive!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n smirks and says '#yStart runnin' cos my ^&*% is explosive!#n'",ch,NULL,NULL,TO_ROOM);
    act("You smile calmly as the dynamite explodes",ch,NULL,NULL,TO_CHAR);
    act("$n smiles calmly as the dynmiate explodes",ch,NULL,NULL,TO_ROOM);
    act("Then you realise, the dynamite is so strong that your gonna be eaten up by it!",ch,NULL,NULL,TO_CHAR);
    act("Then $n realises, the dynamite is so strong that $e's gonna be eaten up by it!",ch,NULL,NULL,TO_ROOM);
    act("Taking a deep breath of air you scream '#ySHIIIIIIIIIIIIIIIIIIIIIIIIIIIITTTTTTTTTT!#n'",ch,NULL,NULL,TO_CHAR);
    act("Taking a deep breath of air $n screams '#ySHIIIIIIIIIIIIIIIIIIIIIIIIIIIITTTTTTTTTT!#n'",ch,NULL,NULL,TO_ROOM);
    act("You start running away as you shout '#yUh.. Didn't expect it to be this strong#n' as you fly in the air!",ch,NULL,NULL,TO_CHAR);
    act("$n starts running away as $e shouts '#yUh.. Didn't expect it to be this strong#n' as $e flies in the air!",ch,NULL,NULL,TO_ROOM);

    if ( str_cmp( "N", ch->groove ) && str_cmp( "A", ch->groove ) )
       ch->hit += 750000;

    SET_BIT( ch->arena, AFF2_DM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( vch != ch && is_safe( ch, vch ) )
          continue;

       if ( vch != ch )
          {
             if ( ( ( IS_CLASS( vch, CLASS_ANGEL ) || IS_CLASS( vch, CLASS_SHADOW_WAR ) ) && vch->ctimer[8] > 0 )
                  || !str_cmp( "N", vch->groove ) || !str_cmp( "A", vch->groove ) )
                vch->hit = UMAX( 1, vch->hit - 750000 );

             act("You gasp as you get whisked into the air by the explosive blast!",vch,NULL,NULL,TO_CHAR);
             act("$n gasps as $e gets whisked into the air by the explosive blast!",vch,NULL,NULL,TO_ROOM);

             vch->hit -= 250000;
             stop_fighting( vch, TRUE );
             vch->air_timer = number_range( 1, 3 );
             continue;
          }

       stop_fighting( vch, TRUE );
       vch->air_timer = number_range( 1, 2 );
    }
}

void do_hyper_jump( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    subtract_stock( ch, 200 );

    act("You jump into the air arching your back high and aiming the #CVenus Gospel#n like a javelin",ch,NULL,NULL,TO_CHAR);
    act("$n jumps into the air arching $s back high and aiming the #CVenus Gospel#n like a javelin",ch,NULL,NULL,TO_ROOM);
    act("You then yell '#yHyper jump!#n' as you reach for the stars like a true pilot!",ch,NULL,NULL,TO_CHAR);
    act("$n then yells '#yHyper jump!#n' as $e reaches for the stars like a true pilot!",ch,NULL,NULL,TO_ROOM);

    stop_fighting( ch, TRUE );
    ch->special_id = 3;

    if ( ch->strike_timer > 0 )
       perform_hjump( ch );
       else ch->ctimer[8] = 1;
}

void do_dragon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Who do you want to use Dragon on?\n\r", ch );
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

    if ( is_safe(ch, victim) ) return;

    act("You open your arms up to the heavens and beckon a dragon to come from above.",ch,NULL,NULL,TO_CHAR);
    act("$n opens $s arms up to the heavens and beckons a dragon to come from above.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly a Dragon slams down infront of you and you smile.",ch,NULL,NULL,TO_CHAR);
    act("Suddenly a Dragon slams down infront of $n and $e smiles.",ch,NULL,NULL,TO_ROOM);
    act("You say gently '#yLike the woman of my dreams, 'cept your one scaly ^&%**&#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says gently '#yLike the woman of my dreams, 'cept your one scaly ^&%**&#n'",ch,NULL,NULL,TO_ROOM);
    act("You command the Dragon to attack and decimate your attacker",ch,NULL,NULL,TO_CHAR);
    act("$n commands the Dragon to attack and decimate $s attacker",ch,NULL,NULL,TO_ROOM);
    act("You start to beg for the Highwind to stop its attack with the Dragon",victim,NULL,NULL,TO_CHAR);
    act("$n starts to beg for the Highwind to stop its attack with the Dragon",victim,NULL,NULL,TO_ROOM);
    act("You say '#yYou kiddin'? It's a woman.. WITH PMS! Pain in the ass! Forget it man!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says '#yYou kiddin'? It's a woman.. WITH PMS! Pain in the ass! Forget it man!#n'",ch,NULL,NULL,TO_ROOM);
    act("You cackle as the angered Dragon sits down and squashes the life and energy from $N!",ch,NULL,victim,TO_CHAR);
    act("$n cackles as the angered Dragon sits down and squashes the life and energy from $N!",ch,NULL,victim,TO_NOTVICT);
    act("$n cackles as the angered Dragon sits down and squashes the life and energy from you!",ch,NULL,victim,TO_VICT);
    act("You feel refreshed as the life comes back into you",ch,NULL,NULL,TO_CHAR);
    act("$n feels refreshed as the life comes back into $m",ch,NULL,NULL,TO_ROOM);

    ch->wait = 12;
    subtract_stock( ch, 200 );
    SET_BIT( ch->arena, AFF2_DM );
    hit_gain( ch, 1000000 );
    move_gain( ch, 1500000 );

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_focus = AERIAL;
    ch->attack_var = BLUNT;
    hurt_person( ch, victim, 2000000, TRUE );
}

void do_dragon_dive( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    OBJ_DATA *obj;
    int count = 0, num;

    if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
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

    act("You jump high into the air and hang there for a while.",ch,NULL,NULL,TO_CHAR);
    act("$n jumps high into the air and hands there for a while.",ch,NULL,NULL,TO_ROOM);
    act("You then say with a soft laugh '#yThis is gonna be a splash! So hold onto your drawers and don't piss in 'em!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n then says with a soft laugh '#yThis is gonna be a splash! So hold onto your drawers and don't piss in 'em!#n'",ch,NULL,NULL,TO_ROOM);
	
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch,vch) || is_safe(ch,vch) )
          continue;

       if ( ch->ctimer[15] == 0 )
          num = number_range( 3, 6 );
          else num = 1;

       for ( count = 0; count < num; count++ )
       {
          if ( number_percent() > 97 - devil_luck( ch, vch, 5 ) && !IS_SET( vch->cmbt[5], SKL2_AMIND ) && ch->ctimer[15] == 0 )
             {
                act("#w#zYou slam the spear right through $N's entire body causing them to fall back,#n #R#zblood flowing everywhere!#n",ch,NULL,vch,TO_CHAR);
                act("$n slams the spear right through your entire body causing them to fall back, #Rblood flowing everywhere!",ch,NULL,vch,TO_VICT);
                act("$n slams the spear right through $N's entire body causing them to fall back, #Rblood flowing everywhere!",ch,NULL,vch,TO_NOTVICT);

                stop_fighting( vch, TRUE );
                vch->hit = -2000010;
                vch->position = POS_MORTAL;
                count = 6;
             }
             else {
                     act("You slam your spear down and TWIST it viciously into $N!",ch,NULL,vch,TO_CHAR);
                     act("$n slams $s spear down and TWISTS it viciously into you!",ch,NULL,vch,TO_VICT);
                     act("$n slams $s spear down and TWISTS it viciously into $N!",ch,NULL,vch,TO_NOTVICT);
                     ch->attack_type = ATTACK_PHYSICAL;
                     ch->attack_focus = AERIAL;
                     ch->attack_var = PIERCE;

                     hurt_person( ch, vch, 500000, TRUE );

                     if ( vch->in_room == NULL || vch->position <= POS_STUNNED )
                        count = 6;
                  }
       }
    }
}

void do_dragoon_throw( CHAR_DATA *ch, char *argument )
{
   DESCRIPTOR_DATA *d;
   CHAR_DATA *vch, *vch_next;
   OBJ_DATA *obj;
   int dam;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

   if ( get_stock( ch ) < 300 )
      {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
      }

   ch->wait = 12;
   subtract_stock( ch, 300 );

   act("You jump high into the air touching the heavens as you poise the #CVenus Gospel#n",ch,NULL,NULL,TO_CHAR);
   act("$n jumps high into the air touching the heavens as $e poises the #CVenus Gospel#n",ch,NULL,NULL,TO_ROOM);
   act("You start yelling to call your spears forth to your hand!",ch,NULL,NULL,TO_CHAR);
   act("$n starts yelling to call $s spears forth to $s hand!",ch,NULL,NULL,TO_ROOM);
   act("You throw hundreds of spears everywhere yelling '#yEat this ya backstabbin' prick!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n throws hundreds of spears everywhere yelling '#yEat this ya backstabbin' prick!#n'",ch,NULL,NULL,TO_ROOM);
   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("You watch as $N crumbles to dust..",vch,NULL,NULL,TO_CHAR);
      act("$n watches as $N crumbles to dust..",vch,NULL,NULL,TO_ROOM);

      hurt_person( ch, vch, vch->max_hit, TRUE );
   }
   
   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != ch )
	{
          if ( ch->in_room->area != d->character->in_room->area )
             continue;

          if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
             continue;

          if ( !IS_SET( d->character->more, MORE_IMPALE ) )
             SET_BIT( d->character->more, MORE_IMPALE );

          if ( ch->in_room != d->character->in_room )
             d->character->wait += 24;
             else d->character->wait += 12;

          if ( ch->in_room != d->character->in_room )
             act("You hear a loud voice from the distance yelling '#yEat this ya backstabbin' prick!#n'",d->character,NULL,NULL,TO_CHAR);

          act("You crumple over in deep pain as your winded from the spears pinning you down.",d->character,NULL,NULL,TO_CHAR);
          act("$n crumples over in deep pain as $e's winded from the spears pinning $m down.",d->character,NULL,NULL,TO_ROOM);

          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_focus = AERIAL;
          ch->attack_var = PIERCE;
          hurt_person( ch, d->character, 3500000, TRUE );
	}
   }
}

void do_highwind( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_DRAGOON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
       {
          send_to_char( "You need a stabbing weapon for this technique.\n\r", ch );
          return;
       }

    if ( get_stock( ch ) < 300 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    ch->wait = 12;
    subtract_stock( ch, 300 );

    act("You blow a puff of smoke in front of you and summon your trusty Highwind!",ch,NULL,NULL,TO_CHAR);
    act("$n blows a puff of smoke in front of $m and summons $s trusty Highwind!",ch,NULL,NULL,TO_ROOM);
    act("On the side of the Highwind you pat the pretty picture of the bikini clad female.",ch,NULL,NULL,TO_CHAR);
    act("On the side of the Highwind $n pats the pretty picture of the bikini clad female.",ch,NULL,NULL,TO_ROOM);
    act("You then say '#yYou see this baby...? She's gonna blow you...#n'",ch,NULL,NULL,TO_CHAR);
    act("$n then says '#yYou see this baby...? She's gonna blow you...#n'",ch,NULL,NULL,TO_ROOM);
    act("You smile coyly and continue '#y...Blow you away.. eat ^**% and die!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n smiles coyly and continues '#y...Blow you away.. eat ^**% and die!#n'",ch,NULL,NULL,TO_ROOM);
    act("Blowing a final puff on your cigarette you order your crew: '#yDo your worst..#n'",ch,NULL,NULL,TO_CHAR);
    act("Blowing a final puff on $s cigarette $n orders $s crew: '#yDo your worst..#n'",ch,NULL,NULL,TO_ROOM);
    act("You jump into the air leaving the scene as the Highwind opens fire!",ch,NULL,NULL,TO_CHAR);
    act("$n jumps into the air leaving the scene as the Highwind opens fire!",ch,NULL,NULL,TO_ROOM);
    act("You laugh loudly as the Highwind pummels ALL with beams and rockets!",ch,NULL,NULL,TO_CHAR);
    act("$n laughs loudly as the Highwind pummels ALL with beams and rockets!",ch,NULL,NULL,TO_ROOM); 
    SET_BIT( ch->arena, AFF2_DM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch,vch) || is_safe(ch,vch) )
          continue;     

       if ( !IS_ARM_R(vch,LOST_ARM) )
          {
             SET_BIT(vch->loc_hp[3],LOST_ARM);
             SET_BIT(vch->loc_hp[6],BLEEDING_ARM_R);
             make_part(vch,"arm");

             if ( IS_ARM_L(vch,LOST_ARM) && IS_ARM_R(vch,LOST_ARM) )
                {
                   if ( ( obj = get_eq_char( vch, WEAR_ARMS ) ) != NULL )
                      take_item(vch,obj);
                }

             if ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) != NULL )
                take_item(vch,obj);

             if ( ( obj = get_eq_char( vch, WEAR_HANDS ) ) != NULL )
                take_item(vch,obj);

             if ( ( obj = get_eq_char( vch, WEAR_WRIST_R ) ) != NULL )
                take_item(vch,obj);

             if ( ( obj = get_eq_char( vch, WEAR_FINGER_R ) ) != NULL )
                take_item(vch,obj);
          }

       if ( !IS_ARM_L(vch,LOST_ARM) )
          {
             SET_BIT(vch->loc_hp[2],LOST_ARM);
             SET_BIT(vch->loc_hp[6],BLEEDING_ARM_L);
             make_part(vch,"arm");

             if ( IS_ARM_L(vch,LOST_ARM) && IS_ARM_R(vch,LOST_ARM) )
                {
                   if ( ( obj = get_eq_char( vch, WEAR_ARMS ) ) != NULL )
                      take_item(vch,obj);
                }

             if ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) != NULL )
                take_item(vch,obj);
 
             if ( ( obj = get_eq_char( vch, WEAR_HANDS ) ) != NULL )
                take_item(vch,obj);

             if ( ( obj = get_eq_char( vch, WEAR_WRIST_L ) ) != NULL )
                take_item(vch,obj);

             if ( ( obj = get_eq_char( vch, WEAR_FINGER_L ) ) != NULL )
                take_item(vch,obj);
          }

       act("You stand there bewildered as you are pummeled with shots all over your body!",vch,NULL,NULL,TO_CHAR);
       act("$n stands there bewildered as $e is pummeled with shots all over $s body!",vch,NULL,NULL,TO_ROOM);

       if ( IS_SET( vch->more, MORE_PAIN ) )
          SET_BIT( vch->more, MORE_PAIN );

       if ( !IS_SET( vch->more, MORE_BLIND ) )
          {
             SET_BIT( vch->more, MORE_BLIND );
             vch->blind_timer = 4;
          }

       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_focus = AERIAL;
       ch->attack_var = BEAM;
       hurt_person( ch, vch, 4000000, TRUE );
    }
}
