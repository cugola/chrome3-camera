#include <gst/gst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define PIPELINE "v4l2src device=/dev/video0 do-timestamp=true ! queue max-size-buffers=1200 max-size-time=5000000000 max-size-bytes=0 ! videorate ! image/jpeg,width=800,height=448,framerate=25/1 ! matroskamux min-index-interval=40000000 ! filesink name=my_filesink sync=false"

#define PIPELINE "v4l2src device=/dev/video0 do-timestamp=true ! queue max-size-buffers=1200 max-size-time=0 max-size-bytes=0 ! videorate ! image/jpeg,width=800,height=448,framerate=25/1 ! mux. alsasrc device=default:CARD=imx6wandboardsg provide-clock=true do-timestamp=true ! audio/x-raw,format=S16LE,rate=44100 ! queue max-size-buffers=0 max-size-time=5000000000 max-size-bytes=0 ! imxmp3audioenc bitrate=64 ! mux. matroskamux name=mux ! filesink name=my_filesink sync=false"  


int main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstElement *filesink;
    GError *error = NULL;
    char *cmd, *filename;
    
    gst_init(&argc, &argv);
    
    pipeline = gst_parse_launch(PIPELINE, &error);
    if(!pipeline) {
        g_print("Parse error: %s\n", error->message);
        exit(1);
    }
    if(error) {
        g_printerr("ERROR: %s\n", error->message);
        g_error_free(error);
        exit(1);
    }

    // expected sequence of commands...
    // setfn filename -> start -> stop -> setfn filename -> start -> stop -> .... -> end
    // printf("ready\n");
    cmd = (char *) malloc(128);
    filename = (char *) malloc(256);

    GstBus *bus;

    while(1) {
        scanf("%s", cmd);
        if(strcmp(cmd,"setfn")==0) {
            scanf("%s", filename);
            filesink = gst_bin_get_by_name(GST_BIN(pipeline), "my_filesink");
            g_object_set(filesink, "location", filename, NULL);
            g_object_unref(filesink);
            if(gst_element_set_state(pipeline, GST_STATE_PAUSED)==GST_STATE_CHANGE_FAILURE) break;
            // printf("paused\n");
        } else if(strcmp(cmd,"start")==0) {
            if(gst_element_set_state(pipeline, GST_STATE_PLAYING)==GST_STATE_CHANGE_FAILURE) break;
            // printf("started\n");
        } else if(strcmp(cmd,"stop")==0) {
            gst_element_send_event (pipeline, gst_event_new_eos());
            //printf("EOS\n");
 	    bus = gst_element_get_bus (pipeline);
            gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
            if(gst_element_set_state(pipeline, GST_STATE_NULL)==GST_STATE_CHANGE_FAILURE) break;
            //printf("stopped\n");
	    break;	
        } else if(strcmp(cmd,"end")==0) {
            break;
        }
    }
    free(cmd);
    free(filename);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    // printf("ended\n");
    exit(0);
    //return 0;   
}
    
