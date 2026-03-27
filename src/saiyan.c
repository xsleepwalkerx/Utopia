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

void do_saiyancreate( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_SAIYAN) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char( "Syntax: saiyancreate <item>\n\r", ch );
          send_to_char( "items are: plate (1), boots (1), leggings (1), gloves (1), scouter (1)\n\r", ch );
          send_to_char( "           cape (1), headband (1), bracer (1), ring (1), collar (1),\n\r", ch );
          send_to_char( "           belt (1)\n\r", ch );
          return;
       }

    cost = 1;

    if ( !str_cmp(arg,"plate") )
       vnum = 100;
       else if ( !str_cmp(arg,"boots") )
               vnum = 101;
       else if ( !str_cmp(arg,"gloves") )
               vnum = 102;
       else if ( !str_cmp(arg,"scouter") )
               vnum = 103;
       else if ( !str_cmp(arg,"leggings") )
               vnum = 104;
       else if ( !str_cmp(arg,"cape") )
               vnum = 105;
       else if ( !str_cmp(arg,"headband") )
               vnum = 106;
       else if ( !str_cmp(arg,"bracer") )
               vnum = 107;
       else if ( !str_cmp(arg,"ring") )
               vnum = 108;
       else if ( !str_cmp(arg,"collar") )
               vnum = 109;
       else if ( !str_cmp(arg,"belt") )
               vnum = 110;
       else {
               do_saiyancreate(ch,"");
               return;
            }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold#n piece to make this item.\n\r", cost);
	    send_to_char(arg,ch);
          return;
       }

    ch->gold -= cost;

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
       {
	    send_to_char("Missing object, please inform KaVir.\n\r",ch);
	    return;
       }

    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("You silently meditate as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$n silently meditate as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

bool is_eye_scouter( CHAR_DATA *ch )
{
   OBJ_DATA *obj;

   return TRUE;

   if ( ( obj = get_eq_char( ch, WEAR_FACE ) ) == NULL )
      {
         send_to_char( "But you are not wearing your Eye Scouter.\n\r", ch );
         return FALSE;
      }

   if ( obj->pIndexData->vnum != 103 )
      {
         send_to_char( "But you are not wearing your Eye Scouter.\n\r", ch );
         return FALSE;
      }

   return TRUE;
}

void do_power_ball( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   char buf[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_SAIYAN ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("You must do more training before you can execute this technique.\n\r",ch);
         return;
      }

   if ( ch->ctimer[0] > 0 )
      {
         stc("Your device is still recovering from the last use.\n\r",ch);
         return;
      }

   ch->ctimer[0] = 8;

   act("You release a small grin as you look all around you.",ch,NULL,NULL,TO_CHAR);
   act("$n releases a small grin as you look all around you.",ch,NULL,NULL,TO_ROOM);
   act("Reaching down, you pull out a metallic ball as you close your eyes.",ch,NULL,NULL,TO_CHAR);
   act("Reaching down, $n pulls out a metallic ball as $e closes $s eyes.",ch,NULL,NULL,TO_ROOM);
   act("You say softly '#RSo be it... Now see the raw power of a Saiya-Jin...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says softly '#RSo be it... Now see the raw power of a Saiya-Jin...#n'",ch,NULL,NULL,TO_ROOM);
   act("You spread open your hand as the ball floats into the air above you.",ch,NULL,NULL,TO_CHAR);
   act("$n spreads open $s hand as the ball floats into the air above $m.",ch,NULL,NULL,TO_ROOM);
   act("The sky around you darkens as pulses of energy emits from the glowing ball.",ch,NULL,NULL,TO_CHAR);
   act("The sky around $n darkens as pulses of energy emits from the glowing ball.",ch,NULL,NULL,TO_ROOM);

   ch->wait = 4;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( IS_NPC( vch ) )
         continue;

      if ( !IS_CLASS(vch, CLASS_SAIYAN) )
         continue;

      if ( vch->saiyanab[BODY] == 500 )
         continue;

      if ( vch->position <= POS_STUNNED )
         vch->position = POS_STANDING;

      vch->hit = UMIN( 1000000 + vch->hit, vch->max_hit );
      vch->mana = vch->max_mana;
      vch->move = vch->max_move;
      vch->saiyanab[BODY] = 500;

      free_string( vch->morph );
      vch->morph = str_dup( "" );

      if ( IS_SET(vch->affected_by, AFF_POLYMORPH) )
         REMOVE_BIT(vch->affected_by, AFF_POLYMORPH);

      act("\n\rYou start trembling as you lower your head and laugh insanely.",vch,NULL,NULL,TO_CHAR);
      act("\n\r$n starts trembling as $e lowers $s head and laughs insanely.",vch,NULL,NULL,TO_ROOM);
      act("A mass of #obrown#n furry hair flows outwards your body as you grow to a massive size.",vch,NULL,NULL,TO_CHAR);
      act("A mass of #obrown#n furry hair flows outwards $n's body as $e grows to a massive size.",vch,NULL,NULL,TO_ROOM);
      act("Your eyes grow smaller as your nose and mouth turns into a huge snort.",vch,NULL,NULL,TO_CHAR);
      act("$n's eyes grow smaller as $s nose and mouth turns into a huge snort.",vch,NULL,NULL,TO_ROOM);
      act("#wStOmPiNg#n across the room, your body starts trembling as your eyes flashes #ygold#n.",vch,NULL,NULL,TO_CHAR);
      act("#wStOmPiNg#n across the room, $n's body starts trembling as $s eyes flashes #ygold#n.",vch,NULL,NULL,TO_ROOM);

      sprintf(buf, "#o%s the Oozaru#n", vch->name);
      free_string( vch->morph );
      vch->morph = str_dup( buf );

      if ( !IS_SET(vch->affected_by, AFF_POLYMORPH) )
         SET_BIT(vch->affected_by, AFF_POLYMORPH);

      act("#RYou roar at the #Cmoon#R loudly, beating you #yHUGE#R chest with your #wMaSsIvE#R fists.#n",vch,NULL,NULL,TO_CHAR);
      act("#R$n roars at the #Cmoon#R loudly, beating $s #yHUGE#R chest with $s #wMaSsIvE#R fists.#n",vch,NULL,NULL,TO_ROOM);
      act("Your hair grows out #CVERY #pthick#n and long as you #LClEnCh#n your fists in anticipation.",vch,NULL,NULL,TO_CHAR);
      act("$n's hair grows out #CVERY #pthick#n and long as $e #LClEnCheS#n $s fists in anticipation.",vch,NULL,NULL,TO_ROOM);
      act("You #yhAmMeR#n your chest as you release an #REARTH SHATTERING ROAR#n blasting everything away from you.",vch,NULL,NULL,TO_CHAR);
      act("$n #yhAmMeRs#n $s chest as $e releases an #REARTH SHATTERING ROAR#n blasting everything away from $m.",vch,NULL,NULL,TO_ROOM);
      stc("#yYou are now an Oozaru.#n\n\r",vch);
   }

   act("The ball of energy slowly fades out of existence right before your eyes.",ch,NULL,NULL,TO_CHAR);
   act("The ball of energy slowly fades out of existence right before your eyes.",ch,NULL,NULL,TO_ROOM);
}

void do_power_read( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim = NULL;
   char      arg [MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_SAIYAN) ) 
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( is_eye_scouter(ch) == FALSE )
      return; 

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         send_to_char( "Whose powerlevel do you wish to read?\n\r", ch );
         return;
     }

   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\n\r", ch );
         return;
      }

   if ( victim == ch )
      {
         send_to_char( "You already know your powerlevel.\n\r", ch );
         return;
      }

   act("Blinking red lights appear on your Eye Scouter as you focus on $N intentionly.",ch,NULL,victim,TO_CHAR);
   act("Blinking red lights appear on $n's Eye Scouter as $e focus on you intentionly.",ch,NULL,victim,TO_VICT);
   act("Blinking red lights appear on $n's Eye Scouter as $e focus on $N intentionly.",ch,NULL,victim,TO_NOTVICT);

   if ( IS_CLASS(victim,CLASS_SAIYAN) )
      {
         act("#nSon $N:#n",ch,NULL,victim,TO_CHAR);
         sprintf(arg,"#wPower Level: #c%d#n\n\r",victim->saiyan_powerlevel);
         stc(arg,ch);
         if ( victim->saiyan_powerlevel - ch->saiyan_powerlevel >= 1000 )
            stc("Be very careful, this envicee is alot more powerful than you.\n\r",ch);
            else if ( victim->saiyan_powerlevel - ch->saiyan_powerlevel <= -1000 )
                    stc("No need to worry, for your powerlevel dwarfs this envicee.\n\r",ch);
                    else stc("Your fighting capabilities are about the same.\n\r",ch);
      }
      else stc("This envicee has no power level.\n\r",ch);

   ch->wait = 12; 
}

void do_power_search( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *chroom;
   ROOM_INDEX_DATA *victimroom;
   CHAR_DATA *victim;
   bool safe = FALSE;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_SAIYAN) ) 
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( IS_SET(ch->more, MORE_SAFE) )
      safe = TRUE;

   if ( is_eye_scouter(ch) == FALSE )
      return; 

   if ( arg[0] == '\0' )
      {
         send_to_char( "Whose aura you want to find?\n\r", ch );
         return;
      } 

   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They are not here.\n\r", ch );
         return;
      }

   act("Blinking red lights appear on your Eye Scouter as you focus on $N's aura.",ch,NULL,victim,TO_CHAR);
   act("Blinking red lights appear on $n's Eye Scouter as $e focus on something in the distance.",ch,NULL,NULL,TO_ROOM);

   chroom = ch->in_room;
   victimroom = victim->in_room;

   char_from_room(ch);
   char_to_room(ch,victimroom);
   if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim,AFF_SHADOWPLANE) )
      {
         REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
         do_look(ch,"auto");
         SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
      }
      else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim,AFF_SHADOWPLANE) )
      {
         SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
         do_look(ch,"auto");
         REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
      }
      else do_look(ch,"auto");

   char_from_room(ch);
   char_to_room(ch,chroom);

   if ( safe == TRUE )
      SET_BIT(ch->more, MORE_SAFE);

   ch->wait = 12;
}

void do_imbreed( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char lin[MAX_STRING_LENGTH];

    if ( !IS_CLASS(ch, CLASS_SAIYAN) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    sprintf(lin,"===================================================================================\n\r");

    if ( arg[0] == '\0' )
    {
      stc(" Syntax: Imbreed <last name of skill>\n\r",ch);
      stc("==============================|  #wSaiya-Jin  Powers#n  |==============================\n\r",ch);
	stc("                             #wRage of the Lonely Saiyan#n                             \n\r",ch);
	stc(lin,ch);
	sprintf(buf," Saiyan Perception [#y%s#n]  Fluid Movement [#y%s#n]  Lightning Strikes [#y%s#n]\n\r",
	        IS_SET(ch->saiyan_drill, 1) ? "*" : " ",
	        IS_SET(ch->saiyan_drill, 2) ? "*" : " ",
	        IS_SET(ch->saiyan_drill, 4) ? "*" : " ");
	stc(buf, ch);
	sprintf(buf,"       Saiyan Tail [#y%s#n]  Psychotic Rage [#y%s#n]  Blurry Hands [#y%s#n]\n\r",
	        IS_SET(ch->saiyan_drill, 8) ? "*" : " ",
	        IS_SET(ch->saiyan_drill, 16) ? "*" : " ",
	        IS_SET(ch->saiyan_drill, 32) ? "*" : " ");
	stc(buf, ch);
	stc(lin,ch);
	stc("                              #wSaiyanijtsu Power Level#n\n\r",ch);
	stc(lin,ch);
	sprintf(buf,"                  Your Power Level is at #c%d#n.\n\r",ch->saiyan_powerlevel);
      stc(buf,ch);
	stc(lin,ch);
	return;
    }
    
    if ( !str_cmp(arg, "perception" ) )
            {
               if ( IS_SET(ch->saiyan_drill, 1) )
                  {
                      send_to_char("Your senses are already heightened.\n\r",ch);
                      return;
                  }

               SET_BIT(ch->saiyan_drill, 1);
               send_to_char("Your senses are increased to incredible proportions.\n\r",ch);
               save_char_obj(ch);
            }
    else if ( !str_cmp(arg, "movement" ) )
            {
               if ( IS_SET(ch->saiyan_drill, 2) )
                  {
                      send_to_char("Your are already one with the wind.\n\r",ch);
                      return;
                  }

               SET_BIT(ch->saiyan_drill, 2);
               send_to_char("Your are now one with the wind.\n\r",ch);
               save_char_obj(ch);
            }
    else if ( !str_cmp(arg, "strikes" ) )
            {
               if ( IS_SET(ch->saiyan_drill, 4) )
                  {
                      send_to_char("Your hands already strike with the speed of lightning.\n\r",ch);
                      return;
                  }

               SET_BIT(ch->saiyan_drill, 4);
               send_to_char("Your hands now strike with the speed of lightning.\n\r",ch);
               save_char_obj(ch);
            }
    else if ( !str_cmp(arg, "tail" ) )
            {
               if ( IS_SET(ch->saiyan_drill, 8) )
                  {
                      send_to_char("Your tail is already grown out.\n\r",ch);
                      return;
                  }

               SET_BIT(ch->saiyan_drill, 8);
               send_to_char("Your tail grow to its normal length.\n\r",ch);
               save_char_obj(ch);
            }
    else if ( !str_cmp(arg, "rage" ) )
            {
               if ( IS_SET(ch->saiyan_drill, 16) )
                  {
                      send_to_char("Your rage is already enraged.\n\r",ch);
                      return;
                  }

               SET_BIT(ch->saiyan_drill, 16);
               send_to_char("You cackle wildly with psychotic rage!\n\r",ch);
               save_char_obj(ch);
            }
    else if ( !str_cmp(arg, "hands" ) )
            {
               if ( IS_SET(ch->saiyan_drill, 32) )
                  {
                      send_to_char("Your can already parry blows with ease.\n\r",ch);
                      return;
                  }

               SET_BIT(ch->saiyan_drill, 32);
               send_to_char("Your hands now become a blur as you can parry easier.\n\r",ch);
               save_char_obj(ch);
            }
            else do_imbreed(ch,"");
}

void do_spiritbomb( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;
 
   if ( !IS_CLASS(ch, CLASS_SAIYAN))
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 200 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->saiyan_powerlevel < 50000 || ch->saiyanab[BODY] == 500 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   argument = one_argument(argument, arg);

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to use Spirit Bomb on?\n\r", ch );
         return;
      }
    
   if ( arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }

   if ( is_safe( ch, victim ) )
      return;

   act("You say '#yThe Earth, Stars, Universe... lend me your strength#n'.",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yThe Earth, Stars, Universe... lend me your strength#n'.",ch,NULL,NULL,TO_ROOM);
   act("You draw your right hand back as it starts pulsing with energy.",ch,NULL,NULL,TO_CHAR);
   act("$n draws $s right hand back as it starts pulsing with energy.",ch,NULL,NULL,TO_ROOM);
   act("The air around you goes heavy as sparkles of light rises from the ground.",ch,NULL,NULL,TO_CHAR);
   act("The air around $n goes heavy as sparkles of light rises from the ground.",ch,NULL,NULL,TO_ROOM);
   act("The sparkles transcede into your hands as the mass of energy grows bigger.",ch,NULL,NULL,TO_CHAR);
   act("The sparkles transcede into $n's hands as the mass of energy grows bigger.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly you exclaim '#ySpirit Bomb!#n'",ch,NULL,NULL,TO_CHAR);
   act("Suddenly $n exclaims '#ySpirit Bomb!#n'",ch,NULL,NULL,TO_ROOM);
   act("You SmAsH your fist into $N as the MaSs of EnErgY EXPLODES in $S FaCe!!!",ch,NULL,victim,TO_CHAR);
   act("$n SmAsHeS $s fist into you as the MaSs of EnErgY EXPLODES in your FaCe!!!",ch,NULL,victim,TO_VICT);
   act("$n SmAsHeS $s fist into $N as the MaSs of EnErgY EXPLODES in $S FaCe!!!",ch,NULL,victim,TO_NOTVICT);

   ch->wait = 12;
   subtract_stock( ch, 200 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_var = HIT;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 2000000, TRUE );
}

void do_rage_invoke( CHAR_DATA *ch, char *argument )
{
    int count;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int move = ch->move, max = 3;

    if ( IS_NPC(ch) )
       return;
 
    if ( !IS_CLASS( ch, CLASS_SAIYAN ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->saiyanab[BODY] != 500 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[5] > 0 )
       {
          stc("You are still recovering from the last one.\n\r",ch);
          return;
       }

    argument = one_argument(argument, arg);

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to punch in the guts?\n\r", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe(ch,victim) )
       return;

    if ( ch->clannum == 2 )
       max = 4;

    ch->ctimer[5] = 4;

    send_to_char("Your body explodes with mystical energy as you roar with rage.\n\r",ch);
    act("$n's body explodes with mystical energy as $e roars with rage.",ch,NULL,NULL,TO_ROOM);
    act("You ChArGe dIrEcTlY into $N with a SeRiEs of DEVASTATING attacks.",ch,NULL,victim,TO_CHAR);
    act("$n ChArGeS dIrEcTlY into you with a SeRiEs of DEVASTATING attacks.",ch,NULL,victim,TO_VICT);
    act("$n ChArGeS dIrEcTlY into $N with a SeRiEs of DEVASTATING attacks.",ch,NULL,victim,TO_NOTVICT);

    for ( count = 0; count < max; count++ )
    {
       ch->move = UMAX( 30000, ch->max_move );

       switch( number_range( 2, 4 ) )
       {
          case 4: if ( !IS_SET(ch->more3, MORE3_BLAST) )
                     {
                        do_mouthblast( ch, "" );
                        break;
                     }
          case 3: do_scatterpunch( ch, "" );
                  break;
          case 2: do_pounce( ch, "" );
                  break;
          default:
          case 1: do_tailswipe( ch, "" );
                  break;
       }

       ch->move = move;

       if ( ch == NULL || victim == NULL || ch->fighting != victim )
          {
             ch->wait = 12;
             return;
          }
    }

    ch->wait = 12;
}

void do_scatterpunch( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;
 
   if ( !IS_CLASS( ch, CLASS_SAIYAN ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyanab[BODY] != 500 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   argument = one_argument(argument, arg);

   if ( ch->move < 500000 )
      {
         send_to_char( "You are too tired for this.\n\r", ch );
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to punch in the guts?\n\r", ch );
         return;
      }
    
   if ( arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }

   if ( is_safe(ch,victim) )
      return;

   ch->wait = 18;
   ch->move -= 500000;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = HIT;
   ch->attack_focus = LOW;

   act("You suddenly grab $N with your massive hands.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly grabs $N with $s massive hands.",ch,NULL,victim,TO_NOTVICT);
   act("$n suddenly grabs you with $s massive hands.",ch,NULL,victim,TO_VICT);
   act("Grinning evilly, you punch $M VICIOUSLY in the stomach and SLAM $M on the floor.",ch,NULL,victim,TO_CHAR);
   act("Grinning evilly, $n punches you VICIOUSLY in the stomach and SLAMS you on the floor.",ch,NULL,victim,TO_VICT);
   act("Grinning evilly, $n punches $M VICIOUSLY in the stomach and SLAMS $M on the floor.",ch,NULL,victim,TO_NOTVICT);

   hurt_person( ch, victim, 900000, TRUE );
}

void do_pounce( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_SAIYAN ) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->saiyanab[BODY] != 500 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    argument = one_argument(argument, arg);

    if ( ch->move < 500000 )
       {
          send_to_char( "You are too tired for this.\n\r", ch );
          return;
       }

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to jump onto?\n\r", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( is_safe(ch,victim) )
       return;

    ch->wait = 18;
    ch->move -= 500000;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = DIRECT;

    act("You clench your fists as you LeAp into the AiR rOaRiNg LoUdLy!!!",ch,NULL,NULL,TO_CHAR);
    act("$n clenches $s fists as $e LeApS into the AiR rOaRiNg LoUdLy!!!",ch,NULL,NULL,TO_ROOM);
    act("You then FaLl on top of $N as you SeNd $M crashing against the ground!",ch,NULL,victim,TO_CHAR);
    act("$n then FaLlS on top of you as $e SeNds you crashing against the ground!",ch,NULL,victim,TO_VICT);
    act("$n then FaLlS on top of $N as $e SeNds $M crashing against the ground!",ch,NULL,victim,TO_NOTVICT);
    act("You KeEp on jumping on $N's body VICIOUSLY as you FlAtTeN $M up like a PaNcAkE!!!",ch,NULL,victim,TO_CHAR);
    act("$n KeEpS on jumping on your body VICIOUSLY as $e FlAtTeNs you up like a PaNcAkE!!!",ch,NULL,victim,TO_VICT);
    act("$n KeEpS on jumping on $N's body VICIOUSLY as $e FlAtTeNs $M up like a PaNcAkE!!!",ch,NULL,victim,TO_NOTVICT);

    hurt_person( ch, victim, number_range( 900000, 1000000 ), TRUE );
}

void do_tailswipe( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
       return;
 
    if ( !IS_CLASS(ch, CLASS_SAIYAN) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->saiyanab[BODY] != 500 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    argument = one_argument(argument, arg);

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to stun?\n\r", ch );
	    return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
	          return;
             }
       }

    if ( ch->move < 500000 )
       {
          send_to_char( "You are too tired for this.\n\r", ch );
	    return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You suddenly SMASH your tail across $N's body!!!",ch,NULL,victim,TO_CHAR);
    act("$n suddenly SMASHES $s tail across your body!!!",ch,NULL,victim,TO_VICT);
    act("$n suddenly SMASHES $s tail across $N's body!!!",ch,NULL,victim,TO_NOTVICT);

    ch->move -= 500000;
    ch->wait = 18;
    victim->move = UMAX( 0, victim->move - 500000 );
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = HIT;
    ch->attack_focus = LOW;

    hurt_person( ch, victim, 900000, TRUE );
}

void do_mouthblast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int dam = 750000;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_SAIYAN) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->saiyanab[BODY] != 500 )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->ctimer[2] > 0 )
     {
        stc("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( ch->mana < 1500000 )
     {
        send_to_char( "You don't have enough magical energy for this.\n\r", ch );
        return;
     }

  if ( ( victim = ch->fighting ) == NULL )
     {
        stc("You aren't in a fight.\n\r",ch);
        return;
     }

  if ( is_safe(ch, victim) )
     return;

  ch->ctimer[2] = 4;

  act("You suddenly open your mouth releasing a beam of MASSIVE EnErGy!!!",ch,NULL,NULL,TO_CHAR);
  act("$n suddenly opens $s mouth releasing a beam of MASSIVE EnErGy!!!",ch,NULL,NULL,TO_ROOM);
  act("The beam of energy RIPS your body ApARt!!!",victim,NULL,NULL,TO_CHAR);
  act("The beam of energy RIPS $n's body ApARt!!!",victim,NULL,NULL,TO_ROOM);
  ch->mana -= 1500000; 
  victim->mana = UMAX( 0, victim->mana - 1000000 );

  ch->wait = 6;
  ch->attack_type = ATTACK_PHYSICAL;
  ch->ele_attack = ATTACK_THUNDER;
  ch->attack_var = BEAM;
  ch->attack_focus = DIRECT;

  hurt_person( ch, victim, dam,TRUE );
}

void do_bakuhatsuha( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim, *victim_next;
  int dam;

  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_SAIYAN) || ch->clannum != 2 )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->saiyan_powerlevel < 4500 )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->hit < 30000 )
     {
        stc("Your body is too weak to sustain the potential damage.\n\r",ch);
        return;
     }

  if ( ch->move < 100000 )
     {
        stc("You are too tired for this.\n\r",ch);
        return;
     }

  if ( ch->ctimer[1] > 0 )
     {
       stc("You are still recovering from the last one.\n\r",ch);
       return;
     }

  ch->ctimer[1] = 12;
  ch->hit -= 30000;
  ch->move -= 100000;
  ch->wait = 18;

  stc("Unlimited energy draws towards your body as you brace yourself.\n\r",ch); 
  act("Unlimited energy draws towards $n's body as $e braces $mself.\n\r",ch,NULL,NULL,TO_ROOM); 
  stc("You cackle wildly as you viciously raise your right hand in the air.\n\r",ch); 
  act("$n cackles wildly as $e viciously raises $s right hand in the air.",ch,NULL,NULL,TO_ROOM); 
  stc("Your face stretches painfully as the entire area explodes with fericious energy!\n\r",ch); 
  act("$n's face stretches painfully as the entire area explodes with fericious energy!",ch,NULL,NULL,TO_ROOM); 

  for ( victim = ch->in_room->people; victim != NULL; victim = victim_next )
  {
     victim_next = victim->next_in_room;

     if ( victim == ch )
        continue;

     if ( ( is_safe(ch,victim) ) || ( is_same_group(ch,victim) ) )
        continue;

     dam = 100000;

     ch->ele_attack = ATTACK_THUNDER;
     stc("You scream as the explosion engulfs you!\n\r",victim);
     hurt_person(ch,victim,dam,TRUE);
  }
}

void do_supersaiyan( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_INPUT_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   bool test = FALSE;

   if ( IS_NPC(ch) )
      return;

   buf[0] = '\0';
 
   if ( !IS_CLASS(ch, CLASS_SAIYAN) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ch->saiyanab[BODY] > 0 && ch->saiyanab[BODY] < 500 ) 
      {
          free_string( ch->morph );
          ch->morph = str_dup( "" );
          ch->wait = 12;
          REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
          ch->saiyanab[BODY] = 0;  
          act("#yTime stands still as pulses of electricity flows through your body.#n",ch,NULL,NULL,TO_CHAR);
          act("#yTime stands still as pulses of electricity flows through $n's body.#n",ch,NULL,NULL,TO_ROOM);
          act("#yYour #Rmuscles #wsoftens#y as you return to normal.#n",ch,NULL,NULL,TO_CHAR);
          act("#y$n's #Rmuscles #wsoftens#y as $e returns to normal.#n",ch,NULL,NULL,TO_ROOM);
          return;
      }

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);

   act("#yTime stands still as pulses of #Lelectricity#y flows through your body.#n",ch,NULL,NULL,TO_CHAR);
   act("#yTime stands still as pulses of #Lelectricity#y flows through $n's body.#n",ch,NULL,NULL,TO_ROOM);
   act("#yYou start trembling as you lower your head and laugh insanely.#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n starts trembling as $e lowers $s head and laughs insanely.#n",ch,NULL,NULL,TO_ROOM);
   act("#ySuddenly, your blazing eyes turn #Lblue#y as your hair become alit in #Rflames#y, standing out!#n",ch,NULL,NULL,TO_CHAR);
   act("#ySuddenly, $n's blazing eyes turn #Lblue#y as $s hair becomes alit in #Rflames#y, standing out!#n",ch,NULL,NULL,TO_ROOM);
   act("#yYou clench your fists in #Rrage#y as #CMaSsIvE#y jolts of #welectricity#y flashes around you.#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n clenches $s fists in #Rrage#y as #CMaSsIvE#y jolts of #welectricity#y flashes around $m.#n",ch,NULL,NULL,TO_ROOM);
   act("#yYour hair grows #eEXTREMELY #grIgId#y as the destructive electricity surges through your powerful body.#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n's hair grows #eEXTREMELY #grIgId#y as the destructive electricity surges through $s powerful body.#n",ch,NULL,NULL,TO_ROOM);
   act("#yYou suddenly #CSmAsH#y your foot into the ground letting the #Pdazzling #welectricity#y consumes you.#n",ch,NULL,NULL,TO_CHAR);
   act("#y$n suddenly #CSmAsHeS#y $s foot into the ground letting the #Pdazzling #welectricity#y consumes $m.#n",ch,NULL,NULL,TO_ROOM);

   act("#wYou are now a #RSuper#w Saiya-Jin Level 2.#n",ch,NULL,NULL,TO_CHAR);
   sprintf(buf, "#y%s the Super Saiya-Jin#n", ch->name);
   ch->saiyanab[BODY] = 50;

   free_string( ch->morph );
   ch->morph = str_dup( buf );
   SET_BIT(ch->affected_by, AFF_POLYMORPH);
}

void do_kaioken( CHAR_DATA *ch, char *argument )
{
    int sn;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_SAIYAN) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->saiyanab[BODY] == 500 )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ( sn = skill_lookup( "kaioken" ) ) < 0 ) return;

    if ( is_affected(ch,skill_lookup("kaioken")) )
       {
          stc("You are already focusing on your Kaiokence!\n\r",ch);
          return;
       }

    (*skill_table[sn].spell_fun) ( sn, 30, ch, ch );
    WAIT_STATE( ch, 12 );
}

void do_solarflare( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;
    EXIT_DATA *pexit;
    int door;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_SAIYAN) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ch->ctimer[5] > 0 )
       {
          sprintf(buf, "You can't use another taiyouken for another %d ticks.\n\r", ch->ctimer[5] );
          stc(buf,ch);
          return;
       }

    if ( ( victim = ch->fighting ) == NULL )
       {
          argument = one_argument( argument, arg );

          if ( arg[0] == '\0' )
             {
                send_to_char( "Who do you wish to use Taiyouken on?\n\r", ch );
                return;
             }

          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They are not here.\n\r", ch );
                return;
             }

          if ( is_safe( ch, victim ) )
             return;

         act("A mass of energy manifests in your hands.",ch,NULL,NULL,TO_CHAR);
         act("The mass of sheer energy levitates from your hands.",ch,NULL,NULL,TO_CHAR);
         act("You scream '#yTaiyouken!!!#n' as the mass of energy sheers brightly.", ch, NULL, NULL, TO_CHAR );
         act("The sphere of energy spirals towards $N.",ch,NULL,victim,TO_CHAR);

         act("A look of severe concentration fills $n's face.",ch,NULL,victim,TO_VICT);
         act("A mass of searing energy manifests in $s hands.",ch,NULL,victim,TO_VICT);
         act("The sparkling ball of energy levitates from $n's hands.",ch,NULL,victim,TO_VICT);
         act("$n screams '#yTaiyouken!!!#n' as the mass of energy sheers brightly.", ch, NULL, victim, TO_VICT );
         act("The mass of energy spirals towards you.",ch,NULL,victim,TO_VICT);

         act("A look of severe concentration fills $n's face.",ch,NULL,victim,TO_NOTVICT);
         act("A mass of searing energy manifests in $s hands.",ch,NULL,victim,TO_NOTVICT);
         act("The sparkling ball of energy levitates from $n's hands.",ch,NULL,victim,TO_NOTVICT);
         act("$n screams '#yTaiyouken!!!#n' as the mass of energy sheers brightly.", ch, NULL, victim, TO_NOTVICT );
         act("The mass of energy spirals towards $N.",ch,NULL,victim,TO_NOTVICT);

         act("You roar wildly with rage as you clutch your face, staggering from the brightness of the light.",victim,NULL,NULL,TO_CHAR);
         act("$n roars wildly with rage as $e clutches $s face, staggering from the brightness of the light.",victim,NULL,NULL,TO_ROOM);

         victim->wait = number_range( 12, 24 );
         ch->ctimer[5] = 1;

         if ( !IS_SET(victim->more, MORE_BLIND) )
            {
               SET_BIT(victim->more, MORE_BLIND);
               victim->blind_timer = 5;
            }

         return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_BARRIER) )
       {
          send_to_char("As you try to flee, a powerful blast SmAsH through your mind.\n\r",ch);
          act("As $n tries to flee, a powerful blast SmAsHeS through $s mind.",ch,NULL,NULL,TO_ROOM);
          ch->wait = 12;
	    return;
       }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
          send_to_char("You try to move but your foot is impaled into the ground.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SHIFT) )
       {
          send_to_char("It is impossible to flee from within this colourful vortex.\n\r",ch);
          return;
       }

    if ( IS_AFFECTED(ch, AFF_WEBBED) )
       {
          send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
          return;
       }

    if ( IS_SET(ch->more, MORE_NET) )
       {
	    send_to_char( "You are trapped in a spiritual net.\n\r", ch );
	    return;
       }

    if ( IS_SET(ch->more, MORE_IMPALE) )
       {
          send_to_char("You try to move but a spear keeps your foot in place.\n\r",ch);
          return;
       }

    if ( is_memb(ch) )
       return;
  
    was_in = ch->in_room;
   
    send_to_char("You jump high into the sky shouting '#yTaiyouken!!!#n'\n\r", ch );
    act("$n jumps high into the sky shouting '#yTaiyouken!!!#n'", ch, NULL, NULL, TO_ROOM );
    act("The energy around the sun starts bubbling up as mass explosions soars all over it.", ch, NULL, NULL, TO_ROOM );
    act("The energy around the sun starts bubbling up as mass explosions soars all over it.", ch, NULL, NULL, TO_CHAR );
    act("The room suddenly turns #wwhite#n as eXpLoSIve light stuns the vicinity!!!", ch, NULL, NULL, TO_CHAR );
    act("The room suddenly turns #wwhite#n as eXpLoSIve light stuns the vicinity!!!", ch, NULL, NULL, TO_ROOM );

    for ( attempt =  0; attempt < 6; attempt++ ) 
        {
	   door = number_door( );
	   if ( ( pexit = was_in->exit[door] ) == 0
	      ||   pexit->to_room == NULL
	      ||   IS_SET(pexit->exit_info, EX_CLOSED)
	      || ( IS_NPC(ch)
	      &&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
	      continue;

  	   move_char( ch, door );
	   if ( ( now_in = ch->in_room ) == was_in )
	      continue;

	   if ( !IS_NPC(ch) )
	      send_to_char( "You escape from combat!\n\r", ch ); 

           ch->in_room = was_in;
           add_tracks( victim, door );
           ch->in_room = now_in;

           stop_fighting( ch, TRUE );

           ch->ctimer[5] = 1;
           return;
       }
}

void do_distrucodisc( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if ( IS_NPC(ch) ) return;
 
    if ( !IS_CLASS(ch, CLASS_SAIYAN) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    argument = one_argument(argument, arg);

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to blast?\n\r", ch );
          return;
       }
    
    if ( arg[0] != '\0' )
       {
          if ( ( victim = get_char_room( ch, arg ) ) == NULL )
             {
                send_to_char( "They aren't here.\n\r", ch );
                return;
             }
       }

    if ( ch->move < 500000 )
       {
          send_to_char( "You are too tired for this.\n\r", ch );
	    return;
       }

    if ( is_safe(ch,victim) )
       return;

    act("You lean forward as you raise your right palm into the air.",ch,NULL,victim,TO_CHAR);
    act("$n leans forward raising his right palm into the air.",ch,NULL,victim,TO_ROOM);
    act("You yell , '#yKien-zan!!!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n yells, '#yKien-zan!!!#n'",ch,NULL,NULL,TO_ROOM);
    act("You concentrate fiercely as a spiral of energy manifests on top of your palm.",ch,NULL,NULL,TO_CHAR);
    act("$n concentrates fiercely as a spiral of energy manifests on top of $s palm.",ch,NULL,NULL,TO_ROOM);
    act("Your arm trembles with power as the energy spins around rapidly.",ch,NULL,NULL,TO_CHAR);
    act("$n's arm trembles with power as the energy spins around rapidly.",ch,NULL,NULL,TO_ROOM);
    act("Focusing on $N, you fling your palm down throwing the spiral of energy towards $M.",ch,NULL,victim,TO_CHAR);
    act("Focusing on you, $n flings $s palm down throwing the spiral of energy towards you!",ch,NULL,victim,TO_VICT);
    act("Focusing on $N, $n flings $s palm down throwing the spiral of energy towards $M.",ch,NULL,victim,TO_NOTVICT);

    act("There is a ROARING sound as the spiral of energy SlIcEs ThRoUgH tHe AiR!!!",ch,NULL,NULL,TO_CHAR);
    act("There is a ROARING sound as the spiral of energy SlIcEs ThRoUgH tHe AiR!!!",ch,NULL,NULL,TO_ROOM);

    ch->move -= 500000;
    ch->wait = 18;

    ch->attack_type = ATTACK_PHYSICAL;
    ch->ele_attack = ATTACK_THUNDER;
    ch->attack_var = SLASH;
    ch->attack_focus = OVERHEAD;

    if ( number_percent() < 50 || IS_CLASS(victim, CLASS_PALADIN) || IS_CLASS(victim, CLASS_ASSASSIN) )
       {
           act("The spiral of energy RiPs ThrOuGh $n's body!",victim,NULL,NULL,TO_ROOM);
           act("The spiral of energy RiPs ThrOuGh your body!",victim,NULL,NULL,TO_CHAR);
           hurt_person( ch, victim, 800000, TRUE );
           return;
       }

    act("The spiral of energy CuTs sAvAgEly ThrOuGh $n's arm!",victim,NULL,NULL,TO_ROOM);
    act("The spiral of energy CuTs sAvAgEly ThrOuGh your arm!",victim,NULL,NULL,TO_CHAR);

    if ( !IS_ARM_L(victim,LOST_ARM) )
       {
	     SET_BIT(victim->loc_hp[2],LOST_ARM);
	     SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	     make_part(victim,"arm");
	     if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	        {
 	            if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
	               take_item(victim,obj);
	        }
	     if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
	        take_item(victim,obj);
	     if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	        take_item(victim,obj);
	     if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	        take_item(victim,obj);
	     if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	        take_item(victim,obj);

           hurt_person( ch, victim, 900000, TRUE );
       }
 	 else if ( !IS_ARM_R(victim,LOST_ARM) )
               {
                    SET_BIT(victim->loc_hp[3],LOST_ARM);
	 	        SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	              make_part(victim,"arm");
	              if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	                 {
 	                    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		                 take_item(victim,obj);
	                 }
	              if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
	                 take_item(victim,obj);
	              if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	                 take_item(victim,obj);
	              if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
	                 take_item(victim,obj);
	              if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
	                 take_item(victim,obj);

                    hurt_person( ch, victim, 900000, TRUE );
               }
               else hurt_person( ch, victim, 800000, TRUE );
}

void perform_gallet_gun( CHAR_DATA *ch, char *argument )
{
   DESCRIPTOR_DATA *d;
   CHAR_DATA *vch, *vch_next;
   int dam;

   if ( IS_NPC(ch) )
      return;

   SET_BIT( ch->arena, AFF2_DM );

   act("The muscles on your forehead bulges with the anger inside of you as you sneer viciously.",ch,NULL,NULL,TO_CHAR);
   act("The muscles on $n's forehead bulges with the anger inside of $m as $e sneers viciously.",ch,NULL,NULL,TO_ROOM);
   act("You release a #w#zMASSIVE#n beam of energy as it #y#zDRIVES#n through the very ground beneath you!",ch,NULL,NULL,TO_CHAR);
   act("$n releases a #w#zMASSIVE#n beam of energy as it #y#zDRIVES#n through the very ground beneath $m!",ch,NULL,NULL,TO_ROOM);
   sta("The entire area #R#zEXPLODES#n as a RoArInG ATOMIC bOmB #C#zERUPTS#n, DeStrOyInG EVERYTHING in ItS PATH!!!",ch->in_room->area);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( !IS_NPC(vch) || is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      act("You scream loudly as SEARING energy RiPs tHrouGh your BoDY!!!", vch, NULL, NULL, TO_CHAR);
      act("$n screams loudly as SEARING energy RiPs tHrouGh $s BoDY!!!", vch, NULL, NULL, TO_ROOM);
      ch->attack_type = ATTACK_PHYSICAL;
      ch->attack_var = BEAM;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 4500000, TRUE );
   }

   for ( d = descriptor_list; d != NULL; d = d->next )
   {
      if ( d->connected == CON_PLAYING && d->character != NULL && d->character != ch )
	{
          if ( is_safe( ch, d->character ) || is_same_group( ch, d->character ) )
             continue;

          if ( ch->in_room->area != d->character->in_room->area )
             continue;

          act("You scream loudly as SEARING energy RiPs tHrouGh your BoDY!!!", d->character, NULL, NULL, TO_CHAR);
          act("$n screams loudly as SEARING energy RiPs tHrouGh $s BoDY!!!", d->character, NULL, NULL, TO_ROOM);

          d->character->agg = 100;
          ch->attack_type = ATTACK_PHYSICAL;
          ch->attack_var = BEAM;
          ch->attack_focus = AERIAL;
          hurt_person( ch, d->character, 4500000, TRUE );
	}
   }

   REMOVE_BIT( ch->arena, AFF2_DM );
}

void perform_kamehameha( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   char buf[MAX_INPUT_LENGTH];
   int dam;

   if ( IS_NPC(ch) )
      return;

   act("You scream '#yChou Kamehameha!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yChou Kamehameha!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You release a #w#zHUGE#n beam of energy across the vicinity, SMAsHing everyone in its path!",ch,NULL,NULL,TO_CHAR);
   act("$n releases a #w#zHUGE#n beam of energy across the vicinity, SMAsHing everyone in its path!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      dam = 1000000;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_var = BEAM;
      ch->attack_focus = DIRECT;
      stc("You got SLAMMED towards the ground as the beam of energy SEARS through you.\n\r",vch);
      act("$n got SLAMMED towards the ground as the beam of energy SEARS through $m.",vch,NULL,NULL,TO_ROOM);
      hurt_person( ch, vch, dam, TRUE );
   }
}

void do_gallet_gun( CHAR_DATA *ch, char *argument)
{
   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
        stc("Huh?\n\r",ch);
        return;
      }

   act("The muscles on your forehead bulges with the anger inside of you as you sneer viciously.",ch,NULL,NULL,TO_CHAR);
   act("The muscles on $n's forehead bulges with the anger inside of $m as $e sneers viciously.",ch,NULL,NULL,TO_ROOM);
   act("You cackle '#yHeheHeheHeHE! I don't need this STINKING planet!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n cackles '#yHeheHeheHeHE! I don't need this STINKING planet!#n'",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yI will just do a gallet gun and blow the WHOLE thing #R#zUP#n#y!!! HeheHaha HAHA!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yI will just do a gallet gun and blow the WHOLE thing #R#zUP#n#y!!! HeheHaha HAHA!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You zoom high into the sky trembling with rage!",ch,NULL,NULL,TO_CHAR);
   act("$n zooms high into the sky trembling with rage!",ch,NULL,NULL,TO_ROOM);
   act("Tilting your body towards the center of the planet, your body crackles with tremedous power as you charge up!!",ch,NULL,NULL,TO_CHAR);
   act("Tilting $s body towards the center of the planet, $n's body crackles with tremedous power as $e charges up!!",ch,NULL,NULL,TO_ROOM);
   ch->nemesis = 4;
   ch->saiyanstuff = 1;
   subtract_stock( ch, 300 );
}

void do_chou_kamehameha( CHAR_DATA *ch, char *argument)
{
   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 20 )
      {
         stc("You must do more training before you can execute this technique.\n\r",ch);
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
        stc("Huh?\n\r",ch);
        return;
      }

   if ( ch->ctimer[4] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->move < 1000000 )
      {
         stc("You are too tired to execute this tehcnique.\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   act("You snarl '#yStupid... Let me show you the ultimate kamehameha!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n snarls '#yStupid... Let me show you the ultimate kamehameha!#n'",ch,NULL,NULL,TO_ROOM);
   act("You mutter '#yAhhhh...#n' as you clasp your hands together concentrating on your ki.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yAhhhh...#n' as $e clasps $s hands together concentrating on $s ki.",ch,NULL,NULL,TO_ROOM);
   act("A spark of electricity forms between your palms as your feet shuffles into a stance.",ch,NULL,NULL,TO_CHAR);
   act("A spark of electricity forms between $n's palms as $s feet shuffles into a stance.",ch,NULL,NULL,TO_ROOM);
   ch->ctimer[4] = 4;
   ch->nemesis = 1;
   ch->saiyanstuff = 2;
   ch->mana -= 1000000;
   ch->move -= 1000000;
}

void do_big_bang( CHAR_DATA *ch, char *argument)
{
   bool check = FALSE;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->move < 500000 )
      {
         stc("You are too tired to execute this tehcnique.\n\r",ch);
         return;
      }

   argument = one_argument(argument, arg);

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast?\n\r", ch );
         return;
      }
    
   if ( arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }

   if ( is_safe( ch, victim ) )
      return;

   act("You sneer as you raise your hand towards $N.",ch,NULL,victim,TO_CHAR);
   act("$n sneers as $e raises $s hand towards you.",ch,NULL,victim,TO_VICT);
   act("$n sneers as $e raises $s hand towards $N.",ch,NULL,victim,TO_NOTVICT);
   act("Sparks of electricity forms on your hand as you grin evilly around you.",ch,NULL,NULL,TO_CHAR);
   act("Sparks of electricity forms on $n's hand as $e grins evilly around $m.",ch,NULL,NULL,TO_ROOM);

   if ( IS_SET( ch->affected_by, AFF_POLYMORPH ) )
      {
         check = TRUE;
         REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      }

   act("You scream '#ySuper $n... Biiig Bang Atttaacckkk!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#ySuper $n... Biiig Bang Atttaacckkk!!!#n#y!!!#n'",ch,NULL,NULL,TO_ROOM);

   if ( check == TRUE )
      SET_BIT(ch->affected_by, AFF_POLYMORPH);

   act("You spread your hand releasing a powerful beam of energy towards $N!",ch,NULL,victim,TO_CHAR);
   act("$n spreads $s hand releasing a powerful beam of energy towards you!",ch,NULL,victim,TO_VICT);
   act("$n spreads $s hand releasing a powerful beam of energy towards $N!",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_SET( victim->more, MORE_PAIN ) )
      SET_BIT( victim->more, MORE_PAIN );

   ch->move -= 500000;
   ch->wait = 12;

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_var = BEAM;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 650000, TRUE );
}

void do_kamehameha( CHAR_DATA *ch, char *argument)
{
   bool check = FALSE;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->mana < 250000 )
      {
         stc("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   argument = one_argument(argument, arg);

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to blast?\n\r", ch );
         return;
      }
    
   if ( arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }

   if ( is_safe( ch, victim ) )
      return;

   act("You clasp your hands together concentrating on your ki.",ch,NULL,NULL,TO_CHAR);
   act("$n clasps $s hands together concentrating on $s ki.",ch,NULL,NULL,TO_ROOM);
   act("A spark of electricity forms between your palms as your feet shuffles into a stance.",ch,NULL,NULL,TO_CHAR);
   act("A spark of electricity forms between $n's palms as $s feet shuffles into a stance.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yKamehameha!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n screams '#yKamehameha!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You release a #w#zHUGE#n beam of energy across the vicinity, SMAsHing everyone in its path!",ch,NULL,NULL,TO_CHAR);
   act("$n releases a #w#zHUGE#n beam of energy across the vicinity, SMAsHing everyone in its path!",ch,NULL,NULL,TO_ROOM);
   stc("You got SLAMMED towards the ground as the beam of energy SEARS through you.\n\r",victim);
   act("$n got SLAMMED towards the ground as the beam of energy SEARS through $m.",victim,NULL,NULL,TO_ROOM);

   ch->mana -= 250000;
   ch->wait = 12;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_var = BEAM;
   ch->attack_focus = DIRECT;

   hurt_person( ch, victim, 700000, TRUE );
}

void do_kakusan( CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *vch, *vch_next;
   int dam;

   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->mana < 1000000 )
      {
         stc("You don't have enough magical energy to do this.\n\r",ch);
         return;
      }

   ch->mana -= 1000000;
   ch->wait = 18;
   act("A mass of energy manifests in your hands.",ch,NULL,NULL,TO_CHAR);
   act("You scream '#yKakusan!!!#n' as you release a huge energy ball.", ch, NULL, NULL, TO_CHAR );
   act("The energy ball suddenly splits striking up everyone within the vicinity.",ch,NULL,NULL,TO_CHAR);

   act("A look of severe concentration fills $n's face.",ch,NULL,NULL,TO_ROOM);
   act("A mass of searing energy manifests in $s hands.",ch,NULL,NULL,TO_ROOM);
   act("$n screams '#yKakusan!!!#n' as $e releases a huge energy ball.", ch, NULL, NULL, TO_ROOM );
   act("The energy ball suddenly splits up striking everyone within the vicinity.",ch,NULL,NULL,TO_ROOM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
	vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) || is_safe(ch,vch) )
         continue;

      stc("You reel back as you are showered with energy balls.\n\r",vch);
      act("$n reels back as $e is showered with energy balls.",vch,NULL,NULL,TO_ROOM);

      dam = 650000;
      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_var = BEAM;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, dam, TRUE );
   }
}

void do_soukidan( CHAR_DATA *ch, char *argument)
{
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA       *pexit;
   CHAR_DATA       *victim;
   char            arg1      [MAX_INPUT_LENGTH];
   char            arg2      [MAX_INPUT_LENGTH];
   char            revdoor   [MAX_INPUT_LENGTH];
   int             door;
   int             count, dam;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
        stc("Huh?\n\r",ch);
        return;
      }

   if ( ch->move < 10000 )
      {
         stc("You are too tired to execute this technique.\n\r",ch);
         return;
      }

   if ( arg1[0] == '\0' )
      {
	   send_to_char("Please specify a direction to fire your soukidan at.\n\r", ch);
         return;
      }

   if ( arg2[0] == '\0' )
      {
	   send_to_char("Please specify a target to fire this at.\n\r", ch);
         return;
      }

   if      ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) )
	{door = 0;sprintf(arg1,"north");}
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east" ) )
	{door = 1;sprintf(arg1,"east");}
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) )
	{door = 2;sprintf(arg1,"south");}
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west" ) )
	{door = 3;sprintf(arg1,"west");}
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up" ) )
	{door = 4;sprintf(arg1,"up");}
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down" ) )
	{door = 5;sprintf(arg1,"down");}
    else {
	     send_to_char("You can only fire north, south, east, west, up or down.\n\r", ch);
           return;
         }

   if      ( !str_cmp( arg1, "north" ) ) {door = 0;sprintf(revdoor,"south");}
    else if ( !str_cmp( arg1, "east"  ) ) {door = 1;sprintf(revdoor,"west" );}
    else if ( !str_cmp( arg1, "south" ) ) {door = 2;sprintf(revdoor,"north");}
    else if ( !str_cmp( arg1, "west"  ) ) {door = 3;sprintf(revdoor,"east" );}
    else if ( !str_cmp( arg1, "up"    ) ) {door = 4;sprintf(revdoor,"down" );}
    else if ( !str_cmp( arg1, "down"  ) ) {door = 5;sprintf(revdoor,"up"   );}
    else return;

   location = ch->in_room;

   ch->move -= 10000;
   ch->wait = 4;
   act("You raise your finger sharply as a spiral of energy generates in front of you.",ch,NULL,NULL,TO_CHAR);
   act("$n raises $s finger sharply as a spiral of energy generates in front of $m.",ch,NULL,NULL,TO_ROOM);
   act("The spiral of energy flashes as you concentrate viciously, focusing on your target.",ch,NULL,NULL,TO_CHAR);
   act("The spiral of energy flashes as $n concentrates viciously, focusing on $s target.",ch,NULL,NULL,TO_ROOM);
   act("You do a series of swirling motions with your finger as the energy ball zooms out of the room.",ch,NULL,NULL,TO_CHAR);
   act("$n does a series of swirling motions with $s finger as the energy ball zooms out of the room.",ch,NULL,NULL,TO_ROOM);

   for ( count = 0; count < 10; count++ )
   {
      if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
      {
	   act("The wall shakes slightly as it absorbs the energy ball.",ch,NULL,NULL,TO_CHAR);
	   act("The wall shakes slightly as it absorbs the energy ball.",ch,NULL,NULL,TO_ROOM);
 	   return;
      }

      char_from_room(ch);
      char_to_room(ch,to_room);

      if ( ( victim = get_char_room( ch, arg2 ) ) != NULL && !is_safe(ch,victim) )
      {
         act("The energy ball zooms towards the sky as you point your finger upwards.",ch,NULL,NULL,TO_CHAR);
         act("Suddenly, you feel a MaSsIvE explosion as the ball strikes down rapidly onto its target.",ch,NULL,NULL,TO_CHAR);

         act("A energy ball flies straight towards you from seemily nowhere.",ch,NULL,victim,TO_VICT);
         act("At the last second, the ball suddenly twirls upwards over your head.",ch,NULL,victim,TO_VICT);
         act("Then the ball zooms straight down onto you causing a MaSsIvE explosion!!!",ch,NULL,victim,TO_VICT);

         act("A energy ball flies straight towards $N from seemily nowhere.",ch,NULL,victim,TO_NOTVICT);
         act("At the last second, the ball suddenly twirls upwards over $N's head.",ch,NULL,victim,TO_NOTVICT);
         act("Then the ball zooms straight down onto $M causing a MaSsIvE explosion!!!",ch,NULL,victim,TO_NOTVICT);

         ch->ele_attack = ATTACK_THUNDER;
         dam = 8000;

         hurt_person( ch, victim, dam, TRUE );
   
	   char_from_room(ch);
    	   char_to_room(ch,location);

         act("Suddenly, you feel a MaSsIvE explosion as the ball strikes down rapidly onto its target.",ch,NULL,NULL,TO_ROOM);
         return;
      }
 
      act("A massive ball of energy whizzes past you!",ch,NULL,NULL,TO_ROOM);
   }
}

void do_ryuken( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg [MAX_INPUT_LENGTH];
   int dam;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch,CLASS_SAIYAN) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ch->saiyanab[BODY] == 500 )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Who do you wish to smash?\n\r", ch );
         return;
      }

   if ( arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
              send_to_char( "They aren't here.\n\r", ch );
              return;
            }
      }

   if ( is_safe(ch,victim) )
      return;

   ch->wait = 12;

   act("You crouch on your knees as your hands blazes with energy.",ch,NULL,NULL,TO_CHAR);
   act("$n crouches on $s knees as $s hands blazes with energy.",ch,NULL,NULL,TO_ROOM);
   act("Releasing your inner being, you rush towards $N with a fierce look in your eyes!",ch,NULL,victim,TO_CHAR);
   act("Releasing $s inner being, $n rushes towards you with a fierce look in $s eyes!",ch,NULL,victim,TO_VICT);
   act("Releasing $s inner being, $n rushes towards $N with a fierce look in $s eyes!",ch,NULL,victim,TO_NOTVICT);
   act("You yell '#RRyyyuuu...#n' as you SmAsH your glowing fist upwards $N's face!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#RRyyyuuu...#n' as $e SmAsHeS $s glowing fist upwards your face!",ch,NULL,victim,TO_VICT);
   act("$n yells '#RRyyyuuu...#n' as $e SmAsHeS $s glowing fist upwards $N's face!",ch,NULL,victim,TO_NOTVICT);
   act("Your entire body suddenly turns gold as your eyes flashes with an incredible rage.",ch,NULL,NULL,TO_CHAR);
   act("$n's entire body suddenly turns gold as $s eyes flashes with an incredible rage.",ch,NULL,NULL,TO_ROOM);
   act("Your hand transforms into a vision of the eternal dragon as you yell '#RKEN!!!#n'.",ch,NULL,NULL,TO_CHAR);
   act("$n's hand transforms into a vision of the eternal dragon as $e yells '#RKEN!!!#n'.",ch,NULL,NULL,TO_ROOM);
   act("The dragon wraps itself around you as it stares straight up into the sky.",victim,NULL,NULL,TO_CHAR);
   act("The dragon wraps itself around $n as it stares straight up into the sky.",victim,NULL,NULL,TO_ROOM);
   act("Shimmering energy radiates of the dragon as it SqUeEzEs you ViCIouSLy!",victim,NULL,NULL,TO_CHAR);
   act("Shimmering energy radiates of the dragon as it SqUeEzEs $n's ViCIouSLy!",victim,NULL,NULL,TO_ROOM);
   act("A loud scream fills the air as the whole vicinity ERUPTS with inTeNsE energy!",victim,NULL,NULL,TO_CHAR);
   act("A loud scream fills the air as the whole vicinity ERUPTS with inTeNsE energy!",victim,NULL,NULL,TO_ROOM);
   act("You smile proudly as your hand returns back to normal, SmAsHiNg $N off into the air.",ch,NULL,victim,TO_CHAR);
   act("$n smiles proudly as $s hand returns back to normal, SmAsHiNg you off into the air.",ch,NULL,victim,TO_VICT);
   act("$n smiles proudly as $s hand returns back to normal, SmAsHiNg $N off into the air.",ch,NULL,victim,TO_NOTVICT);

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_var = BLAST;
   ch->attack_focus = UPPERCUT;
   hurt_person( ch, victim, 1000000, TRUE );

   act("You land back softly on your feet as you clench your fists viciously.",ch,NULL,NULL,TO_CHAR);
   act("$n lands back softly on $s feet as $e clenches $s fists viciously.",ch,NULL,NULL,TO_ROOM);
}
