// The content of this file was part of the assignment. It is intended for testing purposes.

#include <cassert>
#include <iostream>
#include <functional>
#include <string>

#include "gc.h"

using namespace std;

class Node : public gc_object {
public:
    int val;
    Node* left{ nullptr };
    Node* right{ nullptr };

    Node(int val) : val(val) {}

    ~Node() {
        cout << "Erased:" << val << endl;
    }

protected:
    void get_ptrs(function<void(gc_object*)> callback) override {
        callback(left);
        callback(right);
    }
};

void test1() {
    {
        gc_root_ptr<Node> treeRoot = new Node(15);
        treeRoot->left = new Node(260);
        treeRoot->right = new Node(-8);
        treeRoot->right = new Node(0);

        // Expected output: "Erased: -8"
        gc::collect();

        cout << endl;
    }

    // Expected output: "Erased: 15", "Erased: 260", "Erased: 0"
    gc::collect();

    cout << endl;
}

void test2() {
    gc_root_ptr<Node> root = new Node(5);

    Node* node = new Node(*root);
    root->val = 18;

    // Expected output: "Erased: 5"
    gc::collect();

    node = new Node(3);
    *node = *root;
    root->val = 9;

    // Expected output: "Erased: 18"
    gc::collect();

    node = new Node(move(*root));
    root.reset();

    // Expected output: "Erased: 9", "Erased: 9"
    gc::collect();

    node = new Node(2);
    Node* node2 = new Node(3);
    *node2 = move(*node);

    // Expected output: "Erased: 2", "Erased: 2"
    gc::collect();
}

int main(int argc, char** argv) {
    test1();
    test2();
}