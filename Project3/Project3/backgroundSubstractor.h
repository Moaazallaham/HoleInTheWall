#pragma once
#include<iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

#include <opencv2/imgproc/imgproc.hpp>  
using namespace cv;
using namespace std;
class backgroundSubstractor
{
public:
	backgroundSubstractor();
	~backgroundSubstractor();

	Mat backgroundSubstractor::backgroundSub(Mat background, Mat player);

	bool backgroundSubstractor::resultCalculator(Mat wall, Mat player, int &score1);
	
};




