#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>  


using namespace cv;

void buildGaussianPyramid(Mat &intensity, std::vector<Mat> &GauPyr);

void resizeMap(std::vector<Mat> &featureMap, Mat &conspicuity);

void buildMap(std::vector<Mat> &pyramid, Mat &conspicuity);

void buildRGBY(Mat &intensity, Mat &imageCopy, std::vector<Mat> &R_GauPyr, std::vector<Mat> &G_GauPyr, std::vector<Mat> &B_GauPyr, std::vector<Mat> &Y_GauPyr);

void buildIntensityMap(std::vector<Mat> &GauPyr, Mat &I_bar);

void buildColorMap(Mat &intensity, Mat &imageCopy, Mat &C_bar);

void buildOrientationMap(Mat &intensity, Mat &O_bar);

void buildSaliencyMap(Mat &I_bar,  Mat &C_bar,  Mat &O_bar,  Mat &SalicencyMap);

//在应用中调用此函数
void SaliencyDetection(Mat &image, Mat &SaliencyMap);//image is input,SaliencyMap is output.