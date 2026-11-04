#include "bplustree.hpp"
#include <optional>
#include <string>

int find_position(const std::array<Key, MAX_KEYS> &keys, int size, Key key) {
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

split_node(int key, std::string value, int position, int size, std::array<Key, MAX_KEYS> &keys, std::array<std::string, MAX_KEYS> &values){

}

InsertResult insert_node(int key, std::string value, int position, int &size, 
    std::array<Key, MAX_KEYS> & keys, std::array<std::string, MAX_KEYS> &values){
    int pos = find_position(keys, size, key); 
    if (pos < size){
          if(keys[pos] == key){
            values[pos] = value;
            return InsertResult::Updated;
        }
        else{
            return insert_key(key, value, pos, size, keys, values);
        }
    }
    else if(pos == size && size < MAX_KEYS){
        keys[pos] = key;
        values[pos] = value;
        size++;
        return InsertResult::Inserted; 
    }
    else {
        insert_key(key, value, pos, size, keys, values); 
        // this will create a situation where the max_keys, first insert the key and value, then split the node.
        // split_node(key, value, pos, size, keys, values); yet to implement
        return InsertResult::Full;
    }
}

