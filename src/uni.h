DECLARE_DO_FUN(do_raise);
DECLARE_DO_FUN(do_sing);
int countinspires(CHAR_DATA *);
void inspire(CHAR_DATA *);
void uni_heal(CHAR_DATA *, CHAR_DATA *);

#define IS_UNI(ch) (ch->class & CLASS_UNICORN)

// ch->pcdata->powers[]
#define UNI_SONG	0
#define UNI_WAIT	1
#define UNI_DISC_HEAL	2

// ch->pcdata->powers[0] || ch->pcdata->powers[UNI_SONG]
#define SONG_HEAL	1
#define SONG_INSPIRE	2
#define SONG_QUELL	3
