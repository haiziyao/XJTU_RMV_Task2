#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main(int, char**){
    String img_url = "../resources/test_iamge.png";
    Mat img =imread(img_url);
    if(img.empty()){
        cout << "img_url：ERROR，找不到图片"<<endl;
        return -1;
    }
    Mat img_gray,img_blur,img_gaussblur,img_median,img_bin,img_adtbin,img_canny,img_hsv,img_mask;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    blur(img, img_blur, Size(5,5));
    GaussianBlur(img, img_gaussblur, Size(5,5), 1.5);
    medianBlur(img, img_median, 5);
    threshold(img_gray, img_bin, 128, 255, THRESH_BINARY);
    adaptiveThreshold(img_gray,img_adtbin,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);
    Canny(img_gray,img_canny,100,200);
    cvtColor(img, img_hsv, COLOR_BGR2HSV);
    inRange(img_hsv, Scalar(0,100,100), Scalar(10,255,255), img_mask);

    namedWindow("img",WINDOW_FREERATIO);
    imshow("img",img);
    waitKey(0);
    return 0;
}
