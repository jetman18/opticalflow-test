
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "px4flow.hpp"
#include "transfe.hpp"
#include <string> 
#include <chrono>
using namespace std;
using namespace cv;
#define IMAGE_WIDTH 128 // FRAMESIZE_QQVGA2 => 128x160
#define SEARCH_SIZE 6
#define FLOW_FEATURE_THRESHOLD 30
#define FLOW_VALUE_THRESHOLD 4000
#define BAUDRATE_  9600
PX4Flow *px4flow;
transfe  *uart;
int main(){
    px4flow = new PX4Flow(IMAGE_WIDTH,SEARCH_SIZE,FLOW_FEATURE_THRESHOLD,FLOW_VALUE_THRESHOLD);
    uart = new transfe(BAUDRATE_,"/dev/ttyS1");
   
    Mat frame;
    Mat p_frame;
    VideoCapture cap(0);

    cap.set(CAP_PROP_FRAME_WIDTH,320);
    cap.set(CAP_PROP_FRAME_HEIGHT,240);
    cap.set(CAP_PROP_FRAME_FPS,40);

    while(1){
        
        p_frame = frame;
        cap.read(frame);
	if(p_frame.empty()){
           cout<<"frame empty"<<endl;
           continue;  
        }
	// convert color to gray
        cvtColor(frame,frame,COLOR_RGB2GRAY);
     
        int down_width = 128;
        int down_height =128;
        resize(frame,frame,Size(down_width, down_height),INTER_LINEAR);
    
        float flow_rate_x, flow_rate_y;
        int quality = px4flow->compute_flow(p_frame.data,frame.data,0,0,0, &flow_rate_x, &flow_rate_y);
  
        flow_rate_x *=100;
        flow_rate_y *=100;

        if(flow_rate_x > 100)flow_rate_x =100;
        esle if(flow_rate_x < 100)flow_rate_x = -100;

        if(flow_rate_y > 100)flow_rate_y =100;
        esle if(flow_rate_y < 100)flow_rate_y = -100;
        
        uart->send_data((char)flow_rate_x,(char)flow_rate_x,(char)quality);
        //cout<<(int)(rate_x*100)<<endl;        
    }
    cap.release();//Releasing the buffer memory//
    return 0;
}
