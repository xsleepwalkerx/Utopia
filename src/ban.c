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

/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/*************************************************************************** 
*       ROT 1.4 is copyright 1996-1997 by Russ Walsh                       * 
*       By using this code, you have agreed to follow the terms of the     * 
*       ROT license, in the file doc/rot.license                           * 
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"

BAN_DATA *ban_list;

/* stuff for recycling ban structures */
BAN_DATA *ban_free;

BAN_DATA *new_ban(void)
{
    static BAN_DATA ban_zero;
    BAN_DATA *ban;

    if (ban_free == NULL)
	ban = alloc_perm(sizeof(*ban));
    else
    {
	ban = ban_free;
	ban_free = ban_free->next;
    }

    *ban = ban_zero;
    ban->name = &str_empty[0];
    return ban;
}

void free_ban(BAN_DATA *ban)
{
    free_string(ban->name);

    ban->next = ban_free;
    ban_free = ban;
}

void save_bans(void)
{
    BAN_DATA *pban;
    FILE *fp;
    bool found = FALSE;

    fclose( fpReserve ); 
    if ( ( fp = fopen( BAN_FILE, "w" ) ) == NULL )
    {
        perror( BAN_FILE );
    }

    for (pban = ban_list; pban != NULL; pban = pban->next)
    {
	if (IS_SET(pban->ban_flags,BAN_PERMANENT))
	{
	    found = TRUE;
	    fprintf(fp,"%-20s %-2d %d\n",pban->name,pban->level,
		pban->ban_flags);
	}
     }

     fclose(fp);
     fpReserve = fopen( NULL_FILE, "r" );
     if (!found)
	unlink(BAN_FILE);
}

void load_bans(void)
{
    FILE *fp;
    BAN_DATA *ban_last;

    if ( ( fp = fopen( BAN_FILE, "r" ) ) == NULL )
        return;
 
    ban_last = NULL;
    for ( ; ; )
    {
        BAN_DATA *pban;
        if ( feof(fp) )
        {
            fclose( fp );
            return;
        }
 
        pban = new_ban();
 
        pban->name = str_dup(fread_word(fp));
	pban->level = fread_number(fp);
	pban->ban_flags = fread_number(fp);
	fread_to_eol(fp);

        if (ban_list == NULL)
	    ban_list = pban;
	else
	    ban_last->next = pban;
	ban_last = pban;
    }
}

bool check_ban(char *site,int type)
{
    BAN_DATA *pban;
    char host[MAX_STRING_LENGTH];

    strcpy(host,capitalize(site));
    host[0] = LOWER(host[0]);

    for ( pban = ban_list; pban != NULL; pban = pban->next ) 
    {
	if(!IS_SET(pban->ban_flags,type))
	    continue;

	if (IS_SET(pban->ban_flags,BAN_PREFIX) 
	&&  IS_SET(pban->ban_flags,BAN_SUFFIX)  
	&&  strstr(pban->name,host) != NULL)
	    return TRUE;

	if (IS_SET(pban->ban_flags,BAN_PREFIX)
	&&  !str_suffix(pban->name,host))
	    return TRUE;

	if (IS_SET(pban->ban_flags,BAN_SUFFIX)
	&&  !str_prefix(pban->name,host))
	    return TRUE;

	if ( (strstr(pban->name,host) != NULL)
	||  !str_suffix(pban->name,host)
	||  !str_prefix(pban->name,host) )
	    return TRUE;

	if ( (strstr(pban->name,host) != NULL)
	||  !str_suffix(pban->name,host)
	||  !str_prefix(pban->name,host) )
            return TRUE;

    }

    return FALSE;
}

bool check_adr(char *site,int type)
{
    char bans[MAX_STRING_LENGTH];
    char host[MAX_STRING_LENGTH];

    strcpy(host,capitalize(site));
    host[0] = LOWER(host[0]);

    if ( (strstr(bans,host) != NULL)
    ||  !str_suffix(bans,host)
    ||  !str_prefix(bans,host) )
	    return TRUE;

    if ( (strstr(bans,host) != NULL)
    ||  !str_suffix(bans,host)
    ||  !str_prefix(bans,host) )
	    return TRUE;

    return FALSE;
}


void ban_site(CHAR_DATA *ch, char *argument, bool fPerm)
{
    char buf[MAX_STRING_LENGTH],buf2[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
    char *name;
    BAN_DATA *pban, *prev;
    bool prefix = FALSE,suffix = FALSE;
    int type;

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' )
    {
	if (ban_list == NULL)
	{
	    send_to_char("No sites banned at this time.\n\r",ch);
	    return;
  	}

        sprintf(buf, "Banned sites  level  type     status\n\r" );

        for (pban = ban_list;pban != NULL;pban = pban->next)
        {
	    sprintf(buf2, "%-12s    %-3d  %-7s  %s\n\r",
            pban->name, pban->level,
		IS_SET(pban->ban_flags,BAN_NEWBIES) ? "newbies" :
		IS_SET(pban->ban_flags,BAN_PERMIT)  ? "permit"  :
		IS_SET(pban->ban_flags,BAN_ALL)     ? "all"	: "",
	    	IS_SET(pban->ban_flags,BAN_PERMANENT) ? "perm" : "temp");

          strcat(buf, buf2);
        }

        stc(buf, ch);
        return;
    }

    /* find out what type of ban */
    if (arg2[0] == '\0' || !str_prefix(arg2,"all"))
	type = BAN_ALL;
    else if (!str_prefix(arg2,"newbies"))
	type = BAN_NEWBIES;
    else if (!str_prefix(arg2,"permit"))
	type = BAN_PERMIT;
    else
    {
	send_to_char("Acceptable ban types are all, newbies, and permit.\n\r", ch); 
	return;
    }

    name = arg1;

    if (name[0] == '*')
    {
	prefix = TRUE;
	name++;
    }

    if (name[strlen(name) - 1] == '*')
    {
	suffix = TRUE;
	name[strlen(name) - 1] = '\0';
    }

    if (strlen(name) == 0)
    {
	send_to_char("You have to ban SOMETHING.\n\r",ch);
	return;
    }

    prev = NULL;
    for ( pban = ban_list; pban != NULL; prev = pban, pban = pban->next )
    {
        if (!str_cmp(name,pban->name))
        {
	    if ( pban->level > ch->level )
	    {
            	send_to_char( "That ban was set by a higher power.\n\r", ch );
            	return;
	    }
	    else
	    {
		if (prev == NULL)
		    ban_list = pban->next;
		else
		    prev->next = pban->next;
		free_ban(pban);
	    }
        }
    }

    pban = new_ban();
    pban->name = str_dup(name);
    pban->level = get_trust(ch);

    /* set ban type */
    pban->ban_flags = type;

    if (prefix)
	SET_BIT(pban->ban_flags,BAN_PREFIX);
    if (suffix)
	SET_BIT(pban->ban_flags,BAN_SUFFIX);
    if (fPerm)
	SET_BIT(pban->ban_flags,BAN_PERMANENT);

    pban->next  = ban_list;
    ban_list    = pban;
    save_bans();
    sprintf(buf,"%s has been banned.\n\r",pban->name);
    send_to_char( buf, ch );
    return;
}

void do_ban(CHAR_DATA *ch, char *argument)
{
    ban_site(ch,argument,FALSE);
}

void do_permban(CHAR_DATA *ch, char *argument)
{
    ban_site(ch,argument,TRUE);
}

void do_allow( CHAR_DATA *ch, char *argument )                        
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    BAN_DATA *prev;
    BAN_DATA *curr;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Remove which site from the ban list?\n\r", ch );
        return;
    }

    prev = NULL;
    for ( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
    {
        if ( !str_cmp( arg, curr->name ) )
        {
	    if ( curr->level > ch->level )
	    {
		send_to_char(
		   "You are not powerful enough to lift that ban.\n\r",ch);
		return;
	    }
            if ( prev == NULL )
                ban_list   = ban_list->next;
            else
                prev->next = curr->next;

            free_ban(curr);
	    sprintf(buf,"Ban on %s lifted.\n\r",arg);
            send_to_char( buf, ch );
	    save_bans();
            return;
        }
    }

    send_to_char( "Site is not banned.\n\r", ch );
    return;
}

void do_multicheck(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  DESCRIPTOR_DATA *d2;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
      
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->connected != CON_PLAYING) continue;  
    if ( d->character == NULL || IS_IMMORTAL(d->character) ) continue;
    for ( d2 = d->next; d2 != NULL; d2 = d2->next )
    {
      if (!str_cmp(d->host, d2->host))
      {
        if (d2->connected != CON_PLAYING) continue;
        if (d2->character == NULL || d->character == NULL || IS_IMMORTAL(d2->character)) continue;
        found = TRUE;
        sprintf(buf, "%s and %s are multiplaying bitches\n\r",d2->character->name, d->character->name);
        send_to_char(buf,ch);
      }
    }
  }
 
  if (!found) send_to_char("Noone is multiplaying atm.\n\r",ch);
}
