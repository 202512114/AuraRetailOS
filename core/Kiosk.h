#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../common/UI.h"
#include "../inventory/InventoryProxy.h"
#include "../hardware/Dispenser.h"
#include "../payment/Payment.h"

namespace aura {

class PricingPolicy {
public:
    virtual ~PricingPolicy() {}
    virtual int compute(int basePrice) const = 0;
    virtual std::string getName() const = 0;
};

class StandardPricing : public PricingPolicy {
public:
    int compute(int basePrice) const override { return basePrice; }
    std::string getName() const override { return "Standard Pricing"; }
};

class DiscountPricing : public PricingPolicy {
public:
    int compute(int basePrice) const override { return std::max(1, basePrice - 2); }
    std::string getName() const override { return "Discount Pricing"; }
};

class KioskInterface {
public:
    virtual ~KioskInterface() {}
    virtual void purchaseItem() = 0;
    virtual void refundTransaction() = 0;
    virtual void runDiagnostics() = 0;
    virtual void restockInventory() = 0;
};

class Kiosk : public KioskInterface {
private:
    std::string kioskType;
    InventoryProxy& proxy;
    std::unique_ptr<Dispenser> dispenser;
    std::unique_ptr<PricingPolicy> pricing;
    std::vector<std::string> modules;
    std::vector<Transaction> transactions;
    std::vector<std::string> history;
    int totalRevenue = 0;
    int txCounter = 1;

public:
    Kiosk(std::string kioskType, InventoryProxy& proxy, std::unique_ptr<Dispenser> dispenser, std::unique_ptr<PricingPolicy> pricing)
        : kioskType(std::move(kioskType)), proxy(proxy), dispenser(std::move(dispenser)), pricing(std::move(pricing)) {}

    std::string type() const { return kioskType; }
    std::string dispenserName() const { return dispenser->getType(); }
    std::string pricingName() const { return pricing->getName(); }
    int revenue() const { return totalRevenue; }

    bool refrigerationAvailable() const {
        return std::find(modules.begin(), modules.end(), "Refrigeration Module") != modules.end();
    }

    InventoryProxy& getProxy() { return proxy; }
    const std::vector<std::string>& getModules() const { return modules; }
    const std::vector<Transaction>& getTransactions() const { return transactions; }

    std::string nextTransactionId() {
        std::ostringstream out;
        out << "TXN-" << std::setw(4) << std::setfill('0') << txCounter++;
        return out.str();
    }

    void attachModule(const std::string& module) {
        if (std::find(modules.begin(), modules.end(), module) != modules.end()) {
            std::cout << module << " is already attached.\n";
            return;
        }
        modules.push_back(module);
        std::cout << module << " attached successfully.\n";
    }

    std::string showModules() const {
        if (modules.empty()) {
            return "None";
        } else {
            std::string s = "";
            for (const auto& m : modules) {
                s += m + " ";
            }
            return s;
        }
        return "None";
    }

    void replaceDispenser(std::unique_ptr<Dispenser> d) {
        dispenser = std::move(d);
        std::cout << "Dispenser changed to " << dispenser->getType() << ".\n";
    }

    bool purchaseVisibleItem(int visibleId, int qty, PaymentProcessor& payment) {
        auto items = proxy.customerItems(kioskType, refrigerationAvailable());
        VisibleItem selected;
        bool found = false;

        for (const auto& item : items) {
            if (item.visibleId == visibleId) {
                selected = item;
                found = true;
                break;
            }
        }

        if (!found) return false;

        int total = pricing->compute(selected.price) * qty;
        std::cout << "Final amount: Rs." << total << "\n";

        if (!payment.pay(total)) {
            std::cout << "Payment failed.\n";
            history.push_back("Purchase failed: payment rejected");
            return false;
        }

        bool stockOk = proxy.purchaseByVisibleId(kioskType, refrigerationAvailable(), visibleId, qty, selected);
        if (!stockOk) {
            payment.refund(total);
            std::cout << "Purchase failed due to stock issue. Transaction rolled back.\n";
            history.push_back("Purchase failed: stock issue");
            return false;
        }

        if (!dispenser->dispense(selected.name)) {
            payment.refund(total);
            std::cout << "Dispensing failed. Transaction rolled back.\n";
            history.push_back("Purchase failed: dispenser issue");
            return false;
        }

        Transaction tx;
        tx.id = nextTransactionId();
        tx.itemName = selected.name;
        tx.quantity = qty;
        tx.amount = total;
        tx.paymentMethod = payment.getName();
        tx.refunded = false;
        transactions.push_back(tx);

        totalRevenue += total;

        std::cout << "Stock updated successfully.\n";
        std::cout << "Purchase completed.\n";
        std::cout << "Transaction ID: " << tx.id << "\n";

        history.push_back("Purchase success: " + tx.id + " | " + tx.itemName + " | " + tx.paymentMethod);
        return true;
    }

    bool refundById(const std::string& txId) {
        for (auto& tx : transactions) {
            if (tx.id == txId) {
                if (tx.refunded) {
                    std::cout << "This transaction is already refunded.\n";
                    history.push_back("Refund failed: already refunded");
                    return false;
                }

                std::unique_ptr<PaymentProcessor> method = PaymentFactory::create(tx.paymentMethod);
                if (!method) {
                    std::cout << "Refund failed due to unknown payment method.\n";
                    history.push_back("Refund failed: unknown payment method");
                    return false;
                }

                std::cout << "Transaction Found:\n";
                std::cout << "Item           : " << tx.itemName << "\n";
                std::cout << "Amount         : Rs." << tx.amount << "\n";
                std::cout << "Payment Method : " << tx.paymentMethod << "\n";

                if (method->refund(tx.amount)) {
                    tx.refunded = true;
                    std::cout << "Refund completed successfully.\n";
                    history.push_back("Refund success: " + tx.id);
                    return true;
                }

                std::cout << "Refund failed.\n";
                history.push_back("Refund failed: " + tx.id);
                return false;
            }
        }

        std::cout << "Transaction not found.\n";
        history.push_back("Refund failed: transaction not found");
        return false;
    }

    void showTransactions() const {
        UI::section("Transaction History");
        std::cout << "+-----------------------------------------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(10) << "Txn ID" << std::setw(28) << "Item" << std::setw(8) << "Qty"
             << std::setw(10) << "Amount" << std::setw(14) << "Method" << std::setw(10) << "Status" << "|\n";
        std::cout << "+-----------------------------------------------------------------------------------+\n";

        if (transactions.empty()) {
            std::cout << "| " << std::left << std::setw(81) << "No transactions available." << "|\n";
        } else {
            for (const auto& tx : transactions) {
                std::cout << "| " << std::left << std::setw(10) << tx.id
                     << std::setw(28) << tx.itemName
                     << std::setw(8) << tx.quantity
                     << std::setw(10) << tx.amount
                     << std::setw(14) << tx.paymentMethod
                     << std::setw(10) << (tx.refunded ? "Refunded" : "Paid") << "|\n";
            }
        }

        std::cout << "+-----------------------------------------------------------------------------------+\n";
    }

    void diagnostics() {
        std::cout << "\n+-------------------------------- SYSTEM DIAGNOSTICS -------------------------------+\n";
        std::cout << "| Kiosk Type       : " << std::left << std::setw(58) << kioskType << "|\n";
        std::cout << "| Dispenser        : " << std::left << std::setw(58) << dispenser->getType() << "|\n";
        std::cout << "| Dispenser Status : " << std::left << std::setw(58) << dispenser->diagnostics() << "|\n";
        std::cout << "| Refrigeration    : " << std::left << std::setw(58) << (refrigerationAvailable() ? "Attached" : "Not Attached") << "|\n";
        std::cout << "| Pricing Policy   : " << std::left << std::setw(58) << pricing->getName() << "|\n";
        std::cout << "| Revenue          : Rs." << std::left << std::setw(55) << totalRevenue << "|\n";
        std::cout << "+-----------------------------------------------------------------------------------+\n";
        history.push_back("Diagnostics executed");
    }

    void purchaseItem() override {}
    void refundTransaction() override {}
    void runDiagnostics() override {}
    void restockInventory() override {}
};

} // namespace aura
