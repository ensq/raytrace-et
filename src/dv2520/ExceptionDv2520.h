#ifndef DV2520_EXCEPTIONDV2520_H
#define DV2520_EXCEPTIONDV2520_H

class ExceptionDv2520 : public std::exception {
  public:
    ExceptionDv2520(std::string p_func) throw();
    virtual ~ExceptionDv2520() throw();
    
    virtual const char* what() const throw();
  protected:
  private:
    std::string m_func;
};

#endif // DV2520_EXCEPTIONDV2520_H
