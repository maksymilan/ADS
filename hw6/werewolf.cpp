#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unistd.h>
using namespace std;
class player{
    private:
    int status;
    int honesty;
    int statement;
    public:
    static int num_players;
    static int num_wolves;
    static int num_liars;
    player(){
        set_honesty(0);
        set_status(0);
    }
    const int get_statement(){return statement;}
    int get_status(){return status;}
    int get_honesty(){return honesty;}
    void set_status(int s){status = s;}
    void set_honesty(int h){honesty = h;}//返回之前的honesty值
    void set_statement(int s){statement = s;}
};
class wolf:public player{
    public:
    wolf(player& other){
        this->set_honesty(other.get_honesty());
        this->set_status(-1);
    }
};
int get_status(vector<player>& v,int statement);
bool apply_current_player_statement(vector<player>& v,int statement);
bool paradox(vector<player>& v);
void undo_statement(vector<player>& v,int statement,int old_status=0);

void back_tracking(vector<player> & v,vector<vector<int>> & answer,int index){
    int assum_status = 1; //假设这个人是人类
    bool honesty = true;
    int old_status;
    bool self_unknown = false;
    if(index == player::num_players+1){
        //将当前玩家的身份信息装入一个vector当中，然后将这个vector push到answer当中
        if(!paradox(v))
        return;
        vector<int> temp_v;
        for(int i=1;i<v.size();++i){
            temp_v.push_back(v[i].get_status());
        }
        answer.push_back(temp_v);
        return;//遍历完了
    }
    int real_status = v[index].get_status();
    if(real_status == 0){
        self_unknown = true;
        v[index].set_status(assum_status);//先认为他是人类，第二个分支认为他是狼人
        //每一个玩家的话能用来确认一个玩家的身份，玩家1说玩家2为狼人，如果我们根据先前的假设发现玩家2是人类，那么就出现矛盾，判定玩家1说假话
        //如果玩家2的身份还没确定，那么我们先认为该玩家的话是真话，进行回溯，然后认为该玩家说的是假话，检查是否会产生矛盾，再进行回溯
        old_status = get_status(v,v[index].get_statement());
        if(apply_current_player_statement(v,v[index].get_statement())){
            v[index].set_honesty(1);
            if(paradox(v)){//如果这个玩家的真话没有造成其他的矛盾，我们就就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }//这个玩家说的话不会与现在确定的身份产生矛盾，可以尝试这个玩家说的是真话
        //这个玩家的说法会产生矛盾，只能尝试这个玩家说的是假话
        if(apply_current_player_statement(v,-v[index].get_statement())){
            v[index].set_honesty(-1);
            if(paradox(v)){//如果这个玩家的假话没有造成其他的矛盾，我们就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }
        v[index].set_status(-assum_status);
        if(apply_current_player_statement(v,v[index].get_statement())){
            v[index].set_honesty(1);
            if(paradox(v)){//如果这个玩家的真话没有造成其他的矛盾，我们就就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }//这个玩家说的话不会与现在确定的身份产生矛盾，可以尝试这个玩家说的是真话
        //这个玩家的说法会产生矛盾，只能尝试这个玩家说的是假话
        if(apply_current_player_statement(v,-v[index].get_statement())){
            v[index].set_honesty(-1);
            if(paradox(v)){//如果这个玩家的假话没有造成其他的矛盾，我们就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }
    }else if(real_status == -1){
        //表明该玩家身份已经确定为狼人，那我们只需要判断说假话还是说真话就可以了
        old_status = get_status(v,v[index].get_statement());
        if(apply_current_player_statement(v,v[index].get_statement())){
            v[index].set_honesty(1);
            if(paradox(v)){//如果这个玩家的真话没有造成其他的矛盾，我们就就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }//这个玩家说的话不会与现在确定的身份产生矛盾，可以尝试这个玩家说的是真话
        //这个玩家的说法会产生矛盾，只能尝试这个玩家说的是假话
        if(apply_current_player_statement(v,-v[index].get_statement())){
            v[index].set_honesty(-1);
            if(paradox(v)){//如果这个玩家的假话没有造成其他的矛盾，我们就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }
    }else{
        //表明该玩家身份已经确定为人类，我们只需要判断假话还是真话
        old_status = get_status(v,v[index].get_statement());
        if(apply_current_player_statement(v,v[index].get_statement())){
            v[index].set_honesty(1);
            if(paradox(v)){//如果这个玩家的真话没有造成其他的矛盾，我们就就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }//这个玩家说的话不会与现在确定的身份产生矛盾，可以尝试这个玩家说的是真话
        //这个玩家的说法会产生矛盾，只能尝试这个玩家说的是假话
        if(apply_current_player_statement(v,-v[index].get_statement())){
            v[index].set_honesty(-1);
            if(paradox(v)){//如果这个玩家的假话没有造成其他的矛盾，我们就进行回溯，否则不执行
                back_tracking(v,answer,index+1);
            }
            undo_statement(v,v[index].get_statement(),old_status);
        }
    }

    if(self_unknown)//如果原来这个下标所在的玩家的身份不确定，那么我们要恢复不确定状态
        v[index].set_status(0);
    v[index].set_honesty(0);
    return;
}
bool apply_current_player_statement(vector<player>& v,int statement){//根据玩家的说法来确定身份，如果与之前的身份矛盾，那么返回false，这个玩家的说法不成立
    int index,status;
    if(statement < 0)
        index = -statement;
    else
        index = statement;
    // status = v[index].get_status();
    status = get_status(v,statement);
    if(status == 0){
        v[index].set_status(statement/index);
        return true;//玩家的说法不与原来的身份产生矛盾
    }else if(status*statement < 0){
        return false;
    }else{
        return true;
    }
}//采用当前玩家的陈述，为真或为假时，对其他玩家的身份进行确认，返回值是被更改身份的玩家的原来的身份，undo操作就是将被更改身份的玩家恢复为原来的身份
int get_status(vector<player>& v,int statement){
    int index,status;
    if(statement < 0)
        index = -statement;
    else
        index = statement;
    status = v[index].get_status();
    return status;}

void undo_statement(vector<player>& v,int statement,int old_status){
    if(statement < 0)
        statement = -statement;
    v[statement].set_status(old_status);
}//取消这个陈述的操作，如果说，这个陈述说三号玩家是人类，那么就
//v是传入的玩家的状态表，answer是储存所有正确的答案，index表示当前玩家的下标，由于设置了哨兵节点，所以玩家的index是从1开始的
void print_answer(vector<vector<int>> &vv){
    for(auto v:vv){
        for(auto num:v){
            cout<<num<<" ";
        }
        cout<<endl;
    }
    if(vv.size() == 0){
        cout<<"No Solution";
        return;
    }
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
bool paradox(vector<player>& v){
    int num_wolves=0,num_liar_wolves=0,num_identities=0,liars=0,num_not_liars_wolves=0,num_people=0,not_liars=0,unknow_honesty=0;
    for(int i=1;i<v.size();++i){
        if(v[i].get_status() == -1){
            num_wolves++;
            if(v[i].get_honesty() == -1)
            num_liar_wolves++;
            if(v[i].get_honesty() == 1)
            num_not_liars_wolves++;
        }
        if(v[i].get_status() == 1){
            num_people++;
        }
        if(v[i].get_status() != 0)
            num_identities++;
        if(v[i].get_honesty() == -1)
            liars++;
        if(v[i].get_honesty() == 1)
            not_liars++;
        if(v[i].get_honesty() == 0)
            unknow_honesty++;
    }
    if(num_people > player::num_players - player::num_wolves){
        return false;
    }
    if(liars > player::num_liars)
        return false;
    if(num_identities==player::num_players){
        if(num_wolves != player::num_wolves)
        return false;
    }
    if(num_wolves > player::num_wolves){
        return false;
    }else if(num_wolves == player::num_wolves){
        if((num_liar_wolves == 0&&num_not_liars_wolves == player::num_wolves)||num_liar_wolves == player::num_wolves)
            return false;
    }
    if(unknow_honesty == 0){
        if(liars != player::num_liars)
        return false;
    }
    return true;
}//对当前的状态进行计数，判定条件有，狼人数不能大于给定的数量，如果狼人数等于给定的数量，那么撒谎的狼人数不能为0也不能等于给定的狼人数量
//如果每一位玩家的身份都确定了，那么总的撒谎的人的数量要等于给定的数量
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
    vector<wolf> wolves_v; 
    player_v.push_back(node);
    for(int i=0;i<num_players;++i){
        player p;
        char c;
        int statement;
        file>>statement;
        p.set_statement(statement);
        // if(file.fail())
        // cout<<"invalid input"<<endl;
        player_v.push_back(p);
    }
    // for(auto p:player_v){
    //     cout<<p.get_statement()<<" ";
    // }
    // cout<<endl;
    back_tracking(player_v,answer,1);
    print_answer(answer);
}
