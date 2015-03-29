/** \file HDD.h
 *  @brief Header for HDD class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef HDD_H
#define	HDD_H

#include <vector>
#include "CFileInBackup.h"

/** @brief Handles operations with files on hard drive*/
class HDD {
public:
    /**
     * Checks if file exists on a hard drive
     * @param name Name of a file with a full path
     */
    static bool exists(const std::string & name);
    /**
     * Splits string according to '\\n' into arr
     * @param arr Reference to vector with substrings
     * @param line Whole line to split
     */
    static void getStrings(std::vector<std::string>& arr, std::string& line);
    /**
     * Returns size of a file on hard drive
     * @param fileName Name of a file
     * @return Size of a file on hard drive
     */
    static std::streampos getFileSize(const std::string & fileName);
    /**
     * Gets last modification time
     * @param fileName Name of a file
     * @return Last modification time of a file on hard drive
     */
    static std::string getLastModificationTime(const std::string & fileName);
    /**
     * Creates header of a fileName with information from hard drive
     * Format is Name/LMT/Size/position
     * @param fileName Name of a file
     * @param position Position in backup file
     * @return Header based on information from hard drive and given position
     */
    static std::string getHeader(const std::string & fileName, int position);
    /**
     * Finds all files and directories in dirn and inserts them to vector files with their full path
     * Except system backup files (ending with "~"), they have a wrong LMT
     * @param dirn Path to a directory
     * @param files Reference to a vector with file paths
     * @return true = success / false = dirn doesn't exist
     */
    static bool getFiles(const std::string & dirn, std::vector<std::string>& files);
    /**
     * Checks if a fileName is a directory
     * @param fileName
     * @return true if it's a directory
     */
    static bool isDirectory(const std::string & fileName);

};

#endif	/* HDD_H */

