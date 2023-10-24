#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

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
        ifstream file(path);
        if (file) {
            file.close();
            throw runtime_error("File already exists");
        }

        ofstream outputFile(path);
        if (!outputFile) {
            throw runtime_error("Error opening the file for writing");
        }
        outputFile << content;
    }
};


int main() {
    string filePath;
    int command;

    cout << "Choose the command:\n";
    cout << "1 - Read file\n";
    cout << "2 - Write to file\n";
    cin >> command;

    if (command == 1) {
        cout << "Enter the path to the file: ";
        cin >> filePath;
        IReader* reader = new FileReader();
        try {
            string fileContent = reader->read(filePath);
            cout << "File content:\n" << fileContent;
        } catch (const exception& exception) {
            cerr << "Error: " << exception.what() << endl;
        }
    } else if (command == 2) {
        cout << "Enter the path to the file: ";
        cin >> filePath;
        IWriter* writer = new FileWriter();
        try {
            cout << "Enter new content to write to the file: ";
            string newContent;
            cin.ignore();
            getline(cin, newContent);
            writer->write(filePath, newContent);
            cout << "File has been updated with new content." << endl;
        } catch (const exception& exception) {
            cerr << "Error: " << exception.what() << endl;
        }
    }

    return 0;
}
