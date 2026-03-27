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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h>
#endif
#include "merc.h"

void do_research( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       stc("#w/-----------------------Animal    Spirits-----------------------|\n\r",ch);
       stc("#w|   #gCrane                     -     ",ch);

       if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 1 ) )
          stc("#y(Known)#w                     |\n\r",ch);
          else stc("#e(UnKnown)#w                  |\n\r",ch);
       
       stc("#w|   #gSnake                     -     ",ch);

       if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 2 ) )
          stc("#y(Known)#w                     |\n\r",ch);
          else stc("#e(UnKnown)#w                  |\n\r",ch);

       stc("#w|   #gTiger                     -     ",ch);

       if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 4 ) )
          stc("#y(Known)#w                     |\n\r",ch);
          else stc("#e(UnKnown)#w                  |\n\r",ch);

       stc("#w|   #gDragon                    -     ",ch);

       if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 8 ) )
          stc("#y(Known)#w                     |\n\r",ch);
          else stc("#e(UnKnown)#w                  |\n\r",ch);

       stc("#w|   #gLeopard                   -     ",ch);

       if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 16 ) )
          stc("#y(Known)#w                     |\n\r",ch);
          else stc("#e(UnKnown)#w                  |\n\r",ch);

       if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 32 ) )
          {
               stc("#w|   #gPhoenix                   -     ",ch);
               stc("#y(Known)#w                     |\n\r",ch);
          }

       stc("#w\\-----------------------Ninjitsu Techniques---------------------/\n\r",ch);

       sprintf( buf, "#w|   #gThrust Kick                                         #y[%3d%%]#w  |\n\r",ch->nlist[0]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gSpin Kick                                           #y[%3d%%]#w  |\n\r",ch->nlist[1]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gNSweep                                              #y[%3d%%]#w  |\n\r",ch->nlist[2]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gReverse Sweep                                       #y[%3d%%]#w  |\n\r",ch->nlist[3]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gUppercut                                            #y[%3d%%]#w  |\n\r",ch->nlist[4]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gBackfist                                            #y[%3d%%]#w  |\n\r",ch->nlist[5]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gLow Kick                                            #y[%3d%%]#w  |\n\r",ch->nlist[6]);
       stc(buf,ch);
       sprintf( buf, "#w|   #gNKnee                                               #y[%3d%%]#w  |\n\r",ch->nlist[7]);
       stc(buf,ch);

       stc("#w\\----------------------------#bInfo#w-------------------------------/\n\r",ch);
       stc("#w|   #pTo learn an animal spirit, Type Research (spirit)#w           |\n\r",ch);
       stc("#w|   #pTo learn Ninjitsu Techniques, keep on performing it till 100#w|\n\r",ch);
       stc("#w\\---------------------------------------------------------------/\n\r",ch);
       return;
    }

    if ( !str_cmp(arg,"crane") )
       {
           SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],1);
           stc("Ok.\n\r",ch);

           if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 1 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 2 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 4 )
                && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 8 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 16 ) )
              {
                 stc("The way of the #RPhoenix#n is shown to #eyou#n.\n\r",ch);
                 SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],32);
              }

           return;
       }
       else if ( !str_cmp(arg,"snake") )
       {
           SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],2);
           stc("Ok.\n\r",ch);

           if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 1 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 2 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 4 )
                && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 8 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 16 ) )
              {
                 stc("The way of the #RPhoenix#n is shown to #eyou#n.\n\r",ch);
                 SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],32);
              }

           return;
       }
       else if ( !str_cmp(arg,"tiger") )
       {
           SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],4);
           stc("Ok.\n\r",ch);

           if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 1 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 2 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 4 )
                && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 8 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 16 ) )
              {
                 stc("The way of the #RPhoenix#n is shown to #eyou#n.\n\r",ch);
                 SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],32);
              }

           return;
       }
       else if ( !str_cmp(arg,"dragon") )
       {
           SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],8);
           stc("Ok.\n\r",ch);

           if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 1 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 2 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 4 )
                && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 8 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 16 ) )
              {
                 stc("The way of the #RPhoenix#n is shown to #eyou#n.\n\r",ch);
                 SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],32);
              }

           return;
       }
       else if ( !str_cmp(arg,"leopard") )
       {
           SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],16);
           stc("Ok.\n\r",ch);

           if ( IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 1 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 2 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 4 )
                && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 8 ) && IS_SET( ch->pcdata->ninja_powers[NINJA_EMBODY], 16 ) )
              {
                 stc("The way of the #RPhoenix#n is shown to #eyou#n.\n\r",ch);
                 SET_BIT(ch->pcdata->ninja_powers[NINJA_EMBODY],32);
              }

           return;
       }
       else do_research(ch,"");
}

bool is_ninja_combo( CHAR_DATA *ch, int move )
{
   /* special_id - last command entered */
   /* special_id_2 - nth move in combo string */
  
   if ( ch->special_id_2 == 0 )
      return TRUE; 

   /* (Dragon) Dragon Wave - Uppercut, Knee, Backfist, Spin, Uppercut, Sweep */
   /* (Snake) Dolphin Kick - Thrust, Spin, Sweep, Backfist, Lowkick, Knee */
   /* (Tiger) Hand Swerve  - Lowkick, Sweep, Thrust, Knee, Reverse, Backfist */
   /* (Crane) Shindo-Rappa - Knee, Knee, Uppercut, Knee, Thrust, Reverse */
   /* (Phoenix) Fire Ball  - Uppercut, Sweep, Lowkick, Lowkick, Knee, Lowkick */

   /* (Snake) */
   if ( ch->special_id == 1 && ch->special_id_2 == 1 && move == 2 )
      return TRUE;
   if ( ch->special_id == 2 && ch->special_id_2 == 2 && move == 3 )
      return TRUE;
   if ( ch->special_id == 3 && ch->special_id_2 == 3 && move == 6 )
      return TRUE;
   if ( ch->special_id == 6 && ch->special_id_2 == 4 && move == 7 )
      return TRUE;
   if ( ch->special_id == 7 && ch->special_id_2 == 5 && move == 8 )
      return TRUE;

   /* (Dragon) */
   if ( ch->special_id == 5 && ch->special_id_2 == 1 && move == 8 )
      return TRUE;
   if ( ch->special_id == 8 && ch->special_id_2 == 2 && move == 6 )
      return TRUE;
   if ( ch->special_id == 6 && ch->special_id_2 == 3 && move == 2 )
      return TRUE;
   if ( ch->special_id == 2 && ch->special_id_2 == 4 && move == 5 )
      return TRUE;
   if ( ch->special_id == 5 && ch->special_id_2 == 5 && move == 3 )
      return TRUE;

   /* (Tiger) */
   if ( ch->special_id == 7 && ch->special_id_2 == 1 && move == 3 )
      return TRUE;
   if ( ch->special_id == 3 && ch->special_id_2 == 2 && move == 1 )
      return TRUE;
   if ( ch->special_id == 1 && ch->special_id_2 == 3 && move == 8 )
      return TRUE;
   if ( ch->special_id == 8 && ch->special_id_2 == 4 && move == 4 )
      return TRUE;
   if ( ch->special_id == 4 && ch->special_id_2 == 5 && move == 6 )
      return TRUE;

   /* (Crane) */
   if ( ch->special_id == 8 && ch->special_id_2 == 1 && move == 8 )
      return TRUE;
   if ( ch->special_id == 8 && ch->special_id_2 == 2 && move == 5 )
      return TRUE;
   if ( ch->special_id == 5 && ch->special_id_2 == 3 && move == 8 )
      return TRUE;
   if ( ch->special_id == 8 && ch->special_id_2 == 4 && move == 1 )
      return TRUE;
   if ( ch->special_id == 1 && ch->special_id_2 == 5 && move == 4 )
      return TRUE;

   /* (Phoenix) */
   if ( ch->special_id == 5 && ch->special_id_2 == 1 && move == 3 )
      return TRUE;
   if ( ch->special_id == 3 && ch->special_id_2 == 2 && move == 7 )
      return TRUE;
   if ( ch->special_id == 7 && ch->special_id_2 == 3 && move == 7 )
      return TRUE;
   if ( ch->special_id == 7 && ch->special_id_2 == 4 && move == 8 )
      return TRUE;
   if ( ch->special_id == 8 && ch->special_id_2 == 5 && move == 7 )
      return TRUE;

   return FALSE;
}

void do_thrust_kick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   one_argument( argument, arg );

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to kick in the stomach?\n\r", ch );
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

   if ( !is_ninja_combo(ch,1) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 1;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   act("You thrust your foot into $N's stomach.",ch,NULL,victim,TO_CHAR);
   act("$n thrusts $s foot into your stomach.",ch,NULL,victim,TO_VICT);
   act("$n thrusts $s foot into $N's stomach.",ch,NULL,victim,TO_NOTVICT);
}

void do_spin_kick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_MONK ) )
      {
          do_kuruda_spinkick( ch, argument );
          return;
      }

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to kick in the chest?\n\r", ch );
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

   if ( !is_ninja_combo(ch,2) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 2;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   act("You whirl your foot into $N's chest.",ch,NULL,victim,TO_CHAR);
   act("$n whirls $s foot into your chest.",ch,NULL,victim,TO_VICT);
   act("$n whirls $s foot into $N's chest.",ch,NULL,victim,TO_NOTVICT);
}

void do_nsweep( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   one_argument( argument, arg );

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to trip down?\n\r", ch );
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

   if ( !is_ninja_combo(ch,3) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 3;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 6 )
      {
          int count;

          ch->special_id = 0;
          ch->special_id_2 = 0;
          ch->wait = 4;

          if ( ch->ninjacrap == 2 )
             {
                ch->wait = 12;
                ch->ninjacrap = 0;
             }
             else ch->ninjacrap = 2;

          act("You mutter '#yIzn Zeik...#n' as you are surrounded by a red aura.",ch,NULL,NULL,TO_CHAR);
          act("$n mutters '#yIzn Zeik...#n' as $e is surrounded by a red aura.",ch,NULL,victim,TO_VICT);
          act("$n mutters '#yIzn Zeik...#n' as $e is surrounded by a red aura.",ch,NULL,NULL,TO_NOTVICT);

          act("You yell '#yZen Shorukara Donden!!!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n yells '#yZen Shorukara Donden!!!#n'",ch,NULL,victim,TO_VICT);
          act("$n yells '#yZen Shorukara Donden!!!#n'",ch,NULL,NULL,TO_NOTVICT);

          for ( count = 1; count < 5; count++ )
          {
             act("Your dragon uppercut DeCiMaTeS $N as you spin around.", ch,NULL,victim,TO_CHAR );
             act("$n's dragon uppercut DeCiMaTeS you as $e spins around.", ch,NULL,victim,TO_VICT );
             act("$n's dragon uppercut DeCiMaTeS $N as $e spins around.", ch,NULL,victim,TO_NOTVICT );

             act("Your shadowy image's dragon uppercut DeCiMaTeS $N.", ch,NULL,victim,TO_CHAR );
             act("$n's shadowy image DeCiMaTeS YOU with a dragon uppercut.", ch,NULL,victim,TO_VICT );
             act("$n's shadowy image DeCiMaTeS $N with a dragon uppercut DeCiMaTeS $N.", ch,NULL,victim,TO_NOTVICT );

             act("Your shadowy image's dragon uppercut DeCiMaTeS $N.", ch,NULL,victim,TO_CHAR );
             act("$n's shadowy image DeCiMaTeS YOU with a dragon uppercut.", ch,NULL,victim,TO_VICT );
             act("$n's shadowy image DeCiMaTeS $N with a dragon uppercut DeCiMaTeS $N.", ch,NULL,victim,TO_NOTVICT );
          }

          act("You land back on your feet as $N crashes into the ground.", ch,NULL,victim,TO_CHAR );
          act("$n lands back onto $s feet silently.", ch,NULL,NULL,TO_ROOM );
          ch->attack_type = ATTACK_PHYSICAL;
          ch->ele_attack = ATTACK_DARK;
          ch->attack_var = HIT;
          ch->attack_focus = UPPERCUT;
          SET_BIT( ch->arena, AFF2_COMBO );

          if ( IS_CLASS( ch, CLASS_MONK ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
             hurt_person( ch, victim, 700000, TRUE );
             else hurt_person( ch, victim, 500000, TRUE );

          return;
      }

   act("You suddenly twirl your foot under $N's feet, tripping $M down.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly twirls $s foot under your feet, tripping you down.",ch,NULL,victim,TO_VICT);
   act("$n suddenly twirls $s foot under $N's feet, tripping $M down.",ch,NULL,victim,TO_NOTVICT);
}

void do_reverse( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to trip down?\n\r", ch );
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

   if ( !is_ninja_combo(ch,4) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 4;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 6 )
      {
          int count;

          ch->special_id = 0;
          ch->special_id_2 = 0;
          ch->wait = 4;

          if ( ch->ninjacrap == 5 )
             {
                ch->wait = 12;
                ch->ninjacrap = 0;
             }
             else ch->ninjacrap = 5;

          act("You mutter '#yDDDDooooo#n' as you slam your fist into $N's stomach, sending $M to $S knees.",ch,NULL,victim,TO_CHAR);
          act("$n mutters '#yDDDDooooo#n' as $e slams $s fist into your stomach, sending you to your knees.",ch,NULL,victim,TO_VICT);
          act("$n mutters '#yDDDDooooo#n' as $e slams $s fist into $N's stomach, sending $M to $S knees.",ch,NULL,victim,TO_NOTVICT);

          act("As $N gasps for breath, you thrust your palm into $S throat staggering $M.",ch,NULL,victim,TO_CHAR);
          act("As you gasp for breath, $n thrusts $s palm into your throat staggering you.",ch,NULL,victim,TO_VICT);
          act("As $N gasps for breath, $n thrusts $s palm into $S throat staggering $M.",ch,NULL,victim,TO_NOTVICT);

          act("You mutter '#yRRRRapppppa#n' as you give $N a vicious uppercut, sending $M into the air.",ch,NULL,victim,TO_CHAR);
          act("$n mutters '#yRRRRapppppa#n' as $e gives you a vicious uppercut, sending you flying into the air.",ch,NULL,victim,TO_VICT);
          act("$n mutters '#yRRRRapppppa#n' as $e gives $N a vicious uppercut, sending $M into the air.",ch,NULL,victim,TO_NOTVICT);

          for ( count = 0; count < 5; count++ )
          {
             act("Your image gives $N a vicious uppercut, sending $M into the air.",ch,NULL,victim,TO_CHAR);
             act("$n's image gives you a vicious uppercut, sending you flying into the air.",ch,NULL,victim,TO_VICT);
             act("$n's image gives $N a vicious uppercut, sending $M into the air.",ch,NULL,victim,TO_NOTVICT);
          }

          ch->ele_attack = ATTACK_EARTH;
          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = HIT;
          ch->attack_focus = UPPERCUT;
          SET_BIT( ch->arena, AFF2_COMBO );

          if ( IS_CLASS( ch, CLASS_MONK ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
             hurt_person( ch, victim, 700000, TRUE );
             else hurt_person( ch, victim, 500000, TRUE );

          return;
      }

   act("You catch $N offguard by spinning your foot under $S's feet, tripping $M down.",ch,NULL,victim,TO_CHAR);
   act("$n catches you offguard by spinning $s foot under your feet, tripping you down.",ch,NULL,victim,TO_VICT);
   act("$n catches $N offguard by spinning $s foot under $S's feet, tripping $M down.",ch,NULL,victim,TO_NOTVICT);
}

void do_uppercut( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to punch in the face?\n\r", ch );
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

   if ( !is_ninja_combo(ch,5) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 5;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   act("You smash your fist upwards $N's face.",ch,NULL,victim,TO_CHAR);
   act("$n smashes $s fist upwards your face.",ch,NULL,victim,TO_VICT);
   act("$n smashes $s fist upwards $N's face.",ch,NULL,victim,TO_NOTVICT);
}

void do_backfist( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to smash in the face?\n\r", ch );
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

   if ( !is_ninja_combo(ch,6) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 6;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 6 )
      {
          ch->special_id = 0;
          ch->special_id_2 = 0;
          ch->wait = 4;

          if ( ch->ninjacrap == 3 )
             {
                ch->wait = 12;
                ch->ninjacrap = 0;
             }
             else ch->ninjacrap = 3;

          act("You mutter '#yDoras#n' as you slam your fist into $N's stomach, sending $M to $S knees.",ch,NULL,victim,TO_CHAR);
          act("$n mutters '#yDoras#n' as $e slams $s fist into your stomach, sending you to your knees.",ch,NULL,victim,TO_VICT);
          act("$n mutters '#yDoras#n' as $e slams $s fist into $N's stomach, sending $M to $S knees.",ch,NULL,victim,TO_NOTVICT);

          act("As $N gasps for breath, you thrust your palm into $S throat staggering $M.",ch,NULL,victim,TO_CHAR);
          act("As you gasp for breath, $n thrusts $s palm into your throat staggering you.",ch,NULL,victim,TO_VICT);
          act("As $N gasps for breath, $n thrusts $s palm into $S throat staggering $M.",ch,NULL,victim,TO_NOTVICT);

          act("You mutter '#ySiniku...#n' as you give $N a vicious uppercut, sending $M into the air.",ch,NULL,victim,TO_CHAR);
          act("$n mutters '#ySiniku...#n' as $e gives you a vicious uppercut, sending you flying into the air.",ch,NULL,victim,TO_VICT);
          act("$n mutters '#ySiniku...#n' as $e gives $N a vicious uppercut, sending $M into the air.",ch,NULL,victim,TO_NOTVICT);

          act("You leap into the air, spinning your foot in $N's face.",ch,NULL,victim,TO_CHAR);
          act("$n leaps into the air, spinning $s foot in your face.",ch,NULL,victim,TO_VICT);
          act("$n leaps into the air, spinning $s foot in $N's face.",ch,NULL,victim,TO_NOTVICT);

          act("You yell '#ySei!#n' as you DRIVE your elbow onto $N's back, SmAsHiNg $M onto the ground!",ch,NULL,victim,TO_CHAR);
          act("$n yells '#ySei!#n' as $e DRIVES $s elbow onto your back, SmAsHiNg you onto the ground!",ch,NULL,victim,TO_VICT);
          act("$n yells '#ySei!#n' as $e DRIVES $s elbow onto $N's back, SmAsHiNg $M onto the ground!",ch,NULL,victim,TO_NOTVICT);

          ch->ele_attack = ATTACK_THUNDER;
          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = HIT;
          ch->attack_focus = AERIAL;
          SET_BIT( ch->arena, AFF2_COMBO );

          if ( IS_CLASS( ch, CLASS_MONK ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
             hurt_person( ch, victim, 700000, TRUE );
             else hurt_person( ch, victim, 500000, TRUE );

          return;
      }

   act("You suddenly turn around smashing your palm into $N's face.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly turns around smashing $s palm into your face.",ch,NULL,victim,TO_VICT);
   act("$n suddenly turns around smashing $s palm into $N's face.",ch,NULL,victim,TO_NOTVICT);
}

void do_lowkick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to kick in the knee?\n\r", ch );
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

   if ( !is_ninja_combo(ch,7) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 7;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 6 )
      {
          act("You draw back your right hand as it glows with blazing energy.",ch,NULL,NULL,TO_CHAR);
          act("$n draws back $s right hand as it glows with blazing energy.",ch,NULL,NULL,TO_ROOM);
          act("You yell '#yJustice Feest!#n' as you dash towards $N, SmAsHiNg your fist into $S stomach!",ch,NULL,victim,TO_CHAR);
          act("$n yells '#yJustice Feest!#n' as $e dashes towards you, SmAsHiNg $s fist into your stomach!",ch,NULL,victim,TO_VICT);
          act("$n yells '#yJustice Feest!#n' as $e dashes towards $N, SmAsHiNg $s fist into $S stomach!",ch,NULL,victim,TO_NOTVICT);
          act("You scream loudly as a MaSsIvE explosion pulses through you!",victim,NULL,NULL,TO_CHAR);
          act("$n scream loudly as a MaSsIvE explosion pulses through $m!",victim,NULL,NULL,TO_ROOM);

          ch->special_id = 0;
          ch->special_id_2 = 0;
          ch->wait = 4;

          if ( ch->ninjacrap == 4 )
             {
                ch->wait = 12;
                ch->ninjacrap = 0;
             }
             else ch->ninjacrap = 4;

          act("You yell '#yOOoOoOoo Firie BaLl!!!#n' as you release a MaSsIvE fireball at $N!", ch,NULL,victim,TO_CHAR );
          act("$n yells '#yOOoOoOoo Firie BaLl!!!#n' as $e releases a MaSsIvE fireball at you!", ch,NULL,victim,TO_VICT );
          act("$n yells '#yOOoOoOoo Firie BaLl!!!#n' as $e releases a MaSsIvE fireball at $N!", ch,NULL,victim,TO_NOTVICT );
          act("The fireball SmAsHeS through you as the room shakes violently from the impact!", victim,NULL,NULL,TO_CHAR );
          act("The fireball SmAsHeS through $n as the room shakes violently from the impact!", victim,NULL,NULL,TO_ROOM );
          act("$N crashes onto the ground in pain.", ch,NULL,victim,TO_CHAR );
          act("you crash onto the ground unconscious.", ch,NULL,victim,TO_VICT );
          act("$N crashes onto the ground in pain.", ch,NULL,victim,TO_NOTVICT );

          ch->ele_attack = ATTACK_FIRE;
          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = BLAST;
          ch->attack_focus = DIRECT;
          SET_BIT( ch->arena, AFF2_COMBO );

          if ( IS_CLASS( ch, CLASS_MONK ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
             hurt_person( ch, victim, 700000, TRUE );
             else hurt_person( ch, victim, 500000, TRUE );

          return;
      }

   act("You twirl your foot across $N's knees.",ch,NULL,victim,TO_CHAR);
   act("$n twirls $s foot across into your knees.",ch,NULL,victim,TO_VICT);
   act("$n twirls $s foot across into $N's knees.",ch,NULL,victim,TO_NOTVICT);
}

void do_nknee( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_NINJA) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to knee in the stomach?\n\r", ch );
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

   if ( !is_ninja_combo(ch,8) )
      {
         ch->special_id = 0;
         ch->special_id_2 = 0;
         ch->wait = 12;
      }
      else {
              ch->special_id = 8;
              ch->special_id_2++;
              ch->wait = 0 ;
           }

   if ( ch->special_id_2 == 6 )
      {
          int count;

          ch->special_id = 0;
          ch->special_id_2 = 0;
          ch->wait = 4;

          if ( ch->ninjacrap == 1 )
             {
                ch->wait = 12;
                ch->ninjacrap = 0;
             }
             else ch->ninjacrap = 1;

          for ( count = 1; count < 5; count++ )
          {
             if ( victim->position == POS_DEAD )
                return;

             act("Your lightning kicks smashes into $N viciously.", ch,NULL,victim,TO_CHAR );
             act("$n's feet become blurry as they appear to strike you all over your face.", ch,NULL,victim,TO_VICT );
             act("$n's lightning kicks smashes into $N viciously.", ch,NULL,victim,TO_NOTVICT );
          } 

          act("You suddenly trip $N with a rapid foot sweep.", ch,NULL,victim,TO_CHAR );
          act("$n suddenly trips you with a rapid foot sweep.", ch,NULL,victim,TO_VICT );
          act("$n suddenly trips $N with a rapid foot sweep.", ch,NULL,victim,TO_NOTVICT );

          for ( count = 1; count < 5; count++ )
          {
             if ( victim->position == POS_DEAD )
                return;

             act("Your lightning kicks smashes into $N viciously.", ch,NULL,victim,TO_CHAR );
             act("$n's feet become blurry as they appear to strike you all over your face.", ch,NULL,victim,TO_VICT );
             act("$n's lightning kicks smashes into $N viciously.", ch,NULL,victim,TO_NOTVICT );
          }

          act("You are surrounded by a #cpalish blue#n outline.", ch,NULL,victim,TO_CHAR );
          act("$n is surrounded by a #cpalish blue#n outline.", ch,NULL,victim,TO_VICT );
          act("$n is surrounded by a #cpalish blue#n outline.", ch,NULL,victim,TO_NOTVICT );

          act("You yell '#yDolphin Kick#n' as you swiftly somersault kick $N high into the skies.", ch,NULL,victim,TO_CHAR );
          act("$n yells '#yDolphin Kick#n' as $e swiftly somersault kicks you high into the skies.", ch,NULL,victim,TO_VICT );
          act("$n yells '#yDolphin Kick#n' as $e swiftly somersault kicks $N high into the skies.", ch,NULL,victim,TO_NOTVICT );
          act("$N crashes onto the ground in pain.", ch,NULL,victim,TO_CHAR );
          act("you crash onto the ground unconscious.", ch,NULL,victim,TO_VICT );
          act("$N crashes onto the ground in pain.", ch,NULL,victim,TO_NOTVICT );

          ch->attack_type = ATTACK_PHYSICAL;
          ch->ele_attack = ATTACK_ICE;
          ch->attack_var = HIT;
          ch->attack_focus = UPPERCUT;
          SET_BIT( ch->arena, AFF2_COMBO );

          if ( IS_CLASS( ch, CLASS_MONK ) && IS_CLASS( ch, CLASS_SAMURAI ) && !IS_SET( ch->special, SPC_DOPPLE ) )
             hurt_person( ch, victim, 700000, TRUE );
             else hurt_person( ch, victim, 500000, TRUE );

          return;
      }

   act("You ram your knee in $N's guts.",ch,NULL,victim,TO_CHAR);
   act("$n rams $s knee in your guts.",ch,NULL,victim,TO_VICT);
   act("$n rams $s knee in $N's guts.",ch,NULL,victim,TO_NOTVICT);
}

void do_alertness( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
	    send_to_char( "Huh?\n\r", ch );
	    return;
       }

    act("You place your hand on the earth as you softly concentrate on your surroundings.",ch,NULL,NULL,TO_CHAR);
    act("$n places $s hand on the earth as $e softly concentrates on $s surroundings.",ch,NULL,NULL,TO_ROOM);

    sprintf( buf, "#eAuras near you (#w%s#e)#n:\n\r", ch->in_room->area->name );
    send_to_char( buf, ch );
    found = FALSE;
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	    if ( ( d->connected == CON_PLAYING
	    ||   d->connected == CON_EDITING)
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
          &&   !IS_IMMORTAL(victim)
          &&   victim != ch
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   victim->pcdata->chobj == NULL )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
    }

    if ( !found )
	 send_to_char( "You are the only being throughout the surrounding vicinity.\n\r", ch );
}

void do_ki_pinch( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) && ch->pIndexData->vnum != 559 ) 
     return;

  if ( !IS_NPC( ch ) )
     {
        if ( !IS_CLASS( ch, CLASS_SHADOW ) )
           {
              send_to_char("Huh?\n\r",ch);
              return;
           }

        if ( ch->gifts[AKI] < 8 )
           {
              send_to_char("You have to improve your Ki fighting style to the 10th Level.\n\r",ch);
              return;
           }

        if ( ch->pkill < 10 )
           {
              send_to_char("You must belief in Grahf more before you can do this technique.\n\r",ch);
              return;
           }

        if ( ch->gifts[CURRENT_METER] != AKI )
           {
              send_to_char("You aren't concentrating on the Ki fighting style.\n\r",ch);
              return;
           }

        if ( ch->ctimer[11] > 0 )
           {
              send_to_char("You are still recovering from the last one.\n\r",ch);
              return;
           }
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        send_to_char( "Silence who?\n", ch );
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

  ch->wait = 6;

  if ( !is_martial( victim ) )
     {
        if ( number_percent() > 85 && !IS_SET( victim->more, MORE_FROZEN ) )
           {
               SET_BIT( victim->more, MORE_FROZEN );
               victim->freeze_timer = 1;
           }
           else victim->wait = number_range( 12, 24 );
     }

  if ( IS_CLASS( ch, CLASS_SHADOW ) )
     {
        ch->ctimer[11] = 4;
        victim->mana = UMAX( 0, victim->mana - ( victim->max_mana * 0.25 ) );
     }

  act("You fade out of existence appearing behind $N's back!",ch,NULL,victim,TO_CHAR);
  act("$n fades out of existence appearing behind your back!",ch,NULL,victim,TO_VICT);
  act("$n fades out of existence appearing behind $N's back!",ch,NULL,victim,TO_NOTVICT);
  act("You mutter '#ySuis Jui...#n' as you softly pinch $N on $S neck.",ch,NULL,victim,TO_CHAR);
  act("$n mutters '#ySuis Jui...#n' as $e softly pinches you on your neck.",ch,NULL,victim,TO_VICT);
  act("$n mutters '#ySuis Jui...#n' as $e softly pinches $N on $S neck.",ch,NULL,victim,TO_NOTVICT);
  act("Your muscles tighten as your breathing become laboured.",victim,NULL,NULL,TO_CHAR);
  act("$n's muscles tighten as $s breathing becomes laboured.",victim,NULL,NULL,TO_ROOM);

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = HIT;
  ch->attack_focus = DIRECT;
  hurt_person( ch, victim, 750000, TRUE );
}

void do_nerval_touch( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg );

   if ( !IS_CLASS( ch, CLASS_NINJA ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->ninja_powers[NINJAKI] < 95 )
      {
         send_to_char("You don't have enough ninjitsu ki for this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Who to you wish to stun?\n\r",ch);
         return;
      }

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to execute this technique.\n\r",ch); 
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

   if ( victim->stunned > 0 )
      {
         stc("They are already stunned.\n\r",ch); 
         return;
      }

   if ( is_martial( victim ) )
      {
         stc("They easily evade your technique by sidestepping your attack.\n\r", ch );
         return;
      }

   ch->pcdata->ninja_powers[NINJAKI] -= 95;

   act("You yell '#yKinze!#n' as you stand erect, poking $N with your finger.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yKinze!#n' as $e stands erect, poking you with $s finger.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yKinze!#n' as $e stands erect, poking $N with $s finger.",ch,NULL,victim,TO_NOTVICT);
   act("You scream as crackling static electricity shocks you violently.",victim,NULL,NULL,TO_CHAR);
   act("$n screams as crackling static electricity shocks $m violently.",victim,NULL,NULL,TO_ROOM);
   act("You twiddle your fingers as you cross one arm into a straight line, knees bent.",ch,NULL,NULL,TO_CHAR);
   act("$n twiddles $s fingers as $e crosses one arm into a straight line, knees bent.",ch,NULL,NULL,TO_ROOM);
   act("You mutter '#yAHHHHhhhh aHHhhHh AhhHHhh...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yAHHHHhhhh aHHhhHh AhhHHhh...#n'",ch,NULL,NULL,TO_ROOM);
   act("#eThe surroundings goes blurry as multiple images shimmers around your body.#n",ch,NULL,NULL,TO_CHAR);
   act("#eThe surroundings goes blurry as multiple images shimmers around $n#e's body.#n",ch,NULL,NULL,TO_ROOM);
   act("#eAs the images go closer and closer together, a huge spiral of energy forms into existence.#n",ch,NULL,NULL,TO_CHAR);
   act("#eAs the images go closer and closer together, a huge spiral of energy forms into existence.#n",ch,NULL,NULL,TO_ROOM);
   act("You yell '#ySei!#n' as you shimmer through $N pinching $M all over.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#ySei!#n' as $e shimmers through you pinching you all over.",ch,NULL,victim,TO_VICT);
   act("$n yells '#ySei!#n' as $e shimmers through $N pinching $M all over.",ch,NULL,victim,TO_NOTVICT);
   victim->stunned = 1;
}

void do_hishoken_slam( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim = ch->fighting;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_NINJA ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         send_to_char("You hadn't unlocked the secret of the scrolls for this technique yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         send_to_char("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( victim == NULL )
      {
         send_to_char("You can only do this technique in a fight gaiden.\n\r",ch);
         return;
      }

   act("You quickly rush towards $N dodging all of $S attacks at you.",ch,NULL,victim,TO_CHAR);
   act("$n quickly rushes towards you dodging all of your attacks at $m.",ch,NULL,victim,TO_VICT);
   act("$n quickly rushes towards $N dodging all of $S attacks at $m.",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#ySush...#n' as you kick $N in the chest.",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#ySush...#n' as $e kicks you in the chest.",ch,NULL,victim,TO_VICT);
   act("$n mutters '#ySush...#n' as $e kicks $N in the chest.",ch,NULL,victim,TO_NOTVICT);
   act("As $N staggers back, you quickly grab $M and throw $M over your head.",ch,NULL,victim,TO_CHAR);
   act("As you stagger back, $e quickly grabs you and throws you over $s head.",ch,NULL,victim,TO_VICT);
   act("As $N staggers back, $e quickly grabs $M and throws $M over $s head.",ch,NULL,victim,TO_NOTVICT);
   act("You then strike $N at the throat viciously as you cut off $S air supply.",ch,NULL,victim,TO_CHAR);
   act("$n then strikes you at the throat viciously as $e cuts off your air supply.",ch,NULL,victim,TO_VICT);
   act("$n then strikes $N at the throat viciously as $e cuts off $S air supply.",ch,NULL,victim,TO_NOTVICT);

   ch->ctimer[4] = 6;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = LOW;
   hurt_person( ch, victim, 250000, TRUE );
 
   stop_fighting( ch, TRUE );

   if ( victim != NULL && victim->position > POS_STUNNED )
      victim->position = POS_STUNNED;
}

void do_caltrops( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int obj_num = 127;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS( ch, CLASS_NINJA ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char("You hadn't unlocked the secret of the scrolls for this technique yet.\n\r",ch);
          return;
       }

    if ( ch->ctimer[12] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( pObjIndex = get_obj_index( obj_num ) ) == NULL )
       {
	    send_to_char("Missing object, please inform Oni.\n\r",ch);
	    return;
       }

    obj = create_object( pObjIndex, 50 );
    obj_to_room( obj, ch->in_room );
    obj->timer = 8;
    ch->ctimer[12] = 6;

    act("You suddenly open your hands releasing a bunch of tiny needles into the air.",ch,NULL,NULL,TO_CHAR);
    act("$n suddenly opens $s hands releasing a bunch of tiny needles into the air.",ch,NULL,NULL,TO_ROOM);
    act("The needles bunch silently of the ground as you quickly glance around you.",ch,NULL,NULL,TO_CHAR);
    act("The needles bunch silently of the ground as $n quickly glances around $m.",ch,NULL,NULL,TO_ROOM);
}

void do_jakouha( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int cost = 4000000;

    argument = one_argument( argument, arg );
 
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char("You hadn't unlocked the secret of the scrolls for this technique yet.\n\r",ch);
          return;
       }

    if ( ch->position == POS_STANDING )
       {
          send_to_char( "But you aren't in a fight.\n\r", ch );
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to stun?\n\r",ch);
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

    if ( ch->move < cost )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( is_martial( victim ) || IS_CLASS( victim, CLASS_NINJA ) )
       {
          stc("They easily dodge your technique.\n\r",ch);
          return;
       }

    if ( is_safe(ch,victim) ) 
       return;

    act("You suddenly catch $N offguard with a side kick to the face.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly catches you offguard with a side kick to the face.",ch,NULL,victim,TO_VICT);
    act("$n suddenly catches $N offguard with a side kick to the face.",ch,NULL,victim,TO_NOTVICT);
    act("You then kneel down pushing your palm into $N's chin.",ch,NULL,victim,TO_CHAR);
    act("$n then kneels down pushing $s palm into your chin.",ch,NULL,victim,TO_VICT);
    act("$n then kneels down pushing $s palm into $N's chin.",ch,NULL,victim,TO_NOTVICT);
    act("You mutter '#ySora Den...#n' as you push your palms into $N's stomach staggering $M.",ch,NULL,victim,TO_CHAR);
    act("$n mutters '#ySora Den...#n' as $e pushes $s palms into your stomach staggering you.",ch,NULL,victim,TO_VICT);
    act("$n mutters '#ySora Den...#n' as $e pushes $s palms into $N's stomach staggering $M.",ch,NULL,victim,TO_NOTVICT);

    ch->move -= cost;
    victim->wait = number_range( 12, 18 );
}

void do_izunaken( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
	    send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->move < 500000 )
	 {
	    stc("You are too tired to do this!\n\r",ch);
	    return;
 	 }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
	    send_to_char( "Whom to you want to disarm?\n\r", ch );
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

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON )
       && ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL || obj->item_type != ITEM_WEAPON )
       && ( ( obj = get_eq_char( victim, WEAR_THIRD ) ) == NULL || obj->item_type != ITEM_WEAPON )
       && ( ( obj = get_eq_char( victim, WEAR_FOURTH ) ) == NULL || obj->item_type != ITEM_WEAPON ) ) 
          {
             send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
             return;
          }

    if ( is_safe(ch,victim) )
       return;

    ch->move -= 500000;

    if ( number_percent() < 50 || IS_CLASS(victim, CLASS_PALADIN) || IS_CLASS(victim, CLASS_ASSASSIN) )
       {
          act("$N easily dodges your technique.",ch,NULL,victim,TO_CHAR);
          ch->wait = 8;
          return;
       }

    act("You crouch on your knees as you SmAsH your fist into $N's stomach.",ch,NULL,victim,TO_CHAR);
    act("$n crouches on $s knees as $e SmAsHeS $s fist into your stomach.",ch,NULL,victim,TO_VICT);
    act("$n crouches on $s knees as $e SmAsHeS $s fist into $N's stomach.",ch,NULL,victim,TO_NOTVICT);
    act("Kicking $N in the face, you lean back grasping $S weapon firmly.",ch,NULL,victim,TO_CHAR);
    act("Kicking you in the face, $n leans back grasping your weapon firmly.",ch,NULL,victim,TO_VICT);
    act("Kicking $N in the face, $n leans back grasping $S weapon firmly.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly flip backwards onto your feet knocking $N's weapon aside.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly flips backwards onto $s feet knocking your weapon aside.",ch,NULL,victim,TO_VICT);
    act("$n suddenly flips backwards onto $s feet knocking $N's weapon aside.",ch,NULL,victim,TO_NOTVICT);

    obj_from_char( obj );
    obj_to_char( obj, victim );
    ch->wait = 8;

    ch->attack_type = ATTACK_PHYSICAL;
    hurt_person( ch, victim, 400000, TRUE );
}

void do_stardance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 562 )
       return;

    if ( !IS_NPC( ch ) )
       {
           if ( !IS_CLASS(ch, CLASS_NINJA) )
              {
                 send_to_char( "Huh?\n\r", ch );
                 return;
              }

           if ( get_stock( ch ) < 100 )
              {
                 stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                 return;
              }
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to go wild upon?\n\r",ch);
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

    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("You flick your hand revealing three shurikens as you throw another one at $N's chest.",ch,NULL,victim,TO_CHAR);
    act("$n flicks $s hand revealing three shurikens as $e throws another one at your chest.",ch,NULL,victim,TO_VICT);
    act("$n flicks $s hand revealing three shurikens as $e throws another one at $N's chest.",ch,NULL,victim,TO_NOTVICT);
    act("$N coughs out blood as you kneel down and release your three shurikens into $S stomach!",ch,NULL,victim,TO_CHAR);
    act("You cough out blood as $n kneels down and releases $s three shurikens into your stomach!",ch,NULL,victim,TO_VICT);
    act("$N coughs out blood as $n kneels down and releases $s three shurikens into $S stomach!",ch,NULL,victim,TO_NOTVICT);
    act("You slip another shuriken from nowhere as $N holds $S stomach in pain.",ch,NULL,victim,TO_CHAR);
    act("$n slips another shuriken from nowhere as you hold your stomach in pain.",ch,NULL,victim,TO_VICT);
    act("$n slips another shuriken from nowhere as $N holds $S stomach in pain.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's legs causing $M to scream in pain.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your leg causing you to scream loudly.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's legs $M to scream in pain.",ch,NULL,victim,TO_NOTVICT);
    act("You slip another shuriken from nowhere as $N holds $S stomach in pain.",ch,NULL,victim,TO_CHAR);
    act("$n slips another shuriken from nowhere as you hold your stomach in pain.",ch,NULL,victim,TO_VICT);
    act("$n slips another shuriken from nowhere as $N holds $S stomach in pain.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("You flick your hand revealing five shurikens as you throw another one at $N's chest.",ch,NULL,victim,TO_CHAR);
    act("$n flicks $s hand revealing five shurikens as $e throws another one at your chest.",ch,NULL,victim,TO_VICT);
    act("$n flicks $s hand revealing five shurikens as $e throws another one at $N's chest.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
    act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);

    if ( !IS_NPC( ch ) )
       {
          ch->wait = 12;
          subtract_stock( ch, 100 );
          SET_BIT( ch->arena, AFF2_DM );
       }

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = DIRECT;

    if ( !IS_NPC( ch ) )
       hurt_person( ch, victim, 1000000, TRUE );
       else hurt_person( ch, victim, 500000, TRUE );
}

void do_illusion( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    CHAR_DATA *opponent = NULL;
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) && ch->pIndexData->vnum != 562 )
       return;

    if ( !IS_NPC( ch ) )
       {
           if ( !IS_CLASS(ch, CLASS_NINJA) )
              {
                 send_to_char( "Huh?\n\r", ch );
                 return;
              }

           if ( ch->pkill < 20 )
              {
                 send_to_char("You hadn't unlocked the secret of the scrolls for this technique yet.\n\r",ch);
                 return;
              }

           if ( ch->ctimer[2] > 0 )
              {
                 stc("You are still recovering from the last one.\n\r",ch);
                 return;
              }

           if ( ch->pcdata->followers > 0 )
              {
	           send_to_char("You already made some type of dummy.\n\r",ch);
                 return;
              }

           ch->ctimer[2] = 8;
           ch->pcdata->followers++;
       }

    victim = create_mobile( get_mob_index( 1 ) );
    victim->sex = ch->sex;
    victim->level = 980;
    victim->armor = char_ac(ch);

    if ( IS_NPC( ch ) )
       {
          victim->hitroll = 90;
          victim->damroll = 90;
       }
       else {
               victim->hitroll = get_curr_dex( ch );
               victim->damroll = get_curr_str( ch );
            }

    victim->hit = victim->max_hit = ch->max_hit / 2;
    victim->mana = victim->max_mana = ch->max_mana;
    victim->move = victim->max_move = ch->max_move;

    victim->name = str_dup(ch->name);

    if ( !IS_NPC( ch ) )
       victim->short_descr = str_dup(ch->name);
       else victim->short_descr = str_dup(ch->short_descr);

    victim->practice = 2;
    SET_BIT(victim->act, ACT_TIMER);
    SET_BIT( victim->act, ACT_NINJA );
    sprintf( buf, "%s is hovering here.\n\r", ch->name );

    victim->long_descr = str_dup(buf);
    victim->itemaffect = ch->itemaffect;
    victim->affected_by = ch->affected_by;

    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );

    act( "You twiddle your fingers as you summon a perfect image of yourself.", ch, NULL, NULL, TO_CHAR );
    act( "$n twiddles $s fingers as $e suddenly splits into two images of $mself.", ch, NULL, NULL, TO_ROOM );

    if ( ch->fighting != NULL )
       {
           opponent = ch->fighting;

           act( "Your dummy attacks your foes as you circle behind them and initiate a counter attack.", ch, NULL, NULL, TO_CHAR );
           act( "Both images of $n leaps into action confusing everyone with their techniques.", ch, NULL, NULL, TO_ROOM );

           if ( IS_NPC( ch ) )
              SET_BIT( opponent->more2, MORE2_NOLEGS );

           stop_fighting( opponent, FALSE );
           stop_fighting( ch, FALSE );

           set_fighting( opponent, victim );
           set_fighting( victim, opponent );
           set_fighting( ch, opponent );

           multi_hit( ch, opponent, TYPE_UNDEFINED );
       }
}

void do_throw( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj = NULL;
  char arg[MIL];
  CHAR_DATA *victim;

  if ( IS_NPC(ch) ) return;

  argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( ch->gifts[AZI] < 2 )
       {
          send_to_char("You have to improve your Zi fighting style to the 2nd Level.\n\r",ch);
          return;
       }

    if ( ch->gifts[CURRENT_METER] != AZI )
       {
          send_to_char("You aren't concentrating on the Zi fighting style.\n\r",ch);
          return;
       }

  for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
  {
     if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) && obj->item_type == ITEM_WEAPON )
        break;
  }

  if ( obj == NULL )
     {
        if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) )
           if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) )
              {
	           send_to_char( "But you aren't holding anything in your hands.\n\r", ch );
	           return;
              }  
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
         stc("Whose do you wish to throw your knife shuriken at?\n\r",ch);
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

  act("You flick your weapon onto your finger as you throw it at $N viciously.",ch,NULL,victim,TO_CHAR);
  act("$n flicks $s weapon onto $s finger as $e throws it at you viciously.",ch,NULL,victim,TO_VICT);
  act("$n flicks $s weapon onto $s finger as $e throws it at $N viciously.",ch,NULL,victim,TO_NOTVICT);
  act("$n winces in pain as the weapon slices into $m.",victim,NULL,NULL,TO_ROOM);
  act("You wince in pain as the weapon slices into you.",victim,NULL,NULL,TO_CHAR);

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );

  ch->wait = 12;

  ch->attack_type = ATTACK_PHYSICAL;
  ch->attack_var = PIERCE;
  ch->attack_focus = DIRECT;
  hurt_person( ch, victim, 500000, TRUE );
}

void do_somer_strike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_SHADOW) )
       {
          send_to_char( "Huh?\n\r", ch );
          return;
       }

    if ( IS_CLASS(ch, CLASS_SHADOW) )
       {
          if ( get_stock( ch ) < 100 )
             {
                stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                return;
             }            
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

    if ( is_safe(ch,victim) ) 
       return;

    act("You grin evilly as you lower your head silently.",ch,NULL,NULL,TO_CHAR);
    act("$n grins evilly as $e lowers $s head silently.",ch,NULL,NULL,TO_ROOM);
    act("You suddenly leap high into the air above $N's head.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly leaps high into the air above your head.",ch,NULL,victim,TO_VICT);
    act("$n suddenly leaps high into the air above $N's head.",ch,NULL,victim,TO_NOTVICT);
    act("You yell #y'Nokizura Den!!#n' as you rip out your katana SlAsHiNg down $N's body!!!",ch,NULL,victim,TO_CHAR);
    act("$n yells #y'Nokizura Den!!#n' as $e rips out $s katana SlAsHiNg down your body!!!",ch,NULL,victim,TO_VICT);
    act("$n yells #y'Nokizura Den!!#n' as $e rips out $s katana SlAsHiNg down $N's body!!!",ch,NULL,victim,TO_NOTVICT);
    act("As your katana RiPs $N's body aparts, it suddenly implodes with unbelievable energy.",ch,NULL,victim,TO_CHAR);
    act("As $n's katana RiPs your body apart, it suddenly implodes with unbelievable energy.",ch,NULL,victim,TO_VICT);
    act("As $n's katana RiPs $N's body aparts, it suddenly implodes with unbelievable energy.",ch,NULL,victim,TO_NOTVICT);
 
    if ( !IS_SET( victim->more, MORE_BLIND ) )
       {
          SET_BIT( victim->more, MORE_BLIND );
          victim->blind_timer = 4;
       }

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = AERIAL;

    if ( IS_CLASS(ch, CLASS_SHADOW) )
       {
          ch->wait = 12;
          subtract_stock( ch, 100 );
          SET_BIT( ch->arena, AFF2_DM );
       }

    hurt_person( ch, victim, 1000000, TRUE );
}

void do_rising_tiger( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch) && ch->pIndexData->vnum != 562 )
     return;

  if ( !IS_NPC( ch ) )
     {
         if ( !IS_CLASS(ch, CLASS_NINJA) )
            {
               send_to_char( "Huh?\n\r", ch );
               return;
            }

         if ( get_stock( ch ) < 200 )
            {
               stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
               return;
            }
     }

  if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
     {
        stc("Who do you wish to go wild upon?\n\r",ch);
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

  act("You suddenly throw a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly throws a shuriken at your throat choking you.",ch,NULL,victim,TO_VICT);
  act("$n suddenly throws a shuriken at $N's throat choking $M.",ch,NULL,victim,TO_NOTVICT);
  act("You flick your hand revealing three shurikens as you throw another one at $N's chest.",ch,NULL,victim,TO_CHAR);
  act("$n flicks $s hand revealing three shurikens as $e throws another one at your chest.",ch,NULL,victim,TO_VICT);
  act("$n flicks $s hand revealing three shurikens as $e throws another one at $N's chest.",ch,NULL,victim,TO_NOTVICT);
  act("$N coughs out blood as you kneel down and release your three shurikens into $S stomach!",ch,NULL,victim,TO_CHAR);
  act("You cough out blood as $n kneels down and releases $s three shurikens into your stomach!",ch,NULL,victim,TO_VICT);
  act("$N coughs out blood as $n kneels down and releases $s three shurikens into $S stomach!",ch,NULL,victim,TO_NOTVICT);
  act("You twirl your foot around into $N's face.",ch,NULL,victim,TO_CHAR);
  act("$n twirls $s foot around into your face.",ch,NULL,victim,TO_VICT);
  act("$n twirls $s foot around into $N's face.",ch,NULL,victim,TO_NOTVICT);
  act("You suddenly catch $N offguard with a side kick to the face.",ch,NULL,victim,TO_CHAR);
  act("$n suddenly catches you offguard with a side kick to the face.",ch,NULL,victim,TO_VICT);
  act("$n suddenly catches $N offguard with a side kick to the face.",ch,NULL,victim,TO_NOTVICT);
  act("You quickly dash towards $N as you pull out a dagger and ThRuSt it through $S chest.",ch,NULL,victim,TO_CHAR);
  act("$n quickly dashes towards you as $e pulls out a dagger and ThRuStS it through your chest.",ch,NULL,victim,TO_VICT);
  act("$n quickly dashes towards $N as $e pulls out a dagger and ThRuStS it through $S chest.",ch,NULL,victim,TO_NOTVICT);
  act("You then unsheath your other dagger as you RiP it Up through $N's body.",ch,NULL,victim,TO_CHAR);
  act("$n then unsheath $s other dagger as $e RiPs it Up through your body.",ch,NULL,victim,TO_VICT);
  act("$n then unsheath $s other dagger as $e RiPs it Up through $N's body.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You ram your palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s palm into your stomach causing you to drop to the ground in pain.",ch,NULL,victim,TO_VICT);
  act("$n rams $s palm into $N's stomach causing $M to drop to the ground in pain.",ch,NULL,victim,TO_NOTVICT);
  act("You push your palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_CHAR);
  act("$n pushes $s palm up your chin making you stagger backwards.",ch,NULL,victim,TO_VICT);
  act("$n pushes $s palm up $N's chin making $M stagger backwards.",ch,NULL,victim,TO_NOTVICT);
  act("You yell '#yGermi Tu!#n' as you spin backwards giving $N a ViCiOuS flash kick.",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yGermi Tu!#n' as $e spins backwards giving you a ViCiOuS flash kick.",ch,NULL,victim,TO_VICT);
  act("$n yells '#yGermi Tu!#n' as $e spins backwards giving $N a ViCiOuS flash kick.",ch,NULL,victim,TO_NOTVICT);
  act("Landing on your feet, you mutter '#yAhhHhhhHhh...#n' as your hands cackle with lightning.",ch,NULL,NULL,TO_CHAR);
  act("Landing on $s feet, $n mutters '#yAhhHhhhHhh...#n' as $s hands cackle with lightning.",ch,NULL,NULL,TO_ROOM);
  act("You yell '#yRising Tiger!#n' as you SmAsH your fist upwards $N's body!",ch,NULL,victim,TO_CHAR);
  act("$n yells '#yRising Tiger!#n' as $e SmAsHeS $s fist upwards your body!",ch,NULL,victim,TO_VICT);
  act("$n yells '#yRising Tiger!#n' as $e SmAsHeS $s fist upwards $N's body!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("$N's body explodes with electrical pulses as your fist SmAsHeS $M into the air!",ch,NULL,victim,TO_CHAR);
  act("Your body explodes with electrical pulses as $n's fist SmAsHeS you into the air!",ch,NULL,victim,TO_VICT);
  act("$N's body explodes with electrical pulses as $n's fist SmAsHeS $M into the air!",ch,NULL,victim,TO_NOTVICT);
  act("You suddenly unsheath your katana and whirl it across $N's body!",ch,NULL,victim,TO_CHAR);
  act("$n suddenly unsheaths $s katana and whirls it across your body!",ch,NULL,victim,TO_VICT);
  act("$n suddenly unsheaths $s katana and whirls it across $N's body!",ch,NULL,victim,TO_NOTVICT);

  if ( !IS_NPC( ch ) )
     {
        ch->wait = 12;
        subtract_stock( ch, 200 );
        SET_BIT( ch->arena, AFF2_DM );
     }

  ch->attack_type = ATTACK_PHYSICAL;
  ch->ele_attack = ATTACK_THUNDER;
  ch->attack_var = HIT;
  ch->attack_focus = UPPERCUT;

  hurt_person( ch, victim, 2000000, TRUE );
}

void do_kakusu( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;
	
   if ( !IS_SET( ch->cmbt[7], SKL_KAKUSU ) && !IS_CLASS(ch, CLASS_NINJA) )
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
         send_to_char( "You appear from the shadows.\n\r", ch );
         act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM );
      }
      else {
              act( "$n disappears into the shadows.", ch, NULL, NULL, TO_ROOM );
              send_to_char( "You disappear into the shadows.\n\r", ch );
              SET_BIT(ch->act, PLR_WIZINVIS);
           }
}

void do_net( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
       {
	    send_to_char("Huh?\n\r",ch);
	    return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Who do you want to throw a net on?\n\r", ch );
          return;
       }
    
    if ( ch->position == POS_STANDING )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( ch == victim )
       {
	    send_to_char( "Dont be foolish.\n\r", ch );
	    return;
       }

    if ( is_safe(ch, victim) ) 
       return;

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
       {
	    send_to_char( "You cannot throw a net over an ethereal person.\n\r", ch );
	    return;
       }

    if ( IS_SET(victim->more, MORE_NET) )
       {
	    send_to_char( "The victim is already entrapped in a tangling net.\n\r", ch );
	    return;
       }

    WAIT_STATE( ch, 6 );

    if ( ( number_percent() < 50 && victim->position >= POS_FIGHTING ) || IS_CLASS(victim, CLASS_MONK) )
       {
          act("$N swiftly evades the net you try to cast over $M.",ch,NULL,victim,TO_CHAR);
          act("You swiftly evade the net $n tries to throw on you.",ch,NULL,victim,TO_VICT);
          act("$N swiftly evades the net $n tries to cast over $M.",ch,NULL,victim,TO_NOTVICT);
          return;
       }

    act("$N struggles weakly as you throw a net over $M.",ch,NULL,victim,TO_CHAR);
    act("$n throws a net on you.",ch,NULL,victim,TO_VICT);
    act("$n throws a net over $N.",ch,NULL,victim,TO_NOTVICT);

    SET_BIT(victim->more, MORE_NET);
}
