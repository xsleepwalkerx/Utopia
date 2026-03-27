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

/*
 * Local functions.
 */
void do_grant( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_DROW) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Grant <person> <power>\n\r", ch );
	send_to_char("#pDrowfire #0(#r2500#0)#n,  #pDarkness #0(#r7500#0)#n,  #pDrowpoison #0(#r2500#0)#n.\n\r",ch);
      send_to_char("#pDrowsight #0(#r5000#0)#n,  #pDrowshield #0(#r5000#0)#n,  #pLevitation #0(#r1000#0)#n.\n\r", ch );
	send_to_char("#pShadowwalk #0(#r10000#0)#n,  #pFightdance#0 (#r100000#0)#n,  #pSpiderarms #0(#r7500#0)#n.\n\r", ch );
	send_to_char("#pDarktendrils #0(#r10000#0)#n,  #pElamshin #0(#r50000#0)#n, #pWeb #0(#r5000#0)#n.\n\r",ch );
	send_to_char("#pDrewst #0(#r20000#0),  #pLuth #0(#r10000#0)#n.\n\r", ch );
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

    if (!str_cmp(arg2,"drowfire")) 
	{inpart = DPOWER_DROWFIRE; cost = 2500;}
    else if (!str_cmp(arg2,"darkness"))
	{inpart = DPOWER_DARKNESS; cost= 7500;}
    else if (!str_cmp(arg2,"drowsight"))
	{inpart = DPOWER_DROWSIGHT; cost= 5000;}
    else if (!str_cmp(arg2,"spiderarms"))
	{inpart = DPOWER_ARMS; cost = 7500;}
    else if (!str_cmp(arg2,"web"))
	{inpart = DPOWER_WEB; cost = 5000;}
    else if (!str_cmp(arg2,"elamshin"))
	{inpart = DPOWER_ELAMSHIN; cost = 50000;}
    else if (!str_cmp(arg2,"luth"))
	{inpart = DPOWER_LUTH; cost = 10000;}
    else if (!str_cmp(arg2,"drowshield"))
     	{inpart = DPOWER_DROWSHIELD; cost = 5000;}
    else if (!str_cmp(arg2,"levitation"))
	{inpart = DPOWER_LEVITATION; cost = 1000;}
    else if (!str_cmp(arg2,"shadowwalk"))
	{inpart = DPOWER_SHADOWWALK; cost = 10000;}
    else if (!str_cmp(arg2,"drowpoison"))
	{inpart = DPOWER_DROWPOISON; cost = 2500;}
    else if (!str_cmp(arg2,"confuse"))
	{inpart = DPOWER_CONFUSE; cost = 2500;}
    else if (!str_cmp(arg2,"darktendrils"))
	{inpart = DPOWER_DARKTENDRILS; cost =10000;}
    else if (!str_cmp(arg2,"drewst"))
	{inpart = DPOWER_DREWST; cost =20000;} 
	else if (!str_cmp(arg2,"fightdance"))
	{inpart = DPOWER_FIGHTDANCE; cost =10000;}
	else {
              do_grant( ch, "" );
              return;
           }

    if (IS_SET(victim->pcdata->powers[1], inpart))
    {
	send_to_char("They have already got that power.\n\r",ch);
	return;
    }

    if( !IS_CLASS(victim, CLASS_DROW) )
    {
	stc( "You can not grant powers to Non-Drows.\n\r",ch);
	return;
    }

    cost = 0;

    if (ch->pcdata->stats[DROW_POWER] < cost) 
    {
	send_to_char("You have insufficient power to grant that gift.\n\r",ch);
	return;
    }

    SET_BIT(victim->pcdata->powers[1], inpart);
    ch->pcdata->stats[DROW_POWER]   -= cost;
    if (victim != ch) send_to_char("You have been granted a gift from your matron!\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    if (victim != ch) save_char_obj(ch);
    save_char_obj(victim);
}

void do_chaosblast(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) ) 
      {
         send_to_char("Huh?\n\r", ch);
	   return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to smite chaos upon?\n\r",ch);
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

   WAIT_STATE( ch, 12 );
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You grin evilly as you tilt your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n grins evilly as $e tilts $s hands together.",ch,NULL,NULL,TO_ROOM);
   act("You then release a blast of chaos at $N saying '#yFear Lloth's wrath!#n'",ch,NULL,victim,TO_CHAR);
   act("$n then releases a blast of chaos at you saying '#yFear Lloth's wrath!#n'",ch,NULL,victim,TO_VICT);
   act("$n then releases a blast of chaos at $N saying '#yFear Lloth's wrath!#n'",ch,NULL,victim,TO_NOTVICT);     
   act("#0Your eyes glow #Rred#0 as #RLloth's hatred #0wreaks havok upon $N.#n",ch,NULL,victim,TO_CHAR);
   act("#0$n eyes glow #Rred#0 as #RLloth's wrath #0engulfs on you.#n",ch,NULL,victim,TO_VICT);
   act("#0$n eyes glow #Rred#0 as #RLloth's wrath #0engulfs upon $N.#n",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_CLASS( victim, CLASS_DROW ) )
      SET_BIT( victim->more, MORE_CONFUSION );

   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_drowcreate( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
    int cost = 6;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_DARK_ELF) )
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if (arg[0] == '\0')
    {
	send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
	send_to_char("Whip, Dagger, Ring, Amulet, Armor, Helmet,\n\r", ch);
	send_to_char("Leggings, Boots, Gauntlets, Sleeves,\n\r", ch );
	send_to_char("Belt, Bracer, Mask, Cloak.\n\r", ch);
      send_to_char("Arrow, Crossbow.\n\r", ch);
	return;
    }

    if (!str_cmp(arg,"dagger")) vnum = 326;
    else if (!str_cmp(arg,"whip")) vnum = 327;
    else if (!str_cmp(arg,"belt"))   vnum = 328;
    else if (!str_cmp(arg, "ring"))	vnum = 329;
    else if (!str_cmp(arg, "amulet"))	vnum = 330;
    else if (!str_cmp(arg,"helmet"))	vnum = 331;
    else if (!str_cmp(arg,"leggings"))	vnum = 332;
    else if (!str_cmp(arg,"boots")) vnum = 333;
    else if (!str_cmp(arg,"gauntlets")) vnum = 334;
    else if (!str_cmp(arg,"sleeves")) vnum = 335;
    else if (!str_cmp(arg,"cloak")) vnum = 336;
    else if (!str_cmp(arg,"bracer")) vnum = 337;
    else if (!str_cmp(arg,"mask")) vnum = 338;
    else if (!str_cmp(arg,"armor")) vnum = 339;
    else if (!str_cmp(arg,"crossbow")) vnum = 398;
    else if (!str_cmp(arg,"arrow")) vnum = 399;
    else
    {
	send_to_char("That is an invalid type.\n\r", ch );
	return;
    }

    if ( ch->gold < cost)
    {
       send_to_char("It costs #w1 #ygold piece#n to create droww equipment.\n\r",ch);
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
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_llothbless( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_DROW) ) 
     {
  	  send_to_char("Huh?\n\r", ch );
	  return;
     }

  if ( ch->mana < 50000 )
  {
    stc("You don't have enough magical energy for that.\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "llothbless" ) ) < 0 ) return;

  if ( is_affected(victim,skill_lookup("llothbless")) )
  {
    stc("The person already has LLoth's Blessing!\n\r",ch);
    return;
  }

  level = 30;
  ch->mana -= 50000;
  (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
  WAIT_STATE( ch, 12 );
  return;
}

void do_shadow_flare( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) && ch->pIndexData->vnum != 551 ) 
      return;

   if ( !IS_NPC(ch) ) 
      {
         if ( !IS_CLASS( ch, CLASS_DROW ) ) 
            {
               send_to_char("Huh?\n\r", ch);
               return;
            }

         if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
            {
               stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
               return;
            }

         if ( get_stock( ch ) < 300 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to burn with demonic energy?\n\r",ch);
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

   if ( !IS_NPC(ch) ) 
      {
         WAIT_STATE( ch, 12 );
         subtract_stock( ch, 300 );
      }

   SET_BIT( ch->arena, AFF2_DM );

   act("You say '#yEndless energy drains toward the Void... ShadowFlare!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yEndless energy drains toward the Void... ShadowFlare!#n'",ch,NULL,NULL,TO_ROOM);
   act("$n grins evilly as $e waves $s arm in a circular motion and points it at you.",ch,NULL,victim,TO_VICT);
   act("$n grins evilly as $e waves $s arm in a circular motion and points it at $N.",ch,NULL,victim,TO_NOTVICT);

   act("\n\rBalls of pitch-black energy is drawn inside of your body.",ch,NULL,victim,TO_VICT);
   act("\n\rBalls of pitch-black energy is drawn inside of $n's body.",victim,NULL,NULL,TO_ROOM);
   act("You suddenly turn into a shadowly outline as a demonic cackle fills the room.",ch,NULL,victim,TO_VICT);
   act("$n suddenly turns into a shadowly outline as a demonic cackle fills the room.",victim,NULL,NULL,TO_ROOM);
   act("The Demonic Cackles deafens your ears as the balls of Pitch-Black energy rips apart your body.",ch,NULL,victim,TO_VICT);
   act("The Demonic Cackles deafens your ears as the balls of Pitch-Black energy rips apart $n's body.",victim,NULL,NULL,TO_ROOM);
   act("The room is filled with blinding energy as the pitch-black substance ROARS from within you!!!",ch,NULL,victim,TO_VICT);
   act("The room is filled with blinding energy as the pitch-black substance ROARS from within $n!!!",victim,NULL,NULL,TO_ROOM);

   if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
      SET_BIT( victim->affected_by, AFF_FLAMING );

   ch->attack_var = BEAM;
   ch->attack_focus = DIRECT;
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;

   if ( IS_NPC( ch ) )
      hurt_person( ch, victim, 2500000, TRUE );
      else hurt_person( ch, victim, 3500000, TRUE );
}

void do_earth_blast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  char arg[MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DROW) ) 
     {
        send_to_char("Huh?\n\r", ch );
        return;
     }

  if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
     {
        stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
        return;
     }

  if ( ch->pkill < 8 )
     {
        stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
        return;
     }

  argument = one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use Earth Blast on?\n\r",ch);
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

  if ( !IS_NPC(ch) )
     {
        if ( ch->move < 250000 )
           {
              stc("You are too tired to execute this technique.\n\r",ch);
              return;
           }

        ch->move -= 250000;
     }

  if ( number_percent() > 85 && !IS_SET( victim->mflags, MAGE_EMBRACED1 ) )
     SET_BIT( victim->mflags, MAGE_EMBRACED1 );

  act("You cup your hands in front of you in a circular motion.",ch,NULL,NULL,TO_CHAR);
  act("$n cups $s hands in front of $m in a circular motion.",ch,NULL,NULL,TO_ROOM);
  act("The Earth starts trembling as you suddenly raise your hands into the air!",ch,NULL,NULL,TO_CHAR);
  act("The Earth starts trembling as $n suddenly raises $s hands into the air!",ch,NULL,NULL,TO_ROOM);
  act("You yell '#yVia LLos#n' as a MASsIvE rock JOLTS from the ground SMASHING into $N's face!!",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yVia LLos#n' as a MASsIvE rock JOLTS from the ground SMASHING into your face!!",ch,NULL,victim,TO_VICT);
  act("$n yells '#yVia LLos#n' as a MASsIvE rock JOLTS from the ground SMASHING into $N's face!!",ch,NULL,victim,TO_NOTVICT);
  act("You crash back onto the ground stunned!",victim,NULL,NULL,TO_CHAR);
  act("$n crashes back onto the ground stunned!",victim,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  ch->attack_var = BLUNT;
  ch->attack_focus = UPPERCUT;
  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_EARTH;
  hurt_person( ch, victim, 700000, TRUE );
}

void do_dark_thrust( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 551 ) 
     return;

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to use Dark Thrust on?\n\r",ch);
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

  if ( is_safe(ch, victim) )
     return;

  act("You charge towards $N knocking $M down with a shoulderush.",ch,NULL,victim,TO_CHAR);
  act("$n charges towards you knocking you down with a shoulderush.",ch,NULL,victim,TO_VICT);
  act("$n charges towards $N knocking $M down with a shoulderush.",ch,NULL,victim,TO_NOTVICT);
  act("As $N tries to get up, you lash your dagger upwards $S body causing $M to scream in pain.",ch,NULL,victim,TO_CHAR);
  act("As you try to get up, $n lashes $s dagger upwards your body causing you to scream in pain.",ch,NULL,victim,TO_VICT);
  act("As $N tries to get up, $n lashes $s dagger upwards $S body causing $M to scream in pain.",ch,NULL,victim,TO_NOTVICT);
  act("With blood skewing from $N wounds, you savagely thrust your dagger through $S chest.",ch,NULL,victim,TO_CHAR);
  act("With blood skewing from your wounds, $n savagely thrust $s dagger through your chest.",ch,NULL,victim,TO_VICT);
  act("With blood skewing from $N wounds, $n savagely thrust $s dagger through $S chest.",ch,NULL,victim,TO_NOTVICT);
  act("You RiP your dagger across $N's devastated body as you push $M away from you.",ch,NULL,victim,TO_CHAR);
  act("$n RiPs $s dagger across your devastated body as $e pushes you away from $m.",ch,NULL,victim,TO_VICT);
  act("$n RiPs $s dagger across $N's devastated body as $e pushes $M away from $m.",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 12;

  ch->attack_type = ATTACK_PHYSICAL;
  ch->ele_attack = ATTACK_DARK;
  ch->attack_focus = DIRECT;
  ch->attack_var = SLASH;

  hurt_person( ch, victim, 1000000, TRUE );
}

void do_bloodlet( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) ) 
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 12 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who do you wish to use BloodLet on?\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
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

   if ( IS_NPC(victim) )
      {
         stc("That doesn't seem like a good idea.\n\r",ch);
         return;
      }

   if ( IS_SET(victim->more, MORE_BLOOD) )
      {
         stc("You already punctured mortal wounds into them.\n\r",ch);
         return;
      }

   ch->mana -= 1000000;

   act("You quickly pull out a stiletto slicing upwards through $N's throat.",ch,NULL,victim,TO_CHAR);
   act("$n quickly pulls out a stiletto slicing upwards through your throat.",ch,NULL,victim,TO_VICT);
   act("$n quickly pulls out a stiletto slicing upwards through $N's throat.",ch,NULL,victim,TO_NOTVICT);
   act("As globs of blood spray over you, you thrust the weapon into $N's stomach!",ch,NULL,victim,TO_CHAR);
   act("As globs of your blood spray over $n, $e thrusts the weapon into your stomach!",ch,NULL,victim,TO_VICT);
   act("As globs of blood spray over $n, $e thrusts the weapon into $N's stomach!",ch,NULL,victim,TO_NOTVICT);
   act("As $N kneels to the ground choking on $S own blood, you DrIvE the weapon through $S feet!",ch,NULL,victim,TO_CHAR);
   act("As you kneel to the ground choking on your own blood, $n DrIvEs the weapon through your feet!",ch,NULL,victim,TO_VICT);
   act("As $N kneels to the ground choking on $S own blood, $n DrIvEs the weapon through $S feet!",ch,NULL,victim,TO_NOTVICT);
   SET_BIT(victim->more, MORE_BLOOD);

   if ( ch->fighting == NULL )
      ch->wait = 12;
}

void do_dischard( CHAR_DATA *ch, char *argument )
{
     CHAR_DATA *victim;
     char arg[MAX_INPUT_LENGTH];

     argument = one_argument( argument, arg );
 
     if ( IS_NPC(ch) )
        return;

     if ( !IS_CLASS(ch, CLASS_DROW) && !IS_SET( ch->cmbt[7], SKL_DISCHARD ) ) 
        {
          send_to_char("Huh?\n\r", ch );
          return;
        }

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
        {
           stc("Whom do you wish to dischard their protection from the deities?\n\r",ch);
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
    
     if ( ( victim == ch ) )
        {
           stc("That doesn't seem like a good idea.\n\r",ch);
           return;
        }

     if ( !IS_SET(victim->more, MORE_SAFE) )
        {
           stc("They arenot protected by the deities.\n\r",ch);
           return;
        }

    act("You kneel down asking Lloth to remove the protection granted to $N.",ch,NULL,victim,TO_CHAR);
    REMOVE_BIT(victim->more, MORE_SAFE);
}

void do_drewst( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

   if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_DREWST) )
      {
	    send_to_char("Huh?\n\r", ch );
	    return;
      }

    argument = one_argument( argument, arg );

    if ( ( obj = get_obj_room( ch, arg ) ) == NULL )
    {
	 send_to_char( "Nothing like that in this room.\n\r", ch );
	 return;
    }

    if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy to do this.\n\r",ch);
         return;
      }

    if ( obj->chobj == NULL )
       {
	    send_to_char("You can't draw a being from this inanimate object.\n\r",ch);
	    return;
       }

    act("You mumble ancient curses onto $p.",ch,obj,NULL,TO_CHAR);
    act("$n mumbles ancient curses onto $p.",ch,obj,NULL,TO_ROOM);

    victim = obj->chobj;
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
    ch->mana -= 1000000;
    victim->wait = 48;
}

void do_lloth_throw( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int count;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 551 ) 
     return;

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to throw your dagger to?\n\r",ch);
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

  if ( is_safe(ch, victim) )
     return;

  act("You hold your dagger by the tip as you raise it high above you.",ch,NULL,NULL,TO_CHAR);
  act("$n holds $s dagger by the tip as $e raises it high above you.",ch,NULL,NULL,TO_ROOM);
  act("You mutter a soft prayer to Lloth as the dagger goes pitch-black.",ch,NULL,NULL,TO_CHAR);
  act("$n mutters a soft prayer to Lloth as the dagger goes pitch-black.",ch,NULL,NULL,TO_ROOM);

  for ( count = 0; count < 5; count++ )
  {
     act("$N screams loudly as you accurately pick $M out with your dagger.",ch,NULL,victim,TO_CHAR);
     act("You scream loudly as $n accurately picks you out with $s dagger.",ch,NULL,victim,TO_VICT);
     act("$N screams loudly as $n accurately picks $M out with $s dagger.",ch,NULL,victim,TO_NOTVICT);
  }

  ch->wait = 12;

  ch->attack_type = ATTACK_PHYSICAL;
  ch->ele_attack = ATTACK_DARK;
  ch->attack_focus = DIRECT;
  ch->attack_var = PIERCE;
  hurt_person( ch, victim, 1333333, TRUE );
}

void do_heal ( CHAR_DATA *ch, char *argument ) 
{
   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_DROW) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   hit_gain( ch, 300000 );
   mana_gain( ch, 300000 );
   move_gain( ch, 300000 );

   send_to_char("You silently tend your wounds.\n\r",ch);
   act("$n silently tends $s wounds.",ch,NULL,NULL,TO_ROOM);
   WAIT_STATE(ch, 12);
}

void do_drowhate(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DROWHATE))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_DROWHATE)) {
	send_to_char("You calm your hatred.\n\r", ch );
	REMOVE_BIT(ch->newbits, NEW_DROWHATE);
	}
	else {
	send_to_char("You invoke your hatred for others.\n\r", ch );
	SET_BIT(ch->newbits, NEW_DROWHATE);
	}
}

void do_darktendrils(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DARKTENDRILS))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_DARKTENDRILS)) {
	send_to_char("Your tendrils of darkness disappear.\n\r", ch );
	REMOVE_BIT(ch->newbits, NEW_DARKTENDRILS);
	}
	else {
	send_to_char("You call forth dark tendrils to attack your foes.\n\r", ch );
	SET_BIT(ch->newbits, NEW_DARKTENDRILS);
	}
}

void do_darkdodge(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DROW) ) 
         {
		send_to_char("Huh?\n\r", ch );
		return;
         }

	if ( IS_SET(ch->newbits, NEW_DARKDODGE) )
         {
	       send_to_char("Your Dark Nature will no longer protect you.\n\r", ch );
   	       REMOVE_BIT(ch->newbits, NEW_DARKDODGE);
	   }
	   else {
	           send_to_char("Your Dark Nature will protect you.\n\r", ch );
	           SET_BIT(ch->newbits, NEW_DARKDODGE);
	        }
}

void do_spiderform( CHAR_DATA *ch, char *argument ) 
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
 
	argument = one_argument(argument,arg);

	if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_SPIDERFORM) )
	   {
            send_to_char("Huh?\n\r", ch );
	      return;
         }

      if ( IS_SET(ch->newbits,NEW_DFORM) )
         {
	      sprintf(buf, "$n morphs back into %s.", GET_PROPER_NAME(ch));
            act(buf, ch, NULL, NULL, TO_ROOM);
            stc("You return to your normal form.\n\r", ch);

            free_string(ch->morph);
            ch->morph = str_dup("");
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

            REMOVE_BIT(ch->newbits,NEW_DFORM);
	      REMOVE_BIT(ch->newbits,THIRD_HAND);
	      REMOVE_BIT(ch->newbits,FOURTH_HAND);
            WAIT_STATE(ch, 7);
            return;   
         }
         else if ( !IS_SET(ch->newbits,NEW_DFORM) )
                 {
	      	  act("You mutate into a giant spider.",ch,NULL,NULL,TO_CHAR);
                    act("$n mutates into a giant spider.",ch,NULL,NULL,TO_ROOM);
                    sprintf(buf,"#r%s the #eS#wo#eu#wl #oSpider#n",ch->name);

                    free_string(ch->morph);
                    ch->morph = str_dup(buf);
                    SET_BIT(ch->newbits,NEW_DFORM);
	              SET_BIT(ch->newbits,THIRD_HAND);
	              SET_BIT(ch->newbits,FOURTH_HAND);
                    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	              return;
                }
}


void do_drowsight(CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;
	
   if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSIGHT) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
      {   
         REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
         send_to_char( "Your senses return to normal.\n\r", ch );
      }
      else {
              SET_BIT(ch->act, PLR_HOLYLIGHT);
              send_to_char( "Your senses increase to incredible proportions.\n\r", ch);
           }
}

void do_drowshield(CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DROWSHIELD) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( !IS_IMMUNE(ch,IMM_SHIELDED) )
      {
         send_to_char("You shield your aura from those around you.\n\r",ch);
         SET_BIT(ch->immune, IMM_SHIELDED);
         return;
      }

   send_to_char("You stop shielding your aura.\n\r",ch);
   REMOVE_BIT(ch->immune, IMM_SHIELDED);
}

void do_drowpowers(CHAR_DATA *ch,char *argument)
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   send_to_char("#6The powers Lloth has bestowed upon you...#n\n\r", ch );
   send_to_char("#4-----------------------------------------#n\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_SPEED) )
      send_to_char("You move quite quickly.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_TOUGHSKIN) )
      send_to_char("Your skin has been toughened.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DROWFIRE) )
      send_to_char("You can call Drowfire upon your enemies.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DARKNESS) )
      send_to_char("You can summon a globe of darkness.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DROWSIGHT) )
      send_to_char("You can use enhanced sight.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION) )
      send_to_char("You can levitate in the air.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DROWSHIELD) )
      send_to_char("You can shield your aura.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
      send_to_char("You have control over deadly poisons.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWWALK) )
      send_to_char("You can walk through the shadows.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE) )
      send_to_char("You have mastered the art of garotte.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE) )
      send_to_char("You have dark-enhanced garotte.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_ARMS) )
      send_to_char("You have extra spidery arms.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DROWHATE) )
      send_to_char("You can invoke the hatred of the Drow.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_SPIDERFORM) )
      send_to_char("You can mutate into a large spider.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_WEB) )
      send_to_char("You can shoot a spidery web at opponents.\n\r", ch);

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_GLAMOUR) )
      send_to_char("You can glamourosly change the appearance of items.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_CONFUSE) )
      send_to_char("You can confuse your enemies during battle.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_EARTHSHATTER) )
      send_to_char("You can shatter the earth under your feet.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_DARKTENDRILS) )
      send_to_char("You call forth Tendrils of Darkness to aid battle.\n\r", ch );

   if ( IS_SET(ch->newbits, NEW_FIGHTDANCE) ) 
      send_to_char("You are dancing in the ancient style of Lloth.\n\r", ch );

   if ( IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWSTRENGTH) )
      send_to_char("The shadows give you strength .\n\r", ch );

   send_to_char("\n\r", ch);

   sprintf( buf, "You have %d drow power points!.\n\r",
   ch->pcdata->stats[DROW_POWER] );
   send_to_char( buf, ch );

   sprintf( buf, "You have %d points of magic resistance.\n\r", 
   ch->pcdata->stats[DROW_MAGIC] );
   send_to_char( buf, ch );

   if ( weather_info.sunlight == SUN_DARK )
      send_to_char("You feel strong in the night.\n\r", ch );

   send_to_char("\n\r", ch );
}

void do_fightdance(CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_FIGHTDANCE) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( IS_SET(ch->newbits, NEW_FIGHTDANCE) )
      {
         send_to_char("You will no longer dance The Dance of Lloth.\n\r", ch );
         REMOVE_BIT(ch->newbits, NEW_FIGHTDANCE);
      }
      else {
              send_to_char("You will start dancing The Dance of Lloth while fighting.\n\r", ch );
              SET_BIT(ch->newbits, NEW_FIGHTDANCE);
           }
}

void do_darkness(CHAR_DATA *ch, char *argument) 
{
	bool blah = FALSE;

	if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DARKNESS) )
         {
           stc("Huh?\n\r",ch);
           return;
         }

      if ( ch->fight_timer > 0 )
      {
         stc("Not with a fight timer.\n\r",ch);
         return;
      }

	if ( IS_SET(ch->newbits, NEW_DARKNESS) ) 
         {
  	      send_to_char("You banish your cloak of darkness.\n\r", ch );
	      REMOVE_BIT(ch->newbits, NEW_DARKNESS);
	      act("The cloak of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
	      if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) && ch->in_room->drows == 1 )
	         REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
            ch->in_room->drows--;
	      return;
	}

	if (ch->mana < 100000) {
	send_to_char("You don't have enough mana to summon the darkness.\n\r",ch );
	return;}

	send_to_char("You wrap a cloak of darkness around yourself.\n\r", ch );
	act("$n wraps a cloak of darkness around $mself.",ch,NULL,NULL,TO_ROOM);
	ch->mana -= 100000;
	if (IS_SET(ch->extra, TIED_UP)) {
		REMOVE_BIT(ch->extra, TIED_UP);
		blah = TRUE;}

	if (blah) send_to_char("The darkness sets you free.\n\r", ch );		

	SET_BIT(ch->newbits, NEW_DARKNESS);
	if (!IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
	SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
      ch->in_room->drows++;
}

void do_confuse(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_CONFUSE)) {
	send_to_char("Huh?\n\r",  ch);
	return;}

	if ((victim = ch->fighting) == NULL) {
	send_to_char("You are not fighting anyone.\n\r",  ch);
	return;}

	if (ch->move < 300000) {
	send_to_char("You are too tired to execute this technique.\n\r",ch);
	return;}

	act("$n attempts to confuse you.",ch,NULL,victim,TO_VICT);
	act("You attempt to confuse $N.",ch,NULL,victim,TO_CHAR);
	act("$n attempts to confuse $N.",ch,NULL,victim,TO_NOTVICT);

      ch->wait = 12;
	ch->move -= 300000;

	if ( number_percent() < 85 )
         {
	      send_to_char("You failed.\n\r", ch );
    	      return;
         }
	   else {
		     do_flee(victim,"");	
	           return;	
              }
}

void do_earthshatter( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_EARTHSHATTER)) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (ch->mana < 600000) {
	send_to_char("You need more mana.\n\r", ch );
	return;}

	ch->mana -= 600000;

	send_to_char("You summon the power of the underworld, shattering the earth.\n\r", ch );
	act("$n causes the earth to shatter violently beneath your feet hurting you badly.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

  	  if (vch == ch) continue;
 
        if ( is_safe(ch,vch) || is_same_group(ch,vch) )
           continue;

        dam  = char_damcap(ch);

        ch->ele_attack = ATTACK_EARTH;
        hurt_person( ch, vch, 600000, TRUE );
    }

    WAIT_STATE(ch, 12);
}

void do_shadowstrength(CHAR_DATA *ch, char *argument)
{
  if ( !IS_NPC(ch) )
     {            
	   if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWSTRENGTH) )
	      { 
               send_to_char("Huh?\n\r", ch );
	         return;
            }

         if ( !IS_SET(ch->newbits, NEW_DARKNESS) && !SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
            {
	         send_to_char("Darkness must be activated to use this power.\n\r", ch );
 	         return;
            }

	   if ( IS_SET(ch->newbits, NEW_SHADSTR) )
	      {
	         send_to_char("The power of the shadows leaves you.\n\r", ch );
	         REMOVE_BIT(ch->newbits, NEW_SHADSTR);
	      }
	      else { 
	              send_to_char("The power of the shadows joins you.\n\r", ch );
	              SET_BIT(ch->newbits, NEW_SHADSTR);
	           }
     }
}

void do_hex( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_DROW) )
       {
	     send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
       {
           stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
           return;
       }

    if ( ch->mana < 500000 )
       {
          send_to_char("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to hex?\n\r",ch);
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

    if ( ch == victim )
       {
	    send_to_char( "Dont be foolish.\n\r", ch );
	    return;
       }

    if ( is_safe(ch, victim) ) return;
   
    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       {
            stc("They are protected by the holy saints.\n\r",ch);
            return;
       }

    if ( IS_SET(victim->more, MORE_HEX) )
       {
	    send_to_char( "Their speech is already impaired.\n\r", ch );
	    return;
       }

    ch->wait = 6;

    act("You circle your hands around $N's body as you murmur an ancient language.",ch,NULL,victim,TO_CHAR);
    act("$n circle $s hands around your body as $e murmurs some weird phrases.",ch,NULL,victim,TO_VICT);
    act("Your speech is impaired!",victim,NULL,NULL,TO_CHAR);

    SET_BIT(victim->more, MORE_HEX);
    ch->mana -= 500000;
}

void do_drowfire( CHAR_DATA *ch, char *argument ) 
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_DROWFIRE) ) 
       {
	    send_to_char("Huh?\n\r", ch );
	    return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to use Drowfire on?\n\r",ch);
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

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       {
            stc("They are protected by the holy saints.\n\r",ch);
            return;
       }

    if ( ch->mana < 500000 ) 
       {
	    send_to_char("You don't have enough mana.\n\r", ch );
	    return;
       }

    if ( ( sn = skill_lookup( "drowfire" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 1.5;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    ch->mana = ch->mana - 500000;
}

void do_shoot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    OBJ_DATA        *obj;
    char            arg  [MAX_INPUT_LENGTH];
    int             bonus = 100000;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DROW) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
       {
          stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
          return;
       }

    if ( ch->pkill < 8 )
       {
          stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
          return;
       }

    if ( ch->cmbt[3] < 1 )
       {
          stc("You are still preparing yourself to fire another arrow.\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
       if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "You are not holding a crossbow.\n\r", ch );
             return;
          }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
    {
       stc("Who do you wish to shoot?\n\r",ch);
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

    ch->cmbt[3]--;
   
    if ( IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
       {
           act("You yell '#yOloth plynn dos!!#n' as you pull the trigger firing an arrow at $N.",ch,NULL,victim,TO_CHAR);
           act("$n yells '#yOloth plynn dos!!#n' as $e pulls the trigger firing an arrow at you.",ch,NULL,victim,TO_VICT);
           act("$n yells '#yOloth plynn dos!!#n' as $e pulls the trigger firing an arrow at $N.",ch,NULL,victim,TO_NOTVICT);
           act("You scream loudly as the arrow pierces your mortal flesh.",victim,NULL,NULL,TO_CHAR);
           act("$n screams loudly as the arrow pierces $s mortal flesh.",victim,NULL,NULL,TO_ROOM);
           bonus = 100000;
       }
       else {
                act("You yell '#ySsussun pholor dor!!#n' as you pull the trigger firing an arrow at $N.",ch,NULL,victim,TO_CHAR);
                act("$n yells '#ySsussun pholor dor!!#n' as $e pulls the trigger firing an arrow at you.",ch,NULL,victim,TO_VICT);
                act("$n yells '#ySsussun pholor dor!!#n' as $e pulls the trigger firing an arrow at $N.",ch,NULL,victim,TO_NOTVICT);

                ch->ele_attack = ATTACK_FIRE;
                act("The arrow explodes brightly as it pierces your mortal flesh.",victim,NULL,NULL,TO_CHAR);
                act("The arrow explodes brightly as it pierces $n's mortal flesh.",victim,NULL,NULL,TO_ROOM);

                if ( number_percent() > 49 && !IS_SET( victim->more, MORE_BLIND ) )
                   {
                      SET_BIT( victim->more, MORE_BLIND );
                      victim->blind_timer = 6;
                   }
            }

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = PIERCE;
    ch->wait = 3;
    hurt_person( ch, victim, 200000 + bonus, TRUE );
}

void do_velkyn(CHAR_DATA *ch, char *argument) 
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( IS_SET(ch->newbits, NEW_DARKNESS) ) 
      {
         stc("But you are already in darkness.\n\r",ch);
         return;
      }

   if ( ch->fighting == NULL )
      {
         stc("But you aren't in a fight.\n\r",ch);
         return;
      }

   if ( IS_NPC(ch->fighting) && ch->fighting->master == NULL )
      {
         stc("You can only do this against players.\n\r",ch);
         return;
      }

   if ( ch->attacker != NULL && ch->attacker != ch->fighting )
      {
         stc("You can only do this against your attacker.\n\r",ch);
         return;
      }

   if ( ch->mana < 2000000 )
      {
         send_to_char("You don't have enough mana to summon the darkness.\n\r",ch );
         return;
      }

   send_to_char("You wrap a cloak of darkness around yourself.\n\r", ch );
   act("$n wraps a cloak of darkness around $mself.",ch,NULL,NULL,TO_ROOM);
   ch->mana -= 2000000;
 
   SET_BIT(ch->newbits, NEW_DARKNESS);
 
   if ( !IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
      SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);

   ch->in_room->drows++;
}

void do_garotte(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int chance;

   one_argument(argument, arg);
   chance = number_percent();

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_DROW) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( ch->fighting != NULL )
      {
         send_to_char("You are in a fight!\n\r", ch );
	   return;
      }

   if ( ( victim = get_char_room(ch, arg) ) == NULL )
      {
         send_to_char("Garotte whom?\n\r", ch );
         return;
      }

   if ( victim == ch )
      {
         send_to_char("That would be a bad idea.\n\r", ch );
         return;
      }

   if ( is_safe(ch, victim) )
      return;

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL
    ||   ( obj->value[3] != 4 && obj->pIndexData->vnum != 91 ) )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL
    ||   ( obj->value[3] != 4 && obj->pIndexData->vnum != 91 ) ) )
    {
        send_to_char( "You need to wield a whip.\n\r", ch );
        return;
    }
     
	check_killer(ch, victim);
	WAIT_STATE(ch, skill_table[gsn_garotte].beats);
	
	if (chance <= 5) {
	damage(ch, victim, 0, gsn_garotte);
      ch->wait = 6;
	return;}

	multi_hit(ch, victim, gsn_garotte);
	multi_hit(ch, victim, gsn_garotte);
      ch->wait = 6;
}

void do_dark_garotte(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int chance;
	int i;

	one_argument(argument, arg);
	chance = number_percent();

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE)) {
	send_to_char("Huh?\n\r", ch );
	return;}

      if ( !IS_SET(ch->newbits, NEW_DARKNESS) )
      {
	send_to_char("You have to be surrounded in darkness.\n\r", ch );
	return;}

      if ( ch->fight_timer > 0 )
      {
	   send_to_char("Not with a fight timer!\n\r", ch );
  	   return;
      }

	if ((victim = get_char_room(ch, arg)) == NULL) {
	send_to_char("Dark Garotte whom?\n\r", ch );
	return;}

	if (victim == ch) {
	send_to_char("That would be a bad idea.\n\r", ch );
	return;}

	if (is_safe(ch, victim)) return;

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL
    ||   ( obj->value[3] != 4 && obj->pIndexData->vnum != 91 ) )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL
    ||   ( obj->value[3] != 4 && obj->pIndexData->vnum != 91 ) ) )
    {
        send_to_char( "You need to wield a whip.\n\r", ch );
        return;
    }
     
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't garotte a fighting person.\n\r", ch );
        return;
    }

	check_killer(ch, victim);
	ch->wait = 12;
 
    chance = 95 - UMIN( 20, ch->pkill );

    if ( number_percent() >= chance )
       {
          act("You wrap your whip around $N's neck refusing to let go!",ch,NULL,victim,TO_CHAR);
          act("$n wraps $s whip around your neck refusing to let go!",ch,NULL,victim,TO_VICT);
          act("$n wraps $s whip around $N's neck refusing to let go!",ch,NULL,victim,TO_NOTVICT);
          hurt_person( ch, victim, 1000000, TRUE );
          return;
       }
       else if ( number_percent() <= 40 )
       {
          damage(ch, victim, 0, gsn_garotte);
          show_total_damage( ch, victim );
          return;
       }
       else {
               for (i = 1; i < 4; i++)
               {
	            multi_hit(ch, victim, gsn_garotte);
               }
 
               show_total_damage( ch, victim );
            }
}

void do_olath( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_DROW) || ch->clannum != 3 )
       {
	    send_to_char("Huh?\n\r", ch );
	    return;
       }

    if ( ch->move < 10000 )
       {
          stc("You are too tired to do this.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more, MORE_HEX) )
       {
          send_to_char("You are prevented from using this mystical ability.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use the Olath Nizzre on?\n\r",ch);
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

    if ( ( victim == ch ) || ( IS_NPC(victim) ) )
       {
	    stc("That doesn't seem like a good idea.\n\r",ch);
	    return;
       }

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       {
            stc("They are protected by the holy saints.\n\r",ch);
            return;
       }

    act("You concentrate intently as you grab $N's head and start to chant rites.",ch,NULL,victim,TO_CHAR);
    act("$n places $s hands on your head, and you scream in utter pain.",ch,NULL,victim,TO_VICT);
    act("$n places $s hands on $N's head and $N screams in pain.",ch,NULL,victim,TO_ROOM);
    SET_BIT(victim->monkstuff, MONK_DEATH);
    ch->move -= 10000;
    ch->fight_timer += 10;
    ch->wait = 12;
}

void lloth_curse( CHAR_DATA *ch )
{
   AFFECT_DATA af;

   if ( !IS_SET(ch->more, MORE_POISON) ) 
      SET_BIT(ch->more, MORE_POISON);

   if ( !IS_SET(ch->more, MORE_CONFUSION) ) 
      SET_BIT(ch->more, MORE_CONFUSION);

   if ( !IS_SET(ch->more2, MORE2_NOLEGS) )
      SET_BIT(ch->more2, MORE2_NOLEGS);

   if ( !IS_SET(ch->more2, MORE2_NOARMS) )
      SET_BIT(ch->more2, MORE2_NOARMS);

   if ( !IS_SET(ch->more, MORE_HEX) ) 
      SET_BIT(ch->more, MORE_HEX);

   if ( !IS_SET(ch->more, MORE_BLIND) ) 
      SET_BIT(ch->more, MORE_BLIND);

   if ( !IS_SET(ch->more, MORE_PINCH) ) 
      SET_BIT(ch->more, MORE_PINCH);

   if ( !IS_SET(ch->more, MORE_PAIN) ) 
      SET_BIT(ch->more, MORE_PAIN);

   if ( !IS_SET(ch->more, MORE_DISEMPOWER) )
      SET_BIT(ch->more, MORE_DISEMPOWER);

   ch->gshield = 0;

   if ( !IS_SET( ch->monkstuff, MONK_DEATH ) )
      SET_BIT( ch->monkstuff, MONK_DEATH );

   if ( !IS_SET( ch->more, MORE_BLOOD ) )
      SET_BIT( ch->more, MORE_BLOOD );

   if ( IS_SET( ch->monkstuff, MONK_HEAL ) )
      REMOVE_BIT( ch->monkstuff, MONK_HEAL );

   if ( is_affected( ch, skill_lookup("weaken") ) )
      affect_strip( ch, skill_lookup("weaken") );

   SET_BIT( ch->more, MORE_FROZEN );
   ch->freeze_timer = number_range( 6, 15 );

   if ( is_affected( ch, skill_lookup("rose storm") ) )
      affect_strip( ch, skill_lookup("rose storm") );

   ch->hit = UMAX( 1, ch->hit - 1000000 );
   af.type = skill_lookup("weaken");
   af.duration  = 5;
   af.location  = APPLY_HIT;
   af.modifier  = -1 * char_hitroll( ch ) / 2;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   if ( !is_affected( ch, skill_lookup("despondency") ) )
      {
         af.type = skill_lookup("despondency");
         af.duration  = 5;
         af.location  = APPLY_DAMROLL;
         af.modifier  = -1 * char_damroll( ch ) / 2;
         af.bitvector = 0;
         affect_to_char( ch, &af );
      }
}

void do_pandora_box( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   AFFECT_DATA af;
 
   if ( IS_NPC( ch ) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) || ch->pkill < 20 )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 )
      {
         send_to_char( "Not in the arena.\n\r", ch );
         return;
      }

   if ( ch->fight_timer == 0 || ch->fighting == NULL || IS_NPC( ch->fighting ) )
      {
         send_to_char("Lloth only acknowledge those who thrives for Chaos...\n\r", ch );
         return;
      }

   if ( IS_SET( ch->more, MORE_BOMB ) )
      {
         send_to_char("You are still recovering from the last one.\n\r", ch );
         return;
      }

   if ( ch->hit > 1000000 )
      {
         send_to_char("#w'#eAhhh, you are too healthy to start the fun Elf #L*Cackles!!!*#w' #P- Lloth#n\n\r", ch );
         return;
      }

   SET_BIT( ch->more, MORE_BOMB );
   act("You suddenly cackle wildly as insanity crawls through your shivering bones.", ch, NULL, NULL, TO_CHAR );
   act("$n suddenly cackles wildly as insanity crawls through $s shivering bones.", ch, NULL, NULL, TO_ROOM );
   act("You sneer '#yImbeciles! You demons leave me with no choice...#n'", ch, NULL, NULL, TO_CHAR );
   act("$n sneers '#yImbeciles! You demons leave me with no choice...#n'", ch, NULL, NULL, TO_ROOM );
   act("Dropping to your knees, you open your palms as a spiral of light fades in sight.", ch, NULL, NULL, TO_CHAR );
   act("Dropping to $s knees, $n opens $s palms as a spiral of light fades in sight.", ch, NULL, NULL, TO_ROOM );
   act("As the light hovers in the air, it suddenly molds into a mystical box of chaos energy.", ch, NULL, NULL, TO_CHAR );
   act("As the light hovers in the air, it suddenly molds into a mystical box of chaos energy.", ch, NULL, NULL, TO_ROOM );
   act("Everyone around you screams with absolute terror in their eyes as you move towards the box.", ch, NULL, NULL, TO_CHAR );
   act("You scream with absolute terror in your eyes as $n moves towards the box.", ch, NULL, NULL, TO_ROOM );
   act("You cackle '#yTremble before the true power of Lloth.#n' as you open the box slowly.", ch, NULL, NULL, TO_CHAR );
   act("$n cackles '#yTremble before the true power of Lloth.#n' as $e opens the box slowly.", ch, NULL, NULL, TO_ROOM );
   act("A tear of happiness rolls down your cheeks as the room is suddenly filled with blinding light.", ch, NULL, NULL, TO_CHAR );
   act("A tear of fear rolls down your cheeks as the room is suddenly filled with blinding light.", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "#c%s #Rfoolishly #wopens #cthe #RP#ea#rn#ed#Ro#er#Ra #wBox#c!!!#n", ch->name );
   do_info( ch, buf );

   switch( number_range( 1, 20 ) )
   {
       case 5:
       case 17:
       case 12: if ( !armageddon )
                   {
                      armageddon = TRUE;
                      armageddon_timer = 24;
                      do_info( NULL, "#wRun #gfor #yyour #Clives!!! #wArmageddon #ehas #R#zarrived!!!#n" );
                      restore_char(ch );
                      return;
                   }

                stc( "#eWithout even glancing at you, #wLloth #Rfades #eback into the #Ldarkness...#n\n\r", ch );
                break;

       case 10:
       case 14:
       case 8: if ( mysafe != 0 )
                  {
                     mysafe = 0;
                     do_info( NULL, "#wS#ca#wf#ce#ws #gare back #y#zin#n#g!!!#n");
                     return;
                  }

                mysafe = 10;
                do_info( NULL, "#eS#ra#ef#re#es #gare #P#zout#n#g!!!#n");
                break;

       case 6:
       case 16: for ( d = descriptor_list; d != NULL; d = d->next )
                {
                   if ( d->connected == CON_PLAYING && !IS_IMMORTAL( d->character ) )
                      {
                         stc( "#wLloth #Rcackles #ewith glee as she #Rsaps #eaway all of your #wlifeforce#e.#n\n\r", d->character );
                         d->character->hit = 1;
                         d->character->mana = 1;
                         d->character->move = 1;
                         d->character->agg = 100;
                         d->character->fight_timer = 20;

                         if ( !IS_SET( d->character->more, MORE_PINCH ) ) 
                            SET_BIT( d->character->more, MORE_PINCH );
                      }
                }
                break;

       case 3: for ( d = descriptor_list; d != NULL; d = d->next )
               {
                   if ( d->connected == CON_PLAYING && d->character != ch && !IS_IMMORTAL( d->character ) )
                      {
                         stc( "#wLloth #Rsmirks #eevilly as she #Rrestores all of your #wlifeforce#e.#n\n\r", d->character );
                         restore_char( d->character );
                      }
               }

               stc( "#eWithout even glancing at you, #wLloth #Rfades #eback into the #Ldarknesse...#n\n\r", ch );
               break;

       case 11: for ( d = descriptor_list; d != NULL; d = d->next )
                {
                   if ( d->connected == CON_PLAYING && !IS_IMMORTAL( d->character ) )
                      {
                         d->character->agg = 100;
                         paradox( d->character );
                      }
                }
                break;

       case 1:
       case 18:
       case 9: paradox( ch );
               break;

       case 7:
       case 2:
       case 15: stc( "#wLloth #Rcackles #ewith glee as she #Rblesses #eyou with #winfinite #Rpower#e!#n\n\r", ch );

                sprintf( buf, "#e%s #ggot #wMAJORLY blessed #gfor pleasing #wLloth#e!#n", ch->name );
                do_info( ch, buf );

                if ( !is_affected( ch, skill_lookup("rose storm") ) )
                   spell_rose_storm( skill_lookup( "rose storm" ), 50, ch, ch );

                if ( !is_affected( ch, skill_lookup("darkside") ) )
                   spell_darkside( skill_lookup( "darkside" ), 1000, ch, ch );

                if ( !is_affected( ch, skill_lookup("regeneration") ) )
                   spell_regeneration( skill_lookup( "regeneration" ), 1000, ch, ch );

                if ( !IS_SET( ch->monkstuff, MONK_HEAL ) )
                   SET_BIT( ch->monkstuff, MONK_HEAL );

                if ( !is_affected( ch, skill_lookup("blessing of the saints") ) )
                   {
                      af.type = skill_lookup("blessing of the saints");
                      af.duration  = 1000;
                      af.location  = 0;
                      af.modifier  = 0;
                      af.bitvector = 0;
                      affect_to_char( ch, &af );
                   }

                if ( !is_affected( ch, skill_lookup("alabaster potion") ) )
                   {
                      af.type = skill_lookup("alabaster potion");
                      af.duration  = 250;
                      af.location  = APPLY_STR;
                      af.modifier  = 4;
                      af.bitvector = 0;
                      affect_to_char( ch, &af );
                   }

                if ( !is_affected( ch, skill_lookup("haste") ) )
                   {
                      af.type = skill_lookup("haste");
                      af.duration  = 250;
                      af.location  = APPLY_DEX;
                      af.modifier  = 4;
                      af.bitvector = 0;
                      affect_to_char( ch, &af );
                   }
                
                ch->hit = 2500000;
                ch->mana = ch->max_mana;
                ch->move = ch->max_move;
                break;

       case 4:
       case 19: stc( "#wLloth #Rcackles #ewith glee as she #Rwhacks #eyou with tons of #Pmaladictions#e!#n\n\r", ch );

                sprintf( buf, "#e%s #ggot #wwhacked #gby tons of #emaladictions #gfor insulting #wLloth#e!#n", ch->name );
                do_info( ch, buf );

                lloth_curse( ch );

                break;

       case 13:
       case 20: for ( d = descriptor_list; d != NULL; d = d->next )
                {
                   if ( d->connected == CON_PLAYING && d->character != ch && !IS_IMMORTAL( d->character ) )
                      {
                         stc( "#wLloth #Rcackles #ewith glee as she #Rwhacks #eyou with tons of #Pmaladictions#e!#n\n\r", d->character );
                         lloth_curse( d->character );
                      }
                }

                stc( "#wLloth #Rsmiles #eas she #Rcurses #eall who #wstands #ein her way#e!#n\n\r", ch );
                break;
   }
}

void do_venom(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( IS_NPC( ch ) )
       return;

    if ( !IS_CLASS(ch, CLASS_DROW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
       {
          stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
          return;
       }

    if ( ch->pkill < 4 )
       {
          stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
          return;
       }

    if ( ch->mana < 150000 )
       {
          send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    ch->mana -= 150000;
	
    if ( argument[0] == '\0' )
       {
          send_to_char("Which item do you wish to coat with deadly poison?\n\r", ch);
          return;
       }

    if ( ( obj = get_obj_carry(ch, argument) ) == NULL )
       {
          send_to_char("You are not carrying that item.\n\r", ch);
          return;
       }

    if ( obj->item_type != ITEM_WEAPON )
       {
          send_to_char("That is not a weapon!\n\r", ch);
          return;
       }

    if ( IS_WEAP(obj, WEAPON_NPOISON) )
       {
          act("$p is already coated with a deadly venom.", ch, obj, NULL, TO_CHAR);
          return;
       }

    act("You coat $p with a deadly venom.", ch, obj, NULL, TO_CHAR);
    act("$n coats $p with a deadly venom.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 12);
    SET_BIT(obj->weapflags, WEAPON_NPOISON);
}
