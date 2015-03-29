/** \file CFile.cpp
 *  @brief Implementation of CFile class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

#include "CFile.h"

CFile::CFile() {
    m_size = -1;
}

CFile::~CFile() {
}

int CFile::GetSize()const {
    return m_size;
}

