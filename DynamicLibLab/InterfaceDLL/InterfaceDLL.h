#pragma once

#ifdef INTERFACEDLL_EXPORTS
#define API_DECL __declspec(dllexport)
#else
#define API_DECL __declspec(dllimport)
#endif

class IComputationManager {
public:
    virtual ~IComputationManager() {}

    virtual bool LoadWorker() = 0;
    virtual void RunWorker() = 0;
    virtual void UnloadWorker() = 0;
    virtual bool IsWorkerLoaded() = 0;
};


extern "C" API_DECL IComputationManager* CreateManager();