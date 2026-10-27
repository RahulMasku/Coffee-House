# Coffee House Management System — C++ Edition

A modern C++ clone of the original Coffee House Management System.

The original C project is intentionally kept unchanged. This is a separate implementation designed to demonstrate modern C++ programming, OOP, STL data structures, order processing, inventory management, and reporting.

## Features
- Product/menu management
- Product search by name/category
- Customer order creation and cart aggregation
- Stock validation and automatic inventory deduction
- Cash, Card, and UPI payment selection
- Automatic 5% / 10% discounts
- Order history and order-status workflow
- Admin dashboard
- Product creation and restocking
- Low-stock inventory report
- Sales and product-sales report
- Validated console input

## C++ concepts
- Classes and encapsulation
- Structures
- vector and map
- STL algorithms and lambdas
- References and pointers
- Range-based loops
- const correctness
- Searching, filtering and aggregation

## Build

Windows / MinGW:
g++ -std=c++17 -O2 coffee_house.cpp -o coffee_house.exe
coffee_house.exe

Linux/macOS:
g++ -std=c++17 -O2 coffee_house.cpp -o coffee_house
./coffee_house

## Admin login
Password: 9014

## Architecture
Customer -> Browse/Search -> Cart -> Stock Validation -> Pricing/Discount -> Payment -> Inventory Deduction -> Order Tracking

Admin -> Product Management -> Inventory -> Orders -> Sales Reporting

## Complexity
- Product lookup by ID: O(n)
- Menu display: O(n)
- Product search: O(n)
- Add/find cart item: O(n)
- Sales aggregation: O(total order items)

A future iteration can use unordered_map for average O(1) product-ID lookup and SQL persistence.

## Repository structure
Coffee-House/
- Coffee House Management System.c       (original, unchanged)
- COFFEE HOUSE.exe                       (original, unchanged)
- README.md                              (original, unchanged)
- Coffee-House-CPP/
  - coffee_house.cpp
  - README.md
