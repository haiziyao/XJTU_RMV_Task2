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
int learn(){
    String img_url = "resources/test_image.png";
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
    
    
    Mat img_dila,img_erode,img_morph,img_equa;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(img_bin,img_dila,kernel);
    erode(img_bin,img_erode,kernel);
    morphologyEx(img_bin, img_morph, MORPH_OPEN, kernel);//开运算，先腐蚀后膨胀
    equalizeHist(img_gray,img_equa);



    namedWindow("img",WINDOW_FREERATIO);
    imshow("0. 原始图像", img);
    imshow("1. 灰度图", img_gray);
    imshow("2. 均值模糊", img_blur);
    imshow("3. 高斯模糊", img_gaussblur);
    imshow("4. 中值模糊", img_median);
    imshow("5. 固定阈值二值化", img_bin);
    imshow("6. 自适应阈值二值化", img_adtbin);
    imshow("7. Canny边缘检测", img_canny);
    imshow("8. HSV色彩空间", img_hsv);
    imshow("9. 颜色掩码（红色）", img_mask);
    imshow("10. 膨胀操作", img_dila);
    imshow("11. 腐蚀操作", img_erode);
    imshow("12. 开运算", img_morph);

    waitKey(0);
    return 0;
}
