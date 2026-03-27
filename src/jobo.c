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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

int char_damroll(CHAR_DATA *ch)
{
   int value = 0;

   if ( IS_NPC( ch ) )
      value = ch->damroll;
      else value = char_damcap( ch );

   return value;
}

int char_hitroll(CHAR_DATA *ch)
{
   int value = 0;

   if ( IS_NPC( ch ) )
      value = ch->hitroll;
      else value = char_damcap( ch );

   return value;
}

int char_ac(CHAR_DATA *ch)
{
   int value = 0;

   value = GET_AC( ch );

   if ( IS_NPC(ch) )
      {
         if ( ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
            return -100000;
            else return value;
      }

   return value;
}

bool is_martial( CHAR_DATA *ch )
{
   if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
        && IS_SET( ch->arena, AFF2_INARENA ) )
      {
         int team;

         if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
            team = 0;
            else team = 1;

         if ( team_list->power_mode[team] == 3 )
            return TRUE;
      }

   if ( ( IS_CLASS( ch, CLASS_ASSASSIN ) && ( IS_SET( ch->special, SPC_KNIGHT )
        || IS_SET( ch->special, SPC_MONK ) ) )
        || ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum < 571 )
        || ( IS_CLASS( ch, CLASS_MONK ) && IS_CLASS( ch, CLASS_NINJA ) )
        || ( IS_CLASS( ch, CLASS_SHADOW ) && ch->gifts[CURRENT_METER] == ANI ) )
      return TRUE;

   return FALSE;
}

void do_clearstats2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    if (IS_NPC(ch)) return;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        if ( obj->wear_loc != WEAR_NONE )
        {obj_from_char(obj); obj_to_char(obj,ch);}
    }

    while ( ch->affected )
      affect_remove( ch, ch->affected );

    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    if (IS_SET(ch->affected_by, AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

    ch->affected_by      = 0;
    ch->armor            = 100;
    ch->hit              = UMAX( 1, ch->hit  );
    ch->mana             = UMAX( 1, ch->mana );
    ch->move             = UMAX( 1, ch->move );
    ch->hitroll          = 0;
    ch->damroll          = 0;
    ch->saving_throw     = 0;
    ch->pcdata->mod_str  = 0;
    ch->pcdata->mod_int  = 0;
    ch->pcdata->mod_wis  = 0;
    ch->pcdata->mod_dex  = 0;
    ch->pcdata->mod_con  = 0;
    if (IS_SET(ch->newbits, NEW_DFORM)) REMOVE_BIT(ch->newbits, NEW_DFORM);
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

/*
 *  If an item have a bad char in it's short/long/name desc, the mud
 *  will freeze when the char with that item tries to log on, so use
 *  this check on any renaming of items (glamour/reshape/etc).
 */
bool has_bad_chars(CHAR_DATA *ch, char *argument)
{
  int i;

  if ( argument[0] == '\0' ) return FALSE;
  for (i = 0; argument[i] != '\0' ; i++)
  {
    if (argument[i] == '~') return TRUE;
  }
  return FALSE;
}

void do_bountylist(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

  stc("#r--==#L**#r==--==#L**#r==      #RBOUNTY LIST    #r==#L**#r==--==#L**#r==--#n\n\r",ch);
  stc("  #CName               Bounty      Level   Gen\n\r#n",ch);
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character != NULL )
    {
       if (!d->connected == CON_PLAYING) continue;
       if (d->character->level > 6) continue;
       if (!can_see(ch, d->character)) continue;
       sprintf(buf, "  #w%-15s#n #R%9d    #g%5d      #y%d#n\n\r",d->character->name, d->character->pcdata->bounty,
                    d->character->tier, d->character->generation);
       stc(buf,ch);
    }
  }
  stc("#r--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#L**#r==--==#L**#r==--==#L**#r==--#n\n\r",ch);
}

void do_version( CHAR_DATA *ch, char *argument) 
{
  send_to_char("#y*************************************************#n\n\r",ch);
  send_to_char("#y*        #RThis mud runs Utopia V.O.D             #y*#n\n\r",ch);
  send_to_char("#y*                                               *#n\n\r",ch);
  send_to_char("#y* #GThe Dystopian codebase version 1.0.1 can be   #y*#n\n\r",ch);
  send_to_char("#y* #G           downloaded from :                  #y*#n\n\r",ch);
  send_to_char("#y* #G   http://www.daimi.au.dk/~jobo/dystopia/     #y*#n\n\r",ch);
  send_to_char("#y*                                               *#n\n\r",ch);
  send_to_char("#y* #G         The Utopia codebase can be           #y*#n\n\r",ch);
  send_to_char("#y* #G             downloaded from :                #y*#n\n\r",ch);
  send_to_char("#y* #G   http://www.geocities.com/hutopia2001/      #y*#n\n\r",ch);
  send_to_char("#y*************************************************#n\n\r",ch);
  return;
}

/* The costy healer */
void do_healme(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL && ch->in_room->vnum != ROOM_VNUM_ALTAR)
  {
    send_to_char("The healer is located at the alter in midgaard.\n\r",ch);
    return;
  }
  if (ch->fight_timer > 0)
  {
    send_to_char("Not with a fighttimer.\n\r",ch);
    return;
  }

  if (ch->pcdata->quest < 3000)
  {
    send_to_char("The healer demands 3000 qps as payment for his service.\n\r",ch);
    return;
  }
  ch->hit  = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;
  ch->loc_hp[0] = 0;
  ch->loc_hp[1] = 0;
  ch->loc_hp[2] = 0;
  ch->loc_hp[3] = 0;
  ch->loc_hp[4] = 0;
  ch->loc_hp[5] = 0;
  ch->loc_hp[6] = 0;
  send_to_char("The healer heals your wounds and takes 3000 qps as payment.\n\r",ch);
  ch->pcdata->quest -= 3000;
  WAIT_STATE(ch, 36);
  return;
}

void do_class_count( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  FILE *fp;
  int mage_count=0, drow_count=0, ww_count=0, monk_count=0, angel_count=0, dragon_count=0, dragoon_count =0, samurai_count =0;
  int vampire_count=0, ninja_count=0, jedi_count=0, demon_count=0, paladin_count=0, sorceror_count=0, amazon_count=0;
  int shadow_count=0, hunter_count=0, dknight_count=0, daywalker_count=0, undead_count=0, saiyan_count=0;
  int summoner_count=0, wizard_count=0, necro_count=0, dara_count=0, assassin_count=0, blade_count=0, divine_count=0;
  int kodo_count=0, kuruda_count=0, shaolin_count=0, guyver_count=0, garou_count=0, reaver_count=0;
  int elf_count=0, dragooon_count=0, fiend_count=0, hobbit_count=0;
  int snow_count=0, phoenix_count=0, duergar_count=0, wisp_count=0, sdwr_count=0;

  if ( IS_NPC(ch) ) return;

  ch->wait += 12;

  if ( ( fp = fopen("class.dat", "r") ) == NULL )
     {
        bug("There isn't a class.dat file",0);
        return;
     }

  demon_count = fread_number(fp);
  mage_count = fread_number(fp);
  ww_count = fread_number(fp);
  vampire_count = fread_number(fp);
  jedi_count = fread_number(fp);
  drow_count = fread_number(fp);
  monk_count = fread_number(fp);
  ninja_count = fread_number(fp);
  angel_count = fread_number(fp);
  dragoon_count = fread_number(fp);
  dragon_count = fread_number(fp);
  paladin_count = fread_number(fp);
  amazon_count = fread_number(fp);
  sorceror_count = fread_number(fp);
  shadow_count = fread_number(fp);
  hunter_count = fread_number(fp);
  dknight_count = fread_number(fp);
  daywalker_count = fread_number(fp);
  undead_count = fread_number(fp);
  saiyan_count = fread_number(fp);
  samurai_count = fread_number(fp);
  summoner_count = fread_number(fp);
  wizard_count = fread_number(fp);
  necro_count = fread_number(fp);
  dara_count = fread_number(fp);
  assassin_count = fread_number(fp);
  blade_count = fread_number(fp);
  divine_count = fread_number(fp);
  kodo_count = fread_number(fp);
  shaolin_count = fread_number(fp);
  kuruda_count = fread_number(fp);
  guyver_count = fread_number(fp);
  garou_count = fread_number(fp);
  reaver_count = fread_number(fp);
  elf_count = fread_number(fp);
  dragooon_count = fread_number(fp);
  fiend_count = fread_number(fp);
  hobbit_count = fread_number(fp);
  snow_count = fread_number(fp);
  duergar_count = fread_number(fp);
  phoenix_count = fread_number(fp);
  wisp_count = fread_number(fp);
  sdwr_count = fread_number(fp);

  fclose(fp);

  send_to_char("#b--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b== #R[#y^^#R] CLASS COUNT [#y^^#R]#b ==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--#n\n\r\n\r",ch);
  send_to_char("#LTotal number of each Class :#n\n\r\n\r",ch);
  sprintf(buf, "#pDragons#n         : %-3d      #wPaladins#n        : %-3d       #eNinjas#n      : %-3d\n\r",
    dragon_count, paladin_count, ninja_count);
  send_to_char(buf,ch);
  sprintf(buf, "#wMonks#n           : %-3d      #yWizards#n         : %-3d       #RVampires#n    : %-3d\n\r",
    monk_count, wizard_count, vampire_count);
  send_to_char(buf,ch);
  sprintf(buf, "#rDemons#n          : %-3d      #yWerewolves#n      : %-3d       #eDrows#n       : %-3d\n\r",
    demon_count, ww_count, drow_count);
  send_to_char(buf,ch);
  sprintf(buf, "#wAngels#n          : %-3d      #eClones#n          : %-3d       #cHighwinds#n   : %-3d\n\r",
    angel_count, assassin_count, dragoon_count);
  send_to_char(buf,ch);
  sprintf(buf, "#eNecromancers#n    : %-3d      #RUndead Knights#n  : %-3d       #eCommanders#n  : %-3d\n\r",
    necro_count, undead_count, kuruda_count);
  send_to_char(buf,ch);
  sprintf(buf, "#CSamurais#n        : %-3d      #eAssassins#n       : %-3d       #RSaiyans#n     : %-3d\n\r",
    samurai_count, shadow_count, saiyan_count);
  send_to_char(buf,ch);
  sprintf(buf, "#wHobbits#n         : %-3d      #CSnow Elves#n      : %-3d       #gDuergars#n    : %-3d\n\r",
    hobbit_count, snow_count, duergar_count);
  send_to_char(buf,ch);
  sprintf(buf, "#eShadow Warriors#n : %-3d      #yWisps#n           : %-3d       #RPhoenixes#n   : %-3d\n\r",
    sdwr_count, wisp_count, phoenix_count);
  send_to_char(buf,ch);
  sprintf(buf, "#PDaramons#n        : %-3d      #RDaywalkers#n      : %-3d       #PDragoons#n    : %-3d\n\r",
	dara_count, daywalker_count, dragooon_count);
  send_to_char(buf,ch);
  sprintf(buf, "#RFiends#n          : %-3d      #PBlade Lords#n     : %-3d\n\r",
	fiend_count, guyver_count);
  send_to_char(buf,ch);

  send_to_char("#b--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==--#n\n\r\n\r",ch);
}

void do_update_class_count( int class, bool add )
{
  FILE *fp;
  int mage_count=0, drow_count=0, ww_count=0, monk_count=0, angel_count=0, dragon_count=0, dragoon_count =0, samurai_count =0;
  int vampire_count=0, ninja_count=0, jedi_count=0, demon_count=0, paladin_count=0, sorceror_count=0, amazon_count=0;
  int shadow_count=0, hunter_count=0, dknight_count=0, daywalker_count=0, undead_count=0, saiyan_count=0;
  int summoner_count=0, wizard_count=0, necro_count=0, dara_count=0, assassin_count=0, blade_count=0, divine_count=0;
  int kodo_count=0, kuruda_count=0, shaolin_count=0, guyver_count=0, garou_count=0, reaver_count=0;
  int elf_count=0, dragooon_count=0, fiend_count=0, hobbit_count=0;
  int snow_count=0, phoenix_count=0, duergar_count=0, wisp_count=0, sdwr_count=0;
  int adder = 1;

  if ( ( fp = fopen("class.dat", "r") ) == NULL )
     {
         bug("There isn't a class.dat file",0);
         return;
     }

  demon_count = fread_number(fp);
  mage_count = fread_number(fp);
  ww_count = fread_number(fp);
  vampire_count = fread_number(fp);
  jedi_count = fread_number(fp);
  drow_count = fread_number(fp);
  monk_count = fread_number(fp);
  ninja_count = fread_number(fp);
  angel_count = fread_number(fp);
  dragoon_count = fread_number(fp);
  dragon_count = fread_number(fp);
  paladin_count = fread_number(fp);
  amazon_count = fread_number(fp);
  sorceror_count = fread_number(fp);
  shadow_count = fread_number(fp);
  hunter_count = fread_number(fp);
  dknight_count = fread_number(fp);
  daywalker_count = fread_number(fp);
  undead_count = fread_number(fp);
  saiyan_count = fread_number(fp);
  samurai_count = fread_number(fp);
  summoner_count = fread_number(fp);
  wizard_count = fread_number(fp);
  necro_count = fread_number(fp);
  dara_count = fread_number(fp);
  assassin_count = fread_number(fp);
  blade_count = fread_number(fp);
  divine_count = fread_number(fp);
  kodo_count = fread_number(fp);
  shaolin_count = fread_number(fp);
  kuruda_count = fread_number(fp);
  guyver_count = fread_number(fp);
  garou_count = fread_number(fp);
  reaver_count = fread_number(fp);
  elf_count = fread_number(fp);
  dragooon_count = fread_number(fp);
  fiend_count = fread_number(fp);
  hobbit_count = fread_number(fp);
  snow_count = fread_number(fp);
  duergar_count = fread_number(fp);
  phoenix_count = fread_number(fp);
  wisp_count = fread_number(fp);
  sdwr_count = fread_number(fp);

  fclose(fp);

  if ( !add )
     adder = -1;

  switch( class )
  {
     case 0:
     default: return;
     case CLASS_MONK:          monk_count += adder; break;
     case CLASS_DEMON:         demon_count += adder; break;
     case CLASS_NINJA:         ninja_count += adder; break;
     case CLASS_DROW:          drow_count += adder; break;
     case CLASS_ANGEL:         angel_count += adder; break;
     case CLASS_VAMPIRE:       vampire_count += adder; break;
     case CLASS_WEREWOLF:      ww_count += adder; break;
     case CLASS_DRAGON:        dragon_count += adder; break;
     case CLASS_DRAGOON:       dragoon_count += adder; break;
     case CLASS_HIGHLANDER:    jedi_count += adder; break;
     case CLASS_MAGE:          mage_count += adder; break;
     case CLASS_PALADIN:       paladin_count += adder; break;
     case CLASS_AMAZON:        amazon_count += adder; break;
     case CLASS_SORCEROR:      sorceror_count += adder; break;
     case CLASS_SHADOW:        shadow_count += adder; break;
     case CLASS_HUNTER:        hunter_count += adder; break;
     case CLASS_DARK_KNIGHT:   dknight_count += adder; break;
     case CLASS_DAYWALKER:     daywalker_count += adder; break;
     case CLASS_UNDEAD_KNIGHT: undead_count += adder; break;
     case CLASS_SAIYAN:        saiyan_count += adder; break;
     case CLASS_SAMURAI:       samurai_count += adder; break;
     case CLASS_SUMMONER:      summoner_count += adder; break;
     case CLASS_WIZARD:        wizard_count += adder; break;
     case CLASS_NECROMANCER:   necro_count += adder; break;
     case CLASS_DARAMON:       dara_count += adder; break;
     case CLASS_ASSASSIN:      assassin_count += adder; break;
     case CLASS_NIGHT_BLADE:   blade_count += adder; break;
     case CLASS_DIVINE_KNIGHT: divine_count += adder; break;
     case CLASS_KODO:          kodo_count += adder; break;
     case CLASS_KURUDA:        kuruda_count += adder; break;
     case CLASS_SHAOLIN:       shaolin_count += adder; break;
     case CLASS_MASTER:        guyver_count += adder; break;
     case CLASS_GAROU:         garou_count += adder; break;
     case CLASS_SOULREAVER:    reaver_count += adder;break;
     case CLASS_FIEND:         fiend_count += adder;break;
     case CLASS_HOBBIT:        hobbit_count += adder;break;
     case CLASS_DARK_ELF:      elf_count += adder;break;
     case CLASS_DRAGON_KNIGHT: dragooon_count += adder;break;
     case CLASS_SNOW_ELF:      snow_count += adder;break;
     case CLASS_WISP:          wisp_count += adder;break;
     case CLASS_PHOENIX:       phoenix_count += adder;break;
     case CLASS_DUERGAR:       duergar_count += adder;break;
     case CLASS_SHADOW_WAR:    sdwr_count += adder;break;
  }

  if ( ( fp = fopen("class.dat","w") ) == NULL )
     {
        log_string("Error writing to kingdom.dat");
        return;
     }

  fprintf( fp, "%d\n", demon_count );
  fprintf( fp, "%d\n", mage_count );
  fprintf( fp, "%d\n", ww_count );
  fprintf( fp, "%d\n", vampire_count );
  fprintf( fp, "%d\n", jedi_count );  
  fprintf( fp, "%d\n", drow_count );
  fprintf( fp, "%d\n", monk_count );
  fprintf( fp, "%d\n", ninja_count );
  fprintf( fp, "%d\n", angel_count );
  fprintf( fp, "%d\n", dragoon_count );
  fprintf( fp, "%d\n", dragon_count );
  fprintf( fp, "%d\n", paladin_count );
  fprintf( fp, "%d\n", amazon_count );
  fprintf( fp, "%d\n", sorceror_count );
  fprintf( fp, "%d\n", shadow_count );
  fprintf( fp, "%d\n", hunter_count );
  fprintf( fp, "%d\n", dknight_count );
  fprintf( fp, "%d\n", daywalker_count );
  fprintf( fp, "%d\n", undead_count );
  fprintf( fp, "%d\n", saiyan_count );
  fprintf( fp, "%d\n", samurai_count );
  fprintf( fp, "%d\n", summoner_count );
  fprintf( fp, "%d\n", wizard_count );
  fprintf( fp, "%d\n", necro_count );
  fprintf( fp, "%d\n", dara_count );
  fprintf( fp, "%d\n", assassin_count );
  fprintf( fp, "%d\n", blade_count );
  fprintf( fp, "%d\n", divine_count );
  fprintf( fp, "%d\n", kodo_count );
  fprintf( fp, "%d\n", shaolin_count );
  fprintf( fp, "%d\n", kuruda_count );
  fprintf( fp, "%d\n", guyver_count );
  fprintf( fp, "%d\n", garou_count );
  fprintf( fp, "%d\n", reaver_count );
  fprintf( fp, "%d\n", elf_count );
  fprintf( fp, "%d\n", dragooon_count );
  fprintf( fp, "%d\n", fiend_count );
  fprintf( fp, "%d\n", hobbit_count );
  fprintf( fp, "%d\n", snow_count );
  fprintf( fp, "%d\n", duergar_count );
  fprintf( fp, "%d\n", phoenix_count );
  fprintf( fp, "%d\n", wisp_count );
  fprintf( fp, "%d\n", sdwr_count );
  fclose(fp);
}

int check_class_max( CHAR_DATA *ch, int number )
{
  FILE *fp;
  int mage_count=0, drow_count=0, ww_count=0, monk_count=0, angel_count=0, dragon_count=0, dragoon_count =0, samurai_count =0;
  int vampire_count=0, ninja_count=0, jedi_count=0, demon_count=0, paladin_count=0, sorceror_count=0, amazon_count=0;
  int shadow_count=0, hunter_count=0, dknight_count=0, daywalker_count=0, undead_count=0, saiyan_count=0;
  int summoner_count=0, wizard_count=0, necro_count=0, dara_count=0, assassin_count=0, blade_count=0, divine_count=0;
  int kodo_count=0, kuruda_count=0, shaolin_count=0, guyver_count=0, garou_count=0, reaver_count=0;
  int elf_count=0, dragooon_count=0, fiend_count=0, hobbit_count=0;
  int snow_count=0, phoenix_count=0, duergar_count=0, wisp_count=0, sdwr_count=0;
  int counter = 0;
  int class;

  if ( IS_NPC(ch) ) return -1;

  if ( ( fp = fopen("class.dat", "r") ) == NULL )
     {
         bug("There isn't a class.dat file",0);
         return -1;
     }

  demon_count = fread_number(fp);
  mage_count = fread_number(fp);
  ww_count = fread_number(fp);
  vampire_count = fread_number(fp);
  jedi_count = fread_number(fp);
  drow_count = fread_number(fp);
  monk_count = fread_number(fp);
  ninja_count = fread_number(fp);
  angel_count = fread_number(fp);
  dragoon_count = fread_number(fp);
  dragon_count = fread_number(fp);
  paladin_count = fread_number(fp);
  amazon_count = fread_number(fp);
  sorceror_count = fread_number(fp);
  shadow_count = fread_number(fp);
  hunter_count = fread_number(fp);
  dknight_count = fread_number(fp);
  daywalker_count = fread_number(fp);
  undead_count = fread_number(fp);
  saiyan_count = fread_number(fp);
  samurai_count = fread_number(fp);
  summoner_count = fread_number(fp);
  wizard_count = fread_number(fp);
  necro_count = fread_number(fp);
  dara_count = fread_number(fp);
  assassin_count = fread_number(fp);
  blade_count = fread_number(fp);
  divine_count = fread_number(fp);
  kodo_count = fread_number(fp);
  shaolin_count = fread_number(fp);
  kuruda_count = fread_number(fp);
  guyver_count = fread_number(fp);
  garou_count = fread_number(fp);
  reaver_count = fread_number(fp);
  elf_count = fread_number(fp);
  dragooon_count = fread_number(fp);
  fiend_count = fread_number(fp);
  hobbit_count = fread_number(fp);
  snow_count = fread_number(fp);
  duergar_count = fread_number(fp);
  phoenix_count = fread_number(fp);
  wisp_count = fread_number(fp);
  sdwr_count = fread_number(fp);

  fclose(fp);

  if ( number == 1 )
     class = ch->class;
     else if ( number == 2 )
             class = ch->class_2;
     else if ( number == 3 )
             class = ch->class_3;
     else if ( number == 4 )
             class = ch->class_4;
             else class = ch->class_5;

  switch( class )
  {
     case 0:
     default: return -1;
     case CLASS_MONK:          counter = monk_count; break;
     case CLASS_DEMON:         counter = demon_count; break;
     case CLASS_NINJA:         counter = ninja_count; break;
     case CLASS_DROW:          counter = drow_count; break;
     case CLASS_ANGEL:         counter = angel_count; break;

     case CLASS_VAMPIRE:       counter = vampire_count; break;
     case CLASS_WEREWOLF:      counter = ww_count; break;
     case CLASS_MASTER:        counter = guyver_count; break;

     case CLASS_DRAGON:        counter = dragon_count; break;
     case CLASS_DRAGOON:       counter = dragoon_count; break;

     case CLASS_HIGHLANDER:    counter = jedi_count; break;
     case CLASS_MAGE:          counter = mage_count; break;

     case CLASS_SUMMONER:      counter = summoner_count; break;
     case CLASS_WIZARD:        counter = wizard_count; break;
     case CLASS_NECROMANCER:   counter = necro_count; break;

     case CLASS_SHADOW:        counter = shadow_count; break;
     case CLASS_HUNTER:        counter = hunter_count; break;

     case CLASS_DARK_KNIGHT:   counter = dknight_count; break;
     case CLASS_DAYWALKER:     counter = daywalker_count; break;
     case CLASS_UNDEAD_KNIGHT: counter = undead_count; break;
     case CLASS_SAIYAN:        counter = saiyan_count; break;

     case CLASS_AMAZON:        counter = amazon_count; break;
     case CLASS_SORCEROR:      counter = sorceror_count; break;

     case CLASS_PALADIN:       counter = paladin_count; break;
     case CLASS_SAMURAI:       counter = samurai_count; break;
     case CLASS_DARAMON:       counter = dara_count; break;
     case CLASS_ASSASSIN:      counter = assassin_count; break;
     case CLASS_NIGHT_BLADE:   counter = blade_count; break;
     case CLASS_DIVINE_KNIGHT: counter = divine_count; break;
     case CLASS_KODO:          counter = kodo_count; break;
  }

  if ( counter > 99 )
     {
        stc( "That class is full at the moment. Please choose another one.\n\r", ch );
        
        if ( number == 1 )
           ch->class = 0;
           else if ( number == 2 )
                   ch->class_2 = 0;
           else if ( number == 3 )
                   ch->class_3 = 0;
           else if ( number == 4 )
                   ch->class_4 = 0;
                   else ch->class_5 = 0;

        return -1;
     }

  return 1;
}

/* MUDSTAT command */
void do_mudstat( CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;  
  int mage_count=0, drow_count=0, ww_count=0, monk_count=0, angel_count=0, dragon_count=0, dragoon_count =0, samurai_count =0;
  int vampire_count=0, ninja_count=0, jedi_count=0, demon_count=0, paladin_count=0, sorceror_count=0, amazon_count=0;
  int shadow_count=0, hunter_count=0, dknight_count=0, daywalker_count=0, undead_count=0, saiyan_count=0;
  int summoner_count=0, wizard_count=0, necro_count=0, dara_count=0, assassin_count=0, blade_count=0, divine_count=0;
  int newbie_count=0, total_count=0;
  int kodo_count=0, kuruda_count=0, shaolin_count=0, guyver_count=0, garou_count=0, reaver_count=0;
  int elf_count=0, dragooon_count=0, fiend_count=0, hobbit_count=0;
  int snow_count=0, phoenix_count=0, duergar_count=0, wisp_count=0, sdwr_count=0;
  extern char str_boot_time[];

  if ( IS_NPC(ch) )
     return;

  ch->wait += 12;

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character != NULL ) 
       gch = d->character;
       else continue;

    if (!d->connected == CON_PLAYING) continue;

    if (gch->level > 6) continue;

    total_count++;

    switch ( gch->class )
    {
      case 0                   : newbie_count++;break;
      case CLASS_DEMON         : demon_count++;break;
      case CLASS_MAGE          : mage_count++;break;
      case CLASS_WEREWOLF      : ww_count++;break;
      case CLASS_VAMPIRE       : vampire_count++;break;
      case CLASS_HIGHLANDER    : jedi_count++;break;
      case CLASS_DROW          : drow_count++;break;
      case CLASS_MONK          : monk_count++;break;
      case CLASS_NINJA         : ninja_count++;break;
      case CLASS_ANGEL         : angel_count++;break;
      case CLASS_DRAGOON       : dragoon_count++;break;
      case CLASS_DRAGON        : dragon_count++;break;
      case CLASS_PALADIN       : paladin_count++;break;
      case CLASS_AMAZON        : amazon_count++;break;
      case CLASS_SORCEROR      : sorceror_count++;break;
      case CLASS_SHADOW        : shadow_count++;break;
      case CLASS_HUNTER        : hunter_count++;break;
      case CLASS_DARK_KNIGHT   : dknight_count++;break;
      case CLASS_UNDEAD_KNIGHT : undead_count++;break;
      case CLASS_SAIYAN        : saiyan_count++;break;
      case CLASS_DAYWALKER     : daywalker_count++;break;
      case CLASS_SAMURAI       : samurai_count++;break;
      case CLASS_SUMMONER      : summoner_count++;break;
      case CLASS_NECROMANCER   : necro_count++;break;
      case CLASS_WIZARD        : wizard_count++;break;
      case CLASS_DARAMON       : dara_count++;break;
      case CLASS_ASSASSIN      : assassin_count++;break;
      case CLASS_NIGHT_BLADE   : blade_count++;break;
      case CLASS_DIVINE_KNIGHT : divine_count++;break;
      case CLASS_SHAOLIN       : shaolin_count++;break;
      case CLASS_KURUDA        : kuruda_count++;break;
      case CLASS_GAROU         : garou_count++;break;
      case CLASS_MASTER        : guyver_count++;break;
      case CLASS_SOULREAVER    : reaver_count++;break;
      case CLASS_FIEND         : fiend_count++;break;
      case CLASS_HOBBIT        : hobbit_count++;break;
      case CLASS_SNOW_ELF      : snow_count++;break;
      case CLASS_DUERGAR       : duergar_count++;break;
      case CLASS_PHOENIX       : phoenix_count++;break;
      case CLASS_WISP          : wisp_count++;break;
      case CLASS_DARK_ELF      : elf_count++;break;
      case CLASS_SHADOW_WAR    : sdwr_count++;break;
      case CLASS_DRAGON_KNIGHT : dragooon_count++;break;
    }
  }

  send_to_char("#b--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b== #R[#y^^#R]  MUD STATS  [#y^^#R]#b ==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--#n\n\r\n\r",ch);
  send_to_char("#LOnline Players by Class:#n\n\r\n\r",ch);
  sprintf(buf, "#pDragons#n         : %-3d      #wPaladins#n     : %-3d       #eNinjas#n      : %-3d\n\r",
    dragon_count, paladin_count, ninja_count);
  send_to_char(buf,ch);
  sprintf(buf, "#wMonks#n           : %-3d      #yWizards#n      : %-3d       #RVampires#n    : %-3d\n\r",
    monk_count, wizard_count, vampire_count);
  send_to_char(buf,ch);
  sprintf(buf, "#rDemons#n          : %-3d      #yWerewolves#n   : %-3d       #eDrows#n       : %-3d\n\r",
    demon_count, ww_count, drow_count);
  send_to_char(buf,ch);
  sprintf(buf, "#RUndead Knights#n  : %-3d      #eClones#n       : %-3d       #cHighwinds#n   : %-3d\n\r",
    undead_count, assassin_count, dragoon_count);
  send_to_char(buf,ch);
  sprintf(buf, "#eNecromancers#n    : %-3d      #wAngels#n       : %-3d       #eCommanders#n  : %-3d\n\r",
    necro_count, angel_count, kuruda_count);
  send_to_char(buf,ch);
  sprintf(buf, "#CSamurais#n        : %-3d      #eAssassins#n    : %-3d       #RSaiyans#n     : %-3d\n\r",
    samurai_count, shadow_count, saiyan_count);
  send_to_char(buf,ch);
  sprintf(buf, "#wHobbits#n         : %-3d      #CSnow Elves#n   : %-3d       #gDuergars#n    : %-3d\n\r",
    hobbit_count, snow_count, duergar_count);
  send_to_char(buf,ch);
  sprintf(buf, "#eShadow Warriors#n : %-3d      #yWisps#n        : %-3d       #RPhoenixes#n   : %-3d\n\r",
    sdwr_count, wisp_count, phoenix_count);
  send_to_char(buf,ch);
  sprintf(buf, "#PDaramons#n        : %-3d      #RDaywalkers#n   : %-3d       #PDragoons#n    : %-3d\n\r",
	dara_count, daywalker_count, dragooon_count);
  send_to_char(buf,ch);
  sprintf(buf, "#PBlade Lords#n     : %-3d      #RFiends#n       : %-3d\n\r",
	guyver_count, fiend_count);
  send_to_char(buf,ch);

  if ( ( arena != FIGHT_WAR || IS_IMMORTAL( ch ) ) && w1_kdg != -1 )
     {
         send_to_char("#b--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==     WAR RESULTS      #b==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--#n\n\r\n\r",ch);

         sprintf( buf, "#oCongratulations of %s #obeating all comers with a score of #w%d #owar kills:#n\n\r\n\r", king_table[ w1_kdg ].who_name, w1_kills );
         stc(buf,ch);

         sprintf( buf, "#eRunners up go to %s #ewith its top notch score of #w%d #ewar kills:#n\n\r\n\r", king_table[ w2_kdg ].who_name, w2_kills );
         stc(buf,ch);

         sprintf( buf, "#CMan of the Tournament is none other then #w%s #Cwith their unmerciful #w%d #Ckills:#n\n\r", man_match, man_kills );
         stc(buf,ch);
     }

  send_to_char("#b--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==      OTHER STATS     #b==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--#n\n\r\n\r",ch);

  if ( arena == FIGHT_WAR )
     send_to_char("#LA MASSIVE Kingdom War is #w#zON!!!#n #LAll #RHell #Lis loose!#n\n\r\n\r",ch);
     else {
            if ( armageddon == TRUE )
               send_to_char("#LArmageddon is #w#zON!!!#n\n\r",ch);
               else if ( mysafe != 0 )
                       send_to_char("#LSafes are #w#zOUT!!!#n\n\r",ch);

            if ( nogang != 0 )
               send_to_char("#LGroup Battles are #w#zOUT!!!#n\n\r",ch);
          }

  if ( dungeon_timer > 0 )
    send_to_char("#LThe #eDungeon #Lof #RDeath #Lis waiting for you... #wComeOn#L!#n\n\r\n\r#n",ch);

  if ( betting_points > 0 && betting_points < 10000000 )
     {
        sprintf( buf, "#LBetting will be #wturned on #Lwhen #y%d #Lmore #ygold #Lpieces are put in the pool.#n\n\r", 10000000 - betting_points );
        stc( buf, ch );
     }

  if ( arena == FIGHT_AUTO )
    send_to_char("#LThe #PA#oren#Pa #Lis open for #w#zall#n #Lcompetitors!#n\n\r\n\r#n",ch);
    else if ( arena == FIGHT_ARENA )
            send_to_char("#LThe #PA#oren#Pa #Lis open for #cone #eon #Cone #Rcombat#L!#n\n\r\n\r#n",ch);

  if ( scavenger_time > 0 )
    send_to_char("#LA #PScAvEnGeR #oHuNt #Lis currently going on!#n\n\r\n\r#n",ch);

   switch( eipheal )
   {
       default:
       case 0: break;

       case 1: stc( "#PEipheal #Lhas enhanced #w#zALL#n #Ldamage by #C100%#L!!!#n\n\r", ch );
               break;

       case 2: stc( "#PEipheal #Lhas enhanced #w#zALL#n #Lprotection by #C100%#L!!!#n\n\r", ch );
               break;

       case 3: stc( "#PEipheal #Lhas enhanced #w#zALL#n #Lfocus priorities to #C100%#L!!!#n\n\r", ch );
               break;

       case 4: stc( "#PEipheal #Lhates #wHoly attacks#L, thus he #e#znullifies#n #Lthem!!!#n\n\r", ch );
               break;

       case 5: stc( "#PEipheal #Lhates #RFire attacks#L, thus he #e#znullifies#n #Lthem!!!#n\n\r", ch );
               break;

       case 6: stc( "#PEipheal #Lhates #CCold attacks#L, thus he #e#znullifies#n #Lthem!!!#n\n\r", ch );
               break;

       case 7: stc( "#PEipheal #Lhates #yThunder attacks#L, thus he #e#znullifies#n #Lthem!!!#n\n\r", ch );
               break;

       case 8: stc( "#PEipheal #Lhates #oEarth attacks#L, thus he #e#znullifies#n #Lthem!!!#n\n\r", ch );
               break;

       case 9: stc( "#PEipheal #Lhas enhanced #o#zLunar#n #Lattacks by #C100%#L!!!#n\n\r", ch );
               break;

       case 10: stc( "#PEipheal #Lhas enhanced #w#zALL#n #Lweaknesses by #C200%#L!!!#n\n\r", ch );
                break;
   }

   switch( conan )
   {
       default: break;

       case 1: stc( "#oConan #Lhas enhanced #w#zslash#n #Ldamage by #C50%#L!!!#n\n\r", ch );
               break;

       case 2: stc( "#oConan #Lhas enhanced #w#zpierce#n #Ldamage by #C50%#L!!!#n\n\r", ch );
               break;

       case 3: stc( "#oConan #Lhas enhanced #w#zblast#n #Ldamage by #C50%#L!!!#n\n\r", ch );
               break;

       case 4: stc( "#oConan #Lhas enhanced #w#zblunt#n #Ldamage by #C50%#L!!!#n\n\r", ch );
               break;

       case 5: stc( "#oConan #Lhas enhanced #w#zbeam#n #Ldamage by #C50%#L!!!#n\n\r", ch );
               break;

       case 6: stc( "#oConan #Lhas enhanced #w#zphysical#n #Ldamage by #C25%#L!!!#n\n\r", ch );
               break;

       case 7: stc( "#oConan #Lhas enhanced #w#zmagical#n #Ldamage by #C25%#L!!!#n\n\r", ch );
               break;

       case 8: stc( "#oConan #Lhas enhanced #w#zmanual#n #Lblocking by #C25%#L!!!#n\n\r", ch );
               break;

       case 9: stc( "#oConan #Lhas enhanced #w#zall#n #Lcounters by #C25%#L!!!#n\n\r", ch );
               break;

       case 10: stc( "#oConan #Lhas enhanced all #w#znormal#n #Lsummon damage by #C200%#L!!!#n\n\r", ch );
                break;

       case 11: stc( "#oConan #Lhas enhanced all #w#znatural#n #Ldamage by #C100%#L!!!#n\n\r", ch );
                break;
   }

  sprintf(buf, "#RNumber of players connected since last copyover/restart :#C %d\n\r#n", players_logged);
  send_to_char(buf,ch);
  sprintf(buf, "#RNumber of players decapitated since last copyover/restart  :#C %d\n\r\n\r#n", players_decap);
  send_to_char(buf,ch);
  sprintf( buf, "#eValley #Lof #eDeath #Lwas last #w(re)#ystarted #Lat : #C%s\r#LThe system time is currently            : #C%s\n\r", str_boot_time, (char *) ctime( &current_time));
  send_to_char( buf, ch );
  send_to_char("#b--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==#w**#b==--==--#n\n\r\n\r",ch);
}

/* show linkdeads - code by Marlow, not Jobo */

void do_linkdead(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;

  for (gch = char_list; gch != NULL; gch = gch->next)
  {
    if (IS_NPC(gch) || gch->desc) continue;
    found = TRUE;
    sprintf(buf, "Name: %12s. (Room: %5d)\n\r", gch->name, gch->in_room == NULL ? : gch->in_room->vnum);
    send_to_char(buf,ch);
  }
  if(!found) send_to_char("No Linkdead Players found\n\r",ch);
}

void restore_char( CHAR_DATA *ch )
{
    int count;

    ch->hit = ch->max_hit;
    ch->mana = ch->max_mana;
    ch->move = ch->max_move;
    ch->agg = 0;
    ch->wait = 0;
  
    if ( ch->position < POS_FIGHTING )
       ch->position = POS_STANDING;

    clear_affects( ch );

    if ( is_affected( ch, skill_lookup("gods disfavor") ) )
       affect_strip( ch, skill_lookup("gods disfavor") );

    if ( IS_NPC(ch) )
       return;

    if ( IS_CLASS( ch, CLASS_WEREWOLF ) )
       do_unwerewolf( ch, "" );

    // You lose the Avenger affect with restores
    if ( ( IS_CLASS( ch, CLASS_ANGEL ) || IS_CLASS( ch, CLASS_SHADOW_WAR ) ) && ch->ctimer[8] > 0 )
       {
          ch->ctimer[8] = 0;

          if ( IS_CLASS( ch, CLASS_ANGEL ) )
             do_trueform( ch, "" );
       }

    for ( count = 0; count < 20; count++ )
    {
       if ( !( IS_CLASS( ch, CLASS_DARAMON ) && count == 8 )
            && !( IS_CLASS( ch, CLASS_ANGEL ) && ( count == 3 || count == 4 ) )
            && !( IS_CLASS( ch, CLASS_DEMON ) && ( count == 11 || count == 12 ) ) )
          ch->ctimer[count] = 0;
    }

    if ( IS_SET(ch->newbits, NEW_MONKCLOAK) )
       REMOVE_BIT(ch->newbits, NEW_MONKCLOAK);

    if ( IS_SET(ch->more2, MORE2_VORTEX) )
       REMOVE_BIT(ch->more2, MORE2_VORTEX);

    if ( IS_SET( ch->more3, MORE3_CONFUSE ) )
       REMOVE_BIT( ch->more3, MORE3_CONFUSE );

    if ( IS_SET(ch->more2, MORE2_DESPERATE) )
       REMOVE_BIT(ch->more2, MORE2_DESPERATE);

    if ( IS_SET(ch->extra, EXTRA_DANCE) )
       REMOVE_BIT(ch->extra, EXTRA_DANCE);

    if ( IS_SET(ch->more3, MORE3_CLAN1) )
       REMOVE_BIT(ch->more3, MORE3_CLAN1);

    if ( IS_SET(ch->more3, MORE3_CLAN2) )
       REMOVE_BIT(ch->more3, MORE3_CLAN2);

    if ( IS_SET(ch->more3,MORE3_DASH) )
       REMOVE_BIT(ch->more3,MORE3_DASH);

    if ( IS_SET(ch->more3,MORE3_BLAST) )
       REMOVE_BIT(ch->more3,MORE3_BLAST);

    if ( IS_SET(ch->more3,MORE3_BOMB) )
       REMOVE_BIT(ch->more3,MORE3_BOMB);

    if ( IS_SET(ch->more2,MORE2_COMBO1) )
       REMOVE_BIT(ch->more2,MORE2_COMBO1);

    if ( IS_SET(ch->more2,MORE2_COMBO2) )
       REMOVE_BIT(ch->more2,MORE2_COMBO2);

    if ( IS_SET(ch->more2,MORE2_COMBO3) )
       REMOVE_BIT(ch->more2,MORE2_COMBO3);

    if ( IS_SET(ch->more2,MORE2_COMBO4) )
       REMOVE_BIT(ch->more2,MORE2_COMBO4);

    if ( IS_SET(ch->more2,MORE2_COMBO5) )
       REMOVE_BIT(ch->more2,MORE2_COMBO5);

    if ( IS_SET(ch->more2, MORE2_SHADOW) )
       REMOVE_BIT(ch->more2, MORE2_SHADOW);

    if ( IS_SET(ch->more2,MORE2_CHANCE) )
       REMOVE_BIT(ch->more2,MORE2_CHANCE);

    if ( IS_SET(ch->more2, MORE2_DSMASHER) )
       REMOVE_BIT(ch->more2, MORE2_DSMASHER);

    if ( IS_SET(ch->more,MORE_NINJA) )
       REMOVE_BIT(ch->more,MORE_NINJA);

    if ( IS_SET(ch->more,MORE_VEIL) )
       REMOVE_BIT(ch->more,MORE_VEIL);

    if ( IS_SET(ch->more,MORE_DVEIL) )
       REMOVE_BIT(ch->more,MORE_DVEIL);

    if ( IS_SET(ch->more,MORE_HAMMER) )
       REMOVE_BIT(ch->more,MORE_HAMMER);

    if ( IS_SET(ch->more,MORE_SMOKE) )
       REMOVE_BIT(ch->more,MORE_SMOKE);

    if ( IS_SET(ch->newbits, NEW_CLOAK) )
       REMOVE_BIT(ch->newbits, NEW_CLOAK);

    ch->tick_timer[TIMER_CAN_GUST] = 0;
    ch->tick_timer[TIMER_INFERNO] = 0;
    ch->tick_timer[TIMER_CAN_BREATHE_FROST] = 0;

    if ( IS_CLASS( ch, CLASS_UNDEAD_KNIGHT ) )
       {
          ch->pcdata->powers[POWER_FREEZE] = 0;
          ch->pcdata->powers[POWER_WORD] = 0;
          ch->pcdata->powers[POWER_FLAME] = 0;
          ch->pcdata->powers[POWER_BLIND] = 0;
          ch->pcdata->powers[POWER_CHILL] = 0;
          ch->pcdata->powers[POWER_BLAST] = 0;
       }

    ch->lightning_timer = 0;
    ch->fear_timer = 0;
    ch->poison_timer = 0;
    ch->block_meter = ( IS_SET( ch->cmbt[5], SKL2_IBODY ) ) ? 150 : 100;

    ch->sam_chi[CURRENT] = ch->sam_chi[MAXIMUM];
    REMOVE_BIT( ch->more3, MORE3_DODGE );
    REMOVE_BIT( ch->more3, MORE3_RETORT );

    ch->focus[CURRENT] = 100;
    ch->chi[CURRENT] = 200;
    ch->dlife[CURRENT] = 100;
    ch->vlife[CURRENT] = 100;
    ch->force = 100;

    ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM];
    ch->quint[BODY] = ch->quint[AVATAR] = ch->spheres[MPRI];
    ch->pcdata->powers[DRAGOON_SPIRIT] = ch->pcdata->powers[DRAGOON_LEVEL] * 100;

    if ( IS_CLASS( ch, CLASS_KURUDA ) )
       {
          if ( IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
             REMOVE_BIT( ch->pcdata->powers[MONK_SKILL], MONK_INVIN );
       }

    if ( IS_CLASS( ch, CLASS_MONK ) && ch->pcdata->powers[MONK_SHIKI] > 0 )
       ch->pcdata->powers[MONK_SHIKI] = 0;

    if ( IS_CLASS(ch, CLASS_DROW) )
       ch->cmbt[3] = 5;

    if ( IS_SET( ch->more2, MORE2_SPINCH ) )
       REMOVE_BIT(ch->more2, MORE2_SPINCH);

    ch->saiyan_timer = 0;
    ch->control_timer = 0;
    ch->obeah_timer = 0;
    ch->vampirestuff = 0;
    ch->dragoon_timer = 0;
    ch->dragon_timer = 0;
    ch->divine_timer = 0;
    ch->monk_timer = 0;
    ch->throw_timer = 0;
    ch->defiance_timer = 0;
    ch->power_timer = 0;
    ch->use_timer = 0;
    ch->jedi_timer = 0;
    ch->ninja_timer = 0;
    ch->aragami_timer = 0;
    ch->null_timer = 0;
    ch->guarding = 0;
    ch->mage_timer = 0;
    ch->plasma_timer = 0;
    ch->absorb_timer = 0;

    ch->achi = 0;

    ch->seeself = 0;
    ch->noprotection = 0;
    ch->revdam = 0;
    ch->revregen = 0;
}

void clear_affects( CHAR_DATA *ch )
{
    if ( IS_SET(ch->garou1, GAROU_WHELP2) )
       {
           REMOVE_BIT(ch->garou1, GAROU_WHELP2);
           ch->max_hit += 750000;
       }

    if ( IS_SET(ch->more, MORE_BLOOD) )
       REMOVE_BIT(ch->more, MORE_BLOOD);
 
    if ( IS_SET(ch->garou1, GAROU_PAWS) )
       REMOVE_BIT(ch->garou1, GAROU_PAWS);

    if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
       REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);

    if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);

    if ( IS_EXTRA(ch, EXTRA_ROT) )
       REMOVE_BIT(ch->extra, EXTRA_ROT);

    if ( IS_SET(ch->flag2, AFF2_BALOR) )
       REMOVE_BIT(ch->flag2, AFF2_BALOR);

    if ( IS_SET(ch->more2,MORE2_DESPONDENCY) ) 
       REMOVE_BIT(ch->more2,MORE2_DESPONDENCY);

    if ( IS_SET(ch->more, MORE_DISEMPOWER) )
       REMOVE_BIT(ch->more, MORE_DISEMPOWER);

    if ( IS_SET(ch->more, MORE_HEX) )
       REMOVE_BIT(ch->more, MORE_HEX);

    if ( IS_SET( ch->extra, EXTRA_LOWDEF ) )
       REMOVE_BIT( ch->extra, EXTRA_LOWDEF );

    if ( IS_SET( ch->extra, EXTRA_LOWOFF ) )
       REMOVE_BIT( ch->extra, EXTRA_LOWOFF );

    if ( IS_SET(ch->more,MORE_STONE) )
       {
          REMOVE_BIT(ch->more,MORE_STONE);
          ch->stone_timer = 0;
       }

    if ( IS_SET(ch->more, MORE_BLIND) )
       {
          REMOVE_BIT(ch->more, MORE_BLIND);
          ch->blind_timer = 0;
       }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       REMOVE_BIT(ch->more, MORE_IMPALE);

    if ( IS_SET(ch->more, MORE_POISON) )
       REMOVE_BIT(ch->more, MORE_POISON);

    if ( IS_SET(ch->more2, MORE2_NOLEGS) )
       REMOVE_BIT(ch->more2, MORE2_NOLEGS);

    if ( IS_SET(ch->more2, MORE2_NOARMS) )
       REMOVE_BIT(ch->more2, MORE2_NOARMS);

    if ( IS_SET(ch->more, MORE_PINCH) )
       REMOVE_BIT(ch->more, MORE_PINCH);

    if ( IS_SET(ch->more, MORE_CONFUSION) )
       REMOVE_BIT(ch->more, MORE_CONFUSION);

    if ( IS_SET( ch->more3, MORE3_CONFUSE ) )
       REMOVE_BIT( ch->more3, MORE3_CONFUSE );

    if ( IS_SET(ch->more, MORE_PAIN) )
       REMOVE_BIT(ch->more, MORE_PAIN);

    if ( IS_SET( ch->extra, EXTRA_TLOCK ) )
       REMOVE_BIT( ch->extra, EXTRA_TLOCK );

    if ( IS_SET( ch->elemental, ELE_NONE ) )
       REMOVE_BIT( ch->elemental, ELE_NONE );

    if ( IS_SET( ch->elemental, ELE_BTHROUGH ) )
       REMOVE_BIT( ch->elemental, ELE_BTHROUGH );

    if ( IS_SET( ch->extra, EXTRA_FBITE ) )
       REMOVE_BIT( ch->extra, EXTRA_FBITE );

    if ( IS_SET(ch->more, MORE_FROZEN) )
       {
          REMOVE_BIT(ch->more, MORE_FROZEN);
          ch->freeze_timer = 0;
       }

    if ( !IS_NPC( ch ) && IS_SET( ch->extra, EXTRA_ZOMBIE ) )
       REMOVE_BIT( ch->extra, EXTRA_ZOMBIE );

    ch->stunned = 0;
    ch->noblock = 0;
    ch->nodamage = 0;

    if ( is_affected( ch,skill_lookup("despondency") ) )
       affect_strip( ch, skill_lookup("despondency") );

    if ( is_affected( ch,skill_lookup("web") ) )
       affect_strip( ch, skill_lookup("web") );

    if ( is_affected( ch,skill_lookup("faerie fire") ) )
       affect_strip( ch, skill_lookup("faerie fire") );

    if ( is_affected( ch,skill_lookup("blindness") ) )
       affect_strip( ch, skill_lookup("blindness") );

    if ( is_affected( ch,skill_lookup("poison") ) )
       affect_strip( ch, skill_lookup("poison") );

    if ( is_affected( ch,skill_lookup("age") ) )
       affect_strip( ch, skill_lookup("age") );

    if ( is_affected( ch,skill_lookup("royal flush") ) )
       affect_strip( ch, skill_lookup("royal flush") );

    if ( is_affected( ch,skill_lookup("vampires touch") ) )
       affect_strip( ch, skill_lookup("vampires touch") );

    if ( is_affected( ch,skill_lookup("slow") ) )
       affect_strip( ch, skill_lookup("slow") );

    if ( is_affected( ch,skill_lookup("lolth charm") ) )
       affect_strip( ch, skill_lookup("lolth charm") );

    if ( is_affected( ch,skill_lookup("soulstop") ) )
       affect_strip( ch, skill_lookup("soulstop") );

    if ( is_affected( ch,skill_lookup("cloud kill") ) )
       affect_strip( ch, skill_lookup("cloud kill") );

    if ( is_affected( ch, skill_lookup("curse of allah") ) )
       affect_strip( ch, skill_lookup("curse of allah") );

    if ( is_affected( ch,skill_lookup("chill touch") ) )
       affect_strip( ch, skill_lookup("chill touch") );

    if ( is_affected( ch,skill_lookup("curse") ) )
       affect_strip( ch, skill_lookup("curse") );

    if ( is_affected( ch,skill_lookup("drowfire") ) )
       affect_strip( ch, skill_lookup("drowfire") );

    if ( is_affected( ch,skill_lookup("blindness") ) )
       affect_strip( ch, skill_lookup("blindness") );

    if ( is_affected( ch,skill_lookup("infirmity") ) )
       affect_strip( ch, skill_lookup("infirmity") );

    if ( is_affected( ch,skill_lookup("weaken") ) )
       affect_strip( ch, skill_lookup("weaken") );

    if ( is_affected( ch, skill_lookup("last prayer") ) )
       affect_strip( ch, skill_lookup("last prayer") );

    if ( is_affected( ch, skill_lookup("fear") ) )
       affect_strip( ch, skill_lookup("fear") );

    if ( is_affected( ch, skill_lookup("drow poison") ) )
       affect_strip( ch, skill_lookup("drow poison") );

    if ( is_affected( ch, skill_lookup("vision destroyer") ) )
       affect_strip( ch, skill_lookup("vision destroyer") );

    if ( IS_SET(ch->monkstuff, MONK_DEATH) )
       REMOVE_BIT(ch->monkstuff, MONK_DEATH);

    if ( IS_SET(ch->more, MORE_SIN) )
       REMOVE_BIT(ch->more, MORE_SIN);

    if ( IS_SET(ch->more,MORE_TAUNT) ) 
       REMOVE_BIT(ch->more, MORE_TAUNT);
}

void update_pulses( CHAR_DATA *ch )
{
      if ( IS_NPC( ch ) )
         return;

	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->vampirestuff > 0 )
         {
		ch->vampirestuff--;
            if ( ch->vampirestuff == 0 )
               send_to_char("You now have the ability to darken another area.\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->dragon_timer > 0 )
         {
		ch->dragon_timer--;
            if ( ch->dragon_timer == 0 )
               send_to_char("You have regain your dragon senses.\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->aragami_timer > 0 )
         {
		ch->aragami_timer--;
            if ( ch->aragami_timer == 0 )
               send_to_char("#wYou can now do another #ycounter #Ctechnique#w.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->achi > 0 )
         {
		ch->achi--;
            if ( ch->achi == 0 )
               send_to_char("#wYou can now do another #ygreat #Csummon#w.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->stunning_timer > 0 )
         {
		ch->stunning_timer--;
            if ( ch->stunning_timer == 0 )
               send_to_char("#wYou can now do another #ystunning #Cblow#w.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->throw_timer > 0 )
         {
		ch->throw_timer--;
            if ( ch->throw_timer == 0 )
               send_to_char("#wYou can now do another #yweapon #Ctechnique#w.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->break_timer > 0 )
         {
            if ( --ch->break_timer == 0 )
               send_to_char("#wYou can now do another #obreaker#w.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->saiyan_timer > 0 )
         {
		ch->saiyan_timer--;
            if ( ch->saiyan_timer == 0 )
               send_to_char("#wYou can now do another #yarmour #Ctechnique#w.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->monk_timer > 0 )
         {
		ch->monk_timer--;
            if ( ch->monk_timer == 0 )
               send_to_char("#wYou can now do another #yhealing #Ctechnique#w.#n\n\r",ch);
         }

      if (ch->pcdata->powers[POWER_WORD] > 0) ch->pcdata->powers[POWER_WORD]--;
      if (ch->pcdata->powers[POWER_FLAME] > 0) ch->pcdata->powers[POWER_FLAME]--;
      if (ch->pcdata->powers[POWER_BLIND] > 0) ch->pcdata->powers[POWER_BLIND]--;
      if (ch->pcdata->powers[POWER_FREEZE] > 0) ch->pcdata->powers[POWER_FREEZE]--;
      if (ch->pcdata->powers[POWER_CHILL] > 0) ch->pcdata->powers[POWER_CHILL]--;
      if (ch->pcdata->powers[POWER_BLAST] > 0) ch->pcdata->powers[POWER_BLAST]--;
      if (ch->lightning_timer > 0) ch->lightning_timer--;
      if (ch->fear_timer > 0) ch->fear_timer--;
      if (ch->poison_timer > 0) ch->poison_timer--;

	if ( !IS_NPC(ch) && ch->ninja_timer > 0 )
         {
		ch->ninja_timer--;
            if ( ch->ninja_timer == 0 )
               {
                  if ( IS_CLASS( ch, CLASS_SHADOW_WAR ) )
                     send_to_char("You can now do another dark hadou.\n\r",ch);
                     else send_to_char("You can now do another somersault strike.\n\r",ch);
               }
         }

	if ( !IS_NPC(ch) && ch->dragoon_timer > 0 )
         {
		ch->dragoon_timer--;
            if ( ch->dragoon_timer == 0 )
               send_to_char("You can now do another Jump Attack.\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->divine_timer > 0 )
         {
		ch->divine_timer--;
            if ( ch->divine_timer == 0 )
               send_to_char("You can now block another attack.\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->defiance_timer > 0 )
         ch->defiance_timer--;

	if ( !IS_NPC(ch) && ch->null_timer > 0 )
         ch->null_timer--;

	if ( !IS_NPC(ch) && ch->use_timer > 0 )
         {
		ch->use_timer--;
            if ( ch->use_timer == 0 )
               send_to_char("You can now use another item.\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->power_timer > 0 )
         ch->power_timer--;

	if ( !IS_NPC(ch) && ch->control_timer > 0 )
         {
		ch->control_timer--;
            if ( ch->control_timer == 0 )
               send_to_char("You can now do another Control.\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->mage_timer > 0 )
         {
		ch->mage_timer--;
            if ( ch->mage_timer == 0 )
               send_to_char("#PYou can now summon another #wdeity#P.#n\n\r",ch);
         }

	if ( !IS_NPC(ch) && ch->plasma_timer > 0 )
         ch->plasma_timer--;
}

void check_chou_kamehameha( CHAR_DATA *ch )
{
   switch( ch->nemesis )
   {
      default:
      case 0: perform_kamehameha( ch, "" );
   }
}

void check_gallet_gun( CHAR_DATA *ch )
{
   switch( ch->nemesis )
   {
      case 3: act("You put your hands together as sparks of electricity flashes amongst them.",ch,NULL,NULL,TO_CHAR);
              act("$n puts $s hands together as sparks of electricity flashes amongst them.",ch,NULL,NULL,TO_ROOM);
              act("The electricity transfuses into a spiral of energy as you concentrate harder.",ch,NULL,NULL,TO_CHAR);
              act("The electricity transfuses into a spiral of energy as $n concentrates harder.",ch,NULL,NULL,TO_ROOM);
              break;
      case 2: act("You scream loudly as the energy surrounding your body flashes viciously.",ch,NULL,NULL,TO_CHAR);
              act("$n screams loudly as the energy surrounding $s body flashes viciously.",ch,NULL,NULL,TO_ROOM);
              act("The air around you darkens as your energy ball sucks all the light from within.",ch,NULL,NULL,TO_CHAR);
              act("The air around $n's darkens as $s energy ball sucks all the light from within.",ch,NULL,NULL,TO_ROOM);
              break;
      case 1: act("The entire sky darkens before the savage flashes of light radiating off your trembling body.",ch,NULL,NULL,TO_CHAR);
              act("The entire sky darkens before the savage flashes of light radiating off $n'd trembling body.",ch,NULL,NULL,TO_ROOM);
              act("You snarl with pain as you attempt to control the Huge flow of energy amongst you.",ch,NULL,NULL,TO_CHAR);
              act("$n snarls with pain as $e attempts to control the Huge flow of energy amongst $m.",ch,NULL,NULL,TO_ROOM);
              act("Blinded from the energy radiating from you, you painfully redirect your hands towards the earth.",ch,NULL,NULL,TO_CHAR);
              act("Blinded from the energy radiating from $m, $n painfully redirects $s hands towards the earth.",ch,NULL,NULL,TO_ROOM);
              break;
      case 0: perform_gallet_gun( ch, "" );
   }
}

void update_timers( CHAR_DATA *ch )
{
      int gain = ( ch->fight_timer > 0 ) ? 5 : 25;

      if ( ch->blocking == -10 )
         ch->blocking = 0;

      if ( ch->position < POS_STUNNED && ( ch->in_room->vnum < 550 || ch->in_room->vnum > 629 ) && ch->air_timer == 0 )
         {
            if ( ch->mort_timer == 0 )
               ch->mort_timer = 10;
               else if ( --ch->mort_timer == 1 )
                       {
                          act("#RYou #egasp one final time before #wdying #efrom your #Rhideous #owounds#e...#n'",ch,NULL,NULL,TO_CHAR);
                          act("#R$n #egasps one final time before #wdying #efrom $s #Rhideous #owounds#e...#n'",ch,NULL,NULL,TO_ROOM);
 
                          stop_fighting( ch, TRUE );

                          if ( ch->attacker != NULL )
                             {
                                restore_char( ch->attacker );
                                ch->attacker->fight_timer = 0;
                             }

                          ch->raiding = 0;
                          ch->mort_timer = 0;
                          ch->fight_timer = 0;
                          raw_kill(ch);
                          ch->safe_timer = 5;
                          ch->login_timer = 3;

                          players_decap++;

                          if ( ( players_decap % 25 ) == 0 && players_decap > 25 && !armageddon )
                             do_nogang( ch, "" );

                          if ( ( players_decap % 30 ) == 0 && arena == FIGHT_CLEAR )
                             {
                                arena = FIGHT_AUTO;
                                do_info( NULL, "#wThe #PA#oren#Pa #gis open for #w#zEverybody#n#g! Use #yFtag #gto Join!!!#n" );
                             }

                          if ( ( players_decap % 50 ) == 0 && !armageddon )
                             {
                                start_scavenger();
                                do_sinfo( NULL, "#w50 #yitems #ghas been #Pscattered #gthroughout the #Prealm#g! Use #yScavenger #gto Join!!!#n" );
                             }
                       }
         }

      if ( ch->raiding > 0 )
         {
            if ( --ch->raiding == 0 )
               {
                  char buf[MAX_STRING_LENGTH];
                  char * deity_table[10] = { "#CShiva", "#RIfrit", "#wSiren", "#eDiablos", "#gTitan", "#yQuezacotl", "#pOdin", "#CCarbunkle", "#cLuna", "#oGaia" };

                  sprintf( buf, "#gThe #oland #gof #P%s #gis now under control by the %s #gclan!#n", ch->in_room->area->name, deity_table[ ch->pcdata->deity - 1 ] );
                  do_info( NULL, buf );

                  king_table[ch->in_room->area->land].members--;
                  king_table[ch->in_room->area->land = ch->pcdata->deity].members++;
               }
         }

      if ( ch->noskill > 0 )
         {
            if ( --ch->noskill > 0 )
               stc( "#RYour #wbody #yspasms #Rviolently #was it tries to contain the #Punstable #yenergy #wwithin you.#n", ch );
               else {
                       act( "#RYou #wclench your arms #Ppowerfully #was your body #Ceradicates #wthe unstable energy!#n", ch, NULL, NULL, TO_CHAR );
                       act( "#R$n #wclenches $s arms #Ppowerfully #was $s body #Ceradicates #wthe unstable energy!#n", ch, NULL, NULL, TO_ROOM );
                    }
         }

      if ( ch->jedi_timer > 0 )
         {
            if ( --ch->jedi_timer == 0 )
               {
                  act( "#yYour #olunar #Cshield #osuddenly shatters as beautiful energy fades into the #Csky#o.#n", ch, NULL, NULL, TO_CHAR );
                  act( "#y$n#y's #olunar #Cshield #osuddenly shatters as beautiful energy fades into the #Csky#o.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      ch->block_meter = UMIN( ( IS_SET( ch->cmbt[5], SKL2_IBODY ) ) ? 150 : 100, ch->block_meter + gain );

      if ( ch->strike_timer > 0 )
         {
            if ( arena != FIGHT_START || --ch->strike_timer == 0 )
               {
                  ch->strike_timer = 0;
                  ch->caller = NULL;
               }
         }

      if ( IS_SET( ch->itemaffect, ITEMA_REAPER ) || ( IS_CLASS( ch, CLASS_MASTER ) && ch->tier > 2 ) )
         {
            if ( ch->cmbt[2] != 10 )
               {
                  ch->cmbt[2] = 10;

                  if ( ch->position > POS_STUNNED && !IS_CLASS( ch, CLASS_MASTER ) )
                     {
                        act( "#PYour #cfacial #Pfeatures #edarken #Pas you breathe in #Ccoldly#P...#n", ch, NULL, NULL, TO_CHAR );
                        act( "#P$n#P's #cfacial #Pfeatures #edarken #Pas $e breathes in #Ccoldly#P...#n", ch, NULL, NULL, TO_ROOM );
                     }
               }
               else {
                       ch->cmbt[2] = -10;

                       if ( ch->position > POS_STUNNED && !IS_CLASS( ch, CLASS_MASTER ) )
                          {
                             act( "#yYour #cfacial #yfeatures #wlighten #yas you breathe out #Psoftly#y...#n", ch, NULL, NULL, TO_CHAR );
                             act( "#y$n#y's #cfacial #yfeatures #wlightens #yas $e breathes out #Psoftly#y...#n", ch, NULL, NULL, TO_ROOM );
                          }
                    }
         }

      if ( ch->anger_level > 300 )
         {
            if ( --ch->anger_level == 301 )
               {
                  ch->anger_level = 0;
                  act( "#PYou #wgrowl #Rangrilly #was your #Rrage #wsubsides...#n", ch, NULL, NULL, TO_CHAR );
                  act( "#P$n #wgrowls #Rangrilly #was $s #Rrage #wsubsides...#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->ctimer[18] > 0 )
         {
            if ( !str_cmp( "K", ch->groove ) )
               {
                  if ( --ch->ctimer[18] == 0 )
                     {
                        act( "#RYou #ygrowl #wangrilly #gas you #ereluctantly #ycalm down...#n", ch, NULL, NULL, TO_CHAR );
                        act( "#R$n #ygrowls #wangrilly #gas $e #ereluctantly #ycalms down...#n", ch, NULL, NULL, TO_ROOM );
                        ch->ctimer[9] = 0;
                     }
               }
               else if ( ( ch->ctimer[18] = UMAX( 0, ch->ctimer[18] - 2 ) ) == 0 )
                       {
                          act( "#yYou #wsigh softly as you #Crelax #wback into your #Rfighting #yposure#w.#n", ch, NULL, NULL, TO_CHAR );
                          act( "#y$n #wsighs softly as $e #Crelaxes #wback into $s #Rfighting #yposure#w.#n", ch, NULL, NULL, TO_ROOM );

                          if ( str_cmp( "N", ch->groove ) && str_cmp( "M", ch->groove ) )
                             ch->ctimer[9] = 0;
                       }
         }

      if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
           && IS_SET( ch->arena, AFF2_INARENA ) )
         {
            int team;

            if ( IS_SET( ch->arena, AFF2_TEAM1 ) )
               team = 0;
               else team = 1;

            if ( team_list->power_mode[team] == 5 )
               ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * 0.02 ) );

            if ( team_list->power_duration[team] > 0 )
               {
                  if ( team_list->power_mode[team] != 4 )
                     {
                        if ( --team_list->power_duration[team] == 0 )
                           team_list->power_mode[team] = 0;
                     }
                     else {
                             team_list->power_duration[team] = UMAX( 0, team_list->power_duration[team] - 5 );

                             if ( team_list->power_duration[team] == 0 )
                                team_list->power_mode[team] = 0;
                          }
               }
         }

       if ( IS_CLASS( ch, CLASS_DARAMON ) )
          {
             if ( ( ch->daramon_timer > 0 && --ch->daramon_timer == 0 )
                  || ( ch->daramon_timer == 0 && IS_SET( ch->more3, MORE3_ONE ) ) )
                {
                   if ( IS_SET( ch->more3, MORE3_ONE ) )
                      {
                         REMOVE_BIT( ch->more3, MORE3_ONE );
                         act( "You are no longer one with the universe as you collapse to your knees gasping for breath.", ch, NULL, NULL, TO_CHAR );
                         act( "$n is no longer one with the universe as $e collapses to $s knees gasping for breath.", ch, NULL, NULL, TO_ROOM );
                      }
                }
          }

      if ( IS_CLASS( ch, CLASS_PHOENIX ) )
         {
            if ( ch->fight_timer > 0 )
               ch->ctimer[8] = UMIN( 15, ch->ctimer[8]++ );
               else ch->ctimer[8] = 0;
         }

      if ( IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && IS_SET( ch->pcdata->powers[DRAGOON_CLASS], DRAGOON_USER )
           && ch->class_2 == 0 )
         {
            ch->pcdata->powers[DRAGOON_SPIRIT] = UMAX( 0, ch->pcdata->powers[DRAGOON_SPIRIT] - 50 );

            if ( ch->pcdata->powers[DRAGOON_SPIRIT] == 0 )
               do_dragoon_transform( ch, "" );
         }

      if ( IS_CLASS( ch, CLASS_FIEND ) && IS_SET( ch->affected_by, AFF_POLYMORPH ) && ch->class_2 == 0 )
         {
            if ( ch->pcdata->powers[DEMON_TRIGGER] > 0 )
               {
                  hit_gain( ch, ( ch->max_hit / 100 ) );
                  mana_gain( ch, ( ch->max_mana / 50 ) );
                  move_gain( ch, ( ch->max_move / 50 ) );
                  ch->pcdata->powers[DEMON_TRIGGER] = UMAX( 0, ch->pcdata->powers[DEMON_TRIGGER] - 100 );

                  if ( ch->pcdata->powers[DEMON_TRIGGER] == 0 )
                     do_true_demon( ch, "" );
               }
               else do_true_demon( ch, "" );
         }

      if ( IS_SET( ch->more, MORE_CLOUD ) )
         {
            if ( ch->mana > 500000 && IS_SET( ch->in_room->room_flags, ROOM_TOTAL_DARKNESS ) )
               ch->mana -= 500000;
               else do_dark_cloud( ch, "" );
         }

      if ( ch->fight_timer == 0 && ch->ctimer[9] > 0 )
         ch->ctimer[9] = 0;

      if ( !IS_SET( ch->extra, EXTRA_TLOCK ) )
         {         
            if ( is_affected( ch, skill_lookup( "speed up" ) ) )
               update_pulses( ch );

            update_pulses( ch );
         }
         else if ( number_percent() > 50 )
                 update_pulses( ch );

      if ( IS_CLASS( ch, CLASS_SNOW_ELF ) && ch->class_2 == 0 && IS_SET( ch->more, MORE_FROZEN ) && ch->pkill >= 6 )
         {
            act("#yThe #Lice #yaround your body causes you to #gregenerate!#n",ch,NULL,NULL,TO_CHAR);
            act("#yThe #Lice #yaround $n's body causes $m to #gregenerate!#n",ch,NULL,NULL,TO_ROOM);
            ch->hit = UMIN( ch->max_hit, ch->hit - is_elemental( ATTACK_ICE, ch, 1000000 ) );
         }

	if ( ch->freeze_timer > 0 )
         {
            ch->freeze_timer--;

            if ( ch->freeze_timer == 0 )
               {
                  if ( IS_CLASS (ch, CLASS_PHOENIX ) && ch->class_2 == 0 )
                     ch->hit = UMAX( 1, ch->hit - is_elemental( ATTACK_ICE, ch, 1000000 ) );

                  if ( IS_SET(ch->more, MORE_FROZEN) )
                     {
                        REMOVE_BIT(ch->more, MORE_FROZEN);
                        send_to_char("#bThe #wice #bencasing you melts away.#n\n\r",ch);
                        act("#bThe #wice #bencasing $n #bmelts away.#n",ch,NULL,NULL,TO_ROOM);
                     }
               }
         }

      if ( ch->stunned > 0 )
         {
            ch->stunned--;

            if ( ch->stunned == 0 )
               {
                  stc("#wYou #gno longer feel #yd#oizz#yy#g.#n\n\r", ch );
                  act("#w$n #gno longer feels #yd#oizz#yy#g.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->seeself > 0 )
         {
            ch->seeself--;

            if ( ch->seeself == 0 )
               {
                  stc("#PYou #ebreak hold of the #wcurse #ethat made you see #ydoubles#e.#n\n\r", ch );
                  act("#P$n #ebreaks hold of the #wcurse #ethat made $m see #ydoubles#e.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->revdam > 0 )
         {
            ch->revdam--;

            if ( ch->revdam == 0 )
               {
                  stc("#PYou #ebreak hold of the #wcurse #ethat made you #yhurt#e yourself.#n\n\r", ch );
                  act("#P$n #ebreaks hold of the #wcurse #ethat made $m #yhurt#e $mself.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->revregen > 0 )
         {
            ch->revregen--;

            if ( ch->revregen == 0 )
               {
                  stc("#PYou #ebreak hold of the #wcurse #ethat made your regen #ykills#e yourself.#n\n\r", ch );
                  act("#P$n #ebreaks hold of the #wcurse #ethat made $s regen #ykills#e $mself.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->noprotection > 0 )
         {
            ch->noprotection--;

            if ( ch->noprotection == 0 )
               {
                  stc("#PYou #ebreak hold of the #wcurse #ethat made you #ydefenseless#e.#n\n\r", ch );
                  act("#P$n #ebreaks hold of the #wcurse #ethat made $m #ydefenseless#e.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->noblock > 0 )
         {
            ch->noblock--;

            if ( ch->noblock == 0 )
               {
                  stc("#PYou #ebreak hold of the #wcurse #ethat was #yimmobolizing #eyou.#n\n\r", ch );
                  act("#P$n #ebreaks hold of the #wcurse #ethat was #yimmobolizing #e$m.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->nodamage > 0 )
         {
            ch->nodamage--;

            if ( ch->nodamage == 0 )
               {
                  stc("#PYou #ebreak hold of the #wcurse #ethat was #ydampering #eyour strength.#n\n\r", ch );
                  act("#P$n #ebreaks hold of the #wcurse #ethat was #ydampering #e$s strength.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ch->stone_timer > 0 )
         {
            if ( --ch->stone_timer == 0 )
               {
                  if ( IS_SET( ch->more, MORE_STONE ) )
                     {
                        REMOVE_BIT(ch->more, MORE_STONE);
                        send_to_char("#yYou #ocan suddenly #gmove #oagain!#n\n\r",ch);
                        act("#y$n #ocan suddenly #gmove #oagain!#n",ch,NULL,NULL,TO_ROOM);
                     }
               }
         }

      if ( ch->blind_timer > 0 )
         {
            if ( armageddon == TRUE )
               ch->blind_timer = 0;
               else ch->blind_timer--;

            if ( ch->blind_timer == 0 )
               {
                  if ( IS_SET(ch->more, MORE_BLIND) )
                     {
                        REMOVE_BIT(ch->more, MORE_BLIND);
                        send_to_char("#yYou can suddenly #wsee #yagain!#n\n\r",ch);
                        act("#y$n can suddenly #wsee #yagain!#n",ch,NULL,NULL,TO_ROOM);
                     }
               }
         }

	if ( !IS_NPC(ch) && ch->obeah_timer > 0 )
         {
		ch->obeah_timer--;
            if ( ch->obeah_timer == 0 )
               {
		      send_to_char("Your third eye stops glowing.\n\r",ch);
		      act("The eye in $n's forehead stops glowing.",ch,NULL,NULL,TO_ROOM);
               }
         }

      if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->mergence > 0 )
         ch->mergence--;

      if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_DRAGOON ) && ch->ctimer[8] > 0 )
         update_jumping( ch );

      if ( !IS_NPC( ch ) && ( IS_CLASS( ch, CLASS_MONK ) || IS_CLASS( ch, CLASS_NINJA ) ) && ch->ctimer[8] > 0 )
         {
            if ( --ch->ctimer[8] == 0 && ch->fighting != NULL )
               perform_innocence( ch, ch->fighting );
         }

      if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_KURUDA ) && ch->ctimer[19] > 0 )
         {
            ch->ctimer[19]--;
            continue_sbomb( ch );

            if ( ch->ctimer[19] == 0 && IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
               REMOVE_BIT( ch->pcdata->powers[MONK_SKILL], MONK_INVIN );
         }

      if ( !IS_NPC(ch) && ch->nemesis > 0 && IS_CLASS( ch, CLASS_SAIYAN ) )
         {
            ch->nemesis--;

            switch( ch->saiyanstuff )
            {
               case 2: check_chou_kamehameha( ch ); break;
               case 1: check_gallet_gun( ch ); break;
            }
         }

      if ( ch->saiyanlevel > 0 )
         {
            add_grove( ch, NULL, -40 );

            if ( ch->ctimer[9] == 100 )
               {
                  act( "#yYou #wsuddenly #Cclench #wyour fist as you #Ractivate #wyour groove mode!#n", ch, NULL, NULL, TO_CHAR );
                  act( "#y$n #wsuddenly #Cclenches #w$s fist as $e #Ractivates #w$s groove mode!#n", ch, NULL, NULL, TO_ROOM );
                  ch->ctimer[18] = 10;
                  ch->saiyanlevel = 0;
               }
               else {
                       act("#yYour #ebody #Pglows #wbrightly #eas #Rferocious #yenergy #eradiates off you.#n",ch,NULL,NULL,TO_CHAR);
                       act("#y$n#y's #ebody #Pglows #wbrightly #eas #Rferocious #yenergy #eradiates off $m.#n",ch,NULL,NULL,TO_ROOM);
                    }
         }

      if ( !IS_NPC(ch) && ch->nemesis > 0 && ( IS_CLASS( ch, CLASS_PALADIN ) || IS_CLASS( ch, CLASS_MONK ) ) )
         {
            if ( IS_CLASS( ch, CLASS_MONK ) && ch->pcdata->powers[MONK_SHIKI] > 0 )
               {
                  switch( ch->nemesis )
                  {
                     case 3: act("#yYour #ehand starts #Pglowly #wbrightly #eas you try to #Cchannel #ethe energy within you.#n",ch,NULL,NULL,TO_CHAR);
                             act("#y$n#y's #ehand starts #Pglowly #wbrightly #eas $e tries to #Cchannel #ethe energy within $m.#n",ch,NULL,NULL,TO_ROOM);
                             break;
                     case 2: act("#yYou #esuddenly raise your hand back as #Cunbelievable #yenergy #wEmItS #eitself from it!#n",ch,NULL,NULL,TO_CHAR);
                             act("#y$e #esuddenly raise $s hand back as #Cunbelievable #yenergy #wEmItS #eitself from it!#n",ch,NULL,NULL,TO_ROOM);
                             break;
                     case 1: if ( ch->fighting != NULL )
                                perform_final_heaven( ch, ch->fighting );
                                else ch->pcdata->powers[MONK_SHIKI] = 0;
                  }
               }
               else switch( ch->nemesis )
               {
                  case 4: if ( IS_CLASS(ch,CLASS_PALADIN) )
                          {
                             act("You say, '#yZxnit dbex aqsf krdma...#n'",ch,NULL,NULL,TO_CHAR);
                             act("$n says, '#yZxnit dbex aqsf krdma...#n'",ch,NULL,NULL,TO_ROOM);
                          }
                          else if ( IS_CLASS(ch,CLASS_MONK) )
                                  act("As you rush through the forests, your blazing hand leaves a river of flames behind you.",ch,NULL,NULL,TO_CHAR);
                          break;
                  case 3: if ( IS_CLASS(ch,CLASS_PALADIN) )
                          {
                             act("You say, '#yNem tiez hix arvxt ksdr...#n'",ch,NULL,NULL,TO_CHAR);
                             act("$n says, '#yNem tiez hix arvxt ksdr...#n'",ch,NULL,NULL,TO_ROOM);
                          }
                          else if ( IS_CLASS(ch,CLASS_MONK) )
                                  act("The waters part away from your feet as you blaze across the oceans.",ch,NULL,NULL,TO_CHAR);
                          break;
                  case 2: if ( IS_CLASS(ch,CLASS_PALADIN) )
                          {
                             act("You say, '#yTui Zxit Vertz wodk jdnr wols...#n'",ch,NULL,NULL,TO_CHAR);
                             act("$n says, '#yTui Zxit Vertz wodk jdnr wols...#n'",ch,NULL,NULL,TO_ROOM);
                             act("A sparkle in the distance catch your eyes.",ch,NULL,NULL,TO_ROOM);
                             act("A sparkle in the distance catch your eyes.",ch,NULL,NULL,TO_CHAR);
                          }
                          else if ( IS_CLASS(ch,CLASS_MONK) )
                                  act("As you reach lightspeed your body becomes blurry with unlimited passion.",ch,NULL,NULL,TO_CHAR);
                          break;
                  case 1: if ( IS_CLASS(ch,CLASS_PALADIN) )
                          {
                             act("You say, '#yApocalypse has arrived... You are your own Nemesis...#n'",ch,NULL,NULL,TO_CHAR);
                             act("$n says, '#yApocalypse has arrived... You are your own Nemesis...#n'",ch,NULL,NULL,TO_ROOM);
                             act("You say, '#yRedemption is at hand.#n'",ch,NULL,NULL,TO_CHAR);
                             act("$n says, '#yRedemption is at hand.#n'",ch,NULL,NULL,TO_ROOM);
                          }
                          else if ( IS_CLASS(ch,CLASS_MONK) )
                                  do_my_final_heaven(ch,"");
                          break;
               }
            ch->nemesis--;
         }

     if ( IS_SET( ch->newbits, NEW_ALIGHT ) ) 
        {
           ch->mana = UMAX( 0, ch->mana - ( ch->max_mana * 0.1 ) );

           if ( ch->mana == 0 || IS_AFFECTED( ch, AFF_MANA ) )
              do_anti_light( ch, "" );
        }

     if ( ch->position > POS_STUNNED )
        {
           if ( ch->topmode == 100 && ch->hit > ch->max_hit * 0.66 )
              ch->hit = UMIN( ch->max_hit, ch->hit + 100000 );
              else if ( !IS_NPC( ch ) && ch->topmode == 33 && ch->hit < ch->max_hit * 0.34 )
                      ch->hit = UMIN( ch->max_hit * 0.33, ch->hit + 100000 );
              else if ( !IS_NPC( ch ) && ch->topmode == 66 && ch->hit > ch->max_hit * 0.33
                        && ch->hit < ch->max_hit * 0.67 )
                      ch->hit = UMIN( ch->max_hit * 0.66, ch->hit + 100000 );

           if ( !IS_NPC( ch ) && ch->pcdata->blood_power == 6 )
              {
                 hit_gain( ch, 150000 );
                 mana_gain( ch, 250000 );
                 move_gain( ch, 250000 );
                 ch->agg = UMAX( 0, ch->agg - 10 );
              }

           if ( is_affected( ch, skill_lookup("regeneration") ) )
              {
                 if ( number_percent() > 50 && ch->hit < ch->max_hit )
                    {
                       act( "Your body releases a bunch of holy sparks as you feel more refreshed.", ch, NULL, NULL, TO_CHAR );
                       act( "$n's body releases a bunch of holy sparks as $e feels more refreshed.", ch, NULL, NULL, TO_ROOM );
                    }

                 hit_gain( ch, 100000 );
              }
              else if ( !IS_NPC(ch) && IS_CLASS( ch, CLASS_VAMPIRE ) && ch->pcdata->powers[VAMPIRE_REAVER] == REAVER_LIGHT )
                      hit_gain( ch, 100000 );

           if ( IS_CLASS( ch, CLASS_SHADOW ) && ch->gifts[AMI] > 1 && ch->position >= POS_STUNNED )
              {
                 if ( ch->gifts[CURRENT_METER] == AMI )
                    {
                       hit_gain( ch, 50000 );
                       mana_gain( ch, 500000 );
                       move_gain( ch, 500000 );
                    }
                    else {
                            mana_gain( ch, 100000 );
                            move_gain( ch, 100000 );
                         }
              }
        }

     if ( IS_SET(ch->monkstuff, MONK_HEAL) )
        {
           if ( ch->position > POS_INCAP && !( ch->hit == ch->max_hit && ch->mana == ch->max_mana &&
                ch->move == ch->max_move ) )
              {
                 hit_gain( ch, 50000 );
                 mana_gain( ch, 75000 );
                 move_gain( ch, 200000 );
                 stc("Your body emits glowing sparks.\n\r",ch);
                 act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM);
              }               
              else {
                      stc("The sparks fizzle and die.\n\r",ch);
                      act("The sparks around $n's body fizzles and dies.",ch,NULL,NULL,TO_ROOM);
                      REMOVE_BIT(ch->monkstuff, MONK_HEAL);
                   }
        }

     if ( IS_SET( ch->more3, MORE3_TURBO ) )
        {
           if ( ch->move > 2000001 )
              ch->move -= 800000;

           if ( ch->move <= 2000001 )
              do_blitzstrike( ch, "" );
        }
 
     if ( IS_SET( ch->more, MORE_BLITZ ) )
        {
           if ( ch->move > 2000001 )
              ch->move -= 800000;

           if ( ch->move <= 2000001 )
              do_blitztouch( ch, "" );
        }

     if ( IS_CLASS( ch, CLASS_ASSASSIN ) && IS_SET( ch->special, SPC_SOR ) )
        mana_gain( ch, UMIN( 300000, ch->pkill * 15000 ) );

     if ( IS_CLASS( ch, CLASS_ANGEL ) )
        {
           mana_gain( ch, UMIN( 200000, ch->pkill * 10000 ) + 300000 );
           move_gain( ch, UMIN( 200000, ch->pkill * 10000 ) + 300000 );
        }
        else if ( IS_GOOD( ch ) )
                mana_gain( ch, 250000 );
        else if ( IS_EVIL( ch ) )
                move_gain( ch, 250000 );

     if ( IS_SET( ch->cmbt[7], SKL_FOCUS ) )
        mana_gain( ch, 250000 );

     if ( IS_SET( ch->cmbt[7], SKL_BALANCE ) )
        move_gain( ch, 250000 );

     if ( ch->mana < 0 )
        ch->mana = 0;

     if ( ch->move < 0 )
        ch->move = 0;

     if ( ( IS_CLASS( ch, CLASS_ANGEL ) || IS_CLASS( ch, CLASS_SHADOW_WAR ) ) && ch->ctimer[8] > 0 )
        return;

     if ( ch->air_timer > 0 || ( ch->ctimer[8] > 0 && ( IS_CLASS( ch, CLASS_MONK ) || IS_CLASS( ch, CLASS_DRAGOON ) ) ) )
        return;

     if ( IS_SET( ch->in_room->room_flags, ROOM_BLAZE ) && !IS_CLASS( ch, CLASS_DEMON ) && number_percent() > 60 )
        {
           act("#RYou #yscream #Rin agony as the #yhungry flames #Reat through your flesh!#n", ch, NULL, NULL, TO_CHAR );
           act("#R$n #yscreams #Rin agony as the #yhungry flames #Reat through $s flesh!#n", ch, NULL, NULL, TO_ROOM );

           if ( IS_CLASS( ch, CLASS_PHOENIX ) )
              ch->hit = UMIN( ch->max_hit, ch->hit + 100000 );
              else if ( ch->hit > 99999 )
                      ch->hit = UMAX( 1, ch->hit - 100000 );
        }

     if ( IS_SET(ch->in_room->room_flags, ROOM_ASSASSIN) && !IS_CLASS( ch, CLASS_ASSASSIN ) && ch->position > POS_STUNNED )
        {
           if ( IS_CLASS( ch, CLASS_DEMON ) )
              {
                 hit_gain( ch, number_range(100000,250000) );
                 mana_gain( ch, number_range(250000,350000) );
                 move_gain( ch, number_range(250000,350000) );
                 act("#RYour #obody shimmers violently as the spirits transfer it strength into you.#n",ch,NULL,NULL,TO_CHAR);
                 act("#R$n #oappears to look stronger in the middle of the chaos.#n",ch,NULL,NULL,TO_ROOM);
              }
              else {
                      ch->move = UMAX( 0, ch->move - number_range(100000,300000) );
                      ch->mana = UMAX( 0, ch->mana - number_range(100000,300000) );

                      if ( ch->hit > 74999 )
                         ch->hit = UMAX( 1, ch->hit - number_range(25000,75000) );

                      act("Your body turns pale as demonic energy flies through you, draining your will to fight.",ch,NULL,NULL,TO_CHAR);
                      act("$n's body turns pale as demonic energy flies through $m, draining $s will to fight.",ch,NULL,NULL,TO_ROOM);

                      if ( IS_NPC( ch ) && number_percent() > 89 && ( ch->pIndexData->vnum < 550 ||
                           ch->pIndexData->vnum > 570 ) )
                         {
                            raw_kill( ch );
                            return;
                         }
                   }
        }

     if ( IS_SET( ch->monkstuff, MONK_DEATH ) )
        {
           if ( ch->hit > 50000 )
              {
                 ch->hit -= 50000;
                 ch->mana = UMAX( 0, ch->mana - 50000 );
                 ch->move = UMAX( 0, ch->move - 50000 );
                 stc("You writhe in agony as magical energies tear you asunder.\n\r",ch);
                 act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM);
              }
              else {
                      stc("You feel the magical forces leave your body.\n\r",ch);
                      act("The magical forces leave $n's body.",ch,NULL,NULL,TO_ROOM);
                      REMOVE_BIT( ch->monkstuff, MONK_DEATH );
                   }
	  }

      if ( is_affected( ch, skill_lookup( "cloud kill" ) ) )
         {
            if ( number_percent() > 50 )
               {
                  act( "You cough and gag as the fumes have your face all green and tormented.", ch, NULL, NULL, TO_CHAR );
                  act( "$n coughs and gags as the fumes have $s face all green and tormented.", ch, NULL, NULL, TO_ROOM );
               }

            ch->mana = UMAX( 0, ch->mana - 200000 );

            if ( IS_CLASS( ch, CLASS_DUERGAR ) )
               ch->hit = UMIN( ch->max_hit, ch->hit + ( 100000 * UMAX( 1, ch->pkill / 10 ) ) );
               else ch->hit -= 100000;

            update_pos( ch );

            if ( ch->position < POS_STUNNED )
               affect_strip( ch, skill_lookup("cloud kill") );

            if ( ch->position == POS_DEAD )
               {
                  raw_kill( ch );
                  return;
               }
         }

     if ( is_affected( ch, skill_lookup("drow poison") ) )
        {
            if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
               affect_strip( ch, skill_lookup("drow poison") );
               else if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) )
                       affect_strip( ch, skill_lookup("drow poison") );
               else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DRAGON) && IS_DRAGONPOWER(ch,DRAGON_IMMUNITY) )
                       affect_strip( ch, skill_lookup("drow poison") );
               else if ( ( IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_SHADOW_WAR) ) && ch->ctimer[8] > 0 )
                       affect_strip( ch, skill_lookup("drow poison") );
               else {
                       if ( number_percent() > 60 )
                          {
                             stc("You scream loudly as the mystical poison devours your life and soul.\n\r",ch);
                             act("$n screams loudly as a mystical poison rips apart $s mortal body.",ch,NULL,NULL,TO_ROOM);
                          }

                       if ( !IS_CLASS( ch, CLASS_DUERGAR ) )
                          {
                             ch->hit -= 50000;
 
                             update_pos( ch );

                             if ( ch->position < POS_STUNNED )
                                affect_strip( ch, skill_lookup("drow poison") );
                          }
                          else ch->hit = UMIN( ch->max_hit, ch->hit + ( 50000 * UMIN( 1, ch->pkill / 10 ) ) );
                    }
        }

     if ( IS_CLASS( ch, CLASS_DUERGAR ) && ch->position > POS_MORTAL )
        {
           if ( IS_SET( ch->in_room->area->bits, ROOM_TOTAL_DARKNESS ) )
              ch->hit = UMIN( ch->max_hit, ch->hit + ( 50000 * UMAX( 1, ch->pkill / 10 ) ) );

           if ( weather_info.sky == SKY_ECLIPSE )
              ch->hit = UMIN( ch->max_hit, ch->hit + 100000 );

           if ( IS_SET( ch->in_room->area->bits, ROOM_ANTI_LIGHT ) )
              ch->hit = UMIN( ch->max_hit, ch->hit + 150000 );
        }

     if ( IS_SET(ch->more, MORE_BLOOD) )
        {
           if ( ch->hit > 50000 )
              {
                 ch->hit -= 50000;
                 ch->move = UMAX( 0, ch->move - 200000 );

                 if ( number_percent() > 60 )
                    {
                       stc("You scream in agony as a spurrel of blood sprays from your wounds.\n\r",ch);
                       act("$n screams in agony as a spurrel of blood sprays from $s wounds.",ch,NULL,NULL,TO_ROOM);
                    }
              }
              else {
                      stc("Your wounds is healed as you pull the devastating weapon from your body.\n\r",ch);
                      act("$n removes the devastating stiletto from $s body.",ch,NULL,NULL,TO_ROOM);
                      REMOVE_BIT(ch->more, MORE_BLOOD);
                   }
	  }

     if ( is_affected( ch, skill_lookup("last prayer") ) && ch->hit > 100000 )
        ch->hit -= 100000;

     if ( IS_SET( ch->more, MORE_HAMMER ) && ch->fighting != NULL && IS_CLASS( ch->fighting, CLASS_DUERGAR ) )
        {
           if ( ch->hit > 75000 )
              {
                 ch->hit -= 75000;
                 stc("You groan in pain as the #edemonic #gaxe#n leeches your life force.\n\r",ch);
                 act("$n groans in pain as the #edemonic #gaxe#n leeches $s life force.",ch,NULL,NULL,TO_ROOM);
                 ch->fighting->hit = UMIN( ch->fighting->max_hit, ch->fighting->hit + 75000 );
              }
              else REMOVE_BIT( ch->more, MORE_HAMMER );
        }

      if ( IS_SET( ch->in_room->area->bits, MAGMA ) && ch->hit > 200000 && number_percent() > 79 )
         {
            act("#RYou scream as the hot energy seers your body!",ch,NULL,NULL,TO_CHAR);
            act("#R$n #Rscreams as the hot energy seers $s body!",ch,NULL,NULL,TO_ROOM);

            if ( IS_CLASS( ch, CLASS_PHOENIX ) )
               ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );
               else if ( IS_CLASS( ch, CLASS_SNOW_ELF ) )
                       ch->hit -= 200000;
                       else ch->hit -= 100000;
         }
         else if ( IS_SET( ch->in_room->room_flags, ROOM_FLAME ) && ch->hit > 100000 && number_percent() > 79 )
         {
            act("#RYou scream as the fiery flames eats through your body!",ch,NULL,NULL,TO_CHAR);
            act("#R$n #Rscreams as the fiery flames eat through $s body!",ch,NULL,NULL,TO_ROOM);

            if ( !IS_AFFECTED( ch, AFF_FLAMING ) )
               SET_BIT( ch->affected_by, AFF_FLAMING );

            if ( IS_CLASS( ch, CLASS_PHOENIX ) )
               ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );
               else if ( IS_CLASS( ch, CLASS_SNOW_ELF ) )
                       ch->hit -= 100000;
                       else ch->hit -= 50000;
         }

      update_pos( ch );
}

void check_arena( CHAR_DATA *ch )
{
   if ( arena == FIGHT_BEGINS )
      show_start( ch );

   if ( arena == FIGHT_ROUNDS )
      show_round( ch );

   if ( arena == FIGHT_POSE )
      show_poses( ch );

   if ( arena == FIGHT_BEGIN )
      do_announce_fight( ch );

   if ( arena == FIGHT_ROUND )
      do_announce_round( ch );

   if ( arena == FIGHT_ANNOUNCE && IS_SET( ch->arena, AFF2_INARENA ) )
      undo_fight( ch ); 

   if ( IS_NPC( ch ) )
      return;

   if ( !IS_IMMORTAL(ch) && !IS_EXTRA(ch,EXTRA_OSWITCH) && ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 ) && !IS_SET(ch->arena, AFF2_INARENA) 
        && ch->pcdata->team == 0 )
      {
         stc("Stay out of the Arena!\n\r",ch);
         char_from_room(ch);
         char_to_room(ch, get_room_index(632));
         do_look(ch,"");
      }

   if ( !IS_IMMORTAL(ch) && !IS_EXTRA(ch,EXTRA_OSWITCH) && ( ch->in_room->vnum < 550 || ch->in_room->vnum >= 630 ) && ( IS_SET(ch->arena, AFF2_INARENA) 
        || ch->pcdata->team > 0 ) )
      {
         stc("Your battle isn't finish. Get your ASS back in there boy!!!\n\r",ch);
         char_from_room(ch);
         char_to_room(ch, get_room_index(550));
         do_look(ch,"");
      }

   if ( ch->arena_timer > 0 && ( arena == FIGHT_START || arena == FIGHT_BATTLEOVER || arena == FIGHT_PRE ) )
      {
         ch->arena_timer--;
         if ( ch->arena_timer < 1 )
            {
               if ( ch->round > 0 )
                  {
                     stop_fighting( ch, TRUE );

                     if ( arena != FIGHT_TIMEOVER )
                        {
                           arena = FIGHT_TIMEOVER;
                           do_tinfo( NULL, NULL, "#RTime #eOver..." );
                        }

                     ch->wait = 24;
                  }
                  else if ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) )
                          {
                              if ( arena != FIGHT_BATTLEOVER )
                                 arena = FIGHT_BATTLEOVER;

                              if ( !IS_SET( ch->arena, AFF2_WINNER ) && ch->challenged != NULL
                                   && !IS_SET( ch->challenged->arena, AFF2_WINNER ) )
                                 {
                                    if ( ( ch->challenged->hit * 100 / ch->challenged->max_hit ) > ( ch->hit * 100 / ch->max_hit ) )
                                       SET_BIT( ch->challenged->arena, AFF2_WINNER );
                                       else SET_BIT( ch->arena, AFF2_WINNER );

                                    arena_wait = 1;
                                 }

                              stc( "\n\r", ch );
                              stc( "#w********  ***  ***       ***  ********    #o*********  ***       ***  ********  ******\n\r", ch );
                              stc( "#w********  ***  *****   *****  ********    #o*********  ***       ***  ********  **   **\n\r", ch );
                              stc( "#w   **     ***  *** ** ** ***  **          #o**     **   ***     ***   **        **   **\n\r", ch );
                              stc( "#w   **     ***  ***  ***  ***  ****        #o**     **   ***     ***   ****      ******\n\r", ch );
                              stc( "#w   **     ***  ***   *   ***  ****        #o**     **    ***   ***    ****      **  ***\n\r", ch );
                              stc( "#w   **     ***  ***       ***  **          #o**     **     *** ***     **        **   ***\n\r", ch );
                              stc( "#w   **     ***  ***       ***  ********    #o*********      *****      ********  **    ***\n\r", ch );
                              stc( "#w   **     ***  ***       ***  ********    #o*********       ***       ********  **    ***\n\r\n\r", ch );
                          }
                  else if ( IS_SET(ch->arena, AFF2_INARENA) || IS_SET(ch->arena,AFF2_CHALLENGED) || IS_SET(ch->arena, AFF2_CHALLENGER) )
                          undo_arena(ch); 
            }
      }
}
