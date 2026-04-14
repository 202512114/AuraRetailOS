#pragma once
#include "../inventory/Inventory.h"

namespace aura {

inline void seedInventory(Inventory& inventory) {
    inventory.addProduct({"P1", "Pain Relief", "Pharmacy Kiosk", 25, 50, false});
    inventory.addProduct({"P2", "Insulin Pack", "Pharmacy Kiosk", 45, 20, true});
    inventory.addProduct({"P3", "Bandage Roll", "Pharmacy Kiosk", 15, 30, false});
    inventory.addProduct({"P4", "Antiseptic", "Pharmacy Kiosk", 35, 25, false});

    inventory.addProduct({"F1", "Sandwich", "Food Kiosk", 20, 30, false});
    inventory.addProduct({"F2", "Juice Bottle", "Food Kiosk", 35, 15, true});
    inventory.addProduct({"F3", "Biscuit Pack", "Food Kiosk", 10, 60, false});
    inventory.addProduct({"F4", "Water Bottle", "Food Kiosk", 20, 70, false});

    inventory.addProduct({"E1", "Emergency Blanket", "Emergency Relief Kiosk", 60, 18, false});
    inventory.addProduct({"E2", "Energy Bar", "Emergency Relief Kiosk", 12, 90, false});
    inventory.addProduct({"E3", "Water Bottle", "Emergency Relief Kiosk", 20, 80, false});
    inventory.addProduct({"E4", "First Aid Pouch", "Emergency Relief Kiosk", 40, 20, false});

    Bundle firstAid;
    firstAid.key = "BP1";
    firstAid.name = "First Aid Kit";
    firstAid.kioskType = "Pharmacy Kiosk";
    firstAid.parts.push_back({"P3", 1});
    firstAid.parts.push_back({"P4", 1});
    firstAid.parts.push_back({"P1", 1});
    inventory.addBundle(firstAid);

    Bundle snackCombo;
    snackCombo.key = "BF1";
    snackCombo.name = "Snack Combo";
    snackCombo.kioskType = "Food Kiosk";
    snackCombo.parts.push_back({"F3", 1});
    snackCombo.parts.push_back({"F4", 2});
    inventory.addBundle(snackCombo);

    Bundle survivalPack;
    survivalPack.key = "BE1";
    survivalPack.name = "Survival Pack";
    survivalPack.kioskType = "Emergency Relief Kiosk";
    survivalPack.parts.push_back({"E1", 1});
    survivalPack.parts.push_back({"E2", 2});
    survivalPack.parts.push_back({"E3", 2});
    inventory.addBundle(survivalPack);
}

} // namespace aura
