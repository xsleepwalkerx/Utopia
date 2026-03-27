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

void do_scavenger( CHAR_DATA *ch, char *argument )
{
   if ( ( get_age( ch )-17 ) < 2 )
      {
         stc( "But you are a newbie.\n\r",ch );
         return;
      }

   if ( scavenger_time == 0 )
      {
         stc( "A Scavenger Hunt isn't on atm.\n\r", ch );
         return;
      }

   if ( IS_SET( ch->extra, EXTRA_HUNT ) )
      {
         stc( "You are already in the hunt.\n\r", ch );
         return;
      }

   stop_fighting( ch, TRUE );
   restore_char( ch );
   SET_BIT( ch->extra, EXTRA_HUNT );
   ch->scavenger_item = 0;
   stc( "#wYou have #yjoined #wthe #ghunt#w! #PGood #yluck #Ckid#w.#n\n\r", ch );
}

void start_scavenger( void )
{
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;
   int count, vnum;
   ROOM_INDEX_DATA *room;
   char buf[MAX_STRING_LENGTH];

   if ( scavenger_time > 0 )
      return;

   scavenger_items = 50;
   scavenger_time = 24;

   for ( count = 0; count < 50; count++ )
   {
     for ( ; ; )
     {
        vnum = number_range( 0, 30100 );

        if ( vnum > 30000 && !( vnum >= 30049 && vnum < 30071 ) )
           continue;

        if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
          continue;

        break;
     }

     obj = create_object( pObjIndex, 50 );
     obj->item_type = ITEM_HUNT;

     if ( IS_SET( obj->quest, QUEST_ARTIFACT ) )
        REMOVE_BIT( obj->quest, QUEST_ARTIFACT );

     sprintf( buf, "%s scavenger", obj->name );
     free_string( obj->name );
     obj->name = str_dup( buf );

     for ( ; ; )
     {
        room = get_room_index( number_range( 0, 32000 ) );

        if ( room == NULL )
           continue;

        if ( room->vnum < 100 || room->vnum == 651 || ( room->vnum >= 550 && room->vnum < 633 ) )
           continue;

        break;
     }

     obj_to_room( obj, room );
     obj->timer = 24;
   }
}

void stop_scavenger( void )
{
   char buf[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *winner = NULL;
   int highest = 0;
   int vnum;
   int value;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->character == NULL || !IS_SET( d->character->extra, EXTRA_HUNT ) )
         continue;

      REMOVE_BIT( d->character->extra, EXTRA_HUNT );

      if ( d->character->scavenger_item == 0 )
         continue;
 
      if ( d->character->scavenger_item > highest )
         {
            winner = d->character;
            highest = d->character->scavenger_item;
         }

      if ( d->character->scavenger_item > 4 )
         {
            if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
                 return;

            obj = create_object( pObjIndex, 60 );
            obj_to_char( obj, d->character );

		if ( d->character->scavenger_item > 19 )
               value = 5;
               else value = d->character->scavenger_item / 5;

            obj->cost = 0;
            obj->value[0] = value;
            obj->item_type = ITEM_PKTOKEN;

            if ( obj->questmaker != NULL )
               free_string(obj->questmaker);

            obj->questmaker = str_dup(d->character->name);
            free_string( obj->name );
            obj->name = str_dup( "PK token" );
            sprintf(buf,"a #P%d #CPK #ytoken#n",value);
            free_string( obj->short_descr );
            obj->short_descr = str_dup( buf );
            sprintf(buf,"A %d PK token lies on the floor.",value);
            free_string( obj->description );
            obj->description = str_dup( buf );

            if ( obj->questowner != NULL )
               free_string( obj->questowner );

            obj->questowner = str_dup( d->character->pcdata->switchname );

            if ( !( d->character->generation == 1 && d->character->tier == 5 ) )
               {
                  value = URANGE( 1, d->character->scavenger_item / 3, 3 );

                  if ( d->character->tier == 5 )
                     {
                        if ( d->character->generation == 3 && value > 2 )
                           value = 2;
                           else if ( d->character->generation == 2 && value > 1 )
                                   value = 1;
                     }

                  d->character->generation -= value;

                  if ( d->character->generation < 1 )
                     {
                        stc("#wYou #ghave gone #yup#g the #Cranks#g... #wWell #edone#g.#n\n\r",d->character);
                        d->character->generation = 5 + d->character->generation;
                        d->character->tier++;
                        d->character->max_hit += 1000000;
                        d->character->max_mana += 1000000;
                        d->character->max_move += 1000000;
                     }
               }
         }

      d->character->bank = UMIN( 500000000, d->character->bank + ( d->character->scavenger_item * 100000 ) );
      sprintf( buf, "#gYou have found #w%d #yitems #gand had been awarded #w%d #ygold #gpieces!#n\n\r", d->character->scavenger_item, d->character->scavenger_item * 50000 );
      stc( buf, d->character );
      d->character->scavenger_item = 0;
   }

   scavenger_time = 0;
   scavenger_items = 50;

   if ( winner == NULL )
      {
         do_sinfo( NULL, "#gThe #whunt has #y#zended#n#g!" );
         return;
      }

   sprintf( buf, "#gThe #whunt has #y#zended#n#g! #C%s #gis the winner with #w%d #gitems found!#n", winner->name, highest );
   do_sinfo( NULL, buf );

   find_item( winner, number_range( 0, 2 ) );
}
