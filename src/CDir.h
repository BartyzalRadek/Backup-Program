/** \file CDir.h
 *  @brief Header for CDir class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef CDIR_H
#define	CDIR_H

#include "CGenericFile.h"

/**
 * \def DEF_DIR_MODE
 * Default directory mode = 0755 =  rwx/r-x/r-x. 
 */
#define DEF_DIR_MODE 0755   

/** @brief Represents a directory*/
class CDir : public CGenericFile {
public:

    CDir(const std::string & name);
    virtual ~CDir();

    /** 
     * Creates a directory.
     * @see CreateDirectoryTree()
     * @param dirName Name of the directory with a full path
     * @param rwx Rights to the directory, default is 0755
     * @return True if directory exists or if it was successfully created
     */
    bool CreateDirectory(const std::string & dirName, int rwx = DEF_DIR_MODE) const;
    /** 
     * Creates a directory tree.
     * @see CreateDirectory()
     * @return True if directory tree exists or if it was successfully created
     */
    bool CreateDirectoryTree() const;

    void Print() const;

protected:
};

#endif	/* CDIR_H */

