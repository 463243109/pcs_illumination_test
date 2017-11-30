#include<density_cluster.h>
using namespace std;

int DENSITY_COORDINATE::fore_density = 3;

//DENSITY_COORDINATE two_dim[800][800]; // two dim

DENSITY_COORDINATE three_dim[400][400][400] ; //three dim

void get_cluster_map(cv::Mat substract /*, DENSITY_COORDINATE **two_dim*/){ // three dim
//    std::cout<<two_dim[234][324].num<<" "<<two_dim[234][324].density<<std::endl;
//    cv::Mat density_map(800,800,CV_8UC1,cv::Scalar(255));
    int p , c ,s ;
    for(int i = 0 ; i < substract.rows ; i++ ){
        for(int j = 0 ; j < substract.cols ; j++){
            float f_p = substract.at<cv::Vec3f>(i,j)[0]/*+400*/;
            float f_c = substract.at<cv::Vec3f>(i,j)[1]/*+400*/;
            float f_s = substract.at<cv::Vec3f>(i,j)[2]/*+400*/;
            p = f_p + 200;
            c = f_c + 200;
            s = f_s + 200;
            if((p >= 400 || p < 0 || c >= 400 || c < 0 || s>=400 || s<0)) continue;
            three_dim[p][c][s].add_c(cv::Point(j,i));

            for(int ir = -RADIUS ; ir <= RADIUS ; ir++)
                for(int jr = -RADIUS ; jr <= RADIUS ; jr++)
                    for(int kr = -RADIUS ; kr <= RADIUS ; kr++){
                        if(0 != ir && 0 != jr) {
                            if(p+ir >= 400 || p+ir < 0 || c+jr >= 400 || c+jr < 0 || s+kr>=400 || s+kr<0) continue;
                            three_dim[p+ir][c+jr][s+kr].add_d();
                        }
                    }

        }
    }

}
//void get_cluster_map(cv::Mat substract /*, DENSITY_COORDINATE **two_dim*/){ // two dim
//    cv::Mat density_map(800,800,CV_8UC1,cv::Scalar(255));
//    int p , c;
//    for(int i = 0 ; i < substract.rows ; i++ ){
//        for(int j = 0 ; j < substract.cols ; j++){
//            float f_p = substract.at<cv::Vec3f>(i,j)[0];
//            float f_c = substract.at<cv::Vec3f>(i,j)[1];
//            p = f_p + 400;
//            c = f_c + 400;

//            two_dim[p][c].add_c(cv::Point(j,i));

//            if(density_map.at<uchar>(p,c) != 0)  density_map.at<uchar>(p,c)--;
//            for(int ir = -RADIUS ; ir <= RADIUS ; ir++)
//                for(int jr = -RADIUS ; jr <= RADIUS ; jr++)
//                    if(0 != ir && 0 != jr) {
//                        if(p+ir >= 800 || p+ir < 0 || c+jr >= 800 || c+jr < 0) continue;
//                        two_dim[p+ir][c+jr].add_d();
//                        if(density_map.at<uchar>(p+ir,c+jr) != 0)  density_map.at<uchar>(p+ir,c+jr)--;
//                    }
//        }
//    }
//    cout<<max_p<<" "<<min_p<<" "<<max_c<<" "<<min_c<<endl;
//    cv::imshow("density",density_map);
//    cv::waitKey(0);
//}

//int get_hist(cv::Mat & hist){
//    int max_num = 0 , max_density = 0;
//    for(int i = 0 ; i < 800 ; i++)
//        for(int j = 0 ; j < 800 ; j++){
//            if(100 < two_dim[i][j].density){
////                std::cout<<"density"<<two_dim[i][j].density<<std::endl;
////                std::cout<<" i j : "<<i <<j<<std::endl;
//            }
//            max_density = std::max(two_dim[i][j].density , max_density);
//        }
//    std::cout<<max_num<<" "<<max_density<<std::endl;
//    hist = cv::Mat(1000 , 2000 ,CV_8UC1 ,cv::Scalar(255));
////    cv::imshow("hist",hist);
////    cv::waitKey();
//    int hist_num[2000] = {0};
//    for(int i = 0 ; i < 800 ; i++)
//        for(int j = 0 ; j < 800 ; j++){
////            std::cout<<"hist_density : "<<two_dim[i][j].density/max_density * 1999<<std::endl;
//            if(two_dim[i][j].density < 900000)hist_num[two_dim[i][j].density/450/*/max_density * 1999*/] += two_dim[i][j].num ;
//        }
////    for(int i = 1 ; i < 2000 ; i++){
//////        std::cout<<hist_num[i]<<std::endl;
////        max_num = std::max(max_num , hist_num[i]);
////    }
//    std::cout<<max_num<<std::endl;

//    for(int i = 0 ; i <2000 ; i++){
//        cv::line(hist,cv::Point(i , 999),cv::Point(i,999-(hist_num[i])/100),cv::Scalar(0));
//    }
//    cv::imshow("hist",hist);
//    cv::waitKey();
//     int mark ;
//     for(int i = 0 ; i <2000 ; i++){
//         if(hist_num[i] == 0){
//             mark = i ;
//             break;
//         }
//     }
//     std::cout<<"mark : "<<mark<<std::endl;
//     return mark;
//}

//void get_first_fore(cv::Mat frame , int mark){ // two dim
//    for(int i = 0 ; i < 800 ; i++){
//        for(int j = 0 ; j < 800 ; j++){
//            if(two_dim[i][j].density > 450*mark ) continue;
//            for(int k = 0 ; k <two_dim[i][j].num ; k++){
//                frame.at<cv::Vec3b>(two_dim[i][j].coordinate[k]) = cv::Vec3b(255,255,255);
//            }
//        }
//    }
//    cv::imshow("fore",frame);
//    cv::waitKey();

//}

void get_first_fore(cv::Mat & frame , int mark){ // three dim
    for(int i = 0 ; i < 400 ; i++){
        for(int j = 0 ; j < 400 ; j++)
            for(int k = 0 ; k < 400 ; k++) {
                if(three_dim[i][j][k].density > mark ) continue;
                for(int n = 0 ; n <three_dim[i][j][k].num ; n++){

                    frame.at<cv::Vec3b>(three_dim[i][j][k].coordinate[n]) = cv::Vec3b(255,255,255);
                }
            }
    }
    cv::imwrite("fore.bmp",frame);
    cv::imshow("fore",frame);
    cv::waitKey(0);

}


void get_first_fore_dark(cv::Mat & frame ,cv::Mat frame1 , int mark){ // three dim , dark zone relese
    for(int i = 0 ; i < 400 ; i++){
        for(int j = 0 ; j < 400 ; j++)
            for(int k = 0 ; k < 400 ; k++) {
                if(three_dim[i][j][k].density > mark ) continue;
//                if((i-200)*(i-200)+(j-200)*(j-200)+(k-200)*(k-200) <= 200) continue;
                for(int n = 0 ; n <three_dim[i][j][k].num ; n++){

                    frame.at<cv::Vec3b>(three_dim[i][j][k].coordinate[n]) = cv::Vec3b(255,255,255);
                }
            }
    }
    // 腐蚀
//    cv::erode(frame, frame, cv::Mat());

//    // 膨胀
//    cv::dilate(frame, frame, cv::Mat());
    cv::imwrite("fore_dark.bmp",frame);
    cv::imshow("fore",frame);
    cv::waitKey(0);

}
