#ifndef LBO_UTIL_H
#define LBO_UTIL_H

#include <bitset>

namespace lbo {
    void trimComments(std::string& io_str);

    #define CHAR_SET_NUM 128
    template<typename T>
    void tokenize(std::string const& p_str,
                  char const* p_delims,
                  T& io_tokens) {
        T tokens;
        
        std::bitset<CHAR_SET_NUM> delimset;
        while(*p_delims!=0) {
            unsigned char ascii = *p_delims;
            delimset[ascii] = true;
            p_delims++;
        }

        typedef std::string::const_iterator striter;
        striter beg, end;
        end = p_str.end();

        bool inToken = false;
        for(striter it = p_str.begin(); it!=end; it++) {
            if(delimset[*it]==true) {
                if(inToken==true) {
                    tokens.push(typename T::value_type(beg, it));
                    inToken = false;
                }
            } else if(inToken==false) {
                beg = it;
                inToken = true;
            }
        }
        if(inToken==true) {
            tokens.push(typename T::value_type(beg, p_str.end()));
        }
        io_tokens.swap(tokens);
    }
}

#endif // LBO_UTIL_H
