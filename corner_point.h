#ifndef CORNER_POINT_H
#define CORNER_POINT_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <stdio.h>
#include <stdlib.h>

class Corner_point
{
public:
    Corner_point() : r_o_w(1) , wrong_track_num(0) , cluster_mark(-1) {}
    int r_o_w ;//>0记为可使用为约束 =0记为暂不可用 <0记为不可用 初试置为1
    int wrong_track_num; //标记暂不可用的次数 初始置为0
//    cv::Point3f point;
//    cv::Point3f dispalcement;

    //寻找簇中心
    cv::Point3f cluster_dpm;//用于计算簇的位移
    static double radius;//半径
    int density;//密度
    double min_distance;//到较大密度点的最小距离
    bool if_center;//是否密度中心
    int get_density(Corner_point Points[] , int size);
    double get_mdistance(Corner_point Points[] , int size);

    //DBSCAN
    int cluster_mark;//聚类的标记
    int db_density;//DBSCAN所用密度
    static double core_dis;
    int get_db_density(Corner_point Points[] , int size);



    Corner_point(cv::Point3f get_point);
//    void move(cv::Point3f fxy);
//    void get(cv::Point3f get_point);



};

void find_center(Corner_point Corner_points[], std::vector<int> &cluster_centre , int size);

bool if_inside(Corner_point center,Corner_point test_point);

int get_cluster(Corner_point Corner_points[], std::vector<int> cluster_centre, int size);

//void get_meanvec(std::vector<Corner_point> & Corner_points,int cluster_num );


#endif // CORNER_POINT_H
