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

namespace DualityEngine{
    
    template<class K, class V>
    class HashMap{
        private:
            
            std::unordered_map<K, V> internalMap;
        
        public:
            
            HashMap();
            ~HashMap();
            V& at ( const K& key );
            void clear() noexcept;
            bool contains ( const K& key ) const;
                template <class... Args>
            bool emplace ( Args&&... args );
            bool erase ( const K& key );
            void reserve ( std::size_t n );                
            
    };
    
}

#endif	/* HASHMAP_H */

