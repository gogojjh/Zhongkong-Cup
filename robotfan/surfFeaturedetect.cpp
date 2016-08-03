#include "stdafx.h"
#include "surfFeaturedetect.h"

bool surfFeaturedetect(Mat testImage, string t_name)
{
	Mat image = imread(t_name);
	vector<KeyPoint>keypoints1, keypoints2;
	//构建SURF特征检测器
	SurfFeatureDetector surf(minHessian);//阈值
	//检测SURF特征
	surf.detect(image, keypoints1);
	cout << "模板图片特征点数: " << keypoints1.size() << endl;
	surf.detect(testImage, keypoints2);
	cout << "测试图片特征点数: " << keypoints2.size() << endl;

	//SURF特征匹配
	//构造SURF描述子提取器,用于特征性质
	SurfDescriptorExtractor surfDesc;
	//提取SURF描述子
	static Mat descriptors1, descriptors2;
	//构造N维描述子的向量
	surfDesc.compute(image, keypoints1, descriptors1);
	surfDesc.compute(testImage , keypoints2, descriptors2);
	if ( descriptors1.empty() )
		cvError(0,"MatchFinder","1st descriptor empty",__FILE__,__LINE__);    
	if ( descriptors2.empty() )
		 cvError(0,"MatchFinder","2nd descriptor empty",__FILE__,__LINE__);
	if(descriptors1.type()!=CV_32F) 
	{
		descriptors1.convertTo(descriptors1, CV_32F);
	}
	if(descriptors2.type()!=CV_32F) 
	{
		descriptors2.convertTo(descriptors2, CV_32F);
	}
	//构造匹配器
	static BruteForceMatcher< L2<float>> matcher;
	//匹配两幅图像的描述子
	vector<DMatch>matches;
	//mathces存储的是两个匹配特征的欧式距离,可用matches
	matcher.match(descriptors1, descriptors2, matches);

	double maxDist = 0;  
    double minDist = 100;  
    for( int i = 0; i < descriptors1.rows; i++ )  
    {  
        double dist = matches[i].distance;  
        if( dist < minDist )  minDist = dist;  
        if( dist > maxDist ) maxDist = dist;  
    }  
    cout << "最大距离: " << maxDist << " 最小距离: " << minDist << endl;
	
	//筛选好的匹配点
    vector< DMatch > goodMatches;  
    for( int i = 0; i < descriptors1.rows; i++ )  
    {  
        if( matches[i].distance < 2*0.1 )  
			goodMatches.push_back( matches[i]);   
    }  
    cout<< "*******************好的匹配点数目: "<<goodMatches.size()<<endl;  
	/*
	Mat resultImg;  
    drawMatches( image, keypoints1, testImage, keypoints2,   
        goodMatches, resultImg, Scalar::all(-1), Scalar::all(-1), vector<char>(),   
        DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);   
	imshow("经筛选的匹配结果", resultImg);
	waitKey();*/

	if (1.0*goodMatches.size()/keypoints1.size() >= t) return true;
	    															  else return false;
}