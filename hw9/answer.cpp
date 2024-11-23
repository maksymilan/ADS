#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;
void print(vector<pair<int,double>>&);
bool Greater(pair<int,double> a,pair<int,double> b){
    return a.first<b.first;
}
int main(){
    int tank_capacity,distance,distance_per_gas,num_Of_station;
    vector<pair<int,double>> s_p;
    int station_dis;
    double station_pri;
    cin>>tank_capacity>>distance>>distance_per_gas>>num_Of_station;
    for(int i=0;i<num_Of_station;++i){
        cin>>station_pri;
        cin>>station_dis;
        s_p.push_back(make_pair(station_dis,station_pri));
    }
    s_p.push_back(make_pair(distance,0));
    sort(s_p.begin(),s_p.end(),Greater);
    double cur_cap=0,sum=0,distanc=0,volume=0,longest_dis=0;
    bool can_reach = true;
    for(int i=0;i<num_Of_station;++i){
        volume=0;
        int next_dis = s_p[i+1].first-s_p[i].first;
        double need_volume = 1.0*next_dis/distance_per_gas;
        if(need_volume > tank_capacity){//如果两个加油站之间的距离，加满油都到不了，那么直接break;
            can_reach = false;
            longest_dis = s_p[i].first+tank_capacity*distance_per_gas;
            break;
        }
        double virtual_cap = cur_cap;
        for(int j=i+1;j<num_Of_station+1;++j){//计算当前的加油站应该加多少油
            double temp_volume = 1.0*(s_p[j].first-s_p[j-1].first)/distance_per_gas;
            if(volume + cur_cap > tank_capacity){
                volume = tank_capacity - cur_cap;
                break;
            }
            if(s_p[j].second<s_p[i].second){
                if(j == i+1){//如果下一个加油站价格比这个加油站小
                    //如果当前的油量能到达下一个加油站，那么就不用加油
                    if(virtual_cap >= need_volume){
                        volume += 0;
                    }else{//如果不够的话就加油到刚好到达下一个加油站，如果
                        volume += need_volume - virtual_cap;
                    }
                    break;
                }else{//如果是第二个或者第三个加油站价格比较便宜，那么加油到刚好到达这个站，如果无法加油到刚好到达这个站就最多加满
                    if(temp_volume + volume + cur_cap > tank_capacity){
                        volume = tank_capacity - cur_cap;
                    }else{
                        volume += temp_volume;
                    }
                    break;
                }
            }else{//如果下一个加油站比这个加油站贵，那么先加油到刚好到下一个加油站，然后继续看再下一个加油站
                //如果当前的油量能到达下一个加油站，那么就不用加油
                if(cur_cap >= temp_volume){
                    volume += 0;
                }else{//如果不够的话就加油到刚好到达下一个加油站
                    double add = temp_volume - virtual_cap;
                    virtual_cap += add;
                    volume += add;
                }
            }
            virtual_cap -= temp_volume;
        }
        //将加的油用的钱进行更新
        sum += volume*s_p[i].second;
        //对当前的油量进行更新
        cur_cap += volume;
        //到达下一个目标点用了多少油，对油量进行更新
        if(i != num_Of_station-1)
        cur_cap -= 1.0*(s_p[i+1].first-s_p[i].first)/distance_per_gas;
        else
        cur_cap -= 1.0*(distance-s_p[i].first)/distance_per_gas;
    }
    if(!can_reach)
    cout<<"The maximum travel distance = "<<fixed<<setprecision(2)<<longest_dis;
    else
    cout<<fixed<<setprecision(2)<<sum;
}

