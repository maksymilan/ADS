#include <iostream>
#include <memory>
using namespace std;
class node{
    public:
        node* get_parent();
        node* get_left();
        node* get_right();
        bool get_balence();
        void binary_insert(node* new_node);//正常的二叉树插入，插入后回溯每一个父节点的平衡因子
        void set_balence_factor();
        void set_parent(node* node);
        void set_left(node *node);
        void set_right(node* node);
        int get_value();
        int get_height();
        int cal_height();//递归地表示树的高度，树的高度等于1加左子树和右子树高度的最大值，如果是叶子节点就返回0
        void set_height();//将得到的高度设置为私有变量
        //接下来定义四种旋转方法
        //如果插入节点的值比第一个不平衡节点的值小，那么再比较插入节点的值和不平衡节点的左子树根节点的值，如果插入节点的值比不平衡节点的值大，那么再比较插入节点和不平衡节点的右子树的根节点的值
        //分为LLrotaion,LRrotation,RRrotation,RLrotation
        //没一次旋转我们只需要确定即将成为根节点的节点和原来是根节点的节点，这两个节点确定就能进行旋转了，如果是原右子节点要成为根节点那么就是左旋，如果是原左子节点要成为根节点那么就是右旋
        //我们只对不平衡节点调用该方法，所以以下的方法不需要参数
        void LL_rotation();//这种情况只需要选择不平衡节点和它的左子节点进行一次右旋
        void RR_rotation();//这种情况只需要选择不平衡节点和它的右子节点进行一次左旋
        void LR_rotation();//这种情况要先选择不平衡节点的左子树根节点和左子树的右节点进行一次左旋，然后选择新的左子树根节点和不平衡节点进行一次右旋
        void RL_rotation();//这种情况要先选择不平衡节点的右子树根节点和右子树的左节点进行一次右旋，然后选择新的右子树根节点和不平衡节点进行一次左旋
        void choose_rotation_method(node*& insertion);//传入插入的节点，只有当目前的节点为不平衡节点时才需要调用这个方法来选择旋转的方式
        node(){};
        node(int value):val(value),left(nullptr),right(nullptr),parent(nullptr),height(0){};
    private:
        bool balence;//返回以该节点为根节点的树是否平衡
        node* parent;
        node* left;
        node* right;
        int balance_factor;
        int height;
        int val;

};
void left_rotation(node* old_node);//选择两个节点，进行左旋
void right_rotation(node* old_node);//选择两个节点，进行右旋
node* find_root(node* old_root);

int main(){
    int node_num;
    cin>>node_num;
    int value;  
    node* root = new node(-1);;
    for(int i=0;i<node_num;i++){
        cin>>value;
        node* ptr_node = new node(value);
        if(root->get_value() != -1){
            root = find_root(root);
        }//当根节点被旋转后，要重新定位根节点，否则二叉树插入的起点会出现问题，导致二叉树的性质被破坏
        root->binary_insert(ptr_node);
        node* parent = ptr_node->get_parent();
        while(parent != nullptr){
            node* grandparent = parent->get_parent();
            parent->set_balence_factor();
            if(!parent->get_balence()){
                parent->choose_rotation_method(ptr_node);

            }//如果不平衡
            parent = grandparent;
        }
    }
    node *final = find_root(root);
    cout<<final->get_value()<<endl;
    return 0;
}
void left_rotation(node* old_node){
    node* grandparent = old_node->get_parent();
    node* new_node = old_node->get_right();
    if(grandparent != nullptr){
        if((grandparent->get_value()) > (old_node->get_value())){
            grandparent->set_left(new_node);
        }else{
            grandparent->set_right(new_node);
        }
        new_node->set_parent(grandparent);
    }else{
        new_node->set_parent(nullptr);
    }
    old_node->set_right(new_node->get_left());
    if(old_node->get_right() != nullptr){
            old_node->get_right()->set_parent(old_node);
    }
    new_node->set_left(old_node);
    old_node->set_parent(new_node);
}
void right_rotation(node* old_node){
    node* grandparent = old_node->get_parent();
    node* new_node = old_node->get_left();
    if(grandparent != nullptr){
        if((grandparent->get_value())>(old_node->get_value())){
            grandparent->set_left(new_node);
        }else{
            grandparent->set_right(new_node);
        }
        new_node->set_parent(grandparent);
    }else{
        new_node->set_parent(nullptr);
    }
    old_node->set_left(new_node->get_right());
    if(old_node->get_left() != nullptr){
        old_node->get_left()->set_parent(old_node);
    }
    new_node->set_right(old_node);
    old_node->set_parent(new_node);
}
node* find_root(node* old_root){
    node* parent = old_root->get_parent();
    while(parent != nullptr){
        old_root = parent;
        parent = old_root->get_parent();
    }
    return old_root;
}
bool node::get_balence(){
    return balence;
}
node* node::get_parent(){
    return parent;
}
node* node::get_left(){
    return left;
}
node* node::get_right(){
    return right;
}
void node::set_parent(node* node){
    parent = node;
}
void node::set_left(node* node){
    left = node;
}
void node::set_right(node* node){
    right = node;
}
int node::get_value(){
    return val;
}
int node::get_height(){
    set_height();
    return height;
}
void node::binary_insert(node* new_node){
    if(val == -1){
        val = new_node->get_value();
        return;
    }//初始化的根节点为-1，只会执行一次
    if(val < new_node->get_value()){
        if(right == nullptr){
            new_node->set_parent(this);
            right = new_node;
            return;
        }else{
            right->binary_insert(new_node);
        }
    }else{
        if(left == nullptr){
            new_node->set_parent(this);
            left = new_node;
            return;
        }else{
            left->binary_insert(new_node);
        }
    }
}
int node::cal_height(){
    if(left == nullptr&&right == nullptr){
        return 0;
    }
    if(left == nullptr&&right != nullptr){
        return 1+right->cal_height();
    }
    if(left != nullptr&&right == nullptr){
        return 1+left->cal_height();
    }
    return 1+max(left->cal_height(),right->cal_height());
}
void node::set_height(){
    int h = cal_height();
    height = h;
}
void node::set_balence_factor(){
    if(left == nullptr&&right == nullptr){
        balance_factor = 0;
    }
    if(left == nullptr&&right != nullptr){
        balance_factor = right->get_height()+1;
    }
    if(left != nullptr&&right == nullptr){
        balance_factor = left->get_height()+1;
    }
    if(left != nullptr && right != nullptr){
        balance_factor = abs(left->get_height()-right->get_height());
    }
    if(0<=balance_factor&&balance_factor<=1){
        balence = true;
    }else{
        balence = false;
    }
}
void node::choose_rotation_method(node*& insertion){
    if(insertion->get_value() < val){
        //左儿子节点一定是存在的，如果不存在，插入的节点一定是作为左儿子节点，让左子树的高度变为1，
        //因为原来这棵树是平衡的，所以右子树的高度只能为1或0，所以如果左儿子节点不存在，插入这个节点这棵树后还是平衡的，矛盾
        if(insertion->get_value() > left->get_value()){
            LR_rotation();
        }//LR rotation
        else{
            LL_rotation();
        }
    }else{
        if(insertion->get_value() > right->get_value()){
            RR_rotation();
        }else{
            RL_rotation();
        }
    }
}
void node::LL_rotation(){
    right_rotation(this);
}
void node::RR_rotation(){
    left_rotation(this);
}
void node::LR_rotation(){
    left_rotation(left);
    right_rotation(this);
}
void node::RL_rotation(){
    right_rotation(right);
    left_rotation(this);
}