#include "backgroundSubstractor.h"

#include<iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
using namespace cv;
using namespace std;
backgroundSubstractor::backgroundSubstractor()
{
}


backgroundSubstractor::~backgroundSubstractor()
{
}
Mat backgroundSubstractor:: backgroundSub(Mat background, Mat player)
{
	resize(background, background, Size(1280, 720));
	resize(player, player, Size(1280, 720));

	GaussianBlur(background, background, Size(21, 21), 0);
	GaussianBlur(player, player, Size(21, 21), 0);

	Mat diff;
	absdiff(background, player, diff);
	threshold(diff, diff, 50, (255), CV_THRESH_BINARY); // grayscale needed 
														//adaptiveThreshold(diff, diff, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 12, 2);
	medianBlur(diff, diff, 15);
	//dilate(diff, diff, 0, Point(-1, -1), 2, 1, 1);

	Mat im_floodfill = diff.clone();

	bitwise_not(im_floodfill, im_floodfill);
	floodFill(im_floodfill, cv::Point(0, 0), Scalar(255, 0, 0));
	//imshow("before", im_floodfill);
	//imshow("diff", diff);
	Vec3b colour5 = im_floodfill.at<Vec3b>(Point(0, 0));
	cout << colour5 << endl;
	diff = (diff | im_floodfill);
	//imshow("before", diff);

	Mat im_person = diff.clone();

	for (int i = 0; i < im_person.cols; i++)
		for (int j = 0; j < im_person.rows; j++) {
			Vec3b colour = im_person.at<Vec3b>(Point(i, j));
			if (colour.val[0] == 255 && colour.val[1] == 255 && colour.val[2] == 255) {
				im_person.at<Vec3b>(Point(i, j))[0] = 0;
				im_person.at<Vec3b>(Point(i, j))[1] = 0;
				im_person.at<Vec3b>(Point(i, j))[2] = 255;

			}

		}
	return im_person;
}


bool backgroundSubstractor:: resultCalculator(Mat wall, Mat player, int &score1)
{
	float rows = player.rows;
	float cols = player.cols;

	resize(wall, wall, Size(1280, 720));
	Mat im_person2 = player.clone();
	floodFill(im_person2, cv::Point(0, 0), Scalar(255, 0, 0));
	im_person2 = (player | wall);

	int pinkPixels = 0;
	for (int i = 0; i < im_person2.cols; i++)
		for (int j = 0; j < im_person2.rows; j++) {
			Vec3b colour = im_person2.at<Vec3b>(Point(i, j));
			if (colour.val[0] == 254 && colour.val[1] == 0 && colour.val[2] == 255)
				pinkPixels++;
		}
	float loseing_pixel = 0.0;
	loseing_pixel = pinkPixels * 100 / (rows * cols);
	score1 = loseing_pixel;
	if (loseing_pixel > 5.5)
		return false;
	else
	{
		int scoresystem = 0;
		for (int i = 0; i < im_person2.cols; i++)
			for (int j = 0; j < im_person2.rows; j++) {
				Vec3b colour = im_person2.at<Vec3b>(Point(i, j));
				if (colour.val[0] == 1 && colour.val[1] == 255 && colour.val[2] == 255)
					scoresystem++;

			}
		scoresystem = (scoresystem - pinkPixels) / 100;
		score1 += scoresystem;
		return true;
	}


}