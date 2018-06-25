//This Code is quote from https://qiita.com/NaotakaSaito/items/f1f1548c8b760629cd26

/*
   CvCapture -> RaspiCamCvCapture
   cvCreateCameraCapture -> raspiCamCvCreateCameraCapture
   cvQueryFrame -> raspiCamCvQueryFrame
   cvReleaseCapture -> raspiCamCvReleaseCapture
   cvGetCaptureProperty -> raspiCamCvGetCaptureProperty
*/
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <typeinfo>

#include "RaspiCamCV.h"

using namespace std;
using namespace cv;

Mat detectFaceInImage(Mat &image,string &cascade_file,int *num){
    CascadeClassifier cascade;
    cascade.load(cascade_file);

    vector<Rect> faces;
    cascade.detectMultiScale(image, faces, 1.1,3,0,Size(20,20));

    *num = faces.size();

    for (int i = 0; i < faces.size(); i++){
        rectangle(image, Point(faces[i].x,faces[i].y),Point(faces[i].x + faces[i].width,faces[i].y + faces[i].height),Scalar(0,200,0),3,CV_AA);
    }

    return image;
}

int main( int argc, const char** argv )
{

    //CvCapture* capture = 0;
    RaspiCamCvCapture* capture = 0;

    RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
    config->width=320;
    config->height=240;
    config->bitrate=0;  // zero: leave as default
    config->framerate=0;
    config->monochrome=0;

    // save 
    //VideoWriter writer("result.avi",CV_FOURCC_DEFAULT,10,Size(640,480),true);

    // --------------------------------------------------------------
    cv::Mat frame1,grayImage;

    // catch raspi-cam
    capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 

    cvNamedWindow( "origin", 1 );

    //cvSetMouseCallback("origin",on_mouse,0);
    if( capture )
    {
        cout << "In capture ..." << endl;
        for(;;)
        {

            //IplImage* iplImg = cvQueryFrame( capture );
            IplImage* iplImg1;
            iplImg1 = raspiCamCvQueryFrame( capture );
            frame1 = iplImg1;
            char msg[64];
            int numFace;

            // optimize frame
            cv::flip(frame1,frame1,0);

            // face detect
            string filename = "/home/pi/opencv-2.4/data/haarcascades/haarcascade_frontalface_default.xml";
            frame1=detectFaceInImage(frame1,filename,&numFace);

            // print number of face
            if(numFace){
                sprintf(msg,"%d",numFace);
                cout << msg << endl;
            }

            // display Image
            cv::imshow("origin", frame1);

            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }

        waitKey(0);

_cleanup_:

        //cvReleaseCapture( &capture );
        // change for raspicam
        raspiCamCvReleaseCapture(&capture);
    }
    cvDestroyWindow("origin");

    return 0;
}

