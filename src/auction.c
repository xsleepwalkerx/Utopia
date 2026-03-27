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

void do_acinfo( CHAR_DATA *ch, char *argument )
{
   DESCRIPTOR_DATA *d;
    
   if ( argument[0] == '\0' )
      return;

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
     if ( d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->deaf, CHANNEL_INFO) )
        {
          send_to_char( "#L[#w<#oAuction#w>#L] ",d->character );
          send_to_char( argument, d->character );
          send_to_char( " #L[#w<#oAuction#w>#L]#n\n\r",   d->character );
        }
   }
}

void do_bid( CHAR_DATA *ch, char *argument )
{
  int bid;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];

  if ( IS_NPC( ch ) )
     return;

  if ( auction_list == NULL )
     {
        stc( "#wThere isn't anything to bid on at the moment.#n\n\r", ch );
        return;
     }

  if ( ch == auction_list->seller )
     {
        stc( "#wBidding on your own item? don't be stupid.#n\n\r", ch );
        return;
     }

  argument = one_argument(argument, arg);

  if ( arg[0] == '\0' )
     {
        send_to_char( "#RSyntax - #wBid #y<Amount>#n\n\r", ch );
        return;
     }

  if ( !is_number( arg ) )
     {
        stc( "Bids must be an integer number.\n\r", ch );
        return;
     }
     else bid = atoi( arg );

  if ( bid > ch->bank )
     {
        stc( "You don't have the money in the bank to pay this bid...\n\r", ch );
        return;
     }

  if ( bid <= auction_list->current_bid )
     {
        sprintf( buf, "You must beat the current bid of #w%d #ygold#n pieces for your call to be accepted.\n\r", auction_list->current_bid );
        stc( buf, ch );
        return;
     }

  auction_list->buyer = ch;
  auction_list->current_bid = bid;
  auction_list->bidding_time = 20;

  stc( "You raise your hand solemly as you place a bid on the item.\n\r", ch );
  sprintf( buf, "#y%s #Cbids #w%d #ygold #Cpieces on %s#C.", ch->pcdata->switchname,
                auction_list->current_bid, auction_list->object->short_descr );
  do_acinfo( ch, buf );
}

void do_enquire( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
     return;

  if ( auction_list == NULL )
     {
        stc( "There aren't any auctions going on currently.\n\r", ch );
        return;
     }

  if ( auction_list->object == NULL )
     {
        stc( "Somehow, the item being auctioned isn't there... Is this treachery?\n\r", ch );
        return;
     }

  spell_identify( 0, 0, ch, auction_list->object );
}

void do_auction( CHAR_DATA *ch, char *argument )
{
  AUCTION_DATA *item;
  OBJ_DATA *obj;
  int min_bid;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];

  if ( IS_NPC( ch ) )
     return;

  argument = one_argument(argument, arg);

  if ( arg[0] == '\0' )
     {
        send_to_char( "#RSyntax - #wAuction #C<item> #y<Minimum Bid>#n\n\r", ch );
        return;
     }

  if ( !str_cmp( "cancel", arg ) )
     {
        if ( auction_list == NULL )
           {
              stc( "There isn't an auction happening atm.\n\r", ch );
              return;
           }

        if ( auction_list->seller != ch )
           {
              stc( "Only the seller can cancel the current auction.\n\r", ch );
              return;
           }

        stc( "#wYour auction is now off the queue.#n\n\r", ch );
        sprintf( buf, "#CBids on %s #Chas been cancelled by #y%s#C.", auction_list->object->short_descr,
                      ch->pcdata->switchname );
        do_acinfo( ch, buf );

        free_auction();
        return;
     }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
     {
        send_to_char( "You don't have that item.\n\r", ch );
        return;
     }

  if ( !IS_IMMORTAL( ch ) && ( obj->item_type == ITEM_QUEST || IS_SET( obj->quest, QUEST_RELIC ) ) )
     {
        stc("You cannot auction this item!\n\r",ch);
        return;
     }

  if ( IS_SET( obj->extra_flags, ITEM_AUCTION ) )
     {
        send_to_char( "That item is already on the queue.\n\r", ch );
        return;
     }

  argument = one_argument(argument, arg);

  if ( arg[0] == '\0' )
     {
        send_to_char( "#RSyntax - #wAuction #C<item> #y<Minimum Bid>#n\n\r", ch );
        return;
     }

  if ( !is_number( arg ) )
     {
        stc( "Minimum Bid must be an integer number.\n\r", ch );
        return;
     }
     else min_bid = atoi( arg );

  if ( min_bid < 100 || min_bid > 10000000 )
     {
        stc( "Minimum Bid must be in the range #w100 - 10 mill #ygold#n pieces.\n\r", ch );
        return;
     }

  if ( auction_free == NULL )
     item = alloc_perm( sizeof(*item) );
     else {
             item = auction_free;
             auction_free = auction_free->next;
          }

  item->bidding_time = -1;
  item->buyer = NULL;
  item->seller = ch;
  item->object = obj;
  item->current_bid = --min_bid;
  item->next = NULL;

  if ( !IS_SET( item->object->extra_flags, ITEM_AUCTION ) )
     SET_BIT( item->object->extra_flags, ITEM_AUCTION );

  if ( auction_list == NULL )
     {
        auction_list = item;
        auction_timer = 0;

        sprintf( buf, "#CNow taking bids on %s #Cfor a #wminimum #Cbid of #w%d #ygold #Ccoins by #y%s#C.", item->object->short_descr,
                      min_bid + 1, item->seller->pcdata->switchname );
        do_acinfo( NULL, buf );
     }
     else {
             AUCTION_DATA *auction = auction_list;

             while ( auction->next != NULL )
             {
                auction = auction->next;
             }

             auction->next = item;
             item->bidding_time = -16;
             stc( "#wYour auction is now in the queue.#n\n\r", ch );
          }
}

void free_auction( void )
{
   AUCTION_DATA *item;

   if ( auction_list == NULL )
      return;

   item = auction_list;
   auction_list = auction_list->next;

   if ( IS_SET( item->object->extra_flags, ITEM_AUCTION ) )
      REMOVE_BIT( item->object->extra_flags, ITEM_AUCTION );

   item->buyer = item->seller = NULL;
   item->object = NULL;
   item->current_bid = item->bidding_time = -1;

   if ( auction_free == NULL )
      item->next = NULL;
      else item->next = auction_free;

   auction_free = item;
   auction_timer = 7;
}

void remove_auction( CHAR_DATA *ch )
{
   AUCTION_DATA *item, *prev;

   if ( auction_list == NULL )
      return;

   if ( auction_list->seller == ch )
      {
         free_auction();
         return;
      }

   prev = auction_list;
   item = auction_list->next;

   while ( item != NULL )
   {
      if ( item->seller == ch )
         {
            prev->next = item->next;

            if ( IS_SET( item->object->extra_flags, ITEM_AUCTION ) )
               REMOVE_BIT( item->object->extra_flags, ITEM_AUCTION );

            item->buyer = item->seller = NULL;
            item->object = NULL;
            item->current_bid = item->bidding_time = -1;

            if ( auction_free == NULL )
               item->next = NULL;
               else item->next = auction_free;

            auction_free = item;

            return;
         }

      prev = item;
      item = item->next;
   }
}

void remove_auction_o( OBJ_DATA *obj )
{
   AUCTION_DATA *item, *prev;

   if ( auction_list == NULL )
      return;

   if ( auction_list->object == obj )
      {
         free_auction();
         return;
      }

   prev = auction_list;
   item = auction_list->next;

   while ( item != NULL )
   {
      if ( item->object == obj )
         {
            prev->next = item->next;

            if ( IS_SET( item->object->extra_flags, ITEM_AUCTION ) )
               REMOVE_BIT( item->object->extra_flags, ITEM_AUCTION );

            item->buyer = item->seller = NULL;
            item->object = NULL;
            item->current_bid = item->bidding_time = -1;

            if ( auction_free == NULL )
               item->next = NULL;
               else item->next = auction_free;

            auction_free = item;

            return;
         }

      prev = item;
      item = item->next;
   }
}

void auction_progess( void )
{
   char buf[MAX_INPUT_LENGTH];

   if ( auction_list == NULL || --auction_timer > 0 )
      return;

   if ( auction_list->seller == NULL || auction_list->object == NULL )
      {
         sprintf( buf, "The current auction has been cancelled due to the item being lost." );
         do_acinfo( NULL, buf );
         free_auction();
         return;
      }

   if ( auction_list->bidding_time < 0 )
      {
         if ( auction_list->bidding_time == -16 )
            {
               auction_list->bidding_time = 0;
               sprintf( buf, "#CNow taking bids on %s #Cfor a #wminimum #Cbid of #w%d #ygold #Ccoins by #y%s#C.", auction_list->object->short_descr,
                             auction_list->current_bid + 1, auction_list->seller->pcdata->switchname );
               do_acinfo( NULL, buf );
            }

         if ( --auction_list->bidding_time == -15 )
            {
               sprintf( buf, "#CNo bids on %s #C- item removed.#n", auction_list->object->short_descr );
               do_acinfo( NULL, buf );
               free_auction();
            }

         return;
      }

   switch( --auction_list->bidding_time )
   {
      default: break;
      case 10: sprintf( buf, "#CA #w%d #Cbid on %s #Cgoing once...#n", auction_list->current_bid, auction_list->object->short_descr );
               do_acinfo( NULL, buf );
               break;
      case 5: sprintf( buf, "#CA #w%d #Cbid on %s #Cgoing twice...#n", auction_list->current_bid, auction_list->object->short_descr );
              do_acinfo( NULL, buf );
              break;
      case 0: if ( auction_list->buyer->gold < auction_list->current_bid && auction_list->buyer->bank < auction_list->current_bid )
                 {
                    auction_list->seller->bank = UMIN( 500000000, auction_list->seller->bank + ( auction_list->buyer->bank / 2 ) +
                                                       ( auction_list->buyer->gold / 2 ) );
                    auction_list->buyer->bank /= 2;
                    auction_list->buyer->gold /= 2;
                    sprintf( buf, "#CBids on %s #Cremoved due to cheating from #y%s#C...#n", auction_list->object->short_descr,
                                  auction_list->buyer->pcdata->switchname );
                    do_acinfo( NULL, buf );
                    free_auction();
                    return;
                 }

              sprintf( buf, "#C%s #Csold to #y%s #Cfor #w%d #ygold #Cpieces!#n", auction_list->object->short_descr, auction_list->buyer->pcdata->switchname, auction_list->current_bid );
              do_acinfo( NULL, buf );

              auction_list->seller->bank = UMIN( 500000000, auction_list->seller->bank + auction_list->current_bid );

              if ( auction_list->buyer->bank >= auction_list->current_bid )
                 auction_list->buyer->bank -= auction_list->current_bid;
                 else auction_list->buyer->gold -= auction_list->current_bid;

              act( "#wA small fairy suddenly takes $p #wfrom #Cyou #wbefore disappearing in a #Pshimmering #eportal#w.#n", auction_list->seller, auction_list->object, NULL, TO_CHAR );
              act( "#wA small fairy suddenly takes $p #wfrom #C$n #wbefore disappearing in a #Pshimmering #eportal#w.#n", auction_list->seller, auction_list->object, NULL, TO_ROOM );

              if ( auction_list->object->in_room != NULL )
                 obj_from_room( auction_list->object );
                 else if ( auction_list->object->carried_by != NULL )
                         obj_from_char( auction_list->object );
                 else if ( auction_list->object->in_obj != NULL )
                         obj_from_obj( auction_list->object );

              obj_to_char( auction_list->object, auction_list->buyer );

              if ( auction_list->object->questowner != NULL )
                 free_string( auction_list->object->questowner );

              auction_list->object->questowner = str_dup( auction_list->buyer->pcdata->switchname );

              act( "#wA small fairy suddenly gives #Cyou $p #wbefore disappearing in a #Pshimmering #eportal#w.#n", auction_list->buyer, auction_list->object, NULL, TO_CHAR );
              act( "#wA small fairy suddenly gives #C$n $p #wbefore disappearing in a #Pshimmering #eportal#w.#n", auction_list->buyer, auction_list->object, NULL, TO_ROOM );

              free_auction();
              break;
   }
}
