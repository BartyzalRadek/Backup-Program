/** \file CFile.h
 *  @brief Header for CFile class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#ifndef CFILE_H
#define	CFILE_H

#include "CGenericFile.h"

/** @brief Represents a file*/
class CFile : public CGenericFile {
public:
    CFile();
    virtual ~CFile();

    /**@return m_size*/
    int GetSize() const;

    virtual void Print() const = 0;

protected:
    int m_size; /**< Size of a file*/

};

#endif	/* CFILE_H */

