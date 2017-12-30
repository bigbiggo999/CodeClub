#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
    //Mat im(600,800, CV_8UC1);
    Mat im(600,800,CV_8UC3);
    namedWindow("image");
    int k,j;
    /**
     *用at方法给每个像素设置值
     
    for(int i=0;i<im.rows;i++){
        for(int j=0;j<im.cols;j++){
            im.at<uchar>(i,j)=0;
        }
    }
     */
    /**
     *用迭代器方法给每个像素设置值
     
    MatIterator_<uchar> Mbegin,Mend;
    for(Mbegin=im.begin<uchar>(),Mend=im.end<uchar>();Mbegin!=Mend;Mbegin++){
        *Mbegin=255;
    }
     */
    /**
     *用指针方法给每个像素设置值
     */
    for(int i=0;i<im.rows;i++){
        Vec3b *p = im.ptr<Vec3b>(i);
        for(int j=0;j<im.cols;j++){
            p[j][0]=0;
            p[j][1]=0;
            p[j][2]=0;
        }
    }
     
    if( im.empty()){
        cout << "Can not load image." << endl;
        return -1;
    }
    int i=100;
    Vec3b *p=im.ptr<Vec3b>(i);
    Vec3b *p2=im.ptr<Vec3b>(i+1);
    Vec3b *p3=im.ptr<Vec3b>(i+2);
    for( j=0,k=0;j<im.cols-600;j++){
        p[j][0]=0;
        p[j][1]=0;
        p[j][2]=255;
        p2[j][0]=0;
        p2[j][1]=0;
        p2[j][2]=255;
        p3[j][0]=0;
        p3[j][1]=0;
        p3[j][2]=255;
        cvWaitKey(5);
        imshow("image", im);
        if(j>20){
            while(k<j-20){
                p[k][0]=0;
                p[k][1]=0;
                p[k][2]=0;
                p2[k][0]=0;
                p2[k][1]=0;
                p2[k][2]=0;
                p3[k][0]=0;
                p3[k][1]=0;
                p3[k][2]=0;
                k++;
            }
        }
    }
    while(k<j){
        p[k][0]=0;
        p[k][1]=0;
        p[k][2]=0;
        p2[k][0]=0;
        p2[k][1]=0;
        p2[k][2]=0;
        p3[k][0]=0;
        p3[k][1]=0;
        p3[k][2]=0;
        k++;
        cvWaitKey(5);
        imshow("image", im);
    }
    k=j;
    for(;i<im.rows;i++){
        Vec3b *p=im.ptr<Vec3b>(i);
        j=k;
        for(;j<k+7;j++){
            p[j][0]=0;
            p[j][1]=0;
            p[j][2]=255;
        }
        cvWaitKey(5);
        imshow("image", im);
    }
    
    
    
    waitKey(0);
    return 0;
}
    
    
