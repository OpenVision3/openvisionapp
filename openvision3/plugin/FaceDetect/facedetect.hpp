//
//  Header.h
//  openvision3
//
//  Created by HONGLIN on 2016-05-08.
//  Copyright © 2016 ZHIMAABC. All rights reserved.
//

#ifndef facedetect_h
#define facedetect_h

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp> 

using namespace cv;

void detectAndDisplay(Mat &frame, CascadeClassifier& cascade);

#endif /* Header_h */
