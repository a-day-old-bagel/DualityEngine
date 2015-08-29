#include "HashMap.h"

using namespace DualityEngine;

template<class K, class V>
HashMap<K,V>::HashMap(){
    
}

template<class K, class V>
HashMap<K,V>::~HashMap(){
    
}

template<class K, class V>
V& HashMap<K,V>::at(const K& key){
    return internalMap.at(key);
}

template<class K, class V>
void HashMap<K,V>::clear() noexcept{
    internalMap.clear();
}

template<class K, class V>
bool HashMap<K,V>::contains(const K& key) const{
    return (bool)internalMap.count(key);
}

template<class K, class V>
template <class... Args>
bool HashMap<K,V>::emplace ( Args&&... args ){
    return internalMap.emplace(std::forward<Args>(args)...).second;
}

template<class K, class V>
bool HashMap<K,V>::erase(const K& key){
    return (bool)internalMap.erase(key);
}

template<class K, class V>
void HashMap<K,V>::reserve(std::size_t n){
    internalMap.reserve(n);
}

template<class K, class V>
typename HashMap<K,V>::const_iterator HashMap<K,V>::begin() const {
    return internalMap.begin();
}

template<class K, class V>
typename HashMap<K,V>::const_iterator HashMap<K,V>::end() const {
    return internalMap.end();
}