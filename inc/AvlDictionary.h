#ifndef AVL_DICTIONARY_H
#define AVL_DICTIONARY_H

#include <algorithm>
#include <stdexcept>

template<class K, class V>
class AvlDictionary {
    AvlDictionary<K, V>* left;
    AvlDictionary<K, V>* right;
    K key;
    V value;
    int balanceFactor = 0;
    int height = 0;
    bool nil = true;
    void rotateLeft();
    void rotateRight();
    void balance();
    void retrace();
public:
    AvlDictionary<K, V>() {}
    AvlDictionary(AvlDictionary<K, V>&) = delete;
    AvlDictionary<K, V>& operator=(AvlDictionary<K, V>&) = delete;
    void insert(K key, V value);
    V& search(const K& key);
    V deleteKey(const K& key);
    const AvlDictionary<K, V>& getLeft() const;
    const AvlDictionary<K, V>& getRight() const;
    const K& getKey() const;
    const V& getValue() const;
    bool isNil() const;
    ~AvlDictionary();
};

template<class K, class V>
void AvlDictionary<K, V>::insert(K key, V value) {
    if(nil) {
        left = new AvlDictionary<K, V>();
        right = new AvlDictionary<K, V>();
        this->key = key;
        this->value = value;
        height = 1;
        nil = false;
        return;
    }

    if(key == this->key) {
        throw std::runtime_error("Attempt to insert existing key");
    }

    (key < this->key ? left : right)->insert(key, value);

    balanceFactor = right->height - left->height;
    height = std::max(left->height, right->height) + 1;

    balance();
}

template<class K, class V>
void AvlDictionary<K, V>::balance() {
    if(balanceFactor >= -1 && balanceFactor <= 1) return;

    if(balanceFactor < 0) {
        if(left->balanceFactor > 0) left->rotateLeft();
        rotateRight();
    } else {
        if(right->balanceFactor < 0) right->rotateRight();
        rotateLeft();
    }
}

template<class K, class V>
void AvlDictionary<K, V>::retrace() {
    if(nil) return;
    left->retrace();
    balanceFactor = right->height - left->height;
    height = std::max(left->height, right->height) + 1;
}

template<class K, class V>
void AvlDictionary<K, V>::rotateLeft() {
    auto right = this->right;

    auto key = this->key;
    auto value = this->value;
    this->key = right->key;
    this->value = right->value;
    right->key = key;
    right->value = value;

    this->right = right->right;
    right->right = right->left;
    right->left = this->left;
    this->left = right;

    right->balanceFactor = right->right->height - right->left->height;
    right->height = std::max(right->left->height, right->right->height) + 1;
    this->balanceFactor = this->right->height - this->left->height;
    this->height = std::max(this->left->height, this->right->height) + 1;
}

template<class K, class V>
void AvlDictionary<K, V>::rotateRight() {
    auto left = this->left;

    auto key = this->key;
    auto value = this->value;
    this->key = left->key;
    this->value = left->value;
    left->key = key;
    left->value = value;

    this->left = left->left;
    left->left = left->right;
    left->right = this->right;
    this->right = left;

    left->balanceFactor = left->right->height - left->left->height;
    left->height = std::max(left->left->height, left->right->height);
    this->balanceFactor = this->right->height - this->left->height;
    this->height = std::max(this->left->height, this->right->height);
}

template<class K, class V>
V &AvlDictionary<K, V>::search(const K &key) {
    if(nil) throw std::runtime_error("No such key exists");
    if(key == this->key) return value;
    return (key < this->key ? left : right)->search(key);
}

template<class K, class V>
V AvlDictionary<K, V>::deleteKey(const K &key) {
    if(nil) throw std::runtime_error("No such key exists");

    auto value = this->value;

    if(key < this->key) {
        value = left->deleteKey(key);

        balanceFactor = right->height - left->height;
        height = std::max(left->height, right->height) + 1;
    } else if(key > this->key) {
        value = right->deleteKey(key);

        balanceFactor = right->height - left->height;
        height = std::max(left->height, right->height) + 1;
    } else if(left->nil && right->nil) {
        delete left;
        delete right;
        balanceFactor = 0;
        height = 0;
        nil = true;
    } else if(left->nil || right->nil) {
        AvlDictionary<K, V>* child;

        if(left->nil) {
            delete left;
            child = right;
        } else {
            child = left;
            delete right;
        }

        left = child->left;
        right = child->right;
        this->key = child->key;
        this->value = child->value;
        balanceFactor = child->balanceFactor;
        height = child->height;

        child->nil = true;
        delete child;
    } else {
        auto successor = right;
        while(!successor->left->nil) successor = successor->left;

        this->key = successor->key;
        this->value = successor->value;

        auto tmp = successor->right;
        delete successor->left;
        successor->left = tmp->left;
        successor->right = tmp->right;
        successor->key = tmp->key;
        successor->value = tmp->value;
        successor->height = tmp->height;
        successor->balanceFactor = tmp->balanceFactor;
        successor->nil = tmp->nil;
        tmp->nil = true;
        delete tmp;

        right->retrace();

        balanceFactor = right->height - left->height;
        height = std::max(left->height, right->height) + 1;
    }

    balance();
    return value;
}

template<class K, class V>
const K &AvlDictionary<K, V>::getKey() const {
    if(nil) throw std::runtime_error("This node has no key");
    return key;
}

template<class K, class V>
const V &AvlDictionary<K, V>::getValue() const {
    if(nil) throw std::runtime_error("This node has no value");
    return value;
}

template<class K, class V>
const AvlDictionary<K, V> &AvlDictionary<K, V>::getLeft() const {
    if(nil) throw std::runtime_error("This node has no children");
    return *left;
}

template<class K, class V>
const AvlDictionary<K, V> &AvlDictionary<K, V>::getRight() const {
    if(nil) throw std::runtime_error("This node has no children");
    return *right;
}

template<class K, class V>
bool AvlDictionary<K, V>::isNil() const {
    return nil;
}

template<class K, class V>
AvlDictionary<K, V>::~AvlDictionary() {
    if(nil) return;
    delete left;
    delete right;
}

#endif
