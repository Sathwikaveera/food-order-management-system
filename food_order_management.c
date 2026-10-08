#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define FILENAME "credentials.txt"
#define MAX_NAME 100
#define MAX_ITEMS 10
#define MAX_RESTAURANTS 3

typedef struct {
    char name[MAX_NAME];
    float price;
} MenuItem;

typedef struct {
    char name[MAX_NAME];
    MenuItem menu[MAX_ITEMS];
    int num_items;
} Restaurant;

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} User;

typedef struct {
    MenuItem items[MAX_ITEMS];
    int quantity[MAX_ITEMS];
    int item_count;
    int restaurant_index;
} Order;

int checkUsernameExists(char *username) {
    FILE *file;
    char file_username[MAX_USERNAME], file_password[MAX_PASSWORD];

    file = fopen(FILENAME, "r");
    if (file == NULL) return 0;

    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
        if (strcmp(username, file_username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void saveOrderHistory(char *username, Order *order) {
    FILE *file;
    char filename[MAX_NAME];
    snprintf(filename, sizeof(filename), "%s_order_history.txt", username);

    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening order history file.\n");
        return;
    }

    fprintf(file, "\n--- New Order ---\n");
    float total = 0.0;
    for (int i = 0; i < order->item_count; i++) {
        float cost = order->items[i].price * order->quantity[i];
        fprintf(file, "%d x %s - $%.2f\n", order->quantity[i], order->items[i].name, cost);
        total += cost;
    }
    fprintf(file, "Total Bill: $%.2f\n", total);

    fclose(file);
    printf("✅ Order history saved!\n");
}

int validatePassword(char *password) {
    int len = strlen(password);
    if (len < 8) {
        printf("❌ Password must be at least 8 characters long.\n");
        return 0;
    }

    int has_digit = 0, has_alpha = 0;
    for (int i = 0; i < len; i++) {
        if (isdigit(password[i])) has_digit = 1;
        if (isalpha(password[i])) has_alpha = 1;
    }

    if (!has_digit || !has_alpha) {
        printf("❌ Password must contain both letters and digits.\n");
        return 0;
    }

    return 1;
}

void signup() {
    FILE *file;
    char username[MAX_USERNAME], password[MAX_PASSWORD];

    printf("Enter a username: ");
    scanf("%49s", username);

    if (checkUsernameExists(username)) {
        printf("❌ Username already exists!\n");
        return;
    }

    do {
        printf("Enter a password: ");
        scanf("%49s", password);
    } while (!validatePassword(password));

    file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error opening credentials file.\n");
        return;
    }

    fprintf(file, "%s %s\n", username, password);
    printf("✅ Signup successful! You can now log in.\n");
    fclose(file);
}

int login(char *username) {
    FILE *file;
    char password[MAX_PASSWORD];
    char file_username[MAX_USERNAME], file_password[MAX_PASSWORD];
    int found = 0;

    printf("Enter your username: ");
    scanf("%49s", username);

    printf("Enter your password: ");
    scanf("%49s", password);

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("❌ No users registered yet.\n");
        return 0;
    }

    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
            printf("✅ Login successful!\n");
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found) printf("❌ Invalid username or password.\n");
    return found;
}

void displayMenu(Restaurant *restaurant) {
    printf("\n🍽️ Menu for %s:\n", restaurant->name);
    for (int i = 0; i < restaurant->num_items; i++) {
        printf("%d. %s - $%.2f\n", i + 1, restaurant->menu[i].name, restaurant->menu[i].price);
    }
}

void initializeRestaurants(Restaurant restaurants[]) {
    strcpy(restaurants[0].name, "Pizza Place");
    restaurants[0].num_items = 3;
    strcpy(restaurants[0].menu[0].name, "Margherita Pizza"); restaurants[0].menu[0].price = 12.99;
    strcpy(restaurants[0].menu[1].name, "Pepperoni Pizza"); restaurants[0].menu[1].price = 14.99;
    strcpy(restaurants[0].menu[2].name, "Veggie Pizza"); restaurants[0].menu[2].price = 13.99;

    strcpy(restaurants[1].name, "Burger Joint");
    restaurants[1].num_items = 3;
    strcpy(restaurants[1].menu[0].name, "Cheeseburger"); restaurants[1].menu[0].price = 9.99;
    strcpy(restaurants[1].menu[1].name, "Bacon Burger"); restaurants[1].menu[1].price = 11.99;
    strcpy(restaurants[1].menu[2].name, "Veggie Burger"); restaurants[1].menu[2].price = 10.99;

    strcpy(restaurants[2].name, "Sushi Spot");
    restaurants[2].num_items = 3;
    strcpy(restaurants[2].menu[0].name, "California Roll"); restaurants[2].menu[0].price = 8.99;
    strcpy(restaurants[2].menu[1].name, "Spicy Tuna Roll"); restaurants[2].menu[1].price = 10.99;
    strcpy(restaurants[2].menu[2].name, "Salmon Roll"); restaurants[2].menu[2].price = 9.99;
}

void orderFood(Restaurant restaurants[], Order *order) {
    int restaurantChoice;

    if (order->item_count == 0) {
        printf("\nSelect a Restaurant:\n");
        for (int i = 0; i < MAX_RESTAURANTS; i++) {
            printf("%d. %s\n", i + 1, restaurants[i].name);
        }
        printf("Enter your choice: ");
        scanf("%d", &restaurantChoice);

        if (restaurantChoice <= 0 || restaurantChoice > MAX_RESTAURANTS) {
            printf("❌ Invalid restaurant choice!\n");
            return;
        }

        order->restaurant_index = restaurantChoice - 1;
    } else {
        printf("⚠️ You already have an order from %s. Finish or submit it first.\n",
               restaurants[order->restaurant_index].name);
        return;
    }

    Restaurant *selectedRestaurant = &restaurants[order->restaurant_index];
    displayMenu(selectedRestaurant);

    int foodChoice, quantity;
    printf("Enter the item number to order (0 to finish): ");
    scanf("%d", &foodChoice);

    while (foodChoice != 0) {
        if (order->item_count >= MAX_ITEMS) {
            printf("⚠️ Maximum items reached!\n");
            break;
        }

        if (foodChoice > 0 && foodChoice <= selectedRestaurant->num_items) {
            printf("Enter quantity: ");
            scanf("%d", &quantity);

            order->items[order->item_count] = selectedRestaurant->menu[foodChoice - 1];
            order->quantity[order->item_count] = quantity;
            order->item_count++;
        } else {
            printf("❌ Invalid item number.\n");
        }
        printf("Enter the item number to order (0 to finish): ");
        scanf("%d", &foodChoice);
    }
}

int main() {
    Restaurant restaurants[MAX_RESTAURANTS];
    int choice;
    int loggedIn = 0;
    char username[MAX_USERNAME];
    Order currentOrder = {0};

    initializeRestaurants(restaurants);

    while (1) {
        printf("\n--- 🍕 Online Food Order System ---\n");

        if (!loggedIn) {
            printf("1. Login\n");
            printf("2. Signup\n");
        }
        printf("3. Order Food\n");
        printf("4. View Bill\n");
        printf("5. Submit Order\n");
        printf("6. Logout\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (!loggedIn) loggedIn = login(username);
                else printf("⚠️ You are already logged in.\n");
                break;
            case 2:
                if (!loggedIn) signup();
                else printf("⚠️ You are already logged in.\n");
                break;
            case 3:
                if (loggedIn) orderFood(restaurants, &currentOrder);
                else printf("❌ Please log in first.\n");
                break;
            case 4:
                if (loggedIn) {
                    float bill = 0.0;
                    printf("\n🧾 Current Order:\n");
                    for (int i = 0; i < currentOrder.item_count; i++) {
                        float cost = currentOrder.items[i].price * currentOrder.quantity[i];
                        printf("%d x %s - $%.2f\n", currentOrder.quantity[i], currentOrder.items[i].name, cost);
                        bill += cost;
                    }
                    printf("Total Bill: $%.2f\n", bill);
                } else {
                    printf("❌ Please log in first.\n");
                }
                break;
            case 5:
                if (loggedIn) {
                    if (currentOrder.item_count > 0) {
                        saveOrderHistory(username, &currentOrder);
                        memset(&currentOrder, 0, sizeof(Order));
                    } else {
                        printf("⚠️ No items in your order to submit.\n");
                    }
                } else {
                    printf("❌ Please log in first.\n");
                }
                break;
            case 6:
                if (loggedIn) {
                    loggedIn = 0;
                    memset(username, 0, sizeof(username));
                    memset(&currentOrder, 0, sizeof(Order));
                    printf("✅ Logged out successfully.\n");
                } else {
                    printf("⚠️ You are not logged in.\n");
                }
                break;
            case 7:
                printf("👋 Exiting... Thank you!\n");
                return 0;
            default:
                printf("❌ Invalid choice. Try again.\n");
        }
    }

    return 0;
}