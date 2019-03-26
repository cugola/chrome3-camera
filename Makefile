CFLAGS = -Wall -O2
GST_LIBS = `pkg-config --libs gstreamer-1.0`
GST_CFLAGS = `pkg-config --cflags gstreamer-1.0 `
INSTALL ?= /usr/bin/install

BINS = play play_flip record_high record_medium record_low

all: $(BINS)

play:
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) play.c -o play 

play_flip:
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) play_flip.c -o play_flip

record_high:
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) record_high.c -o record_high

record_medium:
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) record_medium.c -o record_medium

record_low:
	$(CC) $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(LDFLAGS) record_low.c -o record_low

clean:
	$(RM) $(BINS)

