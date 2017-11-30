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
#include <density_cluster.h>
#include <corner_point.h>
#include <Windows.h>
using namespace std;

//Corner_point corner_points[82944];

//cv::Vec3f get_var(cv::Mat subtract_value  ,cv::Point p) {
//    assert(subtract_value.type() == CV_32FC3);
//    int mark= 0;
//    int all_subs[3][100] ;
//    for(int x = -2 ; x <=2 ; x++) {
//        for(int y = -2 ; y <=2 ; y++) {
//            int i = x + p.x , j = p.y+y;
//            if(i < 0 || j < 0 || i >= subtract_value.cols || j >= subtract_value.rows) continue;
//            if(subtract_value.at<cv::Vec3f>(j , i)[0] < -8 || subtract_value.at<cv::Vec3f>(j , i)[0] > 20
//                    ||subtract_value.at<cv::Vec3f>(j , i)[1] <-9 || subtract_value.at<cv::Vec3f>(j , i)[1] > 10
//                    ||(subtract_value.at<cv::Vec3f>(j , i)[2] >2.5&&subtract_value.at<cv::Vec3f>(j , i)[2]<4 ) ||subtract_value.at<cv::Vec3f>(j , i)[2] < 0 ||  subtract_value.at<cv::Vec3f>(j , i)[2] > 26.5) {
//                for(int k = 0 ; k < 3 ; k++) {
//                    all_subs[k][mark] = subtract_value.at<cv::Vec3f>(j , i)[k];
//                }
//                mark++;
//            }

//        }
//    }

//}

int get_max_diff(cv::Vec3b all_p[25] , int mark) {
    int max_diff = 0;
    for(int i = 0 ; i < mark ; i++) {
        for(int j = i+1 ; j < mark ; j++) {
            int temp = 0;
            for(int k = 0 ;k < 3 ; k++) temp += (all_p[i][k] - all_p[j][k])*(all_p[i][k] - all_p[j][k]);
            max_diff = max(max_diff , temp);
//            cout<<max_diff<<endl;
        }
    }
    return max_diff;
}


bool if_biger_differ(cv::Mat subtract_value ,cv::Point p , cv::Mat back , cv::Mat fore) {
    assert(subtract_value.type() == CV_32FC3);
    int mark= 0;
    cv::Vec3b all_p[2][25];
    for(int x = -2 ; x <=2 ; x++) {
        for(int y = -2 ; y <=2 ; y++) {
            int i = x + p.x , j = p.y+y;
            if(i < 0 || j < 0 || i >= subtract_value.cols || j >= subtract_value.rows) continue;
            all_p[0][mark] = back.at<cv::Vec3b>(j,i);
            all_p[1][mark] = fore.at<cv::Vec3b>(j,i);
            mark++;
        }
    }
//    cout<<get_max_diff(all_p[0] , mark)<<" "<<get_max_diff(all_p[1] , mark)<<endl;
    return (get_max_diff(all_p[0] , mark) < get_max_diff(all_p[1],mark));

}

void lct_detection(cv::Mat & lct_mark ) {
    for(int i = 0 ; i < lct_mark.rows ; i++) {
        for(int j = 0 ; j < lct_mark.cols ; j++) {
            if(255 != lct_mark.at<uchar>(i,j) && 0 != lct_mark.at<uchar>(i,j)) {
                int vote = 0 ;
                for(int y = 0 ; (y + i<lct_mark.rows) /*&& (y<250)*/ ; y++) {
                    if(255 == lct_mark.at<uchar>(y+i,j)) {
                        vote++;
                        break;
                    }
                }
                for(int y = 0 ; y + i >=0 /*&& y>-250*/; y--) {
                    if(255 == lct_mark.at<uchar>(y+i,j)) {
                        vote++;
                        break;
                    }
                }
                for(int x = 0 ; x + j <lct_mark.cols /*&& x< 250*/ ; x++) {
                    if(255 == lct_mark.at<uchar>(i,j+x)) {
                        vote++;
                        break;
                    }
                }
                for(int x = 0 ; x + i >=0 /*&& x>-250*/; x--) {
                    if(255 == lct_mark.at<uchar>(i,i+x)) {
                        vote++;
                        break;
                    }
                }
                if(vote > 2) lct_mark.at<uchar>(i,j) = 255;
                else lct_mark.at<uchar>(i,j) = 0;
            }
        }

    }

}

void cvtColorPCS(const cv::Mat& RGB, cv::Mat& pcs)
{
    pcs = cv::Mat(RGB.size(), CV_32FC3);
    //pcs = Mat(RGB.size(), CV_8UFC1);
    cv::Mat A = (cv::Mat_<float>(3, 3) <<	27.07439,  -22.80783,  -1.806681,
                                    -5.646736,  -7.722125, 12.86503,
                                    -4.163133,  -4.579428, -4.576049);
    cv::Mat B = (cv::Mat_<float>(3, 3) <<	0.9465229,  0.2946927, -0.1313419,
                                    -0.1179179, 0.9929960, 0.007371554,
                                    0.09230461, -0.04645794, 0.9946464);

    cv::Mat pTem1, pTem2;
    for (int i = 0; i < RGB.rows; ++i)
    {
        for (int j = 0; j < RGB.cols; ++j)
        {
            cv::Mat temp1 = (cv::Mat_<float>(3, 1) <<	(float)(RGB.at<uchar>(i, j*3)),
                                                (float)(RGB.at<uchar>(i, j*3 + 1)),
                                                (float)(RGB.at<uchar>(i, j*3 + 2)));
            cv::Mat temp2 = B * temp1;

            for (int pos = 0; pos < 3; ++pos)
                temp2.at<float>(pos, 0) = temp2.at<float>(pos, 0) > 0 ? temp2.at<float>(pos, 0) : -temp2.at<float>(pos, 0);


            temp1.at<float>(0, 0) = std::log(temp2.at<float>(0, 0));
            temp1.at<float>(1, 0) = std::log(temp2.at<float>(1, 0));
            temp1.at<float>(2, 0) = std::log(temp2.at<float>(2, 0));

            temp2 = A * temp1;
            pcs.at<float>(i, j*3) = abs(temp2.at<float>(0, 0));
            pcs.at<float>(i, j*3 + 1) = abs(temp2.at<float>(1, 0));
            pcs.at<float>(i, j*3 + 2) = abs(temp2.at<float>(2, 0));

            //pcs.at<uchar>(i, j) = int(abs(temp2.at<float>(2, 0)) * 255.0 / (temp2.at<float>(0, 0) + temp2.at<float>(1, 0) + temp2.at<float>(2, 0)));
        }
    }
}

cv::Mat show_dividecolor_cluster(cv::Mat BGR , int x, int y){
    cv::Mat show(256,256,CV_8UC1,cv::Scalar(255));
    if(x >2 || x< 0 || y <0 ||y >2) return show;
    for(int i = 0 ; i < BGR.rows ; i++){
        for(int j = 0 ; j <BGR.cols ; j++){
//            std::cout<< i <<" "<<j<<" "<<show.at<uchar>(BGR.at<cv::Vec3b>(i,j)[x],BGR.at<cv::Vec3b>(i,j)[y])<<std::endl;
            if(show.at<uchar>(BGR.at<cv::Vec3b>(i,j)[x],BGR.at<cv::Vec3b>(i,j)[y]) > 10)
                show.at<uchar>(BGR.at<cv::Vec3b>(i,j)[x],BGR.at<cv::Vec3b>(i,j)[y] ) -= 10;
        }
    }
    cv::imshow("show", show);
    cv::waitKey(0);
    return show;
}


int main()
{
    cv::Mat frame1 , frame2;
    cv::Mat pcs1,pcs2;
//    frame1 = cv::imread("F:/lab_fixed_illumination_changes/2855.bmp");
//    frame2 = cv::imread("F:/lab_fixed_illumination_changes/1801.bmp");
    frame1 = cv::imread("1___.bmp");
    frame2 = cv::imread("2___.bmp");


    DWORD start_time = GetTickCount();
    cvtColorPCS(frame1,pcs1);
    cvtColorPCS(frame2,pcs2);
    DWORD end_time = GetTickCount();
    std::cout << "The pcs run time is:" << (end_time - start_time) << "ms!" << std::endl;



    start_time = GetTickCount();

    cv::Mat subtract_value(frame1.rows,frame1.cols,CV_32FC3 , cv::Scalar(0,0,0));


    //corner point
//    Corner_point corner_points[82944];
//    cv::Mat map_show(1000,1000,CV_8UC1,cv::Scalar(255));

    //single match(p c s)every channel
    cv::Mat single_match[3];
    for(int i = 0 ; i < 3 ; i++) {
        single_match[i].create(800,800,CV_8UC1);
        single_match[i] = cv::Scalar::all(255);//清零
        for(int j = 0 ; j < 40 ; j++) {
            cv::line(single_match[i],cv::Point(single_match[i].cols/2 + 10*j,j),cv::Point(single_match[i].cols/2+10*j,single_match[i].rows-1) , cv::Scalar(0) );
            cv::line(single_match[i],cv::Point(single_match[i].cols/2 - 10*j,j),cv::Point(single_match[i].cols/2-10*j,single_match[i].rows-1) , cv::Scalar(0) );
        }
    }
    int pcs_mark[3][800] = {0};

    for(int i = 0 ; i < frame1.cols ; i++){
        for(int j = 0 ; j < frame1.rows ; j++){
            for(int k = 0 ; k < 3 ; k++){
                float temp = pcs1.at<cv::Vec3f>(j,i)[k]- pcs2.at<cv::Vec3f>(j,i)[k];
                if(temp < -40) continue;
                if(temp >= 40) continue;
                subtract_value.at<cv::Vec3f>(j , i)[k] = temp ;

                //single match hist
                int hist_x = 10*(temp+40);
                if(pcs_mark[k][hist_x] < 800) single_match[k].at<uchar>(800-pcs_mark[k][hist_x]++,hist_x) = 150;

            }
//            //corner_point
//            corner_points[j*frame1.cols + i].cluster_dpm.x =subtract_value.at<cv::Vec3f>(j , i)[0] * 10;
//            corner_points[j*frame1.cols + i].cluster_dpm.y =subtract_value.at<cv::Vec3f>(j , i)[1] * 10;
//            corner_points[j*frame1.cols + i].cluster_dpm.z =subtract_value.at<cv::Vec3f>(j , i)[2] * 10;

//            cout<<"x y z : "<<corner_points[j*frame1.cols + i].point.x<<" "<<corner_points[j*frame1.cols + i].point.y<<" "<<corner_points[j*frame1.cols + i].point.z<<endl;
//            if(map_show.at<uchar>(corner_points[j*frame1.cols + i].point.y + 500 , corner_points[j*frame1.cols + i].point.z + 500) != 5){
//                map_show.at<uchar>(corner_points[j*frame1.cols + i].point.y + 500 , corner_points[j*frame1.cols + i].point.z + 500)-=10;
//            }
        }
    }
    cv::imshow("p",single_match[0]);
    cv::imshow("c",single_match[1]);
    cv::imshow("s",single_match[2]);
    cv::imwrite("verify_s_1___.bmp",single_match[2]);
    cv::waitKey();

    //just for verify
    for(int i = 0 ; i < frame2.cols ; i++) {
        for(int j = 0 ; j < frame2.rows ; j++) {
            if(subtract_value.at<cv::Vec3f>(j,i)[2]<17) frame2.at<cv::Vec3b>(j,i)[1] = (frame2.at<cv::Vec3b>(j,i)[1] + 255)/2;
//            else if(subtract_value.at<cv::Vec3f>(j,i)[2]>=8 && subtract_value.at<cv::Vec3f>(j,i)[2]<13) frame2.at<cv::Vec3b>(j,i)[0] = (frame2.at<cv::Vec3b>(j,i)[0] + 255)/2;
            else frame2.at<cv::Vec3b>(j,i)[2] = (frame2.at<cv::Vec3b>(j,i)[2] + 255)/2;
        }
    }
    cv::imshow("verify",frame2);
    cv::imwrite("verify1___.bmp",frame2);
    cv::waitKey();


//    //three channel matching
//    for(int i = 0 ; i < frame2.cols ; i++) {
//        for(int j = 0 ; j < frame2.rows ; j++) {
////            if(subtract_value.at<cv::Vec3f>(j , i)[0] < -11 || subtract_value.at<cv::Vec3f>(j , i)[0] > 11
////                    ||subtract_value.at<cv::Vec3f>(j , i)[1] <-7 || subtract_value.at<cv::Vec3f>(j , i)[1] > 7
////                    /*||(subtract_value.at<cv::Vec3f>(j , i)[2] >2.5&&subtract_value.at<cv::Vec3f>(j , i)[2]<4 )*/ ||subtract_value.at<cv::Vec3f>(j , i)[2] < -12 ||  subtract_value.at<cv::Vec3f>(j , i)[2] > 14)
////                    frame2.at<cv::Vec3b>(j,i) = cv::Vec3b(255,255,255);
//            if(subtract_value.at<cv::Vec3f>(j , i)[0] < -15 || subtract_value.at<cv::Vec3f>(j , i)[0] >20)frame2.at<cv::Vec3b>(j,i)[0] = 255;
//            if(subtract_value.at<cv::Vec3f>(j , i)[1] <-18 || subtract_value.at<cv::Vec3f>(j , i)[1] > 5)frame2.at<cv::Vec3b>(j,i)[1] = 255;
//            if(/*(subtract_value.at<cv::Vec3f>(j , i)[2] >2.5&&subtract_value.at<cv::Vec3f>(j , i)[2]<4 ) ||*/subtract_value.at<cv::Vec3f>(j , i)[2] < 1 ||  subtract_value.at<cv::Vec3f>(j , i)[2] > 30)frame2.at<cv::Vec3b>(j,i)[2] = 255;

//        }
//    }
//    cv::imshow("s",single_match[2]);
//    cv::imwrite("s.bmp",single_match[2]);
//    cv::imshow("p",single_match[0]);
//    cv::imwrite("p.bmp",single_match[0]);
//    cv::imshow("c",single_match[1]);
//    cv::imwrite("c.bmp",single_match[1]);
//    cv::imwrite("result---.bmp",frame2);
//    cv::imshow("fore",frame2);
//    cv::waitKey();



//    cv::Mat single_match[3];
//    for(int i = 0 ; i < 3 ; i++) {
//        single_match[i].create(800,800,CV_8UC1);
//        single_match[i] = cv::Scalar::all(255);//清零
//        for(int j = 0 ; j < 10 ; j++) {
//            cv::line(single_match[i],cv::Point( 10*j,j),cv::Point(10*j,799) , cv::Scalar(0) );
//        }
//    }
//    int pcs_mark[3][800] = {0};

//    //lct
//    cv::Mat show_fore;
//    frame2.copyTo(show_fore);
//    cv::Mat lct_mark(frame1.rows,frame1.cols , CV_8UC1 , cv::Scalar(0));
//    for(int i = 0 ; i < frame1.cols ; i++){
//        for(int j = 0 ; j < frame1.rows ; j++){
////            if(if_biger_differ(subtract_value,cv::Point(i,j),frame1 , frame2)) show_fore.at<cv::Vec3b>(j,i) = cv::Vec3b(255,255,255);

//            if(subtract_value.at<cv::Vec3f>(j , i)[0] < -10 || subtract_value.at<cv::Vec3f>(j , i)[0] > 20
//                    ||subtract_value.at<cv::Vec3f>(j , i)[1] <-18 || subtract_value.at<cv::Vec3f>(j , i)[1] > 5
//                    /*||(subtract_value.at<cv::Vec3f>(j , i)[2] >2.5&&subtract_value.at<cv::Vec3f>(j , i)[2]<4 )*/ ||subtract_value.at<cv::Vec3f>(j , i)[2] < 1 ||  subtract_value.at<cv::Vec3f>(j , i)[2] > 30) {
////                cv::Vec3f var = get_var(subtract_value , cv::Point(i,j));
////                for(int k = 0 ; k < 3 ; k++) {
////                      //single match hist
////                      int hist_x = 100*(float)var[k] + 10;
////                      if(0 == k && hist_x>100) frame2.at<cv::Vec3b>(j,i)[0] = 255;
////                      if(1 == k && hist_x>100) frame2.at<cv::Vec3b>(j,i)[1] = 255;
////                      if(2 == k && hist_x>30 || hist_x < 20) frame2.at<cv::Vec3b>(j,i)[2] = 255;
////                      if(hist_x >=800)continue;
////                      if(pcs_mark[k][hist_x] < 800) single_match[k].at<uchar>(800-pcs_mark[k][hist_x]++,hist_x) = 150;

////                }
//                if(if_biger_differ(subtract_value,cv::Point(i,j),frame1 , frame2)) show_fore.at<cv::Vec3b>(j,i) = cv::Vec3b(0,0,255);
//                else show_fore.at<cv::Vec3b>(j,i) = cv::Vec3b(255,255,255);
//                if(if_biger_differ(subtract_value,cv::Point(i,j),frame1 , frame2)) lct_mark.at<uchar>(j,i) = 255;
//                else lct_mark.at<uchar>(j,i) = 100;
//            }

//        }
//    }

//    lct_detection(lct_mark);
//    cv::imshow("show_froe" , show_fore);
//    cv::imwrite("show_froe___.bmp",show_fore);
//    cv::imshow("froe" , lct_mark);
//    cv::imwrite("froe_lct___.bmp" , lct_mark);
//    cv::waitKey();
//   cv::imshow("p",single_match[0]);
//   cv::imshow("c",single_match[1]);
//   cv::imshow("s",single_match[2]);
//   cv::imshow("froe" , show_fore);
//   cv::waitKey();



//    //cluster 判断
//    std::vector<int> cluster_centre;
//    find_center(corner_points , cluster_centre , frame1.rows * frame1.cols);
//    cout<<"cluster_num "<<get_cluster(corner_points , cluster_centre , frame1.rows * frame1.cols)<<endl;
//    for(int i = 0 ;  i < frame1.rows*frame1.cols ; i++) {
//        if(-1 == corner_points[i].cluster_mark ) {
//            int y = i/frame2.cols;
//            int x = i%frame2.cols;
//            frame2.at<cv::Vec3b>(y,x) = cv::Vec3b(255,255,255);
//        }
//    }
//    cv::imshow("fore_cluster",frame2);
//    cv::imwrite("fore_cluster.bmp",frame2);
//    DWORD total_time = GetTickCount();
//    std::cout << "the total run time is:" << (total_time - start_time) << "ms!" << std::endl;
//    cv::waitKey();


//// 粗浅密度判断
//    get_cluster_map(subtract_value);
//    cv::Mat hist;
////    int mark = get_hist(hist);
////    get_first_fore(frame2,5000);
//    get_first_fore_dark(frame2,frame1,2);


//    end_time = GetTickCount();
//    std::cout << "The sub run time is:" << (end_time - start_time) << "ms!" << std::endl;




//    cv::imshow("subtract",subtract_value);
//    cv::waitKey(0);
//    cv::imwrite("lab_subtract.bmp",subtract_value);
//    cv::Mat frame3;
//    frame1.copyTo(frame3);

//    for(int i = 0 ; i < frame1.cols ; i++)
//        for(int j = 0 ; j < frame1.rows ; j++){
//            int temp = 1;
//            for(int k = 0 ; k < 3 ; k++){
//                temp *= (int)frame1.at<cv::Vec3b>(j,i)[k];
//            }
//            if(temp == 0) frame1.at<cv::Vec3b>(j,i) = cv::Vec3b(255,255,255);
//        }

//    cv::imwrite("hsv_black.bmp",frame1);

//    for(int i = 0 ; i < frame1.cols ; i++)
//        for(int j = 0 ; j < frame1.rows ; j++)
//            for(int k = 0 ; k < 3 ; k++)
//                frame3.at<cv::Vec3b>(j,i)[k] =subtract_value.at<cv::Vec3b>(j , i)[k];

//    cv::imshow("test",frame3);
//    cv::waitKey(0);
//    cv::imwrite("hsv_divide.bmp",frame3);

//    std::vector<cv::Mat> RGB_F;

//    cv::imwrite("lab_pcs_0,2.bmp",show_dividecolor_cluster(frame3 , 0 ,2));
//    cv::imwrite("lab_pcs_1,2.bmp",show_dividecolor_cluster(frame3 , 1 ,2));
//    cv::imwrite("lab_pcs_0,1.bmp",show_dividecolor_cluster(frame3 , 0 ,1));

//    cv::split(frame3,RGB_F);
//    cv::imwrite("lab_pcs_p.bmp",RGB_F[0]);
//    cv::imwrite("lab_pcs_c.bmp",RGB_F[1]);
//    cv::imwrite("lab_pcs_s.bmp",RGB_F[2]);
//    cv::imshow("0",RGB_F[0]);
//    cv::imshow("1",RGB_F[1]);
//    cv::imshow("2",RGB_F[2]);
//    cv::waitKey(0);





    return 0;
}

