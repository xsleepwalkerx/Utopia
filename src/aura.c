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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h>
#endif
#include "merc.h"

void aura_add( CHAR_DATA *ch, int ginpart )
{
   CHAR_DATA *gch;

   if ( ginpart == -1 )
      return;

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
       if ( gch == ch )
          continue;

       if ( is_same_group( gch, ch ) )
	    {
             if ( !IS_SET(gch->group_aura, ginpart) && ch->outer_aura != gch->outer_aura )
                {
                   if ( gch->group_aura == 0 )
                      {
                         stc("The humming increases to a deafening rate as you are surrounded by an aura.\n\r",gch);
                         act("The humming increases to a deafening rate as $n is surrounded by an aura.",gch,NULL,NULL,TO_ROOM);
                      }

                   SET_BIT(gch->group_aura, ginpart);
                   gch->special_aura[ch->outer_aura] = ch->aura[ch->outer_aura] ;
                }
	    }
   }
}

void aura_remove( CHAR_DATA *ch )
{
   CHAR_DATA *gch;
   bool test = FALSE;
   int ginpart = 0;

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
       if ( gch == ch )
          continue;

       if ( is_same_group( gch, ch ) )
	    {
             if ( gch->outer_aura == ch->outer_aura )
                {
                   test = TRUE;
                   break;
                }
	    }
   }

   if ( ch->outer_aura == A_PRAYER )
      ginpart = G_PRAYER;
      else if ( ch->outer_aura == A_MEDITATION )
              ginpart = G_MEDITATION;
      else if ( ch->outer_aura == A_REARTH )
              ginpart = G_REARTH;
      else if ( ch->outer_aura == A_RFIRE )
              ginpart = G_RFIRE;
      else if ( ch->outer_aura == A_RCOLD )
              ginpart = G_RCOLD;
      else if ( ch->outer_aura == A_RTHUNDER )
              ginpart = G_RTHUNDER;
      else if ( ch->outer_aura == A_VIGOR )
              ginpart = G_VIGOR;
      else if ( ch->outer_aura == A_FANATICISM )
              ginpart = G_FANATICISM;
      else if ( ch->outer_aura == A_MIGHT )
              ginpart = G_MIGHT;
      else if ( ch->outer_aura == A_CONCENTRATION )
              ginpart = G_CONCENTRATION;
      else if ( ch->outer_aura == A_HFIRE )
              ginpart = G_HFIRE;
      else if ( ch->outer_aura == A_HFREEZE )
              ginpart = G_HFREEZE;
      else if ( ch->outer_aura == A_HSHOCK )
              ginpart = G_HSHOCK;
      else if ( ch->outer_aura == A_THORNS )
              ginpart = G_THORNS;
      else if ( ch->outer_aura == A_SANCTUARY )
              ginpart = G_SANCTUARY;
      else if ( ch->outer_aura == A_HEALING )
              ginpart = G_HEALING;
      else if ( ch->outer_aura == A_AIM )
              ginpart = G_AIM;
      else if ( ch->outer_aura == A_CLEANSING )
              ginpart = G_CLEANSING;
      else if ( ch->outer_aura == A_STRENGTH )
              ginpart = G_STRENGTH;
      else if ( ch->outer_aura == A_SPEED )
              ginpart = G_SPEED;
      else if ( ch->outer_aura == A_FAITH )
              ginpart = G_FAITH;
      else if ( ch->outer_aura == A_LTAP )
              ginpart = G_LTAP;
      else if ( ch->outer_aura == A_DVISION )
              ginpart = G_DVISION;
      else if ( ch->outer_aura == A_BSHIELD )
              ginpart = G_BSHIELD;
      else if ( ch->outer_aura == A_ADAMAGE )
              ginpart = G_ADAMAGE;
      else if ( ch->outer_aura == A_WEAKEN )
              ginpart = G_WEAKEN;
      else if ( ch->outer_aura == A_IMADIEN )
              ginpart = G_IMADIEN;
      else if ( ch->outer_aura == A_RAGE )
              ginpart = G_RAGE;
      else if ( ch->outer_aura == A_RHOLY )
              ginpart = G_RHOLY;
      else if ( ch->outer_aura == A_RDARK )
              ginpart = G_RDARK;
      else if ( ch->outer_aura == A_CONVICTION )
              ginpart = G_CONVICTION;
              else return;

   if ( test == TRUE && gch != NULL )
      {
         SET_BIT(ch->group_aura, ginpart);
         ch->special_aura[ch->outer_aura] = gch->aura[ch->outer_aura];
         return;
      }

   for ( gch = char_list; gch != NULL; gch = gch->next )
   {
       if ( gch == ch )
          continue;

       if ( is_same_group( gch, ch ) )
	    {
             if ( IS_SET(gch->group_aura, ginpart) )
                {
                   REMOVE_BIT(gch->group_aura, ginpart);

                   if ( gch->group_aura == 0 )
                      {
                          stc("The humming fades as your aura vanish.\n\r",gch);
                          act("The humming fades as $n's aura vanishes.",gch,NULL,NULL,TO_ROOM);
                      }

                   gch->special_aura[ch->outer_aura] = 0 ;
                }
	    }
   }
}

void do_aura( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   int inpart = -1;
   int ginpart = 0;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         stc("                   #c-#P+#c-#CAuras of #yFaith#c-#P+#c-\n\r\n\r", ch );

         if ( ch->aura[0] > 0 )
            stc("  #yPrayer                  ", ch );
            else stc("  #ePrayer                  ", ch );

         if ( ch->aura[1] > 0 )
            stc("#yMeditation              ", ch );
            else stc("#eMeditation              ", ch );

         if ( ch->aura[32] > 0 )
            stc("#yIron Maiden\n\r\n\r", ch );
            else stc("#eIron Maiden\n\r\n\r", ch );

         if ( ch->aura[31] > 0 )
            stc("  #yAmplify Damage          ", ch );
            else stc("  #eAmplify Damage          ", ch );

         if ( ch->aura[33] > 0 )
            stc("#yWeaken                  ", ch );
            else stc("#eWeaken                  ", ch );

         if ( ch->aura[34] > 0 )
            stc("#yRage\n\r\n\r", ch );
            else stc("#eRage\n\r\n\r", ch );

         if ( ch->aura[6] > 0 )
            stc("  #yVigor                   ", ch );
            else stc("  #eVigor                   ", ch );

         if ( ch->aura[7] > 0 )
            stc("#yFanaticism              ", ch );
            else stc("#eFanaticism              ", ch );

         if ( ch->aura[8] > 0 )
            stc("#yMight\n\r\n\r", ch );
            else stc("#eMight\n\r\n\r", ch );

         if ( ch->aura[9] > 0 )
            stc("  #yConcentration           ", ch );
            else stc("  #eConcentration           ", ch );

         if ( ch->aura[10] > 0 )
            stc("#yHoly Fire               ", ch );
            else stc("#eHoly Fire               ", ch );

         if ( ch->aura[11] > 0 )
            stc("#yHoly Freeze\n\r\n\r", ch );
            else stc("#eHoly Freeze\n\r\n\r", ch );

         if ( ch->aura[12] > 0 )
            stc("  #yHoly Shock              ", ch );
            else stc("  #eHoly Shock              ", ch );

         if ( ch->aura[13] > 0 )
            stc("#yThorns                  ", ch );
            else stc("#eThorns                  ", ch );

         if ( ch->aura[14] > 0 )
            stc("#ySanctuary\n\r\n\r", ch );
            else stc("#eSanctuary\n\r\n\r", ch );

         if ( ch->aura[15] > 0 )
            stc("  #yConviction              ", ch );
            else stc("  #eConvction               ", ch );

         if ( ch->aura[20] > 0 )
            stc("#yDivine Healing          ", ch );
            else stc("#eDivine Healing          ", ch );

         if ( ch->aura[22] > 0 )
            stc("#yCleansing\n\r\n\r", ch );
            else stc("#eCleansing\n\r\n\r", ch );

         if ( ch->aura[21] > 0 )
            stc("  #yBlessed Aim             ", ch );
            else stc("  #eBlessed Aim             ", ch );

         if ( ch->aura[23] > 0 )
            stc("#yBlessed Strength        ", ch );
            else stc("#eBlessed Strength        ", ch );

         if ( ch->aura[24] > 0 )
            stc("#yBlessed Speed\n\r\n\r", ch );
            else stc("#eBlessed Speed\n\r\n\r", ch );

         if ( ch->aura[25] > 0 )
            stc("  #yRedemption              ", ch );
            else stc("  #eRedemption              ", ch );

         if ( ch->aura[26] > 0 )
            stc("#yDefiance                ", ch );
            else stc("#eDefiance                ", ch );

         if ( ch->aura[27] > 0 )
            stc("#yFaith\n\r\n\r", ch );
            else stc("#eFaith\n\r\n\r", ch );

         if ( ch->aura[28] > 0 )
            stc("  #yLife Tap                ", ch );
            else stc("  #eLife Tap                ", ch );

         if ( ch->aura[29] > 0 )
            stc("#yDim Vision              ", ch );
            else stc("#eDim Vision              ", ch );

         if ( ch->aura[30] > 0 )
            stc("#yBone Shield\n\r\n\r", ch );
            else stc("#eBone Shield\n\r\n\r", ch );

         if ( IS_CLASS( ch, CLASS_DARAMON ) || ( IS_CLASS( ch, CLASS_PALADIN ) && ch->clannum == 1 ) )
            {
               if ( ch->aura[3] > 0 )
                  stc("  #yResist Fire             ", ch );
                  else stc("  #eResist Fire             ", ch );

               if ( ch->aura[4] > 0 )
                  stc("#yResist Cold             ", ch );
                  else stc("#eResist Cold             ", ch );

               if ( ch->aura[5] > 0 )
                  stc("#yResist Thunder\n\r\n\r", ch );
                  else stc("#eResist Thunder\n\r\n\r", ch );

               if ( ch->aura[2] > 0 )
                  stc("  #yResist Earth            ", ch );
                  else stc("  #eResist Earth            ", ch );

               if ( ch->aura[35] > 0 )
                  stc("#yResist Holy             ", ch );
                  else stc("#eResist Holy             ", ch );

               if ( ch->aura[36] > 0 )
                  stc("#yResist Dark\n\r\n\r", ch );
                  else stc("#eResist Dark\n\r\n\r", ch );
            }

         stc("                   #c-#P+#c-#CAuras of #yFaith#c-#P+#c-\n\r\n\r", ch );

         return;
      }
 
   if ( str_cmp(arg1, "inner") && str_cmp(arg1, "outer") )
      {
          send_to_char("Syntax: #wAura #yInner/#cOuter #p(Aura name)/None#n\n\r",ch);
          return;
      }

   if ( !str_cmp( arg, "prayer" ) )
      {
         inpart = A_PRAYER;
         ginpart = G_PRAYER;
      }
      else if ( !str_cmp( arg, "meditation" ) )
              {
                 inpart = A_MEDITATION;
                 ginpart = G_MEDITATION;
              }
      else if ( !str_cmp( arg, "resist earth" ) )
              {
                 inpart = A_REARTH;
                 ginpart = G_REARTH;
              }
      else if ( !str_cmp( arg, "resist fire" ) )
              {
                 inpart = A_RFIRE;
                 ginpart = G_RFIRE;
              }
      else if ( !str_cmp( arg, "resist cold" ) )
              {
	           inpart = A_RCOLD;
	           ginpart = G_RCOLD;
              }
      else if ( !str_cmp( arg, "resist thunder" ) )
              {
                 inpart = A_RTHUNDER;
                 ginpart = G_RTHUNDER;
              }
      else if ( !str_cmp( arg, "vigor" ) )
              {
                 inpart = A_VIGOR;
                 ginpart = G_VIGOR;
              }
      else if ( !str_cmp( arg, "fanaticism" ) )
              {
                 inpart = A_FANATICISM;
                 ginpart = G_FANATICISM;
              }
      else if ( !str_cmp( arg, "might" ) )
              {
                 inpart = A_MIGHT;
                 ginpart = G_MIGHT;
              }
      else if ( !str_cmp( arg, "concentration" ) )
              {
                 inpart = A_CONCENTRATION;
                 ginpart = G_CONCENTRATION;
              }
      else if ( !str_cmp( arg, "holy fire" ) )
              {
                 inpart = A_HFIRE;
                 ginpart = G_HFIRE;
              }
      else if ( !str_cmp( arg, "holy freeze" ) )
              {
                 inpart = A_HFREEZE;
                 ginpart = G_HFREEZE;
              }
      else if ( !str_cmp( arg, "holy shock" ) )
              {
                 inpart = A_HSHOCK;
                 ginpart = G_HSHOCK;
              }
      else if ( !str_cmp( arg, "thorns" ) )
              {
                 inpart = A_THORNS;
                 ginpart = G_THORNS;
              }
      else if ( !str_cmp( arg, "sanctuary" ) )
              {
                 inpart = A_SANCTUARY;
                 ginpart = G_SANCTUARY;
              }
      else if ( !str_cmp( arg, "conviction" ) )
              {
                 inpart = A_CONVICTION;
                 ginpart = G_CONVICTION;
              }
      else if ( !str_cmp( arg, "divine healing" ) )
              {
                 inpart = A_HEALING;
                 ginpart = G_HEALING;
              }
      else if ( !str_cmp("fromallthegames",arg) )
              do_aura_all( ch, arg );
      else if ( !str_cmp( arg, "cleansing" ) )
              {
                 inpart = A_CLEANSING;
                 ginpart = G_CLEANSING;
              }
      else if ( !str_cmp( arg, "blessed aim" ) )
              {
                 inpart = A_AIM;
                 ginpart = G_AIM;
              }
      else if ( !str_cmp( arg, "blessed strength" ) )
              {
                 inpart = A_STRENGTH;
                 ginpart = G_STRENGTH;
              }
      else if ( !str_cmp( arg, "blessed speed" ) )
              {
                 inpart = A_SPEED;
                 ginpart = G_SPEED;
              }
      else if ( !str_cmp( arg, "redemption" ) )
              {
                 inpart = A_REDEMPTION;
                 ginpart = -1;
              }
      else if ( !str_cmp( arg, "defiance" ) )
              {
                 inpart = A_SALVATION;
                 ginpart = -1;
              }
      else if ( !str_cmp( arg, "faith" ) )
              {
                 inpart = A_FAITH;
                 ginpart = G_FAITH;
              }
      else if ( !str_cmp( arg, "life tap" ) )
              {
                 inpart = A_LTAP;
                 ginpart = G_LTAP;
              }
      else if ( !str_cmp( arg, "bone shield" ) )
              {
                 inpart = A_BSHIELD;
                 ginpart = G_BSHIELD;
              }
      else if ( !str_cmp( arg, "dim vision" ) )
              {
                 inpart = A_DVISION;
                 ginpart = G_DVISION;
              }
      else if ( !str_cmp( arg, "iron maiden" ) )
              {
                 inpart = A_IMADIEN;
                 ginpart = G_IMADIEN;
              }
      else if ( !str_cmp( arg, "amplify damage" ) )
              {
                 inpart = A_ADAMAGE;
                 ginpart = G_ADAMAGE;
              }
      else if ( !str_cmp( arg, "weaken" ) )
              {
                 inpart = A_WEAKEN;
                 ginpart = G_WEAKEN;
              }
      else if ( !str_cmp( arg, "rage" ) )
              {
                 inpart = A_RAGE;
                 ginpart = G_RAGE;
              }
      else if ( !str_cmp( arg, "resist holy" ) )
              {
                 inpart = A_RHOLY;
                 ginpart = G_RHOLY;
              }
      else if ( !str_cmp( arg, "resist dark" ) )
              {
                 inpart = A_RDARK;
                 ginpart = G_RDARK;
              }
      else if ( !str_cmp( arg, "none" ) )
             inpart = -1;
      else {
              send_to_char("Syntax: #wAura #yInner/#cOuter #p(Aura name)/None#n\n\r",ch);
              return;
           }

    if ( inpart != -1 && ch->aura[inpart] < 1 )
       {
            send_to_char("You don't know how to use that aura\n\r",ch);
            return;
       }

    if ( !str_cmp(arg1, "inner") )
       {
          if ( inpart == -1 )
             {
                stc("The humming fades as you lower your inner aura.\n\r",ch);
                act("The humming fades as $n releases $s inner aura.",ch,NULL,NULL,TO_ROOM);
             }
             else {
                     stc("The humming increases to a deafening rate as you adjust your inner aura.\n\r",ch);
                     act("The humming increases to a deafening rate as $n adjusts $s inner aura.",ch,NULL,NULL,TO_ROOM);
                  }

          ch->inner_aura = inpart;
          return;
       }

    if ( !str_cmp(arg1, "outer") )
       {
          if ( inpart == -1 )
             {
                stc("The humming fades as you lower your outer aura.\n\r",ch);
                act("The humming fades as $n releases $s outer aura.",ch,NULL,NULL,TO_ROOM);
                aura_remove(ch);
                ch->outer_aura = inpart;
             }
             else {
                     if ( ch->outer_aura != -1 )
                        aura_remove(ch);

                     stc("The humming increases to a deafening rate as you adjust your outer aura.\n\r",ch);
                     act("The humming increases to a deafening rate as $n adjusts $s outer aura.",ch,NULL,NULL,TO_ROOM);
                     ch->outer_aura = inpart;
                     aura_add(ch, ginpart);
                  }
       }

    if ( !IS_SET( ch->cmbt[5], SKL2_REMINISCE ) )
       ch->wait = 12;
}

void give_random_aura( CHAR_DATA *ch )
{
   int number = -1;
   int tries = 0;

   do
   {
      tries++;

      if ( tries > 49 )
         number = tries - 50;
         else number = number_range( 0, 34 );

      if ( ch->aura[number] > 0 )
         continue;

      if ( ( number > 1 && number < 6 ) || ( number > 15 && number < 20 ) )
         continue;

      break;
   }
   while ( number > -1 && tries < 85 );

   if ( tries > 84 )
      return;

   if ( number == 0 )
      stc( "#wFor your bravery and dedication in battle, you won the #Cprayer #yaura#w.#n\n\r", ch );
      else if ( number == 1 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cmeditation #yaura#w.#n\n\r", ch );
      else if ( number == 6 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cvigor #yaura#w.#n\n\r", ch );
      else if ( number == 7 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cfanaticism #yaura#w.#n\n\r", ch );
      else if ( number == 8 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cmight #yaura#w.#n\n\r", ch );
      else if ( number == 9 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cconcentration #yaura#w.#n\n\r", ch );
      else if ( number == 10 )
              stc( "#wFor your bravery and dedication in battle, you won the #Choly fire #yaura#w.#n\n\r", ch );
      else if ( number == 11 )
              stc( "#wFor your bravery and dedication in battle, you won the #Choly freeze #yaura#w.#n\n\r", ch );
      else if ( number == 12 )
              stc( "#wFor your bravery and dedication in battle, you won the #Choly shock #yaura#w.#n\n\r", ch );
      else if ( number == 13 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cthorns #yaura#w.#n\n\r", ch );
      else if ( number == 14 )
              stc( "#wFor your bravery and dedication in battle, you won the #Csanctuary #yaura#w.#n\n\r", ch );
      else if ( number == 15 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cconviction #yaura#w.#n\n\r", ch );
      else if ( number == 20 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cdivine healing #yaura#w.#n\n\r", ch );
      else if ( number == 21 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cblessed aim #yaura#w.#n\n\r", ch );
      else if ( number == 22 )
              stc( "#wFor your bravery and dedication in battle, you won the #Ccleansing #yaura#w.#n\n\r", ch );
      else if ( number == 23 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cblessed strength #yaura#w.#n\n\r", ch );
      else if ( number == 24 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cblessed speed #yaura#w.#n\n\r", ch );
      else if ( number == 25 )
              stc( "#wFor your bravery and dedication in battle, you won the #Credemption #yaura#w.#n\n\r", ch );
      else if ( number == 26 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cdefiance #yaura#w.#n\n\r", ch );
      else if ( number == 27 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cfaith #yaura#w.#n\n\r", ch );
      else if ( number == 28 )
              stc( "#wFor your bravery and dedication in battle, you won the #Clife tap #yaura#w.#n\n\r", ch );
      else if ( number == 29 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cdim vision #yaura#w.#n\n\r", ch );
      else if ( number == 30 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cbone shield #yaura#w.#n\n\r", ch );
      else if ( number == 31 )
              stc( "#wFor your bravery and dedication in battle, you won the #Camplify damage #yaura#w.#n\n\r", ch );
      else if ( number == 32 )
              stc( "#wFor your bravery and dedication in battle, you won the #Ciron maiden #yaura#w.#n\n\r", ch );
      else if ( number == 33 )
              stc( "#wFor your bravery and dedication in battle, you won the #Cweaken #yaura#w.#n\n\r", ch );
      else if ( number == 34 )
              stc( "#wFor your bravery and dedication in battle, you won the #Crage #yaura#w.#n\n\r", ch );

   ch->aura[number] = 10;
}

