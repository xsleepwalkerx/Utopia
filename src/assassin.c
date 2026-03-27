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

char * show_style( CHAR_DATA *ch )
{
   static char buf[23];

   buf[0] = '\0';

   if ( IS_CLASS( ch, CLASS_SHADOW ) )
      {
         if ( ch->gifts[CURRENT_METER] == AMI )
            sprintf( buf, "#LMi#n" );
            else if ( ch->gifts[CURRENT_METER] == ALI )
                    sprintf( buf, "#RLi#n" );
            else if ( ch->gifts[CURRENT_METER] == ANI )
                    sprintf( buf, "#yNi#n" );
            else if ( ch->gifts[CURRENT_METER] == ACHI )
                    sprintf( buf, "#wChi#n" );
            else if ( ch->gifts[CURRENT_METER] == AKI )
                    sprintf( buf, "#eKi#n" );
            else if ( ch->gifts[CURRENT_METER] == AZI )
                    sprintf( buf, "#pZi#n" );
                    else strcpy( buf, "none" );
      }
      else strcpy( buf, "none" );

   return buf;
}

void do_switch(CHAR_DATA *ch, char *argument) 
{
   char arg[MAX_STRING_LENGTH];
   int count;

   argument = one_argument(argument, arg);

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_CLASS( ch, CLASS_SHADOW ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->clannum != 1 )
      {
         stc("You're not the master assassin yet.\n\r",ch);
         return;
      }

   if ( !str_cmp( "chi", arg ) )
      {
         if ( ch->gifts[CURRENT_METER] == ACHI )
			{
			   stc("You are already in this tech.\n\r",ch);
			   return;
			}

         stc("You get ready to strike in #7#zChi#n technique!\n\r",ch);

         ch->gifts[CURRENT_METER] = ACHI;
         ch->wait = 30 - UMIN( 30, ch->pkill * 1.5 );
         return;
      }
      else if ( !str_cmp( "ki", arg ) )
      {
         if ( ch->gifts[CURRENT_METER] == AKI )
			{
			   stc("You are already in this tech.\n\r",ch);
			   return;
			}

         stc("You get ready to strike in #7#zKi#n technique!\n\r", ch );
        
         ch->gifts[CURRENT_METER] = AKI;
         ch->wait = 30 - UMIN( 30, ch->pkill * 1.5 );
         return;
      }
      else if ( !str_cmp( "mi", arg ) )
      {
         if ( ch->gifts[CURRENT_METER] == AMI )
			{
			   stc("You are already in this tech.\n\r",ch);
			   return;
			}

         stc("You get ready to strike in #7#zMi#n technique!\n\r", ch );
          
         ch->gifts[CURRENT_METER] = AMI;
         ch->wait = 30 - UMIN( 30, ch->pkill * 1.5 );
         return;
      }
      else if ( !str_cmp( "ni", arg ) )
      {
         if ( ch->gifts[CURRENT_METER] == ANI )
			{
			   stc("You are already in this tech.\n\r",ch);
			   return;
			}

         stc("You get ready to strike in #7#zNi#n technique!\n\r", ch );

         ch->gifts[CURRENT_METER] = ANI;
         ch->wait = 30 - UMIN( 30, ch->pkill * 1.5 );
         return;
      }
      else if ( !str_cmp( "li", arg ) )
      {
         if ( ch->gifts[CURRENT_METER] == ALI )
			{
			   stc("You are already in this tech.\n\r",ch);
			   return;
			}

         stc("You get ready to strike in #7#zLi#n technique!\n\r", ch);
       
         ch->gifts[CURRENT_METER] = ALI;
         ch->wait = 30 - UMIN( 30, ch->pkill * 1.5 );
         return;
      }
      else if ( !str_cmp( "zi", arg ) )
      {
         if ( ch->gifts[CURRENT_METER] == AZI )
			{
			   stc("You are already in this tech.\n\r",ch);
			   return;
			}

         stc("You get ready to strike in #7#zZi#n technique!\n\r", ch);

         ch->gifts[CURRENT_METER] = AZI;
         ch->wait = 30 - UMIN( 30, ch->pkill * 1.5 );
         return;
      }
      else if ( !str_cmp( "none", arg ) )
      {

         stc( "You can choose between Chi, Ki, Li, Ni, Mi, Zi techs assassin\n\r", ch );
         return;
      }

   stc( "That isn't a technique, make sure you haven't made a typo.\n\r", ch );
}

void do_mind_barrier( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS( ch, CLASS_PALADIN ) || ch->clannum != 3 )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( IS_SET(ch->in_room->room_flags, ROOM_BARRIER) )
     {
        stc("You release your focus on the room as the invisible barrier vanishes.",ch);
        REMOVE_BIT(ch->in_room->room_flags, ROOM_BARRIER);
        return;
     }

  if ( ch->move < 50000 )
     {
        stc("You are too tired to perform this technique.\n\r",ch);
        return;
     }

  ch->move -= 50000;
  SET_BIT(ch->in_room->room_flags, ROOM_BARRIER);

  act("You raise your finger on your forehead as you concentrate furiously!", ch, NULL, NULL, TO_CHAR);
  act("$n raises $s finger on $s forehead as $e concentrates furiously!", ch, NULL, NULL, TO_ROOM);
  act("You summon a mystical barrier across the room as your hair blows through the wind!", ch, NULL, NULL, TO_CHAR);
  act("$n summons a mystical barrier across the room as $s hair blows through the wind!", ch, NULL, NULL, TO_ROOM);
  ch->wait = 6;
}

void do_assassin_learn( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost;
    char buf[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    cost = 0;

    if (!IS_CLASS(ch, CLASS_SHADOW) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
       centre_text("#e<<<-----(<#r Powers of the Mother of Destruction #e>)----->>>#n",ch);
       centre_text("#e\\^<>^/#n",ch);
       centre_text("#e \\/ #n",ch);
       send_to_char("\n\r",ch);
 	 sprintf(buf,"#wCHI #eTechniques#n: %d", ch->gifts[ACHI]);
       centre_text(buf,ch);
 	 sprintf(buf,"#eKI  #eTechniques#n: %d", ch->gifts[AKI]);
       centre_text(buf,ch);
 	 sprintf(buf,"#RLI  #eTechniques#n: %d", ch->gifts[ALI]);
       centre_text(buf,ch);
 	 sprintf(buf,"#pZI  #eTechniques#n: %d", ch->gifts[AZI]);
       centre_text(buf,ch);
 	 sprintf(buf,"#bMI  #eTechniques#n: %d", ch->gifts[AMI]);
       centre_text(buf,ch);
 	 sprintf(buf,"#yNI  #eTechniques#n: %d", ch->gifts[ANI]);
       centre_text(buf,ch);
       centre_text("#e<<<----------------------------------------------->>>#n\n",ch);
       return;
    }

    if (!str_cmp(arg,"chi"))
    {
	if ( ch->gifts[ACHI] >= 10 )
	{
	    send_to_char("You had already learn all of the techniques for this fighting style.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each technique costs 5 million exp to train.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[ACHI]++;
	ch->exp -= cost;
	sprintf(buf,"You improve on your CHI technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"ki"))
    {
	if ( ch->gifts[AKI] >= 10 )
	{
	    send_to_char("You had already learn all of the techniques for this fighting style.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each technique costs 5 million exp to train.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[AKI]++;
	ch->exp -= cost;
	sprintf(buf,"You improve on your KI technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"ni"))
    {
	if ( ch->gifts[ANI] >= 10 )
	{
	    send_to_char("You had already learn all of the techniques for this fighting style.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each technique costs 5 million exp to train.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[ANI]++;
	ch->exp -= cost;
	sprintf(buf,"You improve on your NI technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"li"))
    {
	if ( ch->gifts[ALI] >= 10 )
	{
	    send_to_char("You had already learn all of the techniques for this fighting style.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each technique costs 5 million exp to train.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[ALI]++;
	ch->exp -= cost;
	sprintf(buf,"You improve on your LI technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"zi"))
    {
	if ( ch->gifts[AZI] >= 10 )
	{
	    send_to_char("You had already learn all of the techniques for this fighting style.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each technique costs 5 million exp to train.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[AZI]++;
	ch->exp -= cost;
	sprintf(buf,"You improve on your ZI technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"mi"))
    {
	if ( ch->gifts[AMI] >= 10 )
	{
	    send_to_char("You had already learn all of the techniques for this fighting style.\n\r",ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    sprintf(buf,"Each technique costs 50 million exp to train.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	ch->gifts[AMI]++;
	ch->exp -= cost;
	sprintf(buf,"You improve on your MI technique.\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("To improve your techniques, type: learn Chi/Ki/Ni/Li/Zi/Mi.\n\r",ch);
}


void do_study ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];

   if (IS_NPC(ch)) return;
  
   argument = one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_SHADOW) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

   if (arg[0] == '\0')
    {
	sprintf(buf,"Techniques: #wCHI (%d), #eKI (%d), #yNI (%d), #RLI (%d), #bMI (%d), #pZI (%d).\n\r",
	ch->gifts[ACHI], ch->gifts[AKI],ch->gifts[ANI],ch->gifts[ALI],ch->gifts[AMI],ch->gifts[AZI]);
	send_to_char(buf,ch);
	return;
    }

    if (arg[0] != '\0')
    {
	if (!str_cmp(arg,"chi"))
	{
	    send_to_char("#wChi - A positive lifeforce energy transfuse for devastating attacks.\n\r\n\r",ch);
	    if (ch->gifts[ACHI] < 1) 
		send_to_char("#gYou know none of the Chi techniques.\n\r",ch);
	    if (ch->gifts[ACHI] > 0) 
		send_to_char("#wLevitate     #g-               Hover yourself above the ground.\n\r",ch);
	    if (ch->gifts[ACHI] > 1) 
		send_to_char("#wGuidedShot   #g-Guided Shot-   Release a ball of chi energy towards the victim.\n\r",ch);
	    if (ch->gifts[ACHI] > 2) 
		send_to_char("#wInnerHealing #g-Inner Healing- Uses your chi to heal and remove negative affects.\n\r",ch);
	    if (ch->gifts[ACHI] > 3) 
		send_to_char("#wInnerRage    #g-Inner Rage-    Your chi is manifested into an uncontrollable rage.\n\r",ch);
		if (ch->gifts[ACHI] > 4)
		send_to_char("#wYang         #g-Yang Power-    Increases your defense and decrease your offensive power.\n\r", ch);
  	    if (ch->gifts[ACHI] > 5)
		send_to_char("#wYin          #g-Yin Power-     Increases your offense and decrease your defensive power.\n\r", ch);
          if (ch->gifts[ACHI] > 6)
		send_to_char("#wCounter      #g-Counter Force- A chi manifested counterattack. (Auto)\r\n", ch);
          if (ch->gifts[ACHI] > 8)
		send_to_char("#wInnerFury    #g-Inner Fury-    A series of insane attacks which drains your chi.\r\n", ch);
	    if (ch->gifts[ACHI] > 9)
             {
		    send_to_char("#wInnerShield  #g-Inner Shield-  a wall of positive Chi energy which dissolves affects.\r\n", ch);
		    send_to_char("#wChiBlast     #g-Chi Blast-     Improved Guided Shot.\r\n", ch);
             }
		return;
	}
	else if (!str_cmp(arg,"ki"))
	{
	    send_to_char("#eKi - A negative lifeforce energy to disrupt the balance.\n\r\n\r",ch);
	    if (ch->gifts[AKI] < 1) 
		send_to_char("#gYou know none of the Ki techniques.\n\r",ch);
	    if (ch->gifts[AKI] > 0) 
		send_to_char("#eFocus           #g-                 Targets your victim at will.\n\r",ch);
	    if (ch->gifts[AKI] > 1) 
		send_to_char("#ePainless        #g-Painless Strike- Attacks a victim's nerve system weakening them.\n\r",ch);
	    if (ch->gifts[AKI] > 2) 
		send_to_char("#eHandBlast       #g-Hand Blast-      Blinds the victim with a blast of Kiotic energy.\n\r",ch);
	    if (ch->gifts[AKI] > 3) 
		send_to_char("#eChinSmite       #g-Chin Smite-      This technique may stun the victim.\n\r",ch);
	    if (ch->gifts[AKI] > 4)
		send_to_char("#eLegDispatcher   #g-Leg Dispatcher-  Mumb the legs, preventing chances of escape.\n\r", ch);
	    if (ch->gifts[AKI] > 5)
		send_to_char("#eArmGrab         #g-Arm Grab-        Mumb the arms, reducing the victim's offensive power.\n\r", ch );
          if (ch->gifts[AKI] > 6)
		send_to_char("#eBodyRip         #g-BodyRip-         Can leave a victim totally mutalitated.\n\r", ch );
	    if (ch->gifts[AKI] > 7)
		send_to_char("#eKiPinch         #g-Ki Pinch-        May leave the victim paralayzed.\n\r", ch);
	    if (ch->gifts[AKI] > 8)
		send_to_char("#eDispel          #g-                 Remove all of a victim's magical effects.\n\r", ch);
	    if (ch->gifts[AKI] > 9)
            send_to_char("#eKiWrath         #g-Kiotic Wrath-    A blast of kiotic energy stunning everyone in the room.\n\r", ch);
	    return;
	}
	else if (!str_cmp(arg,"li"))
	{
	    send_to_char("#RLi - The techniques of the shadows.\n\r\n\r",ch);
	    if (ch->gifts[ALI] < 1) 
		send_to_char("#gYou know none of the Li techniques.\n\r",ch);
	    if (ch->gifts[ALI] > 0) 
		send_to_char("#RSilent Walk    #g-             Move without a trace (Auto).\n\r",ch);
	    if (ch->gifts[ALI] > 2) 
		send_to_char("#RDisguise       #g-Disguise-    Every good assassin knows this one.\n\r",ch);
	    if (ch->gifts[ALI] > 3) 
		send_to_char("#RVanish         #g-Vanish-      The shadows hide you from plain view.\n\r",ch);
 	    if (ch->gifts[ALI] > 4)
		send_to_char("#RCold Stare     #g-             Your stare is enough to lower an opponent's prowess. (Auto)\n\r", ch);
	    if (ch->gifts[ALI] > 6)
		send_to_char("#RIllusion Blur  #g-             You move so fast, that you seem like a blur. (Auto)\n\r", ch );
	    if ( ch->gifts[ALI] > 7 )
             {
		   send_to_char("#RShadow Grasp   #g-             Your shadow prevents people from fleeing. (Auto)\n\r", ch );
		   send_to_char("#RFade           #g-             Use the shadows to aid your escape. (Auto)\n\r", ch );
		   send_to_char("#RFinale Chance  #g-             One more chance to... kill... (Auto)\n\r", ch );
             }
	    if (ch->gifts[ALI] > 8)
		send_to_char("#RDispose        #g-Dispose-     A desvastating attack to the back.\n\r", ch );
	    return;
	}
	else if (!str_cmp(arg,"mi"))
	{
	    send_to_char("#bMi - Gifts from Mother of Destruction.\n\r\n\r",ch);
	    if (ch->gifts[AMI] < 0) 
		send_to_char("#gYou know none of the Mi techniques.\n\r",ch);
	    if (ch->gifts[AMI] > 0) 
		send_to_char("#bEnhanced Regeneration   #g- Improved regen rate. (Auto)\n\r",ch);
	    if (ch->gifts[AMI] > 1) 
             send_to_char("#bTriheal         #g-Tri Heal- Partially restore yourself.\n\r",ch);
	    if (ch->gifts[AMI] > 5)
		send_to_char("#bDivine Strength #g- Cast this to bestow your target with rippling strength.\n\r", ch);
	    if (ch->gifts[AMI] > 6)
		send_to_char("#bMidrain         #g-Mi Drain- Steal the lifeforce from your victim.\n\r", ch );
          if (ch->gifts[AMI] > 7)
		send_to_char("#bDarkMatter      #g-Dark Matter- A black vortex sucks in your victim, ripping them apart.\n\r", ch );
          if (ch->gifts[AMI] > 9)
		send_to_char("#bPossess         #g- Use this technique at the doors of death to save yourself.\n\r", ch );
	    return;
	}
	else if (!str_cmp(arg,"zi"))
	{
	    send_to_char("#pZi - Skills in Weaponry.\n\r\n\r",ch);
	    if (ch->gifts[AZI] < 0) 
		send_to_char("#gYou know none of the Zi techniques.\n\r",ch);
	    if (ch->gifts[AZI] > 0) 
		send_to_char("#pConcentration   #g-                  Improved hit rate. (Auto)\n\r",ch);
	    if (ch->gifts[AZI] > 2) 
		send_to_char("#pThrow           #g-                  Throw a weapon at your victim. (Auto)\n\r",ch);
	    if (ch->gifts[AZI] > 3) 
		send_to_char("#pFlashGrenade    #g-Flash Grenade-    Stuns everyone in the vicinity.\n\r",ch);
	    if (ch->gifts[AZI] > 4) 
		send_to_char("#pMitsukeru       #g-                  An advanced scrying technique.\n\r",ch);
	    if (ch->gifts[AZI] > 5)
		send_to_char("#pHand Dance      #g-                  You skillfully use your techniques to strike at openings. (Auto)\n\r", ch);
	    if (ch->gifts[AZI] > 6)
		send_to_char("#pDagger Spiral   #g-                  A surprise attack used with your dagger. (Auto)\n\r", ch );
          if (ch->gifts[AZI] > 7)
		send_to_char("#pPiercingDasher  #g-Piercing Dasher-  A quick multi hit attack to the victim's chest.\n\r", ch );
          if (ch->gifts[AZI] > 8)
		send_to_char("#pVenomous Strike #g-                  Your attacks poison the victim. (Auto)\n\r", ch );
	    return;
        }
        else do_study(ch,"");
    }
  
}

void do_hand_circua( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int count = 1;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
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

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( !is_safe(ch, vch) )
	     {
              switch( count )
              {
                 case 1: act("You dash towards $N raising your palm into $S chin.",ch,NULL,vch,TO_CHAR);
                         act("$n dashes towards you raising $s palm into your chin.",ch,NULL,vch,TO_VICT);
                         act("$n dashes towards $N raising $s palm into $S chin.",ch,NULL,vch,TO_NOTVICT);
                         act("You suddenly push your hand into $N's throat!",ch,NULL,vch,TO_CHAR);
                         act("$n suddenly pushes $s hand into your throat!",ch,NULL,vch,TO_VICT);
                         act("$n suddenly pushes $s hand into $N's throat!",ch,NULL,vch,TO_NOTVICT);
                         ch->attack_type = ATTACK_PHYSICAL;
                         ch->attack_var = PIERCE;
                         ch->attack_focus = DIRECT;
                         break;
                 case 2: act("You twirl onto your feet flipping towards $N.",ch,NULL,vch,TO_CHAR);
                         act("$n twirls onto $s feet flipping towards you.",ch,NULL,vch,TO_VICT);
                         act("$n twirls onto $s feet flipping towards $N.",ch,NULL,vch,TO_NOTVICT);
                         act("You suddenly break $N's neck with a StAgGeRiNg blow to the face!",ch,NULL,vch,TO_CHAR);
                         act("$n suddenly breaks your neck with a StAgGeRiNg blow to the face!",ch,NULL,vch,TO_VICT);
                         act("$n suddenly breaks $N's neck with a StAgGeRiNg blow to the face!",ch,NULL,vch,TO_NOTVICT);
                         ch->attack_type = ATTACK_PHYSICAL;
                         ch->attack_var = HIT;
                         ch->attack_focus = OVERHEAD;
                         break;
                 case 3: act("Rolling onto your feet, you swirl around $N unexpectedly.",ch,NULL,vch,TO_CHAR);
                         act("Rolling onto $s feet, $n swirls around you unexpectedly.",ch,NULL,vch,TO_VICT);
                         act("Rolling onto $s feet, $n swirls around $N unexpectedly.",ch,NULL,vch,TO_NOTVICT);
                         act("You mutter '#yShora Tek#n' as you give $N a spinning uppercut.",ch,NULL,vch,TO_CHAR);
                         act("$n mutters '#yShora Tek#n' as $e gives you a spinning uppercut.",ch,NULL,vch,TO_VICT);
                         act("$n mutters '#yShora Tek#n' as $e gives $N a spinning uppercut.",ch,NULL,vch,TO_NOTVICT);
                         act("$N's body bursts into flames as you land back onto your feet.",ch,NULL,vch,TO_CHAR);
                         act("Your body burst into flames as $n lands back onto $s feet.",ch,NULL,vch,TO_VICT);
                         act("$N's body bursts into flames as $n lands back onto $s feet.",ch,NULL,vch,TO_NOTVICT);
                         ch->attack_type = ATTACK_PHYSICAL;
                         ch->ele_attack = ATTACK_FIRE;
                         ch->attack_var = PIERCE;
                         ch->attack_focus = UPPERCUT;
                         break;
                 case 4: act("You brace yourself as your hands start to glow with blinding energy.",ch,NULL,NULL,TO_CHAR);
                         act("$n braces $mself as $s hands starts to glow with blinding energy.",ch,NULL,NULL,TO_ROOM);
                         act("You release a blast of energy InCERnINaTIng $N into ashes!",ch,NULL,vch,TO_CHAR);
                         act("$n releases a blast of energy InCERnINaTIng you into ashes!",ch,NULL,vch,TO_VICT);
                         act("$n releases a blast of energy InCERnINaTIng $N into ashes!",ch,NULL,vch,TO_NOTVICT);
                         ch->attack_type = ATTACK_PHYSICAL;
                         ch->ele_attack = ATTACK_THUNDER;
                         ch->attack_var = BLAST;
                         ch->attack_focus = DIRECT;
                         break;
              }

              count++;
              if ( count == 5 )
                 count = 1;

              if ( !IS_NPC( vch ) || ( IS_NPC( vch ) && vch->pIndexData->vnum >= 550 && vch->pIndexData->vnum < 571 ) )
                 {
                    if ( !IS_NPC( vch ) && number_percent() > 94 - devil_luck( ch, vch, 5 ) && ch->ctimer[15] == 0
                         && !IS_SET( vch->cmbt[5], SKL2_AMIND ) )
                       {
                          stop_fighting( vch, TRUE );
                          vch->hit = -2000010;
                          vch->position = POS_MORTAL;
                       }
                       else hurt_person( ch, vch, 3500000, TRUE );
                 }
                 else {
                         vch->hit = -2000010;
                         hurt_person( ch, vch, 1500000, TRUE );
                      }

              stc("\n\r",ch);
	     }
    }
}

void do_piercing_dasher( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[AZI] < 8 )
     {
        send_to_char("You have to improve your Zi fighting style to the 8th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != AZI )
     {
        send_to_char("You aren't concentrating on the Zi fighting style.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whom do you wish to strike?\n\r",ch);
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

  if ( ch->move < 500000 )
     {
        stc("You are too tired to execute this technique.\n\r",ch);
        return;
     }

  if ( is_safe(ch,victim) ) return;

  ch->move -= 500000;

  act("You quickly dash towards $N as you pull out a dagger and ThRuSt it through $S chest.",ch,NULL,victim,TO_CHAR);
  act("$n quickly dashes towards you as $e pulls out a dagger and ThRuStS it through your chest.",ch,NULL,victim,TO_VICT);
  act("$n quickly dashes towards $N as $e pulls out a dagger and ThRuStS it through $S chest.",ch,NULL,victim,TO_NOTVICT);
  act("You then unsheath your other dagger as you RiP it Up through $N's body.",ch,NULL,victim,TO_CHAR);
  act("$n then unsheath $s other dagger as $e RiPs it Up through your body.",ch,NULL,victim,TO_VICT);
  act("$n then unsheath $s other dagger as $e RiPs it Up through $N's body.",ch,NULL,victim,TO_NOTVICT);
  act("As $N staggers back from the blow, you SmAsH $S head away with a backflip kick.",ch,NULL,victim,TO_CHAR);
  act("As you staggers back from the blow, $n SmAsHeS your head away with a backflip kick.",ch,NULL,victim,TO_VICT);
  act("As $N staggers back from the blow, $n SmAsHeS $S head away with a backflip kick.",ch,NULL,victim,TO_NOTVICT);

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = DIRECT;

  ch->wait = 18;
  hurt_person( ch, victim, 1000000, TRUE );

  act("You silently land back on your feet, sheathing your daggers with a small smirk.",ch,NULL,victim,TO_CHAR);
  act("$n silently lands back on $s feet, sheathing $s daggers with a small smirk.",ch,NULL,victim,TO_ROOM);
}

void do_dark_matter( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[AMI] < 9 )
     {
        send_to_char("You have to improve your Mi fighting style to the 9th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != AMI )
     {
        send_to_char("You aren't concentrating on the Mi fighting style.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to cast Dark Matter on?\n\r",ch);
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

  if ( ch->mana < 500000 )
     {
        stc("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  if ( is_safe(ch,victim) ) return;

  ch->mana -= 500000;

  act("You slide your right hand across your body as you quickly raise a finger on your left one.",ch,NULL,NULL,TO_CHAR);
  act("$n slides $s right hand across $s body as $e quickly raises a finger on $s left one.",ch,NULL,NULL,TO_ROOM);

  act("Mumbling the ancient language, you form a circle of chaotic energy with your left hand.",ch,NULL,NULL,TO_CHAR);
  act("Mumbling the ancient language, $n forms a circle of chaotic energy with $s left hand.",ch,NULL,NULL,TO_ROOM);

  act("A look of severe concentration fills your face as you refocus the dark energy.",ch,NULL,NULL,TO_CHAR);
  act("A look of severe concentration fills $n's face as $e refocus the energy surrounding $m.",ch,NULL,NULL,TO_ROOM);

  act("With your hair blowing in the wind, you point your hand at $N with absolute hatred.",ch,NULL,victim,TO_CHAR);
  act("With $s hair blowing in the wind, $n points $s hand at you with absolute hatred.",ch,NULL,victim,TO_VICT);
  act("With $s hair blowing in the wind, $n points $s hand at $N with absolute hatred.",ch,NULL,victim,TO_NOTVICT);

  act("A small black triangle fades into existence as it rotates in a circle.",ch,NULL,NULL,TO_CHAR);
  act("A small black triangle fades into existence as it rotates in a circle.",ch,NULL,NULL,TO_ROOM);
  act("As it grows bigger, $n screams as $e is pulled violently in it.",victim,NULL,NULL,TO_ROOM);
  act("As it grows bigger, you scream as you are pulled violently in it.",victim,NULL,NULL,TO_CHAR);
  act("Flashes of energy appear inside of the triangle as $n is pushed out of it.",victim,NULL,NULL,TO_ROOM);
  act("Your body is stretched begun proportion as a black hole rips you apart!",victim,NULL,NULL,TO_CHAR);
  act("Flashes of energy appear inside of the triangle as you are pushed out of it.",victim,NULL,NULL,TO_CHAR);

  ch->wait = 18;
  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_DARK;
  ch->attack_var = BLAST;
  ch->attack_focus = AERIAL;

  hurt_person( ch, victim, 1000000, TRUE );
}

void do_luminaire( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim, *victim_next;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  act("You slide your right hand across your body as you quickly raise a finger on your left one.",ch,NULL,NULL,TO_CHAR);
  act("$n slides $s right hand across $s body as $e quickly raises a finger on $s left one.",ch,NULL,NULL,TO_ROOM);

  act("Mumbling the ancient language, you form a circle of chaotic energy with your left hand.",ch,NULL,NULL,TO_CHAR);
  act("Mumbling the ancient language, $n forms a circle of chaotic energy with $s left hand.",ch,NULL,NULL,TO_ROOM);

  act("A look of severe concentration fills your face as you refocus the dark energy.",ch,NULL,NULL,TO_CHAR);
  act("A look of severe concentration fills $n's face as $e refocus the energy surrounding $m.",ch,NULL,NULL,TO_ROOM);

  act("With your hair blowing in the wind, you point your hand at the centre of the room with absolute hatred.",ch,NULL,NULL,TO_CHAR);
  act("With $s hair blowing in the wind, $n points $s hand at the centre of the room with absolute hatred.",ch,NULL,NULL,TO_ROOM);

  act("Sparkles of electricity start zapping as it blinks all around you.",ch,NULL,NULL,TO_CHAR);
  act("Sparkles of electricity start zapping as it blinks all around you.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, an invisible force begins to draw all of the static electricity towards the centre of the room.",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, an invisible force begins to draw all of the static electricity towards the centre of the room.",ch,NULL,NULL,TO_ROOM);
  act("As more and more electricity is drawn, a huge mass of energy forms into existence.",ch,NULL,NULL,TO_CHAR);
  act("As more and more electricity is drawn, a huge mass of energy forms into existence.",ch,NULL,NULL,TO_ROOM);
  act("The room TrEmBlEs ViOlEnTlY aS tHe ball of energy eXpLoDeS!!!",ch,NULL,NULL,TO_CHAR);
  act("The room TrEmBlEs ViOlEnTlY aS tHe ball of energy eXpLoDeS!!!",ch,NULL,NULL,TO_ROOM);

  for ( victim = ch->in_room->people; victim != NULL; victim = victim_next )
  {
     victim_next = victim->next_in_room;

     if ( is_safe(ch,victim) )
        continue;

     stc("The mass of electricity ElEcTricUtEs Your Body!!!\n\r",victim);
     act("The mass of electricity ElEcTricUtEs $n's Body!!!",victim,NULL,NULL,TO_ROOM);

     if ( ch->ctimer[15] == 0 && !is_martial( victim ) )
        victim->wait += number_range( 12, 24 );

     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_THUNDER;
     ch->attack_var = BLAST;
     ch->attack_focus = AERIAL;
     hurt_person( ch, victim, 2000000, TRUE );
  }
}

void do_mi_drain( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[AMI] < 7 )
     {
        send_to_char("You have to improve your Mi fighting style to the 7th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != AMI )
     {
        send_to_char("You aren't concentrating on the Mi fighting style.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whose life do you wish to drain?\n\r",ch);
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

  if ( ch->mana < 750000 )
     {
        stc("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  if ( is_safe( ch, victim ) ) 
     return;

  ch->mana -= 750000;

  act("You point a finger towards $N as you focus your hatred towards $M.",ch,NULL,victim,TO_CHAR);
  act("$n points a finger towards you as $e focuses $s hatred towards you.",ch,NULL,victim,TO_VICT);
  act("$n points a finger towards $N as $e focuses $s hatred towards $M.",ch,NULL,victim,TO_NOTVICT);
  act("Your body spasm as part of your life force is ripped from you!",victim,NULL,NULL,TO_CHAR);
  act("$n's body spasm as part of $s life force is ripped from $m!",victim,NULL,NULL,TO_ROOM);
  hit_gain( ch, 250000 );

  ch->wait = 18;
  ch->attack_type = ATTACK_MAGICAL;
  ch->ele_attack = ATTACK_DARK;
  ch->attack_var = SOUL;
  ch->attack_focus = DIRECT;
  hurt_person( ch, victim, 850000, TRUE );
}

void do_possess( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  int temp, chance;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[AMI] < 10 )
     {
        send_to_char("You have to improve your Mi fighting style to the 10th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != AMI )
     {
        send_to_char("You aren't concentrating on the Mi fighting style.\n\r",ch);
        return;
     }

  if ( ch->position > POS_MORTAL )
     {
        send_to_char("But you aren't mortally wounded... and still have a chance.\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     {
        stc("Whose life do you wish to take?\n\r",ch);
        return;
     }
    
  if ( ( victim = get_char_area( ch, arg ) ) == NULL )
     {
        send_to_char( "They aren't here.\n\r", ch );
        return;
     }

  if ( is_safe( ch, victim ) ) 
     return;

  if ( victim->in_room == ch->in_room )
     {
        if ( IS_NPC( victim ) )
           chance = 50;
           else chance = 80;
     }
     else {
             if ( IS_NPC( victim ) )
                chance = 60;
                else chance = 80;
          }

  if ( ch->ctimer[15] > 0 || number_percent() < chance )
     {
        ch->position = POS_STANDING;
        act( "You fail miserably in the attempt to take over $N's mind.", ch, NULL, victim, TO_CHAR );
        ch->position = POS_MORTAL;
        ch->wait = 18;
        return;
     }

  ch->position = POS_STANDING;

  if ( ch->in_room == victim->in_room )
     {
        act( "As you are about to decapitate $N's head, $E suddenly gets up and grabs your neck.", victim, NULL, ch, TO_CHAR );
        act( "As $n is about to decapitate your head, you suddenly get up and grab $s neck.", victim, NULL, ch, TO_VICT );
        act( "As $n is about to decapitate $N's head, $E suddenly gets up and grabs $s neck.", victim, NULL, ch, TO_NOTVICT );
     }
     else {
             act( "You scream in pain as you feel a presence taking over your mind.", victim, NULL, NULL, TO_CHAR );
             act( "$n screams in pain as $e feels a presence taking over $s mind.", victim, NULL, NULL, TO_ROOM );
             act( "You cackle gleefully as you take over $N's body.", ch, NULL, victim, TO_CHAR );
          }

  act("You snarl '#yRemember the name $t...#n' as the room goes bright!!!", ch, ch->pcdata->switchname, NULL, TO_CHAR );
  act("$n snarls '#yRemember the name $t...#n' as the room goes bright!!!", ch, ch->pcdata->switchname, NULL, TO_ROOM );

  victim->position = POS_MORTAL;
  temp = ch->hit;
  ch->hit = victim->hit;
  victim->hit = temp;
  ch->wait = 36;

  temp = ch->mana;
  ch->mana = victim->mana;
  victim->mana = temp;

  temp = ch->move;
  ch->move = victim->move;
  victim->move = temp;

  if ( ch->in_room != victim->in_room )
     {
        location = ch->in_room;
        char_from_room( ch );
        char_to_room( ch, victim->in_room );
        char_from_room( victim );
        char_to_room( victim, location );       
     }

  clear_affects( ch );
}

void do_tri_heal( CHAR_DATA *ch, char *argument )
{  
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[AMI] < 2 )
     {
        send_to_char("You have to improve your Mi fighting style to the 2nd Level.\n\r",ch);
        return;
     }

  act("You raise your arms in the air as you mutter '#yNiz Toda Geraz...#n'",ch,NULL,NULL,TO_CHAR);
  act("$n raises $s arms in the air as $e mutters '#yNiz Toda Geraz...#n'",ch,NULL,NULL,TO_ROOM);
  act("Your body explodes with kiotic energy as it flows through your blood.",ch,NULL,NULL,TO_CHAR);
  act("$n's body explodes with kiotic energy as it flows through $s blood.",ch,NULL,NULL,TO_ROOM);

  hit_gain( ch, 300000 );

  if ( ch->clannum == 1 )
     {
        mana_gain( ch, 300000 );
        move_gain( ch, 300000 );
     }
     else {
             mana_gain( ch, 600000 );
             move_gain( ch, 600000 );
          }

  ch->wait = 12;
}

void do_flash_grenade(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *rch;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[AZI] < 4 )
       {
          send_to_char("You have to improve your Zi fighting style to the 4th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AZI )
       {
          send_to_char("You aren't concentrating on the Zi fighting style.\n\r",ch);
          return;
       }

    if ( ch->fight_timer > 0 )
       {
          stc("You had been spotted and lost the element of surprise.\n\r",ch);
          return;
       }

    if ( ch->move < 1000000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    act("$n concentrate furiously on $s hatred as $e quickly raises $s hand into the air.",ch,NULL,NULL,TO_ROOM);
    act("The room is suddenly filled with blazing energy.",ch,NULL,NULL,TO_ROOM);
    act("You concentrate furiously on your hatred as you quickly raise your hand into the air.",ch,NULL,NULL,TO_CHAR);
    act("The room is suddenly filled with blazing energy.",ch,NULL,NULL,TO_CHAR);
    ch->move -= 1000000;

    for(rch=ch->in_room->people;rch != NULL;rch=rch->next_in_room)
    {
       if ( rch->fighting != NULL )
          stop_fighting(rch,TRUE);

       if ( IS_CLASS(rch, CLASS_DROW) && IS_SET(rch->newbits, NEW_DARKNESS) )
          {
             REMOVE_BIT(rch->newbits, NEW_DARKNESS);
             rch->in_room->drows--;
          }

       if ( is_safe(ch, rch) )
          continue;

       if ( !IS_SET(rch->more, MORE_BLIND) )
          {
             SET_BIT(rch->more, MORE_BLIND);
             rch->more = 4;
          }

       act("You scream as the explosion sears your eyes.",rch,NULL,NULL,TO_CHAR);
       act("$N is stunned from the huge explosion as $E crashes into the ground.",ch,NULL,rch,TO_NOTVICT);
       act("$N is stunned from the huge explosion as $E crashes into the ground.",ch,NULL,rch,TO_CHAR);
       rch->position = POS_STUNNED; 
    }

    ch->fight_timer = 20;

    if ( IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
       REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
}

void do_dispose( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int dam, olddam;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[ALI] < 9 )
       {
          send_to_char("You have to improve your Li fighting style to the 9th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != ALI )
       {
          send_to_char("You aren't concentrating on the Li fighting style.\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
    {
         send_to_char( "Dispose of whom?\n\r", ch );
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

    if ( ch->fighting != NULL || ch->fight_timer > 0 )
       dam = 700000;
       else dam = 2000000;

    olddam = dam;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = DIRECT;

    dam = is_attack_type( ch, victim, dam );

    sprintf(buf,"You fade from the shadows thrusting your dagger into $N's back! [#r#z%d#n]", dam);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n fades from the shadows thrusting $s dagger into your back! [#r#z%d#n]", dam);
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"$n fades from the shadows thrusting $s dagger into $N's back! [#r#z%d#n]", dam);
    act(buf,ch,NULL,victim,TO_NOTVICT);

    dam = olddam;

    hurt_person( ch, victim, dam, TRUE );
}

void do_assassin_vanish( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[ALI] < 4 )
       {
          send_to_char("You have to improve your Li fighting style to the 4th Level.\n\r",ch);
          return;
       }

    if ( ch->fight_timer > 0 )  
       {
          send_to_char("Not until your fight timer expires.\n\r", ch );
          return;
       }	

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
       {
	    REMOVE_BIT(ch->act, PLR_WIZINVIS);
	    send_to_char( "You fade into existence as the shadows release its hold on you.\n\r", ch );
  	    act( "$n fades into existence as the shadows release its hold on $m.", ch, NULL, NULL, TO_ROOM );
       }
       else {
	         act( "You concentrate softly as the shadows wrap itself amongst you.", ch, NULL, NULL, TO_CHAR );
	         act( "$n concentrates softly as he fades out of existence.", ch, NULL, NULL, TO_ROOM );
	         SET_BIT(ch->act, PLR_WIZINVIS);
            }
}

void do_disguise( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 

    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) 
      return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[ALI] < 3 )
       {
          send_to_char("You have to improve your Li fighting style to the 3rd Level.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "#RSyntax - #eDisguise Name.#n\n\r", ch );
          return;
       }

    if ( ch->fight_timer > 0 )
       {
          stc("Not with a fight timer.\n\r",ch);
          return;
       }

     if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
        {
           if ( argument[0] == '\0' )
              {
                  send_to_char( "#RSyntax - #eDisguise Name.#n\n\r", ch );
                  return;
              }

           if ( strlen( argument ) < 3 )
              {
                 stc( "Your disguise has to have three letters at least.\n\r", ch );
                 return;
              }

           free_string( ch->morph );
           ch->morph = str_dup( argument );
           SET_BIT(ch->affected_by, AFF_POLYMORPH);
           act("You reform yourself into a new image...",ch,NULL,NULL,TO_CHAR);
           return;
        }

     free_string( ch->morph );
     ch->morph = str_dup( "" );
     REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
     act("You form back to your normal image...",ch,NULL,NULL,TO_CHAR);
}

void do_fade( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int cost = 4000000;

    if ( IS_CLASS(ch, CLASS_MONK) )
       {
          do_monk_fade( ch, "" );
          return;
       }

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[ALI] < 8 )
       {
          send_to_char("You have to improve your Li fighting style to the 8th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != ALI )
       {
          send_to_char("You aren't concentrating on the Li fighting style.\n\r",ch);
          return;
       }

    if ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 )
       {
          send_to_char( "Not in the arena.\n\r", ch );
          return;
       }

    if ( ch->pcdata->kingdom == 1 && ch->in_room->vnum >= 2500 && ch->in_room->vnum < 3000 )
       {
          stc("You don't see a need of fading away within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 2 && ch->in_room->vnum >= 3500 && ch->in_room->vnum < 4000 )
       {
          stc("You don't see a need of fading away within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 3 && ch->in_room->vnum >= 4500 && ch->in_room->vnum < 5000 )
       {
          stc("You don't see a need of fading away within your own base.\n\r",ch);
          return;
       }

    if ( ch->pcdata->kingdom == 4 && ch->in_room->vnum >= 16500 && ch->in_room->vnum < 17000 )
       {
          stc("You don't see a need of fading away within your own base.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char("It is #c#zimpossible#n to flee from within this #Lco#glo#Lur#gful #evortex#n.\n\r",ch);
          return;
       }

    if ( ch->move < cost )
       {
          send_to_char( "You are too tired to do this.\n\r", ch );
          return;
       }

    ch->move -= cost;
    stop_fighting( ch, TRUE );
   
    send_to_char( "You step into the shadows as it wraps its protective cloak aroud you...\n\r", ch );
    act( "$n vanishes in the shadows as it appear to wrap itself around $m.", ch, NULL, NULL, TO_ROOM );
    get_random_room( ch );
    do_look( ch, "auto" );
}

void do_assassin_guided( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 2 )
     {
        send_to_char("You have to improve your Chi fighting style to the 2nd Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to release an energy blast into?\n\r",ch);
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

  if ( ch->focus[CURRENT] < 10 )
     {
        send_to_char( "You don't have enough life force for that.\n\r", ch );
        return;
     }

  if ( is_safe(ch,victim) ) return;

  ch->focus[CURRENT] -= 10;
  ch->wait = 18;

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("You release a blast of energy towards $N!",ch,NULL,victim,TO_CHAR);
  act("$n releases a blast of energy towards you!",ch,NULL,victim,TO_VICT);
  act("$n releases a blast of energy towards $N!",ch,NULL,victim,TO_NOTVICT);
  act("You got SlAmMeD into the ground as the energy blast SOARS through you!!",victim,NULL,NULL,TO_CHAR);
  act("$n got SlAmMeD into the ground as the energy blast SOARS through $m!!",victim,NULL,NULL,TO_ROOM);

  if ( number_percent() > 93 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
     {
        act("You are stunned.",victim,NULL,NULL,TO_CHAR);
        victim->wait = number_range( 12, 18 );
     }

  ch->ele_attack = ATTACK_THUNDER;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = BLAST;
  ch->attack_focus = DIRECT;
  hurt_person( ch, victim, 1000000, TRUE );
}

void do_inner_healing( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 3 )
     {
        send_to_char("You have to improve your Chi fighting style to the 3rd Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( ch->focus[CURRENT] < 10 )
     {
         send_to_char( "You don't have enough life force for that.\n\r", ch );
         return;
     }

  ch->focus[CURRENT] -= 10;
  ch->agg = UMAX( 0, ch->agg - 10 );

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("A beam of energy descends from the heavens engulfing you with its brillance.",ch,NULL,NULL,TO_CHAR);
  act("A beam of energy descends from the heavens engulfing $n with its brillance.",ch,NULL,NULL,TO_ROOM);
  act("Swirals of palish waves soothes your body as it sparks and fizzes.",ch,NULL,NULL,TO_CHAR);
  act("Swirals of palish waves soothes $n's body as it sparks and fizzes.",ch,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  clear_affects( ch );

  hit_gain( ch, 400000 );

  if ( number_percent() > 30 )
     SET_BIT( ch->monkstuff, MONK_HEAL );
}

void do_inner_rage( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 4 )
     {
        send_to_char("You have to improve your Chi fighting style to the 4th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( IS_SET(ch->more3, MORE3_RAGE) )
     {
        REMOVE_BIT(ch->more3, MORE3_RAGE);
        REMOVE_BIT(ch->itemaffect, ITEMA_FIREWALL);
        act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
        act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
        act("The demonic energy flowing through you fades away as you lower your concentration.",ch,NULL,NULL,TO_CHAR);
        act("The demonic energy flowing through $n fades away as $e lowers $s concentration.",ch,NULL,NULL,TO_ROOM);
        return;
     }

  if ( ch->focus[CURRENT] < 10 )
     {
        send_to_char( "You don't have enough life force for that.\n\r", ch );
        return;
     }

  ch->focus[CURRENT] -= 10;

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("Your body is engulfed by a massive wall of fire as your eyes blazes with demonic insanity.",ch,NULL,NULL,TO_CHAR);
  act("$n's body is engulfed by a massive wall of fire as $s eyes blazes with demonic insanity.",ch,NULL,NULL,TO_ROOM);

  SET_BIT(ch->more3, MORE3_RAGE);
  SET_BIT(ch->itemaffect, ITEMA_FIREWALL);
}

void do_yang_power( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 5 )
     {
        send_to_char("You have to improve your Chi fighting style to the 5th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( IS_SET(ch->more3, MORE3_YANG) )
     {
        REMOVE_BIT(ch->more3, MORE3_YANG);
        act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
        act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
        act("The shimmering energy engulfing your body slowly fades away.",ch,NULL,NULL,TO_CHAR);
        act("The shimmering energy engulfing $n's body slowly fades away.",ch,NULL,NULL,TO_ROOM);
        return;
     }

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("Transparent symbols encircles your body as it is engulfed with shimmering energy.",ch,NULL,NULL,TO_CHAR);
  act("Transparent symbols encircles $n's body as it is engulfed with shimmering energy.",ch,NULL,NULL,TO_ROOM);

  SET_BIT(ch->more3, MORE3_YANG);

  if ( IS_SET(ch->more3, MORE3_YIN) )
     REMOVE_BIT(ch->more3, MORE3_YIN);
}

void do_yin_power( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 6 )
     {
        send_to_char("You have to improve your Chi fighting style to the 6th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( IS_SET(ch->more3, MORE3_YIN) )
     {
        REMOVE_BIT(ch->more3, MORE3_YIN);
        act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
        act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
        act("The instence energy engulfing your body slowly fades away.",ch,NULL,NULL,TO_CHAR);
        act("The instence energy engulfing $n's body slowly fades away.",ch,NULL,NULL,TO_ROOM);
        return;
     }

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("The ground beneath you shakes violently as you are engulfed with instence energy.",ch,NULL,NULL,TO_CHAR);
  act("The ground beneath $n's shakes violently as $e is engulfed with instence energy.",ch,NULL,NULL,TO_ROOM);

  SET_BIT(ch->more3, MORE3_YIN);

  if ( IS_SET(ch->more3, MORE3_YANG) )
     REMOVE_BIT(ch->more3, MORE3_YANG);
}

void do_assassin_blast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 10 )
     {
        send_to_char("You have to improve your Chi fighting style to the 10th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to release an energy blast into?\n\r",ch);
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

  if ( ch->focus[CURRENT] < 70 )
     {
         send_to_char( "You don't have enough life force for that.\n\r", ch );
         return;
     }

  if ( is_safe(ch,victim) ) return;

  ch->focus[CURRENT] -= 70;

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("A bolt of energy manifests into your hands as you concentrate harder!",ch,NULL,NULL,TO_CHAR);
  act("A bolt of energy manifests into $n's hands as $e concentrates harder!",ch,NULL,NULL,TO_ROOM);
  act("You mutter '#yDinaten Shata Outa...#n' as the energy ball humms violently with demonic power.",ch,NULL,NULL,TO_CHAR);
  act("$n mutters '#yDinaten Shata Outa...#n' as the energy ball humms violently with demonic power.",ch,NULL,NULL,TO_ROOM);
  act("You release a HuGe blast of energy towards $N!",ch,NULL,victim,TO_VICT);
  act("$n releases a HuGe blast of energy towards you!",ch,NULL,victim,TO_VICT);
  act("$n releases a HuGe blast of energy towards $N!",ch,NULL,victim,TO_NOTVICT);
  act("You got SlAmMeD into the ground as the energy blast SOARS through you!!",victim,NULL,NULL,TO_CHAR);
  act("$n got SlAmMeD into the ground as the energy blast SOARS through $m!!",victim,NULL,NULL,TO_ROOM);
  act("You are stunned.",victim,NULL,NULL,TO_CHAR);

  if ( !is_martial( victim ) )
     victim->wait = number_range( 18, 24 );

  ch->ele_attack = ATTACK_THUNDER;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = BLAST;
  ch->attack_focus = DIRECT;

  ch->wait = 12;
  hurt_person( ch, victim, 1500000, TRUE );
}

void do_inner_shield( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[ACHI] < 10 )
       {
          send_to_char("You have to improve your Chi fighting style to the 10th Level.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more2, MORE2_SHIELD) )
       {
          act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
          act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
          act("You relax the chi energy protecting your body.",ch,NULL,NULL,TO_CHAR);
          act("$n looks more relax as $e releases the bluish energy arround $s body.",ch,NULL,NULL,TO_ROOM);
          REMOVE_BIT(ch->more2, MORE2_SHIELD);
          return;
       }

    act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
    act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
    act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
    act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
    act("Two circles of palish energy encircles your body rapidly as it forms into you.",ch,NULL,NULL,TO_CHAR);
    act("Two circles of palish energy encircles $n's body rapidly as it forms into $m.",ch,NULL,NULL,TO_ROOM);
    act("Your body explodes with positive energy as dazzling light soars through the room.",ch,NULL,NULL,TO_CHAR);
    act("$n's body explodes with positive energy as dazzling light soars through the room.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->more2, MORE2_SHIELD);
    ch->wait = 12;
}

void do_inner_fury( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[ACHI] < 9 )
     {
        send_to_char("You have to improve your Chi fighting style to the 9th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != ACHI )
     {
        send_to_char("You aren't concentrating on the Chi fighting style.\n\r",ch);
        return;
     }

  if ( !IS_SET(ch->more3, MORE3_RAGE) )
     {
        send_to_char("You aren't focusing on your inner rage.\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Who do you wish to wreck your rage onto?\n\r",ch);
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

  if ( ch->focus[CURRENT] < 25 )
     {
         send_to_char( "You don't have enough life force for that.\n\r", ch );
         return;
     }

  if ( is_safe(ch,victim) ) return;

  ch->focus[CURRENT] -= 25;

  act("You quickly fold your hands together as you bow your head in prayer.",ch,NULL,NULL,TO_CHAR);
  act("$n quickly folds $s hands together as $e bows $s head in prayer.",ch,NULL,NULL,TO_ROOM);
  act("Your body flares with bluish positive energy as you focus on your lifeforce.",ch,NULL,NULL,TO_CHAR);
  act("$n's body flares with bluish positive energy as $e focuses on $s lifeforce.",ch,NULL,NULL,TO_ROOM);
  act("You yell '#yNia Dos Strikas!#n' as your hands charged up with unbelievable energy.",ch,NULL,NULL,TO_CHAR);
  act("$n yells '#yNia Dos Strikas!#n' as $s hands charged up with unbelievable energy.",ch,NULL,NULL,TO_ROOM);
  act("You rush to $N with a demonic cackle pummelling $M to the ground.",ch,NULL,victim,TO_CHAR);
  act("$n rushes to you with a demonic cackle pummelling you to the ground.",ch,NULL,victim,TO_VICT);
  act("$n rushes to $N with a demonic cackle pummelling $M to the ground.",ch,NULL,victim,TO_NOTVICT);

  multi_hit( ch, victim, TYPE_UNDEFINED );
  multi_hit( ch, victim, TYPE_UNDEFINED );
  ch->wait = 12;
}

void do_assassin_focus( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_SHADOW) && !IS_CLASS(ch, CLASS_SHADOW_WAR) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[BODY] < 5 )
     {
        stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
        return;
     }

  if ( ( victim = get_char_room( ch, arg) ) == NULL )
     {
        stc("They aren't here.\n\r",ch);
        return;
     }

  if ( ch->fighting == NULL )
     {
        stc("You must be fighting to focus your attention on another target.\n\r",ch);
        return;
     }

  if ( ch->fighting == victim )
     {
        stc("You are already fighting them.\n\r",ch);
        return;
     }

  act("You roll onto your feet subtlely dodging an attack from $N.",ch,NULL,victim,TO_CHAR);
  act("$n rolls onto $s feet subtlely dodging an attack from you.",ch,NULL,victim,TO_VICT);
  act("$n rolls onto $s feet subtlely dodging an attack from $N.",ch,NULL,victim,TO_NOTVICT);

  act("You yell '#yZidora Nuden!!#n' as you SmAsH your palm into $N's chest.",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yZidora Nuden!!#n' as $e SmAsHeS $s palm into your chest.",ch,NULL,victim,TO_VICT);
  act("$n yells '#yZidora Nuden!!#n' as $e SmAsHeS $s palm into $N's chest.",ch,NULL,victim,TO_NOTVICT);
  act("You fall to the ground wobbling as you gasp for breath.",victim,NULL,NULL,TO_CHAR);
  act("$n falls to the ground wobbling as $e gasps for breath.",victim,NULL,NULL,TO_ROOM);

  hurt_person( ch, ch->fighting, 250000, TRUE );
  ch->fighting = NULL;
  ch->position = POS_STANDING;
  ch->wait = 6;
  do_kill( ch, arg );
}

void do_dispel( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int test = 0;
    int test2 = 0;
    AFFECT_DATA *affect_next, *affect;

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( IS_CLASS(ch, CLASS_SHADOW) && ch->gifts[AKI] < 9 )
       {
          send_to_char("You have to improve your Ki fighting style to the 9th Level.\n\r",ch);
          return;
       }

    if ( IS_CLASS(ch, CLASS_SHADOW) && ch->gifts[CURRENT_METER] != AKI )
       {
          send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
          return;
       }

    if ( IS_CLASS( ch, CLASS_DARK_ELF ) && ch->pcdata->powers[DROW_CLASS] != DROW_MAGI )
       {
          stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
          return;
       }

    if ( ch->mana < 300000 )
    {
      stc("You don't have enough magical energy for that.\n\r",ch);
      return;
    }   

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Dispel whom?\n", ch );
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

    if ( is_affected(victim, skill_lookup("gods disfavor") ) )
       {
          send_to_char("God refuses to allow anything to disrupt this curse.\n\r",ch);
	    return;
       }

    if ( is_affected( victim, skill_lookup("blessing of the saints") ) )
       {
          send_to_char("They are protected by the holy saints.\n\r", ch );
          return;
       }

    if ( ch->clannum != 1 && IS_CLASS( victim, CLASS_WIZARD ) )
       {
          send_to_char("They wave away your spell with a gesture from their hand.\n\r", ch );
          return;
       }

    ch->wait = 6;
    ch->mana -= 300000;

    act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
    act("You concentrate on $N's weaknesses as you push your hand towards $M.",ch,NULL,victim,TO_CHAR);
    act("$n concentrates fiercely on $N as $e pushes $s hand towards $M.",ch,NULL,victim,TO_NOTVICT);
    act("$n concentrates fiercely on you as $e pushes $s hand towards you.",ch,NULL,victim,TO_VICT);

    if ( IS_SET(victim->affected_by, AFF_POLYMORPH) )
       test = 1;

    if ( IS_SET(victim->monkstuff, MONK_HEAL) )
       REMOVE_BIT(victim->monkstuff, MONK_HEAL);

    if ( IS_AFFECTED(victim, AFF_CHARM) )
       test2 = 1;

    affect = victim->affected;

    while ( affect )
    {
       affect_next = affect->next;

       if ( affect->duration != -1 )
          affect_remove( victim, affect );

       affect = affect_next;
    }

    act("You remove all magical affects from $N.",ch,NULL,victim,TO_CHAR);
    act("$n has removed all magical affects from $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n has removed all magical affects from you.",ch,NULL,victim,TO_VICT);

    if ( test == 1 )
	 SET_BIT(victim->affected_by, AFF_POLYMORPH);

    if ( test2 == 1 )
       {
          AFFECT_DATA af;

          af.type      = skill_lookup("charm person");
          af.duration  = -1;
          af.location  = APPLY_NONE;
          af.modifier  = 0;
          af.bitvector = AFF_CHARM;
          affect_to_char( victim, &af );
       }
}

void do_painless( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[ALI] < 2 )
       {
          send_to_char("You have to improve your Li fighting style to the 2nd Level.\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( ch->move < 250000 )
       {
          send_to_char("You are too tired to execute this technique.\n\r", ch);
          return;
	 }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Cripple whom?\n", ch );
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

    if ( IS_SET( victim->more, MORE_PAIN ) )
       {
          stc("They are already in pain!\n\r",ch);
          return;
       }

    ch->move -= 250000;
    act("You crouch on your knee as you bend your fingers in an ancient structure.",ch,NULL,NULL,TO_CHAR);
    act("$n crouches on $s knee as $e bends $s fingers in an ancient structure.",ch,NULL,NULL,TO_ROOM);
    act("Concentrating on $N intently, your hands glows with an unearthly energy.",ch,NULL,victim,TO_CHAR);
    act("Concentrating on you intently, $n's hands glows with an unearthly energy.",ch,NULL,victim,TO_VICT);
    act("Concentrating on $N intently, $n's hands glows with an unearthly energy.",ch,NULL,victim,TO_NOTVICT);
    act("You mutter '#ySai...#n' as you reach forward and touch $N.",ch,NULL,victim,TO_CHAR);
    act("$n mutters '#ySai...#n' as $e reaches forward and touches you.",ch,NULL,victim,TO_VICT);
    act("$n mutters '#ySai...#n' as $e reaches forward and touches $N.",ch,NULL,victim,TO_NOTVICT);
    act("You feel weaker as a wave of anxiety floods through you.",victim,NULL,NULL,TO_CHAR);
    act("$n gasps for breath as $e begins to look weary.",victim,NULL,NULL,TO_ROOM);

    if ( !IS_SET( victim->more, MORE_PAIN ) )
       SET_BIT( victim->more, MORE_PAIN );

    ch->wait = 12;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = LOW;
    hurt_person( ch, victim, 700000, TRUE );
}

void do_hand_blast( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if ( IS_CLASS(ch, CLASS_DROW) )
       {
          do_drow_blast( ch, argument );
          return;
       }

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[AKI] < 3 )
       {
          send_to_char("You have to improve your Ki fighting style to the 3rd Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AKI )
       {
          send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Blind whom?\n", ch );
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

    if ( ch->chi[CURRENT] < 5 )
       {
          send_to_char( "You don't have enough kiotic energy for that.\n\r", ch );
          return;
       }

    if ( is_safe(ch,victim) ) return;

    if ( IS_SET(victim->more, MORE_BLIND) )
       {
          stc("Your target is already blinded one way or the other.\n\r",ch);
          return;
       }

    act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
    act("A manifest of energy forms on your right hand as you raise it high above your body.",ch,NULL,NULL,TO_CHAR);
    act("$n's right hand starts glowing brightly as $e raise it high above $s body.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly a blast of energy shoots from $s hand stunning you making you rub your eyes.",ch,NULL,victim,TO_VICT);
    act("Suddenly a blast of energy shoots from $s hand stunning $N causing $M to rubs $S eyes.",ch,NULL,victim,TO_NOTVICT);
    act("Suddenly a blast of energy shoots from your hand stunning $N causing $M to rubs $S eyes.",ch,NULL,victim,TO_CHAR);

    SET_BIT(victim->more, MORE_BLIND);
    victim->blind_timer = 5;
    ch->chi[CURRENT] -= 5;
    stc("The energy blast blinds you.\n\r",victim);
    act("$n is blinded from the kiotic energy blast.",victim,NULL,NULL,TO_ROOM);
}

void do_chin_smite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[AKI] < 4 )
       {
          send_to_char("You have to improve your Ki fighting style to the 4th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AKI )
       {
          send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Stun whom?\n", ch );
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

    if ( ch->chi[CURRENT] < 5 )
       {
          send_to_char( "You don't have enough kiotic energy for that.\n\r", ch );
          return;
       }

    if ( is_safe(ch,victim) ) return;

    act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
    act("You leap towards $N landing on your knees.",ch,NULL,victim,TO_CHAR);
    act("$n leaps towards you landing on $s knees.",ch,NULL,victim,TO_VICT);
    act("$n leaps towards $N landing on $s knees.",ch,NULL,victim,TO_NOTVICT);
    act("You mutter '#yZin nappa...#n' as you whirl your hands in a spiral fashion.",ch,NULL,NULL,TO_CHAR);
    act("$n mutters '#yZin nappa...#n' as $e whirls $s hands in a spiral fashion.",ch,NULL,NULL,TO_ROOM);
    act("You suddenly raise your palm viciously into $N's chin, stunning $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly raises $s palm viciously into your chin, stunning you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly raises $s palm viciously into $N's chin, stunning $M.",ch,NULL,victim,TO_NOTVICT);

    ch->chi[CURRENT] -= 5;
    ch->wait = 12;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = LOW;
    hurt_person( ch, victim, 700000, TRUE );

    if ( victim != NULL && victim->position > POS_STUNNED && number_percent() > 94 - devil_luck( ch, victim, 5 ) )
       victim->position = POS_STUNNED;      
}

void do_leg_dispatcher( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[AKI] < 5 )
       {
          send_to_char("You have to improve your Ki fighting style to the 5th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AKI )
       {
          send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Disable whose legs?\n", ch );
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

    if ( ch->chi[CURRENT] < 20 )
    {
        send_to_char( "You don't have enough kiotic energy for that.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
       stc("You can't use this technique on yourself.\n",ch); 
	 return;
    }

    if ( is_safe(ch,victim) ) return;

    if ( IS_SET(victim->more2, MORE2_NOLEGS) )
    {
       stc("Their movement had already been disabled.\n",ch); 
	 return;
    }

    act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
    act("You leap towards $N landing on your knees.",ch,NULL,victim,TO_CHAR);
    act("$n leaps towards you landing on $s knees.",ch,NULL,victim,TO_VICT);
    act("$n leaps towards $N landing on $s knees.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly catch $N offguard with a swift foot sweep, tripping $M onto the ground.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly catches you offguard with a swift foot sweep, tripping you onto the ground.",ch,NULL,victim,TO_VICT);
    act("$n suddenly catches $N offguard with a swift foot sweep, tripping $M onto the ground.",ch,NULL,victim,TO_NOTVICT);
    act("You yell '#yNis Humpaka!!#n' as you DrIvE your elbow into $N's knees.",ch,NULL,victim,TO_CHAR);
    act("$n yells '#yNis Humpaka!!#n' as $e DrIvEs $s elbow into your knees.",ch,NULL,victim,TO_VICT);
    act("$n yells '#yNis Humpaka!!#n' as $e DrIvEs $s elbow into $N's knees.",ch,NULL,victim,TO_NOTVICT);
    act("You scream loudly from the pain soaring through your legs.",victim,NULL,NULL,TO_CHAR);
    act("$n screams loudly from the pain soaring through $s legs.",victim,NULL,NULL,TO_ROOM);

    ch->chi[CURRENT] -= 20;
    ch->wait = 4;
    SET_BIT(victim->more2, MORE2_NOLEGS);
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = LOW;
    hurt_person(ch,victim,300000,TRUE);
}

void do_arm_grab( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[AKI] < 6 )
       {
          send_to_char("You have to improve your Ki fighting style to the 6th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AKI )
       {
          send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Disable whose arms?\n", ch );
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

    if ( ch->chi[CURRENT] < 20 )
    {
        send_to_char( "You don't have enough kiotic energy for that.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
       stc("You can't use this technique on yourself.\n",ch); 
	 return;
    }

   if ( is_safe(ch,victim) ) return;

    if ( IS_SET(victim->more2, MORE2_NOARMS) )
    {
       stc("Their attacks had already been disabled.\n",ch); 
	 return;
    }

    act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
    act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
    act("You leap towards $N landing on your knees.",ch,NULL,victim,TO_CHAR);
    act("$n leaps towards you landing on $s knees.",ch,NULL,victim,TO_VICT);
    act("$n leaps towards $N landing on $s knees.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly grab $N's arm, breaking it viciously with a LOUD snap.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly grabs your arm, breaking it viciously with a LOUD snap.",ch,NULL,victim,TO_VICT);
    act("$n suddenly grabs $N's arm, breaking it viciously with a LOUD snap.",ch,NULL,victim,TO_NOTVICT);
    act("You scream loudly from the pain soaring through your arms.",victim,NULL,NULL,TO_CHAR);
    act("$n screams loudly from the pain soaring through $s arms.",victim,NULL,NULL,TO_ROOM);

    ch->chi[CURRENT] -= 20;
    ch->wait = 4;
    SET_BIT(victim->more2, MORE2_NOARMS);
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = OVERHEAD;
    hurt_person(ch,victim,300000,TRUE);
}

void do_body_rip( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->gifts[AKI] < 7 )
       {
          send_to_char("You have to improve your Ki fighting style to the 7th Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AKI )
       {
          send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
          return;
       }

    if ( ch->chi[CURRENT] < 50 )
       {
          send_to_char( "You don't have enough kiotic energy for that.\n\r", ch );
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Rip whose body apart?\n", ch );
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

    if ( IS_CLASS( victim, CLASS_PALADIN ) || IS_CLASS( victim, CLASS_SHADOW ) 
         || number_percent() > 65 + devil_luck( ch, victim, 10 ) )
       {
          stc("They barely block your attack with a clash of their weapon.\n",ch); 
          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = HIT;
          ch->attack_focus = DIRECT;
          hurt_person( ch, victim, 1000000, TRUE );
          ch->wait = 18;
          return;
       }

    if ( is_safe(ch,victim) ) return;

    ch->chi[CURRENT] -= 50;

    if ( !IS_ARM_L(victim,LOST_ARM) )
       {
           act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
           act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
           act("You yell '#yPlies Den!!#n' as you dash towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_CHAR);
           act("$n yells '#yPlies Den!!#n' as $e dashes towards you stunning you with an elbow to the face.",ch,NULL,victim,TO_VICT);
           act("$n yells '#yPlies Den!!#n' as $e dashes towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
           act("As $N staggers back from the blow, you RiP $S left arm from $S body viciously.",ch,NULL,victim,TO_CHAR);
           act("As you stagger back from the blow, $n RiPs your left arm from your body viciously.",ch,NULL,victim,TO_VICT);
           act("As $N staggers back from the blow, $n RiPs $S left arm from $S body viciously.",ch,NULL,victim,TO_NOTVICT);

           SET_BIT(victim->loc_hp[2],LOST_ARM);
           SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	     make_part(victim,"arm");
	     if ( IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM) )
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
 	 else if ( !IS_LEG_L(victim,LOST_LEG) )
               {
                   act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
                   act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
                   act("You yell '#yPlies Den!!#n' as you dash towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_CHAR);
                   act("$n yells '#yPlies Den!!#n' as $e dashes towards you stunning you with an elbow to the face.",ch,NULL,victim,TO_VICT);
                   act("$n yells '#yPlies Den!!#n' as $e dashes towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
                   act("As $N staggers back from the blow, you RiP $S left leg from $S body viciously.",ch,NULL,victim,TO_CHAR);
                   act("As you stagger back from the blow, $n RiPs your left leg from your body viciously.",ch,NULL,victim,TO_VICT);
                   act("As $N staggers back from the blow, $n RiPs $S left leg from $S body viciously.",ch,NULL,victim,TO_NOTVICT);

 	             SET_BIT(victim->loc_hp[4],LOST_LEG);
	             SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
	             make_part(victim,"leg");
	             if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	                {
 	                   if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
	                      take_item(victim,obj);
	                }

 	             if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	                take_item(victim,obj);
               }
       else if ( !IS_ARM_R(victim,LOST_ARM) )
               {
                   act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
                   act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
                   act("You yell '#yPlies Den!!#n' as you dash towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_CHAR);
                   act("$n yells '#yPlies Den!!#n' as $e dashes towards you stunning you with an elbow to the face.",ch,NULL,victim,TO_VICT);
                   act("$n yells '#yPlies Den!!#n' as $e dashes towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
                   act("As $N staggers back from the blow, you RiP $S right arm from $S body viciously.",ch,NULL,victim,TO_CHAR);
                   act("As you stagger back from the blow, $n RiPs your right arm from your body viciously.",ch,NULL,victim,TO_VICT);
                   act("As $N staggers back from the blow, $n RiPs $S right arm from $S body viciously.",ch,NULL,victim,TO_NOTVICT);

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
 	 else if ( !IS_LEG_R(victim,LOST_LEG) )
               {
                   act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
                   act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
                   act("You yell '#yPlies Den!!#n' as you dash towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_CHAR);
                   act("$n yells '#yPlies Den!!#n' as $e dashes towards you stunning you with an elbow to the face.",ch,NULL,victim,TO_VICT);
                   act("$n yells '#yPlies Den!!#n' as $e dashes towards $N stunning $M with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
                   act("As $N staggers back from the blow, you RiP $S right leg from $S body viciously.",ch,NULL,victim,TO_CHAR);
                   act("As you stagger back from the blow, $n RiPs your right leg from your body viciously.",ch,NULL,victim,TO_VICT);
                   act("As $N staggers back from the blow, $n RiPs $S right leg from $S body viciously.",ch,NULL,victim,TO_NOTVICT);

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
               else {
                       stc("The victim has no body parts to rip off.\n\r",ch);
                       return;
                    }

   ch->wait = 18;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = LOW;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_ki_wrath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int count = 0;

  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_SHADOW) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->gifts[AKI] < 10 )
     {
        send_to_char("You have to improve your Ki fighting style to the 10th Level.\n\r",ch);
        return;
     }

  if ( ch->gifts[CURRENT_METER] != AKI )
     {
        send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
        return;
     }

  victim = ch->in_room->people;
  for ( ; victim ; victim = victim->next_in_room )
  {
     if ( victim->fighting == ch )
        count++;
  }

  if ( count < 2 )
     {
        stc("You are not fighting enough people at the same time to use this move.\n\r",ch);
        return;
     }

  act("You cackle with glee as your body flares with redish negative energy.",ch,NULL,NULL,TO_CHAR);
  act("$n cackles with glee as $s body flares with redish negative energy.",ch,NULL,NULL,TO_ROOM);
  act("Your body explodes with kiotic energy as you laugh with rage.",ch,NULL,NULL,TO_CHAR);
  act("$n's body explodes with kiotic energy as $e laughs with rage.",ch,NULL,NULL,TO_ROOM);
  act("A blast of kiotic energy sears through the vicinity in a clasp of thunder!",ch,NULL,NULL,TO_CHAR);
  act("A blast of kiotic energy sears through the vicinity in a clasp of thunder!",ch,NULL,NULL,TO_ROOM);

  victim = ch->in_room->people;
  for ( ; victim ; victim = victim->next_in_room )
  {
     if ( ( victim == ch ) || ( victim->fighting != ch ) )
        continue;

     act("The blast of kiotic energy lifts you off your feet and send you flying to the other end of the room.",victim,NULL,ch,TO_CHAR);
     act("The blast of kiotic energy lifts $n off $s feet and sends $m flying to the other end of the room.",victim,NULL,ch,TO_VICT);
     act("The blast of kiotic energy lifts $n off $s feet and sends $m flying to the other end of the room.",victim,NULL,ch,TO_NOTVICT);

     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_DARK;
     ch->attack_var = BLAST;
     ch->attack_focus = UPPERCUT;
     hurt_person( ch, victim, 250000, TRUE );

     stop_fighting( victim, TRUE );
     victim->position = POS_STUNNED;
  }

  ch->wait = 4;
  stop_fighting( ch, TRUE );
}

bool is_assassin_combo( CHAR_DATA *ch, int move )
{
   /* special_id - last command entered */
   /* special_id_2 - nth move in combo string */
  
   if ( ch->special_id_2 == 0 )
      return TRUE; 

   /* Raijin - Jab, RoundHouse, SideWinder, Shin, Elbow, HeadButt */
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 1 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 2 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 3 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 4 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 5 ) && ( move == 3 ) )
      return TRUE;

   /* Senretsu - Elbow, RoundHouse, SideWinder, RoundHouse, Jab, Jab, Shin */
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 1 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 2 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 3 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 4 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 5 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 6 ) && ( move == 1 ) )
      return TRUE;

   /* Hagan - HeadButt, Jab, Elbow, Shin, RoundHouse, Jab, Elbow */
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 1 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 2 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 3 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 4 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 5 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 6 ) && ( move == 6 ) )
      return TRUE;

   /* Hoten - SideWinder, Shin, Jab, Jab, RoundHouse, Jab, HeadBuTT, SideWinder, Shin */
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 1 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 2 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 3 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 4 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 5 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 6 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 7 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 8 ) && ( move == 1 ) )
      return TRUE;

   /* Fukio - Shin, Shin, Sidewinder, RoundHouse, RoundHouse, Jab, HeadButt, SideWinder, Jab, Elbow, HeadButt, SideWinder, Shin*/
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 1 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 2 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 3 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 4 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 5 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 6 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 7) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 8 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 9 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 10 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 11 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 12 ) && ( move == 1 ) )
      return TRUE;

   /* Tenbu - HeadButt, Jab, Elbow, SideWinder, Shin, Shin, RoundHouse, SideWinder, RoundHouse, Elbow, HeadButt, Elbow, Jab*/
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 1 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 2 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 3 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 4 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 5 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 6 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 7 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 8 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 9 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 10 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 11 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 12 ) && ( move == 5 ) )
      return TRUE;

   /* ZiDen - HeadButt, RoundHouse, Shin, Jab, HeadButt, Shin, Jab, Jab, RoundHouse, SideWinder, HeadButt, RoundHouse, SideWinder*/
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 1 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 2 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 3 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 4 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 5 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 6 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 7 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 8 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 9 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 10 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 11 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 12 ) && ( move == 4 ) )
      return TRUE;

   /* KoHo - HeadButt, Shin, Jab, Elbow, HeadButt, Jab, HeadButt, HeadButt, Jab, Elbow, Shin, Jab, HeadButt*/
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 1 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 2 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 3 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 4 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 5 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 6 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 7 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 8 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 9 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 10 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 11 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 12 ) && ( move == 3 ) )
      return TRUE;

   /* RyuJin - SideWinder, Elbow, Jab, Shin, Elbow, Jab, HeadButt, HeadButt, Roundhouse, Elbow, Shin, Jab, Roundhouse, SideWinder, Shin*/
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 1 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 2 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 3 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 4 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 5 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 6 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 7 ) && ( move == 3 ) )
      return TRUE;
   if ( ( ch->special_id == 3 ) && ( ch->special_id_2 == 8 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 9 ) && ( move == 6 ) )
      return TRUE;
   if ( ( ch->special_id == 6 ) && ( ch->special_id_2 == 10 ) && ( move == 1 ) )
      return TRUE;
   if ( ( ch->special_id == 1 ) && ( ch->special_id_2 == 11 ) && ( move == 5 ) )
      return TRUE;
   if ( ( ch->special_id == 5 ) && ( ch->special_id_2 == 12 ) && ( move == 2 ) )
      return TRUE;
   if ( ( ch->special_id == 2 ) && ( ch->special_id_2 == 13 ) && ( move == 4 ) )
      return TRUE;
   if ( ( ch->special_id == 4 ) && ( ch->special_id_2 == 14 ) && ( move == 1 ) )
      return TRUE;

   return FALSE; 
}

void do_assassin_ryujin( CHAR_DATA *ch, CHAR_DATA *victim )
{
  act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
  act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);

  act("You spin around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_CHAR);
  act("$n spins around catching you offguard with an elbow to the face.",ch,NULL,victim,TO_VICT);
  act("$n spins around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
  act("You spin around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_CHAR);
  act("$n spins around catching you offguard with an elbow to the face.",ch,NULL,victim,TO_VICT);
  act("$n spins around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
  act("You spin around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_CHAR);
  act("$n spins around catching you offguard with an elbow to the face.",ch,NULL,victim,TO_VICT);
  act("$n spins around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
  act("You spin around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_CHAR);
  act("$n spins around catching you offguard with an elbow to the face.",ch,NULL,victim,TO_VICT);
  act("$n spins around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
  act("You suddenly push your palm into $N's chin making $M stagger back in pain.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly pushes $s palm into your chin making you stagger back in pain.",ch,NULL,victim,TO_VICT);
  act("$n suddenly pushes $s palm into $N's chin making $M stagger back in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You unsheath your sword as you mutter '#yNodara...#n', driving it into $N's body.",ch,NULL,victim,TO_CHAR);
  act("$n unsheaths $s sword as $e mutters '#yNodara...#n', driving it into your body.",ch,NULL,victim,TO_VICT);
  act("$n unsheaths $s sword as $e mutters '#yNodara...#n', driving it into $N's body.",ch,NULL,victim,TO_NOTVICT);
  act("You thrust your sword downwards $N's body as you mutter '#yEiji...#n'.",ch,NULL,victim,TO_CHAR);
  act("$n thrusts $s sword downwards your body as $e mutters '#yEiji...#n'.",ch,NULL,victim,TO_VICT);
  act("$n thrusts $s sword downwards $N's body as $e mutters '#yEiji...#n'.",ch,NULL,victim,TO_NOTVICT);
  act("You yell '#eRyujin!#n' as you quickly whirl your sword upwards $N's body, ripping $S chest apart.",ch,NULL,victim,TO_CHAR);
  act("$n yells '#eRyujin!#n' as $e quickly whirls $s sword upwards your body, ripping your chest apart.",ch,NULL,victim,TO_VICT);
  act("$n yells '#eRyujin!#n' as $e quickly whirls $s sword upwards $N's body, ripping $S chest apart.",ch,NULL,victim,TO_NOTVICT);

  act("As you land on your feet, $N's body bursts in flames!",ch,NULL,victim,TO_CHAR);
  act("As $n lands on $s feet, your body bursts in flames!",ch,NULL,victim,TO_VICT);
  act("As $n lands on $s feet, $N's body bursts in flames!",ch,NULL,victim,TO_NOTVICT);

  ch->ctimer[2] = 4;
  victim->agg = 100;

  if ( !IS_AFFECTED( victim, AFF_FLAMING ) )
     SET_BIT( victim->affected_by, AFF_FLAMING );

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SLASH;
  ch->attack_focus = UPPERCUT;
  SET_BIT( ch->arena, AFF2_COMBO );
  hurt_person(ch,victim,1000000,TRUE);
}

void do_assassin_tenbu( CHAR_DATA *ch, CHAR_DATA *victim )
{
  act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
  act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);

  act("You suddenly PuMmEl the ground with your fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_CHAR);
  act("$n suddenly PuMmEls the ground with $s fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_ROOM);
  act("Your image suddenly PuMmElS the ground with its fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_CHAR);
  act("$n's image suddenly PuMmElS the ground with its fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_ROOM);
  act("Your image suddenly PuMmElS the ground with its fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_CHAR);
  act("$n's image suddenly PuMmElS the ground with its fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_ROOM);
  act("Your image suddenly PuMmElS the ground with its fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_CHAR);
  act("$n's image suddenly PuMmElS the ground with its fist, causing a DeVaStAtInG EaRtHqUaKe!!!",ch,NULL,NULL,TO_ROOM);

  act("A MaSsIvE ShArD of RoCk JoLtS from the ground as it CoLlIdEs into you!",victim,NULL,NULL,TO_CHAR);
  act("A MaSsIvE ShArD of RoCk JoLtS from the ground as it CoLlIdEs into $n!",victim,NULL,NULL,TO_ROOM);

  ch->ctimer[4] = 4;
  victim->agg = 100;

  if ( !IS_SET( victim->mflags, MAGE_EMBRACED2 ) && !IS_SET( victim->mflags, MAGE_EMBRACED1 ) )
     SET_BIT( victim->mflags, MAGE_EMBRACED2 );

  ch->ele_attack = ATTACK_EARTH;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = BLUNT;
  ch->attack_focus = LOW;
  SET_BIT( ch->arena, AFF2_COMBO );
  hurt_person(ch,victim,1000000,TRUE);
}

void do_assassin_koho( CHAR_DATA *ch, CHAR_DATA *victim )
{
  act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
  act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);

  act("You mutter '#yKoho...#n' as you punch $N ViCiOuSlY in the stomach, encasing $M in ice.",ch,NULL,victim,TO_CHAR);
  act("$n mutters '#yKoho...#n' as $e punches you ViCiOuSlY in the stomach, encasing you in ice.",ch,NULL,victim,TO_VICT);
  act("$n mutters '#yKoho...#n' as $e punches $N ViCiOuSlY in the stomach, encasing $M in ice.",ch,NULL,victim,TO_NOTVICT);

  act("You suddenly smash your palms into $N making the ice around $M grow bigger.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly smashes $s palms into you making the ice around you grow bigger.",ch,NULL,victim,TO_VICT);
  act("$n suddenly smashes $s palms into $N making the ice around $M grow bigger.",ch,NULL,victim,TO_NOTVICT);

  act("You raise your palm upwards $N's body as it explodes with ferocious energy.",ch,NULL,victim,TO_CHAR);
  act("$n raise $s palm upwards your body as it explodes with ferocious energy.",ch,NULL,victim,TO_VICT);
  act("$n raise $s palm upwards $N's body as it explodes with ferocious energy.",ch,NULL,victim,TO_NOTVICT);

  act("You close your eyes suddenly as you push your right palm in front of you.",ch,NULL,NULL,TO_CHAR);
  act("$n closes $s eyes suddenly as $e pushes $s right palm in front of $m.",ch,NULL,NULL,TO_ROOM);

  act("A mass of energy begins to manifest in your hand as you push it towards $N.",ch,NULL,victim,TO_CHAR);
  act("A mass of energy begins to manifest in $n's hand as $e pushes it towards you.",ch,NULL,victim,TO_VICT);
  act("A mass of energy begins to manifest in $n's hand as $e pushes it towards $N.",ch,NULL,victim,TO_NOTVICT);

  act("You release the mass of energy as it ShAtTeRs the IcE encasing $N, ripping $S body apart!!!",ch,NULL,victim,TO_CHAR);
  act("$n releases the mass of energy as it ShAtTeRs the IcE encasing you, ripping your body apart!!!",ch,NULL,victim,TO_VICT);
  act("$n releases the mass of energy as it ShAtTeRs the IcE encasing $N, ripping $S body apart!!!",ch,NULL,victim,TO_NOTVICT);

  ch->ctimer[5] = 4;
  victim->agg = 100;

  if ( !is_martial( victim ) && !IS_SET( victim->more, MORE_FROZEN ) )
     {
        SET_BIT( victim->more, MORE_FROZEN );
        victim->freeze_timer = 1;
     }

  SET_BIT( ch->arena, AFF2_COMBO );
  ch->ele_attack = ATTACK_ICE;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = HIT;
  ch->attack_focus = DIRECT;
  hurt_person(ch,victim,1000000,TRUE);
}

void do_assassin_ziden( CHAR_DATA *ch, CHAR_DATA *victim )
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *vch, *vch_next;

  act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
  act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);

  act("Shimmering energy emits of your body as you backflip high into the skies.",ch,NULL,NULL,TO_CHAR);
  act("Shimmering energy emits of $n's body as $e backflips high into the skies.",ch,NULL,NULL,TO_ROOM);

  act("#eShimmering energy emits of your image as it backflips high into the skies.#n",ch,NULL,NULL,TO_CHAR);
  act("#eShimmering energy emits of your image as it backflips high into the skies.#n",ch,NULL,NULL,TO_CHAR);
  act("#eShimmering energy emits of your image as it backflips high into the skies.#n",ch,NULL,NULL,TO_CHAR);

  act("#eShimmering energy emits of $n's image as it backflips high into the skies.#n",ch,NULL,NULL,TO_ROOM);
  act("#eShimmering energy emits of $n's image as it backflips high into the skies.#n",ch,NULL,NULL,TO_ROOM);
  act("#eShimmering energy emits of $n's image as it backflips high into the skies.#n",ch,NULL,NULL,TO_ROOM);

  act("You spread your arms wide apart as you body blazes with demonic fire.",ch,NULL,NULL,TO_CHAR);
  act("$n spreads $s arms wide apart as $s body blazes with demonic fire.",ch,NULL,NULL,TO_ROOM);

  act("You roar with rage as your body becomes a living inferno!!",ch,NULL,NULL,TO_CHAR);
  act("$n roars with rage as $s body becomes a living inferno!!",ch,NULL,NULL,TO_ROOM);

  act("You SlAm yourself into $N causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_CHAR);
  act("$n SlAmS $mself into you causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_VICT);
  act("$n SlAmS $mself into $N causing a HUGE EXPLOSION!!!",ch,NULL,victim,TO_NOTVICT);

  ch->ctimer[6] = 4;
  SET_BIT( ch->arena, AFF2_COMBO );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
        continue;

     act("The ShOcKwAvE BlAsT you into the air as the flames eat through your flesh!",vch,NULL,NULL,TO_CHAR);
     act("The ShOcKwAvE BlAsTs $n into the air as the flames eat through $s flesh!",vch,NULL,NULL,TO_ROOM);
     ch->ele_attack = ATTACK_FIRE;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = BLAST;
     ch->attack_focus = LOW;
     hurt_person( ch, vch, 1000000, TRUE );
  }

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
     if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
        {
           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
              continue;

           if ( ch->in_room->area != d->character->in_room->area )
              continue;

           act("The ShOcKwAvE BlAsT you into the air as the flames eat through your flesh!",d->character,NULL,NULL,TO_CHAR);
           act("The ShOcKwAvE BlAsTs $n into the air as the flames eat through $s flesh!",d->character,NULL,NULL,TO_ROOM);

           d->character->agg = 100;
           ch->ele_attack = ATTACK_FIRE;
           ch->attack_type = ATTACK_MAGICAL;
           ch->attack_var = BLAST;
           ch->attack_focus = LOW;
           hurt_person(ch,d->character,1000000,TRUE);
        }
  }
}

void do_assassin_fukio( CHAR_DATA *ch, CHAR_DATA *victim )
{
  ch->ctimer[10] = 4;

  act("You tilt your head backwards as you brace your body viciously.",ch,NULL,NULL,TO_CHAR);
  act("$n tilts $s head backwards as $e braces $s body viciously.",ch,NULL,NULL,TO_ROOM);

  act("Shimmering energy emits of your body as you backflip high into the skies.",ch,NULL,NULL,TO_CHAR);
  act("Shimmering energy emits of $n's body as $e backflips high into the skies.",ch,NULL,NULL,TO_ROOM);

  act("#eShimmering energy emits of your image as it backflips high into the skies.#n",ch,NULL,NULL,TO_CHAR);
  act("#eShimmering energy emits of your image as it backflips high into the skies.#n",ch,NULL,NULL,TO_CHAR);
  act("#eShimmering energy emits of your image as it backflips high into the skies.#n",ch,NULL,NULL,TO_CHAR);

  act("#eShimmering energy emits of $n's image as it backflips high into the skies.#n",ch,NULL,NULL,TO_ROOM);
  act("#eShimmering energy emits of $n's image as it backflips high into the skies.#n",ch,NULL,NULL,TO_ROOM);
  act("#eShimmering energy emits of $n's image as it backflips high into the skies.#n",ch,NULL,NULL,TO_ROOM);
  act("As you push your right hand towards $N, it is engulfed in a manifest of energy.",ch,NULL,victim,TO_CHAR);
  act("As $n pushes $s right hand towards you, it is engulfed in a manifest of energy.",ch,NULL,victim,TO_VICT);
  act("As $n pushes $s right hand towards $N, it is engulfed in a manifest of energy.",ch,NULL,victim,TO_NOTVICT);

  act("You rapidly thrust your right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_CHAR);
  act("$n rapidly thrusts $s right hand at you, smashing you with a blistering energy bolt.",ch,NULL,victim,TO_VICT);
  act("$n rapidly thrusts $s right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_NOTVICT);

  act("You viciously push your left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_CHAR);
  act("$n viciously pushes $s left hand towards you, releasing a bolt of energy onto you.",ch,NULL,victim,TO_VICT);
  act("$n viciously pushes $s left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_NOTVICT);

  act("You rapidly thrust your right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_CHAR);
  act("$n rapidly thrusts $s right hand at you, smashing you with a blistering energy bolt.",ch,NULL,victim,TO_VICT);
  act("$n rapidly thrusts $s right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_NOTVICT);

  act("You viciously push your left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_CHAR);
  act("$n viciously pushes $s left hand towards you, releasing a bolt of energy onto you.",ch,NULL,victim,TO_VICT);
  act("$n viciously pushes $s left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_NOTVICT);

  act("You rapidly thrust your right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_CHAR);
  act("$n rapidly thrusts $s right hand at you, smashing you with a blistering energy bolt.",ch,NULL,victim,TO_VICT);
  act("$n rapidly thrusts $s right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_NOTVICT);

  act("You viciously push your left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_CHAR);
  act("$n viciously pushes $s left hand towards you, releasing a bolt of energy onto you.",ch,NULL,victim,TO_VICT);
  act("$n viciously pushes $s left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image rapidly thrust its right hand at $N, smashing $M with a blistering energy bolt.#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image rapidly thrusts its right hand at you, smashing you with a blistering energy bolt.#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image rapidly thrusts its right hand at $N, smashing $M with a blistering energy bolt.#n",ch,NULL,victim,TO_NOTVICT);

  act("You viciously push your left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_CHAR);
  act("$n viciously pushes $s left hand towards you, releasing a bolt of energy onto you.",ch,NULL,victim,TO_VICT);
  act("$n viciously pushes $s left hand towards $N, releasing a bolt of energy onto $M.",ch,NULL,victim,TO_NOTVICT);

  act("You rapidly thrust your right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_CHAR);
  act("$n rapidly thrusts $s right hand at you, smashing you with a blistering energy bolt.",ch,NULL,victim,TO_VICT);
  act("$n rapidly thrusts $s right hand at $N, smashing $M with a blistering energy bolt.",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image viciously pushes its left hand towards $N, releasing a bolt of energy onto $M.#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image viciously pushes its left hand towards you, releasing a bolt of energy onto you.#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image viciously pushes its left hand towards $N, releasing a bolt of energy onto $M.#n",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image rapidly thrust its right hand at $N, smashing $M with a blistering energy bolt.#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image rapidly thrusts its right hand at you, smashing you with a blistering energy bolt.#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image rapidly thrusts its right hand at $N, smashing $M with a blistering energy bolt.#n",ch,NULL,victim,TO_NOTVICT);

  act("#eYour image viciously pushes its left hand towards $N, releasing a bolt of energy onto $M.#n",ch,NULL,victim,TO_CHAR);
  act("#e$n's image viciously pushes its left hand towards you, releasing a bolt of energy onto you.#n",ch,NULL,victim,TO_VICT);
  act("#e$n's image viciously pushes its left hand towards $N, releasing a bolt of energy onto $M.#n",ch,NULL,victim,TO_NOTVICT);

  act("You push your hands together releasing a beam of energy towards $N.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s hands together releasing a beam of energy towards you.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s hands together releasing a beam of energy towards $N.",ch,NULL,victim,TO_NOTVICT);

  act("You push your hands together releasing a beam of energy towards $N.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s hands together releasing a beam of energy towards you.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s hands together releasing a beam of energy towards $N.",ch,NULL,victim,TO_NOTVICT);

  act("You push your hands together releasing a beam of energy towards $N.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s hands together releasing a beam of energy towards you.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s hands together releasing a beam of energy towards $N.",ch,NULL,victim,TO_NOTVICT);

  act("As you start falling back towards the ground, you tilt your hands together summoning massive energy.",ch,NULL,NULL,TO_CHAR);
  act("As $n starts falling back towards the ground, $e tilts $s hands together summoning massive energy.",ch,NULL,NULL,TO_ROOM);

  act("You yell '#yOooooo Fukio#n' as you RELEASE a HUGE BoLt of #C#zenergy#n towards $N, StAgGeRiNg $M.",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yOooooo Fukio#n' as you RELEASE a HUGE BoLt of #C#zenergy#n towards you, StAgGeRiNg you.",ch,NULL,victim,TO_VICT);
  act("$n yells '#yOooooo Fukio#n' as you RELEASE a HUGE BoLt of #C#zenergy#n towards $N, StAgGeRiNg $M.",ch,NULL,victim,TO_NOTVICT);

  SET_BIT( ch->arena, AFF2_COMBO );
  ch->ele_attack = ATTACK_THUNDER;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = BEAM;
  ch->attack_focus = AERIAL;
  hurt_person(ch,victim,1000000,TRUE);

  act("You land softly on your feet as you backflip into a fighting stance.",ch,NULL,NULL,TO_CHAR);
  act("$n lands softly on $s feet as $e backflips into a fighting stance.",ch,NULL,NULL,TO_ROOM);
}

void do_assassin_shin( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->gifts[ANI] < 1 )
      {
        send_to_char("You have to improve your Ni fighting style to the 1st Level.\n\r",ch);
        return;
      }

   if ( ch->gifts[CURRENT_METER] != ANI )
      {
        send_to_char("You aren't concentrating on the Ni fighting style.\n\r",ch);
        return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a rising shin to?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING)
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( !is_assassin_combo(ch,1) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
      }
      else {
              ch->special_id = 1;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 7 )
      {
         act("You pause midway as sparkles of energy emits of your body.",ch,NULL,NULL,TO_CHAR);
         act("$n pauses midway as sparkles of energy emits of $s body.",ch,NULL,NULL,TO_ROOM);
         act("You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_CHAR);
         act("$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.",ch,NULL,victim,TO_VICT);
         act("$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_NOTVICT);
         act("Your image twirls its body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_CHAR);
         act("$n's image twirls its body around StAgGeRiNg you with a vicious roundhouse kick.",ch,NULL,victim,TO_VICT);
         act("$n's image twirls its body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_NOTVICT);
         act("You lean forward as you kick $N viciously in the face.",ch,NULL,victim,TO_CHAR);
         act("$n leans forward as $e kicks you viciously in the face.",ch,NULL,victim,TO_VICT);
         act("$n leans forward as $e kicks $N viciously in the face.",ch,NULL,victim,TO_NOTVICT);
         act("Your image leans forward as it kicks $N viciously in the face.",ch,NULL,victim,TO_CHAR);
         act("$n's image leans forward as it kicks you viciously in the face.",ch,NULL,victim,TO_VICT);
         act("$n's image leans forward as it kicks $N viciously in the face.",ch,NULL,victim,TO_NOTVICT);
         act("Your image leans forward as it kicks $N viciously in the face.",ch,NULL,victim,TO_CHAR);
         act("$n's image leans forward as it kicks you viciously in the face.",ch,NULL,victim,TO_VICT);
         act("$n's image leans forward as it kicks $N viciously in the face.",ch,NULL,victim,TO_NOTVICT);
         act("You yell '#ySenretsu!#n' as you SmAsH $N's face with a powerful flash kick!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#ySenretsu!#n' as $e SmAsHeS your face with a powerful flash kick!",ch,NULL,victim,TO_VICT);
         act("$n yells '#ySenretsu!#n' as $e SmAsHeS $N's face with a powerful flash kick!",ch,NULL,victim,TO_NOTVICT);
         act("You yell '#yUnN!#n' as you DrIvE your fist into $N's face sending $M sprawling.",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yUnN!#n' as $e DrIvEs $s fist into your face sending you sprawling.",ch,NULL,victim,TO_VICT);
         act("$n yells '#yUnN!#n' as $e DrIvEs $s fist into $N's face sending $M sprawling.",ch,NULL,victim,TO_NOTVICT);

         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
         SET_BIT( ch->arena, AFF2_COMBO );

         victim->move = UMAX( 0, victim->move - 250000 );
         victim->mana = UMAX( 0, victim->mana - 250000 );
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = HIT;
         ch->attack_focus = DIRECT;
         hurt_person(ch,victim,700000,TRUE);

         return;
      }

   if ( ch->special_id_2 == 9 )
      {
         act("You pause midway as sparkles of energy emits of your body.",ch,NULL,NULL,TO_CHAR);
         act("$n pauses midway as sparkles of energy emits of $s body.",ch,NULL,NULL,TO_ROOM);
         act("You leap high into the sky emitting off palish energy in your path.",ch,NULL,NULL,TO_CHAR);
         act("$n leaps high into the sky emitting off palish energy in $s path.",ch,NULL,NULL,TO_ROOM);
         act("You suddenly SmAsH your foot into $N's face as you land on your feet.",ch,NULL,victim,TO_CHAR);
         act("$n suddenly SmAsHeS $s foot into your face as $e lands on $s feet.",ch,NULL,victim,TO_VICT);
         act("$n suddenly SmAsHeS $s foot into $N's face as $e lands on $s feet.",ch,NULL,victim,TO_NOTVICT);
         act("Your image suddenly SmAsHeS its foot into $N's face as it lands on its feet.",ch,NULL,victim,TO_CHAR);
         act("$n's image suddenly SmAsHeS its foot into your face as it lands on its feet.",ch,NULL,victim,TO_VICT);
         act("$n's image suddenly SmAsHeS its foot into $N's face as it lands on its feet.",ch,NULL,victim,TO_NOTVICT);
         act("You mutter '#yHoten Das...#n' as you whirl your palms into $N's chest viciously!",ch,NULL,victim,TO_CHAR);
         act("$n mutters '#yHoten Das...#n' as $e whirls $s palms into your chest viciously!",ch,NULL,victim,TO_VICT);
         act("$n mutters '#yHoten Das...#n' as $e whirls $s palms into $N's chest viciously!",ch,NULL,victim,TO_NOTVICT);
         act("Your image mutters '#yHoten Das...#n' as it whirls its palms into $N's chest viciously!",ch,NULL,victim,TO_CHAR);
         act("$n's image mutters '#yHoten Das...#n' as it whirls its palms into your chest viciously!",ch,NULL,victim,TO_VICT);
         act("$n's image mutters '#yHoten Das...#n' as it whirls its palms into $N's chest viciously!",ch,NULL,victim,TO_NOTVICT);
         act("Your image mutters '#yHoten Das...#n' as it whirls its palms into $N's chest viciously!",ch,NULL,victim,TO_CHAR);
         act("$n's image mutters '#yHoten Das...#n' as it whirls its palms into your chest viciously!",ch,NULL,victim,TO_VICT);
         act("$n's image mutters '#yHoten Das...#n' as it whirls its palms into $N's chest viciously!",ch,NULL,victim,TO_NOTVICT);
         act("Your image mutters '#yHoten Das...#n' as it whirls its palms into $N's chest viciously!",ch,NULL,victim,TO_CHAR);
         act("$n's image mutters '#yHoten Das...#n' as it whirls its palms into your chest viciously!",ch,NULL,victim,TO_VICT);
         act("$n's image mutters '#yHoten Das...#n' as it whirls its palms into $N's chest viciously!",ch,NULL,victim,TO_NOTVICT);
         act("You yell '#yThrice!#n' as you stun $N with a quick uppercut to the face!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yThrice!#n' as $e stuns you with a quick uppercut to the face!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yThrice!#n' as $e stuns $N with a quick uppercut to the face!",ch,NULL,victim,TO_NOTVICT);

         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
         SET_BIT( ch->arena, AFF2_COMBO );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = HIT;
         ch->attack_focus = UPPERCUT;
         hurt_person(ch,victim,750000,TRUE);
         return;
      }

   if ( ch->special_id_2 == 13 )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;

         if ( ch->ctimer[10] == 0 )
            do_assassin_fukio(ch,victim);

         return;
      }


   if ( ch->special_id_2 == 15 )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;

         if ( ch->ctimer[2] == 0 )
            do_assassin_ryujin(ch,victim);

         return;
      }
   
   act("You SmAsH $N's chin with the palm of your hand as you leap into the air.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHes your chin with the palm of $s hand as $e leaps into the air.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHes $N's chin with the palm of your hand as you leap into the air.",ch,NULL,victim,TO_NOTVICT);
}

void do_assassin_roundhouse( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->gifts[ANI] < 2 )
      {
        send_to_char("You have to improve your Ni fighting style to the 2nd Level.\n\r",ch);
        return;
      }

   if ( ch->gifts[CURRENT_METER] != ANI )
      {
        send_to_char("You aren't concentrating on the Ni fighting style.\n\r",ch);
        return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a roundhouse kick to?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING)
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( !is_assassin_combo(ch,2) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
      }
      else {
              ch->special_id = 2;
              ch->special_id_2++;
              ch->wait = 0 ;
           }
   
   act("You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.",ch,NULL,victim,TO_VICT);
   act("$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.",ch,NULL,victim,TO_NOTVICT);
}

void do_assassin_headbutt( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->gifts[ANI] < 3 )
      {
        send_to_char("You have to improve your Ni fighting style to the 3rd Level.\n\r",ch);
        return;
      }

   if ( ch->gifts[CURRENT_METER] != ANI )
      {
        send_to_char("You aren't concentrating on the Ni fighting style.\n\r",ch);
        return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a headbutt to?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING)
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( !is_assassin_combo(ch,3) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
      }
      else {
              ch->special_id = 3;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 6 )
      {
         act("You pause midway as sparkles of energy emits of your body.",ch,NULL,NULL,TO_CHAR);
         act("$n pauses midway as sparkles of energy emits of $s body.",ch,NULL,NULL,TO_ROOM);
         act("You quickly leap a few feet backwards as magical leaves encircles your raised right fist.",ch,NULL,NULL,TO_CHAR);
         act("$n quickly leaps a few feet backwards as magical leaves encircles $s raised right fist.",ch,NULL,NULL,TO_ROOM);
         act("You yell '#yEi Raijin!#n' as you DrIvE your fist into $N sending $M into the ground.",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yEi Raijin!#n' as $e DrIvEs $s fist into you sending you into the ground.",ch,NULL,victim,TO_VICT);
         act("$n yells '#yEi Raijin!#n' as $e DrIvEs $s fist into $N sending $M into the ground.",ch,NULL,victim,TO_NOTVICT);
         act("The magical leaves flashes through your body creating a huge explosion of positive energy.",victim,NULL,NULL,TO_CHAR);
         act("The magical leaves flashes through $n's body creating a huge explosion of positive energy.",victim,NULL,NULL,TO_ROOM);
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = HIT;
         ch->attack_focus = LOW;
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
         SET_BIT( ch->arena, AFF2_COMBO );

         hurt_person(ch,victim,750000,TRUE);
         return;
      }

   if ( ch->special_id_2 == 13 )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;

         if ( ch->ctimer[5] == 0 )
            do_assassin_koho(ch,victim);

         return;
      }
   
   act("You SmAsH your head into $N stunning $M.",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHes $s head into you stunning you.",ch,NULL,victim,TO_VICT);
   act("$n SmAsHes $s head into $N stunning $M.",ch,NULL,victim,TO_NOTVICT);
}

void do_assassin_kick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->gifts[ANI] < 4 )
      {
        send_to_char("You have to improve your Ni fighting style to the 4th Level.\n\r",ch);
        return;
      }

   if ( ch->gifts[CURRENT_METER] != ANI )
      {
        send_to_char("You aren't concentrating on the Ni fighting style.\n\r",ch);
        return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a super kick to?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING)
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( !is_assassin_combo(ch,4) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
      }
      else {
              ch->special_id = 4;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 13 )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;

         if ( ch->ctimer[6] == 0 )
            do_assassin_ziden(ch,victim);

         return;
      }
   
   act("You lean forward as you kick $N viciously in the face.",ch,NULL,victim,TO_CHAR);
   act("$n leans forward as $e kicks you viciously in the face.",ch,NULL,victim,TO_VICT);
   act("$n leans forward as $e kicks $N viciously in the face.",ch,NULL,victim,TO_NOTVICT);
}

void do_assassin_jab( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->gifts[ANI] < 5 )
      {
        send_to_char("You have to improve your Ni fighting style to the 5th Level.\n\r",ch);
        return;
      }

   if ( ch->gifts[CURRENT_METER] != ANI )
      {
        send_to_char("You aren't concentrating on the Ni fighting style.\n\r",ch);
        return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to give a swift punch to?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING)
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( !is_assassin_combo(ch,5) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
      }
      else {
              ch->special_id = 5;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 13 )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;

         if ( ch->ctimer[4] == 0 )
            do_assassin_tenbu(ch,victim);

         return;
      }
   
   act("You quickly punch $N hard in the stomach.",ch,NULL,victim,TO_CHAR);
   act("$n quickly punches you hard in the stomach.",ch,NULL,victim,TO_VICT);
   act("$n quickly punches $N hard in the stomach.",ch,NULL,victim,TO_NOTVICT);
}

void do_assassin_elbow( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch, CLASS_SHADOW) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->gifts[ANI] < 6 )
      {
        send_to_char("You have to improve your Ni fighting style to the 6th Level.\n\r",ch);
        return;
      }

   if ( ch->gifts[CURRENT_METER] != ANI )
      {
        send_to_char("You aren't concentrating on the Ni fighting style.\n\r",ch);
        return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to elbow in the face?\n\r", ch );
	   return;
      }
    
   if (ch->position == POS_STANDING)
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( !is_assassin_combo(ch,6) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
      }
      else {
              ch->special_id = 6;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 7 )
      {
         act("You pause midway as sparkles of energy emits of your body.",ch,NULL,NULL,TO_CHAR);
         act("$n pauses midway as sparkles of energy emits of $s body.",ch,NULL,NULL,TO_ROOM);
         act("You mutter '#yOooooo...#n' as you spread your hands apart.",ch,NULL,NULL,TO_CHAR);
         act("$n mutters '#yOooooo...#n' as $e spreads $s hands apart.",ch,NULL,NULL,TO_ROOM);
         act("You yell '#yHagan!#n' as you ClAsP $N's head together with your hands!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yHagan!#n' as $e ClAsPs your head together with $s hands!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yHagan!#n' as $e ClAsPs $N's head together with $s hands!",ch,NULL,victim,TO_NOTVICT);
         act("You stagger backwards as everything becomes a dazed.",victim,NULL,NULL,TO_CHAR);
         act("$n staggers backwards as everything becomes a dazed to $m.",victim,NULL,NULL,TO_ROOM);

         victim->move = UMAX( 0, victim->move - 250000 );
         victim->mana = UMAX( 0, victim->mana - 250000 );
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = BLUNT;
         ch->attack_focus = DIRECT;
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 4;
         SET_BIT( ch->arena, AFF2_COMBO );

         hurt_person(ch,victim,700000,TRUE);

         return;
      }
   
   act("You spin around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_CHAR);
   act("$n spins around catching you offguard with an elbow to the face.",ch,NULL,victim,TO_VICT);
   act("$n spins around catching $N offguard with an elbow to the face.",ch,NULL,victim,TO_NOTVICT);
}
