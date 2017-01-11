#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include<stdio.h>

using namespace cv;
using namespace std;
 Mat image,image_rgb,processed;
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
int main(){
     cout << "get pixcel\n";
 VideoCapture capture(-1); //Create a VideoCapture object 
 capture.set(CV_CAP_PROP_FRAME_WIDTH,360);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
capture.read(image); 
capture.read(image_rgb); 
//image=imread("sheet.png");  //Loaded 3-channel picture
//image_rgb=imread("sheet.png");  //Loaded 3-channel picture
	//image_rgb=image;
     if (image.empty()) //check whether the image is loaded or not
     {
          cout << "Error : Image cannot be loaded..!!" << endl;
          return -1;
     }
	
    
	namedWindow("Window1",CV_WINDOW_AUTOSIZE);
	 setMouseCallback("Window1", CallBackFunc, NULL);
	while(1)
	{
	capture.read(image); 
	cvtColor(image,image, COLOR_BGR2HSV);
	inRange(image, Scalar(13,160,140), Scalar(16,255, 255),processed); 
    imshow("Window1",image);
	imshow("processed",processed);
    waitKey(1);
    }
	return 1;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
 unsigned char b,g,r;
char buffer[32];
 unsigned char *input = (unsigned char*)(image.data);
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		  
		  
	b= ((uchar*)(image.data + image.step*y))[x*3];
    g= ((uchar*)(image.data + image.step*y))[x*3+1];
    r= ((uchar*)(image.data + image.step*y))[x*3+2];
		//sprintf(buffer,"r=%d g=%d b=%d\n",r,g,b);
		  sprintf(buffer,"h=%d s=%d v=%d\n",b,g,r);
		  cout<<buffer<< endl;
		  	  
		  
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
  
}