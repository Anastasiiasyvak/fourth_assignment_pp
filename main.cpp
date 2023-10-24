#include <iostream>
#include <cstring>
using namespace std;

class CaesarCipher {
private:
    char* encryptedText;

public:
    CaesarCipher() : encryptedText(nullptr) {}

    ~CaesarCipher() {
        delete[] encryptedText;
    }

    char* encrypt(const char* rawText, int key) {
        int length = strlen(rawText);
        encryptedText = new char[length + 1];

        for (int i = 0; i < length; i++) {
            char currentChar = rawText[i];
            if (isupper(currentChar)) {
                currentChar = char(int(currentChar + key - 65) % 26 + 65);
            } else if (islower(currentChar)) {
                currentChar = char(int(currentChar + key - 97) % 26 + 97);
            }
            encryptedText[i] = currentChar;
        }

        encryptedText[length] = '\0';

        return encryptedText;
    }

    char* decrypt(int key1) {
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

    const char* getEncryptedText() const {
        return encryptedText;
    }
};

int main() {
    char rawText[100];
    int key = 0;
    int key1 = 0;
    CaesarCipher cipher;

    while (true) {
        int command = 0;
        std::cout << "\nChoose the command: \n1 encrypt command \n2 - decrypt command \n";
        std::cin >> command;
        std::cin.ignore();

        switch (command) {
            case 1:
                cout << "Enter the rawText:\n";
                cin.getline(rawText, sizeof(rawText));
                cout << "Enter our key:\n";
                cin >> key;
                cout << "Text : " << rawText;
                cout << "\nKey: " << key;
                cipher.encrypt(rawText, key);
                cout << "\nEncrypted text: " << cipher.getEncryptedText();
                break;

            case 2:
                if (cipher.getEncryptedText()) {
                    cout << "Enter key for decrypting:\n";
                    cin >> key1;
                    char* decryptedText = cipher.decrypt(key1);
                    cout << "\nDecrypted text:  " << decryptedText;
                    delete[] decryptedText;
                } else {
                    cout << "\nNo encrypted text to decrypt.";
                }
                break;

            default:
                cout << "\nInvalid command.";
                break;
        }
    }
}
