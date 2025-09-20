#include <cstddef>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;


Mat rotateImage(const Mat& src, double angle);
Mat cropTopLeftQuarter(const Mat& src);
int work(){
    //int hmin = 63,smin=207,vmin=175;
    //int hmax =112,smax=255,vmax=255;
    int hmin = 62,smin=0,vmin=255;
    int hmax =105,smax=59,vmax=255;
    Scalar lower(hmin,smin,vmin);
    Scalar upper(hmax,smax,vmax);

    String img_url = "resources/test_image.png";
    Mat img = imread(img_url);
    if(img.empty()){
        cout<<"img not fount,你把帅气的l管藏哪里了"<<endl;
        return -1;
    }
    Mat img_gray,img_hsv;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    cvtColor(img, img_hsv, COLOR_BGR2HSV);
    imwrite("img/gray.png", img_gray);
    imwrite("img/hsv.png",img_hsv);
    Mat img_blur,img_gaussblur;
    blur(img, img_blur, Size(3,3));
    GaussianBlur(img, img_gaussblur, Size(13,13), 7);
    imwrite("img/blur.png", img_blur);
    imwrite("img/gaussblur.png", img_gaussblur);
    Mat img_mask,img_morph_close,img_morph_open,img_morph_gauss,img_morph_erode,img_morph_dilate,img_deal;
    inRange(img_hsv, lower, upper, img_mask);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(11,11));
    morphologyEx(img_mask, img_morph_close, MORPH_CLOSE, kernel);
    morphologyEx(img_mask, img_morph_open, MORPH_OPEN, kernel);
    imwrite("img/mask.png", img_mask);
    imwrite("img/morph_close.png", img_morph_close);
    imwrite("img/morph_open.png", img_morph_open);
    GaussianBlur(img_morph_close, img_morph_gauss, Size(5,5), 2,2);
    imwrite("img/morph_gauss.png",img_morph_gauss);
    Mat erodeKernel = getStructuringElement(MORPH_RECT, Size(5,5));
    erode(img_morph_gauss,img_morph_erode, erodeKernel);
    imwrite("img/morph_erode.png", img_morph_erode);
    dilate(img_morph_erode, img_morph_dilate, erodeKernel);
    imwrite("img/morph_dilate.png", img_morph_dilate);
    GaussianBlur(img_morph_dilate, img_deal, Size(5,5), 2,2);
    imwrite("img/morph_dilate_gauss.png", img_deal);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_deal,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    Mat contourImg = Mat::zeros(img.size(),CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(img, contours, (int)i, Scalar(0, 255, 0), 2);
    }
    imwrite("img/img_contours.png", img);
    
    for(size_t i = 0;i<contours.size();i++){
        double area = contourArea(contours[i]);
        if (area<500) continue;
        Rect light = boundingRect(contours[i]);
        rectangle(img_deal,light,Scalar(255,0,0),5);
    }
    imshow("img", img_deal);
    
    Mat img_rotate = rotateImage(img, 35.0);
    Mat img_crop = cropTopLeftQuarter(img);
    imwrite("img/rotate.png", img_rotate);
    imwrite("img/crop.png", img_crop);
    while(1){
        char key = waitKey(1);
        if(key==27){
            break;
        }
    }


    return 0;
}

Mat rotateImage(const Mat& src, double angle) {
    
    Point2f center((src.cols - 1) / 2.0f, (src.rows - 1) / 2.0f);
    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);  
    Rect2f bbox = RotatedRect(center, src.size(), angle).boundingRect2f();
    rotMat.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rotMat.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    Mat dst;
    warpAffine(src, dst, rotMat, bbox.size());
    return dst;
}
Mat cropTopLeftQuarter(const Mat& src) {
    int cropWidth = src.cols / 2;
    int cropHeight = src.rows / 2;
    Rect cropRect(0, 0, cropWidth, cropHeight);  // 从(0,0)开始裁剪
    return src(cropRect).clone();  // clone()确保返回独立的图像副本
}