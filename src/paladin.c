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

void do_avatar( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
        
    if (IS_NPC(ch)) return;
         
    if ( !IS_CLASS(ch, CLASS_PALADIN) || IS_SET( ch->special, SPC_SWORD ) )
	 {
		stc("Huh?\n\r",ch);
		return;
	 }

    if ( ch->max_hit < 7500000 )
       {
           stc("You don't have enough vitality for that.\n\r",ch);
           return;
       }

    if ( ch->max_mana < 7500000 )
       {
           stc("You don't have enough magical energy for that.\n\r",ch);
           return;
       }

    if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
       {
           stc("But you are the Avatar.\n\r",ch);
           return;
       }

    stc("You concentrate on the eight virtues as its powers flow through your body.\n\r",ch);
    stc("Suddenly, they scatter apart as your body bursts with holy energy.\n\r",ch);

    if ( ch->sex == SEX_FEMALE )
       sprintf(buf, "#PLady #y%s #wThe #LAva#Ptar#n", ch->name);
       else sprintf(buf, "#LSir #y%s #wThe #PAva#Ltar#n", ch->name);

    free_string( ch->morph );
    ch->morph = str_dup( buf );
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
}

void do_pgrant( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char lin[MAX_STRING_LENGTH];
    int inpart = 0;
    int cost = 0;

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_PALADIN) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    sprintf(lin,"#y===============================================================================#n\n\r");

    if ( arg[0] == '\0' )
    {
      stc(" #wSyntax: PGrant <sword|abilities> <power>\n\r#n",ch);
      stc("#y=========================| #wPowers of the Holy Sword #y|==========================#n\n\r",ch);
      sprintf(buf,"           #wStasis Sword [#c%s#w]      Crush Punch [#c%s#w]     Lightning Stab  [#c%s#w]#n\n\r",
             IS_SET(ch->paladinab[SPIRIT],32) ? "*" : " ",
             IS_SET(ch->paladinab[SPIRIT],64) ? "*" : " ",
             IS_SET(ch->paladinab[SPIRIT],128) ? "*" : " ");
      stc(buf,ch);
      sprintf(buf,"           #wSplit Punch  [#c%s#w]                          Holy Explosion [#c%s#w]#n\n\r",
             IS_SET(ch->paladinab[SPIRIT],256) ? "*" : " ",
             IS_SET(ch->paladinab[SPIRIT],512) ? "*" : " ");
      stc(buf,ch);

	stc(lin,ch);

	   stc("                          #wAuras of Blessed Destiny#n                                   \n\r",ch);
      stc(lin,ch);
      sprintf(buf,"           #wMight       [#c%s%s%s%s%s%s%s%s%s%s#w]   Concentration  [#c%s%s%s%s%s%s%s%s%s%s#w]#n\n\r",
                  ch->aura[A_MIGHT] > 0 ? "*" : " ",
                  ch->aura[A_MIGHT] > 1 ? "*" : " ",
                  ch->aura[A_MIGHT] > 2 ? "*" : " ",
                  ch->aura[A_MIGHT] > 3 ? "*" : " ",
                  ch->aura[A_MIGHT] > 4 ? "*" : " ",
                  ch->aura[A_MIGHT] > 5 ? "*" : " ",
                  ch->aura[A_MIGHT] > 6 ? "*" : " ",
                  ch->aura[A_MIGHT] > 7 ? "*" : " ",
                  ch->aura[A_MIGHT] > 8 ? "*" : " ",
                  ch->aura[A_MIGHT] > 9 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 0 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 1 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 2 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 3 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 4 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 5 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 6 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 7 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 8 ? "*" : " ",
                  ch->aura[A_CONCENTRATION] > 9 ? "*" : " ");
      stc(buf, ch);

      sprintf(buf,"           #wHoly Fire   [#c%s%s%s%s%s%s%s%s%s%s#w]   Holy Freeze    [#c%s%s%s%s%s%s%s%s%s%s#w]#n\n\r",
                  ch->aura[A_HFIRE] > 0 ? "*" : " ",
                  ch->aura[A_HFIRE] > 1 ? "*" : " ",
                  ch->aura[A_HFIRE] > 2 ? "*" : " ",
                  ch->aura[A_HFIRE] > 3 ? "*" : " ",
                  ch->aura[A_HFIRE] > 4 ? "*" : " ",
                  ch->aura[A_HFIRE] > 5 ? "*" : " ",
                  ch->aura[A_HFIRE] > 6 ? "*" : " ",
                  ch->aura[A_HFIRE] > 7 ? "*" : " ",
                  ch->aura[A_HFIRE] > 8 ? "*" : " ",
                  ch->aura[A_HFIRE] > 9 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 0 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 1 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 2 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 3 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 4 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 5 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 6 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 7 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 8 ? "*" : " ",
                  ch->aura[A_HFREEZE] > 9 ? "*" : " ");
      stc(buf, ch);

      sprintf(buf,"           #wHoly Shock  [#c%s%s%s%s%s%s%s%s%s%s#w]   Thorns         [#c%s%s%s%s%s%s%s%s%s%s#w]#n\n\r",
                  ch->aura[A_HSHOCK] > 0 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 1 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 2 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 3 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 4 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 5 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 6 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 7 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 8 ? "*" : " ",
                  ch->aura[A_HSHOCK] > 9 ? "*" : " ",
                  ch->aura[A_THORNS] > 0 ? "*" : " ",
                  ch->aura[A_THORNS] > 1 ? "*" : " ",
                  ch->aura[A_THORNS] > 2 ? "*" : " ",
                  ch->aura[A_THORNS] > 3 ? "*" : " ",
                  ch->aura[A_THORNS] > 4 ? "*" : " ",
                  ch->aura[A_THORNS] > 5 ? "*" : " ",
                  ch->aura[A_THORNS] > 6 ? "*" : " ",
                  ch->aura[A_THORNS] > 7 ? "*" : " ",
                  ch->aura[A_THORNS] > 8 ? "*" : " ",
                  ch->aura[A_THORNS] > 9 ? "*" : " ");
      stc(buf, ch);

      sprintf(buf,"           #wSanctuary   [#c%s%s%s%s%s%s%s%s%s%s#w]   Conviction     [#c%s%s%s%s%s%s%s%s%s%s#w]#n\n\r",
                  ch->aura[A_SANCTUARY] > 0 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 1 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 2 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 3 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 4 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 5 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 6 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 7 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 8 ? "*" : " ",
                  ch->aura[A_SANCTUARY] > 9 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 0 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 1 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 2 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 3 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 4 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 5 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 6 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 7 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 8 ? "*" : " ",
                  ch->aura[A_CONVICTION] > 9 ? "*" : " ");
      stc(buf, ch);

	stc(lin,ch);

      stc("                          #wAbilities of the Paladin#n                                   \n\r",ch);
      stc(lin,ch);
      sprintf(buf,"           #wCharge      [#c%s%s%s%s%s%s%s%s%s%s#w]   Soul Hammer    [#c%s%s%s%s%s%s%s%s%s%s#w]#n\n\r",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_CHARGE] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ",
                  ch->aura[A_HAMMER] == 0 ? "*" : " ");
      stc(buf, ch);
      sprintf(buf,"\n                      #wJustice Cutter   [#c%s%s%s%s%s%s%s%s%s%s#w]#n\n\r",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ",
                   ch->aura[A_CUTTER] == 0 ? "*" : " ");
      stc(buf, ch);
      stc(lin,ch);

      stc("                          #wAbilities of the Crusades#n                                   \n\r",ch);
      stc(lin,ch);
	sprintf(buf,"           #wGodly Sight [#c%s#w]       God's Search [#c%s#w]    #wGod's Read [#c%s#w]#n\n\r",
	        IS_SET(ch->paladinab[SPIRIT],1) ? "*" : " ",
              IS_SET(ch->paladinab[SPIRIT],2) ? "*" : " ",
              IS_SET(ch->paladinab[SPIRIT],4) ? "*" : " ");
	stc(buf, ch);
	sprintf(buf,"           #wHonour Gate [#c%s#w]                           God's Heal [#c%s#w]#n\n\r",
              IS_SET(ch->paladinab[SPIRIT],16) ? "*" : " ",
              IS_SET(ch->paladinab[SPIRIT],32) ? "*" : " " );
	stc(buf, ch);
	stc(lin,ch);
	return;
    }
    
  if ( !str_cmp(arg1, "sword" ) )
  {
    cost = 0;
    if ( !str_cmp( arg, "stasis sword" ) )
       inpart = 32;
       else if ( !str_cmp( arg, "crush punch" ) )
               inpart = 64;
       else if ( !str_cmp( arg, "lightning stab" ) )
               inpart = 128;
       else if ( !str_cmp( arg, "split punch" ) )
               inpart = 256;
       else if ( !str_cmp( arg, "holy explosion" ) )
               inpart = 512;
       else {
               do_pgrant(ch,"");
               return;
            }
  }
  else if ( !str_cmp(arg1, "abilities" ) )
  {
         if ( !str_cmp( arg, "sight" ) )
	      inpart = 1;
            else if ( !str_cmp( arg, "search" ) )
	              inpart = 2;
            else if ( !str_cmp( arg, "read" ) )
	              inpart = 4;
            else if ( !str_cmp( arg, "gate" ) )
	              inpart = 16;
            else if ( !str_cmp( arg, "heal" ) )
	              inpart = 32;
                    else {
                            do_pgrant(ch,"");
                            return;
                         }
  }
  else {
          do_pgrant(ch,"");
          return;
       }

  if ( IS_SET(ch->paladinab[SPIRIT],inpart) )
     {
	  stc("You have already learned this ability.\n\r",ch);
	  return;
     }
    
  SET_BIT(ch->paladinab[SPIRIT],inpart);
  send_to_char("Ok.\n\r",ch);
  save_char_obj(ch);
}

void do_holyarmor( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost = 1, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_PALADIN) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: holyarmor <item>\n\r", ch );
	send_to_char( "items are: ring (1), pendant (1), armor (1), leggings (1), boots (1)\n\r", ch );
	send_to_char( "           sleeves (1), helmet (1), robe (1), visor (1), gauntlets (1)\n\r", ch );
	send_to_char( "           bracers (1), sword (1), shield (1), belt (1)\n\r", ch );
	return;
    }

    if (!str_cmp(arg,"ring"))
    {
       vnum = 157;
    }
    else if (!str_cmp(arg,"pendant"))
            {
               vnum = 158;
            }
    else if (!str_cmp(arg,"armor"))
            {
               vnum = 159;
            }
    else if (!str_cmp(arg,"leggings"))
            {
               vnum = 160;
            }
    else if (!str_cmp(arg,"boots"))
            {
               vnum = 161;
            }
    else if (!str_cmp(arg,"belt"))
            {
               vnum = 170;
            }
    else if (!str_cmp(arg,"sleeves"))
            {
               vnum = 162;
            }
    else if (!str_cmp(arg,"helmet"))
            {
               vnum = 163;
            }
    else if (!str_cmp(arg,"robe"))
            {
               vnum = 164;
            }
    else if (!str_cmp(arg,"visor"))
            {
               vnum = 165;
            }
    else if (!str_cmp(arg,"gauntlets"))
            {
               vnum = 166;
            }
    else if (!str_cmp(arg,"bracers"))
            {
               vnum = 167;
            }
    else if (!str_cmp(arg,"sword"))
            {
               vnum = 168;
            }
    else if (!str_cmp(arg,"shield"))
            {
               vnum = 169;
            }
    else {
            do_holyarmor(ch,"");
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
    act("You pray to God silently as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$n softly goes to $s knees and pray as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_stasis_sword( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
     return;

  if ( !IS_NPC(ch) )
     {
        if ( !IS_CLASS(ch, CLASS_PALADIN) )
           return;

        if ( !IS_SET(ch->paladinab[SPIRIT],32) )
           {
               stc("You don't know how to do this technique holy knight.\n\r",ch);
               return;
           }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

        if ( ch->mana < 250000 )
           { 
              stc("You don't have enough magical energy for that.\n\r",ch);
              return;
           }
     }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to freeze?\n\r", ch );
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

  act("You say, '#yHold steady, Stasis Sword!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yHold steady, Stasis Sword!#n'",ch,NULL,NULL,TO_ROOM);
  act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
  act("Icy crystal falls from the sky as it scatters around $n's body.",victim,NULL,NULL,TO_ROOM);
  act("Icy crystal falls from the sky as it scatters around your body.",victim,NULL,NULL,TO_CHAR);
  act("$n screams loudly as the crystals form into an icy sword, encasing $s body in sub-zero ice.",victim,NULL,NULL,TO_ROOM);
  act("You scream loudly as the crystals form into an icy sword, encasing your body in sub-zero ice.",victim,NULL,NULL,TO_CHAR);

  if ( number_percent() > 96 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
     {
        victim->freeze_timer = number_range( 2, 4 ) / 2;
        SET_BIT( victim->more, MORE_FROZEN );
     }

  if ( !IS_NPC(ch) )
     ch->mana -= 250000;

  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_ICE;
  ch->attack_var = HIT;
  ch->attack_focus = DIRECT;
  ch->wait = 18;

  hurt_person(ch,victim,1000000,TRUE);
}

void do_holy_explosion( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
     return;

  if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_PALADIN) )
     return;

  one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to confuse?\n\r", ch );
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

  if ( !IS_NPC(ch) )
     {
        if ( !IS_SET(ch->paladinab[SPIRIT],512) )
        {
           stc("You don't know how to do this technique holy knight.\n\r",ch);
           return;
        }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

        if ( ch->mana < 250000 )
           { 
              stc("You don't have enough magical energy for that.\n\r",ch);
              return;
           }
     }

  act("You say, '#yInsanity is merely a perspection of the mind, Holy Explosion!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yInsanity is merely a perspection of the mind, Holy Explosion!#n'",ch,NULL,NULL,TO_ROOM);
  act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
  act("$n's entire body is engulfed by a beam of holy energy from the heavens.",victim,NULL,NULL,TO_ROOM);
  act("Your entire body is engulfed by a beam of holy energy from the heavens.",victim,NULL,NULL,TO_CHAR);
  act("As the energy filters through $n's body, $e cackles wildly with madness.",victim,NULL,NULL,TO_ROOM);
  act("As the energy filters through your body, you cackle wildly with madness.",victim,NULL,NULL,TO_CHAR);

  if ( number_percent() > 60 - devil_luck( ch, victim, 10 ) )
     {
        if ( !IS_SET(victim->more,MORE_CONFUSION) )
           SET_BIT(victim->more,MORE_CONFUSION);
     }

  if ( !IS_NPC(ch) )
     ch->mana -= 250000;

  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_HOLY;
  ch->attack_var = BEAM;
  ch->attack_focus = AERIAL;
  ch->wait = 18;
  hurt_person(ch,victim,1000000,TRUE);
}

void do_crush_punch( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
     return;

  if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_PALADIN) )
     return;

  one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to drain?\n\r", ch );
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

  if ( !IS_NPC(ch) )
     {
        if ( !IS_SET(ch->paladinab[SPIRIT],64) )
           {
              stc("You don't know how to do this technique holy knight.\n\r",ch);
              return;
           }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

        if ( ch->mana < 250000 )
           { 
              stc("You don't have enough magical energy for that.\n\r",ch);
              return;
           }
     }

  if ( !IS_NPC(ch) )
     ch->mana -= 250000;

  act("You say, '#yEarth's upcoming doom is certain, Crush Punch!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yEarth's upcoming doom is certain, Crush Punch!#n'",ch,NULL,NULL,TO_ROOM);
  act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
  act("Spirals of energy rises from the ground and caresses $n's body by swirling around $m.",victim,NULL,NULL,TO_ROOM);
  act("Spirals of energy rises from the ground and caresses your body by swirling around you.",victim,NULL,NULL,TO_CHAR);
  act("Suddenly the energy explodes draining $n of his vital energy.",victim,NULL,NULL,TO_ROOM);
  act("Suddenly the energy explodes draining you of your vital energy.",victim,NULL,NULL,TO_CHAR);

  ch->hit = UMIN( ch->hit + 100000, ch->max_hit );
  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_EARTH;
  ch->attack_var = SOUL;
  ch->attack_focus = UPPERCUT;
  ch->wait = 18;
  hurt_person( ch, victim, 1000000, TRUE );
}

void do_lightning_stab( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj = NULL;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
     return;

  if ( !IS_NPC(ch) )
     {
        if ( !IS_CLASS(ch, CLASS_PALADIN) )
           return;

        if ( !IS_SET(ch->paladinab[SPIRIT],128) )
           {
              stc("You don't know how to do this technique holy knight.\n\r",ch);
              return;
           }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

        if ( ch->mana < 250000 )
           { 
              stc("You don't have enough magical energy for that.\n\r",ch);
              return;
           }
     }

  one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to hex?\n\r", ch );
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

  act("You say, '#yMay the Force of Nature strike fear in your soul, Lightning Stab!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yMay the Force of Nature strike fear in your soul, Lightning Stab!#n'",ch,NULL,NULL,TO_ROOM);
  act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
  act("$n face madly contorts as a Huge Lightning bolt hits $m savagely.",victim,NULL,NULL,TO_ROOM);
  act("Your face madly contorts as a Huge Lightning bolt hits you savagely.",victim,NULL,NULL,TO_CHAR);
  act("An electrifying Sword forms around $n short-circuiting $s body.",victim,NULL,NULL,TO_ROOM);
  act("An electrifying Sword forms around you short-circuiting your body.",victim,NULL,NULL,TO_CHAR);

  if ( number_percent() > 40 - devil_luck( ch, victim, 10 ) )
     {
        if ( !IS_SET(victim->more,MORE_HEX) )
           SET_BIT(victim->more,MORE_HEX);
     }

  if ( !IS_NPC(ch) )
     ch->mana -= 250000;

  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_THUNDER;
  ch->attack_var = BLUNT;
  ch->attack_focus = AERIAL;

  ch->wait = 18;
  hurt_person(ch,victim,1000000,TRUE);
}

void do_split_punch( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];
  int dam = 1000000;

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
     return;

  if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_PALADIN) )
     return;

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

  if ( !IS_NPC(ch) )
     {
        if ( !IS_SET(ch->paladinab[SPIRIT],256) )
        {
            stc("You don't know how to do this technique holy knight.\n\r",ch);
            return;
        }

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

        if ( ch->mana < 500000 )
           { 
              stc("You don't have enough magical energy for that.\n\r",ch);
              return;
           }
     }

  act("You say, '#yThere is no such thing as a third state, Split Punch!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yThere is no such thing as a third state, Split Punch!#n'",ch,NULL,NULL,TO_ROOM);
  act("You swing your sword in a down arch towards the ground.",ch,NULL,victim,TO_CHAR);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_VICT);
  act("$n swings $s sword in a down arch towards the ground.",ch,NULL,victim,TO_NOTVICT);
  act("A Huge Sword rises underneath the ground thrusting through $n body.",victim,NULL,NULL,TO_ROOM);
  act("A Huge Sword rises underneath the ground thrusting through your body.",victim,NULL,NULL,TO_CHAR);
  act("$n cries out loudly as the Sword rips $n innards apart as it flies into the sky.",victim,NULL,NULL,TO_ROOM);
  act("You cry out loudly as the Sword rips your innards apart as it flies into the sky.",victim,NULL,NULL,TO_CHAR);

  if ( number_percent() > 94 - devil_luck( ch, victim, 5 ) )
     {
        act("$n spews out blood as $e collapses onto the ground unconscious.",victim,NULL,NULL,TO_ROOM);
        act("You spew out blood as you collapse onto the ground unconscious.",victim,NULL,NULL,TO_CHAR);
        dam *= 1.5;
     }

  if ( !IS_NPC(ch) )
     ch->mana -= 500000;

  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_EARTH;
  ch->attack_var = PIERCE;
  ch->attack_focus = UPPERCUT;

  ch->wait = 18;
  hurt_person( ch, victim, dam, TRUE );
}

void do_godsread( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_PALADIN) )
       return;

    if ( !IS_SET(ch->paladinab[SPIRIT],4) )
       {
          stc("You don't know how to do this technique holy knight.\n\r",ch);
          return;
       }

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

    if ( IS_CLASS(victim, CLASS_PALADIN) )
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

void do_godlysight( CHAR_DATA *ch, char *argument )
{
	if ( IS_NPC(ch) )  return;

	if ( !IS_CLASS(ch, CLASS_PALADIN) )
	{
		stc("Huh?\n\r",ch);
		return;
	}

       if ( !IS_SET(ch->paladinab[SPIRIT],1) )
       {
          stc("You don't know how to do this technique holy knight.\n\r",ch);
          return;
       }

	send_to_char("You pause a moment and close your eyes.\n\r",ch);

	if (IS_SET(ch->act, PLR_HOLYLIGHT) )
	{
		REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		send_to_char("When you open them you've stopped mimicking god's vision.\n\r",ch);
	}
	else
	{
		send_to_char("When you open them you've started mimicking god's vision.\n\r",ch);
		SET_BIT(ch->act, PLR_HOLYLIGHT);
	}
	return;
}

void do_justice_cutter( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  int dam = 0;
  char      arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  argument = one_argument( argument, arg );

  if ( !IS_CLASS(ch, CLASS_PALADIN) )
     return;

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

  if ( !IS_NPC(ch) )
     {
        if ( ch->pkill < 5 )
           { 
              stc("#yYour #Cfaith #win the #yLight #wisn't strong enough to do this technique.#n\n\r",ch);
              return;
           }

        if ( ch->hit < 100000 )
           { 
              stc("You don't have vitality for that.\n\r",ch);
              return;
           }

        if ( ch->move < 1000000 )
           { 
              stc("You are too tired to do this technique.\n\r",ch);
              return;
           }
     }

  if ( arg[0] == '\0' )
     {
	    send_to_char( "Do this on whom?\n\r", ch );
	    return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
          send_to_char( "They aren't here.\n\r", ch );
          return;
     }

  if ( victim == ch ) 
     {
        stc("Not on yourself!\n\r",ch);
        return;
     }

  if ( is_safe(ch,victim) )
     return;

  if ( IS_CLASS(victim, CLASS_PALADIN) )
     {
        stc("Your victim seems to dodge your attack with ease.\n\r",ch);
        return;
     }

  if ( IS_ARM_L(victim,LOST_ARM) )
     {
        stc("Your victim dont have a left arm.\n\r",ch);
        return;
     }

  ch->hit -= 100000;
  ch->move -= 1000000;
  ch->wait = 4;

  act("Holy energy converges to the tip of your sword as you rush to $N.",ch,NULL,victim,TO_CHAR);
  act("Holy energy converges to the tip of $n's sword as $e rushes straight towards you.",ch,NULL,victim,TO_VICT);
  act("Holy energy converges to the tip of $n's sword as $e rushes to $N.",ch,NULL,victim,TO_NOTVICT);
  act("You violently swipe your sword to the left as palish energy follow your slash.",ch,NULL,victim,TO_CHAR);
  act("$n violently swipes $s sword to the left as palish energy follow $s slash.",ch,NULL,victim,TO_ROOM);
  act("You then rapidly spin your blade as it arches upwards $N's body.",ch,NULL,victim,TO_CHAR);
  act("$n then rapidly spins $s blade as it arches upwards your body.",ch,NULL,victim,TO_VICT);
  act("$n then rapidly spins $s blade as it arches upwards $N's body.",ch,NULL,victim,TO_NOTVICT);
  act("Finally, you savagely thrust your sword deep into $N's body.",ch,NULL,victim,TO_CHAR);
  act("Finally, $n savagely thrust $s sword deep into your body.",ch,NULL,victim,TO_VICT);
  act("Finally, $n savagely thrust $s sword deep into $N's body.",ch,NULL,victim,TO_NOTVICT);

  dam = 300000 + UMIN( 200000, ch->pkill * 10000 );
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SLASH;
  ch->attack_focus = DIRECT;

  if ( !IS_ARM_L(victim,LOST_ARM) )
     {
         SET_BIT(victim->loc_hp[2],LOST_ARM);
         SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	   make_part(victim,"arm");
         if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
            {
               if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
               take_item(victim,obj);
	      }
         if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
            take_item(victim,obj);
     }

  if ( !IS_LEG_R(victim,LOST_LEG) )
     {
	   SET_BIT(victim->loc_hp[5],LOST_LEG);
	   SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
  	   make_part(victim,"leg");
	   if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	      {
	         if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
		      take_item(victim,obj);
	      }
	   if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	      take_item(victim,obj);
     }

  if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4) )
     SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);

  if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2) )
     SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);

  if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1) )
     SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);

  hurt_person( ch, victim, dam, TRUE );
}

void do_subdue( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char      arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  argument = one_argument( argument, arg );

  if ( !IS_CLASS(ch, CLASS_PALADIN) )
     return;

  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
        {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
        }

  if ( !IS_NPC(ch) )
     {
        if ( ch->pkill < 20 )
           { 
              stc("#yYour #Cfaith #win the #yLight #wisn't strong enough to do this technique.#n\n\r",ch);
              return;
           }

        if ( ch->ctimer[0] > 0 )
           { 
              stc("You are still recovering from the last one.\n\r",ch);
              return;
           }

        if ( ch->move < 1000000 )
           { 
              stc("You are too tired to do this technique.\n\r",ch);
              return;
           }
     }

  if ( arg[0] == '\0' )
     {
	    send_to_char( "Do this on whom?\n\r", ch );
	    return;
     }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
     {
          send_to_char( "They aren't here.\n\r", ch );
          return;
     }

  if ( is_safe(ch,victim) )
     return;

  if ( IS_CLASS(victim, CLASS_PALADIN) )
     {
        stc("Your victim seems to dodge your attack with ease.\n\r",ch);
        return;
     }

  if ( IS_SET( victim->extra, EXTRA_SUBDUE ) )
     {
        stc("Their actions are already subdued.\n\r",ch);
        return;
     }

  ch->ctimer[0] = 6;
  ch->move -= 1000000;
  ch->wait = 4;
  SET_BIT( victim->extra, EXTRA_SUBDUE );

  act("Holy energy converges to the tip of your sword as you rush to $N.",ch,NULL,victim,TO_CHAR);
  act("Holy energy converges to the tip of $n's sword as $e rushes straight towards you.",ch,NULL,victim,TO_VICT);
  act("Holy energy converges to the tip of $n's sword as $e rushes to $N.",ch,NULL,victim,TO_NOTVICT);
  act("You release your sword as it appears to go straight through $N's body!",ch,NULL,victim,TO_CHAR);
  act("$n releases $s sword as it appears to go straight through your body!",ch,NULL,victim,TO_VICT);
  act("$n releases $s sword as it appears to go straight through $N's body!",ch,NULL,victim,TO_NOTVICT);
  act("You then go on your knees and drape your arms aside as the sword reappears!!!",ch,NULL,NULL,TO_CHAR);
  act("$n then goes on $s knees and drapes $s arms aside as the sword reappears!!!",ch,NULL,NULL,TO_ROOM);

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = DIRECT;

  hurt_person( ch, victim, 300000, TRUE );
}

void do_godsheal( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   int i, test = 0;
   int bonus = 300000;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) return;

   if ( ( !IS_CLASS(ch, CLASS_PALADIN) && !IS_CLASS(ch, CLASS_ANGEL) ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( IS_CLASS(ch, CLASS_PALADIN) && !IS_SET(ch->paladinab[SPIRIT],32) )
      {
         stc("You don't know how to do this technique holy knight.\n\r",ch);
         return;
      }

   victim = ch;

   for( i = 0; i < 7; i++ )
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

   if ( victim->loc_hp[6] > 0 )
      {
         stc("Your wounds close up and stop bleeding.\n\r",victim);
         victim->loc_hp[6] = 0;
      }

   act( "You go on your knees and silently pray to God for salvation.", ch, NULL, NULL, TO_CHAR );
   act( "$n goes on $s knees and silently prays to God for salvation.", ch, NULL, NULL, TO_ROOM );
   act( "You say softly '#yAll is graceful before the Light.#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n says softly '#yAll is graceful before the Light.#n'", ch, NULL, NULL, TO_ROOM );

   if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + victim->loc_hp[3] + victim->loc_hp[4] +
          victim->loc_hp[5]) != 0 )
      {
         stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",victim);
         victim->loc_hp[0] = 0;
         victim->loc_hp[1] = 0;
         victim->loc_hp[2] = 0;
         victim->loc_hp[3] = 0;
         victim->loc_hp[4] = 0;
         victim->loc_hp[5] = 0;
      }

   act( "You feel a warm presence comforting you as your wounds fade away.", victim, NULL, NULL, TO_CHAR );
   act( "$n feels a warm presence comforting $m as $s wounds fade away.", victim, NULL, NULL, TO_ROOM );

   if ( ch->fight_timer == 0 )
      bonus = 5000000;

   if ( !IS_NPC( ch ) && ch->pcdata->personality == PER_COMPASSION )
      bonus += 200000;

   hit_gain( victim, bonus );
   mana_gain( victim, bonus );
   move_gain( victim, bonus );
 
   ch->wait = 12;
}

void do_godssearch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];
    bool safe = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_PALADIN) || IS_SET( ch->special, SPC_SWORD ) )
      {
	   send_to_char("Huh?\n\r", ch );
	   return;
      }

    if ( !IS_SET(ch->paladinab[SPIRIT],2) )
       {
          stc("You don't know how to do this technique holy knight.\n\r",ch);
          return;
       }
	
      if (arg[0] == '\0')
    {
	send_to_char( "Search fon whom?\n\r", ch );
	return;
    }

    if (!IS_NPC(ch) && IS_SET(ch->more, MORE_SAFE)) safe = TRUE;

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim,AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim,AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    act("A look on concentration appears on $n's face.",ch,NULL,NULL,TO_ROOM);
    ch->wait = 12;

    if ( safe )
       SET_BIT(ch->more, MORE_SAFE);
}

void do_soul_hammer( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  int dam = 1000000;
  char arg [MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
     return;

  if ( !IS_CLASS(ch, CLASS_PALADIN) || IS_SET( ch->special, SPC_SWORD ) )
     return;

  if ( !IS_NPC(ch) )
     {  
        if ( get_stock( ch ) < 100 )
           {
              stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
              return;
           }
     }

  one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to strike down?\n\r", ch );
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

  ch->wait = 8;

  if ( !IS_NPC(ch) )
     { 
        act("You say, '#yNia vira zojara tlwes...#n'",ch,NULL,NULL,TO_CHAR);
        act("$n says, '#yNia vira zojara tlwes...#n'",ch,NULL,NULL,TO_ROOM);
        SET_BIT( ch->arena, AFF2_DM );
        subtract_stock( ch, 100 );

        if ( !IS_NPC( victim ) || ( victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum <= 561 ) )
           dam = 1000000;
           else dam = victim->max_hit;
	}

  act("A blurry hammer shimmers into existence as it hovers in the air.",ch,NULL,NULL,TO_CHAR);
  act("A blurry hammer shimmers into existence as it hovers in the air.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, you grab the spirtual hammer and CrUsH $N's very soul mightly.",ch,NULL,victim,TO_CHAR);
  act("Suddenly, $n grabs the spirtual hammer and CrUsHeS your very soul mightly.",ch,NULL,victim,TO_VICT);
  act("Suddenly, $n grabs the spirtual hammer and CrUsHeS $N's very soul mightly.",ch,NULL,victim,TO_NOTVICT);

  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_HOLY;  
  ch->attack_var = SOUL;
  ch->attack_focus = OVERHEAD;

  hurt_person(ch,victim,dam,TRUE);
}

void do_flash_sword( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 33101 ) 
       return;

    ch->wait = 12;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_PALADIN) )
             return;

          if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
             if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                {
                   send_to_char( "But you are not holding a sword.\n\r", ch );
                   return;
                }

          if ( get_stock( ch ) < 200 )
             {
                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                return;
             }

          SET_BIT( ch->arena, AFF2_DM );
          subtract_stock( ch, 200 );
       }

    act("You leap back a few feet as you raise your sword above you.",ch,NULL,NULL,TO_CHAR);
    act("$n leaps back a few feet as $e raises $s sword above $m.",ch,NULL,NULL,TO_ROOM);
    act("You gaze towards the heavens as your sword flashes with holy energy.",ch,NULL,NULL,TO_CHAR);
    act("$n gazes towards the heavens as $s sword flashes with holy energy.",ch,NULL,NULL,TO_ROOM);
    act("Your sword starts vibrating as the ground trembles beneath your feet.",ch,NULL,NULL,TO_CHAR);
    act("$n's sword starts vibrating as the ground trembles beneath $s feet.",ch,NULL,NULL,TO_ROOM);
    act("Your body flares with divine energy as you concentrate furiously.",ch,NULL,NULL,TO_CHAR);
    act("$n's body flares with divine energy as $e concentrates furiously.",ch,NULL,NULL,TO_ROOM);
    act("The room is FiLlEd with MASSIVE eXpLoSiOnS as a wave of blinding light flashes across the room.",ch,NULL,NULL,TO_CHAR);
    act("The room is FiLlEd with MASSIVE eXpLoSiOnS as a wave of blinding light flashes across the room.",ch,NULL,NULL,TO_ROOM);
    act("Your body goes black against the light as a spiral of divine energy strikes at your foes.",ch,NULL,NULL,TO_CHAR);
    act("$n's body goes black against the light as a spiral of divine energy strikes at $s foes.",ch,NULL,NULL,TO_ROOM);
 
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;
       
        if ( is_same_group(ch,vch) || is_safe(ch,vch) ) 
           continue;

        act("The light SmAsHeS through your defenseless body!!!",vch,NULL,NULL,TO_CHAR);
        act("The light SmAsHeS through $n's defenseless body!!!",vch,NULL,NULL,TO_ROOM);

        if ( number_percent() >= 50 && !IS_SET( vch->more, MORE_BLIND ) )
           { 
              SET_BIT( vch->more, MORE_BLIND );
              vch->blind_timer = 6;
           }

        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_HOLY;
        ch->attack_var = BLAST;
        ch->attack_focus = UPPERCUT;

        if ( !IS_NPC( ch ) )
           hurt_person( ch, vch, 2000000, TRUE );
           else hurt_person( ch, vch, 1000000, TRUE );
    }

    if ( !IS_NPC( ch ) )
       ch->mana -= 500000;
}

void do_eruption_sword( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *obj;
    int dam;

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 33101 ) 
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_PALADIN) )
             return;

          if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
             if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                {
                   send_to_char( "But you are not holding a sword.\n\r", ch );
                   return;
                }

          if ( get_stock( ch ) < 300 )
             {
                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                return;
             }

          SET_BIT( ch->arena, AFF2_DM );
          subtract_stock( ch, 300 );
       }

    ch->wait = 12;

    act("You leap back a few feet as you raise your sword above you.",ch,NULL,NULL,TO_CHAR);
    act("$n leaps back a few feet as $e raises $s sword above $m.",ch,NULL,NULL,TO_ROOM);
    act("You gaze towards the heavens as your sword flashes with holy energy.",ch,NULL,NULL,TO_CHAR);
    act("$n gazes towards the heavens as $s sword flashes with holy energy.",ch,NULL,NULL,TO_ROOM);
    act("MaSsIvE explosions manifests around your sword as it turns blazing red.",ch,NULL,NULL,TO_CHAR);
    act("MaSsIvE explosions manifests around $n's sword as it turns blazing red.",ch,NULL,NULL,TO_ROOM);
    act("You leap into the air as palish energy fades behind you.",ch,NULL,NULL,TO_CHAR);
    act("$n leaps into the air as palish energy fades behind $m.",ch,NULL,NULL,TO_ROOM);
    act("You ViCoUsLy THRUST your sword into the ground as you land back onto your feet!!!",ch,NULL,NULL,TO_CHAR);
    act("$n ViCoUsLy THRUSTS $s sword into the ground as $e lands back onto $s feet!!!",ch,NULL,NULL,TO_ROOM);
    act("The entire ground TrEmBlEs as spurts of light blasts into the air!",ch,NULL,NULL,TO_CHAR);
    act("The entire ground TrEmBlEs as spurts of light blasts into the air!",ch,NULL,NULL,TO_ROOM);
 
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;
       
        if ( is_same_group(ch,vch) ) 
           continue;

        act("You scream as the explosions sends you flying across the room dazed!!!",vch,NULL,NULL,TO_CHAR);
        act("$n screams as the explosions sends $m flying across the room dazed!!!",vch,NULL,NULL,TO_ROOM);

        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_FIRE;
        ch->attack_var = BLAST;
        ch->attack_focus = UPPERCUT;
        hurt_person( ch, vch, 3500000, TRUE );
    }

    act("The entire area bursts into flames as an AtOmIc explosion fills the room in one FiNaL outburst!!!",ch,NULL,NULL,TO_CHAR);
    act("The entire area bursts into flames as an AtOmIc explosion fills the room in one FiNaL outburst!!!",ch,NULL,NULL,TO_ROOM);
}

void do_nemesis( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *friend;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_PALADIN) || IS_SET( ch->special, SPC_SWORD ) )
      return;

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
         }

   friend = ch->in_room->people;   

   while ( friend != NULL )
   {
      if ( is_same_group( ch, friend ) && ch != friend && IS_CLASS( friend, CLASS_MONK ) 
           && friend->move >= 1000000 && friend->focus[CURRENT] >= 85 && friend->nemesis == 0
           && friend->position > POS_STUNNED )
         break;

      friend = friend->next_in_room;
   }

   if ( friend == NULL )
      {
         send_to_char("You dont have the right people to do the technique with you.\n\r",ch);
         return;
      }

   if ( ch->mana < 500000 )
      {
         act("You don't have enough magical energy for that.",ch,NULL,NULL,TO_CHAR);
         return;
      }

   friend->move -= 1000000;
   ch->mana -= 500000;
   friend->focus[CURRENT] -= 85;

   act("\n\rYou silently pull your sword towards you as you look straight into the heavens.",ch,NULL,NULL,TO_CHAR);
   act("\n\r$n silently pulls $s sword towards $m as $e look straight into the heavens.",ch,NULL,NULL,TO_ROOM);
   act("A holy wave of energy swirls around your body as you mutter the ancient language of the holy word.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters repeatedly, '#yDies unkli zxne kf...#n' as A holy wave of energy swirls around $s body.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly $n is surrounded by blazing energy as $e looks towards $s ever glowing right hand.",friend,NULL,NULL,TO_ROOM);
   act("Suddenly you are surrounded by blazing energy as you look down at your ever glowing right hand.",friend,NULL,NULL,TO_CHAR);

   act("You yell '#yUnnnn RekKa DueDANT!#n'",friend,NULL,NULL,TO_CHAR);
   act("$n yells '#yUnnnn RekKa DueDANT!#n'",friend,NULL,NULL,TO_ROOM);

   act("With your left hand in front of you, you rush straight pass your victims running into the distance.",friend,NULL,NULL,TO_CHAR);
   act("With $s left hand in front of $m, $n rushes straight pass you as $e dashes into the distance.",friend,NULL,NULL,TO_ROOM);
   act("Sparkles of holy energy pale of $n's body as it flows into the distance after $n.",friend,NULL,NULL,TO_ROOM);
   ch->wait = 24;
   friend->wait = 24;
   ch->nemesis = 4;
   stop_fighting( friend, TRUE );
   friend->nemesis = 4;

   friend->timer = 0;
   friend->was_in_room = friend->in_room;

   char_from_room( friend );
   char_to_room( friend, get_room_index( 2 ) );
}

void do_my_final_heaven( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *fch, *fch_next;
    
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room = NULL;

    act("You blaze into the room rushing straight towards your target.",ch,NULL,NULL,TO_CHAR);
    act("$n blazes into the room rushing straight towards you, the light from $s hand blinding you.",ch,NULL,NULL,TO_ROOM);
    act("As you smash through your target, you brace your body as the entire area ExPlOdEs ViOlEnTlY!!!",ch,NULL,NULL,TO_CHAR);
    act("As $n SmAsHeS tHrOuGh $s TaRgEt, $e BrAcEs $s BoDy As ThE eNtIrE ArEa ExPlOdEs ViOlEnTlY!!!",ch,NULL,NULL,TO_ROOM);
    act("Explosive EnErGy detonates the room with a MASSIVE pounding!!",ch,NULL,NULL,TO_ROOM);
    act("Explosive eNeRgY detonates the room with a MASSIVE pounding!!",ch,NULL,NULL,TO_CHAR);
    ch->wait = 0;

    for ( fch = char_list; fch != NULL; fch = fch_next )
    {
        fch_next = fch->next;

        if ( fch->in_room->area != ch->in_room->area )
           continue;

        if ( is_same_group( ch, fch ) )
           continue;

        if ( is_safe( ch, fch ) )
           continue;

        fch->wait += 36;

        if ( fch->in_room != ch->in_room )
           {
              act("You stagger backwards as a wave of energy soars through your defenseless body.",fch,NULL,NULL,TO_CHAR);
              act("$n staggers backwards as a wave of energy soars through $s defenseless body.",fch,NULL,NULL,TO_ROOM);
              fch->hit = UMAX( 1, fch->hit - 1000000 );  
              continue;
           }

        act("The explosive energy flashes through your body as your molecules gets disrupted.",fch,NULL,NULL,TO_CHAR);
        act("The explosive energy flashes through $n body as $s molecules gets disrupted.",fch,NULL,NULL,TO_ROOM);

        ch->attack_type = ATTACK_MAGICAL;
        ch->ele_attack = ATTACK_HOLY;
        ch->attack_var = SOUL;
        ch->attack_focus = DIRECT;
        hurt_person( ch, fch, 2000000, TRUE );
    }
}

void do_genesis( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *monk, *angel, *vch, *vch_next;
   OBJ_DATA *obj;

   if ( IS_NPC( ch ) ) 
      return;

   if ( IS_CLASS( ch, CLASS_ANGEL ) )
      {
         do_angelic_genesis( ch, argument );
         return;
      }

   if ( !IS_CLASS( ch, CLASS_PALADIN ) || IS_SET( ch->special, SPC_SWORD ) )
      return;

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "But you are not holding a sword.\n\r", ch );
            return;
         }

   if ( ch->mana < 1500000 )
      {
         act("You don't have enough magical energy for that.",ch,NULL,NULL,TO_CHAR);
         return;
      }

   monk = ch->in_room->people;   

   while ( monk != NULL )
   {
      if ( is_same_group(ch,monk) && ch != monk && IS_CLASS( monk, CLASS_MONK ) 
           && monk->position >= POS_FIGHTING && monk->move >= 1500000 && monk->focus[CURRENT] > 84 )
         break;

      monk = monk->next_in_room;
   }

   if ( monk == NULL )
      {
         send_to_char("You dont have the right people to do the technique with you.\n\r",ch);
         return;
      }

   angel = ch->in_room->people;   

   while ( angel != NULL )
   {
      if ( is_same_group(ch,angel) && ch != angel && monk != angel && IS_CLASS( angel, CLASS_ANGEL )
           && angel->position >= POS_FIGHTING && angel->mana >= 1500000 )
         break;

      angel = angel->next_in_room;
   }

   if ( angel == NULL )
      {
         send_to_char("You dont have the right people to do the technique with you.\n\r",ch);
         return;
      }

   monk->move -= 1500000;
   monk->focus[CURRENT] -= 85;
   ch->mana -= 1500000;
   angel->mana -= 1500000;

   act("You mutter '#yZiay ni eria Liun...#n' as you raise your sword to your face.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yZiay ni eria Liun...#n' as $e raises $s sword to $s face.",ch,NULL,NULL,TO_ROOM);
   act("You say '#yThe time has come...#n' as a spiral of holy energy encircles you.",angel,NULL,NULL,TO_CHAR);
   act("$n say '#yThe time has come...#n' as a spiral of holy energy encircles $m.",angel,NULL,NULL,TO_ROOM);
   act("You crouch forward on your knees as you tilt your hands together.",monk,NULL,NULL,TO_CHAR);
   act("$n crouches forward on $s knees as $e tilts $s hands together.",monk,NULL,NULL,TO_ROOM);
   act("You mutter '#ySui Aski qmiou ti...#n' as light converges up your blade.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#ySui Aski qmiou ti...#n' as light converges up your blade.",ch,NULL,NULL,TO_ROOM);
   act("You say '#yThe dark light of nothingness consumes all...#n' as a flash of light fills the room.",angel,NULL,NULL,TO_CHAR);
   act("$n says '#yThe dark light of nothingness consumes all...#n' as a flash of light fills the room.",angel,NULL,NULL,TO_ROOM);
   act("You grin evilly as the very air around you rips open and flashes of dark energy soars from it.",angel,NULL,NULL,TO_CHAR);
   act("$n grins evilly as the very air around $m rips open and flashes of dark energy soars from it.",angel,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( is_same_group(ch,vch) ) 
         continue;

      if ( is_safe(ch,vch) ) 
         continue;

      act("Your body dissolves from the negative effects of the dark light!",vch,NULL,NULL,TO_CHAR);
      act("$n's body dissolves from the negative effects of the dark light!",vch,NULL,NULL,TO_ROOM);
      vch->hit -= 1000000;
   }

   act("\n\rYou mutter '#yOOoooooo...#n' as sparks of electricity fizzes between your hands.",monk,NULL,NULL,TO_CHAR);
   act("\n\r$n mutters '#yOOoooooo...#n' as sparks of electricity fizzes between $s hands.",monk,NULL,NULL,TO_ROOM);
   act("You mutter '#yAi sui Allah ni hunta...#n' as holy symbols transcede from your body.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yAi sui Allah ni hunta...#n' as holy symbols transcede from $n's body.",ch,NULL,NULL,TO_ROOM);
   act("You say '#yAnd God says Let there be Light...#n' as a loud hum fills the vicinity.",angel,NULL,NULL,TO_CHAR);
   act("$n says '#yAnd God says Let there be Light...#n' as a loud hum fills the vicinity.",angel,NULL,NULL,TO_ROOM);
   act("You yell '#yZodora Kou Houden!#n' as you release a MaSsIvE beam of energy across the room!",monk,NULL,NULL,TO_CHAR);
   act("$n yells '#yZodora Kou Houden!#n' as $e releases a MaSsIvE beam of energy across the room!",monk,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( is_same_group(ch,vch) ) 
         continue;

      if ( is_safe(ch,vch) ) 
         continue;

      act("The beam of energy RiPs through your body as you are blasted away!",vch,NULL,NULL,TO_CHAR);
      act("The beam of energy RiPs through $n's body as $e is blasted away!",vch,NULL,NULL,TO_ROOM);
      vch->hit -= 1000000;
   }

   act("\n\rYou mutter '#yNu de Alpi zi Omeas, Sui quon dai per...#n' as you struggle to clutch your trembling sword.",ch,NULL,NULL,TO_CHAR);
   act("\n\r$n mutters '#yNu de Alpi zi Omeas, Sui quon dai per...#n' as $e struggles to clutch $s trembling sword.",ch,NULL,NULL,TO_ROOM);
   act("You say '#yFrom Alpha to Omega, the beginning is the end...#n' as the room turns slowly into the void.",angel,NULL,NULL,TO_CHAR);
   act("$n says '#yFrom Alpha to Omega, the beginning is the end...#n' as the room turns slowly into the void.",angel,NULL,NULL,TO_ROOM);
   act("Everything starts rotating around you as a sparkle flashes in the distance.",angel,NULL,NULL,TO_CHAR);
   act("Everything starts rotating around you as a sparkle flashes in the distance.",angel,NULL,NULL,TO_ROOM);
   act("Comets fly by you as you are drawn towards a black hole!",angel,NULL,NULL,TO_CHAR);
   act("Comets fly by you as you are drawn towards a black hole!",angel,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( is_same_group(ch,vch) ) 
         continue;

      if ( is_safe(ch,vch) ) 
         continue;

      if ( !IS_AFFECTED(vch, AFF_FLAMING) ) 
         SET_BIT(vch->affected_by, AFF_FLAMING);

      if ( !IS_SET(vch->more, MORE_POISON) ) 
         SET_BIT(vch->more, MORE_POISON);

      if ( !IS_SET(vch->more, MORE_CONFUSION) ) 
         SET_BIT(vch->more, MORE_CONFUSION);

      if ( !IS_SET(vch->more, MORE_HEX) ) 
         SET_BIT(vch->more, MORE_HEX);

      if ( !IS_SET(vch->more, MORE_PINCH) ) 
         SET_BIT(vch->more, MORE_PINCH);

      if ( !IS_SET(vch->more, MORE_PAIN) ) 
         SET_BIT(vch->more, MORE_PAIN);

      if ( !IS_SET(vch->more, MORE_BLIND) ) 
         SET_BIT(vch->more, MORE_BLIND);

      act("You hear a demonic shriek fills the air as powerful vibrations shatters your body!",vch,NULL,NULL,TO_CHAR);
      act("$n hears a demonic shriek fills the air as powerful vibrations shatters $s body!",vch,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 2000000, TRUE );
   }

   act("\n\rYou mutter '#yIt is fuitile to resist the Light...#n' as you turn around slowly and fold your arms.",ch,NULL,NULL,TO_CHAR);
   act("\n\r$n mutters '#yIt is fuitile to resist the Light...#n' as $e turns around slowly and fold $s arms.",ch,NULL,NULL,TO_ROOM);
}

void do_no_mercy( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim = NULL;
   int dam = 2000000;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
      return;

   if ( !IS_NPC(ch) )
      {  
         OBJ_DATA *obj;
         char arg [MAX_INPUT_LENGTH];

         if ( !IS_CLASS( ch, CLASS_PALADIN ) || ( ch->clannum != 1 && !IS_SET( ch->special, SPC_SWORD ) ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
            if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
               {
                  send_to_char( "But you are not holding a sword.\n\r", ch );
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
               send_to_char( "Whom do you want to strike?\n\r", ch );
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

         ch->wait = 12;
         subtract_stock( ch, 200 );
      }
      else if ( ch->fighting != NULL ) 
              victim = ch->fighting;
      else {
              stc("But you arenot in a fight.\n\r",ch);
              return;
           }

   act("\n\r$n bent down to one knee grasping $s weapon as $s gasps for breath.",ch,NULL,NULL,TO_ROOM);
   act("\n\rYou bent down to one knee grasping your weapon as you gasp for breath.",ch,NULL,NULL,TO_CHAR);
   act("You say, '#ySo you think you have me eh?... I think not.#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#ySo you think you have me eh?... I think not.#n'",ch,NULL,NULL,TO_ROOM);
   act("\nYour right hand starts glowing brightly as you raise it high above your body.",ch,NULL,NULL,TO_CHAR);
   act("\n$n's right hand starts glowing brightly as $e raise it high above $s body.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly a blast of energy shoots from $s hand stunning you making you rub your eyes.",ch,NULL,victim,TO_VICT);
   act("You suddenly release a blast of energy from your hand stunning $N as $E rubs $S eyes.",ch,NULL,victim,TO_CHAR);
   act("Suddenly a blast of energy shoots from $s hand stunning $N causing $M to rubs $S eyes.",ch,NULL,victim,TO_NOTVICT);
   stc("The energy blast blinds you.\n\r",victim);
   act("$n is blinded from the energy blast.",victim,NULL,NULL,TO_ROOM);

   if ( IS_SET( victim->more, MORE_BLIND ) )
      SET_BIT( victim->more, MORE_BLIND );

   act("Sparkles of energy converges to the tip of your weapon as it starts trembling.",ch,NULL,NULL,TO_CHAR);
   act("Sparkles of energy converges to the tip of $n's weapon as it starts trembling.",ch,NULL,NULL,TO_ROOM);
   act("An evil grin appears on your face as time becomes distorted.",ch,NULL,NULL,TO_CHAR);
   act("An evil grin appears on $n's face as time becomes distorted.",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yNO MeRcY!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNO MeRcY!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You lash your weapon in a 720 degree angle sending a huge shockwave into $N.",ch,NULL,victim,TO_CHAR);
   act("$n lashes $s weapon in a 720 degree angle sending a huge shockwave into $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n lashes $s weapon in a 720 degree angle sending a huge shockwave straight into YOU.",ch,NULL,victim,TO_VICT);
   act("You feel as if half your soul was ReMoVeD fRoM EXISTENCE as the ShOcKwAvE paralyses your body.\n\r",ch,NULL,victim,TO_VICT);
   act("$n's body becomes blurry as the ShOcKwAvE paralyses $s body.\n\r",victim,NULL,NULL,TO_ROOM);

   if ( !is_martial( victim ) && !IS_CLASS( victim, CLASS_WISP ) )
      victim->wait = 18;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_focus = OVERHEAD;
   ch->attack_var = BLUNT;
   hurt_person( ch, victim, dam, TRUE );
}

void do_demon_slice( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;
   CHAR_DATA *victim = NULL;

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 550 && ch->pIndexData->vnum != 557 && ch->pIndexData->vnum != 552 ) 
      return;

   if ( !IS_NPC(ch) )
      {  
         OBJ_DATA *obj;
         char arg [MAX_INPUT_LENGTH];

         if ( !IS_CLASS( ch, CLASS_PALADIN ) || ( ch->clannum != 1 && !IS_SET( ch->special, SPC_SWORD ) ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
            if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
               {
                  send_to_char( "But you are not holding a sword.\n\r", ch );
                  return;
               }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }

         one_argument( argument, arg );

         if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
            {
               send_to_char( "Whom do you want to strike?\n\r", ch );
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

         ch->wait = 12;
         subtract_stock( ch, 300 );
      }
      else if ( ch->fighting != NULL ) 
              victim = ch->fighting;
      else {
              stc("But you arenot in a fight.\n\r",ch);
              return;
           }
  
   act("\n$n's right hand starts glowing brightly as $e raise it high above $s body.",ch,NULL,NULL,TO_ROOM);
   act("\nYour right hand starts glowing brightly as you raise it high above your body.",ch,NULL,NULL,TO_CHAR);
   act("You suddenly release a blast of energy from your hand stunning $N as $E rubs $S eyes.",ch,NULL,victim,TO_CHAR);
   act("Suddenly a blast of energy shoots from $s hand stunning you making you rub your eyes.",ch,NULL,victim,TO_VICT);
   act("Suddenly a blast of energy shoots from $s hand stunning $N causing $M to rubs $S eyes.",ch,NULL,victim,TO_NOTVICT);

   act("Sparkles of energy converges to the tip of your weapon as it starts trembling.",ch,NULL,NULL,TO_CHAR);
   act("Sparkles of energy converges to the tip of $n's weapon as it starts trembling.",ch,NULL,NULL,TO_ROOM);
   act("Twirling your weapon viciously, you rush straight towards $N.",ch,NULL,victim,TO_CHAR);
   act("Twirling $s weapon viciously, $n rushes straight towards $N.",ch,NULL,victim,TO_NOTVICT);
   act("Twirling $s weapon viciously, $n rushes straight towards you.",ch,NULL,victim,TO_VICT);
   act("You say '#yDeMOn SlIcE!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yDeMOn SlIcE!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_ARM_L(victim,LOST_ARM) )
      {
         SET_BIT(victim->loc_hp[2],LOST_ARM);
         SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	   make_part(victim,"arm");
         if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
            {
               if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
               take_item(victim,obj);
	      }
         if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
            take_item(victim,obj);
      }

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_LEG_R(victim,LOST_LEG) )
      {
	   SET_BIT(victim->loc_hp[5],LOST_LEG);
	   SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
  	   make_part(victim,"leg");
	   if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	      {
	         if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
		      take_item(victim,obj);
	      }
	   if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	      take_item(victim,obj);
      }

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_ARM_R(victim,LOST_ARM) )
      {
         SET_BIT(victim->loc_hp[3],LOST_ARM);
         SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	   make_part(victim,"arm");
         if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
            {
               if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
               take_item(victim,obj);
	      }
         if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
            take_item(victim,obj);
         if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
            take_item(victim,obj);
      }

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_LEG_L(victim,LOST_LEG) )
      {
         SET_BIT(victim->loc_hp[4],LOST_LEG);
	   SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
  	   make_part(victim,"leg");
	   if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	      {
	         if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
		      take_item(victim,obj);
	      }
	   if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	      take_item(victim,obj);
      }

   act("Your weapon bEcoMEs a bLUr as you spin around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_CHAR);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through your body!!",ch,NULL,victim,TO_VICT);
   act("$n's weapon bEcoMEs a bLUr as $e spins around rapidly sAvAGely cutting through $N's body!!",ch,NULL,victim,TO_NOTVICT);

   if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4) )
      SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);

   if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2) )
      SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);

   if ( !SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1) )
      SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);

   act("$n flip backwards to his feet as he twirls his weapon and points it at you.",ch,NULL,NULL,TO_ROOM);
   act("You flip backwards to your feet as you twirl your weapon and point it at your enemy.",ch,NULL,NULL,TO_CHAR);

   if ( IS_NPC( ch ) )
      ch->hit = UMIN( ch->hit + 2000000, ch->max_hit ); 
      else hit_gain( ch, 1000000 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_focus = DIRECT;
   ch->attack_var = SLASH;
   hurt_person( ch, victim, 3500000, TRUE );
}

void do_durandal( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_PALADIN) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 30 || ch->tier < 5 )
      {
         stc("#yYou #wneed more #Pfaith #win the #yLight #wwarrior.#n\n\r",ch);
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;

   act( "You tighten the grip on your horse's rein as the room is suddenly fill with light.", ch, NULL, NULL, TO_CHAR );
   act( "$n tightens the grip on $s horse's rein as the room is suddenly fill with light.", ch, NULL, NULL, TO_ROOM );
   act( "Finding yourself admist the heavens, you hear soft gentle music in the background.", ch, NULL, NULL, TO_CHAR );
   act( "Finding yourself admist the heavens, you hear soft gentle music in the background.", ch, NULL, NULL, TO_ROOM );
   act( "A beam of light suddenly shines down beside you, revealing a MASSIVE sword.", ch, NULL, NULL, TO_CHAR );
   act( "A beam of light suddenly shines down beside $n, revealing a MASSIVE sword.", ch, NULL, NULL, TO_ROOM );
   act( "Your horse whines as you take a grip of the sword called Durandal...", ch, NULL, NULL, TO_CHAR );
   act( "$n's horse whines as $e takes a grip of the sword called Durandal...", ch, NULL, NULL, TO_ROOM );
   act( "You then raise it high into the air as it is struck by a lightning bolt!", ch, NULL, NULL, TO_CHAR );
   act( "$n then raises it high into the air as it is struck by a lightning bolt!", ch, NULL, NULL, TO_ROOM );
   act( "Your body fizzes with electricity as deadly flames erupt from the sword.", ch, NULL, NULL, TO_CHAR );
   act( "$n's body fizzes with electricity as deadly flames erupt from the sword.", ch, NULL, NULL, TO_ROOM );
   act( "'#yFor our loved ones!#n' you exclaim as you charge across the vicinity!!!", ch, NULL, NULL, TO_CHAR );
   act( "'#yFor our loved ones!#n' $n exclaims as $e charges across the vicinity!!!", ch, NULL, NULL, TO_ROOM );

   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) )
         continue;

      act( "A TREMEDOUS explosion fills the air as you ClEaVe through $N's body!", ch, NULL, vch, TO_CHAR );
      act( "A TREMEDOUS explosion fills the air as $n ClEaVeS through your body!", ch, NULL, vch, TO_VICT );
      act( "A TREMEDOUS explosion fills the air as $n ClEaVeS through $N's body!", ch, NULL, vch, TO_NOTVICT );

      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = SLASH;
      ch->attack_focus = ( number_range( 1, 2 ) == 1 ) ? UPPERCUT : OVERHEAD;

      if ( vch->alignment <= -350 )
         {
            AFFECT_DATA *paf, *paf_next;

            for ( paf = vch->affected; paf != NULL; paf = paf_next )
            {
               bool test;

               test = FALSE;
               paf_next = paf->next;

               if ( paf->type == skill_lookup("despondency")
                    || paf->type == skill_lookup("web")
                    || paf->type == skill_lookup("faerie fire")
                    || paf->type == skill_lookup("blindness")
                    || paf->type == skill_lookup("poison")
                    || paf->type == skill_lookup("royal flush")
                    || paf->type == skill_lookup("age")
                    || paf->type == skill_lookup("slow")
                    || paf->type == skill_lookup("curse of allah")
                    || paf->type == skill_lookup("chill touch")
                    || paf->type == skill_lookup("curse")
                    || paf->type == skill_lookup("gods disfavor")
                    || paf->type == skill_lookup("drowfire")
                    || paf->type == skill_lookup("weaken")
                    || paf->type == skill_lookup("last prayer")
                    || paf->type == skill_lookup("fear")
                    || paf->type == skill_lookup("infirmity") )
                  test = TRUE;

               if ( test == FALSE && paf->duration != -1 )
                  {
                     if ( ( paf->type == skill_lookup("overdrive") && is_affected( vch, skill_lookup("subsitance") ) )
                          || ( paf->type == skill_lookup("subsitance") && is_affected( vch, skill_lookup("overdrive") ) ) )
                        ;
                        else if ( !is_affected( ch, paf->type ) )
                        {
                           paf->type -= 10000;
                           affect_to_char( ch, paf );
                        }

                     affect_remove( vch, paf );
                  }
            }

            hurt_person( ch, vch, 3000000, TRUE );
         }
         else hurt_person( ch, vch, 2000000, TRUE );
   }
}

void do_vengeance( CHAR_DATA *ch, char *argument )
{
   int sn;
   char arg [MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_PALADIN) || IS_SET( ch->special, SPC_SWORD ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 25 )
      {
         stc("#yYou #wneed more #Pfaith #win the #yLight #wwarrior.#n\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ch->mana < 1000000 )
      {
         stc("You need at least 1 million mana to use this power.\n\r", ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Whom do you wish to protect?\n\r", ch );
         return;
      }
    
   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( ch == victim )
      {
         stc( "You are one of God's warriors. Use this on the innocents.\n\r", ch );
         return;
      }

   if ( victim->alignment <= -350 )
      {
         stc( "They are beyond the Light's protective grace.\n\r", ch );
         return;
      }

   if ( ( sn = skill_lookup( "vengeance" ) ) < 0 ) return;

   if ( is_affected(victim,skill_lookup("vengeance") ) )
      {
         stc("They are already protected by the Light!\n\r",ch);
         return;
      }

   (*skill_table[sn].spell_fun) ( sn, 10, ch, victim );
   WAIT_STATE( ch, 8 );
   ch->mana -= 1000000;
}

void do_holy_shield( CHAR_DATA *ch, char *argument )
{
   int sn;
   char arg [MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_PALADIN) || IS_SET( ch->special, SPC_SWORD ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("#yYou #wneed more #Pfaith #win the #yLight #wwarrior.#n\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ch->mana < 4000000 )
      {
         stc("You need at least 4 million mana to use this power.\n\r", ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Whom do you wish to protect?\n\r", ch );
         return;
      }
    
   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( ( sn = skill_lookup( "holy shield" ) ) < 0 ) return;

   if ( is_affected(victim,skill_lookup("holy shield") ) )
      {
         stc("They are already protected by the Light!\n\r",ch);
         return;
      }

   (*skill_table[sn].spell_fun) ( sn, 10, ch, victim );
   WAIT_STATE( ch, 8 );
   ch->mana -= 4000000;
}
