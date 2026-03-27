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


/**********************************
 * All kinds of new stuff by Jobo *
 **********************************/

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

void load_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../area/leader.txt", "r")) == NULL)
  {   
    log_string("Error: leader.txt not found!");
    exit(1);
  }
  leader_board.bestpk_name = fread_string(fp);  
  leader_board.bestpk_number = fread_number(fp);
  leader_board.pk_name = fread_string(fp);
  leader_board.pk_number = fread_number(fp);
  leader_board.pd_name = fread_string(fp);
  leader_board.pd_number = fread_number(fp);
  leader_board.mk_name = fread_string(fp);
  leader_board.mk_number = fread_number(fp);
  leader_board.md_name = fread_string(fp);
  leader_board.md_number = fread_number(fp);
  leader_board.tt_name = fread_string(fp);
  leader_board.tt_number = fread_number(fp);
  leader_board.qc_name = fread_string(fp);
  leader_board.qc_number = fread_number(fp);
  leader_board.pkpd_name = fread_string(fp);
  leader_board.pkpd_number = fread_number(fp);
  leader_board.t_name = fread_string(fp);
  leader_board.t_number = fread_number(fp);
  fclose(fp);
}

void save_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../area/leader.txt","w")) == NULL)
  {
    log_string("Error writing to leader.txt");
  }
  fprintf(fp, "%s~\n", leader_board.bestpk_name); 
  fprintf(fp, "%d\n", leader_board.bestpk_number);
  fprintf(fp, "%s~\n", leader_board.pk_name);
  fprintf(fp, "%d\n", leader_board.pk_number);
  fprintf(fp, "%s~\n", leader_board.pd_name);     
  fprintf(fp, "%d\n", leader_board.pd_number);
  fprintf(fp, "%s~\n", leader_board.mk_name);
  fprintf(fp, "%d\n", leader_board.mk_number);
  fprintf(fp, "%s~\n", leader_board.md_name);
  fprintf(fp, "%d\n", leader_board.md_number);
  fprintf(fp, "%s~\n", leader_board.tt_name);
  fprintf(fp, "%d\n", leader_board.tt_number);
  fprintf(fp, "%s~\n", leader_board.qc_name);
  fprintf(fp, "%d\n", leader_board.qc_number);
  fprintf(fp, "%s~\n", leader_board.pkpd_name);
  fprintf(fp, "%d\n", leader_board.pkpd_number);
  fprintf(fp, "%s~\n", leader_board.t_name);
  fprintf(fp, "%d\n", leader_board.t_number);
  fclose (fp);
}

void do_leader( CHAR_DATA *ch, char *argument )
{    
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  stc("#r-==--==#L**#r==--==#L**#r==--==#L**#r== #GLEADER BOARD #r==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#n\n\r\n\r",ch);

  send_to_char("   #oMr. Evolver      #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.bestpk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d#o transmigration levels#n\n\r",leader_board.bestpk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMr. Deadly       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.pk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #opkills#n\n\r",leader_board.pk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMr. Hacker       #C--->    #GLindel        #owith #G1 #ofailed attempt#n\n\r",ch);

  send_to_char("   #oMr. Slayer       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.mk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #omkills#n\n\r",leader_board.mk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMr. Champion     #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.t_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #otournament wins#n\n\r",leader_board.t_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMr. No life      #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.tt_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oHours played#n\n\r",leader_board.tt_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMr. Miser        #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.qc_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oGold pieces locked away#n\n\r",leader_board.qc_number);
  send_to_char(buf,ch);

  send_to_char("   #oMr. Legend       #C--->    ",ch);
  sprintf(buf, "#G%-13s", leader_board.pkpd_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #olegend points#n\n\r",leader_board.pkpd_number);
  send_to_char(buf,ch);

  stc("\n\r#r-==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#n\n\r",ch);
}

void check_leaderboard( CHAR_DATA *ch, char *argument )
{
  bool changed = FALSE;

  if ( IS_NPC(ch) )
     return;
  if (ch->level > 6) return;
  if ( get_translevel( ch ) > leader_board.bestpk_number )
  {
    leader_board.bestpk_number = get_translevel( ch );
    free_string(leader_board.bestpk_name);
    leader_board.bestpk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->mdeath > leader_board.md_number)
  {
    leader_board.md_number = ch->mdeath;
    free_string(leader_board.md_name);
    leader_board.md_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->mkill > leader_board.mk_number)
  {
    leader_board.mk_number = ch->mkill;
    free_string(leader_board.mk_name);
    leader_board.mk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pkill > leader_board.pk_number)
  {
    leader_board.pk_number = ch->pkill;
    free_string(leader_board.pk_name);
    leader_board.pk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pdeath > leader_board.pd_number)
  {
    leader_board.pd_number = ch->pdeath;
    free_string(leader_board.pd_name);
    leader_board.pd_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pcdata->twins > leader_board.t_number)
  {
    leader_board.t_number = ch->pcdata->twins;
    free_string(leader_board.t_name);
    leader_board.t_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->bank > leader_board.qc_number)
  {
    leader_board.qc_number = ch->bank;
    free_string(leader_board.qc_name);
    leader_board.qc_name = str_dup(ch->name);
    changed = TRUE;
  }
  if ((get_age(ch)-17)*2 > leader_board.tt_number)
  {
    leader_board.tt_number = (get_age(ch)-17)*2;
    free_string(leader_board.tt_name);
    leader_board.tt_name = str_dup(ch->name);
    changed = TRUE;
  }

  if (ch->pcdata->faithpoints > leader_board.pkpd_number)
  {
    leader_board.pkpd_number = ch->pcdata->faithpoints;
    free_string(leader_board.pkpd_name);
    leader_board.pkpd_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (changed) save_leaderboard();
}

void do_leaderclear(CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (ch->level < 7) return;
  free_string(leader_board.bestpk_name);
  leader_board.bestpk_name = str_dup("Noone");
  leader_board.bestpk_number = 0;
  free_string(leader_board.pk_name);
  leader_board.pk_name = str_dup("Noone");
  leader_board.pk_number = 0;
  free_string(leader_board.pd_name);
  leader_board.pd_name = str_dup("Noone");
  leader_board.pd_number = 0;
  free_string(leader_board.md_name);
  leader_board.md_name = str_dup("Noone");
  leader_board.md_number = 0;
  free_string(leader_board.mk_name);
  leader_board.mk_name = str_dup("Noone");
  leader_board.mk_number = 0;
  free_string(leader_board.qc_name);
  leader_board.qc_name = str_dup("Noone");
  leader_board.qc_number = 0;
  free_string(leader_board.tt_name);
  leader_board.tt_name = str_dup("Noone");
  leader_board.tt_number = 0;
  free_string(leader_board.pkpd_name);
  leader_board.pkpd_name = str_dup("Noone");
  leader_board.pkpd_number = 0;
  leader_board.t_name = str_dup("Noone");
  leader_board.t_number = 0;
  save_leaderboard();
  send_to_char("Leader board cleared.\n\r",ch);
}
