#include <stdafx.h>

#include <lbo.h>
#include <lbo_util.h>
#include <lbo_File.h>
#include <lbo_ObjParser.h>

#include <queue>
#include <iostream>

namespace lbo {
    bool parse(char* p_filename,
               std::vector<float>& io_vertices,
               std::vector<unsigned>& io_indices) {
        // Retrieve file contents:
        lbo::File f(p_filename);
        if(f.read()==false) {
            return false;
        }
        std::string str(f.getContents());

        // Trim contents of unecessary data:
        lbo::trimComments(str);

        // Tokenize contents once:
        std::queue<std::string> tokens;
        lbo::tokenize(str, "\n /", tokens);

        lbo::ObjParser parser(tokens);
        parser.parse(io_vertices, io_indices);

        //std::cin.get();

        return true;
    }
}
