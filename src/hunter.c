//class by bryantos, it was started right before he shut down the mud so
//its not finished
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



void do_veil(CHAR_DATA *ch, char *argument)
{
if (IS_NPC(ch)) 
		return;
	else if (!IS_CLASS(ch, CLASS_HUNTER) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    if ( IS_SET(ch->act, PLR_SACREDINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SACREDINVIS);
        send_to_char( "You are now able to be seen..\n\r", ch );
        act("$n slowly emerges into sight.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
        if (has_timer(ch)) return;

        send_to_char( "You hide yourself.\n\r", ch );
        act("$n slowly fades into the background.",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->act, PLR_SACREDINVIS);
    }
return;         
}




void do_blend(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

  if (IS_NPC(ch)) 
		return;
	else if (!IS_CLASS(ch, CLASS_HUNTER) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}


    if (ch->mana < 1500)
    {
        send_to_char("You do not have enough vitality to do
that!\n\r",ch);
        return;
    }
    if (ch->fight_timer > 0)
    {
       send_to_char("You may not hunt with a fight
timer.\n\r", ch);
       return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Make who your prey?\n\r", ch );   
        return;
    }

    if ((victim = get_char_world(ch, arg)) == NULL)
    {
	send_to_char("They aren't here!\n\r", ch);
	return;
    }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
        return;
    }

    if (victim->in_room->vnum == ch->in_room->vnum)
    {
        send_to_char("But you're already there!\n\r",ch);
        return;
    }
    if (victim->mana > ch->mana && !IS_NPC(victim)) 
    {
	send_to_char("Get more mana.\n\r", ch);
	return;
    }

        act("You dissapear.", ch, NULL, victim,
TO_CHAR);
        act("$n disapears.", ch, NULL, NULL,
TO_ROOM);
        char_from_room(ch);
        char_to_room(ch,victim->in_room);
        act("$n appears infront of $N.", ch, NULL, victim,
TO_ROOM);
        act("$n steps out from behind the tree.", ch, NULL, victim,
TO_VICT);
	if (!IS_NPC(victim)) ch->mana -= 1000;
	do_look(ch, "");
WAIT_STATE(ch, 6);
    return;
} /*
void do_edge( CHAR_DATA *ch, char *argument )

{

    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_HUNTER))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Edges Learned (%d).\n\r", ch->pcdata->powers[PMONK]);
	send_to_char(buf,ch);
	send_to_char("Powers you have:\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] < 1 )
	   send_to_char("You have no knowledge of any current Edges.\n\r",
ch);
	  if ( ch->pcdata->powers[PMONK] >= 1 )
	   send_to_char("Veil.\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 2 )
	   send_to_char("Blend\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 3 )
	   send_to_char("Radiate\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 4 )
	   send_to_char("Donate\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 5 )
	   send_to_char("Bluster\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 6 )
	   send_to_char("Respire\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 7 )
	   send_to_char("Payback\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 8 )
	   send_to_char("Scry/Reada\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 9 )
	   send_to_char("Discern\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 10 )
	   send_to_char("Restorehealth\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 11 )
	   send_to_char("Foresee\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 12 )
	   send_to_char("Pinpoint\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 13 )
         send_to_char("Ward\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 14 )
         send_to_char("Burden\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 15 )
         send_to_char("Charge\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 16 )
         send_to_char("Momento\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 17 )
         send_to_char("Blaze\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 18 )
         send_to_char("Flash \n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 19 )
         send_to_char("Surge\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 20 )
         send_to_char("You have completed your Mantra training.\n\r", ch);
	return;
    }

	if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"power"))
	{
	  	  if ( ch->pcdata->powers[PMONK] >= 1 )
	   send_to_char("Veil.\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 2 )
	   send_to_char("Blend\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 3 )
	   send_to_char("Radiate\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 4 )
	   send_to_char("Donate\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 5 )
	   send_to_char("Bluster\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 6 )
	   send_to_char("Respire\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 7 )
	   send_to_char("Payback\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 8 )
	   send_to_char("Scry/Reada\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 9 )
	   send_to_char("Discern\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 10 )
	   send_to_char("Restorehealth\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 11 )
	   send_to_char("Foresee\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 12 )
	   send_to_char("Pinpoint\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 13 )
         send_to_char("Ward\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 14 )
         send_to_char("Burden\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 15 )
         send_to_char("Charge\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 16 )
         send_to_char("Momento\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 17 )
         send_to_char("Blaze\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 18 )
         send_to_char("Flash \n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 19 )
         send_to_char("Surge\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 20 )
	  send_to_char("You have completed your Mantra training.\n\r",ch);
	  return;
	}
	sprintf(buf,"Edges Learned (%d).\n\r", ch->pcdata->powers[PMONK]);
sprintf(buf, "\n\rType Edge power improve to improve your powers.\n\r");

	send_to_char(buf,ch);
	return;
    }

    if (!str_cmp(arg2,"improve"))
    {
	int cost;
	int max = 20;
	
	cost = (ch->pcdata->powers[PMONK]+1) * 6.5;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[PMONK] >= max )
	{
    sprintf(buf,"You have already gained all the known Edges.\n\r");
	    send_to_char(buf,ch);
	    return;
	}

	if ( cost > ch->practice )
	{
    sprintf(buf,"It costs you %d primal learn the Edge.\n\r", cost);

	    send_to_char(buf,ch);
	    return;
	}

	ch->pcdata->powers[PMONK]++;
	ch->practice -= cost;
	sprintf(buf,"You learn a Edge.\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("To learn Edge, type: edge power improve.\n\r",ch);
	return;
}
*/

void do_radiate( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
  
  	if (!IS_CLASS(ch, CLASS_HUNTER) )
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( ch->pcdata->powers[PMONK] < 6 )
  	{
  	  send_to_char("You have not learned the Radiate
Edge.\n\r", ch);
  	  return;
  	}

/* Remove radiate in handler.c */
 
    if ( IS_SET(ch->newbits, NEW_MONKSKIN) )
    {
	REMOVE_BIT( ch->newbits, NEW_MONKSKIN );
	send_to_char( "Your aura disappears.\n\r", ch );
    }
    else
    {
	send_to_char( "You are surrounded by aura of protection.\n\r", ch
);
	SET_BIT(ch->newbits, NEW_MONKSKIN);
    }
    return;
}


		

void do_bluster( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );

  if (IS_NPC(ch)) 
		return;
	else if (!IS_CLASS(ch, CLASS_HUNTER) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    sprintf(buf,"%s: BLUSTER! %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    if (arg1[0] == '\0')
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room
)
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
    }

/*
    if (!str_cmp(arg1,"all"))
        {
        for (d = descriptor_list; d != NULL; d = d->next)
        {
            victim = d->character;
	    
	    if ( victim->fighting != NULL )
            stop_fighting( victim, TRUE );
	}
	}*/
	if (!str_cmp(arg1,"all"))
        {
        for (d = descriptor_list; d; d = d->next)
        {
            if (!d->character) continue;
            victim = d->character;

            if ( victim->fighting  )
            stop_fighting( victim, TRUE );
            ch->fight_timer += 12;       
 }   
}

    send_to_char( "Ok.\n\r", ch );
    return;
}
/*
void do_smite (CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];
	int dam;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	
	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
		return;
	if (!IS_CLASS(ch, CLASS_HUNTER)
	{
		send_to_char("Sorry you suck WAYY too much!\n\r", ch);
		return;
	}
	
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting to smite someone.\n\r",
ch);
		return;
	}
	dam = (((ch->hitroll / 2) * (7 - ch->generation)) * 0.8);
	if (!IS_NPC(victim))
		dam = dam * 2.0;
 	if (IS_CLASS(victim, CLASS_DEMON))
		dam = dam * 2.0;
	if ( dam <= 0 )
		dam = 1;
	sprintf(buf,"Your smite strikes $N [%d]\n\r",dam);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n's smite strikes you [%d]\n\r",dam);
	act(buf,ch,NULL,victim,TO_VICT);
	sprintf(buf,"$n's ASS KICK strikes $N [%d]\n\r",dam);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	victim->hit -= dam;
	WAIT_STATE(ch,24);
	return;
}

void do_edge( CHAR_DATA *ch, char *argument )
{

    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_HUNTER))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Edges: Mercy (%d), Vision (%d), Defence (%d), Zeal
(%d).\n\r",
	ch->pcdata->powers[NPOWER_MERCY],
ch->pcdata->powers[NPOWER_VISION],
	ch->pcdata->powers[NPOWER_DEFENCE],
ch->pcdata->powers[NPOWER_ZEAL]);
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"mercy"))
	{
	    send_to_char("Mercy - The true fighting power of the
Edges.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_MERCY] < 1) 
		send_to_char("You have none of the Edges in this
class.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_MERCY] > 0) 
		send_to_char("Veil -Hide- allows you to conceal your
presence.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_MERCY] > 1) 
		send_to_char("Illuminate - Reveal any unseen supernatural
creatures in the room.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_MERCY] > 2) 
		send_to_char("Radiate - Once this power is activated, your
character radiates a white light that impedes supernatural attacks.
People protected by this field appear blurry and indistinct to the eyes of
unnatural creatures. (harder to hit).\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_MERCY] > 3) 
		send_to_char("Donate - The character donates some of his
health to another to heal their wounds.\n\r",ch);
		if (ch->pcdata->powers[NPOWER_MERCY] > 4)
		send_to_char("\n\r", ch);
		if (ch->pcdata->powers[NPOWER_MERCY] > 5)
		send_to_char("Bluster - By yelling the simple word stop
the player can stop fights in a room.\n\r", ch);
if (ch->pcdata->powers[NPOWER_MERCY] > 6)
		send_to_char("Respire    - Also called the "Kiss of Life,"
this edge allows you to heal another person by breathing into their
mouth.\n\r", ch);
if (ch->pcdata->powers[NPOWER_MERCY] > 7)
		send_to_char("Suspend - By using this power a player can
prevent another from fleeing into the shadowplane.\n\r", ch);
if (ch->pcdata->powers[NPOWER_MERCY] > 8)
		send_to_char("Payback   - The player absorbs some of the
damage from the attackers hits and redirects a fraction of it back at the
attacker.\n\r", ch);
if (ch->pcdata->powers[NPOWER_MERCY] > 9)
		send_to_char("\n\r", ch);
	    

return;
	}
	else if (!str_cmp(arg1,"Vision"))
	{
	    send_to_char("Vision - The ability of instinct, foresight, and
awareness.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_VISION] < 1) 
		send_to_char("You have none of the Vision Edges.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_VISION] > 0) 
		send_to_char("Scry/Reada - Scry and Readaura.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_VISION] > 1) 
		send_to_char("Discern - (Truesight) Your character
exhibits a hyper-alertness to supernatural creatures, a surreal
perspective that allows you to perceive things about the unkown that most
could not..\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_VISION] > 2) 
		send_to_char("\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_VISION] > 3) 
		send_to_char("Restore - This is the power to regrow lost
or mangled limbs and organs.\n\r",ch);
		if (ch->pcdata->powers[NPOWER_VISION] > 4)
		send_to_char(" Foresee - Foresee allows hunters a brief
glimpse into the future and the actions of others giving them time to
better dodge attacks. (better parry/dodge)\n\r", ch);
	   if (ch->pcdata->powers[NPOWER_VISION] > 5) 
		send_to_char("Pinpoint - This edge gives the player an
insight into the weaknesses of others. (enchanced damage)\n\r",ch);
          if (ch->pcdata->powers[NPOWER_VISION] > 6)
 		send_to_char("\n\r",ch);
          if (ch->pcdata->powers[NPOWER_VISION] > 7 )
		send_to_char("Instinct - Allows the player to feel when
they are being watched. (scry detection)\n\r",ch);
          if (ch->pcdata->powers[NPOWER_VISION] > 8 )
		send_to_char("\n\r",ch);
          if (ch->pcdata->powers[NPOWER_VISION] > 9)
		send_to_char("Blend - The Player imagines where he wants
to be and he's there. (only works on mobs)\n\r",ch);


return;
	}

	else if (!str_cmp(arg1,"Defence"))
	{
	    send_to_char("Ningenno - The battle, attacking and getting
away.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_DEFENCE] < 1) 
		send_to_char("You have no powers of this.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_DEFENCE] > 0) 
		send_to_char("1.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_DEFENCE] > 1) 
		send_to_char("2.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_DEFENCE] > 2) 
		send_to_char("3.\n\r",ch);
            if (ch->pcdata->powers[NPOWER_DEFENCE] > 3) 
		send_to_char("4.\n\r",ch);
		if (ch->pcdata->powers[NPOWER_DEFENCE] > 4)
		send_to_char("5.\n\r", ch);
          if (ch->pcdata->powers[NPOWER_DEFENCE] > 5)
		send_to_char("6.\n\r", ch);
          if (ch->pcdata->powers[NPOWER_DEFENCE] > 6)
		send_to_char("7.\n\r", ch);
          if (ch->pcdata->powers[NPOWER_DEFENCE] > 7)
		send_to_char("8.\n\r", ch);
          if (ch->pcdata->powers[NPOWER_DEFENCE] > 8)
		send_to_char("9.\n\r", ch);
          if (ch->pcdata->powers[NPOWER_DEFENCE] > 9)
		send_to_char("10.\n\r", ch);

	    return;
	}
	sprintf(buf,"Edges: Mercy (%d), Vision (%d), Defence (%d), Zeal
(%d).\n\r",
	ch->pcdata->powers[NPOWER_MERCY],
ch->pcdata->powers[NPOWER_VISION],
	ch->pcdata->powers[NPOWER_DEFENCE],
ch->pcdata->powers[NPOWER_ZEAL]);
	send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"mercy"   )) {improve = NPOWER_MERCY;
max=10;}
	else if (!str_cmp(arg1,"vision"   )) {improve = NPOWER_VISION;
max=10;}
	else if (!str_cmp(arg1,"defence"   )) {improve = NPOWER_DEFENCE;
max=10;}
      else if (!str_cmp(arg1,"zeal"   )) {improve = NPOWER_ZEAL;
max=10;}
	else
      {
	    send_to_char("Edges: Mercy, Vision, Defence, Zeal.\n\r",ch);
	    return;
	}							
	cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the powers of the %s
Edge.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your %s
Edge.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You improve your ability in the %s Edge.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve an Edge, type: Edge <edge type>
improve.\n\r",ch);
	return;
}
*/
