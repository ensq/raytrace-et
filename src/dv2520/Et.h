#ifndef DV2520_ET_H
#define DV2520_ET_H

#define TOBII_TX_DETAIL
#include <eyex\EyeX.h>

#pragma comment (lib, "Tobii.EyeX.Client.lib")

class Et {
 public:
    Et();
    ~Et();

    bool init();
 protected:
 private:
    const TX_STRING m_interactorId;
};

#endif // DV2520_ET_H
