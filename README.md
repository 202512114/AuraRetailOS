# AuraRetailOS

Professional folder-structured version of the uploaded single-file Aura Retail OS project.

## Run
Compile only `main.cpp`:

```bash
g++ -std=c++17 main.cpp -o AuraRetailOS
./AuraRetailOS
```

## Stored Data
The program stores required project data in the `data/` folder:
- `inventory_products.csv`
- `inventory_bundles.csv`
- `transactions.csv`
- `config.csv`
- `config.json`

These files are created/updated automatically on startup, save, and exit.
