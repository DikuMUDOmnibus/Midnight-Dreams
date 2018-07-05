 
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define CRYSDEF 47.4
bool crys = TRUE;

bool	check_dodge		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	check_parry		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	dam_message		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void	death_cry		args( ( CHAR_DATA *ch ) );
void	group_gain		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	xp_compute		args( ( CHAR_DATA *gch, CHAR_DATA *victim ) );
void	set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_counter		args( ( CHAR_DATA *ch) );
bool	can_bypass		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int		number_attacks	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int		dambonus		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance) );
void	update_damcap	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    check_arena     args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

CONFIG_DATA		*	classcon;

/*bool check_huge_serp( CHAR_DATA *ch)
{
        AFFECT_DATA *paf;
        bool yes = FALSE;

        if (IS_AFFECTED(ch, AFF_POLYMORPH))
        for ( paf = ch->affected; paf != NULL; paf = paf->next )
        {
            if ( paf->modifier == 2048 && affect_bit_name( paf->bitvector 
) == " polymorph")
            {
                    yes = TRUE;
            }

        }
        return yes;
}

bool check_huge_serp( CHAR_DATA *ch )
{
 AFFECT_DATA *pAf;
 bool found = FALSE;

 for (pAf = ch->affected; pAf; pAf = pAf->next)
 {
    if (pAf->type == skill_lookup("polymorph") && pAf->location ==
APPLY_POLY && pAf->modifier == POLY_HUGE_SERPENT)
	found = TRUE;
 }

 return found;
}

bool check_wyvern( CHAR_DATA *ch)
{
        AFFECT_DATA *paf;
        bool yes = FALSE;

	if (IS_AFFECTED(ch, AFF_POLYMORPH))
        for ( paf = ch->affected; paf != NULL; paf = paf->next )
        {
            if ( paf->modifier == 1024 && affect_bit_name( paf->bitvector
) == " polymorph")
            {
                    yes = TRUE;
            }

        }
        return yes;
}*/

bool check_poly( CHAR_DATA *ch, int form )
{
 AFFECT_DATA *pAf;
 bool found = FALSE;

 for (pAf = ch->affected; pAf; pAf = pAf->next)
 {
    if (pAf->type == skill_lookup("polymorph") && pAf->location ==
APPLY_POLY && pAf->modifier == form)
	found = TRUE;
 }

 return found;
}

void violence_update( void )
{
	CHAR_DATA *ch,*vch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    CHAR_DATA *rch_next;
    CHAR_DATA *mount;
    OBJ_DATA  *obj;
  
    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
if (IS_MOOGLE(ch))
{
    bool fighting = ch->fighting ? 1 : 0;
    if (ch->pcdata->powers[MOOG_WAIT] > 0)
    {
	if (--ch->pcdata->powers[MOOG_WAIT] == 0)
	{
	    if (dance_checkup(ch))
	    if (!fighting && moogle_table[ch->sn].type == MOOG_VIOLENCE)
		multi_hit(ch,ch->uni,TYPE_UNDEFINED);
	}
	else
	{
	    act("#5$n runs around shouting, \"Kuppo! Kuppo!\"",
		ch,0,0,TO_ROOM);
	    stc("#5You run around shouting, \"Kuppo! Kuppo!\"\n\r",ch);
	}
    }
}
if (IS_UNI(ch))
{
    if (ch->pcdata->powers[UNI_SONG] != 0)
    {
	if (--ch->pcdata->powers[UNI_WAIT] <= 0)
	{
	    if (ch->pcdata->powers[UNI_SONG] == SONG_HEAL)
	    {
		if (ch->uni)
		{
if (((CHAR_DATA*)ch->uni)->in_room != ch->in_room)
{stc("They are no longer in your room!\n\r", ch);} else {
		    act("$n mends $N's wounds.",ch,0,ch->uni,TO_NOTVICT);
		    act("You mend $N's wounds.",ch,0,ch->uni,TO_CHAR);
		    act("$n mends your wounds.",ch,0,ch->uni,TO_VICT);
		    uni_heal(ch,(CHAR_DATA*)ch->uni);
		    ch->uni = NULL;
		}}
		else {
		    act("$n mends $s wounds.",ch,0,0,TO_ROOM);
		    stc("You mend your wounds.\n\r", ch);
		    uni_heal(ch,ch);
		}
	    }
	    else if (ch->pcdata->powers[UNI_SONG] == SONG_QUELL)
	    {
		act("$n raises his horn and calls it peace.",
		    ch,0,0,TO_ROOM);
		stc("You raise up your horn and call peace.\n\r",ch);
		for (vch = ch->in_room->people;vch;vch=vch->next_in_room)
		    if (vch->fighting) stop_fighting(vch,TRUE);
	    }
	    else if (ch->pcdata->powers[UNI_SONG] == SONG_INSPIRE)
	    {
		if (ch->uni)
		{
if (((CHAR_DATA*)ch->uni)->in_room != ch->in_room)
{stc("They are no longer in your room!\n\r", ch);} else {
		    act("$n sings a song of inspiration to $N.",
			ch,0,ch->uni,TO_NOTVICT);
		    act("You sing a song of inspiration to $N.",
			ch,0,ch->uni,TO_CHAR);
		    act("$n sings a song of inspiration to you.",
			ch,0,ch->uni,TO_VICT);
		    stc("#6You feel inspired!\n\r", (CHAR_DATA *)ch->uni);
		    inspire((CHAR_DATA *)ch->uni);}
ch->uni = NULL;
		}
		else
		{
		    act("$n inspires $mself. Wierd, eh?\n\r",
			ch,0,0,TO_ROOM);
		    stc("Wierd. You inspire yourself?\n\r",ch);
		    stc("#6You feel inspired!\n\r", ch);
		    inspire(ch);
		}
	    }
	    ch->pcdata->powers[UNI_SONG] = 0;
	    ch->pcdata->powers[UNI_WAIT] = 0;
	}
	else
	{
	    act("#7$n continues to sing..#n",ch,0,0,TO_ROOM);
	    stc("#7You continue to sing..#n.\n\r", ch);
	}
    }
}

if (ch->special & SPC_GOLEMBIND && !ch->fighting)
{stc("You feel your hands and feet break free.\n\r", ch);
 REMOVE_BIT(ch->special, SPC_GOLEMBIND);}
        victim = ch->blinkykill;
	if (ch->blinkykill == NULL)
		continue;
	if (IS_SET(ch->flag3, AFF3_BLINK_1ST_RD))
	{
		REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
		SET_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
	}
	else if (IS_SET(ch->flag3, AFF3_BLINK_2ND_RD))
	{
	    REMOVE_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
	    REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
	    /* Has the victim left? */
	    if (ch->blinkykill->in_room != ch->in_room)
	    {
		ch->blinkykill = NULL;
		act("$n pops back into existence.", ch, NULL, NULL, TO_ROOM);
		stc("You pop back into existence.\n\r", ch);
		stc("Your victim is no longer in the room.\n\r", ch);
		break;
	    }
	    act("$n pops back into existencse next to $N.", ch,
NULL,ch->blinkykill, TO_NOTVICT);
	    act("You pop back into existence next to $N.", ch, NULL, ch->blinkykill, TO_CHAR);
	    act("$n pops back into existence next to you.", ch, NULL,ch->blinkykill, TO_VICT);
	    set_fighting(ch, ch->blinkykill);
act("$n says, 'I'll swallow your soul!'.",ch,0,0,TO_ROOM);
act("You say, 'I'll swallow your soul!'.",ch,0,0,TO_CHAR);
{bool stopper = FALSE; if (ch->blinkykill && IS_CLASS(ch->blinkykill,
CLASS_GOLEM)) stopper = TRUE;
	    ch->blinkykill = NULL;
if (victim && victim != char_free)
            multi_hit(ch, victim, gsn_blinky);
if (stopper)
{act("You blink on stone and it backfires!",ch,0x0,0x0,TO_CHAR);
act("$n tries to blink on stone and it backfires!",ch,0x0,0x0,TO_ROOM);
WAIT_STATE(ch,PULSE_VIOLENCE);}
if (!stopper)
if (victim && victim != char_free)
            multi_hit(ch, victim, gsn_blinky);
            multi_hit(ch, victim, gsn_blinky);

	    if (IS_SET(ch->newbits, THIRD_HAND) && number_range(0,1)==1 &&
			get_eq_char(ch, WEAR_THIRD) != NULL && !stopper)
if (victim && victim != char_free)
		multi_hit(ch, victim, gsn_blinky);
	    if (IS_SET(ch->newbits, FOURTH_HAND) && number_range(0,1)==1 &&
			get_eq_char(ch, WEAR_FOURTH) != NULL)
if (victim && victim != char_free)
		multi_hit(ch, victim, gsn_blinky);
	    update_pos(victim);
	}}
    }/* End of for ( ch = char_list; ch != NULL; ch = ch->next ) */

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
		ch_next	= ch->next;
		if (ch->fight_timer > 0)
			ch->fight_timer--;
        if (ch->rage > 0 && IS_CLASS(ch, CLASS_DEMON))
		    ch->rage--;
		if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
		    continue;
		if (!IS_NPC(ch) && !IS_NPC(victim))
			ch->fight_timer = 10;

        if (ch->class != CLASS_DEMON && ch->alignment > 0 && !IS_NPC(ch) && ch->level > 2)
		{
			OBJ_DATA *obj_next;
		    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
		    {
				obj_next = obj->next_content;
				if (IS_OBJ_STAT2(obj, ITEM_ATTACK_GOOD) && dice(1, 3) == 1)
				{
					act("$p jumps into the air and slams into $n.", ch, obj, NULL, TO_ROOM);
					act("$p jumps into the air and slams into you.", ch, obj, NULL, TO_CHAR);
					ch->hit -= number_range( 40, 60 );
					update_pos( ch );
				}
			}
		}

		if ( IS_AWAKE(ch) && IS_AWAKE(victim) && ch->in_room == victim->in_room ) 
		{
			multi_hit( ch, victim, TYPE_UNDEFINED );
			if (IS_CLASS(ch, CLASS_DROW) && ch->cur_form == FRM_DROWSPIDER && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS))
				multi_hit(ch, victim, gsn_spiderform);
		} 
		else
		    stop_fighting( ch, FALSE );
		if ( ( victim = ch->fighting ) == NULL )
		    continue;
		for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
		{
		    rch_next = rch->next_in_room;
		    if ( IS_AWAKE(rch) && rch->fighting == NULL )
		    {
				/* Mount's auto-assist their riders and vice versa.*/
				if ( (mount = rch->mount) != NULL )
				{
					if (mount == ch) multi_hit( rch, victim, TYPE_UNDEFINED );
						continue;
				}
		
				/* PC's auto-assist others in their group.*/
				if ( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) )
				{
			    if ( ( !IS_NPC(rch) || IS_AFFECTED(rch,
AFF_CHARM) ) && is_same_group(ch, rch) && !is_safe(rch,victim) )
//				if (!is_safe(rch,victim))
					multi_hit( rch, victim, TYPE_UNDEFINED );
					continue;
				}
				/* NPC's assist NPC's of same type or 12.5% chance regardless.*/
				if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) )
				{
					if ( rch->pIndexData == ch->pIndexData ||   number_bits( 3 ) == 0 )
					{
						CHAR_DATA *vch;
						CHAR_DATA *target;
						int number;
						target = NULL;
						number = 0;
						for ( vch = ch->in_room->people; vch; vch = vch->next )
						{
							if ( can_see( rch, vch ) && is_same_group( vch, victim ) && number_range( 0, number ) == 0 )
							{
								target = vch;
								number++;
							}
						}
					if ( target != NULL )
				    multi_hit( rch, target, TYPE_UNDEFINED );
					} 
				}/* End of if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) ) */
			} /* End of if ( IS_AWAKE(rch) && rch->fighting == NULL )*/
		}/*End of for ( rch = ch->in_room->people; rch != NULL; rch = rch_next ) */
    }/* End of for ( ch = char_list; ch != NULL; ch = ch->next )*/
for (ch = char_list; ch; ch = ch->next)
if (IS_MOOGLE(ch))
{
    if (ch->pcdata->powers[KUPPO_BITS] & KUPPO_SUN_BATH)
    {
	if (ch->position == POS_FIGHTING)
	{
	    act("$n's wounds knit themselves back together.",
		ch,0,0,TO_ROOM);
	    stc("Your wounds knit themselves back together.\n\r",ch);
	    smooregen2(ch,750,0,0,500);
	}
	else
	{
	    act("$n starts to feel the pain of his past battle, although it was enjoyable at the time.",
		ch,0,0,TO_ROOM);
	    stc("You start to feel the pain of your last battle.\n\r", ch);
	    hurt_person(ch,ch,500);
	    REMOVE_BIT(ch->pcdata->powers[KUPPO_BITS], KUPPO_SUN_BATH);
	}
    }
}


    return;
}/* End void violence_update( void ) */

void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	OBJ_DATA *wield;
    OBJ_DATA *wield1;
    OBJ_DATA *wield2;
    OBJ_DATA *wield3;
    OBJ_DATA *wield4;
    int	sn;
    bool blink = dt == gsn_blinky ? TRUE : FALSE;
    int	level;
    int chance;
    int unarmed;
    int wieldorig = 0;
    int wieldtype = 0;
    int maxcount;
    int countup;
    int x;
    int	wieldie;

	if ( ch->position < POS_SLEEPING )
		return;

    if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_VAMPIRE)) 
       && (ch->position == POS_FIGHTING) 
       && IS_SET(ch->act, PLR_WIZINVIS)
       && !IS_IMMORTAL(ch))
    {
       REMOVE_BIT(ch->act, PLR_WIZINVIS);
       send_to_char( "You step from the shadows.\n\r",ch );
       act("$n steps from the shadows.",ch,NULL,NULL,TO_ROOM);
	
    }

      if ( dt == gsn_circle )
	{
		one_hit(ch, victim, dt, 1 );
		return;
	}
    if ( dt == gsn_headbutt )
    {
		one_hit( ch, victim, dt, 1 );
		return;
    }
    if ( dt == gsn_spiderform )
    {
	for (x = 1; x < 6; x++)
		one_hit(ch,victim,dt,0);
	return;
    }
    if ( dt == gsn_fangs )
    {
        one_hit( ch, victim, dt, 1 );
        return;
    } 
    if ( dt== gsn_quills)
    { if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
   		one_hit(ch,victim,dt,1);
    	return;
    }
    if ( dt==gsn_venomtong)
    {
  		one_hit(ch,victim,dt,1);
   		return;
    }
    if ( dt==gsn_spiketail)
    {
	    one_hit(ch,victim,dt,1);
	    return;
    }  
    if ( dt==gsn_badbreath)
    {
	    one_hit(ch,victim,dt,1);
	    return;
    }  
    if ( dt==gsn_magma)
    {
	    one_hit(ch,victim,dt,1);
	    return;
    }  
    if ( dt==gsn_shards)
    {
	    one_hit(ch,victim,dt,1);
	    return;
    }  
    if ( dt==gsn_cheapshot)
    {
	    send_to_char("You shoulder charge hard!\n\r",ch);
	    send_to_char("You are shoulder charged!\n\r",victim);
	    one_hit(ch,victim,dt,1);
	    one_hit(ch,victim,dt,1);
		return;
    } 
    if ( dt == gsn_buffet )
    {
        one_hit( ch, victim, dt, 1 );
        return;
    } 
    if ( dt == gsn_sweep )
    {
	  one_hit( ch, victim, dt, 1 );
	  return;
    }
    if ( dt == gsn_rfangs )
    {
         one_hit( ch, victim, dt, 1);
         return;
    }
    if (is_safe(ch,victim)) return;
    wield1 = get_eq_char( ch, WEAR_WIELD );
    wield2 = get_eq_char( ch, WEAR_HOLD );
    wield3 = get_eq_char( ch, WEAR_THIRD );
    wield4 = get_eq_char( ch, WEAR_FOURTH);
    if ( wield1 != NULL && wield1->item_type == ITEM_WEAPON )
		wieldorig  = 1;
    if ( wield2 != NULL && wield2->item_type == ITEM_WEAPON )
		wieldorig += 2;
    if ( wield3 != NULL && wield3->item_type == ITEM_WEAPON )
		wieldorig += 4;
    if ( wield4 != NULL && wield4->item_type == ITEM_WEAPON )
		wieldorig += 8;
    wieldtype = wieldorig;
    wieldie = number_range(1,4);
    if ( wieldorig == 15 ) 
    { 
        if (wieldie == 2) 
			wieldtype = 8; 
		else
			wieldtype = 4;
    }
    if ( wieldorig == 14 )
    {
		if (wieldie == 2)
			wieldtype = 8;
		else if (wieldie == 1)
			wieldtype = 4;
		else if (wieldie == 3)
			wieldtype = 2;
		else
			wieldtype = 1;
    }
    if ( wieldorig == 13 )
    {
		if ( wieldie == 1)
			wieldtype = 8;
		else if (wieldie == 2)
			wieldtype = 4;
		else wieldtype = 1;
    }
    if ( wieldorig == 12 )
    {
		if ( wieldie == 1 )
			wieldtype = 8;
		else
			wieldtype = 4;
    }
    if ( wieldorig == 11 )
    {
		if ( wieldie == 1 )
			wieldtype = 8;
		if ( wieldie == 2 )
			wieldtype = 2;
		else wieldtype = 1;
    }
    if ( wieldorig == 10 )
    {
		if ( wieldie == 1 )
			wieldtype = 8;
		else wieldtype = 2;
    }
    if ( wieldorig == 9 )
    {
		if ( wieldie == 1 )
			wieldtype = 8;
		else wieldtype = 1;
    }
    if ( wieldorig == 8 )
		wieldtype = 8;
    if ( wieldorig == 7 )
    {
		if ( wieldie == 1 )
			wieldtype = 4;
		else if ( wieldie == 2 )
			wieldtype = 2;
		else wieldtype = 1;
    }
    if ( wieldorig == 6 )
    {
		if ( wieldie == 1 || wieldie == 2)
			wieldtype = 2;
		else wieldtype = 4;
    } 
    if ( wieldorig == 5 )
    {
		if ( wieldie == 1)
			wieldtype = 4;
        else wieldtype = 1;
    }
    if ( wieldorig == 4 )
		wieldtype = 4;
    if ( wieldorig == 3 )
    {
        if ( wieldie == 2 || wieldie == 4)
			wieldtype = 2;
        else wieldtype = 1;
    }
    if ( wieldorig == 2 )
		wieldtype = 2;
    if ( wieldorig == 1 )
		wieldtype = 1;

    if ( wieldtype == 8 )
		wield = wield4;
    else if ( wieldtype == 4 )
		wield = wield3;
    else if ( wieldtype == 2 )
		wield = wield2;
    else 
		wield = wield1;

    if (!IS_NPC(ch) && ch->stance[0] > 0 && number_percent() <= 5)
    {
		int stance = ch->stance[0];
		if (ch->stance[stance] >= 200)
		{
			special_move(ch,victim);
			return;
		}
    }
    unarmed = 0;
	one_hit( ch, victim, dt, wieldtype );

    if ( victim == NULL || victim->position != POS_FIGHTING )
		return;

    if ( dt == TYPE_UNDEFINED )
    {
		dt = TYPE_HIT;
		if ( wield != NULL && wield->item_type == ITEM_WEAPON )
		{
			dt += wield->value[3];
			if (wield->value[0] >= 1)
			{
				/* Look, just don't ask...   KaVir */
				if (wield->value[0] >= 1000)
					sn = wield->value[0] - ((wield->value[0] / 1000) * 1000);
				else
					sn = wield->value[0];
				if (sn != 0 && victim->position == POS_FIGHTING)
					(*skill_table[sn].spell_fun) (sn,wield->level,ch,victim);
			}
		}
    }

    if ( victim == NULL || victim->position != POS_FIGHTING )
		return;

    if ( ch->fighting != victim || dt == gsn_backstab || dt == gsn_garotte || dt == gsn_headbutt || dt == gsn_circle)
		return;

    maxcount = number_attacks(ch, victim);
if (blink && IS_CLASS(victim,CLASS_GOLEM) && !IS_STONE(victim))
maxcount -= 3;
else if (blink && !IS_CLASS(victim,CLASS_GOLEM))
maxcount += 3;
    if (!IS_NPC(ch))
    {
		chance = 0;
		if (wield != NULL && wield->item_type == ITEM_WEAPON)
		{
			int tempnum = wield->value[3];
			chance = (ch->wpn[tempnum]) * 0.5;
		}
		else
			chance = (ch->wpn[0]) * 0.5;
		if (number_percent() <= chance)
			maxcount += 1;
    }
    if (wieldorig == 3)
		maxcount += 1;
    for (countup = 0; countup <= maxcount; countup++)
    {
		wieldtype = wieldorig;
        if ( wieldorig == 15 ) 
		{ 
			if (wieldie == 2) 
				wieldtype = 8; 
			else	
				wieldtype = 4;
		}
		if ( wieldorig == 14 )
		{
			if (wieldie == 2)
				wieldtype = 8;
			else if (wieldie == 1)
				wieldtype = 4;
			else if (wieldie == 3)
				wieldtype = 2;
			else
				wieldtype = 1;
		}
		if ( wieldorig == 13 )
		{
			if ( wieldie == 1)
				wieldtype = 8;
			else if (wieldie == 2)
				wieldtype = 4;
			else wieldtype = 1;
		}
		if ( wieldorig == 12 )
		{
			if ( wieldie == 1 )
				wieldtype = 8;
			else
				wieldtype = 4;
		}
		if ( wieldorig == 11 )
		{
			if ( wieldie == 1 )
				wieldtype = 8;
			if ( wieldie == 2 )
				wieldtype = 2;
			else wieldtype = 1;
		}
		if ( wieldorig == 10 )
		{
			if ( wieldie == 1 )
				wieldtype = 8;
			else wieldtype = 2;
		}
		if ( wieldorig == 9 )
		{
			if ( wieldie == 1 )
				wieldtype = 8;
			else wieldtype = 1;
		}
		if ( wieldorig == 7 )
		{
			if ( wieldie == 1 )
				wieldtype = 4;
			else if ( wieldie == 2 )
				wieldtype = 2;
			else wieldtype = 1;
		}
		if ( wieldorig == 6 )
		{
			if ( wieldie == 1 || wieldie == 2)
				wieldtype = 2;
			else wieldtype = 4;
		} 
		if ( wieldorig == 5 )
		{
	        if ( wieldie == 1)
				wieldtype = 4;
			else wieldtype = 1;
		}
		if ( wieldorig == 3 )
		{
	        if ( wieldie == 2 || wieldie == 4)
				wieldtype = 2;
			else wieldtype = 1;
		}
		one_hit( ch, victim, -1, wieldtype );
		if ( victim == NULL || victim->position != POS_FIGHTING )
			return;
		if ( ch->fighting != victim )
			return;
    }/* End of for (countup = 0; countup <= maxcount; countup++)*/
if (is_affected(ch, skill_lookup("wind slash")))
    damage(ch,victim,dice(60,60),skill_lookup("wind slash"));
if (is_affected(ch, skill_lookup("elf fire")))
    damage(ch,victim,dice(40,40),skill_lookup("elf fire"));
if (is_affected(ch, skill_lookup("sand blast")))
    damage(ch,victim,dice(65,50),skill_lookup("sand blast"));
    if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FANGS))
		one_hit( ch, victim, ( TYPE_HIT + 5 ), 0 );
    if ( victim == NULL || victim->position != POS_FIGHTING )
		return;
    if (!IS_NPC(ch))
    {
		if (IS_VAMPAFF(ch,VAM_HORNS) && number_range(1,5) == 1)
			multi_hit( ch, victim, gsn_headbutt );
        if (IS_VAMPAFF(ch,VAM_TAIL) && number_range(1,5) == 1)
			multi_hit( ch, victim, gsn_sweep );
        if (IS_VAMPAFF(ch,VAM_HEAD) && number_range(1,6) == 1)
			multi_hit( ch, victim, gsn_fangs );
		if (IS_SET(ch->newbits,NEW_QUILLS) && number_range(1,5) == 1)
			multi_hit(ch,victim,gsn_quills);
    }
	if (!IS_NPC(ch)&& IS_CLASS(ch,CLASS_WEREWOLF))
	{
		if (IS_SET(ch->newbits,NEW_SLAM) && number_range(1,4)==1)
		multi_hit(ch,victim,gsn_cheapshot);
        
	}
    if (!IS_NPC(ch))
    {
        if (IS_VAMPAFF(ch,VAM_WINGS) && number_range(1,6) == 1)
			multi_hit( ch, victim, gsn_buffet );
    }        
    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_WEREWOLF) && ch->power[DISC_WERE_RAPT] > 0)
    {
		multi_hit(ch,victim,gsn_rfangs);
    }            
    if ( victim == NULL || victim->position != POS_FIGHTING )
		return;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_VAMPAFF(ch, VAM_SERPENTIS))
		spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
    else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] > 0)
		spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
    else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
		spell_poison(gsn_poison,(ch->level*number_range(10,20)),ch,victim);
	else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && (ch->pcdata->powers[NPOWER_NINGENNO] > 4))
		spell_poison(gsn_poison, (ch->level*number_range(15,20)),ch,victim);
	if (victim->itemaffect < 1)
		return;
    if (IS_NPC(victim) || victim->spl[1] < 4)
		level = victim->level;
    else level = (victim->spl[1] * 0.25);
    
    if (IS_SET(victim->newbits, NEW_MONKFLAME) && !IS_AFFECTED(ch, AFF_FLAMING))
    {
		if (number_percent() > 35)
		{
	        SET_BIT(ch->affected_by, AFF_FLAMING);
			if (!IS_CLASS(ch, CLASS_WEREWOLF))
			{
				act("Your flaming hands catch $n on fire!", ch, NULL, victim, TO_VICT);
				act("$N's flaming hands catch $n on fire!", ch, NULL, victim, TO_NOTVICT);
				act("$N's flaming hands catch you on fire!.", ch, NULL, victim, TO_CHAR);
			}
			else
			{
				act("Your flaming claws catch $n on fire!", ch, NULL, victim, TO_VICT);
				act("$N's flaming claws catch $n on fire!", ch, NULL, victim, TO_NOTVICT);
				act("$N's flaming claws catch you on fire!.", ch, NULL, victim, TO_CHAR);
			}
		}
		else
			return;
    }
level *= .2;
    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) && ch->position == POS_FIGHTING)
		if ( (sn = skill_lookup( "lightning bolt" ) ) > 0)
			(*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD) && ch->position == POS_FIGHTING)
		if ( (sn = skill_lookup( "fireball" ) ) > 0)
			(*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD) && ch->position == POS_FIGHTING)
		if ( (sn = skill_lookup( "chill touch" ) ) > 0)
			(*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) && ch->position == POS_FIGHTING)
		if ( (sn = skill_lookup( "acid blast" ) ) > 0)
			(*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) && ch->position == POS_FIGHTING)
		if ( (sn = skill_lookup( "chaos blast" ) ) > 0)
			(*skill_table[sn].spell_fun) (sn,level,victim,ch);
    return;
}/* End of void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) */

int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int insp = countinspires(ch) - countinspires(victim);
    int count = 1;
insp /= 2;
if (insp > 4) insp = 4;
if (insp < 4) insp = 4;
    if ( IS_NPC(ch) )
    {
		if (ch->level >= 50)
			count += 1;
		count += (ch->level / 500)*3;
		if (ch->level >= 100)
			count += 1;
		if (ch->level >= 200)
			count += 1;
		if (ch->level >= 300)
			count += 1;
		count += insp;
		return count;
    }
    /* stance bonuses */
    /* if fighting a mob... stance bonuses */
    if (IS_NPC(victim))
    {
		if ( IS_STANCE(ch, STANCE_VIPER) && number_percent() > ch->stance[STANCE_VIPER] * 0.5)
			count += 1;
		else if ( IS_STANCE(ch, STANCE_MANTIS) && number_percent() > ch->stance[STANCE_MANTIS] * 0.5)
			count += 1;
    	else if ( IS_STANCE(ch, STANCE_TIGER) && number_percent() > ch->stance[STANCE_TIGER] * 0.5)
			count += 1;
    	else if ( IS_STANCE(ch, STANCE_WOLF) && number_percent() > ch->stance[STANCE_WOLF] * .5)
			count += 2;
if (IS_GOLEM(ch) && (IS_STANCE(ch,STANCE_MANTIS) ||
IS_STANCE(ch,STANCE_TIGER))) count += 1;
    }
    /* PC stance bonuse for class discs. */
if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_MONK) && (weather_info.sunlight ==
SUN_LIGHT)) count += 5;
else if (IS_CLASS(ch,CLASS_MONK)) count += 2;
    if (!IS_NPC(ch))
    {   
		if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) 
			count += 1;
if (IS_MOOGLE(ch))
    count+= 5;
	if (is_affected(ch, skill_lookup("haste")))
			count += 3;
        if (IS_CLASS(ch, CLASS_AMAZON) && (ch->pcdata->powers[PAMAZON] >= 3))
                        count += 1;
        if (IS_CLASS(ch, CLASS_AMAZON) && (ch->pcdata->powers[PAMAZON] >= 6))
                        count += 1;
        if (IS_CLASS(ch, CLASS_VAMPIRE) && victim->power[DISC_VAMP_CELE] >= 3)
			count += 1;
        if (IS_CLASS(ch, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] >= 3)
			count += 1;
    	if (IS_CLASS(ch,CLASS_WEREWOLF) && ch->power[DISC_WERE_BOAR] > 6)
			count += 1;
        if (IS_CLASS(ch, CLASS_DROW) && (IS_SET(ch->special, SPC_DROW_WAR) || ch->generation < 3))
			count += 2;

if (IS_CLASS(ch, CLASS_MECHA) && (ch->pcdata->powers[MPOWER_PSCIONICS] >= 1))
                        count += 2;
if (IS_CLASS(ch, CLASS_MECHA) && (ch->pcdata->powers[MPOWER_COMBAT] >= 1))
                        count += 2;
if (IS_CLASS(ch, CLASS_MECHA) && (ch->pcdata->powers[MPOWER_PILOTING] >= 3))
                        count += 2;
        if (IS_CLASS(ch, CLASS_BERSERK) && (ch->bloodlust > 20))
			count +=1;
        if (IS_CLASS(ch, CLASS_BERSERK) && (ch->bloodlust > 40))
			count +=2;
        if (IS_CLASS(ch, CLASS_BERSERK) && (ch->bloodlust > 60))
			count +=3;
         if (IS_CLASS(ch, CLASS_BERSERK) && (ch->bloodlust > 80))
			count +=4;
 if (IS_CLASS(ch, CLASS_BERSERK) && (ch->bloodlust > 100))    
                        count +=5;
	if (IS_CLASS(ch, CLASS_NINJA) &&
(ch->pcdata->powers[NPOWER_CHIKYU] >= 5))
			count += 2;
	if (IS_CLASS(ch, CLASS_NINJA) && (ch->pcdata->powers[NPOWER_CHIKYU] >= 3))
                        count += 2;
	if (IS_CLASS(ch, CLASS_MAGE) && (ch->max_mana >= 10000))
			count += 1;
	if (IS_CLASS(ch, CLASS_MAGE) && (ch->max_mana >= 20000))
			count += 1;
	if (IS_CLASS(ch, CLASS_MONK) && (ch->pcdata->powers[PMONK] >= 8))
			count += 1;
	if (IS_CLASS(ch, CLASS_MONK)) count += number_range(0, 1);
        if (IS_CLASS(ch, CLASS_JEDI) && (ch->pcdata->powers[JPOWER_SPIRIT] >= 5))
                        count += 1;
        if (IS_CLASS(ch, CLASS_JEDI) && (ch->pcdata->powers[JPOWER_MIND] >= 4))
                        count += 1;
        if (IS_CLASS(ch, CLASS_JEDI) && (ch->pcdata->powers[JPOWER_BODY] >= 6))
                        count += 1;
        if (IS_CLASS(ch, CLASS_JEDI) && (ch->pcdata->powers[JPOWER_TECHNIQUE] >= 9))
                        count += 1;
        if (IS_INFORM(ch,GOLEM_RUBY)) count += 25; //haha they hit 0 every time
	if (IS_INFORM(ch,GOLEM_CRYSTAL))
		count -= 4; /* this is a defense form */
	else if (IS_INFORM(ch,GOLEM_OBSIDIAN))
		count += 7; /* pure offense */
	if (IS_INFORM(ch,GOLEM_OBSIDIAN) && !IS_CLASS(victim,CLASS_SHAPE))
		count += 1;
	if (IS_INFORM(victim,GOLEM_OBSIDIAN))
		count += 1;
	if (ch->generation < 3)
			count += 1;
        if (ch->generation <= 1)
   			count +=3;
	if (ch->max_move >= 4000)
			count += 1;
	if (ch->max_move >= 8000)
			count += 2;
if (IS_NPC(victim) && !IS_NPC(ch) && ch->class != 0)
	count += 5;
      /* this is for landers But too many breaks eq too easy
  if (ch->max_hit >= 200)
		count +=1;
        if (ch->max_hit >= 500)
                count +=3;
        if (ch->max_hit >= 1000)
                count +=5;*/
        if (ch->max_hit >= 1500)
                count +=3;
        if (ch->max_move >= 12000)
			count += 3;
if (IS_CLASS(ch, CLASS_AMAZON) && IS_CLASS(victim, CLASS_SHAPE))
  count += 4;
	if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_MONK))
			count -= 1;
        if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_DROW))
                  count -= 1;
        if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_HIGHLANDER))
                  count -= 1;
        if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_NINJA))
                  count -= 2;
	if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_MECHA))
		count -=2;
        if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_DEMON))
                  count -= 1;
	if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_JEDI) && (victim->pcdata->powers[JPOWER_SPIRIT] > 3))
		count -= 1;
	if (IS_CLASS(ch, CLASS_JEDI)) count += 1;
    }
	if (IS_CLASS(ch, CLASS_DEMON)) count += 1;
	if (IS_CLASS(ch, CLASS_SHAPE)) {if ((get_weapon_type(ch) == UNARMED && will_use(ch, "Enhanced Unarmed")) || (get_weapon_type(ch) == SWORD && will_use(ch, "Enhanced SWORD")) || (get_weapon_type(ch) == DAGGER && will_use(ch, "Enhanced Dagger")) || (get_weapon_type(ch) == WHIP && will_use(ch, "Enhanced Whip")) || (get_weapon_type(ch) == MACE && will_use(ch, "Enhanced Mace")) || (get_weapon_type(ch) == FIREARM && will_use(ch, "Enhanced FireArm")))
	count += 1;}
	if (will_use(ch, "Speed")) count += 1;
	if (will_use(ch, "Super Speed")) count += 2;
	if (will_use(ch, "Lightning Speed")) count += 1;
	if (will_use(ch, "Unbelieveable Power")) count += 1;
	if (IS_INFORM(victim,GOLEM_CRYSTAL)) count -= 3;

count += insp;

for (classcon = config_first; classcon; classcon = classcon->next)
 if (!IS_NPC(ch) && classcon->classbit == ch->class) count += classcon->attacks;
    /* minimum of 1 attack */
    if ( count < 1)
		count = 1;
    return count;
}/* End of int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim ) */


/* Hit one guy once */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype)
{
	OBJ_DATA *wield;
    char buf[MAX_STRING_LENGTH];
    int victim_ac;
    int thac0;
    int thac0_00;
    int thac0_32;
    int dam, wdam;
    int diceroll;
    int level;
    int attack_modify;
    int right_hand;
    int improveskill;
	
	if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
		return;
if (is_safe(ch,victim)) return;
    if (ch->power[DISC_DAEM_HELL] > 3 && number_range(1, 50-ch->generation) == 1 && ch->fighting != NULL)
    {
        wdam = dice(ch->generation + 5, 10);
		sprintf(buf,"#1%s breathes out a cloud of fire at you#n.[%d]", ch->name, wdam);
		sprintf(buf,"#1%s breathes out a cloud of fire at %s#n.",ch->name,victim->name );
		sprintf(buf,"#1You breathe out a cloud of fire at %s#n.[%d]",victim->name, wdam );
		hurt_person(ch, victim, wdam);
//        victim->hit -= wdam;
		update_pos(victim);
    }

    /*
     * Figure out the type of damage message.
     */
    if (handtype == 8)
    {
		wield = get_eq_char( ch, WEAR_FOURTH );
		right_hand = 8;
    }
    else if (handtype == 4)
    {
		wield = get_eq_char( ch, WEAR_THIRD );
		right_hand = 4;
    }
    else if (handtype == 2)
    {
    	wield = get_eq_char( ch, WEAR_HOLD );
		right_hand = 2;
    }
    else if (handtype == 0) 
    {
		wield = NULL;
		right_hand = 0;
    }
    else
    {
    	wield = get_eq_char( ch, WEAR_WIELD );
		right_hand = 1;
    }
    if ( dt == TYPE_UNDEFINED )
    {
	dt = TYPE_HIT;
	if ( wield != NULL && wield->item_type == ITEM_WEAPON )
		dt += wield->value[3];
	if (dt == TYPE_HIT && wield)
	{
	    send_to_char("Bugged WEAPON! NOTE TO IMPS NOW!\r\n",ch);
	    dt += 3;
	}
    }

    if (ch->wpn[dt-1000] > 5)
		level = (ch->wpn[dt-1000] / 5);
    else
		level = 1;
    if (level > 40)
		level = 40;

    /* Calculate to-hit-armor-class-0 versus armor.*/
     
    if ( IS_NPC(ch) )
    {
		thac0_00 = 20;
		thac0_32 =  0;
    }
    else
    {
		thac0_00 = SKILL_THAC0_00;
		thac0_32 = SKILL_THAC0_32;
    }
    thac0 = interpolate( level, thac0_00, thac0_32 ) - char_hitroll(ch);
    victim_ac = UMAX( -100, char_ac(victim) / 10 );
    if ( !can_see( ch, victim ) )
		victim_ac -= 4;
     /* The moment of excitement! */
    while ( ( diceroll = number_bits( 5 ) ) >= 20 );

    if ( diceroll == 0 || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
    {
	/* Miss. */
		damage( ch, victim, 0, dt );
		tail_chain( );
		improveskill = 0;
		if (!is_safe(ch,victim))
		{
			while (improveskill < 1)
			{
				improve_wpn(ch,dt,right_hand);
    			improve_stance(ch);
if (IS_GOLEM(ch)) improve_stance(ch);
				improveskill++;
			}
		}
		return;
    }

     /* Hit Calc damage. */
    if ( IS_NPC(ch) )
    {
		dam = number_range( ch->level / 2, ch->level * 3 / 2 );
		if ( wield != NULL )
			dam += dam * 0.5;
    }
    else
    {
		if ( IS_VAMPAFF(ch,VAM_CLAWS) && wield == NULL)
			dam = dice( 30, 30 );
		else if ( IS_SET(ch->newbits, NEW_MONKADAM) && wield == NULL)
			dam = dice( 35, 35 );
		else if ( wield != NULL && wield->item_type == ITEM_WEAPON )
			dam = number_range( wield->value[1], wield->value[2] );
		else
			dam = dice( 1, 5 );
    }
	/* Bonuses.*/
    dam += ((char_damroll(ch) + char_hitroll(ch)) / 2);

if (dt == gsn_cheapshot) dam = NR(400,1250);
if (dt == gsn_buffet) dam = NR(400,950);
if (dt == gsn_sweep) dam = NR(400,950);
if (dt == gsn_quills) dam =  NR(400,950);
if (dt == gsn_fangs) dam = NR(400,950);
if (dt == gsn_rfangs) dam = NR(400,950);

    if ( !IS_AWAKE(victim) )
	dam *= 2;
    if ( dt == gsn_backstab )
	dam *= number_range(12,18);
    if ( dt == gsn_circle && wield != NULL)
        if ( wield->value[0] != 2 )
           dam *= 2+ (ch->level / 10);
        else
           dam *=2 + (ch->level / 8);
if (dt == gsn_circle) dam /= 2.25;
    if ( !IS_NPC(ch) && dt >= TYPE_HIT)
    {
	if (IS_CLASS(ch, CLASS_HIGHLANDER))
	    dam = dam + (dam * ((ch->wpn[dt-1000]+1) / 170));
	else
	    dam = dam + (dam * ((ch->wpn[dt-1000]+1) / 130));
    }
    if ( dam <= 0 )
		dam = 1;

	if (IS_CLASS(ch, CLASS_DEMON))
	{
		if (ch->power[DISC_DAEM_TEMP] > 5)
			dam *= 2.3;
		dam *= 2.1;
	}
        else if (IS_CLASS(ch, CLASS_BERSERK))
        {
		if (ch->bloodlust >= 2)
			dam *= 1.1;
                else if (ch->bloodlust >= 8)
                      dam *= 1.2;
 else if (ch->bloodlust >= 14)
dam *= 1.3;  
 else if (ch->bloodlust >= 22)
dam *= 1.4;     
 else if (ch->bloodlust >= 28)
dam *= 1.6; 
 else if (ch->bloodlust >= 36)
dam *= 1.9;
 else if (ch->bloodlust >= 40)
dam *= 2.1; 
 else if (ch->bloodlust >= 50)
dam *= 2.3; 
 else if (ch->bloodlust >= 60)
dam *= 2.5; 
 else if (ch->bloodlust >= 70)
dam *= 3;
 else if (ch->bloodlust >= 80)
dam *= 3.5; 
 else if (ch->bloodlust >= 90)
dam *= 4;                    
 else if (ch->bloodlust >= 100)
dam *= 5;
}                      
	else if (IS_CLASS(ch, CLASS_WEREWOLF))
	{
		if (ch->power[DISC_WERE_PAIN] >= 10)
			dam *= 1.1;
		else if (ch->power[DISC_WERE_PAIN] >= 7)
			dam *= .9;
		else if (ch->power[DISC_WERE_PAIN] >= 3)
			dam *= .8;
		if(!IS_NPC(victim)) 
		{
		if (ch->rage >= 1)
		dam *= 1.1;
		}
		if (IS_NPC(victim)) dam *= 1.1;
	}
	else if (IS_CLASS(ch, CLASS_MONK))
	{
		//daytime/nighttime dam put in cap_dam ()
		if (ch->pcdata->powers[PMONK] >= 12)
			dam *= 1.9;
		else if (ch->pcdata->powers[PMONK] >= 6)
			dam *= 1.8;
		else if (ch->pcdata->powers[PMONK] >= 3)
			dam *= 1.7;
		if(!IS_NPC(victim)) dam *= 1.6;
                if (IS_NPC(victim)) dam *= 1.4; 
	}
	else if (IS_CLASS(ch, CLASS_VAMPIRE))
	{

		if (ch->pcdata->rank == AGE_METHUSELAH) 
			dam *= 2;
		else if (ch->pcdata->rank == AGE_ELDER)
			dam *= 1.9;
		else if (ch->pcdata->rank == AGE_ANCILLA)
			dam *= 1.8;
		else
			dam *= 1.7;
        if (weather_info.sunlight == SUN_LIGHT)
            dam *= 0.35;
        if (weather_info.sunlight == SUN_DARK)
            dam *= 3.1;
        }        
		 if (IS_CLASS(ch, CLASS_JEDI))
		 {
			if ((ch->generation == 4) && IS_SET(ch->special, SPC_PRINCE))
				dam *= 2.6;
			else if (ch->generation == 4)
				dam *= 2.5;
			else if (ch->generation < 3)
				dam *= 2.4;
			else
				dam *= 2.3;
			if (ch->pcdata->powers[JPOWER_SPIRIT] > 4)
				dam *= 1.5;

			if (ch->pcdata->powers[JEDI_STATE] > 0)
				dam *= 1 + ((ch->pcdata->powers[JEDI_STATE] )*.15);

		 }

        if (IS_CLASS(ch, CLASS_MAGE))
        {
         if (ch->generation <= 2)
            dam *= 1.2;
              else
            dam *= 1.1;
	if (check_poly(ch, POLY_HUGE_SERPENT) || check_poly(ch, POLY_WYVERN))
		dam *= 3;
        } 
	  if (IS_CLASS(ch, CLASS_SHAPE))
	{
	    if ((get_weapon_type(ch) == UNARMED && will_use(ch, "Enhanced Unarmed")) || (get_weapon_type(ch) == SWORD && will_use(ch, "Enhanced SWORD")) || (get_weapon_type(ch) == DAGGER && will_use(ch, "Enhanced Dagger")) || (get_weapon_type(ch) == WHIP && will_use(ch, "Enhanced Whip")) || (get_weapon_type(ch) == MACE && will_use(ch, "Enhanced Mace")) || (get_weapon_type(ch) == FIREARM && will_use(ch, "Enhanced FireArm")))
		dam *= 1.4;
		if (will_use(ch, "Spirit")) dam *= 2;
		dam *= 1.3;
            if (will_use(ch, "Unbelieveable Power")) dam *= 1.3;
	    if (will_use(ch, "Enhanced Damage")) dam *= 1.6;
	    if (will_use(ch, "Unnatural Strength")) dam *= 1.4;
	}
	  if (will_use(victim, "Rock-Like Skin")) dam /= 1.4;
	  if (will_use(victim, "Immobility")) dam /= 1.6;
//	if (IS_CLASS(victim, CLASS_MONK)) dam /= 1.4;
	if (IS_CLASS(ch, CLASS_NINJA))
	{
         if (ch->generation <= 3)
            dam *= 1.7;
		 else
			 dam *= 1.8;
		dam *= 1.6;//mabey now they'll acually hit their damcap..
        }

 if (IS_CLASS(ch, CLASS_MECHA))
        {


     if (ch->generation <= 3)
            dam *= 2.9;
                 else
                         dam *= 2.4;
                dam *= 2.2;
if (ch->pcdata->powers[MPOWER_PSCIONICS] >= 1)
	   dam *=2.8;
        }
        if (IS_CLASS(ch, CLASS_AMAZON))
        {
        if (ch->generation <= 3)
           dam *= 2.8;
                 else 
            dam *= 2.6;
	if (ch->pcdata->powers[PAMAZON] > 8 && get_weapon_type(ch) == UNARMED)
	    dam *= 1.6;
	else dam *= 1.4;
        }
        if (IS_CLASS(ch, CLASS_DROW))
        {
         if (IS_SET(ch->newbits, NEW_DARKNESS))
            dam *= 2.4;
                else
                         dam *= 1.8;
        }
         if (IS_CLASS(ch, CLASS_HIGHLANDER))
        {
         if (ch->max_hit >= 15000)
            dam *= 2.1;
        }

         if (IS_CLASS(victim, CLASS_WEREWOLF) && !IS_NPC(victim) &&
		victim->rage > 0)
	 {
		 dam += number_range(1, victim->rage);
               if (IS_ITEMAFF(ch, ITEMA_RIGHT_SILVER) || IS_ITEMAFF(ch,
                   ITEMA_LEFT_SILVER) )
               {
                    if (victim->siltol >= 100)
                       dam *= 1.6;
                    else if (victim->siltol >= 75)
                       dam *= 1.9;
                    else if (victim->siltol >= 50)
                       dam *= 2;
                    else if (victim->siltol >= 25)
                       dam *= 2.3;
                    else if (victim->siltol >= 10)
                      dam *= 2.5;
                    else  dam *= 3;
               }

	 }
	/* Other Resistances */
    if (IS_ITEMAFF(victim, ITEMA_RESISTANCE))
	    dam /= 2;

/* hrmm code removed, read it carefully!
	if ( !IS_NPC(ch) && IS_NPC(victim))
	{
		if (ch->stance[0])
			dam = dam * 1.1;
		else if (ch->stance[STANCE_VIPER])
			dam = dam * 1.24;
		else if (ch->stance[STANCE_CRANE])
			dam = dam * 1.26;
		else if (ch->stance[STANCE_MONGOOSE])
			dam = dam * 1.4;
		else if (ch->stance[STANCE_BULL])
			dam = dam * 1.30;
		else if (ch->stance[STANCE_CRAB])
			dam = dam * 1.36;
		else if (ch->stance[STANCE_MANTIS])
			dam = dam * 1.52;
		else if (ch->stance[STANCE_TIGER])
			dam = dam * 1.58;
		else if (ch->stance[STANCE_DRAGON])
			dam = dam * 1.55;
		else if (ch->stance[STANCE_MONKEY])
			dam = dam * 1.45;
		else if (ch->stance[STANCE_SWALLOW])
			dam = dam * 1.55;
		else if (ch->stance[STANCE_WOLF])
			dam = dam * 1.7;
	}
*/
    if ( !IS_NPC(ch) && !IS_NPC(victim) )
    {
		ch->fight_timer = 20;
		victim->fight_timer = 20;
/*
		if (ch->stance[0] && victim->stance[0])
			dam = dam;
		else if (ch->stance[0] && victim->stance[STANCE_VIPER])
			dam = dam * 0.88;
		else if (ch->stance[0] && victim->stance[STANCE_CRANE])
			dam = dam * 0.85;
		else if (ch->stance[0] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 0.85;
		else if (ch->stance[0] && victim->stance[STANCE_BULL])
			dam = dam * 0.85;
		else if (ch->stance[0] && victim->stance[STANCE_CRAB])
			dam = dam * 0.85;
		else if (ch->stance[0] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.73;
		else if (ch->stance[0] && victim->stance[STANCE_TIGER])
			dam = dam * 0.73;
		else if (ch->stance[0] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.75;
		else if (ch->stance[0] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[0] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.7;
		else if (ch->stance[0] && victim->stance[STANCE_WOLF])
			dam = dam * 0.75;
		else if (ch->stance[STANCE_VIPER] && victim->stance[0])
			dam = dam * 1.12;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_VIPER])
			dam = dam;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_CRANE])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_BULL])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_CRAB])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_TIGER])
			dam = dam * 0.79;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.73;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.88;
		else if (ch->stance[STANCE_VIPER] && victim->stance[STANCE_WOLF])
			dam = dam * 0.76;
		else if (ch->stance[STANCE_CRANE] && victim->stance[0])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_VIPER])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_CRANE])
			dam = dam;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_MONGOOSE])
			dam = dam;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_BULL])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_CRAB])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.76;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_TIGER])
			dam = dam * 0.82;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.97;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_CRANE] && victim->stance[STANCE_WOLF])
			dam = dam * 0.79;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[0])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_VIPER])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_CRANE])
			dam = dam;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_MONGOOSE])
			dam = dam;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_BULL])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_CRAB])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.76;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_TIGER])
			dam = dam * 0.82;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.97;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_MONGOOSE] && victim->stance[STANCE_WOLF])
			dam = dam * 0.79;
		else if (ch->stance[STANCE_BULL] && victim->stance[0])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_VIPER])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_CRANE])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_BULL])
			dam = dam;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_CRAB])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.85;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_TIGER])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.85;
		else if (ch->stance[STANCE_BULL] && victim->stance[STANCE_WOLF])
			dam = dam * 0.76;
		else if (ch->stance[STANCE_CRAB] && victim->stance[0])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_VIPER])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_CRANE])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_BULL])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_CRAB])
			dam = dam;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.85;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_TIGER])
			dam = dam * 0.85;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.79;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.79;
		else if (ch->stance[STANCE_CRAB] && victim->stance[STANCE_WOLF])
			dam = dam * 0.85;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[0])
			dam = dam * 1.27;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_VIPER])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_CRANE])
			dam = dam * 1.24;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 1.24;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_BULL])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_CRAB])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_MANTIS])
			dam = dam;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_TIGER])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_DRAGON])
			dam = dam;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_MONKEY])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_SWALLOW])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_MANTIS] && victim->stance[STANCE_WOLF])
			dam = dam;
		else if (ch->stance[STANCE_TIGER] && victim->stance[0])
			dam = dam * 1.27;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_VIPER])
			dam = dam * 1.21;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_CRANE])
			dam = dam * 1.18;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 1.18;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_BULL])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_CRAB])
			dam = dam * 1.25;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_MANTIS])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_TIGER])
			dam = dam;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_DRAGON])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_SWALLOW])
			dam = dam * 1.03;
		else if (ch->stance[STANCE_TIGER] && victim->stance[STANCE_WOLF])
			dam = dam * 1.30;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[0])
			dam = dam * 1.3;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_VIPER])
			dam = dam * 1.27;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_CRANE])
			dam = dam * 1.03;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 1.03;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_BULL])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_CRAB])
			dam = dam * 1.21;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_MANTIS])
			dam = dam;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_TIGER])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_DRAGON])
			dam = dam;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_DRAGON] && victim->stance[STANCE_WOLF])
			dam = dam * 0.93;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[0])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_VIPER])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_CRANE])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_MONGOOSE])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_BULL])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_CRAB])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_TIGER])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_DRAGON])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_SWALLOW])
			dam = dam;
		else if (ch->stance[STANCE_MONKEY] && victim->stance[STANCE_WOLF])
			dam = dam;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[0])
			dam = dam * 1.3;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_VIPER])
			dam = dam * 1.12;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_CRANE])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 1.09;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_BULL])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_CRAB])
			dam = dam * 1.21;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_MANTIS])
			dam = dam * 0.91;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_TIGER])
			dam = dam * 0.97;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_DRAGON])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_SWALLOW])
			dam = dam;
		else if (ch->stance[STANCE_SWALLOW] && victim->stance[STANCE_WOLF])
			dam = dam * 1.06;
		else if (ch->stance[STANCE_WOLF] && victim->stance[0])
			dam = dam * 1.27;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_VIPER])
			dam = dam * 1.24;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_CRANE])
			dam = dam * 1.21;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_MONGOOSE])
			dam = dam * 1.21;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_BULL])
			dam = dam * 1.24;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_CRAB])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_MANTIS])
			dam = dam;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_TIGER])
			dam = dam * 0.97;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_DRAGON])
			dam = dam * 1.15;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_MONKEY])
			dam = dam;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_SWALLOW])
			dam = dam * 0.94;
		else if (ch->stance[STANCE_WOLF] && victim->stance[STANCE_WOLF])
			dam = dam;
*/    } 

		if (IS_CLASS(victim, CLASS_JEDI))
		{
		    if (victim->pcdata->powers[JEDI_STATE] > 5)
			dam *= .75;
//			dam -= (ch->pcdata->powers[JEDI_STATE]) * 50;
		}

	if (!IS_NPC(ch) && wield != NULL && !balancing)
	{

		if (number_range(1,6) == 2)
		{
    		if (ch->fighting == victim && IS_WEAP(wield,WEAPON_VAMPIRIC))
			{
				wdam = number_range(1,wield->level / 5 + 1);
				act("$p draws life from $n.",victim,wield,NULL,TO_ROOM);
				act("You feel $p drawing your life away.",victim,wield,NULL,TO_CHAR);
				hurt_person(ch, victim, wdam);
//				victim->hit -= wdam;
				update_pos( victim );
				ch->alignment = UMAX(-1000,ch->alignment - 1);
				ch->hit += wdam/2;
			}
			if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FLAMING) || IS_WEAP(wield, WEAPON_ELE_FLAME) )  )
			{
				wdam = number_range(1,wield->level / 4 + 1);
				act("$n is burned by $p.",victim,wield,NULL,TO_ROOM);
				act("$p sears your flesh.",victim,wield,NULL,TO_CHAR);
				fire_effect( (void *) victim,wield->level/2,wdam,TARGET_CHAR);
				hurt_person(ch, victim, wdam);
//				victim->hit -= wdam;
				update_pos( victim );
			}
			if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FROST) || IS_WEAP(wield, WEAPON_ELE_WATER) ) )
			{
				wdam = number_range(1,wield->level / 6 + 2);
				act("$p freezes $n.",victim,wield,NULL,TO_ROOM);
				act("The cold touch of $p surrounds you with ice.",
				victim,wield,NULL,TO_CHAR);
				cold_effect(victim,wield->level/2,wdam,TARGET_CHAR);
				hurt_person(ch, victim, wdam);
//				victim->hit -= wdam;
				update_pos( victim );
			}
			if (ch->fighting == victim && IS_WEAP(wield, WEAPON_ELE_EARTH) )
			{
				wdam = number_range(1,wield->level / 5 + 2);
				act("$p covers $n in acid.",victim,wield,NULL,TO_ROOM);
				act("The corrosive touch of $p surrounds you with acid.",
				victim,wield,NULL,TO_CHAR);
				acid_effect(victim,wield->level/2,wdam,TARGET_CHAR);
				hurt_person(ch, victim, wdam);
//				victim->hit -= wdam;
				update_pos( victim );		
			}
			if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_SHOCKING) || IS_WEAP(wield, WEAPON_ELE_AIR) ) )
			{
				wdam = number_range(1,wield->level/5 + 2);
				act("$n is struck by lightning from $p.",victim,wield,NULL,TO_ROOM);
				act("You are shocked by $p.",victim,wield,NULL,TO_CHAR);
				shock_effect(victim,wield->level/2,wdam,TARGET_CHAR);
				hurt_person(ch, victim, wdam);
//				victim->hit -= wdam;
				update_pos( victim );
			}
			if (ch->fighting == victim && IS_WEAP(wield, WEAPON_SUNBLADE))
			{
				wdam = number_range(wield->level/2, wield->level+1);
				if (victim->alignment > -100)
					wdam = 0;
				if (victim->alignment > -300)
					wdam = wdam / 2;
				if (victim->alignment == -1000)
					wdam *= 2;
				if (victim->class == CLASS_VAMPIRE)
					wdam *= 3;
				if (wdam > 0)
				{
					act("$n's flesh is seared by the radiant aura of $p.",victim,wield,NULL,TO_ROOM);
					act("Your flesh is seared by the radiant aura of $p.",victim,wield,NULL,TO_CHAR);
					hurt_person(ch, victim, wdam);
//					victim->hit -= wdam;
					update_pos( victim );
					ch->alignment = UMAX(-1000,ch->alignment + 1);
				}
			}
			if (ch->fighting == victim && IS_WEAP(wield, WEAPON_DRAGONLANCE))
			{
				wdam = number_range(wield->level/2, wield->level+1)*4;
				wdam = 0;
				if (wdam > 0)
				{
					act("$n's flesh is torn apart by $p.",victim,wield,NULL,TO_ROOM);
					act("Your flesh is torn apart by $p.",victim,wield,NULL,TO_CHAR);
					hurt_person(ch, victim, wdam);
//					victim->hit -= wdam;
					update_pos( victim );
				}
			}
		}
	}
    dam = cap_dam(ch,victim,dam);
    if ( dt == gsn_garotte )
		dam *= (number_range(2,5));
// 6 attacks every round of 200 damage per hit is unfair!
   if (IS_CLASS(ch, CLASS_DROW) && ch->cur_form == FRM_DROWSPIDER && 
		dt == gsn_spiderform) 
		dam /= 2.3;
    attack_modify = dice(1, 500);
    randomize_damage(ch,dam,attack_modify);
    damage( ch, victim, dam, dt );
    if (IS_SET(victim->newbits, NEW_LWRATH) && dt != gsn_garotte)
    {
	    act("You absorb the damage and $n is struck by Luna's Wrath!", ch, NULL, victim, TO_VICT);
		act("$N absorbs $n's hit and $n is struck by Luna's Wrath!", ch, NULL, victim, TO_NOTVICT);
		if (!IS_NPC(ch))
			act("$n absorbs your damage and you are struck by Luna's Wrath!", ch, NULL, victim, TO_CHAR);
		ch->hit -= 150;
		victim->hit += 100;
		if (victim->hit > victim->max_hit)
			victim->hit = victim->max_hit;
		REMOVE_BIT(victim->newbits, NEW_LWRATH);
	}
	tail_chain( );
	improveskill = 0;
    if (!is_safe(ch,victim))
    {
		while (improveskill < 5)
		{
			improve_wpn(ch,dt,right_hand);
			improve_stance(ch);
if (IS_GOLEM(ch)) improve_stance(ch);
			improveskill++;
		}
    }
}
int randomize_damage( CHAR_DATA *ch, int dam, int am )
{
    dam = dam * (am + 50) / 100;
    return dam;
}
int cap_dam(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    

    if (!IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] > 10)
		dam *= 2.0;
	
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW))
	{
	    if (IS_SET(victim->newbits, NEW_DROWHATE))
		    dam *= 1.6;
	}

    /* Vampire protean 7 (flesh of lava) reduces damage */
    if (IS_CLASS(victim, CLASS_VAMPIRE) )
   {
if (victim->power[DISC_VAMP_PROT] >=11)
{
dam *=40;
dam /=100;
}
	if (victim->power[DISC_VAMP_PROT] >= 8)
	{
  		dam *= 60;
		dam /= 100;
	}
	if (IS_EXTRA(victim, EXTRA_POTENCY) )
	    dam *= 1.25;
    }
    // I couldn't find where their resistance was, so I lowered it a bit 
    // in this way
    if (IS_CLASS(victim, CLASS_VAMPIRE)) dam *= 1.7;
    /* Vampire take extra damage during the day */
    if (!IS_NPC(victim) && (weather_info.sunlight == SUN_LIGHT) && IS_CLASS(victim, CLASS_VAMPIRE))
    {
                dam *= 1.2;
    }
    if (IS_CLASS(victim, CLASS_DEMON) && victim->power[DISC_DAEM_IMMU] >4)
    {
		dam *= 60;
		dam /= 100;
    }

    /* Vampire fortitude reduces damage */
    if (IS_CLASS(victim, CLASS_VAMPIRE) &&
		victim->power[DISC_VAMP_FORT] > 0)
    {

	if (victim->power[DISC_VAMP_FORT] == 10)
		dam *= 0.7;
else if (victim->power[DISC_VAMP_FORT] >=11)
                dam *= 1;
	else if (victim->power[DISC_VAMP_FORT] >= 7)
		dam *= 0.75;
	else if (victim->power[DISC_VAMP_FORT] >= 5)
		dam *= 0.8;
	else if (victim->power[DISC_VAMP_FORT] >= 3)
		dam *= 0.9;
    }

    if (victim->power[DISC_WERE_BOAR] > 4 && IS_CLASS(victim, CLASS_WEREWOLF))
    {
	if (IS_SET(victim->newbits, NEW_SKIN) )
	    dam *= 1.2;
	dam *= 80;
        dam /= 100;
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_DROWSHIELD))
    {
		dam *= 85;
		dam /= 100;
    }
    if (!IS_NPC(victim) && victim->pcdata->powers[PAMAZON] >=6 && IS_CLASS(victim, CLASS_AMAZON))
    {
                dam *= 80;
                dam /= 100;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HIGHLANDER))
    {
                dam *= 70;
                dam /= 100;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE))
    {
	if (!check_poly(victim,POLY_HUGE_SERPENT) && 
	!check_poly(victim,POLY_WYVERN))
	{
            dam *= 90;
            dam /= 100;
	}
	else	if (check_poly(victim,POLY_HUGE_SERPENT) ||
check_poly(victim,POLY_WYVERN))
	{
	    dam *= 70;
	    dam /= 100;
	}
    }
    if (!IS_NPC(victim) && victim->pcdata->powers[PMONK] > 3 && IS_CLASS(victim, CLASS_MONK))
    {
	dam *= 80;
        dam /= 100;
    }
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK))
	{
	    if (weather_info.sunlight == SUN_LIGHT)
	    {
		dam *= 150;
		dam /= 100;
	    }
	    else if (weather_info.sunlight == SUN_DARK)
	    {
		dam *= 70;
		dam /= 110;
	    }
	}
	if (!IS_NPC(victim) && victim->pcdata->powers[NPOWER_CHIKYU] > 1 && IS_CLASS(victim, CLASS_NINJA))
	{
	dam *= 65;
        dam /= 100;
	}

 if (!IS_NPC(victim) && victim->pcdata->powers[NPOWER_CHIKYU] > 1 && IS_CLASS(victim, CLASS_NINJA))
        {
        dam *= 60;
        dam /= 100;
        }

	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON))
	{
		dam *= (80 - (victim->generation * 2));
		dam /= 100;
	}
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_BERSERK))
{
dam *= (40 + (ch->bloodlust));
dam /= 100;                                                             
}       
 

 

	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_JEDI) && (victim->pcdata->powers[JPOWER_SPIRIT] > 3))
		dam *= 0.8;
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_JEDI) && (victim->generation < 3))
		dam *= 0.7;
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_JEDI) && (victim->generation == 4))
		dam *= 0.75;
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_JEDI) && (victim->generation == 6))
		dam *= 0.8;
    if (dam < 0)
		dam = 1;
                                                                               
    if (dam > 30000)
		dam = 30000;                                               

    return dam;
}
bool can_counter( CHAR_DATA *ch)
{
    if (!IS_NPC(ch) && IS_STANCE(ch, STANCE_MONKEY))
		return TRUE;
    return FALSE;
}
bool can_bypass( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if (IS_NPC(ch) || IS_NPC(victim))
		return FALSE;
    else if (IS_STANCE(ch, STANCE_VIPER))
		return TRUE;
    else if (IS_STANCE(ch, STANCE_MANTIS))
		return TRUE;
    else if (IS_STANCE(ch, STANCE_TIGER))
		return TRUE;
    else if (IS_STANCE(ch, STANCE_WOLF))
		return TRUE;
    return FALSE;
}
void update_damcap( CHAR_DATA *ch, CHAR_DATA *victim )
{
	int max_dam = 1000;
    if (!IS_NPC(ch))
    {
		if (IS_CLASS(ch, CLASS_DROW))
		{
			if (ch->generation < 2) max_dam = 3000;
		else if      (ch->generation < 3) max_dam = 2800;
		    else if (ch->generation < 4) max_dam = 2700;
		    else if (ch->generation < 5) max_dam = 2600;
			else                      max_dam = 2500;
			if (weather_info.sunlight == SUN_DARK)
				max_dam += 1050;
			if (IS_SET(ch->newbits, NEW_DROWHATE))
				max_dam += 1050;
			if (IS_SET(ch->special, SPC_DROW_WAR))
				max_dam += 250;
			else if (IS_SET(ch->special, SPC_DROW_MAG))
				max_dam += 200;
			else if (IS_SET(ch->special, SPC_DROW_CLE))
				max_dam += 150;
		}

                else if (IS_CLASS(ch, CLASS_AMAZON))
                {
                     if (ch->generation < 3) max_dam = 2700;
                else if (ch->generation < 4) max_dam = 2600;
                else if (ch->generation < 5) max_dam = 2500;
                     else                    max_dam = 2400;
                if (ch->max_hit > 10000)
                     max_dam += 700;
                if (ch->pcdata->powers[PAMAZON] >= 5)
                     max_dam += 600;
                if (ch->pcdata->powers[PAMAZON] >= 7)
                     max_dam += 400;
                }
                else if (IS_CLASS(ch, CLASS_HIGHLANDER))
		{
		     max_dam += ch->wpn[1] * 1.8;
		}
		else if (IS_CLASS(ch, CLASS_MONK))
		{    
                  if (ch->generation < 3) max_dam = 2400;
                else if (ch->generation < 4) max_dam = 2300;
                  else                    max_dam = 2200;
			if (ch->pcdata->powers[PMONK] > 0)
				max_dam += (ch->pcdata->powers[PMONK] * 70);
			if ( (weather_info.sunlight == SUN_LIGHT) || (weather_info.sunlight == SUN_RISE) )
				max_dam += 500;
                  if  (weather_info.sunlight == SUN_DARK)
                        max_dam -= 1150;
		}
		else if (IS_CLASS(ch, CLASS_JEDI))
		{
			if ((ch->generation == 4) && IS_SET(ch->special, SPC_PRINCE))
				max_dam = 2400;
			else if (ch->generation == 4)
				max_dam = 2300;
			else if (ch->generation < 3)
				max_dam = 2600;
			else
				max_dam = 2250;
			max_dam += ((ch->pcdata->powers[JPOWER_BODY] + ch->pcdata->powers[JPOWER_MIND] +
			ch->pcdata->powers[JPOWER_SPIRIT] + ch->pcdata->powers[JPOWER_TECHNIQUE]) * 50);
		}
		else if (IS_CLASS(ch, CLASS_NINJA))
		{
				max_dam += 750;
			if (ch->pcdata->powers[NPOWER_SORA] > 0)
				max_dam += (ch->pcdata->powers[NPOWER_SORA] * 175);
			if (ch->pcdata->powers[NPOWER_CHIKYU] > 0)
				max_dam += (ch->pcdata->powers[NPOWER_CHIKYU] * 150);
			if (ch->pcdata->powers[NPOWER_NINGENNO] > 0)
				max_dam += (ch->pcdata->powers[NPOWER_NINGENNO] * 150);
		}   else if (IS_CLASS(ch, CLASS_BERSERK))
{
max_dam += 750;                                                    
if (ch->bloodlust >=10)
	max_dam += 250;
if (ch->bloodlust >=20)
max_dam += 250; 
if (ch->bloodlust >=30)
max_dam += 250;  
if (ch->bloodlust >=40)
max_dam += 250;  
if (ch->bloodlust >=50)
max_dam += 250;  
if (ch->bloodlust >=60)
max_dam += 250;  
if (ch->bloodlust >=70)
max_dam += 250;  
if (ch->bloodlust >=80)
max_dam += 250; 
 if (ch->bloodlust >=90)
max_dam += 250;  
if (ch->bloodlust >=100)
max_dam += 250;          
}       
  else if (IS_CLASS(ch, CLASS_MECHA))
                {
                                max_dam += 750;
                        if (ch->pcdata->powers[MPOWER_PSCIONICS] > 1)
                                max_dam += (ch->pcdata->powers[MPOWER_PSCIONICS] * 175);
                        if (ch->pcdata->powers[MPOWER_COMBAT] > 4)
                                max_dam += (ch->pcdata->powers[NPOWER_CHIKYU] * 180);
                }  
		else if (IS_CLASS(ch, CLASS_MAGE))
		{
			if (ch->generation < 2)
				max_dam = 2300;
			else
				max_dam = 1800;
			if (check_poly(ch, POLY_HUGE_SERPENT))
				max_dam += 3000;
			if (check_poly(ch, POLY_WYVERN))
				max_dam += 3000;
		}
		else if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
			if      (ch->generation < 3) max_dam = 2700;
		    else if (ch->generation < 4) max_dam = 2600;
		    else if (ch->generation < 5) max_dam = 2500;
			else                      max_dam = 2400;
		    max_dam += (ch->rage * 1.2);
			if (ch->power[DISC_VAMP_PRES] > 4)
				max_dam += 500;
			if (ch->pcdata->rank == AGE_METHUSELAH) max_dam += 750;
		    else if (ch->pcdata->rank == AGE_ELDER) max_dam += 300;
		    else if (ch->pcdata->rank == AGE_ANCILLA) max_dam += 150;
                  if (ch->power[DISC_VAMP_PRES] > 0
                  && (weather_info.sunlight == SUN_DARK))
                        max_dam += 1500;
                  if (ch->power[DISC_VAMP_PRES] > 0
                  && (weather_info.sunlight == SUN_LIGHT))
                        max_dam -= 500;

		}	
		else if (IS_CLASS(ch, CLASS_WEREWOLF) || IS_SET(ch->special, SPC_WOLFMAN))
		{
			if      (ch->generation <= 2) max_dam = 2700;
		    else if (ch->generation <= 3) max_dam = 2600;
		    else if (ch->generation <= 4) max_dam = 2500;
			else                      max_dam = 2400;
			max_dam += ch->rage * 2;
		    if (ch->power[DISC_WERE_LUNA] > 6 && (weather_info.sunlight == SUN_DARK))
			    max_dam += 350;
			if (ch->power[DISC_WERE_BEAR] > 0)
				max_dam += (ch->power[DISC_WERE_BEAR]*1.2);
			if (ch->power[DISC_WERE_OWL] > 0)
				max_dam += (ch->power[DISC_WERE_OWL] *1.2);
			if (ch->power[DISC_WERE_HAWK] > 0)
				max_dam += (ch->power[DISC_WERE_HAWK] *1.1);
			if (ch->power[DISC_WERE_LUNA] > 0)
				max_dam += (ch->power[DISC_WERE_LUNA] * 1.1);
			if (ch->power[DISC_WERE_LYNX] > 0)
				max_dam += (ch->power[DISC_WERE_LYNX] * 1.2);
			if (ch->power[DISC_WERE_SPID] > 0)
				max_dam += (ch->power[DISC_WERE_SPID] * 1.2);
			if (ch->power[DISC_WERE_MANT] > 0)
				max_dam += (ch->power[DISC_WERE_MANT]  * 1.1);
			if (ch->power[DISC_WERE_PAIN] > 0)
				max_dam += (ch->power[DISC_WERE_PAIN] *1.2);
			if (ch->power[DISC_WERE_BOAR] > 0)
				max_dam += (ch->power[DISC_WERE_BOAR] * 1.1);
			if (ch->power[DISC_WERE_WOLF] > 0)
				max_dam += (ch->power[DISC_WERE_WOLF] * 1.3);
			if (ch->power[DISC_WERE_RAPT] > 0)
				max_dam += (ch->power[DISC_WERE_RAPT] * 1.1);
			if (ch->power[DISC_WERE_CONG] > 0)
				max_dam += (ch->power[DISC_WERE_CONG] *1.3);
		}        
		else if (IS_CLASS(ch, CLASS_DEMON))
		{
		   max_dam += 2200;
		   max_dam = (ch->generation * 190);
		   max_dam += ch->power[DISC_DAEM_ATTA] * 170;
		}

	 /* Status Damcap 50/status */
	    max_dam += (ch->race * 100);

	}
	if (IS_CLASS(ch, CLASS_SHAPE))
	{ if (will_use(ch, "Spirit")) max_dam += 400;
	  if ((get_weapon_type(ch) == UNARMED && will_use(ch, "Enhanced Unarmed")) || (get_weapon_type(ch) == SWORD && will_use(ch, "Enhanced SWORD")) || (get_weapon_type(ch) == DAGGER && will_use(ch, "Enhanced Dagger")) || (get_weapon_type(ch) == WHIP && will_use(ch, "Enhanced Whip")) || (get_weapon_type(ch) == MACE && will_use(ch, "Enhanced Mace")) || (get_weapon_type(ch) == FIREARM && will_use(ch, "Enhanced FireArm")))
	  max_dam += 300;
	  max_dam += 200;
	  if (will_use(ch, "Enhanced Damage")) max_dam += 350;
	  if (will_use(ch, "Unnatural Strength")) max_dam += 200;
	if (will_use(ch, "Unbelieveable Power")) max_dam += 300;
	}
    /* Max Dam Reductions */
    /* Werewolves */

    if (!IS_NPC(victim))
    {
		int silver_tol = (victim->siltol * 4);
		if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, ITEMA_RIGHT_SILVER))
			max_dam += (400 - silver_tol);
		if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, ITEMA_LEFT_SILVER))
			max_dam += (400 - silver_tol);
    	
        /* Vampies */
		if (!IS_NPC(ch) && (IS_CLASS(victim, CLASS_VAMPIRE)))
 			max_dam -= (victim->power[DISC_VAMP_FORT] * 5); 
        /*Demons */
		if (!IS_NPC(ch) && IS_CLASS(victim, CLASS_DEMON))
		    max_dam -= (victim->power[DISC_DAEM_IMMU] * 10);


    }

if (IS_CLASS(ch, CLASS_GOLEM))
{
if (IS_SET(ch->pcdata->powers[0], GOLEM_CRYSTAL))
    max_dam = 2200;
else if (IS_SET(ch->pcdata->powers[0], GOLEM_OBSIDIAN))
    max_dam = 6800 + (ch->race * 40);
}
    if (IS_ITEMAFF(ch, ITEMA_ARTIFACT))
		max_dam += 500;

	    if      (ch->stance[0] == STANCE_BULL)   max_dam += 250;
	    else if (ch->stance[0] == STANCE_DRAGON) max_dam += 250;
        else if (ch->stance[0] == STANCE_WOLF)   max_dam += 400;
	    else if (ch->stance[0] == STANCE_TIGER) max_dam += 250;

for (classcon = config_first; classcon; classcon = classcon->next)
 if (!IS_NPC(ch) && classcon->classbit == ch->class) max_dam *= classcon->damcap;

    if (max_dam < 1000)
		max_dam = 1000;
    if (max_dam > 7500)
	max_dam = 7500;
    if (ch->level >= LEVEL_BUILDER)
		max_dam = 30000;
    ch->damcap[DAM_CAP] = max_dam;
    ch->damcap[DAM_CHANGE] = 0;
    return;
}
int check_immune(CHAR_DATA *ch, int dam_type)
{
    int immune, def;
    int bit;
    CHAR_DATA *killing;

    immune = 2;
    def = IS_NORMAL;
    killing = ch->fighting;
    if (dam_type == DAM_NONE)
	return immune;

    /* Firstly, do the backbone for each race, and then do the powers,
	which are not at all race specific - so that ie, eladrin can
	learn protean and be immune to fire! */

    /* Eladrin aura deals with itself, as it is switched on and off
	and removes and sets the bits in both */

 if (killing && IS_CLASS(killing, CLASS_MAGE)) return IS_NORMAL;

    if (ch->class == CLASS_VAMPIRE)
    {
		/* They are resistant to cold */
		if (dam_type == DAM_COLD) immune = IS_RESISTANT;
 		/* They are vulnerable to drowning */
		if (dam_type == DAM_DROWNING) immune = IS_VULNERABLE;
		/* They are vulnerable to fire */
		if (dam_type == DAM_FIRE) immune = IS_VULNERABLE;
    }

    /* Firstly, we'll do flesh of lava! */
    if (ch->power[DISC_VAMP_PROT] >= 7 && dam_type == DAM_FIRE)
		immune = IS_RESISTANT;

    /* The corruption discipline */
    if (dam_type == DAM_POISON && ch->power[DISC_DAEM_CORR] >= 3)
		immune = IS_DIMMUNE;
    if (dam_type == DAM_POISON && ch->power[DISC_DAEM_CORR] >= 1)
		immune = IS_RESISTANT;

    /* The hellfire discipline */
    if (dam_type == DAM_FIRE && ch->power[DISC_DAEM_HELL] >= 2)
		immune = IS_DIMMUNE;
    if (dam_type == DAM_FIRE && ch->power[DISC_DAEM_HELL] >= 1)
		immune = IS_RESISTANT;

    /* The geluge discipline */
    if (dam_type == DAM_COLD && ch->power[DISC_DAEM_GELU] >= 4)
		immune = IS_DIMMUNE;
    if (dam_type == DAM_COLD && ch->power[DISC_DAEM_GELU] >= 1)
		immune = IS_RESISTANT;

    /* Fortitude level 1 for vampires */
    if (dam_type == DAM_POISON && ch->power[DISC_VAMP_FORT] >= 1)
		immune = IS_RESISTANT;
    if (dam_type == DAM_COLD && IS_CLASS(ch, CLASS_SHAPE)) immune =
IS_VULNERABLE;

    /* Returns immune if it has been changed at all ! */
	if (immune != 2)
		return immune;
    
    /* set bits to check -- VULN etc. must ALL be the same or this will fail */
    switch (dam_type)
    {
		case(DAM_BASH):         bit = RES_BASH;         break;
		case(DAM_PIERCE):       bit = RES_PIERCE;       break;
		case(DAM_SLASH):        bit = RES_SLASH;        break;
		case(DAM_FIRE):         bit = RES_FIRE;         break;
		case(DAM_COLD):         bit = RES_COLD;         break;
		case(DAM_LIGHTNING):    bit = RES_LIGHTNING;    break;
		case(DAM_ACID):         bit = RES_ACID;         break;
		case(DAM_POISON):       bit = RES_POISON;       break;
		case(DAM_NEGATIVE):     bit = RES_NEGATIVE;     break;
		case(DAM_HOLY):         bit = RES_HOLY;         break;
		case(DAM_ENERGY):       bit = RES_ENERGY;       break;
		case(DAM_MENTAL):       bit = RES_MENTAL;       break;
		case(DAM_DISEASE):      bit = RES_DISEASE;      break;
		case(DAM_DROWNING):     bit = RES_DROWNING;     break;
		case(DAM_LIGHT):        bit = RES_LIGHT;        break;
		case(DAM_CHARM):        bit = RES_CHARM;        break;
		case(DAM_SOUND):        bit = RES_SOUND;        break;
		default:                return def;
    }

    if (immune == 2)
	return def;
    else
	return immune;
}

bool damage_old( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show ) 
{

    OBJ_DATA *corpse;
    bool immune;

    if ( victim->position == POS_DEAD )
		return FALSE;
    
    /* damage reduction 
    if ( dam > 35)
		dam = (dam - 35)/2 + 35; 
    if ( dam > 80)
		dam = (dam - 80)/2 + 80; */
   
    if ( victim != ch )
    {
		/* Certain attacks are forbidden. Most other attacks are returned. */
		if ( is_safe( ch, victim ) )
			return FALSE;
		if ( victim->position > POS_STUNNED )
		{
	    /* If victim is charmed, ch might attack victim's master. */
			if ( IS_NPC(ch) && IS_NPC(victim) && IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
				&& victim->master->in_room == ch->in_room && number_bits( 3 ) == 0 )
			{
				stop_fighting( ch, FALSE );
				set_fighting( ch, victim->master);
				return FALSE;
			}
		}
		/* More charm stuff.*/
		if ( victim->master == ch )
			stop_follower( victim );
    }

    /* Inviso attacks ... not.  */
    if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
    {
		affect_strip( ch, gsn_invis );
		affect_strip( ch, gsn_mass_invis );
		REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
		act( "$n fades into existence.", ch, NULL, NULL, TO_ROOM );
    }

    /* Damage modifiers.*/

    if ( dam > 1 && !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK]  > 10 )
		dam = 9 * dam / 10;
 
    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) )
		dam /= 2;
 
    immune = FALSE;
    switch(check_immune(victim,dam_type))
    {
		case(IS_DIMMUNE):
		    dam -= dam/3;
			break;
		case(IS_RESISTANT):     
		    dam -= dam/3;
			break;
		case(IS_VULNERABLE):
		    dam += dam/2;
			break;
    }

    if (show)
		dam_message( ch, victim, dam, dt);

    if (dam == 0)
		return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;
    if ( !IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL && victim->hit < 1 )
		victim->hit = 1;
	update_pos( victim );

    switch( victim->position )
    {
		case POS_MORTAL:
			act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
			send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", victim );
			break;
	    case POS_INCAP:
			act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
			send_to_char("You are incapacitated and will slowly die, if not aided.\n\r",victim );
			break;
	    case POS_STUNNED:
			act( "$n is stunned, but will probably recover.",victim, NULL, NULL, TO_ROOM );
			send_to_char("You are stunned, but will probably recover.\n\r",victim );
			break;
		case POS_DEAD:
			act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
			send_to_char( "You have been KILLED!!\n\r\n\r", victim );
			stop_fighting(ch, TRUE);
/*                        if (IS_NPC(victim))
                        if (IS_SET(ch->act, PLR_AUTOSAC))
                        do_sacrifice(ch,"corpse");*/
			break;
		default:
			if ( dam > victim->max_hit / 4 )
				send_to_char( "That really did HURT!\n\r", victim );
			if ( victim->hit < victim->max_hit / 4 )
				send_to_char( "#1You sure are BLEEDING!\n\r", victim );
			break;
    }

    /* Sleep spells and extremely wounded folks. */
    if (victim->position == POS_DEAD && !IS_NPC(victim) && victim->class !=0 )
    {
		stop_fighting(victim, TRUE);
		return TRUE;
    }
    else
		update_pos(victim);

    if ( !IS_AWAKE(victim) )
		stop_fighting( victim, FALSE );

    /*  Payoff for killing things.*/
   
 if ( victim->position == POS_DEAD && victim->class==0 && victim->level < 3)
    {
		group_gain( ch, victim );
		if (IS_NPC(victim) && !IS_NPC(ch) )
		    ch->mkill += 1;	
		if ( !IS_NPC(victim) && victim->level < 3)
		{    
			if (IS_NPC(ch))
				victim->mdeath += 1;
			sprintf(log_buf, "%s killed by %s at %d", victim->name, (IS_NPC(ch) ? ch->short_descr : ch->name),victim->in_room->vnum );
			log_string( log_buf );
		}		    
		raw_kill(victim);

		if ( !IS_NPC(ch) && IS_NPC(victim) )
		{
			corpse = get_obj_list( ch, "corpse", ch->in_room->contents );
	 	    if ( IS_SET(ch->act, PLR_AUTOLOOT) && corpse && corpse->contains) /* exists and not empty */
				do_get( ch, "all corpse" );
      
			if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
				return TRUE;  /* leave if corpse has treasure */
			else
				do_sacrifice( ch, "corpse" );
		}
		return TRUE;
	}
	if ( victim == ch )
		return TRUE;

    /* Wimp out? */
    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
		if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
		&& victim->hit < victim->max_hit / 5) 
		|| ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
		&& victim->master->in_room != victim->in_room ) )
			do_flee( victim, "" );
    }

/*    if ( !IS_NPC(victim) &&  victim->hit > 0 && victim->hit <=
victim->wimpy && victim->wait < PULSE_VIOLENCE / 2 )
		do_flee( victim, "" );*/
    tail_chain( );
    return TRUE;
}

void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
    int sn;
    int insp = countinspires(ch) - countinspires(victim);
    CHAR_DATA *emb;
    int max_dam = ch->damcap[DAM_CAP];
if (is_safe(ch, victim)) return;
insp /= 2;
if (insp > 8) insp = 8;
if (insp < -8) insp = -8;
{float a = (float)insp * .7;
if (a < 0) a = 0;dam *= (a+1);}
if (IS_MOOGLE(ch)&& dt>= TYPE_HIT) dam *= 3.5;
if (IS_MOOGLE(victim)&& dt>= TYPE_HIT) dam /= 3.2;
    if ( victim->position == POS_DEAD )
		return;

	if (ch->embracing!=NULL )
    {
		if ((emb=ch->embracing)!=NULL) stop_embrace(ch,emb);
		else stop_embrace(ch,NULL);
    }
if (IS_NPC(ch) && !IS_NPC(victim))   // the older you get, the more
    dam += get_hours_played(victim); // vulnerable you are..

if (!IS_CLASS(ch, CLASS_SHAPE) && dt >= TYPE_HIT) dam *= 2;
else if (dt >= TYPE_HIT) dam *= 1.67;

if (IS_CLASS(ch, CLASS_GOLEM) && dt >= TYPE_HIT)
{
 if (IS_SET(ch->pcdata->powers[0], GOLEM_CRYSTAL))
    dam *= 1.5;
 if (IS_SET(ch->pcdata->powers[0], GOLEM_OBSIDIAN))
    dam *= 6.4;
}
if (IS_CLASS(victim, CLASS_GOLEM))// && dt >= TYPE_HIT)
{
 if (IS_SET(victim->pcdata->powers[0], GOLEM_CRYSTAL))
 {
    dam /= CRYSDEF;
    crys = TRUE;
 }
 else if (IS_SET(victim->pcdata->powers[0], GOLEM_OBSIDIAN) && dt >= TYPE_HIT)
    dam /= 4.4;
 else if (IS_INFORM(ch,GOLEM_OBSIDIAN))
    dam /= 2.4;
 else if (IS_SET(victim->pcdata->powers[0], GOLEM_EMERALD))
    dam *= 4.9; /* hah */
 else dam /= 4.9;
}
if (!IS_CLASS(victim,CLASS_GOLEM) || !IS_INFORM(victim,GOLEM_CRYSTAL))
    crys = FALSE;
for (classcon = config_first; classcon; classcon = classcon->next)
 if (dt >= TYPE_HIT &&
!IS_NPC(ch) && classcon->classbit == ch->class) dam *= classcon->damage;

for (classcon = config_first; classcon; classcon = classcon->next)
 if (dt >= TYPE_HIT &&
!IS_NPC(victim) && classcon->classbit == victim->class) dam /= classcon->toughness;

    /* Stop up any residual loopholes. */
//    if (ch->damcap[DAM_CHANGE] == 1)
		update_damcap(ch,victim);
    if ( dam > ch->damcap[DAM_CAP]/* max_dam*/ && dt != gsn_garotte && !nodamcap)
		dam = max_dam;
    if ( victim != ch )
    {
		/* Certain attacks are forbidden.Most other attacks are returned.*/
		if ( is_safe( ch, victim ) )
			return;
		check_killer( ch, victim );
		if ( victim->position > POS_STUNNED )
		{
		    if ( victim->fighting == NULL )
				set_fighting( victim, ch );
		    victim->position = POS_FIGHTING;
		}
		if ( victim->position > POS_STUNNED )
		{
			if ( ch->fighting == NULL )
				set_fighting( ch, victim );
		    /*If victim is charmed, ch might attack victim's master.  */
		    if ( IS_NPC(ch)	&& IS_NPC(victim) && IS_AFFECTED(victim, AFF_CHARM) &&  victim->master != NULL
			&&   victim->master->in_room == ch->in_room && number_bits( 3 ) == 0 )
		    {
				stop_fighting( ch, FALSE );
				multi_hit( ch, victim->master, TYPE_UNDEFINED );
				return;
		    }
		}
		/* More charm stuff.*/
		if ( victim->master == ch )
			stop_follower( victim );
         	/* Damage modifiers.*/
		if ( IS_AFFECTED(ch, AFF_HIDE) )
		{
		    if (!can_see(victim,ch))
			{
				dam *= 1.5;
				send_to_char("You use your concealment to get a surprise attack!\n\r",ch);
			}
		    REMOVE_BIT( ch->affected_by, AFF_HIDE );
			act( "$n leaps from $s concealment.", ch, NULL, NULL, TO_ROOM );
		}
		sn = shape_lookup("Enhanced Stealth");
 		if ( IS_SET(ch->act, PLR_SACREDINVIS))
		{ if ((able_to_use(ch, sn) || will_use(ch, "Enhanced
Stealth")) && IS_NPC(victim)) {} else {
			REMOVE_BIT(ch->act, PLR_SACREDINVIS);
			send_to_char( "You emerge from your cloak of invisibility.\n\r", ch );
		    act("$n slowly emerges from a cloak of holy invisibility.",ch,NULL,NULL,TO_ROOM);
		}}
		if (will_use(ch, "Enhanced Stealth") || will_use(ch,
"Enhanced Stealth") || will_use(ch, "Enhanced Stealth") ){}
		if ( IS_AFFECTED(victim, AFF_SANCTUARY) && dam > 1  )
		    dam *= 0.5;
		if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
		    dam -= dam * 0.25;

		if ( dam < 0 )
		    dam = 0;

		/* Check for disarm, trip, parry, and dodge. */
		if ( dt >= TYPE_HIT )
		{
			if ( IS_NPC(ch) && number_percent( ) < ch->level * 0.5 )
				disarm( ch, victim );

		    if ( IS_NPC(ch) && number_percent( ) < ch->level * 0.5 )
				trip( ch, victim );

			if ( check_parry( ch, victim, dt ) )
				return;
		    if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && 
			victim->stance[STANCE_CRANE] > 100 && !can_counter(ch)
			&& !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
				return;
		    else if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) && 
			victim->stance[STANCE_MANTIS] > 100 && !can_counter(ch)
			&& !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
				return;
		    if ( check_dodge( ch, victim, dt ) )
				return;
			if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && 
			victim->stance[STANCE_MONGOOSE] > 100 && !can_counter(ch)
			&& !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
				return;
		    else if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && 
			victim->stance[STANCE_SWALLOW] > 100 && !can_counter(ch)
			&& !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
				return;
		    else if (IS_GOLEM(victim) && !IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && 
			victim->stance[STANCE_SWALLOW] > 100 && !can_counter(ch)
			&& !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
				return;
		    else if (IS_GOLEM(victim) && !IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && 
			victim->stance[STANCE_SWALLOW] > 100 && !can_counter(ch)
			&& !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
				return;
		}


		if (!IS_NPC(victim) && dam > 2)
		{
		int chance = number_range(0,100);

		if (IS_CLASS(victim, CLASS_WEREWOLF) &&
			victim->power[DISC_WERE_LUNA] >= 9 && chance >=90)
		{
if (IS_CLASS(ch, CLASS_GOLEM) && ch->pcdata->powers[0] > 0)
{
stc("#3You try to absorb some STONE and it lashes against you!\n\r",victim);
act("#3Your opponent tries to absorb you, STONE, and you lash against $S.",
    ch,NULL,victim,TO_CHAR);
dam *= 1.4;
}
else {		    stc("#3You absorb some of you opponents damage.\r\n",victim);
		    stc("#3Your opponents absorbs some of your damage.\r\n",ch);
		    dam *= .80;}
		}
		else if (IS_CLASS(victim, CLASS_MONK) &&
			victim->pcdata->powers[PMONK] >= 12 &&
			chance >= 80 &&
			((weather_info.sunlight == SUN_LIGHT ||
			weather_info.sunlight == SUN_RISE) ) )
		{
if (IS_CLASS(ch, CLASS_GOLEM) && ch->pcdata->powers[0] > 0)
{
stc("#3You try to absorb some STONE and it lashes against you!\n\r",victim);
act("#3Your opponent tries to absorb you, STONE, and you lash against $S.",
    ch,NULL,victim,TO_CHAR);
dam *= 1.4;
}
else {
		    stc("#3You feel God's power encompass you, absorbing some damage.\r\n", victim);
		    act("#3Your oppnent lets God's power encompasses $S.",
				ch, NULL, victim, TO_CHAR );
		    dam *= .80;
}		}

		}
if (IS_GOLEM(ch) && IS_INFORM(ch,GOLEM_CRYSTAL))
    dam = number_range(0,15); // hehehehehehehehehehehehehehehehehehehehehehe
		dam_message( ch, victim, dam, dt );
    }
    hurt_person(ch,victim,dam);
    return;
}
void adv_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
    if ( victim->position == POS_DEAD )
		return;
    if ( dam > 1000 ) dam = 1000;
    if ( victim != ch )
    {
		if ( is_safe( ch, victim ) )
		    return;
		check_killer( ch, victim );
		if ( victim->position > POS_STUNNED )
		{
			if ( victim->fighting == NULL )
			set_fighting( victim, ch );
		    victim->position = POS_FIGHTING;
		}	
		if ( victim->position > POS_STUNNED )
		{
			if ( ch->fighting == NULL )
			set_fighting( ch, victim );
		    /* If victim is charmed, ch might attack victim's master.  */
		    if ( IS_NPC(ch)
		    &&   IS_NPC(victim)
			&&   IS_AFFECTED(victim, AFF_CHARM)
		    &&   victim->master != NULL
		    &&   victim->master->in_room == ch->in_room
			&&   number_bits( 3 ) == 0 )
		    {
				stop_fighting( ch, FALSE );
				multi_hit( ch, victim->master, TYPE_UNDEFINED );
				return;
			}
		}

		if ( victim->master == ch )
		    stop_follower( victim );
		if ( IS_AFFECTED(victim, AFF_SANCTUARY) && dam > 1 )
		    dam /= 2;
		if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
		    dam -= dam / 4;
		if (!IS_NPC(victim) && !IS_NPC(ch)) dam = dam *2/3;
		if ( dam < 0 )
		    dam = 0;
    }
    return;
}/* End of void adv_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam )*/

void hurt_person( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
	char buf[MAX_STRING_LENGTH];
    int vnum;
int blah[20];

for (vnum = 0; vnum < 20; vnum++)
    blah[vnum] = IS_NPC(victim) ? 0 : victim->pcdata->powers[vnum];

if (!crys && IS_CLASS(victim, CLASS_GOLEM) && IS_INFORM(victim,GOLEM_CRYSTAL))
    dam /= CRYSDEF;
else
    crys = TRUE;

if (IS_UNI(victim) && IS_NPC(ch)) 
dam /= 17;
else if (IS_UNI(victim)) 
dam /= 6.2;
else if (IS_UNI(ch) && IS_NPC(victim)) 
dam *= 3.2;
else if (IS_UNI(ch)) 
dam *= 1.5;

    /* Hurt the victim. Inform the victim of his new state.  */
    victim->hit -= dam;
/*
if (IS_INFORM(victim, GOLEM_EMERALD) && victim->hit > 0 && NR(1,3)==1)
{
NO_STONE(victim);
    smooregen(victim, NR(6000, 10000), NR(1000,6000),NR(1000,9000));
    act("#2The emerald around $n liquifies and sinks into the ground.",
	victim,0x0,0x0,TO_ROOM);
    act("#2The liquid emerald touches $n's mouth and $e springs back up.#n",
	victim,0x0,0x0,TO_ROOM);
    act("#2The emerald around you liquifies and sinks into the ground.",
	victim,0x0,0x0,TO_CHAR);
    act("#2You feel the liquid emerald touch your mouth and you spring back to life!#n",
	victim,0x0,0x0,TO_CHAR);
    act("#6You feel much better!#n",victim,0x0,0x0,TO_CHAR);
victim->pcdata->powers[1] = 1;
}
else
*/
if (victim->hit <= 0 && IS_INFORM(victim,GOLEM_EMERALD))
{
NO_STONE(victim);
    victim->hit = number_range(victim->max_hit / 6, victim->max_hit / 3);
    update_pos(victim);
    act("#2The emerald around $n liquifies and sinks into the ground.",
	victim,0x0,0x0,TO_ROOM);
    act("#2The liquid emerald touches $n's mouth and $e springs back up.#n",
	victim,0x0,0x0,TO_ROOM);
    act("#2The emerald around you liquifies and sinks into the ground.",
	victim,0x0,0x0,TO_CHAR);
    act("#2You feel the liquid emerald touch your mouth and you spring back to life!#n",
	victim,0x0,0x0,TO_CHAR);
    act("#6You feel much better!#n",victim,0x0,0x0,TO_CHAR);
victim->pcdata->powers[1] = 1;
}
if (IS_CLASS(victim, CLASS_GOLEM) && victim->hit <= 0)
ch->wait = 0;
    if ( !IS_NPC(victim)  && victim->level >= LEVEL_IMMORTAL && victim->hit < 1 )
		victim->hit = 1;
    if ( !IS_NPC(victim)  && IS_SET(victim->newbits, NEW_CLOAK) && victim->hit < 1 )
    {
		victim->hit += (victim->max_hit * .25);
		REMOVE_BIT(victim->newbits, NEW_CLOAK);
    }
    update_pos( victim );
    switch( victim->position )
    {
	    case POS_MORTAL:
			act( "$n is mortally wounded, and spraying blood everywhere.", victim, NULL, NULL, TO_ROOM );
			send_to_char( "You are mortally wounded, and spraying blood everywhere.\n\r", victim );
if (IS_GOLEM(victim)) NO_STONE(victim);
if (IS_UNI(victim)) {victim->pcdata->powers[UNI_SONG] = 0;
victim->pcdata->powers[UNI_WAIT] = 0;}
			/* Arena Code Retry */
			if ((IS_SET(victim->act, PLR_CHALLENGER) && IS_SET(ch->act, PLR_CHALLENGED))
				|| (IS_SET(victim->act, PLR_CHALLENGED) && IS_SET(ch->act, PLR_CHALLENGER)))
			{
				if (ch->challenged != NULL)
					victim = ch->challenged;
				else
					victim = ch->challenger;
				char_from_room(ch);
				char_to_room(ch,get_room_index(69));
				char_from_room(victim);
				char_to_room(victim,get_room_index(70));
				ch->hit = ch->max_hit;
				ch->mana = ch->max_mana;
				ch->move = ch->max_move;
				ch->loc_hp[0] = 0;
			    ch->loc_hp[1] = 0;
				ch->loc_hp[2] = 0;
			    ch->loc_hp[3] = 0;
				ch->loc_hp[4] = 0;
				ch->loc_hp[5] = 0;
				ch->loc_hp[6] = 0;
				if (IS_AFFECTED(ch, AFF_FLAMING))
				    REMOVE_BIT(ch->affected_by, AFF_FLAMING);
				update_pos(ch);
				save_char_obj(ch);
				do_look(ch, "auto");
				victim->hit = victim->max_hit;
				victim->mana = victim->max_mana;
				victim->move = victim->max_move;
				victim->loc_hp[0] = 0;
			    victim->loc_hp[1] = 0;
				victim->loc_hp[2] = 0;
			    victim->loc_hp[3] = 0;
				victim->loc_hp[4] = 0;
				victim->loc_hp[5] = 0;
				victim->loc_hp[6] = 0;
				if (IS_AFFECTED(victim, AFF_FLAMING))
				    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
				update_pos(victim);
				save_char_obj(victim);
				do_look(victim, "auto");
				ch->challenger = NULL;
				ch->challenged = NULL;
				victim->challenged = NULL;
				victim->challenger = NULL;
				if IS_SET(victim->act, PLR_CHALLENGED)
				{
					REMOVE_BIT(victim->act,PLR_CHALLENGED);
					REMOVE_BIT(ch->act,PLR_CHALLENGER);
				}
				else
				{
					REMOVE_BIT(ch->act,PLR_CHALLENGED);
					REMOVE_BIT(victim->act,PLR_CHALLENGER);
				}
				send_to_char("You were beaten.\n\r",victim);
				send_to_char("You were victorious.\n\r",ch);
				ch->fight_timer = 0;
				victim->fight_timer = 0;
				victim->pcdata->alosses += 1;
				ch->pcdata->awins +=1;
				arena = FIGHT_OPEN;			
		    	sprintf(buf,"[ARENA] %s has been beaten by %s.",victim->pcdata->switchname,ch->pcdata->switchname);
		    	do_info(ch,buf);
				/*for ( d = descriptor_list; d; d = d->next )
				{
					if( d->connected == CON_PLAYING)
					{
						if (d->character->gladiator != NULL)
						{
							if (!str_cmp(d->character->gladiator, ch->pcdata->switchname))
							{
								send_to_char("You have won your arena wager.\n\r", d->character);
								d->character->pcdata->quest += (d->character->pcdata->plr_wager * 2);
							}
							else
								send_to_char("You have lost your arena wager.\n\r", d->character);
							d->character->gladiator = NULL;
						}
					}
				}*/
				arena = FIGHT_OPEN;			
			}
			if (will_use(ch, "AutoDecapitate") || will_use(ch,
"AutoDecapitate") || will_use(ch, "AutoDecapitate") || can_use(ch,
"AutoDecapitate")) {if
(ch->shape[autodecap] == TRUE) {do_decapitate(ch,
victim->name);do_sacrifice(ch, "head");}}
			break;
	    case POS_INCAP:
			act( "$n is incapacitated, and bleeding badly.",
			victim, NULL, NULL, TO_ROOM );
			send_to_char("You are incapacitated, and bleeding badly.\n\r", victim );
			break;
	    case POS_STUNNED:
			act( "$n is stunned, but will soon recover.",
			victim, NULL, NULL, TO_ROOM );
			send_to_char("You are stunned, but will soon recover.\n\r",victim );
			break;
	    case POS_DEAD:
			act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
			send_to_char( "You have been KILLED!!\n\r\n\r", victim );
/*			if (IS_NPC(victim))
			if (IS_SET(ch->act, PLR_AUTOSAC))
			do_sacrifice(ch,"corpse");*/
			break;
	    default:
			if ( dam > victim->max_hit / 4 )
				send_to_char( "That really did HURT!\n\r", victim );
			if ( victim->hit < victim->max_hit / 4 && dam > 0 )
			{
				if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE)
				&& number_percent() < victim->beast)
					vamp_rage(victim);
				else
					send_to_char( "You sure are BLEEDING!\n\r", victim );
			}
			break;
    }

    /* Sleep spells and extremely wounded folks.  */
    if ( !IS_AWAKE(victim) )
		stop_fighting( victim, FALSE );
    /*
     * Payoff for killing things.
     */
if (victim && !IS_NPC(victim) && !IS_CLASS(victim,CLASS_GOLEM))
{int a;
for (a = 0; a < 20; a++)
    victim->pcdata->powers[a] = blah[a];
}

    if ( victim->position == POS_DEAD )
    {
		group_gain( ch, victim );

		if ( !IS_NPC(victim) )
		{
			sprintf( log_buf, "%s killed by %s at %d",
			victim->name,
			(IS_NPC(ch) ? ch->short_descr : ch->name),
			victim->in_room->vnum );
			log_string( log_buf );
	        /* Dying penalty: 1/2 your current exp.  */
			if ( victim->exp > 0 )
				victim->exp = victim->exp / 2;
		}
    	if (IS_NPC(victim) && !IS_NPC(ch))
		{
		    ch->mkill += 1;
		    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION) || IS_CLASS(ch, CLASS_DROW))
			{
				if ((vnum = victim->pIndexData->vnum) > 29000)
				{
					switch (vnum)
					{
				    default:
						if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION))
						{
							ch->pcdata->stats[DEMON_CURRENT] +=victim->level;
							ch->pcdata->stats[DEMON_TOTAL]   +=victim->level;
						}
						else
							ch->pcdata->stats[DROW_POWER] += victim->level;
					break;
				    case 29600:
					case 30001:
					case 30006:
				    case 30007:
				    case 30008:
				    case 30009:
					case 30000:
					break;
				    }
				}
				else if (IS_NPC(victim) && !IS_SET(victim->act, ACT_NOEXP))
				{
					if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION))
					{
						ch->pcdata->stats[DEMON_CURRENT] += victim->level/2;
						ch->pcdata->stats[DEMON_TOTAL] += victim->level/2;
					}
					else
						ch->pcdata->stats[DROW_POWER] += victim->level/2;
				}
			}
		    if (ch->level == 1 && ch->mkill > 4)
		    {
			send_to_char("You are now NOTE TRUSTED!\r\n", ch);
			SET_BIT(ch->extra, EXTRA_NOTE_TRUST);
				ch->level = 2;
				save_char_obj(ch);
			}
		}
		if (!IS_NPC(victim) && IS_NPC(ch)) 
	    {
			victim->mdeath = victim->mdeath + 1;
	    }
		raw_kill( victim );
		if ( IS_SET(ch->act, PLR_AUTOLOOT) )
		    do_get( ch, "all corpse" );
		else
		    do_look( ch, "in corpse" );

		if ( !IS_NPC(ch) && IS_NPC(victim) )
		{
			if ( IS_SET(ch->act, PLR_AUTOSAC) )
				do_sacrifice( ch, "corpse" );
		}
		return;
    }

    if ( victim == ch )
		return;

    /* Take care of link dead people. */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
		if ( number_range( 0, victim->wait ) == 0 )
		{
			do_recall( victim, "" );
			return;
		}
    }

    /* Wimp out? */
    if ( IS_NPC(victim) && dam > 0 )
    {
		if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 1 ) == 0 && victim->hit < victim->max_hit / 2 )
		|| ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL && victim->master->in_room != victim->in_room ) )
			do_flee( victim, "" );
    }
    if ( !IS_NPC(victim) && victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait == 0 )
		do_flee( victim, "" );
    tail_chain( );
    return;
}


bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
if (world_peace) return TRUE;
if (IS_PTEST_SAFE(ch) && !IS_NPC(victim)) 
{stc("But your ptesting!\n\r", ch);return TRUE;}
else if (IS_PTEST_SAFE(victim) && !IS_NPC(ch)) 
{stc("But your ptesting!\n\r",ch);return TRUE;}

if (IS_ITEMAFF(ch, ITEMA_PEACE) && !IS_NPC(victim)) return TRUE;
if (IS_ITEMAFF(victim,ITEMA_PEACE) && !IS_NPC(ch)) return TRUE;
	if (!IS_NPC(victim) && !IS_NPC(ch))
	{
   if(!IS_NPC(victim))
{
   if (get_hours_played(victim) < 3 && victim->race == 0)
   {
	send_to_char("No newbie killing!\n\r",ch);
	return TRUE;
   }
}
if (!IS_NPC(ch) && !IS_NPC(victim) && get_hours_played(ch) < 3 && ch->race
== 0)
{
    stc("Your a newbie, peace!\n\r",ch);
    return TRUE;
}
		if ( ch->level < 3 || victim->level < 3 )
		{
			send_to_char( "Both players must be avatars to fight.\n\r", ch );
	        	return TRUE;
		}
		else if (victim->level > 7)
		{
			send_to_char( "You are not permitted to attack an immortal.\n\r", ch);
			return TRUE;
		}
	}
/*
	if (victim->fight_timer > 0)
		return FALSE;*/
/*
	if (IS_AFFECTED(ch, AFF_PEACE) && ch->fighting != NULL)
		REMOVE_BIT(ch->affected_by, AFF_PEACE);
	if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL)
		REMOVE_BIT(victim->affected_by, AFF_PEACE);
*/
    /* Ethereal people can only attack other ethereal people */
    if ( IS_AFFECTED(ch, AFF_ETHEREAL) && !IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
		send_to_char( "You cannot while ethereal.\n\r", ch );
		return TRUE;
    }
    if ( !IS_AFFECTED(ch, AFF_ETHEREAL) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
		send_to_char( "You cannot fight an ethereal person.\n\r", ch );
		return TRUE;
    }

    /* You cannot attack across planes */
    if ( IS_SET(ch->affected_by, AFF_SHADOWPLANE) &&
		!IS_SET(victim->affected_by, AFF_SHADOWPLANE))
	{
		act( "You are too insubstantial!", ch, NULL, victim, TO_CHAR );
		if (ch->position == POS_FIGHTING)
			stop_fighting(ch,TRUE);
		return TRUE;
    }
    if (!IS_SET(ch->affected_by, AFF_SHADOWPLANE) &&
	IS_SET(victim->affected_by, AFF_SHADOWPLANE))
    {
		act( "$E is too insubstantial!", ch, NULL, victim, TO_CHAR );
		if (ch->position == POS_FIGHTING)
			stop_fighting(ch,TRUE);
		return TRUE;
    }

        if (victim->fight_timer > 0)
                return FALSE;

/*
    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE)) 
    {
		send_to_char( "You cannot fight in a safe room.\n\r", ch );
		return TRUE;
    }
 */   /* ARENA room checking */
    if (  IS_SET(ch->in_room->room_flags,ROOM_ARENA) && IS_SET(victim->in_room->room_flags,ROOM_ARENA) )
        return FALSE;

    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
    {
		send_to_char( "Objects cannot fight!\n\r", ch );
		return TRUE;
    }
    else if ( IS_HEAD(victim,LOST_HEAD) || IS_EXTRA(victim,EXTRA_OSWITCH))
    {
		send_to_char( "You cannot attack an object.\n\r", ch );
		return TRUE;
    }

    if ( IS_NPC(ch) || IS_NPC(victim) )
		return FALSE;

    /* Thx Josh! */
    if ( victim->fighting == ch )
		return FALSE;

    if (!IS_NPC(victim) && !IS_NPC(ch) && IS_AFFECTED(ch, AFF_PEACE))
    {
		send_to_char( "You are unable to attack them.\n\r", ch );
        return TRUE;
    }

    if (!IS_NPC(ch) && !IS_NPC(victim) && IS_AFFECTED(victim, AFF_PEACE))
    {
		send_to_char( "You can't seem to attack them.\n\r", ch );
        return TRUE;
    }

    if (!IS_NPC(ch) && !IS_NPC(victim) && IS_ITEMAFF(ch, ITEMA_PEACE))
    {
		send_to_char( "You are unable to attack them.\n\r", ch );
		return TRUE;
    }

    if (!IS_NPC(ch) && !IS_NPC(victim) && IS_ITEMAFF(victim, ITEMA_PEACE))
    {
		send_to_char( "You can't seem to attack them.\n\r", ch );
		return TRUE;
    }
	return FALSE;
}
bool no_attack( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if (!IS_NPC(ch) && !IS_NPC(victim))
	{
    /*if ( !CAN_PK(ch) || !CAN_PK(victim) )*/
		if (ch->level < 3 || victim->level < 3)
		{
			send_to_char( "Both players must be avatars to fight.\n\r", ch );
	        return TRUE;
		}
		else if (victim->level > 7)
		{
			send_to_char ("You are not allowed to attack immortals.\n\r", ch);
			return TRUE;
		}
	}
    if (victim->fight_timer > 0)
   		return FALSE;

    /* Ethereal people can only attack other ethereal people */
    if ( IS_AFFECTED(ch, AFF_ETHEREAL) && !IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
		send_to_char( "You cannot while ethereal.\n\r", ch );
		return TRUE;
    }
    if ( !IS_AFFECTED(ch, AFF_ETHEREAL) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
		send_to_char( "You cannot fight an ethereal person.\n\r", ch );
		return TRUE;
    }

    /* You cannot attack across planes */
    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE) )
    {
		act( "You are too insubstantial!", ch, NULL, victim, TO_CHAR );
		if (ch->position == POS_FIGHTING)
			stop_fighting(ch,TRUE);

		return TRUE;
    }
    if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE) )
    {
		act( "$E is too insubstantial!", ch, NULL, victim, TO_CHAR );
		if (ch->position == POS_FIGHTING)
			stop_fighting(ch,TRUE);
		return TRUE;
    }

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) && ch->max_hit <=20000)
    {
		send_to_char( "You cannot fight in a safe room.\n\r", ch );
		return TRUE;
    }
    if (IS_SET(victim->in_room->room_flags,ROOM_SAFE) && victim->max_hit <=20000)
    {
                send_to_char( "You cannot fight in a safe room.\n\r", ch );
                return TRUE;
    }

    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
    {
		send_to_char( "Objects cannot fight!\n\r", ch );
		return TRUE;
    }
    else if ( IS_HEAD(victim,LOST_HEAD) || IS_EXTRA(victim,EXTRA_OSWITCH) )
    {
		send_to_char( "You cannot attack objects.\n\r", ch );
		return TRUE;
    }
    return FALSE;
}
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
    /*Follow charm thread to responsible character.Attacking someone's charmed char is hostile! */
    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
		victim = victim->master;

    /* NPC's are fair game.So are killers and thieves. */
    if ( IS_NPC(victim) )
		return;

    /* Charm-o-rama. */
    if ( IS_SET(ch->affected_by, AFF_CHARM) )
    {
		if ( ch->master == NULL )
		{
			affect_strip( ch, gsn_charm_person );
		    REMOVE_BIT( ch->affected_by, AFF_CHARM );
		    return;
		}
		stop_follower( ch );
		return;
    }
    return;
}/* End of void check_killer( CHAR_DATA *ch, CHAR_DATA *victim ) */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *obj = NULL;
    int chance = 0;
    bool claws = FALSE;
    int insp = IS_UNI(ch) ? countinspires(ch) : 0;
if (insp > 12) insp = 12;
    if ( !IS_AWAKE(victim) )
		return FALSE;

    if ( IS_NPC(victim) )
		obj = NULL;
    else if ( IS_CLASS(victim, CLASS_WEREWOLF)	&& 	victim->power[DISC_WERE_BEAR] > 2	&& 
	IS_VAMPAFF(victim, VAM_CLAWS) && get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL)
	{
		obj = NULL;
		claws = TRUE;
    }
    else if ( IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits, NEW_MONKADAM) &&
	get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL)
    {
		obj = NULL;
		claws = TRUE;
    }

    else
    {
		if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON )
		{
			if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL || obj->item_type != ITEM_WEAPON )
				return FALSE;
		}
    }
    if (dt < 1000 || dt > 1012)
		return FALSE;
    if (!IS_NPC(ch))
		chance -= (ch->wpn[dt-1000] * 0.1);
	else
		chance -= (ch->level * 0.5);
    if (!IS_NPC(victim))
		chance += (victim->wpn[dt-1000] * 0.5);
	else
		chance += victim->level;

    if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && victim->stance[STANCE_CRANE] > 0
    && !can_counter(ch) && !can_bypass(ch,victim))
		chance += (victim->stance[STANCE_CRANE] * 0.25);

    else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) &&	victim->stance[STANCE_MANTIS] > 0
	&& !can_counter(ch) && !can_bypass(ch,victim))
		chance += (victim->stance[STANCE_MANTIS] *
IS_GOLEM(victim) ? 0.50 : 0.25);

    chance -= (char_hitroll(ch) * 0.1);


    if (claws) 
    {
            if (IS_CLASS(victim, CLASS_WEREWOLF) )
            {
	    if (victim->power[DISC_WERE_LYNX] > 3)
	       chance += (char_hitroll(victim) * 0.1);
	    else
	       chance += (char_hitroll(victim) * 0.075);
            }
    }
    else	chance += (char_hitroll(victim) * 0.1);
if (IS_MOOGLE(ch)) chance -= 65;
if (IS_MOOGLE(victim)) chance += 40;
for (classcon = config_first; classcon; classcon = classcon->next)
 if (!IS_NPC(ch) && classcon->classbit == ch->class) chance -= classcon->parryoff; 

for (classcon = config_first; classcon; classcon = classcon->next)
 if (!IS_NPC(victim) && classcon->classbit == victim->class) chance += classcon->parrydef;

if (IS_CLASS(ch,CLASS_SHAPE))
	chance -= 23;
chance -= (insp * 7);

if (IS_CLASS(victim, CLASS_GOLEM))
{
 if (victim->pcdata->powers[0] == 0)
	chance += 20;
 if (victim->pcdata->powers[0] & GOLEM_CRYSTAL)
	chance += 40;
 if (IS_INFORM(victim,GOLEM_OBSIDIAN))
	chance += 27;

}
    if (!IS_NPC(ch))
    {
if (IS_CLASS(ch, CLASS_GOLEM))
{
    if (ch->pcdata->powers[0] & GOLEM_CRYSTAL)
	chance -= 23;
    else if (ch->pcdata->powers[0] & GOLEM_OBSIDIAN)
	chance -= number_range(34,40);
    if (IS_INFORM(ch,GOLEM_OBSIDIAN))
	chance -= 43;
}
		if (IS_CLASS(ch, CLASS_VAMPIRE))
        {
                  if (ch->power[DISC_VAMP_CELE] > 0
                  && (weather_info.sunlight == SUN_LIGHT))
                        chance += 15;
                  if (ch->rage > 50)
                        chance += 15;
		  chance -= ch->power[DISC_VAMP_CELE] ;
        }
		if (ch->power[DISC_DAEM_ATTA] > 0 && IS_CLASS(ch, CLASS_DEMON))
			chance -= (ch->power[DISC_DAEM_ATTA] + 2);

		if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS))
			chance -= 25;
		else if (IS_CLASS(ch, CLASS_DROW))
			chance -= 20;

		if (IS_SET(ch->newbits, NEW_MONKFLAME) && IS_CLASS(ch, CLASS_MONK))
		    chance -= 28;
		if (IS_CLASS(ch, CLASS_BERSERK))
			chance -= (ch->bloodlust * 2);
		if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >= 4)
			chance -= 25;/*
if (IS_CLASS(ch, CLASS_MECHA) && ch->pcdata->powers[MPOWER_COMBAT] >= 4)
                        chance -= 25;  */       
	    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 0 )
			chance -= (ch->power[DISC_WERE_MANT] * 2.5);
            if (IS_CLASS(ch, CLASS_MAGE))
		{
                        chance -= 20;
			if (check_poly(ch, POLY_HUGE_SERPENT) ||
check_poly(ch, POLY_WYVERN))
			chance -= 5;
		}
/* 1.5k wpn gives ALOT of bonus    */
            if (IS_CLASS(ch, CLASS_HIGHLANDER))
                        chance -= 15;

            if (IS_CLASS(ch, CLASS_JEDI) && ch->pcdata->powers[JPOWER_BODY] >=4)
                        chance -= 25;
    }
    if (!IS_NPC(victim))
    {
		if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS))
			chance += 25;
		else if (IS_CLASS(victim, CLASS_DROW))
			chance += 20;

		if (IS_CLASS(victim, CLASS_NINJA))
			chance += 20;
                if (IS_CLASS(victim, CLASS_BERSERK))
			chance +=20;
if (IS_CLASS(victim, CLASS_MECHA))
                        chance += 20;      
		if (IS_SET(victim->newbits, NEW_MONKFLAME) && IS_CLASS(victim, CLASS_MONK))
			chance += 28;
		if (victim->power[DISC_DAEM_ATTA] > 0)
			chance += (victim->power[DISC_DAEM_ATTA]+2);
	    if (IS_CLASS(victim, CLASS_WEREWOLF) &&
			victim->power[DISC_WERE_MANT] > 0 )
			chance += (victim->power[DISC_WERE_MANT] *2.5);
            if (IS_CLASS(victim, CLASS_JEDI) && victim->pcdata->powers[JPOWER_BODY] >= 4)
                        chance += 25;
    }

    if (chance > 75)
		chance = 75;
    else if (chance < 25)
		chance = 25;

    if (!IS_NPC(ch) && ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
		if (ch->pcdata->rank == AGE_METHUSELAH)
			chance -= 20;
		else if (ch->pcdata->rank == AGE_ELDER)
			chance -= 15;
		else if (ch->pcdata->rank == AGE_ANCILLA)
			chance -= 10;
    }

    if (!IS_NPC(victim) && victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE))
    {
		if (victim->pcdata->rank == AGE_METHUSELAH)
			chance += 20;
		else if (victim->pcdata->rank == AGE_ELDER)
			chance += 15;
		else if (victim->pcdata->rank == AGE_ANCILLA)
			chance += 10;
    }

        if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_BERSERK)) )
         {
		if (victim->bloodlust > 10)
			chance += 30;
                if (victim->bloodlust > 50)
			chance += 20;
		if (victim->bloodlust >= 100)
			chance += 15;
	}
	if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_NINJA)) )
	{
		if (victim->generation < 3)
			chance += 20;
		else
			chance += 25;
	}
  if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_MECHA)) )
        {
                if (victim->generation < 3)
                        chance += 20;
                else
                        chance += 25;
        }

  if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_BERSERK)) )
{
if (victim->bloodlust > 10)
chance -= 15;                                                        
    if (victim->bloodlust > 50)
chance -= 20;                                                    
        if (victim->bloodlust >= 100)       
chance -= 35;
                                                    }                
          
	if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_NINJA)) )
	{
		if (ch->generation < 3)
			chance -= 25;
		else
			chance -= 20;
	}
 if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_NINJA)) )
        {
                if (ch->generation < 3)
                        chance -= 25;
                else
                        chance -= 20;
        } 
        if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_AMAZON)))
                chance -= 20;
        if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_AMAZON)))
                chance += 20;
        if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_MAGE)))
	{
                chance -= 15;
		if (check_poly(ch,POLY_HUGE_SERPENT) || check_poly(ch, POLY_WYVERN))
		chance -= 10;
	}
        if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_MAGE)))
	{
                chance += 15;
		if (check_poly(victim, POLY_HUGE_SERPENT) || check_poly(victim, POLY_WYVERN))
		chance += 10;
	}
	if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_MONK) &&  IS_SET(ch->newbits, NEW_MONKADAM) ) )
		chance -= 25;
	if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_MONK) &&  IS_SET(victim->newbits, NEW_MONKADAM) ) )
		chance += 25;
/* 1.5k wpn give alot of bonus
        if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_HIGHLANDER)))
                chance -= 15;
        if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_HIGHLANDER)))
                chance += 15;
*/
	if ( number_percent( ) < 100 && number_percent( ) >= chance )
		return FALSE;

    if (claws)
    {
		if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		{
			if (IS_CLASS(victim, CLASS_WEREWOLF))
				act( "You parry $n's blow with your claws.",  ch, NULL, victim, TO_VICT );
			else
				act( "You parry $n's blow with your Adamantium hands.", ch,NULL,victim, TO_VICT);
        }
		if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		{
			if (IS_CLASS(victim, CLASS_WEREWOLF))
			    act( "$N parries your blow with $S claws.", ch, NULL, victim, TO_CHAR );
			else
				act( "$N parries your blow with $S Adamantium claws.", ch,NULL,victim, TO_CHAR);
		}
		return TRUE;
    }
    if (!IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0 && obj->value[3] <= 12)
    {
		if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		    act( "You parry $n's blow with $p.",  ch, obj, victim, TO_VICT );
		if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		    act( "$N parries your blow with $p.", ch, obj, victim, TO_CHAR );
		return TRUE;
    }
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT    );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		act( "$N parries your attack.", ch, NULL, victim, TO_CHAR    );
    return TRUE;
}/* End of bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )*/
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int insp = IS_UNI(ch) ? countinspires(ch) : 0;
    int chance = 0;
if (insp > 12) insp = 12;
    if ( !IS_AWAKE(victim) )
		return FALSE;
    if (!IS_NPC(ch))
		chance -= (ch->wpn[dt-1000] * 0.1);
	else
		chance -= (ch->level * 0.5);
    if (!IS_NPC(victim))
		chance += (victim->wpn[0] * 0.5);
	else
		chance += victim->level;
    if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && victim->stance[STANCE_MONGOOSE] > 0
	&& !can_counter(ch) && !can_bypass(ch,victim))
		(chance += victim->stance[STANCE_MONGOOSE] * 0.25);
    if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) &&	victim->stance[STANCE_SWALLOW] > 0
	&& !can_counter(ch) && !can_bypass(ch,victim))
	(chance += victim->stance[STANCE_SWALLOW] * 0.25);

    if (!IS_NPC(ch))
    {
		     if (ch->power[DISC_DAEM_ATTA] > 0)
			chance -= (ch->power[DISC_DAEM_ATTA]+2);
		else if (IS_CLASS(ch, CLASS_WEREWOLF) &&
			ch->power[DISC_WERE_MANT] <= 10 )
	    chance -= (ch->power[DISC_WERE_MANT] * 2.5);
		else if (IS_CLASS(ch, CLASS_VAMPIRE))
		    chance -= ch->power[DISC_VAMP_CELE];
    }
    if (!IS_NPC(victim))
    {
		     if (victim->power[DISC_DAEM_ATTA] > 0)
			chance += (victim->power[DISC_DAEM_ATTA]+2) ;
		else if (IS_CLASS(victim, CLASS_WEREWOLF) &&
				victim->power[DISC_WERE_MANT] <= 10)
            chance += (victim->power[DISC_WERE_MANT] * 2.5);
    }

    if (chance > 75)
		chance = 75;
    else if (chance < 25)
		chance = 25;
if (IS_MOOGLE(ch)) chance -= 65;
if (IS_MOOGLE(victim)) chance += 40;

for (classcon = config_first; classcon; classcon = classcon->next)
 if (!IS_NPC(ch) && classcon->classbit == ch->class) chance -= classcon->dodgeoff;

for (classcon = config_first; classcon; classcon = classcon->next)
 if (!IS_NPC(victim) &&classcon->classbit == victim->class) chance += classcon->dodgedef;

chance -= (insp * 7);

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
		if (ch->pcdata->rank == AGE_METHUSELAH)
			chance -= 20;
		else if (ch->pcdata->rank == AGE_ELDER)
			chance -= 15;
		else if (ch->pcdata->rank == AGE_ANCILLA)
			chance -= 10;
    }
	if (will_use(ch, "Super Speed")) chance -= 20;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
    {
		if (victim->pcdata->rank == AGE_METHUSELAH)
			chance += 20;
		else if (victim->pcdata->rank == AGE_ELDER)
			chance += 15;
		else if (victim->pcdata->rank == AGE_ANCILLA)
			chance += 10;
    }
if (IS_CLASS(victim, CLASS_GOLEM))
{
 if (victim->pcdata->powers[0] & GOLEM_CRYSTAL)
	chance += 30;
 else
	chance += 16;
}
if (IS_CLASS(ch, CLASS_GOLEM))
{
    if (ch->pcdata->powers[0] & GOLEM_CRYSTAL)
	chance -= 23;
    else if (ch->pcdata->powers[0] & GOLEM_OBSIDIAN)
	chance -= number_range(30, 45);
}
if (IS_CLASS(ch,CLASS_SHAPE))
chance -= 20;
	if (will_use(victim, "Enhanced Dodge"))
		chance += 30;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_AMAZON))
                        chance += 20;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_AMAZON))
                        chance -= 20;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW))
                        chance -= 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW))
                        chance += 25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK))
                        chance -= 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK))
                        chance += 25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
                        chance -= 25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_BERSERK))
			chance +=25;
if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_BERSERK))
			chance -=25;
 if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MECHA))
                        chance -= 20;   
if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
                        chance += 35;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
                        chance += 25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON))
                        chance -= 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON))
                        chance += 25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE))
	{
                        chance -= 10;
		if (check_poly(ch, POLY_HUGE_SERPENT) || check_poly(ch,
POLY_WYVERN))
			chance -= 15;
	}
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE))                        chance += 20;
	{
			chance += 20;
		if (check_poly(victim, POLY_HUGE_SERPENT) ||
check_poly(victim, POLY_WYVERN))
			chance += 5;
	}
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF))
                        chance -= 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF))
                        chance += 25;
/* 1.5k slice give alot of bonus
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HIGHLANDER))
                        chance -= 20;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HIGHLANDER))
                        chance += 20;
*/
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_JEDI) && ch->pcdata->powers[JPOWER_BODY] >=3)
                        chance -= 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_JEDI) && victim->pcdata->powers[JPOWER_BODY] >= 3)
                        chance += 25;
    if ( number_percent( ) < 100 && number_percent( ) >= chance )
		return FALSE;

    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR );
    return TRUE;
}/* End of bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) */
void update_pos( CHAR_DATA *victim )
{
	CHAR_DATA *mount;
    if ( victim->hit > 0 )
    {
    	if ( victim->position <= POS_STUNNED )
		{
			bool gm_stance = FALSE;
		    victim->position = POS_STANDING;
		    if (!IS_NPC(victim) && victim->stance[0] > 0)
		    {
				int stance = victim->stance[0];
				if (victim->stance[stance] >= 200) gm_stance = TRUE;
		    }
			if (IS_NPC(victim) || victim->max_hit * 0.25 > victim->hit || !gm_stance)
		    {
				act( "$n clambers back to $s feet.", victim, NULL, NULL, TO_ROOM);
				act( "You clamber back to your feet.", victim, NULL, NULL, TO_CHAR);
		    }
		    else
		    {
				act( "$n flips back up to $s feet.", victim, NULL, NULL, TO_ROOM);
				act( "You flip back up to your feet.", victim, NULL, NULL, TO_CHAR);
		    }
		}
		return;
    }
    else if ( (mount = victim->mount) != NULL)
    {
		if (victim->mounted == IS_MOUNT)
		{
		    act("$n rolls off $N.",mount,NULL,victim,TO_ROOM);
			act("You roll off $N.",mount,NULL,victim,TO_CHAR);
		}
		else if (victim->mounted == IS_RIDING)
		{
		    act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
			act("You fall off $N.",victim,NULL,mount,TO_CHAR);
		}
		mount->mount    = NULL;
		victim->mount   = NULL;
		mount->mounted  = IS_ON_FOOT;
		victim->mounted = IS_ON_FOOT;
	}

    if (!IS_NPC(victim) && victim->hit <=-11 && IS_HERO(victim))
    {
        victim->hit = -10;
        if (victim->position == POS_FIGHTING) 
           stop_fighting(victim,TRUE);
        return;
    }

    if ( IS_NPC(victim) || victim->hit <= -11 )
    {
		victim->position = POS_DEAD;
		return;
    }

    if ( victim->hit <= -6 )
		victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 )
		victim->position = POS_INCAP;
    else
		victim->position = POS_STUNNED;
    return;
}/* End of void update_pos( CHAR_DATA *victim ) */


void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( ch->fighting != NULL )
    {
		bug( "Set_fighting: already fighting", 0 );
		return;
    }
    if ( IS_AFFECTED(ch, AFF_SLEEP) )
		affect_strip( ch, gsn_sleep );
    ch->fighting = victim;
    ch->position = POS_FIGHTING;

    if (ch->autostance > 0 && ch->autostance < 12 &&
IS_STANCE(ch, STANCE_NONE))
    {
	switch(ch->autostance)
	{
	case STANCE_WOLF:	do_stance(ch, "wolf"); break;
	case STANCE_BULL:	do_stance(ch, "bull");	break;
	case STANCE_CRANE:	do_stance(ch, "crane");	break;
	case STANCE_CRAB:		do_stance(ch, "crab");	break;
	case STANCE_MONGOOSE:	do_stance(ch, "mongoose");	break;
	case STANCE_VIPER:	do_stance(ch, "viper");	break;
	case STANCE_DRAGON:	do_stance(ch, "dragon");	break;
	case STANCE_TIGER:	do_stance(ch, "tiger");	break;
	case STANCE_MANTIS:	do_stance(ch, "mantis");	break;
	case STANCE_MONKEY:	do_stance(ch, "monkey");	break;
	case STANCE_SWALLOW:	do_stance(ch, "swallow");	break;
	default:	send_to_char("Autostance bug, report to imp.\r\n", ch);
	}
    }

    ch->damcap[DAM_CHANGE] = 1;
    return;
}
void stop_embrace( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if (victim==NULL)
	{
		ch->embracing=NULL;
		return;
	}
	victim->embraced=NULL;
	ch->embracing=NULL;
	return;
}
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
		if ( fch == ch || ( fBoth && fch->fighting == ch ) )
		{
		    fch->fighting	= NULL;
		    fch->position	= POS_STANDING;
			update_pos( fch );
		}
    }
    return;
}
void stop_newfighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
		if ( fch == ch || ( fBoth && fch->newfighting == ch ) )
		{
		    fch->newfighting	= NULL;
		    fch->position	= POS_STANDING;
			update_pos( fch );
		}
    }
    return;
}
void make_corpse( CHAR_DATA *ch )
{
	char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;

    
	if ( IS_NPC(ch) )
    {
		name		= ch->short_descr;
		corpse		= create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
		corpse->timer	= number_range( 4, 8 );
		corpse->value[2]= ch->pIndexData->vnum;
		if ( ch->gold > 0 )
		{
			obj_to_obj( create_money( ch->gold ), corpse );
			ch->gold = 0;
		}
    }
    else
    {
		name		= ch->name;
		corpse		= create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
		corpse->timer	= number_range( 25, 40 );
		/* Why should players keep their gold? */
		if ( ch->gold > 0 )
		{
		    obj = create_money( ch->gold );
		    if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
		        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
		    obj_to_obj( obj, corpse );
			ch->gold = 0;
		}
    }
	if (IS_SET(ch->extra, EXTRA_ZOMBIE))
		SET_BIT(corpse->quest, QUEST_ZOMBIE);

    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );
   
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
		obj_next = obj->next_content;
		obj_from_char( obj );
		if ( IS_SET( obj->extra_flags, ITEM_VANISH ) )
			extract_obj( obj );
		else
		{
			if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
				SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
			obj_to_obj( obj, corpse );
		}
    }

    /* If you die in the shadowplane, your corpse should stay there */
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
		SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);
    obj_to_room( corpse, ch->in_room );
    return;
}
void death_cry( CHAR_DATA *ch )
{
	ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;

    if ( IS_NPC(ch) )
		msg = "You hear something's death cry.";
    else
		msg = "You hear someone's death cry.";
    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
		EXIT_DATA *pexit;
		if ( ( pexit = was_in_room->exit[door] ) != NULL && pexit->to_room != NULL && pexit->to_room != was_in_room )
		{
			ch->in_room = pexit->to_room;
		    act( msg, ch, NULL, NULL, TO_ROOM );
		}
    }
    ch->in_room = was_in_room;
    return;
}
void make_part( CHAR_DATA *ch, char *argument )
{
    char  arg [MAX_INPUT_LENGTH];
    int vnum;

    argument = one_argument(argument,arg);
    vnum = 0;

    if (arg[0] == '\0')
		return;
    if (!str_cmp(arg,"head")) vnum = OBJ_VNUM_SEVERED_HEAD;
    else if (!str_cmp(arg,"arm")) vnum = OBJ_VNUM_SLICED_ARM;
    else if (!str_cmp(arg,"leg")) vnum = OBJ_VNUM_SLICED_LEG;
    else if (!str_cmp(arg,"heart")) vnum = OBJ_VNUM_TORN_HEART;
    else if (!str_cmp(arg,"turd")) vnum = OBJ_VNUM_TORN_HEART;
    else if (!str_cmp(arg,"entrails")) vnum = OBJ_VNUM_SPILLED_ENTRAILS;
    else if (!str_cmp(arg,"brain")) vnum = OBJ_VNUM_QUIVERING_BRAIN;
    else if (!str_cmp(arg,"eyeball")) vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
    else if (!str_cmp(arg,"blood")) vnum = OBJ_VNUM_SPILT_BLOOD;
    else if (!str_cmp(arg,"face")) vnum = OBJ_VNUM_RIPPED_FACE;
    else if (!str_cmp(arg,"windpipe")) vnum = OBJ_VNUM_TORN_WINDPIPE;
    else if (!str_cmp(arg,"cracked_head")) vnum = OBJ_VNUM_CRACKED_HEAD;
    else if (!str_cmp(arg,"ear")) vnum = OBJ_VNUM_SLICED_EAR;
    else if (!str_cmp(arg,"nose")) vnum = OBJ_VNUM_SLICED_NOSE;
    else if (!str_cmp(arg,"tooth")) vnum = OBJ_VNUM_KNOCKED_TOOTH;
    else if (!str_cmp(arg,"tongue")) vnum = OBJ_VNUM_TORN_TONGUE;
    else if (!str_cmp(arg,"hand")) vnum = OBJ_VNUM_SEVERED_HAND;
    else if (!str_cmp(arg,"foot")) vnum = OBJ_VNUM_SEVERED_FOOT;
    else if (!str_cmp(arg,"thumb")) vnum = OBJ_VNUM_SEVERED_THUMB;
    else if (!str_cmp(arg,"index")) vnum = OBJ_VNUM_SEVERED_INDEX;
    else if (!str_cmp(arg,"middle")) vnum = OBJ_VNUM_SEVERED_MIDDLE;
    else if (!str_cmp(arg,"ring")) vnum = OBJ_VNUM_SEVERED_RING;
    else if (!str_cmp(arg,"little")) vnum = OBJ_VNUM_SEVERED_LITTLE;
    else if (!str_cmp(arg,"toe")) vnum = OBJ_VNUM_SEVERED_TOE;

    if ( vnum != 0 )
    {
		char buf[MAX_STRING_LENGTH];
		OBJ_DATA *obj;
		char *name;
		name = IS_NPC(ch) ? ch->short_descr : ch->name;
		obj = create_object( get_obj_index( vnum ), 0 );
		if (IS_NPC(ch))
			obj->timer = number_range(2,5);
		else
			obj->timer = -1;
		if (!str_cmp(arg,"head") && IS_NPC(ch))
			obj->value[1] = ch->pIndexData->vnum;
		else if (!str_cmp(arg,"head") && !IS_NPC(ch))
	    {
			ch->pcdata->chobj = obj;
			obj->chobj = ch;
			obj->timer = number_range(2,3);
		}
		else if (!str_cmp(arg,"arm"))
			SET_BIT(obj->extra_flags2, ITEM_ARM);
		else if (!str_cmp(arg,"brain") && !IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH) && IS_HEAD(ch, LOST_HEAD))
		{
			if (ch->pcdata->chobj != NULL)
				ch->pcdata->chobj->chobj = NULL;
			ch->pcdata->chobj = obj;
			obj->chobj = ch;
		    obj->timer = number_range(2,3);
		}

		/* For blood :) KaVir */
		if (vnum == OBJ_VNUM_SPILT_BLOOD) obj->timer = 2;

		/* For voodoo dolls - KaVir */
		if (!IS_NPC(ch))
		{
		    sprintf( buf, obj->name, name );
			free_string( obj->name );
		    obj->name = str_dup( buf );
		}
		else
		{
			sprintf( buf, obj->name, "mob" );
		    free_string( obj->name );
		    obj->name = str_dup( buf );
		}

		sprintf( buf, obj->short_descr, name );
		free_string( obj->short_descr );
		obj->short_descr = str_dup( buf );
	
		sprintf( buf, obj->description, name );
		free_string( obj->description );
		obj->description = str_dup( buf );

    	if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
			SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
		obj_to_room( obj, ch->in_room );
    }
    return;
}/* End of void make_part( CHAR_DATA *ch, char *argument ) */
void raw_kill( CHAR_DATA *victim )
{
    CHAR_DATA *mount;
    stop_fighting( victim, TRUE );
    death_cry( victim );
    make_corpse( victim );

    if ( (mount = victim->mount) != NULL)
    {
		if (victim->mounted == IS_MOUNT)
		{
		    act("$n rolls off the corpse of $N.",mount,NULL,victim,TO_ROOM);
		    act("You roll off the corpse of $N.",mount,NULL,victim,TO_CHAR);
		}
		else if (victim->mounted == IS_RIDING)
		{
		    act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
			act("You fall off $N.",victim,NULL,mount,TO_CHAR);
		}
		mount->mount    = NULL;
		victim->mount   = NULL;
		mount->mounted  = IS_ON_FOOT;
		victim->mounted = IS_ON_FOOT;
    }
 
    if ( IS_NPC(victim) )
    {
		victim->pIndexData->killed++;
		kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
		extract_char( victim, TRUE );
		return;
    }

    extract_char( victim, FALSE );
    while ( victim->affected )
	affect_remove( victim, victim->affected );
    if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    {
    	victim->affected_by	= AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    	victim->affected_by	= AFF_POLYMORPH;
    else if (IS_AFFECTED(victim,AFF_ETHEREAL))
    	victim->affected_by	= AFF_ETHEREAL;
    else
    	victim->affected_by	= 0;

    REMOVE_BIT(victim->immune, IMM_STAKE);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);
    REMOVE_BIT(victim->extra, EXTRA_PREGNANT);
    REMOVE_BIT(victim->extra, EXTRA_LABOUR);
    victim->pcdata->stats[DEMON_POWER] = 0;

    victim->itemaffect	 = 0;
    victim->loc_hp[0]    = 0;
    victim->loc_hp[1]    = 0;
    victim->loc_hp[2]    = 0;
    victim->loc_hp[3]    = 0;
    victim->loc_hp[4]    = 0;
    victim->loc_hp[5]    = 0;
    victim->loc_hp[6]    = 0;
    victim->armor	 = 100;
    victim->position	 = POS_RESTING;
    victim->hit		 = UMAX( 1, victim->hit  );
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->carry_weight = 0;
    victim->carry_number = 0;
    do_call(victim,"all");
    save_char_obj( victim );
    return;
}
void behead( CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if (IS_NPC(victim))
		return;

    location = victim->in_room;
    stop_fighting( victim, TRUE );
    make_corpse( victim );
	extract_char( victim, FALSE );
    char_from_room(victim);
    char_to_room(victim,location);

    make_part( victim, "head" );

    while ( victim->affected )
	affect_remove( victim, victim->affected );
    if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    {
    	victim->affected_by	= AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    	victim->affected_by	= AFF_POLYMORPH;
    else if (IS_AFFECTED(victim,AFF_ETHEREAL))
    	victim->affected_by	= AFF_ETHEREAL;
    else
    	victim->affected_by	= 0;

    REMOVE_BIT(victim->immune, IMM_STAKE);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);
    REMOVE_BIT(victim->extra, EXTRA_PREGNANT);
    REMOVE_BIT(victim->extra, EXTRA_LABOUR);
    victim->pcdata->stats[DEMON_POWER] = 0;

    victim->itemaffect	 = 0;
    victim->loc_hp[0]    = 0;
    victim->loc_hp[1]    = 0;
    victim->loc_hp[2]    = 0;
    victim->loc_hp[3]    = 0;
    victim->loc_hp[4]    = 0;
    victim->loc_hp[5]    = 0;
    victim->loc_hp[6]    = 0;
    victim->affected_by	 = 0;
    victim->armor	 = 100;
    victim->position	 = POS_STANDING;
    victim->hit		 = 1;
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->carry_weight = 0;
    victim->carry_number = 0;

    if (victim->cur_form != get_normal_form(victim))
    {
		set_form(victim, get_normal_form(victim));
		stc("Your body reverts to it's normal form.\n\r",victim);
    }

    SET_BIT(victim->loc_hp[0],LOST_HEAD);
    SET_BIT(victim->affected_by,AFF_POLYMORPH);
    sprintf(buf,"the severed head of %s",victim->name);
    free_string(victim->morph);
    victim->morph = str_dup(buf);
    do_call(victim,"all");
    save_char_obj( victim );
    return;
}
void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch = NULL;
    CHAR_DATA *lch;
    CHAR_DATA *mount;
    int xp;
    int members;
/*
if ( gch->affected )
  {
      for ( paf = gch->affected; paf != NULL; paf = paf->next )
      {

      if (paf->type && skill_table[paf->type].name && strcmp(skill_table[paf->type].name,"4x")== 0) { xp = xp * paf->modifier;
sprintf(buf2,"{C%d{Wx {MExperience!{x\n\r",paf->modifier);
send_to_char(buf2, gch); }
      }
   }
if you renable this put it in or after the for loop, or it will crash*/

    /* Monsters don't get kill xp's or alignment changes.P-killing doesn't help either.Dying of mortal wounds or poison doesn't give xp to anyone!  */
    if ( (IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch )
		return;
    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
		if ( is_same_group( gch, ch ) )
			members++;
    }

    if ( members == 0 )
    {
		members = 1;
    }

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
		OBJ_DATA *obj;
		OBJ_DATA *obj_next;

		if ( !is_same_group( gch, ch ) )
			continue;
		xp = xp_compute( gch, victim );
		sprintf( buf, "You receive %d experience points.\n\r", xp );
		send_to_char( buf, gch );
		if ((mount = gch->mount) != NULL) send_to_char( buf, mount );
			gain_exp( gch, xp );
		for ( obj = ch->carrying; obj != NULL; obj = obj_next )
		{
		    obj_next = obj->next_content;
		    if ( obj->wear_loc == WEAR_NONE )
				continue;
		    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
		    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
			||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
			{
				act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
				act( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );
				obj_from_char( obj );
				obj_to_room( obj, ch->in_room );
		    }
		}
    }
    return;
}

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
    int xp = 0;
    int diff;
    int hours;
    int count=0;

    if (!IS_NPC(victim)) return 0;

    if (IS_NPC(victim) && IS_SET(victim->act, ACT_NOEXP))
        return 0;

    diff = (victim->level * 22);

/*    if (diff > 1700)
        diff = 1700;
*/
    if (diff >= 1500) xp = diff * 20;
    else if (diff >= 1400) xp = diff * 14;
    else if (diff >= 1300) xp = diff * 13;
    else if (diff >= 1200) xp = diff * 13;
    else if (diff >= 1000) xp = diff * 13;
    else if (diff >= 600) xp = diff * 13;
    else if (diff >= 500) xp = diff * 12;
    else if (diff >= 400) xp = diff * 12;
    else if (diff >= 300) xp = diff * 12;
    else if (diff >= 200) xp = diff * 11;
    else if (diff >= 100) xp = diff * 11;
    else if (diff > 0) xp = diff * 10;
    else return 0;


    /* Player kills */
    if (!IS_NPC(victim)) xp *= 3;

    /* Good killing evil */
    if (IS_GOOD(gch) && IS_EVIL(victim)) xp = xp * 3 / 2;

    /* Evil killing good */
    else if (IS_EVIL(gch) && IS_GOOD(victim)) xp = xp * 3 / 2;

    /* Good killing good */
    else if (IS_GOOD(gch) && IS_GOOD(victim)) xp = xp * 2 / 3;

    /* Evil killing evil */
    else if (IS_EVIL(gch) && IS_EVIL(victim)) xp= xp * 2 / 3;

    /* randomize the rewards */
    xp = number_range ( (xp * 5 / 6), (xp * 6 / 5));

    hours = get_hours_played(gch);

/* more hours played = more xp! */

    if (hours >= 100)
        xp *= 3;
    else if (hours >= 10)
        xp *= 1.3;
    else if (hours >= 20)
        xp *= 1.5;
    else if (hours >= 30)
        xp *= 1.7;
    else if (hours >= 40)
        xp *= 1.9;
    else if (hours >= 50)
        xp *= 2;
    else if (hours >= 60)
        xp *= 2.2;
    else if (hours >= 70)
        xp *= 2.4;
    else if (hours >= 110)
    xp *=3.2;
     else if (hours >= 160)
      xp *=3.5;
     else if (hours >= 200)
      xp *=4;
     else if (hours >= 300)
      xp *=5;
    else if (hours >= 80)
        xp *= 2.6;
    else if (hours >= 90)
        xp *= 2.8;
        xp *= 1.3;
/* hrmm...
    xp *= config_info->research_exp_mult;
    xp /= config_info->research_exp_div;
*/

    /* Discipline points */
    if (!IS_NPC(gch) && xp >= 750 && gch->pcdata->disc_points != 666
                && gch->pcdata->disc_research != -1)
    {
                stc("You gained a discipline point.\n\r", gch);
                gain_disc_points(gch, 1);
    }
/*if (victim->level > 400) {
lvl = victim->level;
lvl /= 400;
xp *= lvl;}
if (victim->level < 401) xp *= 2.5;*/
    if (xp < 0)
        xp = 1;
//      xp *= 1.6;
//      xp *= 1.8;
if (IS_MOOGLE(gch))
{int nuts = victim->level*.025, t;
for (t = 0; t < 10; t++)
    nuts = number_fuzzy(nuts);
sprintf(log_buf, "#2You get #3%d#2 kuppo nuts from the rotted corpse.#n\n\r", nuts);
stc(log_buf, gch);
gch->pcdata->powers[MOOG_NUTS] += nuts;
}
if (nodamcap) xp *= .80;
{bool grin = FALSE;
for ( victim =gch->in_room->people; victim != NULL; victim = victim->next_in_room )
        {
            if ( is_same_group( gch, victim ) )
            {
count++;if(str_cmp(victim->immpower,"")&&str_cmp(victim->immpower,"(null)"))
grin = TRUE;}
        }
if (!grin)
xp /= count;
}
    return xp;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
    static char * const attack_table[] =
    {
		"hit",
		"slice",  "stab",  "slash", "whip", "claw",
		"blast",  "pound", "crush", "grep", "bite",
		"pierce", "suck"
    };
    static char * const attack_table2[] =
    {
		"hits",
		"slices",  "stabs",  "slashes", "whips", "claws",
		"blasts",  "pounds", "crushes", "greps", "bites",
		"pierces", "sucks"
    };

    char buf1[512], buf2[512], buf3[512];
    const char *vs;
    const char *vp;
    const char *attack;
    const char *attack2;
    int damp;
    int bodyloc;
    int recover;
    bool critical = FALSE;
    char punct;

	if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_REND)&& dam>0 )
		critical=TRUE;
         if ( dam ==   0 ) { vs = " miss";	vp = " misses";		}
	else if ( dam <=  25 ) { vs = " with laughable force"; vp = " with laughable force";}
    else if ( dam <=  50 ) { vs = " lightly";	vp = " lightly";	}
    else if ( dam <= 100 ) { vs = "";		vp = "";		}
    else if ( dam <= 250 ) { vs = " hard";	vp = " hard";		}
    else if ( dam <= 500 ) { vs = " very hard";	vp = " very hard";	}
    else if ( dam <= 750 ) { vs = " extremely hard"; vp = " extremely hard";}
	else if ( dam <= 1000 ) { vs = " insanely hard!"; vp = " insanely hard!";}
    else if ( dam <= 1250 ) { vs = " amazingly hard!!";vp = " amazingly!!";}	
    else if ( dam <= 1500 ) { vs = " DAMN HARD";vp= " DAMN HARD";}
	else if ( dam <= 1750 ) { vs = " -#3LUDICROUSLY HARD#n-";vp= " -#3LUDICROUSLY HARD#n-";}
	else if ( dam <= 2000 ) { vs = " for #3**#1 SOUL WRENCHING DAMAGE #3**#n";vp= " #3**#1 SOUL WRENCHING DAMAGE #3**#n";}
    else if ( dam <= 2250 ) { vs = " for #1-==*#6 EARTH #3SHATTERING #6DAMAGE #1*==-";vp = " for #1-==*#6 EARTH #3SHATTERING #6DAMAGE #1*==-";}
	else if ( dam <= 3000 ) { vs = " for #3o#0xvxvx#3(#7====#5UNSPEAKABLE#7====#3)#0xvxvx#3o#n damage"; vp = " for #3o#0xvxvx#3(#7====#5UNSPEAKABLE#7====#3)#0xvxvx#3o#n damage"; }
	else if ( dam <= 3500 ) { vs = " #7MONSTROUSLY HARD#n";vp= " #7MONSTROUSLY HARD#n";}
	else if ( dam <= 4000 ) { vs = " #1HARDER THAN #7GOD#n";vp= " #1HARDER THAN GOD#n";}
	else     { vs = " #5and tosses them around#n";vp= " #5and tosses them around#n";}

    /* If victim's hp are less/equal to 'damp', attacker gets a death blow */
    if (IS_NPC(victim))
		damp = 0;
    else 
		damp = -10;
    if ( (victim->hit - dam > damp ) || (dt >= 0 && dt < MAX_SKILL) || (IS_NPC(victim) && IS_SET(victim->act, ACT_NOPARTS)) )
    {
    	punct   = (dam <= 250) ? '.' : '!';
    	if ( dt == TYPE_HIT && !IS_NPC(ch) && !IS_VAMPAFF(ch,VAM_CLAWS) )
    	{
		    if (dam == 0)
		    {
				sprintf( buf1, "$n%s $N%c[%d]",  vp, punct, dam );
	    		sprintf( buf2, "You%s $N%c[%d]", vs, punct, dam );
		    	sprintf( buf3, "$n%s you%c", vp, punct );
		    }
			else
		    {
		    	sprintf( buf1, "$n hits $N%s%c[%d]",  vp, punct, dam );
				sprintf( buf2, "You hit $N%s%c[%d]", vs, punct, dam );
	    		sprintf( buf3, "$n hits you%s%c[%d]", vp, punct, dam );
			if (dam > 500)
				critical = TRUE;
		    }
    	}
		else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_SET(ch->newbits, NEW_MONKFLAME))
    	{
			if (dam >= 1)
		    {
		        if (!IS_CLASS(ch, CLASS_WEREWOLF))
			    {
	    			sprintf( buf1, "$n's flaming hands hit $N%s%c[%d]", vp, punct, dam );
		    		sprintf( buf2, "Your flaming hands hit $N%s%c[%d]", vs, punct, dam );
		    		sprintf( buf3, "$n's flaming hands hit you%s%c[%d]", vp, punct, dam );
				}
				else
				{
					sprintf( buf1, "$n's flaming claws hit $N%s%c[%d]", vp, punct, dam );
	                sprintf( buf2, "Your flaming claws hit $N%s%c[%d]", vs, punct, dam );
		            sprintf( buf3, "$n's flaming claws hit you%s%c[%d]", vp, punct, dam );
				}
			}
		    else 
			{
	    		sprintf( buf1, "$n's hit%s $N%c[%d]",  vp, punct, dam );
		    	sprintf( buf2, "Your hit%s $N%c[%d]", vs, punct, dam );
		    	sprintf( buf3, "$n hit%s you%c[%d]", vp, punct, dam );
			if (dam > 500)
				critical = TRUE;
			}
		}
    	else
    	{
			if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_CLAWS))
		    {
		    	attack  = attack_table[dt - TYPE_HIT + 5];
				attack2 = attack_table2[dt - TYPE_HIT + 5];
			}
			else if ( dt >= 0 && dt < MAX_SKILL )
			{
		    	attack = skill_table[dt].noun_damage;
		    	attack2 = skill_table[dt].noun_damage;
			}
			else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]) )
			{
	    		attack  = attack_table[dt - TYPE_HIT];
		    	attack2 = attack_table2[dt - TYPE_HIT];
		    }
			else
			{
				dt  = TYPE_HIT;
				attack  = attack_table[0];
				attack2 = attack_table2[0];
			}
		    if (dam == 0)
		    {
				sprintf( buf1, "$n's %s%s $N%c[%d]",  attack, vp, punct, dam );
	    		sprintf( buf2, "Your %s%s $N%c[%d]",  attack, vp, punct, dam );
		    	sprintf( buf3, "$n's %s%s you%c[%d]", attack, vp, punct, dam );
		    }
		    else if (IS_SET(ch->newbits, NEW_MONKFLAME) && (dam == 0))
		    {
				sprintf( buf1, "$n's flaming %s%s $N%c[%d]", attack2, vp, punct, dam );
	    		sprintf( buf2, "Your flaming %s%s $N%c[%d]",  attack, vp, punct, dam );
		    	sprintf( buf3, "$n's flaming %s%s you%c[%d]", attack2, vp, punct, dam );
		    }
		    else
		    {
				if ( dt >= 0 && dt < MAX_SKILL )
				{
				    sprintf( buf1, "$n's %s strikes $N%s%c[%d]",  attack2, vp, punct, dam );
	    		    sprintf( buf2, "Your %s strikes $N%s%c[%d]",  attack, vp, punct, dam );
	    			sprintf( buf3, "$n's %s strikes you%s%c[%d]", attack2, vp, punct, dam );
				}
				else if (IS_SET(ch->newbits, NEW_MONKFLAME))
				{
			        sprintf( buf1, "$n's flaming %s hits $N%s%c[%d]", attack2, vp, punct, dam);
	                sprintf( buf2, "Your flaming %s hits $N%s%c[%d]", attack, vp, punct, dam);
		            sprintf( buf3, "$n's flaming %s hits you%s%c[%d]", attack2, vp, punct, dam);
			    }
    			else
				{
		    	    sprintf( buf1, "$n %s $N%s%c[%d]",  attack2, vp, punct, dam );
				    sprintf( buf2, "You %s $N%s%c[%d]",  attack, vp, punct, dam );
	    		    sprintf( buf3, "$n %s you%s%c[%d]", attack2, vp, punct, dam );
				if (dam > 500)
					critical = TRUE;
				}
		    }
		
			recover = 0;

		    /* Check for weapon resistance - KaVir */
		    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SLASH) && (attack == "slash" || attack == "slice") && dam > 0 )
				recover = number_range(1,dam);
		    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_STAB) && (attack == "stab" || attack == "pierce") && dam > 0 )
				recover = number_range(1,dam);
			if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SMASH) && (attack == "blast" || attack == "pound"
			|| attack == "crush") && dam > 0 )
				recover = number_range(1,dam);
			if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_ANIMAL) && (attack == "bite" || attack == "claw") && dam > 0 )
				recover = number_range(1,dam);
		    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_MISC) && (attack == "grep" || attack == "suck" || attack == "whip") && dam > 0 )
				recover = number_range(1,dam);

		    victim->hit = victim->hit + recover;

		if (!IS_NPC(victim) && IS_SET(victim->newbits, NEW_SKIN )
			&& IS_CLASS(victim, CLASS_WEREWOLF) &&
			(dam - recover > 0) )
		{
			victim->hit += number_range(1, (dam-recover));
		}


		}
		act( buf1, ch, NULL, victim, TO_NOTVICT );
	    act( buf2, ch, NULL, victim, TO_CHAR );
		act( buf3, ch, NULL, victim, TO_VICT );
		if (critical) critical_hit(ch,victim,dt,dam);
			return;
	}
    if ( dt == TYPE_HIT && !IS_NPC( ch ) && !IS_VAMPAFF(ch,VAM_CLAWS) &&!IS_VAMPAFF(ch,VAM_FANGS))
    {
		damp=number_range(1,5);
		if ( damp == 1 )
		{
			act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR);
		    act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT);
		    act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT);
			make_part(victim,"face");
		}
		else if ( damp == 2 )
		{
			act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR);
		    act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT);
		    act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT);
			make_part(victim,"windpipe");
		}
		else if ( damp == 3 )
		{
		    act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR);
		    act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT);
			act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"entrails");
		}
		else if ( damp == 4 )
		{
		    if (!IS_BODY(victim,BROKEN_SPINE))
				SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
			    act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR);
			    act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT);
				act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT);
		}	
		else if ( damp == 5 )
		{
		    act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR);
		    act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT);
		    act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT);
		    if (!IS_BODY(victim,BROKEN_NECK))
			{
				act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR);
				act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM);
				SET_BIT(victim->loc_hp[1],BROKEN_NECK);
			}
		}
		return;
    }
    if ( dt >= 0 && dt < MAX_SKILL )
		attack = skill_table[dt].noun_damage;
    else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]) )
		attack = attack_table[dt - TYPE_HIT];
    else
    {
		dt  = TYPE_HIT;
		attack  = attack_table[0];
    }

    if ( attack == "slash" || attack == "slice" )
    {
		damp=number_range(1,8);
		if ( damp == 1 )
		{
		    act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR);
			act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT);
		    act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"entrails");
		}
		else if ( damp == 2 )
		{
		    act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
			act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
		    act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 3 )
		{
			if (!IS_BODY(victim,CUT_THROAT))
				SET_BIT(victim->loc_hp[1],CUT_THROAT);
		    if (!IS_BLEEDING(victim,BLEEDING_THROAT))
				SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
		    act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR);
		    act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT);
			act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT);
			make_part(victim,"blood");
		}
		else if ( damp == 4 )
		{
		    if (!IS_BODY(victim,CUT_THROAT))
				SET_BIT(victim->loc_hp[1],CUT_THROAT);
		    if (!IS_BLEEDING(victim,BLEEDING_THROAT))
		    	SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
		    act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
		    act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
			act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"blood");
		}
		else if ( damp == 5 )
		{
		    if (!IS_HEAD(victim,BROKEN_SKULL))
		    {
				act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"brain");
				SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
			}
		    else
			{
				act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
				act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
		    }
		}
		else if ( damp == 6 )
		{
		    act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR);
		    act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT);
			act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 7 )
		{
			if (!IS_ARM_L(victim,LOST_ARM))
		    {
		    	act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
				act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
	    		act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"arm");
				SET_BIT(victim->loc_hp[2],LOST_ARM);
				if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
				    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
				if (IS_BLEEDING(victim,BLEEDING_HAND_L))
					REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
			}
			else if (!IS_ARM_R(victim,LOST_ARM))
		    {
		    	act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"arm");
				SET_BIT(victim->loc_hp[3],LOST_ARM);
				if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
				    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
				if (IS_BLEEDING(victim,BLEEDING_HAND_R))
				    REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
			}
			else
			{
				act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
				act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
			}
		}
		else if ( damp == 8 )
		{
		    if (!IS_LEG_L(victim,LOST_LEG))
		    {
				act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"leg");
				SET_BIT(victim->loc_hp[4],LOST_LEG);
				if (!IS_BLEEDING(victim,BLEEDING_LEG_L))
				    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
				if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
				    REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
			}
			else if (!IS_LEG_R(victim,LOST_LEG))
		    {
		    	act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
				act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
	    		act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"leg");
				SET_BIT(victim->loc_hp[5],LOST_LEG);
				if (!IS_BLEEDING(victim,BLEEDING_LEG_R))
				    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
				if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
					REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
			}
		    else
		    {
				act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
				act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
			}
		}
    }
    else if ( attack == "stab" || attack == "pierce" )
    {
		damp=number_range(1,5);
		if ( damp == 1 )
		{
		    act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR);
			act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT);
		    act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"blood");
		}
		else if ( damp == 2 )
		{
			act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
		    act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
		    act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 3 )
		{
		    act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR);
		    act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT);
			act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 4 )
		{
		    act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR);
			act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT);
		    act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 5 )
		{
		    act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR);
		    act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT);
			act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT);
		    if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
		    else if (!IS_HEAD(victim,LOST_EYE_R))
				SET_BIT(victim->loc_hp[0],LOST_EYE_R);
		    else if (!IS_HEAD(victim,LOST_EYE_L))
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
		}
    }
    else if ( attack == "blast" || attack == "pound" || attack == "crush" )
    {
		damp=number_range(1,3);
		bodyloc = 0;
		if ( damp == 1)
		{
		    act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR);
			act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT);
		    act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT);
		    if (IS_BODY(victim,BROKEN_RIBS_1 )) {bodyloc += 1;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
		    if (IS_BODY(victim,BROKEN_RIBS_2 )) {bodyloc += 2;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
		    if (IS_BODY(victim,BROKEN_RIBS_4 )) {bodyloc += 4;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
		    if (IS_BODY(victim,BROKEN_RIBS_8 )) {bodyloc += 8;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
		    if (IS_BODY(victim,BROKEN_RIBS_16)) {bodyloc += 16;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
		    bodyloc += number_range(1,3);
		    if (bodyloc > 24)
				bodyloc = 24;
			if (bodyloc >= 16)
			{
				bodyloc -= 16;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);
			}
		    if (bodyloc >= 8)
			{
				bodyloc -= 8;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);
			}
		    if (bodyloc >= 4)
			{
				bodyloc -= 4;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);
			}
		    if (bodyloc >= 2)
			{
				bodyloc -= 2;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);
			}
			if (bodyloc >= 1)
			{
				bodyloc -= 1;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);
			}
		}
		else if ( damp == 2)
		{
			act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_CHAR);
		    act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_NOTVICT);
		    act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, victim, TO_VICT);
			if (!IS_BODY(victim,BROKEN_SPINE))
				SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
		}
		else if ( damp == 3)
		{
		    if (!IS_HEAD(victim,BROKEN_SKULL))
		    {
				act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"brain");
				SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
		    }
		    else
		    {
				act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, TO_CHAR);
		    	act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, TO_VICT);
			}
		}
    }
    else if ( !IS_NPC( ch ) && (attack == "bite" ||IS_VAMPAFF(ch,VAM_FANGS)))
    {
		act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR);
		act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT);
		act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT);
		make_part(victim,"blood");
		if (!IS_BODY(victim,CUT_THROAT))
		    SET_BIT(victim->loc_hp[1],CUT_THROAT);
		if (!IS_BLEEDING(victim,BLEEDING_THROAT))
		    SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    else if ( !IS_NPC(ch) && (attack == "claw" || IS_VAMPAFF(ch,VAM_CLAWS)))
    {
		damp=number_range(1,2);
		if ( damp == 1 )
		{
		    act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
		    act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
			act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"blood");
		    if (!IS_BODY(victim,CUT_THROAT))
				SET_BIT(victim->loc_hp[1],CUT_THROAT);
		    if (!IS_BLEEDING(victim,BLEEDING_THROAT))
		    	SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
		}
		if ( damp == 2 )
		{
			if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
		    {
		    	act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
	    		act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
				act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"eyeball");
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
			}	
			else if (!IS_HEAD(victim,LOST_EYE_R))
		    {
		    	act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
				act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
	    		act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"eyeball");
				SET_BIT(victim->loc_hp[0],LOST_EYE_R);
			}
		    else if (!IS_HEAD(victim,LOST_EYE_L))
		    {
				act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
	    		act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"eyeball");
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
			}
			else
			{
		    	act("You claw open $N's chest.", ch, NULL, victim, TO_CHAR);
		    	act("$n claws open $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n claws open $N's chest.", ch, NULL, victim, TO_VICT);
			}
		}
    }
    else if ( attack == "whip" )
    {
		act("You entangle $N around the neck, and squeeze the life out of $S.", ch, NULL, victim, TO_CHAR);
		act("$n entangle $N around the neck, and squeezes the life out of $S.", ch, NULL, victim, TO_NOTVICT);
		act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, victim, TO_VICT);
		if (!IS_BODY(victim,BROKEN_NECK))
			SET_BIT(victim->loc_hp[1],BROKEN_NECK);
    }
    else if ( attack == "suck" || attack == "grep" )
    {
		act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_CHAR);
		act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_NOTVICT);
		act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, TO_VICT);
    }
    return;
}
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    char buf [MAX_STRING_LENGTH];

    /* I'm fed up of being disarmed every 10 seconds - KaVir */
    if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5)
		return;
    if (!IS_IMMUNE(ch, IMM_DISARM))
		return;
    if ( ( (obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON )
    {
		if ( ( (obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON )
			return;
    }
    sprintf(buf,"$n disarms you!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf, ch, NULL, victim, TO_VICT    );
    sprintf(buf,"You disarm $N!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_CHAR    );
    sprintf(buf,"$n disarms $N!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    obj_from_char( obj );

    /* Loyal weapons come back ;)  KaVir */
    if (IS_SET(obj->extra_flags, ITEM_LOYAL) && ( !IS_NPC(victim) ) )
    {
        act( "$p leaps back into your hand!", victim, obj, NULL, TO_CHAR );
        act( "$p leaps back into $n's hand!",  victim, obj, NULL, TO_ROOM );
		obj_to_char( obj, victim );
		do_wear( victim, obj->name);
    }
    else if ( IS_NPC(victim) )
		obj_to_char( obj, victim );
    else
		obj_to_room( obj, victim->in_room );
    return;
}
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];

    if (IS_AFFECTED(victim,AFF_FLYING))
		return;

    if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5)
		return;

    if (!IS_NPC(victim))
    {
		if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_FLYING))
			return;
		if (ch->power[DISC_DAEM_ATTA] >= 5)
			return;
		if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION))
			return;
		if ((IS_SET(ch->special, SPC_JEDI_LEVITATE)) && IS_CLASS(ch, CLASS_JEDI))
			return;
    }
    if ( victim->wait == 0)
    {
    	sprintf(buf,"$n trips you and you go down!");
    	ADD_COLOUR(ch,buf,WHITE);
    	act( buf, ch, NULL, victim, TO_VICT    );
    	sprintf(buf,"You trip $N and $E goes down!");
    	ADD_COLOUR(ch,buf,WHITE);
    	act( buf,  ch, NULL, victim, TO_CHAR    );
    	sprintf(buf,"$n trips $N and $E goes down!");
    	ADD_COLOUR(ch,buf,WHITE);
    	act( buf,  ch, NULL, victim, TO_NOTVICT );
		WAIT_STATE( ch,     2 * PULSE_VIOLENCE );
		WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
		victim->position = POS_RESTING;
    }
    return;
}


void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
  
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		send_to_char( "Kill whom?\n\r", ch );
		return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    if ( victim == ch )
    {
		send_to_char( "You cannot kill yourself!\n\r", ch );
		return;
    }

    if ( is_safe( ch, victim ) )
		return;
	if (IS_AFFECTED(victim,AFF_ETHEREAL) && !IS_AFFECTED(ch,AFF_ETHEREAL))
	{
		send_to_char("You cannot attack their vapoury form.\n\r", ch);
		return;
	}
    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
		act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
		return;
    }

    if ( ch->position == POS_FIGHTING )
    {
		send_to_char( "You do the best you can!\n\r", ch );
		return;
    }/*kinky
    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );*/
    check_killer( ch, victim );
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && number_range(1,3) == 1 && ch->power[DISC_WERE_BOAR] > 1 && victim->position == POS_STANDING)
    {
		act("You charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_CHAR);
		act("$n charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_NOTVICT);
		act("$n charge into you, knocking you from your feet.",ch,NULL,victim,TO_VICT);
		victim->position = POS_STUNNED;
		multi_hit( ch, victim, TYPE_UNDEFINED );
		multi_hit( ch, victim, TYPE_UNDEFINED );
		return;
    }
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}
void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		send_to_char( "Backstab whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    if ( victim == ch )
    {
		send_to_char( "How can you sneak up on yourself?\n\r", ch );
		return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11 ) && ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL
    ||   obj->value[3] != 11 ) )
    {
		send_to_char( "You need to wield a piercing weapon.\n\r", ch );
		return;
    }

    if ( victim->fighting != NULL )
    {
		send_to_char( "You can't backstab a fighting person.\n\r", ch );
		return;
    }

    if ( (victim->hit < victim->max_hit ) && !IS_CLASS(ch, CLASS_DROW) ) 
    {
		act( "$N is hurt and suspicious ... you can't sneak up.", ch, NULL, victim, TO_CHAR );
		return;
    }
    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if ( !IS_NPC(victim) && (IS_IMMUNE(victim, IMM_BACKSTAB) &&
	(!IS_CLASS(ch, CLASS_SHAPE) && !IS_CLASS(ch,CLASS_NINJA))))
		damage( ch, victim, 0, gsn_backstab );

    else if ( !IS_AWAKE(victim) ||  IS_NPC(ch) || number_percent( ) <
    ch->pcdata->learned[gsn_backstab] )
	{
                multi_hit( ch, victim, gsn_backstab );
                if (IS_CLASS(ch, CLASS_NINJA))
                {
                if (ch->max_move >= 20000)
                multi_hit( ch, victim, gsn_backstab );
                if (ch->max_move >= 30000)
                multi_hit( ch, victim, gsn_backstab );
                }
	}

    else
	{
		multi_hit( ch, victim, gsn_backstab );
		if (IS_CLASS(ch, CLASS_NINJA))
		{
		if (ch->max_move >= 20000)
		multi_hit( ch, victim, gsn_backstab );
		if (ch->max_move >= 30000)
		multi_hit( ch, victim, gsn_backstab );
		}
	}
    return;
}

void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];    
    int attempt;
if (IS_NPC(ch)) return;
if (ch->special & SPC_GOLEMBIND)
{stc("Your feet are too hard to flee away!\n\r", ch);
return;}
    if ( ( victim = ch->fighting ) == NULL )
    {
		if ( ch->position == POS_FIGHTING )
		    ch->position = POS_STANDING;
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    }
	if(ch->move <= 0)
	{
		send_to_char("You can't find the energy to leave.\n\r",ch);
		return;
	}

    if (IS_AFFECTED(ch, AFF_WEBBED))
	{
		send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
		WAIT_STATE(ch,12);
		return;
    }
	if (((IS_CLASS(victim, CLASS_VAMPIRE) &&
IS_SET(victim->newbits,NEW_COIL)) || check_poly(victim,
POLY_HUGE_SERPENT)) && number_percent( ) > 15)
	{
		sprintf(buf,"Not with %s coiled around you!",victim->name);
		send_to_char(buf,ch);
		act("$n cant escape with $N coiled around them.",ch,NULL,victim,TO_ROOM);
		WAIT_STATE(ch,12);
		return;
	}
	if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_SET(victim->newbits,NEW_JAWLOCK) && number_percent( ) > 10)
	{
		sprintf(buf,"Not with %s's jaws clamped on your neck!",victim->name);
		send_to_char(buf,ch);
		sprintf(buf, "%s cannot escape your clamped jaws!\r\n",ch->name);
		send_to_char(buf, ch);
		act("$n cant escape $N's clamped jaws!",ch,NULL,victim,TO_ROOM);
		WAIT_STATE(ch,12);
		return;
	}
    if (IS_AFFECTED(ch,AFF_TENDRILS))
	{
		send_to_char("You are unable to move while tangled in tendrils.\n\r",ch);
		WAIT_STATE(ch,12);
		return;
	}
    if (!IS_NPC(ch) && ch->rage >= 0)
    {
		if (IS_CLASS(ch,CLASS_VAMPIRE) && number_percent() <= ch->rage)
		{
			send_to_char("Your inner beast refuses to let you run!\n\r",ch);
		    WAIT_STATE(ch,12);
		    return;
		}
		else if (IS_CLASS(ch, CLASS_WEREWOLF) && number_percent() <= ch->rage * 0.3)
		{
		    send_to_char("Your rage is too great!\n\r",ch);
		    WAIT_STATE(ch,12);
			return;
		}
    }
    was_in = ch->in_room;
   if (number_range(1,2) == 1)
   {
    for ( attempt = 0; attempt < 6; attempt++ )
    {
		EXIT_DATA *pexit;
		int door;
		door = number_door( );
		if ( ( pexit = was_in->exit[door] ) == 0 ||   pexit->to_room == NULL
		||   IS_SET(pexit->exit_info, EX_CLOSED) || ( IS_NPC(ch) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
			continue;
		move_char( ch, door );
		if ( ( now_in = ch->in_room ) == was_in )
			continue;
        ch->in_room = was_in;
		act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
		ch->in_room = now_in;
		if ( !IS_NPC(ch) )
		    send_to_char( "You flee from combat!  Coward!\n\r", ch );
		WAIT_STATE(ch,12);
		stop_fighting( ch, TRUE );
		return;
    }
    send_to_char("You cannot escape!\r\n", ch);
    WAIT_STATE(ch, 12);
    return;
   }
   else
   {
	WAIT_STATE(ch,12);
    send_to_char( "You were unable to escape!\n\r", ch );
   }
    return;
}

/*bool check_huge_serp( CHAR_DATA *ch)
{
        for ( paf = ch->affected; paf != NULL; paf = paf->next )
        {
            if ( paf->modifier == 2048 )
            {
                    return TRUE;
            }
        }
}*/

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
		send_to_char( "Rescue whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    if ( victim == ch )
    {
		send_to_char( "What about fleeing instead?\n\r", ch );
		return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
		send_to_char( "Doesn't need your help!\n\r", ch );
		return;
    }

    if ( ch->fighting == victim )
    {
		send_to_char( "Too late.\n\r", ch );
		return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
		send_to_char( "That person is not fighting right now.\n\r", ch );
		return;
    }

    if ( is_safe(ch, fch) || is_safe(ch, victim) )
		return;
    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
    {
		send_to_char( "You fail the rescue.\n\r", ch );
		return;
    }
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );
    check_killer( ch, fch );
    set_fighting( ch, fch );
    set_fighting( fch, ch );
    do_humanity(ch,"");
    return;
}
void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *boots;
    int dam;
    int stance;
    int hitpoints;

    if ( !IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
    {
		send_to_char("First you should learn to kick.\n\r", ch );
		return;
    }
    if ( ( victim = ch->fighting ) == NULL )
    {
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    }
    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_kick] )
    	dam = number_range(1,4);
    else
    {
    	dam = 0;
    	damage( ch, victim, dam, gsn_kick );
		return;
    }
    dam += char_damroll(ch);
    if (dam == 0)
		dam = 1;
    if ( !IS_AWAKE(victim) )
		dam *= 2;
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_POTE] > 0)
    {
	if (ch->power[DISC_VAMP_POTE] >= 10)
	    dam *= 1.8;
	else if (ch->power[DISC_VAMP_POTE] >= 7)
	    dam *= 1.7;
	else if (ch->power[DISC_VAMP_POTE] >= 5)
	    dam *= 1.6;
	else if (ch->power[DISC_VAMP_POTE] >= 3)
	    dam *= 1.5;
    }
    else if (IS_CLASS(ch, CLASS_DEMON) && ch->power[DISC_DAEM_ATTA] > 0)
    {
	if (ch->power[DISC_DAEM_ATTA] >= 8)
	    dam *= 1.7;
	else if (ch->power[DISC_DAEM_ATTA] >= 5)
	    dam *= 1.6;
	else if (ch->power[DISC_DAEM_ATTA] >= 3)
	    dam *= 1.5;
    }
    if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
    {
		if (IS_SET(victim->special, SPC_WOLFMAN))
			dam *= 0.5;
		if (victim->power[DISC_WERE_BOAR] > 2 )
			dam *= 0.5;
		if ( ( boots = get_eq_char( ch, WEAR_FEET ) ) != NULL && IS_SET(boots->spectype, SITEM_SILVER))
			dam *= 2;
    }
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF))
    {
		if (ch->power[DISC_WERE_LUNA] > 6)
		{
			if (weather_info.sunlight == SUN_DARK)
   		         dam *= 1.1;
		}
    }    

    /* Vampires should be tougher at night and weaker during the day. */
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) )
    {
		if (weather_info.sunlight == SUN_LIGHT && dam > 1)
		    dam /= 2;
		else if (weather_info.sunlight == SUN_DARK)
			dam *= 1.5;
    }
    if ( !IS_NPC(ch) )
		dam = dam + (dam * ((ch->wpn[0]+1) / 100));

    if ( !IS_NPC(ch) )
    {
		stance = ch->stance[0];
		if ( IS_STANCE(ch, STANCE_NORMAL) )
			dam *= 1.25;
		else 
			dam = dambonus(ch,victim,dam,stance);
    }

    if ( !IS_NPC(victim) && dam > 750 )
		dam = 750;
    if ( dam <= 0 )
		dam = 2;

    hitpoints = victim->hit;
    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_KICK))
    {
		victim->hit += dam;
		damage( ch, victim, dam, gsn_kick );
		victim->hit = hitpoints;
    }
    else damage( ch, victim, dam, gsn_kick );
	if (!IS_IMMUNE(victim, IMM_KICK))
	{
	if (will_use(ch, "Enhanced Kick")) {dam /= 1.3;damage(ch, victim, dam, gsn_kick);}
	if (will_use(ch, "Enhanced Kick")) {dam /= 1.6;damage(ch, victim, dam, gsn_kick);}
	if (will_use(ch, "Enhanced Kick")) {dam /= 1.9;damage(ch, victim, dam, gsn_kick);}
	}
    return;
}
int dambonus( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance)
{
    if (dam < 1)
		return 0;
    if (stance < 1)
		return dam;
if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MONKEY))
dam *= .4;
    if (!IS_NPC(ch) && !can_counter(victim))
    {
		if ( IS_STANCE(ch, STANCE_MONKEY) )
		{
			int mindam = dam * 0.25;
			dam *= (ch->stance[STANCE_MONKEY]+1) / 200;
			if (dam < mindam)
				dam = mindam;
		}
		else if ( IS_STANCE(ch, STANCE_BULL) && ch->stance[STANCE_BULL] > 100 )
			dam += dam * (ch->stance[STANCE_BULL] / 100);
		else if ( IS_STANCE(ch, STANCE_DRAGON) && ch->stance[STANCE_DRAGON] > 100 )
			dam += dam * (ch->stance[STANCE_DRAGON] / 100);
		else if ( IS_STANCE(ch, STANCE_WOLF) && ch->stance[STANCE_WOLF] > 100)
			dam += dam*(ch->stance[STANCE_WOLF] /100);
		else if ( IS_STANCE(ch, STANCE_TIGER) && ch->stance[STANCE_TIGER] > 100 )
			dam += dam * (ch->stance[STANCE_TIGER] / 100);
		else if ( ch->stance[0] > 0 && ch->stance[stance] < 100 )
			dam *= 0.5;
    }
    if (!IS_NPC(victim) && !can_counter(ch))
    {
		if ( IS_STANCE(victim, STANCE_CRAB) && victim->stance[STANCE_CRAB] > 100 )
	    dam /= victim->stance[STANCE_CRAB]/100;
        else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100)
            dam /= victim->stance[STANCE_DRAGON]/100;
		else if ( IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 100 )
			dam /= victim->stance[STANCE_SWALLOW]/100;
    }
    return dam;
}
void do_punch( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
    int store;
    bool broke = FALSE;

    one_argument(argument,arg);

    if ( IS_NPC(ch) )
		return;

    if ( ch->level < skill_table[gsn_punch].skill_level)
    {
		send_to_char( "First you should learn to punch.\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    }

    if ( ch == victim )
    {
		send_to_char( "You cannot punch yourself!\n\r", ch );
		return;
    }

    if (is_safe(ch,victim))
		return;

    if ( victim->hit < victim->max_hit )
    {
		send_to_char( "They are hurt and suspicious.\n\r", ch );
		return;
    }

    if ( victim->position < POS_FIGHTING )
    {
		send_to_char( "You can only punch someone who is standing.\n\r", ch );
		return;
    }

    act("You draw your fist back and aim a punch at $N.",ch,NULL,victim,TO_CHAR);
    act("$n draws $s fist back and aims a punch at you.",ch,NULL,victim,TO_VICT);
    act("$n draws $s fist back and aims a punch at $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, skill_table[gsn_punch].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_punch] )
    	dam = number_range(1,4);
    else
    {
    	dam = 0;
    	damage( ch, victim, dam, gsn_punch );
	check_killer( ch, victim );
		return;
    }

    dam += char_damroll(ch);
    if (dam == 0)
		dam = 1;
    if ( !IS_AWAKE(victim) )
		dam *= 2;
    if ( !IS_NPC(ch) )
		dam = dam + (dam * (ch->wpn[0] / 100));
    if ( dam <= 0 )
		dam = 1;

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BOAR] > 3)
    {
		store = victim->hit;
		victim->hit += dam;
		damage( ch, victim, dam, gsn_punch );
		victim->hit = store;
		if (number_percent() <= 25 && !IS_ARM_L(ch,LOST_ARM) && !IS_ARM_L(ch,LOST_HAND))
		{
			if (!IS_ARM_L(ch, LOST_FINGER_I) && !IS_ARM_L(ch, BROKEN_FINGER_I))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
				broke = TRUE;
			}
			if (!IS_ARM_L(ch, LOST_FINGER_M) && !IS_ARM_L(ch, BROKEN_FINGER_M))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
				broke = TRUE;
			}
			if (!IS_ARM_L(ch, LOST_FINGER_R) && !IS_ARM_L(ch, BROKEN_FINGER_R))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
				broke = TRUE;
			}
			if (!IS_ARM_L(ch, LOST_FINGER_L) && !IS_ARM_L(ch, BROKEN_FINGER_L))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
				broke = TRUE;
			}
			if (broke)
			{
				act("The fingers on your left hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
				act("The fingers on $n's left hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
			}
		}
		else if (number_percent() <= 25 && !IS_ARM_R(ch,LOST_ARM) && !IS_ARM_R(ch,LOST_HAND))
		{
			if (!IS_ARM_R(ch, LOST_FINGER_I) && !IS_ARM_R(ch, BROKEN_FINGER_I))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
				broke = TRUE;
			}
			if (!IS_ARM_R(ch, LOST_FINGER_M) && !IS_ARM_R(ch, BROKEN_FINGER_M))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
				broke = TRUE;
			}
			if (!IS_ARM_R(ch, LOST_FINGER_R) && !IS_ARM_R(ch, BROKEN_FINGER_R))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
				broke = TRUE;
			}
			if (!IS_ARM_R(ch, LOST_FINGER_L) && !IS_ARM_R(ch, BROKEN_FINGER_L))
			{
				SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
				broke = TRUE;
			}
			if (broke)
			{
				act("The fingers on your right hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
				act("The fingers on $n's right hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
			}
		}
		stop_fighting(victim,TRUE);
		return;
	}
    damage( ch, victim, dam, gsn_punch );
    if (victim == NULL || victim->position == POS_DEAD || dam < 1)
		return;
    if (victim->position == POS_FIGHTING)
		stop_fighting(victim,TRUE);
    if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
    {
    	act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    	act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
		SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_JAW))
    {
    	act("Your jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    	act("$n's jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
		SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
    act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
    victim->position = POS_STUNNED;
    return;
}
void do_berserk( CHAR_DATA *ch, char *argument )
{
	char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    int number_hit = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( ch->level < skill_table[gsn_berserk].skill_level)
    {
		send_to_char( "You are not wild enough to go berserk.\n\r", ch );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_berserk].beats );
    if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
    {
    	act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    	act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
	return;
    }

    act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
    act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
		vch_next	= vch->next;
		if (number_hit > 6)
			continue;
if (is_same_group(ch, vch)) continue;
		if ( vch->in_room == NULL )
			continue;
		if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
			continue;
		if ( ch == vch )
		    continue;
		if ( vch->in_room == ch->in_room )
		{
			if ( ( mount = ch->mount ) != NULL )
			{
				if ( mount == vch )
					continue;
			}
			if (can_see(ch,vch))
			{
				multi_hit( ch, vch, TYPE_UNDEFINED );
                number_hit++;	        
		    }
		}
    }
    do_beastlike(ch,"");
    return;
}
void do_hurl( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    CHAR_DATA       *mount;
    ROOM_INDEX_DATA *to_room;
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

    if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1 )
    {
		send_to_char("Maybe you should learn the skill first?\n\r",ch);
		return;
    }

    if ( arg1[0] == '\0' )
    {
		send_to_char("Who do you wish to hurl?\n\r", ch);
		return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
		send_to_char("They are not here.\n\r", ch);
		return;
    }

    if ( victim == ch )
    {
		send_to_char( "How can you hurl yourself?\n\r", ch );
		return;
    }

    if ( is_safe( ch, victim ) )
		return;

    if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
    {
		send_to_char("But they have someone on their back!\n\r",ch);
		return;
    }
    else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
		send_to_char("But they are riding!\n\r",ch);
		return;
    }

    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
    {
		send_to_char("You are unable to get their feet of the ground.\n\r",ch);
		return;
    }

    if ( IS_NPC(victim) && victim->level > 900 )
    {
		send_to_char("You are unable to get their feet of the ground.\n\r",ch);
		return;
    }

    if ( (victim->hit < victim->max_hit) || ( victim->position == POS_FIGHTING && victim->fighting != ch) )
    {
		act( "$N is hurt and suspicious, and you are unable to approach $M.",
	    ch, NULL, victim, TO_CHAR );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_hurl].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_hurl] )
    {
	    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
	    multi_hit( victim, ch, TYPE_UNDEFINED );
	    return;
    }

    rev_dir = 0;

    if ( arg2[0] == '\0' )
	door = number_range(0,3);
    else
    {
    	if      ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "north" ) )
			door = 0;
    	else if ( !str_cmp( arg2, "e" ) || !str_cmp( arg2, "east" ) )
			door = 1;
    	else if ( !str_cmp( arg2, "s" ) || !str_cmp( arg2, "south" ) )
			door = 2;
    	else if ( !str_cmp( arg2, "w" ) || !str_cmp( arg2, "west" ) )
			door = 3;
    	else
    	{
			send_to_char("You can only hurl people north, south, east or west.\n\r", ch);
			return;
    	}
    }

    if (door == 0)
	{
		sprintf(direction,"north");
		rev_dir = 2;
	}
    if (door == 1)
	{
		sprintf(direction,"east");
		rev_dir = 3;
	}
    if (door == 2)
	{
		sprintf(direction,"south");
		rev_dir = 0;
	}
    if (door == 3) 
	{
		sprintf(direction,"west");
		rev_dir = 1;
	}

    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
		sprintf(buf,"$n hurls $N into the %s wall.", direction);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		sprintf(buf,"You hurl $N into the %s wall.", direction);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n hurls you into the %s wall.", direction);
		act(buf,ch,NULL,victim,TO_VICT);
		dam = number_range(ch->level, (ch->level * 4));
		victim->hit = victim->hit - dam;
		update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch))
			ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch))
			victim->mdeath = victim->mdeath + 1;
		if (victim->position == POS_DEAD)
		{
			raw_kill(victim);
			return;
		}
		return;
    }
    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim,AFF_PASS_DOOR ) && !IS_AFFECTED(victim,AFF_ETHEREAL) )
    {
		if (IS_SET(pexit->exit_info, EX_LOCKED))
		    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		if (IS_SET(pexit->exit_info, EX_CLOSED))
		    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
		sprintf(buf,"$n hoists $N in the air and hurls $M %s.", direction);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		sprintf(buf,"You hoist $N in the air and hurl $M %s.", direction);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n hurls you %s, smashing you through the %s.", direction,pexit->keyword);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"There is a loud crash as $n smashes through the $d.");
		act(buf,victim,NULL,pexit->keyword,TO_ROOM);
		if ( ( to_room   = pexit->to_room  ) != NULL &&  ( pexit_rev = to_room->exit[rev_dir] ) != NULL
		&&   pexit_rev->to_room == ch->in_room &&   pexit_rev->keyword != NULL )
		{
			if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
				REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
		    if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
			REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
		    if (door == 0)
				sprintf(direction,"south");
			if (door == 1)
				sprintf(direction,"west");
		    if (door == 2)
				sprintf(direction,"north");
		    if (door == 3)
				sprintf(direction,"east");
			char_from_room(victim);
		    char_to_room(victim,to_room);
			sprintf(buf,"$n comes smashing in through the %s $d.", direction);
		    act(buf,victim,NULL,pexit->keyword,TO_ROOM);
		    dam = number_range(ch->level, (ch->level * 6));
			victim->hit = victim->hit - dam;
		    update_pos(victim);
    	    if (IS_NPC(victim) && !IS_NPC(ch))
				ch->mkill = ch->mkill + 1;
    	    if (!IS_NPC(victim) && IS_NPC(ch))
				victim->mdeath = victim->mdeath + 1;
			if (victim->position == POS_DEAD)
			{
				raw_kill(victim);
				return;
			}
		}
    }
    else
    {
		sprintf(buf,"$n hurls $N %s.", direction);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		sprintf(buf,"You hurl $N %s.", direction);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n hurls you %s.", direction);
		act(buf,ch,NULL,victim,TO_VICT);
		if (door == 0)
			sprintf(direction,"south");
		if (door == 1)
			sprintf(direction,"west");
		if (door == 2)	
			sprintf(direction,"north");
		if (door == 3)
			sprintf(direction,"east");
		char_from_room(victim);
		char_to_room(victim,to_room);
		sprintf(buf,"$n comes flying in from the %s.", direction);
		act(buf,victim,NULL,NULL,TO_ROOM);
		dam = number_range(ch->level, (ch->level * 2));
		victim->hit = victim->hit - dam;
		update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch))
			ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch))
			victim->mdeath = victim->mdeath + 1;
		if (victim->position == POS_DEAD)
		{
			raw_kill(victim);
			return;
		}
    }
    return;
}
void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    if ( !IS_NPC(ch) && ch->level < skill_table[gsn_disarm].skill_level)
    {
		send_to_char( "You don't know how to disarm opponents.\n\r", ch );
		return;
    }

    if ( ( get_eq_char( ch, WEAR_WIELD ) == NULL ) && ( get_eq_char( ch, WEAR_HOLD  ) == NULL ) )
    {
		send_to_char( "You must wield a weapon to disarm.\n\r", ch );
		return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    }

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )  &&   ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL ) )
    {
		send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + victim->level - ch->level;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
		send_to_char( "You failed.\n\r", ch );
    else if ( IS_NPC(ch) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
		disarm( ch, victim );
    else
		send_to_char( "You failed.\n\r", ch );
    return;
}
void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}
void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
		send_to_char( "Slay whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    if ( ch == victim )
    {
		send_to_char( "Suicide is a mortal sin.\n\r", ch );
		return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && !is_pure("slay",ch->immpower))
    {
		send_to_char( "You failed.\n\r", ch );
		return;
    }

    if ( !IS_NPC(victim) && victim->level >= ch->level )
    {
if (!is_pure("slay",ch->immpower)) {
		send_to_char( "You failed.\n\r", ch );
		return;}
    }

    act( "You slay $M in cold blood!",  ch, NULL, victim, TO_CHAR    );
    act( "$n slays you in cold blood!", ch, NULL, victim, TO_VICT    );
    act( "$n slays $N in cold blood!",  ch, NULL, victim, TO_NOTVICT );
    raw_kill( victim );
    return;
}
void do_killperson( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
		return;
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
		return;
    send_to_char( "You have been KILLED!!\n\r\n\r", victim );
    if (IS_NPC(victim) && !IS_NPC(ch))
		ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch))
		victim->mdeath = victim->mdeath + 1;
    raw_kill( victim );
    return;
}
void do_stealsoul( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot do this whilst you are in the arena.\n\r", ch);
		return;
	}

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
		stc("Huh?\n\r",ch);
		return;
    }

    if ( arg[0] == '\0' )
    {
		send_to_char( "Who's soul are you going to steal?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    if ( IS_CLASS(victim, CLASS_DEMON) )
    {
        stc("Demons don't have a soul!\n\r",ch);
        return;
    }

    if ( ch == victim )
    {
		send_to_char( "That might be a bit tricky...\n\r", ch );
		return;
    }

    if ( IS_NPC(victim) )
    {
		send_to_char( "You can only steal the soul of other players.\n\r", ch );
		return;
    }

    if ( ch->level < 3 )
    {
		send_to_char( "You must be an avatar to steal someone's soul.\n\r", ch );
		return;
    }

	if ((victim->level < 3) || (victim->level > 7))
    {
		send_to_char( "You can only steal the soul of other Avatars.\n\r", ch );
		return;
    }

    if ( victim->position > 1 )
    {
		send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
		return;
    }

    if (is_safe(ch,victim))
		return;
    act( "You tear $N's soul out of their body!",  ch, NULL, victim, TO_CHAR    );
    send_to_char( "Your soul is torn from your body!\n\r", victim);
    act( "$n tears $N's soul out of $M lifeless body!",  ch, NULL, victim, TO_NOTVICT );
    act( "$n's soul is torn from $m body!", victim, NULL, NULL, TO_ROOM);
    
	if( (ch->race > 0) && ((victim->race < 1) && (victim->pkill < 2)) && victim->max_hit < 15000)
    {
		ch->paradox[0] += ch->race;
		ch->paradox[1] += ch->race;
		ch->paradox[2] += PARADOX_TICK;
    	sprintf(buf,"%s's has been stolen by %s.",victim->name,ch->name);
    	do_info(ch,buf);
		sprintf( log_buf, "%s soulstealed by %s at %d for no status.",
	    victim->name, ch->name, victim->in_room->vnum );
		log_string( log_buf );
		do_paradox(ch,"self");
		if (IS_CLASS(victim, CLASS_VAMPIRE))
			do_mortalvamp(victim,"");
		else if (IS_SET(victim->special, SPC_WOLFMAN))
			do_unwerewolf(victim,"");
		if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
			do_unpolymorph(victim,"");
		if (IS_CLASS(victim, CLASS_DROW) && IS_POLYAFF(victim, POLY_SPIDER))
			do_spiderform(victim,"");
		if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_POLYAFF(victim, POLY_ZULOFORM))
		{
			if (IS_EXTRA(victim, EXTRA_DRAGON))
			{
				REMOVE_BIT(victim->extra,EXTRA_DRAGON);
				act( "You transform back into zuloform.", victim, NULL, NULL, TO_CHAR );
			    act( "$n shrinks back into a big black monster.", victim, NULL, NULL, TO_ROOM );

			    victim->damroll = victim->damroll - 25;
				victim->hitroll = victim->hitroll - 25;
				free_string( victim->morph );
				victim->morph = str_dup( "A big black monster" );
			}
	        REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
		    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	        act( "You transform into human form.", victim, NULL, NULL, TO_CHAR );
		    act( "$n's shrinks back into human form.", victim, NULL, NULL, TO_ROOM );
			victim->damroll = victim->damroll - 50;
	        victim->hitroll = victim->hitroll - 50; 
		    free_string( victim->morph );
			victim->morph = str_dup( "" );
		}
		if (IS_EXTRA(victim,EXTRA_EARTHMELD) )
		{
			REMOVE_BIT(victim->affected_by, AFF_SHIFT);
		    REMOVE_BIT(victim->extra, EXTRA_EARTHMELD);
			REMOVE_BIT(victim->act, PLR_WIZINVIS);
		}

		behead( victim );
		do_beastlike(ch,"");
		ch->pkill = ch->pkill + 1;
		victim->pdeath = victim->pdeath + 1;
		victim->level = 2;
		return;
    }
    
    if (ch->race == 0) ch->race++;
        ch->pcdata->souls++;
    if (ch->pcdata->souls >= souls_needed(ch))
        stc("You have achieved the required souls for a promotion!\n\r",ch);
    if (IS_CLASS(victim, CLASS_VAMPIRE))
		do_mortalvamp(victim,"");
    else if (IS_SET(victim->special, SPC_WOLFMAN))
		do_unwerewolf(victim,"");
    if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
		do_unpolymorph(victim,"");
    if (IS_CLASS(victim, CLASS_DROW) && IS_POLYAFF(victim, POLY_SPIDER))
		do_spiderform(victim,"");
	behead( victim );
    do_beastlike(ch,"");
    ch->pkill = ch->pkill + 1;
    victim->pdeath = victim->pdeath + 1;
    victim->pcdata->stats[UNI_RAGE] = 0;
    victim->level = 2;
    sprintf(buf,"%s's soul has been stolen by %s.",victim->name,ch->name);
    ch->exp += victim->exp / 2;
    victim->exp /= 2;
    do_info(ch,buf);
    sprintf( log_buf, "%s soulstolen by %s at %d.",
	victim->name, ch->name, victim->in_room->vnum );
    log_string( log_buf );
    return;
}
/* For decapitating players - KaVir */
void do_decapitate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    OBJ_DATA  *obj2;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int gained_status = 0;

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
		return;
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot do this whilst you are in the arena.\n\r", ch);
		return;
	}
    obj2 = get_eq_char( ch, WEAR_HOLD );
    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL && !IS_CLASS(ch, CLASS_MONK))
    {
    	if ( ( obj2 = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    	{
		    send_to_char( "First you better get a weapon out!\n\r", ch );
		    return;
        }
    }

    if ( ( obj != NULL && obj->item_type != ITEM_WEAPON ) && !IS_CLASS(ch, CLASS_MONK))
    {
    	if ( ( obj2 != NULL && obj2->item_type != ITEM_WEAPON ) )
    	{
			send_to_char( "But you are not wielding any weapons!\n\r", ch );
			return;
        }
    }

    if ( ( obj  != NULL && obj->value[3]  != 1 && obj->value[3]  != 3 )
	&& ( obj2 != NULL && obj2->value[3] != 1 && obj2->value[3] != 3 )
&& !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_MECHA)  &&
 !IS_CLASS(ch, CLASS_NINJA) 
 && !IS_CLASS(ch, CLASS_AMAZON)) 
    { if (!IS_CLASS(ch, CLASS_SHAPE)) {
		if (IS_CLASS(ch, CLASS_DROW))
		{
			if (( obj != NULL && obj->value[3] != 4) && (obj2 != NULL && obj2->value[3] != 4))
			{
				send_to_char("You need to wield a slashing, slicing, or whipping weapon to decapitate.\n\r", ch );
				return;
			}
		}
		else
		{
			send_to_char( "You need to wield a slashing or slicing weapon to decapitate.\n\r", ch );
			return;
		}
    }}

    if ( arg[0] == '\0' )
    {
		send_to_char( "Decapitate whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    if ( ch == victim )
    {
		send_to_char( "That might be a bit tricky...\n\r", ch );
		return;
    }

    if ( IS_NPC(victim) )
    {
		send_to_char( "You can only decapitate other players.\n\r", ch );
		return;
    }

	if (ch->level < 3)
    {
		send_to_char( "You must be an avatar to decapitate someone.\n\r", ch );
		return;
    }

	if (victim->level < 3)
    {
		send_to_char( "You can only decapitate other avatars.\n\r", ch );
		return;
    }

    if ( victim->position > 1 )
    {
		send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
		return;
    }

    if (is_safe(ch,victim))
		return;

    if ( IS_CLASS(ch, CLASS_VAMPIRE) && IS_SET(ch->special, SPC_INCONNU) && (IS_CLASS(victim, CLASS_VAMPIRE) || IS_VAMPAFF(victim, VAM_MORTAL)))
    {
		send_to_char( "You cannot decapitate another vampire.\n\r", ch );
		return;
    }

    if ( IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special, SPC_INCONNU) && (IS_CLASS(ch, CLASS_VAMPIRE) || IS_VAMPAFF(ch, VAM_MORTAL)))
    {
		send_to_char( "You cannot decapitate an Inconnu vampire.\n\r", ch );
		return;
    }

    if ( IS_CLASS(ch, CLASS_VAMPIRE) && IS_CLASS(victim, CLASS_VAMPIRE) && strlen(ch->clan) > 1 && strlen(victim->clan) > 1)
    {
		if (!str_cmp(ch->clan,victim->clan))
		{
			send_to_char( "You cannot decapitate someone of your own clan.\n\r", ch );
			return;
		}
    }
	if (!IS_CLASS(ch, CLASS_MONK))
	{
		act( "You bring your weapon down upon $N's neck!",  ch, NULL, victim, TO_CHAR    );
		send_to_char( "Your head is sliced from your shoulders!\n\r", victim);
		act( "$n swings $s weapon down towards $N's neck!",  ch, NULL, victim, TO_NOTVICT );
		act( "$n's head is sliced from $s shoulders!", victim, NULL, NULL, TO_ROOM);
	}
	else
	{
		act( "You place the palm of your hand on $N's head and tear it clean from their shoulders!",  ch, NULL, victim, TO_CHAR    );
		send_to_char( "Your head has been ripped from your shoulders!\n\r", victim);
		act( "$n tears $N's head off!",  ch, NULL, victim, TO_NOTVICT );
		act( "$n's head is ripped from $s shoulders!", victim, NULL, NULL, TO_ROOM);
	}

    if (IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(victim, CLASS_DEMON) && !IS_CLASS(victim, CLASS_DROW))
	{
		if (victim->race == 0 && ch->race == 0 )
			ch->pcdata->stats[DROW_POWER] += 2000;
		else 
			ch->pcdata->stats[DROW_POWER] += 2000* victim->race;
	}
    /* the set to work out if you have just capped someone for no status */
	if( (ch->race > 0) && ((victim->race < 1) && (victim->pkill < 2)) && victim->max_hit < 15000)
    {
		ch->paradox[0] += ch->race;
		ch->paradox[1] += ch->race;
		ch->paradox[2] += PARADOX_TICK;
		if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
			sprintf(buf,"%s Screams in Unearthly Pain as %s latches onto him #7Fangs #nBared as he Sucks the #1Blood#n from the #7Pale Faced #0corpse ", victim->pcdata->switchname, ch->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MIND))
		{
			sprintf (buf, "%s hisses loudly as they wrap thier tentacles around %s's head and #1rips#n out thier still #6pulsing #5brain#n.",ch->pcdata->switchname, victim->pcdata->switchname);
/*			sprintf (buf, "%s wraps thier tentacles around
%s's head and #1rips#n our thier #5brain#n.",ch->pcdata->switchname,
victim->pcdata->switchname);*/
		}
		else if (IS_CLASS(ch, CLASS_WEREWOLF))
		{
			sprintf(buf,"%s howls in #1rage #nas he rips %s apart limb from limb.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MAGE))
		{
			sprintf(buf,"#6%s raises his arms #4chanting #6loudly as %s's head #1explodes #6everywhere#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MONK))
		{
sprintf(buf,"%s calls on the #7Power of God#n to destroy %s mortal body.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_DROW))
		{
sprintf(buf,"#1%s sac#0rifi#1ces %s's s#0ou#1l to L#0lot#1h#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_NINJA))
		{
sprintf(buf,"%s Plunges his #1Blood Soaked #nDagger into %s's Back, pushing it so hard that it Bores through the ribcage pushing out the #1heart #nto the floor ", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_BERSERK))
{
sprintf(buf, "%s %sIM TO TIRED TO CODE THIS FUCKING THING IN", ch->pcdata->switchname,
victim->pcdata->switchname);
}
 else if (IS_CLASS(ch, CLASS_MECHA))
                {
sprintf(buf,"#3You see Bright balls of lights and explosions everywhere as #1%s #2screams #3'#1MASSATSU RATTO!!!#3'#n #3yanking its #4vibro#1 blade #3down charging past #2%s#n. #5A moment later #1\\#7|#1/#n #3they split in half#n.", ch->pcdata->switchname, victim->pcdata->switchname);
                }
                
		else if (IS_CLASS(ch, CLASS_DEMON))
		{
			sprintf(buf,"%s tears %s's soul from this mortal realm", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_HIGHLANDER))
		{
			sprintf(buf,"%s Falls onto the floor in convulsions as %s Decapitates %s and in a #5Purple hazy #7mist#n #4Electricity #nhums around and rushes violently through there body", victim->pcdata->switchname, ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_AMAZON))
			sprintf(buf,"#6%s #6p#4ray#6s to Ar#4tem#6is to cl#4ean#6se t#4h#6e im#4puri#6ty in %s#n.",
				ch->pcdata->switchname, victim->pcdata->switchname);
		else if (IS_CLASS(ch, CLASS_SHAPE))
			sprintf(buf,"#5%s #2sh#3ifts #4in#2to #6the #1Decapitator of Doom,#5 and slices off %s's head.", ch->name, victim->name);
 		else if (IS_CLASS(ch, CLASS_JEDI))
			sprintf(buf,"%s Drops to his knees and Practically melts as %s's #2lightsabre#n Is Thrusted Between the lazer charred Corpse.", victim->pcdata->switchname, ch->pcdata->switchname);
		else 
			sprintf(buf,"%s has been decapitated by %s.",victim->pcdata->switchname,ch->pcdata->switchname);


   		do_info(ch,buf);
		sprintf( log_buf, "%s decapitated by %s at %d for no status.",
		victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
		log_string( log_buf );
		do_paradox(ch,"self");

		if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_POLYAFF(victim, POLY_ZULOFORM))
		{
			if (IS_EXTRA(victim, EXTRA_DRAGON))
			{
				REMOVE_BIT(victim->extra,EXTRA_DRAGON);
				act( "You transform back into zuloform.", victim, NULL, NULL, TO_CHAR );
			    act( "$n shrinks back into a big black monster.", victim, NULL, NULL, TO_ROOM );
			    victim->damroll = victim->damroll - 25;
				victim->hitroll = victim->hitroll - 25;
				free_string( victim->morph );
				victim->morph = str_dup( "A big black monster" );
			}
	        	REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
		    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	        act( "You transform into human form.", victim, NULL, NULL, TO_CHAR );
		    act( "$n's shrinks back into human form.", victim, NULL, NULL, TO_ROOM );
			victim->damroll = victim->damroll - 50;
	        victim->hitroll = victim->hitroll - 50; 
		    free_string( victim->morph );
			victim->morph = str_dup( "" );
		}
		if (IS_EXTRA(victim,EXTRA_EARTHMELD) )
		{
			REMOVE_BIT(victim->affected_by, AFF_SHIFT);
		    REMOVE_BIT(victim->extra, EXTRA_EARTHMELD);
			REMOVE_BIT(victim->act, PLR_WIZINVIS);
		}
		/* End */
		if (IS_CLASS(victim, CLASS_VAMPIRE))
			do_mortalvamp(victim,"");
		else if (IS_SET(victim->special, SPC_WOLFMAN))
			do_unwerewolf(victim,"");
		if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
			do_unpolymorph(victim,"");
		if (IS_CLASS(victim, CLASS_DROW) && victim->cur_form == FRM_DROWSPIDER)
			do_spiderform(victim,"");
		ch->fight_timer += 10;
		behead( victim );
		do_beastlike(ch,"");
		ch->pkill = ch->pkill + 1;
		victim->pdeath = victim->pdeath + 1;
		victim->level = 2;
		victim->fight_timer = 0;
	
return;
	}/*end of if( (ch->race > 0) && ((victim->race < 1) && (victim->pkill < 2)) )*/

    ch->exp = ch->exp + 1000;

    /* This means an avatar will always gain status */
    if (ch->race == 0)
    {
    	ch->race = ch->race + 1;
		gained_status = 1;
	if ( victim->race > 0)
	    victim->race--;
    }
	 /* But if they have status, only gain status up to 50 and only if their
       victim had at least one status */
    else if ((ch->race < 60) && (ch->race - victim->race <= 10)) 
    {
		gained_status = 1;
		ch->race = ch->race + 1;
	if( victim->race > 0)
	    victim->race--;
    }

    /* and this loses status, but only if they had more than 0 */
    if (victim->race == 0)
    	victim->race = victim->race;

    act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, TO_CHAR    );
    act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, TO_NOTVICT );
    act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, TO_NOTVICT );
		if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_POLYAFF(victim, POLY_ZULOFORM))
		{
			if (IS_EXTRA(victim, EXTRA_DRAGON))
			{
				REMOVE_BIT(victim->extra,EXTRA_DRAGON);
				act( "You transform back into zuloform.", victim, NULL, NULL, TO_CHAR );
			    act( "$n shrinks back into a big black monster.", victim, NULL, NULL, TO_ROOM );
			    victim->damroll = victim->damroll - 25;
				victim->hitroll = victim->hitroll - 25;
				free_string( victim->morph );
				victim->morph = str_dup( "A big black monster" );
			}
	        REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
		    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	        act( "You transform into human form.", victim, NULL, NULL, TO_CHAR );
		    act( "$n's shrinks back into human form.", victim, NULL, NULL, TO_ROOM );
			victim->damroll = victim->damroll - 50;
	        victim->hitroll = victim->hitroll - 50; 
		    free_string( victim->morph );
			victim->morph = str_dup( "" );
		}
		if (IS_EXTRA(victim,EXTRA_EARTHMELD) )
		{
			REMOVE_BIT(victim->affected_by, AFF_SHIFT);
		    REMOVE_BIT(victim->extra, EXTRA_EARTHMELD);
			REMOVE_BIT(victim->act, PLR_WIZINVIS);
		}
    if (IS_CLASS(victim, CLASS_VAMPIRE))
		do_mortalvamp(victim,"");
    else if (IS_SET(victim->special, SPC_WOLFMAN))
		do_unwerewolf(victim,"");
    if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
		do_unpolymorph(victim,"");
	if (IS_CLASS(victim, CLASS_DROW) && victim->cur_form == FRM_DROWSPIDER)
		do_spiderform(victim,"");
    ch->fight_timer += 10;
    if (IS_CLASS(ch, CLASS_DEMON) && !IS_NPC(ch) && !IS_NPC(victim) && victim->race + 15 >= ch->race)
    {
		send_to_char("You rip away a part of their soul.\n\r", ch);
		send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
		ch->pcdata->souls += 1;
		if (ch->pcdata->souls > souls_needed(ch))
			ch->pcdata->souls = souls_needed(ch);
    }
    behead( victim );
    do_beastlike(ch,"");
    ch->pkill = ch->pkill + 1;
    victim->pdeath = victim->pdeath + 1;
    victim->rage = 0;
    victim->level = 2;
	victim->fight_timer = 0;

		if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
			sprintf(buf,"%s Screams in Unearthly Pain as %s latches onto him #7Fangs #nBared as he Sucks the #1Blood#n from the #7Pale Faced #0corpse ", victim->pcdata->switchname, ch->pcdata->switchname);
		}
		else if(IS_CLASS(ch, CLASS_MIND))
		{
			sprintf (buf,"%s hisses loudly as they wrap thier tentacles around %s's head and #1rips#n out thier still #6pulsing #5brain#n.",ch->pcdata->switchname, victim->pcdata->switchname);
/*			sprintf (buf, "%s wraps thier tentacles around
%s's head and #1rips#n our thier #5brain#n.",ch->pcdata->switchname,
victim->pcdata->switchname);*/
		}
		else if (IS_CLASS(ch, CLASS_WEREWOLF))
		{
			sprintf(buf,"%s howls in #1rage #nas he rips %s apart limb from limb.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MAGE))
		{
			sprintf(buf,"#6%s raises his arms #4chanting #6loudly as %s's head #1explodes #6everywhere#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MONK))
		{
			sprintf(buf,"%s calls on the #7Power of God#n to destroy %s mortal body.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_DROW))
		{
			sprintf(buf,"#1%s sac#0rifi#1ces %s's s#0ou#1l to L#0lot#1h#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_BERSERK))
{
sprintf(buf, "%s %sIM TO TIRED TO CODE THIS FUCKING THING IN", ch->pcdata->switchname, victim->pcdata->switchname);
} 
		else if (IS_CLASS(ch, CLASS_NINJA))
		{
			sprintf(buf,"%s Plunges his #1Blood Soaked #nDagger into %s's Back, pushing it so hard that it Bores through the ribcage pushing out the #1heart #nto the floor ", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_MECHA))
					 {
sprintf(buf,"#3You see Bright balls of lights and explosions everywhere as #1%s #2screams #3'#1MASSATSU RATTO!!!#3'#n #3yanking its #4vibro#1 blade  #3down charging past #2%s#n. #5A moment later #1\\#7|#1/#n #3they split in half#n.", ch->pcdata->switchname, victim->pcdata->switchname);
					 }
		else if (IS_CLASS(ch, CLASS_DEMON))
		{
			sprintf(buf,"%s tears %s's soul from this mortal realm", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_HIGHLANDER))
		{
			sprintf(buf,"%s Falls onto the floor in convulsions as %s Decapitates %s and in a #5Purple hazy #7mist#n #4Electricity #nhums around and rushes violently through there body", victim->pcdata->switchname, ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_SHAPE))
			sprintf(buf,"#5%s #2sh#3ifts #4in#2to #6the #1Decapitator of Doom, #5and slices off %s's head.", ch->name, victim->name);
		else if (IS_CLASS(ch, CLASS_AMAZON))
			sprintf(buf,"#6%s #6p#4ray#6s to Ar#4tem#6is to cl#4ean#6se t#4h#6e im#4puri#6ty in %s#n.",
				ch->pcdata->switchname, victim->pcdata->switchname);
		else if (IS_CLASS(ch, CLASS_JEDI))
			sprintf(buf,"%s Drops to his knees and Practically melts as %s's #2lightsabre#n Is Thrusted Between the lazer charred Corpse.", victim->pcdata->switchname, ch->pcdata->switchname);
		else 
			sprintf(buf,"%s has been decapitated by %s.",victim->pcdata->switchname,ch->pcdata->switchname);

    ch->exp += victim->exp / 2;
    victim->exp /= 2;
    do_info(ch,buf);
    sprintf( log_buf, "%s decapitated by %s at %d.", victim->name, ch->name, victim->in_room->vnum );
    log_string( log_buf );

if (!IS_NPC(victim) && victim->pcdata->bounty > 0)  
{
    sprintf(log_buf,"%s recives a #2%d#n quest point bounty, for killing %s.\n\r",
    ch->name, victim->pcdata->bounty, victim->name);
    do_info(victim,log_buf);
    ch->pcdata->quest += victim->pcdata->bounty;
    victim->pcdata->bounty =0;
}

 return;
}

void do_tear( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int gained_status = 0;

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
		return;

	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot do this whilst you are in the arena.\n\r", ch);
		return;
	}
    if ( !IS_CLASS(ch, CLASS_WEREWOLF))
    {
		send_to_char( "Huh?\n\r", ch );
		return;
    }

    if ( !IS_SET(ch->special, SPC_WOLFMAN))
    {
		send_to_char( "You can only tear heads off while in Crinos form.\n\r", ch );
		return;
	 }

    if ( !IS_VAMPAFF(ch, VAM_CLAWS))
    {
		send_to_char( "You better get your claws out first.\n\r", ch );
		return;
    }

    if ( arg[0] == '\0' )
    {
		send_to_char( "Who's head do you wish to tear off?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }
    if ( IS_NPC(victim) )
    {
		send_to_char( "You can only tear the heads off other players.\n\r", ch );
		return;
    }

	if (ch->level < 3)
    {
		send_to_char( "You must be an avatar to tear someone's head off.\n\r", ch );
		return;
    }

	if (victim->level < 3)
    {
		send_to_char( "You can only tear the head off another avatar.\n\r", ch );
		return;
    }

    if ( victim->position > 1 )
    {
		send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
		return;
    }

    if (is_safe(ch,victim))
		return;

    act( "You tear $N's head from $S shoulders!",  ch, NULL, victim, TO_CHAR    );
    send_to_char( "Your head is torn from your shoulders!\n\r", victim);
    act( "$n tears $N's head from $S shoulders!",  ch, NULL, victim, TO_NOTVICT );
    if ((IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
	&& !IS_CLASS(victim, CLASS_DEMON) && !IS_CLASS(victim, CLASS_DROW)
	&& !IS_SET(victim->special, SPC_CHAMPION)) 
    {
		if (ch->race == 0 && victim->race == 0)
		{
			ch->pcdata->stats[DEMON_CURRENT] += 1000;
			 ch->pcdata->stats[DEMON_TOTAL] += 1000;
		}
		else
		{
		    ch->pcdata->stats[DEMON_CURRENT] += victim->race * 1000;
			ch->pcdata->stats[DEMON_TOTAL] += victim->race * 1000;
		}
    }
	if (IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(victim, CLASS_DROW) &&
	!IS_CLASS(victim, CLASS_DEMON) && !IS_SET(victim->special, SPC_CHAMPION))
	{
		if (ch->race == 0 && victim->race == 0)
			ch->pcdata->stats[DROW_POWER] += 1000;
		else
			ch->pcdata->stats[DROW_POWER] += 1000 * victim->race;
	}
 
	if( (ch->race > 0) && ((victim->race < 1) && (victim->pkill < 2)) && victim->max_hit < 15000)
    {
		ch->paradox[0] += ch->race;
		ch->paradox[1] += ch->race;
		ch->paradox[2] += PARADOX_TICK;

		if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
			sprintf(buf,"%s Screams in Unearthly Pain as %s latches onto him #7Fangs #nBared as he Sucks the #1Blood#n from the #7Pale Faced #0corpse ", victim->pcdata->switchname, ch->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MIND))
		{
			sprintf (buf, "%s hisses loudly as they wrap thier tentacles around %s's head and #1rips#n out thier still #6pulsing #5brain#n.",ch->pcdata->switchname, victim->pcdata->switchname);
/*			sprintf (buf, "%s wraps thier tentacles around
%s's head and #1rips#n our thier #5brain#n.",ch->pcdata->switchname,
victim->pcdata->switchname);*/
		}
		else if (IS_CLASS(ch, CLASS_WEREWOLF))
		{
			sprintf(buf,"%s howls in #1rage #nas he rips %s apart limb from limb.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MAGE))
		{
			sprintf(buf,"#6%s raises his arms #4chanting #6loudly as %s's head #1explodes #6everywhere#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MONK))
		{
			sprintf(buf,"%s calls on the #7Power of God#n to destroy %s mortal body.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_DROW))
		{
			sprintf(buf,"#1%s sac#0rifi#1ces %s's s#0ou#1l to L#0lot#1h#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_BERSERK))
{
sprintf(buf, "%s %s IM TO TIRED TO CODE THIS FUCKING THING IN", ch->pcdata->switchname, victim->pcdata->switchname);
} 
		else if (IS_CLASS(ch, CLASS_NINJA))
		{
			sprintf(buf,"%s Plunges his #1Blood Soaked #nDagger into %s Back, pushing it so hard that it Bores through the ribcage pushing out the #1heart #nto the floor ", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_MECHA))
                {
sprintf(buf,"#3You see Bright balls of lights and explosions everywhere as #1%s #2screams #3'#1MASSATSU RATTO!!!#3'#n #3yanking its #4vibro#1 blade #3down charging past #2%s#n. #5A moment later #1\\#7|#1/#n #3they split in half#n.", ch->pcdata->switchname, victim->pcdata->switchname);
					 }
		else if (IS_CLASS(ch, CLASS_DEMON))
		{
			sprintf(buf,"%s tears %s's soul from this mortal realm", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_HIGHLANDER))
		{
			sprintf(buf,"%s Falls onto the floor in convulsions as %s Decapitates %s and in a #5Purple hazy #7mist#n #4Electricity #nhums around and rushes violently through there body", victim->pcdata->switchname, ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_SHAPE))
			sprintf(buf,"#5%s #2sh#3ifts #4in#2to #6the #1Decapitator of Doom,#5 and slices off %s's head.", ch->name, victim->name);
 		else if (IS_CLASS(ch, CLASS_AMAZON))
			sprintf(buf,"#6%s #6p#4ray#6s to Ar#4tem#6is to cl#4ean#6se t#4h#6e im#4puri#6ty in %s#n.",
				ch->pcdata->switchname, victim->pcdata->switchname);
		else if (IS_CLASS(ch, CLASS_JEDI))
			sprintf(buf,"%s Drops to his knees and Practically melts as %s's #2lightsabre#n Is Thrusted Between the lazer charred Corpse.", victim->pcdata->switchname, ch->pcdata->switchname);
		else 
			sprintf(buf,"%s has been decapitated by %s.",victim->pcdata->switchname,ch->pcdata->switchname);

    	do_info(ch,buf);
		sprintf( log_buf, "%s decapitated by %s at %d for no status.",
	    victim->name, ch->name, victim->in_room->vnum );
		log_string( log_buf );
		if (IS_CLASS(victim, CLASS_VAMPIRE))
			do_mortalvamp(victim,"");
		else if (IS_SET(victim->special, SPC_WOLFMAN))
			do_unwerewolf(victim,"");
		if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH))
			do_unpolymorph(victim,"");
		if (IS_CLASS(victim, CLASS_DROW) && victim->cur_form == FRM_DROWSPIDER)
			do_spiderform(victim,"");
		ch->fight_timer += 10;
		if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_POLYAFF(victim, POLY_ZULOFORM))
		{
			if (IS_EXTRA(victim, EXTRA_DRAGON))
			{
				REMOVE_BIT(victim->extra,EXTRA_DRAGON);
				act( "You transform back into zuloform.", victim, NULL, NULL, TO_CHAR );
			    act( "$n shrinks back into a big black monster.", victim, NULL, NULL, TO_ROOM );
			    victim->damroll = victim->damroll - 25;
				victim->hitroll = victim->hitroll - 25;
				free_string( victim->morph );
				victim->morph = str_dup( "A big black monster" );
			}
	        REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
		    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	        act( "You transform into human form.", victim, NULL, NULL, TO_CHAR );
		    act( "$n's shrinks back into human form.", victim, NULL, NULL, TO_ROOM );
			victim->damroll = victim->damroll - 50;
	        victim->hitroll = victim->hitroll - 50; 
		    free_string( victim->morph );
			victim->morph = str_dup( "" );
		}
		if (IS_EXTRA(victim,EXTRA_EARTHMELD) )
		{
			REMOVE_BIT(victim->affected_by, AFF_SHIFT);
		    REMOVE_BIT(victim->extra, EXTRA_EARTHMELD);
			REMOVE_BIT(victim->act, PLR_WIZINVIS);
		}

		behead( victim );
		do_beastlike(ch,"");
		ch->pkill = ch->pkill + 1;
		victim->pdeath = victim->pdeath + 1;
		victim->fight_timer = 0;
		return;
	}
    ch->exp = ch->exp + 1000;

    /* This means an avatar will always gain status */
	if (ch->race == 0) 
    {
    	ch->race = ch->race + 1;
		gained_status = 1;
	if ( victim->race > 0)
	    victim->race--;
    }
    /* But if they have status, only gain status up to 50 and only if their 
       victim had at least one status */
	else if ((ch->race < 60) && (ch->race - victim->race <= 10))
    {
		gained_status = 1;
		ch->race = ch->race + 1;
	if (victim->race > 0 )
	    victim->race--;
    }

    /* and this loses status, but only if they had more than 0 */
	if (victim->race == 0)
    	victim->race = victim->race;

    act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, TO_CHAR    );
    act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, TO_NOTVICT );
    act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, TO_NOTVICT );
    if (IS_CLASS(victim, CLASS_VAMPIRE))
		do_mortalvamp(victim,"");
    else if (IS_SET(victim->special, SPC_WOLFMAN))
		do_unwerewolf(victim,"");
    if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
		do_unpolymorph(victim,"");
	if (IS_CLASS(victim, CLASS_DROW) && victim->cur_form == FRM_DROWSPIDER)
		do_spiderform(victim,"");
    ch->fight_timer += 10;
		if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_POLYAFF(victim, POLY_ZULOFORM))
		{
			if (IS_EXTRA(victim, EXTRA_DRAGON))
			{
				REMOVE_BIT(victim->extra,EXTRA_DRAGON);
				act( "You transform back into zuloform.", victim, NULL, NULL, TO_CHAR );
			    act( "$n shrinks back into a big black monster.", victim, NULL, NULL, TO_ROOM );
				 victim->damroll = victim->damroll - 25;
				victim->hitroll = victim->hitroll - 25;
				free_string( victim->morph );
				victim->morph = str_dup( "A big black monster" );
			}
	        REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
		    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	        act( "You transform into human form.", victim, NULL, NULL, TO_CHAR );
		    act( "$n's shrinks back into human form.", victim, NULL, NULL, TO_ROOM );
			victim->damroll = victim->damroll - 50;
	        victim->hitroll = victim->hitroll - 50; 
		    free_string( victim->morph );
			victim->morph = str_dup( "" );
		}
		if (IS_EXTRA(victim,EXTRA_EARTHMELD) )
		{
			REMOVE_BIT(victim->affected_by, AFF_SHIFT);
		    REMOVE_BIT(victim->extra, EXTRA_EARTHMELD);
			REMOVE_BIT(victim->act, PLR_WIZINVIS);
		}

    behead( victim );
    do_beastlike(ch,"");
    ch->pkill = ch->pkill + 1;
    victim->pdeath = victim->pdeath + 1;
    victim->rage = 0;
    victim->level = 2;
		if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
			sprintf(buf,"%s Screams in Unearthly Pain as %s latches onto him #7Fangs #nBared as he Sucks the #1Blood#n from the #7Pale Faced #0corpse ", victim->pcdata->switchname, ch->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MIND))
		{
			sprintf (buf, "%s hisses loudly as they wrap thier tentacles#n around %s's head and #1rips#n out thier still #6pulsing #5brain#n.",ch->pcdata->switchname, victim->pcdata->switchname);
/*			sprintf (buf, "%s wraps thier tentacles around
%s's head and #1rips#n our thier #5brain#n.",ch->pcdata->switchname,
victim->pcdata->switchname);*/
		}
		else if (IS_CLASS(ch, CLASS_WEREWOLF))
		{
			sprintf(buf,"%s howls in #1rage #nas he rips %s apart limb from limb.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MAGE))
		{
			sprintf(buf,"#6%s raises his arms #4chanting #6loudly as %s's head #1explodes #6everywhere#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_MONK))
		{
			sprintf(buf,"%s calls on the #7Power of God#n to destroy %s mortal body.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_DROW))
		{
			sprintf(buf,"#1%s sac#0rifi#1ces %s's s#0ou#1l to L#0lot#1h#n.", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_BERSERK))
{
sprintf(buf, "%s %s IM TO TIRED TO CODE THIS FUCKING THING IN", ch->pcdata->switchname, victim->pcdata->switchname);
} 
		else if (IS_CLASS(ch, CLASS_NINJA))
		{
			sprintf(buf,"%s Plunges his #1Blood Soaked #nDagger into %s's Back, pushing it so hard that it Bores through the ribcage pushing out the #1heart #nto the floor ", ch->pcdata->switchname, victim->pcdata->switchname);
		}
else if (IS_CLASS(ch, CLASS_MECHA))
                {
sprintf(buf,"#3You see Bright balls of lights and explosions everywhere as #1%s #2screams #3'#1MASSATSU RATTO!!!#3'#n #3yanking its #4vibro#1 blade #3down charging past #2%s#n. #5A moment later #1\\#7|#1/#n #3they split in half#n.", ch->pcdata->switchname, victim->pcdata->switchname);
					 }
		else if (IS_CLASS(ch, CLASS_DEMON))
		{
			sprintf(buf,"%s tears %s's soul from this mortal realm", ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_HIGHLANDER))
		{
			sprintf(buf,"%s Falls onto the floor in convulsions as %s Decapitates %s and in a #5Purple hazy #7mist#n #4Electricity #nhums around and rushes violently through there body", victim->pcdata->switchname, ch->pcdata->switchname, victim->pcdata->switchname);
		}
		else if (IS_CLASS(ch, CLASS_SHAPE))
			sprintf(buf,"#5%s #2sh#3ifts #4in#2to #6the #1Decapitator of Doom, and slices off %s's head.", ch->name, victim->name);
		else if (IS_CLASS(ch, CLASS_AMAZON))
			sprintf(buf,"#6%s #6p#4ray#6s to Ar#4tem#6is to cl#4ean#6se t#4h#6e im#4puri#6ty in %s#n.",
				ch->pcdata->switchname, victim->pcdata->switchname);
		else if (IS_CLASS(ch, CLASS_JEDI))
			sprintf(buf,"%s Drops to his knees and Practically melts as %s's #2lightsabre#n Is Thrusted Between the lazer charred Corpse.", victim->pcdata->switchname, ch->pcdata->switchname);
		else 
			sprintf(buf,"%s has been decapitated by %s.",victim->pcdata->switchname,ch->pcdata->switchname);

    do_info(ch,buf);
	victim->fight_timer = 0;
    sprintf( log_buf, "%s decapitated by %s at %d.",
	victim->name, ch->name, victim->in_room->vnum );
    log_string( log_buf );
    return;
}
void do_crack( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    OBJ_DATA  *right;
    OBJ_DATA  *left;

    right = get_eq_char( ch, WEAR_WIELD );
    left = get_eq_char( ch, WEAR_HOLD );
    if (right != NULL && right->pIndexData->vnum == 12)
		obj = right;
    else if (left != NULL && left->pIndexData->vnum == 12)
		obj = left;
    else
    {
		send_to_char("You are not holding any heads.\n\r",ch);
		return;
    }
    act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR);
    act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM);
    act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_CHAR);
    if (obj->chobj != NULL)
    {
		act("$p cracks open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_NOTVICT);
    	act("$p crack open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_VICT);
	}
    else
 		act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_ROOM);
	crack_head(ch,obj,obj->name);
    obj_from_char(obj);
	 extract_obj(obj);
}
void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument )
{
    CHAR_DATA *victim;
    MOB_INDEX_DATA *pMobIndex; 

    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char       buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if (str_cmp(arg2,"mob") && obj->chobj != NULL && !IS_NPC(obj->chobj) && IS_AFFECTED(obj->chobj,AFF_POLYMORPH))
    {
		victim = obj->chobj;
		make_part(victim,"cracked_head");
		make_part(victim,"brain");
		sprintf(buf,"the quivering brain of %s",victim->name);
		free_string(victim->morph);
		victim->morph = str_dup(buf);
		return;
    }
    else if (!str_cmp(arg2,"mob"))
    {
		if ( ( pMobIndex = get_mob_index( obj->value[1] ) ) == NULL )
			return;
		victim = create_mobile( pMobIndex );
		char_to_room(victim,ch->in_room);
		make_part(victim,"cracked_head");
		make_part(victim,"brain");
		extract_char(victim,TRUE);
		return;
    }
    else
    {
		if ( ( pMobIndex = get_mob_index( 30002 ) ) == NULL )
			return;
		victim = create_mobile( pMobIndex );

		sprintf( buf, capitalize(arg2) );
		free_string( victim->short_descr );
		victim->short_descr = str_dup( buf );
		char_to_room(victim,ch->in_room);
		make_part(victim,"cracked_head");
		make_part(victim,"brain");
		extract_char(victim,TRUE);
		return;
    }
    return;
}
/* Voodoo skill by KaVir */

void do_voodoo( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char       buf [MAX_INPUT_LENGTH];
    char     part1 [MAX_INPUT_LENGTH];
    char     part2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0')
    {
		send_to_char( "Who do you wish to use voodoo magic on?\n\r", ch );
		return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
		send_to_char( "You are not holding a voodoo doll.\n\r", ch );
		return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
		send_to_char( "They are not here.\n\r", ch );
		return;
    }

    if ( IS_NPC(victim) )
    {
		send_to_char( "Not on NPC's.\n\r", ch );
		return;
    }

    sprintf(part2,obj->name);
    sprintf(part1,"%s voodoo doll",victim->name);

    if ( str_cmp(part1,part2) )
    {
		sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
		send_to_char( buf, ch );
		return;
    }

    if ( arg2[0] == '\0')
		send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
    else if ( !str_cmp(arg2, "stab") )
    {
		WAIT_STATE(ch,12);
		act("You stab a pin through $p.", ch, obj, NULL, TO_CHAR); 
 		act("$n stabs a pin through $p.", ch, obj, NULL, TO_ROOM);
		if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
			return;
		act("You feel an agonising pain in your chest!", victim, NULL, NULL, TO_CHAR);
 		act("$n clutches $s chest in agony!", victim, NULL, NULL, TO_ROOM);
    }
    else if ( !str_cmp(arg2, "burn") )
    {
		WAIT_STATE(ch,12);
		act("You set fire to $p.", ch, obj, NULL, TO_CHAR);
 		act("$n sets fire to $p.", ch, obj, NULL, TO_ROOM);
		act("$p burns to ashes.", ch, obj, NULL, TO_CHAR);
 		act("$p burns to ashes.", ch, obj, NULL, TO_ROOM);
		obj_from_char(obj);
		extract_obj(obj);
		if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
			return;
		if (IS_AFFECTED(victim,AFF_FLAMING) )
			return;
		SET_BIT(victim->affected_by, AFF_FLAMING);
		act("You suddenly burst into flames!", victim, NULL, NULL, TO_CHAR);
	 	act("$n suddenly bursts into flames!", victim, NULL, NULL, TO_ROOM);
    }
    else if ( !str_cmp(arg2, "throw") )
    {
		WAIT_STATE(ch,12);
		act("You throw $p to the ground.", ch, obj, NULL, TO_CHAR);
	 	act("$n throws $p to the ground.", ch, obj, NULL, TO_ROOM);
		obj_from_char(obj);
		obj_to_room(obj, ch->in_room);
		if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
			return;
		if (victim->position < POS_STANDING)
			return;
		if (victim->position == POS_FIGHTING)
		    stop_fighting(victim, TRUE);
		act("A strange force picks you up and hurls you to the ground!", victim, NULL, NULL, TO_CHAR);
 		act("$n is hurled to the ground by a strange force.", victim, NULL, NULL, TO_ROOM);
		victim->position = POS_RESTING;
		victim->hit = victim->hit - number_range(ch->level,(5*ch->level));
		update_pos(victim);
		if (victim->position == POS_DEAD && !IS_NPC(victim))
		{
			do_killperson(ch,victim->name);
			return;
		}
    }
    else
		send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
    return;
}
/*  void check_arena( CHAR_DATA *ch, CHAR_DATA *victim )
{
    DESCRIPTOR_DATA *d; needed for Arena bet checking
    char buf[MAX_STRING_LENGTH];
    float odds;
    float lvl1, lvl2;
    int payoff;

    sprintf(buf,"[Arena] %s has defeated %s!\n\r", ch->name,victim->name);
    for ( d = descriptor_list; d; d = d->next )
    {
		if( d->connected == CON_PLAYING && ( d->character != victim && d->character != ch ))
		{
			ADD_COLOUR(ch,buf,L_RED);
			send_to_char( buf, d->character );
		}
    }
    ch->pcdata->awins += 1;
    victim->pcdata->alosses += 1;
    lvl1 = ch->hit;
    lvl2 = victim->hit;
    odds = ( lvl2 / lvl1 );
    for( d = descriptor_list; d; d = d->next )
    {
       if (d->connected == CON_PLAYING )
       {
          if (d->character->gladiator == ch)
          {
             payoff = d->character->pcdata->plr_wager * ( odds + 1 );
             sprintf(buf, "You won! Your wager: %d, payoff: %d\n\r",
                           d->character->pcdata->plr_wager, payoff );
             send_to_char(buf,d->character);
             d->character->exp += payoff;
              reset the betting info 
             d->character->gladiator = NULL;
             d->character->pcdata->plr_wager = 0;
             payoff = 0;
          }
          if (d->character->gladiator != ch
           && d->character->pcdata->plr_wager >= 1)
          {
             int tmp = 0;
             sprintf(buf, "You lost! Your wager: %d\n\r",
                           d->character->pcdata->plr_wager);
             send_to_char(buf,d->character);
             if (d->character->pcdata->plr_wager >d->character->exp )
             {
                tmp = d->character->pcdata->plr_wager; 
                d->character->pcdata->plr_wager -= tmp;
             }
             if ( tmp > 0 )
                d->character->pcdata->quest -= tmp;
             d->character->exp -= d->character->pcdata->plr_wager;
              reset the betting info 
			  d->character->gladiator = NULL;
             d->character->pcdata->plr_wager = 0;
          }
       }
    }

       now move both fighters out of arena and back
       to the regular "world" be sure to define 
       ROOM_VNUM_AWINNER and ROOM_VNUM_ALOSER 
    stop_fighting(victim,TRUE);
    char_from_room(victim);
    char_to_room(victim,get_room_index(ROOM_VNUM_ALOSER));
    victim->hit = victim->max_hit;
    victim->mana = victim->max_mana;
    update_pos( victim );
    do_look(victim, "auto");
    stop_fighting(ch,TRUE);
    char_from_room(ch);
    char_to_room(ch,get_room_index(ROOM_VNUM_AWINNER));
    ch->hit = ch->max_hit;
    ch->mana = ch->max_mana;
    update_pos( ch );
    do_look(ch, "auto");

    if (IS_SET(ch->act,PLR_CHALLENGER))
        REMOVE_BIT(ch->act,PLR_CHALLENGER);
    if (IS_SET(victim->act,PLR_CHALLENGER))
        REMOVE_BIT(victim->act,PLR_CHALLENGER);
    if (IS_SET(victim->act,PLR_CHALLENGED))
        REMOVE_BIT(victim->act,PLR_CHALLENGED);
    if (IS_SET(ch->act,PLR_CHALLENGED))
        REMOVE_BIT(ch->act,PLR_CHALLENGED);

    ch->challenger = NULL;
    ch->challenged = NULL;
    victim->challenger = NULL;
    victim->challenged = NULL;
    REMOVE_BIT(ch->act, PLR_SILENCE);
    REMOVE_BIT(ch->act,PLR_NO_TELL);
    REMOVE_BIT(victim->act,PLR_SILENCE);
    REMOVE_BIT(victim->act,PLR_NO_TELL);

    arena = FIGHT_OPEN;  clear the arena 
return;
} */
void do_garotte(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int chance;

	one_argument(argument, arg);
	chance = number_percent();

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE))
	{
		send_to_char("Huh?\n\r", ch );
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("Garotte whom?\n\r", ch );
		return;
	}

	if (victim == ch)
	{
		send_to_char("That would be a bad idea.\n\r", ch );
		return;
	}

	if (is_safe(ch, victim))
		return;

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->value[3] != 4 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->value[3] != 4 ) )
    {
        send_to_char( "You need to wield a whip.\n\r", ch );
        return;
    }
     
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't garotte a fighting person.\n\r", ch );
        return;
    }

/*	if (victim->hit < victim->max_hit)
	{
		act("$N is too hurt for you to sneak up.", ch, NULL,victim,TO_CHAR);
		return;
	}
*/
	check_killer(ch, victim);
	WAIT_STATE(ch, skill_table[gsn_garotte].beats);
if (victim->hit < victim->hit * .80)
{
    stc("They must be 80% health or higher..\n\r", ch);
    return;
}	
	if (chance <= 5)
	{
        ch->fight_timer += 5;
		damage(ch, victim, 0, gsn_garotte);
		return;
	}

	if ( chance == 13 )
	{ /* bad luck :) */
		act("$n wraps a whip around $N's neck! $N is choking to death!",ch,NULL,victim,TO_NOTVICT);
		act("You wrap your whip around $N's neck and refuse to let go!",ch,NULL,victim,TO_CHAR);
		act("$n wraps his whip around your neck, and you can't break free!",ch,NULL,victim,TO_VICT);
		if (!IS_NPC(victim))
		{
			ch->fight_timer += 5;
			behead(victim);
			sprintf(buf, "%s has been assassinated by %s!",victim->name,ch->name);
			do_info(ch,buf);
		}
		else
		{
			victim->hit = 1;
			damage(ch, victim, 1000, gsn_garotte);
		}
		return;
	} 
	else
	{
		multi_hit(ch, victim, gsn_garotte);
		return;
	}
	return;
}

void do_dark_garotte(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int chance;
	int i;

	one_argument(argument, arg);
	chance = number_percent();

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)
	|| !IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE))
	{
		send_to_char("Huh?\n\r", ch );
		return;
	}

	if (!IS_SET(ch->newbits, NEW_DARKNESS))
	{
		send_to_char("You have to be surrounded in darkness.\n\r", ch );
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("Dark Garotte whom?\n\r", ch );
		return;
	}

	if (victim == ch)
	{
		send_to_char("That would be a bad idea.\n\r", ch );
		return;
	}

	if (is_safe(ch, victim))
		return;

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->value[3] != 4 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->value[3] != 4 ) )
    {
        send_to_char( "You need to wield a whip.\n\r", ch );
        return;
    }
     
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't garotte a fighting person.\n\r", ch );
        return;
    }

/*	if (victim->hit < victim->max_hit)
	{
		act("$N is too hurt for you to sneak up.", ch, NULL,victim,TO_CHAR);
		return;
	}
*/
	check_killer(ch, victim);
	WAIT_STATE(ch, skill_table[gsn_garotte].beats);
	REMOVE_BIT(ch->newbits, NEW_DARKNESS);	
	REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);

	if ( chance >= 95 )
	{ /* bad luck :) */
		act("$n wraps a whip around $N's neck! $N is choking to death!",ch,NULL,victim,TO_NOTVICT);
		act("You wrap your whip around $N's neck and refuse to let go!",ch,NULL,victim,TO_CHAR);
		act("$n wraps his whip around your neck, and you can't break free!",ch,NULL,victim,TO_VICT);
		if (!IS_NPC(victim))
		{
	        	ch->fight_timer += 5;
			behead(victim);
			sprintf(buf, "%s has been assassinated by %s!",victim->name,ch->name);
			do_info(ch,buf);
		}
		else
		{
			victim->hit = 1;
			damage(ch, victim, 1000, gsn_garotte);
		}
	}
	else if (chance <= 5)
	{
		ch->fight_timer += 5;
		damage(ch, victim, 0, gsn_garotte);
	}
	else 
	{
		for (i = 1; i <= 3; i++)
			multi_hit(ch, victim, gsn_garotte);
	}
	return;
}


bool has_timer(CHAR_DATA *ch) 
{
	if (ch->fight_timer > 0 && !IS_NPC(ch)) 
	{
		send_to_char("Not until your fight timer runs out!\n\r", ch );
		return TRUE;
	}
	return FALSE;
}

void do_bash(CHAR_DATA *ch, char *argument)
{
int dam =0, sn = skill_lookup("bash");
CHAR_DATA *victim = NULL;
    if ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level)
    {
		send_to_char("First you should learn to bash.\n\r", ch );
		return;
    }
    if ( ( victim = ch->fighting ) == NULL )
    {
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    }
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[sn] )
    	dam = number_range(1,4);
    else
    {
if (IS_CLASS(victim,CLASS_GOLEM) && victim->pcdata->powers[1] != 1){
    	dam = 0;
    	damage( ch, victim, dam, sn );
		return;}
    }
/*
if (IS_CLASS(victim,CLASS_GOLEM) && victim->pcdata->powers[1] >= 1)
    dam = victim->max_hit + 1000;*/
    dam += char_damroll(ch);
    if (dam <= 0)
		dam = 1;
    if ( !IS_AWAKE(victim) )
		dam *= 2;
/*
    if ((victim->wait2 > 0 || victim->countdown > 0)&& (number_percent() >
85 || IS_INFORM(victim,GOLEM_EMERALD)))
    {dam *= 3.5;victim->wait2 =1;victim->countdown =
0;victim->convert_to = 0;
act("$n is distrupted!",victim,0x0,0x0,TO_ROOM);
act("Your distripted!",victim,0x0,0x0,TO_CHAR);
if (IS_INFORM(victim,GOLEM_EMERALD))
{act("#2The emerald around $n shatters.#n",victim,0,0,TO_ROOM);
act("#2The emerald around you shatters.#n",victim,0,0,TO_CHAR);
NO_STONE(victim);}
}*/
dam = dice(dam,5);
    damage(ch,victim,dam,sn);

return;
}

void deadstuff( CHAR_DATA *ch, CHAR_DATA *victim)
{
int vnum = 0;
    if ( victim->position == POS_DEAD )
    {
		group_gain( ch, victim );

		if ( !IS_NPC(victim) )
		{
			sprintf( log_buf, "%s killed by %s at %d",
			victim->name,
			(IS_NPC(ch) ? ch->short_descr : ch->name),
			victim->in_room->vnum );
			log_string( log_buf );
	        /* Dying penalty: 1/2 your current exp.  */
			if ( victim->exp > 0 )
				victim->exp = victim->exp / 2;
		}
    	if (IS_NPC(victim) && !IS_NPC(ch))
		{
		    ch->mkill += 1;
		    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION) || IS_CLASS(ch, CLASS_DROW))
			{
				if ((vnum = victim->pIndexData->vnum) > 29000)
				{
					switch (vnum)
					{
				    default:
						if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION))
						{
							ch->pcdata->stats[DEMON_CURRENT] +=victim->level;
							ch->pcdata->stats[DEMON_TOTAL]   +=victim->level;
						}
						else
							ch->pcdata->stats[DROW_POWER] += victim->level;
					break;
				    case 29600:
					case 30001:
					case 30006:
				    case 30007:
				    case 30008:
				    case 30009:
					case 30000:
					break;
				    }
				}
				else if (IS_NPC(victim) && !IS_SET(victim->act, ACT_NOEXP))
				{
					if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION))
					{
						ch->pcdata->stats[DEMON_CURRENT] += victim->level/2;
						ch->pcdata->stats[DEMON_TOTAL] += victim->level/2;
					}
					else
						ch->pcdata->stats[DROW_POWER] += victim->level/2;
				}
			}
		    if (ch->level == 1 && ch->mkill > 4)
		    {
			send_to_char("You are not NOTE TRUSTED!\r\n", ch);
			SET_BIT(ch->extra, EXTRA_NOTE_TRUST);
				ch->level = 2;
				save_char_obj(ch);
			}
		}
		if (!IS_NPC(victim) && IS_NPC(ch)) 
	    {
			victim->mdeath = victim->mdeath + 1;
	    }
		raw_kill( victim );
		if ( IS_SET(ch->act, PLR_AUTOLOOT) )
		    do_get( ch, "all corpse" );
		else
		    do_look( ch, "in corpse" );

		if ( !IS_NPC(ch) && IS_NPC(victim) )
		{
			if ( IS_SET(ch->act, PLR_AUTOSAC) )
				do_sacrifice( ch, "corpse" );
		}
		return;
    }

    if ( victim == ch )
		return;


}
