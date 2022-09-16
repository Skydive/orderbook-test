
// C++ lacks good string processing...

// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;

// trim from start (in place)
namespace util {
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
}
