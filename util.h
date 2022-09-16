#pragma once

#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;

namespace util {
    // C++ lacks good string processing...
    // https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
    // trim from start (in place)

    static inline void ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(),
            [](unsigned char c){return !std::isspace(c);}));
    }

    // trim from end (in place)
    static inline void rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(),
            [](unsigned char c){return !std::isspace(c);}).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(string &s) {
        ltrim(s);
        rtrim(s);
    }

    // https://stackoverflow.com/questions/20834838/using-tuple-in-unordered-map
    typedef tuple<u64, int, short> key_t;
    struct key_hash : public std::unary_function<key_t, std::size_t> {
        std::size_t operator()(const key_t& k) const {
            return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
        }
    };
     
    struct key_equal : public std::binary_function<key_t, key_t, bool> {
        bool operator()(const key_t& v0, const key_t& v1) const {
            return v0 == v1;
        }
    };
    
}
