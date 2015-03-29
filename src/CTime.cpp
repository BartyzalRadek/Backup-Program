/** \file CTime.cpp
 *  @brief Implementation of CTime class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#include "CTime.h"
#include <sstream>

using namespace std;

CTime::CTime() {
}

CTime::~CTime() {
}

void CTime::LoadCurrentTime() {
    time_t rawtime;

    time(&rawtime); //get current time
    m_time = localtime(&rawtime);
}

void CTime::LoadStats(const time_t * stats) {
    m_time = localtime(stats);
}

int CTime::GetDayOfWeek() const {
    return m_time->tm_wday;
}

string CTime::ToString() const {
    string time;
    stringstream ss;

    ss << (m_time->tm_year + 1900) << "/";
    if (m_time->tm_mon + 1 < 10) ss << 0;
    ss << m_time->tm_mon + 1 << "/";
    if (m_time->tm_mday < 10) ss << 0;
    ss << m_time->tm_mday << " - ";
    if (m_time->tm_hour < 10) ss << 0;
    ss << m_time->tm_hour << ":";
    if (m_time->tm_min < 10) ss << 0;
    ss << m_time->tm_min << ":";
    if (m_time->tm_sec < 10) ss << 0;
    ss << m_time->tm_sec;

    time = ss.str();
    return time;
}



