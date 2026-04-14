#pragma once
#include <memory>
#include "Kiosk.h"

namespace aura {

struct KioskBuilder {
    static std::unique_ptr<Kiosk> makePharmacy(InventoryProxy& proxy) {
        return std::unique_ptr<Kiosk>(new Kiosk(
            "Pharmacy Kiosk",
            proxy,
            std::unique_ptr<Dispenser>(new RoboticDispenser()),
            std::unique_ptr<PricingPolicy>(new StandardPricing())
        ));
    }

    static std::unique_ptr<Kiosk> makeFood(InventoryProxy& proxy) {
        return std::unique_ptr<Kiosk>(new Kiosk(
            "Food Kiosk",
            proxy,
            std::unique_ptr<Dispenser>(new SpiralDispenser()),
            std::unique_ptr<PricingPolicy>(new DiscountPricing())
        ));
    }

    static std::unique_ptr<Kiosk> makeEmergency(InventoryProxy& proxy) {
        return std::unique_ptr<Kiosk>(new Kiosk(
            "Emergency Relief Kiosk",
            proxy,
            std::unique_ptr<Dispenser>(new ConveyorDispenser()),
            std::unique_ptr<PricingPolicy>(new StandardPricing())
        ));
    }
};

} // namespace aura
