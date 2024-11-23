#include <iostream>
using namespace std;
class node;
class B_tree;
class B_tree{
    private:
        node* root;
    public:
        static int const order=3;
        B_tree():root(nullptr){}//自动初始化根节点为null
        void set_root(node* n){
            root = n;
        }
        node* get_root(){
            return root;
        }
        void print();
};
class element{
    private:
    element* next;
    element* prev;
    node* left;
    node* right;
    int value;
    public:
    element(int val):value(val),next(nullptr),prev(nullptr){}
    int get_val(){
        return value;
    }
    element* get_next(){
        return next;
    }
    element* get_prev(){
        return prev;
    }
    void set_next(element* n){
        next = n;
    }
    void set_prev(element* p){
        prev = p;
    }
    node* get_left(){
        return left;
    }
    node* get_right(){
        return right;
    }
    void set_left(node* l){
        left = l;
    }
    void set_right(node* r){
        right = r;
    }
};//以双向链表的形式储存每个节点内部的值
class node{
    private:
    int num_ele;
    node* parent;
    element* first_element;
    bool is_leaf;
    public:
    element* find_ele(int position);
    void split(B_tree* tree);//分裂节点为三部分，中间的部分向上融入父节点,如果父节点为空，那么以选出的element新建一个根节点，如果不为空，将选出的element插入到该节点中，插入后，将该element的prev的右节点指向分裂节点的左半边，将该element的左节点指向分裂节点的左半边，将该element的右节点指向分裂节点的右半边，将该element的next的左节点指向分裂节点的右半边
    void insert(element* new_element,B_tree* tree);//插入一个新的值
    bool check_overflow(){
        if(num_ele > B_tree::order){
            return true;
        }else{
            return false;
        }
    };//检查是否溢出
    void increase(){
        ++num_ele;
    }
    void set_leaf(bool leaf_bool){
        is_leaf = leaf_bool;
    }
    bool check_leaf(){
        return is_leaf;
    }
    void add_element(element* ele);
    node():is_leaf(true){};
    node(element* ele):first_element(ele),num_ele(1),is_leaf(true){}
};
class leaf:node{
    private:
    bool is_leaf;
    public:
    leaf():is_leaf(true){};
};
int main(){
    int num_elements;
    cin>>num_elements;
    B_tree* tree = new B_tree();
    for(int i=0;i<num_elements;++i){
        int value;
        cin>>value;
        element* ele = new element(value);
        if(tree->get_root() == nullptr){
            node* ptr_node = new node(ele);
            tree->set_root(ptr_node);
        }else{
            tree->get_root()->insert(ele,tree);
        }
    }
    return 0;
}
void node::insert(element* ele,B_tree* tree){
    if(check_leaf()){
        add_element(ele);
        if(check_overflow()){
            split(tree);
        }
    }else{
        element* ptr_ele = first_element;
        while(ptr_ele->get_next() != nullptr){
            if(ele->get_val() > ptr_ele->get_val()){
                ptr_ele = ptr_ele->get_next();
            }else{
                if(ele->get_val() == ptr_ele->get_val()){
                    cout<<"dupicated"<<ele->get_val()<<endl;
                    return;
                }
                ptr_ele->get_left()->insert(ele);
                return;
            }
        }
        if(ele->get_val() > ptr_ele->get_val()){
            ptr_ele->get_right()->insert(ele,tree);
        }else{
            if(ele->get_val() == ptr_ele->get_val()){
                cout<<"dupicated"<<ele->get_val()<<endl;
                return;
            }
            ptr_ele->get_left()->insert(ele,tree);
        }
    }
}
void node::split(B_tree* tree){
    int position = (B_tree::order)/2+1;
    element* mid_ele = find_ele(position);
    element* right_firt = mid_ele;
    element* left_first = this->first_element;
    mid_ele->get_prev()->set_next(nullptr);
    mid_ele->set_prev(nullptr);
    mid_ele->get_next()->set_prev(nullptr);
    mid_ele->set_next(nullptr);//对节点内的元素进行分割
    node* right = new node(right_firt);
    if(parent == nullptr){
        node* root = new node(mid_ele);
        root->set_leaf(false);
        mid_ele->set_left(this);
        mid_ele->set_right(right);
        tree->set_root(root);
    }else{
        this->parent->add_element(mid_ele);
        mid_ele->set_left(this);
        mid_ele->set_right(right);
        if(mid_ele->get_prev()){
            mid_ele->get_prev()->set_right(mid_ele->get_left());
        }
        if(mid_ele->get_next()){
            mid_ele->get_next()->set_left(mid_ele->get_right());
        }
        if(parent->check_overflow()){
            parent->split(tree);
        }
    }
}
void node::add_element(element* ele){
    element* ptr_ele = first_element;
    element* ptr_prev;
    while(ptr_ele != nullptr){
        if(ele->get_val() > ptr_ele->get_val()){
            ptr_prev = ptr_ele;
            ptr_ele = ptr_ele->get_next();
        }else{
            if(ele->get_val() == ptr_ele->get_val()){
                cout<<"dupicated"<<ele->get_val()<<endl;
                return;
            }
            if(ptr_ele->get_prev() == nullptr){
                ele->set_next(ptr_ele);
                ptr_ele->set_prev(ele);
                increase();
                return;
            }
            ele->set_next(ptr_ele);
            ele->set_prev(ptr_ele->get_prev());
            ptr_ele->get_prev()->set_next(ele);
            ptr_ele->set_prev(ele);
            increase;
            return;
        }
    }
    ptr_prev->set_next(ele);
    ele->set_prev(ptr_prev);
    increase();
    return;
}
element* node:: find_ele(int p){
    element* ptr = first_element;
    for(int i=0;i<p;++i){
        ptr = ptr->get_next();
    }
    return ptr;
}