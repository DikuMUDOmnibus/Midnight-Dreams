#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "merc.h"

extern control, port;
#define DD DESCRIPTOR_DATA
bool    write_to_descriptor     args( ( DD *desc, char *txt, int length )
);
void attempt_copyover( void );
void crash_notice( int );

void init_signals( void )
{
  pfile = FALSE;
  pfile_name = str_dup("");

    signal(SIGPFILE,	crash_notice);
    signal(SIGFPE,	crash_notice);
    signal(SIGINT,	crash_notice);
    signal(SIGHUP,	crash_notice);
    signal(SIGSEGV,	crash_notice);
    signal(SIGTERM,	crash_notice);
    signal(SIGIOT,	crash_notice);
    signal(SIGUSR1,	crash_notice);
    signal(SIGIO,	crash_notice);
    signal(SIGPIPE,	crash_notice);
    signal(SIGTRAP,	crash_notice);
return;
}

void crash_notice( int sig )
{
 FILE *fp;

 fp =fopen("crash.txt", "a");

 fprintf(fp, "Crash at %s system time.\n", mud_time);
 fprintf(fp, "Last pfile was: %s..%s\n", pfile_name, !pfile ? "loaded properly" : "BUGGED");
 fprintf(fp, "Last command: %s <BY> %s <%s>\n",
last_user,last_command,command_finished ? "finished" : "BUGGED");
fflush(fp);
fclose(fp);
{
 DESCRIPTOR_DATA *d;
 DESCRIPTOR_DATA *d_next;

// if (sig != SIGINT && sig != SIGPFILE)
if (sig != SIGPFILE)
attempt_copyover();

for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected >= CON_PLAYING) do_wake(d->character,
"");
for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected >= CON_PLAYING) do_call(d->character,
"all");
for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected >= CON_PLAYING)
save_char_obj(d->character);

 for (d = descriptor_list; d; d = d_next)
 {
  d_next = d->next;

  if (d->character)
     stc("The Midnight Dreams are about to crash. Hold on tight.. \n\r", d->character);
  else
     write_to_descriptor(d, "We are about to crash. Hold on..", 0);

if (pfile)
{sprintf(log_buf, "%s has a bugged pfile. He crashed the mud..\n\r",
	pfile_name);
 write_to_descriptor(d, log_buf, 0);}

/*if (d->character)
{do_wake(d->character,"");do_call(d->character,"all");save_char_obj(d->character);}*/

  close_socket(d);
} }
abort();
}

void attempt_copyover(void)
{
 DESCRIPTOR_DATA *d;
 FILE *fp;
	fp = fopen ("../area/kinkboot.dat", "w");
	if (!fp)
		return;

log_string("CRASHING..");
for (d = descriptor_list; d; d = d->next)
{
 CHAR_DATA *ch = d->original ? d->original : d->character;
 save_char_obj(ch);
 if (!ch) {write_to_descriptor(d,"We are crashing. Please reconnect.\n\r", 0);close_socket(d);continue;}
fprintf(fp, "%d %s %s\n", d->descriptor, d->character->pcdata->switchname, d->host);
 write_to_descriptor(d, "The Midnight Dreams are crashing! Attempting to copyover from the crash.\n\r", 0);
}
        fprintf (fp, "-1\n");
	fflush(fp);
        fclose (fp);

execl(EXE_FILE, "MidNighT", CHAR2(port), "copyover", CHAR2(control), (char *) NULL);

for (d = descriptor_list; d; d = d->next)
 write_to_descriptor(d, "COPYOVER FAILED! Attempting NORMAL boot!\n\r", 0);

return;
}

