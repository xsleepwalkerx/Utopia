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

void do_sanren_geki( CHAR_DATA *ch, CHAR_DATA *victim )
{
   /*
   int bonus = UMIN( 200, ch->pkill * 10 ) + 100;
   int divider = 1;*/

   act("I don't know what the fuck Sanren Geki looks like but this is just a test...",ch,NULL,NULL,TO_CHAR);
   act("I don't know what the fuck Sanren Geki looks like but this is just a test...",ch,NULL,NULL,TO_ROOM);
   act("To see if it works lol.",ch,NULL,NULL,TO_CHAR);
   act("To see if it works lol.",ch,NULL,NULL,TO_ROOM);

  /* 
   if ( ch->pkill < 10 )
      divider = 3;
      else if ( ch->pkill < 20 )
              divider = 2;
      
   ch->ctimer[9] = UMIN( bonus, ch->ctimer[9] + ( number_range( 35, 40 ) / divider ) );
   victim->mana = UMAX( 0, victim->mana - 500000 );*/

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_FIRE;

   hurt_person(ch,victim,500000,TRUE);
}

bool is_kofkuruda_combo( CHAR_DATA *ch, CHAR_DATA *victim, int move_num )
{
   if ( move_num == 5 )
      {
         /* Sanren Geki      - HighKick, JumpKick, SnapKick, SpinKick, Sidewinder */
         if ( ch->clist[0] == 1 && ch->clist[1] == 2 && ch->clist[2] == 3
              && ch->clist[3] == 4 && ch->clist[4] == 5 )
            {
                do_sanren_geki(ch,victim);

                ch->special_id = 0;

                if ( ch->ninjastuff == 150 )
                   ch->wait = 12;
                   else ch->wait = 4;

                ch->ninjastuff = 150;

                return TRUE;
            }
   }
   return FALSE;
}

void do_highkick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   /*int dam;*/

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to high kick?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   ch->clist[ ch->special_id  ] = 1;
   ch->special_id++;

   if ( ch->special_id < 6 )
      {
         if ( is_kofkuruda_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   /*dam = UMIN( char_damroll(ch), 3000 );*/

   act("You extend your leg high into the air and WhIp $N with the FoRcE of your leg",ch,NULL,victim,TO_CHAR);
   act("$n extends $s leg high into the air and WhIpS $N with the FoRcE of $s leg",ch,NULL,victim,TO_NOTVICT);
   act("$n extends $s leg high into the air and WhIpS you with the FoRcE of $s leg",ch,NULL,victim,TO_VICT);
}

void do_jumpkick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   /*int dam;*/

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to jump kick?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   ch->clist[ ch->special_id  ] = 2;
   ch->special_id++;

   if ( ch->special_id < 6 )
      {
         if ( is_kofkuruda_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   /*dam = UMIN( char_damroll(ch), 3000 );*/

   act("You jump into the air and ViCiOuSlY whip your leg across $N's face.",ch,NULL,victim,TO_CHAR);
   act("$n jumps into the air and ViCiOuSlY whips $s leg across $N's face.",ch,NULL,victim,TO_NOTVICT);
   act("$n jumps into the air and ViCiOuSlY whips $s leg across your face.",ch,NULL,victim,TO_VICT);
}

void do_snapkick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   /*int dam;*/

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to snap kick?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   ch->clist[ ch->special_id  ] = 3;
   ch->special_id++;

   if ( ch->special_id < 6 )
      {
         if ( is_kofkuruda_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   /*dam = UMIN( char_damroll(ch), 3000 );*/

   act("You raise your leg high in the air with the speed of a panther, SmAsHiNg $N in the chin",ch,NULL,victim,TO_CHAR);
   act("$n raises $s leg high in the air with the speed of a panther, SmAsHiNg $N in the chin",ch,NULL,victim,TO_NOTVICT);
   act("$n raises $s leg high in the air with the speed of a panther, SmAsHiNg you in the chin",ch,NULL,victim,TO_VICT);
}

void do_kuruda_spinkick( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   /*int dam;*/

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to spin kick?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   ch->clist[ ch->special_id  ] = 4;
   ch->special_id++;

   if ( ch->special_id < 6 )
      {
         if ( is_kofkuruda_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   /*dam = UMIN( char_damroll(ch), 3000 );*/

   act("You extend your leg and spin fast landing an ExCrUcIaTiNg blow in $N's stomach",ch,NULL,victim,TO_CHAR);
   act("$n extends $s leg and spins fast landing an ExCrUcIaTiNg blow in $N's stomach",ch,NULL,victim,TO_NOTVICT);
   act("$n extends $s leg and spins fast landing an ExCrUcIaTiNg blow in your stomach",ch,NULL,victim,TO_VICT);
}

void do_kuruda_sidewinder( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   /*int dam;*/

   if ( !IS_CLASS(ch, CLASS_ASSASSIN) || !IS_SET( ch->special, SPC_MONK ) )
      {
	   stc("Huh?\n\r",ch);
	   return;
      }

   if ( ( get_eq_char( ch, WEAR_WIELD ) != NULL ) || ( get_eq_char( ch, WEAR_HOLD ) != NULL ) )
      {
         stc("You need free hands to perform this technique.\n\r",ch); 
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to use sidewinder on?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   ch->clist[ ch->special_id  ] = 5;
   ch->special_id++;

   if ( ch->special_id < 6 )
      {
         if ( is_kofkuruda_combo( ch, victim, ch->special_id ) )
            return;
      }
      else {
              ch->special_id = 0;
              ch->wait = 12;
              act("You gasp for breath as you pause in the midst of your attacks.",ch,NULL,NULL,TO_CHAR);
              act("$n gasps for breath as $e pauses in the midst of $s attacks.",ch,NULL,NULL,TO_ROOM);
              return;
           }

   /*dam = UMIN( char_damroll(ch), 3000 );*/

   act("You deliver a series of BlOwS with your legs, paining $N greatly.",ch,NULL,victim,TO_CHAR);
   act("$n delivers a series of BlOwS with $s legs, paining $N greatly.",ch,NULL,victim,TO_NOTVICT);
   act("$n delivers a series of BlOwS with $s legs, paining you greatly.",ch,NULL,victim,TO_VICT);
}
