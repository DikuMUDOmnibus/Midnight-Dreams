//all of these are original. All coded by the great and powerful Smoo.
//immspar doesn't give the right sudjestions, but the stats that it says
//are correct. the who command may be used with 'who (class)' or 'who 
//(name prefix)' to shorten the size.
//GIVE ME CREDIT!!! read the license please, at least my part of it..I'm
//not even asking for anything just give me credit
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"
#include "player.h"

#define bah(a,b) (nolarger(a,b))

/* local declarations */
void randomize_object(int vnum);
int number_attacks(CHAR_DATA *ch, CHAR_DATA *vict);


void do_loaddummy(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *dummy = NULL;
 CHAR_DATA *real;
 DESCRIPTOR_DATA *d = ch->desc;

 if (!char_exists(TRUE, argument))
 {
    stc("That player doesn't exist!\n\r", ch);
    return;
 }

 ch->desc = NULL;
 d->character = NULL;
 real = ch;
 load_char_obj(d, argument);
 dummy = d->character;
 dummy->next = char_list;
 char_list = dummy;
 char_to_room(dummy, real->in_room);
 dummy->desc = NULL;
 d->character = real;
 real->desc = d;

 act("$n has created dummy $N!", real, 0, dummy, TO_ROOM);
 act("You have created dummy $N!", real, 0, dummy, TO_CHAR);

 return;
}

void do_clearreply(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vch;
 bool donesomething = FALSE;

 for (vch = char_list; vch; vch = vch->next)
 {
   if (vch->level >= 7) continue;
    if (vch->reply == ch)
    {
	sprintf(log_buf, "%s no longer replies to you.\n\r", vch->name);
	stc(log_buf, ch);
	donesomething = TRUE;
	vch->reply = NULL;
    }
 }

 if (!donesomething)
 {
    stc("Nobody was replying to you anywho.\n\r", ch);
    return;
 }

 return;
}

void do_checkpk(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *check1;
    CHAR_DATA *check2;
    char buf[MAX_STRING_LENGTH];

for (d = descriptor_list;d != NULL; d = d->next)
{
 if ((check1 = d->character) == NULL) continue;
if (d->connected > CON_PLAYING) continue;

 if ((check2 = d->character->fighting) == NULL) continue;

 if (IS_NPC(check2)) continue;

	sprintf(buf, "%s(%0.2f) is fighting %s(%0.2f)\n\r", check1->name,
atof(CHAR2((check1->hit / check1->max_hit )))
,check2->name,atof(CHAR2((check2->hit/check2->max_hit))));
	send_to_char(buf, ch);
}
for (d = descriptor_list; d; d = d->next)
{if ((check1 = d->character) == NULL) continue;
if (d->connected > CON_PLAYING) continue;

 if (check1->hit <500)
    {
	sprintf(buf, "%s's hp is at %d/%d(%0.2f)\n\r", check1->name,
check1->hit, check1->max_hit,
atof(CHAR2((check1->hit/check1->max_hit))));
	send_to_char(buf, ch);
    }
}
send_to_char("Done.\n\r", ch);
return;
}

char *CHAR2(long iNum)
{
 char buf[300];

 sprintf(buf, "%ld", iNum);

 return str_dup(buf);
}
void do_setimmpower(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char arg1[MIL];
 char arg2[MIL];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {stc("Choose the arguments in the form: setimmpower (player) (command)\n\r", ch);
  return;}

 if ((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They aren't here..\n\r", ch);
    return;
 }

 if (!str_cmp(arg2, "stat"))
 {
    stc(vict->immpower,ch);stc("\n\r",ch);
    return;
 }

 if (is_pure(arg2, vict->immpower))
 {
    char string[1200];
    char string2[1200];
    int a = 0, b = 0, c = 0;

    if (!str_cmp(vict->immpower, arg2))
    {
	stc("ImmPower removed.\n\r", ch);
	free_string(vict->immpower);
	vict->immpower = str_dup("");
	return;
    }
strcpy(string,vict->immpower);

    do
    {
	for (b = 0; b < strlen(arg2); b++)
	  {char pink = string[a+b];
	  if (LOWER(pink) != LOWER(arg2[b])) break;}

      if (b == strlen(arg2) && (string[a+b] == ' ' || string[a+b] ==
'\0'))
        {int iA, iB = a;
for (iA = a; string[iA] != '\0' && iA <= (strlen(arg2) +iB); iA++,a++)
{if (string[iA] == '\0') break;}} else{
	string2[c] = string[a];
	c++;
	a++;
	string2[c] = '\0';}
    }
    while (string[a] != '\0');

    free_string(vict->immpower);
    vict->immpower = str_dup(string2);

    save_char_obj(vict);
    stc("Imm power removed.\n\r", ch);
    return;
 }

 {int cmd = 0;
    for (cmd = 0; str_cmp(cmd_table[cmd].name, ""); cmd++)
    {
	if (!str_cmp(cmd_table[cmd].name, arg2)) break;
    }

    if (!str_cmp(cmd_table[cmd].name,""))
    {
	for (cmd = 0; str_cmp(cmd_table[cmd].name,""); cmd++)
	{
	    if (!str_prefix(arg2,cmd_table[cmd].name)) break;
	}
	if (!str_cmp(cmd_table[cmd].name,""))
	{
	    stc("That command doesn't exist!\n\r", ch);
	    return;
	}
    }

sprintf(log_buf,"%s %s", vict->immpower, cmd_table[cmd].name);
free_string(vict->immpower);
vict->immpower = str_dup(log_buf);
    stc("Immpower set!\n\r", ch);
 }

return;
}


CONFIG_DATA	*	config_first;

void do_classconfig(CHAR_DATA *ch, char *argument)
{
 CONFIG_DATA * config;
 char arg1[MIL];
 char arg2[MIL];
 char arg3[MIL];
 char arg4[MIL];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);
 argument = one_argument(argument, arg3);
 strcpy(arg4, argument);

 if (arg1[0] == '\0')
 {
    stc("Please choose out of the following options;
\rclassconfig create (Class name)   	to add a class into the database
\rclassconfig list			to list all the classes and
\rstatistics of them
\rclassconfig mod class_name type modifier to add (modifier) to the type of statistic
\rclassconfig set class_name type modifier to set a type (statistic) to
modifier
\r
\rhelp classconfig for the details.\n\r", ch);
return;
}


 if (!str_cmp(arg1, "create"))
 {
    if (arg2[0] == '\0')
    {
	stc("Please pick a name of a class to create..\n\r", ch);
	return;
    }

    if (class_lookupn( arg2 ) == -1)
    {
	stc("That class doesn't exist. Make sure its added to class.c\n\r", ch);
	stc("Please choose another class ;)\n\r", ch);
	return;
    }

    for (config = config_first; config; config = config->next)
    {
	if ( config->number == class_lookupn( arg2 ) )
	{
	stc(config->name,ch);
	    stc("That class is already in the database.\n\r", ch);
	    return;
	}
    }

    config 			= 	NULL;
    config 			=	alloc_perm ( sizeof ( CONFIG_DATA ) );
    config->name		=	class_table[class_lookupn(arg2)].class_name;
    config->classbit		=	class_table[class_lookupn(arg2)].class;
    config->number		=	class_lookupn(arg2);
    config->attacks		=	0;
    config->damcap		=	1;
    config->damage		=	1;
    config->parryoff		=	0;
    config->dodgeoff		=	0;
    config->parrydef		=	0;
    config->dodgedef		=	0;
    config->toughness		=	1;

    config->next 		=	config_first;
    config_first		=	config;

    save_class_configs();

    stc("Ok.\n\r", ch);

    return;
 }

 else if (!str_cmp(arg1, "list"))
 {

stc("Order is class
name/#3(1)#nnumber/#3(2)#nbit/#3(3)#nattacks+/#3(4)#ndamcap*/#3(5)#ndamage*/#3(6)#nparryoff+/#3(7)#ndodgeoff+/#3(8)#nparrydef+/#3(9)#ndodgedef+/#3(10)#ntoughness*\n\r",ch);
    for (config = config_first; config; config = config->next)
    {
    sprintf(log_buf, "%s #3(1)#n%d #3(2)#n%ld #3(3)#n%d #3(4)#n%f #3(5)#n%f #3(6)#n%d #3(7)#n%d #3(8)#n%d #3(9)#n%d #3(10)#n%f\n\r",
     config->name,
     config->number, config->classbit, config->attacks, config->damcap,
     config->damage, config->parryoff, config->dodgeoff, config->parrydef,
     config->dodgedef, config->toughness);
    stc(log_buf, ch);
    }
    
    return;
 }


 else if (!str_cmp(arg1, "mod"))
 {

    if (is_number(arg2)) config = get_config(atof(arg2), NULL);
     else
    config = get_config( -1, arg2 );

    if ( !config )
    {
	stc("Please enter a class number or name as the second argument.\n\r", ch);
	return;
    }

    if (!strcmp(arg3, "attacks") && strlen(arg3) > 3)
	config->attacks += atof(arg4);
    else if (!strcmp(arg3, "damcap") && strlen(arg3) > 3)
	config->damcap *= atof(arg4);
    else if (!str_prefix(arg3, "damage") && strlen(arg3) > 2)
	config->damage *= atof(arg4);
    else if (!strcmp(arg3, "parryoff") && strlen(arg3) > 3)
	config->parryoff += atof(arg4);
    else if (!strcmp(arg3, "dodgeoff") && strlen(arg3) > 3)
	config->dodgeoff += atof(arg4);
    else if (!strcmp(arg3, "parrydef") && strlen(arg3) > 3)
	config->parrydef += atof(arg4);
    else if (!strcmp(arg3, "dodgedef") && strlen(arg3) > 3)
	config->dodgedef += atof(arg4);
    else if (!str_prefix(arg3, "toughness") && strlen(arg3) > 3)
	config->toughness *= atof(arg4);
    else
     {
	stc("Please choose out of: attacks, damcap, damage, dodgedef(defense), parrydef(defense), dodgeoff(offense), parryoff(offense), or toughness.\n\r", ch);
	return;
     }
save_class_configs();
    stc("Ok!\n\r", ch);
    return;

 }

 else if (!str_cmp(arg1, "set"))
 {

    if (is_number(arg2)) config = get_config(atof(arg2), NULL);
     else
    config = get_config( -1, arg2 );

    if ( !config )
    {
	stc("Please enter a class number or name as the second argument.\n\r", ch);
	return;
    }

    if (!strcmp(arg3, "attacks") && strlen(arg3) > 3)
	config->attacks = atof(arg4);
    else if (!strcmp(arg3, "damcap") && strlen(arg3) > 3)
	config->damcap = atof(arg4);
    else if (!str_prefix(arg3, "damage") && strlen(arg3) > 2)
	config->damage = atof(arg4);
    else if (!strcmp(arg3, "parryoff") && strlen(arg3) > 3)
	config->parryoff = atof(arg4);
    else if (!strcmp(arg3, "dodgeoff") && strlen(arg3) > 3)
	config->dodgeoff = atof(arg4);
    else if (!strcmp(arg3, "parrydef") && strlen(arg3) > 3)
	config->parrydef = atof(arg4);
    else if (!strcmp(arg3, "dodgedef") && strlen(arg3) > 3)
	config->dodgedef = atof(arg4);
    else if (!str_prefix(arg3, "toughness") && strlen(arg3) > 3)
	config->toughness = atof(arg4);
    else
     {
	stc("Please choose out of: attacks, damcap, damage, dodgedef(defense), parrydef(defense), dodgeoff(offense), parryoff(offense), or toughness.\n\r", ch);
	return;
     }
save_class_configs();
    stc("Ok!\n\r", ch);
    return;

 }
do_classconfig(ch,""); 
return;
}

void save_class_configs( void )
{
 FILE *fp;
 CONFIG_DATA *config;

 rename("../config.dat","../config.bak");

 if (( fp = fopen( "../config.dat", "w" ) ) == NULL )
 {
    bug("Can't open class config file for writing. Aborting.", 0);
    return;
 }

 for (config = config_first; config; config = config->next)
 {
    fprintf(fp, "%s %d %ld %d %f %f %d %d %d %d %f\n", config->name, 
     config->number, config->classbit, config->attacks, config->damcap,
     config->damage, config->parryoff, config->dodgeoff, config->parrydef,
     config->dodgedef, config->toughness);
 }

 fprintf(fp, "END\n");

 fflush(fp);
 fclose(fp);

return;
}

CONFIG_DATA *get_config( int number, char *name)
{
 CONFIG_DATA *config;

 if (number == -1 && name == NULL) return NULL;

 if (number != -1) {
    for (config = config_first; config; config = config->next)
	if (config->number == number) break;

    return config;    //make sure theres a check to see if its NULL on use!
 }

 for (config = config_first; config; config = config->next)
    if (!str_cmp(config->name, name)) break;

return config;
}

void load_class_configs( void )
{
 CONFIG_DATA *config;
 FILE *fp;
 char *buf;

 if (( fp = fopen("../config.dat", "r")) == NULL)
 {
    bug("config.dat in ../(root) dirrectory doesn't exist. Creating.",0);
    save_class_configs();
    return;
 }

 for (buf = fread_word(fp); str_cmp(buf, "END"); buf = fread_word(fp))
 {
    config			=	alloc_perm( sizeof( *config ) );
    config->name		=	str_dup(buf);
    fscanf(fp, "%d %ld %d %f %f %d %d %d %d %f", &config->number,
&config->classbit, &config->attacks, &config->damcap, &config->damage,
&config->parryoff, &config->dodgeoff, &config->parrydef,
&config->dodgedef, &config->toughness);

    config->next		=	config_first;
    config_first		=	config;
 }

 return;
}


void randomize_object(int vnum)
{
 OBJ_DATA *obj;
 ROOM_INDEX_DATA	*rm;
 int count = 0;
 int a;

 for (a = 1; a <= 60000; a++)
 {
    if (get_room_index(a) != NULL) count++;
 }

 for (a = 1; a <= 60000; a++)
 {
    if ((rm = get_room_index(a)) != NULL && number_range(1, count * 2) == 1)
	break;
    rm = NULL;
 }

if (rm) {
obj = create_object(get_obj_index(vnum),0);
if (!obj) return;
obj_to_room(obj, rm);
}
else randomize_object(vnum);
return;
}

void do_multiple(CHAR_DATA *ch, char *argument)
{
int a;
int b;
char times[MAX_INPUT_LENGTH];

argument = one_argument(argument, times);
a = atoi(times);
if (a > 240 || a < 2) {stc("Range is 2 to 240.\n\r", ch);return;}
for (b = 0;b != a;b++)
    interpret(ch, argument);
return;
}

void do_randomload(CHAR_DATA *ch, char *argument)
{
 if (!get_obj_index(atoi(argument)))
 {
    stc("That object doesn't exist!\n\r", ch);
    return;
 }
 stc(get_obj_index((atoi(argument)))->short_descr, ch);
 stc(" randomly loaded to a room on the mud.\n\r", ch);

 randomize_object(atoi(argument));

 return;
}

void do_rename(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char buf[MSL];
 OBJ_DATA *obj;

 argument = one_argument(argument, buf);

 if ((vict = get_char_world(ch, buf)) == NULL || IS_NPC(vict))
 {
    stc("THey aren't here!\n\r", ch);
    return;
 }
 else strcpy(buf, argument);

 sprintf(log_buf, "%s%s", PLAYER_DIR, capitalize(buf));

 if (!check_parse_name(buf) || fopen(log_buf,"r"))
 {
    stc("Illegal name. Choose another.\n\r", ch);
    return;
 }

 sprintf(log_buf, "%s%s", PLAYER_DIR, capitalize(vict->pcdata->switchname));
 unlink(log_buf);

 for (obj = object_list; obj; obj = obj->next)
 {
    if (!str_cmp(obj->questowner, vict->pcdata->switchname))
    {
	free_string(obj->questowner);
	obj->questowner = str_dup(buf);
    }
    if (!str_cmp(obj->questmaker, vict->pcdata->switchname))
    {
	free_string(obj->questmaker);
	obj->questmaker = str_dup(buf);
    }
 }

 free_string(vict->name);
 vict->name = str_dup(buf);
 free_string(vict->pcdata->switchname);
 vict->pcdata->switchname = str_dup(buf);

 act("#6You feel like $n!#n", vict, 0, 0, TO_CHAR);
 act("You turn the poor bastard into $t!", ch, buf, 0, TO_CHAR);

 return;
}

void do_immspar(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 char arg2[MIL];
 CHAR_DATA *p1, *p2;
 int attacks1;
 int attacks2;
 int a;
 long tot1 = 0;
 long tot2 = 0;
 int dam = 0;
 int hit1 = 0;
 int hit2 = 0;
 int status1;
 int status2;

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {
    stc("Please please 2 maxd players with the same gen who are in the same room, and type immspar (p1) (p2).\n\r", ch);
    return;
 }

 if (atoi(argument) == 0)
 {
    stc("Please choose the number of rounds to hold the spar for, after the victims of the spar.\n\r", ch);
    return;
 }

 if ((p1 = get_char_room(ch, arg1)) == NULL || (p2 = get_char_room(ch, arg2)) == NULL)
 {
    stc("One of them peeps aren't here!\n\r", ch);
    return;
 }

 
 status1 = p1->race;
 status2 = p2->race;

 if (p1->race > p2->race) p2->race = p1->race;
 else p1->race = p2->race;

 attacks1 = number_attacks(p1, p2);
 attacks2 = number_attacks(p2, p1);

 attacks1 *= atoi(argument);
 attacks2 *= atoi(argument);
balancing = TRUE;
 for (a = 0; a <= attacks1; a++)
 {
    p2->hit = p2->max_hit;
    p2->move = p2->max_move;
    p2->mana = p2->max_mana;
    dam = p2->hit;

    one_hit(p1, p2, get_eq_char(p1, WEAR_WIELD) ?
TYPE_HIT+get_eq_char(p1,WEAR_WIELD)->value[3] : TYPE_HIT, get_eq_char(p1,
WEAR_WIELD)
? 20 : 0);

 if (p2->hit != dam)  hit1++;

    tot1 += dam - p2->hit;
 }


 for (a = 0; a <= attacks2; a++)
 {
    p1->hit = p1->max_hit;
    p1->move = p1->max_move;
    p1->mana = p1->max_mana;
    dam = p1->hit;

    one_hit(p2, p1, get_eq_char(p2, WEAR_WIELD) ?
TYPE_HIT+get_eq_char(p2,WEAR_WIELD)->value[3]:TYPE_HIT, get_eq_char(p2, WEAR_WIELD)
? 20 : 0);

 if (p1->hit != dam)  hit2++;

    tot2 += dam - p1->hit;
 }

 sprintf(log_buf, "%s did a total of %ld damage, %s did %ld damage.\n\r",
	p1->name, tot1, p2->name, tot2);
 stc(log_buf, ch);
 sprintf(log_buf, "%s scored %d out of %d hits (avg %2.3f)\n\r",
	p1->name, hit1, attacks1, (atof(CHAR2(hit1)) / atof(CHAR2(attacks1)))*100);
 stc(log_buf, ch);
 sprintf(log_buf, "%s's average damage per hit is %f.\n\r",
	p1->name, atof(CHAR2(tot1))/atof(CHAR2(hit1)));
 stc(log_buf, ch);
 sprintf(log_buf, "%s scored %d out of %d hits (avg %2.3f)\n\r",
	p2->name, hit2, attacks2, (atof(CHAR2(hit2)) / atof(CHAR2(attacks2)))*100);
 stc(log_buf, ch);
 sprintf(log_buf, "%s's average damage per hit is %f.\n\r",
	p2->name, atof(CHAR2(tot2))/atof(CHAR2(hit2)));
 stc(log_buf, ch);
 sprintf(log_buf, "\n\rTo even out the damage modify the %s class by %f\n\r",
	class_table[class_lookup(p1->class)].class_name,
(atof(CHAR2(tot2))/atof(CHAR2(hit2)))/(atof(CHAR2(tot1))/atof(CHAR2(hit1))));
 stc(log_buf, ch);
 sprintf(log_buf, "OR OR OR OR OR\n\rModify the %s class's damage by %f.\n\r",
	class_table[class_lookup(p2->class)].class_name,
(atof(CHAR2(tot1))/atof(CHAR2(hit1)))/(atof(CHAR2(tot2))/atof(CHAR2(hit2))));
 stc(log_buf, ch);
balancing = FALSE;

stop_fighting(p1,TRUE);
if (p2->fighting || p2->position == POS_FIGHTING)
stop_fighting(p2,TRUE);
p1->fight_timer = 0;
p2->fight_timer = 0;
p1->race = status1;
p2->race = status2;

 return;
}

void do_daynight(CHAR_DATA *ch, char *argument)
{
 if (weather_info.sunlight == SUN_LIGHT)
 {
    stc("It is now night.\n\r", ch);
    weather_info.sunlight = SUN_DARK;
 }
 else
 {
    stc("It is now day.\n\r", ch);
    weather_info.sunlight = SUN_LIGHT;
 }
return;
}

void do_classme(CHAR_DATA *ch, char *argument)
{
	char arg1[MIL];

	argument = one_argument(argument, arg1);

	if ( IS_NPC(ch) )
                return;
        if (ch->class != 0)
        {
                send_to_char("You are already have a class.\n\r", ch);
                return;
        }
    if ( arg1[0] == '\0')
    {
    send_to_char( "Syntax: classme <class>.\n\r", ch );
    do_class(ch,"");
    stc("#7You may only classme into a class with a #2--#7 next to it.#n\n\r",ch);
        return;
    }
    sprintf(log_buf, "self %s", arg1);
    do_class(ch,log_buf);
return;
}

char *get_status_name(CHAR_DATA *ch)
{
 if (ch->level < 2)
    return "Newbie";
 else if (ch->level == 2)
    return "Loomer";
 else if (ch->level <= 6)
 {
    if (ch->race == 0)
	return "Dreamer";
    else if (ch->race <= 4 )
	return "Omen";
    else if (ch->race <= 9 )
	return "Ninto";
    else if (ch->race <= 14)  
	return "Deminint";
    else if (ch->race <= 19)
	return "Lesser spirit";
    else if (ch->race <= 24)
	return "Greater Spirit";
    else if (ch->race <= 29)
	return "Supreme Loomer";
    else if (ch->race <= 34)
	return "NightMare";
    else
	return "Omnipresent";
 }
 else
    if (ch->trust == 7)
	return "QuestMaker";
    else if (ch->trust == 8)
	return "Builder";
    else if (ch->trust == 9)
	return "Enforcer";
    else if (ch->trust == 10)
	return "Judge";
    else if (ch->trust == 11)
	return "High Judge";
    else if (ch->trust == 12)
	return "Implementor";
 else
    return "Unknown";
}

char *centerjob (char *fill, const char *txt, int maxlen )
{
 int Tot;
 int tofill;
 int iA;
 const char *txt2 = txt;
 static char buf[MSL];
 char Lik[MSL];

 strcpy(buf,"");
 tofill = maxlen;
 tofill -= strlen(txt);
 tofill += (strlen(txt) - strlen(bash_color(txt)));

 tofill /= 2;

 for (Tot = 0, iA = 0; iA < tofill && Tot < maxlen; iA++, Tot++)
    strcat(buf, fill);

 for ( ; *txt; txt++ )
 {
    sprintf(Lik, "%s%c", buf, *txt);
    strcpy(buf, Lik);
 }

 Tot += strlen(bash_color(txt2));

 for (iA = Tot; iA < maxlen; iA++)
    strcat(buf, fill);

 return buf;
}

char *name_funny( char *name, int start, int until )
{
 static char buf[MSL];
 char temp[MSL];
 int iA;

 strcpy(buf,"");

 for (iA = 0; iA < until && *name; iA++, name++)
 {
    sprintf(temp, "%s#%d%c", buf, start+iA, *name);
    strcpy(buf,temp);
 }

 if (*name)
    strcat(buf, name);

 return buf;
}

void do_watch(CHAR_DATA *ch, char *argument)
{
 int a = atoi(argument);
 DESCRIPTOR_DATA *d;

 if (*argument == '\0')
 {
    stc("Choose out of self, list or watch (number) to watch a descriptor.\n\r",ch);
    do_watch(ch, "list");
    return;
 }

 if (!str_cmp(argument, "self"))
 {
   if (!ch->desc->watching)
   {
	stc("You aren't watching anybody!\n\r", ch);
	return;
   }
   ch->desc->watching->watcher = NULL;
   ch->desc->watching = NULL;
   stc("You are no longer watching anybody.\n\r", ch);
   return;
 }


 if (!str_cmp(argument, "list"))
 {
   for (d = descriptor_list; d; d = d->next)
   {
	sprintf(log_buf, "%4d - %43s - Player: %s\n\r",
	    d->descriptor, d->host, d->original ?
d->original->pcdata->switchname : d->character ?
d->character->pcdata->switchname :
"Not Completely Connected..");
	stc(log_buf, ch);
   }
   return;
 }

 for (d = descriptor_list; d; d = d->next)
    if (d->descriptor == a) break;

 if (!d)
 {
    stc("That descriptor does not exist!\n\r", ch);
    stc("Choose out of:\n\r", ch);
    do_watch(ch, "list");
    return;
 }

 if (d->watcher)
 {
    stc("Someone is already watching him/her.\n\r", ch);
    return;
 }

 d->watcher = ch->desc;
 ch->desc->watching = d;

 stc("Watching.\n\r", ch);
 return;
}

GRANITE_DATA *granite_struct(GRANITE_DATA *hold, structure_dowhat WHAT)
{
 static GRANITE_DATA *free = NULL;
 GRANITE_DATA *retur;

 if (WHAT == sCreate)
 {
    if (free)
    {
        retur = free;
        free = free->next;
        retur->next = NULL;
        //log_string("1:)");
        return retur;
    }
    else
    {
        retur = alloc_perm(sizeof(GRANITE_DATA));
        retur->vnum = 0;
        retur->name = NULL;
        retur->next = NULL;
        //log_string("2:(");
        return retur;
    }
 }
 else
 if (WHAT == sFree)
 {
    if (!hold) return 0;
    hold->next = free;
    free = hold;
    hold->vnum = 0;
  //sprintf(log_buf, "freeing granite with name: %s",hold->name);
    //log_string(log_buf);
    free_string(hold->name);
    return 0;
 }
 else
 if (WHAT == sFreeList)
 {
    if (!hold) return 0;
    retur = hold->next;

    granite_struct(hold, sFree);

    return granite_struct(retur, sFreeList);
 }

 return 0;
}


void do_freevnums(CHAR_DATA *ch, char *argument)
{int iA, free = 0, start = 0, count = 0, needed = atoi(argument);

 for (iA = 0; iA < 30000; iA++)
 {
    if (!get_room_index(iA) && !get_obj_index(iA) && !get_mob_index(iA))
    { if (!free) start = iA;
      free++;
    }
    else if (free)
    {
	sprintf(log_buf, "VNUMS %7d-%7d    ", start, start+free);
      if (free >= needed)
      {
	stc(log_buf, ch);
	count++;
      }
	if (count % 3 == 2)
	{
	    count = 0;
	    stc("\n\r", ch);
	}
	start = 0;
	free = 0;
    }
 }

 stc("\n\r", ch);

 return;
}

char *nolarger(char *str, int len)
{
 static char buf[MSL];
strcpy(buf, str);
set_len(buf, len);
  return buf;
}

#define MAX_WHO_LEN (79)
#define MWL         (MAX_WHO_LEN)
void do_who(CHAR_DATA *ch, char *argument)
{
 DESCRIPTOR_DATA *d;
int race_len = 0;
int class_len = 0;
int name_len = 0;
int imm_name_len = 0;
int lord_len = 10;
int title_len = 0;
int imm_title_len = 0;
 int top_status = 0;
 int count = 0;
 char buf[MSL];
 char buf2[MSL];
 int stat;
 bool fClass = FALSE;
 int iClass = 0;
 CHAR_DATA *vict;
 bool hasimm = FALSE;
 bool hasmort = FALSE;
 bool hasavatar = FALSE;

 if (class_lookupn(argument) != -1)
 {
    fClass = TRUE;
    iClass = class_table[class_lookupn(argument)].class;
 }

 for (d = descriptor_list; d; d = d->next)
 {
    if (d->connected > CON_PLAYING) continue;
    if ((vict = d->character) == NULL)
	continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!fClass && *argument && str_prefix(argument, vict->pcdata->switchname))
	continue;
if (strlen(bash_color(vict->pcdata->switchname)) > imm_name_len &&
vict->level >= 7)
    imm_name_len = strlen(bash_color(vict->pcdata->switchname));
if (strlen(bash_color(vict->lord)) > lord_len && vict->level >= 7)
    lord_len = strlen(bash_color(vict->lord));
if (strlen(bash_color(get_status_name(vict))) > race_len && vict->level <=
6)
    race_len = strlen(bash_color(get_status_name(vict)));
if (strlen(bash_color(get_class_name(vict))) > class_len && vict->level <=
6)
    class_len = strlen(bash_color(get_class_name(vict)));
if (strlen(bash_color(vict->pcdata->switchname)) > name_len && vict->level
<= 6)
    name_len = strlen(bash_color(vict->pcdata->switchname));

    count++;

    if (vict->level > 6) hasimm = TRUE;
    else if (vict->level < 3) hasmort = TRUE;
    else hasavatar = TRUE;

    if (vict->level <= 2) continue;

    if (!can_see(ch, vict) && vict->level > 6) continue;

    if (vict->race > top_status)
	top_status = vict->race;
 }

 race_len += 2;
 class_len += 2;
 name_len += 1;
 imm_name_len += 6;
 lord_len += 2;

 title_len = MWL-6-race_len-class_len-2-name_len-2;
 imm_title_len = MWL-6-lord_len-5-imm_name_len-3;

 sprintf(buf, "%s",
    name_funny("Midnight ", 4, 4));
 strcat(buf, name_funny("Dreams", 4, 4));
 stc( centerjob("#7/-_-\\", buf, 12 + strlen(bash_color(buf))), ch);
 stc("/-\n\r\n\r", ch);
if (hasimm) {
 stc( centerjob("#4(#6*#4)", "Immortalz", 23+strlen("Immortalz")), ch);
 stc( "\n\r\n\r", ch);

 for (d = descriptor_list; d; d = d->next)
 {
    if (d->connected > CON_PLAYING)
        continue;
    if ((vict = d->character) == NULL)
	continue;
    if (vict->level <= 6)
	continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!fClass && *argument && str_prefix(argument, vict->pcdata->switchname)) continue;

    if (!can_see(ch, vict)) continue;

    sprintf(buf, "#1[#6IMM#7[>#5%s#7<]#4<#6%2d#4> ",
	centerjob("_", vict->lord, lord_len),
	vict->trust);

    sprintf(buf2, "%s #2%-*s#1]\n\r",
	centerjob(" ", name_funny(vict->pcdata->switchname, 1,number_range(2,3)), imm_name_len),
imm_title_len + (strlen(vict->pcdata->title) - strlen(bash_color(vict->pcdata->title))),
bah(vict->pcdata->title,imm_title_len));
    strcat(buf,buf2);

    stc(buf, ch);
 }
}
if (hasavatar) {
 stc("\n\r", ch);
 sprintf(buf, "%s", name_funny("Avatarz", 1, 3));
 stc("#6-", ch);
 stc( centerjob("#6_--", buf, 23+strlen(bash_color(buf))), ch);
 stc("#6-", ch);
 stc( "\n\r\n\r", ch);

 for (stat = top_status; stat >= 0; stat--)
 {
    for (d = descriptor_list; d; d = d->next)
    {
	if (d->connected > CON_PLAYING)
	    continue;
	if ((vict = d->character) == NULL)
	    continue;
	if (vict->level <= 2 || vict->level > 6)
	    continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!fClass && *argument && str_prefix(argument,
vict->pcdata->switchname)) continue;
	if (vict->race != stat)
	    continue;

    sprintf(buf,"#1[#2%-3d#7[>#3%s",
	vict->race,
	centerjob(" ",get_status_name(vict), race_len));
    sprintf(buf2,"#7-#6%s#7[>#2%-*s %-*s#1]\n\r",
	centerjob(" ",get_class_name(vict), class_len),
	name_len,
	vict->pcdata->switchname,
title_len + (strlen(vict->pcdata->title) - strlen(bash_color(vict->pcdata->title))),
bah(vict->pcdata->title,title_len));
    strcat(buf,buf2);

    stc(buf, ch);
    }
 }
}
if (hasmort) {
 stc("\n\r", ch);
 sprintf(buf, "%s", name_funny("Mortalz", 1, 3));
 stc("#6-", ch);
 stc( centerjob("#6_--", buf, 23+strlen(bash_color(buf))), ch);
 stc("#6-", ch);
 stc( "\n\r\n\r", ch);

 for (d = descriptor_list; d; d = d->next)
 {
    if (d->connected > CON_PLAYING)
	continue;
    if ((vict = d->character) == NULL)
	continue;
if (fClass && !IS_SET(vict->class, iClass)) continue;
if (!fClass && *argument && str_prefix(argument,
vict->pcdata->switchname)) continue;
    if (vict->level > 2)
	continue;

    sprintf(buf,"#1[#2%-3d#7[>#3%s",
	vict->race,
	centerjob(" ",get_status_name(vict), race_len));
    sprintf(buf2,"#7-#6%s#7[>#2%-*s %-*s#1]\n\r",
	centerjob(" ",get_class_name(vict), class_len),
	name_len,
	vict->pcdata->switchname,
title_len + (strlen(vict->pcdata->title) - strlen(bash_color(vict->pcdata->title))),
bah(vict->pcdata->title,title_len));
strcat(buf, buf2);

    stc(buf, ch);
 }
}
 sprintf(buf, " #3%d #2visible players are online. ", count);
 stc( centerjob("#7/-_-\\", buf, 9 + strlen(bash_color(buf))), ch);
 stc("#7-\n\r\n\r", ch);

 return;
}

void set_len(char *buff, int len)
{
 int iA;

 for (iA = 0;*buff &&  iA <len; buff++)
 {
    if (*buff == '#' && *(buff+1) != '#')
	; else iA++;
 }
 *buff = '\0';

 return;
}

void do_beep(CHAR_DATA *ch, char *argument)
{
//(imm only)
 char arg1[MIL];
 CHAR_DATA *vict;

 argument = one_argument(argument, arg1);

 if((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They're not here!\n\r", ch);
    return;
 }

 sprintf(log_buf, "\a%s\a", argument);
 stc(log_buf, vict);

 return;
}

void do_die(CHAR_DATA *ch, char *argument)
{
 if (!IS_NPC(ch)) 
{stc("Huh?\n\r", ch);return;}
 if (!ch->wizard) return;

 act("$n falls to the ground, dead, from the call of $m master.",
	ch,0,0,TO_ROOM);
 stc("You fall to the ground, dead, by the call of your master.", ch);
 stc("YOU ARE DEAD!!!\n\r", ch);

 extract_char(ch,TRUE);
 return;
}

void do_set(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
if (1==1||IS_NPC(ch)) return;
 argument = one_argument(argument, arg1);

 if (arg1[0] == '\0' || !*argument)
 {
    stc("The only current option is: decap (sets your personal decap message)
\rcosts 1k qps.\n\r", ch);
    stc("Format: set [option] [message]\n\r", ch);
    return;
 }

 if (!str_cmp(arg1, "decap"))
 {
    if (ch->pcdata->quest < 1000)
    {
	stc("You need 1000 quest points to set that!\n\r", ch);
	return;
    }

    if (!isname("$n",argument) || !isname("$N",argument))
    {
	stc("The decap message must contain $n for your name and $N for your victims name.\n\r",ch);
	return;
    }

    ch->pcdata->quest -= 1000;

    free_string(ch->decap);
    ch->decap = str_dup(argument);
    stc("Set.\n\r", ch);
    return;
 }

 do_set(ch,"");
 return;
}

void do_unrestore(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict = get_char_world(ch,argument);

 if (!str_cmp(argument, "all"))
 {
  DESCRIPTOR_DATA *d;
 for (d = descriptor_list; d; d = d->next)
 {
    if (!d->character || d->connected > CON_PLAYING || IS_NPC(d->character))
	continue;
    do_unrestore(ch,d->character->pcdata->switchname);
 }
 return;
 }
 if (!*argument || vict == NULL)
 {
    stc("Unrestore who??\n\r", ch);
    return;
 }

 vict->hit = 1;
 vict->mana = 1;
 vict->move = 1;

 sprintf(log_buf, "#2%s #6Has #5been #3UN-#1RESTORED#7!! #*K#*I#*L#*L#*!#n",
    IS_NPC(vict) ? vict->short_descr : vict->pcdata->switchname);
 do_info(ch, log_buf);
 return;
}

void do_gsocial(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 argument = one_argument(argument, arg1);

if (ch->act & PLR_SILENCE)
{
    stc("You are silenced..shh and shush..\n\r", ch);
    return;
}

if (ch->deaf & CHANNEL_GSOCIAL)
{
    stc("But you are deaf to gsocials!\n\r", ch);
    return;
}

if (arg1[0] == '\0')
{
    stc("Choose a social to use!\n\r", ch);
    return;
}

 if (! check_social(ch, arg1, argument,TYPE_GSOCIAL))
 {
    stc("That social doesn't exist!\n\r", ch);
    return;
 }

 return;
}



void do_delete(CHAR_DATA *ch, char *argument)
{
 if (IS_NPC(ch)) return;

 if (ch->flag3 & AFF3_DELETE && !strcmp( crypt(argument,
ch->pcdata->pwd), ch->pcdata->pwd))
 {
    char buf[MIL];
    char buf2[MIL];
    char buf3[MIL];

    sprintf(log_buf, "%s has deleted.", ch->pcdata->switchname);
    log_string(log_buf);

    strcpy(buf,"../player/");
    strcat(buf,capitalize(ch->pcdata->switchname));
    strcpy(buf2,"../player/store/");
    strcat(buf2,capitalize(ch->pcdata->switchname));
    strcpy(buf3,"../player/backup/");
    strcat(buf3,capitalize(ch->pcdata->switchname));

    stc("#6You have been deleted.\n\r", ch);
    do_quit(ch,"");

    unlink(buf);
    unlink(buf2);
    unlink(buf3);
    return;
 }
 else if (!strcmp( crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd))
 {
    ch->flag3 |= AFF3_DELETE;
    stc("#3You have been given the delete flag. This means that if you type
\rdelete (your password) one more time, you will be deleted immediately.
\rto cancel this, type delete (anything else)\n\r", ch);
    return;
 }
 else if (ch->flag3 & AFF3_DELETE)
 {
    stc("#3Delete flag removed.\n\r", ch);
    REMOVE_BIT(ch->flag3, AFF3_DELETE);
    return;
 }
 else
 {
    stc("#3To delete your character, type delete (your password)
\rand then another time to confirm.\n\r", ch);
    return;
 }
 return;
}

void do_pdelete(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char buf[MIL];
 char buf2[MIL];
 char buf3[MIL];


 if (!*argument)
 {
    stc("Delete whom?\n\r", ch);
    return;
 }

 if ((vict = get_char_world(ch, argument)) == NULL)
 {
    strcpy(buf,"../player/");
    strcat(buf,capitalize(argument));
    strcpy(buf2,"../player/store/");
    strcat(buf2,capitalize(argument));
    strcpy(buf3,"../player/backup/");
    strcat(buf3,capitalize(argument));

    stc("They have been deleted.\n\r", ch);

    unlink(buf);
    unlink(buf2);
    unlink(buf3);

    return;
 }
else if (IS_NPC(vict))
{
    stc("Not on NPC's!\n\r", ch);
    return;
}
    strcpy(buf,"../player/");
    strcat(buf,capitalize(vict->pcdata->switchname));
    strcpy(buf2,"../player/store/");
    strcat(buf2,capitalize(vict->pcdata->switchname));
    strcpy(buf3,"../player/backup/");
    strcat(buf3,capitalize(vict->pcdata->switchname));

    stc("#6You have been deleted.\n\r", vict);
    stc("#7Deleted.\n\r", ch);
    vict->fighting = NULL;
    vict->position = POS_STANDING;
    do_quit(vict,"");

    unlink(buf);
    unlink(buf2);
    unlink(buf3);

    return;
}

void do_eatmortal(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 OBJ_DATA *obj;
 OBJ_DATA *obj2;

 if (IS_NPC(ch)) return;

 if (!*argument)
 {
    stc("Eat whom?\n\r", ch);
    return;
 }

 if ((vict = get_char_world(ch, argument)) == NULL)
 {
    stc("They aren't close enough to your stomach! (not in this realm)\n\r", ch);
    return;
 }

if ((obj = get_obj_carry(ch, "stomach")) == NULL)
{ obj = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM),0);
 obj_to_char(obj, ch);}
 obj2 = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM),0);


 sprintf(log_buf, "#4%s's#1 stomach#n", ch->pcdata->switchname);
 free_string(obj->short_descr);
 obj->short_descr = str_dup(log_buf);

 sprintf(log_buf, "%s stomach", ch->pcdata->switchname);
 free_string(obj->name);
 obj->name = str_dup(log_buf);

 obj->item_type = ITEM_CONTAINER;

 char_from_room(vict);

 obj_to_obj(obj2, obj);

 sprintf(log_buf, "#2Torn up pieces of %s#n", PERS(vict,ch));
 free_string(obj2->short_descr);
 obj2->short_descr = str_dup(log_buf);

 sprintf(log_buf, "%s pieces torn", ch->pcdata->switchname);
 free_string(obj2->name);
 obj2->name = str_dup(log_buf);

 sprintf(log_buf, "#2Some torn up pieces of %s are here.\n\r",
	PERS(vict,ch));
 free_string(obj2->description);
 obj2->description = str_dup(log_buf);

 sprintf(log_buf, "\rYou are in here for sins against the gods, and you will remain 
\runtil told to move. HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\rHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA
\r Sucks 2 B U - %s\n\r", ch->pcdata->switchname);
 free_string(obj->description);
 obj->description = str_dup(log_buf);


    vict->pcdata->obj_vnum = 0;
    obj2->chobj = vict;
    vict->pcdata->chobj = obj2;
    SET_BIT(vict->affected_by, AFF_POLYMORPH);
    SET_BIT(vict->extra, EXTRA_OSWITCH);
    free_string(vict->morph);
    vict->morph = str_dup(obj2->short_descr);
do_help(vict, "---head");
 return;
}

void do_curseditem(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char arg1[MIL];
 OBJ_DATA *tell;

argument = one_argument(argument, arg1);

 if (!*arg1 || !*argument)
 {
    stc("Send what to whom? ie curseditem smoo [tell/trans/restore]\n\r", ch);
    return;
 }

 if ((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 tell = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM),0);

 free_string(tell->short_descr);
 sprintf(log_buf, "a %s", argument);
 tell->short_descr = str_dup(log_buf);
 free_string(tell->name);
 tell->name = str_dup("");
 if (!IS_NPC(vict))
 {
    free_string(tell->questowner);
    tell->questowner = str_dup(vict->pcdata->switchname);
 }

 tell->extra_flags |= ITEM_NODROP;
tell->timer = 100;
 obj_to_char(tell,vict);
 act("$p appears in your hands in a swirl of smoke.",vict,tell,0,TO_CHAR);
 act("You send them a none-named, none-droppable $t.",ch,argument,0,TO_CHAR);
 return;
}

AFFECT_DATA blank_aff(int sn)
{
 AFFECT_DATA af;

 af.type = sn;
 af.duration = 0;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.bitvector = 0;

 return af;
}
