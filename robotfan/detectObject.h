#include "defineName.h"
#include <Windows.h>
#include <io.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <queue>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

// some utility functions
void MakeDir( const string& filepath );
void help( const char* progName );
void GetDirList( const string& directory, vector<string>* dirlist );
void GetFileList( const string& directory, vector<string>* filelist );

const string kVocabularyFile("/vocabulary.xml.gz");
const string kBowImageDescriptorsDir("/bagOfWords");
const string kSvmsDirs("/svms");

class Params 
{
public:
	Params(): wordCount( 1000 ), detectorType( "SURF" ),
			descriptorType( "SURF" ), matcherType( "FlannBased" ){ }
	
	int		wordCount;
	string	detectorType;
	string	descriptorType;
	string	matcherType;
};

char detectObject(Mat image);
Mat BuildVocabulary(const string& databaseDir, 
							   const vector<string>& categories, 
							   const Ptr<FeatureDetector>& detector, 
							   const Ptr<DescriptorExtractor>& extractor,
							   int wordCount);
void ComputeBowImageDescriptors(const string& databaseDir,
													const vector<string> &categories, 
													const Ptr<FeatureDetector>& detector,
													Ptr<BOWImgDescriptorExtractor>& bowExtractor,
													const string &imageDescriptorsDir,
													map<string, Mat>* samples);
void trainSVM(const map<string, Mat> &samples, 
					 const string &category, 
					 const CvSVMParams &svmParams,
					 CvSVM *svm);
string classifyBySVM( const Mat& queryDescriptor, 
								 const map<string, Mat>& samples, 
						    	 const string& svmDir);
void getFiles( string path, vector<string> &files);