#include <iostream>
#include <fstream>
using namespace std;
int main(){
    ofstream file("test.txt");
    file<<1000<<endl;
    for(int i=1;i<1000;++i){
        file<<i<<" ";
    }
    file<<1000;
    return 0;
}