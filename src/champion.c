/***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
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
#include "monk.h"

bool special_slam( CHAR_DATA *victim, int dir )
{
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA *pexit;
   EXIT_DATA *pexit_rev;
   char buf[ MAX_INPUT_LENGTH ];
   char direction[ MAX_INPUT_LENGTH ];
   int door;
   int rev_dir;
   bool test = FALSE;

   rev_dir = 0;

   door = dir;

   if ( door == 0 ) {sprintf(direction,"north");rev_dir = 2;}
   if ( door == 1 ) {sprintf(direction,"east");rev_dir = 3;}
   if ( door == 2 ) {sprintf(direction,"south");rev_dir = 0;}
   if ( door == 3 ) {sprintf(direction,"west");rev_dir = 1;}
   if ( door == 4 ) {sprintf(direction,"up");rev_dir = 5;}
   if ( door == 5 ) {sprintf(direction,"down");rev_dir = 4;}

   if ( victim->in_room->vnum >= 33500 && victim->in_room->vnum <= 33800 )
      {
         sprintf( buf, "#w$n goes flying to the %s.#n", direction );
         act(buf,victim,NULL,NULL,TO_ROOM);
         return TRUE;
      }

   if ( IS_SET(victim->in_room->room_flags, ROOM_SHIFT) || IS_SET(victim->in_room->room_flags, ROOM_ENTOMB)
        || IS_SET(victim->in_room->room_flags, ROOM_BARRIER) || IS_SET(victim->in_room->room_flags, ROOM_CATACOMB) )
      return TRUE;

   if ( ( pexit = victim->in_room->exit[door] ) == NULL || ( to_room = pexit->to_room ) == NULL )
      return TRUE;

   pexit = victim->in_room->exit[door];

   if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
      {
         if ( IS_SET(pexit->exit_info, EX_LOCKED) )
            REMOVE_BIT(pexit->exit_info, EX_LOCKED);

         if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
            REMOVE_BIT(pexit->exit_info, EX_CLOSED);

         sprintf( buf, "#w$n got SMASH through the %s to the %s.#n", pexit->keyword, direction);
         act( buf,victim,NULL,NULL,TO_ROOM);
         sprintf( buf,"#wYou are flung to the %s, smashing through the %s.#n", direction,pexit->keyword);
         act( buf,victim,NULL,NULL,TO_CHAR);
         sprintf( buf,"#wThere is a loud crash as $n smashes through the $d.#n");
         act( buf,victim,NULL,pexit->keyword,TO_ROOM);

         if ( ( to_room = pexit->to_room ) != NULL && ( pexit_rev = to_room->exit[rev_dir] ) != NULL
              && pexit_rev->to_room == victim->in_room && pexit_rev->keyword != NULL )
            {
               if ( IS_SET(pexit_rev->exit_info, EX_LOCKED) )
                  REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );

               if ( IS_SET(pexit_rev->exit_info, EX_CLOSED) )
                  REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );

               if (door == 0) sprintf(direction,"south");
               if (door == 1) sprintf(direction,"west");
               if (door == 2) sprintf(direction,"north");
               if (door == 3) sprintf(direction,"east");
               if (door == 4) sprintf(direction,"down");
               if (door == 5) sprintf(direction,"up");

               if ( victim->in_room == to_room )
                  test == TRUE;
                  else add_tracks( victim, door );

               char_from_room(victim);
               char_to_room(victim,to_room);

               sprintf( buf,"#w$n comes smashing in through the %s $d.#n", direction);
               act( buf,victim,NULL,pexit->keyword,TO_ROOM);
            }
      }
      else {
              sprintf(buf,"#w$n goes flying to the %s.#n", direction);
              act(buf,victim,NULL,NULL,TO_ROOM);

              if ( door == 0 ) sprintf(direction,"south");
              if ( door == 1 ) sprintf(direction,"west");
              if ( door == 2 ) sprintf(direction,"north");
              if ( door == 3 ) sprintf(direction,"east");
              if ( door == 4 ) sprintf(direction,"down");
              if ( door == 5 ) sprintf(direction,"up");

              if ( victim->in_room == to_room )
                 test == TRUE;
                 else add_tracks( victim, door );

              char_from_room(victim);
              char_to_room(victim,to_room);

              sprintf(buf,"#w$n comes flying in from the %s.#n", direction);
              act(buf,victim,NULL,NULL,TO_ROOM);
           }

   return test;
}

void do_deathtouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 20 )
       {
          stc("You aren't powerful enough to perform the death touch.\n\r",ch);
          return;
       }

    if ( ch->move < 1000000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use the Death Touch on?\n\r",ch);
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

    if ( victim == ch )
       {
           stc("That doesn't seem like a good idea.\n\r",ch);
           return;
       }

    if ( IS_SET(victim->monkstuff, MONK_DEATH) )
       {
	   stc("They are already crippled.\n\r",ch);
	   return;
       }

    ch->move -= 1000000;
    act("You place your hands on $N's head and channel negative energy into $m.",ch,NULL,victim,TO_CHAR);
    act("$n places $s hands on your head, and you scream in utter pain.",ch,NULL,victim,TO_VICT);
    act("$n places $s hands on $N's head and $N screams in pain.",ch,NULL,victim,TO_NOTVICT);
    SET_BIT(victim->monkstuff, MONK_DEATH);
}

void do_chitilak( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 12 )
       {
          stc("You aren't powerful enough to perform the Chi Tilak.\n\r",ch);
          return;
       }

    if ( ch->focus[CURRENT] < 50 )
       {
          stc("You don't have enough focus for this technique.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to drain life from?\n\r",ch);
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

    ch->wait = 6;
    ch->hit = UMIN( ch->hit + 250000, ch->max_hit );
    ch->move = UMIN( ch->move + 1500000, ch->max_move );
    ch->focus[CURRENT] -= 50;

    act("You place your hands on $N's head and drain positive energy from $m.",ch,NULL,victim,TO_CHAR);
    act("$n places $s hands on your head, and you start feeling weaker.",ch,NULL,victim,TO_VICT);
    act("$n places $s hands on $N's head and $E suddenly looks weaker.",ch,NULL,victim,TO_NOTVICT);
    hurt_person( ch, victim, 350000, TRUE );
}

void do_healingtouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 16 )
       {
          stc("You aren't powerful enough to perform the Healing Touch.\n\r",ch);
          return;
       }

    if ( ch->move < 1000000 )
       {
          stc("You are too tired to execute this technique.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       victim = ch;    
       else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
               {
                  send_to_char( "They aren't here.\n\r", ch );
                  return;
               }

    WAIT_STATE(ch, 12);
    ch->move -= 1000000;

    if ( ch == victim )
       {
          stc("You focus your energy, and magical sparks leap out of your body.\n\r",ch);
          act("$n concentrates, and magical sparks leap out of $s body.",ch,NULL,NULL,TO_ROOM);
       }
       else {
               act("You focus your energy, and magical sparks leap out of $N's body.",ch,NULL,victim,TO_CHAR);
               act("$n concentrates, and magical sparks leap out of your body.",ch,NULL,victim,TO_VICT);
               act("$n concentrates, and magical sparks leap out of $N's body.",ch,NULL,victim,TO_NOTVICT);
            }

    SET_BIT(victim->monkstuff, MONK_HEAL);
}

void do_demmak( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 8 )
       {
          stc("You aren't powerful enough to perform the Demmak.\n\r",ch);
          return;
       }

    if ( ch->demmak > 0 )
       {
	   send_to_char("You are already performing the Dem Mak.\n\r",ch);
	   return;
       }

    if ( ch->position != POS_FIGHTING )
       {
          stc("You need to be fighting to perform the Dem Mak.\n\r",ch);
          return;
       }

    if ( ch->move < 250000 )
       {
          stc("You are too tired to perform this technique.\n\r",ch);
          return;
       }
  
    ch->move -= 250000;
    act("You begin the sweeping motion of the Dem Mak technique.",ch,NULL,NULL,TO_CHAR);
    act("$n begins the sweeping motion of the Dem Mak technique.",ch,NULL,NULL,TO_ROOM);
    ch->demmak = 3;
}

void do_ironhammer( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   int count = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->monkab[BODY] < 7 )
      {
         stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->focus[CURRENT] < 60 )
      {
	   stc("You need at least 60 focus points to use the Iron Hammer.\n\r", ch);
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

   ch->focus[CURRENT] -= 60;
   stc("Your hands glow dimly as you draw your opponent's chi within you.\n\r",ch);
   act("$n's hands glows dimly as $e draws $s opponent's chi within $mself.",ch,NULL,NULL,TO_ROOM);
   stc("You scream loudly as you rapidly spin around and smash the ground with your right fist.\n\r",ch);
   act("$n screams loudly as $e rapidly spins around and smashes the ground with $s right fist.",ch,NULL,NULL,TO_ROOM);
 
   victim = ch->in_room->people;
   for ( ; victim ; victim = victim->next_in_room )
   {
      if ( victim == ch || victim->fighting != ch )
         continue;

      act("The shock from $N's punch lifts you off your feet and send you flying to the other end of the room.",victim,NULL,ch,TO_CHAR);
      act("The shock from your punch lifts $n off $s feet and sends $m flying to the other end of the room.",victim,NULL,ch,TO_VICT);
      act("The shock from $N's punch lifts $n off $s feet and sends $m flying to the other end of the room.",victim,NULL,ch,TO_NOTVICT);
      victim->position = POS_STUNNED;
      victim->fighting = NULL;
   }

   ch->fighting = NULL;
   ch->position = POS_STANDING;
}

void do_kakeio( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( IS_CLASS(ch, CLASS_MONK) && ch->pkill < 20 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( IS_CLASS(ch, CLASS_MONK) && ch->ctimer[0] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use Kakeio on?\n\r",ch);
          return;
       }

    if ( ch->move < 500000 )
       {
          stc("You are too tired to execute this technique atm?\n\r",ch);
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

    act("You stun $N with a vicious kick in the stomach.",ch,NULL,victim,TO_CHAR);
    act("$n stuns you with a vicious kick in the stomach.",ch,NULL,victim,TO_VICT);
    act("$n stuns $N with a vicious kick in the stomach.",ch,NULL,victim,TO_NOTVICT);
    act("You suddenly spin around smashing your foot across $N's face.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly spins around smashing $s foot across your face.",ch,NULL,victim,TO_VICT);
    act("$n suddenly spins around smashing $s foot across $N's face.",ch,NULL,victim,TO_NOTVICT);
    act("You then kick $N in the chest as #Psparks #wof #yelectricity#n fizzes around your foot.",ch,NULL,victim,TO_CHAR);
    act("$n then kicks you in the chest as #Psparks #wof #yelectricity#n fizzes around $s foot.",ch,NULL,victim,TO_VICT);
    act("$n then kicks $N in the chest as #Psparks #wof #yelectricity#n fizzes around $s foot.",ch,NULL,victim,TO_NOTVICT);
    act("You then kick $N in the chest as #Psparks #wof #yelectricity#n fizzes around your foot.",ch,NULL,victim,TO_CHAR);
    act("$n then kicks you in the chest as #Psparks #wof #yelectricity#n fizzes around $s foot.",ch,NULL,victim,TO_VICT);
    act("$n then kicks $N in the chest as #Psparks #wof #yelectricity#n fizzes around $s foot.",ch,NULL,victim,TO_NOTVICT);
    act("You then kick $N in the chest as #Psparks #wof #yelectricity#n fizzes around your foot.",ch,NULL,victim,TO_CHAR);
    act("$n then kicks you in the chest as #Psparks #wof #yelectricity#n fizzes around $s foot.",ch,NULL,victim,TO_VICT);
    act("$n then kicks $N in the chest as #Psparks #wof #yelectricity#n fizzes around $s foot.",ch,NULL,victim,TO_NOTVICT);
    act("Finally you do a half-split SmAsHiNg your foot down $N's body as $E gets struck by #ylightning#n.",ch,NULL,victim,TO_CHAR);
    act("Finally $n does a half-split SmAsHiNg $s foot down your body as you get struck by #ylightning#n.",ch,NULL,victim,TO_VICT);
    act("Finally $n does a half-split SmAsHiNg $s foot down $N's body as $E gets struck by #ylightning#n.",ch,NULL,victim,TO_NOTVICT);

    ch->move -= 500000;

    if ( IS_CLASS(ch, CLASS_MONK) )
       {
          ch->ctimer[0] = 4;
          ch->wait = 3;
       }
       else ch->wait = 18;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->ele_attack = ATTACK_THUNDER;
    ch->attack_var = HIT;
    ch->attack_focus = OVERHEAD;

    if ( IS_CLASS( ch, CLASS_MONK ) )
       hurt_person( ch, victim, 750000, TRUE );
       else hurt_person( ch, victim, 1000000, TRUE );

    act("You land softly on your feet as you backflip into a fighting stance.",ch,NULL,NULL,TO_CHAR);
    act("$n lands softly on $s feet as $e backflips into a fighting stance.",ch,NULL,NULL,TO_ROOM);
}

void do_power_shot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 5 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use this on?\n\r",ch);
          return;
       }

    if ( ch->move < 500000 )
       {
          stc("You are too tired to execute this technique atm.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_world( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
                return;
             }
       }

    if ( is_safe(ch,victim) )
       return;

    act("You spread your arms apart as you form an energy blade.",ch,NULL,NULL,TO_CHAR);
    act("$n spreads $s arms apart as $e forms an energy blade.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yTaste the Windblade of Fury!#n' as you throw it in the air.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yTaste the Windblade of Fury!#n' as $e throws it in the air.",ch,NULL,NULL,TO_ROOM);
    act("A powerful greenish aura radiates off you as your blade hums loudly.",ch,NULL,NULL,TO_CHAR);
    act("A powerful greenish aura radiates off $n as $s blade hums loudly.",ch,NULL,NULL,TO_ROOM);
    act("You then catch the blade beautifully as you point it into the skies.",ch,NULL,NULL,TO_CHAR);
    act("$n then catches the blade beautifully as $e points it into the skies.",ch,NULL,NULL,TO_ROOM);
    act("You scream '#yPOWER SHOT!!!#n' as you release a POWERFUL blast of wind!",ch,NULL,NULL,TO_CHAR);
    act("$n screams '#yPOWER SHOT!!!#n' as $e releases a POWERFUL blast of wind!",ch,NULL,NULL,TO_ROOM);
    act("You stagger back as a torrent of wind RiPs through your flesh!",victim,NULL,NULL,TO_CHAR);
    act("$n staggers back as a torrent of wind RiPs through $s flesh!",victim,NULL,NULL,TO_ROOM);

    ch->move -= 500000;
    ch->ctimer[0] = 1;
    ch->wait = 6;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = AERIAL;
    hurt_person( ch, victim, 500000, TRUE );
}

void do_master_wave( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->move < 1500000 )
       {
          stc("You are too tired to execute this technique atm.\n\r",ch);
          return;
       }
    
    ch->move -= 1500000;
    ch->ctimer[0] = 1;
    ch->wait = 6;

    act("You spread your arms apart as you form an energy blade.",ch,NULL,NULL,TO_CHAR);
    act("$n spreads $s arms apart as $e forms an energy blade.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yTaste the Windblade of Fury!#n' as you throw it in the air.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yTaste the Windblade of Fury!#n' as $e throws it in the air.",ch,NULL,NULL,TO_ROOM);
    act("A powerful greenish aura radiates off you as your blade hums loudly.",ch,NULL,NULL,TO_CHAR);
    act("A powerful greenish aura radiates off $n as $s blade hums loudly.",ch,NULL,NULL,TO_ROOM);
    act("You then catch the blade beautifully as you point it into the skies.",ch,NULL,NULL,TO_CHAR);
    act("$n then catches the blade beautifully as $e points it into the skies.",ch,NULL,NULL,TO_ROOM);
    act("You scream '#yPOWER WAVE!!!#n' as you release a POWERFUL arc of wind!",ch,NULL,NULL,TO_CHAR);
    act("$n screams '#yPOWER WAVE!!!#n' as $e releases a POWERFUL arc of wind!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;
       
       if ( is_safe( ch, vch ) ) 
          continue;

       act("You stagger back as a torrent of wind RiPs through your flesh!",vch,NULL,NULL,TO_CHAR);
       act("$n staggers back as a torrent of wind RiPs through $s flesh!",vch,NULL,NULL,TO_ROOM);

       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = SLASH;
       ch->attack_focus = AERIAL;
       hurt_person( ch, vch, 500000, TRUE );
    }
}

void do_air_blade( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_SET( ch->pcdata->hidden_skill, 1 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[13] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 3500000 )
       {
          stc("You don't have enough magical energy to do this technique.\n\r",ch);
          return;
       }
    
    ch->mana -= 3500000;
    ch->ctimer[13] = 6;
    ch->wait = 6;

    act("You clench your hand as a torrent of wind swirls around it.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as a torrent of wind swirls around it.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yI will rip you all to shreds... Air BLADE!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yI will rip you all to shreds... Air BLADE!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("The area darkens as a series of whirlwinds fill the vicinity.",ch,NULL,NULL,TO_CHAR);
    act("The area darkens as a series of whirlwinds fill the vicinity.",ch,NULL,NULL,TO_ROOM);
    act("Dust and pebbles beneath your feet swirl to the skies as the winds get stronger.",ch,NULL,NULL,TO_CHAR);
    act("Dust and pebbles beneath your feet swirl to the skies as the winds get stronger.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;
       
       if ( is_safe( ch, vch ) ) 
          continue;

       act("You stagger as a torrent of wind BlAsTs you into the skies!",vch,NULL,NULL,TO_CHAR);
       act("$n staggers as a torrent of wind BlAsTs $m into the skies!",vch,NULL,NULL,TO_ROOM);

       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BLAST;
       ch->attack_focus = UPPERCUT;

       hurt_person( ch, vch, 3000000, TRUE );

       if ( vch == NULL || vch->in_room == NULL )
          continue;

       vch->air_timer = 3;
       stop_fighting( vch, TRUE );
    }
}

void do_fangbreaker_blade( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_SET( ch->pcdata->hidden_skill, 2 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[13] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use this on?\n\r",ch);
          return;
       }

    if ( ch->move < 3500000 )
       {
          stc("You are too tired to execute this technique atm.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_world( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
                return;
             }
       }

    if ( is_safe(ch,victim) )
       return;

    act("You spread your arms apart as you form an energy blade.",ch,NULL,NULL,TO_CHAR);
    act("$n spreads $s arms apart as $e forms an energy blade.",ch,NULL,NULL,TO_ROOM);
    act("You say '#yOh mighty wind, show all the power of Justice!#n' as you throw it in the air.",ch,NULL,NULL,TO_CHAR);
    act("$n says '#yOh mighty wind, show all the power of Justice!#n' as $e throws it in the air.",ch,NULL,NULL,TO_ROOM);
    act("A powerful greenish aura radiates off you as your blade hums loudly.",ch,NULL,NULL,TO_CHAR);
    act("A powerful greenish aura radiates off $n as $s blade hums loudly.",ch,NULL,NULL,TO_ROOM);
    act("You then catch the blade beautifully as you dash towards $N!",ch,NULL,victim,TO_CHAR);
    act("$n then catches the blade beautifully as $e dashes towards you!",ch,NULL,victim,TO_VICT);
    act("$n then catches the blade beautifully as $e dashes towards $N!",ch,NULL,victim,TO_NOTVICT);
    act("You scream '#yFang!#n' as you slash violently across $N's chest!",ch,NULL,victim,TO_CHAR);
    act("$n screams '#yFang!#n' as $e slashes violently across your chest!",ch,NULL,victim,TO_VICT);
    act("$n screams '#yFang!#n' as $e slashes violently across $N's chest!",ch,NULL,victim,TO_NOTVICT);
    act("You then scream '#yBreaker!#n' as you twirl your blade up $N's body!",ch,NULL,victim,TO_CHAR);
    act("$n then screams '#yBreaker!#n' as $e twirls $s blade up your body!",ch,NULL,victim,TO_VICT);
    act("$n then screams '#yBreaker!#n' as $e twirls $s blade up $N's body!",ch,NULL,victim,TO_NOTVICT);
    act("You finish off with '#yBlade!!!#n' as you DrIvE your blade into $N's head!!!",ch,NULL,victim,TO_CHAR);
    act("$n finishes off with '#yBlade!!!#n' as $e DrIvEs $s blade into your head!!!",ch,NULL,victim,TO_VICT);
    act("$n finishes off with '#yBlade!!!#n' as $e DrIvEs $s blade into $N's head!!!",ch,NULL,victim,TO_NOTVICT);
    act("#pYou stagger back from the pain as a torrent of deadly #Cwind #psaps your defense.#n",victim,NULL,NULL,TO_CHAR);
    act("#p$n #pstaggers back from the pain as a torrent of deadly #Cwind #psaps $s defense.#n",victim,NULL,NULL,TO_ROOM);

    ch->move -= 3500000;
    ch->ctimer[13] = 6;
    ch->wait = 6;
    SET_BIT( victim->extra, EXTRA_LOWDEF );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = OVERHEAD;
    hurt_person( ch, victim, 3500000, TRUE );
}

void do_breaker( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_SET( ch->pcdata->hidden_skill, 8 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->break_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 3500000 )
       {
          stc("You don't have enough magical energy to do this technique.\n\r",ch);
          return;
       }

    act( "You do a small leap into the air as a manifest of energy engulfs your weapon.", ch, NULL, NULL, TO_CHAR );
    act( "$n does a small leap into the air as a manifest of energy engulfs $s weapon.", ch, NULL, NULL, TO_ROOM );
    act( "You scream '#oBreaker!#n' as you softly land back onto your knees...", ch, NULL, NULL, TO_CHAR );
    act( "$n screams '#oBreaker!#n' as $e softly lands back onto $s knees...", ch, NULL, NULL, TO_ROOM );
    act( "Mass of small vibrations fill the room as you silently watch them.", ch, NULL, NULL, TO_CHAR );
    act( "Mass of small vibrations fill the room as you try in vain to balance yourself.", ch, NULL, NULL, TO_ROOM );
    act( "The ground starts trembling violently as the earth starts cracking open!", ch, NULL, NULL, TO_CHAR );
    act( "The ground starts trembling violently as the earth starts cracking open!", ch, NULL, NULL, TO_ROOM );

    ch->mana -= 3500000;
    ch->break_timer = 40;
    ch->wait = 6;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;
       
       if ( is_safe( ch, vch ) ) 
          continue;

       act( "#oYour body stiffens as you are encased by the #Rmerciless #gearth#o.#n", vch, NULL, NULL, TO_CHAR );
       act( "#o$n#o's body stiffens as $e is encased by the #Rmerciless #gearth#o.#n", vch, NULL, NULL, TO_ROOM );
       vch->stone_timer = 2;
       SET_BIT( vch->more, MORE_STONE );
    }
}

void do_justice_starfist( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    DESCRIPTOR_DATA *d;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_SET( ch->pcdata->hidden_skill, 32 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;

    act("You smile happily as you gaze down at your ever glowing right hand.",ch,NULL,NULL,TO_CHAR);
    act("$n smiles happily as $e gazes down at $s ever glowing right hand.",ch,NULL,NULL,TO_ROOM);
    act("You then clench your fist saying '#yTremble before Justice oh dutiful sinners!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n then clenches $s fist saying '#yTremble before Justice oh dutiful sinners!#n'",ch,NULL,NULL,TO_ROOM);

    act("With your left hand in front of you, you rush straight pass your victims leaping into the air.",ch,NULL,NULL,TO_CHAR);
    act("With $s left hand in front of $m, $n rushes straight pass you as $e dashes leaping into the air.",ch,NULL,NULL,TO_ROOM);
    act("#sS#sp#sa#sr#sk#sl#se#ss#n of #wholy #yenergy#n pale of $n's body as it flows into the distance after $m.",ch,NULL,NULL,TO_ROOM);

    act( "The light intensifies as powerful gravity makes it hard for you to turn.", ch, NULL, NULL, TO_CHAR );
    act( "The light intensifies as powerful gravity makes it hard for $n to turn.", ch, NULL, NULL, TO_ROOM );
    act( "A loud hum fills the air as you focus silently on your target...", ch, NULL, NULL, TO_CHAR );
    act( "A loud hum fills the air as $n focuses silently on you...", ch, NULL, NULL, TO_ROOM );

    act("You scream '#yFeel the WRATH of GOD!#n' as you dive towards the earth!!!",ch,NULL,NULL,TO_CHAR);
    act("$n screams '#yFeel the WRATH of GOD!#n' as $e dives towards the earth!!!",ch,NULL,NULL,TO_ROOM);

    act("The ground trembles violently as your glowing fist goes RiGhT through it!",ch,NULL,NULL,TO_CHAR);
    act("The ground trembles violently as $n's glowing fist goes RiGhT through it!",ch,NULL,NULL,TO_ROOM);

    act("You finally release the huge manifest of energy as INTO THE EARTH!",ch,NULL,NULL,TO_CHAR);
    act("$n finally releases the huge manifest of energy as INTO THE EARTH!",ch,NULL,NULL,TO_ROOM);

    act("#wA HUGE EXPLOSION FROM ABOVE DEVASTATES the vicinity below you, BURSTING everything into OBLIVION!!!#n",ch,NULL,NULL,TO_CHAR);
    act("#wA HUGE EXPLOSION FROM ABOVE DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!#n",ch,NULL,NULL,TO_ROOM);
    SET_BIT( ch->arena, AFF2_POWER );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( !IS_NPC( vch ) || is_safe(ch,vch) )
          continue;

       act("#yYour #wbody evaporates as the explosion #RANNIHILATES #wYOU!#n",d->character,NULL,NULL,TO_CHAR);
       act("#y$n#y's #wbody evaporates as a tremedous explosion #RANNIHILATES #w$m!#n",d->character,NULL,NULL,TO_ROOM);
       ch->attack_type = ATTACK_PHYSICAL;
       ch->ele_attack = ATTACK_HOLY;
       ch->attack_var = BLAST;
       ch->attack_focus = AERIAL;
       hurt_person( ch, vch, 4000000, TRUE );
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	 {
           if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
              continue;

           act("#yYour #wbody evaporates as the explosion #RANNIHILATES #wYOU!#n",d->character,NULL,NULL,TO_CHAR);
           act("#y$n#y's #wbody evaporates as a tremedous explosion #RANNIHILATES #w$m!#n",d->character,NULL,NULL,TO_ROOM);
           ch->attack_type = ATTACK_PHYSICAL;
           ch->ele_attack = ATTACK_HOLY;
           ch->attack_var = BLAST;
           ch->attack_focus = AERIAL;
           hurt_person( ch, d->character, 4000000, TRUE );
       }
    }
}

void do_divine_eradication( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_SET( ch->pcdata->hidden_skill, 128 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->throw_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
    ch->throw_timer = 50;
    SET_BIT( ch->arena, AFF2_POWER );
    SET_BIT( ch->arena, AFF2_UB );

    act( "You strut a little in front of you as you push your right arm aside.", ch, NULL, NULL, TO_CHAR );
    act( "$n struts a little in front of $m as $e pushes $s right arm aside.", ch, NULL, NULL, TO_ROOM );
    act( "You mutter '#wCharging...#n' as everyone gets draw into a greenish vortex.", ch, NULL, NULL, TO_CHAR );
    act( "$n mutters '#wCharging...#n' as everyone gets draw into a greenish vortex.", ch, NULL, NULL, TO_ROOM );
    act( "You jolt back your head as your eyes start glowing with the power of Justice.", ch, NULL, NULL, TO_CHAR );
    act( "$n jolts back $s head as $s eyes starts glowing with the power of Justice.", ch, NULL, NULL, TO_ROOM );
    act( "You then raise your right hand forward as shards of pure light radiates off it.", ch, NULL, NULL, TO_CHAR );
    act( "$n then raises $s right hand forward as shards of pure light radiates off it.", ch, NULL, NULL, TO_ROOM );
    act( "You close your eyes as the light starts merging with your arm slowly.", ch, NULL, NULL, TO_CHAR );
    act( "$n closes $s eyes as the light starts merging with $s arm slowly.", ch, NULL, NULL, TO_ROOM );
    act( "Your arm is now so bright that it is impossible to gaze at.", ch, NULL, NULL, TO_CHAR );
    act( "$n's arm is now so bright that it is impossible to gaze at.", ch, NULL, NULL, TO_ROOM );
    act( "You yell '#wCharge!#n' as you RELEASE A TREMEDOUS BEAM OF HOLY ENERGY!!!!!", ch, NULL, NULL, TO_CHAR );
    act( "$n yells '#wCharge!#n' as $e RELEASES A TREMEDOUS BEAM OF HOLY ENERGY!!!!!", ch, NULL, NULL, TO_ROOM );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( is_safe( ch, vch ) )
          continue;

       act( "#cYou #wdon't even get time to #Rscream #was the beam #eERADICATES #wyou!#n", vch, NULL, NULL, TO_CHAR );
       act( "#c$n #wdoesn't even get time to #Rscream #was the beam #eERADICATES #w$m!#n", vch, NULL, NULL, TO_ROOM );

       ch->ele_attack = ATTACK_HOLY;
       ch->attack_type = ATTACK_PHYSICAL;
       ch->attack_var = BEAM;
       ch->attack_focus = DIRECT;
       hurt_person( ch, vch, 5000000, TRUE );
    }

    REMOVE_BIT( ch->arena, AFF2_UB );
}

void do_whirlwind_crusher( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_SET( ch->pcdata->hidden_skill, 2 ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->ctimer[13] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use this on?\n\r",ch);
          return;
       }

    if ( ch->move < 3500000 )
       {
          stc("You are too tired to execute this technique atm.\n\r",ch);
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_world( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
                return;
             }
       }

    if ( is_safe(ch,victim) )
       return;

    act("You spread your arms apart as you form an energy blade.",ch,NULL,NULL,TO_CHAR);
    act("$n spreads $s arms apart as $e forms an energy blade.",ch,NULL,NULL,TO_ROOM);
    act("You say '#yOh mighty wind, may you crush all!#n' as you throw it in the air.",ch,NULL,NULL,TO_CHAR);
    act("$n says '#yOh mighty wind, may you crush all!#n' as $e throws it in the air.",ch,NULL,NULL,TO_ROOM);
    act("A powerful greenish aura radiates off you as your blade hums loudly.",ch,NULL,NULL,TO_CHAR);
    act("A powerful greenish aura radiates off $n as $s blade hums loudly.",ch,NULL,NULL,TO_ROOM);
    act("You then catch the blade beautifully as you dash towards $N!",ch,NULL,victim,TO_CHAR);
    act("$n then catches the blade beautifully as $e dashes towards you!",ch,NULL,victim,TO_VICT);
    act("$n then catches the blade beautifully as $e dashes towards $N!",ch,NULL,victim,TO_NOTVICT);
    act("You skid onto your knees as you swerve your free hand onto your blade...",ch,NULL,NULL,TO_CHAR);
    act("$n skids onto $s knees as $e swerves $s free hand onto $s blade...",ch,NULL,NULL,TO_ROOM);
    act("You yell '#yWhirlwind Crusher!#n' as you flash by $N brilliantly!",ch,NULL,victim,TO_CHAR);
    act("$n yells '#yWhirlwind Crusher!#n' as $e flashes by you brilliantly!",ch,NULL,victim,TO_VICT);
    act("$n yells '#yWhirlwind Crusher!#n' as $e flashes by $N brilliantly!",ch,NULL,victim,TO_NOTVICT);
    act("You scream in pain as violent flashes of light RiPs through your body!!",victim,NULL,NULL,TO_CHAR);
    act("$n screams in pain as violent flashes of light RiPs through $s body!!",victim,NULL,NULL,TO_ROOM);
    act("#pYou stagger back from the pain as a torrent of deadly #Cwind #psaps your strength.#n",victim,NULL,NULL,TO_CHAR);
    act("#p$n #pstaggers back from the pain as a torrent of deadly #Cwind #psaps $s strength.#n",victim,NULL,NULL,TO_ROOM);

    ch->move -= 3500000;
    ch->ctimer[13] = 6;
    ch->wait = 6;
    SET_BIT( victim->extra, EXTRA_LOWOFF );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = DIRECT;
    hurt_person( ch, victim, 3500000, TRUE );
}

void do_vampire_fang( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->alignment > -750 )
       {
          stc("#wOnly #ygood #wweapons master can execute this technique.#n\n\r",ch);
          return;
       }

    if ( ch->ctimer[6] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use this on?\n\r",ch);
          return;
       }

    if ( ch->move < 1500000 )
       {
          stc("You are too tired to execute this technique atm.\n\r",ch);
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

    act("You clench your hand as it explodes with demonic energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it explodes with demonic energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yI shall devour your soul...#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yI shall devour your soul...#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("A glowing blade manifests into your hand as you dash towards $N!",ch,NULL,victim,TO_CHAR);
    act("A glowing blade manifests into $n's hand as $e dashes towards you!",ch,NULL,victim,TO_VICT);
    act("A glowing blade manifests into $n's hand as $e dashes towards $N!",ch,NULL,victim,TO_NOTVICT);
    act("You RaPiDlY slice through $N's chest as you leech away at $S energy!",ch,NULL,victim,TO_CHAR);
    act("$n RaPiDlY slices through your chest as $e leeches away at your energy!",ch,NULL,victim,TO_VICT);
    act("$n RaPiDlY slices through $N's chest as $e leeches away at $S energy!",ch,NULL,victim,TO_NOTVICT);
    act("You RaPiDlY slice through $N's chest as you leech away at $S energy!",ch,NULL,victim,TO_CHAR);
    act("$n RaPiDlY slices through your chest as $e leeches away at your energy!",ch,NULL,victim,TO_VICT);
    act("$n RaPiDlY slices through $N's chest as $e leeches away at $S energy!",ch,NULL,victim,TO_NOTVICT);
    act("You RaPiDlY slice through $N's chest as you leech away at $S energy!",ch,NULL,victim,TO_CHAR);
    act("$n RaPiDlY slices through your chest as $e leeches away at your energy!",ch,NULL,victim,TO_VICT);
    act("$n RaPiDlY slices through $N's chest as $e leeches away at $S energy!",ch,NULL,victim,TO_NOTVICT);
    act("You then draw back your weapon as shards of light radiates off it...",ch,NULL,NULL,TO_CHAR);
    act("$n then draws back $s weapon as shards of light radiates off it...",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yTake this!#n' as you DrIvE your blade through $N's heart!",ch,NULL,victim,TO_CHAR);
    act("$n snarls '#yTake this!#n' as $e DrIvEs $s blade through your heart!",ch,NULL,victim,TO_VICT);
    act("$n snarls '#yTake this!#n' as $e DrIvEs $s blade through $N's heart!",ch,NULL,victim,TO_NOTVICT);

    ch->move -= 1500000;
    ch->ctimer[6] = 6;
    ch->wait = 6;

    victim->hit -= 1500000;
    hit_gain( ch, 1000000 );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = PIERCE;
    ch->attack_focus = OVERHEAD;
    hurt_person( ch, victim, 0, TRUE );
}

void do_sigma_rage( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    AFFECT_DATA af;
    int option, count, sn;
    int maxcount = ( number_percent() > 69 ) ? 2 : 1;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 25 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->alignment < 750 )
       {
          stc("#wOnly #ygood #wweapons master can execute this technique.#n\n\r",ch);
          return;
       }

    if ( ch->fight_timer == 0 )
       {
          stc("You can't do this technique without a fight timer.\n\r",ch);
          return;
       }

    if ( ch->mana < 3500000 )
       {
          stc("You are too tired to execute this technique atm?\n\r",ch);
          return;
       }
    
    ch->mana -= 3500000;
    ch->wait = 6;

    act("You clench your hand as it explodes with fiery energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it explodes with fiery energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yWeapons conquer all... Sigma Rage!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yWeapons conquer all... Sigma Rage!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("You then push your glowing hand above your head as the room explodes!!!",ch,NULL,NULL,TO_CHAR);
    act("$n then pushes $s glowing hand above $s head as the room explodes!!!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_NPC(vch) )
          continue;

       if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom > 0 ) )
          continue;

       act("You brace yourself as the flames fuel your power.",vch,NULL,NULL,TO_CHAR);
       act("$n braces $mself as the flames fuels $s power!",vch,NULL,NULL,TO_ROOM);

       for ( count = 0; count < maxcount; count++ )
       {
          switch( number_range( 1, 8 ) )
          {
             default:
             case 1: sn = skill_lookup( "holy seal" );
                     act( "#yYou #wcan now absorb holy #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb holy #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 2: sn = skill_lookup( "demonic seal" );
                     act( "#yYou #wcan now absorb #edark #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #edark #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 3: sn = skill_lookup( "fiery seal" );
                     act( "#yYou #wcan now absorb #Rfiery #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #Rfiery #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 4: sn = skill_lookup( "icy seal" );
                     act( "#yYou #wcan now absorb #Cicy attacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #Cicy attacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 5: sn = skill_lookup( "thunder seal" );
                     act( "#yYou #wcan now absorb #ythunder #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #ythunder #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 6: sn = skill_lookup( "earth seal" );
                     act( "#yYou #wcan now absorb #gearth #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #gearth #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 7: sn = skill_lookup( "natural seal" );
                     act( "#yYou #wcan now absorb #Pnatural #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #Pnatural #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
             case 8: sn = skill_lookup( "lunar seal" );
                     act( "#yYou #wcan now absorb #olunar #Cattacks#w!#n", vch, NULL, NULL, TO_CHAR );
                     act( "#y$n #wcan now absorb #olunar #Cattacks#w!#n", vch, NULL, NULL, TO_ROOM );
                     break;
          }

          af.type      = sn;
          af.duration  = 5;
          af.modifier  = 0;
          af.location  = option;
          af.bitvector = 0;
          affect_to_char( vch, &af );
       }
    }
}

void do_speed_up( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    AFFECT_DATA af;
    int sn;

    if ( ( sn = skill_lookup( "speed up" ) ) < 0 )
       return;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) || ch->clannum != 1 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 20 )
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

    act("You clench your hand as a torrent of wind swirls around it.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as a torrent of wind swirls around it.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yThose who strike first usually win... Speed... Up...!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yThose who strike first usually win... Speed... Up...!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("You then LeAp high into the air as you spread your arms apart.",ch,NULL,NULL,TO_CHAR);
    act("$n then LeApS high into the air as $e spreads $s arms apart.",ch,NULL,NULL,TO_ROOM);
    act("Shards of light radiates off your body as the room goes blinding white!!!",ch,NULL,NULL,TO_CHAR);
    act("Shards of light radiates off $n's body as the room goes blinding white!!!",ch,NULL,NULL,TO_ROOM);

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

       act("You cackle with glee as you move like the wind.",vch,NULL,NULL,TO_CHAR);
       act("$n cackles with glee as $e moves like the wind!",vch,NULL,NULL,TO_ROOM);

       af.type      = sn;
       af.duration  = 50;
       af.modifier  = 10;
       af.location  = APPLY_AGI;
       af.bitvector = 0;
       affect_to_char( vch, &af );
    }
}

void do_raging_soul( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;
    AFFECT_DATA af;
    int sn, option;

    if ( ( sn = skill_lookup( "raging soul" ) ) < 0 )
       return;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 15 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->mana < 2500000 )
       {
          stc("You don't have enough magical energy to do this technique.\n\r",ch);
          return;
       }
    
    ch->mana -= 2500000;
    ch->wait = 4;

    act("You clench your hand as it explodes with fiery energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it explodes with fiery energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yGive us the heart to kill all... Raging Soul!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yGive us the heart to kill all... Raging Soul!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);
    act("The area brightens as the ground trembles beneath you.",ch,NULL,NULL,TO_CHAR);
    act("The area brightens as the ground trembles beneath you.",ch,NULL,NULL,TO_ROOM);
    act("Dust and pebbles beneath your feet swirl to the skies as flames sprout up.",ch,NULL,NULL,TO_CHAR);
    act("Dust and pebbles beneath your feet swirl to the skies as flames sprout up.",ch,NULL,NULL,TO_ROOM);

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

       act("You brace yourself as the flames fuel your power.",vch,NULL,NULL,TO_CHAR);
       act("$n braces $mself as the flames fuels $s power!",vch,NULL,NULL,TO_ROOM);

       vch->blocking = -10;
       af.type      = sn;

       if ( vch == ch )
          {
             af.duration  = 5;

             if ( ch->clannum == 1 )
                af.modifier  = 15;
                else af.modifier  = 15;
          }
          else {
                  af.duration  = 24;

                  if ( ch->clannum == 1 )
                     af.modifier = 15;
                     else af.modifier = 5;
               }

       af.location  = APPLY_STR;
       af.bitvector = 0;
       affect_to_char( vch, &af );

       if ( vch != ch )
          continue;

       af.type      = sn;
       af.duration  = 5;

       if ( ch->clannum == 1 )
          af.modifier  = 15;
          else af.modifier  = 15;

       af.location  = APPLY_DEX;
       af.bitvector = 0;
       affect_to_char( vch, &af );

       af.type      = sn;
       af.duration  = 5;

       if ( ch->clannum == 1 )
          af.modifier  = 15;
          else af.modifier  = 15;

       af.location  = APPLY_EVA;
       af.bitvector = 0;
       affect_to_char( vch, &af );

       af.type      = sn;
       af.duration  = 5;

       if ( ch->clannum == 1 )
          af.modifier  = 15;
          else af.modifier  = 15;

       af.location  = APPLY_INT;
       af.bitvector = 0;
       affect_to_char( vch, &af );

       af.type      = sn;
       af.duration  = 5;

       if ( ch->clannum == 1 )
          af.modifier  = 15;
          else af.modifier  = 15;

       af.location  = APPLY_WIS;
       af.bitvector = 0;
       affect_to_char( vch, &af );

       af.type      = sn;
       af.duration  = 5;

       if ( ch->clannum == 1 )
          af.modifier  = 15;
          else af.modifier  = 15;

       af.location  = APPLY_CON;
       af.bitvector = 0;
       affect_to_char( vch, &af );

       switch( number_range( 1, 8 ) )
       {
          default:
          case 1: option = APPLY_ICES; break;
          case 2: option = APPLY_BOLTS; break;
          case 3: option = APPLY_HOLYS; break;
          case 4: option = APPLY_FIRES; break;
          case 5: option = APPLY_EARTHS; break;
          case 6: option = APPLY_DARKS; break;
          case 7: option = APPLY_LUNARS; break;
          case 8: option = APPLY_NATURALS; break;
       }

       af.type      = sn;
       af.duration  = 5;

       if ( ch->clannum == 1 )
          af.modifier  = 100;
          else af.modifier  = 100;

       af.location  = option;
       af.bitvector = 0;
       affect_to_char( vch, &af );
    }
}

void do_entice( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[2] > 0 )
       {
          stc("You can't do this skill for awhile weapons master.\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
         && ch->ctimer[9] != 100 ) )
       ch->ctimer[2] = 1;

    ch->wait = 6;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 100 );

    act("You clench your hand as it explodes with fiery energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it explodes with fiery energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yForce all to do their best... Entice!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yForce all to do their best... Entice!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_NPC(vch) )
          continue;

       if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom > 0 ) )
          continue;

       act("You fold your arms proudly as you can do another armour skill.",vch,NULL,NULL,TO_CHAR);
       act("$n folds $s arms proudly as $e can do another armour skill!",vch,NULL,NULL,TO_ROOM);
       vch->saiyan_timer = 0;
    }
}

void do_riposte( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[3] > 0 )
       {
          stc("You can't do this skill for awhile weapons master.\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
         && ch->ctimer[9] != 100 ) )
       ch->ctimer[3] = 1;

    ch->wait = 6;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 200 );

    act("You clench your hand as it explodes with fiery energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it explodes with fiery energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yMove like the wind of Fury... Riposte!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yMove like the wind of Fury... Riposte!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_NPC(vch) )
          continue;

       if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom > 0 ) )
          continue;

       act("You fold your arms proudly as you can do another counter skill.",vch,NULL,NULL,TO_CHAR);
       act("$n folds $s arms proudly as $e can do another counter skill!",vch,NULL,NULL,TO_ROOM);
       vch->aragami_timer = 0;
    }
}

void do_forestall( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch, *vch_next;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[4] > 0 )
       {
          stc("You can't do this skill for awhile weapons master.\n\r",ch);
          return;
       }

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( ch->ctimer[15] > 0 || ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25
         && ch->ctimer[9] != 100 ) )
       ch->ctimer[4] = 1;

    ch->wait = 6;
    SET_BIT( ch->arena, AFF2_DM );
    subtract_stock( ch, 300 );

    act("You clench your hand as it explodes with fiery energy.",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s hand as it explodes with fiery energy.",ch,NULL,NULL,TO_ROOM);
    act("You snarl '#yLet your foes know their last mistake... Forestall!!!#n' as you open it.",ch,NULL,NULL,TO_CHAR);
    act("$n snarls '#yLet your foes know their last mistake... Forestall!!!#n' as $e opens it.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_NPC(vch) )
          continue;

       if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
            && ch->pcdata->kingdom > 0 ) )
          continue;

       act("You fold your arms proudly as you can do more attacking skills.",vch,NULL,NULL,TO_CHAR);
       act("$n folds $s arms proudly as $e can do more attacking skills!",vch,NULL,NULL,TO_ROOM);
       vch->achi = 0;
       vch->throw_timer = 0;
    }
}

void do_fadedstrike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int dam = 1000000;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MASTER) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 5 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( ch->move < 500000 )
       {
          stc("You are too tired to do this!\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use this strike on?\n\r",ch);
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

    act("You punch the air in front of you viciously as pitch black energy manifests into existence.",ch,NULL,NULL,TO_CHAR);
    act("$n punches the air in front of $m viciously as pitch black energy manifests into existence.",ch,NULL,NULL,TO_ROOM);
    act("You keep on punching the air in front of you as the ball of energy grows bigger and bigger.",ch,NULL,NULL,TO_CHAR);
    act("$n keeps on punching the air in front of $m as the ball of energy grows bigger and bigger.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly the ball flaShEs towards $N causing him to stagger backwards.",ch,NULL,victim,TO_CHAR);
    act("Suddenly the ball flaShEs towards you making you stagger backwards.",ch,NULL,victim,TO_VICT);
    act("Suddenly the ball flaShEs towards $N causing him to stagger backwards.",ch,NULL,victim,TO_NOTVICT);
    act("#eYour shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_CHAR);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg you hard in the guts.#n",ch,NULL,victim,TO_VICT);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_NOTVICT);
    act("#eYour shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_CHAR);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg you hard in the guts.#n",ch,NULL,victim,TO_VICT);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_NOTVICT);
    act("#eYour shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_CHAR);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg you hard in the guts.#n",ch,NULL,victim,TO_VICT);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_NOTVICT);
    act("#eYour shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_CHAR);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg you hard in the guts.#n",ch,NULL,victim,TO_VICT);
    act("#e$n's shadow suddenly leaps from the ground StRiKiNg $N hard in the guts.#n",ch,NULL,victim,TO_NOTVICT);
    act("#eYour shadow suddenly leaps from the ground SmAsHiNg $N onto the ground.#n",ch,NULL,victim,TO_CHAR);
    act("#e$n's shadow suddenly leaps from the ground SmAsHiNg you onto the ground.#n",ch,NULL,victim,TO_VICT);
    act("#e$n's shadow suddenly leaps from the ground SmAsHiNg $N onto the ground.#n",ch,NULL,victim,TO_NOTVICT);

    ch->wait = 18;
    ch->move -= 500000;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = DIRECT;

    hurt_person(ch,victim,dam,TRUE);
}

void do_reflection( CHAR_DATA *ch, char *argument )
{
    int sn;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) || ch->clannum != 2 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->mana < 10000 )
       {
	  stc("You need at least 10000 mana to use this power.\n\r", ch);
	  return;
       }

    if ( ( sn = skill_lookup( "divine reflection" ) ) < 0 ) return;

    if ( is_affected(ch,skill_lookup("divine reflection") ) )
       {
          stc("You are already surrounded by a white outline!\n\r",ch);
          return;
       }

    (*skill_table[sn].spell_fun) ( sn, 10, ch, ch );
    WAIT_STATE( ch, 12 );
    ch->mana -= 10000;
}

void do_divineray( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
 
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 20 )
       {
          stc("You aren't powerful enough to perform the Divine Ray.\n\r",ch);
          return;
       }

    if ( ch->ctimer[1] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    if ( ch->mana < 2500000 )
       {
          stc("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          stc("Who do you wish to use Divine Ray on?\n\r",ch);
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

    if ( IS_CLASS( victim, CLASS_MONK ) || IS_CLASS( victim, CLASS_ANGEL ) || IS_CLASS( victim, CLASS_PALADIN ) )
       {
          stc("You can't use this against your holy brothers.\n\r",ch);
          return;
       }

    if ( is_safe( ch, victim ) ) 
       return;

    stc("\n\rYou take out the Holy Book and start flipping through the ancient pages.\n\r",ch);
    stc("As you start reading the divine text, your right hand start glowing with holy power as you raise it in the air.\n\r",ch);
    act("Suddenly, you close the book as you point your glowing hand at $N.",ch,NULL,victim,TO_CHAR);
    act("'#y$N#y... Feel the Wrath of God!#n', you shout as $N is surrounded by a shimmering outline.\n\r",ch,NULL,victim,TO_CHAR);

    act("\n\r$n takes out the Holy Book and starts flipping through the ancient pages.",ch,NULL,NULL,TO_ROOM);
    act("As $e starts reading the divine text, $s right hand start glowing with holy power as $e raise it in the air.",ch,NULL,NULL,TO_ROOM);

    act("Suddenly, $e closes the book as $e points $s glowing hand at you!",ch,NULL,victim,TO_VICT);
    act("Suddenly, $e closes the book as $e points $s glowing hand at $N!",ch,NULL,victim,TO_NOTVICT);
    act("'#y$N#y... Feel the Wrath of God!#n', $n shouts as you are surrounded by a shimmering outline.\n\r",ch,NULL,victim,TO_VICT);
    act("'#y$N#y... Feel the Wrath of God!#n', $n shouts as $N is surrounded by a shimmering outline.\n\r",ch,NULL,victim,TO_NOTVICT);

    act("A huge beam of Holy Energy descends from the heaven SMASHING into $n, causing the entire room to shake violently!",victim,NULL,NULL,TO_ROOM);
    act("As the holy explosion completely fills the room, $n collapses on the ground screaming from the unbelievable pain.\n\r",victim,NULL,NULL,TO_ROOM);

    act("A huge beam of Holy Energy descends from the heaven SMASHING into you!",victim,NULL,NULL,TO_CHAR);
    act("You lose control of your nervous system as you scream from the unbelievable pain.\n\r",victim,NULL,NULL,TO_CHAR);

    ch->mana -= 2500000;
    victim->mana = UMAX( 0, victim->mana - 1500000 );
    victim->move = UMAX( 0, victim->move - 1500000 );

    if ( !is_affected( victim, skill_lookup("blessing of the saints") ) )
       spell_infirmity( skill_lookup("weaken"), 4, ch, victim );

    ch->wait = 6;
    ch->ctimer[1] = 4;

    ch->attack_type = ATTACK_MAGICAL;
    ch->ele_attack = ATTACK_HOLY;
    ch->attack_var = SOUL;
    ch->attack_focus = AERIAL;
    hurt_person( ch, victim, 500000, TRUE );
}
