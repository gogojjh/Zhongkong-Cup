#include "stdafx.h"
#include "serialPort.h"
//#include "manager.h"

#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <stdlib.h>

using namespace std;

VideoCapture capture(0);

int main()
{
	char result;
	int number = 2;
	//result = manager('D', number);/*
	Mat img;
	waitKey(2000);
    capture >> img;
	imwrite("../video/t1.jpg",img);
	waitKey(2000);
	capture >> img;
	imwrite("../video/t2.jpg",img);

	CSerialPort mySerialPort;
	extern char status;
	if (!mySerialPort.InitPort(3))
	{
		cout << "initPort fail !" << std::endl;
	}
	else
	{
		cout << "initPort success !" << std::endl;
	}

	if (!mySerialPort.OpenListenThread())
	{
		cout << "OpenListenThread fail !" << std::endl;
	}
	else
	{
		cout << "OpenListenThread success !" << std::endl;
	}
	int temp;
	cin >> temp;
	system("pause");
	return 0;
}
