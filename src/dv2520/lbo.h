// Linebacker Obj (lbo)
// Wavefront Obj parser

#ifndef LBO_LBO_H
#define LBO_LBO_H

#include <vector>

namespace lbo {
    bool parse(char* p_filename,
               std::vector<float>& io_vertices,
               std::vector<unsigned>& io_indices);
}

#endif // LBO_LBO_H
