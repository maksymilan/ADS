//这个代码专门用于产生给定数量的随机的矩形
const int UPPER = 50;
#include "case_generator.h"
void get_case(int n,std::string path){//向给定的文件路径内填入给定个数的矩形的长和宽（长宽的大小没有限定，但是都小于给定的宽度）
    std::cout<<n<<" rectangls are generated"<<std::endl;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,UPPER);
    std::ofstream file(path);
    int length,width;
    for(int i=0;i<n;++i){
        length = dist(rng);
        width = dist(rng);
        file<<length<<" "<<width<<std::endl;
    }
}
void generate_file_folder(int n){
    for(int i=10;i<n;++i){
        std::string filename = "./case/"+std::to_string(i) + ".txt";
        std::ofstream file(filename);
        get_case(i,filename);
    }
}