# Banking Management System
---

## Project Overview
A feature-rich desktop banking application built with **C programming language** and **GTK 3** graphical user interface toolkit. The application provides a secure platform for users to create bank accounts, login, and perform basic banking operations through an intuitive graphical interface designed with **Glade**.

---

## Table of Contents
1. [Features](#features)
2. [Technology Stack](#technology-stack)
3. [System Requirements](#system-requirements)
4. [Installation Guide](#installation-guide)
5. [Project Structure](#project-structure)
6. [UI Design](#ui-design)
7. [Core Functionality](#core-functionality)
8. [Data Management](#data-management)
9. [Usage Guide](#usage-guide)
10. [Development Journey](#development-journey)
11. [Troubleshooting](#troubleshooting)
12. [Future Enhancements](#future-enhancements)
13. [License](#license)

---

## Features

### Current Features ✅
- **User Registration**: Create new accounts with personal details
- **Secure Login**: Account number and PIN authentication
- **Data Persistence**: Account information stored in text files
- **Random Generation**: Automatic 8-digit account numbers and 4-digit PINs
- **Input Validation**: Empty field checking and duplicate ID prevention
- **GUI Interface**: Professional-looking windows and dialogs

### Upcoming Features 
- Deposit and withdrawal functionality
- Fund transfers between accounts
- Transaction history with timestamps
- PIN change option
- Account statement generation

---

## Technology Stack

| Component | Technology | Purpose |
|-----------|------------|---------|
| **Language** | C (C17 standard) | Core programming language |
| **GUI Framework** | GTK 3.24 | Graphical user interface |
| **UI Designer** | Glade 3.40 | Visual interface design |
| **Compiler** | GCC (MinGW-w64) | Compilation on Windows |
| **Build System** | MSYS2 | Development environment |
| **Data Storage** | Text files (CSV format) | Account persistence |
| **Version Control** | Git | Source code management |

---

## System Requirements

### Windows
- **OS**: Windows 10 or 11 (64-bit)
- **Memory**: 2 GB RAM minimum
- **Storage**: 500 MB free space
- **Dependencies**: MSYS2 with MinGW-w64

### Linux
- **OS**: Ubuntu 20.04+ / Debian 11+ / Fedora 35+
- **Memory**: 1 GB RAM minimum
- **Storage**: 200 MB free space
- **Dependencies**: GTK 3 development libraries

### Development Tools Required
- GCC compiler
- MSYS2 (Windows) or build-essential (Linux)
- pkg-config
- GTK 3 development libraries
- Glade (optional, for UI modifications)

---

## Installation Guide

### Windows Installation (using MSYS2)

#### Step 1: Install MSYS2
1. Download MSYS2 from [msys2.org](https://www.msys2.org/)
2. Run installer (default location: `C:\msys64`)
3. Open **MSYS2 MinGW 64-bit** from Start Menu

#### Step 2: Install GTK and Tools
```bash
# Update package database
pacman -Syu

# Install GTK3 and development tools
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-pkg-config
pacman -S mingw-w64-x86_64-gcc
```

#### Step 3: Clone or Download Project
```bash
# Navigate to your projects folder
cd /c/Users/YourUsername/projects/

# Clone repository
git clone https://github.com/MawiraVictor/Banking-management-system.git
cd Banking-management-system
```

#### Step 4: Compile the Application
```bash
gcc banking.c -o banking.exe `pkg-config --cflags --libs gtk+-3.0`
```

#### Step 5: Run the Application
```bash
./banking.exe
```

### Linux Installation (Ubuntu/Debian)

#### Step 1: Install Dependencies
```bash
sudo apt update
sudo apt install libgtk-3-dev gcc make pkg-config git
```

#### Step 2: Clone and Compile
```bash
git clone https://github.com/MawiraVictor/Banking-management-system.git
cd Banking-management-system
gcc banking.c -o banking `pkg-config --cflags --libs gtk+-3.0`
```

#### Step 3: Run
```bash
./banking
```

---

## Project Structure

```
Banking-management-system/
│
├── banking.c                # Main application source code
├── accounts.txt             # Account data storage (created at runtime)
│
├── ui/                       # Glade UI files
│   ├── banking.ui           # Main window interface
│   ├── login.ui             # Login dialog
│   └── register.ui          # Registration dialog
│
├── headers/                  # Header files (for modular version)
│   ├── account.h
│   ├── transactions.h
│   ├── history.h
│   └── utils.h
│
├── build.bat                 # Windows build script
├── build.sh                  # Linux build script
├── LICENSE                   # MIT License
└── README.md                 # Project overview
```

---

## UI Design

### Main Window (`ui/banking.ui`)
```
┌─────────────────────────────────┐
│    PAMOJA COMMERCIAL BANK       │
│                 │
│                                 │
│    Welcome to ATM Services      │
│                                 │
│    ┌────────┐  ┌────────┐       │
│    │ LOGIN  │  │REGISTER│       │
│    └────────┘  └────────┘       │
└─────────────────────────────────┘
```

**Widget IDs:**
- `welcomeWindow` - Main application window
- `btnLogin` - Login button
- `btnRegister` - Register button

### Login Dialog (`ui/login.ui`)
```
┌─────────────────────────────────┐
│      Login to Your Account      │
│                                 │
│    Account Number: _________    │
│                                 │
│    PIN:           [****]        │
│                                 │
│    ┌────────┐  ┌────────┐       │
│    │ Cancel │  │ Login  │       │
│    └────────┘  └────────┘       │
└─────────────────────────────────┘
```

**Widget IDs:**
- `loginDialog` - Dialog window
- `entryAccount` - Account number entry
- `entryPin` - PIN entry (masked)
- `btnLoginSubmit` - Login button
- `btnLoginCancel` - Cancel button

### Registration Dialog (`ui/register.ui`)
```
┌─────────────────────────────────┐
│        Create New Account       │
│                                 │
│    Full Name:  [___________]    │
│    ID/Passport:[___________]    │
│    KRA PIN:    [___________]    │
│    Phone:      [___________]    │
│    Deposit:    [___________]    │
│                                 │
│    ┌────────┐  ┌────────┐       │
│    │ Cancel │  │Register│       │
│    └────────┘  └────────┘       │
└─────────────────────────────────┘
```

**Widget IDs:**
- `registerDialog` - Dialog window
- `entryFullName` - Full name entry
- `entryID` - ID/Passport number
- `entryKRA` - KRA PIN entry
- `entryPhone` - Phone number entry
- `entryDeposit` - Initial deposit amount
- `btnRegisterSubmit` - Register button
- `btnRegisterCancel` - Cancel button

---

## Core Functionality

### Data Structures

```c
typedef struct {
    char account_number[15];  // 8-digit account number
    char pin[5];              // 4-digit PIN
    char full_name[50];       // Customer full name
    char id_number[20];       // National ID/Passport
    char kra_pin[15];         // KRA PIN
    char phone[15];           // Phone number
    float balance;            // Account balance
} Account;
```

### Key Functions

#### Registration Functions
| Function | Description |
|----------|-------------|
| `generate_account_number()` | Creates random 8-digit account number |
| `generate_pin()` | Creates random 4-digit PIN |
| `account_exists()` | Checks if ID is already registered |
| `save_account()` | Writes account data to file |
| `on_register_submit_clicked()` | Handles registration form submission |
| `on_register_cancel_clicked()` | Clears and closes registration dialog |

#### Login Functions
| Function | Description |
|----------|-------------|
| `on_login_submit_clicked()` | Validates credentials against file |
| `on_login_cancel_clicked()` | Clears and closes login dialog |
| `load_login_dialog()` | Loads login UI and connects signals |

#### Utility Functions
| Function | Description |
|----------|-------------|
| `show_error()` | Displays error message dialog |
| `show_info()` | Displays information dialog |
| `load_registration_dialog()` | Loads registration UI |

---

## Data Management

### File Format (`accounts.txt`)
Accounts are stored in CSV format with the following fields:
```
account_number,pin,full_name,id_number,kra_pin,balance
```

**Example:**
```
12345678,1234,John Doe,12345678,A001234567,1000.00
87654321,5678,Jane Smith,87654321,B009876543,2500.50
```

### File Operations

**Reading Accounts:**
```c
FILE *file = fopen("accounts.txt", "r");
while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%f",
           acc_no, pin, name, id, kra, &balance);
}
fclose(file);
```

**Writing New Account:**
```c
FILE *file = fopen("accounts.txt", "a");
fprintf(file, "%s,%s,%s,%s,%s,%.2f\n",
        acc->account_number, acc->pin, acc->full_name,
        acc->id_number, acc->kra_pin, acc->balance);
fclose(file);
```

---

## Usage Guide

### 1. Starting the Application
```bash
# Windows
./banking.exe

# Linux
./banking
```

### 2. Registering a New Account
1. Click **REGISTER** button on main window
2. Fill in all required fields:
   - Full Name
   - ID/Passport Number
   - KRA PIN
   - Phone Number
   - Initial Deposit
3. Click **Register**
4. Note your generated **Account Number** and **PIN**

### 3. Logging In
1. Click **LOGIN** button
2. Enter your 8-digit Account Number
3. Enter your 4-digit PIN
4. Click **Login**

### 4. Successful Login
Upon successful authentication, you'll see a confirmation message. (Dashboard features coming soon!)

---

## Development Journey

### Challenges Overcome
| Challenge | Solution |
|-----------|----------|
| OneDrive permission issues | Moved project outside OneDrive folder |
| GTK header not found | Used correct MSYS2 MinGW 64-bit terminal |
| Buttons not working | Replaced GtkLabel with GtkButton widgets |
| Dialog not found | Set proper widget IDs in Glade |
| Signal connection errors | Verified widget types match signal types |

### Key Learning Points
1. **GTK initialization** requires proper environment setup
2. **Glade IDs** must exactly match those used in code
3. **Widget types** matter - Labels cannot receive click signals
4. **File paths** should use forward slashes even on Windows
5. **Memory management** is handled automatically by GTK

---

## Troubleshooting

### Common Issues and Solutions

#### Issue: "gtk/gtk.h: No such file or directory"
**Solution:** Ensure you're using MSYS2 MinGW 64-bit terminal and GTK is installed:
```bash
pacman -S mingw-w64-x86_64-gtk3
```

#### Issue: "Permission denied" when reading UI files
**Solution:** Move project out of OneDrive folder or ensure files are fully downloaded locally.

#### Issue: Buttons don't respond to clicks
**Solution:** Check widget types in Glade - ensure they are `GtkButton`, not `GtkLabel` or `GtkButtonBox`.

#### Issue: Dialog doesn't appear
**Solution:** Verify widget IDs match between Glade and code using:
```bash
grep -E 'id="' ui/*.ui
```

#### Issue: Compilation fails with undefined references
**Solution:** Ensure pkg-config flags are properly included:
```bash
gcc banking.c -o banking.exe `pkg-config --cflags --libs gtk+-3.0`
```

#### Issue: Account file not being created
**Solution:** Check write permissions in project directory. The file will be created automatically on first registration.

---

## Future Enhancements

### Planned Features
- [ ] **Account Dashboard** - View balance and account details
- [ ] **Deposit Money** - Add funds to account
- [ ] **Withdraw Money** - Remove funds with validation
- [ ] **Transfer Funds** - Send money between accounts
- [ ] **Transaction History** - View past transactions with timestamps
- [ ] **PIN Change** - Allow users to change their PIN
- [ ] **Account Statements** - Generate PDF statements
- [ ] **Multiple Accounts** - Support for multiple accounts per user

### Technical Improvements
- [ ] Encrypt sensitive data (PINs) in storage
- [ ] Add database support (SQLite)
- [ ] Implement multi-threading for better responsiveness
- [ ] Add unit tests
- [ ] Create installers for Windows and Linux
- [ ] Add theming support

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2026 Victor Mawira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
```

---

## Author

**Victor Mawira**
- GitHub: [@MawiraVictor](https://github.com/MawiraVictor)
- Medium: [@mawiravictor](https://medium.com/@vickmawira66)
- LinkedIn: [Victor Mawira](https://www.linkedin.com/in/victor-mawira-841a672b0/)

---

## Acknowledgments

- GTK Development Team
- MSYS2 Project
- Open Source Community

---

*Documentation last updated: March 2026*