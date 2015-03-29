/** \file CGenericFile.h
 *  @brief Header for CGenericFile class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

/** \mainpage
 *  This is a simple backup program.
 * \section sect1 Main Description
 * The program checks a current day and if backup is enabled backs up everything in a directory specified in a configuration file.
 * \n Then it offers you a variety of options, such as recovering a single file or a directory.
 * 
 * \attention Before starting the program: 
 * \n Specify which days you want to backup and what do you want to backup in configuration file. 
 * 
 * \section sect2 Features
 * \subsection step1 Exit
 * Exit the program, all changes you made to backup will be saved.
 * \subsection step2 Full Backup
 * Backs up everything in a given path to a backup file. \n
 * Doesn't backup system backup files (ending with "~"), they have a different LMT, \n
 * so it wouldn't make sense to recover them ergo backup them.
 * \subsection step3 Full Recovery
 * Recovers latest versions of all files from backup.
 * \subsection step4 Recover a single entry
 * You choose either a file or a directory from backup and recover latest version of it.
 * \subsection step5 Delete backup file
 * Simply removes the backup file and asks you to enter a new one.\n
 * Especially useful when you mistype a backup file name.
 * \subsection step6 Delete backup file and exit
 * Remove backup file and exit the program.
 */

#ifndef CGENERICFILE_H
#define	CGENERICFILE_H

#include <string>

/** @brief Represents a generic file*/
class CGenericFile {
public:
    CGenericFile();
    CGenericFile(const std::string & name);
    virtual ~CGenericFile();

    /**@return m_name*/
    std::string getName() const;

    virtual void Print() const = 0;

protected:
    /**Name of the file*/
    std::string m_name;
};


#endif	/* CGENERICFILE_H */

