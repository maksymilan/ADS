#include "case_generator.h"
std::string PATH = "./case/10.txt";
//the simplest online algorithm
// const int BATCHSIZE = 10000;
const int GIVEN_WIDTH = 100;
int main(){
    // get_case(BATCHSIZE,PATH);//生成测试案例在 case文件夹下
    // generate_file_folder(10000);
    //需要有一个准确的算法和一个近似算法
    // std::cout<<cal_height_online_1(1000)<<std::endl;
    // std::cout<<cal_height_offline_vertical_1(100)<<std::endl;
    // std::cout<<cal_height_offline_horizontally_1(10)<<std::endl;

    //追加写入
    // std::ofstream online_1("./result/online_1.txt",std::ios::app);
    // std::ofstream offline_vertical("./result/offline_vertical.txt",std::ios::app);
    // std::ofstream offline_horizontal("./result/offline_horizontal.txt",std::ios::app);

    //删除写入
    std::ofstream online_1("./result/online_1.txt");
    std::ofstream offline_vertical("./result/offline_vertical.txt");
    std::ofstream offline_horizontal("./result/offline_horizontal.txt");

    for(int i=10;i<10000;++i){
        //------------------------------------------------------------//
        auto start_online_1 = std::chrono::high_resolution_clock::now();
        int result_1 = cal_height_online_1(i);
        auto end_online_1 = std::chrono::high_resolution_clock::now();
        online_1<<i<<" "<<std::chrono::duration_cast<std::chrono::microseconds>(end_online_1 - start_online_1).count()<<" "<<result_1<<std::endl;
        //-----------------------------------------------------------//

        //----------------------------------------------------------//
        auto start_offline_vertical = std::chrono::high_resolution_clock::now();
        int result_vertical = cal_height_offline_vertical_1(i);
        auto end_offline_vertical = std::chrono::high_resolution_clock::now();
        offline_vertical<<i<<" "<<std::chrono::duration_cast<std::chrono::microseconds>(end_offline_vertical - start_offline_vertical).count()<<" "<<result_vertical<<std::endl;
        //----------------------------------------------------------//

        //----------------------------------------------------------//
        auto start_offline_horizontal = std::chrono::high_resolution_clock::now();
        int result_horizontal = cal_height_offline_horizontally_1(i);
        auto end_offline_horizontal = std::chrono::high_resolution_clock::now();
        offline_horizontal<<i<<" "<<std::chrono::duration_cast<std::chrono::microseconds>(end_offline_horizontal - start_offline_horizontal).count()<<" "<<result_vertical<<std::endl;

    }
    return 0;
}

void set(int* container,int start,int end,int value){
    for(int i=start;i<=end;++i){
        container[i] = value;
    }
}

    //-----------------------online 1---------------------//
int cal_height_online_1(int num){
    std::string filename = "./case/" + std::to_string(num) + ".txt";
    std::ifstream file(filename);
    // int container[GIVEN_WIDTH];
    std::vector<int> container(GIVEN_WIDTH,0);
    // for(int i=0;i<GIVEN_WIDTH;++i){
    //     container[i] = 0;
    // }//init the container
    std::pair<int,int> p;
    std::string line;
    int count = 0;
    // std::cout<<"-------------------------cal_height_online_1-------------------"<<std::endl;
    while(getline(file,line)){
        std::stringstream ss(line);
        ss>>p.first>>p.second;//first is height,second is width
        int start = find_position_LM(container,p.second);
        int min_height = *max_element(container.begin()+start,container.begin()+start+p.second);

        //------------------------------debug mode------------------------------------------//
        // std::cout<<"the "<<++count<<"th rectangle has height: "<<p.first<<" width: "<<p.second<<std::endl;
        // std::cout<<"the start place of the rectangle is "<<start<<std::endl;
        // std::cout<<"the min_height of this rectangle's base is "<<min_height<<std::endl;
        //------------------------------debug mode-----------------------------------------//

        for(int i= start;i<start+p.second;++i){
            container[i] = min_height + p.first;
        }
    }
    // std::cout<<"-------------------------cal_height_online_1-------------------"<<std::endl;

    int height = *max_element(container.begin(),container.end());
    return height;
}

int find_position_LM(std::vector<int> container,int width){//return the Left Most start place that a rectangle can be placed
    //用width作为滑动窗口的宽度，在这个窗口内寻找最大值作为最低高度，每次返回最低高度的窗口的起点位置
    int H = container[0],start = 0;
    int min_height = container[0];
    for(int i=0;i<GIVEN_WIDTH - width;++i){
        // std::cout<<"start find location, min height now is "<< min_height <<std::endl;
        if(container[i] >= min_height){
            if(container[i+width] >= min_height)
            continue;
            else{
                if(container[i+1] < min_height){
                int new_min = *max_element(container.begin()+i+1,container.begin()+1+i+width);
                if(new_min < min_height){
                    // std::cout<<"new min height is "<<new_min<<std::endl;
                    start = i+1;
                    min_height = new_min;
                }
            }
            }
        }
    }
    // for(int i=0;i<=GIVEN_WIDTH-width;++i){
    //     int min_height = *max_element(container.begin()+i,container.begin()+i+width);
    //     if(min_height < H){
    //         H = min_height;
    //         start = i;
    //     }
    // }
    return start;
}

    //------------------------------offline 1 vertically placed--------------------------//
int cal_height_offline_vertical_1(int num){
    std::string filename = "./case/" + std::to_string(num) + ".txt";
    // std::cout<<"-------------------------cal_height_offline_vertical_1-------------------"<<std::endl;
    std::vector<std::pair<int,int>> rectangles;
    std::vector<int> container(GIVEN_WIDTH,0);
    std::string line;
    std::ifstream file(filename);
    while(getline(file,line)){
        std::stringstream ss(line);
        int height,width;
        ss>>height>>width;
        if(height<width)
        std::swap(height,width);
        rectangles.emplace_back(std::make_pair(height,width));
    }//make sure that every rectangle is placed verticaly
    std::sort(rectangles.begin(),rectangles.end(),[](const std::pair<int,int> &a,const std::pair<int,int> &b){
        return a.first > b.first;
    });//按照第一个元素的高进行降序排列
    int count = 0;
    for(auto &p :rectangles){
        int start = find_position_LM(container,p.second);
        int min_height = *max_element(container.begin()+start,container.begin()+start+p.second);

        // //-----------------------------debug mode-----------------------------------------//
        // if(min_height < 10)
        // {std::cout<<"the "<<++count<<"th rectangle has height: "<<p.first<<" width: "<<p.second<<std::endl;
        // std::cout<<"the start place of the rectangle is "<<start<<std::endl;
        // std::cout<<"the min_height of this rectangle's base is "<<min_height<<std::endl;}
        // //----------------------------debug mode------------------------------------------//

        for(int i=start;i<start+p.second;++i){
            container[i] = min_height + p.first;
        }
    }
    // std::cout<<"-------------------------cal_height_offline_vertical_1-------------------"<<std::endl;
    int height = *max_element(container.begin(),container.end());
    return height;
}

    //-------------------------offline 2 horizontally placed-------------------//
int cal_height_offline_horizontally_1(int num){
    std::string filename = "./case/" + std::to_string(num) + ".txt";
    // std::cout<<"-------------------------cal_height_offline_horizontal_1-------------------"<<std::endl;
    std::vector<std::pair<int,int>> rectangles;
    std::vector<int> container(GIVEN_WIDTH,0);
    std::string line;
    std::ifstream file(filename);
    while(getline(file,line)){
        std::stringstream ss(line);
        int height,width;
        ss>>height>>width;
        if(height > width)
        std::swap(height,width);
        rectangles.emplace_back(std::make_pair(height,width));
    }//make sure that every rectangle is placed verticaly
    std::sort(rectangles.begin(),rectangles.end(),[](const std::pair<int,int> &a,const std::pair<int,int> &b){
        return a.second > b.second;
    });//按照第一个元素的高进行降序排列
    int count = 0;
    for(auto &p :rectangles){
        int start = find_position_LM(container,p.second);
        int min_height = *max_element(container.begin()+start,container.begin()+start+p.second);
        
        // //-----------------------------------------debug mode--------------------------------//
        // std::cout<<"the "<<++count<<"th rectangle has height: "<<p.first<<" width: "<<p.second<<std::endl;
        // std::cout<<"the start place of the rectangle is "<<start<<std::endl;
        // std::cout<<"the min_height of this rectangle's base is "<<min_height<<std::endl;
        // //--------------------------------------------debug mode-----------------------------//

        for(int i=start;i<start+p.second;++i){
            container[i] = min_height + p.first;
        }
    }
    // std::cout<<"-------------------------cal_height_offline_horizontal_1-------------------"<<std::endl;
    int height = *max_element(container.begin(),container.end());
    return height;
}