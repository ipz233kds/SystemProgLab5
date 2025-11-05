#include "pch.h" 
#include "InterfaceDLL.h"
#include "../ICompute.h" 
#include <windows.h>
#include <iostream>

class ComputationManager : public IComputationManager {
private:
    HMODULE hWorkerDLL;
    ICompute* pWorker; 
    CreateComputeFn pfnCreate;

public:
    ComputationManager() : hWorkerDLL(NULL), pWorker(NULL), pfnCreate(NULL) {
        std::cout << "[InterfaceDLL]: Менеджер створено." << std::endl;
    }

    ~ComputationManager() {
        if (IsWorkerLoaded()) {
            UnloadWorker();
        }
    }

    bool IsWorkerLoaded() override {
        return (hWorkerDLL != NULL && pWorker != NULL);
    }

    bool LoadWorker() override {
        if (IsWorkerLoaded()) {
            std::cout << "[InterfaceDLL]: Помилка: ComputationDLL вже завантажена." << std::endl;
            return true;
        }

        std::cout << "[InterfaceDLL]: Спроба завантажити ComputationDLL.dll..." << std::endl;
        hWorkerDLL = LoadLibrary(L"ComputationDLL.dll");
        if (hWorkerDLL == NULL) {
            std::cout << "[InterfaceDLL]: ПОМИЛКА: Не вдалося знайти/завантажити ComputationDLL.dll. Error: " << GetLastError() << std::endl;
            return false;
        }

        pfnCreate = (CreateComputeFn)GetProcAddress(hWorkerDLL, "CreateComputeInstance");
        if (pfnCreate == NULL) {
            std::cout << "[InterfaceDLL]: ПОМИЛКА: Не вдалося знайти функцію 'CreateComputeInstance'." << std::endl;
            FreeLibrary(hWorkerDLL);
            hWorkerDLL = NULL;
            return false;
        }

        pWorker = pfnCreate();
        std::cout << "[InterfaceDLL]: ComputationDLL успішно завантажена." << std::endl;
        return true;
    }

    void RunWorker() override {
        if (!IsWorkerLoaded()) {
            std::cout << "[InterfaceDLL]: Помилка: Робітника не завантажено." << std::endl;
            return;
        }
        pWorker->RunComputation();
    }

    void UnloadWorker() override {
        if (!IsWorkerLoaded()) {
            std::cout << "[InterfaceDLL]: Помилка: Робітника і так не завантажено." << std::endl;
            return;
        }

        delete pWorker;
        pWorker = NULL;

        FreeLibrary(hWorkerDLL);
        hWorkerDLL = NULL;
        pfnCreate = NULL;

        std::cout << "[InterfaceDLL]: ComputationDLL успішно вивантажена." << std::endl;
    }
};

extern "C" API_DECL IComputationManager* CreateManager() {
    return new ComputationManager();
}