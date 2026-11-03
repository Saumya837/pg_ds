#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>

constexpr int MAX_KEYS = 5;              // max keys per node
constexpr int MIN_KEYS = (MAX_KEYS + 1) / 2;  // fill floor for non-root nodes (used in delete)

using Key = int64_t;
using Value = std::string;

enum class InsertResult {Inserted, Updated, Full};

// Common header so the tree can hold either kind behind one pointer.
struct Node {
    bool is_leaf;
    int size = 0;                               // keys currently in use
    explicit Node(bool leaf) : is_leaf(leaf) {}
    virtual ~Node() noexcept = default;
};

// // k keys -> k + 1 children.
// // Rule: everything in children[i] < keys[i] <= everything in children[i + 1]
struct InternalNode : Node {
    std::array<Key, MAX_KEYS> keys{};
    std::array<Key, MAX_KEYS + 1> children{};
    InternalNode() : Node(false) {}
};

struct LeafNode : Node {
    std::array<Key,  MAX_KEYS> keys{};
    std::array<Value, MAX_KEYS> values{};   // values[i] belongs to keys[i]
    LeafNode* next = nullptr;               // right sibling, for range scans=

    LeafNode() : Node(true) {}
    ~LeafNode() noexcept override = default;

    // Day 1: you implement these.
    InsertResult         insert_sorted(Key key, const Value& value);
    std::optional<Value> find(Key key) const;
    bool                 check() const;     // keys strictly increasing, size <= MAX_KEYS
};

int find_pos(const std::array<Key, MAX_KEYS>& keys, int size, Key key);