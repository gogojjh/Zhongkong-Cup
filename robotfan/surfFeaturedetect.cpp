#include "stdafx.h"
#include "surfFeaturedetect.h"

bool surfFeaturedetect(Mat testImage, string t_name)
{
	Mat image = imread(t_name);
	vector<KeyPoint>keypoints1, keypoints2;
	//����SURF���������
	SurfFeatureDetector surf(minHessian);//��ֵ
	//���SURF����
	surf.detect(image, keypoints1);
	cout << "ģ��ͼƬ��������: " << keypoints1.size() << endl;
	surf.detect(testImage, keypoints2);
	cout << "����ͼƬ��������: " << keypoints2.size() << endl;

	//SURF����ƥ��
	//����SURF��������ȡ��,������������
	SurfDescriptorExtractor surfDesc;
	//��ȡSURF������
	static Mat descriptors1, descriptors2;
	//����Nά�����ӵ�����
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
	//����ƥ����
	static BruteForceMatcher< L2<float>> matcher;
	//ƥ������ͼ���������
	vector<DMatch>matches;
	//mathces�洢��������ƥ��������ŷʽ����,����matches
	matcher.match(descriptors1, descriptors2, matches);

	double maxDist = 0;  
    double minDist = 100;  
    for( int i = 0; i < descriptors1.rows; i++ )  
    {  
        double dist = matches[i].distance;  
        if( dist < minDist )  minDist = dist;  
        if( dist > maxDist ) maxDist = dist;  
    }  
    cout << "������: " << maxDist << " ��С����: " << minDist << endl;
	
	//ɸѡ�õ�ƥ���
    vector< DMatch > goodMatches;  
    for( int i = 0; i < descriptors1.rows; i++ )  
    {  
        if( matches[i].distance < 2*0.1 )  
			goodMatches.push_back( matches[i]);   
    }  
    cout<< "*******************�õ�ƥ�����Ŀ: "<<goodMatches.size()<<endl;  
	/*
	Mat resultImg;  
    drawMatches( image, keypoints1, testImage, keypoints2,   
        goodMatches, resultImg, Scalar::all(-1), Scalar::all(-1), vector<char>(),   
        DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);   
	imshow("��ɸѡ��ƥ����", resultImg);
	waitKey();*/

	if (1.0*goodMatches.size()/keypoints1.size() >= t) return true;
	    															  else return false;
}