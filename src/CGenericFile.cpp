/** \file CGenericFile.cpp
 *  @brief Implementation of CGenericFile class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

#include "CGenericFile.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>

using namespace std;

CGenericFile::CGenericFile() {
    m_name = "";
}

CGenericFile::CGenericFile(const string & name) {
    m_name = name;
}

CGenericFile::~CGenericFile() {
}

string CGenericFile::getName()const {
    return m_name;
}
