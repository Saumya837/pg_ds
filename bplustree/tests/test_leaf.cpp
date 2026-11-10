#include "bplustree.hpp"
#include <cassert>
#include <iostream>
#include <vector>

static LeafNode* make_leaf(std::vector<Key> ks){
    LeafNode* n = new LeafNode();
    for (Key k : ks) insert_node(n, k, "v" + std::to_string(k));
    return n;
}

static std::vector<Key> keys_of(LeafNode* n){
    return std::vector<Key>(n->keys.begin(), n->keys.begin() + n->size);
}

static void split_case(Key k, std::vector<Key> left, std::vector<Key> right){
    LeafNode* n = make_leaf({10, 20, 30, 40, 50});
    LeafInsert r = insert_node(n, k, "new");
    assert(r.result == InsertResult::Split);
    assert(keys_of(n) == left);
    assert(keys_of(r.new_leaf) == right);
    assert(r.separator == right[0]);
    assert(n->next == r.new_leaf);
}

int main(){
    // basic inserts
    LeafNode* a = make_leaf({30, 10, 20});
    assert((keys_of(a) == std::vector<Key>{10, 20, 30}));
    assert(insert_node(a, 20, "z").result == InsertResult::Updated);
    assert(a->size == 3 && a->values[1] == "z");

    // splits
    split_case(60, {10, 20, 30}, {40, 50, 60});
    split_case(45, {10, 20, 30}, {40, 45, 50});
    split_case(15, {10, 15, 20}, {30, 40, 50});
    split_case(25, {10, 20, 25}, {30, 40, 50});
    split_case(35, {10, 20, 30}, {35, 40, 50});
    split_case(5,  {5, 10, 20},  {30, 40, 50});

    // chain survives a split
    LeafNode* left = make_leaf({10, 20, 30, 40, 50});
    LeafNode* far  = make_leaf({70, 80, 90});
    left->next = far;
    LeafInsert r = insert_node(left, 60, "x");
    assert(left->next == r.new_leaf && r.new_leaf->next == far);

    // overwrite in a full leaf: no split
    LeafNode* full = make_leaf({10, 20, 30, 40, 50});
    assert(insert_node(full, 30, "q").result == InsertResult::Updated);
    assert(full->size == 5 && full->next == nullptr);

    std::cout << "all leaf tests passed\n";
}