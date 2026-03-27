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

void do_swordcreate( CHAR_DATA *ch, char *argument )
{
   char arg [MAX_INPUT_LENGTH];
   int cost, vnum;
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch, CLASS_SORCEROR) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char( "Syntax: swordcreate <item>\n\r", ch );
         send_to_char( "items are: ring (1), chain (1), robe (1), pants (1)\n\r", ch );
         send_to_char( "           sandals (1), shoulderguards (1), turban (1), cloak (1)\n\r", ch );
         send_to_char( "           handkerchief (1), gloves (1), bracer (1), sword (1)\n\r", ch );
         return;
      }

   cost = 1;

   if ( !str_cmp(arg,"ring") )
      vnum = 142;
      else if ( !str_cmp(arg,"chain") )
              vnum = 143;
      else if ( !str_cmp(arg,"robe") )
              vnum = 144;
      else if ( !str_cmp(arg,"pants") )
              vnum = 145;
      else if ( !str_cmp(arg,"sandals") )
              vnum = 146;
      else if ( !str_cmp(arg,"shoulderguards") )
              vnum = 147;
      else if ( !str_cmp(arg,"turban") )
              vnum = 148;
      else if ( !str_cmp(arg,"cloak") )
              vnum = 149;
      else if ( !str_cmp(arg,"handkerchief") )
              vnum = 150;
      else if ( !str_cmp(arg,"gloves") )
              vnum = 151;
      else if ( !str_cmp(arg,"bracer") )
              vnum = 152;
      else if ( !str_cmp(arg,"sword") )
              vnum = 156;
              else {
                      do_swordcreate(ch,"");
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
   act("You manipulate your magikal energy as $p appears in your hands.",ch,obj,NULL,TO_CHAR);
   act("$n manipulates $s magikal energy as $p appears in $s hands.",ch,obj,NULL,TO_ROOM);
}

void do_swordlearn( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) return;

   if ( !IS_CLASS(ch, CLASS_SORCEROR) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( arg[0] == '\0' )
      {
         centre_text("#w<<<-----(<#c Swords of the Ages #w>)----->>>#n",ch);
         centre_text("#c\\^<>^/#n",ch);
         centre_text("#c \\/ #n",ch);
         send_to_char("\n\r",ch);
         sprintf(buf,"Swords of #LIce#n      : %d\n\r", ch->sorcerorab[SICE]);
         send_to_char(buf,ch);
         sprintf(buf,"Swords of #yThunder#n  : %d\n\r", ch->sorcerorab[STHUNDER]);
         send_to_char(buf,ch);
         sprintf(buf,"Swords of #RFire#n     : %d\n\r", ch->sorcerorab[SFIRE]);
         send_to_char(buf,ch);
         sprintf(buf,"Swords of #gEarth#n    : %d\n\r", ch->sorcerorab[SEARTH]);
         send_to_char(buf,ch);
         sprintf(buf,"Swords of #yMoon#n     : %d\n\r", ch->sorcerorab[SMOON]);
         send_to_char(buf,ch);
         sprintf(buf,"Swords of #eDarkness#n : %d\n\r", ch->sorcerorab[SDARK]);
         send_to_char(buf,ch);
         centre_text("#w<<<----------------------------------------------->>>#n\n",ch);
         return;
      }

   if ( !str_cmp(arg,"fire") )
      {
         if ( ch->sorcerorab[SFIRE] >= 9 )
            {
               send_to_char("#RYou had already learn all of the swords of fire.#n\n\r",ch);
               return;
            }

         ch->sorcerorab[SFIRE]++;
         sprintf(buf,"#RYour experience of the Swords of Fire improve.#n\n\r");
         send_to_char(buf,ch);
      }
      else if ( !str_cmp(arg,"ice") )
      {
         if ( ch->sorcerorab[SICE] >= 9 )
            {
               send_to_char("#LYou had already learn all of the swords of ice.#n\n\r",ch);
               return;
            }

         ch->sorcerorab[SICE]++;
         sprintf(buf,"#LYour experience of the Swords of Ice improve.#n\n\r");
         send_to_char(buf,ch);
      }
      else if ( !str_cmp(arg,"thunder") )
      {
         if ( ch->sorcerorab[STHUNDER] >= 9 )
            {
               send_to_char("#yYou had already learn all of the swords of thunder.#n\n\r",ch);
               return;
            }

         ch->sorcerorab[STHUNDER]++;
         sprintf(buf,"#yYour experience of the Swords of Thunder improve.#n\n\r");
         send_to_char(buf,ch);
      }
      else if ( !str_cmp(arg,"earth") )
      {
         if ( ch->sorcerorab[SEARTH] >= 9 )
            {
               send_to_char("#gYou had already learn all of the swords of earth.#n\n\r",ch);
               return;
            }

         ch->sorcerorab[SEARTH]++;
         sprintf(buf,"#gYour experience of the Swords of Earth improve.#n\n\r");
         send_to_char(buf,ch);
      }
      else if ( !str_cmp(arg,"moon") && ch->clannum == 2 )
      {
         if ( ch->sorcerorab[SMOON] >= 9 )
            {
               send_to_char("#yYou had already learn all of the swords of moon.#n\n\r",ch);
               return;
            }

         ch->sorcerorab[SMOON]++;
         sprintf(buf,"#yYour experience of the Swords of Moon improve.#n\n\r");
         send_to_char(buf,ch);
      }
      else if ( !str_cmp(arg,"darkness") && ch->clannum == 1 )
      {
         if ( ch->sorcerorab[SDARK] >= 9 )
            {
               send_to_char("#eYou had already learn all of the swords of darkness.#n\n\r",ch);
               return;
            }

         ch->sorcerorab[SDARK]++;
         sprintf(buf,"#eYour experience of the Swords of Darkness improve.#n\n\r");
         send_to_char(buf,ch);
      }
      else {
             send_to_char("To improve your knowledge, type: swordlearn #RFire#P/#bIce#P/#yLightning#P/#gEarth#n.\n\r",ch);
             send_to_char("                        Or type: swordlearn #eDarkness#P/#yMoon#n.\n\r",ch);
             return;
           }
}

bool is_sword( CHAR_DATA *ch )
{
   OBJ_DATA *obj;
   OBJ_DATA *obj2;

   if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
      if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || ( obj->value[3] != 3 && obj->pIndexData->vnum != 91 ) )
         {
            send_to_char( "You are not holding a sword.\n\r", ch );
            return FALSE;
         }

   if ( ( obj2 = get_eq_char( ch, WEAR_WIELD ) ) != NULL )
      if ( ( obj2 = get_eq_char( ch, WEAR_HOLD ) ) != NULL )
         {
             send_to_char( "You need a free hand to utilize this technique.\n\r", ch );
             return FALSE;
         }

   return TRUE;
}

bool is_ele_sword( CHAR_DATA *ch, int ele_type )
{
   OBJ_DATA *obj;

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) || !IS_SET(obj->weapflags, ele_type) )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) || !IS_SET(obj->weapflags, ele_type) )
         {
             send_to_char( "Your sword is of the wrong element for this technique.\n\r", ch );
             return FALSE;
         }

   return TRUE;
}

void do_piercing_one( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int count;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !is_sword( ch ) )
      return;

   ch->wait = 8;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   act("Lowering your sword to your side, you grin as you point your left hand towards your enemies.",ch,NULL,NULL,TO_CHAR);
   act("Lowering $s sword to $s side, $n grins as $e points $s left hand towards you.",ch,NULL,NULL,TO_ROOM);
   act("A manifest of energy swerves around your finger as the sky darkens.",ch,NULL,NULL,TO_CHAR);
   act("A manifest of energy swerves around $n's finger as the sky darkens.",ch,NULL,NULL,TO_ROOM);
   act("Multiple lightning bolts flashes through the vicinity as hundrends of mystical portals appear.",ch,NULL,NULL,TO_CHAR);
   act("Multiple lightning bolts flashes through the vicinity as hundrends of mystical portals appear.",ch,NULL,NULL,TO_ROOM);
   act("Focusing on your magick, the portals start shimmering violently.",ch,NULL,NULL,TO_CHAR);
   act("Focusing on $s magick powers, the portals start shimmering violently.",ch,NULL,NULL,TO_ROOM);
   act("A huge sword slowly glides through the portal, blinking in and out of existence as it hovers in the air.",ch,NULL,NULL,TO_CHAR);
   act("A huge sword slowly glides through the portal, blinking in and out of existence as it hovers in the air.",ch,NULL,NULL,TO_ROOM);
   act("Then another sword glides through a portal, followed by another and another.",ch,NULL,NULL,TO_CHAR);
   act("Then another sword glides through a portal, followed by another and another.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly the heavens is filled with thousands of spiritual swords, creating a rift of fear.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly the heavens is filled with thousands of spiritual swords, creating a rift of fear.",ch,NULL,NULL,TO_ROOM);
   act("\n\rLeaning forward, you grin as you push your palm towards the center of the room.",ch,NULL,NULL,TO_CHAR);
   act("\n\rLeaning forward, $n grins as $e pushes $s palm towards the center of the room.",ch,NULL,NULL,TO_ROOM);

   act("You exclaim '#yHungry Fiends, show them the true power of the Swords... Piercing One!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n exclaims '#yHungry Fiends, show them the true power of the Swords... Piercing One!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("Explosions DeVaStAtEs ThE rOoM aS tHe SWoRd RAINS dOwn On Its ViCTiMS!!!",ch,NULL,NULL,TO_CHAR);
   act("Explosions DeVaStAtEs ThE rOoM aS tHe SWoRd RAINS dOwn On Its ViCTiMS!!!",ch,NULL,NULL,TO_ROOM);

   for ( count = 0; count < 3; count++ )
   {
      act("The vicinity ERUPTS as HuNdReNdS of SWORDS sHATtErS tHe VeRY gROunD!!!",ch,NULL,NULL,TO_CHAR);
      act("The vicinity ERUPTS as HuNdReNdS of SWORDS sHATtErS tHe VeRY gROunD!!!",ch,NULL,NULL,TO_ROOM);

      for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
        vch_next = vch->next_in_room;

        if ( is_safe(ch, vch) )
           continue;
 
        if ( !IS_SET(vch->more, MORE_HEX) )
           SET_BIT(vch->more, MORE_HEX);

        if ( !IS_SET(vch->more, MORE_BLIND) )
           {
              SET_BIT(vch->more, MORE_BLIND);
              vch->blind_timer = 8;
           }

        if ( !IS_SET(vch->more, MORE_IMPALE) )
           SET_BIT(vch->more, MORE_IMPALE);

        ch->attack_type = ATTACK_MAGICAL;
        ch->attack_var = PIERCE;
        ch->attack_focus = AERIAL;

        if ( ch->ctimer[15] == 0 )
           hurt_person( ch, vch, 500000, TRUE );
      }
   }

   act("the entire area EXPLODES as a huge SWORD drives through the GrOunD, DESTROYING EvErYtHiNG!!!!",ch,NULL,NULL,TO_CHAR);
   act("the entire area EXPLODES as a huge SWORD drives through the GrOunD, DESTROYING EvErYtHiNG!!!!",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->in_room->area->bits,WASTE);
   REMOVE_BIT(ch->in_room->area->bits,UPDATEWASTE);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
        vch_next = vch->next_in_room;

        if ( is_safe(ch, vch) )
           continue;
 
        stc("You body is RiPPed ToO sHrEdS as a HUGE aToMIc BoMb SeArS ThrOuGH YOUR FlEsH!!!\n\r",vch);
        act("$n's body is RiPPed ToO sHrEdS as a HUGE aToMIc BoMb SeArS ThrOuGH $s FlEsH!!!",vch,NULL,NULL,TO_ROOM);

        ch->attack_type = ATTACK_MAGICAL;
        ch->attack_var = BLAST;
        ch->attack_focus = UPPERCUT;
        hurt_person( ch, vch, 1500000, TRUE );
   }
}

void do_aura_wave( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int cost;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 100000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 100000;
   cost = 300000;

   act("You say, '#yNu tie Zunj Darjs!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNu tie Zunj Darjs!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You slowly start twirling your sword about your body.",ch,NULL,NULL,TO_CHAR);
   act("$n slowly starts twirling $s sword about $s body.",ch,NULL,NULL,TO_ROOM);
   act("As your twirl your blade faster, a swiral of light encircles you.",ch,NULL,NULL,TO_CHAR);
   act("As $n twirls $s blade faster, a swiral of light encircles $m.",ch,NULL,NULL,TO_ROOM);
   act("The swirling light suddenly burst into shimmering colours, scattering across the room.",ch,NULL,NULL,TO_CHAR);
   act("The swirling light suddenly burst into shimmering colours, scattering across the room.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) )
         continue;

      if ( !can_heal(ch,vch) )
         continue;

      act("You feel refresh as the colours brushes pass your body.",vch,NULL,NULL,TO_CHAR);
      act("$n feels refresh as the colours brushes pass $s body.",vch,NULL,NULL,TO_ROOM);
      hit_gain( vch, cost );
   }

   ch->wait = 12;
}

void perform_solkatti( CHAR_DATA *ch, CHAR_DATA *victim )
{
   act("You smirk at $N's stupidity as you flick your blade free from its sheath.",ch,NULL,victim,TO_CHAR);
   act("$n smirks at your stupidity as $e flicks $s blade free from its sheath.",ch,NULL,victim,TO_VICT);
   act("$n smirks at $N's stupidity as $e flicks $s blade free from its sheath.",ch,NULL,victim,TO_NOTVICT);
   act("You then leap into the air as blinding light radiates off Sol Katti.",ch,NULL,NULL,TO_CHAR);
   act("$n then leaps into the air as blinding light radiates off Sol Katti.",ch,NULL,NULL,TO_ROOM);
   act("Just as you are about to strike $N's head, you suddenly disappear...",ch,NULL,victim,TO_CHAR);
   act("Just as $n is about to strike your head, $e suddenly disappears...",ch,NULL,victim,TO_VICT);
   act("Just as $n is about to strike $N's head, $e suddenly disappears...",ch,NULL,victim,TO_NOTVICT);
   act("... to appear behind $M as you thrust your blade there!",ch,NULL,victim,TO_CHAR);
   act("... to appear behind you as $e thrusts $s blade there!",ch,NULL,victim,TO_VICT);
   act("... to appear behind $M as $e thrusts $s blade there!",ch,NULL,victim,TO_NOTVICT);
   act("You then twirl around in 4 different directions as you split into 4 images!!!",ch,NULL,NULL,TO_CHAR);
   act("$n then twirls around in 4 different directions as $e splits into 4 images!!!",ch,NULL,NULL,TO_ROOM);
   act("You crouch on your knee as your images dance their blades around $N's body.",ch,NULL,victim,TO_CHAR);
   act("$n crouches on $s knee as $e also appears to dance $s blades around your body.",ch,NULL,victim,TO_VICT);
   act("$n crouches on $s knee as $e also appears to dance $s blades around $N's body.",ch,NULL,victim,TO_NOTVICT);
   act("Time slows down as a loud humming fills the air from your glowing sword.",ch,NULL,NULL,TO_CHAR);
   act("Time slows down as a loud humming fills the air from $n's glowing sword.",ch,NULL,NULL,TO_ROOM);
   act("You scream '#yHui!#n' as you drive the blade through $N's chest!",ch,NULL,victim,TO_CHAR);
   act("$n screams '#yHui!#n' as $e drives the blade through your chest!",ch,NULL,victim,TO_VICT);
   act("$n screams '#yHui!#n' as $e drives the blade through $N's chest!",ch,NULL,victim,TO_NOTVICT);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->ele_attack = ATTACK_HOLY;
   ch->attack_focus = DIRECT;
   ch->attack_var = PIERCE;

   victim->hit -= 2000000;  
   hurt_person( ch, victim, 0, TRUE );
}

void do_sol_katti( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next, *victim;
   int sn;
   char arg[MAX_INPUT_LENGTH];

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->pkill < 30 )
      {
         send_to_char( "#CYou #chaven't mastered this aspect of the #esword #cas yet #Pmagi #Cknight#c.#n\n", ch ); 
         return;
      }

   argument = one_argument( argument, arg );

   if ( arg[0] == '\0' )
      {
         if ( IS_SET( ch->special, SPC_SWORD ) )
            send_to_char("#cSyntax: #wSol#eKatti #cOffense#n\n\r", ch);
            else send_to_char("#cSyntax: #wSol#eKatti #cOffense#w/#cDefense#e/#wFocus#e/#wCritical#n\n\r", ch);
         return;
      }

   if ( !str_cmp(arg, "defense") && !IS_SET( ch->special, SPC_SWORD ) )
      {
         if ( ( sn = skill_lookup( "sol katti" ) ) < 0 )
            return;

         ch->wait = 18;
         act("You say, '#ySol Katti!!#n' as you let go a slick grin.",ch,NULL,NULL,TO_CHAR);
         act("$n says, '#ySol Katti!!#n' as $e lets go a slick grin.",ch,NULL,NULL,TO_ROOM);
         act("Pulling out a slim blade, you raise it proudly above you.",ch,NULL,NULL,TO_CHAR);
         act("Pulling out a slim blade, $n raises it proudly above $m.",ch,NULL,NULL,TO_ROOM);
         act("You then strut forward as you swerve the blade about stylishly.",ch,NULL,NULL,TO_CHAR);
         act("$n then struts forward as $e swerves the blade about stylishly.",ch,NULL,NULL,TO_ROOM);

         for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
         {
            vch_next = vch->next_in_room;

            if ( IS_NPC(vch) )
               continue;

            if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
                 && ch->pcdata->kingdom > 0 ) )
               continue;

            if ( is_affected( vch, sn ) )
               continue;

            act("Your body suddenly burst out with a dazzling aura as you feel more energetic.",vch,NULL,NULL,TO_CHAR);
            act("$n's body suddenly burst out with a dazzling aura as $e feels more energetic.",vch,NULL,NULL,TO_ROOM);

            (*skill_table[sn].spell_fun) ( sn, 10, vch, vch );
         }
 
         return;
      }

   if ( !str_cmp(arg, "critical") && !IS_SET( ch->special, SPC_SWORD ) )
      {
         CHAR_DATA *mob;

         mob = get_mob( 103 );

         if ( mob == NULL )
            return;

         ch->wait = 18;
         act("You go on your knees as you silently pray for Luc's soul.",ch,NULL,NULL,TO_CHAR);
         act("$n goes on $s knees as $e silently prays for Luc's soul.",ch,NULL,NULL,TO_ROOM);
         act("Suddenly you feel blessed by the presence of a young girl.",ch,NULL,NULL,TO_CHAR);
         act("Suddenly $n feels blessed by the presence of a young girl.",ch,NULL,NULL,TO_ROOM);
         act("Nino says '#yWarrior of the Rogue Clan... I will help you.#n'",ch,NULL,NULL,TO_CHAR);
         act("Nino says '#yWarrior of the Rogue Clan... I will help you.#n'",ch,NULL,NULL,TO_ROOM);
         do_chat( mob, "Nino... I won't fail you again..." );

         jaggareffect = 50;
         do_info( mob, "#rJaggar#e has blessed the vicinity with a #w50#e% critical attack chance..." );
         return;
      }

   if ( !str_cmp(arg, "offense") )
      {
         if ( ch->blocking < 0 )
            {
               stc( "#wYou can execute this #ytechnique #wagain in a moment warrior.#n\n\r", ch );
               return;
            }

         if ( ch->aragami_timer > 0 )
            {
               stc("You are still recovering from the last one.\n\r",ch);
               return;
            }

         act("#CYou #csmile to yourself as you place your #ohand #cat your #eblade#c.#n",ch,NULL,NULL,TO_CHAR);
         act("#C$n #csmiles to $mself as $e places $s #ohand #cat $s #eblade#c.#n",ch,NULL,NULL,TO_ROOM);;
         act("#yYou #Cclose your eyes as you think on #wpleasant #Cthoughts.#n",ch,NULL,NULL,TO_CHAR);
         act("#y$n #Ccloses $s eyes as $e thinks on #wpleasant #Cthoughts.#n",ch,NULL,NULL,TO_ROOM);

         ch->aragami_timer = 40;
         ch->blocking = 12;
         ch->ninjastuff = 120;
         SET_BIT( ch->arena, AFF2_AUTOGUARD );
         return;
      }

   if ( !str_cmp(arg, "focus") && !IS_SET( ch->special, SPC_SWORD ) )
      {
         int option;
         AFFECT_DATA af;

         if ( ( sn = skill_lookup( "concentration" ) ) < 0 )
            return;

         argument = one_argument( argument, arg );

         if ( arg[0] == '\0' )
            {
               send_to_char("#cWhich element you want to focus on?#n\n\r", ch);
               return;
            }

         if ( !str_cmp(arg, "fire") )
            option = APPLY_FIRES;
            else if ( !str_cmp(arg, "ice") )
                    option = APPLY_ICES;
            else if ( !str_cmp(arg, "thunder") )
                    option = APPLY_BOLTS;
            else if ( !str_cmp(arg, "earth") )
                    option = APPLY_EARTHS;
            else if ( !str_cmp(arg, "holy") )
                    option = APPLY_HOLYS;
            else if ( !str_cmp(arg, "dark") )
                    option = APPLY_DARKS;
            else if ( !str_cmp(arg, "lunar") )
                    option = APPLY_LUNARS;
                    else {
                            send_to_char("#cWhich element you want to focus on?#n\n\r", ch);
                            return;
                         }

         argument = one_argument( argument, arg );

         if ( arg[0] == '\0' )
            {
               send_to_char("#cWho do you want to use this on?#n\n\r", ch);
               return;
            }

         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }

         if ( is_affected( victim, sn ) )
            {
               send_to_char( "They are already focusing on an element.\n\r", ch );
               return;
            }

         ch->wait = 18;
         act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
         act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
         act("You say, '#ySol Katti!#n'",ch,NULL,NULL,TO_CHAR);
         act("$n says, '#ySol Katti!#n'",ch,NULL,NULL,TO_ROOM);
         act("#CYour #wweapons start glowing with #R#zferocious#n #opower#w!#n", victim, NULL, NULL, TO_CHAR);
         act("#C$n's #wweapons start glowing with #R#zferocious#n #opower#w!#n", victim, NULL, NULL, TO_ROOM);

         af.type      = sn;
         af.duration  = 24;
         af.modifier  = 5;
         af.location  = APPLY_WIS;
         af.bitvector = 0;
         affect_to_char( victim, &af );

         af.type      = sn;
         af.duration  = 24;
         af.modifier  = 100;
         af.location  = option;
         af.bitvector = 0;
         affect_to_char( victim, &af );
         return;
      }
}

void do_mani_katti( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int sn;

   if ( ( sn = skill_lookup( "mani katti" ) ) < 0 )
      return;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->pkill < 20 )
      {
         send_to_char( "#CYou #chaven't mastered this aspect of the #esword #cas yet #Pmagi #Cknight#c.#n\n", ch ); 
         return;
      }

   act("You say, '#yMani Katti!!#n' as you let go a very soft smile.",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yMani Katti!!#n' as $e lets go a very soft smile.",ch,NULL,NULL,TO_ROOM);
   act("Pulling out a slim blade, you raise it proudly above you.",ch,NULL,NULL,TO_CHAR);
   act("Pulling out a slim blade, $n raises it proudly above $m.",ch,NULL,NULL,TO_ROOM);
   act("A shard of light radiates off the blade as the sky darkens.",ch,NULL,NULL,TO_CHAR);
   act("A shard of light radiates off the blade as the sky darkens.",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yI will do you proud Luc#n' as it starts raining.",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yI will do you proud Luc#n' as it starts raining.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) )
         continue;

      if ( !is_same_group(ch,vch) && !( ch->pcdata->kingdom == vch->pcdata->kingdom
           && ch->pcdata->kingdom > 0 ) )
         continue;

      if ( is_affected( vch, sn ) )
         continue;

      act("As the rain splatters off your body, you feel more potent with your magical powers.",vch,NULL,NULL,TO_CHAR);
      act("As the rain splatters off $n's body, $e feels more potent with $s magical powers.",vch,NULL,NULL,TO_ROOM);

      (*skill_table[sn].spell_fun) ( sn, 10, vch, vch );
   }

   ch->wait = 18;
}

void do_icearmor( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
          send_to_char( "Huh?\n", ch ); 
          return;
      }

   if ( ch->sorcerorab[SICE] < 3 )
      {
          send_to_char( "Huh?\n", ch ); 
          return;
      }

   if ( is_affected( ch, skill_lookup("ice armor") ) )
      {
          send_to_char( "You already have ice armor on your body.\n", ch ); 
	    return;
      }

   if ( is_sword( ch ) < 3 )
      {
          send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
          return;
      }

   if ( ch->mana < 5000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 5000;
   act("You say, '#yZkl Vjni Xpli!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yZkl Vjni Xpli!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You whirl your sword high into the sky, suddenly slamming it through the ground.",ch,NULL,NULL,TO_CHAR);
   act("$n whirls $s sword high into the sky, suddenly slamming it through the ground.",ch,NULL,NULL,TO_ROOM);
   act("Focusing on the hilt of your blade, you draw out its very essence.",ch,NULL,NULL,TO_CHAR);
   act("Focusing on the hilt of $s blade, $n draws out its very essence.",ch,NULL,NULL,TO_ROOM);
   act("Soft laughter is heard as a spiritual armor hovers towards you, uniting with your soul.",ch,NULL,NULL,TO_CHAR);
   act("Soft laughter is heard as a spiritual armor hovers towards $n, uniting with $s soul.",ch,NULL,NULL,TO_ROOM);

   spell_icearmor(skill_lookup("ice armor"),10,ch,ch);
   ch->wait = 12;
}

void do_icesword( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Ice Sword on?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch, victim) )
      return;

   ch->mana -= 500000;

   act("You say, '#yMani ceraj den!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yMani ceraj den!#n'",ch,NULL,NULL,TO_ROOM);
   act("You leap high into the air, focusing your magick on your blade.",ch,NULL,NULL,TO_CHAR);
   act("$n leaps high into the air, focusing $s magick on $s blade.",ch,NULL,NULL,TO_ROOM);
   act("Redirecting the magical energies, you twirl your body as you somersault down towards $N.",ch,NULL,victim,TO_CHAR);
   act("Redirecting the magical energies, $n twirls $s body as $e somersaults down towards you!",ch,NULL,victim,TO_VICT);
   act("Redirecting the magical energies, $n twirls $s body as $e somersaults down towards $N!",ch,NULL,victim,TO_NOTVICT);
   act("Explosive light emits off $N as your blade SlIcEs through $S body.",ch,NULL,victim,TO_CHAR);
   act("Explosive light emits off you as $n's blade SlIcEs through your body.",ch,NULL,victim,TO_VICT);
   act("Explosive light emits off $N as $n's blade SlIcEs through $S body.",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) && !IS_SET( victim->more, MORE_FROZEN ) )
      {
         stc("#LThe #wexplosive #yenergy #Limmobilizes your body, preventing movement.#n\n\r",victim);
         act("#LThe #wexplosive #yenergy #Limmobilizes $n#L's body, preventing movement.#n",victim,NULL,NULL,TO_ROOM);
         victim->freeze_timer = 1;
         SET_BIT( victim->more, MORE_FROZEN );
      }

   ch->wait = 18;
   ch->ele_attack = ATTACK_ICE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = AERIAL;
   hurt_person( ch, victim, dam, TRUE );
}

void do_icewall(CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( IS_SET(ch->in_room->room_flags, ROOM_ICYSWORD) )
      {
         send_to_char( "The room is already impaled with icy swords.\n\r", ch );
         return;
      }

   if ( ch->mana < 250000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 250000;
   SET_BIT(ch->in_room->room_flags, ROOM_ICYSWORD);
   act("Lowering your sword to your side, you grin as you point your left hand towards your enemies.",ch,NULL,NULL,TO_CHAR);
   act("Lowering $s sword to $s side, $n grins as $e points $s left hand towards you.",ch,NULL,NULL,TO_ROOM);
   act("A manifest of energy swerves around your finger as you softly close you eyes.",ch,NULL,NULL,TO_CHAR);
   act("A manifest of energy swerves around $n's finger as $e softly closes $s eyes.",ch,NULL,NULL,TO_ROOM);
   act("You mutter, '#yJin skli xiz...#n'",ch,NULL,NULL,TO_CHAR);
   act("$n mutters, '#yJin skli xiz...#n'",ch,NULL,NULL,TO_ROOM);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_CHAR);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_CHAR);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_CHAR);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_CHAR);

   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_ROOM);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_ROOM);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_ROOM);
   act("The room SHakEs ViOleNTly As A hUgE IcY sWoRd SMASHES tHrOuGh aN eXiT.",ch,NULL,NULL,TO_ROOM);
   ch->wait = 6;
}

void do_iceshield( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[SICE] < 6 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 6 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( IS_SET(ch->more, MORE_WALL) )
      {
         send_to_char( "Your body is already being protected by an icy aura.\n", ch ); 
         return;
      }

   if ( ch->mana < 100000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 100000;

   act("You say, '#yAsl tij ne xnasl!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yAsl tij ne xnasl!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You softly let go of your sword as it hovers silently in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n softly lets go of $s sword as it hovers silently in the air.",ch,NULL,NULL,TO_ROOM);
   act("The sword begins to emits off a bluish aura as the light is drawn towards your body.",ch,NULL,NULL,TO_CHAR);
   act("The sword begins to emits off a bluish aura as the light is drawn towards $n's body.",ch,NULL,NULL,TO_ROOM);
   act("Finally, your body is surrounded by an freezing aura as you grab back your sword gracefully.",ch,NULL,NULL,TO_CHAR);
   act("$n's body is surrounded by an freezing aura as $e grabs back $s sword gracefully.",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->more,MORE_WALL);
   ch->wait = 12;
}

void do_freezingrain( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[SICE] < 7 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 7 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( ch->mana < 100000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 100000;

   act("You say, '#yNu tie Zbhj Cia qer Thun!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNu tie Zbhj Cia qer Thun!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
   act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
   act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, the heavens parted as a MASSIVE hail storm DeVaStAtEs THE vICiNiTY!!",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, the heavens parted as a MASSIVE hail storm DeVaStAtEs THE vICiNiTY!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group(ch,vch) && !is_safe(ch, vch) )
         {
            act("Your body takes a TrEmEdOuS BEATING as huge hail stones CrUsHeS your body!!!",vch,NULL,NULL,TO_CHAR);
            act("$n's body takes a TrEmEdOuS BEATING as huge hail stones CrUsHeS $s body!!!",vch,NULL,NULL,TO_ROOM);
            ch->ele_attack = ATTACK_ICE;

            if ( IS_CLASS( vch, CLASS_DEMON ) || IS_CLASS( vch, CLASS_WEREWOLF ) )
               hurt_person(ch,vch,60000,TRUE);
               else hurt_person(ch,vch,30000,TRUE);

            continue;
         }

      if ( !can_heal(ch,vch) )
         continue;

      act("You feel refresh as the sparkles of ice caresses your body.",vch,NULL,NULL,TO_CHAR);
      act("$n feels refresh as the sparkles of ice caresses $s body.",vch,NULL,NULL,TO_ROOM);
      vch->hit = UMIN( vch->max_hit, vch->hit + 10000 );
      update_pos(vch);
  }

  ch->wait = 18;
}

void do_frigidcut(CHAR_DATA *ch,char *argument)
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_QUICKSAND) )
      {
         stc("The ground is already encased in ice magi knight.\n\r",ch);
         return;
      }

   ch->mana -= 500000;
   act("You say, '#yLjs Xnb Asnj Qplw!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yLjs Xnb Asnj Qplw!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You grip your sword with two hands as you raise it high above your body.",ch,NULL,NULL,TO_CHAR);
   act("$n grips $s sword with two hands as $e raises it high above $s body.",ch,NULL,NULL,TO_ROOM);
   act("Crackling energy sparkles of the blade as you thrust it deeply through the ground.",ch,NULL,NULL,TO_CHAR);
   act("Crackling energy sparkles of the blade as $n thrusts it deeply through the ground.",ch,NULL,NULL,TO_ROOM);
   act("The floor beneath your feet turn blue as a slippery layer of ice flashes throughout the vicinity.",ch,NULL,NULL,TO_CHAR);
   act("The floor beneath your feet turn blue as a slippery layer of ice flashes throughout the vicinity.",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->in_room->room_flags, ROOM_QUICKSAND);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group(ch,vch) && !is_safe(ch, vch) )
         {
            if ( IS_AFFECTED(vch, AFF_FLYING) ) 
               {
                  REMOVE_BIT( vch->affected_by, AFF_FLYING );
                  vch->wait += 12;
               }
         }
   }

   ch->wait = 6;
}

void do_shiningwind( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[SICE] < 9 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 9 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( ch->ctimer[1] > 0 )
      {
         send_to_char( "You are not prepare to do another Shining Wind yet.\n", ch ); 
         return;
      }

   if ( ch->mana < 25000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 25000;
   ch->ctimer[1] = 4;

   act("You say, '#yNu Zlt vei laq Cia!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNu Zlt vei laq Cia!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
   act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
   act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
   act("A spiral of air zooms across your body before swirling towards other targets!!",ch,NULL,NULL,TO_CHAR);
   act("A spiral of air zooms across $n's body before swirling towards other targets!!",ch,NULL,NULL,TO_ROOM);

   ch->hit = UMIN( ch->max_hit, ch->hit + 50000 );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( !is_same_group(ch,vch) || !can_heal(ch,vch) )
         continue;

      act("A soft wind swerves around your tired body, rejuventing you.",vch,NULL,NULL,TO_CHAR);
      act("A soft wind swerves around $n's tired body, rejuventing $m.",vch,NULL,NULL,TO_ROOM);

      vch->hit = UMIN( vch->max_hit, vch->hit + 50000 );
      update_pos(vch);
   }

   act("Before your very eyes, the spiral of air starts spinning rapidly turning into a massive tornado!!!",ch,NULL,NULL,TO_CHAR);
   act("Before your very eyes, the spiral of air starts spinning rapidly turning into a massive tornado!!!",ch,NULL,NULL,TO_ROOM);
   act("The tornado starts SoArInG across the room out of CoNtRoL!!!",ch,NULL,NULL,TO_ROOM);
   act("The tornado starts SoArInG across the room out of CoNtRoL!!!",ch,NULL,NULL,TO_CHAR);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) ) 
         continue;

      if ( ( !IS_NPC(ch) ) && ( is_safe(ch, vch) ) )
         continue;

      stc("The mystical tornado RIPS through your defenses!!!\n\r",vch);
      stc("The mystical tornado RIPS through your defenses!!!\n\r",vch);
      act("The mystical tornado RIPS through $n defenses!!!",vch,NULL,NULL,TO_ROOM);
      act("The mystical tornado RIPS through $n defenses!!!",vch,NULL,NULL,TO_ROOM);

      ch->ele_attack = ATTACK_ICE;
      hurt_person(ch,vch,30000,TRUE);
  }

  ch->wait = 12;
}

void do_earthsword( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Earth Sword on?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch, victim) )
      return;

   ch->mana -= 500000;

   act("You say, '#yBanri Thi Serj!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yBanri Thi Serj!#n'",ch,NULL,NULL,TO_ROOM);
   act("You suddenly open your free hand paralysing $N with the gravity of the earth.",ch,NULL,victim,TO_CHAR);
   act("$n suddenly opens $s free hand paralysing you with the gravity of the earth.",ch,NULL,victim,TO_VICT);
   act("$n suddenly opens $s free hand paralysing $N with the gravity of the earth.",ch,NULL,victim,TO_NOTVICT);
   act("Redirecting the magical energies, you viciously swirl your blade across $N's body.",ch,NULL,victim,TO_CHAR);
   act("Redirecting the magical energies, $n viciously swirls $s blade across your body!",ch,NULL,victim,TO_VICT);
   act("Redirecting the magical energies, $n viciously swirls $s blade across $N's body!",ch,NULL,victim,TO_NOTVICT);
   act("Explosive light emits off you as you recoil from the vibrations.",victim,NULL,NULL,TO_CHAR);
   act("Explosive light emits off $n as $e recoils from the vibrations.",victim,NULL,NULL,TO_ROOM);

   if ( number_percent() > 89 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->mflags, MAGE_EMBRACED2 ) )
      {
         stc("#oThe explosive energy immobilizes your body, preventing movement.#n\n\r",victim);
         act("#oThe explosive energy immobilizes $n#o's body, preventing movement.#n",victim,NULL,NULL,TO_ROOM);
         victim->move = UMAX( 0, victim->move - 1000000 );
         SET_BIT( victim->mflags, MAGE_EMBRACED2 );
      }

   ch->wait = 18;
   ch->ele_attack = ATTACK_EARTH;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   hurt_person(ch,victim,dam,TRUE);
}

void do_earthwall(CHAR_DATA *ch, char *argument)
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[SEARTH] < 6 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 6 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( ch->mana < 10000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 10000;
   act("Lowering your sword to your side, you grin as you point your left hand towards your enemies.",ch,NULL,NULL,TO_CHAR);
   act("Lowering $s sword to $s side, $n grins as $e points $s left hand towards you.",ch,NULL,NULL,TO_ROOM);
   act("A manifest of energy swerves around your finger as you softly close you eyes.",ch,NULL,NULL,TO_CHAR);
   act("A manifest of energy swerves around $n's finger as $e softly closes $s eyes.",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yBanri skli xiz...!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yBanri skli xiz...!#n'",ch,NULL,NULL,TO_ROOM);
   act("The earth amongst you trembles as a barrage of rock plummets into the sky!",ch,NULL,NULL,TO_CHAR);
   act("The earth amongst you trembles as a barrage of rock plummets into the sky!",ch,NULL,NULL,TO_ROOM);
   act("The earth amongst you trembles as a barrage of rock plummets into the sky!",ch,NULL,NULL,TO_CHAR);
   act("The earth amongst you trembles as a barrage of rock plummets into the sky!",ch,NULL,NULL,TO_ROOM);
   act("The earth crashes onto the ground forming a towering wall of unpassable granite!",ch,NULL,NULL,TO_CHAR);
   act("The earth crashes onto the ground forming a towering wall of unpassable granite!",ch,NULL,NULL,TO_ROOM);
   ch->wait = 6;
}

void do_pulsar_drill( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[SEARTH] < 7 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 7 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( ch->move < 100000 )
      {
          send_to_char( "You are too tired to execute this technique.\n", ch ); 
          return;
      }
   
   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to Pulsar Drill?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch,victim) )
      return;

   act("You are surrounded by mystical energy.", ch,NULL,victim,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,victim,TO_ROOM );
   act("You say, '#yAia Banti Sho Ke!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yAia Banti Sho Ke!#n'",ch,NULL,NULL,TO_ROOM);

   act("You clutch your sword as you leap high into the air.", ch,NULL,NULL,TO_CHAR );
   act("$n clutches $s sword as $e leaps high into the air.", ch,NULL,NULL,TO_ROOM );
   act("You twist your body viciously creating a gently whirlwind around you.", ch,NULL,NULL,TO_CHAR );
   act("$n twists $s body viciously creating a gently whirlwind around $m.", ch,NULL,NULL,TO_ROOM );
   act("You yell '#ySoooooi!#n' as you drill yourself straight into $N's chest!", ch,NULL,victim,TO_CHAR );
   act("$n yells '#ySoooooi!#n' as $e drills $mself straight into your chest!", ch,NULL,victim,TO_VICT );
   act("$n yells '#ySoooooi!#n' as $e drills $mself straight into $N's chest!", ch,NULL,victim,TO_NOTVICT );
   act("Your sword flares as you flip yourself off $N at the last second.", ch,NULL,victim,TO_CHAR );
   act("$n swords flares as $e flips $mself off you at the last second.", ch,NULL,victim,TO_VICT );
   act("$n swords flares as $e flips $mself off $N at the last second.", ch,NULL,victim,TO_NOTVICT );

   victim->mana = UMAX( 0, victim->move - 75000 );
   hurt_person(ch,victim,25000,TRUE);

   act("You softly land back on your knees as you look coldly around you.", ch,NULL,NULL,TO_CHAR );
   act("$n softly lands back on $s knees as $e looks coldly around $m.", ch,NULL,NULL,TO_ROOM );
   ch->move -= 100000;
   ch->wait = 12;
}

void do_rockblast( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 750000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 1000000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 1000000;
   ch->wait = 18;

   act("You say, '#yNia Brajhi Tu Demanet!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNia Brajhi Tu Demanet!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
   act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
   act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
   act("The earth beneath your feet starts rumbling softly!!",ch,NULL,NULL,TO_CHAR);
   act("The earth beneath your feet starts rumbling softly!!",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, the ground BuRsTs wide OPEN as HUGE shards of stone ejects into the air!!!",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, the ground BuRsTs wide OPEN as HUGE shards of stone ejects into the air!!!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) ) 
        continue;

     if ( !IS_NPC(ch) && is_safe(ch, vch) )
        continue;
 
     stc("A MASSIVE shard flies through your body, spraying blood all over the place!!!\n\r",vch);
     act("A MASSIVE shard flies through $n's body, spraying blood all over the place!!!",vch,NULL,NULL,TO_ROOM);

     ch->ele_attack = ATTACK_EARTH;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = BLUNT;
     ch->attack_focus = UPPERCUT;
     hurt_person( ch, vch, dam, TRUE );
   }
}

void do_earth_grasp( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->ctimer[0] > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   if ( ch->mana < 500000 )
      {
         stc("You don't have enough magical energy to perform this technique.\n\r",ch);
         return;
      }

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to pull towards you?\n\r", ch );
         return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_area( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   if ( ch->in_room == victim->in_room )
      {
         send_to_char( "They are already near you clone.\n\r", ch );
         return;
      }

   ch->mana -= 500000;
   ch->ctimer[0] = 4;

   act("Sensing $N's frantic, you sneer as you open your free hand.",ch,NULL,victim,TO_CHAR);
   act("Sensing your frantic, $n sneers as $e opens $s free hand.",ch,NULL,victim,TO_VICT);
   act("Sensing $N's frantic, $n sneers as $e opens $s free hand.",ch,NULL,victim,TO_NOTVICT);

   act("#oSuddenly, some unseen force rips $n away from the vicinity!#n",victim,NULL,NULL,TO_ROOM);

   stop_fighting( victim, TRUE );
   char_from_room( victim );
   char_to_room( victim, ch->in_room );

   act("A strange force suddenly flings you back to where you were on the ground!",victim,NULL,NULL,TO_CHAR);
   act("A strange force suddenly flings $n back to where $e was on the ground!",victim,NULL,NULL,TO_ROOM);
   victim->wait = 18;
}

void do_moonsword( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Moon Sword on?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch, victim) )
      return;

   ch->mana -= 500000;
   act("You say, '#yMun sa Luni Ler!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yMun sa Luni Ler!#n'",ch,NULL,NULL,TO_ROOM);
   act("You drive your sword into the ground viciously as you grip the blade tightly!",ch,NULL,NULL,TO_CHAR);
   act("$n drives $s sword into the ground viciously as $e grips the blade tightly!",ch,NULL,NULL,TO_ROOM);
   act("You concentrate softly as a soft humming fills the air.",ch,NULL,NULL,TO_CHAR);
   act("$n concentrates softly as a soft humming fills the air.",ch,NULL,NULL,TO_ROOM);
   act("The air around you roars as palish energy from $N is sucked into your blade.",ch,NULL,victim,TO_CHAR);
   act("The air around $n roars as palish energy from you is sucked into $s blade.",ch,NULL,victim,TO_VICT);
   act("The air around $n roars as palish energy from $N is sucked into $s blade.",ch,NULL,victim,TO_NOTVICT);
   act("You feel weak as your lifeforce is taken from you.",victim,NULL,NULL,TO_CHAR);
   act("$n feels weaker as $s lifeforce is taken from $m.",victim,NULL,NULL,TO_ROOM);

   hit_gain( ch, 100000 );
   ch->attack_type = ATTACK_MAGICAL;
   ch->attack_var = SOUL;
   ch->attack_focus = DIRECT;
   ch->ele_attack = ATTACK_LUNAR;

   ch->wait = 18;
   hurt_person( ch, victim, dam, TRUE );
}

void do_moonslash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !is_sword( ch ) )
      return;

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
         send_to_char( "Whom do you want to strike?\n\r", ch );
         return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
      }

   if ( is_safe(ch, victim) )
      return;

   ch->wait = 4;
   subtract_stock( ch, 300 );
   SET_BIT( ch->arena, AFF2_DM );

   act( "You tilt your sword forward as you bend down and stare coldly at $N.", ch, NULL, victim, TO_CHAR );
   act( "$n tilts $s sword forward as $e bends down and stare coldly at you.", ch, NULL, victim, TO_VICT );
   act( "$n tilts $s sword forward as $e bends down and stare coldly at $N.", ch, NULL, victim, TO_NOTVICT );
   act( "You suddenly spin around as you slash across $N's body in a vicious arc!", ch, NULL, victim, TO_CHAR );
   act( "$n suddenly spins around as $e slashes across your body in a vicious arc!", ch, NULL, victim, TO_VICT );
   act( "$n suddenly spins around as $e slashes across $N's body in a vicious arc!", ch, NULL, victim, TO_NOTVICT );
   act( "As your blade sears through $N's flesh, a violent rush of air flashes across $S body!", ch, NULL, victim, TO_CHAR );
   act( "As $n's blade sears through your flesh, a violent rush of air flashes across you!", ch, NULL, victim, TO_VICT );
   act( "As $n's blade sears through $N's flesh, a violent rush of air flashes across $S body!", ch, NULL, victim, TO_NOTVICT );
   act( "You scream in agony as the vacuum rips your body apart causing you to stagger.", victim, NULL, NULL, TO_CHAR );
   act( "$n screams in agony as the vacuum rips $s body apart causing $m to stagger.", victim, NULL, NULL, TO_ROOM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = OVERHEAD;
   ch->ele_attack = ATTACK_LUNAR;

   if ( ch->ctimer[15] == 0 )
      hurt_person(ch,victim,dam,TRUE);

   if ( victim == NULL || victim->position < POS_STUNNED || ch->position < POS_STUNNED )
      return;

   act( "You rapidly flick your wrist as your sword appears to dance around $N!", ch, NULL, victim, TO_CHAR );
   act( "$n rapidly flicks $s wrist as $s sword appears to dance around you!", ch, NULL, victim, TO_VICT );
   act( "$n rapidly flicks $s wrist as $s sword appears to dance around $N!", ch, NULL, victim, TO_NOTVICT );
   act( "Vicious jolts of air SmAsHeS you all over your body making you reel in pain!", victim, NULL, NULL, TO_CHAR );
   act( "Vicious jolts of air SmAsHeS $n all over $s body making $m reel in pain!", victim, NULL, NULL, TO_ROOM );
   act( "You suddenly do a reverse spin driving your sword up $N's body as you leap into the air!", ch, NULL, victim, TO_CHAR );
   act( "$n suddenly does a reverse spin driving $s sword up your body as $e leaps into the air!", ch, NULL, victim, TO_VICT );
   act( "$n suddenly does a reverse spin driving $s sword up $N's body as $e leaps into the air!", ch, NULL, victim, TO_NOTVICT );
   act( "You flail your arms as you are suddenly blast away by another violent rush of air.", victim, NULL, NULL, TO_CHAR );
   act( "$n flails $s arms as $e is suddenly blast away by another violent rush of air.", victim, NULL, NULL, TO_ROOM );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = AERIAL;
   ch->ele_attack = ATTACK_LUNAR;

   if ( ch->ctimer[15] == 0 )
      hurt_person(ch,victim,dam,TRUE);

   if ( victim == NULL || victim->position < POS_STUNNED || ch->position < POS_STUNNED )
      return;

   act("You mutter, '#yLuni de un Asla Thi!#n' as you spin around cutting the air wildly.",ch,NULL,NULL,TO_CHAR);
   act("$n mutters, '#yLuni de un Asla Thi!#n' as $e spins around cutting the air wildly.",ch,NULL,NULL,TO_ROOM);
   act("The vicinity suddenly goes dark as an arc of light flashes throughout your weak body!",victim,NULL,NULL,TO_CHAR);
   act("The vicinity suddenly goes dark as an arc of light flashes throughout $n's weak body!",victim,NULL,NULL,TO_ROOM);
   act("The lights continue to RiP through your body as you spew blood all over the ground!",victim,NULL,NULL,TO_CHAR);
   act("The lights continue to RiP through $n's body as $e spews blood all over the ground!",victim,NULL,NULL,TO_ROOM);

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = AERIAL;
   ch->ele_attack = ATTACK_LUNAR;

   if ( ch->ctimer[15] == 0 )
      hurt_person(ch,victim,1500000,TRUE);

   act("As the light clears, you stand coldly staring into the distance.",ch,NULL,NULL,TO_CHAR);
   act("As the light clears, you see $n standing coldly staring into the distance.",ch,NULL,NULL,TO_ROOM);
}

void do_lunabeam( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam;
   int counter = 0;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 1500000 )
      {
         send_to_char("You don't have enough magical energy for this.\n\r",ch);
         return;
      }

   ch->mana -= 1500000;

   act("You clutch your sword tightly as it starts trembling in your hand.",ch,NULL,NULL,TO_CHAR);
   act("$n clutches $s sword tightly as it starts trembling in $s hand.",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yLuni no asi Biem!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yLuni no asi Biem!#n'",ch,NULL,NULL,TO_ROOM);
   act("A loud humming fills the air as your sword starts absorbing the light around you.",ch,NULL,NULL,TO_CHAR);
   act("A loud humming fills the air as $n's sword starts absorbing the light around you.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) ) 
         continue;

      if ( is_safe(ch, vch) )
         continue;

      if ( IS_NPC( vch ) && !IS_SET(vch->act, ACT_NINJA) && vch->master == NULL )
         continue;

      counter++;

      act("You feel weaker as palish energy is drawn from you into the blade!",vch,NULL,NULL,TO_CHAR);
      act("$n feels weaker as palish energy is drawn from $m into the blade!",vch,NULL,NULL,TO_ROOM);
   }

   act("You suddenly release a MaSsIvE energy beam across the vicinity!",ch,NULL,NULL,TO_CHAR);
   act("$n suddenly releases a MaSsIvE energy beam across the vicinity!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) ) 
         {
            act("You feel warmer as a wave of luna energy refreshes you.",vch,NULL,NULL,TO_CHAR);
            act("$n looks better as a wave of luna energy refreshes $m.",vch,NULL,NULL,TO_ROOM);

            hit_gain( vch, UMIN( 400000, ( counter * 80000 ) ) );
            continue;
         }

      if ( !is_safe(ch, vch) )
         {
            dam = UMIN( counter * 150000, 800000 );
            act("Your body shudders as the moon energy rips through your flesh!",vch,NULL,NULL,TO_CHAR);
            act("$n's body shudders as the moon energy rips through $s flesh!",vch,NULL,NULL,TO_ROOM);
            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_var = BEAM;
            ch->attack_focus = DIRECT;
            ch->ele_attack = ATTACK_LUNAR;
            hurt_person( ch, vch, dam, TRUE );
         }
   }

   ch->wait = 18;
}

void do_darksword( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam = 1000000;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
       {
          send_to_char( "Huh?\n", ch ); 
          return;
       }

    if ( !is_sword( ch ) )
       return;

    if ( ch->mana < 500000 )
       {
          send_to_char( "You don't have enough magical energy for that.\n\r", ch );
          return;
       }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to use Dark Sword on?\n\r", ch );
          return;
       }
    
    if ( ch->position == POS_STANDING || arg[0] != '\0' )
       {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
       }

    if ( is_safe(ch, victim) )
       return;

    ch->mana -= 500000;

    act("You say, '#yDaki Se Wui!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yDaki Se Wui!#n'",ch,NULL,NULL,TO_ROOM);
    act("You suddenly open your free hand paralysing $N with the fear of the void.",ch,NULL,victim,TO_CHAR);
    act("$n suddenly opens $s free hand paralysing you with the fear of the void.",ch,NULL,victim,TO_VICT);
    act("$n suddenly opens $s free hand paralysing $N with the fear of the void.",ch,NULL,victim,TO_NOTVICT);
    act("You grin evilly as you quickly leap above $N's head!",ch,NULL,victim,TO_CHAR);
    act("$n grins evilly as $e quickly leaps above your head!",ch,NULL,victim,TO_VICT);
    act("$n grins evilly as $e quickly leaps above $N's head!",ch,NULL,victim,TO_NOTVICT);
    act("You then SmAsH your sword down $N's body RiPpInG iT apart!",ch,NULL,victim,TO_CHAR);
    act("$n then SmAsHeS $s sword down your body RiPpInG iT apart!",ch,NULL,victim,TO_VICT);
    act("$n then SmAsHeS $s sword down $N's body RiPpInG iT apart!",ch,NULL,victim,TO_NOTVICT);

    if ( number_percent() > 80 - devil_luck( ch, victim, 5 ) && !IS_SET( victim->monkstuff, MONK_DEATH ) )
       {
          stc("#eThe dark energy drives fear into your heart, leeching your will to live.#n\n\r",victim);
          act("#eThe dark energy drives fear into $n#e's heart, leeching $s will to live.#n",victim,NULL,NULL,TO_ROOM);
          SET_BIT( victim->monkstuff, MONK_DEATH );
       }

    ch->wait = 18;
    ch->ele_attack = ATTACK_DARK;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = SLASH;
    ch->attack_focus = AERIAL;
    hurt_person( ch, victim, dam, TRUE );
}

void do_chaos_sword( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam = 900000;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
       {
          send_to_char( "Huh?\n", ch ); 
          return;
       }

    if ( !is_sword( ch ) )
       return;

    if ( ch->mana < 500000 )
       {
          send_to_char( "You don't have enough magical energy for that.\n\r", ch );
          return;
       }

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whom do you want to use Chaos Sword on?\n\r", ch );
          return;
       }
    
    if ( ch->position == POS_STANDING || arg[0] != '\0' )
       {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
       }

    if ( is_safe(ch, victim) )
       return;

    ch->mana -= 500000;

    act("You say, '#yDaki Lis Sai Ko!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yDaki Lis Sai Ko!#n'",ch,NULL,NULL,TO_ROOM);
    act("You push your palm into $N viciously as you slash across $S body.",ch,NULL,victim,TO_CHAR);
    act("$n pushes $s palm into you viciously as $e slashes across your body.",ch,NULL,victim,TO_VICT);
    act("$n pushes $s palm into $N viciously as $e slashes across $S body.",ch,NULL,victim,TO_NOTVICT);
    act("You then drive your sword over $N's head as your palm starts pulsing with demonic energy.",ch,NULL,victim,TO_CHAR);
    act("$n then drives $s sword over your head as $s palm starts pulsing with demonic energy.",ch,NULL,victim,TO_VICT);
    act("$n then drives $s sword over $N's head as $s palm starts pulsing with demonic energy.",ch,NULL,victim,TO_NOTVICT);
    act("You cackle wildly as you grasp $N's arm sending a blast of energy through $M!",ch,NULL,victim,TO_CHAR);
    act("$n cackles wildly as $e grasps your arm sending a blast of energy through you!",ch,NULL,victim,TO_VICT);
    act("$n cackles wildly as $e grasps $N's arm sending a blast of energy through $M!",ch,NULL,victim,TO_NOTVICT);

    if ( !IS_SET( victim->more, MORE_CONFUSION ) && number_percent() > 40 - devil_luck( ch, victim, 10 ) )
       SET_BIT( victim->more, MORE_CONFUSION );

    if ( !IS_SET( victim->more, MORE_PAIN ) && number_percent() > 40 - devil_luck( ch, victim, 10 ) )
       SET_BIT( victim->more, MORE_PAIN );

    if ( !IS_SET( victim->more, MORE_PINCH ) && number_percent() > 40 - devil_luck( ch, victim, 10 ) )
       SET_BIT( victim->more, MORE_PINCH );

    if ( !IS_SET( victim->more, MORE_BLIND ) && number_percent() > 40 - devil_luck( ch, victim, 10 ) )
       SET_BIT( victim->more, MORE_BLIND );
  
    ch->wait = 18;
    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = DIRECT;
    hurt_person(ch,victim,dam,TRUE);
}

void do_despairstrike( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
       {
          send_to_char( "Huh?\n", ch ); 
          return;
       }

    if ( get_stock( ch ) < 200 )
       {
          stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
          return;
       }

    if ( !is_sword( ch ) )
       return;

    one_argument( argument, arg );

    if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
       {
          send_to_char( "Whose lifeforce do you wish to disrupt?\n\r", ch );
          return;
       }
    
    if ( ch->position == POS_STANDING || arg[0] != '\0' )
       {
         if ( ( victim = get_char_room( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
	         return;
            }
       }

    if ( is_safe(ch, victim) )
       return;

    act("You say, '#yDaki zi nana Sus!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yDaki zi nana Sus!#n'",ch,NULL,NULL,TO_ROOM);
    act("You sneer as you viciously throw your sword at $N!",ch,NULL,victim,TO_CHAR);
    act("$n sneers as $e viciously throws $s sword at you!",ch,NULL,victim,TO_VICT);
    act("$n sneers as $e viciously throws $s sword at $N!",ch,NULL,victim,TO_NOTVICT);
    act("You scream as the sword rips through your body!",victim,NULL,NULL,TO_CHAR);
    act("$n screams as the sword rips through $s body!",victim,NULL,NULL,TO_ROOM);
    act("Suddenly, your body bursts into flames as visions of your death flashes by.",victim,NULL,NULL,TO_CHAR);
    act("Suddenly, $n body bursts into flames as visions of $s death flashes by.",victim,NULL,NULL,TO_ROOM);
    act("Dashing towards $N, you quickly grab your sword as you slash across $S chest!",ch,NULL,victim,TO_CHAR);
    act("Dashing towards you, $n quickly grabs $s sword as $e slashes across your chest!",ch,NULL,victim,TO_VICT);
    act("Dashing towards $N, $n quickly grabs $s sword as $e slashes across $S chest!",ch,NULL,victim,TO_NOTVICT);

    if ( IS_CLASS(victim,CLASS_DROW) && victim->ctimer[1] == 0 )
       victim->ctimer[1] = 24;

    if ( IS_CLASS(victim,CLASS_DEMON) && victim->ctimer[2] == 0 )
       victim->ctimer[2] = 24;

    if ( IS_CLASS(victim,CLASS_PHOENIX) && victim->ctimer[1] == 0 )
       victim->ctimer[1] = 24;

    if ( IS_CLASS(victim,CLASS_PHOENIX) && victim->ctimer[3] == 0 )
       victim->ctimer[3] = 24;

    if ( IS_CLASS(victim,CLASS_SHADOW) && victim->ctimer[3] == 0 )
       victim->ctimer[3] = 24;

    if ( IS_CLASS(victim,CLASS_NIGHT_BLADE) && victim->ctimer[3] == 0 )
       victim->ctimer[3] = 24;

    if ( IS_CLASS(victim,CLASS_VAMPIRE) && victim->obeah_timer == 0 )
       victim->obeah_timer = 100;

    victim->defiance_timer = 100;

    ch->wait = 12;
    subtract_stock( ch, 200 );
    SET_BIT( ch->arena, AFF2_DM );

    ch->attack_type = ATTACK_PHYSICAL;
    ch->attack_var = BLAST;
    ch->attack_focus = DIRECT;

    hurt_person( ch, victim, 2000000, TRUE );
}

void do_deathfield( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 750000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( IS_SET(ch->in_room->room_flags, ROOM_ASSASSIN) )
      {
         send_to_char( "The room is already dampen with a kiotic field.\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 1500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 1500000;
   SET_BIT(ch->in_room->room_flags, ROOM_ASSASSIN);
   ch->wait = 18;

   act("You say, '#yUn ani Daki!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yUn ani Daki!#n'",ch,NULL,NULL,TO_ROOM);
   act("You DrIvE your sword into the ground as a sly sneer appears on your face.",ch,NULL,NULL,TO_CHAR);
   act("$n DrIvEs $s sword into the ground as a sly sneer appears on $s face.",ch,NULL,NULL,TO_ROOM);
   act("Sparks of electricity fizzes about the room as a portal manifests into existence.",ch,NULL,NULL,TO_CHAR);
   act("Sparks of electricity fizzes about the room as a portal manifests into existence.",ch,NULL,NULL,TO_ROOM);
   act("Soft whispers of glee floats from the portal as you start cackling wildly.",ch,NULL,NULL,TO_CHAR);
   act("Soft whispers of glee floats from the portal as $n starts cackling wildly.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, a flash of dark energy soars across the vicinity as you tremble with rage.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, a flash of dark energy soars across the vicinity as $n trembles with rage.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) ) 
         continue;

      if ( !IS_NPC(ch) && is_safe(ch, vch) )
         continue;

      stc("The demonic energy sears your flesh as you clutch your head in sheer pain!\n\r",vch);
      act("The demonic energy sears $n's flesh as $e clutches $s head in sheer pain",vch,NULL,NULL,TO_ROOM);

      if ( IS_NPC( vch ) && number_percent() > 74 - devil_luck( ch, vch, 10 ) )
         hurt_person(ch,vch,vch->hit + 100,TRUE);
         else {
                 ch->ele_attack = ATTACK_DARK;
                 ch->attack_type = ATTACK_MAGICAL;
                 ch->attack_var = SOUL;
                 ch->attack_focus = DIRECT;
                 hurt_person(ch,vch,dam,TRUE);
              }
   }
}

void do_ravingeyes( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) )
     return;

  if ( !IS_CLASS(ch, CLASS_SORCEROR) )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( ch->sorcerorab[STHUNDER] < 2 )
     {
        stc("Huh?\n\r",ch);
        return;
     }

  if ( is_sword( ch ) < 2 )
     {
        send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
        return;
     }

  if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
     {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
        REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
        send_to_char( "Your release the electrons surrounding your eyes as they return to normal\n\r", ch );
     }
     else {
             SET_BIT(ch->act, PLR_HOLYLIGHT);
             SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
             SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
             send_to_char( "Focusing on the molecules of front of your eyes, you adjust the electrons raising your sight to incredible proportions.\n\r", ch );
          }
}

void do_cushionwhirl( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[STHUNDER] < 3 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 3 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( IS_AFFECTED(ch, AFF_FLYING) ) 
      {
         send_to_char("You gently lower yourself down onto the ground.\r\n", ch);
         act("$n gently lowers $mself down onto the ground.", ch, NULL, NULL, TO_ROOM);
         REMOVE_BIT(ch->affected_by, AFF_FLYING);
         return;
      }

   act("You say, '#yTi oi Wer!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yTi oi Wer!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You slowly start twirling your sword about your body.",ch,NULL,NULL,TO_CHAR);
   act("$n slowly starts twirling $s sword about $s body.",ch,NULL,NULL,TO_ROOM);
   act("As your twirl your blade faster, a swiral of light encircles you.",ch,NULL,NULL,TO_CHAR);
   act("As $n twirls $s blade faster, a swiral of light encircles $m.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly the air beneath your feet soars upwards levitating you in the air.",ch,NULL,NULL,TO_CHAR);
   act("Suddenly the air beneath $n's feet soars upwards levitating $m in the air.",ch,NULL,NULL,TO_ROOM);
   SET_BIT(ch->affected_by, AFF_FLYING);
}

void do_blitzsword( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Blitz Sword on?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch, victim) )
      return;

   ch->mana -= 500000;

   act("You say, '#yNij Kzna du den!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNij Kzna du den!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You scrape your sword across the ground, creating a dazzle of sparkling energy.",ch,NULL,NULL,TO_CHAR);
   act("$n scrapes $s sword across the ground, creating a dazzle of sparkling energy.",ch,NULL,NULL,TO_ROOM);
   act("Drawing the electricity with your magick, you use all of you strength to twirl your blade above your head.",ch,NULL,NULL,TO_CHAR);
   act("Drawing the electricity with $s magick, $n uses all of $s strength to twirl $s blade above $s head.",ch,NULL,NULL,TO_ROOM);
   act("You SMASH your sword downwards $N body!",ch,NULL,victim,TO_CHAR);
   act("$n SMASHES $s sword downwards your body!",ch,NULL,victim,TO_VICT);
   act("$n SMASHES $s sword downwards $N body!",ch,NULL,victim,TO_NOTVICT);
   act("$N screams loudly as the palish energy from your blade sears $S flesh!!",ch,NULL,victim,TO_CHAR);
   act("You scream loudly as the palish energy from $n's blade sears your flesh!!",ch,NULL,victim,TO_VICT);
   act("$N screams loudly as the palish energy from $n's blade sears $S flesh!!",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 90 - devil_luck( ch, victim, 5 ) && !is_martial( victim ) )
      {
         stc("#yThe palish energy ElEcTrIcUtEs YOU!#n\n\r",victim);
         act("#yThe palish energy ElEcTrIcUtEs $n#y!#n",victim,NULL,NULL,TO_ROOM);
         victim->wait = UMIN( 18, victim->wait + 12 );
      }

   ch->wait = 18;
   ch->ele_attack = ATTACK_THUNDER;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = BLAST;
   ch->attack_focus = OVERHEAD;
   hurt_person(ch,victim,dam,TRUE);   
}

void do_thunderbeam( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch, *vch_next;
   int dam = 750000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 1000000 )
      {
         send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 1000000;

   act("You say, '#yVu zaq pli Ons!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yVu zaq pli Ons!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You softly raise your sword towards your face.",ch,NULL,NULL,TO_CHAR);
   act("$n softly raises $s sword towards $s face.",ch,NULL,NULL,TO_ROOM);
   act("Focusing on your magick, your blade starts charging up with explosive energy.",ch,NULL,NULL,TO_CHAR);
   act("Focusing on $s magick, $n's blade starts charging up with explosive energy.",ch,NULL,NULL,TO_ROOM);
   act("You point your sword at your enemies as a MASSIVE beam shoots from it.",ch,NULL,NULL,TO_CHAR);
   act("$n points $s sword at $s enemies as a MASSIVE beam shoots from it.",ch,NULL,NULL,TO_ROOM);
   ch->wait = 18;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) ) 
         continue;

      if ( is_safe(ch, vch) )
         continue;

      stc("The energy beam SOARS through your body!\n\r",vch);
      act("The energy beam SOARS through $n's body!",vch,NULL,NULL,TO_ROOM);

      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BEAM;
      ch->attack_focus = DIRECT;
      hurt_person(ch,vch,dam,TRUE);
  }
}

void do_blitztouch( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( IS_SET(ch->more, MORE_BLITZ) )
      {
         act( "#yYou #wrelease the #Cspirit #wof the blade.#n", ch, NULL, NULL, TO_CHAR );
         act( "#y$n #wreleases the #Cspirit #wof the blade.#n", ch, NULL, NULL, TO_ROOM );
         REMOVE_BIT(ch->more, MORE_BLITZ);

         if ( ch->fighting == NULL )
            ch->wait = 12;

         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->move < 8000000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   act("You say, '#yMani Kjh Det!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yMani Kjh Det!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You softly release your sword as it levitates in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n softly releases $s sword as it levitates in the air.",ch,NULL,NULL,TO_ROOM);
   act("Concentrating on your magick, your body sparkles with electricity as you summon the spirit of the blade.",ch,NULL,NULL,TO_CHAR);
   act("Concentrating on $s magick, $n's body sparkles with electricity as $e summons the spirit of the blade.",ch,NULL,NULL,TO_ROOM);
   act("Your fighting style has improved.",ch,NULL,NULL,TO_CHAR);
   SET_BIT( ch->more, MORE_BLITZ );
   ch->wait = 4;
}

void do_aurawhirl( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   int count;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_SORCEROR) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( ch->sorcerorab[STHUNDER] < 7 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 7 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( ch->fighting == NULL )
      {
	   send_to_char( "You aren't in a fight.\n\r", ch );
         return;
      }

   victim = ch->fighting;

   if ( ch->move < 50000 )
      {
	   send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   if ( is_safe(ch,victim) )
      return;

   act("You say, '#yZui Rej Das!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yZui Rej Das!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You slowly start twirling your sword about your body.",ch,NULL,NULL,TO_CHAR);
   act("$n slowly starts twirling $s sword about $s body.",ch,NULL,NULL,TO_ROOM);
   act("As your twirl your blade faster, a swiral of light encircles you.",ch,NULL,NULL,TO_CHAR);
   act("As $n twirls $s blade faster, a swiral of light encircles $m.",ch,NULL,NULL,TO_ROOM);
   act("Spinning as fast as a hurricane you zOoM towards $N.",ch,NULL,victim,TO_CHAR);
   act("Spinning as fast as a hurricane $n zOoMS towards you.",ch,NULL,victim,TO_VICT);
   act("Spinning as fast as a hurricane $n zOoMS towards $N.",ch,NULL,victim,TO_NOTVICT);

   for ( count = 0; count < 20; count++ )
   {
      one_hit(ch,victim,TYPE_UNDEFINED,1);
   }

   ch->move -= 50000;
   ch->wait = 12;
}

void do_blitzstrike( CHAR_DATA *ch, char *argument )
{
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( IS_SET( ch->more3, MORE3_TURBO ) )
      {
         REMOVE_BIT( ch->more3, MORE3_TURBO );
         act( "#yYou #wrelease the #Celectricity #wsurrounding your blade.#n", ch, NULL, NULL, TO_CHAR );
         act( "#y$n #wreleases the #Celectricity #wsurrounding $s blade.#n", ch, NULL, NULL, TO_ROOM );

         if ( ch->fighting == NULL )
            ch->wait = 12;

         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->move < 8000000 )
      {
         send_to_char( "You are too tired to execute this technique.\n\r", ch );
         return;
      }

   act("You say, '#yFue Xes Kliu!!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yFue Xes Kliu!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You slowly start twirling your sword about your body.",ch,NULL,NULL,TO_CHAR);
   act("$n slowly starts twirling $s sword about $s body.",ch,NULL,NULL,TO_ROOM);
   act("As your twirl your blade faster, a swiral of light encircles you.",ch,NULL,NULL,TO_CHAR);
   act("As $n twirls $s blade faster, a swiral of light encircles $m.",ch,NULL,NULL,TO_ROOM);
   act("You quickly raise your blade above your head as a huge lightning bolt StRIkEs it.",ch,NULL,NULL,TO_CHAR);
   act("$n quickly raises $s blade above $s head as a huge lightning bolt StRIkEs it.",ch,NULL,NULL,TO_ROOM);

   SET_BIT( ch->more3, MORE3_TURBO );
   ch->wait = 4;
}

void do_firesword( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int dam = 1000000;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 500000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Whom do you want to use Fire Sword on?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch, victim) )
      return;

   ch->mana -= 500000;

   act("You say, '#yBlaz Vu Tiy Gef!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yBlaz Vu Tiy Gef!!#n'",ch,NULL,NULL,TO_ROOM);
   act("Focusing your magick towards the tip of your blade, you dash towards $N.",ch,NULL,victim,TO_CHAR);
   act("Focusing $s magick towards the tip of $s blade, $n dashes towards you.",ch,NULL,victim,TO_VICT);
   act("Focusing $s magick towards the tip of $s blade, $n dashes towards $N.",ch,NULL,victim,TO_NOTVICT);
   act("You whirl your blade upwards, RiPpInG through $N's body as you leap into the air.",ch,NULL,victim,TO_CHAR);
   act("$n whirls $s blade upwards, RiPpInG through your body as $e leaps into the air.",ch,NULL,victim,TO_VICT);
   act("$n whirls $s blade upwards, RiPpInG through $N's body as $e leaps into the air.",ch,NULL,victim,TO_NOTVICT);

   if ( number_percent() > 84 - devil_luck( ch, victim, 5 ) )
      {
         stc("#RThe heat from the searing blade sets you on fire!#n\n\r",victim);
         act("#RThe heat from the searing blade sets $n #Ron fire!#n",victim,NULL,NULL,TO_ROOM);
         dam += 150000;

         if ( !IS_AFFECTED(victim, AFF_FLAMING) )
            SET_BIT(victim->affected_by, AFF_FLAMING);
      }

   ch->wait = 18;
   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = UPPERCUT;
   hurt_person(ch,victim,dam,TRUE);
}

void do_quadraslice( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 750000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }
   
   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to Quadra Slice?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch,victim) )
      return;

   act("You are surrounded by mystical energy.", ch,NULL,victim,TO_CHAR );
   act("$n is surrounded by mystical energy.", ch,NULL,victim,TO_ROOM );
   act("You say, '#yNiz Des wei Jiy!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNiz Des wei Jiy!!#n'",ch,NULL,NULL,TO_ROOM);
   act("You rush to $N as sparkling energy converges on your sword.", ch,NULL,victim,TO_CHAR );
   act("$n rushes to you as sparkling energy converges on $s sword.", ch,NULL,victim,TO_VICT );
   act("$n rushes to $N as sparkling energy converges on $s sword.", ch,NULL,victim,TO_NOTVICT );
   act("You strike $N viciously as you blur behind $S unguarded back.", ch,NULL,victim,TO_CHAR );
   act("$n strikes you viciously as $e blurs behind your unguarded back.", ch,NULL,victim,TO_VICT );
   act("$n strikes $N viciously as $e blurs behind $S unguarded back.", ch,NULL,victim,TO_NOTVICT );
   act("Suddenly, your body splits into images as you circle around $N striking $M hard.", ch,NULL,victim,TO_CHAR );
   act("Suddenly, $n body splits into images as $e circles around you striking hard.", ch,NULL,victim,TO_VICT );
   act("Suddenly, $n body splits into images as $e circles around $N striking $M hard.", ch,NULL,victim,TO_NOTVICT );
   act("Leaping towards $N's body, your twirl backwards slicing your sword through $M swiftly.", ch,NULL,victim,TO_CHAR );
   act("Leaping towards your body, $n twirls backwards slicing $s sword through you swiftly.", ch,NULL,victim,TO_VICT );
   act("Leaping towards $N's body, $n twirls backwards slicing $s sword through $M swiftly.", ch,NULL,victim,TO_NOTVICT );

   ch->mana -= 750000;
   ch->wait = 18;
   victim->mana = UMAX( 0, victim->mana - 1000000 );

   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;
   hurt_person( ch, victim, 850000, TRUE );
}

void do_magiwall( CHAR_DATA *ch, char *argument )
{
   int sn;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ( sn = skill_lookup( "magiwall" ) ) < 0 ) return;

   if ( is_affected(ch,skill_lookup("magiwall") ) )
      {
         stc("You are already surrounded by a magical shield!\n\r",ch);
         return;
      }

   if ( ch->ctimer[9] < 200 )
      {
         send_to_char("Your anger isn't sufficient enough to execute this technique.\n\r",ch);
         return;
      }

   if ( ch->mana < 500000 )
      {
         send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   (*skill_table[sn].spell_fun) ( sn, 3, ch, ch );
   WAIT_STATE( ch, 12 );
   ch->mana -= 500000;
   ch->ctimer[9] -= 200;
}

void do_ragedash( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   if ( !is_sword( ch ) )
      return;

   one_argument( argument, arg );

   if ( ( ( victim = ch->fighting ) == NULL ) && ( arg[0] == '\0' ) )
      {
	   send_to_char( "Who do you want to Rage Dash on?\n\r", ch );
	   return;
      }
    
   if ( ch->position == POS_STANDING || arg[0] != '\0' )
      {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
           {
              send_to_char( "They aren't here.\n\r", ch );
	        return;
           }
      }

   if ( is_safe(ch,victim) )
      return;

   act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
   act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
   act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yNiz Des wei Jiy!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yNiz Des wei Jiy!!#n'",ch,NULL,NULL,TO_ROOM);
   act("As the explosive energy dances on your hand, you grab hold of your blade and FlAsH thrOuGh $N!!!",ch,NULL,victim,TO_CHAR);
   act("As the explosive energy dances on $n's hand, $e grabs hold of $s blade and FlAsHEs thrOuGh YOU!!!",ch,NULL,victim,TO_VICT);
   act("As the explosive energy dances on $n's hand, $e grabs hold of $s blade and FlAsHeS thrOuGh $N!!!",ch,NULL,victim,TO_NOTVICT);

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   ch->ele_attack = ATTACK_FIRE;
   ch->attack_type = ATTACK_PHYSICAL;
   ch->attack_var = SLASH;
   ch->attack_focus = DIRECT;

   hurt_person( ch, victim, 1000000, TRUE );
}

void do_dancingflames( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam = 750000;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) || IS_SET( ch->special, SPC_SWORD ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !is_sword( ch ) )
      return;

   if ( ch->mana < 1000000 )
      {
	   send_to_char( "You don't have enough magical energy for that.\n\r", ch );
         return;
      }

   ch->mana -= 1000000;
   ch->wait = 18;

   act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
   act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
   act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
   act("You say, '#yBlaz Hu Boim!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yBlaz Hu Boim!!#n'",ch,NULL,NULL,TO_ROOM);
   act("$n's hands glows #rblazing red#n as $e leans forward and point one of them at you.",ch,NULL,NULL,TO_ROOM);
   act("Your hands glows #rblazing red#n as you lean forward and point one of them at centre of the room.",ch,NULL,NULL,TO_CHAR);
   act("A HUGE fireball SmAsHeS into the area, FiLlInG the room with flames.",ch,NULL,NULL,TO_ROOM);
   act("A HUGE fireball SmAsHeS into the area, FiLlInG the room with flames.",ch,NULL,NULL,TO_CHAR);
 
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( is_same_group(ch,vch) ) 
         continue;

      if ( ( !is_safe(ch, vch) ) && ( vch != ch ) )
         {
            act("You stagger as the fireball SwEePs you off YOUR fEeT!!!",vch,NULL,NULL,TO_CHAR);
            act("$n staggers as the fireball SwEePs $m OFF $s FeET!!!.",vch,NULL,NULL,TO_ROOM);
       
            if ( !IS_AFFECTED(vch, AFF_FLAMING) && number_percent() > 80 ) 
               SET_BIT(vch->affected_by, AFF_FLAMING);

            ch->ele_attack = ATTACK_FIRE;
            ch->attack_type = ATTACK_MAGICAL;
            ch->attack_var = BLAST;
            ch->attack_focus = AERIAL;
            hurt_person(ch,vch,dam,TRUE);
        }
   }
}

void do_finalflame( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam = 50000;

   if ( !IS_CLASS(ch, CLASS_SORCEROR) )
      {
         send_to_char("Huh?\n\r",ch);
	   return;
      }

   if ( ch->sorcerorab[SFIRE] < 9 )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( is_sword( ch ) < 9 )
      {
         send_to_char( "This graceful ability is too difficult for your sword to handle.\n", ch ); 
         return;
      }

   if ( ch->ctimer[3] > 0 )
      {
         send_to_char( "You are not prepare to do another Final Flame yet.\n", ch ); 
         return;
      }

   if ( ch->mana < 50000 )
      {
         send_to_char("You don't have enough magical energy for that.\n\r",ch);
         return;
      }

   ch->ctimer[3] = 6;
   ch->mana -= 50000;
   ch->wait = 12;

   act("You lower your sword as you raise your free hand in the air.",ch,NULL,NULL,TO_CHAR);
   act("$n lowers $s sword as $e raises $s free hand in the air.",ch,NULL,NULL,TO_ROOM);
   act("Focusing a manifest of energy on your hand, you grin as you concentrate viciously.",ch,NULL,NULL,TO_CHAR);
   act("Focusing a manifest of energy on $s hand, $n grins as $e concentrate viciously.",ch,NULL,NULL,TO_ROOM);
   act("You point a hand towards your enemies as you close your eyes.",ch,NULL,NULL,TO_CHAR);
   act("$n points a hand towards you as $e closes $s eyes.",ch,NULL,NULL,TO_VICT);
   act("You say, '#yDi Finel Blaz!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yDi Finel Blaz!!#n'",ch,NULL,NULL,TO_ROOM);
   act("The sky turns blazing red as MaSsIvE fire BALLS erupts from the heavens.",ch,NULL,NULL,TO_ROOM);
   act("The sky turns blazing red as MaSsIvE fire BALLS erupts from the heavens.",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_ROOM);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
   act("The ground EXPLODES as a massive fire ball CrAsHeS through IT!!!",ch,NULL,NULL,TO_CHAR);
 
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;
       
      if ( is_same_group(ch,vch) ) 
         continue;

      if ( ( !is_safe(ch, vch) ) )
         {
            if ( !IS_AFFECTED(vch, AFF_FLAMING) ) 
               SET_BIT(vch->affected_by, AFF_FLAMING);

            if ( IS_CLASS( vch, CLASS_DROW ) || IS_CLASS( vch, CLASS_VAMPIRE ) )
               hurt_person(ch,vch,dam * 2,TRUE);
               else hurt_person(ch,vch,dam,TRUE);
         }
   }
}

void do_encase(CHAR_DATA *ch, char *argument)
{
   OBJ_DATA *obj;
   char arg[MAX_INPUT_LENGTH];
   int count = 0;

   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS( ch, CLASS_ASSASSIN ) || !IS_SET( ch->special, SPC_SOR ) )
      if ( !IS_SET( ch->cmbt[7], SKL_ENCASE ) )
      {
         send_to_char( "Huh?\n", ch ); 
         return;
      }

   if ( !IS_SET( ch->cmbt[7], SKL_ENCASE ) && !is_sword( ch ) )
      return;

   if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ) )
      if ( ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) )
         {
            send_to_char( "You need a weapon to utilize this technique.\n\r", ch );
            return;
         }

   argument = one_argument( argument, arg );

   if ( ch->mana < 500000 )
      {
         send_to_char( "You don't have enough magical energy.\n\r", ch );
         return;
      }

   if ( arg[0] == '\0' )
      {
         send_to_char("What spell do you wish to encase on your weapon?\n\r", ch);
         return;
      }

   if ( !str_cmp(arg, "none") )
      {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("The energy around your weapon fades away...", ch, NULL, NULL, TO_CHAR);
          act("The energy around $n's weapon fades away...", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);
          }
      }
      else if ( !str_cmp(arg, "fire") )
      {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yBlaz Trui Zeri Thun!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yBlaz Trui Zeri Thun!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon bursts into flames as ferocious energy emits of it.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon bursts into flames as ferocious energy emits of it.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);
          }
       }
       else if ( !str_cmp(arg, "ice") )
       {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yLjs Xnb Trui Nai Bjr!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yLjs Xnb Trui Nai Bjr!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon turns into ice as frigid air flows from it.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon turns into ice as frigid air flows from it.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);
          }
       }
       else if ( !str_cmp(arg, "moon") )
       {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yLuni Arrr Suuch!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yLuni Arrr Suuch!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon glows bright yellow as it shimmers with moon energy.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon glows bright yellow as it shimmers with moon energy.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);
          }
       }
       else if ( !str_cmp(arg, "dark") )
       {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yDei Vinas Tu Pinn!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yDei Vinas Tu Pinn!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon glows dark as a loud humming fills the air.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon glows dark as a loud humming fills the air.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_DARK);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);
          }
       }
       else if ( !str_cmp(arg, "saint") )
       {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yNu Holi Ses un Pinn!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yNu Holi Ses un Pinn!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon glows bright as a loud humming fills the air.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon glows bright as a loud humming fills the air.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_SAINT);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);
          }
       }
       else if ( !str_cmp(arg, "thunder") )
       {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yNij Kzna Trui Eil!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yNij Kzna Trui Eil!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon turns into a massive lightning bolt as sparks fizzes out of it.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon turns into a massive lightning bolt as sparks fizzes out of it.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_EARTH) )
                REMOVE_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);
          }
       }
       else if ( !str_cmp(arg, "earth") )
       {
          act("You lean forward as you raise your weapon high above you.", ch, NULL, NULL, TO_CHAR);
          act("$n leans forward as $e raises $s weapon high above $m.", ch, NULL, NULL, TO_ROOM);
          act("You say, '#yQwe Trui Vjh Saz!#n'",ch,NULL,NULL,TO_CHAR);
          act("$n says, '#yQwe Trui Vjh Saz!#n'",ch,NULL,NULL,TO_ROOM);
          act("Your weapon turns into stone as small rubbles fall to the ground.", ch, NULL, NULL, TO_CHAR);
          act("$n's weapon turns into stone as small rubbles fall to the ground.", ch, NULL, NULL, TO_ROOM);

          WAIT_STATE(ch, 4);
          ch->mana -= 500000;

          while( count < 4 )
          {
             switch( count )
             {
                default:
                case 0: obj = get_eq_char( ch, WEAR_WIELD ); break;
                case 1: obj = get_eq_char( ch, WEAR_HOLD ); break;
                case 2: obj = get_eq_char( ch, WEAR_THIRD ); break;
                case 3: obj = get_eq_char( ch, WEAR_FOURTH ); break;
             }

             count++;

             if ( obj == NULL )
                continue;

             if ( IS_SET( obj->weapflags, WEAPON_NOENCASE ) )
                continue;

             SET_BIT(obj->weapflags, TRUE_EARTH);

             if ( IS_SET(obj->weapflags, TRUE_BOLT) )
                REMOVE_BIT(obj->weapflags, TRUE_BOLT);

             if ( IS_SET(obj->weapflags, TRUE_FIRE) )
                REMOVE_BIT(obj->weapflags, TRUE_FIRE);

             if ( IS_SET(obj->weapflags, TRUE_DARK) )
                REMOVE_BIT(obj->weapflags, TRUE_DARK);

             if ( IS_SET(obj->weapflags, TRUE_ICE) )
                REMOVE_BIT(obj->weapflags, TRUE_ICE);

             if ( IS_SET(obj->weapflags, TRUE_MOON) )
                REMOVE_BIT(obj->weapflags, TRUE_MOON);

             if ( IS_SET(obj->weapflags, TRUE_SAINT) )
                REMOVE_BIT(obj->weapflags, TRUE_SAINT);
          }
       }
       else stc("Syntax - #wEncase #gFire/Ice/Thunder/Earth/Dark/Saint/Moon/None#n\n\r",ch);
}

