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

void do_ereshkigal( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   AFFECT_DATA af;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS( ch, CLASS_NECROMANCER ) ) 
      {
         stc("Huh?\n\r", ch );
         return;
      }

   if ( ch->pkill < 30 || ch->tier < 5 )
      {
         send_to_char("#eOpening the #Pdamned #obook #eat this stage #R#zWILL#n #ekill you.#n\n\r",ch);
         return;
      }

   if ( ch->alignment > -750 )
      {
         stc("#eOnly the hands of #Revil #ecan use this forbidden weapon.#n\n\r",ch);
         return;
      }

   if ( ch->throw_timer > 0 )
      {
         stc("You are still recovering from the last one.\n\r",ch);
         return;
      }

   ch->wait = UMAX( 0, ( 18 - get_curr_agi( ch ) / 2 ) );
   ch->throw_timer = 50;

   act( "You tighten the grip around your book as the room is suddenly fill with darkness.", ch, NULL, NULL, TO_CHAR );
   act( "$n tightens the grip around $s book as the room is suddenly fill with darkness.", ch, NULL, NULL, TO_ROOM );
   act( "Finding yourself admist the depths of hell, you hear loud screaming and pleas.", ch, NULL, NULL, TO_CHAR );
   act( "Finding yourself admist the depths of hell, you hear loud screaming and pleas.", ch, NULL, NULL, TO_ROOM );
   act( "You then clench your fist as your book starts pulsing with demonic power.", ch, NULL, NULL, TO_CHAR );
   act( "$n then clenches $s fist as $s book starts pulsing with demonic power.", ch, NULL, NULL, TO_ROOM );
   act( "You then spread your fingers as balls of black matter manifests on them.", ch, NULL, NULL, TO_CHAR );
   act( "$n then spreads $s fingers as balls of black matter manifests on them.", ch, NULL, NULL, TO_ROOM );
   act( "You sneer '#eDarkness shall ROCK you!#n' as you release the mass of energy!", ch, NULL, NULL, TO_CHAR );
   act( "$n sneers '#eDarkness shall ROCK you!#n' as $e releases the mass of energy!", ch, NULL, NULL, TO_ROOM );
   act( "Sparks of dark lightning flashes from the ball as it levitates into the air.", ch, NULL, NULL, TO_CHAR );
   act( "Sparks of dark lightning flashes from the ball as it levitates into the air.", ch, NULL, NULL, TO_ROOM );

   SET_BIT( ch->arena, AFF2_DM );

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( is_safe( ch, vch ) )
         continue;

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_STR;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_INT;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_WIS;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_DEX;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_EVA;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_CON;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      af.type      = skill_lookup("weaken");
      af.duration  = 20;
      af.location  = APPLY_AGI;
      af.modifier  = -10;
      af.bitvector = 0;
      affect_to_char( vch, &af );

      act( "#PYour #eare suddenly #wstruck #eby a #Rblast #eof #CFeRoCiOuS #y#zlightning#n#e!!!#n", vch, NULL, NULL, TO_CHAR );
      act( "#P$n #eis suddenly #wstruck #eby a #Rblast #eof #CFeRoCiOuS #y#zlightning#n#e!!!#n", vch, NULL, NULL, TO_ROOM );

      ch->ele_attack = ATTACK_DARK;
      ch->attack_type = ATTACK_MAGICAL;
      ch->attack_var = BLAST;
      ch->attack_focus = AERIAL;
      hurt_person( ch, vch, 3000000, TRUE );
   }

   weather_info.sky = SKY_ECLIPSE;
}

void do_binding_agony( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS(ch, CLASS_NECROMANCER) || ch->clannum != 1 )
       {
	    send_to_char("Huh?\n\r",ch);
	    return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
       {
          send_to_char("They are not here.\n\r", ch);
          return;
       }

    if ( is_safe(ch,victim) )
       return;

    if ( IS_NPC( victim ) )
       {
          send_to_char("You can only do this on players.\n\r", ch);
          return;
       }

    act("You release your soul as it makes a spiritual link with $N's body.",ch,NULL,victim,TO_CHAR);
    act("$n releases $s soul as it makes a spiritual link with your body.",ch,NULL,victim,TO_VICT);
    ch->binding = victim;
    victim->binded = ch;
}

void do_shadowgrasp( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);
    bool nosafe = TRUE;

    if ( IS_NPC(ch) )
       return;

    if ( !IS_CLASS( ch, CLASS_NECROMANCER ) && !IS_SET( ch->cmbt[7], SKL_GRASP ) )
       {
	   send_to_char("Huh?\n\r",ch);
	   return;
       }

    if ( (get_age(ch)-17) < 2 )
       {
          stc("But you are a newbie.\n\r",ch);
          return;
       }

    if ( arg[0] == '\0' )
       {
	    send_to_char( "Rip who from where?\n\r", ch );
	    return;
       }

    if ( ( victim = get_char_area( ch, arg ) ) == NULL )
       {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
       }
 
    if ( (get_age(victim)-17) < 2 )
       {
          stc("But they are a newbie.\n\r",ch);
          return;
       }

    if ( IS_AFFECTED(ch,AFF_SHADOWPLANE) || IS_AFFECTED(ch,AFF_ETHEREAL) )
       {
          send_to_char("You can't do this attack from the NetherRealms.\n\r",ch);
          return;
       }  

    if ( !IS_AFFECTED(victim,AFF_SHADOWPLANE) && !IS_AFFECTED(victim,AFF_ETHEREAL) )
       {
          send_to_char("They aren't in the NetherRealms.\n\r",ch);
          return;
       }  

    if ( victim->fight_timer > 0 )
       {
          send_to_char("They have a fight timer.\n\r",ch);
          return;
       }

    if ( IS_AFFECTED(victim,AFF_SHADOWPLANE) )
       SET_BIT(ch->affected_by, AFF_SHADOWPLANE);

    if ( IS_AFFECTED(victim,AFF_ETHEREAL) )
       SET_BIT(ch->affected_by, AFF_ETHEREAL);

    if ( is_safe( ch, victim ) )
       nosafe = FALSE;

    if ( IS_AFFECTED(ch,AFF_ETHEREAL) )
       REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

    if ( IS_AFFECTED(ch,AFF_SHADOWPLANE) )
       REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);

    if ( !nosafe )
       return;

    if ( IS_AFFECTED(victim,AFF_SHADOWPLANE) )
       REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);

    if ( IS_AFFECTED(victim,AFF_ETHEREAL) )
       REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);

    act("$n suddenly vanishes from sight!.",victim,NULL,NULL,TO_ROOM);
    stop_fighting( victim, TRUE );

    char_from_room(victim);
    char_to_room(victim,ch->in_room);

    act("Grinning evilly, you rip $N from the Shadows disrupting his body.\n\r",ch,NULL,victim,TO_CHAR);
    act("Suddenly, the room goes dark as a force rips you through the shadows!\n\r",ch,NULL,victim,TO_VICT);
    act("Suddenly, the room goes dark as $n rips $N from the Realm of the Shadows!\n\r",ch,NULL,victim,TO_NOTVICT);
    do_look(victim, "auto");

    victim->agg = 100;
    victim->wait = 18;
    update_pos(victim);
}

void do_reduce( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg[MIL];
  
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !IS_CLASS(ch,CLASS_NECROMANCER) )
     return;

  if ( arg[0] == '\0' )
     {
        stc("Which corpse's weight do you want to lower?\n\r",ch);
        return;
     }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
     {
        stc("You are not carrying that object.\n\r",ch);
        return;
     }

  if ( obj->item_type != ITEM_CORPSE_NPC )
     {
        send_to_char("You can only lower the weight of corpses in your inventory.\n\r",ch);
        return;
     }

  if ( obj->weight <= 1 )
     {
        stc("That corpse is already as light as possible!\n\r",ch);
        return;
     }

  ch->carry_weight = UMAX( ch->carry_weight - obj->weight + 1, 0 );
  obj->weight = 1;
  act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
  act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );
  act("You rearrange $p's pattern, lowering it's weight.",ch,obj,NULL,TO_CHAR);
  act("$n's eyes glow dark grey.",ch,NULL,NULL,TO_ROOM);
  WAIT_STATE(ch, 8);
}

void do_seek( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );
 
   if ( IS_NPC(ch) )
      return;

   if ( !IS_CLASS(ch,CLASS_NECROMANCER) )
      return;

   if ( ch->ctimer[1] > 0 )
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
         if ( ( victim = get_char_world( ch, arg ) ) == NULL )
            {
               send_to_char( "They aren't here.\n\r", ch );
               return;
            }
      }
   
   if ( is_safe( ch, victim ) )
      return;

   if ( victim->fight_timer == 0 )
      {
         stc( "#eThe zombies can't focus on someone's lifeforce unless they have a #wfight timer#e.#n\n\r", ch );
         return;
      }

   if ( ch->in_room == victim->in_room )
      {
         send_to_char( "They are already near you.\n\r", ch );
         return;
      }

   ch->mana -= 500000;
   ch->ctimer[1] = 4;

   act("#eSensing #w$N#w's #efrantic, you sneer as you send zombies after $M.#n",ch,NULL,victim,TO_CHAR);

   act("You heard a rumbling sound behind you as a group of zombies arrive in the area.",victim,NULL,NULL,TO_CHAR);
   act("As you strike out at them, they eventually pin you down on the ground.",victim,NULL,NULL,TO_CHAR);
   act("#oSuddenly, they drag you away from the vicinity!#n",victim,NULL,NULL,TO_ROOM);
   act("#oSuddenly, a bunch of ravenous zombies drag $n #oaway from the vicinity!#n",victim,NULL,NULL,TO_ROOM);

   stop_fighting( victim, TRUE );
   char_from_room( victim );
   char_to_room( victim, ch->in_room );

   act("Done with their work, the zombies drop you on the ground and go back to their master!",victim,NULL,NULL,TO_CHAR);
   act("Done with their work, the zombies drop $n on the ground and go back to their master!",victim,NULL,NULL,TO_ROOM);
   victim->wait = 18;
}

void do_necro_drain( CHAR_DATA *ch, char *argument )
{
  int cost = 300000;
  
  if ( IS_NPC(ch) ) 
     return;

  if ( !IS_CLASS(ch,CLASS_NECROMANCER) )
     return;

  act("You drain the lifeforce from the earth around you causing it to rumble.",ch,NULL,NULL,TO_CHAR);
  act("$n drains the lifeforce from the earth around you causing it to rumble.",ch,NULL,NULL,TO_ROOM);

  hit_gain( ch, cost );
  mana_gain( ch, cost );
  ch->wait = 12;
}

void do_calling(CHAR_DATA *ch, char *argument)
{
    char buf2[MAX_STRING_LENGTH]; 
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int max = 0;

    if ( IS_NPC( ch ) )
       return;

    if ( !IS_CLASS(ch, CLASS_NECROMANCER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 15 )
       {
          send_to_char("#eYou aren't powerful enough to control this #Rvile #ycreature #efrom hell as yet.#n\n\r",ch);
          return;
       }

    if ( ch->pcdata->followers > max )
       {
          send_to_char("The great demon refuses to be beckoned after mere zombies.\n\r",ch);
          return;
       }

    act( "You mumble in the ancient language as you concentrate fiercely.", ch, NULL, NULL, TO_CHAR );
    act( "$n mumbles in an ancient language as $e concentrates fiercely.", ch, NULL, NULL, TO_ROOM );

    victim=create_mobile( get_mob_index( 23 ) );
    sprintf(buf,"Nydul, the Devil");
    sprintf(buf2,"Nydul, a powerful demoness from the netherworld, stands here.\n\r");
    free_string(victim->short_descr);
    victim->short_descr = str_dup(buf);
    free_string(victim->name);
    victim->name = str_dup(buf);
    free_string(victim->long_descr);
    victim->long_descr= str_dup(buf2);

    char_to_room( victim, ch->in_room );

    act("You say, '#yGreat Demon, come and obey me!#n",ch,NULL,NULL,TO_CHAR);
    act("$n says, '#yGreat Demon, come and obey me!#n",ch,NULL,NULL,TO_ROOM);

    if ( !IS_SET(victim->extra, EXTRA_ZOMBIE) ) 
       SET_BIT(victim->extra, EXTRA_ZOMBIE); 

    if ( IS_SET( ch->cmbt[7], SKL_KAKUSU ) )
       SET_BIT( victim->act, PLR_WIZINVIS );

    sprintf(buf, "%s steps out of a shimmering portal and snarls.\n\r",victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
 
    add_follower( victim, ch );
    ch->pcdata->followers++;

    af.type      = skill_lookup("charm person");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    af.type      = skill_lookup("protection from evil");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PROTECT;
    affect_to_char( victim, &af );

    af.type      = skill_lookup("dark shield");
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    victim->special = 0;
    victim->level = 999;

    SET_BIT( victim->itemaffect, ITEMA_RESISTANCE );
    SET_BIT( victim->act, ACT_NINJA );
    SET_BIT( victim->itemaffect, ITEMA_SPEED );

    victim->hitroll = number_range( 2500000, 2500000 + UMIN( 500000, ( ch->pkill - 15 ) * 100000 ) );
    victim->damroll = number_range( 2500000, 2500000 + UMIN( 500000, ( ch->pkill - 15 ) * 100000 ) );

    if ( IS_SET( ch->cmbt[7], SKL_HITRATE ) )
       SET_BIT( victim->cmbt[7], SKL_HITRATE );

    if ( IS_SET( ch->cmbt[7], SKL_CRITICAL ) )
       SET_BIT( victim->cmbt[7], SKL_CRITICAL );

    if ( IS_SET( ch->cmbt[7], SKL_RETORT ) )
       SET_BIT( victim->cmbt[7], SKL_RETORT );

    if ( IS_SET( ch->cmbt[7], SKL_DODGE ) )
       SET_BIT( victim->cmbt[7], SKL_DODGE );

    victim->hit = victim->max_hit = ch->max_hit;

    if ( IS_AFFECTED( ch, AFF_ETHEREAL ) )
       SET_BIT( victim->affected_by, AFF_ETHEREAL );

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
       SET_BIT( victim->affected_by, AFF_SHADOWPLANE );
 
    victim->armor = -75000;
    do_group( ch, victim->name );
}

void do_reanimate( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_NECROMANCER) )
       {
          send_to_char("Huh?\n\r",ch);
          return;
       }

    if ( ch->pkill < 20 )
       {
          send_to_char("You haven't mastered the dark magicks as yet.\n\r",ch);
          return;
       }

    if ( !IS_SET( ch->loc_hp[0], LOST_HEAD ) || ch->pcdata->chobj == NULL )
       {
          send_to_char("#eYou can only do this spell from the #ograve#e.#n\n\r",ch);
          return;
       }

    if ( ch->mana < 5000000 )
       {
          send_to_char("You don't have enough magical energy to cast this spell.\n\r",ch);
          return;
       }

    REMOVE_BIT( ch->loc_hp[0], LOST_HEAD );
    REMOVE_BIT( ch->affected_by, AFF_POLYMORPH );
    free_string( ch->morph );
    ch->morph = str_dup("");
    ch->pcdata->capture_timer = 0;
    obj = ch->pcdata->chobj;
    ch->pcdata->chobj = NULL;
    extract_obj( obj );
    ch->position = POS_STANDING;
    ch->hit = 2500000;

    act( "Shards of dark energy radiates of your corpse as you cackle with glee.", ch, NULL, NULL, TO_CHAR );
    act( "Shards of dark energy radiates of $n's corpse as $e cackles with glee.", ch, NULL, NULL, TO_ROOM );
    act( "Struggling with all of your energy, you suddenly go back on your knees.", ch, NULL, NULL, TO_CHAR );
    act( "Struggling with all of $s energy, $n suddenly go back on $s knees.", ch, NULL, NULL, TO_ROOM );
    act( "Picking up your decapitated head, you snap it onto your body with a grin.", ch, NULL, NULL, TO_CHAR );
    act( "Picking up $s decapitated head, $n snaps it onto $s body with a grin.", ch, NULL, NULL, TO_ROOM );
    act( "Zombies from the vicinity starts swarming towards you as the hunt begins again...", ch, NULL, NULL, TO_CHAR );
    act( "Zombies from the vicinity starts swarming towards $n as the hunt begins again...", ch, NULL, NULL, TO_ROOM );
}

void do_gas_cloud( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_NECROMANCER) )
       {
           send_to_char("Huh?\n\r",ch);
	     return;
       }

    if ( ch->pkill < 10 )
       {
          send_to_char("You haven't mastered the dark magicks as yet.\n\r",ch);
          return;
       }

    if ( ch->ctimer[0] > 0 )
       {
           send_to_char("You are still recovering from the last one.\n\r",ch);
	     return;
       }

    if ( ch->mana < 2000000 )
       {
          send_to_char("You don't have enough magical energy for that.\n\r",ch);
          return;
       }

    ch->mana -= 2000000;
    ch->ctimer[0] = 4;
    ch->wait = 4;

    act("You spread your fingers releasing a puff of poisonous gas within the atmosphere.",ch,NULL,NULL,TO_CHAR);
    act("$n spreads $s fingers releasing a puff of poisonous gas within the atmosphere.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if ( is_same_group(ch,vch) ) 
           continue;

        if ( IS_NPC(vch) && IS_SET(vch->extra, EXTRA_ZOMBIE) ) 
           continue;

        if ( !is_safe(ch, vch) && vch != ch )
	     {
              act("You cough violently as the deadly fumes choke the air out of you.",vch,NULL,NULL,TO_CHAR);
              act("$n coughs violently as the deadly fumes chokes the air out of $m.",vch,NULL,NULL,TO_ROOM);

              if ( !IS_SET( vch->more, MORE_POISON ) ) 
                 SET_BIT( vch->more, MORE_POISON );

              if ( IS_CLASS( vch, CLASS_DUERGAR ) )
                 {
                    vch->hit = UMIN( vch->max_hit, vch->hit + ( 1500000 * UMAX( 1, vch->pkill / 10 ) ) );
                    continue;
                 }

              ch->attack_type = ATTACK_MAGICAL;
              ch->attack_var = HIT;
              ch->attack_focus = AERIAL;

              if ( ch->class_2 != 0 )
                 hurt_person( ch, vch, 500000, TRUE );
                 else hurt_person( ch, vch, 1000000, TRUE );
	     }
    }
}

void do_madness( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if ( IS_NPC(ch) ) 
       return;

    if ( !IS_CLASS(ch, CLASS_NECROMANCER) )
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
    subtract_stock( ch, 300 );
    SET_BIT( ch->arena, AFF2_DM );

    act("You start cackling with glee as you tilt your hands together.",ch,NULL,NULL,TO_CHAR);
    act("$n starts cackling with glee as $e tilt $s hands together.",ch,NULL,NULL,TO_ROOM);
    act("You then release a demonic pulse of energy as it slowly floats into the air.",ch,NULL,NULL,TO_CHAR);
    act("$n then releases a demonic pulse of energy as it slowly floats into the air.",ch,NULL,NULL,TO_ROOM);
    act("Suddenly, the ball becomes clear showing all a haunting scene...",ch,NULL,NULL,TO_CHAR);
    act("Suddenly, the ball becomes clear showing all a haunting scene...",ch,NULL,NULL,TO_ROOM);
    act("You see a young child rocking herself silently on a chair.",ch,NULL,NULL,TO_CHAR);
    act("You see a young child rocking herself silently on a chair.",ch,NULL,NULL,TO_ROOM);
    act("On her lap rests a very old and scruffy teddy bear, with its head barely hanging on.",ch,NULL,NULL,TO_CHAR);
    act("On her lap rests a very old and scruffy teddy bear, with its head barely hanging on.",ch,NULL,NULL,TO_ROOM);
    act("The bear's eyes grow red with the purest evil as the girl CACKLES LOUDLY!!!",ch,NULL,NULL,TO_CHAR);
    act("The bear's eyes grow red with the purest evil as the girl CACKLES LOUDLY!!!",ch,NULL,NULL,TO_ROOM);
    act("You collapse terrified on your knees as you realize the source of all evil.",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;

       if ( IS_CLASS( vch, CLASS_DEMON ) || is_safe( ch, vch ) )
          continue;

       ch->ele_attack = ATTACK_DARK;
       ch->attack_var = SOUL;
       ch->attack_focus = DIRECT;
       ch->attack_type = ATTACK_MAGICAL;

       switch( number_range( 1, 4 ) )
       {
          case 1: act( "#PYou #ecover your eyes in #Rterror #eas #wEVERYTHING #elooks like a replica of you!#n", vch, NULL, NULL, TO_CHAR );
                  act( "#P$n #ecovers $s eyes in #Rterror #eas #wEVERYTHING #elooks like a replica of $m!#n", vch, NULL, NULL, TO_ROOM );
                  vch->seeself = 6;
                  hurt_person( ch, vch, 3000000, TRUE );
                  break;

          case 2: act( "#PYou #eopen your eyes widely in #Rsurprise #eas the laws of #PPAIN #ehas been #wSWITCHED #efor you!#n", vch, NULL, NULL, TO_CHAR );
                  act( "#P$n #eopens $s eyes widely in #Rsurprise #eas the laws of #PPAIN #ehas been #wSWITCHED #efor $m!#n", vch, NULL, NULL, TO_ROOM );
                  hurt_person( ch, vch, 3000000, TRUE );

                  if ( vch == NULL || vch->in_room == NULL )
                     continue;

                  vch->revdam = 6;
                  break;

          case 3: act( "#PYou #escream in #Rshock #eas all of your armor and protection #wMELTS#e away into #gACID#e!#n", vch, NULL, NULL, TO_CHAR );
                  act( "#P$n #escreams in #Rshock #eas all of $s armor and protection #wMELTS#e away into #gACID#e!#n", vch, NULL, NULL, TO_ROOM );
                  vch->noprotection = 4;
                  hurt_person( ch, vch, 3000000, TRUE );
                  break;

          case 4: act( "#PYou #eopen your eyes widely in #Rsurprise #eas the laws of #yREGEN #ehas been #wSWITCHED #efor you!#n", vch, NULL, NULL, TO_CHAR );
                  act( "#P$n #eopens $s eyes widely in #Rsurprise #eas the laws of #yREGEN #ehas been #wSWITCHED #efor $m!#n", vch, NULL, NULL, TO_ROOM );
                  hurt_person( ch, vch, 3000000, TRUE );

                  if ( vch == NULL || vch->in_room == NULL )
                     continue;

                  vch->revregen = 20;
                  vch->agg = 0;
                  stop_fighting( vch, TRUE );
                  break;
       }
    }

    act("As you glance behind you, you see a sihoullete of a deity grinning from the shadows...",ch,NULL,NULL,TO_CHAR);
    act("As you glance behind you, you see a sihoullete of a deity grinning from the shadows...",ch,NULL,NULL,TO_ROOM);
}

void do_corpse_explosion( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;

   if ( IS_NPC(ch) ) 
      return;

   if ( !IS_CLASS(ch, CLASS_NECROMANCER) )
      {
         send_to_char("Huh?\n\r",ch);
         return;
      }

   if ( get_stock( ch ) < 100 )
      {
         stc( "#wYou don't have enough #ystock #wfor that #Ctechnique #wwarrior.#n\n\r", ch );
         return;
      }

   ch->wait = 12;
   subtract_stock( ch, 100 );
   SET_BIT( ch->arena, AFF2_DM );

   act("You point at the corpse beneath your feet releasing its stored energies.",ch,NULL,NULL,TO_CHAR);
   act("$n points at the corpse beneath $s feet releasing its stored energies.",ch,NULL,NULL,TO_ROOM);

   for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
   {
      vch_next = vch->next_in_room;

      if ( IS_NPC(vch) && IS_SET(vch->extra, EXTRA_ZOMBIE) ) 
         continue;

      if ( is_safe(ch,vch) ) 
         continue;

      act("The explosion sears through your mortal flesh.",vch,NULL,NULL,TO_CHAR);
      act("The explosion sears through $n's mortal flesh.",vch,NULL,NULL,TO_ROOM);
      ch->attack_type = ATTACK_MAGICAL;
      ch->ele_attack = ATTACK_THUNDER;
      ch->attack_var = BLAST;
      ch->attack_focus = UPPERCUT;
      hurt_person( ch, vch, 1000000, TRUE );
   }
}
