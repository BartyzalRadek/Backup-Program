/** \file CBackupFile.h
 *  @brief Header for CBackupFile class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef CBACKUPFILE_H
#define	CBACKUPFILE_H

//------CFileInBackup.h includes CFile.h
#include "CFileInBackup.h"
#include <vector>
#include <set>
#include <map>
#include <fstream>

/** @brief Represents a backup file*/
class CBackupFile : public CFile {
public:
    CBackupFile(const std::string & name);
    virtual ~CBackupFile();

    /**
     * Appends file to the end of backup.
     * @param fileName Name of the file to append
     * @return true = success / false = failed
     */
    bool Append(const std::string & fileName);
    /**
     * Checks if backup file exists, if not, creates a new one.
     * @see LoadFiles()
     * @return true = success / false = failed
     */
    bool CheckFile();
    /**
     * Fills m_files with  headers of files from backup
     * @return true = success / false = End of file, while reading header
     */
    bool LoadFiles();
    /**
     * Finds all versions of fileName in backup.
     * @param fileName Name of the file to search for 
     * @return either empty set or set of versions
     */
    const std::set<CFileInBackup> Find(const std::string & fileName) const;
    /**
     * Backs up all files at path. Except system backup files (ending with "~"), they have a wrong LMT
     * @param path Full path to backup - either file or directory
     * @return true = success / false = failed
     */
    bool FullBackup(const std::string & path);
    /**
     * Recovers most recent versions of all files from backup.
     * @see Recover()
     * @return true = success / false = failed
     */
    bool FullRecovery();
    /**
     * Reads header from inFile and inserts it to sHeader
     * @param pos Beginning of header
     * @param sHeader Reference to sHeader filled with header
     * @return true = success / false = reached the end of file, while reading header
     */
    bool ReadHeader(std::string & sHeader, std::ifstream & inFile) const;
    /**
     * Recovers a single file or a directory. \n
     * Cannot be const because of UpdateHeader() 
     * @see FullRecovery()
     * @see UpdateHeader()
     * @param file Instance of CFileInBackup to be recovered
     * @return true = success / false = failed
     */
    bool Recover(const CFileInBackup & file);
    /**
     * Recovers latest files from a chosen directory
     * @param directory Name of a directory you want to recover files from.
     * @return true = success / false = failed
     */
    bool RecoverDirectory(const std::string & directory);
    /**
     * Updates header of a file in backup, that have been recovered to reflect the new LMT.
     * By doing so, it prevents backing up the same file over and over again after it has been recovered, because
     * when a file is recovered, it's LMT changes but the LMT stored in backup is still the old one, thats why it needs
     * to be updated.
     * Tip: If you LoadFiles() after updating header, the vector of versions would NOT be sorted, therefore the set
     * @param file File entry in backup you want to update 
     * @return true = success / false = cannot open backup file
     */
    bool UpdateHeader(const CFileInBackup & file);
    /**
     * Updates entry in a map of headers m_files
     * @see UpdateHeader
     * @param oldFile File header you want to rewrite
     * @param newFile File header you want to insert
     * @return true if success / false if oldFile isn't in m_files
     */
    bool UpdateMapEntry(const CFileInBackup & oldFile, const CFileInBackup & newFile);
    /**
     * Insert the file into m_files.
     * @param file Header you want to insert to map m_files.
     */
    void InsertToMap(const CFileInBackup & file);
    /**
     * Searches m_files for all files from a directory and pushes their set of versions to a vector
     * @param directory Name of a directory, you want to search
     * @param filesInDirectory vector of all versions of the files in the directory 
     */
    void GetFilesFromDirectory(const std::string & directory, std::vector< std::set<CFileInBackup> > & filesInDirectory) const;
    /** @return m_files */
    const std::map<std::string, std::set<CFileInBackup> > & GetFiles() const;
    /**
     * Compares last modification time of fileName with latest version of that file in backup.
     * @param fileName Name of a file you want to check
     * @param fileVersions Vector of all versions of fileName in backup
     * @return true if fileName is unique version / false if it's already in backup
     */
    bool IsNewVersion(const std::string & fileName, const std::set<CFileInBackup> & fileVersions) const;

    void Print() const;

protected:
    /**Map of headers of files in backup \n
     * Key is name with full path \n
     * Value is a set of versions of the specific file.\n
     * Why set? See UpdateHeader()
     */
    std::map<std::string, std::set<CFileInBackup> > m_files;
};

#endif	/* CBACKUPFILE_H */

