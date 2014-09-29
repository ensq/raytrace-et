#include <stdafx.h>

#include <Et.h>

Et::Et() : m_interactorId("dv2520") {
    m_context = TX_EMPTY_HANDLE;
    m_interactorSnapshot = TX_EMPTY_HANDLE;
}
Et::~Et() {
}

bool Et::init() {
    return true;
}
