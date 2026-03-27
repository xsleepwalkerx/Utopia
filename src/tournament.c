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

void do_talk( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
 
   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) return;

   if ( arg[0] == '\0' )
      {
         stc("Whom do you wish to talk to?\n\r",ch);
         return;
      }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         stc("They aren't here.\n\r",ch);
         return;
      }

   if ( !IS_NPC( victim ) )
      {
         stc("You can only talk to mobs.\n\r",ch);
         return;
      }

   if ( victim->pIndexData->vnum == 500 )
      {
         int num = 0;        

         if ( (get_age(ch)-17) < 2 )
            {
               act( "The Enrollment Officer looks at you annoyed as he says '#yDude, Newbies aren't allowed in.#n'", ch, NULL, NULL, TO_CHAR );
               act( "The Enrollment Officer looks at $n annoyed as he says '#yDude, Newbies aren't allowed in.#n'", ch, NULL, NULL, TO_ROOM );
               return;
            }

         if ( IS_SET( ch->arena, AFF2_COMPETITOR ) )
            {
               act( "The Enrollment Officer looks at you annoyed as he says '#yDude, you are already in.#n'", ch, NULL, NULL, TO_CHAR );
               act( "The Enrollment Officer looks at $n annoyed as he says '#yDude, you are already in.#n'", ch, NULL, NULL, TO_ROOM );
               return;
            }

         if ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) )
            {
               act( "The Enrollment Officer looks at you annoyed as he says '#yDude, you can't be in both a team AND kof.#n'", ch, NULL, NULL, TO_CHAR );
               act( "The Enrollment Officer looks at $n annoyed as he says '#yDude, you can't be in both a team AND kof.#n'", ch, NULL, NULL, TO_ROOM );
               return;
            }

         if ( num_contestants == 15 && arena != FIGHT_CLEAR )
            {
               act( "The Enrollment Officer says solemnly '#yStupid people in the arena... Can you wait a few minutes please?#n'", ch, NULL, NULL, TO_CHAR );
               act( "The Enrollment Officer says solemnly '#yStupid people in the arena... Can you wait a few minutes please?#n'", ch, NULL, NULL, TO_ROOM );
               return;
            }

         if ( num_contestants == 16 || arena != FIGHT_CLEAR )
            {
               act( "The Enrollment Officer says solemnly '#yAll of the entries in the tournament had already been taken.#n'", ch, NULL, NULL, TO_CHAR );
               act( "The Enrollment Officer says solemnly '#yAll of the entries in the tournament had already been taken.#n'", ch, NULL, NULL, TO_ROOM );
               act( "The Enrollment Officer continues '#yWhy not go inside and enjoy the show?#n'", ch, NULL, NULL, TO_CHAR );
               act( "The Enrollment Officer continues '#yWhy not go inside and enjoy the show?#n'", ch, NULL, NULL, TO_ROOM );
               return;
            }
   
         act( "The Enrollment Officer sighs as he looks at you and say '#yOh Boy... Alright, here we go.#n'", ch, NULL, NULL, TO_CHAR );
         act( "The Enrollment Officer sighs as he looks at $n and say '#yOh Boy... Alright, here we go.#n'", ch, NULL, NULL, TO_ROOM );
         act( "Snapping his fingers, a huge punching machine shimmers into existence before you.", ch, NULL, NULL, TO_CHAR );
         act( "Snapping his fingers, a huge punching machine shimmers into existence before you.", ch, NULL, NULL, TO_ROOM );
         act( "Leaning against the machine, the Officer says '#yPunch this as hard as you can.#n'", ch, NULL, NULL, TO_CHAR );
         act( "Leaning against the machine, the Officer says '#yPunch this as hard as you can.#n'", ch, NULL, NULL, TO_ROOM );
         act( "You crouch on your knee as you gather all of your strength together.", ch, NULL, NULL, TO_CHAR );
         act( "$n crouches on $s knee as $e gathers all of $s strength together.", ch, NULL, NULL, TO_ROOM );
         act( "You scream loudly as you PuNcH the machine's guard with ALL of your MiGhT!!!", ch, NULL, NULL, TO_CHAR );
         act( "$n screams loudly as $e PuNcHeS the machine's guard with ALL of $s MiGhT!!!", ch, NULL, NULL, TO_ROOM );

         if ( ch->generation < 5 )
            num += 6 - ch->generation;

         num += ( ch->tier * 10 );
         num += ch->race;
         num += ( ch->pcdata->clanrank - 1 ) * 5;
         num += ch->pkill;
         num += ( ch->pcdata->legend * 2 );

         num = UMAX( 0, num );

         sprintf( arg, "\n\rThe machine shudders from the blow as the number #w%d#n flashes on a LCD screen on it.", num );
         act( arg, ch, NULL, NULL, TO_CHAR );
         act( arg, ch, NULL, NULL, TO_ROOM );

         if ( num < 5 )
            {
               act( "Patting you on the back, the Officer says '#ySorry Chump. Better luck next year.#n'", ch, NULL, NULL, TO_CHAR );
               act( "Patting $n on the back, the Officer says '#ySorry Chump. Better luck next year.#n'", ch, NULL, NULL, TO_ROOM );
               return;
            }

         act( "The Officer's eyes bulge in surprise as he gasps '#yHoly ^*&%. Finally, a strong dude.#n'", ch, NULL, NULL, TO_CHAR );
         act( "The Officer's eyes bulge in surprise as he gasps '#yHoly ^*&%. Finally, a strong dude.#n'", ch, NULL, NULL, TO_ROOM );
         act( "You frown disapprovingly as the Officer takes a good look closer at you.", ch, NULL, NULL, TO_CHAR );
         act( "$n frowns disapprovingly as the Officer takes a good look closer at $m.", ch, NULL, NULL, TO_ROOM );

         act( "The Officer nod as he finishes '#yExcellent Kid. Good luck in your upcoming battles.#n'", ch, NULL, NULL, TO_CHAR );
         act( "The Officer nod as he finishes '#yExcellent Kid. Good luck in your upcoming battles.#n'", ch, NULL, NULL, TO_ROOM );
         act( "Slapping you hard on the back, the Officer says '#yGo get 'em!#n'", ch, NULL, NULL, TO_CHAR );
         act( "Slapping $n hard on the back, the Officer says '#yGo get 'em!#n'", ch, NULL, NULL, TO_ROOM );

         num_contestants++;
         num = 0;

         while ( participants[ num ] != NULL )
           num++;

         participants[ num ] = ch;
         ch->round = 0;

         SET_BIT( ch->arena, AFF2_COMPETITOR );

         sprintf( arg, "#y%s #whas just entered the #CKing #wof #RFighters #PTournament#w!!!#n", ch->pcdata->switchname );
         do_info( ch, arg );

         if ( num_contestants == 16 )
            do_initialize_tournament( ch );

         return;
      }
}

void do_tinfo( CHAR_DATA *ch, CHAR_DATA *victim, char *argument )
{
   DESCRIPTOR_DATA *d;

   if ( argument[0] == '\0' )
      return;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && !IS_SET(d->character->deaf, CHANNEL_INFO) )
         {
            if ( ch != NULL && d->character == ch )
               continue;

            if ( victim != NULL && d->character == victim )
               continue;

            send_to_char( "#g[#y<#wTournament#y>#g]#p ",d->character );
            send_to_char( argument, d->character );
            send_to_char( " #g[#y<#wTournament#y>#g]#n\n\r", d->character );
         }
   }
}

void do_announce_fight( CHAR_DATA *ch )
{
   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      return;

   act( "The Seraph slashes her arms to the right as she shouts '#ySurvive!#n'", ch, NULL, NULL, TO_CHAR );
   ch->arena_timer = 50;
}

void undo_fight( CHAR_DATA *ch )
{
   CHAR_DATA *victim;
   char name[MAX_STRING_LENGTH]; 
   int chit, cmhit, vhit, vmhit;
 
   if ( ch->challenged == NULL )
      return;

   victim = ch->challenged;

   if ( ch->max_hit > 999999 )
      {
         chit = ch->hit;
         cmhit = ch->max_hit / 100;
      }
      else {
              chit = ch->hit * 100;
              cmhit = ch->max_hit;
           }

   if ( victim->max_hit > 999999 )
      {
         vhit = victim->hit;
         vmhit = victim->max_hit / 100;
      }
      else {
              vhit = victim->hit * 100;
              vmhit = victim->max_hit;
           }

   if ( ch->hit == -10 )
      clean_arena( ch, victim );
      else if ( vhit / vmhit > chit / cmhit )
              clean_arena( ch, victim );
              else clean_arena( victim, ch );
}

void do_give_up( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_INPUT_LENGTH];

   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      {
         stc( "But you aren't in an arena match.\n\r", ch );
         return;
      }

   act( "#wYou #ggo down on your knees as you #ccover #gyour face with #eshame#g.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n #ggoes down on $s knees as $e #ccovers #g$s face with #eshame#g.#n", ch, NULL, NULL, TO_ROOM );
   act( "#wYou #rmutter '#yI give up...#r' as you throw in the #Ctowel#r.#n", ch, NULL, NULL, TO_CHAR );
   act( "#w$n #rmutters '#yI give up...#r' as $e throws in the #Ctowel#r.#n", ch, NULL, NULL, TO_ROOM );

   if ( !IS_NPC( ch ) )
      sprintf( buf, "#c%s #rhas #wgiven #eup #rthe Match!", ch->name );
      else sprintf( buf, "#c%s #rhas #wgiven #eup #rthe Match!", ch->short_descr );

   do_tinfo( ch, NULL, buf );

   SET_BIT( ch->arena, AFF2_GIVEUP );
   ch->hit = -10;
   undo_fight( ch );
   REMOVE_BIT( ch->arena, AFF2_GIVEUP );
}

void do_announce_round( CHAR_DATA *ch )
{
   if ( !IS_SET( ch->arena, AFF2_INARENA ) )
      return;

   act( "The air vibrates softly as an image of a beautiful seraph forms into existence.", ch, NULL, NULL, TO_CHAR );
   act( "She rotates her hand 90 degrees to her face in a salute as she says '#yGet Ready.#n'", ch, NULL, NULL, TO_CHAR );
}

void do_announce_match( void )
{
   CHAR_DATA *competitor, *opponent;
   char buf[MAX_INPUT_LENGTH];
   char *cname, *oname;

   battle_num++;

   competitor = NULL;
   opponent = NULL;

   if ( battle_num < 9 )
      {
         int count = ( battle_num - 1 ) * 2;

         competitor = participants[count];
         cname = str_dup( battle_name[count] );

         opponent = participants[count + 1];
         oname = str_dup( battle_name[count + 1] );
      }
      else if ( battle_num < 13 )
      {
         int count = ( battle_num - 9 ) * 4;
         int counter;

         for ( counter = count; counter < count + 2; counter++ )
         {
            if ( participants[counter] == NULL || participants[counter]->round != 2 )
               continue;

            competitor = participants[counter];
            cname = str_dup( battle_name[counter] );
            break;
         }

         for ( counter = count + 2; counter < count + 4; counter++ )
         {
            if ( participants[counter] == NULL || participants[counter]->round != 2 )
               continue;

            opponent = participants[counter];
            oname = str_dup( battle_name[counter] );
            break;
         }
      }
      else if ( battle_num < 15 )
      {
         int count = ( battle_num - 13 ) * 8;
         int counter;

         for ( counter = count; counter < count + 4; counter++ )
         {
            if ( participants[counter] == NULL || participants[counter]->round != 3 )
               continue;

            competitor = participants[counter];
            cname = str_dup( battle_name[counter] );
            break;
         }

         for ( counter = count + 4; counter < count + 8; counter++ )
         {
            if ( participants[counter] == NULL || participants[counter]->round != 3 )
               continue;

            opponent = participants[counter];
            oname = str_dup( battle_name[counter] );
            break;
         }
      }
      else if ( battle_num >= 15 )
      {
         int count = 0;
         int counter;

         for ( counter = count; counter < count + 8; counter++ )
         {
            if ( participants[counter] == NULL || participants[counter]->round != 4 )
               continue;

            competitor = participants[counter];
            cname = str_dup( battle_name[counter] );
            break;
         }

         for ( counter = count + 8; counter < count + 16; counter++ )
         {
            if ( participants[counter] == NULL || participants[counter]->round != 4 )
               continue;

            opponent = participants[counter];
            oname = str_dup( battle_name[counter] );
            break;
         }
      }

   if ( opponent == NULL && competitor != NULL )
      {
         sprintf( buf, "#CAs #y%s #ghas no opponent, they #P#zautomatically#n #gadvances to the #wnext #Rround#g.", cname );
         do_tinfo( NULL, NULL, buf );
         competitor->round++;
         arena = FIGHT_INITWAIT;

         if ( competitor->round == 5 )
            show_winner( competitor );

         return;
      }
      else if ( opponent != NULL && competitor == NULL )
      {
         sprintf( buf, "#CAs #y%s #ghas no opponent, they #P#zautomatically#n #gadvances to the #wnext #Rround#g.", oname );
         do_tinfo( NULL, NULL, buf );
         opponent->round++;
         arena = FIGHT_INITWAIT;

         if ( opponent->round == 5 )
            show_winner( opponent );

         return;
      }
      else if ( opponent == NULL && competitor == NULL )
      {
         sprintf( buf, "#CAs #gthere are #eno #Pcompetitors #gfor this slot, it is a #wcancelled #rmatch#g." );
         do_tinfo( NULL, NULL, buf );
         arena = FIGHT_INITWAIT;

         if ( battle_num == 15 )
            {
               sprintf( buf, "#PSince #wthis #ewas #wthe #CChampionship #RFight#w, there is #R#zno#n #LWorld #yChampion#g." );
               do_tinfo( NULL, NULL, buf );
               arena = FIGHT_CLEAR;
            }

         return;
      }

   arena = FIGHT_ROUND;
 
   if ( battle_num == 1 )
      sprintf( buf, "#rThe #wfirst #ymatch #gin the #Ctournament #eis... #R%s #wVs #L%s#g!#n", cname, oname );
      else if ( battle_num != 15 )
              sprintf( buf, "#rThe #Pnext #ymatch #gin the #Ctournament #eis... #R%s #wVs #L%s#g!#n", cname, oname );
              else sprintf( buf, "#LThe #P#zChampionship#n #wmatch #eis... #y%s #wVs #R%s#g!#n", cname, oname );


   if ( ( IS_NPC( competitor ) && IS_NPC( opponent ) ) || ( !IS_NPC( competitor ) && IS_NPC( opponent ) ) )
      opponent->max_hit = competitor->max_hit;
      else if ( !IS_NPC( opponent ) && IS_NPC( competitor ) )
              competitor->max_hit = opponent->max_hit;

   do_tinfo( competitor, opponent, buf );

   if ( mybet == TRUE )
      {
         strcpy(buf,"#eTo #wwager #eon the #pfight#e, type: #wbet #g(amount) #b(player name)#n");
         do_tinfo( competitor, opponent, buf );
         betting_time = 10;
      }

   stop_fighting( competitor, TRUE );
   competitor->fight_timer = 0;
   competitor->nemesis = 0;
   restore_char( competitor );
   SET_BIT( competitor->arena, AFF2_INARENA );
   char_from_room( competitor );
   char_to_room( competitor, get_room_index( 555 ) );
   do_look( competitor, "" );

   if ( IS_CLASS( competitor, CLASS_NINJA ) )
      competitor->wait = 30;
      else competitor->wait = 36;

   competitor->challenged = opponent;
   restore_char( competitor );

   if ( IS_NPC( competitor ) )
      competitor->fight_timer = 3;

   if ( IS_SET(competitor->extra, EXTRA_AFK) )
      REMOVE_BIT(competitor->extra, EXTRA_AFK);

   stop_fighting( opponent, TRUE );
   opponent->fight_timer = 0;
   opponent->nemesis = 0;
   restore_char( opponent );
   SET_BIT( opponent->arena, AFF2_INARENA );
   char_from_room( opponent );
   char_to_room( opponent, get_room_index( 555 ) );
   do_look( opponent, "" );

   if ( IS_CLASS( opponent, CLASS_NINJA ) )
      opponent->wait = 30;
      else opponent->wait = 36;

   opponent->challenged = competitor;
   restore_char( opponent );

   if ( IS_NPC( opponent ) )
      opponent->fight_timer = 3;

   if ( IS_SET(opponent->extra, EXTRA_AFK) )
      REMOVE_BIT(opponent->extra, EXTRA_AFK);
}

void do_initialize_tournament( CHAR_DATA *ch )
{
   int rng, count, position;

   do_info( ch, "#gThe #CKing #wof #RFighters #PTournament #gwill be starting #yshortly#g!!!#n" );

   for ( count = 0; count < 16; count++ )
   {
      temp[count] = participants[count];
      participants[count] = NULL;
   }

   for ( count = 0; count < 16; count++ )
   {
      rng = 15 - UMIN( count, 14 );
      position = -1;

      do
      {
         position += number_range( 1, rng );

         if ( position > 15 )
            {
               position -= 15;
               rng--;

               if ( rng == 1 )
                  position = -1;
            }
      }
      while ( participants[position] != NULL );

      participants[position] = temp[count];
      participants[position]->timer = 0;
      participants[position]->round = 1;

      free_string( battle_name[ position ] );

      if ( !IS_NPC( participants[position] ) )
         battle_name[ position ] = str_dup( participants[position]->pcdata->switchname );
         else battle_name[ position ] = str_dup( participants[position]->short_descr );
   }

   arena = FIGHT_INITIALIZE;
}

void show_victory_pose( CHAR_DATA *ch )
{
   act( "\n\rThe annoucer leaps to the ground as he lands softly on his knees.", ch, NULL, NULL, TO_CHAR );
   act( "\n\rThe annoucer leaps to the ground as he lands softly on his knees.", ch, NULL, NULL, TO_ROOM );
   act( "He then points his hand at you as he says '#yWinner!#n'", ch, NULL, NULL, TO_CHAR );
   act( "He then points his hand at $n as he says '#yWinner!#n'", ch, NULL, NULL, TO_ROOM );

   if ( IS_CLASS( ch, CLASS_MONK ) && number_percent() > 50 )
      {
          act("\n\rSmirking angrily, you lower your head as you open your hand.",ch,NULL,NULL,TO_CHAR);
          act("\n\rSmirking angrily, $n lowers $s head as $e opens $s hand.",ch,NULL,NULL,TO_ROOM);
          act("MasSiVe sprouts of flames leap from your hand as your eyes darken with a strange emotion.",ch,NULL,NULL,TO_CHAR);
          act("MasSiVe sprouts of flames leap from $n's hand as $s eyes darken with a strange emotion.",ch,NULL,NULL,TO_ROOM);
          act("Suddenly, you close your hand extinguishing the flames as your eyes flashes with intensity.",ch,NULL,NULL,TO_CHAR);
          act("Suddenly, $n closes $s hand extinguishing the flames as $s eyes flashes with intensity.",ch,NULL,NULL,TO_ROOM);
          return;
      }

   if ( IS_CLASS( ch, CLASS_DROW ) && number_percent() > 50 )
      {
          act("\n\rLolth flashes before you from the darkness as you rush into her arms.",ch,NULL,NULL,TO_CHAR);
          act("\n\rLolth flashes before $n from the darkness as $e rushes into her arms.",ch,NULL,NULL,TO_ROOM);
          act("Lolth sneers '#yAhhhh $n my child!#n' as she holds $m gently like a mother.",ch,NULL,NULL,TO_CHAR);
          act("Lolth sneers '#yAhhhh $n my child!#n' as she holds $m gently like a mother.",ch,NULL,NULL,TO_ROOM);
          act("Looking at your fallen foe, Lolth lets go a very sinister grin.\n\r",ch,NULL,NULL,TO_CHAR);
          act("Looking at $n'd fallen foe, Lolth lets go a very sinister grin.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
      }


   switch( number_range( 1, 5 ) )
   {
      default:
      case 2: act("\n\rYou do a roundhouse kick across the room as you punch the air in front of you.",ch,NULL,NULL,TO_CHAR);
              act("\n\r$n does a roundhouse kick across the room as $e punches the air in front of $m.",ch,NULL,NULL,TO_ROOM);
              act("Leaning back, you do a high kick as you swerve your hands about you.",ch,NULL,NULL,TO_CHAR);
              act("Leaning back, $n does a high kick as $e swerves $s hands about $m.",ch,NULL,NULL,TO_ROOM);
              act("Tilting forward, you clench your fist shouting '#yYeaah!#n'",ch,NULL,NULL,TO_CHAR);
              act("Tilting forward, $n clenches $s fist shouting '#yYeaah!#n'",ch,NULL,NULL,TO_ROOM);
              act("Suddenly recomposing yourself, you stare coldly at the room before looking at the distance...\n\r",ch,NULL,NULL,TO_CHAR);
              act("Suddenly recomposing $mself, $n stares coldly at you before looking at the distance...\n\r",ch,NULL,NULL,TO_ROOM);
              break;
 
      case 3: act("\n\rYou grin evilly as you take a hold of your clothes, shaking the wrinkles off it.",ch,NULL,NULL,TO_CHAR);
              act("\n\r$n grins evilly as $e take a hold of $s clothes, shaking the wrinkles off it.",ch,NULL,NULL,TO_ROOM);
              act("Ki energy radiates off your body as you crouch like an ape taunting your fallen foe.",ch,NULL,NULL,TO_CHAR);
              act("Ki energy radiates off $n's body as $e crouches like an ape taunting $s fallen foe.",ch,NULL,NULL,TO_ROOM);
              act("You yell '#yYosh!#n' as you clench your fists tightly.\n\r",ch,NULL,NULL,TO_CHAR);
              act("$n yells '#yYosh!#n' as $e clenches $s fists tightly.\n\r",ch,NULL,NULL,TO_ROOM);
              break;

      case 4: act("\n\rClosing your eyes, you take a deep breath as you clench your fists.",ch,NULL,NULL,TO_CHAR);
              act("\n\rClosing $s eyes, $n takes a deep breath as $e clenches $s fists.",ch,NULL,NULL,TO_ROOM);
              act("Releasing your inner ki, you growl with rage as you focus on the energy.",ch,NULL,NULL,TO_CHAR);
              act("Releasing $s inner ki, $n growls with rage as $e focuses on the energy.",ch,NULL,NULL,TO_ROOM);
              act("Finally, you breathe a sigh of relief as you look around you silently.\n\r",ch,NULL,NULL,TO_CHAR);
              act("Finally, $n breathes a sigh of relief as $e looks around $m silently.\n\r",ch,NULL,NULL,TO_ROOM);
              break;

      case 5: act("\n\rYou ruffle your clothes as you tilt forward arrogantly.",ch,NULL,NULL,TO_CHAR);
              act("\n\r$n ruffles $s clothes as $e tilts forward arrogantly.",ch,NULL,NULL,TO_ROOM);
              act("Pointing a finger at your fallen foe, you sneer '#yStandard!#n'\n\r",ch,NULL,NULL,TO_CHAR);
              act("Pointing a finger at $s fallen foe, $n sneers '#yStandard!#n'\n\r",ch,NULL,NULL,TO_ROOM);
              break;

      case 1: act("\n\rYou stare blankly ahead as your cape blows in the gentle wind.",ch,NULL,NULL,TO_CHAR);
              act("\n\r$n stares blankly ahead as $s cape blows in the gentle wind.",ch,NULL,NULL,TO_ROOM);
              break;
   }
}

void show_winner( CHAR_DATA *champion )
{
   char buf[MAX_STRING_LENGTH];
   int count;

   char_from_room( champion );
   char_to_room( champion, get_room_index(572) );

   if ( !IS_NPC( champion ) )
      sprintf(buf,"#R%s #yis the #w#zNEW#n #CWorld #LChampion#y!!!#n", champion->name );
      else sprintf(buf,"#R%s #yis the #w#zNEW#n #CWorld #LChampion#y!!!#n", champion->short_descr );

   champion->arena = 0;
   champion->round = 0;

   do_tinfo( champion, NULL, buf );
   do_shout( champion, "I am the Champion of the WOOOOORRRLLLDD!!!" );

   if ( !IS_NPC( champion ) )
      {
         act( "#gStill #yclenching #gyour #Rfists #gfrom your #wMaSsIvE #Rshout#g, you suddenly hear a voice in your head.#n", champion, NULL, NULL, TO_CHAR );
         act( "#P'Congratulations Warrior. Your prize money has been incremented to your account.'#n", champion, NULL, NULL, TO_CHAR );
         champion->bank = UMIN( 1000000000, champion->bank + 100000 );
         champion->pcdata->twins++;
         check_leaderboard( champion, "" );
         save_char_obj( champion );
      }

   if ( betting_points == 10000000 )
      {
         if ( !IS_NPC( champion ) )
            champion->bank = UMIN( 1000000000, champion->bank + 2500000 );

         do_info( NULL, "#ePlayers #wcan no longer #ybet #win #Ctournaments #wand #gteam battles#w!!!#n" );
         betting_points = 0;
         mybet = FALSE;
      }

   REMOVE_BIT( champion->arena, AFF2_COMPETITOR );
   arena = FIGHT_CLEAR;
   num_contestants = 0;
   battle_num = 0;

   for ( count = 0; count < 16; count++ )
       participants[ count ] = NULL;
}

void do_zanretsu_ken( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt your palm forward, blocking $N's attack as you focus on your chi.",ch,NULL,victim,TO_CHAR);
   act("$n tilts $s palms forward, blocking your attack as $e focuses on $s chi.",ch,NULL,victim,TO_VICT);
   act("$n tilts $s palms forward, blocking $N's attack as $e focuses on $s chi.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yZanretsu Ken!#n' as you punch $N viciously in the chest!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yZanretsu Ken!#n' as $e punches you viciously in the chest!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yZanretsu Ken!#n' as $e punches $N viciously in the chest!",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("$N's body is pushed into the air as you punch $M hard all over $S upper body.",ch,NULL,victim,TO_CHAR);
   act("Your body is pushed into the air as $n punches you hard all over your upper body.",ch,NULL,victim,TO_VICT);
   act("$N's body is pushed into the air as $n punches $M hard all over $S upper body.",ch,NULL,victim,TO_NOTVICT);
   act("You then send $N across the room with a swift jab to the guts!",ch,NULL,victim,TO_CHAR);
   act("$n then sends you across the room with a swift jab to the guts!",ch,NULL,victim,TO_VICT);
   act("$n then sends $N across the room with a swift jab to the guts!",ch,NULL,victim,TO_NOTVICT);

   special_slam( victim, number_range( 0, 3 ) );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = HIT;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      hurt_person( ch, victim, 1500000, TRUE );
      else hurt_person( ch, victim, 500000, TRUE );

   ch->fight_timer = 0;
}

void do_shut_up( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You suddenly scream '#yShad UP!#n' as you leap towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly screams '#yShad UP!#n' as $e leaps towards you!",ch,NULL,victim,TO_VICT);
   act("$n suddenly screams '#yShad UP!#n' as $e leaps towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("You then send $N across the room with a swift boot to the face!",ch,NULL,victim,TO_CHAR);
   act("$n then sends you across the room with a swift boot to the face!",ch,NULL,victim,TO_VICT);
   act("$n then sends $N across the room with a swift boot to the face!",ch,NULL,victim,TO_NOTVICT);

   special_slam( victim, number_range( 0, 3 ) );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = AERIAL;
   ch->attack_var = HIT;

   hurt_person( ch, victim, 500000, TRUE );
   ch->fight_timer = 0;
}

void do_air_blast( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act( "As $N moves in to strike, you turn around slowly as you fold your arms.", ch, NULL, victim, TO_CHAR );
   act( "As you move in to strike, $n turns around slowly as $e folds $s arms.", ch, NULL, victim, TO_VICT );
   act( "As $N moves in to strike, $n turns around slowly as $e folds $s arms.", ch, NULL, victim, TO_NOTVICT );
   act( "$N finds $Mself stuck in midway as you chuckle silently to yourself.", ch, NULL, victim, TO_CHAR );
   act( "You find yourself paralysed in midway as $n chuckles silently to $mself.", ch, NULL, victim, TO_VICT );
   act( "$N finds $Mself stuck in midway as $n chuckles silently to $mself.", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, your cape flutters up in the wind as $N is BlAsT away!!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n's cape flutters up in the wind as you are BlAsT away!!", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n's cape flutters up in the wind as $N is BlAsT away!!", ch, NULL, victim, TO_NOTVICT );
   act( "You glance over your shoulders as you make $N crashes hard with your mind!", ch, NULL, victim, TO_CHAR );
   act( "$n glances over $s shoulders as $e makes you crash hard with $s mind!", ch, NULL, victim, TO_VICT );
   act( "$n glances over $s shoulders as $e makes $N crashes hard with $s mind!", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = UPPERCUT;
   ch->attack_var = SOUL;

   victim->move = UMAX( 0, victim->move - 500000 );
   hurt_person( ch, victim, 500000, TRUE );
}

void do_mob_counter( CHAR_DATA *ch, CHAR_DATA *victim )
{
   if ( !IS_NPC( ch ) )
      return;

   if ( ch->pIndexData->vnum == 565 )
      {
         do_zanretsu_ken( ch, victim );
         return;
      }

   if ( ch->pIndexData->vnum == 563 )
      {
         do_air_blast( ch, victim );
         return;
      }

   if ( ch->pIndexData->vnum == 566 )
      {
         do_shut_up( ch, victim );
         return;
      }

   if ( ch->pIndexData->vnum == 562 )
      {
         int hp = victim->hit;

         do_monkdisarm( ch, victim );

         if ( hp == victim->hit && ch->fighting == victim )
            {
               act( "You suddenly roll backwards screaming '#yYeeeash!#n' as you kick $N the face!", ch, NULL, victim, TO_CHAR );
               act( "$n suddenly rolls backwards screaming '#yYeeeash!#n' as $e kicks you the face!", ch, NULL, victim, TO_VICT );
               act( "$n suddenly rolls backwards screaming '#yYeeeash!#n' as $e kicks $N the face!", ch, NULL, victim, TO_NOTVICT );
               act( "The bluish flames from your foot SeArS $N's flesh as $E is flung aside!", ch, NULL, victim, TO_CHAR );
               act( "The bluish flames from $n's foot SeArS your flesh as you are flung aside!", ch, NULL, victim, TO_VICT );
               act( "The bluish flames from $n's foot SeArS $N's flesh as $E is flung aside!", ch, NULL, victim, TO_NOTVICT );

               special_slam( victim, number_range( 0, 3 ) );

               ch->attack_type = ATTACK_PHYSICAL;
               ch->attack_focus = DIRECT;
               ch->attack_var = BLAST;

               hurt_person( ch, victim, 300000, TRUE );

               act( "You land back on your knees as you swerve your hands in a ninjitsu pose.", ch, NULL, NULL, TO_CHAR );
               act( "$n lands back on $s knees as $e swerves $s hands in a ninjitsu pose.", ch, NULL, NULL, TO_ROOM );
               ch->fight_timer = 0;
            }

         return;
      }

   if ( ch->pIndexData->vnum == 564 )
      {
         perform_aragami( ch, victim );
         return;
      }
}

void need_saving( CHAR_DATA *ch )
{
   CHAR_DATA *vch, *vch_next;
   CHAR_DATA *mob2 = NULL, *mob3 = NULL, *mob4 = NULL, *mob;
   int num2 = 0, num3 = 0, num4 = 0, num = 0;

   if ( !IS_NPC( ch ) || ch->hit < ch->max_hit * 0.25 || ch->in_room->vnum < 33500 || ch->in_room->vnum > 33800 )
      return;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !IS_NPC( vch ) )
         {
            if ( vch->fighting == ch )
               return;
               else continue;
         }

      if ( vch == ch || vch->pIndexData->vnum < 550 || vch->pIndexData->vnum > 570 )
         continue;

      if ( mob2 == NULL || vch->fighting == mob2 )
         {
            if ( mob2 == NULL )
               mob2 = vch->fighting;

            num2++;
         }
         else if ( mob3 == NULL || vch->fighting == mob3 )
         {
            if ( mob3 == NULL )
               mob3 = vch->fighting;

            num3++;
         }
         else if ( mob4 == NULL || vch->fighting == mob4 )
         {
            if ( mob4 == NULL )
               mob4 = vch->fighting;

            num4++;
         }
   }

   if ( num2 == 0 )
      return;

   if ( num3 > num2 && num3 > num4 )
      {
         mob = mob3;
         num = num3;
      }
      else if ( num4 > num2 && num4 > num3 )
              {
                 mob = mob4;
                 num = num3;
              }
              else {
                      mob = mob2;
                      num = num2;
                   }

   if ( mob == NULL || ( num < 2 && mob->hit > mob->max_hit * 0.25 ) )
      return;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      if ( IS_NPC( vch ) || vch == ch )
         continue;

      if ( vch->fighting == mob )
         {
            act( "#RYou #wrescue #R$N!",  ch, NULL, mob, TO_CHAR );
            act( "#R$n #wrescues #Ryou!", ch, NULL, mob, TO_VICT );
            act( "#R$n #wrescues #R$N!",  ch, NULL, mob, TO_NOTVICT );

            stop_fighting( ch, FALSE );
            set_fighting( ch, vch );
            vch->fighting = ch;

            return;
         }
   }
}

void do_krizalid( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 2 ) )
      {
         SET_BIT( ch->cmbt[0], 2 );
         do_kyaku( ch, "" );
         return;
      }

   if ( number_percent() > 89 )
      {
         do_ayanae( ch, "" );
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 && number_percent() > 84 )
      do_ki_charge(ch,"");

   if ( ch->fighting == NULL )
      return;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 5);
      else choice = number_range( 1, 6);

   switch(choice)
   {
      default: break;
      case 4:
      case 2: do_ki_grasp(ch,""); break;
      case 3:
      case 5: 
      case 1: do_ki_charge(ch,""); break;
   }
}

void do_zero_punch( CHAR_DATA *ch )
{
   if ( ch->fighting == NULL )
      return;

   act( "You yell '#yHui!#n' as you crouches forward punching $N hard in the guts!", ch, NULL, ch->fighting, TO_CHAR );
   act( "$n yells '#yHui!#n' as $e crouches forward punching you hard in the guts!", ch, NULL, ch->fighting, TO_VICT );
   act( "$n yells '#yHui!#n' as $e crouches forward punching $N hard in the guts!", ch, NULL, ch->fighting, TO_NOTVICT );
   act( "Kiotic energy fizzes from your body as you crouch to your knees in agony.", ch->fighting, NULL, NULL, TO_CHAR );
   act( "Kiotic energy fizzes from $n's body as $e crouches to $s knees in agony.", ch->fighting, NULL, NULL, TO_ROOM );

   if ( number_percent() > 84 )
      ch->fighting->stunned = 2;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_focus = LOW;
   ch->attack_var = BLUNT;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      hurt_person( ch, ch->fighting, 1000000, TRUE );
      else hurt_person( ch, ch->fighting, 500000, TRUE );
}

void do_zero_swipe( CHAR_DATA *ch )
{
   CHAR_DATA *victim;

   if ( ch->fighting == NULL )
      return;

   victim = ch->fighting;

   act("You grin evilly as you lean forward, grabbing the edge of your kuruda armor.",ch,NULL,NULL,TO_CHAR);
   act("$n grins evilly as $e leans forward, grabbing the edge of $s kuruda armor.",ch,NULL,NULL,TO_ROOM);
   act("You feel the kiotic energy stirring within you as you leap in front of $N.",ch,NULL,victim,TO_CHAR);
   act("$n feels the kiotic energy stirring within $m as $e leaps in front of you.",ch,NULL,victim,TO_VICT);
   act("$n feels the kiotic energy stirring within $m as $e leaps in front of $N.",ch,NULL,victim,TO_NOTVICT);
   act("You suddenly wave your cape across $N's chest as your SpIkEs rip through it!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly waves $s cape across your chest as $s SpIkEs rip through it!",ch,NULL,victim,TO_VICT);
   act("$n suddenly waves $s cape across $N's chest as $s SpIkEs rip through it!",ch,NULL,victim,TO_NOTVICT);
   act("You then SaVaGeLy swipe your cape to the right as $N screams in pain!",ch,NULL,victim,TO_CHAR);
   act("$n then SaVaGeLy swipes $s cape to the right as you scream in pain!",ch,NULL,victim,TO_VICT);
   act("$n then SaVaGeLy swipes $s cape to the right as $N screams in pain!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 74 )
      special_slam( victim, number_range( 0, 3 ) );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLUNT;

   hurt_person( ch, ch->fighting, 1000000, TRUE );
   ch->fight_timer = 0;
}

void do_zero( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      {
         CHAR_DATA *victim;
         int weakness = 0, wele = 0;
         char arg[MAX_INPUT_LENGTH];

         victim = ch->fighting;

         sprintf( arg, "none" );

         if ( get_fire_prof( victim ) < weakness )
            {
               weakness = get_fire_prof( victim );
               wele = ATTACK_FIRE;
               sprintf( arg, "fire" );
            }

         if ( get_ice_prof( victim ) < weakness )
            {
               weakness = get_ice_prof( victim );
               wele = ATTACK_ICE;
               sprintf( arg, "ice" );
            }

         if ( get_thunder_prof( victim ) < weakness )
            {
               weakness = get_thunder_prof( victim );
               wele = ATTACK_THUNDER;
               sprintf( arg, "thunder" );
            }

         if ( get_earth_prof( victim ) < weakness )
            {
               weakness = get_earth_prof( victim );
               wele = ATTACK_EARTH;
               sprintf( arg, "earth" );
            }

         if ( get_holy_prof( victim ) < weakness )
            {
               weakness = get_holy_prof( victim );
               wele = ATTACK_HOLY;
               sprintf( arg, "holy" );
            }

         if ( get_dark_prof( victim ) < weakness )
            {
               weakness = get_dark_prof( victim );
               wele = ATTACK_DARK;
               sprintf( arg, "dark" );
            }

         if ( wele != ch->wfocus )
            do_lockon( ch, arg );
      }

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( number_percent() > 85 )
      {
         do_spirit_bomb( ch, "" );
         return;
      }

   if ( number_percent() > 89 )
      {
         do_ayanae( ch, "" );
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 3);
      else choice = number_range( 1, 4);

   switch(choice)
   {
      default: break;
      case 2: do_zero_punch(ch); break;
      case 3: 
      case 1: do_zero_swipe(ch); break;
   }
}

void do_sparda( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->fighting->hit < 3000000 && IS_SET( ch->cmbt[0], 2 ) )
      {
         CHAR_DATA *victim = ch->fighting;

         act( "\n\rYou suddenly dash towards $N as your Blade #Psizzles#n with #we#yl#we#yc#wt#yr#wi#yc#wi#yt#wy#n!", ch, NULL, victim, TO_CHAR );
         act( "\n\r$n suddenly dashes towards you as $s Blade #Psizzles#n with #we#yl#we#yc#wt#yr#wi#yc#wi#yt#wy#n!", ch, NULL, victim, TO_VICT );
         act( "\n\r$n suddenly dashes towards $N as $s Blade #Psizzles#n with #we#yl#we#yc#wt#yr#wi#yc#wi#yt#wy#n!", ch, NULL, victim, TO_NOTVICT );
         act( "There is a TREMEDOUS clash as weapons meet sending your blade into the air!!!", ch, NULL, NULL, TO_CHAR );
         act( "There is a TREMEDOUS clash as weapons meet sending $n's blade into the air!!!", ch, NULL, NULL, TO_ROOM );
         act( "You chuckle at the thought of beating a defenseless Dante as you prepare yourself.", victim, NULL, NULL, TO_CHAR );
         act( "$n chuckles at the thought of beating a defenseless Dante as $e prepares $mself.", victim, NULL, NULL, TO_ROOM );
         act( "Looking around to dish out the killing blow, you notice Dante isn't here.", victim, NULL, NULL, TO_CHAR );
         act( "Looking around to dish out the killing blow, $n notice Dante isn't here.", victim, NULL, NULL, TO_ROOM );
         act( "As $N frowns thinking on $S next move, you suddenly press your gun against $S chin.", ch, NULL, victim, TO_CHAR );
         act( "As you frown thinking on your next move, you suddenly feel cold steel against your chin.", ch, NULL, victim, TO_VICT );
         act( "As $N frowns thinking on $S next move, $E suddenly feels cold steel against $S chin.", ch, NULL, victim, TO_NOTVICT );
         act( "$N glances slowly to the side facing the barrel with you gazing silently away.", ch, NULL, victim, TO_CHAR );
         act( "You glance slowly to the side facing the barrel of a gun with $n gazing silently away.", ch, NULL, victim, TO_VICT );
         act( "$N glances slowly to the side facing the barrel of a gun with $n gazing silently away.", ch, NULL, victim, TO_NOTVICT );
         act( "A mass of energy manifests from the gun as a loud hum fills the air.", ch, NULL, NULL, TO_CHAR );
         act( "A mass of energy manifests from the gun as a loud hum fills the air.", ch, NULL, NULL, TO_ROOM );
         act( "You mutter '#yToo easy...#n' as you BlAsT away $N's face with the overcharged shot!!!", ch, NULL, victim, TO_CHAR );
         act( "$n mutters '#yToo easy...#n' as $e BlAsTs away your face with the overcharged shot!!!", ch, NULL, victim, TO_VICT );
         act( "$n mutters '#yToo easy...#n' as $e BlAsTs away $N's face with the overcharged shot!!!", ch, NULL, victim, TO_NOTVICT );

         stop_fighting( victim, TRUE );
         victim->hit = -2000010;
         victim->position = POS_MORTAL;
         hurt_person( ch, victim, 0, TRUE );

         act( "As Dante walks away into the distance, his blade slams into the ground beside your body.", victim, NULL, NULL, TO_CHAR );
         act( "As Dante walks away into the distance, his blade slams into the ground beside $n's body.", victim, NULL, NULL, TO_ROOM );
         return;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 2 ) )
      {
         SET_BIT( ch->cmbt[0], 2 );

         if ( !IS_CLASS( ch->fighting, CLASS_PHOENIX ) )
            {
               do_fiend_inferno( ch, "" );
               return;
            }
      }

   if ( number_percent() > 85 )
      {
         do_round_trip( ch, "" );
         return;
      }

   if ( number_percent() > 85 )
      {
         CHAR_DATA *victim = ch->fighting;

         act( "You roll past $N's attacks as you quickly pull out your hand made guns.", ch, NULL, victim, TO_CHAR );
         act( "$n rolls past your attacks as $e quickly pulls out $s hand made guns.", ch, NULL, victim, TO_VICT );
         act( "$n rolls past $N's attacks as $e quickly pulls out $s hand made guns.", ch, NULL, victim, TO_NOTVICT );
         act( "You say '#yIts Showtime!#n' as you start shooting $N stylishly!", ch, NULL, victim, TO_CHAR );
         act( "$n says '#yIts Showtime!#n' as $e starts shooting you stylishly!", ch, NULL, victim, TO_VICT );
         act( "$n says '#yIts Showtime!#n' as $e starts shooting $N stylishly!", ch, NULL, victim, TO_NOTVICT );
         act( "You then drive your blade ViCiOuSlY up $N's body catapulting $M into the air!", ch, NULL, victim, TO_CHAR );
         act( "$n then drives $s blade ViCiOuSlY up your body catapulting you into the air!", ch, NULL, victim, TO_VICT );
         act( "$n then drives $s blade ViCiOuSlY up $N's body catapulting $M into the air!", ch, NULL, victim, TO_NOTVICT );
         act( "As $N falls down to the earth, you juggle $M mercilessly with your deadly shooting.", ch, NULL, victim, TO_CHAR );
         act( "As you fall down to the earth, $n juggles you mercilessly with $s deadly shooting.", ch, NULL, victim, TO_VICT );
         act( "As $N falls down to the earth, $n juggles $M mercilessly with $s deadly shooting.", ch, NULL, victim, TO_NOTVICT );
         act( "You then drive your blade ViCiOuSlY up $N's body again catapulting $M into the air!", ch, NULL, victim, TO_CHAR );
         act( "$n then drives $s blade ViCiOuSlY up your body again catapulting you into the air!", ch, NULL, victim, TO_VICT );
         act( "$n then drives $s blade ViCiOuSlY up $N's body again catapulting $M into the air!", ch, NULL, victim, TO_NOTVICT );
         act( "As $N falls down to the earth, you juggle $M mercilessly with your deadly shooting.", ch, NULL, victim, TO_CHAR );
         act( "As you fall down to the earth, $n juggles you mercilessly with $s deadly shooting.", ch, NULL, victim, TO_VICT );
         act( "As $N falls down to the earth, $n juggles $M mercilessly with $s deadly shooting.", ch, NULL, victim, TO_NOTVICT );
         act("You yell '#yHai!#n' as you DrIvE your blade through $N's chest!",ch,NULL,victim,TO_CHAR);
         act("$n yells '#yHai!#n' as $e DrIvEs $s blade through your chest!",ch,NULL,victim,TO_VICT);
         act("$n yells '#yHai!#n' as $e DrIvEs $s blade through $N's chest!",ch,NULL,victim,TO_NOTVICT);

         special_slam( victim, number_range( 0, 3 ) );

         act("Pulling out your shotgun, you chuckle '#yBoom!#n' as you BlAsT $N away!!!", ch, NULL, victim, TO_CHAR );
         act("Pulling out $s shotgun, $n chuckles '#yBoom!#n' as $e BlAsTs you away!!!", ch, NULL, victim, TO_VICT );
         act("Pulling out $s shotgun, $n chuckles '#yBoom!#n' as $e BlAsTs $N away!!!", ch, NULL, victim, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_focus = AERIAL;
         ch->attack_var = BLAST;

         hurt_person( ch, victim, 1500000, TRUE );
         ch->fight_timer = 0;
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 5);
      else choice = number_range( 1, 6);

   switch(choice)
   {
      default: break;
      case 5:
      case 4: do_magma_drive(ch,""); break;
      case 2: if ( !IS_CLASS( ch->fighting, CLASS_PHOENIX ) )
                 {
                    do_meteor_blast(ch,""); 
                    break;
                 }
      case 3:
      case 1: do_stinger(ch,""); break;
   }
}

void do_vampire( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 2 ) )
      {
         SET_BIT( ch->cmbt[0], 2 );

         if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
            do_soul_eraser( ch, "" );
            else do_soul_reaper( ch, "" );

         return;
      }

   if ( number_percent() > 85 )
      {
         do_wolf_blast( ch, "" );
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 3);
      else choice = number_range( 1, 4);

   switch(choice)
   {
      default: break;
      case 2: do_lamprey(ch,""); break;
      case 3:
      case 1: do_drain(ch,""); break;
   }
}

void do_drow( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( number_percent() > 80 )
      {
         do_shadow_flare( ch, "" );
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 5);
      else choice = number_range( 1, 6);

   switch(choice)
   {
      default: break;
      case 5:
      case 1:
      case 4: do_dark_thrust(ch,""); break;
      case 2: do_lloth_throw(ch,""); break;
   }
}

void do_angel( CHAR_DATA *ch, char *argument )
{
   int choice, check = 0;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      {
         check = 10;

         if ( number_percent() > 85 )
            do_heavenly_cloak( ch, "" );

         if ( ch->hit < ch->max_hit * 0.1 && !IS_SET( ch->cmbt[0], 2 ) )
            {
               ch->class = CLASS_ANGEL;
               SET_BIT( ch->cmbt[0], 2 );
               do_martyr( ch, "" );
               return;
            }

         if ( number_percent() > 85 )
            {
               do_angel_smite( ch, "" );
               return;
            }
      }

   if ( number_percent() > 85 - check )
      {
         do_wrathofgod( ch, "" );
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 5 );
      else choice = number_range( 1, 6 );

   switch(choice)
   {
      default: break;
      case 3: do_fist_takedown(ch,""); break;
      case 5: do_blade_dance(ch,""); break;
      case 1: do_fist_heaven(ch,""); break;
      case 2:
      case 4: do_mega_strike(ch,""); break;
   }
}

void do_ninja( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->pIndexData->vnum == 559 && number_percent() > 85 ) 
      {
         do_ki_pinch( ch, "" );
         return;
      }

   if ( ch->pIndexData->vnum == 559 && number_percent() > 87 && !IS_SET( ch->more2, MORE2_GUARD ) ) 
      {
         act( "You crouch on your knees as you swerve your hands around silently.", ch, NULL, NULL, TO_CHAR );
         act( "$n crouches on $s knees as $e swerves $s hands around silently.", ch, NULL, NULL, TO_ROOM );
         act( "You chuckle to yourself as fog slowly rise from the ground...", ch, NULL, NULL, TO_CHAR );
         act( "$n chuckles to $mself as fog slowly rise from the ground...", ch, NULL, NULL, TO_ROOM );
         act( "An outline of an ancient crest shimmers before you as the fog envelops you.", ch, NULL, NULL, TO_CHAR );
         act( "An outline of an ancient crest shimmers before you as the fog envelops $n.", ch, NULL, NULL, TO_ROOM );
         SET_BIT( ch->more2, MORE2_GUARD );
         return;
      }

   if ( number_percent() > 87 && !IS_CLASS( ch->fighting, CLASS_WISP ) && !IS_NPC( ch->fighting ) )
      {
         do_dinichi( ch, "" );
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 && number_percent() > 74 )
      {
         if ( ch->pIndexData->vnum == 560 )
            do_zsikuku( ch, "" );
            else do_split_strike( ch, "" );
      }

   if ( number_percent() > 30 )
      do_hakiznetu( ch, "" );
}

void do_knight( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->pIndexData->vnum == 550 || ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 ) ) 
      {
         if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 2 ) )
            {
               SET_BIT( ch->cmbt[0], 2 );
               act("\n\r$n eyes is ablazed in demonic fire as $s grins evilly at you.",ch,NULL,NULL,TO_ROOM);
               do_say(ch,"I will not die... The SorceresS' child will wipe the evil from the world!");
               do_demon_slice( ch, "" );
               return;
            }

         if ( ch->hit < ch->max_hit * 0.5 && !IS_SET( ch->cmbt[0], 4 ) )
            {
               SET_BIT( ch->cmbt[0], 4 );
               do_no_mercy( ch, "" );
               return;
            }

         if ( number_percent() > 85 )
            {
               do_soul_hammer(ch,"");
               return;
            }
      }
 
   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range(1,5);
      else choice = number_range(1,6);

   switch(choice)
   {
      case 1:
      case 3: if ( !IS_CLASS( ch->fighting, CLASS_WISP ) )
                 {
                    do_lightning_stab(ch,""); 
                    break;
                 }

      case 2: if ( !IS_CLASS( ch->fighting, CLASS_DUERGAR ) )
                 {
                    do_split_punch(ch,""); 
                    break;
                 }
      case 5:
      case 4: do_holy_explosion(ch,""); break;
   }
}

void do_joe_heal( CHAR_DATA *ch )
{
   act( "You stand proudly as you place your hands on your waist.", ch, NULL, NULL, TO_CHAR );
   act( "Joe stands proudly as he places his hands on his waist.", ch, NULL, NULL, TO_ROOM );
   act( "You smile at the crowd as you exclaim '#yBelieve in yourself!#n'", ch, NULL, NULL, TO_CHAR );
   act( "Joe smiles at the crowd as he exclaims '#yBelieve in yourself!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The crowd ROARS for their champion as you suddenly look alot healthier.", ch, NULL, NULL, TO_CHAR );
   act( "The crowd ROARS for their champion as Joe suddenly looks alot healthier.", ch, NULL, NULL, TO_ROOM );
   ch->hit = UMIN( ch->max_hit, ch->hit + 1500000 );
}

void do_joe_crowd( CHAR_DATA *ch )
{
   if ( ch->fighting == NULL )
      return;

   act( "You snap your finger to a silent beat as you wiggle your body around.", ch, NULL, NULL, TO_CHAR );
   act( "Joe snaps his finger to a silent beat as he wiggles his body around.", ch, NULL, NULL, TO_ROOM );
   act( "You then blow a kiss to the crowd as you exclaim '#yI do this for you my loyal FANS!#n'", ch, NULL, NULL, TO_CHAR );
   act( "Joe then blows a kiss to the crowd as he exclaims '#yI do this for you my loyal FANS!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The crowd becomes INSANE as they throw roses at you, but cans and bottles at $N!", ch, NULL, ch->fighting, TO_CHAR );
   act( "The crowd becomes INSANE as they throw roses at Joe, but cans and bottles at YOU!", ch, NULL, ch->fighting, TO_VICT );
   act( "The crowd becomes INSANE as they throw roses at Joe, but cans and bottles at $N!", ch, NULL, ch->fighting, TO_NOTVICT );
   act( "$N curses in frustration as $E tries $S best to avoid the avalanche.", ch, NULL, ch->fighting, TO_CHAR );
   act( "You curse in frustration as you try your best to avoid the avalanche.", ch, NULL, ch->fighting, TO_VICT );
   act( "$N curses in frustration as $E tries $S best to avoid the avalanche.", ch, NULL, ch->fighting, TO_NOTVICT );
   act( "$N suddenly staggers forward as a HUGE bottle crack $S skull!", ch, NULL, ch->fighting, TO_CHAR );
   act( "You suddenly stagger forward as a HUGE bottle crack your skull!", ch, NULL, ch->fighting, TO_VICT );
   act( "$N suddenly staggers forward as a HUGE bottle crack $S skull!", ch, NULL, ch->fighting, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = OVERHEAD;
   ch->attack_var = BLUNT;
   hurt_person( ch, ch->fighting, number_range( 750000, 1500000 ), TRUE );
}

void do_joe_punch( CHAR_DATA *ch )
{
   act( "You leap into the air as you shout '#yFear my killer punch!#n'", ch, NULL, NULL, TO_CHAR );
   act( "Joe leaps into the air as he shouts '#yFear my killer punch!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The crowd goes BALLISTIC as you suddenly dash into $N, giving $M a GUT SHOT!!", ch, NULL, ch->fighting, TO_CHAR );
   act( "The crowd goes BALLISTIC as Joe suddenly dashes into you, giving you a GUT SHOT!!", ch, NULL, ch->fighting, TO_VICT );
   act( "The crowd goes BALLISTIC as Joe suddenly dashes into $N, giving $M a GUT SHOT!!", ch, NULL, ch->fighting, TO_NOTVICT );

   if ( number_percent() < 80 )
      {
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_focus = DIRECT;
         ch->attack_var = HIT;
         hurt_person( ch, ch->fighting, 1, TRUE );

         if ( ch->fighting == NULL || number_percent() < 66 )
            return;

         act( "You scream '#yOne more time baby!#n' as you punch $N from behind!", ch, NULL, ch->fighting, TO_CHAR );
         act( "Joe screams '#yOne more time baby!#n' as he punches you from behind!", ch, NULL, ch->fighting, TO_VICT );
         act( "Joe screams '#yOne more time baby!#n' as he punches $N from behind!", ch, NULL, ch->fighting, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_focus = DIRECT;
         ch->attack_var = HIT;
         hurt_person( ch, ch->fighting, 2, TRUE );
         return;
      }

   act( "As $N roll on the ground laughing, you stand there flexing your huge muscles.", ch, NULL, ch->fighting, TO_CHAR );
   act( "As you roll on the ground laughing, Joe stands there flexing his huge muscles.", ch, NULL, ch->fighting, TO_VICT );
   act( "As $N roll on the ground laughing, Joe stands there flexing his huge muscles.", ch, NULL, ch->fighting, TO_NOTVICT );
   act( "Your eyes suddenly blazes red as you screams '#yNEVER give up!#n'", ch, NULL, NULL, TO_CHAR );
   act( "Joe's eyes suddenly blazes red as he screams '#yNEVER give up!#n'", ch, NULL, NULL, TO_ROOM );
   act( "The crowd become POSSESSED as you CHARGE straight towards $N with GLOWING FISTS!!!", ch, NULL, ch->fighting, TO_CHAR );
   act( "The crowd become POSSESSED as Joe CHARGES straight towards you with GLOWING FISTS!!!", ch, NULL, ch->fighting, TO_VICT );
   act( "The crowd become POSSESSED as Joe CHARGES straight towards $N with GLOWING FISTS!!!", ch, NULL, ch->fighting, TO_NOTVICT );
   act( "The impact causes $N to see stars as the crowd's roar DEAFENS you!!!!!!", ch, NULL, ch->fighting, TO_CHAR );
   act( "The impact causes you to see stars as the crowd's roar DEAFENS you!!!!!!", ch, NULL, ch->fighting, TO_VICT );
   act( "The impact causes $N to see stars as the crowd's roar DEAFENS you!!!!!!", ch, NULL, ch->fighting, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = HIT;

   SET_BIT( ch->arena, AFF2_UB );
   hurt_person( ch, ch->fighting, 3000000, TRUE );
   REMOVE_BIT( ch->arena, AFF2_UB );
}

void do_joe( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   choice = number_range(1,4);

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   switch(choice)
   {
      case 2: do_joe_heal( ch ); break;
      case 4:
      case 1: do_joe_punch( ch ); break;
      case 3: do_joe_crowd( ch ); break;
      default: break;
   }
}

void do_guardian( CHAR_DATA *ch, char *argument )
{
   int choice, count;

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      {
         if ( ch->hit < ch->max_hit * 0.25 && !IS_SET( ch->cmbt[0], 2 ) )
            {
               SET_BIT( ch->cmbt[0], 2 );
               act("\n\r$n eyes is ablazed in demonic fire as $s grins evilly at you.",ch,NULL,NULL,TO_ROOM);
               do_say(ch,"Foolish mortals... Watch your upcoming demise...!");
               do_lunar_shield( ch, "" );
               return;
            }

         if ( ch->hit < ch->max_hit * 0.75 && !IS_SET( ch->cmbt[0], 4 ) )
            {
               SET_BIT( ch->cmbt[0], 4 );
               do_lunar_shield( ch, "" );
               return;
            }
      }

   if ( number_percent() > 85 )
      {
         do_lunar_blade(ch,"");
         return;
      }

   if ( number_percent() > 75 )
      {
         do_buddhastomp(ch,"");
         return;
      }

   if ( number_percent() > 75 )
      {
         do_shock_wave(ch,"");
         return;
      }

   for ( count = 0; count < 4; count++ )
   {
      choice = number_range(1,5);

      switch( choice )
      {
         case 1: do_f_uppercut( ch, "" ); break;
         case 2: do_l_slash( ch, "" ); break;
         case 3: do_tremor( ch, "" ); break;
         case 4: do_empower( ch, "" ); break;
         case 5: do_chill_blast( ch, "" ); break;
      }

      if ( ch->fighting == NULL )
         return;
   }
}

void do_killer( CHAR_DATA *ch, char *argument )
{
   int choice;
   CHAR_DATA *victim;

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   victim = ch->fighting;

   if ( !IS_NPC( victim ) && number_percent() > 94 )
      {
         act( "You suddenly leap in front of $N as your eyes pierce through $S soul.", ch, NULL, victim, TO_CHAR );
         act( "$n suddenly leaps in front of you as $s eyes pierce through your soul.", ch, NULL, victim, TO_VICT );
         act( "$n suddenly leaps in front of $N as $s eyes pierce through $S soul.", ch, NULL, victim, TO_NOTVICT );
         act( "Sensing the end near, $N frantically tries to cut you in half!", ch, NULL, victim, TO_CHAR );
         act( "Sensing the end near, you frantically try to cut $n in half!", ch, NULL, victim, TO_VICT );
         act( "Sensing the end near, $N frantically tries to cut $n in half!", ch, NULL, victim, TO_NOTVICT );
         act( "No matter what $N does, $S weapons seem to go through your body.", ch, NULL, victim, TO_CHAR );
         act( "No matter what you do, your weapons seem to go through $n's body.", ch, NULL, victim, TO_VICT );
         act( "No matter what $N does, $S weapons seem to go through $n's body.", ch, NULL, victim, TO_NOTVICT );
         act( "Unsheathing your daggers, the last thing $N sees is you fading away...", ch, NULL, victim, TO_CHAR );
         act( "Unsheathing $s daggers, the last thing you see is $n fading away...", ch, NULL, victim, TO_VICT );
         act( "Unsheathing $s daggers, the last thing $N sees is $n fading away...", ch, NULL, victim, TO_NOTVICT );

         stop_fighting( victim, TRUE );
         victim->hit = -1000010;
         victim->position = POS_MORTAL;
         return;
      }

   if ( number_percent() < 40 )
      return;

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

   hurt_person( ch, victim, 500000, TRUE );

   act("You silently land back on your feet, sheathing your daggers with a small smirk.",ch,NULL,NULL,TO_CHAR);
   act("$n silently lands back on $s feet, sheathing $s daggers with a small smirk.",ch,NULL,NULL,TO_ROOM);
}

void do_sdemon( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   choice = number_range(1,4);

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( number_percent() > 85 )
      {
         do_hail_storm(ch,"");
         return;
      }

   switch(choice)
   {
      case 2: do_dark_holy( ch, "" ); break;
      case 4:
      case 1: do_leech( ch, "" ); break;
      case 3: do_frostbreath( ch, "" ); break;
      default: break;
   }
}

void do_terry( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->pIndexData->vnum < 550 || ch->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 4 ) )
      {
         SET_BIT( ch->cmbt[0], 4 );
         do_triple_geyser( ch, "" );
         return;
      }

   if ( number_percent() > 85 )
      {
         do_overheat_geyser(ch,"");
         return;
      }

   if ( number_percent() > 85 )
      {
         do_buster_wolf(ch,"");
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 && number_percent() > 74 )
      do_power_dunk( ch, "" );

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 79 )
      do_power_charge( ch, "" );

   if ( ch->fighting == NULL )
      return;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 && number_percent() > 74 )
      {
         CHAR_DATA *victim = ch->fighting;

         act("You yell '#yPoweer Waave!#n' as you suddenly SmAsH your fist onto the ground!",ch,NULL,NULL,TO_CHAR);
         act("$n yells '#yPoweer Waave!#n' as $e suddenly SmAsHeS $s fist onto the ground!",ch,NULL,NULL,TO_ROOM);
         act("A rippling effect flashes across the room as the energy wave goes at you!",victim,NULL,NULL,TO_CHAR);
         act("A rippling effect flashes across the room as the energy wave goes at $n!",victim,NULL,NULL,TO_ROOM);
         act("You stagger back in pain as a MaSsIvE energy wave SLAMS into you.",victim,NULL,NULL,TO_CHAR);
         act("$n staggers back in pain as a MaSsIvE energy wave SLAMS into $m.",victim,NULL,NULL,TO_ROOM);

         if ( !IS_CLASS( victim, CLASS_WISP ) )
            ch->ele_attack = ATTACK_THUNDER;
         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_var = BLAST;
         ch->attack_focus = LOW;
         hurt_person( ch, victim, 500000, TRUE );

         if ( ch->fighting == NULL )
            return;
      }
 
   choice = number_range(1,6);

   switch(choice)
   {
      case 2: do_rising_tackle(ch,""); break;
      case 5:
      case 4: do_bare_knuckle(ch,""); break;
      case 1:
      case 3: do_burn_knuckle(ch,""); break;
   }
}

void do_kouken( CHAR_DATA *ch )
{
   CHAR_DATA *victim;

   if ( ch->fighting == NULL )
      return;

   victim = ch->fighting;

   act("You slowly draw back your hand in a circle motion.",ch,NULL,NULL,TO_CHAR);
   act("$n slowly draws back $s hand in a circle motion.",ch,NULL,NULL,TO_ROOM);
   act("A small mass of energy manifests around your hand as you concentrate on $N.",ch,NULL,victim,TO_CHAR);
   act("A small mass of energy manifests around $n's hand as $e concentrates on you.",ch,NULL,victim,TO_VICT);
   act("A small mass of energy manifests around $n's hand as $e concentrates on $N.",ch,NULL,victim,TO_NOTVICT);
   act("You mutter '#yKo ou...#n' as you SmAsH your fist into $N's chest viciously!",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yKo ou...#n' as $e SmAsHeS $s fist into your chest viciously!",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yKo ou...#n' as $e SmAsHeS $s fist into $N's chest viciously!",ch,NULL,victim,TO_NOTVICT);
   act("Releasing the blast into $N, you yell '#yKEN!#n' as $E is thrown away from the force!",ch,NULL,victim,TO_CHAR);
   act("Releasing the blast into you, $n yells '#yKEN!#n' as you are thrown away from the force!",ch,NULL,victim,TO_VICT);
   act("Releasing the blast into $N, $n yells '#yKEN!#n' as $E is thrown away from the force!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 50 && !IS_SET( victim->more, MORE_BLIND ) )
      {
         victim->blind_timer = 4;
         SET_BIT( victim->more, MORE_BLIND );
      }

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;

   hurt_person( ch, victim, 750000, TRUE );
}

void do_haou_ken( CHAR_DATA *ch )
{
   CHAR_DATA *victim;

   if ( ch->fighting == NULL )
      return;

   victim = ch->fighting;

   act("You mutter '#yHaou...#n' as you tilt your hands together.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yHaou...#n' as $e tilts $s hands together.",ch,NULL,NULL,TO_ROOM);
   act("A loud humming fills the air as a sphere of energy manifests between your hands.",ch,NULL,NULL,TO_CHAR);
   act("A loud humming fills the air as a sphere of energy manifests between $n's hands.",ch,NULL,NULL,TO_ROOM);
   act("The energy flares with intensity as your eyes darken with concentration.",ch,NULL,NULL,TO_CHAR);
   act("The energy flares with intensity as $n's eyes darken with concentration.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yShou Kou Ken!#n' as you release the HuGe FiReBaLl towards $N!!!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yShou Kou Ken!#n' as $e releases the HuGe FiReBaLl towards you!!!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yShou Kou Ken!#n' as $e releases the HuGe FiReBaLl towards $N!!!",ch,NULL,victim,TO_NOTVICT);
   act("The fireball RoArS into you as you are knocked across the room!",victim,NULL,NULL,TO_CHAR);
   act("The fireball RoArS into $n as $e is knocked across the room!",victim,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      {
         int dam = 1500000;

         if ( victim->blocking > 0 )
            {
               dam *= 3;
               victim->blocking = victim->block_meter = 0;
               act( "#eSuddenly a #yviolent #Rforce #erips your arms apart as you realize your #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_CHAR );
               act( "#eSuddenly a #yviolent #Rforce #erips $n#e's arms apart as $e realizes $s #Cguard #eis #y#zbroken#e.#n", victim, NULL, NULL, TO_ROOM );
               victim->noblock = UMAX( 1, victim->noblock );
            }

         SET_BIT( ch->arena, AFF2_UB );
         hurt_person( ch, victim, dam, TRUE );
         REMOVE_BIT( ch->arena, AFF2_UB );
      }
      else hurt_person( ch, victim, 1500000, TRUE );

   act("You crouch on one knee as you spread your arms apart in a fighting stance.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches on one knee as $e spreads $s arms apart in a fighting stance.",ch,NULL,NULL,TO_ROOM);
}

void do_ko_hou( CHAR_DATA *ch )
{
   CHAR_DATA *victim;

   if ( ch->fighting == NULL )
      return;

   victim = ch->fighting;

   act("You mutter '#yKo...#n' as you punch $N hard in the guts.",ch,NULL,victim,TO_CHAR);
   act("$n mutters '#yKo...#n' as $e punches you hard in the guts.",ch,NULL,victim,TO_VICT);
   act("$n mutters '#yKo...#n' as $e punches $N hard in the guts.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yHou!#n' as you leap into the air DrIvInG your fist up $N's chin.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yHou!#n' as $e leaps into the air DrIvInG $s fist up your chin.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yHou!#n' as $e leaps into the air DrIvInG $s fist up $N's chin.",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = UPPERCUT;
   ch->attack_var = HIT;

   hurt_person( ch, victim, 1000000, TRUE );

   act("You land back on your knees softly as you glance around yourself.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back on $s knees softly as $e glances around $mself.",ch,NULL,NULL,TO_ROOM);
}

void do_ryuuko_ranbu( CHAR_DATA *ch )
{
   CHAR_DATA *victim;

   if ( ch->fighting == NULL )
      return;

   victim = ch->fighting;

   act("You dash towards $N as you drive your knee viciously into $S stomach.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you as $e drives $s knee viciously into your stomach.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N as $e drives $s knee viciously into $S stomach.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yKyokugenryuu Ougi!#n' as you punch $N in the face.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yKyokugenryuu Ougi!#n' as $e punches you in the face.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yKyokugenryuu Ougi!#n' as $e punches $N in the face.",ch,NULL,victim,TO_NOTVICT);
   act("You twirl yourself around SmAsHiNg your foot across $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n twirls $mself around SmAsHiNg $s foot across your face!",ch,NULL,victim,TO_VICT);
   act("$n twirls $mself around SmAsHiNg $s foot across $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yOra!#n' as you push your palm into $N's stomach.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yOra!#n' as $e pushes $s palm into $N's stomach.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yOra!#n' as $e pushes $s palm into $N's stomach.",ch,NULL,victim,TO_NOTVICT);
   act("You twirl yourself around SmAsHiNg your foot across $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n twirls $mself around SmAsHiNg $s foot across your face!",ch,NULL,victim,TO_VICT);
   act("$n twirls $mself around SmAsHiNg $s foot across $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yOra!#n' as you give $N a powerful uppercut.",ch,NULL,victim,TO_CHAR);
   act("$n yell '#yOra!#n' as $e gives you a powerful uppercut.",ch,NULL,victim,TO_VICT);
   act("$n yell '#yOra!#n' as $e gives $N a powerful uppercut.",ch,NULL,victim,TO_NOTVICT);
   act("Staggering $N with your palm, you spin around giving $M a swift sweep.",ch,NULL,victim,TO_CHAR);
   act("Staggering you with $s palm, $n spins around giving you a swift sweep.",ch,NULL,victim,TO_VICT);
   act("Staggering $N with your palm, $n spins around giving $M a swift sweep.",ch,NULL,victim,TO_NOTVICT);
   act("You then give $N a punch into the face as you push $M away from you with your palm.",ch,NULL,victim,TO_CHAR);
   act("$n then gives you a punch into the face as $e pushes you away from $m with $s palm.",ch,NULL,victim,TO_VICT);
   act("$n then gives $N a punch into the face as $e pushes $M away from $m with $s palm.",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#yOra!#n' as you twirl your foot across $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yOra!#n' as $e twirls $s foot across your face!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yOra!#n' as $e twirls $s foot across $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("You grimace as you give $N a vicious uppercut sending $M sprawling into the air!",ch,NULL,victim,TO_CHAR);
   act("$n grimaces as $e gives you a vicious uppercut sending you sprawling into the air!",ch,NULL,victim,TO_VICT);
   act("$n grimaces as $e gives $N a vicious uppercut sending $M sprawling into the air!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("You rapidly spin your body around as you drive your fist upwards $N's chin!",ch,NULL,victim,TO_CHAR);
   act("$n rapidly spins $s body around as $e drives $s fist upwards your chin!",ch,NULL,victim,TO_VICT);
   act("$n rapidly spins $s body around as $e drives $s fist upwards $N's chin!",ch,NULL,victim,TO_NOTVICT);
   act("A manifest of energy swirls around your hands as you spin around one more time.",ch,NULL,NULL,TO_CHAR);
   act("A manifest of energy swirls around $n's hands as $e spins around one more time.",ch,NULL,NULL,TO_ROOM);
   act("You yell '#yMorata!#n' as you SmAsH your glowing fist into $N's face, driving $M on the ground!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yMorata!#n' as $e SmAsHeS $s glowing fist into your face, driving you on the ground!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yMorata!#n' as $e SmAsHeS $s glowing fist into $N's face, driving $M on the ground!",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = UPPERCUT;
   ch->attack_var = HIT;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      hurt_person( ch, victim, 3000000, TRUE );
      else hurt_person( ch, victim, 2000000, TRUE );

   act("You softly land back on your knees out of breath.",ch,NULL,NULL,TO_CHAR);
   act("$n softly lands back on $s knees out of breath.",ch,NULL,NULL,TO_ROOM);
}

void do_tenchi( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You crouch forward VICIOUSLY StRiKiNg $N in the guts!",ch,NULL,victim,TO_CHAR);
   act("$n crouches forward VICIOUSLY StRiKiNg you in the guts!",ch,NULL,victim,TO_VICT);
   act("$n crouches forward VICIOUSLY StRiKiNg $N in the guts!",ch,NULL,victim,TO_NOTVICT);
   act("You scream as the power of the HALO sends you flying across the room dazed!",victim,NULL,NULL,TO_CHAR);
   act("$n screams as the power of the HALO sends $m flying across the room dazed!",victim,NULL,NULL,TO_ROOM);

   SET_BIT( ch->cmbt[0], 2 );
   special_slam( victim, number_range( 0, 3 ) );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLUNT;

   SET_BIT( ch->arena, AFF2_UB );
   hurt_person( ch, victim, 5000000, TRUE );
   REMOVE_BIT( ch->arena, AFF2_UB );

   act("You yell '#yHou!#n' as you bend back tilting your fists to stabilize your body.",ch,NULL,NULL,TO_CHAR);
   act("$n yells '#yHou!#n' as $e bends back tilting $s fists to stabilize $s body.",ch,NULL,NULL,TO_ROOM);
}

void do_ryo( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 4 ) )
      {
         SET_BIT( ch->cmbt[0], 4 );
         act("You breathe deeply as you store your #Cchi#n together, waiting for #wanyone#n to #ystrike#n...",ch,NULL,NULL,TO_CHAR);
         act("$n breathes deeply as $e stores $s #Cchi#n together, waiting for #wanyone#n to #ystrike#n...",ch,NULL,NULL,TO_ROOM);
         return;
      }

   if ( IS_SET( ch->cmbt[0], 4 ) && !IS_SET( ch->cmbt[0], 2 ) )
      {
         CHAR_DATA *victim = ch->fighting;

         SET_BIT( ch->cmbt[0], 2 );
         act("You crouch forward VICIOUSLY StRiKiNg $N in the guts!",ch,NULL,victim,TO_CHAR);
         act("$n crouches forward VICIOUSLY StRiKiNg you in the guts!",ch,NULL,victim,TO_VICT);
         act("$n crouches forward VICIOUSLY StRiKiNg $N in the guts!",ch,NULL,victim,TO_NOTVICT);
         act("You scream as the power of the HALO sends you flying across the room dazed!",victim,NULL,NULL,TO_CHAR);
         act("$n screams as the power of the HALO sends $m flying across the room dazed!",victim,NULL,NULL,TO_ROOM);
         act("You yell '#yHou!#n' as you bend back tilting your fists to stabilize your body.",ch,NULL,NULL,TO_CHAR);
         act("$n yells '#yHou!#n' as $e bends back tilting $s fists to stabilize $s body.",ch,NULL,NULL,TO_ROOM);

         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_focus = DIRECT;
         ch->attack_var = BLUNT;

         if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
            {
               SET_BIT( ch->arena, AFF2_UB );
               hurt_person( ch, victim, 3500000, TRUE );
               REMOVE_BIT( ch->arena, AFF2_UB );
            }
            else hurt_person( ch, victim, 2500000, TRUE );

         return;
      }

   if ( number_percent() > 85 )
      {
         do_ryuuko_ranbu(ch);
         return;
      }

   if ( number_percent() > 85 )
      {
         do_haou_ken(ch);
         return;
      }
 
   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      choice = number_range( 1, 4);
      else choice = number_range(1,5);

   switch(choice)
   {
      case 4:
      case 2: do_kouken(ch); break;
      case 1:
      case 3: do_ko_hou(ch); break;
   }
}

void do_kyo( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 4 ) )
      {
         SET_BIT( ch->cmbt[0], 4 );
         perform_sina( ch, ch->fighting );
         return;
      }

   if ( number_percent() > 85 )
      {
         do_orochi_nagi(ch,"");
         return;
      }

   if ( number_percent() > 85 )
      {
         do_yami_sugi(ch,"");
         return;
      }

   if ( number_percent() > 85 )
      {
         CHAR_DATA *victim = ch->fighting;

         act( "You twist yourself back as you start charging up your lifeforce.", ch, NULL, NULL, TO_CHAR );
         act( "$n twists $mself back as $e starts charging up $s lifeforce.", ch, NULL, NULL, TO_ROOM );
         act( "You glare at your hand as it starts trembling from the power.", ch, NULL, NULL, TO_CHAR );
         act( "$n glares at $s hand as it starts trembling from the power.", ch, NULL, NULL, TO_ROOM );
         act( "You growl '#yaaaaaarrrrrrgh...#n' as a spiral of flames encircle your feet.", ch, NULL, NULL, TO_CHAR );
         act( "$n growls '#yaaaaaarrrrrrgh...#n' as a spiral of flames encircle $s feet.", ch, NULL, NULL, TO_ROOM );
         act( "You yell '#yOia De!#n' as you suddenly spin fluidly at $N!", ch, NULL, victim, TO_CHAR );
         act( "$n yells '#yOia De!#n' as $e suddenly spins fluidly at you!", ch, NULL, victim, TO_VICT );
         act( "$n yells '#yOia De!#n' as $e suddenly spins fluidly at $N!", ch, NULL, victim, TO_NOTVICT );
         act( "The air RoArS with POWER as you SmAsH your glowing fist into $N's facE!", ch, NULL, victim, TO_CHAR );
         act( "The air RoArS with POWER as $n SmAsHeS $s glowing fist into your facE!", ch, NULL, victim, TO_VICT );
         act( "The air RoArS with POWER as $n SmAsHeS $s glowing fist into $N's facE!", ch, NULL, victim, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_focus = DIRECT;
         ch->attack_var = HIT;

         if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
            hurt_person( ch, victim, 2500000, TRUE );
            else hurt_person( ch, victim, 1500000, TRUE );

         act( "You skid across the floor as you finish your powerful swing.", ch, NULL, NULL, TO_CHAR );
         act( "$n skids across the floor as $e finishes $s powerful swing.", ch, NULL, NULL, TO_ROOM );
         return;
      }

   if ( number_percent() > 50 )
      do_nanase(ch,"");

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      perform_aragami( ch, ch->fighting );

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      do_oniyaki(ch,"");

   if ( ch->in_room->vnum < 33500 || ch->in_room->vnum > 33800 || ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "You suddenly skip towards $N as your fist explodes into ferocious flames.", ch, NULL, ch->fighting, TO_CHAR );
         act( "$n suddenly skips towards you as $s fist explodes into ferocious flames.", ch, NULL, ch->fighting, TO_VICT );
         act( "$n suddenly skips towards $N as $s fist explodes into ferocious flames.", ch, NULL, ch->fighting, TO_NOTVICT );
         act( "You mutter '#yEai Yuik...#n' as you push your fist into $N's stomach!", ch, NULL, ch->fighting, TO_CHAR );
         act( "$n mutters '#yEai Yuik...#n' as $e pushes $s fist into your stomach!", ch, NULL, ch->fighting, TO_VICT );
         act( "$n mutters '#yEai Yuik...#n' as $e pushes $s fist into $N's stomach!", ch, NULL, ch->fighting, TO_NOTVICT );
         act( "Your body implodes as you are catapulted into the air.", ch->fighting, NULL, NULL, TO_CHAR );
         act( "$n's body implodes as $e is catapulted into the air.", ch->fighting, NULL, NULL, TO_ROOM );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = LOW;
         ch->attack_var = BLAST;

         hurt_person( ch, ch->fighting, 500000, TRUE );
      }

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "You yell '#yHui!#n' as you leap into the air kicking $N hard!", ch, NULL, ch->fighting, TO_CHAR );
         act( "$n yells '#yHui!#n' as $e leaps into the air kicking you hard!", ch, NULL, ch->fighting, TO_VICT );
         act( "$n yells '#yHui!#n' as $e leaps into the air kicking $N hard!", ch, NULL, ch->fighting, TO_NOTVICT );
         act( "You then yell '#yHui!#n' again as you SMASH your other foot in $N!!", ch, NULL, ch->fighting, TO_CHAR );
         act( "$n then yells '#yHui!#n' again as $e SMASHES $s other foot in you!!", ch, NULL, ch->fighting, TO_VICT );
         act( "$n then yells '#yHui!#n' again as $e SMASHES $s other foot in $N!!", ch, NULL, ch->fighting, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->attack_focus = AERIAL;
         ch->attack_var = HIT;

         hurt_person( ch, ch->fighting, 500000, TRUE );

         act( "You softly land back on your knees as you grin cockily.", ch, NULL, NULL, TO_CHAR );
         act( "$n softly lands back on $s knees as $e grins cockily.", ch, NULL, NULL, TO_ROOM );
      }

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "You yell '#yScro Noi Ayda!#n' as you DrIvE your fist up $N's face!", ch, NULL, ch->fighting, TO_CHAR );
         act( "$n yells '#yScro Noi Ayda!#n' as $e DrIvEs $s fist up your face!", ch, NULL, ch->fighting, TO_VICT );
         act( "$n yells '#yScro Noi Ayda!#n' as $e DrIvEs $s fist up $N's face!", ch, NULL, ch->fighting, TO_NOTVICT );
         act( "Your body implodes in devastating flames as you are flung across the room!", ch->fighting, NULL, NULL, TO_CHAR );
         act( "$n's body implodes in devastating flames as $e is flung across the room!", ch->fighting, NULL, NULL, TO_ROOM );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = UPPERCUT;
         ch->attack_var = BLAST;

         hurt_person( ch, ch->fighting, 500000, TRUE );

         act( "You smirk to yourself as you emit your deadly flames temporarily.", ch, NULL, NULL, TO_CHAR );
         act( "$n smirks to $mself as $e emits $s deadly flames temporarily.", ch, NULL, NULL, TO_ROOM );
      }
}

void do_chain_drive( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim = ch->fighting;

   act( "You stand arrogantly as you raise your right hand slightly to your hips.", ch, NULL, NULL, TO_CHAR );
   act( "$n stands arrogantly as $e raises $s right hand slightly to $s hips.", ch, NULL, NULL, TO_ROOM );
   act( "You give $N a fancy A-OK sign as you quickly pull out your cool shades.", ch, NULL, victim, TO_CHAR );
   act( "$n gives you a fancy A-OK sign as $e quickly pulls out $s cool shades.", ch, NULL, victim, TO_VICT );
   act( "$n gives $N a fancy A-OK sign as $e quickly pulls out $s cool shades.", ch, NULL, victim, TO_NOTVICT );
   act( "Giving $N a subtle smirk, you suddenly throw your shades into $S face!", ch, NULL, victim, TO_CHAR );
   act( "Giving you a subtle smirk, $n suddenly throws $s shades into your face!", ch, NULL, victim, TO_VICT );
   act( "Giving $N a subtle smirk, $n suddenly throws $s shades into $S face!", ch, NULL, victim, TO_NOTVICT );
   act( "As $N hits away the shades in anger, you suddenly fade out of sight...", ch, NULL, victim, TO_CHAR );
   act( "As you hit away the shades in anger, $n suddenly fade out of sight...", ch, NULL, victim, TO_VICT );
   act( "As $N hits away the shades in anger, $n suddenly fade out of sight...", ch, NULL, victim, TO_NOTVICT );
   act( "As you scan the area for $N, you suddenly feel very hot and uncomfortable.", victim, NULL, ch, TO_CHAR );
   act( "As $n scans the area for $N, $e suddenly looks very hot and uncomfortable.", victim, NULL, ch, TO_NOTVICT );
   act( "You appear in front of $N as you quickly punch $M hard in the stomach.", ch, NULL, victim, TO_CHAR );
   act( "$n appears in front of you as $e quickly punches you hard in the stomach.", ch, NULL, victim, TO_VICT );
   act( "$n appears in front of $N as $e quickly punches $M hard in the stomach.", ch, NULL, victim, TO_NOTVICT );

   act( "You mutter '#yHmph...#n' as you release devastating flames around you.", ch, NULL, NULL, TO_CHAR );
   act( "$n mutters '#yHmph...#n' as $e releases devastating flames around $m.", ch, NULL, NULL, TO_ROOM );
   act( "You then lean back as you singe $N's face with a savage blast!", ch, NULL, victim, TO_CHAR );
   act( "$n then leans back as you singe your face with a savage blast!", ch, NULL, victim, TO_VICT );
   act( "$n then leans back as $e singes $N's face with a savage blast!", ch, NULL, victim, TO_NOTVICT );

   act( "You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.", ch, NULL, victim, TO_CHAR );
   act( "$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.", ch, NULL, victim, TO_VICT );
   act( "$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.", ch, NULL, victim, TO_NOTVICT );

   act( "You scream '#yYai!#n' as you drive your flaming foot up $N's body!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yYai!#n' as $e drives $s flaming foot up your body!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yYai!#n' as $e drives $s flaming foot up $N's body!", ch, NULL, victim, TO_NOTVICT );

   act( "You lean forward as you kick $N viciously in the face.", ch, NULL, victim, TO_CHAR );
   act( "$n leans forward as $e kicks you viciously in the face.", ch, NULL, victim, TO_VICT );
   act( "$n leans forward as $e kicks $N viciously in the face.", ch, NULL, victim, TO_NOTVICT );

   act( "You mutter '#yHmph...#n' as you release devastating flames around you.", ch, NULL, NULL, TO_CHAR );
   act( "$n mutters '#yHmph...#n' as $e releases devastating flames around $m.", ch, NULL, NULL, TO_ROOM );
   act( "You then lean back as you singe $N's face with a savage blast!", ch, NULL, victim, TO_CHAR );
   act( "$n then leans back as you singe your face with a savage blast!", ch, NULL, victim, TO_VICT );
   act( "$n then leans back as $e singes $N's face with a savage blast!", ch, NULL, victim, TO_NOTVICT );

   act( "You mutter '#yHmph...#n' as you release devastating flames around you.", ch, NULL, NULL, TO_CHAR );
   act( "$n mutters '#yHmph...#n' as $e releases devastating flames around $m.", ch, NULL, NULL, TO_ROOM );
   act( "You then lean back as you singe $N's face with a savage blast!", ch, NULL, victim, TO_CHAR );
   act( "$n then leans back as you singe your face with a savage blast!", ch, NULL, victim, TO_VICT );
   act( "$n then leans back as $e singes $N's face with a savage blast!", ch, NULL, victim, TO_NOTVICT );

   act( "You ram your fist in $N's guts viciously.", ch, NULL, victim, TO_CHAR );
   act( "$n rams $s fist in your guts viciously.", ch, NULL, victim, TO_VICT );
   act( "$n rams $s fist in $N's guts viciously.", ch, NULL, victim, TO_NOTVICT );

   act( "You scream '#yYai!#n' as you drive your flaming foot up $N's body!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yYai!#n' as $e drives $s flaming foot up your body!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yYai!#n' as $e drives $s flaming foot up $N's body!", ch, NULL, victim, TO_NOTVICT );

   act( "You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.", ch, NULL, victim, TO_CHAR );
   act( "$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.", ch, NULL, victim, TO_VICT );
   act( "$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.", ch, NULL, victim, TO_NOTVICT );

   act( "You scream '#yYai!#n' as you drive your flaming foot up $N's body!", ch, NULL, victim, TO_CHAR );
   act( "$n screams '#yYai!#n' as $e drives $s flaming foot up your body!", ch, NULL, victim, TO_VICT );
   act( "$n screams '#yYai!#n' as $e drives $s flaming foot up $N's body!", ch, NULL, victim, TO_NOTVICT );

   act( "You twirl your body around StAgGeRiNg $N with a vicious roundhouse kick.", ch, NULL, victim, TO_CHAR );
   act( "$n twirls $s body around StAgGeRiNg you with a vicious roundhouse kick.", ch, NULL, victim, TO_VICT );
   act( "$n twirls $s body around StAgGeRiNg $N with a vicious roundhouse kick.", ch, NULL, victim, TO_NOTVICT );

   act( "You ram your fist in $N's guts viciously.", ch, NULL, victim, TO_CHAR );
   act( "$n rams $s fist in your guts viciously.", ch, NULL, victim, TO_VICT );
   act( "$n rams $s fist in $N's guts viciously.", ch, NULL, victim, TO_NOTVICT );

   act( "You ram your fist in $N's guts viciously.", ch, NULL, victim, TO_CHAR );
   act( "$n rams $s fist in your guts viciously.", ch, NULL, victim, TO_VICT );
   act( "$n rams $s fist in $N's guts viciously.", ch, NULL, victim, TO_NOTVICT );

   act( "You lean forward as you kick $N viciously in the face.", ch, NULL, victim, TO_CHAR );
   act( "$n leans forward as $e kicks you viciously in the face.", ch, NULL, victim, TO_VICT );
   act( "$n leans forward as $e kicks $N viciously in the face.", ch, NULL, victim, TO_NOTVICT );

   act( "You yell '#yEeai Yai!#n' as you quickly leap into the air.", ch, NULL, NULL, TO_CHAR );
   act( "$n yell '#yEeai Yai!#n' as $e quickly leaps into the air.", ch, NULL, NULL, TO_ROOM );
   act( "Your hand ignites in ferocious flames as you rip it across $N's body!", ch, NULL, victim, TO_CHAR );
   act( "$n's hand ignites in ferocious flames as $e rips it across $N's body!", ch, NULL, victim, TO_VICT );
   act( "$n's hand ignites in ferocious flames as $e rips it across $N's body!", ch, NULL, victim, TO_NOTVICT );

   act( "You then slide back as you suddenly fade away out of sight again...", ch, NULL, NULL, TO_CHAR );
   act( "$n then slides back as $e suddenly fades away out of sight again...", ch, NULL, NULL, TO_ROOM );

   act( "You mutter '#ePunk...#n' as you appear in front of $N; pushing your hands into $M.", ch, NULL, victim, TO_CHAR );
   act( "$n mutters '#ePunk...#n' as $e appears in front of you; pushing $s hands into you.", ch, NULL, victim, TO_VICT );
   act( "$n mutters '#ePunk...#n' as $e appears in front of $N; pushing $s hands into $M.", ch, NULL, victim, TO_NOTVICT );

   special_slam( victim, number_range( 0, 3 ) );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;
   SET_BIT( ch->arena, AFF2_UB );

   hurt_person( ch, victim, 5000000, TRUE );

   REMOVE_BIT( ch->arena, AFF2_UB );
}

void do_shadow_drive( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim = ch->fighting;

   act( "You stand arrogantly as you raise your right hand slightly to your hips.", ch, NULL, NULL, TO_CHAR );
   act( "$n stands arrogantly as $e raises $s right hand slightly to $s hips.", ch, NULL, NULL, TO_ROOM );
   act( "You then open your hand as a sprout of dark flames hover above it...", ch, NULL, NULL, TO_CHAR );
   act( "$n then open $s hand as a sprout of dark flames hover above it...", ch, NULL, NULL, TO_ROOM );
   act( "You grin evilly at $N as your eyes flickers behind your deadly flames.", ch, NULL, victim, TO_CHAR );
   act( "$n grins evilly at you as $s eyes flickers behind $s deadly flames.", ch, NULL, victim, TO_VICT );
   act( "$n grins evilly at $N as $s eyes flickers behind $s deadly flames.", ch, NULL, victim, TO_NOTVICT );
   act( "Suddenly, you dash towards $N as you fade out of sight!", ch, NULL, victim, TO_CHAR );
   act( "Suddenly, $n dashes towards you as $e fades out of sight!", ch, NULL, victim, TO_VICT );
   act( "Suddenly, $n dashes towards $N as $e fades out of sight!", ch, NULL, victim, TO_NOTVICT );
   act( "As you scan the area for $N, you suddenly feel very hot and uncomfortable.", victim, NULL, ch, TO_CHAR );
   act( "As $n scans the area for $N, $e suddenly looks very hot and uncomfortable.", victim, NULL, ch, TO_NOTVICT );
   act( "You mutter '#yDie...#n' as you appear in front of $N; pushing your hands into $M.", ch, NULL, victim, TO_CHAR );
   act( "$n mutters '#yDie...#n' as $e appears in front of you; pushing $s hands into you.", ch, NULL, victim, TO_VICT );
   act( "$n mutters '#yDie...#n' as $e appears in front of $N; pushing $s hands into $M.", ch, NULL, victim, TO_NOTVICT );
   act( "You clench your fists going '#yHmph...#n' as $N gets catapulted into the air!!!", ch, NULL, victim, TO_CHAR );
   act( "$n clenches $s fists going '#yHmph...#n' as you get catapulted into the air!!!", ch, NULL, victim, TO_VICT );
   act( "$n clenches $s fists going '#yHmph...#n' as $N gets catapulted into the air!!!", ch, NULL, victim, TO_NOTVICT );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_focus = DIRECT;
   ch->attack_var = BLAST;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      hurt_person( ch, victim, 2500000, TRUE );
      else hurt_person( ch, victim, 2000000, TRUE );

   if ( victim == NULL || victim->in_room == NULL )
      return;

   victim->air_timer = number_range( 2, 3 );
   stop_fighting( victim, TRUE );
}

void do_kk( CHAR_DATA *ch, char *argument )
{
   int choice;
   CHAR_DATA *victim = ch->fighting;

   need_saving( ch );

   victim->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               victim = ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 4 ) )
      {
         SET_BIT( ch->cmbt[0], 4 );
         do_chain_drive( ch, "" );
         return;
      }

   if ( number_percent() > 85 )
      {
         do_shadow_drive(ch,"");
         return;
      }

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 && number_percent() > 74 )
      {
         CHAR_DATA *vch, *vch_next;

         act( "You mutter '#yHmph...#n' as you release devastating flames around you.", ch, NULL, NULL, TO_CHAR );
         act( "$n mutters '#yHmph...#n' as $e releases devastating flames around $m.", ch, NULL, NULL, TO_ROOM );
         act( "The flames glow #CBLUE #yHOT#n on your hands as you grin evilly...", ch, NULL, NULL, TO_CHAR );
         act( "The flames glow #CBLUE #yHOT#n on $n's hands as $e grins evilly...", ch, NULL, NULL, TO_ROOM );
         act( "You mutter '#eBegone...#n' as you flick your arm to the left!", ch, NULL, NULL, TO_CHAR );
         act( "$n mutters '#eBegone...#n' as $e flicks $s arm to the left!", ch, NULL, NULL, TO_ROOM );
         act( "A flash of reddish light suddenly SoArS across the room in ANGER!!!", ch, NULL, NULL, TO_CHAR );
         act( "A flash of reddish light suddenly SoArS across the room in ANGER!!!", ch, NULL, NULL, TO_ROOM );

         for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
         {
            vch_next = vch->next_in_room;

            if ( is_safe( ch, vch ) || is_same_group( ch, vch ) )
               continue;

            act("You scream loudly in pain as the ferocious flames eradicates your flesh!",vch,NULL,NULL,TO_CHAR);
            act("$n screams loudly in pain as the ferocious flames eradicates $s flesh!",vch,NULL,NULL,TO_ROOM);

            ch->attack_type = ATTACK_PHYSICAL;
            ch->ele_attack = ATTACK_FIRE;
            ch->attack_focus = DIRECT;
            ch->attack_var = BEAM;

            hurt_person( ch, vch, 2000000, TRUE );
         }

         return;
      }

   if ( number_percent() > 50 )
      {
         act( "You mutter '#yHmph...#n' as you release devastating flames around you.", ch, NULL, NULL, TO_CHAR );
         act( "$n mutters '#yHmph...#n' as $e releases devastating flames around $m.", ch, NULL, NULL, TO_ROOM );
         act( "You then lean back as you singe $N's face with a savage blast!", ch, NULL, victim, TO_CHAR );
         act( "$n then leans back as you singe your face with a savage blast!", ch, NULL, victim, TO_VICT );
         act( "$n then leans back as $e singes $N's face with a savage blast!", ch, NULL, victim, TO_NOTVICT );
         act( "You go on your knees in pain as everything becomes hazy to you.", victim, NULL, NULL, TO_CHAR );
         act( "$n goes on $s knees in pain as everything becomes hazy to $m.", victim, NULL, NULL, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = DIRECT;
         ch->attack_var = BLAST;

         hurt_person( ch, victim, 400000, TRUE );
      }

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "You scream '#yYai!#n' as you drive your foot up $N's body!", ch, NULL, victim, TO_CHAR );
         act( "$n screams '#yYai!#n' as $e drives $s foot up your body!", ch, NULL, victim, TO_VICT );
         act( "$n screams '#yYai!#n' as $e drives $s foot up $N's body!", ch, NULL, victim, TO_NOTVICT );
         act( "Your body spasm in midair as hungry flames eat through your flesh.", victim, NULL, NULL, TO_CHAR );
         act( "$n's body spasms in midair as hungry flames eat through $s flesh.", victim, NULL, NULL, TO_ROOM );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = UPPERCUT;
         ch->attack_var = BLAST;

         hurt_person( ch, victim, 400000, TRUE );
      }

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "You scream '#yHui!#n' as you twist your foot around suddenly.", ch, NULL, NULL, TO_CHAR );
         act( "$n screams '#yHui!#n' as $e twists $s foot around suddenly.", ch, NULL, NULL, TO_ROOM );
         act( "You then leap forward as you kick a ball of fire at $N!", ch, NULL, victim, TO_CHAR );
         act( "$n then leaps forward as $e kicks a ball of fire at you!", ch, NULL, victim, TO_VICT );
         act( "$n then leaps forward as $e kicks a ball of fire at $N!", ch, NULL, victim, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = AERIAL;
         ch->attack_var = BLAST;

         hurt_person( ch, victim, 400000, TRUE );
      }

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "You yell '#yEeai Yai!#n' as you quickly leap into the air.", ch, NULL, NULL, TO_CHAR );
         act( "$n yell '#yEeai Yai!#n' as $e quickly leaps into the air.", ch, NULL, NULL, TO_ROOM );
         act( "Your hand ignites in ferocious flames as you rip it across $N's body!", ch, NULL, victim, TO_CHAR );
         act( "$n's hand ignites in ferocious flames as $e rips it across $N's body!", ch, NULL, victim, TO_VICT );
         act( "$n's hand ignites in ferocious flames as $e rips it across $N's body!", ch, NULL, victim, TO_NOTVICT );
         act( "You swerve your hand in a deadly arc as you SmAsH $N away from you!!!", ch, NULL, victim, TO_CHAR );
         act( "$n swerves $s hand in a deadly arc as $e SmAsHeS you away from $m!!!", ch, NULL, victim, TO_VICT );
         act( "$n swerves $s hand in a deadly arc as $e SmAsHeS $N away from $m!!!", ch, NULL, victim, TO_NOTVICT );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = UPPERCUT;
         ch->attack_var = BLAST;

         hurt_person( ch, victim, 400000, TRUE );

         act( "You silently land back onto your knees...", ch, NULL, NULL, TO_CHAR );
         act( "$n silently lands back onto $s knees...", ch, NULL, NULL, TO_NOTVICT );
      }

   if ( ch->in_room->vnum < 33500 || ch->in_room->vnum > 33800 || ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      {
         act( "Your face distort in anger as you jump into the air.", ch, NULL, NULL, TO_CHAR );
         act( "$n's face distorts in anger as $e jumps into the air.", ch, NULL, NULL, TO_ROOM );
         act( "Your foot starts glowing with power as you twist yourself around.", ch, NULL, NULL, TO_CHAR );
         act( "$n's foot starts glowing with power as $e twists $mself around.", ch, NULL, NULL, TO_ROOM );
         act( "You scream '#RDIE!!#n' as you kick a MASSIVE fireball at $N!", ch, NULL, victim, TO_CHAR );
         act( "$n screams '#RDIE!!#n' as $e kicks a MASSIVE fireball at you!", ch, NULL, victim, TO_VICT );
         act( "$n screams '#RDIE!!#n' as $e kicks a MASSIVE fireball at $N!", ch, NULL, victim, TO_NOTVICT );
         act( "You stagger backwards in pain as the hungry flames consume you.", victim, NULL, NULL, TO_CHAR );
         act( "$n staggers backwards in pain as the hungry flames consume $m.", victim, NULL, NULL, TO_ROOM );

         ch->attack_type = ATTACK_PHYSICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_focus = AERIAL;
         ch->attack_var = BLAST;

         hurt_person( ch, victim, 1000000, TRUE );

         act( "You silently land back onto your knees...", ch, NULL, NULL, TO_CHAR );
         act( "$n silently lands back onto $s knees...", ch, NULL, NULL, TO_NOTVICT );
      }
}

void do_andy( CHAR_DATA *ch, char *argument )
{
   int choice, number = 92;
   CHAR_DATA *victim = ch->fighting;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 )
      number = 89;

   need_saving( ch );

   if ( number_percent() > number )
      SET_BIT( ch->more3, MORE3_RETORT );

   victim->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               victim = ch->fighting = ch->fighting->master;
            }
            else if ( ch->fighting->pIndexData->vnum < 550 || ch->fighting->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( ch->hit < ch->max_hit * 0.2 && !IS_SET( ch->cmbt[0], 4 ) )
      {
         SET_BIT( ch->cmbt[0], 4 );
         do_illusion( ch, "" );
      }

   if ( ch->hit > 5000000 && IS_SET( ch->more, MORE_FOCUS ) )
      REMOVE_BIT( ch->more, MORE_FOCUS );
      else if ( ch->hit <= 5000000 && !IS_SET( ch->more, MORE_FOCUS ) )
      {
         SET_BIT( ch->more, MORE_FOCUS );
         send_to_char("You concentrate with all of your might as your focus achieve a new height.\n\r",ch);
         act("$n's body trembles as $e focuses on $s concentration.",ch,NULL,NULL,TO_ROOM);
      }

   if ( number_percent() > 85 )
      {
         do_rising_tiger(ch,"");
         return;
      }

   if ( number_percent() > 50 )
      do_nanpa(ch,victim);

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      do_myo_suhaten(ch,victim);

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 40 )
      do_stardance(ch,"");

   if ( ch->fighting == NULL )
      return;

   if ( number_percent() > 50 )
      do_gin_sentsu(ch,victim);

   if ( ch->fighting == NULL )
      return;

   if ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 && number_percent() > 50 )
      do_pain_touch(ch,"");
}

void do_robbies_arse( CHAR_DATA *ch )
{
   act("You end your #7Angels#n song and stare long and deep into the crowd before you with a cheeky grin.",ch,NULL,NULL,TO_CHAR);
   act("Robbie Williams ends his #7Angels#n song and stares long and deep into the crowd before him with a cheeky grin.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly you spot a fan ontop of someones shoulders showing their arse to you",ch,NULL,NULL,TO_CHAR);
   act("Suddenly Robbie Williams spots a fan ontop of someones shoulders showing their arse to him",ch,NULL,NULL,TO_ROOM);
   act("You turn around and show your #zarse#n to the crowd, including $N",ch,NULL,ch->fighting,TO_CHAR);
   act("Robbie Williams turns around and shows his #zarse#n to the crowd, including $N",ch,NULL,ch->fighting,TO_NOTVICT);
   act("Robbie Williams turns around and shows his #zarsen#n to the crowd, including you",ch,NULL,ch->fighting,TO_VICT);
   act("You scream at the top of your lungs '#yYou think THAT is an #zarse?!#n #yTHIS!! Is an #zARSE!#n'",ch,NULL,NULL,TO_CHAR);
   act("Robbie Williams screams at the top of his lungs '#yYou think THAT is an #zarse?!#n #yTHIS!! Is an #zARSE!#n'",ch,NULL,NULL,TO_ROOM);
   act("The crowd goes wild and it gives you the confidence to carry on with your SLANE gig baby!",ch,NULL,NULL,TO_CHAR);
   act("The crowd goes wild and it gives Robbie Williams the confidence to carry on with his SLANE gig baby!",ch,NULL,NULL,TO_ROOM);
   act(" ",ch,NULL,NULL,TO_CHAR);
   act(" ",ch,NULL,NULL,TO_ROOM);
   act("Suddenly the stench from Robbie Williams arse #gpoisons#n you! Run away!",ch->fighting,NULL,NULL,TO_CHAR);
   act("Suddenly the stench from your arse #gpoisons#n $N!",ch,NULL,ch->fighting,TO_VICT);
   act("Suddenly the stench from Robbie Williams arse #gpoisons#n $N! Run away!",ch,NULL,ch->fighting,TO_NOTVICT);

   ch->hit = UMIN( ch->max_hit, ch->hit + 500000 );

   if ( ( IS_CLASS(ch->fighting, CLASS_DROW) && IS_SET(ch->fighting->pcdata->powers[1], DPOWER_DROWPOISON) )
        || IS_CLASS(ch->fighting,CLASS_DRAGON) || IS_CLASS(ch->fighting,CLASS_DUERGAR) )
      {
         act("You absorb the stench outputted from Robbie Williams arse!",ch->fighting,NULL,NULL,TO_CHAR);
         act("$n absorbs the stench outputted from Robbie Williams arse!",ch->fighting,NULL,NULL,TO_ROOM);
         act("$n absorbs the stench outputted from your arse!",ch->fighting,NULL,NULL,TO_NOTVICT);
         return;
      }
      else spell_dpoison( skill_lookup( "drow poison" ), 60, ch, ch->fighting );
}

void do_robbie_tiger( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	int sn;

	act("You dance in vain to your latest single 'Rock DJ' in a skimpy pair of underpants with a tiger on the front",ch,NULL,NULL,TO_CHAR);
	act("Robbie Williams dances in vain to his latest single 'Rock DJ' in a skimpy pair of underpants with a tiger on the front",ch,NULL,NULL,TO_ROOM);
	
       if ( number_percent() > 50 && !IS_SET( ch->fighting->more, MORE_BLIND ) )
	   {
          SET_BIT( ch->fighting->more, MORE_BLIND );
          ch->fighting->blind_timer = 10;
	   }

	   act("You suddenly toss your underpants on the ground and dance unbridled and free infront of everyone",ch,NULL,NULL,TO_CHAR);
	   act("Robbie Williams suddenly tosses his underpants on the ground and dances unbridled and free infront of everyone",ch,NULL,NULL,TO_ROOM);
	   act("You scream as the sight of Robbie Williams naked blinds you.. You scream '#yDirty male slut!!!#n'",ch->fighting,NULL,NULL,TO_CHAR);
	   act("$n screams as the sight of Robbie Williams naked blinds $m.. $n screams '#yDirty male slut!!!#n'",ch->fighting,NULL,NULL,TO_ROOM);
	   act("$n screams as the sight of you naked blinds $m.. $n screams '#yDirty male slut!!!#n'",ch->fighting,NULL,NULL,TO_NOTVICT);

		      if ( number_percent() > 84 )
			  {
				 sn = skill_lookup("last prayer");
				 spell_last_prayer( sn, 10, ch, ch->fighting );

                 if ( ch->fighting->outer_aura != -1 )
                 do_aura( ch->fighting,"outer none" );

                 if ( ch->fighting->inner_aura != -1 )
                 do_aura( ch->fighting,"inner none" );
			  }

			  act("You start to unzip your skin from your back as you pull your entire epidermis off yourself!",ch,NULL,NULL,TO_CHAR);
			  act("Robbie Williams starts to unzip his skin from his back as he pulls his entire epidermis off himself!",ch,NULL,NULL,TO_ROOM);
			  act("You keep dancing and posing as a man of #Rred muscle#n and #Rdark bleeding blood#n",ch,NULL,NULL,TO_CHAR);
			  act("Robbie Williams keeps dancing and posing as a man of #Rred muscle#n and #Rdark bleeding blood#n",ch,NULL,NULL,TO_ROOM);
			  act("You jump back taken by surprise and scream in #z#7fear!#n",ch->fighting,NULL,NULL,TO_CHAR);
			  act("$n jumps back taken by surprise and screams in #z#7fear!#n",ch->fighting,NULL,NULL,TO_ROOM);

                    act("You then rip your whole #Rbleeding muscular#n body apart, leaving you as a dancing skeleton",ch,NULL,NULL,TO_CHAR);
					act("Robbie Williams then rips his whole #Rbleeding muscular#n body apart, leaving him as a dancing skeleton",ch,NULL,NULL,TO_ROOM);
					act("You scream and faint in #z#7fear#n dropping to the ground in a heap",ch->fighting,NULL,NULL,TO_CHAR);
					act("$n screams and faints in #z#7fear#n dropping to the ground in a heap",ch->fighting,NULL,NULL,TO_ROOM);

                              ch->attack_type = ATTACK_PHYSICAL;
                              ch->attack_focus = DIRECT;
                              ch->attack_var = HIT;
					hurt_person( ch, ch->fighting, 1000000, TRUE );

					if ( ch->fighting != NULL && ch->fighting->position > POS_STUNNED && number_percent() > 80 )
					{
					victim = ch->fighting;
                    stop_fighting( victim, TRUE );
                    victim->position = POS_STUNNED;
					}
}

void do_robbie_mike( CHAR_DATA *ch )
{
	act("You start to flick your microphone back and forth in your hand as you smirk to yourself",ch,NULL,NULL,TO_CHAR);
	act("Robbie Williams starts to flick his microphone back and forth in his hand as he smirks to himself",ch,NULL,NULL,TO_ROOM);
	act("You then start singing '#RGive a proper giggle I'll be quite polite but when i rock the mike i rock the mike RIGHT#n'",ch,NULL,NULL,TO_CHAR);
	act("Robbie Williams starts singing '#RGive a proper giggle I'll be quite polite but when i rock the mike i rock the mike RIGHT#n'",ch,NULL,NULL,TO_ROOM);
	act("You then stare at $N and realise you feel rather offended by there presence...",ch,NULL,ch->fighting,TO_CHAR);
	act("Robbie Williams then stares at $N and realises he feels rather offended by $S presence...",ch,NULL,ch->fighting,TO_NOTVICT);
	act("Robbie Williams then stares at you and realises he feels rather offended by your presence...",ch,NULL,ch->fighting,TO_VICT);
	act("You then sing '#RYou're gonna rock the mike $N!!#n' as you promptly shove it up there arse",ch,NULL,ch->fighting,TO_CHAR);
	act("Robbie Williams then sings '#RYou're gonna rock the mike $N!!#n' as he promptly shoves it up $S arse",ch,NULL,ch->fighting,TO_NOTVICT);
	act("Robbie Williams then sings '#RYou're gonna rock the mike $N!!#n' as he promptly shoves it up your arse",ch,NULL,ch->fighting,TO_VICT);

    if ( number_percent() > 60  && !SET_BIT( ch->fighting->extra, EXTRA_TLOCK) )
	   SET_BIT( ch->fighting->extra, EXTRA_TLOCK );

	act("You roll around in agony preventing you from doing anything productive",ch->fighting,NULL,NULL,TO_CHAR);
	act("$n rolls around in agony preventing $m from doing anything productive",ch->fighting,NULL,NULL,TO_ROOM);

	if ( number_percent() > 30 )
	   {
		 ch->fighting->mana = UMAX( 0, ch->fighting->mana - 3000000 );
		 ch->fighting->move = UMAX( 0, ch->fighting->move - 3000000 );
	   }

	act("The mike in your arse suddenly winds you as you fall on the floor gasping for breath",ch->fighting,NULL,NULL,TO_CHAR);
	act("The mike in $n's arse suddenly winds $m as $e falls on the floor gasping for breath",ch->fighting,NULL,NULL,TO_ROOM);

    if ( number_percent() > 40 )
	   {
        if ( !IS_SET(ch->fighting->more,MORE_CONFUSION) )
           SET_BIT(ch->fighting->more,MORE_CONFUSION);
	   }

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_focus = DIRECT;
    ch->attack_var = HIT;
    hurt_person( ch, ch->fighting, 1000000, TRUE );
}

void do_robbie( CHAR_DATA *ch, char *argument )
{
   int choice;

   need_saving( ch );

   ch->fighting->fight_timer = 20;

   if ( IS_NPC( ch->fighting ) && number_percent() > 19 )
      {
         if ( ch->fighting->master != NULL )
            {
               act( "You savagely push $N aside with your hand as you dash at $S master!", ch, NULL, ch->fighting, TO_CHAR );
               act( "$n savagely pushes you aside with $s hand as $e dashes at your master!", ch, NULL, ch->fighting, TO_VICT );
               act( "$n savagely pushes $N aside with $s hand as $e dashes at $S master!", ch, NULL, ch->fighting, TO_NOTVICT );
               ch->fighting = ch->fighting->master;
            }
            else if ( ch->pIndexData->vnum < 550 || ch->pIndexData->vnum > 570 )
                    ch->fighting->hit = 1;
      }

   if ( number_percent() > 85 )
      {
         act("You pop some Ecstacy into your mouth and turn into a deranged party animal!",ch,NULL,NULL,TO_CHAR);
		 act("Robbie Williams pops some Ecstacy into his mouth and turns into a deranged party animal!",ch,NULL,NULL,TO_ROOM);
		 act("Your eyes glow #zbright #Rred#n as you start to knock everyones block off!",ch,NULL,NULL,TO_CHAR);
		 act("Robbie Williams eyes glow #zbright #Rred#n as he starts to knock everyones block off!",ch,NULL,NULL,TO_ROOM);
		 act(" ",ch,NULL,NULL,TO_CHAR);
		 act(" ",ch,NULL,NULL,TO_ROOM);
	    
             multi_hit( ch, ch->fighting, TYPE_UNDEFINED );
		 multi_hit( ch, ch->fighting, TYPE_UNDEFINED );
		 multi_hit( ch, ch->fighting, TYPE_UNDEFINED );
		 multi_hit( ch, ch->fighting, TYPE_UNDEFINED );
		 multi_hit( ch, ch->fighting, TYPE_UNDEFINED );
		 multi_hit( ch, ch->fighting, TYPE_UNDEFINED );
		 multi_hit( ch, ch->fighting, TYPE_UNDEFINED );

		 act(" ",ch,NULL,NULL,TO_CHAR);
		 act(" ",ch,NULL,NULL,TO_ROOM);
		 act("You start to cry, go to rehab, then go on 'anti-depressents' to stop being so depressed",ch,NULL,NULL,TO_CHAR);
		 act("Robbie Williams starts to cry, goes to rehab, then goes on 'anti-depressents' to stop being so depressed",ch,NULL,NULL,TO_ROOM);
         return;
      }

   choice = number_range(1,4);

   switch(choice)
   {
      case 2: do_robbie_tiger( ch ); break;
      case 4:
      case 1: do_robbies_arse( ch ); break;
      case 3: do_robbie_mike( ch ); break;
      default: break;
   }
}

void do_soldier( CHAR_DATA *ch, char *argument )
{
}
