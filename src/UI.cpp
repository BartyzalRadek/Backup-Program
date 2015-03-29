/** \file UI.cpp
 *  @brief Implementation of UI class.
 *  \author Radek Bartyzal <bartyrad@fit.cvut.cz> */


#include "UI.h"
#include "CTime.h"
#include <iostream>
#include <fstream>

using namespace std;

/**
 * \def CONFIG_FILE
 * Name of a configuration file
 */
#define CONFIG_FILE "./config.cfg"  

bool UI::getBackupDays(const string & configFile, int * backupDays, string & path) {
    ifstream inFile(configFile.c_str(), ifstream::in);
    char c = '#';
    int line = 0;

    if (!inFile.is_open()) {
        cout << "Could not find config file." << endl;
        return false;
    }

    while (!inFile.eof()) {
        if (c == '=') {
            if (line == 7) {
                getline(inFile, path);
                break;
            } else {
                inFile.get(c);
                if (c == 'Y') backupDays[line] = 1;
                if (c == 'N') backupDays[line] = 0;
            }

            line++;
        } else {
            inFile.get(c);
        }
    }

    inFile.close();
    return true;
}

bool UI::isBackupDay(string & path) {
    static const string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    int backupDays[7] = {0};
    int day;
    CTime time;

    time.LoadCurrentTime();
    day = time.GetDayOfWeek(); //SUNDAY = 0

    cout << "Today is: " << days[day] << " - " << time.ToString() << endl;

    if (!UI::getBackupDays(CONFIG_FILE, backupDays, path)) {
        cout << "Unable to determine whether to backup." << endl;
        return false;
    }

    cout << "Path to backup: '" << path << "'" << endl;

    if (backupDays[day] == 1) {
        cout << "Backing up is enabled." << endl << endl;
        return true;
    } else {
        cout << "Backing up is disabled." << endl << endl;
        return false;
    }
}

void UI::greetings() {
    cout << "\tWELCOME TO BACKUP PROGRAM " << endl;
    cout << "\t  Version: 1.1" << endl;
    cout << "\t  Author: Radek Bartyzal" << endl << endl;
}

int UI::whatToDo(CBackupFile & a, bool backup) {
    int choice = -1;
    string path;

    while (true) {
        cout << endl << "Choose [0-5]:" << endl;
        cout << "[0] Exit" << endl << "[1] Full Backup" << endl << "[2] Full Recovery" << endl << "[3] Recover a single entry" << endl;
        cout << "[4] Remove backup file" << endl << "[5] Remove backup file and exit" << endl;
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 0:
                return 0;
            case 1:
                if (backup) {
                    cout << "Write full path to location you want to backup:" << endl;
                    getline(std::cin, path);
                    a.FullBackup(path);
                } else {
                    cout << "Backing up is disabled today." << endl;
                }
                break;
            case 2:
                a.FullRecovery();
                break;
            case 3:
                recoverSingleEntry(a);
                break;
            case 4:
                //------Do exactly the same as case 5
            case 5:
                cout << "Removing backup file: '" << a.getName() << "'" << endl << endl;
                remove(a.getName().c_str());
                break;
        }

        if (choice == 0 || choice == 4 || choice == 5) {
            return choice;
        }
    }
}

void UI::recoverSingleEntry(CBackupFile & a) {
    int fileName = -1;
    int fileVersion = -1;

    if (a.GetFiles().size() != 0) {
        a.Print();
        fileName = -1;

        while (fileName < 0 || fileName > (int) a.GetFiles().size() - 1) {
            cout << "Select one file [0-" << a.GetFiles().size() - 1 << "]" << endl;
            cin >> fileName;
            cin.ignore();
        }

        map<string, set<CFileInBackup> > m = a.GetFiles();
        map<string, set<CFileInBackup> >::const_iterator it = m.begin();

        for (int i = 0; i < fileName; i++) {
            it++;
        }

        //-----If it's a directory, recover latest files
        if ((*(it->second.begin())).IsDirectory()) {
            a.RecoverDirectory((*(it->second.begin())).getName());
            return;
        }

        //----If it's a file, offer to choose from versions
        PrintSet(it->second);

        fileVersion = -1;
        while (fileVersion < 0 || fileVersion > (int) it->second.size() - 1) {
            cout << "Select version of a file [0-" << it->second.size() - 1 << "]" << endl;
            cin >> fileVersion;
            cin.ignore();
        }

        set<CFileInBackup>::iterator sit = it->second.begin();
        for (int i = 0; i < fileVersion; i++) {
            sit++;
        }

        a.Recover((*sit));

    } else {
        cout << "No files in backup." << endl;
    }
}

void UI::PrintSet(const set<CFileInBackup> & files) {
    int i = 0;
    set<CFileInBackup>::iterator sit;

    for (sit = files.begin(); sit != files.end(); ++sit) {
        cout << "[" << i << "] ";
        (*sit).Print();
        i++;
    }
}




