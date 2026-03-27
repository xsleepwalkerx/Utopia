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

void do_gpeace( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
 
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_ANGEL) && !IS_SET( ch->cmbt[7], SKL_GODPEACE ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     victim = ch;
     else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
                return;
             }

  if ( victim->fight_timer > 0 )
     {
        stc("They have a fight timer.\n\r",ch);
        return;
     }

  if ( IS_SET(victim->more, MORE_SAFE) )
     {
        if ( victim != ch )
           {
              stc( "They are already protected by God.n\r", ch );
              return;
           }

        REMOVE_BIT(victim->more, MORE_SAFE);
        act("Time to punish the unbelivers.", victim, NULL, NULL, TO_CHAR);
        act("$n looks wicked.", victim, NULL, NULL, TO_ROOM);
        return;
     }

  SET_BIT(victim->more, MORE_SAFE);
  act("The almighty hand of God protects you from further harm.", victim, NULL, NULL, TO_CHAR);
  act("$n smiles, seems God is on $s side.", victim, NULL, NULL, TO_ROOM);
}

void do_angel_create( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ANGEL) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: holyarmor <item>\n\r", ch );
	send_to_char( "items are: ring (1), collar (1), armor (1), leggings (1), boots (1)\n\r", ch );
	send_to_char( "           sleeves (1), helmet (1), cloak (1), visor (1), gloves (1)\n\r", ch );
	send_to_char( "           bracers (1), spear(1), sword (1)\n\r", ch );
	return;
    }

    cost = 1;

    if (!str_cmp(arg,"ring"))
    {
       vnum = 171;
    }
    else if (!str_cmp(arg,"collar"))
            {
               vnum = 172;
            }
    else if (!str_cmp(arg,"armor"))
            {
               cost = 15;
               vnum = 173;
            }
    else if (!str_cmp(arg,"leggings"))
            {
               vnum = 174;
            }
    else if (!str_cmp(arg,"boots"))
            {
               vnum = 175;
            }
    else if (!str_cmp(arg,"sleeves"))
            {
               vnum = 176;
            }
    else if (!str_cmp(arg,"helmet"))
            {
               vnum = 177;
            }
    else if (!str_cmp(arg,"cloak"))
            {
               vnum = 178;
            }
    else if (!str_cmp(arg,"visor"))
            {
               vnum = 179;
            }
    else if (!str_cmp(arg,"gloves"))
            {
               vnum = 180;
            }
    else if (!str_cmp(arg,"bracers"))
            {
               vnum = 181;
            }
    else if ( !str_cmp(arg,"sword") )
            {
               vnum = 182;
            }
    else if (!str_cmp(arg,"spear") )
            {
               vnum = 183;
            }
    else {
            do_angel_create(ch,"");
            return;
         }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold pieces#n to make this item.\n\r", cost);
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

    if ( vnum == 182 )
       SET_BIT( obj->weapflags, TRUE_SAINT );

    act("You pray to God silently as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$n softly goes to $s knees and pray as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_holyword( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch,*vch_next;
   int direction, range;
   bool pre_room;
   int count;

	if ( IS_NPC(ch) )  return;

	if ( !IS_CLASS(ch, CLASS_ANGEL) )
	{
	   stc("Huh?\n\r",ch);
  	   return;
	}

   if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
      if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
         if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
         {
            send_to_char( "You need a free hand to utilize this technique.\n\r", ch );
            return;
         }

      if ( ch->ctimer[5] > 0 )
      {
	   send_to_char("You are still recovering from the last one.\n\r",ch);
	   return;
      } 

      if ( ch->mana < 1000000 )
      {
	   send_to_char("You don't have enough magical energy for that.\n\r",ch);
	   return;
      } 

      ch->mana -= 1000000;
      ch->ctimer[5] = 4;

	send_to_char("You concentrate a moment.\n\r",ch);
	act("$n stops and concentrates a moment.\n\r",ch,NULL,NULL, TO_ROOM);

      act("You say, '#yAnd God has say to me, 'Believe in Me, My son and the Trinity...'#n'",ch,NULL,NULL,TO_CHAR);
      act("$n says, '#yAnd God has say to me, 'Believe in Me, My son and the Trinity...'#n'",ch,NULL,NULL,TO_ROOM);
      act("You say, '#yWhich falls down the the Holy Word... Faith!#n'",ch,NULL,NULL,TO_CHAR);
      act("$n says, '#yWhich falls down the the Holy Word... Faith!#n'",ch,NULL,NULL,TO_ROOM);
      send_to_char("\n\r#wYou wave your hand in a circular motion as Holy Symbols envelops the entire room.#n\n\r",ch);
      act("\n\r#w$n #wwaves $s hand in a circular motion as Holy Symbols fill the entire room.#n",ch,NULL,NULL, TO_ROOM);
      act("As your beautiful voice fills the air, everyone drops down their weapons in amazement.",ch,NULL,NULL,TO_CHAR);
      act("As $n's beautiful voice fills the air, you drop down their weapons in amazement.",ch,NULL,NULL,TO_ROOM);

      for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next_in_room;

         if ( is_safe( ch, vch ) )
            continue;

         if ( IS_CLASS( vch, CLASS_DEMON ) || vch->alignment < 0 )
            {
               act("You snarl '#yWhat is this?#n' as you feel your skin being ripped apart by a strong wind.",vch,NULL,NULL,TO_CHAR);
               act("$n snarls '#yWhat is this?#n' as $e feels $s skin being ripped apart by a strong wind.",vch,NULL,NULL,TO_ROOM);
               act("You scream '#yNOOOOOOOOOOO!#n' as you are suddenly blasted away!",vch,NULL,NULL,TO_CHAR);
               act("$n screams '#yNOOOOOOOOOOO!#n' as $e is suddenly blasted away!",vch,NULL,NULL,TO_ROOM);

               direction = number_range( 0, 3 );
               range = number_range( 1, 3 );

               do
               {
                  pre_room = special_slam( vch, direction );
                  count++;
               }
               while ( count < range && pre_room != TRUE );

               ch->ele_attack = ATTACK_HOLY;
               ch->attack_focus = AERIAL;
               ch->attack_type = ATTACK_MAGICAL;
               ch->attack_var = SOUL;
               hurt_person( ch, vch, 500000, TRUE );

               if ( vch->in_room == NULL || vch->position <= POS_STUNNED )
                  continue;

               stop_fighting( vch, TRUE );
               vch->position = POS_STUNNED;
               continue;
            }
        
         stop_fighting( vch, TRUE );
      }
}

void do_ether_form( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))		return;

	if ( !IS_CLASS(ch, CLASS_ANGEL) && !IS_SET( ch->cmbt[7], SKL_PLANAR ) )
	{
		stc("Huh?\n\r",ch);
		return;
	}

        if ( IS_CLASS(ch, CLASS_ANGEL) && ch->angelab[SPIRIT] < 1 )
	{
		send_to_char("You haven't yet gained the power to shift your phase of existance.\n\r",ch);
		return;
	}

      if ( ch->fight_timer > 0 )
         {
            stc("Not with a fight timer.\n\r",ch);
            return;
         }

	if (IS_AFFECTED(ch, AFF_ETHEREAL) )
	{
		REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
		send_to_char("You shift your body into phase with the natural plane.\n\r",ch);
	}
	else
	{
		send_to_char("You shift your body out of phase with the natural plane.\n\r",ch);
		SET_BIT(ch->affected_by, AFF_ETHEREAL);
	}
	return;
}

void do_angel_smite( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf1 [MAX_STRING_LENGTH];
	char buf2 [MAX_STRING_LENGTH];
	char buf3 [MAX_STRING_LENGTH];
	char verb [MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	one_argument( argument, arg );

	if (IS_NPC(ch) && ch->pIndexData->vnum != 553)  return;

      if ( !IS_NPC( ch ) )
         {
            if ( !IS_CLASS(ch, CLASS_ANGEL) )
               {
                  send_to_char("Huh?\n\r",ch);
                  return;
               }

            if ( ch->mana < 500000 )
               {
                  send_to_char("You don't have enough magical energy for that.\n\r",ch);
                  return;
               }
         }

   if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
      if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
         if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
         {
            send_to_char( "You need a free hand to utilize this technique.\n\r", ch );
            return;
         }

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
 	   send_to_char("Whom do you wish to smite?\n\r",ch);
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

	if ( is_safe( ch, victim ))		return;

	strcpy(verb, "annihilates");
	  
      act("You raise your arm and hurl a bolt of purity at $N!", ch, NULL, victim, TO_CHAR);
	act("$n raises $s arm and hurls a bolt of purity at you!", ch, NULL, victim, TO_VICT);
	act("$n raises $s arm and hurls a bolt of purity at $N!", ch, NULL, victim, TO_NOTVICT);
	sprintf(buf1,"Your bolt of purity %s $N.",verb);
	sprintf(buf2,"$n's bolt of purity %s you.",verb);
	sprintf(buf3,"$n's bolt of purity %s $N.",verb);
	act(buf1, ch, NULL, victim, TO_CHAR);
	act(buf2, ch, NULL, victim, TO_VICT);
	act(buf3, ch, NULL, victim, TO_NOTVICT);

	act("$N reels from your blow.", ch, NULL, victim, TO_CHAR);
	act("You reel from $n's blow.", ch, NULL, victim, TO_VICT);
	act("$N reels from $n's blow.", ch, NULL, victim, TO_NOTVICT);

      if ( !IS_NPC( ch ) )
         {
            ch->mana -= 500000;
            ch->wait = 12;
         }

      ch->ele_attack = ATTACK_HOLY;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_focus = AERIAL;
      ch->attack_var = SOUL;

      if ( !IS_NPC( ch ) )
         hurt_person( ch, victim, 700000, TRUE );
         else hurt_person( ch, victim, 3500000, TRUE );
}

void do_martyr( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *obj;
        
   if ( IS_NPC( ch ) && ch->pIndexData->vnum != 553 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS( ch, CLASS_ANGEL ) )
            {
               stc("Huh?\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->pIndexData->vnum != 182 )
            if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || obj->pIndexData->vnum != 182 )
               {
                  send_to_char( "You need the Sword of the Heavens to utilize this technique.\n\r", ch );
                  return;
               }
      }

   if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
      if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
         if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
         {
            send_to_char( "You need a free hand to utilize this technique.\n\r", ch );
            return;
         }

   act( "You suddenly twirl your sword in the air as dazzling light sparkles off it.", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly twirls $s sword in the air as dazzling light sparkles off it.", ch, NULL, NULL, TO_ROOM );
   act( "You cackle '#yHeathen... may God forgive you#n' as you RAM the sword into your guts!", ch, NULL, NULL, TO_CHAR );
   act( "$n cackles '#yHeathen... may God forgive you#n' as $e RAMS the sword into $s guts!", ch, NULL, NULL, TO_ROOM );
   act( "You fall onto your knees as shards of very intense light blast from your body.", ch, NULL, NULL, TO_CHAR );
   act( "$n falls onto $s knees as shards of very intense light blast from $s body.", ch, NULL, NULL, TO_ROOM );
   act( "Blood trails down your lips... leaving a grin on your face as the light engulfs all...", ch, NULL, NULL, TO_CHAR );
   act( "Blood trails down $n's lips... leaving a grin on $s face as the light engulfs all...", ch, NULL, NULL, TO_ROOM );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT( ch->affected_by, AFF_POLYMORPH );

   sprintf( buf, "#wThe #yAve#Rn#yger#n", ch->name );

   if ( !IS_NPC( ch ) )
      {
         free_string( ch->morph );
         ch->morph = str_dup( buf );
      }
      else {
              free_string( ch->short_descr );
              ch->short_descr = str_dup( buf );
           }

   if ( !IS_NPC( ch ) )
      {
         ch->ctimer[17] = 7;
         ch->wait = 12;
         subtract_stock( ch, 300 );
      }
      else ch->ctimer[17] = 10;

   act( "You point your powerful blade as you sneer '#ySay your prayers now!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n points $s powerful blade as $e sneers '#ySay your prayers now!#n'", ch, NULL, NULL, TO_ROOM );
}

void do_trueform( CHAR_DATA *ch, char *argument )
{
	char buf [MAX_STRING_LENGTH];
        
	if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 ) return;

      if ( !IS_CLASS(ch, CLASS_ANGEL) )
	{
		stc("Huh?\n\r",ch);
		return;
	}

      if ( ch->ctimer[17] > 0 )
	{
		stc("You are the Avenger... you can't stop now...\n\r",ch);
		return;
	}

      if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
	{
		act( "You change back into human form.", ch, NULL, NULL, TO_CHAR );
		act( "$n changes back into $s human form.", ch, NULL, NULL, TO_ROOM );
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

            if ( !IS_NPC( ch ) )
               {
              	free_string( ch->morph );
                  ch->morph = str_dup( "" );
               }
               else {
                       free_string( ch->short_descr );
                       ch->short_descr = str_dup( "Tyrael" );
                    }

		return; 
	}   
	else
	{
		act( "You unleash your full essence in a blaze of light.", ch, NULL, NULL, TO_CHAR );
		act( "$n unleashes $s full essence in a blaze of light.", ch, NULL, NULL, TO_ROOM );
		sprintf(buf, "#y%s #wthe #LS#werap#Lh#n", ch->name);
		free_string( ch->morph );
		ch->morph = str_dup( buf );
		SET_BIT(ch->affected_by, AFF_POLYMORPH);
		return;
	}
	return;
}

void do_gshield( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
	
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_ANGEL) && !can_do_skill( ch, 22 ) )
	{
         stc("Huh?\n\r",ch);
         return;
	}

   argument = one_argument(argument, arg);
	
   if ( !IS_CLASS(ch, CLASS_ANGEL) )
      victim = ch;
      else if ( ( victim = get_char_room( ch, arg) ) == NULL )
      {
         send_to_char("You MUST choose to be someone's guardian shield.\n\r",ch);
         return;
      }

   if ( IS_NPC(victim) )
      {
         send_to_char("They are not worthy of having a guardian shield!\n\r",ch);
         return;
      }

   if ( ch->guarding > 0 )
      {
         send_to_char("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( victim->position == POS_FIGHTING )
      {
         if ( victim != ch )
            send_to_char("#eThey are already being attacked by the heathens!#n\n\r",ch);
            else send_to_char("#eYou are already being attacked by the heathens!#n\n\r",ch);

         return;
      }

   if ( victim->gshield > 0 )
      {
         send_to_char("Someone is already shielding their spirit.\n\r",ch);
         return;
      }

   if ( victim->binded != NULL )
      {
         send_to_char("Someone is already guarding their spirit.\n\r",ch);
         return;
      }

   send_to_char("You become even more translucent.\n\r",ch);

   if ( ch != victim )
      {
         act("#CYou merge yourself into $N.#n",ch,NULL,victim,TO_CHAR);
         act("#CYou become more transparent as $n's veal covers your defenseless body.#n",ch,NULL, victim, TO_VICT);
         act("#C$N becomes more transparent as $n's veal covers $S defenseless body.#n",ch,NULL, victim, TO_NOTVICT);
         victim->gshield = 12;        
      }
      else {
              act("#CYou become more transparent as you protect your body from the heathens.#n",ch,NULL, NULL, TO_CHAR);
              act("#C$n become more transparent as $e protects $s body from the heathens.#n",ch,NULL, NULL, TO_ROOM);
              ch->gshield = 12;
           }

   ch->guarding = 12 - UMIN( 6, ch->pkill / 2 );
}

void do_gspirit( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
      char arg[MAX_INPUT_LENGTH];
	
	if ( IS_NPC(ch) )  return;

      if ( !IS_CLASS(ch, CLASS_ANGEL) )
	{
         stc("Huh?\n\r",ch);
         return;
	}

      if ( ch->binding != NULL )
      {
		act("#wYou stop guarding $N with your spirit.#n",ch,NULL,ch->binding,TO_CHAR);
		act("#w$n stops guarding you with $s spirit.#n",ch,NULL,ch->binding,TO_VICT);
            ch->binding->binded = NULL;
            ch->binding = NULL;
		return;
      }

      if ( ch->safe_timer > 0 || nogang == 10 )
         {
            stc("You can't use this ability with a safe timer or with group battles out.\n\r",ch);
            return;
         }

      argument = one_argument(argument, arg);
	
	if ( ( victim = get_char_room( ch, arg) ) == NULL )
	{
		send_to_char("You MUST choose to be someone's guardian spirit.\n\r",ch);
		return;
	}

	if ( IS_NPC(victim) )
	{
		send_to_char("They are not worthy of having a guardian spirit!\n\r",ch);
		return;
	}

	if ( ch == victim )
	{
		send_to_char("You already ARE your own guardian spirit!\n\r",ch);
		return;
	}

      if ( victim->binded != NULL )
      {
		send_to_char("Someone is already guarding their spirit.\n\r",ch);
		return;
      }

	send_to_char("You become even more translucent.\n\r",ch);
	act("#wYou merge yourself into $N.#n",ch,NULL,victim,TO_CHAR);
	act("#w$N becomes more transparent as $n's soul covers $S vulnerable body.#n",ch,NULL, victim, TO_NOTVICT);
	act("#wYou become more transparent as $n's soul covers your vulnerable body.#n",ch,NULL, victim, TO_VICT);

      victim->binded = ch;
      ch->binding = victim;

      if ( victim->gshield > 0 )
         {
            victim->gshield = 0;

            if ( victim->attacker != NULL )
               {
                  stc("#yYou are now vulnerable to multiple attacks.#n\n\r",ch);
                  victim->attacker->attacker = NULL;
                  victim->attacker = NULL;
               }
         }
}

void do_angel_spread( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_ANGEL) )
	{
	   stc("Huh?\n\r",ch);
	   return;
	}

   if ( ch->angelab[BODY] < 4 )
	{
 	   send_to_char("You haven't yet gained your wings from god.\n\r",ch);
  	   return;
	}

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION) )
      {
	   send_to_char("Your wings are already spread out.\n\r",ch);
  	   return;
	}

   send_to_char("You spread your wings out, give them a shake and start flying.\n\r",ch);
   act("$n spreads $s wings out, gives them a shake and starts to fly.",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->pcdata->powers[1], DPOWER_LEVITATION);
}

void do_angel_fold( CHAR_DATA *ch, char *argument )
{
   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_ANGEL) )
	{
	   stc("Huh?\n\r",ch);
	   return;
	}

   if ( ch->angelab[BODY] < 4 )
	{
 	   send_to_char("You haven't yet gained your wings from god.\n\r",ch);
  	   return;
	}

   if ( !IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION) )
      {
	   send_to_char("Your wings are already folded.\n\r",ch);
  	   return;
	}

   send_to_char("Your fold your wings up tightly behind your back.\n\r",ch);
   act("$n's wings fold up tightly behind $s back.",ch, NULL,NULL, TO_ROOM);
   REMOVE_BIT(ch->pcdata->powers[1], DPOWER_LEVITATION);
}

void do_enlighten( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char lin[MAX_STRING_LENGTH];
    int inpart = 0;
    int cost = 0;
    int choice = 5;

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_ANGEL) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    sprintf(lin,"#b===============================================================================#n\n\r");

    if ( arg[0] == '\0' )
       {
          stc(" #wSyntax: enlighten <abilities|aura> <option>\n\r#n",ch);
          stc(lin,ch);
          stc("                             #wAbilities of Heaven#n                                   \n\r",ch);
          stc(lin,ch);
          sprintf(buf,"          #wCombat    [#c%s%s%s%s%s#w]          Body     [#c%s%s%s%s%s#w]#n\n\r",
	        ch->angelab[COMBAT] > 0 ? "*" : " ",
	        ch->angelab[COMBAT] > 1 ? "*" : " ",
	        ch->angelab[COMBAT] > 2 ? "*" : " ",
	        ch->angelab[COMBAT] > 3 ? "*" : " ",
	        ch->angelab[COMBAT] > 4 ? "*" : " ",
	        ch->angelab[BODY]  > 0 ? "*" : " ",
              ch->angelab[BODY]  > 1 ? "*" : " ",
              ch->angelab[BODY]  > 2 ? "*" : " ",
              ch->angelab[BODY]  > 3 ? "*" : " ",
              ch->angelab[BODY]  > 4 ? "*" : " " );
          stc(buf, ch);
          sprintf(buf,"          #wMagic     [#c%s%s%s%s%s#w]#n\n\r",
	        ch->angelab[SPIRIT] > 0 ? "*" : " ",
              ch->angelab[SPIRIT] > 1 ? "*" : " ",
              ch->angelab[SPIRIT] > 2 ? "*" : " ",
              ch->angelab[SPIRIT] > 3 ? "*" : " ",
              ch->angelab[SPIRIT] > 4 ? "*" : " " );
          stc(buf, ch);
          stc(lin,ch);
          return;
       }
    
  if ( !str_cmp(arg1, "abilities" ) )
  {
         if ( !str_cmp( arg, "body" ) )
	{ inpart = BODY; choice = 5; }
    else if ( !str_cmp( arg, "magic" ) )
	{ inpart = SPIRIT; choice = 5; }
    else if ( !str_cmp( arg, "combat" ) )
	{ inpart = COMBAT; choice = 5; }
    else
    {
      do_enlighten(ch,"");
      return;
    }
  }
  else {
          do_enlighten(ch,"");
          return;
       }

 if ( !str_cmp(arg1, "abilities" ) )
    {
        if ( ch->angelab[inpart] >= choice )
	     {
             stc("You have already learned all you can in this ability.\n\r",ch);
             return;
           }
    
        if ( ch->exp < cost )
           {
              send_to_char("You do not have enough experience to learn that power.\n\r",ch);
              return;
           }

        ch->angelab[inpart]++;
        ch->exp -= cost;
        send_to_char("Ok.\n\r",ch);
        save_char_obj(ch);
        return; 
    }
}

void do_mana_shield( CHAR_DATA *ch, char *argument )
{
      if ( IS_NPC(ch) )
         return;

      if ( !IS_CLASS(ch, CLASS_ANGEL) )
	{
		stc("Huh?\n\r",ch);
		return;
	}

      if ( ch->angelab[SPIRIT] < 4 )
	{
		send_to_char("You haven't yet gained the power to create a barrier of energy yet.\n\r",ch);
		return;
	}

	if ( IS_SET(ch->more, MORE_MSHIELD) )
	{
		REMOVE_BIT(ch->more, MORE_MSHIELD);
		act("The yellow ball of energy engulfing $n fades away.",ch,NULL,NULL,TO_ROOM);
		act("The yellow ball of energy engulfing you fades away.",ch,NULL,NULL,TO_CHAR);
	}
	else
	{
          if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
             if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
                if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
                   {
                      send_to_char( "You need a free hand to utilize this technique.\n\r", ch );
                      return;
                   }

            if ( ch->mana < ch->max_mana * 0.7 )
               {
                  send_to_char("You don't have enough magical energy for this.\n\r",ch);
                  return;
               }

            if ( IS_AFFECTED(ch, AFF_MANA) )
               {
                  send_to_char("You have too much magical energy for this.\n\r",ch);
                  return;
               }

		send_to_char("You close your eyes as you raise a hand into the air.\n\r",ch);
		act("$n closes $s eyes as $e raises a hand into the air.",ch,NULL,NULL,TO_ROOM);
		act("A yellow ball of energy engulfs $n as he smiles.",ch,NULL,NULL,TO_ROOM);
		act("A yellow ball of energy engulfs you as you smile.",ch,NULL,NULL,TO_CHAR);
		SET_BIT(ch->more, MORE_MSHIELD);
	}
	return;
}

void do_fist_takedown( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
      OBJ_DATA *obj;
      CHAR_DATA *victim;
      int chance = 75;

	one_argument( argument, arg );

      if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 ) 
         return;

      if ( !IS_NPC( ch ) )
         {
            if ( !IS_CLASS(ch, CLASS_ANGEL) )
               {
                  send_to_char("Huh?\n\r",ch);
                  return;
               }

            if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
               if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL )
                  if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL )
                  {
                     send_to_char( "You need a free hand to do this technique.\n\r", ch );
                     return;
                  }

            if ( ch->move < 1500000 )
               {
                  send_to_char("You are too tired to do this technique.\n\r",ch);
                  return;
               }

            chance = 94;
         }

      if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
         {
            send_to_char("Whom do you wish to stun?\n\r",ch);
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

	if ( victim == ch )
	{
		send_to_char("You can't do this to yourself.\n\r",ch);
		return;
	}

	if ( is_safe( ch, victim ))		return;

      if ( !IS_NPC( ch ) )
         ch->move -= 1500000;

      act("Sparkles of energy fizzes of your body as you fly towards $N.",ch,NULL,victim,TO_CHAR);
      act("Sparkles of energy fizzes of $n's body as $e flies towards you.",ch,NULL,victim,TO_VICT);
      act("Sparkles of energy fizzes of $n's body as $e flies towards $N.",ch,NULL,victim,TO_NOTVICT);
      act("Grabbing $N by the collar, you look into $S eyes as you raise $M slightly above you.",ch,NULL,victim,TO_CHAR);
      act("Grabbing you by the collar, $n looks into your eyes as $e raises you slightly above $m.",ch,NULL,victim,TO_VICT);
      act("Grabbing $N by the collar, $n looks into $S eyes as $e raises $M slightly above $m.",ch,NULL,victim,TO_NOTVICT);
      act("You then DRIVE $N onto the ground ViCiOuSlY as the entire area ErUpTs!!!",ch,NULL,victim,TO_CHAR);
      act("$n then DRIVES you onto the ground ViCiOuSlY as the entire area ErUpTs!!!",ch,NULL,victim,TO_VICT);
      act("$n then DRIVES $N onto the ground ViCiOuSlY as the entire area ErUpTs!!!",ch,NULL,victim,TO_NOTVICT);

      if ( is_martial( victim ) )
         {
            act("You suddenly do a reverse flip onto your feet.",victim,NULL,NULL,TO_CHAR);
            act("$n suddenly does a reverse flip onto $s feet.",victim,NULL,NULL,TO_ROOM);
         }
         else if ( number_percent() > chance )
                 victim->wait = UMIN( 18, victim->wait + 12 );

      ch->wait = 18;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = AERIAL;
      ch->attack_var = BLUNT;

      if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
         hurt_person( ch, victim, 2000000, TRUE );
         else hurt_person( ch, victim, 1000000, TRUE );
}

void do_mega_strike( CHAR_DATA *ch, char *argument)
{
      char arg[MAX_INPUT_LENGTH];
      OBJ_DATA *obj;
      CHAR_DATA *victim;
      int dam = 1500000;

	one_argument( argument, arg );

      if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 ) 
         return;

      if ( !IS_NPC( ch ) )
         {
            if ( !IS_CLASS(ch, CLASS_ANGEL) )
               {
                  send_to_char("Huh?\n\r",ch);
                  return;
               }

            if ( !can_do_skill( ch, 60 ) )
               {
                  send_to_char("Huh?\n\r",ch);
                  return;
               }

            if ( ch->throw_timer > 0 )
               {
                  stc("You are still recovering from the last one.\n\r",ch);
                  return;
               }

            if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
               if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL )
                  if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL )
                  {
                     send_to_char( "You need a free hand to do this technique.\n\r", ch );
                     return;
                  }
         }

      if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
         {
            send_to_char("Whom do you wish to strike?\n\r",ch);
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

      act("You wave your free hand in front of you as you push your palm towards $N.",ch,NULL,victim,TO_CHAR);
      act("$n waves $s free hand in front of $m as $e pushes $s palm towards you.",ch,NULL,victim,TO_VICT);
      act("$n waves $s free hand in front of $m as $e pushes $s palm towards $N.",ch,NULL,victim,TO_NOTVICT);
      act("A strange humming fills the air as pitch black energy manifests around your hand.",ch,NULL,NULL,TO_CHAR);
      act("A strange humming fills the air as pitch black energy manifests around your hand.",ch,NULL,NULL,TO_ROOM);
      act("Suddenly you spread out your fingers releasing the energy towards $N!",ch,NULL,victim,TO_CHAR);
      act("Suddenly $n spreads out $s fingers releasing the energy towards you!",ch,NULL,victim,TO_VICT);
      act("Suddenly $n spreads out $s fingers releasing the energy towards $N!",ch,NULL,victim,TO_NOTVICT);
      act("As $N staggers back from the blow, you viciously SwIpE your sword through $S midsection!",ch,NULL,victim,TO_CHAR);
      act("As you stagger back from the blow, $n viciously SwIpEs $s sword through your midsection!",ch,NULL,victim,TO_VICT);
      act("As $N staggers back from the blow, $n viciously SwIpEs $s sword through $S midsection!",ch,NULL,victim,TO_NOTVICT);
      act("Explosive energy fills the room as $n screams loudly.",victim,NULL,NULL,TO_ROOM);
      act("Explosive energy fills the room as you scream loudly.",victim,NULL,NULL,TO_CHAR);

      if ( !IS_NPC( ch ) )
         {
            ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
            ch->throw_timer = 50;
            dam = 2000000;
            SET_BIT( victim->more, MORE_BLIND );
            SET_BIT( ch->arena, AFF2_DM );
         }

      ch->ele_attack = ATTACK_HOLY;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = DIRECT;
      ch->attack_var = SLASH;

      if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
         hurt_person( ch, victim, dam * 2, TRUE );
         else hurt_person( ch, victim, dam, TRUE );
}

void do_blade_dance( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
      OBJ_DATA *obj;
      CHAR_DATA *victim;

	one_argument( argument, arg );

      if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 ) 
         return;

      if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
         {
            send_to_char("Whom do you wish to strike?\n\r",ch);
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

	if ( is_safe( ch, victim ))		return;

      act("You suddenly point your hand at $N.",ch,NULL,victim,TO_CHAR);
      act("Suddenly, $n points $s hand at you.",ch,NULL,victim,TO_VICT);
      act("Suddenly, $n points $s hand at $N.",ch,NULL,victim,TO_NOTVICT);
      act("The Sword of Heavens rushes to $N and twirls itself in a half-arch making $M screams in pain.",ch,NULL,victim,TO_CHAR);
      act("The Sword of Heavens rushes to you and twirls itself in a half-arch making you screams in pain.",ch,NULL,victim,TO_VICT);
      act("The Sword of Heavens rushes to $N and twirls itself in a half-arch making $M screams in pain.",ch,NULL,victim,TO_NOTVICT);
      act("Then it does a series of unpredictable savage cuts throughout $N's body.",ch,NULL,victim,TO_CHAR);
      act("Then it does a series of unpredictable savage cuts throughout $N's body.",ch,NULL,victim,TO_NOTVICT);
      act("Then it does a series of unpredictable savage cuts throughout your body.",ch,NULL,victim,TO_VICT);
      act("Suddenly, it circles itself behind $N and trip $M onto the ground with a sonic-fast sweep.",ch,NULL,victim,TO_CHAR);
      act("Suddenly, it circles itself behind you and trip you onto the ground with a sonic-fast sweep.",ch,NULL,victim,TO_VICT);
      act("Suddenly, it circles itself behind $N and trip $M onto the ground with a sonic-fast sweep.",ch,NULL,victim,TO_NOTVICT);
      act("Finally, it rushes high straight into the sky, then SlAmS iTsElF DoWn rIgHt InTo $N BODY!.",ch,NULL,victim,TO_CHAR);
      act("Finally, it rushes high straight into the sky, then SlAmS iTsElF DoWn rIgHt InTo your BODY!.",ch,NULL,victim,TO_VICT);
      act("Finally, it rushes high straight into the sky, then SlAmS iTsElF DoWn rIgHt InTo $N BODY!.",ch,NULL,victim,TO_NOTVICT);

      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = DIRECT;
      ch->attack_var = SLASH;

      if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
         hurt_person( ch, victim, 2000000, TRUE );
         else hurt_person( ch, victim, 1000000, TRUE );
}

void do_fist_heaven(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *rch, *rch_next;
    int count;
    int dam = 1500000;

      if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 ) 
         return;

      if ( !IS_NPC( ch ) )
         {
            if ( !IS_CLASS(ch, CLASS_ANGEL) )
               {
                  send_to_char("Huh?\n\r",ch);
                  return;
               }

            if ( get_stock( ch ) < 100 )
               {
                  stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                  return;
               }

            dam = 1000000;
         }

    SET_BIT( ch->arena, AFF2_DM );
    act("You fly high into the sky as you push your palm towards the centre of the room!",ch,NULL,NULL,TO_CHAR);
    act("$n flies high into the sky as $e pushes $s palm towards you!",ch,NULL,NULL,TO_ROOM);
    act("The sky goes dark as numerous thunder bolts fill the air.",ch,NULL,NULL,TO_CHAR);
    act("The sky goes dark as numerous thunder bolts fill the air.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly a swiral of energy circles everyone in the room.",ch,NULL,NULL,TO_CHAR);
    act("Suddenly a swiral of energy circles everyone in the room.",ch,NULL,NULL,TO_ROOM);
    act("A MaSsIvE ThUnDEr BOlT SMASHES InTo the RoOm StUnNiNg EveRyOne iN Its Way!!!",ch,NULL,NULL,TO_CHAR);
    act("A MaSsIvE ThUnDEr BOlT SMASHES InTo the RoOm StUnNiNg EveRyOne iN Its Way!!!",ch,NULL,NULL,TO_ROOM);

    if ( !IS_NPC( ch ) )
       {
          ch->wait = 12;
          subtract_stock( ch, 100 );
       }

    for( rch = ch->in_room->people; rch != NULL; rch = rch_next )
    {
       rch_next = rch->next_in_room;

       if ( is_same_group(ch,rch) )
          continue;

       if ( is_safe(ch, rch) )
          continue;

       ch->ele_attack = ATTACK_THUNDER;
       ch->attack_type = ATTACK_MAGICAL;
       ch->attack_focus = AERIAL;
       ch->attack_var = BLUNT;

       if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
          hurt_person( ch, rch, 2000000, TRUE );
          else hurt_person( ch, rch, dam, TRUE );
    }
}

void do_angelic_genesis( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_ANGEL) )
	 {
          send_to_char("Huh?\n\r",ch);
          return;
	 }

    if ( ch->pkill < 18 )
       {
          stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
          return;
       }

    if ( ch->ctimer[4] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->hit > 1000000 )
       {
          stc("You aren't in times of need perfect being.\n\r",ch);
          return;
       }

    if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
       if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
          if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
          {
             send_to_char( "You need a free hand to do this technique.\n\r", ch );
             return;
          }

    act("You fly high into the sky as you spread your wings widely!",ch,NULL,NULL,TO_CHAR);
    act("$n flies high into the sky as $e spreads $s wings widely!",ch,NULL,NULL,TO_ROOM);
    act("You say solemnly '#yIn the beginning, one shall find his end...#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says solemnly '#yIn the beginning, one shall find his end...#n'",ch,NULL,NULL,TO_ROOM);
    act("You then say with a smile '#yLet the wheel of times renew thy suffering.#n'",ch,NULL,NULL,TO_CHAR);
    act("$n then says with a smile '#yLet the wheel of times renew thy suffering.#n'",ch,NULL,NULL,TO_ROOM);
    act("You suddenly open your free hand as the room flashes from a mysterious light!",ch,NULL,NULL,TO_CHAR);
    act("$n suddenly opens $s free hand as the room flashes from a mysterious light!",ch,NULL,NULL,TO_ROOM);
    ch->ctimer[4] = 8;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->character == NULL )
          continue;

       stc( "The air suddenly thickens as everything become blurry to you.\n\r", d->character );

       if ( d->character != ch )
          stc( "As you shake the stars from your head, you realize the atmosphere looks familiar to you.\n\r", d->character );
       
       d->character->fight_timer = 0;
       stop_fighting( d->character, TRUE );
       restore_char( d->character );
    }

    ch->wait = 12;
}

void do_wrathofgod( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
 
  argument = one_argument( argument, arg );

      if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 ) 
         return;

      if ( !IS_NPC( ch ) )
         {
            if ( !IS_CLASS(ch, CLASS_ANGEL) )
               {
                  send_to_char("Huh?\n\r",ch);
                  return;
               }

            if ( get_stock( ch ) < 200 )
               {
                  stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                  return;
               }
         }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to face God's wrath?\n\r",ch);
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

  if ( !IS_NPC( ch ) && IS_CLASS( victim, CLASS_ANGEL ) )
     {
        stc("#wAngels are born without #esin#w!#n\n\r",ch);
        return;
     }

  if ( !IS_NPC( ch ) && ( IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_MONK ) ) )
     {
        stc("#wYou can't find the #Rheart #wto use this against the mortals who fight for #ygood#w!#n\n\r",ch);
        return;
     }

  if ( !IS_NPC( ch ) )
     {
        ch->wait = 12;
        subtract_stock( ch, 200 );
     }

  SET_BIT( ch->arena, AFF2_DM );
  act("You say '#yThere is no greater fury than that of the wrath of God!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says '#yThere is no greater fury than that of the wrath of God!#n'",ch,NULL,NULL,TO_ROOM);
  act("You spread your wings as you fly towards $N.",ch,NULL,victim,TO_CHAR);
  act("$n spreads $s wings as $e flies towards you.",ch,NULL,victim,TO_VICT);
  act("$n spreads $s wings as $e flies towards $N.",ch,NULL,victim,TO_NOTVICT);
  act("As you touch $N's forehead, $S eyes open wide with absolute fear!",ch,NULL,victim,TO_CHAR);
  act("As $n touches your forehead, your eyes open wide with absolute fear!",ch,NULL,victim,TO_VICT);
  act("As $n touches $N's forehead, $S eyes open wide with absolute fear!",ch,NULL,victim,TO_NOTVICT);

  send_to_char("#RYou burn them with the fires of hell!#n\n\r",ch);
  act("#R$n burns you with the fires of hell!#n",ch, NULL,victim,TO_VICT);
  if ( !IS_AFFECTED(victim, AFF_FLAMING) ) 
     SET_BIT(victim->affected_by, AFF_FLAMING);

  send_to_char("#gTheir body is wrecked by poison!#n\n\r",ch);
  send_to_char("#gYour body is wrecked by poison!#n\n\r",victim);
  if ( !IS_SET(victim->more, MORE_POISON) ) 
     SET_BIT(victim->more, MORE_POISON);

  send_to_char("#cTheir mind is confused with the weight of their sins!#n\n\r",ch);
  send_to_char("#cYour mind is confused with the weight of your sins!#n\n\r",victim);
  if ( !IS_SET(victim->more, MORE_CONFUSION) ) 
     SET_BIT(victim->more, MORE_CONFUSION);

  send_to_char("#eThey go down on their knees as their sins paralyse them with terror!#n\n\r",ch);
  send_to_char("#eYou go down on your knees as your sins paralyse you with terror!#n\n\r",ch);
  if ( !IS_SET(victim->more2, MORE2_NOARMS) )
     SET_BIT(victim->more2, MORE2_NOARMS);

  send_to_char("#pThey can't find any words to describe what they have done!#n\n\r",ch);
  send_to_char("#pYou suddenly can't find the words to describe what you have done!#n\n\r",victim);
  if ( !IS_SET(victim->more, MORE_HEX) ) 
     SET_BIT(victim->more, MORE_HEX);

  send_to_char("#yThey cover their eyes unable to witness this corrupt world!#n\n\r",ch);
  send_to_char("#yYou cover your eyes as you are no longer able to witness this corrupt world!#n\n\r",victim);
  if ( !IS_SET(victim->more, MORE_BLIND) ) 
     SET_BIT(victim->more, MORE_BLIND);

  send_to_char("#wTheir strength pales before their very eyes!#n\n\r",ch);
  send_to_char("#wYour strength pales before your very eyes!#n\n\r",victim);
  if ( !IS_SET(victim->more, MORE_PINCH) ) 
     SET_BIT(victim->more, MORE_PINCH);

  if ( !IS_SET(victim->more, MORE_PAIN) ) 
     SET_BIT(victim->more, MORE_PAIN);

  if ( !IS_SET(victim->more, MORE_DISEMPOWER) )
     SET_BIT(victim->more, MORE_DISEMPOWER);

  if ( victim->gshield > 0 )
     victim->gshield = 0;

  act("You scream with terror as you are struck with the wrath of God!",victim,NULL,NULL,TO_CHAR);
  act("$n screams with terror as $s is struck with the wrath of a divine presence!",victim,NULL,NULL,TO_ROOM);
  send_to_char("#CGod's fury is upon them for they have truly sinned!!!#n\n\r",ch);

  ch->ele_attack = ATTACK_HOLY;
  ch->attack_type = ATTACK_MAGICAL;
  ch->attack_focus = DIRECT;
  ch->attack_var = SOUL;

  if ( IS_NPC( ch ) && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
     hurt_person( ch, victim, 5000000, TRUE );
     else hurt_person( ch, victim, 2500000, TRUE );
}

void do_seraph_curse( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch)) return;

   if ( !IS_CLASS( ch, CLASS_ANGEL ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ch->pkill < 14 )
      {
         stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
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

   if ( !is_same_group(ch,victim) && !( ch->pcdata->kingdom == victim->pcdata->kingdom
         && ch->pcdata->kingdom > 0 ) )
      {
         stc("They have to be in the same group or kingdom as you.\n\r", ch );
         return;
      }

   if ( IS_SET( victim->elemental, ELE_NONE ) )
      {
         stc("They already can't do elemental attacks divine one.\n\r", ch );
         return;
      }

   send_to_char("You concentrate a moment.\n\r",ch);
   act("$n stops and concentrates a moment.\n\r",ch,NULL,NULL, TO_ROOM);

   act("You say, '#yMay the blessed rain give me new powers...'#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yMay the blessed rain give me new powers...'#n'",ch,NULL,NULL,TO_ROOM);

   act("You snarl at $N, '#yThis is the rage of the fallen angel!'#n'",ch,NULL,victim,TO_CHAR);
   act("$n snarls at you, '#yThis is the rage of the fallen angel!'#n'",ch,NULL,victim,TO_VICT);
   act("$n snarls at $N, '#yThis is the rage of the fallen angel!'#n'",ch,NULL,victim,TO_NOTVICT);

   act("You push your hand at $N as $E is suddenly struck down by an invisible blast.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s hand at you as you are suddenly struck down by an invisible blast.",ch,NULL,victim,TO_VICT);
   act("$n pushes $s hand at $N as $E is suddenly struck down by an invisible blast.",ch,NULL,victim,TO_NOTVICT);

   act("#wThe light frowns at you as you no longer are blessed with #Pelemental #Rpowers#w.#n",victim,NULL,NULL, TO_CHAR);

   SET_BIT( victim->elemental, ELE_NONE );
}

void do_hope( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS( ch, CLASS_ANGEL ) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->pkill < 14 )
     {
        stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
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

  if ( ( sn = skill_lookup( "hope" ) ) < 0 ) return;

  if ( is_affected(victim,skill_lookup("hope")) )
  {
    stc("The person is already filled with hope!\n\r",ch);
    return;
  }

  ch->mana -= 5000000;
  level = 30;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 12 );
}

void do_holy_ball( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_ANGEL) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
      if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
         if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
         {
            send_to_char( "You need a free hand to utilize this technique.\n\r", ch );
            return;
         }

   if ( ch->ctimer[1] > 0 )
      {
         send_to_char( "You are still recovering from the last one.\n", ch ); 
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

   if ( is_safe(ch,victim) )
      return;

   act("You fly into the air as you focus your mental energy towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n flies into the air as $e focuses $s mental energy towards you!",ch,NULL,victim,TO_VICT);
   act("$n flies into the air as $e focuses $s mental energy towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("Three spirals of energy descend from the heavens swerving in a circular motion!",ch,NULL,NULL,TO_CHAR);
   act("Three spirals of energy descend from the heavens swerving in a circular motion!",ch,NULL,NULL,TO_ROOM);
   act("The spirals of energy circles around you as they flare with intensity!",victim,NULL,NULL,TO_CHAR);
   act("The spirals of energy circles around $n as they flare with intensity!",victim,NULL,NULL,TO_ROOM);
   act("Your body starts pulsing as the balls fades into you.",victim,NULL,NULL,TO_CHAR);
   act("$n's body starts pulsing as the balls fades into $m.",victim,NULL,NULL,TO_ROOM);
   act("You scream as MaSsIvE energy within you implodes, engulfing you in blazing energy!",victim,NULL,NULL,TO_CHAR);
   act("$n screams as MaSsIvE energy within $m implodes, engulfing $m in blazing energy!",victim,NULL,NULL,TO_ROOM);
   act("The air ROARS as the beam BlAsTs from within you shooting towards the heavens!!!",victim,NULL,NULL,TO_CHAR);
   act("The air ROARS as the beam BlAsTs from within $m shooting towards the heavens!!!",victim,NULL,NULL,TO_ROOM);
   act("You are knocked across the room in a dazed!",victim,NULL,NULL,TO_CHAR);
   act("$n is knocked across the room in a dazed!",victim,NULL,NULL,TO_ROOM); 

   ch->ctimer[1] = 3;
   victim->mana = UMAX( 0, victim->mana - ( victim->max_mana / 3 ) );
}

void do_houseofgod( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim, *vch, *vch_next;
  OBJ_DATA *obj;

  if (IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_ANGEL) || ch->clannum != 3 )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->ctimer[2] > 0 )
     {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
     }

  if ( ch->move < 15000 )
     {
	   stc("You are too tired to spread the Glory of God.\n\r",ch);
	   return;
     }

  ch->ctimer[2] = 4;
  ch->move -= 15000;

  act("You call for God to transfer the pain of these mortals to yourself.", ch, NULL, NULL, TO_CHAR);
  act("$n says '#yLet not these followers of God suffer, let their pain be mine instead#n'.", ch, NULL, NULL, TO_ROOM);
  act("You call for God's worshippers to stand forth and pray for Goodwill.", ch, NULL, NULL, TO_CHAR);
  act("$n says '#yNo need to hide from the Light, God loves all#n'.", ch, NULL, NULL, TO_ROOM);

  for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
  {
      if ( obj->chobj == NULL )
         continue;

      victim = obj->chobj;

      if ( IS_NPC(victim) )
         continue;

      victim->pcdata->obj_vnum = 0;
      obj->chobj = NULL;
      victim->pcdata->chobj = NULL;
      
      REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(victim->extra, EXTRA_OSWITCH);

      free_string(victim->morph);
      victim->morph = str_dup("");

      act("$p transforms into $n.",victim,obj,NULL,TO_ROOM);
      act("You reform your human body.",victim,obj,NULL,TO_CHAR);

      extract_obj(obj);
  }

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
      vch_next = vch->next_in_room;

      stop_fighting(vch,TRUE);

      if ( IS_AFFECTED(vch, AFF_ETHEREAL) )
         REMOVE_BIT(vch->affected_by, AFF_ETHEREAL);

      if ( IS_AFFECTED(vch, AFF_SHADOWPLANE) )
         REMOVE_BIT(vch->affected_by, AFF_SHADOWPLANE);

      if ( IS_SET(vch->more, MORE_SAFE) )
         REMOVE_BIT(vch->more, MORE_SAFE);

      if ( is_safe(ch,vch) )
         continue;

      if ( !is_martial( vch ) )
         vch->wait = 24;
  }
}

void do_redemption( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_ANGEL) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc( "Whom do you wish to inspire great fear in?\n\r",ch);
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
           send_to_char( "You don't have enough magical energy for that.\n\r", ch );
           return;
        }

     if ( is_safe( ch, victim ) )
        return;

     if ( IS_CLASS( victim, CLASS_MONK ) || IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_ANGEL ) )
        {
           stc("Followers of the Light knows no fear.\n\r",ch);
           return;
        }

     if ( !IS_SET(victim->mflags, MAGE_EMBRACED2) )
        {
           act("You fly high into the sky as you push your palm towards $N!",ch,NULL,victim,TO_CHAR);
           act("$n flies high into the sky as $e pushes $s palm towards you!",ch,NULL,victim,TO_VICT);
           act("$n flies high into the sky as $e pushes $s palm towards $N!",ch,NULL,victim,TO_NOTVICT);
           act("As visions of the future fills your eyes, you fall to the ground cowering in fear.",victim,NULL,NULL,TO_CHAR);
           act("$n falls to the ground cowering in fear.",victim,NULL,NULL,TO_ROOM);

           ch->mana -= 1000000;
           SET_BIT(victim->mflags, MAGE_EMBRACED2);
           WAIT_STATE(ch, 4);
        }
        else {
                stc("They are already cowering in fear.\n\r",ch);
                return;
             }
}

void do_purification( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_ANGEL) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 6 )
      {
         stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
         return;
      }

   if ( ch->mana < 3000000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 3000000;
   sn = skill_lookup( "blessing of the saints" );

   send_to_char("You concentrate a moment.\n\r",ch);
   act("$n stops and concentrates a moment.\n\r",ch,NULL,NULL, TO_ROOM);

   act("You say, '#yHave faith in the light... and be cleansed of all evil intentions.'#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yHave faith in the light... and be cleansed of all evil intentions'#n'",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yPurification of the Divine Child!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yPurification of the Divine Child!#n'",ch,NULL,NULL,TO_ROOM);
   send_to_char("\n\r#wYou wave your hand in a circular motion as Holy Symbols envelops the entire room.#n\n\r",ch);
   act("\n\r#w$n #wwaves $s hand in a circular motion as Holy Symbols fill the entire room.#n",ch,NULL,NULL, TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group( ch, vch ) && ( IS_NPC( vch ) || vch->pcdata->kingdom != ch->pcdata->kingdom
           || ch->pcdata->kingdom == 0 ) )
         continue;

      act("You feel refresh as the delicate waves pass your body.",vch,NULL,NULL,TO_CHAR);
      act("$n feels refresh as the delicate waves pass $s body.",vch,NULL,NULL,TO_ROOM);
      clear_affects( vch );

      if ( sn > 0 && !is_affected( vch, sn ) )
         (*skill_table[sn].spell_fun) ( sn, 999, ch, vch );
   }

   ch->wait = 12;
}

void do_heavenly_cloak( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 553 )
      return;

   if ( !IS_NPC( ch ) )
      {
         if ( !IS_CLASS(ch, CLASS_ANGEL) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }     

         if ( ch->pkill < 18 )
            {
               stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
               return;
            }

         if ( ch->ctimer[0] > 0 )
            {
               send_to_char("You are still recovering from the last one.\n\r",ch);
               return;
            } 

         ch->ctimer[0] = 6;
      }

   send_to_char("You concentrate a moment.\n\r",ch);
   act("$n stops and concentrates a moment.\n\r",ch,NULL,NULL, TO_ROOM);

   act("You say, '#yMay the blessed rain give me new powers...'#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yMay the blessed rain give me new powers...'#n'",ch,NULL,NULL,TO_ROOM);
   act("You then go on your knees and pray as a beautiful shower fill the vicinity.",ch,NULL,NULL,TO_CHAR);
   act("$n then goes on $s knees and prays as a beautiful shower fill the vicinity.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC( ch ) && vch != ch )
         continue;

      if ( !IS_NPC( ch ) && !is_same_group( ch, vch ) && ( IS_NPC( vch ) || vch->pcdata->kingdom != ch->pcdata->kingdom
           || ch->pcdata->kingdom == 0 ) )
         continue;

      SET_BIT( vch->more3, MORE3_DODGE );
      act("An outline of a blurry cloak radiates off you as the rain falls off your body.",vch,NULL,NULL,TO_CHAR);
      act("An outline of a blurry cloak radiates off $n as the rain falls off $s body.",vch,NULL,NULL,TO_ROOM);
   }

   ch->wait = 12;
}

void do_blazing_hand( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ANGEL ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET( ch->more3, MORE3_HAND ) )
      {
         REMOVE_BIT( ch->more3, MORE3_HAND );
         act("You turn around softly looking into the distance.",ch,NULL,NULL,TO_CHAR);
         act("$n turns around softly looking into the distance.",ch,NULL,NULL,TO_ROOM);
         act("Opening your right hand, the bluish flames from top of it fade away...",ch,NULL,NULL,TO_CHAR);
         act("As $n opens $s right hand, the bluish flames from top of it fade away...",ch,NULL,NULL,TO_ROOM);
         return;
      }

   SET_BIT( ch->more3, MORE3_HAND );
   act("You turn around softly looking into the distance.",ch,NULL,NULL,TO_CHAR);
   act("$n turns around softly looking into the distance.",ch,NULL,NULL,TO_ROOM);
   act("Opening your right hand, a blast of bluish flames hovers from top of it.",ch,NULL,NULL,TO_CHAR);
   act("As $n opens $s right hand, a blast of bluish flames hovers from top of it.",ch,NULL,NULL,TO_ROOM);
   act("You grin evilly as you clench your fist, covering the deadly flames.",ch,NULL,NULL,TO_CHAR);
   act("$n grins evilly as $e clenches $s fist, covering the deadly flames.",ch,NULL,NULL,TO_ROOM);
}

void do_divine_judgement( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_ANGEL) )
	 {
          send_to_char("Huh?\n\r",ch);
          return;
	 }

    if ( ch->pkill < 20 )
       {
          stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
          return;
       }

    if ( ch->ctimer[3] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->hit > ch->max_hit * 0.25 )
       {
          stc("You aren't in times of need perfect being.\n\r",ch);
          return;
       }

    if ( !IS_CLASS( ch, CLASS_DEMON ) || IS_SET( ch->special, SPC_DOPPLE ) )
       if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
          if ( get_eq_char( ch, WEAR_HOLD ) != NULL )
          {
             send_to_char( "You need a free hand to do this technique.\n\r", ch );
             return;
          }

    act("You fly high into the sky as you spread your wings widely!",ch,NULL,NULL,TO_CHAR);
    act("$n flies high into the sky as $e spreads $s wings widely!",ch,NULL,NULL,TO_ROOM);
    act("You say solemnly '#yFear not the wrath of God, for he shall forgive the unforgiven...#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says solemnly '#yFear not the wrath of God, for he shall forgive the unforgiven#n'",ch,NULL,NULL,TO_ROOM);
    act("You then say with a smile '#yLet us bond now and bring peace throughout the land.#n'",ch,NULL,NULL,TO_CHAR);
    act("$n then says with a smile '#yLet us bond now and bring peace throughout the land.#n'",ch,NULL,NULL,TO_ROOM);
    act("You suddenly open your free hand as the room flashes from a mysterious light!",ch,NULL,NULL,TO_CHAR);
    act("$n suddenly opens $s free hand as the room flashes from a mysterious light!",ch,NULL,NULL,TO_ROOM);
    ch->ctimer[3] = 12;
    nofighting = 6;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->character == NULL )
          continue;

       stc( "The air suddenly thickens as everything become blurry to you.\n\r", d->character );

       if ( d->character != ch )
          stc( "As you shake the stars from your head, you suddenly have a desire to stop fighting.\n\r", d->character );
       
       stop_fighting( d->character, TRUE );
    }

    ch->wait = 6;
}
