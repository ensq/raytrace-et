#include <stdafx.h>
#include <lbo_File.h>

#include <fstream>

namespace lbo {
    File::File(const char* p_name) : m_name(p_name) {
        m_length = 0;
        m_contents = nullptr;
        m_status = FileStatus_NA;
    }
    File::~File() {
        if(m_contents!=nullptr) {
            free(m_contents);
        }
    }

    bool File::read() {
        bool readSuccess = true;

        std::ifstream strm;
        strm.open(m_name, std::ifstream::in);
        if(strm.is_open()==true) {
            strm.seekg(0, std::ios::end);
            m_length = (size_t)strm.tellg();
            strm.seekg(0);
            m_contents = (char*)malloc(m_length + 1);
            std::memset(m_contents, 0, m_length + 1);
            strm.read(m_contents, m_length);
            strm.close();
        } else {
            if(strm.eof()==true) {
                m_status = FileStatus_EOF;
            } else if(strm.bad()==true) {
                m_status = FileStatus_BAD;
            } else if(strm.fail()==true) {
                m_status = FileStatus_FAIL;
            }
            readSuccess = false;
        }

        return readSuccess;
    }
    char* File::getContents() const {
        return m_contents;
    }
    size_t File::getLength() const {
        return m_length;
    }
    FileStatus File::getStatus() const {
        return m_status;
    }
}
