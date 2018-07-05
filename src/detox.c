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
#include "merc.h"


/* Needs Major Revision!!! -Gol*/
void do_detox( CHAR_DATA *ch, char *argument )
{        
    if ( IS_NPC(ch) )
		return;
	if (ch->class == 0)
	{	
		send_to_char("You are already classless.\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Train avatar before detox.\n\r", ch);
		return;
	}
	else if (ch->in_room->vnum != 3206)
	{
		send_to_char("You are not in the detox chamber.\n\r", ch);
		return;
	}
	else if (ch->pcdata->quest < 1000)
	{
		send_to_char("You cannot afford the 1000 qp required to detox.\n\r", ch);
		return;
	}
	else if ( IS_AFFECTED(ch, AFF_CHARM)   )
	{
		send_to_char( "Not whilst charmed!" ,ch);
		return;
	}
	else if (IS_NEWFLAG(ch,AFF_ALLOW_VAMP))
	{
		send_to_char("You must have vamp allow on first!\n\r",
ch);
		return;
	}
	else if (ch->spl[2] < 101)
	{
		send_to_char("You must have more than 100 in blue magic to do this.\n\r", ch);
		return;
	}
	else
	{
		do_remove(ch, "all");
		/* Vampires */
		if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
			do_mortalvamp(ch,"");
			if (IS_POLYAFF(ch, POLY_ZULOFORM))
			{
				if (IS_EXTRA(ch, EXTRA_DRAGON))
				{
					REMOVE_BIT(ch->extra,EXTRA_DRAGON);
				    ch->damroll = ch->damroll - 25;
					ch->hitroll = ch->hitroll - 25;
					free_string( ch->morph );
					ch->morph = str_dup( "A big black monster" );
				}
			    REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
				REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
				ch->damroll = ch->damroll - 50;
				ch->hitroll = ch->hitroll - 50; 
			}
		    if (IS_EXTRA(ch, EXTRA_POTENCY))
			{ 
				ch->generation =ch->generation +1;
				REMOVE_BIT(ch->extra, EXTRA_POTENCY);
			}
				REMOVE_BIT(ch->newbits, NEW_TIDE);
			if (IS_EXTRA(ch, EXTRA_BAAL))
		    {
				ch->power[DISC_VAMP_POTE] -= 1;
				ch->power[DISC_VAMP_CELE] -= 1;
			    ch->power[DISC_VAMP_FORT] -=1;
				REMOVE_BIT(ch->extra, EXTRA_BAAL);
			}   
		    if (IS_EXTRA(ch, EXTRA_FLASH))
			{
			    ch->power[DISC_VAMP_CELE] -= 1;
				REMOVE_BIT(ch->extra, EXTRA_FLASH);
			}
		    if (IS_VAMPAFF(ch,VAM_FANGS) )
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
			if (IS_VAMPAFF(ch,VAM_DISGUISED) )
			{
				free_string( ch->morph );
				ch->morph = str_dup( ch->name );
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
			}
		    if (IS_VAMPAFF(ch,VAM_CLAWS) )
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
			if (IS_VAMPAFF(ch,VAM_DISGUISED) )
			{
				free_string( ch->morph );
				ch->morph = str_dup( ch->name );
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
			}
			if (IS_SET(ch->newbits, NEW_MONKFLAME))
				REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
			if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_VAMPAFF(ch,VAM_CLAWS) ) 
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
			if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
			if (IS_SET(ch->special, SPC_INCONNU))
				REMOVE_BIT(ch->special, SPC_INCONNU );
			if (IS_SET(ch->special, SPC_ANARCH))
				REMOVE_BIT(ch->special, SPC_ANARCH );
			if (IS_SET(ch->special, SPC_SIRE))
				REMOVE_BIT(ch->special, SPC_SIRE);
			if (IS_SET(ch->pcdata->stats[UNI_AFF], VAM_CHANGED))
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
			if (IS_POLYAFF(ch, POLY_MIST))
			{	
				REMOVE_BIT(ch->polyaff, POLY_MIST);
				REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
			}
			if (IS_AFFECTED(ch, AFF_POLYMORPH))
			{
				if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
				{
					REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
				    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
					REMOVE_BIT(ch->polyaff, POLY_BAT);
				}
				else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
				{
				    REMOVE_BIT(ch->polyaff, POLY_WOLF);
					ch->max_hit = ch->max_hit - 500;
				    ch->hit = ch->hit - 500;
					if (ch->hit < 1)
						ch->hit = 1;
				}
				else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
				{
				    REMOVE_BIT(ch->polyaff, POLY_MIST);
				    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
				}	
				REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
				REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
				clear_stats(ch);
  				free_string( ch->morph );
    			ch->morph = str_dup( "" );
			}
		}
		/* Mage */
		else if (IS_CLASS(ch, CLASS_MAGE))
		{
			if (IS_CLASS(ch, CLASS_MAGE) && IS_AFFECTED(ch, AFF_POLYMORPH)) 
				do_unpolymorph(ch,"");
			ch->pcdata->powers[MPOWER_RUNE0] = 0;
			ch->pcdata->powers[MPOWER_RUNE1] = 0;
			ch->pcdata->powers[MPOWER_RUNE2] = 0;
			ch->pcdata->powers[MPOWER_RUNE3] = 0; 

		}
		/* Drow */
		else if (IS_CLASS(ch, CLASS_DROW))
		{
			if (ch->cur_form != get_normal_form(ch))
				set_form(ch, get_normal_form(ch) );
			if (IS_SET(ch->special, SPC_DROW_WAR))
				REMOVE_BIT(ch->special, SPC_DROW_WAR );
			if (IS_SET(ch->special, SPC_DROW_MAG))
				REMOVE_BIT(ch->special, SPC_DROW_MAG );
			if (IS_SET(ch->special, SPC_DROW_CLE))
				REMOVE_BIT(ch->special, SPC_DROW_CLE );
			if (IS_SET(ch->affected_by, AFF_POLYMORPH))
				REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
			if (IS_SET(ch->newbits, NEW_DARKNESS))
			{
				REMOVE_BIT(ch->newbits, NEW_DARKNESS);
				if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
					REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
			}
			if (IS_SET(ch->newbits, NEW_DROWHATE))
				REMOVE_BIT(ch->newbits, NEW_DROWHATE);
			ch->pcdata->powers[DROW_POWER]     = 0;
  			ch->pcdata->stats[DROW_TOTAL]      = 0;
			ch->pcdata->stats[DROW_MAGIC]      = 0;
			ch->pcdata->powers[1]              = 0;
		}
		/* Monk */
		else if (IS_CLASS(ch, CLASS_MONK))
		{
			if (IS_SET(ch->newbits, NEW_MONKFLAME))
				REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
			if ( IS_SET(ch->act, PLR_SACREDINVIS) )
				REMOVE_BIT(ch->act, PLR_SACREDINVIS);
		    if (IS_SET(ch->newbits, NEW_MONKADAM))
				REMOVE_BIT(ch->newbits, NEW_MONKADAM);
		}
		/* Werewolf */
		else if (IS_CLASS(ch, CLASS_WEREWOLF))
		{
			if (IS_SET(ch->special, SPC_WOLFMAN))
				do_unwerewolf(ch,"");
			if (IS_SET(ch->newbits, NEW_SCLAWS))
				REMOVE_BIT(ch->newbits, NEW_SCLAWS);
			if (IS_SET(ch->newbits, NEW_SLAM))
				REMOVE_BIT(ch->newbits,NEW_SLAM);
			if (IS_SET(ch->newbits, NEW_QUILLS))
				REMOVE_BIT(ch->newbits,NEW_QUILLS);
			if (IS_SET(ch->newbits, NEW_PERCEPTION))
				REMOVE_BIT(ch->newbits,NEW_PERCEPTION);
			if (IS_SET(ch->newbits, NEW_JAWLOCK))
  				REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
			if (IS_SET(ch->newbits, NEW_REND))
				REMOVE_BIT(ch->newbits,NEW_REND);
		}
		/* Demons */
		else if (IS_CLASS(ch, CLASS_DEMON))
		{
			if (IS_DEMAFF(ch,DEM_HORNS))
				REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);		
			if (IS_DEMAFF(ch,DEM_HOOVES))
				REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
			if (IS_DEMAFF(ch,DEM_WINGS))
			{
				if (IS_DEMAFF(ch,DEM_UNFOLDED) )
					REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
				REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
			}
			if (IS_SET(ch->special, SPC_DEMON_LORD))
				REMOVE_BIT(ch->special, SPC_DEMON_LORD);
			if ( IS_EXTRA(ch, EXTRA_OSWITCH) ) 
				do_humanform(ch,"");
			if (IS_SET(ch->special, SPC_CHAMPION))	
				REMOVE_BIT(ch->special, SPC_CHAMPION );
			ch->pcdata->powers[DPOWER_FLAGS]   = 0;
		  	ch->pcdata->stats[DEMON_TOTAL]     = 0;
			ch->pcdata->stats[DEMON_CURRENT]   = 0;
			if (IS_SET(ch->newbits, THIRD_HAND))
				REMOVE_BIT(ch->newbits, THIRD_HAND);
			if (IS_SET(ch->newbits, FOURTH_HAND))
				REMOVE_BIT(ch->newbits, FOURTH_HAND);
		}
		/* Ninja */
		else if (IS_CLASS(ch, CLASS_NINJA))
		{
			ch->pcdata->powers[NPOWER_SORA] = 0;
			ch->pcdata->powers[NPOWER_CHIKYU] = 0;
			ch->pcdata->powers[NPOWER_NINGENNO] = 0;
			if ( IS_SET(ch->act, PLR_SACREDINVIS) )
				REMOVE_BIT(ch->act, PLR_SACREDINVIS);
			if ( IS_SET(ch->special, SPC_ROGUE) )
				REMOVE_BIT(ch->special, SPC_ROGUE);
		}
		/* Highlanders */
		
		/* Amazons */
		else if (IS_CLASS(ch, CLASS_AMAZON))
		{
			if (IS_SET(ch->newbits, NEW_SKIN))
			{ 
				ch->armor += 100;
  				REMOVE_BIT(ch->newbits,NEW_SKIN);
			}
			ch->pcdata->powers[PAMAZON]	   = 0;
			ch->pcdata->powers[AMA_COMBO]  = 0;
			if ( IS_SET(ch->act, PLR_SACREDINVIS) )
				REMOVE_BIT(ch->act, PLR_SACREDINVIS);
			if (IS_SET(ch->newbits, NEW_JAWLOCK))
				REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
		    if (IS_SET(ch->newbits, NEW_MONKFLAME))
				REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
		    if (IS_SET(ch->newbits, NEW_MONKADAM))
				REMOVE_BIT(ch->newbits, NEW_MONKADAM);
		}
		/* Jedi */
		
		/* General */
		if (IS_SET(ch->affected_by, AFF_ETHEREAL)    )
			REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
		if (IS_SET(ch->special, SPC_PRINCE))
			REMOVE_BIT(ch->special, SPC_PRINCE);
		if (IS_SET(ch->act, PLR_HOLYLIGHT) )
			REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		if (IS_SET(ch->act, PLR_WIZINVIS) )
			REMOVE_BIT(ch->act, PLR_WIZINVIS);
		if (IS_AFFECTED(ch,AFF_SHADOWPLANE) )
			REMOVE_BIT(ch->affected_by,AFF_SHADOWPLANE);
		if (IS_IMMUNE(ch,IMM_SHIELDED) )
			REMOVE_BIT(ch->immune, IMM_SHIELDED);
		if (IS_AFFECTED(ch,PLR_SHADOWSIGHT) )
			REMOVE_BIT(ch->affected_by, PLR_SHADOWSIGHT);
		ch->power[DISC_VAMP_CELE] = -1;
		ch->power[DISC_VAMP_FORT] = -1;
		ch->power[DISC_VAMP_OBTE] = -1;
		ch->power[DISC_VAMP_PRES] = -1;
		ch->power[DISC_VAMP_QUIE] = -1;
		ch->power[DISC_VAMP_THAU] = -1;
		ch->power[DISC_VAMP_AUSP] = -1;
		ch->power[DISC_VAMP_DOMI] = -1;
		ch->power[DISC_VAMP_OBFU] = -1;
		ch->power[DISC_VAMP_POTE] = -1;
		ch->power[DISC_VAMP_PROT] = -1;
		ch->power[DISC_VAMP_SERP] = -1;
		ch->power[DISC_VAMP_VICI] = -1;
		ch->power[DISC_VAMP_DAIM] = -1;
		ch->power[DISC_VAMP_ANIM] = -1;

		ch->power[DISC_WERE_BEAR] = -1;
		ch->power[DISC_WERE_LYNX] = -1;
		ch->power[DISC_WERE_BOAR] = -1;
		ch->power[DISC_WERE_OWL] = -1;
		ch->power[DISC_WERE_SPID] = -1;
		ch->power[DISC_WERE_WOLF] = -1;
		ch->power[DISC_WERE_HAWK] = -1;
		ch->power[DISC_WERE_MANT] = -1;
		ch->power[DISC_WERE_RAPT] = -1;
		ch->power[DISC_WERE_LUNA] = -1;
		ch->power[DISC_WERE_PAIN] = -1;
		ch->power[DISC_WERE_CONG] = -1;

		ch->power[DISC_DAEM_HELL] = -1;
		ch->power[DISC_DAEM_ATTA] = -1;
		ch->power[DISC_DAEM_TEMP] = -1;
		ch->power[DISC_DAEM_MORP] = -1;
		ch->power[DISC_DAEM_CORR] = -1;
		ch->power[DISC_DAEM_GELU] = -1;
		ch->power[DISC_DAEM_DISC] = -1;
		ch->power[DISC_DAEM_NETH] = -1;
		ch->power[DISC_DAEM_IMMU] = -1;

		ch->pcdata->stats[UNI_AFF]     = 0;
		ch->pcdata->stats[UNI_CURRENT] = -1;

		do_remove(ch, "all");
		do_clearstats(ch, "");
		ch->move = ch->max_move;
		ch->mana = ch->max_mana;
		ch->hit = ch->max_hit;
		ch->pcdata->rank = 0;
		ch->rage = 0;
		ch->generation = 3;
    	free_string(ch->lord);
    	ch->lord     = str_dup( "" );
    	free_string(ch->clan);
    	ch->clan     = str_dup( "" );
    	ch->generation     = 4;
	ch->hitroll = 0;
	ch->damroll = 0;
	ch->armor = 100;
    	ch->beast = 15;
    	ch->class = 0;
    	ch->trust = 0;
    	ch->level = 2;
	ch->pcdata->quest -= 1000;
    	send_to_char("You are fully detoxified and now a mortal.\n\r", ch);
    }
}
