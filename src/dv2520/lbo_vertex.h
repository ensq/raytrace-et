#ifndef LBO_VERTEX_H
#define LBO_VERTEX_H

namespace lbo {
struct f2 {
    float f[2];
};

struct f3 {
    float f[3];
};

struct i3 {
    int i[3];
};

struct vertex {
    float pos[3];
    float tex[2];
    float nor[3];
};
}

#endif // LBO_VERTEX_H

//typedef std::tuple<int, int, int> face;
