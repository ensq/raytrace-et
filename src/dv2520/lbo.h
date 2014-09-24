// Linebacker Obj (lbo)
// Wavefront Obj parser

#ifndef LBO_LBO_H
#define LBO_LBO_H

#include <vector>

namespace lbo {
enum UserFlags {
    UserFlags_FACE_NOR = 0x02, // Faces dictate Normal indices.
    UserFlags_FACE_TEX = 0x04, // Faces specify Texture Coordinate indices.
    UserFlags_TEX      = 0x08, // Want Texture Coordinates
    // returned in vertex-list.
    UserFlags_NOR      = 0x10, // Want Normals returned in vertex-list.
    UserFlags_GEN_IDX  = 0x20
};

void configure(unsigned p_flags);

bool parse(char* p_filename,
           std::vector<float>& io_vertices,
           std::vector<unsigned>& io_indices);
}

#endif // LBO_LBO_H
