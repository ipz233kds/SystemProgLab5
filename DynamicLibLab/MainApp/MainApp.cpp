#include <iostream>
#include <string>
#include "InterfaceDLL.h"
#include <windows.h> 
#include <shlwapi.h> 
#pragma comment(lib, "shlwapi.lib")

void PrintMenu() {
    std::cout << "\n--- МЕНЮ УТІЛІТИ ---" << std::endl;
    std::cout << "1. Завантажити ComputationDLL (Явне завантаження)" << std::endl;
    std::cout << "2. Виконати обчислення (Багатопотоковість)" << std::endl;
    std::cout << "3. Вивантажити ComputationDLL" << std::endl;
    std::cout << "4. Показати роботу shlwapi.dll (Стороння DLL)" << std::endl;
    std::cout << "0. Вихід" << std::endl;
    std::cout << "Ваш вибір: ";
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    std::wcout.imbue(std::locale("ukrainian"));
    std::wcin.imbue(std::locale("ukrainian"));

    std::cout << "--- MainApp: Програма запущена ---" << std::endl;

    IComputationManager* pManager = CreateManager();
    std::cout << "--- MainApp: Менеджер з InterfaceDLL створено (неявне завантаження) ---" << std::endl;

    bool isRunning = true;
    while (isRunning) {
        PrintMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            if (pManager->IsWorkerLoaded()) {
                std::cout << "[MainApp]: ПОМИЛКА: ComputationDLL вже завантажена." << std::endl;
            }
            else {
                pManager->LoadWorker();
            }
            break;
        case 2:
            if (pManager->IsWorkerLoaded()) {
                std::cout << "[MainApp]: Запуск обчислень... (Дивіться на Диспетчер завдань)" << std::endl;
                pManager->RunWorker();
                std::cout << "[MainApp]: Обчислення завершено." << std::endl;
            }
            else {
                std::cout << "[MainApp]: ПОМИЛКА: Спочатку завантажте DLL (пункт 1)." << std::endl;
            }
            break;
        case 3:
            pManager->UnloadWorker();
            break;
        case 4:
        {
            std::cout << "[MainApp]: Виклик функції PathFindFileNameW() із shlwapi.dll..." << std::endl;

            wchar_t szPath[MAX_PATH] = L"C:\\Windows\\System32\\notepad.exe";

            wchar_t* pszFileName = PathFindFileNameW(szPath);

            std::wcout << L"  [shlwapi]: Повний шлях: " << szPath << std::endl;
            std::wcout << L"  [shlwapi]: Функція знайшла ім'я файлу: " << pszFileName << std::endl;
        }
        break;
        case 0:
            isRunning = false;
            break;
        default:
            std::cout << "[MainApp]: Невірний вибір." << std::endl;
        }
    }

    delete pManager;
    std::cout << "--- MainApp: Менеджер видалено. Програма завершує роботу ---" << std::endl;
    return 0;
}