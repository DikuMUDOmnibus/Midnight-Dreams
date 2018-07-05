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

void do_mechatalent( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
	 argument = one_argument( argument, arg1 );
	 argument = one_argument( argument, arg2 );

	if (IS_NPC(ch)) return;

	 if (!IS_CLASS(ch, CLASS_MECHA))
	 {
	send_to_char("Huh?\n\r",ch);
	return;
	 }

	 if (arg1[0] == '\0' && arg2[0] == '\0')
	 {
	sprintf(buf,"Mech Talents:  Tactics (%d), Pscionics (%d), Reflex (%d), Combat (%d), Piloting (%d).\n\r",
	ch->pcdata->powers[MPOWER_TACTICS], ch->pcdata->powers[MPOWER_PSCIONICS],
	ch->pcdata->powers[MPOWER_REFLEX], ch->pcdata->powers[MPOWER_COMBAT], ch->pcdata->powers[MPOWER_PILOTING]);
	send_to_char(buf,ch);
	return;
 }
	 if (arg2[0] == '\0')
	 {
	if (!str_cmp(arg1,"tactics"))
	{
		 send_to_char("Tactics - Forms of attacks, formation, executions, and Approcahes.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_TACTICS] < 1)
		send_to_char("You Havent Learned any tactics at the academy.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_TACTICS] > 0)
		send_to_char("Radar <Scry> Locating your enemies upon the Radar Screen.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_TACTICS] > 1)
		send_to_char("JumpJets <Flying> The ability to Launch oneself across the Battlefield.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_TACTICS] > 2)
		send_to_char("MechaRush <Bash> You activate the jump jets in a short Forward burst propeling you at velocitys uncomprehendable to the enemy.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_TACTICS] > 3)
		send_to_char("clusterbomb <multimissile> for every 4 points of status one bomb.\n\r",ch);
		if (ch->pcdata->powers[MPOWER_TACTICS] > 4)
		send_to_char("NightVision <Truesight> Activate torso display for nightvision.\n\r", ch);
		if (ch->pcdata->powers[MPOWER_TACTICS] > 5)
		send_to_char("Concusion Grenade <Blindness> You Launch a Concussion Grenade Destroying the Enemys Senses.\n\r", ch);
		 return;
	}

	else if (!str_cmp(arg1,"pscionics"))
	{
		 send_to_char("Pscionics- Mental and Physical Enhancements to increase Lethality with Mecha.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PSCIONICS] < 1)
		send_to_char("You havent Learned any Mental skills or gained any implants from the Academy!.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PSCIONICS] > 0)
		send_to_char("MechaJack <+attacks> You Insert your thought process into mecha through a psionic Implant Increasing you Mobility.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PSCIONICS] > 1)
		send_to_char("Psychic-Eng <Damage> You use your Psychi To Drive your Mechas Bullets Deeper and Faster and more efficiently into the enemy.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PSCIONICS] > 2)
		send_to_char("Psychicblast <Psycho Ball> You unleash A flury Of Pscionic Waves utterly destroying all the nerves in your enemy.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PSCIONICS] > 3)
		send_to_char("Pscionicimpulse -charm- Allows you to Control your enemys Thought Process Through Enchanting Psycho-impulses.\n\r",ch);
		if (ch->pcdata->powers[MPOWER_PSCIONICS] > 4)
		send_to_char("PscionicScream -Rage- You Go Insane Thrashing about and become stronger than ever imagined.\n\r", ch);
		 return;
	}
	else if (!str_cmp(arg1,"reflex"))
	{
		 send_to_char("Reflex - How you react in Battle.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_REFLEX] < 1)
		send_to_char("The academy hasn't taught you had to analyze your reflexis yet.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_REFLEX] > 0)
		send_to_char("Crouch -Dodge- You insinctively Crouch as much as possible to dodge attacks.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_REFLEX] > 1)
		send_to_char("Step-in -Auto-Attack- You step inside your opponents attack as he lashes out and deal a powerful blow.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_REFLEX] > 2)
		send_to_char("lowblow -Flee- You Swing At Your Opponents Groin Knocing Him down stunned so you can flee .\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_REFLEX] > 3)
		send_to_char("Gyro-Shot -no-stun- As you are hit down to the ground stunned, You shoot The Gyro Fluid Bringing You back to your feet in an instant.\n\r",ch);
		if (ch->pcdata->powers[MPOWER_REFLEX] > 4)
		send_to_char("MechaRoll -Dodge Decap- Youre mecha overrides your commands and rolls out of the way to prevent decapitation\n\r", ch);
		 return;
	}
	else if (!str_cmp(arg1,"combat"))
	{
		 send_to_char("Combat - Your Battling Skills.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_COMBAT] < 1)
		send_to_char("The academy hasn't taught you how to fight in a Mecha yet.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_COMBAT] > 0)
		send_to_char("Stafe -Dodge- You move sideways with your JumpJets to dodge your opponents move.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_COMBAT] > 1)
		send_to_char("uppercut -Attack- You Release A barrage of Uppercuts Slicing with Your Vibro Blade.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_COMBAT] > 2)
		send_to_char("Box -brawl- You Rip off the opponents Weapons and drop yours to fight them hand to hand .\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_COMBAT] > 3)
		send_to_char("pressure -Bearhug- You Pull your Opponent Into an Overwhelming Pressure Bearhug Draining All his strength and Energy With Great amounts of yours.\n\r",ch);
		if (ch->pcdata->powers[MPOWER_COMBAT] > 4)
		send_to_char("kamikazi -Final Attack- With no option left of survival you Draw Your Vibro Blade And Scream In Terror as you leap upon your enemy Snapping Bones, ripping Flesh, Biting, clawing, and worse.\n\r", ch);
		 return;
	}
 else if (!str_cmp(arg1,"piloting"))
	{
	 send_to_char("Piloting - Maintenence and Control of the Mecha.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PILOTING] < 1)
		send_to_char("The academy hasn't taught you how to use your mech yet.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PILOTING] > 0)
		send_to_char("Repair -Heal- You Repair your mech making it as good as new!!!!\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PILOTING] > 1)
		send_to_char("engineer -Mecha-making- The creation of your Mecha parts.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PILOTING] > 2)
		send_to_char("Flight -goto- You launch your Wings And Pilot your mech to your enemy through radar .\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PILOTING] > 3)
		send_to_char("MechaLove -auto-attack- Your mecha AI starts to attract to You And starts Helping You in battle while you do other things.\n\r",ch);
		 if (ch->pcdata->powers[MPOWER_PILOTING] > 4)
		send_to_char("Burst -Attack- You Release Bursts of ammo from your mecha into your enemy\n\r", ch);
		 return;
	}
  sprintf(buf,"Mech Talents:  Tactics (%d), Pscionics (%d), Reflex (%d), Combat (%d), Piloting (%d).\n\r",
	ch->pcdata->powers[MPOWER_TACTICS], ch->pcdata->powers[MPOWER_PSCIONICS],
	ch->pcdata->powers[MPOWER_REFLEX], ch->pcdata->powers[MPOWER_COMBAT], ch->pcdata->powers[MPOWER_PILOTING]);
	send_to_char(buf,ch);
	return;
	 }
	 if (!str_cmp(arg2,"improve"))
	 {
	int improve;
	int cost;
	int max;

		  if (!str_cmp(arg1,"tactics"   )) {improve = MPOWER_TACTICS;   max=6;}
	else if (!str_cmp(arg1,"pscionics"   )) {improve = MPOWER_PSCIONICS;   max=5;}
	else if (!str_cmp(arg1,"reflex"   )) {improve = MPOWER_REFLEX;   max=5;}
	else if (!str_cmp(arg1,"combat"   )) {improve = MPOWER_COMBAT;   max=5;}
	else if (!str_cmp(arg1,"piloting" )) {improve = MPOWER_PILOTING; max=5;}
	else
	{
		 send_to_char("Principles: Tactics, Pscionics, Reflex, Combat, Piloting.\n\r",ch);
		 return;
	}
	cost = (ch->pcdata->powers[improve]+1) * 18;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
		 sprintf(buf,"You have already take all the %s courses.\n\r", arg1);
		 send_to_char(buf,ch);
		 return;
	}
	if ( cost > ch->practice )
	{
		 sprintf(buf,"It costs you %d primal to Take the %s course.\n\r", cost, arg1);
		 send_to_char(buf,ch);
		 return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You finish a semester of %s.\n\r", arg1);
	send_to_char(buf,ch);
	 }
	 else send_to_char("To take a course, type: mechatalent <principle type> improve.\n\r",ch);
	return;
 }

 void do_pscionic_scream(  CHAR_DATA *ch, char *argument  )
 {
	 if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_MECHA) )
	{
	send_to_char("Huh?\n\r", ch);
	return;
 }
	if ( ch->rage >= 50 )
 {
		 send_to_char("You Already have begun!!\n\r",ch);
		 return;
	}

	if ( ch->move < 2000 )
	{
		send_to_char("You Dont Have Enough energy!.\n\r", ch);
		return;
	}
	send_to_char("You let out an unearthly Scream and Thrash about charging everyone.\n\r",ch);
	act("$n Lets out an Unearthly scream Through his suit knocking you back as theyre mecha starts to thrash about.",ch,NULL,NULL,TO_ROOM);
	ch->rage += 400;
	ch->move -= 1000;
	WAIT_STATE(ch,12);
	return;
	 return;
}



void do_academy( CHAR_DATA *ch, char *argument )

{

	 CHAR_DATA *victim;

	 char      arg [MAX_INPUT_LENGTH];



	 argument = one_argument( argument, arg );



	 if (IS_NPC(ch)) return;



	if (!IS_CLASS(ch, CLASS_MECHA) )

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}



	 if (IS_CLASS(ch, CLASS_MECHA) && !IS_SET(ch->special, SPC_PRINCE)

		  && ch->generation != 1

		  && ch->generation != 2 )

	 {

	send_to_char("You arent a veteran!.\n\r",ch);

	return;

	 }



	 if ( arg[0] == '\0' )

	 {

	send_to_char( "Take who to the academy?\n\r", ch );

	return;

	 }



	 if ( ( victim = get_char_room( ch, arg ) ) == NULL )

	 {

	send_to_char( "They aren't here.\n\r", ch );

	return;

	 }



	 if ( IS_NPC(victim) )

	 {

	send_to_char( "Not on NPC's.\n\r", ch );

	return;

	 }



	 if ( IS_IMMORTAL(victim) )

	 {

	send_to_char( "Not on Immortals's.\n\r", ch );

	return;

	 }



	 if ( ch == victim )

	 {

	send_to_char( "You have already been to the academy.\n\r", ch );

	return;

	 }



	 if (IS_CLASS(victim, CLASS_MECHA))

	 {

	send_to_char( "They have already graduated from the academy.\n\r", ch );

	return;

	 }



	 if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )

	 {

	send_to_char( "You can only teach avatars.\n\r", ch );

	return;

	 }



	 if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))

	 {

	send_to_char( "You are unable to take vampires!\n\r", ch );

	return;

	 }



	 if (IS_CLASS(victim, CLASS_WEREWOLF))

	 {

	send_to_char( "You are unable to take werewolves!\n\r", ch );

	return;

	 }



	 if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))

	 {

	send_to_char( "You are unable to take demons!\n\r", ch );

	return;

	 }





	 if (IS_CLASS(victim, CLASS_HIGHLANDER))

	 {

	send_to_char( "You are unable to take highlanders.\n\r", ch );

	return;

	 }



	 if (IS_CLASS(victim, CLASS_DROW))

	 {

	 send_to_char( "Not on drows!\n\r",ch);

	 return;

	 }



/* When im done with Mechas ill scratch code these fuckers im bored-Bry




	 if (IS_CLASS(victim, CLASS_PALADIN))

	 {

	 send_to_char("Not on paladins!\n\r", ch);

	 return;

	 }

*/
	if (victim->class != 0)
	{
	send_to_char("You cannot take a classed person to the academy!.\n\r", ch);
	return;
	}


	 if (!IS_IMMUNE(victim,IMM_VAMPIRE))

	 {

	send_to_char( "You cannot take an unwilling person to the academy.\n\r", ch );

	return;

	 }



	 if (ch->exp < 100000)

	 {

	send_to_char("You cannot afford the 100000 credits required to pay for gas.\n\r",ch);

	return;

	 }



	 if (victim->exp < 10000)

	 {

	send_to_char("They cannot afford the 10000 Credits to stay at the academy.\n\r",ch);

	return;

	 }



	 ch->exp -= 10000;

	 victim->exp -= 10000;



	 act("you take $N to the academy.", ch, NULL, victim, TO_CHAR);

	 act("$n takes $N to the academy.", ch, NULL, victim, TO_NOTVICT);

	 act("$n takes you to the academy.", ch, NULL, victim, TO_VICT);

	 victim->class    = CLASS_MECHA;

	 victim->level = LEVEL_ARCHMAGE;

	 victim->pcdata->powers[MPOWER_REFLEX] = 0;
	 victim->pcdata->powers[MPOWER_PILOTING] = 0;
	 victim->pcdata->powers[MPOWER_PSCIONICS] = 0;
	 victim->pcdata->powers[MPOWER_COMBAT] = 0;
	 victim->pcdata->powers[MPOWER_PILOTING] = 0;

	 if (victim->trust > 6)

	 victim->trust = victim->trust;

	 else

	 victim->trust = LEVEL_ARCHMAGE;



	 send_to_char( "You are now a licensed MechWarrior!.\n\r", victim );

	 free_string(victim->lord);

	 victim->lord = str_dup(ch->name);

	free_string(victim->clan);

	 victim->clan=str_dup(ch->clan);

	 victim->generation = ch->generation + 2;

	 save_char_obj(ch);

	 save_char_obj(victim);

	 return;

}





void do_infro( CHAR_DATA *ch, char *argument )
{
	 if (IS_NPC(ch)) return;
	if (!IS_CLASS(ch, CLASS_MECHA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
  if (IS_CLASS(ch, CLASS_MECHA) && ch->pcdata->powers[MPOWER_TACTICS] < 5 )

	{

	  send_to_char("You have not taken the 5th course of Tactics.\n\r", ch);

	  return;

	}



	else if (ch->move < 500 )

	{

		send_to_char("You don't have the energy to activate the console.\n\r", ch);

		return;

	}



	 if ( IS_SET(ch->act, PLR_HOLYLIGHT) )

	 {

	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);

	send_to_char( "You flick off the the console switch.\n\r", ch );

	 }

	 else

	 {

	SET_BIT(ch->act, PLR_HOLYLIGHT);

	send_to_char( "You activate the nightvision Console -<|>-.\n\r", ch );

	ch->move -= 500;

	 }



	 return;

}



void do_lowblow( CHAR_DATA *ch, char *argument )

{
	 AFFECT_DATA af;
	 ROOM_INDEX_DATA *was_in;
	 ROOM_INDEX_DATA *now_in;
	 CHAR_DATA *victim;
	 int attempt;
	if (IS_CLASS(ch, CLASS_MECHA) && ch->pcdata->powers[MPOWER_REFLEX] < 3 )
	{
		send_to_char("You have not taken the third semester of Reflex Training.\n\r", ch);
		return;
	}
	else if (!IS_CLASS(ch, CLASS_MECHA) )
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

	if ( IS_NPC(ch) )
		return;
	 if ( ( victim = ch->fighting ) == NULL )
	 {
		if ( ch->position == POS_FIGHTING )
			 ch->position = POS_STANDING;
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
	}
	if(ch->move <= 0)
	{
		send_to_char("Your muscles arent responding!\n\r",ch);
		return;
	}
	if (ch->move < 2500)
	{
		send_to_char("Your Mech doesnt want to listen.\n\r", ch);
		return;
	}
	 was_in = ch->in_room;
	{
		EXIT_DATA *pexit;
		int door;
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
			/* Use escape instead of flee so people know it's the ninja power */
			ch->in_room = was_in;
			act( "$n hits you in the Groin and runs!", ch, NULL, NULL, TO_ROOM );
			af.type = skill_lookup("sleep");
			af.duration = 0;
			af.modifier = 4000;
			af.location = APPLY_AC;
			af.bitvector = AFF_STUNNED;
			affect_to_char(victim, &af);
			ch->in_room = now_in;
			if ( !IS_NPC(ch) )
				send_to_char( "You hit them in the groin and run!!\n\r", ch );
			ch->move -= 2500;
			ch->fight_timer += 5;
			stop_fighting( ch, TRUE );
			return;
		}
	}
//    send_to_char( "BUG: Inform an Implmentor\n\r", ch );
	 return;
}

void do_mechastats( CHAR_DATA *ch, char *argument )

{

	 char buf[MAX_STRING_LENGTH];

	 char arg[MAX_INPUT_LENGTH];

	 CHAR_DATA *gch;



	 one_argument( argument, arg );



	 if (IS_NPC(ch)) return;

	 if (!IS_CLASS(ch, CLASS_MECHA) )

	 {

	send_to_char("Huh?\n\r",ch);

	return;

	 }



	 if (strlen(ch->clan) < 2)

	 {

	send_to_char("But you don't belong to any clan!\n\r",ch);

	return;

	 }



	 sprintf( buf, "%s clan:\n\r", ch->clan );

	 send_to_char( buf, ch );

	 send_to_char("[      Name      ] [ Gen ] [ Hits  % ] [ Mana  % ] [ Move  % ]\n\r", ch );

	 for ( gch = char_list; gch != NULL; gch = gch->next )

	 {

	if ( IS_NPC(gch) ) continue;

	if ( !IS_CLASS(gch, CLASS_NINJA) ) continue;

	if ( !str_cmp(ch->clan,gch->clan) )

	{

		 sprintf( buf,

		 "[%-16s] [  %d  ] [%-6d%3d] [%-6d%3d] [%-6d%3d] [%7d]\n\r",

		capitalize( gch->name ),

		gch->generation,

		gch->hit,  (gch->hit  * 100 / gch->max_hit ),

		gch->mana, (gch->mana * 100 / gch->max_mana),

		gch->move, (gch->move * 100 / gch->max_move),

					  gch->exp);

		send_to_char( buf, ch );

	}

	 }

	 return;

}

void do_concussion_grenade( CHAR_DATA *ch, char *argument )
{
	 AFFECT_DATA af;
	 CHAR_DATA *victim;

	 char      arg1 [MAX_INPUT_LENGTH];

	 argument = one_argument( argument, arg1 );



	 if (IS_NPC(ch)) return;



	if (!IS_CLASS(ch, CLASS_MECHA))

	{

		send_to_char("Huh?\n\r", ch);

		return;

	}

		  else if ( ch->pcdata->powers[MPOWER_TACTICS] < 6 )

	{

		 send_to_char("You have not taken the sixth semester of tactics.\n\r", ch);

		 return;

	}

	if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	 if ( arg1[0] == '\0' )

	 {

	send_to_char( "Throw it at who?\n\r", ch );

	return;

	 }



	 if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )

	 {

	send_to_char( "They aren't here.\n\r", ch );

	return;

	 }


	if (victim->level < 3)
	{
		send_to_char("You may not do this to a mortal.\n\r", ch);
		return;
	}
	 if ( ch == victim )

	 {

	send_to_char( "Not on yourself!\n\r", ch );

	return;

	 }
	if (is_safe(ch, victim))
		return;

	 if ( ch->position == POS_FIGHTING )

	 {

	send_to_char( "Not while fighting!\n\r", ch );

	return;

	 }



	if (ch->move < 1000)

	{

	send_to_char("You don't have enough energy.\n\r", ch);

	return;

	}



	 if ( victim->in_room == ch->in_room )

	{

	act("Pha-Bump is the sound you here as you launch a Concussion Grenade at $N Knocking him senseless.",ch,NULL,victim,TO_CHAR);
//eh sorry but JEEEZUZ coulda got a bit offensive
	act("$n aims his launcher at you and Pha-Bump, 'NOOOOOOO AGGH I CANT SEE WHERE AM I GOING?'.",ch,NULL,victim,TO_VICT);

	af.type = skill_lookup("mecha sleep");
	af.duration = 0;
	af.modifier = 10000;
	af.location = APPLY_AC;
	af.bitvector = AFF_STUNNED;
	affect_to_char(victim, &af);

	ch->move -= 1000;

	WAIT_STATE(ch, 6);

	return;

	}



	return;

}

void do_engineer( CHAR_DATA *ch, char *argument )
{
	 OBJ_INDEX_DATA *pObjIndex;
	 OBJ_DATA *obj;
	 char arg[MAX_INPUT_LENGTH];
	 int vnum = 0;

	 argument = one_argument( argument, arg );

	 if (IS_NPC(ch)) return;

	 if (!IS_CLASS(ch, CLASS_MECHA))
	 {
		 send_to_char("You stare at the tools doing nothing.\n\r",ch);
		 return;
	 }
	 else if ( ch->pcdata->powers[MPOWER_PILOTING] < 2 )
	{
		 send_to_char("You have not taken the second semester of piloting.\n\r", ch);
		 return;
		 }

	 if (arg[0] == '\0')
	 {
		send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
		send_to_char("Vibro Blade, S-M-A Mp5, Torso armor unit, Jump jets, Jk52 hover wings, S-M-A Mecha Cowl, energy Puslor, Mecha Jack, Fiber-optic visor, blast guard, ammunition pouch.\n\r", ch );
		return;
	 }
	 if      (!str_cmp(arg,"vibroblade"     )) vnum = 0;
	 else if (!str_cmp(arg,"s-m-amp5"   )) vnum = 0;
	 else if (!str_cmp(arg,"torsoarmorunit"    )) vnum = 0;
	 else if (!str_cmp(arg,"jumpjets"   )) vnum = 0;
	 else if (!str_cmp(arg,"s-m-amechacowl"   )) vnum = 0;
  else if (!str_cmp(arg,"energypulsor"   )) vnum = 0;
  else if (!str_cmp(arg,"mechajack"   )) vnum = 0;
  else if (!str_cmp(arg,"fiber-opticvisor"   )) vnum = 0;
  else if (!str_cmp(arg,"blastguard"   )) vnum = 0;
  else if (!str_cmp(arg,"ammunitionpouch"   )) vnum = 0;
	else if (!str_cmp(arg,"jk52hoverwings")) vnum = 0;
	 else
	 {
	send_to_char("That is an invalid type.\n\r", ch );
	return;
	 }
	 if ( ch->practice < 120)
	 {
		send_to_char("It costs 120 points of primal to engineer a piece of equipment.\n\r",ch);
		return;
	 }
	 if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
	 {
	send_to_char("MECHA ARMOR STILL UNDER CONSTRUCTION.\n\r",ch);
	return;
	 }
	 ch->practice -= 120;
	 obj = create_object(pObjIndex, 50);
	 obj_to_char(obj, ch);
/*olc isn't working*/
if (!str_cmp(arg,"ninja-to")) obj->value[3] = 11;
if (!str_cmp(arg,"ninja-to")) obj->value[0] = 70;
	 act("$p lies in the created rubble.",ch,obj,NULL,TO_CHAR);
	 act("$p falls into $n's pile of rubble.",ch,obj,NULL,TO_ROOM);
	 return;
}

void do_flight(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_MECHA))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	else if ( ch->pcdata->powers[MPOWER_PILOTING] < 3 )
	{
		 send_to_char("You have not taken the third semester of Piloting.\n\r", ch);
		 return;
		 }

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Fly to whom?\n\r", ch );
	return;}

	location = victim->in_room;

	if (ch->move < 500) {
	send_to_char("You are to tired to engage your wings.\n\r", ch );
	return;}
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
	if ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68))
	{
		send_to_char("You cannot transport inside the arena.\n\r", ch);
		return;
	}

	if (ch->fight_timer > 0)
		  {
		  send_to_char("Not with a fight timer!\n\r",ch);
		  return;
		  }

	if (!IS_NPC(victim))
	{
	if (ch->max_move < victim->max_move)
	{send_to_char("Get more move!\n\r",ch);
	return;}
	}

	act("You burst your jump jets and release Your wings flying high off into the air.", ch, NULL, NULL, TO_CHAR);
	act("$n jumps off the ground in a burst of light flinging themself with ease as there wings extend from there Mecha Unit and Fly off.", ch, NULL, NULL, TO_ROOM);
	ch->move -= 500;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");
	act("You Disengage your wings and burst your jump jets slowly down to the ground.", ch, NULL, NULL, TO_CHAR);
	act("You here a rumbling and $n in a great mecha suit retracts its wings and slowly on jets sets its way down to you  .", ch, NULL, NULL, TO_CHAR);

	return;
}
void do_burst( CHAR_DATA *ch, char *argument )
{
	 char arg[MAX_INPUT_LENGTH];
	 CHAR_DATA *victim;
	 int dam;
	 argument = one_argument( argument, arg );


	 if(!IS_CLASS(ch,CLASS_MECHA))
	 {
	 send_to_char("Huh?\n\r",ch);
	 return;
	 }
	 else if ( ch->pcdata->powers[MPOWER_PILOTING] < 5 )
	{
		 send_to_char("You have not taken the fifth semester of piloting.\n\r", ch);
		 return;
		 }

	 if ( arg[0] == '\0' )
	 {
		  send_to_char( "Let loose hell on whom?\n\r", ch );
	return;
	 }


	 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	 {
	send_to_char( "They aren't here.\n\r", ch );
	return;
	 }

	 if ( victim == ch )
	 {
		  send_to_char( "Thats why We have Mecha Laws?\n\r", ch );
	return;
	 }

if (is_safe(ch,victim)==TRUE) return;

	 dam = char_damroll(ch);

	 if (IS_NPC(victim))
	dam += number_range(800, 1100);
	 else
	dam += number_range(700, 1000);

WAIT_STATE(ch,16);
if (ch->pcdata->powers[MPOWER_PILOTING] >1)
damage(ch,victim,dam,gsn_burst);
damage(ch,victim,dam,gsn_burst);
if (ch->pcdata->powers[MPOWER_PILOTING] > 2)
damage(ch,victim,dam,gsn_burst);
if (ch->pcdata->powers[MPOWER_PILOTING] == 5)
damage(ch,victim,dam,gsn_burst);
	 return;
}

void do_jump_jets(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;
//I may not use this	CHAR_DATA *victim2;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_MECHA))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	else if ( ch->pcdata->powers[MPOWER_TACTICS] < 1 )
	{
		 send_to_char("You have not taken the second semester of tactics.\n\r", ch);
		 return;
		 }

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Fly to whom?\n\r", ch );
	return;}

	location = victim->in_room;

	if (victim->in_room == NULL || victim->in_room->area != ch->in_room->area)
		{send_to_char("You cant spot them on the Battlefield.\n\r",ch);
		return;
		}

	if (ch->move < 500) {
	send_to_char("You overheated!!!.\n\r", ch );
	return;
	}

	if (!IS_NPC(victim))
	{
	if (ch->max_move < victim->max_move)
	{send_to_char("You cant catch up to them!\n\r",ch);
	return;}
	}

	act("You break your mecha off into a run spotting the enemy and blast off your jets as you hover after hi,.", ch, NULL, NULL, TO_CHAR);
	act("$n starts there mecha in a full out run seeming to spot something and suddenly activates some switch blasting him off over the ground.", ch, NULL, NULL, TO_ROOM);
	ch->move -= 500;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");
	act("You slowly release the gas flow on your jump jets as you land in front of your enemy.", ch, NULL, NULL, TO_CHAR);
	act("You feel a sudden rush of air then a huge after shock, as the dust in your eyes clear $n mecha is standing right in front of you assault rifle in hand.", ch, NULL, NULL, TO_CHAR);

	return;
}
void do_mecha_rush( CHAR_DATA *ch, char *argument )
{
	 CHAR_DATA       *victim;
	 ROOM_INDEX_DATA *to_room = NULL;
	 EXIT_DATA       *pexit;
	 EXIT_DATA       *pexit_rev;
	 char            buf       [MAX_INPUT_LENGTH];
	 char            direction [MAX_INPUT_LENGTH];
	 char            arg1      [MAX_INPUT_LENGTH];
	 char            arg2      [MAX_INPUT_LENGTH];
	 int             door;
	 int             rev_dir;
	 int             dam;

	 argument = one_argument( argument, arg1 );
	 argument = one_argument( argument, arg2 );
	 if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_MECHA))
	 {
	send_to_char("Huh?\n\r",ch);
	return;
	 }


	 if (ch->pcdata->powers[MPOWER_TACTICS] < 2)
	 {
	stc("You must take third semester of tacitcs.\n\r",ch);
	return;

	 }

	 if ( arg1[0] == '\0' )
	 {
		  send_to_char("Who do you wish to mecha rush?\n\r", ch);
		  return;
	 }
	 if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	 {
		  send_to_char("They are not here.\n\r", ch);
		  return;
	 }

	 if ( victim == ch )
	 {
		  send_to_char( "Not on yourself!\n\r", ch );
		  return;
	 }


/* for when vehicle code comes in and my higher tech mecha is made
	 if ( victim->size > ch->size)
	 {
		  send_to_char("They are too large to affect.\n\r",ch);
		  return;
	 }*/

	 WAIT_STATE( ch, 12 );
	 act("You silently pump your jumpjets to full speed and rush into $N from behind.", ch, NULL, victim, TO_CHAR);
	 act("$n rushes silently in rushing $N from behind.", ch, NULL, victim, TO_NOTVICT);
	 act("You feel an eerie silence and then it hits you, $n mecha-rushed you from behind.", ch, NULL, victim, TO_VICT);

	 rev_dir = 0;
	 if ( arg2[0] == '\0' )
		  door = number_range(0,3);
	 else
	 {
		  if      ( !str_prefix( arg2, "north" ) )
				door = 0;
		  else if ( !str_prefix( arg2, "east" ) )
				door = 1;
		  else if ( !str_prefix( arg2, "south" ) )
				door = 2;
		  else if ( !str_prefix( arg2, "west" ) )
				door = 3;
		  else
		  {
				send_to_char("You can only mecha-rush people north, south, east or west.\n\r", ch);
				return;
		  }
	 }

	 if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
	 if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
	 if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
	 if (door == 3) {sprintf(direction,"west");rev_dir = 1;}

	 if (( pexit = ch->in_room->exit[door]) == NULL)
	 {
		  sprintf(buf,"$N is flung into the  %s wall.", direction);
		  act(buf,ch,NULL,victim,TO_NOTVICT);
		  sprintf(buf,"$N is flung into the %s wall.", direction);
		  act(buf,ch,NULL,victim,TO_CHAR);
		  sprintf(buf,"You are mecha-rushed into the %s wall.", direction);
		  act(buf,ch,NULL,victim,TO_VICT);
		  dam = dice(victim->size*3+1, ch->pcdata->powers[MPOWER_TACTICS] * 3);
	  damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
        return;
    }
    
    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) )
    {
        if (IS_SET(pexit->exit_info, EX_LOCKED))
            REMOVE_BIT(pexit->exit_info, EX_LOCKED);
        if (IS_SET(pexit->exit_info, EX_CLOSED))
            REMOVE_BIT(pexit->exit_info, EX_CLOSED);
		  sprintf(buf,"$N is sent Flying across the battlefield by %s's mecha.", direction);
		  act(buf,ch,NULL,victim,TO_NOTVICT);
		  sprintf(buf,"$N is sent flying across the battlefield by %s's mecha.", direction);
		  act(buf,ch,NULL,victim,TO_CHAR);
		  sprintf(buf,"'oof' You fly into the air as you feel %s's mecha charge right into your spine.", direction);
		  act(buf,ch,NULL,victim,TO_VICT);
		  sprintf(buf,"There is no noise only the painful grown of $n as they smash through $d from the mecha rush.");
		  act(buf,victim,NULL,pexit->keyword,TO_ROOM);

		  if ( ( to_room   = pexit->to_room               ) != NULL && ( pexit_rev = to_room->exit[rev_dir] ) != NULL
		  &&   pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL )
		  {
				if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
					 REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
				if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
					 REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
				if (door == 0) sprintf(direction,"south");
				if (door == 1) sprintf(direction,"west");
				if (door == 2) sprintf(direction,"north");
				if (door == 3) sprintf(direction,"east");
				char_from_room(victim);
				char_to_room(victim,to_room);
				sprintf(buf,"$n comes smashing in through the %s $d.", direction);
				act(buf,victim,NULL,pexit->keyword,TO_ROOM);
				dam = dice(victim->size*3+1, ch->pcdata->powers[MPOWER_TACTICS] * 7);
			damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
		  }
	 }
	 else
	 {
		  sprintf(buf,"$N flies %s.", direction);
		  act(buf,ch,NULL,victim,TO_NOTVICT);
		  sprintf(buf,"$N flies %s.", direction);
		  act(buf,ch,NULL,victim,TO_CHAR);
		  sprintf(buf,"You fly %s.", direction);
		  act(buf,ch,NULL,victim,TO_VICT);
		  if (door == 0) sprintf(direction,"south");
		  if (door == 1) sprintf(direction,"west");
		  if (door == 2) sprintf(direction,"north");
		  if (door == 3) sprintf(direction,"east");
	  char_from_room(victim);
		  char_to_room(victim,pexit->to_room);
		  sprintf(buf,"$n comes flying in from the %s.", direction);
		  act(buf,victim,NULL,NULL,TO_ROOM);
		  dam = dice(victim->size*3+1, ch->pcdata->powers[MPOWER_TACTICS] * 5);
	  damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
	 }
	 return;
}

void do_psychicblast( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;

char buf[MAX_STRING_LENGTH];

int dam;
if (IS_NPC(ch)) return;

if (!IS_CLASS(ch,CLASS_MECHA))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->pcdata->powers[MPOWER_PSCIONICS] < 2)
{
send_to_char("you need 3 semesters of pscionics to use this skill.\n\r",ch);
	 return;
}

if (ch->move < 300)
{
	 send_to_char("You are too exhausted.\r\n", ch);
	 return;
}

if ( ( victim = ch->fighting ) == NULL )
{
send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;

}
WAIT_STATE( ch, 12 );
if (!IS_NPC(victim))
{
dam = ch->pcdata->powers[MPOWER_PSCIONICS] * 650;
}
else if (IS_NPC(victim))
{
dam = ch->pcdata->powers[MPOWER_PSCIONICS] * 650;
}

dam += number_range(60,80);

if ( dam <= 0 )
   dam = 1;
sprintf(buf,"You send sudden impulses through your jack Thrashing $N's nerves! [%d]",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"You notice $n squinting through his torso unit and suddenly you start to convulse! [%d]",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n seems to fluxuate inside his mech and suddenly $N starts convulsing! [%d]",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);

send_to_char("\n\r",ch);
hurt_person(ch, victim, dam);
ch->move -= number_range(200, 300);
return;
}
void do_cluster_bomb(CHAR_DATA *ch, char *argument) 
{

    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
	 int sn;
	 int level;
	 int spelltype;

	 argument = one_argument( argument, arg );
	 if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_MECHA))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

if (ch->pcdata->powers[MPOWER_TACTICS] < 3)
{
send_to_char("you need 4 semesters of tactics to use this skill.\n\r",ch);
	 return;
}

	 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
		  if ((victim = ch->fighting) == NULL)
		{
			send_to_char( "They aren't here.\n\r", ch );
			return;
		}
	 if (ch->move < 200)
	{
		send_to_char("Your mecha wont respond.\n\r", ch);
		  return;
	}
	 if ( ( sn = skill_lookup( "clusterbomb" ) ) < 0 )
		return;
	 spelltype = skill_table[sn].target;
	 level = ch->spl[spelltype] * 1.1;
	 level = level * 4;
	 act("You strafe right and throw a cluster bomb at $N.",ch,NULL,victim,TO_CHAR);
	 act("$n strafes and throws $s cluster bomb at you.",ch,NULL,victim,TO_VICT);
	 (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
	 WAIT_STATE( ch, 12 );
	 ch->move = ch->move - 200;
	 return;
}
void do_kamikazi (CHAR_DATA *ch, char *argument )
{
	 CHAR_DATA *victim;
	 char arg [MAX_INPUT_LENGTH];
	 argument = one_argument( argument, arg );
	 if (IS_NPC(ch))
	 return;
	 if (!IS_CLASS(ch, CLASS_MECHA) )
	 {
		send_to_char("huh?\n\r", ch);
		return;
	 }

 if (IS_CLASS(ch, CLASS_MECHA) && ch->pcdata->powers[MPOWER_COMBAT] < 4 )

	{

	  send_to_char("You have not taken the 5th course of Combat.\n\r", ch);

	  return;

	}
	 if (ch->move < 10000)
	 {
		send_to_char("You do not have enough move.\n\r", ch);
		return;
	 }

	 if (ch->hit > 1000)
	 {
		send_to_char("You are not near death.\n\r", ch);
		return;
	 }
	 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	 {
		send_to_char("They aren't here.\n\r", ch);
		return;
	 }
//    if (is_safe(ch, victim))
//    return;
	 if (victim->level < 3)
	 {
		send_to_char("Not on a mortal.\n\r", ch);
		return;
	 }
	 if (IS_NPC(victim))
	 {
		send_to_char("Huh?", ch);
		return;
	 }
	 act("You Scream with the last possible strength you have and thrust your vibro blade through $N's head And Simply destroy them like a ragdoll.",ch,NULL,victim,TO_CHAR);
	 act("$n screams in Pain and Anger as he lunges Bringing a hilt with something on it through the head of some poor helpless soul.",ch,NULL,victim,TO_NOTVICT);
	 act("$n Screams and charges you bringing a stick down and ==={-----> Straight through your head, your body goes numb as he rips your corpse apart!",ch,NULL,victim,TO_VICT);
	 victim->hit -= 22000;
	 ch->move -= 10000;
	 if (ch->fighting == NULL)
	 set_fighting(ch,victim);
	 update_pos(victim);
	 update_pos(ch);
	 WAIT_STATE(ch, 5);
	 act("You here the scream of the vibro blade as you are thrashed lifelessy about severed by all parts of the body!",ch,NULL,victim,TO_CHAR);
	 act("$n falls off the sliced corpse of there victim and slowly dies.",ch,NULL,victim,TO_NOTVICT);
	 act("$n falls off your sliced body and starts to slowly die.",ch,NULL,victim,TO_VICT);
	 WAIT_STATE(ch, 40);
	 return;
}


