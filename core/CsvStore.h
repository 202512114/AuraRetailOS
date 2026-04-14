#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "../common/Registry.h"
#include "../inventory/Inventory.h"

namespace aura {

struct CsvStore {
    static void saveProducts(const Inventory& inv, const std::string& fileName) {
        std::ofstream out(fileName.c_str());
        out << "key,name,kiosk,price,stock,cold\n";
        for (const auto& p : inv.rawProducts()) {
            out << p.second.key << "," << p.second.name << "," << p.second.kioskType << "," << p.second.price
                << "," << p.second.stock << "," << (p.second.needsRefrigeration ? 1 : 0) << "\n";
        }
    }

    static void saveBundles(const Inventory& inv, const std::string& fileName) {
        std::ofstream out(fileName.c_str());
        out << "key,name,kiosk,contents\n";
        for (const auto& b : inv.rawBundles()) {
            out << b.second.key << "," << b.second.name << "," << b.second.kioskType << "," << inv.bundleContents(b.second) << "\n";
        }
    }

    static void saveTransactions(const std::vector<Transaction>& txs, const std::string& fileName) {
        std::ofstream out(fileName.c_str());
        out << "id,item,quantity,amount,paymentMethod,refunded\n";
        for (const auto& tx : txs) {
            out << tx.id << "," << tx.itemName << "," << tx.quantity << "," << tx.amount
                << "," << tx.paymentMethod << "," << (tx.refunded ? 1 : 0) << "\n";
        }
    }

    static void saveConfig(const std::vector<std::pair<std::string, std::string>>& cfg, const std::string& fileName) {
        std::ofstream out(fileName.c_str());
        out << "key,value\n";
        for (const auto& kv : cfg) out << kv.first << "," << kv.second << "\n";
    }

    static void saveConfigJson(const std::vector<std::pair<std::string, std::string>>& cfg, const std::string& fileName) {
        std::ofstream out(fileName.c_str());
        out << "{\n";
        for (size_t i = 0; i < cfg.size(); ++i) {
            out << "  \"" << cfg[i].first << "\": \"" << cfg[i].second << "\"";
            if (i + 1 < cfg.size()) out << ",";
            out << "\n";
        }
        out << "}\n";
    }
};

} // namespace aura
