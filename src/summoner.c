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

void do_g_zantetsuken( CHAR_DATA *ch )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yZantetsuken...#n'",ch,NULL,NULL,TO_ROOM);
  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yZantetsuken...#n'",ch,NULL,NULL,TO_CHAR);

  act("The AIR ROARS as Gilgamesh grabs Zantetsuken with his free hand and slashes upwards to the right.",ch,NULL,NULL,TO_CHAR);
  act("The AIR ROARS as Gilgamesh grabs Zantetsuken with his free hand and slashes upwards to the right.",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( IS_IMMORTAL(vch) )
        continue; 

     if ( ch == vch )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     if ( !IS_SET(vch->more, MORE_BLIND) )
        {
           SET_BIT(vch->more, MORE_BLIND);
           act("You screams in agony as a beam of blinding light tears through your body, cutting the air in half.",vch,NULL,NULL,TO_CHAR);
           act("$n screams in agony as a beam of blinding light tears through $s body, cutting the air in half.",vch,NULL,NULL,TO_ROOM);
        }

     act("A blazing tornado RAZES through the room and RIPS $n's body like a doll into the sky.",vch,NULL,NULL,TO_ROOM);
     act("A blazing tornado RAZES through the room and RIPS your body like a doll into the sky.",vch,NULL,NULL,TO_CHAR);

     stop_fighting( vch, TRUE );
     vch->air_timer = number_range( 5, 10 );
     vch->hit = -2000010;
     vch->position = POS_MORTAL;
  }

  act("As pieces of his opponents falls to the ground, Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_ROOM);
  act("As pieces of his opponents falls to the ground, Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_CHAR);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_CHAR);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_ROOM);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_CHAR);
}

void do_masamune( CHAR_DATA *ch )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yMasamune...#n'",ch,NULL,NULL,TO_ROOM);
  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yMasamune...#n'",ch,NULL,NULL,TO_CHAR);

  act("Palish energy blurs through the air as Gilgamesh grabs Masamune and slashes rapidly in front of $m.",ch,NULL,NULL,TO_ROOM);
  act("Palish energy blurs through the air as Gilgamesh grabs Masamune and slashes rapidly in front of $m.",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( IS_IMMORTAL(vch) )
        continue; 

     if ( ch == vch )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     if ( !IS_AFFECTED(vch, AFF_FLAMING) ) 
        SET_BIT(vch->affected_by, AFF_FLAMING);

     act("$n roars as the palish energy DeMoLiSheS $s bOdY!!!",vch,NULL,NULL,TO_ROOM);
     act("You roar as the palish energy DeMoLiSheS yOuR bOdY!!!",vch,NULL,NULL,TO_CHAR);

     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = SLASH;
     ch->attack_focus = UPPERCUT;
     hurt_person(ch,vch,2500000,TRUE);
  }

  act("Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_CHAR);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_CHAR);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_ROOM);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_CHAR);
}

void do_excalibur( CHAR_DATA *ch )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yExcalibur...#n'",ch,NULL,NULL,TO_ROOM);
  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yExcalibur...#n'",ch,NULL,NULL,TO_CHAR);

  act("Grabbing Excalibur, Gilgamesh SlAsHes to the left with ALL of his MiGhT!!!",ch,NULL,NULL,TO_ROOM);
  act("Grabbing Excalibur, Gilgamesh SlAsHes to the left with ALL of his MiGhT!!!",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( IS_IMMORTAL(vch) )
        continue; 

     if ( ch == vch )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     act("$n's body nearly got CuT iN hALf as Excalibur slices through it like butter!!!",vch,NULL,NULL,TO_ROOM);
     act("Your body nearly got CuT iN hALf as Excalibur slices through it like butter!!!",vch,NULL,NULL,TO_CHAR);

     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = SLASH;
     ch->attack_focus = DIRECT;
     hurt_person(ch,vch,1500000,TRUE);
  }

  act("Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_CHAR);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_CHAR);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_ROOM);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_CHAR);
}

void do_excalipur( CHAR_DATA *ch )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yExcalipur...#n'",ch,NULL,NULL,TO_ROOM);
  act("\n\rGilgamesh's eyes glow with demonic energy as he mutters '#yExcalipur...#n'",ch,NULL,NULL,TO_CHAR);

  act("Grabbing Excalipur, Gilgamesh SlAsHes to the right with ALL of his MiGhT!!!",ch,NULL,NULL,TO_ROOM);
  act("Grabbing Excalipur, Gilgamesh SlAsHes to the right with ALL of his MiGhT!!!",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( IS_IMMORTAL(vch) )
        continue; 

     if ( is_safe(ch, vch) )
        continue;

     act("$n laugh as Excalipur tickles $s body.",vch,NULL,NULL,TO_ROOM);
     act("Your laugh loudly as Excalipur bounces off your body... is this a joke?",vch,NULL,NULL,TO_CHAR);

     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = SLASH;
     ch->attack_focus = DIRECT;
     hurt_person(ch,vch,1,TRUE);
  }

  act("Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh's eyes glow blazing red as he coldly looks around him.",ch,NULL,NULL,TO_CHAR);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh mutters '#yThis isn't over yet... I must find the interdimensional portal...#n'",ch,NULL,NULL,TO_CHAR);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_ROOM);
  act("You breathe a sigh of relief as Gilgamesh walks into the distance, shimmering out of existence.",ch,NULL,NULL,TO_CHAR);
}

void do_tyrael( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( IS_NPC(ch) )
     return;

  ch->achi = 40;

  act("The heavens suddenly parted as bright sunlight shines in your face.",ch,NULL,NULL,TO_CHAR);
  act("The heavens suddenly parted as bright sunlight shines in your face.",ch,NULL,NULL,TO_ROOM);
  act("A gentle beam shines before you as a winged figure hovers above.",ch,NULL,NULL,TO_CHAR);
  act("A gentle beam shines before you as a winged figure hovers above.",ch,NULL,NULL,TO_ROOM);
  act("The angel then sings '#yI am Tyrael... the saviour of mankind.#n'",ch,NULL,NULL,TO_CHAR);
  act("The angel then sings '#yI am Tyrael... the saviour of mankind.#n'",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !is_same_group(ch,vch) )
        continue;

     if ( vch != ch && nogang != 0 )
        continue;

     act("You mysteriously feel refresh as Tyrael points a glowing finger at you.",vch,NULL,NULL,TO_CHAR);
     act("$n mysteriously feels refresh as Tyrael points a glowing finger at $m.",vch,NULL,NULL,TO_ROOM);
     hit_gain( vch, 1500000 );
  }

  act("Tyrael's eyes burn with holious fury as he unsheathes his sword.",ch,NULL,NULL,TO_CHAR);
  act("Tyrael's eyes burn with holious fury as he unsheathes his sword.",ch,NULL,NULL,TO_ROOM);
  act("He then continues '#y... and the Angel of Death!#n'",ch,NULL,NULL,TO_CHAR);
  act("He then continues '#y... and the Angel of Death!#n'",ch,NULL,NULL,TO_ROOM);
  act("Before anyone can recover, he RiPs the very air apart with a MASSIVE swing!!!",ch,NULL,NULL,TO_CHAR);
  act("Before anyone can recover, he RiPs the very air apart with a MASSIVE swing!!!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_safe(ch,vch) )
        continue;

     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = SLASH;
     ch->attack_focus = DIRECT;
     hurt_person( ch, vch, 1500000, TRUE );
  }
}

void do_mywind( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( IS_NPC(ch) )
     return;

  ch->achi = 40;

  act("You spread your arms apart as you form an energy blade.",ch,NULL,NULL,TO_CHAR);
  act("$n spreads $s arms apart as $e forms an energy blade.",ch,NULL,NULL,TO_ROOM);
  act("You snarl '#yMay you strike from above!#n' as you throw it in the air.",ch,NULL,NULL,TO_CHAR);
  act("$n snarls '#yMay you strike from above!#n' as $e throws it in the air.",ch,NULL,NULL,TO_ROOM);
  act("A powerful greenish aura radiates off you as your blade hums loudly.",ch,NULL,NULL,TO_CHAR);
  act("A powerful greenish aura radiates off $n as $s blade hums loudly.",ch,NULL,NULL,TO_ROOM);
  act("You then catch the blade beautifully as you point it into the skies.",ch,NULL,NULL,TO_CHAR);
  act("$n then catches the blade beautifully as $e points it into the skies.",ch,NULL,NULL,TO_ROOM);
  act("You scream '#yGo for it BIG eye!!!#n' as you point your blade at your foe!",ch,NULL,NULL,TO_CHAR);
  act("$n screams '#yGo for it BIG eye!!!#n' as $e points $s blade at you!",ch,NULL,NULL,TO_ROOM);
  act("A sparkle catches your eyes as you see a flash of light in the distance.",ch,NULL,NULL,TO_CHAR);
  act("A sparkle catches your eyes as you see a flash of light in the distance.",ch,NULL,NULL,TO_ROOM);
  act("A strong torrent of wind send you onto the floor as a MASSIVe airship flies by you.",ch,NULL,NULL,TO_CHAR);
  act("A strong torrent of wind send you onto the floor as a MASSIVe airship flies by you.",ch,NULL,NULL,TO_ROOM);
  act("Looking up, you see a pissed off Cid grinning down at you.",ch,NULL,NULL,TO_CHAR);
  act("Looking up, you see a pissed off Cid grinning down at you.",ch,NULL,NULL,TO_ROOM);
  act("The Highwind beam cannon starts charging ViOlEnTlY as you cover your ears in pain!",ch,NULL,NULL,TO_CHAR);
  act("The Highwind beam cannon starts charging ViOlEnTlY as you cover your ears in pain!",ch,NULL,NULL,TO_ROOM);
  act("Cid sneers '#yEat this Motherf%*&^((#n' as he release MASSIVE beams into the vicinity!!!",ch,NULL,NULL,TO_CHAR);
  act("Cid sneers '#yEat this Motherf%*&^((#n' as he release MASSIVE beams into the vicinity!!!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_safe(ch,vch) )
        continue;

     act("You scream in agony as you are BlAsT into oblivion!!!",vch,NULL,NULL,TO_CHAR);
     act("$n screams in agony as $e is BlAsT into oblivion!!!",vch,NULL,NULL,TO_ROOM);

     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = BEAM;
     ch->attack_focus = AERIAL;
     hurt_person( ch, vch, 3000000, TRUE );

     if ( vch == NULL || vch->in_room == NULL )
        continue;

     vch->air_timer = number_range( 2, 3 );
     stop_fighting( vch, TRUE );
  }
}

void do_lancelot( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( IS_NPC(ch) )
     return;

  ch->achi = 40;

  act("The heavens fill the room with holy matrimony as brilliant light flashes through the vicinity.",ch,NULL,NULL,TO_CHAR);
  act("The heavens fill the room with holy matrimony as brilliant light flashes through the vicinity.",ch,NULL,NULL,TO_ROOM);
  act("Admist the centre of this dazzling light, a divine figure awaits.",ch,NULL,NULL,TO_CHAR);
  act("Admist the centre of this dazzling light, a divine figure awaits.",ch,NULL,NULL,TO_ROOM);
  act("The figure on it lowers its lance as it scans the area.",ch,NULL,NULL,TO_CHAR);
  act("The figure on it lowers its lance as it scans the area.",ch,NULL,NULL,TO_ROOM);
  act("A horse whines loudly as the figure draws back the reins, raising its lance in the air.",ch,NULL,NULL,TO_CHAR);
  act("A horse whines loudly as the figure draws back the reins, raising its lance in the air.",ch,NULL,NULL,TO_ROOM);
  act("Someone shouts '#yHalt!!#n' as it rushes straight into its target!",ch,NULL,NULL,TO_CHAR);
  act("Someone shouts '#yHalt!!#n' as it rushes straight into its target!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_safe(ch,vch) )
        continue;

     act("You get struck by a HuGe lightning BOLT as the figure's spear is RAMMED through you!",vch,NULL,NULL,TO_CHAR);
     act("$n gets struck by a HuGe lightning BOLT as the figure's spear is RAMMED through $m!",vch,NULL,NULL,TO_ROOM);

     vch->wait = UMIN( 18, vch->wait + 12 );

     ch->attack_type = ATTACK_PHYSICAL;
     ch->ele_attack = ATTACK_THUNDER;
     ch->attack_var = PIERCE;
     ch->attack_focus = DIRECT;
     hurt_person( ch, vch, 2000000, TRUE );
  }

  act("The figure dashes into the heavens, fading out of sight.",ch,NULL,NULL,TO_CHAR);
  act("The figure dashes into the heavens, fading out of sight.",ch,NULL,NULL,TO_ROOM);
}

void do_gilgamesh( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  ch->achi = 40;

  act("The sky turns dark as thunderbolts devastate the area, heavy rain clouding your vision.",ch,NULL,NULL,TO_CHAR);
  act("The sky turns dark as thunderbolts devastate the area, heavy rain clouding your vision.",ch,NULL,NULL,TO_ROOM);
  act("As the lightning blinds you, multiple sparkles appear from the heavens.",ch,NULL,NULL,TO_CHAR);
  act("As the lightning blinds you, multiple sparkles appear from the heavens.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, the ground ShAkEs ViOlEnTlY as four legendary sword SmAsHeS into the vicinity around you.",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, the ground ShAkEs ViOlEnTlY as four legendary sword SmAsHeS into the vicinity around you.",ch,NULL,NULL,TO_ROOM);
  act("A manifest of electricity hovers between the swords as a dark form fades into existence.",ch,NULL,NULL,TO_CHAR);
  act("A manifest of electricity hovers between the swords as a dark form fades into existence.",ch,NULL,NULL,TO_ROOM);
  act("A dark robed figure slowly raises his head staring straight into your soul...",ch,NULL,NULL,TO_ROOM);
  act("A dark robed figure slowly raises his head staring straight into your soul...",ch,NULL,NULL,TO_CHAR);

  act("\n\rGilgamesh mutters '#yVile Creatures, Sinners of Hell...#n'\n\r",ch,NULL,NULL,TO_ROOM);
  act("\n\rGilgamesh mutters '#yVile Creatures, Sinners of Hell...#n'\n\r",ch,NULL,NULL,TO_CHAR);
  act("Gilgamesh exclaims '#yFace this blade!#n'",ch,NULL,NULL,TO_ROOM);
  act("Gilgamesh exclaims '#yFace this blade!#n'",ch,NULL,NULL,TO_CHAR);

  if ( number_percent() > 89 )
     {
        do_g_zantetsuken(ch);
        return;
     }

  switch( number_range(1,3) )
  {
     case 2: do_excalibur(ch);
             break;
     case 3: do_masamune(ch);
             break;
     case 1: do_excalipur(ch);
             break;
  }
}

void do_l_lightblade( CHAR_DATA *ch )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   AFFECT_DATA af;
   int sn = skill_lookup( "last prayer" );

   act("Lloth suddenly grimaces as flashes of her skull laughs at you.",ch,NULL,NULL,TO_CHAR);
   act("Lloth suddenly grimaces as flashes of her skull laughs at you.",ch,NULL,NULL,TO_ROOM);
   act("Lloth spreads her arms as she cackles '#eAll of you shall suffer!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("Lloth spreads her arms as she cackles '#eAll of you shall suffer!!!#n'",ch,NULL,NULL,TO_ROOM);
   act("A menacing blade of light suddenly manifests into Lloth's hand.",ch,NULL,NULL,TO_CHAR);
   act("A menacing blade of light suddenly manifests into Lloth's hand.",ch,NULL,NULL,TO_ROOM);
   act("She then start dashing towards you as her body split into unlimited images!",ch,NULL,NULL,TO_CHAR);
   act("She then start dashing towards you as her body split into unlimited images!",ch,NULL,NULL,TO_ROOM);
   act("A loud voice booms '#eThis is the Wrath of the Heavens!#n'",ch,NULL,NULL,TO_CHAR);
   act("A loud voice booms '#eThis is the Wrath of the Heavens!#n'",ch,NULL,NULL,TO_ROOM);
   act("A VIOLENT flash of light BLASTS everyone away as the earth ROARS for vengeance.",ch,NULL,NULL,TO_CHAR);
   act("A VIOLENT flash of light BLASTS everyone away as the earth ROARS for vengeance.",ch,NULL,NULL,TO_ROOM);
   act("The voice continues '#eRepent, EAT THIS, and DIE!!!!#n'",ch,NULL,NULL,TO_CHAR);
   act("The voice continues '#eRepent, EAT THIS, and DIE!!!!#n'",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) )
         continue;

      if ( IS_IMMORTAL(vch) )
         continue; 

      if ( is_safe(ch, vch) )
         continue;

      af.type = sn;
      af.location = APPLY_DEX;
      af.modifier = -4;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      af.type = sn;
      af.location = APPLY_INT;
      af.modifier = -4;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      af.type = sn;
      af.location = APPLY_STR;
      af.modifier = -4;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      af.type = sn;
      af.location = APPLY_CON;
      af.modifier = -4;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      af.type = sn;
      af.location = APPLY_WIS;
      af.modifier = -4;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      af.type = skill_lookup( "lolth charm" );
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      af.type = skill_lookup("vision destroyer");
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.duration = 10;
      af.bitvector = 0;
      affect_to_char(vch, &af);

      SET_BIT( vch->monkstuff, MONK_DEATH );
      vch->noprotection = 2;

      act("An image of Lloth leaps in front of you while grinning evilly.",vch,NULL,NULL,TO_CHAR);
      act("It then slashes you savagely across the chest ripping it apart!",vch,NULL,NULL,TO_CHAR);
      act("It twirl its blade up your body sending you into the air.",vch,NULL,NULL,TO_CHAR);
      act("Lloth's features flickers into a skeletonly mask as it leap with you.",vch,NULL,NULL,TO_CHAR);
      act("The deadly blade of hers pulses with demonic energy as she slams you on the ground.",vch,NULL,NULL,TO_CHAR);
      act("A voice screams '#eHui!#n' as the image SLASHES through you like BUTTER!!!",vch,NULL,NULL,TO_CHAR);

      act("#eAn image of #PLloth #eleaps in front of #w$n #ewhile grinning #Pevilly#e.#n",vch,NULL,NULL,TO_ROOM);
      act("In a #C#zflash#n of #ylight#n, $n is lying in a #Rbloody #eheap#n on the #oground#n.",vch,NULL,NULL,TO_ROOM);

      ch->attack_type = ATTACK_PHYSICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SLASH;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 3500000, TRUE );
   }

   act("Lloth then struts to the center of the room as shadows leap beside her.",ch,NULL,NULL,TO_CHAR);
   act("Lloth then struts to the center of the room as shadows leap beside her.",ch,NULL,NULL,TO_ROOM);
   act("Lloth covers her figure seductively as she vanishes into the light.",ch,NULL,NULL,TO_CHAR);
   act("Lloth covers her figure seductively as she vanishes into the light.",ch,NULL,NULL,TO_ROOM);
   act("A loud voice booms '#eWeep for your God... for Lloth has answered...#n'",ch,NULL,NULL,TO_CHAR);
   act("A loud voice booms '#eWeep for your God... for Lloth has answered...#n'",ch,NULL,NULL,TO_ROOM);
}

void do_lloth_kiss( CHAR_DATA *ch )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   AFFECT_DATA af;
   int sn = skill_lookup( "drow poison" );

   act("Lloth ponders for an instance before cackling loudly.",ch,NULL,NULL,TO_CHAR);
   act("Lloth ponders for an instance before cackling loudly.",ch,NULL,NULL,TO_ROOM);
   act("Lloth says '#eCome taste the kiss of death.#n' as she blows one at you!",ch,NULL,NULL,TO_CHAR);
   act("Lloth says '#eCome taste the kiss of death.#n' as she blows one at you!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) )
         continue;

      if ( IS_IMMORTAL(vch) )
         continue; 

      if ( is_safe(ch, vch) )
         continue;

      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_STR;
      af.modifier  = -1;
      af.bitvector = AFF_POISON;
      affect_to_char( vch, &af );

      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_INT;
      af.modifier  = -2;
      af.bitvector = AFF_POISON;
      affect_to_char( vch, &af );

      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_CON;
      af.modifier  = -1;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      SET_BIT( vch->more, MORE_POISON );
      SET_BIT( vch->monkstuff, MONK_DEATH );

      act("$n clutches $s head in pain as $e appears to be very ill.",vch,NULL,NULL,TO_ROOM);
      act("You clutch your head in pain as you feel very sick.",vch,NULL,NULL,TO_CHAR);

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 2500000, TRUE );
   }

   act("Lloth then struts to the center of the room as shadows leap beside her.",ch,NULL,NULL,TO_CHAR);
   act("Lloth then struts to the center of the room as shadows leap beside her.",ch,NULL,NULL,TO_ROOM);
   act("Lloth covers her figure seductively as she vanishes into the light.",ch,NULL,NULL,TO_CHAR);
   act("Lloth covers her figure seductively as she vanishes into the light.",ch,NULL,NULL,TO_ROOM);
   act("A loud voice booms '#eYou shall welcome Death...#n'",ch,NULL,NULL,TO_CHAR);
   act("A loud voice booms '#eYou shall welcome Death...#n'",ch,NULL,NULL,TO_ROOM);
}

void do_yochlol( CHAR_DATA *ch )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   act("Lloth says '#yCome forth my loyal servants!#n' as pairs of red eyes appear in the shadows.",ch,NULL,NULL,TO_CHAR);
   act("Lloth says '#yCome forth my loyal servants!#n' as pairs of red eyes appear in the shadows.",ch,NULL,NULL,TO_ROOM);
   act("Two slimely substances oozes past the Chaos Deity as they encircle the room.",ch,NULL,NULL,TO_CHAR);
   act("Two slimely substances oozes past the Chaos Deity as they encircle the room.",ch,NULL,NULL,TO_ROOM);
   act("A loud voice booms '#eTo insult Lloth is Death!#n' as the slimes TOWER above you.",ch,NULL,NULL,TO_CHAR);
   act("A loud voice booms '#eTo insult Lloth is Death!#n' as the slimes TOWER above you.",ch,NULL,NULL,TO_ROOM);
   act("The slime transforms into a pair of Yochlols as they release a TERRIFYING scream!!!",ch,NULL,NULL,TO_CHAR);
   act("The slime transforms into a pair of Yochlols as they release a TERRIFYING scream!!!",ch,NULL,NULL,TO_ROOM);
   act("Suddenly the yochlols open their mouths as they emit off a barrage of Chaos energy!",ch,NULL,NULL,TO_CHAR);
   act("Suddenly the yochlols open their mouths as they emit off a barrage of Chaos energy!",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) )
         continue;

      if ( IS_IMMORTAL(vch) )
         continue; 

      if ( is_safe(ch, vch) )
         continue;

      SET_BIT( vch->more, MORE_CONFUSION );

      if ( !is_martial( vch ) )
         vch->wait += 12;

      act("$n clutches $s head in agony as $e is filled with plight.",vch,NULL,NULL,TO_ROOM);
      act("You clutch your head in agony as you are filled with plight.",vch,NULL,NULL,TO_CHAR);

      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_DARK;
      ch->attack_var = SOUL;
      ch->attack_focus = DIRECT;
      hurt_person( ch, vch, 2000000, TRUE );
   }

   act("Lloth then struts to the center of the room as the Yochlols leap beside her.",ch,NULL,NULL,TO_CHAR);
   act("Lloth then struts to the center of the room as the Yochlols leap beside her.",ch,NULL,NULL,TO_ROOM);
   act("Lloth covers her figure seductively as she vanishes into the light.",ch,NULL,NULL,TO_CHAR);
   act("Lloth covers her figure seductively as she vanishes into the light.",ch,NULL,NULL,TO_ROOM);
   act("A loud voice booms '#eNext time, you won't be so lucky...#n'",ch,NULL,NULL,TO_CHAR);
   act("A loud voice booms '#eNext time, you won't be so lucky...#n'",ch,NULL,NULL,TO_ROOM);
}

void do_lloth( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) 
     return;

  ch->achi = 40;

  act("The sky turns dark as a beautiful beam of energy shines down from the heaven.",ch,NULL,NULL,TO_CHAR);
  act("The sky turns dark as a beautiful beam of energy shines down from the heaven.",ch,NULL,NULL,TO_ROOM);

  act("Your mouth open in terror as a gorgeous woman emerges from the light.",ch,NULL,NULL,TO_CHAR);
  act("Your mouth open in terror as a gorgeous woman emerges from the light.",ch,NULL,NULL,TO_ROOM);
  act("The woman gazes around with piercing eyes as two shadowly figures appear behind her.",ch,NULL,NULL,TO_CHAR);
  act("The woman gazes around with piercing eyes as two shadowly figures appear behind her.",ch,NULL,NULL,TO_ROOM);
  act("\n\rLloth mutters '#yFoolish Creatures, you dare defy against me?#n'\n\r",ch,NULL,NULL,TO_ROOM);
  act("\n\rLloth mutters '#yFoolish Creatures, you dare defy against me?#n'\n\r",ch,NULL,NULL,TO_CHAR);

  if ( number_percent() > 84 )
     {
        do_l_lightblade(ch);
        return;
     }

  switch( number_range(1,3) )
  {
     case 2: do_lloth_kiss(ch);
             break;
     case 3:
     case 1: do_yochlol(ch);
             break;
  }
}

void do_diamond_dust( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;
  int chance = 50;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS( ch, CLASS_SNOW_ELF ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->pcdata->deity != DEI_SHIVA )
           {
	        send_to_char("Huh?\n\r",ch);
              return;
           }

        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Shiva to summon her. Kill her foes.\n\r",ch);
              return;
           }
     }      

  if ( IS_CLASS( ch, CLASS_SNOW_ELF ) && ch->pkill < 10 )
     {
	  send_to_char("You aren't powerful enough to request your deity assistance as yet.\n\r",ch);
        return;
     }

  if ( ch->mage_timer > 0 )
     {
	  send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( !IS_CLASS( ch, CLASS_SNOW_ELF ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        ch->mage_timer = 25;
        ch->cmbt[0]--;
     }
     else {
             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
             chance = 0;
          }

  ch->wait = 6;

  dam += UMIN( 1500000, ( king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members + king_table[DEI_BIRD].members
               + king_table[DEI_CARB].members + king_table[DEI_SIREN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_IFRIT].members - king_table[DEI_SHIVA].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  act("You say, '#yShatter thy enemies with thee freezing might... Diamond Dust!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yShatter thy enemies with thee freezing might... Diamond Dust!#n'",ch,NULL,NULL,TO_ROOM);
  act("You gasp as a huge icicle smashes itself from the ground.",ch,NULL,NULL,TO_CHAR);
  act("You gasp as a huge icicle smashes itself from the ground.",ch,NULL,NULL,TO_ROOM);
  act("As the fog clears up, you see a beautiful creature trap within its shivering prison.",ch,NULL,NULL,TO_CHAR);
  act("As the fog clears up, you see a beautiful creature trap within its shivering prison.",ch,NULL,NULL,TO_ROOM);
  act("A smirk appears on the creature's face as she braces her body, shattering the ice shards while apart!",ch,NULL,NULL,TO_CHAR);
  act("A smirk appears on the creature's face as she braces her body, shattering the ice shards while apart!",ch,NULL,NULL,TO_ROOM);
  act("Shiva stretches her body to the limit as blinding energy manifests into her hands.",ch,NULL,NULL,TO_ROOM);
  act("Shiva stretches her body to the limit as blinding energy manifests into her hands.",ch,NULL,NULL,TO_CHAR);
  act("The Ball of searing energy grows larger as Shiva draws her hands back.",ch,NULL,NULL,TO_ROOM);
  act("The Ball of searing energy grows larger as Shiva draws her hands back.",ch,NULL,NULL,TO_CHAR);
  act("Shiva pushes her hand forward as a huge beam of freezing energy shatters everything in its path!",ch,NULL,NULL,TO_ROOM);
  act("Shiva pushes her hand forward as a huge beam of freezing energy shatters everything in its path!",ch,NULL,NULL,TO_CHAR);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     if ( !IS_SET( vch->more, MORE_FROZEN ) && !is_martial( vch ) && number_percent() > chance - devil_luck( ch, vch, 5 ) )
        {
           SET_BIT( vch->more, MORE_FROZEN );

           if ( IS_CLASS( ch, CLASS_SNOW_ELF ) )
              vch->freeze_timer = 2;
              else vch->freeze_timer = 1;
        }

     act("Your body becomes paralysed as the beam of energy ZOOMS through your weak body.",vch,NULL,NULL,TO_CHAR);
     act("$n's body becomes paralysed as the beam of energy ZOOMS through $s weak body.",vch,NULL,NULL,TO_ROOM);

     ch->ele_attack = ATTACK_ICE;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = BEAM;
     ch->attack_focus = DIRECT;

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( IS_SET( vch->elemental, ELE_WWATER ) )
                hurt_person( ch, vch, dam * 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_SHIVA )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_IFRIT )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );
  }
}

void do_silent_voice( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS( ch, CLASS_HOBBIT ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->pcdata->deity != DEI_SIREN )
           {
	        send_to_char("Huh?\n\r",ch);
              return;
           }

        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Siren to summon her. Kill her foes.\n\r",ch);
              return;
           }
     }

  if ( IS_CLASS( ch, CLASS_HOBBIT ) && ch->pkill < 10 )
     {
	  send_to_char("You aren't powerful enough to request your deity assistance as yet.\n\r",ch);
        return;
     }

  if ( ch->mage_timer > 0 )
     {
	  send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( !IS_CLASS( ch, CLASS_HOBBIT ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        ch->mage_timer = 25;
        ch->cmbt[0]--;
     }
     else {
             if ( IS_CLASS( ch, CLASS_HOBBIT ) && is_oring( ch ) == TRUE )
                {
                   send_to_char( "You can't use this divine power while corrupted from the ring.\n\r", ch );
                   return;
                }

             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
          }

  act("You say, '#yMake thee mouth thirst for words... Silent Voice!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yMake thee mouth thirst for words... Silent Voice!#n'",ch,NULL,NULL,TO_ROOM);
  act("Beautiful notes floats through the air as it caresses your body.",ch,NULL,NULL,TO_CHAR);
  act("Beautiful notes floats through the air as it caresses your body.",ch,NULL,NULL,TO_ROOM);
  act("Soft waves lap at your feet as a huge rock jotted from beneath the waters.",ch,NULL,NULL,TO_CHAR);
  act("Soft waves lap at your feet as a huge rock jotted from beneath the waters.",ch,NULL,NULL,TO_ROOM);
  act("Pleasant light fills the room as Siren fades into existence.",ch,NULL,NULL,TO_CHAR);
  act("Pleasant light fills the room as Siren fades into existence.",ch,NULL,NULL,TO_ROOM);
  act("Soft music fills the air as Siren starts playing her harp.",ch,NULL,NULL,TO_CHAR);
  act("Soft music fills the air as Siren starts playing her harp.",ch,NULL,NULL,TO_ROOM);
  act("Mystical Waves BEATS the room with its violent speeds.",ch,NULL,NULL,TO_CHAR);
  act("Mystical Waves BEATS the room with its violent speeds.",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members + king_table[DEI_BIRD].members
               + king_table[DEI_CARB].members + king_table[DEI_SIREN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_DIABLOS].members - king_table[DEI_SIREN].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     ch->ele_attack = ATTACK_HOLY;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = SOUL;
     ch->attack_focus = AERIAL;

     if ( !IS_SET(vch->more, MORE_HEX) )
        SET_BIT(vch->more, MORE_HEX);

     act("You feel tired as the soothing music drains your life force.",vch,NULL,NULL,TO_CHAR);
     act("$n feels tired as the soothing music drains $s life force.",vch,NULL,NULL,TO_ROOM);

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( IS_SET( vch->elemental, ELE_WHOLY ) )
                hurt_person( ch, vch, dam * 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_SIREN )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_DIABLOS )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );

     if ( vch == NULL || vch->in_room == NULL || vch->position < POS_STUNNED )
        continue;

     stop_fighting( vch, TRUE );
     vch->wait = 12;
  }

  if ( IS_SET(ch->act, PLR_WIZINVIS) )
     REMOVE_BIT(ch->act, PLR_WIZINVIS);
}

void do_luna( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) ) 
     return;

  if ( ch->pcdata->deity != DEI_LUNA && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Luna to summon it. Kill her foes.\n\r",ch);
              return;
           }

        if ( ch->mage_timer > 0 )
           {
	        send_to_char("You are still recovering from the last one.\n\r",ch);
              return;
           }

        ch->mage_timer = 25;
        ch->wait = 6;
        ch->cmbt[0]--;
     }
     else {
             if ( ch->mage_timer > 0 )
                {
	             send_to_char("You are still recovering from the last one.\n\r",ch);
                   return;
                }

             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
             ch->wait = 6;
          }

  act("You say, '#yDamn thy dreams with the forbidden past... Lunar Kiss!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yDamn thy dreams with the forbidden past... Lunar Kiss!#n'",ch,NULL,NULL,TO_ROOM);
  act("Beautiful notes floats through the air as it caresses your body.",ch,NULL,NULL,TO_CHAR);
  act("Beautiful notes floats through the air as it caresses your body.",ch,NULL,NULL,TO_ROOM);
  act("The skies suddenly goes dark as a beautiful light shines down upon you.",ch,NULL,NULL,TO_CHAR);
  act("The skies suddenly goes dark as a beautiful light shines down upon you.",ch,NULL,NULL,TO_ROOM);
  act("Gazing up with your eyes shielded, you see a beautiful figure above.",ch,NULL,NULL,TO_CHAR);
  act("Gazing up with your eyes shielded, you see a beautiful figure above.",ch,NULL,NULL,TO_ROOM);
  act("Her hair blows soundlessly into the winds as she blows a kiss at you.",ch,NULL,NULL,TO_CHAR);
  act("Her hair blows soundlessly into the winds as she blows a kiss at you.",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_LUNA].members + king_table[DEI_IFRIT].members + king_table[DEI_DIABLOS].members
               + king_table[DEI_ODIN].members + king_table[DEI_TITAN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_GAIA].members - king_table[DEI_LUNA].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     ch->ele_attack = ATTACK_LUNAR;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = SOUL;
     ch->attack_focus = AERIAL;

     act("You feel sleepy as the mysterious kiss sends you... to... another realm...",vch,NULL,NULL,TO_CHAR);
     act("$n looks sleepy as $e suddenly collapse to $s knees unconcious.",vch,NULL,NULL,TO_ROOM);

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( IS_SET( vch->elemental, ELE_WLUNAR ) )
                hurt_person( ch, vch, dam * 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_LUNA )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_GAIA )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );

     if ( vch == NULL || vch->in_room == NULL || vch->position < POS_STUNNED )
        continue;

     stop_fighting( vch, TRUE );
     vch->position = POS_SLEEPING;
     vch->wait = 12;
  }

  if ( IS_SET(ch->act, PLR_WIZINVIS) )
     REMOVE_BIT(ch->act, PLR_WIZINVIS);
}

void do_gaia( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) ) 
     return;

  if ( ch->pcdata->deity != DEI_GAIA && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Gaia to summon it. Kill her foes.\n\r",ch);
              return;
           }

        if ( ch->mage_timer > 0 )
           {
	        send_to_char("You are still recovering from the last one.\n\r",ch);
              return;
           }

        ch->mage_timer = 25;
        ch->wait = 6;
        ch->cmbt[0]--;
     }
     else {
             if ( ch->mage_timer > 0 )
                {
	             send_to_char("You are still recovering from the last one.\n\r",ch);
                   return;
                }

             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
             ch->wait = 6;
          }

  act("You say, '#ySmite down ALL with the force of nature... Vengeance!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#ySmite down ALL with the force of nature... Vengeance!#n'",ch,NULL,NULL,TO_ROOM);
  act("The ground beneath your feet trembles ViOleNtLy as it starts to crack.",ch,NULL,NULL,TO_CHAR);
  act("The ground beneath your feet trembles ViOleNtLy as it starts to crack.",ch,NULL,NULL,TO_ROOM);
  act("A voice snarls '#PDeath to the followers of civilisation!#n' as the skies darken.",ch,NULL,NULL,TO_CHAR);
  act("A voice snarls '#PDeath to the followers of civilisation!#n' as the skies darken.",ch,NULL,NULL,TO_ROOM);
  act("A howling wind SeNds you SpraWlInG as a powerful beam of lava spruts high above.",ch,NULL,NULL,TO_CHAR);
  act("A howling wind SeNds you SpraWlInG as a powerful beam of lava spruts high above.",ch,NULL,NULL,TO_ROOM);
  act("All the colours of the rainbow forms together into a deadly spiral of energy.",ch,NULL,NULL,TO_CHAR);
  act("All the colours of the rainbow forms together into a deadly spiral of energy.",ch,NULL,NULL,TO_ROOM);
  act("Before you recover, the colourful beam rains down from above!!!",ch,NULL,NULL,TO_CHAR);
  act("Before you recover, the colourful beam rains down from above!!!",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members + king_table[DEI_BIRD].members
               + king_table[DEI_CARB].members + king_table[DEI_SIREN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_LUNA].members - king_table[DEI_GAIA].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = SOUL;
     ch->attack_focus = OVERHEAD;

     act("You scream in AgOnY as you are smited down by Gaia's vengeance!",vch,NULL,NULL,TO_CHAR);
     act("$n screams in AgOnY as $e is smited down by Gaia's vengeance!",vch,NULL,NULL,TO_ROOM);

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_GAIA )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_LUNA )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );

     if ( vch == NULL || vch->in_room == NULL || vch->position < POS_STUNNED )
        continue;

     stop_fighting( vch, TRUE );
     vch->air_timer = 1;
  }

  if ( IS_SET(ch->act, PLR_WIZINVIS) )
     REMOVE_BIT(ch->act, PLR_WIZINVIS);
}

void do_reflect_beam( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) )
     return;

  if ( ch->pcdata->deity != DEI_CARB && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Carburkle to summon it. Kill it foes.\n\r",ch);
              return;
           }

        if ( ch->mage_timer > 0 )
           {
	        send_to_char("You are still recovering from the last one.\n\r",ch);
              return;
           }

        ch->mage_timer = 25;
        ch->wait = 6;
        ch->cmbt[0]--;
     }
     else {
             if ( ch->mage_timer > 0 )
                {
	             send_to_char("You are still recovering from the last one.\n\r",ch);
                   return;
                }

             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
             ch->wait = 6;
          }

  act("You say, '#yCute beast, keep harmful spells from our reach... Reflection!!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yCute beast, keep harmful spells from our reach... Reflection!!#n'",ch,NULL,NULL,TO_ROOM);
  act("A small hole magically appears on the ground as two bright eyes stare from the darkness.",ch,NULL,NULL,TO_CHAR);
  act("A small hole magically appears on the ground as two bright eyes stare from the darkness.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, a small creature leaps in the air leaving a trail of stardust behind.",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, a small creature leaps in the air leaving a trail of stardust behind.",ch,NULL,NULL,TO_ROOM);
  act("Pointing its long tail at you, sparkling energy converges to the tip of it.",ch,NULL,NULL,TO_CHAR);
  act("Pointing its long tail at you, sparkling energy converges to the tip of it.",ch,NULL,NULL,TO_ROOM);
  act("Carburkle screams loudly as its tail shoots palish energy onto its friends.",ch,NULL,NULL,TO_CHAR);
  act("Carburkle screams loudly as its tail shoots palish energy onto its friends.",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members + king_table[DEI_BIRD].members
               + king_table[DEI_CARB].members + king_table[DEI_SIREN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_ODIN].members - king_table[DEI_CARB].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( IS_NPC(vch) )
        continue;

     if ( is_same_group( ch, vch ) || ( !IS_NPC( ch ) && !IS_NPC( vch ) && 
          ch->pcdata->kingdom == vch->pcdata->kingdom && ch->pcdata->kingdom != 0 && ch->pcdata->kingdom != 5 ) )
        {
           if ( !IS_SET( vch->monkstuff, MONK_HEAL ) )
              SET_BIT( vch->monkstuff, MONK_HEAL );

           spell_divine_reflection( skill_lookup("divine reflection"), 24, ch, vch );
           act("Your body is protected with Carbunkle's aura.",vch,NULL,NULL,TO_CHAR);
           continue;
        }

     if ( is_safe(ch, vch) )
        continue;

     if ( !IS_SET( vch->more, MORE_CONFUSION ) )
        SET_BIT( vch->more, MORE_CONFUSION );

     act("Everything become a daze to you as you swing wildly about!",vch,NULL,NULL,TO_CHAR);     
     act("Everything become a daze to $n as $e swings wildly about!",vch,NULL,NULL,TO_ROOM);     
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = HIT;
     ch->attack_focus = DIRECT;

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_CARB )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_ODIN )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );
  }

  act("The small hole fades away as Carbunkle dives back into it.",ch,NULL,NULL,TO_CHAR);
  act("The small hole fades away as Carbunkle dives back into it.",ch,NULL,NULL,TO_ROOM);
}

void do_thunder_storm( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS( ch, CLASS_WISP ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->pcdata->deity != DEI_BIRD )
           {
	        send_to_char("Huh?\n\r",ch);
              return;
           }

        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Quezacotl to summon it. Kill it foes.\n\r",ch);
              return;
           }
     }

  if ( IS_CLASS( ch, CLASS_WISP ) && ch->pkill < 10 )
     {
	  send_to_char("You aren't powerful enough to request your deity assistance as yet.\n\r",ch);
        return;
     }

  if ( ch->mage_timer > 0 )
     {
	  send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( !IS_CLASS( ch, CLASS_WISP ) )
     {
        if ( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) )
           ch->mage_timer = 25;
           else {
                   ch->mage_timer = 25;
                   ch->cmbt[0]--;
                }

        act("You say, '#yAncient bird... inpart thy powers to us... Thunder Storm!#n'",ch,NULL,NULL,TO_CHAR);
        act("$n says, '#yAncient bird... inpart thy powers to us... Thunder Storm!#n'",ch,NULL,NULL,TO_ROOM);
     }
     else {
             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
             dam *= 2;
             act("You cooe, '#yBig BIG birdo... inpart thy powers to usto... umm Thundi Stormy!#n'",ch,NULL,NULL,TO_CHAR);
             act("$n cooes, '#yBig BIG birdo... inpart thy powers to usto... umm Thundi Stormy!#n'",ch,NULL,NULL,TO_ROOM);
          }

  act("The Sky suddenly turns dark and cloudy as rain starts to fall rapidly.",ch,NULL,NULL,TO_CHAR);
  act("The Sky suddenly turns dark and cloudy as rain starts to fall rapidly.",ch,NULL,NULL,TO_ROOM);
  act("Numerous thunder bolts form into an electrifying tornado.",ch,NULL,NULL,TO_CHAR);
  act("Numerous thunder bolts form into an electrifying tornado.",ch,NULL,NULL,TO_ROOM);
  act("As the tornado stops spinning, it forms into a huge, blurry bird!",ch,NULL,NULL,TO_CHAR);
  act("As the tornado stops spinning, it forms into a huge, blurry bird!",ch,NULL,NULL,TO_ROOM);
    
  act("Quezacotl spreads out its wings wide apart as it looks straight into the sky.",ch,NULL,NULL,TO_CHAR);
  act("Quezacotl spreads out its wings wide apart as it looks straight into the sky.",ch,NULL,NULL,TO_ROOM);
  act("A manifest of electricity leaps from its mouth and spreads across the room.",ch,NULL,NULL,TO_CHAR);
  act("A manifest of electricity leaps from its mouth and spreads across the room.",ch,NULL,NULL,TO_ROOM);
  act("As the energy soars through your body, a tremedous Thunder Bolt CRASHES into the ground mightily.",ch,NULL,NULL,TO_CHAR);
  act("As the energy soars through your body, a tremedous Thunder Bolt CRASHES into the ground mightily.",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_SHIVA].members + king_table[DEI_GAIA].members + king_table[DEI_BIRD].members
               + king_table[DEI_CARB].members + king_table[DEI_SIREN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_TITAN].members - king_table[DEI_BIRD].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     if ( !is_martial( vch ) )
        vch->wait = UMIN( 18, vch->wait + 12 );

     ch->ele_attack = ATTACK_THUNDER;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = BEAM;
     ch->attack_focus = DIRECT;

     act("The Manifest of electricity ZUMMS through your NERVAL system DISRUPTING their proper functions!",vch,NULL,NULL,TO_CHAR);     
     act("The Manifest of electricity ZUMMS through $n's NERVAL system DISRUPTING their proper functions!",vch,NULL,NULL,TO_ROOM);

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( IS_SET( vch->elemental, ELE_WTHUNDER ) )
                hurt_person( ch, vch, dam * 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_BIRD )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_TITAN )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );
  }
}

void do_zantetsuken( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) ) 
     return;

  if ( ch->pcdata->deity != DEI_ODIN && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Odin to summon him. Kill his foes.\n\r",ch);
              return;
           }

        if ( ch->mage_timer > 0 )
           {
	        send_to_char("You are still recovering from the last one.\n\r",ch);
              return;
           }

        ch->mage_timer = 25;
        ch->wait = 6;
        ch->cmbt[0]--;
     }
     else {
             if ( ch->mage_timer > 0 )
                {
	             send_to_char("You are still recovering from the last one.\n\r",ch);
                   return;
                }

             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
             ch->wait = 6;
          }

  act("You say, '#yAgony of the Swift Blade... Zantetsuken!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yAgony of the Swift Blade... Zantetsuken!#n'",ch,NULL,NULL,TO_ROOM);
  act("The sky turns dark as thunderbolts devastate the area, heavy rain clouding your vision.",ch,NULL,NULL,TO_CHAR);
  act("The sky turns dark as thunderbolts devastate the area, heavy rain clouding your vision.",ch,NULL,NULL,TO_ROOM);
  act("You hear splashes from puddles as a beautiful warhorse walks through.",ch,NULL,NULL,TO_CHAR);
  act("You hear splashes from puddles as a beautiful warhorse walks through.",ch,NULL,NULL,TO_ROOM);
  act("The figure on it lowers its sword as it scans the area.",ch,NULL,NULL,TO_CHAR);
  act("The figure on it lowers its sword as it scans the area.",ch,NULL,NULL,TO_ROOM);
  act("Odin's eyes blazes with demonic fire as it stares deep into your soul.",ch,NULL,NULL,TO_CHAR);
  act("Odin's eyes blazes with demonic fire as it stares deep into your soul.",ch,NULL,NULL,TO_ROOM);
  act("The horse whines loudly as Odin draws back the reins, raising Zantetsuken in the air.",ch,NULL,NULL,TO_CHAR);
  act("The horse whines loudly as Odin draws back the reins, raising Zantetsuken in the air.",ch,NULL,NULL,TO_ROOM);
  act("Odin grins evilly as it rushes straight into its target!",ch,NULL,NULL,TO_CHAR);
  act("Odin grins evilly as it rushes straight into its target!",ch,NULL,NULL,TO_ROOM);
  act("Odin slashes VICIOUSLY to the left as palish energy shimmers after the blade!",ch,NULL,NULL,TO_CHAR);
  act("Odin slashes VICIOUSLY to the left as palish energy shimmers after the blade!",ch,NULL,NULL,TO_ROOM);
  act("Odin swipes POWERFULLY to the right as palish energy shimmers after the blade!",ch,NULL,NULL,TO_CHAR);
  act("Odin swipes POWERFULLY to the right as palish energy shimmers after the blade!",ch,NULL,NULL,TO_ROOM);
  act("Odin slashes VICIOUSLY to the left as palish energy shimmers after the blade!",ch,NULL,NULL,TO_CHAR);
  act("Odin slashes VICIOUSLY to the left as palish energy shimmers after the blade!",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_LUNA].members + king_table[DEI_IFRIT].members + king_table[DEI_DIABLOS].members
               + king_table[DEI_ODIN].members + king_table[DEI_TITAN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_CARB].members - king_table[DEI_ODIN].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;
 
  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     if ( number_percent() > 94 - devil_luck( ch, vch, 5 ) && !IS_SET( vch->cmbt[5], SKL2_AMIND ) )
        {
           stop_fighting( vch, TRUE );
           vch->hit = -2000010;
           vch->position = POS_MORTAL;
           continue;
        }

     ch->attack_type = ATTACK_PHYSICAL;
     ch->attack_var = SLASH;
     ch->attack_focus = DIRECT;

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_ODIN )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_CARB )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );
  }

  act("Odin dashes into the distance, cackling wildly.",ch,NULL,NULL,TO_CHAR);
  act("Odin dashes into the distance, cackling wildly.",ch,NULL,NULL,TO_ROOM);
}

void do_ddarkside( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS( ch, CLASS_HOBBIT ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        if ( ch->pcdata->deity != DEI_DIABLOS )
           {
	        send_to_char("Huh?\n\r",ch);
              return;
           }

        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Diablos to summon him. Kill his foes.\n\r",ch);
              return;
           }
     }

  if ( IS_CLASS( ch, CLASS_HOBBIT ) && ch->pkill < 10 )
     {
	  send_to_char("You aren't powerful enough to request your deity assistance as yet.\n\r",ch);
        return;
     }

  if ( ch->mage_timer > 0 )
     {
	  send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( !IS_CLASS( ch, CLASS_HOBBIT ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        ch->mage_timer = 25;
        ch->cmbt[0]--;
     }
     else {
             if ( IS_CLASS( ch, CLASS_HOBBIT ) && is_oring( ch ) == FALSE )
                {
                   send_to_char( "It is hapless to make use of the Dark powers without the ring.\n\r", ch );
                   return;
                }

             SET_BIT( ch->arena, AFF2_DM );
             ch->mage_timer = 25;
          }

  act("You say, '#yFriends and foes, beware of the Darkside!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yFriends and foes, beware of the Darkside!#n'",ch,NULL,NULL,TO_ROOM);
  act("High squeeky sounds fills the air as countless bats flies all over the room!",ch,NULL,NULL,TO_CHAR);
  act("High squeeky sounds fills the air as countless bats flies all over the room!",ch,NULL,NULL,TO_ROOM);
  act("More and more bats arrive as all of them slowly transform in Diablos.",ch,NULL,NULL,TO_CHAR);
  act("More and more bats arrive as all of them slowly transform in Diablos.",ch,NULL,NULL,TO_ROOM);
  act("Looking around itself angrily, it releases a very loud scream as it spreads its wings.",ch,NULL,NULL,TO_CHAR);
  act("Looking around itself angrily, it releases a very loud scream as it spreads its wings.",ch,NULL,NULL,TO_ROOM);

  dam += UMIN( 1500000, ( king_table[DEI_LUNA].members + king_table[DEI_IFRIT].members + king_table[DEI_DIABLOS].members
               + king_table[DEI_ODIN].members + king_table[DEI_TITAN].members ) * 75000 );
  bonus = URANGE( 0, ( king_table[DEI_SIREN].members - king_table[DEI_DIABLOS].members ) * 150000, 3000000 );

  if ( conan == 10 )
     dam *= 3;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
      vch_next = vch->next_in_room;

     if ( is_same_group( ch, vch ) || ( !IS_NPC( ch ) && !IS_NPC( vch ) && 
          ch->pcdata->kingdom == vch->pcdata->kingdom && ch->pcdata->kingdom != 0 && ch->pcdata->kingdom != 5 ) )
        spell_darkside(skill_lookup("darkside"),500,ch,vch);
  }

  act("Flapping it powerful wings, Diablo concentrates as the room suddenly goes dark.",ch,NULL,NULL,TO_CHAR);
  act("Flapping it powerful wings, Diablo concentrates as the room suddenly goes dark.",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( is_same_group(ch,vch) )
        continue;

     if ( is_safe(ch, vch) )
        continue;

     ch->ele_attack = ATTACK_DARK;
     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = SOUL;
     ch->attack_focus = DIRECT;

     act("Something DESTROYS your mortal body!",vch,NULL,NULL,TO_CHAR);
     act("Something DESTROYS $n's body!",vch,NULL,NULL,TO_ROOM);

     if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
        hurt_person( ch, vch, dam / 2, TRUE );
        else if ( IS_SET( vch->elemental, ELE_WDARK ) )
                hurt_person( ch, vch, dam * 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_DIABLOS )
                hurt_person( ch, vch, dam / 2, TRUE );
        else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_SIREN )
                hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                else hurt_person( ch, vch, dam, TRUE );
  }
}

void do_ifrit( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 ), bonus;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_PHOENIX ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
      {
        if ( ch->pcdata->deity != DEI_IFRIT )
           {
	        send_to_char("Huh?\n\r",ch);
              return;
           }

        if ( ch->cmbt[0] == 0 )
           {
              stc("You don't have the approval of Ifrit to summon him. Kill his foes.\n\r",ch);
              return;
           }
      }

  if ( IS_CLASS( ch, CLASS_PHOENIX ) && ch->pkill < 10 )
     {
	  send_to_char("You aren't powerful enough to request your deity assistance as yet.\n\r",ch);
        return;
     }

  if ( ch->mage_timer > 0 )
     {
	  send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( !IS_CLASS( ch, CLASS_PHOENIX ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        ch->mage_timer = 25;
        ch->cmbt[0]--;
     }
     else {
             ch->mage_timer = 25;
             SET_BIT( ch->arena, AFF2_DM );
          }

   act("You say, '#yIfrit, Let all that stand before you suffer... Hell Fire!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yIfrit, Let all that stand before you suffer... Hell Fire!#n'",ch,NULL,NULL,TO_ROOM);
   act("The ground trembles as huge flaming boulders flies upwards into the sky.",ch,NULL,NULL,TO_CHAR);
   act("The ground trembles as huge flaming boulders flies upwards into the sky.",ch,NULL,NULL,TO_ROOM);
   act("Ifrit smashes from underneath the ground as it lets out a loud roar.",ch,NULL,NULL,TO_CHAR);
   act("Ifrit smashes from underneath the ground as it lets out a loud roar.",ch,NULL,NULL,TO_ROOM);
   act("Concentrating furious, a gigantic ball of fire SOARS from the ground carrying Ifrit into the sky.",ch,NULL,NULL,TO_CHAR);
   act("Concentrating furious, a gigantic ball of fire SOARS from the ground carrying Ifrit into the sky.",ch,NULL,NULL,TO_ROOM);
   act("Letting out a huge roar as Ifrit raises its arms, it pounds the fireball sending it CRASHING into the ground!",ch,NULL,NULL,TO_ROOM);
   act("Letting out a huge roar as Ifrit raises its arms, it pounds the fireball sending it CRASHING into the ground!",ch,NULL,NULL,TO_CHAR);
   act("The Huge fireball explodes sending the entire vicinity in flames!",ch,NULL,NULL,TO_ROOM);
   act("The Huge fireball explodes sending the entire vicinity in flames!",ch,NULL,NULL,TO_CHAR);

   dam += UMIN( 1500000, ( king_table[DEI_LUNA].members + king_table[DEI_IFRIT].members + king_table[DEI_DIABLOS].members
                + king_table[DEI_ODIN].members + king_table[DEI_TITAN].members ) * 75000 );
   bonus = URANGE( 0, ( king_table[DEI_SHIVA].members - king_table[DEI_IFRIT].members ) * 150000, 3000000 );

   if ( conan == 10 )
      dam *= 3;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) )
         continue;

      if ( is_safe(ch, vch) )
         continue;

      if ( !IS_AFFECTED( vch, AFF_FLAMING ) )
         SET_BIT( vch->affected_by, AFF_FLAMING );

      ch->ele_attack = ATTACK_FIRE;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = AERIAL;

      vch->move = UMAX( 0, vch->move - ( vch->max_move * 0.25 ) );
      vch->mana = UMAX( 0, vch->mana - ( vch->max_mana * 0.25 ) );

      act("The Cackling flames eat through your mortal flesh!",vch,NULL,NULL,TO_CHAR);
      act("The Cackling flames eat through $n's flesh!",vch,NULL,NULL,TO_ROOM);

      if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
         hurt_person( ch, vch, dam / 2, TRUE );
         else if ( IS_SET( vch->elemental, ELE_WFIRE ) )
                 hurt_person( ch, vch, dam * 2, TRUE );
         else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_IFRIT )
                 hurt_person( ch, vch, dam / 2, TRUE );
         else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_SHIVA )
                 hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                 else hurt_person( ch, vch, dam, TRUE );
   }
}

void do_titan( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   bool pre_room;
   int direction;
   int count, bonus;
   int dam = 750000 + ( ( get_curr_wis( ch ) + 1 ) * 15000 );

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_DUERGAR ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
      {
         if ( ch->pcdata->deity != DEI_TITAN )
            {
	         send_to_char("Huh?\n\r",ch);
               return;
            }

         if ( ch->cmbt[0] == 0 )
            {
               stc("You don't have the approval of Titan to summon him. Kill his foes.\n\r",ch);
               return;
            }
      }

  if ( IS_CLASS( ch, CLASS_DUERGAR ) && ch->pkill < 10 )
     {
	  send_to_char("You aren't powerful enough to request your deity assistance as yet.\n\r",ch);
        return;
     }

  if ( ch->mage_timer > 0 )
     {
	  send_to_char("You are still recovering from the last one.\n\r",ch);
        return;
     }

  if ( !IS_CLASS( ch, CLASS_DUERGAR ) && !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
        ch->mage_timer = 25;
        ch->cmbt[0]--;
     }
     else {
             ch->mage_timer = 25;
             SET_BIT( ch->arena, AFF2_DM );
          }

   act("You say, '#yUnleash your raw power Titan and show no fear... Anger of the Land!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yUnleash your raw power Titan and show no fear... Anger of the Land!#n'",ch,NULL,NULL,TO_ROOM);
   act("A fist SmAsHeS itself from the ground beneath you as you hear a low growl...",ch,NULL,NULL,TO_CHAR);
   act("A fist SmAsHeS itself from the ground beneath you as you hear a low growl...",ch,NULL,NULL,TO_ROOM);
   act("The ground shatters as a MASSIVE giant steps out from beneath it and looks around him.",ch,NULL,NULL,TO_CHAR);
   act("The ground shatters as a MASSIVE giant steps out from beneath it and looks around him.",ch,NULL,NULL,TO_ROOM);
   act("Titan roars as he places his hands under the terrain and tries to lift it.",ch,NULL,NULL,TO_CHAR);
   act("Titan roars as he places his hands under the terrain and tries to lift it.",ch,NULL,NULL,TO_ROOM);
   act("Having difficulty, he roars again using all of his strength as he tilts the land above him.",ch,NULL,NULL,TO_CHAR);
   act("Having difficulty, he roars again using all of his strength as he tilts the land above him.",ch,NULL,NULL,TO_ROOM);
   act("With a heave, he throws the terrain towards you as he flexes his muscles.",ch,NULL,NULL,TO_CHAR);
   act("With a heave, he throws the terrain towards you as he flexes his muscles.",ch,NULL,NULL,TO_ROOM);

   dam += UMIN( 1500000, ( king_table[DEI_LUNA].members + king_table[DEI_IFRIT].members + king_table[DEI_DIABLOS].members
                + king_table[DEI_ODIN].members + king_table[DEI_TITAN].members ) * 75000 );
   bonus = URANGE( 0, ( king_table[DEI_BIRD].members - king_table[DEI_TITAN].members ) * 150000, 3000000 );

   if ( conan == 10 )
      dam *= 3;

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_same_group(ch,vch) )
         continue;

      if ( is_safe(ch, vch) )
         continue;

      act("You scream loudly as the entire land mass gets topple over you!",vch,NULL,NULL,TO_CHAR);
      act("$n screams loudly as the entire land mass gets topple over $m!",vch,NULL,NULL,TO_ROOM);

      if ( !IS_SET( vch->more, MORE_BLIND ) )
         {
            SET_BIT( vch->more, MORE_BLIND );
            vch->blind_timer = 2;
         }

      count = 0;
      direction = number_range( 0, 3 );

      do
      {
         pre_room = special_slam( vch, direction );
         count++;
      }
      while ( count < 10 && pre_room != TRUE );

      ch->ele_attack = ATTACK_EARTH;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLUNT;
      ch->attack_focus = LOW;

      if ( IS_CLASS( vch, CLASS_WIZARD ) && vch->clannum == 2 )
         hurt_person( ch, vch, dam / 2, TRUE );
         else if ( IS_SET( vch->elemental, ELE_WEARTH ) )
                 hurt_person( ch, vch, dam * 2, TRUE );
         else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_TITAN )
                 hurt_person( ch, vch, dam / 2, TRUE );
         else if ( !IS_NPC( vch ) && vch->pcdata->deity == DEI_BIRD )
                 hurt_person( ch, vch, ( dam + bonus ) * 2, TRUE );
                 else hurt_person( ch, vch, dam, TRUE );

      if ( vch != NULL && vch->in_room != NULL && vch->position > POS_STUNNED )
         {
            stop_fighting( vch, TRUE );
            vch->wait = 12;
         }
   }

   if ( IS_SET(ch->act, PLR_WIZINVIS) )
      REMOVE_BIT(ch->act, PLR_WIZINVIS);
}

void do_sun_bath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

  if ( !( IS_CLASS( ch, CLASS_WIZARD ) && ( ch->clannum == 2 || ch->clannum == 6 ) ) )
     {
	  send_to_char("Huh?\n\r",ch);
	  return;
     }

  if ( !str_cmp( "S", ch->groove ) && ch->hit < ch->max_hit * 0.25 && ch->ctimer[9] != 100 )
     {
	  send_to_char("Due to its nature, you need full stock to do this technique.\n\r",ch);
	  return;
     }

  if ( get_stock( ch ) < 100 )
     {
        stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
        return;
     }
          
  ch->wait = 6;
  subtract_stock( ch, 100 );

  act("You say, '#yMoogle Woogle, Toggle Doggle!!#n'",ch,NULL,NULL,TO_CHAR);
  act("$n says, '#yMoogle Woogle, Toggle Doggle!!#n'",ch,NULL,NULL,TO_ROOM);
  act("The heavens parted as bright sunlight shines in your face.",ch,NULL,NULL,TO_CHAR);
  act("The heavens parted as bright sunlight shines in your face.",ch,NULL,NULL,TO_ROOM);
  act("Suddenly, a dancing moogle does the moonwalk in front your very eyes!",ch,NULL,NULL,TO_CHAR);
  act("Suddenly, a dancing moogle does the moonwalk in front your very eyes!",ch,NULL,NULL,TO_ROOM);
  act("Looking at you sheepishly, it suddenly vanishes leaving a trail of magic dust in its path!",ch,NULL,NULL,TO_CHAR);
  act("Looking at you sheepishly, it suddenly vanishes leaving a trail of magic dust in its path!",ch,NULL,NULL,TO_ROOM);

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
  {
     vch_next = vch->next_in_room;

     if ( !is_same_group(ch,vch) )
        continue;

     if ( vch != ch && nogang != 0 )
        continue;

     act("You feel refresh as the magic dust bathe your tired body under the sun.",vch,NULL,NULL,TO_CHAR);
     act("$n feels refresh as the magic dust bathe $s tired body under the sun.",vch,NULL,NULL,TO_ROOM);
     hit_gain( vch, 1500000 );
  }
}

void do_salamander( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam = 2000000;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_WIZARD) && ( ch->clannum == 2 || ch->clannum == 6 ) )
       {
          send_to_char("Huh?\n\r",ch);
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

    act("You say, '#rEvil Dragon, lend me your demonic powers... Merton!#n'",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#rEvil Dragon, lend me your demonic powers... Merton!#n'",ch,NULL,NULL,TO_ROOM);
    act("$n's hands glows #rblazing red#n as $e leans forward and point one of them at you.",ch,NULL,NULL,TO_ROOM);
    act("Your hands glows #rblazing red#n as you lean forward and point one of them at the room.",ch,NULL,NULL,TO_CHAR);
    act("The entire area starts trembling violently as a low rumble is heard in the distance.",ch,NULL,NULL,TO_ROOM);
    act("The entire area starts trembling violently as a low rumble is heard in the distance.",ch,NULL,NULL,TO_CHAR);
    act("Suddenly the vicinity becomes a river of flames as they ANNIHLATES everything in their path.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly the vicinity becomes a river of flames as they ANNIHLATES everything in their path.",ch,NULL,NULL,TO_CHAR);
    act("As the unholy flames becomes hotter, a dragon looking form appears inbetween the blazing fires.",ch,NULL,NULL,TO_ROOM);
    act("As the unholy flames becomes hotter, a dragon looking form appears inbetween the blazing fires.",ch,NULL,NULL,TO_CHAR);
    act("The Salmander looks straight up as demonic energy rises from the ground CAUSING A HUGE EXPLOSION!",ch,NULL,NULL,TO_ROOM);
    act("The Salmander looks straight up as demonic energy rises from the ground CAUSING A HUGE EXPLOSION!",ch,NULL,NULL,TO_CHAR);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

         if ( is_safe(ch, vch) )
         continue;

         if ( !IS_AFFECTED( vch, AFF_FLAMING ) ) 
            SET_BIT( vch->affected_by, AFF_FLAMING );

         act("Your scream loudly as the flames eats your body.",vch,NULL,NULL,TO_CHAR);
         act("$n screams loudly as the flames eats $s body.",vch,NULL,NULL,TO_ROOM);
         ch->attack_type = ATTACK_MAGICAL;
         ch->ele_attack = ATTACK_FIRE;
         ch->attack_var = BLAST;
         ch->attack_focus = UPPERCUT;
         hurt_person( ch, vch, 2000000, TRUE );
    }

    act("The flames dies away softly...",ch,NULL,NULL,TO_ROOM);
    act("The flames dies away softly...",ch,NULL,NULL,TO_CHAR);
}

void do_bahamut( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int dam, count;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_WIZARD) && ( ch->clannum == 2 || ch->clannum == 6 ) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }
 
   if ( get_stock( ch ) < 300 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }
          
   ch->wait = 12;
   SET_BIT( ch->arena, AFF2_DM );
   subtract_stock( ch, 300 );

   act("You say, '#yLegendary Dragon, entrust your powers to us... Mega Flare!#n'",ch,NULL,NULL,TO_CHAR);
   act("$n says, '#yLegendary Dragon, entrust your powers to us... Mega Flare!#n'",ch,NULL,NULL,TO_ROOM);
   act("The sky goes dark as thunderstorms fill the horizon.",ch,NULL,NULL,TO_CHAR);
   act("The sky goes dark as thunderstorms fill the horizon.",ch,NULL,NULL,TO_ROOM);
   act("A shadowy figure flies on the edge of the clouds, then dives back into its misty protection.",ch,NULL,NULL,TO_CHAR);
   act("A shadowy figure flies on the edge of the clouds, then dives back into its misty protection.",ch,NULL,NULL,TO_ROOM);
   act("Suddenly, the clouds parted as a huge dragon zooms straight towards the earth!",ch,NULL,NULL,TO_CHAR);
   act("Suddenly, the clouds parted as a huge dragon zooms straight towards the earth!",ch,NULL,NULL,TO_ROOM);
   act("At the last second, the dragon stops it downwards path by hovering over you.",ch,NULL,NULL,TO_CHAR);
   act("At the last second, the dragon stops it downwards path by hovering over you.",ch,NULL,NULL,TO_ROOM);
    
   for ( count = 1; count <= 3; count++ )
   {
      act("Bahamut releases a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_CHAR);
      act("Bahamut releases a fireball which SmAsHeS tHe ArEa ViOlEnTlY!!!",ch,NULL,NULL,TO_ROOM);
   }

   act("Bahamut draws in it breath as a manifest of energy forms on its mouth.",ch,NULL,NULL,TO_CHAR);
   act("Bahamut draws in it breath as a manifest of energy forms on its mouth.",ch,NULL,NULL,TO_ROOM);
   act("The mass of energy grows bigger as Bahamut struggles to hold it in place.",ch,NULL,NULL,TO_CHAR);
   act("The mass of energy grows bigger as Bahamut struggles to hold it in place.",ch,NULL,NULL,TO_ROOM);
   act("Unbelievable energy emits of Bahamut as blinding light fills the room.",ch,NULL,NULL,TO_CHAR);
   act("Unbelievable energy emits of Bahamut as blinding light fills the room.",ch,NULL,NULL,TO_ROOM);
   act("Bahamut releases the huge manifest of energy as it ROARS TOWARDS THE GROUND!",ch,NULL,NULL,TO_CHAR);
   act("Bahamut releases the huge manifest of energy as it ROARS TOWARDS THE GROUND!",ch,NULL,NULL,TO_ROOM);
   act("A HUGE EXPLOSION DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!",ch,NULL,NULL,TO_CHAR);
   act("A HUGE EXPLOSION DEVASTATES the vicinity as the ground TREMBLES IN AGONY, SPLITTING IN HALF!!!",ch,NULL,NULL,TO_ROOM);
 
   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
     vch_next = vch->next_in_room;

     if ( is_safe(ch, vch) )
        continue;

     if ( !IS_AFFECTED( vch, AFF_FLAMING ) ) 
        SET_BIT( vch->affected_by, AFF_FLAMING );

     act("Your body evaporates as the hungry flames SCORCHES your defenseless body!",vch,NULL,NULL,TO_CHAR);
     act("$n's body evaporates as the hungry flames SCORCHES $s defenseless body!",vch,NULL,NULL,TO_ROOM);

     ch->attack_type = ATTACK_MAGICAL;
     ch->attack_var = SOUL;
     ch->attack_focus = AERIAL;
     hurt_person( ch, vch, 3500000, TRUE );
  }
}
