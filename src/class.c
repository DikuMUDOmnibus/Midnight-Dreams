
/* By Smoo */
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

/*
the structure is:
the class number/definition (as defined in class.h),
class name, starting trust(when classed, for invis things), starting 
level (spells) bits (as defined in class.h)
*///add new classes to the bottom or you'll fuck up classconfig
const   struct  strctr_class_table  class_table       []            =
{
  {0,		"Mortal",	2,	2,	0,	BIT_CLOSED},
  {CLASS_DEMON,	"Demon",	4,	3,	1,	0	},
  {CLASS_MAGE,	"Mage",		5,	4,	3,	BIT_CLOSED},
  {CLASS_WEREWOLF,"Werewolf",	3,	3,	7,	0	},
  {CLASS_VAMPIRE,"Vampire",	3,	3,	4,	0	},
  {CLASS_HIGHLANDER,"Highlander",3,	3,	1,	BIT_CLOSED},
  {CLASS_DROW,	"Drow", 	4,	3,	5,	0	},
  {CLASS_MONK,	"Monk",		6,	3,	4,	0	},
  {CLASS_NINJA,	"Ninja",	5,	3,	4,	BIT_CLOSED},
  {CLASS_AMAZON,"Amazon",	3,	3,	3,	0	},
  {CLASS_DRAGON,"Dragon",	3,	3,	5,	BIT_CLOSED|BIT_DONTCLASS},
  {CLASS_HERO,	"Hero",		3,	3,	3,	BIT_CLOSED|BIT_DONTCLASS},
  {CLASS_JEDI,	"Jedi",		5,	5,	6,	0	},
  {CLASS_SHAPE,	"Shape",	4,	4,	4,	0	},
  {CLASS_GOLEM,	"Golem",	3,	4,	10,	0,},
  {CLASS_MIND, "Mind",		3,	3,	4,	BIT_CLOSED|BIT_DONTCLASS},
  {CLASS_MECHA, "Pilot",	3,	3,	3,	BIT_CLOSED|BIT_DONTCLASS},
  {CLASS_UNICORN,"Unicorn",	3,	3,	5,
				BIT_CLOSED|BIT_DONTCLASS|BIT_PTEST_TOGGLE},
  {CLASS_COW,	"cow",		6,	6,	1,	BIT_PTEST_SAFE},
  {CLASS_HUNTER,"Hunter",        3,     3,      1,	BIT_CLOSED|BIT_PTEST_TOGGLE},
  {CLASS_BERSERK, "Zerker",     3,      3,      3, 	BIT_CLOSED|BIT_PTEST_SAFE},
  {CLASS_MOOGLE,"Moogle",	4,	4,	5, 	BIT_CLOSED|BIT_DONTCLASS|BIT_PTEST_SAFE},
  {0,		"",		0,	0,	0,	0	}//keep this as
								 //last
};

const char clan_rank[10][16][100] =
{
 {"", "Lemure", "Nupperibo", "Spinagon", "Barbazu", "Abishai",
  "Osyluth", "Hamatula","Erinyes", "Amnizuo", "Cornugon", "Gelugon", 
  "Pit Fiend", "Pit Lord", "Pit Lord", "Satan's Right Hand"},
 {"","#5Purple#6","#1Red#6","#4Blue#6","#2Green#6","#3Yellow#6",
  "#0Black#6","#7White#6"},
 {"", "#1Entribed#6", "#6Chief#6", "#3Entrusted#6",
  "#2PackMember#6", "#2PackMember#6", "#2PackMember#6", "#2PackMember#6",
"#2PackMember#6", "#2PackMember#6", "#2PackMember#6" },
 {"", "#6Master#6", "#7Founder#6 of ", "#2Prince#6 of ",
"#3Youngling of #6", "#3Youngling of #6", "#3Youngling of #6",
"#3Youngling of #6", "#3Youngling of #6", "#3Youngling of #6",
"#3Youngling of #6", "#3Youngling of #6", "#3Youngling of #6" },
 {"", "Lloth's Avatar", "House Matron", "Noble", "Childae", "Unwanted"},
 {"", "Master", "Nabishyo", "Takuha", "Ninja", "Silibing"},
 {"","General","Moogle Knight","Soldier","Fluffy","Baby Moogle"},
};

#define CLAN(xch) ((xch->clan && str_cmp(xch->clan,"") && str_cmp(xch->clan, "(null)")) ? xch->clan : IS_CLASS(xch, CLASS_WEREWOLF) ? "Ronin" : IS_CLASS(xch, CLASS_VAMPIRE) ? "Caitiff" :"Rogue") 
#define TABLE(a, b) (clan_rank[a][b > 15 ? 15 : b])


char *get_class_name( CHAR_DATA *ch )
{
 static char retur[MSL];

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	sprintf(retur, "{[%s#6 %s]}", CLAN(ch), TABLE(0, ch->generation));
    }
    else if (IS_CLASS(ch, CLASS_MAGE))
    {
	sprintf(retur, "{%s %s}", TABLE(1,ch->pcdata->powers[0]+1), 
	    ch->generation == 1 ? "Grand Sorcerer" : !IS_SET(ch->extra,
EXTRA_TRANSC) ? (ch->level == 6 ? "ArchMage" : ch->level == 5 ? "Mage" :
"Apprentice") : IS_SET(ch->extra, EXTRA_NECRO) ? "Necromancer" :
IS_SET(ch->extra, EXTRA_ILLUS) ? "Illusionist" : IS_SET(ch->newbits,
NEW_INVK) ? "Invoker" : "Sorcerer");
    }
    else if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	sprintf(retur, "(%s#6%s%s)", TABLE(2, ch->generation),
ch->generation != 1 ? " of " : "",
ch->generation == 1 ? "" : CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
	sprintf(retur, "<%s%s#6>",
TABLE(3,ch->generation),ch->generation == 1 ? " Vampire" : CLAN(ch));
    }
    else if (IS_CLASS(ch,CLASS_COW))
	sprintf(retur,"#6!!CoW!!");
    else if (IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	sprintf(retur, "#6<(>HighLander<)>#n");
    }
    else if (IS_CLASS(ch, CLASS_BERSERK))
    {
	sprintf(retur, "#1XzX#nZERKER#1XzX");
    }
    else if (IS_CLASS(ch, CLASS_DROW))
    {
	sprintf(retur, ".oO %s#6 %s0o.", ch->generation == 1 ? "Lloth's Avatar"
	    : CLAN(ch), ch->generation == 1 ? "" : TABLE(4,ch->generation));
    }
    else if (IS_CLASS(ch, CLASS_MONK))
    {
	sprintf(retur, "*** %s#6 %s ***", ch->generation == 1 ? "Monk" :
	CLAN(ch), ch->generation == 4 ? "Monk" : "Master");
    }
    else if (IS_CLASS(ch, CLASS_NINJA))
    {
	sprintf(retur, "**[) %s %s#6 (]**", ch->generation == 1 ? "Ninja" : TABLE(5, ch->generation), ch->generation == 1 ? TABLE(5,ch->generation) : CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_AMAZON))
    {
	if (ch->sex == SEX_MALE)
	    sprintf(retur, "#5-+{ #3Amazon Slave#5 }+-#n");
	else if (ch->generation == 1)
	    sprintf(retur, "#5-+{ #6Amazon Queen#5 }+-#n");
	else if (!ch->clan || !str_cmp(ch->clan,"") || !str_cmp(ch->clan, "(null)"))
	    sprintf(retur, "#5-+{ Amazon }+-#n");
	else if (ch->generation == 2)
	    sprintf(retur, "#5-+{ #6%s#6 Princess#5 }+-#n", ch->clan);
	else 
	    sprintf(retur, "#5-+{ #6%s#6 Amazon#5 }+-#n", ch->clan);
    }
    else if (IS_CLASS(ch, CLASS_DRAGON))
    {
	sprintf(retur, "DRAGON????");
    }
    else if (IS_CLASS(ch, CLASS_HERO))
    {
	sprintf(retur, "Wtf is a HERO?");
    }
    else if (IS_CLASS(ch, CLASS_JEDI))
    {
	if (ch->generation == 1)
	    sprintf(retur, "-<[ Force Lord ]>-");
	else if (ch->generation == 2)
	    sprintf(retur, "-<[ %s#6 Master ]>-", CLAN(ch));
	else if (IS_SET(ch->special, SPC_PRINCE))
	    sprintf(retur, "-<[ %s#6 Knight ]>-", CLAN(ch));
	else if (ch->generation == 6)
	    sprintf(retur, "-<[ %s#6 Apprentice ]>-", CLAN(ch));
	else
	    sprintf(retur, "-<[ Adept of %s#6 ]>-", CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_HUNTER))
    {
        if (ch->generation == 1)
             sprintf(retur, "#3<#2~-~#3> #7Head#0Hunter #3<#2~-~#3>#n");
        else 
             sprintf(retur, "#3<#2~-~#3> #7Head#0Hunter #3<#2~-~#3>#n");  
    }
 else if (IS_CLASS(ch, CLASS_MECHA))
    {
        if (ch->generation == 1)
            sprintf(retur, "^o^ Mecha Slayer ^o^");
        else if (ch->generation == 2)
            sprintf(retur, "^o^ %s#4 Veteran ^o^", CLAN(ch));
        else
            sprintf(retur, "^o^ %s#6 Pilot ^o^", CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_SHAPE))
    {
	if (!ch->clan || !str_cmp(ch->clan, "") || !str_cmp(ch->clan, "(null)"))
	    sprintf(retur, "#1{-=+._Shape_.+=-}");
	else if (ch->generation == 1)
	    sprintf(retur, "#1{-=+._PolyMorphanter_.+=-}");
	else if (ch->generation == 2)
	    sprintf(retur, "#1{-=+._King of %s#1_.+=-}", CLAN(ch));
	else if (ch->generation == 3)
	    sprintf(retur, "#1{-=+._Forceman of %s#1_.+=-}", CLAN(ch));
	else
	    sprintf(retur, "#1{-=+._ArmyMan of %s#1_.+=-}", CLAN(ch));
    }
    else if (IS_CLASS(ch, CLASS_GOLEM))
    {
	sprintf(retur," #3[][#n%s#3][#1GoLeM#3][#n%s#3][]#n",
	CLAN(ch), golem_ranks[ch->generation]);	
    }
    else if (IS_MOOGLE(ch))
    {
	sprintf(retur,"#0[#1\\/#0]#6<#7%s#7#7%s#7%s#6>#0[#1\\/#0]#n", TABLE(6,ch->generation), str_cmp(ch->clan,"") ? " of ":"", str_cmp(ch->clan,"") ? CLAN(ch) : "");
    }
    else if (IS_UNI(ch))
    {
	sprintf(retur,"#2<#3:)#2> #7UnicorN #2<#3(:#2>#n");
    }
    else
	sprintf(retur, "Mortal");

 return retur;
}


int class_lookup(int class)
{
int a;
for (a = 0;class_table[a].class_name[0] != '\0';a++)
if (class_table[a].class == class) break;
if (class_table[a].class_name[0] != '\0') return a;
return -1;
}

int class_lookupn(char * class)
{
int a;
for (a = 0;class_table[a].class_name[0] != '\0';a++)
if (!str_cmp(class_table[a].class_name, class)) break;
if (class_table[a].class_name[0] != '\0') return a;
return -1;
}


void do_class(CHAR_DATA *ch, char *argument)
{
  int a = 0;
  CHAR_DATA *victim;
  int cl = 0;
  char buf[MIL];
  char arg1[MIL];
  char arg2[MIL];

argument = one_argument(argument, arg1);
argument = one_argument(argument, arg2);


if (arg1[0] == '\0') {
send_to_char("You may choose from:\n\r",ch);
for (cl = 0;class_table[cl].class_name[0] != '\0'; cl++)
{if (++a % 3 == 0) stc("\n\r", ch);
sprintf(log_buf, "%s%s#6%-12s  ", class_table[cl].bits & BIT_CLOSED ?
"#5|" : "#2-",
class_table[cl].bits & BIT_DONTCLASS ? "#1*" :
"#2-", class_table[cl].class_name); stc(log_buf, ch);
}
send_to_char("\n\r#n",ch);
return;}

if ((victim = get_char_world(ch,arg1)) == NULL)
    {
	send_to_char("They aren't here!\n\r",ch);
	return;
    }

if (!str_cmp(arg2, "none") || !str_cmp(arg2,"mortal"))
    {
    int a;
if (victim->class == 0)
return;
	do_mclear(ch, victim->pcdata->switchname);

	for (a = 0;a <= 20;a++)
	victim->pcdata->powers[a] = 0;
	set_learnable_disciplines(victim);

REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
if (victim->trust < 7)
{
REMOVE_BIT(victim->act, PLR_WIZINVIS);
REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
}


	if (IS_AFFECTED(victim,AFF_POLYMORPH))
	{do_unpolymorph(victim,"");
	 free_string(victim->morph);
	 victim->morph = str_dup("");
	 REMOVE_BIT(victim->affected_by,AFF_POLYMORPH);
	}

victim->morphy = FALSE;

	for (a = 1; a <= 11; a++)
	victim->pcdata->stats[a] = 0;

        if ( IS_EXTRA(victim, EXTRA_OSWITCH) )
        {
         do_humanform(victim,"");
        } 

	victim->special = 0;
	victim->polyaff = 0;

	REMOVE_BIT(victim->immune, IMM_SHIELDED);    
	victim->warp = 0;
	victim->warpcount = 0;
	victim->extra = 0;
	victim->newbits = 0;
	REMOVE_BIT(victim->newbits, THIRD_HAND);
	REMOVE_BIT(victim->newbits, FOURTH_HAND);
        REMOVE_BIT(victim->act, PLR_WIZINVIS);
                        REMOVE_BIT(victim->act, OLD_PLR_DEMON);
                        REMOVE_BIT(victim->act, OLD_PLR_CHAMPION);
                        REMOVE_BIT(victim->act, OLD_PLR_WEREWOLF);
                        REMOVE_BIT(victim->act, OLD_PLR_WOLFMAN);
                        REMOVE_BIT(victim->act, OLD_PLR_VAMPIRE);       

      REMOVE_BIT(victim->affected_by, AFF_SHIFT); 
victim->newbits = 0;
for (a = 0; a <= 13; a++)
reset_weapon(victim, a);
for (a = 0; a <= 4; a++)
reset_spell(victim, a);
victim->move = victim->max_move;
victim->mana = victim->max_mana;
victim->hit = victim->max_hit;
victim->pcdata->rank = 0;
victim->generation = 4;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );

victim->class = 0;
if (ch->level < 7)
{
ch->level = 3;
ch->trust = 0;
}
stc("Ok!\n\r",ch);
stc("You are classless!\n\r",victim);
return;
}

    if (class_lookupn(arg2) == -1) {
stc("That class doesn't exist!\n\r",ch);return;}
if (IC_BIT_NAME(arg2, BIT_CLOSED) && ch->trust < 7 &&
!is_pure("class",ch->immpower))
{stc("That class is closed.\n\r", ch);return;}
if (IC_BIT_NAME(arg2, BIT_DONTCLASS) && victim->level < 7 &&
!is_pure("class",ch->immpower) && ch->level < 12) {
stc("That class is closed to the mortals.\n\r",ch);return;}
cl = class_lookupn(arg2);
sprintf(log_buf, "%s none",victim->pcdata->switchname);
do_class(ch,log_buf);

    victim->class = class_table[cl].class;
    victim->trust = class_table[cl].start_trust;
    victim->level = class_table[cl].start_level;
sprintf(log_buf, "%s %d", victim->pcdata->switchname,class_table[cl].start_gen);
do_generation(ch, log_buf);
    set_learnable_disciplines(victim);
    sprintf(buf,"#6You are now a %s!#n\n\r",class_table[cl].class_name);
    stc(buf,victim);
    stc("Ok.\n\r",ch);
return;
}

void do_ptesttoggle(CHAR_DATA *ch, char *argument)
{

if (IS_NPC(ch)) return;

if (!IC_BIT_NUMB(ch->class,BIT_PTEST_TOGGLE) &&
!IS_PTEST_SAFE(ch))
{stc("Huh?\n\r",ch);return;}

if (ch->fight_timer > 0) {
stc("Wait til your fight timer runs out. Do not abuse this!\n\r",ch);
return;
}

if (IS_PTEST_SAFE(ch))
{REM_PTEST_SAFE(ch);
 stc("You are no longer safe from pk (ptesting mode off)\n\r",ch);
 return;
}
else
{SET_PTEST_SAFE(ch);
 stc("You are now in safe (ptest) mode. DO NOT abuse this!\n\r",ch);
 return;
}

return;
}

/*char *parse_class_list( void )
{
int a = 0;
int count = 0;


for (a = 0;class_table[a].class_name[0] != '\0';a++)
count++;

*/

void do_magecolor(CHAR_DATA *ch, char *argument)
{
 char arg[MIL];
 CHAR_DATA *vict;

 argument = one_argument(argument, arg);

 if (arg[0] == '\0')
 {
    stc("Change what mage to what color?
\r(magecolor [victim] [purple/red/blue/green/yellow/black/white])\n\r",ch);
    return;
 }

 if ((vict = get_char_world(ch, arg)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 if (!IS_CLASS(vict, CLASS_MAGE))
 {
    stc("They aren't a mage!\n\r", ch);
    return;
 }

 if (!*argument)
 {
    stc("What color? red/purple/blue/green/yellow/black/white\n\r",ch);
    return;
 }

#define mageecol(a,b) if (!str_cmp(argument, a))\
{\
 vict->pcdata->powers[0] = b;\
 sprintf(log_buf, "You are now a %s mage!\n\r", a);\
 stc(log_buf, vict);\
 return;\
}

 mageecol("purple",PURPLE_MAGIC)
 mageecol("red",RED_MAGIC)
 mageecol("blue",BLUE_MAGIC)
 mageecol("yellow",YELLOW_MAGIC)
 mageecol("green",GREEN_MAGIC)
 mageecol("black",BLACK_MAGIC)
 mageecol("white",WHITE_MAGIC)

 stc("That color doesn't exist! Choose purple/red/blue/yellow/green/black/white!\n\r", ch);
 return;
}
