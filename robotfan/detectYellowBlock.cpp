#include "stdafx.h"
#include "detectBlock.h"
#include "detectYellowBlock.h"
#include "manager.h"

char detectYellowBlock(Mat image)
{
	int T=15; //面积与边长之比的阈值
	//关于image的直方图对象
	ColorHistogram hc;	
	MatND colorhist = hc.getHueHistogram(image);
	//遍历直方图数据
	//hc.getHistogramStat(colorhist);
	/*
	//绘制直方图
	Mat histImg = hc.getHistogramImage(colorhist);
	namedWindow("YellowBlockHistogram");
	imshow("YellowBlockHistogram", histImg);*/

	Mat thresholded, thresholded1, thresholded2, thresholded3;
	threshold(hc.v[0], thresholded1, 10, 255, 1); //h空间
	threshold(hc.v[0], thresholded2, 30, 255, 0); //h空间
	threshold(hc.v[1], thresholded3, 125, 255, 1); //s空间 变成黑色
	thresholded = thresholded1+thresholded2+thresholded3;
	//imshow("1", thresholded1);
	//imshow("2", thresholded2);
	//imshow("3", thresholded3);

	//namedWindow("YellowBlockBinary");
	//imshow("YellowBlockBinary", thresholded);
	int top = (int) (0.05*thresholded.rows); 
	int bottom = (int) (0.05*thresholded.rows);
    int left = (int) (0.05*thresholded.cols); 
	int right = (int) (0.05*thresholded.cols);
	Scalar value = Scalar( 255 );
    copyMakeBorder( thresholded, thresholded, top, bottom, left, right, 0, value );
	
	
	/*Mat opened;
	dilate(thresholded, opened, Mat());
	namedWindow("DilatedImage");
	imshow("DilatedImage", opened);
	
	Mat dilated;
	dilate(thresholded, dilated, Mat());
	namedWindow("DilatedImage");
	imshow("DilatedImage", dilated);*/

	//闭运算
	Mat closed;
	morphologyEx(thresholded, closed, MORPH_CLOSE,  Mat());
	//namedWindow("ClosedImage");
	//imshow("ClosedImage", closed);
	
	//绘制轮廓
	vector<vector<Point>>contours;
	findContours(closed, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//筛选不合格轮廓
	int cmin = 100; //最小轮廓长度
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())	
	{
		if (itc->size()<cmin)
			itc = contours.erase(itc);
		else
			itc++;
	}

	Mat result(closed.size(), CV_8U, Scalar(255));
	double area, length, p;
	double a[2] = {0,0}; 
	cout << "Size=" << contours.size() << endl;
	for ( int i=0; i<contours.size(); i++)
	{
		area = abs(contourArea( contours[i] ));
		length = abs(arcLength( contours[i], true ));
		p = area/length;
		if (p > a[0]) 
		{
			a[1] = a[0];
			a[0] = p;
		}
		else if (p > a[1]) a[1] = p; 
 		cout << "Area=" << area << "   " << "Length=" << length << "  " << "Property=" << p << endl;
	}
	drawContours(result, contours, -1, Scalar(0), 1);
	//namedWindow("DrawContours");
	//imshow("DrawContours", result);
	cout << "Property=" << a[1] << endl;
	//waitKey();
	
	if (a[1] > T) return YELLOWBLOCK;
			  else return NOTHING;
}