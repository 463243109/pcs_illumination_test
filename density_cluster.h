#ifndef DENSITY_CLUSTER
#define DENSITY_CLUSTER
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>

#define RADIUS 2


class DENSITY_COORDINATE{

public:
    static int fore_density;
    int density ;
    int num ;
    std::vector<cv::Point> coordinate;
    DENSITY_COORDINATE():density(0) , num(0) {}

    void add_d(){
        int d = density;
        if(d>2073600)std::cout<<d<<std::endl;
        ++density;
    }

    void add_c(const cv::Point & xy){
        coordinate.push_back(xy);
        ++density;
        ++num;
    }

    bool if_fore(){
        if (density <= fore_density) return true;
        return false;
    }

};

void get_cluster_map(cv::Mat substract /*, DENSITY_COORDINATE **two_dim*/);

//void get_hist(cv::Mat & hist);

int get_hist(cv::Mat & hist);

void get_first_fore(cv::Mat &frame , int mark);
void get_first_fore_dark(cv::Mat & frame ,cv::Mat frame1 , int mark);

#endif // DENSITY_CLUSTER

