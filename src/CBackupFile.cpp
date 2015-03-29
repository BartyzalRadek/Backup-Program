/** \file CBackupFile.cpp
 *  @brief Implementation of CBackupFile class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#include "HDD.h"
#include "CDir.h"
//-----UI.h includes CBackupFile.h
#include "UI.h"
#include <iostream>
#include <cstring> //strlen

using namespace std;

CBackupFile::CBackupFile(const string& name) {
    m_name = name;
    m_size = -1;
}

CBackupFile::~CBackupFile() {
}

bool CBackupFile::Append(const string& fileName) {
    cout << "Appending: '" << fileName << "' ";
    ofstream outFile(m_name.c_str(), ofstream::out | ofstream::app | ofstream::binary);
    ifstream inFile(fileName.c_str(), ios::in | ios::binary);

    int pos = HDD::getFileSize(m_name);
    string header = HDD::getHeader(fileName, pos);

    if (!outFile.is_open() || !inFile.is_open()) {
        cout << endl << "Could not open: '" << m_name << "' or '" << fileName << "'" << endl;
        return false;
    }

    outFile << header;

    if (!HDD::isDirectory(fileName)) {
        outFile << inFile.rdbuf();
    }

    CFileInBackup file(header);
    InsertToMap(file);

    inFile.close();
    outFile.close();
    cout << "- Success." << endl;
    m_size = HDD::getFileSize(m_name);
    return true;
}

bool CBackupFile::CheckFile() {
    if (HDD::exists(m_name)) {
        cout << "Backup file '" << m_name << "' exists." << endl;
    } else {
        cout << "Backup file '" << m_name << "' does not exists. Creating a new one." << endl;
        ofstream file(m_name.c_str());
    }

    ifstream inFile(m_name.c_str(), ifstream::binary | ifstream::ate | ifstream::in);
    if (inFile.is_open()) {
        if (inFile.tellg() == -1) {
            m_size = 0;
        } else {
            m_size = inFile.tellg();
        }
        inFile.close();
        if (!LoadFiles()) {
            cout << "Could not load files from backup." << endl;
            return false;
        }
        return true;
    }

    cout << "Could not create '" << m_name << "' " << endl;
    return false;
}

bool CBackupFile::LoadFiles() {
    if (m_size < 4) return true;

    ifstream inFile(m_name.c_str(), ifstream::in | ifstream::binary);
    if (!inFile.is_open()) {
        cout << endl << "Could not read from: '" << m_name << "' " << endl;
        return false;
    }

    while (true) {
        //---------Read header
        string sHeader;
        if (!ReadHeader(sHeader, inFile)) {
            inFile.close();
            cout << endl << "Could not get files from backup." << endl;
            return false;
        }
        CFileInBackup file(sHeader);

        InsertToMap(file);

        //---------Skip file data if it's not a directory
        if (!file.IsDirectory()) {
            inFile.seekg((int) inFile.tellg() + file.GetSize());
            if (inFile.tellg() == m_size) {
                inFile.close();
                return true;
            }
        }
    }
}

const map<string, set<CFileInBackup> > & CBackupFile::GetFiles() const {
    return m_files;
}

const set<CFileInBackup> CBackupFile::Find(const string & fileName)const {
    if (m_size < 4) {
        cout << "Backup file is empty." << endl;
        return set<CFileInBackup>();
    }

    map<string, set<CFileInBackup> >::const_iterator it = m_files.find(fileName);
    if (it == m_files.end()) {
        cout << "Could not find file: '" << fileName << "' in backup." << endl;
        return set<CFileInBackup>();
    }

    return it->second;
}

bool CBackupFile::FullBackup(const string & path) {
    vector<string> files;
    bool suc = true;

    cout << endl << "Full Backup:" << endl;
    if (!HDD::getFiles(path, files)) {
        cout << "Could not get files from: '" << path << "'" << endl;
        return false;
    }

    if (m_size < 4) {
        cout << "Backup file is empty - appending all." << endl;
        for (int i = 0; i < (int) files.size(); i++) {
            if (Append(files[i]) == false) suc = false;
        }
    } else {
        for (int i = 0; i < (int) files.size(); i++) {
            //-----Get all version of a file in backup
            set<CFileInBackup> fileVersions = Find(files[i]);
            if (fileVersions.size() != 0) {
                //-------If it's a directory, don't append it again
                if (HDD::isDirectory(files[i])) {
                    cout << "Directory: '" << files[i] << "' is already backed up." << endl;
                    continue;
                }
                //-------If it's new version, append it
                if (IsNewVersion(files[i], fileVersions)) {
                    if (Append(files[i]) == false) suc = false;
                } else {
                    cout << "File: '" << files[i] << "' is up to date, no need to append it." << endl;
                }
            } else {
                //-------file wasn't found in backup -> append it
                if (Append(files[i]) == false) suc = false;
            }
        }
    }

    if (suc) {
        cout << "Full backup successfully finished." << endl;
        return true;
    } else {
        cout << "Full backup failed" << endl;
        return false;
    }
}

bool CBackupFile::UpdateHeader(const CFileInBackup & file) {
    int position = file.GetPosition();
    string header = HDD::getHeader(file.getName(), position);
    ofstream outFile(m_name.c_str(), ofstream::out | ofstream::in | ofstream::binary);

    if (!outFile.is_open()) {
        cout << endl << "Could not open: '" << m_name << "'" << endl;
        return false;
    }

    if (position != 0) {
        outFile.seekp(position);
    } else {
        outFile.seekp(outFile.beg);
    }

    //-----I can just rewrite the old header because the only thing that has changed is LMT which is always the same size
    outFile << header;

    outFile.close();
    UpdateMapEntry(file, CFileInBackup(header));

    return true;
}

bool CBackupFile::UpdateMapEntry(const CFileInBackup & oldFile, const CFileInBackup & newFile) {
    map<string, set<CFileInBackup> >::iterator it;
    it = m_files.find(oldFile.getName());
    if (it == m_files.end()) {
        return false;
    } else {
        set<CFileInBackup>::iterator sit;

        //-----Find the same version of a file as oldFile and replace it with newFile 
        for (sit = it->second.begin(); sit != it->second.end(); ++sit) {
            if ((*sit).GetLMT() == oldFile.GetLMT()) {
                it->second.erase(sit);
                it->second.insert(newFile);
            }
        }
    }

    return true;
}

void CBackupFile::InsertToMap(const CFileInBackup & file) {
    map<string, set<CFileInBackup> >::iterator it;
    it = m_files.find(file.getName());

    //--------If key exists, insert the file into its set, otherwise insert new set with one entry
    if (it == m_files.end()) {
        set<CFileInBackup> versions;
        versions.insert(file);
        m_files.insert(pair<string, set<CFileInBackup> >(file.getName(), versions));
    } else {
        it->second.insert(file);
    }
}

bool CBackupFile::ReadHeader(string & sHeader, ifstream & inFile)const {
    int size = 0;
    int delimCount = 0;
    char delimiter;
    int pos = inFile.tellg();

    //----------Get to the end of header = get size of header
    while (delimCount != 4) {
        inFile.get(delimiter);
        if (inFile.tellg() == m_size) {
            inFile.close();
            return false;
        }
        if (delimiter == '\n') {
            delimCount++;
        }
    }
    //---------Inititalize *header
    size = (int) inFile.tellg() - pos;
    char *header = new char[size + 1];
    header[size] = '\0';
    //---------Jump to beginning of header
    if (pos != 0) {
        inFile.seekg(pos);
    } else {
        inFile.seekg(inFile.beg);
    }
    //---------Fill *header with header
    inFile.read(header, size);
    sHeader = string(header, header + strlen(header));
    delete [] header;
    return true;
}

bool CBackupFile::Recover(const CFileInBackup & file) {
    string fileName = file.getName();
    CDir dir(fileName);
    int copySize;

    if (!file.IsDifferent()) {
        file.IsDirectory() ? cout << "Directory: '" : cout << "File: '";
        cout << file.getName() << "' is up to date, no need to recover." << endl;
        return true;
    }

    if (!dir.CreateDirectoryTree()) {
        cout << endl << "Could not recover directory tree to: '" << fileName << "'" << endl;
        return false;
    }

    //------It's a directory
    if (file.IsDirectory()) {
        cout << "Recovering directory: '" << fileName << "' ";
        dir.CreateDirectory(fileName);
        cout << " - Success." << endl;
        return true;
    }

    //-------It's a file
    cout << "Recovering: '" << fileName << "' ";

    ifstream inFile(m_name.c_str(), ifstream::in | ifstream::binary);
    //-------Read data of file to recover
    copySize = file.GetSize();
    inFile.seekg(file.GetPosition() + file.GetHeaderLength());
    char* toCopy = new char[copySize + 1];
    toCopy[copySize] = '\0';
    inFile.read(toCopy, copySize);
    inFile.close();
    //--------Create directory tree
    if (!dir.CreateDirectoryTree()) {
        cout << endl << "Could not recover directory tree to: '" << fileName << "'" << endl;
        return false;
    }
    //--------Create file
    ofstream outFile(fileName.c_str(), ofstream::out | ofstream::binary);
    //--------Fill it with data toCopy
    if (!outFile.is_open()) {
        cout << endl << "Could not write into: '" << fileName << "' " << endl;
        return false;
    }
    outFile.write(toCopy, copySize);
    outFile.close();

    delete [] toCopy;
    UpdateHeader(file);

    cout << " - Success." << endl;
    return true;
}

bool CBackupFile::RecoverDirectory(const std::string & directory) {
    bool suc = true;
    set<CFileInBackup>::iterator sit;
    vector< set<CFileInBackup> > filesInDirectory;
    GetFilesFromDirectory(directory, filesInDirectory);

    cout << "Recovering latest files from directory: '" << directory << "'" << endl;

    //--------Recover latest files
    for (int i = 0; i < (int) filesInDirectory.size(); i++) {
        sit = filesInDirectory[i].begin();
        if (!Recover((*sit))) suc = false;
    }

    if (suc) {
        cout << "Recovering successful." << endl;
        return true;
    } else {
        cout << "Recovering failed." << endl;
        return false;
    }
}

bool CBackupFile::FullRecovery() {
    bool suc = true;

    if (m_size < 4) {
        cout << "Nothing to recover." << endl;
        return true;
    }

    cout << endl << "Full Recovery: " << endl;

    for (map<string, set<CFileInBackup> >::const_iterator it = m_files.begin(); it != m_files.end(); ++it) {
        //-----Recovering latest version
        if (!Recover((*(it->second.begin())))) {
            cout << "Recovering failed at: '" << it->first << "' " << endl;
            suc = false;
            break;
        }
    }

    if (suc) {
        cout << "Full recovery successful." << endl;
        return true;
    } else {
        cout << "Full recovery failed." << endl;
        return false;
    }
}

void CBackupFile::GetFilesFromDirectory(const string & directory, vector< set<CFileInBackup> > & filesInDirectory) const {
    for (map<string, set<CFileInBackup> >::const_iterator it = m_files.begin(); it != m_files.end(); ++it) {
        if (it->first.find(directory) != string::npos) {
            filesInDirectory.push_back(it->second);
        }
    }
}

void CBackupFile::Print() const {
    cout << "Entries in backup: " << endl;
    int i = 0;

    for (map<string, set<CFileInBackup> >::const_iterator it = m_files.begin(); it != m_files.end(); ++it) {
        cout << "[" << i << "] '";
        cout << it->first << "' " << endl;
        i++;
    }

    cout << endl;
}

bool CBackupFile::IsNewVersion(const string & fileName, const set<CFileInBackup> & fileVersions) const {
    if ((*fileVersions.begin()).IsDifferent()) {
        return true;
    } else {
        return false;
    }
}