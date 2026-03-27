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
#include "garou.h"

void do_lunar_touch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch)) return;

   if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "#oYour connection with #CLuna #oneeds to be stronger first.#n\n\r", ch );
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to bless with Luna Power?\n\r", ch );
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

   if ( ch == victim )
      {
         stc("You can't do this to yourself!\n\r", ch );
         return;
      }

   if ( !is_same_group(ch,victim) && !( ch->pcdata->kingdom == victim->pcdata->kingdom
         && ch->pcdata->kingdom > 0 ) )
      {
         stc("They have to be in the same group or kingdom as you.\n\r", ch );
         return;
      }

   if ( IS_SET( victim->elemental, ELE_LUNAR ) )
      {
         stc("They are already blessed with lunar energy.\n\r", ch );
         return;
      }

   act("You say, '#yBlessed Queen... Give them strength.'#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yBlessed Queen... Give them strength.'#n'",ch,NULL,NULL,TO_ROOM);
   act("You then howl mightily at the moon as a gentle beam enshrouds $N.",ch,NULL,victim,TO_CHAR);
   act("$n then howls mightily at the moon as a gentle beam enshrouds you.",ch,NULL,victim,TO_VICT);
   act("$n then howls mightily at the moon as a gentle beam enshrouds $N.",ch,NULL,victim,TO_NOTVICT);
   act("#oLuna smiles at you as you are now blessed with #wLuna Power#o.#n",victim,NULL,NULL,TO_CHAR);

   SET_BIT( victim->elemental, ELE_LUNAR );
}

void do_lunar_eclipse( CHAR_DATA *ch, char *argument )
{
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_WEREWOLF ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 25 )
      {
         stc( "#oYour connection with #CLuna #oneeds to be stronger first.#n\n\r", ch );
         return;
      }

   if ( weather_info.sky != SKY_ECLIPSE )
      {
         stc( "#eThere isn't an eclipse atm #ogarou warrior#e.#n\n\r", ch );
         return;
      }

   weather_info.sky = SKY_CLOUDLESS;
   WAIT_STATE( ch, 12 );

   send_to_char( "You beat your chest as you howl at the moon with all of your might.\n\r", ch );
   sta("#oThe eclipse suddenly fades away as the #wmoon #ogoes back to its #Cslumber#o.#n\n\r",ch->in_room->area);

   if ( ( sn = skill_lookup( "lunar eclipse" ) ) < 0 ) return;

   if ( is_affected(ch,skill_lookup("lunar eclipse") ) )
      {
         stc("You are already surrounded by a white outline!\n\r",ch);
         return;
      }

   (*skill_table[sn].spell_fun) ( sn, 100, ch, ch );
}

void do_lunar_power( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 30 )
      {
         stc( "#oYour connection with #CLuna #oneeds to be stronger first.#n\n\r", ch );
         return;
      }

   ch->wait = 12;
   act("You snarl, '#yFill us with the strength of the lands and the might of the moon!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n snarls, '#yFill us with the strength of the lands and the might of the moon!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You howl, '#yLUUUUUUUUUUUUNA#n' as it suddenly goes dark.",ch,NULL,NULL,TO_CHAR);
   act("$n howls, '#yLUUUUUUUUUUUUNA#n' as it suddenly goes dark.",ch,NULL,NULL,TO_ROOM);
   act("Gazing into the skies, you see the gentle moon pulsing with energy.",ch,NULL,NULL,TO_CHAR);
   act("Gazing into the skies, you see the gentle moon pulsing with energy.",ch,NULL,NULL,TO_ROOM);
   act("You then hear a whisper '#oYour prayer has been answered...#n'",ch,NULL,NULL,TO_CHAR);
   act("You then hear a whisper '#oYour prayer has been answered...#n'",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) )
         continue;

      if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
           && ch->pcdata->kingdom > 0 ) )
         continue;

      if ( IS_SET( vch->elemental, ELE_LUNARP ) )
         continue;

      act("As the rain splatters off your body, you feel more potent with your lunar powers.",vch,NULL,NULL,TO_CHAR);
      act("As the rain splatters off $n's body, $e feels more potent with $s lunar powers.",vch,NULL,NULL,TO_ROOM);
      SET_BIT( vch->elemental, ELE_LUNARP );
   }
}

void do_berserk( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( IS_NPC( ch ) )
     return;

  if ( ch->ctimer[15] > 0 )
     {
        stc( "#wYou are the #gO#wn#ge#w... You don't resort to these #ypetty #wmindless skills.#n\n\r", ch );
        return;
     }

  if ( IS_CLASS( ch, CLASS_NINJA ) )
     ch->wait = 24;
     else ch->wait = 12;

  act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
  act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) ) continue;

      if ( is_safe(ch,vch) ) continue;

      multi_hit( ch, vch, TYPE_UNDEFINED );
  }
}

bool is_garou( CHAR_DATA *ch )
{
  if ( IS_NPC(ch) ) return FALSE;
  if ( IS_CLASS(ch, CLASS_WEREWOLF) )
    return TRUE;
  else
    return FALSE;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_CLASS( ch, CLASS_SHADOW ) )
       {
          do_assassin_vanish( ch, "" );
          return;
       }

    send_to_char("Huh?\n\r",ch);
    return;

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
       {
          REMOVE_BIT(ch->act, PLR_WIZINVIS);
          send_to_char( "You slowly fade into existance.\n\r", ch );
          act("$n slowly fades into existance.",ch,NULL,NULL,TO_ROOM);
       }
       else {
               send_to_char( "You slowly fade out of existance.\n\r", ch );
               act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM);
               SET_BIT(ch->act, PLR_WIZINVIS);
            }
}

void do_moonarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }
    
    if ( arg[0] == '\0' )
       {
          send_to_char("What piece of moon armour do you wish to create? Ring Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r", ch);
          return;
       }

    if      (!str_cmp(arg,"ring"     )) vnum = 341;
    else if (!str_cmp(arg,"bracer"    )) vnum = 342;
    else if (!str_cmp(arg,"collar"    )) vnum = 343;
    else if (!str_cmp(arg,"belt"   )) vnum = 345;
    else if (!str_cmp(arg,"plate"    )) vnum = 344;
    else if (!str_cmp(arg,"helmet"    )) vnum = 346;
    else if (!str_cmp(arg,"leggings"    )) vnum = 347;
    else if (!str_cmp(arg,"boots"    )) vnum = 348;
    else if (!str_cmp(arg,"gloves"    )) vnum = 349;
    else if (!str_cmp(arg,"sleeves"    )) vnum = 350;
    else if (!str_cmp(arg,"cape"    )) vnum = 351;
    else if (!str_cmp(arg,"mask"    )) vnum = 352;
    else
    {
    do_moonarmor(ch,"");
    return;
    }
    if (ch->gold < 1)
    {
	send_to_char("It costs #w1 #ygold piece#n to create a piece of moon armour\n\r",ch);
	return;
    }
   
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Jobo.\n\r",ch);
	return;
    }

    ch->gold -= 6;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in a beam of moonlight.",ch,obj,NULL,TO_CHAR);
    act("$p appears in a beam of moonlight.",ch,obj,NULL,TO_ROOM);
}

void do_spiral_moon( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          send_to_char("Huh?\n\r", ch);
          return;
       }

    if ( get_stock( ch ) < 300 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    ch->wait = 6;
    subtract_stock( ch, 300 );
    SET_BIT( ch->arena, AFF2_DM );

    act("You crouch on your knee as you tilt your hands closely together.",ch,NULL,NULL,TO_CHAR);
    act("$n crouches on $s knee as $e tilts $s hands closely together.",ch,NULL,NULL,TO_ROOM);
    act("You howl loudly as a MaSsIvE moon fades between your hands.",ch,NULL,NULL,TO_CHAR);
    act("$n howls loudly as a MaSsIvE moon fades between $s hands.",ch,NULL,NULL,TO_ROOM);
    act("The moon goes blurry as it flares with the power of Luna.",ch,NULL,NULL,TO_CHAR);
    act("The moon goes blurry as it flares with the power of Luna.",ch,NULL,NULL,TO_ROOM);
    act("The moon FlAsHeS through the vicinity as a manifest of yellow energy appears in your hands!!!",ch,NULL,NULL,TO_CHAR);
    act("The moon FlAsHeS through the vicinity as a manifest of yellow energy appears in $n's hands!!!",ch,NULL,NULL,TO_ROOM);
    act("As the energy ball glows bigger and bigger, your eyes glow red as you snarl with rage!",ch,NULL,NULL,TO_CHAR);
    act("As the energy ball glows bigger and bigger, $n's eyes glow red as you snarl with rage!",ch,NULL,NULL,TO_ROOM);
    act("You howl '#yAaaawoooOOOoH!!!#n' as you release a BLAST of LuNa energy across the room!",ch,NULL,NULL,TO_CHAR);
    act("$n howls '#yAaaawoooOOOoH!!!#n' as $e releases a BLAST of LuNa energy across the room!",ch,NULL,NULL,TO_ROOM);
    hit_gain( ch, 1000000 );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch,vch) )
          continue;

       if ( is_safe(ch,vch) )
          continue;

       if ( !is_affected( vch, skill_lookup("blessing of the saints") )
            && !is_affected( vch, skill_lookup("weaken") ) )
          {
              af.type      = skill_lookup("weaken");
              af.duration  = 20;
              af.location  = APPLY_STR;
              af.modifier  = -5;
              af.bitvector = 0;
              affect_to_char( vch, &af );

              af.type      = skill_lookup("weaken");
              af.duration  = 20;
              af.location  = APPLY_DEX;
              af.modifier  = -5;
              af.bitvector = 0;
              affect_to_char( vch, &af );
 
              af.type      = skill_lookup("weaken");
              af.duration  = 20;
              af.location  = APPLY_WIS;
              af.modifier  = -5;
              af.bitvector = 0;
              affect_to_char( vch, &af );
          }

       ch->attack_type = ATTACK_MAGICAL;
       ch->ele_attack = ATTACK_LUNAR;
       ch->attack_var = SOUL;
       ch->attack_focus = DIRECT;
       act("Your body goes pale as the energy beams soars through you!",vch,NULL,NULL,TO_CHAR);
       act("$n's body goes pale as the energy beams soars through $m!",vch,NULL,NULL,TO_ROOM);
       hurt_person( ch, vch, 3000000, TRUE );
    }
}

void do_moonbeam( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int dam = 700000;

    argument = one_argument( argument, arg );
  
    if ( IS_NPC(ch) )
    	return;
 
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 250000 )
       {
          send_to_char("You do not have enough energy to summon a Moonbeam.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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
  
    act("$N is struck by a huge moonbeam.", ch, NULL, victim,TO_CHAR);
    act("$N is struck by a deadly beam of moonlight from $n.", ch, NULL, victim,TO_NOTVICT);
    act("You are struck by a deadly beam of moonlight!", ch, NULL, victim,TO_VICT);

    ch->mana -= 250000;
    WAIT_STATE( ch, 12 );
    ch->ele_attack = ATTACK_LUNAR;
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = BEAM;
    ch->attack_focus = AERIAL;
    hurt_person( ch, victim, dam, TRUE );
}

void do_gifts( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    char lin [MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    { 
	sprintf(lin,"================================================================================\n\r");
	stc(lin, ch);
	stc("                                     Breed\n\r",ch);
	stc(lin, ch);
	sprintf(buf,"        Homid     [ %d ]         Metis    [ %d ]         Lupus       [ %d ]\n\r",
			ch->gifts[HOMID], ch->gifts[METIS], ch->gifts[LUPUS] );
	stc(buf, ch);
        stc(lin, ch);
	stc("                                    Auspice\n\r",ch);
	stc(lin, ch);
	sprintf(buf,"       Ragabash   [ %d ]        Theurge   [ %d ]        Philodox     [ %d ]\n\r",
		      ch->gifts[RAGABASH], ch->gifts[THEURGE], ch->gifts[PHILODOX] );
	stc(buf, ch);
	sprintf(buf,"       Galliard   [ %d ]                                 Ahroun     [ %d ]\n\r",
	      ch->gifts[GALLIARD], ch->gifts[AHROUN] );
	stc(buf, ch);
	stc(lin, ch);
	  stc("                                     Tribe\n\r",ch);
	stc(lin, ch);
	sprintf(buf,"     Black Furies [ %d ]       Wendigo    [ %d ]    Children of Gaia [ %d ]\n\r",
			ch->gifts[BLACKFURIES], ch->gifts[WENDIGO], ch->gifts[CHILDREN] );
	stc(buf, ch);
	sprintf(buf,"     Silver Fangs [ %d ]       Uktena     [ %d ]      Get of Fenris  [ %d ]\n\r",
			ch->gifts[SILVERFANGS], ch->gifts[UKTENA],ch->gifts[GETOFFENRIS] );
	stc(buf, ch);
	sprintf(buf,"      Red Talons  [ %d ]     Shadow Lords [ %d ]     Silent Striders [ %d ]\n\r",
			ch->gifts[REDTALONS], ch->gifts[SHADOWLORDS], ch->gifts[SILENTSTRIDERS] );
	stc(buf, ch);
	sprintf(buf,"      Fianna      [ %d ]                             StarGazers     [ %d ]\n\r",
	ch->gifts[FIANNA], ch->gifts[STARGAZERS]);
	stc(buf, ch);
	stc(lin, ch);
	return;
    }

    if (arg2[0] == '\0')
    {

      if ( !str_cmp(arg1, "ragabash" ) )
      {
        if ( ch->gifts[RAGABASH] == 0 )
        {
	  stc("You have no knowledge of the Gifts of Ragabash.\n\r",ch);
	  return;
	}
	if ( ch->gifts[RAGABASH] >= 1 )
	{
	  divide_to_char(ch);
	  centre_text("The Gifts of Ragabash",ch);
	  divide_to_char(ch);
	  centre_text("#oVanish#n",ch);
	}

	if ( ch->gifts[RAGABASH] >= 2 )	
	  centre_text("#yScent of Running Water (Auto)#n",ch);
	if ( ch->gifts[RAGABASH] >= 4 )
	  centre_text("#oBlissful#n",ch);

	if ( ch->gifts[RAGABASH] >= 4 )
	  centre_text("#oLunasBlessing#n",ch);
	if ( ch->gifts[RAGABASH] >= 5 )
	  centre_text("#oWhelp#n",ch);
	divide_to_char(ch);
	return;
      }

      if ( !str_cmp(arg1, "lupus") )
      {
	if ( ch->gifts[LUPUS] == 0 )
	{
	  stc("You have no knowledge of the Gifts of Lupus\n\r",ch);
	  return;
	}
	if ( ch->gifts[LUPUS] >= 1 )
	{
	  divide_to_char(ch);
	  centre_text("The Gifts of Lupus",ch);
	}
	if ( ch->gifts[LUPUS] >= 3 )
	  centre_text("#oGnaw#n",ch);
	if ( ch->gifts[LUPUS] >= 4 )
	  centre_text("#oElementalGift#n",ch);
	if ( ch->gifts[LUPUS] >= 5 )
	  centre_text("#oSongBeast#n",ch);
	divide_to_char(ch);
	return;
      }

      if ( !str_cmp(arg1, "metis") )
      {
	if ( ch->gifts[METIS] == 0 )
	{
	  stc("You have no knowledge of the Gifts of Metis.\n\r",ch);
	  return;
	}
	if ( ch->gifts[METIS] >= 1 )
	{
	  divide_to_char(ch);
	  centre_text("The Gifts of Metis",ch);
	  divide_to_char(ch);
	  centre_text("#oQuills#n",ch);
	}
	if ( ch->gifts[METIS] >= 3 )
	  centre_text("#yEyes of the Cat (Auto)#n",ch);
	if ( ch->gifts[METIS] >= 4 )
	  centre_text("#oWither#n",ch);
	divide_to_char(ch);
	return;
      }

      if ( !str_cmp(arg1, "theurge") )
      {
	if ( ch->gifts[THEURGE] == 0 )
	{
	  stc("You have no knowledge of the Theurge Gifts.\n\r",ch);
	  return;
	}
        else
	{
	  divide_to_char(ch);
	  centre_text("The Gifts of Theurge",ch);
	  divide_to_char(ch);
	  centre_text("#oMother#n",ch);
	  centre_text("#yGrasp the Beyond (Auto)#n",ch);
	  centre_text("#oSpiritDrain#n",ch);
	  centre_text("#oFeral#n",ch);
	  centre_text("#oMalleable#n",ch);
          divide_to_char(ch);
	  return;
	}
      }
      if ( !str_cmp(arg1, "Philodox" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of Philodox",ch);
	divide_to_char(ch);
	centre_text("#yResist Pain (Auto)#n",ch);
	centre_text("#yKing of the Beasts (Auto)#n",ch);
	centre_text("#yStrength of Purpose (Auto)#n",ch);
	centre_text("#yWeak Arm (Auto)#n",ch);
	centre_text("#oWallOfGranite#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp(arg1, "Galliard" ))
{
	divide_to_char(ch);
        centre_text("The Gifts of Galliard",ch);
        divide_to_char(ch);
        centre_text("#oKlaive#n",ch);
        centre_text("#oRageSong#n",ch);
        centre_text("#oBridgeGate#n",ch);
        divide_to_char(ch);
        return;
      }
      if ( !str_cmp(arg1, "Ahroun" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of Ahroun",ch);
	divide_to_char(ch);
	centre_text("#oRazor Claws#n",ch);
	centre_text("#yHeart of Fury (Auto)#n",ch);
	centre_text("#oClenched#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp( arg1, "Black Furies" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Black Furies",ch);
	divide_to_char(ch);
	centre_text("#yCoup De Grace (Auto)#n",ch);
	centre_text("#yVisceral Agony (Auto)#n",ch);
	centre_text("#oBodyWrack#n",ch);
	centre_text("#oAeolus#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp( arg1, "Children of Gaia" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Children of Gaia",ch);
	divide_to_char(ch);
	centre_text("#oDazzle#n",ch);
	centre_text("#oGarouSerenity#n",ch);
	centre_text("#oHaloOfSun#n",ch);
	centre_text("#oLivingWood#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp(arg1, "Get of Fenris" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Get of Fenris",ch);
	divide_to_char(ch);
	centre_text("#oSnarl#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp(arg1, "Red Talons") )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Red Talons",ch);
	divide_to_char(ch);
	centre_text("#oAvalanche#n",ch);
	centre_text("#oBarrage#n",ch);
	centre_text("#oVengeance#n",ch);
	centre_text("#oQuicksand#n",ch);
	centre_text("#oWaste#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp( arg1, "Shadow Lords" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Shadow Lords",ch);
	divide_to_char(ch);
	centre_text("#oClapofThunder#n",ch);
	centre_text("#oIcyChill#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp( arg1, "Silent Striders" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Silent Striders",ch);
	divide_to_char(ch);
	centre_text("#ySpeed of Thought (Auto)#n",ch);
	centre_text("#ySpeed Beyond Thought (Auto)#n",ch);
	centre_text("#yMessenger's Speed (Auto)#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp(arg1, "Silver Fangs" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Silver Fangs",ch);
	divide_to_char(ch);
	centre_text("#oLambent#n",ch);
	centre_text("#oLunasAvenger#n",ch);
	centre_text("#oPaws#n",ch);
	divide_to_char(ch);
	return;
      }
     if ( !str_cmp(arg1, "Fianna") )
      {
        divide_to_char(ch);
        centre_text("The Gifts of Fianna",ch);
        divide_to_char(ch);
        centre_text("#oBrew#n",ch);
        centre_text("#oBalorGaze#n",ch);
	centre_text("#oSpriggan#n",ch);
        divide_to_char(ch);
	return;
}
      if ( !str_cmp(arg1, "Stargazers") )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the StarGazers",ch);
	divide_to_char(ch);
	centre_text("#oMerciful#n",ch);
	centre_text("#oPreternatural Awareness (Auto)#n",ch);
	centre_text("#oCircle#n",ch);
      centre_text("#ySight from Beyond (Auto)#n",ch);  
      centre_text("#oMoonBeam#n",ch);  
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp(arg1, "Uktena" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Uktena",ch);
	divide_to_char(ch);
	centre_text("#oDarkShroud#n",ch);
	centre_text("#oFlameSpirit#n",ch);
	divide_to_char(ch);
	return;
      }
      if ( !str_cmp(arg1, "Wendigo" ) )
      {
	divide_to_char(ch);
	centre_text("The Gifts of the Wendigo",ch);
	divide_to_char(ch);
	centre_text("#oCuttingWind#n",ch);
	centre_text("#oCannibal#n",ch);
	centre_text("#oHeartOfIce#n",ch);
	centre_text("#oChillFrost#n",ch);
	divide_to_char(ch);
	return;
      }

      if ( !str_cmp(arg1, "homid") )
      {
        if ( ch->gifts[HOMID] == 0 )
        {
	  stc("You have no knowledge of the Gift of Homid.\n\r",ch);
	  return;
        }
        if ( ch->gifts[HOMID] >= 1 )
        {
	  divide_to_char(ch);
	  centre_text("The Gifts of Homid",ch);
	}
	if ( ch->gifts[HOMID] >= 2 )
        centre_text("#oStareDown#n",ch);
	if ( ch->gifts[HOMID] >= 3 )
	  centre_text("#oDisquiet#n",ch);
	if ( ch->gifts[HOMID] >= 4 )
	  centre_text("#oReshape#n",ch);
	if ( ch->gifts[HOMID] >= 5 )
	  centre_text("#oCocoon#n",ch);
	divide_to_char(ch);
	return;
      }
      else
      {
        do_gifts(ch, "");
        return;
      }
      return;
    }

    if (!str_cmp(arg2,"learn"))
    {
	int improve;
	int cost;
	int max = 5;

	     if (!str_cmp(arg1,"homid"            )) improve = HOMID;
	else if (!str_cmp(arg1,"metis"            )) improve = METIS;
	else if (!str_cmp(arg1,"lupus"            )) improve = LUPUS;
	else if (!str_cmp(arg1,"ragabash"         )) improve = RAGABASH;
	else if (!str_cmp(arg1,"theurge"          )) improve = THEURGE;
	else if (!str_cmp(arg1,"philodox"         )) improve = PHILODOX;
        else if (!str_cmp(arg1,"galliard"	  )) improve = GALLIARD;
 	else if (!str_cmp(arg1,"fianna"         )) improve = FIANNA;  
	else if (!str_cmp(arg1,"ahroun"           )) improve = AHROUN;
	else if (!str_cmp(arg1,"black furies"     )) improve = BLACKFURIES;
        else if (!str_cmp(arg1,"children of gaia" )) improve = CHILDREN;
	else if (!str_cmp(arg1,"get of fenris"    )) improve = GETOFFENRIS;
        else if (!str_cmp(arg1,"red talons"       )) improve = REDTALONS;
	else if (!str_cmp(arg1,"shadow lords"     )) improve = SHADOWLORDS;
	else if (!str_cmp(arg1,"silent striders"  )) improve = SILENTSTRIDERS;
	else if (!str_cmp(arg1,"silver fangs"     )) improve = SILVERFANGS;
	else if (!str_cmp(arg1,"stargazers"       )) improve = STARGAZERS;
	else if (!str_cmp(arg1,"uktena"           )) improve = UKTENA;
	else if (!str_cmp(arg1,"wendigo"          )) improve = WENDIGO;
        else
	{
	    do_gifts(ch,"");
	    return;
	}

	cost = (ch->gifts[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->gifts[improve] >= max )
	{
	    sprintf(buf,"You have already gained all of the %s gifts.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}

	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to learn a new %s gift.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You have learned a new %s gift.\n\r", arg1);
	send_to_char(buf,ch);
	return;
    }

    else send_to_char("#wTo learn a new gift, type: #ygifts #o'<name>' #ylearn#w.#n\n\r",ch);
}

void do_dazzle( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];

  argument = one_argument( argument, arg );
 
  if ( !is_garou(ch) ) return;

  if ( ch->ctimer[5] > 0 )
     {
        send_to_char("You are still recovering from the last one.\n\r", ch);
        return;
     }

  if ( arg[0] == '\0' )
     {
        stc("Whom do you wish to use this on?\n\r",ch);
        return;
     }

  if ( ( victim = get_char_room(ch, arg) ) == NULL )
     {
        stc("They aren't here.\n\r",ch);
        return;
     }

  if ( is_safe( ch, victim ) )
     return;

  act("You gaze at $N, dazzling $M with Gaia's Glory.",ch,NULL,victim,TO_CHAR);
  act("You catch $n's gaze, and are dazzled by $s glory.",ch,NULL,victim,TO_VICT);
  act("$N catches $n's gaze, and is dazzled by $s glory.",ch,NULL,victim,TO_NOTVICT);
  act("You collapse to the ground still affected by the power of Gaia.",victim,NULL,NULL,TO_CHAR);
  act("$n collapses to the ground still affected by the power of Gaia.",victim,NULL,NULL,TO_ROOM);

  ch->ctimer[5] = 6;
  stop_fighting( victim, TRUE );
  victim->position = POS_STUNNED;
}

void do_aeolus(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *rch;

    if ( !IS_CLASS(ch,CLASS_WEREWOLF) || ch->clannum != 1 )
       {
           stc("Huh?\n\r",ch);
           return;
       }

    if ( ch->move < 100000 )
       {
           stc("You are too tired to do this.\n\r",ch);
           return;
       }

    if ( ch->ctimer[1] > 0 )
       {
           stc("You are still recovering from the last one.\n\r",ch);
           return;
       }

    if ( ch->fighting == NULL )
       {
           stc("You are fighting no one.\n\r",ch);
           return;
       }

    ch->move -= 100000;
    ch->ctimer[1] = 4;
    act("$n fills the room with fog.",ch,NULL,NULL,TO_ROOM);
    act("You fill the room with fog.",ch,NULL,NULL,TO_CHAR);
    
    for ( rch = ch->in_room->people; rch != NULL; rch=rch->next_in_room )
    {
       if ( rch->fighting == ch || rch== ch )
          {
	       stop_fighting(rch,TRUE);

	       if ( rch==ch )
                continue;

             rch->wait = 36;

	       act("You find it impossible to continue battle with $n.",ch,NULL,rch,TO_VICT);
	       act("$N is blinded by fog.",ch,NULL,rch,TO_NOTVICT);
          }
    }
}


void do_icychill( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

    if ( ch->fight_timer > 0 )
       {
          send_to_char("Not until your fight timer expires.\n\r", ch );
          return;
       }

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
	    send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[SHADOWLORDS] < 4 )
       {
          send_to_char("You must obtain at least level 4 in Shadow Lords to use Icy Chill.\n\r",ch);
          return;
       }

    if ( ch->mana < 200000 )
       {
          send_to_char("You don't have enough mana for that.\n\r",ch);
          return;
       }
    
    if ( IS_SET( ch->more, MORE_SAFE ) )
       {
          REMOVE_BIT( ch->more, MORE_SAFE );
          act( "You lower your icy chill of despair.",  ch, NULL, NULL, TO_CHAR );
          act( "$n looks less scary.",  ch, NULL, NULL, TO_NOTVICT );
          return;
       }
  
    SET_BIT( ch->more, MORE_SAFE );
    act( "You raise your icy chill of despair.",  ch, NULL, NULL, TO_CHAR);
    act( "$n suddenly looks very scary.",  ch, NULL, NULL, TO_NOTVICT);
}

/*
 * Homid Gifts
 * Level One  - Persuasion    - Like Awe for Vampires.  Automatic
 * Level Two  - Staredown     - Mobs flee
 * Level Three- Disquiet      - Spell effect, -hit\dam in combat for opponent.
 * Level Four - Reshape Object- Rename objects
 * Level Five - Cocoon        - Damage Reduction
 */

/*
 * Homid - 2 - Staredown 
 */
void do_staredown( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
	    stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[HOMID] < 2 )
       {
          stc("You have not learned the Homid gift of Staredown.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

    arg[0] = UPPER(arg[0]);    

    if ( victim->fighting == NULL )
       {
          sprintf(buf, "%s is not fighting anyone.", arg);
          stc(buf, ch);
          return;
       }  

    WAIT_STATE( ch, 18 );

    if ( IS_NPC(victim) )
    {
      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR);
      act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      ch->wait = 6;
      return;
    }

    else
    {
        if ( number_percent() < 89 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT);
          act("$n stares deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_NOTVICT);
	    return;
        }

        act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR);
        act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT);
        act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT);
        do_flee( victim, "" );

        if ( victim->wait < 18 )
           victim->wait = 18;
    }
}

void do_disquiet( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

    if ( is_affected( victim, skill_lookup("weaken") ) )
    {
      stc("They are already weakened.",ch);
      return;
    }

    if ( is_safe( ch, victim) ) return;

    if ( ch->gnosis[GCURRENT] < 10 )
       {
          stc("You need ten points of gnosis power to use the gift of Disquiet.\n\r",ch);
          return;
       }

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       {
          stc("They are protected by the holy saints.\n\r",ch);
          return;
       }

    af.type      = skill_lookup("weaken");
    af.duration  = 20;
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = skill_lookup("weaken");
    af.duration  = 20;
    af.location  = APPLY_DEX;
    af.modifier  = -2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    ch->gnosis[CURRENT] -= 10;
    act("Your eyes glow a sinister red as you glare at $N.",ch,NULL,victim,TO_CHAR);
    act("$n's eyes glow a sinister red as $e glares at you.\n\rYou feel odd.",ch,NULL,victim,TO_VICT);
    act("$n's eyes glow a sinister red as $e glares at $N.",ch,NULL,victim,TO_NOTVICT);
}

void do_brew( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA	*obj;
    char	arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) 
       return;

    if ( !IS_CLASS(ch,CLASS_WEREWOLF) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[FIANNA] < 1 )
       {
          stc("You have not learned the Fianna gift of brew.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "You must state what you wish to brew?\n\r", ch );
          return;
       }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
       {
          send_to_char( "That isn't here.\n\r", ch );
          return;
       }

    if ( obj->item_type != ITEM_FOUNTAIN )
       {
          send_to_char( "You can only brew fountains.\n\r", ch );
          return;
       }

    if ( strlen( obj->questmaker ) > 2 && obj->questmaker != NULL
    &&	 !str_cmp( obj->questmaker, ch->name ) )
    {
	send_to_char( "You can't brew a fountain that belongs to someone else.\n\r", ch );
	return;
    }

    if ( obj->value[2] == 1 )
    {
	obj->value[2] = 0;
	act( "You transform the beer in $p into water.", ch, obj, NULL,TO_CHAR );
	act( "$n transforms the beer in $p into water.", ch, obj, NULL, TO_ROOM );
    }
    else if ( obj->value[2] == 0 )
    {
	obj->value[2] = 1;
	act( "You change the water in $p into beer.", ch, obj, NULL,TO_CHAR );
	act( "$n change the water in $p into beer.", ch, obj, NULL,TO_ROOM );
    }
    else
	send_to_char( "You can only change water and beer.\n\r", ch );
}

void do_giant( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[FIANNA] < 5 )
       {
          send_to_char("You have not learned this Fianna gift.\n\r",ch);
          return;
       }

    
    if ( IS_POLYAFF(ch, POLY_ZULO) )
       {   
          REMOVE_BIT(ch->polyaff, POLY_ZULO);
          REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
          act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
          act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );

          free_string( ch->morph );
          ch->morph = str_dup( "" );

          if ( IS_SET(ch->special, SPC_WOLFMAN) )
             {
                SET_BIT(ch->affected_by, AFF_POLYMORPH);
                SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
                sprintf(buf,"#o%s #ythe #ow#Rerewol#of#n",ch->name);
                free_string(ch->morph);
                ch->morph = str_dup(buf);
             }
          return;
       }
  
    if ( ch->mounted == IS_RIDING )
       do_dismount(ch,"");

    act( "You transform into large beast.", ch, NULL, NULL, TO_CHAR );
    act( "$n's body grows and distorts into a giant werewolf.", ch,NULL, NULL, TO_ROOM );
        
    SET_BIT(ch->polyaff, POLY_ZULO);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf, "#o%s #yA massive #ow#Rerewol#of#n", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
}

void do_balorsgaze( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );
 
    if ( IS_NPC(ch) )
       return;
 
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) || ch->clannum != 2 )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }
 
    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

    act("You close your eyes and concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n's right eye begins to glow a baleful red!",ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and cencentrates on $N.",ch,NULL,victim,TO_NOTVICT);

    WAIT_STATE(ch, 12);

    act("You scream in pain as your body is torn apart from the inside out!", victim,NULL,NULL,TO_CHAR);
    act("$n screams in pain as there body is torn apart from the inside out!", victim,NULL,NULL,TO_ROOM);
    
    if ( !IS_SET( victim->more, MORE_PAIN ) )
       SET_BIT( victim->more, MORE_PAIN );

    if ( !IS_SET( victim->more, MORE_PINCH ) )
       SET_BIT( victim->more, MORE_PINCH );
}

void do_reshape(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    OBJ_DATA *obj; 
 
    argument=one_argument(argument,arg1);
    strcpy(arg2,argument);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch,CLASS_WEREWOLF) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[HOMID] < 4 )
       {
          send_to_char("You have not achieved the Homid gift of Reshape Object.\n\r",ch);
          return;
       }

    if ( arg1 == NULL || arg2 == NULL )
       {
          send_to_char("Syntax: Reshape (Item) (Description).\n\r",ch);
          return;
       }

    if ( ( obj = get_obj_carry( ch,arg1 ) ) == NULL )
       {
          send_to_char("You dont have that item.\n\r",ch);
          return;
       }

    if ( strlen(arg2) > 40 || strlen(arg2) < 3 )
       {
          send_to_char("The name should be between 3 and 40 characters.\n\r",ch);
          return;
       }

    free_string(obj->name);
    obj->name = str_dup(arg2);
    free_string(obj->short_descr);
    obj->short_descr=str_dup(arg2);
    send_to_char("Ok.\n\r",ch);
}

void do_cocoon( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( IS_CLASS(ch, CLASS_HOBBIT) )
       {
          do_hobbit_cocoon( ch, argument );
          return;
       }

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
	    stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[HOMID] < 5 )
       {
          stc("You haven't learned the Homid gift of Cocoon.\n\r",ch);
          return;
       }


    if ( IS_GAR1(ch, WOLF_COCOON) )
       {
          stc("Your cocoon breaks and falls to the ground.\n\r",ch);
          act("$n's cocoon breaks and falls to the ground.",ch,NULL,NULL,TO_ROOM);
          REMOVE_BIT(ch->garou1, WOLF_COCOON);
          return;
       } 

    if ( ch->gnosis[GCURRENT] < 1 )
       {
          stc("You do not have enough gnosis to use Cocoon.\n\r",ch);
          return;
       }

    stc("Your body is surrounded by a thick, opaque epidermis.\n\r",ch);
    act("$n's body is surrounded by a thick, opaque epidermis.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->garou1, WOLF_COCOON);
    ch->gnosis[GCURRENT]--;
}

/*
 * Metis Gifts
 * Level One  - Gift of the Porcupine - Quills
 * Level Two  - Burrow          - Like Demon Travel
 * Level Three- Eyes of the Cat - Nightsight
 * Level Four - Wither Limb	- Withers a limb, easier on NPCs
 * Level Five - Totem Gift	- Random, Special Power. Ooo.
 */

void do_quills( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( IS_SET(ch->newbits, NEW_QUILLS) )
       {
          stc("Your quills soften and twist into fur.\n\r",ch);
          act("$n's quills soften and revert back to fur.",ch,NULL,NULL,TO_ROOM);
          REMOVE_BIT(ch->newbits, NEW_QUILLS);
          return;
       }

    stc("Your fur becomes bristly and sharp.\n\r",ch);
    act("$n's fur becomes bristly and sharp.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->newbits, NEW_QUILLS);
}
  
// Eyes of the Cat - Nightsight.  clan.c

void do_wither( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    bool blah = FALSE;
    int chance = 0;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[METIS] < 4 )
       {
          stc("You haven't learned the Metis gift of Wither Limb.\n\r",ch);
          return;
       }

    if ( ch->gnosis[GCURRENT] < 3 )
       {
          stc("You do not have enough gnosis power stored to use the gift of Wither Limb.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          stc("Who's limb do you wish to wither?\n\r",ch);
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          stc("They aren't here.\n\r",ch);
          return;
       }

    if ( IS_ARM_R(victim, LOST_ARM) && IS_ARM_L(victim, LOST_ARM) )
       {
          stc("They have already lost both their arms.\n\r",ch);
          return;
       }
 
    WAIT_STATE(ch, 18);
    ch->gnosis[CURRENT] -= 3;

    if ( IS_NPC( victim ) )
    {
      chance = number_range( 45, 55 );

      if ( number_percent( ) > chance )
         {
            stc("Nothing happened.\n\r",ch);
            return;
         }

       if ( !IS_ARM_R(victim, LOST_ARM) )
           blah = FALSE;
           else blah = TRUE;

	 if ( !blah )
          {
 	  if (!IS_ARM_R(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[3],LOST_ARM);
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
	  act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
	  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
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
	  return;
         }
       
	 else if ( blah )
	 {
 	  if (!IS_ARM_L(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[2],LOST_ARM);
	  else blah = TRUE;
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
	  act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
	  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
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
	  if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	      take_item(victim,obj);
	  return;
        }
    }
    else if ( !IS_NPC(victim) )
    {
      if ( is_safe(ch,victim) )
         return;

      if ( number_percent( ) < 85 || IS_CLASS(victim, CLASS_PALADIN) || IS_CLASS(victim, CLASS_SORCEROR)
           || IS_CLASS(victim, CLASS_DARK_KNIGHT) )
      {
	act("You gaze intently at $N.\n\rNothing happens.",ch,NULL,victim,TO_CHAR);
	act("$n gazes eerily at you.\n\rNothing happens.",ch,NULL,victim,TO_VICT);
	act("$n gazes eerily at $N.",ch,NULL,victim,TO_NOTVICT);
	return;
      }

       if ( !IS_ARM_R(victim, LOST_ARM) )
         blah = FALSE;
       else blah = TRUE;

	 if ( !blah )
         {
 	  if (!IS_ARM_R(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[3],LOST_ARM);
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
	  act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
	  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
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
	  return;
	 }

	 else if ( blah )
	 {
 	  if (!IS_ARM_L(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[2],LOST_ARM);
	  else blah = TRUE;
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
	  act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
	  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
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
	  if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	      take_item(victim,obj);
	  return;
        }
      }
  return;
}

/* Lupus Gifts
 * Heightened Senses-do_truesight
 * Catfeet-Auto
 * Gnaw
 * Elemental Gift
 * Song of the Great Beast
 */
void do_heightened( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[LUPUS] < 1 )
     {
        stc("You must obtain level one Lupus to use Heightened Senses.\n\r",ch);
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

void do_gnaw( CHAR_DATA *ch, char *argument )
{
  if ( !is_garou(ch) )
     return;

  stc("You try to gnaw through your restrains.\n\r",ch);
  act("$n gnaws at $s restrains.",ch,NULL,NULL,TO_ROOM);

  if ( IS_EXTRA(ch, TIED_UP) )
     {
        act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
        act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->extra, TIED_UP);
     }

  if ( IS_SET(ch->more, MORE_NET) )
     {
        send_to_char("You tear yourself free of the net.\n\r",ch);
        REMOVE_BIT(ch->more, MORE_NET); 
     }

  if ( IS_SET(ch->more, MORE_IMPALE) )
     {
        send_to_char("You gnaw away at the spear which was pinning your feet down.\n\r",ch);
        REMOVE_BIT(ch->more, MORE_IMPALE); 
     }

  if ( is_affected(ch, gsn_web) )
     {
        act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
        send_to_char("The webbing entrapping you breaks away.\n\r",ch);
        affect_strip(ch, gsn_web);
     }

  if ( IS_AFFECTED(ch, AFF_WEBBED) )
     {
        act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
        send_to_char("The webbing entrapping you breaks away.\n\r",ch);
        REMOVE_BIT(ch->affected_by, AFF_WEBBED);
     }

  if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
     {
        stc("#gThe earth surrounding you starts to slowly crack.#n\n\r",ch);
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
     }

  if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
     {
        stc("#gThe earth surrounding you falls away.#n\n\r",ch);
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
     }

  if ( IS_SET( ch->more, MORE_HAMMER ) )
     {
        send_to_char( "You flex and the hammer #0axe#n comes out your arm.\n\r", ch );
        REMOVE_BIT( ch->more, MORE_HAMMER );
     }
}

void do_elementalgift( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) 
     return;

  if ( ch->gifts[LUPUS] < 4 )
     {
        stc("You must obtain level 5 Lupus to use Elemental Gift.\n\r",ch);
        return;
     }

  if ( ch->gnosis[GCURRENT] < 1 )
     {
        stc("You must have one point of Gnosis to use this power.\n\r",ch);
        return;
     }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  dam = ch->gifts[LUPUS] * 100000;

  if ( IS_CLASS(victim, CLASS_VAMPIRE) ) dam *= 1.5;

  act("You call upon the forces of Gaia to annihilate $N.",ch,NULL,victim,TO_CHAR);
  act("$n throws back $s head and howls loudly, causing the room to shake.\nThe room turns dark, and a blast of elemental fire consumes you.",ch,NULL,victim,TO_VICT);
  act("$n throws back $s head and howls loudly, causing the room to shake.\nThe room turns dark.\nA blast of elemental fire consumes $N!",ch,NULL,victim,TO_NOTVICT);
  ch->ele_attack = ATTACK_FIRE;
  hurt_person(ch,victim,dam,TRUE);
  ch->gnosis[GCURRENT] -= 1;
  WAIT_STATE(ch, 18);
}  

void do_songbeast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[LUPUS] < 5 )
  {
    stc("You must obtain level 5 Lupus to use this power.\n\r",ch);
    return;
  }

  if ( ch->gnosis[GCURRENT] < 3 )
  {
    stc("You need 3 points of gnosis to use this power.\n\r",ch);
    return;
  }

    if ( ch->pcdata->followers > 1 )
    {
      stc("Nothing happens..\n\r",ch);
      return;
    }

    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->short_descr = str_dup("A huge, bloodthirsty Dinosaur");
    victim->name = str_dup("dinosaur");
    victim->long_descr = str_dup("A huge dinosaur towers over you, roaring loudly and looking for prey. ");
    victim->level = 990;
    victim->hit = 1500000;
    victim->max_hit = 1500000;
    victim->hitroll = 1500000;
    victim->damroll = 1500000;
    victim->armor = -150000;
    SET_BIT(victim->act, ACT_NOEXP);
    
    act("You howl loudly at the sky.\nThe ground beneath your feet begins to shake.\n  Suddenly,  A huge dinosaur emerges from the earth below you.",ch,NULL,NULL,TO_CHAR);
    act("$n howls loudly at the sky.\nThe ground beneath your feet begins to shake.\n  Suddenly, A huge dinosaur bursts from the ground with a loud roar!",ch,NULL,NULL,TO_ROOM);
    char_to_room( victim, ch->in_room );

    add_follower( victim, ch );
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    ch->gnosis[GCURRENT] -= 3;
}

/*
 * Ahroun Gifts
 * 1=razorclaws
 */

void do_razorclaws(CHAR_DATA *ch, char *argument)
{

   if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
      return;

   if ( !IS_VAMPAFF(ch, VAM_CLAWS) )
   {
	stc("Your talons aren't even extended!\n\r",ch);
	return;
   }

   if ( IS_GAR1(ch, WOLF_RAZORCLAWS ) )
   {
      stc("Your claws lose their razor-sharp edge.\n\r",ch);
      act("$n's claws lose their razor-sharp edge.",ch,NULL,NULL,TO_ROOM);
      REMOVE_BIT(ch->garou1, WOLF_RAZORCLAWS);

      if ( IS_SET( ch->more3, MORE3_CLAWS ) )
         REMOVE_BIT( ch->more3, MORE3_CLAWS );
   }
   else
   {
	send_to_char("You hone your claws to a razor sharpness.\n\r",ch);
	act("$n hones $s claws to a razor sharpness.",ch, NULL, NULL, TO_ROOM);
	SET_BIT( ch->garou1, WOLF_RAZORCLAWS );

      if ( !IS_SET( ch->more3, MORE3_CLAWS ) )
         SET_BIT( ch->more3, MORE3_CLAWS );
   }

  return;

}

/* Ragabash
 * Vanish
 * Scent of Running Water - No hunt\track(auto)
 * Blissful Ignorance
 * Luna's Blessing
 * Whelp Body
 */

void do_blissful( CHAR_DATA *ch, char *argument )
{
  if ( !is_garou(ch) ) return;
  
  if ( ch->gifts[RAGABASH] < 3 )
  {
    stc("You must obtain level three Ragabash to use Blissful Ignorance.\n\r",ch);
    return;
  }

  if ( IS_GAR1(ch, GAROU_BLISSFUL) )
  {
    stc("You fade back into view.\n\r",ch);
    REMOVE_BIT(ch->garou1, GAROU_BLISSFUL);
    return;
  }
  else
  {
     if ( ch->fight_timer > 0 )
        {
            stc("Not with a fight timer.\n\r",ch);
            return;
        }

    stc("You become completely invisible.\n\r",ch);
    SET_BIT(ch->garou1, GAROU_BLISSFUL);
    return;
  }
}

void do_lunasblessing( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  
  if ( !IS_CLASS(ch, CLASS_WEREWOLF) ) return;

  if ( ( sn = skill_lookup( "lunas blessing" ) ) < 0 ) return;

  if ( is_affected(ch,skill_lookup("lunas blessing")) )
  {
    stc("You already have Luna's Blessing!\n\r",ch);
    return;
  }

  stc("A beam of moonlight shoots out of the sky and surrounds you.\n\r",ch);
  act("$n is engulfed by a beam of moonlight from the sky.",ch,NULL,NULL,TO_ROOM);
  level = (ch->gifts[RAGABASH]*5);
  (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
  WAIT_STATE( ch, 12 );
}

void do_whelp( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;  

  argument = one_argument( argument, arg );

  if ( !is_garou( ch ) ) return;

    if ( ch->pkill < 10 )
       {
         stc("You aren't powerful enough to do this technique as yet.\n\r",ch);
	   return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 10 )
  {
    stc("You require 10 points of gnosis to use this power.\n\r",ch);
    return;
  }

  if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
     {
        stc("They are protected by the holy saints.\n\r",ch);
        return;
     }

  if ( IS_SET(victim->garou1, GAROU_WHELP2)
	|| IS_SET(victim->garou1, GAROU_WHELP1) )
  {
    stc("They are already affected by a Whelp Curse.\n\r",ch);
    return;
  }

  if ( is_safe(ch,victim) )
     return;

  SET_BIT(victim->garou1, GAROU_WHELP2);
  act("You lay your hands on $N, and $S body starts to shake and quiver.",ch,NULL,victim,TO_CHAR);
  act("$n lays $s hands on you, and you feel your body quiver and weaken.",ch,NULL,victim,TO_VICT);
  act("$n lays $s hands on $N, and $N's body starts to shake and quiver.",ch,NULL,victim,TO_NOTVICT);

  if ( victim->max_hit > 750001 )
     victim->max_hit -= 750000;

  victim->hit = UMIN( victim->hit, victim->max_hit );

  WAIT_STATE(victim, 8);
  WAIT_STATE(ch, 4);
  ch->gnosis[GCURRENT] -= 10;
}

/* Theurge
 * Mother's Touch
 * Grasp the Beyond
 * Spirit Drain
 * Feral Lobotomy
 * The Malleable Spirit
 */

void do_mothers( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg );
  
  if ( !is_garou(ch) ) return;

  if ( ch->gifts[THEURGE] < 1 )
  {
    stc("You must obtain level 1 Theurge to use Mother's Touch.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to heal?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("You cannot find them here.\n\r",ch);
    return;
  }

  if ( victim == ch && ch->mana < 100000 )
  {
    stc("You cannot use this power on yourself!\n\r",ch);
    return;
  }

  if ( victim->hit >= victim->max_hit )
  {
    stc("They are already at full health!\n\r",ch);
    return;
  }

  act("You lay your hands on $N, rejuvenating them.",ch,NULL,victim,TO_CHAR);
  act("$n lays $s hands on you, and you feel healthier.",ch,NULL,victim,TO_VICT);
  act("$n lays $s hands on $N, and $N's wounds begin to close.",ch,NULL,victim,TO_NOTVICT);
  victim->hit += 250000;
  if ( victim->hit > victim->max_hit )
    victim->hit = victim->max_hit;

  if ( victim == ch )
     ch->mana -= 100000;

  WAIT_STATE(ch, 12);
}

// grasp the beyond. auto. lets you get shit from shadowplane.
void do_spiritdrain( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  int hpgain;
 
  one_argument( argument, arg );

  if ( !is_garou(ch) ) return; 
 
  if ( arg[0] == '\0' )
  {
    send_to_char("Drain the spirit of which corpse?\n\r",ch);
    return;
  }

  obj = get_obj_list( ch, arg, ch->in_room->contents );
  if ( obj == NULL )
  {
    send_to_char( "You can't find it.\n\r", ch );
    return;
  }

  if (obj->item_type != ITEM_CORPSE_NPC)
  {
    act( "You are unable to drain the energy of $p.", ch, obj, 0, TO_CHAR);
    return; 
  }
  else if (obj->chobj != NULL && !IS_NPC(obj->chobj)
    && obj->chobj->pcdata->obj_vnum != 0)
  {
    act( "You are unable to drain the energy of $p.", ch, obj, 0, TO_CHAR);
    return;
  }

  hpgain = number_range(100000,250000);
  ch->hit += hpgain;
  if (ch->hit > ch->max_hit)
    ch->hit = ch->max_hit;
  act( "You drain the spirit energy out of $p.", ch, obj, NULL, TO_CHAR );
  act( "$p is drained of energy by $n.", ch, obj, NULL, TO_ROOM );
  extract_obj( obj );
  WAIT_STATE(ch, 12);
}

void do_feral( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[THEURGE] < 4 )
  {
    stc("You must obtain level 4 Theurge to use Feral Lobotomy.\n\r",ch);
    return;
  }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 3 )
  {
    stc("You need three points of gnosis to use this power.\n\r",ch);
    return;
  }

  dam = ch->gifts[THEURGE] * 100000;

  act("You focus your gnosis power on $N.",ch,NULL,victim,TO_CHAR);
  act("$n's claws deliver a power blow of energy to your head.",ch,NULL,victim,TO_VICT);
  act("$n's claws slash open $N's forehead.",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 12;
  hurt_person(ch,victim,dam,TRUE);
  ch->gnosis[GCURRENT] -= 3;
}
  
void do_malleable( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg[MIL];
  
  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->clannum != 2 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( arg[0] == '\0' )
  {
    stc("Which object do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You aren't even carrying that object.\n\r",ch);
    return;
  }

  if ( obj->item_type != ITEM_WEAPON ) 
  {
    stc("This power can only be used on weapons.\n\r",ch);
    return;
  }

  if ( IS_SET(obj->quest, QUEST_MALLEABLE) )
  {
    stc("This object has already been modified.\n\r",ch);
    return;
  }

  obj->value[1] += (ch->gifts[THEURGE]* 1500);
  obj->value[2] += (ch->gifts[THEURGE]* 1500);
  act("You manipulate the spirit in $p, increasing it's effectiveness.",ch,obj,NULL,TO_CHAR);
  SET_BIT(obj->quest, QUEST_MALLEABLE);
  WAIT_STATE(ch, 12);
}
  
/* Philodox Gifts
 * Resist Pain (Auto)
 * King of the Beasts (Auto)
 * Strength of Purpose (Auto)
 * Weak Arm (Auto)
 * Wall of Granite
 */

void do_wallofgranite( CHAR_DATA *ch, char *argument )
{
  if (!is_garou(ch) ) return;

  if ( ch->gifts[PHILODOX] < 5 )
  {
    stc("You must obtain level 5 Philodox to use Wall of Granite.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->garou1, WW_GRANITE) )
  {
    stc("Your wall of granite fades away.\n\r",ch);
    REMOVE_BIT(ch->garou1, WW_GRANITE);
  }

  if ( !IS_SET(ch->garou1, WW_GRANITE) )
  {
    stc("A wall of granite arises around your body to protect you.\n\r",ch);
    SET_BIT(ch->garou1, WW_GRANITE);
    return;
  }
}

/* Ahroun
 * Inspiration (Auto)
 * Razor Claws (Auto)
 * The Falling Touch (Auto)
 * Silver Claws (Auto)
 * Clenched Jaw
 */

void do_clenched( CHAR_DATA *ch, char *argument )
{
  if ( !is_garou(ch) ) return;

  if ( IS_SET(ch->garou1, GAROU_CLENCHED) )
  {
    stc("You relax your jaw.\n\r",ch);
    REMOVE_BIT(ch->garou1, GAROU_CLENCHED);
    return;
  }

  if ( !IS_GAR1(ch, GAROU_CLENCHED) )
  {
    stc("You prepare to clench your jaw on your opponents.\n\r",ch);
    SET_BIT(ch->garou1, GAROU_CLENCHED);
    return;
  }
}

/* Black Furies
 * Heightened Senses
 * Coup De Grace (Auto)
 * Visceral Agony (Auto)
 * Body Wrack
 * Wasp Talons (Auto)
 */

void do_bodywrack( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[BLACKFURIES] < 4 )
  {
    stc("You must obtain level 4 Black Furies to use Body Wrack.\n\r",ch);
    return;
  }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 15 )
  {
    stc("You need 15 points of gnosis to use this power.\n\r",ch);
    return;
  }

  if ( victim->hit < 100000 )
  {
    stc("They are almost dead already.  It wouldn't be any fun.\n\r",ch);
    return;
  }

  dam = number_range( ch->gifts[BLACKFURIES] * 120000, ch->gifts[BLACKFURIES]* 140000 );
  act("You focus a blast of gnosis on $N.",ch,NULL,victim,TO_CHAR);
  act("A blast of energy shoots out of $n's claws and wracks your body.",ch,NULL,victim,TO_VICT);
  ch->ele_attack = ATTACK_DARK;
  hurt_person(ch,victim,dam,TRUE);
  ch->gnosis[GCURRENT] -= 15;
  WAIT_STATE(ch, 18);
}
  

/* Children of Gaia
 * Luna's Armor (Auto)
 * Dazzle
 * Calm the Beast
 * Halo of the Sun
 * The Living Wood
 */
void do_garouserenity( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg);

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[CHILDRENOFGAIA] < 3 )
  {
    stc("You must obtain the third Children of Gaia gift to use Serenity.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to calm?\n\r",ch);
    return;
  }
  
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }
 
  if ( number_range(1,4) != 3 )
  {    
      stc("You sing a blissful and soothing song.\n\r",ch);
      act("$n starts singing a blissful and soothing song.",ch,NULL,victim,TO_VICT);
      stc("Nothing seems to happen..\n\r",ch);
      WAIT_STATE(ch,12);
      return;
  }

  if ( IS_CLASS(victim, CLASS_VAMPIRE) )
  {
    if ( victim->rage < 1 )
    {
      stc("They aren't even raged!\n\r",ch);
      return;
    }
    else
    {
      stc("You sing a blissful and soothing song.\n\r",ch);
      act("$n starts singing a blissful and soothing song.",ch,NULL,victim,TO_VICT);
      do_calm(victim,"");
      return;
    }
    WAIT_STATE(ch, 16);
    return;
  }
  else if ( IS_CLASS(victim, CLASS_WEREWOLF) )
  {
    if ( victim->rage < 100 )
    {
	stc("They aren't even raged!\n\r",ch);
	return;
    }

    if ( victim->fight_timer > 0 )
       {
          stc( "They are so enraged atm that no amount of singing will calm them down.\n\r" , ch );
          return;
       }

    WAIT_STATE(ch, 12);
    stc("You sing a blissful and soothing song.\n\r",ch);
    act("$n starts singing a blissful and soothing song.",ch,NULL,victim,TO_VICT);
    do_unwerewolf(victim,"");
    return;

  }
}

void do_halosun( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch,CLASS_WEREWOLF) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

   if (ch->gifts[CHILDRENOFGAIA] < 4)
    {
	send_to_char("You require the level 4 Children of Gaia power to use the Halo of the Sun.\n\r",ch);
	return;
    }

    send_to_char("You are surrounded by a halo of sunlight!\n\r",ch);
    act("$n is surrounded by a halo of sunlight!", ch, NULL, NULL,TO_ROOM);
    return;
}

void do_livingwood( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[CHILDRENOFGAIA] < 5 )
  {
    stc("You must obtain the fifth Children of Gaia gift to use this power.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this power on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }

  if ( IS_SET(victim->mflags, MAGE_EMBRACED2) || IS_SET(victim->mflags, MAGE_EMBRACED1) )
  {
    stc("They are already embraced by the earth.\n\r",ch);
    return;
  }

  act("You call upon the forest to entrap your foe.",ch,NULL,NULL,TO_CHAR);
  act("$n chants softly.\nThe ground around you begins to rise up, entrapping you.",ch,NULL,victim,TO_VICT);
  act("$n begins to chant softly.\nSuddenly, the ground surrounding $N begins to rise up and entrap $M!",ch,NULL,victim,TO_NOTVICT);
  SET_BIT(victim->mflags, MAGE_EMBRACED1);
  WAIT_STATE(ch, 16);
}

/* Get of Fenris
 * Halt the Coward's Flight (Auto)
 * Snarl of the PRedator
 * MIght of Thor (Auto)
 * Venom Blood (Auto)
 * Fenris' Bite (Auto)
 */

void do_snarl( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[GETOFFENRIS] < 2 )
  {
    stc("You must obtain the second gift of Get Of Fenris to use this power.\n\r",ch);
    return;
  }

  if ( ch->fighting == NULL )
  {
    stc("You must be fighting to use this power.\n\r",ch);
    return;
  }

  victim = ch->fighting;

  if ( victim->wait > 0 )
  {
    stc("You snarl visciously, but nothing seems to happen..\n\r",ch);
    return;
  }

  act("You snarl visciously at $N, sending chills up $S spine..",ch,NULL,victim,TO_CHAR);
  act("$n snarls visciously at you.\nYou are scared stiff.",ch,NULL,victim,TO_VICT);

  if ( number_percent() > 90 )
     WAIT_STATE(victim,24);
     else WAIT_STATE(victim,12);

  WAIT_STATE(ch, 12);
}

/* Red Talons
 * Avalanche
 * Quicksand
 * Gaia's Vengeance
 * ONLY THREE POWERS IN THIS SONUVABITCH.
 */

void do_avalanche( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  DESCRIPTOR_DATA *d;

  if ( IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) 
       && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
     {
        do_dragoon_avalanche( ch, argument );
        return;
     }

  if ( !is_garou(ch) ) return;

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  stc("#CYou #wcall upon the power of #oGaia #wto unleash an #Lavalanche #won your foes.#n\n\r",ch);
  act("#C$n #wcalls upon the power of #oGaia #wto unleash an #LAvalanche #won the area!#n",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !IS_NPC( vch ) || is_safe(ch, vch) )
        continue;

     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_ICE;
     ch->attack_var = HIT;
     ch->attack_focus = LOW;
     act("#yYou #ware engulfed by the #Cbarrage #wof snow as you are swept away!#n",vch,NULL,NULL,TO_CHAR);
     act("#y$n #wis engulfed by the #Cbarrage #wof snow as $e is swept away!#n",vch,NULL,NULL,TO_ROOM);
     hurt_person( ch, vch, 2000000, TRUE );
  }

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
     if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
        {
           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
              continue;

           if ( ch->in_room->area != d->character->in_room->area )
              continue;

           act("#yYou #ware engulfed by the #Cbarrage #wof snow as you are swept away!#n",d->character,NULL,NULL,TO_CHAR);
           act("#y$n #wis engulfed by the #Cbarrage #wof snow as $e is swept away!#n",d->character,NULL,NULL,TO_ROOM);
           ch->attack_type = ATTACK_MAGICAL;
           ch->ele_attack = ATTACK_ICE;
           ch->attack_var = HIT;
           ch->attack_focus = LOW;
           hurt_person( ch, d->character, 2000000, TRUE );
        }
  }
}

void do_barrage( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( !is_garou(ch) ) return;

  if ( ch->gnosis[GCURRENT] < 30 )
     {
        stc("You must have 30 points of gnosis to use this power.\n\r",ch);
        return;
     }

  ch->gnosis[GCURRENT] -= 30;
  WAIT_STATE(ch, 6);

  act("#yYou #gcall upon the power of #oGaia #gto barrage its might on its foes!#n",ch,NULL,NULL,TO_CHAR);
  act("#y$n #gcalls upon the power of #oGaia #gto barrage its might on its foes!#n",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_safe(ch, vch) )
        continue;

     vch->mana -= 500000;
     vch->move -= 500000;
     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_EARTH;
     ch->attack_var = HIT;
     ch->attack_focus = LOW;
     hurt_person( ch, vch, 400000, TRUE );
  }
}

/* Shadow Lords
 * Fatal Flaw (Auto)
 * Aura of Confidence (Auto)
 * Clap of Thunder
 * Icy Chill of Dispair (Auto)
 * Strength of the Dominator (Auto)
 */

void do_clapofthunder( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( !is_garou(ch) ) return;
  
  if ( ch->fight_timer > 0 )
  {
    stc("Not with a fight timer.\n\r",ch);
    return;
  }

  if ( ch->gnosis[GCURRENT] < 10 )
  {
    stc("You must have 10 points of gnosis to use this power.\n\r",ch);
    return;
  }
 
  stc("You clap your hands together, sending forth a shockwave that stuns everyone in the room.\n\r",ch);
  act("$n claps $s hands together, sending forth a shockwave that stuns you instantly!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
       vch_next = vch->next_in_room;

       if ( vch == ch )
          continue;

       if ( is_same_group(ch,vch) ) 
          continue;

       if ( !is_safe(ch, vch) )
          {
               stc("You hear an ear poping howl and are suddenly attacked from all sides.\n\r",vch);
               act("You hear an ear poping howl, $n is suddenly attacked from all sides.",vch,NULL,NULL,TO_ROOM);

               vch->hit = UMAX( vch->hit - 500000, 1 );
               vch->position = POS_STUNNED;
	    }
  }

  ch->fight_timer = 20;
  ch->gnosis[GCURRENT] -= 10;
}

/* Silver Fangs
 * Awe (Auto)
 * Silver Claws (Auto)
 * Mindblock (Auto)
 * Luna's Avenger 
 * Paws of the Newborn Cub
 */
void do_lunasavenger( CHAR_DATA *ch, char *argument )
{
  if ( !is_garou(ch) ) return;

  if ( ch->gifts[SILVERFANGS] < 4 )
  {
    stc("You must obtain the fourth gift of the Silver Fangs to use Luna's Avenger.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->garou1, GAROU_AVENGER) )
  {
    stc("Your body warps and shrinks, losing it's silver composition.\n\r",ch);
    REMOVE_BIT(ch->garou1, GAROU_AVENGER);
    return;
  }

  else
  {
    stc("Your body warps and grows, becoming imbued with silver.\n\r",ch);
    SET_BIT(ch->garou1, GAROU_AVENGER);
    return;
  }
}

void do_paws( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg );
  
  if ( !is_garou(ch) ) return;

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 5 )
     {
        stc("You need 5 points of gnosis to use this power.\n\r",ch);
        return;
     }

  if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
     {
        send_to_char("They are protected by the holy saints.\n\r", ch );
        return;
     }

  if ( IS_SET(victim->garou1, GAROU_PAWS) )
     {
        stc("They are already afflicted with this disease..\n\r",ch);
        return;
     }

  act("You focus your gnosis power into your hands, and lay them on $N.",ch,NULL,victim,TO_CHAR);
  act("$n's hands start to glow violently, and $e reaches out and lays them on you.",ch,NULL,victim,TO_VICT);
  act("$n's hands start to glow brightly, and $e reaches out and presses them against $N's forehead.",ch,NULL,victim,TO_NOTVICT);
  act("$N's body starts to weaken and shrink!",ch,NULL,victim,TO_CHAR);
  act("$N's body starts to weaken and shrink!",ch,NULL,victim,TO_NOTVICT);
  stc("Your body starts to weaken and shrink!\n\r",victim);
  SET_BIT(victim->garou1, GAROU_PAWS);
  ch->gnosis[GCURRENT] -= 5;
  WAIT_STATE(ch, 4);
}

/* Stargazers
 * Inner Strength (Auto)
 * Merciful Blow 
 * Preternatural Awareness (Auto)
 * Circular Attack (Auto)
 */
void do_merciful( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  
  if ( !is_garou(ch) ) return;

  if ( ch->gifts[STARGAZERS] < 2 )
  {
    stc("You must obtain the second gift of the Stargazers to use Merciful Blow.\n\r",ch);
    return;
  }

  if ( ch->fighting == NULL )
  {
    stc("You must be fighting to use this power.\n\r",ch);
    return;
  }

  victim = ch->fighting;
  
  if ( victim->hit > 1000000 )
     {
        stc("They are far too healthy to be affected by this power.\n\r",ch);
        return;
     }

  if ( number_percent() < 90 )
  {
    act("You deliver a blow to $N's neck, staggering $S nerval system.",ch,NULL,victim,TO_CHAR);
    act("$n delivers a powerful blow to your neck, staggering your nerval system.",ch,NULL,victim,TO_VICT);
    act("$n delivers a powerful blow to $N's neck, but staggering $S nerval system.",ch,NULL,victim,TO_NOTVICT);
    damage(ch,victim,number_range(500000,650000),FALSE);
    ch->wait = 18;
    return;
  }
  else
  {
    act("You deliver a blow to $N's neck, and $N falls to the ground.",ch,NULL,victim,TO_CHAR);
    act("$n delivers a powerful blow to your neck, and you fall to the ground.",ch,NULL,victim,TO_VICT);
    act("$n delivers a powerful blow to $N's neck, and $E falls to the ground.",ch,NULL,victim,TO_NOTVICT);
    stop_fighting(victim,TRUE);
    stop_fighting(ch,TRUE);
    victim->position = POS_STUNNED;
  }
}

/* Uktena
 * Shroud
 * Spirit of the Bird (Auto)
 * Call Flame Spirit
 * Invisibility
 * Hand of the Earth Lords (Auto)
 */

void do_darkshroud( CHAR_DATA *ch, char *argument )
{
  if ( !is_garou(ch) ) return;

  if ( ch->fight_timer > 0 )
  {
    stc("Not with a fight timer.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
  {
    stc("The room is already in darkness.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
  {
    stc("Not in a safe room!\n\r",ch);
    return;
  }

  stc("You summon a shroud of inky blackness to cover the room.\n\r",ch);
  act("$n summons a shroud of inky blackness to cover the room.\n\r",ch,NULL,NULL,TO_ROOM);
  SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
  WAIT_STATE(ch,8);
}

void do_flamespirit( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;
  
  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;
  
  if ( ch->gifts[UKTENA] < 3 )
  {
    stc("You must obtain the third gift of the Uktena to use Call the Flame Spirit.\n\r",ch);
    return;
  }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 2 )
  {
    stc("You need two points of gnosis to use this power.\n\r",ch);
    return;
  }

  dam = number_range( (ch->gifts[UKTENA]*70000), (ch->gifts[UKTENA]*90000) );

  act("You summon a spirit of Flames to disintegrate $N.",ch,NULL,victim,TO_CHAR);
  act("$n conjures a spirit of flames to smite you.",ch,NULL,victim,TO_VICT);
  act("$n conjures a spirit of flames to smite $N!",ch,NULL,victim,TO_NOTVICT);
  ch->ele_attack = ATTACK_FIRE;
  hurt_person(ch,victim,dam,TRUE);
  ch->gnosis[GCURRENT] -= 2;
  WAIT_STATE(ch, 12);
}

void do_invisibility( CHAR_DATA *ch, char *argument )
{
  if (!is_garou(ch) ) return;

  if ( ch->fight_timer > 0 )
  {
    stc("Not with a fight timer.\n\r",ch);
    return;
  }

  if ( IS_GAR1(ch, GAROU_BLISSFUL) )
  {
    stc("You fade out of your invisibility.\n\r",ch);
    REMOVE_BIT(ch->garou1, GAROU_BLISSFUL);
    return;
  }
  else
  {
    stc("Your body fades out of existance.\n\r",ch);
    SET_BIT(ch->garou1, GAROU_BLISSFUL);
    return;
  }
}

/* Wendigo
 * Camouflage
 * Cutting Wind
 * Call the Cannibal Spirit
 * Heart of Ice
 * Chill of the Early Frost
 */

void do_cuttingwind( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[WENDIGO] < 2 )
  {
    stc("You must obtain the second gift of the Wendigo to use Cutting Wind.\n\r",ch);
    return;
  }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 2 )
  {
    stc("You need 2 points of gnosis to use this power.\n\r",ch);
    return;
  }

  if ( is_safe(ch,victim) )
     return;

  dam = number_range( 400000, 600000 );

  if ( IS_NPC(victim) )
    dam *= 2;

  act("You summon the bitter winds of the Great Wendigo to rip $N.",ch,NULL,victim,TO_CHAR);
  act("$n summons the bitter winds of the Great Wendigo to tear you apart.",ch,NULL,victim,TO_VICT);
  act("$n summons the bitter winds of the Great Wendigo to tear $N apart.",ch,NULL,victim,TO_NOTVICT);
  ch->ele_attack = ATTACK_ICE;
  hurt_person(ch,victim,dam,TRUE);
  ch->gnosis[GCURRENT] -= 2;
  WAIT_STATE(ch, 12);
}

void do_cannibalspirit( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *mob;
  char arg[MIL];
  AFFECT_DATA af;

  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->pkill < 5 )
     {
        stc("You aren't powerful enough to do this technique.\n\r",ch);
        return;
     }

  if ( ch->ctimer[0] > 0 )
  {
    stc("You are still recovering from the last one.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    stc("You cannot find them in this realm.\n\r",ch);
    return;
  }

  if ( ch->gnosis[GCURRENT] < 20 )
  {
    stc("You need 20 gnosis points to use this power.\n\r",ch);
    return;
  }

  if ( is_safe( ch, victim ) )
     return;

  if ( ( mob = create_mobile( get_mob_index( MOB_VNUM_CANNIBAL ) ) ) == NULL )
  {
    stc("Shit.  The Mob is missing.  Fuck.  Tell someone. Fuck!\n\r",ch);
    return;
  }
  
  SET_BIT(mob->affected_by, AFF_DETECT_INVIS);
  SET_BIT(mob->affected_by, AFF_DETECT_HIDDEN);
  SET_BIT(mob->affected_by, AFF_SHADOWSIGHT);
  mob->level = 999;
  ch->ctimer[0] = 4;

  af.type      = skill_lookup("protection from evil");
  af.duration  = -1;
  af.location  = APPLY_NONE;
  af.modifier  = 0;
  af.bitvector = AFF_PROTECT;
  affect_to_char( mob, &af );

  SET_BIT( mob->itemaffect, ITEMA_RESISTANCE );

  if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
     SET_BIT( mob->affected_by, AFF_ETHEREAL );

  if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
     SET_BIT( mob->affected_by, AFF_SHADOWPLANE );

  char_to_room( mob, ch->in_room );

  act("You summon a Great Wendigo avatar to hunt your foe.",ch,NULL,victim,TO_CHAR);
  act("$N bows before you and grins evilly.",ch,NULL,mob,TO_CHAR);
  act("$N bursts out of the ground and grins evilly.",ch,NULL,mob,TO_CHAR);

  char_from_room( mob );
  char_to_room( mob, victim->in_room );

  stc("The huge beast quickly burrows into the ground and seeks out your foe.\n\r",ch);
  stc("A huge werewolf bursts out of the ground and growls verociously.\n\r",victim);
  stc("The werewolf turns to you and grins.\n\r",victim);

  mob->hit = mob->max_hit = 10000000;
  mob->hitroll = mob->damroll = 2500000;
  SET_BIT( mob->act, ACT_TIMER );
  mob->practice = 1;

  victim->wait = UMAX( victim->wait, 12 );

  do_kill( mob, victim->name );
  ch->gnosis[GCURRENT] -= 20;
}

void do_heartofice( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[WENDIGO] < 4 )
  {
    stc("You must obtain the fourth gift of the Wendigo to use this power.\n\r",ch);
    return;
  }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to strike?\n\r",ch);
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

  if ( ch->gnosis[GCURRENT] < 10 )
  {
    stc("You must have 10 points of gnosis to use this power.\n\r",ch);
    return;
  }

  if ( is_safe(ch,victim) ) return;

  dam = number_range( 500000, 600000 );

  act("You summon the frost of the Wendigo to chill $N.",ch,NULL,victim,TO_CHAR);
  act("A cold air fills the room.\nYou are gripped with pain as the cold freezes your body.",ch,NULL,victim,TO_VICT);

  if ( number_percent() > 85 )
     victim->wait = 12;

  ch->ele_attack = ATTACK_ICE;
  hurt_person(ch,victim,dam,TRUE);
  ch->gnosis[GCURRENT] -= 10;
  WAIT_STATE(ch,18);
}

void do_chillfrost( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( !is_garou(ch) ) return;

  if ( ch->gifts[WENDIGO] < 5 )
  {
    stc("You must obtain the fifth gift of the Wendigo to use this power.\n\r",ch);
    return;
  }

  if ( ch->gnosis[GCURRENT] < 40 )
  {
    stc("You need 40 points of gnosis to use this power.\n\r",ch);
    return;
  }

  stc("You conjure the very essence of the Great Wendigo to blanket the area.\nThe skies turn white and the air turns bitter cold.\n\r",ch);
  act("The skies turn white and the sun is banished.\nThe room becomes bitterly cold, and your body starts to shiver.",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
       vch_next = vch->next_in_room;

       if ( vch == ch )
          continue;

       if ( is_same_group(ch,vch) ) 
          continue;

       if ( is_safe(ch, vch) )
          continue;

       if ( number_percent() > 90 && !IS_SET(vch->more, MORE_FROZEN) && !is_martial( vch ) )
          {
             SET_BIT(vch->more, MORE_FROZEN);
             vch->freeze_timer = number_range( 1, 2 );
          }

       ch->ele_attack = ATTACK_ICE;
       hurt_person(ch,vch,number_range(400000,750000),TRUE);
  }

  ch->gnosis[GCURRENT] -= 40;
  WAIT_STATE(ch, 12);
}

void do_quicksand(CHAR_DATA *ch,char *argument)
{
  CHAR_DATA *vch;

  if ( !IS_CLASS( ch,CLASS_WEREWOLF ) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( IS_SET( ch->in_room->room_flags, ROOM_QUICKSAND ) )
     {
        stc("The ground around you resumes its firmness.\n\r",ch);
        REMOVE_BIT(ch->in_room->room_flags, ROOM_QUICKSAND);
        return;
     }

  if ( ch->gnosis[GCURRENT] < 30 )
     {
	  stc("You must have 30 points of gnosis to use this.\n\r",ch);
	  return;
     }

  ch->gnosis[GCURRENT] -= 30;
  stc("#oThe ground around you becomes #Psoft#o.#n\n\r",ch);
  SET_BIT( ch->in_room->room_flags, ROOM_QUICKSAND );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
  {
      if ( is_safe(ch,vch) || is_same_group(ch,vch) || !IS_AFFECTED(vch, AFF_FLYING) )
         continue;

      REMOVE_BIT(vch->affected_by, AFF_FLYING);

      act( "#oYou suddenly find yourself struggling in the quicksand as your flying powers fade.#n", vch, NULL, NULL, TO_CHAR );
      act( "#o$n suddenly finds $mself struggling in the quicksand as $s flying powers fade.#n", vch, NULL, NULL, TO_ROOM );
      vch->wait += 18;
  }
}

void do_waste(CHAR_DATA *ch,char *argument)
{
  if ( !IS_CLASS(ch,CLASS_WEREWOLF) && !IS_IMMORTAL( ch ) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( IS_SET(ch->in_room->area->bits,WASTE) )
     {
       sta("The land shifts into something more recognizable.\n\r",ch->in_room->area);
       REMOVE_BIT(ch->in_room->area->bits,WASTE);
       return;
     }

  if ( !IS_IMMORTAL( ch ) && ch->gnosis[GCURRENT] < 40 )
     {
        stc("You must have 40 points of gnosis to use this.\n\r",ch);        
        return;
     }

  if ( IS_SET( ch->in_room->area->bits, ROOM_TOTAL_DARKNESS ) )
     {
        stc("You can't use this ability while the Wyrms have control over the land.\n\r",ch);        
        return;
     }
  
  if ( !IS_IMMORTAL( ch ) )
     ch->gnosis[GCURRENT] -= 40;

  sta("#oThe #yland #oaround you #Cblurs #oand #wshifts#o.#n\n\r",ch->in_room->area);

  SET_BIT(ch->in_room->area->bits,WASTE);
  REMOVE_BIT(ch->in_room->area->bits,UPDATEWASTE);
}

void do_ragesong(CHAR_DATA *ch,char *argument)
{
  CHAR_DATA *victim;
  char arg[MSL];

  if ( !IS_CLASS(ch,CLASS_WEREWOLF) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[GALLIARD] < 3 )
     {
        stc("You require level 3 in the gift of Galliard to use the Song of Rage.\n\r",ch);
        return;
     }

  argument=one_argument(argument,arg);

  if ( (victim=get_char_room(ch,arg))==NULL )
     {
        stc("They are not here.\n\r",ch);
        return;
     }

  if ( IS_NPC(victim) )
     {
        stc("Not on mobs!\n\r",ch);
        return;
     }

  if ( victim->rage > 0 )
     {
        stc("They are already raged.\n\r",ch);
        return;
     }

  WAIT_STATE(ch,14);
  act("You sing the Song of Rage to $N.\n\r",ch,NULL,victim,TO_CHAR);
  act("$n sings the Song of Rage to $N.",ch,NULL,victim,TO_NOTVICT);
  act("$n sings the Song of Rage to you.",ch,NULL,victim,TO_VICT);

  if ( IS_CLASS(victim,CLASS_WEREWOLF) )
     {
  	  act("You send them into a frenzied rage!",ch,NULL,victim,TO_CHAR);
	  act("$n sends $N into a frenzied rage!",ch,NULL,victim,TO_NOTVICT);
	  act("$n sends you into a frenzied rage!",ch,NULL,victim,TO_VICT);
	  victim->rage = 200;
	  do_werewolf(victim,"");
	  return;
     }
     else if(IS_CLASS(victim,CLASS_VAMPIRE))
     {
        act("You send them into a frenzied rage!",ch,NULL,victim,TO_CHAR);
        act("$n sends $N into a frenzied rage!",ch,NULL,victim,TO_NOTVICT);
        act("$n sends you into a frenzied rage!",ch,NULL,victim,TO_VICT);
	  victim->rage = 300;
        if (victim->beast > 0) do_beastlike(victim,"");
        return;
     }
     else {
	       act("You send them into a frenzied rage!",ch,NULL,victim,TO_CHAR);
	       act("$n sends $N into a frenzied rage!",ch,NULL,victim,TO_NOTVICT);
	       act("$n sends you into a frenzied rage!",ch,NULL,victim,TO_VICT);
 	       return;
          }
}

void do_bridgegate(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) return;
	
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
       {
          stc("Huh?\n\r",ch);
          return;
       }	

    if ( ch->gifts[GALLIARD] < 5 )
       {
          stc("You have not learned the Galliard gift of Bridge Walker.\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg);

    if ( ch->move < 250000 )
       {
          stc("You are too tired to execute this tehcnique.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
         stc("Who do you wish to bridge into combat with?\n\r", ch);
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

    ch->move -= 250000;
    stc("You open a minor Moon Bridge and step inside.\n\r", ch);
    act("$n opens a minor Moon Bridge and steps inside.", ch, NULL, NULL, TO_ROOM);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    ch->wait = 18;
} 

void do_lambent(CHAR_DATA *ch,char *argument)
{
  CHAR_DATA *vch;
 
  if ( !IS_CLASS(ch,CLASS_WEREWOLF) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->gnosis[GCURRENT] < 25 )
     {
        stc("You require 25 gnosis to use this power.\n\r",ch);
        return;
     }

  ch->gnosis[GCURRENT] -= 25;
  act("#wA #esilvery #wglow #yignights #waround $n#w's body.#n",ch,NULL,NULL,TO_ROOM);
  act("#wA #esilvery #wlight #yignights #waround your body.#n",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
  {
      if ( is_safe(ch,vch) || is_same_group(ch,vch) )
         continue;

      if ( number_percent() > 60 - devil_luck( ch, vch, 10 ) && !IS_SET( vch->more, MORE_BLIND ) )
         {
            SET_BIT( vch->more, MORE_BLIND );
            vch->blind_timer = 5;
         }

      spell_blindness( skill_lookup("blindness"), 5000, ch, vch );
  }

  WAIT_STATE(ch,8);
}
