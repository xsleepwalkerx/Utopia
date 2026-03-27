
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

void do_vampirearmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_SOULREAVER) )
  {
      send_to_char("What?\n\r",ch);
      return;
  }

  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of vampire armor you wish to make: Ring Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Visor Dagger.\n\r",ch);
    return;
  }

    if ( ch->gold < 1 )
    {
        send_to_char("It costs #w1 #ygold piece#n to create vampire equipment.\n\r",ch);
        return;
    }

    if      (!str_cmp(arg,"ring"	)) vnum = 313;
    else if (!str_cmp(arg,"plate"	)) vnum = 315;
    else if (!str_cmp(arg,"helmet"	)) vnum = 316;
    else if (!str_cmp(arg,"collar"	)) vnum = 314;
    else if (!str_cmp(arg,"leggings" )) vnum = 317;
    else if (!str_cmp(arg,"boots" )) vnum = 318;
    else if (!str_cmp(arg,"gloves" )) vnum = 319;
    else if (!str_cmp(arg,"sleeves" )) vnum = 320;
    else if (!str_cmp(arg,"cape" )) vnum = 321;
    else if (!str_cmp(arg,"belt" )) vnum = 322;
    else if (!str_cmp(arg,"bracer" )) vnum = 323;
    else if (!str_cmp(arg,"visor" )) vnum = 324;
    else if (!str_cmp(arg,"dagger" )) vnum = 312;
    else
    {
    do_vampirearmor(ch,"");
    return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a Oni.\n\r",ch);
    return;
    }

    ch->gold -= 1;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_abduction( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS( ch, CLASS_VAMPIRE ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pcdata->followers > UMIN( 3, ch->pkill / 6 ) )
       {
          send_to_char("Too many people are following you.\n\r",ch);
          return;
       }

    if ( ch->mana < 500000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }   

    argument = one_argument( argument, arg );

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	 send_to_char("They are not here.\n\r", ch);
	 return;
    }

    if ( !IS_NPC(victim) )
    {
	 send_to_char("Not on players.\n\r", ch);
	 return;
    }

    if ( victim->level == 999 && !IS_SET( victim->extra, EXTRA_ZOMBIE ) )
       {
          send_to_char("They are protected by the immortals.\n\r", ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    ch->pcdata->followers++;
    ch->mana -= 500000;
    ch->wait = 6;

    act("You manipulate $N's mind as you take full control over $S very essence.",ch,NULL,victim,TO_CHAR);
    act("$n manipulates $N's mind as $e takes full control over $S very essence.",ch,NULL,victim,TO_NOTVICT);

    if ( victim->master != NULL )
       stop_follower( victim );

    stop_fighting(victim,TRUE);

    add_follower( victim, ch );
    do_group(ch,victim->name);

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
}

void do_inherit( CHAR_DATA *ch, char *argument )
{
   send_to_char("Huh?\n\r",ch);
   return;
}


void do_evolve( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
      {
        send_to_char("Huh?\n\r",ch);
        return;
      }

   if ( ch->cmbt[4] == AGE_CHILDE )
      {
         if ( ch->exp < 10000000 )
            {
               stc("You are not experienced enough to use the dark arts to train your age.\n\r",ch);
               return;
            }

         stc("You are now an Ancilla.\n\r",ch);
         sprintf(buf, "#r%s #eis now an #RAncilla.#n", ch->name );
         do_info(ch,buf);
         ch->cmbt[4] = AGE_NEONATE;
         ch->exp -= 10000000;
         return;
      }

   if ( ch->cmbt[4] == AGE_NEONATE )
      {
         if ( ch->exp < 20000000 )
            {
               stc("You are not experienced enough to use the dark arts to train your age.\n\r",ch);
               return;
            }

         stc("You are now a Elder.\n\r",ch);
         sprintf(buf, "#r%s #eis now an #RElder.#n", ch->name );
         do_info(ch,buf);
         ch->cmbt[4] = AGE_ANCILLA;
         ch->exp -= 20000000;
         return;
      }

   if ( ch->cmbt[4] == AGE_ANCILLA )
      {
         if ( ch->exp < 30000000 )
            {
               stc("You are not experienced enough to use the dark arts to train your age.\n\r",ch);
               return;
            }

         stc("You are now a Methuselah.\n\r",ch);
         sprintf(buf, "#r%s #eis now a #RMethuselah.#n", ch->name );
         do_info(ch,buf);
         ch->cmbt[4] = AGE_ELDER;
         ch->exp -= 30000000;
         return;
      }

   if ( ch->cmbt[4] == AGE_ELDER )
      {
         if ( ch->exp < 40000000 )
            {
               stc("You are not experienced enough to use the dark arts to train your age.\n\r",ch);
               return;
            }

         stc("You are now a Trueblood, a pure vampire.\n\r",ch);
         sprintf(buf, "#eFear #r%s, #efor they are now a #RPureBlood.#n", ch->name );
         do_info(ch,buf);
         ch->cmbt[4] = AGE_METHUSELAH;
         ch->exp -= 40000000;
         return;
      }

   if ( ch->cmbt[4] == AGE_METHUSELAH )
      {
         if ( ch->exp < 50000000 )
            {
               stc("You are not experienced enough to use the dark arts to train your age.\n\r",ch);
               return;
            }

         stc("You are now a Soulreaver, an #eAngel of Death#n.\n\r",ch);
         sprintf(buf, "#r%s!!!, #eSlaugther all, my #wAngel#e of #RDeath#e!!!#n", ch->name );
         do_info(ch,buf);
         ch->cmbt[4] = AGE_LA_MAGRA;
         ch->exp -= 50000000;
         return;
      }

   if ( ch->cmbt[4] == AGE_LA_MAGRA )
      {
         if ( ch->exp < 60000000 )
            {
               stc("You are not experienced enough to use the dark arts to train your age.\n\r",ch);
               return;
            }

         stc("You are now a La Magra, the legendary #RBlood God#n.\n\r",ch);
         sprintf(buf, "#eFinally, #r%s holds your very #wdestiny#e within their very #Rhands#e!!!#n", ch->name );
         do_info(ch,buf);
         ch->cmbt[4] = AGE_TRUEBLOOD;
         ch->exp -= 60000000;
         return;
      }

   stc("You can't age any further childe.\n\r",ch);
}


/* Inferno */
void do_vinferno(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *inroom;

    if (IS_NPC(ch))
      return;

    inroom = ch->in_room;

    if (!IS_CLASS(ch,CLASS_VAMPIRE))
      {
       send_to_char("Huh?\n\r",ch);
       return;
      }

    if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING))
      {
       REMOVE_BIT(inroom->room_flags,ROOM_FLAMING);
       return;
      }

    if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
      {
       send_to_char("Somthing prevents you from torching this room.\n\r",ch);
       return;
      }

    SET_BIT(inroom->room_flags,ROOM_FLAMING);
    sprintf(buf,"A look of concentration passes over %s's face.\n\r",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    sprintf(buf,"This room is engulfed in flames!");
    send_to_char(buf,ch);
    act(buf,ch,NULL,NULL,TO_ROOM);
}
/* End of Inferno */


/* Mirror */
void do_mirror( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];
    AFFECT_DATA af;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 1000000 )
       {
          stc("You don't have enough magical energy to perform this technique.\n\r",ch);
          return;
       }

    if ( ch->pcdata->condition[COND_THIRST] < 100 )
       {
          stc("You do not have enough blood to create a Mirror Image of yourself.\n\r",ch);
          return;
       }

    if ( ch->pcdata->followers > 0 )
       {
          send_to_char("Nothing happens.\n\r",ch);
          return;
       }

    ch->pcdata->followers++;
    ch->mana -= 1000000;

    victim = create_mobile( get_mob_index( 30 ) );

    if ( IS_AFFECTED( ch, AFF_POLYMORPH ) )
       {
          victim->short_descr = str_dup( ch->morph );
          sprintf(buf, "%s is hovering here.\n\r",ch->morph);
       }
       else {
               victim->short_descr = str_dup( ch->name );
               sprintf(buf, "%s is hovering here.\n\r",ch->name);
            }

    victim->long_descr = str_dup( buf );
    victim->name = str_dup( ch->name );
    victim->level = 990;
    victim->max_hit = ch->max_hit;
    victim->hit = ch->hit / 3;

    victim->mana = ch->mana;
    victim->max_mana = ch->max_mana;
    victim->max_move = ch->max_move;
    victim->move = ch->move;
    victim->hitroll = char_hitroll( ch ) / 4;
    victim->damroll = char_damroll( ch ) / 4;
    ch->pcdata->condition[COND_THIRST] -= 100;
    char_to_room( victim, ch->in_room );

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
       SET_BIT(victim->affected_by, AFF_SHADOWPLANE);

    if ( IS_AFFECTED(ch, AFF_ETHEREAL) )
       SET_BIT(victim->affected_by, AFF_ETHEREAL);

    if ( IS_SET( ch->more3, MORE3_TIGER ) )
       SET_BIT( victim->more3, MORE3_TIGER );

    if ( IS_SET( ch->more3, MORE3_DRAGON ) )
       SET_BIT( victim->more3, MORE3_DRAGON );

    if ( IS_SET( ch->more3, MORE3_CRANE ) )
       SET_BIT( victim->more3, MORE3_CRANE );

    if ( IS_SET( ch->more3, MORE3_FALCON ) )
       SET_BIT( victim->more3, MORE3_FALCON );

    if ( IS_SET( ch->more3, MORE3_MANTIS ) )
       SET_BIT( victim->more3, MORE3_MANTIS );

    if ( IS_SET( ch->more3, MORE3_WOLF ) )
       SET_BIT( victim->more3, MORE3_WOLF );

    act( "You concentrate your powers and form a mirror image of yourself.", ch, NULL, victim, TO_CHAR );
    act( "$n waves $s hands and a mirror image of $mself appears.", ch, NULL, victim, TO_ROOM );
    add_follower( victim, ch );
    do_group(ch,victim->name);
}
/* End of Mirror */


/* Spit */
void do_spit( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int dam;
    char arg [MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
      return;

    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->polyaff, POLY_SERPENT) )
       {
          send_to_char("You aren't in serpent form.\n\r",ch);
          return;
       }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
 	   send_to_char( "Whom do you want to spit at?\n\r", ch );
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

    if ( ch->pcdata->condition[COND_THIRST] < 100 )
       {
          send_to_char("You dont have enough blood.\n\r",ch);
          return;
       }

	if ( is_safe(ch,victim) )
       return;

    WAIT_STATE( ch, 12 );
    ch->pcdata->condition[COND_THIRST] -= 100;

    dam = 500000;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = DIRECT;

    dam = is_attack_type( ch, victim, dam );

    sprintf(buf,"Your spit of acid hits $N incredibly hard! [%d]",dam);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n's spit of acid hits you incredibly hard! [%d]",dam);
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"$n's spit of acid hits $N incredibly hard! [%d]",dam);
    act(buf,ch,NULL,victim,TO_NOTVICT);

    dam = 500000;

    hurt_person(ch,victim,dam,TRUE);
}
/* End of Spit */


/* Majesty */
void do_majesty( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if ( ch->in_room == NULL )
       return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_SET( ch->cmbt[7], SKL_MAJESTY ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 2000000 )
       {
          stc("You dont have enough mana to sacrifice to Caine.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
	send_to_char( "The room is already safe.\n\r", ch );
	return;
    }

    stc("You fall to your knees and pray silently to Baal to lift the accursed room.\n\r",ch);
    stc("The room is a safe haven.\n\r",ch);
    SET_BIT(ch->in_room->room_flags,ROOM_SAFE);

    if ( IS_SET(ch->in_room->bits,NON_SAFE) )
       REMOVE_BIT(ch->in_room->bits,NON_SAFE);

    ch->mana -= 2000000;
}


void do_vbite( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   int dam;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
   {
       send_to_char("Huh?\n\r",ch);
       return;
   }

   if ( !IS_SET(ch->polyaff, POLY_SERPENT) )
   {
       send_to_char("You aren't in serpent form.\n\r",ch);
       return;
   }

   if ( ( victim = ch->fighting ) == NULL )
   {
       send_to_char( "You aren't fighting anyone.\n\r", ch );
       return;
   }

   dam = 100000;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;

   dam = is_attack_type( ch, victim, dam );

   sprintf(buf,"Your bite hits $N incredibly hard! [%d]",dam);
   act(buf,ch,NULL,victim,TO_CHAR);
   sprintf(buf,"$n's bite hits you incredibly hard! [%d]",dam);
   act(buf,ch,NULL,victim,TO_VICT);
   sprintf(buf,"$n's bite hits $N incredibly hard! [%d]",dam);
   act(buf,ch,NULL,victim,TO_NOTVICT);
   send_to_char("\n\r",ch);

   dam = 100000;

   if ( number_percent() > 80 && !IS_SET(victim->more, MORE_POISON) )
      {
         SET_BIT(victim->more, MORE_POISON);
         stc("You suddenly feel very ill.\n\r",victim);
         act("$n suddenly feels very ill.",victim,NULL,NULL,TO_ROOM);
      }

   hurt_person(ch,victim,dam,TRUE);
}

void do_tongue_lash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   int dam;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
   {
       send_to_char("Huh?\n\r",ch);
       return;
   }

   if ( !IS_SET(ch->polyaff, POLY_SERPENT) )
   {
       send_to_char("You aren't in serpent form.\n\r",ch);
       return;
   }

   if ( ( victim = ch->fighting ) == NULL )
   {
       send_to_char( "You aren't fighting anyone.\n\r", ch );
       return;
   }

   dam = 150000;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;

   dam = is_attack_type( ch, victim, dam );

   sprintf(buf,"Your tongue lash hits $N incredibly hard! [%d]",dam);
   act(buf,ch,NULL,victim,TO_CHAR);
   sprintf(buf,"$n's tongue lash hits you incredibly hard! [%d]",dam);
   act(buf,ch,NULL,victim,TO_VICT);
   sprintf(buf,"$n's tongue lash hits $N incredibly hard! [%d]",dam);
   act(buf,ch,NULL,victim,TO_NOTVICT);
   send_to_char("\n\r",ch);

   dam = 150000;

   if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
      {
         victim->wait += number_range( 6, 12 );
         stc("You are stunned.\n\r",victim);
         act("$n is stunned.",victim,NULL,NULL,TO_ROOM);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person(ch,victim,dam,TRUE);
}

/* Frenzy */
void do_frenzy( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_CLASS(ch, CLASS_VAMPIRE) )
       {
          if ( ch->rage > ch->beast )
             {
                stc("Your rage is already too great.\n\r",ch);
                return;
             }

	    if (!IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) do_nightsight(ch,"");
	    if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    if (!IS_VAMPAFF(ch, VAM_CLAWS)) do_claws(ch,"");
	       send_to_char("You bare your fangs and growl as your inner beast consumes you.\n\r",ch);
	    act("$n bares $s fangs and growls as $s inner beast consumes $m.",ch,NULL,NULL,TO_ROOM);
	    ch->rage += 10000;
	    if (ch->beast > 0) do_beastlike(ch,"");
	    WAIT_STATE(ch,12);
  	    return;
    }
    else stc("Huh?\n\r",ch);
}
/* End of Frenzy */


/* Spiritguard */
void do_spiritguard( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
      {
       stc("Huh?\n\r",ch);
       return;
      }

    if ( !IS_SET(ch->flag2, AFF_SPIRITGUARD) )
      {
       stc("You awaken your spirit guardian.\n\r",ch);
       SET_BIT(ch->flag2, AFF_SPIRITGUARD);
       return;
      }

    else
      {
       stc("You dismiss your spirit guardian.\n\r",ch);
       REMOVE_BIT(ch->flag2, AFF_SPIRITGUARD);
       return;
      }
  return;
}
/* End of Spiritguard */


/* Embrace */
void do_embrace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->embracing != NULL )
    {
      stc("You are already embracing someone.\n\r",ch);
      return;
    }

    argument = one_argument(argument, arg);

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
    {
         send_to_char( "Whose blood do you wish to suck?\n\r", ch );
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
          send_to_char("You can't do this to yourself!\n\r",ch);
	    return;
       }

    if ( !IS_NPC(victim) )
       {
          send_to_char("You can only embrace mobs!\n\r",ch);
	    return;
       }

    if ( get_kingdom_num(victim) != -1 )
       {
          send_to_char("They are too powerful!\n\r",ch);
	    return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You leap towards $N sinking your teeth into $S neck!",ch,NULL,victim,TO_CHAR);
    act("$n leaps towards you sinking $s teeth into your neck!",ch,NULL,victim,TO_VICT);
    act("$n leaps towards $N sinking $s teeth into $S neck!",ch,NULL,victim,TO_NOTVICT);
    ch->embracing = victim;
    victim->embraced = ch;
}
/* Embrace */

/* Assassinate */
void do_assassinate( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int dam = 50000;

    if ( IS_CLASS(ch, CLASS_NINJA) )
       {
          do_ninja_assassinate(ch,argument);
          return;
       }

    send_to_char("Huh?\n\r",ch);
    return;

    if ( ch->fight_timer > 0 )
      {
       send_to_char("Not with a fight timer.\n\r",ch);
       return;
      }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
      {
       send_to_char( "Assassinate whom?\n\r", ch );
	 return;
      }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
	 send_to_char( "They aren't here.\n\r", ch );
	 return;
      }

    if ( victim == ch )
      {
       send_to_char( "How can you assassinate yourself?\n\r", ch );
	 return;
      }

    if (is_safe(ch,victim) == TRUE) return;

   WAIT_STATE(ch,8);

   sprintf(buf,"Your assassination hits $N incredibly hard! [#r#z%d#n]", dam);
   act(buf,ch,NULL,victim,TO_CHAR);
   sprintf(buf,"$n's assassination hits you incredibly hard! [#r#z%d#n]\n\r", dam);
   act(buf,ch,NULL,victim,TO_VICT);
   sprintf(buf,"$n's assassination hits $N incredibly hard! [#r#z%d#n]\n\r", dam);
   act(buf,ch,NULL,victim,TO_NOTVICT);

   hurt_person(ch,victim,dam,TRUE);
}
/* End of Assassinate */


/* Mindblast */
void do_mindblast( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam = 25000;

    argument = one_argument(argument,arg);

    if ( IS_NPC(ch) )
      return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE))
      {
       send_to_char("Huh?\n\r",ch);
       return;
      }

    if (arg[0] == '\0')
      {
       send_to_char("Mindblast Whom?\n\r",ch);
       return;
      }

    if ( ch->mana < 100000 )
      {
       send_to_char("You don't have enough magical energy for that.\n\r",ch);
       return;
      }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
       send_to_char( "They arent here.\n\r", ch );
	 return;
      }

    if ( is_safe(ch,victim) )
       return;

    ch->mana -= 100000;

    act("You close your eyes and concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n closes $s eyes and concentrates on you.",ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and concentrates on $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, 12 );

    if ( !IS_AWAKE(victim) )
	dam = 50000;

    act("You clutch your head in agony!",victim,NULL,NULL,TO_CHAR);
    act("$n clutches his head in agony!",victim,NULL,NULL,TO_ROOM);
    victim->hit = UMAX( 0, victim->hit - dam );
    update_pos( ch );
}
/* End of Mindblast */


/* Feed */
void do_feed( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->embracing != NULL )
    {
      stc("You are already embracing someone.\n\r",ch);
      return;
    }

    argument = one_argument(argument, arg);

    if (  arg[0] == '\0' )
    {
      send_to_char( "Whose blood do you wish to suck?\n\r", ch );
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
          send_to_char("You can't do this to yourself!\n\r",ch);
	    return;
       }


    if ( is_safe(ch,victim) )
       return;

    act("You leap towards $N sinking your teeth into $S neck!",ch,NULL,victim,TO_CHAR);
    act("$n leaps towards you sinking $s teeth into your neck!",ch,NULL,victim,TO_VICT);
    act("$n leaps towards $N sinking $s teeth into $S neck!",ch,NULL,victim,TO_NOTVICT);
    ch->embracing = victim;
    victim->embraced = ch;
}
/* Feed */

/*lamprey*/
void do_lamprey( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char buf[MAX_STRING_LENGTH];
 char arg [MAX_INPUT_LENGTH];
 int dam;

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 556 ) 
     return;

  if ( !IS_NPC(ch) )
  {
    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->move < 500000 )
       {
          send_to_char("You are too tired to execute this technique.\n\r",ch);
          return;
       }
  }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
 	   send_to_char( "Whom do you want to spit at?\n\r", ch );
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

 if ( is_safe(ch,victim) == TRUE )
    return;

 WAIT_STATE( ch, 18 );

 if ( !IS_NPC(ch) )
    ch->move -= 500000;

 dam = 1000000;

 ch->attack_type = ATTACK_PHYSICAL;
 ch->attack_var = HIT;
 ch->attack_focus = DIRECT;
 ch->ele_attack = ATTACK_DARK;

 dam = is_elemental( ATTACK_DARK, victim, dam );
 dam = is_attack_type( ch, victim, dam );

 sprintf(buf,"Your tendrils of darkness hits $N incredibly hard! [%d]\n\r",dam);
 act(buf,ch,NULL,victim,TO_CHAR);
 sprintf(buf,"$n's tendrils of darkness hits you incredibly hard! [%d]\n\r",dam);
 act(buf,ch,NULL,victim,TO_VICT);
 sprintf(buf,"$n's tendrils of darkness hits $N incredibly hard! [%d]\n\r",dam);
 act(buf,ch,NULL,victim,TO_NOTVICT);

 dam = 1000000;

 if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] < 10000 )
 {
    send_to_char("Your bloodlust is sated.\n\r",ch);
    ch->pcdata->condition[COND_THIRST] = 10000;
 }

 hurt_person(ch,victim,dam,TRUE);
}
/*End of Lamprey*/


/* Fear */
void do_fear( CHAR_DATA *ch, char *argument )
{
  do_soul_fear( ch, argument );
  return;
}
/* End of Fear */


/* Plasma */
void do_plasma( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if ( IS_NPC(ch) )
	return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
      {
  	 send_to_char("Huh?\n\r", ch);
	 return;
      }

    save_char_obj(ch);

    if ((obj = create_object(get_obj_index( 30007),60)) == NULL)
      {
       send_to_char( "Error - Please inform Oni.\n\r", ch);
	 return;
      }

    if ( IS_EXTRA(ch, EXTRA_PLASMA) )
      {
       ch->pcdata->obj_vnum = 0;
       REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
       REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
       REMOVE_BIT(ch->extra, EXTRA_PLASMA);
       ch->pcdata->chobj = NULL;
       obj->chobj = NULL;
       free_string(ch->morph);
       ch->morph = str_dup("");
      
       act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
       act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
       extract_obj( obj );
       return;
      }

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
      {
 	 send_to_char( "Not while polymorphed.\n\r", ch );
	 return;
      }

    if ( ch->fight_timer > 0 )
      {
	 send_to_char( "Not with a fight timer!\n\r", ch );
	 return;
      }

    act("$n transforms into $p and splashes to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and splashes to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = 30007;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    obj->timer = 12;

    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    SET_BIT(ch->extra, EXTRA_PLASMA);
    free_string(ch->morph);
    ch->morph = str_dup("a pool of blood");
    obj_to_room(obj,ch->in_room);
}
/* End of Plasma */


/* Flash */
void do_flash( CHAR_DATA *ch, char *argument )
{
   char       buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
      return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
      {
       send_to_char("Huh?\n\r", ch);
       return;
      }

    if ( ch->pkill < 5 )
       {
          send_to_char("You aren't powerful enough to do this yet.\n\r", ch);
          return;
       }

    if (IS_EXTRA(ch, EXTRA_FLASH))
      {
       send_to_char("You are already moving swiftly.\n\r", ch);
       return;
      }

    if (ch->pcdata->condition[COND_THIRST] < 5000)
      {
       send_to_char("You have insufficient blood.\n\r",ch);
       return;
      }

    SET_BIT(ch->extra, EXTRA_FLASH);
    ch->pcdata->condition[COND_THIRST] -= 5000;
    sprintf(buf, "%s starts moving with lightening speed.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("You start moving with lightening speed.\n\r",ch);
    return;
}
/* End of Flash */


/* Unveil */
void do_unveil( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
       stc("Huh?\n\r",ch);
       return;
      }

    if ( arg[0] == '\0' )
      {
	 send_to_char( "Whose mind do you wish to Unveil?\n\r", ch );
	 return;
      }

    if ( ( victim = get_char_area( ch, arg ) ) == NULL )
      {
	 send_to_char( "They aren't here.\n\r", ch );
	 return;
      }

    if ( victim == ch )
      {
	 stc("Why would you want to unveil your own mind?\n\r",ch);
	 return;
      }

    if ( IS_NPC( victim ) || IS_IMMORTAL( victim ) || IS_CLASS( victim, CLASS_VAMPIRE ) )
       {
          stc("Their mind is far too complex for you to Unveil.\n\r",ch);
          return;
       }

    victim->unveil = ch;
    ch->unveil = victim;
    act("You gaze deeply into $N's eyes.\n\rYou have unveiled $S mind.\n\r", ch,NULL,victim,TO_CHAR );
}
/* End of Unveil */

/* Change Forms */
void do_change( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];

    if ( IS_CLASS(ch, CLASS_SOULREAVER) )
       {
          do_soul_change( ch, argument );
          return;
       }

    argument = one_argument( argument, arg );

    if (IS_NPC(ch))
      return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DAYWALKER))
      {
	 send_to_char("Huh?\n\r",ch);
	 return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can change between 'human', 'bat', 'mist', 'serpent', 'soulreaver' and 'wolf' forms.\n\r", ch );
	return;
    }


    if ( !str_cmp(arg,"bat") )
      {
	if (ch->mounted == IS_RIDING)
        do_dismount(ch,"");

	act( "You transform into bat form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a bat.", ch, NULL, NULL, TO_ROOM );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
	SET_BIT(ch->polyaff, POLY_BAT);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);

      if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
         SET_BIT(ch->affected_by, AFF_POLYMORPH);

      if ( IS_SET(ch->polyaff, POLY_WOLF) )
         REMOVE_BIT(ch->polyaff, POLY_WOLF);

      if ( IS_SET(ch->polyaff, POLY_SOULREAVER) )
         REMOVE_BIT(ch->polyaff, POLY_SOULREAVER);

      if ( ch->fight_timer == 0 && IS_SET( ch->affected_by, AFF_ETHEREAL ) )
         REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

      if ( IS_SET(ch->polyaff, POLY_MIST) )
         REMOVE_BIT(ch->polyaff, POLY_MIST);

      if ( IS_SET(ch->polyaff, POLY_SERPENT) )
         REMOVE_BIT(ch->polyaff, POLY_SERPENT);

	sprintf(buf, "#e%s the #Rvam#wp#Rire #obat#n", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
     }

    else if ( !str_cmp(arg,"wolf") )
           {
	      if (ch->mounted == IS_RIDING)
              do_dismount(ch,"");

      	act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR );
	      act( "$n transforms into a large timberwolf.", ch, NULL, NULL, TO_ROOM );
	      SET_BIT(ch->polyaff, POLY_WOLF);

            if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
               SET_BIT(ch->affected_by, AFF_POLYMORPH);

	      SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);

            if ( IS_SET(ch->polyaff, POLY_SOULREAVER) )
               REMOVE_BIT(ch->polyaff, POLY_SOULREAVER);

            if ( IS_SET(ch->polyaff, POLY_BAT) )
               REMOVE_BIT(ch->polyaff, POLY_BAT);

            if ( IS_SET(ch->polyaff, POLY_MIST) )
               REMOVE_BIT(ch->polyaff, POLY_MIST);

            if ( IS_SET(ch->polyaff, POLY_SERPENT) )
               REMOVE_BIT(ch->polyaff, POLY_SERPENT);

	      sprintf(buf, "#R%s the #otim#cber#owolf#n", ch->name);
    	      free_string( ch->morph );
    	      ch->morph = str_dup( buf );
	      return;
         }

    else if ( !str_cmp(arg,"serpent") )
           {
	      if (ch->mounted == IS_RIDING)
              do_dismount(ch,"");

      	act( "You transform into a hideous serpent.", ch, NULL, NULL, TO_CHAR );
	      act( "$n transforms into a hideous serpent.", ch, NULL, NULL, TO_ROOM );
	      SET_BIT(ch->polyaff, POLY_SERPENT);

            if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
	         SET_BIT(ch->affected_by, AFF_POLYMORPH);

	      SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);

      if ( ch->fight_timer == 0 && IS_SET( ch->affected_by, AFF_ETHEREAL ) )
         REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

         if ( IS_SET(ch->polyaff, POLY_WOLF) )
            REMOVE_BIT(ch->polyaff, POLY_WOLF);

            if ( IS_SET(ch->polyaff, POLY_SOULREAVER) )
               REMOVE_BIT(ch->polyaff, POLY_SOULREAVER);

            if ( IS_SET(ch->polyaff, POLY_BAT) )
               REMOVE_BIT(ch->polyaff, POLY_BAT);

            if ( IS_SET(ch->polyaff, POLY_MIST) )
               REMOVE_BIT(ch->polyaff, POLY_MIST);

	      sprintf(buf, "#R%s the #ehideous #gser#npent#n", ch->name);
    	      free_string( ch->morph );
    	      ch->morph = str_dup( buf );
	      return;
         }
    else if ( !str_cmp(arg,"mist") )
        {
	   if (ch->mounted == IS_RIDING)
           do_dismount(ch,"");

	   act( "You transform into mist form.", ch, NULL, NULL, TO_CHAR );
	   act( "$n transforms into a white mist.", ch, NULL, NULL, TO_ROOM );
	   if (IS_EXTRA(ch, TIED_UP))
	     {
	      act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR);
	      act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	      REMOVE_BIT(ch->extra, TIED_UP);
	      REMOVE_BIT(ch->extra, GAGGED);
	      REMOVE_BIT(ch->extra, BLINDFOLDED);
	     }

	   if (is_affected(ch, gsn_web))
	     {
	      act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	      send_to_char("The webbing entrapping you falls through your ethereal form.\n\r",ch);
	      affect_strip(ch, gsn_web);
	     }

	   SET_BIT(ch->polyaff, POLY_MIST);
	   SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);

	   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
            SET_BIT(ch->affected_by, AFF_POLYMORPH);

         if ( IS_SET(ch->polyaff, POLY_WOLF) )
            REMOVE_BIT(ch->polyaff, POLY_WOLF);

        if ( IS_SET(ch->polyaff, POLY_SOULREAVER) )
           REMOVE_BIT(ch->polyaff, POLY_SOULREAVER);

        if ( IS_SET(ch->polyaff, POLY_BAT) )
           REMOVE_BIT(ch->polyaff, POLY_BAT);

         if ( IS_SET(ch->polyaff, POLY_SERPENT) )
            REMOVE_BIT(ch->polyaff, POLY_SERPENT);

	   sprintf(buf, "#e%s the #wwhite #emist#n", ch->name);
    	   free_string( ch->morph );
    	   ch->morph = str_dup( buf );
	   return;
      }

    else if ( !str_cmp(arg,"human") )
           {
            if (!IS_AFFECTED(ch, AFF_POLYMORPH))
	        {
	         send_to_char( "You are already in human form.\n\r", ch );
	         return;
	        }

	      if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
	        {
	         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
	         REMOVE_BIT(ch->polyaff, POLY_BAT);
	        }

     else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
             REMOVE_BIT(ch->polyaff, POLY_WOLF);
     else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_SOULREAVER))
	      {
	         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
            REMOVE_BIT(ch->polyaff, POLY_SOULREAVER);
	      }
     else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_SERPENT))
	       REMOVE_BIT(ch->polyaff, POLY_SERPENT);

     else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
	       REMOVE_BIT(ch->polyaff, POLY_MIST);
     else
	   {
	    /* In case they try to change to human from a non-vamp form */
	    send_to_char( "You seem to be stuck in this form.\n\r", ch );
	    return;
	   }

	act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);

  	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }

   else
	send_to_char( "You can change between 'human', 'bat', 'wolf', 'serpent', 'mist' forms.\n\r", ch );
    return;
}
/* End of Change Forms */


/* Shroud */
void do_shroud( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   argument = one_argument (argument, arg);

   if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
         stc( "Huh?\n\r", ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         send_to_char("You aren't powerful enough to do this yet.\n\r", ch);
         return;
      }

   if ( ch->vampirestuff > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   send_to_char( "You concentrate on your surroundings as shadows rise from the ground, filling the area.\n\r", ch );
   sta("#eThe sun is suddenly block out by a mysterious force; you don't know if it is #yday #eor #Lnight#e.#n\n\r",ch->in_room->area);
   ch->vampirestuff = 80;

   if ( IS_SET( ch->in_room->area->bits,WASTE ) || IS_SET( ch->in_room->area->bits,UPDATEWASTE ) )
      {
          if ( IS_SET( ch->in_room->area->bits,WASTE ) )
             REMOVE_BIT( ch->in_room->area->bits, WASTE );

          if ( IS_SET( ch->in_room->area->bits,UPDATEWASTE ) )
             REMOVE_BIT( ch->in_room->area->bits, UPDATEWASTE );

          sta("#oThe land around you shifts into something more recognizable.#n\n\r",ch->in_room->area);
      }
 
   if ( !IS_SET(ch->in_room->area->bits,ROOM_TOTAL_DARKNESS) )
      SET_BIT(ch->in_room->area->bits,ROOM_TOTAL_DARKNESS);

   if ( ch->pkill > 19 && number_percent() > 74 && weather_info.sky != SKY_ECLIPSE 
        && weather_info.sky != SKY_APOCALYPSE )
      weather_info.sky = SKY_ECLIPSE;
}
/* End of shroud */


void do_tongue( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   int dam;

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 15501 && ch->pIndexData->vnum != 15502 ) 
     return;

  if ( !IS_NPC(ch) )
  {
    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->polyaff, POLY_SERPENT) )
    {
       send_to_char("You aren't in serpent form.\n\r",ch);
       return;
    }
  }

   if ( ( victim = ch->fighting ) == NULL )
   {
       send_to_char( "You aren't fighting anyone.\n\r", ch );
       return;
   }

   WAIT_STATE( ch, 18 );
   dam = number_range( 25000, 35000 );
   sprintf(buf,"Your tongue hits $N incredibly hard! [%d]\n\r",dam);
   act(buf,ch,NULL,victim,TO_CHAR);
   sprintf(buf,"$n's tongue hits you incredibly hard! [%d]\n\r",dam);
   act(buf,ch,NULL,victim,TO_VICT);
   sprintf(buf,"$n's tongue hits $N incredibly hard! [%d]\n\r",dam);
   act(buf,ch,NULL,victim,TO_NOTVICT);
   send_to_char("\n\r",ch);

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person(ch,victim,dam,TRUE);
}
/* Tongue */


/* Withering */
void do_withering (CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];

    argument = one_argument (argument, arg);


    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
      {
       send_to_char("Huh?\n\r",ch);
       return;
      }

    if ( arg[0] == '\0' )
      {
       send_to_char("Who do you wish to wither?\n\r",ch);
       return;
      }

    victim = get_char_room(ch, arg);
    if ( ( victim = get_char_room( ch, arg) ) == NULL )
      {
       send_to_char("They arent here.\n\r",ch);
       return;
      }

   if ( is_safe(ch,victim) )
      return;

   if ( IS_CLASS(victim, CLASS_PALADIN) || IS_CLASS(victim, CLASS_SORCEROR) )
      {
         stc("They break your concentration.\n\r",ch);
         return;
      }

   ch->wait += 18;

   sprintf(buf,"A look of concentration crosses over %s's face.",ch->name);
   act(buf,ch,NULL,NULL,TO_ROOM);
   send_to_char("A look of concentration crosses over your face.\n\r",ch);
   if (IS_ARM_R(victim,LOST_ARM) && IS_ARM_L(victim,LOST_ARM) &&
       IS_LEG_R(victim,LOST_LEG) && IS_LEG_L(victim,LOST_LEG))
     {
      send_to_char("All of their limbs are severed already!\n\r",ch);
      return;
     }

   if (number_percent ( ) < 20 )
     {
      if (!IS_LEG_R(victim,LOST_LEG))
        {
         SET_BIT(victim->loc_hp[5],LOST_LEG);
         if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL )
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
            take_item(victim,obj);

         make_part(victim,"leg");
         sprintf(buf,"A supernatural force rips %s's leg off!",victim->name);
         act(buf,ch,NULL,NULL,TO_ROOM);
         send_to_char(buf,ch);
         return;
        }

      if (!IS_ARM_L(victim,LOST_ARM))
        {
         SET_BIT(victim->loc_hp[2],LOST_ARM);
         if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL )
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
            take_item(victim,obj);

         make_part(victim,"arm");
         sprintf(buf,"A supernatural force rips %s's arm off!",victim->name);
         act(buf,ch,NULL,NULL,TO_ROOM);
         send_to_char(buf,ch);
         return;
        }

      if (!IS_LEG_L(victim,LOST_LEG))
        {
         SET_BIT(victim->loc_hp[4],LOST_LEG);
         if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL )
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
            take_item(victim,obj);

         make_part(victim,"leg");
         sprintf(buf,"A supernatural force rips %s's leg off!",victim->name);
         act(buf,ch,NULL,NULL,TO_ROOM);
         send_to_char(buf,ch);
         return;
        }

      if (!IS_ARM_R(victim,LOST_ARM))
        {
         SET_BIT(victim->loc_hp[3],LOST_ARM);
         if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL )
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
            take_item(victim,obj);

         if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
            take_item(victim,obj);

         make_part(victim,"arm");
         sprintf(buf,"A supernatural force rips %s's arm off!",victim->name);
         act(buf,ch,NULL,NULL,TO_ROOM);
         send_to_char(buf,ch);
         return;
        }

     }

 send_to_char("You failed.\n\r",ch);
 return;
}
/* End of Withering */


/* Putrefaction */
void do_rot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
      {
	 send_to_char( "Huh?\n\r", ch );
	 return;
      }

    if ( arg[0] == '\0' )
      {
       send_to_char( "Who do you wish to use Putrefaction on?\n\r", ch );
	 return;
      }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
	 send_to_char( "They aren't here.\n\r", ch );
	 return;
      }

    if (IS_NPC(victim))
      {
       send_to_char("Why use Putrefaction on a mob?\n\r", ch);
       return;
      }

    if ( IS_IMMORTAL(victim) && victim != ch )
      {
       send_to_char( "You can only use Putrefaction on Avatar's or lower.\n\r", ch );
	 return;
      }

    if (is_safe(ch,victim) == TRUE) return;

    act("You close your eyes and concentrate on $N.", ch,NULL,victim,TO_CHAR);
    act("$n closes $s eyes and concentrates on you.", ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and cencentrates on $N.", ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE(ch, 12);

    if (number_percent( ) < 50)
      {
       send_to_char("You failed.\n\r",ch);
       return;
      }

    act("Your flesh begins to rot!", victim,NULL,NULL,TO_CHAR);
    act("$n's flesh begins to rot!", victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->extra, EXTRA_ROT);
}
/* End of Putrefaction */


/* Drainlife */
void do_drain( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int bonus = 50000;

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 556 ) 
       return;

    if ( !IS_NPC(ch) )
       {
          if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
             {
                send_to_char("Huh?\n\r",ch);
                return;
             }

          if ( ch->move < 250000 )
             {
                send_to_char("You are too tired to use this technique.\n\r",ch);
                return;
             }
       }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
 	   send_to_char( "Whom do you want to drain?\n\r", ch );
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

   if ( is_safe(ch, victim) )
      return;

   if ( IS_NPC( ch ) )
      bonus = 300000;
      else if ( !IS_NPC( ch ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_SOUL )
              {
                 ch->move -= 250000;
                 bonus = 50000;
              }

   hit_gain( ch, bonus + UMIN( 50000, ch->pkill * 2500 ) );
   WAIT_STATE( ch, 12 );

   act("You drain the lifeforce out of $N.", ch, NULL, victim, TO_CHAR);
   act("$n drains the lifeforce out of $N.", ch, NULL, victim, TO_NOTVICT);
   act("You feel $n draining your lifeforce.", ch, NULL, victim, TO_VICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;

   if ( !IS_NPC(ch) )
      hurt_person( ch, victim, 700000, TRUE );
      else hurt_person( ch, victim, 850000, TRUE );
}
/* End of Drainlife */


/* Physiccrush */
void do_physic_crush( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 15501 && ch->pIndexData->vnum != 15502 ) 
     return;

  if ( !IS_NPC(ch) )
  {
    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
   	 stc("Huh?\n\r",ch);
	    return;
      }

    if ( ch->pkill < 10 )
       {
          send_to_char("You aren't powerful enough to do this yet.\n\r", ch);
          return;
       }

    if ( ch->vlife[CURRENT] < 50 )
      {
	 stc("You need at least 50 focus points to use this power.\n\r", ch);
	 return;
      }

    if ( ch->move < 1000000 )
      {
  	 stc("You are too tired to do this technique.\n\r", ch);
	 return;
      }
  }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
       stc("Whose lifeforce do you wish to crush?\n\r",ch);
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

    if (is_safe(ch,victim) ) return;

    if ( ( victim == ch ) )
      {
       stc("That doesn't seem like a good idea.\n\r",ch);
	 return;
      }

    act("You close your eyes as you quickly raise your right hand into the air.",ch,NULL,NULL,TO_CHAR);
    act("$n closes $s eyes as $e quickly raises $s right hand into the air.",ch,NULL,NULL,TO_ROOM);
    act("$n screams loudly as $s very body implodes from the searing kinetic energy.",victim,NULL,NULL,TO_ROOM);
    act("You scream loudly as your very body implodes from the searing kinetic energy.",victim,NULL,NULL,TO_CHAR);

    if ( !IS_NPC(ch) )
       {
          ch->vlife[CURRENT] -= 50;
          ch->move -= 1000000;
       }

    ch->attack_type = ATTACK_MAGICAL;
    ch->attack_var = SOUL;
    ch->attack_focus = DIRECT;

    ch->wait = 6;
    hurt_person( ch, victim, 650000, TRUE );
}
/* End of Physiccrush */


/* Infirmity */
void do_infirmity( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char  arg[MAX_INPUT_LENGTH];
    char  buf[MAX_INPUT_LENGTH];
    int   sn;

    argument = one_argument (argument, arg);

    if ( IS_NPC(ch) )
      return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
      {
       send_to_char("Huh?\n\r", ch);
       return;
      }

    if (arg[0] == '\0')
	{
       send_to_char("Who do you wish to use Infirmity on?\n\r",ch);
	 return;
	}

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
	 send_to_char( "They aren't here.\n\r", ch );
	 return;
      }

    if (is_safe(ch,victim) == TRUE)
      return;

    if ( ( sn = skill_lookup( "infirmity" ) ) < 0 )
      {
       sprintf(buf,"Yep, sn is being set to %d.",sn);
       send_to_char(buf,ch);
       return;
      }

    if ( is_affected( victim, skill_lookup("blessing of the saints") ) )
       {
	    stc("They are protected by the holy saints.\n\r", ch);
	    return;
       }

    if ( IS_AFFECTED(victim, AFF_INFIRMITY) )
       {
	    stc("They are already weaken.\n\r", ch);
	    return;
       }

    act( "You gently place your hand on $N's forehead, weakening $M with a deadly curse.", ch, NULL, victim, TO_CHAR );

    (*skill_table[sn].spell_fun) ( sn, 3, ch, victim );
    WAIT_STATE( ch, 12 );
}
/* End of Infirmity */


/* Physicdrain */
void do_physic_drain( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
      {
	 stc("Huh?\n\r",ch);
	 return;
      }

    if ( ch->vlife[CURRENT] < 20 )
      {
	 stc("You need at least 20 focus points to use this power.\n\r", ch);
	 return;
      }

     if ( ch->mana < 50000 )
       {
  	  stc("You don't have enough magical energy for that.\n\r", ch);
	  return;
       }

     if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
        stc("Whose lifeforce do you wish to drain?\n\r",ch);
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

    if (is_safe(ch,victim) ) return;

    if ( ( victim == ch ) )
      {
	 stc("That doesn't seem like a good idea.\n\r",ch);
	 return;
      }

    act("You hover silently towards $N emitting palish energy from within you.",ch,NULL,victim,TO_CHAR);
    act("$n hovers silently towards you emitting palish energy from within $m.",ch,NULL,victim,TO_VICT);
    act("$n hovers silently towards $N emitting palish energy from within $m.",ch,NULL,victim,TO_NOTVICT);
    act("You place your hand on $N's arm as you close your eyes and focus on $S kinetic energy.",ch,NULL,victim,TO_CHAR);
    act("$n places $s hand on your arm as $e closes $s eyes and focuses on your kinetic energy.",ch,NULL,victim,TO_VICT);
    act("$n places $s hand on $N's arm as $e closes $s eyes and focuses on $S kinetic energy.",ch,NULL,victim,TO_NOTVICT);
    act("$N turns blue as some of $s lifeforce is drained into your body.",ch,NULL,victim,TO_CHAR);
    act("You turn blue as some of your lifeforce is drained into $n's body.",ch,NULL,victim,TO_VICT);
    act("$N turns blue as some of $s lifeforce is drained into $n's body.",ch,NULL,victim,TO_NOTVICT);

    if ( number_percent() > 69 && !IS_SET(victim->more, MORE_PAIN) )
       SET_BIT(victim->more, MORE_PAIN);

    ch->vlife[CURRENT] -= 20;
    ch->hit = UMIN( ch->hit + 20000, ch->max_hit );
    ch->mana -= 50000;
    ch->ele_attack = ATTACK_THUNDER;
    ch->attack_type = ATTACK_MENTAL;
    hurt_person(ch,victim,20000,TRUE);
    ch->wait = 8;
}
/* End of Physicdrain */


/* Maelstrom */
void do_maelstrom( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
     {
      stc("Huh?\n\r",ch);
      return;
     }

   if ( ch->pkill < 10 )
      {
         send_to_char("You aren't powerful enough to do this yet.\n\r", ch);
         return;
      }

   if ( ch->vlife[CURRENT] < 50 )
     {
      stc("You don't have enough kinetic energy to do this.\n\r",ch);
      return;
     }

   if ( ch->mana < 1000000 )
     {
      stc("You don't have enough magical energy to do this.\n\r",ch);
      return;
     }

   ch->mana -= 1000000;
   ch->vlife[CURRENT] -= 50;

   act( "As $n glides $s hands down $s body, a mass of energy manifests into existence.",ch,NULL,NULL,TO_ROOM);
   act( "As you glide your hands down your body, a mass of energy manifests into existence.",ch,NULL,NULL,TO_CHAR);
   act("The mass of energy transforms into an ancient symbol as demonic light flashes through it.",ch,NULL,NULL,TO_ROOM);
   act("The mass of energy transforms into an ancient symbol as demonic light flashes through it.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, the symbol blackens as it leaps from within its blinding dimension.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, the symbol blackens as it leaps from within its blinding dimension.",ch,NULL,NULL,TO_CHAR);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
       vch_next = vch->next_in_room;

       if ( is_same_group(ch, vch) )
          continue;

       if ( is_safe(ch, vch) )
          continue;

       dam = 500000;
       ch->attack_type = ATTACK_MAGICAL;
       ch->attack_var = SOUL;
       ch->attack_focus = DIRECT;
       act("You roar in pain as the symbol sears your mortal flesh!",vch,NULL,NULL,TO_CHAR);
       act("$n roars in pain as the symbol sears $s flesh!",vch,NULL,NULL,TO_ROOM);
       hurt_person(ch,vch,dam,TRUE);
   }
}
/* End of Maelstrom */


/* Earthmeld */
void do_earthmeld( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
      {
 	 send_to_char("Huh?\n\r",ch);
	 return;
      }

    if ( IS_SET(ch->more, MORE_WALL) )
    {
       stc("You release your concentration as the earth surrounding you drops to the floor.\n\r",ch);
       act("$n releases $s concentration as the earth surrounding $m drops to the floor.",ch,NULL,NULL,TO_ROOM);
       REMOVE_BIT(ch->more, MORE_WALL);
       return;
    }

    stc("You concentrate as the earth beneath your feet raises and forms a protective barrier around you.\n\r",ch);
    act("$n concentrates as the earth beneath $s feet raises and forms a protective barrier around $m.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->more, MORE_WALL);
}
/* End of Earthmeld */


/* Renew */
void do_renew(CHAR_DATA *ch, char *argument)
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_STRING_LENGTH];
    int       sn, check, oldcheck;

    argument = one_argument(argument, arg);

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
      {
       send_to_char("Huh?\n\r", ch);
       return;
      }

    if ( ch->pkill < 5 )
       {
          send_to_char("You aren't powerful enough to do this yet.\n\r", ch);
          return;
       }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
      {
       send_to_char("Not while you are mortally wounded!.\n\r", ch);
       return;
      }

    if (ch->obeah_timer> 0)
      {
       if (ch->obeah_timer > 1)
         sprintf(buf,"You cannot call upon your Obeah powers for another %d seconds.\n\r",ch->obeah_timer);
       else
         sprintf(buf,"You cannot call upon your Obeah powers for another 1 second.\n\r");
       send_to_char(buf,ch);
       return;
      }

    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
         ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5] +
         ch->loc_hp[6]) == 0 && ch->hit == ch->max_hit)
      {
       send_to_char("You don't require any healing.\n\r",ch);
       return;
      }

   if ( ch->pcdata->condition[COND_THIRST] < 700 )
   	{
	 send_to_char("You have insufficient blood.\n\r",ch);
	 return;
    	}
  else
    {
    ch->pcdata->condition[COND_THIRST] -= 700;
    act("$n's third eye begins to glow with an intense white light.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your third eye begins to glow with an intense white light.\n\r", ch);
    ch->obeah_timer = 40;

    if ((check = ch->loc_hp[6]) > 0)
      {
       oldcheck = 0;
       sn = skill_lookup("clot");
       while (oldcheck != (check = ch->loc_hp[6]))
            {
             oldcheck = check;
             (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
            }
      }

    if ((check = (ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
                  ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5])) > 0)
      {
       oldcheck = 0;
       while (oldcheck != (check = ch->loc_hp[0] + ch->loc_hp[1] +
                   ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] +
                   ch->loc_hp[5]))
            {
             oldcheck = check;
             reg_mend(ch);
            }
      }

    ch->hit = UMIN( ch->hit + 1000000, ch->max_hit );
    move_gain( ch, 1000000 );

    send_to_char("Your wounds are healed greatly!\n\r",ch);
    update_pos(ch);
    ch->wait = 4;
   }
}
/* End of Renew */


void do_soul_strike( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) )
     return;

  argument = one_argument( argument, arg );

  if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
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

  if ( is_safe(ch,victim) ) return;

  act("You leap towards $N with your fangs extracted!",ch,NULL,victim,TO_CHAR);
  act("$n leaps towards you with $s fangs extracted!",ch,NULL,victim,TO_VICT);
  act("$n leaps towards $N with $s fangs extracted!",ch,NULL,victim,TO_NOTVICT);
  act("Grinning evilly, you grab $N by the collar lifting $M in the air!",ch,NULL,victim,TO_CHAR);
  act("Grinning evilly, $n grabs you by the collar lifting you in the air!",ch,NULL,victim,TO_VICT);
  act("Grinning evilly, $n grabs $N by the collar lifting $M in the air!",ch,NULL,victim,TO_NOTVICT);
  act("Your eyes glow red as your SoulReaver starts pulsing with energy.",ch,NULL,NULL,TO_CHAR);
  act("$n's eyes glow red as $s SoulReaver starts pulsing with energy.",ch,NULL,NULL,TO_ROOM);
  act("You suddenly rams your claws through $N's chest!",ch,NULL,victim,TO_CHAR);
  act("$n suddenly rams $s claws through your chest!",ch,NULL,victim,TO_VICT);
  act("$n suddenly rams $s claws through $N's chest!",ch,NULL,victim,TO_NOTVICT);
  act("Your body trembles with adrenaline as $N's life energy seeps through you!!!",ch,NULL,victim,TO_CHAR);
  act("$n's body trembles with adrenaline as your life energy seeps through $m!!!",ch,NULL,victim,TO_VICT);
  act("$n's body trembles with adrenaline as $N's life energy seeps through $m!!!",ch,NULL,victim,TO_NOTVICT);

  hit_gain( ch, 1000000 );

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = SOUL;
  ch->attack_focus = UPPERCUT;
  hurt_person(ch,victim,2000000,TRUE);
}


/* Glide */
void do_glide( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->polyaff, POLY_SOULREAVER) )
      {
       send_to_char("You aren't in soulreaver form.\n\r",ch);
       return;
      }

    if ( ch->fight_timer > 0 )
       {
          send_to_char("not with a fight timer.\n\r",ch);
          return;
       }

    if (ch->move < 40000)
	{
	    stc("You are too tired to do this!\n\r",ch);
	    return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Whom to you want to swoop on?\n\r", ch );
	return;
    }

    if ( ( ( victim = get_char_area( ch, arg ) ) == NULL )
         || ( victim->in_room == NULL ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }

    if (victim->in_room->vnum == ch->in_room->vnum)
    {
	  send_to_char("But you're already there!\n\r",ch);
        return;
    }

    if ( in_kingdom( ch, victim, victim->in_room ) )
       return;

    stc("You flap your wings noisily as you zoom upwards in the sky.\n\r",ch);
    act("$n flaps $s wings noisily as $e zooms upwards in the sky.\n\r",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    do_look(ch, "auto");
    stc("Circling around the vicinity, you charge straight down onto your target.\n\r",ch);
    act("$n charges from the sky straight into $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n charges from the sky straight into you.",ch,NULL,victim,TO_VICT);
    ch->move -= 40000;
    ch->wait = 12;
    multi_hit( ch, victim, TYPE_UNDEFINED );

    if ( victim == NULL || victim->position != POS_FIGHTING ) return;

    multi_hit( ch, victim, TYPE_UNDEFINED );

    if ( victim == NULL || victim->position != POS_FIGHTING ) return;

    multi_hit( ch, victim, TYPE_UNDEFINED );
}


/* Baal */
void do_baal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *vch,*vch_next;

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 15501 && ch->pIndexData->vnum != 15502 ) 
     return;

  if ( !IS_NPC(ch) )
  {
    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
    {
     send_to_char("Huh?\n\r",ch);
     return;
    }

    if ( ch->ctimer[0] > 0 )
       {
          stc("Baal doesn't feel like coming at the moment.\n\r",ch);
          return;
       }

    if ( ch->pcdata->condition[COND_THIRST] < 1000 )
       {
          send_to_char("You have insufficient blood.\n\r", ch);
          return;
       }

    if (ch->mana < 100000)
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    ch->pcdata->condition[COND_THIRST] -= 1000;
    ch->mana -= 100000;
    ch->ctimer[0] = 12;
  }

   act("You say, '#yFor all that is evil in this World ... Come O Harbringer of Evil!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yFor all that is evil in this World ... Come O Harbringer of Evil!#n'",ch,NULL,NULL,TO_ROOM);
   act("The sky turns dark as thunder and lightning devastate the area, heavy rain clouding your vision.",ch,NULL,NULL,TO_CHAR);
   act("The sky turns dark as thunder and lightning devastate the area, heavy rain clouding your vision.",ch,NULL,NULL,TO_ROOM);
   act("A lone fork lightning strikes the earth and splitting it open to release a darkness.",ch,NULL,NULL,TO_CHAR);
   act("A lone fork lightning strikes the earth and splitting it open to release a darkness.",ch,NULL,NULL,TO_ROOM);
   act("The figure say, '#RWho dares to call me forth from the realms of Hell!#n'",ch,NULL,NULL,TO_CHAR);
   act("The figure say, '#RWho dares to call me forth from the realms of Hell!#n'",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yI called you forth and I command you to rid me of my foes!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yI called you forth and I command you to rid me of my foes!#n'",ch,NULL,NULL,TO_ROOM);

   act("Baal's eyes blazes with #Rdemonic fire#n as it stares deep into your soul.",ch,NULL,NULL,TO_CHAR);
   act("Baal's eyes blazes with #Rdemonic fire#n as it stares deep into your soul.",ch,NULL,NULL,TO_ROOM);
   act("You shuffle back as a satanic smirk cross Baal's face.",ch,NULL,NULL,TO_CHAR);
   act("You shuffle back as a satanic smirk cross Baal's face.",ch,NULL,NULL,TO_ROOM);
   act("Baal grins evilly as it rushes straight into its target!",ch,NULL,NULL,TO_CHAR);
   act("Baal grins evilly as it rushes straight into its target!",ch,NULL,NULL,TO_ROOM);
   act("Baal claws #RVICIOUSLY#n to the left as palish energy shimmers after its nails!",ch,NULL,NULL,TO_CHAR);
   act("Baal claws #RVICIOUSLY#n to the left as palish energy shimmers after its nails!",ch,NULL,NULL,TO_ROOM);
   act("Baal swipes #yPOWERFULLY#n to the right as palish energy shimmers after its nails!",ch,NULL,NULL,TO_CHAR);
   act("Baal swipes #yPOWERFULLY#n to the right as palish energy shimmers after its nails!",ch,NULL,NULL,TO_ROOM);
   act("Baal scratches #RVICIOUSLY#n to the left as palish energy shimmers after its nails!",ch,NULL,NULL,TO_CHAR);
   act("Baal scratches #RVICIOUSLY#n to the left as palish energy shimmers after its nails!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) )
         continue;

      if ( is_safe(ch,vch) )
         continue;

      if ( IS_IMMORTAL(vch) )
         continue;

      if ( ch == vch )
         continue;

      hurt_person(ch,vch,50000,TRUE);
  }

  act("Baal dashes into a rift, cackling wildly.",ch,NULL,NULL,TO_CHAR);
  act("Baal dashes into a rift, cackling wildly.",ch,NULL,NULL,TO_ROOM);

  WAIT_STATE(ch,12);
}


/*Grab*/
void do_grab(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument,arg);

    if (IS_NPC(ch))
       return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE))
      {
       send_to_char("Huh?\n\r",ch);
       return;
      }

   if ( arg[0] == '\0' )
     {
      send_to_char( "Who do you wish to Grab?\n\r", ch );
	return;
     }

   if ((victim = get_char_room( ch, arg)) == NULL)
      {
       send_to_char("They arent here.\n\r",ch);
       return;
      }

   if ( ch->fight_timer > 0 && IS_NPC( victim ) )
      {
        send_to_char("You can't use this skill to mobs with your fight timer.\n\r",ch);
        return;
      }

   if ( victim->fight_timer > 0 )
      {
        send_to_char("They have a fight timer.\n\r",ch);
        return;
      }

   if ( !IS_AFFECTED(victim, AFF_SHADOWPLANE))
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


/* Vsilence */
void do_death( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *inroom;
    inroom= ch->in_room;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
       {
          send_to_char("Huh?\n\r", ch);
          return;
       }

    if ( RTIMER(ch->in_room, RTIMER_SILENCE) != 0 )
       {
          sprintf(buf,"The silence leaves the room.\n\r");
          act(buf,ch,NULL,NULL,TO_ROOM);
          send_to_char("The silence leaves the room.\n\r", ch);
          SET_RTIMER(ch->in_room, RTIMER_SILENCE, 0);
          return;
       }

    if ( ch->pcdata->condition[COND_THIRST] < 300 )
       {
          send_to_char("You have insufficient blood.\n\r", ch);
          return;
       }

    ch->pcdata->condition[COND_THIRST] -= 300;
    SET_RTIMER(inroom, RTIMER_SILENCE, 10);
    sprintf(buf,"A look of concentration passes over %s's face.\n\r", ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    sprintf(buf,"An eerie silence fills the room.\n\r");
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
    return;
}
/* End of Vsilence */


/* Coffin */
void do_coffin( CHAR_DATA *ch, char *argument )
{
    char	  arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);


    if ( IS_NPC(ch) )
      return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
	 {
	    send_to_char("Huh?", ch);
	    return;
	 }

    act("You cut your wrist and close your eyes as you suck deeply on your blood.",ch,NULL,NULL,TO_CHAR);
    act("$n cut $s wrist and close $s eyes as $n suck deeply on $s blood.",ch,NULL,NULL,TO_ROOM);
    act("A shadow passes over your face and you feel faint.",ch,NULL,NULL,TO_CHAR);
    act("A shadow passes over $s face and $n looks faint.",ch,NULL,NULL,TO_ROOM);
    clear_affects( ch );
    ch->wait = 48;
}
/* End of Coffin */


/* Tears of Reality */
void do_tears_reality( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   char      buf [MAX_STRING_LENGTH];
   int dam = 1500000;

   if (IS_NPC(ch)) return;

   if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
      {
          send_to_char( "Huh?\n", ch );
          return;
      }

   if ( ch->mana < 3000000 )
      {
         send_to_char("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ch->obeah_timer > 0 )
      {
         if (ch->obeah_timer > 1)
            sprintf(buf,"You cannot call upon your Obeah powers for another %d seconds.\n\r",ch->obeah_timer);
            else sprintf(buf,"You cannot call upon your Obeah powers for another 1 second.\n\r");

         send_to_char(buf,ch);
         return;
      }

   ch->mana -= 3000000;
   act("A tear drop falls out of your eyes landing on your finger tips.",ch,NULL,NULL,TO_CHAR);
   act("A tear drop falls out of $n's eyes landing on $s finger tips.",ch,NULL,NULL,TO_ROOM);
   act("Muttering prayers to the Serpent Gods, you suddenly RaM your claws through your body.",ch,NULL,NULL,TO_CHAR);
   act("Muttering prayers to the Serpent Gods, $n suddenly RaMs $s claws through $s body.",ch,NULL,NULL,TO_ROOM);
   act("Explosive energy JoLts out of your body engulfing everyone in the vicinity.",ch,NULL,NULL,TO_CHAR);
   act("Explosive energy JoLts out of $n's body engulfing everyone in the vicinity.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
     vch_next = vch->next_in_room;

     if ( vch == ch )
        continue;

     if ( !is_same_group(ch,vch) || nogang != 0 || !can_heal(ch,vch) )
        continue;

     act("You feel totally refresh as the soothing light bathe your tired body.",vch,NULL,NULL,TO_CHAR);
     act("$n feels totally refresh as the soothing light bathe $s tired body.",vch,NULL,NULL,TO_ROOM);
     vch->hit = UMIN( vch->max_hit, vch->hit + 3000000 );
     update_pos(vch);
   }

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;

        if ( is_same_group(ch,vch) )
           continue;

        if ( vch == ch )
           continue;

        if ( is_safe(ch, vch) )
           continue;

        ch->ele_attack = ATTACK_THUNDER;
        ch->attack_type = ATTACK_MAGICAL;
        ch->attack_var = BLAST;
        ch->attack_focus = UPPERCUT;
        hurt_person(ch,vch,dam,TRUE);
   }

   stop_fighting( ch, TRUE );

   ch->hit = -2000010;
   ch->position = POS_MORTAL;
   ch->obeah_timer = 40;
}
/* End of Tears of Reality */

void do_bloodwall(CHAR_DATA *ch, char *argument)
{
   OBJ_DATA *obj = NULL;
   OBJ_DATA *objc;
   char arg[MAX_STRING_LENGTH];
   char wall[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];

   argument = one_argument(argument,arg);

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->condition[COND_THIRST] < 100 )
      {
         send_to_char("You don't have enough blood for that.\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' || ( str_cmp(arg,"n") && str_cmp(arg,"s") && str_cmp(arg,"e")
        && str_cmp(arg,"w") && str_cmp(arg,"u") && str_cmp(arg,"d") ) )
      {
         send_to_char("You may wall n, w, s, e, d or u.\n\r",ch);
         return;
      }

   ch->pcdata->condition[COND_THIRST] -= 100;
   sprintf(buf,"A look of concentration passes over %s's face.",ch->name);
   act(buf,ch,NULL,NULL,TO_ROOM);
   send_to_char("A look of concentration passes over your face.\n\r",ch);

   if ( !str_cmp(arg,"n") )
      sprintf(wall,"walln");

   if ( !str_cmp(arg,"w") )
      sprintf(wall,"wallw");

   if ( !str_cmp(arg,"s") )
      sprintf(wall,"walls");

   if ( !str_cmp(arg,"e") ) 
      sprintf(wall,"walle");

   if ( !str_cmp(arg,"d") )
      sprintf(wall,"walld");

   if ( !str_cmp(arg,"u") )
      sprintf(wall,"wallu");

   objc = get_obj_list(ch,wall,ch->in_room->contents);

   if ( objc != NULL)
      {
         send_to_char("There is already a wall blocking that direction.\n\r",ch);
         return;
      }

   WAIT_STATE(ch,8);
   sprintf(buf,"A wall of blood pours out of the ground.");
   act(buf,ch,NULL,NULL,TO_ROOM);
   send_to_char(buf,ch);

   if ( !str_cmp(arg,"n") ) 
      {
         obj=create_object(get_obj_index(30043),0);
         sprintf(buf,"A wall of blood is here, blocking your exit north.");
         obj->short_descr = str_dup(buf);
      }

   if ( !str_cmp(arg,"s") )
      {
         obj=create_object(get_obj_index(30044),0);
         sprintf(buf,"A wall of blood is here, blocking your exit south.");
         obj->short_descr = str_dup(buf);
      }

   if ( !str_cmp(arg,"e") )
      {
         obj=create_object(get_obj_index(30045),0);
         sprintf(buf,"A wall of blood is here, blocking your exit east");
         obj->short_descr = str_dup(buf);
      }

   if ( !str_cmp(arg,"w") )
      {
         obj=create_object(get_obj_index(30046),0);
         sprintf(buf,"A wall of blood is here, blocking your exit west");
         obj->short_descr = str_dup(buf);
      }

   if ( !str_cmp(arg,"d") )
      {
         obj=create_object(get_obj_index(30047),0);
         sprintf(buf,"A wall of blood is here, blocking your exit down");
         obj->short_descr = str_dup(buf);
      }

   if ( !str_cmp(arg,"u") )
      {
         obj=create_object(get_obj_index(30048),0);
         sprintf(buf,"A wall of blood is here, blocking your exit up.");
         obj->short_descr = str_dup(buf);
      }

   obj_to_room(obj,ch->in_room);
   obj->timer = 2;
   obj->item_type = ITEM_WALL; 
}
