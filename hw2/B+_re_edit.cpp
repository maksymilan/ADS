#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>
using namespace std;
const int order = 3;
class Bpnode{
    private:
        Bpnode* next;//叶子节点才有的值
        Bpnode* parent;
    public:
        static Bpnode* root;
        bool isleaf;
        Bpnode(int key,bool is_leaf):isleaf(is_leaf),next(nullptr),parent(nullptr){
            keys.push_back(key);
        }
        Bpnode(bool is_leaf):isleaf(is_leaf){};
        vector<Bpnode*> children;
        vector<int> keys;
        int find_position(int key){
            int it = lower_bound(keys.begin(),keys.end(),key) - keys.begin();
            return it;
        }
        void splitnode(){
            int mid;
            if(isleaf)
            mid = (order)/2+1;
            else
            mid = order/2;
            Bpnode* newnode = new Bpnode(isleaf);//构造同级节点
            int mid_value = keys[mid];
            newnode->keys.assign(keys.begin()+mid,keys.end());
            if(parent == nullptr){//说明是根节点在进行分裂
                if(isleaf){
                keys.resize(mid);
                Bpnode* newroot = new Bpnode(keys[mid],false);//构造新的根节点
                this->next = newnode;//只针对叶子节点的操作
                this->parent = newroot;
                newnode->parent = newroot;
                newroot->children.push_back(this);
                newroot->children.push_back(newnode);
                root = newroot;
                }else{
                newnode->keys.assign(keys.begin()+mid+1,keys.end());
                keys.resize(mid);
                Bpnode* newroot = new Bpnode(keys[mid],false);//构造新的根节点
                this->parent = newroot;
                newnode->parent = newroot;
                newroot->children.push_back(this);
                newroot->children.push_back(newnode);
                newnode->children.assign(children.begin()+mid+1,children.end());
                for(Bpnode* child:newnode->children){
                    child->parent = newnode;
                }
                this->children.resize(mid+1);
                root = newroot;
                }
            }else{
                if(isleaf){//有父节点的叶子节点进行分裂
                keys.resize(mid);
                int pos = parent->find_position(mid_value);
                parent->keys.insert(parent->keys.begin()+pos,mid_value);
                this->next = newnode;
                newnode->parent = parent;
                parent->children.insert(parent->children.begin()+pos+1,newnode);
                if(parent->isFull()){
                    parent->splitnode();
                }
                }else{//有父节点的内部节点进行分裂
                    Bpnode* internalnode = new Bpnode(isleaf);
                    internalnode->keys.assign(keys.begin()+mid+1,keys.end());
                    keys.resize(mid);
                    internalnode->parent = parent;
                    int pos = parent->find_position(mid_value);
                    parent->keys.insert(parent->keys.begin()+pos,mid_value);
                    // parent->children.insert(parent->children.begin()+pos,this);//插入左子树
                    parent->children.insert(parent->children.begin()+pos+1,internalnode);
                    internalnode->children.assign(children.begin()+mid+1,children.end());
                    for(Bpnode* child:internalnode->children){
                        child->parent = internalnode;
                    }
                    this->children.resize(mid+1);
                    if(parent->isFull()){
                        parent->splitnode();
                    }               
                }
            }
        }
        bool isFull(){
            // if(keys.size() >= order+1){
            //     return true;
            // }else{
            //     return false;
            // }
            if(isleaf){
                if(keys.size()>=order+1){
                    return true;
                }else{
                    return false;
                }
            }else{
                if(keys.size() >= order){
                    return true;
                }else{
                    return false;
                }
            }
        }
        void insertkey(int key){
            auto it = find(keys.begin(),keys.end(),key);
            if(it != keys.end()){
                cout<<"Key "<<key<<" is duplicated"<<endl;
                return;
            }
            int pos = find_position(key);
            if(isleaf){
                keys.insert(keys.begin()+pos,key);
                if(isFull()){
                    splitnode();
                }
            }else{
                children[pos]->insertkey(key);
            }
        }
};
Bpnode* Bpnode::root = nullptr;
//每一个节点只需要考虑插入值，是叶子节点就插入，不是就将值传入合适的子节点位置，然后进行下一步判断
class Bptree{
    private:
    public:
        Bpnode* root;
        void insert(int key){
            root->insertkey(key);
            if(root->isFull());
            root->splitnode();
        }
        Bptree():root(nullptr){};

};
//保存根节点，实现分裂的功能，传入父节点和要分裂的子节点的位置，然后对子节点进行分裂，分裂后再看分裂的节点类型，如果是内部节点，那么将分裂的节点的子节点进行重新分配，如果是叶子节点，添加两个节点间的指针
void print_tree(Bpnode* root){
    queue<Bpnode*> q;
    q.push(root);
    while(!q.empty()){
        int levelsize = q.size();
        while(levelsize != 0){
            Bpnode* node = q.front();
            q.pop();
            if(!node->isleaf){
                for(Bpnode* n:node->children){
                    q.push(n);
                }
            }
            cout<<"[";
            if(node->keys.size() == 1){
                cout<<node->keys[0]<<"]";
            }else{
                for(int i=0;i<node->keys.size()-1;++i){
                    cout<<node->keys[i]<<",";
                }
                cout<<node->keys.back()<<"]";
            }
            --levelsize;
        }
        cout<<endl;
    }
}
int main(){
    ifstream file("test.txt");
     if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }
    int num_keys;
    file>>num_keys;
    Bptree* tree = new Bptree();
    for(int i=0;i<num_keys;++i){
        int key;
        file>>key;
        if(tree->root == nullptr){
            tree->root = new Bpnode(key,true);
        }else{
            tree->root->insertkey(key);
            if(Bpnode::root != nullptr)
            tree->root = Bpnode::root;
        }
    }
    print_tree(tree->root);
}