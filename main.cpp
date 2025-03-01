#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <conio.h>

const std::string filename = "passwords.txt";

std::string hidePassword() {
    std::string password;
    char ch;
    while ((ch = static_cast<char>(getch())) != '\r') {
        if (ch == 8) {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else {
            std::cout << '*';
            password += ch;
        }
    }
    return password;
}

bool hasSpace(std::string p) {
    bool hasSpace = false;

    for (int i = 0; i < p.length(); ++i) {
        if (p[i] == ' ') {
            hasSpace = true;
        } else {
            hasSpace = false;
            break;
        }
    }

    if (hasSpace) {
        std::cout << "Invalid input! No empty passwords!" << std::endl;
    }

    return hasSpace;
}

void savePassword() {
    /*
    std::string p;
    while (true) {
        std::cout << "Enter your password:";
        std::cin >> p;
        if (p.empty()) {
            std::cout << "Password is empty!" << std::endl;
        } else {
            break;
        }
    }
    */

    std::string p;
    do {
        std::cout << "Enter your password: ";
        p = hidePassword();
        std::cout << std::endl;
    } while (hasSpace(p));

    std::ofstream writeFile(filename, std::ios::app);
    if (!writeFile) {
        std::cerr << "Can't open file!" << std::endl;
        return;
    }
    writeFile << p << std::endl;
    std::cout << "Password saved!" << std::endl;
    writeFile.close();
}

void viewPasswords() {
    std::ifstream getPasswordsFile(filename);
    if (!getPasswordsFile) {
        std::cerr << "Can't open file!" << std::endl;
        return;
    }

    if (getPasswordsFile.peek() == EOF) {
        std::cout << "No passwords found!" << std::endl;
    } else {
        std::cout << "Stored passwords:" << std::endl;
        std::string line;
        while (std::getline(getPasswordsFile, line)) {
            std::cout << line << std::endl;
        }
    }

    getPasswordsFile.close();
}

void deletePassword() {
    std::ifstream getPasswordsFile(filename);
    if (!getPasswordsFile) {
        std::cerr << "Can't open file!" << std::endl;
        return;
    }

    if (getPasswordsFile.peek() == EOF) {
        std::cout << "No passwords found!" << std::endl;
    } else {
        std::cout << "Enter password to delete: ";
        std::string p;
        std::cin >> p;

        std::ofstream tempFile("temp.txt");
        if (!tempFile) {
            std::cerr << "Can't open file!" << std::endl;
            return;
        }

        bool found = false;
        bool deleted;
        std::string line;
        while (std::getline(getPasswordsFile, line)) {
            if (line == p) {
                found = true;
                std::cout << "Confirmation: are you sure? (y/n): ";
                char choice;
                std::cin >> choice;
                choice = static_cast<char>(tolower(choice));
                switch (choice) {
                    case 'y':
                        deleted = true;
                        continue;
                    case 'n':
                        deleted = false;
                        break;
                    default:
                        std::cout << "Invalid input!" << std::endl;
                        break;
                }
            }
            tempFile << line << std::endl;
        }

        getPasswordsFile.close();
        tempFile.close();

        if (found) {
            std::remove(filename.c_str());
            std::rename("temp.txt", filename.c_str());
            std::cout << (deleted ? "Password deleted!" : "Password not deleted!") << std::endl;
        }
        else {
            std::cout << "Password not found!" << std::endl;
            std::remove("temp.txt");
        }

    }
}

void updatePassword() {
    std::ifstream getPasswordsFile(filename);
    if (!getPasswordsFile) {
        std::cerr << "Can't open file!" << std::endl;
        return;
    }

    if (getPasswordsFile.peek() == EOF) {
        std::cout << "No passwords found!" << std::endl;
    } else {
        std::cout << "Enter password to update: ";
        std::string p;
        p = hidePassword();
        std::cout << std::endl;

        std::ofstream tempFile("temp.txt");
        if (!tempFile) {
            std::cerr << "Can't open file!" << std::endl;
            return;
        }

        bool found = false;
        std::string line;
        while (std::getline(getPasswordsFile, line)) {
            if (line == p) {
                std::string newP;
                do {
                    std::cout << "Enter new password: ";
                    newP = hidePassword();
                    std::cout << std::endl;
                } while (hasSpace(newP));

                tempFile << newP << std::endl;

                std::cout << "Password updated!" << std::endl;
                found = true;
                continue;
            }
            tempFile << line << std::endl;
        }

        getPasswordsFile.close();
        tempFile.close();

        if (found) {
            std::remove(filename.c_str());
            std::rename("temp.txt", filename.c_str());
        }
        else {
            std::cout << "Password not found!" << std::endl;
            std::remove("temp.txt");
        }
    }
}

bool findPassword() {
    std::ifstream getPasswordsFile(filename);
    if (!getPasswordsFile) {
        std::cerr << "Can't open file!" << std::endl;
        return false;
    }

    if (getPasswordsFile.peek() == EOF) {
        std::cout << "No passwords found!" << std::endl;
        return false;
    }

    std::cout << "Enter password to find:";
    std::string p;
    std::cin >> p;

    std::string line;
    bool found = false;
    while (std::getline(getPasswordsFile, line)) {
        if (line == p) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Password not found!" << std::endl;
        // getPasswordsFile.close(); // apparently unnecessary
    }

    return found;
}

int main() {

    std::string choice;

    while (true) {
        std::cout << "----------Menu--------" << std::endl;
        std::cout << "Save password     (s)" << std::endl;;
        std::cout << "View passwords    (v)" << std::endl;
        std::cout << "Delete a password (d)" << std::endl;
        std::cout << "Update a password (u)" << std::endl;
        std::cout << "Find a password   (f)" << std::endl;
        std::cout << "Exit              (e)" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice.length() > 1) {
            std::cout << "Invalid input! Only 1 character!" << std::endl;
            continue;
        }

        if (!std::isalpha(choice[0])) {
            std::cout << "Invalid input! Only letters!" << std::endl;
            continue;
        }

        choice = static_cast<char>(tolower(choice[0]));

        switch (choice[0]) {
            case 's': {
                savePassword();
                break;
            }

            case 'v':
                viewPasswords();
                break;

            case 'd': {
                deletePassword();
                break;
            }

            case 'u':
                updatePassword();
                break;

            case 'f':
                if (findPassword()) {
                    std::cout << "Password found!" << std::endl;
                }
                break;

            case 'e':
                std::cout << "Exiting...";
                return 0;

            default:
                std::cout << "Invalid choice!\n";
                break;
        }
    }
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.