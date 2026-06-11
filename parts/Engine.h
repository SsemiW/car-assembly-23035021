#pragma once
#include "IEngine.h"

class GmEngine : public IEngine
{
public:
    std::string name() const override { return "GM"; }
    Engine engineType() const override { return Engine::GM; }
};

class ToyotaEngine : public IEngine
{
public:
    std::string name() const override { return "TOYOTA"; }
    Engine engineType() const override { return Engine::TOYOTA; }
};

class WiaEngine : public IEngine
{
public:
    std::string name() const override { return "WIA"; }
    Engine engineType() const override { return Engine::WIA; }
};

class BrokenEngine : public IEngine
{
public:
    std::string name() const override { return "BROKEN"; }
    Engine engineType() const override { return Engine::BROKEN; }
};
