#pragma once
#include <string>
#include <vector>

namespace aura {

struct Product {
    std::string key;
    std::string name;
    std::string kioskType;
    int price;
    int stock;
    bool needsRefrigeration;
};

struct BundlePart {
    std::string productKey;
    int qty;
};

struct Bundle {
    std::string key;
    std::string name;
    std::string kioskType;
    std::vector<BundlePart> parts;
};

struct VisibleItem {
    bool isBundle;
    int visibleId;
    std::string key;
    std::string kioskType;
    std::string name;
    int price;
    int stock;
    std::string storage;
    std::string contents;
};

struct Transaction {
    std::string id;
    std::string itemName;
    int quantity;
    int amount;
    std::string paymentMethod;
    bool refunded;
};

} // namespace aura
