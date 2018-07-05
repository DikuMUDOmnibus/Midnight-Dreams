//I started this class right before the mud went down, bryantos didn't
//give me a warning.
/* OH WOW its ANOTHER SmOO ClASS!! */
#ifdef macintosh
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

void do_sing(CHAR_DATA *ch, char *argument)
{
 char arg[MIL];
 CHAR_DATA *vict;

 if (!IS_UNI(ch))
 {
    if (!check_social(ch,"sing",argument,TYPE_SOCIAL))
	stc("Huh?\n\r", ch);
    return;
 }

 argument = one_argument(argument, arg);

 if (!*arg)
 {
     stc("Sing what song?
\rHeal
\rInspire
\rQuell\n\r", ch);
    return;
 }

 if (ch->pcdata->powers[UNI_SONG] != 0)
 {
    stc("You are already singing a song!\n\r", ch);
    return;
 }
 else if (!str_cmp(arg, "heal"))
 {
    ch->pcdata->powers[UNI_WAIT] = 2;
    ch->pcdata->powers[UNI_SONG] = SONG_HEAL;
 }
 else if (!str_cmp(arg, "inspire"))
 {
    ch->pcdata->powers[UNI_SONG] = SONG_INSPIRE;
    ch->pcdata->powers[UNI_WAIT] = 2;
 }
 else if (!str_cmp(arg,"night"))
 {
    int a;
    for (a = 0; a < atoi(argument); a++)
	inspire(ch);
 }
 else if (!str_cmp(arg, "quell"))
 {
    ch->pcdata->powers[UNI_SONG] = SONG_QUELL;
    ch->pcdata->powers[UNI_WAIT] = 3;
 }
 else
 {
    do_sing(ch,"");
    return;
 }

 if (*argument && (vict = get_char_world(ch,argument)) != NULL)
    ch->uni = (void *) vict;

 act("#7$n begins to sing a song.#n",ch,0,0,TO_ROOM);
 act("#7You being to sing a song.#n",ch,0,0,TO_CHAR);

 return;
}

void inspire(CHAR_DATA *ch)
{
 AFFECT_DATA af;

 af.type = skill_lookup("inspire");
 af.duration = NR(4,7);
 af.location = APPLY_NONE;
 af.modifier = 0;
 af.bitvector = 0;
 affect_to_char(ch,&af);
 return;
}

int countinspires(CHAR_DATA *ch)
{int retur = 0;
 AFFECT_DATA *paf;

 for (paf = ch->affected; paf; paf = paf->next)
    if (paf->type == skill_lookup("inspire")) retur++;

 return retur;
}

void do_raise(CHAR_DATA *ch, char *argument)
{
 int cost;

 if (!IS_UNI(ch))
 {
    check_social(ch,"raise",argument,TYPE_SOCIAL);
    return;
 }

 if (!str_prefix("heal",argument))
 {int a = ch->pcdata->powers[UNI_DISC_HEAL];

  for (cost = 1000000; a > 0; a--)
    cost *= 1.8;

  argument += 5;
  while (*argument == ' ')
    argument++;

 if (str_cmp(argument, "improve"))
 {
    sprintf(log_buf, "#2[#3%3d#2] #7Heal	Next Level:#6%d#n\n\r",
	ch->pcdata->powers[UNI_DISC_HEAL], cost);
    stc(log_buf, ch);
    stc("Type raise heal improve to improve your healing power.\n\r",ch);
    return;
 }

 if (ch->exp < cost)
 {
    stc("You don't have enough exp to raise that!\n\r", ch);
    do_raise(ch,"heal");
    return;
 }

 ch->exp -= cost;
 ch->pcdata->powers[UNI_DISC_HEAL]++;
 act("#7$n raises $m horn in defense, as $e improves $m power.",
	ch,0,0,TO_ROOM);
 act("#7You raise your horn in defense as you improve your power.",
	ch,0,0,TO_CHAR);
 return;
 }
 
 stc("Right now you may only raise the heal power.
\rType raise heal, or raise heal improve.\n\r", ch);
 return;

}

void uni_heal(CHAR_DATA *ch, CHAR_DATA *vict)
{
    int hp = ch->pcdata->powers[UNI_DISC_HEAL] * 100;
    int mana = ch->pcdata->powers[UNI_DISC_HEAL] * 75;
    int move = ch->pcdata->powers[UNI_DISC_HEAL] * 75;

    if (vict->fighting)
    {
	hp /= 3;
    }

    smooregen2(vict,hp,mana,move,750);

    return;
}
