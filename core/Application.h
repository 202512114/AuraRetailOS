#pragma once
#include <memory>
#include "../common/Input.h"
#include "../common/Registry.h"
#include "../common/UI.h"
#include "../inventory/InventoryProxy.h"
#include "CsvStore.h"
#include "KioskBuilder.h"
#include "Menus.h"
#include "SeedData.h"

namespace aura {

inline int run() {
    UI::title("AURA RETAIL OS");
    std::cout << " Smart Modular Retail Kiosk Simulation\n";
    UI::line('=');

    CentralRegistry::instance().set("System", "Aura Retail OS");
    CentralRegistry::instance().set("City", "Zephyrus");
    CentralRegistry::instance().set("Mode", "Normal");

    Inventory inventory;
    seedInventory(inventory);
    InventoryProxy proxy(inventory);

    CsvStore::saveProducts(inventory, "data/inventory_products.csv");
    CsvStore::saveBundles(inventory, "data/inventory_bundles.csv");
    CsvStore::saveTransactions({}, "data/transactions.csv");
    CsvStore::saveConfig(CentralRegistry::instance().all(), "data/config.csv");
    CsvStore::saveConfigJson(CentralRegistry::instance().all(), "data/config.json");

    UI::title("KIOSK SETUP");
    std::cout << "1. Pharmacy Kiosk\n2. Food Kiosk\n3. Emergency Relief Kiosk\n";
    int setup = Input::getInt("Choice: ", 1, 3);

    std::unique_ptr<Kiosk> kiosk;
    if (setup == 1) kiosk = KioskBuilder::makePharmacy(proxy);
    else if (setup == 2) kiosk = KioskBuilder::makeFood(proxy);
    else kiosk = KioskBuilder::makeEmergency(proxy);

    std::cout << "\nKiosk launched successfully.\n";
    std::cout << "Kiosk Type       : " << kiosk->type() << "\n";
    std::cout << "Default Dispenser: " << kiosk->dispenserName() << "\n";
    std::cout << "Pricing Policy   : " << kiosk->pricingName() << "\n";

    while (true) {
        UI::title("MAIN MENU");
        std::cout << "1. Customer Panel\n2. Inventory Manager Panel\n3. Save System State\n0. Exit\n";
        int menu = Input::getInt("Choice: ", 0, 3);

        if (menu == 0) break;
        if (menu == 1) customerPanel(*kiosk);
        else if (menu == 2) managerPanel(*kiosk);
        else if (menu == 3) {
            CsvStore::saveProducts(inventory, "data/inventory_products.csv");
            CsvStore::saveBundles(inventory, "data/inventory_bundles.csv");
            CsvStore::saveTransactions(kiosk->getTransactions(), "data/transactions.csv");
            CsvStore::saveConfig(CentralRegistry::instance().all(), "data/config.csv");
            CsvStore::saveConfigJson(CentralRegistry::instance().all(), "data/config.json");
            std::cout << "System data saved successfully.\n";
        }
    }

    CsvStore::saveProducts(inventory, "data/inventory_products.csv");
    CsvStore::saveBundles(inventory, "data/inventory_bundles.csv");
    CsvStore::saveTransactions(kiosk->getTransactions(), "data/transactions.csv");
    CsvStore::saveConfig(CentralRegistry::instance().all(), "data/config.csv");
    CsvStore::saveConfigJson(CentralRegistry::instance().all(), "data/config.json");

    std::cout << "\nFinal Revenue: Rs." << kiosk->revenue() << "\n";
    std::cout << "All data saved. Exiting Aura Retail OS.\n";
    return 0;
}

} // namespace aura
