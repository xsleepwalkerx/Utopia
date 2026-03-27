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

void do_monkarmor( CHAR_DATA *ch, char *argument )
{
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;
   char arg[MAX_INPUT_LENGTH];
   int vnum = 0;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_IMMORTAL(ch) )
      {
         if ( !IS_CLASS(ch, CLASS_MONK) )
            {
               send_to_char("What?\n\r",ch);
               return;
            }
      }

   if ( arg[0] == '\0' )
      {
         send_to_char("Please specify which piece of priest armor you wish to make: Ring Pendant Robe Helmet Jeans Sandals Gloves Armbands Cloak Sash Bracer Fan Sword.\n\r",ch);
         return;
      }

   if ( ch->gold < 6 )
      {
         send_to_char("It costs #w6 #ygold pieces#n to create priest equipment.\n\r",ch);
         return;
      }

   if ( !str_cmp(arg,"bracer" ) ) vnum = 306;
      else if ( !str_cmp(arg,"cloak" ) ) vnum = 304;
      else if ( !str_cmp(arg,"gloves" ) ) vnum = 302;
      else if ( !str_cmp(arg,"ring" ) ) vnum = 297;
      else if ( !str_cmp(arg,"pendant" ) ) vnum = 298;
      else if ( !str_cmp(arg,"robe" ) ) vnum = 299;
      else if ( !str_cmp(arg,"helmet" ) ) vnum = 301;
      else if ( !str_cmp(arg,"sandals" ) ) vnum = 308;
      else if ( !str_cmp(arg,"jeans" ) ) vnum = 300;
      else if ( !str_cmp(arg,"armbands" ) ) vnum = 303;
      else if ( !str_cmp(arg,"sash" ) ) vnum = 305;
      else if ( !str_cmp(arg,"fan" ) ) vnum = 309;
      else if ( !str_cmp(arg,"sword" ) ) vnum = 310;
      else {
              do_monkarmor(ch,"");
              return;
           }

   if ( vnum == 0 || ( pObjIndex = get_obj_index( vnum ) ) == NULL )
      {
         send_to_char("Missing object, please inform a God.\n\r",ch);
         return;
      }

   obj = create_object(pObjIndex, 60);
   obj->questowner = str_dup(ch->pcdata->switchname);
   obj_to_char(obj, ch);
   ch->gold -= 6;
   act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
   act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
}

void do_flaminghands( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->monkab[BODY] < 3 )
       {
          stc("You haven't mastered the arts enough to do this technique.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->newbits, NEW_MONKFLAME) )
       {
          REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
          send_to_char("Your hands are no longer engulfed by holy flames.\n\r",ch);
          act("$n's hands are no longer engulfed by holy flames.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }

    if ( !IS_SET(ch->newbits, NEW_MONKFLAME) )
       {
          SET_BIT(ch->newbits, NEW_MONKFLAME);
          send_to_char("Your hands are engulfed by holy flames!\n\r",ch);
          act("$n's hands are engulfed by holy flames!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
}

void do_adamantium( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->monkab[BODY] < 3 )
       {
          send_to_char("You have not learned this ability yet.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->newbits, NEW_MONKADAM) )
       {
           REMOVE_BIT(ch->newbits, NEW_MONKADAM);
           send_to_char("Your hands resume their normal toughness.\n\r",ch);
           act("$n's hands resume their normal toughness.\n\r",ch,NULL,NULL,TO_ROOM);
           return;
       }

    if ( !IS_SET(ch->newbits, NEW_MONKADAM) )
       {
          SET_BIT(ch->newbits, NEW_MONKADAM);
          send_to_char("Your hands turn as hard as adamantium!\n\r",ch);
          act("$n's hands turn as hard as adamantium!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
}

void do_monkcloak( CHAR_DATA *ch, char *argument )
{
   int sn;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 8 )
      {
         stc("You aren't powerful enough to perform the Cloak of Life.\n\r",ch);
         return;
      }

   if ( ch->mana < 1500000 )
      {
         stc("You don't have enough magical energy for this.\n\r", ch);
         return;
      }

   if ( ( sn = skill_lookup( "cloak of life" ) ) < 0 ) return;

   if ( is_affected(ch,skill_lookup("cloak of life") ) )
      {
         stc("You are already blessed with the Cloak of Life!\n\r",ch);
         return;
      }

   (*skill_table[sn].spell_fun) ( sn, 30, ch, ch );
   WAIT_STATE( ch, 12 );
 
   ch->mana -= 1500000;
}

void do_steelskin( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->monkab[BODY] < 4 )
      {
         stc("You need a level 4 of the Body to use the Skin of Steel.\n\r",ch);
         return;
      }

   if ( IS_SET(ch->newbits, NEW_MONKSKIN) )
      {
         REMOVE_BIT(ch->newbits, NEW_MONKSKIN);
         send_to_char("Your skin turns soft as you relax your concentration.\n\r",ch);
         act("$n's skin resume its normal toughness.",ch,NULL,NULL,TO_ROOM);
         return;
      }

   if ( ch->move < 100000 )
      {
         stc("You are too tired to do this technique.\n\r", ch);
         return;
      }

   SET_BIT(ch->newbits, NEW_MONKSKIN);
   send_to_char("You sit down, cross your knees together and start meditating on your surroundings.\n\r",ch);
   act("$n sits down, crosses $s knees together and start meditating on $s surroundings.",ch,NULL,NULL,TO_ROOM);
   send_to_char("You start concentrating furiously as you draw unlimited chi within your body...\n\r",ch);
   act("$n starts concentrating furiously as sparkles of energy emits of $s body...",ch,NULL,NULL,TO_ROOM);
   send_to_char("Your skin hardens into a thin layer of steel as your chi fades away!\n\r",ch);
   act("$n's skin hardens into steel as the sparkles of energy fizzles and dies.",ch,NULL,NULL,TO_ROOM);

   ch->move -= 100000;
}

void do_godsbless( CHAR_DATA *ch, char *argument )
{
   int sn,level;
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_MONK) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->monkab[SPIRIT] < 2 )
      {
         stc("Your spirit is far too weak.\n\r",ch);
         stc("You need to obtain level 2 in Spirit to use GodsBless.\n\r",ch);
         return;
      }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         stc("They are not here.\n\r",ch);
         return;
      }

   if ( ( sn = skill_lookup( "godbless" ) ) < 0 ) return;

   if ( is_affected(victim,skill_lookup("godbless") ) )
      {
         stc("The person already has God's Blessing!\n\r",ch);
         return;
      }

   level = 30;
   (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
   WAIT_STATE( ch, 12 );
}

void do_learn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char lin[MAX_STRING_LENGTH];
    int inpart = 0;

    if ( IS_NPC(ch) ) return;

    if ( IS_CLASS(ch, CLASS_SHADOW) )
    {
      do_assassin_learn( ch, argument );
	return;
    }

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_MONK) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    sprintf(lin,"#b===============================================================================#n\n\r");

    if ( arg[0] == '\0' )
       {
          stc(" #wSyntax: Learn abilities\n\r#n",ch);
          stc(lin,ch);
          stc("                             #wAbilities of Discovery#n                                   \n\r",ch);
          stc(lin,ch);
          sprintf(buf,"          #wChi          [#c%s%s%s%s%s#w]          Body          [#c%s%s%s%s%s#w]#n\n\r",
                         ch->pkill > 3 ? "*" : " ",
                         ch->pkill > 7 ? "*" : " ",
                         ch->pkill > 11 ? "*" : " ",
                         ch->pkill > 15 ? "*" : " ",
                         ch->pkill > 19 ? "*" : " ",
                         ch->pkill > 3 ? "*" : " ",
                         ch->pkill > 7 ? "*" : " ",
                         ch->pkill > 11 ? "*" : " ",
                         ch->pkill > 15 ? "*" : " ",
                         ch->pkill > 19 ? "*" : " " );
          stc(buf, ch);
          sprintf(buf,"          #wAbilities    [#c%d#w]#n\n\r", ch->monkab[BODY] );
          stc(buf, ch);
          stc(lin,ch);
          return;
       }

    if ( !str_cmp(arg, "abilities" ) )
       inpart = BODY;
       else {
               do_learn(ch,"");
               return;
            }

    if ( ch->monkab[inpart] >= 9 )
       {
          stc("You have already learned all you can in this ability.\n\r",ch);
          return;
       }
    
    ch->monkab[inpart]++;
    send_to_char("#wYou have improved your abilities in the martial arts.#n\n\r",ch);
    save_char_obj( ch );
}
