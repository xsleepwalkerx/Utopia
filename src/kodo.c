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

bool is_bow( CHAR_DATA *ch )
{
   OBJ_DATA *obj;

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 423 ) )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 423 ) )
         if ( ( ( obj = get_eq_char( ch, WEAR_THIRD ) ) == NULL ) || ( obj->pIndexData->vnum != 423 ) )
            if ( ( ( obj = get_eq_char( ch, WEAR_FOURTH ) ) == NULL ) || ( obj->pIndexData->vnum != 423 ) )
               {
                  send_to_char( "You need a Wooden Bow to utilize this technique.\n\r", ch );
                  return FALSE;
               }

   return TRUE;
}

void do_kodocreate( CHAR_DATA *ch, char *argument )
{
   char	 arg [MAX_INPUT_LENGTH];
   int cost = 1, vnum;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_KODO) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Syntax: kodocreate <item>\n\r", ch );
         send_to_char( "items are: armour(1), leggings(1), sandals(1), bow (1), helmet (1), bracer(1),\n\r", ch );
         send_to_char( "           kimono(1), gloves(1), ring(1), necklace (1).\n\r", ch );
         return;
      }

   if ( !str_cmp(arg,"armour") )
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
	else if ( !str_cmp(arg,"kimono") )
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

void do_arrowlearn( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  int cost;
  char buf[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  cost = 1000000;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     {
       centre_text("#w<<<-~-~-(<#c Arrows of life #w>)-~-~->>>#n",ch);
       centre_text("#c\\^<>^/#n",ch);
       centre_text("#c \\/ #n",ch);
       send_to_char("\n\r",ch);
 	 sprintf(buf,"Arrows of #CIce#n: %d\n\r", ch->arrowab[AICE]);
       send_to_char(buf,ch);
 	 sprintf(buf,"Arrows of #gPoison#n: %d\n\r", ch->arrowab[APOISON]);
       send_to_char(buf,ch);
 	 sprintf(buf,"Arrows of #rFire#n: %d\n\r", ch->arrowab[AFIRE]);
       send_to_char(buf,ch);
       centre_text("#w<<<-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~->>>#n\n",ch);
       return;
    }

 if ( !str_cmp(arg,"fire") )
    {
	if ( ch->arrowab[AFIRE] >= 3 )
	{
	    send_to_char("You have already learnt all of the arrows of fire.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each arrow technique costs 1 million exp to learn Kodo Master.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->arrowab[AFIRE]++;
	ch->exp -= cost;
	sprintf(buf,"Your experience of the arrows of fire improve.\n\r");
	send_to_char(buf,ch);
    }
    else if ( !str_cmp(arg,"ice") )
    {
	if ( ch->arrowab[AICE] >= 3 )
	{
	    send_to_char("You have already learnt all of the arrows of ice.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each arrow technique costs 1 million exp to learn Kodo Master.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->arrowab[AICE]++;
	ch->exp -= cost;
	sprintf(buf,"Your experience of the arrows of ice improve.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"poison"))
    {
	if ( ch->arrowab[APOISON] >= 3 )
	{
	    send_to_char("You have already learnt all of the arrows of poison.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each arrow technique costs 1 million exp to learn Kodo Master.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->arrowab[APOISON]++;
	ch->exp -= cost;
	sprintf(buf,"Your experience of the arrows of poison improve.\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("To improve your knowledge, type: arrowlearn #RFire#e/#bIce#e/#gPoison#e.\n\r",ch);
}

void do_hells_barrage( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 1500000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }
   
  if ( !is_bow( ch ) )
     return;

  if ( ch->move < 500000 )
     {
	  send_to_char("You don't have enough energy to do this.\n\r",ch);
	  return;
     }

  one_argument( argument, arg);

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you wish to feel the burning hell?\n\r", ch );
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

  if ( IS_SET(ch->more, MORE_BOMB) )
     {
        send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  ch->move -= 500000;
  ch->wait = 8;
  SET_BIT(ch->more, MORE_BOMB);

  act("You ready your bow with the barrage of #rfire#n arrows.",ch,NULL,NULL,TO_CHAR);
  act("$n readies $s bow prepared to strike!",ch,NULL,NULL,TO_ROOM);
  act("Drawing your bow and pulling it tight into your shoulders you softly draw the string tightly behind you.",ch,NULL,NULL,TO_CHAR);
  act("$n draws $s bow and softly draws the string.",ch,NULL,NULL,TO_ROOM);
  act("You say '#yI was always taught to kill my enemies in the fastest most PaINfuL way!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says '#yI was always taught to kill my enemies in the fastest most PaINfuL way!#n'",ch,NULL,NULL,TO_ROOM);
  act("With great accuracy and with one final look at your enemy you pull back on your#rKodo #0bow#n.",ch,NULL,NULL,TO_CHAR);
  act("$n stares and pulls back even further on $s bow.",ch,NULL,NULL,TO_ROOM);
  act("You scream '#r#zHELL'S BARRAGE!!#n' as you UnLEaSH ten flaming arrows directly into $N!",ch,NULL,victim,TO_CHAR);
  act("$n screams '#r#zHELL'S BARRAGE!!#n' as they UnLEaSH ten flaming arrows into $N!",ch,NULL,victim,TO_NOTVICT);
  act("$n screams '#r#zHELL'S BARRAGE!!#n' as they UnLEaSH ten flaming arrows into you!",ch,NULL,victim,TO_VICT);
  act("Your victim staggers around the room dazed and alight from the barrage of fire arrows you viciously ploughed into them.",ch,NULL,NULL,TO_CHAR);
  act("You stagger around dazed and alight from the barrage of fire arrows viciously ploughed into you.",ch,NULL,victim,TO_VICT);
  act("$N staggers around dazed and light from the barrage of fire arrows viciously ploughed into them.",ch,NULL,victim,TO_NOTVICT);

  victim->agg = 100;
  hurt_person(ch,victim,dam,TRUE);
}

void do_fire_arrow( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 600000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
       send_to_char("Huh?\n\r",ch);
       return;
     }

  if ( !is_bow( ch ) )
     return;

  if ( ch->arrowab[AFIRE] < 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->move < 100000)
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

  if ( is_safe(ch,victim) )
     return;

  if ( ch->in_room != victim->in_room && IS_SET(ch->in_room->room_flags, ROOM_LUMICA) )
     {
        act("You can't attack other people from afar in this safe haven due to your honour.",ch,NULL,NULL,TO_CHAR);
        return;
     }

  if ( !IS_AFFECTED(victim, AFF_FLAMING) )
     SET_BIT(victim->affected_by, AFF_FLAMING);

  ch->move -= 100000;
  ch->wait = 18;
  act("Your #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #rFlAmInG#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #rFlAmInG#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #rFlAmInG#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #rFlAmInG#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #rFlAmInG#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #rFlAmInG#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #rFlAmInG#n arrows strike you hard!",ch,NULL,victim,TO_VICT);

  ch->ele_attack = ATTACK_FIRE;
  victim->agg += 30;
  hurt_person(ch,victim,dam,TRUE);
}

void do_detonative_arrow (  CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 500000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
        send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !is_bow( ch ) )
     return;

  if ( ch->arrowab[AFIRE] < 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->move < 175000 )
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
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
     }

  if ( is_safe(ch,victim) )
     return;
   
  ch->move -= 175000;
  ch->wait = 18;

  act("You pull a potentially lethal fire-detonator arrow onto your bow.",ch,NULL,NULL,TO_CHAR);
  act("$n pulls a potentially lethal fire-detonator arrow onto $s bow.",ch,NULL,NULL,TO_ROOM);
  act("You pull your string back tight and UnLEaSH the entire power of the arrow.",ch,NULL,NULL,TO_CHAR);
  act("$n pulls $s string back tight and UnLEaSHeS the entire power of the arrow.",ch,NULL,NULL,TO_ROOM);
  act("You smile wildly as the detonative arrow #0#zEXPLODES#n right before $N eyes.",ch,NULL,victim,TO_CHAR);
  act("$n smiles wildly as the detonative arrow #0#zEXPLODES#n right before $N eyes.",ch,NULL,victim,TO_NOTVICT);
  act("$n smiles wildly as the detonative arrow #0#zEXPLODES#n right before your eyes!",ch,NULL,victim,TO_VICT);
  act("$N screams as $E feels $S is being burnt alive!",ch,NULL,victim,TO_CHAR);
  act("$N screams as $E feels $S is being burnt alive!",ch,NULL,victim,TO_NOTVICT);
  act("You scream as you feel you are being burnt alive!",ch,NULL,victim,TO_VICT);
  act("$N screams as $E feels $S is being burnt alive!",ch,NULL,victim,TO_CHAR);
  act("$N screams as $E feels $S is being burnt alive!",ch,NULL,victim,TO_NOTVICT);
  act("You scream as you feel you are being burnt alive!",ch,NULL,victim,TO_VICT);
  act("$N screams as $E feels $S is being burnt alive!",ch,NULL,victim,TO_CHAR);
  act("$N screams as $E feels $S is being burnt alive!",ch,NULL,victim,TO_NOTVICT);
  act("You scream as you feel you are being burnt alive!",ch,NULL,victim,TO_VICT);

  if ( number_percent() > 50 )
     dam *= 1.5;

  ch->ele_attack = ATTACK_FIRE;
  victim->agg += 20;
  hurt_person(ch,victim,dam,TRUE);
}

void do_explosive_arrow (  CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 500000;
 
  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !is_bow( ch ) )
     return;

  if ( ch->arrowab[AFIRE] < 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->move < 150000 )
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
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
     }

  if ( is_safe(ch,victim) )
     return;

  if ( !IS_SET( victim->more, MORE_BLIND ) )
     {
        SET_BIT( victim->more, MORE_BLIND );
        victim->blind_timer = 10;
     }

  ch->move -= 150000;
  ch->wait = 12;

  act("You tug your explosive arrow into place...",ch,NULL,NULL,TO_CHAR);
  act("$n tugs $s explosive arrow into place...",ch,NULL,NULL,TO_ROOM);
  act("You draw back on the string of your bow and take aim.",ch,NULL,NULL,TO_CHAR);
  act("$n draws back on the string of $s bow and takes aim.",ch,NULL,NULL,TO_ROOM);
  act("You examine your victim waiting for the right time to strike with cool accuracy.",ch,NULL,NULL,TO_CHAR);
  act("$n examins $s victim waiting for the right time to strike with cool accuracy.",ch,NULL,NULL,TO_ROOM);
  act("You mutter under your breath '#yGod said let there be light.....#n'",ch,NULL,NULL,TO_CHAR);
  act("$n mutters under $s breath '#yGod said let there be light.....#n'",ch,NULL,NULL,TO_ROOM);
  act("You lower your head in shame as you say '#yBut I!.. I am the destroyer of the LIGHT!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n lowers $s head in shame as $e says '#yBut I!.. I am the destroyer of the LIGHT!#n'",ch,NULL,NULL,TO_ROOM);
  act("You suddenly scream like a war cry '#rExPloSive ARroW!!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n suddenly screams like a war cry '#rExPloSive ARroW!!#n'",ch,NULL,NULL,TO_ROOM);
  act("You let go of your arrow as it flies towards $N and hits them with a EaRtH ShAtTeRiNg BANG!",ch,NULL,victim,TO_CHAR);
  act("$n lets go of $s arrow as it flies towards $N and hits them with a EaRtH ShAtTeRiNg BANG!",ch,NULL,victim,TO_NOTVICT);
  act("$n lets go of $s arrow as it flies towards you and hits you with a EaRtH ShAtTeRiNg BANG!",ch,NULL,victim,TO_VICT);
  act("You snicker to yourself as $N can't bare but scream in unbearable pain!",ch,NULL,victim,TO_CHAR);
  act("$n snickers to $mself as $N can't bare but scream in unbearable pain!",ch,NULL,victim,TO_NOTVICT);
  act("$n snickers to $mself as you can't bare but scream in unbearable pain!",ch,NULL,victim,TO_VICT);

  ch->ele_attack = ATTACK_FIRE;
  victim->agg += 20;
  hurt_person(ch,victim,dam,TRUE);
}

void do_poison_arrow (  CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 550000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !is_bow( ch ) )
     return;

  if ( ch->arrowab[APOISON] < 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->move < 100000)
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

  if ( is_safe(ch,victim) )
     return;

  if ( ch->in_room != victim->in_room && IS_SET(ch->in_room->room_flags, ROOM_LUMICA) )
     {
        act("You can't attack other people from afar in this safe haven due to your honour.",ch,NULL,NULL,TO_CHAR);
        return;
     }

  if ( !IS_SET( victim->more, MORE_POISON ) )
     SET_BIT( victim->more, MORE_POISON );

  ch->move -= 100000;
  ch->wait = 18;

  act("Your #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #gPoIsOn#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #gPoIsOn#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #gPoIsOn#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #gPoIsOn#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #gPoIsOn#n arrows strike you hard!",ch,NULL,victim,TO_VICT);
  act("Your #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_CHAR);
  act("$n #gPoIsOn#n arrows strike $N hard!",ch,NULL,victim,TO_NOTVICT);
  act("$n #gPoIsOn#n arrows strike you hard!",ch,NULL,victim,TO_VICT);

  victim->agg += 30;
  hurt_person(ch,victim,dam,TRUE);
}

void do_poison_confusion (  CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 500000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !is_bow( ch ) )
     return;

  if ( ch->arrowab[APOISON] < 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->mana < 100000)
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

  if ( !IS_SET( victim->more, MORE_HEX ) )
     SET_BIT( victim->more, MORE_HEX );

  if ( number_percent() > 70 && !IS_SET( victim->more, MORE_CONFUSION ) )
     SET_BIT( victim->more, MORE_CONFUSION );

  ch->mana -= 100000;
  ch->wait = 18;

  act("You lather up your arrow with a specialist poison.",ch,NULL,NULL,TO_CHAR);
  act("$n lathers up $s arrow with a specialist poison.",ch,NULL,NULL,TO_ROOM);
  act("You carefully place your arrow onto your bow and pull back.",ch,NULL,NULL,TO_CHAR);
  act("$n carefully places $s arrow onto $s bow and pulls back.",ch,NULL,NULL,TO_ROOM);
  act("You take aim and whisper through the trees '#yMother of confusion..#n'",ch,NULL,NULL,TO_CHAR);
  act("$n takes aim and whispers through the forestry '#yMother of confusion..#n'",ch,NULL,NULL,TO_ROOM);
  act("You sneer as you mutter '#yGuide my arrow..#n'",ch,NULL,NULL,TO_CHAR);
  act("$n sneers as $e mutters '#yGuide my arrow..#n'",ch,NULL,NULL,TO_ROOM);
  act("You then release your arrow and it PoUnDs $N in the chest!",ch,NULL,victim,TO_CHAR);
  act("$n then releases $s arrow and it PoUnDs $N in the chest!",ch,NULL,victim,TO_NOTVICT);
  act("$n then releases $s arrow and it PoUnDs you in the chest!",ch,NULL,victim,TO_VICT);
  act("You then release your arrow and it PoUnDs $N in the chest!",ch,NULL,victim,TO_CHAR);
  act("$n then releases $s arrow and it PoUnDs $N in the chest!",ch,NULL,victim,TO_NOTVICT);
  act("$n then releases $s arrow and it PoUnDs you in the chest!",ch,NULL,victim,TO_VICT);
  act("You then release your arrow and it PoUnDs $N in the chest!",ch,NULL,victim,TO_CHAR);
  act("$n then releases $s arrow and it PoUnDs $N in the chest!",ch,NULL,victim,TO_NOTVICT);
  act("$n then releases $s arrow and it PoUnDs you in the chest!",ch,NULL,victim,TO_VICT);

  victim->agg += 20;
  hurt_person(ch,victim,dam,TRUE);
}

void do_poison_paralyse( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam = 500000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_KODO) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !is_bow( ch ) )
     return;

  if ( ch->arrowab[APOISON] < 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( ch->mana < 175000)
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
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
     }

  if ( is_safe(ch,victim) )
     return;

  if ( !IS_SET( victim->more2, MORE2_NOLEGS ) )
     SET_BIT( victim->more2, MORE2_NOLEGS );

  ch->mana -= 175000;
  ch->wait = 18;

  act("You slide the paralysing arrow into position and you draw backwards.",ch,NULL,NULL,TO_CHAR);
  act("$n slides the paralysing arrow into position and draws backwards.",ch,NULL,NULL,TO_ROOM);
  act("You take your aim and whisper '#ysuffer the terror..#n'",ch,NULL,NULL,TO_CHAR);
  act("$n takes aim and whispers '#ysuffer the terror..#n'",ch,NULL,NULL,TO_ROOM);
  act("You then let your arrow fly as it SlIcEs through the air like a KnIfE",ch,NULL,NULL,TO_CHAR);
  act("$n lets $s arrow fly and it SlIcEs through the air like a KnIfE",ch,NULL,NULL,TO_ROOM);
  act("You smirk to yourself and whisper '#yParalyse them my Poison..#n'",ch,NULL,NULL,TO_CHAR);
  act("$n smirks to $mself and they whisper '#yParalyse them my Poison..#n'",ch,NULL,NULL,TO_ROOM);
  act("The arrow suddenly explodes and a great #ggreen#n light eminates from it",ch,NULL,NULL,TO_CHAR);
  act("The arrow suddenly explodes and a great #ggreen#n light eminates from it",ch,NULL,NULL,TO_ROOM);
  act("You cackle as the shrapnel from the exploding arrow ImPaLeS $N to the ground!",ch,NULL,victim,TO_CHAR);
  act("$n cackles as the shrapnel from the exploding arrow ImPaLeS $N to the ground!",ch,NULL,victim,TO_NOTVICT);
  act("$n cackles as the shrapnel from the exploding arrow ImPaLeS you to the ground!",ch,NULL,victim,TO_VICT);

  victim->agg += 20;
  hurt_person(ch,victim,dam,TRUE);
}

