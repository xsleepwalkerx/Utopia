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

void do_electric_shield( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_WISP) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( IS_SET(ch->itemaffect, ITEMA_ELECSHIELD) )
      {
         REMOVE_BIT(ch->itemaffect, ITEMA_ELECSHIELD);
	   send_to_char( "#yThe electric crackles then gently fades away.#n\n\r", ch );
         return;
      }

   if ( ch->mana < 2500000 )
      {
	   send_to_char( "You dont have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 2500000;

   act("You #zflash#n #7bright #yyellow#n and cooe '#ymee mee iooo ioooo#n'",ch,NULL,NULL,TO_CHAR);
   act("$n #zflashes#n #7bright #yyellow#n and cooes '#ymee mee iooo ioooo#n'",ch,NULL,NULL,TO_ROOM);
   act("You are suddenly surrounded by crackling #ylightning#n and cooe '#yeyo! eyo!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n is suddenly surrounded by crackling #ylightning#n and cooes '#yeyo! eyo!#n'",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->itemaffect, ITEMA_ELECSHIELD);
   ch->wait = 12;
}

void do_colour_spray( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = NULL;
  OBJ_DATA *obj;
  char arg [MAX_INPUT_LENGTH];
  int dam = 250000;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS (ch, CLASS_WISP ) )
     {
	  send_to_char("Huh?\n\r", ch);
        return;
     }

  one_argument( argument, arg );

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Whom do you want to kill?\n\r", ch );
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

  switch( number_range( 1, 8 ) )
	 {
        default: 

		 case 6: act("#7You turn a white colour and start spraying $N #7white!#n",ch,NULL,victim,TO_CHAR);
                     act("#7$n #7turns a white colour and starts spraying $N #7white!#n",ch,NULL,victim,TO_NOTVICT);
                     act("#7$n #7turns a white colour and starts spraying you white!#n",ch,NULL,victim,TO_VICT);

				 if ( number_percent() > 69 - devil_luck( ch, victim, 5 ) )
				 {
				 act("#7You look a bit #Phealthier#7 through all the #Rchaos#n",ch,NULL,NULL,TO_CHAR);
				 act("#7$n #7looks a bit #Phealthier#7 through all the #Rchaos#n",ch,NULL,NULL,TO_ROOM);

				 ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );
				 }

                         ch->ele_attack = ATTACK_HOLY;
				 break;

		 case 2: act("#RYou turn a red colour and start spraying $N #Rred!#n",ch,NULL,victim,TO_CHAR);
                     act("#R$n #Rturns a red colour and start spraying $N #Rred!#n",ch,NULL,victim,TO_NOTVICT);
                     act("#R$n #Rturns a red colour and start spraying you red!#n,",ch,NULL,victim,TO_VICT);

				 if ( number_percent() > 49 - devil_luck( ch, victim, 10 ) )
				 {
				 act("#RYou make a lame roar sound and cooe '#yWeeeee#R' as you viciously attack $N#n",ch,NULL,victim,TO_CHAR);
				 act("#R$n #Rmakes a lame roar sound and cooes '#yWeeeee#R' as $e viciously attacks $N#n",ch,NULL,victim,TO_NOTVICT);
				 act("#R$n #Rmakes a lame roar sound and cooes '#yWeeeee#R' as $e viciously attakcs you#n",ch,NULL,victim,TO_VICT);
                         dam += 50000;
				 }

                         ch->ele_attack = ATTACK_FIRE;
				 break;

             case 3: act("#LYou turn a blue colour and start spraying $N #Lblue!#n",ch,NULL,victim,TO_CHAR);
                     act("#L$n #Lturns a blue colour and starts spraying $N #Lblue!#n",ch,NULL,victim,TO_NOTVICT);
                     act("#L$n #Lturns a blue colour and starts spraying you blue!#n",ch,NULL,victim,TO_VICT);

				 if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) 
                              && !IS_SET( victim->more, MORE_FROZEN ) )
				 {
				 act("#LYou cooe '#yMaa Maa.... Freezo!#L' as $N #Lcan't #zmove!#n",ch,NULL,victim,TO_CHAR);
				 act("#L$n #Lcooes '#yMaa Maa.... Freezo!#L' as $N #Lcan't #zmove!#n",ch,NULL,victim,TO_NOTVICT);
				 act("#L$n #Lcooes '#yMaa Maa.... Freezo!#L' as you can't #zmove!#n",ch,NULL,victim,TO_VICT);
                         victim->freeze_timer = 1;
                         SET_BIT( victim->more, MORE_FROZEN );
				 }

                         ch->ele_attack = ATTACK_ICE;
				 break;

             case 1:
             case 8:
		 case 4: act("#yYou turn a yellow colour and start spraying $N #yyellow!#n",ch,NULL,victim,TO_CHAR);
                     act("#y$n #yturns a yellow colour and starts spraying $N #yyellow!#n",ch,NULL,victim,TO_NOTVICT);
                     act("#y$n #yturns a yellow colour and starts spraying you yellow!#n",ch,NULL,victim,TO_VICT);

				 if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
				 {
				 act("#yYou cooe 'Weeo weeo! Keeyo!' as you slap $N #ysilly!#n",ch,NULL,victim,TO_CHAR);
				 act("#y$n #ycooes 'Weeo weeo! Keeyo!' as $e slaps $N #ysilly!#n",ch,NULL,victim,TO_NOTVICT);
				 act("#y$n #ycooes 'Weeo weeo! Keeyo!' as $e slaps you silly!#n",ch,NULL,victim,TO_VICT);
                         victim->wait = UMIN( 18, victim->wait + 12 );
				 }

                         ch->ele_attack = ATTACK_THUNDER;
				 break;

		 case 7: act("#gYou turn a green colour and start spraying $N #ggreen!#n",ch,NULL,victim,TO_CHAR);
                     act("#g$n #gturns a green colour and starts spraying $N #ggreen!#n",ch,NULL,victim,TO_NOTVICT);
                     act("#g$n #gturns a green colour and starts spraying you green!#n",ch,NULL,victim,TO_VICT);

				 if ( number_percent() > 60 - devil_luck( ch, victim, 10 ) && !IS_SET(victim->mflags, MAGE_EMBRACED2) &&
                              !IS_SET(victim->mflags, MAGE_EMBRACED1) )
				 {
				 act("#gYou cooe '#yWazzo Wazzo mahazo!#g' as $N #ggets tangled in the ground#n",ch,NULL,victim,TO_CHAR);
				 act("#g$n #gcooes '#yWazzo Wazzo mahazo!#g' as $N #ggets tangled in the ground#n",ch,NULL,victim,TO_NOTVICT);
				 act("#g$n #gcooes '#yWazzo Wazzo mahazo!#g' as you get tangled in the ground#n",ch,NULL,victim,TO_VICT);
                         SET_BIT(victim->mflags, MAGE_EMBRACED2);
				 }

                         ch->ele_attack = ATTACK_EARTH;
				 break;

		 case 5: act("#0You turn a black colour and start spraying $N #0black!#n",ch,NULL,victim,TO_CHAR);
                     act("#0$n #0turns a black colour and starts spraying $N #0black!#n",ch,NULL,victim,TO_NOTVICT);
                     act("#0$n #0turns a black colour and starts spraying you black!#n",ch,NULL,victim,TO_VICT);

				 if ( number_percent() > 74 - devil_luck( ch, victim, 10 ) && !IS_SET(victim->monkstuff, MONK_DEATH) )
				 {
				 act("#0You cooe '#yEvil evviiiiiil!#0' as $N #0suddenly feels very bad#n",ch,NULL,victim,TO_CHAR);
				 act("#0$n #0cooes '#yEvil evviiiiiil!#0' as $N #0suddenly feels very bad#n",ch,NULL,victim,TO_NOTVICT);
				 act("#0$n #0cooes '#yEvil evviiiiiil!#0' as you suddenly feel very bad#n",ch,NULL,victim,TO_VICT);
                         SET_BIT(victim->monkstuff, MONK_DEATH);
				 }

                         ch->ele_attack = ATTACK_DARK;
				 break;
  }

  ch->wait = 4;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_focus = DIRECT;
  ch->attack_var = BLAST;
  SET_BIT( ch->arena, AFF2_COMBO );
  hurt_person( ch, victim, dam, TRUE );
}

void do_inner_light( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))		return;

	if ( !IS_CLASS(ch, CLASS_WISP) )
	{
		stc("Huh?\n\r",ch);
		return;
	}

	if ( IS_SET(ch->more, MORE_LIGHT) )
	{
		REMOVE_BIT(ch->more, MORE_LIGHT);
		send_to_char("You release your concentration as the light around your body dims then fades away.\n\r",ch);
		act("The blazing light enveloping $n's body fades away.",ch,NULL,NULL,TO_ROOM);
	}
	else
	{
		send_to_char("Your body is enveloped by endless amount of blazing energy.\n\r",ch);
		act("You cover your eyes as blazing energy bursts out of $n's body.",ch,NULL,NULL,TO_ROOM);
		SET_BIT(ch->more, MORE_LIGHT);
	}
}

void do_heat_shield(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *rch, *rch_next;
    int dam = 1000000;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_WISP) )
	   {
          send_to_char("Huh?\n\r",ch);
          return;
	   }

    if ( ch->pkill < 8 )
	   {
		  stc("You are not strong enough to know this yet, wisp\n\r",ch);
		  return;
	   }

    if ( ch->ctimer[0] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 500000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    if ( ch->hit > ch->max_hit * 0.25 )
	   {
		  stc("You aren't desperate enough to use this attack yet!\n\r",ch);
		  return;
	   }

    ch->mana -= 500000;

    act("#RYou suddenly glow a vicious red colour as your body size increases temporarily.#n",ch,NULL,NULL,TO_CHAR);
	act("#R$n#R suddenly glows a vicious red colour as $s body size increases temporarily.#n",ch,NULL,NULL,TO_ROOM);
	act("#R'#yMezo lazo rezo!#R' you cooe as you heat up the entire room with your body!#n",ch,NULL,NULL,TO_CHAR);
	act("#R'#yMezo lazo rezo!#R' $n#R cooes as $e heats up the entire room with $s body!#n",ch,NULL,NULL,TO_ROOM);

    ch->ctimer[0] = 6;
    ch->wait = 4;

    for( rch = ch->in_room->people; rch != NULL; rch = rch_next )
    {
       rch_next = rch->next_in_room;

       if ( is_same_group(ch,rch) )
          continue;

       if ( is_safe(ch, rch) )
          continue;

	   act("#RYou scream '#yARGHHHHHHH!#R' as the energy burns you severely!#n",rch,NULL,NULL,TO_CHAR);
	   act("#R$n#R screams '#yARGHHHHHHH!#R' as the energy burns $m severely!#n",rch,NULL,NULL,TO_ROOM);

       ch->ele_attack = ATTACK_FIRE;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_focus = UPPERCUT;
       ch->attack_var = BLAST;
       hurt_person( ch, rch, dam, TRUE );
    }
}

void do_call_lightning( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_WISP) )
	 {
          send_to_char("Huh?\n\r",ch);
          return;
	 }

    if ( ch->pkill < 10 )
       {
          send_to_char("You aren't powerful enough to use this yet, Wisp.\n\r",ch);
          return;
       }

    if ( ch->ctimer[1] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

	if ( ch->mana < 500000 )
       { 
          stc("You don't have enough magic energy for that.\n\r",ch);
          return;
       }

	if ( weather_info.sky == SKY_RAINING || weather_info.sky == SKY_LIGHTNING )
	   {
		  stc("You cannot do this while it is raining or lightning, wisp!\n\r",ch);
		  return;
	   }

	ch->mana -= 500000;

    act("You bobble in the air and cooe '#yQueza queza heeeeep heeeep!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n bobbles in the air and cooe '#yQueza queza heeeeep heeeep!#n'",ch,NULL,NULL,TO_ROOM);
    act("#yQuezacotl#n arrives before you and you start to flash happily in different colours",ch,NULL,NULL,TO_CHAR);
    act("#yQuezacotl#n arrives before $n and $n starts to flash happily in different colours",ch,NULL,NULL,TO_ROOM);
    act("#yQuezacotl#n uses his stormy self to bring the clouds together!",ch,NULL,NULL,TO_CHAR);
    act("#yQuezacotl#n uses his stormy self to bring the clouds together!",ch,NULL,NULL,TO_ROOM);
    act("#yQuezacotl#n says solemnly '#yNow go wee one.. fulfill your duty for me..#n'",ch,NULL,NULL,TO_CHAR);
    act("#yQuezacotl#n says solemnly '#yNow go wee one.. fulfill your duty for me..#n'",ch,NULL,NULL,TO_ROOM);
    act("With a sudden #zflash#n of energy #yQuezacotl#n has disappeared..",ch,NULL,NULL,TO_CHAR);
    act("With a sudden #zflash#n of energy #yQuezacotl#n has disappeared..",ch,NULL,NULL,TO_ROOM);
    ch->ctimer[1] = 12;

    weather_info.sky = SKY_RAINING;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->character == NULL )
          continue;

       stc( "#7Clouds #0form up ahead...#n\n\r", d->character );

       if ( d->character != ch )
          stc( "Suddenly #Lrain#n appears...\n\r", d->character );
	}

    ch->wait = 4;

   if ( number_percent() > 50 )
	{
	   sta("The rain gets heavier.. #zCRASH!! BANG!!#n #ylightning#n has brewed..\n\r", ch->in_room->area );
	   weather_info.sky = SKY_LIGHTNING;
	}
}

void do_thunderstorm(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *vch, *vch_next;
    int chance = 75;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_WISP) )
	   {
          send_to_char("Huh?\n\r",ch);
          return;
	   }

	if ( weather_info.sky != SKY_RAINING && weather_info.sky != SKY_LIGHTNING )
	   {
		  stc("You cannot call a thunderstorm without rain or lightning!\n\r", ch);
		  return;
	   }

    if ( ch->pkill < 14 )
	   {
		  stc("You are not strong enough to know this yet, wisp\n\r",ch);
		  return;
	   }

    if ( ch->ctimer[2] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 500000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    ch->mana -= 500000;

    act("#yYou turn a bright yellow colour as you cooe 'Meeko meeko lahimimimi!'#n",ch,NULL,NULL,TO_CHAR);
	act("#y$n#y turns a bright yellow colour as $e cooes 'Meeko meeko lahimimimi!#n",ch,NULL,NULL,TO_ROOM);
	act("#yYour chanting causes a giant thunderstorm to brew!#n",ch,NULL,NULL,TO_CHAR);
	act("#y$n's#y chanting causes a giant thunderstorm to brew!#n",ch,NULL,NULL,TO_ROOM);
	act("#yThousands of lightning bolts fly around as you rejoice; cooeing away 'pla pla pla!'#n",ch,NULL,NULL,TO_CHAR);
	act("#yThousands of lightning bolts fly around as $n#y rejoices; cooeing away 'pla pla pla!'#n",ch,NULL,NULL,TO_ROOM);

    ch->ctimer[2] = 2;
    ch->wait = 4;

    if ( weather_info.sky == SKY_LIGHTNING )
       chance = 50;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) || number_percent() < chance )
         continue;

      act("#yBOOM! You stagger backwards as your struck by lightning!#n",vch,NULL,NULL,TO_CHAR);
      act("#yBOOM! $n staggers backwards as $e's struck by lightning!#n",vch,NULL,NULL,TO_ROOM);
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_focus = AERIAL;
      ch->attack_var = HIT;
      hurt_person( ch, vch, 1500000, TRUE );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->character == NULL || d->character == ch )
         continue;

      if ( number_percent() < chance - devil_luck( ch, d->character, 10 ) )
         continue;

      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
         {
            if ( ch->in_room->area != d->character->in_room->area )
               continue;

            if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
               continue;

            act("#yBOOM! You stagger backwards as your struck by lightning!#n",d->character,NULL,NULL,TO_CHAR);
	      act("#yBOOM! $n staggers backwards as $e's struck by lightning!#n",d->character,NULL,NULL,TO_ROOM);

            d->character->agg = 100;

            if ( !IS_CLASS( d->character, CLASS_WISP ) && !is_martial( d->character ) )
               d->character->wait = UMIN( 24, d->character->wait + number_range( 12, 18 ) );

            ch->ele_attack = ATTACK_THUNDER;
            ch->attack_type = ATTACK_PHYSICAL;
            ch->attack_focus = AERIAL;
            ch->attack_var = HIT;
            hurt_person( ch, d->character, 500000, TRUE );
         }
   }
}

void do_plasma_blade( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int obj_num = 94;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;   

    if (!IS_CLASS(ch, CLASS_WISP) )
    {
	send_to_char( "Huh?\n\r", ch);
	return;
    }

    if ( ch->pkill < 16 )
       {
          stc("You are not strong enough to know this yet, wisp\n\r",ch);
          return;
       }

    if ( IS_HEAD( ch, LOST_HEAD ) )
       {
          send_to_char("You cannot change your form without a head.\n\r",ch);
          return;
       }

    if ( ( obj = ch->pcdata->chobj ) != NULL )
    {
       if ( IS_EXTRA( ch, EXTRA_OSWITCH ) )
          REMOVE_BIT( ch->extra,EXTRA_OSWITCH );

       ch->pcdata->obj_vnum = 0;
       obj->chobj = NULL;
       obj->level = 4;
       ch->pcdata->chobj = NULL;
       free_string(ch->morph);
       sprintf(buf,"#w%s the #yWisp#n", ch->pcdata->switchname );
       ch->morph = str_dup(buf);
       act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
       stc("You reform back into your body.\n\r", ch );
       extract_obj(obj);

       if ( ch->in_room->vnum == ROOM_VNUM_IN_OBJECT )
          {
             char_from_room( ch );
             char_to_room( ch, get_room_index( 800 ) );
          }

       return;
    }

    if ((obj = create_object(get_obj_index(obj_num),60)) == NULL)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_WEBBED))
    {
	send_to_char( "Not with all this sticky webbing on.\n\r", ch);
	return;
    }

    if (ch->fight_timer>0)
    {
       send_to_char("Not with a fighttimer.\n\r",ch);
       return;
    }

	   if ( ch->ctimer[6] > 0 )
	   {
		  stc("You can't do another plasma blade yet..\n\r",ch);
	   return;
	   }

    obj_to_room( obj, ch->in_room );
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);

    ch->ctimer[6] = 50;

    ch->pcdata->obj_vnum = obj_num;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    obj->timer = 12;
    SET_BIT( ch->affected_by, AFF_POLYMORPH );
    SET_BIT( ch->extra, EXTRA_OSWITCH );
    free_string( ch->morph );
    ch->morph = str_dup(obj->short_descr);
}

void do_icarus(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *vch, *vch_next;
    int dam = 1000000;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) )
       return;
    
    if ( !IS_CLASS(ch, CLASS_WISP) )
	   {
          send_to_char("Huh?\n\r",ch);
          return;
	   }

	if ( weather_info.sky == SKY_ECLIPSE || weather_info.sky == SKY_APOCALYPSE || IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) || IS_SET(ch->in_room->area->bits,ROOM_TOTAL_DARKNESS) )
	   {
		  stc("You cannot use Icarus when the sunlight cannot be seen!\n\r", ch);
		  return;
	   }

    if ( ch->pkill < 18 )
	   {
		  stc("You are not strong enough to know this yet, wisp\n\r",ch);
		  return;
	   }

    if ( ch->ctimer[3] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 500000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    ch->mana -= 500000;

    act("#RYou glow bright red in anger '#ybroooooooo ICA ICA ICARUS!#R'#n",ch,NULL,NULL,TO_CHAR);
	act("#R$n#R glows bright red in anger '#ybroooooooo ICA ICA ICARUS!#R'#n",ch,NULL,NULL,TO_ROOM);
	act("#RIcarus aims down towards the area and you cooe '#yIca Ica!#R'#n",ch,NULL,NULL,TO_CHAR);
	act("#RIcarus aims down towards the area and $n#R cooes '#yIca Ica!#R'#n",ch,NULL,NULL,TO_ROOM);
	act("#RIcarus begins to charge up a powerful sunbeam to stagger everyone!#n",ch,NULL,NULL,TO_CHAR);
	act("#RIcarus begins to charge up a powerful sunbeam to stagger everyone!#n",ch,NULL,NULL,TO_ROOM);
	act("#ySuddenly you turn into a speck of dust, avoiding the wrath of #RIcarus#y..#n",ch,NULL,NULL,TO_CHAR);
	act("#ySuddenly $n#y turns into a speck of dust, avoiding the wrath of #RIcarus#y..#n",ch,NULL,NULL,TO_ROOM);

    ch->ctimer[3] = 6;
    ch->wait = 4;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("#yIcarus fires down on you and sizzles you to a cinder! OUCH!#n",vch,NULL,NULL,TO_CHAR);
      act("#yIcarus fires down on $n #yand sizzles $m to a cinder! OUCH!#n",vch,NULL,NULL,TO_ROOM);
      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_focus = AERIAL;
      ch->attack_var = HIT;
      hurt_person( ch, vch, 1000000, TRUE );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	{
          if ( ch->in_room->area != d->character->in_room->area )
             continue;

          if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
             continue;

          act("#yIcarus fires down on you and sizzles you to a cinder! OUCH!#n",d->character,NULL,NULL,TO_CHAR);
	    act("#yIcarus fires down on $n #yand sizzles $m to a cinder! OUCH!#n",d->character,NULL,NULL,TO_ROOM);
          ch->attack_type = ATTACK_MAGICAL;
          ch->ele_attack = ATTACK_THUNDER;
          ch->attack_focus = AERIAL;
          ch->attack_var = HIT;
          hurt_person( ch, d->character, dam, TRUE );
	}
   }
}

void do_mindtap( CHAR_DATA *ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char buffy[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
    
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_WISP) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You are not strong enough to know this yet, wisp\n\r",ch);
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( arg1[0] == '\0' || arg2[0] == '\0' )
      {
         send_to_char( "Force whom to do what?\n\r", ch );
         return;
      }

   if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( is_safe(ch,victim) )
      return;

   if ( victim == ch )
      {
         send_to_char( "How can you force yourself??\n\r", ch );
         return;
      }

   if ( !str_suffix(arg2,"give") || !str_suffix(arg2,"delete") || 
        !str_suffix(arg2,"quit") || !str_suffix(arg2,"remove") 
        || !str_suffix(arg2,"detox") || !str_suffix(arg2,"gift")
        || !str_suffix(arg2,"drop") || !str_suffix(arg2,"consent")
        || !str_suffix(arg2,"giveup") ) 
      { 
         send_to_char( "You can't force them to do that.\n\r", ch ); 
         return; 
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800
        && ( !str_suffix(arg2,"warp") || !str_suffix(arg2,"recall") ) )
      { 
         send_to_char( "You can't force them to do that in this area.\n\r", ch ); 
         return; 
      }

   ch->ctimer[4] = 6;
   ch->wait = 4;

   sprintf( buffy, "%s %s", arg2, argument );

   act("You slam yourself into $N as you fade into $S body.",ch,NULL,victim,TO_CHAR);
   act("$n slams $mself into you as $e fades into your body.",ch,NULL,victim,TO_VICT);
   act("$n slams $mself into $N as $e fades into $S body.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly grasp your head in agony as a piercing voice wrecks havoc into your mind.",victim,NULL,NULL,TO_CHAR);
   act("$n suddenly grasps $s head in agony as a piercing voice wrecks havoc into $s mind.",victim,NULL,NULL,TO_ROOM);

   sprintf( buf, "Yes... I... I must... %s", buffy );
   do_say( victim, buf ); 

   interpret( victim, buffy );
}

void do_colourbeam( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim = NULL;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam = 1000000;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS (ch, CLASS_WISP ) )
      {
         send_to_char("Huh?\n\r", ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to kill?\n\r", ch );
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

   act("#yYou glow bright yellow in excitement '#ybig biiiig BEAMO attack!#R'#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n#y glows bright yellow in excitement '#ybig biiiig BEAMO attack!#R'#n",ch,NULL,NULL,TO_ROOM);

   switch( number_range( 1, 8 ) )
   {
      default: 
      case 6: act("#7You turn a white colour and release a holy beam at $N#7!#n",ch,NULL,victim,TO_CHAR);
              act("#7$n #7turns a white colour and releases a holy beam at $N#7!#n",ch,NULL,victim,TO_NOTVICT);
              act("#7$n #7turns a white colour and releases a holy beam at you!#n",ch,NULL,victim,TO_VICT);

              if ( ch->ctimer[7] == 0 )
                 {
                    act("#7You look alot #Phealthier#7 through all the #Rchaos#n",ch,NULL,NULL,TO_CHAR);
                    act("#7$n #7looks alot #Phealthier#7 through all the #Rchaos#n",ch,NULL,NULL,TO_ROOM);
                    ch->hit = UMIN( ch->max_hit, ch->hit + 500000 );
                 }

              ch->ele_attack = ATTACK_HOLY;
              break;

      case 2: act("#RYou turn a red colour and release a fiery beam at $N#R!#n",ch,NULL,victim,TO_CHAR);
              act("#R$n #Rturns a red colour and releases a fiery beam at $N#R!#n",ch,NULL,victim,TO_NOTVICT);
              act("#R$n #Rturns a red colour and releases a fiery beam at you!#n,",ch,NULL,victim,TO_VICT);

              if ( ch->ctimer[7] == 0 )
                 {
                    act("#RYou make a lame roar sound and cooe '#yDieeeee#R' giving $N #Ra critical hit!#n",ch,NULL,victim,TO_CHAR);
                    act("#R$n #Rmakes a lame roar sound and cooes '#yDieeeee#R' giving $N #Ra critical hit!#n",ch,NULL,victim,TO_NOTVICT);
                    act("#R$n #Rmakes a lame roar sound and cooes '#yDieeeee#R' giving you a critical hit!#n",ch,NULL,victim,TO_VICT);
                    dam += 500000;
                 }

              ch->ele_attack = ATTACK_FIRE;
	        break;

      case 3: act("#LYou turn a blue colour and release a freezing beam at $N#L!#n",ch,NULL,victim,TO_CHAR);
              act("#L$n #Lturns a blue colour and releases a freezing beam at $N#!#n",ch,NULL,victim,TO_NOTVICT);
              act("#L$n #Lturns a blue colour and releases a freezing beam at you!#n",ch,NULL,victim,TO_VICT);

              if ( ch->ctimer[7] == 0 )
                 {
                    act("#LYou cooe '#yMaa Maa.... Freezo!#L' as $N #Lcan't #zmove!#n",ch,NULL,victim,TO_CHAR);
                    act("#L$n #Lcooes '#yMaa Maa.... Freezo!#L' as $N #Lcan't #zmove!#n",ch,NULL,victim,TO_NOTVICT);
                    act("#L$n #Lcooes '#yMaa Maa.... Freezo!#L' as you can't #zmove!#n",ch,NULL,victim,TO_VICT);
                    victim->freeze_timer = 1;
                    SET_BIT( victim->more, MORE_FROZEN );
                 }

              ch->ele_attack = ATTACK_ICE;
              break;

      case 1:
      case 8:
      case 4: act("#yYou turn a yellow colour and release a shocking beam at $N#y!#n",ch,NULL,victim,TO_CHAR);
              act("#y$n #yturns a yellow colour and releases a shocking beam at $N#y!#n",ch,NULL,victim,TO_NOTVICT);
              act("#y$n #yturns a yellow colour and releases a shocking beam at you!#n",ch,NULL,victim,TO_VICT);

              if ( ch->ctimer[7] == 0 )
                 {
                    act("#yYou cooe 'Weeo weeo! Keeyo!' as you give $N #ya b^%%&* slap!#n",ch,NULL,victim,TO_CHAR);
                    act("#y$n #ycooes 'Weeo weeo! Keeyo!' as $e gives $N #ya b^%%&* slap!#n",ch,NULL,victim,TO_NOTVICT);
                    act("#y$n #ycooes 'Weeo weeo! Keeyo!' as $e gives you a b^%%&* slap!#n",ch,NULL,victim,TO_VICT);
                    victim->wait = UMIN( 18, victim->wait + 12 );
                 }

              ch->ele_attack = ATTACK_THUNDER;
              break;

      case 7: act("#gYou turn a green colour and release a viney beam $N#g!#n",ch,NULL,victim,TO_CHAR);
              act("#g$n #gturns a green colour and releases a viney beam $N#g!#n",ch,NULL,victim,TO_NOTVICT);
              act("#g$n #gturns a green colour and releases a viney beam you green!#n",ch,NULL,victim,TO_VICT);

              if ( ch->ctimer[7] == 0 )
                 {
                    act("#gYou cooe '#yWazzo Wazzo mahazo!#g' as $N #ggets tangled in the ground#n",ch,NULL,victim,TO_CHAR);
                    act("#g$n #gcooes '#yWazzo Wazzo mahazo!#g' as $N #ggets tangled in the ground#n",ch,NULL,victim,TO_NOTVICT);
                    act("#g$n #gcooes '#yWazzo Wazzo mahazo!#g' as you get tangled in the ground#n",ch,NULL,victim,TO_VICT);
                    SET_BIT(victim->mflags, MAGE_EMBRACED2);
                 }

              ch->ele_attack = ATTACK_EARTH;
              break;

      case 5: act("#0You turn a black colour and release a demonic beam $N#0!#n",ch,NULL,victim,TO_CHAR);
              act("#0$n #0turns a black colour and releases a demonic beam $N#0!#n",ch,NULL,victim,TO_NOTVICT);
              act("#0$n #0turns a black colour and releases a demonic beam you black!#n",ch,NULL,victim,TO_VICT);

              if ( ch->ctimer[7] == 0 )
                 {
                    act("#0You cooe '#yEvil evviiiiiil!#0' as $N #0suddenly feels very bad#n",ch,NULL,victim,TO_CHAR);
                    act("#0$n #0cooes '#yEvil evviiiiiil!#0' as $N #0suddenly feels very bad#n",ch,NULL,victim,TO_NOTVICT);
                    act("#0$n #0cooes '#yEvil evviiiiiil!#0' as you suddenly feel very bad#n",ch,NULL,victim,TO_VICT);
                    SET_BIT(victim->monkstuff, MONK_DEATH);
                 }

              ch->ele_attack = ATTACK_DARK;
              break;
   }

   if ( ch->ctimer[7] > 0 )
      dam = 200000;

   if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
        && ch->ctimer[9] != 100 ) )
      ch->ctimer[7] = 2;

   ch->wait = 4;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = BEAM;
   hurt_person( ch, victim, dam, TRUE );
}

void do_rain_blade( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 2000000;
   DESCRIPTOR_DATA *d;

   if ( IS_NPC(ch) )
      return;
    
   if ( !IS_CLASS (ch, CLASS_WISP ) )
      {
         send_to_char("Huh?\n\r", ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   act("#RYou glow bright red in anger '#ybroooooooo I I IA KIIIIL YOU!#R'#n",ch,NULL,NULL,TO_CHAR);
   act("#R$n#R glows bright red in anger '#ybroooooooo I I IA KIIIIL YOU!#R'#n",ch,NULL,NULL,TO_ROOM);
   act("#yYou concentrate for a moment as you transform yourself into a MASSIVE cannon!#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n #yconcentrates for a moment as $e transforms $mself into a MASSIVE cannon!#n",ch,NULL,NULL,TO_ROOM);
   act("#yA glint of #Pdazzling #wlight #yradiates of your #emecha #ybody as you start charging up!!!#n",ch,NULL,NULL,TO_CHAR);
   act("#yA glint of #Pdazzling #wlight #yradiates of $n#y's #emecha #ybody as $e starts charging up!!!#n",ch,NULL,NULL,TO_ROOM);
   act("#PYou mutter '#yRAAAAAAIIIIIINNNNN...!#P' as your cannon hums away violently.#n",ch,NULL,NULL,TO_CHAR);
   act("#P$n mutters '#yRAAAAAAIIIIIINNNNN...!#P' as $s cannon hums away violently.#n",ch,NULL,NULL,TO_ROOM);
   act("#RYou scream '#yBLADE!#R' while releasing a #yTREMEDOUS #Rblast into the air!#n",ch,NULL,NULL,TO_CHAR);
   act("#R$n #Rscreams '#yBLADE!#R' while releasing a #yTREMEDOUS #Rblast into the air!#n",ch,NULL,NULL,TO_ROOM);
   act("#yThe area suddenly #edarkens #yas a barrage of #wbeams #yrain down into the vicinity!#n",ch,NULL,NULL,TO_CHAR);
   act("#yThe area suddenly #edarkens #yas a barrage of #wbeams #yrain down into the vicinity!#n",ch,NULL,NULL,TO_ROOM);

   ch->wait = 4;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("#yThe #wbeams #yof #edeadly #yenergy RIP through your body like a knife in butter!#n",vch,NULL,NULL,TO_CHAR);
      act("#yThe #wbeams #yof #edeadly #yenergy RIP through $n#y's body like a knife in butter!#n",vch,NULL,NULL,TO_ROOM);
      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_focus = AERIAL;
      ch->attack_var = BEAM;
      hurt_person( ch, vch, dam, TRUE );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	{
          if ( ch->in_room->area != d->character->in_room->area )
             continue;

          if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
             continue;

          act("#yThe #wbeams #yof #edeadly #yenergy RIP through your body like a knife in butter!#n",d->character,NULL,NULL,TO_CHAR);
          act("#yThe #wbeams #yof #edeadly #yenergy RIP through $n#y's body like a knife in butter!#n",d->character,NULL,NULL,TO_ROOM);
          ch->attack_type = ATTACK_PHYSICAL;
          ch->ele_attack = ATTACK_THUNDER;
          ch->attack_focus = AERIAL;
          ch->attack_var = BEAM;
          hurt_person( ch, d->character, dam, TRUE );
	}
   }
}

void perform_trigger( CHAR_DATA *ch, CHAR_DATA *victim )
{
   SET_BIT( ch->arena, AFF2_DM );

   act("You slam yourself into $N as you crackle with electricity.",ch,NULL,victim,TO_CHAR);
   act("$n slams $mself into you as $e crackles with electricity.",ch,NULL,victim,TO_VICT);
   act("$n slams $mself into $N as $e crackles with electricity.",ch,NULL,victim,TO_NOTVICT);
   act("You cooe '#yTRIGGA TIME!#n' as you transform into Benimaru!!!",ch,NULL,NULL,TO_CHAR);
   act("$n cooes '#yTRIGGA TIME!#n' as $e transforms into Benimaru!!!",ch,NULL,NULL,TO_ROOM);
   act("You then grab $N by the neck stylishly as your hair fizzes with static energy.",ch,NULL,victim,TO_CHAR);
   act("Benimaru then grabs you by the neck stylishly as his hair fizzes with static energy.",ch,NULL,victim,TO_VICT);
   act("Benimaru then grabs $N by the neck stylishly as his hair fizzes with static energy.",ch,NULL,victim,TO_NOTVICT);
   act("You scream '#yIya No Sida!#n' as you release your innate electricity into $N!",ch,NULL,victim,TO_CHAR);
   act("Benimaru screams '#yIya No Sida!#n' as he releases his innate electricity into you!",ch,NULL,victim,TO_VICT);
   act("Benimaru screams '#yIya No Sida!#n' as he releases his innate electricity into $N!",ch,NULL,victim,TO_NOTVICT);
   act("You then SLAM $N onto the ground very hard as you clench your fists!",ch,NULL,victim,TO_CHAR);
   act("Benimaru then SLAMS you onto the ground very hard as he clenches his fists!",ch,NULL,victim,TO_VICT);
   act("Benimaru then SLAMS $N onto the ground very hard as he clenches his fists!",ch,NULL,victim,TO_NOTVICT);
   act("$N's body suddenly implodes with crackling energy as you ask '#yUnderstand?#n'",ch,NULL,victim,TO_CHAR);
   act("Your body suddenly implodes with crackling energy as Benimaru asks '#yUnderstand?#n'",ch,NULL,victim,TO_VICT);
   act("$N's body suddenly implodes with crackling energy as Benimaru asks '#yUnderstand?#n'",ch,NULL,victim,TO_NOTVICT);

   if ( !is_martial( victim ) )
      victim->wait = UMAX( 12, victim->wait );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLUNT;
   hurt_person( ch, victim, 3500000, TRUE );

   REMOVE_BIT( ch->arena, AFF2_DM );

   act("You then whisper '#yThank you...#n' before transforming back to your original form.",ch,NULL,NULL,TO_CHAR);
   act("Benimaru then whispers '#yThank you...#n' before transforming back to $n.",ch,NULL,NULL,TO_ROOM);
}

void do_electrotrigger( CHAR_DATA *ch, char *argument )
{
   if ( !IS_CLASS (ch, CLASS_WISP ) )
      {
         send_to_char("Huh?\n\r", ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->blocking < 0 )
      {
         stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
         return;
      }

   ch->blocking = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_AUTOGUARD );

   act("You surround yourself with crackling #ylightning#n and cooe '#yyup! yup!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n surrounds $mself with crackling #ylightning#n and cooes '#yyup! yup!#n'",ch,NULL,NULL,TO_ROOM);
}
