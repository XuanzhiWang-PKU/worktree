/****************
@
@Author:Limbo
@
****************/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;



/*******************************************************************************************************************/
class ColorHistogram {

private:

    int histSize[3];
    float hranges[2];
    const float* ranges[3];
    int channels[3];

public:

    ColorHistogram() {

        // Prepare arguments for a color histogram
        histSize[0] = histSize[1] = histSize[2] = 256;
        hranges[0] = 0.0;   // BRG range
        hranges[1] = 255.0;
        ranges[0] = hranges; // all channels have the same range
        ranges[1] = hranges;
        ranges[2] = hranges;
        channels[0] = 0;    // the three channels
        channels[1] = 1;
        channels[2] = 2;
    }

    // Computes the histogram.
    cv::MatND getHistogram(const cv::Mat &image) {

        cv::MatND hist;

        // BGR color histogram
        hranges[0] = 0.0;   // BRG range
        hranges[1] = 255.0;
        channels[0] = 0;    // the three channels
        channels[1] = 1;
        channels[2] = 2;

        // Compute histogram
        cv::calcHist(&image,
                     1,          // histogram of 1 image only
                     channels,   // the channel used
                     cv::Mat(),  // no mask is used
                     hist,       // the resulting histogram
                     3,          // it is a 3D histogram
                     histSize,   // number of bins
                     ranges      // pixel value range
                    );

        return hist;
    }

    // Computes the histogram.
    cv::SparseMat getSparseHistogram(const cv::Mat &image) {

        cv::SparseMat hist(3, histSize, CV_32F);

        // BGR color histogram
        hranges[0] = 0.0;       // BRG range
        hranges[1] = 255.0;
        channels[0] = 0;    // the three channels
        channels[1] = 1;
        channels[2] = 2;

        // Compute histogram
        cv::calcHist(&image,
                     1,          // histogram of 1 image only
                     channels,           // the channel used
                     cv::Mat(),          // no mask is used
                     hist,       // the resulting histogram
                     3,          // it is a 3D histogram
                     histSize,           // number of bins
                     ranges      // pixel value range
                    );

        return hist;
    }

    // Computes the 2D ab histogram.
    // BGR source image is converted to Lab
    cv::MatND getabHistogram(const cv::Mat &image) {

        cv::MatND hist;

        // Convert to Lab color space
        cv::Mat lab;
        cv::cvtColor(image, lab, CV_BGR2Lab);

        // Prepare arguments for a 2D color histogram
        hranges[0] = -128.0;
        hranges[1] = 127.0;
        channels[0] = 1; // the two channels used are ab
        channels[1] = 2;

        // Compute histogram
        cv::calcHist(&lab,
                     1,          // histogram of 1 image only
                     channels,   // the channel used
                     cv::Mat(),  // no mask is used
                     hist,       // the resulting histogram
                     2,          // it is a 2D histogram
                     histSize,   // number of bins
                     ranges      // pixel value range
                    );

        return hist;
    }

    // Computes the 1D Hue histogram with a mask.
    // BGR source image is converted to HSV
    cv::MatND getHueHistogram(const cv::Mat &image) {

        cv::MatND hist;

        // Convert to Lab color space
        cv::Mat hue;
        cv::cvtColor(image, hue, CV_BGR2HSV);

        // Prepare arguments for a 1D hue histogram
        hranges[0] = 0.0;
        hranges[1] = 180.0;
        channels[0] = 0; // the hue channel

        // Compute histogram
        cv::calcHist(&hue,
                     1,          // histogram of 1 image only
                     channels,   // the channel used
                     cv::Mat(),  // no mask is used
                     hist,       // the resulting histogram
                     1,          // it is a 1D histogram
                     histSize,   // number of bins
                     ranges      // pixel value range
                    );

        return hist;
    }

    cv::MatND getHueHistogram_H(const cv::Mat &image, int minH = 0) {
        MatND hist;
        Mat hsv;
        cvtColor(image, hsv, CV_BGR2HSV);
        Mat mask;
        if (minH > 0) {
            std::vector<Mat>v;
            split(hsv, v);
            threshold(v[0], mask, minH, 255, cv::THRESH_BINARY);
        }
        hranges[0] = 0.0;
        hranges[1] = 180.0;
        channels[0] = 0; // the hue channel

        cv::calcHist(&hsv,
                     1,          // histogram of 1 image only
                     channels,   // the channel used
                     cv::Mat(),  // no mask is used
                     hist,       // the resulting histogram
                     1,          // it is a 1D histogram
                     histSize,   // number of bins
                     ranges      // pixel value range
                    );
        return hist;

    }
    cv::MatND getHueHistogram_S(const cv::Mat &image, int minS = 0) {
        MatND hist;
        Mat hsv;
        cvtColor(image, hsv, CV_BGR2HSV);
        Mat mask;
        if (minS > 0) {
            std::vector<Mat>v;
            split(hsv, v);
            threshold(v[1], mask, minS, 255, cv::THRESH_BINARY);
        }
        hranges[0] = 0.0;
        hranges[1] = 180.0;
        channels[0] = 0; // the hue channel

        cv::calcHist(&hsv,
                     1,          // histogram of 1 image only
                     channels,   // the channel used
                     cv::Mat(),  // no mask is used
                     hist,       // the resulting histogram
                     1,          // it is a 1D histogram
                     histSize,   // number of bins
                     ranges      // pixel value range
                    );
        return hist;

    }
    cv::Mat colorReduce(const cv::Mat &image, int div = 64) {

        int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0));
        // mask used to round the pixel value
        uchar mask = 0xFF << n; // e.g. for div=16, mask= 0xF0

        cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
        cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();

        // Set output image (always 1-channel)
        cv::Mat result(image.rows, image.cols, image.type());
        cv::Mat_<cv::Vec3b>::iterator itr = result.begin<cv::Vec3b>();

        for ( ; it != itend; ++it, ++itr) {

            (*itr)[0] = ((*it)[0] & mask) + div / 2;
            (*itr)[1] = ((*it)[1] & mask) + div / 2;
            (*itr)[2] = ((*it)[2] & mask) + div / 2;
        }

        return result;
    }
};

/**********************************************************************************************************************/

//ContentFinder

class ContentFinder {
private:
    float hranges[2];
    const float*ranges[3];
    int channels[3];
    float threshold;
    MatND histogram;
public:
    ContentFinder(): threshold(-1.0f) {
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
    }
    void setThreshold(float t) {
        threshold = t;
    }
    float getThreshold() {
        return threshold;
    }
    void setHistogram(const MatND& h) {
        histogram = h;
        normalize(histogram, histogram, 1.0);
    }
    Mat find(const Mat& image, float min, float max, int*channels, int dim) {
        Mat result;

        hranges[0] = min;   // range [0,255]
        hranges[1] = max;
        for (int i = 0; i < dim; i++)
            this->channels[i] = channels[i]; // the three channels
        cv::calcBackProject(&image,
                            1,            // one image
                            channels,     // vector specifying what histogram dimensions belong to what image channels
                            histogram,    // the histogram we are using
                            result,       // the resulting back projection image
                            ranges,       // the range of values, for each dimension
                            255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
                           );
        if (threshold > 0.0)
            cv::threshold(result , result , 255 * threshold , 255 , THRESH_BINARY ) ;
        return result;
    }
    Mat find2(const Mat& image) {
        Mat result;

        hranges[0] = 0.0;   // range [0,255]
        hranges[1] = 255.0;
        channels[0] = 0;    // the three channels
        channels[1] = 1;
        channels[2] = 2;
        cv::calcBackProject(&image,
                            1,            // one image
                            channels,     // vector specifying what histogram dimensions belong to what image channels
                            histogram,    // the histogram we are using
                            result,       // the resulting back projection image
                            ranges,       // the range of values, for each dimension
                            255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
                           );
        if (threshold > 0.0)
            cv::threshold(result , result , 255 * threshold , 255 , THRESH_BINARY ) ;
        return result;
    }
};
//Using hsv_histogram and closed-operation or opened-operation
//the default kernal is 5*5;
//num=0,use rgb;
//num=1,use hsc;
Mat imgProcess(const Mat &image, Mat &sample, float th, int num);
/*****************
/num=0,open-operation;
/num=1,close-operation;
/num=2,open&close-operation;
*****************/
//Mat isOpenClose(const Mat &image,int num);

void draw(const Mat &image);
