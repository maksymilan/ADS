#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;
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
    virtual void push(pair<int,int>)=0;//将一个元素合并到堆内,不同的堆push方式是最大的差异
    bool empty();
};
class binary_heap:public heaps{
    public:
    pair<int,int> pop();
    void push(pair<int,int>);
    void perculate_up(int index);//将堆尾元素进行上浮
    void perculate_down(int index);//将堆顶元素进行下沉
};

class fibonacci_node{
    friend class fibonacci_heap;//声明友类，这样fibonacci_heap就可以直接访问fibonacci_node的私有变量
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
// class fibnacci_heap:public heaps{
//     //斐波那契堆是由松弛的树组成的森林，每一棵树都满足堆序性
//     //树的合并操作，两棵树如果度相等，那么对比堆顶元素，堆顶元素小的作为新的堆顶，然后将另一个堆的堆顶最为孩子添加到这棵树中，这棵树的度加一，
//     //斐波那契堆的操作，插入操作直接在根表中添加一个节点，提取堆顶元素的操作是将先将堆顶元素的所有孩子节点添加到根表中，对根表中的树进行合并，然后返回堆顶元素
// };
class fibonacci_heap:public heaps{
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
        bool empty();
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

typedef pair<int,int> edge;//first表示目标点，second表示这两个点对间的距离
class node{
    private:
        int from_;
        vector<edge> adj_nodes;
    public:
        node(){};
        node(int from):from_(from){}
        bool operator==(node& other)const{
            return from_ == other.get_from();
        }
        int get_from(){
            return from_;
        }//得到该节点表示的是哪一个节点数字
        vector<edge>& get_adj_nodes(){
            return adj_nodes;
        }//返回该节点的邻接节点的表的引用
        void add_adj(node& other,int weight){
                adj_nodes.push_back(make_pair(other.get_from(),weight));
        }//向该节点添加一个新的节点
};
class graph{
    private:
    int V;//图中目前储存的节点数量
    // vector<node> adj;//数组储存节点，每一个节点内部有一个数组储存到不同节点的距离值，便于在迪杰斯特拉算法过程中将节点加入到堆当中进行排序
    map<int,node> adj;//用图来储存，第一个元素是这个节点的值，第二个元素储存该节点对象
    public:
    graph(){
        V = 0;
    }//初始化空图
    map<int,node>& get_adj_table(){
        return adj;
    }
    int get_vertexes(){
        return adj.size();
    }//返回该图中节点的数量
    node& get_node(int V){
        return adj[V];
    }
    void add_edge(node& from,node& to,int weight){
    //这里可以用map储存adj进行优化，这样每次查询都是常数时间
        if(adj.count(from.get_from())){
            adj[from.get_from()].add_adj(to,weight);
        }
        else{
            from.add_adj(to,weight);
            adj.insert(make_pair(from.get_from(),from));
        }
    }//向图中添加一条边

    void print_graph(){
        for(auto& pair:adj){
            vector<edge> adj_nodes = pair.second.get_adj_nodes();
            cout<<pair.first<<":";
            for(auto& e:adj_nodes){
                cout<<e.first<<"("<<e.second<<")"<<" ";
            }
            cout<<endl;
        }
        cout<<"total vertexes is:"<<V<<endl;
    }
    int dijkstra(node& src,node& dest);//对图中的两个节点通过迪杰斯特拉算法找到最短路径
};
int get_a_random_number(int limit){
    random_device rd;//生成种子
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1,limit);
    int random_number1 = dis(gen);
    return random_number1;
}

//加载文件
bool loadGraphFromFile(const string& filename, graph& g){
    ifstream file(filename);
    if(!file.is_open()){
        cout << "Error opening file" << endl;
        return false;
    }
    string line;
    while(getline(file,line)){
        istringstream iss(line);
        char ch;
        int from,to,weight;
        iss >> ch;  
        iss>>from>>to>>weight;
        node src(from);
        node dest(to);
        g.add_edge(src,dest,weight); 
    }
    file.close();
    return true;
}

int main(){
    graph g;
    cout<<"start loading file"<<endl;
    auto begin = high_resolution_clock::now();
    if(loadGraphFromFile("USA-road-d.USA.gr",g)){//读取文件
        //USA-road-d.NY.gr
        // g.print_graph();
        cout<<"total vertexes is:"<<g.get_vertexes()<<endl;
    }else{
        cout<<"Can't open file! Programme exit"<<endl;
        return 0;
    }//图的读取文件初始化
    auto stop = high_resolution_clock::now();
    auto dura = duration_cast<milliseconds>(stop - begin);
    cout<<"graph init finished, time taken :"<<dura.count()<<"ms"<<endl;//计算图的初始化时间
    // 下面是开始对图中随机一个节点到另一个目标节点的最短距离进行迪杰斯特拉算法
    ofstream o_file("binary_heap_result_USA.txt");//输入文件
    vector<int> res;
    int i;
    for(i = 0;i<10;++i){
        int src_index = get_a_random_number(g.get_vertexes());
        int dest_index = get_a_random_number(g.get_vertexes());
        while(dest_index == src_index){
            dest_index = get_a_random_number(g.get_vertexes());
        }//生成两个随机数，这两个随机数分别代表dijikstra算法的起点和终点
        // cout<<"The random number is "<<src_index<<" "<<dest_index<<endl;
        auto start = high_resolution_clock::now();//开始时间戳
        int ans = g.dijkstra(g.get_adj_table()[src_index],g.get_adj_table()[dest_index]);//dijkstra算法，传入两个节点，在图当中计算这两个结点的最短路径
        auto end = high_resolution_clock::now();//结束时间戳
        auto duration = duration_cast<milliseconds>(end - start);//计算算法时间
        int d = static_cast<int>(duration.count());
        res.push_back(d);
        o_file<<"Time: "<<d<<"ms"<<endl;//将得到的结果输入文件
        // cout<<"The shortest distance from "<<g.get_adj_table()[src_index].get_from()<<" to "<<g.get_adj_table()[dest_index].get_from()<<" is "<<ans<<endl;
        // cout<<"Time taken by dijkstra function:"<<duration.count()<<"ms"<<endl;
        // o_file<<"The shortest distance from "<<g.get_adj_table()[src_index].get_from()<<" to "<<g.get_adj_table()[dest_index].get_from()<<" is "<<ans<<endl;
        // o_file<<"order: "<<i<<" "<<"Time: "<<duration.count()<<"ms"<<endl;//将得到的结果输入文件
    }//进行一千次循环
    cout<<"programme executed "<<i<<" times successfuly"<<endl;
    return 0;
}
int graph::dijkstra(node& src,node& dest){
    unordered_map<int,int> dist;
    for(auto& n:adj){
        dist[n.first] = INT_MAX;
    }//对map的元素进行遍历初始化
    unordered_map<int,bool> visited;
    for(auto& n:adj){
        visited[n.first] = false;
    }//对map的元素进行遍历初始化
    binary_heap h;//未实现的堆数据结构
    dist[src.get_from()] = 0;
    h.push(make_pair(0,src.get_from()));
    while(!h.empty()){//如果堆为空但是目标节点还未访问到，说明这两个节点之间没有通路
        pair<int,int> top = h.pop();
        int u = top.second;
        if(visited[u]){ 
            continue;
        }//如果节点u已经被访问过，那么节点就已经找到了最短路径，跳过后续步骤
        if(u == dest.get_from()){
            return top.first;
        }//如果访问到的节点和目标节点一样，那么就已经找到了我们需要的最短路径，不需要再进行后续计算
        visited[u] = true;
        vector<edge>& adj_nodes = get_node(u).get_adj_nodes();
        for(auto e:adj_nodes){
            int v = e.first;
            if(visited[v]){
                continue;
            }//表示该点的最短路径已经输出过了，这一个后输出的值不是最短路径，直接跳过
            int weight = e.second;
            if(dist[v] > dist[u] + weight){
                dist[v] = dist[u] + weight;
                h.push(make_pair(dist[v],v));//将能够优化的值输入到堆中
            }
            
        }
    }
    return dist[dest.get_from()];
}
//目前要做的就是实现堆排序，我们要对节点进行排序，不仅要传入节点的距离值，也要传入节点本身

vector<pair<int,int>>& heaps::get_vec(){
    return h;
}

int heaps::get_size(){
    return h.size();
}

bool heaps::empty(){
    return h.size() == 0;
}

pair<int,int> binary_heap::pop(){
    vector<pair<int,int>>& vec = get_vec();
    pair<int,int> top = vec.front();
    vec[0] = vec[vec.size()-1];//将堆尾赋值到堆顶
    vec.erase(vec.end());//消除堆尾
    perculate_down(0);//将堆顶元素进行下沉
    return top;
}

void binary_heap::push(pair<int,int> p){
    vector<pair<int,int>>& vec = get_vec();
    vec.push_back(p);
    int index = vec.size() - 1;
    perculate_up(index);
}

void binary_heap::perculate_down(int index){
    int size = get_size();
    vector<pair<int,int>>& vec = get_vec();
    while(index < size){
        int left = 2*index + 1;
        int right = 2*index + 2;
        int min = index;
        if(left < size && vec[left].first < vec[min].first){
            min = left;
        }
        if(right < size && vec[right].first < vec[min].first){
            min = right;
        }//明确只有孩子节点的值小于父节点才会进行交换，如果是相等的，那么不会进行交换
        if(min != index){
            swap(vec[min],vec[index]);
            index = min;
        }else{
            break;
        }
    }
}

void binary_heap::perculate_up(int index){
    vector<pair<int,int>>& vec = get_vec();
    while(index > 0){//index等于0的时候跳出，此时index已经成为了根节点了
        int parent = (index - 1)/2;
        if(vec[parent].first > vec[index].first){
            swap(vec[parent],vec[index]);
            index = parent;
        }else{
            break;
        }
    }
}

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
    // node->left_ = node->right_ = node;//这一句不能写,如果写了,那么在删除最小节点的时候原来的堆数据就彻底丢失了
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
        removefrom(min);//这里是将min_node的左右节点连接起来，但是min_node的左右指针指向还没有变
        num_nodes_--;
        if(num_nodes_ == 0){
            min_node = nullptr;
        }else{
            min_node = min_node->right_;
        }
        consolidate();
    }
    return min;
}

void fibonacci_heap::consolidate(){
    //先将所有根表中的节点储存起来，然后将每一个节点输入degree_v并储存，一个一个地进行合并，最后degree_v中储存的就是合并后的斐波那契堆，再通过degree_v重新建立根表
    fibonacci_node* current = min_node;
    if(min_node == nullptr)
    return;
    max_degree = static_cast<int>(log2(num_nodes_))+1;
    vector<fibonacci_node*> degree_v(max_degree,nullptr);
    vector<fibonacci_node*> root_nodes;
    do{
        root_nodes.push_back(current);
        current = current->right_;
    }while(current != min_node);
    for(auto& root:root_nodes){
        removefrom(root);
        root->left_ = root->right_ = root;
    }
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
        node->parent_ = root;
        return;
    }else{
        node->left_ = child->left_;
        child->left_->right_ = node;
        child->left_ = node;
        node->right_ = child; 
    }
}
bool fibonacci_heap::empty(){
    return num_nodes_ == 0;
}
