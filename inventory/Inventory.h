#pragma once
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "Item.h"

namespace aura {

class Inventory {
private:
    std::map<std::string, Product> products;
    std::map<std::string, Bundle> bundles;

public:
    void addProduct(const Product& p) {
        products[p.key] = p;
    }

    void addBundle(const Bundle& b) {
        bundles[b.key] = b;
    }

    Product* getProduct(const std::string& key) {
        auto it = products.find(key);
        return it == products.end() ? nullptr : &it->second;
    }

    const Product* getProductConst(const std::string& key) const {
        auto it = products.find(key);
        return it == products.end() ? nullptr : &it->second;
    }

    Bundle* getBundle(const std::string& key) {
        auto it = bundles.find(key);
        return it == bundles.end() ? nullptr : &it->second;
    }

    const Bundle* getBundleConst(const std::string& key) const {
        auto it = bundles.find(key);
        return it == bundles.end() ? nullptr : &it->second;
    }

    bool restockProduct(const std::string& key, int qty) {
        Product* p = getProduct(key);
        if (!p) return false;
        p->stock += qty;
        return true;
    }

    bool removeProduct(const std::string& key) {
        auto it = products.find(key);
        if (it == products.end()) return false;
        products.erase(it);

        std::vector<std::string> toDelete;
        for (const auto& b : bundles) {
            for (const auto& part : b.second.parts) {
                if (part.productKey == key) {
                    toDelete.push_back(b.first);
                    break;
                }
            }
        }
        for (const auto& keyToDelete : toDelete) bundles.erase(keyToDelete);

        return true;
    }

    bool bundleNeedsCold(const Bundle& bundle) const {
        for (const auto& part : bundle.parts) {
            const Product* p = getProductConst(part.productKey);
            if (p && p->needsRefrigeration) return true;
        }
        return false;
    }

    int bundlePrice(const Bundle& bundle) const {
        int total = 0;
        for (const auto& part : bundle.parts) {
            const Product* p = getProductConst(part.productKey);
            if (p) total += p->price * part.qty;
        }
        return total;
    }

    int bundleStock(const Bundle& bundle, bool refrigerationAvailable) const {
        if (bundle.parts.empty()) return 0;
        int answer = 1000000000;

        for (const auto& part : bundle.parts) {
            const Product* p = getProductConst(part.productKey);
            if (!p) return 0;
            if (p->needsRefrigeration && !refrigerationAvailable) return 0;
            answer = std::min(answer, p->stock / part.qty);
        }

        return answer == 1000000000 ? 0 : answer;
    }

    std::string bundleContents(const Bundle& bundle) const {
        std::ostringstream out;
        for (size_t i = 0; i < bundle.parts.size(); ++i) {
            const auto& part = bundle.parts[i];
            const Product* p = getProductConst(part.productKey);
            if (part.qty > 1) out << part.qty << " ";
            out << (p ? p->name : part.productKey);
            if (i + 1 < bundle.parts.size()) out << " + ";
        }
        return out.str();
    }

    std::vector<VisibleItem> customerItems(const std::string& kioskType, bool refrigerationAvailable) const {
        std::vector<VisibleItem> items;
        int id = 1;

        for (const auto& p : products) {
            if (p.second.kioskType != kioskType) continue;
            if (p.second.needsRefrigeration && !refrigerationAvailable) continue;
            if (p.second.stock <= 0) continue;

            items.push_back({false, id++, p.second.key, p.second.kioskType, p.second.name,
                             p.second.price, p.second.stock,
                             p.second.needsRefrigeration ? "Cold" : "Normal", ""});
        }

        for (const auto& b : bundles) {
            if (b.second.kioskType != kioskType) continue;
            int stock = bundleStock(b.second, refrigerationAvailable);
            if (stock <= 0) continue;

            items.push_back({true, id++, b.second.key, b.second.kioskType, b.second.name,
                             bundlePrice(b.second), stock,
                             bundleNeedsCold(b.second) ? "Cold" : "Normal",
                             bundleContents(b.second)});
        }

        return items;
    }

    std::vector<VisibleItem> managerProducts() const {
        std::vector<VisibleItem> items;
        int id = 1;

        for (const auto& p : products) {
            if (p.second.stock <= 0) continue;
            items.push_back({false, id++, p.second.key, p.second.kioskType, p.second.name,
                             p.second.price, p.second.stock,
                             p.second.needsRefrigeration ? "Cold" : "Normal", ""});
        }

        return items;
    }

    std::vector<VisibleItem> managerBundles(bool refrigerationAvailable) const {
        std::vector<VisibleItem> items;
        int id = 1;

        for (const auto& b : bundles) {
            int stock = bundleStock(b.second, refrigerationAvailable);
            if (stock <= 0) continue;

            items.push_back({true, id++, b.second.key, b.second.kioskType, b.second.name,
                             bundlePrice(b.second), stock,
                             bundleNeedsCold(b.second) ? "Cold" : "Normal",
                             bundleContents(b.second)});
        }

        return items;
    }

    bool purchaseVisibleItem(const VisibleItem& item, int qty) {
        if (!item.isBundle) {
            Product* p = getProduct(item.key);
            if (!p || p->stock < qty) return false;
            p->stock -= qty;
            return true;
        }

        Bundle* b = getBundle(item.key);
        if (!b) return false;

        for (const auto& part : b->parts) {
            Product* p = getProduct(part.productKey);
            if (!p || p->stock < part.qty * qty) return false;
        }

        for (const auto& part : b->parts) {
            Product* p = getProduct(part.productKey);
            p->stock -= part.qty * qty;
        }

        return true;
    }

    const std::map<std::string, Product>& rawProducts() const { return products; }
    const std::map<std::string, Bundle>& rawBundles() const { return bundles; }
};

} // namespace aura
