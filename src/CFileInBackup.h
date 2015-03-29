/** \file CFileInBackup.h
 *  @brief Header for CFileInBackup class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef CFILEINBACKUP_H
#define	CFILEINBACKUP_H

#include "CFile.h"

/**
 * @brief Represents a file entry in backup*/
class CFileInBackup : public CFile {
public:
    CFileInBackup();
    CFileInBackup(const std::string & header);
    virtual ~CFileInBackup();

    /**@return m_position*/
    int GetPosition() const;
    /**@return m_headerLength*/
    int GetHeaderLength() const;
    /**@return m_lastModTime*/
    std::string GetLMT() const;
    /**Checks if a file in backup is different from a file on hard drive*/
    bool IsDifferent() const;
    /**Checks whether entry represents file or directory */
    bool IsDirectory() const;
    /**Used for sorting - from latest to oldest*/
    bool operator<(const CFileInBackup & file) const;

    void Print() const;

protected:
    int m_position; /**< At what byte starts header of file*/
    int m_headerLength; /**< Length of header*/
    std::string m_lastModTime; /**< Last modification time stored as string for easier comparison*/

};

#endif	/* CFILEINBACKUP_H */

