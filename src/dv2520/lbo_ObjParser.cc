#include <stdafx.h>
#include <lbo_flags.h>
#include <lbo_ObjParser.h>

#include <tuple>
#include <stdlib.h> // atof, atoi

namespace lbo {
static const std::string g_defPosition    = "v";
static const std::string g_defTexCoord    = "vt";
static const std::string g_defNormal      = "vn";
static const std::string g_defFace        = "f";
static const std::string g_defGroup       = "g";
static const std::string g_defMaterial    = "mtllib";
static const std::string g_defMaterialUse = "usemtl";
static const std::string g_defSmooth      = "s";

ObjParser::ObjParser(std::queue<std::string>& p_tokens, unsigned p_flags) {
    m_tokens = &p_tokens;
    m_flags = p_flags;
}
ObjParser::~ObjParser() {
}

void ObjParser::parse(std::vector<float>& io_vertices,
                      std::vector<unsigned>& io_indices) {
    Symbol s;
    std::string str;
    f3 f3_;
    f2 f2_;
    i3 i3_;
    while(m_tokens->empty()==false) {
        popString(str);
        s = getSymbol(str);
        switch(s) {
        case Symbol_POSITION:
            f3_.f[0] = popFloatingPoint();
            f3_.f[1] = popFloatingPoint();
            f3_.f[2] = popFloatingPoint();
            m_poss.push_back(f3_);
            break;
        case Symbol_TEXCOORD:
            f2_.f[0] = popFloatingPoint();
            f2_.f[1] = popFloatingPoint();
            m_texs.push_back(f2_);
            break;
        case Symbol_NORMAL:
            f3_.f[0] = popFloatingPoint();
            f3_.f[1] = popFloatingPoint();
            f3_.f[2] = popFloatingPoint();
            m_nors.push_back(f3_);
            break;
        case Symbol_FACE:
            for(unsigned i = 0; i<3; i++) {
                i3_.i[0] = i3_.i[1] = i3_.i[2] = -1;
                if(m_flags&Flags_FACE_POS) {
                    i3_.i[0] = popInteger() - 1;
                }
                if(m_flags&Flags_FACE_TEX) {
                    i3_.i[1] = popInteger() - 1;
                }
                if(m_flags&Flags_FACE_NOR) {
                    i3_.i[2] = popInteger() - 1;
                }

                unsigned idx;
                if(m_flags&Flags_GEN_IDX) {
                    std::map<std::tuple<int, int, int>,
                        unsigned>::iterator it;
                    it = m_facs.find(std::make_tuple(i3_.i[0],
                                                     i3_.i[1], i3_.i[2]));
                    bool newVertex = it==m_facs.end();
                    if(newVertex==true) {
                        vertex v;
                        std::memset(&v, 0, sizeof(v));
                        if(i3_.i[0]!=-1) {
                            memcpy(v.pos, m_poss[i3_.i[0]].f,
                                   sizeof(float) * 3);
                        }
                        if(i3_.i[1]!=-1) {
                            memcpy(v.tex, m_texs[i3_.i[1]].f,
                                   sizeof(float) * 2);
                        }
                        if(i3_.i[2]!=-1) {
                            memcpy(v.nor, m_nors[i3_.i[2]].f,
                                   sizeof(float) * 3);
                        }
                        m_vertices.push_back(v);
                        idx = m_vertices.size() - 1;

                        m_facs[std::make_tuple(i3_.i[0], i3_.i[1],
                                               i3_.i[2])] = idx;
                    } else {
                        idx = it->second;
                    }
                } else {
                    idx = i3_.i[0];

                    // Hack:
                    vertex v;
                    std::memset(&v, 0, sizeof(v));
                    v.pos[0] = m_poss.at(idx).f[0];
                    v.pos[1] = m_poss.at(idx).f[1];
                    v.pos[2] = m_poss.at(idx).f[2];
                    m_vertices.push_back(v);
                }
                m_indices.push_back(idx);
            }
            break;
        // Not yet implemented; ignore:
        case Symbol_GROUP:
            popString(str);
            break;
        case Symbol_SMOOTH:
            popString(str);
            break;
        case Symbol_MATERIALUSE:
            popString(str);
            break;
        case Symbol_MATERIAL:
            popString(str);
            break;
        }
    }

    // Writeback parsed indices:
    io_indices.swap(m_indices);

    // Writeback parsed vertices:
    if(m_flags&Flags_POS
            && m_flags&Flags_TEX
            && m_flags&Flags_NOR) {
        io_vertices.insert(io_vertices.end(),
                           &m_vertices.front().pos[0],
                           &m_vertices.back().pos[0] + 8);
    } else {
        for(size_t i = 0; i<m_vertices.size(); i++) {
            vertex* v = &m_vertices.at(i);
            if(m_flags&Flags_POS) {
                io_vertices.insert(io_vertices.end(),
                                   &v->pos[0],
                                   &v->pos[0] + 3);
            }
            if(m_flags&Flags_TEX) {
                io_vertices.insert(io_vertices.end(),
                                   &v->tex[0],
                                   &v->tex[0] + 2);
            }
            if(m_flags&Flags_NOR) {
                io_vertices.insert(io_vertices.end(),
                                   &v->nor[0],
                                   &v->nor[0] + 3);
            }
        }
    }
}

Symbol ObjParser::getSymbol(std::string& p_str) {
    Symbol s = Symbol_INVALID;
    if(p_str.compare(g_defPosition)==0) {
        s = Symbol_POSITION;
    } else  if(p_str.compare(g_defTexCoord)==0) {
        s = Symbol_TEXCOORD;
    } else if(p_str.compare(g_defNormal)==0) {
        s = Symbol_NORMAL;
    } else if(p_str.compare(g_defFace)==0) {
        s = Symbol_FACE;
    } else if(p_str.compare(g_defGroup)==0) {
        s = Symbol_GROUP;
    } else if(p_str.compare(g_defMaterial)==0) {
        s = Symbol_MATERIAL;
    } else if(p_str.compare(g_defMaterialUse)==0) {
        s = Symbol_MATERIALUSE;
    } else if(p_str.compare(g_defSmooth)==0) {
        s = Symbol_SMOOTH;
    }
    return s;
}

void ObjParser::popString(std::string& p_str) {
    p_str.swap(m_tokens->front());
    m_tokens->pop();
}
float ObjParser::popFloatingPoint() {
    std::string str;
    popString(str);
    return (float)atof(str.c_str());
}
int ObjParser::popInteger() {
    std::string str;
    popString(str);
    return atoi(str.c_str());
}
}
