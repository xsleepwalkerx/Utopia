/*
 * who_html.c
 *
 * This piece of code allows the mud to create a html file, that looks
 * a bit like the 'who screen'. By calling this function every few ticks,
 * the mud can keep an updated listing which can be accessed from the web.
 *
 * Code by Brian Graversen aka Jobo
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define HTML_DIR   "../../public_html/"

char * plr_class_rank( int generation )
{
  static char buf[512];

  buf[0] = '\0';

  if ( generation == 100 )
     strcat(buf, " <font color=\"#00FF00\">No<font color=\"#FFFFFF\">vi<font color=\"#00FF00\">ce</font>");
     else if ( generation > 89 )
             strcat(buf, " <font color=\"#FFFFFF\">Dr<font color=\"#FFFF00\">eam<font color=\"#FFFFFF\">er</font>");
     else if ( generation > 79 )
             strcat(buf, " <font color=\"#33CC66\">Ach<font color=\"#FFFFFF\">ie<font color=\"#33CC66\">ver</font>");
     else if ( generation > 69 )
             strcat(buf, " <font color=\"#FFFFFF\">Com<font color=\"#660000\">bat<font color=\"#FFFFFF\">ant</font>");
     else if ( generation > 59 )
             strcat(buf, " <font color=\"#00FF00\">Gla<font color=\"#33CCFF\">dia<font color=\"#00FF00\">tor</font>");
     else if ( generation > 49 )
             strcat(buf, " <font color=\"#FFFFFF\">Sol<font color=\"#00FF00\">d<font color=\"#FFFFFF\">ier</font>");
     else if ( generation > 29 )
             strcat(buf, " <font color=\"#00FF00\">Ge<font color=\"#FFFF00\">ner<font color=\"#00FF00\">al</font>");
     else if ( generation > 9 )
             strcat(buf, " <font color=\"#660000\">Ma<font color=\"#33CC66\">st<font color=\"#660000\">er</font>");
     else if ( generation > 1 )
             strcat(buf, " <font color=\"#FFFF00\">Grd <font color=\"#660000\">Mas<font color=\"#FFFF00\">ter</font>");
             else strcat(buf, " <font color=\"#666666\">Le<font color=\"#FFFFFF\">ge<font color=\"#666666\">nd</font>");

  return (buf[0] != '\0') ? buf+1 : "none";
}

char * plr_class_name( CHAR_DATA *ch, int clannum, int arg )
{
  static char buf[512];

  buf[0] = '\0';

  switch( arg )
  {
    default: strcat(buf, " <font color=\"#FFFF00\">H<font color=\"#FFFFFF\">u<font color=\"#FFFF00\">m<font color=\"#FFFFFF\">a<font color=\"#FFFF00\">n</font>");
             break;
    case CLASS_DEMON: strcat(buf, " <font color=\"#660000\">[<font color=\"#666666\">D<font color=\"#FF0000\">e<font color=\"#FFFFFF\">m<font color=\"#FF0000\">o<font color=\"#666666\">n<font color=\"#660000\">]</font>");
                      break;
    case CLASS_MAGE: strcat(buf, " <font color=\"#CC33CC\">{Wa<font color=\"#FFFF00\">rlo<font color=\"#FF0000\">ck}</font>");
                     break;
    case CLASS_AMAZON: strcat(buf, " <font color=\"#009900\">[0o<font color=\"#FFFFFF\">:<font color=\"#33CCFF\">Ama<font color=\"#CC33CC\">zon<font color=\"#FFFFFF\">:<font color=\"#009900\">o0]</font>");
                       break;
    case CLASS_SORCEROR: strcat(buf, " <font color=\"#CC33CC\">{<font color=\"#666666\">Sorc<font color=\"#FFFFFF\">eror<font color=\"#CC33CC\">}</font>");
                         break;
    case CLASS_WIZARD: strcat(buf, " <font color=\"#FFFF00\">{<font color=\"#FFFFFF\">Wi<font color=\"#666666\">za<font color=\"#33CCFF\">rd<font color=\"#FFFF00\">}</font>");
                       break;
    case CLASS_NECROMANCER: strcat(buf, " <font color=\"#666666\">{Nec<font color=\"#FF0000\">roman<font color=\"#666666\">cer}</font>");
                            break;
    case CLASS_SUMMONER: strcat(buf, " <font color=\"#33CCFF\">{Sum<font color=\"#FFFFFF\">m<font color=\"#009900\">o<font color=\"#33CCFF\">ner}</font>");
                         break;
    case CLASS_WEREWOLF: strcat(buf, " <font color=\"#FFFF00\">((<font color=\"#660000\">We<font color=\"#009900\">rewo<font color=\"#660000\">lf<font color=\"#FFFF00\">))</font>");
                         break;
    case CLASS_VAMPIRE: strcat(buf, " <font color=\"#FF0000\"><<<<font color=\"#FFFF00\">Vamp<font color=\"#666666\">ire<font color=\"#FF0000\">>>></font>");
                        break;
    case CLASS_HIGHLANDER: strcat(buf, " <font color=\"#33CCFF\">-=<font color=\"#FFFF00\">Jedi <font color=\"#00FF00\">Knight<font color=\"#33CCFF\">=-</font>");
                           break;
    case CLASS_SAMURAI: strcat(buf, " <font color=\"#33CCFF\">[=)<font color=\"#FFFFFF\">Gua<font color=\"#009900\">rd<font color=\"#FFFFFF\">ian<font color=\"#FFFF00\">(=]</font>");
                        break;
    case CLASS_DROW: strcat(buf, " <font color=\"#666666\">.o0<font color=\"#CC33CC\">Drow<font color=\"#666666\">0o.</font>");
                     break;
    case CLASS_ASSASSIN: strcat(buf, " <font color=\"#666666\">***<font color=\"#FFFF00\">Assassin<font color=\"#666666\">***</font>");
                         break;
    case CLASS_HUNTER: strcat(buf, " <font color=\"#007700\">x[-<font color=\"#FFFF00\">(<font color=\"#33CC66\">Hunter<font color=\"#FFFF00\">)<font color=\"#007700\">-]x</font>");
                        break;
    case CLASS_MONK: strcat(buf, " <font color=\"#FFFFFF\">.x[<font color=\"#FFFF00\">Champion<font color=\"#FFFFFF\">]x.</font>");
                     break;
    case CLASS_DARAMON: strcat(buf, " <font color=\"#CC33CC\">.x[<font color=\"#FFFFFF\">Da<font color=\"#FFFF00\">ram<font color=\"#FFFFFF\">on<font color=\"#CC33CC\">]x.</font>");
                        break;
    case CLASS_NINJA: strcat(buf, " <font color=\"#0000BB\">***<font color=\"#666666\">Ni<font color=\"#660000\">n<font color=\"#666666\">ja<font color=\"#0000BB\">***</font>");
                      break;
    case CLASS_ANGEL: strcat(buf, " <font color=\"#FFFF00\">~~~<font color=\"#FFFFFF\">An<font color=\"#33CCFF\">g<font color=\"#FFFFFF\">el<font color=\"#FFFF00\">~~~</font>");
                      break;
    case CLASS_PALADIN: strcat(buf, " <font color=\"#FFFFFF\">==)=<font color=\"#33CCFF\">Kni<font color=\"#FFFF00\">ght<font color=\"#666666\">---</font>");
                        break;
    case CLASS_DRAGOON: strcat(buf, " <font color=\"#33CC66\">---=<font color=\"#009900\">Dra<font color=\"#FFFFFF\">goon<font color=\"#666666\">---)))</font>");
                        break;
    case CLASS_DRAGON: strcat(buf, " <font color=\"#CC33CC\">-*-<font color=\"#FF0000\">D<font color=\"#666666\">rago<font color=\"#FF0000\">n<font color=\"#CC33CC\">-*-</font>");
                       break;
    case CLASS_DARK_KNIGHT: strcat(buf, " <font color=\"#660000\">==)=<font color=\"#666666\">Dark <font color=\"#33CCFF\">Knight<font color=\"#FFFF00\">---</font>");
                            break;
    case CLASS_DAYWALKER: strcat(buf, " <font color=\"#FF0000\">(-]=O <font color=\"#FFFF00\">Day<font color=\"#33CCFF\">wal<font color=\"#FFFF00\">ker <font color=\"#FF0000\">O=[-)</font>");
                          break;
    case CLASS_SAIYAN: strcat(buf, " <font color=\"#FF0000\">(((<font color=\"#FFFF00\">Saiya<font color=\"#FFFFFF\">-<font color=\"#FF0000\">Jin<font color=\"#FFFF00\">)))</font>");
                       break;
    case CLASS_UNDEAD_KNIGHT: strcat(buf, " <font color=\"#666666\">\\_-<font color=\"#660000\">Undead <font color=\"#FFFFFF\">Knight<font color=\"#666666\">-_/-</font>");
                              break;
  }

  return (buf[0] != '\0') ? buf+1 : "none";
}

void parse_who()
{
  CHAR_DATA *ch;
  DESCRIPTOR_DATA *d;
  FILE *fp;
  char whofile[200];
  char buf[MAX_STRING_LENGTH];
  char fadein_data[MAX_STRING_LENGTH];
  char fadeout_data[MAX_STRING_LENGTH];
  char body_data[4 * MAX_STRING_LENGTH]; // should be enough
  int total = 0;

  return;
  sprintf(whofile, "%s%s", HTML_DIR, "who.html");

  if ( ( fp = fopen(whofile, "w")) == NULL )
     {
        log_string("Error writing to who.html");
        return;
     }

  /*
   * Setting up the header and foot of the file
   */
  sprintf(fadein_data, "<html>\n\r");
  strcat(fadein_data, "<body text=\"#FFFFFF\" bgcolor=\"#000000\" link=\"#33CCFF\" vlink=\"#33FF33\" alink=\"#FF0000\">\n\r");
  strcat(fadein_data, "<font face=\"Courier\" Size=\"3\">\n\r");
  strcat(fadein_data, "<b><font color=\"#33FF33\"><center>Online Players</center></font></b><font color=\"#FFFFFF\"></font>\n\r");
  strcat(fadein_data, "<p><table WIDTH=\"100%\">\n\r");
  strcat(fadein_data, "\n\r");
  strcat(fadein_data, "<tr>\n\r");
  strcat(fadein_data, "<td><b><u>Name</u></b></td>\n\r");
  strcat(fadein_data, "<td><b><u>Class</u></b></td>\n\r");
  strcat(fadein_data, "<td><b><u>Rank</u></b></td>\n\r");
  strcat(fadein_data, "</tr>\n\r");

  sprintf(fadeout_data, "</body>\n\r");
  strcat(fadeout_data, "</html>\n\r");

  /*
   * Now let's fill out the body of the file.
   */
  sprintf(body_data, "\n\r");
  strcat(body_data, "<font face=\"Courier\" Size=\"3\">\n\r");
  for ( d = descriptor_list; d; d = d->next )
  {
     if ( d->connected != CON_PLAYING ) 
        continue;

     if ( ( ch = d->character ) == NULL )
        continue;

     if ( ch->level > 6 )
        continue;

     total++;

     strcat(body_data, "<tr>\n\r");
     sprintf(buf, "<td>%s</td>\n\r", ch->name);
     strcat(body_data, buf);
     sprintf(buf, "<td>%s</td>\n\r", plr_class_name(ch,ch->clannum,ch->class));
     strcat(body_data, buf);
     sprintf(buf, "<td>%s</td>\n\r", plr_class_rank(ch->generation));
     strcat(body_data, buf);
     strcat(body_data, "</tr>\n\r");
  }

  strcat(body_data, "\n\r</table>\n\r");
  sprintf( buf, "<br><br><center><font face=\"Courier\" Size=\"3\"Total Number of Players: %d</center><br><br>\n\r", total);
  strcat(body_data, buf);
  sprintf(buf, "<center>Number of players connected since last copyover/restart      : %d</center><br>\n\r", players_logged);
  strcat(body_data, buf);
  sprintf(buf, "<center>Number of players decapitated since last copyover/restart    : %d</center><br><br>\n\r", players_decap);
  strcat(body_data, buf);

  strcat(body_data, "<center><font color=\"#CC77CC\">This page is updated every 60 seconds</font></center>" );

  /*
   * Write it all to the file, and return.
   */
  fprintf(fp, fadein_data);
  fprintf(fp, body_data);
  fprintf(fp, fadeout_data);

  fclose (fp);
}
