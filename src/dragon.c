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

void check_dragon_form( CHAR_DATA *ch, char *argument )
{
}

void do_dragonshift( CHAR_DATA *ch, char *argument )
{
   char buf [MAX_STRING_LENGTH];
        
   if ( IS_NPC(ch) )
      return;
        
   if ( !IS_CLASS(ch, CLASS_DRAGON) )
      {
         stc("Huh?\n\r",ch);
         return;
      }
     
   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      {
         act("You return back into human form.\n\r",ch,NULL,NULL,TO_CHAR);
         act("$n returns back into human form.\n\r",ch,NULL,NULL,TO_ROOM);
         REMOVE_BIT(ch->pcdata->powers[1], DPOWER_LEVITATION);
         REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
         REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
         REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
         REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
         free_string( ch->morph );
         ch->morph = str_dup( "" );
         ch->dragonform = 0;
         return;
      }
    
   if ( ch->mounted == IS_RIDING )
      do_dismount(ch,"");

   if ( ch->loc_hp[6] > 0 )
      {
         stc("Your wounds close up and stop bleeding.\n\r",ch);
         ch->loc_hp[6] = 0;
      }
  
   if ( ( ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0 )
      {
         stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",ch);
         ch->loc_hp[0] = 0;
         ch->loc_hp[1] = 0;
         ch->loc_hp[2] = 0;
         ch->loc_hp[3] = 0;
         ch->loc_hp[4] = 0;
         ch->loc_hp[5] = 0;
      }

    sprintf(buf, "%s the Supreme Fiery Dragon", ch->name);
    act("Your body grows huge as large, hard scales form on your back.",ch,NULL,NULL,TO_CHAR);
    act("$n's body grows huge as large, hard scales form on $s back.",ch,NULL,NULL,TO_ROOM);

    if ( IS_DRAGONPOWER(ch, DRAGON_SIGHT) )
       {
          act("Your senses grown beyond the capacity of normal men.",ch,NULL,NULL,TO_CHAR);
          act("$n's eyes flashes gold.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->act, PLR_HOLYLIGHT);
          SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
       }

    if ( IS_DRAGONPOWER(ch, DRAGON_WINGS) )
       {
          act("Huge, powerful wings spread out of your body.",ch,NULL,NULL,TO_CHAR);
          act("Huge, powerful wings spread out of $n's body.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->pcdata->powers[1], DPOWER_LEVITATION);
       }

    if ( IS_DRAGONPOWER(ch, DRAGON_TAIL) )
       {
          act("A large, strong tail grows out from your body.",ch,NULL,NULL,TO_CHAR);
          act("A large, strong tail grows out from $n's body.",ch,NULL,NULL,TO_ROOM);
       }

    act("You let out a high-pitch roar as you are transformed into a hideous dragon!",ch,NULL,NULL,TO_CHAR);
    act("$n lets out a high-pitch roar as $e is transformed into a hideous dragon!",ch,NULL,NULL,TO_ROOM);
  
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
    ch->wait = 12;
}

int get_dragon_cost( CHAR_DATA *ch, char *argument )
{
    int cost;

    return 0;
  
    if (!str_cmp(argument,"toughness"))
       return cost = 50 + ( 14 * ch->pcdata->dragon_toughness );

    if (!str_cmp(argument,"magic"))
       return cost = 50 + ( 14 * ch->pcdata->dragon_magic );

    if (!str_cmp(argument,"power"))
       return cost = 50 + ( 14 * ch->pcdata->dragon_power );

    if (!str_cmp(argument,"armor"))
       return cost = 50 + ( 14 * ch->pcdata->dragon_armor );

    if (!str_cmp(argument,"sight"))
       return cost = 1000;

    if (!str_cmp(argument,"swoop"))
       return cost = 1000;

    if (!str_cmp(argument,"fury"))
       return cost = 1500;

    if (!str_cmp(argument,"immunity"))
       return cost = 500;

    if (!str_cmp(argument,"wings"))
       return cost = 200;

    if (!str_cmp(argument,"tail"))
       return cost = 400;

    if (!str_cmp(argument,"intuition"))
       return cost = 1500;

    if (!str_cmp(argument,"breath"))
       return cost = 700;

    if (!str_cmp(argument,"crush"))
       return cost = 700;

    if (!str_cmp(argument,"age"))
       return cost = 50000;

    return 0;
}

void do_dragongift( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int cost;
        
    if (IS_NPC(ch)) return;
        
    if (!IS_CLASS(ch, CLASS_DRAGON))
	 {
		stc("Huh?\n\r",ch);
		return;
	 }
     
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       stc("#e--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--#n\n",ch);
       stc("#cChoose a Dragon Art that you want to learn:#n\n",ch);

       if ( ch->pcdata->dragon_toughness < 50 )
          {
             sprintf(buf,"#yDragon Toughness ( in dragon form )   : #w%d  #g( %d per point )#n\n",ch->pcdata->dragon_toughness,get_dragon_cost(ch,"toughness"));
             stc(buf,ch);
          }
          else stc("#yDragon Toughness ( in dragon form )   : #w50#n\n",ch);

       if ( ch->pcdata->dragon_power < 50 )
          {
             sprintf(buf,"#yDragon Power     ( in dragon form )   : #w%d  #g( %d per point )#n\n",ch->pcdata->dragon_power,get_dragon_cost(ch,"power"));
             stc(buf,ch);
          }
          else stc("#yDragon Power     ( in dragon form )   : #w50#n\n",ch);
 
       if ( ch->pcdata->dragon_armor < 50 )
          {
             sprintf(buf,"#yDragon Armor     ( in dragon form )   : #w%d  #g( %d per point )#n\n",ch->pcdata->dragon_armor,get_dragon_cost(ch,"armor"));
             stc(buf,ch);
          }
          else stc("#yDragon Armor     ( in dragon form )   : #w50#n\n",ch);

       if ( ch->pcdata->dragon_magic < 50 )
          {
             sprintf(buf,"#yDragon Magic     ( in dragon form )   : #w%d  #g( %d per point )#n\n",ch->pcdata->dragon_magic,get_dragon_cost(ch,"magic"));
             stc(buf,ch);
          }
          else stc("#yDragon Magic     ( in dragon form )   : #w50#n\n",ch);

       sprintf(buf,"#yDragon Age ( to max out age )  : #w%d#n\n", get_dragon_cost(ch,"age") );
       stc(buf,ch);

       stc("#e--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--#n\n",ch);
       stc("#cSkills Learned:#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_SWOOP) )
          {
             sprintf(buf,"#y*Dragon Swoop ( dragonswoop )-#wswoop in on an opponent. #g( %d )#n\n",get_dragon_cost(ch,"swoop"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Swoop ( dragonswoop )- #w( learned )#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_SIGHT) )
          {
             sprintf(buf,"#y*Dragon Sight ( automatic )-#wtruesight in dragon form. #g( %d )#n\n",get_dragon_cost(ch,"sight"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Sight ( automatic )- #w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_WINGS) )
          {
             sprintf(buf,"#y*Dragon Wings ( automatic )-#wwings in dragon form. #g( %d )#n\n",get_dragon_cost(ch,"wings"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Wings ( automatic )-#w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_TAIL) )
          {
             sprintf(buf,"#y*Dragon Tail ( automatic )-#wtail in dragon form. #g( %d )#n\n",get_dragon_cost(ch,"tail"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Tail ( automatic )-#w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_BREATH) )
          {
             sprintf(buf,"#y*Dragon Breath ( dragonbreath )-#wuse a breath attack in dragon form.#n #g( %d )\n",get_dragon_cost(ch,"breath"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Breath ( dragonbreath )-#w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_FURY) )
          {
             sprintf(buf,"#y*Dragon Fury ( automatic )-#wunleash the rage of the dragon in dragon form.#n #g( %d )\n",get_dragon_cost(ch,"fury"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Fury ( automatic )-#w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_CRUSH) )
          {
             sprintf(buf,"#y*Dragon Crush ( automatic )-#wcrushing attack in dragon form.#n #g( %d )\n",get_dragon_cost(ch,"crush"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Crush ( automatic )-#w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_IMMUNITY) )
          {
             sprintf(buf,"#y*Dragon Immunity ( automatic )-#wimmunities to poison in dragon form.#n #g( %d )\n",get_dragon_cost(ch,"immunity"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Immunity ( automatic )-#w( learned ).#n\n",ch);

       if ( !IS_DRAGONPOWER(ch, DRAGON_INTUITION) )
          {
             sprintf(buf,"#y*Dragon Intuition( automatic )-#wdetection when being scryed in dragon form.#n #g( %d )\n",get_dragon_cost(ch,"intuition"));
             stc(buf,ch);
          }
          else stc("#y*Dragon Intuition ( automatic )-#w( learned ).#n\n",ch);
 
       stc("#e--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--#n\n",ch);
       return;
    }

    if (!str_cmp(arg,"toughness"))
       {
          if ( ch->pcdata->dragon_toughness >= 50 )
             {
                stc("Your dragon toughness is already at max.\n\r",ch); 
                return;
             }

          ch->pcdata->dragon_toughness++;
          stc("Your dragon toughness improves.\n\r",ch);
       }
       else if (!str_cmp(arg,"power"))
               {
                   if ( ch->pcdata->dragon_power >= 50 )
                      {
                         stc("Your dragon power is already at max.\n\r",ch); 
                         return;
                      }

                   ch->pcdata->dragon_power++;
                   stc("Your dragon power improves.\n\r",ch);
               }
       else if (!str_cmp(arg,"magic"))
               {
                   if ( ch->pcdata->dragon_magic >= 50 )
                      {
                         stc("Your dragon magic is already at max.\n\r",ch); 
                         return;
                      }

                   ch->pcdata->dragon_magic++;
                   stc("Your dragon magic improves.\n\r",ch);
               }
       else if (!str_cmp(arg,"armor"))
               {
                   if ( ch->pcdata->dragon_armor >= 50 )
                      {
                         stc("Your dragon armor is already at max.\n\r",ch); 
                         return;
                      }

                   ch->pcdata->dragon_armor++;
                   stc("Your dragon armor improves.\n\r",ch);
               }
       else if (!str_cmp(arg,"age"))
               {
                   if ( ch->pcdata->dragon_age > 5000000 )
                      {
                         stc("You are already at max age.\n\r",ch);
                         return;
                      }
 
                   ch->pcdata->dragon_age = 5000000;

                   stc("You are now a fully grown dragon.\n\r",ch);
               }
       else if (!str_cmp(arg,"swoop"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_SWOOP) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_SWOOP); 
                   stc("You are now capable of using dragon swoop.\n\r",ch);
               }
       else if (!str_cmp(arg,"wings"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_WINGS) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_WINGS); 
                   stc("You now have wings in dragon form.\n\r",ch);
               }
       else if (!str_cmp(arg,"tail"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_TAIL) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_TAIL); 
                   stc("You now have a tail in dragon form.\n\r",ch);
               }
       else if (!str_cmp(arg,"sight"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_SIGHT) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_SIGHT); 
                   stc("You now have improve senses in dragon form.\n\r",ch);
               }
       else if (!str_cmp(arg,"breath"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_BREATH) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_BREATH); 
                   stc("You now use a devastating breath attack.\n\r",ch);
               }
       else if (!str_cmp(arg,"fury"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_FURY) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_FURY); 
                   stc("You can now releash the full power of a dragon.\n\r",ch);
               }
       else if (!str_cmp(arg,"crush"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_CRUSH) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_CRUSH); 
                   stc("Your attacks can now do aggravated damage.\n\r",ch);
               }
       else if (!str_cmp(arg,"immunity"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_IMMUNITY) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_IMMUNITY); 
                   stc("Your body is now resistant to all forms of poison.\n\r",ch);
               }
       else if (!str_cmp(arg,"intuition"))
               {
                   if ( IS_DRAGONPOWER(ch, DRAGON_INTUITION) )
                      {
                         stc("But you already know this skill.\n\r",ch);
                         return;
                      } 

                   SET_BIT(ch->pcdata->powers[9], DRAGON_INTUITION); 
                   stc("You can now sense danger.\n\r",ch);
               }
       else do_dragongift(ch,"");
}

void do_dragonswoop( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch,CLASS_DRAGON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          send_to_char("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( !IS_DRAGONPOWER(ch, DRAGON_SWOOP) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_DRAGONPOWER(ch, DRAGON_WINGS) )
       {
          send_to_char("You need wings to use this skill.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Whom to you want to swoop on?\n\r", ch );
          return;
       }

    if ( ( ( victim = get_char_area( ch, arg ) ) == NULL ) || ( victim->in_room == NULL ) )
       {
          send_to_char( "They aren't here.\n\r", ch );
          return;
       }
 
    if ( victim->in_room->vnum == ch->in_room->vnum )
       {
          send_to_char("But you're already there!\n\r",ch);
          return;
       }

    stc("You flap your wings noisily as you zoom upwards in the sky.\n\r",ch);
    act("$n flaps $s wings noisily as $e zooms upwards in the sky.\n\r",ch,NULL,NULL,TO_ROOM);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    do_look(ch, "auto");
    stc("Circling around the vicinity, you charge straight down onto your target.\n\r",ch);
    act("$n charges from the sky straight into $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n charges from the sky straight into you.",ch,NULL,victim,TO_VICT);

    if ( ch->position < POS_STANDING || IS_SET( ch->more, MORE_FROZEN ) )
       return;

    ch->ctimer[0] = 1;
    ch->wait = 12;

    multi_hit( ch, victim, TYPE_UNDEFINED );

    if ( victim == NULL || victim->position != POS_FIGHTING ) return;

    multi_hit( ch, victim, TYPE_UNDEFINED );
}

void do_dragonbreath( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    int dam = char_damroll( ch );
    char arg[MAX_INPUT_LENGTH];
    int max_count = 15, count;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_NPC(ch) )
	 {
         if ( !IS_CLASS(ch,CLASS_DRAGON) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( !IS_DRAGONPOWER(ch, DRAGON_BREATH) )
            {
               send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( ch->dragon_timer > 0 )
            {
               stc("You haven't recovered your dragon senses yet.\n\r",ch);
               return;
            }

         if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
            {
               stc("Who do you wish to breathe on?\n\r",ch);
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

         if ( ch->clannum == 1 )
            max_count += UMIN( 5, ch->pkill / 4 );

         ch->dragon_timer = number_range( 7, 8 );
         ch->wait = 12;
      }
      else victim = ch->fighting;

    for ( count = 0; count < max_count; count++ )
    {
       ch->ele_attack = ATTACK_FIRE;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BLAST;
       ch->attack_focus = DIRECT;
       damage(ch,victim,-123456,gsn_breath);

       if ( victim == NULL || victim->position != POS_FIGHTING )
          {
             show_total_damage( ch, victim );
             return;
          }
    }

    show_total_damage( ch, victim );
}

void do_highclaw( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);

    if ( !IS_CLASS(ch,CLASS_DRAGON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to High Claw?\n\r", ch );
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

    if ( number_percent() > -1 )
       {
          act("You ANNIHILATE $N with your high claw.",ch,NULL,victim,TO_CHAR);
          act("$n ANNIHILATES you with a very deadly high claw.",ch,NULL,victim,TO_VICT);
          act("$n ANNIHILATES $N with $s high claw.",ch,NULL,victim,TO_NOTVICT);

          if ( ch->ctimer[15] == 0 )
             victim->move = UMAX( 0, victim->move - 500000 );

          ch->attack_type = ATTACK_PHYSICAL;
          ch->ele_attack = ATTACK_THUNDER;
          ch->attack_var = HIT;
          ch->attack_focus = OVERHEAD;
          hurt_person(ch,victim,1000000,TRUE);
       }
       else {
               act("$N barely rolls under your POWERFUL high claw.",ch,NULL,victim,TO_CHAR);
               act("You barely roll under $n's POWERFUL high claw.",ch,NULL,victim,TO_VICT);
               act("$N barely rolls under $n's POWERFUL high claw.",ch,NULL,victim,TO_NOTVICT);
            }
}

void do_tailsweep( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);

    if ( !IS_CLASS(ch,CLASS_DRAGON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_DRAGONPOWER(ch, DRAGON_TAIL) )
       {
          send_to_char("You need a tail to do this skill.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to Tail Sweep?\n\r", ch );
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

    if ( number_percent() > -1 )
       {
          act("You ERADICATE $N with your powerful tail sweep.",ch,NULL,victim,TO_CHAR);
          act("$n ERADICATES you with a very powerful tail sweep.",ch,NULL,victim,TO_VICT);
          act("$n ERADICATES $N with $s powerful tail sweep.",ch,NULL,victim,TO_NOTVICT);

          if ( number_percent() > 94 && !is_martial( victim ) )
             victim->wait += 12;

          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = HIT;
          ch->attack_focus = LOW;
          hurt_person( ch, victim, 1000000, TRUE );
       }
       else {
               act("$N swiftly leaps over your swooping tail.",ch,NULL,victim,TO_CHAR);
               act("You swiftly leaps over $n's swooping tail.",ch,NULL,victim,TO_VICT);
               act("$N swiftly leaps over $n's swooping tail.",ch,NULL,victim,TO_NOTVICT);
            }
}

void do_shoulderrush( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);

    if ( !IS_CLASS(ch,CLASS_DRAGON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to knock down?\n\r", ch );
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

    if ( number_percent() > -1 )
       {
          act("You KNOCK $N off $S feet as you crash directly into $M.",ch,NULL,victim,TO_CHAR);
          act("$n KNOCKS you off your feet as $s crashes into you.",ch,NULL,victim,TO_VICT);
          act("$n KNOCKS $N off $S feet with a swift shoulder rush.",ch,NULL,victim,TO_NOTVICT);

          if ( ch->ctimer[15] == 0 )
             victim->move = UMAX( 0, victim->move - 500000 );

          ch->attack_type = ATTACK_PHYSICAL;
          ch->ele_attack = ATTACK_FIRE;
          ch->attack_var = HIT;
          ch->attack_focus = DIRECT;
          hurt_person( ch, victim, 1000000, TRUE );
       }
       else {
               act("$N suddenly step aside dodging your rush.",ch,NULL,victim,TO_CHAR);
               act("You suddenly step aside dodging $n's rush.",ch,NULL,victim,TO_VICT);
               act("$N suddenly steps aside dodging $n's rush.",ch,NULL,victim,TO_NOTVICT);
            }
}

void do_stomp( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if ( !IS_CLASS(ch,CLASS_DRAGON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to stomp on?\n\r", ch );
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

    if ( number_percent() > -1 )
       {
          act("You VICIOUSLY STOMP $N onto the ground.",ch,NULL,victim,TO_CHAR);
          act("$n VICIOUSLY STOMPS you onto the ground.",ch,NULL,victim,TO_VICT);
          act("$n VICIOUSLY STOMPS $N onto the ground.",ch,NULL,victim,TO_NOTVICT);

          if ( ch->ctimer[15] == 0 )
            victim->mana = UMAX( 0, victim->mana - 500000 );

          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = HIT;
          ch->attack_focus = AERIAL;
          hurt_person(ch,victim,1000000,TRUE);
       }
       else {
               act("$N suddenly step aside dodging your stomp.",ch,NULL,victim,TO_CHAR);
               act("You suddenly step aside dodging $n's stomp.",ch,NULL,victim,TO_VICT);
               act("$N suddenly steps aside dodging $n's stomp.",ch,NULL,victim,TO_NOTVICT);
            }
}

void do_hyper_claw( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);

    if ( !IS_CLASS(ch,CLASS_DRAGON) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }
 
    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to High Claw?\n\r", ch );
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
    subtract_stock( ch, 100 );
    SET_BIT( ch->arena, AFF2_DM );

    act("#PYou #Craise your claw in the air as #yshards #Cof #wlight #Cradiate of it...#n",ch,NULL,NULL,TO_CHAR);
    act("#P$n #Craises $s claw in the air as #yshards #Cof #wlight #Cradiate of it...#n",ch,NULL,NULL,TO_ROOM);
    act("#PYou #Cthen #wflash #Cyour claw across #g$N #Cas the #wlight #Crips through $S body!!#n",ch,NULL,victim,TO_CHAR);
    act("#P$n #Cthen #wflashes #C$s claw across #gyou #Cas the #wlight #Crips through your body!!#n",ch,NULL,victim,TO_VICT);
    act("#P$n #Cthen #wflashes #C$s claw across #g$N #Cas the #wlight #Crips through $S body!!#n",ch,NULL,victim,TO_NOTVICT);
    act("#PYou #RANNIHILATE #g$N #Cwith your high claw.#n",ch,NULL,victim,TO_CHAR);
    act("#P$n #RANNIHILATES #gyou #Cwith a very deadly high claw.#n",ch,NULL,victim,TO_VICT);
    act("#P$n #RANNIHILATES #g$N #Cwith $s high claw.#n",ch,NULL,victim,TO_NOTVICT);

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = OVERHEAD;

    if ( ch->clannum == 1 )
       hurt_person(ch,victim,1250000,TRUE);
       else hurt_person(ch,victim,1000000,TRUE);
}

void do_dragon_flare( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int count;
  int dam;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch,CLASS_DRAGON) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 200 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( !IS_DRAGONPOWER(ch, DRAGON_BREATH) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  act("You spread your wings widely as you fly into the sky!",ch,NULL,NULL,TO_CHAR);
  act("$n spreads $s wings widely as $e flies into the sky!",ch,NULL,NULL,TO_ROOM);
  act("The sky goes dark as thunderstorms forms around you.",ch,NULL,NULL,TO_CHAR);
  act("The sky goes dark as thunderstorms fill the horizon.",ch,NULL,NULL,TO_ROOM);
  act("A shadowy figure flies on the edge of the clouds, then dives back into its misty protection.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, You zoom through the clouds straight towards the earth!",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, the clouds parted as a huge dragon zooms straight towards the earth!",ch,NULL,NULL,TO_ROOM);
  act("At the last second, you stop your downwards path by hovering loudly.",ch,NULL,NULL,TO_CHAR);
  act("At the last second, $n stops it downwards path by hovering over you.",ch,NULL,NULL,TO_ROOM);
  act("You release a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_CHAR);
  act("$n releases a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_ROOM);
  act("You release a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_CHAR);
  act("$n releases a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_ROOM);
  act("You release a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_CHAR);
  act("$n releases a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_ROOM);

  act("You draw in your breath as a manifest of energy forms on your mouth.",ch,NULL,NULL,TO_CHAR);
  act("$n draws in it breath as a manifest of energy forms on its mouth.",ch,NULL,NULL,TO_ROOM);
  act("The mass of energy grows bigger as you struggle to hold it in place.",ch,NULL,NULL,TO_CHAR);
  act("The mass of energy grows bigger as $n struggles to hold it in place.",ch,NULL,NULL,TO_ROOM);
  act("Unbelievable energy emits of you as blinding light fills the room.",ch,NULL,NULL,TO_CHAR);
  act("Unbelievable energy emits of $n as blinding light fills the room.",ch,NULL,NULL,TO_ROOM);
  act("You release the huge manifest of energy as it ROARS TOWARDS THE GROUND!",ch,NULL,NULL,TO_CHAR);
  act("$n releases the huge manifest of energy as it ROARS TOWARDS THE GROUND!",ch,NULL,NULL,TO_ROOM);
  act("A HUGE EXPLOSION DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!",ch,NULL,NULL,TO_CHAR);
  act("A HUGE EXPLOSION DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!",ch,NULL,NULL,TO_ROOM);

  ch->wait = 12;
  subtract_stock( ch, 200 );
  SET_BIT( ch->arena, AFF2_DM );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe( ch, vch ) )
        continue;

     if ( !IS_AFFECTED( vch, AFF_FLAMING ) ) 
        SET_BIT( vch->affected_by, AFF_FLAMING );

     act("Your body evaporates as the hungry flames SCORCHES your defenseless body!",vch,NULL,NULL,TO_CHAR);
     ch->ele_attack = ATTACK_FIRE;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = BLAST;
     ch->attack_focus = AERIAL;

     if ( ch->clannum == 1 )
        hurt_person( ch, vch, 2500000, TRUE );
        else hurt_person( ch, vch, 2000000, TRUE );
  }
}

void do_final_flare( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  DESCRIPTOR_DATA *d;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch,CLASS_DRAGON) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( get_stock( ch ) < 300 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }

  if ( !IS_DRAGONPOWER(ch, DRAGON_BREATH) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  ch->wait = 12;
  subtract_stock( ch, 300 );
  SET_BIT( ch->arena, AFF2_DM );

  act("You spread your wings widely as you fly off into space!",ch,NULL,NULL,TO_CHAR);
  act("$n spreads $s wings widely as $e flies off into space!",ch,NULL,NULL,TO_ROOM);
  act("You swiftly circle around the planet as your body shimmers.",ch,NULL,NULL,TO_CHAR);
  act("Stopping suddenly, you tilt your head backwards looking at the planet below.",ch,NULL,NULL,TO_CHAR);
  act("You swiftly open your lower left wing!",ch,NULL,NULL,TO_CHAR);
  act("You swiftly open your lower right wing!",ch,NULL,NULL,TO_CHAR);
  act("You swiftly open your centre left wing!",ch,NULL,NULL,TO_CHAR);
  act("You swiftly open your centre right wing!",ch,NULL,NULL,TO_CHAR);
  act("You swiftly open your upper left wing!",ch,NULL,NULL,TO_CHAR);
  act("You swiftly open your upper right wing!",ch,NULL,NULL,TO_CHAR);
  act("Roaring loudly, you draw in your breath as a manifest of energy forms on your mouth.",ch,NULL,NULL,TO_CHAR);
  act("A loud humming fills the air as the sunlight flows through your body, illuminating it.",ch,NULL,NULL,TO_CHAR);
  act("The mass of energy grows bigger as you struggle to hold it in place.",ch,NULL,NULL,TO_CHAR);
  act("The mass of energy GROWS bigger as you struggle to hold it in place.",ch,NULL,NULL,TO_CHAR);
  act("The mass of energy GROWS EVEN BIGGER as you struggle to hold it in place.",ch,NULL,NULL,TO_CHAR);
  act("Unbelievable energy emits of you as blinding flashes towards the earth.",ch,NULL,NULL,TO_CHAR);
  act("You see a small twinkle in the sky.",ch,NULL,NULL,TO_ROOM);
  act("You finally release the huge manifest of energy as it ROARS TOWARDS THE EARTH!",ch,NULL,NULL,TO_CHAR);

  act("A HUGE EXPLOSION FROM ABOVE DEVASTATES the vicinity below you, BURSTING everything into OBLIVION!!!",ch,NULL,NULL,TO_CHAR);
  act("A HUGE EXPLOSION FROM ABOVE DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !IS_NPC( vch ) || is_safe(ch,vch) )
        continue;

     act("Your body evaporates as the explosion ANNIHILATES YOU!",vch,NULL,NULL,TO_CHAR);
     act("$n's body evaporates as a tremedous explosion ANNIHILATES $m!",vch,NULL,NULL,TO_ROOM);
     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = BEAM;
     ch->attack_focus = AERIAL;

     if ( ch->clannum == 1 )
        hurt_person( ch, vch, 4000000, TRUE );
        else hurt_person( ch, vch, 3500000, TRUE );
  }

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
       if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	 {
           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) || IS_CLASS( d->character, CLASS_DUERGAR ) )
              continue;

           act("Your body evaporates as the explosion ANNIHILATES YOU!",d->character,NULL,NULL,TO_CHAR);
           act("$n's body evaporates as a tremedous explosion ANNIHILATES YOU!",d->character,NULL,NULL,TO_ROOM);
           ch->attack_type = ATTACK_PHYSICAL;
           ch->attack_var = BEAM;
           ch->attack_focus = AERIAL;

           if ( ch->clannum == 1 )
              hurt_person( ch, d->character, 4000000, TRUE );
              else hurt_person( ch, d->character, 3500000, TRUE );
       }
  }
}

void do_breach( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
     return;

  if ( !IS_CLASS(ch,CLASS_DRAGON) )
     {
        stc( "Huh?\n\r", ch );
        return;
     }

  if ( IS_SET(ch->more, MORE_SAFE) )
     {
        REMOVE_BIT(ch->more, MORE_SAFE);
        act( "The barrier encasing your body vanishes.",  ch, NULL, NULL, TO_CHAR );
        act( "The barrier encasing $n body vanishes.",  ch, NULL, NULL, TO_ROOM );
        return;
     }

  if ( ch->fight_timer > 0 ) 
     {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
     }

  SET_BIT(ch->more, MORE_SAFE);
  act( "You raise your finger to your forehand as you surround yourself with an energy barrier.",  ch, NULL, NULL, TO_CHAR    );
  act( "$n raises $s finger to $s forehand as $e surrounds $mself with an energy barrier.",  ch, NULL, NULL, TO_ROOM    );
  ch->wait = 12;
}

void do_hyper_drive( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    AFFECT_DATA af;
    int sn;

    if ( ( sn = skill_lookup( "hyper drive" ) ) < 0 )
       return;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_DRAGON) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 15 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->mana < 5000000 )
       {
          stc("You don't have enough magical energy to do this technique.\n\r",ch);
          return;
       }
    
    ch->mana -= 5000000;
    ch->wait = 6;

    act("You clench your hand as it radiates off demonic energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it radiates off demonic energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yI don't care if my fangs are broken... Fight till you Die!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yI don't care if my fangs are broken... Fight till you Die!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("You then SmAsH your fist into the ground as it vibrates beneath you.",ch,NULL,NULL,TO_CHAR);
    act("$n thenes SmAsHeS $s fist into the ground as it vibrates beneath you.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly a powerful beam of energy RiP the ground apart as it engulfs all!!!",ch,NULL,NULL,TO_CHAR);
    act("Suddenly a powerful beam of energy RiP the ground apart as it engulfs all!!!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_NPC(vch) )
          continue;

       if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom > 0 ) )
          continue;

       if ( is_affected( vch, sn ) )
          continue;

       act("You cackle with glee as you feel your powers grow.",vch,NULL,NULL,TO_CHAR);
       act("$n cackles with glee as $e feels $s powers grow!",vch,NULL,NULL,TO_ROOM);

       af.type      = sn;
       af.duration  = 50;
       af.modifier  = 0;
       af.location  = 0;
       af.bitvector = 0;
       affect_to_char( vch, &af );
    }
}
