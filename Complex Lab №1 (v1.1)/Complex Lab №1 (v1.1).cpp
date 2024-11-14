#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip> 

using namespace std;

struct VideoCards {
    string CardName;
    string CardCoolingType;
    int CapacityVRAM;
    int VideoMemoryFrequency;
    int DirectXVersion;
};

int main() {
    setlocale(LC_ALL, "RUSSIAN");

    VideoCards Card;

    int card_count = 0;
    int UserInput = -1;
    int Status = 1;
    string Interface(107, '-');
    cout << Interface << endl << "Приветствуем! Перед вами каталог видеокарт. Вы можете:\n1) Добавить запись;\n2) Просмотреть все записи;\n3) Поиск записи по атрибуту;\n4) Выход;\n";

    while (Status == 1) {
        cout << Interface << endl << "Введите номер операции которую хотите совершить: ";
        cin >> UserInput;
        switch (UserInput) {
        case 1: {
            cout << "Вы выбрали добавление.\nВведите модель видеокарты, тип её охлаждения, количество гигабайт видеопамяти, частоту графического процессора (в мегагерцах), поддерживаемую версию DirectX\n(Пример: GTX 2060; Active; 6; 1300; 12)\n";

            ofstream inFile("File.bin", ios::binary | ios::app);
            if (inFile) {

                //Обработка ввода пользователем
                string NewCard, temp;
                cin.ignore();
                getline(cin, NewCard);
                int count = 0;
                NewCard += ";";

                for (int i = 0; i < NewCard.size(); i++) {

                    if (NewCard[i] != ';') {
                        temp += NewCard[i];
                    }
                    else {
                        if (count == 0) {
                            Card.CardName = temp;
                            count++;
                            temp = "";
                        }
                        else if (count == 1) {
                            Card.CardCoolingType = temp;
                            count++;
                            temp = "";
                        }
                        else if (count == 2) {
                            Card.CapacityVRAM = stoi(temp);
                            count++;
                            temp = "";
                        }
                        else if (count == 3) {
                            Card.VideoMemoryFrequency = stoi(temp);
                            count++;
                            temp = "";
                        }
                        else if (count == 4) {
                            Card.DirectXVersion = stoi(temp);
                            count++;
                            temp = "";
                        }
                    }
                }

                //Запись
                size_t nameLength = Card.CardName.size();
                inFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                inFile.write(Card.CardName.c_str(), nameLength);

                size_t coolingTypeLength = Card.CardCoolingType.size();
                inFile.write(reinterpret_cast<char*>(&coolingTypeLength), sizeof(coolingTypeLength));
                inFile.write(Card.CardCoolingType.c_str(), coolingTypeLength);

                inFile.write(reinterpret_cast<char*>(&Card.CapacityVRAM), sizeof(Card.CapacityVRAM));
                inFile.write(reinterpret_cast<char*>(&Card.VideoMemoryFrequency), sizeof(Card.VideoMemoryFrequency));
                inFile.write(reinterpret_cast<char*>(&Card.DirectXVersion), sizeof(Card.DirectXVersion));
                inFile.close();
            }
            UserInput = -1;
            break;
        }
        case 2: {
            cout << "Вы выбрали просмотр всех записей.\n";

            //Чтение
            ifstream outFile("File.bin", ios::binary | ios::in);
            if (outFile) {

                if (outFile.peek() == ifstream::traits_type::eof()) {
                    cout << "Записей нет.";
                    break;
                }

                cout << "+-----+-----------------+------------------+-------------------+--------------------+---------------------+" << endl;
                cout << "|  N  |      Model      |   Cooling type   |   Capacity VRAM   |  Memory Frequency  |   DirectX Version   |" << endl;
                cout << "+-----+-----------------+------------------+-------------------+--------------------+---------------------+" << endl;

                while (true) {
                    VideoCards SomeCard;
                    card_count++;
                    size_t nameLength;
                    if (!outFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength))) {
                        break;
                    }
                    char* nameBuffer = new char[nameLength + 1];
                    outFile.read(nameBuffer, nameLength);
                    nameBuffer[nameLength] = '\0';
                    SomeCard.CardName = nameBuffer;
                    delete[] nameBuffer;

                    size_t coolingTypeLength;
                    outFile.read(reinterpret_cast<char*>(&coolingTypeLength), sizeof(coolingTypeLength));
                    char* coolingBuffer = new char[coolingTypeLength + 1];
                    outFile.read(coolingBuffer, coolingTypeLength);
                    coolingBuffer[coolingTypeLength] = '\0';
                    SomeCard.CardCoolingType = coolingBuffer;
                    delete[] coolingBuffer;

                    outFile.read(reinterpret_cast<char*>(&SomeCard.CapacityVRAM), sizeof(SomeCard.CapacityVRAM));
                    outFile.read(reinterpret_cast<char*>(&SomeCard.VideoMemoryFrequency), sizeof(SomeCard.VideoMemoryFrequency));
                    outFile.read(reinterpret_cast<char*>(&SomeCard.DirectXVersion), sizeof(SomeCard.DirectXVersion));

                    cout << "|  " << setw(3) << left << card_count << "| " << setw(16) << SomeCard.CardName <<
                        "| " << setw(17) << SomeCard.CardCoolingType << "| " << setw(18) << SomeCard.CapacityVRAM << "| " << setw(19) <<
                        SomeCard.VideoMemoryFrequency << "| " << setw(20) << SomeCard.DirectXVersion << "|" << endl;
                    cout << Interface << endl;
                }
                outFile.close();
                card_count = 0;
            }
            UserInput = -1;
            break;
        }
        case 3: {
            cout << "Вы выбрали поиск записи по атрибуту или по диапазону.\n1) Поиск по атрибуту\n2) Поиск по диапазону\nВыберите какой поиск вы хотите использовать: ";
            int Op = -1;
            cin >> Op;
            vector<VideoCards> AllCards;
            ifstream outFile("File.bin", ios::binary | ios::in);
            if (outFile) {

                if (outFile.peek() == ifstream::traits_type::eof()) {
                    cout << "Записей нет.";
                    break;
                }

                while (true) {
                    VideoCards SomeCard;

                    size_t nameLength;
                    if (!outFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength))) {
                        break;
                    }
                    char* nameBuffer = new char[nameLength + 1];
                    outFile.read(nameBuffer, nameLength);
                    nameBuffer[nameLength] = '\0';
                    SomeCard.CardName = nameBuffer;
                    delete[] nameBuffer;

                    size_t coolingTypeLength;
                    outFile.read(reinterpret_cast<char*>(&coolingTypeLength), sizeof(coolingTypeLength));
                    char* coolingBuffer = new char[coolingTypeLength + 1];
                    outFile.read(coolingBuffer, coolingTypeLength);
                    coolingBuffer[coolingTypeLength] = '\0';
                    SomeCard.CardCoolingType = coolingBuffer;
                    delete[] coolingBuffer;

                    outFile.read(reinterpret_cast<char*>(&SomeCard.CapacityVRAM), sizeof(SomeCard.CapacityVRAM));
                    outFile.read(reinterpret_cast<char*>(&SomeCard.VideoMemoryFrequency), sizeof(SomeCard.VideoMemoryFrequency));
                    outFile.read(reinterpret_cast<char*>(&SomeCard.DirectXVersion), sizeof(SomeCard.DirectXVersion));

                    AllCards.push_back(SomeCard);
                }
                outFile.close();
            }
            switch (Op) {
            case 1: {
                string attribute, value;
                cout << "Введите атрибут для поиска (model или cooling): ";
                cin >> attribute;
                cout << "Введите значение атрибута: ";
                cin.ignore();
                getline(cin, value);
                bool found = false;
                cout << "+-----+-----------------+------------------+-------------------+--------------------+---------------------+" << endl;
                cout << "|  N  |      Model      |   Cooling type   |   Capacity VRAM   |  Memory Frequency  |   DirectX Version   |" << endl;
                cout << "+-----+-----------------+------------------+-------------------+--------------------+---------------------+" << endl;

                for (const auto& card : AllCards) {
                    if ((attribute == "model" || attribute == "Model" || attribute == "m") && (card.CardName.find(value) != string::npos)) {
                        found = true;
                        card_count++;
                        cout << "|  " << setw(3) << left << card_count << "| " << setw(16) << card.CardName <<
                            "| " << setw(17) << card.CardCoolingType << "| " << setw(18) << card.CapacityVRAM << "| " << setw(19) <<
                            card.VideoMemoryFrequency << "| " << setw(20) << card.DirectXVersion << "|" << endl;
                        cout << Interface << endl;
                    }
                    else if ((attribute == "cooling" || attribute == "Cooling" || attribute == "c") && card.CardCoolingType.find(value) != string::npos) {
                        found = true;
                        card_count++;
                        cout << "|  " << setw(3) << left << card_count << "| " << setw(16) << card.CardName <<
                            "| " << setw(17) << card.CardCoolingType << "| " << setw(18) << card.CapacityVRAM << "| " << setw(19) <<
                            card.VideoMemoryFrequency << "| " << setw(20) << card.DirectXVersion << "|" << endl;
                        cout << Interface << endl;
                    }
                }
                if (!found) {
                    cout << "| " << setw(73) << "Записи не найдены в заданном диапазоне." << setw(33) << "| " << endl;
                    card_count = 0;
                }
                card_count = 0;
                break;
            }
            case 2: {
                string attribute;
                int min, max;
                cout << "Введите атрибут для поиска (vram или frequency): ";
                cin >> attribute;
                cout << "Введите минимальное значение: ";
                cin >> min;
                cout << "Введите максимальное значение: ";
                cin >> max;
                bool found = false;
                cout << "+-----+-----------------+------------------+-------------------+--------------------+---------------------+" << endl;
                cout << "|  N  |      Model      |   Cooling type   |   Capacity VRAM   |  Memory Frequency  |   DirectX Version   |" << endl;
                cout << "+-----+-----------------+------------------+-------------------+--------------------+---------------------+" << endl;

                for (const auto& card : AllCards) {
                    if ((attribute == "vram" || attribute == "Vram" || attribute == "v") && card.CapacityVRAM >= min && card.CapacityVRAM <= max) {
                        found = true;
                        card_count++;
                        cout << "|  " << setw(3) << left << card_count << "| " << setw(16) << card.CardName <<
                            "| " << setw(17) << card.CardCoolingType << "| " << setw(18) << card.CapacityVRAM << "| " << setw(19) <<
                            card.VideoMemoryFrequency << "| " << setw(20) << card.DirectXVersion << "|" << endl;
                        cout << Interface << endl;
                    }
                    else if ((attribute == "frequency" || attribute == "Frequency" || attribute == "f") && card.VideoMemoryFrequency >= min && card.VideoMemoryFrequency <= max) {
                        found = true;
                        card_count++;
                        cout << "|  " << setw(3) << left << card_count << "| " << setw(16) << card.CardName <<
                            "| " << setw(17) << card.CardCoolingType << "| " << setw(18) << card.CapacityVRAM << "| " << setw(19) <<
                            card.VideoMemoryFrequency << "| " << setw(20) << card.DirectXVersion << "|" << endl;
                        cout << Interface << endl;
                    }
                }
                if (!found) {
                    cout << "| " << setw(73) << "Записи не найдены в заданном диапазоне." << setw(33) << "| " << endl;
                    card_count = 0;
                }
                card_count = 0;
                break;
            }
            default: {
                cout << "Неверный ввод.\n";
            }
            }
            UserInput = 0;
            break;
        }
        case 4: {
            cout << "Выход из программы.\n";
            Status = 0;
            break;
        }
        default: {
            cout << "Неверный ввод.\n";
            UserInput = 0;
        }
        }
    }
}