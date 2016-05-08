//Face detector that is referenced from OpenCV
//Xin Chen
//Date: May 7, 2016

#include "facedetect.hpp"

int main (int argc, char* argv[])
{
        const string face_cascade_name = "haarcascade_frontalface_alt.xml";
        CascadeClassifier face_cascade;
        if (!face_cascade.load(face_cascade_name))
        {
                printf("Error loading haarcascade face\n");
                return -1;
        }
        string inputImgName;
        Mat frame;
        if (argc == 1)
                inputImgName = "group1.jpg";
        else if (argc == 2)
                inputImgName = argv[1];
        else
                printf("please type: facedetector imagename\n\n");

        //Read an image
        frame = imread(inputImgName);
        if (!frame.empty())
                detectAndDisplay(frame, face_cascade);
        namedWindow("Display", WINDOW_AUTOSIZE);
        imshow("Display", frame);
        waitKey();

        return 0;
}
