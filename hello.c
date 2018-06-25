#include <cv.h>
#include <highgui.h>
#include <stdio.h>

int main (int argc, char **argv)
{
    double w = 640, h = 480;
 
    CvCapture *capture = cvCreateCameraCapture(-1);
    if (capture==NULL) {
        printf("Camera not found.\n");
        return 0;
    }
    
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);
 
    // ウィンドウを作成する
    char *windowName = "camera";
    cvNamedWindow( windowName, CV_WINDOW_AUTOSIZE );
    
    while (1) {
        IplImage *frame = cvQueryFrame(capture);
        cvShowImage( windowName, frame );
        
        cvWaitKey( 1 );
    }
 
    
    cvReleaseCapture (&capture);
    cvDestroyWindow (windowName);
    
    return 0;
}
