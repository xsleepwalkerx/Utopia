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

void do_transfuse( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int option = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_DROW) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != 0 )
      {
         stc("you have already chosen a specialization.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( !str_cmp("clerical",arg) )
      option = DROW_CLERICAL;
      else if ( !str_cmp("militia",arg) )
              option = DROW_MILITIA;
      else if ( !str_cmp("magi",arg) )
              option = DROW_MAGI;
              else {
                      stc( "#eSyntax - #LTransfuse #wClerical#L/#PMagi#L/#eMilitia#n\n\r", ch );
                      return;
                   }

   stc( "#eYou #whave #echosen a #wspecialization#R. May your #Lvictims #Rdie #eeven more beautiful.#n\n\r", ch );
   ch->pcdata->powers[DROW_CLASS] = option;
}

void do_bless( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to bless?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 6;

   if ( ( sn = skill_lookup( "blessed soul" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already blessed.\n\r",ch);
         return;
      }

   spell_drow_bless( sn, 60, ch, victim );
}

void do_drow_blast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to deplete?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 12;

   act("You push your hand directly infront of $N's face.",ch,NULL,victim,TO_CHAR);
   act("$n pushes $s hand directly infront of $N's face.",ch,NULL,victim,TO_NOTVICT);
   act("$n pushes $s hand directly infront of your face.",ch,NULL,victim,TO_VICT);
   act("#0Feeling the force of the attack you fly backwards!!",victim,NULL,NULL,TO_CHAR);
   act("#0Feeling the force of the attack $n flies backwards!!",victim,NULL,NULL,TO_ROOM);      

   ch->attack_var = BLAST;
   ch->attack_focus = DIRECT;
   ch->attack_type = ATTACK_MAGICAL;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_lolth_wrath( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 3000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to feel the wrath?\n\r", ch );
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

   ch->mana -= 3000000;
   ch->wait = 6;

   if ( ( sn = skill_lookup( "vision destroyer" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already weakened.\n\r",ch);
         return;
      }

   spell_lwrath( sn, 60, ch, victim );
}

void do_aid( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 4 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to aid?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 6;

   if ( ( sn = skill_lookup( "aided soul" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already aided.\n\r",ch);
         return;
      }

   spell_aid( sn, 60, ch, victim );
}

void do_flame_blade( CHAR_DATA *ch, char *argument )
{
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( sn = skill_lookup( "flame blade" ) ) < 0 )
      return;

   if ( is_affected( ch, sn ) )
      {
         stc("A mystical blade of flames is already encircling you.\n\r",ch);
         return;
      }

   ch->mana -= 250000;
   ch->wait = 6;

   spell_fblade( sn, 60, ch, ch );
}

void do_hold_person( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[1] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->move < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you wish to hold?\n\r", ch );
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

   ch->move -= 250000;
   ch->ctimer[1] = 4;

   act("You hold $N within your grasp as you relentlessly attack them!",ch,NULL,victim,TO_CHAR);
   act("$n holds $N within $s grasp as $e relentlessly attacks $M!",ch,NULL,victim,TO_NOTVICT);
   act("$n holds you within $s grasp as $e relentlessly attacks you!",ch,NULL,victim,TO_VICT);
   act("$N welps as $e feels $s bones being CrUsHeD in your #revil#n grasp!",ch,NULL,victim,TO_CHAR);
   act("You welp as you feel your bones being CrUsHeD in the #0dark#n cleric's #revil#n grasp!",ch,NULL,victim,TO_VICT);
   act("$N welps as $E feels $S bones being CrUsHeD in the #0dark#n cleric's #revil#n grasp!",ch,NULL,victim,TO_NOTVICT);

   if ( victim->stunned == 0 && !is_martial( victim ) )
      victim->stunned = 1;
}

void do_poison_drop( CHAR_DATA *ch )
{
   if ( is_affected( ch, skill_lookup( "poison" ) ) )
      affect_strip( ch, skill_lookup( "poison" ) );

   if ( is_affected( ch, skill_lookup( "cloud kill" ) ) )
      affect_strip( ch, skill_lookup( "cloud kill" ) );

   if ( is_affected( ch, skill_lookup( "drow poison" ) ) )
      affect_strip( ch, skill_lookup( "drow poison" ) );

   if ( IS_SET( ch->more, MORE_POISON ) )
      REMOVE_BIT( ch->more, MORE_POISON );

   if ( IS_SET( ch->monkstuff, MONK_DEATH ) )
      REMOVE_BIT( ch->monkstuff, MONK_DEATH );

   if ( IS_SET( ch->more, MORE_BLOOD ) )
      REMOVE_BIT( ch->more, MORE_BLOOD );
}

void do_neutralize( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 12 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to neutralize?\n\r", ch );
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
   
   ch->mana -= 100000;
   ch->wait = 4;

   act("You place your hands on $N as warmth seeps into their veins.",ch,NULL,victim,TO_CHAR);
   act("$n places $s hands on $N as warmth seeps into their veins.",ch,NULL,victim,TO_NOTVICT);
   act("$n places $s hands on you as warmth seeps into your veins.",ch,NULL,victim,TO_VICT);
   act("You gasp in horror as #Ggreen#n slime oozes slowly from your #rcrimson #Rveins#n.",victim,NULL,NULL,TO_CHAR);
   act("$n gasps in horror as #Ggreen#n slime oozes slowly from $s #rcrimson #Rveins#n.",victim,NULL,NULL,TO_ROOM);
   do_poison_drop( victim );
}

void do_lightning_blast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to strike with a bolt?\n\r", ch );
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

   ch->mana -= 50000;
   ch->wait = 18;

   act("You raise your hands high into the sky as the outside of them glow #Lblue#n.",ch,NULL,NULL,TO_CHAR);
   act("$n raises $s hands high into the sky as the outside of them glow #Lblue#n.",ch,NULL,NULL,TO_ROOM);
   act("You gaze softly into the sky as the #Csky blue#n now becomes #0overcast#n.",ch,NULL,NULL,TO_CHAR);
   act("$n gazes softly into the sky as the #Csky blue#n now becomes #0overcast#n.",ch,NULL,NULL,TO_ROOM);
   act("You point your #Lblue#n hand at $N as a massive #zlightning#n #Cbolt#n FaLlS on them!",ch,NULL,victim,TO_CHAR);
   act("$n points $s #Lblue#n hand at $N as a massive #zlightning#n #Cbolt#n FaLlS on them!",ch,NULL,victim,TO_NOTVICT);
   act("$n points $s #Lblue#n hand at you as a massive #zlightning#n #Cbolt#n FaLlS on you!",ch,NULL,victim,TO_VICT);
   act("You shiver in total shock as the bolt sizzles you!",victim,NULL,NULL,TO_CHAR);
   act("$n shivers in total shock as the bolt sizzles $m",victim,NULL,NULL,TO_ROOM);

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_MAGICAL;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_lolth_prayer( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to pray for?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 6;

   if ( ( sn = skill_lookup( "prayer" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person already has your prayers.\n\r",ch);
         return;
      }

   spell_prayer( sn, 60, ch, victim );
}

void do_unparalyse( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you wish to free?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 3;

   act("You release the bitter grasp that surrounds $N.",ch,NULL,victim,TO_CHAR);
   act("$n releases the bitter grasp that surrounds $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n releases the bitter grasp that surrounds you.",ch,NULL,victim,TO_VICT);
   victim->stunned = 0;
   victim->freeze_timer = 0;

   if ( IS_SET( victim->more, MORE_FROZEN ) )
      REMOVE_BIT( victim->more, MORE_FROZEN );
}

void do_shammer( CHAR_DATA *ch, char *argument )
{
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( sn = skill_lookup( "soul hammer" ) ) < 0 )
      return;

   if ( is_affected( ch, sn ) )
      {
         stc("The shammering hammer encircles your soul cleric.\n\r",ch);
         return;
      }

   ch->mana -= 100000;
   ch->wait = 8;

   spell_shammer( sn, 60, ch, ch );
}

void do_shame( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 100000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to hex?\n\r", ch );
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

   if ( IS_SET( victim->more, MORE_HEX ) )
      {
         stc( "They are already weighed down with their unbearing shame.\n\r", ch );
         return;
      }

   ch->mana -= 100000;
   ch->wait = 6;
   SET_BIT( victim->more, MORE_HEX );

   act("You place your hands on $N's head and whisper '#yFeel the shame for not believing in the great #0Lolth!#n'",ch,NULL,victim,TO_CHAR);
   act("$n places $s hands on $N's head and whispers '#yFeel the shame for not believing in the great #0Lolth!#n'",ch,NULL,victim,TO_NOTVICT);
   act("$n places $s hands on your head and whispers '#yFeel the shame for not believing in the great #0Lolth!#n'",ch,NULL,victim,TO_VICT);
   act("You scream in unbelievable pain as #0Lolth#n has no #rmercy#n on your soul!",victim,NULL,NULL,TO_CHAR);
   act("$n screams in unbelievable pain as #0Lolth#n has no #rmercy#n on $s soul!",victim,NULL,NULL,TO_ROOM);
   act("You scream in unbelievable pain as #0Lolth#n has no #rmercy#n on your soul!",victim,NULL,NULL,TO_CHAR);
   act("$n screams in unbelievable pain as #0Lolth#n has no #rmercy#n on $s soul!",victim,NULL,NULL,TO_ROOM);
   act("You stare down at the ground with #Rcrimson#n cheeks, stabbing the floor with your big toe.",victim,NULL,NULL,TO_CHAR);
   act("$n stares down at the ground with #Rcrimson#n cheeks, stabbing the floor with $s big toe.",victim,NULL,NULL,TO_ROOM); 
 
   ch->attack_type = ATTACK_MAGICAL;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_cure( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int bonus = 300000;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 750000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      victim = ch;
      else if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
      }

   if ( !can_heal( ch, victim ) )
      return;

   if ( ch->pcdata->personality == PER_COMPASSION )
      bonus = 500000;
   
   ch->wait = 12;
   ch->mana -= 750000;

   act("You place your hands upon $N's head.",ch,NULL,victim,TO_CHAR);
   act("$n places $s hands upon $N's head.",ch,NULL,victim,TO_NOTVICT);
   act("$n places $s hands upon your head.",ch,NULL,victim,TO_VICT);
   act("You feel warmth spread in your body as all wounds are healed.",victim,NULL,NULL,TO_CHAR);
   act("$n feels warmth spread in $s body as all wounds are healed.",victim,NULL,NULL,TO_ROOM);
   act("You tingle as the warmth heads into the tips of your fingers and toes.",victim,NULL,NULL,TO_CHAR);
   act("$n tingles as the warmth heads into the tips of $s fingers and toes.",victim,NULL,NULL,TO_ROOM);
   act("You sigh as your body feels rejuvenated.",victim,NULL,NULL,TO_CHAR);
   act("$n sighs as $s body feels rejuvenated.",victim,NULL,NULL,TO_ROOM);
   hit_gain( victim, bonus );
}

void do_poison( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 4 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to seep the poison into?\n\r", ch );
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

   if ( is_affected( victim, skill_lookup("drow poison") ) )
      {
         stc( "They are already poisoned.\n\r", ch );
         return;
      }

   if ( ( IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_DROWPOISON) )
        || IS_CLASS(victim,CLASS_DRAGON) )
      {
         send_to_char( "They are immuned to poison.\n\r", ch );
         return;
      }
   
   ch->mana -= 250000;
   ch->wait = 12;

   act("You raise your hands to your mouth and like a #rfire eater#n breathe #ggreen#n gooe all over $N.",ch,NULL,victim,TO_CHAR);
   act("$n raises $s hands to $s mouth and like a #rfire eater#n breathes #ggreen#n gooe all over $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n raises $s hands to $s mouth and like a #rfire eater#n breathes #ggreen#n gooe all over you.",ch,NULL,victim,TO_VICT);
   act("You gag and gasp for breath as the #gpoison#n overcomes you!",victim,NULL,NULL,TO_CHAR);
   act("$n gags and gasps for breath as the #gpoison#n overcomes $m!",victim,NULL,NULL,TO_ROOM);

   spell_dpoison( skill_lookup( "drow poison" ), 60, ch, victim );
}

void do_cura( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      victim = ch;
      else if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
              return;
           }
      }

   if ( !can_heal( ch, victim ) )
      return;
   
   ch->wait = 4;
   subtract_stock( ch, 200 );

   act("You place your hands upon $N's head.",ch,NULL,victim,TO_CHAR);
   act("$n places $s hands upon $N's head.",ch,NULL,victim,TO_NOTVICT);
   act("$n places $s hands upon your head.",ch,NULL,victim,TO_VICT);
   act("You feel warmth spread in your body as all wounds are healed.",victim,NULL,NULL,TO_CHAR);
   act("$n feels warmth spread in $s body as all wounds are healed.",victim,NULL,NULL,TO_ROOM);
   act("You tingle as the warmth heads into the tips of your fingers and toes.",victim,NULL,NULL,TO_CHAR);
   act("$n tingles as the warmth heads into the tips of $s fingers and toes.",victim,NULL,NULL,TO_ROOM);
   act("You sigh as your body feels rejuvenated.",victim,NULL,NULL,TO_CHAR);
   act("$n sighs as $s body feels rejuvenated.",victim,NULL,NULL,TO_ROOM);
   act("A happy #Ppurple#n aura surrounds you as you continue to be bathed in precious #ylight#n.",victim,NULL,NULL,TO_CHAR);
   act("A happy #Ppurple#n aura surrounds $n as $e continues to be bathed in precious #ylight#n.",victim,NULL,NULL,TO_ROOM);
   act("You stand on your feet fully cured of all ailments.",victim,NULL,NULL,TO_CHAR);
   act("$n stands on $s feet fully cured of all ailments.",victim,NULL,NULL,TO_ROOM);

   if ( ch != victim )
      victim->agg /= 2;

   clear_affects( victim );
   hit_gain( victim, 2000000 );
}

void do_rigid_thinking( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 300000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to strike with a bolt?\n\r", ch );
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

   if ( number_percent() > 50 && !IS_SET( victim->more, MORE_CONFUSION ) )
      SET_BIT( victim->more, MORE_CONFUSION );

   ch->mana -= 300000;
   ch->wait = 18;

   act("You powerfully strike $N with a magical beam of #zlightning!#n",ch,NULL,victim,TO_CHAR);
   act("$n powerfully strikes $N with a magical beam of #zlightning!#n",ch,NULL,victim,TO_NOTVICT);
   act("$n powerfully strikes you with a magical beam of #zlightning!#n",ch,NULL,victim,TO_VICT);
   act("You walk around dazed, not knowing what your doing, not able to hit a thing!",victim,NULL,NULL,TO_CHAR);
   act("$n walks around dazed, not knowing what $e is doing, not able to hit a thing!",victim,NULL,NULL,TO_ROOM);   
 
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_THUNDER;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_flame_strike( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int sn;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( sn = skill_lookup( "flame blade" ) ) < 0 )
      return;

   if ( !is_affected( ch, sn ) )
      {
         stc("You need a magical flame blade to cast this spell.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to strike with a flame?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You charge your #Rflame#n blade up as it glows #rcrimson#n.",ch,NULL,NULL,TO_CHAR);
   act("$n charges $s #Rflame#n blade up as it glows #rcrimson#n.",ch,NULL,NULL,TO_ROOM);
   act("You twirl around three times before unleashing the blades power upon $N!",ch,NULL,victim,TO_CHAR);
   act("$n twirls around three times before unleashing the blades power upon $N!",ch,NULL,victim,TO_NOTVICT);
   act("$n twirls around three times before unleashing the blades power upon you!",ch,NULL,victim,TO_VICT);
   act("You stagger backwards as #Rburning #rflames#n overcome you.",victim,NULL,NULL,TO_CHAR);
   act("$n staggers backwards as #Rburning #rflames#n overcome $m.",victim,NULL,NULL,TO_ROOM);

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = OVERHEAD;
   ch->attack_var = SLASH;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_fireshield( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to give fire protection?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 8;

   if ( ( sn = skill_lookup( "fire shield" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person already has the fire shield encircling them.\n\r",ch);
         return;
      }

   spell_fshield( sn, 60, ch, victim );
}

void do_shillelagh( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to give Shillelagh's power to?\n\r", ch );
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
   
   ch->mana -= 250000;
   ch->wait = 8;

   if ( ( sn = skill_lookup( "shillelagh" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person already has Shillelagh's power.\n\r",ch);
         return;
      }

   spell_shille( sn, 60, ch, victim );
}

void do_raise_dead( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[12] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 7500000 )
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

   if ( victim->position > POS_INCAP )
      {
         stc( "You can only revive morted players with your unholy magic.\n\r", ch );
         return;
      }

   if ( !can_heal( ch, victim ) )
      return;

   ch->mana -= 7500000;
   ch->wait = 12;
   ch->ctimer[12] = 6;

   act("You conjure up a magical concoction and lay it in a circle of #0Lolth#n.",ch,NULL,NULL,TO_CHAR);
   act("$n conjures up a magical concoction and lays it in a circle of #0Lolth#n.",ch,NULL,NULL,TO_ROOM);
   act("You stare intently your eyes glowing #rcrimson#n as #Ppurple#n #ysparks#n surround the room.",ch,NULL,NULL,TO_CHAR);
   act("$n stares intently $s eyes glowing #rcrimson#n as #Ppurple#n #ysparks#n surround the room.",ch,NULL,NULL,TO_ROOM);
   act("You grin as a #0dark#n portal of #0C#Ph#0A#Po#0S#n tears a hole into this realm.",ch,NULL,NULL,TO_CHAR);
   act("$n grins as a #0dark#n portal of #0C#Ph#0A#Po#0S#n tears a hole into this realm.",ch,NULL,NULL,TO_ROOM);
   act("You bow infront of the '#0Dark Being#n' that enters this realm and without looking point at the beloved $N.",ch,NULL,victim,TO_CHAR);
   act("$n bows infront of the '#0Dark Being#n' that enters this realm and without looking points at the beloved $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n bows infront of the '#0Dark Being#n' that enters this realm and without looking points at the beloved you.",ch,NULL,victim,TO_VICT);
   act("Raising yourself back on your feet, you watch as the '#0Dark Being#n' swaggers over to $N.",ch,NULL,victim,TO_CHAR);
   act("Going back on to $s feet, $n watches as the '#0Dark Being#n' swaggers over to $N.",ch,NULL,victim,TO_NOTVICT);
   act("Going back on to $s feet, $n watches as the '#0Dark Being#n' swaggers over to you.",ch,NULL,victim,TO_VICT);

   give_align( ch, 100 );

   victim->position = POS_STANDING;
   victim->hit = victim->max_hit / 2;
   clear_affects( victim );
   victim->fight_timer = 0;

   act("The #0dark#n mysterious yet unwelcoming being places a stony hand on $N's shoulder.",ch,NULL,victim,TO_CHAR);
   act("The #0dark#n mysterious yet unwelcoming being places a stony hand on $N's shoulder.",ch,NULL,victim,TO_NOTVICT);
   act("The #0dark#n mysterious yet unwelcoming being places a stony hand on your shoulder.",ch,NULL,victim,TO_VICT);
   act("After being dazed for such a long time your eyes almost drop out your skull as you stare in amazement at the '#0Dark Being#n'.",victim,NULL,NULL,TO_CHAR);
   act("After being dazed for such a long time $n's eyes almost drop out $s skull as $e stares in amazement at the '#0Dark Being#n'.",victim,NULL,NULL,TO_ROOM);
   act("Watching all this going on you give a wry smile, another good job done for a Cleric as the '#0Dark Being#n' disappears.. in a #zflash#n.",ch,NULL,NULL,TO_CHAR);
   act("Watching all this going on $n gives a wry smile, another good job done for a Cleric as the '#0Dark Being#n' disappears.. in a #zflash#n.",ch,NULL,NULL,TO_ROOM);
}

void do_lolth_strength( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to bless with lolth's power?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 8;

   if ( ( sn = skill_lookup( "lolth strength" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already has Lolth's Strength.\n\r",ch);
         return;
      }

   spell_lolthst( sn, 60, ch, victim );
}

void do_lolth_shield( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to empower with Lolth's Shield?\n\r", ch );
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
   
   if ( ( sn = skill_lookup( "lolth shield" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already receiving Lolth's Shield.\n\r",ch);
         return;
      }

   ch->mana -= 50000;
   ch->wait = 4;

   spell_lshield( sn, 60, ch, victim );
}

void do_blur( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to blur?\n\r", ch );
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
   
   if ( ( sn = skill_lookup( "blur" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already blurred within time.\n\r",ch);
         return;
      }

   ch->mana -= 250000;
   ch->wait = 4;

   spell_blur ( sn, 60, ch, victim );
}

void do_horror( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 4 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[10] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to horrify?\n\r", ch );
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

   ch->ctimer[10] = 4;
   ch->move -= 1500000;
   ch->wait = 6;

   act("You spin around and arrive behind $N.",ch,NULL,victim,TO_CHAR);
   act("$n spins around and arrives behind $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n spins around and arrives behind you.",ch,NULL,victim,TO_VICT);
   act("Pulling $N's head backwards, you whisper '#yhorror...#n'",ch,NULL,victim,TO_CHAR);
   act("Pulling $N's head backwards, $n whispers '#yhorror...#n'",ch,NULL,victim,TO_NOTVICT);
   act("Pulling your head backwards, $n whispers '#yhorror...#n'",ch,NULL,victim,TO_VICT);

   do_flee( victim, "" );
}

void do_acid_arrow( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to strike with a acid missile?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 12;

   act("Swirling your hands in a mystical circle you create a #ggreen#n gooey substance.",ch,NULL,NULL,TO_CHAR);
   act("Swirling $s hands in a mystical circle $n creates a #ggreen#n gooey substance.",ch,NULL,NULL,TO_ROOM);
   act("Getting it into a perfect ball you ThRaSh it at $N!",ch,NULL,victim,TO_CHAR);
   act("Getting it into a perfect ball $n ThRaShEs it at $N!",ch,NULL,victim,TO_NOTVICT);
   act("Getting it into a perfect ball $n ThRaShEs it at you!",ch,NULL,victim,TO_VICT);
   act("#0You scream as the #gacid#0 eats away at your face!#n",victim,NULL,NULL,TO_CHAR);
   act("#0$n screams as the #gacid#0 eats away at $s face!#n",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 50 && !IS_SET( victim->more, MORE_BLIND ) )
      {
         victim->blind_timer = 5;
         SET_BIT( victim->more, MORE_BLIND );
      }

   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = HIT;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_vampiric_touch( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to deliver the touch of the vampires to?\n\r", ch );
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

   if ( ( sn = skill_lookup( "vampires touch" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already affected by vampiric touches.\n\r",ch);
         return;
      }

   ch->mana -= 1000000;
   ch->wait = 3;

   spell_vtouch( sn, 60, ch, victim );
}

void do_ice_storm( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 12 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to summon a icestorm upon?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 12;

   act("You guide your hands through the air awaiting for something #Cspecial#n.",ch,NULL,NULL,TO_CHAR);
   act("$n guides $s hands through the air awaiting for something #Cspecial#n.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly you witness the whole sky fill with clouds of sky #Cblue#n.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly $n witnesses as $s spell fills the whole sky with clouds of sky #Cblue#n.",ch,NULL,NULL,TO_ROOM);
   act("As cool as ice you whisper '#CIce storm...#n'",ch,NULL,NULL,TO_CHAR);
   act("As cool as ice $n whispers '#CIce storm...#n'",ch,NULL,NULL,TO_ROOM);
   act("#CAs giant shards of ice fall from the sky you can't avoid but be PiErCeD!",victim,NULL,NULL,TO_CHAR);
   act("#CAs giant shards of ice fall from the sky $n can't avoid but be PiErCeD!",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 60 && !is_affected( ch, skill_lookup( "slow" ) ) )
      spell_slow( skill_lookup( "slow" ), 3, ch, victim );

   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_confusion( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to confuse?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You conjure up a #Ppowerful#n #0spell#n and make $N believe the world is spinning endlessly!",ch,NULL,victim,TO_CHAR);
   act("$n conjures up a #Ppowerful#n #0spell#n and makes $N believe the world is spinning endlessly!",ch,NULL,victim,TO_NOTVICT);
   act("$n conjures up a #Ppowerful#n #0spell#n and makes you believe the world is spinning endlessly!",ch,NULL,victim,TO_VICT);
   act("You feel so dizzy as you go around hitting #0nothing...#n",victim,NULL,NULL,TO_CHAR);
   act("$n feels so dizzy as $e goes around hitting #0nothing...#n",victim,NULL,NULL,TO_ROOM);

   SET_BIT( victim->more, MORE_CONFUSION );

   if ( number_percent() > 74 && ( victim->in_room->vnum < 33500 || victim->in_room->vnum > 33800 ) )
      SET_BIT( victim->more3, MORE3_CONFUSE );

   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = SOUL;
   hurt_person( ch, victim, 2000000, TRUE );
}

void do_invunerability( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to give invunerability to?\n\r", ch );
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
   
   if ( ( sn = skill_lookup( "invunerability" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already invunerable to certain spells.\n\r",ch);
         return;
      }

   ch->mana -= 1000000;
   ch->wait = 18;

   spell_invun( sn, 60, ch, victim );
}

void do_lolth_charm( CHAR_DATA *ch, char *argument )
{
   int sn;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to charm with Lolth?\n\r", ch );
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

   if ( ( sn = skill_lookup( "lolth charm" ) ) < 0 )
      return;

   if ( is_affected( victim, sn ) )
      {
         stc("This person is already charmed by Lolth.\n\r",ch);
         return;
      }

   ch->mana -= 250000;
   ch->wait = 4;

   spell_lcharm( sn, 60, ch, victim );
}

void do_cloudkill( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int dam;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_CLERICAL && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Clerical House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to poison with a cloudkill?\n\r", ch );
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

   if ( ( IS_CLASS(victim, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
        || IS_CLASS(victim,CLASS_DRAGON) )
      {
         send_to_char( "They are immuned to poison.\n\r", ch );
         return;
      }
   
   act("You turn the clouds a bright #Cblue#n.",ch,NULL,NULL,TO_CHAR);
   act("$n turns the clouds a bright #Cblue#n.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly you throw your hands wildly in the air as #ggreen#n #Cfluid#n falls from the sky.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly $n throws $s hands wildly in the air as #ggreen#n #Cfluid#n falls from the sky.",ch,NULL,NULL,TO_ROOM);
   act("You scream wildly as the #ppoison#n seeps through your skin.",victim,NULL,NULL,TO_CHAR);
   act("$n screams wildly as the #ppoison#n seeps through $s skin.",victim,NULL,NULL,TO_ROOM);

   spell_ckill( skill_lookup( "cloud kill" ), 60, ch, victim );
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 300 );

   if ( !IS_CLASS( victim, CLASS_DUERGAR ) )
      {
         ch->attack_type = ATTACK_MAGICAL;
         ch->attack_focus = AERIAL;
         ch->attack_var = HIT;
         hurt_person( ch, victim, 3000000, TRUE );   
      }
      else victim->hit = UMIN( victim->max_hit, victim->hit + ( 3000000 * UMIN( 1, victim->pkill / 10 ) ) );
}

void do_shadow_door( CHAR_DATA *ch, char *argument )
{
   ROOM_INDEX_DATA *was_in;
   ROOM_INDEX_DATA *now_in;
   EXIT_DATA *pexit;
   int door;
   CHAR_DATA *victim;
   int attempt;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->ctimer[12] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc( "You don't have enough magical energy for this technique.\n\r", ch );
         return;
      }

   if ( ( victim = ch->fighting ) == NULL )
      {
         send_to_char( "You aren't fighting anyone.\n\r", ch );
         return;
      }

   was_in = ch->in_room;

   send_to_char("#eYou #Lopen #ea gate and #wstep #ethrough a #Pdimensional #Ldoor#e.#n\n\r", ch );

    for ( attempt = 0; attempt < 6; attempt++ ) 
    {
      door = number_door( );

      if ( ( pexit = was_in->exit[door] ) == 0
           ||   pexit->to_room == NULL
           ||   IS_SET(pexit->exit_info, EX_CLOSED)
           || ( IS_NPC(ch) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
         continue;

      move_char( ch, door );

      if ( ( now_in = ch->in_room ) == was_in )
         continue;

      if ( IS_SET(ch->newbits, NEW_DARKNESS) )
	   {
		REMOVE_BIT(ch->newbits, NEW_DARKNESS);

            if ( ch->in_room->drows > 0 )
               {
                  ch->in_room->drows--;

    	            if ( ch->in_room->drows == 0 && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
                     REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
               }
         }

      if ( IS_SET(ch->act, PLR_WIZINVIS) )
         REMOVE_BIT(ch->act, PLR_WIZINVIS);

      ch->in_room = was_in;
      add_tracks( ch, door );

      act( "#e$n #Lhas stepped through a #Pdoor #Lthat leads to the #y$T#L!#n", ch, NULL, dir_name[door], TO_ROOM );

      ch->in_room = now_in;

      if ( ch->fighting != NULL )
         ch->fighting->wait = 24;

      stop_fighting( ch, TRUE );
      ch->mana -= 1500000;
      ch->ctimer[12] = 4;
      return;
    }
}

void do_spider_summon( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->pcdata->followers > 0 )
      {
	   send_to_char("You have too many beings following you for this.\n\r",ch);
	   return;
      }

   if ( ch->mana < 1000000 )
      {
	   send_to_char("You are too tired to execute this technique.\n\r",ch);
	   return;
      }

   ch->pcdata->followers++;
   ch->mana -= 1000000;

   victim = create_mobile( get_mob_index( 14 ) );
   victim->level = 960;
   victim->hitroll = 1250000;
   victim->damroll = 1250000;
   victim->hit = 3000000;
   victim->max_hit = 3000000;
   victim->armor = number_range(-40000,-50000);
   SET_BIT( victim->act, ACT_MOUNT );

   victim->name = str_dup( "spider lloth" );
   victim->long_descr = str_dup("a large, ugly spider is standing here\n\r");
   victim->short_descr = str_dup("A spider of Lloth");

   char_to_room( victim, ch->in_room );

   af.type      = skill_lookup("charm person");
   af.duration  = -1;
   af.location  = APPLY_NONE;
   af.modifier  = 0;
   af.bitvector = AFF_CHARM;
   affect_to_char( victim, &af );

   act("You pull a #0black widow spider#n from Lolth's dimension!",ch,NULL,NULL,TO_CHAR);
   act("$n pulls a #0black widow spider#n from Lolth's dimension!",ch,NULL,NULL,TO_ROOM);
   act("The spiders abide by your every command.",ch,NULL,NULL,TO_CHAR);
   act("The spiders abide by $n's every command.",ch,NULL,NULL,TO_ROOM);
   add_follower( victim, ch );
   do_group( ch, victim->name );
}

void do_shocking_grasp( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int chance = 94;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc( "You don't have enough magical energy for this technique.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to grasp with shocking energy?\n\r", ch );
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

   if ( number_percent() > chance && victim->stunned == 0 && !is_martial( victim ) )
      victim->stunned = 1;

   ch->mana -= 250000;
   ch->wait = 12;

   act("You grasp $N #0firmly#n by the #Phand#n.",ch,NULL,victim,TO_CHAR);
   act("$n grasps $N #0firmily#n by the #Phand#n.",ch,NULL,victim,TO_NOTVICT);
   act("$n grasps you #0firmly#n by the #Phand#n.",ch,NULL,victim,TO_VICT);
   act("You scream as $N's grasp shocks your core!",victim,NULL,ch,TO_CHAR);
   act("$n screams as $N's grasp shocks $s core!",victim,NULL,ch,TO_ROOM);
   act("#yYou scream as $N#y's grasp shocks your core!#n",victim,NULL,ch,TO_CHAR);
   act("#y$n screams as $N#y's grasp shocks $s core!#n",victim,NULL,ch,TO_ROOM);

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLUNT;
   hurt_person( ch, victim, 700000, TRUE );
}   

void do_dancing_flames( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *militia = NULL, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to grace with the dance of flames?\n\r", ch );
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
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DROW ) || ( vch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && vch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
           || vch->move < 1000000 || vch->mana < 1000000 )
         continue;

      if ( ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
            continue;

      militia = vch;
      break;
   }

   if ( militia == NULL )
      {
         stc("You need to have a dedicated militia to perform this technique with.\n\r",ch);
         return;
      }

   if ( is_safe( militia, victim ) )
      return;

   ch->mana -= 1500000;
   ch->wait = 18;
   militia->mana -= 1000000;
   militia->move -= 1000000;

   act( "#RYou raise your staff high in the air as it bursts into flames.#n", ch, NULL, NULL, TO_CHAR );   
   act( "#R$n raises $s staff high in the air as it bursts into flames.#n", ch, NULL, NULL, TO_ROOM );   
   act( "#RYou mutter '#ySyi Flum Tr#R' as you tap $N gently with the staff.#n", ch, NULL, militia, TO_CHAR );   
   act( "#R$n mutters '#ySyi Flum Tr#R' as $e taps you gently with the staff.#n", ch, NULL, militia, TO_VICT );   
   act( "#R$n mutters '#ySyi Flum Tr#R' as $e taps $N gently with the staff.#n", ch, NULL, militia, TO_NOTVICT );   
   act( "#RYou twirl your flaming blade viciously as you dash towards $N!#n", militia, NULL, victim, TO_CHAR );   
   act( "#R$n twirls $s flaming blade viciously as $e dashes towards you!#n", militia, NULL, victim, TO_VICT );   
   act( "#R$n twirls $s flaming blade viciously as $e dashes towards $N!#n", militia, NULL, victim, TO_NOTVICT );   
   act( "#RYou dance to and fro by $N slashing $M all over $S body.#n", militia, NULL, victim, TO_CHAR );   
   act( "#R$n dances to and fro by you slashing you all over your body.#n", militia, NULL, victim, TO_VICT );   
   act( "#R$n dances to and fro by $N slashing $M all over $S body.#n", militia, NULL, victim, TO_NOTVICT );   
   act( "#RYou yell '#yHooooi!#R' as you DrIvE your blade over $N's head!#n", militia, NULL, victim, TO_CHAR );   
   act( "#R$n yells '#yHooooi!#R' as $e DrIvEs $s blade over your head!#n", militia, NULL, victim, TO_VICT );   
   act( "#R$n yells '#yHooooi!#R' as $e DrIvEs $s blade over $N's head!#n", militia, NULL, victim, TO_NOTVICT );   

   militia->attack_type = ATTACK_PHYSICAL;
   militia->ele_attack = ATTACK_FIRE;
   militia->attack_focus = OVERHEAD;
   militia->attack_var = SLASH;
   hurt_person( militia, victim, 1500000, TRUE );
}   

void do_holy_shame( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *militia = NULL, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;   

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to shame before Lolth?\n\r", ch );
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
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DROW ) || ( vch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && vch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
           || vch->move < 1000000 || vch->mana < 1000000 )
         continue;

      if ( ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
            continue;

      militia = vch;
      break;
   }

   if ( militia == NULL )
      {
         stc("You need to have a dedicated militia to perform this technique with.\n\r",ch);
         return;
      }

   if ( is_safe( militia, victim ) )
      return;

   ch->mana -= 1500000;
   ch->wait = 18;
   militia->mana -= 1000000;
   militia->move -= 1000000;

   act( "#wYou raise your staff high in the air as it shimmers with holy energy.#n", ch, NULL, NULL, TO_CHAR );   
   act( "#w$n raises $s staff high in the air as it shimmers with holy energy.#n", ch, NULL, NULL, TO_ROOM );   
   act( "#wYou mutter '#ySyi Mani Tr#w' as you tap $N gently with the staff.#n", ch, NULL, militia, TO_CHAR );   
   act( "#w$n mutters '#ySyi Mani Tr#w' as $e taps you gently with the staff.#n", ch, NULL, militia, TO_VICT );   
   act( "#w$n mutters '#ySyi Mani Tr#w' as $e taps $N gently with the staff.#n", ch, NULL, militia, TO_NOTVICT );   
   act( "#wYou twirl your divine blade viciously as you leap into the air!#n", militia, NULL, NULL, TO_CHAR );   
   act( "#w$n twirls $s divine blade viciously as $e leaps into the air!#n", militia, NULL, NULL, TO_ROOM );   
   act( "#wYou land back on the ground, hitting it hard as an energy wave flashes from it.#n", militia, NULL, NULL, TO_CHAR );
   act( "#w$n lands back on the ground, hitting it hard as an energy wave flashes from it.#n", militia, NULL, NULL, TO_ROOM );
   act( "#wYou scream in agony as the blast of energy sends you flying.#n", victim, NULL, NULL, TO_CHAR );
   act( "#w$n screams in agony as the blast of energy sends $m flying.#n", victim, NULL, NULL, TO_ROOM );

   militia->attack_type = ATTACK_MAGICAL;
   militia->ele_attack = ATTACK_HOLY;
   militia->attack_focus = DIRECT;
   militia->attack_var = BLAST;
   hurt_person( militia, victim, 1500000, TRUE );
}   

void do_dark_strike( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *militia = NULL, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to strike for glory from Lolth?\n\r", ch );
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
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DROW ) || ( vch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && vch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
           || vch->move < 1000000 || vch->mana < 1000000 )
         continue;

      if ( ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
            continue;

      militia = vch;
      break;
   }

   if ( militia == NULL )
      {
         stc("You need to have a dedicated militia to perform this technique with.\n\r",ch);
         return;
      }

   if ( is_safe( militia, victim ) )
      return;

   ch->mana -= 1500000;
   ch->wait = 18;
   militia->mana -= 1000000;
   militia->move -= 1000000;

   act( "#eYou raise your staff high in the air as it shimmers with dark energy.#n", ch, NULL, NULL, TO_CHAR );   
   act( "#e$n raises $s staff high in the air as it shimmers with dark energy.#n", ch, NULL, NULL, TO_ROOM );   
   act( "#eYou mutter '#ySyi Bleo Tr#e' as you tap $N gently with the staff.#n", ch, NULL, militia, TO_CHAR );   
   act( "#e$n mutters '#ySyi Bleo Tr#e' as $e taps you gently with the staff.#n", ch, NULL, militia, TO_VICT );   
   act( "#e$n mutters '#ySyi Bleo Tr#e' as $e taps $N gently with the staff.#n", ch, NULL, militia, TO_NOTVICT );   
   act( "#eYou twirl your demonic blade viciously as you flip over $N!#n", militia, NULL, victim, TO_CHAR );   
   act( "#e$n twirls $s demonic blade viciously as $e flips over you!#n", militia, NULL, victim, TO_VICT );   
   act( "#e$n twirls $s demonic blade viciously as $e flips over $N!#n", militia, NULL, victim, TO_NOTVICT );   
   act( "#eYou yell '#yYaaaah!#e' as you DrIvE your blade down $N's body!#n", militia, NULL, victim, TO_CHAR );   
   act( "#e$n yells '#yYaaaah!#e' as $e DrIvEs $s blade down your body!#n", militia, NULL, victim, TO_VICT );   
   act( "#e$n yells '#yYaaaah!#e' as $e DrIvEs $s blade down $N's body!#n", militia, NULL, victim, TO_NOTVICT );   
   act( "#eYou then SlAsH across $N's chest causing $M to scream in pain.#n", militia, NULL, victim, TO_CHAR );   
   act( "#e$n then SlAsHeS across your chest causing you to scream in pain.#n", militia, NULL, victim, TO_VICT );   
   act( "#e$n then SlAsHeS across $N's chest causing $M to scream in pain.#n", militia, NULL, victim, TO_NOTVICT );   

   militia->attack_type = ATTACK_PHYSICAL;
   militia->ele_attack = ATTACK_DARK;
   militia->attack_focus = OVERHEAD;
   militia->attack_var = SLASH;
   hurt_person( militia, victim, 1500000, TRUE );
}   

void do_roaring_skies( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *militia = NULL, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to electricute?\n\r", ch );
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
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DROW ) || ( vch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && vch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
           || vch->move < 1000000 || vch->mana < 1000000 )
         continue;

      if ( ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
            continue;

      militia = vch;
      break;
   }

   if ( militia == NULL )
      {
         stc("You need to have a dedicated militia to perform this technique with.\n\r",ch);
         return;
      }

   if ( is_safe( militia, victim ) )
      return;

   ch->mana -= 1500000;
   ch->wait = 18;
   militia->mana -= 1000000;
   militia->move -= 1000000;

   act( "#yYou raise your staff high in the air as it fizzes with electricity.#n", ch, NULL, NULL, TO_CHAR );   
   act( "#y$n raises $s staff high in the air as it fizzes with electricity.#n", ch, NULL, NULL, TO_ROOM );   
   act( "#yYou mutter '#wSyi Bliz Tr#y' as you tap $N gently with the staff.#n", ch, NULL, militia, TO_CHAR );   
   act( "#y$n mutters '#wSyi Bliz Tr#y' as $e taps you gently with the staff.#n", ch, NULL, militia, TO_VICT );   
   act( "#y$n mutters '#wSyi Bliz Tr#y' as $e taps $N gently with the staff.#n", ch, NULL, militia, TO_NOTVICT );   
   act( "#yYou twirl your shocking blade viciously as it is struck with lightning!#n", militia, NULL, NULL, TO_CHAR );   
   act( "#y$n twirls $s shocking blade viciously as it is struck with lightning!#n", militia, NULL, NULL, TO_ROOM );   
   act( "#yYou then dash towards $N as you impale your blade right through $S chest!#n", militia, NULL, victim, TO_CHAR );   
   act( "#y$n then dashes towards you as $e impales $s blade right through your chest!#n", militia, NULL, victim, TO_VICT );   
   act( "#y$n then dashes towards $N as $e impales $s blade right through $S chest!#n", militia, NULL, victim, TO_NOTVICT );   
   act( "#yYou spasm violently as you lie trembling on the ground!#n", victim, NULL, NULL, TO_CHAR );   
   act( "#y$n spasms violently as $e lies trembling on the ground!#n", victim, NULL, NULL, TO_ROOM );   

   militia->attack_type = ATTACK_PHYSICAL;
   militia->ele_attack = ATTACK_THUNDER;
   militia->attack_focus = DIRECT;
   militia->attack_var = PIERCE;
   hurt_person( militia, victim, 1500000, TRUE );
}   

void do_titans_rage( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *militia = NULL, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to blast with rocks?\n\r", ch );
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
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DROW ) || ( vch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && vch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
           || vch->move < 1000000 || vch->mana < 1000000 )
         continue;

      if ( ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
            continue;

      militia = vch;
      break;
   }

   if ( militia == NULL )
      {
         stc("You need to have a dedicated militia to perform this technique with.\n\r",ch);
         return;
      }

   if ( is_safe( militia, victim ) )
      return;

   ch->mana -= 1500000;
   ch->wait = 18;
   militia->mana -= 1000000;
   militia->move -= 1000000;

   act( "#gYou raise your staff high in the air as it turns into bark.#n", ch, NULL, NULL, TO_CHAR );   
   act( "#g$n raises $s staff high in the air as it turns into bark.#n", ch, NULL, NULL, TO_ROOM );   
   act( "#gYou mutter '#ySyi Zuwr Tr#g' as you tap $N gently with the staff.#n", ch, NULL, militia, TO_CHAR );   
   act( "#g$n mutters '#ySyi Zuwr Tr#g' as $e taps you gently with the staff.#n", ch, NULL, militia, TO_VICT );   
   act( "#g$n mutters '#ySyi Zuwr Tr#g' as $e taps $N gently with the staff.#n", ch, NULL, militia, TO_NOTVICT );   
   act( "#gYou twirl your rubbly blade viciously as you leap into the air!#n", militia, NULL, NULL, TO_CHAR );   
   act( "#g$n twirls $s rubbly blade viciously as $e leaps into the air!#n", militia, NULL, NULL, TO_ROOM );   
   act( "#gYou land back on the ground, hitting it hard as shards of rock leap from it.#n", militia, NULL, NULL, TO_CHAR );
   act( "#g$n lands back on the ground, hitting it hard as shard of rock leap from it.#n", militia, NULL, NULL, TO_ROOM );
   act( "#gYou are knocked away as the rocks pummel you HaRd!#n", victim, NULL, NULL, TO_CHAR );   
   act( "#g$n is knocked away as the rocks pummel $m HaRd!#n", victim, NULL, NULL, TO_ROOM );   

   militia->attack_type = ATTACK_PHYSICAL;
   militia->ele_attack = ATTACK_EARTH;
   militia->attack_focus = UPPERCUT;
   militia->attack_var = BLUNT;
   hurt_person( militia, victim, 1500000, TRUE );
}   

void do_swift_wave( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *militia = NULL, *vch, *vch_next;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MAGI && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Magi House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to wash away?\n\r", ch );
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
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( vch == ch || IS_NPC( vch ) || !is_same_group( ch, vch ) )
         continue;

      if ( !IS_CLASS( vch, CLASS_DROW ) || ( vch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && vch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
           || vch->move < 1000000 || vch->mana < 1000000 )
         continue;

      if ( ( ( obj = get_eq_char( vch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
         if ( ( ( obj = get_eq_char( vch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 326 && obj->pIndexData->vnum != 91 ) )
            continue;

      militia = vch;
      break;
   }

   if ( militia == NULL )
      {
         stc("You need to have a dedicated militia to perform this technique with.\n\r",ch);
         return;
      }

   if ( is_safe( militia, victim ) )
      return;

   ch->mana -= 1500000;
   ch->wait = 18;
   militia->mana -= 1000000;
   militia->move -= 1000000;

   act( "#LYou raise your staff high in the air as it melts into water.#n", ch, NULL, NULL, TO_CHAR );   
   act( "#L$n raises $s staff high in the air as it melts into water.#n", ch, NULL, NULL, TO_ROOM );   
   act( "#LYou mutter '#ySyi Jiua Tr#L' as you tap $N gently with the staff.#n", ch, NULL, militia, TO_CHAR );   
   act( "#L$n mutters '#ySyi Jiua Tr#L' as $e taps you gently with the staff.#n", ch, NULL, militia, TO_VICT );   
   act( "#L$n mutters '#ySyi Jiua Tr#L' as $e taps $N gently with the staff.#n", ch, NULL, militia, TO_NOTVICT );   
   act( "#LYou twirl your transparent blade viciously as you let it drop to the ground.#n", militia, NULL, NULL, TO_CHAR );   
   act( "#L$n twirls $s transparent blade viciously as $e lets it drop to the ground.#n", militia, NULL, NULL, TO_ROOM );   
   act( "#LAs the blade shatters, a ToRrEnT of water sizzles from the icy shards.#n", militia, NULL, NULL, TO_CHAR );
   act( "#LAs the blade shatters, a ToRrEnT of water sizzles from the icy shards.#n", militia, NULL, NULL, TO_ROOM );
   act( "#LYou flail your arms in vain as the deadly waves wash you away!#n", victim, NULL, NULL, TO_CHAR );
   act( "#L$n flails $s arms in vain as the deadly waves wash $m away!#n", victim, NULL, NULL, TO_ROOM );

   militia->attack_type = ATTACK_MAGICAL;
   militia->ele_attack = ATTACK_ICE;
   militia->attack_focus = DIRECT;
   militia->attack_var = BLAST;
   hurt_person( militia, victim, 1500000, TRUE );
}   

void do_black_widow( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to seep the poison into?\n\r", ch );
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

   if ( IS_SET( victim->more, MORE_POISON ) )
      {
         stc( "They are already severely poisoned elf.\n\r", ch );
         return;
      }
   
   ch->mana -= 50000;
   ch->wait = 4;

   act("You take a deep breath as energy swirls around you in a shroud of #0black#n mist.",ch,NULL,NULL,TO_CHAR);
   act("$n takes a deep breath as energy swirls around $m creating a shroud of #0black#n mist.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly thousands of Black Widow spiders appear at your feet ready to do your bidding.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly thousands of Black Widow spiders appear at $s feet ready to do $s bidding.",ch,NULL,NULL,TO_ROOM);
   act("You cackle in the #0darkness#n as you mutter '#yLittle Lolth followers, do thy worst!#n'",ch,NULL,NULL,TO_CHAR);
   act("Someone cackles in the #0darkness#n as $e mutters '#yLittle Lolth followers, do thy worst!#n'",ch,NULL,NULL,TO_ROOM);
   act("You point to your target and the spiders crawl up to $N and viciously bite $m causing them to yelp in pain!",ch,NULL,victim,TO_CHAR);
   act("$n points to $N and the spiders crawl up to $s victim, viciously biting $m causing them to yelp in pain!",ch,NULL,victim,TO_NOTVICT);
   act("$n points to you and the spiders crawl up to you. They viciously bite at you causing you to yelp in pain!",ch,NULL,victim,TO_VICT);
   SET_BIT( victim->more, MORE_POISON );
}

void do_lolth_dance( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->move < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you want to sweep into the dance of death?\n\r", ch );
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

   ch->move -= 250000;
   ch->wait = 12;

   act("You tip toe around the whole room in a devious dance of death.",ch,NULL,NULL,TO_CHAR);
   act("$n tip toes around the whole room in a devious dance of death.",ch,NULL,NULL,TO_ROOM);
   act("Your dance finally reaches a crescendo and a #0dark#n mist appears as a mystical aura.",ch,NULL,NULL,TO_CHAR);
   act("$n's dance finally reaches a crescendo and a #0dark#n mist appears as a mystical aura.",ch,NULL,NULL,TO_ROOM);
   act("You breathe in the mystical aura and let it fill your body, your eyes turn a deep #0black#n.",ch,NULL,NULL,TO_CHAR);
   act("$n breathes in the mystical aura and lets it fill their body, $s eyes turn a deep #0black#n.",ch,NULL,NULL,TO_ROOM);
   act("#0You then casually breathe the darkness that STrIKeS $N with Lolth's PaSsIoN!#n",ch,NULL,victim,TO_CHAR);
   act("#0$n then casually breathes the darkness that STrIKeS $N with Lolth's PaSsIoN!#n",ch,NULL,victim,TO_NOTVICT);
   act("#0$n then casually breathes the darkness that STrIKeS you with Lolth's PaSsIoN!#n",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_waver( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( IS_SET( ch->act, PLR_WIZINVIS ) )
      {
         act("Your complexion appears full again as the light bathes your face.",ch,NULL,NULL,TO_CHAR);
         act("$n's complexion appears full again as the light bathes $s face.",ch,NULL,NULL,TO_ROOM);
         REMOVE_BIT( ch->act, PLR_WIZINVIS );
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc( "You try to fade into the shadows but you can't concentrate properly.\n\r", ch );
         return;
      }

   act("The sun goes down and #0darkness#n surrounds you.",ch,NULL,NULL, TO_CHAR);
   act("The sun goes down and #0darkness#n surrounds $n.",ch,NULL,NULL,TO_ROOM);
   act("With a slight twinkle in your eye you retreat backwards into the #0shadows#n.",ch,NULL,NULL,TO_CHAR);
   act("With a slight twinkle in $s eye $n retreats backwards into the #0shadows#n.",ch,NULL,NULL,TO_ROOM);
   act("You breathe softly waiting for your next #0sacrifice#n...",ch,NULL,NULL,TO_CHAR);
   act("$n breathes softly waiting for $s next #0sacrifice#n...",ch,NULL,NULL,TO_ROOM);
   SET_BIT( ch->act, PLR_WIZINVIS );
}

void do_scout( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   ROOM_INDEX_DATA *chroom;
   ROOM_INDEX_DATA *victimroom;
   char arg [MAX_INPUT_LENGTH];
   char buf [MAX_INPUT_LENGTH];
   bool safe = FALSE;
   bool icy = FALSE;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 4 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "#PScout #eon whom?#n\n\r", ch );
         return;
      }

   if ( !IS_NPC(ch) && IS_SET(ch->more, MORE_SAFE) )
      safe = TRUE;

   if ( IS_SET( ch->in_room->room_flags, ROOM_ICETRAP ) && IS_CLASS( ch, CLASS_SNOW_ELF ) )
      icy = TRUE;

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

   char_from_room( ch );
   char_to_room( ch, chroom );

   act( "\n\r#eSatisfied on #w$N#e's whereabout, another #Pscout #eappears beside you.#n", ch, NULL, victim, TO_CHAR );
   stc( "#PShe #ethen relays the following to you using #whand #esignals:#n\n\r", ch );

   if ( safe )
      SET_BIT(ch->more, MORE_SAFE);

   if ( icy == TRUE )
      SET_BIT( ch->in_room->room_flags, ROOM_ICETRAP );

   if ( IS_NPC( victim ) ) 
      sprintf(buf, "#P%s #eis an #yNPC#e.#n\n\r",victim->short_descr);
      else sprintf(buf, "#P%s #eis a #yPlayer#e.#n\n\r", victim->name);

   send_to_char( buf, ch );
   sprintf(buf,"#PHp: #y%d#e/#y%d#e, #PMana: #y%d#e/#y%d#e, #PMove: #y%d#e/#y%d#e.#n\n\r", victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move );
   send_to_char( buf, ch );

   stc( "#e'#yPraise Lloth...#e' the #Pscout #esignals before vanishing into the shadows once more.#n\n\r", ch );
}

void do_demi_drow( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next, *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to give the wrath of Lolth?\n\r", ch );
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

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You start to concentrate on all the inner energies consumed in you...",ch,NULL,NULL,TO_CHAR);
   act("$n starts to concentrate on all the inner energies consumed inside $m...",ch,NULL,NULL,TO_ROOM);
   act("The energy surrounding your body makes you tremble as you levitate above your foes.",ch,NULL,NULL,TO_CHAR);
   act("The energy surrounding $n's body makes $m tremble as $e levitates above $s foes.",ch,NULL,NULL,TO_ROOM);
   act("You throw your arms wide apart from your chest and the #0dark#n energy ImPlOdEs creating #rchaos#n!",ch,NULL,NULL,TO_CHAR);
   act("$n throws $s arms wide apart from $s chest and the #0dark#n energy ImPlOdEs creating #rchaos#n!",ch,NULL,NULL,TO_ROOM);
   act("#RYou scream as the violent #0darkness#R corrupts your soul#n!",victim,NULL,NULL,TO_CHAR);
   act("#R$n screams as the violent #0darkness#R corrupts $s soul#n!",victim,NULL,NULL,TO_ROOM);
   
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("You try to avoid the #0darkness#n but get ImPlOdEd in its very wake.",vch,NULL,NULL,TO_CHAR);
      act("$n tries to avoid the #0darkness#n but gets ImPlOdEd in its very wake.",vch,NULL,NULL,TO_ROOM);
      ch->ele_attack = ATTACK_EARTH;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_focus = UPPERCUT;
      ch->attack_var = BLAST;
      hurt_person( ch, vch, 1000000, TRUE );
   }
}

void do_under_world( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 300000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to surround with burning hell?\n\r", ch );
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

   ch->mana -= 300000;
   ch->wait = 12;
  
   act("You viciously dance around $N in leaps and bounds.",ch,NULL,victim,TO_CHAR);
   act("$n viciously dances around $N in leaps and bounds.",ch,NULL,victim,TO_NOTVICT);
   act("$n viciously dances around you in leaps and bounds.",ch,NULL,victim,TO_VICT);
   act("You then casually draw a circle around $N and chant the words '#yElvyyn gisul desiol!#n'",ch,NULL,victim,TO_CHAR);
   act("$n then casually draws a circle around $N and chants the words '#yElvyyn gisul desiol!#n'",ch,NULL,victim,TO_NOTVICT);
   act("$n then casually draws a circle around you and chants the words '#yElvyyn gisul desiol!#n'",ch,NULL,victim,TO_VICT);
   act("You observe with a slick smile as #rmagma#n GuShEs up and surrounds $N.",ch,NULL,victim,TO_CHAR);
   act("$n observes with a slick smile as #rmagma#n GuShEs up and surrounds $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n observes with a slick smile as #rmagma#n GuShEs up and surrounds you.",ch,NULL,victim,TO_VICT);
   act("You cackle as $N screams, the #rmagma#n then rushes to engulf $m RiPpInG $m of any skin.",ch,NULL,victim,TO_CHAR);
   act("$n cackles as $N screams, the #rmagma#n then rushes to engulf $m RiPpInG $n of any skin.",ch,NULL,victim,TO_NOTVICT);
   act("$n cackles as you scream, the #rmagma#n then rushes to engulf you RiPpInG you of any skin.",ch,NULL,victim,TO_VICT);
   act("#0You ScReAm as the power of #RLolth's #rburning #Rhatred#0 overcomes you#n!",victim,NULL,NULL,TO_CHAR);
   act("#0$n ScReAmS as the power of #RLolth's #rburning #Rhatred#0 overcomes $m#n!",victim,NULL,NULL,TO_ROOM);
   act("#0You ScReAm as the power of #RLolth's #rburning #Rhatred#0 overcomes you#n!",victim,NULL,NULL,TO_CHAR);
   act("#0$n ScReAmS as the power of #RLolth's #rburning #Rhatred#0 overcomes $m#n!",victim,NULL,NULL,TO_ROOM);
   act("#0You ScReAm as the power of #RLolth's #rburning #Rhatred#0 overcomes you#n!",victim,NULL,NULL,TO_CHAR);
   act("#0$n ScReAmS as the power of #RLolth's #rburning #Rhatred#0 overcomes $m#n!",victim,NULL,NULL,TO_ROOM);

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_MAGICAL;
   hurt_person( ch, victim, 700000, TRUE );
}

void do_series_slash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
      {
         send_to_char( "But you are not using a weapon in your left hand.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to slash in series?\n\r", ch );
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

   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );
   
   act("#0Your image moves with LiGhTnInG speed and slashes $N at the left#n!",ch,NULL,victim,TO_CHAR);
   act("#0$n's image moves with LiGhTnInG speed and slashes $N at the left#n!",ch,NULL,victim,TO_NOTVICT);
   act("#0$n's image moves with LiGhTnInG speed and slashes you at the left#n!",ch,NULL,victim,TO_VICT);
   act("#0Your image moves with LiGhTnInG speed and slashes $N at the right#n!",ch,NULL,victim,TO_CHAR);
   act("#0$n's image moves with LiGhTnInG speed and slashes $N at the right#n!",ch,NULL,victim,TO_NOTVICT);
   act("#0$n's image moves with LiGhTnInG speed and slashes you at the right#n!",ch,NULL,victim,TO_VICT);
   act("#0Your image moves with LiGhTnInG speed and slashes $N at the back#n!",ch,NULL,victim,TO_CHAR);
   act("#0$n's image moves with LiGhTnInG speed and slashes $N at the back#n!",ch,NULL,victim,TO_NOTVICT);
   act("#0$n's image moves with LiGhTnInG speed and slashes you at the back#n!",ch,NULL,victim,TO_VICT);
   act("#0Finally, you go wild and your image moves with LiGhTnInG speed and you scream '#yOliuso beryy!#n'",ch,NULL,NULL,TO_CHAR);
   act("#0Finally, $n goes wild and $s image moves with LiGhTnInG speed and $e screams '#yOliuso beryy!#n'",ch,NULL,NULL,TO_ROOM);
   act("#0You move closer to $N and then scream '#yOliuso enblyrr!#n'",ch,NULL,victim,TO_CHAR);
   act("#0$n moves closer to $N and then screams '#yOliuso enblyrr!#n'",ch,NULL,victim,TO_NOTVICT);
   act("#0$n moves closer to you and then screams '#yOliuso enblyrr!#n'",ch,NULL,victim,TO_VICT);
   act("You then take your aim and scream '#yOliuso tyvyr!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n then takes $s aim and screams '#yOliuso tyvyr!#n'",ch,NULL,NULL,TO_ROOM);
   act("#0Your image moves with LiGhTnInG speed and slashes $N at the front#n!",ch,NULL,victim,TO_CHAR);
   act("#0$n's image moves with LiGhTnInG speed and slashes $N at the front#n!",ch,NULL,victim,TO_NOTVICT);
   act("#0$n's image moves with LiGhTnInG speed and slashes you at the front#n!",ch,NULL,victim,TO_VICT);
   act("#0Your image moves with LiGhTnInG speed and StRiKeS $N at the front#n!",ch,NULL,victim,TO_CHAR);
   act("#0$n's image moves with LiGhTnInG speed and StRiKeS $N at the front#n!",ch,NULL,victim,TO_NOTVICT);
   act("#0$n's image moves with LiGhTnInG speed and StRiKeS you at the front#n!",ch,NULL,victim,TO_VICT);
   act("#0Your image moves with LiGhTnInG speed and ViCiOuSlY StRiKeS $N at the front#n!",ch,NULL,victim,TO_CHAR);
   act("#0$n's image moves with LiGhTnInG speed and ViCiOuSlY StRiKeS $N at the front#n!",ch,NULL,victim,TO_NOTVICT);
   act("#0$n's image moves with LiGhTnInG speed and ViCiOuSlY StRiKeS you at the front#n!",ch,NULL,victim,TO_VICT);

   if ( !IS_ARM_L(victim,LOST_ARM) )
      {
	    SET_BIT(victim->loc_hp[2],LOST_ARM);
	    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	    make_part(victim,"arm");
	 
          if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
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

   if ( number_percent() > 40 && !IS_ARM_R(victim,LOST_ARM) )
      {
          SET_BIT(victim->loc_hp[3],LOST_ARM);
          SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
          make_part(victim,"arm");

          if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
             {
                if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                   take_item(victim,obj);
             }

           if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
              take_item(victim,obj);
           if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
              take_item(victim,obj);
           if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
              take_item(victim,obj);
           if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
              take_item(victim,obj);
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = SLASH;
   hurt_person( ch, victim, 3500000, TRUE );
}

void do_catacomb(CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 4 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( IS_SET( ch->in_room->room_flags, ROOM_CATACOMB ) )
      {
         stc( "#eYou silently remove the encavement with a few wrist movements.#n\n\r", ch );
         REMOVE_BIT(ch->in_room->room_flags, ROOM_CATACOMB);
         return;
      }

   if ( ch->move < 1500000 )
      {
         send_to_char( "You don't have enough energy for that.\n\r", ch );
         return;
      }

   ch->move -= 1500000;
   ch->wait = 12;

   SET_BIT(ch->in_room->room_flags, ROOM_CATACOMB);
   act("#0You place your hands on the ground and concentrate deeply...#n",ch,NULL,NULL,TO_CHAR);
   act("#0$n places $s hands on the ground and concentrates deeply...#n",ch,NULL,NULL,TO_ROOM);
   act("#0You dig your hand into the earth and RiP up the soil causing the room to become a catacomb!#n",ch,NULL,NULL,TO_CHAR);
   act("#0$n digs $s hands into the earth and RiPs up the soil causing the room to become a catacomb!#n",ch,NULL,NULL,TO_ROOM);  
}

void do_lolth_shoot(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || ( obj->pIndexData->vnum != 398 && obj->pIndexData->vnum != 91 ) )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || ( obj->pIndexData->vnum != 398 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "But you are not using a #eDrow #PCross#wbow#n.\n\r", ch );
            return;
         }

   if ( ch->move < 100000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to fire your crossbow at?\n\r", ch );
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

   ch->move -= 100000;
   ch->wait = 12;

   act("You dive across the room and position your crossbow.",ch,NULL,NULL,TO_CHAR);
   act("$n dives across the room and positions $s crossbow.",ch,NULL,NULL,TO_ROOM);
   act("You shoot your crossbow and the arrows pound $N!",ch,NULL,victim,TO_CHAR);
   act("$n shoots $s crossbow and the arrows pound $N!",ch,NULL,victim,TO_NOTVICT);
   act("$n shoots $s crossbow and the arrows pound you!",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person(ch,victim,700000,TRUE);
}

void do_orbb_lance(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 12 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to use the imaginary lance on?\n\r", ch );
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

   ch->mana -= 250000;
   ch->wait = 18;

   act("You stare at your hand intently as your eyes glow bright #yyellow#n.",ch,NULL,NULL,TO_CHAR);
   act("$n stares at $s hand intently as $s eyes glow bright #yyellow#n.",ch,NULL,NULL,TO_ROOM);
   act("The glowing glare that your eyes emit suddenly turns your hand into a #Ccyan #0Lance#n!",ch,NULL,NULL,TO_CHAR);
   act("The glowing glare that $n's eyes emit suddenly turns $s hand into a #Ccyan #0Lance#n!",ch,NULL,NULL,TO_ROOM);
   act("You mutter the words softly '#yKyyr beniyryr...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n mutters the words softly '#yKyyr beniyryr...#n'",ch,NULL,NULL,TO_ROOM);
   act("Suddenly your hand lance BuRsTs into #zTHOUSANDS#n of #0spiders#n.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly $n's hand lance BuRsTs into #zTHOUSANDS#n of #0spiders#n.",ch,NULL,NULL,TO_ROOM);
   act("You then take a leap into the sky and drift your lance all over $N's body!",ch,NULL,victim,TO_CHAR);
   act("$n then takes a leap into the sky and drifts $s lance all over $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("$n then takes a leap into the sky and drifts $s lance all over your body!",ch,NULL,victim,TO_VICT);
   act("The little spiders that surround your lance transfers to $N's body and viciously bite $m.",ch,NULL,victim,TO_CHAR);
   act("The little spiders that surround $n's lance transfer to $N's body and viciously bite $m.",ch,NULL,victim,TO_NOTVICT);
   act("The little spiders that surround $n's lance transfer to your body and viciously bite you.",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = AERIAL;
   ch->attack_var = HIT;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_olplynir_dro(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to drain energy from?\n\r", ch );
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

   ch->mana -= 1000000;
   ch->wait = 12;

   act("You touch your forehead softly with two fingers and concentrate.",ch,NULL,NULL,TO_CHAR);
   act("$n touches $s forehead softly with two fingers and concentrates.",ch,NULL,NULL,TO_ROOM);
   act("You send a shockwave up to $N and it paralyzes $M!",ch,NULL,victim,TO_CHAR);
   act("$n sends a shockwave up to $N and it paralyzes $M!",ch,NULL,victim,TO_NOTVICT);
   act("$n sends a shockwave up to you and it paralyzes you!",ch,NULL,victim,TO_VICT);
   act("You swiftly come behind $N and start to suck of all $S lifeforce!",ch,NULL,victim,TO_CHAR);
   act("$n swiftly comes behind $N and starts to suck of all $S lifeforce!",ch,NULL,victim,TO_NOTVICT);
   act("$n swiftly comes behind you and starts to suck of all your lifeforce!",ch,NULL,victim,TO_VICT);
   
   hit_gain( ch, 100000 );
   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_focus = DIRECT;
   ch->attack_var = HIT;

   hurt_person( ch, victim, 700000, TRUE );
}

void do_fallen_angel(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to instill darkness upon?\n\r", ch );
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
 
   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You recite the mystical past of all the elves '#ySsenir flyrno bedioplyrr.#n'",ch,NULL,NULL,TO_CHAR);
   act("$n recites the mystical past of all the elves '#ySsenir flyrno bedioplyrr.#n'",ch,NULL,NULL,TO_ROOM);
   act("You keep chanting the words of the fallen elves as a demonic blast flashes past you and into $N.",ch,NULL,victim,TO_CHAR);
   act("$n keeps chanting the words of the fallen elves as a demonic blast flashes past $m and into $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n keeps chanting the words of the fallen elves as a demonic blast flashes past $m and into you.",ch,NULL,victim,TO_VICT);
   act("#0The demonic blast corrupts $N's very soul as you cackle!#n",ch,NULL,victim,TO_CHAR);
   act("#0The demonic blast corrupts $N's very soul as $n cackles!#n",ch,NULL,victim,TO_NOTVICT);
   act("#0The demonic blast corrupts your very soul as $n cackles!#n",ch,NULL,victim,TO_VICT);
   act("You snarl as the wind brushes against your silver hair '#yhmph... Azazel would be proud....#n'",ch,NULL,NULL,TO_CHAR);
   act("$n snarls as the wind brushes against $s silver hair '#yhmph... Azazel would be proud....#n'",ch,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;
   hurt_person( ch, victim, 2000000, TRUE );
}

void do_shadow_shoot(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 16 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( !IS_SET( ch->act, PLR_WIZINVIS ) )
      {
         stc( "You need to be hiding within the shadows to execute this technique.\n\r", ch );
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc( "You are too distracted to focus your attention on your foe.\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 12 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a crossbow.\n\r", ch );
            return;
         }

   if ( ch->move < 1500000 )
      {
         stc("You don't have enough energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to shoot from the darkness?\n\r", ch );
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

   ch->move -= 1500000;
   ch->wait = 6;

   act("You silently fire your bow at $N from within the #0shadows#n!",ch,NULL,victim,TO_CHAR);
   act("$n silently fires $s bow at $N from within the #0shadows#n!",ch,NULL,victim,TO_NOTVICT);
   act("$n silently fires $s bow at you from within the #0shadows#n!",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = LOW;
   ch->attack_var = PIERCE;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_viryns_mace(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 300000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to instill Viryn's pain upon?\n\r", ch );
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

   ch->mana -= 300000;
   ch->wait = 18;
   
   act("You get on your knees and whisper to yourself '#yViryn oopulnir ryyr#n'",ch,NULL,NULL,TO_CHAR);
   act("$n gets on $s knees and whispers '#yViryn oopulnir ryyr#n'",ch,NULL,NULL,TO_ROOM);
   act("You then stand up and watch as the magical mace transforms in the palm of your hand.",ch,NULL,NULL,TO_CHAR);
   act("$n then stands up and watches as the magical mace transforms into the plam of $s hands.",ch,NULL,NULL,TO_ROOM);
   act("You look up to the heavens asking if you can borrow Viryn's power and StRiKe at $N!",ch,NULL,victim,TO_CHAR);
   act("$n looks up to the heavens asking if $e can borrow Viryn's power and StRiKeS at $N!",ch,NULL,victim,TO_NOTVICT);
   act("$n looks up to the heavens asking if $e can borrow Viryn's power and StRiKeS at you!",ch,NULL,victim,TO_VICT);
   act("You strike $N with amazing FoRcE!",ch,NULL,victim,TO_CHAR);
   act("$n strikes $N with amazing FoRcE!",ch,NULL,victim,TO_NOTVICT);
   act("$n strikes you with amazing FoRcE!",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_DARK;
   hurt_person(ch,victim,600000,TRUE);
}

void do_elghinn_wusuul(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You haven't advanced enough in your studies to do this technique yet.\n\r",ch);
         return;
      }

   if ( ch->move < 1500000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( ch->ctimer[5] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you wish to convert?\n\r", ch );
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

   ch->move -= 1500000;
   ch->wait = 8;
   ch->ctimer[5] = 6;

   act("You bow your head in prayer as the sky turns #0black#n.",ch,NULL,NULL,TO_CHAR);
   act("$n bows $s head in prayer as the sky turns #0black#n.",ch,NULL,NULL,TO_ROOM);
   act("You secretly use your worst techniques to convert $N to a follower of Lolth.",ch,NULL,victim,TO_CHAR);
   act("$n secretly uses $s worst techniques to convert $N to a follower of Lloth.",ch,NULL,victim,TO_NOTVICT);
   act("$n secretly uses $s worst techniques to convert you to a follower of Lloth.",ch,NULL,victim,TO_VICT);
   act("You cackle as $N doesn't realise $s most FaTaL mistake.",ch,NULL,victim,TO_CHAR);
   act("$n cackles as $N doesn't realise $s most FaTaL mistake.",ch,NULL,victim,TO_NOTVICT);
   act("$n cackles as you don't realise your most FaTaL mistake.",ch,NULL,victim,TO_VICT);
   act("You see Lloth who appears behind $N ready to backstab $m!",ch,NULL,victim,TO_CHAR);
   act("$n sees Lloth who appears behind $N ready to backstab $m!",ch,NULL,victim,TO_NOTVICT);
   act("$n watches intently for something that appears not to be there!",ch,NULL,victim,TO_VICT);
   act("You watch as Lolth raises the knife and ViCiOuSlY #ztwists#n the knife in $N's back!",ch,NULL,victim,TO_CHAR);
   act("$n watches as Lolth raises the knife and ViCiOuSlY #ztwists#n the knife in $N's back!",ch,NULL,victim,TO_NOTVICT);
   act("$n watches as Lolth raises the knife and ViCiOuSlY #ztwists#n the knife in your back!",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = LOW;
   ch->attack_var = PIERCE;
   hurt_person( ch, victim, 1000000, TRUE );
}

void do_lolth_thalack(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;
   
   if ( !IS_CLASS( ch, CLASS_DROW ) )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->pcdata->powers[DROW_CLASS] != DROW_MILITIA && ch->pcdata->powers[DROW_CLASS] != DROW_AVATAR )
      {
         stc("You aren't a fully pledged member of the Militia House... Learn your place.\n\r",ch);
         return;
      }

   if ( ch->mana < 350000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to feel the power of Lolth?\n\r", ch );
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

   ch->mana -= 350000;
   ch->wait = 18;

   act("You smile up to the heavens as a #0dark#n swirling mist delivers you power!",ch,NULL,NULL,TO_CHAR);
   act("$n smiles up to the heavens as a #0dark#n swirling mist delivers $m power!",ch,NULL,NULL,TO_ROOM);
   act("You suddenly seem to be thirteen times as powerful as before as you point at $N.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly seems to be thirteen times as powerful as before as $e points at $N.",ch,NULL,victim,TO_NOTVICT);
   act("$n suddenly seems to be thirteen times as powerful as before as $e points at you.",ch,NULL,victim,TO_VICT);
   act("You let out a shrill battle cry '#yTHAAAALAAAACKKKKK!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n lets out a shrill battle cry '#yTHAAAALAAAACKKKKK!#n'",ch,NULL,NULL,TO_ROOM);
   act("Your energy bursts out EnGuLfInG $N!",ch,NULL,victim,TO_CHAR);
   act("$n's energy bursts out EnGuLfInG $N!",ch,NULL,victim,TO_NOTVICT);
   act("$n's energy bursts out EnGuLfInG you!",ch,NULL,victim,TO_VICT);
   act("You celebrate in all your mischief as you hear the torture of $N!",ch,NULL,victim,TO_CHAR);
   act("$n celebrates in all $s mischief as $e hears the torture of $N!",ch,NULL,victim,TO_NOTVICT);
   act("$n celebrates in all $s mischief as $e hears the torture of you!",ch,NULL,victim,TO_VICT);

   ch->attack_type = ATTACK_MAGICAL;
   ch->ele_attack = ATTACK_THUNDER;
   hurt_person(ch,victim,600000,TRUE);
}

void do_death_smell( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next, *victim;
   char arg [MAX_INPUT_LENGTH];
   int sn;
   
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_DROW ) || ch->clannum != 2 )
      {
         stc( "Huh?\n\r", ch );
         return;
      }

   if ( ch->ctimer[2] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 50000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to give the breath of Lolth?\n\r", ch );
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

   ch->mana -= 50000;
   ch->wait = 12;
   ch->ctimer[2] = 12;

   act("You get down on your knees preparing a mass amount of power.",ch,NULL,NULL,TO_CHAR);
   act("$n gets down on $s knees preparing a mass amount of power.",ch,NULL,NULL,TO_ROOM);
   act("You softly whisper #Rinto #rthe #0darkness#n '#0#zDeath#n..#0#zSmell#n..'",ch,NULL,NULL,TO_CHAR);
   act("$n softly whispers #Rinto #rthe #0darkness#n '#0#zDeath#n..#0#zSmell#n..'",ch,NULL,NULL,TO_ROOM);
   act("You disappear into thin air as a opaque black mist fills the room.",ch,NULL,NULL,TO_CHAR);
   act("$n disappears into thin air as a opaque black mist fills the room.",ch,NULL,NULL,TO_ROOM);
   act("#0You cry out in so much pain as the smell overcomes you!#n",victim,NULL,NULL,TO_CHAR);
   act("#0$n cries out in  so much pain as the smell overcomes $m!#n",victim,NULL,NULL,TO_ROOM);
   act("#lYou cry out in so much pain as the smell overcomes you!#n",victim,NULL,NULL,TO_CHAR);
   act("#l$n cries out in so much pain as the smell overcomes $m!#n",victim,NULL,NULL,TO_ROOM);
   act("#0You let out your final cry as the smell poisons your very soul!#n",victim,NULL,NULL,TO_CHAR);
   act("#0$n lets out $s final cry as the smell poisons $s very soul!#n",victim,NULL,NULL,TO_ROOM);
   act("#rYou collapse on the ground.. with a poisoned soul.. to the #0core#n.",victim,NULL,NULL,TO_CHAR);
   act("#r$n collapses on the ground.. with a poisoned soul.. to the #0core#n.",victim,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      if ( number_percent() > 60 && !IS_SET(vch->monkstuff, MONK_DEATH) )
         SET_BIT(vch->monkstuff, MONK_DEATH);

      act("#0The #n#zevil#n #0corrupts your very soul as the death smell is inhaled!#n",vch,NULL,NULL,TO_CHAR);
      act("#0The #n#zevil#n #0corrupts $n's very soul as the death smell is inhaled!#n",vch,NULL,NULL,TO_ROOM);

      hurt_person(ch,vch,100000,TRUE);
   }
}     
