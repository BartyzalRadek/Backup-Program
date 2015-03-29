/** \file CTime.h
 *  @brief Header for CTime class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef CTIME_H
#define	CTIME_H

#include <time.h>
#include <string>

/** @brief Represents a time structure*/
class CTime {
public:
    CTime();
    virtual ~CTime();

    /**Loads current time into m_time     */
    void LoadCurrentTime();
    /**
     * Loads time structure into m_time
     * @param stats Information about a file
     */
    void LoadStats(const time_t *stats);
    /** @return [0-6] 0 = Sunday*/
    int GetDayOfWeek() const;
    /**@return Time  in format: YYYY/MM/DD - HH/MM/SS*/
    std::string ToString() const;

private:
    struct tm* m_time;

};

#endif	/* CTIME_H */

