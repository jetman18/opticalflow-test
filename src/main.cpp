
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "px4flow.hpp"
#include "debug.hpp"
#include <string> 
#include <chrono>
using namespace std;
using namespace cv;
#define IMAGE_WIDTH 128 // FRAMESIZE_QQVGA2 => 128x160
#define SEARCH_SIZE 6
#define FLOW_FEATURE_THRESHOLD 30
#define FLOW_VALUE_THRESHOLD 4000

PX4Flow *px4flow;
debug  *uart;
int main(){
    px4flow = new PX4Flow(IMAGE_WIDTH,SEARCH_SIZE,FLOW_FEATURE_THRESHOLD,FLOW_VALUE_THRESHOLD);
    uart = new debug(9600,"/dev/ttyS1");
    //boot time
   //auto start = std::chrono::high_resolution_clock::now();
   
    Mat frame;
    Mat p_frame;
    VideoCapture cap(0);

    cap.set(CAP_PROP_FRAME_WIDTH,320);
    cap.set(CAP_PROP_FRAME_HEIGHT,240);

/*
    //save video
    Size frame_size(400,400);
    int frames_per_second = 10;

    //Create and initialize the VideoWriter object 
     VideoWriter oVideoWriter("test.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true); 
*/
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

  
/*      
        down_width = 400;
        down_height =400;
        Mat ff;
        resize(frame,ff,Size(down_width, down_height),INTER_LINEAR);
        cvtColor(ff,ff,COLOR_GRAY2RGB);

*/       // oVideoWriter.write(ff); 
        
//line
/*
        Point p1(150,150), p2(150 - int(rate_x*10),150 - int(rate_y*10));
        int thickness = 2;
        line(frame, p1, p2, Scalar(0,255, 0),thickness, LINE_8);
        
        //cricle
        Point center(150,150);//Declaring the center point
        int radius = sqrtf((rate_x*10)*(rate_x*10) + (rate_y*10)*(rate_y*10)); 
        Scalar line_Color(0, 0, 255);//Color of the circle
        int thicknes = 2;//thickens of the line
        circle(frame,center,radius,line_Color, thicknes);
        //oVideoWriter.write(ff); 

        putText(frame, "Quality:",cv::Point(10, 290), 1,1,CV_RGB(0,255,0),1.5);
        std::string s = std::to_string(quality);
        putText(frame, s,cv::Point(80, 290),1,1,CV_RGB(255,0,255),1.3);

        putText(frame, "Rate x:",cv::Point(10, 270), 1,1,CV_RGB(0,255,0), 1.3);
        std::string x = std::to_string(int(rate_x*10));
        putText(frame, x,cv::Point(80, 270),1,1,CV_RGB(255,0,0),1.3);

        putText(frame, "Rate y:",cv::Point(10, 250), 1,1,CV_RGB(0,255,0), 1.3);
        std::string y = std::to_string(int(rate_y*10));
        putText(frame, y,cv::Point(80, 250),1,1,CV_RGB(255,0,0),1.3);
*/
  //      imshow("2",ff);

    }
    cap.release();//Releasing the buffer memory//
    return 0;
}
