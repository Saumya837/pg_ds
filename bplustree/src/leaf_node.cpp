#include "bplustree.hpp"
#include <optional>
#include <string>

int find_position(const std::array<Key, MAX_KEYS> &keys,
    int size, Key key) {
    int pos = 0;
    while (pos < size && keys[pos] < key){
        pos++;
    }
    return pos;
}

InsertResult insert_key(int key, std::string value, int position, int &size, 
    std::array<Key, MAX_KEYS> &keys, std::array<std::string, MAX_KEYS> &values){
    for (int i = size; i > position; i--){
        keys[i] = keys[i - 1];
        values[i] = values[i - 1];
    }
    keys[position] = key;
    values[position] = value;
    size++;
    return InsertResult::Inserted; 
}

LeafNode* split_node(LeafNode* node, int key, std::string value, int position){
    int mid = node->size >> 1;
    LeafNode* new_node = new LeafNode();

    int idx1 = position > mid ? mid + 1 : mid;
    new_node->size = node->size - idx1;
    for(int i = 0; i < new_node->size; i++){
        new_node->keys[i] = node->keys[idx1 + i];
        new_node->values[i] = node->values[idx1 + i];
    }
    node->size = idx1;
    if(position > mid){
        insert_key(key, value, position - idx1, new_node->size, new_node->keys, new_node->values);
    }
    else{
        insert_key(key, value, position, node->size, node->keys, node->values);
    }

    new_node->next = node -> next;
    node->next = new_node;
    return new_node;
}

LeafInsert insert_node(LeafNode* node, Key key, const Value& value){
    int pos = find_position(node->keys, node->size, key);
 
    // 1. key exists -> overwrite (only look at live slots)
    if (pos < node->size && node->keys[pos] == key){
        node->values[pos] = value;
        return {InsertResult::Updated};
    }
 
    // 2. full -> split
    if (node->size == MAX_KEYS){
        LeafNode* new_node = split_node(node, key, value, pos);
        return {InsertResult::Split, new_node->keys[0], new_node};
    }
 
    // 3. room -> plain insert (also covers pos == size)
    insert_key(key, value, pos, node->size, node->keys, node->values);
    return {InsertResult::Inserted};
}