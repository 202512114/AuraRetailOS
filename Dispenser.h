#pragma once
#include <iostream>
#include <string>

namespace aura {

class Dispenser {
public:
    virtual ~Dispenser() {}
    virtual bool dispense(const std::string& itemName) = 0;
    virtual std::string getType() const = 0;
    virtual std::string diagnostics() const = 0;
};

class SpiralDispenser : public Dispenser {
public:
    bool dispense(const std::string& itemName) override {
        std::cout << "Spiral unit rotated and dispensed " << itemName << ".\n";
        return true;
    }
    std::string getType() const override { return "Spiral Dispenser"; }
    std::string diagnostics() const override { return "Online"; }
};

class RoboticDispenser : public Dispenser {
public:
    bool dispense(const std::string& itemName) override {
        std::cout << "Robotic arm picked and delivered " << itemName << ".\n";
        return true;
    }
    std::string getType() const override { return "Robotic Arm Dispenser"; }
    std::string diagnostics() const override { return "Online"; }
};

class ConveyorDispenser : public Dispenser {
public:
    bool dispense(const std::string& itemName) override {
        std::cout << "Conveyor belt moved and delivered " << itemName << ".\n";
        return true;
    }
    std::string getType() const override { return "Conveyor Dispenser"; }
    std::string diagnostics() const override { return "Online"; }
};

} // namespace aura
