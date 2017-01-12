//Note:5x4.5 inch rectangle is used for refrence calculation
//use run the compile file in the folder to compile the source code  
//use the inluce and lib files in the folder to compile the code.
// Headers for Open-Source Computer Vision (OPENCV)
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include<stdio.h>
#define AREF 3000
#define DREF 22

void CallBackFunc(int event, int x, int y, int flags, void* userdata);
using namespace cv;
using namespace std;
	Mat frame;
RNG rng(12345);
int main()
{
float area;
float distance;

namedWindow("orginal");

setMouseCallback("orginal", CallBackFunc, NULL);
VideoCapture capture(0); //Create a VideoCapture object

 capture.set(CV_CAP_PROP_FRAME_WIDTH,360);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);

if (!capture.isOpened()) //Check if capture successfully opned
	return -1;



  vector<vector<Point> > contours_poly( 40);
  vector<Rect> boundRect( 40 );
  vector<Point2f>center( 40 );
  vector<float>radius( 40 );
  vector<vector<Point> > contours;
  	Mat  frame2, processed;      	//Create a container for current video frame

  Scalar color =  Scalar(0,255,0);
while(1) {



	if(!capture.read(frame))
		break;

	cvtColor(frame,processed, COLOR_BGR2HSV);
	cvtColor(frame,frame2, COLOR_BGR2GRAY);

	inRange(processed, Scalar(13,160,140), Scalar(16,255, 255),processed);

	Mat erode_element(10,10,CV_8U,Scalar(1));
	Mat dilate_element(10,10,CV_8U,Scalar(1));

	dilate(processed,processed,dilate_element);
	erode(processed,processed,erode_element);
	
	erode(processed,processed,erode_element);
	dilate(processed,processed,dilate_element);

	

	
	findContours(processed, contours, CV_RETR_EXTERNAL,
	CV_CHAIN_APPROX_NONE);

	//cout << "contours = " << contours.size() << endl;


 for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }
	 
	   for( int i = 0; i< contours.size(); i++ )
     {
		if(1)
		{
       drawContours( frame, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
		if(radius[i] >20)
		{
		area=boundRect[i].area();
		distance=(float)DREF* (float)sqrt((AREF/area));
		}
	  cout << "radius at " <<i<<" ="<<radius[i] <<"Area="<<boundRect[i].area()<<" distance="<<distance <<" Inch"<< endl;
       circle( frame, center[i], (int)radius[i], color, 2, 8, 0 );
     }
	 }








	imshow("orginal",frame);	
	

	if (waitKey(2)>=0)
		break;
	
}


capture.release();
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
 unsigned char b,g,r;
char buffer[32];
 unsigned char *input = (unsigned char*)(frame.data);
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		  
		  
	b= ((uchar*)(frame.data + frame.step*y))[x*3];
    g= ((uchar*)(frame.data + frame.step*y))[x*3+1];
    r= ((uchar*)(frame.data + frame.step*y))[x*3+2];
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
