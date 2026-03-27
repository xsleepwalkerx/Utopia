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

void do_samuricreate( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int cost, vnum;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_SAMURAI) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: samuricreate <item>\n\r", ch );
	send_to_char( "items are: kimono (1), sandals (1), turana (1), muramasa (1), ki-jira (1), masamune (1)\n\r", ch );
	return;
    }

    cost = 1;

    if ( !str_cmp(arg,"kimono") )
    {
       vnum = 184;
    }
    else if ( !str_cmp(arg,"sandals") )
            {
               vnum = 185;
            }
    else if ( !str_cmp(arg,"turana") )
            {
               vnum = 186;
            }
    else if ( !str_cmp(arg,"muramasa") )
            {
               vnum = 187;
            }
    else if ( !str_cmp(arg,"ki-jira") )
            {
               vnum = 188;
            }
    else if ( !str_cmp(arg,"masamune") )
            {
               vnum = 189;
            }
    else {
            do_samuricreate(ch,"");
            return;
         }

    if ( cost > ch->gold )
       {
	    sprintf(arg,"It costs you #w%d #ygold#n pieces to make this item.\n\r", cost);
	    send_to_char(arg,ch);
          return;
       }

    ch->gold -= cost;

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
       {
	    send_to_char("Missing object, please inform Joey.\n\r",ch);
	    return;
       }

    obj = create_object(pObjIndex, 70);
    obj_to_char(obj, ch);
    act("You pray for the Emperor's honour as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$n prays for the Emperor's honour as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_seppuko( CHAR_DATA *ch, char *argument )
{
   int chance = 90;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SAMURAI) || IS_SET( ch->special, SPC_SWORD ) )
      {
          send_to_char("Huh?\n\r",ch);
          return;
      }

   if ( ch->pkill < 20 )
      {
         stc("Your mastery of the Katana isn't high enough to do this technique.\n\r",ch);
         return;
      }

   ch->wait = 18;

   if ( ch->hit > 1500000 || ch->fight_timer == 0 )
      {
         send_to_char("You don't feel like throwing away your life atm.\n\r",ch);
         return;
      }

   if ( ( ch->in_room->vnum >= 550 && ch->in_room->vnum < 630 ) || IS_SET(ch->arena, AFF2_INARENA) || ch->pcdata->team != 0 )
      {
         stc("Not in the arena!\n\r",ch);
         return;
      }

   if ( ch->hit < 500000 )
      chance = 0;

   if ( number_percent() < chance )
      {
         stc("You don't have the heart to commit your fate at the current moment.\n\r",ch);
         return;
      }

   act("You say '#yHun kati donira ju...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yHun kati donira ju...#n'",ch,NULL,NULL,TO_ROOM);
   act("You say '#yNias si Empir doden...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says '#yNias si Empir doden...#n'",ch,NULL,NULL,TO_ROOM);
   act("You mutter '#yJun vio ten...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n mutters '#yJun vio ten...#n'",ch,NULL,NULL,TO_ROOM);

   act("\n\rRaising your katana high into the air, you suddenly ram it through your heart!",ch,NULL,NULL,TO_CHAR);
   act("\n\rRaising $s katana high into the air, $n suddenly rams it through $s heart!",ch,NULL,NULL,TO_ROOM);
   act("As you fall lifeless to the ground, your stiffened hands cut yourself in half with your katana...",ch,NULL,NULL,TO_CHAR);
   act("As $n falls lifeless to the ground, $s stiffened hands cut $mself in half with $s katana...",ch,NULL,NULL,TO_ROOM);
   act("$n is DEAD!!!",ch,NULL,NULL,TO_ROOM);

   stop_fighting( ch, TRUE );

   if ( ch->attacker != NULL )
      {
         restore_char( ch->attacker );
         ch->attacker->fight_timer = 0;
      }

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

void do_turana( CHAR_DATA *ch, OBJ_DATA *obj )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam;

  act("\n\rYou concentrate on Turana as a manifest of energy emits off it.",ch,NULL,NULL,TO_CHAR);
  act("\n\r$n concentrates on Turana as a manifest of energy emits off it.",ch,NULL,NULL,TO_ROOM);

  act("The sky is filled with lightning as crackling electricity flashes from the legendary katana.",ch,NULL,NULL,TO_ROOM);
  act("The sky is filled with lightning as crackling electricity flashes from the legendary katana.",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch,vch) )
        continue;

     act("$n's got stunned by a huge bolt of thunder!",vch,NULL,NULL,TO_ROOM);
     act("You got stunned by a huge bolt of thunder!",vch,NULL,NULL,TO_CHAR);

     if ( number_percent() > 84 - devil_luck( ch, vch, 5 ) && !is_martial( vch ) )
        vch->wait += 12;

     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_THUNDER;
     ch->attack_var = BLUNT;
     ch->attack_focus = AERIAL;
     hurt_person( ch, vch, 1000000, TRUE );
  }
}

void do_muramasa( CHAR_DATA *ch, OBJ_DATA *obj )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  act("\n\rYou concentrate on Muramasa as a manifest of energy emits off it.",ch,NULL,NULL,TO_CHAR);
  act("\n\r$n concentrates on Muramasa as a manifest of energy emits off it.",ch,NULL,NULL,TO_ROOM);

  act("Holy energy radiates around the room as it is drawn towards it target.",ch,NULL,NULL,TO_ROOM);
  act("Holy energy radiates around the room as it is drawn towards it target.",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !can_heal( ch, vch ) )
        continue;

     act("$n looks better as $e is bathe within the holy energy.",vch,NULL,NULL,TO_ROOM);
     act("You look better as you are bathe within the holy energy.",vch,NULL,NULL,TO_CHAR);
     hit_gain( vch, 1000000 );
  }
}

void do_smasamune( CHAR_DATA *ch, OBJ_DATA *obj )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  act("\n\rYou concentrate on Masamune as a manifest of energy emits off it.",ch,NULL,NULL,TO_CHAR);
  act("\n\r$n concentrates on Masamune as a manifest of energy emits off it.",ch,NULL,NULL,TO_ROOM);

  act("The Room goes dark as haunting music flows through the air.",ch,NULL,NULL,TO_CHAR);
  act("The Room goes dark as haunting music flows through the air.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, there is a loud cackle as millions of ghosts float upwards toward the heavens.",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, there is a loud cackle as millions of ghosts float upwards toward the heavens.",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch,vch) )
        continue;

     act("$n looks pale as a ghost touches $s body.",vch,NULL,NULL,TO_ROOM);
     act("You suddenly feel very pale as a ghost touches your body, draining your life energy from it.",vch,NULL,NULL,TO_CHAR);

     if ( !IS_SET(vch->more, MORE_PAIN) )
        SET_BIT(vch->more, MORE_PAIN);

     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_DARK;
     ch->attack_var = SOUL;
     ch->attack_focus = UPPERCUT;
     hurt_person( ch, vch, 2500000, TRUE );
  }
}

void do_kijira( CHAR_DATA *ch, OBJ_DATA *obj )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam;

  act("\n\rYou concentrate on Ki-jira as a manifest of energy emits off it.",ch,NULL,NULL,TO_CHAR);
  act("\n\r$n concentrates on Ki-jira as a manifest of energy emits off it.",ch,NULL,NULL,TO_ROOM);

  act("Explosive energy flashes through the room filling it with blinding light.",ch,NULL,NULL,TO_CHAR);
  act("Explosive energy flashes through the room filling it with blinding light.",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch,vch) )
        continue;

     act("$n screams loudly as multiple beams slashes through $s body!",vch,NULL,NULL,TO_ROOM);
     act("You scream loudly as multiple beams slashes through your body!",vch,NULL,NULL,TO_CHAR);

     if ( number_percent() > 49 && !IS_SET( vch->more, MORE_BLIND ) )
        {
           SET_BIT( vch->more, MORE_BLIND );
           vch->blind_timer = 3;
        }

     ch->attack_type = ATTACK_MAGICAL;
     ch->ele_attack = ATTACK_HOLY;
     ch->attack_var = BEAM;
     ch->attack_focus = OVERHEAD;
     hurt_person( ch, vch, 1000000, TRUE );
  }
}

void do_sdraw( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if ( IS_NPC(ch) ) return;

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_SAMURAI) || IS_SET( ch->special, SPC_SWORD ) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 10 )
       {
          stc("Your mastery of the Katana isn't high enough to do this technique.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
          send_to_char("Draw out which katana blade?\n\r",ch);
          return;
       }

    if (!str_cmp(arg,"turana"))
       {
          if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
             if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                {
                   send_to_char( "You are not holding a sword.\n\r", ch );
                   return;
                }

          if ( ch->ctimer[0] > 0 )
             {          
                stc("You are still recovering from the last one.\n\r",ch);
                return;
             }

          if ( ch->mana < 5000000 )
             {          
                stc("You don't have enough magical energy for that.\n\r",ch);
                return;
             }

          ch->mana -= 5000000;
          ch->wait = 12;
          ch->ctimer[0] = 6;
          do_turana(ch,obj); 
       }
       else if (!str_cmp(arg,"muramasa"))
               {
                  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                        if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                        {
                           send_to_char( "You are not holding a sword.\n\r", ch );
                           return;
                        }

                  if ( ch->ctimer[0] > 0 )
                     {          
                        stc("You are still recovering from the last one.\n\r",ch);
                        return;
                     }

                  if ( ch->mana < 5000000 )
                     {          
                        stc("You don't have enough magical energy for that.\n\r",ch);
                        return;
                     }

                  ch->mana -= 5000000;
                  ch->ctimer[0] = 6;
                  ch->wait = 6;
                  do_muramasa(ch,obj); 
               }
       else if (!str_cmp(arg,"ki-jira"))
               {
                  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                        if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                        {
                           send_to_char( "You are not holding a sword.\n\r", ch );
                           return;
                        }

                  if ( ch->ctimer[0] > 0 )
                     {          
                        stc("You are still recovering from the last one.\n\r",ch);
                        return;
                     }

                  if ( ch->mana < 5000000 )
                     {          
                        stc("You don't have enough magical energy for that.\n\r",ch);
                        return;
                     }

                  ch->mana -= 5000000;
                  ch->wait = 12;
                  ch->ctimer[0] = 6;
                  do_kijira(ch,obj); 
               }
       else if (!str_cmp(arg,"masamune"))
               {
                  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                     if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                        if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
                        {
                           send_to_char( "You are not holding a sword.\n\r", ch );
                           return;
                        }

                  if ( get_stock( ch ) < 200 )
                     {
                        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
                        return;
                     }

                  ch->wait = 12;
                  SET_BIT( ch->arena, AFF2_DM );
                  subtract_stock( ch, 200 );
                  do_smasamune(ch,obj); 
               }
               else {
                       ch->wait = 0;
                       stc("That katana blade doesn't exits.\n\r",ch);
                       return;
                    }
}

void do_knowledge( CHAR_DATA *ch, char *argument )
{
    char      buf [MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_SAMURAI) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    stc("#w/-----------------------Samurai Abilities-----------------------|\n\r",ch);
    stc("#w|   #gTrueSight                 -     #y(Known)#w                     |\n\r",ch);
    stc("#w|   #gSeppuku                   -     #y(Known)#w                     |\n\r",ch);
    stc("#w|   #gSdraw                     -     #y(Known)#w                     |\n\r",ch);
    stc("#w\\--------------------------Death Blows--------------------------/\n\r",ch);

    sprintf( buf, "#w|   #gAmaoto         #r4 LS,HS                               #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gEngetsu        #r5 LS,LS,HS                            #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gAmaguma        #r5 MS,HS                               #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gHimatsu        #r6 LS,LS,LS,HS                         #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gYako           #r6 LS,MS,HS                            #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gZanretsu       #r6 MS,LS,HS                            #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gMyogetsu       #r6 HS,HS                               #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gFestive Wind     #rLS,LS,LS,LS,HS                      #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gEarth Dance      #rLS,LS,MS,HS                         #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gHaze of Fire     #rLS,MS,LS,HS                         #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #gCrystal Water    #rMS,LS,LS,HS                         #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    sprintf( buf, "#w|   #CNikitsu          #rMS,MS,LS,HS,MS                      #y[%3d%%]#w  |\n\r",100);
    stc(buf,ch);
    stc("#w\\----------------------------#bInfo#w-------------------------------/\n\r",ch);
    stc("#w|   #pTo learn an ability, Type Study (Ability)#w                   |\n\r",ch);
    stc("#w|   #pTo learn a Death Blow, keep on typing what the list said#w    |\n\r",ch);
    stc("#w\\---------------------------------------------------------------/\n\r",ch);
}

void do_amaoto( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You leap towards $N somersaulting your body.",ch,NULL,victim,TO_CHAR);
   act("$n leaps towards you somersaulting $s body.",ch,NULL,victim,TO_VICT);
   act("$n leaps towards $N somersaulting $s body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image leaps towards $N somersaulting its body.",ch,NULL,victim,TO_CHAR);
   act("$n's image leaps towards you somersaulting its body.",ch,NULL,victim,TO_VICT);
   act("$n's image leaps towards $N somersaulting its body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image leaps towards $N somersaulting its body.",ch,NULL,victim,TO_CHAR);
   act("$n's image leaps towards you somersaulting its body.",ch,NULL,victim,TO_VICT);
   act("$n's image leaps towards $N somersaulting its body.",ch,NULL,victim,TO_NOTVICT);

   act("You slice downwards $N's body as you twirl yourself around.",ch,NULL,victim,TO_CHAR);
   act("$n slices downwards your body as $e twirls $mself around.",ch,NULL,victim,TO_VICT);
   act("$n slices downwards $N's body as $e twirls $mself around.",ch,NULL,victim,TO_NOTVICT);
   act("Your image slices downwards $N's body as it twirls itself around.",ch,NULL,victim,TO_CHAR);
   act("$n's image slices downwards your body as it twirls itself around.",ch,NULL,victim,TO_VICT);
   act("$n's image slices downwards $N's body as it twirls itself around.",ch,NULL,victim,TO_NOTVICT);

   act("As you land on your feet, you slice your katana across $N's feet, tripping $M.",ch,NULL,victim,TO_CHAR);
   act("As $n lands on $s feet, $e slices $s katana across your feet, tripping you.",ch,NULL,victim,TO_VICT);
   act("As $n lands on $s feet, $e slices $s katana across $N's feet, tripping $M.",ch,NULL,victim,TO_NOTVICT);
   act("As your image lands on its feet, it slices its katana across $N's feet, tripping $M.",ch,NULL,victim,TO_CHAR);
   act("As $n's image lands on its feet, it slices its katana across your feet, tripping you.",ch,NULL,victim,TO_VICT);
   act("As $n's image lands on its feet, it slices its katana across $N's feet, tripping $M.",ch,NULL,victim,TO_NOTVICT);

   if ( victim != NULL && number_percent() > 94 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
      victim->wait = 12;

   SET_BIT( ch->arena, AFF2_COMBO );
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = LOW;
   hurt_person(ch, victim, 700000, TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_engetsu( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You rush towards $N thrusting your katana through $M.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you thrusting $s katana through you.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N thrusting $s katana through $M.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rush towards $N thrusting its katana through $M.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you thrusting its katana through you.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N thrusting its katana through $M.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rush towards $N thrusting its katana through $M.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you thrusting its katana through you.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N thrusting its katana through $M.",ch,NULL,victim,TO_NOTVICT);

   act("You then turn around slicing $M roughly across the body.",ch,NULL,victim,TO_CHAR);
   act("$n then turn around slicing you roughly across the body.",ch,NULL,victim,TO_VICT);
   act("$n then turn around slicing $M roughly across the body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image turn around slicing $M roughly across the body.",ch,NULL,victim,TO_CHAR);
   act("$n's image then turn around slicing you roughly across the body.",ch,NULL,victim,TO_VICT);
   act("$n's image then turn around slicing $M roughly across the body.",ch,NULL,victim,TO_NOTVICT);

   act("You raise your katana above you as sparkling energy converges to the tip.",ch,NULL,victim,TO_CHAR);
   act("$n raises $s katana above $m as sparkling energy converges to the tip.",ch,NULL,victim,TO_VICT);
   act("$n raises $s katana above $m as sparkling energy converges to the tip.",ch,NULL,victim,TO_NOTVICT);

   act("You then BASH $M down with a savage attack to the head.",ch,NULL,victim,TO_CHAR);
   act("$n then BASHES you down with a savage attack to the head.",ch,NULL,victim,TO_VICT);
   act("$n then BASHES $M down with a savage attack to the head.",ch,NULL,victim,TO_NOTVICT);
   act("Your image then BASH $M down with a savage attack to the head.#n",ch,NULL,victim,TO_CHAR);
   act("$n's image then BASHES you down with a savage attack to the head.",ch,NULL,victim,TO_VICT);
   act("$n's image then BASHES $M down with a savage attack to the head.",ch,NULL,victim,TO_NOTVICT);
   act("Your image then BASH $M down with a savage attack to the head.",ch,NULL,victim,TO_CHAR);
   act("$n's image then BASHES you down with a savage attack to the head.",ch,NULL,victim,TO_VICT);
   act("$n's image then BASHES $M down with a savage attack to the head.",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLUNT;
   ch->attack_focus = OVERHEAD;

   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person(ch, victim, 700000, TRUE);

   if ( victim != NULL && number_percent() > 94 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
      victim->stunned = 1;

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_amaguma( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You rush towards $N thrusting your katana through $M.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you thrusting $s katana through you.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N thrusting $s katana through $M.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rush towards $N thrusting its katana through $M.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you thrusting its katana through you.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N thrusting its katana through $M.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rush towards $N thrusting its katana through $M.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you thrusting its katana through you.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N thrusting its katana through $M.",ch,NULL,victim,TO_NOTVICT);

   act("You then raise your katana high above you impaling $N.",ch,NULL,victim,TO_CHAR);
   act("$n then raises $s katana high above $m impaling you.",ch,NULL,victim,TO_VICT);
   act("$n then raises $s katana high above $m impaling $N.",ch,NULL,victim,TO_NOTVICT);

   act("As $N slides down on the katana, you grab a shimmering one and rip it through $M.",ch,NULL,victim,TO_CHAR);
   act("As you slide down on the katana, $n grabs a shimmering one and rips it through you.",ch,NULL,victim,TO_VICT);
   act("As $N slides down on the katana, $n grabs a shimmering one and rips it through $M.",ch,NULL,victim,TO_NOTVICT);
   act("As $N slides down on the katana, your image grab a shimmering one and rips it through $M.",ch,NULL,victim,TO_CHAR);
   act("As you slide down on the katana, $n's image grabs a shimmering one and rips it through you.",ch,NULL,victim,TO_VICT);
   act("As $N slides down on the katana, $n's image grabs a shimmering one and rips it through $M.",ch,NULL,victim,TO_NOTVICT);
   act("As $N slides down on the katana, your image grab a shimmering one and rips it through $M.",ch,NULL,victim,TO_CHAR);
   act("As you slide down on the katana, $n's image grabs a shimmering one and rips it through you.",ch,NULL,victim,TO_VICT);
   act("As $N slides down on the katana, $n's image grabs a shimmering one and rips it through $M.",ch,NULL,victim,TO_NOTVICT);

   act("You then draw your katana out of $N's body as you swipe it across $S face.",ch,NULL,victim,TO_CHAR);
   act("$n then draws $s katana out of your body as $e swipes it across $S face.",ch,NULL,victim,TO_VICT);
   act("$n then draws $s katana out of $N's body as $e swipes it across $S face.",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 49 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->more, MORE_PINCH ) )
      SET_BIT( victim->more, MORE_PINCH );

   ch->agg = UMAX( 0, ch->agg - 15 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = UPPERCUT;

   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person(ch, victim, 700000, TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_himatsu( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You jump towards $N slicing your katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n jumps towards you slicing $s katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n jumps towards $N slicing $s katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image jumps towards $N slicing its katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n's image jumps towards you slicing its katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n's image jumps towards $N slicing its katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image jumps towards $N slicing its katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n's image jumps towards you slicing its katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n's image jumps towards $N slicing its katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);

   act("As $N reels back from the blow, you rapidly spin your body drilling your katana into $M.",ch,NULL,victim,TO_CHAR);
   act("As you reels back from the blow, $n rapidly spins $s body drilling $s katana into you.",ch,NULL,victim,TO_VICT);
   act("As $N reels back from the blow, $n rapidly spins $s body drilling $s katana into $M.",ch,NULL,victim,TO_NOTVICT);
   act("As $N reels back from the blow, you rapidly spin your body drilling your katana into $M.",ch,NULL,victim,TO_CHAR);
   act("As you reels back from the blow, $n rapidly spins $s body drilling $s katana into you.",ch,NULL,victim,TO_VICT);
   act("As $N reels back from the blow, $n rapidly spins $s body drilling $s katana into $M.",ch,NULL,victim,TO_NOTVICT);
   act("As $N reels back from the blow, you rapidly spin your body drilling your katana into $M.",ch,NULL,victim,TO_CHAR);
   act("As you reels back from the blow, $n rapidly spins $s body drilling $s katana into you.",ch,NULL,victim,TO_VICT);
   act("As $N reels back from the blow, $n rapidly spins $s body drilling $s katana into $M.",ch,NULL,victim,TO_NOTVICT);

   act("You suddenly tilt your body backwards stunning $N with a backflip kick.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly tilts $s body backwards stunning you with a backflip kick.",ch,NULL,victim,TO_VICT);
   act("$n suddenly tilts $s body backwards stunning $N with a backflip kick.",ch,NULL,victim,TO_NOTVICT);
 
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = AERIAL;

   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person(ch, victim, 700000, TRUE);

   if ( victim != NULL && number_percent() > 94 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
      victim->stunned = 1;

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_yako( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You roll yourself into a ball as you rapidly spin towards $N.",ch,NULL,victim,TO_CHAR);
   act("$n rolls $mself into a ball as $e rapidly spins towards you.",ch,NULL,victim,TO_VICT);
   act("$n rolls $mself into a ball as $e rapidly spins towards $N.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rolls itself into a ball as it rapidly spins towards $N.",ch,NULL,victim,TO_CHAR);
   act("$n's image rolls itself into a ball as it rapidly spins towards you.",ch,NULL,victim,TO_VICT);
   act("$n's image rolls itself into a ball as it rapidly spins towards $N.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rolls itself into a ball as it rapidly spins towards $N.",ch,NULL,victim,TO_CHAR);
   act("$n's image rolls itself into a ball as it rapidly spins towards you.",ch,NULL,victim,TO_VICT);
   act("$n's image rolls itself into a ball as it rapidly spins towards $N.",ch,NULL,victim,TO_NOTVICT);

   act("$N screams loudly as you slice your katana downwards $S innwards.",ch,NULL,victim,TO_CHAR);
   act("You scream loudly as $n slices $s katana downwards your innwards.",ch,NULL,victim,TO_VICT);
   act("$N screams loudly as $n slices $s katana downwards $S innwards.",ch,NULL,victim,TO_NOTVICT);
   act("$N screams loudly as your image slice its katana downwards $S innwards.",ch,NULL,victim,TO_CHAR);
   act("You scream loudly as $n's image slices its katana downwards your innwards.",ch,NULL,victim,TO_VICT);
   act("$N screams loudly as $n's image slices its katana downwards $S innwards.",ch,NULL,victim,TO_NOTVICT);
   act("$N screams loudly as your image slice its katana downwards $S innwards.",ch,NULL,victim,TO_CHAR);
   act("You scream loudly as $n's image slices its katana downwards your innwards.",ch,NULL,victim,TO_VICT);
   act("$N screams loudly as $n's image slices its katana downwards $S innwards.",ch,NULL,victim,TO_NOTVICT);

   act("Suddenly, you land on your feet as you raise your katana into the air.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, $n lands on $s feet as $e raises $s katana into the air.",ch,NULL,NULL,TO_ROOM);

   act("Shimmering energy emits of your katana as you DRIVE it through $N's body.",ch,NULL,victim,TO_CHAR);
   act("Shimmering energy emits of $n's katana as $e DRIVES it through your body.",ch,NULL,victim,TO_VICT);
   act("Shimmering energy emits of $n's katana as $e DRIVES it through $N's body.",ch,NULL,victim,TO_NOTVICT);

   victim->move = UMAX( 0, victim->move - 250000 );
 
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;
   SET_BIT( ch->arena, AFF2_COMBO );
   hurt_person(ch, victim, 700000, TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_zanretsu( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You wave your katana in a half circle as you close your eyes.",ch,NULL,NULL,TO_CHAR);
   act("$n waves $s katana in a half circle as $e closes $s eyes.",ch,NULL,NULL,TO_ROOM);
   act("Your image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_CHAR);
   act("$n's image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_ROOM);
   act("Your image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_CHAR);
   act("$n's image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_ROOM);
   act("Your image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_CHAR);
   act("$n's image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_ROOM);
   act("Your image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_CHAR);
   act("$n's image waves its katana in a half circle as it closes its eyes.",ch,NULL,NULL,TO_ROOM);

   act("Suddenly you scream loudly as you slice your katana across $N's waist!",ch,NULL,victim,TO_CHAR);
   act("Suddenly $n screams loudly as $e slices $s katana across your waist!",ch,NULL,victim,TO_VICT);
   act("Suddenly $n screams loudly as $e slices $s katana across $N's waist!",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly your image screams loudly as it slices its katana across $N's waist!",ch,NULL,victim,TO_CHAR);
   act("Suddenly $n's image screams loudly as it slices its katana across your waist!",ch,NULL,victim,TO_VICT);
   act("Suddenly $n's image screams loudly as it slices its katana across $N's waist!",ch,NULL,victim,TO_NOTVICT);

   act("As $N falls to the ground, you RAM your katana up $S face!",ch,NULL,victim,TO_CHAR);
   act("As you fall to the ground, $n RAMS $s katana up your face!",ch,NULL,victim,TO_VICT);
   act("As $N falls to the ground, $n RAMS $s katana up $S face!",ch,NULL,victim,TO_NOTVICT);
 
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = UPPERCUT;

   SET_BIT( ch->arena, AFF2_COMBO );

   if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) )
      hurt_person(ch, victim, 900000, TRUE);
      else hurt_person(ch, victim, 700000, TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_myogetsu( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You raise your katana behind your back as manifests of energy soars from it.",ch,NULL,NULL,TO_CHAR);
   act("$n raises $s katana behind $s back as manifests of energy soars from it.",ch,NULL,NULL,TO_ROOM);
 
   act("$n staggers backwards as an energy bolt flashes into $m!",victim,NULL,NULL,TO_ROOM);
   act("You stagger backwards as an energy bolt flashes into you!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers backwards as an energy bolt flashes into $m!",victim,NULL,NULL,TO_ROOM);
   act("You stagger backwards as an energy bolt flashes into you!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers backwards as an energy bolt flashes into $m!",victim,NULL,NULL,TO_ROOM);
   act("You stagger backwards as an energy bolt flashes into you!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers backwards as an energy bolt flashes into $m!",victim,NULL,NULL,TO_ROOM);
   act("You stagger backwards as an energy bolt flashes into you!",victim,NULL,NULL,TO_CHAR);
   act("$n staggers backwards as an energy bolt flashes into $m!",victim,NULL,NULL,TO_ROOM);
   act("You stagger backwards as an energy bolt flashes into you!",victim,NULL,NULL,TO_CHAR);

   act("You then drive your katana upwards $N's body as you leap into the air.",ch,NULL,victim,TO_CHAR);
   act("$n then drives $s katana upwards your body as $e leaps into the air.",ch,NULL,victim,TO_VICT);
   act("$n then drives $s katana upwards $N's body as $e leaps into the air.",ch,NULL,victim,TO_NOTVICT);

   act("As $N got slammed onto the ground, you thrust your katana through $S chest.",ch,NULL,victim,TO_CHAR);
   act("As you got slammed onto the ground, $n thrusts $s katana through your chest.",ch,NULL,victim,TO_VICT);
   act("As $N got slammed onto the ground, $n thrusts $s katana through $S chest.",ch,NULL,victim,TO_NOTVICT);

   act("You backflip onto your feet ripping your katana through $N's body.",ch,NULL,victim,TO_CHAR);
   act("$n backflips onto $s feet ripping $s katana through your body.",ch,NULL,victim,TO_VICT);
   act("$n backflips onto $s feet ripping $s katana through $N's body.",ch,NULL,victim,TO_NOTVICT);

   victim->mana = UMAX( 0, victim->mana - 250000 );
   
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BEAM;
   ch->attack_focus = AERIAL;
   SET_BIT( ch->arena, AFF2_COMBO );

   hurt_person(ch, victim, 700000, TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_festive_wind( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = 700000;

   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You leap backwards as you coldly stare at $N.",ch,NULL,victim,TO_CHAR);
   act("$n leaps backwards as $e coldly stares at you.",ch,NULL,victim,TO_VICT);
   act("$n leaps backwards as $e coldly stares at $N.",ch,NULL,victim,TO_NOTVICT);
   act("Your image leaps backwards as it coldly stares at $N.",ch,NULL,victim,TO_CHAR);
   act("$n's image leaps backwards as it coldly stares at you.",ch,NULL,victim,TO_VICT);
   act("$n's image leaps backwards as it coldly stares at $N.",ch,NULL,victim,TO_NOTVICT);
   act("Your image leaps backwards as it coldly stares at $N.",ch,NULL,victim,TO_CHAR);
   act("$n's image leaps backwards as it coldly stares at you.",ch,NULL,victim,TO_VICT);
   act("$n's image leaps backwards as it coldly stares at $N.",ch,NULL,victim,TO_NOTVICT);

   act("Suddenly, you FLASH into $N with your katana, sending $M high into the sky.",ch,NULL,victim,TO_CHAR);
   act("Suddenly, $n FLASHES into you with $s katana, sending you high into the sky.",ch,NULL,victim,TO_VICT);
   act("Suddenly, $n FLASHES into $N with $s katana, sending $M high into the sky.",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly, your image FLASHES into $N with its katana, sending $M high into the sky.",ch,NULL,victim,TO_CHAR);
   act("Suddenly, $n's image FLASHES into you with its katana, sending you high into the sky.",ch,NULL,victim,TO_VICT);
   act("Suddenly, $n's image FLASHES into $N with its katana, sending $M high into the sky.",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly, your image FLASHES into $N with its katana, sending $M high into the sky.",ch,NULL,victim,TO_CHAR);
   act("Suddenly, $n's image FLASHES into you with its katana, sending you high into the sky.",ch,NULL,victim,TO_VICT);
   act("Suddenly, $n's image FLASHES into $N with its katana, sending $M high into the sky.",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly, your image FLASHES into $N with its katana, sending $M high into the sky.",ch,NULL,victim,TO_CHAR);
   act("Suddenly, $n's image FLASHES into you with its katana, sending you high into the sky.",ch,NULL,victim,TO_VICT);
   act("Suddenly, $n's image FLASHES into $N with its katana, sending $M high into the sky.",ch,NULL,victim,TO_NOTVICT);

   act("The room is filled with fog as a huge moon fades into existence.",ch,NULL,NULL,TO_CHAR);
   act("The room is filled with fog as a huge moon fades into existence.",ch,NULL,NULL,TO_ROOM);

   act("An ear-shrieking howl chills your heart as symbols appear around you.",victim,NULL,NULL,TO_CHAR);
   act("An ear-shrieking howl chills your heart as symbols appear around $n.",victim,NULL,NULL,TO_ROOM);

   act("You CRASH back onto the ground HaRd as the fog clears up.",victim,NULL,NULL,TO_CHAR);
   act("$n CRASHES back onto the ground HaRd as the fog clears up.",victim,NULL,NULL,TO_ROOM);

   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   SET_BIT( ch->arena, AFF2_COMBO );

   hurt_person( ch, victim, dam, TRUE );

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_earth_dance( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = 700000;

   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You leap towards $N twirling your body.",ch,NULL,victim,TO_CHAR);
   act("$n leaps towards you twirling $s body.",ch,NULL,victim,TO_VICT);
   act("$n leaps towards $N twirling $s body.",ch,NULL,victim,TO_NOTVICT);

   act("Closing your eyes softly, you RAM your katana behind you through $N's body.",ch,NULL,victim,TO_CHAR);
   act("Closing $s eyes softly, $n RAMS $s katana behind $m through your body.",ch,NULL,victim,TO_VICT);
   act("Closing $s eyes softly, $n RAMS $s katana behind $m through $N's body.",ch,NULL,victim,TO_NOTVICT);
   act("Closing its eyes softly, your image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_CHAR);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through your body.",ch,NULL,victim,TO_VICT);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_NOTVICT);
   act("Closing its eyes softly, your image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_CHAR);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through your body.",ch,NULL,victim,TO_VICT);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_NOTVICT);
   act("Closing its eyes softly, your image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_CHAR);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through your body.",ch,NULL,victim,TO_VICT);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_NOTVICT);
   act("Closing its eyes softly, your image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_CHAR);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through your body.",ch,NULL,victim,TO_VICT);
   act("Closing its eyes softly, $n's image RAMS its katana behind it through $N's body.",ch,NULL,victim,TO_NOTVICT);

   act("A spiral of leaves floats from the skies encircling your body.",victim,NULL,NULL,TO_CHAR);
   act("A spiral of leaves floats from the skies encircling $n's body.",victim,NULL,NULL,TO_ROOM);

   act("As the delicate leaves brush past you, you feel alot weaker.",victim,NULL,NULL,TO_CHAR);
   act("As the delicate leaves brush past $n, $e looks alot weaker.",victim,NULL,NULL,TO_ROOM);

   act("Drawing your katana out of $N's body, you leap back as $E collapses.",ch,NULL,victim,TO_CHAR);
   act("Drawing $s katana out of your body, $n leaps back as you collapse.",ch,NULL,victim,TO_VICT);
   act("Drawing $s katana out of $N's body, $n leaps back as $E collapses.",ch,NULL,victim,TO_NOTVICT);

   ch->ele_attack = ATTACK_EARTH;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = PIERCE;
   ch->attack_focus = DIRECT;
   SET_BIT( ch->arena, AFF2_COMBO );

   hurt_person(ch,victim,dam,TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_haze_fire( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = 700000;

   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("ExPlOsIvE fire sears from your scabbard as you coldly stare at $N.",ch,NULL,victim,TO_CHAR);
   act("ExPlOsIvE fire sears from $n's scabbard as $e coldly stares at you.",ch,NULL,victim,TO_VICT);
   act("ExPlOsIvE fire sears from $n's scabbard as $e coldly stares at $N.",ch,NULL,victim,TO_NOTVICT);

   act("You rush towards $N driving your katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you driving $s katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N driving $s katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you driving its katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);

   act("The room EXPLODES with FEROICIOUS energy burning $N as you land on your feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n lands on $s feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n lands on $s feet.",ch,NULL,victim,TO_NOTVICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as your image lands on its feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n's image lands on its feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n's image lands on its feet.",ch,NULL,victim,TO_NOTVICT);

   act("You rush towards $N driving your katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you driving $s katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N driving $s katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you driving its katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);

   act("The room EXPLODES with FEROICIOUS energy burning $N as you land on your feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n lands on $s feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n lands on $s feet.",ch,NULL,victim,TO_NOTVICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as your image lands on its feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n's image lands on its feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n's image lands on its feet.",ch,NULL,victim,TO_NOTVICT);

   act("You rush towards $N driving your katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you driving $s katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N driving $s katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you driving its katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);

   act("The room EXPLODES with FEROICIOUS energy burning $N as you land on your feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n lands on $s feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n lands on $s feet.",ch,NULL,victim,TO_NOTVICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as your image lands on its feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n's image lands on its feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n's image lands on its feet.",ch,NULL,victim,TO_NOTVICT);

   act("You rush towards $N driving your katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n rushes towards you driving $s katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n rushes towards $N driving $s katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);
   act("Your image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_CHAR);
   act("$n's image rushes towards you driving its katana upwards your body.",ch,NULL,victim,TO_VICT);
   act("$n's image rushes towards $N driving its katana upwards $S body.",ch,NULL,victim,TO_NOTVICT);

   act("The room EXPLODES with FEROICIOUS energy burning $N as you land on your feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n lands on $s feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n lands on $s feet.",ch,NULL,victim,TO_NOTVICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as your image lands on its feet.",ch,NULL,victim,TO_CHAR);
   act("The room EXPLODES with FEROICIOUS energy burning you as $n's image lands on its feet.",ch,NULL,victim,TO_VICT);
   act("The room EXPLODES with FEROICIOUS energy burning $N as $n's image lands on its feet.",ch,NULL,victim,TO_NOTVICT);

   if ( !IS_AFFECTED(victim, AFF_FLAMING) ) 
      SET_BIT(victim->affected_by, AFF_FLAMING);

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = UPPERCUT;
   SET_BIT( ch->arena, AFF2_COMBO );

   hurt_person(ch,victim,dam,TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_crystal_water( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int dam = 700000;

   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("Unbelievable energy radiates of your scabbard as you coldly stare at $N.",ch,NULL,victim,TO_CHAR);
   act("Unbelievable energy radiates of $n's scabbard as $e coldly stares at you.",ch,NULL,victim,TO_VICT);
   act("Unbelievable energy radiates of $n's scabbard as $e coldly stares at $N.",ch,NULL,victim,TO_NOTVICT);

   act("You dash towards $N leaping into the air SlAsHiNg your katana across $S body.",ch,NULL,victim,TO_CHAR);
   act("$n dashes towards you leaping into the air SlAsHiNg $s katana across your body.",ch,NULL,victim,TO_VICT);
   act("$n dashes towards $N leaping into the air SlAsHiNg $s katana across $S body.",ch,NULL,victim,TO_NOTVICT);

   act("Shards of energy zooms from $N's body as you SlAsH $M viciously across the chest.",ch,NULL,victim,TO_CHAR);
   act("Shards of energy zooms from your body as $n SlAsHeS you viciously across the chest.",ch,NULL,victim,TO_VICT);
   act("Shards of energy zooms from $N's body as $n SlAsHeS $M viciously across the chest.",ch,NULL,victim,TO_NOTVICT);
   act("Shards of energy zooms from $N's body as you SlAsH $M viciously across the chest.",ch,NULL,victim,TO_CHAR);
   act("Shards of energy zooms from your body as $n SlAsHeS you viciously across the chest.",ch,NULL,victim,TO_VICT);
   act("Shards of energy zooms from $N's body as $n SlAsHeS $M viciously across the chest.",ch,NULL,victim,TO_NOTVICT);
   act("Shards of energy zooms from $N's body as you SlAsH $M viciously across the chest.",ch,NULL,victim,TO_CHAR);
   act("Shards of energy zooms from your body as $n SlAsHeS you viciously across the chest.",ch,NULL,victim,TO_VICT);
   act("Shards of energy zooms from $N's body as $n SlAsHeS $M viciously across the chest.",ch,NULL,victim,TO_NOTVICT);
   act("Shards of energy zooms from $N's body as you SlAsH $M viciously across the chest.",ch,NULL,victim,TO_CHAR);
   act("Shards of energy zooms from your body as $n SlAsHeS you viciously across the chest.",ch,NULL,victim,TO_VICT);
   act("Shards of energy zooms from $N's body as $n SlAsHeS $M viciously across the chest.",ch,NULL,victim,TO_NOTVICT);

   act("Transparent symbols fades into $N's body while you SlAsH $M across the waist.",ch,NULL,victim,TO_CHAR);
   act("Transparent symbols fades into your body while $n SlAsHeS you across the waist.",ch,NULL,victim,TO_VICT);
   act("Transparent symbols fades into $N's body while $n SlAsHeS $M across the waist.",ch,NULL,victim,TO_NOTVICT);
   act("Transparent symbols fades into $N's body while you SlAsH $M across the waist.",ch,NULL,victim,TO_CHAR);
   act("Transparent symbols fades into your body while $n SlAsHeS you across the waist.",ch,NULL,victim,TO_VICT);
   act("Transparent symbols fades into $N's body while $n SlAsHeS $M across the waist.",ch,NULL,victim,TO_NOTVICT);
   act("Transparent symbols fades into $N's body while you SlAsH $M across the waist.",ch,NULL,victim,TO_CHAR);
   act("Transparent symbols fades into your body while $n SlAsHeS you across the waist.",ch,NULL,victim,TO_VICT);
   act("Transparent symbols fades into $N's body while $n SlAsHeS $M across the waist.",ch,NULL,victim,TO_NOTVICT);

   act("$n's body ImPlOdEs as the symbols ROARS out of $m, encasing $m in ice.",victim,NULL,NULL,TO_ROOM);
   act("Your body ImPlOdEs as the symbols ROARS out of you, encasing you in ice.",victim,NULL,NULL,TO_CHAR);

   act("$N CraSHeS onto the ground as you land back softly on your feet.",ch,NULL,victim,TO_CHAR);
   act("You CraSH onto the ground as $n lands back softly on $s feet.",ch,NULL,victim,TO_VICT);
   act("$N CraSHeS onto the ground as $n lands back softly on $s feet.",ch,NULL,victim,TO_NOTVICT);

   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = AERIAL;
   SET_BIT( ch->arena, AFF2_COMBO );

   hurt_person(ch,victim,dam,TRUE);

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

void do_nikitsu( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You tilt yourself forward as you softly put your hand on the hilt of your scabbard.",ch,NULL,NULL,TO_CHAR);
   act("$n tilts $mself forward as $e softly puts $s hand on the hilt of $s scabbard.",ch,NULL,NULL,TO_ROOM);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_CHAR);
   act("#wMassive energy emits of you as you close your eyes tightening your grip on the katana.#n\r\r",ch,NULL,NULL,TO_ROOM);

   act("You grin evilly as you raise your katana high above your head.",ch,NULL,NULL,TO_CHAR);
   act("$n grins evilly as $e raises $s katana high above $s head.",ch,NULL,NULL,TO_ROOM);

   act("You yell '#yEzai!!!#n' as you rush towards $N and thrust your katana through $S heart!",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yEzai!!!#n' as $e rushes towards you and thrusts $s katana through your heart!",ch,NULL,victim,TO_VICT);
   act("$n yells '#yEzai!!!#n' as $e rushes towards $N and thrusts $s katana through $S heart!",ch,NULL,victim,TO_NOTVICT);

   act("Suddenly the room goes dark as you RiP your katana up through $N's body!",ch,NULL,victim,TO_CHAR);
   act("Suddenly the room goes dark as $n RiPs $s katana up through your body!",ch,NULL,victim,TO_VICT);
   act("Suddenly the room goes dark as $s RiPs $s katana up through $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("Suddenly the room goes dark as your image RiPs its katana up through $N's body!",ch,NULL,victim,TO_CHAR);
   act("Suddenly the room goes dark as $n's image RiPs its katana up through your body!",ch,NULL,victim,TO_VICT);
   act("Suddenly the room goes dark as $s's image RiPs its katana up through $N's body!",ch,NULL,victim,TO_NOTVICT);

   act("the room ExPlOdEs with MASSIVE energy as an unseen force ANNIHILATES $n!!!",victim,NULL,NULL,TO_ROOM);
   act("the room ExPlOdEs with MASSIVE energy as an unseen force ANNIHILATES you!!!",victim,NULL,NULL,TO_CHAR);

   SET_BIT( ch->arena, AFF2_COMBO );
   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_DARK;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 700000, TRUE );

   act("\n\rThere is a loud CLASP as you lean forward onto one knee, sheathing your katana ViCiOuSlY!!!",ch,NULL,NULL,TO_CHAR);
   act("\n\rThere is a loud CLASP as $n leans forward onto one knee, sheathing $s katana ViCiOuSlY!!!",ch,NULL,NULL,TO_ROOM);
}

bool is_sam_combo( CHAR_DATA *ch, CHAR_DATA *victim, int move_num )
{
   if ( move_num == 2 )
      {
         if ( ch->nlist[0] == 1 && ch->nlist[1] == 3 )
            {
                do_amaoto(ch,victim);
                ch->wait = 12;

                return TRUE;
            }

         if ( ch->nlist[0] == 2 && ch->nlist[1] == 3 )
            {
                do_amaguma(ch,victim);
                ch->wait = 12;

                return TRUE;
            }

         if ( ch->nlist[0] == 3 && ch->nlist[1] == 3 )
            {
                do_myogetsu(ch,victim);
                ch->wait = 12;

                return TRUE;
            }
      }

   if ( move_num == 3 )
      {
         if ( ch->nlist[0] == 1 && ch->nlist[1] == 1 && ch->nlist[2] == 3 )
            {
                do_engetsu(ch,victim);
                ch->wait = 8;

                return TRUE;
            }

         if ( ch->nlist[0] == 1 && ch->nlist[1] == 2 && ch->nlist[2] == 3 )
            {
                do_yako(ch,victim);
                ch->wait = 8;

                return TRUE;
            }

         if ( ch->nlist[0] == 2 && ch->nlist[1] == 1 && ch->nlist[2] == 3 )
            {
                do_zanretsu(ch,victim);
                ch->wait = 8;

                return TRUE;
            }
      }

   if ( move_num == 4 )
      {
         if ( ch->nlist[0] == 1 && ch->nlist[1] == 1 && ch->nlist[2] == 1
              && ch->nlist[3] == 3 )
            {
                do_himatsu(ch,victim);
                ch->wait = 7;

                return TRUE;
            }

         if ( ch->nlist[0] == 1 && ch->nlist[1] == 1 && ch->nlist[2] == 2
              && ch->nlist[3] == 3 )
            {
                do_earth_dance(ch,victim);
                ch->wait = 7;

                return TRUE;
            }

         if ( ch->nlist[0] == 1 && ch->nlist[1] == 2 && ch->nlist[2] == 1
              && ch->nlist[3] == 3 )
            {
                do_haze_fire(ch,victim);
                ch->wait = 7;

                return TRUE;
            }

         if ( ch->nlist[0] == 2 && ch->nlist[1] == 1 && ch->nlist[2] == 1
              && ch->nlist[3] == 3 )
            {
                do_crystal_water(ch,victim);
                ch->wait = 7;

                return TRUE;
            }
      }

   if ( move_num == 5 )
      {
         if ( ch->nlist[0] == 1 && ch->nlist[1] == 1 && ch->nlist[2] == 1
              && ch->nlist[3] == 1 && ch->nlist[4] == 3 )
            {
                do_festive_wind(ch,victim);
                ch->wait = 5;

                return TRUE;
            }

         if ( ch->nlist[0] == 2 && ch->nlist[1] == 2 && ch->nlist[2] == 1
              && ch->nlist[3] == 3 && ch->nlist[4] == 2 )
            {
                do_nikitsu(ch,victim);
                ch->wait = 5;

                return TRUE;
            }
      }

   return FALSE;
}

void do_elemental_slash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   int count;

   if ( !IS_CLASS( ch, CLASS_SAMURAI ) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ch->pkill < 10 )
      {
         stc("Your mastery of the Katana isn't high enough to do this technique.\n\r",ch);
         return;
      }

   if ( ch->ctimer[3] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ( victim = ch->fighting ) == NULL )
      {
	   send_to_char( "But you aren't in a fight warrior.\n\r", ch );
	   return;
      }
    
   if ( ch->move < 3000000 )
      {
	   send_to_char( "You are too tired to execute this technique.\n\r", ch );
	   return;
      }

   if ( is_safe(ch, victim) )
      return;

   ch->ctimer[3] = 6;
   ch->wait = 12;
   ch->move -= 3000000;

   for ( count = 0; count < 2; count++ )
   {
       switch( number_range( 1, 5 ) )
       {
          default:
          case 1: do_earth_dance(ch,victim);
                  break;
          case 2: do_haze_fire(ch,victim);
                  break;
          case 3: do_crystal_water(ch,victim);
                  break;
          case 4: do_festive_wind(ch,victim);
                  break;
          case 5: do_nikitsu(ch,victim);
                  break;
       }

       if ( victim == NULL || ch == NULL || victim->in_room != ch->in_room || victim->position < POS_STUNNED )
          return;
   }
}

void do_light_slash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int dam, move;
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( !IS_CLASS(ch,CLASS_SAMURAI) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to slash?\n\r", ch );
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

   if ( is_safe(ch, victim) )
      return;

   move = 1;
   ch->nlist[ch->special_id_2] = move;
   ch->special_id_2++;

   if ( is_sam_combo(ch, victim, ch->special_id_2) )
      {
         for ( move = 0; move < 5; move++ )
             ch->nlist[move] = 0;

         ch->special_id_2 = 0;
         return;
      }

   if ( ch->special_id_2 > 5 )
      {
         act("You leap backwards as you sheath your katana, waiting for the next opening.",ch,NULL,NULL,TO_CHAR);
         act("$n leaps backwards as $e sheathes $s katana, waiting for the next opening.",ch,NULL,NULL,TO_ROOM);
         ch->special_id_2 = 0;
         ch->wait = 6;
         return;
      }

   dam = 5000;

   act("You smash your scabbard across $N's face as $E staggers back from your blow.",ch,NULL,victim,TO_CHAR);
   act("$n smashes $s scabbard across your face as you stagger back from $s blow.",ch,NULL,victim,TO_VICT);
   act("$n smashes $s scabbard across $N's face as $E staggers back from $s blow.",ch,NULL,victim,TO_NOTVICT);

   hurt_person(ch,victim,dam,TRUE);
}

void do_samurai_hslash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int dam, move;
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( !IS_CLASS(ch,CLASS_SAMURAI) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to slash?\n\r", ch );
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

   if ( is_safe(ch, victim) )
      return;

   move = 3;
   ch->nlist[ch->special_id_2] = move;
   ch->special_id_2++;

   if ( is_sam_combo(ch, victim, ch->special_id_2) )
      {
         for ( move = 0; move < 5; move++ )
             ch->nlist[move] = 0;

         ch->special_id_2 = 0;
         return;
      }

   if ( ch->special_id_2 > 5 )
      {
         act("You leap backwards as you sheath your katana, waiting for the next opening.",ch,NULL,NULL,TO_CHAR);
         act("$n leaps backwards as $e sheathes $s katana, waiting for the next opening.",ch,NULL,NULL,TO_ROOM);
         ch->special_id_2 = 0;
         ch->wait = 6;
         return;
      }

   dam = 5000;

   act("You yell '#yEai!#n' as you DRIVE the katana down $N's body.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yEai!#n' as $e DRIVES the katana down your body.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yEai!#n' as $e DRIVES the katana down $N's body.",ch,NULL,victim,TO_NOTVICT);

   hurt_person(ch,victim,dam,TRUE);
}

void do_samurai_mslash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int dam, move;
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if ( !IS_CLASS(ch,CLASS_SAMURAI) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to slash?\n\r", ch );
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

   if ( is_safe(ch, victim) )
      return;

   move = 2;
   ch->nlist[ch->special_id_2] = move;
   ch->special_id_2++;

   if ( is_sam_combo(ch, victim, ch->special_id_2) )
      {
         for ( move = 0; move < 5; move++ )
             ch->nlist[move] = 0;

         ch->special_id_2 = 0;
         return;
      }

   if ( ch->special_id_2 > 5 )
      {
         act("You leap backwards as you sheath your katana, waiting for the next opening.",ch,NULL,NULL,TO_CHAR);
         act("$n leaps backwards as $e sheathes $s katana, waiting for the next opening.",ch,NULL,NULL,TO_ROOM);
         ch->special_id_2 = 0;
         ch->wait = 6;
         return;
      }

   dam = 5000;

   act("You yell '#yTsunmi!#n' as you ThRuSt the katana through $N's body.",ch,NULL,victim,TO_CHAR);
   act("$n yells '#yTsunmi!#n' as $e ThRuStS the katana through your body.",ch,NULL,victim,TO_VICT);
   act("$n yells '#yTsunmi!#n' as $e ThRuStS the katana through $N's body.",ch,NULL,victim,TO_NOTVICT);

   hurt_person(ch,victim,dam,TRUE);
}

void do_withdraw( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( !IS_CLASS(ch,CLASS_SAMURAI) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   one_argument( argument, arg );

   act("You leap backwards as you sheath your katana, waiting for the next opening.",ch,NULL,NULL,TO_CHAR);
   act("$n leaps backwards as $e sheathes $s katana, waiting for the next opening.",ch,NULL,NULL,TO_ROOM);
   ch->special_id_2 = 0;
   ch->wait = 4;
}

void do_combo( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int hits;
    OBJ_DATA *obj;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch,CLASS_SAMURAI) )
       {
          stc("Huh?\n\r",ch);
          return;
       }

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
       if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
          if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
          {
             send_to_char( "You are not holding a sword.\n\r", ch );
             return;
          }

    if ( get_stock( ch ) < 100 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( ( victim = ch->fighting ) == NULL )
       {
          send_to_char( "But you aren't in a fight warrior.\n\r", ch );
          return;
       }

    ch->wait = 12;
    ch->special_id_2 = 0;
    SET_BIT( ch->arena, AFF2_DM );

    if ( ch->ctimer[15] > 0 )
       hits = 1;
       else if ( get_stock( ch ) == 300 )
       {
          hits = 7;
          subtract_stock( ch, 300 );
       }
       else if ( get_stock( ch ) > 199 )
               {
                  hits = 4;
                  subtract_stock( ch, 200 );
               }
       else {
               subtract_stock( ch, 100 );
               hits = 2;
            }

    while ( hits > 0 )
    {
        switch( number_range( 1, 7 ) )
        {
           default:
           case 1: do_amaoto( ch, victim );
                   break;

           case 2: do_engetsu( ch, victim );
                   break;

           case 3: do_amaguma( ch, victim );
                   break;

           case 4: do_himatsu( ch, victim );
                   break;

           case 5: do_yako( ch, victim );
                   break;

           case 6: do_zanretsu( ch, victim );
                   break;

           case 7: do_myogetsu( ch, victim );
                   break;
        }

        if ( victim == NULL || victim->in_room == NULL || ch->fighting == NULL )
           return;

        hits--;
    }
}

void do_quick_lash( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;

   if ( !IS_CLASS(ch,CLASS_SAMURAI) )
      {
         stc("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         if ( ( obj = get_eq_char( ch, WEAR_BACK ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return;
         }

   argument = one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         stc("Whom do you wish to slash?\n\r",ch);
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

   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 300 );

   act("You close your eyes calmly as you hover your free hand over your katana.",ch,NULL,NULL,TO_CHAR);
   act("$n closes $s eyes calmly as $e hovers $s free hand over $s katana.",ch,NULL,NULL,TO_ROOM);
   act("You mutter praises to the emperor as a glint of light radiates of your blade.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters praises to the emperor as a glint of light radiates of $s blade.",ch,NULL,NULL,TO_ROOM);
   act("You suddenly dash right past $N as your hands move like a blur!!!",ch,NULL,victim,TO_CHAR);
   act("$n suddenly dashes right past you as $s hands move like a blur!!!",ch,NULL,victim,TO_VICT);
   act("$n suddenly dashes right past $N as $s hands move like a blur!!!",ch,NULL,victim,TO_NOTVICT);
   act("You look puzzled as sparks of light appear around your waist...",victim,NULL,NULL,TO_CHAR);
   act("$n looks puzzled as sparks of light appear around $s waist...",victim,NULL,NULL,TO_ROOM);
   act("You sheath your katana with a LOUD clasp as a deadly light rips through $N's body!!!",ch,NULL,victim,TO_CHAR);
   act("$n sheathes $s katana with a LOUD clasp as a deadly light rips through your body!!!",ch,NULL,victim,TO_VICT);
   act("$n sheathes $s katana with a LOUD clasp as a deadly light rips through $N's body!!!",ch,NULL,victim,TO_NOTVICT);
   act("You chuckle silently to yourself as $N falls to the ground in pain.",ch,NULL,victim,TO_CHAR);
   act("$n chuckles silently to $mself as you fall to the ground in pain.",ch,NULL,victim,TO_VICT);
   act("$n chuckles silently to $mself as $N falls to the ground in pain.",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 3500000, TRUE );
}
