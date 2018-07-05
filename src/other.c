#include <stdio.h>

#include "merc.h"

/*Stuff by Gol*/

void do_autostance(CHAR_DATA *ch, char *argument)
{
    sh_int stance;

    if (IS_NPC(ch) )
	return;
    if (argument[0] == '\0')
    {
	send_to_char("Autostance <stance>.\r\n", ch);
	return;
    }
    if (!str_cmp(argument, "none") )
    {
	send_to_char("Autostance removed!\r\n", ch);
	ch->autostance = 0;
	return;
    }
    if (!str_cmp(argument, "bull") )
	stance = STANCE_BULL;
    else if (!str_cmp(argument, "wolf") && IS_CLASS(ch,CLASS_WEREWOLF))
	stance = STANCE_WOLF;
    else if (!str_cmp(argument, "viper"))
	stance = STANCE_VIPER;
    else if (!str_cmp(argument, "crane"))
	stance = STANCE_CRANE;
    else if (!str_cmp(argument, "crab") )
	stance = STANCE_CRAB;
    else if (!str_cmp(argument, "mongoose"))
	stance = STANCE_MONGOOSE;
    else if (!str_cmp(argument, "tiger") && ch->stance[STANCE_VIPER] >=
200 && ch->stance[STANCE_BULL] >= 200)
	stance = STANCE_TIGER;
    else if (!str_cmp(argument, "dragon")&& ch->stance[STANCE_BULL] >= 200 
&& ch->stance[STANCE_CRAB] >= 200)
	stance = STANCE_DRAGON;
    else if (!str_cmp(argument, "mantis")&&ch->stance[STANCE_CRANE] >=
200 && ch->stance[STANCE_VIPER] >= 200)
	stance = STANCE_MANTIS;
    else if (!str_cmp(argument, "swallow") && ch->stance[STANCE_MONGOOSE] 
>= 200 && ch->stance[STANCE_CRAB] >= 200)
	stance = STANCE_SWALLOW;
    else if (!str_cmp(argument, "monkey") && ch->stance[STANCE_CRANE] >=
200 && ch->stance[STANCE_MONGOOSE] >= 200)
	stance = STANCE_MONKEY;
    else
    {
	send_to_char("Invalid stance.\r\n", ch);
	return;
    }
    ch->autostance = stance;
    send_to_char(capitalize(argument), ch);
    send_to_char(" stance is set.\r\n", ch);
    return;
}

//gets how many real hours they played on mud
int get_hours_played(CHAR_DATA *ch)
{
    return ( ch->played + (int) (current_time - ch->logon) ) / 3600;
}

int get_extra_hitroll(CHAR_DATA *ch)
{
    int xhit = 0;
 
    if (ch->cur_form > 0 && ch->cur_form < MAX_FORMS)
    {
	xhit += form_data[ch->cur_form].hit;
    }
    if (IS_CLASS(ch, CLASS_DEMON) )
    {
	xhit += ch->generation * 10;
    }
    return xhit;
}

int get_extra_damroll(CHAR_DATA *ch)
{
    int xdam = 0;
    if (ch->cur_form >0 && ch->cur_form < MAX_FORMS)
    {
	xdam += form_data[ch->cur_form].dam;
    }
    if (IS_CLASS(ch, CLASS_DEMON) )
    {
	xdam += ch->generation * 10;
    }
    return xdam;
}

void do_clearpks(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
	return;
    if (ch->in_room->vnum != 3054)
    {
	send_to_char("Please step into the Temple Altar to clear your records.\r\n", ch);
	return;
    }
    send_to_char("Done\r\n", ch);
    ch->pkill = 0;
    ch->pdeath = 0;
}
