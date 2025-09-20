#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;





int findhsv(){
    int hmin = 62,smin=0,vmin=255;
    int hmax =105,smax=59,vmax=255;

    String img_url = "resources/test_image.png";
    Mat img =imread(img_url);
    if(img.empty()){
        cout << "img_url,ERROR,找不到 帅气的l管,注意图片url"<<endl;
        return -1;
    }
    Mat imgGauss,imgHSV,imgMask;
    GaussianBlur(img, imgGauss, Size(13,13), 7);
    cvtColor(imgGauss, imgHSV, COLOR_BGR2HSV);


    //这种方式在opencv中会有警告，不过懒得管了，先用着
    namedWindow("Trackbars", 400);
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);

    while(1){
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax,smax,vmax);
        inRange(imgHSV, lower, upper, imgMask);
        imshow("img",img);
        imshow("imgHSV",imgHSV);
        imshow("imgMask",imgMask);
        char key = waitKey(1);
        if(key==27){
            imwrite("imgfind/mask.png",imgMask);
            cout<<"upper:  "<<upper<<endl;
            cout<<"lower:  "<<lower<<endl;
            break;
        }
    }
    

    return 0 ;
}