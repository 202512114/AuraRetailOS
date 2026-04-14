#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "Inventory.h"

namespace aura {

class InventoryProxy {
private:
    Inventory& inventory;
    std::string role = "CUSTOMER";

public:
    explicit InventoryProxy(Inventory& inventory) : inventory(inventory) {}

    void setRole(const std::string& roleName) { role = roleName; }

    std::vector<VisibleItem> customerItems(const std::string& kioskType, bool refrigerationAvailable) const {
        return inventory.customerItems(kioskType, refrigerationAvailable);
    }

    std::vector<VisibleItem> managerProducts() const {
        return inventory.managerProducts();
    }

    std::vector<VisibleItem> managerBundles(bool refrigerationAvailable) const {
        return inventory.managerBundles(refrigerationAvailable);
    }

    void showCustomerItems(const std::string& kioskType, bool refrigerationAvailable) const {
        auto items = customerItems(kioskType, refrigerationAvailable);

        std::cout << "\n+--------------------------------------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(3) << "No" << std::setw(42) << "Item" << std::setw(10) << "Price"
             << std::setw(8) << "Qty" << std::setw(10) << "Type" << std::setw(12) << "Storage" << "|\n";
        std::cout << "+--------------------------------------------------------------------------------+\n";

        if (items.empty()) {
            std::cout << "| " << std::left << std::setw(80) << "No items available for this kiosk." << "|\n";
        } else {
            for (const auto& item : items) {
                std::cout << "| " << std::left << std::setw(3) << item.visibleId
                     << std::setw(42) << item.name
                     << std::setw(10) << item.price
                     << std::setw(8) << item.stock
                     << std::setw(10) << (item.isBundle ? "Bundle" : "Product")
                     << std::setw(12) << item.storage << "|\n";
            }
        }

        std::cout << "+--------------------------------------------------------------------------------+\n";
    }

    void showManagerProducts() const {
        auto items = managerProducts();

        std::cout << "\n+----------------------------------------------------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(3) << "No" << std::setw(24) << "Kiosk System" << std::setw(28) << "Product"
             << std::setw(10) << "Price" << std::setw(8) << "Qty" << std::setw(12) << "Storage" << "|\n";
        std::cout << "+----------------------------------------------------------------------------------------------+\n";

        if (items.empty()) {
            std::cout << "| " << std::left << std::setw(92) << "No products available." << "|\n";
        } else {
            for (const auto& item : items) {
                std::cout << "| " << std::left << std::setw(3) << item.visibleId
                     << std::setw(24) << item.kioskType
                     << std::setw(28) << item.name
                     << std::setw(10) << item.price
                     << std::setw(8) << item.stock
                     << std::setw(12) << item.storage << "|\n";
            }
        }

        std::cout << "+----------------------------------------------------------------------------------------------+\n";
    }

    void showManagerBundles(bool refrigerationAvailable) const {
        auto items = managerBundles(refrigerationAvailable);

        std::cout << "\n+----------------------------------------------------------------------------------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(3) << "No" << std::setw(24) << "Kiosk System" << std::setw(22) << "Bundle"
             << std::setw(60) << "Contains" << std::setw(10) << "Price" << std::setw(8) << "Qty" << "|\n";
        std::cout << "+----------------------------------------------------------------------------------------------------------------------------+\n";

        if (items.empty()) {
            std::cout << "| " << std::left << std::setw(112) << "No bundles available." << "|\n";
        } else {
            for (const auto& item : items) {
                std::cout << "| " << std::left << std::setw(3) << item.visibleId
                     << std::setw(24) << item.kioskType
                     << std::setw(22) << item.name
                     << std::setw(60) << item.contents
                     << std::setw(10) << item.price
                     << std::setw(8) << item.stock << "|\n";
            }
        }

        std::cout << "+----------------------------------------------------------------------------------------------------------------------------+\n";
    }

    bool addProduct(const Product& p) {
        if (role != "MANAGER") return false;
        inventory.addProduct(p);
        return true;
    }

    bool addBundle(const Bundle& b) {
        if (role != "MANAGER") return false;
        inventory.addBundle(b);
        return true;
    }

    bool restockByVisibleId(int visibleId, int qty) {
        if (role != "MANAGER") return false;
        auto items = managerProducts();
        for (const auto& item : items) {
            if (item.visibleId == visibleId) {
                return inventory.restockProduct(item.key, qty);
            }
        }
        return false;
    }

    bool removeByVisibleId(int visibleId) {
        if (role != "MANAGER") return false;
        auto items = managerProducts();
        for (const auto& item : items) {
            if (item.visibleId == visibleId) {
                return inventory.removeProduct(item.key);
            }
        }
        return false;
    }

    bool purchaseByVisibleId(const std::string& kioskType, bool refrigerationAvailable, int visibleId, int qty, VisibleItem& selected) {
        auto items = customerItems(kioskType, refrigerationAvailable);
        for (const auto& item : items) {
            if (item.visibleId == visibleId) {
                selected = item;
                return inventory.purchaseVisibleItem(item, qty);
            }
        }
        return false;
    }

    const Inventory& raw() const { return inventory; }
};

} // namespace aura
