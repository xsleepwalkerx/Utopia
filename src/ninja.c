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

void do_ninjaarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_SHADOW) && !IS_CLASS(ch, CLASS_NIGHT_BLADE)
         && !IS_CLASS(ch, CLASS_ASSASSIN) && !IS_CLASS(ch, CLASS_SHADOW_WAR) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char("Please specify which piece of ninja eq you wish to make: Ring Collar Robe Cap \n\r",ch);
          send_to_char("Mask Leggings Boots Gloves Sleeves Cloak Belt Bracer Katana Dagger Shuriken. \n\r",ch);
          return;
       }

    if ( ch->gold < 1 )
       {
          send_to_char("You don't have the #w1 #ygold piece#n needed.\n\r",ch);
          return;
       }

    if (!str_cmp(arg,"dagger" )) vnum = 128;
    else if (!str_cmp(arg,"katana"   )) vnum = 129;
    else if (!str_cmp(arg,"bow"   )) vnum = 384;
    else if (!str_cmp(arg,"shuriken"   )) vnum = 385;
    else if (!str_cmp(arg,"ring"   )) vnum = 130;
    else if (!str_cmp(arg,"collar"   )) vnum = 131;
    else if (!str_cmp(arg,"robe"   )) vnum = 133;
    else if (!str_cmp(arg,"cap"   )) vnum = 134;
    else if (!str_cmp(arg,"leggings"   )) vnum = 135;
    else if (!str_cmp(arg,"boots"   )) vnum = 136;
    else if (!str_cmp(arg,"gloves"   )) vnum = 139;
    else if (!str_cmp(arg,"sleeves"   )) vnum = 137;
    else if (!str_cmp(arg,"cloak"   )) vnum = 138;
    else if (!str_cmp(arg,"belt"   )) vnum = 140;
    else if (!str_cmp(arg,"mask"   )) vnum = 141;
    else if (!str_cmp(arg,"bracer"   )) vnum = 132;
    else {
            do_ninjaarmor(ch,"");
            return;
         }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL )
       {
          send_to_char("Missing object, please inform a God .\n\r",ch);
          return;
       }

    ch->gold -= 1;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("You make $p from the shadows.",ch,obj,NULL,TO_CHAR);
    act("$n forms $p from the shadows.",ch,obj,NULL,TO_ROOM);
}

void do_mitsukeru( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char arg [MAX_INPUT_LENGTH];
    bool safe = FALSE;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_NIGHT_BLADE) && !IS_CLASS(ch, CLASS_SHADOW) )
       {
	    send_to_char("Huh?\n\r", ch );
	    return;
       }

    if ( IS_CLASS(ch, CLASS_SHADOW) && ch->gifts[AZI] < 5 )
       {
          send_to_char("You have to improve your Li fighting style to the 5th Level.\n\r",ch);
	    return;
       }

    if ( arg[0] == '\0' )
       {
	    send_to_char( "Scry on whom?\n\r", ch );
          return;
       }

    if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_SAFE) )
       safe = TRUE;

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE) )
       {
          REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
          do_look(ch,"auto");
          SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
       }
       else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE) )
               {
                  SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
                  do_look(ch,"auto");
                  REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
               }
               else do_look(ch,"auto");

    char_from_room(ch);
    char_to_room(ch,chroom);

    if ( safe )
       SET_BIT(ch->more, MORE_SAFE);
}
