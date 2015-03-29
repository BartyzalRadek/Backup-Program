/** \file CDir.cpp
 *  @brief Implementation of CDir class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

#include "CDir.h"
#include "HDD.h"
#include <sys/stat.h>//lstat, bash calls
#include <iostream>
#include <sstream> //Convert int to string
using namespace std;

CDir::CDir(const string & name) : CGenericFile(name) {
}

CDir::~CDir(){
}

bool CDir::CreateDirectory(const string & dirName, int rwx) const {
    if (HDD::exists(dirName) == false) {
        if (mkdir(dirName.c_str(), rwx) == 0) {
            return true;
        } else {
            cout << "Could not create directory: '" << dirName << "'" << endl;
            return false;
        }
    } else {
        return true;
    }
}

bool CDir::CreateDirectoryTree() const {
    string path = m_name;
    bool success = false;
    int index = 0;
    vector<string> paths;
    
    //------Directories end with '/', erase it so the splitting works
    if(path.at(path.length() - 1) == '/') path.erase(path.end()-1);

    //------Split path into paths
    while (path.length() > 0) {
        index = path.find_last_of("/");
        path = path.substr(0, index);
        paths.push_back(path + "/");
    }

    for (int i = paths.size() - 1; i >= 0; i--) {
        CreateDirectory(paths[i]) ? success = true : success = false;
    }

    if (success) {
        return true;
    } else {
        cout << "Could not create directory tree to : '" << m_name << "'" << endl;
        return false;
    }
}

void CDir::Print() const {
    cout<<"Directory: '"<<m_name<<"' "<<endl;
}

