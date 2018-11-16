/****************
@
@Author:Limbo
@
****************/
#include "ycylib2.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
using namespace cv;
using namespace std;
Mat imgProcess(const Mat &image, Mat &sample, float th, int num) {
	ColorHistogram hc;
	ContentFinder finder;
	MatND colorhist;
	// rbg
	if (num == 0) {
		colorhist = hc.getHistogram(sample);
		finder.setHistogram(colorhist);
		finder.setThreshold(th);
		Mat result = finder.find2(image);
		return result;
	}
	//hsv
	else if (num == 1) {
		Mat hsv;
		colorhist = hc.getHueHistogram(sample);
		finder.setHistogram(colorhist);
		finder.setThreshold(th);
		cvtColor(image, hsv, CV_BGR2HSV);
		// int ch[2] = {1, 2};
		// ch[0] = 0;
		int ch[1]={0};
		Mat result = finder.find(hsv, 0.0f, 180.0f, ch, 1);
		return result;
	}
	//both
	/*else if(num=2){
	Mat hsv;
		ColorHistogram hc;
		MatND colorhist=hc.getHueHistogram(sample);
		ContentFinder finder;
		finder.setHistogram(colorhist);
		finder.setThreshold(0.02f);
		cvtColor(image,hsv,CV_BGR2HSV);
		int ch[2]={1,2};
		ch[0]=0;
		Mat result1=finder.find(hsv,0.0f,180.0f,ch,1);

		//RGB histogram
		ColorHistogram hc2;
		image=hc2.colorReduce(image,100);
		sample=hc2.colorReduce(sample,100);
		MatND colorhist2=hc2.getHistogram(sample);
		ContentFinder finder2;
		finder2.setHistogram(colorhist2);
		finder2.setThreshold(0.09f);
		Mat result2=finder2.find2(image);
	Mat result=result1 &result2;
	return result;
	} */

}
void draw(const Mat &image) {
	vector < std::vector<cv::Point> > contours;
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat contoursimg(image.size(), CV_8U, Scalar(255));
	Mat conimg = contoursimg.clone();
	drawContours(conimg, contours, -1, Scalar(0), 2);
	int cmax;
	int s[1000], i = 1;
	if (contours.size() > 0)
	{
		vector<std::vector<cv::Point> >::iterator itc = contours.begin();
		while (itc != contours.end())
		{
			s[i] = itc->size();
			cout << s[i] << endl;
			++i;
			++itc;
		}
		for (int l = 2; l <= i - 1; l++)
			if (s[l - 1] > s[l])
				s[l] = s[l - 1];
		cmax = s[i - 1];
		itc = contours.begin();
		while (itc != contours.end())
		{
			if (itc->size() < cmax)
			{
				itc = contours.erase(itc);
			}
			else
				++itc;
		}
		vector<vector<Point> > contours_poly( contours.size() );
		vector<Rect> boundRect( contours.size() );
		vector<Point2f>center( contours.size() );
		vector<float>radius( contours.size() );
		for ( int i = 0; i < contours.size(); i++ )
		{
			approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
			boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
		}
		Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
		for ( int i = 0; i < contours.size(); i++ )
		{
			Scalar color = Scalar( 150, 70, 0 );
			drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			//tl是左上角坐标, br是右下角坐标
			rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
			circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
		}

		imshow("cont", drawing);

	}
}
/*Mat isOpenClose(const Mat &image,int num){
 	Mat element5(5, 5, CV_8U, Scalar(1));
 if(num=1){
	Mat closed;
	morphologyEx(image,closed, MORPH_CLOSE, element5);
	return closed;
 }
 else if(num=0){
	Mat open;
	morphologyEx(image,open,MORPH_OPEN,element5);
	return open;
 }
 else if(num=2){
  			Mat element5(5, 5, CV_8U, Scalar(1));
		Mat closed;
		morphologyEx(image, closed, MORPH_CLOSE, element5);

		Mat open;
		morphologyEx(image, open, MORPH_OPEN, element5);
		return open;
 }
}*/
