#pragma once
#include <iostream>
#include <memory>
#include <string>

namespace aura {

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() {}
    virtual bool pay(int amount) = 0;
    virtual bool refund(int amount) = 0;
    virtual std::string getName() const = 0;
};

class UPIAdapter : public PaymentProcessor {
public:
    bool pay(int amount) override {
        std::cout << "UPI payment successful for Rs." << amount << "\n";
        return true;
    }

    bool refund(int amount) override {
        std::cout << "Refunded Rs." << amount << " to UPI\n";
        return true;
    }

    std::string getName() const override { return "UPI"; }
};

class CardAdapter : public PaymentProcessor {
public:
    bool pay(int amount) override {
        std::cout << "Card payment approved for Rs." << amount << "\n";
        return true;
    }

    bool refund(int amount) override {
        std::cout << "Refunded Rs." << amount << " to Card\n";
        return true;
    }

    std::string getName() const override { return "Card"; }
};

class WalletAdapter : public PaymentProcessor {
public:
    bool pay(int amount) override {
        std::cout << "Wallet payment successful for Rs." << amount << "\n";
        return true;
    }

    bool refund(int amount) override {
        std::cout << "Refunded Rs." << amount << " to Wallet\n";
        return true;
    }

    std::string getName() const override { return "Wallet"; }
};

class NetBankingAdapter : public PaymentProcessor {
public:
    bool pay(int amount) override {
        std::cout << "NetBanking payment successful for Rs." << amount << "\n";
        return true;
    }

    bool refund(int amount) override {
        std::cout << "Refunded Rs." << amount << " to NetBanking\n";
        return true;
    }

    std::string getName() const override { return "NetBanking"; }
};

class PaymentFactory {
public:
    static std::unique_ptr<PaymentProcessor> create(const std::string& name) {
        if (name == "UPI") return std::unique_ptr<PaymentProcessor>(new UPIAdapter());
        if (name == "Card") return std::unique_ptr<PaymentProcessor>(new CardAdapter());
        if (name == "Wallet") return std::unique_ptr<PaymentProcessor>(new WalletAdapter());
        if (name == "NetBanking") return std::unique_ptr<PaymentProcessor>(new NetBankingAdapter());
        return nullptr;
    }
};

} // namespace aura
