/* 
 * File:   HashMap.h
 * Author: adayoldbagel
 *
 * Created on July 2, 2015, 11:29 AM
 * 
 * Wrapper class for whatever implementation of a hash map I wish to use.
 * Right now, I'm using std::unordered_map from C++ standard library.
 */

#ifndef HASHMAP_H
#define	HASHMAP_H

#include <unordered_map>

namespace DualityEngine {
    
    template<class K, class V>
    class KvMap {
        private:
            
            std::unordered_map<K, V> internalMap;
        
        public:
            
            KvMap();
            ~KvMap();
            V& at ( const K& key );
            void clear() noexcept;
            bool contains ( const K& key ) const;
                template <class... Args>
            bool emplace ( Args&&... args );
            bool erase ( const K& key );
            void reserve ( std::size_t n );
            
                typedef typename std::unordered_map<K, V>::const_iterator const_iterator;
            const_iterator begin() const;// { return internalMap.begin(); }
            const_iterator end() const;// { return internalMap.end(); }
            
    };



    template<class K, class V>
    KvMap<K, V>::KvMap() {

    }

    template<class K, class V>
    KvMap<K, V>::~KvMap() {

    }

    template<class K, class V>
    V& KvMap<K, V>::at(const K &key) {
        return internalMap.at(key);
    }

    template<class K, class V>
    void KvMap<K, V>::clear() noexcept {
        internalMap.clear();
    }

    template<class K, class V>
    bool KvMap<K, V>::contains(const K& key) const {
        return (bool) internalMap.count(key);
    }

    template<class K, class V>
    template<class... Args>
    bool KvMap<K, V>::emplace(Args &&... args) {
        return internalMap.emplace(std::forward<Args>(args)...).second;
    }

    template<class K, class V>
    bool KvMap<K, V>::erase(const K &key) {
        return (bool) internalMap.erase(key);
    }

    template<class K, class V>
    void KvMap<K, V>::reserve(std::size_t n) {
        internalMap.reserve(n);
    }

    template<class K, class V>
    typename KvMap<K, V>::const_iterator KvMap<K, V>::begin() const {
        return internalMap.begin();
    }

    template<class K, class V>
    typename KvMap<K, V>::const_iterator KvMap<K, V>::end() const {
        return internalMap.end();
    }
     
}

#endif	/* HASHMAP_H */

