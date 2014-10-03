#include <stdafx.h>

#include <Printer.h>

Printer::Printer(std::string p_filename) {
    m_file = std::ofstream(p_filename);
    if(m_file.is_open()==false) {
        throw ExceptionDv2520("Could not open file " + p_filename + "!\n");
    }
}
Printer::~Printer() {
    assert(m_file.is_open());
    m_file.close();
}

void Printer::printFrame(float p_msRaysGenerate, float p_msRaysIntersect,
                         float p_msLighting) {
#define SEP " "
    m_file << p_msRaysGenerate << " " << p_msRaysIntersect << " "
           << p_msLighting << "\n";
}
