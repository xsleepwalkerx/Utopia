#define crypt(s1, s2)	(s1)

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
#include<types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


char *	const	where_name	[] =
{
    "#R[#CLight         #R] ",
    "#R[#COn Finger     #R] ",
    "#R[#COn Finger     #R] ",
    "#R[#CAround Neck   #R] ",
    "#R[#CAround Neck   #R] ",
    "#R[#COn Body       #R] ",
    "#R[#COn Head       #R] ",
    "#R[#COn Legs       #R] ",
    "#R[#COn Feet       #R] ",
    "#R[#COn Hands      #R] ",
    "#R[#COn Arms       #R] ",
    "#R[#COff Hand      #R] ",
    "#R[#CAround Body   #R] ",
    "#R[#CAround Waist  #R] ",
    "#R[#CAround Wrist  #R] ",
    "#R[#CAround Wrist  #R] ",
    "#R[#CRight Hand    #R] ",
    "#R[#CLeft Hand     #R] ",
    "#R[#CThird Hand    #R] ",
    "#R[#CFourth Hand   #R] ",
    "#R[#COn Face       #R] ",
    "#R[#CLeft Scabbard #R] ",
    "#R[#CRight Scabbard#R] ",
    "#R[#COn Back       #R] ",
};

const char * exitname [6] =
{
	"#Cn#0orth#n",
	"#Ce#0ast#n",
	"#Cs#0outh#n",
	"#Cw#0est#n",
	"#Cu#0p#n",
	"#Cd#0own#n"
};

const char * exitname2 [6] =
{
	"to the north",
	"to the east",
	"to the south",
	"to the west",
	"above you",
	"below you"
};


/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );

void	evil_eye		args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	check_left_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_left_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void	obj_score		args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';

    if ( obj->item_type == ITEM_HUNT )   strcat( buf, "#P(#yScAvEnGeR#P)#n " );

    if ( IS_SET(obj->quest, QUEST_ARTIFACT)) strcat(buf, "#7(Artifact)#n ");
    else if ( IS_SET(obj->quest, QUEST_RELIC)) strcat(buf,"#3(#7Relic#3)#n " );

    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "#6(Invis)#n " );

    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#4(Blue Aura)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#1(Red Aura)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#3(Yellow Aura)#n "  );
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "#4(Magical)#n " );
    if ( IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	!IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf,
"#0(Shadowplane)#n " );
    if (!IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	 IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "#7(Normal plane)#n " );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
	if ( obj->condition < 100) strcat(buf, " #1(Damaged)#n");
    }
    else
    {
	if ( obj->description != NULL )
	    strcat( buf, obj->description );
    }

    return buf;
}

/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;

      if ( obj->pIndexData->vnum == 127 )
         {
            if ( list == ch->in_room->contents )
               continue;
         }

	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !str_cmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "     ", ch );
	    }
	}
	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "\n\r", ch );
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf6[MAX_STRING_LENGTH];
    char buf7[MAX_STRING_LENGTH];
    char mount2[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    buf[0] = '\0';
    buf2[0] = '\0';
    buf3[0] = '\0';
    buf6[0] = '\0';

    if ( ch->seeself > 0 )
       victim = ch;

    if ( IS_CLASS(victim, CLASS_WISP) && IS_SET(victim->more, MORE_LIGHT) )
	 return;

    if ( IS_SET(victim->more3, MORE3_MIRROR) )
	 return;

    if ( IS_CLASS(victim, CLASS_GUYVER) && IS_SET(victim->special, SPC_GUY_WAR) )
	 return;

    if (!IS_NPC(victim) && victim->pcdata->chobj != NULL )
	return;

    if ((mount = victim->mount) != NULL && IS_SET(victim->mounted, IS_MOUNT))
	return;

    if ( !IS_NPC(victim) && IS_SET(victim->flag2, VAMP_OBJMASK) )
    {
	sprintf(buf, "     %s\n\r",victim->objdesc);
	stc(buf,ch);
	return;
    }

    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
	strcat( buf, "     " );
    else {
	if (!IS_NPC(victim) && victim->desc==NULL && armageddon ) strcat( buf,"#r(#eKill#P-#cMe#R)#n ");
         else if (!IS_NPC(victim) && victim->desc==NULL ) strcat( buf,"#r(Link-Dead)#n ");

      if ( !IS_NPC(victim) && victim->pcdata->team > 0 )
         {
             switch( victim->pcdata->team )
             {
                case 1: strcat( buf, "(#rRED team#n) "   );
                        break;
                case 2: strcat( buf, "(#bBLUE team#n) "   );
                        break;
                case 3: strcat( buf, "(#yYELLOW team#n) "   );
                        break;
                case 4: strcat( buf, "(#eGREY team#n) "   );
                        break;
                case 5: strcat( buf, "(#pPURPLE team#n) "   );
                        break;
                case 6: strcat( buf, "(#wWHITE team#n) "   );
                        break;
                case 7: strcat( buf, "(#gGREEN team#n) "   );
                        break;
                case 8: strcat( buf, "(#CCYAN team#n) "   );
                        break;
                case 9: strcat( buf, "(#rR#yA#wI#eN#bB#pO#CW #yteam#n) "   );
                        break;
                default: break;
             }
         }

    	if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "#L(Invis)#n "   );
    	if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "#0(Hide)#n "    );
    	if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "#l(Translucent)#n ");
    	if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "#P(Pink Aura)#n "  );
    	if ( IS_EVIL(victim)
    	&&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "#R(Red Aura)#n "   );
      if ( IS_CLASS(victim, CLASS_ASSASSIN) && IS_SET( victim->special, SPC_KNIGHT ) && victim->ctimer[0] > 0 )
         strcat( buf, "#e(Tactical Mode)#n " );
      if ( ch->anger_level > 300 )
         strcat( buf, "#R(Violent Aura)#n " );
    }

    if ( ( IS_AFFECTED(ch, AFF_SHADOWPLANE) || IS_AFFECTED(ch, AFF_ETHEREAL) )
         && !IS_AFFECTED(victim, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_ETHEREAL) )
       strcat( buf, "#0(#CNormal plane#0)#n "     );
       else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE) )
               strcat( buf, "#C(#0Shadowplane#C)#n "     );

    if ( !IS_AFFECTED(ch, AFF_ETHEREAL) && IS_AFFECTED(victim, AFF_ETHEREAL) )
       strcat( buf, "#C(#yEthereal#C) ");

    if ( !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_AUSPEX) && 
	 !IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) &&
	  IS_VAMPAFF(victim, VAM_DISGUISED) )
    {
	strcat( buf, "(");
	strcat( buf, victim->name);
	strcat( buf, ") ");
    }
if (victim->embracing!= NULL)
{
sprintf(buf7,"\n\r...%s is holding %s in a stern embrace!",victim->name,victim->embracing->name);
}
else if (victim->embraced!=NULL)
{
sprintf(buf7,"\n\r...%s is being embraced by %s!",victim->name,victim->embraced->name);
}
    if ( IS_EXTRA(victim, EXTRA_ROT))
     {
        if (IS_NPC(victim)) 
          sprintf(buf6,"\n\r...is coated with a layer of #yrotten skin#n!");
        else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
           sprintf(buf6,"\n\r...is coated with a layer of #yrotten skin#n!");
        else 
           sprintf(buf6,"\n\r...is coated with a layer of #yrotten skin#n!");
       }


    if ( IS_AFFECTED(victim, AFF_FLAMING) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf2, "\n\r...is engulfed in #rblazing flames#n!" );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf2, "\n\r...is engulfed in #rblazing flames#n!" );
	else
	    sprintf( buf2, "\n\r...is engulfed in #rblazing flames#n!" );
    }

    if ( !IS_NPC(victim) && IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
    {
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...is gagged and blindfolded!" );
	else if (IS_EXTRA(victim,GAGGED))
	    sprintf( buf3, "...is gagged!" );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...is blindfolded!" );
    }
    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
    {
	strcat( buf, victim->morph );
	strcat( buf, " is lying here." );
    	strcat( buf, buf2 );
    	strcat( buf, buf3 );
    	strcat( buf, buf6 );
        strcat(buf,buf7);   
     strcat( buf, "\n\r" );
    	buf[5] = UPPER(buf[5]);
    	send_to_char( buf, ch );
	return;
    }
    
    if ( IS_EXTRA(victim, TIED_UP) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf3, "\n\r...is tied up" );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf3, "\n\r...is tied up" );
	else
	    sprintf( buf3, "\n\r...is tied up" );
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, ", gagged and blindfolded!" );
	else if (IS_EXTRA(victim,GAGGED))
	    strcat( buf3, " and gagged!" );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, " and blindfolded!" );
	else
	    strcat( buf3, "!" );
    }

    if ( IS_AFFECTED(victim, AFF_WEBBED) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf4, "\n\r...is coated in a #wsticky web#n." );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf4, "\n\r...is coated in a #wsticky web#n." );
	else
	    sprintf( buf4, "\n\r...is coated in a #wsticky web#n." );
	strcat( buf3, buf4 );
    }

    if ( IS_SET(victim->more, MORE_NET) )
    {
	if (IS_NPC(victim))
	    sprintf( buf4, "\n\r...is trapped in a #cspiritual net#n." );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf4, "\n\r...is coated in a #cspiritual net#n." );
	else
	    sprintf( buf4, "\n\r...is coated in a #cspiritual net#n." );
	strcat( buf3, buf4 );
    }

    if ( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH) )
	strcat( buf, victim->morph );
    else if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' && (mount = victim->mount) == NULL )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	if ( IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF) )
	{
          if ( IS_CLASS(victim, CLASS_SHADOW_WAR) )
             act( "...is surrounded by #eswirling#n shadows.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_TIGER ) )
             act( "...is swaying back and forth in the #wtiger#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_DRAGON ) )
             act( "...is scowling in the #Rdragon#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_SNAKE ) )
             act( "...is withering in the #gsnake#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_HYENA ) )
             act( "...is grinning to themselves in the #chyena#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_CRANE ) )
             act( "...is leaning back in the #Ccrane#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_FALCON ) )
             act( "...is tilting in the #Lfalcon#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_MANTIS ) )
             act( "...is crouching in the #ymantis#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET( victim->more3, MORE3_WOLF ) )
             act( "...is swaying back and forth in the #owolf#n stance.", ch,NULL,victim,TO_CHAR );

          if ( IS_SET(victim->more3, MORE3_SOUL) )
             {
                act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
                act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
                act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
                act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
                act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
             }

          if ( IS_CLASS(victim, CLASS_SAIYAN) && victim->saiyanab[BODY] == 80 )
             act( "...$s entire body is covered by a #wmass#n of #Rred #cfur#n.", victim,NULL,ch,TO_VICT );

          if ( IS_CLASS(victim, CLASS_SAIYAN) && ( victim->saiyanab[BODY] == 50 || victim->saiyanab[BODY] == 60 ) )
             act( "...$e is surrounded by a crackling field of #Pdazzling #ylightning#n.", victim,NULL,ch,TO_VICT );

          if ( victim->inner_aura != -1 )
             act( "...is engulfed in a #wblazing#n aura.", ch,NULL,victim,TO_CHAR );

          if ( victim->outer_aura != -1 || victim->group_aura > 0 )
             act( "...is surrounded by a #yhazing#n aura.", ch,NULL,victim,TO_CHAR );

          if ( IS_CLASS(victim, CLASS_ANGEL) && victim->angelab[BODY] > 3 )
	       act( "...the room is fill by hundrends of flowing tendrils from $N's body.", ch,NULL,victim,TO_CHAR );
          if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL) )
	       act( "...the room is fill with the grace and beauty of $N's wings.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
		act( "...is surrounded by a crackling shield of #ylightning#n.", ch,NULL,NULL,TO_CHAR );
		if ( IS_ITEMAFF(victim, ITEMA_ELECSHIELD) ) 
		act( "...is surrounded by a crackling shield of #ylightning#n.", ch,NULL,NULL,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
		act( "...is surrounded by a burning shield of #Rf#yi#Rr#ye#n.", ch,NULL,NULL,TO_CHAR );
	    if ( !IS_CLASS( victim, CLASS_SHADOW ) && IS_ITEMAFF(victim, ITEMA_FIREWALL) ) 
		act( "...is surrounded by a huge wall of #Rf#yi#Rr#ye#n.", ch,NULL,NULL,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
		act( "...is surrounded by a shimmering shield of #Cice#n.", ch,NULL,NULL,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
		act( "...is surrounded by a bubbling shield of #La#Rc#Li#Rd#n.", ch,NULL,NULL,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
		act( "...is surrounded by a swirling shield of #0c#Rh#0a#Ro#0s#n.", ch,NULL,NULL,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) ) 
		act( "...is surrounded by a flickering shield of #0darkness#n.", ch,NULL,NULL,TO_CHAR );
          if ( IS_ITEMAFF(victim, ITEMA_REAPER) ) 
	      act( "...is contorted between #wgood#n and #eevil#n.", ch,NULL,NULL,TO_CHAR );
	}
	return;
    }
    else
    	strcat( buf, PERS( victim, ch ) );

    if ((mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	if (IS_NPC(mount))
	    sprintf( mount2, " is here riding %s", mount->short_descr );
	else
	    sprintf( mount2, " is here riding %s", mount->name );
	strcat( buf, mount2 );
	if (victim->position == POS_FIGHTING)
	{
	    strcat( buf, ", fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	}
	else strcat( buf, "." );
    }
    else if ( victim->position == POS_STANDING && IS_AFFECTED(victim, AFF_FLYING) )
	strcat( buf, " is hovering here" );
    else if ( victim->position == POS_STANDING && (!IS_NPC(victim) && (IS_VAMPAFF(victim, VAM_FLYING))) )
	strcat( buf, " is hovering here" );
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here" );
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
IS_CLASS(victim, CLASS_DRAGON) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here" );
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
IS_CLASS(victim, CLASS_ANGEL) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here" );
    else if ( IS_CLASS(victim, CLASS_DRAGOON) )
	strcat(buf, " is hovering here" );
    else
    {
    	switch ( victim->position )
    	{
    	case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
        case POS_MORTAL:   strcat( buf, " is #Rmortally wounded#n." );   break; 
    	case POS_INCAP:    strcat( buf, " is #rincapacitated#n." );      break;
    	case POS_STUNNED:  strcat( buf, " is lying here #Cstunned#n." ); break;
    	case POS_SLEEPING: strcat( buf, " is sleeping here." );      break;
    	case POS_RESTING:  strcat( buf, " is resting here." );       break;
    	case POS_MEDITATING: strcat( buf, " is meditating here." );  break;
    	case POS_SITTING:  strcat( buf, " is sitting here." );       break;
    	case POS_STANDING: strcat( buf, " is here." ); break;
    	case POS_FIGHTING:
	    strcat( buf, " is here, fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	    break;
	}
    }

    strcat( buf, buf2 );
    strcat( buf, buf3 );
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF) ) return;

    if ( IS_CLASS(victim, CLASS_SHADOW_WAR) )
       act( "...is surrounded by #eswirling#n shadows.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_TIGER ) )
       act( "...is swaying back and forth in the #wtiger#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_DRAGON ) )
       act( "...is scowling in the #Rdragon#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_SNAKE ) )
       act( "...is withering in the #gsnake#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_HYENA ) )
       act( "...is grinning to themselves in the #chyena#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_CRANE ) )
       act( "...is leaning back in the #Ccrane#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_FALCON ) )
       act( "...is tilting in the #Lfalcon#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_MANTIS ) )
       act( "...is crouching in the #ymantis#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET( victim->more3, MORE3_WOLF ) )
       act( "...is swaying back and forth in the #owolf#n stance.", ch,NULL,victim,TO_CHAR );

    if ( IS_SET(victim->more3, MORE3_SOUL) )
       {
           act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
           act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
           act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
           act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
           act( "...An blurry image of $N is here fading in and out of existence.", ch,NULL,victim,TO_CHAR );
       }

    if ( IS_CLASS(victim, CLASS_SAIYAN) && victim->saiyanab[BODY] == 80 )
       act( "...$s entire body is covered by a #wmass#n of #Rred #cfur#n.", victim,NULL,ch,TO_VICT );

    if ( IS_CLASS(victim, CLASS_SAIYAN) && ( victim->saiyanab[BODY] == 50 || victim->saiyanab[BODY] == 60 ) )
       act( "...$e is surrounded by a crackling field of #Pdazzling #ylightning#n.", victim,NULL,ch,TO_VICT );

    if ( victim->inner_aura != -1 )
       act( "...is engulfed in a #wblazing#n aura.", ch,NULL,victim,TO_CHAR );

    if ( victim->outer_aura != -1 || victim->group_aura > 0 )
       act( "...is surrounded by a #yhazing#n aura.", ch,NULL,victim,TO_CHAR );

    if ( IS_CLASS(victim, CLASS_ANGEL) && victim->angelab[BODY] > 3 )
       act( "...the room is fill by hundrends of flowing tendrils from $N's body.", ch,NULL,victim,TO_CHAR );

    if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL) )
	 act( "...the room is fill with the grace and beauty of $N's wings.", ch,NULL,victim,TO_CHAR );

    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
	act( "...is surrounded by a crackling shield of lightning.", ch,NULL,NULL,TO_CHAR );
	if ( IS_ITEMAFF(victim, ITEMA_ELECSHIELD) ) 
    act( "...is surrounded by a crackling shield of #ylightning#n.", ch,NULL,NULL,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
	act( "...is surrounded by a burning shield of fire.", ch,NULL,NULL,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
	act( "...is surrounded by a shimmering shield of ice.", ch,NULL,NULL,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
	act( "...is surrounded by a bubbling shield of acid.", ch,NULL,NULL,TO_CHAR );
    if ( !IS_CLASS( victim, CLASS_SHADOW ) && IS_ITEMAFF(victim, ITEMA_FIREWALL) ) 
	act( "...is surrounded by a huge wall of #Rf#yi#Rr#ye#n.", ch,NULL,NULL,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
	act( "...is surrounded by a swirling shield of chaos.", ch,NULL,NULL,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) ) 
	act( "...is surrounded by a flickering shield of darkness.", ch,NULL,NULL,TO_CHAR );
    return;
}



void evil_eye( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf [MAX_STRING_LENGTH];
    int tabletype;

    if (IS_NPC(victim)) return;
    if (victim == ch) return;
    if (ch->level < 3 || victim->level < 3) return;
    if (victim->power[DISC_DAEM_DISC] < 2) return;
    if (victim->power[DISC_DAEM_DISC] < dice(1, 10)) return;
    if (!str_suffix(victim->poweraction,"gi") ||
	!str_suffix(victim->poweraction,"qui"))
	return;
    if (is_safe(ch, victim) ) return;
    if (victim->powertype != NULL && strlen(victim->powertype) > 1)
    {
	sprintf(buf,"\n\r%s\n\r",victim->powertype);
	send_to_char(buf,ch);
    }
    if (IS_SET(victim->spectype,EYE_SELFACTION) && victim->poweraction != NULL)
	interpret(victim,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_ACTION) && victim->poweraction != NULL)
	interpret(ch,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_SPELL) && victim->specpower > 0)
    {
	tabletype = skill_table[victim->specpower].target;
	(*skill_table[victim->specpower].spell_fun) (victim->specpower,victim->spl[tabletype],victim,ch);
    }
    return;
}


void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear, tWear = MAX_WEAR;
    int percent, hit, mhit;
    bool found;

    if ( !IS_IMMORTAL( ch ) && !IS_NPC( victim ) && IS_CLASS( victim, CLASS_DARK_ELF )
         && victim->pcdata->powers[DROW_CLASS] == DROW_MILITIA
         && !IS_CLASS( ch, CLASS_DROW ) && !IS_CLASS( ch, CLASS_DARK_ELF ) )
       {
          act("$N stares at you intently and screams in terror.",victim,NULL,ch,TO_CHAR);
          act("$N stares at $n intently and screams in terror.",victim,NULL,ch,TO_NOTVICT);
          act("You stare at $n intently and scream in terror.",victim,NULL,ch,TO_VICT);
          act("Lloth appears before $N and blasts $M away from you ViCiOuSlY!",victim,NULL,ch,TO_CHAR);
          act("Lloth appears before $N and blasts $M away from $n ViCiOuSlY!",victim,NULL,ch,TO_NOTVICT);
          act("Lloth appears before you and blasts you away from $n ViCiOuSlY!",victim,NULL,ch,TO_VICT);
          ch->hit = UMAX( 1, ch->hit - 5000000 );
          ch->agg = 100;
          return;
       }

    if ( can_see( victim, ch ) )
    {
	act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
    }

    if ( IS_CLASS(victim, CLASS_WISP) && IS_SET(victim->more, MORE_LIGHT) )
       {
          act("You scream as the blazing light from $N's body blinds you.",ch,NULL,victim,TO_CHAR);
          act("$n screams as the blazing light from $N's body blinds $m.",ch,NULL,victim,TO_NOTVICT);
          act("$n screams as the blazing light from your body blinds $m.",ch,NULL,victim,TO_VICT);
          SET_BIT(ch->more, MORE_BLIND);
	    return;
       }

    if (!IS_NPC(ch) && IS_HEAD(victim,LOST_HEAD))
    {
	act( "$N is lying here.", ch, NULL, victim, TO_CHAR );
	return;
    }
    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    hit = victim->hit / 100;
    mhit = victim->max_hit / 100;

    if ( mhit > 0 )
       percent = ( 100 * hit ) / mhit;
       else percent = -1;

    strcpy( buf, PERS(victim, ch) );

         if ( percent >= 100 ) strcat( buf, " is in perfect health.\n\r"  );
    else if ( percent >=  90 ) strcat( buf, " is slightly scratched.\n\r" );
    else if ( percent >=  80 ) strcat( buf, " has a few bruises.\n\r"     );
    else if ( percent >=  70 ) strcat( buf, " has some cuts.\n\r"         );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has many nasty wounds.\n\r" );
    else if ( percent >=  40 ) strcat( buf, " is bleeding freely.\n\r"    );
    else if ( percent >=  30 ) strcat( buf, " is covered in blood.\n\r"   );
    else if ( percent >=  20 ) strcat( buf, " is leaking guts.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is almost dead.\n\r"        );
    else                       strcat( buf, " is DYING.\n\r"              );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
 
    if (!IS_NPC( victim ))
    {
    	if (IS_AFFECTED(victim, AFF_INFRARED) || IS_VAMPAFF(victim, VAM_NIGHTSIGHT)) act("$N's eyes are glowing bright red.",ch,NULL,victim,TO_CHAR);
    	if (IS_AFFECTED(victim, AFF_FLYING)) act("$N is hovering in the air.",ch,NULL,victim,TO_CHAR);
		if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION))
			act("$N is hovering in the air.", ch,NULL,victim,TO_CHAR);
	    if (IS_VAMPAFF(victim, VAM_FANGS)) act("$N has a pair of long, pointed fangs.",ch,NULL,victim,TO_CHAR);
	    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has razor sharp claws protruding from under $S finger nails.",ch,NULL,victim,TO_CHAR);
		else if (IS_CLASS(victim, CLASS_NINJA) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has IronClaws protruding from $S wrists.",ch,NULL,victim,TO_CHAR);
	    else if (!IS_NPC(victim) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has razor sharp talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
	    if (!IS_NPC(victim))
	    {
			if (!IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->newbits, NEW_SKIN)) act("$N has scales all over $S body.",ch,NULL,victim,TO_CHAR);
			if (IS_VAMPAFF(victim, VAM_HORNS)) act("$N has a pair of pointed horns extending from $S head.",ch,NULL,victim,TO_CHAR);
			if (IS_VAMPAFF(victim, VAM_WINGS))
			{
	    		if (IS_DEMAFF(victim, DEM_UNFOLDED))
					act("$N has a pair of batlike wings spread out from behind $S back.",ch,NULL,victim,TO_CHAR);
	    		else
			 		act("$N has a pair of batlike wings folded behind $S back.",ch,NULL,victim,TO_CHAR);
			}
			if (IS_VAMPAFF(victim, VAM_HEAD))
			 {
			   act("$N's head resembles that of a fierce lion.",ch,NULL,victim,TO_CHAR);
			 }
		   if (IS_VAMPAFF(victim, VAM_TAIL))
				act("$N has a sharp tail extending from $S spine.",ch,NULL,victim,TO_CHAR); 
		   if (IS_VAMPAFF(victim, VAM_EXOSKELETON))
				act("$N's skin is covered by a hard exoskeleton.",ch,NULL,victim,TO_CHAR);
 
	  	}
	}
	found = FALSE;

    for ( iWear = 0; iWear < tWear; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj != obj)
	    {
	    	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    	send_to_char( "\n\r", ch );
	    }
	    else
		send_to_char( "you\n\r", ch);
	}
    }

    send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
    show_list_to_char( victim->carrying, ch, TRUE, TRUE );

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( IS_IMMORTAL(ch) || IS_IMMORTAL(rch) )
	{
	    if ( !IS_NPC(rch)
	    &&   IS_SET(rch->act, PLR_WIZINVIS)
	    &&   get_trust( ch ) < get_trust( rch ) )
		continue;
	}
	else
	{
	    if ( !IS_NPC(rch)
	    && ( IS_SET(rch->act, PLR_WIZINVIS)
	    ||   IS_ITEMAFF(rch, ITEMA_VANISH) )
	    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
	    &&   !IS_ITEMAFF(ch, ITEMA_VISION) )
		continue;
	}

	if ( !IS_NPC(rch) && IS_HEAD(rch,LOST_HEAD))
	    continue;

	if ( !IS_NPC(rch) && IS_EXTRA(rch,EXTRA_OSWITCH))
	    continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room )
	&&      ( IS_AFFECTED(rch, AFF_INFRARED)||(!IS_NPC(rch)&&IS_VAMPAFF(rch, VAM_NIGHTSIGHT))))
	{
	    send_to_char( "You see glowing #Rred#n eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{
    if ( IS_EXTRA(ch, BLINDFOLDED) )
    {
        send_to_char( "You can't see a thing through the blindfold!\n\r", ch );
        return FALSE;
    }     

    if ( IS_SET(ch->garou1, GAROU_BLISSFUL) )
    {
	 send_to_char( "You can't see a thing from this realm!\n\r", ch );
       return FALSE;
    }

    if ( armageddon == TRUE )
       return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_EYES) )
       return TRUE;

    if ( IS_SET(ch->more, MORE_BLIND) && !IS_IMMORTAL(ch) && !IS_SET( ch->cmbt[5], SKL2_SSIGHT ) )
       {
          send_to_char( "You can't see a thing!\n\r", ch );
          return FALSE;
       }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
       return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
       return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
    {
	send_to_char( "You have no eyes!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    return TRUE;
}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char *pdesc;
    int door;
    bool found;
    bool twastes = FALSE;

    if ( ch->desc == NULL && (wizard = ch->wizard) == NULL) return;

    if (ch->in_room == NULL) return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( IS_SET(ch->flag2,AFF_TOTALBLIND))
    {
    send_to_char("You can't see anything because you're blinded!\n\r",ch);
    return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( IS_SET(ch->in_room->area->bits,WASTE) )
       twastes = TRUE;

    if ( IS_SET(ch->in_room->area->bits,ROOM_TOTAL_DARKNESS) && !IS_ITEMAFF(ch, ITEMA_EYES)
         && !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS( ch, CLASS_SHADOW )
         && !( !IS_NPC(ch) && ch->pcdata->team > 0 )
         && !armageddon && !IS_SET( ch->cmbt[7], SKL_EYES ) )
       {
  	    send_to_char( "It is pitch black ... \n\r", ch );
	    return;
       }

    if ( !IS_NPC(ch) && !IS_ITEMAFF(ch, ITEMA_EYES) && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) &&
	   !IS_IMMORTAL(ch) && !IS_CLASS(ch, CLASS_DROW) && !( !IS_NPC(ch) && ch->pcdata->team > 0 ) 
         && !IS_ITEMAFF( ch, ITEMA_DARKNESS ) && !armageddon && !IS_SET( ch->more2, MORE2_LEOPARD )
         && !IS_SET( ch->cmbt[7], SKL_EYES ) && !IS_CLASS( ch, CLASS_SHADOW ) )
       {
	    send_to_char( "It is pitch black ... \n\r", ch );
          return;
       }

    if ( IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) && !can_do_skill( ch, 44 ) ) 
       {
	    send_to_char( "It is pitch black ... \n\r", ch );
          return;
       }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	    act( "$p",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
      else if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
	{
	    sprintf(buf, "%s\n\r", ch->in_room->name);
	    send_to_char( buf, ch );
	}
      else if ( weather_info.sky == SKY_ECLIPSE )
             stc( "#ePitch Darkness#n\n\r", ch );
  	else if ( twastes )
             stc( "#yTrackless Wastes#n\n\r", ch );
    	else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "#eThe shadow plane#n\n\r", ch );
    	else if ( IS_AFFECTED(ch, AFF_ETHEREAL) )
	    send_to_char( "#CThe spiritual plane#n\n\r", ch );
	else
	{
	    sprintf(buf, "%s\n\r", ch->in_room->name);
	    send_to_char( buf, ch );
	}

      if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
	    do_exits( ch, "auto" );
          else if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) && !( twastes && !IS_CLASS( ch, CLASS_WEREWOLF ) )
                    && weather_info.sky != SKY_ECLIPSE )
                  do_exits( ch, "auto" );

	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	{
	    act( "You are inside $p.",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
	    show_list_to_char( ch->pcdata->chobj->in_obj->contains, ch, FALSE, FALSE );
	}
      else if ( !( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 ) )
      {
	    if ( twastes )
	       stc("The land surrounding you is devoid of any landmarks.\n\r",ch);
             else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_SHADOWPLANE) )
                     send_to_char( "You are standing in complete darkness.\n\r", ch );
             else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_ETHEREAL) )
                     send_to_char( "You are hovering admist a beautiful bluish vortex.\n\r", ch );
      }
	else if ( !IS_SET(ch->act, PLR_BRIEF) && ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) )
	{
	    send_to_char( ch->in_room->description, ch );
	    if (ch->in_room->blood == 1000)
		sprintf(buf,"You notice that the room is completely drenched in blood.\n\r");
	    else if (ch->in_room->blood > 750)
		sprintf(buf,"You notice that there is a very large amount of blood around the room.\n\r");
	    else if (ch->in_room->blood > 500)
		sprintf(buf,"You notice that there is a large quantity of blood around the room.\n\r");
	    else if (ch->in_room->blood > 250)
		sprintf(buf,"You notice a fair amount of blood on the floor.\n\r");
	    else if (ch->in_room->blood > 100)
		sprintf(buf,"You notice several blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 50)
		sprintf(buf,"You notice a few blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 25)
		sprintf(buf,"You notice a couple of blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 0)
		sprintf(buf,"You notice a few drops of blood on the floor.\n\r");
	    else sprintf(buf,"You notice nothing special in the room.\n\r");
	    ADD_COLOUR(ch, buf, L_RED);
	    if (ch->in_room->blood > 0) send_to_char(buf,ch);
	}

      if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
         send_to_char("#w..This room is #Rengulfed #yin a #Lco#glo#Lur#gful #evortex#w!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_FLAMING) )
         send_to_char("#r..This room is engulfed in #Cflames#r!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_NOSTOCK) && IS_CLASS( ch, CLASS_ASSASSIN )
           && IS_SET( ch->special, SPC_KNIGHT ) )
         send_to_char("#e..This room is surrounded by an #wanti#y-#cstock #Pshield#e!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_ICETRAP) && IS_CLASS( ch, CLASS_SNOW_ELF ) )
         send_to_char("#y..This room is covered with #Licy #Ptraps#y!#n\n\r",ch);

      if ( IS_SET( ch->in_room->room_flags, ROOM_CATACOMB ) )
         send_to_char("#e..This room is surrounded by an #oencavement#e!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_ICYSWORD) )
         send_to_char("#y..This room is impaled with #Licy #Pswords#y!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_BARRIER) )
         send_to_char("#w..This room is surrounded by an #Renergy #ybarrier#w!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_ENTOMB) )
         send_to_char("#L..This room is encased by #wice #cwalls#L!#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_CATACOMB) )
         act( "#g..This room is surrounded by #whuge #gwalls of #ygranite.#n", ch, NULL, NULL, TO_CHAR );

      if ( IS_SET(ch->in_room->room_flags, ROOM_NOMOVE) )
         stc("#wThe room is surrounding by a #csoft #yglow#w, preventing #Pactions#w.#n\n\r",ch);

      if ( IS_SET(ch->in_room->room_flags, ROOM_LUMICA) )
         stc("#wThe room is surrounding by a #csoft #yglow#w, preventing #Pmagick#w.#n\n\r",ch);

      if ( IS_SET( ch->in_room->room_flags, ROOM_WHIRLWIND ) )
         stc("#wThe room is emitting a #csoft #yhue#w, preventing #Pdamage#w.#n\n\r",ch);

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );

        for (door=0 ; door < 6 ; door++)
	{	
	    if (ch->in_room == NULL) continue;
	    if (ch->in_room->exit[door] == NULL) continue;

	    if (IS_SET(ch->in_room->exit[door]->exit_info,EX_PRISMATIC_WALL)) 	    
	    {
		sprintf(buf, "     You see a shimmering wall of many colours %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_STAKE_WALL))
	    {
		sprintf(buf, "     You see a bunch of thorny stakes %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ICE_WALL))
	    {
		sprintf(buf, "     You see a glacier of ice %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_CALTROP_WALL))
	    {
		sprintf(buf, "     You see a wall of caltrops %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_SWORD_WALL))
	    {
		sprintf(buf, "     You see a spinning wall of swords %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_MUSHROOM_WALL))
	    {
		sprintf(buf, "     You see a vibrating mound of mushrooms %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ASH_WALL))
	    {
		sprintf(buf, "    You see a deadly wall of ash %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	}
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "It doesn't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    if (obj->value[2] == 1 || obj->value[2] == 3)
	    {
		send_to_char( "It seems to be closed.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in it.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "It contains a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "It's about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "It's about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "It is almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "It's completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow it is MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

          if ( !IS_IMMORTAL(ch) && IS_SET( obj->extra_flags, ITEM_DETONATE ) )
             {
                act("As you look in $p, #rf#ee#ra#er#y grazes#n your face as it #wexplodes#n!",ch,obj,NULL,TO_CHAR);
                act("As $n looks in $p, the room #wexplodes#n with a #z#Cflash#n of #ylight#n!",ch,obj,NULL,TO_ROOM);
                act("You are flung across the room as the #Rflames#n eat through your flesh!",ch,NULL,NULL,TO_CHAR);
                act("$n is flung across the room as the #Rflames#n eat through $s flesh!",ch,NULL,NULL,TO_ROOM);
                extract_obj( obj );

                ch->hit -= 10000000;
                update_pos( ch );

                if ( ch->position == POS_DEAD )
                   {
                      act("You are DEAD!!!",ch,NULL,NULL,TO_CHAR);
                      raw_kill( ch );
                   }

                sprintf(buf,"#wdE #eBomB #z#PeXpLodEs#n #gin #y%s's #gFaCe#w!!!#n", ch->name);
                do_info(ch,buf);
                return;
             }

	    act( "$p contains:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	evil_eye(victim,ch);
	return;
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !str_cmp(arg1,vch->morph))
	    {
		show_char_to_char_1( vch, ch );
		evil_eye(vch,ch);
		return;
	    }
	    continue;
	}
    }

    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
    {
	obj = get_obj_in_obj(ch,arg1);
	if (obj != NULL)
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );
*/

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
	else
	{
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
    }
    else
    {
	if ((pexit = ch->in_room->exit[door]) == NULL) return;
	if ((pRoomIndex = pexit->to_room) == NULL) return;
	location = ch->in_room;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
	char_from_room(ch);
	char_to_room(ch,location);
    }

    return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	     if (obj->condition >= 100) 
	    sprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
	else if (obj->condition >= 75 )
	    sprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
	else if (obj->condition >= 50 )
	    sprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
	else if (obj->condition >= 25 )
	    sprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
	else
	    sprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
	send_to_char(buf,ch);
	switch ( obj->item_type )
	{
	default:
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    sprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    strcpy( buf, fAuto ? "#R[#GExits#7:#C" : "Obvious exits:\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL)
/*	&&   !IS_SET(pexit->exit_info, EX_CLOSED) ) */ // Just plain silly - Jobo
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s\n\r",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->to_room )
			?  "Too dark to tell"
			: pexit->to_room->name
		    );
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "#R]#x\n\r" );

    send_to_char( buf, ch );
    return;
}



const char * transformation_message[2][12] =
{
{
"\n\rYour body shudders and convulses as your form transforms. Arms and legs grow out of your slimy torso, your skin turns a sickly veined green.\n\r" ,
"\n\rYour demonic form is reduced to a pool of pulsating jelly, the fires of hell heat and char your semi-liquid form until you stiffen.  Limbs grow and spiny talons extend from your arms and back.\n\r" ,
"\n\rYou start the transformation.  Your gargoylish apperance is changed as you grow another two feet in height.  Scales, dripping with acid, begin to cover your skin.\n\r" ,
"\n\rThe fires of hell surround you once more.  Sharp claws start to grow from your hands, and you grow a viscious barbed tail.  Small flames play over your burnt and blackened skin.\n\r" ,
"\n\rYour next form is that of a sickly, gaunt skeleton.  Your leathery flesh held tight over a demonic frame.  A huge tail, with a poisonous barb on the end protrudes out of the base of your spine.\n\r" ,
"\n\rAs you transform, you grow another four feet in height. This time, wicked barbs cover your entire body.  You find you have the ability to wield weapons, and use armor.\n\r" ,
"\n\rWhat?  What is happening, with the transformation into an Erinyes, your form is no longer repulsive and demonic, but you are now a beautiful mortal, with long auburn hair and huge white wings growing out of your back.  You can see the world again with your mortal eyes, but the  passions of a demon will always stay.\n\r",
"\n\rYour skin melts and you transform once more, but this time you become a gruesome pug nosed, vile dwarf-like creature with large leathery wings and a mouth filled of razor sharp teeth, an Amnizu.\n\r" ,
"\n\rThe intense heat of the fires of Baator surround you. Your skin is wrenched apart and you transform into a grotesque 9-foot tall monstrosity with huge wings, a snaking, prehensile tail, and a long barbed whip.  A true demon, a Cornugon.\n\r",
"\n\rThe power of Baator now fills your very soul.  You grow again as the chill winds of Caina tear the flesh from your bones.  You look to see what you have become, you look alien, with a twelve foot long icy insect-like body, your head bulging with multi-faceted eyes and you have a long tail covered with razor-sharp spikes.\n\r",
"\n\rAaargh!  You are cast into the pit of flame. The fires, more intense than ever before sear and scar your flesh as it bubbles and boils. You have become the most terrible demon, the Pit Fiend, a huge bat winged  humanoid, twelve feet tall, with large fangs that drip with a vile green liquid and a hulking red scaly body that bursts into flame when you are angered or excited.\n\r",
"\n\rYour fiery skin blackens as you are infused with the embodiment of ultimate evil.  You are the most powerful demon, the lord of all.  You are everything, nothing can oppose you.  Twenty foot tall, your torso drips with the vile acid of the Abyss.  A ravenous hunger is gnawing at your bones.  Welcome to Immortality..."
	},
	{
"Your first transformation is quite remarkable, you grow to over seven feet tall, and your skin starts to dull and lose all signs of vitality.  Your teeth begin to extend.  The world starts to look different, you have left mortality behind.\n\r",
"You start to grow, the base of your spine slowly extends into a short stump of a tail.  The metamorphosis is tearing you apart.  Your skin begin to harden, and your hands twist into claws.\n\r",
"Once again the painful metamorphosis strikes.  Your bones rend through your scaled flesh as jagged spines shoot through your back.  You feel as if you are being wrenched apart.  You are now over ten feet tall, and your skin is covered with thick green scales.\n\r",
"You transform once again.  This time a small pair of wings burst through your back.  Your hands and feet have become viscious claws, and your nose has extended into a snout.  Long jagged teeth begin to grow in your mouth, and your tail now reaches the floor.\n\r",
"You fall to the floor in agony.  Clutching your chest as the transformation strikes.  You grow to over fifteen feet in length, towering above all the mortals that you have left behind.  Impenetrable scales now cover your body,  and smoke issues from your nostrils when you breathe.\n\r",
"You grow to over twenty feet in length, your tail and claws grow at an alarming rate.  Your wings are now so powerful that you can fly.  Nobody can oppose you now.  Suddenly it hits you, a hunger so intense that you cannot ignore it. You must feed.\n\r",
"",
"",
"",
"",
"You have reached the pinnacle of power for a dragon.  As your body undergoes the by now familiar transformation you grow to over fifty feet tall, with incredibly sharp claws and fangs, a powerful tail and a fiery breath weapon. Nobody can oppose you now.  You are the lord of the realm.\n\r",
""
	}
};


 
void do_far( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   ROOM_INDEX_DATA *chroom;
   ROOM_INDEX_DATA *victimroom;
   char arg1[MAX_STRING_LENGTH];

   argument = one_argument (argument, arg1);

    if ( IS_NPC(ch) )
	return;

	if (arg1[0] == '\0')
	{
	   send_to_char("Who do you wish to use farcommand on?\n\r",ch);
	   return;
	}

  	if (!IS_CLASS(ch, CLASS_VAMPIRE))
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PRES] < 9 )
  	{
  	send_to_char("You need level 9 Presence to use this power.\n\r", ch);
	return;
	}

      if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
     if ( !IS_NPC(victim) )
	{
	   send_to_char("Their mind is too strong!\n\r",ch);
	   return;
	}
    if ( victim->level > 250)
    {
    send_to_char("They are too big!\n\r",ch);
    return;
    } 
  
    chroom = ch->in_room;
    victimroom = victim->in_room;
    char_from_room(ch);
    char_to_room(ch,victimroom);
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("You possess their body.\n\r",ch);
    WAIT_STATE(ch, 50);
    char_from_room(ch);
    char_to_room(ch,chroom);  	
    
    return;
}

char * get_alignment( int alignment )
{
   static char buf[10];

   buf[0] = '\0';

   if ( alignment >= 1000 ) 
      strcpy( buf, "Angelic" );
      else if ( alignment > 700 ) 
              strcpy( buf, "Saintly" );
      else if ( alignment > 350 ) 
              strcpy( buf, "Good" );
      else if ( alignment > 100 ) 
              strcpy( buf, "Kind" );
      else if ( alignment > -100 ) 
              strcpy( buf, "Neutral" );
      else if ( alignment > -350 ) 
              strcpy( buf, "Mean" );
      else if ( alignment > -700 ) 
              strcpy( buf, "Evil" );
      else if ( alignment <= -1000 ) 
              strcpy( buf, "Demonic" );
              else strcpy( buf, "Satanic" );

     return buf;
}

char * get_sex( int sex )
{
   static char buf[10];

   buf[0] = '\0';

   if ( sex == 1 ) 
      strcpy( buf, "Male" );
      else if ( sex == 2 ) 
              strcpy( buf, "Female" );
              else strcpy( buf, "Neuter" );

     return buf;
}

char * get_race( int race )
{
   static char buf[23];

   buf[0] = '\0';

   if ( race == CLASS_PALADIN ) 
      strcpy( buf, "#wPaladin" );
      else if ( race == CLASS_DRAGOON ) 
              strcpy( buf, "#CHighwind" );
      else if ( race == CLASS_DRAGON_KNIGHT ) 
              strcpy( buf, "#PDragoon" );
      else if ( race == CLASS_DRAGON ) 
              strcpy( buf, "#pDragon" );
      else if ( race == CLASS_MONK ) 
              strcpy( buf, "#CMonk" );
      else if ( race == CLASS_DEMON ) 
              strcpy( buf, "#rDemon" );
      else if ( race == CLASS_WEREWOLF ) 
              strcpy( buf, "#yWerewolf" );
      else if ( race == CLASS_VAMPIRE ) 
              strcpy( buf, "#RVampire" );
      else if ( race == CLASS_NINJA ) 
              strcpy( buf, "#LNinja" );
      else if ( race == CLASS_DROW ) 
              strcpy( buf, "#eDrow" );
      else if ( race == CLASS_SHADOW ) 
              strcpy( buf, "#eAssassin" );
      else if ( race == CLASS_SHADOW_WAR ) 
              strcpy( buf, "#eShadow War" );
      else if ( race == CLASS_NIGHT_BLADE ) 
              strcpy( buf, "#LGaiden" );
      else if ( race == CLASS_WIZARD ) 
              strcpy( buf, "#cWizard" );
      else if ( race == CLASS_SAIYAN ) 
              strcpy( buf, "#RSaiya-Jin" );
      else if ( race == CLASS_SAMURAI ) 
              strcpy( buf, "#CSamurai" );
      else if ( race == CLASS_ASSASSIN ) 
              strcpy( buf, "#eClone" );
      else if ( race == CLASS_ANGEL ) 
              strcpy( buf, "#wAngel" );
      else if ( race == CLASS_UNDEAD_KNIGHT ) 
              strcpy( buf, "#eUndead Kngt" );
      else if ( race == CLASS_NECROMANCER ) 
              strcpy( buf, "#eNecromancer" );
      else if ( race == CLASS_SNOW_ELF ) 
              strcpy( buf, "#wSnow Elf" );
      else if ( race == CLASS_WISP ) 
              strcpy( buf, "#yWisp" );
      else if ( race == CLASS_PHOENIX ) 
              strcpy( buf, "#RPhoenix" );
      else if ( race == CLASS_DUERGAR ) 
              strcpy( buf, "#gDuergar" );
      else if ( race == CLASS_HOBBIT ) 
              strcpy( buf, "#wHobbit" );
      else if ( race == CLASS_DAYWALKER ) 
              strcpy( buf, "#yDaywalker" );
      else if ( race == CLASS_DARAMON ) 
              strcpy( buf, "#PDaramon" );
      else if ( race == CLASS_FIEND ) 
              strcpy( buf, "#RFiend" );
      else if ( race == CLASS_KURUDA ) 
              strcpy( buf, "#cCommander" );
      else strcpy( buf, "#oHuman" );

   return buf;
}

char * get_stars( int num, int max_num )
{
   static char buf[10];
   int digit, count;

   buf[0] = '\0';

   if ( num == 0 )
      return buf;
  
   digit = UMAX( 1, UMIN( num * 10 / max_num, 10 ) );

   for ( count = 1; count < digit + 1; count++ )
      strcat( buf, "*" );

   return buf;
}

void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) 
       return;

    stc( "#r/===========================================\\#n\n\r", ch );
    sprintf( buf,	"#r| #wName: #y%12s #r| #wSurname: #y%12s#r \\#n\n\r", ch->pcdata->switchname, ch->pcdata->lastname );
    send_to_char( buf, ch );

    stc( "#r|=========================================================\\#n\n\r", ch );

    sprintf( buf,	"#r| #wSex: #y%6s #r| #wClass: %14s#n #r| #wAlignment: #y%8s #r|#n\n\r", get_sex( ch->sex ), get_race( ch->class ),
                  get_alignment( ch->alignment ) );
    send_to_char( buf, ch );

    stc( "#r|---------------------------------------------------------|#n\n\r", ch );

    sprintf( buf,	"#r| #wHp: #y%12s ", get_stars( ch->hit, ch->max_hit ) );
    send_to_char( buf, ch );

    sprintf( buf,	"#r| #wMn: #y%12s ", get_stars( ch->mana, ch->max_mana) );
    send_to_char( buf, ch );

    sprintf( buf,	"#r| #wMv: #y%13s #r|#n\n\r", get_stars( ch->move, ch->max_move) );
    send_to_char( buf, ch );

    stc( "#r|---------------------------------------------------------|#n\n\r", ch );

    sprintf( buf, "#r| #wStrength: %2d(#y%2d#w) #r| #wSpirit:   %2d(#y%2d#w) #r| ", 
                  ch->pcdata->perm_str, get_curr_str( ch ), ch->pcdata->perm_wis, get_curr_wis( ch ) );
    send_to_char( buf, ch );

    if ( ch->pkill < 20 )
       sprintf( buf, "#wAgility:   ??(#y??#w) #r|#n\n\r" );
       else sprintf( buf, "#wAgility:   %2d(#y%2d#w) #r|#n\n\r", ch->pcdata->perm_agi, get_curr_agi( ch ) );

    send_to_char( buf, ch );

    if ( ch->pkill < 20 )
       sprintf( buf, "#r| #wVitality: %2d(#y%2d#w) #r| #wTension:  ??(#y??#w) #r| #wDexterity: %2d(#y%2d#w) #r|#n\n\r", 
                     ch->pcdata->perm_con, get_curr_con( ch ), ch->pcdata->perm_dex, get_curr_dex( ch ) );
       else sprintf( buf, "#r| #wVitality: %2d(#y%2d#w) #r| #wTension:  %2d(#y%2d#w) #r| #wDexterity: %2d(#y%2d#w) #r|#n\n\r", 
                          ch->pcdata->perm_con, get_curr_con( ch ), ch->pcdata->perm_tension, get_curr_tens( ch ),
                          ch->pcdata->perm_dex, get_curr_dex( ch ) );

    send_to_char( buf, ch );

    sprintf( buf, "#r| #wMind:     %2d(#y%2d#w) #r| #wLuck:     %2d(#y%2d#w) #r| #wEvasion:   %2d(#y%2d#w) #r|\n\r", 
                  ch->pcdata->perm_int, get_curr_int( ch ), ch->pcdata->perm_luck, get_curr_luck( ch ),
                  ch->pcdata->perm_eva, get_curr_eva( ch ) );
    send_to_char( buf, ch );

    stc( "#r|---------------------------------------------------------|#n\n\r", ch );

    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT) )
       {
          birth_date( ch, FALSE );
          stc( "#r|---------------------------------------------------------|#n\n\r", ch );
       }

    sprintf( buf, "#r| #wAutoexit: #y%6s #r| #wAutoloot: #y%6s #r| #wMCP: #y%12s #r|#n\n\r",
                  IS_SET( ch->act, PLR_AUTOEXIT ) ? "yes" : "no",
                  IS_SET( ch->act, PLR_AUTOLOOT ) ? "yes" : "no",
                  ch->desc->out_compress ? "yes" : "no" );
    send_to_char( buf, ch );

    stc( "#r|---------------------------------------------------------|#n\n\r", ch );

    sprintf( buf, "#r| #wGold: #y%10d #r| #wBank: #y%10d #r| #wBounty: #y%9d #r|\n\r",
                  ch->gold,
                  ch->bank,
                  ch->pcdata->bounty );
    send_to_char( buf, ch );

    stc( "#r|---------------------------------------------------------|#n\n\r", ch );

    sprintf( buf, "#r| #wPKills: #y%8d #r| #wTLvl: #y%10d #r| #wMkills: #y%9d #r|#n\n\r",
                  ch->pkill, get_translevel( ch ), ch->mkill );
    send_to_char( buf, ch );

    stc( "#r\\=========================================================/#n\n\r", ch );
}

char *	const	day_name	[] =
{
    "#wDay of the Holy Siren", "#RDay of the Ferocious Ifrit", "#gDay of the Iron Titan", "#oDay of the Mysterious Luna", "#yDay of the Swift Quezacotl",
    "#CDay of the Cold Shiva", "#eDay of the Dark Diablos"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"#wIt is #y%d #wo'clock #C%s#w, %s#w, #y%d%s #wthe Month of #C%s#w.\n\r#gUtopia: Valley of Death #wstarted up at #y%s\r#wThe system time is #y%s#n\r",

	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );

    if ( weather_info.sky == SKY_ECLIPSE )
       stc( "#wThe #Csky #wis engulfed by a #ehaunting eclipse#w.#n\n\r", ch );
       else if ( weather_info.sky == SKY_APOCALYPSE )
               stc( "#RApocalypse #wis in the realm as the doors to #Rhell #ware open.#w.#n\n\r", ch );
       else if ( weather_info.sky == SKY_ICESTORM )
               stc( "#wA feriocious #Cicestorm #wis ripping everything apart.#n\n\r", ch );
       else if ( weather_info.sky == SKY_SNOWY )
               stc( "#wThe entire vicinity is #Ccovered #win a #Clayer of #ynice#w, #Cbeautiful #wsnow.#n\n\r", ch );
       else if ( weather_info.sky == SKY_LIGHTNING )
               stc( "#wA #opowerful #ythunderstorm #wis raising havoc through the vicinity#n\n\r", ch );
       else if ( weather_info.sky == SKY_RAINING )
               stc( "#wThe area is currently filled with #Ctons #wof #Cpuddles #wfrom the annoying rain#n\n\r", ch );

    return;
}

char * show_legend( int legend )
{
   static char buf[10];

   buf[0] = '\0';

   switch( legend )
   {
      default: strcpy( buf, "[Unk]" );
               break;

      case 1: strcpy( buf, "[Cit]" );
               break;

      case 2: strcpy( buf, "[Leg]" );
              break;

      case 3: strcpy( buf, "[Myt]" );
              break;

      case 4: strcpy( buf, "[Tit]" );
              break;

      case 5: strcpy( buf, "[Ora]" );
              break;

      case 6: strcpy( buf, "[Imm]" );
              break;

      case 7: strcpy( buf, "[Dem]" );
              break;

      case 8: strcpy( buf, "[Dei]" );
              break;

      case 9: strcpy( buf, "[Alm]" );
              break;

      case 10: strcpy( buf, "[Drg]" );
               break;
   }

   return buf;
}

char * show_deity( CHAR_DATA *ch )
{
   static char buf[10];

   buf[0] = '\0';

   if ( IS_NPC( ch ) )
      {
         strcpy( buf, "#p" );
         return buf;
      }

   switch( ch->pcdata->deity )
   {
      default: strcpy( buf, "#P" );
               break;

      case DEI_SIREN: strcpy( buf, "#w" );
                      break;

      case DEI_DIABLOS: strcpy( buf, "#e" );
                        break;

      case DEI_CARB: strcpy( buf, "#C" );
                     break;

      case DEI_ODIN: strcpy( buf, "#p" );
                     break;

      case DEI_IFRIT: strcpy( buf, "#R" );
                      break;

      case DEI_SHIVA: strcpy( buf, "#L" );
                      break;

      case DEI_BIRD: strcpy( buf, "#y" );
                      break;

      case DEI_TITAN: strcpy( buf, "#g" );
                      break;

      case DEI_LUNA: strcpy( buf, "#c" );
                     break;

      case DEI_GAIA: strcpy( buf, "#o" );
                     break;
   }

   return buf;
}

/*
 * New 'who' command originally by Alander of Rivers of Mud.
*/
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf5[MAX_STRING_LENGTH];
    char buf6[MAX_STRING_LENGTH];
    char buf7[MAX_STRING_LENGTH];
    char buf8[MAX_STRING_LENGTH];
    char buf9[MAX_STRING_LENGTH];
    char buf10[MAX_STRING_LENGTH];
    char buf11[MAX_STRING_LENGTH];
    char buf12[MAX_STRING_LENGTH];
    char buf13[MAX_STRING_LENGTH];
    char buf14[MAX_STRING_LENGTH];
    char buf15[MAX_STRING_LENGTH];
    char buf16[MAX_STRING_LENGTH];
    char buf17[MAX_STRING_LENGTH];
    char buf18[MAX_STRING_LENGTH];
    char buf19[MAX_STRING_LENGTH];
    char buf20[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    char king[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *wch;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    bool fClassRestrict = FALSE;
    bool fImmortalOnly = FALSE;
    bool fStatusOnly = FALSE;
    bool fTransOnly = FALSE;
    bool fGenOnly = FALSE;
    bool fRankOnly = FALSE;
    bool fTierOnly = FALSE;
    bool tempmain = FALSE; 
    bool imm = FALSE,ava = FALSE,helper = FALSE;
    char arg[MAX_STRING_LENGTH];
    bool a1 = FALSE;
    bool a2 = FALSE;
    bool a3 = FALSE;
    bool a4 = FALSE;
    bool a5 = FALSE;
    bool a6 = FALSE;
    bool a7 = FALSE;
    bool a8 = FALSE;
    bool a9 = FALSE;
    bool a10 = FALSE;
    bool a11 = FALSE;

    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;

    nNumber = 0;
    argument = one_argument( argument, arg );

    if ( arg[0] != '\0' )
    {
	if ( is_number( arg ) )
	{
          send_to_char("Enter Who by itself or 'status', 'gen', 'god', 'level', or 'transmigration'.\n\r",ch);
	    return;
	}
	else
	{
	    if (   !str_cmp( arg, "imm" ) || !str_cmp( arg, "immortal" )
		|| !str_cmp( arg, "ava" ) || !str_cmp( arg, "avatar"   ) )
	    {
		fClassRestrict = TRUE;
	    }
	    else if ( !str_cmp( arg, "status" ) )
	    {
		fStatusOnly = TRUE;
	    }
	    else if ( !str_cmp( arg, "transmigration" ) )
	    {
		fTransOnly = TRUE;
	    }
	    else if ( !str_cmp( arg, "gen" ) )
	    {
		fGenOnly = TRUE;
	    }
	    else if ( !str_cmp( arg, "level" ) )
	    {
		fTierOnly = TRUE;
	    }
	    else if ( !str_cmp( arg, "god" ) || !str_cmp( arg, "imp" ) )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
		send_to_char("Enter Who by itself or 'status', 'gen', 'god', 'level', or 'transmigration'.\n\r",ch);
		return;
	    }
	}
    }

    nMatch = 0;
    buf[0] = '\0';
    buf3[0] = '\0';
    buf4[0] = '\0';
    buf5[0] = '\0';
    buf6[0] = '\0';
    buf7[0] = '\0';
    buf8[0] = '\0';
    buf9[0] = '\0';
    buf10[0] = '\0';
    buf11[0] = '\0';
    buf12[0] = '\0';
    buf13[0] = '\0';
    buf14[0] = '\0';
    buf15[0] = '\0';
    buf16[0] = '\0';
    buf17[0] = '\0';
    buf18[0] = '\0';
    buf19[0] = '\0';
    buf20[0] = '\0';
 
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	char const *class;

	if ( (d->connected != CON_PLAYING && d->connected != CON_EDITING) 
	   || (!can_see( ch, d->character ) && (!IS_SET(ch->act, PLR_WATCHER)) )  )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;

	if ( wch->level < iLevelLower || wch->level > iLevelUpper
           || ( fImmortalOnly  && wch->level <  LEVEL_IMMORTAL )
           || ( fClassRestrict && wch->level != LEVEL_HERO ) )
	   continue;

      if ( fStatusOnly )
         {
            if ( wch == ch )
               continue;

            if ( armageddon != TRUE )
               {
                  int range = 20;
 
                  if ( wch->pkill < 20 )
                     range = 5;
                     else if ( wch->pkill < 40 )
                             range = 10;

                  if ( wch->pkill + range < ch->pkill && ch->retaliate != wch )
                     continue;
               }

            if ( (get_age(wch)-17) < 2 )
               continue;
         }

      if ( fTransOnly )
         {
            if ( wch == ch )
               continue;

            if ( (get_age(wch)-17) < 2 )
               continue;

            if ( ( get_translevel( wch ) + 1000000 <= get_translevel( ch ) )
                 || ( get_translevel( wch ) >= get_translevel( ch ) + 1000000 ) )
               continue;
         }

      if ( fTierOnly )
         {
            if ( wch == ch )
               continue;

            if ( wch->tier != ch->tier )
               continue;
         }

      if ( fGenOnly )
         {
            if ( wch == ch || wch->tier != ch->tier )
               continue;

            if ( wch->generation > ch->generation )
               continue;

            if ( (get_age(wch)-17) < 2 )
               continue;
         }

	nMatch++;

	class = " ";

	if ( ( IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH) ) 
             && wch->pcdata->chobj != NULL )
	{
	    if (wch->pcdata->chobj->pIndexData->vnum == 12)
	    	class = "#y-#R=#wA Head#R=#y-   ";
          else if (wch->pcdata->chobj->pIndexData->vnum == 30005)
	    	class = "#c-#p=#yA Brain#p=#c-  ";
	    else
	    	class = "#P-#r=#cObject#r=#P-   ";
       }
        else if ( ( get_age(wch)-17 ) < 2 )
	    	class = "#P-#w=#pNewbie#w=#P-   ";
        else if ( wch->pcdata->capture_timer > 0 )
	    	class = "#e+#w+#ePrisoner#w+#e+ ";
        else if ( wch->raiding > 0 )
	    	class = "#c|=#e+#RRaider#e+#c=| ";
        else switch ( wch->level )
	     {
                default: break;
                case MAX_LEVEL     :
                case MAX_LEVEL -  1: 
                case MAX_LEVEL -  2:
                case MAX_LEVEL -  3:
                case MAX_LEVEL -  4:
                case MAX_LEVEL -  5: class = "#y*#L=#wAdmin#L=#y*    "; break;
                case MAX_LEVEL -  6:
                case MAX_LEVEL -  7:
                case MAX_LEVEL -  8:
                case MAX_LEVEL -  9:
                case MAX_LEVEL - 10:
                case MAX_LEVEL - 11:
                case MAX_LEVEL - 12:

                if ( wch->pcdata->team > 0 )
                   {
                       class ="#w-=#PPK #CQUE#gST#y=- ";
                       break;
                   }
                   else if ( IS_EXTRA(wch, EXTRA_AFK) )
                   {
                       class ="#y-#r=#RAFK#r=#y-      ";
                       break;
                   }
                   else if ( IS_SET( wch->arena, AFF2_COMPETITOR ) )
                           {
                               class ="#w-#g=#CK.O.F.#g=#w-   ";
                               break;
                           }
                   else if ( IS_SET( wch->arena, AFF2_TEAM1 ) || IS_SET( wch->arena, AFF2_TEAM2 ) )
                           {
                               class ="#y-#c=#oTEAM#c=#y-     ";
                               break;
                           }
                   else if ( IS_SET(wch->arena, AFF2_INARENA) )
                           {
                               class ="#c-#w=#gARENA#w=#c-    ";
                               break;
                           }
                   else if ( IS_SET( wch->extra, EXTRA_HUNT ) )
                           {
                               class ="#p-#e=#ySCAVENGER#e=#p-";
                               break;
                           }
  	           else {
 		            if ( wch->pkill == 0 )          class = "#y-#w=#cNovice#w=#y-   ";
                           else if ( wch->pkill < 10 )  class = "#e-#L=#RKiller#L=#e-   ";
                           else if ( wch->pkill < 20 )  class = "#r-#g=#eSadist#g=#r-   ";
                           else if ( wch->pkill < 30 )  class = "#y-#w=#LWarrior#w=#y-  ";
                           else if ( wch->pkill < 50 )  class = "#e-#r=#gTyrant#r=#e-   ";
                           else if ( wch->pkill < 75 )  class = "#w-#g=#yLegend#g=#w-   ";
                           else if ( wch->pkill < 100 ) class = "#L-#P=#wDiety#P=#L-    ";
                           else if ( wch->pkill < 150 ) class = "#g-#e=#PSupreme#e=#g-  ";
                           else if ( wch->pkill < 200 ) class = "#r-#l=#eDestroyer#l=#r-";
                                                        else class = "#c-#w=#CUltimate#w=#c- ";
                       }
	     }

	    if ( IS_SET( wch->special, SPC_DOPPLE ) )
             {
                if ( strlen( wch->pcdata->title ) > 0 )
                   sprintf( kav," #w-]+%s+[-#e#e#e", wch->pcdata->title );
                   else sprintf(kav," -]+#CDo#wPp#yLe#RGa#gNg#wE#er+[-");
             }
             else if ( wch->class == CLASS_PALADIN )
             {
                if ( strlen( wch->pcdata->title ) > 0 )
                   sprintf( kav," #w==)=%s#e---#e#e", wch->pcdata->title );
                   else if ( IS_SET( wch->special, SPC_SWORD ) )
                           sprintf(kav," #w==)=#eSword #oMaster#e---#e#e#e");
                   else if ( IS_CLASS( wch, CLASS_DIVINE_KNIGHT ) )
                           sprintf(kav," #w==)=#CHeavenly #yKnight#e---#e#e#e");
                           else sprintf(kav," #w==)=#yPala#Ldin#e---#e#e#e");
             }
             else if ( wch->class == CLASS_NINJA )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #b***%s#b***#b#b", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_VAMPIRE ) )
                                   sprintf(kav," #r***#ySt#ea#wl#ek#yer#r***");
                           else sprintf(kav," #b***#eNi#rn#eja#b***#b#b");
                     }
             else if ( wch->class == CLASS_SHADOW )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #e***%s#e***#e#e", wch->pcdata->title );
                           else if ( wch->clannum == 1 )
                                   sprintf(kav," #e***#wElite #rAss#eas#rsin#e***#e");
                                   else sprintf(kav," #e***#rAss#eas#rsin#e***#e#e");
                     }
             else if ( wch->class == CLASS_SHADOW_WAR )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #L***%s#L***#e#e", wch->pcdata->title );
                           else sprintf(kav," #L***#eShadow #RWarrior#L***#e#e#e");
                     }
             else if ( wch->class == CLASS_NIGHT_BLADE )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #b***%s#b***#b#b", wch->pcdata->title );
                           else sprintf(kav," #b***#eGa#rid#een#b***#b#b");
                     }
             else if ( wch->class == CLASS_DEMON )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #r[%s#r]#r#r", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_FIEND ) && IS_CLASS( wch, CLASS_SHADOW_WAR ) )
                                   sprintf(kav," #r[#eShadow #RD#eem#ro#en#r]");
                                   else sprintf(kav," #r[#eD#Re#wm#Ro#en#r]");
                     }
             else if ( wch->class == CLASS_FIEND )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #r[%s#r]#r#r", wch->pcdata->title );
                           else sprintf(kav," #r[#eF#Ri#we#Rn#ed#r]");
                     }
             else if ( wch->class == CLASS_DRAGON )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #p-*-%s#p-*-#p#p", wch->pcdata->title );
                           else if ( wch->clannum == 1 )
                                   sprintf(kav," #p-*-#wElder #RD#erago#Rn#p-*-#p");
                                   else sprintf(kav," #p-*-#RD#erago#Rn#p-*-#p#p");
                     }
             else if ( wch->class == CLASS_WIZARD )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #y{%s#y}#y#y", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_NECROMANCER ) )
                                   sprintf(kav," #c+#y{#eDark #PMagician#y}#c+#y");
                           else if ( wch->clannum == 6 )
                                   sprintf(kav," #y{#wAr#ech #CMa#Pge#y}#y");
                                   else sprintf(kav," #y{#wWi#eza#Crd#y}#y#y");
                     }
             else if ( wch->class == CLASS_SAIYAN )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf(kav," #R(((%s#y)))#y#y", wch->pcdata->title);
                           else sprintf(kav," #R(((#ySaiya#w-#RJin#y)))#y#y");
                     }
             else if ( wch->class == CLASS_SAMURAI )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf(kav," #c[=)%s#y(=]#y#y", wch->pcdata->title);
                           else sprintf(kav," #c[=)#wSam#Gu#wrai#y(=]#y#y");
                     }
             else if ( wch->class == CLASS_ASSASSIN && IS_SET( wch->special, SPC_SOR ) )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #p{%s#p}#p#p", wch->pcdata->title );
                           else sprintf(kav," #p{#wSorc#eeror#p}#p#p#p");
                     }
             else if ( wch->class == CLASS_KURUDA )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #e{{%s#e}}#e#e", wch->pcdata->title );
                           else if ( wch->class_2 == CLASS_ASSASSIN && IS_SET( wch->special, SPC_KNIGHT )
                                     && IS_SET( wch->special, SPC_SOR ) )
                                   sprintf(kav," #w{{#cElite #eCom#yman#eder#w}}#e");
                                   else sprintf(kav," #e{{#wCom#yman#wder#e}}#e#e");
                     }
             else if ( wch->class == CLASS_ASSASSIN && IS_SET( wch->special, SPC_MONK ) )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #e.x[%s#e]x.#e#e", wch->pcdata->title );
                           else sprintf(kav," #e.x[#yKuruda#e]x.#e#e#e#e");
                     }
             else if ( wch->class == CLASS_ASSASSIN && IS_SET( wch->special, SPC_KNIGHT ) )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #e==)=%s#w---#w#w", wch->pcdata->title );
                           else sprintf(kav," #e==)=Dark #cKnight#w---#w#w#w#w");
                     }
             else if ( wch->class == CLASS_WEREWOLF )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #o((%s#o))#o#o", wch->pcdata->title );
                           else sprintf(kav," #o((#RWe#grewo#Rlf#o))#o#o");
                     }
             else if ( wch->class == CLASS_VAMPIRE )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #R<<%s#R>>#R#R", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_WEREWOLF ) && IS_CLASS( wch, CLASS_DAYWALKER ) )
                                   sprintf(kav," #R<<#oHy#Rbr#oid#R>>#R#R");
                                   else sprintf(kav," #R<<#yVam#epire#R>>#R#R#R");
                     }
             else if ( wch->class == CLASS_MONK )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #w.x[%s#w]x.#w#w", wch->pcdata->title );
                           else if ( wch->class_3 == CLASS_ASSASSIN && IS_SET( wch->special, SPC_KNIGHT )
                                     && wch->class_2 == CLASS_NINJA )
                                   sprintf(kav," #w.x[#PThe #yO#wn#Ce#w]x.#w");
                           else if ( wch->class_2 == CLASS_WEREWOLF )
                                   sprintf(kav," #y.x[#oGa#wr#oou#y]x.#w#w");
                           else if ( wch->class_2 == CLASS_NINJA )
                                   sprintf(kav," #w.x[#yGrand Master#w]x.#w#w#w#w");
                                   else sprintf(kav," #w.x[#yMonk#w]x.#w#w#w#w");
                     }
	       else if ( wch->class == CLASS_MASTER )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #e.o0%s#e0o.#e#e", wch->pcdata->title );
                           else if ( wch->clannum == 1 )
                                   sprintf(kav," #C.o0#cBlade #yLord#C0o.#e#e#e");
                                   else sprintf(kav," #e.o0#PWeapons Master#e0o.#e#e#e#e");
                     }
             else if ( wch->class == CLASS_DROW )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #e.o0%s#e0o.#e#e", wch->pcdata->title );
                           else if ( wch->pcdata->powers[DROW_CLASS] == DROW_AVATAR )
                                   sprintf(kav," #o.o0#eLloth's #PAvatar#o0o.#e#e#e");
                                   else sprintf(kav," #e.o0#PDrow#e0o.#e#e#e#e");
                     }
             else if ( wch->class == CLASS_ANGEL )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #y~~~%s#y~~~#y#y", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_DRAGON_KNIGHT ) )
                                   sprintf(kav," #c~~~#PGuardian #wAn#Cg#wel#C~~~#y");
                           else if ( IS_CLASS( wch, CLASS_DEMON ) )
                                   sprintf(kav," #e~~~#RFallen #wAn#Cg#wel#e~~~#y");
                           else sprintf(kav," #y~~~#wAn#Cg#wel#y~~~#y#y");
                     }
             else if ( wch->class == CLASS_DRAGOON )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #c---=%s#e--->>>#e#e", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_FIEND ) && IS_CLASS( wch, CLASS_SAIYAN )
                                     && IS_CLASS( wch, CLASS_SHADOW_WAR ) )
                                   sprintf(kav," #wx#Cx#wx #RBAD ASS #ex#Cx#ex");
                                   else sprintf(kav," #c---=#gHigh#wwind#e--->>>#e#e#e");
                     }
             else if ( wch->class == CLASS_DRAGON_KNIGHT )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf( kav," #C---=%s#e--->>>#e#e", wch->pcdata->title );
                           else if ( IS_SET( wch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DIVINE ) )
                                   sprintf(kav," #C---=#CDivine #PDragoon#e--->>>#e#e#e");
                           else sprintf(kav," #C---=#PDragoon#e--->>>#e#e#e#e");
                     }
             else if ( wch->class == CLASS_UNDEAD_KNIGHT )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf(kav," #e\\_-%s#e-_/#e#e", wch->pcdata->title );
                           else if ( IS_CLASS( wch, CLASS_NECROMANCER ) )
                                   sprintf(kav," #e\\_-#rUndead #wLord#e-_/#e#e#e");
                                   else sprintf(kav," #e\\_-#rUndead #wKnight#e-_/#e#e#e");
                     }
             else if ( wch->class == CLASS_NECROMANCER )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf(kav," #e{%s#e}#e#e", wch->pcdata->title);
                           else sprintf(kav," #e{Nec#Rroman#ecer}#e#e#e#e");
                     }
			 else if ( wch->class == CLASS_SNOW_ELF )
                     {
                        if ( strlen( wch->pcdata->title ) > 0 )
                           sprintf(kav," #g<{>%s#g<}>#e#e", wch->pcdata->title);
                           else if ( IS_CLASS( wch, CLASS_WISP ) && IS_CLASS( wch, CLASS_PHOENIX )
                                     && IS_CLASS( wch, CLASS_DUERGAR ) )
                                   sprintf(kav," #g<{>#yEle#Cmen#gtal #RFury#g<}>#e");
                                   else sprintf(kav," #g<{>#CSnow#gy #CElf#g<}>#e#e");
                     }
			 else if ( wch->class == CLASS_WISP )
					 {
						if ( strlen( wch->pcdata->title ) > 0 )
						   sprintf(kav," #p[~]%s#p[~]#e#e", wch->pcdata->title);
						   else sprintf(kav, " #p[~]#yW#7is#yp#p[~]#e#e");
					 }
			 else if ( wch->class == CLASS_PHOENIX )
					 {
						if ( strlen( wch->pcdata->title ) > 0 )
						   sprintf(kav, " #r~*~%s#r~*~#e#e", wch->pcdata->title);
						   else sprintf(kav, " #r~*~#RPh#0oen#Rix#r~*~#e#e");
                     }
			 else if ( wch->class == CLASS_DUERGAR )
					 { 
						if ( strlen( wch->pcdata->title ) > 0 )
						   sprintf(kav, " #G(*)%s#G(*)#e#e", wch->pcdata->title);
						   else sprintf(kav, " #G(*)#gDu#7erg#gar#G(*)#e#e");
					 }
			 else if ( wch->class == CLASS_HOBBIT )
					 {
						if ( strlen( wch->pcdata->title ) > 0 )
						   sprintf(kav, " #e[]%s#e[]#e#e", wch->pcdata->title);
						   else sprintf(kav, " #0[]#7Ho#0bb#7it#0[]#e#e");
					 }
			 else if ( wch->class == CLASS_DAYWALKER )
					 {
						if ( strlen( wch->pcdata->title ) > 0 )
						   sprintf( kav, " #R<-]=O%s#RO=[->#e#e", wch->pcdata->title );
						   else sprintf( kav, " #R<-]=O#yDay#Cwal#yker#RO=[->#R#R");
					 }
			 else if ( wch->class == CLASS_DARAMON )
					 {
						if ( strlen( wch->pcdata->title ) > 0 )
						   sprintf( kav, " #P.x[%s#P]x.#e#e", wch->pcdata->title );
						   else sprintf( kav, " #P.x[#wDa#yram#won#P]x.#P#P");
					 }
             else sprintf( kav, " #yH#wu#ym#wa#yn#y#y" );

      if ( wch->pcdata->kingdom != 0 )
         sprintf( king, "#y{**%s#y**}", king_table[wch->pcdata->kingdom].who_name);
         else sprintf( king, "" );

	if ( wch->level > 6 )
        {
            sprintf( buf14 + strlen(buf14), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
	  imm = TRUE;
	}    
      else if ( IS_SET( wch->act, ACT_HELPER ) )
      {
         sprintf( buf15 + strlen(buf15), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
         helper = TRUE;
	}    
	else if ( wch->level > 0 )
        {
          if ( ( get_age(wch)-17 ) < 2 )
          {
             sprintf( buf3 + strlen(buf3), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a1 = TRUE;
          }
          else if ( wch->pkill >= 200 )
          {
             sprintf( buf13 + strlen(buf13), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a11 = TRUE;
          }
          else if ( wch->pkill >= 150 && wch->pkill < 200 )
          {
             sprintf( buf12 + strlen(buf12), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a10 = TRUE;
          }
          else if ( wch->pkill >= 100 && wch->pkill < 150 )
          {
             sprintf( buf11 + strlen(buf11), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a9 = TRUE;
          }
          else if ( wch->pkill >= 75 && wch->pkill < 100 )
          {
             sprintf( buf10 + strlen(buf10), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a8 = TRUE;
          }
          else if ( wch->pkill >= 50 && wch->pkill < 75 )
          {
             sprintf( buf9 + strlen(buf9), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a7 = TRUE;
          }
          else if ( wch->pkill >= 30 && wch->pkill < 50 )
          {
             sprintf( buf8 + strlen(buf8), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a6 = TRUE;
          }
          else if ( wch->pkill >= 20 && wch->pkill < 30 )
          {
             sprintf( buf7 + strlen(buf7), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a5 = TRUE;
          }
          else if ( wch->pkill >= 10 && wch->pkill < 20 )
          {
             sprintf( buf6 + strlen(buf6), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a4 = TRUE;
          }
          else if ( wch->pkill >= 1 && wch->pkill < 10 )
          {
             sprintf( buf5 + strlen(buf5), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a3 = TRUE;
          }
          else if ( wch->pkill == 0 )
          {
             sprintf( buf4 + strlen(buf4), "  %-10s %s%-12s  %-39s %-20s\n\r",
	              class, show_deity( wch ), wch->pcdata->switchname,
                      kav, king );
             a2 = TRUE;
          }

	  ava = TRUE;
        }

     if ( !tempmain )
        {
           stc( "                   #y<#r<#y<#L----------  #RValley #0of #RDeath  #L----------#y>#r>#y>\n\r", ch);
           tempmain = TRUE;
        }
    }  

    if ( imm )
    {
      stc( "\n\r                       #L----#w<#y<<   <<#w< #cImmortals #w>#y>>   >>#w>#L----#n\n\r", ch );
      stc(buf14,ch);
    }

    if ( helper )
    {
      stc( "\n\r                       #L----#o<#c<<   <<#y< #P Helpers  #y>#c>>   >>#o>#L----#n\n\r", ch );
      stc(buf15,ch);
    }

    if ( ava )
    {
      stc( "\n\r                       #L----#g<#p<<   <<#g< #c Players  #g>#p>>   >>#g>#L----#n\n\r", ch );
      if ( a11 ) stc(buf13,ch);
      if ( a10 ) stc(buf12,ch);
      if ( a9  ) stc(buf11,ch);
      if ( a8  ) stc(buf10,ch);
      if ( a7  ) stc(buf9,ch);
      if ( a6  ) stc(buf8,ch);
      if ( a5  ) stc(buf7,ch);
      if ( a4  ) stc(buf6,ch);
      if ( a3  ) stc(buf5,ch);
      if ( a2  ) stc(buf4,ch);
      if ( a1  ) stc(buf3,ch);
    }
 
    stc("\n\r",ch);
    sprintf( buf, "                    #p----<<<   <<< #wTotal Players : %d #p>>>   >>>----#n\n\r", nMatch );
    stc( buf, ch );
}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    bool found;

    if (!IS_NPC(ch) && IS_HEAD(ch,LOST_HEAD))
        {send_to_char( "You are not a container.\n\r", ch ); return;}
    else if (!IS_NPC(ch) && IS_EXTRA(ch,EXTRA_OSWITCH))
    {
        if ( !IS_NPC(ch) && (obj = ch->pcdata->chobj) == NULL)
	    {send_to_char( "You are not a container.\n\r", ch ); return;}
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "You are not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "You don't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "You are empty.\n\r", ch );
		break;
	    }
	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in you.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "You contain a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "You're about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "You're about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "You are almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "You're completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow you are MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    act( "$p contain:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}


void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_wizlist( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "wizlist" );
    return;
}

void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found, supersight;

    one_argument( argument, arg );

    if ( IS_SET(ch->in_room->area->bits,WASTE) && !IS_CLASS( ch, CLASS_WEREWOLF ) )
       {
           stc("You cant make out anything in this rugged terrain.\n\r",ch);
           return;
       }

    if ( arg[0] == '\0' )
    {
      if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
         supersight = TRUE;
         else supersight = FALSE;

	sprintf( buf, "#wPlayers near you #y(#c%s#y)#n:\n\r", ch->in_room->area->name );
	send_to_char( buf, ch );
	found = FALSE;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( ( d->connected == CON_PLAYING
	    ||   d->connected == CON_EDITING)
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   victim->pcdata->chobj == NULL
	    &&   ( supersight || can_see( ch, victim ) ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}

	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}

void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;
    int overall;
    int con_hit;
    int con_dam;
    int con_ac;
    int con_hp;

    one_argument( argument, arg );
    overall = 0;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    act( "You examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_CHAR );
    act( "$n examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_NOTVICT );
    act( "$n examines you closely, looking for your weaknesses.", ch, NULL, victim, TO_VICT );

    if (!IS_NPC(victim)) do_skill(ch,victim->name);

    con_hit = char_hitroll(victim);
    con_dam = char_damroll(victim);
    con_ac = char_ac(victim);
    con_hp = victim->hit;

    if (con_hp < 1) con_hp = 1;

    diff = victim->level - ch->level + con_hit - char_hitroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more skilled than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as skilled as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as skilled as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as skilled as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more skilled than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more skilled than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more skilled than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = victim->level - ch->level + con_dam - char_damroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more powerful than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as powerful as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as powerful as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as powerful as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more powerful than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more powerful than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more powerful than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = ch->hit * 100 / con_hp;
         if ( diff <=  10 ) {msg = "$E is currently FAR healthier than you are."; overall = overall - 3;}
    else if ( diff <=  50 ) {msg = "$E is currently much healthier than you are."; overall = overall - 2;}
    else if ( diff <=  75 ) {msg = "$E is currently slightly healthier than you are."; overall = overall - 1;}
    else if ( diff <= 125 ) {msg = "$E is currently about as healthy as you are.";}
    else if ( diff <= 200 ) {msg = "You are currently slightly healthier than $M."; overall = overall + 1;}
    else if ( diff <= 500 ) {msg = "You are currently much healthier than $M."; overall = overall + 2;}
    else                    {msg = "You are currently FAR healthier than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = con_ac - char_ac(ch);
         if ( diff <= -100) {msg = "$E is FAR better armoured than you."; overall = overall - 3;}
    else if ( diff <= -50 ) {msg = "$E looks much better armoured than you."; overall = overall - 2;}
    else if ( diff <= -25 ) {msg = "$E looks better armoured than you."; overall = overall - 1;}
    else if ( diff <=  25 ) {msg = "$E seems about as well armoured as you.";}
    else if ( diff <=  50 ) {msg = "You are better armoured than $M."; overall = overall + 1;}
    else if ( diff <=  100) {msg = "You are much better armoured than $M."; overall = overall + 2;}
    else                    {msg = "You are FAR better armoured than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = overall;
         if ( diff <= -11 ) msg = "Conclusion: $E would kill you in seconds.";
    else if ( diff <=  -7 ) msg = "Conclusion: You would need a lot of luck to beat $M.";
    else if ( diff <=  -3 ) msg = "Conclusion: You would need some luck to beat $N.";
    else if ( diff <=   2 ) msg = "Conclusion: It would be a very close fight.";
    else if ( diff <=   6 ) msg = "Conclusion: You shouldn't have a lot of trouble defeating $M.";
    else if ( diff <=  10 ) msg = "Conclusion: $N is no match for you.  You can easily beat $M.";
    else                    msg = "Conclusion: $E wouldn't last more than a few seconds against you.";
    act( msg, ch, NULL, victim, TO_CHAR );

    return;
}


void set_prefix( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->prefix );
    ch->prefix = str_dup( buf );
    return;
}



void do_prefix( CHAR_DATA *ch, char *argument )
{
 
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your prefix to what?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 13 )
	argument[13] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_prefix( ch, argument );
    send_to_char( "Prefix Set.\n\r", ch );
}

void set_title( CHAR_DATA *ch, char *argument )
{
    int count; 
    char buf[MAX_STRING_LENGTH];
    int hash = 3;

    if ( IS_NPC(ch) )
	return;

    if ( IS_CLASS( ch, CLASS_ASSASSIN ) )
       {
          stc( "You are forbidden from changing your set title clone.\n\r", ch );
          return;
       }

    if ( argument[0] == '\0' )
       {
           send_to_char( "Reseting title.\n\r", ch );
           free_string( ch->pcdata->title );
           ch->pcdata->title = str_dup( "" );
           return;
       }

    for ( count = 0; count < strlen(argument); count++ )
    {
       if ( count < strlen(argument) && argument[count] == '#' && argument[count + 1] != '#' )
          hash--;
    }

    if ( hash < 0 )
    {
       stc("You can only have a max of 3 colours in your title.\n\r",ch);
       return;
    }

    if ( count - ( ( 3 - hash ) * 2 ) > 14 )
    {
       stc("You can only have a max of 14 letters in your title.\n\r",ch);
       return;
    }

    smash_tilde( argument );

    for ( count = 0; count < hash; count++ )
    {
       strcat( argument, "#w" );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( argument );

    sprintf( buf, "Your new title is now: %s\n\r", ch->pcdata->title );
    send_to_char( buf, ch );
}

void set_kingtitle( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];
    
    if ( IS_NPC(ch) )
    {
        bug( "Set_title: NPC.", 0 );
        return;
    }
     
    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
        buf[0] = ' ';
        strcpy( buf+1, title );
    }
    else
    {
        strcpy( buf, title );
    }
    
    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );   
    return;
}    


void do_title( CHAR_DATA *ch, char *argument )
{
 
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 17 )
	argument[17] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    send_to_char( "Ok.\n\r", ch );
}

void do_kingrank( CHAR_DATA *ch, char *argument )
{
     
    char buf[MAX_STRING_LENGTH];
        
    if ( IS_NPC(ch) )
        return;
    
    if ( argument[0] == '\0' )
    {
        send_to_char( "Change your title to what?\n\r", ch );
        return;
    }   
     
    if ( strlen(argument) > 17 )
        argument[17] = '\0';
 
    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_kingtitle( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}

void do_afk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

	if ( IS_NPC(ch) )
	return;

        if (ch->fight_timer>0)
	{
	send_to_char("not with a fighttimer.\n\r",ch);
	return;
	}

        if (IS_SET(ch->extra, EXTRA_AFK))
	{
	send_to_char( "You are no longer AFK.\n\r", ch );
	sprintf(buf,"%s is no longer AFK!",ch->pcdata->switchname);
        ADD_COLOUR(ch,buf,L_RED);
	do_info(ch,buf);
	REMOVE_BIT(ch->extra,EXTRA_AFK);
      ch->wait = 12;
        return;
	}
	
	else if (!IS_SET(ch->extra,EXTRA_AFK))
	{
	send_to_char( "You are now AFK.\n\r", ch );
	sprintf(buf,"%s is now AFK!",ch->pcdata->switchname);
	ADD_COLOUR(ch,buf,L_RED);
	do_info(ch,buf);
	SET_BIT(ch->extra,EXTRA_AFK);
	WAIT_STATE(ch, 25);
        return;
	}

else
return;

}


void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen2(buf) + strlen2(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    char hit_str[MAX_INPUT_LENGTH];
    char mana_str[MAX_INPUT_LENGTH];
    char move_str[MAX_INPUT_LENGTH];
    char mhit_str[MAX_INPUT_LENGTH];
    char mmana_str[MAX_INPUT_LENGTH];
    char mmove_str[MAX_INPUT_LENGTH];
    char exp_str[MAX_INPUT_LENGTH];
    sprintf(hit_str, "%d", ch->hit);
    COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
    sprintf(mana_str, "%d", ch->mana);
    COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
    sprintf(move_str, "%d", ch->move);
    COL_SCALE(move_str, ch, ch->move, ch->max_move);
    sprintf(exp_str, "%d", ch->exp);
    COL_SCALE(exp_str, ch, ch->exp, 1000);
    sprintf(mhit_str, "%d", ch->max_hit);
    ADD_COLOUR(ch, mhit_str, L_CYAN);
    sprintf(mmana_str, "%d", ch->max_mana);
    ADD_COLOUR(ch, mmana_str, L_CYAN);
    sprintf(mmove_str, "%d", ch->max_move);
    ADD_COLOUR(ch, mmove_str, L_CYAN);
    sprintf( buf,
	"You report: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	hit_str,  mhit_str,
	mana_str, mmana_str,
	move_str, mmove_str,
	exp_str   );

    send_to_char( buf, ch );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch == NULL ) continue;
	if ( vch == ch ) continue;
	if ( vch->in_room == NULL ) continue;
	if ( vch->in_room != ch->in_room ) continue;
    	sprintf(hit_str, "%d", ch->hit);
    	COL_SCALE(hit_str, vch, ch->hit, ch->max_hit);
    	sprintf(mana_str, "%d", ch->mana);
    	COL_SCALE(mana_str, vch, ch->mana, ch->max_mana);
    	sprintf(move_str, "%d", ch->move);
    	COL_SCALE(move_str, vch, ch->move, ch->max_move);
    	sprintf(exp_str, "%d", ch->exp);
    	COL_SCALE(exp_str, vch, ch->exp, 1000);
    	sprintf(mhit_str, "%d", ch->max_hit);
    	ADD_COLOUR(vch, mhit_str, L_CYAN);
    	sprintf(mmana_str, "%d", ch->max_mana);
    	ADD_COLOUR(vch, mmana_str, L_CYAN);
    	sprintf(mmove_str, "%d", ch->max_move);
    	ADD_COLOUR(vch, mmove_str, L_CYAN);
    	if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    ch->morph,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
        else
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    IS_NPC(ch) ? capitalize(ch->short_descr) : ch->name,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
	buf[0] = UPPER(buf[0]);
    	send_to_char( buf, vch );
    }
    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	int col;

	col    = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    if ( ch->level < skill_table[sn].skill_level )
		continue;
	    sprintf( buf, "%18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "You have %d exp left.\n\r", ch->exp );
	send_to_char( buf, ch );
    }
    else if ( !str_cmp(argument,"all") )
    {
	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

      for ( sn = 0; sn < MAX_SKILL; sn++ )
      {
	    if ( skill_table[sn].name != NULL && ch->level >= skill_table[sn].skill_level )
		 ch->pcdata->learned[sn] = 100;
      }

      stc("You practice all of the skills and spells available to you.\n\r",ch);
      return;
    }
    else
    {
	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   ch->level < skill_table[sn].skill_level ) )
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	
	if ( ch->pcdata->learned[sn] >= SKILL_ADEPT )
	{
	    sprintf( buf, "You are already an adept of %s.\n\r",
	         skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    if (ch->pcdata->learned[sn] == 0)
	    {
	    	ch->pcdata->learned[sn] +=100;
	    }
	    else
	    {
	    	ch->pcdata->learned[sn] += int_app[get_curr_int(ch)].learn;
	    }
	    if ( ch->pcdata->learned[sn] < SKILL_ADEPT )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = SKILL_ADEPT;
		act( "You are now an adept of $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
	    }
	}
    }
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit * 0.35 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( str_cmp( arg1, ch->pcdata->pwd ) &&
         str_cmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ))
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->pcdata->switchname );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    if (!IS_EXTRA(ch,EXTRA_NEWPASS)) SET_BIT(ch->extra,EXTRA_NEWPASS);
    save_char_obj( ch );
    if (ch->desc != NULL && ch->desc->connected == CON_PLAYING )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", social_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}

void do_gsocial( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim = NULL;
    int cmd;
    bool found;
    DESCRIPTOR_DATA *d;

    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg );

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( arg2[0] == social_table[cmd].name[0]
	&&   !str_prefix( arg2, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
       {
          stc("That social doesn't exists.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
    {
        buf[0] = '\0';

        if ( social_table[cmd].others_no_arg != NULL )
           {
               strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
               strcat( buf, social_table[cmd].others_no_arg );
               strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );
    
               for ( d = descriptor_list; d != NULL; d = d->next )
               {
                  if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO) )
                     act( buf, ch, NULL, d->character,TO_VICT );
               }

               buf[0] = '\0';
           }

        if ( social_table[cmd].char_no_arg != NULL )
           {
               strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
               strcat( buf, social_table[cmd].char_no_arg );
               strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );

               act( buf, ch, NULL, NULL, TO_CHAR );
           }
    }
    else if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( IS_NPC(victim) )
    {
	send_to_char( "Not on mobs.\n\r", ch );
    }
    else if ( victim == ch )
    {
        buf[0] = '\0';

        if ( social_table[cmd].others_auto != NULL )
           {
              strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
              strcat( buf, social_table[cmd].others_auto );
              strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );
    
              for ( d = descriptor_list; d != NULL; d = d->next )
              {
                 if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO) )
                    act( buf, ch, NULL, d->character,TO_VICT );
              }

              buf[0] = '\0';
           }

        if ( social_table[cmd].char_auto != NULL )
           {
              strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
              strcat( buf, social_table[cmd].char_auto );
              strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );

              act( buf, ch, NULL, NULL, TO_CHAR );
           }
    }
    else
    {
        buf[0] = '\0';

        if ( social_table[cmd].others_world != NULL )
           {
              strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
              strcat( buf, social_table[cmd].others_world );
              strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );
    
              for ( d = descriptor_list; d != NULL; d = d->next )
              {
                 if ( d->connected != CON_PLAYING || IS_SET(d->character->deaf, CHANNEL_INFO) )
                    continue;

                 if ( d->character == ch || d->character == victim )
                    continue;

                 if ( !can_see(d->character,victim) )
                    act( buf, ch, "someone", d->character, TO_VICT );
                    else act( buf, ch, victim->pcdata->switchname, d->character, TO_VICT );
              }

              buf[0] = '\0';
           }

        if ( social_table[cmd].char_found != NULL )
           {
              strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
              strcat( buf, social_table[cmd].char_found );
              strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );

              act( buf, ch, NULL, victim, TO_CHAR );

              buf[0] = '\0';
           }

        if ( social_table[cmd].vict_found != NULL )
           {
              strcat( buf, "#C[#w<#gGSoCiAL#w>#c]#y " );
              strcat( buf, social_table[cmd].vict_found );
              strcat( buf, " #C[#w<#gGSoCiAL#w>#c]#n" );

              act( buf, ch, NULL, victim, TO_VICT );
           }
    }
 
}

void do_xsocials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; xsocial_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", xsocial_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    col = 0;
    for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name != NULL; sn++ )
    {
	sprintf( buf, "%-12s", skill_table[sn].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level == 0
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
	{
	    send_to_char( "You are silenced.\n\r", ch );
	    return;
	}

	send_to_char( "Channels:", ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_CHAT)
	    ? " +CHAT"
	    : " -chat",
	    ch );

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_IMMTALK)
		? " +IMMTALK"
		: " -immtalk",
		ch );
	}

	send_to_char( !IS_SET(ch->deaf, CHANNEL_MUSIC)
	    ? " +MUSIC"
	    : " -music",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_INFO)
	    ? " +INFO"
	    : " -info",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_CLANTALK)
	    ? " +CLANTALK"
	    : " -clantalk",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_SHOUT)
	    ? " +SHOUT"
	    : " -shout",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_HOWL)
	    ? " +HOWL"
	    : " -howl",
	    ch );

	if (IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_LOG)
		? " +LOG"
		: " -log",
		ch );
	}

        if (IS_CLASS( ch, CLASS_MAGE) || IS_IMMORTAL(ch))
        {
            send_to_char( !IS_SET(ch->deaf, CHANNEL_MAGETALK)
                ? " +MAGE"
                : " -mage",
                ch );
        }

  	  if (IS_CLASS(ch, CLASS_DRAGOON) || IS_IMMORTAL(ch)) {
		send_to_char( !IS_SET(ch->deaf, CHANNEL_DRAGTALK)
			? " +DRAGTALK"
			: " -dragtalk",
			ch ); }

  	  if ( IS_CLASS(ch, CLASS_PALADIN) || IS_IMMORTAL(ch)) {
		send_to_char( !IS_SET(ch->deaf, CHANNEL_PALADIN)
			? " +PALADIN"
			: " -paladin",
			ch ); }

        if (IS_CLASS( ch, CLASS_DRAGON) || IS_IMMORTAL(ch))
        {
            send_to_char( !IS_SET(ch->deaf, CHANNEL_ROAR)
                ? " +ROAR"
                : " -roar",
                ch );
        }

        if ( IS_CLASS( ch, CLASS_MONK) || IS_IMMORTAL(ch) )
        {
            send_to_char( !IS_SET(ch->deaf, CHANNEL_MONK)
                ? " +MONK"
                : " -monk",
                ch );
        }

	if (IS_CLASS(ch, CLASS_DEMON) || IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_PRAY)
		? " +PRAY"
		: " -pray",
		ch );
	}

	if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_VAMPTALK)
		? " +VAMP"
		: " -vamp",
		ch );
	}

	if (IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch)) {
		send_to_char( !IS_SET(ch->deaf, CHANNEL_HIGHTALK)
			? " +HIGHT"
			: " -hight",
			ch ); }

	if( IS_CLASS(ch, CLASS_DROW) || IS_IMMORTAL(ch)) {
		send_to_char( !IS_SET(ch->deaf, CHANNEL_SIGN)
			? " +SIGN"
			: " -sign",
			ch ); }

	send_to_char( !IS_SET(ch->deaf, CHANNEL_TELL)
	    ? " +TELL"
	    : " -tell",
	    ch );

	send_to_char( ".\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

        if ( !str_cmp( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
	else if ( !str_cmp( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
	else if ( !str_cmp( arg+1, "music"    ) ) bit = CHANNEL_MUSIC;
	else if ( !str_cmp( arg+1, "clantalk" ) ) bit = CHANNEL_CLANTALK;
	else if ( !str_cmp( arg+1, "shout"    ) ) bit = CHANNEL_SHOUT;
	else if ( !str_cmp( arg+1, "yell"     ) ) bit = CHANNEL_YELL;
	else if ( !str_cmp( arg+1, "howl"     ) ) bit = CHANNEL_HOWL;
	else if ( !str_cmp( arg+1, "dragtalk"     ) ) bit = CHANNEL_DRAGTALK;
	else if ( !str_cmp( arg+1, "roar"     ) ) bit = CHANNEL_ROAR;
	else if ( !str_cmp( arg+1, "info"     ) ) bit = CHANNEL_INFO;
	else if ( !str_cmp( arg+1, "paladin"     ) ) bit = CHANNEL_PALADIN;
	else if (IS_IMMORTAL(ch) && !str_cmp( arg+1, "log") ) bit = CHANNEL_LOG;
        else if ( !str_cmp( arg+1, "monk"     ) ) bit = CHANNEL_MONK;          
        else if ( !str_cmp( arg+1, "mage"     ) ) bit = CHANNEL_MAGETALK;
	else if ( !str_cmp( arg+1, "vamp"     ) ) bit = CHANNEL_VAMPTALK;
	else if ( !str_cmp( arg+1, "tell"     ) ) bit = CHANNEL_TELL;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    REMOVE_BIT (ch->deaf, bit);
	else
	    SET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "[ Keyword  ] Option\n\r", ch );

	send_to_char(  IS_SET(ch->act, PLR_ANSI)
            ? "[+ANSI     ] You have ansi colour on.\n\r"
	    : "[-ansi     ] You have ansi colour off.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOEXIT)
            ? "[+AUTOEXIT ] You automatically see exits.\n\r"
	    : "[-autoexit ] You don't automatically see exits.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOLOOT)
	    ? "[+AUTOLOOT ] You automatically loot corpses.\n\r"
	    : "[-autoloot ] You don't automatically loot corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOSAC)
	    ? "[+AUTOSAC  ] You automatically sacrifice corpses.\n\r"
	    : "[-autosac  ] You don't automatically sacrifice corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BLANK)
	    ? "[+BLANK    ] You have a blank line before your prompt.\n\r"
	    : "[-blank    ] You have no blank line before your prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BRIEF)
	    ? "[+BRIEF    ] You see brief descriptions.\n\r"
	    : "[-brief    ] You see long descriptions.\n\r"
	    , ch );
         
	send_to_char(  IS_SET(ch->act, PLR_COMBINE)
	    ? "[+COMBINE  ] You see object lists in combined format.\n\r"
	    : "[-combine  ] You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PROMPT)
	    ? "[+PROMPT   ] You have a prompt.\n\r"
	    : "[-prompt   ] You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_TELNET_GA)
	    ? "[+TELNETGA ] You receive a telnet GA sequence.\n\r"
	    : "[-telnetga ] You don't receive a telnet GA sequence.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_SILENCE)
	    ? "[+SILENCE  ] You are silenced.\n\r"
	    : ""
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "[-tell     ] You can't use 'tell'.\n\r"
	    , ch );
    }
    else
    {
	bool fSet;
	int bit;

	     if ( arg[0] == '+' ) fSet = TRUE;
	else if ( arg[0] == '-' ) fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}

             if ( !str_cmp( arg+1, "ansi"     ) ) bit = PLR_ANSI;
        else if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
	else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
	else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
	else if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
        else if ( !str_cmp( arg+1, "brief2"    ) ) bit = PLR_BRIEF2;
	else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
	    SET_BIT    (ch->act, bit);
	else
	    REMOVE_BIT (ch->act, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}

void do_ansi( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_ANSI)) do_config(ch,"-ansi");
    else do_config(ch,"+ansi");
    return;
}

void do_autoexit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOEXIT)) do_config(ch,"-autoexit");
    else do_config(ch,"+autoexit");
    return;
}

void do_autoloot( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOLOOT)) do_config(ch,"-autoloot");
    else do_config(ch,"+autoloot");
    return;
}

void do_autosac( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOSAC)) do_config(ch,"-autosac");
    else do_config(ch,"+autosac");
    return;
}

void do_blank( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BLANK)) do_config(ch,"-blank");
    else do_config(ch,"+blank");
    return;
}

void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF)) do_config(ch,"-brief");
    else do_config(ch,"+brief");
    return;
}

void do_brief2( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF2)) do_config(ch,"-brief2");
    else do_config(ch,"+brief2");
    return;
}

void do_diagnose( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg  [MAX_INPUT_LENGTH];
    int teeth = 0;
    int ribs = 0;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (arg == '\0')
    {
	send_to_char("Who do you wish to diagnose?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("Nobody here by that name.\n\r",ch);
	return;
    }
    act("$n examines $N carefully, diagnosing $S injuries.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you carefully, diagnosing your injuries.",ch,NULL,victim,TO_VICT);
    act("Your diagnoses of $N reveals the following...",ch,NULL,victim,TO_CHAR);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + 
	   victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
	   victim->loc_hp[6] ) == 0 )
    {
	act("$N has no apparent injuries.",ch,NULL,victim,TO_CHAR);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	return;
    }
    /* Check head */
    if (IS_HEAD(victim,LOST_EYE_L) && IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost both of $S eyes.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_L))
	act("$N has lost $S left eye.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost $S right eye.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_EAR_L) && IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost both of $S ears.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_L))
	act("$N has lost $S left ear.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost $S right ear.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_NOSE))
	act("$N has lost $S nose.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,BROKEN_NOSE))
	act("$N has got a broken nose.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,BROKEN_JAW))
	act("$N has got a broken jaw.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
	act("$N has had $S head cut off.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HEAD))
	    act("...Blood is spurting from the stump of $S neck.",ch,NULL,victim,TO_CHAR);
    }
    else
    {
	if (IS_BODY(victim,BROKEN_NECK))
	    act("$N has got a broken neck.",ch,NULL,victim,TO_CHAR);
	if (IS_BODY(victim,CUT_THROAT))
	{
	    act("$N has had $S throat cut open.",ch,NULL,victim,TO_CHAR);
	    if (IS_BLEEDING(victim,BLEEDING_THROAT))
		act("...Blood is pouring from the wound.",ch,NULL,victim,TO_CHAR);
	}
    }
    if (IS_HEAD(victim,BROKEN_SKULL))
	act("$N has got a broken skull.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_TOOTH_1 )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2 )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4 )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8 )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16)) teeth += 16;
    if (teeth > 0)
    {
	sprintf(buf,"$N has had %d teeth knocked out.",teeth);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_HEAD(victim,LOST_TONGUE))
	act("$N has had $S tongue ripped out.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
    	send_to_char("----------------------------------------------------------------------------\n\r",ch);
    	return;
    }
    /* Check body */
    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;
    if (ribs > 0)
    {
	sprintf(buf,"$N has got %d broken ribs.",ribs);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_BODY(victim,BROKEN_SPINE))
	act("$N has got a broken spine.",ch,NULL,victim,TO_CHAR);
    /* Check arms */
    check_left_arm(ch,victim);
    check_right_arm(ch,victim);
    check_left_leg(ch,victim);
    check_right_leg(ch,victim);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    return;
}

void check_left_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S arms.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L) && IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_ARM))
    {
	act("$N has lost $S left arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N arms are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,BROKEN_ARM))
	act("$N's left arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND) &&
	!IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S hands.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L) && IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_HAND))
    {
	act("$N has lost $S left hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	return;
    if (IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost $S right arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND))
	return;
    if (IS_ARM_R(victim,LOST_HAND))
    {
	act("$N has lost $S right hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_left_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost both of $S legs.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L) && IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_LEG))
    {
	act("$N has lost $S left leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N legs are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_LEG_L(victim,BROKEN_LEG))
	act("$N's left leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost both of $S feet.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L) && IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_FOOT))
    {
	act("$N has lost $S left foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_L(victim,BROKEN_TOE_A) && !IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_B) && !IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_C) && !IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_D) && !IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	return;
    if (IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost $S right leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N's right leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
	return;
    if (IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost $S right foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_R(victim,BROKEN_TOE_A) && !IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_B) && !IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_C) && !IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_D) && !IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void obj_score( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    sprintf( buf,"You are %s.\n\r",obj->short_descr);
    send_to_char( buf, ch );

    sprintf( buf,"Type %s, Extra flags %s.\n\r",item_type_name(obj),
	extra_bit_name(obj->extra_flags));
    send_to_char( buf, ch );

    sprintf( buf,"You weigh %d pounds and are worth %d gold coins.\n\r",obj->weight,obj->cost);
    send_to_char( buf, ch );

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
	obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You were created by %s, and are owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	sprintf( buf, "You were created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You are owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }

    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "You contain level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_QUEST:
	sprintf( buf, "Your quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "You have %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	sprintf( buf, "You inflict %d to %d damage in combat (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		sprintf(buf,"You are a minor spell weapon.\n\r");
	    else if (obj->level < 20)
		sprintf(buf,"You are a lesser spell weapon.\n\r");
	    else if (obj->level < 30)
		sprintf(buf,"You are an average spell weapon.\n\r");
	    else if (obj->level < 40)
		sprintf(buf,"You are a greater spell weapon.\n\r");
	    else if (obj->level < 50)
		sprintf(buf,"You are a major spell weapon.\n\r");
	    else
		sprintf(buf,"You are a supreme spell weapon.\n\r");
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    sprintf (buf, "You are dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "You are the bane of all evil.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "You drink the souls of your victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "You have been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "You crackle with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "You are dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "You have been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4 || itemtype == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "You allow your wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "You grant your wielder the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 4)
	    sprintf (buf, "You allow your wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "You render your wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "You allow your wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "You protect your wielder from evil.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "You protect your wielder in combat.\n\r");
	else if (itemtype == 9)
	    sprintf (buf, "You allow your wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    sprintf (buf, "You surround your wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    sprintf (buf, "You surround your wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    sprintf (buf, "You surround your wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    sprintf (buf, "You surround your wielder with a shield of acid.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "You fill your wielder with the power to destroy all.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "You enable your wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "You protect your wielder from player attacks.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "You surround your wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "You grant your wielder superior protection.\n\r");
	else if (itemtype == 22 || itemtype == 14)
	    sprintf (buf, "You grant your wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "You make your wielder fleet-footed.\n\r");
	else if (itemtype == 50)
	    sprintf (buf, "You have the power to stun your victims.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "You conceal your wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "You invoke the power of your wielders beast.\n\r");
	else if (itemtype == 80)
	    sprintf (buf, "You allow your wielder to invoke the power of the holy saint.\n\r");
	else if (itemtype == 81)
	    sprintf (buf, "You allow your wielder to guard themself.\n\r");
	else if (itemtype == 82)
	    sprintf (buf, "You wrap your wielder in a cloak of darkness.\n\r");
	else if (itemtype == 83)
	    sprintf (buf, "You fill your wielder with unholy rage.\n\r");
	else if (itemtype == 84)
	    sprintf (buf, "You allow the wielder to see all.\n\r");
	else if (itemtype == 85)
	    sprintf (buf, "You give incredible regeneration powers.\n\r");
	else if (itemtype == 61)
	    sprintf (buf, "You do crippling damage to all dragoons.\n\r");
	else if (itemtype == 62)
	    sprintf (buf, "You nullify all dragoon attacks.\n\r");
	else if (itemtype == 63)
	    sprintf (buf, "You allow the wielder to control their victims.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );

      if ( IS_SET(obj->weapflags, TRUE_FIRE) )
         stc( "You are engulfed with #Rferocious #rflames#n.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_ICE) )
         stc( "You are covered with #Licy #wshards#n.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_MOON) )
         stc( "You are blessed with the #ypowers#n of #oLuna#n.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_SAINT) )
         stc( "You use the #wsacred#n powers of the #wheavens#n.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_DARK) )
         stc( "Your powers are derived from the #eDark #RLord#n himself.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_BOLT) )
         stc( "You are #Psizzling#n with intense #yenergy#n.\n\r", ch );

      if ( IS_SET(obj->weapflags, TRUE_EARTH) )
         stc( "You are encased in a #orocky #csubstance#n.\n\r", ch );

	break;

    case ITEM_ARMOR:
	sprintf( buf, "Your armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4 || obj->value[3] == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "You allow your wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "You grant your wearer the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 4)
	    sprintf (buf, "You allow your wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "You render your wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "You allow your wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "You protect your wearer from evil.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "You protect your wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "You allow your wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    sprintf (buf, "You surround your wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    sprintf (buf, "You surround your wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    sprintf (buf, "You surround your wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    sprintf (buf, "You surround your wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 30)
	    sprintf (buf, "You fill your wielder with the power to destroy all.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "You enable your wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "You protect your wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "You surround your wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "You grant your wearer superior protection.\n\r");
	else if (obj->value[3] == 22 || obj->value[3] == 14)
	    sprintf (buf, "You grant your wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "You make your wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "You conceal your wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "You invoke the power of your wearers beast.\n\r");
	else if (obj->value[3] == 80)
	    sprintf (buf, "This ancient item invokes the power of the holy saint.\n\r");
	else if (obj->value[3] == 81)
	    sprintf (buf, "This ancient item allows you to guard yourself.\n\r");
	else if (obj->value[3] == 82)
	    sprintf (buf, "This ancient item wraps you in a cloak of darkness.\n\r");
	else if (obj->value[3] == 83)
	    sprintf (buf, "This ancient item fills you with unholy rage.\n\r");
	else if (obj->value[3] == 84)
	    sprintf (buf, "This ancient item allows you to see all.\n\r");
	else if (obj->value[3] == 85)
	    sprintf (buf, "This ancient item gives you incredible regeneration powers.\n\r");
	else if (obj->value[3] == 61)
	    sprintf (buf, "You do crippling damage to all dragoons.\n\r");
	else if (obj->value[3] == 62)
	    sprintf (buf, "You nullify all dragoon attacks.\n\r");
	else if (obj->value[3] == 63)
	    sprintf (buf, "You allow the wielder to control their victims.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "prompt" );
       return;
   }

   if( !str_cmp( argument, "on" ) )
   {
      if (IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt on!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         SET_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !str_cmp( argument, "off" ) )
   {
      if (!IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt off!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         REMOVE_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !str_cmp( argument, "clear" ) )
   {
      free_string(ch->prompt);
      ch->prompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

/* Do_prompt from Morgenes from Aldara Mud */
void do_cprompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "cprompt" );
       return;
   }

   if( !str_cmp( argument, "clear" ) )
   {
      free_string(ch->cprompt);
      ch->cprompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->cprompt );
   ch->cprompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

void do_setlogin( CHAR_DATA *ch, char *argument )
{
    int count; 
    char buf[MAX_STRING_LENGTH];
    int test = 0;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Reseting login message.\n\r", ch );
      free_string( ch->pcdata->login );
      ch->pcdata->login = str_dup( "" );
	return;
    }

    for ( count = 0; count < UMIN( 68, strlen(argument) ); count++ )
    {
       if ( argument[count] == '$' )
          {
            if ( argument[count + 1] != 'n' )
               {
                  stc("You are only allow a $n in this message.\n\r",ch);
                  return;
               }
               else {
                       test = 1;
                    }
          }
    }

    if ( test == 0 )
    {
       stc("You need to have a $n in your login message.\n\r",ch);
       return;
    }

    if ( strlen(argument) > 70 )
	argument[70] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);

    free_string( ch->pcdata->login );
    ch->pcdata->login = str_dup( buf );

    sprintf( buf, "Your login message is now: %s\n\r", ch->pcdata->login );
    send_to_char( buf, ch );
}

void do_setlogout( CHAR_DATA *ch, char *argument )
{
    int count; 
    char buf[MAX_STRING_LENGTH];
    int test = 0;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Reseting logout message.\n\r", ch );
      free_string( ch->pcdata->logout );
      ch->pcdata->logout = str_dup( "" );
	return;
    }

    for ( count = 0; count < UMIN( 68, strlen(argument) ); count++ )
    {
       if ( argument[count] == '$' )
          {
            if ( argument[count + 1] != 'n' )
               {
                  stc("You are only allow a $n in this message.\n\r",ch);
                  return;
               }
               else {
                       test = 1;
                    }
          }
    }

    if ( test == 0 )
    {
       stc("You need to have a $n in your logout message.\n\r",ch);
       return;
    }

    if ( strlen(argument) > 70 )
	argument[70] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);

    free_string( ch->pcdata->logout );
    ch->pcdata->logout = str_dup( buf );

    sprintf( buf, "Your logout message is now: %s\n\r", ch->pcdata->logout );
    send_to_char( buf, ch );
}

void do_setfatality( CHAR_DATA *ch, char *argument )
{
    int count; 
    char buf[MAX_STRING_LENGTH];
    int test = 0;
    int test2 = 0;

    if ( IS_NPC(ch) )
       return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Reseting 1st line of fatality message.\n\r", ch );
      free_string( ch->pcdata->decap );
      ch->pcdata->decap = str_dup( "" );
	return;
    }

    for ( count = 0; count < UMIN( 68, strlen(argument) ); count++ )
    {
       if ( argument[count] == '$' && argument[count + 1] != 'n' && argument[count + 1] != 't' )
          {
              stc("You are only allow a $n and $t in this message.\n\r",ch);
              return;
          }

       if ( argument[count] == '$' && argument[count + 1] == 'n' && test == 0 )
          {
             test = 1;
          }

       if ( argument[count] == '$' && argument[count + 1] == 't' && test2 == 0 )
          {
             test2 = 1;
          }
    }

    if ( test == 0 || test2 == 0 )
    {
        for ( count = 0; count < UMIN( 68, strlen(ch->pcdata->decap_2) ); count++ )
        {
          if ( ch->pcdata->decap_2[count] == '$' && ch->pcdata->decap_2[count + 1] != 'n' && ch->pcdata->decap_2[count + 1] != 't' )
          {
              stc("You are only allow a $n and $t in this message.\n\r",ch);
              return;
          }

          if ( ch->pcdata->decap_2[count] == '$' && ch->pcdata->decap_2[count + 1] == 'n' && test == 0 )
          {
             test = 1;
          }

          if ( ch->pcdata->decap_2[count] == '$' && ch->pcdata->decap_2[count + 1] == 't' && test2 == 0 )
          {
             test2 = 1;
          }
       }
    }

    if ( test == 0 || test2 == 0 )
    {
       stc("You need to have both a $n and a $t in your fatality message.\n\r",ch);
       return;
    }

    if ( strlen(argument) > 70 )
	argument[70] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);

    free_string( ch->pcdata->decap );
    ch->pcdata->decap = str_dup( buf );

    sprintf( buf, "Your fatality message is now: %s\n\r", ch->pcdata->decap );
    send_to_char( buf, ch );
}

void do_setfatality2( CHAR_DATA *ch, char *argument )
{
    int count; 
    char buf[MAX_STRING_LENGTH];
    int test = 0;
    int test2 = 0;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Reseting 2nd line of fatality message.\n\r", ch );
      free_string( ch->pcdata->decap_2 );
      ch->pcdata->decap_2 = str_dup( "" );
	return;
    }

    for ( count = 0; count < UMIN( 68, strlen(argument) ); count++ )
    {
       if ( argument[count] == '$' && argument[count + 1] != 'n' && argument[count + 1] != 't' )
          {
              stc("You are only allow a $n and $t in this message.\n\r",ch);
              return;
          }

       if ( argument[count] == '$' && argument[count + 1] == 'n' && test == 0 )
          {
             test = 1;
          }

       if ( argument[count] == '$' && argument[count + 1] == 't' && test2 == 0 )
          {
             test2 = 1;
          }
    }

    if ( test == 0 || test2 == 0 )
    {
        for ( count = 0; count < UMIN( 68, strlen(ch->pcdata->decap) ); count++ )
        {
          if ( ch->pcdata->decap[count] == '$' && ch->pcdata->decap[count + 1] != 'n' && ch->pcdata->decap[count + 1] != 't' )
          {
              stc("You are only allow a $n and $t in this message.\n\r",ch);
              return;
          }

          if ( ch->pcdata->decap[count] == '$' && ch->pcdata->decap[count + 1] == 'n' && test == 0 )
          {
             test = 1;
          }

          if ( ch->pcdata->decap[count] == '$' && ch->pcdata->decap[count + 1] == 't' && test2 == 0 )
          {
             test2 = 1;
          }
       }
    }

    if ( test == 0 || test2 == 0 )
    {
       stc("You need to have both a $n and a $t in your fatality message.\n\r",ch);
       return;
    }

    if ( strlen(argument) > 70 )
	argument[70] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);

    free_string( ch->pcdata->decap_2 );
    ch->pcdata->decap_2 = str_dup( buf );

    sprintf( buf, "Your fatality message is now: %s\n\r", ch->pcdata->decap_2 );
    send_to_char( buf, ch );
}
