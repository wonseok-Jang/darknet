./darknet detector demo cfg/coco.data cfg/yolov3.cfg /aveesSSD/darknet/yolov3.weights -c 0 -buffer_size 0 -offset 175 -process_num 1 &
./darknet detector demo cfg/coco.data cfg/yolov3.cfg /aveesSSD/darknet/yolov3.weights -c 1 -buffer_size 0 -offset 175 -process_num 2
