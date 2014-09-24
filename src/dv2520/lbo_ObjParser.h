#ifndef LBO_OBJFIER_H
#define LBO_OBJFIER_H

#include <map>
#include <queue>
#include <vector>

#include <lbo_vertex.h>

namespace lbo {
enum Symbol {
    Symbol_POSITION,    // v
    Symbol_TEXCOORD,    // vt
    Symbol_NORMAL,      // vn
    Symbol_FACE,        // f
    Symbol_GROUP,       // g
    Symbol_MATERIAL,    // mtllib
    Symbol_MATERIALUSE, // usemtl
    Symbol_SMOOTH,      // s
    Symbol_INVALID
};

class ObjParser {
  public:
    ObjParser(std::queue<std::string>& p_tokens, unsigned p_flags);
    ~ObjParser();

    void parse(std::vector<float>& io_vertices,
               std::vector<unsigned>& io_indices);
  protected:
  private:
    Symbol getSymbol(std::string& p_str);

    void popString(std::string& p_str);
    float popFloatingPoint();
    int popInteger();

    std::queue<std::string>* m_tokens;
    unsigned m_flags;

    std::vector<f3> m_poss;
    std::vector<f2> m_texs;
    std::vector<f3> m_nors;

    std::map<std::tuple<int, int, int>, unsigned> m_facs;
    std::vector<vertex> m_vertices;
    std::vector<unsigned> m_indices;
};
}

#endif // LBO_OBJFIER_H
