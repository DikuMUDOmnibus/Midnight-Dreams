/* CLass table shit by smoo */
#define CLASS_DEMON      1
#define CLASS_MAGE       2
#define CLASS_WEREWOLF	 4
#define CLASS_VAMPIRE	 8
#define CLASS_HIGHLANDER 16
#define CLASS_DROW	 32
#define CLASS_MONK	 64
#define CLASS_NINJA	 128
#define CLASS_AMAZON	 256
#define CLASS_DRAGON	 512
#define CLASS_HERO	 1024
#define CLASS_JEDI	 2048
#define CLASS_SHAPE	 4096
#define CLASS_GOLEM	 8192
#define CLASS_MIND	 16384
#define CLASS_MECHA	 32768
#define CLASS_UNICORN	 65536
#define CLASS_COW	131072	
#define CLASS_HUNTER    262144
#define CLASS_BERSERK   524288
#define CLASS_MOOGLE   1048576
int class_lookup	( int class );
int class_lookupn	( char * class );

extern  const   struct  strctr_class_table    class_table [];
struct strctr_class_table
{
    long         class;
    char   *    class_name;
    int         start_trust;
    int         start_level;
    int		start_gen;
    int         bits; // BIT_PTEST_SAFE, etc
}; 

#define BIT_PTEST_NORMAL	1	//class in ptesting,not safe
#define BIT_PTEST_SAFE		2	//class in ptesting,safe from pk
#define BIT_PTEST_TOGGLE	4	//class in ptesting,players can
					//toggle if their safe or not
#define BIT_DONTCLASS		8	//unable to class /w class command
#define BIT_CLOSED		16	//unable to class /w classme
					//classme is in smoo.c

#define IC_BIT_NUMB(class,bit)	(class_table[class_lookup(class)].bits & \
				bit)
#define IC_BIT_NAME(class,bit)	(class_table[class_lookupn(class)].bits &\
				bit)


#define IS_PTEST_SAFE(ch) (ch->flag3 & AFF3_PTEST_SAFE||IC_BIT_NUMB(ch->class, BIT_PTEST_SAFE))
#define SET_PTEST_SAFE(ch) (SET_BIT(ch->flag3, AFF3_PTEST_SAFE))
#define REM_PTEST_SAFE(ch) (REMOVE_BIT(ch->flag3, AFF3_PTEST_SAFE))
DECLARE_DO_FUN(do_ptesttoggle);
