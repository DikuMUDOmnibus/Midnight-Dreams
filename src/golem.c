/*'SMOO-CLASS.*/
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

#define STC(b,c) stc(b,c); stc("\n\r", c);

char golem_ranks[11][20] =
{"",

 "Rok", "Ankh",  "Mith", "Surreal", "Tith",
 "Lark","Mentro","Hard", "Soft",    "Tiny"};
void Hey_Look_A_Realistic_Bomb(CHAR_DATA *ch, ROOM_INDEX_DATA *r, int potent );
void emerald_convert(CHAR_DATA *);
void obsidian_convert(CHAR_DATA *);
void crystal_convert(CHAR_DATA *);

#define REQUIRETACT(ch, lev)	\
	if ((ch)->pcdata->powers[TACTICALS] < (lev))\
	{\
	    sprintf(log_buf, "You need %d in tacticals to use that!\n\r",\
		(lev));\
	    stc(log_buf, ch);\
	    return;\
	}
#define REQMAGI(ch, lev)	\
	if ((ch)->pcdata->powers[MAGITOS] < (lev))\
	{\
	    sprintf(log_buf, "You need %d in MagiTos to use that!\n\r",\
		(lev));\
	    stc(log_buf, ch);\
	    return;\
	}
#define SS(skill) skill_lookup((skill))
#define COMBO(gsn,dam) \
	{if (!ch->fighting) {stc("You aren't fighting anybody..\n\r",ch);return;}\
	if (number_range(1,100) <= 19) {damage(ch, ch->fighting, 0, \
	SS(gsn));}else {\
	damage(ch, ch->fighting, dam, SS(gsn));\
	if (IS_INFORM(ch,GOLEM_CRYSTAL)&&ch->pcdata->powers[MAGITOS] >= 10)\
	WAIT_STATE(ch,skill_table[SS(gsn)].beats/1.4); else\
	WAIT_STATE(ch, skill_table[SS(gsn)].beats);}}	

#define IS_MOVE(ch, move) (ch)->moves & (move)
#define SET_MOVE(ch, move) (ch)->moves |= (move)


void do_sumosplash(CHAR_DATA *ch, char *argument)
{
 int dam = number_range(250,500);
 if (IS_STONE(ch)) dam *= 2.3;
 if (!IS_CLASS(ch, CLASS_GOLEM)) {do_huh(ch,"");return;}

 REQUIRETACT(ch,1);

 COMBO("sumosplash", dam)

 if (IS_MOVE(ch,OBSIDIAN_MOVE2))
    SET_MOVE(ch,OBSIDIAN_MOVE3);
 else if (IS_MOVE(ch,EMERALD_MOVE2))
    {ch->moves = 0;
if (ch->pcdata->powers[MAGITOS] >= 3)
emerald_convert(ch);}
 else {ch->moves = 0;SET_MOVE(ch,EMERALD_MOVE1);}

 return;
}

void do_bazmapunch(CHAR_DATA *ch, char *argument)
{
 int dam = number_range(250,500);
 if (IS_STONE(ch)) dam *= 2.3;
 if (!IS_CLASS(ch, CLASS_GOLEM)) {do_huh(ch,"");return;}

 REQUIRETACT(ch,2);

 COMBO("bazmapunch", dam)

 if (IS_MOVE(ch, CRYSTAL_MOVE2))
    {ch->moves = 0;
if (ch->pcdata->powers[MAGITOS] >= 5)
crystal_convert(ch);}
 else if (IS_MOVE(ch,OBSIDIAN_MOVE3))
    {SET_MOVE(ch,OBSIDIAN_MOVE4);}
 else if (IS_MOVE(ch,EMERALD_MOVE1))
    {SET_MOVE(ch,EMERALD_MOVE2);}
 else ch->moves = 0;

 return;
}

void do_neckcrack(CHAR_DATA *ch, char *argument)
{
 int dam = number_range(250,500);
 if (IS_STONE(ch)) dam *= 2.3;
 if (!IS_CLASS(ch, CLASS_GOLEM)) {do_huh(ch,"");return;}

 REQUIRETACT(ch,10);

 COMBO("neckcrack", dam)

if (IS_MOVE(ch, OBSIDIAN_MOVE5))
{ch->moves = 0;
if (ch->pcdata->powers[MAGITOS] >= 7)
obsidian_convert(ch);}
else if (IS_MOVE(ch, OBSIDIAN_MOVE4))
SET_MOVE(ch,OBSIDIAN_MOVE5);
else
 {ch->moves = 0;}

 return;
}

void do_backslash(CHAR_DATA *ch, char *argument)
{
 int dam = number_range(50,700);
 if (IS_STONE(ch)) dam *= 2.3;
 if (!IS_CLASS(ch, CLASS_GOLEM)) {do_huh(ch,"");return;}

 REQUIRETACT(ch,7);

 COMBO("backslash", dam)

 {ch->moves = 0;SET_MOVE(ch,OBSIDIAN_MOVE1);}

 return;
}

void do_elbowbash(CHAR_DATA *ch, char *argument)
{
 int dam = number_range(350,625);
 if (IS_STONE(ch)) dam *= 2.3;
 if (!IS_CLASS(ch, CLASS_GOLEM)) {do_huh(ch,"");return;}

 REQUIRETACT(ch,4);

 COMBO("elbowbash", dam)

if (IS_MOVE(ch,OBSIDIAN_MOVE1))
 SET_MOVE(ch,OBSIDIAN_MOVE2);
else
 {ch->moves = 0; SET_MOVE(ch, CRYSTAL_MOVE1);}

 return;
}

void do_groinkick(CHAR_DATA *ch,char *argument)
{
 int dam = number_range(450,725);
 if (IS_STONE(ch)) dam *= 3.2;
 if (!IS_CLASS(ch, CLASS_GOLEM)) {do_huh(ch,"");return;}

 REQUIRETACT(ch,5);

 if (ch->fighting && ch->fighting->sex == SEX_FEMALE)
{
  stc("They don't have the proper groin mechanisms to make that HURT!\n\r", ch);
}else
  COMBO("groinkick", dam)

 if (IS_MOVE(ch, CRYSTAL_MOVE1))
     SET_MOVE(ch, CRYSTAL_MOVE2);
 else ch->moves = 0;

 return;
}


void crystal_convert( CHAR_DATA * ch)
{
 act("$n's body glows brightly..",ch,0x0,0x0,TO_ROOM);
 act("Your body glows brightly..",ch,0x0,0x0,TO_CHAR);
 if (IS_STONE(ch)) {ch->countdown = 1; crystal_change(ch);} else
 ch->countdown = 4;
 ch->convert_to = GOLEM_CRYSTAL;

 return;
}

void crystal_change(CHAR_DATA *ch)
{
 char buf[200];
 if ((!ch->fighting || ch->position != POS_FIGHTING) &&
!IS_SET(ch->pcdata->powers[0], GOLEM_CRYSTAL))
 {
    ch->countdown = 0;ch->convert_to = 0;
    return;
 }
if (ch->countdown <= 0 && !IS_SET(ch->pcdata->powers[0],GOLEM_CRYSTAL))
    return;

 if (IS_SET(ch->pcdata->powers[0], GOLEM_CRYSTAL))
 {
     if (ch->wait2 > 0) return;
     act("#5The crystal shell surrounding $n shatters and $e pops back to life!#n",ch,0x0,0x0,TO_ROOM);
     act("#5The crystal shell surrounding your shatters and you pop back to life!#n",ch,0x0,0x0,TO_CHAR);
     REMOVE_BIT(ch->pcdata->powers[0],GOLEM_CRYSTAL);
     return;
 }

 if (--ch->countdown > 0)
 {
    sprintf(buf,"#%d$n's body glows even brighter..",ch->countdown);
    act(buf,ch,0x0,0x0,TO_ROOM);
    sprintf(buf,"#%dYour body glows even brighter..",ch->countdown);
    act(buf,ch,0x0,0x0,TO_CHAR);
 }
 else
 {if (IS_STONE(ch)) NO_STONE(ch);
    act("#6Your body unleashes a BURST of bright energy and you solidify into a crystal!#n",ch,0x0,0x0,TO_CHAR);
    act("#6$n's body unleashes a BURST of bright energy and $e solidifies into a crystal!#n",ch,0x0,0x0,TO_ROOM);
    SET_BIT(ch->pcdata->powers[0], GOLEM_CRYSTAL);
    ch->wait2 += PULSE_VIOLENCE *7;
   WAIT_STATE(ch->fighting, PULSE_VIOLENCE *2);
 }

return;
}

void emerald_convert( CHAR_DATA * ch)
{
 if (ch->move < (ch->max_move /2))
 {
    stc("#2You don't feel the POWER!\n\r{x", ch);
    ch->moves = 0;
    return;
 }

 ch->move -= (ch->max_move /2);

 act("#2$n bursts in a bright green light and then as $e dims, $e turns into a bright green rock!#n",
 	ch,0x0,0x0,TO_ROOM);
 act("#2You burst with a bright green light, then you dim and become solidified.#n",
	ch,0x0,0x0,TO_CHAR);
 NO_STONE(ch);
 SET_BIT(ch->pcdata->powers[0], GOLEM_EMERALD);
 ch->wait2 += 5 * PULSE_VIOLENCE;
 return;
}

void emerald_change( CHAR_DATA * ch)
{
 if (IS_SET(ch->pcdata->powers[0], GOLEM_EMERALD))
 {
     if (ch->wait2 > 0) return;
    act("#2$n's green hue fades slowly as he become more vulnerable.#n",
	ch,0x0,0x0,TO_ROOM);
    act("#2Your green hue fades slowly and you become more vulnerable.#n",
	ch,0x0,0x0,TO_CHAR);
     NO_STONE(ch);
     return;
 }
return;
}


void obsidian_convert( CHAR_DATA * ch)
{
 act("$n's body fades..",ch,0x0,0x0,TO_ROOM);
 act("Your body fades..",ch,0x0,0x0,TO_CHAR);
 if (IS_STONE(ch)) {ch->countdown = 1; obsidian_change(ch);} else
 ch->countdown = 10;
 ch->convert_to = GOLEM_OBSIDIAN;

 return;
}



void obsidian_change(CHAR_DATA *ch)
{
 char buf[200];
 if ((!ch->fighting || ch->position != POS_FIGHTING) &&
!IS_SET(ch->pcdata->powers[0], GOLEM_OBSIDIAN))
 {
    ch->countdown = 0;ch->convert_to = 0;
    return;
 }
if (ch->countdown <= 0 && !IS_SET(ch->pcdata->powers[0],GOLEM_OBSIDIAN))
    return;

 if (IS_SET(ch->pcdata->powers[0], GOLEM_OBSIDIAN))
 {
     if (ch->wait2 > 0) return;
     act("$n suddenly appears more visable..",ch,0x0,0x0,TO_ROOM);
     act("Your eyes suddenly make everything appear much brighter..",
	ch,0x0,0x0,TO_CHAR);
     REMOVE_BIT(ch->pcdata->powers[0],GOLEM_OBSIDIAN);
     return;
 }

 if (--ch->countdown > 0)
 {
    sprintf(buf,"#%d$n's body fades away..",ch->countdown);
    act(buf,ch,0x0,0x0,TO_ROOM);
    sprintf(buf,"#%dYour body fades away..",ch->countdown);
    act(buf,ch,0x0,0x0,TO_CHAR);
 }
 else
 {if (IS_STONE(ch)) NO_STONE(ch);
    act("$n's black hands harden and become deadly fists, $m body hardens making him look like a rock.",
	ch,0x0,0x0,TO_ROOM);
    act("Your black hands harden and become deadly fists, your body hardens and you feel invincible.",
	ch,0x0,0x0,TO_CHAR);

    SET_BIT(ch->pcdata->powers[0], GOLEM_OBSIDIAN);
    ch->wait2 += PULSE_VIOLENCE *8;

   WAIT_STATE(ch->fighting, PULSE_VIOLENCE *2);
 }

return;
}

void do_solidify( CHAR_DATA *ch, char *argument )
{
 char arg[MSL];
 argument = one_argument(argument, arg);
 if (!IS_CLASS(ch, CLASS_GOLEM))
 {
    stc("Nah, thats ok, your not too hard headed.\n\r", ch);
    return;
 }

 if (arg[0] == '\0')
 {
    stc("You can solidify into a red ruby..\n\r", ch);
    stc("A shiny gold ticking bomb..\n\r", ch);
    stc("You can create or go to portals with granite..\n\r", ch);
    return;
 }

 if (!str_cmp(arg, "granite"))
 {char buf[MSL];
  argument = one_argument(argument, buf);

  REQMAGI(ch, 6)

  if (!str_cmp(buf, "project"))
  {
    if (ch->fighting)
    {
	stc("You can't concentrate hard enough!\n\r", ch);
	return;
    }
    act("$n walks into the astral world.", ch, 0, 0, TO_ROOM);
    act("You step into the astral world.", ch, 0, 0, TO_CHAR);
    char_from_room(ch);
    char_to_room(ch,get_room_index(8));
    ch->pcdata->powers[2] = IN_ASTRAL;
    do_look(ch,"auto");
    return;
  }

  else  if (!str_cmp(buf, "list"))
  {
	GRANITE_DATA *d;
	stc("Your current portals are:#*\n\r", ch);
	for (d = ch->influx; d; d = d->next)
	{
	    stc(d->name, ch);
	    stc("\n\r#*", ch);
	}
	stc("#n", ch);
  return;
  }

  else if (!str_cmp(buf, "delete"))
  {
     GRANITE_DATA *d, *prev = NULL;

      for (d = ch->influx; d; prev = d,d = d->next)
      {
	if (!str_cmp(bash_color(d->name), bash_color(argument)))
	{
	    if (!prev)
		ch->influx = ch->influx->next;
	    else
		prev->next = d->next;
	    granite_struct(d, sFree);
	    stc("Deleted.\n\r", ch);
	    return;
	}
      }
      stc("Not found. Type solid granite list.\n\r", ch);
      return;
  }

  else if (!str_cmp(buf, "create"))
  {GRANITE_DATA *d;int lala = 0;

    if (!ch->in_room)
    {
	stc("Your not in a room inside the REAL plane..\n\r", ch);
	return;
    }

{int count = 0;
for (d = ch->influx; d; d = d->next)
    count++;
if (count > 50)
{
    stc("Nah,- thats ok, You already have 50..\n\r", ch);
    return;
}}
if (!str_prefix("woha: ",argument))
{
argument = one_argument(argument,arg);
argument = one_argument(argument,arg);
lala = atoi(arg);
}
    if (*argument == '\0')
    {
	stc("Choose a name to name this astral portal.\n\r", ch);
	return;
    }

    d = granite_struct(0,sCreate);
    d->vnum = lala > 0 ? lala : ch->in_room->vnum;
    d->name = str_dup(argument);
    d->next = NULL;

    {GRANITE_DATA *d2;
	for (d2 = ch->influx; d2 && d2->next; d2 = d2->next)
	    ;
	if (!d2)
	    ch->influx = d;
	else
	    d2->next = d;}

  stc(d->name, ch);
  stc(" created.\n\r", ch);
  return;
  }
 else
 {
    stc("Please choose out of:
\rsolid granite create/list/project/delete\n\r", ch);
    return;
 }
 }

 if (!str_cmp(arg, "gold"))
 {
    REQMAGI(ch, 4)
    act("#3$n reshapes into a giant ball, made from gold.#n",
	ch,0,0,TO_ROOM);
    stc("#3You reshape into a giant ball, made from gold.#n\n\r",
	ch);

    act("#3You hear a gentle, 'Tick TIck. TiCK TiCk'#n",
	ch,0,0,TO_ROOM);
    stc("Tick tick. Tick tick.\n\r", ch);

    if (IS_STONE(ch)) ch->countdown = 1; else
    ch->countdown = 5;
    if (IS_STONE(ch)) NO_STONE(ch);
    SET_BIT(ch->pcdata->powers[0], GOLEM_GOLD);

    ch->morphy = TRUE;
    free_string(ch->morph);
    ch->morph = str_dup("#3A golden, ticking bomb#n");

    return;
 }
    

 if (!str_cmp(arg, "ruby"))
 {

    REQMAGI(ch, 1)
ch->convert_to = 0;
ch->countdown = 0; 
    if (IS_STONE(ch))
	NO_STONE(ch);

    act("#1$n solidifies into a large red rock.", ch, 0, 0, TO_ROOM);
    act("#1$n the large ruby starts shining in the $t#n", ch, weather_info.sunlight == SUN_DARK ? "#0night" : "#3day", 0, TO_ROOM);
    stc("#1You solidify into a large red rock and start shining.\n\r", ch);

    ch->morphy = TRUE;
    SET_BIT(ch->pcdata->powers[0], GOLEM_RUBY);
    SET_MOVE(ch, IN_RUBY);

    sprintf(log_buf, "#1A Large Red Stone#n");

    free_string(ch->morph);
    ch->morph = str_dup(log_buf);

    return;
 }

 do_solidify(ch,"");
 return;
}

void gold_change(CHAR_DATA *ch)
{static int hehe = 0;

if (ch->countdown <= 0 )
    return;

if (++hehe == 1) return;
else hehe = 0;

 if (--ch->countdown > 0)
 {
    act("#3You hear a gentle, 'Tick TIck. TiCK TiCk'#n",
	ch,0,0,TO_ROOM);
    stc("Tick tick. Tick tick.\n\r", ch);
 }
 else if (ch->countdown <= 0)
 {
    act("#3Golden slivers spurt out in every direction, damaging everything in its sight and",
	ch,0,0,TO_ROOM);
    act("#1A large fireball starts twirling around in the air and suddenly flashes in every direction.#n",
	ch,0,0,TO_ROOM);
    act("#nYou completely #3BURST#n in every direction sending #3Golden Slivers#n passing throughout the rooom, and",
	ch,0,0,TO_CHAR);
    act("#1Fire starts twirling around in the room and then flashes in every direction.#n",
	ch,0,0,TO_CHAR);
    Hey_Look_A_Realistic_Bomb(ch, ch->in_room, number_range(3,4));
    act("#3The Gold around $n is #7rusted#3 and fades away.#n",
	ch,0,0,TO_ROOM);
    act("#3The gold around you becomes #7rusted#3 and fades away.#n",
	ch,0,0,TO_CHAR);
    ch->morphy = FALSE;
    NO_STONE(ch);
 }

return;
}

void Hey_Look_A_Realistic_Bomb(CHAR_DATA *ch, ROOM_INDEX_DATA *r, int potent )
{
 CHAR_DATA *vch, *vch_next;
 int a;

 if (!r) return;

 if (potent == 0)
 {
    if (ch->in_room)
	char_from_room(ch);
    char_to_room(ch,r);
    ch->wait = PULSE_VIOLENCE*3.5;
    return;
 }

 for (vch = r->people; vch; vch = vch->next_in_room)
     stc("#3Golden splits fly in and out of the room followed by a #1thrash of fire.#n\n\r",
    vch);

nodamcap = TRUE;

 for (vch = r->people; vch; vch = vch_next)
 {
    vch_next = vch->next_in_room;

    stc("#3Golden splitners stab into your neck spilling #1blood for the burning fire to #6evaporate.#n\n\r", vch);

    damage(ch, vch, IS_NPC(vch) ? ch->hit *2 : ch->hit / 28, skill_lookup("Gold Golem"));
 }

nodamcap = FALSE;

 for (a = 0; a < 6; a++)
 {
    if (r->exit[a] && r->exit[a]->to_room)
	Hey_Look_A_Realistic_Bomb(ch, r->exit[a]->to_room, potent-1);
 }

 return;
}

char *get_astral_room(CHAR_DATA *ch)
{
 char buf[MSL*6];
 char *point = buf;
 int count = 0, a = 0;
 GRANITE_DATA *d;

 for (d = ch->influx; d; d = d->next)
    count++;

sprintf(buf,"#5Welcome to your portal chambers, %s %s. This room is covered
\r#5in granite and has mystical entrances to anywhere.\n\r",
golem_ranks[ch->generation], ch->pcdata->switchname);
#define pid(numb) get_d(ch->influx,ch->pcdata->powers[2]+numb)

for (a = 0; a < 4; a++) {
 if (pid(a))
 {
    sprintf(log_buf, "#3To the %s#3 you can find #2%s#2#n\n\r", dir_name[a], pid(a)->name);
    strcat(buf,log_buf);
 }
}
if (pid(4))
    strcat(buf, "#1More portals are continued UP\n\r");
if (ch->pcdata->powers[2] > 4)
    strcat(buf, "#4More portals are continued DOWN.\n\r");
strcat(buf,"#n");

 return point;
}

GRANITE_DATA *get_d(GRANITE_DATA *list, int count)
{
 int iA = 0;
 for (list = list; list; list = list->next)
    if (++iA == count) break;

 return list;
}

void astral_move(CHAR_DATA *ch, short dir)
{int vnum = 0;

 if (dir == 4 && pid(4))
 {
    ch->pcdata->powers[2] += 4;
    do_look(ch,"auto");
    return;
 }
 if (dir == 5 && ch->pcdata->powers[2] > 4)
 {
    ch->pcdata->powers[2] -= 4;
    do_look(ch,"auto");
    return;
 }    

 if (!pid(dir))
 {
    stc("A spiritual ghost tells you, 'You can't go that way.'\n\r", ch);
    return;
 }

 if (!get_room_index((vnum = pid(dir)->vnum)))
 {
    stc("That link has been blocked off from a weak mind.\n\r", ch);
    return;
 }

 char_from_room(ch);
 char_to_room(ch, get_room_index(vnum));

 act("#3$n arrives through the astral plane.", ch, 0, 0, TO_ROOM);
 stc("You step out of the astral plane.\n\r", ch);

 ch->pcdata->powers[2] = 0;
 do_look(ch, "auto");
 return;
}

void do_harden(CHAR_DATA *ch, char *argument)
{
 int type = -1, cost;

 if (!IS_GOLEM(ch))
 {
    stc("Huh?\n\r", ch);
    return;
 }
if (!*argument)
{
 STC( centerjob("#1*#2-#3=#2-#1*","",60/5), ch);
 STC( centerjob(" ",name_funny("Golem Powers",3,5),60), ch);
 STC( centerjob("#1*#2-#3=#2-#1*","",60/5), ch);
stc("#7",ch);
 STC( centerjob(".","",60),ch);
sprintf(log_buf, "#6Symantics#3(#2%d#3)#7", ch->pcdata->powers[SYMANTICS]);
 stc("#7",ch);
 stc( centerjob(".",log_buf,20),ch);
sprintf(log_buf, "#6Tacticals#3(#2%d#3)#7", ch->pcdata->powers[TACTICALS]);
 stc("#7",ch);
 stc( centerjob(".",log_buf,20),ch);
sprintf(log_buf, "#6MagiTos#3(#2%d#3)#7", ch->pcdata->powers[MAGITOS]);
 stc("#7",ch);
 STC( centerjob(".",log_buf,20),ch);
 STC( centerjob(".","",60), ch);
 STC( centerjob("#1*#2-#3=#2-#1*","",60/5), ch);
 return;
}

if (!str_cmp(argument, "symantics")) type = SYMANTICS;
if (!str_cmp(argument, "Tacticals")) type = TACTICALS;
if (!str_cmp(argument, "magitos"))   type = MAGITOS;

if (type == -1)
{
    stc("That power doesn't exist!\n\r", ch);
    return;
}

if (ch->pcdata->powers[type] > 9)
{
    stc("The maximum for that power is 9!\n\r", ch);
    return;
}

cost = ((((type-SYMANTICS)+1)*120000)*(ch->pcdata->powers[type]+1));

if (ch->exp < cost)
{
    sprintf(log_buf, "You don't have enough exp(%d needed)\n\r", cost);
    stc(log_buf, ch);
    return;
}

ch->exp -= cost;

sprintf(log_buf, "%d exp docked.\n\r", cost);

stc(log_buf, ch);

ch->pcdata->powers[type]++;

return;

}

void do_golpowers(CHAR_DATA *ch, char *argument)
{
 if (!IS_GOLEM(ch))
 {
    stc("Huh?\n\r", ch);
    return;
 }

sprintf(log_buf, "#7Symantics");
STC(centerjob("#1*#2-#3=#2-#1*",log_buf,12+strlen(bash_color(log_buf))),ch);
#define SYMP (ch->pcdata->powers[SYMANTICS])
 if (SYMP == 0)
    STC("#6You have no trainings in Symantics.",ch);
 if (SYMP >= 1)
    stc("#6Symantics: Shield", ch);
 if (SYMP >= 3)
    stc(" Nightsight", ch);
 if (SYMP >= 5)
    stc(" Hitch", ch);
 if (SYMP >= 7)
    stc(" Scry", ch);
 if (SYMP >= 10)
    stc(" Truesight", ch);
 stc("\n\r\n\r", ch);

#define TACTP (ch->pcdata->powers[TACTICALS])   
 sprintf(log_buf, "#1Tacticals");
 STC(centerjob("#2-#3=",log_buf,30+strlen(bash_color(log_buf))),ch);
 stc("\n\r", ch);
 if (TACTP == 0)
    stc("#5You have no training in Tacticals.\n\r", ch);
 else stc("#1Tacticals: ", ch);
 if (TACTP >= 1)
    stc("#3Sumosplash", ch); 
 if (TACTP >= 2)
    stc(" Bazmapunch", ch);
 if (TACTP >= 4)
    stc(" Elbowbash", ch);
 if (TACTP >= 5)
    stc(" groinkick", ch);
 if (TACTP >= 7)
    stc(" Backslash", ch);
 if (TACTP >= 8)
    stc(" Bind", ch);
 if (TACTP >= 10)
    stc(" Neckcrack", ch);

 stc("\n\r\n\r", ch);

sprintf(log_buf, "#2MagiTos");
 stc(centerjob("#1_#2-#3=#4-#5_", log_buf, 12+strlen(bash_color(log_buf))),ch);
 STC("#1_#2-\n\r", ch);
#define MAGIP (ch->pcdata->powers[MAGITOS])
 if (MAGIP == 0)
    stc("#5You have no training in MagiTos!\n\r", ch);
 else
    stc("#1MagiTos: #5", ch);
 if (MAGIP >= 1)
    stc("Ruby_Form", ch);
 if (MAGIP >= 3)
    stc(" Emerald_Form", ch);
 if (MAGIP >= 4)
    stc(" Gold_Form", ch);
 if (MAGIP >= 5)
    stc(" Crystal_Form", ch);
 if (MAGIP >= 6)
    stc(" Granite_Form\n\r", ch);
 if (MAGIP >= 7)
    stc("         #5Obsidian_Form", ch);
 if (MAGIP >= 10)
    stc(" Enhanced_Crystal", ch);
 STC("",ch);

 return;
}

void do_hitch(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict = get_char_room(ch, argument);

 if (!IS_GOLEM(ch))
 {
    stc("Huh?\n\r", ch);
    return;
 }

 if (SYMP < 5)
 {
    stc("You need Symantics 3 to use that.\n\r", ch);
    return;
 }

 if (!*argument)
 {
    stc("Hitch who?\n\r", ch);
    return;
 }

 else if (!vict)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 act("You take your weapon and make it cling on to $N.",ch,0,vict,TO_CHAR);
 act("$n takes $m weapon and makes it cling on to you.",ch,0,vict,TO_VICT);
 act("$n takes $m weapon and makes it cling to $N.",ch,0,vict,TO_NOTVICT);
 do_follow(vict, ch->pcdata->switchname);

 return;
}
 
void do_golbind(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;

 if (!IS_GOLEM(ch))
 {
    stc("Huh?\n\r", ch);
    return;
 }

 REQUIRETACT(ch,8)

 if ((vict = get_char_room(ch, argument)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 else if (vict->fighting)
 {
    stc("They can't be fighting!\n\r", ch);
    return;
 }

 else if (ch->fighting)
 {
    stc("Your already fighting!\n\r", ch);
    return;
 }

 else if (vict->special & SPC_GOLEMBIND)
 {
    stc("They are already being binded!\n\r", ch);
    return;
 }

 else if (ch->move < 5001)
 {
    stc("You need at least 5001 movement points!\n\r", ch);
    return;
 }

 else ch->move -= 5000;

 act("#1$n reaches out $m hands and legs to touch your hands and legs, and they #7solidify#n.",ch,0,vict,TO_VICT);
 act("#1You reach out your hands and legs to touch $N's hands and legs, and then they #7solidify#n.",ch,0,vict,TO_CHAR);
 act("#1$n reaches out $m hands and legs to touch $N's hands and legs, and then they #7solidify.#n",ch,0,vict,TO_NOTVICT);

 vict->special |= SPC_GOLEMBIND;
 multi_hit(ch, vict, TYPE_HIT);
 multi_hit(ch, vict, TYPE_HIT);
 multi_hit(ch, vict, TYPE_HIT);

 return;
}

void do_modify(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char arg1[MIL], arg2[MIL];
 char *full;

 if (!IS_GOLEM(ch))
 {
    stc("Huh?\n\r", ch);
    return;
 }

 if (ch->generation > 3)
 {
    stc("You are not powerful enough!\n\r", ch);
    return;
 }

 argument = one_argument(argument, arg1);
 full = argument;
 argument = one_argument(argument, arg2);

 if (arg1[0] == '\0' || arg2[0] == '\0')
 {
    stc("Modify whos texture to what?(format: modify (player) (ditto/normal)\n\r", ch);
    if (ch->generation == 1)
    {
	stc("You, Rok, may also choose which texture for them to be!\n\r", ch);
	stc("(Type modify (player) (texture other than ditto and normal))\n\r", ch);
    }
    return;
 }

 if ((vict = get_char_room(ch, arg1)) == NULL)
 {
    stc("They aren't here!\n\r", ch);
    return;
 }

 if (!IS_GOLEM(vict))
 {
    stc("They are not a rock! They can't be texturized!\n\r", ch);
    return;
 }

 if (vict->generation < ch->generation)
 {
    stc("They are too powerful for you!\n\r", ch);
    return;
 }

 if (ch->generation != 1 && str_cmp(arg2, "ditto") && str_cmp(arg2, "normal"))
 {
    stc("Choose out of the commands 'ditto' and 'normal' (ditto to make
\rtheir texture(clan) the same as yours, normal to make their texture
\rrogue.\n\r", ch);
    return;
 }

 if (!str_cmp(arg2, "ditto"))
 {
    free_string(vict->clan);
    vict->clan = str_dup(ch->clan);
 }
 else if (!str_cmp(arg2, "normal"))
 {
    free_string(vict->clan);
    vict->clan = str_dup("Rogue");
 }
 else
 {
    free_string(vict->clan);
    vict->clan = str_dup(full);
 }

 sprintf(log_buf, "noreport self %d", vict->generation);
 do_generation(vict, log_buf);
 stc("Done.\n\r", ch);
 return;
}

void do_study(CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 OBJ_DATA *obj2;
 int vnum;

 if (!IS_GOLEM(ch))
 {
    stc("Huh?\n\r", ch);
    return;
 }

 for (obj = ch->carrying; obj ;obj = obj->next_content)
    if (obj->pIndexData->vnum == 24)
	break;
 if (!obj)
 {
    stc("Your inventory does not contain one of those precious granite stones!\n\r", ch);
    return;
 }
#define bi(arr) (!str_cmp(argument, arr))
 vnum = bi("ring") ? 11400 : bi("collar") ? 11401 : bi("breastplate") ||
bi("plate") ? 11402 : bi("helmet") ? 11403 : bi("leggings") ? 11404 :
bi("boots") ? 11405 : bi("gauntlets") ? 11406 : bi("sleeves") ? 11407 :
bi("cape") ? 11408 : bi("belt") ? 11409 : bi("bracer") ? 11410 :
bi("visor") ? 11411 : bi("emerald") ? 11412 : bi("obsidian") ? 11413 : 0;

 if (!vnum)
 {
    stc("Please choose out of: a ring, collar, breastplate, helmet,
\rleggings, boots, gauntlets, sleeves, cape, belt, bracer, visor,
\remerald(sword) or obsidian(sword).\n\r", ch);
    return;
 }

 if (ch->practice < 45)
 {
    stc("You need 45 primal points to use that!\n\r", ch);
    return;
 }

 if (!get_obj_index(vnum))
 {
    stc("Bug. Inform Smoo(tangerine)\n\r", ch);
    return;
 }

 obj2 = create_object(get_obj_index(vnum),0);
 sprintf(log_buf, "$n modifies %s into $p!", obj->short_descr);
 act(log_buf, ch, obj2, 0, TO_ROOM);
 sprintf(log_buf, "You modifie %s into $p!", obj->short_descr);
 act(log_buf, ch, obj2, 0, TO_CHAR);

 extract_obj(obj);
 obj_to_char(obj2, ch);

 return;
}
