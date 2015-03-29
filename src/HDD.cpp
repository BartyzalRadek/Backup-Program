/** \file HDD.cpp
 *  @brief Implementation of HDD class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

#include "HDD.h"
#include "CTime.h"
#include <stdio.h>
#include <cstring> //strcmp
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <sstream> //Convert int to string
using namespace std;

void HDD::getStrings(vector<string>& arr, string& line) {
    string word;
    int beg = 0;

    line.erase(line.find_last_of("\n"), line.length() - line.find_last_of("\n"));

    for (int i = 0; i < (int) line.length(); i++) {
        if (line.at(i) == '\n') {
            word = line.substr(beg, i - beg);
            beg = i + 1;
            arr.push_back(word);
        }
    }
    line.erase(0, beg);
    arr.push_back(line);
}

streampos HDD::getFileSize(const string & fileName) {
    struct stat attrib;

    if (lstat(fileName.c_str(), &attrib) == 0) {
        return attrib.st_size;
    }

    cout << endl << "File does not exists: " << fileName << endl;
    return -1;
}

string HDD::getLastModificationTime(const string & fileName) {
    struct stat attrib; // create a file attribute structure
    string lmt;
    CTime c;

    stat(fileName.c_str(), &attrib); // get the attributes of file
    c.LoadStats(&(attrib.st_mtime));
    lmt = c.ToString();

    return lmt;
}

string HDD::getHeader(const string & fileName, int position) {
    string header;
    stringstream ss;
    ss << position;
    string pos = ss.str();

    header.append(fileName).append("\n").append(getLastModificationTime(fileName)).append("\n");

    if (isDirectory(fileName)) {
        header.append("-1").append("\n");
    } else {
        ss.str(std::string());
        ss << getFileSize(fileName);
        string size = ss.str();
        header.append(size).append("\n");
    }

    header.append(pos).append("\n");

    return header;
}

bool HDD::exists(const string & name) {
    struct stat attrib;
    if (lstat(name.c_str(), &attrib) == 0) {
        return true;
    }
    return false;
}

bool HDD::isDirectory(const string & fileName) {
    struct stat st;
    int err;

    err = lstat(fileName.c_str(), &st);
    if (S_ISDIR(st.st_mode) && err == 0) {
        return true;
    } else {
        return false;
    }
}

bool HDD::getFiles(const string & dirn, vector<string>& files) {
    string name = dirn;

    if (name.length() > 1 && name.at(name.length() - 1) == '/') {
        name.erase(name.end() - 1);
    }

    if (!exists(name)) {
        cout << "File/directory: '" << name << "' does not exist." << endl;
        return false;
    }

    //----------If its a single file
    if (!isDirectory(name)) {
        files.push_back(name);
        return true;
    }

    //----------If its a directory
    DIR *dir = NULL;
    struct dirent *drnt = NULL;
    string filename = "";
    vector<string> directories;

    dir = opendir(name.c_str());

    if (!dir) {
        cout << "Can not open directory: " << "'" << name << "'" << endl;
        return false;
    }

    while ((drnt = readdir(dir))) {
        //-----Exclude directories '.' and '..' to prevent endless loop
        if (strcmp(drnt->d_name, ".") != 0 && strcmp(drnt->d_name, "..") != 0) {

            filename.append(name).append("/").append(drnt->d_name);

            if (isDirectory(filename)) {
                filename.append("/");
                directories.push_back(filename);
            }

            //---------Push back both files and directories
            //---------Don't backup system backup files - wrong LMT
            if (filename.at(filename.length() - 1) != '~') {
                files.push_back(filename);
            }

            filename.clear();
        }
    }
    closedir(dir);

    //------Recursively search all directories
    if (directories.size() > 0) {
        for (int i = 0; i < (int) directories.size(); i++) {
            getFiles(directories[i], files);
        }
    }

    return true;
}

