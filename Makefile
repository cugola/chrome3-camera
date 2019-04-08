CFLAGS = -Wall -O2
GST_LIBS = `pkg-config --libs gstreamer-1.0`
GST_CFLAGS = `pkg-config --cflags gstreamer-1.0 `
INSTALL ?= /usr/bin/install

BINS = play play_flip record_high record_medium record_low

all: $(BINS)

play: play.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) play.c -o play 

play_flip: play_flip.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) play_flip.c -o play_flip

record_high: record_high.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) record_high.c -o record_high

record_medium: record_medium.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) record_medium.c -o record_medium

record_low: record_low.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) record_low.c -o record_low

clean:
	$(RM) $(BINS)

