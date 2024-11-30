#ifndef CG
#define CG
#include <iostream>
#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

int find_position_LM(std::vector<int> container,int width);//return the start position can hold,if no position can hold the rectangle, return -1
void get_case(int n,std::string path);//n is the number of rectangles randomly generated, and this function is used to generate test cases
int cal_height_online_1(int num);//the simplest online algorithm,return the approximation height
void generate_file_folder(int n);
int cal_height_offline_vertical_1(int num);
int cal_height_offline_horizontally_1(int num);
#endif