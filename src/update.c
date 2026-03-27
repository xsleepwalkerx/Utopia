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
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * Local functions.
 */
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void  room_update     args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void  ww_update       args( ( void ) );
void  embrace_update  args((void));
void	werewolf_regen	args ( ( CHAR_DATA *ch, int multiplier) );

void update_drunks				args ((CHAR_DATA *ch));
void sex_update					args ((CHAR_DATA *ch));
void update_angel                               args ((CHAR_DATA *ch));
void update_demon				args ((CHAR_DATA *ch));
void update_drow				args ((CHAR_DATA *ch));
void update_dragon				args ((CHAR_DATA *ch));
void update_kingdom				args ((CHAR_DATA *ch));
void update_arti_regen				args ((CHAR_DATA *ch));
void regen_limb					args ((CHAR_DATA *ch));
void update_safe_powers				args ((CHAR_DATA *ch));
void update_knight				args ((CHAR_DATA *ch));

const sh_int  revdir [6] =
{
    2, 3, 0, 1, 5, 4
};

void put_wall(ROOM_INDEX_DATA *room, int dir, int wall)
{
    int timer;

    if ( room->exit[dir]->to_room == NULL )
       return;

    timer = dice(2, 2);

    switch (dir)
    {
	case DIR_NORTH :
	    SET_RTIMER(room, RTIMER_WALL_NORTH, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_SOUTH, timer);
	break;
	case DIR_EAST :
	    SET_RTIMER(room, RTIMER_WALL_EAST, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_WEST, timer);
	break;
	case DIR_SOUTH :
	    SET_RTIMER(room, RTIMER_WALL_SOUTH, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_NORTH, timer);
	break;
	case DIR_WEST :
	    SET_RTIMER(room, RTIMER_WALL_WEST, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_EAST, timer);
	break;
	case DIR_UP :
	    SET_RTIMER(room, RTIMER_WALL_UP, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_DOWN, timer);
	break;
	case DIR_DOWN :
	    SET_RTIMER(room, RTIMER_WALL_DOWN, timer);
	    SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_UP, timer);
	break;
    }   
    room->exit[dir]->exit_info = wall;

}

void make_wall(ROOM_INDEX_DATA *room, int dir, int wall)
{
    int was;
    bool makewall = TRUE;

    if (dir < 0 || dir > 5)
    {
	bug("Invalid direction (create wall)", 0);
	return;
    }

    if (room->exit[dir] == NULL)
    {
	bug ("Cant make wall!!!!", 0);
	return;
    }

    was = room->exit[dir]->exit_info;

/*    if ( was == wall )
       return; */

    if (was == EX_ICE_WALL && wall == EX_FIRE_WALL)
    {
	room_message(room, "The wall of ice bursts into flames.");
	room_message(room->exit[dir]->to_room, "The wall of ice bursts into flames.");
    }

    if (was == EX_FIRE_WALL && wall == EX_ICE_WALL)
    {
	room_message(room, "The wall of flames flickers blue for a second.");
	room_message(room->exit[dir]->to_room, "The wall of flames flickers blue for a second.");
	makewall = FALSE;
    }

    if (makewall) put_wall(room, dir, wall);

    if (room->exit[dir]->to_room == NULL)
    {
	bug("Exit doesnt lead anywhere (create wall).", 0);
	return;
    }

    if (room->exit[dir]->to_room->exit[revdir[dir]] == NULL)
    {
	bug("Reverse Exit doesnt lead anywhere (create wall).", 0);
	return;
    }

    if (makewall) put_wall(room->exit[dir]->to_room, revdir[dir], wall);
}

ROOM_INDEX_DATA * locate_obj (OBJ_DATA *obj)
{
    if (obj->in_obj != NULL)
	return locate_obj(obj->in_obj);

    if (obj->carried_by != NULL)
	return obj->carried_by->in_room;

    return obj->in_room;
}

void gain_exp( CHAR_DATA *ch, int gain )
{
    CHAR_DATA *mount = NULL;
    CHAR_DATA *master = NULL;
    if ( IS_NPC(ch) && (mount = ch->mount) != NULL && !IS_NPC(mount))
    {
	if ( (master = ch->master) == NULL || master != mount )
	    mount->exp += gain;
    }

    if ( !IS_NPC(ch) )
	ch->exp += gain;
    return;
}
/*
 * Regeneration stuff.
 */
void hit_gain( CHAR_DATA *ch, int heal )
{
    int old_hp = ch->hit;
    int wis = get_curr_wis( ch ) + 1;

    if ( wis > 9 )
       heal *= UMIN( 2.0, ( wis * 0.01 ) + 1.0 );

    if ( doubleheal > 0 )
       heal *= 2;

    if ( IS_SET( ch->cmbt[5], SKL2_HSOUL ) )
       heal += ( ( heal / 300000 ) * 200000 );

    if ( IS_AFFECTED( ch, AFF_POISON ) || IS_SET( ch->more, MORE_POISON ) )
       {
          if ( IS_CLASS( ch, CLASS_DUERGAR ) )
             heal *= 2;
             else heal *= 0.75;
       }

    if ( !IS_CLASS( ch, CLASS_PHOENIX ) && IS_AFFECTED( ch, AFF_FLAMING ) )
	 heal *= 0.9;

    if ( IS_SET( ch->extra, EXTRA_LTOUCH ) )
       heal /= 3;

    if ( !IS_NPC( ch ) && ch->ctimer[15] > 0 )
       heal /= 4;

    if ( ch->revregen > 0 || ( !IS_NPC( ch ) && IS_SET( ch->extra, EXTRA_ZOMBIE ) ) )
       heal *= -1;

    if ( IS_NPC( ch ) )
       ch->hit = URANGE( 1, ch->hit + heal, ch->max_hit );
       else ch->hit = UMIN( ch->max_hit, ch->hit + heal );
    
    update_pos( ch );

    if ( ch->topmode == 100 && ch->hit >= ch->max_hit * 0.67
         && old_hp < ch->max_hit * 0.67 )
       {
          stc( "#wT#e.#CO#e.#RP#e. #P#zIN#n#g!!!#n\n\r", ch );
          act( "#c$n #gclenches $s fists as $e #P#zactivates#n #g$s #wT#e.#CO#e.#RP#e. #gmode!#n", ch, NULL, NULL, TO_ROOM );
       }
}

void mana_gain( CHAR_DATA *ch, int heal )
{
    if ( IS_SET( ch->monkstuff, MONK_DEATH ) )
       {
          if ( IS_CLASS( ch, CLASS_DEMON ) )
             heal *= 2;
             else heal *= 0.75;
       }

    if ( !IS_CLASS( ch, CLASS_DRAGON_KNIGHT ) && ch->in_room != NULL && IS_SET( ch->in_room->room_flags, ROOM_WHIRLWIND ) )
	 heal *= 0.5;

    ch->mana = UMIN( ch->max_mana, ch->mana + heal );
}

void move_gain( CHAR_DATA *ch, int heal )
{
    if ( IS_AFFECTED( ch, AFF_POISON ) || IS_SET( ch->more, MORE_POISON ) )
       {
          if ( IS_CLASS( ch, CLASS_DUERGAR ) )
             heal *= 2;
             else heal *= 0.75;
       }

    if ( !IS_CLASS( ch, CLASS_PHOENIX ) && IS_AFFECTED( ch, AFF_FLAMING ) )
	 heal *= 0.9;

    ch->move = UMIN( ch->max_move, ch->move + heal );
}


void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( value == 0 || IS_NPC(ch) )
	return;

    if ( !IS_NPC(ch) && IS_HERO(ch) && !IS_CLASS(ch, CLASS_VAMPIRE)
	&& iCond != COND_DRUNK )
	return;

    condition				= ch->pcdata->condition[iCond];
    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) )
	{
			ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );
	}
	else
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 10000 );

    if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
	    {
		send_to_char( "You are REALLY hungry.\n\r",  ch );
		act( "You hear $n's stomach rumbling.", ch, NULL, NULL, TO_ROOM );
	    }
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE) ) 
		send_to_char( "You are REALLY thirsty.\n\r", ch );
	    else if (ch->hit > 0)
	    {
		send_to_char( "You are DYING from lack of blood!\n\r", ch );
		act( "$n gets a hungry look in $s eyes.", ch, NULL, NULL, TO_ROOM );
		ch->hit = ch->hit - number_range(2,5);
		if (number_percent() <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }
    else if ( ch->pcdata->condition[iCond] < 10 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE) ) 
		send_to_char( "You feel hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE) ) 
		send_to_char( "You feel thirsty.\n\r", ch );
	    else
	    {
		send_to_char( "You crave blood.\n\r", ch );
		if (number_range(1,1000) <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (number_percent() > (ch->pcdata->condition[COND_THIRST]+75)
		    && !IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;
	}
    }

    return;
}

/*
 * Mob autonomous action.
 * This function takes 25% to % of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    int door;
    EXIT_DATA *pexit;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
		ch_next = ch->next;

		if ( ch->in_room == NULL ) continue;

            update_timers( ch );

            if ( ch->position == POS_DEAD )
               {
                  raw_kill( ch );
                  continue;
               }

            if ( IS_NPC( ch ) && ch->pIndexData->vnum >= 550 && ch->pIndexData->vnum <= 570
                && ch->fighting == NULL && ch->challenged != NULL && arena == FIGHT_START 
                && ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 && ( number_percent() > 39
                || ch->fight_timer == 0 ) && ch->challenged->air_timer == 0 && ch->air_timer == 0 )
               {
                  char_from_room( ch );
                  char_to_room( ch, ch->challenged->in_room );
                  multi_hit( ch, ch->challenged, TYPE_UNDEFINED );
               }

		if ( !IS_NPC(ch) )
		{
               int new_total = conaneffect;

               if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->pkill > 19 )
                  new_total += 1000000;

               if ( IS_CLASS(ch, CLASS_FIEND) && IS_SET( ch->more, MORE_WALL ) )
                  new_total += 1000000;

               if ( IS_SET( ch->more3, MORE3_DRAGON ) )
                  new_total += 750000;

               if ( IS_SET(ch->more2, MORE2_TIE) && ch->pcdata->capture_timer == 0 && ch->hit >= ch->max_hit )
                  REMOVE_BIT(ch->more2, MORE2_TIE);

               if ( ch->mort_timer > 0 && ch->hit >= ch->max_hit / 4 )
                  ch->mort_timer = 0;

               update_drunks(ch);
               sex_update(ch);

               if ( ch->hit < -new_total - 10 )
                  ch->hit = -new_total - 10;

               if ( ch->hit > 0 - new_total )
                  {
                     if ( IS_ITEMAFF(ch, ITEMA_REGEN) )
                        {
                           update_drow(ch);
                           update_drow(ch);
                        }
                        else if ( IS_CLASS( ch, CLASS_DRAGON ) )
                                {
                                   if ( ch->agg == 0 )
                                      update_drow(ch);

                                   update_drow(ch);
                                }
                                else update_drow(ch);

                     if ( ( IS_CLASS( ch, CLASS_PHOENIX ) || IS_SET( ch->more2, MORE2_PHOENIX ) 
                          || is_affected(ch, skill_lookup("darkside") ) ) && ch->agg == 0 )
                        update_drow(ch);
                  } 
                  else {
                          ch->hit += number_range(0,1);

                          if ( ch->hit > 0 - new_total )
                             update_pos( ch );
                       }
            }
		else 
		{
                  if ( ch->agg == 0 && is_affected(ch, skill_lookup("darkside") ) )
                     update_drow(ch);

                  update_drow(ch);

                  /* Wander */
                 if ( !IS_SET(ch->act, ACT_SENTINEL) && ( door = number_bits( 5 ) ) <= 5 && ch->in_room != NULL
                      && ( pexit = ch->in_room->exit[door] ) != NULL && pexit->to_room != NULL
                      && !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)
                      && !( pexit->to_room->area != ch->in_room->area ) && ch->position == POS_STANDING )
                    move_char( ch, door );

			if ( IS_AFFECTED(ch, AFF_CHARM) ) continue;
			/* Examine call for special procedure */
               if ( ch->spec_fun != 0 )
			{
				if ( (*ch->spec_fun) ( ch ) )
					continue;
				if (ch == NULL) continue;
			}
		}
    }
}



/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch = NULL;
    int diff;
    bool char_up;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  6:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "The day has begun.\n\r" );
	break;

    case  7:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The sun rises in the east.\n\r" );
	break;

    case 18:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The sun slowly disappears in the west.\n\r" );
	break;

    case 19:
        weather_info.sunlight = SUN_DARK;
	strcat( buf, "The night has begun.\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;

      if ( dungeon_timer > 0 )
         {
            if ( --dungeon_timer == 0 )
               do_info( NULL, "#eThe #Cgates #eto #RHell #ehas been closed...#n" );
         }

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    char_up = FALSE;
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   (ch = d->character) != NULL
	    &&  !IS_NPC(ch))
	    {
            if ( IS_SET(ch->more3, MORE3_DAMNATION) )
               {
                  REMOVE_BIT(ch->more3, MORE3_DAMNATION);
                  stc( "The psionic shield around youe body vanishes.\n\r", ch );
               }

		send_to_char( "You hear a clock in the distance strike midnight.\n\r",ch);

                if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
                {
                send_to_char("The silence leaves the room.\n\r",ch);
                act("The silence leaves the room.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_SILENCE);
                
                }
 	     } 
                 
	    
	}
	break;
    }

    if ( time_info.day   >= 7 )
    {
	time_info.day = 0;
	time_info.month++;

      if ( is_dungeon > -1 )
         {
            if ( ++is_dungeon == 6 )
               {
                  do_info( NULL, "#eThe #CDungeon #eof #RDeath #eis now closed...#n" );
                  do_info( NULL, "#eFor #wall #ewho had tried their #R#zbest#n#e... Await for the #creturn#e...#n" );
                  clear_dungeon();
               }
         }

      if ( ( time_info.month == 0 || time_info.month == 6 || time_info.month == 12 )
           && is_dungeon == -1 )
         create_dungeon();
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    diff = number_percent() > 50 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    if ( time_info.month >= 0 && time_info.month <= 3 )
       {
          switch ( weather_info.sky )
          {
             default: if ( ( time_info.hour % 12 ) == 0 )
                         {
                            weather_info.sky = SKY_CLOUDLESS;
                            strcat( buf, "#CThe sky suddenly brightens as the beautiful #ysun #Cglaze your face.#n\n\r" );
                         }
                      break;

             case SKY_CLOUDLESS: if ( weather_info.mmhg <  990 || ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
                                    {
                                       strcat( buf, "#CThe sky is getting #wcloudy#C.#n\n\r" );
                                       weather_info.sky = SKY_CLOUDY;
                                    }
                                 break;

             case SKY_CLOUDY: if ( weather_info.mmhg <  970 || ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
                                 {
                                    strcat( buf, "#wIt #Cstarts #wto #Csnow.#n\n\r" );
                                    weather_info.sky = SKY_SNOWY;
                                 }

                              if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
                                 {
                                    strcat( buf, "#CThe clouds disappear.#n\n\r" );
                                    weather_info.sky = SKY_CLOUDLESS;
                                 }
                              break;

             case SKY_SNOWY: if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
                                {
                                   strcat( buf, "#wA #Cfreezing #wwind shivers your bones as it now snows in a #R#zfury#n#w.#n\n\r" );
                                   weather_info.sky = SKY_ICESTORM;
                                }

                             if ( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
                                {
                                   strcat( buf, "#wThe #Csnowing #whas stopped.#n\n\r" );
                                   weather_info.sky = SKY_CLOUDY;
                                }
                             break;

             case SKY_ICESTORM: if ( weather_info.mmhg > 1010 || ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
                                   {
                                      strcat( buf, "#wThe #Cicestorm #whas stopped.#n\n\r" );
                                      weather_info.sky = SKY_SNOWY;
                                      break;
                                   }
                                break;
          }
       }

    if ( time_info.month >= 4 && time_info.month <= 11 )
       {
          switch ( weather_info.sky )
          {
             default: if ( ( time_info.hour % 12 ) == 0 )
                         {
                            weather_info.sky = SKY_CLOUDLESS;
                            strcat( buf, "#CThe sky suddenly brightens as the beautiful #ysun #Cglaze your face.#n\n\r" );
                         }
                      break;

             case SKY_CLOUDLESS: if ( weather_info.mmhg <  990 || ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
                                    {
                                       strcat( buf, "#CThe sky is getting #wcloudy#C.#n\n\r" );
                                       weather_info.sky = SKY_CLOUDY;
                                    }
                                 break;

             case SKY_CLOUDY: if ( weather_info.mmhg <  970 || ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
                                 {
                                    strcat( buf, "#pIt #Lstarts #pto #Lrain.#n\n\r" );
                                    weather_info.sky = SKY_RAINING;
                                 }

                              if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
                                 {
                                    strcat( buf, "#CThe clouds disappear.#n\n\r" );
                                    weather_info.sky = SKY_CLOUDLESS;
                                 }
                              break;

             case SKY_RAINING: if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
                                  {
                                     strcat( buf, "#yLightning #w#zflashes#n #oin the sky.#n\n\r" );
                                     weather_info.sky = SKY_LIGHTNING;
                                  }

                               if ( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
                                  {
                                     strcat( buf, "#pThe #Lrain #pstopped.#n\n\r" );
                                     weather_info.sky = SKY_CLOUDY;
                                  }
                               break;

             case SKY_LIGHTNING: if ( weather_info.mmhg > 1010 || ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
                                    {
                                       strcat( buf, "#oThe #ylightning #ohas stopped.#n\n\r" );
                                       weather_info.sky = SKY_RAINING;
                                       break;
                                    }
                                 break;
          }
       }

    if ( time_info.month >= 12 && time_info.month <= 16 )
       {
          switch ( weather_info.sky )
          {
             default: if ( ( time_info.hour % 12 ) == 0 )
                         {
                            weather_info.sky = SKY_CLOUDLESS;
                            strcat( buf, "#CThe sky suddenly brightens as the beautiful #ysun #Cglaze your face.#n\n\r" );
                         }
                      break;

             case SKY_CLOUDLESS: if ( weather_info.mmhg <  990 || ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
                                    {
                                       strcat( buf, "#CThe sky is getting #wcloudy#C.#n\n\r" );
                                       weather_info.sky = SKY_CLOUDY;
                                    }
                                 break;

             case SKY_CLOUDY: if ( weather_info.mmhg <  970 || ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
                                 {
                                    strcat( buf, "#eThe #ysun #eis suddenly engulfed by a haunting moon.#n\n\r" );
                                    weather_info.sky = SKY_ECLIPSE;
                                 }

                              if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
                                 {
                                    strcat( buf, "#CThe clouds disappear.#n\n\r" );
                                    weather_info.sky = SKY_CLOUDLESS;
                                 }
                              break;

             case SKY_ECLIPSE: if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
                                  {
                                     strcat( buf, "#eThe doors to #Rhell #ehas been opened... and the #wend #eis near.#n\n\r" );
                                     weather_info.sky = SKY_APOCALYPSE;
                                  }

                               if ( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
                                  {
                                     strcat( buf, "#eThe #ysun #eis now #wvisible #eagain.#n\n\r" );
                                     weather_info.sky = SKY_CLOUDY;
                                  }
                               break;

             case SKY_APOCALYPSE: if ( weather_info.mmhg > 1010 || ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
                                     {
                                        strcat( buf, "#eThe doors to #Rhell #ehas been closed... thus evil is #Rvanquished #efor now.#n\n\r" );
                                        weather_info.sky = SKY_ECLIPSE;
                                        break;
                                     }
                                   break;
          }
       }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}

void beware_lightning( )
{ 
   int dam = 0; 
   CHAR_DATA *victim = NULL, *temp = NULL; 
   char buf[256]; 
   DESCRIPTOR_DATA *d;

   if ( !(weather_info.sky == SKY_LIGHTNING) )
      return;

   if ( number_percent() < 70 )
      return; /* Bolt only 30% of time */ 

   /* nobody targeted 10% */ 
   if ( number_percent() > 89 ) 
      { 
         for ( d = descriptor_list; d != NULL; d = d->next )
         {
            if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
                 && IS_OUTSIDE(d->character) && IS_AWAKE(d->character) )
               send_to_char( "#oYou hear the #wclap #oof distant #ythunder#o.#n\n\r", d->character );
         }
        
         return; 
       }

   /* PCs only */ 
   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected != CON_PLAYING || d->character == NULL || !IS_OUTSIDE( d->character )
           || IS_IMMORTAL( d->character ) || d->character->in_room->vnum < 831 )
         continue;

      victim = d->character;

      if ( number_percent() > 89 ) 
         { 
            /* damage routine here */ 
            dam = number_range( 500000, 1500000 );

            if ( !IS_CLASS( victim, CLASS_WISP ) )
               {
                  URANGE( -10, victim->hit - is_elemental( ATTACK_THUNDER, victim, dam ), victim->max_hit );
                  victim->wait = number_range( 12, 24 );
               }
               else victim->hit = UMIN( victim->max_hit, victim->hit + 2000000 );

            act("#w#zKAZAK!#n #oa #ylightning #obolt hits $n. You hear a #esick #Rsizzle#o.#n", victim, NULL, NULL, TO_ROOM); 
            act("#w#zKAZAK!#n #oa #ylightning #obolt hits you. You hear a #esick #Rsizzle#o.#n", victim, NULL, NULL, TO_CHAR); 

            if ( dam > victim->max_hit / 2)
               act("That Really did #R#zHURT#n!", victim, NULL, NULL, TO_CHAR); 

            for ( d = descriptor_list; d != NULL; d = d->next )
            {
               if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
                    && IS_OUTSIDE(d->character) && IS_AWAKE(d->character) 
                    && d->character->in_room != victim->in_room )
                  send_to_char( "#w#zBOOM!!!#n #oYou hear a #wSICKENING #osizzle as someone is #ystruck#o.#n\n\r", d->character );
            }

            update_pos(victim); 

            switch ( victim->position )
            {
               case POS_MORTAL: act("$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM); 
                                act("You are mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_CHAR); 
                                break; 

               case POS_INCAP: act("$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM); 
                               act("You are incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_CHAR); 
                               break; 

               case POS_STUNNED: act("$n is stunned, but will probably regain consciousness.", victim, NULL, NULL, TO_ROOM); 
                                 act("You're stunned, but will probably regain consciousness.", victim, NULL, NULL, TO_CHAR); 
                                 break; 

               case POS_DEAD: act("$n is DEAD! #RR#e.#RI#e.#RP#e...#n", victim, NULL, NULL, TO_ROOM); 
                              act("You have been killed!!!", victim, NULL, NULL, TO_CHAR); 
                              break; 

               default: break;
            }

            if ( victim->position == POS_DEAD ) 
               { 
                  sprintf(buf, "%s killed by a Thunderstorm.", victim->pcdata->switchname ); 
                  log_string(buf); 
                  raw_kill(victim); 
               }
               
            return;
         }
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
            && IS_OUTSIDE(d->character) && IS_AWAKE(d->character) )
          send_to_char( "#w#zBOOM!!!#n #oYou hear a #wclap #oof #ythunder#o nearby.#n\n\r", d->character );
    }
}

void beware_icestorm( )
{ 
   int dam = 0; 
   CHAR_DATA *victim = NULL, *temp = NULL; 
   char buf[256]; 
   DESCRIPTOR_DATA *d;

   if ( weather_info.sky != SKY_ICESTORM )
      return;

   if ( number_percent() < 70 )
      return; /* strikes only 30% of time */ 

   /* nobody targeted 10% */ 
   if ( number_percent() > 89 ) 
      { 
         for ( d = descriptor_list; d != NULL; d = d->next )
         {
            if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
                 && IS_OUTSIDE(d->character) && IS_AWAKE(d->character) )
               send_to_char( "#wYou #Cshiver #win #Ragony #was the deadly breeze #Cfreezes #wthe vicinity.#n\n\r", d->character );
         }
        
         return; 
       }

   /* PCs only */ 
   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected != CON_PLAYING || d->character == NULL || !IS_OUTSIDE( d->character )
           || IS_IMMORTAL( d->character ) || d->character->in_room->vnum < 831 )
         continue;

      victim = d->character;

      if ( number_percent() > 89 ) 
         { 
            /* damage routine here */ 
            dam = number_range( 500000, 1500000 );

            if ( !IS_CLASS( victim, CLASS_SNOW_ELF ) && !IS_SET( victim->more, MORE_FROZEN ) )
               {
                  victim->freeze_timer = number_range( 1, 2 );
                  SET_BIT( victim->more, MORE_FROZEN );
                  URANGE( -10, victim->hit - is_elemental( ATTACK_ICE, victim, dam ), victim->max_hit );
               }
               else victim->hit = UMIN( victim->max_hit, victim->hit + 2000000 );

            act("#wCRACKLE! #CShards #wof #Cice #wsuddenly encases $n. You hear a soft #yhollow#w.#n", victim, NULL, NULL, TO_ROOM); 
            act("#wCRACKLE! #CShards #wof #Cice #wsuddenly encases you. You hear a soft #yhollow#w.#n", victim, NULL, NULL, TO_CHAR); 

            for ( d = descriptor_list; d != NULL; d = d->next )
            {
               if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
                    && IS_OUTSIDE(d->character) && IS_AWAKE(d->character) 
                    && d->character->in_room != victim->in_room )
                  send_to_char( "#wCRACKLE!!! You hear the #Ccrackling #wsound of #ysomeone #wbeing #Cfrozen#w.#n\n\r", d->character );
            }

            update_pos(victim); 

            switch ( victim->position )
            {
               case POS_MORTAL: act("$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM); 
                                act("You are mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_CHAR); 
                                break; 

               case POS_INCAP: act("$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM); 
                               act("You are incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_CHAR); 
                               break; 

               case POS_STUNNED: act("$n is stunned, but will probably regain consciousness.", victim, NULL, NULL, TO_ROOM); 
                                 act("You're stunned, but will probably regain consciousness.", victim, NULL, NULL, TO_CHAR); 
                                 break; 

               case POS_DEAD: act("$n is DEAD! #RR#e.#RI#e.#RP#e...#n", victim, NULL, NULL, TO_ROOM); 
                              act("You have been killed!!!", victim, NULL, NULL, TO_CHAR); 
                              break; 

               default: break;
            }

            if ( victim->position == POS_DEAD ) 
               { 
                  sprintf(buf, "%s killed by an Icestorm.", victim->pcdata->switchname ); 
                  log_string(buf); 
                  raw_kill(victim); 
               }
               
            return;
         }
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
       if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
            && IS_OUTSIDE(d->character) && IS_AWAKE(d->character) )
          send_to_char( "#wCRACKLE!!! You hear the #esickening #wsound of #Cice #wbreaking.#n\n\r", d->character );
    }
}

int is_wall(EXIT_DATA *ex)
{

    if (IS_SET(ex->exit_info, EX_ICE_WALL)) return 1;
    if (IS_SET(ex->exit_info, EX_FIRE_WALL)) return 2;
    if (IS_SET(ex->exit_info, EX_SWORD_WALL)) return 3;
    if (IS_SET(ex->exit_info, EX_PRISMATIC_WALL)) return 4;
    if (IS_SET(ex->exit_info, EX_IRON_WALL)) return 5;
    if (IS_SET(ex->exit_info, EX_MUSHROOM_WALL)) return 6;
    if (IS_SET(ex->exit_info, EX_CALTROP_WALL)) return 7;
    if (IS_SET(ex->exit_info, EX_ASH_WALL)) return 8;
    if (IS_SET(ex->exit_info, EX_ICE_SWORDS)) return 9;
    return 0;
}

const char * wall[MAX_WALL+1] = 
{
    "","wall of ice", "wall of fire", "wall of swords",
	"prismatic wall", "wall or iron", "wall of mushrooms",
	"wall of caltrops", "wall of ash"
};

/*
 * Update all rooooooms, like gore, poison clouds etc....
 */
void room_update( void )
{
    int i =0;
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *room;

    
    for ( room = room_list; room != NULL; room = room->next_room)
    {
        
        if (RTIMER(room, RTIMER_WALL_NORTH) == 1
	    && room->exit[DIR_NORTH] != NULL
	    && is_wall(room->exit[DIR_NORTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_NORTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_NORTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_SOUTH) == 1
	    && room->exit[DIR_SOUTH] != NULL
	    && is_wall(room->exit[DIR_SOUTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_SOUTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_SOUTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_EAST) == 1
	    && room->exit[DIR_EAST] != NULL
	    && is_wall(room->exit[DIR_EAST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_EAST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_EAST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_WEST) == 1
	    && room->exit[DIR_WEST] != NULL
	    && is_wall(room->exit[DIR_WEST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_WEST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_WEST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_UP) == 1
	    && room->exit[DIR_UP] != NULL
	    && is_wall(room->exit[DIR_UP])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_UP])]);
	    room_message(room, buf);
	    make_wall(room, DIR_UP, 0);
        }

        if (RTIMER(room, RTIMER_WALL_DOWN) == 1
	    && room->exit[DIR_DOWN] != NULL
	    && is_wall(room->exit[DIR_DOWN])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_DOWN])]);
	    room_message(room, buf);
	    make_wall(room, DIR_DOWN, 0);
        }


	if (RTIMER(room, RTIMER_STINKING_CLOUD) == 1)
	    room_message(room, "The poisonous vapours dissipate and clear.");

	if (RTIMER(room, RTIMER_HIDE_ROOM) == 1)
	    room_message(room, "The shroud leaves the room.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 2)
	    room_message(room, "The vapourous ghosts start howling insanely.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 3)
	    room_message(room, "The vapourous ghosts start moaning.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 1)
	    room_message(room, "The vapourous ghosts dissipate and vanish.");

	if (RTIMER(room, RTIMER_GLYPH_PROTECTION) == 1)
	    room_message(room, "The glyph of protection flares and vanishes.");

	if (RTIMER(room, RTIMER_SWARM_BEES) == 1)
	    room_message(room, "The bees fly away into the sky.");

	if (RTIMER(room, RTIMER_DISCORD) == 1)
	    room_message(room, "The banging and crashing stops.");

	if (RTIMER(room, RTIMER_SWARM_BATS) == 1)
	    room_message(room, "The bats flap away into the night.");

	if (RTIMER(room, RTIMER_SWARM_RATS) == 1)
	    room_message(room, "The rats scurry away into the floorboards.");
  
      if( RTIMER(room, RTIMER_SILENCE) < 0) room->tick_timer[i] = UMAX(room->tick_timer[i], 0);
      if (RTIMER(room, RTIMER_SILENCE) == 1)
	    room_message(room, "The silence disappates.");

	for (i = 0 ; i < MAX_RTIMER ; i++)
	    room->tick_timer[i] = UMAX(room->tick_timer[i] - 1, 0);

    }

    return;

}

void do_eipheal( int nocheck )
{
   CHAR_DATA *mob;
   int option = number_range( 1, 5 );

   if ( nocheck == 0 && eipheal == 0 && number_percent() < 50 )
      return;

   mob = get_mob( 102 );

   if ( mob == NULL )
      return;

   switch( option )
   {
       default:
       case 1: do_chat( mob, "Hoo hoo hoo... A good wind blows today. Lets try this..." );
               break;
       case 2: do_chat( mob, "Foolish mortals... The Heavenly Clan SHALL win!" );
               break;
       case 3: do_chat( mob, "Whose life should I destroy today?" );
               break;
       case 4: do_chat( mob, "Power is everlasting glory to the Heavenly Clan." );
               break;
       case 5: do_chat( mob, "I see your future... and your demise..." );
               break;
   }

   if ( eipheal > 0 && number_percent() < 65 )
      eipheal = 0;
      else eipheal = number_range( 1, 10 );

   switch( eipheal )
   {
       case 0: do_info( mob, "#PEipheal #ghas turned off his #z#yprevious#n #gspell... until next time...#n" );
               break;

       default:
       case 1: do_info( mob, "#PEipheal #ghas enhanced #w#zALL#n #gdamage by #C100%#g!!!#n" );
               break;

       case 2: do_info( mob, "#PEipheal #ghas enhanced #w#zALL#n #gprotection by #C100%#g!!!#n" );
               break;

       case 3: do_info( mob, "#PEipheal #ghas enhanced #w#zALL#n #gfocus priorities to #C100%#g!!!#n" );
               break;

       case 4: do_info( mob, "#PEipheal #ghates #wHoly attacks#g, thus he #e#znullifies#n #gthem!!!#n" );
               break;

       case 5: do_info( mob, "#PEipheal #ghates #RFire attacks#g, thus he #e#znullifies#n #gthem!!!#n" );
               break;

       case 6: do_info( mob, "#PEipheal #ghates #CCold attacks#g, thus he #e#znullifies#n #gthem!!!#n" );
               break;

       case 7: do_info( mob, "#PEipheal #ghates #yThunder attacks#g, thus he #e#znullifies#n #gthem!!!#n" );
               break;

       case 8: do_info( mob, "#PEipheal #ghates #oEarth attacks#g, thus he #e#znullifies#n #gthem!!!#n" );
               break;

       case 9: do_info( mob, "#PEipheal #ghas enhanced #o#zLunar#n #gattacks by #C100%#g!!!#n" );
               break;

       case 10: do_info( mob, "#PEipheal #ghas enhanced #w#zALL#n #gweaknesses by #C200%#g!!!#n" );
                break;
   }
}

void do_conan( int nocheck )
{
   CHAR_DATA *mob;
   int option = number_range( 1, 5 );

   if ( nocheck == 0 && conan == 0 && number_percent() < 50 )
      return;

   mob = get_mob( 105 );

   if ( mob == NULL )
      return;

   switch( option )
   {
       default:
       case 1: do_chat( mob, "MUUUUUUAAAAAAA!" );
               break;
       case 2: do_chat( mob, "I am INVINCIBLE!!!" );
               break;
       case 3: do_chat( mob, "Strike me with that puny weapon... I DARE you!" );
               break;
       case 4: do_chat( mob, "Who doesn't lust for more power?" );
               break;
       case 5: do_chat( mob, "Bloody WarGod, fill me with strength!" );
               break;
   }

   if ( conan > 0 && number_percent() < 65 )
      conan = 0;
      else conan = number_range( 1, 11 );

   switch( conan )
   {
       case 0: do_info( mob, "#oConan #ghas removed his #z#yprevious#n #gpower until next time.#n" );
               break;

       default:
       case 1: do_info( mob, "#oConan #ghas enhanced #w#zslash#n #gdamage by #C50%#g!!!#n" );
               break;

       case 2: do_info( mob, "#oConan #ghas enhanced #w#zpierce#n #gdamage by #C50%#g!!!#n" );
               break;

       case 3: do_info( mob, "#oConan #ghas enhanced #w#zblast#n #gdamage by #C50%#g!!!#n" );
               break;

       case 4: do_info( mob, "#oConan #ghas enhanced #w#zblunt#n #gdamage by #C50%#g!!!#n" );
               break;

       case 5: do_info( mob, "#oConan #ghas enhanced #w#zbeam#n #gdamage by #C50%#g!!!#n" );
               break;

       case 6: do_info( mob, "#oConan #ghas enhanced #w#zphysical#n #gdamage by #C25%#g!!!#n" );
               break;

       case 7: do_info( mob, "#oConan #ghas enhanced #w#zmagical#n #gdamage by #C25%#g!!!#n" );
               break;

       case 8: do_info( mob, "#oConan #ghas enhanced #w#zmanual#n #gblocking by #C25%#g!!!#n" );
               break;

       case 9: do_info( mob, "#oConan #ghas enhanced #w#zall#n #gcounters by #C25%#g!!!#n" );
               break;

       case 10: do_info( mob, "#oConan #ghas enhanced all #w#znormal#n #gsummon damage by #C200%#g!!!#n" );
                break;

       case 11: do_info( mob, "#oConan #ghas enhanced all #w#znatural#n #gdamage by #C100%#g!!!#n" );
                break;
   }
}

void do_conan_effect( void )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *mob, *mob2;
   int option = number_range( 102, 104 );
   DESCRIPTOR_DATA *d;

   mob = get_mob( 105 );

   if ( mob == NULL )
      return;

   mob2 = get_mob( option );

   if ( mob2 == NULL )
      return;

   switch( number_range( 1, 5 ) )
   {
       default:
       case 1: do_chat( mob, "Don't worry, I will save you." );
               break;
       case 2: do_chat( mob, "Here, come lean on my shoulders." );
               break;
       case 3: do_chat( mob, "You are gravely wounded, you shouldn't strain yourself." );
               break;
       case 4: do_chat( mob, "True warriors believe in honour so don't despair." );
               break;
       case 5: do_chat( mob, "Relax now, for the Legendary Conan is here." );
               break;
   }

   switch( option )
   {
       default:
       case 104: do_chat( mob2, "Thank you Conan." );
                 break;
       case 102: do_chat( mob2, "Curse you warrior." );
                 break;
       case 103: do_chat( mob2, "I don't need your sympathy..." );
                 break;
   }

   if ( conaneffect == 0 )
      {
         conaneffect = number_range( 500000, 2000000 );

         sprintf( buf, "#oConan#c has extended the #wlife#eforce #cof the vicinity by #w%d#c!", conaneffect );
      }
      else {
              conaneffect = 0;
              sprintf( buf, "#oConan#c has removed his blesses from the vicinity.", conaneffect );
           }

   do_info( mob, buf );
}

void do_jaggar_effect( void )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *mob;
   int option = number_range( 1, 5 );
   DESCRIPTOR_DATA *d;

   mob = get_mob( 103 );

   if ( mob == NULL )
      return;

   switch( option )
   {
       default:
       case 1: do_chat( mob, "..." );
               break;
       case 2: do_chat( mob, "More... Power..." );
               break;
       case 3: do_chat( mob, "Blessed Nino... Give me the heart to prevail..." );
               break;
       case 4: do_chat( mob, "Kill everyone that is against love..." );
               break;
       case 5: do_chat( mob, "One day... I will get my revenge..." );
               break;
   }

   if ( jaggareffect == 0 )
      {
         jaggareffect = number_range( 10, 40 );

         sprintf( buf, "#rJaggar#e has blessed the vicinity with a #w%d#e%% critical attack chance...", jaggareffect );
      }
      else {
              jaggareffect = 0;
              sprintf( buf, "#rJaggar#e has removed his blesses from the vicinity...", jaggareffect );
           }

   do_info( mob, buf );
}

void do_jaggar( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *mob;
   int option = number_range( 1, 5 );
   DESCRIPTOR_DATA *d;

   mob = get_mob( 103 );

   if ( mob == NULL )
      return;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL )
         stc( "\n\r", d->character );
   }

   switch( option )
   {
       default:
       case 1: do_chat( mob, "I am sorry Lady Nino..." );
               break;
       case 2: do_chat( mob, "To protect one loved ones... I will do anything..." );
               break;
       case 3: do_chat( mob, "To stand before the angel of death means..." );
               break;
       case 4: do_chat( mob, "The reborn protects its own..." );
               break;
       case 5: do_chat( mob, "Don't cry young one... I promise to make this pleasant..." );
               break;
   }

   act( "A cloak man suddenly leap in front of you as his eyes pierce through your soul.", ch, NULL, NULL, TO_CHAR );
   act( "A cloak man suddenly leap in front of $n as his eyes pierce through $s soul.", ch, NULL, NULL, TO_ROOM );
   act( "Sensing the end near, you frantically try to cut the assassin in half!", ch, NULL, NULL, TO_CHAR );
   act( "Sensing the end near, $n frantically tries to cut the assassin in half!", ch, NULL, NULL, TO_ROOM );
   act( "No matter what you do, your weapons seem to go through his body.", ch, NULL, NULL, TO_CHAR );
   act( "No matter what $n does, $s weapons seem to go through his body.", ch, NULL, NULL, TO_ROOM );
   act( "Unsheathing his daggers, the last thing you see is Jaggar fading away...", ch, NULL, NULL, TO_CHAR );
   act( "Unsheathing his daggers, the last thing $n sees is Jaggar fading away...", ch, NULL, NULL, TO_ROOM );

   sprintf( buf, "#w%s #ehas been assassinated by #rJaggar#e... The #wAngel #eof #rDeath#e...", ch->pcdata->switchname );
   do_info( mob, buf );

   stop_fighting( ch, TRUE );
   ch->hit = -1000010;
   ch->position = POS_MORTAL;

   jaggar = number_range( 15, 30 );
}

void do_altima( CHAR_DATA *ch, CHAR_DATA *victim )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *mob;
   int option = number_range( 1, 5 );
   DESCRIPTOR_DATA *d;

   mob = get_mob( 104 );

   if ( mob == NULL )
      return;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL )
         stc( "\n\r", d->character );
   }

   switch( option )
   {
       default:
       case 1: do_chat( mob, "Praise the Light, and marvel at its wonders" );
               break;
       case 2: do_chat( mob, "Warriors of the Light, hate not evil for it is the reason of your existence." );
               break;
       case 3: do_chat( mob, "Have faith in the Light... and walk unharmed through the path of evil." );
               break;
       case 4: do_chat( mob, "Come behind me children of man, for I shall protect you." );
               break;
       case 5: do_chat( mob, "This is indeed Justice." );
               break;
   }

   act( "#yYou #ware suddenly surrounded by a #Pbeautiful #yaura #was #C$N's #wattack shatters into pieces.#n", ch, NULL, victim, TO_CHAR );
   act( "#y$n #wis suddenly surrounded by a #Pbeautiful #yaura #was #Cyour #wattack shatters into pieces.#n", ch, NULL, victim, TO_VICT );
   act( "#y$n #wis suddenly surrounded by a #Pbeautiful #yaura #was #C$N's #wattack shatters into pieces.#n", ch, NULL, victim, TO_NOTVICT );

   altima = number_range( 25, 50 );
}

/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    OBJ_DATA  *obj;
    bool is_obj;
    bool drop_out = FALSE;
    time_t save_time;
    int timer, count;
    char buf[MAX_STRING_LENGTH];
    int ass_timer;

    save_time	= current_time;
    ch_save	= NULL;
    ch_quit	= NULL;

    if ( endozone > 0 )
       {
          if ( --endozone == 0 )
             do_info( NULL, "#wThe #yHeavens #whave dispersed the #CEndo #pZone#w... You are #Rsafe #wnow...#n" );
       }

    if ( scavenger_time > 0 )
       {
          scavenger_time--;

          if ( scavenger_time == 0 )
             stop_scavenger();
             else {
                     char buf[MAX_INPUT_LENGTH];
                     DESCRIPTOR_DATA *d;
                     int finders = 0;

                     for ( d = descriptor_list; d != NULL; d = d->next )
                     {
                        if ( d->character == NULL || !IS_SET( d->character->extra, EXTRA_HUNT )
                             || d->character->scavenger_item == 0 )
                           continue;

                        finders++;
                     }

                     sprintf( buf, "#w%d #yitems #ghad been found by #w%d #Cpeople#g! You have #w%d #Rticks #gremaining.#n", 50 - scavenger_items, finders, scavenger_time );
                     do_sinfo( NULL, buf );
                  }
       }

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
       if ( obj->carried_by == NULL && obj->in_room != NULL && !IS_SET(obj->quest, QUEST_ARTIFACT) && obj->timer == -1
            && obj->item_type != ITEM_PAGE && obj->item_type != ITEM_BOOK && obj->pIndexData->vnum != 400
            && obj->pIndexData->vnum != 91 && obj->item_type != ITEM_DTOKEN && obj->item_type != ITEM_QUEST
            && obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_PKTOKEN ) 
          obj->timer = 10;
    }

    if ( arena == FIGHT_AUTO && ( ( time_info.hour % 4 ) == 0 ) )
       {
           int count = 0;
           DESCRIPTOR_DATA *d;

           for ( d = descriptor_list; d != NULL; d = d->next )
           { 
              if ( d->character != NULL && d->character->pcdata->team == 9 )
                 count++;
           }

           if ( count > 1 )
              {
                 do_ainfo( NULL, "#wThe #CQ#yu#ee#Rs#Lt #gfor #eSurvival #ghas #w#zStarted#n#g!!!#n" );
                 first_team = -1;
                 second_team = -1;
                 third_team = -1;
                 arena = FIGHT_QUEST;
              }
              else {
                      arena = FIGHT_CLEAR;
    
                      for ( d = descriptor_list; d != NULL; d = d->next )
                      { 
                         if ( d->character != NULL && d->character->pcdata->team == 9 )
                            {
                               d->character->pcdata->team = 0;
                               restore_char( d->character );
                            }
                      }

                      do_ainfo( NULL, "#e<#r<#e<#r<#e<#gThe Quest for #oSurvival #ghas been #R#zcancelled#n#g!#e>#r>#e>#r>#e>#n" );
                   }
       }

    if ( armageddon_timer > 0 )
       {
          armageddon_timer--;

          if ( armageddon_timer == 0 && armageddon == TRUE )
             {
                armageddon = FALSE;
                do_info( NULL, "#wThe #gWorld #yhas #rregained its #w#zSanity!!!#n" );
             }
       }

    if ( arena == FIGHT_WAR )
       {
          if ( ( time_info.hour % 12 ) == 0 && w1_kdg != -1 )
             {
                sprintf( buf, "%s #Pis currently in the #y#zlead#n #Pwith #w%d #Pkills:#n", king_table[ w1_kdg ].who_name, w1_kills );
                do_info( NULL, buf );
             }
       }

    if ( team_list != NULL && arena == FIGHT_CLEAR )
       {
          bool challenger = FALSE, opponent = FALSE;
          int counter;
          char buf2[MAX_STRING_LENGTH];

          counter = 0;

          while ( counter < 4 && challenger == FALSE )
          {
             if ( team_list->challenger[counter] != NULL )
                {
                   challenger = TRUE;
                   break;
                }

             counter++;
          }          

          counter = 0;

          while ( counter < 4 && opponent == FALSE )
          {
             if ( team_list->opponent[counter] != NULL )
                {
                   opponent = TRUE;
                   break;
                }

             counter++;
          }

          if ( challenger == TRUE && opponent == TRUE )
             {
                bool comma = FALSE;

                sprintf( buf, "%s #Pteam #w- ", team_list->team_name[0] );

                counter = 0;

                while ( counter < 4 )
                {
                   if ( team_list->challenger[counter] != NULL )
                      {
                         if ( IS_NPC( team_list->challenger[counter] ) )
                            sprintf( buf2, "%s", team_list->challenger[counter]->short_descr );
                            else sprintf( buf2, "%s", team_list->challenger[counter]->pcdata->switchname );
  
                         if ( comma == FALSE )
                            comma = TRUE;
                            else strcat( buf, "#y,#w " );

                         strcat( buf, buf2 );
                      }

                   counter++;
                }

                do_tinfo( NULL, NULL, buf );

                do_tinfo( NULL, NULL, "                   #wV#ys#n                   " );

                sprintf( buf, "%s #Pteam #w- ", team_list->team_name[1] );

                counter = 0;
                comma = FALSE;

                while ( counter < 4 )
                {
                   if ( team_list->opponent[counter] != NULL )
                      {
                         if ( IS_NPC( team_list->opponent[counter] ) )
                            sprintf( buf2, "%s", team_list->opponent[counter]->short_descr );
                            else sprintf( buf2, "%s", team_list->opponent[counter]->pcdata->switchname );

                         if ( comma == FALSE )
                            comma = TRUE;
                            else strcat( buf, "#y,#w " );

                         strcat( buf, buf2 );
                      }

                   counter++;
                }

                do_tinfo( NULL, NULL, buf );

                arena = FIGHT_INTRO;

                if ( mybet == TRUE )
                   {
                      strcpy(buf,"#eTo #wwager #eon the #pfight#e, type: #wbet #g(amount) #b(player name)#n");
                      do_tinfo( NULL, NULL, buf );
                      betting_time = 10;
                   }
             }
             else if ( challenger != TRUE && opponent != TRUE )
                     remove_team();
             else if ( time_info.hour == 0 )
                     {
                         if ( team_list->team_numbers[0] > 0 )
                            sprintf( buf, "#gThe #wchallenge #gfrom %s #Pteam #ghad been removed dued to #eno #gcompetition.", team_list->team_name[0] );
                            else sprintf( buf, "#gThe #wchallenge #gfrom %s #Pteam #ghad been removed dued to #eno #gcompetition.", team_list->team_name[1] );

                         do_tinfo( NULL, NULL, buf );
                         remove_team();
                     }                     
       }

    if ( arena == FIGHT_INITWAIT )
       {
          DESCRIPTOR_DATA *d;

          for ( d = descriptor_list; d != NULL; d = d->next )
          {
             if ( d == NULL || d->character == NULL )
                continue;

             if ( d->character->pcdata->watching != NULL )
                {
                   act( "You stop watching $N.", d->character, NULL, d->character->pcdata->watching, TO_CHAR );
                   d->character->pcdata->watching = NULL;
                }
          }

          do_announce_match();
          arena == FIGHT_ROUND;
       }

    if ( arena == FIGHT_INITIALIZE )
       {
          do_tinfo( NULL, NULL, "#yThis #gis #eit#R! #yAll #gthe contestants are #C#zready#n#w!!!#n" );
          do_tinfo( NULL, NULL, "#yWho #rshall #Pbe this #gyear's #wWorld Champion#r?#n" );
          arena = FIGHT_INITWAIT;
       }

    if ( doubleheal > 0 )
       {
          if ( --doubleheal == 0 )
             do_info( ch, "#wThe #cgates #wto #yheavens #whave been closed... #eCry#w!#n" );
       }

    if ( ( time_info.hour % 6 ) == 0 )
       do_eipheal( 0 );

    if ( time_info.hour == 1 )
       do_jaggar_effect();

    if ( time_info.hour == 7 )
       do_conan_effect();

    if ( time_info.hour == 5 || time_info.hour == 10 || time_info.hour == 15 || time_info.hour == 20 )
       do_conan( 0 );

    if ( arena == FIGHT_CLEAR && num_contestants > 3 )
       {
          char arg[MAX_INPUT_LENGTH];
          int num, tries = 0;
          bool found = FALSE;

          do
          {
             ch = NULL;
             tries++;

             if ( tries > 10 )
                num = tries - 10;
                else num = number_range( 1, 19 );

             switch( num )
             {
                default:
                case 1: sprintf( arg, "arena1" ); break;
                case 2: sprintf( arg, "arena2" ); break;
                case 3: sprintf( arg, "arena3" ); break;
                case 4: sprintf( arg, "arena4" ); break;
                case 5: sprintf( arg, "arena5" ); break;
                case 6: sprintf( arg, "arena6" ); break;
                case 7: sprintf( arg, "arena7" ); break;
                case 8: sprintf( arg, "arena8" ); break;
                case 9: sprintf( arg, "arena9" ); break;
                case 10: sprintf( arg, "arenax" ); break;
                case 11: sprintf( arg, "arenay" ); break;
                case 12: sprintf( arg, "arenaz" ); break;
                case 13: sprintf( arg, "arenaa" ); break;
                case 14: sprintf( arg, "arenab" ); break;
                case 15: sprintf( arg, "arenad" ); break;
                case 16: sprintf( arg, "arenac" ); break;
                case 17: sprintf( arg, "arenae" ); break;
                case 18:
                case 19: sprintf( arg, "arenaf" ); break;
             }

             for ( ch = char_list; ch != NULL ; ch = ch->next )
             {
                if ( !IS_NPC( ch ) || ch->in_room == NULL || ( ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800 ) )
                   continue;

                if ( !is_name( arg, ch->name ) )
                   continue;

                break;
             }

             num = 0;

             while ( num < 16 && ch != NULL )
             {
                if ( participants[ num ] == ch )
                   ch = NULL;

                num++;
             }
          }
          while ( ch == NULL && tries < 23 );

          if ( ch != NULL )
             {
                num_contestants++;
                num = 0;

                while ( participants[ num ] != NULL )
                  num++;

                participants[ num ] = ch;
                ch->round = 0;

                SET_BIT( ch->arena, AFF2_COMPETITOR );

                sprintf( arg, "#y%s #whas just entered the #CKing #wof #RFighters #PTournament#w!!!#n", ch->short_descr );
                do_info( ch, arg );

                if ( num_contestants == 16 )
                   do_initialize_tournament( ch );

                ch = NULL;
             }
       }

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	int i;
	ch_next = ch->next;

	if (!IS_NPC(ch) && (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)))
	    is_obj = TRUE;
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	    {is_obj = TRUE;SET_BIT(ch->extra, EXTRA_OSWITCH);}
	else
	    is_obj = FALSE;
	/*
	 * Find dude with oldest save time.
	 */
	if ( !IS_NPC(ch)
	&& ( ch->desc == NULL || ch->desc->connected == CON_PLAYING )
	&&   ch->level >= 2
	&&   ch->save_time < save_time )
	{
	    ch_save	= ch;
	    save_time	= ch->save_time;
	}

      if ( IS_NPC( ch ) && ch->master != NULL && !IS_NPC( ch->master ) && ch->master->in_room != ch->in_room
           && ch->position != POS_STUNNED && ch->pIndexData->vnum != 1 )
         {
            if ( ch->position == POS_FIGHTING )
               stop_fighting( ch, TRUE );

            char_from_room( ch );
            char_to_room( ch, ch->master->in_room );
         }

       if ( arena == FIGHT_WAR && ( time_info.hour % 3 ) == 0 )
          {
              if ( IS_NPC( ch ) && ch->pIndexData->vnum == 100 )
                 {
                    int option = number_range( 1, 5 );

                    if ( ch->max_hit < 10000000 )
                       ch->max_hit = UMIN( 10000000, ch->max_hit + 2000000 );

                    if ( ( time_info.hour == 3 || time_info.hour == 21 ) && man_kills > 0 )
                       {
                           sprintf( buf, "Hmph... %s isn't doing too badly with %d kills. Almost a true warrior.", man_match, man_kills );
                           do_chat( ch, buf );
                           option = 10;
                       }

                    switch( option )
                    {
                        case 1: do_chat( ch, "Don't hesitate, Break through now!" );
                                break;

                        case 2: do_chat( ch, "Huh? Now there is a rare sight." );
                                break;

                        case 3: do_chat( ch, "The dead should be allowed to rest." );
                                break;

                        case 4: do_chat( ch, "Never futile... but neverending." );
                                break;

                        case 5: do_chat( ch, "Lost your nerve?" );
                                break;
                    }
                 }
          }
          else if ( arena == FIGHT_WAR && ( time_info.hour % 2 ) == 0 )
          {
              if ( IS_NPC( ch ) && ch->pIndexData->vnum == 101 )
                 {
                    int option = number_range( 1, 4 );

                    if ( ( time_info.hour == 8 || time_info.hour == 18 ) && w2_kdg != -1 )
                       {
                           sprintf( buf, "Oh my, %s is so close to %s. Just need %d more kills to past them.", king_table[ w2_kdg ].who_name, king_table[ w1_kdg ].who_name, w1_kills - w2_kills + 1 );
                           do_chat( ch, buf );
                           option = 10;
                       }

                    switch( option )
                    {
                       case 1: do_chat( ch, "The action is so tense right now, that a needle dropping can shatter the moment." );
                               break;

                       case 2: do_chat( ch, "I am sure Lord Zero is having a great time watching this chaos." );
                               break;

                       case 3: do_chat( ch, "If only we had people like these back during the Clone wars..." );
                               break;

                       case 4: do_chat( ch, "Say Auron? Good food, good girls, good fights - what more could you want?" );
                               break;
                    }
                 }
          }

       if ( arena == FIGHT_WAIT )
          {
             switch( time_info.hour )
             {
                case 1: if ( IS_NPC( ch ) && ch->pIndexData->vnum == 100 )
                           do_chat( ch, "You are as boring as ever Krizalid..." );
                        break;

                case 3: if ( IS_NPC( ch ) && ch->pIndexData->vnum == 101 )
                           do_chat( ch, "Yeah yeah whatever. I hope we get a good fight this time." );
                        break;

                case 4: if ( IS_NPC( ch ) && ch->pIndexData->vnum == 100 )
                           do_chat( ch, "From these amateurs? Dream on, Heh." );
                        break;

                case 7: if ( IS_NPC( ch ) && ch->pIndexData->vnum == 100 )
                           do_chat( ch, "What is taking Nests so long to start this thing?" );
                        break;

                case 8: if ( IS_NPC( ch ) && ch->pIndexData->vnum == 101 )
                           do_chat( ch, "We have to make sure all competitors are fully prepared Auron." );
                        break;

                case 12: if ( IS_NPC( ch ) && ch->pIndexData->vnum == 100 )
                            {
                               do_chat( ch, "It begins." );
                               arena = FIGHT_WAR;
                            }

                         break;
             }           
          }

      if ( IS_SET( ch->more3, MORE3_CRANE ) && ch->position > POS_INCAP )
         {
            act("#yYou #wclose your eyes as you #ctilt #wback, spreading your palms apart.#n",ch,NULL,NULL,TO_CHAR);
            act("#y$n #wcloses $s eyes as $e #ctilts #wback, spreading $s palms apart.#n",ch,NULL,NULL,TO_ROOM);
            act("#yYou #wtake a deep breath as your #Cchi #yenergy #wsoars through your body.#n",ch,NULL,NULL,TO_CHAR);
            act("#y$n #wtakes a deep breath as $s body glows with a #psoft #yyellowish #wenergy.#n",ch,NULL,NULL,TO_ROOM);
            hit_gain( ch, 300000 );
            mana_gain( ch, 500000 );
            move_gain( ch, 500000 );
         }

      if ( IS_NPC( ch ) && IS_SET( ch->act, ACT_TIMER ) )
         {
            if ( ch->fighting == NULL )
               {
                  ch->practice--;

                  if ( ch->practice == 0 )
                     {
                         act( "#y$n #gleaves the #wvicinity #gas it #Pcontinues #gon $s #yjourney#g.#n", ch, NULL, NULL, TO_ROOM );
                         extract_char( ch,TRUE );
                         continue;
                     }
               }
         }       

      if ( IS_SET( ch->extra, EXTRA_TLOCK ) )
         {          
            if ( ( time_info.hour % 2 ) == 0 )
               {
                   for ( count = 0; count < 18; count++ )
                   {
                      if ( count == 8 || count == 9 || count == 15 )
                         continue;

                      if ( IS_SET( ch->cmbt[7], SKL_ACCELERATE ) )
                         ch->ctimer[count] = UMAX( 0, ch->ctimer[count] - 2 );
                         else ch->ctimer[count]--;
                   }
               }
         }
         else {
                 for ( count = 0; count < 18; count++ )
                 {
                    if ( count == 8 || count == 9 || count == 15 )
                       continue;

                    if ( ch->ctimer[count] > 0 )
                       {
                          if ( IS_SET( ch->cmbt[7], SKL_ACCELERATE ) )
                             ch->ctimer[count] = UMAX( 0, ch->ctimer[count] - 2 );
                             else ch->ctimer[count]--;
                       }
                 }
              }

      if ( ch->guarding > 0 )
         ch->guarding--;

      if ( !IS_NPC(ch) )
         ch->pcdata->perm_luck = number_range( 0, 10 );

      if ( ch->fight_timer == 0 && ch->dam_boost != 1.0 )
         ch->dam_boost = 1.0;

      if ( IS_NPC( ch ) && IS_SET(ch->extra, EXTRA_ZOMBIE) && ch->master == NULL && ch->fighting == NULL )
         {
             act( "#R$n #cfades #eback into the ground.#n", ch, NULL, NULL, TO_ROOM );
             extract_char( ch,TRUE );
             continue;
         }

      if ( !IS_NPC(ch) && ch->login_timer > 0 )
         {
             ch->login_timer--;

             if ( ch->login_timer == 0 )
                stc("#w#zYou can now quit.#n\n\r",ch);
         }

      if ( !IS_NPC(ch) && ch->pcdata->capture_timer > 0 )
         ch->pcdata->capture_timer--;

      if ( ch->safe_timer > 0 )
         {
           ch->safe_timer--;

           if ( ch->safe_timer == 0 )
              stc("#p#zYou are now pkable again.#n\n\r",ch);
         }

      if ( IS_CLASS( ch, CLASS_SHADOW ) )
         {
            if ( ch->pkill > 19 )
               ass_timer = 1;
               else if ( ch->pkill > 14 )
                       ass_timer = 2;
               else if ( ch->pkill > 9 )
                       ass_timer = 4;
               else if ( ch->pkill > 4 )
                       ass_timer = 6;
                       else ass_timer = 12;
         }

      if ( IS_CLASS( ch, CLASS_SHADOW ) && ( time_info.hour % ass_timer ) == 0 && ch->clannum != 1  )
         {
            int best = 6;
            int counter;

            for ( counter = 6; counter < 12; counter++ )
            {
               if ( counter - 6 == ch->gifts[CURRENT_METER] )
                  ch->gifts[counter] += number_range(-25,10);
                  else ch->gifts[counter] += number_range(-10,25);

               if ( ch->gifts[counter] <= 0 )
                  ch->gifts[counter] = 0 + number_range( 1, 10 );

               if ( ch->gifts[counter] >= 100 )
                  ch->gifts[counter] = 100 - number_range( 1, 10 );

               if ( ch->gifts[counter] > ch->gifts[best] )
                  best = counter;
            }

            if ( best - 6 != ch->gifts[CURRENT_METER] )
               {
                  if ( IS_SET(ch->more3, MORE3_RAGE) )
                     REMOVE_BIT(ch->more3, MORE3_RAGE);

                  if ( IS_SET(ch->more3, MORE3_YIN) )
                     REMOVE_BIT(ch->more3, MORE3_YIN);

                  if ( IS_SET(ch->more3, MORE3_YANG) )
                     REMOVE_BIT(ch->more3, MORE3_YANG);

                  if ( IS_SET(ch->itemaffect, ITEMA_FIREWALL) )
                     REMOVE_BIT(ch->itemaffect, ITEMA_FIREWALL);

                  act("You fall to your knees cackling wildly as you clutch your head in agony!",ch,NULL,NULL,TO_CHAR);
                  act("$n falls to $s knees cackling wildly as $e clutches $s head in agony!",ch,NULL,NULL,TO_ROOM);
                  act("As your eyes glow red, you lean back screaming with rage!",ch,NULL,NULL,TO_CHAR);
                  act("As $n's eyes glow red, $e leans back screaming with rage!",ch,NULL,NULL,TO_ROOM);

                  ch->gifts[CURRENT_METER] = best - 6;

                  switch( ch->gifts[CURRENT_METER] )
                  {
                     case 0: act("You arrogantly stand at attention as you stare about you coldly.",ch,NULL,NULL,TO_CHAR);
                             act("$n arrogantly stands at attention as $e stares about $m coldly.",ch,NULL,NULL,TO_ROOM);
                             act("Your body bursts out with positive energy.",ch,NULL,NULL,TO_CHAR);
                             act("$n's body bursts out with positive energy.",ch,NULL,NULL,TO_ROOM);
                             break;
  
                     case 1: act("Your body explodes with kiotic energy as you cackle wildly.",ch,NULL,NULL,TO_CHAR);
                             act("$n's body explodes with kiotic energy as $e cackles wildly.",ch,NULL,NULL,TO_ROOM);
                             break;

                     case 2: act("You do a beautiful roundhouse kick as you start swaying in a legendary fighting stance.",ch,NULL,NULL,TO_CHAR);
                             act("$n does a beautiful roundhouse kick as $e starts swaying in a legendary fighting stance.",ch,NULL,NULL,TO_ROOM);
                             ch->special_id = 0;
                             ch->special_id_2 = 0;
                             break;
 
                     case 3: act("The shadows rises from the ground blankening you from sight.",ch,NULL,NULL,TO_CHAR);
                             act("The shadows rises from the ground blankening $n from sight.",ch,NULL,NULL,TO_ROOM);
                             break;

                     case 4: act("You grin evilly as you lower your hand to your weapon, ready to attack.",ch,NULL,NULL,TO_CHAR);
                             act("$n grins evilly as $e lowers $s hand to $s weapon, ready to attack.",ch,NULL,NULL,TO_ROOM);
                             break;

                     case 5: act("Your hatred for the unviverse becomes so great that everyone cowers in your presence.",ch,NULL,NULL,TO_CHAR);
                             act("$n's hatred for the unviverse becomes so great that you cower in $s presence.",ch,NULL,NULL,TO_ROOM);
                             break;
                  }
               }
         }

      if ( IS_CLASS(ch,CLASS_SAIYAN) && ( time_info.day % 4 ) == 0 && time_info.hour == 20 && ch->saiyanab[BODY] != 500 )
         {
            char buf[MAX_STRING_LENGTH];

            if ( ch->position <= POS_STUNNED )
               ch->position = POS_STANDING;

            ch->hit = UMIN( ch->hit + 1500000, ch->max_hit );
            ch->mana = ch->max_mana;
            ch->move = ch->max_move;

            free_string( ch->morph );
      	ch->morph = str_dup( "" );

            if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
               REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

            act("You start trembling as you lower your head and laugh insanely.",ch,NULL,NULL,TO_CHAR);
            act("$n starts trembling as $e lowers $s head and laughs insanely.",ch,NULL,NULL,TO_ROOM);
            act("A mass of #obrown#n furry hair flows outwards your body as you grow to a massive size.",ch,NULL,NULL,TO_CHAR);
            act("A mass of #obrown#n furry hair flows outwards $n's body as $e grows to a massive size.",ch,NULL,NULL,TO_ROOM);
            act("Your eyes grow smaller as your nose and mouth turns into a huge snort.",ch,NULL,NULL,TO_CHAR);
            act("$n's eyes grow smaller as $s nose and mouth turns into a huge snort.",ch,NULL,NULL,TO_ROOM);
            act("#wStOmPiNg#n across the room, your body starts trembling as your eyes flashes #ygold#n.",ch,NULL,NULL,TO_CHAR);
            act("#wStOmPiNg#n across the room, $n's body starts trembling as $s eyes flashes #ygold#n.",ch,NULL,NULL,TO_ROOM);

            sprintf(buf, "#o%s the Oozaru#n", ch->name);
            ch->saiyanab[BODY] = 500;
            free_string( ch->morph );
            ch->morph = str_dup( buf );

            if ( !IS_SET(ch->affected_by, AFF_POLYMORPH) )
               SET_BIT(ch->affected_by, AFF_POLYMORPH);

            act("#RYou roar at the #Cmoon#R loudly, beating you #yHUGE#R chest with your #wMaSsIvE#R fists.#n",ch,NULL,NULL,TO_CHAR);
            act("#R$n roars at the #Cmoon#R loudly, beating $s #yHUGE#R chest with $s #wMaSsIvE#R fists.#n",ch,NULL,NULL,TO_ROOM);
            act("Your hair grows out #CVERY #pthick#n and long as you #LClEnCh#n your fists in anticipation.",ch,NULL,NULL,TO_CHAR);
            act("$n's hair grows out #CVERY #pthick#n and long as $e #LClEnCheS#n $s fists in anticipation.",ch,NULL,NULL,TO_ROOM);
            act("You #yhAmMeR#n your chest as you release an #REARTH SHATTERING ROAR#n blasting everything away from you.",ch,NULL,NULL,TO_CHAR);
            act("$n #yhAmMeRs#n $s chest as $e releases an #REARTH SHATTERING ROAR#n blasting everything away from $m.\n\r",ch,NULL,NULL,TO_ROOM);
            stc("#yYou are now an Oozaru.#n\n\r",ch);
         }

      if ( IS_CLASS(ch,CLASS_SAIYAN) && time_info.hour == 7 && ch->saiyanab[BODY] == 500 )
         {
            act("You start trembling as you lower your head and laugh insanely.",ch,NULL,NULL,TO_CHAR);
            act("$n starts trembling as $e lowers $s head and laughs insanely.",ch,NULL,NULL,TO_ROOM);
            act("You #Rroar#n at the #ysun#n loudly, beating you HuGe chest as you grow smaller.",ch,NULL,NULL,TO_CHAR);
            act("$n #Rroars at the #ysun#n loudly, beating $s HuGe chest as $s grows smaller.",ch,NULL,NULL,TO_ROOM);
            act("The #ofur#n surrounding your body drops off as your #Rrage #ccalms#n down.",ch,NULL,NULL,TO_CHAR);
            act("The #ofur#n surrounding $n's body drops off as $s #Rrage #ccalms#n down.\n\r",ch,NULL,NULL,TO_ROOM);
            stc("You are now back to normal.\n\r\n\r",ch);

            ch->saiyanab[BODY] = 0;

            free_string( ch->morph );
      	ch->morph = str_dup( "" );
            if ( IS_SET(ch->affected_by, AFF_POLYMORPH) )
               REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
         }

      if ( IS_SET( ch->extra, EXTRA_LOWDEF ) && ( IS_NPC( ch ) || ch->fight_timer == 0 ) )
         REMOVE_BIT( ch->extra, EXTRA_LOWDEF );

      if ( IS_SET( ch->extra, EXTRA_LOWOFF ) && ( IS_NPC( ch ) || ch->fight_timer == 0 ) )
         REMOVE_BIT( ch->extra, EXTRA_LOWOFF );

      if ( IS_SET(ch->in_room->room_flags, ROOM_LAVAPIT) && ( ( time_info.hour % 6 ) == 0 ) )
         {
             act("#RThe #ylava #Rseeps back into the ground as the hole beneath your feet vanish.#n",ch,NULL,NULL,TO_CHAR);
             act("#RThe #ylava #Rseeps back into the ground as the hole beneath your feet vanish.#n",ch,NULL,NULL,TO_ROOM);
             REMOVE_BIT(ch->in_room->room_flags, ROOM_LAVAPIT);
         }

      if ( IS_SET(ch->in_room->room_flags, ROOM_ASSASSIN) && ( ( time_info.hour % 12 ) == 0 ) )
         {
             act("#eThe #cmysterious #Pfield #evanishes.#n",ch,NULL,NULL,TO_CHAR);
             act("#eThe #cmysterious #Pfield #evanishes.#n",ch,NULL,NULL,TO_ROOM);
             REMOVE_BIT(ch->in_room->room_flags, ROOM_ASSASSIN);
         }

      if ( IS_SET(ch->in_room->room_flags, ROOM_BLAZE) && ( ( time_info.hour % 6 ) == 0 ) )
         {
             act("#RThe #yferocious #Rflames die away.#n",ch,NULL,NULL,TO_CHAR);
             act("#RThe #yferocious #Rflames die away.#n",ch,NULL,NULL,TO_ROOM);
             REMOVE_BIT(ch->in_room->room_flags, ROOM_BLAZE);
         }

      if ( IS_SET(ch->in_room->room_flags, ROOM_LUMICA) && ( ( time_info.hour % 3 ) == 0 ) )
         {
             stc("#wThe #csoft #yglow #wsurrounding the room surrounding #efades away.#n\n\r",ch);
             act("#wThe #csoft #yglow #wsurrounding the room surrounding #efades away.#n",ch,NULL,NULL,TO_ROOM);
             REMOVE_BIT(ch->in_room->room_flags, ROOM_LUMICA);
         }

      if ( !IS_NPC( ch ) && !IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->in_room->room_flags, ROOM_LAVAPIT)
           && ch->position >= POS_STUNNED )
         {
             act("The lava splashes across the room SEARING your flesh!",ch,NULL,NULL,TO_CHAR);
             act("The lava splashes across the room SEARING $n's flesh!",ch,NULL,NULL,TO_ROOM);

             if ( !( ( IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_SHADOW_WAR) ) && ch->ctimer[8] > 0 ) )
                {
                   ch->hit -= is_elemental( ATTACK_FIRE, ch, 150000 );
                   update_pos(ch);
                }
         }

      if ( IS_CLASS(ch, CLASS_DROW) && ch->cmbt[3] < 5 )
         ch->cmbt[3] = UMIN( 5, ch->cmbt[3] + 2 );

	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast > 0
	  && ch->pcdata->condition[COND_THIRST] <= 15 )
	{
	  act("You bare your fangs and scream in rage from lack of blood.",ch,NULL,NULL,TO_CHAR);
	  act("$n bares $s fangs and screams in rage.",ch,NULL,NULL,TO_ROOM);
	  do_berserk(ch,"");
	  do_beastlike(ch,"");
	}

      if ( ( IS_SET(ch->more3, MORE3_SILENCE ) ) && ( ch->fight_timer == 0 ) )
	  {
            stc("#7The #Cice #7melts and you can #ytalk#n again.\n\r",ch);
            REMOVE_BIT(ch->more3, MORE3_SILENCE);
	  }

      if ( ( IS_SET(ch->more, MORE_SIN) ) && ( ch->position != POS_FIGHTING ) )
         {
            stc("You feel much better as the sins of your past leaves you.\n\r",ch);
            REMOVE_BIT(ch->more, MORE_SIN);
         }

      if ( ( IS_SET(ch->more,MORE_TAUNT) ) && ( ch->position != POS_FIGHTING ) )
         {
            stc("Your anger boils down as you return to normal.\n\r",ch);
            REMOVE_BIT(ch->more, MORE_TAUNT);
         }

      if ( IS_SET(ch->monkstuff, MONK_DEATH) && ( time_info.hour % 2 ) == 0 && ch->position != POS_FIGHTING )
        REMOVE_BIT(ch->monkstuff, MONK_DEATH);

	if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->ninja_powers[HARA_KIRI] > 0 ) 
         {
		if (ch->pcdata->ninja_powers[HARA_KIRI] == 1)
		send_to_char("#eThe effects of the #Rharakiri #ewear off.#n\n\r", ch );
		ch->pcdata->ninja_powers[HARA_KIRI]--;	
   	   }

      if ( ch->quint[BODY] < ch->spheres[MPRI] )
         ch->quint[BODY]++;

      if ( ch->quint[AVATAR] < ch->spheres[MPRI] )
         ch->quint[AVATAR]++;

      if ( time_info.hour == 12 )
         {
            check_leaderboard(ch,"");

            if ( IS_SET(ch->in_room->room_flags, ROOM_ICETRAP) )
               {
                  act("#LThe #Cicy trap #Lhidden in the room melts away before your #weyes#L.#n",ch,NULL,NULL,TO_CHAR);
                  act("#LThe #Cicy trap #Lhidden in the room melts away before your #weyes#L.#n",ch,NULL,NULL,TO_ROOM);
                  REMOVE_BIT(ch->in_room->room_flags, ROOM_ICETRAP);
               }

            if ( IS_SET( ch->elemental, ELE_LUNARP ) )
               {
                  act("#yYou #osnarl as your #wlunar #Rpower #ois stripped away from you.#n",ch,NULL,NULL,TO_CHAR);
                  REMOVE_BIT( ch->elemental, ELE_LUNARP );
               }

            if ( IS_SET(ch->newbits, NEW_MONKCLOAK) )
               REMOVE_BIT(ch->newbits, NEW_MONKCLOAK);

            if ( IS_SET(ch->more, MORE_BOMB) )
               REMOVE_BIT(ch->more, MORE_BOMB);

            if ( IS_SET(ch->more2, MORE2_VORTEX) )
               REMOVE_BIT(ch->more2, MORE2_VORTEX);

            if ( IS_SET(ch->more2, MORE2_DSMASHER) )
               REMOVE_BIT(ch->more2, MORE2_DSMASHER);

            if ( IS_SET(ch->more2, MORE2_DESPERATE) )
               REMOVE_BIT(ch->more2, MORE2_DESPERATE);
         }

      if ( IS_SET(ch->more, MORE_BLIND) && ch->blind_timer == 0 )
         {
             REMOVE_BIT(ch->more, MORE_BLIND);
             send_to_char("#wYour vision returns!#n\n\r",ch);
             act("#wThe white gaze surrounding $n's eyes #Cclears#w.#n",ch,NULL,NULL,TO_ROOM);
         }

      if ( IS_SET(ch->more, MORE_FROZEN) )
         {
             REMOVE_BIT(ch->more, MORE_FROZEN);
             ch->freeze_timer = 0;
             send_to_char("#bThe #wice #bencasing you melts away.#n\n\r",ch);
             act("#bThe #wice #bencasing $n #bmelts away.#n",ch,NULL,NULL,TO_ROOM);
         }

      if ( IS_SET(ch->more, MORE_POISON) && ch->hit > 250000 )
         {
             if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON) )
                REMOVE_BIT(ch->more, MORE_POISON);
                else if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) )
                        REMOVE_BIT(ch->more, MORE_POISON);
                else if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DRAGON) && IS_DRAGONPOWER(ch,DRAGON_IMMUNITY) )
                        REMOVE_BIT(ch->more, MORE_POISON);
                else if ( ( IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_SHADOW_WAR) ) && ch->ctimer[8] > 0 )
                        REMOVE_BIT(ch->more, MORE_POISON);
                else {
                        send_to_char("You cough violently as the poison rips through your vital systems.\n\r",ch);
                        act("$n coughs violently as the poison rips through $s vital systems.",ch,NULL,NULL,TO_ROOM); 

                        if ( !IS_CLASS( ch, CLASS_DUERGAR ) )
                           ch->hit -= 250000;
                           else ch->hit = UMIN( ch->max_hit, ch->hit + ( 250000 * UMIN( 1, ch->pkill / 10 ) ) );
                     }
         }

      if ( IS_SET( ch->extra, EXTRA_TLOCK ) && ch->fight_timer == 0 )
         {
            send_to_char("Time has been restored to you as the air has been lifted.\n\r",ch);
            REMOVE_BIT( ch->extra, EXTRA_TLOCK );
         }

      if ( IS_SET(ch->in_room->room_flags, ROOM_NOMOVE) )
         {
            act("#CThe deadly #wenergy #Csuddenly #yfades #Caway allowing you to move again.#n",ch,NULL,NULL,TO_CHAR);
            act("#CThe deadly #wenergy #Csuddenly #yfades #Caway allowing you to move again.#n",ch,NULL,NULL,TO_ROOM);
            REMOVE_BIT(ch->in_room->room_flags, ROOM_NOMOVE);
         }

      if ( ( time_info.hour % 6 ) == 0 )
         {
             if ( IS_SET( ch->in_room->room_flags, ROOM_BARRIER ) )
                {
                   act("#rThe #Cmystical #Pbarrier #evanishes.#n",ch,NULL,NULL,TO_CHAR);
                   act("#rThe #Cmystical #Pbarrier #evanishes.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT(ch->in_room->room_flags, ROOM_BARRIER);
                }

             if ( IS_CLASS( ch, CLASS_SNOW_ELF ) && IS_SET( ch->more2, MORE2_DIG ) )
                {
                   act("You burst from beneath the snow as you gasp loudly for breath.",ch,NULL,NULL,TO_CHAR);
                   act("$n bursts from beneath the snow as $e gasps loudly for breath.",ch,NULL,NULL,TO_ROOM);

                   REMOVE_BIT( ch->more2, MORE2_DIG );
                   ch->wait = 12;
                   return;
                }

             if ( IS_SET( ch->elemental, ELE_LUNAR ) )
                {
                   act("#yYour #otechniques lost their #wlunar #oaffect.#n",ch,NULL,NULL,TO_CHAR);
                   REMOVE_BIT( ch->elemental, ELE_LUNAR );
                }

             if ( IS_SET( ch->extra, EXTRA_LTOUCH ) )
                {
                   act("#wYou feel a great burden leaving your #Psoul #was the #eleeching spirits #ware gone.#n",ch,NULL,NULL,TO_CHAR);
                   act("#w$n #wfeels a great burden leaving $s #Psoul #was the #eleeching spirits #ware gone.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->extra, EXTRA_LTOUCH );
                }

             if ( IS_SET( ch->extra, EXTRA_FBITE ) )
                {
                   act("#CYou feel a great warmth in your toes as the #wfrostbite #Cis gone.#n",ch,NULL,NULL,TO_CHAR);
                   act("#C$n #Cfeels a great warmth in $s toes as the #wfrostbite #Cis gone.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->extra, EXTRA_FBITE );
                }

             if ( IS_SET(ch->garou1, GAROU_WHELP2) )
                {
                   REMOVE_BIT(ch->garou1, GAROU_WHELP2);
                   send_to_char("You feel better as the curse leaves you.\n\r",ch);
                   ch->max_hit += 750000;
                }

             if ( IS_SET(ch->in_room->room_flags, ROOM_FLAMING) )
                {
                   act("#RThe flames around the room #edies #Ras the ashes flutter away.#n",ch,NULL,NULL,TO_CHAR);
                   act("#RThe flames around the room #edies #Ras the ashes flutter away.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT(ch->in_room->room_flags, ROOM_FLAMING);
                }

             if ( IS_SET( ch->in_room->room_flags, ROOM_CATACOMB ) )
                {
                   act("#eThe #oencavement #esurrounding the room vanishes before your #Peyes#e.#n",ch,NULL,NULL,TO_CHAR);
                   act("#eThe #oencavement #esurrounding the room vanishes before your #Peyes#e.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT(ch->in_room->room_flags, ROOM_CATACOMB);
                }

             if ( IS_SET(ch->in_room->room_flags, ROOM_ICYSWORD) )
                {
                   act("#LThe #Cicy swords #Lsurrounding the room melts away before your #weyes#L.#n",ch,NULL,NULL,TO_CHAR);
                   act("#LThe #Cicy swords #Lsurrounding the room melts away before your #weyes#L.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT(ch->in_room->room_flags, ROOM_ICYSWORD);
                }

             if ( IS_SET( ch->in_room->room_flags, ROOM_WHIRLWIND ) )
                {
                   act("#CThe deadly #wwinds #Csuddenly #yscreech #Cas the room becomes calm again.#n",ch,NULL,NULL,TO_CHAR);
                   act("#CThe deadly #wwinds #Csuddenly #yscreech #Cas the room becomes calm again.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT(ch->in_room->room_flags, ROOM_WHIRLWIND);
                }

             if ( IS_SET(ch->in_room->room_flags, ROOM_ENTOMB) )
                {
                   act("#LThe icy wall encasing the room melts away before your #weyes#L.#n",ch,NULL,NULL,TO_CHAR);
                   act("#LThe icy wall encasing the room melts away before your #weyes#L.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT(ch->in_room->room_flags, ROOM_ENTOMB);
                }

             if ( IS_SET(ch->more, MORE_IMPALE) )
                {
                   REMOVE_BIT(ch->more, MORE_IMPALE);
                   send_to_char("The spear holding your foot dissolves into the ground!\n\r",ch);
                   act("The spear holding $n's foot dissolves into the ground.",ch,NULL,NULL,TO_ROOM);
                }

             if ( IS_SET( ch->elemental, ELE_WWATER ) )
                {
                   act("#LYou no longer fear the chilling water.#n",ch,NULL,NULL,TO_CHAR);
                   act("#L$n #Lno longer fears the chilling water.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WWATER );
                }

             if ( IS_SET( ch->elemental, ELE_WFIRE ) )
                {
                   act("#RYou no longer fear the blazing flames.#n",ch,NULL,NULL,TO_CHAR);
                   act("#R$n #Rno longer fears the blazing flames.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WFIRE );
                }

             if ( IS_SET( ch->elemental, ELE_WLUNAR ) )
                {
                   act("#oYou no longer fear the mysterious moon.#n",ch,NULL,NULL,TO_CHAR);
                   act("#o$n #ono longer fears the mysterious moon.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WLUNAR );
                }

             if ( IS_SET( ch->elemental, ELE_WTHUNDER ) )
                {
                   act("#yYou no longer fear the clapping thunder.#n",ch,NULL,NULL,TO_CHAR);
                   act("#y$n #yno longer fears the clapping thunder.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WTHUNDER );
                }

             if ( IS_SET( ch->elemental, ELE_WEARTH ) )
                {
                   act("#gYou no longer fear the harsh earth.#n",ch,NULL,NULL,TO_CHAR);
                   act("#g$n #gno longer fears the harsh earth.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WEARTH );
                }

             if ( IS_SET( ch->elemental, ELE_WHOLY ) )
                {
                   act("#wYou no longer fear the sacred light.#n",ch,NULL,NULL,TO_CHAR);
                   act("#w$n #wno longer fears the sacred light.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WHOLY );
                }

             if ( IS_SET( ch->elemental, ELE_WDARK ) )
                {
                   act("#eYou no longer fear the demonic void.#n",ch,NULL,NULL,TO_CHAR);
                   act("#e$n #eno longer fears the demonic void.#n",ch,NULL,NULL,TO_ROOM);
                   REMOVE_BIT( ch->elemental, ELE_WDARK );
                }
         }

      if ( ( time_info.hour % 2 ) == 0 )
         {
            if ( IS_SET(ch->more2, MORE2_NOLEGS) )
               {
                  send_to_char("Your movement is restored.\n\r",ch);
                  REMOVE_BIT(ch->more2, MORE2_NOLEGS);
               }

            if ( IS_SET(ch->more2, MORE2_NOARMS) )
               {
                   send_to_char("You can move your arms at will again.\n\r",ch);
                   REMOVE_BIT(ch->more2, MORE2_NOARMS);
               }

            if ( IS_SET( ch->more2, MORE2_SPINCH ) )
               {
                  act("#wYour #Ranger #wis restored to #Cnormal#w.#n",ch,NULL,NULL,TO_CHAR);
                  act("#w$n#w's #Ranger #wis restored to #Cnormal#w.#n",ch,NULL,NULL,TO_ROOM);
                  REMOVE_BIT(ch->more2, MORE2_SPINCH);
               }

            if ( !IS_NPC( ch ) && IS_SET( ch->extra, EXTRA_ZOMBIE ) )
               {
                  send_to_char("#oThe curse of the undead leaves your body.#n\n\r",ch);
                  act("#oThe curse of the undead leaves $n#o's body.#n", ch, NULL, NULL, TO_ROOM);
                  REMOVE_BIT( ch->extra, EXTRA_ZOMBIE );
               }
         }

      /* Leighton - The char has a 5% chance of losing his headache here */
      if ( IS_SET(ch->more, MORE_PINCH) )
         { 
            if ( 95 < number_percent() || ch->fight_timer == 0 )
               {
                  stc("Your head suddenly feels clear as the headache disappears.\n\r",ch);
                  act("$n has recovered from $s crippling headache.",ch,NULL,NULL,TO_ROOM);
                  REMOVE_BIT(ch->more, MORE_PINCH);
               }
         }

      if ( IS_SET(ch->more, MORE_PAIN) )
         { 
            if ( 50 < number_percent() )
               {
                  stc("Your suddenly start moving properly again.\n\r",ch);
                  act("$n suddenly starts moving properly again.",ch,NULL,NULL,TO_ROOM);
                  REMOVE_BIT(ch->more, MORE_PAIN);
               }
         }

      if ( ch->gshield > 0 )
         {
            ch->gshield--;
            if ( ch->gshield == 0 )
               stc("You are now vulnerable to multiple attacks.\n\r",ch);
         }

      if ( ( time_info.hour % 12 ) == 0 )
         {
            if ( IS_SET(ch->more,MORE_DVEIL) )
               REMOVE_BIT(ch->more,MORE_DVEIL);

            if ( IS_SET(ch->newbits, NEW_CLOAK) )
               {
                  send_to_char("Your cloak of death is restored.\n\r",ch);
                  REMOVE_BIT(ch->newbits, NEW_CLOAK);
               }
         }

      if ( IS_SET( ch->more, MORE_HEX ) )
         {
            REMOVE_BIT( ch->more, MORE_HEX );
            send_to_char("#PYou can speak properly again.#n\n\r",ch);
            act("#P$n #Pcan speak properly again.#n", ch, NULL, NULL, TO_ROOM );
         }

      if ( ( time_info.hour % 3 ) == 0 )
         {
            if ( IS_SET( ch->more2, MORE2_REVWALK ) )
               {
                  REMOVE_BIT( ch->more2, MORE2_REVWALK );
                  send_to_char("#CYou now have control of your feet again.#n\n\r",ch);
                  act("#C$n #Cnow has control of $s feet again.#n", ch, NULL, NULL, TO_ROOM );
               }
         }

      if ( ( time_info.hour % 4 ) == 0 )
         {
            if ( IS_SET( ch->elemental, ELE_NONE ) )
               {
                  act("#wYou are now capable of executing #Pelemental #Rattacks again.#n",ch,NULL,NULL,TO_CHAR);
                  act("#w$n is now capable of executing #Pelemental #Rattacks again.#n",ch,NULL,NULL,TO_ROOM);
                  REMOVE_BIT( ch->elemental, ELE_NONE );
               }

            if ( IS_SET( ch->elemental, ELE_BTHROUGH ) )
               {
                  act("#CYou clench your fist as you #wshatter #Cthe #ecurse #Cshrouding your #Presilience#C.#n",ch,NULL,NULL,TO_CHAR);
                  act("#C$n clenches $s fist as $e #wshatters #Cthe #ecurse #Cshrouding $s #Presilience#C.#n",ch,NULL,NULL,TO_ROOM);
                  REMOVE_BIT( ch->elemental, ELE_BTHROUGH );
               }

            if ( IS_SET(ch->more2, MORE2_SHADOW) )
               REMOVE_BIT(ch->more2, MORE2_SHADOW);

            if ( IS_SET(ch->more2,MORE2_CHANCE) )
               REMOVE_BIT(ch->more2,MORE2_CHANCE);

		if ( IS_EXTRA(ch, EXTRA_ROT) )
               {
                   send_to_char("Your flesh feels better.\n\r", ch);
                   REMOVE_BIT(ch->extra, EXTRA_ROT);
               }

            if ( IS_SET(ch->flag2, AFF2_BALOR) )
               REMOVE_BIT(ch->flag2, AFF2_BALOR);

            if ( IS_SET(ch->more, MORE_POISON) )
               {
                  REMOVE_BIT(ch->more, MORE_POISON);
                  stc("Your body feels stronger as the deadly poison leaves you.\n\r",ch);
               }

            if ( IS_SET(ch->more, MORE_BLOOD) )
               {
	            stc("Your wounds is healed as you pull the devastating weapon from your body.\n\r",ch);
	            act("$n removes the devastating stiletto from $s body.",ch,NULL,NULL,TO_ROOM);
	            REMOVE_BIT(ch->more, MORE_BLOOD);
               }

            if ( IS_SET(ch->garou1, GAROU_PAWS) )
               {
                  REMOVE_BIT(ch->garou1, GAROU_PAWS);
                  stc("Your body feels stronger as the deadly curse leaves you.\n\r",ch);
               }

            if ( IS_SET(ch->more, MORE_DISEMPOWER) )
               REMOVE_BIT(ch->more, MORE_DISEMPOWER);
         }

      if ( IS_CLASS( ch, CLASS_NINJA ) && IS_SET( ch->more2, MORE2_DIG ) )
         {
            act("You burst from beneath the ground as you gasp loudly for breath.",ch,NULL,NULL,TO_CHAR);
            act("$n bursts from beneath the ground as $e gasps loudly for breath.",ch,NULL,NULL,TO_ROOM);

            REMOVE_BIT( ch->more2, MORE2_DIG );
            return;
         }

      if ( IS_SET( ch->more, MORE_HAMMER ) && ch->fight_timer == 0 )
         {
            act( "With a grunt, you finally manage to pull the #gwretched #eaxe#n from your feet.", ch, NULL, NULL, TO_CHAR );
            act( "With a grunt, $n finally manages to pull the #gwretched #eaxe#n from $s feet.", ch, NULL, NULL, TO_ROOM );
            REMOVE_BIT( ch->more, MORE_HAMMER );
         }

      if ( ch->class_3 != 0 )
         timer = 8;
         else timer = 4;

      if ( ( time_info.hour % timer ) == 0 )
         {
            if ( IS_SET(ch->more,MORE_NINJA) )
               REMOVE_BIT(ch->more,MORE_NINJA);

            if ( IS_SET(ch->more,MORE_VEIL) )
               REMOVE_BIT(ch->more,MORE_VEIL);

            if ( IS_SET(ch->more,MORE_SMOKE) )
               REMOVE_BIT(ch->more,MORE_SMOKE);

            if ( IS_SET(ch->extra, EXTRA_DANCE) )
               REMOVE_BIT(ch->extra, EXTRA_DANCE);

            if ( IS_SET(ch->more3, MORE3_CLAN1) )
               REMOVE_BIT(ch->more3, MORE3_CLAN1);

            if ( IS_SET(ch->more3, MORE3_CLAN2) )
               REMOVE_BIT(ch->more3, MORE3_CLAN2);

            if ( IS_SET(ch->more3,MORE3_RAGE) )
               REMOVE_BIT(ch->more3,MORE3_RAGE);

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
         }

      if ( IS_SET(ch->more3, MORE3_CONFUSE) && ch->fight_timer == 0 )
         REMOVE_BIT(ch->more3, MORE3_CONFUSE);

      if ( IS_SET(ch->more, MORE_CONFUSION) )
         REMOVE_BIT(ch->more, MORE_CONFUSION);

	if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
        {
          stc("The earth surrounding you starts to slowly crack.\n\r",ch);
	  SET_BIT(ch->mflags, MAGE_EMBRACED1);
          REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
        }
        else if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
        {
          stc("The earth surrounding you falls away.\n\r",ch);
          REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
        }

      if ( !IS_NPC( ch ) && !( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
           && IS_SET( ch->arena, AFF2_INARENA ) ) && ch->fight_timer == 0 && ch->in_room != NULL
           && ( ch->in_room->vnum < 33500 || ch->in_room->vnum > 33800 ) )
         {
            if ( ch->position > POS_STUNNED && !is_obj )
               {
                  hit_gain( ch, ch->max_hit * 0.25 );
                  mana_gain( ch, ch->max_mana * 0.25 );
                  move_gain( ch, ch->max_move * 0.25 );
               }
         }

      gain_condition( ch, COND_DRUNK,  -1 );

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL && !is_obj )
	{
	    OBJ_DATA *obj;

	    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    || ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 ) )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    --ch->in_room->light;
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	    }

          ch->timer += 1;

	    if ( ch->pcdata->capture_timer == 0 && ch->timer >= 12
               && !( armageddon && !ch->desc ) )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		}
	    }

	    if ( ch->pcdata->capture_timer == 0 && ch->timer > 30 ) ch_quit = ch;
	}

  	  for ( paf = ch->affected; paf != NULL; paf = paf_next )
	  {
	    paf_next	= paf->next;

	    if ( paf->duration > 0 )
	       paf->duration--;
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off && !is_obj)
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
		    }
		}
	  
		affect_remove( ch, paf );
	    }
        }

     if ( ch->position == POS_STUNNED )
        {
           int new_total = 0;

           if ( IS_CLASS(ch, CLASS_ASSASSIN) && IS_SET( ch->special, SPC_KNIGHT ) && ch->pkill > 19 )
              new_total += 1000000;

           if ( IS_CLASS(ch, CLASS_FIEND) && IS_SET( ch->more, MORE_WALL ) )
              new_total += 1000000;

           if ( IS_SET( ch->more3, MORE3_DRAGON ) )
              new_total += 750000;

           if ( ch->hit > 0 - new_total )
              {
                  ch->position = POS_STANDING;
                  act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM);
                  act( "You clamber back to your feet.", ch, NULL, NULL, TO_CHAR);
              }
        }

      if ( ( IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_SHADOW_WAR) ) && ch->ctimer[8] > 0 )
         continue;

      if ( ch->air_timer > 0 || ( ch->ctimer[8] > 0 && ( IS_CLASS( ch, CLASS_MONK ) || IS_CLASS( ch, CLASS_DRAGOON ) ) ) )
         continue;

      if ( is_affected( ch,skill_lookup("despondency") ) )
         {
            AFFECT_DATA af;

            stc("You scream loudly as the Darkside devours your life and soul.\n\r",ch);
            act("$n screams loudly as a mystical force rips apart $s mortal body.",ch,NULL,NULL,TO_ROOM);


            af.type = skill_lookup("despondency");

            af.location  = APPLY_STR;
            af.modifier  = -2;
            af.duration  = 0;
            af.bitvector = 0;
	
            affect_join( ch, &af );

            if ( ch->hit > 1 )
               {
                  ch->hit = UMAX( 1, ch->hit - ( ch->max_hit / 50 ) );
                  update_pos( ch );
               }
         }

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */
	if ( !IS_NPC( ch ) && ch->loc_hp[6] > 0 && !is_obj && ch->in_room != NULL )
	{
	    int dam = 0;
	    int minhit = 0;
	    if (!IS_NPC(ch)) minhit = -11;
	    if (IS_BLEEDING(ch,BLEEDING_HEAD) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's neck.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your neck.\n\r", ch );
		dam += number_range(20,50);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_THROAT) && (ch->hit-dam) > minhit )
	    {
		act( "Blood pours from the slash in $n's throat.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "Blood pours from the slash in your throat.\n\r", ch );
		dam += number_range(10,20);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_HERO(ch)) { ch->hit = ch->hit - dam;
		if (ch->hit < 1) ch->hit = 1; }
	    else ch->hit = ch->hit - dam;
	    update_pos(ch);
	    ch->in_room->blood += dam;
	    if (ch->in_room->blood > 1000) ch->in_room->blood = 1000;
	    if (ch->hit <=-11 || (IS_NPC(ch) && ch->hit < 1))
	    {
		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}

	if ( ch->position == POS_INCAP && !is_obj && !drop_out )
	{
          ch->hit = ch->hit + number_range(3,5);
	    update_pos( ch );
            if (ch->position > POS_INCAP)
            {
                act( "$n's wounds stop bleeding and seal up.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "Your wounds stop bleeding and seal up.\n\r", ch );
            }
            if (ch->position > POS_STUNNED)
            {
                act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You clamber back to your feet.\n\r", ch );
            }
	}
	else if ( ch->position == POS_MORTAL && !is_obj && !drop_out )
	{
	    drop_out = FALSE;
          ch->hit = ch->hit + number_range(3,5);

	    if (!drop_out)
	    {
	    	update_pos( ch );
            	if (ch->position == POS_INCAP)
            	{
                    act( "$n's wounds begin to close, and $s bones pop back into place.", ch, NULL, NULL, TO_ROOM );
                    send_to_char( "Your wounds begin to close, and your bones pop back into place.\n\r", ch );
            	}
	    }
	}
	else if ( ch->position == POS_DEAD && !is_obj && !drop_out )
	{
	   update_pos(ch);
	   if (!IS_NPC(ch))
		do_killperson(ch,ch->name);
	}
      else if ( IS_AFFECTED(ch, AFF_FLAMING) && !is_obj && !drop_out && ch->in_room != NULL )
	{
	    if (IS_NPC(ch)) continue;

	    act( "#y$n#y's #Rflesh burns and #ecrisps#R.#n", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "#yYour #Rflesh burns and #ecrisps#R.#n\n\r", ch );

          ch->hit = UMIN( ch->hit + UMIN( is_elemental( ATTACK_FIRE, ch, 100000 ) * -1, 250000 ), ch->max_hit );
	    update_pos(ch);

	    if (ch->hit <= 0)
             {
		    drop_out = TRUE;
                REMOVE_BIT(ch->affected_by, AFF_FLAMING);
             }
	}
	drop_out = FALSE;

      if ( ch->in_room != NULL && ch->position < POS_STUNNED )
         {
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
         }
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    if ( ch_save != NULL || ch_quit != NULL )
    {
	for ( ch = char_list; ch != NULL; ch = ch_next )
	{
	    ch_next = ch->next;
	    if ( ch == ch_save )
		save_char_obj( ch );
	    if ( ch == ch_quit )
		do_quit( ch, "" );
	}
    }

    return;
}

/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;

	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

      if ( IS_SET( obj->quest, QUEST_ARTIFACT ) )
         {
            char buf[MAX_INPUT_LENGTH];

            sprintf( buf, "#w%s #ghas been #sr#se#sc#sl#sa#si#sm#se#sd #gby the #Pimmortals#g!#n", obj->short_descr );
            do_info( NULL, buf );
         }

	switch ( obj->item_type )
	{
         default:              message = "$p vanishes.";         	break;
         case ITEM_FOUNTAIN:   message = "$p dries up.";         	break;
         case ITEM_CORPSE_NPC: message = "$p decays into dust."; 	break;
         case ITEM_CORPSE_PC:  message = "$p decays into dust."; 	break;
         case ITEM_FOOD:       message = "$p decomposes.";		break;
         case ITEM_TRASH:      message = "$p crumbles into dust.";	break;
         case ITEM_EGG:        message = "$p cracks open.";		break;
         case ITEM_WEAPON:     message = "$p turns to fine dust and blows away."; break;
         case ITEM_WALL:       message = "$p flows back into the ground.";break;
	}

	if ( obj->carried_by != NULL && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
	{
	    act( message, obj->carried_by, obj, NULL, TO_CHAR );
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
	{
	    act( message, rch, obj, NULL, TO_ROOM );
	    act( message, rch, obj, NULL, TO_CHAR );
	}

	/* If the item is an egg, we need to create a mob and shell!
	 * KaVir
	 */
	if ( obj->item_type == ITEM_EGG && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED))
	{
	    CHAR_DATA      *creature;
	    OBJ_DATA       *egg;
	    if ( get_mob_index( obj->value[0] ) != NULL )
	    {
		if (obj->carried_by != NULL && obj->carried_by->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->carried_by->in_room);
		}
		else if (obj->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->in_room);
		}
		else
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,get_room_index(ROOM_VNUM_HELL));
		}
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, creature->in_room );
	    	act( "$n clambers out of $p.", creature, obj, NULL, TO_ROOM );
	    }
	    else if (obj->in_room != NULL)
	    {
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, obj->in_room );
	    }
	}

        if (IS_OBJ_STAT2(obj, ITEM_DAEMONSEED)
	    && obj != NULL
	    && obj->in_obj == NULL
	    && (locate_obj(obj))->people )
	{
	    char buf[MAX_STRING_LENGTH];
	    CHAR_DATA *vch;
          int dam;

	    sprintf(buf, "%s suddenly explodes in a ball of flame, incinerating you!\n\r",obj->short_descr);
	    buf[0] = UPPER(buf[0]);
	    if ((locate_obj(obj))->people == NULL) break;
	    for ( vch = (locate_obj(obj))->people; vch != NULL;vch=vch->next_in_room )
    	    {
		if (vch->class == 0 || (!IS_NPC(vch) && vch->level < 3)) continue;
	    if ( IS_SET(vch->in_room->room_flags,ROOM_SAFE) )
	    {
		stc("You are unaffected by the blast.\n\r",vch);
		continue;
	    }
            dam = 2000000;
            dam = is_elemental( ATTACK_FIRE, vch, dam );
            vch->hit = UMAX( 1, vch->hit - dam );
		send_to_char(buf, vch);
            sprintf(buf,"The flames strike you incredibly hard![%d]\n\r", dam);
		stc(buf,vch);
 	    }
	}

	if (obj != NULL) extract_obj( obj );
    }
}



/*
 */
void aggr_update( void )
{
    CHAR_DATA *ch, *victim, *vch;
    CHAR_DATA *ch_next, *vch_next;
    OBJ_DATA *obj = NULL;
    OBJ_DATA *chobj = NULL;
    ROOM_INDEX_DATA *objroom = NULL;
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( ( ch = d->character ) == NULL )
         continue;

      if ( ch->in_room != NULL && ch->in_room->vnum >= 33500 && ch->in_room->vnum <= 33800
           && ch->position == POS_MORTAL )
         {
            int vnum = ch->in_room->vnum - 33500;

            while ( vnum >= 20 )
            {
               vnum -= 20;
            }

            restore_char( ch );
            ch->position = POS_STANDING;
            stc( "#yYou #wslam your #ofist #winto the ground as you now realize your #Rmistakes#w.#n\n\r", ch );
            ch->safe_timer = 1;            

            if ( vnum == 19 )
               check_boss( ch );

            vnum = 33500 + ( ( ( ch->in_room->vnum - 33500 ) / 20 ) * 20 );
            char_from_room( ch );
            char_to_room( ch, get_room_index( vnum ) ); 
         }

      if ( ch->stunned > 0 && IS_SET( ch->cmbt[7], SKL_STUNA ) )
         {
            stc("#wYou #gno longer feel #yd#oizz#yy#g.#n\n\r", ch );
            act("#w$n #gno longer feels #yd#oizz#yy#g.#n", ch, NULL, NULL, TO_ROOM );
            ch->stunned = 0;
         }

      if ( IS_SET( ch->more, MORE_FROZEN ) && IS_SET( ch->cmbt[5], SKL2_THEART ) )
         {
            if ( IS_CLASS( ch, CLASS_PHOENIX ) && ch->class_2 == 0 )
               ch->hit = UMAX( 1, ch->hit - is_elemental( ATTACK_ICE, ch, 1000000 ) );

            REMOVE_BIT( ch->more, MORE_FROZEN );
            send_to_char("#bThe #wice #bencasing you melts away.#n\n\r",ch);
            act("#bThe #wice #bencasing $n #bmelts away.#n",ch,NULL,NULL,TO_ROOM);
            ch->freeze_timer = 0;
         }

      if ( ch->mana > ch->max_mana || IS_AFFECTED(ch, AFF_MANA) )
         ch->mana = ch->max_mana;

      if ( ch->move > ch->max_move || IS_AFFECTED(ch, AFF_MOVE) )
         ch->move = ch->max_move;

      if ( ch->hit > ch->max_hit )
         ch->hit = ch->max_hit;

	if (( d->connected == CON_PLAYING
	||    d->connected == CON_EDITING )
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&&   ch->pcdata->chobj != NULL )
	{
          obj = ch->pcdata->chobj;
	    if (obj->in_room != NULL)
	    	objroom = obj->in_room;
	    else if (obj->in_obj != NULL)
	    	objroom = get_room_index(ROOM_VNUM_IN_OBJECT);
	    else if (obj->carried_by != NULL)
	    {
		if (obj->carried_by != ch && obj->carried_by->in_room != NULL)
		    objroom = obj->carried_by->in_room;
		else continue;
	    }
	    else continue;
	    if (ch->in_room != objroom && objroom != NULL)
	    {
	    	char_from_room(ch);
	    	char_to_room(ch,objroom);
		do_look(ch,"auto");
	    }
	}
	else if (( d->connected == CON_PLAYING
	||         d->connected == CON_EDITING )
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&&   (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0) )
	{
	    if (ch->pcdata->obj_vnum != 0)
	    {
		bind_char(ch);
		continue;
	    }
	    if (IS_HEAD(ch,LOST_HEAD))
	    {
	    	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	    	send_to_char("You are able to regain a body.\n\r",ch);
	    	ch->position = POS_RESTING;
	    	ch->hit = 1;
	    } else {
	    	send_to_char("You return to your body.\n\r",ch);
		REMOVE_BIT(ch->extra,EXTRA_OSWITCH);}
	    REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	    if ( ( chobj = ch->pcdata->chobj ) != NULL )
		chobj->chobj = NULL;
	    ch->pcdata->chobj = NULL;
	    do_look(ch,"auto");
	}
	continue;
    }

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
       int count;

       ch_next = ch->next;

       if ( ch->in_room == NULL ) continue;

       if ( IS_NPC( ch ) && ch->master != NULL && !IS_NPC( ch->master ) && ch->master->in_room != ch->in_room
            && ch->position != POS_STUNNED && ch->pIndexData->vnum != 1 )
          {
             if ( ch->position == POS_FIGHTING )
                stop_fighting( ch, TRUE );

             char_from_room( ch );
             char_to_room( ch, ch->master->in_room );
          }

       if ( !IS_NPC( ch ) || !IS_SET( ch->act, ACT_AGGRESSIVE )
            || ch->fighting != NULL
            || ch->air_timer > 0
            || IS_AFFECTED( ch, AFF_CHARM )
            || !IS_AWAKE( ch ) )
          continue;

       /*
        * Ok we have a 'wch' player character and a 'ch' npc aggressor.
        * Now make the aggressor fight a RANDOM pc victim in the room,
        *   giving each 'vch' an equal chance of selection.
        */

       count = 0;
       victim = NULL;

       for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
       {
          vch_next = vch->next_in_room;

          if ( !IS_NPC( vch ) && !is_safe( ch, vch )
               && vch->pcdata != NULL
               && vch->level < LEVEL_IMMORTAL
               && vch->position > POS_MORTAL
               && can_see( ch, vch ) )
             {
                if ( number_range( 0, count ) == 0 )
                   victim = vch;
           
                count++;
             }
       }

       if ( victim == NULL )
          continue;

       // attacks person here
       multi_hit( ch, victim, TYPE_UNDEFINED );
    }
}
 
void embrace_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *ch;
    CHAR_DATA       *victim;
    int blpr;  /* variable to check for amout of blood sucked. Shakti */
    char buf[MAX_STRING_LENGTH];

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
      if ( ( ch = d->character) == NULL )
	continue;

      if ( IS_SET( ch->more3, MORE3_SOUL ) )
         {
            act("One of your images fade in and out of existance.",ch,NULL,NULL,TO_CHAR);
            act("One of $n's images fade in and out of existance.",ch,NULL,NULL,TO_ROOM);
         }

      if ( IS_CLASS(ch, CLASS_MONK) && ch->fighting == NULL && ch->monkblock > 0 )
         {
 	      ch->monkblock -= 10;
            if ( ch->monkblock < 0 )
               ch->monkblock = 0;
         }

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) && 100 > ch->focus[CURRENT] )
        {
  
            if ( ch->fight_timer == 0 )
               ch->focus[CURRENT] += 30;
               else ch->focus[CURRENT] += 5;
        
            if ( ch->focus[CURRENT] > 100 )
               ch->focus[CURRENT] = 100;
        }

	 if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DAYWALKER) && 100 > ch->dlife[CURRENT] )
        {
            if ( ch->fight_timer == 0 )
               ch->dlife[CURRENT] = 100;
               else ch->dlife[CURRENT] = UMIN( 100, ch->dlife[CURRENT] + 15 );
        }

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) && 100 > ch->focus[CURRENT] )
         ch->focus[CURRENT] = UMIN( 100, ch->focus[CURRENT] + 10 );

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) && 200 > ch->chi[CURRENT] )
         ch->chi[CURRENT] = UMIN( 200, ch->chi[CURRENT] + 10 );

      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE)
          && 100 > ch->vlife[CURRENT] )
        {
            ch->vlife[CURRENT] += 5;

            if ( ch->vlife[CURRENT] > 100 )
               ch->vlife[CURRENT] = 100;
        }

	if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) 
	  && ch->gnosis[GMAXIMUM] > 0 && ch->gnosis[GMAXIMUM]
	   > ch->gnosis[GCURRENT] )
	{
	  if ( ch->position <= POS_SLEEPING )
	    ch->gnosis[GCURRENT] += number_range(20,30);
	  else if ( ch->position <= POS_RESTING )
	    ch->gnosis[GCURRENT] += number_range(10,20);
	  else ch->gnosis[GCURRENT] += 10 + UMIN( 5, ch->pkill / 4 );

	  if(ch->gnosis[GCURRENT] > ch->gnosis[GMAXIMUM] ) ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM];
	}

      if ( IS_NPC(ch) || ch->embracing == NULL )
        continue;

      ch->embracing->embraced = NULL;
      ch->embracing = NULL;

    continue; 
  }
  return;
}

void ww_update( void )
{
}


/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int     pulse_ww;
    static  int     pulse_embrace;


    if ( --pulse_ww       <= 0 )
    {
        pulse_ww        = PULSE_WW;
        ww_update       ( );
    }

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 );
	area_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
      auction_progess   ( );
    }

    if ( --pulse_embrace <= 0)
    {
        pulse_embrace = PULSE_EMBRACE;
        embrace_update ( ); 
    }

    if ( --pulse_point    <= 0 )
    {
	pulse_point = PULSE_TICK;
	weather_update( );
      parse_who ( );
	char_update	( );
	obj_update	( );
      room_update	( );
    }

    aggr_update( );
    tail_chain( );
    return;
}


void update_drunks(CHAR_DATA *ch)
{
	// I strongly suggest adding more stuff to drunks -xiphias *grin*
	if (ch->pcdata->condition[COND_DRUNK] > 10 && number_range(1,10) == 1)
	{
		send_to_char("You hiccup loudly.\n\r",ch);
		act("$n hiccups.",ch,NULL,NULL,TO_ROOM);
	}
}

void sex_update(CHAR_DATA *ch)
{
	if (ch->pcdata->stage[0] > 0 || ch->pcdata->stage[2] > 0)
	{
		CHAR_DATA *vch;
		if (ch->pcdata->stage[1] > 0 && ch->pcdata->stage[2] >= 225)
		{
			ch->pcdata->stage[2] += 1;
			if ( ( vch = ch->pcdata->partner ) != NULL &&
			!IS_NPC(vch) && vch->pcdata->partner == ch 
			&& ((vch->pcdata->stage[2] >= 200 && vch->sex == SEX_FEMALE) 
			|| (ch->pcdata->stage[2] >= 200 && ch->sex == SEX_FEMALE)))
			{
				if (ch->in_room != vch->in_room)return ;
				if (vch->pcdata->stage[2] >= 225 
				&& ch->pcdata->stage[2] >= 225 
				&& vch->pcdata->stage[2] < 240 
				&& ch->pcdata->stage[2] < 240)
				{
					ch->pcdata->stage[2] = 240;
					vch->pcdata->stage[2] = 240;
				}
				if (ch->sex == SEX_MALE && vch->pcdata->stage[2] >= 240)
				{
					act("You thrust deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_CHAR);
					act("$n thrusts deeply between your warm, damp thighs.",ch,NULL,vch,TO_VICT);
					act("$n thrusts deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_NOTVICT);
					if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
						ch->pcdata->stage[2] = vch->pcdata->stage[2];
				}
				else if (ch->sex == SEX_FEMALE && vch->pcdata->stage[2] >= 240)
				{
					act("You squeeze your legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_CHAR);
					act("$n squeezes $s legs tightly around you, moaning loudly.",ch,NULL,vch,TO_VICT);
					act("$n squeezes $s legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_NOTVICT);
					if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
						ch->pcdata->stage[2] = vch->pcdata->stage[2];
				}
			}
			if (ch->pcdata->stage[2] >= 250)
			{
				if ( ( vch = ch->pcdata->partner ) != NULL &&
					!IS_NPC(vch) && vch->pcdata->partner == ch &&
					ch->in_room == vch->in_room)
				{
					vch->pcdata->stage[2] = 250;
					if (ch->sex == SEX_MALE)
					{
						stage_update(ch,vch,2,"xm-thrust");
						stage_update(vch,ch,2,"xf-squeeze");
					}
					else
					{
						stage_update(vch,ch,2,"xm-thrust");
						stage_update(ch,vch,2,"xf-squeeze");
					}
											
					ch->pcdata->stage[0] = 0;
					vch->pcdata->stage[0] = 0;

                              if ( number_percent() > 60 )
                                 {
                                    send_to_char("Congratulations on achieving a simultanious orgasm!\n\r",ch);
                                    send_to_char("Congratulations on achieving a simultanious orgasm!\n\r",vch);
                                 }
				}
			}
		}
		else
		{
			if (ch->pcdata->stage[0] > 0 && ch->pcdata->stage[2] < 1 &&
				ch->position != POS_RESTING) 
			{
				if (ch->pcdata->stage[0] > 1)
					ch->pcdata->stage[0] -= 1;
				else
					ch->pcdata->stage[0] = 0;
			}
			else if (ch->pcdata->stage[2]>0 && ch->pcdata->stage[0] < 1)
			{
				if (ch->pcdata->stage[2] > 10)
					ch->pcdata->stage[2] -= 10;
				else
					ch->pcdata->stage[2] = 0;
				if (ch->sex == SEX_MALE && ch->pcdata->stage[2] == 0)
					send_to_char("You feel fully recovered.\n\r",ch);
			}
		}
	}
}

void update_safe_powers(CHAR_DATA *ch)
{
  return;

  if ( IS_SET(ch->newbits, NEW_DARKNESS) && ch->fight_timer > 0 )
  {
    if (ch->mana >= 10000) ch->mana -= 10000;
    else
    {
      if ( IS_CLASS(ch, CLASS_DROW) && ch->in_room->drows > 0 )
         ch->in_room->drows--;

      if ( ch->in_room->drows == 0 )
         {
            REMOVE_BIT(ch->newbits, NEW_DARKNESS);
            REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
         }

      send_to_char("You cannot pay the upkeep and banish your cloak of darkness.\n\r", ch );
      act("The cloak of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
    }
  }

  return;
}

void update_angel(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 712 && ch->in_room->vnum <= 717 ))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }

  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
     werewolf_regen(ch, 1);

  regen_limb(ch);
}

void update_demon(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 700 && ch->in_room->vnum <= 711))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 2);

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 2);

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 2);

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 2);
  }

  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
     werewolf_regen(ch, 2);

  regen_limb(ch);
}

void update_drow(CHAR_DATA *ch)
{
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move
       || ch->revregen > 0 )
     werewolf_regen(ch, 1);

  regen_limb(ch);
}

void update_dragon(CHAR_DATA *ch)
{
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
     werewolf_regen(ch, 2);

  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
     werewolf_regen(ch, 2);

  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
     werewolf_regen(ch, 2);

  regen_limb(ch);
}

void update_kingdom(CHAR_DATA *ch)
{
  if ( IS_NPC(ch) || ch->pcdata->kingdom == 0 )
     return;

  if ( ( ch->pcdata->kingdom == 1 && ch->in_room->vnum == 90008 )
       || ( ch->pcdata->kingdom == 2 && ch->in_room->vnum == 90108 )
       || ( ch->pcdata->kingdom == 3 && ch->in_room->vnum == 90208 )
       || ( ch->pcdata->kingdom == 4 && ch->in_room->vnum == 90308 )
       || ( ch->pcdata->kingdom == 5 && ch->in_room->vnum == 90408 )
       || ( ch->pcdata->kingdom == 6 && ch->in_room->vnum == 90508 )
       || ( ch->pcdata->kingdom == 7 && ch->in_room->vnum == 90608 )
       || ( ch->pcdata->kingdom == 8 && ch->in_room->vnum == 90708 )
       || ( ch->pcdata->kingdom == 9 && ch->in_room->vnum == 90808 )
       || ( ch->pcdata->kingdom == 10 && ch->in_room->vnum == 90908 ) )
     {
        if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
           werewolf_regen(ch, 2);

        if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
           werewolf_regen(ch, 2);

        if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
           werewolf_regen(ch, 2);

        regen_limb(ch);
        regen_limb(ch);
     }
}

void update_mage(CHAR_DATA *ch)
{
  if (ch->position == POS_MEDITATING && ch->mana < ch->max_mana)
  {
    ch->mana += number_range(10000,30000);
    if (ch->mana > ch->max_mana)
      ch->mana = ch->max_mana;
  }
  return;
}

void update_arti_regen(CHAR_DATA *ch)
{
  if (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_knight (CHAR_DATA *ch)
{
  if (ch->pcdata->powers[POWER_WORD] > 0) ch->pcdata->powers[POWER_WORD]--;
  if (ch->pcdata->powers[POWER_FLAME] > 0) ch->pcdata->powers[POWER_FLAME]--;
  if (ch->pcdata->powers[POWER_BLIND] > 0) ch->pcdata->powers[POWER_BLIND]--;
  if (ch->pcdata->powers[POWER_FREEZE] > 0) ch->pcdata->powers[POWER_FREEZE]--;
  if (ch->pcdata->powers[POWER_CHILL] > 0) ch->pcdata->powers[POWER_CHILL]--;
  if (ch->pcdata->powers[POWER_BLAST] > 0) ch->pcdata->powers[POWER_BLAST]--;
  if (ch->lightning_timer > 0) ch->lightning_timer--;
  if (ch->fear_timer > 0) ch->fear_timer--;
  if (ch->poison_timer > 0) ch->poison_timer--;
}

void regen_limb(CHAR_DATA *ch)
{
  if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
       && IS_SET( ch->arena, AFF2_INARENA ) && arena == FIGHT_SPECIAL_KO )
     return;

  if (ch->loc_hp[6] > 0)
  {
    int sn = skill_lookup( "clot" );
    spell_clot( sn, 60, ch, ch );
  }
  else if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
    reg_mend(ch);
  return;
}

void werewolf_regen( CHAR_DATA *ch, int multiplier )
{
   int divider = 10;
   bool ltouch = FALSE, zombie = FALSE;

   if ( ch->agg > 0 )
      {
         if ( ch->air_timer > 0 || ( ( IS_CLASS( ch, CLASS_DRAGOON ) || IS_CLASS( ch, CLASS_MONK ) ) && ch->ctimer[8] > 0 )
              || ch->fighting != NULL )
            return;

         if ( !IS_NPC( ch ) && IS_CLASS( ch, CLASS_KURUDA ) && IS_SET( ch->pcdata->powers[MONK_SKILL], MONK_INVIN ) )
            return;

         if ( IS_NPC( ch ) )
            ch->agg -= 30;
            else ch->agg -= 15;

         if ( ch->agg <= 0 )
            {
               ch->agg = 0;
               stc("#rYou body have regenerated all #paggravated #edamage#r.#n\n\r",ch);
            }

        return;
     }

  if ( ch->in_room != NULL && ( ch->in_room->vnum == 33519 || ch->in_room->vnum == 33539 || ch->in_room->vnum == 33559
       || ch->in_room->vnum == 33579 || ch->in_room->vnum == 33599 || ch->in_room->vnum == 33619
       || ch->in_room->vnum == 33639 || ch->in_room->vnum == 33659 || ch->in_room->vnum == 33679
       || ch->in_room->vnum == 33699 ) )
     return;

  if ( ch->fighting != NULL || IS_SET( ch->newbits, NEW_ALIGHT ) )
     return;

  if ( ( IS_SET( ch->arena, AFF2_TEAM1 ) || IS_SET( ch->arena, AFF2_TEAM2 ) ) 
       && IS_SET( ch->arena, AFF2_INARENA ) )
     return;

  if ( ( IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_SHADOW_WAR) ) && ch->ctimer[8] > 0 )
     return;

  if ( IS_ITEMAFF( ch, ITEMA_REGENERATE ) || IS_CLASS( ch, CLASS_WEREWOLF ) )
     divider = 8;

  if ( ch->revregen == 0 )
     {
        if ( IS_SET( ch->extra, EXTRA_LTOUCH ) )
           {
              REMOVE_BIT( ch->extra, EXTRA_LTOUCH );
              ltouch = TRUE;
           }

        if ( !IS_NPC( ch ) && IS_SET( ch->extra, EXTRA_ZOMBIE ) )
           {
              REMOVE_BIT( ch->extra, EXTRA_ZOMBIE );
              zombie = TRUE;
           }

        hit_gain( ch, ( ch->max_hit / divider ) );

        if ( ltouch )
           SET_BIT( ch->extra, EXTRA_LTOUCH );

        if ( zombie )
           SET_BIT( ch->extra, EXTRA_ZOMBIE );

        mana_gain( ch, ( ch->max_mana / divider ) );
        move_gain( ch, ( ch->max_move / divider ) );
     }
     else {
             divider *= 2;
             ch->hit -= ( ch->max_hit / divider );
             ch->mana = UMAX(ch->mana - ( ch->max_mana / divider ), 0);
             ch->move = UMAX(ch->move - ( ch->max_move / divider ), 0);
          }

  update_pos(ch);
}
