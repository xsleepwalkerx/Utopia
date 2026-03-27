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

void my_legend( CHAR_DATA *ch )
{
   if ( ch->pcdata->legend >= 10 )
      return;
        
   ch->max_hit += 300000;
   ch->pcdata->legend++;

   if ( ch->pcdata->legend == 3 || ch->pcdata->legend == 6 || ch->pcdata->legend == 9 )
      {
         ch->max_mana += 1000000;
         ch->max_move += 1000000;
      }
}

void show_delf( CHAR_DATA *ch )
{
   char buf[MAX_INPUT_LENGTH];

   act( "You concentrate softly as dark energy manifests inbetween your hands.", ch, NULL, NULL, TO_CHAR );
   act( "$n concentrates softly as dark energy manifests inbetween $s hands.", ch, NULL, NULL, TO_ROOM );
   act( "You mumble softly in the ancient language of Lolth as your eyes blazes.", ch, NULL, NULL, TO_CHAR );
   act( "$n mumbles softly in the ancient language of Lolth as $s eyes blazes.", ch, NULL, NULL, TO_ROOM );
   act( "You find yourself standing in the presence of Lolth as your eyes open wide with fear.", ch, NULL, NULL, TO_CHAR );
   act( "Lolth's voice fills the room with venom saying '#yI know why you are here.#n'", ch, NULL, NULL, TO_CHAR );
   act( "Lolth continues '#yFor the first time in your life pitiful creature...#n'", ch, NULL, NULL, TO_CHAR );
   act( "Lolth finishes '#yYou have made the right choice.#n'", ch, NULL, NULL, TO_CHAR );
   act( "Lolth suddenly open her hand as a blade of light fades into existence.", ch, NULL, NULL, TO_CHAR );
   act( "Grabbing the blade, she DrIveS it into you as everything becomes brilliant!!!", ch, NULL, NULL, TO_CHAR );
   act( "As the light clears, you find yourself back on Utopia in a new body.", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly fades back into view with $s hair grey and $s skin dark...", ch, NULL, NULL, TO_ROOM );
   act( "You are now a #edrow#n.", ch, NULL, NULL, TO_CHAR );

   sprintf(buf,"#wThe #Csky #ggoes #edark #gas #y%s #Psummons #eLloth#g!#n", ch->pcdata->switchname );
   do_info( ch, buf );
}

void do_remort( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   int bonus = number_range( 1, 5 );
   DESCRIPTOR_DATA *d;
   CHAR_DATA *lucky = NULL;

   if ( IS_NPC( ch ) )
      return;

   if ( IS_CLASS( ch, CLASS_SNOW_ELF ) || IS_CLASS( ch, CLASS_PHOENIX ) || IS_CLASS( ch, CLASS_HOBBIT )
        || IS_CLASS( ch, CLASS_DUERGAR ) || IS_CLASS( ch, CLASS_WISP ) || IS_CLASS( ch, CLASS_WIZARD ) 
        || IS_CLASS( ch, CLASS_DARAMON ) || IS_CLASS( ch, CLASS_MONK ) || IS_CLASS( ch, CLASS_NECROMANCER )
        || IS_CLASS( ch, CLASS_SHADOW ) || IS_CLASS( ch, CLASS_DRAGOON ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->legend < 10 )
      {
         stc("You aren't worthy enough to summon forth the legendary dragon.\n\r",ch);
         return;
      }

   if ( ch->class_2 != 0 )
      {
         stc("#eYou have already chosen a #Pdeity #eor #worganization #eto follow.#n\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp("sea",arg) )
      {
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA );
         stc( "#LYou place your trust in the Blue Sea Dragon.#n\n\r", ch );
      }
      else if ( !str_cmp("redeyed",arg) )
      {
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE );
         stc( "#RYou place your trust in the Red Eyed Dragon.#n\n\r", ch );
      }
      else if ( !str_cmp("jade",arg) )
      {
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE );
         stc( "#gYou place your trust in the Jade Dragon.#n\n\r", ch );
      }
      else if ( !str_cmp("holy",arg) )
      {
         if ( ch->alignment < 0 )
            {
               stc( "#wThe Silver Dragon doesn't obey the followers of #eEvil#w.#w\n\r", ch );
               return;
            }

         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY );
         ch->alignment = 1000;
         stc( "#wYou place your trust in the Silver Dragon.#n\n\r", ch );
      }
      else if ( !str_cmp("dark",arg) )
      {
         if ( ch->alignment > 0 )
            {
               stc( "#eThe Dark Dragon doesn't obey the followers of #wGood#e.#w\n\r", ch );
               return;
            }

         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK );
         ch->alignment = -1000;
         stc( "#eYou place your trust in the Dark Dragon.#n\n\r", ch );
      }
      else if ( !str_cmp("garou",arg) )
      {
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         ch->class_2 = CLASS_GAROU;
         do_update_class_count( ch->class_2, TRUE );
         save_char_obj( ch );

         stc( "#oYou are now a #yGarou#o... On your forehead is the #Rmark #oof the wolves.#n\n\r", ch );
         sprintf(buf,"#wThe #Csky #ggoes #edark #gas #y%s #Psummons #oGaia#g!#n", ch->pcdata->switchname );
         do_info( ch, buf );

         return;
      }
      else if ( !str_cmp("clerical",arg) )
      {
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         ch->class_2 = CLASS_DARK_ELF;
         ch->pcdata->powers[DROW_CLASS] = DROW_CLERICAL;
         show_delf( ch );
         return;
      }
      else if ( !str_cmp("magi",arg) )
      {
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         ch->class_2 = CLASS_DARK_ELF;
         ch->pcdata->powers[DROW_CLASS] = DROW_MAGI;
         show_delf( ch );
         return;
      }
      else if ( !str_cmp("militia",arg) )
      {
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         ch->class_2 = CLASS_DARK_ELF;
         ch->pcdata->powers[DROW_CLASS] = DROW_MILITIA;
         show_delf( ch );
         return;
      }
      else if ( !str_cmp("fiend",arg) )
      {
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         ch->class_2 = CLASS_FIEND;

         stc( "#eYou sell your soul to #RLucifier#e... Your wish had been granted.#n\n\r", ch );
         sprintf(buf,"#wThe #Csky #ggoes #edark #gas #y%s #Psummons #RLucifier#g!#n", ch->pcdata->switchname );
         do_info( ch, buf );

         return;
      }
      else if ( !str_cmp("commander",arg) )
      {
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         ch->class_2 = CLASS_KURUDA;

         stc( "#wYou #eare now a #yCommander#e of Nests... Do the organization proud.#n\n\r", ch );
         sprintf(buf,"#wThe #Csky #ggoes #edark #gas #y%s #Pjoins #eNests#g!#n", ch->pcdata->switchname );
         do_info( ch, buf );

         return;
      }
      else {
              stc( "#pChoices for #wdragoon specialization #pare : #RRedeyed#w, #gJade#w, #LSea#w, #eDark#w, Holy.#n\n\r", ch );
              stc( "#pChoices for #wdark elf specialization #pare: #wClerical, #CMagi, #eMilitia.#n\n\r", ch );
              stc( "#pOther choices are                          : #oGarou, #RFiend, #cCommander.#n\n\r", ch );
              return;
           }

   ch->pcdata->powers[DRAGOON_LEVEL] = 1;
   ch->class_2 = CLASS_DRAGON_KNIGHT;
   do_update_class_count( ch->class_2, TRUE );
   save_char_obj( ch );

   act( "You hover over the relics as you grin softly.", ch, NULL, NULL, TO_CHAR );
   act( "$n hovers over the relics as $e grins softly.", ch, NULL, NULL, TO_ROOM );
   act( "You mutter '#yAt long last, I can wish for what I most desire...#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n mutters '#yAt long last, I can wish for what I most desire...#n'", ch, NULL, NULL, TO_ROOM );
   act( "The artifacts starts to pulse with energy as you quickly draw in your breath.", ch, NULL, NULL, TO_CHAR );
   act( "The artifacts starts to pulse with energy as $n quickly draws in $s breath.", ch, NULL, NULL, TO_ROOM );
   act( "You yell '#yIde yo, Bahamut!#n' as the sky suddenly darkens.", ch, NULL, NULL, TO_CHAR );
   act( "$n yells '#yIde yo, Bahamut!#n' as the sky suddenly darkens.", ch, NULL, NULL, TO_ROOM );
   act( "The water turns red as unbelievable lightning FlAsHeS around the vicinity.", ch, NULL, NULL, TO_CHAR );
   act( "The water turns red as unbelievable lightning FlAsHeS around the vicinity.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly a massive beam shimmers from the relics as a sprut of flames erupt from them.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly a massive beam shimmers from the relics as a sprut of flames erupt from them.", ch, NULL, NULL, TO_ROOM );
   act( "The flames encircle around the entire vicinity as it forms into the shape of a long dragon.", ch, NULL, NULL, TO_CHAR );
   act( "The flames encircle around the entire vicinity as it forms into the shape of a long dragon.", ch, NULL, NULL, TO_ROOM );
   act( "The room is filled with light for a instant as the Dragon appears before you.", ch, NULL, NULL, TO_CHAR );
   act( "The room is filled with light for a instant as the Dragon appears before $n.", ch, NULL, NULL, TO_ROOM );
   act( "Bahamut flaps his mighty wings as he soars slightly above you.", ch, NULL, NULL, TO_CHAR );
   act( "Bahamut flaps his mighty wings as he soars slightly above $n.", ch, NULL, NULL, TO_ROOM );
   act( "Time seem to stand still as sparks of electricity fills the room...", ch, NULL, NULL, TO_CHAR );
   act( "Time seem to stand still as sparks of electricity fills the room...", ch, NULL, NULL, TO_ROOM );
   act( "#RSuddenly Bahamut screams as the entire room ERUPTS INTO DEADLY FLAMES!!!#n", ch, NULL, NULL, TO_CHAR );
   act( "#RSuddenly Bahamut screams as the entire room ERUPTS INTO DEADLY FLAMES!!!#n", ch, NULL, NULL, TO_ROOM );

   sprintf(buf,"#wThe #Csky #ggoes #edark #gas #y%s #Psummons #gthe #wDivine #RDragon#g!#n", ch->pcdata->switchname );
   do_info( ch, buf );

   return;

   switch( bonus )
   {
      case 4: do_info( NULL, "#wThe #yrealm #wis refreshed by #RBahamut's #wDivine Power!#n" );
              break;
      case 1: do_info( NULL, "#yLord #RBahamut #yfills you all with #RTREMEDOUS #ypower!#n" );
              break;
      case 3: do_info( NULL, "#PYour hidden powers has now been released by Lord #RBahamut!#n" );
              break;
      case 2: do_info( NULL, "#CYour fame as warriors has been revealed by Lord #RBahamut!#n" );
              break;
      case 5: bonus = 0;

              for ( d = descriptor_list; d != NULL; d = d->next )
              {
                 if ( d->character == NULL || IS_IMMORTAL( d->character ) )
                    continue;

                 bonus++;
              }

              if ( bonus == 1 )
                 do_info( NULL, "#yLord #RBahamut #yfills you all with #RTREMEDOUS #ypower!#n" );
                 else {
                         int lucky_choice, count;

                         while( lucky == NULL )
                         {
                            lucky_choice = number_range( 1, bonus );
                            count = 0;

                            for ( d = descriptor_list; d != NULL; d = d->next )
                            {
                               if ( d->character == NULL || IS_IMMORTAL( d->character )
                                    || IS_CLASS( d->character, CLASS_DRAGON_KNIGHT ) )
                                  continue;

                               count++;

                               if ( lucky_choice == count )
                                  {
                                      if ( d->character != ch )
                                         lucky = d->character;

                                      break;
                                  }
                            }
                         }

                         sprintf(buf,"#LIn honour of this special day, Lord #RBahamut also grants #w%s #Lthe power of the Dragoons!#n", lucky->pcdata->switchname );
                         do_info( NULL, buf );

                         while ( lucky->pcdata->legend < 10 )
                         {
                            my_legend( lucky );
                         }

                         return;
                      }

              break;
   }   

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      int count;

      if ( d->character == NULL || IS_IMMORTAL( d->character ) 
           || d->character == ch || IS_CLASS( d->character, CLASS_DRAGON_KNIGHT ) )
         continue;

      if ( ( get_age( d->character ) - 17 ) < 2 )
         continue;

      switch( bonus )
      {
         default:
         case 4: restore_char( d->character ); give_bm_affect( d->character ); d->character->pcdata->blood_meter = 200; break;
         case 1: d->character->race += number_range( 5, 10 ); break;
         case 3: d->character->pkill += 5; break;
         case 2: my_legend( d->character ); break;
      }
   }
}

void do_dragoon_learn( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char lin[MAX_STRING_LENGTH];
   int inpart = 0;
   int cost = 1000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DRAGON_KNIGHT) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg );

   sprintf(lin,"#c===============================================================================#n\n\r");

   if ( arg[0] == '\0' )
      {
          stc( " #PSyntax: Learn <ability> <last word of option>\n\r#n",ch);
          stc( lin, ch );
          stc( "                          #wGifts of the Renegade Knight#n\n\r",ch);
          stc( lin, ch );

          sprintf( buf,"      #PWeapon Guard     [#y%s#P]   Evade            [#y%s#P]   Weapon Weld      [#y%s#P]#n\n\r",
                   ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_GUARD ) ) ? "*" : " ",
                   ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_EVADE ) ) ? "*" : " ",
                   ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_IMPROVE ) ) ? "*" : " " );
          stc( buf, ch );

          stc( lin, ch );
          stc( "                     #wDragoon Addition table for all Weapons#n\n\r",ch);
          stc( lin, ch );

          sprintf( buf,"      #PWhip Smack     [#y%3d#P]   More & More    [#y%3d#P]   Hard Blade     [#y%3d#P]#n\n\r",
                   ch->pcdata->powers[DRAGOON_RCOMBO1], ch->pcdata->powers[DRAGOON_RCOMBO2], ch->pcdata->powers[DRAGOON_RCOMBO3] );
          stc( buf, ch );

          sprintf( buf,"      #PHarpoon        [#y%3d#P]   Spinning Cane  [#y%3d#P]   Rod Typhoon    [#y%3d#P]#n\n\r",
                   ch->pcdata->powers[DRAGOON_LCOMBO1], ch->pcdata->powers[DRAGOON_LCOMBO2], ch->pcdata->powers[DRAGOON_LCOMBO3] );
          stc( buf, ch );

          sprintf( buf,"      #PGust of Wind   [#y%3d#P]   Double Slash   [#y%3d#P]   Volcano        [#y%3d#P]#n\n\r",
                   ch->pcdata->powers[DRAGOON_LCOMBO4], ch->pcdata->powers[DRAGOON_SCOMBO1], ch->pcdata->powers[DRAGOON_SCOMBO2] );
          stc( buf, ch );

          sprintf( buf,"      #PBurning Rush   [#y%3d#P]   Crush Dance    [#y%3d#P]   Madness Hero   [#y%3d#P]#n\n\r",
                   ch->pcdata->powers[DRAGOON_SCOMBO3], ch->pcdata->powers[DRAGOON_SCOMBO4], ch->pcdata->powers[DRAGOON_SCOMBO5] );
          stc( buf, ch );

          sprintf( buf,"                             #PMoon Strike    [#y%3d#P]#n\n\r",
                   ch->pcdata->powers[DRAGOON_SCOMBO6] );
          stc( buf, ch );

          stc( lin, ch );

          sprintf( buf,"                     #wYour #PDragoon #yLevel #wis currently at #P%d.#n\n\r",
                   ch->pcdata->powers[DRAGOON_LEVEL] );
          stc( buf, ch );

          sprintf( buf,"                     #wYour #PDragoon #ySpirit #wis currently #P%d#w/#P%d.#n\n\r",
                   ch->pcdata->powers[DRAGOON_SPIRIT], ch->pcdata->powers[DRAGOON_LEVEL] * 100 );
          stc( buf, ch );
          stc( lin, ch );
          return;
      }

   if ( !str_cmp(arg1, "ability" ) )
      {
          if ( !str_cmp( arg, "weld" ) )
             {
                inpart = DRAGOON_IMPROVE;
                cost = 10000000;
             }
             else if ( !str_cmp( arg, "evade" ) )
             {
                inpart = DRAGOON_EVADE;
                cost = 50000000;
             }
             else if ( !str_cmp( arg, "guard" ) )
             {
                inpart = DRAGOON_GUARD;
                cost = 1000000;
             }
             else {
                     do_dragoon_learn( ch, "" );
                     return;
                  }

          if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], inpart ) )
             {
                send_to_char("You have already bought this power.\n\r",ch);
                return;
             }

          if ( ch->exp < cost )
             {
                send_to_char("You have insufficient experience to buy this power.\n\r",ch);
                return;
             }

          stc( "#PYou #cmake a #ysacrifice #cto the #wdivine #Pdragon #cas you are granted a new #Ppower#c.#n\n\r", ch );
          SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], inpart );
          ch->exp -= cost;
          return;
      } 

   do_dragoon_learn( ch, "" );
}

void do_weapon_weld( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   OBJ_DATA *obj; 

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_IMPROVE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   argument = one_argument(argument,arg);

   if ( arg[0] == '\0' )
      {
         send_to_char("Improve which weapon?\n\r",ch);
         return;
      }
     
   if ( ( obj= get_obj_carry(ch,arg) ) == NULL )
      {
         send_to_char("You dont have that weapon.\n\r",ch);
         return;
      }
 
   if ( obj->item_type != ITEM_WEAPON || IS_SET( obj->weapflags, WEAPON_JAGGED ) )
      {
         send_to_char("You are unable to jag this weapon.\n\r",ch);
         return;
      }

   if ( obj->value[3] > 3 || obj->value[3] < 1 )
      {
         send_to_char("You are unable to jag this weapon.\n\r",ch);
         return;
      }

   obj->value[1] += 30000;
   obj->value[2] += 30000;
   SET_BIT( obj->weapflags, WEAPON_JAGGED );
 
   act( "You take out a small cutting stone skillfully jagging the edge of $p.", ch, obj,NULL, TO_CHAR );
   act( "$n takes out a small cutting stone skillfully jagging the edge of $p.", ch, obj,NULL, TO_ROOM );
}

void can_dragoon_level( CHAR_DATA *ch )
{
   bool test = FALSE;

   if ( IS_NPC( ch ) )
      return;

   if ( ch->pcdata->powers[DRAGOON_LEVEL] < 2 && ch->pcdata->powers[DRAGOON_SPS] >= 200 )
      test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 3 && ch->pcdata->powers[DRAGOON_SPS] >= 500 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 4 && ch->pcdata->powers[DRAGOON_SPS] >= 1000 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 5 && ch->pcdata->powers[DRAGOON_SPS] >= 2000 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 6 && ch->pcdata->powers[DRAGOON_SPS] >= 4000 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 7 && ch->pcdata->powers[DRAGOON_SPS] >= 8000 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 8 && ch->pcdata->powers[DRAGOON_SPS] >= 10000 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 9 && ch->pcdata->powers[DRAGOON_SPS] >= 15000 )
              test = TRUE;
      else if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 && ch->pcdata->powers[DRAGOON_SPS] >= 20000 )
              test = TRUE;

   if ( test == TRUE )
      {
         ch->pcdata->powers[DRAGOON_LEVEL]++;
         stc( "#wYour #Pdragoon #ylevel #ghas #C#zimproved#n#g!!!#n\n\r", ch );

         if ( ch->pcdata->powers[DRAGOON_LEVEL] == 3 || ch->pcdata->powers[DRAGOON_LEVEL] == 5 ||
              ch->pcdata->powers[DRAGOON_LEVEL] == 7 || ch->pcdata->powers[DRAGOON_LEVEL] == 10 )
            stc( "#cYour #yhave #C#zlearned#n #ya #Rnew #Pdragoon #ymagic!!!#n\n\r", ch );
      }
}

void do_whip_smack( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 1 )
      {
         send_to_char( "But you are not using a #eRapier#n.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your rapier into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_RCOMBO1] < 100 )
      ch->pcdata->powers[DRAGOON_RCOMBO1] += 20;

   dam = 250000;
   level = ( ch->pcdata->powers[DRAGOON_RCOMBO1] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.0;
              break;

      case 2: multi = 1.25;
              break;

      case 3: multi = 1.5;
              break;

      case 4: multi = 1.75;
              break;

      default:
      case 5: multi = 2.0;
              break;
   }

   spgain = 35;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_RCOMBO1] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you dash towards $N, $E barely evades your driving blade by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n dashes towards you, you barely evades $s driving blade by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n dashes towards $N, $E barely evades $s driving blade by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You dash towards $N as you thrust your blade into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n dashes towards you as $e thrusts $s blade into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n dashes towards $N as $e thrusts $s blade into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 2;
                    spgain /= 2;
                 }
           }

   if ( !miss )
      {
         act("Suddenly you slash your rapier across $N's chest screaming '#yWhip Smack!#n'",ch,NULL,victim,TO_CHAR);
         act("Suddenly $n slashes $s rapier across your chest screaming '#yWhip Smack!#n'",ch,NULL,victim,TO_VICT);
         act("Suddenly $n slashes $s rapier across $N's chest screaming '#yWhip Smack!#n'",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_more_and_more( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 1 )
         {
            send_to_char( "But you are not using a #eRapier#n.\n\r", ch );
            return;
         }

   if ( ch->move < 50000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your rapier into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_RCOMBO2] < 100 )
      ch->pcdata->powers[DRAGOON_RCOMBO2] += 20;

   dam = 350000;
   level = ( ch->pcdata->powers[DRAGOON_RCOMBO2] / 20 ) + 1;

   switch( level )
   {
      case 1: spgain = 30;
              break;

      case 2: spgain = 45;
              break;

      case 3: spgain = 60;
              break;

      case 4: spgain = 75;
              break;

      default:
      case 5: spgain = 102;
              break;
   }

   multi = 1.5;
   ch->move -= 50000;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_RCOMBO2] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you dash towards $N, $E barely evades your driving blade by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n dashes towards you, you barely evades $s driving blade by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n dashes towards $N, $E barely evades $s driving blade by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You dash towards $N as you thrust your blade into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n dashes towards you as $e thrusts $s blade into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n dashes towards $N as $e thrusts $s blade into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 3;
                    spgain /= 3;
                 }
           }

   if ( !miss )
      {
         act("You scream '#yDie!#n' as you thrust your blade right into $N's stomach!",ch,NULL,victim,TO_CHAR);
         act("$n screams '#yDie!#n' as $e thrusts $s blade right into your stomach!",ch,NULL,victim,TO_VICT);
         act("$n screams '#yDie!#n' as $e thrusts $s blade right into $N's stomach!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 3;
               spgain = ( spgain * 2 ) / 3;
            }
      }

   if ( !miss )
      {
         act("You whip your blade up $N's body screaming '#yMore & More!#n' as you glare evilly.",ch,NULL,victim,TO_CHAR);
         act("$n whips $s blade up your body screaming '#yMore & More!#n' as $e glares evilly.",ch,NULL,victim,TO_VICT);
         act("$n whips $s blade up $N's body screaming '#yMore & More!#n' as $e glares evilly.",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_hard_blade( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 1 )
         {
            send_to_char( "But you are not using a #eRapier#n.\n\r", ch );
            return;
         }

   if ( ch->move < 50000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your rapier into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_RCOMBO3] < 100 )
      ch->pcdata->powers[DRAGOON_RCOMBO3] += 20;

   dam = 200000;
   level = ( ch->pcdata->powers[DRAGOON_RCOMBO3] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.0;
              break;

      case 2: multi = 1.5;
              break;

      case 3: multi = 2.0;
              break;

      case 4: multi = 2.5;
              break;

      default:
      case 5: multi = 3.0;
              break;
   }

   spgain = 35;
   ch->move -= 50000;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_RCOMBO3] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you dash towards $N, $E barely evades your driving blade by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n dashes towards you, you barely evades $s driving blade by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n dashes towards $N, $E barely evades $s driving blade by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You dash towards $N as you thrust your blade into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n dashes towards you as $e thrusts $s blade into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n dashes towards $N as $e thrusts $s blade into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 6;
                    spgain /= 6;
                 }
           }

   if ( !miss )
      {
         act("You then drive your blade downwards $N's chest as you tilt yourself to the ground.",ch,NULL,victim,TO_CHAR);
         act("$n then drives $s blade downwards your chest as $e tilts $mself to the ground.",ch,NULL,victim,TO_VICT);
         act("$n then drives $s blade downwards $N's chest as $e tilts $mself to the ground.",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 6;
               spgain = ( spgain * 2 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You twist your body as you leap into the air and slash $N viciously down $S body again!",ch,NULL,victim,TO_CHAR);
         act("$n twists $s body as $e leaps into the air and slashes you viciously down your body again!",ch,NULL,victim,TO_VICT);
         act("$n twists $s body as $e leaps into the air and slashes $N viciously down $S body again!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 3 ) / 6;
               spgain = ( spgain * 3 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You quickly slash across $N's stomach causing $M to yelp in pain.",ch,NULL,victim,TO_CHAR);
         act("$n quickly slashes across your stomach causing you to yelp in pain.",ch,NULL,victim,TO_VICT);
         act("$n quickly slashes across $N's stomach causing $M to yelp in pain.",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 4 ) / 6;
               spgain = ( spgain * 4 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You suddenly scream '#yDie!#n' as you thrust your blade through $N's chest!",ch,NULL,victim,TO_CHAR);
         act("$n suddenly screams '#yDie!#n' as $e thrusts $s blade through your chest!",ch,NULL,victim,TO_VICT);
         act("$n suddenly screams '#yDie!#n' as $e thrusts $s blade through $N's chest!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 5 ) / 6;
               spgain = ( spgain * 5 ) / 6;
            }
      }

   if ( !miss )
      {
         act("As $N staggers from an invisible force, you mutter '#yHard Blade...#n' as you glare evilly.",ch,NULL,victim,TO_CHAR);
         act("As you staggers from an invisible force, $n mutters '#yHard Blade...#n' as $e glares evilly.",ch,NULL,victim,TO_VICT);
         act("As $N staggers from an invisible force, $n mutters '#yHard Blade...#n' as $e glares evilly.",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_dragoon_harpoon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 2 )
         {
            send_to_char( "But you are not using a #cSpear#n.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to thrust your spear into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_LCOMBO1] < 100 )
      ch->pcdata->powers[DRAGOON_LCOMBO1] += 20;

   dam = 375000;
   level = ( ch->pcdata->powers[DRAGOON_LCOMBO1] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.0;
              spgain = 34;
              break;

      case 2: multi = 1.1;
              spgain = 38;
              break;

      case 3: multi = 1.2;
              spgain = 42;
              break;

      case 4: multi = 1.3;
              spgain = 45;
              break;

      default:
      case 5: multi = 1.5;
              spgain = 50;
              break;
   }

   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_LCOMBO1] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you rush towards $N, $E barely evades your driving spear by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n rushes towards you, you barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n rushes towards $N, $E barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You rush towards $N as you thrust your spear into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n rushes towards you as $e thrusts $s spear into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n rushes towards $N as $e thrusts $s spear into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 2;
                    spgain /= 2;
                 }
           }

   if ( !miss )
      {
         act("You suddenly yell '#yAaaaai... Harpoon!#n' as you DrIvE your spear into $N again!",ch,NULL,victim,TO_CHAR);
         act("$n suddenly yells '#yAaaaai... Harpoon!#n' as $e DrIvEs $s spear into you again!",ch,NULL,victim,TO_VICT);
         act("$n suddenly yells '#yAaaaai... Harpoon!#n' as $e DrIvEs $s spear into $N again!",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_spinning_cane( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 2 )
         {
            send_to_char( "But you are not using a #cSpear#n.\n\r", ch );
            return;
         }

   if ( ch->move < 50000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to thrust your spear into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_LCOMBO2] < 100 )
      ch->pcdata->powers[DRAGOON_LCOMBO2] += 20;

   dam = 300000;
   level = ( ch->pcdata->powers[DRAGOON_LCOMBO2] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.0;
              break;

      case 2: multi = 1.25;
              break;

      case 3: multi = 1.5;
              break;

      case 4: multi = 1.75;
              break;

      default:
      case 5: multi = 2.0;
              break;
   }

   spgain = 35;
   ch->move -= 50000;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_LCOMBO2] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you rush towards $N, $E barely evades your driving spear by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n rushes towards you, you barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n rushes towards $N, $E barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You rush towards $N as you thrust your spear into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n rushes towards you as $e thrusts $s spear into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n rushes towards $N as $e thrusts $s spear into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 3;
                    spgain /= 3;
                 }
           }

   if ( !miss )
      {
         act("You rapidly twirl your spear across $N chest as you scream loudly!",ch,NULL,victim,TO_CHAR);
         act("$n rapidly twirls $s spear across $N chest as $e screams loudly!",ch,NULL,victim,TO_VICT);
         act("$n rapidly twirls $s spear across $N chest as $e screams loudly!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 3;
               spgain = ( spgain * 2 ) / 3;
            }
      }

   if ( !miss )
      {
         act("You SaVaGeLy SlAsH $N down $S chest as you yell '#ySpinning Cane!#n'",ch,NULL,victim,TO_CHAR);
         act("$n SaVaGeLy SlAsHeS you down your chest as $e yells '#ySpinning Cane!#n'",ch,NULL,victim,TO_VICT);
         act("$n SaVaGeLy SlAsHeS $N down $S chest as $e yells '#ySpinning Cane!#n'",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_rod_typhoon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 2 )
         {
            send_to_char( "But you are not using a #cSpear#n.\n\r", ch );
            return;
         }

   if ( ch->move < 50000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to thrust your spear into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_LCOMBO3] < 100 )
      ch->pcdata->powers[DRAGOON_LCOMBO3] += 20;

   dam = 300000;
   level = ( ch->pcdata->powers[DRAGOON_LCOMBO3] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.5;
              spgain = 30;
              break;

      case 2: multi = 1.62;
              spgain = 45;
              break;

      case 3: multi = 1.74;
              spgain = 60;
              break;

      case 4: multi = 1.86;
              spgain = 75;
              break;

      default:
      case 5: multi = 2.02;
              spgain = 100;
              break;
   }

   ch->move -= 50000;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_LCOMBO3] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you rush towards $N, $E barely evades your driving spear by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n rushes towards you, you barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n rushes towards $N, $E barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You rush towards $N as you thrust your spear into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n rushes towards you as $e thrusts $s spear into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n rushes towards $N as $e thrusts $s spear into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 5;
                    spgain /= 5;
                 }
           }

   if ( !miss )
      {
         act("You twist your body around as you whack $N hard with your spear.",ch,NULL,victim,TO_CHAR);
         act("$n twists $s body around as $e whacks you hard with $s spear.",ch,NULL,victim,TO_VICT);
         act("$n twists $s body around as $e whacks $N hard with $s spear.",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 5;
               spgain = ( spgain * 2 ) / 5;
            }
      }

   if ( !miss )
      {
         act("You yell '#yDah!#n' as you twist your body whacking $N hard again!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yDah!#n' as $e twists $s body whacking you hard again!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yDah!#n' as $e twists $s body whacking $N hard again!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 3 ) / 5;
               spgain = ( spgain * 3 ) / 5;
            }
      }

   if ( !miss )
      {
         act("You yell '#yDah!#n' as you TwIsT your body WhAcKiNg $N HARD again!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yDah!#n' as $e TwIsTs $s body WhAcKiNg you HARD again!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yDah!#n' as $e TwIsTs $s body WhAcKiNg $N HARD again!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 4 ) / 5;
               spgain = ( spgain * 4 ) / 5;
            }
      }

   ch->attack_type = ATTACK_PHYSICAL;

   if ( !miss )
      {
         act("You SaVaGeLy SlAsH $N down $S chest as you yell '#yRod Typhoon!#n'",ch,NULL,victim,TO_CHAR);
         act("$n SaVaGeLy SlAsHeS you down your chest as $e yells '#yRod Typhoon!#n'",ch,NULL,victim,TO_VICT);
         act("$n SaVaGeLy SlAsHeS $N down $S chest as $e yells '#yRod Typhoon!#n'",ch,NULL,victim,TO_NOTVICT);

         hurt_person( ch, victim, dam * multi, TRUE );

         act("The momentum of the blow sends you onto your knees as you struggle to keep balance.",ch,NULL,NULL,TO_CHAR);
         act("The momentum of the blow sends $n onto $s knees as $e struggles to keep balance.",ch,NULL,NULL,TO_ROOM);
      }
      else hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_gust_wind( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 2 )
         {
            send_to_char( "But you are not using a #cSpear#n.\n\r", ch );
            return;
         }

   if ( ch->move < 100000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to thrust your spear into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_LCOMBO4] < 100 )
      ch->pcdata->powers[DRAGOON_LCOMBO4] += 20;

   dam = 185000;
   level = ( ch->pcdata->powers[DRAGOON_LCOMBO4] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 2.0;
              break;

      case 2: multi = 2.4;
              break;

      case 3: multi = 2.8;
              break;

      case 4: multi = 3.2;
              break;

      default:
      case 5: multi = 3.5;
              break;
   }

   spgain = 35;
   ch->move -= 100000;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_LCOMBO4] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you rush towards $N, $E barely evades your driving spear by leaping aside.",ch,NULL,victim,TO_CHAR);
         act("As $n rushes towards you, you barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_VICT);
         act("As $n rushes towards $N, $E barely evades $s driving spear by leaping aside.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You rush towards $N as you thrust your spear into $S chest viciously.",ch,NULL,victim,TO_CHAR);
              act("$n rushes towards you as $e thrusts $s spear into your chest viciously.",ch,NULL,victim,TO_VICT);
              act("$n rushes towards $N as $e thrusts $s spear into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 7;
                    spgain /= 7;
                 }
           }

   if ( !miss )
      {
         act("You yell '#yDah#n' as you slash your spear upwards $N's body!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yDah#n' as $e slashes $s spear upwards your body!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yDah#n' as $e slashes $s spear upwards $N's body!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 7;
               spgain = ( spgain * 2 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You drive your spear onto the ground with sheer intensity on your face!",ch,NULL,NULL,TO_CHAR);
         act("$n drives $s spear onto the ground with sheer intensity on $s face!",ch,NULL,NULL,TO_ROOM);
         act("You then hold onto the spear as you spin around it kicking $N viciously!",ch,NULL,victim,TO_CHAR);
         act("$n then holds onto the spear as $e spins around it kicking you viciously!",ch,NULL,victim,TO_VICT);
         act("$n then holds onto the spear as $e spins around it kicking $N viciously!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 3 ) / 7;
               spgain = ( spgain * 3 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You yell '#yDah#n' as you slash your spear upwards $N's body!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yDah#n' as $e slashes $s spear upwards your body!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yDah#n' as $e slashes $s spear upwards $N's body!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 4 ) / 7;
               spgain = ( spgain * 4 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You SaVaGeLy SlAsH $N down $S chest as the wind RoArS loudly!",ch,NULL,victim,TO_CHAR);
         act("$n SaVaGeLy SlAsHeS you down your chest as the wind RoArS loudly!",ch,NULL,victim,TO_VICT);
         act("$n SaVaGeLy SlAsHeS $N down $S chest as the wind RoArS loudly!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 5 ) / 7;
               spgain = ( spgain * 5 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You yell '#yDah#n' as you slash your spear upwards $N's body!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yDah#n' as $e slashes $s spear upwards your body!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yDah#n' as $e slashes $s spear upwards $N's body!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 6 ) / 7;
               spgain = ( spgain * 6 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You leap high above $N as you suddenly dive SpEaR fIrSt into $N!!!",ch,NULL,victim,TO_CHAR);
         act("$n leaps high above you as $e suddenly dives SpEaR fIrSt into you!!!",ch,NULL,victim,TO_VICT);
         act("$n leaps high above $N as $e suddenly dives SpEaR fIrSt into $N!!!",ch,NULL,victim,TO_NOTVICT);
         act("You then flip off $N gently while cackling '#yGust of Wind Dance!#n'",ch,NULL,victim,TO_CHAR);
         act("$n then flips off you gently while cackling '#yGust of Wind Dance!#n'",ch,NULL,victim,TO_VICT);
         act("$n then flips off $N gently while cackling '#yGust of Wind Dance!#n'",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_demon_dance( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword or rapier.\n\r", ch );
            return;
         }

   if ( ch->move < 250000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your rapier into?\n\r", ch );
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

   ch->move -= 250000;
   ch->wait = 12;

   act("You dash towards $N as you thrust your blade into $S chest viciously.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you as $e thrusts $s blade into your chest viciously.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N as $e thrusts $s blade into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);
   act("You then drive your blade downwards $N's chest as you tilt yourself to the ground.",ch,NULL,victim,TO_CHAR);
   act("$n then drives $s blade downwards your chest as $e tilts $mself to the ground.",ch,NULL,victim,TO_VICT);
   act("$n then drives $s blade downwards $N's chest as $e tilts $mself to the ground.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly scream '#yDie!#n' as you thrust your blade through $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly screams '#yDie!#n' as $e thrusts $s blade through your chest!",ch,NULL,victim,TO_VICT);
   act("$n suddenly screams '#yDie!#n' as $e thrusts $s blade through $N's chest!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin around as you slash $N viciously across $S stomach.",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins around as $e slashes you viciously across your stomach.",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins around as $e slashes $N viciously across $S stomach.",ch,NULL,victim,TO_NOTVICT);
   act("You quickly leap into the air, twirling your blade around as you drive it up $N!",ch,NULL,victim,TO_CHAR);
   act("$n quickly leaps into the air, twirling $s blade around as $e drives it up you!",ch,NULL,victim,TO_VICT);
   act("$n quickly leaps into the air, twirling $s blade around as $e drives it up $N!",ch,NULL,victim,TO_NOTVICT);
   act("You then drive your blade downwards $N's chest as you tilt yourself to the ground.",ch,NULL,victim,TO_CHAR);
   act("$n then drives $s blade downwards your chest as $e tilts $mself to the ground.",ch,NULL,victim,TO_VICT);
   act("$n then drives $s blade downwards $N's chest as $e tilts $mself to the ground.",ch,NULL,victim,TO_NOTVICT);
   act("You leap towards $N screaming loudly as you SlAsH $S chest SaVaGeLy!!!",ch,NULL,victim,TO_CHAR);
   act("$n leaps towards you screaming loudly as $e SlAsHeS your chest SaVaGeLy!!!",ch,NULL,victim,TO_VICT);
   act("$n leaps towards $N screaming loudly as $e SlAsHeS $S chest SaVaGeLy!!!",ch,NULL,victim,TO_NOTVICT);

   act("You quickly leap away from $N, slashing at $S chest viciously as you mutter '#yDemon's Dance...#n'.",ch,NULL,victim,TO_CHAR);
   act("$n quickly leaps away from you, slashing at your chest viciously as $e mutters '#yDemon's Dance...#n'.",ch,NULL,victim,TO_VICT);
   act("$n quickly leaps away from $N, slashing at $S chest viciously as $e mutters '#yDemon's Dance...#n'.",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      ch->pcdata->powers[DRAGOON_SPIRIT] = UMIN( ch->pcdata->powers[DRAGOON_SPIRIT] + 100, URANGE( 100, ch->pkill * 50, 1000 ) );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 700000, TRUE );

   act("You land back on your knees gently, staring coldly around you.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s knees gently, staring coldly around $m.",ch,NULL,NULL,TO_ROOM);
}

void do_flower_storm( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 2 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a spear.\n\r", ch );
            return;
         }

   if ( ch->move < 250000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to thrust your spear into?\n\r", ch );
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

   ch->move -= 250000;
   ch->wait = 12;

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      ch->pcdata->powers[DRAGOON_SPIRIT] = UMIN( ch->pcdata->powers[DRAGOON_SPIRIT] + 200, URANGE( 100, ch->pkill * 50, 1000 ) );

   act("You rush towards $N as you thrust your spear into $S chest viciously.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you as $e thrusts $s spear into your chest viciously.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N as $e thrusts $s spear into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yDah#n' as you slash your spear upwards $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yDah#n' as $e slashes $s spear upwards your body!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yDah#n' as $e slashes $s spear upwards $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("Admist the air, you suddenly drive your spear viciously in the ground!",ch,NULL,NULL,TO_CHAR);
   act("Admist the air, $n suddenly drives $s spear viciously in the ground!",ch,NULL,NULL,TO_ROOM);
   act("As you fall back towards the earth, you push yourself off your spear kicking $N viciously!",ch,NULL,victim,TO_CHAR);
   act("As $n falls back towards the earth, $e pushes $mself off $s spear kicking you viciously!",ch,NULL,victim,TO_VICT);
   act("As $n falls back towards the earth, $e pushes $mself off $s spear kicking $N viciously!",ch,NULL,victim,TO_NOTVICT);
   act("You rush towards $N as you thrust your spear into $S chest viciously.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you as $e thrusts $s spear into your chest viciously.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N as $e thrusts $s spear into $S chest viciously.",ch,NULL,victim,TO_NOTVICT);
   act("You quickly slash your spear across $N chest as you scream loudly!",ch,NULL,victim,TO_CHAR);
   act("$n quickly slashes $s spear across $N chest as $e screams loudly!",ch,NULL,victim,TO_VICT);
   act("$n quickly slashes $s spear across $N chest as $e screams loudly!",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly, you turn your back towards $N, DrIvInG the butt of your spear into $N!",ch,NULL,victim,TO_CHAR);
   act("Suddenly, $n turns $s back towards you, DrIvInG the butt of $s spear into you!",ch,NULL,victim,TO_VICT);
   act("Suddenly, $n turns $s back towards $N, DrIvInG the butt of $s spear into $N!",ch,NULL,victim,TO_NOTVICT);
   act("You SlAsH your spear upwards $N's body viciously as you lean back from the force!",ch,NULL,victim,TO_CHAR);
   act("$n SlAsHeS $s spear upwards your body viciously as $e leans back from the force!",ch,NULL,victim,TO_VICT);
   act("$n SlAsHeS $s spear upwards $N's body viciously as $e leans back from the force!",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yFlower Storm...#n' as you leap into the air StRiKiNg $N SaVaGeLy!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yFlower Storm...#n' as $e leaps into the air StRiKiNg you SaVaGeLy!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yFlower Storm...#n' as $e leaps into the air StRiKiNg $N SaVaGeLy!",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 600000, TRUE );
}

void do_blazing_dynamo( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   if ( ch->move < 250000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your sword into?\n\r", ch );
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

   ch->move -= 250000;
   ch->wait = 12;

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      ch->pcdata->powers[DRAGOON_SPIRIT] = UMIN( ch->pcdata->powers[DRAGOON_SPIRIT] + 150, URANGE( 100, ch->pkill * 50, 1000 ) );

   act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
   act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
   act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yToi!#n' as you slash upwards $N's body viciously.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yToi!#n' as $e slashes upwards your body viciously.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yToi!#n' as $e slashes upwards $N's body viciously.",ch,NULL,victim,TO_NOTVICT);
   act("You spin around rapidly slashing $N across the chest!",ch,NULL,victim,TO_CHAR);
   act("$n spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
   act("$n spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly slash downwards $N's body SaVaGeLy from the other side!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly slashes downwards your body SaVaGeLy from the other side!",ch,NULL,victim,TO_VICT);
   act("$n suddenly slashes downwards $N's body SaVaGeLy from the other side!",ch,NULL,victim,TO_NOTVICT);
   act("You spin around rapidly slashing $N across the chest!",ch,NULL,victim,TO_CHAR);
   act("$n spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
   act("$n spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yToi!#n' as you slash upwards $N's body viciously.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yToi!#n' as $e slashes upwards your body viciously.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yToi!#n' as $e slashes upwards $N's body viciously.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly dash towards $N giving $M a deadly drop kick to the stomach!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly dashes towards you giving you a deadly drop kick to the stomach!",ch,NULL,victim,TO_VICT);
   act("$n suddenly dashes towards $N giving $M a deadly drop kick to the stomach!",ch,NULL,victim,TO_NOTVICT);
   act("You then do a quick double flip high above $N's head.",ch,NULL,victim,TO_CHAR);
   act("$n then does a quick double flip high above your head.",ch,NULL,victim,TO_VICT);
   act("$n then does a quick double flip high above $N's head.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yBlazing Dynamo!#n' as you DrIvE your sword down $N's head!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yBlazing Dynamo!#n' as $e DrIvEs $s sword down your head!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yBlazing Dynamo!#n' as $e DrIvEs $s sword down $N's head!",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 650000, TRUE );
}

void do_double_slash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 3 )
         {
            send_to_char( "But you are not using a #RSword#n.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your sword into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_SCOMBO1] < 100 )
      ch->pcdata->powers[DRAGOON_SCOMBO1] += 20;

   dam = 260000;
   level = ( ch->pcdata->powers[DRAGOON_SCOMBO1] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.5;
              break;

      case 2: multi = 1.57;
              break;

      case 3: multi = 1.65;
              break;

      case 4: multi = 1.8;
              break;

      default:
      case 5: multi = 2.02;
              break;
   }

   spgain = 35;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_SCOMBO1] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you run towards $N, $E rolls out of the way of your deadly slash.",ch,NULL,victim,TO_CHAR);
         act("As $n runs towards you, you rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_VICT);
         act("As $n runs towards $N, $E rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
              act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
              act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 2;
                    spgain /= 2;
                 }
           }

   if ( !miss )
      {
         act("You suddenly yell '#yDouble Slash!#n' as you SlAsH across $N's chest!",ch,NULL,victim,TO_CHAR);
         act("$n suddenly yells '#yDouble Slash!#n' as $e SlAsH across your chest!",ch,NULL,victim,TO_VICT);
         act("$n suddenly yells '#yDouble Slash!#n' as $e SlAsH across $N's chest!",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_burning_rush( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 3 )
         {
            send_to_char( "But you are not using a #RSword#n.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your sword into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_SCOMBO3] < 100 )
      ch->pcdata->powers[DRAGOON_SCOMBO3] += 20;

   dam = 350000;
   level = ( ch->pcdata->powers[DRAGOON_SCOMBO3] / 20 ) + 1;

   switch( level )
   {
      case 1: spgain = 30;
              break;

      case 2: spgain = 45;
              break;

      case 3: spgain = 60;
              break;

      case 4: spgain = 75;
              break;

      default:
      case 5: spgain = 102;
              break;
   }

   multi = 1.5;
   ch->wait = 12;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_SCOMBO3] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you run towards $N, $E rolls out of the way of your deadly slash.",ch,NULL,victim,TO_CHAR);
         act("As $n runs towards you, you rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_VICT);
         act("As $n runs towards $N, $E rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
              act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
              act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 3;
                    spgain /= 3;
                 }
           }

   if ( !miss )
      {
         act("You then KiCk $N SaVaGeLy in the stomach causing $M to bend over in pain!",ch,NULL,victim,TO_CHAR);
         act("$n spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
         act("$n spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 3;
               spgain = ( spgain * 2 ) / 3;
            }
      }

   if ( !miss )
      {
         act("You mutter '#yBurning Rush...#n' as you swipe your sword upwards $N's face!",ch,NULL,victim,TO_CHAR);
         act("$n mutters '#yBurning Rush...#n' as $e swipes $s sword upwards your face!",ch,NULL,victim,TO_VICT);
         act("$n mutters '#yBurning Rush...#n' as $e swipes $s sword upwards $N's face!",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_crush_dance( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 3 )
         {
            send_to_char( "But you are not using a #RSword#n.\n\r", ch );
            return;
         }

   if ( ch->move < 50000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your sword into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_SCOMBO4] < 100 )
      ch->pcdata->powers[DRAGOON_SCOMBO4] += 20;

   dam = 240000;
   level = ( ch->pcdata->powers[DRAGOON_SCOMBO4] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 1.5;
              spgain = 50;
              break;

      case 2: multi = 1.72;
              spgain = 65;
              break;

      case 3: multi = 1.95;
              spgain = 75;
              break;

      case 4: multi = 2.17;
              spgain = 85;
              break;

      default:
      case 5: multi = 2.5;
              spgain = 100;
              break;
   }

   ch->wait = 12;
   ch->move -= 50000;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_SCOMBO4] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you run towards $N, $E rolls out of the way of your deadly slash.",ch,NULL,victim,TO_CHAR);
         act("As $n runs towards you, you rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_VICT);
         act("As $n runs towards $N, $E rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
              act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
              act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 5;
                    spgain /= 5;
                 }
           }

   if ( !miss )
      {
         act("You spin around rapidly slashing $N across the chest!",ch,NULL,victim,TO_CHAR);
         act("$n spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
         act("$n spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 5;
               spgain = ( spgain * 2 ) / 5;
            }
      }

   if ( !miss )
      {
         act("You suddenly do a reverse spin slashing $N across the chest again!",ch,NULL,victim,TO_CHAR);
         act("$n suddenly does a reverse spin slashing you across the chest again!",ch,NULL,victim,TO_VICT);
         act("$n suddenly does a reverse spin slashing $N across the chest again!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 3 ) / 5;
               spgain = ( spgain * 3 ) / 5;
            }
      }

   if ( !miss )
      {
         act("You stagger forward from the blow as you thrust your blade into $N's stomach!",ch,NULL,victim,TO_CHAR);
         act("$n staggers forward from the blow as $e thrusts $s blade into your stomach!",ch,NULL,victim,TO_VICT);
         act("$n staggers forward from the blow as $e thrusts $s blade into $N's stomach!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 4 ) / 5;
               spgain = ( spgain * 4 ) / 5;
            }
      }

   if ( !miss )
      {
         act("You yell '#yCrush Dance!#n' as you SmItE $N with a deadly UpPeR sLaSh!!!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yCrush Dance!#n' as $e SmItEs you with a deadly UpPeR sLaSh!!!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yCrush Dance!#n' as $e SmItEs $N with a deadly UpPeR sLaSh!!!",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_madness_hero( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 3 )
         {
            send_to_char( "But you are not using a #RSword#n.\n\r", ch );
            return;
         }

   if ( ch->move < 100000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your sword into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_SCOMBO5] < 100 )
      ch->pcdata->powers[DRAGOON_SCOMBO5] += 20;

   dam = 450000;
   level = ( ch->pcdata->powers[DRAGOON_SCOMBO5] / 20 ) + 1;

   switch( level )
   {
      case 1: spgain = 60;
              break;

      case 2: spgain = 90;
              break;

      case 3: spgain = 120;
              break;

      case 4: spgain = 150;
              break;

      default:
      case 5: spgain = 204;
              break;
   }

   multi = 1.0;
   ch->wait = 12;
   ch->move -= 100000;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_SCOMBO5] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you run towards $N, $E rolls out of the way of your deadly slash.",ch,NULL,victim,TO_CHAR);
         act("As $n runs towards you, you rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_VICT);
         act("As $n runs towards $N, $E rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
              act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
              act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 6;
                    spgain /= 6;
                 }
           }

   if ( !miss )
      {
         act("You wildly spin around rapidly slashing $N across the chest!",ch,NULL,victim,TO_CHAR);
         act("$n wildly spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
         act("$n wildly spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 6;
               spgain = ( spgain * 3 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You cackle with glee as you slash downwards $N's body viciously.",ch,NULL,victim,TO_CHAR);
         act("$n cackles with glee $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
         act("$n cackles with glee as $e slashes downwards $N's body viciously.",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 3 ) / 6;
               spgain = ( spgain * 3 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You wildly spin around rapidly slashing $N across the chest!",ch,NULL,victim,TO_CHAR);
         act("$n wildly spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
         act("$n wildly spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 4 ) / 6;
               spgain = ( spgain * 4 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You cackle with glee as you slash upwards $N's body viciously.",ch,NULL,victim,TO_CHAR);
         act("$n cackles with glee $e slashes upwards your body viciously.",ch,NULL,victim,TO_VICT);
         act("$n cackles with glee as $e slashes upwards $N's body viciously.",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 5 ) / 6;
               spgain = ( spgain * 5 ) / 6;
            }
      }

   if ( !miss )
      {
         act("You yell '#yMadness... HERO!!!#n' as you DrIvE your BlAde down $N's body!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yMadness... HERO!!!#n' as $e DrIvEs $s BlAde down your body!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yMadness... HERO!!!#n' as $e DrIvEs $s BlAde down $N's body!",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_moon_strike( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   int dam, level, spgain, per;
   float multi;
   bool miss = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         stc("You can't do normal additions in your dragoon form.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->value[3] != 3 )
         {
            send_to_char( "But you are not using a #RSword#n.\n\r", ch );
            return;
         }

   if ( ch->move < 100000 )
      { 
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash your sword into?\n\r", ch );
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

   if ( ch->pcdata->powers[DRAGOON_SCOMBO6] < 100 )
      ch->pcdata->powers[DRAGOON_SCOMBO6] += 20;

   dam = 190000;
   level = ( ch->pcdata->powers[DRAGOON_SCOMBO6] / 20 ) + 1;

   switch( level )
   {
      case 1: multi = 2.0;
              break;

      case 2: multi = 2.4;
              break;

      case 3: multi = 2.8;
              break;

      case 4: multi = 3.2;
              break;

      default:
      case 5: multi = 3.5;
              break;
   }

   spgain = 20;
   ch->wait = 12;
   ch->move -= 100000;

   per = UMAX( 40, ch->pcdata->powers[DRAGOON_SCOMBO6] );

   if ( number_percent() > per )
      miss = TRUE;

   if ( miss )
      {
         act("As you run towards $N, $E rolls out of the way of your deadly slash.",ch,NULL,victim,TO_CHAR);
         act("As $n runs towards you, you rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_VICT);
         act("As $n runs towards $N, $E rolls out of the way of $s deadly slash.",ch,NULL,victim,TO_NOTVICT);
         dam = 0;
      }
      else {      
              act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
              act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
              act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);

              if ( number_percent() > per )
                 {
                    miss = TRUE;
                    dam /= 7;
                    spgain /= 7;
                 }
           }

   if ( !miss )
      {
         act("You spin around rapidly slashing $N across the chest!",ch,NULL,victim,TO_CHAR);
         act("$n spins around rapidly slashing you across the chest!",ch,NULL,victim,TO_VICT);
         act("$n spins around rapidly slashing $N across the chest!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 2 ) / 7;
               spgain = ( spgain * 2 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You run towards $N as you slash downwards $S's body viciously.",ch,NULL,victim,TO_CHAR);
         act("$n runs towards $N as $e slashes downwards your body viciously.",ch,NULL,victim,TO_VICT);
         act("$n runs towards $N as $e slashes downwards $S's body viciously.",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 3 ) / 7;
               spgain = ( spgain * 3 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You suddenly slash downwards $N's body SaVaGeLy from the other side!",ch,NULL,victim,TO_CHAR);
         act("$n suddenly slashes downwards your body SaVaGeLy from the other side!",ch,NULL,victim,TO_VICT);
         act("$n suddenly slashes downwards $N's body SaVaGeLy from the other side!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 4 ) / 7;
               spgain = ( spgain * 4 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You ViCiOuSLy thrust your blade into $N's stomach!",ch,NULL,victim,TO_CHAR);
         act("$n ViCiOuSLy thrusts $s blade into your stomach!",ch,NULL,victim,TO_VICT);
         act("$n ViCiOuSLy thrusts $s blade into $N's stomach!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 5 ) / 7;
               spgain = ( spgain * 5 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You ViCiOuSLy thrust your blade into $N's stomach!",ch,NULL,victim,TO_CHAR);
         act("$n ViCiOuSLy thrusts $s blade into your stomach!",ch,NULL,victim,TO_VICT);
         act("$n ViCiOuSLy thrusts $s blade into $N's stomach!",ch,NULL,victim,TO_NOTVICT);

         if ( number_percent() > per )
            {
               miss = TRUE;
               dam = ( dam * 6 ) / 7;
               spgain = ( spgain * 6 ) / 7;
            }
      }

   if ( !miss )
      {
         act("You yell '#yMoon Strike!#n' as you twist your sword deeper into $N's body!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yMoon Strike!#n' as $e twists $s sword deeper into your body!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yMoon Strike!#n' as $e twists $s sword deeper into $N's body!",ch,NULL,victim,TO_NOTVICT);
         act("At the last second, you SaVaGeLy kick $N away from you.",ch,NULL,victim,TO_CHAR);
         act("At the last second, $n SaVaGeLy kicks you away from $m.",ch,NULL,victim,TO_VICT);
         act("At the last second, $n SaVaGeLy kicks $N away from $m.",ch,NULL,victim,TO_NOTVICT);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, dam * multi, TRUE );

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         ch->pcdata->powers[DRAGOON_SPIRIT] += spgain;

         if ( ch->pcdata->powers[DRAGOON_SPIRIT] > ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
            ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

         if ( ch->pcdata->powers[DRAGOON_LEVEL] < 10 )
            {
               ch->pcdata->powers[DRAGOON_SPS] += spgain;
               can_dragoon_level( ch );
            }
      }
}

void do_jade_shift( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   act( "#gYou spread your arms apart as a manifest of #wblinding#g energy hovers before you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#g$n spreads $s arms apart as a manifest of #wblinding#g energy hovers before $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#gAs the ball of energy explodes around you, you close your eyes as a soft rumbling appears.#n", ch, NULL, NULL, TO_CHAR );
   act( "#gAs the ball of energy explodes around $n, $e closes $s eyes as a soft rumbling appears.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou scream '#yAwwwwwwwh#g' as your body merge #wviolently #gwith the #wlight #garound you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#g$n screams '#yAwwwwwwwh#g' as $s body merge #wviolently #gwith the #wlight #garound $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#gA mass of colourful leaves suddenly appear from nowhere as they rapidly encircle you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#gA mass of colourful leaves suddenly appear from nowhere as they rapidly encircle someone.#n", ch, NULL, NULL, TO_ROOM );
   act( "#gYou feel MaSsIvE wings forming from your back as a ferocious #wwhirlwind #gengulfs you!#n", ch, NULL, NULL, TO_CHAR );
   act( "#gMaSsIvE wings grows from someone's back as a ferocious #wwhirlwind #gengulfs $m!#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #gJade #PDragoon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act( "#gAs the #wlight #garound you dims, you notice your weapon is now made off an ancient bark!#n", ch, NULL, NULL, TO_CHAR );
   act( "#gAs the #wlight #garound $n #gdims, you notice $s weapon is now made off an ancient bark!#n", ch, NULL, NULL, TO_ROOM );
   act( "#gGripping your weapon, you yell '#yDai!#g' as you lean forward while spreading your wings widely!#n", ch, NULL, NULL, TO_CHAR );
   act( "#gGripping $s weapon, $n #gyells '#yDai!#g' as $e leans forward while spreading $s wings widely!#n", ch, NULL, NULL, TO_ROOM );
   act( "#gThe ferocious winds encircling you suddenly go calm as the leaves fall to the ground.#n", ch, NULL, NULL, TO_CHAR );
   act( "#gThe ferocious winds encircling $n #gsuddenly go calm as the leaves fall to the ground.#n", ch, NULL, NULL, TO_ROOM );
}

void do_fire_shift( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   act( "#RYour body trembles with rage as you stagger forward onto your knees.#n", ch, NULL, NULL, TO_CHAR );
   act( "#R$n's body trembles with rage as $e staggers forward onto $s knees.#n", ch, NULL, NULL, TO_ROOM );
   act( "#RA sadistic grin appears on your face as your eyes #yblaze #Rwith ferocious intensity!#n", ch, NULL, NULL, TO_CHAR );
   act( "#RA sadistic grin appears on $n's face as $s eyes #yblaze #Rwith ferocious intensity!#n", ch, NULL, NULL, TO_ROOM );
   act( "#RYou snarl '#yArrrrrrghh..#R' as the ground shakes violently beneaths your feet.#n", ch, NULL, NULL, TO_CHAR );
   act( "#R$n snarls '#yArrrrrrghh..#R' as the ground shakes violently beneaths $s feet.#n", ch, NULL, NULL, TO_ROOM );
   act( "#RA small ball of #yenergy #Rsuddenly rise in front of you #yexploding #Rin a MaSs Of FlAmEs!!#n", ch, NULL, NULL, TO_CHAR );
   act( "#RA small ball of #yenergy #Rsuddenly rise in front of $n #yexploding #Rin a MaSs Of FlAmEs!!#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #RFire #PDragoon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act( "#RYour eyes #yenlit #Rin flames as a pair of fiery wings sprouts from behind your back.#n", ch, NULL, NULL, TO_CHAR );
   act( "#R$n#R's eyes #yenlit #Rin flames as a pair of fiery wings sprouts from behind $s back.#n", ch, NULL, NULL, TO_ROOM );
   act( "#RYou yell '#yDai!#R' as you tilt your now flaming weapon into the #ylight #Remitting from you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#R$n #Ryells '#yDai!#R' as $e tilts $s now flaming weapon into the #ylight #Remitting from $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#RYou crouch forward extending your free hand out as the flames scatter across the room!#n", ch, NULL, NULL, TO_CHAR );
   act( "#R$n #Rcrouches forward extending $s free hand out as the flames scatter across the room!#n", ch, NULL, NULL, TO_ROOM );
}

void do_dark_shift( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   act( "#eYou focus on your #lmental #epowers as your power gently levitates in the air...#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n focuses on $s #lmental #epowers as $s power gently levitates in the air...#n", ch, NULL, NULL, TO_ROOM );
   act( "#eYou stare straight up into the sky as arcs of #ldemonic #eelectricity flies from your body.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n stares straight up into the sky as arcs of #ldemonic #eelectricity flies from $s body.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eAs the arcs intensify, you suddenly find yourself in a bleak ball of chaotic energy.#n", ch, NULL, NULL, TO_CHAR );
   act( "#eAs the arcs intensify, $n suddenly finds $mself in a bleak ball of chaotic energy.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eThe ball starts convulses, growing smaller and smaller as it #lcrumples #eyour body.#n", ch, NULL, NULL, TO_CHAR );
   act( "#eThe ball starts convulses, growing smaller and smaller as it #lcrumples #e$n's body.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eA mass of bright light fills the room as the mass of demonic energy fades away...#n", ch, NULL, NULL, TO_CHAR );
   act( "#eA mass of bright light fills the room as the mass of demonic energy fades away...#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #eDark #PDragoon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act( "#eSuddenly, an explosion fills the air as you find yourself back where you came from!#n", ch, NULL, NULL, TO_CHAR );
   act( "#eSuddenly, an explosion fills the air as $n finds $mself back in the room!#n", ch, NULL, NULL, TO_ROOM );
   act( "#eYou then fold your beautiful, black wings behind your back, #lsmirking #eto yourself.#n", ch, NULL, NULL, TO_CHAR );
   act( "#e$n #ethen folds $s beautiful, black wings behind $s back, #lsmirking #eto $mself.#n", ch, NULL, NULL, TO_ROOM );
   act( "#eA blast of #ldazzling stardust #eradiates from you as you coldly point your weapon ahead!#n", ch, NULL, NULL, TO_CHAR );
   act( "#eA blast of #ldazzling stardust #eradiates from $n as $e coldly points $s weapon ahead!#n", ch, NULL, NULL, TO_ROOM );
}

void do_holy_shift( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   act( "#wYou smile softly as you open your palm right in front of you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n smiles softly as $e opens $s palm right in front of you.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wA mass of #ydivine #wenergy manifests above your hand as you look above you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#wA mass of #ydivine #wenergy manifests above $n's hand as $e looks above $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou mutter '#yDragi um Si xoi...#w' as beautiful waves of light swerves by you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n mutters '#yDragi um Si xoi...#w' as beautiful waves of light swerves by $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wVoices from the heavens pour down on you as your body starts #ymerging #wwith the light.#n", ch, NULL, NULL, TO_CHAR );
   act( "#wVoices from the heavens pour down on $n as $s body starts #ymerging #wwith the light.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou scream in rejoice as arcs of #ylight #wblast from your body forming wings!#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n screams in rejoice as arcs of #ylight #wblast from $s body forming wings!#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #wHoly #PDragoon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act( "#wYou stretch your body from side to side as your hands are engulfed with #yblinding #wenergy.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n #wstretches your body from side to side as $s hands are engulfed with #yblinding #wenergy.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wA mass of #yfeathers #wfall from your angel wings as you bow your head in prayer.#n", ch, NULL, NULL, TO_CHAR );
   act( "#wA mass of #yfeathers #wfall from $n#w's angel wings as $e bows $s head in prayer.#n", ch, NULL, NULL, TO_ROOM );
}

void do_divine_shift( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   act( "#wYou subtively pull out the #CDivine #oStone #was you raise it into the air.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n #wsubtively pulls out the #CDivine #oStone #was $e raises it into the air.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wA beautiful #P#zsparkle#n #wof #ylight #wradiates down your body just as it pulses.#n", ch, NULL, NULL, TO_CHAR );
   act( "#wA beautiful #P#zsparkle#n #wof #ylight #wradiates down $n#w's body just as it pulses.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou smile as the #ylight #Rengulfs #wyour body while the ground starts trembling.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n smiles as the #ylight #Rengulfs #w$s body while the ground starts trembling.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou then push your right hand forward as the #yenergy #Ptransforms #wit into a #ocannon.#n", ch, NULL, NULL, TO_CHAR );
   act( "#wAdmist the #ylight#w, you see #C$n's #whand transforming it into a #oHUGE cannon#w.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou then concentrate on the #CDragon #yBuster #was the room explodes into #Rflames#w.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n then concentrates on the #CDragon #yBuster #was the room explodes into #Rflames#w.#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #CDivine #PDragoon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act( "#wAdmist the #ylight#w, you flap your #ccrystal clear #wwings as your body becomes normal!#n", ch, NULL, NULL, TO_CHAR );
   act( "#wAdmist the #ylight#w, $n #wflaps $s #ccrystal clear #wwings as $s body becomes normal!#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou then #C#zSMASH#n a heavy foot onto the ground as an #Peye scanner #wshields your right eye!!!#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n #wthen #C#zSMASHES#n a heavy foot onto the ground as an #Peye scanner #wshields $s right eye!!!#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou scream '#yDai!#w' as 3 more pairs of powerful wings ErUpT from your back!#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n screams '#yDai!#w' as 3 more pairs of powerful wings ErUpT from $s back!#n", ch, NULL, NULL, TO_ROOM );
}

void do_water_shift( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   act( "#LYou draw a #cmagical #Lsymbol with your hand in the air as your eyes sparkles.#n", ch, NULL, NULL, TO_CHAR );
   act( "#L$n draws a #cmagical #Lsymbol with $s hand in the air as $s eyes sparkles.#n", ch, NULL, NULL, TO_ROOM );
   act( "#LSuddenly, you curl up into a ball as a clear #cbubble #Lforms around you.#n", ch, NULL, NULL, TO_CHAR );
   act( "#LSuddenly, $n curls up into a ball as a clear #cbubble #Lforms around $m.#n", ch, NULL, NULL, TO_ROOM );
   act( "#LThe bubble #cfloats #Lgently into the air as you start muttering a spell...#n", ch, NULL, NULL, TO_CHAR );
   act( "#LThe bubble #cfloats #Lgently into the air as $n starts muttering a spell...#n", ch, NULL, NULL, TO_ROOM );
   act( "#LYou feel the bubble pulsing with #cenergy #Laround you as it fills with light!#n", ch, NULL, NULL, TO_CHAR );
   act( "#L$n feels the bubble pulsing with #cenergy #Laround $m as it fills with light!#n", ch, NULL, NULL, TO_ROOM );
   act( "#LThe bubble PoPs loudly as a #cMaSs #Lof water showers the vicinity!#n", ch, NULL, NULL, TO_CHAR );
   act( "#LThe bubble PoPs loudly as a #cMaSs #Lof water showers the vicinity!#n", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "%s, the #LSea #PDragoon#n", ch->name );

   free_string( ch->morph );
   ch->morph = str_dup( buf );

   if ( !IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act( "#LAdmist the water, you flap your #ccrystal clear #Lwings as your body becomes normal!#n", ch, NULL, NULL, TO_CHAR );
   act( "#LAdmist the water, $n flaps $s #ccrystal clear #Lwings as $s body becomes normal!#n", ch, NULL, NULL, TO_ROOM );
   act( "#LYou give a small #cwink #Kas you open your palm watching your weapon manifests in it!#n", ch, NULL, NULL, TO_CHAR );
   act( "#L$n gives a small #cwink #Las $e opens $s palm watching $s weapon manifests in it!#n", ch, NULL, NULL, TO_ROOM );
   act( "#LYou then fly rapidly across the room leaving a trail of stardust behind you!#n", ch, NULL, NULL, TO_CHAR );
   act( "#LYou then flies rapidly across the room leaving a trail of stardust behind $m!#n", ch, NULL, NULL, TO_ROOM );
}

void do_dragoon_transform( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         if ( ch->position == POS_FIGHTING && ch->pcdata->powers[DRAGOON_SPIRIT] > 0 )
            {
               stc("But you are still in a fight dragon knight.\n\r",ch);
               return;
            }

         if ( ch->position != POS_FIGHTING )
            ch->wait = 12;

         act( "A palish light emits from your body as you suddenly feel weak.", ch, NULL, NULL, TO_CHAR );
         act( "A palish light emits from $n's body as $e suddenly feels weak.", ch, NULL, NULL, TO_ROOM );
         act( "The wings on your body vanishes from sight as it merges within you.", ch, NULL, NULL, TO_CHAR );
         act( "The wings on $n's body vanishes from sight as it merges within $m.", ch, NULL, NULL, TO_ROOM );

         free_string( ch->morph );
         ch->morph = str_dup( "" );

         REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
         REMOVE_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER );

         if ( IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
            REMOVE_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );

         act( "As the light fades away, you find yourself back in your human form.", ch, NULL, NULL, TO_CHAR );
         act( "As the light fades away, $n finds $mself back in $s human form.", ch, NULL, NULL, TO_ROOM );

         if ( IS_SET( ch->more, MORE_WALL ) )
            {
               REMOVE_BIT( ch->more, MORE_WALL );

               if ( IS_SET( ch->in_room->room_flags, ROOM_SHIFT ) )
                  {
                      act( "#wThe #Lco#glo#Lur#gful #evortex #yshimmers #was it suddenly #Rimplodes #win front of you.#n", ch, NULL, NULL, TO_CHAR );
                      act( "#wThe #Lco#glo#Lur#gful #evortex #yshimmers #was it suddenly #Rimplodes #win front of you.#n", ch, NULL, NULL, TO_ROOM );
                      REMOVE_BIT( ch->in_room->room_flags, ROOM_SHIFT );
                  }
            }

         if ( IS_CLASS( ch, CLASS_DRAGON ) && ch->position >= POS_STUNNED ) 
            {
               do_dragonshift( ch, "" );
               ch->wait = 0;
            }

         return;
      }

   if ( ch->pcdata->powers[DRAGOON_SPIRIT] < 100 )
      {
         stc("You don't have enough spiritual energy within you to transform into a dragoon.\n\r",ch);
         return;
      }

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DIVINE ) )
      do_divine_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
              do_jade_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
              do_fire_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
              do_dark_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
              do_water_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
              do_holy_shift( ch );
              else {
                      stc( "Huh?\n\r", ch );
                      return;
                   }

   clear_affects( ch );

   SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER );

   if ( !IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
      SET_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );
}

void do_shift( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int count = 0;
   int drg_class[3];
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      drg_class[0] = 1;
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
              drg_class[0] = 2;
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
              drg_class[0] = 3;
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
              drg_class[0] = 4;
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
              drg_class[0] = 5;
              else {
                      stc("You haven't chosen a Dragon Spirit as of yet.\n\r",ch);
                      return;
                   }

   if ( ch->pcdata->powers[DRAGOON_SPIRIT] < ch->pcdata->powers[DRAGOON_LEVEL] * 100 )
      {
         stc("Your Dragoon Spirit needs to be at max to use this special ability.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
      {
         send_to_char( "The room is already engulfed by a colourful vortex.\n\r", ch );
         return;
      }

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group(ch,vch) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DRAGON_KNIGHT ) || vch->pcdata->powers[DRAGOON_SPIRIT] < vch->pcdata->powers[DRAGOON_LEVEL] * 100 )
         continue;

      if ( IS_SET( vch->affected_by, AFF_POLYMORPH ) )
         continue;

      if ( count < 2 )
         {
            if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
               drg_class[count + 1] = 1;
               else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
                       drg_class[count + 1] = 2;
               else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
                       drg_class[count + 1] = 3;
               else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
                       drg_class[count + 1] = 4;
               else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
                       drg_class[count + 1] = 5;
                       else continue;

            if ( count == 0 && drg_class[0] == drg_class[1] )
               continue;

            if ( count == 1 && ( drg_class[0] == drg_class[2] || drg_class[1] == drg_class[2] ) )
               continue;
         }

      count++;
   }

   /* This is to add the user as being one of the dragoons needed for this special command */
   count++;

   if ( count < 3 )
      {
         stc( "You don't have enough experienced followers to fully exploit their true forms.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DIVINE ) )
      do_divine_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
              do_jade_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
              do_fire_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
              do_dark_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
              do_water_shift( ch );
      else if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
              do_holy_shift( ch );
              else {
                      stc( "Huh?\n\r", ch );
                      return;
                   }

   clear_affects( ch );
   hit_gain( ch, 3000000 );

   if ( !IS_SET( ch->more, MORE_WALL ) )
      SET_BIT( ch->more, MORE_WALL );

   if ( !IS_SET( ch->pcdata->powers[1], DPOWER_LEVITATION ) )
      SET_BIT( ch->pcdata->powers[1], DPOWER_LEVITATION );

   SET_BIT(ch->in_room->room_flags, ROOM_SHIFT);

   act( "#wThe room #gshimmers #was it is filled with #yblinding #wlight!#n", ch, NULL, NULL, TO_CHAR );
   act( "#wThe room #gshimmers #was it is filled with #yblinding #wlight!#n", ch, NULL, NULL, TO_ROOM );
   act( "#wAs the light #efades #wback to normal, you find yourself within a #Lcolourful #evortex#w!#n", ch, NULL, NULL, TO_CHAR );
   act( "#wAs the light #efades #wback to normal, you find yourself within a #Lcolourful #evortex#w!#n", ch, NULL, NULL, TO_ROOM );
   act( "#gBeautiful leaves #Lencircles #wyour body as #ysparks #wof #Rflames #Cdazzle #wthe floor below!#n", ch, NULL, NULL, TO_CHAR );
   act( "#gBeautiful leaves #Lencircles #wyour body as #ysparks #wof #Rflames #Cdazzle #wthe floor below!#n", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group(ch,vch) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DRAGON_KNIGHT ) || vch->pcdata->powers[DRAGOON_SPIRIT] < vch->pcdata->powers[DRAGOON_LEVEL] * 100 )
         continue;

      clear_affects( vch );
      hit_gain( vch, 3000000 );

      if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
         continue;

      if ( IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DIVINE ) )
         sprintf( buf, "%s, the #CDivine #PDragoon#n", vch->name );
         else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
                 sprintf( buf, "%s, the #RFire #PDragoon#n", vch->name );
         else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
                 sprintf( buf, "%s, the #gJade #PDragoon#n", vch->name );
         else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
                 sprintf( buf, "%s, the #eDark #PDragoon#n", vch->name );
         else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
                 sprintf( buf, "%s, the #wHoly #PDragoon#n", vch->name );
         else if ( IS_SET( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
                 sprintf( buf, "%s, the #LSea #PDragoon#n", vch->name );
                 else continue;

      free_string( vch->morph );
      vch->morph = str_dup( buf );

      SET_BIT( vch->affected_by, AFF_POLYMORPH );
      SET_BIT( vch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER );

      if ( !IS_SET( vch->pcdata->powers[1], DPOWER_LEVITATION ) )
         SET_BIT( vch->pcdata->powers[1], DPOWER_LEVITATION );
   }
}

void do_swirling_dance( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
         {
            send_to_char( "You need to be using a weapon to use this ability.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release a barrage onto?\n\r", ch );
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

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You open your hand revealing shards of light as you fly towards $N!", ch, NULL, victim, TO_CHAR );
   act( "$n opens $s hand revealing shards of light as $e flies towards you!", ch, NULL, victim, TO_VICT );
   act( "$n opens $s hand revealing shards of light as $e flies towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream '#yYai!#n' as you lash the shard across $N's chest!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yYai!#n' as $e lashes the shard across your chest!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yYai!#n' as $e lashes the shard across $N's chest!", ch, NULL, victim, TO_NOTVICT );
   act( "You then slash across $N's chest again as your armor starts pulsing.", ch, NULL, victim, TO_CHAR );
   act( "$n then slashes across your chest again as $s armor starts pulsing.", ch, NULL, victim, TO_VICT );
   act( "$n then slashes across $N's chest again as $s armor starts pulsing.", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, you dance around $N as your armor slices through $S body!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n dances around you as $s armor slices through your body!", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n dances around $N as $s armor slices through $S body!", ch, NULL, victim, TO_NOTVICT );
   act( "You giggle softly as you twirl over $N's head as you slash it quickly!", ch, NULL, victim, TO_CHAR );
   act( "$n giggles softly as $e twirls over your head as $e slashes it quickly!", ch, NULL, victim, TO_VICT );
   act( "$n giggles softly as $e twirls over $N's head as $e slashes it quickly!", ch, NULL, victim, TO_NOTVICT );

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_hand_cannon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
      {
         stc( "Huh?\n\r", ch );
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
	   send_to_char( "Whom do you want to release a barrage onto?\n\r", ch );
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

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clenching your blazing fists sadly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clenching $s blazing fists sadly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly fly above $N as you release a BlAsT of energy towards $N!", ch, NULL, victim, TO_CHAR );
   act( "$n suddenly flies above you as $e releases a BlAsT of energy towards you!", ch, NULL, victim, TO_VICT );
   act( "$n suddenly flies above $N as $e releases a BlAsT of energy towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "As $N staggers back from the blast, you fire another one at $M!", ch, NULL, victim, TO_CHAR );
   act( "As you stagger back from the blast, $e fires another one at you!", ch, NULL, victim, TO_VICT );
   act( "As $N staggers back from the blast, $e fires another one at $M!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream loudly as you quickly let loose a barrage of beams towards $N!", ch, NULL, victim, TO_CHAR );
   act( "$n screams loudly as $e quickly lets loose a barrage of beams towards you!", ch, NULL, victim, TO_VICT );
   act( "$n screams loudly as $e quickly lets loose a barrage of beams towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You smile happily as multiple explosions blast all around $M!!!", ch, NULL, victim, TO_CHAR );
   act( "$n smiles happily as multiple explosions blast all around you!!!", ch, NULL, victim, TO_VICT );
   act( "$n smiles happily as multiple explosions blast all around $M!!!", ch, NULL, victim, TO_NOTVICT );

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BEAM;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_dark_shrine( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
         {
            send_to_char( "You need to be using a weapon to use this ability.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release a barrage onto?\n\r", ch );
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

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Placing your weapon in front of you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Placing $s weapon in front of $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You fly rapidly towards $N as you thrust your weapon through $S chest!", ch, NULL, victim, TO_CHAR );
   act( "$n flies rapidly towards you as $e thrusts $s weapon through your chest!", ch, NULL, victim, TO_VICT );
   act( "$n flies rapidly towards $N as $e thrusts $s weapon through $S chest!", ch, NULL, victim, TO_NOTVICT );
   act( "You skillfully twist your hand around as you slash down $N's stomach!", ch, NULL, victim, TO_CHAR );
   act( "$n skillfully twists $s hand around as $e slashes down your stomach!", ch, NULL, victim, TO_VICT );
   act( "$n skillfully twists $s hand around as $e slashes down $N's stomach!", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, you thrust your weapon into $N's chest as you sneer '#yDie!#n'", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n thrusts $s weapon into your chest as $e sneers '#yDie!#n'", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n thrusts $s weapon into $N's chest as $e sneers '#yDie!#n'", ch, NULL, victim, TO_NOTVICT );
   act( "You then slash upwards $N's body as you fly above $M...", ch, NULL, victim, TO_CHAR );
   act( "$n then slashes upwards your body as $e flies above you...", ch, NULL, victim, TO_VICT );
   act( "$n then slashes upwards $N's body as $e flies above $M...", ch, NULL, victim, TO_NOTVICT );
   act( "You sneer '#yHUI!#n' as you DrIvE your weapon coldly down $S body as it glitters.", ch, NULL, victim, TO_CHAR );
   act( "$n sneers '#yHUI!#n' as $e DrIvEs $s weapon coldly down your body as it glitters.", ch, NULL, victim, TO_VICT );
   act( "$n sneers '#yHUI!#n' as $e DrIvEs $s weapon coldly down $S body as it glitters.", ch, NULL, victim, TO_NOTVICT );

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_sonic_bomb( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
         {
            send_to_char( "You need to be using a weapon to use this ability.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release a barrage onto?\n\r", ch );
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

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon with both hands, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon with both hands, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You fly towards $N screaming '#yDai!#n' as you thrust your weapon into $M.", ch, NULL, victim, TO_CHAR );
   act( "$n flies towards you screaming '#yDai!#n' as $e thrusts $s weapon into you.", ch, NULL, victim, TO_VICT );
   act( "$n flies towards $N screaming '#yDai!#n' as $e thrusts $s weapon into $M.", ch, NULL, victim, TO_NOTVICT );
   act( "You then slash viciously down $N's body screaming '#yDai!#n' again.", ch, NULL, victim, TO_CHAR );
   act( "$n then slashes viciously down your body screaming '#yDai!#n' again.", ch, NULL, victim, TO_VICT );
   act( "$n then slashes viciously down $N's body screaming '#yDai!#n' again.", ch, NULL, victim, TO_NOTVICT );
   act( "You lash back your weapon StRiKiNg $N's face as you make $M stagger.", ch, NULL, victim, TO_CHAR );
   act( "$n lashes back $s weapon StRiKiNg your face as $e makes you stagger.", ch, NULL, victim, TO_VICT );
   act( "$n lashes back $s weapon StRiKiNg $N's face as $e makes $M stagger.", ch, NULL, victim, TO_NOTVICT );
   act( "Ferocious energy engulfs your weapon as you DrIvE it into $N again.", ch, NULL, victim, TO_CHAR );
   act( "Ferocious energy engulfs $n's weapon as $e DrIvEs it into you again.", ch, NULL, victim, TO_VICT );
   act( "Ferocious energy engulfs $n's weapon as $e DrIvEs it into $N again.", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly you leap into the air staring blanking into the heavens.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly $n leaps into the air staring blanking into the heavens.", ch, NULL, NULL, TO_ROOM );
   act( "You then scream '#yEEEEEYAI!#n' as $e DrIvEs $s weapon into $N's head!", ch, NULL, victim, TO_CHAR );
   act( "$n then screams '#yEEEEEYAI!#n' as $e DrIvEs $s weapon into your head!", ch, NULL, victim, TO_VICT );
   act( "$n then screams '#yEEEEEYAI!#n' as $e DrIvEs $s weapon into $N's head!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream in agony as the ground ExPlOdEs into your flesh!", victim, NULL, NULL, TO_CHAR );
   act( "$n screams in agony as the ground ExPlOdEs into $s flesh!", victim, NULL, NULL, TO_ROOM );

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_flaming_twirl( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || obj->item_type != ITEM_WEAPON )
         {
            send_to_char( "You need to be using a weapon to use this ability.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to release a barrage onto?\n\r", ch );
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

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon tightly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon tightly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You scream '#yTai!#n' as you fly by $N slashing at $S side violently.", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yTai!#n' as $e flies by you slashing at your side violently.", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yTai!#n' as $e flies by $N slashing at $S side violently.", ch, NULL, victim, TO_NOTVICT );
   act( "You then DrIvE your weapon into $N's back screaming '#yYai!#n'", ch, NULL, victim, TO_CHAR );
   act( "$n then DrIvEs $s weapon into your back screaming '#yYai!#n'", ch, NULL, victim, TO_VICT );
   act( "$n then DrIvEs $s weapon into $N's back screaming '#yYai!#n'", ch, NULL, victim, TO_NOTVICT );
   act( "Flying pass $N again, you suddenly slice ViCiOuSlY through $S chest.", ch, NULL, victim, TO_CHAR );
   act( "Flying pass you again, $n suddenly slices ViCiOuSlY through your chest.", ch, NULL, victim, TO_VICT );
   act( "Flying pass $N again, $n suddenly slices ViCiOuSlY through $S chest.", ch, NULL, victim, TO_NOTVICT );
   act( "You scream '#yDrill!#n' as you twirl yourself into $N's body!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yDrill!#n' as $e twirls $mself into your body!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yDrill!#n' as $e twirls $mself into $N's body!", ch, NULL, victim, TO_NOTVICT );
   act( "You then fly back way above $N muttering '#yarrrrrrrgh...#n'", ch, NULL, victim, TO_CHAR );
   act( "$n then flies back way above you muttering '#yarrrrrrrgh...#n'", ch, NULL, victim, TO_VICT );
   act( "$n then flies back way above $N muttering '#yarrrrrrrgh...#n'", ch, NULL, victim, TO_NOTVICT );
   act( "You cackle with glee as you LaSh your weapon across $N's face!", ch, NULL, victim, TO_CHAR );
   act( "$n cackles with glee as $e LaSheS $s weapon across your face!", ch, NULL, victim, TO_VICT );
   act( "$n cackles with glee as $e LaSheS $s weapon across $N's face!", ch, NULL, victim, TO_NOTVICT );

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_whirlpool( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_WHIRLWIND ) )
      {
         send_to_char( "The room is already being swarmed by a deadly whirlpool.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 2 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 2 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon with both hands, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon with both hands, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You twirl your weapon viciously as you sneer '#yWhirlPool!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n twirls $s weapon viciously as $e sneers '#yWhirlPool!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The sky goes dark as you realize that a MaSsIvE blast of wind is encircling the room.", ch, NULL, NULL, TO_CHAR );
   act( "The sky goes dark as you realize that a MaSsIvE blast of wind is encircling the room.", ch, NULL, NULL, TO_ROOM );
   act( "As the whirlwind gets closer and closer to you, everything starts feeling differently.", ch, NULL, NULL, TO_CHAR );
   act( "As the whirlwind gets closer and closer to you, everything starts feeling differently.", ch, NULL, NULL, TO_ROOM );
   act( "The room is suddenly engulfed with an anti-damage field!", ch, NULL, NULL, TO_CHAR );
   act( "The room is suddenly engulfed with an anti-damage field!", ch, NULL, NULL, TO_ROOM );
   SET_BIT( ch->in_room->room_flags, ROOM_WHIRLWIND );
}

void do_wing_blaster( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 850000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 10 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 10 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon with both hands, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon with both hands, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You twirl your weapon viciously as you sneer '#yWing Blaster!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n twirls $s weapon viciously as $e sneers '#yWing Blaster!#n'", ch, NULL, NULL, TO_ROOM );
   act( "You see a sparkle in the distance as ferocious wind surrounds the vicinity.", ch, NULL, NULL, TO_CHAR );
   act( "You see a sparkle in the distance as ferocious wind surrounds the vicinity.", ch, NULL, NULL, TO_ROOM );
   act( "A bird made out of energy shrieks loudly as it flies straight towards your foe!", ch, NULL, NULL, TO_CHAR );
   act( "A bird made out of energy shrieks loudly as it flies straight towards you!", ch, NULL, NULL, TO_ROOM );
   act( "A trail of shockwaves flashes behind the bird as the entire room trembles from the force!", ch, NULL, NULL, TO_CHAR );
   act( "A trail of shockwaves flashes behind the bird as the entire room trembles from the force!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "You scream in agony as the blast of air trailing the bird rips your body apart!", vch, NULL, NULL, TO_CHAR );
      act( "$n screams in agony as the blast of air trailing the bird rips $s body apart!", vch, NULL, NULL, TO_ROOM );
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, dam, TRUE );
   }
}

void do_rose_storm( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 15 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 5 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 5 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon with both hands, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon with both hands, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You twirl your weapon viciously as you sneer '#yRose Storm!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n twirls $s weapon viciously as $e sneers '#yRose Storm!#n'", ch, NULL, NULL, TO_ROOM );
   act( "You point your weapon in front of you as it is engulfed with ferocious winds.", ch, NULL, NULL, TO_CHAR );
   act( "$n points $s weapon in front of $m as it is engulfed with ferocious winds.", ch, NULL, NULL, TO_ROOM );
   act( "The winds become even stronger as a small rose petal flutters away in it.", ch, NULL, NULL, TO_CHAR );
   act( "The winds become even stronger as a small rose petal flutters away in it.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, the entire vicinity is fill with petals as they fly all over the place.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, the entire vicinity is fill with petals as they fly all over the place.", ch, NULL, NULL, TO_ROOM );

   if ( ( sn = skill_lookup( "rose storm" ) ) < 0 )
      return;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_ITEMAFF( vch, ITEMA_DSTAFF ) && !is_same_group(ch,vch) )
         continue;

      if ( is_affected( vch, sn ) )
         continue;

      give_align( ch, 10 );

      spell_rose_storm( sn, 300, vch, vch );
   }
}

void do_gaspless( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you blast with ferocious energy?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon with both hands, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon with both hands, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You twirl your weapon viciously as you sneer '#yGaspless!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n twirls $s weapon viciously as $e sneers '#yGaspless!#n'", ch, NULL, NULL, TO_ROOM );
   act( "Ferocious wind encircles your body as you twirl your weapon even faster.", ch, NULL, NULL, TO_CHAR );
   act( "Ferocious wind encircles $n's body as $e twirls $s weapon even faster.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly scream as you smash your weapon into the ground!", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly screams as $e smashes $s weapon into the ground!", ch, NULL, NULL, TO_ROOM );
   act( "You create a MaSsIvE sHoCwAvE that ZoOmS sTrAiGht tOwArDs $N!", ch, NULL, victim, TO_CHAR );
   act( "$n creates a MaSsIvE sHoCwAvE that ZoOmS sTrAiGht tOwArDs you!", ch, NULL, victim, TO_VICT );
   act( "$n creates a MaSsIvE sHoCwAvE that ZoOmS sTrAiGht tOwArDs $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You stagger back blindly from the ShOcKwAvE as you try to gain a steady grip.", victim, NULL, NULL, TO_CHAR );
   act( "$n stagger back blindly from the ShOcKwAvE as $e tries to gain a steady grip.", victim, NULL, NULL, TO_ROOM );
   act( "You hold your weapon tightly, staring at the ground as gentle wind engulfs $N!", ch, NULL, victim, TO_CHAR );
   act( "$n holds $s weapon tightly, staring at the ground as gentle wind engulfs you!", ch, NULL, victim, TO_VICT );
   act( "$n holds $s weapon tightly, staring at the ground as gentle wind engulfs $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream '#yGone!#n' as the wind RiPs $N's body apart!!!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yGone!#n' as the wind RiPs your body apart!!!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yGone!#n' as the wind RiPs $N's body apart!!!", ch, NULL, victim, TO_NOTVICT );

   hurt_person( ch, victim, 2000000, TRUE );

   if ( victim != NULL && victim->position > POS_STUNNED && number_percent() > 70 - devil_luck( ch, victim, 5 ) ) 
      {
         stop_fighting( victim, TRUE );
         victim->position = POS_STUNNED;
      }
}

void do_jade_dragon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon with both hands, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon with both hands, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_CHAR );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, a MaSsIvE tornado forms into existence as a ShImMeRiNg dragon steps through.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, a MaSsIvE tornado forms into existence as a ShImMeRiNg dragon steps through.", ch, NULL, NULL, TO_ROOM );
   act( "The dragon flaps it wings loudly as it stares blankly around the vicinity.", ch, NULL, NULL, TO_CHAR );
   act( "The dragon flaps it wings loudly as it stares blankly around the vicinity.", ch, NULL, NULL, TO_ROOM );
   act( "It then lowers a part of its armour like skin revealing multiple cannons!", ch, NULL, NULL, TO_CHAR );
   act( "It then lowers a part of its armour like skin revealing multiple cannons!", ch, NULL, NULL, TO_ROOM );
   act( "A loud humming fills the air as the Jade Dragon charges up all of its cannons...", ch, NULL, NULL, TO_CHAR );
   act( "A loud humming fills the air as the Jade Dragon charges up all of its cannons...", ch, NULL, NULL, TO_ROOM );
   act( "At the last second, it releases a MASSIVE BlAsT of EnErGy across the room!", ch, NULL, NULL, TO_CHAR );
   act( "At the last second, it releases a MASSIVE BlAsT of EnErGy straight towards you!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "You scream in pain as the blast of energy rips your body apart!", vch, NULL, NULL, TO_CHAR );
      act( "$n screams in pain as the blast of energy rips $s body apart!", vch, NULL, NULL, TO_ROOM );
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BEAM;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 3500000, TRUE );
   }
}

void do_flame_shot( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];
   int dam = 850000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 10 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you blast with ferocious energy?\n\r", ch );
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

   ch->mana -= ( ch->max_mana / 10 );
   ch->wait = 12;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = AERIAL;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon tightly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon tightly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly throw your weapon above you as your eyes blazes with rage.", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly throws $s weapon above $m as $s eyes blazes with rage.", ch, NULL, NULL, TO_ROOM );
   act( "Staring angrily at $N, you tilt your hands together saying '#yArrrrrrgh...#n'", ch, NULL, victim, TO_CHAR );
   act( "Staring angrily at you, $n tilts $s hands together saying '#yArrrrrrgh...#n'", ch, NULL, victim, TO_VICT );
   act( "Staring angrily at $N, $n tilts $s hands together saying '#yArrrrrrgh...#n'", ch, NULL, victim, TO_NOTVICT );
   act( "A mass of fiery energy manifests between your trembling hands as you close your eyes.", ch, NULL, NULL, TO_CHAR );
   act( "A mass of fiery energy manifests between $n's trembling hands as $e closes $s eyes.", ch, NULL, NULL, TO_ROOM );
   act( "With a small comet hovering there, you draw back your fists muttering '#yFlame...#n'", ch, NULL, NULL, TO_CHAR );
   act( "With a small comet hovering there, $n draws back $s fists muttering '#yFlame...#n'", ch, NULL, NULL, TO_ROOM );
   act( "You PuNcH the comet viciously screaming '#ySHOT!#n' as it spirals towards $N!", ch, NULL, victim, TO_CHAR );
   act( "$n PuNcHeS the comet viciously screaming '#ySHOT!#n' as it spirals towards you!", ch, NULL, victim, TO_VICT );
   act( "$n PuNcHeS the comet viciously screaming '#ySHOT!#n' as it spirals towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "Your echo screams '#ySHOT!#n' as the comet STRIKES $N causing a HuGe explosion!", ch, NULL, victim, TO_CHAR );
   act( "$n's echo screams '#ySHOT!#n' as the comet STRIKES you causing a HuGe explosion!", ch, NULL, victim, TO_VICT );
   act( "$n's echo screams '#ySHOT!#n' as the comet STRIKES $N causing a HuGe explosion!", ch, NULL, victim, TO_NOTVICT );
   act( "You catch back your weapon as you cruelly watch the flames eating $N's flesh.", ch, NULL, victim, TO_CHAR );
   act( "$n catches back $s weapon as $e cruelly watches the flames eating your flesh.", ch, NULL, victim, TO_VICT );
   act( "$n catches back $s weapon as $e cruelly watches the flames eating $N's flesh.", ch, NULL, victim, TO_NOTVICT );

   hurt_person( ch, victim, dam, TRUE );
}

void do_explosion( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 850000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 15 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 5 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 5 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon tightly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon tightly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You raise your weapon behind your back as you scream '#yExplosion!#n", ch, NULL, NULL, TO_CHAR );
   act( "$n raises $s weapon behind $s back as $e screams '#yExplosion!#n", ch, NULL, NULL, TO_ROOM );
   act( "Your entire body starts trembling as you snarl '#yArrrrrrrrgh...#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n's entire body starts trembling as $e snarls '#yArrrrrrrrgh...#n'", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly DrIvE your weapon deep into the ground with all of your MiGhT!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly DrIvEs $s weapon deep into the ground with all of $s MiGhT!!!", ch, NULL, NULL, TO_ROOM );
   act( "A small line of flames blazes across the room towards your target!", ch, NULL, NULL, TO_CHAR );
   act( "A small line of flames blazes across the room towards you!", ch, NULL, NULL, TO_ROOM );
   act( "The ground beneath your feet starts melting as molten lava seeps through it!", ch, NULL, NULL, TO_CHAR );
   act( "The ground beneath your feet starts melting as molten lava seeps through it!", ch, NULL, NULL, TO_ROOM );
   act( "A barrage of flames leap from the ground burning all as shards fly into the air!", ch, NULL, NULL, TO_CHAR );
   act( "A barrage of flames leap from the ground burning all as shards fly into the air!", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, the shards of rocks turn around and CrAsH StRaIgHt into the vicinity!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, the shards of rocks turn around and CrAsH StRaIgHt into the vicinity!", ch, NULL, NULL, TO_ROOM );
   act( "Multiple explosions BlAsT right around you as the entire room fills with LiGhT!!!", ch, NULL, NULL, TO_CHAR );
   act( "Multiple explosions BlAsT right around you as the entire room fills with LiGhT!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("You scream loudly as the DeAdLy FlAmEs eats your defenseless body.",vch,NULL,NULL,TO_CHAR);
      act("$n screams loudly as the DeAdLy FlAmEs eats $s defenseless body.",vch,NULL,NULL,TO_ROOM);

      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = LOW;
      hurt_person( ch, vch, dam, TRUE );
   }
}

void do_final_burst( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you blast with ferocious energy?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon tightly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon tightly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly curl up into a ball as you are engulfed by a ball of fire.", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly curls up into a ball as $e is engulfed by a ball of fire.", ch, NULL, NULL, TO_ROOM );
   act( "You scream '#yArrrrrrrgh!#n' as the ball of fire levitates with you into the sky.", ch, NULL, NULL, TO_CHAR );
   act( "$n screams '#yArrrrrrrgh!#n' as the ball of fire levitates with $m into the sky.", ch, NULL, NULL, TO_ROOM );
   act( "You suddenly spread your arms and legs apart viciously, bursting the bubble as you glare.", ch, NULL, NULL, TO_CHAR );
   act( "$n suddenly spreads $s arms and legs apart viciously, bursting the bubble as $e glares.", ch, NULL, NULL, TO_ROOM );
   act( "You then tilt forward, using your weapon as a lance as it bursts into flames.", ch, NULL, NULL, TO_CHAR );
   act( "$n then tilt forward, using $s weapon as a lance as it bursts into flames.", ch, NULL, NULL, TO_ROOM );
   act( "As the flames get more ferocious around you, you scream '#yFinal Burst!#n'", ch, NULL, NULL, TO_CHAR );
   act( "As the flames get more ferocious around $n, $e screams '#yFinal Burst!#n'", ch, NULL, NULL, TO_ROOM );
   act( "Your wings then burst into flames as your entire body starts trembling from the power.", ch, NULL, NULL, TO_CHAR );
   act( "$n's wings then burst into flames as $s entire body starts trembling from the power.", ch, NULL, NULL, TO_ROOM );
   act( "You then ZoOm straight towards $N in a RiVeR of FerOcIoUs FlAmEs!!!", ch, NULL, victim, TO_CHAR );
   act( "$n then ZoOmS straight towards you in a RiVeR of FerOcIoUs FlAmEs!!!", ch, NULL, victim, TO_VICT );
   act( "$n then ZoOmS straight towards $N in a RiVeR of FerOcIoUs FlAmEs!!!", ch, NULL, victim, TO_NOTVICT );
   act( "You cackle with glee as the deadly flames painfully eats through $N's flesh!", ch, NULL, victim, TO_CHAR );
   act( "$n cackles with glee as the deadly flames painfully eats through your flesh!", ch, NULL, victim, TO_VICT );
   act( "$n cackles with glee as the deadly flames painfully eats through $N's flesh!", ch, NULL, victim, TO_NOTVICT );

   hurt_person( ch, victim, 2000000, TRUE  );
}

void do_merton( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 1250000;
   bool hurt = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 2 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 2 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon tightly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon tightly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "Your eyes burst with demonic flames as you scream '#yMerton!#n'",ch,NULL,NULL,TO_CHAR);
   act( "$n's eyes burst with demonic flames as $e screams '#yMerton!#n'",ch,NULL,NULL,TO_ROOM);
   act( "$n's hands glows #rblazing red#n as $e leans forward and point one of them at you.",ch,NULL,NULL,TO_ROOM);
   act( "Your hands glows #rblazing red#n as you lean forward and point one of them at the room.",ch,NULL,NULL,TO_CHAR);
   act( "The entire area starts trembling violently as a low rumble is heard in the distance.",ch,NULL,NULL,TO_ROOM);
   act( "The entire area starts trembling violently as a low rumble is heard in the distance.",ch,NULL,NULL,TO_CHAR);
   act( "Suddenly the vicinity becomes a river of flames as they ANNIHLATES everything in their path.",ch,NULL,NULL,TO_ROOM);
   act( "Suddenly the vicinity becomes a river of flames as they ANNIHLATES everything in their path.",ch,NULL,NULL,TO_CHAR);
   act( "As the unholy flames becomes hotter, a dragon looking form appears inbetween the blazing fires.",ch,NULL,NULL,TO_ROOM);
   act( "As the unholy flames becomes hotter, a dragon looking form appears inbetween the blazing fires.",ch,NULL,NULL,TO_CHAR);
   act( "The Salmander looks straight up as demonic energy rises from the ground CAUSING A HUGE EXPLOSION!",ch,NULL,NULL,TO_ROOM);
   act( "The Salmander looks straight up as demonic energy rises from the ground CAUSING A HUGE EXPLOSION!",ch,NULL,NULL,TO_CHAR);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group( ch, vch ) || is_safe( ch, vch ) )
         continue;

      act("You scream loudly as the flames eats your body.",vch,NULL,NULL,TO_CHAR);
      act("$n screams loudly as the flames eats $s body.",vch,NULL,NULL,TO_ROOM);
      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = UPPERCUT;
      hurt_person( ch, vch, dam, TRUE );
   }
}

void do_redeyed_dragon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clutching your weapon tightly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clutching $s weapon tightly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_CHAR );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_ROOM );
   act( "The ground beneath your feet starts trembling as rocks fly through the air!", ch, NULL, NULL, TO_CHAR );
   act( "The ground beneath your feet starts trembling as rocks fly through the air!", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly a blast of MaGmA sprouts from the ground as it sizzles high above you.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly a blast of MaGmA sprouts from the ground as it sizzles high above you.", ch, NULL, NULL, TO_ROOM );
   act( "Admist the magma, you see a pair of burning eyes as the flames transform into a dragon!", ch, NULL, NULL, TO_CHAR );
   act( "Admist the magma, you see a pair of burning eyes as the flames transform into a dragon!", ch, NULL, NULL, TO_ROOM );
   act( "Staring coldly around itself, the dragon flaps its wings noisily!", ch, NULL, NULL, TO_CHAR );
   act( "Staring coldly around itself, the dragon flaps its wings noisily!", ch, NULL, NULL, TO_ROOM );
   act( "It then curls up like a cannon as it bursts into ferocious flames!", ch, NULL, NULL, TO_CHAR );
   act( "It then curls up like a cannon as it bursts into ferocious flames!", ch, NULL, NULL, TO_ROOM );
   act( "Letting out a loud shriek, it ZoOmS straight into your target CauSiNg A mAsSiVe EXPLOSION!", ch, NULL, NULL, TO_CHAR );
   act( "Letting out a loud shriek, it ZoOmS straight into you CauSiNg A mAsSiVe EXPLOSION!", ch, NULL, NULL, TO_ROOM );
   act( "As the deadly shards of light dances across the bleak room, it suddenly EXPLODES agAiN!!!", ch, NULL, NULL, TO_CHAR );
   act( "As the deadly shards of light dances across the bleak room, it suddenly EXPLODES agAiN!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("You scream loudly as the DeAdLy FlAmEs eats your defenseless body.",vch,NULL,NULL,TO_CHAR);
      act("$n screams loudly as the DeAdLy FlAmEs eats $s defenseless body.",vch,NULL,NULL,TO_ROOM);
      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 3500000, TRUE );
   }
}

void do_astral_drain( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg[MAX_STRING_LENGTH];
   int dam = 850000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 10 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whose life do you wish to drain?\n\r", ch );
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

   ch->mana -= ( ch->max_mana / 10 );
   ch->wait = 12;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = AERIAL;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Placing your weapon in front of you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Placing $s weapon in front of $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You sneer '#yAstral Drain...#n' as your weapon starts pulsing with demonic energy.", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#yAstral Drain...#n' as $s weapon starts pulsing with demonic energy.", ch, NULL, NULL, TO_ROOM );
   act( "You twirl swiftly into the air as you ViCiOuSlY throw your weapon at $N!", ch, NULL, victim, TO_CHAR );
   act( "$n twirl swiftly into the air as $e ViCiOuSlY throws $s weapon at you!", ch, NULL, victim, TO_VICT );
   act( "$n twirl swiftly into the air as $e ViCiOuSlY throws $s weapon at $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream in agony as the weapon thrusts itself deep into your guts!", victim, NULL, NULL, TO_CHAR );
   act( "$n screams in agony as the weapon thrusts itself deep into $s guts!", victim, NULL, NULL, TO_ROOM );
   act( "You scream '#yArrrrrrgh!#n' as your weapon violently sucks $N's lifeforce towards you!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yArrrrrrgh!#n' as $s weapon violently sucks your lifeforce towards $m!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yArrrrrrgh!#n' as $s weapon violently sucks $N's lifeforce towards $m!", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, a palish tornado manifests before you as the lifeforce swirls around it!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, a palish tornado manifests before $n as the lifeforce swirls around it!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_ITEMAFF( vch, ITEMA_DSTAFF ) )
         if ( !is_same_group(ch,vch) || !can_heal(ch,vch) )
            continue;

      act("You swoon with dark pleasure as your wounds start dissolving in front of you.",vch,NULL,NULL,TO_CHAR);
      act("$n swoons with dark pleasure as $s wounds start dissolving in front of $m.",vch,NULL,NULL,TO_ROOM);
      hit_gain( vch, 100000 );
   }

   act( "You then open your hand causing $N to stagger backwards in pain as your weapon returns!", ch, NULL, victim, TO_CHAR );
   act( "$n then opens $s hand causing you to stagger backwards in pain as $s weapon returns!", ch, NULL, victim, TO_VICT );
   act( "$n then opens $s hand causing $N to stagger backwards in pain as $s weapon returns!", ch, NULL, victim, TO_NOTVICT );

   hurt_person( ch, victim, dam, TRUE );

   act( "You swipe the air viciously as you stare coldly into the distance!", ch, NULL, NULL, TO_CHAR );
   act( "$n swipes the air viciously as $e stares coldly into the distance!", ch, NULL, NULL, TO_ROOM );
}

void do_death_dimension( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn;
   bool test = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 15 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 5 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 5 );
   ch->wait = 12;

   if ( ( sn = skill_lookup( "fear" ) ) < 0 )
      return;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Placing your weapon in front of you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Placing $s weapon in front of $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You calmly raise your weapon to your face as a it glitters with a spark of red light.", ch, NULL, NULL, TO_CHAR );
   act( "$n calmly raises $s weapon to $s face as a it glitters with a spark of red light.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, the light hovers into the air, transforming into a demonic black hole.", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, the light hovers into the air, transforming into a demonic black hole.", ch, NULL, NULL, TO_ROOM );
   act( "As the void gets bigger before your eyes, you sneer '#yDeath Dimension...#n'", ch, NULL, NULL, TO_CHAR );
   act( "As the void gets bigger before your eyes, $n sneers '#yDeath Dimension...#n'", ch, NULL, NULL, TO_ROOM );
   act( "The black hole transcede in front of you as it sucks in all of your foes!", ch, NULL, NULL, TO_CHAR );
   act( "The black hole transcede in front of $n as it sucks in all of $s foes!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      if ( IS_ITEMAFF( vch, ITEMA_DSTAFF ) || is_affected( vch, sn ) )
         continue;

      if ( IS_SET(vch->more2, MORE2_SHIELD) )
         {
            test = TRUE;
            REMOVE_BIT(vch->more2, MORE2_SHIELD);
         }

      if ( number_percent() > 40 - devil_luck( ch, vch, 10 ) )
         spell_fear( sn, 10, vch, vch );

      if ( test == TRUE )
         SET_BIT(vch->more2, MORE2_SHIELD);

      test = FALSE;

      act( "Your eyes fill with terror as you are sucked out of sight, clutching the air in vain!", vch, NULL, NULL, TO_CHAR );
      act( "$n's eyes fill with terror as $e is sucked out of sight, clutching the air in vain!", vch, NULL, NULL, TO_ROOM );
      
      ch->ele_attack = ATTACK_DARK;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 900000, TRUE );
   }

   act( "You sneer '#yGone!#n as you swipe the air viciously with your weapon!", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#yGone!#n as $e swipes the air viciously with $s weapon!", ch, NULL, NULL, TO_ROOM );
}

void do_demon_gate( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 3 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 3 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Placing your weapon in front of you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Placing $s weapon in front of $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You lean back spreading your legs apart as a small red drop falls on the ground.", ch, NULL, NULL, TO_CHAR );
   act( "$n leans back spreading $s legs apart as a small red drop falls on the ground.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, a massive beam of demonic energy shoots through your legs violently!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, a massive beam of demonic energy shoots through $n's legs violently!", ch, NULL, NULL, TO_ROOM );
   act( "As the light rips your body in half, you sneer '#yDemon's Gate...#n'", ch, NULL, NULL, TO_CHAR );
   act( "As the light rips $n's body in half, $e sneers '#yDemon's Gate...#n'", ch, NULL, NULL, TO_ROOM );
   act( "Both of your halves merge into a towering gate of blinding light.", ch, NULL, NULL, TO_CHAR );
   act( "Both of $n's halves merge into a towering gate of blinding light.", ch, NULL, NULL, TO_ROOM );
   act( "As the gates open, you can see another dimension with cruel demons waiting in glee.", ch, NULL, NULL, TO_CHAR );
   act( "As the gates open, you can see another dimension with cruel demons waiting in glee.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, a MaSsIvE breeZe strikes the room blasting everything straight towards the gate!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, a MaSsIvE breeZe strikes the room blasting everything straight towards the gate!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group( ch, vch ) || is_safe( ch, vch ) || !IS_NPC( vch ) )
         continue;

      if ( !IS_SET( vch->act, ACT_NINJA ) && vch->master == NULL )
         continue;

      if ( vch->pIndexData->vnum >= 550 && vch->pIndexData->vnum <= 570 )
         continue;      

      act( "Your eyes fill with terror as you are sucked straight into the gates of hell!", vch, NULL, NULL, TO_CHAR );
      act( "$n's eyes fill with terror as $e is sucked straight into the gates of hell!", vch, NULL, NULL, TO_ROOM );
      
      hit_gain( ch, 500000 );

      vch->hit = -1000;
      hurt_person( ch, vch, 10000, TRUE );
   }
}

void do_last_prayer( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];
   int sn;
   bool test = FALSE;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who to you want to curse?\n\r", ch );
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

   if ( ( sn = skill_lookup( "last prayer" ) ) < 0 )
      return;

   if ( IS_ITEMAFF( victim, ITEMA_DSTAFF ) )
      {
         stc( "Your attack is useless against the wielder of the staff.\n\r", ch );
         return;
      }

   if ( is_affected( victim, sn ) )
      {
         stc( "That person's soul is already damned.\n\r", ch );
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   if ( IS_SET(victim->more2, MORE2_SHIELD) )
      {
         test = TRUE;
         REMOVE_BIT(victim->more2, MORE2_SHIELD);
      }

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Placing your weapon in front of you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Placing $s weapon in front of $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "Your hand merge with your weapon to form a demonic lance as you smile softly.", ch, NULL, NULL, TO_CHAR );
   act( "$n's hand merge with $s weapon to form a demonic lance as $e smiles softly.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, your open your other hand blasting $N with blinding light!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n opens $s other hand blasting you with blinding light!", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n opens $s other hand blasting $N with blinding light!", ch, NULL, victim, TO_NOTVICT );
   act( "You then scream '#yArrrrrrrgh!#n' as $N staggers across the room in a dazed!", ch, NULL, victim, TO_CHAR );
   act( "$n then screams '#yArrrrrrrgh!#n' as you stagger across the room in a dazed!", ch, NULL, victim, TO_VICT );
   act( "$n then screams '#yArrrrrrrgh!#n' as $N staggers across the room in a dazed!", ch, NULL, victim, TO_NOTVICT );
   act( "You quickly DrIvE your lance into $N's body as you sneer '#yLast Prayer...#n'", ch, NULL, victim, TO_CHAR );
   act( "$n quickly DrIvEs $s lance into your body as $e sneers '#yLast Prayer...#n'", ch, NULL, victim, TO_VICT );
   act( "$n quickly DrIvEs $s lance into $N's body as $e sneers '#yLast Prayer...#n'", ch, NULL, victim, TO_NOTVICT );
   act( "Your eyes fill with terror as you see your soul being shattered into pieces.", victim, NULL, NULL, TO_CHAR );
   act( "$n's eyes fill with terror as $e sees $s soul being shattered into pieces.", victim, NULL, NULL, TO_ROOM );

   spell_last_prayer( sn, 10, ch, victim );

   if ( test == TRUE )
      SET_BIT(victim->more2, MORE2_SHIELD);

   if ( victim->outer_aura != -1 )
      do_aura( victim,"outer none" );

   if ( victim->inner_aura != -1 )
      do_aura( victim,"inner none" );

   act( "You sneer '#yGone!#n as you swipe the air viciously with your weapon!", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#yGone!#n as $e swipes the air viciously with $s weapon!", ch, NULL, NULL, TO_ROOM );

   ch->ele_attack = ATTACK_DARK;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 2000000, TRUE );
}

void do_dark_dragon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who to you want to fill with terror?\n\r", ch );
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

   if ( ch->ctimer[7] == 0 && !is_martial( victim ) )
      victim->wait = 12;

   if ( ch->ctimer[7] == 0 && !IS_ITEMAFF( victim, ITEMA_DSTAFF ) )
      {
         victim->mana = UMAX( 0, victim->mana / 2 );
         victim->move = UMAX( 0, victim->move / 2 );
      }

   if ( ch->ctimer[15] > 0 )
      ch->ctimer[7] = 2;

   ch->wait = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->ele_attack = ATTACK_DARK;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = LOW;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Placing your weapon in front of you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Placing $s weapon in front of $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_CHAR );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_ROOM );
   act( "You stare at your feet as you see a massive shadow of a dragon moving towards you!", ch, NULL, NULL, TO_CHAR );
   act( "You stare at your feet as you see a massive shadow of a dragon moving towards you!", ch, NULL, NULL, TO_ROOM );
   act( "The Dark Dragon then leaps from the shadows as it waves it tail around angrily!", ch, NULL, NULL, TO_CHAR );
   act( "The Dark Dragon then leaps from the shadows as it waves it tail around angrily!", ch, NULL, NULL, TO_ROOM );
   act( "As its tail dips into the ground again, another shadow of the dragon suddenly appears.", ch, NULL, NULL, TO_CHAR );
   act( "As its tail dips into the ground again, another shadow of the dragon suddenly appears.", ch, NULL, NULL, TO_ROOM );
   act( "The dragon lets out a loud shriek as its shadow suddenly zooms underneath you!", victim, NULL, NULL, TO_CHAR );
   act( "The dragon lets out a loud shriek as its shadow suddenly zooms underneath $n!", victim, NULL, NULL, TO_ROOM );
   act( "You scream loudly with fear as the shadow totally engulfs you within its depths.", victim, NULL, NULL, TO_CHAR );
   act( "$n screams loudly with fear as the shadow totally engulfs $m within its depths.", victim, NULL, NULL, TO_ROOM );
   act( "Letting out another shriek, the dragon flies away RiPpIng the shadow apart!!!", ch, NULL, NULL, TO_CHAR );
   act( "Letting out another shriek, the dragon flies away RiPpIng the shadow apart!!!", ch, NULL, NULL, TO_ROOM );
   act( "The blinding light vaporizes your flesh as you tremble in awe of the power.", victim, NULL, NULL, TO_CHAR );
   act( "The blinding light vaporizes $n's flesh as $e trembles in awe of the power.", victim, NULL, NULL, TO_ROOM );

   hurt_person( ch, victim, 3500000, TRUE );
}

void do_moonlight( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 10 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who to you want to revive?\n\r", ch );
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

   if ( IS_NPC( victim ) )
      {
         stc( "You can't revive non-players with your holy magic.\n\r", ch );
         return;
      }

   if ( victim->position >= POS_STUNNED )
      {
         stc( "You can only revive mortally wounded players with your holy magic.\n\r", ch );
         return;
      }

   if ( !can_heal( ch, victim ) )
      return;

   ch->mana -= ( ch->max_mana / 10 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clenching your blazing fists sadly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clenching $s blazing fists sadly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You flutter your angel wings rapidly as you push your palm in front of you.", ch, NULL, NULL, TO_CHAR );
   act( "$n flutters $s angel wings rapidly as $e pushes $s palm in front of $m.", ch, NULL, NULL, TO_ROOM );
   act( "As a mass of divine energy manifests on your palms, you sing '#wMoonLight!#n'", ch, NULL, NULL, TO_CHAR );
   act( "As a mass of divine energy manifests on $n's palms, $e sings '#wMoonLight!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The sky suddenly goes dark as the mass of energy starts pulsing softly.", ch, NULL, NULL, TO_CHAR );
   act( "The sky suddenly goes dark as the mass of energy starts pulsing softly.", ch, NULL, NULL, TO_ROOM );
   act( "The energy starts levitating above you as it transform into a beautiful moon.", ch, NULL, NULL, TO_CHAR );
   act( "The energy starts levitating above you as it transform into a beautiful moon.", ch, NULL, NULL, TO_ROOM );

   victim->position = POS_STANDING;
   victim->hit = victim->max_hit * 0.75;
   give_align( ch, 100 );

   act( "Suddenly a beam of holy light transcede from the moon as it engulfs $N!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly you feel your body being engulfed by a warm, gentle mass of energy!", ch, NULL, victim, TO_VICT );
   act( "Suddenly a beam of holy light transcede from the moon as it engulfs $N!", ch, NULL, victim, TO_NOTVICT );
   act( "You stagger back to your feet as you struggle to maintain balance in the light.", victim, NULL, NULL, TO_CHAR );
   act( "$n staggers back to $s feet as $e struggles to maintain balance in the light.", victim, NULL, NULL, TO_ROOM );
   act( "You whisper '#wGentle life...#n' as the soft moonlight vanishes from sight.", ch, NULL, NULL, TO_CHAR );
   act( "$n whispers '#wGentle life...#n' as the soft moonlight vanishes from sight.", ch, NULL, NULL, TO_ROOM );
}

void do_star_children( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
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

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clenching your blazing fists sadly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clenching $s blazing fists sadly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You spread your arms and legs apart as you stare into the heavens.", ch, NULL, NULL, TO_CHAR );
   act( "$n spreads $s arms and legs apart as $e stares into the heavens.", ch, NULL, NULL, TO_ROOM );
   act( "Your eyes suddenly radiates holy energy as you sing '#yStar Children!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n's eyes suddenly radiates holy energy as $e sings '#yStar Children!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The clouds above you parted as you are engulfed by a beam of holy energy.", ch, NULL, NULL, TO_CHAR );
   act( "The clouds above $n parted as $e is engulfed by a beam of holy energy.", ch, NULL, NULL, TO_ROOM );
   act( "A lone angel suddenly swoop down from the clouds as he encircles your body.", ch, NULL, NULL, TO_CHAR );
   act( "A lone angel suddenly swoop down from the clouds as he encircles $n's body.", ch, NULL, NULL, TO_ROOM );
   act( "As you coldly stare around you with your blazing eyes, a soft humming fill the air.", ch, NULL, NULL, TO_CHAR );
   act( "As $n coldly stares around $m with $s blazing eyes, a soft humming fill the air.", ch, NULL, NULL, TO_ROOM );
   act( "The room is suddenly filled with a flash of light as thousands of angels appear.", ch, NULL, NULL, TO_CHAR );
   act( "The room is suddenly filled with a flash of light as thousands of angels appear.", ch, NULL, NULL, TO_ROOM );
   act( "You smile as you open the holy book with your hands and start singing a hymn.", ch, NULL, NULL, TO_CHAR );
   act( "$n smiles as $e opens the holy book with $s hands and start singing a hymn.", ch, NULL, NULL, TO_ROOM );
   act( "The angels follow your suit as their voices fill the vicinity!", ch, NULL, NULL, TO_CHAR );
   act( "The angels follow $n's suit as their voices fill the vicinity!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "The voices fill your head with redemption as you clutch your head in pain!", vch, NULL, NULL, TO_CHAR );
      act( "The voices fill $n's head with redemption as $e clutches $s head in pain!", vch, NULL, NULL, TO_ROOM );

      if ( !IS_ITEMAFF( vch, ITEMA_DSTAFF ) && !IS_SET( vch->more3, MORE3_CONFUSE )
           && ( vch->in_room->vnum < 33500 || vch->in_room->vnum > 33800 )
           && number_percent() > 60 - devil_luck( ch, vch, 10 ) )
         SET_BIT( vch->more3, MORE3_CONFUSE );
      
      ch->ele_attack = ATTACK_HOLY;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = SOUL;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 2000000, TRUE );
   }
}

void do_heavens_gate( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 15 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 3 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 3 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clenching your blazing fists sadly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clenching $s blazing fists sadly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You tilt your hands together as sparks of electricity flashes between them.", ch, NULL, NULL, TO_CHAR );
   act( "$n tilts $s hands together as sparks of electricity flashes between them.", ch, NULL, NULL, TO_ROOM );
   act( "You sneer '#yArrrrrgh...#n' as the electricity fizzes through your body.", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#yArrrrrgh...#n' as the electricity fizzes through $s body.", ch, NULL, NULL, TO_ROOM );
   act( "As the electricity intensifies, a small vortex forms on the palm of your hands.", ch, NULL, NULL, TO_CHAR );
   act( "As the electricity intensifies, a small vortex forms on the palm of $n's hands.", ch, NULL, NULL, TO_ROOM );
   act( "Your eyes suddenly radiates holy energy as you sing '#yHeaven's Gate!#n'", ch, NULL, NULL, TO_CHAR );
   act( "$n's eyes suddenly radiates holy energy as $e sings '#yHeaven's Gate!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The vortex suddenly opens wider as it then explodes into a mass of energy!", ch, NULL, NULL, TO_CHAR );
   act( "The vortex suddenly opens wider as it then explodes into a mass of energy!", ch, NULL, NULL, TO_ROOM );
   act( "The divine energy shoots up into the sky filling it with multiple explosions!", ch, NULL, NULL, TO_CHAR );
   act( "The divine energy shoots up into the sky filling it with multiple explosions!", ch, NULL, NULL, TO_ROOM );
   act( "You close your eyes whispering '#yLovely Grace...#n' as snowflakes fall upon the room.", ch, NULL, NULL, TO_CHAR );
   act( "$n closes $s eyes whispering '#yLovely Grace...#n' as snowflakes fall upon the room.", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_ITEMAFF( vch, ITEMA_DSTAFF ) && !can_heal( ch, vch ) )
         continue;

      act( "You feel much better as the beautiful snowflakes heals your wounds!", vch, NULL, NULL, TO_CHAR );
      act( "$n feels much better as the beautiful snowflakes heals $s wounds!", vch, NULL, NULL, TO_ROOM );
      hit_gain( vch, 1000000 );
   }
}

void do_regeneration( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 2 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom to you want to give incredible regeneration powers?\n\r", ch );
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

   if ( ( sn = skill_lookup( "regeneration" ) ) < 0 )
      {
         stc( "That spell isn't in. Please consult an imm about this.\n\r", ch );
         return;
      }

   if ( is_affected( victim, sn ) )
      {
         stc( "That person's body is already regenning at a rapid pace.\n\r", ch );
         return;
      }

   ch->mana -= ( ch->max_mana / 2 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clenching your blazing fists sadly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clenching $s blazing fists sadly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You flutter your angel wings rapidly as you push your palm in front of you.", ch, NULL, NULL, TO_CHAR );
   act( "$n flutters $s angel wings rapidly as $e pushes $s palm in front of $m.", ch, NULL, NULL, TO_ROOM );
   act( "As a mass of divine energy manifests on your palms, you sing '#wRegeneration!#n'", ch, NULL, NULL, TO_CHAR );
   act( "As a mass of divine energy manifests on $n's palms, $e sings '#wRegeneration!#n'", ch, NULL, NULL, TO_ROOM );
   act( "You fly straight towards $N as you hover above $M smiling happily.", ch, NULL, victim, TO_CHAR );
   act( "$n flies straight towards you as $e hovers above you smiling happily.", ch, NULL, victim, TO_VICT );
   act( "$n flies straight towards $N as $e hovers above $M smiling happily.", ch, NULL, victim, TO_NOTVICT );
   act( "You then gently touch $N's forehead with your palm as $S body starts pulsing.", ch, NULL, victim, TO_CHAR );
   act( "$n then gently touches your forehead with $s palm as your body starts pulsing.", ch, NULL, victim, TO_VICT );
   act( "$n then gently touches $N's forehead with $s palm as $S body starts pulsing.", ch, NULL, victim, TO_NOTVICT );
   act( "You whisper '#yDivine Intervention...#n' as you release the mass of energy.", ch, NULL, NULL, TO_CHAR );
   act( "$n whispers '#yDivine Intervention...#n' as $e releases the mass of energy.", ch, NULL, NULL, TO_ROOM );
   act( "Shards of light emits of the energy as they spiral straight into your body.", victim, NULL, NULL, TO_CHAR );
   act( "Shards of light emits of the energy as they spiral straight into $n's body.", victim, NULL, NULL, TO_ROOM );
   act( "You feel your wounds rapidly disappearing as your bones become stronger.", victim, NULL, NULL, TO_CHAR );
   act( "$n feels $s wounds rapidly disappearing as $s bones become stronger.", victim, NULL, NULL, TO_ROOM );

   spell_regeneration( sn, 1000, ch, victim );
}

void do_silver_dragon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *vch, *vch_next;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom to you want to show redemption to?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->ele_attack = ATTACK_HOLY;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Clenching your blazing fists sadly, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Clenching $s blazing fists sadly, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_CHAR );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_ROOM );
   act( "You snarl '#yArrrrrrgh#n' as you tilt your hands while crouching forward.", ch, NULL, NULL, TO_CHAR );
   act( "$n snarls '#yArrrrrrgh#n' as $e tilts $s hands while crouching forward.", ch, NULL, NULL, TO_ROOM );
   act( "A mass of energy manifests between your hands releasing soft waves of light.", ch, NULL, NULL, TO_CHAR );
   act( "A mass of energy manifests between $n's hands releasing soft waves of light.", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_ITEMAFF( vch, ITEMA_DSTAFF ) && !can_heal( ch, vch ) )
         continue;

      if ( vch == ch )
         continue;

      act( "You feel much better as the gentle waves caresses your body!", vch, NULL, NULL, TO_CHAR );
      act( "$n feels much better as the gentle waves caresses $s body!", vch, NULL, NULL, TO_ROOM );

      hit_gain( vch, 1500000 );

      vch->move = vch->max_move;
      vch->mana = vch->max_mana;
      vch->wait = 0;
   }

   act( "You scream loudly as you release the blast of energy towards $N!", ch, NULL, victim, TO_CHAR );
   act( "$n screams loudly as $e releases the blast of energy towards you!", ch, NULL, victim, TO_VICT );
   act( "$n screams loudly as $e releases the blast of energy towards $N!", ch, NULL, victim, TO_NOTVICT );
   act( "The beam midair starts pulsing as it suddenly transform into a screaming dragon!", ch, NULL, NULL, TO_CHAR );
   act( "The beam midair starts pulsing as it suddenly transform into a screaming dragon!", ch, NULL, NULL, TO_ROOM );
   act( "Leaving behind a trail of stardust, the Silver Dragon flies straight into you!", victim, NULL, NULL, TO_CHAR );
   act( "Leaving behind a trail of stardust, the Silver Dragon flies straight into $n!", victim, NULL, NULL, TO_ROOM );
   act( "You scream loudly in pain as the dragon shatters into silver shards onto you!", victim, NULL, NULL, TO_CHAR );
   act( "You scream loudly in pain as the dragon shatters into silver shards onto $m!", victim, NULL, NULL, TO_ROOM );

   hurt_person( ch, victim, 3500000, TRUE );
}

void do_freezing_rain( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];
   int dam = 850000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 10 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast with freezing energy?\n\r", ch );
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

   ch->mana -= ( ch->max_mana / 10 );
   ch->wait = 12;
   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = AERIAL;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You draw a magical symbol with your hand in the air as your eyes sparkles.", ch, NULL, NULL, TO_CHAR );
   act( "$n draws a magical symbol with $s hand in the air as $s eyes sparkles.", ch, NULL, NULL, TO_ROOM );
   act( "The symbol pulses with energy as the sky around you brightens.", ch, NULL, NULL, TO_CHAR );
   act( "The symbol pulses with energy as the sky around you brightens.", ch, NULL, NULL, TO_ROOM );
   act( "You sneer '#yFreezing Rain!#n' as you clench your fist viciously.", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#yFreezing Rain!#n' as $s clench $s fist viciously.", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, shard of icicles fall from the heavens straight into you!", victim, NULL, NULL, TO_CHAR );
   act( "Suddenly, shard of icicles fall from the heavens straight into $n!", victim, NULL, NULL, TO_ROOM );
   act( "You giggle as $N screams in pain each time an ice shard impales $M.", ch, NULL, victim, TO_CHAR );
   act( "$n giggles as you scream in pain each time an ice shard impales you.", ch, NULL, victim, TO_VICT );
   act( "$n giggles as $N screams in pain each time an ice shard impales $M.", ch, NULL, victim, TO_NOTVICT );
   act( "Your face suddenly darkens with anger as you open your fist quickly.", ch, NULL, NULL, TO_CHAR );
   act( "$n's face suddenly darkens with anger as $e opens $s fist quickly.", ch, NULL, NULL, TO_ROOM );
   act( "A MaSsIvE IcE ShArD falls straight ThroUgH yOuR weakened BoDy!", victim, NULL, NULL, TO_CHAR );
   act( "A MaSsIvE IcE ShArD falls straight ThroUgH $n's weakened BoDy!", victim, NULL, NULL, TO_ROOM );

   hurt_person( ch, victim, dam, TRUE );
}

void do_rainbow_breath( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 15 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 5 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 5 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You draw a magical symbol with your hand in the air as your eyes sparkles.", ch, NULL, NULL, TO_CHAR );
   act( "$n draws a magical symbol with $s hand in the air as $s eyes sparkles.", ch, NULL, NULL, TO_ROOM );
   act( "You tilt your hands together as a beam of light arcs between them.", ch, NULL, NULL, TO_CHAR );
   act( "$n tilts $s hands together as a beam of light arcs between them.", ch, NULL, NULL, TO_ROOM );
   act( "The light suddenly bursts into a rainbow as the symbol starts pulsing.", ch, NULL, NULL, TO_CHAR );
   act( "The light suddenly bursts into a rainbow as the symbol starts pulsing.", ch, NULL, NULL, TO_ROOM );
   act( "Throwing it above you, you sneer '#yRainbow Breath!#n' as you draw in your own.", ch, NULL, NULL, TO_CHAR );
   act( "Throwing it above $mself, $n sneers '#yRainbow Breath!#n' as $e draws in $s own.", ch, NULL, NULL, TO_ROOM );
   act( "You then let go a soft puff of freezing air as the rainbow flies across the room.", ch, NULL, NULL, TO_CHAR );
   act( "$n then lets go a soft puff of freezing air as the rainbow flies across the room.", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_ITEMAFF( vch, ITEMA_DSTAFF ) && !can_heal( ch, vch ) )
         continue;

      act( "You feel much better as the gentle rainbow bathes your body!", vch, NULL, NULL, TO_CHAR );
      act( "$n feels much better as the gentle rainbow bathes $s body!", vch, NULL, NULL, TO_ROOM );
      clear_affects( vch );
      hit_gain( vch, 750000 );
   }
}

void do_dragoon_dust( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 950000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc( "You aren't powerful enough to use this magical ability.\n\r", ch );
         return;
      }

   if ( ch->mana < ch->max_mana / 3 )
      { 
         stc("You don't have enough magical energy to do this mystical ability.\n\r",ch);
         return;
      }

   ch->mana -= ( ch->max_mana / 3 );
   ch->wait = 12;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You draw a magical symbol with your hand in the air as your eyes sparkles.", ch, NULL, NULL, TO_CHAR );
   act( "$n draws a magical symbol with $s hand in the air as $s eyes sparkles.", ch, NULL, NULL, TO_ROOM );
   act( "A MaSsIvE IcE sHaRd JoLts from the GroUnD as you land upon it.", ch, NULL, NULL, TO_CHAR );
   act( "A MaSsIvE IcE sHaRd JoLts from the GroUnD as $n lands upon it.", ch, NULL, NULL, TO_ROOM );
   act( "It suddenly starts trembling as the symbol pulses with energy.", ch, NULL, NULL, TO_CHAR );
   act( "It suddenly starts trembling as the symbol pulses with energy.", ch, NULL, NULL, TO_ROOM );
   act( "You then raise your fist above you as it starts radiating blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "$n then raises $s fist above $m as it starts radiating blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "You sneer '#yDiamond Dust!#n' as you SmAsH your fist right into the IcE ShArD!", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#yDiamond Dust!#n' as $e SmAsHeS $s fist right into the IcE ShArD!", ch, NULL, NULL, TO_ROOM );
   act( "The IcIcLe ViOlEnTlY EXploDES as deadly shards of ice flies everywhere!!", ch, NULL, NULL, TO_CHAR );
   act( "The IcIcLe ViOlEnTlY EXploDES as deadly shards of ice flies everywhere!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act( "You scream loudly in pain as you are struck by a series of sharp icicles!", vch, NULL, NULL, TO_CHAR );
      act( "$n screams loudly in pain as $e is struck by a series of sharp icicles!", vch, NULL, NULL, TO_ROOM );
      
      ch->ele_attack = ATTACK_ICE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = PIERCE;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, dam, TRUE );
   }
}

void do_dragoon_avalanche( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "You draw a magical symbol with your hand in the air as your eyes sparkles.", ch, NULL, NULL, TO_CHAR );
   act( "$n draws a magical symbol with $s hand in the air as $s eyes sparkles.", ch, NULL, NULL, TO_ROOM );
   act( "You hit your chest angrily as you sneer '#yAvalanche!#n", ch, NULL, NULL, TO_CHAR );
   act( "$n hits $s chest angrily as $e sneers '#yAvalanche!#n", ch, NULL, NULL, TO_ROOM );
   act( "You open your fist as the symbol suddenly bursts into a snowy outpour!", ch, NULL, NULL, TO_CHAR );
   act( "$n opens $s fist as the symbol suddenly bursts into a snowy outpour!", ch, NULL, NULL, TO_ROOM );
   act( "You feel a wonderful chill through your bones as ferocious winds grazes you!", ch, NULL, NULL, TO_CHAR );
   act( "You feel a hideous chill through your bones as ferocious winds grazes you!", ch, NULL, NULL, TO_ROOM );
   act( "You grin evilly as a HuGe SnOw AvAlAnChe BLAST through the vicinity below!!!", ch, NULL, NULL, TO_CHAR );
   act( "$n grins evilly as a HuGe SnOw AvAlAnChe BLAST through the vicinity below!!!", ch, NULL, NULL, TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_ITEMAFF( vch, ITEMA_DSTAFF ) || can_heal( ch, vch ) )
         {
            act( "The snow washes through your body as your wounds start disappearing!", vch, NULL, NULL, TO_CHAR );
            act( "The snow washes through $n's body as $s wounds start disappearing!", vch, NULL, NULL, TO_ROOM );
            hit_gain( vch, 500000 );
            continue;
         }

      if ( is_safe(ch,vch) )
         continue;

      act( "You frail your arms in vain as you are swept away by the deadly snow!", vch, NULL, NULL, TO_CHAR );
      act( "$n frails $s arms in vain as $e is swept away by the deadly snow!", vch, NULL, NULL, TO_ROOM );
      
      ch->ele_attack = ATTACK_ICE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = HIT;
      ch->attack_focus = LOW;
      hurt_person( ch, vch, 2000000, TRUE );
   }
}

void do_bluesea_dragon( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) || !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER ) )
      {
         stc("But you aren't in your dragoon form.\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast with freezing energy?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = AERIAL;

   act( "You fly straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_CHAR );
   act( "$n flies straight into the air leaving behind a trail of stardust.", ch, NULL, NULL, TO_ROOM );
   act( "Swerving your body around you, You concentrate as energy emits of you.", ch, NULL, NULL, TO_CHAR );
   act( "Swerving $s body around $m, $n concentrates as energy emits of $m.", ch, NULL, NULL, TO_ROOM );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_CHAR );
   act( "The room shimmers as it is filled with blinding light!", ch, NULL, NULL, TO_ROOM );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_CHAR );
   act( "As the light fades back to normal, you find yourself within a colourful vortex!", ch, NULL, NULL, TO_ROOM );
   act( "Splashes of water hit your face as you see a shadow leaps out and back into it!", ch, NULL, NULL, TO_CHAR );
   act( "Splashes of water hit your face as you see a shadow leaps out and back into it!", ch, NULL, NULL, TO_ROOM );
   act( "Suddenly, a long slithering dragon leaps out of the water again in front of you!", ch, NULL, NULL, TO_CHAR );
   act( "Suddenly, a long slithering dragon leaps out of the water again in front of you!", ch, NULL, NULL, TO_ROOM );
   act( "Letting out a loud shriek, it starts circling around you savagely!", victim, NULL, NULL, TO_CHAR );
   act( "Letting out a loud shriek, it starts circling around $n savagely!", victim, NULL, NULL, TO_ROOM );
   act( "As it goes faster and faster, the water starts swerving into a deadly whirlpool!", victim, NULL, NULL, TO_CHAR );
   act( "As it goes faster and faster, the water starts swerving into a deadly whirlpool!", victim, NULL, NULL, TO_ROOM );
   act( "Your eyes fill with terror as you are suddenly sucked into the whirlpool!", victim, NULL, NULL, TO_CHAR );
   act( "$n's eyes fill with terror as $e is suddenly sucked into the whirlpool!", victim, NULL, NULL, TO_ROOM );
   act( "Looking above you, you see the Sea Dragon leaping into the air...", victim, NULL, NULL, TO_CHAR );
   act( "Looking above you, you see the Sea Dragon leaping into the air...", victim, NULL, NULL, TO_ROOM );
   act( "The Dragon's skin turns into crystal as it DiVeS viciously into you!!!", victim, NULL, NULL, TO_CHAR );
   act( "The Dragon's skin turns into crystal as it DiVeS viciously into $n!!!", victim, NULL, NULL, TO_ROOM );
   act( "The entire ground trembles as HuGe waVeS SpLaShEs vIoLenTly EvErYhWeRe!!!", victim, NULL, NULL, TO_CHAR );
   act( "The entire ground trembles as HuGe waVeS SpLaShEs vIoLenTly EvErYhWeRe!!!", victim, NULL, NULL, TO_ROOM );

   hurt_person( ch, victim, 3500000, TRUE );
}
