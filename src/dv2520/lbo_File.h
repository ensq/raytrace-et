#ifndef LBO_FILE_H
#define LBO_FILE_H

namespace lbo {
enum FileStatus {
    FileStatus_NA,
    FileStatus_EOF,
    FileStatus_BAD,
    FileStatus_FAIL
};

class File {
  public:
    File(const char* p_name);
    ~File();

    bool read();
    char* getContents() const;
    size_t getLength() const;
    FileStatus getStatus() const;
  protected:
  private:
    const char* m_name;
    size_t m_length;
    char* m_contents;
    FileStatus m_status;
};
}

#endif // LBO_FILE_H
