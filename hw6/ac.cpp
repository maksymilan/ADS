#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_map>
using namespace std;

// WolfGame 类定义，处理狼人游戏的核心逻辑
class WolfGame {
public:
    int total, numberOfWolf, numberOfLiar;  // 玩家总数、狼人数量、撒谎者数量
    int actualNumberOfLiar;  // 实际的撒谎者数量
    vector<int> assume;  // 存储每个玩家的陈述，正数表示指认某玩家是人类，负数表示指认某玩家是狼人
    vector<vector<int>> id;  // 存储每个玩家的陈述列表，id[i] 中存储所有指认 i 的陈述
    vector<int> wolfIndex;  // 存储当前假设的狼人编号
    vector<int> ans;  // 存储符合条件的狼人组合
    vector<bool> isWolf;  // 记录每个玩家是否是假设为狼人，true 为狼人，false 为非狼人
    bool findAnswer;  // 标志是否已经找到符合条件的解

    // 构造函数，初始化游戏参数
    WolfGame(int n, int w, int l):total(n), numberOfWolf(w), numberOfLiar(l) { 
        // 初始化变量大小
        this->assume.resize(n+1);  // 玩家编号从1开始，因此分配 n+1 大小的数组
        this->id.resize(n+1);  // 每个玩家的指认列表
        this->isWolf.resize(n+1);  // 每个玩家是否是狼人的标志
        this->actualNumberOfLiar = 0;  // 初始化实际撒谎者数量为0
        this->findAnswer = false;  // 初始化为未找到解
    }

    // 打印最终找到的解
    void PrintAnswer() {
        if (this->findAnswer) {  // 如果找到了解
            cout << this->ans[0];  // 输出第一个狼人编号
            for (int i = 1; i < this->ans.size(); i++)  // 输出剩余的狼人编号
                cout << " " << this->ans[i];
            cout << endl;
        } else cout << "No Solution" << endl;  // 如果没有找到符合条件的解，输出"No Solution"
    }

    // 深度优先搜索函数，用于枚举所有可能的狼人组合
    void Search(int idx) {
        if (this->findAnswer) return;  // 如果已经找到答案，直接返回
        if (this->actualNumberOfLiar > this->numberOfLiar) return;  // 如果当前撒谎者数量超过限制，直接返回
        
        // 如果当前狼人数量已经达到要求，则进行验证
        if (this->wolfIndex.size() == this->numberOfWolf) {
            int lieWolf = 0, liarNumber = 0;  // lieWolf 记录说谎的狼人数量，liarNumber 记录总的撒谎者数量
            for (int i = 1; i <= this->total; i++) {
                int cId = this->assume[i];  // 当前玩家的陈述
                // 如果玩家 i 的陈述与当前假设的身份不符，则此玩家是撒谎者
                if ((cId > 0 && this->isWolf[abs(cId)]) || (cId < 0 && !this->isWolf[abs(cId)])) {
                    lieWolf += this->isWolf[i] ? 1 : 0;  // 如果撒谎者本身是狼人，计入 lieWolf
                    liarNumber += 1;  // 撒谎者数量加1
                }
            }
            // 检查条件：有至少一个狼人撒谎，但并非所有狼人都撒谎，且总的撒谎者数量等于给定数量
            if (lieWolf > 0 && lieWolf < this->numberOfWolf && liarNumber == this->numberOfLiar) {
                this->findAnswer = true;  // 标记找到答案
                for (const auto &x : this->wolfIndex) ans.push_back(x);  // 将当前的狼人组合保存为答案
            }
            return;  // 返回上层递归
        }

        // 枚举当前玩家 idx 是否是狼人
        for (int i = 0; i < 2 && idx > 0; i++) {  // i=0 假设玩家是狼人，i=1 假设玩家不是狼人
            if (i == 0) {
                this->isWolf[idx] = true;  // 假设当前玩家 idx 是狼人
                this->wolfIndex.push_back(idx);  // 将其加入狼人编号列表
            }
            int sym = (i == 0) ? 1 : -1;  // sym 用于判断当前假设的正反情况（狼人 vs 非狼人）
            int tmp = this->actualNumberOfLiar;  // 保存当前撒谎者数量，便于回溯
            // 更新撒谎者数量，根据当前玩家 idx 的陈述及其假设的身份
            for (const auto &x : this->id[idx]) {
                this->actualNumberOfLiar += x * sym > 0 ? 1 : 0;  // 如果陈述与假设身份不符，增加撒谎者数量
            }
            // 递归搜索下一个玩家
            this->Search(idx-1);
            // 回溯状态
            if (i == 0) {
                this->isWolf[idx] = false;  // 假设当前玩家不是狼人
                this->wolfIndex.pop_back();  // 从狼人编号列表中移除
            }
            this->actualNumberOfLiar = tmp;  // 恢复撒谎者数量
        }
    }
};

int main() {
    int n, w, l;
    cin >> n >> w >> l;  // 读取玩家总数 n，狼人数量 w，撒谎者数量 l
    WolfGame *g = new WolfGame(n, w, l);  // 创建 WolfGame 对象
    for (int i = 1; i <= n; i++) {
        cin >> g->assume[i];  // 读取每个玩家的陈述
        int sym = g->assume[i] > 0 ? 1 : -1;  // 判断陈述是正数还是负数，正数表示认定某玩家是人类，负数表示认定某玩家是狼人
        g->id[g->assume[i] * sym].push_back(sym * i);  // 将陈述加入相应玩家的指认列表
    }

    g->Search(g->total);  // 从最后一个玩家开始进行深度优先搜索，尝试找到符合条件的解
    g->PrintAnswer();  // 输出结果

    return 0;
}