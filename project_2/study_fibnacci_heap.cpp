#include <iostream>
#include <cmath>
#include <vector>
#include <climits>  // For INT_MAX
using namespace std;

class FibonacciHeapNode {
public:
    int key;
    int degree;
    bool mark;
    FibonacciHeapNode *parent;
    FibonacciHeapNode *child;
    FibonacciHeapNode *left;
    FibonacciHeapNode *right;

    FibonacciHeapNode(int value) {
        key = value;
        degree = 0;
        mark = false;
        parent = nullptr;
        child = nullptr;
        left = this;
        right = this;
    }
};
class FibonacciHeap {
private:
    FibonacciHeapNode *minNode;  // 指向最小节点的指针
    int numOfNodes;              // 堆中节点的数量

public:
    FibonacciHeap() {
        minNode = nullptr;
        numOfNodes = 0;
    }

    // 插入新节点
    void insert(int key) {
        FibonacciHeapNode *node = new FibonacciHeapNode(key);
        if (minNode == nullptr) {
            minNode = node;
        } else {
            // 将新节点插入到根表
            minNode->left->right = node;
            node->left = minNode->left;
            node->right = minNode;
            minNode->left = node;

            // 更新最小节点
            if (key < minNode->key) {
                minNode = node;
            }
        }
        numOfNodes++;
    }

    // 返回最小节点
    FibonacciHeapNode* findMin() {
        return minNode;
    }

    // 合并两个斐波那契堆
    void merge(FibonacciHeap &other) {
        if (other.minNode == nullptr) return;

        if (minNode == nullptr) {
            minNode = other.minNode;
        } else {
            // 合并两个根表
            minNode->right->left = other.minNode->left;
            other.minNode->left->right = minNode->right;
            minNode->right = other.minNode;
            other.minNode->left = minNode;

            // 更新最小节点
            if (other.minNode->key < minNode->key) {
                minNode = other.minNode;
            }
        }
        numOfNodes += other.numOfNodes;
    }

    // 提取最小值
    FibonacciHeapNode* extractMin() {
        FibonacciHeapNode *oldMin = minNode;
        if (oldMin != nullptr) {
            // 将 min 的每个子节点添加到根表
            if (oldMin->child != nullptr) {
                FibonacciHeapNode *child = oldMin->child;
                do {
                    FibonacciHeapNode *next = child->right;
                    child->right = child->left = child;
                    child->parent = nullptr;
                    insertNodeIntoRootList(child);
                    child = next;
                } while (child != oldMin->child);
            }

            // 从根表移除最小节点
            removeNodeFromRootList(oldMin);

            // 合并同阶的树（consolidate）
            consolidate();

            numOfNodes--;
            if (numOfNodes == 0) {
                minNode = nullptr;
            } else {
                minNode = findNewMin();
            }
        }
        return oldMin;
    }

    // 减少键值
    void decreaseKey(FibonacciHeapNode *node, int newKey) {
        if (newKey > node->key) {
            cerr << "New key is greater than current key!" << endl;
            return;
        }
        node->key = newKey;
        FibonacciHeapNode *parent = node->parent;
        if (parent != nullptr && node->key < parent->key) {
            cut(node, parent);
            cascadingCut(parent);
        }
        if (node->key < minNode->key) {
            minNode = node;
        }
    }

    // 删除操作
    void deleteNode(FibonacciHeapNode *node) {
        decreaseKey(node, INT_MIN);  // 将节点的键值减少到负无穷
        extractMin();                // 删除最小节点
    }

private:
    // 将节点插入根表
    void insertNodeIntoRootList(FibonacciHeapNode *node) {
        node->left = minNode->left;
        node->right = minNode;
        minNode->left->right = node;
        minNode->left = node;
    }

    // 从根表移除节点
    void removeNodeFromRootList(FibonacciHeapNode *node) {
        node->left->right = node->right;
        node->right->left = node->left;
    }

    // 合并同阶树
    void consolidate() {
        const int maxDegree = static_cast<int>(log2(numOfNodes)) + 1;
        vector<FibonacciHeapNode*> degreeTable(maxDegree, nullptr);

        vector<FibonacciHeapNode*> rootNodes;
        FibonacciHeapNode *current = minNode;
        do {
            rootNodes.push_back(current);
            current = current->right;
        } while (current != minNode);

        for (FibonacciHeapNode *node : rootNodes) {
            int d = node->degree;
            while (degreeTable[d] != nullptr) {
                FibonacciHeapNode *other = degreeTable[d];
                if (node->key > other->key) swap(node, other);
                link(other, node);
                degreeTable[d] = nullptr;
                d++;
            }
            degreeTable[d] = node;
        }

        minNode = nullptr;
        for (FibonacciHeapNode *node : degreeTable) {
            if (node != nullptr) {
                if (minNode == nullptr) {
                    minNode = node;
                    minNode->left = minNode->right = minNode;
                } else {
                    insertNodeIntoRootList(node);
                    if (node->key < minNode->key) {
                        minNode = node;
                    }
                }
            }
        }
    }

    // 将节点other链接到node作为其子节点
    void link(FibonacciHeapNode *other, FibonacciHeapNode *node) {
        removeNodeFromRootList(other);
        other->parent = node;
        if (node->child == nullptr) {
            node->child = other;
            other->left = other->right = other;
        } else {
            other->left = node->child->left;
            other->right = node->child;
            node->child->left->right = other;
            node->child->left = other;
        }
        node->degree++;
        other->mark = false;
    }

    // 切断子节点并将其添加到根表
    void cut(FibonacciHeapNode *child, FibonacciHeapNode *parent) {
        if (child->right == child) {
            parent->child = nullptr;
        } else {
            child->right->left = child->left;
            child->left->right = child->right;
            if (parent->child == child) {
                parent->child = child->right;
            }
        }
        parent->degree--;
        insertNodeIntoRootList(child);
        child->parent = nullptr;
        child->mark = false;
    }

    // 级联切割
    void cascadingCut(FibonacciHeapNode *node) {
        FibonacciHeapNode *parent = node->parent;
        if (parent != nullptr) {
            if (!node->mark) {
                node->mark = true;
            } else {
                cut(node, parent);
                cascadingCut(parent);
            }
        }
    }

    // 找到新的最小节点
    FibonacciHeapNode* findNewMin() {
        FibonacciHeapNode *current = minNode->right;
        FibonacciHeapNode *min = minNode;
        do {
            if (current->key < min->key) {
                min = current;
            }
            current = current->right;
        } while (current != minNode);
        return min;
    }
};