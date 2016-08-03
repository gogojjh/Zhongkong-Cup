#include "stdafx.h"
#include "detectObject.h"

char detectObject(Mat testimage)
{
	Params params;
	string databaseDir = "D:\\user\\Documents\\VS2010\\robotfan\\data\\train";
	string testDir = "D:\\user\\Documents\\VS2010\\robotfan\\data\\testobject";
	string resultDir = "D:\\user\\Documents\\VS2010\\robotfan\\data\\result";
	string bowImageDescriptorsDir = resultDir + kBowImageDescriptorsDir;
	string svmsDir = resultDir + kSvmsDirs;

	vector<string> categories;
	getFiles( databaseDir, categories );

	Ptr<FeatureDetector> detector = FeatureDetector::create( params.descriptorType );
	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create( params.descriptorType );
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create( params.matcherType );
	if ( detector.empty() || extractor.empty() || matcher.empty() ) 
	{
		cout << "feature detector or descriptor extractor or descriptor matcher cannot be created.\n Maybe try other types?" << endl;
	}
	Mat vocabulary;
	string vocabularyFile = resultDir + "\\" + kVocabularyFile;
	FileStorage fs(vocabularyFile, FileStorage::READ);
	if (fs.isOpened())
	{
		fs["vocabulary"] >> vocabulary;
	}
	else
	{
		vocabulary = BuildVocabulary( databaseDir, categories, detector, extractor, params.wordCount );
		FileStorage fs( vocabularyFile, FileStorage::WRITE );
		if ( fs.isOpened() ) 
		{
			fs << "vocabulary" << vocabulary;
		}
	}

	Ptr<BOWImgDescriptorExtractor> bowExtractor = new BOWImgDescriptorExtractor( extractor, matcher );
	bowExtractor -> setVocabulary( vocabulary );
	map<string, Mat> samples;//key: category name, value: histogram
	
	ComputeBowImageDescriptors( databaseDir, categories, detector, bowExtractor, bowImageDescriptorsDir,  &samples );
	
	vector<KeyPoint> keypoints;
	detector->detect(testimage, keypoints); //特征点提取
	Mat testDescriptor;
	bowExtractor->compute(testimage, keypoints, testDescriptor); //构建词带
	string category;
	category = classifyBySVM(testDescriptor, samples, svmsDir); //分类
	cout << "I think it should be " << category << "." << endl;
	
	char c;
		if (category=="jianyikele") c=JIANYIKELE;
 else if (category=="fenda") c=FENDA;
 else if (category=="xuebi") c=XUEBI;
 else if (category=="yangleduo") c=YANGLEDUO;
 else if (category=="mengniu_chun") c=MENGNIU;
 else if (category=="qqxing") c=QQXING;
 else if ((category=="nothing") || (category=="meinianda") || (category=="baishikele") || (category=="xuehua")
			|| (category=="qqxing_cartoon") || (category=="yili") || (category=="youjun") 
			|| (category=="youyi") || (category=="baishikele_lan")) c=NOTHING;
	/*
	char c = NOTHING;
	int sum = 0, count = 0;
	for (int index=0; index!=categories.size(); index++)
	{
		//打开每张train图片
		if (categories[index]=="") continue;
		//cout << "processing category " << categories[index] << endl;
		string currentCategory = testDir + "\\" + categories[index];
		vector<string> filelist;
		getFiles(currentCategory, filelist);
		for ( auto fileindex = filelist.begin(); fileindex != filelist.end(); ++fileindex ) 
		{		
			if (*fileindex=="") continue;
			string testImageName = currentCategory + "\\" + *fileindex;
			Mat testimage = imread(testImageName);
			vector<KeyPoint> keypoints;
			detector->detect(testimage, keypoints);
			Mat testDescriptor;
			bowExtractor->compute(testimage, keypoints, testDescriptor);
			string category;
			category = classifyBySVM(testDescriptor, samples, svmsDir);
			if (category == categories[index]) count++;
			sum++;
			cout << categories[index] << "   " << category << endl;
		}
	}
	cout << "Number of testing = " << sum << " Success = " << count << " Possiblity = " << count/sum << endl;
	*/
	return c;
}

//提取特征点+聚类操作找代表点,每个代表点就比作一个word
Mat BuildVocabulary(const string& databaseDir, 
							   const vector<string>& categories, 
							   const Ptr<FeatureDetector>& detector, 
							   const Ptr<DescriptorExtractor>& extractor,
							   int wordCount)
{
	Mat allDescriptors;
	for (int index=0; index!=categories.size(); index++)
	{
		//打开每张train图片
		if (categories[index]=="") continue;
		cout << "processing category " << categories[index] << endl;
		string currentCategory = databaseDir + "\\" + categories[index];
		vector<string> filelist;
		getFiles(currentCategory, filelist);
		for ( auto fileindex = filelist.begin(); fileindex != filelist.end(); ++fileindex ) 
		{		
			string filepath = currentCategory + "\\" + *fileindex;
			Mat image = imread(filepath);
			if (image.empty())
			{
				continue;
			}
			vector<KeyPoint> keypoints;
			Mat descriptors;
			SurfFeatureDetector surf(50);
			surf.detect( image, keypoints);
			extractor->compute( image, keypoints, descriptors );
			if (allDescriptors.empty())
			{
				allDescriptors.create(0, descriptors.cols, descriptors.type());
			}
			allDescriptors.push_back(descriptors); //存储每张图片的特征点描述符
		}
		//提取每张图片的特征点并构造feature聚类,返回中心点
		cout << "done processing category " << categories[index] << endl;
	}
	assert(!allDescriptors.empty());//????
	cout << "build vocabulary..." << endl;
	BOWKMeansTrainer bowTrainer(wordCount);
	Mat vocabulary = bowTrainer.cluster(allDescriptors);
	cout << "done build vocabulary..." << endl;
	return vocabulary;
}

//统计每种类别的特征在每张图片中的出现频率(类似直方图),作为这图片的词包
//只需把每张图片的特征点作为输出,vocablary处理就可算出
void ComputeBowImageDescriptors(const string& databaseDir,
													const vector<string> &categories, 
													const Ptr<FeatureDetector>& detector,
													Ptr<BOWImgDescriptorExtractor>& bowExtractor,
													const string &imageDescriptorsDir,
													map<string, Mat>* samples)
{
	for (auto i=0; i!=categories.size(); i++)
	{
		if (categories[i]=="") continue;
		string currentCategory = databaseDir + "\\" + categories[i];
		vector<string> filelist;
		getFiles(currentCategory, filelist);
		for ( auto fileitr = filelist.begin(); fileitr != filelist.end(); ++fileitr ) 
		{
			string descriptorFileName = imageDescriptorsDir + "\\" + ( *fileitr ) + ".xml.gz";
			FileStorage fs( descriptorFileName, FileStorage::READ );
			Mat imageDescriptor;
			if (fs.isOpened())
			{
				fs["imageDescriptor"] >> imageDescriptor;
			}
			else
			{
				string filepath = currentCategory + "\\" + *fileitr;
				Mat image = imread( filepath );
				if (image.empty())
				{
					continue;
				}
				vector<KeyPoint> keyPoints;
				detector->detect( image, keyPoints );
				bowExtractor->compute( image, keyPoints, imageDescriptor );				
				fs.open(descriptorFileName, FileStorage::WRITE);
				if (fs.isOpened())
				{
					fs << "imageDescriptor" << imageDescriptor;
				}
			}
			if (samples->count(categories[i])==0)
			{
				(*samples)[categories[i]].create( 0, imageDescriptor.cols, imageDescriptor.type() );
			}
			(*samples)[categories[i]].push_back( imageDescriptor );
		}
	}
}
	
//对每个类别都训练一个二元分类器.对于待分类的特征使用每个分类器计算在该类的可能性,选最高的.
//属于category的样本,label=1, 不属于为-1.准备好每个样本及其对应的label之后调用CvSvm的train方法
void trainSVM(const map<string, Mat> &samples, 
					 const string &category, 
					 const CvSVMParams &svmParams,
					 CvSVM *svm)
{
	Mat allSamples(0, samples.at(category).cols, samples.at(category).type());
	Mat responses(0, 1, CV_32SC1);
	
	allSamples.push_back(samples.at(category));
	Mat posResponses(samples.at(category).rows, 1, CV_32SC1, Scalar::all(1));
	responses.push_back(posResponses);

	for (auto itr = samples.begin(); itr != samples.end(); itr++)
	{
		if (itr->first == category)
		{
			continue;
		}
		allSamples.push_back(itr->second);
		Mat response(itr->second.rows, 1, CV_32SC1, Scalar::all(-1));
		responses.push_back(response);
	}		
	svm->train(allSamples, responses, Mat(), Mat(), svmParams);
}

//使用某张待分类图片的bow输入,使用分类器计算可能性,取最大
string classifyBySVM( const Mat& queryDescriptor, 
								 const map<string, Mat>& samples, 
						    	 const string& svmDir )
{
	string category;
	SVMParams svmParams;
	int sign = 0;
	float confidence = -2.0f;
	for (auto itr = samples.begin(); itr != samples.end(); itr++)
	{
		CvSVM svm;
		string svmFileName = svmDir + "\\" + itr->first + ".xml.gz";
		FileStorage fs(svmFileName, FileStorage::READ);
		if (fs.isOpened())
		{
			fs.release();
			svm.load(svmFileName.c_str());
		}
		else
		{
			trainSVM(samples, itr->first, svmParams, &svm);
			if (!svmDir.empty())
			{
				svm.save(svmFileName.c_str());
			}
		}
		if (sign == 0)
		{
			float scoreValue = svm.predict(queryDescriptor, true);
			float classValue = svm.predict(queryDescriptor, false);
			//分类1 or -1			
			sign = ( scoreValue < 0.0f ) == ( classValue < 0.0f )? 1 : -1;
		}
		float curConfidence = sign*svm.predict(queryDescriptor, true);
		if (curConfidence>confidence)
		{
			confidence = curConfidence;
			category = itr->first;
		}		
	}		
	return category;
}

void getFiles(string path, vector<string> &files)
{
	fstream fp;
	char save[100];
	string p = "dir /b "+path+" >> D:\\user\\Documents\\VS2010\\robotfan\\category.txt";
	system(p.c_str());
	fp.open("D:\\user\\Documents\\VS2010\\robotfan\\category.txt", ios::in);
	while(!fp.eof())  
    {  
        fp.getline(save, 100, '\n');
        //cout << save << endl;
		files.push_back(save);
    }  
	fp.close();
	system("del /Q D:\\user\\Documents\\VS2010\\robotfan\\category.txt");
}

/*void getFiles( string path, vector<string> &files )
{
	//文件句柄
	long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			/*if((fileinfo.attrib &  _A_SUBDIR))
			{
				//if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );

		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}*/

/*char detectObject(Mat image1)
{
	vector<KeyPoint>keypoints1, keypoints2;
	//构建SURF特征检测器
	SurfFeatureDetector surf(2500.); //阈值
	//检测SURF特征
	surf.detect(image1, keypoints1);

	//绘制特征点
	Mat featureimage;
	drawKeypoints(image1, 
			keypoints1, //特征点向量
			featureimage, 
			Scalar(255,255,255), 
			DrawMatchesFlags::DRAW_RICH_KEYPOINTS); //标志位
	imshow("detectObject SURFFEATURE", featureimage);	
	
	Mat image2 = imread("../objecttemplate.jpg");
	surf.detect(image2, keypoints2);

	//SURF特征匹配
	//构造SURF描述子提取器,用于特征性质
	SurfDescriptorExtractor surfDesc;
	//提取SURF描述子
	Mat descriptors1, descriptors2;
	//构造N维描述子的向量
	surfDesc.compute(image1, keypoints1, descriptors1);
	surfDesc.compute(image2, keypoints2, descriptors2);
	//构造匹配器
	FlannBasedMatcher matcher;
	//匹配两幅图像的描述子
	vector<DMatch>matches;
	//mathces存储的是两个匹配特征的欧式距离,可用matches
	matcher.match(descriptors1, descriptors2, matches);
	cout << "before filtering  " << matches.size() << endl;

	//对匹配结果进行前N个筛选
	int N = 25;
	nth_element(matches.begin(), matches.begin()+24, matches.end());
	matches.erase(matches.begin()+25, matches.end());
	Mat imageMatches;
	drawMatches(image1, keypoints1, image2, keypoints2,
		matches, imageMatches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	cout << "before filtering  " << matches.size() << endl;
	imshow("deteceObject SURFMATCH", imageMatches);
	
	waitKey();
	return XUEBI;
}
*/
/*对matches进行筛选
  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_1.rows; i++ )
  { if( matches[i].distance < 2*min_dist )
    { good_matches.push_back( matches[i]); }
  }

*/



