#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

const int MAX_SIZE = 1000;

class IReader;  //  прототипи
class IWriter;

int command = 0;
int key, key1;
string inputFilePath, outputFilePath, inputFilePath2, outputFilePath2;

IReader* reader = nullptr;
IWriter* writer = nullptr;


class Ceasar{

public:

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


};





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
        outputFile.close();
    }
};


class TextEditor {
private:
    char* Buffer;
    bool has_text;

public:
    TextEditor() : Buffer(nullptr), has_text(false) {}

    ~TextEditor() {
        delete[] Buffer;
    }

    char* getBuffer() const {
        return Buffer;
    }

    friend class substringEditor;
    friend class CutCopyPaste;

    void TextToAppend() {
        cout << "Enter text to append: ";
        char text[MAX_SIZE];
        cin.getline(text, MAX_SIZE);

        if (!has_text) {
            Buffer = new char[strlen(text) + 1];
            strcpy(Buffer, text);
        } else {
            char *temp = new char[strlen(Buffer) + strlen(text) + 2];
            strcpy(temp, Buffer);
            strcat(temp, " ");
            strcat(temp, text);
            Buffer = temp;
        }

        std::cout << Buffer;

        has_text = true;
    }

    void NewLine() {
        strcat(Buffer, "\n");
        std::cout << "New line started.\n";
    }

    void ForSavingInFile() {
        std::string fileForSaving;
        std::cout << "Enter the file name for saving: ";
        std::cin >> fileForSaving;

        std::ofstream file(fileForSaving);

        if (file.is_open()) {
            file << Buffer;
            file.close();
            std::cout << "Text has been saved successfully." << std::endl;
        } else {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
        }
    }

    void ForLoadingFromFile() {
        char fileForLoading[MAX_SIZE];
        std::cout << "Enter the file name for loading: ";
        std::cin >> fileForLoading;
        std::ifstream loadFile(fileForLoading);

        if (!loadFile) {
            std::cout << "Error opening file for loading." << std::endl;
        } else {
            loadFile.seekg(0, std::ios::end);
            size_t fileSize = loadFile.tellg();
            loadFile.seekg(0, std::ios::beg);
            Buffer = new char[fileSize + 1];
            loadFile.read(Buffer, fileSize);
            Buffer[fileSize] = '\0';
            std::cout << "Text has been loaded successfully." << std::endl;
            loadFile.close();
            has_text = true;
        }
    }

    void PrintLoadedText() {
        std::cout << Buffer;
    }

    void Deletecommand() {
        int row, index, numSymbols;
        std::cout << "Choose line, index, and number of symbols: ";
        std::cin >> row >> index >> numSymbols;

        char *line = strtok(Buffer, "\n");
        int current_row = 0;

        while (line != nullptr) {
            if (current_row == row) {
                int lineLength = strlen(line);

                if (index >= 0 && index < lineLength) {
                    if (numSymbols > 0) {
                        if (index + numSymbols <= lineLength) {
                            memmove(&line[index], &line[index + numSymbols], lineLength - index - numSymbols + 1);
                            std::cout << line << std::endl;

                        }
                    }
                }
            }

            current_row++;
            line = strtok(nullptr, "\n");
        }
    }

    void encryptSecond()
    {
        cout << "Enter your key: ";
        cin >> key;
        int length = strlen(Buffer);
        char* result = new char[length + 1];

        for (int i = 0; i < length; i++) {
            char currentChar = Buffer[i];
            if (isupper(currentChar)) {
                currentChar = char(int(currentChar + key - 65) % 26 + 65);
            } else if (islower(currentChar)) {
                currentChar = char(int(currentChar + key - 97) % 26 + 97);
            }
            result[i] = currentChar;
        }

        result[length] = '\0';

        cout << result;

    }

    void decryptSecond() {
        cout << "Enter key for decrypting: ";
        cin >> key1;
        int length = strlen(Buffer);
        char* result = new char[length + 1];

        key1 = key1 % 26;

        for (int i = 0; i < length; i++) {
            char currentChar = Buffer[i];
            if (isupper(currentChar)) {
                currentChar = char(int(currentChar - key1 - 65 + 26) % 26 + 65);
            } else if (islower(currentChar)) {
                currentChar = char(int(currentChar - key1 - 97 + 26) % 26 + 97);
            }
            result[i] = currentChar;
        }

        result[length] = '\0';

        cout << result;

    }

};

class substringEditor{
public:
    static void InsertAtIndex(int index1, int index2, const char *wordForAdding, char* Buffer) {
        std::cout << "Choose line and index: ";
        std::cin >> index1 >> index2;
        std::cin.ignore();
        std::cout << "Enter text to insert:\n";
        char text[MAX_SIZE];
        std::cin.getline(text, MAX_SIZE);

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (index1 >= 0 && index1 < num_lines && index2 >= 0 && index2 <= strlen(lines[index1])) {
            char new_line[MAX_SIZE];
            strcpy(new_line, lines[index1]);
            strncpy(new_line + index2, text, MAX_SIZE - index2);
            strncat(new_line, lines[index1] + index2, MAX_SIZE - strlen(new_line));

            delete[] Buffer;
            Buffer = new char[MAX_SIZE];

            for (int i = 0; i < num_lines; i++) {
                if (i == index1) {
                    strncat(Buffer, new_line, MAX_SIZE - strlen(Buffer));
                } else {
                    strncat(Buffer, lines[i], MAX_SIZE - strlen(Buffer));
                }

                if (i < num_lines - 1) {
                    strncat(Buffer, "\n", MAX_SIZE - strlen(Buffer));
                }
            }

            std::cout << Buffer;
        } else {
            std::cout << "Invalid index values." << std::endl;
        }
    }

    static void TextToSearch(char* Buffer) {
        std::cout << "Enter text to search: ";
        char search_text[MAX_SIZE];
        std::cin.ignore();
        std::cin.getline(search_text, MAX_SIZE);

        int row = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            int position = 0;
            char *line_start = line;

            while (1) {
                char *word_in_line = strstr(line_start, search_text);

                if (word_in_line != nullptr) {
                    position = word_in_line - line_start;
                    std::cout << "Text '" << search_text << "' found in line " << row << " at position " << position << std::endl;
                    line_start = word_in_line + 1;
                } else {
                    break;
                }
            }

            row++;
            line = strtok(nullptr, "\n");
        }

        if (row == 0) {
            std::cout << "The word '" << search_text << "' is not found in the Buffer." << std::endl;
        }
    }

    static void InsertNewCommand(int index1, int index2, const char* wordForAdding, char* Buffer) {
        if (Buffer == nullptr) {
            std::cerr << "Buffer is empty." << std::endl;
            return;
        }

        char* lines[20];
        int num_lines = 0;
        char* line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (index1 < 0 || index1 >= num_lines || index2 < 0 || index2 > strlen(lines[index1])) {
            std::cerr << "Invalid line or index values." << std::endl;
            return;
        }

        char new_line[MAX_SIZE];
        strncpy(new_line, lines[index1], index2);
        new_line[index2] = '\0';
        strcat(new_line, wordForAdding);
        strcat(new_line, lines[index1] + index2);

        delete[] Buffer;
        Buffer = new char[MAX_SIZE];

        for (int i = 0; i < num_lines; i++) {
            if (i == index1) {
                strncat(Buffer, new_line, MAX_SIZE - strlen(Buffer));
            } else {
                strncat(Buffer, lines[i], MAX_SIZE - strlen(Buffer));
            }

            if (i < num_lines - 1) {
                strncat(Buffer, "\n", MAX_SIZE - strlen(Buffer));
            }
        }

        std::cout << Buffer;
    }

};

class CutCopyPaste{
private:
    static char clipboard[MAX_SIZE];

public:
    static void PasteCommand(char* Buffer, TextEditor& textEditor) {
        int row, index;
        std::cout << "Choose line and index to paste: ";
        std::cin >> row >> index;
        std::cin.ignore();

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (row >= 0 && row < num_lines && index >= 0 && index <= strlen(lines[row])) {
            char new_line[MAX_SIZE];
            strcpy(new_line, lines[row]);
            strncpy(new_line + index, clipboard, MAX_SIZE - index);
            strncat(new_line, lines[row] + index, MAX_SIZE - strlen(new_line));

            delete[] Buffer;
            Buffer = new char[MAX_SIZE];

            for (int i = 0; i < num_lines; i++) {
                if (i == row) {
                    strncat(Buffer, new_line, MAX_SIZE - strlen(Buffer));
                } else {
                    strncat(Buffer, lines[i], MAX_SIZE - strlen(Buffer));
                }

                if (i < num_lines - 1) {
                    strncat(Buffer, "\n", MAX_SIZE - strlen(Buffer));
                }
            }

            std::cout << Buffer;
        } else {
            std::cout << "Invalid index values." << std::endl;
        }
    }

    static void CutCommand(char* Buffer, TextEditor& textEditor) {
        int row, index, numSymbols;
        std::cout << "Choose line, index, and number of symbols: ";
        std::cin >> row >> index >> numSymbols;

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (row >= 0 && row < num_lines) {
            char *current_line = lines[row];
            int lineLength = strlen(current_line);

            if (index >= 0 && index < lineLength) {
                if (numSymbols > 0) {
                    if (index + numSymbols <= lineLength) {
                        strncpy(clipboard, &current_line[index], numSymbols);
                        clipboard[numSymbols] = '\0';

                        memmove(&current_line[index], &current_line[index + numSymbols], lineLength - index - numSymbols + 1);
                        std::cout << "Cutted Text: " << clipboard << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Invalid index values." << std::endl;
        }
    }

    static void CopyCommand(char* Buffer) {
        int row , index , numSymbols;
        std::cout << "Choose line, index, and number of symbols: ";
        std::cin >> row >> index >> numSymbols;

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (row >= 0 && row < num_lines) {
            char *current_line = lines[row];
            int lineLength = strlen(current_line);

            if (index >= 0 && index < lineLength) {
                if (numSymbols > 0) {
                    if (index + numSymbols <= lineLength) {
                        strncpy(clipboard, &current_line[index], numSymbols);
                        clipboard[numSymbols] = '\0';

                        std::cout << "Copied Text: " << clipboard << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Invalid index values." << std::endl;
        }
    }
};

char CutCopyPaste::clipboard[MAX_SIZE] = "";




int main() {
    int choice = 0;
    cout << "Choose an option:\n";
    cout << "1 - Text Editor\n";
    cout << "2 - File Encryption/Decryption\n";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        TextEditor textEditor;

        while (true) {
            int command = 0;
            cout << "\nChoose the command:\n";
            cout << "1 - Append text\n";
            cout << "2 - New line\n";
            cout << "3 - Save text to file\n";
            cout << "4 - Load text from file\n";
            cout << "5 - Print loaded text\n";
            cout << "6 - Insert text at index\n";
            cout << "7 - Search for text\n";
            cout << "8 - Delete text\n";
            cout << "11 - Cut text\n";
            cout << "12 - Paste text\n";
            cout << "13 - Copy text\n";
            cout << "14 - Insert new text at index\n";
            cout << "15 - encrypt\n";
            cout << "16 - decrypt\n";
            cout << "0 - Exit\n";
            cin >> command;
            cin.ignore();

            switch (command) {
                case 1:
                    textEditor.TextToAppend();
                    break;

                case 2:
                    textEditor.NewLine();
                    break;

                case 3:
                    textEditor.ForSavingInFile();
                    break;

                case 4:
                    textEditor.ForLoadingFromFile();
                    break;

                case 5:
                    textEditor.PrintLoadedText();
                    break;

                case 6:
                    int index1, index2;
                    char wordForAdding[MAX_SIZE];
                    substringEditor::InsertAtIndex(index1, index2, wordForAdding, textEditor.getBuffer());
                    break;

                case 7:
                    substringEditor::TextToSearch(textEditor.getBuffer());
                    break;

                case 8:
                    textEditor.Deletecommand();
                    break;

                case 11:
                    CutCopyPaste::CutCommand(textEditor.getBuffer(), textEditor);
                    break;

                case 12:
                    CutCopyPaste::PasteCommand(textEditor.getBuffer(), textEditor);
                    break;

                case 13:
                    CutCopyPaste::CopyCommand(textEditor.getBuffer());
                    break;

                case 14:
                {
                    int index1, index2;
                    char wordForAdding[MAX_SIZE];

                    cout << "Choose line and index ";
                    cin >> index1 >> index2;
                    cin.ignore();

                    cout << "Write text: ";
                    cin.getline(wordForAdding, MAX_SIZE);

                    substringEditor::InsertNewCommand(index1, index2, wordForAdding, textEditor.getBuffer());

                    break;
                }

                case 15:
                    textEditor.encryptSecond();
                    break;

                case 16:
                    textEditor.decryptSecond();
                    break;

                case 0:
                    return 0;

                default:
                    cout << "The command is not implemented." << endl;
                    break;
            }
        }
    } else if (choice == 2) {
        SecretMode secretMode;
        Ceasar ceasar;

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

                                char* encryptedText = ceasar.encrypt(const_cast<char*>(fileContent.c_str()), key);
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

                                char* decryptedText = ceasar.decrypt(const_cast<char*>(fileContent.c_str()), key1);
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
                        char* encryptedText = ceasar.encrypt(const_cast<char*>(fileContent.c_str()), key);
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
    } else {
        cout << "Invalid choice. Please choose 1 or 2." << endl;
    }

    return 0;
}