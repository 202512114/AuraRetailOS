#pragma once
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "../common/Input.h"
#include "../common/UI.h"
#include "../payment/Payment.h"
#include "../hardware/Dispenser.h"
#include "Kiosk.h"

namespace aura {

inline std::unique_ptr<PaymentProcessor> choosePayment() {
    int ch = Input::getInt("Choose payment 1.UPI 2.Card 3.Wallet 4.NetBanking 0.Cancel : ", 0, 4);
    if (ch == 0) return nullptr;
    if (ch == 1) return std::unique_ptr<PaymentProcessor>(new UPIAdapter());
    if (ch == 2) return std::unique_ptr<PaymentProcessor>(new CardAdapter());
    if (ch == 3) return std::unique_ptr<PaymentProcessor>(new WalletAdapter());
    return std::unique_ptr<PaymentProcessor>(new NetBankingAdapter());
}

inline std::unique_ptr<Dispenser> chooseDispenser() {
    int ch = Input::getInt("Choose dispenser 1.Spiral 2.Robotic Arm 3.Conveyor : ", 1, 3);
    if (ch == 1) return std::unique_ptr<Dispenser>(new SpiralDispenser());
    if (ch == 2) return std::unique_ptr<Dispenser>(new RoboticDispenser());
    return std::unique_ptr<Dispenser>(new ConveyorDispenser());
}

inline void customerPanel(Kiosk& kiosk) {
    while (true) {
        UI::title("CUSTOMER PANEL");
        std::cout << "1. Browse Items\n2. Purchase Item\n3. Kiosk Information\n0. Back\n";
        int ch = Input::getInt("Choice: ", 0, 3);
        if (ch == 0) break;

        if (ch == 1) {
            kiosk.getProxy().showCustomerItems(kiosk.type(), kiosk.refrigerationAvailable());
        } else if (ch == 2) {
            auto items = kiosk.getProxy().customerItems(kiosk.type(), kiosk.refrigerationAvailable());
            kiosk.getProxy().showCustomerItems(kiosk.type(), kiosk.refrigerationAvailable());
            if (items.empty()) continue;

            int idx = Input::getInt("Enter item number (0 cancel): ", 0, (int)items.size());
            if (idx == 0) continue;

            int qty = Input::getNonNegative("Enter quantity: ");
            if (qty <= 0) continue;

            auto payment = choosePayment();
            if (!payment) {
                std::cout << "Purchase cancelled.\n";
                continue;
            }

            if (!kiosk.purchaseVisibleItem(idx, qty, *payment)) {
                std::cout << "Purchase could not be completed.\n";
            }
        } else if (ch == 3) {
            std::cout << "Kiosk Type       : " << kiosk.type() << "\n";
            std::cout << "Active Dispenser : " << kiosk.dispenserName() << "\n";
            std::cout << "Pricing Policy   : " << kiosk.pricingName() << "\n";
            std::cout << "Attached Modules : " << kiosk.showModules() << "\n";
        }
    }
}

inline void managerPanel(Kiosk& kiosk) {
    std::string password = Input::getLine("Enter manager password: ");
    if (password != "admin") {
        std::cout << "Wrong password.\n";
        return;
    }

    kiosk.getProxy().setRole("MANAGER");

    while (true) {
        UI::title("INVENTORY MANAGER PANEL");
        std::cout << "1. View Items\n";
        std::cout << "2. View Bundles\n";
        std::cout << "3. Add New Product\n";
        std::cout << "4. Create Bundle\n";
        std::cout << "5. Restock Product\n";
        std::cout << "6. Remove Product\n";
        std::cout << "7. Attach Hardware Module\n";
        std::cout << "8. Replace Dispenser\n";
        std::cout << "9. Run Diagnostics\n";
        std::cout << "10. Process Refund by Transaction ID\n";
        std::cout << "11. View Transaction History\n";
        std::cout << "0. Back\n";

        int ch = Input::getInt("Choice: ", 0, 11);
        if (ch == 0) break;

        if (ch == 1) {
            kiosk.getProxy().showManagerProducts();
        } else if (ch == 2) {
            kiosk.getProxy().showManagerBundles(kiosk.refrigerationAvailable());
        } else if (ch == 3) {
            int ks = Input::getInt("Select kiosk system 1.Pharmacy 2.Food 3.Emergency Relief : ", 1, 3);
            std::string kioskSystem = ks == 1 ? "Pharmacy Kiosk" : ks == 2 ? "Food Kiosk" : "Emergency Relief Kiosk";

            std::string name = Input::getLine("Enter product name: ");
            int price = Input::getNonNegative("Enter price: ");
            int qty = Input::getNonNegative("Enter quantity: ");
            int cold = Input::getInt("Needs refrigeration? 1.Yes 0.No : ", 0, 1);

            static int productCounter = 1000;
            Product p;
            p.key = "N" + std::to_string(productCounter++);
            p.name = name;
            p.kioskType = kioskSystem;
            p.price = price;
            p.stock = qty;
            p.needsRefrigeration = (cold == 1);

            std::cout << (kiosk.getProxy().addProduct(p) ? "Product added successfully.\n" : "Add product failed.\n");
        } else if (ch == 4) {
            int ks = Input::getInt("Select kiosk system 1.Pharmacy 2.Food 3.Emergency Relief : ", 1, 3);
            std::string kioskSystem = ks == 1 ? "Pharmacy Kiosk" : ks == 2 ? "Food Kiosk" : "Emergency Relief Kiosk";

            auto allProducts = kiosk.getProxy().managerProducts();
            std::vector<VisibleItem> allowedProducts;
            for (const auto& item : allProducts) {
                if (item.kioskType == kioskSystem) allowedProducts.push_back(item);
            }

            if (allowedProducts.empty()) {
                std::cout << "No products available to create bundle for this kiosk.\n";
                continue;
            }

            std::cout << "\nAvailable products for bundle creation:\n";
            std::cout << "+--------------------------------------------------------------------------------+\n";
            std::cout << "| " << std::left << std::setw(3) << "No" << std::setw(42) << "Product" << std::setw(10) << "Price"
                 << std::setw(8) << "Qty" << std::setw(12) << "Storage" << "|\n";
            std::cout << "+--------------------------------------------------------------------------------+\n";
            for (size_t i = 0; i < allowedProducts.size(); ++i) {
                std::cout << "| " << std::left << std::setw(3) << i + 1
                     << std::setw(42) << allowedProducts[i].name
                     << std::setw(10) << allowedProducts[i].price
                     << std::setw(8) << allowedProducts[i].stock
                     << std::setw(12) << allowedProducts[i].storage << "|\n";
            }
            std::cout << "+--------------------------------------------------------------------------------+\n";

            std::string bundleName = Input::getLine("Enter bundle name: ");
            int count = Input::getInt("How many different items in this bundle? ", 1, 10);

            Bundle b;
            static int bundleCounter = 2000;
            b.key = "B" + std::to_string(bundleCounter++);
            b.name = bundleName;
            b.kioskType = kioskSystem;

            for (int i = 1; i <= count; ++i) {
                int itemNo = Input::getInt("Select product number for part " + std::to_string(i) + ": ", 1, (int)allowedProducts.size());
                int qty = Input::getInt("Enter quantity of selected product in bundle: ", 1, 100);
                b.parts.push_back({allowedProducts[itemNo - 1].key, qty});
            }

            std::cout << (kiosk.getProxy().addBundle(b) ? "Bundle created successfully.\n" : "Bundle creation failed.\n");
        } else if (ch == 5) {
            kiosk.getProxy().showManagerProducts();
            auto items = kiosk.getProxy().managerProducts();
            if (items.empty()) continue;

            int idx = Input::getInt("Enter product number to restock: ", 1, (int)items.size());
            int qty = Input::getNonNegative("Enter quantity to add: ");

            std::cout << (kiosk.getProxy().restockByVisibleId(idx, qty)
                     ? "Stock updated successfully.\n"
                     : "Restock failed.\n");
        } else if (ch == 6) {
            kiosk.getProxy().showManagerProducts();
            auto items = kiosk.getProxy().managerProducts();
            if (items.empty()) continue;

            int idx = Input::getInt("Enter product number to remove: ", 1, (int)items.size());

            std::cout << (kiosk.getProxy().removeByVisibleId(idx)
                     ? "Product removed successfully.\n"
                     : "Remove failed.\n");
        } else if (ch == 7) {
            int module = Input::getInt("Attach module 1.Refrigeration 2.Solar 3.Network 0.Cancel : ", 0, 3);
            if (module == 1) kiosk.attachModule("Refrigeration Module");
            else if (module == 2) kiosk.attachModule("Solar Monitoring Module");
            else if (module == 3) kiosk.attachModule("Network Connectivity Module");
        } else if (ch == 8) {
            kiosk.replaceDispenser(chooseDispenser());
        } else if (ch == 9) {
            kiosk.diagnostics();
        } else if (ch == 10) {
            std::string txId = Input::getLine("Enter transaction ID: ");
            kiosk.refundById(txId);
        } else if (ch == 11) {
            kiosk.showTransactions();
        }
    }

    kiosk.getProxy().setRole("CUSTOMER");
}

} // namespace aura
