#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unistd.h>
using namespace std;
struct player{
    int status;
    int honesty;
    int statement;
    static int num_players;
    static int num_wolves;
    static int num_liars;
};
bool backtracking(vector<player> &v,vector<vector<int>>& answer,int index);
bool paradox(vector<player> &v);
void print_answer(vector<vector<int>> &vv){
    vector<vector<int>> ans;
    for(auto v:vv){
        vector<int> pos;
        for(int i=0;i<v.size();++i){
            if(v[i]==-1)
                pos.push_back(i+1);
        }
        ans.push_back(pos);
    }

    sort(ans.begin(),ans.end());
    vector<int> v=ans[ans.size()-1];
    reverse(v.begin(),v.end());
    for(int i=0;i<v.size();++i){
        if(i == 0)
        cout<<v[i];
        else
        cout<<" "<<v[i];
    }
}

int player::num_liars = 0;
int player::num_players = 0;
int player::num_wolves = 0;
int main(){
    ifstream file("test.txt");
    if(!file.is_open()){
        cout<<"file not open"<<endl;
        return 0;
    }
    int num_players,num_wolves,num_liars;
    file>>num_players>>num_wolves>>num_liars;
    player::num_liars = num_liars;
    player::num_players = num_players;
    player::num_wolves = num_wolves;
    player node;//哨兵
    vector<vector<int>> answer;
    vector<player> player_v;
    player_v.push_back(node);
    for(int i=0;i<num_players;++i){
        player p;
        char c;
        int statement;
        file>>statement;
        p.statement = statement;
        player_v.push_back(p);
    }
    if(backtracking(player_v,answer,player_v.size()-1)){
        print_answer(answer);
    }else{  
        cout<<"No Solution";
    }
    return 0;
}
//自己的解法是，由每一个人的说法来进行身份确定，然后根据当前确定的身份判断是否会产生矛盾，对于给定的sample来说，答案是正确的，但是oj上的测试案例通过不了
//参考的解法是，对每一个人进行身份假设，然后，根据假设的身份判断是否会产生矛盾
bool paradox(vector<player>& v){
    int statement,status,index;
    int cnt_people=0,cnt_wolf=0,liars=0,wolf_liars=0,peo_liars=0,unknown_status=0;
    for(int i=1;i<v.size();++i){
        player p = v[i];
        statement = p.statement;
        index = statement;
        status = p.status;
        if(statement<0)
            index = -index;
        if(status == 1){
            cnt_people++;
            if(statement*v[index].status < 0){
                liars++;
                peo_liars++;
            }
        }else if(status == -1){
            cnt_wolf++;
            if(statement*v[index].status < 0){
                liars++;
                wolf_liars++;
            }
        }else{
            unknown_status++;
        }
    }
    if(unknown_status != 0){
        if(liars > player::num_liars||cnt_wolf > player::num_wolves||wolf_liars 
        >= player::num_wolves||cnt_people > player::num_players-player::num_wolves)
            return false;
        return true;
    }else{
        if(wolf_liars==0||wolf_liars >= player::num_wolves)
            return false;
        if(liars != player::num_liars)
            return false;
        if(cnt_wolf != player::num_wolves)
            return false;
        return true;
    }
}
bool backtracking(vector<player> &v,vector<vector<int>>& answer,int index){
    if(index < 1){
        if(!paradox(v))
        return false;
        vector<int> temp_v;
        for(int i=1;i<v.size();++i){
            temp_v.push_back(v[i].status);
        }
        answer.push_back(temp_v);
        return true;
    }
    bool Find = false;
    bool right = paradox(v);
    if(right){
        v[index].status = -1;//优先考虑是狼人
        Find = backtracking(v,answer,index-1);
        if(!Find){
            v[index].status = 0;
        }
    }
    if(!Find&&right){//如果假设为狼人没找到，那么假设为人再进行寻找
        v[index].status = 1;
        Find = backtracking(v,answer,index-1);
        if(!Find){
            v[index].status = 0;
        }
    }
    return Find;
}