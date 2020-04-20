#ifndef DEMO_H
#define DEMO_H

#include "image.h"

#define iteration 1000
#define start_log 25
#define cycle 1

double gettimeafterboot();
detection *get_network_boxes(network *net, int w, int h, float thresh, float hier, int *map, int relative, int *num);
int size_network(network *net);
void remember_network(network *net);
detection *avg_predictions(network *net, int *nboxes);
void *detect_in_thread(void *ptr);
void *fetch_in_thread(void *ptr);
void *display_in_thread(void *ptr);
void *display_loop(void *ptr);
void *detect_loop(void *ptr);

char **demo_names;
image **demo_alphabet;
int demo_classes;

network *net;
image buff [3];
image buff_letter[3];
int buff_index;
void * cap;
float fps;
float demo_thresh;
float demo_hier;
int running;

int demo_frame;
int demo_index;
float **predictions;
float *avg;
int demo_done;
int demo_total;
demo_time;

double image_waiting_array[iteration];
double fetch_array[iteration];
double detect_array[iteration];
double display_array[iteration];
double slack[iteration];
double fps_array[iteration];
double latency[iteration];

int count;

int process_number;
float camera_fps;
float *ptr_camera_fps;
long int frame_number[3];
double frame_timestamp[3];
double detect_start;
double detect_end;
double detect_time;
double display_time;
double fetch_start;
double fetch_time;
double image_waiting_time;
int offset;

double image_waiting_sum[cycle];
double fetch_sum[cycle];
double detect_sum[cycle];
double display_sum[cycle];
double slack_sum[cycle];
double fps_sum[cycle];
double latency_sum[cycle];

int display_index;

#endif
