#include "HSVfind.h"
#include <cstddef>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <tesseract/baseapi.h>
using namespace cv;
using namespace std;

char findnum(Mat imgnum);

int HSVfindcontour(){
    int hmin = 62,smin=0,vmin=255;
    int hmax =105,smax=59,vmax=255;
    Scalar lower(hmin,smin,vmin);
    Scalar upper(hmax,smax,vmax);

    String img_url = "resources/test_image.png";
    Mat img = imread(img_url);
    Mat img_origi = imread(img_url);
    if(img.empty()){
        cout<<"img not fount,你把帅气的l管藏哪里了"<<endl;
        return -1;
    }
    Mat img_gray,img_hsv,img_mask,img_morph;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    imwrite("imgwork/gray.png", img_gray);
    Mat img_blur,img_gaussblur;
    blur(img, img_blur, Size(3,3));
    GaussianBlur(img, img_gaussblur, Size(13,13), 7);
    imwrite("imgwork/blur.png", img_blur);
    imwrite("imgwork/gaussblur.png", img_gaussblur);
    cvtColor(img_gaussblur, img_hsv, COLOR_BGR2HSV);
    inRange(img_hsv, lower, upper, img_mask);
    imwrite("imgwork/mask.png", img_mask);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(7,7));
    morphologyEx(img_mask, img_morph, MORPH_CLOSE, kernel);
    imwrite("imgwork/morph.png", img_morph);
    

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Rect> rects;
    findContours(img_morph,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    Mat contourImg = Mat::zeros(img.size(),CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(img, contours, (int)i, Scalar(255, 255, 0), 4);
    }
    imwrite("imgwork/contours.png", img);
    
    for(size_t i = 0;i<contours.size();i++){
        double area = contourArea(contours[i]);
        if (area<300) continue;
        Rect light = boundingRect(contours[i]);
        rectangle(img,light,Scalar(0,0,255),5);
        rects.push_back(light);
    }
    imwrite("imgwork/rect.png", img);

    Rect desrec;
    if (rects.size() == 2) {
        Rect rect1 = rects[0];
        Rect rect2 = rects[1];
        Point center1(rect1.x + rect1.width / 2, rect1.y + rect1.height / 2);
        Point center2(rect2.x + rect2.width / 2, rect2.y + rect2.height / 2);
        Point newCenter((center1.x + center2.x) / 2, (center1.y + center2.y) / 2);
        
        int longSide1 = max(rect1.width, rect1.height);
        int longSide2 = max(rect2.width, rect2.height);
        int newSide = 2 * max(longSide1, longSide2);
        
        int newX = newCenter.x - newSide / 2;
        int newY = newCenter.y - newSide / 2;
        
        Rect newRect(newX, newY, newSide, newSide);
        desrec = newRect;
        rectangle(img, newRect, Scalar(0, 255, 0), 3);
    }

     
    char num = findnum(img_origi(desrec));
    cout<<"识别出来的数字是："<<num<<endl;
    String text(1,num);
    putText(img, text, Point(desrec.x,desrec.y), FONT_HERSHEY_DUPLEX, 2, Scalar(255,255,0),2);
    
    imshow("img", img);
    imwrite("imgwork/num.png", img);
    while(1){
        char key = waitKey(1);
        if(key==27){
            break;
        }
    }
    return 0;
}

// Tesseract OCR库 轻量级识别数字
char findnum(Mat imgnum){
    if(imgnum.empty()){
        cout<<"找不到我们帅气的l管哥哥了，img错误"<<endl;
        return -1;
    }
    Mat img_gray,img_bin;
    cvtColor(imgnum,img_gray, COLOR_BGR2GRAY);
    threshold(img_gray, img_bin,0,255, THRESH_BINARY_INV | THRESH_OTSU);//反向二值化
    imshow("imgnum",img_bin);
    tesseract::TessBaseAPI tess;
    // 仅识别数字（设置白名单：0-9）
    tess.SetVariable("tessedit_char_whitelist", "0123456789");
    // 初始化OCR引擎（参数为训练数据路径，""表示使用默认路径）
    if (tess.Init("", "osd")) {
        cerr << "无法初始化tesseract OCR引擎！" << endl;
        return -1;
    }
    tess.SetImage(img_bin.data, img_bin.cols, img_bin.rows, 1, img_bin.cols);
    char* outText = tess.GetUTF8Text();
    string text(outText);
    delete[] outText;
    ///int result =  text[0]-'0';
    char result = text[0];
    return result;
}