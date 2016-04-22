/*
 计算输入图像的显著图；
 显著图为灰度图, 显著图中越亮的地方表示越容易吸引人眼的注意；
 算法为经典的Itti方法, 实现基于Aditya Sundararajan等的代码。
 */

#include "IttiSaliency.h"

// Generate Gaussian Pyramid of intensity image
void buildGaussianPyramid(Mat &intensity, std::vector<Mat> &GauPyr)
{
	std::vector<Mat> bgr(3);
    
    split(intensity, bgr);	//Split the input image into 3 separate channels and store it in bgr
	intensity = (bgr[0] + bgr[1] + bgr[2]) / 3;	//Take average of the 3 separated channels. This is the intensity.

	buildPyramid(intensity, GauPyr, 8);	//Construct Gaussian Pyramid of the intensity image
}

//Resize all the images of Feature Map and add them for conspicuity map
void resizeMap(std::vector<Mat> &featureMap, Mat &conspicuity) 
{
   	int i;
   	Mat dest;
    conspicuity = Mat::zeros(featureMap[5].size(), 0); //Initialize all pixels to zero

    for(i = 0; i < 6; i++)
    {
        while(featureMap[i].rows != featureMap[5].rows) //Perform pyrDown till the all the images of a Map are of same size
        {
        	    pyrDown(featureMap[i], dest);
        	    featureMap[i] = dest;
        }
	conspicuity += featureMap[i]; //Add all the images of a feature map and store it in conspicuity map.
    }
}

//Generate Feature Maps
void buildMap(std::vector<Mat> &pyramid, Mat &conspicuity)
{
    int i, c, del, j;
    Mat src, dest;
    std::vector<Mat> featureMap(6);

    i = 0;

	// Perform centre-surround of Gaussian Pyramid
	// surround = 2 to 4; centre = c + del = 5 to 8
    for(c = 2; c <= 4; c++)
	{
		for(del = 3; del <= 4; del++)
		{
			src = pyramid[c + del];
		    for(j = 1; j <= del; j++)
            {
                pyrUp(src, dest);
				src = dest;
            }
            featureMap[i] = abs(pyramid[c] - src(Range(0, pyramid[c].rows), Range(0, pyramid[c].cols)));
            i++;
		}
	}

	resizeMap(featureMap, conspicuity);
}

//Generate Red, Green, Blue, and Yellow maps required for Color Map
void buildRGBY(Mat &intensity, Mat &imageCopy, std::vector<Mat> &R_GauPyr, std::vector<Mat> &G_GauPyr, std::vector<Mat> &B_GauPyr, std::vector<Mat> &Y_GauPyr)
{
	double maxVal;
	int i, j;
	Vec3b zero(0, 0, 0);
	std::vector<Mat> channel(3);

	//Normalize the input image	
	minMaxLoc(intensity, NULL, &maxVal, NULL, NULL);
	for(i = 0; i < imageCopy.rows; i++)
	{
		for(j = 0; j < imageCopy.cols; j++)
		{
			if(intensity.at<uchar>(i,j) > (int)(maxVal/10))
			{
				imageCopy.at<Vec3b>(i, j)[0] = saturate_cast<uchar>( (255 *imageCopy.at<Vec3b>(i, j)[0])/maxVal );
                imageCopy.at<Vec3b>(i, j)[1] = saturate_cast<uchar>( (255 *imageCopy.at<Vec3b>(i, j)[1])/maxVal );
                imageCopy.at<Vec3b>(i, j)[2] = saturate_cast<uchar>( (255 *imageCopy.at<Vec3b>(i, j)[2])/maxVal );
			}

			else
			{
				imageCopy.at<Vec3b>(i, j) = zero;
			}
		}
	}

	split(imageCopy, channel);

    Mat R = channel[2] - ((channel[1]  + channel[0]) / 2) ; //Generate RED channel
	threshold(R, R, 0, 255, THRESH_TOZERO);

	Mat G = channel[1] - ((channel[0] + channel[2]) / 2); //Generate GREEN channel
	threshold(G, G, 0, 255, THRESH_TOZERO);

	Mat B = channel[0] - ((channel[0] + channel[2]) / 2); //Generate BLUE channel
	threshold(B, B, 0, 255, THRESH_TOZERO);

	Mat Y = (((channel[2] + channel[1]) / 2) - abs(channel[2] - channel[1]) / 2 - channel[0]); //Generate YELLOW channel
	threshold(Y, Y, 0, 255, THRESH_TOZERO);

	buildPyramid(R, R_GauPyr, 8);
	buildPyramid(G, G_GauPyr, 8);
	buildPyramid(B, B_GauPyr, 8);
	buildPyramid(Y, Y_GauPyr, 8);
}


//Generate Intensity Map
void buildIntensityMap(std::vector<Mat> &GauPyr, Mat &I_bar)
{
    buildMap(GauPyr, I_bar);
}

//Generate Color Map
void buildColorMap(Mat &intensity, Mat &imageCopy, Mat &C_bar)
{
    std::vector<Mat> R_GauPyr(9);
	std::vector<Mat> G_GauPyr(9);
	std::vector<Mat> B_GauPyr(9);
	std::vector<Mat> Y_GauPyr(9);

    std::vector<Mat> RG(6);
	std::vector<Mat> BY(6);
	std::vector<Mat> colorMap(6);

	Mat temp1, temp2, src, dest;

	int i, c, del, j;

    buildRGBY(intensity, imageCopy, R_GauPyr, G_GauPyr, B_GauPyr, Y_GauPyr);


	// Perform centre-surround of Gaussian Pyramid;
	// surround = 2 to 4; centre = c + del = 5 to 8
    i = 0;
	for(c = 2; c <= 4; c++)
	{
		for(del = 3; del <= 4; del++)
		{
			temp1 = R_GauPyr[c + del] - G_GauPyr[c + del];
			temp2 = Y_GauPyr[c + del] - B_GauPyr[c + del];

			for(j = 1; j <= del; j++)
            {
                pyrUp(temp1, dest);
                temp1 = dest;

		        pyrUp(temp2, dest);
		        temp2 = dest;
            }

		RG[i] = abs((R_GauPyr[c] - G_GauPyr[c]) - (temp1(Range(0, R_GauPyr[c].rows), Range(0, R_GauPyr[c].cols))));
		BY[i] = abs(temp1(Range(0, B_GauPyr[c].rows), Range(0, B_GauPyr[c].cols)) - (B_GauPyr[c] - Y_GauPyr[c]));

		colorMap[i] = RG[i] + BY[i];
		i++;
		}
	}

	resizeMap(colorMap, C_bar);
}



//Generate Orientation Map
void buildOrientationMap(Mat &intensity, Mat &O_bar)
{
    std::vector<Mat> kernel(4);
	std::vector<Mat> gaborImage(4);

	std::vector<Mat> GauPyr_0(9);
	std::vector<Mat> GauPyr_45(9);
	std::vector<Mat> GauPyr_90(9);
	std::vector<Mat> GauPyr_135(9);

	Mat totalOriMap_0, totalOriMap_45, totalOriMap_90, totalOriMap_135;
    int theta,  j = 0;

	//Calculate Gabor Filter for theta = 0, 45, 90, 135	
	for(theta = 0; theta < 4; theta++)
	{
		kernel[theta] = getGaborKernel(Size (7, 7), 3, theta * (CV_PI/4), 1.0, 0, 0);
		filter2D(intensity, gaborImage[theta], -1, kernel[theta]);
	}

	buildPyramid(gaborImage[0], GauPyr_0, 8);
	buildPyramid(gaborImage[1], GauPyr_45, 8);
	buildPyramid(gaborImage[2], GauPyr_90, 8);
	buildPyramid(gaborImage[3], GauPyr_135, 8);

	buildMap(GauPyr_0, totalOriMap_0);
	buildMap(GauPyr_45, totalOriMap_45);
	buildMap(GauPyr_90, totalOriMap_90);
	buildMap(GauPyr_135, totalOriMap_135);

	//Add each conspicuity map for theta = 0, 45, 90, 135
	//This will give conspicuity map for orientation	
	O_bar = totalOriMap_0 + totalOriMap_45 + totalOriMap_90 + totalOriMap_135;
}

//Build Saliency Map
void buildSaliencyMap(Mat &I_bar,  Mat &C_bar,  Mat &O_bar,  Mat &SalicencyMap)
{
	    
    Mat dest;

	Mat SalMap = (I_bar + C_bar + O_bar) / 3; //Saliency Map is average of the 3 conspicuity maps
   	
	resize(SalMap, dest, SalicencyMap.size(), 0, 0, INTER_LINEAR); //resize saliency map to size of input image	
	SalicencyMap = dest;	
	
}

void SaliencyDetection(Mat &image, Mat &SaliencyMap)//image is input, SaliencyMap is output
{
	std::vector<Mat> GauPyr(9);
	Mat I_bar, C_bar, O_bar;
	Mat imageCopy = image;
	SaliencyMap = image;	   			
	
    buildGaussianPyramid(image, GauPyr);	//Generate Gaussian Pyramid 

    buildIntensityMap(GauPyr, I_bar);		//Generate Intensity Map
    buildColorMap(image, imageCopy, C_bar);	//Generate Color Map
	buildOrientationMap(image, O_bar);		//Generate Orientation Map
 
    buildSaliencyMap(I_bar, C_bar, O_bar, SaliencyMap);	//Generate Saliency Map
}