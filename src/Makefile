CC      = gcc
PROF    =
NOCRYPT =
C_FLAGS = -O -g $(PROF) $(NOCRYPT)
L_FLAGS = -O $(PROF)

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o bit.o data_base.o \
	board.o build.o clan.o comm.o class_sav.o darkheart.o db.o \
        fight.o kav_wiz.o handler.o kav_fight.o kav_info.o dragon.o \
	interp.o kingdom.o magic.o mem.o olc.o champion.o vamp.o hobbit.o \
	olc_act.o olc_save.o rot_wiz.o save.o special.o string.o jobo.o \
	update.o wizutil.o arena.o  ocommands.o const.o vladdonly.o dragon_knight.o \
        trees.o deflate.o aura.o monk.o angel.o paladin.o drow.o amazon.o \
        mccp.o jope.o zutil.o adler32.o ban.o highlander.o demon.o elf.o dungeon.o \
        highwind.o mage.o garou.o ninja.o gaiden.o shaolin.o kuruda.o fiend.o \
        sorceror.o who_html.o divine.o hunter.o knight.o shadow.o vampire.o scavenger.o \
        undead_knight.o saiyan.o daywalker.o samurai.o summoner.o necromancer.o powers.o \
        wizard.o daramon.o nightblade.o assassin.o kodo.o werewolf.o snow_elves.o \
        wisp.o duergar.o phoenix.o tournament.o auction.o kofkuruda.o team.o groove.o \

merc: $(O_FILES)
	mv starwars.exe ../
	$(CC) $(L_FLAGS) -o starwars $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
