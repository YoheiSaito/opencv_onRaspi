#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#pragma comment(lib, "opencv_core")
#pragma comment(lib, "opencv_highgui")
#pragma comment(lib, "opencv_imgproc")

int main(void){
	cv::Mat srcImg = cv::imread("lenna.jpg");
	if(srcImg.empty())
		return -1;
	
	cv::Point2f center = cv::Point2f(static_cast<float>(srcImg.cols/2), static_cast<float>(srcImg.rows/2));
	
	double degree = 180;
	double scale = 1.0;
	
	cv::Mat affine;
	cv::getRotationMatrix2D(center, degree, scale).copyTo(affine);
	cv::warpAffine(srcImg, srcImg, affine, srcImg.size(), cv::INTER_CUBIC);
	
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("image", srcImg);
	cv::waitKey();
	
	return 0;
}
