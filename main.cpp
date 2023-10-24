#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

class IReader;  //  прототипи
class IWriter;

int command = 0;
int key, key1;
string inputFilePath, outputFilePath, inputFilePath2, outputFilePath2;

IReader* reader = nullptr;
IWriter* writer = nullptr;

char* encrypt(char* rawText, int key)
{
    int length = strlen(rawText);
    char* result = new char[length + 1];

    for (int i = 0; i < length; i++) {
        char currentChar = rawText[i];
        if (isupper(currentChar)) {
            currentChar = char(int(currentChar + key - 65) % 26 + 65);
        } else if (islower(currentChar)) {
            currentChar = char(int(currentChar + key - 97) % 26 + 97);
        }
        result[i] = currentChar;
    }

    result[length] = '\0';

    return result;
}

char* decrypt(char* encryptedText, int key1) {
    int length = strlen(encryptedText);
    char* result = new char[length + 1];

    key1 = key1 % 26;

    for (int i = 0; i < length; i++) {
        char currentChar = encryptedText[i];
        if (isupper(currentChar)) {
            currentChar = char(int(currentChar - key1 - 65 + 26) % 26 + 65);
        } else if (islower(currentChar)) {
            currentChar = char(int(currentChar - key1 - 97 + 26) % 26 + 97);
        }
        result[i] = currentChar;
    }

    result[length] = '\0';

    return result;
}

class SecretMode {
public:
    int generateRandomKey() {
        srand(time(0));
        return rand() % 26;
    }
};

class IReader {
public:
    virtual ~IReader() {}
    virtual string read(const string& path) = 0;
};

class IWriter {
public:
    virtual ~IWriter() {}
    virtual void write(const string& path, const string& content) = 0;
};

class FileReader : public IReader {
public:
    virtual string read(const string& path) override {
        ifstream file(path);
        if (!file) {
            throw runtime_error("File not found");
        }

        string content;
        string line;
        while (getline(file, line)) {
            content += line + '\n';
        }
        return content;
    }
};

class FileWriter : public IWriter {
public:
    virtual void write(const string& path, const string& content) override {
        ofstream outputFile(path);
        if (!outputFile) {
            throw runtime_error("Error opening the file for writing");
        }
        outputFile << content;
    }
};

int main() {
    SecretMode secretMode;

    while (true) {
        int mode = 0;
        cout << "Choose the mode:\n";
        cout << "1 - Normal Mode\n";
        cout << "2 - Secret Mode\n";
        cin >> mode;
        cin.ignore();

        switch (mode) {
            case 1:
                command = 0;
                cout << "Choose the command:\n";
                cout << "1 - Encrypt\n";
                cout << "2 - Decrypt\n";
                cin >> command;
                cin.ignore();

                switch (command) {
                    case 1:
                        cout << "Enter the path to the input file: ";
                        cin >> inputFilePath;

                        cout << "Enter the path to the output file: ";
                        cin >> outputFilePath;

                        reader = new FileReader();
                        writer = new FileWriter();

                        try {
                            string fileContent = reader->read(inputFilePath);
                            cout << "Enter your key: ";
                            cin >> key;

                            char* encryptedText = encrypt(const_cast<char*>(fileContent.c_str()), key);
                            writer->write(outputFilePath, encryptedText);
                            delete[] encryptedText;
                            cout << "Encryption completed." << endl;
                        } catch (const exception& exception) {
                            cerr << "Error: " << exception.what() << endl;
                        }
                        delete reader;
                        delete writer;
                        break;

                    case 2:
                        cout << "Enter the path to the input file: ";
                        cin >> inputFilePath2;

                        cout << "Enter the path to the output file: ";
                        cin >> outputFilePath2;

                        reader = new FileReader();
                        writer = new FileWriter();

                        try {
                            string fileContent = reader->read(inputFilePath2);
                            cout << "Enter key for decrypting: ";
                            cin >> key1;

                            char* decryptedText = decrypt(const_cast<char*>(fileContent.c_str()), key1);
                            writer->write(outputFilePath2, decryptedText);
                            delete[] decryptedText;
                            cout << "Decryption completed." << endl;
                        } catch (const exception& exception) {
                            cerr << "Error: " << exception.what() << endl;
                        }
                        delete reader;
                        delete writer;
                        break;

                    default:
                        cout << "Invalid command." << endl;
                }
                break;

            case 2:
                key = secretMode.generateRandomKey();
                cout << "Generated key for secret mode: " << key << endl;
                cout << "Enter the path to the input file: ";
                cin >> inputFilePath;
                cout << "Enter the path to the output file: ";
                cin >> outputFilePath;
                reader = new FileReader();
                writer = new FileWriter();

                try{
                    string fileContent = reader->read(inputFilePath);
                    char* encryptedText = encrypt(const_cast<char*>(fileContent.c_str()), key);
                    writer->write(outputFilePath, encryptedText);
                    delete[] encryptedText;
                    cout << "Encryption completed." << endl;

                } catch(const exception& exception) {
                    cerr << "Error: " << exception.what() << endl;
                }
                delete reader;
                delete writer;
                break;

            default:
                cout << "Invalid mode." << endl;
                break;
        }
    }
}




