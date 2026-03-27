
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

char *const pos_table[]=
{
"dead", "mortal", "incap", "stun", "sleep", "rest", "sit", "fight", "stand",
NULL
};

char *const eq_table[]=
{
"light", "finger_l", "finger_r", "neck_1", "neck_2", "torso", "head", "legs",
"feet", "hands", "arms", "shield", "body", "waist", "wrist_l", "wrist_r",
"wield", "hold", "float", NULL
};

void do_beep ( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
	return;

    argument = one_argument( argument, arg );

    if  ( arg[0] == '\0' )
    {
	send_to_char( "Beep who?\n\r", ch );
	return;
    }

    if ( !( victim = get_char_world( ch, arg ) ) )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }


    if ( IS_NPC(victim))
    {
	send_to_char( "They are not beepable.\n\r", ch );
	return;
    }

    sprintf( buf, "\aYou beep %s.\n\r", victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "\a%s has beeped you.\n\r", ch->name );
    send_to_char( buf, victim );
    return;
}

void do_sever( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       send_to_char("Who are you going to rip in half?\n\r",ch);
       return; 
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL ) 
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if(victim == ch)
    {
	send_to_char("You reach your neck fine, but cant' get your ankles.\n\r",ch);
	return;
    }

    if(IS_SET(victim->affected_by2,AFF_SEVERED))
    {
	send_to_char("They have no legs to rip off.\n\r",ch);
	return;
    }

    SET_BIT(victim->affected_by2,AFF_SEVERED);
    if(!IS_NPC(victim))
    act("$n picks you up and rips you in half! Oh no!",ch,NULL,victim,TO_VICT);
    act("$n picks up $N and rips $S legs off!",ch,NULL,victim,TO_NOTVICT);
    send_to_char("You rip them in half!\n\r",ch);

    obj = create_object( get_obj_index(OBJ_VNUM_LEGS ), 0 );

   
if(IS_NPC(victim))
    sprintf(buf,"A pair of %s's legs are here, twitching.",victim->short_descr);
else
    sprintf(buf,"A pair of %s's legs are here, twitching.",victim->name);	
    free_string(obj->description);
    obj->description = str_dup( buf );


if(IS_NPC(victim))
    sprintf(buf,"A pair of %s's legs",victim->short_descr);
else
    sprintf(buf,"A pair of %s's legs",victim->name);    
    free_string(obj->short_descr);
    obj->short_descr = str_dup( buf );

    obj_to_char(obj,ch);

}

