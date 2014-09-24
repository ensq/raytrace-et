#include <stdafx.h>

#include <lbo.h>
#include <lbo_util.h>
#include <lbo_File.h>
#include <lbo_flags.h>
#include <lbo_ObjParser.h>

#include <queue>
#include <iostream>

namespace lbo {
// Default configuration flag settings:
unsigned g_flags = Flags_FACE_POS // Always positive.
                   | Flags_FACE_NOR
                   | Flags_FACE_TEX
                   | Flags_POS // Always positive.
                   | Flags_NOR
                   | Flags_TEX
                   | Flags_GEN_IDX;

void configure(unsigned p_flags) {
    g_flags = 0;
    g_flags = Flags_FACE_POS | Flags_POS;

    if(p_flags&UserFlags_FACE_NOR) {
        g_flags |= Flags_FACE_NOR;
    }
    if(p_flags&UserFlags_FACE_TEX) {
        g_flags |= Flags_FACE_TEX;
    }
    if(p_flags&UserFlags_NOR) {
        g_flags |= Flags_NOR;
    }
    if(p_flags&UserFlags_TEX) {
        g_flags |= Flags_TEX;
    }
    if(p_flags&UserFlags_GEN_IDX) {
        g_flags |= Flags_GEN_IDX;
    }
}

bool parse(char* p_filename,
           std::vector<float>& io_vertices,
           std::vector<unsigned>& io_indices) {
    assert(g_flags&Flags_FACE_POS);
    assert(g_flags&Flags_POS);

    // Retrieve file contents:
    lbo::File f(p_filename);
    if(f.read()==false) {
        return false;
    }
    std::string str(f.getContents());

    // Trim contents of unecessary data:
    // This function is ridiculously slow for larger files:
    //lbo::trimComments(str);

    // Tokenize contents once:
    std::queue<std::string> tokens;
    lbo::tokenize(str, "\n /", tokens);

    lbo::ObjParser parser(tokens, g_flags);
    parser.parse(io_vertices, io_indices);

    return true;
}
}
