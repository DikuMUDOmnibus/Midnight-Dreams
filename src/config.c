/* use the one in smoo.c */
// By Smoo
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "config.h"

void do_classconfig(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    int class;
    int bit;
    int ss;

argument = one_argument(argument, arg1);
argument = one_argument(argument, arg2);
argument = one_argument(argument, arg3);

	if (arg1[0] == '\0')
	{send_to_char("
Type classconfig set bit number  to set bit as number
Type classconfig mod bit number  to modify bit by number

bit = (class)damcap or (class)dam or exp or dam<mobile damage>
      or hit<mobile hitpoints> or level<mobile levels>

if the first argument is set, the normal bit will be set to * number
(if the exp of a mob is normally 3500, and you set it to 2, it will be
7000 or twice the ammount) if the first argument is mod, then the current
exp will be x by number.\n\r", ch);
return;}

if (!str_cmp(arg1, "set")) ss = SET;
if (!str_cmp(arg1, "mod")) ss = MOD;

if (ss == 0) {do_classconfig(ch, "");return;}

if (!str_cmp(arg2, "demondamcap")) {class = demon;bit = damcap;}
if (!str_cmp(arg2, "demondam")) {class = demon;bit = dam;}
if (!str_cmp(arg2, "magedamcap")) {class = mage;bit = damcap;}
if (!str_cmp(arg2, "magedam")) {class = mage;bit = dam;}
if (!str_cmp(arg2, "werewolfdamcap")) {class = werewolf;bit = damcap;}
if (!str_cmp(arg2, "werewolfdam")) {class = werewolf;bit = dam;}
if (!str_cmp(arg2, "monkdamcap")) {class = monk;bit = damcap;}
if (!str_cmp(arg2, "monkdam")) {class = monk;bit = dam;}
if (!str_cmp(arg2, "vampiredamcap")) {class = vampire;bit = damcap;}
if (!str_cmp(arg2, "vampiredam")) {class = vampire;bit = dam;}
if (!str_cmp(arg2, "drowdamcap")) {class = drow;bit = damcap;}
if (!str_cmp(arg2, "drowdam")) {class = drow;bit = dam;}
if (!str_cmp(arg2, "amazondamcap")) {class = amazon;bit = damcap;}
if (!str_cmp(arg2, "amazondam")) {class = amazon;bit = dam;}
if (!str_cmp(arg2, "jedidamcap")) {class = jedi;bit = damcap;}
if (!str_cmp(arg2, "jedidam")) {class = jedi;bit = dam;}
if (!str_cmp(arg2, "shapeshifterdamcap")) {class = shape;bit = damcap;}
if (!str_cmp(arg2, "shapeshifterdam")) {class = shape;bit = dam;}
if (!str_cmp(arg2, "dam")) {class = mobile;bit = dam;}
if (!str_cmp(arg2, "hit")) {class = mobile;bit = hit;}
if (!str_cmp(arg2, "level")) {class = mobile; bit = level;}
if (!str_cmp(arg2, "exp")) {class = mobile; bit = exp;}


if (class == 0 || bit == 0)
{do_classconfig(ch, "");return;}

if (!is_number(arg3))
{do_classconfig(ch, "");return;}

if (ss == SET)
    {
	if (bit == damcap) {
	if (class == jedi) config.jedidamcap = arg3;
	if (class == demon) config.demondamcap = arg3;
	if (class == mage) config.magedamcap = arg3;
	if (class == werewolf) config.werewolfdamcap = arg3;
	if (class == monk) config.monkdamcap = arg3;
	if (class == vampire) config.vampiredamcap = arg3;
	if (class == drow) config.drowdamcap = arg3;
	if (class == amazon) config.amazondamcap = arg3;
	if (class == shape) config.shapedamcap = arg3;
	}
	else if (bit == dam) {
	if (class == jedi) config.jedidam = arg3;
	if (class == demon) config.demondam = arg3;
	if (class == mage) config.magedam = arg3;
	if (class == werewolf) config.werewolfdam = arg3;
	if (class == monk) config.monkdam = arg3;
	if (class == vampire) config.vampiredam = arg3;
	if (class == drow) config.drowdam = arg3;
	if (class == amazon) config.amazondam = arg3;
	if (class == shape) config.shapedam = arg3;
	if (class == mobile) config.dam = arg3;
	}
	else if (bit == hit) config.hit = arg3;
	else if (bit == level) config.level = arg3;
	else if (bit == exp) config.exp = arg3;
	else {do_classconfig(ch, "");return;}
    }


if (ss == MOD)
    {
	if (bit == damcap) {
	if (class == jedi) config.jedidamcap *= arg3;
	if (class == demon) config.demondamcap *= arg3;
	if (class == mage) config.magedamcap *= arg3;
	if (class == werewolf) config.werewolfdamcap *= arg3;
	if (class == monk) config.monkdamcap *= arg3;
	if (class == vampire) config.vampiredamcap *= arg3;
	if (class == drow) config.drowdamcap *= arg3;
	if (class == amazon) config.amazondamcap *= arg3;
	if (class == shape) config.shapedamcap *= arg3;
	}
	else if (bit == dam) {
	if (class == jedi) config.jedidam *= arg3;
	if (class == demon) config.demondam *= arg3;
	if (class == mage) config.magedam *= arg3;
	if (class == werewolf) config.werewolfdam *= arg3;
	if (class == monk) config.monkdam *= arg3;
	if (class == vampire) config.vampiredam *= arg3;
	if (class == drow) config.drowdam *= arg3;
	if (class == amazon) config.amazondam *= arg3;
	if (class == shape) config.shapedam *= arg3;
        if (class == mobile) config.dam *= arg3;
	}
	else if (bit == hit) config.hit *= arg3;
	else if (bit == level) config.level *= arg3;
	else if (bit == exp) config.exp *= arg3;
	else {do_classconfig(ch, "");return;}
    }
return;
}
