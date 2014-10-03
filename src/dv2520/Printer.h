#ifndef DV2520_PRINTER_H
#define DV2520_PRINTER_H

class Printer {
 public:
    Printer(std::string p_filename);
    ~Printer();

    void printFrame(float p_msRaysGenerate, float p_msRaysIntersect,
                    float p_msLighting);
 protected:
 private:
    std::ofstream m_file;
};

#endif // DV2520_PRINTER_H
