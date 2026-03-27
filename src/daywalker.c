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


void do_dcreate( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int cost, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Syntax: dcreate <item>\n\r", ch );
          send_to_char( "items are: vest (1), boots (1), gloves (1), pants (1), coat (1)\n\r", ch );
          send_to_char( "           helmet (1), bracer (1), ring (1), collar (1), belt (1)\n\r", ch );
          send_to_char( "           sleeves (1), sword (1), dagger (1), shades (1)\n\r", ch );
          return;
       }

    cost = 1;

    if ( !str_cmp(arg,"vest") )
       {
         vnum = 190;
       }
    else if ( !str_cmp(arg,"boots") )
            {
               vnum = 191;
            }
    else if ( !str_cmp(arg,"gloves") )
            {
               vnum = 192;
            }
    else if ( !str_cmp(arg,"pants") )
            {
               vnum = 193;
            }
    else if ( !str_cmp(arg,"coat") )
            {
               vnum = 194;
            }
    else if ( !str_cmp(arg,"helmet") )
            {
               vnum = 195;
            }
    else if ( !str_cmp(arg,"bracer") )
            {
               vnum = 196;
            }
    else if ( !str_cmp(arg,"ring") )
            {
               vnum = 197;
            }
    else if ( !str_cmp(arg,"collar") )
            {
               vnum = 198;
            }
    else if ( !str_cmp(arg,"belt") )
            {
               vnum = 199;
            }
    else if ( !str_cmp(arg,"sleeves") )
            {
               vnum = 200;
            }
    else if ( !str_cmp(arg,"sword") )
            {
               vnum = 201;
            }
    else if ( !str_cmp(arg,"dagger") )
            {
               vnum = 202;
            }
    else if ( !str_cmp(arg,"shades") )
            {
               vnum = 203;
            }
    else {
            do_dcreate(ch,"");
            return;
         }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold#n piece to make this item.\n\r", cost);
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
    act("You create $p silently.",ch,obj,NULL,TO_CHAR);
    act("$n creates $p silently in the corner.",ch,obj,NULL,TO_ROOM);
}

/* Daywalker skills */
/* Discipline learn */
void do_inquest( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost;
    char buf[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    cost = 1000000;

    if (!IS_CLASS(ch, CLASS_DAYWALKER) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
       centre_text("#e<<<-=[**]=-<#R Dark Spawn of Eternal Tempest #e>-=[**]=->>>#n",ch);
       centre_text("#e|\\ 0 0 /|#n",ch);
       centre_text("#e \\\\[=]// #n",ch);
       send_to_char("\n\r",ch);
 	 sprintf(buf,"#RMatsutus#b: #e[%d]     #yAshanitude#b: #e[%d]     #pOnisha#b: #e[%d]\n\r",
       ch->dskills[DMAT],ch->dskills[DASH],ch->dskills[DONI]);
       centre_text(buf,ch);
 	 sprintf(buf,"#eTourach#b: #e[%d]     #gAluren#b: #e[%d]\n\r", ch->dskills[DTOU],ch->dskills[DALU]);
       centre_text(buf,ch);
 	 sprintf(buf,"                  #wAuspex#b: #e[%d]\n\r", ch->dskills[DAUS]);
       centre_text(buf,ch);
       centre_text("#e<<<-=[**]=-=[**]=-=[**]=-=[**]=-=[**]=-=[**]=-=[**]=->>>#n\n",ch);
       return;
    }

    if (!str_cmp(arg,"matsutus"))
    {
	if ( ch->dskills[DMAT] >= 10 )
	{
	    send_to_char("You have already learnt all of your technique for this discipline.\n\r",ch);
	    return;
	}

	ch->dskills[DMAT] = 10;
	sprintf(buf,"You improve on your Matsutus technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"ashanitude"))
    {
	if ( ch->dskills[DASH] >= 10 )
	{
	    send_to_char("You have already learnt all of your techniques for this discipline.\n\r",ch);
	    return;
	}

	ch->dskills[DASH] = 10;
	sprintf(buf,"You improve on your Ashanitude technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"onisha"))
    {
	if ( ch->dskills[DONI] >= 10 )
	{
	    send_to_char("You have already learnt all of your techniques for this discipline.\n\r",ch);
	    return;
	}

	ch->dskills[DONI] = 10;
	sprintf(buf,"You improve on your Onisha technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"tourach"))
    {
	if ( ch->dskills[DTOU] >= 10 )
	{
	    send_to_char("You have already learnt all of your techniques for this discipline.\n\r",ch);
	    return;
	}

	ch->dskills[DTOU] = 10;
	sprintf(buf,"You improve on your Tourach technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"aluren"))
    {
	if ( ch->dskills[DALU] >= 10 )
	{
	    send_to_char("You have already learnt all of your techniques for this discipline.\n\r",ch);
	    return;
	}

	ch->dskills[DALU] = 10;
	sprintf(buf,"You improve on your Aluren technique.\n\r");
	send_to_char(buf,ch);
    }
    else if (!str_cmp(arg,"auspex"))
    {
	if ( ch->dskills[DAUS] >= 10 )
	{
	    send_to_char("You have already learnt all of your techniques for this discipline.\n\r",ch);
	    return;
	}

	ch->dskills[DAUS] = 10;
	sprintf(buf,"You improve on your Auspex technique.\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("To improve your techniques, type: Inquest #RMatsutus/#yAshanitude/#pOnisha/#eTourach/#gAluren/#wAuspex.\n\r",ch);
}
/* Discipline learn */


/*Daywalker study */
void do_insight( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];

   if (IS_NPC(ch)) return;
  
   argument = one_argument( argument, arg );

  if (!IS_CLASS(ch, CLASS_DAYWALKER) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

   if (arg[0] == '\0')
    {
	sprintf(buf,"Techniques: #RMatsutus [%d], #yAshanitude [%d], #pOnisha [%d], #eTourach [%d], #gAluren [%d], #wAuspex [%d].\n\r",
	ch->dskills[DMAT], ch->dskills[DASH],ch->dskills[DONI],ch->dskills[DTOU],ch->dskills[DALU],ch->dskills[DAUS]);
	send_to_char(buf,ch);
	return;
    }

    if (arg[0] != '\0')
    {
	if (!str_cmp(arg,"matsutus"))
	{
	    send_to_char("#RMatsutus - The Art of War.\n\r\n\r",ch);
	    if (ch->dskills[DMAT] < 1) 
		send_to_char("#wYou do not know any of the Matsutus techniques.\n\r",ch);
          if (ch->dskills[DMAT] > 6)
		send_to_char("#RJiamar       #y-Jiamar-      #wQuick, savage attack which enables one to target a new foe.\r\n", ch);
	    if (ch->dskills[DMAT] > 8)
		send_to_char("#RRecover      #y-Recover-     #wQuickly recover from your wounds.\r\n", ch);
	}
	else if (!str_cmp(arg,"auspex"))
	{
	    send_to_char("#wAuspex - The disciplines Art of Shadow and Mind.\n\r\n\r",ch);
	    if (ch->dskills[DAUS] < 1) 
		send_to_char("#wYou know none of the Auspex techniques.\n\r",ch);
          if (ch->dskills[DAUS] > 7)
		send_to_char("#wSonicSpeed      #y-Sonic Speed-    #wYou gain the speed of 4 cheetahs (Auto).\n\r", ch );
          if (ch->dskills[DAUS] > 9)
		send_to_char("#wPsionicBlast    #y-Psionic Blast-  #wA powerful mindblast that tears through the mind of everyone.\n\r", ch );
	    return;
	}
	else if (!str_cmp(arg,"ashanitude"))
	{
	    send_to_char("#yAshanitude - The power of realm plane link.\n\r\n\r",ch);
	    if (ch->dskills[DASH] < 1) 
		send_to_char("#wYou know none of the Ashanitude techniques.\n\r",ch);
	    if (ch->dskills[DASH] > 2) 
		send_to_char("#yCelestialGrab  #y-Cgrab       #wGrabs your victim into or out of shadowplane.\n\r",ch);
	    if (ch->dskills[DASH] > 3) 
		send_to_char("#yEtherealGrab   #y-Egrab-      #wGrabs your victim into or out of ethereal.\n\r",ch);
 	    if (ch->dskills[DASH] > 4)
		send_to_char("#yStatis         #y-Statis-     #wYour victim is held trapped between two planes.\n\r", ch);
	    if (ch->dskills[DASH] > 7)
		send_to_char("#yShatter        #y-Shatter-    #wYou crush the mind of your victim leaving him reeling in pain.\n\r", ch );
	    if (ch->dskills[DASH] > 8)
		send_to_char("#yMindblock      #y-Mind block- #wProtects you from mental attacks. (Auto)\n\r", ch );
	    return;
	}
	else if (!str_cmp(arg,"tourach"))
	{
	    send_to_char("#eTourach - Mystical Art of Death.\n\r\n\r",ch);
	    if (ch->dskills[DTOU] < 0) 
		send_to_char("#wYou know none of the Tourach techniques.\n\r",ch);
	    if (ch->dskills[DTOU] > 1) 
		send_to_char("#eRathi           #y-Rathi-          #wSummon a Rathi Dragon.\n\r",ch);
	    if (ch->dskills[DTOU] > 2) 
		send_to_char("#eDespoticSeptor  #y-Despoticseptor- #wCreates shield that prevents magical affects upon you.\n\r",ch);
	    if (ch->dskills[DTOU] > 3)
		send_to_char("#eSoullink        #y-Soul Link-      #wYou tune into the soul of your victim. (Auto)\n\r", ch );
	    if (ch->dskills[DTOU] > 6)
		send_to_char("#eDeathgrip       #y-Deathgrip-      #wLeaves your victim weaken and in great pain.\n\r", ch );
        if (ch->dskills[DTOU] > 7)
		send_to_char("#eSouldrain       #y-SoulDrain-      #wDrains the lifeforce of the victim.\n\r", ch );
        if (ch->dskills[DTOU] > 8) 
		send_to_char("#eDarkRitual      #y-Darkritual      #wReforms the victim from objectform.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg,"aluren"))
	{
	    send_to_char("#gAluren - Nature's Essence.\n\r\n\r",ch);
	    if (ch->dskills[DALU] < 0) 
		send_to_char("#gYou know none of the Aluren techniques.\n\r",ch);
	    if (ch->dskills[DALU] > 0) 
		send_to_char("#gChange          #y-Change-               #wTranform into different creatures.\n\r",ch);
	    if (ch->dskills[DALU] > 1) 
		send_to_char("#gBarbedFoliage   #y-Barbed Foliage-       #wCast to create a shield of Barbed Foliage.\n\r",ch);
	    if (ch->dskills[DALU] > 3) 
		send_to_char("#gNatureWrath     #y-NatureWrath-          #wDamage everyone and heals group.\n\r",ch);
	    if (ch->dskills[DALU] > 4) 
		send_to_char("#gSunbeam         #y-Sunbeam               #wSets victim on fire.\n\r",ch);
        if (ch->dskills[DALU] > 8)
		send_to_char("#gSunAura         #y-Sun's Aura-           #wCast to reduce magical damage.\n\r", ch );
	    return;
        }
        else if (!str_cmp(arg,"onisha"))
	   {
	    send_to_char("#pOnisha - Defensive Tactics.\n\r\n\r",ch);
	    if (ch->dskills[DONI] < 0) 
		send_to_char("#wYou know none of the Onisha techniques.\n\r",ch);
	    if (ch->dskills[DONI] > 0) 
		send_to_char("#pDaydream          #y-Daydream-        #wMessage to your victim through their daydreams.\n\r",ch);
	    if (ch->dskills[DONI] > 1) 
		send_to_char("#pNightmare         #y-Nightmare-       #wAttacks a victim in the same area.\n\r",ch);
	    if (ch->dskills[DONI] > 2) 
		send_to_char("#pStakeWall         #y-Stakewall-       #wSummons a wall of stakes.\n\r",ch);
	    if (ch->dskills[DONI] > 3) 
		send_to_char("#pPiercingRain      #y-PiercingRain-    #wSummons a rain of stakes.\n\r",ch);
	    return;
         }   
         else do_insight(ch,"");
     }
  }
/* Daywalker study */

void do_serum(CHAR_DATA *ch, char *argument) 
{
	if ( IS_NPC(ch) ) return;

      if ( !IS_CLASS(ch,CLASS_DAYWALKER) )
         {
            send_to_char("Huh?\n\r",ch);
            return;
         }

      if ( ch->pcdata->ninja_powers[HARA_KIRI] > 0 )
         {
            send_to_char("You are already experiencing the affects of the Serum.\n\r", ch );
            return;
         }

	if ( ch->hit < ch->max_hit / 10 )
         {
            send_to_char("You are hurt too badly already.\n\r", ch );
            return;
         }

	ch->pcdata->ninja_powers[HARA_KIRI] = 1000;
	send_to_char("You inject some serum into yourself.\n\r", ch );
	act("$n injects some serum into $mself.",ch,NULL,NULL,TO_ROOM);
	send_to_char("You roar with rage as unholy power soars through your veins.\n\r", ch );
}

/*Grab*/
void do_cgrab(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument,arg);
 
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch,CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DASH] < 3 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

   if ( arg[0] == '\0' )
      {
          send_to_char( "Who do you wish to Grab?\n\r", ch );
          return;
      }

   if ( ( victim = get_char_room( ch, arg) ) == NULL )
      {
          send_to_char("They arent here.\n\r",ch);
          return;
      }

   if ( ch->fight_timer > 0 && IS_NPC( victim ) && victim->master != NULL )
   {
      stc("You can't do this on creatures like these with a fight timer!\n\r", ch );
      return;
   }

   if ( victim->fight_timer > 0 )
      {
          stc("But they have a fight timer.\n\r",ch);
          return;
      }

   if ( !IS_AFFECTED(victim, AFF_SHADOWPLANE) )
      {
          act("You pull $N into the Shadowplane.",ch,NULL,victim,TO_CHAR);
          act("$n pulls you into the Shadowplane.",ch,NULL,victim,TO_VICT);
          act("$n pulls $N into the Shadowplane.",ch,NULL,victim,TO_NOTVICT);
       
          WAIT_STATE(ch, 8);     
          SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
          return;
      }

   act("You pull $N out of the Shadowplane.",ch,NULL,victim,TO_CHAR);
   act("$n pulls you out of the Shadowplane.",ch,NULL,victim,TO_VICT);
   act("$n pulls $N out of the Shadowplane.",ch,NULL,victim,TO_NOTVICT);

   WAIT_STATE(ch, 8);     
   REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
}
/* End of Grab*/


/*EGrab*/
void do_egrab(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument,arg);
 
    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch,CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Who do you wish to Ethereal Grab?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg) ) == NULL )
       {
          send_to_char("They arent here.\n\r",ch);
          return;
       }

	if ( ch->fight_timer > 0 && IS_NPC( victim ) && victim->master != NULL )
   {
      stc("You can't do this on creatures like these with a fight timer!\n\r", ch );
      return;
   }

    if ( victim->fight_timer > 0 )
       {
          stc("But they have a fight timer.\n\r",ch);
          return;
       }

    if ( !IS_AFFECTED(victim, AFF_ETHEREAL) )
       {
          act("You pull $N into the Ethereal Plane.",ch,NULL,victim,TO_CHAR);
          act("$n pulls you into the Ethereal Plane.",ch,NULL,victim,TO_VICT);
          act("$n pulls $N into the Ethereal Plane.",ch,NULL,victim,TO_NOTVICT);
       
          WAIT_STATE(ch, 8);     
          SET_BIT(victim->affected_by, AFF_ETHEREAL);
          return;
       }

    act("You pull $N out of the Ethereal Plane.",ch,NULL,victim,TO_CHAR);
    act("$n pulls you out of the Ethereal Plane.",ch,NULL,victim,TO_VICT);
    act("$n pulls $N out of the Ethereal Plane.",ch,NULL,victim,TO_NOTVICT);

    WAIT_STATE(ch, 8);     
    REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
}
/* End of EGrab*/


void do_transform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 

    if ( IS_CLASS(ch, CLASS_DRAGON_KNIGHT) )
       {
          do_dragoon_transform( ch, argument );
          return;
       }

    send_to_char("Huh?\n\r",ch);
    return;

/*
    argument = one_argument( argument, arg ); 

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DONI] < 7 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "You can change between 'human', 'spirit', 'wolf' and 'phoenix' forms.\n\r", ch );
          return;
       }

    if ( !str_cmp(arg,"spirit") )
       {
          if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
             {
                send_to_char( "You can only polymorph from human form.\n\r", ch );
                return;
             }

          if ( ch->fight_timer > 0 )
             {
                send_to_char( "Not with a fight timer.\n\r", ch );
                return;
             }

          act( "You feel translucent in form.", ch, NULL, NULL, TO_CHAR );
          act( "$n transforms into a spirit.", ch, NULL, NULL, TO_ROOM );
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
          SET_BIT(ch->polyaff, POLY_SPIRIT);
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
          SET_BIT(ch->affected_by, AFF_POLYMORPH);
      
          if ( !IS_AFFECTED(ch, AFF_ETHEREAL) )
             SET_BIT(ch->affected_by, AFF_ETHEREAL);
      
          if ( IS_EXTRA(ch, TIED_UP) )
             {
                 act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR);
                 act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
                 REMOVE_BIT(ch->extra, TIED_UP);
                 REMOVE_BIT(ch->extra, GAGGED);
                 REMOVE_BIT(ch->extra, BLINDFOLDED);
             }

          if ( is_affected(ch, gsn_web) )
             {
                 act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
                 send_to_char("The webbing entrapping you falls through your ethereal form.\n\r",ch);
                 affect_strip(ch, gsn_web);
             }

          sprintf(buf, "#w%s #ethe #ys#wpiri#yt#n", ch->name);
          free_string( ch->morph );
          ch->morph = str_dup( buf );
          return;
     }
     else if ( !str_cmp(arg,"wolf") )
             {
	          if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
 	             {
                      send_to_char( "You can only polymorph from human form.\n\r", ch );
                      return;
                   }

                if ( ch->mounted == IS_RIDING )
                   do_dismount(ch,"");

                ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
                act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR );
                act( "$n transforms into a dire wolf.", ch, NULL, NULL, TO_ROOM );
                clear_stats(ch);

                ch->pcdata->mod_str = 5;
                ch->pcdata->mod_int = 5;
                ch->pcdata->mod_wis = 5;
                ch->pcdata->mod_dex = 5;
                ch->pcdata->mod_con = 5;
                SET_BIT(ch->polyaff, POLY_WOLF);
                SET_BIT(ch->affected_by, AFF_POLYMORPH);
                SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
                sprintf(buf, "#e%s #othe #edire #owolf#n", ch->name);
                free_string( ch->morph );
                ch->morph = str_dup( buf );
                return;
             }
     else if ( !str_cmp(arg,"phoenix") )
             {
	          if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
                   {
                      send_to_char( "You can only polymorph from human form.\n\r", ch );
                      return;
                   }

                if ( ch->mounted == IS_RIDING )
                   do_dismount(ch,"");

                act( "You transform into a fiery phoenix.", ch, NULL, NULL, TO_CHAR );
                act( "$n transforms into a fiery phoenix.", ch, NULL, NULL, TO_ROOM );
                clear_stats(ch);

                SET_BIT(ch->polyaff, POLY_PHOENIX);
                SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
                SET_BIT(ch->affected_by, AFF_POLYMORPH);
                SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
                sprintf(buf, "#r%s #ythe #Rph#yoen#Rix#n", ch->name);
                free_string( ch->morph );
                ch->morph = str_dup( buf );
                return;
             }
     else if ( !str_cmp(arg,"human") )
             {
                if ( !IS_AFFECTED(ch, AFF_POLYMORPH) )
	             {
                      send_to_char( "You are already in human form.\n\r", ch );
                      return;
                   }

                if ( IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_SPIRIT) )
                   {
                      REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
                      REMOVE_BIT(ch->polyaff, POLY_SPIRIT);
                      REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
                   }
                   else if ( IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF) )
                           REMOVE_BIT(ch->polyaff, POLY_WOLF);
                   else if ( IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_PHOENIX) )
                           {
                              REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
                              REMOVE_BIT(ch->polyaff, POLY_PHOENIX);
                           }
                           else {
                                   send_to_char( "You seem to be stuck in this form.\n\r", ch );
                                   return;
                                }

                act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
                act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
                REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
                clear_stats(ch);
                free_string( ch->morph );
                ch->morph = str_dup( "" );
                return;
             }
             else send_to_char( "You can change between 'human', 'spirit', 'wolf' and 'phoenix' forms.\n\r", ch );*/
}
/* End of Change Forms */


/* Bloodagony */
void do_cursedblood(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_STRING_LENGTH];
    OBJ_DATA *obj; 
    AFFECT_DATA *paf;

    if ( IS_NPC(ch) )
       return;

    one_argument( argument,arg);
    
    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DONI] < 5 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0')
       {
          send_to_char("Cursed Blood what?\n\r",ch);
          return;
       }
     
    if ( ( obj= get_obj_carry(ch,arg) ) == NULL )
       {
          send_to_char("You dont have that weapon.\n\r",ch);
          return;
       }
 
    if ( obj->item_type != ITEM_WEAPON
       ||   IS_SET(obj->quest, QUEST_ARTIFACT)
       ||   obj->chobj != NULL 
       ||  IS_SET(obj->quest, QUEST_BLOODA))
       {
          send_to_char("You are unable to cursed blood this weapon.\n\r",ch);
	    return;
       }

    obj->value[1] += 25000;
    obj->value[2] += 25000;

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
    paf->modifier       = 25000;

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
    paf->modifier       = 25000;

    paf->bitvector      = 0;
    paf->next           = obj->affected;
    obj->affected       = paf;

    SET_BIT(obj->quest, QUEST_BLOODA);
 
     act( "You cut your wrist and smear your blood on $p.", ch, obj,NULL, TO_CHAR );
     act( "$n cuts his wrist and smears blood on $p.", ch, obj, NULL,TO_ROOM );
}
/* End of Bloodagony */



/* Taste */
void do_dtaste( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      buf [MAX_STRING_LENGTH];
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument (argument, arg );

    if ( IS_NPC(ch) )
       return;
 
    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->dskills[DAUS] < 4 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( IS_NPC(victim) )
       {
          send_to_char( "Only useful on players.\n\r",ch);
          return;
       }                               

    sprintf(buf,"You examine $N's blood carefully.\n\r");
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n examines your blood carefully.\n\r");
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"$n examines $N's blood carefully.\n\r");
    act(buf,ch,NULL,victim,TO_NOTVICT);
 
    send_to_char("\n\r",ch);
    send_to_char("\n\r",victim);

    if ( !IS_NPC(victim) )
       {
	    sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
          send_to_char(buf,ch);
       }

    sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
      
    sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    send_to_char(buf,ch);
   
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);

    if ( !IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT) )
       act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
}
/* End of Taste */

/* Karma */
void do_karma( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;
	
    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r", ch);
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
         send_to_char( "You appear from seemingly nowhere.\n\r", ch );
         act( "$n appears from seemingly nowhere.", ch, NULL, NULL, TO_ROOM );
      }
      else {
              act( "$n concentrates softly fading from sight.", ch, NULL, NULL, TO_ROOM );
              send_to_char( "You concentrate softly fading from sight.\n\r", ch );
              SET_BIT(ch->act, PLR_WIZINVIS);
           }
}
/* Karma */


/* PsionicBlast */
void do_psionicblast( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char("You aren't powerful enough to use this technique yet.\n\r",ch);
          return;
       }

    if ( ch->dskills[DAUS] < 10 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 500000 )
       {
          send_to_char("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          send_to_char("You must wait until you are able to use this again.\n\r",ch);
          return;
       }

    ch->wait = 6;
    ch->ctimer[0] = 4;
    ch->mana -= 500000;
    act("The room goes dark as you focus your concentration on all opponents.",ch,NULL,NULL,TO_CHAR);
    act("The room goes dark as $n focuses $s concentration on all opponents.",ch,NULL,NULL,TO_ROOM);
    act("You bare your fangs as you angrily release a searing blast of mental energy in all directions.",ch,NULL,NULL,TO_CHAR);
    act("$n bares $s fangs as $e angrily releases a searing blast of mental energy in all directions.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) ) 
          continue;

       if ( !IS_CLASS( vch, CLASS_DAYWALKER ) )
          {
             act("You scream as unbelievable energy rips your mind apart.",vch,NULL,NULL,TO_CHAR);
             act("$n clutches $s head in agony as he screams loudly in pain!",vch,NULL,NULL,TO_ROOM);
             ch->attack_type = ATTACK_MAGICAL;
             ch->attack_var = SOUL;
             ch->attack_focus = DIRECT;
             hurt_person( ch, vch, 1000000, TRUE );
          }
    }
}
/* PsionicBlast */


/* Recover */
void do_recover( CHAR_DATA *ch, char *argument )
{
    int bonus = 300000;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DMAT] < 9 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    hit_gain( ch, bonus );
    mana_gain( ch, bonus );
    move_gain( ch, bonus );
    ch->wait = 12;
    act("You silently tend to your wounds, removing some of the pain and suffering.",ch,NULL,NULL,TO_CHAR);
    act("$n silently tends to $s wounds, removing some of the pain and suffering.",ch,NULL,NULL,TO_ROOM);
}
/* Recover */


/* Shakar */
void do_shakar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int chance;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER ) )
       {
          stc("Huh?\n\r",ch);
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
	
    act("You suddenly whip out your shakar with a flick of your wrist.",ch,NULL,NULL,TO_CHAR);
    act("$n suddenly whips out $s shakar with a flick of $s wrist.",ch,NULL,NULL,TO_ROOM);
    act("The Shakar pulses with demonic energy as you throw it ViCiOuSlY across the room.",ch,NULL,NULL,TO_CHAR);
    act("The Shakar pulses with demonic energy as $n throws it ViCiOuSlY across the room.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( ch == vch )
           continue;

        if ( !is_safe(ch, vch) )
	     {
              act("You scream in agony as the Shakar slices through your neck, sparying blood everywhere.",vch,NULL,NULL,TO_CHAR);
              act("The room is filled with $n's blood as the Shakar RiPs through $s NeCk!",vch,NULL,NULL,TO_ROOM);

              if ( IS_CLASS( vch, CLASS_VAMPIRE ) )
                 chance = 84;
                 else chance = 94;

              if ( number_percent() > chance - devil_luck( ch, vch, 5 ) && !IS_SET( vch->cmbt[5], SKL2_AMIND ) )
                 {
                    act( "You gasp vainly for air as you collapse in a heap on the ground.", vch, NULL, NULL, TO_CHAR );
                    act( "$n gasps vainly for air as $e collapses in a heap on the ground.", vch, NULL, NULL, TO_ROOM );

                    stop_fighting( vch, TRUE );
                    vch->hit = -2000010;
                    vch->position = POS_MORTAL;
                 }
                 else {
                         ch->attack_type = ATTACK_PHYSICAL;
                         ch->attack_var = SLASH;
                         ch->attack_focus = DIRECT;
                         hurt_person( ch, vch, 3500000, TRUE );
                      }
	     }
    }

    act("You leap into the air as you catch the ancient weapon, sheathing it in your clothes again.",ch,NULL,NULL,TO_CHAR);
    act("$n leaps into the air as $e catches the Shakar, sheathing it in $s clothes again.",ch,NULL,NULL,TO_ROOM);
}
/* Shakar */

void do_rathi( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    AFFECT_DATA af;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DTOU] < 2 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->followers > 0 )
       {
	    send_to_char("Too many people are following you.\n\r",ch);
          return;
       }

    ch->wait = 8;
    ch->pcdata->followers++;

    victim = create_mobile( get_mob_index( 50 ) );

    victim->level = 980;
    victim->armor = -100000;
    victim->hitroll = number_range( 1500000, 2500000 );
    victim->damroll = number_range( 1500000, 2500000 );
    victim->hit = 12000000;
    victim->max_hit = victim->hit;
    SET_BIT( victim->act, ACT_MOUNT );

    char_to_room( victim, ch->in_room );

    act( "You open a shimmering portal to another realm as a Huge Rathi dragon steps through.", ch, NULL, victim, TO_CHAR );
    act( "$n opens a shimmering portal to another realm as a Huge Rathi dragon steps through.", ch, NULL, victim, TO_NOTVICT );

    add_follower( victim, ch );

    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
}

/* Jiamar */
void do_jiamar( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->dskills[DMAT] < 7 )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ( victim = get_char_room( ch, arg) ) == NULL )
     {
        stc("They aren't here.\n\r",ch);
        return;
     }

  if ( ch->fighting == NULL )
     {
        stc("You must be fighting someone to use this ancient technique.\n\r",ch);
        return;
     }

  if ( ch->fighting == victim )
     {
        stc("You are already fighting them.\n\r",ch);
        return;
     }

  act("You bare your fangs with rage as you viciously slash across $N's throat.",ch,NULL,ch->fighting,TO_CHAR);
  act("$n bares $s fangs with rage as $e viciously slashes across your throat.",ch,NULL,ch->fighting,TO_VICT);
  act("$n bares $s fangs with rage as $e viciously slashes across $N's throat.",ch,NULL,ch->fighting,TO_NOTVICT);
  act("You then PuNcH $M onto the ground with your sheer strength.",ch,NULL,ch->fighting,TO_CHAR);
  act("$n then PuNcHeS you onto the ground with $s sheer strength.",ch,NULL,ch->fighting,TO_VICT);
  act("$n then PuNcHeS $M onto the ground with $s sheer strength.",ch,NULL,ch->fighting,TO_NOTVICT);
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = HIT;
  ch->attack_focus = LOW;
  hurt_person( ch, ch->fighting, 700000, TRUE );

  ch->fighting = NULL;
  ch->position = POS_STANDING;

  act("Suddenly, you pounce on $N ripping your claws through $M.",ch,NULL,victim,TO_CHAR);
  act("Suddenly, $n pounces on you ripping $s claws through you.",ch,NULL,victim,TO_VICT);
  act("Suddenly, $n pounces on $N ripping your claws through $M.",ch,NULL,victim,TO_NOTVICT);

  ch->wait = 12;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = HIT;
  ch->attack_focus = AERIAL;
  hurt_person( ch, victim, 700000,TRUE );
}
/* Jiamar */


/* Statis */
void do_statis( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 5 )
       {
          send_to_char("You aren't powerful enough to use this technique yet.\n\r",ch);
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          send_to_char("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Banish whom between the two planes?\n\r", ch );
          return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }

    if ( ch->mana < 100000 )
       {
          send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    victim->wait = 24;
    ch->ctimer[2] = 4;
    ch->mana -= 100000;
    act("You raise the shadows in the room as you grab $N and banish $S soul.",ch,NULL,victim,TO_CHAR);
    act("$n raises the shadows in the room as $e grabs you and banish your soul.",ch,NULL,victim,TO_VICT);
    act("$n raises the shadows in the room as $e grabs $N and banish $S soul.",ch,NULL,victim,TO_NOTVICT);
    act("You mumble in the ancient language releasing a curse onto $N's soul.",ch,NULL,victim,TO_CHAR);
    act("$n mumbles in the ancient language releasing a curse onto your soul.",ch,NULL,victim,TO_VICT);
    act("$n mumbles in the ancient language releasing a curse onto $N's soul.",ch,NULL,victim,TO_NOTVICT);
    act("You scream as your soul is stuck between two barriers, trapped in the netherworld.",victim,NULL,NULL,TO_CHAR);
    act("$n screams as $s soul is stuck between two barriers, trapped in the netherworld.",victim,NULL,NULL,TO_ROOM);
    act("You can't move.",ch,NULL,victim,TO_VICT);
}
/* Statis */


/* Shatter */
void do_shatter( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DASH] < 5 )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Whose mind do you wish to shatter?\n\r", ch );
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

    ch->wait = 12;
    act("You concentrate on $N's mind disrupting $S nervous system.",ch,NULL,victim,TO_CHAR);
    act("$n concentrates on your mind disrupting your nervous system.",ch,NULL,victim,TO_VICT);
    act("$n concentrates on $N's mind disrupting $S nervous system.",ch,NULL,victim,TO_NOTVICT);
    ch->ele_attack = ATTACK_DARK;
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 700000, TRUE );
}
/* Shatter */

/* deathgrip */
void do_death_grip( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
           send_to_char("Huh?\n\r",ch);
           return;
       }

    if ( ch->dskills[DTOU] < 7 ) 
       {
           send_to_char("Huh?\n\r",ch);
           return;
       }

    if ( ch->dlife[CURRENT] < 25 )
       {
           send_to_char("You don't have enough life force for that.\n\r",ch);
           return;
       }

    argument = one_argument( argument, arg );

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

    if ( is_safe( ch, victim ) )
       return;

    if ( IS_SET( victim->more, MORE_PAIN ) )
       {
          stc("They are already in agony!\n\r",ch);
          return;
       }

    ch->wait = 4;
    ch->dlife[CURRENT] -= 25;

    act("You suddenly grip on $N's arm viciously, draining $S will to fight right before $S eyes.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly grips on your arm viciously, draining your will to fight right before your eyes.",ch,NULL,victim,TO_VICT);
    act("$n suddenly grips on $N's arm viciously, draining $S will to fight right before $S eyes.",ch,NULL,victim,TO_NOTVICT);
    SET_BIT( victim->more, MORE_PAIN );
    SET_BIT( victim->monkstuff, MONK_DEATH );
}

/* Soul Drain */
void do_soul_drain( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DTOU] < 8 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dlife[CURRENT] < 20 )
       {
          send_to_char("You don't have enough life force for that.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Whose lifeforce do you wish to drain?\n\r", ch );
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

    ch->wait = 18;
    ch->dlife[CURRENT] -= 20;
    act("You focus on $N's lifeforce as $E suddenly turns pale.",ch,NULL,victim,TO_CHAR);
    act("$n focuses on your lifeforce making your turn pale.",ch,NULL,victim,TO_VICT);
    act("$n focuses on $N's lifeforce as $E suddenly turns pale.",ch,NULL,victim,TO_NOTVICT);
    act("Your body trembles with power as you drain $N of some of $S lifeforce.",ch,NULL,victim,TO_CHAR);
    act("$n's body trembles with power as $e drains you of some of your lifeforce.",ch,NULL,victim,TO_VICT);
    act("$n's body trembles with power as $e drains $N of some of $S lifeforce.",ch,NULL,victim,TO_NOTVICT);
    
    ch->hit = UMIN( ch->hit + 150000, ch->max_hit );
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 850000, TRUE );
}
/* Soul Drain */

/* Dark Ritual */
void do_daywalker_ritual( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DTOU] < 9 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dlife[CURRENT] < 10 )
       {
          send_to_char("You don't have enough life force for that.\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( ( obj = get_obj_room( ch, arg ) ) == NULL )
       {
	    send_to_char( "Nothing like that in this room.\n\r", ch );
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

    if ( IS_SET(victim->more, MORE_SAFE) )
       REMOVE_BIT(victim->more, MORE_SAFE);

    act("$p transforms into $n.",victim,obj,NULL,TO_ROOM);
    act("You reform your human body.",victim,obj,NULL,TO_CHAR);
    extract_obj(obj);
    ch->dlife[CURRENT] -= 10;
    victim->wait = 48;
}
/* Dark Ritual */


/* Despotic Septor */
void do_despotic_septor( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( ch->in_room == NULL )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->dlife[CURRENT] < 100 )
       {
          send_to_char("You don't have enough life force for that.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more2, MORE2_SHIELD) )
       {
          act("You remove the death field surrounding your body.",ch,NULL,NULL,TO_CHAR);
          act("$n removes the death field surrounding $s body.",ch,NULL,NULL,TO_ROOM);
          REMOVE_BIT(ch->more2, MORE2_SHIELD);
          return;
       }

    ch->dlife[CURRENT] -= 100;
    act("You place a death field around your body.",ch,NULL,NULL,TO_CHAR);
    act("$n places a death field around $s body.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->more2, MORE2_SHIELD);
    ch->wait = 12;
}
/* Despotic Septor */


/* Armor Thorns */
void do_armor_thorns( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( ch->in_room == NULL )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DALU] < 1 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more, MORE_WALL) )
       {
          act("You relieve the tension on your skin making it back to normal.",ch,NULL,NULL,TO_CHAR);
          REMOVE_BIT(ch->more, MORE_WALL);
          return;
       }

    act("You harden your skin making it as tough as thorns.",ch,NULL,NULL,TO_CHAR);
    SET_BIT(ch->more, MORE_WALL);
    ch->wait = 12;
}
/* Armor Thorns */


/* Nature's Cleansing */
void do_nature_cleansing( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->dskills[DALU] < 3 ) 
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->mana < 1500000 )
     {
        send_to_char("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  ch->wait = 12;
  ch->mana -= 1500000;
  act("You tilt your head towards the sun as it bathes your body with blazing energy.",ch,NULL,NULL,TO_CHAR);
  act("$n tilts $s head towards the sun as it bathes $s body with blazing energy.",ch,NULL,NULL,TO_ROOM);
  act("You feel more rejuvernated as all negative affects are removed from your body.",ch,NULL,NULL,TO_CHAR);
  clear_affects( ch );
}
/* Nature's Cleansing */


/* Nature's Wrath */
void do_nature_wrath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch, *vch_next;
  int dam;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->dskills[DALU] < 4 ) 
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->mana < 4000000 )
     {
        send_to_char("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  ch->mana -= 4000000;
  act("You release nature's fury into the room.",ch,NULL,NULL,TO_CHAR);
  act("$n releases nature's fury into the room.",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( ch != vch && ( !is_same_group(ch,vch) || nogang != 0 ) )
        continue;

     act("You feel alot better as a soft wind swerves around your tired body.",vch,NULL,NULL,TO_CHAR);
     act("$n looks better as a soft wind swerves around $s tired body.",vch,NULL,NULL,TO_ROOM);

     hit_gain( vch, 500000 );
  }

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
        vch_next = vch->next_in_room;

        if ( is_safe(ch, vch) )
           continue;
 
        stc("You are struck down by a massive beam of energy!\n\r",vch);
        act("$n is struck down by a massive beam of energy!",vch,NULL,NULL,TO_ROOM);

        dam = 900000;
        ch->ele_attack = ATTACK_THUNDER;
        ch->attack_type = ATTACK_MAGICAL;
        ch->attack_var = BLAST;
        ch->attack_focus = AERIAL;
        hurt_person(ch,vch,dam,TRUE);
  }

  ch->wait = 18;
}
/* Nature's Wrath */


/* Sun Beam */
void do_sunbeam( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int dam = 1000000;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DALU] < 5 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->move < 500000 )
       {
          send_to_char("You are too tired to do this.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Who do you wish to set on fire?\n\r", ch );
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

    ch->wait = 18;
    ch->move -= 500000;
    act("You fall on your knees lowering your hands onto the ground.",ch,NULL,NULL,TO_CHAR);
    act("$n falls on $s knees lowering $s hands onto the ground.",ch,NULL,NULL,TO_ROOM);
    act("You scream as a massive beam of fire SmAsHeS directly into you from above.",victim,NULL,NULL,TO_CHAR);
    act("$n screams as a massive beam of fire SmAsHeS directly into $m from above.",victim,NULL,NULL,TO_ROOM);

    if ( !IS_AFFECTED( victim,AFF_FLAMING ) ) 
       SET_BIT( victim->affected_by,AFF_FLAMING );

    ch->ele_attack = ATTACK_FIRE;
    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = BEAM;
    ch->attack_focus = AERIAL;
    hurt_person( ch, victim, dam, TRUE );
}
/* Sun Beam */


/* IceBurst */
void do_iceburst(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DALU] < 6 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

  if ( ch->generation > 2 )
     {
        stc( "You are currently too weak to manipulate this object.\n\r", ch );
        return;
     }

    if ( ch->mana < 150000 )
       {
	    send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    if ( argument[0] == '\0' )
       {
          send_to_char("Which item do you wish to encase in ice?\n\r", ch);
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

    if ( IS_WEAP(obj, WEAPON_ICEBURST) )
       {
          act("$p is already encased in ice.", ch, obj, NULL, TO_CHAR);
          return;
       }

    act("$p shimmers violently as it turns freezing blue.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n, shimmers violently as it turns freezing blue.", ch, obj, NULL, TO_ROOM);
    ch->mana -= 150000;

    WAIT_STATE(ch, 12);
    SET_BIT(obj->weapflags, WEAPON_ICEBURST);
}
/* IceBurst */


/* Holy Water */
void do_holy_water(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->dskills[DALU] < 7 ) 
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

  if ( ch->generation > 2 )
     {
        stc( "You are currently too weak to manipulate this object.\n\r", ch );
        return;
     }

    if ( ch->mana < 150000 )
       {
	    send_to_char( "You don't have enough magical energy.\n\r", ch );
          return;
       }

    if ( argument[0] == '\0' )
       {
          send_to_char("Which item do you wish to sprinkle holy water onto?\n\r", ch);
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

    if ( IS_WEAP(obj, WEAPON_HOLYWATER) )
       {
          act("$p already has holy water sprinkled on it.", ch, obj, NULL, TO_CHAR);
          return;
       }

    act("You sprinkle some holy water upon $p.", ch, obj, NULL, TO_CHAR);
    act("$n sprinkles some holy water upon $p.", ch, obj, NULL, TO_ROOM);
    ch->mana -= 150000;

    WAIT_STATE(ch, 12);
    SET_BIT(obj->weapflags, WEAPON_HOLYWATER);
}
/* HolyWater */

/* Day Dream */
void do_day_dream( CHAR_DATA *ch, char *argument)
{
   char arg1 [MAX_INPUT_LENGTH];
   char arg2 [MAX_INPUT_LENGTH];
   char buf [MAX_STRING_LENGTH];

   CHAR_DATA *victim;
   one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->dskills[DONI] < 1 ) 
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg1[0] == '\0' ) 
      {
         send_to_char("Send a Force message to who?\r\n", ch);
         return;
      }

   if ( arg1[0] == '\0' )
      {
         stc("Who do you wish to send this message to?\n\r",ch);
         return;
      }

   if ( ( victim = get_char_world( ch, arg1)  ) == NULL ) 
      {
         send_to_char("They aren't here.\r\n", ch);
         return;
      }

   sprintf(buf, "%s: %s", ch->pcdata->switchname, argument);
   send_to_char("You receive a message in your head from a DayWalker.\r\n", victim);
   send_to_char( buf, victim);
   send_to_char("Okay.\r\n", ch);
} 
/* Day Dream */


/* Nightmare */
void do_nightmare( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  /*if ( IS_CLASS(ch, CLASS_SOULREAVER) )
     {
        do_soul_nightmare(ch,argument);
        return;
     }*/

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->dskills[DONI] < 2 ) 
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( arg[0] == '\0' )
     {
        stc("Who do you wish to send nightmares upon?\n\r",ch);
        return;
     }

  if ( ch->mana < 500000 )
     {
        stc("You don't have enough magical energy for that.\n\r",ch);
        return;
     }

  if ( ( victim = get_char_area( ch, arg ) ) == NULL )
     {
        stc("You scry the vicinity, but cannot find your victim.\n\r",ch);
        return;
     }

  if ( is_safe(ch,victim) )
     return;

  ch->mana -= 500000;
  WAIT_STATE( ch, 18 );
  act("You send the worst nightmares imaginable to wreck havok in $N's mind.",ch,NULL,victim,TO_CHAR); 

  for ( vch = victim->in_room->people; vch != NULL; vch = vch_next )
  {
       vch_next = vch->next_in_room;

       if ( ch == vch || is_same_group(ch,vch) )
          continue;

       if ( !is_safe(ch,vch) )
       {
          if ( vch == victim )
             dam = 750000;
             else dam = 350000;

          act("The sky turns an ominous black.\n\r...Seconds later, your worst nightmares appear before you, making your fears come alive.",ch,NULL,vch,TO_VICT);
          ch->ele_attack = ATTACK_DARK;
          ch->attack_type = ATTACK_MAGICAL;
          ch->attack_var = SOUL;
          ch->attack_focus = OVERHEAD;
          hurt_person( ch, vch, dam, TRUE );
       }
  }
}
/* Nightmare */


/* Stakewall */
void do_stakewall(CHAR_DATA *ch, char *argument)
{
   int door;

   if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

   if ( ch->mana < 250000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 250000;

   for ( door = 0 ; door < 4 ; door++ )
   {
	if ( ch->in_room->exit[door] != NULL )
         make_wall(ch->in_room, door, EX_STAKE_WALL);
   }

   act("You raise your right hand viciously as hideous stakes rise from the ground.",ch,NULL,NULL,TO_CHAR);
   act("$n raises $s right hand viciously as hideous stakes rise from the ground.",ch,NULL,NULL,TO_ROOM);
   ch->wait = 12;
}
/* Stakewall */


/* Stake rain */
void do_piercing_rain( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->dskills[DONI] < 4 ) 
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to cast Stakes on?\n\r",ch);
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

   if ( ch->mana < 2500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

  if ( is_safe( ch, victim ) ) return;

  ch->mana -= 2500000;

  act("You raise your right hand in the air as the sky goes dark.",ch,NULL,NULL,TO_CHAR);
  act("$n raises $s right hand in the air as the sky goes dark.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly a barrage of stakes fade into existence, flying towards $n!",victim,NULL,NULL,TO_ROOM);
  act("Suddenly a barrage of stakes fade into existence, flying towards you!",victim,NULL,NULL,TO_CHAR);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_CHAR);
  act("You screams loudly as the stake thrusts through your chest!",ch,NULL,victim,TO_VICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_NOTVICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_CHAR);
  act("You screams loudly as the stake thrusts through your chest!",ch,NULL,victim,TO_VICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_NOTVICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_CHAR);
  act("You screams loudly as the stake thrusts through your chest!",ch,NULL,victim,TO_VICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_NOTVICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_CHAR);
  act("You screams loudly as the stake thrusts through your chest!",ch,NULL,victim,TO_VICT);
  act("$N screams loudly as the stake thrusts through $S chest!",ch,NULL,victim,TO_NOTVICT);
  ch->wait = 12;

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = AERIAL;

  if ( IS_CLASS( victim, CLASS_VAMPIRE ) )
     hurt_person( ch, victim, 1250000, TRUE );
     else hurt_person( ch, victim, 700000, TRUE );
}
/* Stake rain */

/* Desparation */
void do_desparation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_DAYWALKER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    ch->wait = 12;
    subtract_stock( ch, 100 );
    SET_BIT( ch->arena, AFF2_DM );

    act("You grin evilly as you look all around you.",ch,NULL,NULL,TO_CHAR);
    act("$n grins evilly as $e looks all around you.",ch,NULL,NULL,TO_ROOM);
    act("You say calmly as you adjust your shades '#yIt's a good day to die.#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says calmly as $e adjusts $s shades '#yIt's a good day to die.#n'",ch,NULL,NULL,TO_ROOM);
    act("Suddenly screaming with rage, you go berserk on everyone.",ch,NULL,NULL,TO_CHAR);
    act("Suddenly screaming with rage, $n goes berserk on everyone.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( is_safe(ch, vch) )
           continue;

        ch->attack_type = ATTACK_PHYSICAL;
        ch->attack_var = HIT;
        ch->attack_focus = DIRECT;
        hurt_person( ch, vch, 1000000, TRUE );
    }
}
/* Desparation */

void do_sonic_thrust( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch, CLASS_DAYWALKER ) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to stab up?\n\r",ch);
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

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  act("You rip out your pulsing shakar as you dash towards $N!",ch,NULL,victim,TO_CHAR);
  act("$n rips out $s pulsing shakar as $e dashes towards you!",ch,NULL,victim,TO_VICT);
  act("$n rips out $s pulsing shakar as $e dashes towards $N!",ch,NULL,victim,TO_NOTVICT);
  act("You thrust your shakar deep into $N's chest as $E staggers back from the blow.",ch,NULL,victim,TO_CHAR);
  act("$n thrusts $s shakar deep into your chest as you stagger back from the blow.",ch,NULL,victim,TO_VICT);
  act("$n thrusts $s shakar deep into $N's chest as $E staggers back from the blow.",ch,NULL,victim,TO_NOTVICT);
  act("You then DrAg your weapon downwards $N's body RiPpInG $S chest open!",ch,NULL,victim,TO_CHAR);
  act("$n then DrAgS $s weapon downwards your body RiPpInG your chest open!",ch,NULL,victim,TO_VICT);
  act("$n then DrAgS $s weapon downwards $N's body RiPpInG $S chest open!",ch,NULL,victim,TO_NOTVICT);
  act("You mutter '#wI hate vampires...#n as your body radiates off energy",ch,NULL,NULL,TO_CHAR);
  act("$n mutters '#wI hate vampires...#n as $s body radiates off energy",ch,NULL,NULL,TO_ROOM);
  act("You yell '#yDie!#n' as you DrIvE the shakar through $N's neck!",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yDie!#n' as $e DrIvEs the shakar through your neck!",ch,NULL,victim,TO_VICT);
  act("$n yells '#yDie!#n' as $e DrIvEs the shakar through $N's neck!",ch,NULL,victim,TO_NOTVICT);

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = DIRECT;

  if ( IS_CLASS(victim, CLASS_VAMPIRE) )
     hurt_person(ch,victim,3500000,TRUE);
     else hurt_person(ch,victim,2000000,TRUE);
}
