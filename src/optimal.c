#define _GNU_SOURCE

#include "network.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#include <sys/time.h>
#include <unistd.h>

#define DEMO 1

#ifdef OPENCV

extern int display_index;

void demo_optimal(char *cfgfile, char *weightfile, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
{
	//demo_frame = avg_frames;
	image **alphabet = load_alphabet();
	demo_names = names;
	demo_alphabet = alphabet;
	demo_classes = classes;
	demo_thresh = thresh;
	demo_hier = hier;
	printf("Demo\n");
	net = load_network(cfgfile, weightfile, 0);
	set_batch_network(net, 1);
	pthread_t detect_thread;
	pthread_t fetch_thread;

	srand(2222222);

	int i;
	demo_total = size_network(net);
	predictions = calloc(demo_frame, sizeof(float*));
	for (i = 0; i < demo_frame; ++i){
		predictions[i] = calloc(demo_total, sizeof(float));
	}
	avg = calloc(demo_total, sizeof(float));

	if(filename){
		printf("video file: %s\n", filename);
		cap = open_video_stream(filename, 0, 0, 0, 0);
	}else{
		cap = open_video_stream_cam_fps(0, cam_index, w, h, frames, ptr_camera_fps);
		printf("camera fps : %f\n",camera_fps);
	}

	if(!cap) error("Couldn't connect to webcam.\n");

	buff[0] = get_image_from_stream(cap);
	buff[1] = copy_image(buff[0]);
	buff[2] = copy_image(buff[0]);
	buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
	buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
	buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

	if(!prefix){
		make_window("Demo", 1352, 1013, fullscreen);
	}

	demo_time=gettimeafterboot();

	for(int iter=0;iter<cycle;iter++){
		while(!demo_done){

			if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");

			if(!prefix){
				fps=1./(gettimeafterboot()-demo_time)*1000;
				demo_time=gettimeafterboot();
				detect_in_thread(0);
				display_index = (buff_index+2)%3;
				display_in_thread(0);

			}else{
				char name[256];
				sprintf(name, "%s_%08d", prefix, count);
				save_image(buff[(buff_index + 2)%3], name);
			}
			pthread_join(fetch_thread, 0);

			if(count>=start_log){
				slack[count-start_log]=(detect_time+display_time)-(offset+fetch_time);
				int n;
				int old_offset=offset;
				if((old_offset+fetch_time)<(detect_time+display_time)){ 
					offset+=1000./(camera_fps);
				}else{
					for(n=1;((old_offset+image_waiting_time-n*1000./camera_fps)>(detect_time));n++){
						offset=old_offset+image_waiting_time-(n+0.5)*1000./(camera_fps);
					}
				}
				printf("Offset : %d\n", offset);
				printf("E2E_delay : %f\n",latency[count-start_log]);
			}
			if(count==(iteration+start_log-1)){
				for(int i=0;i<iteration;i++){
					image_waiting_sum[iter]+=image_waiting_array[i];
					fetch_sum[iter]+=fetch_array[i];
					detect_sum[iter]+=detect_array[i];
					display_sum[iter]+=display_array[i];
					slack_sum[iter]+=slack[i];
					fps_sum[iter]+=fps_array[i];
					latency_sum[iter]+=latency[i];
				}
				break;
			}
			count++;
			buff_index = (buff_index + 1) %3;
		}
		for(i=0; i<cycle;i++){
			printf("avg_image_waiting[%d] : %f\n",i,image_waiting_sum[i]/iteration);
			printf("avg_fetch[%d] : %f\n",i,fetch_sum[i]/iteration);
			printf("avg_detect[%d] : %f\n",i,detect_sum[i]/iteration);
			printf("avg_display[%d] : %f\n",i,display_sum[i]/iteration);
			printf("avg_slack[%d] : %f\n",i,slack_sum[i]/iteration);
			printf("avg_fps[%d] : %f\n",i,fps_sum[i]/iteration);
			printf("avg_latency[%d] : %f\n",i,latency_sum[i]/iteration);
		}
	}
}

#else
void demo(char *cfgfile, char *weightfile, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg, float hier, int w, int h, int frames, int fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif

