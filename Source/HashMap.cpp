#include "../Headers/HashMap.h"

using namespace DualityEngine;

HashMap::HashMap(){
    
}

HashMap::~HashMap(){
    
}

V& HashMap::at(const K& key){
    return internalMap.at(key);
}

void HashMap::clear(){
    internalMap.clear();
}

bool HashMap::contains(const K& key) const{
    return (bool)internalMap.count(key);
}

template <class... Args>
bool HashMap::emplace ( Args&&... args ){
    return internalMap.emplace(args...).second;
}

bool HashMap::erase(const K& key){
    return (bool)internalMap.erase(key);
}

void HashMap::reserve(std::size_t n){
    internalMap.reserve(n);
}