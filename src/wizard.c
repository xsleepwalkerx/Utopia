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
int wyncount;

/*
 * Local functions.
 */
void    adv_spell_damage        args( ( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument) );
void    adv_spell_affect        args( ( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument) );
void    adv_spell_action        args( ( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument) );

const char * sphere_name [9] =
{
    "Correspondence",	"Life",
    "Prime",		"Entropy",
    "Mind",		"Spirit",
    "Forces",		"Matter",
    "Time"
};

bool	are_runes	args( ( OBJ_DATA *page ) );

bool are_runes( OBJ_DATA *page )
{
    if (page->value[1] + page->value[2] + page->value[3] < 1) return FALSE;
    return TRUE;
}

bool is_mage( CHAR_DATA *ch )
{
  if ( !IS_CLASS(ch, CLASS_WIZARD) && !IS_IMMORTAL(ch) )
  {
    stc("Huh?\n\r",ch);
    return FALSE;
  }

  return TRUE;
}    

void enhance_stat( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, int apply_bit, int bonuses, int affect_bit )
{
    AFFECT_DATA af;

    if ( ch != victim && (get_age(victim)-17) < 2 )
    {
       stc("But they are a newbie.\n\r",ch);
       return;
    }

    if ( IS_SET(affect_bit, AFF_WEBBED) && IS_AFFECTED(victim, AFF_WEBBED) )
       affect_bit -= AFF_WEBBED;
       else if ( IS_SET(affect_bit, AFF_WEBBED) && is_safe(ch,victim) )
               affect_bit -= AFF_WEBBED;

    if ( IS_SET(affect_bit, AFF_ETHEREAL) && victim->fight_timer > 0 )
       affect_bit -= AFF_ETHEREAL;

    if ( IS_SET(affect_bit, AFF_SHADOWPLANE) && victim->fight_timer > 0 )
       affect_bit -= AFF_SHADOWPLANE;

    if ( IS_SET(affect_bit, AFF_ETHEREAL) && ch->fight_timer > 0 )
       affect_bit -= AFF_ETHEREAL;

    if ( IS_SET(affect_bit, AFF_SHADOWPLANE) && ch->fight_timer > 0 )
       affect_bit -= AFF_SHADOWPLANE;

    if ( IS_SET(affect_bit, AFF_CHARM) && !IS_AFFECTED(victim, AFF_CHARM) )
    {
	if (victim->level <= 50 && IS_NPC(victim) )
	{
	    if ( victim->master )
	        stop_follower( victim );
            add_follower( victim, ch );
	}
	else
	{
	    send_to_char("The spell failed.\n\r",ch);
	    return;
	}
    }

    af.type      = sn;
    af.duration  = level;
    af.location  = apply_bit;
    af.modifier  = bonuses;
    af.bitvector = affect_bit;
    affect_to_char( victim, &af );
}

void do_spheres( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int disc,cost; 
  cost = 0;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( !is_mage(ch) )
    return;

  if ( arg1[0] == '\0' )
  {
    	stc("-=[**]_[**]=-=[**]_[**]=-=[**]_[**] Spheres [**]_[**]=-=[**]_[**]=-=[**]_[**]=-\n\r",ch);
	stc("[+]                                                                         [+]\n\r",ch);
	sprintf(buf,"[+]    Correspondence  (%s%s%s%s%s)       Life    (%s%s%s%s%s)      Forces  (%s%s%s%s%s)   [+]\n\r",
          ch->spheres[MCOR] >= 1 ? "*" : " ",
	  ch->spheres[MCOR] >= 2 ? "*" : " ",
          ch->spheres[MCOR] >= 3 ? "*" : " ",
          ch->spheres[MCOR] >= 4 ? "*" : " ",
          ch->spheres[MCOR] >= 5 ? "*" : " ",
          ch->spheres[MLIF] >= 1 ? "*" : " ",
          ch->spheres[MLIF] >= 2 ? "*" : " ",
          ch->spheres[MLIF] >= 3 ? "*" : " ",
          ch->spheres[MLIF] >= 4 ? "*" : " ",
          ch->spheres[MLIF] >= 5 ? "*" : " ",
          ch->spheres[MFOR] >= 1 ? "*" : " ",
          ch->spheres[MFOR] >= 2 ? "*" : " ",
          ch->spheres[MFOR] >= 3 ? "*" : " ",
          ch->spheres[MFOR] >= 4 ? "*" : " ",
          ch->spheres[MFOR] >= 5 ? "*" : " " );
	stc(buf,ch);
	sprintf(buf,"[+]       Entropy      (%s%s%s%s%s)       Mind    (%s%s%s%s%s)      Spirit  (%s%s%s%s%s)   [+]\n\r",
          ch->spheres[MENT] >= 1 ? "*" : " ",
          ch->spheres[MENT] >= 2 ? "*" : " ",
          ch->spheres[MENT] >= 3 ? "*" : " ",
          ch->spheres[MENT] >= 4 ? "*" : " ",
          ch->spheres[MENT] >= 5 ? "*" : " ",
          ch->spheres[MMIN] >= 1 ? "*" : " ",
          ch->spheres[MMIN] >= 2 ? "*" : " ",
          ch->spheres[MMIN] >= 3 ? "*" : " ",
          ch->spheres[MMIN] >= 4 ? "*" : " ",
          ch->spheres[MMIN] >= 5 ? "*" : " ",
          ch->spheres[MSPI] >= 1 ? "*" : " ",
          ch->spheres[MSPI] >= 2 ? "*" : " ",
          ch->spheres[MSPI] >= 3 ? "*" : " ",
          ch->spheres[MSPI] >= 4 ? "*" : " ",
          ch->spheres[MSPI] >= 5 ? "*" : " " );
	stc(buf,ch);
	sprintf(buf,"[+]        Prime       (%s%s%s%s%s)       Time    (%s%s%s%s%s)      Matter  (%s%s%s%s%s)   [+]\n\r",
          ch->spheres[MPRI] >= 1 ? "*" : " ",
          ch->spheres[MPRI] >= 2 ? "*" : " ",
          ch->spheres[MPRI] >= 3 ? "*" : " ",
          ch->spheres[MPRI] >= 4 ? "*" : " ",
          ch->spheres[MPRI] >= 5 ? "*" : " ",
          ch->spheres[MTIM] >= 1 ? "*" : " ",
          ch->spheres[MTIM] >= 2 ? "*" : " ",
          ch->spheres[MTIM] >= 3 ? "*" : " ",
          ch->spheres[MTIM] >= 4 ? "*" : " ",
          ch->spheres[MTIM] >= 5 ? "*" : " ",
          ch->spheres[MMAT] >= 1 ? "*" : " ",
          ch->spheres[MMAT] >= 2 ? "*" : " ",
          ch->spheres[MMAT] >= 3 ? "*" : " ",
          ch->spheres[MMAT] >= 4 ? "*" : " ",
          ch->spheres[MMAT] >= 5 ? "*" : " " );
	stc(buf,ch);
	stc("[+]                                                                         [+]\n\r",ch);
	stc("-=[**]_[**]=-=[**]_[**]=-=[**]_[**]---------[**]_[**]=-=[**]_[**]=-=[**]_[**]=-\n\r",ch);
        return;
  }

  if ( arg2[0] == '\0' )
  {
    if ( !str_prefix( arg1, "Life"  ) )
    {
	if ( ch->spheres[MLIF] == 0 )
        {
	  stc("You have no knowledge of the sphere of Life.\n\r",ch);
	  return;
	}

	if ( ch->spheres[MLIF] >= 1 )
        {
	  divide_to_char(ch);
	  centre_text("The Sphere of Life",ch);
	  divide_to_char(ch);
	  centre_text("Ho Tien Chi",ch);
	}
	if ( ch->spheres[MLIF] >= 2 )
          centre_text("Little Good Death",ch);
	if ( ch->spheres[MLIF] >= 3 )
	  centre_text("Better Body",ch);
	if ( ch->spheres[MLIF] >= 4 )
	  centre_text("Shapechange",ch);
	if ( ch->spheres[MLIF] >= 5 )
	  centre_text("Layhands",ch);
	divide_to_char(ch);
	return;
    } 

    if ( !str_prefix( arg1, "Prime" ) )
    {
	if ( ch->spheres[MPRI] == 0 )
	{
	  stc("You have no knowledge of the sphere of Prime.\n\r",ch);
	  return;
	}
	
	if ( ch->spheres[MPRI] >= 1 )
	{
	  centre_text("The Sphere of Prime",ch);
	  divide_to_char(ch);
	  centre_text("Quintessence",ch);
	}
	if ( ch->spheres[MPRI] >=2 )
	  centre_text("Enchant",ch);
	if ( ch->spheres[MPRI] >=3 )
	  centre_text("Rubbing of the Bones",ch);
	if ( ch->spheres[MPRI] >=4 )
	  centre_text("Flames of Purification",ch);
	if ( ch->spheres[MPRI] >=5 )
	  centre_text("Quintessence Blast",ch);
	divide_to_char(ch);
	return;
    }
  
    if ( !str_prefix( arg1, "Matter" ) )
    {
      if ( ch->spheres[MMAT] == 0 )
      {
	stc("You have no knowledge of the sphere of Matter.\n\r",ch);
	return;
      }

      if ( ch->spheres[MMAT] >= 1 )
      {
	divide_to_char(ch);
	centre_text("The Sphere of Matter",ch);
	divide_to_char(ch);
	centre_text("Analyze Substance",ch);
      }
      if ( ch->spheres[MMAT] >= 2 )
        centre_text("Alter State",ch);
      if ( ch->spheres[MMAT] >= 3 )
	centre_text("Alter Weight",ch);
      if ( ch->spheres[MMAT] >= 4 )
	centre_text("Enchant Armor",ch);
      if ( ch->spheres[MMAT] >= 5 )
	centre_text("Hover",ch);
      divide_to_char(ch);
      return;
    }

    if ( !str_prefix( arg1, "Spirit" ) )
    {
      if ( ch->spheres[MSPI] == 0 )
      {
	stc("You have no knowledge of the Sphere of Spirit.\n\r",ch);
	return;
      }

      if ( ch->spheres[MSPI] >= 1 )
      {
	divide_to_char(ch);
	centre_text("The Sphere of Spirit",ch);
	divide_to_char(ch);
	centre_text("Call Spirit",ch);
      }
      if ( ch->spheres[MSPI] >= 2 )
     	centre_text("Spirit Kiss",ch);
      if ( ch->spheres[MSPI] >= 3 )
	centre_text("Awaken the Inanimate",ch);
      if ( ch->spheres[MSPI] >= 4 )
	centre_text("Spirit Blast",ch);
      if ( ch->spheres[MSPI] >= 5 )
	centre_text("Breach the Gauntlet",ch);
      divide_to_char(ch);
      return;
    }

    if ( !str_prefix( arg1, "Entropy" ) )
    {
      if ( ch->spheres[MENT] == 0 )
      {
	stc("You have no knowledge of the Sphere of Entropy.\n\r",ch);
	return;
      }
      else
      {
	divide_to_char(ch);
	centre_text("The Sphere of Entropy",ch);
	divide_to_char(ch);
	sprintf(buf,"You have obtained a level %d knowledge of Entropy.",ch->spheres[MENT]);
	centre_text(buf,ch);
	sprintf(buf,"This multiplies your damage by %f.",(1+(ch->spheres[MENT]*.1)));
	centre_text(buf,ch);
	sprintf(buf,"This divides your damage taken by 1.5.");
	centre_text(buf,ch);
	sprintf(buf,"This increases the level of your spells by %d.",ch->spheres[MENT]*5);
	centre_text(buf,ch);
	divide_to_char(ch);
	return;
      }
    }

    if ( !str_prefix( arg1, "Correspondence" ) )
    {
      if ( ch->spheres[MCOR] == 0 )
      {
        stc("You have no knowledge of the sphere of Correspondence.\n\r",ch);
        return;
      }

      if ( ch->spheres[MCOR] > 0 )
      {
        centre_text("The Sphere of Correspondence",ch);
        divide_to_char(ch);
        centre_text("Perception",ch);
      }

      if ( ch->spheres[MCOR] > 1 )
      {
        centre_text("Gateway to the Non-Living",ch);
      }

      if ( ch->spheres[MCOR] > 2 )
      {
        centre_text("Gateway to the Living",ch);
      }

      if ( ch->spheres[MCOR] > 3 )
      {
        centre_text("The Grasp of Elminster",ch);
        centre_text("The Eternal Gateway",ch);
      }

      if ( ch->spheres[MCOR] > 4 )
      {
        centre_text("Polyappearance",ch);
      }
      divide_to_char(ch);
      return;
    }

    else if ( !str_cmp( arg1, "Mind" ) )
    {
      if ( ch->spheres[MMIN] == 0 )
      {
        stc(" You have no knowledge of the Sphere of the Mind.\n\r",ch);
        return;
      }

      if ( ch->spheres[MMIN] > 0 )
      {
	divide_to_char(ch);
	centre_text("The Sphere of Mind",ch);
	divide_to_char(ch);
	centre_text("Shield",ch);
      }

      return;
    }

    else if ( !str_prefix( arg1, "Forces" ) )
    {
      if ( ch->spheres[MFOR] == 0 )
      {
        stc("You have no knowledge of the Sphere of Forces.\n\r",ch);
        return;
      }
  
      if ( ch->spheres[MFOR] > 0 )
      {
        centre_text("The Sphere of Forces",ch);
        centre_text("Discharge Static",ch);
      }
      if ( ch->spheres[MFOR] > 1 )
      {
        centre_text("Telekinesis",ch);
      }
      if ( ch->spheres[MFOR] > 2 )
      {
        centre_text("Call Lightning",ch);
        centre_text("Control Weather",ch);
      }
      if ( ch->spheres[MFOR] > 3 )
      {
        centre_text("Embracing the Earth Mother",ch);
      }
      if ( ch->spheres[MFOR] > 4 )
      {
        centre_text("Tempest Shards",ch);
      }
      return;
    }

    else if ( !str_cmp( arg1, "Time" ) )
    {
      if ( ch->spheres[MTIM] == 0 )
      {
        stc("You have no knowledge of the Sphere of Time.\n\r",ch);
        return;
      }
      else
      {
        sprintf(buf,"You have a level %d knowledge of Time.\n\r",ch->spheres[MTIM]);
	stc(buf,ch);
	return;
      }    
      return;
    }      


  }


  if ( !str_cmp( arg2, "learn" ) )
  {
         if ( !str_prefix( arg1, "correspondence" ) )
      disc = MCOR;
    else if ( !str_prefix( arg1, "life" ) )
      disc = MLIF;
    else if ( !str_prefix( arg1, "prime" ) )
      disc = MPRI;
    else if ( !str_prefix( arg1, "entropy" ) )
      disc = MENT;
    else if ( !str_prefix( arg1, "mind" ) )
      disc = MMIN;
    else if ( !str_prefix( arg1, "spirit" ) )
      disc = MSPI;
    else if ( !str_prefix( arg1, "forces" ) )
      disc = MFOR;
    else if ( !str_prefix( arg1, "matter" ) )
      disc = MMAT;
    else if ( !str_prefix( arg1, "time" ) )
      disc = MTIM;
    else
    {
      stc("You must select from the following spheres:\n\r  Correspondence, Life, Prime, Entropy, Mind, Spirit, Forces, Matter, Time.\n\r",ch);
      return;
    }
  }
  else
  {
    do_spheres(ch,"");
    return;
  }

  cost = ( ( ch->spheres[disc] + 1 ) * 10 );

  if ( ch->spheres[disc] >= 5 )
  {
    sprintf(buf, "You have already learned all levels of the %s sphere.\n\r",sphere_name[disc] );
    stc(buf,ch);
    return;
  }

  if ( ch->practice < cost )
  {
    sprintf(buf, "You need %d primal to obtain a new level of understanding of the %s sphere.\n\r", cost, sphere_name[disc] );
    stc(buf,ch);
    return;
  }

  ch->practice -= cost;
  ch->spheres[disc]++;
  sprintf(buf, "Your knowledge of the %s sphere increases.\n\r",sphere_name[disc] );
  stc(buf,ch);
  return;

}

void do_perception( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MCOR] < 1 )
  {
    stc("You must obtain level one Correspondence to enable your higher Perception.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_PERCEPTION) )
  {
    stc("Your higher perception fades.\n\r",ch);
    REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    REMOVE_BIT(ch->mflags,MAGE_PERCEPTION);
    return;
  }
  else
  {
    stc("Your perception soars to new levels.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    SET_BIT(ch->act, PLR_HOLYLIGHT);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    SET_BIT(ch->mflags,MAGE_PERCEPTION);
    return;
  }
}

void creategate( CHAR_DATA *ch, int inroom, int toroom, bool perm )
{
  OBJ_DATA *in_gate;
  OBJ_DATA *to_gate;
  ROOM_INDEX_DATA *pIn;
  ROOM_INDEX_DATA *pTo;

  pIn = get_room_index( inroom );
  pTo = get_room_index( toroom );

  in_gate = create_object( get_obj_index( OBJ_VNUM_WGATE ), 0 );
  obj_to_room( in_gate, pIn );
  in_gate->value[0] = toroom;
  in_gate->value[1] = MAGE_INROOM_GATE;
  in_gate->value[3] = inroom;
  if ( !perm )
  in_gate->timer = ch->spheres[MCOR];
  in_gate->item_type = ITEM_WGATE;

  to_gate = create_object( get_obj_index( OBJ_VNUM_WGATE ), 0 );
  obj_to_room( to_gate, pTo );
  to_gate->value[0] = inroom;
  to_gate->value[1] = MAGE_TOROOM_GATE;
  to_gate->value[3] = toroom;
  if ( !perm )
  to_gate->timer = ch->spheres[MCOR];
  to_gate->item_type = ITEM_WGATE;
}

void do_polyappear( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MCOR] < 5 )
  {
    stc("You must become a master of the sphere of Correspondence to use Polyappearance.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_POLYAPPEAR) )
  {
    stc("Your body phases and reforms as one being.\n\r",ch);
    act("$n's many forms phase and reform as one being.",ch,NULL,NULL,TO_ROOM);
    REMOVE_BIT(ch->mflags, MAGE_POLYAPPEAR);
    return;
  }

  else
  {
    stc("Your body phases and splits into multiple figures.\n\r",ch);
    act("$n's body phases and splits into multiple figures.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->mflags, MAGE_POLYAPPEAR);
    return;
  }
}

void do_discharge( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *vch = NULL;
  CHAR_DATA *vch_next = NULL;
  char arg1[MAX_INPUT_LENGTH];
  int damage;

  argument = one_argument( argument, arg1 );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( arg1[0] == '\0' )
  {
    stc("Who do you wish to strike down?\n\r",ch);
    return;
  }

  if ( ch->spheres[MFOR] < 1 )
  {
    stc("You must have a level one knowledge of the Forces to discharge Static Electricity.\n\r",ch);
    return;
  }

  if ( str_cmp(arg1, "all" ) )
  {
     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg1[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to blast?\n\r", ch );
        return;
     }
    
     if ( ch->position == POS_STANDING || arg1[0] != '\0' )
     {
        if ( ( victim = get_char_area( ch, arg1 ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
     }

    WAIT_STATE(ch,24);
    damage = 300000;

    if ( weather_info.sky == SKY_LIGHTNING )
      damage += number_range(200000,300000);

    if ( weather_info.sky == SKY_RAINING )
      damage += number_range(50000,100000);

    ch->ele_attack = ATTACK_THUNDER;
    mage_damage(ch, victim, damage, "electricity burst", MAGEDAM_ELECTRIC);
  }

  else
  {
    damage = 300000;;

    if ( weather_info.sky == SKY_LIGHTNING )
      damage += number_range(100000,350000);

    if ( weather_info.sky == SKY_RAINING )
      damage += number_range(90000,125000);

    WAIT_STATE(ch,16);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;
       
       ch->ele_attack = ATTACK_THUNDER;

       if ( vch != ch )
         mage_damage( ch, vch, damage, "electricity burst", MAGEDAM_ELECTRIC );
    }
  }
}

void do_control_weather( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) )

  if ( ch->spheres[MFOR] < 3 )
  {
    stc("You need a level three knowledge of Forces to Control the Weather.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("How do you want to change the weather?\n     Syntax: controlweather <rain,lightning,clear>\n\r",ch);
    return;
  }

  WAIT_STATE(ch, 16);
  if ( !str_cmp(arg, "rain" ) )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies blacken and rain begins to fall.\n\r",ch);
    weather_info.sky = SKY_RAINING;
  }
  else if ( !str_cmp(arg, "lightning") )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies blacken.\n\r",ch);
    stc("Lightning streaks across the sky.\n\r",ch);
    weather_info.sky = SKY_LIGHTNING;
  }
  else if ( !str_cmp(arg, "clear") )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies slowly clear up.\n\r",ch);
    weather_info.sky = SKY_CLOUDLESS;
  }
  else
  {
    stc("Invalid option.\n   Syntax: controlweather <rain,lightning,clear>\n\r",ch);
    return;
  }
}

void do_earthembrace( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;
 
  if ( ch->spheres[MFOR] < 4 )
  {
    stc("You must obtain a level four knowledge of the Forces to use Embrace of the Earth Mother.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' && IS_CLASS(ch, CLASS_WIZARD))
  {
    stc("Whom do you wish to have Gaia Embrace?\n\r",ch);
    return;
  }
  else if ( arg[0] == '\0' )
  {
    stc( "Whom do you wish to inspire great fear in?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room(ch, arg) ) == NULL )
  {
    stc("You were unable to locate your victim.\n\r",ch);
    return;
  }

  if ( !IS_SET(victim->mflags, MAGE_EMBRACED1) )
     {
        act("You call upon the forces of the Earth to embrace $N!",ch,NULL,victim,TO_CHAR);
        act("$n begins to chant mysteriously.",ch,NULL,victim,TO_ROOM);
        act("The ground shakes and rises to embrace $N!",ch,NULL,victim,TO_NOTVICT);
        act("The ground under your feet starts to surround your body.",ch,NULL,victim,TO_VICT);
        SET_BIT(victim->mflags, MAGE_EMBRACED1);
        WAIT_STATE(ch, 16);
     }
     else{
            act("You call upon the forces of the Earth to release $N!",ch,NULL,victim,TO_CHAR);
            act("$n begins to chant mysteriously.",ch,NULL,victim,TO_ROOM);
            act("The ground shakes and rises to release $N!",ch,NULL,victim,TO_NOTVICT);
            act("The ground under your feet starts to move away from your body.",ch,NULL,victim,TO_VICT);
            REMOVE_BIT(victim->mflags, MAGE_EMBRACED1);
         }
}

void do_tempest( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) || ( ch->clannum != 1 && ch->clannum != 6 ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 100 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to shatter?\n\r", ch );
        return;
     }
    
  if ( ch->position == POS_STANDING || arg[0] != '\0' )
     {
        if ( ( victim = get_char_area( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
     }

  if ( is_safe(ch, victim) )
     return;

  ch->wait = 12;
  SET_BIT( ch->arena, AFF2_DM );
  subtract_stock( ch, 100 );

  chroom = ch->in_room;
  victimroom = victim->in_room;

  char_from_room(ch);
  char_to_room(ch, victimroom);
  act("#CYou #wcall upon the Forces of Nature to #yobliterate #P$N#w.#n",ch,NULL,victim,TO_CHAR); 
  act("#wThe #Csky #wturns an #eominous #wwhite...#n",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) ) 
        continue;

     if ( !is_safe( ch, vch ) )
        {
           act("#wSeconds later, #Rdeadly #wshards of #Cice #wshoot out of the sky, piercing #Pyour #wskin.#n",vch,NULL,NULL,TO_CHAR);
           act("#P$n #wscreams loudly as the #Cice #wshards pierces $s body.#n",vch,NULL,NULL,TO_ROOM);
           ch->attack_type = ATTACK_MAGICAL;
           ch->attack_var = PIERCE;
           ch->attack_focus = AERIAL;
           ch->ele_attack = ATTACK_ICE;
           hurt_person( ch, vch, 1000000, TRUE );
        }
  }

  char_from_room(ch);
  char_to_room(ch, chroom);
}

bool is_immune( CHAR_DATA *ch, int type )
{
  if ( IS_SET(ch->imms[IMMUNITY], type) )
    return TRUE;
  else
    return FALSE;
}

bool is_resistant( CHAR_DATA *ch, int type )
{
  if ( IS_SET(ch->imms[RESISTANCE], type) )
    return TRUE;
  else
    return FALSE;
}

bool is_vulnerable( CHAR_DATA *ch, int type )
{
  if ( IS_SET(ch->imms[VULNERABLE], type ) )
    return TRUE;
  else
    return FALSE;
}

int mage_immunity( CHAR_DATA*ch, int dam, const char *msg, int type )
{
  if ( IS_NPC(ch) ) return dam;

  if ( is_immune( ch, type ) )
    dam = 0;

  else if ( is_resistant( ch, type ) )
    dam /= 2;

  else if ( is_vulnerable( ch, type ) )
    dam *= 2;

  return dam;
}

void mage_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type )
{
  if ( victim->position == POS_DEAD )
    return;

  if ( victim != ch )
  {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if ( is_safe( ch, victim ))
            return;

        if ( victim->position > POS_STUNNED )
        {
            if ( victim->fighting == NULL )
                set_fighting( victim, ch );
            victim->position = POS_FIGHTING;
        }

        if ( victim->position > POS_STUNNED )
        {
            if ( ch->fighting == NULL )
                set_fighting( ch, victim );
        }

        /*
         * More charm stuff.
         */
        if ( victim->master == ch )
            stop_follower( victim );

        if ( dam < 0 )
            dam = 0;

    mage_message( ch, victim, dam, message, type );
    if( IS_NPC(victim))
	hurt_person(ch,victim,dam,FALSE);
    else if( !is_safe(ch, victim) || victim->fight_timer >0 )
	hurt_person(ch,victim,dam,FALSE);

    return;
   }
}

void mage_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type)
{
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  char damm[MAX_STRING_LENGTH];
  char dam2[MAX_STRING_LENGTH];
  const char *chm;
  const char *victm;

  if( number_percent() < 20 ) add_quint( ch, 1 );

  if ( dam == 0 )       {  chm = " miss";        victm = " misses";     }
  else if ( dam <= 50 ) {  chm = " graze";       victm = " grazes";     }
  else if ( dam <= 100) {  chm = " wound";       victm = " wounds";     }
  else if ( dam <= 200) {  chm = " mutilate";    victm = " mutilates";  }
  else if ( dam <= 300) {  chm = " massacre";    victm = " massacres";  }
  else if ( dam <= 400) {  chm = " eviscerate";  victm = " eviscerates";}
  else if ( dam <= 500) {  chm = " annihilate";  victm = " annihilates";}
  else if ( dam <= 600) {  chm = " obliterate";  victm = " obliterates";}
  else if ( dam <= 800) {  chm = " dissipate";   victm = " dissipates"; }
  else if ( dam <= 1000){  chm = " vaporize";    victm = " vaporizes";  }
  else                  {  chm = " liquify";     victm = " liquifies";  }

  if ( !IS_NPC(victim) && ( victim->hit - dam ) < 0 )
  {
    if ( type == MAGEDAM_ELECTRIC )
    {
      act("Your electric shock causes $N to spasm violently and vomit blood.",ch,NULL,victim,TO_CHAR);
      act("$n's electric shock causes $N to spasm violently and vomit blood.",ch,NULL,victim,TO_NOTVICT);
      act("$n's electric shock causes your body to spasm violently as you vomit blood.",ch,NULL,victim,TO_VICT);
      make_part(victim,"blood");
    }

    else if ( type == MAGEDAM_ICESTORM )
    {
      act("Your ice shards tear $N's flesh from $S body and leave $M mortally wounded.",ch,NULL,victim,TO_CHAR);
      act("$n's ice shards tear $N's flesh from $S body and leave $M mortally wounded.",ch,NULL,victim,TO_NOTVICT);
      act("$n's ice shards tear your flesh from your body and leave you mortally wounded.",ch,NULL,victim,TO_VICT);
      make_part(victim,"blood");
    }

    else
    {
      act("Your magical energy tears $N's body asunder.",ch,NULL,victim,TO_CHAR);
      act("$n's magical energies tear $N's body asunder.",ch,NULL,victim,TO_NOTVICT);
      act("$n's magical energies tear your body asunder.",ch,NULL,victim,TO_VICT);
      make_part(victim,"blood");
    }
  }

  if ( dam == 0 && is_immune(ch, type))
  {
    sprintf(buf1, "$N is unaffected by your %s.", message);
    sprintf(buf2, "$N is unaffected by $n's %s.", message);
    sprintf(buf3, "You are unaffected by $n's %s.", message);
  }

  if ( dam == 0 && !is_immune(ch, type))
  {
    sprintf(buf1, "You miss $N with your %s.", message);
    sprintf(buf2, "$n misses $N with $s %s.", message);
    sprintf(buf3, "$n misses you with $s %s.", message);
  }

  else
  {
    if ( ch->spheres[MFOR] > 1 )
    {
      sprintf(damm,"%d",dam);
      ADD_COLOUR(ch, damm, D_RED);
      sprintf(dam2,".[%s]",damm);
    }
    else
      sprintf(dam2,".");

    sprintf(buf1, "You%s $N with your %s%s", chm, message,dam2);
    sprintf(buf2, "$n%s $N with $s %s.", victm, message);
    sprintf(buf3, "$n%s you with $s %s.", victm, message);
  }
 
  act(buf1,ch,NULL,victim,TO_CHAR);
  act(buf2,ch,NULL,victim,TO_NOTVICT);
  act(buf3,ch,NULL,victim,TO_VICT);
}
  
void do_quintessence( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 1 )
  {
    stc("You cannot store Quintessence until you have learned level 1 Prime.\n\r",ch);
    return;
  }

  stc("----------------------------------------------------------------------------\n\r",ch);
  stc("                             Quintessence\n\r", ch);
  stc("----------------------------------------------------------------------------\n\r",ch);
  sprintf(buf,"*   You have %d points of Quintessence stored within your body.               *\n\r",ch->quint[BODY]);
  stc(buf,ch);
  sprintf(buf,"*   You have %d points of Quintessence stored within your Avatar.             *\n\r",ch->quint[AVATAR]);
  stc(buf,ch);
  stc("----------------------------------------------------------------------------\n\r",ch);
}

int check_quint( CHAR_DATA *ch )
{
  if ( ch->spheres[MPRI] < 3 )
    return ch->quint[AVATAR];
  else if ( ch->spheres[MPRI] >= 3 )
    return ( ch->quint[AVATAR] + ch->quint[BODY] );
  else
    return ch->quint[AVATAR];
}

void subtract_quint( CHAR_DATA *ch, int number )
{
  ch->quint[MTOTAL] = check_quint(ch);   // Find Total Quint of Char
  ch->quint[MTOTAL] -= number;           // Subtract from Max
  if ( ch->quint[MTOTAL] > 5 )
  {
    ch->quint[AVATAR] = 5;
    ch->quint[MTOTAL] -= 5;
      if ( ch->quint[MTOTAL] > 5 )
      {
        ch->quint[BODY] = 5;
        ch->quint[MTOTAL] = 0;
      }
      else
      {
        ch->quint[BODY] = ch->quint[MTOTAL];
        ch->quint[MTOTAL] = 0;
      }
  }
  else
  {
    ch->quint[AVATAR] = ch->quint[MTOTAL]; // Move all quint to Avatar
    ch->quint[BODY] = 0;                   // Drain the body
  }
  return;
}

void add_quint( CHAR_DATA *ch, int number )
{
  ch->quint[MTOTAL] = check_quint(ch);
  ch->quint[MTOTAL] += number;
  if ( ch->quint[MTOTAL] > 10 )
  {
    ch->quint[AVATAR] = 10;
    ch->quint[MTOTAL] -= 10;
    if ( ch->quint[MTOTAL] > 10 )
    {
      ch->quint[BODY] = 10;
      ch->quint[MTOTAL] = 0;
    }
    else
    {
      ch->quint[BODY] = ch->quint[MTOTAL];
      ch->quint[MTOTAL] = 0;
    }
  }
  else
  {
    ch->quint[AVATAR] = ch->quint[MTOTAL];
    ch->quint[BODY]   = 0;
  }
  return;
}

void do_enchant( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 2 )
  {
    stc("You must obtain a level two knowledge of Prime to use Enchant Weapon.\n\r",ch);
    return;
  }

  if ( ch->generation > 2 )
     {
        stc( "You are currently too weak to manipulate this object.\n\r", ch );
        return;
     }
 
  if ( arg[0] == '\0' )
  {
    stc("What object do you wish to enchant?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that object.\n\r",ch);
    return;
  }

  if ( obj->item_type != ITEM_WEAPON )
  {
    act("$p is not a weapon.",ch,obj,NULL,TO_CHAR);
    return;
  }

  if ( check_quint(ch) < 1 )
  {
    stc("You need one point of quintessence available to use this power.\n\r",ch);
    return;
  }

  if ( IS_SET(obj->extra_flags, ITEM_MENCHANT) )
  {
    act("$p is too powerful for your quintessence.",ch,obj,NULL,TO_CHAR);
    return;
  }

  if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
  {
    act("$p is too powerful for your quintessence.",ch,obj,NULL,TO_CHAR);
    return;
  }

  obj->value[1] += (ch->spheres[MPRI]*10000);
  obj->value[2] += (ch->spheres[MPRI]*10000);

  SET_BIT(obj->extra_flags, ITEM_MENCHANT);

  act("$p glows radiantly as you infuse your quintessence into it.",ch,obj,NULL,TO_CHAR);
  act("$n's weapon, $p, glows brilliantly.",ch,obj,NULL,TO_ROOM);
  subtract_quint(ch, 1);
}

void do_qblast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 5 )
  {
    stc("You must obtain level five Prime to use Quintessence Blast.\n\r",ch);
    return;
  }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to blast?\n\r", ch );
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

  if ( check_quint(ch) < 4 )
  {
    stc("You must have four points of quintessence to use this power.\n\r",ch);
    return;
  }

  subtract_quint(ch, 4);
  dam = ch->spheres[MPRI] * 150000;
  mage_damage(ch,victim,dam,"quintessence blast",MAGEDAM_QUINT);
  WAIT_STATE(ch, 18);
}

void do_dimmak( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MENT] < 1 )
  {
    stc("You must obtain level one entropy to use Dim Mak.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MFLAGS_DIMMAK) )
  {
    REMOVE_BIT(ch->mflags, MFLAGS_DIMMAK);
    stc("You no longer focus your attacks on your opponent's weakness.\n\r",ch);
    return;
  }

  else
  {
    SET_BIT(ch->mflags, MFLAGS_DIMMAK);
    stc("You now focus your attacks on your opponent's weakness.\n\r",ch);
    return;
  }
}

void do_hotienchi( CHAR_DATA *ch, char *argument )
{
  int i,test=0;
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch)) return;

  if ( ch->spheres[MLIF] < 1 )
  {
    stc("You must obtain level one Life to use Ho Tien Chi.\n\r",ch);
    return;
  }

  for( i=0;i<7;i++)
  {
    if ( ch->loc_hp[i] > 0 )
      test++;
  }

  if ( ch->hit == ch->max_hit && ch->mana == ch->max_mana && ch->move == ch->max_move && test == 0 )
  {
    stc("You are in perfect condition!\n\r",ch);
    return;
  }

  if (ch->loc_hp[6] > 0)
  {
    stc("Your wounds close up and stop bleeding.\n\r",ch);
    ch->loc_hp[6] = 0;
  }
  
  if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
  {
    stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",ch);
    ch->loc_hp[0] = 0;
    ch->loc_hp[1] = 0;
    ch->loc_hp[2] = 0;
    ch->loc_hp[3] = 0;
    ch->loc_hp[4] = 0;
    ch->loc_hp[5] = 0;
  }

  stc("A warm feeling spreads through your body.\n\r",ch);
  ch->hit += (ch->spheres[MLIF]*2000);
  ch->mana += (ch->spheres[MLIF]*2000);
  ch->move += (ch->spheres[MLIF]*2000);
  if ( ch->hit > ch->max_hit )
    ch->hit = ch->max_hit;
  if ( ch->mana > ch->max_mana )
    ch->mana = ch->max_mana;
  if ( ch->move > ch->max_move )
    ch->move = ch->max_move;
  WAIT_STATE(ch,24);
}

void do_littledeath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MLIF] < 2 )
  {
    stc("You must obtain level two Life to use Little Good Death.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("You cannot find your victim here.\n\r",ch);
    return;
  }

  if ( !IS_NPC(victim) )
  {
    stc("A human being is too complex for this spell.\n\r",ch);
    return;
  }

  if ( get_kingdom_num( victim ) > 0 )
     return;

  if ( IS_SET(victim->extra, EXTRA_ZOMBIE) ) 
  {
    stc("But they are already dead magi.\n\r",ch);
    return;
  }

  WAIT_STATE(ch, 24);
  act("You mentally grasp $N's pattern and tear $M to shreds.",ch,NULL,victim,TO_CHAR);
  act("$n's eyes turn black.\n$N's body is torn to shreds before your eyes!",ch,NULL,victim,TO_ROOM);
  raw_kill(victim);
}

void do_betterbody( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MLIF] < 3 )
  {
    stc("You must obtain level three Life to use Better Body.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_BETTERBODY) )
  {
    stc("Your body shrinks to its true form.\n\r",ch);
    REMOVE_BIT(ch->mflags, MAGE_BETTERBODY);
    ch->pcdata->perm_str -= 4;
    ch->pcdata->perm_con -= 2;
    ch->pcdata->perm_dex -= 3;
    return;
  }

  else
  {
    stc("Your body contorts and grows to supernatural size.\n\r",ch);
    SET_BIT(ch->mflags, MAGE_BETTERBODY);
    ch->pcdata->perm_str += 4;
    ch->pcdata->perm_dex += 3;
    ch->pcdata->perm_con += 2;
    return;
  }
}

void do_shapechange( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int choice = 0;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MLIF] < 4 )
  {
    stc("You must obtain level four Life to Change Shape.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    divide_to_char(ch);
    centre_text("Forms",ch);
    divide_to_char(ch);
    stc("    [ 0 ]  Human\n",ch);
    stc("    [ 1 ]  Tiger\n",ch);
    stc("    [ 2 ]  Lion\n",ch);
    stc("    [ 3 ]  Raven\n",ch);
    stc("    [ 4 ]  Frog\n",ch);
    stc("    [ 5 ]  Dog\n",ch);
    stc("    [ 6 ]  Hawk\n",ch);
    stc("    [ 7 ]  Mouse\n",ch);
    divide_to_char(ch);
    return;
  }

  choice = is_number( arg ) ? atoi( arg ) : -1;

  if ( choice == 1 )
  {
    sprintf(buf, "#w%s the #Rf#yerociou#Rs #wtiger#n",ch->name);
    stc("You transform into a ferocious tiger.\n\r",ch);
    act("$n transforms into a tiger.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 2 )
  {
    sprintf(buf, "#r%s the #olion#n",ch->name);
    stc("You transform into a lion.\n\r",ch);
    act("$n transforms into a lion.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 3 )
  {
    sprintf(buf, "#c%s the #Cr#Lave#Cn#n",ch->name);
    stc("You transform into a raven.\n\r",ch);
    act("$n transforms into a raven.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 4 )
  {
    sprintf(buf, "#L%s the #gf#Gr#go#ng",ch->name);
    stc("You transform into a frog.\n\r",ch);
    act("$n transforms into a frog.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 5 )
  {
    sprintf(buf, "#y%s the #odog#n",ch->name);
    stc("You transform into a small dog.\n\r",ch);
    act("$n transforms into a dog.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 6 )
  {
    sprintf(buf, "#c%s the #ph#oa#pw#ok#n",ch->name);
    stc("You transform into a hawk.\n\r",ch);
    act("$n transforms into a hawk.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 7 )
  {
    sprintf(buf, "#e%s #wthe #emou#ose#n",ch->name);
    stc("You transform into a white mouse.\n\r",ch);
    act("$n transforms into a white mouse.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 0 )
  {
    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(ch->mflags, MAGE_SHAPED);
      free_string(ch->morph);
      ch->morph = str_dup("");
      stc("You revert to your human form.\n\r",ch);
      act("$n reverts to $s human form.",ch,NULL,NULL,TO_ROOM);
      return;
    }
    else
    {
      stc("You aren't even shapechanged!\n\r",ch);
      return;
    }
  }
  else
  {
    stc("That's not a choice!\n\r",ch);
    do_shapechange(ch,"");
    return;
  }

  SET_BIT(ch->affected_by, AFF_POLYMORPH);
  SET_BIT(ch->mflags, MAGE_SHAPED);
  clear_stats(ch);
  free_string(ch->morph);
  ch->morph = str_dup(buf);
}  

void do_analyze( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 1 )
  {
    stc("You must obtain level one Matter to use Analyze Substance.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Analyze which object?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that item.\n\r",ch);
    return;
  }
  
  act("You analyze the substance pattern of $p.",ch,obj,NULL,TO_CHAR);
  (*skill_table[skill_lookup("identify")].spell_fun) (skill_lookup("identify"), 50, ch, obj );
}

void do_alterstate( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 2 )
  {
    stc("You must obtain level two Matter to use Alter State.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' && ch->fighting == NULL )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }
  
  if ( ch->fighting == NULL )
  {
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      stc("You are unable to find them in this room.\n\r",ch);
      return;
    }
  }
  else
    victim = ch->fighting;

  if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
     if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL )
        if ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL )
           if ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL )
              {
                 stc("They aren't using any weapons!\n\r",ch);
                 return;
              }

  if ( IS_CLASS(victim, CLASS_PALADIN) || IS_CLASS(victim, CLASS_SORCEROR) || IS_CLASS(victim, CLASS_DARK_KNIGHT) )
     {
         stc("Their grip is too strong.\n\r",ch);
         return;
     }

  obj_from_char( obj );
  obj_to_char( obj, victim );
  act("You alter the state of $N's weapon, causing $M to drop it.",ch,NULL,victim,TO_CHAR);
  act("$n's eyes glow dark black.",ch,NULL,NULL,TO_ROOM);
  act("$p shakes violently, causing you to drop it.",ch,obj,victim,TO_VICT);
  WAIT_STATE(ch, 24);
}

void do_alterweight( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 3 )
  {
    stc("You must obtain level three Matter to use Alter Weight.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Which object's weight do you want to lower?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that object.\n\r",ch);
    return;
  }

  if ( obj->weight <= 1 )
  {
    stc("That object is already as light as possible!\n\r",ch);
    return;
  }

  obj->weight = 1;
  act("You rearrange $p's pattern, lowering it's weight.",ch,obj,NULL,TO_CHAR);
  act("$n's eyes glow bright green.",ch,NULL,NULL,TO_ROOM);
  WAIT_STATE(ch, 8);
}

void do_enchantarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  AFFECT_DATA *paf;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 4 )
  {
    stc("You must obtain level four Matter to use Enchant Armor.\n\r",ch);
    return;
  }

  if ( ch->generation > 2 )
     {
        stc( "You are currently too weak to manipulate this object.\n\r", ch );
        return;
     }

  if ( arg[0] == '\0' )
  {
    stc("Which piece of armor do you want to enchant?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that object.\n\r",ch);
    return;
  }

  if ( IS_SET(obj->quest, QUEST_MAGEENCHANT) )
  {
    stc("This object has already been enchanted.\n\r",ch);
    return;
  }

  if ( obj->item_type != ITEM_ARMOR )
  {
    stc("This item isn't a piece of armor.\n\r",ch);
    return;
  }
  
  if ( check_quint( ch ) < 1 )
  {
    stc("You need one point of quintessence to Enchant Armor.\n\r",ch);
    return;
  }

  act("You lay your hands on $p and rearrange its pattern.",ch,obj,NULL,TO_CHAR);
  act("$n's eyes glow bright blue.\n$n lays $s hands on $p.\n$p glows blue.",ch,obj,NULL,TO_ROOM);
  SET_BIT(obj->quest, QUEST_MAGEENCHANT);
  

  if ( affect_free == NULL )
  {
      paf             = alloc_perm( sizeof(*paf) );
  }
  else
  {
      paf             = affect_free;
      affect_free     = affect_free->next;
  }

  paf->type           = skill_lookup("enchant weapon");
  paf->duration       = -1;
  paf->location       = APPLY_HITROLL;
  paf->modifier       = 10000;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;

  if ( affect_free == NULL )
  {
      paf             = alloc_perm( sizeof(*paf) );
  }
  else
  {
      paf             = affect_free;
      affect_free     = affect_free->next;
  }

  paf->type           = skill_lookup("enchant weapon");
  paf->duration       = -1;
  paf->location       = APPLY_DAMROLL;
  paf->modifier       = 10000;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;

  if ( affect_free == NULL )
     {
         paf             = alloc_perm( sizeof(*paf) );
     }
     else
     {
         paf             = affect_free;
         affect_free     = affect_free->next;
     }

  paf->type           = skill_lookup("enchant weapon");
  paf->duration       = -1;
  paf->location       = APPLY_AC;
  paf->modifier       = -5000;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;

  WAIT_STATE(ch, 16);
  subtract_quint(ch, 1);
}

void do_hover( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 5 )
  {
    stc("You must obtain level 5 matter to use Hoverwalk.\n\r",ch);
    return;
  }

  if ( IS_AFFECTED(ch, AFF_FLYING) )
  {
    stc("You slowly float to the ground as your air cushion dissipates.\n\r",ch);
    act("$n slowly floats to the ground.",ch,NULL,NULL,TO_ROOM);
    REMOVE_BIT(ch->affected_by, AFF_FLYING);
    return;
  }
  else
  {
    stc("You restructure the pattern of the air around your body.\n\rYou slowly float into the air.\n\r",ch);
    act("$n slowly floats into the air.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->affected_by, AFF_FLYING);
    return;
  }
}

void do_mshadow( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;
  
  if ( ch->spheres[MMIN] < 5 )
  {
    stc("You must obtain level five Mind to use Untether.\n\r",ch);
    return;
  }

  if ( ch->fight_timer > 0 )
  {
    stc("Not with a fight timer.\n\r",ch);
    return;
  }

  if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
  {
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
}

void do_callspirit( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;

  if ( IS_NPC(ch) ) return;
 
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MSPI] < 1 )
  {
    stc("You must obtain level one Spirit to Call Spirits.\n\r",ch);
    return;
  }
  
  if ( ch->pcdata->followers > 2 )
  {
    stc("Nothing seems to happen.\n\r",ch);
    return;
  }

  stc("You chant an arcane spell and conjure a spirit.\n\r",ch);
  act("$n chants loudly in a foreign language.",ch,NULL,NULL,TO_ROOM);

    ch->pcdata->followers++;
    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->name = str_dup("spirit");
    victim->short_descr = str_dup("A wicked spirit");
    victim->long_descr = str_dup("A ghostly image hovers over the ground here.\n\r");
    victim->level = 500;
    victim->hit = 8000000;
    victim->max_hit = 2000000;
    victim->hitroll = 2000000;
    victim->damroll = 2000000;
    victim->armor = -100000;
    SET_BIT(victim->act, ACT_NOEXP);
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act("$N materializes next to $n.",ch,NULL,victim,TO_ROOM);
}

void do_spiritkiss( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MSPI] < 2 )
  {
    stc("You must obtain level two Spirit to use Spirit Kiss.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "spirit kiss" ) ) < 0 ) return;
  
  if ( is_affected(ch,sn) )
  {
    stc("You are already blessed by the spirits.\n\r",ch);
    return;
  }

  if ( check_quint(ch) < 1 )
  {
    stc("You need one point of quintessence to call a spirit.\n\r",ch);
    return;
  }

  level = (ch->spheres[MSPI]*20);
  (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
  WAIT_STATE( ch, 12 );
  subtract_quint(ch,1);
}

void do_spiritblast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  if ( IS_CLASS( ch, CLASS_VAMPIRE ) )
     {
        do_reaver_blast( ch, argument );
        return;
     }

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch, CLASS_WIZARD) || ch->clannum != 4 )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 100 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to blast?\n\r", ch );
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

  if ( ch->ctimer[4] == 0 )
     {
        victim->mana = UMAX( 0, victim->mana - 1000000 );
        victim->move = UMAX( 0, victim->move - 1000000 );
        ch->mana = UMIN( ch->max_mana, ch->mana + 2000000 );
        ch->move = UMIN( ch->max_move, ch->move + 2000000 );
     }

  if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
       && ch->ctimer[9] != 100 ) )
     ch->ctimer[4] = 2;

  ch->wait = 12;
  SET_BIT( ch->arena, AFF2_DM );
  subtract_stock( ch, 100 );

  ch->ele_attack = ATTACK_HOLY;
  ch->attack_type = ATTACK_MAGICAL;
  ch->attack_var = SOUL;
  ch->attack_focus = OVERHEAD;

  act("#CYou #wthrow a weird ball of #yplasma #wat #C$N!#n",ch,NULL,victim,TO_CHAR);
  act("#C$n #wthrows a weird ball of #yplasma #wat #Cyou!#n",ch,NULL,victim,TO_VICT);
  act("#C$n #wthrows a weird ball of #yplasma #wat #C$N!#n",ch,NULL,victim,TO_NOTVICT);
  act("#wThe ball suddenly #yexplodes #winto ferocious #Ppurple #wflames!#n",ch,NULL,NULL,TO_CHAR);
  act("#wThe ball suddenly #yexplodes #winto ferocious #Ppurple #wflames!#n",ch,NULL,NULL,TO_ROOM);
  stc("#CYou #yabsorb #wthe #Penergy #wfrom the blast.#n\n\r",ch);
  stc("#yYou #wfeel #edrained#w.#n\n\r",victim);
  act("#C$n #yabsorbs #wthe #Penergy #wfrom the blast.#n",ch,NULL,victim,TO_NOTVICT);
  hurt_person( ch, victim, 1000000, TRUE );
}

void do_rubbing( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 3 )
  {
    stc("You must obtain level three Prime to use Rubbing of the Bones.\n\r",ch);
    return;
  }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Whom do you wish to use this on?\n\r",ch);
        return;
     }
    
  if ( ch->position == POS_STANDING || arg[0] != '\0' )
     {
        if ( ( victim = get_char_area( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
     }

  if ( is_safe(ch,victim) )
     return;

  if ( check_quint(ch) < 3 )
  {
    stc("You must have two points of quintessence to use this power.\n\r",ch);
    return;
  }

  subtract_quint(ch, 3);
  dam = ch->spheres[MPRI] * 50000;

  mage_damage(ch,victim,dam,"quintessence warp",MAGEDAM_QUINT);
  WAIT_STATE(ch, 16);
  act("You are stunned by $n's blast.",ch,NULL,victim,TO_VICT);
  WAIT_STATE(victim, 24);
}

void do_glamour(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	OBJ_DATA *obj;

	argument=one_argument(argument,arg1);
	strcpy(arg2,argument);

	if (IS_NPC(ch)) return;

      if ( !IS_CLASS( ch, CLASS_WIZARD ) || ch->clannum != 4 )
         {
            stc( "Huh?\n\r", ch );
            return;
         }

	if (arg1 == NULL || arg2 == NULL)
	{send_to_char("Syntax: Glamour (book) (description)\n\r",ch);
	return;}

	if ((obj=get_obj_carry(ch,arg1)) == NULL)
	{send_to_char("You dont have that item.\n\r",ch);
	return;}

      if (obj->item_type != ITEM_BOOK )
      {send_to_char( "You can only use this on books.\n\r", ch );
      return;}

	if (strlen(arg2) > 40 || strlen(arg2) < 3)
	{send_to_char("From 3 to 40 characters please.\n\r",ch);
	return;}

	free_string(obj->name);
	obj->name = str_dup(arg2);
	free_string(obj->short_descr);
	obj->short_descr=str_dup(arg2);
	obj->questmaker = str_dup(ch->name);
	send_to_char("Ok.\n\r",ch);

	return;
}

void do_chant( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *book;
    OBJ_DATA *page;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    bool victim_target = FALSE;
    bool object_target = FALSE;
    bool global_target = FALSE; /* Target object/victim may be anywhere */
    int spellno = 1;
    int spellcount = 0;
    int spelltype;
    int sn = 0;
    int level;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );

    if ( !IS_CLASS( ch, CLASS_WIZARD ) || ch->clannum != 4 )
       {
          stc( "Huh?\n\r", ch );
          return;
       }

    wyncount = 0;

    if ( ( book = get_eq_char(ch, WEAR_WIELD) ) == NULL || book->item_type != ITEM_BOOK )
       if ( ( book = get_eq_char(ch, WEAR_HOLD) ) == NULL || book->item_type != ITEM_BOOK )
          if ( ( book = get_eq_char(ch, WEAR_THIRD) ) == NULL || book->item_type != ITEM_BOOK )
             if ( ( book = get_eq_char(ch, WEAR_FOURTH) ) == NULL || book->item_type != ITEM_BOOK )
                {
                   send_to_char( "First you must hold a spellbook.\n\r", ch );
                   return;
                }

    if ( IS_SET(book->value[1], CONT_CLOSED) )
       {
           send_to_char( "First you better open the book.\n\r", ch );
           return;
       }

    if ( book->value[2] < 1 )
       {
           send_to_char( "There are no spells on the index page!\n\r", ch );
           return;
       }

    if ( ( page = get_page( book, book->value[2] ) ) == NULL )
       {
           send_to_char( "The current page seems to have been torn out!\n\r", ch );
           return;
       }

    spellcount = ( ( page->value[1] * 10000 ) + ( page->value[2] * 10 ) + page->value[3] );

    act("You chant the arcane words from $p.",ch,book,NULL,TO_CHAR);
    act("$n chants some arcane words from $p.",ch,book,NULL,TO_ROOM);

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
       REMOVE_BIT(ch->act, PLR_WIZINVIS);

    if ( IS_SET(page->quest, QUEST_MASTER_RUNE) )
       {
           ch->spectype = 0;

           if ( IS_SET(page->spectype, ADV_FAILED) || !IS_SET(page->spectype, ADV_FINISHED) || page->points < 0 )
              send_to_char( "The spell failed because something bad was written on the page.\n\r", ch );
              else if ( IS_SET(page->spectype, ADV_ULTRA_DAMAGE) )
                      {
                          if ( get_stock( ch ) < 300 )
                             {
                                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                                return;
                             }

                          SET_BIT( ch->arena, AFF2_DM );
                          subtract_stock( ch, 300 );

                          ch->spectype = 1;
                          adv_spell_damage(ch,book,page,argument);
                          ch->spectype = 0;
                      }
              else if ( IS_SET(page->spectype, ADV_SUPER_DAMAGE) )
                      {
                          if ( get_stock( ch ) < 200 )
                             {
                                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                                return;
                             }

                          SET_BIT( ch->arena, AFF2_DM );
                          subtract_stock( ch, 200 );

                          ch->spectype = 2;
                          adv_spell_damage(ch,book,page,argument);
                          ch->spectype = 0;
                      }
              else if ( IS_SET(page->spectype, ADV_DAMAGE) )
                      adv_spell_damage(ch,book,page,argument);
              else if ( IS_SET(page->spectype, ADV_AFFECT) )
                      adv_spell_affect(ch,book,page,argument);
              else if ( IS_SET(page->spectype, ADV_ACTION) )
                      adv_spell_action(ch,book,page,argument);
              else send_to_char( "The spell failed because it is accociated with no type.\n\r", ch );

           return;
       }

    switch ( spellcount )
    {
       default: send_to_char( "Nothing happens.\n\r", ch );
                return;

       case 10022: if ( IS_SET( ch->in_room->room_flags, ROOM_BLAZE ) || ch->ctimer[6] > 0 )
                      return;

                   ch->ctimer[6] = 6;

                   act("#RThe room suddenly #yExPlOdEs #Ras a MaNiFeSt oF SeArInG EnErGy EnLiTs the ViCINiTy!!!#n",ch,NULL,NULL,TO_CHAR); 
                   act("#RThe room suddenly #yExPlOdEs #Ras a MaNiFeSt oF SeArInG EnErGy EnLiTs the ViCINiTy!!!#n",ch,NULL,NULL,TO_ROOM); 
                   SET_BIT( ch->in_room->room_flags, ROOM_BLAZE );
                   return;

	  case 5121282: if ( ch->pkill < 20 )
                         {
                            stc( "#CYou #ghaven't advanced enough in your #ystudies #gto chant this magi.#n\n\r", ch );
                            return;
                         }

                      if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
                         {
                            act( "#wThe #Lco#glo#Lur#gful #evortex #yshimmers #was it suddenly #Rimplodes #win front of you.#n", ch, NULL, NULL, TO_CHAR );
                            act( "#wThe #Lco#glo#Lur#gful #evortex #yshimmers #was it suddenly #Rimplodes #win front of you.#n", ch, NULL, NULL, TO_ROOM );
                            REMOVE_BIT( ch->in_room->room_flags, ROOM_SHIFT );
                            return;
                         }

                      SET_BIT( ch->in_room->room_flags, ROOM_SHIFT );

                      act( "#wThe room #gshimmers #was it is filled with #yblinding #wlight!#n", ch, NULL, NULL, TO_CHAR );
                      act( "#wThe room #gshimmers #was it is filled with #yblinding #wlight!#n", ch, NULL, NULL, TO_ROOM );
                      act( "#wAs the light #efades #wback to normal, you find yourself within a #Lcolourful #evortex#w!#n", ch, NULL, NULL, TO_CHAR );
                      act( "#wAs the light #efades #wback to normal, you find yourself within a #Lcolourful #evortex#w!#n", ch, NULL, NULL, TO_ROOM );
                      act( "#gBeautiful leaves #Lencircles #wyour body as #ysparks #wof #Rflames #Cdazzle #wthe floor below!#n", ch, NULL, NULL, TO_CHAR );
                      act( "#gBeautiful leaves #Lencircles #wyour body as #ysparks #wof #Rflames #Cdazzle #wthe floor below!#n", ch, NULL, NULL, TO_ROOM );

                      return;

       case 20022: if ( weather_info.sky == SKY_ICESTORM )
                      return;

                   for ( d = descriptor_list; d != NULL; d = d->next )
                   {
                      if ( d->character == NULL )
                         continue;

                      stc( "#7Swans#n appear tearing a rift in #0space#n.\n\r", d->character );
                      stc( "Suddenly #CWinter#n appears...\n\r", d->character );
                   }

                   sta("The #7snow#n worsens as an #CIcestorm#n brews...#n\n\r", ch->in_room->area );
                   weather_info.sky = SKY_ICESTORM;
                   return;

       case 20024: if ( weather_info.sky == SKY_LIGHTNING )
                      return;

                   for ( d = descriptor_list; d != NULL; d = d->next )
                   {
                      if ( d->character == NULL )
                         continue;

                      stc( "#7Clouds #0form up ahead...#n\n\r", d->character );
                      stc( "Suddenly #Lrain#n appears...\n\r", d->character );
                   }

                   sta("The rain gets heavier.. #zCRASH!! BANG!!#n #ylightning#n has brewed..\n\r", ch->in_room->area );
                   weather_info.sky = SKY_LIGHTNING;
                   return;

       case 40024: if ( ch->pkill < 20 )
                      {
                         stc( "#CYou #ghaven't advanced enough in your #ystudies #gto chant this magi.#n\n\r", ch );
                         return;
                      }

                   if ( IS_SET(ch->in_room->area->bits,WASTE) )
                      {
                         sta("#oThe l#yand #Cshifts #ointo something more #wrecognizable#o.#n\n\r",ch->in_room->area);
                         REMOVE_BIT(ch->in_room->area->bits,WASTE);
                         REMOVE_BIT(ch->in_room->area->bits,UPDATEWASTE);
                         return;
                      }

                   sta("#oThe #yland #oaround you #Cblurs #oand #wshifts#o.#n\n\r",ch->in_room->area);
                   SET_BIT(ch->in_room->area->bits,WASTE);
                   REMOVE_BIT(ch->in_room->area->bits,UPDATEWASTE);
                   return;

       case 80321: /*WATER + ENHANCEMENT + SELF      - Multiple mana spell*/
                   sn = skill_lookup( "energyflux" );
                   global_target = TRUE;
                   victim_target = TRUE;
                   spellno = 1;
                   break;

       case 160162: /*DARK + TRANSPORTATION + TARGETING - Global curse*/ 
                    sn = skill_lookup( "curse" );
                    global_target = TRUE;
                    victim_target = TRUE;
                    break;

       case 160642: /* DARK + REDUCTION + TARGETING - Global Poison*/
                    sn = skill_lookup( "poison" );
                    global_target = TRUE;
                    victim_target = TRUE;
                    break;

       case 161284: /*DARK + CONTROL + AREA           - Sleep */
                    sn = skill_lookup( "sleep" );
                    global_target = TRUE;
                    victim_target = TRUE;
                    break;

       case 640321: /*LIFE + ENHANCEMENT + SELF       - Triple refresh */
                    sn = skill_lookup( "refresh" );
                    global_target = TRUE;
                    victim_target = TRUE;
                    spellno = 1;
                    break;

       case 640642: /*LIFE + REDUCTION + TARGETING    - Global double drain life*/
                    sn = skill_lookup( "energy drain" );
                    spellno = 1;
                    victim_target = TRUE;
                    break;

       case 2560642: /*MIND + REDUCTION + TARGETING    - Global dispel magic */
                     sn = skill_lookup( "dispel magic" );
                     global_target = TRUE;
                     spellno = 1;
                     victim_target = TRUE;
                     break;

       case 2565122: /*MIND + INFORMATION + TARGETING  - Global read aura (ignores shield)*/
                     sn = skill_lookup( "readaura" );
                     global_target = TRUE;
                     victim_target = TRUE;
                     break;

       case 5120161: /*SPIRIT + TRANSPORTATION + SELF  - Teleport */
                     sn = skill_lookup( "teleport" );
                     global_target = TRUE;
                     victim_target = TRUE;
                     break;

       case 5120642: /*SPIRIT + REDUCTION + TARGETING  - Global faerie fire */
                     sn = skill_lookup( "faerie fire" );
                     global_target = TRUE;
                     victim_target = TRUE;
                     break;

       case 81282: /*WATER  |    CONTROL     | TARGETING  - Jail of water */
                   sn = skill_lookup( "jailwater" );
                   victim_target = TRUE;
                   global_target = TRUE;
                   break;
    }

    if ( arg[0] == '\0' && ( victim_target == TRUE || object_target == TRUE ) )
       {
          send_to_char( "Please specify a target.\n\r", ch );
          return;
       }

    if ( victim_target && sn > 0 )
       {
          if ( !global_target && ( victim = get_char_area( ch, arg ) ) == NULL )
             {
                 send_to_char( "They are not here.\n\r", ch );
                 return;
             }
             else if ( global_target && ( victim = get_char_world( ch, arg ) ) == NULL )
                     {
                         send_to_char( "They are not here.\n\r", ch );
                         return;
                     }

          if ( ch->in_room != victim->in_room && ( IS_SET( ch->in_room->room_flags, ROOM_BARRIER ) ||
               IS_SET( victim->in_room->room_flags, ROOM_BARRIER ) ) )
             {
                 act("You frown as you notice the magical barrier negating your mystical powers.",ch,NULL,NULL,TO_CHAR);
                 act("$n frowns as $e notices the magical barrier negating $s mystical powers.",ch,NULL,NULL,TO_ROOM);
                 ch->wait = 12;
                 return;
             }

          spelltype = (skill_table[sn].target);
          level = ch->spl[spelltype] * 0.25;
          (*skill_table[sn].spell_fun) ( sn, level, ch, victim );

          if ( spellno > 1 )
             (*skill_table[sn].spell_fun) ( sn, level, ch, victim );

          if ( spellno > 2 )
             (*skill_table[sn].spell_fun) ( sn, level, ch, victim );

          if ( !IS_IMMORTAL( ch ) )
             WAIT_STATE(ch, skill_table[sn].beats);
       }
       else if ( object_target && sn > 0 )
               {
                  if ( !global_target && ( obj = get_obj_carry( ch, arg ) ) == NULL )
                     {
                         send_to_char( "You are not carrying that object.\n\r", ch );
                         return;
                     }
                     else if ( global_target && ( obj = get_obj_world( ch, arg ) ) == NULL )
                             {
                                send_to_char( "You cannot find any object like that.\n\r", ch );
                                return;
                             }

                  spelltype = (skill_table[sn].target);
                  level = ch->spl[spelltype] * 0.25;
                  (*skill_table[sn].spell_fun) ( sn, level, ch, obj );
                  
                  if ( spellno > 1 )
                     (*skill_table[sn].spell_fun) ( sn, level, ch, obj );

                  if ( spellno > 2 )
                     (*skill_table[sn].spell_fun) ( sn, level, ch, obj );

                  if ( !IS_IMMORTAL( ch) )
                     WAIT_STATE(ch, skill_table[sn].beats);
               }
               else if ( sn > 0 )
                       {
                           spelltype = (skill_table[sn].target);

                           if ( spelltype == TAR_OBJ_INV )
                              {
                                  send_to_char( "Nothing happens.\n\r", ch );
                                  return;
                              }

                           level = ch->spl[spelltype] * 0.25;
                           (*skill_table[sn].spell_fun) ( sn, level, ch, ch );

                           if ( spellno > 1 )
                              (*skill_table[sn].spell_fun) ( sn, level, ch, ch );

                           if ( spellno > 2 )
                              (*skill_table[sn].spell_fun) ( sn, level, ch, ch );

                           if ( !IS_IMMORTAL( ch ) )
                              WAIT_STATE(ch, skill_table[sn].beats);
                        }
                        else send_to_char( "Nothing happens.\n\r", ch );
}

OBJ_DATA *get_page( OBJ_DATA *book, int page_num )
{
    OBJ_DATA *page;
    OBJ_DATA *page_next;

    if (page_num < 1) return NULL;
    for ( page = book->contains; page != NULL; page = page_next )
    {
	page_next = page->next_content;
	if (page->value[0] == page_num) return page;
    }
    return NULL;
}

void adv_spell_damage( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)
{
    char arg [MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    OBJ_DATA *page_next;
    char next_par [MAX_INPUT_LENGTH];
    int mana_cost = 500000;
    int min = page->value[1];
    int max = page->value[2];
    int level;
    bool area_affect = FALSE;
    bool victim_target = FALSE;
    bool global_target = FALSE;
    bool next_page = FALSE;
    bool parameter = FALSE;
    bool not_caster = FALSE;
    bool no_players = FALSE;
    bool cast_message = FALSE;
    bool reversed = FALSE;

    /* Heres My attempt at limiting linked spells to 5, Wynfair */
    if (!IS_SET(page->spectype, ADV_NEXT_PAGE))
	wyncount = 0;	

    if (IS_SET(page->spectype, ADV_NEXT_PAGE))
	wyncount ++;	

    if ( wyncount > 8 )
	 {
     	   wyncount = 0;
 	   return;
	 }

    if (ch->mana < mana_cost)
    {send_to_char("You have insufficient mana to chant this spell.\n\r",ch);return;}

    if (min < 1 || max < 1)
    {send_to_char("The spell failed.\n\r",ch);return;}

    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&
	IS_SET(page->spectype, ADV_SPELL_FIRST))
    {
	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_SET(page->spectype, ADV_PARAMETER))
	{
	    if (!str_cmp(page->chpoweron,"(null)"))
		{send_to_char("The spell failed.\n\r",ch);return;}
	    else strcpy(next_par,page->chpoweron);
	}
	else strcpy(next_par,arg);
	if ( page->specpower < page->value[0] )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
		adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_ULTRA_DAMAGE) && ch->spectype == 1 )
                    adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_SUPER_DAMAGE) && ch->spectype == 2 )
                    adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
		adv_spell_affect(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
		adv_spell_action(ch,book,page_next,next_par);
	    else {send_to_char( "The spell failed.\n\r", ch );return;}
	}
	else {send_to_char( "The spell failed.\n\r", ch );return;}
    }

    one_argument( argument, arg );

    if (strlen(page->victpoweron) > 0 || str_cmp(page->victpoweron,"(null)"))
    {
	if (strlen(page->victpoweroff) > 0 || str_cmp(page->victpoweroff,"(null)"))
	    cast_message = TRUE;
    }

    if (IS_SET(page->spectype, ADV_AREA_AFFECT  )) area_affect   = TRUE;
    if (IS_SET(page->spectype, ADV_VICTIM_TARGET)) victim_target = TRUE;
    if (IS_SET(page->spectype, ADV_GLOBAL_TARGET)) global_target = TRUE;
    if (IS_SET(page->spectype, ADV_NEXT_PAGE    )) next_page     = TRUE;
    if (IS_SET(page->spectype, ADV_PARAMETER    )) parameter     = TRUE;
    if (IS_SET(page->spectype, ADV_NOT_CASTER   )) not_caster    = TRUE;
    if (IS_SET(page->spectype, ADV_NO_PLAYERS   )) no_players    = TRUE;
    if (IS_SET(page->spectype, ADV_REVERSED     )) reversed      = TRUE;

    if (victim_target)
    {
	CHAR_DATA *victim;
      int timer;
	
      if ( ( victim = get_char_area( ch, arg ) ) == NULL )
	{
          if ( ch->fighting == NULL )
             send_to_char( "They are not here.\n\r", ch );
             else victim = ch->fighting;

	    return;
	}

	if (!reversed)
	{
	    if (is_safe(ch,victim))
		return;
      }

	if (reversed)
	{
          if ( !can_heal( ch, victim ) )
             return;

          if ( wyncount == 7 )
             hit_gain( victim, 300000 );

	    sprintf(buf,"Your %s heals $N quite beautifully!",page->chpoweroff);
	    act2(buf,ch,NULL,victim,TO_CHAR);
	    sprintf(buf,"$n's %s heals $N quite beautifully!",page->chpoweroff);
	    act2(buf,ch,NULL,victim,TO_NOTVICT);
	    sprintf(buf,"$n's %s heals you quite beautifully!",page->chpoweroff);
	    act2(buf,ch,NULL,victim,TO_VICT);

          ch->mana -= 50000;
	    WAIT_STATE(ch,12);
	}
	else
	{
          if ( ch->in_room != victim->in_room && ( IS_SET( ch->in_room->room_flags, ROOM_BARRIER ) ||
               IS_SET( victim->in_room->room_flags, ROOM_BARRIER ) ) )
             {
                 act("You frown as you notice the magical barrier negating your mystical powers.",ch,NULL,NULL,TO_CHAR);
                 act("$n frowns as $e notices the magical barrier negating $s mystical powers.",ch,NULL,NULL,TO_ROOM);
                 ch->wait = 12;
                 return;
             }

	    sprintf(buf,"Your %s strikes $N incredibly hard!",page->chpoweroff);
	    act2(buf,ch,NULL,victim,TO_CHAR);
	    sprintf(buf,"$n's %s strikes $N incredibly hard!",page->chpoweroff);
	    act2(buf,ch,NULL,victim,TO_NOTVICT);
	    sprintf(buf,"$n's %s strikes you incredibly hard!",page->chpoweroff);
	    act2(buf,ch,NULL,victim,TO_VICT);

          ch->mana -= 62500;
	    WAIT_STATE(ch,12);

          if ( wyncount == 7 )
             {
                ch->attack_type = ATTACK_MAGICAL;
                ch->attack_var = SOUL;
                ch->attack_focus = AERIAL;

                if ( IS_SET(page->spectype, ADV_ULTRA_DAMAGE) )
                   hurt_person( ch, victim, 3500000, TRUE );
                   else if ( IS_SET(page->spectype, ADV_SUPER_DAMAGE) )
                           hurt_person( ch, victim, 2000000, TRUE );
                           else hurt_person( ch, victim, 700000, TRUE );
             }
	}
    }
    else if (area_affect)
    {
       CHAR_DATA *vch, *vch_next;

       WAIT_STATE(ch,12);

       for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
       {
          vch_next = vch->next_in_room;

          if ( reversed )
             {
                if ( !can_heal( ch, vch ) )
                   return;

                sprintf(buf,"Your %s heals $N quite beautifully!",page->chpoweroff);
                act2(buf,ch,NULL,vch,TO_CHAR);
                sprintf(buf,"$n's %s heals $N quite beautifully!",page->chpoweroff);
                act2(buf,ch,NULL,vch,TO_NOTVICT);
                sprintf(buf,"$n's %s heals you quite beautifully!",page->chpoweroff);
                act2(buf,ch,NULL,vch,TO_VICT);

                if ( wyncount == 7 )
                   hit_gain( vch, 300000 );
             }
             else {
                    if ( is_safe(ch, vch) )
                       continue;

                     sprintf(buf,"Your %s strikes $N incredably hard!",page->chpoweroff);
                     act2(buf,ch,NULL,vch,TO_CHAR);
                     sprintf(buf,"$n's %s strikes $N incredably hard!",page->chpoweroff);
                     act2(buf,ch,NULL,vch,TO_NOTVICT);
                     sprintf(buf,"$n's %s strikes you incredably hard!",page->chpoweroff);
                     act2(buf,ch,NULL,vch,TO_VICT);

                     ch->mana -= 62500;

                     if ( wyncount == 7 )
                        {
                           ch->attack_type = ATTACK_MAGICAL;
                           ch->attack_var = SOUL;
                           ch->attack_focus = AERIAL;

                           if ( IS_SET(page->spectype, ADV_ULTRA_DAMAGE) )
                              hurt_person( ch, vch, 2500000, TRUE );
                              else if ( IS_SET(page->spectype, ADV_SUPER_DAMAGE) )
                                      hurt_person( ch, vch, 1500000, TRUE );
                                      else hurt_person( ch, vch, 500000, TRUE );
                        }
	            }
       }
    }
    else {send_to_char("The spell failed.\n\r",ch);return;}
    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&
	!IS_SET(page->spectype, ADV_SPELL_FIRST))
    {
	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_SET(page->spectype, ADV_PARAMETER))
	{
	    if (!str_cmp(page->chpoweron,"(null)"))
		{send_to_char("The spell failed.\n\r",ch);return;}
	    else strcpy(next_par,page->chpoweron);
	}
	else strcpy(next_par,arg);
	if ( page->specpower < page->value[0] )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
		adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_ULTRA_DAMAGE) && ch->spectype == 1 )
                    adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_SUPER_DAMAGE) && ch->spectype == 2 )
                    adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
		adv_spell_affect(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
		adv_spell_action(ch,book,page_next,next_par);
	    else {send_to_char( "The spell failed.\n\r", ch );return;}
	}
	else {send_to_char( "The spell failed.\n\r", ch );return;}
    }
    return;
}

void adv_spell_affect( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)
{
    char arg [MAX_STRING_LENGTH];
    char c_m [MAX_INPUT_LENGTH];
    char c_1 [MAX_INPUT_LENGTH];
    char c_2 [MAX_INPUT_LENGTH];
    OBJ_DATA *page_next;
    OBJ_DATA *obj = NULL;
    CHAR_DATA *victim = ch;
    char next_par [MAX_INPUT_LENGTH];
    int mana_cost = page->points;
    int apply_bit = page->value[1];
    int bonuses = page->value[2];
    int affect_bit = page->value[3];
    int sn;
    int level = page->level;
    bool any_affects = FALSE;
    bool area_affect = FALSE;
    bool victim_target = FALSE;
    bool object_target = FALSE;
    bool global_target = FALSE;
    bool next_page = FALSE;
    bool parameter = FALSE;
    bool not_caster = FALSE;
    bool no_players = FALSE;
    bool cast_message = FALSE;
    bool message_one = FALSE;
    bool message_two = FALSE;
    bool is_reversed = FALSE;

    if ( ch->pkill < 20 )
       {
          stc( "#CYou #ghaven't advanced enough in your #ystudies #gto chant this magi.#n\n\r", ch );
          return;
       }

    if( mana_cost <= 1 ) mana_cost = 1;

    /* Heres My attempt at limiting linked spells to 5, Wynfair */
    if (!IS_SET(page->spectype, ADV_NEXT_PAGE))
	wyncount = 0;	

    if (IS_SET(page->spectype, ADV_NEXT_PAGE))
	wyncount ++;	

    if ( wyncount > 8 )
	 {
     	   wyncount = 0;
 	   return;
	 }

    if (ch->mana < mana_cost)
    {send_to_char("You have insufficient mana to chant this spell.\n\r",ch);return;}

    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&
	IS_SET(page->spectype, ADV_SPELL_FIRST))
    {
	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_SET(page->spectype, ADV_PARAMETER))
	{
	    if (!str_cmp(page->chpoweron,"(null)"))
		{send_to_char("The spell failed.\n\r",ch);return;}
	    else strcpy(next_par,page->chpoweron);
	}
	else strcpy(next_par,argument);
	if ( page->specpower < page->value[0] )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
		adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_ULTRA_DAMAGE) && ch->spectype == 1 )
                    adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_SUPER_DAMAGE) && ch->spectype == 2 )
                    adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
		adv_spell_affect(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
		adv_spell_action(ch,book,page_next,next_par);
	    else {send_to_char( "The spell failed.\n\r", ch );return;}
	}
	else {send_to_char( "The spell failed.\n\r", ch );return;}
    }

    one_argument( argument, arg );

    if (strlen(page->chpoweroff) > 0 && str_cmp(page->chpoweroff,"(null)"))
	{strcpy(c_m,page->chpoweroff);cast_message = TRUE;}
    if (strlen(page->victpoweron) > 0 && str_cmp(page->victpoweron,"(null)"))
	{strcpy(c_1,page->victpoweron);message_one = TRUE;}
    if (strlen(page->victpoweroff) > 0 && str_cmp(page->victpoweroff,"(null)"))
	{strcpy(c_2,page->victpoweroff);message_two = TRUE;}

    if (IS_SET(page->spectype, ADV_AREA_AFFECT  )) area_affect   = TRUE;
    if (IS_SET(page->spectype, ADV_VICTIM_TARGET)) victim_target = TRUE;
    if (IS_SET(page->spectype, ADV_OBJECT_TARGET)) object_target = TRUE;
    if (IS_SET(page->spectype, ADV_GLOBAL_TARGET)) global_target = TRUE;
    if (IS_SET(page->spectype, ADV_NEXT_PAGE    )) next_page     = TRUE;
    if (IS_SET(page->spectype, ADV_PARAMETER    )) parameter     = TRUE;
    if (IS_SET(page->spectype, ADV_NOT_CASTER   )) not_caster    = TRUE;
    if (IS_SET(page->spectype, ADV_NO_PLAYERS   )) no_players    = TRUE;
    if (IS_SET(page->spectype, ADV_REVERSED     )) 
	{is_reversed = TRUE;bonuses = 0 - bonuses;}

    if (victim_target && !area_affect && !global_target && !object_target)
    {
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if ( victim->in_room == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (victim_target && area_affect && !global_target && !object_target)
    {
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim->in_room == NULL ||
	    victim->in_room->area != ch->in_room->area)
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (victim_target && global_target && !object_target)
    {
	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim->in_room == NULL)
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (object_target && !area_affect && !global_target && !victim_target)
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (object_target && area_affect && !global_target && !victim_target)
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (object_target && global_target && !victim_target)
    {
	if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (obj->in_room == NULL)
	{send_to_char("The spell failed.\n\r",ch);return;}
    }

    if ( ( sn = skill_lookup( "magi sorcery" ) ) < 0 )
       {send_to_char("The spell failed.\n\r",ch);return;}

    if (!victim_target && victim != ch)
	{send_to_char("The spell failed.\n\r",ch);return;}
    if (not_caster && ch == victim)
	{send_to_char("The spell failed.\n\r",ch);return;}
    else if (no_players && !IS_NPC(victim))
	{send_to_char("The spell failed.\n\r",ch);return;}

    if ( is_affected( victim, sn ) )
    {
	send_to_char("They are already affected by a spell of that colour.\n\r",ch);
	return;
    }

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
    {
        stc("The power of the holy saints nullifies your affects.\n\r",ch);
        return;
    }

    if ( apply_bit == 0 )
    {
	enhance_stat(sn,level,ch,victim,APPLY_NONE,bonuses,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_STR) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_STR,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_DEX) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_DEX,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_INT) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_INT,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_WIS) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_WIS,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_AGI) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_AGI,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_CON) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_CON,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_EVA) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_EVA,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_TEN) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_TENSION,bonuses * 0.08,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_MANA) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_MANA,bonuses * 15000,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_HIT) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_HIT,bonuses * 15000,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_MOVE) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_MOVE,bonuses * 15000,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if ( IS_SET(apply_bit, ADV_SAVING_SPELL) )
    {
	enhance_stat(sn,level,ch,victim,APPLY_SAVING_SPELL,bonuses*0.2,affect_bit);
	affect_bit = 0;
	any_affects = TRUE;
    }
    if (!any_affects) {send_to_char("The spell failed.\n\r",ch);return;}

    if (cast_message) act2(c_m,ch,NULL,victim,TO_CHAR);
    if (message_one)  act2(c_1,ch,NULL,victim,TO_VICT);
    if (message_two)  act2(c_2,ch,NULL,victim,TO_NOTVICT);
    
    if (!IS_IMMORTAL( ch ) && !IS_SET(book->spectype, SITEM_MAGE) )
    {
		WAIT_STATE(ch,12);
	ch->mana -= mana_cost;
    }

    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&
	!IS_SET(page->spectype, ADV_SPELL_FIRST))
    {
	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_SET(page->spectype, ADV_PARAMETER))
	{
	    if (!str_cmp(page->chpoweron,"(null)"))
		{send_to_char("The spell failed.\n\r",ch);return;}
	    else strcpy(next_par,page->chpoweron);
	}
	else strcpy(next_par,argument);
	if ( page->specpower < page->value[0] )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
		adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_ULTRA_DAMAGE) && ch->spectype == 1 )
                    adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_SUPER_DAMAGE) && ch->spectype == 2 )
                    adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
		adv_spell_affect(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
		adv_spell_action(ch,book,page_next,next_par);
	    else {send_to_char( "The spell failed.\n\r", ch );return;}
	}
	else {send_to_char( "The spell failed.\n\r", ch );return;}
    }
}

void adv_spell_action( CHAR_DATA *ch, OBJ_DATA *book, OBJ_DATA *page, char *argument)
{
    char arg1 [MAX_STRING_LENGTH];
    char arg2 [MAX_STRING_LENGTH];
    char c_m [MAX_INPUT_LENGTH];
    char c_1 [MAX_INPUT_LENGTH];
    char c_2 [MAX_INPUT_LENGTH];
    OBJ_DATA *page_next;
    OBJ_DATA *obj = NULL;
    CHAR_DATA *victim = NULL;
    CHAR_DATA *victim2 = NULL;
    ROOM_INDEX_DATA *old_room = ch->in_room;
    char next_par [MAX_INPUT_LENGTH];
    int mana_cost = page->points;
    int action_bit = page->value[1];
    int action_type = page->value[2];
    bool area_affect = FALSE;
    bool victim_target = FALSE;
    bool object_target = FALSE;
    bool second_victim = FALSE;
    bool second_object = FALSE;
    bool global_target = FALSE;
    bool next_page = FALSE;
    bool parameter = FALSE;
    bool not_caster = FALSE;
    bool no_players = FALSE;
    bool cast_message = FALSE;
    bool message_one = FALSE;
    bool message_two = FALSE;
    bool is_reversed = FALSE;

    if( mana_cost <= 1 ) mana_cost = 1;

    /* Heres My attempt at limiting linked spells to 5, Wynfair */
    if (!IS_SET(page->spectype, ADV_NEXT_PAGE))
	wyncount = 0;	

    if (IS_SET(page->spectype, ADV_NEXT_PAGE))
	wyncount ++;	

    if ( wyncount > 8 )
	 {
     	   wyncount = 0;
 	   return;
	 }

    if (ch->mana < mana_cost)
    {send_to_char("You have insufficient mana to chant this spell.\n\r",ch);return;}

    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&
	IS_SET(page->spectype, ADV_SPELL_FIRST))
    {
	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_SET(page->spectype, ADV_PARAMETER))
	{
	    if (!str_cmp(page->chpoweron,"(null)"))
		{send_to_char("The spell failed.\n\r",ch);return;}
	    else strcpy(next_par,page->chpoweron);
	}
	else strcpy(next_par,argument);
	if ( page->specpower < page->value[0] )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
		adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_ULTRA_DAMAGE) && ch->spectype == 1 )
                    adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_SUPER_DAMAGE) && ch->spectype == 2 )
                    adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
		adv_spell_affect(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
		adv_spell_action(ch,book,page_next,next_par);
	    else {send_to_char( "The spell failed.\n\r", ch );return;}
	}
	else {send_to_char( "The spell failed.\n\r", ch );return;}
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (strlen(page->chpoweroff) > 0 && str_cmp(page->chpoweroff,"(null)"))
	{strcpy(c_m,page->chpoweroff);cast_message = TRUE;}
    if (strlen(page->victpoweron) > 0 && str_cmp(page->victpoweron,"(null)"))
	{strcpy(c_1,page->victpoweron);message_one = TRUE;}
    if (strlen(page->victpoweroff) > 0 && str_cmp(page->victpoweroff,"(null)"))
	{strcpy(c_2,page->victpoweroff);message_two = TRUE;}

    if (IS_SET(page->spectype, ADV_AREA_AFFECT  )) area_affect   = TRUE;
    if (IS_SET(page->spectype, ADV_VICTIM_TARGET)) victim_target = TRUE;
    if (IS_SET(page->spectype, ADV_OBJECT_TARGET)) object_target = TRUE;
    if (IS_SET(page->spectype, ADV_GLOBAL_TARGET)) global_target = TRUE;
    if (IS_SET(page->spectype, ADV_NEXT_PAGE    )) next_page     = TRUE;
    if (IS_SET(page->spectype, ADV_PARAMETER    )) parameter     = TRUE;
    if (IS_SET(page->spectype, ADV_NOT_CASTER   )) not_caster    = TRUE;
    if (IS_SET(page->spectype, ADV_NO_PLAYERS   )) no_players    = TRUE;
    if (IS_SET(page->spectype, ADV_SECOND_VICTIM)) second_victim = TRUE;
    if (IS_SET(page->spectype, ADV_SECOND_OBJECT)) second_object = TRUE;
    if (IS_SET(page->spectype, ADV_REVERSED     )) is_reversed   = TRUE;

    if (victim_target && !area_affect && !global_target && !object_target)
    {
	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim->in_room == NULL || victim->level > 600 )
	{send_to_char("The spell failed.\n\r",ch);return;}

	if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
	{send_to_char("The spell failed.\n\r",ch);return;}

	if (victim == ch)
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( in_kingdom( ch, victim, victim->in_room ) )
         return;
    }
    else if (victim_target && area_affect && !global_target && !object_target)
    {
	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim->in_room == NULL || victim->level > 600 ||
	    victim->in_room->area != ch->in_room->area)
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim == ch)
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( in_kingdom( ch, victim, victim->in_room ) )
         return;
    }
    else if (victim_target && global_target && !object_target)
    {
	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim->in_room == NULL || victim->level > 600 )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (victim == ch)
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( in_kingdom( ch, victim, victim->in_room ) )
         return;
    }
    else if (object_target && !area_affect && !global_target && !victim_target)
    {
	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( IS_SET(obj->quest, QUEST_ARTIFACT) || obj->item_type == ITEM_HUNT )
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (object_target && area_affect && !global_target && !victim_target)
    {
	if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( IS_SET(obj->quest, QUEST_ARTIFACT) || obj->item_type == ITEM_HUNT )
	{send_to_char("The spell failed.\n\r",ch);return;}
    }
    else if (object_target && global_target && !victim_target)
    {
	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (obj->in_room == NULL)
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( IS_SET(obj->quest, QUEST_ARTIFACT) || obj->item_type == ITEM_HUNT )
	{send_to_char("The spell failed.\n\r",ch);return;}
    }

    if (victim != NULL)
    {
	if (victim->level > 500 )
	{
	    send_to_char("The spell failed.\n\r",ch);
	    return;
	}
    }

    if (victim2 != NULL)
    {
	if (victim2->level > 500 )
	{
	    send_to_char("The spell failed.\n\r",ch);
	    return;
	}
    }

    switch ( action_bit )
    {
    default:
	send_to_char("The spell failed.\n\r",ch);
	return;
    case ACTION_MOVE:
      if (!victim_target && !second_victim && !object_target && !second_object)
	{
	    if (cast_message) act2(c_m,ch,NULL,NULL,TO_CHAR);
	    if (message_one) act2(c_1,ch,NULL,NULL,TO_ROOM);
	    char_from_room(ch);
	    char_to_room(ch,old_room);
	    if (message_two) act2(c_2,ch,NULL,NULL,TO_ROOM);
	}
	else if ( arg1[0] == '\0' )
	{
	    send_to_char("Please specify a target.\n\r",ch);
	    return;
	}
	else if (victim_target && !second_victim && !object_target && !second_object)
	{
	    if (victim == NULL || victim->in_room == NULL || victim->level > 500 ||
		victim->in_room == ch->in_room)
	    {send_to_char("The spell failed.\n\r",ch);return;}
	    if (is_reversed)
	    {
		if (victim->position == POS_FIGHTING)
		{send_to_char("The spell failed.\n\r",ch);return;}
		if (cast_message) act2(c_m,victim,NULL,NULL,TO_CHAR);
		if (message_one) act2(c_1,victim,NULL,NULL,TO_ROOM);
		char_from_room(victim);
		char_to_room(victim,ch->in_room);
		if (message_two) act2(c_2,victim,NULL,NULL,TO_ROOM);
		do_look(victim,"");
	    }
	    else
	    {
		if (ch->position == POS_FIGHTING)
		{send_to_char("The spell failed.\n\r",ch);return;}
		if (cast_message) act2(c_m,ch,NULL,NULL,TO_CHAR);
		if (message_one) act2(c_1,ch,NULL,NULL,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch,victim->in_room);
		if (message_two) act2(c_2,ch,NULL,NULL,TO_ROOM);
		do_look(ch,"");
	    }
	}
	else if (!victim_target && !second_victim && object_target && !second_object)
	{
	    if (obj == NULL || obj->in_room == NULL ||
		obj->in_room == ch->in_room)
	    {send_to_char("The spell failed.\n\r",ch);return;}
	    if (cast_message) act2(c_m,ch,obj,NULL,TO_CHAR);
	    if (message_one) act2(c_1,ch,obj,NULL,TO_ROOM);
	    if (is_reversed)
	    {
		obj_from_room(obj);
		obj_to_room(obj,ch->in_room);
	    }
	    else
	    {
		char_from_room(ch);
		char_to_room(ch,obj->in_room);
		do_look(ch,"");
	    }
	    if (message_two) act2(c_2,ch,obj,NULL,TO_ROOM);
	}
	else if (victim_target && second_victim && !object_target && !second_object)
	{
	    if (victim == NULL || victim->in_room == NULL)
	    {send_to_char("The spell failed.\n\r",ch);return;}
	    if (victim2 == NULL || victim2->in_room == NULL || victim2->level > 500 ||
		victim2->in_room == victim->in_room)
	    {send_to_char("The spell failed.\n\r",ch);return;}
	    if (is_reversed)
	    {
		if (victim2->position == POS_FIGHTING)
		{send_to_char("The spell failed.\n\r",ch);return;}
		if (cast_message) act2(c_m,victim2,NULL,victim,TO_CHAR);
		if (message_one) act2(c_1,victim2,NULL,victim,TO_ROOM);
		char_from_room(victim2);
		char_to_room(victim2,victim->in_room);
		if (message_two) act2(c_2,victim2,NULL,victim,TO_ROOM);
		do_look(victim2,"");
	    }
	    else
	    {
		if (victim->position == POS_FIGHTING)
		{send_to_char("The spell failed.\n\r",ch);return;}
		if (cast_message) act2(c_m,victim,NULL,victim2,TO_CHAR);
		if (message_one) act2(c_1,victim,NULL,victim2,TO_ROOM);
		char_from_room(victim);
		char_to_room(victim,victim2->in_room);
		if (message_two) act2(c_2,victim,NULL,victim2,TO_ROOM);
		do_look(victim,"");
	    }
	}
	else {send_to_char("The spell failed.\n\r",ch);return;}
	break;
    case ACTION_MOB:
	if ( action_type < 1)
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_NPC(ch) || ch->pcdata->followers > 5)
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (( victim = create_mobile( get_mob_index( action_type ))) == NULL || victim->level > 500 )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (cast_message) act2(c_m,ch,NULL,victim,TO_CHAR);
	if (message_one) act2(c_1,ch,NULL,victim,TO_ROOM);
	ch->pcdata->followers++;
	char_to_room( victim, ch->in_room );
	SET_BIT(victim->act, ACT_NOEXP);
	free_string(victim->lord);
	victim->lord = str_dup(ch->name);

	break;
    case ACTION_OBJECT:
	if ( action_type < 1)
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( action_type > 89 && action_type < 100 )
	{send_to_char("The spell failed.\n\r",ch);return;}

      if ( action_type >= 900 && action_type <= 907 )
	{send_to_char("The spell failed.\n\r",ch);return;}

	if (( obj = create_object( get_obj_index( action_type ),0 )) == NULL )
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (cast_message) act2(c_m,ch,obj,NULL,TO_CHAR);
	if (message_one) act2(c_1,ch,obj,NULL,TO_ROOM);
	free_string(obj->questmaker);
	obj->questmaker = str_dup(ch->name);
	obj_to_room( obj, ch->in_room );
	break;
    }
    if (!IS_IMMORTAL( ch ) && !IS_SET(book->spectype, SITEM_MAGE) )
    {
		WAIT_STATE(ch,12);
	ch->mana -= mana_cost;
    }

    if (IS_SET(page->spectype, ADV_NEXT_PAGE) &&
	!IS_SET(page->spectype, ADV_SPELL_FIRST))
    {
	if (strlen(page->chpoweroff) < 2 || !str_cmp(page->chpoweroff,"(null)"))
	{send_to_char("The spell failed.\n\r",ch);return;}
	if (IS_SET(page->spectype, ADV_PARAMETER))
	{
	    if (!str_cmp(page->chpoweron,"(null)"))
		{send_to_char("The spell failed.\n\r",ch);return;}
	    else strcpy(next_par,page->chpoweron);
	}
	else strcpy(next_par,argument);
	if ( page->specpower < page->value[0] )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if ( ( page_next = get_page( book, page->specpower ) ) == NULL )
	{send_to_char( "The spell failed.\n\r", ch );return;}
	if (IS_SET(page_next->quest, QUEST_MASTER_RUNE))
	{
	    if      (IS_SET(page_next->spectype, ADV_DAMAGE))
		adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_ULTRA_DAMAGE) && ch->spectype == 1 )
                    adv_spell_damage(ch,book,page_next,next_par);
            else if ( IS_SET(page_next->spectype, ADV_SUPER_DAMAGE) && ch->spectype == 2 )
                    adv_spell_damage(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_AFFECT))
		adv_spell_affect(ch,book,page_next,next_par);
	    else if (IS_SET(page_next->spectype, ADV_ACTION))
		adv_spell_action(ch,book,page_next,next_par);
	    else {send_to_char( "The spell failed.\n\r", ch );return;}
	}
	else {send_to_char( "The spell failed.\n\r", ch );return;}
    }
    return;
}


void do_mlearn(CHAR_DATA *ch, char *argument) {

	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int type = 0;
	int power = -1;
	int cost;

	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_WIZARD) || ch->clannum != 4) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (!str_cmp(arg1,"sigil")) {
	power = MPOWER_SIGILS;
	if (!str_cmp(arg2,"self")) type = SIGIL_SELF;
	else if (!str_cmp(arg2,"targeting")) type = SIGIL_TARGETING;
	else if (!str_cmp(arg2,"area")) type = SIGIL_AREA;
	else if (!str_cmp(arg2,"object")) type = SIGIL_OBJECT;	
	}

	if (!str_cmp(arg1,"rune")) {
	power = MPOWER_RUNES;
	if (!str_cmp(arg2,"mastery")) type = RUNE_MASTER;
	else if (!str_cmp(arg2,"spirit")) type = RUNE_SPIRIT;
	else if (!str_cmp(arg2,"mind")) type = RUNE_MIND;
	else if (!str_cmp(arg2,"life")) type = RUNE_LIFE;
	else if (!str_cmp(arg2,"death")) type = RUNE_DEATH;
	else if (!str_cmp(arg2,"earth")) type = RUNE_EARTH;
	else if (!str_cmp(arg2,"air")) type = RUNE_AIR;
	else if (!str_cmp(arg2,"fire")) type = RUNE_FIRE;
	else if (!str_cmp(arg2,"water")) type = RUNE_WATER;
	else if (!str_cmp(arg2,"dark")) type = RUNE_DARK;
	else if (!str_cmp(arg2,"light")) type = RUNE_LIGHT;
	}

	if (!str_cmp(arg1,"glyph")) {
	power = MPOWER_GLYPHS;
	if (!str_cmp(arg2,"protection")) type = GLYPH_PROTECTION;
	else if (!str_cmp(arg2,"reduction")) type = GLYPH_REDUCTION;
	else if (!str_cmp(arg2,"control")) type = GLYPH_CONTROL;
	else if (!str_cmp(arg2,"destruction")) type = GLYPH_DESTRUCTION;
	else if (!str_cmp(arg2,"transportation")) type = GLYPH_TRANSPORTATION;
	else if (!str_cmp(arg2,"transformation")) type = GLYPH_TRANSFORMATION;
	else if (!str_cmp(arg2,"creation")) type = GLYPH_CREATION;
	else if (!str_cmp(arg2,"information")) type = GLYPH_INFORMATION;
	else if (!str_cmp(arg2,"summoning")) type = GLYPH_SUMMONING;
	else if (!str_cmp(arg2,"enhancement")) type = GLYPH_ENHANCEMENT;
	}

	if (power == -1) {
	send_to_char("Syntax:  mlearn [power] [type]\n\r", ch );
	send_to_char("\n\rtypes:  sigil, glyph, rune\n\r\n\r", ch);
	return;}

	if (power == MPOWER_SIGILS && type == 0) {
	send_to_char("Syntax:  mlearn sigil [type]\n\r\n\r", ch);
	send_to_char("types:  area, object, self, targeting\n\r\n\r", ch);
	return;}

	if (power == MPOWER_GLYPHS && type == 0) {
	send_to_char("Syntax:  mlearn glyph [type]\n\r\n\r", ch);
	send_to_char("types:  creation, destruction, summoning, transformation\n\r", ch);
	send_to_char("        transportation, enhancement, reduction, control\n\r", ch);
	send_to_char("        protection, information\n\r\n\r", ch);
	return;}

	if (power == MPOWER_RUNES && type == 0) {
	send_to_char("Syntax:  mlearn rune [type]\n\r\n\r", ch );
	send_to_char("types:  fire, air, earth, water, dark, light\n\r", ch );
	send_to_char("        life, death, mind, spirit\n\r", ch );
	send_to_char("        MASTERY\n\r\n\r", ch );
	return;}

	if (power == MPOWER_RUNES && type == RUNE_MASTER) {

	if (IS_SET(ch->pcdata->magejunk[MPOWER_RUNES], RUNE_MASTER)) {
	send_to_char("You already have that.\n\r", ch );
	return;}

      if ( IS_SET(ch->pcdata->magejunk[power], type) ) {
	send_to_char("You already have that.\n\r", ch );
	return;}

	SET_BIT(ch->pcdata->magejunk[power], type);
	send_to_char("You learn how to make the rune of mastery.\n\r", ch );
	return;
	}

      if ( IS_SET(ch->pcdata->magejunk[power], type) ) {
	send_to_char("You already have that.\n\r", ch );
	return;}

	send_to_char("You learn a new power.\n\r", ch );
	SET_BIT(ch->pcdata->magejunk[power], type);
}

void do_read( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to read?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char("You don't have that book.\n\r",ch);
	return;
    }

    if ( obj->item_type == ITEM_PAGE)
    {
	if (!str_cmp(obj->victpoweruse,"(null)"))
	    sprintf(buf,"Untitled page.\n\r");
	else
	    sprintf(buf,"%s.\n\r",obj->victpoweruse);
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	if (obj->chpoweruse == NULL || obj->chpoweruse == '\0' ||
	    !str_cmp(obj->chpoweruse,"(null)"))
	{
	    if (!are_runes(obj))
		send_to_char("This page is blank.\n\r",ch);
	    else if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
		!IS_SET(obj->quest, QUEST_MASTER_RUNE) &&
		!IS_SET(obj->spectype, ADV_STARTED))
		show_runes(ch, obj, FALSE);
	    else
		send_to_char("This page is blank.\n\r",ch);
	    return;
	}
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char(obj->chpoweruse,ch);
	send_to_char("\n\r--------------------------------------------------------------------------------\n\r",ch);
	if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
	    !IS_SET(obj->spectype, ADV_STARTED) &&
	    !IS_SET(obj->quest, QUEST_MASTER_RUNE )) show_runes(ch, obj, TRUE);
	return;
    }

    if ( obj->item_type != ITEM_BOOK)
    { send_to_char( "That's not a book.\n\r", ch ); return; }
    if ( IS_SET(obj->value[1], CONT_CLOSED) )
    {
	if (!str_cmp(obj->victpoweruse,"(null)"))
	    sprintf(buf,"The book is untitled.\n\r");
	else
	    sprintf(buf,"The book is titled '%s'.\n\r",obj->victpoweruse);
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	return;
    }

    if (obj->value[2] == 0)
    {
	int page;

	send_to_char("Index page.\n\r",ch);
	if (obj->value[3] <= 0)
	{
	    send_to_char("<No pages>\n\r",ch);
	    return;
	}
	for ( page = 1; page <= obj->value[3]; page ++ )
	{
	    sprintf(buf,"Page %d:",page);
	    send_to_char(buf,ch);
	    show_page(ch,obj,page,TRUE);
	}
    }
    else
    {
	sprintf(buf,"Page %d:",obj->value[2]);
	send_to_char(buf,ch);
	show_page(ch,obj,obj->value[2],FALSE);
    }
    return;
}

void do_write( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *object;
    CHAR_DATA *mobile;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax: Write <page> <title/line> <text>.\n\r",ch);
	stc("Colour Syntax: Write <page> <0-7> end.spell\n\r", ch);
	stc("0-7 equal colors, 0-4 normal 5 colors. 5-7 special colors.\n\r",ch);
	return;
    }

    if ( ( ( ( obj = get_eq_char(ch, WEAR_HOLD) ) != NULL) && 
	( obj->item_type != ITEM_TOOL || !IS_SET(obj->value[0],TOOL_PEN) ) )
	|| obj == NULL)
    {
	if ( ( ( ( obj = get_eq_char(ch, WEAR_WIELD) ) != NULL) && 
	    ( obj->item_type != ITEM_TOOL || !IS_SET(obj->value[0],TOOL_PEN) ) )
	    || obj == NULL)
	{
	    send_to_char("You are not holding a pen.\n\r",ch);
	    return;
	}
    }

    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }

    if ( obj->item_type != ITEM_PAGE && obj->item_type != ITEM_BOOK )
    {
	send_to_char("You cannot write on that.\n\r",ch);
	return;
    }

    if ( IS_SET(obj->spectype, ADV_FAILED) )
    {
	send_to_char("This page is faulty and needs to be written over.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2,"title") )
    {
        free_string( obj->victpoweruse );
        obj->victpoweruse = str_dup( arg3 );
        send_to_char("Ok.\n\r",ch);
        act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
        return;
    }
    else if ( !str_cmp( arg2,"0") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 0;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( !str_cmp( arg2,"1") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 1;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( !str_cmp( arg2,"2") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 2;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( !str_cmp( arg2,"3") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 3;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( !str_cmp( arg2,"4") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 4;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( !str_cmp( arg2,"5") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 5;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( !str_cmp( arg2,"6") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 6;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if (!str_cmp( arg2,"7") && IS_SET(obj->spectype, ADV_STARTED) && !IS_SET(obj->spectype, ADV_FINISHED) )
    {
                SET_BIT(obj->spectype, ADV_FINISHED);
                obj->toughness = 7;
                obj->points += 1;
                act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
                send_to_char("Ok.\n\r",ch);
                return;
    }
    else if ( str_cmp( arg2,"line") )
    {
        send_to_char("You can write a TITLE or a LINE.\n\r",ch);
        return;
    }
    else if ( obj->item_type == ITEM_BOOK )
    {
        send_to_char("You can only write a title on the book.\n\r",ch);
        return;
    }
    else if ( IS_SET(obj->spectype, ADV_FINISHED) )
            {
                send_to_char("It is already finished!\n\r", ch );
                return;
            }

    if ( obj->chpoweruse != NULL) 
       strcpy(buf,obj->chpoweruse );
	 else return;

    if ( !str_cmp(buf,"(null)") )
    {
	arg3[0] = UPPER(arg3[0]);
	free_string( obj->chpoweruse );
	obj->chpoweruse = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);

      if (!str_cmp(arg3,"start.damage.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_DAMAGE);}
      else if (!str_cmp(arg3,"start.super.damage.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_SUPER_DAMAGE);}
      else if (!str_cmp(arg3,"start.ultra.damage.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_ULTRA_DAMAGE);}
	else if (!str_cmp(arg3,"start.affect.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_AFFECT);}
	else if (!str_cmp(arg3,"start.action.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_ACTION);}
	else if (!str_cmp(arg3,"start.spell") && obj->spectype == 0)
	    SET_BIT(obj->spectype, ADV_STARTED);
	else if (!IS_SET(obj->spectype, ADV_FINISHED))
	    SET_BIT(obj->spectype, ADV_FAILED);
	return;
    }

    if ( obj->chpoweruse != NULL && buf[0] != '\0' )
    {
	if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	{
	    send_to_char("Line too long.\n\r",ch);
	    return;
	}
	else
	{
	    free_string( obj->chpoweruse );
	    strcat( buf, "\n\r" );
	    strcat( buf, arg3 );
	    obj->chpoweruse = str_dup( buf );

	    argument = one_argument( argument, arg1 );
	    strcpy( arg2, argument );

	    if (!str_cmp(arg1,"start.damage.spell") && obj->spectype == 0)
		{SET_BIT(obj->spectype, ADV_STARTED);
		SET_BIT(obj->spectype, ADV_DAMAGE);}
            else if (!str_cmp(arg3,"start.super.damage.spell") && obj->spectype == 0)
	      {SET_BIT(obj->spectype, ADV_STARTED);
	       SET_BIT(obj->spectype, ADV_SUPER_DAMAGE);}
           else if (!str_cmp(arg3,"start.ultra.damage.spell") && obj->spectype == 0)
            {SET_BIT(obj->spectype, ADV_STARTED);
	       SET_BIT(obj->spectype, ADV_ULTRA_DAMAGE);}
	    else if (!str_cmp(arg1,"start.affect.spell") && obj->spectype == 0)
		{SET_BIT(obj->spectype, ADV_STARTED);
		SET_BIT(obj->spectype, ADV_AFFECT);}
	    else if (!str_cmp(arg1,"start.action.spell") && obj->spectype == 0)
		{SET_BIT(obj->spectype, ADV_STARTED);
		SET_BIT(obj->spectype, ADV_ACTION);}
	    else if (!str_cmp(arg1,"start.spell") && obj->spectype == 0)
		SET_BIT(obj->spectype, ADV_STARTED);
	    else if (!str_cmp(arg1,"damage.spell") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_ACTION)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_DAMAGE);
	    else if (!str_cmp(arg1,"affect.spell") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_ACTION)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_AFFECT);
	    else if (!str_cmp(arg1,"action.spell") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_ACTION)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_AFFECT);
	    else if (!str_cmp(arg1,"area.affect") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_AREA_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_AREA_AFFECT);obj->points += 100;}
	    else if (!str_cmp(arg1,"victim.target") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_VICTIM_TARGET)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_VICTIM_TARGET);obj->points += 5;}
	    else if (!str_cmp(arg1,"object.target") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_OBJECT_TARGET)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_OBJECT_TARGET);obj->points += 5;}
	    else if (!str_cmp(arg1,"global.target") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_GLOBAL_TARGET)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_GLOBAL_TARGET);obj->points += 50;}
	    else if (!str_cmp(arg1,"next.page") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_NEXT_PAGE)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_NEXT_PAGE);obj->points += 5;}
	    else if (!str_cmp(arg1,"parameter:") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_PARAMETER)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_PARAMETER);
		    free_string(obj->chpoweron);
		    obj->chpoweron = str_dup(arg2);
		}
	    }
	    else if (!str_cmp(arg1,"spell.first") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_SPELL_FIRST)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_SPELL_FIRST);
	    else if (!str_cmp(arg1,"not.caster") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_NOT_CASTER)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_NOT_CASTER);
	    else if (!str_cmp(arg1,"no.players") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_NO_PLAYERS)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_NO_PLAYERS);
	    else if (!str_cmp(arg1,"second.victim") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_SECOND_VICTIM)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_SECOND_VICTIM);obj->points += 5;}
	    else if (!str_cmp(arg1,"second.object") 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_SECOND_OBJECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_SECOND_OBJECT);obj->points += 5;}
	    else if (!str_cmp(arg1,"reversed")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_REVERSED)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_REVERSED);
	    else if (!str_cmp(arg1,"min.damage:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& ( IS_SET(obj->spectype, ADV_DAMAGE) || IS_SET(obj->spectype, ADV_SUPER_DAMAGE) 
                 || IS_SET(obj->spectype, ADV_ULTRA_DAMAGE) )
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) || 
		    atoi(arg2) < 0 || atoi(arg2) > 1000)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[1] = atoi(arg2);obj->points += (atoi(arg2)*0.25);}
	    }
	    else if (!str_cmp(arg1,"max.damage:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& ( IS_SET(obj->spectype, ADV_DAMAGE) || IS_SET(obj->spectype, ADV_SUPER_DAMAGE) 
                 || IS_SET(obj->spectype, ADV_ULTRA_DAMAGE) )
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) || 
		    atoi(arg2) < 0 || atoi(arg2) > 2000)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[2] = atoi(arg2);obj->points += (atoi(arg2)*0.25);}
	    }
	    else if (!str_cmp(arg1,"move")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_ACTION)
		&& obj->value[1] == ACTION_NONE
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{obj->value[1] = ACTION_MOVE;obj->points += 500;}
	    else if (!str_cmp(arg1,"mob:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_ACTION)
		&& obj->value[1] == ACTION_NONE
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		obj->value[1] = ACTION_MOB;
		if ( arg2[0] == '\0' ||
		    ( mobile = get_char_room( ch, arg2 ) ) == NULL)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[2] = mobile->pIndexData->vnum;obj->points += 500;}
	    }
	    else if (!str_cmp(arg1,"object:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_ACTION)
		&& obj->value[1] == ACTION_NONE
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		obj->value[1] = ACTION_OBJECT;
		if ( arg2[0] == '\0' ||
		    ( object = get_obj_world2( ch, arg2 ) ) == NULL)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else if ( IS_SET(object->quest, QUEST_ARTIFACT) || IS_SET(object->quest, ITEM_DTOKEN)
                      || IS_SET(object->quest, ITEM_QUEST)
                      || ( obj->pIndexData->vnum > 89 && obj->pIndexData->vnum < 93 ) )
		    SET_BIT(object->spectype, ADV_FAILED);
		else {obj->value[2] = object->pIndexData->vnum;obj->points += 500;}
	    }
	    else if (!str_cmp(arg1,"apply:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else if ((!str_cmp(arg2,"strength") || !str_cmp(arg2,"str")) && 
		    !IS_SET(obj->value[1], ADV_STR))
		    SET_BIT(obj->value[1], ADV_STR);
		else if ((!str_cmp(arg2,"dexterity") || !str_cmp(arg2,"dex")) && 
		    !IS_SET(obj->value[1], ADV_DEX))
		    SET_BIT(obj->value[1], ADV_DEX);
		else if ((!str_cmp(arg2,"mind") || !str_cmp(arg2,"min")) && 
		    !IS_SET(obj->value[1], ADV_INT))
		    SET_BIT(obj->value[1], ADV_INT);
		else if ((!str_cmp(arg2,"spirit") || !str_cmp(arg2,"spi")) && 
		    !IS_SET(obj->value[1], ADV_WIS))
		    SET_BIT(obj->value[1], ADV_WIS);
		else if ((!str_cmp(arg2,"vitality") || !str_cmp(arg2,"vit")) && 
		    !IS_SET(obj->value[1], ADV_CON))
		    SET_BIT(obj->value[1], ADV_CON);
		else if (!str_cmp(arg2,"mana") && 
		    !IS_SET(obj->value[1], ADV_MANA))
		    SET_BIT(obj->value[1], ADV_MANA);
		else if ((!str_cmp(arg2,"hp") || !str_cmp(arg2,"hits") || 
		    !str_cmp(arg2,"hitpoints")) && 
		    !IS_SET(obj->value[1], ADV_HIT))
		    SET_BIT(obj->value[1], ADV_HIT);
		else if ((!str_cmp(arg2,"move") || !str_cmp(arg2,"movement")) && 
		    !IS_SET(obj->value[1], ADV_MOVE))
		    SET_BIT(obj->value[1], ADV_MOVE);
		else if ((!str_cmp(arg2,"ac") || !str_cmp(arg2,"armour") ||
		    !str_cmp(arg2,"armor")) && 
		    !IS_SET(obj->value[1], ADV_AC))
		    SET_BIT(obj->value[1], ADV_AC);
		else if ( ( !str_cmp(arg2,"evasion") || !str_cmp(arg2,"eva") ) && 
		    !IS_SET(obj->value[1], ADV_EVA))
		    SET_BIT(obj->value[1], ADV_EVA);
		else if ( ( !str_cmp(arg2,"tension") || !str_cmp(arg2,"ten") ) && 
		    !IS_SET(obj->value[1], ADV_TEN))
		    SET_BIT(obj->value[1], ADV_TEN);
		else if ((!str_cmp(arg2,"agility") || !str_cmp(arg2,"agi") ) && 
		    !IS_SET(obj->value[1], ADV_AGI))
		    SET_BIT(obj->value[1], ADV_AGI);
		else if ((!str_cmp(arg2,"save") || !str_cmp(arg2,"save.spell") ||
		    !str_cmp(arg2,"save_spell")) && 
		    !IS_SET(obj->value[1], ADV_SAVING_SPELL))
		    SET_BIT(obj->value[1], ADV_SAVING_SPELL);
		else {SET_BIT(obj->spectype, ADV_FAILED);return;}
		obj->points += 25;
	    }
	    else if (!str_cmp(arg1,"affect:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else if ((!str_cmp(arg2,"blind") || !str_cmp(arg2,"blindness")) && 
		    !IS_SET(obj->value[3], AFF_BLIND))
		    SET_BIT(obj->value[3], AFF_BLIND);
		else if ((!str_cmp(arg2,"invis") || !str_cmp(arg2,"invisible")
		    || !str_cmp(arg2,"invisibility")) && 
		    !IS_SET(obj->value[3], AFF_BLIND))
		    SET_BIT(obj->value[3], AFF_BLIND);
		else if (!str_cmp(arg2,"detect.evil") && 
		    !IS_SET(obj->value[3], AFF_DETECT_EVIL))
		    SET_BIT(obj->value[3], AFF_DETECT_EVIL);
		else if ((!str_cmp(arg2,"detect.invis") || !str_cmp(arg2,"detect.invisible")
		    || !str_cmp(arg2,"detect.invisibility")) && 
		    !IS_SET(obj->value[3], AFF_DETECT_INVIS))
		    SET_BIT(obj->value[3], AFF_DETECT_INVIS);
		else if (!str_cmp(arg2,"detect.magic") && 
		    !IS_SET(obj->value[3], AFF_DETECT_MAGIC))
		    SET_BIT(obj->value[3], AFF_DETECT_MAGIC);
		else if (!str_cmp(arg2,"detect.hidden") && 
		    !IS_SET(obj->value[3], AFF_DETECT_HIDDEN))
		    SET_BIT(obj->value[3], AFF_DETECT_HIDDEN);
		else if ((!str_cmp(arg2,"shadowplane") || 
		    !str_cmp(arg2,"shadow.plane")) && 
		    !IS_SET(obj->value[3], AFF_SHADOWPLANE))
		    SET_BIT(obj->value[3], AFF_SHADOWPLANE);
		else if (!str_cmp(arg2,"faerie.fire") && 
		    !IS_SET(obj->value[3], AFF_FAERIE_FIRE))
		    SET_BIT(obj->value[3], AFF_FAERIE_FIRE);
		else if ((!str_cmp(arg2,"infravision") || 
		    !str_cmp(arg2,"infrared") || !str_cmp(arg2,"infra")) && 
		    !IS_SET(obj->value[3], AFF_SANCTUARY))
		    SET_BIT(obj->value[3], AFF_SANCTUARY);
		else if (!str_cmp(arg2,"curse") && 
		    !IS_SET(obj->value[3], AFF_CURSE))
		    SET_BIT(obj->value[3], AFF_CURSE);
		else if ((!str_cmp(arg2,"flaming") || !str_cmp(arg2,"burning"))
		    && !IS_SET(obj->value[3], AFF_FLAMING))
		    SET_BIT(obj->value[3], AFF_FLAMING);
		else if (!str_cmp(arg2,"poison") && 
		    !IS_SET(obj->value[3], AFF_POISON))
		    SET_BIT(obj->value[3], AFF_POISON);
		else if ((!str_cmp(arg2,"protect") || !str_cmp(arg2,"protection"))
		    && !IS_SET(obj->value[3], AFF_PROTECT))
		    SET_BIT(obj->value[3], AFF_PROTECT);
		else if ((!str_cmp(arg2,"sanctuary") || !str_cmp(arg2,"sanct"))
		    && !IS_SET(obj->value[3], AFF_SANCTUARY))
		    SET_BIT(obj->value[3], AFF_SANCTUARY);
		else if (!str_cmp(arg2,"ethereal") && 
		    !IS_SET(obj->value[3], AFF_ETHEREAL))
		    SET_BIT(obj->value[3], AFF_ETHEREAL);
		else if (!str_cmp(arg2,"sneak") && 
		    !IS_SET(obj->value[3], AFF_SNEAK))
		    SET_BIT(obj->value[3], AFF_SNEAK);
		else if (!str_cmp(arg2,"hide") && 
		    !IS_SET(obj->value[3], AFF_HIDE))
		    SET_BIT(obj->value[3], AFF_HIDE);
		else if (!str_cmp(arg2,"charm") && 
		    !IS_SET(obj->value[3], AFF_CHARM))
		    SET_BIT(obj->value[3], AFF_CHARM);
		else if ((!str_cmp(arg2,"fly") || !str_cmp(arg2,"flying"))
		    && !IS_SET(obj->value[3], AFF_FLYING))
		    SET_BIT(obj->value[3], AFF_FLYING);
		else if ((!str_cmp(arg2,"passdoor") || !str_cmp(arg2,"pass.door"))
		    && !IS_SET(obj->value[3], AFF_PASS_DOOR))
		    SET_BIT(obj->value[3], AFF_PASS_DOOR);
		else if ((!str_cmp(arg2,"shadowsight") || 
		    !str_cmp(arg2,"shadow.sight")) && 
		    !IS_SET(obj->value[3], AFF_SHADOWSIGHT))
		    SET_BIT(obj->value[3], AFF_SHADOWSIGHT);
		else if ((!str_cmp(arg2,"web") || !str_cmp(arg2,"webbed"))
		    && !IS_SET(obj->value[3], AFF_WEBBED))
		    SET_BIT(obj->value[3], AFF_WEBBED);
		else if (!str_cmp(arg2,"contraception") && 
		    !IS_SET(obj->value[3], AFF_CONTRACEPTION))
		    SET_BIT(obj->value[3], AFF_CONTRACEPTION);

		else {SET_BIT(obj->spectype, ADV_FAILED);return;}
		obj->points += 25;
	    }
	    else if (!str_cmp(arg1,"bonus:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) || 
		    atoi(arg2) < 0 || atoi(arg2) > 100)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[2] = atoi(arg2);obj->points += (atoi(arg2)*15);}
	    }
	    else if (!str_cmp(arg1,"duration:")
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& obj->level == 0
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) || 
		    atoi(arg2) < 1 || atoi(arg2) > 1000)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->level = atoi(arg2);obj->points += (atoi(arg2)*10);}
	    }
	    else if ((!str_cmp(arg1,"message.one:") || !str_cmp(arg1,"message.1:")) 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_MESSAGE_1)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_MESSAGE_1);
		    free_string(obj->chpoweroff);
		    obj->chpoweroff = str_dup(arg2);
		}
	    }
	    else if ((!str_cmp(arg1,"message.two:") || !str_cmp(arg1,"message.2:")) 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_MESSAGE_2)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_MESSAGE_2);
		    free_string(obj->victpoweron);
		    obj->victpoweron = str_dup(arg2);
		}
	    }
	    else if ((!str_cmp(arg1,"message.three:") || !str_cmp(arg1,"message.3:")) 
		&& IS_SET(obj->spectype, ADV_STARTED) 
		&& !IS_SET(obj->spectype, ADV_MESSAGE_3)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_MESSAGE_3);
		    free_string(obj->victpoweroff);
		    obj->victpoweroff = str_dup(arg2);
		}
	    }
	    else if (!IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_FAILED);
	}
    }
    else return;
    act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
    send_to_char("Ok.\n\r",ch);
}

void show_page( CHAR_DATA *ch, OBJ_DATA *book, int pnum, bool pagefalse )
{
    OBJ_DATA *page;
    OBJ_DATA *page_next;
    char buf[MAX_STRING_LENGTH];
    bool found = FALSE;

    for ( page = book->contains; page != NULL; page = page_next )
    {
	page_next = page->next_content;
	if (page->value[0] == pnum)
	{
	    found = TRUE;
	    if (!str_cmp(page->victpoweruse,"(null)"))
		sprintf(buf,"Untitled page.\n\r");
	    else
		sprintf(buf,"%s.\n\r",page->victpoweruse);
	    buf[0] = UPPER(buf[0]);
	    send_to_char(buf,ch);
	    if (!pagefalse)
	    {
		if (page->chpoweruse == NULL || page->chpoweruse == '\0' ||
			!str_cmp(page->chpoweruse,"(null)"))
		{
		    if (!are_runes(page))
			send_to_char("This page is blank.\n\r",ch);
		    else if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
			!IS_SET(page->quest, QUEST_MASTER_RUNE) &&
			!IS_SET(page->spectype, ADV_STARTED))
			show_runes(ch, page, FALSE);
		    else
			send_to_char("This page is blank.\n\r",ch);
		    return;
		}
		send_to_char("--------------------------------------------------------------------------------\n\r",ch);
		send_to_char(page->chpoweruse,ch);
		send_to_char("\n\r--------------------------------------------------------------------------------\n\r",ch);
		if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
		    !IS_SET(page->quest, QUEST_MASTER_RUNE) && 
		    !IS_SET(page->spectype, ADV_STARTED))
		    show_runes(ch, page, TRUE);
	    }
	}
    }
    if (!found)
	send_to_char("This page has been torn out.\n\r",ch);
}

void show_runes( CHAR_DATA *ch, OBJ_DATA *page, bool endline )
{
    if (page->value[1] + page->value[2] + page->value[3] < 1) return;
    send_to_char("This page contains the following symbols:\n\r",ch);
    send_to_char("Runes:",ch);
    if (page->value[1] > 0)
    {
	if (IS_SET(page->value[1], RUNE_FIRE  )) send_to_char(" Fire",  ch);
	if (IS_SET(page->value[1], RUNE_AIR   )) send_to_char(" Air",   ch);
	if (IS_SET(page->value[1], RUNE_EARTH )) send_to_char(" Earth", ch);
	if (IS_SET(page->value[1], RUNE_WATER )) send_to_char(" Water", ch);
	if (IS_SET(page->value[1], RUNE_DARK  )) send_to_char(" Dark",  ch);
	if (IS_SET(page->value[1], RUNE_LIGHT )) send_to_char(" Light", ch);
	if (IS_SET(page->value[1], RUNE_LIFE  )) send_to_char(" Life",  ch);
	if (IS_SET(page->value[1], RUNE_DEATH )) send_to_char(" Death", ch);
	if (IS_SET(page->value[1], RUNE_MIND  )) send_to_char(" Mind",  ch);
	if (IS_SET(page->value[1], RUNE_SPIRIT)) send_to_char(" Spirit",ch);
    }
    else send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    send_to_char("Glyphs:",ch);
    if (page->value[2] > 0)
    {
	if (IS_SET(page->value[2], GLYPH_CREATION)) send_to_char(" Creation", ch);
	if (IS_SET(page->value[2], GLYPH_DESTRUCTION)) send_to_char(" Destruction",  ch);
	if (IS_SET(page->value[2], GLYPH_SUMMONING)) send_to_char(" Summoning",ch);
	if (IS_SET(page->value[2], GLYPH_TRANSFORMATION)) send_to_char(" Transformation",ch);
	if (IS_SET(page->value[2], GLYPH_TRANSPORTATION)) send_to_char(" Transportation", ch);
	if (IS_SET(page->value[2], GLYPH_ENHANCEMENT)) send_to_char(" Enhancement",ch);
	if (IS_SET(page->value[2], GLYPH_REDUCTION )) send_to_char(" Reduction", ch);
	if (IS_SET(page->value[2], GLYPH_CONTROL)) send_to_char(" Control",ch);
	if (IS_SET(page->value[2], GLYPH_PROTECTION)) send_to_char(" Protection",ch);
	if (IS_SET(page->value[2], GLYPH_INFORMATION)) send_to_char(" Information",ch);
    }
    else send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    send_to_char("Sigils:",ch);
    if (page->value[3] > 0)
    {
	if (IS_SET(page->value[3], SIGIL_SELF      )) send_to_char(" Self", ch);
	if (IS_SET(page->value[3], SIGIL_TARGETING )) send_to_char(" Targeting", ch);
	if (IS_SET(page->value[3], SIGIL_AREA      )) send_to_char(" Area", ch);
	if (IS_SET(page->value[3], SIGIL_OBJECT    )) send_to_char(" Object", ch);
    }
    else send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    if (endline) send_to_char("--------------------------------------------------------------------------------\n\r",ch);
}

void do_elemental_touch( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int option;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS( ch, CLASS_WIZARD ) || ch->clannum != 1 ) 
     {
        stc("Huh?\n\r", ch );
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to give a weakness to?\n\r", ch );
        return;
     }
    
  if ( ch->position == POS_STANDING || arg[0] != '\0' )
     {
        if ( ( victim = get_char_area( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
     }

  if ( is_safe( ch, victim ) )
     return;

  if ( victim->elemental != 0 )
     {
        stc("They already have a weakness prescribed from magical forces magi.\n\r", ch );
        return;
     }

  do
  {
     switch( number_range( 1, 7 ) )
     {
        default:
        case 1: option = ELE_WFIRE; break;
        case 2: option = ELE_WEARTH; break;
        case 3: option = ELE_WTHUNDER; break;
        case 4: option = ELE_WDARK; break;
        case 5: option = ELE_WWATER; break;
        case 6: option = ELE_WHOLY; break;
        case 7: option = ELE_WLUNAR; break;
     }
  }
  while ( IS_SET( victim->elemental, option ) );

  SET_BIT( victim->elemental, option );
  ch->wait = 12;

  if ( IS_SET( victim->elemental, ELE_WWATER ) )
     {
        act("#L$N #Lnow fears the chilling water.#n",ch,NULL,victim,TO_CHAR);
        act("#LYou now fear the chilling water.#n",ch,NULL,victim,TO_VICT);
        act("#L$N #Lnow fears the chilling water.#n",ch,NULL,victim,TO_NOTVICT);
     }

  if ( IS_SET( victim->elemental, ELE_WFIRE ) )
     {
        act("#R$N #Rnow fears the blazing flames.#n",ch,NULL,victim,TO_CHAR);
        act("#RYou now fear the blazing flames.#n",ch,NULL,victim,TO_VICT);
        act("#R$N #Rnow fears the blazing flames.#n",ch,NULL,victim,TO_NOTVICT);
     }

  if ( IS_SET( victim->elemental, ELE_WTHUNDER ) )
     {
        act("#y$N #ynow fears the clapping thunder.#n",ch,NULL,victim,TO_CHAR);
        act("#yYou now fear the clapping thunder.#n",ch,NULL,victim,TO_VICT);
        act("#y$N #ynow fears the clapping thunder.#n",ch,NULL,victim,TO_NOTVICT);
     }

  if ( IS_SET( victim->elemental, ELE_WEARTH ) )
     {
        act("#g$N #gnow fears the harsh earth.#n",ch,NULL,victim,TO_CHAR);
        act("#gYou now fear the harsh earth.#n",ch,NULL,victim,TO_VICT);
        act("#g$N #gnow fears the harsh earth.#n",ch,NULL,victim,TO_NOTVICT);
     }

  if ( IS_SET( victim->elemental, ELE_WHOLY ) )
     {
        act("#w$N #wnow fears the sacred light.#n",ch,NULL,victim,TO_CHAR);
        act("#wYou now fear the sacred light.#n",ch,NULL,victim,TO_VICT);
        act("#w$N #wnow fears the sacred light.#n",ch,NULL,victim,TO_NOTVICT);
     }

  if ( IS_SET( victim->elemental, ELE_WDARK ) )
     {
        act("#e$N #enow fears the demonic void.#n",ch,NULL,victim,TO_CHAR);
        act("#eYou now fear the demonic void.#n",ch,NULL,victim,TO_VICT);
        act("#e$N #enow fears the demonic void.#n",ch,NULL,victim,TO_NOTVICT);
     }

  if ( IS_SET( victim->elemental, ELE_WLUNAR ) )
     {
        act("#o$N #onow fears the mysterious moon.#n",ch,NULL,victim,TO_CHAR);
        act("#oYou now fear the mysterious moon.#n",ch,NULL,victim,TO_VICT);
        act("#o$N #onow fears the mysterious moon.#n",ch,NULL,victim,TO_NOTVICT);
     }
}

void do_aureola( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_WIZARD ) || ch->clannum != 4 ) 
      {
         stc("Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 30 || ch->tier < 5 )
      {
         send_to_char("#wOpening the #Csealed #obook #wat this stage #R#zWILL#n #wkill you.#n\n\r",ch);
         return;
      }

   if ( ch->alignment < 750 )
      {
         stc("#wOnly the hands of #ygood #wcan use this holy weapon.#n\n\r",ch);
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;

   act( "You tighten the grip around your book as the room is suddenly fill with light.", ch, NULL, NULL, TO_CHAR );
   act( "$n tightens the grip around $s book as the room is suddenly fill with light.", ch, NULL, NULL, TO_ROOM );
   act( "Finding yourself admist the heavens, you hear soft gentle music in the background.", ch, NULL, NULL, TO_CHAR );
   act( "Finding yourself admist the heavens, you hear soft gentle music in the background.", ch, NULL, NULL, TO_ROOM );
   act( "A beam of light suddenly shines down upon you as your book starts glowing.", ch, NULL, NULL, TO_CHAR );
   act( "A beam of light suddenly shines down upon $n as $s book starts glowing.", ch, NULL, NULL, TO_ROOM );
   act( "You then spread your arms apart as the legendary book hovers before you...", ch, NULL, NULL, TO_CHAR );
   act( "$n then spreads $s arms apart as the legendary book hovers before you...", ch, NULL, NULL, TO_ROOM );
   act( "You grimace in pain as your body rapidly age before your very eyes.", ch, NULL, NULL, TO_CHAR );
   act( "$n grimaces in pain as $s body rapidly ages before $s very eyes.", ch, NULL, NULL, TO_ROOM );
   act( "Your long white hair blows in the wind as you pray softly to God.", ch, NULL, NULL, TO_CHAR );
   act( "$n's long white hair blows in the wind as $e prays softly to God.", ch, NULL, NULL, TO_ROOM );
   act( "The book of Aureola suddenly turns itself to a page with glowing symbols.", ch, NULL, NULL, TO_CHAR );
   act( "The book of Aureola suddenly turns itself to a page with glowing symbols.", ch, NULL, NULL, TO_ROOM );
   act( "A palish beam of yellow light suddenly shines upon on your foes.", ch, NULL, NULL, TO_CHAR );
   act( "A palish beam of yellow light suddenly shines upon on your foes.", ch, NULL, NULL, TO_ROOM );
   act( "Looking up, you see a small sparkle descends from the heavens.", ch, NULL, NULL, TO_CHAR );
   act( "Looking up, you see a small sparkle descends from the heavens.", ch, NULL, NULL, TO_ROOM );
   act( "As the sparkle gets closer, the room StArTs TREMBLING ViOlEnTlY!!!", ch, NULL, NULL, TO_CHAR );
   act( "As the sparkle gets closer, the room StArTs TREMBLING ViOlEnTlY!!!", ch, NULL, NULL, TO_ROOM );
   act( "You clench your fist saying '#wRepent!#n' as fear thunders the room!", ch, NULL, NULL, TO_CHAR );
   act( "$n clenches $s fist saying '#wRepent!#n' as fear thunders the room!", ch, NULL, NULL, TO_ROOM );

   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) )
         continue;

      act( "#wYour #oflesh #wseemingly #eevaporates #was the #C#zdeadly#n #ylight #wrips through it!!!#n", vch, NULL, NULL, TO_CHAR );
      act( "#w$n#w's #oflesh #wseemingly #eevaporates #was the #C#zdeadly#n #ylight #wrips through it!!!#n", vch, NULL, NULL, TO_ROOM );

      SET_BIT( vch->elemental, ELE_WHOLY );

      ch->ele_attack = ATTACK_HOLY;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BEAM;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 3000000, TRUE );
   }
}
