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
#include<types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_topxp(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *vch;
   CHAR_DATA *top[5];
   DESCRIPTOR_DATA *d;
   int i;
   bool showme;
   int players = 0;

   if ( IS_NPC(ch) )
      return;

   send_to_char("Online on #CUtopia: #RD.#eo.#wA#n:\n\r", ch);
   for ( i = 0; i < 5; i++ )
       top[i] = NULL;

   for ( i = 0; i < 5; i++ ) 
   {
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
         if ( d->connected != CON_PLAYING )
            continue;

         if ( ( vch = d->character ) == NULL )
            continue;

         if ( IS_IMMORTAL(vch) )
            continue;

         players++;

         if ( top[i] == NULL || vch->pcdata->score[SCORE_TOTAL_XP] > top[i]->pcdata->score[SCORE_TOTAL_XP] )
            {
              if ( top[0] == vch )
                 continue;
            
              if ( top[1] == vch )
                 continue;

              if ( top[2] == vch )
                 continue;

              if ( top[3] == vch )
                 continue;

              top[i] = vch;
            }
      }
   }

   players = UMIN(players, 5);

   for ( i = 0; i < players; i++ )
       if ( top[i] == NULL )
          top[i] = ch;

   for ( i = 0; i < players; i++ )
   {
      sprintf(buf, "  %d: #R%-12s#7 with #0%d#7 XP and #0%d#7 earnt QP.\n\r", i+1, top[i]->name,
                   top[i]->pcdata->score[SCORE_TOTAL_XP], top[i]->pcdata->questtotal);
      send_to_char(buf, ch);
   }

   showme = TRUE;
   for ( i = 0; i < players; i++ )
      if ( top[i] == ch )
         showme = FALSE;

   if ( showme )
      {
         sprintf(buf, "You: #R%-12s#7 with #0%d#7 XP and #0%d#7 earnt QP.\n\r", ch->name,
                      ch->pcdata->score[SCORE_TOTAL_XP], ch->pcdata->questtotal);
         send_to_char(buf, ch);
      }
}

void do_affects( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;

   if ( IS_NPC(ch) )
      return;

   if ( IS_ITEMAFF(ch, ITEMA_ICESHIELD) )
      send_to_char("#CIceshield#n.\n\r",ch);

   if ( IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD) )
      send_to_char("#LLightningshield#n.\n\r",ch);

   if ( IS_ITEMAFF(ch, ITEMA_ACIDSHIELD) )
      send_to_char("#GAcidshield#n.\n\r",ch);

   if ( IS_ITEMAFF(ch, ITEMA_FIRESHIELD) )
      send_to_char("#RFireshield#n.\n\r",ch);

   if ( IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD) )
      send_to_char("#pChaosshield#n.\n\r",ch);

   if ( IS_AFFECTED(ch, AFF_SANCTUARY) )
      send_to_char("#7Sanctuary#n.\n\r",ch);

   if ( IS_AFFECTED(ch, AFF_PROTECT) )
      send_to_char("#LProtection from evil#n.\n\r",ch);

   if ( IS_AFFECTED(ch, AFF_PROTECT_GOOD) )
      send_to_char("#LProtection from good#n.\n\r",ch);

   if ( IS_AFFECTED(ch, AFF_FLYING) )
      send_to_char("#cFly#n\n\r",ch);

   if ( IS_AFFECTED(ch, AFF_SNEAK) )
      send_to_char("#eSneaking#n\n\r",ch);

    send_to_char("\n\r",ch);

    if ( ch->affected == NULL )
       {			   
          send_to_char( "#wYou have #ynothing #waffecting you at this time.#n\n\r", ch);
          return;
       }

    if ( ch->affected != NULL )
       {
          send_to_char( "You are affected by:\n\r", ch );
          for ( paf = ch->affected; paf != NULL; paf = paf->next )
          {
              if ( paf->duration != -1 )
                 {
                     sprintf( buf, "#wSpell: #o'%s'", skill_table[paf->type].name );
                     send_to_char( buf, ch );

                     if ( str_cmp( "none", affect_loc_name( paf->location ) ) )
                        {
                            sprintf( buf, " #wgives %s #y%s%d #wfor #y%d #whours.#n\n\r",
                                          affect_loc_name( paf->location ),
                                          ( paf->modifier > -1 ) ? "+":"",
                                          paf->modifier,                    
                                          paf->duration );
                        }
                        else sprintf( buf, " #wfor #y%d #whours.#n\n\r", paf->duration );

                     send_to_char( buf, ch );
                  }
          }
       }
}

char * plr_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & PLR_IS_NPC       ) strcat( buf, " npc"           );
    if ( arg & PLR_AUTOEXIT     ) strcat( buf, " autoexit"      );
    if ( arg & PLR_AUTOLOOT     ) strcat( buf, " autoloot"      );
    if ( arg & PLR_AUTOSAC      ) strcat( buf, " autosac"       );
    if ( arg & PLR_BLANK        ) strcat( buf, " blank"         );
    if ( arg & PLR_BRIEF        ) strcat( buf, " brief"         );
    if ( arg & PLR_COMBINE      ) strcat( buf, " combine"       );
    if ( arg & PLR_PROMPT       ) strcat( buf, " prompt"        );
    if ( arg & PLR_TELNET_GA    ) strcat( buf, " telnet_ga"     );
    if ( arg & PLR_HOLYLIGHT    ) strcat( buf, " holylight"     );
    if ( arg & PLR_WIZINVIS     ) strcat( buf, " wizinvis"      );
    if ( arg & PLR_INCOG        ) strcat( buf, " incog"         );
    if ( arg & PLR_ANSI         ) strcat( buf, " ansi"          );
    if ( arg & PLR_SILENCE      ) strcat( buf, " silenced"      );
    if ( arg & PLR_NO_TELL      ) strcat( buf, " no_tell"       );
    if ( arg & PLR_LOG          ) strcat( buf, " log"           );
    if ( arg & PLR_FREEZE       ) strcat( buf, " freeze"        );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * extra_plr_bit_name( int arg )
{
    static char buf[512];
    
    buf[0] = '\0';
    
    if ( arg & EXTRA_TRUSTED            ) strcat( buf, " q_trusted" );
    if ( arg & EXTRA_NEWPASS            ) strcat( buf, " newpass" );
    if ( arg & EXTRA_OSWITCH            ) strcat( buf, " oswitch" );
    if ( arg & EXTRA_SWITCH             ) strcat( buf, " switch" );
    if ( arg & TIED_UP                  ) strcat( buf, " tied_up" );
    if ( arg & GAGGED                   ) strcat( buf, " gagged" );
    if ( arg & BLINDFOLDED              ) strcat( buf, " blindfolded" );
    if ( arg & EXTRA_DONE               ) strcat( buf, " non_virgin" );
    if ( arg & EXTRA_PREGNANT           ) strcat( buf, " pregnant" );
    if ( arg & EXTRA_LABOUR             ) strcat( buf, " labour" );
    if ( arg & EXTRA_BORN               ) strcat( buf, " born" );
    if ( arg & EXTRA_PROMPT             ) strcat( buf, " prompt" );
    if ( arg & EXTRA_MARRIED            ) strcat( buf, " married" );
    if ( arg & EXTRA_CALL_ALL           ) strcat( buf, " call_all" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * get_position_name( int arg )
{
    switch( arg )
    {
        case 0: return "dead";
        case 1: return "mortal";
        case 2: return "incap";
        case 3: return "stunned";
        case 4: return "sleeping";
        case 5: return "meditating";
        case 6: return "sitting";
        case 7: return "resting";
        case 8: return "fighting";
        case 9: return "standing";
    }
    bug( "Get_position_name: unknown type %d.", arg );
    return "(unknown)";
}

/*
 * Itemaffect bit names :)
 */
char * itemaffect_bit_name( int arg )
{
    static char buf[512];
    
    buf[0] = '\0';

    if ( arg & ITEMA_SHOCKSHIELD        ) strcat( buf, " Shockshield"  ); 
    if ( arg & ITEMA_FIRESHIELD         ) strcat( buf, " Fireshield"    );
    if ( arg & ITEMA_ICESHIELD          ) strcat( buf, " Iceshield"     );
    if ( arg & ITEMA_ACIDSHIELD         ) strcat( buf, " Acidshield"    );
    if ( arg & ITEMA_CHAOSSHIELD        ) strcat( buf, " Chaoshield"    );
    if ( arg & ITEMA_ARTIFACT           ) strcat( buf, " Artifact"      );
    if ( arg & ITEMA_REGENERATE         ) strcat( buf, " Regeneration"  );
    if ( arg & ITEMA_SPEED              ) strcat( buf, " Speed"         );
    if ( arg & ITEMA_VORPAL             ) strcat( buf, " Vorpal"        );
    if ( arg & ITEMA_PEACE              ) strcat( buf, " Peace"         );
    if ( arg & ITEMA_REFLECT            ) strcat( buf, " Darkshield"    );
    if ( arg & ITEMA_RESISTANCE         ) strcat( buf, " Resistance"    );
    if ( arg & ITEMA_VISION             ) strcat( buf, " Vision"        );
    if ( arg & ITEMA_STALKER            ) strcat( buf, " Stalker"       );
    if ( arg & ITEMA_VANISH             ) strcat( buf, " Vanish"        );
    if ( arg & ITEMA_RAGER              ) strcat( buf, " Rager"         );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

void do_pstat ( CHAR_DATA *ch, char *argument )
{
    char        arg[MAX_INPUT_LENGTH];
    char        buf[MAX_STRING_LENGTH];
    CHAR_DATA   *victim;
        
    argument = one_argument( argument, arg );
  
    if ( arg[0] == '\0' )
    {
        send_to_char("Pstat whom?\n\r", ch );
        return;
    }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
       send_to_char("They aren't here.\n\r", ch );
        return;
    }
    
    sprintf( buf, "Name : %s.\n\r",
        IS_NPC( victim )        ? victim->short_descr : victim->name );
  send_to_char( buf, ch );
    sprintf( buf, "Sex : %s. Room : %d. Align : %d. Primal : %d. Quest : %d.\n\r",
        victim->sex == SEX_MALE         ? "Male"        :
      victim->sex == SEX_FEMALE       ? "Female"      : "None",
        victim->in_room == NULL         ? 0             : victim->in_room->vnum,
        victim->alignment,
        victim->practice,
        IS_NPC( victim )                ? 0             : victim->pcdata->quest
        );
    send_to_char( buf, ch );
        
    sprintf( buf, "Level : %d. Trust : %d. Gold : %d. Exp : %d.\n\r",
        victim->level,
        victim->trust,
        victim->gold,
        victim->exp );
   send_to_char( buf, ch );
    
       sprintf( buf, "Hit : %d. Dam : %d. AC : %d. Position : %s\n\r",
       char_hitroll( victim ),
        char_damroll( victim ),
        char_ac( victim ),
        capitalize( get_position_name( victim->position ) ));
    send_to_char( buf, ch );
         
    sprintf( buf, "HP %d/%d. Mana %d/%d. Move %d/%d.\n\r",
        victim->hit, victim->max_hit,
        victim->mana, victim->max_mana,
        victim->move, victim->max_move );
    send_to_char( buf, ch );
        
    sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
      get_curr_str(victim),
        get_curr_int(victim),
        get_curr_wis(victim),
        get_curr_dex(victim), 
        get_curr_con(victim) );
    send_to_char( buf, ch );
        
    sprintf( buf, "Fighting : %s. (%d)\n\r",
        victim->fighting ? victim->fighting->name  : "(None)",
        victim->fighting ? victim->fighting->level : 0 ); 
   send_to_char( buf, ch );
        
    sprintf( buf, "Pkill : %d. Pdeath : %d. Mkill : %d. Mdeath : %d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pkill,
        IS_NPC( victim ) ? 0 : victim->pdeath,
        IS_NPC( victim ) ? 0 : victim->mkill,
       IS_NPC( victim ) ? 0 : victim->mdeath );
    send_to_char( buf, ch ); 
        
    sprintf( buf, "TotExp  : %12d. TotMobLev  : %10d. TotQuestPoints : %10d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_QUEST] );
    send_to_char( buf, ch );
        
    sprintf( buf, "HighExp : %12d. HighMobLev : %10d. Tot#Quests     : %10d.\n\r",
      IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_NUM_QUEST] );  
    send_to_char( buf, ch );
        
    if ( !IS_NPC( victim ) )
   {
        sprintf( buf, "Unarmed : %4d.", victim->wpn[0] );
        send_to_char( buf, ch );
        sprintf( buf, " Slice   : %4d.", victim->wpn[1] );
        send_to_char( buf, ch );
        sprintf( buf, " Stab    : %4d.", victim->wpn[2] );
        send_to_char( buf, ch );
        sprintf( buf, " Slash   : %4d.", victim->wpn[3] );
        send_to_char( buf, ch );
        sprintf( buf, " Whip    : %4d.\n\r", victim->wpn[4] );
        send_to_char( buf, ch );
        sprintf( buf, "Claw    : %4d.", victim->wpn[5] );
        send_to_char( buf, ch );
        sprintf( buf, " Blast   : %4d.", victim->wpn[6] );
        send_to_char( buf, ch );
        sprintf( buf, " Pound   : %4d.", victim->wpn[7] );
send_to_char( buf, ch );
        sprintf( buf, " Crush   : %4d.", victim->wpn[8] );
        send_to_char( buf, ch );
        sprintf( buf, " Grep    : %4d.\n\r", victim->wpn[9] );
        send_to_char( buf, ch );
        sprintf( buf, "Bite    : %4d.", victim->wpn[10] );
     send_to_char( buf, ch );   
        sprintf( buf, " Pierce  : %4d.", victim->wpn[11] );
        send_to_char( buf, ch );
        sprintf( buf, " Suck    : %4d.\n\r",victim->wpn[12] );
        send_to_char( buf, ch );
        
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
            "Purple",   victim->spl[PURPLE_MAGIC],
            "Red",      victim->spl[RED_MAGIC],
            "Blue",     victim->spl[BLUE_MAGIC],
           "Green",    victim->spl[GREEN_MAGIC],
            "Yellow",   victim->spl[YELLOW_MAGIC] );
        send_to_char( buf, ch );
        
	 sprintf( buf, "Act         : %s\n\r", plr_bit_name(victim->act ));
    	send_to_char( buf, ch );
	sprintf( buf, "Extra       : %s\n\r",
        victim->extra <= 0 ? "(None)" : extra_plr_bit_name( victim->extra ) );
    	send_to_char( buf, ch );
}       
	sprintf( buf, "ItemAff     : %s\n\r",
        victim->itemaffect <= 0 ? "(None)" : itemaffect_bit_name(victim->itemaffect ) );
    	send_to_char( buf, ch );
           
              sprintf( buf, "Affected by : %s.\n\r",  
        affect_bit_name( victim->affected_by ) );
    	send_to_char( buf, ch );
}

