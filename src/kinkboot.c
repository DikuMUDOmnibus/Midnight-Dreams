//this seemed to have a little bug where it leaves a few people hanging
//every once in a while, I might have fixed it, but not sure.
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>	
#include "merc.h"
#define DD DESCRIPTOR_DATA
void init_descriptor    args( (DESCRIPTOR_DATA *dnew, int desc) );
bool    write_to_descriptor     args( ( DD * desc, char *txt, int length ));
bool    process_output          args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
bool write_to_descriptor2	args( ( int desc, char *txt, int length));

extern int port, control;

void do_kinkboot( CHAR_DATA *ch, char *notimpotant )
{
 FILE *fp;
 DESCRIPTOR_DATA *d, *d_next;

 if ((fp = fopen("../area/kinkboot.dat", "w")) == NULL)
 {
    stc("Copyover failed. ID 001\n", ch);
    log_string("Copyover failed..");
    return;
 }

 for (d = descriptor_list; d; d = d->next)
 {CHAR_DATA *vch=0x0;if ((vch = d->character) == NULL)
   continue;
  if (d->connected < CON_PLAYING) continue;
    d->character->fighting = NULL;
    d->character->position = POS_STANDING;
    if (IS_NPC(d->character)) continue;
smooregen(vch,60000,60000,60000);
    stc("#5K#1i#5n#1k#5B#1o#5o#1t#5i#1n#5g#1! 
\r#2Automatic wake, unfight, call all, and save.#n\n\r", vch);
}


for (d = descriptor_list; d; d = d->next)
{
    if (d->character && d->connected >= CON_PLAYING) do_wake(d->character, "");
}
for (d = descriptor_list; d; d = d->next)
{
    if (d->character && d->connected >= CON_PLAYING)
{
    do_call(d->character, "all");
stc("Saved.\n\r",d->character);}/*the real save is below with no message */
}

 for (d = descriptor_list; d; d = d_next)
 {
    d_next = d->next;
    if (!d->character || d->connected < CON_PLAYING)
    {
	write_to_descriptor(d, "We are rebooting. Please reconnect.\n", 0);
	close_socket(d);
	continue;
    }
    process_output(d, FALSE);
    save_char_obj(d->character);
    fprintf(fp, "%d %s %s\n", d->descriptor, d->character->pcdata->switchname,d->host);
 }

 fprintf(fp, "-1\n");
 fflush(fp);
 fclose(fp);

 execl(EXE_FILE, "Midnight Dreams", CHAR2(port), "KinkBoot", CHAR2(control), (char *) NULL);

 for (d = descriptor_list; d; d = d->next)
    stc("#5K#1i#5n#1k#5B#1o#5o#1t#5 F#1a#5i#1l#5e#1d#5:#1(#n\n\r", d->character);

 return;
}

void kinkboot_recover( )
{
 DESCRIPTOR_DATA *d;
 FILE *fp;
 char name[100];
 char host[100];
 int desc;
 bool fOld;

 log_string("Kinkboot recovery started..");
 if ((fp = fopen("../area/kinkboot.dat", "r")) == NULL)
 {
    log_string("Kinkboot recovery FAILED.");
    exit(1);
 }
 for (; ;)
 {

    fscanf( fp, "%d %s %s", &desc, name, host);

    if (desc == -1)
	break;
    d = alloc_perm(sizeof(*d));
    d->descriptor = desc;

    if (!write_to_descriptor(d, "\n\rRestoring from kinkboot..\n\r", 0))
    {
	close(desc);
	continue;
    }

    init_descriptor(d, desc);

    d->host = str_dup(host);
    d->next = descriptor_list;
    descriptor_list = d;
    d->connected = 15;

    if ((fOld = load_char_obj(d, name)) == FALSE)
    {
	write_to_descriptor2(desc, "\n\rByebye. You wern't saved properly:)\n\r", 0);
	close_socket(d);
	continue;
    }

    stc("#6OOOOhh #2K#3i#2n#3k#2y#3! #4Kinkboot Recovery Successful#n\n\r", d->character);
    if (!d->character->in_room)
	d->character->in_room = get_room_index(ROOM_VNUM_ALTAR);

    d->character->next = char_list;
    char_list = d->character;

    char_to_room(d->character, d->character->in_room);
    d->connected = CON_PLAYING;
    do_look(d->character, "auto");

    act("#6$n #2K#3i#2n#3k#2i#3f#2i#3e#2s#3 i#2n#3t#2o #3p#2l#3a#2c#3e#2!#n", d->character, 0x0, 0x0, TO_ROOM);
 }

 fclose(fp);
 return;
}
