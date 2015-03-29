/** \file UI.h
 *  @brief Header for UI class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef UI_H
#define	UI_H

#include "CBackupFile.h"

/** @brief Handles User Interface operations*/
class UI {
public:
    /**
     * Cross-checks current day with backupDays 
     * @param path Default backup path from configuration file, from getBackupDays()
     * @return true if backup is enabled / false if disabled
     */
    static bool isBackupDay(std::string & path);
    /**
     * Checks configuration file and puts 1/0 into corresponding fields in array backupDays
     * @param configFile Name of a configuration file
     * @param backupDays Array of integers representing days in a week
     * @param path Default backup path from configuration file
     * @return true = success / false = failed
     */
    static bool getBackupDays(const std::string & configFile, int * backupDays, std::string & path);
    /**Prints greetings at the start*/
    static void greetings();
    /**
     * Handles the main User Interface cycle
     * @param a Backup file, cannot be const because of CBackupFile::Recover()
     * @return 0 = Exit / 5 = Remove backup / 6 = Remove backup and exit
     */
    static int whatToDo(CBackupFile & a, bool backup);
    /**
     * Handles User Interface around recovering a single file
     * @param a Header of a file to recover 
     */
    static void recoverSingleEntry(CBackupFile & a);
    /**
     * Prints all entries in a set
     * @param files Vector of instances of CFileInBackup from CBackupFile::m_files
     */
    static void PrintSet(const std::set<CFileInBackup> & files);
};

#endif	/* UI_H */

