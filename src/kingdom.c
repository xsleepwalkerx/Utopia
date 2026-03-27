#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

int devil_luck( CHAR_DATA *ch, CHAR_DATA *victim, int num )
{
   if ( !IS_SET( ch->cmbt[5], SKL2_DLUCK ) || IS_SET( victim->cmbt[5], SKL2_AMIND ) )
      return 0;
      else return number_range( 1, num + ( get_curr_luck( ch ) / 2 ) );
}

void do_pelt( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int direction;
  int attempt;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_SET( ch->cmbt[5], SKL2_THROW ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->ctimer[14] > 0 )
     {
        send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to throw out of the room?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   for ( attempt = 0; attempt < 12; attempt++ )
   {
      EXIT_DATA *pexit;

      if ( direction > 6 )
         direction = attempt - 6;
         else direction = number_range( 0, 5 );

      if ( ( pexit = ch->in_room->exit[direction] ) == 0 || pexit->to_room == NULL
           || IS_SET( pexit->exit_info, EX_CLOSED ) || ( IS_NPC( victim )
           && IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB ) ) )
         {
            direction = -1;
            continue;
         }

      break;
   }

   if ( direction == -1 )
      {
         stc("There are no available exits for you to throw your victim.\n\r", ch );
         return;
      }

   ch->ctimer[14] = 4;

   act( "#yYou #wquickly pick up #g$N #was you punch $M hard in the guts!#n", ch, NULL, victim, TO_CHAR );
   act( "#y$n #wquickly picks up #gyou #was $e punches you hard in the guts!#n", ch, NULL, victim, TO_VICT );
   act( "#y$n #wquickly picks up #g$N #was $e punches $M hard in the guts!#n", ch, NULL, victim, TO_NOTVICT );

   if ( IS_SET( victim->cmbt[5], SKL2_TRIP ) )
      {
          act( "#PYou #esuddenly #Rsweep #eyour legs taking #y$N #edown hard!!#n", victim, NULL, ch, TO_CHAR );
          act( "#P$n #esuddenly #Rsweeps #e$s legs taking #yyou #edown hard!!#n", victim, NULL, ch, TO_VICT );
          act( "#P$n #esuddenly #Rsweeps #e$s legs taking #y$N #edown hard!!#n", victim, NULL, ch, TO_NOTVICT );

          victim->attack_type = ATTACK_PHYSICAL;
          victim->attack_var = HIT;
          victim->attack_focus = LOW;
          hurt_person( victim, ch, 300000, TRUE );

          if ( ch == NULL || ch->position <= POS_STUNNED )
             return;

          stop_fighting( ch, TRUE );

          if ( ch->position > POS_STUNNED )                          
             ch->position = POS_STUNNED;

          return;
      }

   act( "#yYou #wthen fling #g$N #waway from you as #Chard #was you can!!#n", ch, NULL, victim, TO_CHAR );
   act( "#y$n #wthen flings #gyou #waway from $m as #Chard #was $e can!!#n", ch, NULL, victim, TO_VICT );
   act( "#y$n #wthen flings #g$N #waway from $m as #Chard #was $e can!!#n", ch, NULL, victim, TO_NOTVICT );

   special_slam( victim, direction );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = OVERHEAD;
   hurt_person( ch, victim, 600000, TRUE );

   if ( victim == NULL || victim->position <= POS_STUNNED )
      return;

   stop_fighting( victim, TRUE );

   if ( victim->position > POS_STUNNED )                          
      victim->position = POS_STUNNED;
}

void do_stunning_blow( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_SET( ch->cmbt[5], SKL2_STBLOW ) )
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

  if ( ch->stunning_timer > 0 )
     {
        send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you wish to stun?\n\r", ch );
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

   if ( is_safe( ch, victim ) )
      return;

   ch->stunning_timer = 30;

   act( "#wYou yell '#yHai!#w' as you punch #g$N #wsavagely across the stomach!#n", ch, NULL, victim, TO_CHAR );
   act( "#w$n yells '#yHai!#w' as $e punches #gyou #wsavagely across the stomach!#n", ch, NULL, victim, TO_VICT );
   act( "#w$n yells '#yHai!#w' as $e punches #g$N #wsavagely across the stomach!#n", ch, NULL, victim, TO_NOTVICT );
   act( "#gYou #wscream loudly as the #Rblow #wspins you #C#zrapidly#n #wacross the room!#n", victim, NULL, NULL, TO_CHAR );
   act( "#g$n #wscreams loudly as the #Rblow #wspins $m #C#zrapidly#n #wacross the room!#n", victim, NULL, NULL, TO_ROOM );
   act( "#gYou #wland on the ground in a #Rheap #eall #Pknocked #eout...#n", victim, NULL, NULL, TO_CHAR );
   act( "#g$n #wlands on the ground in a #Rheap #eall #Pknocked #eout...#n", victim, NULL, NULL, TO_ROOM );

   if ( victim->position <= POS_STUNNED )
      return;

   stop_fighting( victim, TRUE );

   if ( victim->position > POS_STUNNED )                          
      victim->position = POS_STUNNED;
}

void do_kingnum(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int value;

	if (IS_NPC(ch)) return;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);


	if (arg1[0] == '\0' || arg2[0] == '\0') {
	send_to_char("#7Syntax:  kingnum [char] [number]\n\r#7Set who to what kingdom?\n\r", ch );
       	return;}

	value = is_number(arg2) ? atoi(arg2) : -1;

	if ((victim = get_char_world(ch,arg1)) == NULL) {
	send_to_char("They are not here.\n\r", ch );
	return;}

	if (IS_NPC(victim)) {
	send_to_char("Not on NPCs.\n\r", ch );
	return;}

	if (value < 0 || value > MAX_KINGDOM) {
	send_to_char("That is an invalid number.\n\r", ch );
	return;}

	victim->pcdata->kingdom = value;
      remove_member( victim );
      add_member( victim );

	send_to_char("Clannum changed.\n\r", ch );
	if (IS_SET(victim->special, SPC_PRINCE))
	REMOVE_BIT(victim->special, SPC_PRINCE);

	send_to_char("Your clan has been changed.\n\r", victim);
	save_char_obj(victim);
}
	

void do_kingset(CHAR_DATA *ch, char *argument) {

	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int value;
	char *arg3;

	smash_tilde(argument);
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	arg3 = argument;

	if (IS_NPC(ch)) return;

	if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {

	send_to_char("Syntax:  clanset [kingnum] [thing] [value]\n\r\n\r", ch );
	send_to_char("things:  name, whoname, king, queen, members, recall, dungeon\n\r", ch );
	send_to_char("         pdeaths, pkills, load(if edited outside of mud)\n\r\n\r",ch);
	return;}

	value = is_number(arg1) ? atoi(arg1) : -1;

	if (value < 0 || value > MAX_KINGDOM) {
	send_to_char("That is an invalid kingdom number.\n\r", ch );
	return;}

	if (!str_cmp(arg2,"name")) {
	free_string(king_table[value].name); 
	king_table[value].name = str_dup(arg3);}
	else if (!str_cmp(arg2,"whoname")) {
	free_string(king_table[value].who_name);
	king_table[value].who_name = str_dup(arg3);}
	else if (!str_cmp(arg2,"king")) {
	free_string(king_table[value].leader_name);
	king_table[value].leader_name = str_dup(arg3);}
	else if (!str_cmp(arg2,"queen")) {
	free_string(king_table[value].leader2_name);
	king_table[value].leader2_name = str_dup(arg3);}
	else if (!str_cmp(arg2,"recall") &&  atoi(arg3) >= 0 ) {
	king_table[value].recall = atoi(arg3);}
	else if (!str_cmp(arg2,"members") &&  atoi(arg3) >= 0 ) {
	king_table[value].members = atoi(arg3);}
	else if (!str_cmp(arg2,"dungeon") && atoi(arg3) >= 0) {
	king_table[value].altar = atoi(arg3);}
	else if (!str_cmp(arg2,"pdeaths") && atoi(arg3) >= 0 ){
	king_table[value].pd = atoi(arg3);}
	else if (!str_cmp(arg2,"pkills")  && atoi(arg3) >= 0 ){
	king_table[value].pk = atoi(arg3);}
	else if (!str_cmp(arg2,"money")  && atoi(arg3) >= 0 ){
	king_table[value].money = atoi(arg3);}
	else if (!str_cmp(arg2,"load") ){
	load_kingdoms();}
	else 
		send_to_char("Nothing set.\n\r", ch );

	send_to_char("Done.\n\r", ch );
	save_kingdoms();
}

void load_kingdoms(void) 
{
   FILE *fp;
   int i;

   king_table[0].name = "";
   king_table[0].who_name = "";
   king_table[0].leader_name = "";
   king_table[0].leader2_name = "";
   king_table[0].pk = 0;
   king_table[0].wk = 0;
   king_table[0].pd = 0;
   king_table[0].members = 0;
   king_table[0].num = 0;
   king_table[0].recall = 0;
   king_table[0].object = NULL;
   king_table[0].altar = 0;
   king_table[0].money = 0;

   if ( ( fp = fopen("../area/kingdom.dat", "r") ) == NULL )
      {
         log_string("Error reading from kingdom.dat");
         return;
      }

   mud_version = fread_number(fp);

   for ( i = 1; i < MAX_KINGDOM; i++ )
   {
      king_table[i].name = fread_string(fp);
      king_table[i].who_name = fread_string(fp);
      king_table[i].leader_name = fread_string(fp);
      king_table[i].leader2_name = fread_string(fp);
      king_table[i].members = fread_number(fp);
      king_table[i].pk = fread_number(fp);
      king_table[i].pd = fread_number(fp);
      king_table[i].recall = fread_number(fp);
      king_table[i].altar = fread_number(fp);
      king_table[i].money = fread_number(fp);
      king_table[i].num = i;
      king_table[i].wk = 0;
   }	

   fclose(fp);
}

void save_kingdoms(void) 
{
   FILE *fp;
   int i;

   return;

   if ( ( fp = fopen("../area/kingdom.dat","w")) == NULL )
      {
         log_string("Error writing to kingdom.dat");
         return;
      }

   fprintf(fp,"%d\n",mud_version);

   for (i = 1; i < MAX_KINGDOM; i++) 
       {
          fprintf(fp,"%s~\n", king_table[i].name);
          fprintf(fp,"%s~\n", king_table[i].who_name);
          fprintf(fp,"%s~\n", king_table[i].leader_name);
          fprintf(fp,"%s~\n", king_table[i].leader2_name);
          fprintf(fp,"%d\n",king_table[i].members);
          fprintf(fp,"%d\n%d\n",king_table[i].pk, king_table[i].pd);
          fprintf(fp,"%d\n%d\n%d\n",king_table[i].recall,king_table[i].altar,king_table[i].money);
       }

   fclose(fp);
}

void do_kingdomlist(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_STRING_LENGTH];
   int i;

   if ( IS_NPC(ch) )
      return;

   sprintf(buf, "#wKingdom            Leader         PKs     PDs     Members\n\r" );
   send_to_char(buf,ch);
   sprintf(buf, "#y-------            ------         ---     ---     -------\n\r" );
   send_to_char(buf,ch);
   
   for ( i = 1; i < 5; i++ ) 
   {
      sprintf(buf, "#g%-18s %-14s %-7d %-7d %-2d\n\r", 
                   king_table[i].name,
                   king_table[i].leader_name,
                   king_table[i].pk,
                   king_table[i].pd,
                   king_table[i].members );
      
      send_to_char(buf,ch);
   }

   send_to_char("#n\n\r", ch );
}

void do_induct(CHAR_DATA *ch, char *argument) 
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   argument = one_argument(argument,arg);

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom == 0 )
      {
         send_to_char("You are not in a kingdom.\n\r", ch );
         return;
      }

   if ( king_table[ch->pcdata->kingdom].members > 14 )
      {
         send_to_char("Your kingdom have the maximum members allowed.\n\r", ch );
	   return;
      }

   if ( str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)
        && str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader2_name)
        && !IS_SET(ch->special, SPC_PRINCE) )
      {
         send_to_char("You cannot induct people.\n\r", ch );
         return;
      }

   if ( ( victim = get_char_room(ch, arg) ) == NULL )
      {
         send_to_char("They are not here.\n\r", ch );
         return;
      }

   if ( IS_NPC(victim) )
      {
         send_to_char("Not on NPCs.\n\r", ch );
         return;
      }

   if ( !IS_SET(victim->newbits, NEW_CLANNABLE) )
      {
         send_to_char("That person is not CLANNABLE.\n\r", ch );
         return;
      }

   if ( victim->pcdata->kingdom != 0 )
      {
         send_to_char("That person is already clanned.\n\r", ch );
         return;
      }

   victim->pcdata->kingdom = ch->pcdata->kingdom;

   if ( IS_SET(victim->special, SPC_PRINCE) )
      REMOVE_BIT(victim->special, SPC_PRINCE);

   send_to_char("They are now in your clan.\n\r", ch);
   send_to_char("You are now in a clan.\n\r", victim);
   save_char_obj(victim);

   king_table[ch->pcdata->kingdom].members++;
   save_kingdoms();
}

void do_rogue(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_STRING_LENGTH];

   if ( IS_NPC( ch ) )
      return;

   if ( ch->pcdata->kingdom > 0 )
      {
         send_to_char("You are already in some type of kingdom or organization.\n\r", ch );
         return;
      }

   if ( !IS_SET(ch->newbits, NEW_CLANNABLE) )
      {
         send_to_char("You aren't CLANNABLE.\n\r", ch );
         return;
      }

   ch->pcdata->kingdom = 5;

   if ( IS_SET(ch->special, SPC_PRINCE) )
      REMOVE_BIT(ch->special, SPC_PRINCE);

   send_to_char("#wYou have become a rogue, vowing to put an end to all kingdoms.#n\n\r", ch);
   save_char_obj(ch);

   sprintf( buf, "#w%s#g has become a #erogue#g! #RKill #gthis traitor at will#g!#n", ch->name );
   do_info( ch, buf );

   king_table[ch->pcdata->kingdom].members++;
   save_kingdoms();
   remove_member( ch );
   add_member( ch );
}

void do_outcast(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom == 0 )
      {
         send_to_char("You are not in a kingdom.\n\r", ch );
         return;
      }

   victim = ch;
   sprintf( buf, "#w%s#g has been #youtcasted#g from their #Cclan#g!#n", victim->name );
   do_info( victim, buf );

   send_to_char("#wYou #gleave your kingdom in #ydisgust#g.#n\n\r", victim );
   king_table[victim->pcdata->kingdom].members--;
   free_string( ch->pcdata->title );
   ch->pcdata->title = str_dup( "" );

   victim->pcdata->kingdom = 0;

   if ( IS_SET( victim->special, SPC_PRINCE ) )
      REMOVE_BIT( victim->special, SPC_PRINCE );

   if ( IS_CLASS( ch, CLASS_PALADIN ) || IS_CLASS( ch, CLASS_ANGEL ) || IS_CLASS( ch, CLASS_MONK )
        || IS_CLASS( ch, CLASS_DARAMON ) )
      ch->alignment = 1000;
      else if ( IS_CLASS( ch, CLASS_DEMON ) || IS_CLASS( ch, CLASS_VAMPIRE ) || IS_CLASS( ch, CLASS_DROW )
                || IS_CLASS( ch, CLASS_NECROMANCER ) || IS_CLASS( ch, CLASS_UNDEAD_KNIGHT )
                || IS_CLASS( ch, CLASS_SHADOW_WAR ) || IS_CLASS( ch, CLASS_DUERGAR ) )
              ch->alignment = -1000;
              else ch->alignment = 0;

   remove_member( victim );
   add_member( victim );
   save_kingdoms();
}

void do_prince(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char arg[MAX_STRING_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom == 0 )
      {
         send_to_char("You are not in a clan.\n\r", ch );
         return;
      }

   if ( str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)
        && str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader2_name) )
      {
         send_to_char("You are not the leader of your clan.\n\r", ch );
         return;
      }

   if ( ( victim = get_char_room(ch, arg) ) == NULL )
      {
         send_to_char("They are not here.\n\r", ch );
         return;
      }

   if ( IS_NPC(victim) )
      {
         send_to_char("Not on NPCs.\n\r", ch );
         return;
      }

   if ( victim->pcdata->kingdom != ch->pcdata->kingdom ) 
      {
         send_to_char("They are not in your clan.\n\r", ch );
         return;
      }

   if ( ch == victim )
      {
         send_to_char("You can't do that.\n\r", ch );
         return;
      }

   if ( IS_SET(victim->special, SPC_PRINCE) )
      {
         REMOVE_BIT(victim->special, SPC_PRINCE);
         send_to_char("You are no longer a prince.\n\r", victim);
         send_to_char("They are no longer a prince.\n\r", ch );
         save_char_obj(victim);
         return;
      }

   SET_BIT(victim->special, SPC_PRINCE);
   send_to_char("You are now a prince!\n\r", victim);
   send_to_char("You make them a prince.\n\r", ch );
   save_char_obj(victim);
}

void do_kingdom(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *gch;
   char rankname[9];

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom == 0 )
      {
         send_to_char("You are not in a kingdom.\n\r", ch );
         return;
      }

   sprintf( buf, "#w%s's #gKingdom #cof#n %s:\n\r", king_table[ch->pcdata->kingdom].leader_name,
                 king_table[ch->pcdata->kingdom].name);
   send_to_char( buf, ch );
   send_to_char("#2[    Name    ] [ Hits ] [ HPmax ] [ Mana ] [ Move ] [ Status ] [  Rank  ]#n\n\r", ch );

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
      if ( IS_NPC(gch) ) 
         continue;

      if ( gch->pcdata->kingdom != ch->pcdata->kingdom )
         continue;

      if ( !str_cmp(king_table[ch->pcdata->kingdom].leader_name,gch->pcdata->switchname) ) 
         sprintf(rankname,"King");
         else if ( !str_cmp(king_table[ch->pcdata->kingdom].leader2_name,gch->pcdata->switchname) )
                 sprintf(rankname,"Queen");
         else if ( IS_SET(gch->special, SPC_PRINCE) && gch->sex == SEX_FEMALE ) 
                 sprintf(rankname, "Princess");
         else if ( IS_SET(gch->special, SPC_PRINCE) ) 
                 sprintf(rankname, "Prince");
         else sprintf(rankname,"Member");

       sprintf( buf, "#2[#n%-12s#2] [#n%6d#2] [#n%7d#2] [#n%6d#2] [#n%6d#2] [#n   %-2d   #2] [ #n%-6s #2]#n\n\r",
                     gch->pcdata->switchname ,
                     gch->hit,gch->max_hit,gch->mana,gch->move,
                     gch->race, rankname);
       send_to_char( buf, ch );
   }

   sprintf( buf, "\n\r#wTotal #PFunds #w- #g$%d#n\n\r", king_table[ch->pcdata->kingdom].money );
   send_to_char( buf, ch );
}

void do_krecall(CHAR_DATA *ch, char *argument) 
{
   CHAR_DATA *victim;
   CHAR_DATA *mount;
   ROOM_INDEX_DATA *location;

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom == 0 )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
      {
         send_to_char( "As hard as you try, there is no way to escape this vortex.\n\r", ch );
         return;
      }

   if ( is_affected(ch, skill_lookup("curse of allah") ) )
      {
         stc("God prevents you from using this mystical ability.\n\r",ch);
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc("Not with a fight timer.\n\r",ch);
         return;
      }

   act("$n's body flickers with green energy.",ch,NULL,NULL,TO_ROOM);
   act("Your body flickers with green energy.",ch,NULL,NULL,TO_CHAR);

   if ( ( location = get_room_index(king_table[ch->pcdata->kingdom].recall) ) == NULL )
      {
         send_to_char("You are completely lost.\n\r", ch );
         return;
      }

   if ( ch->in_room == location )
      return;

   if ( IS_AFFECTED(ch, AFF_CURSE) )
      {
         send_to_char("You are unable to recall.\n\r", ch );
         return;
      }

   if ( ch->fighting != NULL )
      stop_fighting(ch,TRUE);
   
   act("$n dissapears.",ch,NULL,NULL,TO_ROOM);
   char_from_room(ch);
   char_to_room(ch,location);
   act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
   do_look(ch,"auto");

   if ( (mount = ch->mount) == NULL )
      return;
 
   char_from_room(mount);
   char_to_room(mount,ch->in_room);
}

void do_withdrawqps(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_STRING_LENGTH];
   int amt;
	
   if ( IS_NPC(ch) )
      return;
	
   amt = atoi(argument);

   if ( ch->in_room->vnum != 7029 && ch->in_room->vnum != 3031
        && ch->in_room->vnum != 2022 && ch->in_room->vnum != 4026
        && ch->in_room->vnum != 17029 && ch->in_room->vnum != 16003 && ch->in_room->vnum != 800
        && ch->in_room->vnum != 15029 && ch->in_room->vnum != 14031 )
      {
         stc("But you aren't at a bank.\n\r",ch);
         return;
      }

   if ( auction_list != NULL && auction_list->buyer == ch )
      {
         stc("You can't withdraw any money as long as you are the current bidder.\n\r",ch);
         return;
      }

   if ( amt < 1 )
      {
         send_to_char("Nope, more than 0 please.\n\r",ch);
         return;
      }

   if ( amt > ch->bank )
      {
         send_to_char("*grin*, there isn't that much in the account!\n\r",ch);
         return;
      }
	
   sprintf(buf,"You withdraw #w%d #ygold pieces#n from the bank.",amt);
   send_to_char(buf,ch);
		
   ch->gold += amt;
   ch->bank -= amt;

   save_char_obj(ch);
}

void do_depositqps(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_STRING_LENGTH];
   int amt;
	
   if ( IS_NPC(ch) )
      return;

   if ( ch->in_room->vnum != 7029 && ch->in_room->vnum != 4026
        && ch->in_room->vnum != 17029 && ch->in_room->vnum != 16003 && ch->in_room->vnum != 800
        && ch->in_room->vnum != 15029 && ch->in_room->vnum != 14031 )
      {
         stc("But you aren't at a bank.\n\r",ch);
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc( "#w#zYou are currently in a battle, so the bank is shut tightly.#n\n\r", ch );
         return;
      }
	
   amt = atoi(argument);

   if ( amt < 1 )
      {
         send_to_char("Uhmm yeah, sure, thanks for the deposit... NOT!\n\r",ch);
         return;
      }

   if ( amt > ch->gold )
      {
         send_to_char("My my my, you have philanthropy down to an artform?\n\rYou deposit more then you own!\n\r",ch);
         return;
      }
	
   sprintf(buf,"You deposit #w%d #ygold pieces#n into the bank.\n\r",amt);
   send_to_char(buf,ch);

   ch->gold -= amt;
   ch->bank += amt;

   check_leaderboard(ch,"");
   save_char_obj(ch);
}

void do_kdeposit(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_STRING_LENGTH];
   int amt;
	
   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom == 0 || ch->pcdata->kingdom > 2 )
      {
         stc( "Only kingdom members can deposit money into their banks.\n\r", ch );
         return;
      }

   if ( ch->pcdata->kingdom == 1 && ch->in_room->vnum != 2022 )
      {
         stc("But you aren't at your kingdom treasury.\n\r",ch);
         return;
      }

   if ( ch->pcdata->kingdom == 3 && ch->in_room->vnum != 3031 )
      {
         stc("But you aren't at your kingdom treasury.\n\r",ch);
         return;
      }

   if ( ch->pcdata->kingdom == 2 && ch->in_room->vnum != 4026 )
      {
         stc("But you aren't at your kingdom treasury.\n\r",ch);
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         stc( "#w#zYou are currently in a battle, so the treasury is shut tightly.#n\n\r", ch );
         return;
      }
	
   amt = atoi(argument);

   if ( amt < 1 )
      {
         send_to_char("Uhmm yeah, sure, thanks for the deposit... NOT!\n\r",ch);
         return;
      }

   if ( amt > ch->gold )
      {
         send_to_char("My my my, you have philanthropy down to an artform?\n\rYou deposit more then you own!\n\r",ch);
         return;
      }
	
   sprintf(buf,"You deposit #w%d #ygold pieces#n into the kingdom.\n\r",amt);
   send_to_char(buf,ch);

   ch->gold -= amt;
   king_table[ch->pcdata->kingdom].money = UMIN( 500000000, king_table[ch->pcdata->kingdom].money + amt );

   save_kingdoms();
   save_char_obj(ch);
}

void do_pknow(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_INPUT_LENGTH];

   if ( (get_age(ch)-17) >= 2 )
      {
         stc("But you aren't a newbie.\n\r",ch);
         return;
      }

   ch->played = 33000;

   stc("Ok!!! You are now ready to PK!!!\n\r",ch);
   sprintf( buf, "#w%s#g is ready to #ypk#g!#n", ch->name );
   do_info(ch,buf);
}

void do_deto(CHAR_DATA *ch, char *argument) 
{
   send_to_char("IF you want to detox, you must type this fully.\n\r", ch );
}

void do_detox(CHAR_DATA *ch, char *argument) 
{
   char buf[MAX_INPUT_LENGTH];
   int count;
   CHAR_DATA *mount;

   if ( IS_NPC(ch) )
      return;

   if ( ch->class == 0 )
      {
         send_to_char("You are not in a class.\n\r", ch );
         return;
      }

   if ( ch->fight_timer > 0 )
      {
         send_to_char("Not with a fight timer.\n\r", ch );
         return;
      }

   if ( ch->pcdata->kingdom > 0 )
      do_outcast( ch, "" );

   if ( IS_SET(ch->newbits, NEW_DARKNESS) )
      {
         if ( ch->in_room->drows > 0 )
            ch->in_room->drows--;
      }
  
   if ( ch->in_room->drows == 0 && IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )  
      REMOVE_BIT( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS );  

   if ( (mount = ch->mount) != NULL )
      do_dismount( ch, "" );

   die_follower( ch );

   while ( ch->affected )
      affect_remove(ch,ch->affected);

   do_remove(ch,"all");

   do_update_class_count( ch->class, FALSE );
   do_update_class_count( ch->class_2, FALSE );
   do_update_class_count( ch->class_3, FALSE );
   do_update_class_count( ch->class_4, FALSE );
   do_update_class_count( ch->class_5, FALSE );

   remove_member(ch);

   free_string( ch->pcdata->title );
   ch->pcdata->title = str_dup( "" );

   if ( IS_CLASS( ch, CLASS_ANGEL ) )
      angels--;

   REMOVE_BIT( ch->more, MORE_BARRIER );
   REMOVE_BIT( ch->extra, EXTRA_DETERMINE );
   REMOVE_BIT( ch->extra, EXTRA_FLASH );

   free_string( ch->groove );
   ch->groove = str_dup( "" );
   ch->garou1     = 0;
   ch->clannum    = 0;
   ch->class      = 0;
   ch->class_2    = 0;
   ch->class_3    = 0;
   ch->class_4    = 0;
   ch->class_5    = 0;
   ch->more       = 0;
   ch->more2      = 0;
   ch->more3      = 0;
   ch->drill      = 0;
   ch->special    = 0;
   ch->newbits    = 12;
   ch->powerlevel = 0;
   ch->itemaffect = 0;
   ch->outer_aura = -1;
   ch->inner_aura = -1;
   ch->pcdata->perm_str  = 5;
   ch->pcdata->perm_int  = 5;
   ch->pcdata->perm_wis  = 5;
   ch->pcdata->perm_dex  = 5;
   ch->pcdata->perm_con  = 5;
   ch->pcdata->perm_eva  = 5;
   ch->immune     = 0;
   ch->rage       = 0;
   ch->flag2      = 0;
   ch->polyaff    = 0;
   ch->safe_timer = 5;
   ch->pkill = ch->pdeath = 0;
   ch->login_timer = 4;
   ch->pcdata->deity = 0;
   ch->pcdata->personality = 0;
   ch->generation = 5;
   ch->race = 0;
   ch->saiyanlevel = ch->saiyan_powerlevel = 0;
   ch->fire_prof = 0;
   ch->lunar_prof = 0;
   ch->ice_prof = 0;
   ch->bolt_prof = 0;
   ch->earth_prof = 0;
   ch->holy_prof = 0;
   ch->dark_prof = 0;
   ch->natural_prof = 0;
   ch->physical_prof = 0;
   ch->magical_prof = 0;
   ch->slash_prof = 0;
   ch->pierce_prof = 0;
   ch->blast_prof = 0;
   ch->beam_prof = 0;
   ch->spirit_prof = 0;
   ch->blunt_prof = 0;
   ch->normal_prof = 0;
   ch->lunar_bonus = 0;
   ch->dark_bonus = 0;
   ch->holy_bonus = 0;
   ch->fire_bonus = 0;
   ch->ice_bonus = 0;
   ch->bolt_bonus = 0;
   ch->earth_bonus = 0;

   if ( ch->binding != NULL )
      {
         act("#wYou stop guarding $N with your spirit.#n",ch,NULL,ch->binding,TO_CHAR);
         act("#w$n stops guarding you with $s spirit.#n",ch,NULL,ch->binding,TO_VICT);
         ch->binding->binded = NULL;
         ch->binding = NULL;
      }

   if ( ch->topmode > 0 )
      ch->topmode = 0;

   if ( ch->pcdata->kingdom > 0 )
      add_member(ch);

   for ( count = 0; count < 8; count++ )
       ch->cmbt[count] = 0;

   for ( count = 0; count < 10; count++ )
       ch->pcdata->ninja_powers[count] = 0;

   for ( count = 0; count < 13; count++ )
   {
      if ( ch->wpn[count] > 200 )
         ch->wpn[count] = 200;
   }

   for ( count = 0; count < 5; count++ )
   {
      if ( ch->spl[count] > 200 )
         ch->spl[count] = 200;
   }

   for ( count = 1; count < 21; count++ )
       ch->gifts[count] = 0;

   for ( count = 1; count < 9; count++ )
       ch->spheres[count] = 0;

   for ( count = 0; count < 4; count++ )
   {
      ch->monkab[count] = 0;
      ch->paladinab[count] = 0;
      ch->angelab[count] = 0;
      ch->sorcerorab[count] = 0;
   }

   for ( count = 0; count < 8; count++ )
       ch->mlist[count] = 0;
 
   for ( count = 0; count < 2; count++ )
       ch->focus[count] = 0;

   for ( count = 0; count < 40; count++ )
       ch->aura[count] = 0;

   ch->alignment = 0;
   ch->elemental = 0;
   ch->tier = 1;
   ch->pcdata->legend = 0;

   ch->hit = ch->max_hit = UMIN( 10000000 + get_translevel( ch ), ch->max_hit );
   ch->mana = ch->max_mana = UMIN( 10000000 + ( get_translevel( ch ) / 2 ), ch->max_mana );
   ch->move = ch->max_move = UMIN( 10000000 + ( get_translevel( ch ) / 2 ), ch->max_move );

   ch->practice = 30 + ( get_translevel( ch ) / 100000 );

   for ( count = 0; count < 20; count++ )
       ch->pcdata->powers[count] = 0;

   for ( count = 0; count < 12; count++ )
       ch->pcdata->stats[count] = 0;

   free_string( ch->morph );
   ch->morph = str_dup( "" );

   free_string( ch->lord );
   ch->lord = str_dup( "" );

   if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

   free_string( ch->clan );
   ch->clan = str_dup( "" );

   if ( IS_IMMORTAL(ch) || ch->level == 1 )
      return;

   ch->home = 800;
   send_to_char("You has left your class organization.\n\r", ch );

   sprintf( buf, "#w%s#g #ydetoxes#g from their #Cclass#g!#n", ch->name );
   do_info(ch,buf);

   save_char_obj( ch );
}

void load_members() 
{
   FILE *fp;
   int i;

   if ( ( fp = fopen("../area/members.dat", "r") ) == NULL )
      {
         for ( i = 0; i < 1000; i++ )
             {
                member[i].class = 0;
                member[i].remort = 0;
                member[i].trimort = 0;
                member[i].quamort = 0;
                member[i].quimort = 0;
                free_string( member[i].name );
                member[i].name = str_dup( "" );
             }

         return;
      }

   for ( i = 0; i < 1000; i++ )
   {
      member[i].name = fread_string(fp);
      member[i].class = fread_number(fp);
      member[i].remort = fread_number(fp);
      member[i].trimort = fread_number(fp);
      member[i].quamort = fread_number(fp);
      member[i].quimort = fread_number(fp);
   }	

   fclose(fp);
}

void save_members() 
{
   FILE *fp;
   int i;

   if ( ( fp = fopen("../area/members.dat","w") ) == NULL )
      {
         log_string("Error writing to members.dat");
         return;
      }

   for ( i = 0; i < 1000; i++ ) 
   {
      fprintf(fp,"%s~\n", member[i].name);
      fprintf(fp,"%d %d %d %d %d\n", member[i].class, member[i].remort, member[i].trimort, 
                                     member[i].quamort, member[i].quimort);
   }

   fclose(fp);
}

void do_memberlist(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   int i, class;
   int count = 0;
   bool found = FALSE;

   if ( IS_NPC(ch) )
      return;

   argument = one_argument(argument, arg);

   if ( !str_cmp("demon",arg) )
      class = 1;
      else if ( !str_cmp("werewolf",arg) )
              class = 3;
      else if ( !str_cmp("vampire",arg) )
              class = 4;
      else if ( !str_cmp("drow",arg) )
              class = 6;
      else if ( !str_cmp("monk",arg) )
              class = 7;
      else if ( !str_cmp("weapons master",arg) )
              class = 44;
      else if ( !str_cmp("ninja",arg) )
              class = 8;
      else if ( !str_cmp("angel",arg) )
              class = 9;
      else if ( !str_cmp("dragon",arg) )
              class = 11;
      else if ( !str_cmp("paladin",arg) )
              class = 12;
      else if ( !str_cmp("undead knight",arg) || !str_cmp("undeadknight",arg) )
              class = 19;
      else if ( !str_cmp("saiyan",arg) || !str_cmp("saiya-jin",arg) )
              class = 21;
      else if ( !str_cmp("wizard",arg) )
              class = 23;
      else if ( !str_cmp("necromancer",arg) )
              class = 25;
      else if ( !str_cmp("samurai",arg) )
              class = 22;
      else if ( !str_cmp("snow elf",arg) )
              class = 39;
      else if ( !str_cmp("hobbit",arg) )
              class = 38;
      else if ( !str_cmp("phoenix",arg) )
              class = 40;
      else if ( !str_cmp("wisp",arg) )
              class = 41;
      else if ( !str_cmp("duergar",arg) )
              class = 42;
      else if ( !str_cmp("shadow warrior",arg) || !str_cmp("shadowwarrior",arg) )
              class = 43;
      else if ( !str_cmp("assassin",arg) )
              class = 28;
      else if ( !str_cmp("clone",arg) || !str_cmp("sorceror",arg)
                || !str_cmp("darkknight",arg) || !str_cmp("dark knight",arg) )
              class = 27;
      else if ( !str_cmp("daramon",arg) )
              class = 26;
      else if ( !str_cmp("daywalker",arg) )
              class = 18;
      else if ( !str_cmp("highwind",arg) )
              class = 10;
      else if ( !str_cmp("dragoon",arg) )
              class = 36;
      else if ( !str_cmp("commander",arg) )
              class = 31;
      else if ( !str_cmp("fiend",arg) )
              class = 37;
      else if ( !str_cmp("kingdom1",arg) )
              class = 51;
      else if ( !str_cmp("kingdom2",arg) )
              class = 52;
      else if ( !str_cmp("kingdom3",arg) )
              class = 53;
      else if ( !str_cmp("kingdom4",arg) )
              class = 54;
      else if ( !str_cmp("kingdom5",arg) )
              class = 55;
              else {
                      stc("Please enter a valid class name.\n\r",ch);
                      return;
                   }

   if ( class > 50 )
      {
         class -= 50;

         sprintf( buf, "#wMembers of %s:#n\n\r\n\r", king_table[ class ].who_name );
         stc(buf,ch);

         for ( i = 0; i < 1000; i++ )
         {
             if ( member[i].quimort == class )
                {
                   found = TRUE;

                   if ( count == 0 )
                      sprintf( buf, "#y%-12s    ", member[i].name );
                      else if ( count == 1 )
                              sprintf( buf, "#g%-12s    ", member[i].name );
                      else if ( count == 2 )
                              sprintf( buf, "#C%-12s    ", member[i].name );
                              else sprintf( buf, "#P%-12s    ", member[i].name );

                   stc(buf,ch);
                   count++;
    
                  if ( count == 4 )
                     {
                        count = 0;
                        stc("\n\r",ch);
                     }
                }
         }

         if ( found == FALSE )
            {
               stc("#yNo #eone #Pfound#y...#n\n\r",ch);
            }
            else stc("\n\r",ch);

         return;
      }

   sprintf( buf, "#wMembers of the #R%s #wclass:#n\n\r\n\r", arg );
   stc(buf,ch);

   for ( i = 0; i < 1000; i++ )
   {
      if ( member[i].class == class || member[i].remort == class || member[i].trimort == class
           || member[i].quamort == class )
         {
            found = TRUE;

            if ( count == 0 )
               sprintf( buf, "#y%-12s    ", member[i].name );
               else if ( count == 1 )
                       sprintf( buf, "#g%-12s    ", member[i].name );
                       else if ( count == 2 )
                               sprintf( buf, "#C%-12s    ", member[i].name );
                               else sprintf( buf, "#P%-12s    ", member[i].name );

            stc(buf,ch);

            count++;
            if ( count == 4 )
               {
                  count = 0;
                  stc("\n\r",ch);
               }
         }
   }

   if ( found == FALSE )
      {
         stc("#yNo #eone #Pfound#y...#n\n\r",ch);
      }
      else stc("\n\r",ch);
}

void add_member( CHAR_DATA *ch )
{
   int i = 0;

   while ( member[i].class != 0 && i < 1000 )
         i++;

   if ( i == 1000 )
      { 
	   log_string("Error: Member List is full!");
         return;
      }

   member[i].class = ch->class;
   member[i].remort = ch->class_2;
   member[i].trimort = ch->class_3;
   member[i].quamort = ch->class_4;
   member[i].quimort = ch->pcdata->kingdom;
   free_string( member[i].name );
   member[i].name = str_dup ( ch->pcdata->switchname );
   save_members();
}

void remove_member( CHAR_DATA *ch )
{
   int i = -1;

   while ( i < 1000 )
   {
      i++;

      if ( i > 999 || str_cmp( ch->pcdata->switchname, member[i].name ) )
         continue;

      member[i].class = 0;
      member[i].remort = 0;
      member[i].trimort = 0;
      member[i].quamort = 0;
      member[i].quimort = 0;
      free_string( member[i].name );
      member[i].name = str_dup( "" );
   }

   save_members();
}

void do_clan_recall( CHAR_DATA *ch, char *argument )
{
   int recall;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_SET(ch->cmbt[6], IN_CLAN) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->fight_timer != 0 )
      {
         stc("Not with an active fight timer.\n\r",ch);
         return;
      }

   switch( ch->class )
   {
      default: recall = -1; break;
      case CLASS_NINJA: recall = 33200; break;
      case CLASS_DROW: recall = 33206; break;
      case CLASS_VAMPIRE: recall = 33218; break;
      case CLASS_DRAGON: recall = 33224; break;
      case CLASS_WIZARD: recall = 33230; break;
      case CLASS_PALADIN: recall = 33236; break;
      case CLASS_SORCEROR: recall = 33242; break;
   }

   if ( recall == -1 )
      {
         stc("Your clan recall isn't in yet.\n\r",ch);
         return;
      }

   act("$n is whisked away by a #Rr#wo#Ra#wr#Ri#wn#Rg #ywind#n!",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room( ch, get_room_index(recall) );

   ch->wait = 12;

   act("You are whisked away by a #Rr#wo#Ra#wr#Ri#wn#Rg #ywind#n!",ch,NULL,NULL,TO_CHAR);
   act("$n is brought into the room by a #Rr#wo#Ra#wr#Ri#wn#Rg #ywind#n!",ch,NULL,NULL,TO_ROOM);

   do_look(ch,"");
}

void do_mystic_warp( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( ch->in_room->vnum == 800 )
      {
         stc("But you are already at the Crystal Cathedral.\n\r",ch);
         return;
      }

   if ( ch->fight_timer != 0 )
      {
         stc("Not with an active fight timer.\n\r",ch);
         return;
      }

   if ( armageddon == TRUE )
      {
         send_to_char("You can't warp while the world is in #rC#eh#Ra#eo#rs#n!!\n\r", ch );
         return;
      }

   act("$n disappears in a #ws#eh#wi#em#wm#ee#wr#ei#wn#eg #cvortex#n.",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room( ch, get_room_index(800) );

   ch->wait = 12;

   if ( IS_AFFECTED(ch, AFF_ETHEREAL) )
      REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

   if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
      REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);

   act("You step through a #ws#eh#wi#em#wm#ee#wr#ei#wn#eg #cvortex#n.",ch,NULL,NULL,TO_CHAR);
   act("$n arrives from a #ws#eh#wi#em#wm#ee#wr#ei#wn#eg #cvortex#n.",ch,NULL,NULL,TO_ROOM);

   do_look(ch,"");
}

void do_obtain(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   char * deity_table[10] = { "#CShiva", "#RIfrit", "#wSiren", "#eDiablos", "#gTitan", "#yQuezacotl", "#pOdin", "#CCarbunkle", "#cLuna", "#oGaia" };
   DESCRIPTOR_DATA *d;

   if ( IS_NPC( ch ) || ch->pcdata->deity == 0 )
      {
         stc( "#wYou are a lone wolf... begone!#n\n\r", ch );
         return;
      }

   if ( !IS_NPC(ch) && (get_age(ch)-17) < 2 )
      {
         stc("#wBut you are a #Pnewbie#w.#n\n\r",ch);
         return;
      }

   if ( ch->raiding > 0 )
      {
         stc( "#wFrustrated at the #yintervention#w, you stop your #Rinvasion #wfor now.#n\n\r", ch );
         ch->raiding = 0;
         return;
      }

   if ( ch->in_room->area->land == ch->pcdata->deity )
      {
         stc( "#wThis #yland #wis already under the protection of your #CDeity#w.#n\n\r", ch );
         return;
      }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->character != NULL && d->character != ch && d->character->in_room != NULL
           && d->character->in_room->area == ch->in_room->area && d->character->raiding > 0 )
         {
            stc( "#wSome #Pclan #wis already invading the #oland!#n\n\r", ch );
            return;
         }
   }

   ch->raiding = 25;
   stc( "#wYou yell your #CDeity's #wname proudly as you wreck havoc upon the land.#n\n\r", ch );

   sprintf( buf, "#gThe #Cclan #gof %s #gis #Rinvading #gthe #oland #gof #P%s#g!!!#n", deity_table[ ch->pcdata->deity - 1 ], ch->in_room->area->name );
   do_info( ch, buf );
}

void do_lands(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char * deity_table[10] = { "#CShiva", "#RIfrit", "#wSiren", "#eDiablos", "#gTitan", "#yQuezacotl", "#pOdin", "#CCarbunkle", "#cLuna", "#oGaia" };
   AREA_DATA *pArea;
   int option, total = 0;

   argument = one_argument(argument, arg);

   if ( arg[0] == '\0' )
      {
         int ifrit = 0, shiva = 0, bird = 0, titan = 0, siren = 0, diablos = 0, odin = 0, carb = 0, luna = 0, gaia = 0;

         for ( pArea = area_first; pArea; pArea = pArea->next )
         {
            switch( pArea->land )
            {
               default: break;
               case 1: shiva++; break;
               case 2: ifrit++; break;
               case 3: siren++; break;
               case 4: diablos++; break;
               case 5: titan++; break;
               case 6: bird++; break;
               case 7: odin++; break;
               case 8: carb++; break;
               case 9: luna++; break;
               case 10: gaia++; break;
            }
         }

         stc( "#y+++++++++++++++++++++#wLANDS OF UTOPIA#y+++++++++++++++++++++\n\r", ch );
         sprintf( buf, "#y+                      #CShiva:     #C%2d                    #y+\n\r", shiva );
         stc( buf, ch );
         sprintf( buf, "#y+                      #RIfrit:     #C%2d                    #y+\n\r", ifrit );
         stc( buf, ch );
         sprintf( buf, "#y+                      #wSiren:     #C%2d                    #y+\n\r", siren ); 
         stc( buf, ch );
         sprintf( buf, "#y+                    #eDiablos:     #C%2d                    #y+\n\r", diablos );
         stc( buf, ch );
         sprintf( buf, "#y+                      #gTitan:     #C%2d                    #y+\n\r", titan ); 
         stc( buf, ch );
         sprintf( buf, "#y+                  #yQuezacotl:     #C%2d                    #y+\n\r", bird );
         stc( buf, ch );
         sprintf( buf, "#y+                       #pOdin:     #C%2d                    #y+\n\r", odin ); 
         stc( buf, ch );
         sprintf( buf, "#y+                  #CCarbunkle:     #C%2d                    #y+\n\r", carb );
         stc( buf, ch );
         sprintf( buf, "#y+                       #cLuna:     #C%2d                    #y+\n\r", luna ); 
         stc( buf, ch );
         sprintf( buf, "#y+                       #oGaia:     #C%2d                    #y+\n\r", gaia );
         stc( buf, ch );
         stc( "#y+++++++++++++++++++++#wLANDS OF UTOPIA#y+++++++++++++++++++++\n\r", ch );

         stc( "\n\r#C(( #wTo see detailed stats, type #Rlands #o'#w<name of deity>#o' #C))#n\n\r", ch );
         return;
      }

   if ( !str_cmp( arg, "shiva" ) )
      option = 1;
      else if ( !str_cmp( arg, "ifrit" ) )
              option = 2;
      else if ( !str_cmp( arg, "siren" ) )
              option = 3;
      else if ( !str_cmp( arg, "diablos" ) )
              option = 4;
      else if ( !str_cmp( arg, "titan" ) )
              option = 5;
      else if ( !str_cmp( arg, "quezacotl" ) )
              option = 6;
      else if ( !str_cmp( arg, "odin" ) )
              option = 7;
      else if ( !str_cmp( arg, "carbunkle" ) )
              option = 8;
      else if ( !str_cmp( arg, "luna" ) )
              option = 9;
      else if ( !str_cmp( arg, "gaia" ) )
              option = 10;
              else {
                      do_lands( ch, "" );
                      return;
                   }

   sprintf( buf, " #w#uLands of %s#n\n\r\n\r", deity_table[option - 1] );
   stc( buf, ch );

   for ( pArea = area_first; pArea; pArea = pArea->next )
   {
      if ( pArea->land == option )
         {
            total++;
            sprintf( buf, " #w%d#C) #y%s#C.\n\r", total, pArea->name );
            stc( buf, ch );
         }
   }

   sprintf( buf, "\n\r #C(( #wTotal: #y%d #C))#n\n\r", total );
   stc( buf, ch );
}


void do_reclass(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( ch->pkill < 30 + ( ( ch->pcdata->clanrank - 1 ) * 10 ) )
      {
         stc("You aren't powerful enough as yet to withstand the deadly forces from this.\n\r",ch);
         return;
      }

   if ( ch->class == 0 )
      {
         stc("You need to be in a class warrior.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         stc("What class to you wish to become?\n\r",ch);
         return;
      }

   if ( !str_cmp(arg,"fallen angel") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 182 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 4;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( (pObjIndex = get_obj_index( 92 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 4;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_ANGEL;
         ch->class_2 = CLASS_DEMON;
         do_update_class_count( ch->class_2, TRUE );

         SET_BIT( ch->cmbt[7], SKL_GODPEACE );
         SET_BIT( ch->cmbt[5], SKL2_DLUCK );
         SET_BIT( ch->cmbt[7], SKL_DESANCTIFY );
         SET_BIT( ch->special, SPC_RECLASS );

         stc( "#wYou have lost the warmth of #yheaven#w... and #Rrevenge #wis on your mind.#n\n\r", ch );
         sprintf(buf,"#y%s #whas fallen grace from #CGod #wand now suffers the wrath of #eHell#w.#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp( arg, "daramon" ) )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 246 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         SET_BIT( ch->special, SPC_RECLASS );
         ch->class = CLASS_DARAMON;
         SET_BIT( ch->cmbt[7], SKL_KNOWLEDGE );
         stc( "Being a #yreserved#n follower of the #PBuddha#n, you wait patiently for the time to strike..\n\r", ch);

         sprintf(buf,"#P%s #wis now a sword keeper of the #CMighty #yBuddha#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"divine dragoon") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 95 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         SET_BIT( ch->special, SPC_RECLASS );
         SET_BIT( ch->cmbt[7], SKL_GUARD );
         SET_BIT( ch->cmbt[7], SKL_DODGE );
         ch->class = CLASS_DRAGON_KNIGHT;
         ch->pcdata->powers[DRAGOON_LEVEL] = 1;
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA );
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE );
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE );
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY );
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK );
         SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DIVINE );

         act( "#wYour body trembles with rage as you stagger forward onto your knees.#n", ch, NULL, NULL, TO_CHAR );
         act( "#w$n's body trembles with rage as $e staggers forward onto $s knees.#n", ch, NULL, NULL, TO_ROOM );
         act( "#wA soft grin appears on your face as your eyes #yblaze #wwith ferocious intensity!#n", ch, NULL, NULL, TO_CHAR );
         act( "#wA soft grin appears on $n's face as $s eyes #yblaze #wwith ferocious intensity!#n", ch, NULL, NULL, TO_ROOM );
         act( "#wYou snarl '#yArrrrrrghh..#w' as the world shakes violently beneaths your feet.#n", ch, NULL, NULL, TO_CHAR );
         act( "#w$n snarls '#yArrrrrrghh..#w' as the world shakes violently beneaths $s feet.#n", ch, NULL, NULL, TO_ROOM );
         act( "#wA MASSIVE ball of #yenergy #wsuddenly rise in front of you #yexploding #win a MaSs Of FlAmEs!!#n", ch, NULL, NULL, TO_CHAR );
         act( "#wA MASSIVE ball of #yenergy #wsuddenly rise in front of $n #yexploding #win a MaSs Of FlAmEs!!#n", ch, NULL, NULL, TO_ROOM );
         act( "#wYour eyes #wenlit #win divine flames as a pair of blinding wings sprouts from behind your back.#n", ch, NULL, NULL, TO_CHAR );
         act( "#w$n#w's eyes #wenlit #win divine flames as a pair of blinding wings sprouts from behind $s back.#n", ch, NULL, NULL, TO_ROOM );
         stc( "#wYou place your trust in the #CDivine #wDragon.#n\n\r", ch );

         sprintf(buf,"#P%s #whas placed their trust in the #CDivine #wDragon!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"lloths avatar") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_DROW;
         ch->pcdata->powers[DROW_CLASS] = DROW_AVATAR;
         ch->pcdata->stats[DROW_MAGIC] = 100;
         SET_BIT( ch->cmbt[7], SKL_DISCHARD );
         SET_BIT( ch->cmbt[7], SKL_FOCUS );
         SET_BIT( ch->cmbt[7], SKL_BALANCE );
         SET_BIT( ch->special, SPC_RECLASS );

         stc( "#wYou are now one of #eLloth's #wtrue followers...#n\n\r", ch );

         sprintf(buf,"#P%s #wis now a true follower of #eLloth#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"elite assassin") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_SHADOW;
         ch->clannum = 1;
         SET_BIT( ch->cmbt[5], SKL2_AMIND );
         SET_BIT( ch->special, SPC_RECLASS );

         stc( "#wYou are now the perfect #eassassin#w...#n\n\r", ch );

         sprintf(buf,"#P%s #wis now the perfect #eassassin#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"hybrid") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_VAMPIRE;
         ch->class_2 = CLASS_WEREWOLF;
         ch->class_3 = CLASS_DAYWALKER;
         ch->gnosis[GMAXIMUM] = ch->siltol = 100;
         SET_BIT( ch->special, SPC_RECLASS );

         do_update_class_count( ch->class_2, TRUE );

         stc( "#wYou now understand the #edreaded #wsecret of vampires and wolves... yourself.#n\n\r", ch );

         sprintf(buf,"#P%s #wis now a #ehybrid#w... #oGaia's #eworst fear!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"elder dragon") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_DRAGON;
         ch->clannum = 1;
         ch->max_hit += 3000000;
         ch->hit += 3000000;
         SET_BIT( ch->special, SPC_RECLASS );

         stc( "#wYou are now an Elder #PDragon#w... Start hording.#n\n\r", ch );

         sprintf(buf,"#P%s #wis now an Elder #PDragon#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"undead lord") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 370 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_UNDEAD_KNIGHT;
         ch->class_2 = CLASS_NECROMANCER;
         SET_BIT( ch->special, SPC_RECLASS );

         do_update_class_count( ch->class_2, TRUE );

         stc( "#wYour future has finally arrived... Salvation? or Destruction...#n\n\r", ch );

         sprintf(buf,"#P%s #wis now an #eUndead #PLord#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"elemental fury") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_SNOW_ELF;
         ch->class_2 = CLASS_PHOENIX;
         ch->class_3 = CLASS_DUERGAR;
         ch->class_4 = CLASS_WISP;
         SET_BIT( ch->special, SPC_RECLASS );

         do_update_class_count( ch->class_2, TRUE );
         do_update_class_count( ch->class_3, TRUE );
         do_update_class_count( ch->class_4, TRUE );

         stc( "#wYou have fused yourself with #Pall #wof the #Rel#yem#gen#Cts#w...#n\n\r", ch );

         sprintf(buf,"#P%s has fused themselves with #Pall #wof the #Rel#yem#gen#Cts#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"arch mage") || !str_cmp(arg,"archmage") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_WIZARD;
         SET_BIT( ch->special, SPC_RECLASS );
         ch->clannum = 6;
         SET_BIT( ch->cmbt[5], SKL2_MCONTROL );

         stc( "#wYou are now a master of #Pmagick#w...#n\n\r", ch );

         sprintf(buf,"#P%s #whas mastered the #CMystical #yArts#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"blade lord") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         SET_BIT( ch->special, SPC_RECLASS );

         ch->class = CLASS_MASTER;
         ch->clannum = 1;
         ch->cmbt[4] += 5;

         stc( "#wYou are now a master of the #PBlade#w...#n\n\r", ch );
         stc( "#wYou #yhave #gwon #Rfive #P#zfree#n #oskill#ebuy #cpoints#L!#y!#C!#n\n\r", ch );

         sprintf(buf,"#P%s #whas mastered the #yArts#w of the #eBlade#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"doppleganger") )
      {
         int count = 0;
         int option;

         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         SET_BIT( ch->special, SPC_RECLASS );
         SET_BIT( ch->special, SPC_DOPPLE );

         do
         {
            switch( count )
            {
               default:
               case 0: option = ch->class = number_range( 1, 43 ); break;
               case 1: option = ch->class_2 = number_range( 1, 43 ); break;
               case 2: option = ch->class_3 = number_range( 1, 43 ); break;
               case 3: option = ch->class_4 = number_range( 1, 43 ); break;
            }
 
            if ( option == 2 || option == 5 || option == 13 || option == 15 ||
                 option == 16 || option == 20 || option == 24 || option == 29 ||
                 option == 30 || option == 32 || option == 33 || option == 34 ||
                 option == 35 || option == 14 || option == 17 || option == 26
                 || option == 19 || option == 7 || option == 8 || option == 10
                 || option == 1 || option == 9 || option == 36 || option == 25
                 || option == 23 || option == 31 )
               continue;

            if ( count == 1 && option == ch->class )
               continue;

            if ( count == 2 && ( option == ch->class || option == ch->class_2 ) )
               continue;

            if ( count == 3 && ( option == ch->class || option == ch->class_2 || option == ch->class_3 ) )
               continue;

            count++;
         }
         while ( count < 4 );

         if ( IS_CLASS( ch, CLASS_ASSASSIN ) )
            SET_BIT( ch->special, ( number_range( 1, 2 ) == 1 ) ? SPC_KNIGHT : SPC_SOR );

         if ( IS_CLASS( ch, CLASS_WEREWOLF ) )
            ch->gnosis[GMAXIMUM] = ch->siltol = 100;

         if ( IS_CLASS( ch, CLASS_VAMPIRE ) )
            ch->beast = 100000;

         if ( IS_CLASS( ch, CLASS_HOBBIT ) )
            {
               if ( (pObjIndex = get_obj_index( 437 )) == NULL)
                  {
                     send_to_char("Missing object, check into it now.\n\r",ch);
                     return;
                  }

               obj = create_object(pObjIndex, 60);
               obj->level = 1;
               obj->questowner = str_dup(ch->pcdata->switchname);
               obj_to_char(obj, ch);
            }
            
         if ( IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) )
            {
               ch->pcdata->powers[DRAGOON_LEVEL] = 1;
               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA );
               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE );
               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE );
               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY );
               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK );
               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DIVINE );

               if ( (pObjIndex = get_obj_index( 95 )) == NULL)
                  {
                     send_to_char("Missing object, check into it now.\n\r",ch);
                     return;
                  }

               obj = create_object(pObjIndex, 60);
               obj->level = 5;
               obj->questowner = str_dup(ch->pcdata->switchname);
               obj_to_char(obj, ch);
            }

         do_update_class_count( ch->class_2, TRUE );
         do_update_class_count( ch->class_3, TRUE );
         do_update_class_count( ch->class_4, TRUE );

         stc( "#wYou are now #P#z4#n different things at the #wsame time#w...#n\n\r", ch );

         sprintf(buf,"#P%s #whas become something #R#zvery#n #emysterious#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"bad ass") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 122 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_DRAGOON;
         ch->class_2 = CLASS_FIEND;
         ch->class_3 = CLASS_SAIYAN;
         ch->class_4 = CLASS_SHADOW_WAR;
         SET_BIT( ch->special, SPC_RECLASS );
         do_update_class_count( ch->class_2, TRUE );
         do_update_class_count( ch->class_3, TRUE );
         do_update_class_count( ch->class_4, TRUE );

         stc( "#wYou are now #cTHE #eBAD #wASS.#n\n\r", ch );

         sprintf(buf,"#P%s #whas become a #Cmoth^&**& #ebad #Rass#w!!!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"elite commander") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 439 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_KURUDA;
         ch->class_2 = CLASS_ASSASSIN;
         SET_BIT( ch->special, SPC_KNIGHT );
         SET_BIT( ch->special, SPC_SOR );
         SET_BIT( ch->special, SPC_RECLASS );
         do_update_class_count( ch->class_2, TRUE );

         stc( "#wYou are now consider to be #PZero's #wequal.#n\n\r", ch );

         sprintf(buf,"#P%s #wis now one of the Directors of #wNe#ests #PCartel#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"heavenly knight") )
      {
         int count;

         if ( (pObjIndex = get_obj_index( 168 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         for ( count = 0; count < 16; count++ )
             ch->aura[count] = 10;

         for ( count = 20; count < 37; count++ )
             ch->aura[count] = 10;

         ch->class = CLASS_PALADIN;
         ch->class_2 = CLASS_DIVINE_KNIGHT;
         SET_BIT( ch->special, SPC_RECLASS );
         ch->clannum = 1;

         do_update_class_count( ch->class_2, TRUE );

         stc( "#wYou are now one of the High Priest's guardians.#n\n\r", ch );

         sprintf(buf,"#P%s #whas been proclaimed as the #yHigh #CPriest's #wguardian#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"grand master") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_MONK;
         ch->class_2 = CLASS_NINJA;
         SET_BIT( ch->special, SPC_RECLASS );
         do_update_class_count( ch->class_2, TRUE );

         SET_BIT( ch->cmbt[5], SKL2_FATTACK );
         SET_BIT( ch->cmbt[5], SKL2_FBLOW );
         SET_BIT( ch->cmbt[5], SKL2_SWITCH );
         SET_BIT( ch->cmbt[5], SKL2_SSTRIKE );
         SET_BIT( ch->cmbt[7], SKL_DODGE );
         SET_BIT( ch->cmbt[5], SKL2_SBLOW );

         stc( "#wYou are now a master of the #ofists #wand the #eshadows#w...#n\n\r", ch );

         sprintf(buf,"#P%s #whas mastered the #CMartial #yArts#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"garou") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_MONK;
         ch->class_2 = CLASS_WEREWOLF;
         ch->gnosis[GMAXIMUM] = ch->siltol = 100;

         SET_BIT( ch->special, SPC_RECLASS );
         do_update_class_count( ch->class_2, TRUE );

         SET_BIT( ch->cmbt[5], SKL2_SWITCH );
         SET_BIT( ch->cmbt[7], SKL_DODGE );

         stc( "#wYou calmly put on your #Rcap #was you search for the world for #Copponents#w.#n\n\r", ch );

         sprintf(buf,"#P%s #wis the legendary #owolf#w... #Rprepare #wyourself!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"shadow demon") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 92 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 4;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_DEMON;
         ch->class_2 = CLASS_FIEND;
         ch->class_3 = CLASS_SHADOW_WAR;

         SET_BIT( ch->special, SPC_RECLASS );
         do_update_class_count( ch->class_2, TRUE );
         do_update_class_count( ch->class_3, TRUE );

         SET_BIT( ch->cmbt[5], SKL2_DLUCK );
         SET_BIT( ch->cmbt[7], SKL_DESANCTIFY );

         stc( "#eYou #Rscream #ein agony as you are faced with the #w#zworst#n #edilemma... #RDemon #eor #yHuman#e?#n#n\n\r", ch );

         sprintf(buf,"#eWill #P%s #esave the #yhumans#e? or will they become the best #Rdemon #eever?#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"sword master") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_PALADIN;
         ch->class_3 = CLASS_SAMURAI;
         ch->class_2 = CLASS_ASSASSIN;
         SET_BIT( ch->special, SPC_KNIGHT );
         SET_BIT( ch->special, SPC_SOR );
         SET_BIT( ch->special, SPC_RECLASS );
         SET_BIT( ch->special, SPC_SWORD );
         do_update_class_count( ch->class_2, TRUE );
         do_update_class_count( ch->class_3, TRUE );

         stc( "#wYou are now consider to be a master of the #PSword.#n\n\r", ch );

         sprintf(buf,"#P%s #wis now one of a #Cmaster #wof the #PSword#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"dark magician") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_WIZARD;
         ch->class_3 = CLASS_NECROMANCER;
         SET_BIT( ch->cmbt[5], SKL2_MCONTROL );
         do_update_class_count( ch->class_2, TRUE );

         stc( "#wYou have #egiven up #wyour soul for more #Rpower#w....#n\n\r", ch );

         sprintf(buf,"#P%s #whas #egiven up #wtheir soul for more #Rpower#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"guardian angel") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_ANGEL;
         ch->class_2 = CLASS_DRAGON_KNIGHT;
         do_update_class_count( ch->class_2, TRUE );

         SET_BIT( ch->cmbt[7], SKL_GUARD );
         SET_BIT( ch->cmbt[7], SKL_DODGE );
         SET_BIT( ch->cmbt[7], SKL_GODPEACE );

         ch->pcdata->powers[DRAGOON_LEVEL] = 1;

         switch( number_range( 1, 5 ) )
         {
            default:
            case 1: SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_SEA ); break;
            case 2: SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_FIRE ); break;
            case 3: SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_JADE ); break;
            case 4: SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY ); break;
            case 5: SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK ); break;
         }

         stc( "#pYou are now the #wprotector #pof the #odragons#p.#n\n\r", ch );

         sprintf(buf,"#p%s is now the #wprotector #pof the #odragons#p!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"stalker") )
      {
         ch->fight_timer = 0;

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_NINJA;
         ch->class_2 = CLASS_VAMPIRE;
         do_update_class_count( ch->class_2, TRUE );

         SET_BIT( ch->cmbt[5], SKL2_FATTACK );
         SET_BIT( ch->cmbt[5], SKL2_FBLOW );
         ch->beast = 100000;
         SET_BIT( ch->immune, IMM_SUNLIGHT );
         SET_BIT( ch->cmbt[7], SKL_ILLUSION );

         stc( "#eYou are now a #ystalker #eof the #Cnight#e.#n\n\r", ch );

         sprintf(buf,"#P%s #eis now a #ystalker #eof the #Cnight#e!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else if ( !str_cmp(arg,"the one") )
      {
         ch->fight_timer = 0;

         if ( (pObjIndex = get_obj_index( 310 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->level = 5;
         obj->questowner = str_dup(ch->pcdata->switchname);
         obj_to_char(obj, ch);

         if ( !IS_IMMORTAL( ch ) )
            {
               ch->level = 1;
               do_detox( ch, "" );
               ch->level = 3;
            }
            else do_detox( ch, "" );

         ch->class = CLASS_MONK;
         ch->class_2 = CLASS_NINJA;
         ch->class_3 = CLASS_ASSASSIN;
         SET_BIT( ch->special, SPC_KNIGHT );
         SET_BIT( ch->special, SPC_RECLASS );
         do_update_class_count( ch->class_2, TRUE );
         do_update_class_count( ch->class_3, TRUE );

         free_string( ch->groove );
         ch->groove = str_dup( "A" );
         ch->pcdata->deity = 99;

         SET_BIT( ch->cmbt[5], SKL2_FATTACK );
         SET_BIT( ch->cmbt[5], SKL2_FBLOW );
         SET_BIT( ch->cmbt[5], SKL2_SWITCH );
         SET_BIT( ch->cmbt[5], SKL2_SSTRIKE );
         SET_BIT( ch->cmbt[7], SKL_DODGE );
         SET_BIT( ch->cmbt[5], SKL2_SBLOW );

         stc( "#wYou are now the #Pone.#n\n\r", ch );

         sprintf(buf,"#P%s #whas been proclaimed as the #Cone#w!#n", ch->pcdata->switchname );
         do_info( ch, buf );
      }
      else {
              stc("That special class isn't in. Please check #wHELP CLASSES#n.\n\r",ch);
              return;
           }

   remove_member( ch );
   add_member( ch );

   ch->pcdata->clanrank++;
   ch->max_hit += 100000;
   ch->max_mana += 50000;
   ch->max_move += 50000;
   do_update_class_count( ch->class, TRUE );
   save_char_obj( ch );
}

void do_autoclass(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int count;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   if ( IS_NPC(ch) )
      return;

   if ( ch->class != 0 )
      {
         stc("You are already in a class.\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         stc("What class to you wish to become?\n\r",ch);
         return;
      }

   if ( !str_cmp(arg,"paladin") )
      {
         ch->class = CLASS_PALADIN;
         send_to_char( "Venture out Knight and protect all that is #wgood#n.\n\r", ch);
      }
      else if ( !str_cmp(arg,"ninja") )
              {
                 ch->class = CLASS_NINJA;
                 SET_BIT( ch->cmbt[5], SKL2_FATTACK );
                 SET_BIT( ch->cmbt[5], SKL2_SWITCH );
                 send_to_char( "You have chosen a life in the #0shadows#n, kill at will.\n\r", ch);
              }
      else if ( !str_cmp(arg,"dragon") )
              {
                 ch->class = CLASS_DRAGON;
                 send_to_char( "May you live to an grand old age fellow #pDragon#n.\n\r", ch);
              }
      else if ( !str_cmp(arg,"dragoon") )
              {
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
                               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_HOLY );
                               stc( "#wYou place your trust in the Silver Dragon.#n\n\r", ch );
                            }
                    else if ( !str_cmp("dark",arg) )
                            {
                               SET_BIT( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_DARK );
                               ch->alignment = -1000;
                               stc( "#eYou place your trust in the Dark Dragon.#n\n\r", ch );
                            }
                            else {
                                    stc("#PSyntax - #oAutoclass #Cdragoon <#Lsea#C/#Rredeyed#C/#gjade#C/#wholy#C/#edark#C>#n\n\r", ch );
                                    return;
                                 }

                 SET_BIT( ch->cmbt[7], SKL_GUARD );
                 SET_BIT( ch->cmbt[7], SKL_DODGE );
                 ch->class = CLASS_DRAGON_KNIGHT;
                 ch->pcdata->powers[DRAGOON_LEVEL] = 1;
              }
      else if ( !str_cmp(arg,"fiend") )
              {
                 ch->class = CLASS_FIEND;
                 send_to_char( "Will you fight for the #chumans#n? Or side with your #epurer #Rbrothers#n?\n\r", ch);
              }
      else if ( !str_cmp(arg,"commander") )
              {
                 ch->class = CLASS_KURUDA;
                 send_to_char( "Serve #wNe#ests#n and your #Psquadron#n well Commander.\n\r", ch);
              }
      else if ( !str_cmp(arg,"wizard") )
              {
                 ch->class = CLASS_WIZARD;
                 send_to_char( "May you learn the #ytrue#n magicks magi.\n\r", ch);
                 SET_BIT( ch->cmbt[5], SKL2_MCONTROL );
              }
      else if ( !str_cmp(arg,"werewolf") )
              {
                 ch->class = CLASS_WEREWOLF;
                 ch->gnosis[GMAXIMUM] = ch->siltol = 100;
                 SET_BIT( ch->cmbt[7], SKL_GUARD );
                 send_to_char( "You have chosen the path of the #0Garou#n, may gaia guide you.\n\r", ch);
              }
      else if ( !str_cmp(arg,"vampire") )
              {
                 ch->class = CLASS_VAMPIRE;
                 ch->beast = 100000;
                 SET_BIT( ch->immune, IMM_SUNLIGHT );
                 SET_BIT( ch->cmbt[7], SKL_ILLUSION );
                 send_to_char( "Fear the #ySun#n nosferatu, god's curse live in you.\n\r", ch);
              }
      else if ( !str_cmp(arg,"drow") )
              {
                 ch->pcdata->stats[DROW_MAGIC] = 100;
                 ch->class = CLASS_DROW;
                 SET_BIT( ch->cmbt[7], SKL_DISCHARD );
                 send_to_char( "Choose your profession, and #PLloth#n will guide you.\n\r", ch);
              }
      else if ( !str_cmp(arg,"highwind") )
              {
                 ch->class = CLASS_DRAGOON;
                 SET_BIT( ch->cmbt[7], SKL_GUARD );
                 send_to_char( "Sit back... #ylight#n a #Ccigarette#n... and get ready to kick some %$^(* #oass#n!.\n\r", ch);
              }
      else if ( !str_cmp(arg,"necromancer") )
              {
                 ch->class = CLASS_NECROMANCER;
                 send_to_char( "You cackle gleefully amongst the #ecorpses#n you call... home.\n\r", ch);
              }
      else if ( !str_cmp(arg,"undead knight") || !str_cmp(arg,"undeadknight") )
              {
                 ch->class = CLASS_UNDEAD_KNIGHT;
                 send_to_char( "You are now forever #edoomed#n in your quest for salvation.\n\r", ch);
              }
      else if ( !str_cmp( arg, "sorceror" ) )
              {
                 ch->class = CLASS_ASSASSIN;
                 SET_BIT( ch->special, SPC_SOR );
                 SET_BIT( ch->cmbt[7], SKL_ILLUSION );
                 SET_BIT( ch->cmbt[7], SKL_ENCASE );
                 send_to_char( "Your eyes #Rblazes#n with #Rferocious #yintensity#n as you cluth your sword...\n\r", ch);
              }
      else if ( !str_cmp(arg,"shadow warrior") )
              {
                 ch->class = CLASS_SHADOW_WAR;
                 SET_BIT( ch->cmbt[7], SKL_ILLUSION );
                 SET_BIT( ch->cmbt[7], SKL_GRASP );
                 SET_BIT( ch->cmbt[5], SKL2_SSWAP );
                 send_to_char( "The #eShadow Goddess#n gives you life... Now Die for her!\n\r", ch);
              }
      else if ( !str_cmp(arg,"monk") )
              {
                 ch->class = CLASS_MONK;
                 send_to_char( "Your faith in the Light will guide you, destroy #7EVIL#n.\n\r", ch);
                 SET_BIT( ch->cmbt[5], SKL2_SWITCH );
                 SET_BIT( ch->cmbt[5], SKL2_SSTRIKE );
                 SET_BIT( ch->cmbt[7], SKL_DODGE );
                 SET_BIT( ch->cmbt[5], SKL2_SBLOW );
              }
      else if ( !str_cmp(arg,"saiyan") || !str_cmp(arg,"saiya-jin") )
              {
                 ch->class = CLASS_SAIYAN;
                 send_to_char( "Go now warrior and #RDESTROY#n everything in your path.\n\r", ch);
              } 
      else if ( !str_cmp(arg,"samurai") )
              {
                 ch->class = CLASS_SAMURAI;
                 SET_BIT( ch->cmbt[7], SKL_GUARD );
                 send_to_char( "Protect the Emperor with your very #wlife#n.\n\r", ch);
              }
      else if ( !str_cmp(arg,"assassin") )
              {
                 ch->class = CLASS_SHADOW;
                 SET_BIT( ch->cmbt[5], SKL2_AMIND );
                 SET_BIT( ch->cmbt[7], SKL_ILLUSION );
                 send_to_char( "You pledge your alligence to #eGrahf#n as he bestows you the #RPower#n.\n\r", ch);
              }
      else if ( !str_cmp( arg, "snow elf" ) )
              {
                 ch->ice_bonus += 35;

                 ch->class = CLASS_SNOW_ELF;
                 ch->pcdata->deity = DEI_SHIVA;
                 SET_BIT( ch->cmbt[5], SKL2_SSIGHT );
                 send_to_char( "Hiding within the #wfrosty #Cwastelands#n, you wait patiently for your time to #ekill#n...\n\r", ch);
              }
      else if ( !str_cmp ( arg, "wisp" ) )
	        {
                 ch->bolt_bonus += 35;

		     ch->class = CLASS_WISP;
                 ch->pcdata->deity = DEI_BIRD;

                 SET_BIT( ch->cmbt[7], SKL_ILLUSION );
	           send_to_char ( "#y#zBursting#n into #Pmany#n #Rcolours#n of #ylight#n you are granted #yWisp#n powers!\n\r", ch);
		   
                 sprintf(buf,"#w%s the #yWisp#n", ch->pcdata->switchname );
                 free_string( ch->morph );
                 ch->morph = str_dup(buf);
                 SET_BIT(ch->affected_by, AFF_POLYMORPH);
              }
      else if ( !str_cmp ( arg, "phoenix" ) )
              {
                 ch->fire_bonus += 35;

	           ch->class = CLASS_PHOENIX;
                 ch->pcdata->deity = DEI_IFRIT;

                 SET_BIT( ch->cmbt[7], SKL_CRITICAL );
                 SET_BIT( ch->cmbt[7], SKL_RETORT );

                 send_to_char ( "#y#zBursting#n into #Rflames#n you are granted #yPhoenix#n powers!\n\r", ch);
		   
                 sprintf(buf,"#R%s #ethe #Rf#ri#ye#Rr#ry #RP#rh#eo#ye#en#Ri#rx#n", ch->pcdata->switchname );
                 free_string( ch->morph );
                 ch->morph = str_dup(buf);
                 SET_BIT(ch->affected_by, AFF_POLYMORPH);
              }
      else if ( !str_cmp ( arg, "hobbit" ) )
              {
                 OBJ_INDEX_DATA *pObjIndex;
                 OBJ_DATA *obj;

                 if ( (pObjIndex = get_obj_index( 437 )) == NULL)
                    {
                       send_to_char("Missing object, check into it now.\n\r",ch);
                       return;
                    }

                 obj = create_object(pObjIndex, 60);
                 obj->questowner = str_dup(ch->pcdata->switchname);
                 obj_to_char(obj, ch);

                 ch->class = CLASS_HOBBIT;
                 ch->pcdata->deity = DEI_SIREN;
                 ch->holy_bonus += 15;
                 ch->dark_bonus += 15;

                 SET_BIT( ch->cmbt[5], SKL2_DLUCK );
                 SET_BIT( ch->cmbt[7], SKL_BLOCK );

                 send_to_char ( "Entrusting #wyou#n with the #ering#n, the deities hope you make the #wrig#eht cho#wice#n.\n\r", ch);
              }
      else if ( !str_cmp ( arg, "duergar" ) )
              {
                 ch->earth_bonus += 35;
                 ch->class = CLASS_DUERGAR;
                 ch->pcdata->deity = DEI_TITAN;

                 SET_BIT( ch->cmbt[7], SKL_BLOCK );
                 SET_BIT( ch->cmbt[7], SKL_PRACTICE );
                 SET_BIT( ch->cmbt[7], SKL_KNOWLEDGE );
                 send_to_char ( "Growling viciously you clutch your #0axe#n ready to stand tall and #gshake the heavens..#n\n\r", ch);
              }
      else if ( !str_cmp( arg, "dark knight" ) )
              {
                 ch->class = CLASS_ASSASSIN;
                 SET_BIT( ch->special, SPC_KNIGHT );
                 SET_BIT( ch->cmbt[7], SKL_GUARD );
                 send_to_char( "Your eyes #Rblazes#n with #Rferocious #yintensity#n as you cluth your sword...\n\r", ch);
              }
      else if ( !str_cmp(arg,"angel") )
              {
                 if ( (pObjIndex = get_obj_index( 182 )) == NULL)
                    {
                       send_to_char("Missing object, check into it now.\n\r",ch);
                       return;
                    }

                 obj = create_object(pObjIndex, 60);
                 obj->level = 4;
                 obj->questowner = str_dup(ch->pcdata->switchname);
                 obj_to_char(obj, ch);

                 ch->class = CLASS_ANGEL;
                 ch->alignment = 1000;
                 SET_BIT( ch->cmbt[7], SKL_GODPEACE );
 
                 stc( "#wYou are now an #yangel#w, #CGod's #wpurest creature.#n\n\r", ch );
              }
      else if ( !str_cmp(arg,"demon") )
              {
                 if ( (pObjIndex = get_obj_index( 92 )) == NULL)
                    {
                       send_to_char("Missing object, check into it now.\n\r",ch);
                       return;
                    }

                 obj = create_object(pObjIndex, 60);
                 obj->level = 4;
                 obj->questowner = str_dup(ch->pcdata->switchname);
                 obj_to_char(obj, ch);

                 ch->class = CLASS_DEMON;
                 ch->alignment = -1000;
                 SET_BIT( ch->cmbt[5], SKL2_DLUCK );
                 SET_BIT( ch->cmbt[7], SKL_DESANCTIFY );
 
                 stc( "#eYou are now a #Rdemon#e... the #PApocalypse #eshall arrive...#n\n\r", ch );
              }
      else if ( !str_cmp( arg, "daywalker" ) )
		  { 
		     ch->class = CLASS_DAYWALKER;
                 SET_BIT( ch->cmbt[7], SKL_PRACTICE );
                 SET_BIT( ch->cmbt[7], SKL_KNOWLEDGE );
                 stc( "Donning your cool shades you are ready to fight all forms of evil, especially the #Rvampire#n\n\r", ch);
		  }
              else {
                      stc("That class isn't in yet. Please choose another one.\n\r",ch);
                      return;
                   }

   remove_member( ch );
   add_member( ch );

   do_update_class_count( ch->class, TRUE );
   save_char_obj( ch );
}

void do_classself(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int count;

   if ( IS_NPC(ch) )
      return;

   argument = one_argument( argument, arg );
   argument = one_argument( argument, arg2 );

   if ( arg[0] == '\0' )
      {
          stc("Which person do you want to class?\n\r",ch);
          return;
      }

   if ( ( victim = get_char_world(ch, arg) ) == NULL ) 
      {
          send_to_char("They are not here.\n\r", ch );
          return;
      }

   if ( IS_NPC(victim) )
      {
	    send_to_char("Not on NPCs.\n\r", ch );
	    return;
      }

   if ( arg2[0] == '\0' )
      {
         stc("What class to you wish to make them?\n\r",ch);
         return;
      }

   if ( !str_cmp(arg2,"none") )
      {
         char_from_room( victim );
         char_to_room( victim, get_room_index( 800 ) );
         do_detox( victim, "" );
         return;
      }

   if ( victim->class != 0 )
      {
         stc("That person is already in a class.\n\r",ch);
         return;
      }

   if ( !str_cmp( arg2, "sorceror" ) )
      {
         OBJ_INDEX_DATA *pObjIndex;
         OBJ_DATA *obj;

         if ( (pObjIndex = get_obj_index( 439 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->questowner = str_dup(victim->pcdata->switchname);
         obj_to_char(obj, victim);

         victim->class = CLASS_ASSASSIN;
         SET_BIT( victim->special, SPC_SOR );
         send_to_char( "Your eyes #Rblazes#n with #Rferocious #yintensity#n as you cluth your sword...\n\r", victim);

         sprintf( buf, "#gWith #eLord Zero's #gapproval, #RKrizalid #ggrants #e%s #gthe honour of joining the #CNests Cartel#g!#n", victim->pcdata->switchname );
         do_info( victim, buf );
      }
      else if ( !str_cmp( arg2, "dark knight" ) )
      {
         OBJ_INDEX_DATA *pObjIndex;
         OBJ_DATA *obj;

         if ( (pObjIndex = get_obj_index( 439 )) == NULL)
            {
               send_to_char("Missing object, check into it now.\n\r",ch);
               return;
            }

         obj = create_object(pObjIndex, 60);
         obj->questowner = str_dup(victim->pcdata->switchname);
         obj_to_char(obj, victim);

         victim->class = CLASS_ASSASSIN;
         SET_BIT( victim->special, SPC_KNIGHT );
         send_to_char( "Your eyes #Rblazes#n with #Rferocious #yintensity#n as you cluth your sword...\n\r", victim);

         sprintf( buf, "#gWith #eLord Zero's #gapproval, #RKrizalid #ggrants #e%s #gthe honour of joining the #CNests Cartel#g!#n", victim->pcdata->switchname );
         do_info( victim, buf );
      }
      else if ( !str_cmp( arg2, "kuruda" ) )
      {
         victim->class = CLASS_ASSASSIN;
         SET_BIT( victim->special, SPC_MONK );
         send_to_char( "Your eyes #Rblazes#n with #Rferocious #yintensity#n as you gaze at your hands...\n\r", victim);

         sprintf( buf, "#gWith #eLord Zero's #gapproval, #RKrizalid #ggrants #e%s #gthe honour of joining the #CNests Cartel#g!#n", victim->pcdata->switchname );
         do_info( victim, buf );
      }
	  else if ( !str_cmp ( arg2, "daywalker" ) )
	  {
		   victim->class = CLASS_DAYWALKER;

		   send_to_char ( "Donning your cool shades and your ever black leather clothes you become Daywalker\n\r", victim);
	  }
	  else if ( !str_cmp ( arg2, "daramon" ) )
	  {
		   victim->class = CLASS_DARAMON;

		   send_to_char ( "Patiently you examine the best time to strike.\n\r", victim);
	  }
        else {
                stc("Tell them to use #Pa#wu#Pt#wo#Pc#wl#Pa#ws#Ps#n for this.\n\r",ch);
                return;
             }

   remove_member( victim );
   add_member( victim );

   do_update_class_count( victim->class, TRUE );
   save_char_obj( victim );
}

void do_travel( CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];
   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
	return;

   if ( arg[0] == '\0' )
      {
         send_to_char( "Travel to where?\n\r", ch );
         return;
      }

   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }
 
   if ( ch == victim )
      {
         send_to_char("But you're already at yourself!\n\r",ch);
         return;
      }

   if ( victim->in_room->vnum == ch->in_room->vnum )
      {
         send_to_char("But you're already there!\n\r",ch);
         return;
      }

   if ( !( IS_CLASS( ch, CLASS_ANGEL ) && ch->pkill > 7 && ch->binding == victim
        && ch->in_room->vnum > 100 ) )
      {
         if ( in_kingdom( ch, victim, victim->in_room ) )
            return;

         if ( ch->fight_timer > 0 )
            {
               stc("Not with a fight timer!\n\r",ch);
               return;
            }
      }

   char_from_room(ch);
   char_to_room(ch,victim->in_room);

   send_to_char("You step through a shimmering portal towards your target!\n\r",ch);
   act("Suddenly, blinding light fills the room as $n appear before you!",ch,NULL,NULL,TO_ROOM);
   do_look(ch, "auto");
   check_traps( ch );

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );
}

void do_seeking(CHAR_DATA *ch, char *argument) 
{
   if ( IS_NPC(ch) )
      return;

   if ( IS_SET(ch->newbits, NEW_CLANNABLE) )
      {
         REMOVE_BIT(ch->newbits, NEW_CLANNABLE);
         send_to_char("You are no longer seeking to join a kingdom.\n\r", ch );
         return;
      }

   SET_BIT(ch->newbits, NEW_CLANNABLE);
   send_to_char("You are now seeking to join a kingdom.\n\r", ch );
}

void do_choose(CHAR_DATA *ch, char *argument) 
{
   char arg[MAX_STRING_LENGTH];
   int count;

   argument = one_argument(argument, arg);

   if ( IS_NPC( ch ) )
      return;

   if ( !str_cmp( "c groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "C" );

         stc( "#wYou are now using the #CC#wapcom fighting groove system.#n\n\r", ch );

         return;
      }
      else if ( !str_cmp( "s groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "S" );
         
         stc( "#wYou are now using the #CS#wnk fighting groove system.#n\n\r", ch );

         return;
      }
      else if ( !str_cmp( "k groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "K" );

         stc( "#wYou are now using the #CK#willer rage fighting groove system.#n\n\r", ch );

         return;
      }
      else if ( !str_cmp( "n groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "N" );

         stc( "#wYou are now using the #CN#wano power meter fighting groove system.#n\n\r", ch );

         return;
      }
      else if ( !str_cmp( "a groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "A" );

         stc( "#wYou are now using the #CA#wnymove advanced combo fighting groove system.#n\n\r", ch );

         return;
      }
      else if ( !str_cmp( "m groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         if ( !IS_SET( ch->pcdata->hidden_skill, 64 ) )
            {
               stc( "Huh?\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "M" );

         stc( "#wYou are now using the #CM#waster groove system.#n\n\r", ch );

         return;
      }
      else if ( !str_cmp( "p groove", arg ) )
      {
         if ( str_cmp( "" , ch->groove ) )
            {
               stc( "You had already chosen a groove. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         free_string( ch->groove );
         ch->groove = str_dup( "P" );

         stc( "#wYou are now using the advanced #CP#narrying fighting groove system.#n\n\r", ch );

         return;
      }
      else if ( ch->pcdata->deity == 99 )
      {
         stc( "You are the one... You don't need any protection from deities.\n\r", ch );
         return;
      }
      else if ( !str_cmp( "siren", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->dark_prof -= 25;
         ch->holy_prof += 25;
         ch->holy_bonus += 10;
         SET_BIT( ch->cmbt[7], SKL_BLOCK );

         ch->pcdata->deity = DEI_SIREN;
         stc( "#wYou place your trust in the holy spirit of Siren#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "diablos", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->dark_prof += 25;
         ch->holy_prof -= 25;
         ch->dark_bonus += 10;

         SET_BIT( ch->cmbt[5], SKL2_DLUCK );
         SET_BIT( ch->cmbt[5], SKL2_FBLOW );

         ch->pcdata->deity = DEI_DIABLOS;
         stc( "#eYou place your trust in the evil spirit of Diablos#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "titan", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->earth_prof += 25;
         ch->bolt_prof -= 25;
         ch->earth_bonus += 10;

         ch->pcdata->deity = DEI_TITAN;
         stc( "#gYou place your trust in the powerful spirit of Titan#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "luna", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->lunar_prof += 25;
         ch->natural_prof -= 25;
         ch->lunar_bonus += 10;

         ch->pcdata->deity = DEI_LUNA;
         stc( "#cYou place your trust in the mysterious spirit of Luna#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "gaia", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->lunar_prof -= 25;
         ch->natural_prof += 25;
         ch->natural_bonus += 10;

         SET_BIT( ch->cmbt[7], SKL_POWERB );
         ch->pcdata->deity = DEI_GAIA;
         stc( "#pYou place your trust in the ancient spirit of Gaia#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "quezacotl", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->earth_prof -= 25;
         ch->bolt_prof += 25;
         ch->bolt_bonus += 10;

         ch->pcdata->deity = DEI_BIRD;
         stc( "#yYou place your trust in the shocking spirit of Quezacotl#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "ifrit", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->ice_prof -= 25;
         ch->fire_prof += 25;
         ch->fire_bonus += 10;

         ch->pcdata->deity = DEI_IFRIT;
         stc( "#RYou place your trust in the fiery spirit of Ifrit#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "shiva", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->ice_prof += 25;
         ch->fire_prof -= 25;
         ch->ice_bonus += 10;

         ch->pcdata->deity = DEI_SHIVA;
         stc( "#LYou place your trust in the chilling spirit of Shiva#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "odin", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->magical_prof -= 20;
         ch->physical_prof += 20;

         ch->pcdata->deity = DEI_ODIN;
         stc( "#oYou place your trust in the destructive spirit of Odin#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "carburkle", arg ) )
      {
         if ( ch->pcdata->deity != 0 )
            {
               stc( "You had already chosen a deity. Use Choose #wnone#n first if you want to pick another one.\n\r", ch );
               return;
            }

         ch->magical_prof += 20;
         ch->physical_prof -= 20;

         ch->pcdata->deity = DEI_CARB;
         stc( "#CYou place your trust in the kind spirit of Carburkle#n.\n\r", ch );

         return;
      }
      else if ( !str_cmp( "none", arg ) )
      {
         stc( "#PYou have to detox to re-choose your deity and/or groove.\n\r", ch );
         return;
      }

   stc( "#PThat word isn't available. Please make sure you didn't make a typo#n.\n\r", ch );
}

void do_superheal( CHAR_DATA *ch, char *argument ) 
{
   int cost = 300000;

   if ( IS_NPC(ch) ) return;

   if ( !IS_SET( ch->cmbt[7], SKL_HEAL ) )
      {
         send_to_char("Huh?\n\r", ch );
         return;
      }

   if ( IS_CLASS( ch, CLASS_NINJA ) || IS_CLASS( ch, CLASS_WISP ) )
      cost = 150000;

   if ( !IS_NPC( ch ) && IS_SET( ch->cmbt[5], SKL2_HSOUL ) )
      {
         if ( IS_CLASS( ch, CLASS_NINJA ) || IS_CLASS( ch, CLASS_WISP ) )
            cost += 50000;
      }

   hit_gain( ch, cost );
   mana_gain( ch, cost );
   move_gain( ch, cost );

   send_to_char("You silently tend your wounds.\n\r",ch);
   act("$n silently tends to $s wounds.",ch,NULL,NULL,TO_ROOM);
   WAIT_STATE( ch, 12 );
}

void do_skillbuy( CHAR_DATA *ch, char *argument ) 
{
   char arg[MAX_STRING_LENGTH];
   int choice, skillset = 7;

   argument = one_argument(argument, arg);

   if ( IS_NPC( ch ) )
      return;

   if ( arg[0] == '\0' )
      {
         stc("                    #w-#o+#w-#CSkills of #RPower#w-#o+#w-\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_KAKUSU ) )
            stc("  #oKakusu                  ", ch );
            else stc("  #eKakusu                  ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_EYES ) )
            stc("#oEyes of the Heavens     ", ch );
            else stc("#eEyes of the Heavens     ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_HEAL ) )
            stc("#oSuper Heal\n\r\n\r", ch );
            else stc("#eSuper Heal\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_BLOCK ) )
            stc("  #oAbsorb                  ", ch );
            else stc("  #eAbsorb                  ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_HITRATE ) )
            stc("#oHitrate                 ", ch );
            else stc("#eHitrate                 ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_DODGE ) )
            stc("#oDodge\n\r\n\r", ch );
            else stc("#eDodge\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_ACCELERATE ) )
            stc("  #oAccelerate              ", ch );
            else stc("  #eAccelerate              ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_TLOCK ) )
            stc("#oTimer Lock              ", ch );
            else stc("#eTimer Lock              ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_HUNT ) )
            stc("#oHunt\n\r\n\r", ch );
            else stc("#eHunt\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_STEALTH ) )
            stc("  #oStealth                 ", ch );
            else stc("  #eStealth                 ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_ENCASE ) )
            stc("#oEncase                  ", ch );
            else stc("#eEncase                  ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_ASHIELD ) )
            stc("#oAura Shield\n\r\n\r", ch );
            else stc("#eAura Shield\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_PRACTICE ) )
            stc("  #oPractice                ", ch );
            else stc("  #ePractice                ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_KNOWLEDGE ) )
            stc("#oKnowledge               ", ch );
            else stc("#eKnowledge               ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_PLANAR ) )
            stc("#oPlanar Caller\n\r\n\r", ch );
            else stc("#ePlanar Caller\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_GRASP ) )
            stc("  #oShadow Grasp            ", ch );
            else stc("  #eShadow Grasp            ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_GODPEACE ) )
            stc("#oGodpeace                ", ch );
            else stc("#eGodpeace                ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_DISCHARD ) )
            stc("#oDischard\n\r\n\r", ch );
            else stc("#eDischard\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_CRITICAL ) )
            stc("  #oCritical                ", ch );
            else stc("  #eCritical                ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_ANTICIPATE ) )
            stc("#oAnticipate              ", ch );
            else stc("#eAnticipate              ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_RETORT ) )
            stc("#oRetort\n\r\n\r", ch );
            else stc("#eRetort\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_SWERVE ) )
            stc("  #oSwerve                  ", ch );
            else stc("  #eSwerve                  ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_POWERB ) )
            stc("#oPower Blow              ", ch );
            else stc("#ePower Blow              ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_STUNA ) )
            stc("#oShock Absorber\n\r\n\r", ch );
            else stc("#eShock Absorber\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_MAJESTY ) )
            stc("  #oMajesty                 ", ch );
            else stc("  #eMajesty                 ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_DESANCTIFY ) )
            stc("#oDesanctify              ", ch );
            else stc("#eDesanctify              ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_FOCUS ) )
            stc("#oFocus\n\r\n\r", ch );
            else stc("#eFocus\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_BALANCE ) )
            stc("  #oBalance                 ", ch );
            else stc("  #eBalance                 ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_ILLUSION ) )
            stc("#oIllusion Blur           ", ch );
            else stc("#eIllusion Blur           ", ch );

         if ( IS_SET( ch->cmbt[7], SKL_GUARD ) )
            stc("#oWeapon Guard\n\r\n\r", ch );
            else stc("#eWeapon Guard\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SCORCHING ) )
            stc("  #oScorching               ", ch );
            else stc("  #eScorching               ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_MAINTENANCE ) )
            stc("#oMaintenance             ", ch );
            else stc("#eMaintenance             ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_DLUCK ) )
            stc("#oDevil Luck\n\r\n\r", ch );
            else stc("#eDevil Luck\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_AMIND ) )
            stc("  #oAssassin Mind           ", ch );
            else stc("  #eAssassin Mind           ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_ANTIAURA ) )
            stc("#oAnti Aura               ", ch );
            else stc("#eAnti Aura               ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_REMINISCE ) )
            stc("#oReminisce\n\r\n\r", ch );
            else stc("#eReminisce\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_THROW ) )
            stc("  #oPelt                    ", ch );
            else stc("  #ePelt                    ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_TRIP ) )
            stc("#oTrip                    ", ch );
            else stc("#eTrip                    ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_MIME ) )
            stc("#oMime\n\r\n\r", ch );
            else stc("#eMime\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SSIGHT ) )
            stc("  #oSniper Sight            ", ch );
            else stc("  #eSniper Sight            ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_STEAL ) )
            stc("#oSteal                   ", ch );
            else stc("#eSteal                   ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_PRESENCE ) )
            stc("#oPresence\n\r\n\r", ch );
            else stc("#ePresence\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SSWAP ) )
            stc("  #oShadow Swap             ", ch );
            else stc("  #eShadow Swap             ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_MCONTROL ) )
            stc("#oMind Control            ", ch );
            else stc("#eMind Control            ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_HSOUL ) )
            stc("#oHelping Soul\n\r\n\r", ch );
            else stc("#eHelping Soul\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_LTOUCH ) )
            stc("  #oLeeching Touch          ", ch );
            else stc("  #eLeeching Touch          ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_STBLOW ) )
            stc("#oStunning Blow           ", ch );
            else stc("#eStunning Blow           ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_ROLL ) )
            stc("#oRoll\n\r\n\r", ch );
            else stc("#eRoll\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SPINCH ) )
            stc("  #oSubtle Pinch            ", ch );
            else stc("  #eSubtle Pinch            ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_RBODY ) )
            stc("#oRigid Body              ", ch );
            else stc("#eRigid Body              ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SFIST ) )
            stc("#oSerpent Fist\n\r\n\r", ch );
            else stc("#eSerpent Fist\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_IBODY ) )
            stc("  #oIron Body               ", ch );
            else stc("  #eIron Body               ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_FATTACK ) )
            stc("#oFirst Attack            ", ch );
            else stc("#eFirst Attack            ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_FBLOW ) )
            stc("#oFinal Blow\n\r\n\r", ch );
            else stc("#eFinal Blow\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_STAGGER ) )
            stc("  #oStagger                 ", ch );
            else stc("  #eStagger                 ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SWITCH ) )
            stc("#oSwitch                  ", ch );
            else stc("#eSwitch                  ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_DISARM ) )
            stc("#oDisarm\n\r\n\r", ch );
            else stc("#eDisarm\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_GRIP ) )
            stc("  #oGrip                    ", ch );
            else stc("  #eGrip                    ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SSTRIKE ) )
            stc("#oSnake Strike            ", ch );
            else stc("#eSnake Strike            ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_SBLOW ) )
            stc("#oShining Blow\n\r\n\r", ch );
            else stc("#eShining Blow\n\r\n\r", ch );

         if ( IS_SET( ch->cmbt[7], SKL_FBITE ) )
            stc("  #oFrost Bite              ", ch );
            else stc("  #eFrost Bite              ", ch );

         if ( IS_SET( ch->cmbt[5], SKL2_THEART ) )
            stc("#oThawing Heart           ", ch );
            else stc("#eThawing Heart           ", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 1 ) )
            stc("#wAir Blade\n\r\n\r", ch );
            else stc("#pAir Blade\n\r\n\r", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 2 ) )
            stc("  #wFangBreaker Blade       ", ch );
            else stc("  #pFangBreaker Blade       ", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 4 ) )
            stc("#wWhirlWind Crusher       ", ch );
            else stc("#pWhirlWind Crusher       ", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 8 ) )
            stc("#wBreaker\n\r\n\r", ch );
            else stc("#pBreaker\n\r\n\r", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 16 ) )
            stc("  #wDangling Effect         ", ch );
            else stc("  #pDangling Effect         ", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 32 ) )
            stc("#wJustice Star Fist       ", ch );
            else stc("#pJustice Star Fist       ", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 64 ) )
            stc("#wM Groove\n\r\n\r", ch );
            else stc("#pM Groove\n\r\n\r", ch );

         if ( IS_SET( ch->pcdata->hidden_skill, 128 ) )
            stc("  #wDivine Eradication      \n\r", ch );
            else stc("  #pDivine Eradication      \n\r", ch );

         stc("                    #w-#o+#w-#CSkills of #RPower#w-#o+#w-\n\r\n\r", ch );

         send_to_char( "What skill do you wish to buy?\n\r", ch );
         return;
      }

   if ( !str_cmp( "kakusu", arg ) )
      choice = SKL_KAKUSU;
      else if ( !str_cmp( "eyes of the heavens", arg ) )
              choice = SKL_EYES;
      else if ( !str_cmp( "hitrate", arg ) )
              choice = SKL_HITRATE;
      else if ( !str_cmp( "dodge", arg ) )
              choice = SKL_DODGE;
      else if ( !str_cmp( "super heal", arg ) )
              choice = SKL_HEAL;
      else if ( !str_cmp( "absorb", arg ) )
              choice = SKL_BLOCK;
      else if ( !str_cmp( "hunt", arg ) )
              choice = SKL_HUNT;
      else if ( !str_cmp( "stealth", arg ) )
              choice = SKL_STEALTH;
      else if ( !str_cmp( "accelerate", arg ) )
              choice = SKL_ACCELERATE;
      else if ( !str_cmp( "timer lock", arg ) )
              choice = SKL_TLOCK;
      else if ( !str_cmp( "encase", arg ) )
              choice = SKL_ENCASE;
      else if ( !str_cmp( "aura shield", arg ) )
              choice = SKL_ASHIELD;
      else if ( !str_cmp( "practice", arg ) )
              choice = SKL_PRACTICE;
      else if ( !str_cmp( "knowledge", arg ) )
              choice = SKL_KNOWLEDGE;
      else if ( !str_cmp( "planar caller", arg ) )
              choice = SKL_PLANAR;
      else if ( !str_cmp( "shadow grasp", arg ) )
              choice = SKL_GRASP;
      else if ( !str_cmp( "critical", arg ) )
              choice = SKL_CRITICAL;
      else if ( !str_cmp( "anticipate", arg ) )
              choice = SKL_ANTICIPATE;
      else if ( !str_cmp( "godpeace", arg ) )
              choice = SKL_GODPEACE;
      else if ( !str_cmp( "dischard", arg ) )
              choice = SKL_DISCHARD;
      else if ( !str_cmp( "retort", arg ) )
              choice = SKL_RETORT;
      else if ( !str_cmp( "swerve", arg ) )
              choice = SKL_SWERVE;
      else if ( !str_cmp( "power blow", arg ) )
              choice = SKL_POWERB;
      else if ( !str_cmp( "shock absorber", arg ) )
              choice = SKL_STUNA;
      else if ( !str_cmp( "majesty", arg ) )
              choice = SKL_MAJESTY;
      else if ( !str_cmp( "desanctify", arg ) )
              choice = SKL_DESANCTIFY;
      else if ( !str_cmp( "focus", arg ) )
              choice = SKL_FOCUS;
      else if ( !str_cmp( "balance", arg ) )
              choice = SKL_BALANCE;
      else if ( !str_cmp( "illusion blur", arg ) )
              choice = SKL_ILLUSION;
      else if ( !str_cmp( "weapon guard", arg ) )
              choice = SKL_GUARD;
      else if ( !str_cmp( "frost bite", arg ) )
              choice = SKL_FBITE;
              else {
                      skillset = 5;

                      if ( !str_cmp( "scorching", arg ) )
                         choice = SKL2_SCORCHING;
                         else if ( !str_cmp( "maintenance", arg ) )
                                 choice = SKL2_MAINTENANCE;
                         else if ( !str_cmp( "devil luck", arg ) )
                                 choice = SKL2_DLUCK;
                         else if ( !str_cmp( "assassin mind", arg ) )
                                 choice = SKL2_AMIND;
                         else if ( !str_cmp( "anti aura", arg ) )
                                 choice = SKL2_ANTIAURA;
                         else if ( !str_cmp( "reminisce", arg ) )
                                 choice = SKL2_REMINISCE;
                         else if ( !str_cmp( "pelt", arg ) )
                                 choice = SKL2_THROW;
                         else if ( !str_cmp( "trip", arg ) )
                                 choice = SKL2_TRIP;
                         else if ( !str_cmp( "mime", arg ) )
                                 choice = SKL2_MIME;
                         else if ( !str_cmp( "sniper sight", arg ) )
                                 choice = SKL2_SSIGHT;
                         else if ( !str_cmp( "steal", arg ) )
                                 choice = SKL2_STEAL;
                         else if ( !str_cmp( "presence", arg ) )
                                 choice = SKL2_PRESENCE;
                         else if ( !str_cmp( "shadow swap", arg ) )
                                 choice = SKL2_SSWAP;
                         else if ( !str_cmp( "mind control", arg ) )
                                 choice = SKL2_MCONTROL;
                         else if ( !str_cmp( "helping soul", arg ) )
                                 choice = SKL2_HSOUL;
                         else if ( !str_cmp( "leeching touch", arg ) )
                                 choice = SKL2_LTOUCH;
                         else if ( !str_cmp( "stunning blow", arg ) )
                                 choice = SKL2_STBLOW;
                         else if ( !str_cmp( "roll", arg ) )
                                 choice = SKL2_ROLL;
                         else if ( !str_cmp( "subtle pinch", arg ) )
                                 choice = SKL2_SPINCH;
                         else if ( !str_cmp( "rigid body", arg ) )
                                 choice = SKL2_RBODY;
                         else if ( !str_cmp( "serpent fist", arg ) )
                                 choice = SKL2_SFIST;
                         else if ( !str_cmp( "iron body", arg ) )
                                 choice = SKL2_IBODY;
                         else if ( !str_cmp( "first attack", arg ) )
                                 choice = SKL2_FATTACK;
                         else if ( !str_cmp( "final blow", arg ) )
                                 choice = SKL2_FBLOW;
                         else if ( !str_cmp( "stagger", arg ) )
                                 choice = SKL2_STAGGER;
                         else if ( !str_cmp( "switch", arg ) )
                                 choice = SKL2_SWITCH;
                         else if ( !str_cmp( "disarm", arg ) )
                                 choice = SKL2_DISARM;
                         else if ( !str_cmp( "grip", arg ) )
                                 choice = SKL2_GRIP;
                         else if ( !str_cmp( "snake strike", arg ) )
                                 choice = SKL2_SSTRIKE;
                         else if ( !str_cmp( "shining blow", arg ) )
                                 choice = SKL2_SBLOW;
                         else if ( !str_cmp( "thawing heart", arg ) )
                                 choice = SKL2_THEART;
                         else {
                                 do_skillbuy( ch, "" );
                                 return;
                              }
                   }

   if ( ch->cmbt[4] == 0 )
      {
         if ( ch->pkill / 3 < ch->cmbt[6] + 1 )
            {
               stc( "You don't have enough player kills to buy your next skill.\n\r", ch );
               return;
            }
      }

   if ( IS_SET( ch->cmbt[skillset], choice ) )
      {
         stc( "You have already bought that skill warrior.\n\r", ch );
         return;
      }

   stc( "#wYou flex your arms showing your tremedous power as you #o#zbuy#n a #Cnew #yskill#w!#n\n\r", ch );
   SET_BIT( ch->cmbt[skillset], choice );

   if ( ch->cmbt[4] == 0 )
      {
         if ( ( ++ch->cmbt[6] % 2 ) == 0 && ch->cmbt[6] < 11 )
            ch->max_hit += 1000000;
      }
      else ch->cmbt[4]--;
}

void do_warcry( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( ch->race < 10 )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( IS_SET( ch->more2, MORE2_WARCRY ) )
       {
	    send_to_char( "You are already releasing the true anger of a warrior.\n\r", ch );
          return;
       }

    if ( ch->sex == SEX_FEMALE )
       {
          act( "You yell '#yFor my dead sisters...#n'", ch, NULL, NULL, TO_CHAR );
          act( "$n yells '#yFor my dead sisters...#n'", ch, NULL, NULL, TO_ROOM );
          act( "You yell '#yFor the honour of our Skye and Desire clans...#n'", ch, NULL, NULL, TO_CHAR );
          act( "$n yells '#yFor the honour of our Skye and Desire clans...#n'", ch, NULL, NULL, TO_ROOM );
          act( "You yell '#yAnd for the right to die for the Queen...#n'", ch, NULL, NULL, TO_CHAR );
          act( "$n yells '#yAnd for the right to die for the Queen...#n'", ch, NULL, NULL, TO_ROOM );
          act( "You yell '#yFeel my wrath and kneel before me weak mortals!!!#n'", ch, NULL, NULL, TO_CHAR );
          act( "$n yells '#yFeel my wrath and kneel before me weak mortals!!!#n'", ch, NULL, NULL, TO_ROOM );
          act("\n\rYou brace yourself as you feel the souls of the legendary sisters fill your worthy body.",ch,NULL,NULL,TO_CHAR);
          act("\n\r$n braces $mself as devastating energy emits of $s muscular body.",ch,NULL,NULL,TO_ROOM);
       }
       else {
               act( "You yell '#yFor my dead brothers...#n'", ch, NULL, NULL, TO_CHAR );
               act( "$n yells '#yFor my dead brothers...#n'", ch, NULL, NULL, TO_ROOM );
               act( "You yell '#yFor the honour of our Kenni and Akuma clans...#n'", ch, NULL, NULL, TO_CHAR );
               act( "$n yells '#yFor the honour of our Kenni and Akuma clans...#n'", ch, NULL, NULL, TO_ROOM );
               act( "You yell '#yAnd for the right to die for the King...#n'", ch, NULL, NULL, TO_CHAR );
               act( "$n yells '#yAnd for the right to die for the King...#n'", ch, NULL, NULL, TO_ROOM );
               act( "You yell '#yFeel my wrath and kneel before me weak mortals!!!#n'", ch, NULL, NULL, TO_CHAR );
               act( "$n yells '#yFeel my wrath and kneel before me weak mortals!!!#n'", ch, NULL, NULL, TO_ROOM );
               act("\n\rYou brace yourself as you feel the souls of the legendary brothers fill your worthy body.",ch,NULL,NULL,TO_CHAR);
               act("\n\r$n braces $mself as devastating energy emits of $s muscular body.",ch,NULL,NULL,TO_ROOM);
            }

    SET_BIT( ch->more2, MORE2_WARCRY );

    WAIT_STATE( ch, 6 );
}

void do_dragonbuster( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( ch->pcdata->clanrank < 6 )
       {
	    send_to_char("Huh?\n\r",ch);
 	    return;
       }

    if ( ch->jedi_timer > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    ch->jedi_timer = 200;
    ch->wait = 6;
    ch->cmbt[5] = 50;
    act( "You go '#yHmph...#n' as you suddenly open your palm...", ch, NULL, NULL, TO_CHAR );
    act( "$n goes '#yHmph...#n' as $e suddenly opens $s palm...", ch, NULL, NULL, TO_ROOM );
    act( "A spark flashes to and for amidst your hand as an outline of a blade is formed.", ch, NULL, NULL, TO_CHAR );
    act( "A spark flashes to and for amidst $n's hand as an outline of a blade is formed.", ch, NULL, NULL, TO_ROOM );
    act( "The room suddenly flashes with an intense light as your blade materializes.", ch, NULL, NULL, TO_CHAR );
    act( "The room suddenly flashes with an intense light as $n's blade materializes.", ch, NULL, NULL, TO_ROOM );
    act( "Pointing the #oDragon #PBuster#n at your foes, you mutter coldly '#eJust Die...#n'", ch, NULL, NULL, TO_CHAR );
    act( "Pointing the #oDragon #PBuster#n at $s foes, $n mutters coldly '#eJust Die...#n'", ch, NULL, NULL, TO_ROOM );
}

void do_control( CHAR_DATA *ch, char *argument )
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

  if ( !IS_ITEMAFF( ch, ITEMA_GRAHF ) )
     {
        send_to_char( "You aren't wearing the #PM#pe#Pm#pe#Pn#pt#Po#n warrior...\n\r", ch );
        return;
     }

  if ( arg1[0] == '\0' || arg2[0] == '\0' )
     {
        send_to_char( "Command whom to do what?\n\r", ch );
        return;
     }

  if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
     {
        send_to_char( "They aren't here.\n\r", ch );
        return;
     }

  if ( victim == ch )
     {
        send_to_char( "How can you command yourself??\n\r", ch );
        return;
     }

  if ( ch->control_timer > 0 )
     {
        send_to_char( "You are still recovering from the last one.\n\r", ch );
        return;
     }

  if ( is_safe(ch,victim) )
     return;

  if ( !str_prefix( arg2, "give" ) || !str_prefix( arg2, "quit" ) 
       || !str_prefix( arg2, "delete" ) || !str_prefix( arg2, "detox" )
       || !str_prefix( arg2, "gift" ) || !str_prefix( arg2, "drop" )
       || !str_prefix( arg2, "consent" ) ) 
     { 
         send_to_char( "You can't order them to do that.\n\r", ch ); 
         return; 
     }  

  ch->wait = 12;
  sprintf( buffy, "%s %s", arg2, argument );
  act("You softly close your eyes as you place your hands on your waist.",ch,NULL,NULL,TO_CHAR);
  act("$n softly closes $s eyes as $e places $s hands on $s waist.",ch,NULL,NULL,TO_ROOM);

  ch->control_timer = 20;
  sprintf( buf, "Foolish Lamb, I order you to %s", buffy );
  do_say(ch,buf); 

  interpret( victim, buffy );
}

void do_destroyer( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom != 2 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->clanrank < 6 )
      {
         stc("#CYou aren't ranked #yhigh #Cenough to use this technique #ySOLDIER#C.#n\n\r",ch);
         return;
      }

   if ( ch->ctimer[18] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to send K9 after?\n\r", ch );
         return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_world( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   ch->ctimer[18] = 4;
   ch->attack_type = ATTACK_PHYSICAL;
   act( "#yYou #Cpoint your finger at #w$N #Cas a blur flies past you!#n",ch,NULL,victim,TO_CHAR);
   act( "#y$n #Cpoints $s finger at #wyou #Cas a blur flies past $m!#n",ch,NULL,victim,TO_VICT);
   act( "#y$n #Cpoints $s finger at #w$N #Cas a blur flies past $m!#n",ch,NULL,victim,TO_NOTVICT);

   if ( ch->in_room == victim->in_room )
      {
         act( "#wYou #Care suddenly knocked into the ground as the #oblur #Ccrash into you!#n",victim,NULL,NULL,TO_CHAR);
         act( "#w$n #Cis suddenly knocked into the ground as the #oblur #Ccrash into $m!#n",victim,NULL,NULL,TO_ROOM);

         hurt_person( ch, victim, 500000, TRUE );

         act( "#CThe #ofur #Con #yyour #Cdog frizzle as it growls savagely at your foe.#n", ch, NULL, NULL, TO_CHAR );
         act( "#CThe #ofur #Con #y$n#C's dog frizzle as it growls savagely at you.#n", ch, NULL, NULL, TO_ROOM );
         return;
      }
      else if ( ch->in_room->area == victim->in_room->area )
              {
                 act( "#wYour #Cneck is suddenly snapped by a #olarge dog #Cas it drags you away!#n",victim,NULL,NULL,TO_CHAR);
                 act( "#w$n's #Cneck is suddenly snapped by a #olarge dog #Cas it drags $m away!#n",victim,NULL,NULL,TO_ROOM);

                 stop_fighting( victim, TRUE );
                 char_from_room( victim );
                 char_to_room( victim, ch->in_room );

                 act( "#CJust as #w$N #Crecovers from the dizziness, #yyour ##odog #Cslams hard into $M!#n",ch,NULL,victim,TO_CHAR);
                 act( "#CJust as #wyou #Crecover from the dizziness, #y$n#C's #odog #Cslams hard into you!#n",ch,NULL,victim,TO_VICT);
                 act( "#CJust as #w$N #Crecovers from the dizziness, #y$n#C's #odog #Cslams hard into $M!#n",ch,NULL,victim,TO_NOTVICT);

                 hurt_person( ch, victim, 750000, TRUE );
              }
              else {
                       act( "#wYou #Care suddenly knocked away by a #w#zTREMEDOUS#n #Cforce!!!#n",victim,NULL,NULL,TO_CHAR);
                       act( "#w$n #Cis suddenly knocked away by a #w#zTREMEDOUS#n #Cforce!!!#n",victim,NULL,NULL,TO_ROOM);

                       special_slam( victim, number_range( 0, 3 ) );
                       hurt_person( ch, victim, 1000000, TRUE );
                   }
}

void do_blaster( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom != 2 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->clanrank < 6 )
      {
         stc("#CYou aren't ranked #yhigh #Cenough to use this technique #ySOLDIER#C.#n\n\r",ch);
         return;
      }

   if ( ch->ctimer[17] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast?\n\r", ch );
         return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_area( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   ch->ctimer[17] = 4;
   ch->attack_type = ATTACK_PROJECTILE;
   ch->wait = 12;
   act( "#yYou #Cpoint your shotgun at #w$N #Cas you start chuckling softly!#n",ch,NULL,victim,TO_CHAR);
   act( "#y$n #Cpoints $s shotgun at #wyou #Cas $e starts chuckling softly!#n",ch,NULL,victim,TO_VICT);
   act( "#y$n #Cpoints $s shotgun at #w$N #Cas $e starts chuckling softly!#n",ch,NULL,victim,TO_NOTVICT);
   act( "#yYou #Cthen send #w$M #Cflying with a #RPOWERFUL #wblast #Cfrom your gun!!!#n",ch,NULL,victim,TO_CHAR);
   act( "#y$n #Cthen send #wyou #Cflying with a #RPOWERFUL #wblast #Cfrom $s gun!!!#n",ch,NULL,victim,TO_VICT);
   act( "#y$n #Cthen send #w$M #Cflying with a #RPOWERFUL #wblast #Cfrom $s gun!!!#n",ch,NULL,victim,TO_NOTVICT);

   hurt_person( ch, victim, 1000000, TRUE );
}

void do_shadow_walk( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim, *mount;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument(argument, arg);

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom != 3 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->clanrank < 5 )
      {
         stc("#PLloth #edoesn't give you the approval of using this technique yet.#n\n\r",ch);
         return;
      }

   if ( time_info.hour > 6 && time_info.hour < 19 && !IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) )
      {
         stc( "#yYou #ecan only use this skill under the #Pcovers #eof darkness.#n\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char("Who do you wish to walk to via the paths of darkness?\r\n", ch);
         return;
      }
	
   if ( ( victim = get_char_world( ch, arg ) ) == NULL || victim == ch || victim->in_room == NULL
        || ch->in_room == NULL )
      {
         send_to_char( "You attempt to walk through the shadows, but fail.\n\r", ch );
         return;
      } 

   if ( victim->in_room == ch->in_room )
      {
         send_to_char( "But you are already there!\n\r", ch );
         return;
      }

   if ( victim->in_room->vnum < 100 )
      {
         stc("You cannot travel to that room\n\r",ch);
         return;
      }

   send_to_char("#yYou #edissipate into the #Pshadows#e.#n\n\r",ch);
   act("#y$n #edissipates into the #Pshadows#e, slowly vanishing from sight.#n",ch,NULL,NULL,TO_ROOM);

   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   do_look(ch,"");

   send_to_char("#yYou #ecoalesce from the darkness which surrounds you.#n\n\r",ch);

   if ( ( mount = ch->mount ) == NULL ) 
      return;

   char_from_room( mount );
   char_to_room( mount, ch->in_room );
}

void do_shroud_strike( CHAR_DATA *ch, char *argument )
{
   int count; 
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( ch->pcdata->kingdom != 3 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->clanrank < 3 )
      {
         stc("#PLloth #edoesn't give you the approval of using this technique yet.#n\n\r",ch);
         return;
      }

   if ( nogang != 0 )
      {
         stc( "#yYou #ecan only use this skill in #Pgroup #ebattles.#n\n\r", ch );
         return;
      }

   if ( time_info.hour > 6 && time_info.hour < 19 && !IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) )
      {
         stc( "#yYou #ecan only use this skill under the #Pcovers #eof darkness.#n\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "#PStrike #ewhom?#n\n\r", ch );
         return;
      }

   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   count = 0;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group( ch, vch ) || IS_NPC( vch ) || vch == ch || vch == victim
           || vch->pcdata->kingdom != ch->pcdata->kingdom
           || vch->fight_timer > 0 )
         continue;
   }

   if ( count == 0 )
      {
         send_to_char( "#yYou #eneed #Ppartners #eto perform this technique with.#n\n\r", ch );
         return;
      }

   act( "#yYou #emake subtle hand signals to your #Pgroup #eas you all dash towards #w$N#e.#n", ch, NULL, victim, TO_CHAR );
   act( "#y$n #emakes subtle hand signals to $s #Pgroup #eas they all dash towards #wyou#e.#n", ch, NULL, victim, TO_VICT );
   act( "#y$n #emakes subtle hand signals to $s #Pgroup #eas you all dash towards #w$N#e.#n", ch, NULL, victim, TO_NOTVICT );
   act( "#yYou #ethrust your dagger into $N's chest before pushing $M away violently#e!#n", ch, NULL, victim, TO_CHAR );
   act( "#y$n #ethrusts $s dagger into your chest before pushing you away violently#e!#n", ch, NULL, victim, TO_VICT );
   act( "#y$n #ethrusts $s dagger into $N's chest before pushing $M away violently#e!#n", ch, NULL, victim, TO_NOTVICT );

   count = 0;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_type = ATTACK_PHYSICAL;
   hurt_person( ch, victim, 1000000, TRUE );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group( ch, vch ) || IS_NPC( vch ) || vch == ch || vch == victim
           || vch->pcdata->kingdom != ch->pcdata->kingdom
           || vch->fight_timer > 0 )
         continue;

      count++;

      act( "#yYou #eslash your sword into $N's chest before punching $M down#e!#n", vch, NULL, victim, TO_CHAR );
      act( "#y$n #eslashes $s sword into your chest before punching $M down#e!#n", vch, NULL, victim, TO_VICT );
      act( "#y$n #eslashes $s sword into $N's chest before punching $M down#e!#n", vch, NULL, victim, TO_NOTVICT );
      vch->ele_attack = ATTACK_DARK;
      vch->attack_type = ATTACK_PHYSICAL;
      hurt_person( vch, victim, 1000000, TRUE );

      if ( count == 5 || victim == NULL || victim->in_room == NULL || victim->position < POS_STUNNED )
         break;
   }
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   one_argument( argument, arg );

   if ( ch->pcdata->kingdom != 3 )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pcdata->clanrank < 4 )
      {
         stc("#PLloth #edoesn't give you the approval of using this technique yet.#n\n\r",ch);
         return;
      }

   if ( ch->ctimer[18] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Backstab whom?\n\r", ch );
         return;
      }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( is_safe( ch, victim ) )
      return;

   check_killer( ch, victim );
   WAIT_STATE( ch, 8 );
   ch->ctimer[18] = 4;

   act("#yYou #edrive your dagger into the #Pspinal #ecord of #w$N#e's back.#n",ch,NULL,victim,TO_CHAR);
   act("#y$n #edrives $s dagger into the #Pspinal #ecord of #wyour #eback.#n",ch,NULL,victim,TO_VICT);
   act("#y$n #edrives $s dagger into the #Pspinal #ecord of #w$N#e's back.#n",ch,NULL,victim,TO_NOTVICT);
   victim->mana -= 2500000;
   victim->move -= 2500000;
   hurt_person( ch, victim, 1500000, TRUE );
}
