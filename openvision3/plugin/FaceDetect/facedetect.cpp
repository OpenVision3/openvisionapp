//Face detector that is referenced from OpenCV
//Xin Chen
//Date: May 7, 2016

#include "facedetect.hpp"

//CascadeClassifier loadCascade(const char *facecascadename)
//{
//    const char *face_cascade_name = "haarcascade_frontalface_alt.xml";
//    CascadeClassifier face_cascade;
//    if (!face_cascade.load(face_cascade_name))
//    {
//        printf("Error loading haarcascade face\n");
//        return face_cascade;
//    }
//    return face_cascade;
//}

// Function Headers
void detectAndDisplay(Mat &frame, CascadeClassifier& cascade)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    Rect roi_b;
    //Rect roi_c;
    size_t faceNum = faces.size();
    for (size_t i = 0; i < faceNum; i++)
    {
        Rect roi_c = faces[i];
        Point pt1(roi_c.x, roi_c.y);
        Point pt2(roi_c.x + roi_c.height, roi_c.y+roi_c.width);
        rectangle(frame, pt1, pt2, Scalar(0,255,0), 2, 8, 0);
    }
}
