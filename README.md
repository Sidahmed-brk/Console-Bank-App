# C++ Bank Management & User Permission System

A feature-complete C++ console application for managing bank clients, account transactions, and user access permissions using local text file database persistence.

## Key Features

### 1. Client Management
- **View Clients:** Display a formatted table of all registered client records.
- **Add New Client:** Add client details with duplicate account number prevention.
- **Update / Delete Client:** Modify client records or soft-delete accounts.
- **Search Client:** Quick lookup by Account Number.

### 2. Transactions Module
- **Deposit & Withdraw:** Execute transactions with immediate balance validation.
- **Total Balances:** View total assets held across all accounts.

### 3. User & Access Control (Bitwise Permissions)
- **Role-Based Access:** Fine-grained permissions assigned per user using bitwise flags (Client List, Add, Delete, Update, Search, Transactions, Manage Users).
- **Admin Protection:** System safeguards to prevent accidental deletion of administrative users.
- **User Operations:** Add, update, delete, search, and list internal bank system operators.

## Data Storage Format

Records are parsed and persisted using custom string delimiters (`#//#`):
- **Clients File (`Clients_File.txt`):** `AccountNum#//#PinCode#//#FullName#//#Phone#//#Balance`
- **Users File (`Users.txt`):** `UserName#//#Password#//#PermissionBitmask`

