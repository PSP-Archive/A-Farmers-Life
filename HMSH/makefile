TARGET = A Farmers life
TARGET_ELF = elf.elf
OBJS = framebuffer.o fontloader.o graphics.o debug.o main.o \
mikmodplayer.o oggplayer.o controller.o zip.o common.o \
zip/zfile.o zip/unzip.o \
system/engine.o system/system.o system/player.o system/map.o system/audio.o \
system/titlescreen.o system/font.o system/commandhandler.o

PSP_PATH = H:/PSP/GAME

CFLAGS = -O3 -G0 -Wall -std=c99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lpsppower -lpsprtc -lpspgu -lpspgum -lmikmod -lvorbisidec -lpspaudiolib -lpspaudio \
	-ljpeg -lmad -lpng -lfreetype -lz -lc -lm
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP 
PSP_EBOOT_TITLE = A Farmer's life

PSPSDK=$(shell psp-config --pspsdk-path)

include $(PSPSDK)/lib/build.mak
