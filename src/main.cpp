/** \file main.cpp
 *  @brief Main file
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */

//-----CDir.h includes CGenericFile.h
#include "CDir.h"
//-----HDD.h includes CFileInBackup.h
#include "HDD.h" 
//-----UI.h includes CBackupFile.h
#include "UI.h"
#include <iostream>
using namespace std;

int main() {
    bool backupDay = false;
    int backupFile = -1;
    string name;
    string path;

    UI::greetings();
    backupDay = UI::isBackupDay(path);

    while (backupFile != 0 && backupFile != 5) {
        cout << "Write name of a backup file: " << endl;
        getline(cin, name);

        CBackupFile a(name);
        a.CheckFile();

        if (backupDay) {
            a.FullBackup(path);
        }

        backupFile = UI::whatToDo(a, backupDay);
    }

    cout << "Exiting now..." << endl;

    return 0;
}
