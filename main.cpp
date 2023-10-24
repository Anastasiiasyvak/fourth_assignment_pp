#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

class IReader{ // загальний інтерфейс для об'єктів ( не надає конкретної реалізації)
public:
    virtual ~IReader(){}
    virtual string read(const string& path) = 0;
};

class FileReader : public IReader{
public:
    virtual string read(const string& path) override {
        ifstream file(path);
        if (!file) {
            throw runtime_error("File not found");
        }

        string content;
        string line;
        while (getline(file, line)){
            content += line + '\n';
        }
        return content;
    }
};


int main() {
    string filePath; // /home/nastia/CLionProjects/forth_assignment/file.txt
    cout << "Enter the path to the file: ";
    cin >> filePath;

    IReader* reader = new FileReader();
    try {
        string fileContent = reader->read(filePath);
        cout << "File content : \n " << fileContent;
    }
    catch (const exception& exception) {
        cerr << "Error: " << exception.what() << endl;
    }

    delete reader;
    return 0;
}

























