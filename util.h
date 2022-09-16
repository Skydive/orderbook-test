
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
                not1(ptr_fun<int, int>(isspace))));
    }

    // trim from end (in place)
    static inline void rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(),
                not1(ptr_fun<int, int>(isspace))).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(string &s) {
        ltrim(s);
        rtrim(s);
    }
}
