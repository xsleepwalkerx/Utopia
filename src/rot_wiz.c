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


void do_pset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char arg4 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;

    sprintf(buf,"%s: Pset %s",ch->name,argument);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    strcpy(arg4, argument);
    if ( arg1[0] == '\0' || arg2[0] == '\0'  )
    {
	send_to_char( "#7Syntax: pset <victim> <area> <field> <value>\n\r",ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "#7Area being one of:\n\r",
ch );
	send_to_char( "  #3silver gold gold+ weapon immune beast\n\r",
ch );
	send_to_char( "  #3blue red yellow green purple \n\r",	        ch
);
	send_to_char( "\n\r",						ch );
	send_to_char( "#7Field being one of:\n\r",			ch
);
	send_to_char( "#3Weapon:  slice stab slash whip claw blast\n\r", ch );
        send_to_char( "#3Weapon:  pound crush grep bite pierce suck \n\r",
ch );
	send_to_char( "#3Immune:  slash stab smash animal misc charm\n\r",
ch ); 
        send_to_char( "#3Immune:  heat cold acid summon voodoo \n\r",
ch );
	send_to_char( "#3Immune:  hurl backstab shielded kick disarm\n\r",
ch );
        send_to_char( "#3Immune:  steal sleep drain sunlight\n\r",
ch );
        send_to_char( "#3         all#n\n\r",          ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */

    if ( !str_cmp( arg2, "bitch"))
    {
	 if(victim->flag4==1)
	{
	victim->flag4=0;
	send_to_char( "Whore removed.\n\r", ch);
	}
	else
	{
	victim->flag4=1;
	send_to_char( "Whore set!\n\r", ch);
	send_to_char( "You bitch!\n\r", victim);
	}
return;
    }
	


    if ( !str_cmp( arg2, "beast" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Beast range is 0 to 100.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->beast = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "quest" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value > 999999 )
	{
	    send_to_char( "Silver Coin range is 1 to 999999.\n\r", ch );
	    return;
	}

	victim->pcdata->quest = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "gold" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value > 500000000 )
	{
	    send_to_char( "Gold Coin range is 1 to 500000000.\n\r", ch );
	    return;
	}

	victim->gold = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }


    if ( !str_cmp( arg2, "gold+" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value > 50000 )
	{
	    send_to_char( "Gold Coin range is 1 to 50000.\n\r", ch );
	    return;
	}

	victim->gold = UMIN( victim->gold + value, 50000 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "purple" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}


	if (IS_JUDGE(ch)) {
		victim->spl[PURPLE_MAGIC] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }


    if ( !str_cmp( arg2, "spells" ) )
    {
	if (IS_NPC(victim))
	return;

	if (!IS_CREATOR(ch))
	{
	  do_pset(ch,"");
	  return;
	}

	if (value > 32000 || value < -32000)
	    return;

	victim->spl[RED_MAGIC]    = value;
	victim->spl[PURPLE_MAGIC] = value;
	victim->spl[GREEN_MAGIC]  = value;
	victim->spl[BLUE_MAGIC]   = value;
	victim->spl[YELLOW_MAGIC] = value;
	
	return;

     }

    if ( !str_cmp( arg2, "red" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->spl[RED_MAGIC] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "blue" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->spl[BLUE_MAGIC] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "green" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->spl[GREEN_MAGIC] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "yellow" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->spl[YELLOW_MAGIC] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "immune" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

        if (arg3 == '\0')
        {
	    send_to_char( "pset <victim> immune <immunity>.\n\r", ch );
	    return;         
        }

	if (IS_JUDGE(ch)) 
        {
                if (!str_cmp(arg3, "voodoo"))
                {
                  if (IS_SET(victim->immune, IMM_VOODOO))
                  {
		    REMOVE_BIT(victim->immune, IMM_VOODOO);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_VOODOO);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "slash"))
                {
                  if (IS_SET(victim->immune, IMM_SLASH))
                  {
		    REMOVE_BIT(victim->immune, IMM_SLASH);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SLASH);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "stab"))
                {
                  if (IS_SET(victim->immune, IMM_STAB))
                  {
		    REMOVE_BIT(victim->immune, IMM_STAB);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_STAB);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "smash"))
                {
                  if (IS_SET(victim->immune, IMM_SMASH))
                  {
		    REMOVE_BIT(victim->immune, IMM_SMASH);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SMASH);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "anmial"))
                {
                  if (IS_SET(victim->immune, IMM_ANIMAL))
                  {
		    REMOVE_BIT(victim->immune, IMM_ANIMAL);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_ANIMAL);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "misc"))
                {
                  if (IS_SET(victim->immune, IMM_MISC))
                  {
		    REMOVE_BIT(victim->immune, IMM_MISC);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_MISC);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "charm"))
                {
                  if (IS_SET(victim->immune, IMM_CHARM))
                  {
		    REMOVE_BIT(victim->immune, IMM_CHARM);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_CHARM);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }
                if (!str_cmp(arg3, "heat"))
                {
                  if (IS_SET(victim->immune, IMM_HEAT))
                  {
		    REMOVE_BIT(victim->immune, IMM_HEAT);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_HEAT);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }
                if (!str_cmp(arg3, "cold"))
                {
                  if (IS_SET(victim->immune, IMM_COLD))
                  {
		    REMOVE_BIT(victim->immune, IMM_COLD);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_COLD);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "lightning"))
                {
                  if (IS_SET(victim->immune, IMM_LIGHTNING))
                  {
		    REMOVE_BIT(victim->immune, IMM_LIGHTNING);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_LIGHTNING);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "acid"))
                {
                  if (IS_SET(victim->immune, IMM_ACID))
                  {
		    REMOVE_BIT(victim->immune, IMM_ACID);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_ACID);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "shield"))
                {
                  if (IS_SET(victim->immune, IMM_SHIELDED))
                  {
		    REMOVE_BIT(victim->immune, IMM_SHIELDED);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SHIELDED);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "hurl"))
                {
                  if (IS_SET(victim->immune, IMM_HURL))
                  {
		    REMOVE_BIT(victim->immune, IMM_HURL);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_HURL);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "backstab"))
                {
                  if (IS_SET(victim->immune, IMM_BACKSTAB))
                  {
		    REMOVE_BIT(victim->immune, IMM_BACKSTAB);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_BACKSTAB);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }
		
                if (!str_cmp(arg3, "kick"))
                {
                  if (IS_SET(victim->immune, IMM_KICK))
                  {
		    REMOVE_BIT(victim->immune, IMM_KICK);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_KICK);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "disarm"))
                {
                  if (IS_SET(victim->immune, IMM_DISARM))
                  {
		    REMOVE_BIT(victim->immune, IMM_DISARM);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_DISARM);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "steal"))
                {
                  if (IS_SET(victim->immune, IMM_STEAL))
                  {
		    REMOVE_BIT(victim->immune, IMM_STEAL);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_STEAL);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "sleep"))
                {
                  if (IS_SET(victim->immune, IMM_SLEEP))
                  {
		    REMOVE_BIT(victim->immune, IMM_SLEEP);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SLEEP);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "sunlight"))
                {
                  if (IS_SET(victim->immune, IMM_SUNLIGHT))
                  {
		    REMOVE_BIT(victim->immune, IMM_SUNLIGHT);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SUNLIGHT);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "all"))
                {
		    SET_BIT(victim->immune, IMM_DRAIN);
		    SET_BIT(victim->immune, IMM_VOODOO);
		    SET_BIT(victim->immune, IMM_SLASH);
		    SET_BIT(victim->immune, IMM_STAB);
		    SET_BIT(victim->immune, IMM_SMASH);
		    SET_BIT(victim->immune, IMM_ANIMAL);
		    SET_BIT(victim->immune, IMM_MISC);
		    SET_BIT(victim->immune, IMM_CHARM);
		    SET_BIT(victim->immune, IMM_HEAT);
		    SET_BIT(victim->immune, IMM_COLD);
		    SET_BIT(victim->immune, IMM_LIGHTNING);
		    SET_BIT(victim->immune, IMM_ACID);
		    SET_BIT(victim->immune, IMM_HURL);
		    SET_BIT(victim->immune, IMM_BACKSTAB);
		    SET_BIT(victim->immune, IMM_KICK);
		    SET_BIT(victim->immune, IMM_DISARM);
		    SET_BIT(victim->immune, IMM_STEAL);
		    SET_BIT(victim->immune, IMM_SLEEP);
		    send_to_char("Ok All Immunities Added.\n\r",ch);
                    return;
                }

                send_to_char("No such immunity exists.\n\r",ch);
                return;
        }
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "weapon" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

        argument = one_argument( argument, arg4 );
        /*
         * Snarf the value (which need not be numeric).
         */
        value = is_number( arg4 ) ? atoi( arg4 ) : -1;

	if (!IS_CREATOR(ch))
	{
	if ( value < 0 || value > 1000 )
	{
	    send_to_char( "Weapon skill range is 0 to 1000.\n\r", ch );
	    return;
	}
	}

	if (IS_JUDGE(ch)) 
        {
                if (!str_cmp(arg3, "unarmed"))
                {
		  victim->wpn[0] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "slice"))
                {
		  victim->wpn[1] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "stab"))
                {
		  victim->wpn[2] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "slash"))
                {
		  victim->wpn[3] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "whip"))
                {
		  victim->wpn[4] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "claw"))
                {
		  victim->wpn[5] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "blast"))
                {
		  victim->wpn[6] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "pound"))
                {
		  victim->wpn[7] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "crush"))
                {
		  victim->wpn[8] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "grep"))
                {
		  victim->wpn[9] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "bite"))
                {
		  victim->wpn[10] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "pierce"))
                {
		  victim->wpn[11] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "suck"))
                {
		  victim->wpn[12] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "all"))
                {
		  victim->wpn[0] = value;
		  victim->wpn[1] = value;
		  victim->wpn[2] = value;
		  victim->wpn[3] = value;
		  victim->wpn[4] = value;
		  victim->wpn[5] = value;
		  victim->wpn[6] = value;
		  victim->wpn[7] = value;
		  victim->wpn[8] = value;
		  victim->wpn[8] = value;
		  victim->wpn[9] = value;
		  victim->wpn[10] = value;
                  victim->wpn[11] = value;
		  victim->wpn[12] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }
                send_to_char("No such weapon skill exists.\n\r",ch);
                return;
        }
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    /*
     * Generate usage message.
     */
    do_pset( ch, "" );
    return;
}

void do_paradox( CHAR_DATA *ch, char *argument)  {
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
        DESCRIPTOR_DATA *d;        
        argument = one_argument(argument, arg);        

	if (arg[0] == '\0') {
		send_to_char("Paradox whom?\n\r", ch );
		return;}

    if ( get_trust(ch) >=  MAX_LEVEL - 2 && !str_cmp(arg,"all"))
    {
        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;
                
          
            { paradox(victim); }
        }
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
	paradox(victim);
	send_to_char("Done!\n\r", ch );
}


void paradox( CHAR_DATA *ch )
{
    char            buf[MAX_STRING_LENGTH]; 
    ROOM_INDEX_DATA *location;

    if ( is_sound( ch ) )
       stc("!!SOUND(thunder.wav V=100 L=1 P=90)\n\r",ch);

    send_to_char("The sins of your past strike back!\n\r",  ch);
    send_to_char("The paradox has come for your soul!\n\r",  ch);
    sprintf( buf, "#w%s #gis struck by the #wdivine #ybolt#g!#n", ch->name);
    do_info(ch, buf);

    SET_BIT(ch->extra, TIED_UP);
    SET_BIT(ch->extra, GAGGED);
    SET_BIT(ch->extra, BLINDFOLDED);

    if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->pkill > 19 )
       ch->hit = -1000010;
       else ch->hit     =  -10;

    ch->move = ch->mana = 0;
    ch->position = POS_MORTAL;

    if ( ( location = get_room_index( 800 ) ) == NULL )
       return;

    if ( ch->in_room == location )
	return;

    char_from_room( ch );
    char_to_room( ch, location );
}

