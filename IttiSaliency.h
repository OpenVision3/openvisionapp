/*
计算输入图像的显著图；
显著图为灰度图, 显著图中越亮的地方表示越容易吸引人眼的注意； 
算法为经典的Itti方法, 实现基于Aditya Sundararajan等的代码。
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>  


using namespace cv;

void buildGaussianPyramid(Mat &intensity, vector<Mat> &GauPyr);

void resizeMap(vector<Mat> &featureMap, Mat &conspicuity);

void buildMap(vector<Mat> &pyramid, Mat &conspicuity);

void buildRGBY(Mat &intensity, Mat &imageCopy, vector<Mat> &R_GauPyr, vector<Mat> &G_GauPyr, vector<Mat> &B_GauPyr, vector<Mat> &Y_GauPyr);

void buildIntensityMap(vector<Mat> &GauPyr, Mat &I_bar);

void buildColorMap(Mat &intensity, Mat &imageCopy, Mat &C_bar);

void buildOrientationMap(Mat &intensity, Mat &O_bar);

void buildSaliencyMap(Mat &I_bar,  Mat &C_bar,  Mat &O_bar,  Mat &SalicencyMap);

//在应用中调用此函数
void SaliencyDetection(Mat &image, Mat &SaliencyMap);//image is input,SaliencyMap is output.