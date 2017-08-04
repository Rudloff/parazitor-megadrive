GENDEV = /opt/gendev

all:
	GENDEV=${GENDEV} make -f ${GENDEV}/sgdk/mkfiles/makefile.gen clean all

run: all
	mednafen out/rom.bin

lint:
	splint -I${GENDEV}/sgdk/inc/ -I${GENDEV}/sgdk/res/ -nolib -nestcomment -mustfreeonly  *.c
