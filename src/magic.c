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


/*
 * Local functions.
 */
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );
void	improve_spl	args( ( CHAR_DATA *ch, int dtype, int sn ) );


bool can_heal( CHAR_DATA *ch, CHAR_DATA *victim )
{
   if ( ch == victim )
      return TRUE;

   if ( !IS_NPC(ch) && !IS_NPC(victim) && (get_age(ch)-17) < 2 )
      {
         stc("But you are a newbie.\n\r",ch);
         return FALSE;
      }

   if ( ch->safe_timer > 0 )
      {
         stc("You can't heal other people until your safe timer is gone.\n\r",ch);
         return FALSE;
      }

   if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 )
      {
         if ( ch->strike_timer > 0 )
            return TRUE;

         if ( !is_same_group( ch, victim ) )
            return FALSE;
      }

   if ( !IS_NPC(ch) && !IS_NPC(victim) && ( ch->pcdata->kingdom != victim->pcdata->kingdom ||
        ch->pcdata->kingdom == 0 ) )
      {
         stc("But they aren't in the same kingdom as you.\n\r",ch);
         return FALSE;
      }

   if ( armageddon == TRUE || mysafe != 0 )
      return TRUE;

   if ( IS_AFFECTED(ch, AFF_ETHEREAL) && !IS_AFFECTED(victim, AFF_ETHEREAL) )
      {
         stc("You are too insubstantial to heal them.\n\r",ch);
         return FALSE;
      }

   if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE) )
      {
         stc("You are too insubstantial to heal them.\n\r",ch);
         return FALSE;
      }

   if ( !IS_AFFECTED(ch, AFF_ETHEREAL) && IS_AFFECTED(victim, AFF_ETHEREAL) )
      {
         stc("They are too insubstantial to be healed by you.\n\r",ch);
         return FALSE;
      }

   if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE) )
      {
         stc("They are too insubstantial to be healed by you.\n\r",ch);
         return FALSE;
      }

   if ( nogang != 0 || victim->attacker != NULL )
      {
         stc("You can't heal others when group battles are off.\n\r",ch);
         return FALSE;
      }

   return TRUE;
}

/*
 * Improve ability at a certain spell type.  KaVir.
 */
 

void improve_spl( CHAR_DATA *ch, int dtype, int sn )
{
    char buf[MAX_INPUT_LENGTH];
    char bufskill[MAX_INPUT_LENGTH];
    char buftype[MAX_INPUT_LENGTH];

         if (dtype == 0 ) sprintf(buftype,"tempest");
    else if (dtype == 1 ) sprintf(buftype,"red");
    else if (dtype == 2 ) sprintf(buftype,"white");
    else if (dtype == 3 ) sprintf(buftype,"green");
    else if (dtype == 4 ) sprintf(buftype,"blue");
    else if (dtype == 5 ) sprintf(buftype,"orange");
    else if (dtype == 6 ) sprintf(buftype,"indigo");
    else if (dtype == 7 ) sprintf(buftype,"violet");
    else return;

       if ( IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_NECROMANCER) || IS_CLASS(ch, CLASS_SUMMONER)
            || IS_CLASS(ch, CLASS_WIZARD) || IS_CLASS(ch, CLASS_SORCEROR) )
       {
          if ( ch->spl[dtype] >= 700 )
             return;

          ch->spl[dtype] = 700;
       }
       else if ( IS_CLASS(ch, CLASS_DROW) )
               {
                  if ( ch->spl[dtype] >= 500 )
                     return;
  
                  ch->spl[dtype] = 500;
               }
       else
       {
          if ( ch->spl[dtype] >= 200 )
             return;

          ch->spl[dtype] = 200;
       }

         if (ch->spl[dtype] == 1  ) sprintf(bufskill,"an apprentice of");
    else if (ch->spl[dtype] == 26 ) sprintf(bufskill,"a student at");
    else if (ch->spl[dtype] == 51 ) sprintf(bufskill,"a scholar at");
    else if (ch->spl[dtype] == 76 ) sprintf(bufskill,"a magus at");
    else if (ch->spl[dtype] == 101) sprintf(bufskill,"an adept at");
    else if (ch->spl[dtype] == 126) sprintf(bufskill,"a mage at");
    else if (ch->spl[dtype] == 151) sprintf(bufskill,"a warlock at");
    else if (ch->spl[dtype] == 200) sprintf(bufskill,"a master wizard at");
    else if (ch->spl[dtype] == 500) sprintf(bufskill,"a grand sorcerer at");
    else if (ch->spl[dtype] == 700) sprintf(bufskill,"the complete master of");
    else return;
    sprintf(buf,"You are now %s %s magic.\n\r",bufskill,buftype);
    ADD_COLOUR(ch,buf,WHITE);
    send_to_char(buf,ch);
}

bool is_obj( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("Huh?\n\r",ch);
	return FALSE;
    }
    if ( obj->chobj == NULL || obj->chobj != ch )
    {
	send_to_char("Huh?\n\r",ch);
	return FALSE;
    }
    if (!IS_CLASS(ch,CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return FALSE;
    }
    if (!IS_DEMPOWER(ch, DEM_MAGIC))
    {
	send_to_char("You haven't been granted the gift of Magic.\n\r",ch);
	return FALSE;
    }
    return TRUE;
}

/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&   !str_prefix( name, skill_table[sn].name ) )
	    return sn;
    }

    return -1;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup( int slot )
{
    extern bool fBootDb;
    int sn;

    if ( slot <= 0 )
	return -1;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( slot == skill_table[sn].slot )
	    return sn;
    }

    if ( fBootDb )
    {
	bug( "Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA *ch, int sn )
{
    char buf  [MAX_STRING_LENGTH];
    char buf2 [MAX_STRING_LENGTH];
    char colour [MAX_STRING_LENGTH];
/*  CHAR_DATA *rch; */
    char *pName;
    int iSyl;
    int length;

    struct syl_type
    {
	char *	old;
	char *	new;
    };

    static const struct syl_type syl_table[] =
    {
	{ " ",		" "		},
	{ "ar",		"abra"		},
	{ "au",		"kada"		},
	{ "bless",	"fido"		},
	{ "blind",	"nose"		},
	{ "bur",	"mosa"		},
	{ "cu",		"judi"		},
	{ "de",		"oculo"		},
	{ "en",		"unso"		},
	{ "light",	"dies"		},
	{ "lo",		"hi"		},
	{ "mor",	"zak"		},
	{ "move",	"sido"		},
	{ "ness",	"lacri"		},
	{ "ning",	"illa"		},
	{ "per",	"duda"		},
	{ "ra",		"gru"		},
	{ "re",		"candus"	},
	{ "son",	"sabru"		},
	{ "tect",	"infra"		},
	{ "tri",	"cula"		},
	{ "ven",	"nofo"		},
	{ "a", "c" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "m" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
    };

    buf[0]	= '\0';
    for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, pName ) )
	    {
		strcat( buf, syl_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    sprintf( buf2, "$n utters the words, '%s'.", buf );
    sprintf( buf,  "$n utters the words, '%s'.", skill_table[sn].name );
    if ( skill_table[sn].target == 0 )
    {
      if ( is_sound( ch ) )
         stc("!!SOUND(tempest.wav V=100 L=1 P=30)\n\r",ch);

    	sprintf(colour,"#e$n's eyes glow bright tempest for a moment.#n");
    	act(colour,ch,NULL,NULL,TO_ROOM);
    	sprintf(colour,"#eYour eyes glow bright tempest for a moment.#n");
    	act(colour,ch,NULL,NULL,TO_CHAR);
    }
    else if ( skill_table[sn].target == 1 )
    {
      if ( is_sound( ch ) )
         stc("!!SOUND(red.wav V=100 L=1 P=30)\n\r",ch);

    	sprintf(colour,"$n's eyes glow bright red for a moment.");
    	ADD_COLOUR(ch,colour,L_RED);
    	act(colour,ch,NULL,NULL,TO_ROOM);
    	sprintf(colour,"Your eyes glow bright red for a moment.");
    	ADD_COLOUR(ch,colour,L_RED);
    	act(colour,ch,NULL,NULL,TO_CHAR);
    }
    else if ( skill_table[sn].target == 2 )
    {
      if ( is_sound( ch ) )
         stc("!!SOUND(white.wav V=100 L=1 P=30)\n\r",ch);

    	sprintf(colour,"#w$n's eyes glow bright white for a moment.#n");
    	act(colour,ch,NULL,NULL,TO_ROOM);
    	sprintf(colour,"#wYour eyes glow bright white for a moment.#n");
    	act(colour,ch,NULL,NULL,TO_CHAR);
    }
    else if ( skill_table[sn].target == 3 )
    {
      if ( is_sound( ch ) )
         stc("!!SOUND(green.wav V=100 L=1 P=30)\n\r",ch);

    	sprintf(colour,"$n's eyes glow bright green for a moment.");
    	ADD_COLOUR(ch,colour,L_GREEN);
    	act(colour,ch,NULL,NULL,TO_ROOM);
    	sprintf(colour,"Your eyes glow bright green for a moment.");
    	ADD_COLOUR(ch,colour,L_GREEN);
    	act(colour,ch,NULL,NULL,TO_CHAR);
    }
    else if ( skill_table[sn].target == 4 )
    {
      if ( is_sound( ch ) )
         stc("!!SOUND(blue.wav V=100 L=1 P=30)\n\r",ch);

    	sprintf(colour,"$n's eyes glow bright blue for a moment.");
    	ADD_COLOUR(ch,colour,L_BLUE);
    	act(colour,ch,NULL,NULL,TO_ROOM);
    	sprintf(colour,"Your eyes glow bright blue for a moment.");
    	ADD_COLOUR(ch,colour,L_BLUE);
    	act(colour,ch,NULL,NULL,TO_CHAR);
    }
    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim )
{
    int save;
    int tsave;

    if (!IS_NPC(victim))
    {
	tsave = ( victim->spl[0]+victim->spl[1]+victim->spl[2]+
		  victim->spl[3]+victim->spl[4] ) * 0.05;
    	save = 50 + ( tsave - level - victim->saving_throw ) * 5;
    }
    else
    	save = 50 + ( victim->level - level - victim->saving_throw ) * 5;
    save = URANGE( 15, save, 85 );
    return number_percent( ) < save;
}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int value = 0;
    int level = 1;

    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL )
	return;

    if ( IS_SET(ch->loc_hp[0],LOST_HEAD) )
       return;

    if ( ( IS_CLASS(ch,CLASS_DRAGON) ) && ( ch->dragonform > 0 ) )
       {
          if ( number_percent() > ch->pcdata->dragon_magic * 2 )
             {
                send_to_char("You lost your concentration.\n\r",ch);
                return;
             }
       }

    if ( !IS_SET( ch->cmbt[5], SKL2_MIME ) )
       {
           if ( !IS_CLASS( ch, CLASS_WIZARD ) && !IS_CLASS( ch, CLASS_NECROMANCER ) && IS_SET(ch->in_room->room_flags, ROOM_LUMICA) && number_percent() < 85 )
              {
                 stc("As you cast the spell, it vaporizes in the anti-magic field.\n\r",ch);
                 act("As $n casts a spell, it vaporizes in the anti-magic field.",ch,NULL,NULL,TO_ROOM);
                 ch->wait = 12;
                 return;
              }

          if ( !IS_CLASS( ch, CLASS_NECROMANCER ) && IS_SET(ch->more, MORE_HEX) )
             {
                 stc("You open your mouth to say something but a bunch of gibberish comes out.\n\r",ch);
                 return;
             }
       }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Cast which what where?\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( arg1 ) ) < 0 )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }
  
    mana = IS_NPC(ch) ? 0 : UMAX(
	skill_table[sn].min_mana,
	100 / ( 2 + (ch->level*12) - skill_table[sn].skill_level) );

    if ( IS_CLASS(ch, CLASS_DEMON) )
       mana = 0;  

    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WEREWOLF) && ch->gifts[OWL_TOTEM] > 3 )
       mana /= 3;

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
      
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Cast the spell on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

      if ( !IS_NPC(victim) && victim->safe_timer > 0 )
	    {
		send_to_char( "They have a safe timer.\n\r", ch );
		return;
	    }

	if ( ch == victim )
	    send_to_char( "Cast this on yourself? Ok...\n\r", ch );
          else if ( is_safe(ch,victim) )
                  return;

      if ( !IS_CLASS( ch, CLASS_NECROMANCER ) && !IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) )
         {
            if (ch == victim )
            {
                send_to_char("You lower your magical resistance....\n\r", ch);
            }
            else if (number_percent( ) <= victim->pcdata->stats[DROW_MAGIC])
            {
               send_to_char("Your spell does not affect them.\n\r", ch);
               return;
            }  
        }

	if ( !IS_NPC(ch) )
	{
	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own follower.\n\r",
		    ch );
		return;
	    }
	}

      if ( !IS_CLASS( ch, CLASS_NECROMANCER ) && is_affected( victim,skill_lookup("divine reflection") ) && victim != ch )
         victim = ch;

	vo = (void *) victim;
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( arg2, ch->name ) )
	{
	    send_to_char( "You cannot cast this spell on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the spell be cast upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	break;
    }

    if ( IS_CLASS(ch, CLASS_DRAGON) && ch->dragonform == 42 )
       mana = 0;

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "You don't have enough mana.\n\r", ch );
	return;
    }
      
    if ( !IS_SET( ch->cmbt[5], SKL2_MIME ) )
       say_spell( ch, sn );

    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_MONK) && ( skill_table[sn].slot > 633 && skill_table[sn].slot < 637 ) )
       {
          value = 100;
       }
       else if ( skill_table[sn].slot == 642 || skill_table[sn].slot == 633 )
       {
          value = 100;
       }
       else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_MONK) && skill_table[sn].slot == 616 )
       {
          value = 100;
       }
       else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_SHADOW) && skill_table[sn].slot == 650 )
       {
          value = 100;
       }
       else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WIZARD) && skill_table[sn].slot == 651 )
       {
          value = 100;
       }
       else if ( ( !IS_NPC(ch) ) && IS_CLASS(ch,CLASS_ANGEL) && ( skill_table[sn].slot == 638 ) )
       {
          value = 100;
       }
       else if ( ( !IS_NPC(ch) ) && IS_CLASS(ch,CLASS_ANGEL) && ( skill_table[sn].slot == 637 ) )
       {
          value = 100;
       }
       else if ( ( !IS_NPC(ch) ) && ( IS_CLASS(ch,CLASS_VAMPIRE) ) && ( skill_table[sn].slot == 653 ) )
       {
          value = 100;
       }
       else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_NECROMANCER) && skill_table[sn].slot > 616 && skill_table[sn].slot < 628 )
       {
          value = 100;
       }
	   else if ( ( !IS_NPC(ch) ) && ( IS_CLASS(ch,CLASS_DAYWALKER) ) && ( skill_table[sn].slot == 655 ) )
       {
          value = 100;
       }
	   else if ( ( !IS_NPC(ch) ) && ( IS_CLASS(ch,CLASS_DAYWALKER) ) && ( skill_table[sn].slot == 654 ) )
       {
          value = 100;
       }
       else if ( ch->level >= skill_table[sn].skill_level )
               value = 100;
       else value = ch->pcdata->learned[sn];

    if ( !IS_NPC(ch) && number_percent( ) > value )
    {
	send_to_char( "You lost your concentration.\n\r", ch );
	ch->mana -= mana / 2;
	improve_spl(ch,skill_table[sn].target,sn);
    }
    else
    {
	ch->mana -= mana;
	/* Check players ability at spell type for spell power...KaVir */
	if (IS_NPC(ch))
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo );
	 else
	{
          if ( !IS_NPC(ch) )
             level = UMAX( 1, ( level * ch->pcdata->perm_int / 4 ) );

	    (*skill_table[sn].spell_fun) ( sn, (ch->spl[skill_table[sn].target]*0.25*level), ch, vo );
	    improve_spl(ch,skill_table[sn].target,sn);
	}
    }

    if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE && ch->fighting != NULL && ch->fighting == ch )
       stop_fighting(ch,FALSE);

    if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }

    return;
}



/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;

    if ( sn <= 0 )
	return;

	if (IS_NPC(ch)) return;

    if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
    {
	bug( "Obj_cast_spell: bad sn %d.", sn );
	return;
    }

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( victim == NULL )
	    victim = ch->fighting;
	if ( victim == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	vo = (void *) victim;
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( victim == NULL )
	    victim = ch;
	vo = (void *) victim;
	break;

    case TAR_CHAR_SELF:
	vo = (void *) ch;
	break;

    case TAR_OBJ_INV:
	if ( obj == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	vo = (void *) obj;
	break;
    }

    target_name = "";
    (*skill_table[sn].spell_fun) ( sn, level, ch, vo );

    if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }

    return;
}



/*
 * Spell functions.
 */
void spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD)) return;

    dam = dice( level, 6 );
    if ( saves_spell( level, victim ) )
	dam /= 2;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_EARTH;

    if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
       dam = char_damcap( ch );

    hp = victim->hit;
    damage( ch, victim, dam, sn - 2000 );

    if ( !IS_CLASS( ch, CLASS_WIZARD ) && !IS_NPC(victim) && IS_IMMUNE(victim, IMM_ACID) )
	victim->hit = hp;

    return;
}


void spell_barbed_foliage( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    af.type      = sn;
    af.duration  = 200;
    af.modifier  = 3;
    af.location  = APPLY_CON;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act( "$n is shrouded in a trench of barbed foliage.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are shrouded in a trench of barbed foliage.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}

void spell_sun_aura( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, skill_lookup("sun aura") ) )
	return;

    af.type      = sn;
    af.duration  = 40;
    af.modifier  = 3;
    af.location  = APPLY_WIS;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act( "$n is shrouded within a beam of sunlight.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are shrouded within a beam of sunlight.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}

void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    af.type      = sn;
    af.duration  = 24;
    af.modifier  = -20;

    if ( IS_CLASS(ch, CLASS_DROW) )
       {
          af.modifier  = -20000;
          af.duration  = 1000;
       }
 
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is shrouded in a suit of translucent glowing armor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are shrouded in a suit of translucent glowing armor.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_icearmor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    af.type      = sn;
    af.duration  = 200;
    af.modifier  = level * -3000;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
}

void spell_gambler( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = ch;
    AFFECT_DATA af;
    int luc;

    if ( is_affected( victim, sn ) )
       return;

    if ( number_percent() > 50 )
       luc = 1;
       else luc = -1;

    act( "You flip a coin in the air as you pray for your success.", ch, NULL, NULL, TO_CHAR );
    act( "$n flips a coin in the air as $e prays for $s success.", ch, NULL, NULL, TO_ROOM );
    act( "You then catch it savagely with a tight grip.", ch, NULL, NULL, TO_CHAR );
    act( "$n then catches it savagely with a tight grip.", ch, NULL, NULL, TO_ROOM );

    if ( luc == 1 )
       {
          act( "Opening your hand, you see the portrait of a #wGoddess#n.", ch, NULL, NULL, TO_CHAR );
          act( "Opening $s hand, $n sees the portrait of a #wGoddess#n.", ch, NULL, NULL, TO_ROOM );
          act( "A soft gentle light emits down on you as you are blessed with good luck.", ch, NULL, NULL, TO_CHAR );
          act( "A soft gentle light emits down on $n as $e is blessed with good luck.", ch, NULL, NULL, TO_ROOM );
       }
       else {
               act( "Opening your hand, you see the portrait of the #eDevil#n.", ch, NULL, NULL, TO_CHAR );
               act( "Opening $s hand, $n sees the portrait of the #eDevil#n.", ch, NULL, NULL, TO_ROOM );
               act( "A harsh dark light emits down on you as you are blessed with bad luck.", ch, NULL, NULL, TO_CHAR );
               act( "A harsh dark light emits down on $n as $e is blessed with bad luck.", ch, NULL, NULL, TO_ROOM );
            }

    if ( ch->fight_timer == 0 )
       ch->wait = 18;

    luc *= number_range( 2, 5 );

    af.type      = sn;
    af.duration  = 10;
    af.modifier  = luc;
    af.location  = APPLY_LUCK;
    af.bitvector = 0;
    affect_to_char( victim, &af );
}

void spell_rflush( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
	AFFECT_DATA af4;
	AFFECT_DATA af5;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( is_affected( victim, sn ) )
	   return;

	act("You tilt your fingers apart as a royal flush of cards appear.",ch,NULL,NULL,TO_CHAR);
	act("$n tilts $s fingers apart as a royal flush of cards appear.",ch,NULL,NULL,TO_ROOM);
	act("You sneer at $N '#yFeel the curse of a winning ace...#n'",ch,NULL,victim,TO_CHAR);
	act("$n sneers at you '#yFeel the curse of a winning ace...#n'",ch,NULL,victim,TO_VICT);
	act("$n sneers at $N '#yFeel the curse of a winning ace...#n'",ch,NULL,victim,TO_NOTVICT);
	act("You scream '#yHai!#n' as you throw the cards into $N's face!",ch,NULL,victim,TO_CHAR);
	act("$n screams '#yHai!#n' as $e throws the cards into your face!",ch,NULL,victim,TO_VICT);
	act("$n screams '#yHai!#n' as $e throws the cards into $N's face!",ch,NULL,victim,TO_NOTVICT);

	af1.type	= sn;
	af1.location	= APPLY_STR;
	af1.modifier	= URANGE( 5, get_curr_str( victim ) * number_range( 10, 25 ) * 0.01, 15 ) * -1;
	af1.duration	= 5;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_DEX;
	af2.modifier	= URANGE( 5, get_curr_dex( victim ) * number_range( 10, 25 ) * 0.01, 15 ) * -1;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af3.type	= sn;
	af3.location	= APPLY_INT;
	af3.modifier	= URANGE( 5, get_curr_int( victim ) * number_range( 10, 25 ) * 0.01, 15 ) * -1;
	af3.duration	= 5;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);
	
	af4.type	= sn;
	af4.location	= APPLY_WIS;
	af4.modifier	= URANGE( 5, get_curr_wis( victim ) * number_range( 10, 25 ) * 0.01, 15 ) * -1;
      af4.duration	= 5;
	af4.bitvector	= 0;
	affect_to_char(victim, &af4);
	
	af5.type	= sn;
	af5.location	= APPLY_CON;
	af5.modifier	= URANGE( 5, get_curr_con( victim ) * number_range( 10, 25 ) * 0.01, 15 ) * -1;
	af5.duration	= 5;
	af5.bitvector	= 0;
	affect_to_char(victim, &af5);

	af5.type	= sn;
	af5.location	= APPLY_EVA;
	af5.modifier	= URANGE( 5, get_curr_eva( victim ) * number_range( 10, 25 ) * 0.01, 15 ) * -1;
	af5.duration	= 5;
	af5.bitvector	= 0;
	affect_to_char(victim, &af5);

	af5.type	= sn;
	af5.location	= APPLY_AGI;
	af5.modifier	= URANGE( 2, get_curr_agi( victim ) * number_range( 25, 50 ) * 0.01, 10 ) * -1;
	af5.duration	= 5;
	af5.bitvector	= 0;
	affect_to_char(victim, &af5);

	af5.type	= sn;
	af5.location	= APPLY_TENSION;
	af5.modifier	= URANGE( 3, get_curr_tens( victim ) * number_range( 25, 40 ) * 0.01, 11 ) * -1;
	af5.duration	= 5;
	af5.bitvector	= 0;
	affect_to_char(victim, &af5);

	act("The blast of contact sends you flying onto the ground in severe pain.",victim,NULL,NULL,TO_CHAR);
	act("The blast of contact sends $n flying onto the ground in severe pain.",victim,NULL,NULL,TO_ROOM);
}

void spell_llothbless( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
	AFFECT_DATA af4;
	AFFECT_DATA af5;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( is_affected( victim, sn ) )
	   return;

	act("You kneel on your knees and pray to Lloth.",ch,NULL,NULL,TO_CHAR);
      act("$n kneels on $s knees and prays to Lloth.",ch,NULL,NULL,TO_NOTVICT);

	af1.type	= sn;
	af1.location	= APPLY_AC;
	af1.modifier	= -2000;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_STR;
	af2.modifier	= 5;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af3.type	= sn;
	af3.location	= APPLY_HITROLL;
	af3.modifier	= 3000;
	af3.duration	= 1000;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);
	
	af4.type	= sn;
	af4.location	= APPLY_DAMROLL;
	af4.modifier	= 3000;
        af4.duration	= 1000;
	af4.bitvector	= 0;
	affect_to_char(victim, &af4);
	
	af5.type	= sn;
	af5.location	= APPLY_DEX;
	af5.modifier	= 5;
	af5.duration	= 1000;
	af5.bitvector	= 0;
	affect_to_char(victim, &af5);

	send_to_char("You are blessed by Lloth!\n\r", victim);
	act("$n is blessed by Lloth!",victim,NULL,NULL,TO_ROOM);
	return;
}

void spell_tide( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af3;
	AFFECT_DATA af4;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( is_affected( victim, sn ) )
	   return;

	act("You kneel on your knees and pray to Caine.",ch,NULL,NULL,TO_CHAR);
      act("$n kneels on his knees and prays to Caine.",ch,NULL,NULL,TO_NOTVICT);

	af1.type	= sn;
	af1.location	= APPLY_CON;
	af1.modifier	= 3;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af3.type	= sn;
	af3.location	= APPLY_DEX;
	af3.modifier	= 5;
	af3.duration	= 1000;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);
	
	af4.type	= sn;
	af4.location	= APPLY_STR;
	af4.modifier	= 3;
	af4.duration	= 1000;
	af4.bitvector	= 0;
	affect_to_char(victim, &af4);
	
	send_to_char("You are blessed by Caine as his blood flows through your veins!\n\r", victim);
	act("$n is blessed by Caine as his blood flow through $s veins!",victim,NULL,NULL,TO_ROOM);
}

void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = 6+level;
    af.location  = APPLY_HITROLL;
    af.modifier  = level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level / 8;
    affect_to_char( victim, &af );
    act( "$n is blessed.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You feel righteous.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_lunas_blessing( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) ) return;

    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_HITROLL;
    af.modifier  = 4000;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_DAMROLL;
    af.modifier  = 4000;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -100;
    affect_to_char( victim, &af );
    act( "$n is filled with Luna's Blessing.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You feel Luna's Blessing engulf you.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}


void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    char buf [MAX_INPUT_LENGTH];

    if ( IS_AFFECTED(victim, AFF_BLIND) || saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.location  = APPLY_HITROLL;

    if ( !IS_NPC(ch) && !IS_NPC(victim) )
       af.modifier  = -30000;
       else af.modifier  = -4;
    af.duration  = 1+level;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    send_to_char( "You are blinded!\n\r", victim );
    if (!IS_NPC(victim))
	sprintf(buf,"%s is blinded!\n\r",victim->name);
    else
	sprintf(buf,"%s is blinded!\n\r",victim->short_descr);
    send_to_char(buf,ch);
}



void spell_burning_hands( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 4,
	 6,  8, 10, 12,	14,	17, 20, 23, 26, 29,
	29, 29, 30, 30,	31,	31, 32, 32, 33, 33,
	34, 34, 35, 35,	36,	36, 37, 37, 38, 38,
	39, 39, 40, 40,	41,	41, 42, 42, 43, 43,
	44, 44, 45, 45,	46,	46, 47, 47, 48, 48,
	50, 60, 70, 80,	90,	100,125,150,175,200
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) )
    {
    	damage( ch, victim, (dam*2), sn - 2000 );
	hp = ((hp - victim->hit)/2) + victim->hit;
    }
    else
    	damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT) )
	victim->hit = hp;
    return;
}



void spell_call_lightning( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hp;

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You must be out of doors.\n\r", ch );
	return;
    }

    if ( weather_info.sky < SKY_RAINING )
    {
	send_to_char( "You need bad weather.\n\r", ch );
	return;
    }

    dam = dice(level/2, 8);

    send_to_char( "God's lightning strikes your foes!\n\r", ch );
    act( "$n calls God's lightning to strike $s foes!",
	ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
	    {
		if (IS_ITEMAFF(vch, ITEMA_SHOCKSHIELD)) continue;
    		if ( saves_spell( level, vch ) )
		    dam /= 2;
    		hp = vch->hit;
    		damage( ch, vch, dam, sn - 2000 );
    		if (!IS_NPC(vch) && IS_IMMUNE(vch, IMM_LIGHTNING) )
		    vch->hit = hp;
	    }
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area
	&&   IS_OUTSIDE(vch)
	&&   IS_AWAKE(vch) )
	    send_to_char( "Lightning flashes in the sky.\n\r", vch );
    }
    return;
}

void spell_dark_forge( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    return;

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );

    if ( IS_SET(obj->quest, QUEST_FORGE)
    ||   IS_SET(obj->quest, QUEST_ARTIFACT)
    ||   obj->chobj != NULL )
    {
	send_to_char("You are unable to empower this item.\n\r",ch);
	return;
    }

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= 400;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= 400;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_AC;
    paf->modifier	= -200;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    SET_BIT(obj->quest, QUEST_FORGE);
    act( "$p glows palish grey.", ch, obj, NULL, TO_CHAR );
    act( "$p glows palish grey.", ch, obj, NULL, TO_ROOM );
    ch->mana = 0;
}

void spell_create_zombie( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf2[MAX_STRING_LENGTH]; 
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int max = 2 + UMIN( 2, ch->pkill / 10 );

    if ( IS_NPC( ch ) )
       return;

    if ( IS_SET( ch->in_room->room_flags, ROOM_LUMICA ) )
       {
          stc("As you cast the spell, it vaporizes in the anti-magic field.\n\r",ch);
          act("As $n casts a spell, it vaporizes in the anti-magic field.",ch,NULL,NULL,TO_ROOM);
          ch->wait = 6;
          return;
       }

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );

    if ( IS_CLASS( ch, CLASS_WIZARD ) )
       max += 3;

    if ( ch->pcdata->followers > max )
       {
          send_to_char("You already have the maximum zombies you can controll.\n\r",ch);
          return;
       }

    victim=create_mobile( get_mob_index( 24 ) );
    sprintf(buf,"A blood-curling zombie");
    sprintf(buf2,"A blood-curling zombie is here.\n\r");
    free_string(victim->short_descr);
    victim->short_descr = str_dup(buf);
    free_string(victim->name);
    victim->name = str_dup(buf);
    free_string(victim->long_descr);
    victim->long_descr= str_dup(buf2);

    if ( IS_SET( ch->cmbt[7], SKL_KAKUSU ) )
       SET_BIT( victim->act, PLR_WIZINVIS );

    if ( !IS_SET(victim->extra, EXTRA_ZOMBIE) ) 
       SET_BIT(victim->extra, EXTRA_ZOMBIE); 

    char_to_room( victim, ch->in_room );

    act("You say, '#yRise servant, and do my bidding!#n",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yRise servant, and do my bidding!#n",ch,NULL,NULL,TO_ROOM);

    sprintf(buf, "%s clambers back up to its feet.\n\r",victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
 
    add_follower( victim, ch );
    ch->pcdata->followers++;

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    af.type      = skill_lookup("protection from evil");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PROTECT;
    affect_to_char( victim, &af );

    af.type      = skill_lookup("dark shield");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    victim->special = 0;
    victim->level = 999;

    SET_BIT( victim->itemaffect, ITEMA_RESISTANCE );
    SET_BIT( victim->act, ACT_NINJA );
    SET_BIT( victim->itemaffect, ITEMA_SPEED );

    victim->hitroll = number_range( 500000, 500000 + UMIN( 500000, ch->pkill * 25000 ) );
    victim->damroll = number_range( 500000, 500000 + UMIN( 500000, ch->pkill * 25000 ) );

    if ( IS_SET( ch->cmbt[7], SKL_HITRATE ) )
       SET_BIT( victim->cmbt[7], SKL_HITRATE );

    if ( IS_SET( ch->cmbt[7], SKL_DODGE ) )
       SET_BIT( victim->cmbt[7], SKL_DODGE );

    if ( IS_SET( ch->cmbt[7], SKL_CRITICAL ) )
       SET_BIT( victim->cmbt[7], SKL_CRITICAL );

    if ( IS_SET( ch->cmbt[7], SKL_RETORT ) )
       SET_BIT( victim->cmbt[7], SKL_RETORT );

    victim->hit = victim->max_hit = number_range( 4000000, 6000000 );

    if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
       SET_BIT( victim->affected_by, AFF_ETHEREAL );

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
       SET_BIT( victim->affected_by, AFF_SHADOWPLANE );
 
    victim->armor = -50000;
    do_group(ch,victim->name);
}

void spell_dark_kinesis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );

    if ( number_percent() < 30 )
       return;

    ch->wait = 12;

    if ( number_percent() < 90 )
       do_drop( victim, "all" );
       else {
               OBJ_DATA *obj;
 
               if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
                  if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL )
                     if ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL )
                        if ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL )
                           return;

               obj_from_char( obj );
               obj_to_char( obj, victim );
               ch->wait = 8;
               act("You alter the state of $N's weapon, causing $M to drop it.",ch,NULL,victim,TO_CHAR);
               act("$n's eyes glow dark black.",ch,NULL,NULL,TO_ROOM);
               act("$p shakes violently, causing you to drop it.",ch,obj,victim,TO_VICT);
            }       
}

void spell_soulstop( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( ch->ctimer[3] > 0 )
	 return;

    ch->ctimer[3] = 4;    

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );
    act( "You then push your hands at $N releasing a blast of Ki energy.", ch, NULL, victim, TO_CHAR );
    act( "$n then pushes $s hands at you releasing a blast of Ki energy.", ch, NULL, victim, TO_VICT );
    act( "$n then pushes $s hands at $N releasing a blast of Ki energy.", ch, NULL, victim, TO_NOTVICT );

    if ( !is_affected(victim, sn) )
       {
          af.type      = sn;
          af.duration  = 5;
          af.location  = APPLY_DEX;
          af.modifier  = -3;
          af.bitvector = 0;
          affect_to_char(victim, &af);
       }

    victim->noblock = UMAX( UMIN( 3, ch->pkill / 15 ), number_range( 1, 3 ) );
    stc("#PYour #ehands and legs feel like lead as you are now #wdefenseless#e...#n\n\r",victim);
    act("#P$n #eis now defenseless.#n",victim,NULL,NULL,TO_ROOM);
}

void spell_soulchill( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    ch->wait = 12;
    subtract_stock( ch, 200 );
    SET_BIT( ch->arena, AFF2_DM );

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );
    act( "You brace your body as a blast of cold air flies from within you.", ch, NULL, NULL, TO_CHAR );
    act( "$n brace $s body as a blast of cold air flies from within $m.", ch, NULL, NULL, TO_CHAR );
    act( "You then DrIvE your fist into the ground releasing freezing energy.", ch, NULL, NULL, TO_CHAR );
    act( "$n then DrIvEs $s fist into the ground releasing freezing energy.", ch, NULL, NULL, TO_ROOM );
    act( "A loud cackle fills the air as you are engulfed by a chilling beam!", victim, NULL, NULL, TO_CHAR );
    act( "A loud cackle fills the air as $n is engulfed by a chilling beam!", victim, NULL, NULL, TO_ROOM );

    if ( !is_affected(victim, sn) )
       {
          af.type      = sn;
          af.duration  = 5;
          af.location  = APPLY_CON;
          af.modifier  = -3;
          af.bitvector = 0;
          affect_to_char(victim, &af);
       }

    if ( ch->ctimer[15] == 0 )
       victim->nodamage = number_range( 1, 2 );

    stc("#PYour #estrength dwindles away before your eyes leaving you #wweak#e...#n\n\r",victim);
    act("#P$n #eis now offenseless.#n",victim,NULL,NULL,TO_ROOM);

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_ICE;
    ch->attack_var = BEAM;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 2000000, TRUE );
}

void spell_soulburn( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    return;

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );

    af.type      = sn;
    af.duration  = 5;
    af.location  = APPLY_STR;
    af.modifier  = -3;
    af.bitvector = 0;

    if ( is_affected(victim, sn) ) 
       affect_join( victim, &af );
	   else affect_to_char(victim, &af);

    stc("You feel yourself weakened...\n\r",victim);
    act("$n's body becomes weaker.\n\r",victim,NULL,NULL,TO_ROOM);
    ch->wait = 12;
}

void spell_soulblight( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );
    act( "You concentrate on $N's soul filling it with blight.", ch, NULL, victim, TO_CHAR );
    act( "$n concentrates on your soul filling it with blight.", ch, NULL, victim, TO_VICT );
    act( "$n concentrates on $N's soul filling it with blight.", ch, NULL, victim, TO_NOTVICT );

    if ( is_affected( victim, skill_lookup("darkside") ) )
       affect_strip( victim, skill_lookup("darkside") );

    hit_gain( ch, 100000 );
    ch->wait = 18;

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;

    if ( !IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
       hurt_person( ch, victim, 900000, TRUE );
       else hurt_person( ch, victim, 450000, TRUE );
}

void spell_soulbolt( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );
    act( "You release a demonic bolt of energy as it sears $N's soul.", ch, NULL, victim, TO_CHAR );
    act( "$n releases a demonic bolt of energy as it sears your soul.", ch, NULL, victim, TO_VICT );
    act( "$n releases a demonic bolt of energy as it sears $N's soul.", ch, NULL, victim, TO_NOTVICT );

    victim->move = UMAX( 0, victim->move - 300000 );
    ch->wait = 12;

    if ( is_affected( victim, skill_lookup("darkside") ) )
       affect_strip( victim, skill_lookup("darkside") );

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = DIRECT;

    if ( !IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
       hurt_person( ch, victim, 500000, TRUE );
       else hurt_person( ch, victim, 250000, TRUE );
}

void spell_dark_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( is_affected(victim, sn) )
	 return;

    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_WIS;
    af.modifier  = 4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( victim != ch )
       {
          act( "$n mumbles in an ancient language as $e swirls $s arms about $N.", ch, NULL, victim, TO_NOTVICT );
          act( "$n mumbles in an ancient language as $e swirls $s arms about you.", ch, NULL, victim, TO_VICT );
          act( "You heard faint demonic screams as swirling souls surrounds your body.", ch, NULL, victim, TO_VICT );
          act( "You mumble in the ancient language as you swirl your arms about $N.", ch, NULL, victim, TO_CHAR );
          act( "You heard faint demonic screams as swirling souls surrounds $n's body.", victim, NULL, NULL, TO_ROOM );
       }
       else {
               act( "$n mumbles in an ancient language as $e swirls $s arms about $m.", ch, NULL, NULL, TO_ROOM );
               act( "You heard faint demonic screams as swirling souls surrounds $n's body.", ch, NULL, NULL, TO_ROOM );
               act( "You mumble in the ancient language as you swirl your arms about yourself.", ch, NULL, NULL, TO_CHAR );
               act( "You heard faint demonic screams as swirling souls surrounds your body.", ch, NULL, NULL, TO_CHAR );
            }

    return;
}

/* Leighton - Necromancer's Dark Path */
void spell_darkpath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_INT;
    af.modifier  = 3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( victim == ch )
       {
           act( "$n mumbles in an ancient language as $e swirls $s arms about $m.", ch, NULL, NULL, TO_ROOM );
           act( "You mumble in the ancient language as you swirl your arms about yourself.", ch, NULL, NULL, TO_CHAR );
       }
       else {
                act( "You mumble in the ancient language as you swirl your arms about $N.", ch, NULL, victim, TO_CHAR );
                act( "$n mumbles in the ancient language as $e swirls $s arms about you.", ch, NULL, victim, TO_VICT );
                act( "$n mumbles in the ancient language as $e swirls $s arms about $N.", ch, NULL, victim, TO_NOTVICT );
            }

    act( "The way of the Ancients is suddenly made clear to you!", victim, NULL, NULL, TO_CHAR );
    act( "$n's body shimmers as $e is blessed with the knowledge of the Ancients!", victim, NULL, NULL, TO_ROOM );
}

void spell_haste( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, skill_lookup("haste") ) )
    {
	send_to_char( "That person is already moving faster than the eye can see.\n\r", ch );
      return;
    }

    if ( is_affected( victim,skill_lookup("slow") ) )
    {
        send_to_char( "You start moving faster than the eye can see.\n\r", victim );
        act( "$n starts moving faster than the eye can see.\n\r", victim, NULL, NULL, TO_ROOM );
	  affect_strip( victim, skill_lookup("slow") );
        return;
    }

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_NONE;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You start moving faster than the eye can see.\n\r", victim );
    act( "$n starts moving faster than the eye can see.\n\r", victim, NULL, NULL, TO_ROOM );
}

void spell_hyper( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    af.type = sn;
    af.duration  = 100;
    af.location  = APPLY_NONE;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "#RYour #ebody pulses with dark energy as you fade in and out of existence.#n\n\r", victim );
    act( "#R$n#R's #ebody pulses with dark energy as $e fades in and out of existence.#n\n\r", victim, NULL, NULL, TO_ROOM );
}

void spell_quicken( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, skill_lookup("quicken") ) )
    {
	send_to_char( "That person is already moving faster than the eye can see.\n\r", ch );
      return;
    }

    if ( IS_CLASS( victim, CLASS_WIZARD ) && victim->clannum == 6 )
    {
	send_to_char( "You feel so powerful that you don't see a need for this.\n\r", ch );
      return;
    }

    if ( is_affected( victim,skill_lookup("age") ) )
       affect_strip( victim, skill_lookup("age") );

    af.type      = sn;

    if ( IS_CLASS( victim, CLASS_WIZARD ) && victim->clannum == 3 )
       af.duration  = 100;
       else af.duration  = 10;

    af.location  = APPLY_NONE;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act( "$N starts moving so fast that $E fades in and out of existence.", ch, NULL, victim, TO_CHAR );
    act( "You start moving so fast that you fade in and out of existence.", ch, NULL, victim, TO_VICT );
    act( "$N starts moving so fast that $E fades in and out of existence.", ch, NULL, victim, TO_NOTVICT );
}

void spell_age( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
       return;

    if ( !is_affected( victim, skill_lookup("age") ) )
       {
          af.type      = sn;
          af.duration  = 1;
          af.location  = APPLY_DEX;
          af.modifier  = -40;
          af.bitvector = 0;
          affect_to_char( victim, &af );

          af.type      = sn;
          af.duration  = 1;
          af.location  = APPLY_EVA;
          af.modifier  = -40;
          af.bitvector = 0;
          affect_to_char( victim, &af );
       }

    act( "#CYou #wlaugh as you release your most #ypowerful #wspell at $N#w.#n\n\r", ch, NULL, victim, TO_CHAR );
    act( "#C$n #wlaughs as $e releases $s most #ypowerful #wspell at you.#n\n\r", ch, NULL, victim, TO_VICT );
    act( "#C$n #wlaughs as $e releases $s most #ypowerful #wspell at $N#w.#n\n\r", ch, NULL, victim, TO_NOTVICT );
    send_to_char( "#yYou #esuddenly feel very, very #Pold#e.#n\n\r", victim );
    act( "#y$n #esuddenly looks very, very #Pold#e.#n\n\r", victim, NULL, NULL, TO_ROOM );
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 3000000, TRUE );
}

void spell_wall( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( is_affected( ch, skill_lookup("magical wall") ) )
    {
	send_to_char( "You are already protected by magical shields of energy.\n\r", ch );
      return;
    }

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    send_to_char( "You grin as two magical shields start moving around your body.\n\r", ch );
    act( "$n's body is rapidly encircled by two shields of magical energy.\n\r", ch, NULL, NULL, TO_ROOM );
}

void spell_magi_wall( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    bool test = FALSE;

    if ( is_affected( ch, skill_lookup("magiwall") ) )
       return;

    if ( IS_SET(ch->more2, MORE2_SHIELD) )
       {
          test = TRUE;
          REMOVE_BIT(ch->more2, MORE2_SHIELD);
       }

    af.type      = sn;
    af.duration  = 3;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    if ( test == TRUE )
       SET_BIT(ch->more2, MORE2_SHIELD);

    send_to_char( "Sensing mortal danger, your blade hums silently as it emits of magical energy.\n\r", ch );
    act( "Sensing mortal danger, $n's blade hums silently as it emits of magical energy.", ch, NULL, NULL, TO_ROOM );
    act( "#wA #Cmystical #Paura #wof magical energy fades around your body.#n", ch, NULL, NULL, TO_CHAR );
    act( "#wA #Cmystical #Paura #wof magical energy fades around #y$n#w's body.#n", ch, NULL, NULL, TO_ROOM );
}

void spell_slow( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, skill_lookup("slow") ) )
    {
      if ( IS_CLASS( ch, CLASS_WIZARD ) )
         send_to_char( "That person is already moving like a frog.\n\r", ch );

      return;
    }

    if ( is_affected( victim,skill_lookup("haste") ) )
    {
       if ( IS_CLASS( ch, CLASS_WIZARD ) )
          {
             send_to_char( "You start moving like you are walking on glue.\n\r", victim );
             act( "$n starts moving extremmely slowly.\n\r", victim, NULL, NULL, TO_ROOM );
          }

	  affect_strip( victim, skill_lookup("haste") );
        return;
    }

    af.type      = sn;
    af.duration  = 20;
    af.location  = APPLY_NONE;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( IS_CLASS( ch, CLASS_WIZARD ) )
       {
          send_to_char( "You start moving like you are walking on glue.\n\r", victim );
          act( "$n starts moving extremmely slowly.\n\r", victim, NULL, NULL, TO_ROOM );
       }
}

void spell_holy_word( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( victim == NULL )
         return;

      act("You feel refresh as the symbols flow through your body.",victim,NULL,ch,TO_CHAR);

	af1.type	= sn;
	af1.location	= APPLY_EVA;
      af1.modifier	= 4;

	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_CON;
      af2.modifier	= 4;

	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
	
	af3.type	= sn;
	af3.location	= APPLY_WIS;
      af3.modifier	= 4;
	af3.duration	= 1000;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);

	act("$n feels refresh as the symbols flow through $s body.",victim,NULL,NULL,TO_ROOM);
}

void spell_fire_wall( int sn, int level, CHAR_DATA *ch, void *vo)
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( IS_SET( victim->itemaffect, ITEMA_FIREWALL ) )
      return;

   act("#RYou are suddenly surrounded by a massive wall of #yblazing #Rfire.#n",ch,NULL,NULL,TO_CHAR);
   act("#R$n #Ris suddenly surrounded by a massive wall of #yblazing #Rfire.#n",ch,NULL,NULL,TO_ROOM);
   SET_BIT( victim->itemaffect, ITEMA_FIREWALL);
}

void spell_earth_wall( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( is_affected( victim, sn ) )
         return;

      act("The earth surrounds your body, protecting you against attacks.",victim,NULL,ch,TO_CHAR);

	af1.type	= sn;
	af1.location	= APPLY_EVA;
      af1.modifier	= 1;

	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

      act("The earth surrounds $n's body, protecting $m against attacks.",victim,NULL,ch,TO_ROOM);
}

void spell_unholy_strength( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    stc("You offer a silent prayer to the devil to fill your target with is undire strength.\n\r",ch);

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_STR;
    af.modifier  = 3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You feel unbelievable power surges through your veins.\n\r", victim );

    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}

void spell_alabaster_potion( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    if ( victim != ch )
       {
           act("You give $N the alabaster potion.",ch,NULL,victim,TO_CHAR);
           act("$n gives you the alabaster potion.",ch,NULL,victim,TO_VICT);
           act("$n gives $N the alabaster potion.",ch,NULL,victim,TO_NOTVICT);
       }

    stc("You open the alabaster potion drinking it down in one gulp.\n\r",victim);
    act("$n opens the alabaster potion drinking it down in one gulp.",victim,NULL,NULL,TO_ROOM);

    af.type      = sn;

    af.duration  = 200;

    af.location  = APPLY_STR;
    af.modifier  = 2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    stc("Your muscles tighten as everything feels lighter around you.\n\r",victim);
    act("$n's muscles bulges from the affects of the potion!",victim,NULL,NULL,TO_ROOM);
}

void spell_divine_strength( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_CLASS( ch, CLASS_SHADOW ) )
       {
         if ( ch->gifts[AMI] < 5 )
            {
               send_to_char("You have to improve your Mi fighting style to the 8th Level.\n\r",ch);
               return;
            }

         if ( ch->gifts[CURRENT_METER] != AMI )
            {
               send_to_char("You aren't concentrating on the Mi fighting style.\n\r",ch);
               return;
            }
       }

    if ( is_affected( victim, sn ) )
	return;

    af.type      = sn;

    af.duration  = 500;
    af.location  = APPLY_STR;
    af.modifier  = 4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( victim != ch )
       {
           act( "You hover above $N sternly as you say '#yDoth thou desire the power?#n'", ch, NULL, victim, TO_CHAR );
           act( "$n hovers above you sternly as $e says '#yDoth thou desire the power?#n'", ch, NULL, victim, TO_VICT );
           act( "$n hovers above $N sternly as $e says '#yDoth thou desire the power?#n'", ch, NULL, victim, TO_NOTVICT );
           act( "You sneer #yPower? I don't need it as long as I have this!#n'", victim, NULL, NULL, TO_CHAR );
           act( "$n sneers #yPower? I don't need it as long as I have this!#n'", victim, NULL, NULL, TO_ROOM );
           act( "You smirk saying '#yA result of your primitive intellect... Do you think you can still win?#n'", ch, NULL, victim, TO_CHAR );
           act( "$n smirks saying '#yA result of your primitive intellect... Do you think you can still win?#n'", ch, NULL, victim, TO_ROOM );
           act( "You point your palm at $N saying '#yThis is the power of the Mother of Destruction...#n'", ch, NULL, victim, TO_CHAR );
           act( "$n points $s palm at you saying '#yThis is the power of the Mother of Destruction...#n'", ch, NULL, victim, TO_VICT );
           act( "$n points $s palm at $N saying '#yThis is the power of the Mother of Destruction...#n'", ch, NULL, victim, TO_NOTVICT );
           act( "You feel MaSsIvE PoWeR flowing within you as $n enshrouds you within the dark light.",ch,NULL,victim,TO_VICT);
           act( "$N's muscles bulges as you enshroud $M within the dark light.",ch,NULL,victim,TO_CHAR);
           act( "$N's muscles bulges as $n enshrouds $M within the dark light.",ch,NULL,victim,TO_NOTVICT);
       }
       else {
                act( "You feel MaSsIvE PoWeR flowing within you as you are enshrouded within the dark light.",ch,NULL,NULL,TO_CHAR);
                act( "$n feels MaSsIvE PoWeR flowing within $m as $e is enshrouded within the dark light.",ch,NULL,NULL,TO_ROOM);
            }
}

void spell_demonic_speed( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
  
    if ( ch->clannum != 7 )
       return;

    if ( is_affected( victim, sn ) )
	return;

    act("You flip through the Holy Book as you scan the Blessed Text.",ch,NULL,NULL,TO_CHAR);
    act("$n flips through the Holy Book as $e scans the Blessed Text.",ch,NULL,NULL,TO_ROOM);
    act("You say '#yDear Father, may you give the faithful the speed worthy of the Light.#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says '#yDear Father, may you give the faithful the speed worthy of the Light.#n'",ch,NULL,NULL,TO_ROOM);

    af.type      = sn;

    af.duration  = 250;
    af.location  = APPLY_DEX;
    af.modifier  = 3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( victim != ch )
       {
           act( "You start moving in a fluid motion as $n enshrouds you within the divine light.",ch,NULL,victim,TO_VICT);
           act( "$N starts moving in a fluid motion as you enshroud $M within the divine light.",ch,NULL,victim,TO_CHAR);
           act( "$N starts moving in a fluid motion as $n enshrouds $M within the divine light.",ch,NULL,victim,TO_NOTVICT);
       }
       else {
                act( "You start moving in a fluid motion as you are enshrouded within the divine light.",ch,NULL,NULL,TO_CHAR);
                act( "$n starts moving in a fluid motion as $e is enshrouded within the divine light.",ch,NULL,NULL,TO_ROOM);
            }
}

void spell_blazing_strikes( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( !IS_CLASS(ch,CLASS_WIZARD) || is_affected( victim, sn ) )
       return;

    act("You flip through the Holy Book as you scan the Blessed Text.",ch,NULL,NULL,TO_CHAR);
    act("$n flips through the Holy Book as $e scans the Blessed Text.",ch,NULL,NULL,TO_ROOM);
    act("You say '#yDear Father, Bestow upon us the knowledge to destroy evil.#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says '#yDear Father, Bestow upon us the knowledge to destroy evil.#n'",ch,NULL,NULL,TO_ROOM);

    af.type      = sn;

    af.duration  = 200;
    af.location  = APPLY_INT;
    af.modifier  = 3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( victim != ch )
       {
           act( "Suddenly, everything becomes clear as $n enshrouds you within the divine light.",ch,NULL,victim,TO_VICT);
           act( "$N seems smarter as you enshroud $M within the divine light.",ch,NULL,victim,TO_CHAR);
           act( "$N seems smarter as $n enshrouds $M within the divine light.",ch,NULL,victim,TO_NOTVICT);
       }
       else {
                act( "Suddenly, everything becomes clear as you are enshrouded within the divine light.",ch,NULL,NULL,TO_CHAR);
                act( "$n seems smarter as $e is enshrouded within the divine light.",ch,NULL,NULL,TO_ROOM);
            }
}

void spell_darkside( int sn, int level, CHAR_DATA *ch, void *vo)
{
      AFFECT_DATA af1;
      AFFECT_DATA af2;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( IS_NPC( ch ) )
         return;

	if ( is_affected(victim, sn) ) 
	   return;

      if ( victim != ch && !IS_NPC( victim ) )
         give_align( ch, 15 );
	
	af1.type	= sn;
	af1.location	= APPLY_STR;
	af1.modifier	= 3;
	af1.duration	= 500;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_DEX;
	af2.modifier	= 3;
	af2.duration	= 500;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
	
	send_to_char("You cackle wildly as the powers of the Darkside embraces you!\n\r", victim);
	act("$n leans back and cackles with rage as the Darkside engulfs $m!",victim,NULL,NULL,TO_ROOM);
}

void spell_despondency( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      if ( IS_NPC( ch ) )
         return;

	if ( victim == ch )
         act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
         else act( "You mumble in the ancient language as you concentrate fiercely on $N.", ch, NULL, victim, TO_CHAR );

      act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_NOTVICT );

	if ( is_affected(victim, sn) ) 
         {
            send_to_char("their soul is already being despondinize.\n\r",ch);
            return;
         }

      if ( IS_NPC( victim ) && victim->pIndexData->vnum >= 550 && victim->pIndexData->vnum < 571 )
         return;

	af1.type	= sn;
	af1.location	= APPLY_STR;
	af1.modifier	= -10;
	af1.duration	= 10;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	send_to_char("Your body weakens as your soul withers and lifeforce ebbs away.\n\r", victim);
	act("$n's body weakens as $e becomes pale and very sick.",victim,NULL,NULL,TO_ROOM);
}

void spell_dark_holy( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );

    act("You cackle wildly as your eyes go on fire.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles wildly as $s eyes go on fire.",ch,NULL,NULL,TO_ROOM);

    act("Suddenly, a beam of demonic energy engulfs $N as the room goes dark.",ch,NULL,victim,TO_CHAR);
    act("Suddenly, a beam of demonic energy engulfs you as the room goes dark.",ch,NULL,victim,TO_VICT);
    act("Suddenly, a beam of demonic energy engulfs $N as the room goes dark.",ch,NULL,victim,TO_NOTVICT);

    act("The room burst into unholy flames as evil laughter fills the very air with its coldness.",ch,NULL,NULL,TO_CHAR);
    act("The room burst into unholy flames as evil laughter fills the very air with its coldness.",ch,NULL,NULL,TO_ROOM);

    act("$N screams loudly as the flames eat away at $S soul.",ch,NULL,victim,TO_CHAR);
    act("You scream loudly as the flames eat away at your soul.",ch,NULL,victim,TO_VICT);
    act("$N screams loudly as the flames eat away at $S soul.",ch,NULL,victim,TO_NOTVICT);

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_focus = UPPERCUT;
    ch->attack_var = BEAM;
    ch->ele_attack = ATTACK_DARK;
    hurt_person( ch, victim, 1000000, TRUE );
}


void spell_cause_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, dice(1, 8) + level / 3, sn - 2000 );
    return;
}



void spell_cause_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, dice(3, 8) + level - 6, sn - 2000 );
    return;
}



void spell_cause_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, dice(2, 8) + level / 2, sn - 2000 );
    return;
}



void spell_change_sex( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    return;
    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = 10 * level;
    af.location  = APPLY_SEX;
    do
    {
	af.modifier  = number_range( 0, 2 ) - victim->sex;
    }
    while ( af.modifier == 0 );
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel different.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}

void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if (IS_IMMORTAL( victim ))
    {
	send_to_char( "You can cast puny mortal magic on immortals!\n\r",ch);
	return;
    }

    if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    ||   level < victim->level
    ||   saves_spell( level, victim ) )
	return;

    if ( victim->master )
	stop_follower( victim );
    add_follower( victim, ch );
    af.type      = sn;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_chill_touch( int sn, int level, CHAR_DATA *ch, void *vo )
{
    bool no_dam = FALSE;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 9,
	10, 10, 10, 11, 11,	12, 12, 13, 13, 13,
	14, 14, 14, 15, 15,	15, 16, 16, 16, 17,
	17, 17, 18, 18, 18,	19, 19, 19, 20, 20,
	20, 21, 21, 21, 22,	22, 22, 23, 23, 23,
	24, 24, 24, 25, 25,	25, 26, 26, 26, 27,
	30, 40, 50, 60, 70,	80, 90,100,110,120
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_COLD) ) no_dam = TRUE;

    if ( ( !IS_NPC(ch) ) && IS_CLASS(ch,CLASS_DRAGON) && ( ch->dragonform == 23 ) )
       dam = dam * 2000;

    hp = victim->hit;

    if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
       dam = char_damcap( ch );

    damage( ch, victim, dam, sn - 2000 );

    if ( !IS_CLASS( ch, CLASS_WIZARD ) && no_dam ) victim->hit = hp;
    return;
}

void spell_kaoken( int sn, int level, CHAR_DATA *ch, void *vo)
{
   AFFECT_DATA af1;
   AFFECT_DATA af2;
   AFFECT_DATA af3;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You move your feet apart into a deep fighting stance and start to clench your fist.",ch,NULL,NULL,TO_CHAR);
   act("$n moves $s feet apart into a deep fighting stance and starts to clench $s fist.",ch,NULL,NULL,TO_ROOM);
   act("You growl deeply in your throat as you concentrate to release your latent power within.",ch,NULL,NULL,TO_CHAR);
   act("$n growls deeply in $s throat as $e concentrates to release $s latent power within.",ch,NULL,NULL,TO_ROOM);
   act("A #RRed Aura#n surrounds you as your muscles bulge and the ground starts to crack under such intense power.",ch,NULL,NULL,TO_CHAR);
   act("A #RRed Aura#n surrounds $n as $s muscles bulge and the ground starts to crack under such intense power.",ch,NULL,NULL,TO_ROOM);
   act("#RAs your eyes glow red, you lean back screaming with rage!#n",ch,NULL,NULL,TO_CHAR);
   act("#RAs $n's eyes glow red, $e leans back screaming with rage!#n",ch,NULL,NULL,TO_ROOM);
   act("You yell, '#RKAAAIIOOOO......KENNNN!!#n' as your body trembles violently.",ch,NULL,NULL,TO_CHAR);
   act("$n yells, '#RKAAAIIOOOO......KENNNN!!#n' as $s body trembles violently.",ch,NULL,NULL,TO_ROOM);

   af3.type = sn;
   af3.location = APPLY_STR;
   af3.modifier = URANGE( 1, ch->pkill / 2, 10 );
   af3.duration = 100;
   af3.bitvector = 0;
   affect_to_char(victim, &af3);
}

void spell_colour_spray( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	10,
	15, 15, 15, 15, 15,	20, 20, 20, 20, 20,
	30, 35, 40, 45, 50,	55, 55, 55, 56, 57,
	58, 58, 59, 60, 61,	61, 62, 63, 64, 64,
	65, 66, 67, 67, 68,	69, 70, 70, 71, 72,
	73, 73, 74, 75, 76,	76, 77, 78, 79, 79,
	85, 95,110,125,150,	175,200,250,300,350
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2,  dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;

    damage( ch, victim, dam, sn - 2000 );
}



void spell_continual_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *light;

    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
    obj_to_room( light, ch->in_room );
    act( "$n twiddles $s thumbs and $p appears.",   ch, light, NULL, TO_ROOM );
    act( "You twiddle your thumbs and $p appears.", ch, light, NULL, TO_CHAR );
    return;
}



void spell_control_weather( int sn, int level, CHAR_DATA *ch, void *vo )
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 3, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 3, 4 );
    else
	send_to_char ("Do you want it to get better or worse?\n\r", ch );

    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
    mushroom->value[0] = 5 + level;
    obj_to_room( mushroom, ch->in_room );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
    return;
}



void spell_create_spring( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *spring;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE))
	return;
    else
    	spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( "$p flows from the ground.", ch, spring, NULL, TO_ROOM );
    act( "$p flows from the ground.", ch, spring, NULL, TO_CHAR );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
	return;
    }

    water = UMIN(
		level * (weather_info.sky >= SKY_RAINING ? 4 : 2),
		obj->value[0] - obj->value[1]
		);
  
    if ( water > 0 )
    {
	obj->value[2] = LIQ_WATER;
	obj->value[1] += water;
	if ( !is_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "%s water", obj->name );
	    free_string( obj->name );
	    obj->name = str_dup( buf );
	}
	act( "$p is filled.", ch, obj, NULL, TO_CHAR );
    }

    return;
}



void spell_cure_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( !is_affected( victim, gsn_blindness ) )
	return;
    affect_strip( victim, gsn_blindness );
    send_to_char( "Your vision returns!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}



void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    if ( !can_heal(ch,victim) )
       return;

    ch->wait = 12;

    heal = dice(3, 8) + level - 6;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    if ( !can_heal(ch,victim) )
       return;

    ch->wait = 12;

    heal = dice(1, 8) + level / 3;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( is_affected( victim, gsn_poison ) )
    {
	affect_strip( victim, gsn_poison );
	act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
	send_to_char( "A warm feeling runs through your body.\n\r", victim );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}



void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    if ( !can_heal(ch,victim) )
       return;

    ch->wait = 12;

    heal = dice(2, 8) + level /2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       return;

    if ( IS_SET( victim->more2, MORE2_SHIELD ) )
       return;

    if ( IS_AFFECTED(victim, AFF_CURSE) )
       return;

    af.type      = sn;
    af.duration  = 4*level;
    af.location  = APPLY_WIS;
    af.modifier  = -1;
    af.bitvector = AFF_CURSE;
    affect_to_char( victim, &af );

    send_to_char( "You feel unclean.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_curse_allah( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( !IS_CLASS( ch, CLASS_MONK ) )
       return;

    if ( ch->pkill < 12 )
       {
          stc("You aren't powerful enough to perform the Curse of Allah.\n\r",ch);
          return;
       }

    if ( ch == victim )
       return;

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) || IS_CLASS( victim, CLASS_ANGEL ) )
       return;

    if ( is_affected(victim, skill_lookup("curse of allah") ) )
       {
          stc("They are already cursed by the one true God.\n\r",ch);
          return;
       }

    act("You flip through the Holy Book as you scan the Blessed Text.",ch,NULL,NULL,TO_CHAR);
    act("$n flips through the Holy Book as $e scans the Blessed Text.",ch,NULL,NULL,TO_ROOM);

    act("You say '#yOur Heavenly Father, may you show these heathens the wrath of your holy powers.#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says '#yOur Heavenly Father, may you show these heathens the wrath of your holy powers.#n'",ch,NULL,NULL,TO_ROOM);

    act("You point a hand at $N as $E is surrounded by a gray outline.",ch,NULL,victim,TO_CHAR);
    act("$n points a hand at you as you are surrounded by a gray outline.",ch,NULL,victim,TO_VICT);
    act("$n points a hand at $N as $E is surrounded by a gray outline.",ch,NULL,victim,TO_NOTVICT);

    af.type      = sn;
    af.duration  = 3;
    af.location  = APPLY_HITROLL;
    af.modifier = -500000;

    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_AC;
    af.modifier = 20000;

    affect_to_char( victim, &af );

    send_to_char( "You feel unclean.\n\r", victim );
    return;
}

void spell_holy_retrieval( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !IS_CLASS( ch, CLASS_ANGEL ) )
       return;

    if ( ch->pkill < 14 )
       {
          stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
          return;
       }

    if ( victim->fight_timer > 0 )
       {
          stc("The Light forbades you to interfere with this fight for now....\n\r",ch);
          return;
       }

    act("You flip through the Holy Book as you scan the Blessed Text.",ch,NULL,NULL,TO_CHAR);
    act("$n flips through the Holy Book as $e scans the Blessed Text.",ch,NULL,NULL,TO_ROOM);

    if ( victim == ch )
       {
          act("You say '#yOur Divine Father, take me from this wretched place back to our mortal realm.#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says '#yOur Divine Father, take me from this wretched place back to our mortal realm.#n'",ch,NULL,NULL,TO_ROOM);
       }
       else {
               act("You say '#yOur Divine Father, take $N from this wretched place back to our mortal realm.#n'",ch,NULL,victim,TO_CHAR);
               act("$n says '#yOur Divine Father, take $N from this wretched place back to our mortal realm.#n'",ch,NULL,victim,TO_ROOM);
            }

    act("Your body is transfused with holy energy as you shimmer back into existence.",victim,NULL,NULL,TO_CHAR);
    act("$n's body is transfused with holy energy as $e shimmers back into existence.",victim,NULL,NULL,TO_ROOM);

    if ( IS_AFFECTED(victim, AFF_SHADOWPLANE) )
	 REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
	 REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
}

void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_EVIL) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_hidden( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_HIDDEN) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_DETECT_HIDDEN;
    affect_to_char( victim, &af );
    send_to_char( "Your awareness improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_INVIS) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_MAGIC) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_MAGIC;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks very delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
    }

    return;
}



void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
  
    if ( !IS_NPC(ch) && IS_EVIL(ch) )
	victim = ch;
  
    if ( IS_GOOD(victim) )
    {
	act( "God protects $N.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
	return;
    }

    dam = dice( level, 4 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn - 2000 );
}



void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA *affect_next, *affect;

    if ( victim != ch && !( IS_CLASS(ch, CLASS_WIZARD) && ch->clannum == 4 ) )
       return;

    if ( is_affected(ch, skill_lookup("curse of allah") ) )
       return;

    if ( is_affected(victim, skill_lookup("gods disfavor") ) )
       {
          send_to_char("God refuses to allow anything to disrupt this curse.\n\r",ch);
	    return;
       }

    if ( victim != ch && IS_CLASS( victim, CLASS_WIZARD ) )
       {
          stc("Their mind is too strong.\n\r",ch);
          return;
       }

    if ( ch->fight_timer > 0 && victim == ch )
       {
          stc("Not with a fight timer.\n\r",ch);
          return;
       }

    if ( victim != ch && is_safe(ch,victim) )
       return;

    if ( !(victim->affected) )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    affect = victim->affected;

    while ( affect )
    {
       affect_next = affect->next;

       if ( affect->duration != -1 )
          affect_remove( victim, affect );

       affect = affect_next;
    }

    if ( ch == victim )
    {
    	act("You remove all magical affects from yourself.",ch,NULL,NULL,TO_CHAR);
    	act("$n has removed all magical affects from $mself.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
    	act("You remove all magical affects from $N.",ch,NULL,victim,TO_CHAR);
    	act("$n has removed all magical affects from $N.",ch,NULL,victim,TO_NOTVICT);
    	act("$n has removed all magical affects from you.",ch,NULL,victim,TO_VICT);
    }
}



void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    send_to_char( "The earth trembles beneath your feet!\n\r", ch );
    act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL || !can_see(ch, vch))
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
		damage( ch, vch, level + dice(2, 8), sn - 2000 );
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "The earth trembles and shivers.\n\r", vch );
    }

    return;
}



void spell_enchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_WEAPON
    ||   IS_SET(obj->quest, QUEST_ENCHANTED)
    ||   IS_SET(obj->quest, QUEST_ARTIFACT)
    ||   obj->chobj != NULL )
    {
	send_to_char("You are unable to enchant this weapon.\n\r",ch);
	return;
    }

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
/*
    paf->type		= 0;
*/
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= 200;

    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
/*
    paf->type		= -1;
*/
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= 200;

    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    SET_BIT(obj->quest, QUEST_ENCHANTED);
    act( "$p glows yellow.", ch, obj, NULL, TO_CHAR );
    act( "$p glows yellow.", ch, obj, NULL, TO_ROOM );
}

void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_ARMOR
    ||   IS_SET(obj->quest, QUEST_ENCHANTED)
    ||   IS_SET(obj->quest, QUEST_ARTIFACT)
    ||   obj->chobj != NULL )
    {
	send_to_char("You are unable to enchant this weapon.\n\r",ch);
	return;
    }

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
/*
    paf->type		= 0;
*/
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= level / 5;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
/*
    paf->type		= -1;
*/
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= level / 5;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_AC;

    if ( IS_CLASS(ch, CLASS_MAGE) ) 
        paf->modifier	= -level / 2;
        else paf->modifier	= -level / 5;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( IS_GOOD(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows blue.", ch, obj, NULL, TO_CHAR );
	act( "$p glows blue.", ch, obj, NULL, TO_ROOM );
    }
    else if ( IS_EVIL(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows red.", ch, obj, NULL, TO_CHAR );
	act( "$p glows red.", ch, obj, NULL, TO_ROOM );
    }
    else
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows yellow.", ch, obj, NULL, TO_CHAR );
	act( "$p glows yellow.", ch, obj, NULL, TO_ROOM );
    }
    return;
}



/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */
void spell_energy_drain( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 )
       {
          if ( is_safe( ch, victim ) )
             return;

          ch->hit = UMIN( ch->max_hit, ch->hit + 300000 );
          ch->mana = UMIN( ch->max_mana, ch->mana + 300000 );
          ch->move = UMIN( ch->max_move, ch->move + 300000 );
          act( "You focus intently on $N's lifeforce as you draw away some of it.", ch, NULL, victim, TO_CHAR );
          act( "$n focuses intently on your lifeforce as $e draws away some of it.", ch, NULL, victim, TO_VICT );
          act( "$n focuses intently on $N's lifeforce as $e draws away some of it.", ch, NULL, victim, TO_NOTVICT );
          victim->move = UMAX( 0, victim->move - 300000 );
          victim->mana = UMAX( 0, victim->mana - 300000 );
          ch->wait = 12;
          hurt_person( ch, victim, 300000, TRUE );
          return;
       }

    return;
}

void spell_blessing( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_CLASS( ch, CLASS_MONK ) )
       {
          if ( ch->pkill < 8 )
             {
                stc("You aren't powerful enough to perform the Blessings of the Saints.\n\r",ch);
                return;
             }
       }

    if ( is_affected(victim, sn) )
	 return;

    if ( IS_CLASS( ch, CLASS_MONK ) )
       {
          if ( ch->mana < 1000000 )
             {
                stc("You don't have enough magical energy that.\n\r",ch);
                return;
             }

          ch->mana -= 1000000;
       }

    af.type      = sn;
    af.duration  = 200;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( level != 999 )
       {
          act("You flip through the Holy Book as you scan the Blessed Text.",ch,NULL,NULL,TO_CHAR);
          act("$n flips through the Holy Book as $e scans the Blessed Text.",ch,NULL,NULL,TO_ROOM);

          act("You say '#yOur Heavenly Father, please protect us sinners against the evils of the world.#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says '#yOur Heavenly Father, please protect us sinners against the evils of the world.#n'",ch,NULL,NULL,TO_ROOM);

          if ( victim == ch )
             {
                stc("#yYou radiate off holy energy as you are engulfed by a beam of light.#n\n\r",ch);
                act("#y$n radiates off holy energy as $e is engulfed by a beam of light.#n",ch,NULL,NULL,TO_ROOM);
             }
             else {
                     stc("#yYou radiate off holy energy as you are engulfed by a beam of light.#n\n\r",victim);
                     act("#y$n radiates off holy energy as $e is engulfed by a beam of light.#n",victim,NULL,NULL,TO_ROOM);
                  }
       }
}

void spell_eyes_heavens( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, sn) )
	 return;

    return;

    if ( ch->mana < 10000 )
       {
          stc("You don't have enough magical energy that.\n\r",ch);
          return;
       }

    ch->mana -= 10000;
       
    af.type      = sn;

    if ( victim == ch )
       af.duration  = 1000;
       else af.duration  = 3;

    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    stc("#yYour eyes turns clear as everything is revealed to you.#n\n\r",victim);
    act("#y$n's eyes turns clear as $e is blessed with holy magic.#n",victim,NULL,NULL,TO_ROOM);
}

void spell_full_cure( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( ch->clannum != 2 )
       return;

    if ( ch->mana < 10000 )
       {
          stc("You don't have enough magical energy that.\n\r",ch);
          return;
       }

    ch->mana -= 10000;

    act("You flip through the Holy Book as you scan the Blessed Text.",ch,NULL,NULL,TO_CHAR);
    act("$n flips through the Holy Book as $e scans the Blessed Text.",ch,NULL,NULL,TO_ROOM);

    if ( victim != ch )
       {
          act("You say '#yBlessed be thy unworthy sinner, forgive $S unwanted choices.#n'",ch,NULL,victim,TO_CHAR);
          act("$n says '#yBlessed be thy unworthy sinner, forgive $S unwanted choices.#n'",ch,NULL,victim,TO_ROOM);
          act("As you lightly touch $N's forehead, sparkles of holy energy emits off $M.",ch,NULL,victim,TO_CHAR);
          act("As $n lightly touches your forehead, sparkles of holy energy emits off you.",ch,NULL,victim,TO_VICT);
          act("As $n lightly touches $N's forehead, sparkles of holy energy emits off $M.",ch,NULL,victim,TO_NOTVICT);
       }
       else {
               act("You say '#yBlessed be thy unworthy saint, forgive my unwanted choices.#n'",ch,NULL,NULL,TO_CHAR);
               act("$n says '#yBlessed be thy unworthy saint, forgive my unwanted choices.#n'",ch,NULL,NULL,TO_ROOM);
            }

    clear_affects( victim );

    if ( ch == victim )
    {
    	act("You remove all negative affects from yourself.",ch,NULL,NULL,TO_CHAR);
    	act("$n has removed all negative affects from $mself.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
    	act("You remove all negative affects from $N.",ch,NULL,victim,TO_CHAR);
    	act("$n has removed all negative affects from $N.",ch,NULL,victim,TO_NOTVICT);
    	act("$n has removed all negative affects from you.",ch,NULL,victim,TO_VICT);
    }
}

void spell_resurrection( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !IS_CLASS( ch, CLASS_MONK ) || ch->clannum != 2 )
       return;

    if ( victim == ch )
       {
          stc("You can't use this ability on yourself.\n\r",ch);
          return;
       }

    if ( ch->focus[CURRENT] < 75 )
       {
          send_to_char("You don't have enough focus points for that.\n\r",ch);
          return;
       }

    if ( victim->position >= POS_STUNNED )
       {
          stc("But they aren't in a mortal condition.\n\r",ch);
          return;
       }

    if ( !can_heal( ch, victim ) )
       return;

    ch->focus[CURRENT] -= 75;
    ch->wait = 24;
    victim->hit = 300000;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    victim->position = POS_STANDING;

    act("You pray to the Light to protect $N's soul from harm.",ch,NULL,victim,TO_CHAR);
    act("$n prays to the Light to protect your soul from harm.",ch,NULL,victim,TO_VICT);
    act("$n prays to the Light to protect $N's soul from harm.",ch,NULL,victim,TO_NOTVICT);
    act("Hovering spirits fade in and out of existence as they circle around you.",victim,NULL,NULL,TO_CHAR);
    act("Hovering spirits fade in and out of existence as they circle around $n.",victim,NULL,NULL,TO_ROOM);
    act("As they touch your body, your wounds vanishes as an unseen force lifts you back onto your feet.",victim,NULL,NULL,TO_CHAR);
    act("As the spirits touch $n's body, $s wounds vanishes as an unseen force lifts $m back onto $s feet.",victim,NULL,NULL,TO_ROOM);
    act("You stagger around for a while trying to regain your composure.",victim,NULL,NULL,TO_CHAR);
    act("$n staggers around for a while trying to regain $s composure.",victim,NULL,NULL,TO_ROOM);
}

void spell_protection_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_CLASS( ch, CLASS_ANGEL ) && ch->pkill < 10 )
       {
          stc( "#wYour #Pfaith #win the #ylight #wisn't powerful enough as yet divine one.#n\n\r", ch );
          return;
       }

    if ( is_affected(victim, skill_lookup("protection from evil") ) )
       return;

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act("You say, '#yAli mu vori kient...#n",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yAli mu vori kient...#n",ch,NULL,NULL,TO_ROOM);
    send_to_char( "You feel protected as the virtues forms with your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}

void spell_godsdisfavor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_CON;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_con( victim ) / -2 : get_curr_con( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_str( victim ) / -2 : get_curr_str( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_INT;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_int( victim ) / -2 : get_curr_int( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_WIS;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_wis( victim ) / -2 : get_curr_wis( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_EVA;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_eva( victim ) / -2 : get_curr_eva( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_AGI;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_agi( victim ) / -2 : get_curr_agi( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_DEX;
    af.modifier  = ( ch->alignment > 349 ) ? get_curr_dex( victim ) / -2 : get_curr_dex( victim ) / -4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act("A voice booms '#yThou shalt pay for harming an innocent!#n",ch,NULL,NULL,TO_CHAR);
    act("A voice booms '#yThou shalt pay for harming an innocent!#n",ch,NULL,NULL,TO_ROOM);
    act("#CYou #ware suddenly struck down by a #ypowerful #wbeam of #RFEROCIOUS #wenergy!!!#n",victim,NULL,NULL,TO_CHAR);
    act("#C$n #wis suddenly struck down by a #ypowerful #wbeam of #RFEROCIOUS #wenergy!!!#n",victim,NULL,NULL,TO_ROOM);
    
    if ( victim->position > POS_STUNNED )
       victim->position = POS_STUNNED;
}

void spell_mani_katti( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( IS_CLASS( ch, CLASS_ANGEL ) || is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration  = 24;

    af.location  = APPLY_INT;
    af.modifier  = 5;
    af.bitvector = AFF_MANA;
    affect_to_char( ch, &af );
}

void spell_sol_katti( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_DEX;
    af.modifier  = 5;
    af.bitvector = AFF_MOVE;
    affect_to_char( ch, &af );

    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_EVA;
    af.modifier  = 5;
    af.bitvector = 0;
    affect_to_char( ch, &af );
}

/* Leighton - Monk's cloak of life */
void spell_cloak_life( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration  = 100;

    af.location  = APPLY_CON;
    af.modifier  = 5;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    act( "$n silently wraps a shimmering cloak around $s body.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You are blessed with the cloak of life.\n\r", ch );
    return;
}

void spell_divine_might( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration  = 2;

    af.location  = APPLY_STR;
    af.modifier  = 5;
    af.bitvector = 0;
    affect_to_char( ch, &af );
}

/* Leighton - Monk's cloak of life */
void spell_divine_reflection( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration = 100;

    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    act( "$n is surrounded by a white outline.", ch, NULL, NULL, TO_ROOM );

    if ( IS_CLASS(ch,CLASS_MONK) )
       send_to_char( "You pray to God for salvation as he surrounds your body with a divine shield.\n\r", ch );
}

void spell_divine_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;

    if ( is_affected(ch, sn) )
	 return;

    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    act( "$n is engulfed in holy energy directed from the heavens.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You pray to God for protection as he surrounds your body with his holy will.\n\r", ch );
}

void spell_godbless( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
        CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( ch->clannum != 2 )
       return;

	if ( victim == ch )
         act("You kneel on your knees and pray to God.",ch,NULL,NULL,TO_CHAR);
         else act("You kneel on your knees and pray for the protection of $N's soul.",ch,NULL,victim,TO_CHAR);

      act("$n kneels on $s knees and prays to God.",ch,NULL,NULL,TO_ROOM);

	if ( is_affected(victim, sn) ) 
	{
		send_to_char("they are already blessed by God.\n\r",ch);
		return;
	}
	
	af1.type	= sn;
	af1.location	= APPLY_AC;
	af1.modifier	= -10000;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_HITROLL;
	af2.modifier	= 5000;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
	
	af3.type	= sn;
      af3.location	= APPLY_DAMROLL;
      af3.modifier      = 5000;
	af3.duration	= 1000;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);

	af3.type	= sn;
      af3.location	= APPLY_HIT;
      af3.modifier      = 50000;
	af3.duration	= 1000;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);
	
	send_to_char("You are blessed by God!\n\r", victim);
	act("$n is blessed by God!",victim,NULL,NULL,TO_ROOM);
}


void spell_mangara_blessing( int sn, int level, CHAR_DATA *ch, void *vo)
{
      AFFECT_DATA af1;
      AFFECT_DATA af2;
      AFFECT_DATA af3;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( victim == ch )
         act("You kneel on your knees and pray to God.",ch,NULL,NULL,TO_CHAR);
         else act("You kneel on your knees and pray for the salvation of $N's soul.",ch,NULL,victim,TO_CHAR);

         act("$n kneels on $s knees and prays to God.",ch,NULL,NULL,TO_ROOM);

	if ( is_affected(victim, sn) ) 
	   {
               send_to_char("They are already blessed by the Divine Savior.\n\r",ch);
               return;
	   }
	
	af1.type	= sn;
	af1.location	= APPLY_HIT;
	af1.modifier	= 1000000;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_MANA;
	af2.modifier	= 1000000;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

        af3.type        = sn;
        af3.location	= APPLY_MOVE;
        af3.modifier    = 1000000;
        af3.duration	= 1000;
	af3.bitvector	= 0;
        affect_to_char(victim, &af3);

      send_to_char("You are blessed by Saint Mangara!\n\r", victim);
      act("$n is blessed by Saint Mangara!",victim,NULL,NULL,TO_ROOM);
}

void spell_hope( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( victim == ch )
         act("You kneel on your knees and pray to God.",ch,NULL,NULL,TO_CHAR);
         else act("You kneel on your knees and pray for the salvation of $N's soul.",ch,NULL,victim,TO_CHAR);

      act("$n kneels on $s knees and prays to God.",ch,NULL,NULL,TO_ROOM);

	if ( is_affected(victim, sn) ) 
	{
		send_to_char("They are already blessed by the almighty power of hope.\n\r",ch);
		return;
	}
	
	af1.type	= sn;
	af1.location	= APPLY_HIT;
	af1.modifier	= 1000000;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_MANA;
	af2.modifier	= 1000000;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);


	af3.type	= sn;
	af3.location	= APPLY_MOVE;
        af3.modifier    = 1000000;
	af3.duration	= 1000;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);

	af1.type	= sn;
	af1.location	= APPLY_CON;
	af1.modifier	= 5;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_WIS;
	af2.modifier	= 5;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
	
	send_to_char("You are filled with hope!\n\r", victim);
	act("$n's heart is filled with hope!",victim,NULL,NULL,TO_ROOM);
}

void spell_demonic_might( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( is_affected(victim, sn) ) 
         return;
	
	af1.type	= sn;
	af1.location	= APPLY_HIT;
	af1.modifier	= 1000000 * ( 1 + ( level / 10 ) );
	af1.duration	= 5;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_MANA;
	af2.modifier	= 1000000 * ( 1 + ( level / 10 ) );
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);


	af3.type	= sn;
	af3.location	= APPLY_MOVE;
	af3.modifier	= 1000000 * ( 1 + ( level / 10 ) );
	af3.duration	= 5;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);

	af1.type	= sn;
	af1.location	= APPLY_CON;
	af1.modifier	= level;
	af1.duration	= 5;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_WIS;
	af2.modifier	= level;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_STR;
	af2.modifier	= level;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_INT;
	af2.modifier	= level;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_DEX;
	af2.modifier	= level;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_EVA;
	af2.modifier	= level;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_AGI;
	af2.modifier	= level / 2;
	af2.duration	= 5;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
}

void spell_battle_tactics( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( is_affected(victim, sn) ) 
         return;
	
	af1.type	= sn;
	af1.location	= APPLY_HIT;
	af1.modifier	= 1500000;
	af1.duration	= level;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_MANA;
	af2.modifier	= 1500000;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);


	af3.type	= sn;
	af3.location	= APPLY_MOVE;
	af3.modifier	= 1500000;
	af3.duration	= level;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);

	af1.type	= sn;
	af1.location	= APPLY_CON;
	af1.modifier	= 5;
	af1.duration	= level;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_WIS;
	af2.modifier	= 5;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_STR;
	af2.modifier	= 5;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_INT;
	af2.modifier	= 5;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_DEX;
	af2.modifier	= 5;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_EVA;
	af2.modifier	= 5;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_AGI;
	af2.modifier	= 5;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
}

void spell_lunar_eclipse( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;

	if ( is_affected( ch, sn ) ) 
         return;
	
	af1.type	= sn;
	af1.location	= APPLY_HIT;
	af1.modifier	= 2000000;
	af1.duration	= level;
	af1.bitvector	= 0;
	affect_to_char(ch, &af1);

	af2.type	= sn;
	af2.location	= APPLY_MANA;
	af2.modifier	= 1000000;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(ch, &af2);

	af3.type	= sn;
	af3.location	= APPLY_MOVE;
	af3.modifier	= 1000000;
	af3.duration	= level;
	af3.bitvector	= 0;
	affect_to_char(ch, &af3);

	af2.type	= sn;
	af2.location	= APPLY_DEX;
	af2.modifier	= 10;
	af2.duration	= level;
	af2.bitvector	= 0;
	affect_to_char(ch, &af2);
}

void spell_battle_lust( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( is_affected(victim, sn) ) 
         return;

      act( "You snarl '#yMy hot blood and my fighting spirit...#n'", ch, NULL, NULL, TO_CHAR );
      act( "$n snarls '#yMy hot blood and my fighting spirit...#n'", ch, NULL, NULL, TO_ROOM );
      act( "You then clench your fist continuing '#yboils and burns!!!#n", ch, NULL, NULL, TO_CHAR );
      act( "$n then clenches $s fist continuing '#yboils and burns!!!#n", ch, NULL, NULL, TO_ROOM );

	if ( victim == ch )
         {
            act("You scream '#RHEATED SOUL!#n' as your body explodes in ferocious flames.",ch,NULL,NULL,TO_CHAR);
            act("$n screams '#RHEATED SOUL!#n' as $s body explodes in ferocious flames.",ch,NULL,NULL,TO_ROOM);
         }
         else {
                 act("You scream '#RHEATED SOUL!#n' as $N's body explodes in ferocious flames.",ch,NULL,victim,TO_CHAR);
                 act("$n screams '#RHEATED SOUL!#n' as your body explodes in ferocious flames.",ch,NULL,victim,TO_VICT);
                 act("$n screams '#RHEATED SOUL!#n' as $N's body explodes in ferocious flames.",ch,NULL,victim,TO_NOTVICT);
              }

	af1.type	= sn;
	af1.location	= APPLY_STR;
	af1.modifier	= 7;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af1.type	= sn;
	af1.location	= APPLY_DEX;
	af1.modifier	= 7;
	af1.duration	= 1000;
	af1.bitvector	= 0;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_INT;
	af2.modifier	= 7;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_AGI;
	af2.modifier	= 7;
	af2.duration	= 1000;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
	
	send_to_char("You are filled with unholy rage!\n\r", victim);
	act("$n's spirit is filled with unholy rage!",victim,NULL,NULL,TO_ROOM);
}

void spell_briar_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    af.type      = sn;
    af.duration  = 200;
    af.modifier  = 10;

    af.location  = APPLY_WIS;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act( "$n is engulfed in a briar shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are engulfed in a briar shield.\n\r", victim );
    if ( ch != victim )
	 send_to_char( "Ok.\n\r", ch );
}

void spell_gaea_blessing( int sn, int level, CHAR_DATA *ch, void *vo)
{
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;
      CHAR_DATA *victim = (CHAR_DATA *) vo;

      act("You kneel on your knees and pray to Gaea.",ch,NULL,NULL,TO_CHAR);
      act("$n kneels on $s knees and prays to Gaea.",ch,NULL,NULL,TO_ROOM);

	if ( is_affected(victim, sn) ) 
	{
		send_to_char("they are already blessed by Gaea.\n\r",ch);
		return;
	}
	
	af2.type	= sn;
	af2.location	= APPLY_DEX;
	af2.modifier	= 1;
	af2.duration	= 200;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);
	
	af3.type	= sn;
	af3.location	= APPLY_STR;
      af3.modifier      = 1;
	af3.duration	= 200;
	af3.bitvector	= 0;
	affect_to_char(victim, &af3);
	
	send_to_char("You are blessed by Gaea!\n\r", victim);
	act("$n is blessed by Gaea!",victim,NULL,NULL,TO_ROOM);
}

/* Leighton - Replace the old infirmity with this one */
void spell_infirmity( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFIRMITY) )
       return;

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_DEX;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_WIS;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_CON;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_INT;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_DAMROLL;
    af.modifier  = -4000;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -4000;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );
    send_to_char( "You feel weaker.\n\r", victim );
    send_to_char( "You feel slower.\n\r", victim );
    send_to_char( "You feel less healthy.\n\r", victim );
}

void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 20,
	 20,  20,  20,  20,  20,	 25,  25,  25,  25,  25,
	 30,  30,  30,  30,  30,	 35,  40,  45,  50,  55,
	 60,  65,  70,  75,  80,	 82,  84,  86,  88,  90,
	 92,  94,  96,  98, 100,	102, 104, 106, 108, 110,
	112, 114, 116, 118, 120,	122, 124, 126, 128, 130,
	150, 200, 250, 300, 400,	500, 650, 750, 850,1000
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);

    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );

    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;

    if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
       dam = char_damcap( ch );

    damage( ch, victim, dam, sn - 2000 );

    if ( !IS_CLASS( ch, CLASS_WIZARD ) && !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT) )
	victim->hit = hp;

    return;
}



void spell_flamestrike( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;
    dam = dice(6, 8);
    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) )
    {
    	damage( ch, victim, (dam*2), sn - 2000 );
	hp = ((hp - victim->hit)/2) + victim->hit;
    }
    else
    	damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT) )
	victim->hit = hp;
    return;
}



void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
	return;

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_AC;

    if ( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 )
       af.modifier = 30000;
       else af.modifier  = 2 * level;

    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );
    send_to_char( "You are surrounded by a pink outline.\n\r", victim );
    act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM );
    return;
}

void spell_spew( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
 
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
            if (IS_NPC(vch)) dam=250*level;
               else dam=100*level;
            if (is_safe(ch,vch)==TRUE) break;
            damage( ch, vch, dam, sn - 2000 );
    }
    return;
}

void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *ich;

    act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );

    for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
    {
	if ( IS_IMMORTAL(ich) )
	    continue;

	if ( ich == ch || saves_spell( level, ich ) )
	    continue;

	affect_strip ( ich, gsn_invis			);
	affect_strip ( ich, gsn_mass_invis		);
	affect_strip ( ich, gsn_sneak			);
	REMOVE_BIT   ( ich->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( ich->affected_by, AFF_INVISIBLE	);
	REMOVE_BIT   ( ich->affected_by, AFF_SNEAK	);

	act( "$n is revealed!", ich, NULL, NULL, TO_ROOM );
	send_to_char( "You are revealed!\n\r", ich );
    }

    return;
}



void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
	return;
    af.type      = sn;
    af.duration  = level + 3;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );
    send_to_char( "You rise up off the ground.\n\r", victim );
    act( "$n rises up off the ground.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char_to_room( create_mobile( get_mob_index(MOB_VNUM_VAMPIRE) ),
	ch->in_room );
    return;
}



/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
void spell_general_purpose( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range( 25, 100 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn - 2000 );
    return;
}



void spell_giant_strength( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 1 + (level >= 18) + (level >= 25);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel stronger.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = UMAX(  20, victim->hit - dice(1,4) );
    if ( saves_spell( level, victim ) )
	dam = UMIN( 50, dam / 4 );
    dam = UMIN( 100, dam );
    damage( ch, victim, dam, sn - 2000 );
    return;
}

void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    ch->wait = 12;

    if ( nogang != 0 || victim->attacker != NULL )
       {
          stc("You can't heal others when group battles are off.\n\r",ch);
          return;
       }

    hit_gain( victim, 50000 );

    if ( victim != ch )
       {
          if ( victim->position < POS_STUNNED )
             give_align( ch, 25 );
             else give_align( ch, 5 );
       }

    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit;
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if (ch == victim)
	act( "$n heals $mself.", ch, NULL, NULL, TO_ROOM );
    else
	act( "$n heals $N.", ch, NULL, victim, TO_NOTVICT );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}



/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
void spell_high_explosive( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    dam = dice( level, 150 );
    if ( saves_spell( level, victim ) ) dam *= 0.5;
    damage( ch, victim, dam, sn - 2000 );
    return; 
}

void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    act("#CYou #wexamine #y$p #wcarefully.#n",ch,obj,NULL,TO_CHAR);
    act("#C$n #wexamines #y$p #wcarefully.#n",ch,obj,NULL,TO_ROOM);

    sprintf( buf,
	"#wObject #y'%s' #wis type #y%s, #wextra flags #y%s#w.\n\r",

	obj->name,
	item_type_name( obj ),
	extra_bit_name( obj->extra_flags )
	);
    send_to_char( buf, ch );

    sprintf( buf, "#wLevel #w%d. #yGold #wpiece value is #y%d#w.\n\r", obj->level, obj->cost );
    send_to_char( buf, ch );

    if (IS_SET(obj->spectype, SITEM_COPPER))
        send_to_char("#wThis item is forged with #ocopper#w.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_IRON))
        send_to_char("#wThis item is forged with #eiron#w.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_STEEL))
        send_to_char("#wThis item is forged with #esteel#w.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_ADAMANTITE))
        send_to_char("#wThis item is forged with #padamantite#w.\n\r",ch);
    if (IS_SET(obj->quest, QUEST_BLOODA))
        send_to_char( "#wThis weapon is dripping with #Rkindred blood#w.\n\r",ch);
    if (IS_SET(obj->quest, QUEST_ENCHANTED))
	send_to_char( "#wThis item has been #yenchanted#w.\n\r", ch );
    if (IS_SET(obj->quest, QUEST_SPELLPROOF))
	send_to_char( "#wThis item is #yresistant #wto offensive spells.\n\r", ch );
    if( IS_SET(obj->extra_flags, ITEM_MENCHANT))
	stc( "#wThis item is infused with #Cquintessence#w.\n\r", ch);

    switch ( obj->item_type )
    {
    case ITEM_PILL: 
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "#wLevel #y%d #wspells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
          sprintf( buf, " #y'%s'", skill_table[obj->value[1]].name );
	    send_to_char( buf, ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
          sprintf( buf, " #y'%s'", skill_table[obj->value[2]].name );
	    send_to_char( buf, ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
          sprintf( buf, " #y'%s'", skill_table[obj->value[3]].name );
	    send_to_char( buf, ch );
	}

	send_to_char( "#w.\n\r", ch );
	break;

    case ITEM_QUEST:
	sprintf( buf, "#yGold #wpiece value is #y%d#w.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_PKTOKEN:
	sprintf( buf, "#yPlayer #RKill #wvalue is #y%d#w.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_DTOKEN:
	sprintf( buf, "#CStatus #wvalue is #y%d#w.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_QUESTCARD:
	sprintf( buf, "#wQuest completion reward is #y%d #wquest points.\n\r", obj->level );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "#wHas #y%d#w(#y%d#w) charges of level #y%d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );
      
	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
          sprintf( buf, " #y'%s'", skill_table[obj->value[3]].name );
	    send_to_char( buf, ch );
	}

	send_to_char( "#w.\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

      if ( IS_SET( obj->weapflags, WEAPON_TWO_HANDS ) )
         stc ( "#wThis #Pmassive #wweapon requires the use of #Ptwo hands#w.#n\n\r", ch );

	if (itemtype == 1)
	    sprintf (buf, "#wThis weapon is dripping with #gcorrosive acid#w.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "#wThis weapon radiates an #eaura of darkness#w.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "#wThis #Rvampiric weapon #wdrinks the #esouls #wof its victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "#wThis weapon has been #Ctempered #win #Rhellfire#w.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "#wThis weapon crackles with sparks of #ylightning#w.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "#wThis weapon is dripping with a #edark #gpoison#w.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "#wThis weapon has been imbued with the power of #y%s#w.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if ( obj->value[0] >= 1000 )
         itemtype = obj->value[0] / 1000;
         else itemtype = 0;

	if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "#wThis weapon allows the wielder to see #Cinvisible things#w.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "#wThis weapon grants the power of #Cflight#w.\n\r");
	else if (itemtype == 45 || itemtype == 1)
	    sprintf (buf, "#wThis weapon allows the wielder to see in the #edark#w.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "#wThis weapon renders the wielder #Cinvisible #wto the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "#wThis weapon allows the wielder to walk through #osolid doors#w.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "#wThis holy weapon protects the wielder from #eevil#w.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "#wThis ancient weapon #yprotects #wthe wielder in combat.\n\r");
	else if ( itemtype == 26 )
      {
	    sprintf (buf, "#wThis ancient weapon grants its wielder #Csupernatural vision#w.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient weapon grants #ysuperior protection #wto its wielder.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis razor sharp weapon can #Cslice #wthrough armour without difficulty.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient weapon allows its wielder to move at #Csupernatural speed#w.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient weapon surrounds its wielder with a shield of #echaos#w.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient weapon #gregenerates #wthe #Rwounds #wof its wielder.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis crafty weapon allows the wielder to walk in #ecomplete silence#w.\n\r");
      }
	else if (itemtype == 9)
	    sprintf (buf, "#wThis crafty weapon allows the wielder to walk in #ecomplete silence.#w\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "#wThis weapon fills the target with the need to #edestroy #Rall#w.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "#wThis ancient weapon surrounds its wielder with a shield of #echaos#w.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "#wThis ancient weapon #gregenerates #wthe #Rwounds #wof its wielder.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "#wThis ancient weapon allows its wielder to move at #Csupernatural speed#w.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "#wThis razor sharp weapon can #Cslice #wthrough armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "#wThis ancient weapon #yprotects #wits wearer from player attacks.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "#wThis ancient weapon surrounds its wielder with a #eshield of darkness#w.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "#wThis ancient weapon grants #ysuperior protection #wto its wielder.\n\r");
	else if (itemtype == 22 || itemtype == 14 )
	    sprintf (buf, "#wThis ancient weapon grants its wielder #Csupernatural vision#w.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "#wThis ancient weapon makes its wielder #Pfleet-footed#w.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "#wThis ancient weapon #Cconceals #wits wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "#wThis ancient weapon #Rinvokes #wthe power of the #Rbeast#w.\n\r");
	else if (itemtype == 80)
	    sprintf (buf, "#wThis ancient weapon #Rinvokes #wthe power of the #Choly saint#w.\n\r");
	else if (itemtype == 50)
	    sprintf (buf, "#wThis ancient weapon has the power to #ostun #wits victims.\n\r");
	else if (itemtype == 81)
	    sprintf (buf, "#wThis ancient weapon allows you to #Cguard #wyourself.\n\r");
	else if (itemtype == 82)
	    sprintf (buf, "#wThis ancient weapon wraps you in a #ecloak of darkness.#w\n\r");
	else if (itemtype == 83)
	    sprintf (buf, "#wThis ancient weapon fills you with #eunholy #Rrage#w.\n\r");
	else if (itemtype == 84)
	    sprintf (buf, "#wThis ancient weapon allows you to #Csee #wall.\n\r");
	else if (itemtype == 85)
	    sprintf (buf, "#wThis ancient weapon gives you incredible #gregeneration #wpowers.\n\r");
	else if (itemtype == 61)
	    sprintf (buf, "#wThis legendary weapon unlocks the #ytrue#n #wpower of the #Pdragoons#w.\n\r");
	else if (itemtype == 62)
	    sprintf (buf, "#wThis ancient weapon #Pnullifies #wall dragoon attacks.\n\r");
	else if (itemtype == 63)
	    sprintf (buf, "#wThis ancient weapon allows you to #econtrol #wyour victims.\n\r");
	else
	    sprintf (buf, "#wThis item is bugged... please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );

      if ( IS_SET(obj->weapflags, TRUE_FIRE) )
         stc( "#wThis ancient weapon is engulfed with #Rferocious #rflames#w.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_ICE) )
         stc( "#wThis ancient weapon is covered with #Licy #wshards#w.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_MOON) )
         stc( "#wThis ancient weapon is blessed with the #ypowers#w of #oLuna#w.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_SAINT) )
         stc( "#wThis ancient weapon uses the sacred powers of the heavens.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_DARK) )
         stc( "#wThis ancient weapon's powers are derived from the #eDark #RLord#w himself.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_BOLT) )
         stc( "#wThis ancient weapon is #Psizzling#w with intense #yenergy#w.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_EARTH) )
         stc( "#wThis ancient weapon is encased in a #orocky #csubstance#w.\n\r", ch );

	break;

    case ITEM_ARMOR:
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4)
	    sprintf (buf, "#wThis object radiates an #eaura #wof #edarkness#w.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "#wThis item allows the wearer to see #Cinvisible #wthings.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "#wThis object grants the power of #Cflight#w.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 1)
	    sprintf (buf, "#wThis item allows the wearer to see in the #edark#w.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "#wThis object renders the wearer #Cinvisible #wto the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "#wThis object allows the wearer to walk through #osolid doors#w.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "#wThis holy relic #yprotects #wthe wearer from #eevil#w.\n\r");
	else if (obj->value[3] == 26)
      {
	    sprintf (buf, "#wThis ancient item grants its wearer #Csupernatural vision#w.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient item grants #ysuperior protection #wto its wearer.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis powerful item allows its wearer to #Cshear #wthrough armour without difficulty.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient item allows its wearer to move at #Csupernatural speed#w.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient item #gregenerates #wthe #Rwounds #wof its wearer.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis ancient item surrounds its wearer with a shield of #echaos#w.\n\r");
          stc(buf,ch);
	    sprintf (buf, "#wThis crafty item allows the wearer to walk in #ecomplete silence#w.\n\r");
      }
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "#wThis ancient relic #yprotects #wthe wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "#wThis crafty item allows the wearer to walk in #ecomplete silence#w.\n\r");
	else if (obj->value[3] == 30)
	    sprintf (buf, "#wThis object fills its wearer with the need to #edestroy #Rall#w.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "#wThis ancient item surrounds its wearer with a shield of #echaos#w.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "#wThis ancient item #gregenerates #wthe #Rwounds #wof its wearer.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "#wThis ancient item allows its wearer to move at #Csupernatural speed#w.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "#wThis powerful item allows its wearer to #Cshear #wthrough armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "#wThis powerful item #yprotects #wits wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "#wThis ancient item surrounds its wearer with a shield of #edarkness#w.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "#wThis ancient item grants #ysuperior protection #wto its wearer.\n\r");
	else if (obj->value[3] == 22 || obj->value[3] == 14 )
	    sprintf (buf, "#wThis ancient item grants its wearer #Csupernatural vision#w.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "#wThis ancient item makes its wearer #Pfleet-footed#w.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "#wThis ancient item #Cconceals #wits wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "#wThis ancient item #Rinvokes #wthe power of the #Rbeast#w.\n\r");
	else if (obj->value[3] == 80)
	    sprintf (buf, "#wThis ancient item invokes the power of the #Choly saint#w.\n\r");
	else if (obj->value[3] == 81)
	    sprintf (buf, "#wThis ancient item allows you to #Cguard #wyourself.\n\r");
	else if (obj->value[3] == 82)
	    sprintf (buf, "#wThis ancient item wraps you in a #ecloak of darkness#w.\n\r");
	else if (obj->value[3] == 83)
	    sprintf (buf, "#wThis ancient item fills you with #eunholy #Rrage#w.\n\r");
	else if (obj->value[3] == 84)
	    sprintf (buf, "#wThis ancient item allows you to #Csee all#w.\n\r");
	else if (obj->value[3] == 85)
	    sprintf (buf, "#wThis ancient item gives you incredible #gregeneration #wpowers.\n\r");
	else if (obj->value[3] == 61)
	    sprintf (buf, "#wThis legendary armour unlocks the #ytrue#n #wpower of the #Pdragoons#w.\n\r");
	else if (obj->value[3] == 62)
	    sprintf (buf, "#wThis ancient item #pnullifies #wall dragoon attacks.\n\r");
	else if (obj->value[3] == 63)
	    sprintf (buf, "#wThis ancient item allows you to #econtrol #wyour victims.\n\r");
	else
	    sprintf (buf, "#wThis item is bugged... please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "#wAffects #y%s #wby #y%d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "#wAffects #y%s #wby #y%d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    if ( obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR )
       {
          show_skill( ch, obj->value[1] );
          show_skill( ch, obj->value[2] );
       }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFRARED) )
	return;
    act( "$n's eyes glow red.\n\r", ch, NULL, NULL, TO_ROOM );
    af.type      = sn;
    af.duration  = 2 * level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;

    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );
    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_know_alignment( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char *msg;
    int ap;

    ap = victim->alignment;

         if ( ap >  700 ) msg = "$N has an aura as white as the driven snow.";
    else if ( ap >  350 ) msg = "$N is of excellent moral character.";
    else if ( ap >  100 ) msg = "$N is often kind and thoughtful.";
    else if ( ap > -100 ) msg = "$N doesn't have a firm moral commitment.";
    else if ( ap > -350 ) msg = "$N lies to $S friends.";
    else if ( ap > -700 ) msg = "$N's slash DISEMBOWELS you!";
    else msg = "I'd rather just not say anything at all about $N.";

    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}



void spell_lightning_bolt( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	10,
	15, 15, 15, 20, 20,	25, 25, 25, 25, 28,
	31, 34, 37, 40, 40,	41, 42, 42, 43, 44,
	44, 45, 46, 46, 47,	48, 48, 49, 50, 50,
	51, 52, 52, 53, 54,	54, 55, 56, 56, 57,
	58, 58, 59, 60, 60,	61, 62, 62, 63, 64,
	70, 80, 90,120,150,	200,250,300,350,400
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );

    if ( ( !IS_NPC(ch) ) && IS_CLASS(ch,CLASS_DRAGON) && ( ch->dragonform == 9 ) )
       dam = dam * 20;

    if ( saves_spell( level, victim ) )
	dam /= 2;

    if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
       dam = char_damcap( ch );

    hp = victim->hit;
    damage( ch, victim, dam, sn - 2000 );

    if ( !IS_CLASS( ch, CLASS_WIZARD ) && !IS_NPC(victim) && IS_IMMUNE(victim, IMM_LIGHTNING) )
	victim->hit = hp;

    return;
}



void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int count=0;

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ))
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s within %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name,
            obj->in_room == NULL || obj->in_room->area == NULL ? "somewhere" : obj->in_room->area->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );

        if (count > 50 )
            break;
        else
	   count++;


    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void spell_magic_missile( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	11, 11, 11, 11, 11,	12, 12, 12, 12, 12,
	13, 13, 13, 13, 13,	14, 14, 14, 14, 14,
	15, 20, 25, 30, 35,	40, 45, 55, 65, 75
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn - 2000 );
    return;
}



void spell_mass_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    CHAR_DATA *gch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_INVISIBLE) )
	    continue;
	act( "$n slowly fades out of existence.", gch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade out of existence.\n\r", gch );
	af.type      = sn;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( gch, &af );
    }
    send_to_char( "Ok.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
	return;
    af.type      = sn;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You turn translucent.\n\r", victim );
    return;
}



void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;
   char buf [MAX_INPUT_LENGTH];
   
   if ( !IS_CLASS( ch, CLASS_WIZARD ) || ch->clannum != 4 )
      return;

   if ( is_safe( ch, victim ) )
      return;

   sn = skill_lookup( "drow poison" );

   if ( is_affected( victim, sn ) )
      return;

   if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_DROW ) && IS_SET( victim->pcdata->powers[1], DPOWER_DROWPOISON ) )
      return;

   if ( !IS_NPC( victim ) && IS_CLASS( victim, CLASS_DRAGON ) && IS_DRAGONPOWER( victim, DRAGON_IMMUNITY ) )
      return;

   if ( !IS_SET( victim->more, MORE_POISON ) )
      SET_BIT( victim->more, MORE_POISON );

   af.type      = sn;
   af.duration  = 24;
   af.location  = APPLY_STR;
   af.modifier  = -3;
   af.bitvector = AFF_POISON;
   affect_join( victim, &af );
   send_to_char( "#gYou feel very sick.#n\n\r", victim );

   if ( ch == victim ) return;

   if (!IS_NPC(victim))
	sprintf(buf,"#g%s #wlooks very sick as your #gpoison #wtakes affect.#n\n\r",victim->name);
      else sprintf(buf,"#g%s #wlooks very sick as your #gpoison #wtakes affect.#n\n\r",victim->short_descr);
 
  send_to_char( buf, ch );
}

void spell_readaura(int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char      buf [MAX_INPUT_LENGTH];

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
}

void spell_protection( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT) )
	return;
    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PROTECT;
    affect_to_char( victim, &af );
    send_to_char( "You feel protected.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    ch->wait = 12;

    if ( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 )
       victim->move = UMIN( victim->move + 1000000, victim->max_move );
       else victim->move = UMIN( victim->move + 100000, victim->max_move );

    act("$n looks less tired.",victim,NULL,NULL,TO_ROOM);
    send_to_char( "You feel less tired.\n\r", victim );
    if (!IS_NPC(victim) && victim->sex == SEX_MALE && 
	victim->pcdata->stage[0] < 1 && victim->pcdata->stage[2] > 0)
	victim->pcdata->stage[2] = 0;
    return;
}



void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];

    one_argument( target_name, arg );

    if ( arg[0] == '\0')
    {
	send_to_char( "Remove curse on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, target_name ) ) != NULL )
    {
      int test = 0;

      if ( IS_AFFECTED(victim, AFF_CURSE) )
         {
            REMOVE_BIT( victim->affected_by, AFF_CURSE );
            test = 1;
         }

      if ( is_affected( victim,skill_lookup("despondency") ) )
         {
	      affect_strip( victim, skill_lookup("despondency") );
            test = 1;
         }

	if ( is_affected( victim, gsn_curse ) )
    	{
	    affect_strip( victim, gsn_curse );
          test = 1;
	}

      if ( test == 1 )
      {
	    send_to_char( "You feel better.\n\r", victim );

	    if ( ch != victim )
		send_to_char( "Ok.\n\r", ch );
      }

	return;
    }
    if ( ( obj = get_obj_carry( ch, arg ) ) != NULL )
    {
	if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
	{
	    REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);
	    act( "$p flickers with energy.", ch, obj, NULL, TO_CHAR );
	}
	else if ( IS_SET(obj->extra_flags, ITEM_NODROP) )
	{
	    REMOVE_BIT(obj->extra_flags, ITEM_NODROP);
	    act( "$p flickers with energy.", ch, obj, NULL, TO_CHAR );
	}
	return;
    }
    send_to_char( "No such creature or object to remove curse on.\n\r", ch );
    return;
}



void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
	return;

    af.type      = sn;
    af.duration  = 100;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( victim, &af );
    act( "$n is surrounded in a white aura.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a white aura!\n\r", victim );
    return;
}

void spell_holy_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED( victim, AFF_SHIELD ) )
       return;

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_CON;
    af.modifier  = 5;
    af.bitvector = AFF_SHIELD;
    affect_to_char( victim, &af );
 
    act( "#yYou #wgo on your knees as you pray silently to #CGod#w.#n", ch, NULL, NULL, TO_CHAR );
    act( "#y$n #wgoes on $s knees as $e prays silently to #CGod#w.#n", ch, NULL, NULL, TO_ROOM );
    act( "#g$n #wis blessed by the #yLight#w!#n", victim, NULL, NULL, TO_ROOM );
    send_to_char( "#gYou #ware blessed by the #yLight#w!#n\n\r", victim );
    return;
}

void spell_vengeance( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_WIS;
    af.modifier  = 5;
    af.bitvector = 0;
    affect_to_char( victim, &af );
 
    act( "#yYou #wgo on your knees as you pray silently to #CGod#w.#n", ch, NULL, NULL, TO_CHAR );
    act( "#y$n #wgoes on $s knees as $e prays silently to #CGod#w.#n", ch, NULL, NULL, TO_ROOM );
    act( "#g$n #wis shielded by the #yLight#w!#n", victim, NULL, NULL, TO_ROOM );
    send_to_char( "#gYou #ware shielded by the #yLight#w!#n\n\r", victim );
    return;
}

void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = 8 + level;
    af.location  = APPLY_AC;
    af.modifier  = -20;

    if ( IS_CLASS(ch, CLASS_DROW) )
       {
          af.modifier  = -15000;
          af.duration  = 1000;
       }


    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a force shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a force shield.\n\r", victim );
    return;
}



void spell_shocking_grasp( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const int dam_each[] = 
    {
	10,
	10, 10, 15, 15, 15,	20, 20, 25, 29, 33,
	36, 39, 39, 39, 40,	40, 41, 41, 42, 42,
	43, 43, 44, 44, 45,	45, 46, 46, 47, 47,
	48, 48, 49, 49, 50,	50, 51, 51, 52, 52,
	53, 53, 54, 54, 55,	55, 56, 56, 57, 57,
	60, 70, 85,100,125,	150,175,200,225,300
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;
    damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_LIGHTNING) )
	victim->hit = hp;
    return;
}



void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
       return;
  
    if ( level < victim->level
    || !IS_NPC( victim )
    || ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SLEEP) )
    || (  IS_NPC(victim) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    ||   saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.duration  = 0; /*changed from 4 to 1*/
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n goes to sleep.", victim, NULL, NULL, TO_ROOM );
	victim->position = POS_SLEEPING;
    }

    return;
}



void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin turns to stone.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    CHAR_DATA *mount;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 3
    ||   victim->fighting != NULL
    ||   victim->in_room->area != ch->in_room->area
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    ||   (IS_NPC(victim) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    ||   (IS_NPC(victim) && saves_spell( level, victim ) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
    act( "$N has summoned you!", victim, NULL, ch,   TO_CHAR );
    do_look( victim, "auto" );
    if ( (mount = victim->mount) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    do_look( mount, "auto" );
    return;
}

void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo )
{
   CHAR_DATA *mount;
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   
   if ( !( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 ) )
      return;

   if ( ch->fight_timer > 0 )
      {
         stc( "You aren't focused enough to pinpoint on their aura.\n\r", ch );
         return;
      }

   if ( victim->in_room->vnum < 100 )
      {
         stc( "You are forbidden on venturing into that area.\n\r", ch );
         return;
      }

   act( "$n slowly fades out of existence.", ch, NULL, NULL, TO_ROOM );

   char_from_room( ch );
   char_to_room( ch, victim->in_room );

   do_look( ch, "" );

   ch->ctimer[0] = 0;
   check_traps( ch );

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );

   act( "You concentrate on your magical energies as you fade in front of $N.", ch, NULL, victim, TO_CHAR );
   act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
}



void spell_ventriloquate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s says '%s'.\n\r",              speaker, target_name );
    sprintf( buf2, "Someone makes %s say '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER(buf1[0]);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( !is_name( speaker, vch->name ) )
	    send_to_char( saves_spell( level, vch ) ? buf2 : buf1, vch );
    }

    return;
}



void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = level / 2;
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel weaker.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * This is for muds that _want_ scrolls of recall.
 * Ick.
 */
void spell_word_of_recall( int sn, int level, CHAR_DATA *ch, void *vo )
{
    do_recall( (CHAR_DATA *) vo, "" );
    return;
}



/*
 * NPC spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    int dam;
    int hpch;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) && !IS_CLASS(ch, CLASS_DRAGON)) return;

    if ( number_percent( ) < 2 * level && !saves_spell( level, victim ) )
    {
	for ( obj_lose = ch->carrying; obj_lose != NULL; obj_lose = obj_next )
	{
/*	    int iWear;
*/
	    obj_next = obj_lose->next_content;

	    if ( number_bits( 2 ) != 0 )
		continue;

	    if (IS_SET(obj_lose->quest,QUEST_SPELLPROOF)) continue;
/*	    switch ( obj_lose->item_type )
	    {
	    case ITEM_ARMOR:
		if ( obj_lose->value[0] > 0 )
		{
		    act( "$p is pitted and etched!", victim, obj_lose, NULL, TO_CHAR );
		    if ( ( iWear = obj_lose->wear_loc ) != WEAR_NONE )
			victim->armor -= apply_ac( obj_lose, iWear );
		    obj_lose->value[0] -= 1;
		    obj_lose->cost      = 0;
		    if ( iWear != WEAR_NONE )
			victim->armor += apply_ac( obj_lose, iWear );
		}
		break;

	    case ITEM_CONTAINER:
		act( "$p fumes and dissolves!",
		    victim, obj_lose, NULL, TO_CHAR );
		extract_obj( obj_lose );
		break;
	    }*/
	}
    }

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );

    if ( dam > 100000 )
       dam = 100000;

    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;

    if( !IS_NPC(victim) ) dam /= 2;
    damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_ACID)&&!IS_CLASS(ch,CLASS_DRAGON) )
	victim->hit = hp;
}



void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );

    if ( dam > 100000 )
       dam = 100000;

    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) )
    {
    	damage( ch, victim, (dam*2), sn - 2000 );
	hp = ((hp - victim->hit)/2) + victim->hit;
    }
    else damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT) && !IS_CLASS(ch, CLASS_DRAGON) )
	victim->hit = hp;
    return;
}



void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    int dam;
    int hpch;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD)) return;

    if ( number_percent( ) < 2 * level && !saves_spell( level, victim ) )
    {
	for ( obj_lose = victim->carrying; obj_lose != NULL;
	obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( number_bits( 2 ) != 0 )
		continue;

	    if (IS_SET(obj_lose->quest,QUEST_SPELLPROOF)) continue;
	    switch ( obj_lose->item_type )
	    {
	    	default:            continue;
	    	case ITEM_CONTAINER:
	    	case ITEM_DRINK_CON:
	    	case ITEM_POTION:   msg = "$p freezes and shatters!"; break;
	    }

	    act( msg, victim, obj_lose, NULL, TO_CHAR );
	    extract_obj( obj_lose );
	}
    }

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );

    if ( dam > 100000 )
       dam = 100000;

    if ( saves_spell( level, victim ) )
	dam /= 2;

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) )
	dam /= 2;

    hp = victim->hit;

    damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_COLD) && !IS_CLASS(ch, CLASS_DRAGON) )
	victim->hit = hp;
    return;
}

void spell_magic_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );

    if ( dam > 100000 )
       dam = 100000;

    if ( saves_spell( level, victim ) )
	dam /= 2;

    damage( ch, victim, dam, sn - 2000 );
}

void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hpch;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;

      if ( ch == vch )
         continue;

      if ( is_same_group(ch,vch) )
         continue;

      if ( !IS_NPC(vch) )
         continue;

      if ( vch->pIndexData->vnum >= 550 && vch->pIndexData->vnum < 571 )
         continue;

	if ( !is_safe(ch,vch) )
	{
	    hpch = UMAX( 10, ch->hit );
	    dam  = number_range( hpch/16+1, hpch/8 );

          if ( !IS_NPC(vch) )
             damage( ch, vch, 100000, sn - 2000 );
	       else damage( ch, vch, dam, sn - 2000 );
	}
    }
    return;
}



void spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) return;

    hpch = UMAX( 10, ch->hit );
    dam = number_range( hpch/16+1, hpch/8 );

    if ( dam > 100000 )
       dam = 100000;

    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;

    damage( ch, victim, dam, sn - 2000 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_LIGHTNING) && !IS_CLASS(ch, CLASS_DRAGON) )
	victim->hit = hp;
    return;
}

/* Extra spells written by KaVir. */

void spell_guardian( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->level = level;
    victim->hit = 100*level;
    victim->max_hit = 100*level;
    victim->hitroll = level;
    victim->damroll = level;
    victim->armor = 100 - (level*7);

    char_to_room( victim, ch->in_room );

    act("You exclaim '#yCome forth, creature of darkness, and do my bidding!#n'", ch,NULL,NULL, TO_CHAR);
    act("$n exclaims '#yCome forth, creature of darkness, and do my bidding!#n'", ch,NULL,NULL, TO_ROOM);
    send_to_char( "A demon bursts from the ground and bows before you.\n\r",ch );
    act( "$N bursts from the ground and bows before $n.", ch, NULL, victim, TO_ROOM );

    add_follower( victim, ch );
    af.type      = sn;
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    return;
}

void spell_soulblade( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA    *obj = (OBJ_DATA *) vo;

    obj = create_object( get_obj_index( OBJ_VNUM_SOULBLADE ), 0 );

    if (IS_NPC(ch)) obj->level = ch->level;
    else if (ch->spl[2] > 4) obj->level = ch->spl[2]/4;
    else obj->level = 1;
    if (obj->level > 60) obj->level = 60;
    obj->value[0] = 13034;
    obj->value[1] = 10;
    obj->value[2] = 20;
    obj->value[3] = number_range(1,12);

    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    if (!IS_NPC(ch))
    {
	if (obj->questowner != NULL) free_string(obj->questowner);
	obj->questowner = str_dup(ch->pcdata->switchname);
    }
    obj_to_char(obj,ch);
    act("$p fades into existance in your hand.", ch, obj, NULL, TO_CHAR);
    act("$p fades into existance in $n's hand.", ch, obj, NULL, TO_ROOM);
    return;
}

void spell_mana( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
	IS_VAMPAFF(ch, VAM_CELERITY))
    {
	if ( ch->move < 25 )
	{
	    send_to_char( "You are too exhausted to do that.\n\r", ch );
	    return;
	}
	ch->move = ch->move - 25;
    }
    else
    {
	if ( ch->move < 5000 )
	{
	    send_to_char( "You are too exhausted to do that.\n\r", ch );
	    return;
	}
	ch->move = ch->move - 5000;
    }
    victim->mana = UMIN( victim->mana + level + 1000, victim->max_mana);
    update_pos(ch);
    update_pos(victim);
    if (ch == victim)
    {
        send_to_char("You draw in energy from your surrounding area.\n\r",
                ch);
        act("$n draws in energy from $s surrounding area.", ch, NULL, NULL,
                TO_ROOM);
        return;
    }
    act("You draw in energy from around you and channel it into $N.",
            ch, NULL, victim, TO_CHAR);
    act("$n draws in energy and channels it into $N.",
            ch, NULL, victim, TO_NOTVICT);
    act("$n draws in energy and channels it into you.",
            ch, NULL, victim, TO_VICT);
}

void spell_frenzy( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    

    if ( is_affected( victim, sn )) return;
    af.type      = sn;
    af.duration  = 1 + level / 10;
    af.location  = APPLY_HITROLL;

    if ( ( !IS_NPC(ch) ) && IS_CLASS(ch,CLASS_DRAGON) && IS_CLASS(victim,CLASS_DRAGON) && ( ch->dragonform == 6 ) )
       {
          af.modifier  = 250000;
          af.duration  = 500;
       }
       else af.modifier  = level / 5;

    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;

    if ( ( !IS_NPC(ch) ) && IS_CLASS(ch,CLASS_DRAGON) && IS_CLASS(victim,CLASS_DRAGON) && ( ch->dragonform == 6 ) )
       {
          af.modifier  = 250000;
          af.duration  = 500;
       }
       else af.modifier  = level / 5;

    affect_to_char( victim, &af );

    af.location  = APPLY_AC;
    af.modifier  = level / 2;
    affect_to_char( victim, &af );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    act( "$n is consumed with rage.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are consumed with rage!\n\r", victim );
    if (!IS_NPC(victim)) do_beastlike(victim,"");
    return;
}

void spell_darkblessing( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) ) return;

    af.type      = sn;
    af.duration  = level / 2;
    af.location  = APPLY_HITROLL;
    af.modifier  = 1 + level / 14;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_DAMROLL;
    af.modifier  = 1 + level / 14;
    affect_to_char( victim, &af );

    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    act( "$n looks wicked.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You feel wicked.\n\r", victim );
    return;
}

void spell_portal( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
    int        duration;
    int cost = 0;

    one_argument( target_name, arg );

    if ( arg[0] == '\0')
       {
          send_to_char( "Who do you wish to create a portal to?\n\r", ch );
          return;
       }

    victim = get_char_world( ch, arg );

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_NPC(ch)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( in_kingdom( ch, victim, victim->in_room ) )
       return;

    if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
       {
          send_to_char( "#wYou are #eforbidden #wto use this #Cspell #yhere#w.#n\n\r", ch );
          return;
       }

    if ( ch->fight_timer > 0 )
       {
          cost = 50000;

          if ( !IS_CLASS( ch, CLASS_WEREWOLF ) && ( IS_SET( ch->in_room->area->bits,WASTE ) 
               || IS_SET( ch->in_room->area->bits,UPDATEWASTE ) ) )
             cost = 500000;

          if ( victim->in_room->area != ch->in_room->area )
             cost = ch->max_mana * 0.1;

          if ( ch->mana < cost )
             {
                stc("You don't have enough magical energy for that.\n\r",ch);
                return;
             }

          ch->mana -= cost;
       }

    duration = 1;

    obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = duration;
    obj_to_room( obj, ch->in_room );

    obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = duration;
    obj_to_room( obj, victim->in_room );

    if ( is_sound( ch ) )
       stc("!!SOUND(portal.wav V=100 L=1 P=60)\n\r",ch);

    if ( is_sound( victim ) )
       stc("!!SOUND(portal.wav V=100 L=1 P=60)\n\r",victim);

    act( "$p appears in front of $n.", ch, obj, NULL, TO_ROOM );
    act( "$p appears in front of you.", ch, obj, NULL, TO_CHAR );
    act( "$p appears in front of $n.", victim, obj, NULL, TO_ROOM );
    act( "$p appears in front of you.", ch, obj, victim, TO_VICT );
    return;
}

/* This spell is designed for potions */
void spell_energyflux( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( IS_CLASS( ch, CLASS_WIZARD ) && ch->clannum == 4 )
       victim->mana = UMIN( victim->mana + 1000000, victim->max_mana );
       else victim->mana = UMIN( victim->mana + 100000, victim->max_mana );

    ch->wait = 12;
    send_to_char("#wYou feel #Cmana #wchannel into your body.#n\n\r",victim);
    return;
}

void spell_voodoo( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA  *obj;
    char       buf [MAX_INPUT_LENGTH];
    char       arg [MAX_INPUT_LENGTH];
    char     part1 [MAX_INPUT_LENGTH];
    char     part2 [MAX_INPUT_LENGTH];
    int       worn;

    one_argument( target_name, arg );

    victim = get_char_world( ch, arg );

    if (ch->practice < 5)
    {
	send_to_char( "It costs 5 points of primal energy to create a voodoo doll.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL )
    {
	send_to_char( "Nobody by that name is playing.\n\r", ch );
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    {
    	if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
	{
	    send_to_char( "You are not holding any body parts.\n\r", ch );
	    return;
	}
	else worn = WEAR_HOLD;
    }
    else worn = WEAR_WIELD;

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if      (obj->value[2] == 12) sprintf(part1,"head %s",victim->name);
    else if (obj->value[2] == 13) sprintf(part1,"heart %s",victim->name);
    else if (obj->value[2] == 14) sprintf(part1,"arm %s",victim->name);
    else if (obj->value[2] == 15) sprintf(part1,"leg %s",victim->name);
    else if (obj->value[2] == 30004) sprintf(part1,"entrails %s",victim->name);
    else if (obj->value[2] == 30005) sprintf(part1,"brain %s",victim->name);
    else if (obj->value[2] == 30006) sprintf(part1,"eye eyeball %s",victim->name);
    else if (obj->value[2] == 30012) sprintf(part1,"face %s",victim->name);
    else if (obj->value[2] == 30013) sprintf(part1,"windpipe %s",victim->name);
    else if (obj->value[2] == 30014) sprintf(part1,"cracked head %s",victim->name);
    else if (obj->value[2] == 30025) sprintf(part1,"ear %s",victim->name);
    else if (obj->value[2] == 30026) sprintf(part1,"nose %s",victim->name);
    else if (obj->value[2] == 30027) sprintf(part1,"tooth %s",victim->name);
    else if (obj->value[2] == 30028) sprintf(part1,"tongue %s",victim->name);
    else if (obj->value[2] == 30029) sprintf(part1,"hand %s",victim->name);
    else if (obj->value[2] == 30030) sprintf(part1,"foot %s",victim->name);
    else if (obj->value[2] == 30031) sprintf(part1,"thumb %s",victim->name);
    else if (obj->value[2] == 30032) sprintf(part1,"index finger %s",victim->name);
    else if (obj->value[2] == 30033) sprintf(part1,"middle finger %s",victim->name);
    else if (obj->value[2] == 30034) sprintf(part1,"ring finger %s",victim->name);
    else if (obj->value[2] == 30035) sprintf(part1,"little finger %s",victim->name);
    else if (obj->value[2] == 30036) sprintf(part1,"toe %s",victim->name);
    else
    {
	sprintf(buf,"%s isn't a part of %s!\n\r",obj->name,victim->name);
	send_to_char( buf, ch );
	return;
    }

    sprintf(part2,obj->name);

    if ( str_cmp(part1,part2) )
    {
	sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
	send_to_char( buf, ch );
	return;
    }

    act("$p vanishes from your hand in a puff of smoke.", ch, obj, NULL, TO_CHAR);
    act("$p vanishes from $n's hand in a puff of smoke.", ch, obj, NULL, TO_ROOM);
    obj_from_char(obj);
    extract_obj(obj);

    obj = create_object( get_obj_index( OBJ_VNUM_VOODOO_DOLL ), 0 );

    sprintf(buf,"%s voodoo doll",victim->name);
    free_string(obj->name);
    obj->name=str_dup(buf);

    sprintf(buf,"a voodoo doll of %s",victim->name);
    free_string(obj->short_descr);
    obj->short_descr=str_dup(buf);

    sprintf(buf,"A voodoo doll of %s lies here.",victim->name);
    free_string(obj->description);
    obj->description=str_dup(buf);

    obj_to_char(obj,ch);
    equip_char(ch,obj,worn);

    act("$p appears in your hand.", ch, obj, NULL, TO_CHAR);
    act("$p appears in $n's hand.", ch, obj, NULL, TO_ROOM);

    ch->practice -= 5;

    return;
}

void spell_transport( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Transport which object?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' )
    {
        send_to_char( "Transport who whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that item.\n\r", ch );
	return;
    }

	if (IS_SET(obj->quest, QUEST_ARTIFACT)) {
	send_to_char("You can't transport that.\n\r", ch );
	return;}

	if (obj->item_type == ITEM_KINGDOM_POWER) {
	send_to_char("You can't transport that.\n\r", ch );
	return;}

    if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_TRANSPORT))
    {
	send_to_char( "You are unable to transport anything to them.\n\r", ch );
	return;
    }

    act("$p vanishes from your hands in an swirl of smoke.",ch,obj,NULL,TO_CHAR);
    act("$p vanishes from $n's hands in a swirl of smoke.",ch,obj,NULL,TO_ROOM);
    obj_from_char(obj);
    obj_to_char(obj,victim);
    act("$p appears in your hands in an swirl of smoke.",victim,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in an swirl of smoke.",victim,obj,NULL,TO_ROOM);
    do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}

void spell_regenerate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( victim->loc_hp[6] > 0 )
    {
	    send_to_char( "You cannot regenerate someone who is still bleeding.\n\r", ch );
	    return;
    }

    if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5]) != 0)
    {
          stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",victim);
          victim->loc_hp[0] = 0;
          victim->loc_hp[1] = 0;
          victim->loc_hp[2] = 0;
          victim->loc_hp[3] = 0;
          victim->loc_hp[4] = 0;
          victim->loc_hp[5] = 0;
   }

    send_to_char( "You completely regenerate yourself.\n\r", ch );
    return;
}

void spell_clot( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if (IS_BLEEDING(victim,BLEEDING_HEAD))
    {
	act("$n's head stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("Your head stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_HEAD);
    }
    else if (IS_BLEEDING(victim,BLEEDING_THROAT))
    {
	act("$n's throat stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("Your throat stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
    {
	act("The stump of $n's left arm stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your left arm stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
    {
	act("The stump of $n's right arm stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your right arm stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
    }
    else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
    {
	act("The stump of $n's left leg stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your left leg stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
    {
	act("The stump of $n's right leg stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your right leg stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
    }
    else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
    {
	act("The stump of $n's left wrist stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your left wrist stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
    {
	act("The stump of $n's right wrist stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your right wrist stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
    }
    else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
    {
	act("The stump of $n's left ankle stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your left ankle stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
    {
	act("The stump of $n's right ankle stops bleeding.",victim,NULL,NULL,TO_ROOM);
	act("The stump of your right ankle stops bleeding.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
    }
    else send_to_char("They have no wounds to clot.\n\r",ch);
}

void spell_mend( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int ribs = 0;

    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(victim,BROKEN_RIBS_1 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(victim,BROKEN_RIBS_2 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(victim,BROKEN_RIBS_4 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(victim,BROKEN_RIBS_8 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(victim,BROKEN_RIBS_16))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",victim,NULL,NULL,TO_ROOM);
	act("One of your ribs snap back into place.",victim,NULL,NULL,TO_CHAR);
    }
    else if (IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
    {
	act("$n's nose snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your nose snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(victim,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your jaw snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(victim,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",victim,NULL,NULL,TO_ROOM);
	act("Your skull knits itself back together.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(victim,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",victim,NULL,NULL,TO_ROOM);
	act("Your spine knits itself back together.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(victim,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your neck snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(victim,BROKEN_ARM) && !IS_ARM_L(victim,LOST_ARM))
    {
	act("$n's left arm snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left arm snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(victim,BROKEN_ARM) && !IS_ARM_R(victim,LOST_ARM))
    {
	act("$n's right arm snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right arm snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(victim,BROKEN_LEG) && !IS_LEG_L(victim,LOST_LEG))
    {
	act("$n's left leg snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left leg snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(victim,BROKEN_LEG) && !IS_LEG_R(victim,LOST_LEG))
    {
	act("$n's right leg snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right leg snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_THUMB))
    {
	act("$n's left thumb snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left thumb snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left index finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left middle finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left ring finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your left little finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_THUMB))
    {
	act("$n's right thumb snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right thumb snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right index finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right middle finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right ring finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",victim,NULL,NULL,TO_ROOM);
	act("Your right little finger snaps back into place.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(victim,CUT_THROAT))
    {
    	if (IS_SET(victim->loc_hp[6], BLEEDING_THROAT))
    	{
	    send_to_char( "But their throat is still bleeding!\n\r", ch );
	    return;
    	}
	act("The wound in $n's throat closes up.",victim,NULL,NULL,TO_ROOM);
	act("The wound in your throat closes up.",victim,NULL,NULL,TO_CHAR);
	REMOVE_BIT(victim->loc_hp[LOC_BODY],CUT_THROAT);
    }
    else send_to_char("They have no bones to mend.\n\r",ch);
}

void spell_major_creation( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char itemkind[10];
    int itemtype;
    int itempower = 0;
    int vn = 0;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if (arg1[0] == '\0')
    {
	send_to_char( "Item can be one of: Rune, Glyph, Sigil, Book, Page or Pen.\n\r", ch );
	return;
    }

/*
The Rune is the foundation/source of the spell.
The Glyphs form the focus/purpose of the spell.
The Sigils form the affects of the spell.
*/

    if (!str_cmp(arg1,"rune"  ))
    {
	if (arg2[0] == '\0')
	{send_to_char("You know of no such Rune.\n\r",ch);return;}
	itemtype = ITEM_SYMBOL;
	vn = 1;
	sprintf(itemkind,"rune");
	if      (!str_cmp(arg2,"fire"   )) itempower = 1;
	else if (!str_cmp(arg2,"air"    )) itempower = 2;
	else if (!str_cmp(arg2,"earth"  )) itempower = 4;
	else if (!str_cmp(arg2,"water"  )) itempower = 8;
	else if (!str_cmp(arg2,"dark"   )) itempower = 16;
	else if (!str_cmp(arg2,"light"  )) itempower = 32;
	else if (!str_cmp(arg2,"life"   )) itempower = 64;
	else if (!str_cmp(arg2,"death"  )) itempower = 128;
	else if (!str_cmp(arg2,"mind"   )) itempower = 256;
	else if (!str_cmp(arg2,"spirit" )) itempower = 512;
	else if (!str_cmp(arg2,"mastery")) itempower = 1024;
	else
	{send_to_char("You know of no such Rune.\n\r",ch);return;}
	if (!IS_SET(ch->pcdata->magejunk[MPOWER_RUNES], itempower))
	{send_to_char("You know of no such Rune.\n\r",ch);return;}
    }
    else if (!str_cmp(arg1,"glyph" ))
    {
	if (arg2[0] == '\0')
	{send_to_char("You know of no such Glyph.\n\r",ch);return;}
	itemtype = ITEM_SYMBOL;
	vn = 2;
	sprintf(itemkind,"glyph");
	if      (!str_cmp(arg2,"creation"      )) itempower = 1;
	else if (!str_cmp(arg2,"destruction"   )) itempower = 2;
	else if (!str_cmp(arg2,"summoning"     )) itempower = 4;
	else if (!str_cmp(arg2,"transformation")) itempower = 8;
	else if (!str_cmp(arg2,"transportation")) itempower = 16;
	else if (!str_cmp(arg2,"enhancement"   )) itempower = 32;
	else if (!str_cmp(arg2,"reduction"     )) itempower = 64;
	else if (!str_cmp(arg2,"control"       )) itempower = 128;
	else if (!str_cmp(arg2,"protection"    )) itempower = 256;
	else if (!str_cmp(arg2,"information"   )) itempower = 512;
	else
	{send_to_char("You know of no such Glyph.\n\r",ch);return;}
	if (!IS_SET(ch->pcdata->magejunk[MPOWER_GLYPHS], itempower))
	{send_to_char("You know of no such Glyph.\n\r",ch);return;}
    }
    else if (!str_cmp(arg1,"sigil" ))
    {
	if (arg2[0] == '\0')
	{send_to_char("You know of no such Sigil.\n\r",ch);return;}
	itemtype = ITEM_SYMBOL;
	vn = 3;
	sprintf(itemkind,"sigil");
	if      (!str_cmp(arg2,"self"     )) itempower = 1;
	else if (!str_cmp(arg2,"targeting")) itempower = 2;
	else if (!str_cmp(arg2,"area"     )) itempower = 4;
	else if (!str_cmp(arg2,"object"   )) itempower = 8;
	else
	{send_to_char("You know of no such Sigil.\n\r",ch);return;}
	if (!IS_SET(ch->pcdata->magejunk[MPOWER_SIGILS], itempower))
	{send_to_char("You know of no such Sigil.\n\r",ch);return;}
    }
    else if (!str_cmp(arg1,"book"  )) {itemtype = ITEM_BOOK;sprintf(itemkind,"book");}
    else if (!str_cmp(arg1,"page"  )) {itemtype = ITEM_PAGE;sprintf(itemkind,"page");}
    else if (!str_cmp(arg1,"pen"   )) {itemtype = ITEM_TOOL;sprintf(itemkind,"pen");}
    else
    {
	send_to_char( "Item can be one of: Rune, Glyph, Sigil, Book, Page or Pen.\n\r", ch );
	return;
    }
    obj = create_object( get_obj_index( OBJ_VNUM_PROTOPLASM ), 0 );
    obj->level = 0;
    obj->item_type = itemtype;

    if (itemtype == ITEM_SYMBOL)
    {
	sprintf(buf,"%s %s",itemkind,arg2);
	obj->value[vn] = itempower;
    }
    else sprintf(buf,"%s",itemkind);
    if (itemtype == ITEM_TOOL)
    {
	obj->value[0] = TOOL_PEN;
	obj->weight = 1;
	obj->wear_flags = ITEM_TAKE + ITEM_HOLD;
    }
    else if (itemtype == ITEM_BOOK)
    {obj->weight = 50;obj->wear_flags = ITEM_TAKE + ITEM_HOLD;}
    free_string(obj->name);
    obj->name=str_dup(buf);
    if (itemtype == ITEM_SYMBOL) sprintf(buf,"a %s of %s",itemkind,arg2);
    else sprintf(buf,"a %s",itemkind);
    free_string(obj->short_descr);
    obj->short_descr=str_dup(buf);
    sprintf(buf,"A %s lies here.",itemkind);
    free_string(obj->description);
    obj->description=str_dup(buf);

    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker=str_dup(ch->name);

    obj_to_char( obj, ch );
    act( "$p suddenly appears in your hands.", ch, obj, NULL, TO_CHAR );
    act( "$p suddenly appears in $n's hands.", ch, obj, NULL, TO_ROOM );
}

void spell_copy( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *rune;
    OBJ_DATA *page;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {send_to_char("Syntax is: cast 'copy' <rune> <page>.\n\r",ch);return;}
    if ( ( rune = get_obj_carry( ch, arg1 ) ) == NULL )
    {send_to_char("You are not carrying that rune.\n\r",ch);return;}
    if ( rune->item_type != ITEM_SYMBOL )
    {send_to_char("That item isn't a rune.\n\r",ch);return;}
    if ( ( page = get_obj_carry( ch, arg2 ) ) == NULL )
    {send_to_char("You are not carrying that page.\n\r",ch);return;}
    if ( page->item_type != ITEM_PAGE )
    {send_to_char("That item isn't a page.\n\r",ch);return;}

    if (rune->value[1] == RUNE_MASTER)
    {
	if ( IS_SET(page->quest, QUEST_MASTER_RUNE) )
	{
	    send_to_char( "There is already a master rune draw on this page.\n\r", ch );
	    return;
	}
	else if (page->value[0] + page->value[1] + page->value[2] + 
	    page->value[3] > 0)
	{
	    send_to_char( "There is already a spell on this page.\n\r", ch );
	    return;
	}
	else
	{
	    act("You copy $p rune onto $P.",ch,rune,page,TO_CHAR);
	    act("$n copies $p rune onto $P.",ch,rune,page,TO_ROOM);
	    SET_BIT(page->quest, QUEST_MASTER_RUNE);
                extract_obj( rune );
	}
	return;
    }
    else if ( IS_SET(page->quest, QUEST_MASTER_RUNE) )
    {
	send_to_char( "There is already a master rune draw on this page.\n\r", ch );
	return;
    }
    else if (rune->value[1] > 0 && !IS_SET(page->value[1], rune->value[1]))
	page->value[1] += rune->value[1];
    else if (rune->value[1] > 0)
	{send_to_char("That rune has already been copied onto the page.\n\r",ch);return;}
    else if (rune->value[2] > 0 && !IS_SET(page->value[2], rune->value[2]))
	page->value[2] += rune->value[2];
    else if (rune->value[2] > 0)
	{send_to_char("That glyph has already been copied onto the page.\n\r",ch);return;}
    else if (rune->value[3] > 0 && !IS_SET(page->value[3], rune->value[3]))
	page->value[3] += rune->value[3];
    else if (rune->value[3] > 0)
	{send_to_char("That sigil has already been copied onto the page.\n\r",ch);return;}

    act("You copy $p onto $P.",ch,rune,page,TO_CHAR);
    act("$n copies $p onto $P.",ch,rune,page,TO_ROOM);
                extract_obj( rune );
}

void spell_insert_page( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *page;
    OBJ_DATA *book;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {send_to_char("Syntax is: cast 'insert page' <page> <book>.\n\r",ch);return;}
    if ( ( page = get_obj_carry( ch, arg1 ) ) == NULL )
    {send_to_char("You are not carrying that page.\n\r",ch);return;}
    if ( page->item_type != ITEM_PAGE )
    {send_to_char("That item isn't a page.\n\r",ch);return;}
    if ( ( book = get_obj_carry( ch, arg2 ) ) == NULL )
    {send_to_char("You are not carrying that book.\n\r",ch);return;}
    if ( book->item_type != ITEM_BOOK )
    {send_to_char("That item isn't a book.\n\r",ch);return;}
    if ( IS_SET(book->value[1], CONT_CLOSED) )
    {send_to_char("First you need to open it!\n\r",ch);return;}

    obj_from_char(page);
    obj_to_obj(page,book);
    book->value[3] += 1;
    book->value[2] = book->value[3];
    page->value[0] = book->value[3];
    page->specpower = book->value[3]+1;

    act("You insert $p into $P.",ch,page,book,TO_CHAR);
    act("$n inserts $p into $P.",ch,page,book,TO_ROOM);
}

void spell_remove_page( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_DATA *page;
    OBJ_DATA *book;
    OBJ_DATA *page_next;
    int count = 0;

    target_name = one_argument( target_name, arg1 );

    if ( arg1[0] == '\0' )
    {send_to_char("Syntax is: cast 'remove page' <book>.\n\r",ch);return;}
    if ( ( book = get_obj_carry( ch, arg1 ) ) == NULL )
    {send_to_char("You are not carrying that book.\n\r",ch);return;}
    if ( book->item_type != ITEM_BOOK )
    {send_to_char("That item isn't a book.\n\r",ch);return;}
    if ( IS_SET(book->value[1], CONT_CLOSED) )
    {send_to_char("First you need to open it!\n\r",ch);return;}
    if ( book->value[2] == 0 )
    {send_to_char("You cannot remove the index page!\n\r",ch);return;}

    if ( ( page = get_page(book, book->value[2]) ) == NULL )
    {
	send_to_char("The page seems to have been torn out.\n\r",ch);
	return;
    }

    obj_from_obj(page);
    obj_to_char(page,ch);
    page->value[0] = 0;

    act("You remove $p from $P.",ch,page,book,TO_CHAR);
    act("$n removes $p from $P.",ch,page,book,TO_ROOM);

    for ( page = book->contains; page != NULL; page = page_next )
    {
	page_next = page->next_content;
	count += 1;
	page->value[0] = count;
    }
    book->value[3] = count;

    if (book->value[2] > book->value[3]) book->value[2] = book->value[3];
}

void spell_minor_creation( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char itemkind[10];
    int itemtype;
    OBJ_DATA *obj;

    return;

    target_name = one_argument( target_name, arg );

    if      (!str_cmp(arg,"potion")) {itemtype = ITEM_POTION;sprintf(itemkind,"potion");}
    else if (!str_cmp(arg,"scroll")) {itemtype = ITEM_SCROLL;sprintf(itemkind,"scroll");}
    else if (!str_cmp(arg,"wand"  )) {itemtype = ITEM_WAND;sprintf(itemkind,"wand");}
    else if (!str_cmp(arg,"staff" )) {itemtype = ITEM_STAFF;sprintf(itemkind,"staff");}
    else if (!str_cmp(arg,"pill"  )) {itemtype = ITEM_PILL;sprintf(itemkind,"pill");}
    else
    {
	send_to_char( "Item can be one of: Potion, Scroll, Wand, Staff or Pill.\n\r", ch );
	return;
    }
    obj = create_object( get_obj_index( OBJ_VNUM_PROTOPLASM ), 0 );
    obj->item_type = itemtype;

    sprintf(buf,"%s %s",ch->name,itemkind);
    free_string(obj->name);
    obj->name=str_dup(buf);
    sprintf(buf,"%s's %s",ch->name, itemkind);
    free_string(obj->short_descr);
    obj->short_descr=str_dup(buf);
    sprintf(buf,"%s's %s lies here.",ch->name,itemkind);
    free_string(obj->description);
    obj->description=str_dup(buf);

    obj->weight = 10;

    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker=str_dup(ch->name);

    obj_to_char( obj, ch );
    act( "$p suddenly appears in your hands.", ch, obj, NULL, TO_CHAR );
    act( "$p suddenly appears in $n's hands.", ch, obj, NULL, TO_ROOM );
    return;
}

void spell_brew( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to brew, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
	send_to_char( "You are not adept at that spell.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "That is not a potion.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an empty potion bottle.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = sn;
    if (obj->value[0] >= 25) obj->value[2] = sn; else obj->value[2] = -1;
    if (obj->value[0] >= 50) obj->value[3] = sn; else obj->value[3] = -1;
    free_string(obj->name);
    sprintf(buf,"%s potion %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s potion of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s potion is lying here.",col);
    obj->description=str_dup(buf);
    act("You brew $p.",ch,obj,NULL,TO_CHAR);
    act("$n brews $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_scribe( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to scribe, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
	send_to_char( "You are not adept at that spell.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_SCROLL )
    {
	send_to_char( "That is not a scroll.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an empty scroll parchment.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = sn;
    if (obj->value[0] >= 25) obj->value[2] = sn; else obj->value[2] = -1;
    if (obj->value[0] >= 50) obj->value[3] = sn; else obj->value[3] = -1;
    free_string(obj->name);
    sprintf(buf,"%s scroll %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s scroll of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s scroll is lying here.",col);
    obj->description=str_dup(buf);
    act("You scribe $p.",ch,obj,NULL,TO_CHAR);
    act("$n scribes $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_carve( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to carve, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
	send_to_char( "You are not adept at that spell.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_WAND )
    {
	send_to_char( "That is not a wand.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an unenchanted wand.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = (obj->value[0] / 5) + 1;
    obj->value[2] = (obj->value[0] / 5) + 1;
    obj->value[3] = sn;
    free_string(obj->name);
    sprintf(buf,"%s wand %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s wand of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s wand is lying here.",col);
    obj->description=str_dup(buf);
    obj->wear_flags = ITEM_TAKE + ITEM_HOLD;
    act("You carve $p.",ch,obj,NULL,TO_CHAR);
    act("$n carves $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_engrave( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to engrave, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
	send_to_char( "You are not adept at that spell.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_STAFF )
    {
	send_to_char( "That is not a staff.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an unenchanted staff.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = (ch->spl[0]+1)/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = (ch->spl[1]+1)/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = (ch->spl[2]+1)/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = (ch->spl[3]+1)/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = (ch->spl[4]+1)/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = (obj->value[0] / 10) + 1;
    obj->value[2] = (obj->value[0] / 10) + 1;
    obj->value[3] = sn;
    free_string(obj->name);
    sprintf(buf,"%s staff %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s staff of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s staff is lying here.",col);
    obj->description=str_dup(buf);
    obj->wear_flags = ITEM_TAKE + ITEM_HOLD;
    act("You engrave $p.",ch,obj,NULL,TO_CHAR);
    act("$n engraves $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_bake( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to bake, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
	send_to_char( "You are not adept at that spell.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_PILL )
    {
	send_to_char( "That is not a pill.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an unused pill.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = sn;
    if (obj->value[0] >= 25) obj->value[2] = sn; else obj->value[2] = -1;
    if (obj->value[0] >= 50) obj->value[3] = sn; else obj->value[3] = -1;
    free_string(obj->name);
    sprintf(buf,"%s pill %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s pill of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s pill is lying here.",col);
    obj->description=str_dup(buf);
    act("You bake $p.",ch,obj,NULL,TO_CHAR);
    act("$n bakes $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_mount( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
 
    if ( IS_NPC( ch ) || !( IS_CLASS( ch, CLASS_PALADIN ) || IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) ) )
       return;

    if ( ch->mounted > 0 )
       {
          send_to_char( "You are already riding.\n\r",ch );
          return;
       }

    ch->wait = 12;

    if ( IS_CLASS( ch, CLASS_PALADIN ) )
    {
	victim=create_mobile( get_mob_index( MOB_VNUM_MOUNT ) );
	victim->level = 950;
	victim->armor = -50000;
	victim->hitroll = 1250000;
	victim->damroll = 1250000;
	victim->hit = 3000000;
	victim->max_hit = 3000000;

	free_string(victim->lord);
	victim->lord = str_dup(ch->name);

	free_string(victim->name);
	victim->name = str_dup("mount white horse war-horse");
	sprintf(buf,"%s's holy war-horse",ch->name);
	free_string(victim->short_descr);
	victim->short_descr = str_dup(buf);
	free_string(victim->long_descr);
	victim->long_descr = str_dup("A beautiful war-horse stands here.\n\r");

	SET_BIT(victim->act, ACT_MOUNT);
	char_to_room( victim, ch->in_room );
	act( "$N emerges from a beam of holy energy.", ch, NULL, victim, TO_CHAR );
	act( "$N emerges from a beam of holy energy.", ch, NULL, victim, TO_ROOM );

      ch->mounted = IS_RIDING;
      victim->mounted = IS_MOUNT;
      ch->mount = victim;
      victim->mount = ch;
      victim->master = ch;
      act( "You pat $N's head as you clamber onto $S back.", ch, NULL, victim, TO_CHAR );
      act( "$n pats $N's head as $e clamber onto $S back.", ch, NULL, victim, TO_NOTVICT );
    }
    else if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
    {
	victim=create_mobile( get_mob_index( MOB_VNUM_MOUNT ) );
	victim->level = 950;
	victim->armor = -100000;
	victim->hitroll = 2250000;
	victim->damroll = 2250000;
	victim->hit = 3000000;
	victim->max_hit = 3000000;

	free_string(victim->lord);
	victim->lord = str_dup(ch->name);

	free_string(victim->name);
	victim->name = str_dup("mount steed bone");
	sprintf(buf,"%s's bone steed",ch->name);
	free_string(victim->short_descr);
	victim->short_descr = str_dup(buf);
	free_string(victim->long_descr);
	victim->long_descr = str_dup("An ugly sight of a beast stands here.\n\r");

	SET_BIT(victim->act, ACT_MOUNT);
	char_to_room( victim, ch->in_room );
	act( "You beckon $N into the room with a flick of your wrist.", ch, NULL, victim, TO_CHAR );
	act( "$n beckons $N into the room with a flick of $s wrist.", ch, NULL, victim, TO_ROOM );

      ch->mounted = IS_RIDING;
      victim->mounted = IS_MOUNT;
      ch->mount = victim;
      victim->mount = ch;
      victim->master = ch;
      act( "You scowl at $N as you clamber onto $S back.", ch, NULL, victim, TO_CHAR );
      act( "$n scowls at $N as $e clamber onto $S back.", ch, NULL, victim, TO_NOTVICT );
    }
}

void spell_hell_hound( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_DEMON) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( IS_SET(ch->extra, EXTRA_OSWITCH) )
       {
          send_to_char("Not in Objectform.\n\r",ch);
          return;
       }

    if ( ch->pcdata->followers > 0 )
       {
          send_to_char("Nothing happens.\n\r",ch);
          return;
       }

    ch->pcdata->followers++;
    ch->wait = 6;

    victim=create_mobile( get_mob_index( 16 ) );

    if ( victim == NULL )
       {
          stc("BUG! Contact the nearest Administrator.\n\r", ch );
          return;
       }

    victim->level = 950;
    SET_BIT(victim->act, ACT_MOUNT);
    victim->armor = -20000;
    victim->hitroll = 1000000;
    victim->damroll = 1000000;
    victim->hit = number_range(2000000,3000000);
    victim->max_hit = victim->hit;

    char_to_room( victim, ch->in_room );

    act( "$N appears in a puff of demonic smoke.", ch, NULL, victim, TO_CHAR );
    act( "$N appears in a puff of demonic smoke.", ch, NULL, victim, TO_ROOM );

    add_follower( victim, ch );
    ch->wait = 12;
}

void spell_scan( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found = FALSE;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->condition < 100 && can_see_obj( ch, obj ) )
	{
	    found = TRUE;
	    act("$p needs repairing.",ch,obj,NULL,TO_CHAR);
	}
    }
    if ( !found )
    {
	send_to_char( "None of your equipment needs repairing.\n\r", ch );
	return;
    }
    return;
}

void spell_repair( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found = FALSE;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->condition < 100 )
	{
	    found = TRUE;
	    obj->condition = 100;
	    act("$p magically repairs itself.",ch,obj,NULL,TO_CHAR);
	    act("$p magically repairs itself.",ch,obj,NULL,TO_ROOM);
	}
    }
    if ( !found )
    {
	send_to_char( "None of your equipment needs repairing.\n\r", ch );
	return;
    }
    return;
}

void spell_spellproof( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->chobj != NULL )
    {
	send_to_char("Your spell has no affect.\n\r",ch);
	return;
    }

    if ( IS_SET(obj->quest, QUEST_SPELLPROOF) )
    {
	send_to_char( "That item is already resistance to spells.\n\r", ch );
	return;
    }

    SET_BIT(obj->quest, QUEST_SPELLPROOF);
    act("$p shimmers for a moment.",ch,obj,NULL,TO_CHAR);
    act("$p shimmers for a moment.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_preserve( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if (IS_SET(obj->quest, QUEST_CLONED))
{
send_to_char("You cant preserve cloned items.",ch);
return;
}

    if ( obj->timer < 1 )
    {
	send_to_char( "That item doesn't require preserving.\n\r", ch );
	return;
    }
    if ( obj->chobj != NULL )
    {
	send_to_char( "You cannot preserve that.\n\r", ch );
	return;
    }

    obj->timer = -1;
    act("$p shimmers for a moment.",ch,obj,NULL,TO_CHAR);
    act("$p shimmers for a moment.",ch,obj,NULL,TO_ROOM);
    return;
}

void spell_chaos_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_DARK;

    if ( level == 9999 )
       {
          if ( IS_NPC( ch ) )
             hurt_person( ch, victim, 750000, TRUE );
             else hurt_person( ch, victim, 500000, TRUE );

          return;
       }

    dam = char_damcap(ch);
    damage( ch, victim, dam, sn - 2000 );
}

void spell_resistance( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->chobj != NULL )
    {
	send_to_char("Your spell has no affect.\n\r",ch);
	return;
    }
    if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	send_to_char( "Not on artifacts.\n\r", ch );
	return;
    }

    if ( obj->resistance < 10 )
    {
	send_to_char( "You cannot make that item any more resistant.\n\r", ch );
	return;
    }

    obj->resistance = 10;
    act("$p sparkles for a moment.",ch,obj,NULL,TO_CHAR);
    act("$p sparkles for a moment.",ch,obj,NULL,TO_ROOM);
}

void spell_drowfire( int sn, int level, CHAR_DATA *ch, void *vo) 
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;

	act("You chant some arcane words on $N.",ch,NULL,victim,TO_CHAR);
	act("$n chants some arcane words on $N.",ch,NULL,victim,TO_NOTVICT);
	act("$n chants some arcane words on you.",ch,NULL,victim,TO_VICT);

      if ( is_affected(victim, skill_lookup("blessing of the saints") ) )
      {
            stc("They are protected by the holy saints.\n\r",ch);
            return;
      }

	if (IS_AFFECTED(victim, AFF_DROWFIRE)) {
		send_to_char("They are already affected by drowfire.\n\r",ch);
		return;}

	af1.type	      = sn;
	af1.location	= APPLY_CON;
      af1.modifier      = -3;

	af1.duration	= 20;
	af1.bitvector	= AFF_DROWFIRE;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_STR;
      af2.modifier	= -2;

	af2.duration	= 20;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	af2.type	= sn;
	af2.location	= APPLY_DEX;
      af2.modifier	= -2;

	af2.duration	= 20;
	af2.bitvector	= 0;
	affect_to_char(victim, &af2);

	send_to_char("You are engulfed in drowfire!\n\r", victim);
	act("$n is engulfed in drowfire!",victim,NULL,NULL,TO_ROOM);
}

void spell_web( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( !IS_NPC(ch) )
       {
          act("You point your finger at $N and a web flies from your hand!",ch,NULL,victim,TO_CHAR);
          act("$n points $s finger at $N and a web flies from $s hand!",ch,NULL,victim,TO_NOTVICT);
          act("$n points $s finger at you and a web flies from $s hand!",ch,NULL,victim,TO_VICT);
       }
       else {
               act("You open your mouth wide spitting out a spew of webbing at $N!",ch,NULL,victim,TO_CHAR);
               act("$n opens $s mouth wide spitting out a spew of webbing at you!",ch,NULL,victim,TO_VICT);
               act("$n opens $s mouth wide spitting out a spew of webbing at $N!",ch,NULL,victim,TO_NOTVICT);
            }

    if ( IS_AFFECTED(victim, AFF_WEBBED) )
    {
	send_to_char( "But they are already webbed!\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) )
       {
          if ( is_safe(ch, victim) ) return;

          if ( saves_spell( level, victim ) && victim->position >= POS_FIGHTING )
          {
	        send_to_char( "You dodge the web!\n\r", victim );
 	        act("$n dodges the web!",victim,NULL,NULL,TO_ROOM);
	        return;
          }
       }

    af.type      = sn;
    af.location  = APPLY_AC;
    af.modifier  = 200;
    af.duration  = number_range(1,2);
    af.bitvector = AFF_WEBBED;
    affect_to_char( victim, &af );
    send_to_char( "You are coated in a sticky web!\n\r", victim );
    act("$n is coated in a sticky web!",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_polymorph( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    char buf [MAX_INPUT_LENGTH];

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	send_to_char("You cannot polymorph from this form.\n\r",ch);
	return;
    }
    if ( ch->position == POS_FIGHTING || is_affected( ch, sn ) )
	return;

    if ( !str_cmp( target_name, "frog" ) )
    {
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a frog!",ch,NULL,NULL,TO_ROOM);
	send_to_char("You polymorph into a frog!\n\r",ch);
	af.type      = sn;
	af.duration  = number_range(3,5);
	af.location  = APPLY_POLY;
	af.modifier  = POLY_FROG;
	af.bitvector = AFF_POLYMORPH;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the frog",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    else if ( !str_cmp( target_name, "fish" ) )
    {
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a fish!",ch,NULL,NULL,TO_ROOM);
	send_to_char("You polymorph into a fish!\n\r",ch);
	af.type      = sn;
	af.duration  = number_range(3,5);
	af.location  = APPLY_POLY;
	af.modifier  = POLY_FISH;
	af.bitvector = AFF_POLYMORPH;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the fish",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    else if ( !str_cmp( target_name, "raven" ) )
    {
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a raven!",ch,NULL,NULL,TO_ROOM);
	send_to_char("You polymorph into a raven!\n\r",ch);
	af.type      = sn;
	af.duration  = number_range(3,5);
	af.location  = APPLY_AC;
	af.modifier  = -150;
	if (IS_AFFECTED(ch, AFF_FLYING)) af.bitvector = AFF_POLYMORPH;
	else af.bitvector = AFF_POLYMORPH + AFF_FLYING;
	affect_to_char( ch, &af );
	af.location  = APPLY_POLY;
	af.modifier  = POLY_RAVEN;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the raven",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
	else if ( !str_cmp( target_name, "cheese" ) )
	{
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a piece of #yCHEESE!#n",ch,NULL,NULL,TO_ROOM);
	send_to_char("You polymorph into a piece of #ycheddar cheese#n!\n\r",ch);
	af.type      = sn;
	af.duration  = 1000;
	af.location  = APPLY_POLY;
	af.modifier  = POLY_CHEESE;
    af.bitvector = AFF_POLYMORPH;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the piece of cheese",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
	}
    send_to_char( "You can polymorph into a frog, a fish, cheese or an raven.\n\r", ch );
    return;
}
/*
void spell_contraception( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_CONTRACEPTION) )
	return;
    if (victim->sex != SEX_FEMALE) return;
    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CONTRACEPTION;
    affect_to_char( victim, &af );
    return;
}
*/
void spell_find_familiar( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;

    if ( target_name[0] == '\0' )
    {
	send_to_char( "Your familiar can be a frog, a raven, a cat or a dog.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
    {
	send_to_char( "But you already have a familiar!\n\r", ch );
	return;
    }

    if ( !str_cmp( target_name, "frog" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_FROG ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else if ( !str_cmp( target_name, "raven" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_RAVEN ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else if ( !str_cmp( target_name, "cat" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_CAT ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else if ( !str_cmp( target_name, "dog" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_DOG ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else 
    {
	send_to_char( "Your familiar can be a frog, a raven, a cat or a dog.\n\r", ch );
	return;
    }

    char_to_room( victim, ch->in_room );
    act( "You make a few gestures and $N appears in a swirl of smoke.", ch, NULL, victim, TO_CHAR );
    act( "$n makes a few gestures and $N appears in a swirl of smoke.", ch, NULL, victim, TO_ROOM );

    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    return;
}

void spell_improve( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	send_to_char( "Not on artifacts.\n\r", ch );
	return;
    }
    if ( obj->pIndexData->vnum != OBJ_VNUM_PROTOPLASM )
    {
	send_to_char( "You cannot enhance this object.\n\r", ch );
	return;
    }
    if (IS_SET(obj->quest, QUEST_IMPROVED))
    {
	send_to_char( "This item has already been improved.\n\r", ch );
	return;
    }
    else if ( obj->points < 750 && obj->item_type != ITEM_WEAPON )
    {
	send_to_char( "The object must be worth at least 750 gold pieces and must be created.\n\r", ch );
	return;
    }
    else if ( obj->points < 1500 && obj->item_type == ITEM_WEAPON )
    {
	send_to_char( "The object must be worth at least 1500 gold pieces and must be created.\n\r", ch );
	return;
    }
    REMOVE_BIT(obj->quest, QUEST_STR);
    REMOVE_BIT(obj->quest, QUEST_DEX);
    REMOVE_BIT(obj->quest, QUEST_INT);
    REMOVE_BIT(obj->quest, QUEST_WIS);
    REMOVE_BIT(obj->quest, QUEST_CON);
    REMOVE_BIT(obj->quest, QUEST_HITROLL);
    REMOVE_BIT(obj->quest, QUEST_DAMROLL);
    REMOVE_BIT(obj->quest, QUEST_HIT);
    REMOVE_BIT(obj->quest, QUEST_MANA);
    REMOVE_BIT(obj->quest, QUEST_MOVE);
    REMOVE_BIT(obj->quest, QUEST_AC);
    SET_BIT(obj->quest, QUEST_IMPROVED);
    act("$p flickers for a moment.",ch,obj,NULL,TO_CHAR);
    act("$p flickers for a moment.",ch,obj,NULL,TO_ROOM);
    return;
}
void spell_spiritkiss( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_HITROLL;
    af.modifier  = 30000;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = 1000;
    af.location  = APPLY_DAMROLL;
    af.modifier  = 30000;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -10;
    affect_to_char( victim, &af );
    act( "$n is filled with spiritual power.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are blessed by the spirits.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
}

void spell_jailwater( int sn, int level, CHAR_DATA *ch, void *vo )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( victim == NULL )
      return;

   if ( victim->in_room == NULL )
      return;

   if ( !IS_SET(victim->in_room->room_flags, ROOM_ENTOMB) )
      SET_BIT(victim->in_room->room_flags, ROOM_ENTOMB);

   act("You exclaim '#yLet this icy prison be your last home!!! Muahahahaha!!#n'", ch, NULL, NULL, TO_CHAR);
   act("$N exclaims '#yLet this icy prison be your last home!!! Muahahahaha!!#n'", victim, NULL, ch, TO_ROOM);
   act("$N calls forth sheets of ice, blocking your path in every direction.", victim, NULL, ch, TO_ROOM);
   send_to_char("You call forth sheets of ice to entrap your prey.\n\r", ch);
   send_to_char("You grin wickedly.\n\r",ch);
}

void spell_lshield( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You get down on your knees and offer the sacrifice of a #gwood#g elf's#c eye#n.",ch,NULL,NULL,TO_CHAR);
   act("$n gets down on $s knees and offers the sacrifice of a #gwood#g elf's#c eye#n.",ch,NULL,NULL,TO_ROOM);
   act("Spinning down you see a sacred enchantment designed to protect its recipient.",ch,NULL,NULL,TO_CHAR);
   act("Spinning down $n sees a sacred enchantment designed to protect its recipient.",ch,NULL,NULL,TO_ROOM);
   act("Swirling your hands around you mutter a few magical words '#yYrr eliyr belor Lolth#n' and enchant $N.",ch,NULL,victim,TO_CHAR);
   act("Swirling $s hands around $n mutters a few magical words '#yYrr eliyr belor Lolth#n' and enchants $N.",ch,NULL,victim,TO_NOTVICT);
   act("Swirling $s hands around $n mutters a few magical words '#yYrr eliyr belor Lolth#n' and ehcnats you.",ch,NULL,victim,TO_VICT);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 200;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_blur( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You raise your hands and twiddle your fingers in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n raises $s hands and twiddles $s fingers in the air.",ch,NULL,NULL,TO_ROOM);
   act("Drawing a circle in mid air you unleash a #0time#c bolt#n onto $N.",ch,NULL,victim,TO_CHAR);
   act("Drawing a circle in mid air $n unleashes a #0time#c bolt#n onto $N.",ch,NULL,victim,TO_NOTVICT);
   act("Drawing a circle in mid air $n unleashes a #0time#c bolt#n onto you.",ch,NULL,victim,TO_VICT);
   act("You feel ten times as #zfast#n; like you are one with #0time#n.",victim,NULL,NULL,TO_CHAR);
   act("$n appears ten times as #zfast#n; like $e is one with #0time#n.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_DEX;
   af.modifier = 2;
   af.duration = 500;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_vtouch( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   bool test = FALSE;

   act("You slowly stride towards $N and grab hold of their hand.",ch,NULL,victim,TO_CHAR);
   act("$n slowly strides towards $N and grabs hold of their hand.",ch,NULL,victim,TO_NOTVICT);
   act("$n slowly strides towards you and grabs hold of your hand.",ch,NULL,victim,TO_VICT);
   act("Twisting $N's hand you whisper '#RVampires Touch..#n'",ch,NULL,victim,TO_CHAR);
   act("Twisting $N's hand $n whispers '#RVampires Touch..#n'",ch,NULL,victim,TO_NOTVICT);
   act("Twisting your hand $n whispers '#RVampires Touch..#n'",ch,NULL,victim,TO_VICT);
   act("You feel your lifeforce being ViCiOuSlY #Rdrained#0 away#n.",victim,NULL,NULL,TO_CHAR);
   act("$n feels $s lifeforce being ViCiOuSlY #Rdrained#0 away#n.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 500;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   if ( test == TRUE )
      SET_BIT(victim->more2, MORE2_SHIELD);
}

void spell_invun( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You bow infront of $N and give them the sacred touch of #0Lolth#n.",ch,NULL,victim,TO_CHAR);
   act("$n bows infront of $N and gives them the sacred touch of #0Lolth#n.",ch,NULL,victim,TO_NOTVICT);
   act("$n bows infront of you and gives you the sacred touch of #0Lolth#n.",ch,NULL,victim,TO_VICT);
   act("You then turn and bow your head in #Pprayer#n thanking #0Lolth#n for temporary #Cinvunerability#n!",victim,NULL,NULL,TO_CHAR);
   act("$n then turns and bows $s head in #Pprayer#n thanking #0Lolth#n for temporary #Cinvunerability#n!",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 500;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_lcharm( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You fearlessly grab at $N's neck.",ch,NULL,victim,TO_CHAR);
   act("$n fearlessly grabs at $N's neck.",ch,NULL,victim,TO_NOTVICT);
   act("$n fearlessly grabs at your neck.",ch,NULL,victim,TO_VICT);
   act("You go down on your knees and scream '#RGood lord! WHAT HAVE I DONE?!'#n",victim,NULL,NULL,TO_CHAR);
   act("$n goes down on $s knees and screams '#RGood lord! WHAT HAVE I DONE?!'#n",victim,NULL,NULL,TO_ROOM);
   act("You suddenly appear to be charmed into fighting with less #rheart#n.",victim,NULL,NULL,TO_CHAR);
   act("$n suddenly appears to be charmed into fighting with less #rheart#n.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 15;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_ckill( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   af.type = sn;
   af.location = APPLY_STR;
   af.modifier = -4;
   af.duration = 1;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_drow_bless( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You get down on your knees and pray for Lolth's help.",ch,NULL,NULL,TO_CHAR);
   act("$n gets down on $s knees and prays for Lolth's help.",ch,NULL,NULL,TO_ROOM);
   act("#0dark #Pswirls#n of energy surround your body forming a #0mystical#n aura.",victim,NULL,NULL,TO_CHAR);
   act("#0dark #Pswirls#n of energy surround $n's body forming a #0mystical#n aura.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_prot( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You place both your hands upon $N's head.",ch,NULL,victim,TO_CHAR);
   act("$n places both $s hands upon $N's head.",ch,NULL,victim,TO_NOTVICT);
   act("$n places both $s hands upon your head.",ch,NULL,victim,TO_VICT);
   act("You mutter some sacred words as #0dark#c energy#P transcends#n upon $N.",ch,NULL,victim,TO_CHAR);
   act("$n mutters some sacred words as #0dark#c energy#P transcends#n upon $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n mutters some sacred words as #0dark#c energy#P transcends#n upon you.",ch,NULL,victim,TO_VICT);
   act("You feel totally #Cbathed#n and relaxed as #0darkness#n flows through your #Rveins#n.",victim,NULL,NULL,TO_CHAR);
   act("$n feels totally #Cbathed#n and relaxed as #0darkness#n flows through $s #Rveins#n.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_lwrath( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You mass up a large #0dark#n ball of #Rhatred#n within your #0shady#n core...",ch,NULL,NULL,TO_CHAR);
   act("$n masses up a large #0dark#n ball of #Rhatred#n within $s #0shady#n core...",ch,NULL,NULL,TO_ROOM);
   act("You point your long bony finger at $N and your eyes glare #Rred#n as you bring a path of utter #0destruction#n!",ch,NULL,victim,TO_CHAR);
   act("$n points $s long bony finger at $N and $s eyes glare #Rred#n as $e brings a path of utter #0destruction#n!",ch,NULL,victim,TO_NOTVICT);
   act("$n points $s long bony finger at you and $s eyes glare #Rred#n as $e brings a path of utter #0destruction#n!",ch,NULL,victim,TO_VICT);
   act("You tremble as all desire to #Rkill#n is poured away from your soul.",victim,NULL,NULL,TO_CHAR);
   act("$n trembles as all desire to #Rkill#n is poured away from $s soul.",victim,NULL,NULL,TO_ROOM);
  
   af.type = sn;
   af.location = APPLY_DEX;
   af.modifier = -3;
   af.duration = 15;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_CON;
   af.modifier = -3;
   af.duration = 15;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_aid( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You twirl your hands then place them on $N.",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s hands then places them on $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n twirls $s hands then places them on you.",ch,NULL,victim,TO_VICT);
   act("You gasp as you are blinded by #ybright#c blue#n light!",victim,NULL,NULL,TO_CHAR);
   act("$n gasps as $e is blinded by #ybright#c blue#n light!",victim,NULL,NULL,TO_ROOM);
   act("You feel bathed as the #Lbluish#n energy rhythmically #zpulsates#n around your body.",victim,NULL,NULL,TO_CHAR);
   act("$n feels bathed as the #Lbluish#n energy rhythmically #zpulsates#n around $s body.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_fblade( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You place your hands high in the air as your eyes glow #rred#n.",ch,NULL,NULL,TO_CHAR);
   act("$n places $s hands high in the air as $s eyes glow #rred#n.",ch,NULL,NULL,TO_ROOM);
   act("Slowly deep #rcrimson#n flames like #rblood#n encircle you to form a dripping #rblade#n!",ch,NULL,NULL,TO_CHAR);
   act("Slowly deep #rcrimson#n flames like #rblood#n encircle you to form a dripping #rblade#n!",ch,NULL,NULL,TO_ROOM);  

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 500;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_prayer( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You get down on your knees and pray for Lolth's help.",ch,NULL,NULL,TO_CHAR);
   act("$n gets down on $s knees and prays for Lolth's help.",ch,NULL,NULL,TO_ROOM);
   act("#0dark #Pswirls#n of energy surround your body forming a #0mystical#n aura!",victim,NULL,NULL,TO_CHAR);
   act("#0dark #Pswirls#n of energy surround $n's body forming a #0mystical#n aura!",victim,NULL,NULL,TO_ROOM);
   act("#zYou shiver as a mystical #0dark#n#z prayer #Pswirls#n#z around your #0mystical#n#z aura#n.",victim,NULL,NULL,TO_CHAR);
   act("#z$n shivers as a mystical #0dark#n#z prayer #Pswirls#n#z around your #0mystical#n#z aura#n.",victim,NULL,NULL,TO_ROOM);
   act("#zYou shiver as a mystical #0dark#n#z prayer #Pswirls#n#z around your #0mystical#n#z aura#n.",victim,NULL,NULL,TO_CHAR);
   act("#z$n shivers as a mystical #0dark#n#z prayer #Pswirls#n#z around your #0mystical#n#z aura#n.",victim,NULL,NULL,TO_ROOM);
 
   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_shammer( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You place your hands on the ground it mystically #zshimmers#n.",ch,NULL,NULL,TO_CHAR);
   act("$n places $s hands on the ground and it mystically #zshimmers#n.",ch,NULL,NULL,TO_ROOM);
   act("Your eyes grow a deep #0black#n as you whisper into the wind '#0soul hammer...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n's eyes grow a deep #0black#n as $e whispers into the wind '#0soul hammer...#n'",ch,NULL,NULL,TO_ROOM);
   act("The #0dark#n aura encasing your body violently #zshimmers#n in the #ylight#n.",ch,NULL,NULL,TO_CHAR);
   act("The #0dark#n aura encasing $n's body violently #zshimmers#n in the #ylight#n.",ch,NULL,NULL,TO_ROOM);    

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_fshield( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You place your palm on $N's head.",ch,NULL,victim,TO_CHAR);
   act("$n places $s palm on $N's head.",ch,NULL,victim,TO_NOTVICT);
   act("$n places $s palm on your head.",ch,NULL,victim,TO_VICT);
   act("You shudder as you feel yourself surrounded by a #rfire#n aura.",victim,NULL,NULL,TO_CHAR);
   act("$n shudders as $e feels surrounded by a #rfire#n aura.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 500;

   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_shille( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You push your hands against $N's weapon.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s hands against $N's weapon.",ch,NULL,victim,TO_NOTVICT); 
   act("$n pushes $s hands against your weapon.",ch,NULL,victim,TO_VICT);
   act("You watch in wonder as your weapon becomes outlined with a #Lblue#n aura making it more powerful!",victim,NULL,NULL,TO_CHAR);
   act("$n watches in wonder as $s weapon becomes outlined with a #Lblue#n aura making it more powerful!",victim,NULL,NULL,TO_ROOM);
   
   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 500;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_lolthst( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("You place both your hands on $N's forehead and whisper '#yLolth's strength shall guide you... Always#n'",ch,NULL,victim,TO_CHAR);
   act("$n places both $s hands on $N's forehead and whispers '#yLolth's strength shall guide you... Always#n'",ch,NULL,victim,TO_NOTVICT);
   act("$n places both $s hands on your forehead and whispers '#yLolth's strength shall guide you... Always#n'",ch,NULL,victim,TO_VICT);
   act("You rejoice happily as every single aspect of your physique and mentality is improved.",victim,NULL,NULL,TO_CHAR);
   act("$n rejoices happily as every single aspect of $s physique and mentality is improved.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_STR;
   af.modifier = 3;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_DEX;
   af.modifier = 3;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
  
   af.type = sn;
   af.location = APPLY_CON;
   af.modifier = 3;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_WIS;
   af.modifier = 3;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_INT;
   af.modifier = 3;
   af.duration = 1000;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_dpoison( int sn, int level, CHAR_DATA *ch, void *vo )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

   af.type      = sn;
   af.duration  = 10;
   af.location  = APPLY_STR;
   af.modifier  = -1;
   af.bitvector = AFF_POISON;
   affect_to_char( victim, &af );

   af.type      = sn;
   af.duration  = 10;
   af.location  = APPLY_INT;
   af.modifier  = -2;
   af.bitvector = AFF_POISON;
   affect_to_char( victim, &af );

   af.type      = sn;
   af.duration  = 10;
   af.location  = APPLY_CON;
   af.modifier  = -1;
   af.bitvector = 0;
   affect_to_char( victim, &af );

   send_to_char( "You suddenly feel very #gsick#n.\n\r", victim );

   if ( !IS_CLASS( victim, CLASS_DUERGAR ) )
      {
         if ( IS_NPC( ch ) )
            hurt_person( ch, victim, 500000, TRUE );
            else hurt_person( ch, victim, 300000, TRUE );
      }
      else victim->hit = UMIN( victim->max_hit, victim->hit + ( 300000 * UMAX( 1, victim->pkill / 10 ) ) );
}

void spell_idust( int sn, int level, CHAR_DATA *ch, void *vo )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

   if ( is_affected( victim, skill_lookup("ice dust") ) )
      return;

   act( "You go on your knees as you offer a silent prayer to Lolth.", ch, NULL, NULL, TO_CHAR );
   act( "$n goes on $s knees as $e offers a silent prayer to Lolth.", ch, NULL, NULL, TO_ROOM );
   act( "You stand upright with a soft smile on your face as a soft light sparkles.", victim, NULL, NULL, TO_CHAR );
   act( "$n stands upright with a soft smile on $s face as a soft light sparkles.", victim, NULL, NULL, TO_ROOM );
   act( "The light bursts into #Lice #wshards#n as they #Pswirl#n around your body beautifully.", victim, NULL, NULL, TO_CHAR );
   act( "The light bursts into #Lice #wshards#n as they #Pswirl#n around $n's body beautifully.", victim, NULL, NULL, TO_ROOM );
   act( "A soft humming fills the air as you surprising feel warm.", victim, NULL, NULL, TO_CHAR );
   act( "A soft humming fills the air as $n surprising feels warm.", victim, NULL, NULL, TO_ROOM );

   af.type      = sn;
   af.duration  = 500;
   af.location  = APPLY_INT;
   af.modifier  = 2;
   af.bitvector = 0;
   affect_to_char( victim, &af );
}

void spell_damnation( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( victim != ch )
      {
         act("You lightly touch $N's forehead muttering '#yLi Xium Sa...#n'",ch,NULL,victim,TO_CHAR);
         act("$n lightly touches your forehead muttering '#yLi Xium Sa...#n'",ch,NULL,victim,TO_VICT);
         act("$n lightly touches $N's forehead muttering '#yLi Xium Sa...#n'",ch,NULL,victim,TO_NOTVICT);
      }
      else {
              act("You raise your hand in the air muttering '#yLi Xium Sa...#n'",ch,NULL,NULL,TO_CHAR);
              act("$n raises $s hand in the air muttering '#yLi Xium Sa...#n'",ch,NULL,NULL,TO_ROOM);
           }

   act("You are suddenly surrounded by a white outline.",victim,NULL,NULL,TO_CHAR);
   act("$n is suddenly surrounded by a white outline.",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration = 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_false_pride( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("Your eyes blazes with demonic energy as you focus on $N intently.",ch,NULL,victim,TO_CHAR);
   act("$n's eyes blazes with demonic energy as $e focuses on you intently.",ch,NULL,victim,TO_VICT);
   act("$n's eyes blazes with demonic energy as $e focuses on $N intently.",ch,NULL,victim,TO_NOTVICT);
   act("You sneer '#yFe Ser Pium#n' as you point a glowing finger at $N!",ch,NULL,victim,TO_CHAR);
   act("$n sneers '#yFe Ser Pium#n' as $e points a glowing finger at you!",ch,NULL,victim,TO_VICT);
   act("$n sneers '#yFe Ser Pium#n' as $e points a glowing finger at $N!",ch,NULL,victim,TO_NOTVICT);
   act("You raise your fists in the air arrogantly as you now feel invincible!",victim,NULL,NULL,TO_CHAR);
   act("$n raises $s fists in the air arrogantly with a cocky expression on $s face!",victim,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_STR;
   af.modifier = level;
   af.duration = 200;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_DEX;
   af.modifier = -level;
   af.duration = 200;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_rose_storm( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   act("A barrage of petals surround your body as a shield of energy pulses softly.",ch,NULL,NULL,TO_CHAR);
   act("A barrage of petals surround $n's body as a shield of energy pulses softly.",ch,NULL,NULL,TO_ROOM);

   af.type = sn;
   af.location = APPLY_CON;
   af.modifier = 5;
   af.duration = level;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_fear( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   af.type = sn;
   af.location = APPLY_WIS;
   af.modifier = -5;
   af.duration = level;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_last_prayer( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   af.type = sn;
   af.location = APPLY_DEX;
   af.modifier = -4;
   af.duration = 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_INT;
   af.modifier = -4;
   af.duration = 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_STR;
   af.modifier = -4;
   af.duration = 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_CON;
   af.modifier = -4;
   af.duration = 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);

   af.type = sn;
   af.location = APPLY_WIS;
   af.modifier = -4;
   af.duration = 10;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void spell_regeneration( int sn, int level, CHAR_DATA *ch, void *vo )
{
   AFFECT_DATA af;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   af.type = sn;
   af.location = APPLY_WIS;
   af.modifier = 5;
   af.duration = level;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

