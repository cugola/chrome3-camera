#include <gst/gst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define PIPELINE "v4l2src device=/dev/video0 do-timestamp=true !  autovideosink" 

//#define PIPELINE "v4l2src device=/dev/video0 do-timestamp=true ! image/jpeg,width=800,height=448 ! jpegdec ! videoflip method=horizontal-flip ! imxipuvideosink sync=false"

#define PIPELINE "v4l2src device=/dev/video0 do-timestamp=true ! video/x-raw, framerate=30/1, width=640, height=360 ! videoflip method=horizontal-flip ! autovideoconvert ! fbdevsink sync=false"

int main(int argc, char *argv[]) {
    GstElement *pipeline;
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

    gst_element_set_state(pipeline, GST_STATE_PAUSED);            
    
    // expected sequence of commands...
    // setfn filename -> start -> stop -> setfn filename -> start -> stop -> .... -> end
    printf("ready\n");
    cmd = (char *) malloc(128);
    filename = (char *) malloc(256);
    while(1) {
        scanf("%s", cmd);
        if(strcmp(cmd,"start")==0) {
	    int res;
            res=gst_element_set_state(pipeline, GST_STATE_PLAYING);	
            if(res==GST_STATE_CHANGE_FAILURE) break;
            //printf("started %d\n",res);
        } else if(strcmp(cmd,"stop")==0) {
            if(gst_element_set_state(pipeline, GST_STATE_PAUSED)==GST_STATE_CHANGE_FAILURE) break;
            //printf("stopped\n");
        } else if(strcmp(cmd,"end")==0) {
            break;
        }
    }
    free(cmd);
    free(filename);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    // printf("ended\n");
    return 0;   
}
    
