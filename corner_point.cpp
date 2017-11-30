#include "corner_point.h"

using namespace std;
double Corner_point::radius = 20;
double Corner_point::core_dis = 20;
double threep_radius = Corner_point::radius * Corner_point::radius * Corner_point::radius;
double threep_core_dis = Corner_point::core_dis * Corner_point::core_dis * Corner_point::core_dis;




//void Corner_point::move(cv::Point3f fxy)
//{
//    dispalcement = fxy;
//    cluster_dpm = fxy * 10;
//    point = point + fxy;
//    cluster_mark = -1; //初始化聚类标记
//    if_center =false;
//}

//void Corner_point::get(cv::Point3f get_point)
//{
//    point = get_point;
//}

int Corner_point::get_density(Corner_point Points[], int size)
{

    density = 0;//密度初始化
    for(int i=0 ; i < size ; i++)
    {
        if(Points[i].r_o_w > 0 )//该点可使用为约束时才能计算在密度之内
        {
            cv::Point3f fxy;
            fxy = cluster_dpm - Points[i].cluster_dpm;
            if(( (fxy.x * fxy.x)+(fxy.y * fxy.y) + (fxy.z * fxy.z)) < threep_radius) density++;
        }

    }
    return density;
}

int Corner_point::get_db_density(Corner_point Points[], int size)
{

    db_density = 0;//密度初始化
    for(int i=0 ; i < size ; i++)
    {
        if(Points[i].r_o_w > 0 )//该点可使用为约束时才能计算在密度之内
        {
            cv::Point3f fxy;
            fxy = cluster_dpm - Points[i].cluster_dpm;
            if(( (fxy.x * fxy.x)+(fxy.y * fxy.y) + (fxy.z * fxy.z)) < threep_radius) density++;
        }

    }
    return db_density;
}

double Corner_point::get_mdistance(Corner_point Points[] , int size)
{
    cv::Point3d fxy;
    min_distance = -1;
    for(int i=0 ; i < size ; i++)
    {
        if(Points[i].r_o_w > 0 )//该点可使用为约束时才能计算在密度之内
        {
            if(density < Points[i].density)
            {
                fxy = cluster_dpm - Points[i].cluster_dpm;
                double d = std::sqrt(fxy.x*fxy.x + fxy.y*fxy.y + fxy.z*fxy.z);
                if((min_distance < 0)|| d < min_distance ) min_distance = d;
            }
        }

    }
    return min_distance ;
}




void find_center(Corner_point Corner_points[] , std::vector<int> & cluster_centre , int size)//粗浅判定方法
{
    std::cout<<"find_center"<<std::endl;
    long long mean_density = 0 ;
    double mean_dis = 20;

    //求每个点的密度
    for(int i=0 ; i < size ; i++ )
    {
        mean_density += Corner_points[i].get_density(Corner_points, size);
    }
//    mean_density = mean_density / size;
    mean_density = 2000;
    std::cout<<"mean_density"<<std::endl;
    //求每个点的最短距离
    for(int i=0 ; i < size ; i++ )
    {
        Corner_points[i].get_mdistance(Corner_points , size);
    }
    std::cout<<"mdistance"<<std::endl;
    //符合密度大距离大的中心点
    for(int i=0 ; i <size ; i++)
    {
        if(Corner_points[i].min_distance == -1)
        {
            cluster_centre.push_back(i);
        }
        else if (Corner_points[i].min_distance > mean_dis)
        {
            if(Corner_points[i].density > mean_density)
            {
                cluster_centre.push_back(i);
                Corner_points[i].if_center = true;
            }

        }
    }
    std::cout<<"center"<<std::endl;


}



bool if_inside(Corner_point center,Corner_point test_point)
{
    cv::Point3f fxy = center.cluster_dpm - test_point.cluster_dpm;
    if((fxy.x*fxy.x + fxy.y*fxy.y + fxy.z*fxy.z) <= threep_core_dis) return true;
    else return false;
}





int get_cluster(Corner_point Corner_points[], std::vector<int> cluster_centre , int size)//DBSCAN、层次遍历
{
    std::cout<<"get_cluster"<<std::endl;
    std::vector<int> queue_point;//遍历所用队列
//    std::vector<int>::iterator ite;//队列容器迭代器

    int cluster_mark = 0;//记录类标记

    double mean_db_density = 0;//核心点密度阈值
//    for(int i=0 ; i < size ; i++ )
//    {
//        mean_db_density +=  Corner_points[i].density;
//    }
//    mean_db_density = mean_db_density /  size;
//    cout<<"mean_density "<<mean_db_density<<endl;
    mean_db_density = 50;

    for(int i =0; i < (int)cluster_centre.size(); i++)//i为中心点标数
    {
        if(Corner_points[cluster_centre[i]].cluster_mark <0 )//遍历过的中心点不需要再次参与运算
        {

            Corner_points[cluster_centre[i]].cluster_mark = cluster_mark;
            for(int j=0 ; j < size ; j++ )//找到首层范围内点，j为全体点标数
            {
                if(Corner_points[j].cluster_mark <0 )
                {
                    if( if_inside(Corner_points[cluster_centre[i]],Corner_points[j]) )
                    {
                        Corner_points[j].cluster_mark = cluster_mark;
                        if(Corner_points[j].db_density >= mean_db_density) queue_point.push_back(j);//密度大则为核心对象
//                        else if(Corner_points[j].if_center) queue_point.push_back(j);
                    }
                }

            }

            while(!queue_point.empty())//处理队列，层次遍历
            {
                for(int i=0 ; i < size ; i++ )//找到范围内点
                {
                    if(Corner_points[i].cluster_mark <0 )
                    {
                        if( if_inside(Corner_points[queue_point[0]],Corner_points[i]) )
                        {
                            Corner_points[i].cluster_mark = cluster_mark;
                            if(Corner_points[i].db_density >= mean_db_density) queue_point.push_back(i);//密度大则为核心对象
//                            else if(Corner_points[j].if_center) queue_point.push_back(i);
                        }
                    }
                }
//                std::cout<<"queue num : "<< queue_point.size()<<std::endl;
                queue_point.erase(queue_point.begin());//处理完成后从队列中剔除
            }
            cluster_mark++;//一次归类完成后类标记加一
        }        
    }
    cout<<"end"<<endl;
    return cluster_mark;//返回类总数（从1开始）
}



