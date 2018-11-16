// svm.cpp : 定义控制台应用程序的入口点。

#include "opencv2/opencv.hpp"
using namespace cv;
using namespace cv::ml;
using namespace std;

class wxzSVM
{
  private:
	int center_Y;
	int center_X;
	int down_Y;
	int up_Y;
	int Area;

  public:
	Mat svm(Mat &image, int type); //svm分类
	Mat SVMFind(int type, Mat image);
	void printInfo();
	int center_y();
	int center_x();
	int down_y();
	int up_y();
	int area();
};
