#include <stdafx.h>
#include <lbo_util.h>

#include <vector>

namespace lbo {
#define CHAR_CMNT '#'
#define CHAR_ENDL '\n'
    void trimComments(std::string& io_str) {
        std::vector<size_t> begs; // Should be an ordered map.
        std::vector<size_t> ends; // -||-

        size_t idxCmnt = io_str.find(CHAR_CMNT, 0);
        while(idxCmnt!=std::string::npos) {
            size_t idxEndl = io_str.find(CHAR_ENDL, idxCmnt);
            if(idxEndl==std::string::npos) {
                idxEndl = io_str.back();
            }
            begs.push_back(idxCmnt);
            ends.push_back(idxEndl);

            idxCmnt = io_str.find(CHAR_CMNT, idxEndl);
        }

        size_t len = 0;
        for(unsigned i = 0; i<begs.size(); i++) {
            size_t beg = begs.at(i) - len;
            size_t end = ends.at(i) - len;
            io_str.erase(beg, end - beg);
            len += end - beg;
        }
    }
}
