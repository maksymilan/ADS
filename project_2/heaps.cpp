#include <vector>
#include <cmath>
#include <climits>
using namespace std;
class heaps{
    private:
    vector<pair<int,int> > h;
    public:
    int get_size();
    vector<pair<int,int>>& get_vec();
    const vector<pair<int,int>>& get_vec() const{
        return h;
    }
    heaps(){};//堆都是通过空堆进行push的
    virtual pair<int,int> pop()=0;//弹出堆顶元素
    virtual void push(pair<int,int>)=0;//将一个元素合并到堆内,不同的堆push方式是最大的差异,第一个是该点到源节点的最短距离，第二个是该点的节点值
    bool empty();
};
class fibonacci_node{
    friend class fibonacci_heap;
    private:
        int val_;
        int num_;//该节点表示的节点值
        int degree_;
        fibonacci_node* left_;
        fibonacci_node* right_;
        fibonacci_node* parent_;
        fibonacci_node* child_;//只保存第一个孩子
        bool mark_;
    public:
        fibonacci_node(int val,int num):val_(val),num_(num),degree_(0),left_(this),right_(this),parent_(nullptr),child_(nullptr),mark_(false){};      
        //左右指针初始化指向自己，这是为了避免在后面的引用过程中左右指针指向空值的情况出现
};
class fibonacci_heap:heaps{
    private:
        int num_nodes_;
        fibonacci_node* min_node;
        int max_degree;
    public:
        fibonacci_heap(){
            num_nodes_ = 0;
            min_node = nullptr;
            max_degree = 0;
        };
        void insert(fibonacci_node* other);//将一个节点加入根表
        // void merge(fibonacci_node* other);//将一个根表和该斐波那契堆的根表合并
        pair<int,int> pop();
        void push(pair<int,int> p);
        fibonacci_node* extract_min();
        fibonacci_node* find_min_root();//找到堆中最小的节点
        void removefrom(fibonacci_node* node);//将该节点移除出根表
        void consolidate();//合并斐波那契堆的所有度相等的堆
        void link(fibonacci_node* node,fibonacci_node* root);//以root为根，将node合并到root当中
};
pair<int,int> fibonacci_heap::pop(){
    fibonacci_node* min_node = extract_min();
    return make_pair(min_node->val_,min_node->num_);
}
void fibonacci_heap::push(pair<int,int> p){
    fibonacci_node* fib_n = new fibonacci_node(p.first,p.second);
    insert(fib_n);
}
void fibonacci_heap::insert(fibonacci_node* other){
    if(min_node == nullptr){
        min_node = other;
        num_nodes_++;
    }else{
        other->left_ = min_node->left_;
        min_node->left_->right_ = other;
        min_node->left_ = other;
        other->right_ = min_node;
    }
    if(other->val_ < min_node->val_){
        min_node = other;
    }//比较插入结点的值是不是比min_node的值小，对最小值进行更新
    num_nodes_++;
}
void fibonacci_heap::removefrom(fibonacci_node* node){
    node->left_->right_ = node->right_;
    node->right_->left_ = node->left_;//node的左右节点进行连接
    // node->left_ = node->right_ = node;//这一句不能写,如果写了,那么原来的堆数据就彻底丢失了
}
fibonacci_node* fibonacci_heap::extract_min(){
    //取出最小值，先将最小的根的所有孩子放入到根表当中，然后将最小的根移除出根表，将斐波那契堆进行相同度的合并（consolidate），并更新最小的节点（find_min_root）
    fibonacci_node* min = min_node;
    if(min != nullptr){
        fibonacci_node* child = min->child_;
        if(child != nullptr){
            do{
                fibonacci_node* next = child->right_;
                child->right_ = child->left_ = child;
                child->parent_ = nullptr;
                insert(child);
                child = next;
            }while(child != min->child_);
        }
        removefrom(min);
        num_nodes_ --;
        if(num_nodes_ == 0){
            min_node = nullptr;
        }
        consolidate();
    }
    return min;
}
fibonacci_node* fibonacci_heap::find_min_root(){
    fibonacci_node* current = min_node->right_;//原来的斐波那契堆将最小点移除了，但是最小点的值还没有更新
    fibonacci_node* min = current;
    do{
        if(current->val_ < min->val_){
            min = current;
        }
        current = current->right_;
    }while(current != min_node->left_);
    return min;
}
void fibonacci_heap::consolidate(){
    fibonacci_node* current = min_node;
    if(min_node == nullptr)
    return;
    max_degree = static_cast<int>(log2(num_nodes_))+1;
    vector<fibonacci_node*> degree_v(max_degree,nullptr);
    vector<fibonacci_node*> root_nodes;
    do{
        root_nodes.push_back(current);
    }while(current != min_node->left_);
    for(auto& cur:root_nodes){
        while(degree_v[cur->degree_] != nullptr){
            fibonacci_node* n = degree_v[cur->degree_];
            if(n->val_ < cur->val_)
            swap(n,cur);//确保cur指向的堆的堆顶元素始终是较小的那一个
            link(n,cur);//将n连接到cur上
            degree_v[cur->degree_] = nullptr;
            cur->degree_++;
        }
        degree_v[cur->degree_] = cur;
    }
    //通过degree_v建立新的堆
    min_node = nullptr;
    for(auto& fib_n:degree_v){
        if(fib_n != nullptr){
            if(min_node == nullptr){
                min_node = fib_n;
            }else{
                insert(fib_n);
                if(fib_n->val_ < min_node->val_){
                    min_node = fib_n;
                }
            }
        }
    }
}
void fibonacci_heap::link(fibonacci_node* node,fibonacci_node* root){
    //将node连接到root,node插入到root的第一个孩子节点之后,孩子节点也是双向循环链表
    fibonacci_node* child = root->child_;
    if(child == nullptr){
        root->child_ = node;
        return;
    }else{
        node->left_ = child->left_;
        child->left_->right_ = node;
        child->left_ = node;
        node->right_ = child; 
    }
}
