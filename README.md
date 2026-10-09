# Food Order Management System

A C-based food ordering application that allows users to register, log in, browse restaurant menus, place orders, calculate bills, and save order history.

## Features

- User signup and login
- Username availability check
- Password validation during signup
- Multiple restaurant menus
- Food item selection and quantity entry
- Automatic bill calculation
- Submit orders and save order history
- Logout and exit options

## Technologies Used

- C Programming
- Structures
- Functions
- File Handling
- String Manipulation
- Standard C Libraries

## Restaurants

The application includes three restaurants:

- Pizza Place
- Burger Joint
- Sushi Spot

Each restaurant offers three food items with individual prices.

## How It Works

1. Register a new account or log in.
2. Select a restaurant.
3. Browse the menu and choose food items.
4. Enter the quantity for each item.
5. View the bill.
6. Submit the order to save the order history.
7. Log out or exit the application.

## How to Run

1. Install a C compiler such as GCC.
2. Download or clone this repository.
3. Open a terminal in the project folder.
4. Compile the program:

```bash
gcc food_order_management.c -o food_order_management
```

5. Run the program:

```bash
./food_order_management
```

On Windows, run:

```bash
food_order_management.exe
```

## File Handling

The program creates `credentials.txt` to store registered usernames and passwords, and a separate order history text file for each user.

**Security Note:** This project is for educational purposes. Passwords are stored as plain text, so the authentication system should not be used for real-world applications.

## Project Highlights

This project demonstrates C programming fundamentals, structures, functions, file handling, user authentication, menu management, billing, and order history management.
