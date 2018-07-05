CC      = gcc
PROF    =
NOCRYPT =
#Uncomment the next line if you want request support
#DBUGFLG = -DREQUESTS
C_FLAGS = -O2 -Wall $(PROF) $(NOCRYPT) $(DBUGFLG) -g -ggdb
L_FLAGS = $(PROF) 

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o const.o \
          db.o fight.o handler.o interp.o magic.o save.o special.o smoo.o\
          kav_info.o kav_fight.o kav_wiz.o clan.o  mage.o rot_wiz.o \
          drow.o arena.o ninja.o vamp.o vampie.o ww.o wizutil.o golem.o\
          rotain.o update.o monk.o daemon.o jedi.o signals.o\
	  build.o darkhear.o amazon.o detox.o other.o shape.o kinkboot.o\
	  class.o mecha.o uni.o bere.o hunter.o newfight.o moogle.o

merc: $(O_FILES)
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
