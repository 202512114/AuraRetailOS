# AuraRetailOS

Aura Retail OS is a modular, object-oriented C++ project that simulates a smart retail kiosk system used in different real-world environments such as pharmacy kiosks, food kiosks, and emergency relief kiosks. The project is designed to demonstrate practical use of OOP principles, dynamic inventory handling, multiple payment integrations, configurable hardware behavior, transaction-based refunds, bundle-based stock deduction, and structured project-level implementation using a proper folder-based architecture.

It is not just a vending machine simulation. It is a complete kiosk management system model that shows how software can control inventory, user interaction, payment flow, hardware abstraction, and administrative operations inside a single unified retail platform.

---

## Project Objective

The main objective of Aura Retail OS is to build a realistic and extensible kiosk-based retail simulation using C++ and Object-Oriented Programming concepts.

This project is intended to show:

- how a retail kiosk system can be designed in a modular way
- how different subsystems can work together in one application
- how OOP concepts solve real-world design problems
- how design patterns improve flexibility, maintainability, and extensibility
- how a single system can support different kiosk domains with minimal change

The project especially focuses on modular architecture, inventory abstraction, multiple payment methods, hardware interchangeability, and dynamic administrative control.

---

## Real-World Idea Behind the Project

In real life, retail kiosks are used in many different places:

- pharmacies for medicines and health products
- food kiosks for snacks and beverages
- emergency relief stations for survival supplies
- smart vending kiosks in malls, airports, hospitals, and stations

Even though the domain changes, the main software needs are similar:

- show items to the customer
- allow item purchase
- process payment
- control item dispensing hardware
- update stock
- keep transaction records
- support refund handling
- allow an administrator to manage the kiosk

Aura Retail OS models exactly this idea. It creates a common software framework that can be adapted to multiple kiosk types while keeping the system modular and easy to maintain.

---

## Main Concept of the System

Aura Retail OS works as a kiosk platform with two major operating modes:

1. Customer Mode  
2. Inventory Manager Mode

In Customer Mode, a user can:

- enter the kiosk interface
- browse available items
- see only items relevant to that kiosk type
- purchase products or bundles
- choose payment method
- get automatic transaction ID generation
- view kiosk information such as dispenser, pricing policy, and attached modules

In Inventory Manager Mode, an authorized manager can:

- view all available products across kiosk systems
- view all bundles across kiosk systems
- add new products
- create new bundles
- restock product quantities
- remove products
- attach hardware modules
- change dispenser hardware
- run diagnostics
- process refunds using transaction IDs
- view transaction history

This separation makes the project realistic, because customers and managers naturally have different roles and access levels in a retail system.

---

## Kiosk Types Implemented

The system supports multiple kiosk types. Each kiosk type has its own environment, its own items, and its own default hardware and pricing behavior.

### 1. Pharmacy Kiosk
This kiosk is used for medical and health-related items.

Example items:
- Pain Relief
- Insulin Pack
- Bandage Roll
- Antiseptic

Example bundle:
- First Aid Kit

Default behavior:
- uses Robotic Arm Dispenser
- uses Standard Pricing

Reason:
Pharmacy products may require careful and precise dispensing, so robotic hardware is more suitable.

---

### 2. Food Kiosk
This kiosk is used for consumable food and beverage items.

Example items:
- Sandwich
- Juice Bottle
- Biscuit Pack
- Water Bottle

Example bundle:
- Snack Combo

Default behavior:
- uses Spiral Dispenser
- uses Discount Pricing

Reason:
Food vending systems often use spiral dispenser mechanisms, and food items may sometimes use promotional or discounted pricing.

---

### 3. Emergency Relief Kiosk
This kiosk is used for survival and emergency-use products.

Example items:
- Emergency Blanket
- Energy Bar
- Water Bottle
- First Aid Pouch

Example bundle:
- Survival Pack

Default behavior:
- uses Conveyor Dispenser
- uses Standard Pricing

Reason:
Emergency items may be larger or packaged differently, so a conveyor-type dispenser makes more sense.

---

## Core Functional Areas

### 1. Inventory Management
The inventory system is one of the most important parts of the project. It stores all products and bundles and keeps track of live stock values.

The system manages:

- product name
- kiosk type
- price
- quantity
- refrigeration requirement
- bundle composition

Inventory is dynamic. It changes whenever:

- a product is purchased
- a bundle is purchased
- a product is restocked
- a product is removed
- a new product is added
- a new bundle is created

This makes the project more realistic than a static menu-based simulation.

---

### 2. Bundle Management
The project supports bundles in addition to normal products.

A bundle is a grouped retail item made from multiple products.

Examples:
- First Aid Kit
- Snack Combo
- Survival Pack

A bundle stores:
- its own name
- its kiosk type
- a list of contained products
- quantity required for each contained product

For example, a bundle may contain:
- 1 Biscuit Pack
- 2 Water Bottles

When a customer purchases a bundle:
- the system does not treat it as a separate stock unit
- instead, it reduces the stock of each contained product according to bundle composition

This is a very important real-world simulation behavior and makes the project more meaningful.

---

### 3. Payment Handling
The payment system allows customers to pay using multiple methods.

Supported methods:
- UPI
- Card
- Wallet
- NetBanking

The customer selects the payment option during purchase.  
The system processes the payment through a common payment interface.  
This makes all payment methods behave uniformly from the perspective of the main kiosk logic.

The project also supports refund processing using the same payment method originally used in the transaction.

This means:
- a purchase done using UPI is refunded via UPI
- a purchase done using Card is refunded via Card
- and so on

This increases realism and demonstrates a strong design structure.

---

### 4. Hardware Dispensing System
Aura Retail OS also simulates physical item-dispensing hardware.

Supported dispenser types:
- Spiral Dispenser
- Robotic Arm Dispenser
- Conveyor Dispenser

Each dispenser represents a different physical hardware strategy for delivering items.

The system is designed so that:
- the purchase logic remains the same
- only the dispensing behavior changes

This means the main software does not need to be rewritten when hardware changes.

That is an important practical design goal in large real-world systems.

---

### 5. Optional Hardware Modules
The system supports optional attachable modules.

Supported modules:
- Refrigeration Module
- Solar Monitoring Module
- Network Connectivity Module

These modules extend kiosk capability without changing the main kiosk structure.

Examples:
- Refrigeration Module allows cold-storage items such as Juice Bottle or Insulin Pack to be available
- Solar Monitoring Module represents sustainability or monitoring support
- Network Connectivity Module represents communication/network functionality

This proves that the system is extensible and modular.

---

### 6. Transaction Management
Every customer purchase creates a transaction record.

Each transaction stores:
- unique transaction ID
- item name
- quantity
- amount
- payment method
- refund status

This is important because the transaction system supports:
- history tracking
- refund tracking
- validation against duplicate refunds
- professional record keeping

The transaction ID is later used in manager mode to process refunds.

This creates a proper purchase-refund workflow like a real kiosk or POS system.

---

### 7. Refund Workflow
Refund is not done manually by entering amount and method separately.

Instead, the manager enters:
- the transaction ID

The system then:
- searches for the matching transaction
- checks if it exists
- checks if it is already refunded
- identifies the original payment method
- processes refund using the same payment method
- marks the transaction as refunded

This is one of the strongest practical features in the project because it connects:
- transaction history
- payment abstraction
- role-based access
- business process correctness

---

### 8. Role-Based System Behavior
The project clearly separates customer and manager behavior.

#### Customer Role
Customer can:
- browse current kiosk items only
- purchase products and bundles
- choose payment
- view kiosk information

Customer cannot:
- add products
- restock items
- remove items
- create bundles
- attach modules
- run diagnostics
- view all kiosk inventories

#### Manager Role
Manager can:
- view all products across all kiosk systems
- view all bundles across all kiosk systems
- add products
- create bundles
- restock products
- remove products
- attach modules
- replace hardware dispenser
- run diagnostics
- process refunds
- view transaction history

Manager access is protected by password:
- `admin`

This makes the project more realistic and properly role-based.

---

## Customer Interface Behavior

The customer interface is intentionally simple and realistic.

The customer does not need to:
- enter user ID
- enter password
- know internal product codes
- know system complexity

Instead, the customer simply:
- opens the customer panel
- views available items
- selects item by visible number
- chooses quantity
- selects payment method
- completes purchase

This design improves usability and simulates a real kiosk interface where the customer sees only relevant user-facing information.

---

## Manager Interface Behavior

The manager interface is more detailed because it is used for maintenance and control.

Manager options include:
- View Items
- View Bundles
- Add New Product
- Create Bundle
- Restock Product
- Remove Product
- Attach Hardware Module
- Replace Dispenser
- Run Diagnostics
- Process Refund by Transaction ID
- View Transaction History

This makes the manager panel function like an operations dashboard for kiosk maintenance.

---

## Inventory Views

### Customer View
When a customer views items:
- only items for the current kiosk system are shown
- item names are shown
- price is shown
- quantity is shown
- type is shown (Product or Bundle)
- storage type is shown (Normal or Cold)

No internal product code is shown.

This keeps the customer interface simple.

### Manager View – Products
When manager views products:
- products from all kiosk systems are visible
- kiosk system column is shown
- product name is shown
- price is shown
- stock is shown
- storage is shown

### Manager View – Bundles
When manager views bundles:
- bundles from all kiosk systems are visible
- kiosk system column is shown
- bundle name is shown
- contents are shown
- bundle price is shown
- bundle stock is shown

This separation makes the manager interface cleaner and easier to understand.

---

## OOP Principles Used

Aura Retail OS strongly demonstrates core OOP principles.

### 1. Encapsulation
Data and logic are grouped together inside classes such as:
- Inventory
- Kiosk
- InventoryProxy
- Payment classes
- Dispenser classes

This improves clarity and protects internal state.

### 2. Abstraction
The system hides implementation details behind interfaces such as:
- PaymentProcessor
- Dispenser
- KioskInterface
- Command

This allows higher-level logic to interact with general behavior instead of low-level implementation details.

### 3. Inheritance
Inheritance is used to model families of related classes such as:
- UPIAdapter, CardAdapter, WalletAdapter, NetBankingAdapter from PaymentProcessor
- SpiralDispenser, RoboticArmDispenser, ConveyorDispenser from Dispenser

This avoids code repetition and supports polymorphism.

### 4. Polymorphism
The program can treat different objects in a uniform way.

Example:
- all payment types are used through PaymentProcessor
- all dispensers are used through Dispenser

This makes the project flexible and extensible.

---

## Design Patterns Used

The project uses multiple design patterns to make the implementation modular and maintainable.

### 1. Composite Pattern
Used in inventory representation with products and bundles.

Why:
- a bundle behaves like a sellable item
- a single product also behaves like a sellable item
- both can be handled through a common item model

### 2. Proxy Pattern
Used through InventoryProxy.

Why:
- it controls access to inventory operations
- it separates customer and manager permissions
- it acts as a controlled layer between UI and inventory data

### 3. Adapter Pattern
Used for payment methods.

Why:
- UPI, Card, Wallet, and NetBanking may have different implementations
- adapters allow them to be used through one common interface

### 4. Strategy Pattern
Used through the dispenser hierarchy.

Why:
- different dispenser types implement different hardware behavior
- the kiosk can swap dispenser strategy without changing core purchase logic

### 5. Command Pattern
Used for operations like:
- purchase
- refund
- restock

Why:
- actions become separate command objects
- logic becomes more organized and extendable

### 6. Factory Pattern
Used in payment creation and kiosk creation.

Why:
- object creation is centralized
- easier to extend with new payment methods and kiosk types later

---

## Important Simulation Scenarios Covered

This project covers the required simulation scenarios in a practical way.

### 1. Adding a New Hardware Module
The manager can dynamically attach:
- Refrigeration Module
- Solar Monitoring Module
- Network Connectivity Module

Effect:
- cold items become available only if refrigeration is attached

### 2. Integrating a New Payment Provider
The project already supports:
- UPI
- Card
- Wallet
- NetBanking

This shows how the system can support multiple providers through a common payment abstraction.

### 3. Expanding Inventory Structure
The project supports:
- normal products
- bundles
- dynamic bundle creation by manager

This proves that the inventory structure is extendable.

---

## Transaction Flow

A standard purchase flow works like this:

1. Customer enters Customer Panel  
2. Customer views items  
3. Customer selects product or bundle  
4. Customer enters quantity  
5. Customer selects payment method  
6. Payment is processed  
7. Dispenser delivers the selected item  
8. Inventory is updated  
9. Transaction record is created  
10. Transaction ID is generated  

This creates a complete transaction lifecycle.

---

## Refund Flow

A standard refund flow works like this:

1. Manager enters Manager Panel  
2. Manager selects refund option  
3. Manager enters transaction ID  
4. System finds transaction  
5. System checks if already refunded  
6. System identifies original payment method  
7. Refund is processed using same method  
8. Transaction is marked refunded  

This gives a realistic refund mechanism.

---

## Data Persistence

The project saves required information into files.

Stored files:
- `inventory_products.csv`
- `inventory_bundles.csv`
- `transactions.csv`
- `config.csv`

These files are updated during save and exit.

This means the project is not only interactive but also supports file-based persistence, which is an important practical software feature.

---

## Folder-Based Project Structure

The project is organized into multiple folders for clarity and modularity.

Suggested structure:

- `main.cpp` → main entry point
- `core/` → kiosk core logic
- `inventory/` → inventory, products, bundles, proxy
- `payment/` → payment abstraction and adapters
- `hardware/` → dispenser abstraction and implementations
- `command/` → command classes
- `utils/` or `data/` → storage files

This structure makes the project easier to maintain, debug, and explain during viva.

---

## Why This Project Is Strong

Aura Retail OS is a strong OOP based project because it demonstrates:

- practical OOP usage
- multiple subsystems working together
- role-based system behavior
- realistic retail use case
- multiple design patterns
- modular architecture
- transaction and refund workflow
- dynamic inventory handling
- bundle logic
- file persistence
- clean separation of concerns

It is not just a small menu program. It is a complete simulation of how a modular kiosk software system can be designed.

---

## How to Run the Project

Compile and run using:

```bash
g++ main.cpp -o AuraRetailOS
./AuraRetailOS
