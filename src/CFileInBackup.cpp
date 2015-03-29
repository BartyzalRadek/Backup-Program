/** \file CFileInBackup.cpp
 *  @brief Implementation of CFileInBackup class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

//------HDD.h includes CFileInBackup.h which includes CFile.h
#include "HDD.h"
#include <sstream> //Convert int to string
#include <iostream>
using namespace std;

CFileInBackup::CFileInBackup(const string& header) {
    string x = header;
    vector<string> arr;
    int num;

    HDD::getStrings(arr, x);

    m_name = arr[0];
    m_lastModTime = arr[1];

    istringstream is1(arr[2]);
    is1 >> num;
    m_size = num;
    istringstream is2(arr[3]);
    is2 >> num;
    m_position = num;

    m_headerLength = header.length();
}

CFileInBackup::CFileInBackup() {
    m_name = "";
    m_lastModTime = "";
    m_size = -1;
    m_position = -1;
    m_headerLength = -1;
}

CFileInBackup::~CFileInBackup() {
}

int CFileInBackup::GetPosition()const {
    return m_position;
}

int CFileInBackup::GetHeaderLength()const {
    return m_headerLength;
}

string CFileInBackup::GetLMT() const {
    return m_lastModTime;
}

void CFileInBackup::Print() const {
    cout << "'" << m_name << "' LMT: " << m_lastModTime << endl;
}

bool CFileInBackup::IsDifferent() const {
    if (m_lastModTime.compare(HDD::getLastModificationTime(m_name)) != 0) {
        return true;
    }
    return false;
}

bool CFileInBackup::IsDirectory() const {
    if (m_size == -1) {
        return true;
    } else {
        return false;
    }
}

bool CFileInBackup::operator<(const CFileInBackup & file) const {
    if (m_lastModTime.compare(file.GetLMT()) > 0) {
        return true;
    } else {
        return false;
    }
}
