#pragma once

class ICompute {
public:
    virtual ~ICompute() {}
    virtual void RunComputation() = 0;
};

typedef ICompute* (*CreateComputeFn)();