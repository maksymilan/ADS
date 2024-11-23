#include <iostream>
using namespace std;
class node{
    private:
    static const int order = 3;
    node* next_same_node_sibling;//同一节点的不同的值的指针
    node* left;
    node* right;
    node* parent;
    node* sibling;
    int val;
    public:
    node* get_left(){
        return left;
    }
    node* get_right(){
        return right;
    }
    node* get_parent(){
        return parent;
    }
    node* get_same_node_sibling(){
        return ;
    }
    int get_val(){
        return val;
    }
    void insert(node* new_node);
    //B+树的insert操作还不清楚，逻辑上来说，先寻找位置，也就是进行查找，如果找到的叶子节点中没有这个值，那么就将这个值插入到叶子节点，如果有的话就输出“重复”
    void print_tree();//层序打印树
    void split();//当树的节点出现上溢出的时候，将该节点分割成三份，中间节点上移到父节点，另外两边节点连接到父节点
    node(int value):val(value),parent(nullptr),left(nullptr),right(nullptr),next_same_node_sibling(nullptr){};
};
class Root{
    private:
    node* Bp_root;
    public:
    void set_root(node* new_root){
        Bp_root = new_root;
    }
    node* get_root(){
        return Bp_root;
    }
    Root(node* r):Bp_root(nullptr){}
};
//Root类始终保存着这棵B+树根节点的信息，当对树插入导致根节点上溢后根节点信息改变时，只需要修改设置的根节点对象，就能保证后续的插入的开始地址不会出现问题
int main(){
    int num_node;
    cin>>num_node;
    Root* r = new Root(nullptr);
    for(int i=0;i<num_node;++i){
        int val;
        cin>>val;
        node* new_node = new node(val);
        if(r->get_root() == nullptr){
            r->set_root(new_node);
        }else{
            node* root = r->get_root();
            root->insert(new_node);
        }
    }
    r->get_root()->print_tree();
    return 0;
}