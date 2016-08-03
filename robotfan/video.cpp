#include "stdafx.h"
//调用摄像头
#include "video.h"

extern VideoCapture capture;

char filename[200];

Mat video(int &number)
{
	number++;
	Mat frame;
    capture >> frame;
    sprintf(filename, "../video/%d.jpg", number);
	cout << "number = " << number << endl;
	imwrite(filename, frame);
	/*namedWindow("src", 0);
	imshow("src", frame);
	cvWaitKey( 0 );*/
	//Mat frame = imread("../video/43.jpg");
	return frame;
}
