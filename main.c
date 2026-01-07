#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 100
#define TAX_RATE 0.05
#define DISCOUNT_LIMIT 5000
#define DISCOUNT_RATE 0.10

// ---------------- STRUCT ----------------
struct Item {
    int id;
    char name[30];
    float price;
    int quantity;
    int sold;
};

struct Item inventory[MAX_ITEMS];
int itemCount = 0;

// Summary variables
int totalCustomers = 0;
float totalRevenue = 0;

// ---------------- FUNCTION DECLARATIONS ----------------
void menu();                     // recursive menu
void addItem();
void updateItem();
int searchItem();
void displayInventory();
void sortItems();
void generateBill();
float applyDiscount(float);
void saveToFile();
void loadFromFile();
void showSummaryReport();

// ---------------- MAIN ----------------
int main() {
    loadFromFile();
    menu();
    return 0;
}

// ---------------- MENU (RECURSIVE) ----------------
void menu() {
    int choice;

    printf("\n===== IMTIAZ SUPERMARKET SYSTEM =====\n");
    printf("1. Add Item\n");
    printf("2. Update Item\n");
    printf("3. Search Item\n");
    printf("4. Display Inventory\n");
    printf("5. Sort Items\n");
    printf("6. Generate Bill\n");
    printf("7. Save Data\n");
    printf("8. Summary Report\n");
    printf("9. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: addItem(); break;
        case 2: updateItem(); break;
        case 3: searchItem(); break;
        case 4: displayInventory(); break;
        case 5: sortItems(); break;
        case 6: generateBill(); break;
        case 7: saveToFile(); break;
        case 8: showSummaryReport(); break;
        case 9: saveToFile(); return;
        default: printf("Invalid choice!\n");
    }

    menu(); // recursive call
}

// ---------------- ADD ITEM ----------------
void addItem() {
    if(itemCount >= MAX_ITEMS) {
        printf("Inventory full!\n");
        return;
    }

    printf("Enter Item ID: ");
    scanf("%d", &inventory[itemCount].id);
    printf("Enter Item Name: ");
    scanf("%s", inventory[itemCount].name);
    printf("Enter Price: ");
    scanf("%f", &inventory[itemCount].price);
    printf("Enter Quantity: ");
    scanf("%d", &inventory[itemCount].quantity);

    inventory[itemCount].sold = 0;
    itemCount++;

    printf("Item added successfully!\n");
}

// ---------------- UPDATE ITEM ----------------
void updateItem() {
    int id, i;
    printf("Enter Item ID to update: ");
    scanf("%d", &id);

    for(i = 0; i < itemCount; i++) {
        if(inventory[i].id == id) {
            printf("Enter new price: ");
            scanf("%f", &inventory[i].price);
            printf("Enter new quantity: ");
            scanf("%d", &inventory[i].quantity);
            printf("Item updated!\n");
            return;
        }
    }
    printf("Item not found!\n");
}

// ---------------- SEARCH ITEM ----------------
int searchItem() {
    int id, i;
    printf("Enter Item ID to search: ");
    scanf("%d", &id);

    for(i = 0; i < itemCount; i++) {
        if(inventory[i].id == id) {
            printf("Found: %s | Price: %.2f | Qty: %d\n",
                   inventory[i].name,
                   inventory[i].price,
                   inventory[i].quantity);
            return i;
        }
    }
    printf("Item not found!\n");
    return -1;
}

// ---------------- DISPLAY INVENTORY ----------------
void displayInventory() {
    int i;
    printf("\nID\tName\tPrice\tQty\n");
    for(i = 0; i < itemCount; i++) {
        printf("%d\t%s\t%.2f\t%d\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].price,
               inventory[i].quantity);
    }
}

// ---------------- SORT ITEMS (BUBBLE SORT) ----------------
void sortItems() {
    int i, j;
    struct Item temp;

    for(i = 0; i < itemCount - 1; i++) {
        for(j = 0; j < itemCount - i - 1; j++) {
            if(strcmp(inventory[j].name, inventory[j + 1].name) > 0) {
                temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
    printf("Items sorted by name!\n");
}

// ---------------- GENERATE BILL ----------------
void generateBill() {
    int id, qty, index;
    float subtotal = 0, tax, discount, total;
    char choice;

    totalCustomers++;

    do {
        printf("Enter Item ID: ");
        scanf("%d", &id);

        index = -1;
        for(int i = 0; i < itemCount; i++) {
            if(inventory[i].id == id) {
                index = i;
                break;
            }
        }

        if(index == -1) {
            printf("Item not found!\n");
        } else {
            printf("Enter quantity: ");
            scanf("%d", &qty);

            if(qty <= inventory[index].quantity) {
                inventory[index].quantity -= qty;
                inventory[index].sold += qty;
                subtotal += inventory[index].price * qty;
            } else {
                printf("Insufficient stock!\n");
            }
        }

        printf("Add more items? (y/n): ");
        scanf(" %c", &choice);

    } while(choice == 'y' || choice == 'Y');

    tax = subtotal * TAX_RATE;
    discount = applyDiscount(subtotal);
    total = subtotal + tax - discount;

    totalRevenue += total;

    printf("\nSubtotal: %.2f", subtotal);
    printf("\nTax: %.2f", tax);
    printf("\nDiscount: %.2f", discount);
    printf("\nFinal Amount: %.2f\n", total);

    FILE *fp = fopen("Sales.txt", "a");
    fprintf(fp, "Bill Amount: %.2f\n", total);
    fclose(fp);
}

// ---------------- APPLY DISCOUNT ----------------
float applyDiscount(float amount) {
    if(amount > DISCOUNT_LIMIT)
        return amount * DISCOUNT_RATE;
    return 0;
}

// ---------------- SAVE TO FILE ----------------
void saveToFile() {
    FILE *fp = fopen("Inventory.txt", "w");
    for(int i = 0; i < itemCount; i++) {
        fprintf(fp, "%d %s %.2f %d %d\n",
                inventory[i].id,
                inventory[i].name,
                inventory[i].price,
                inventory[i].quantity,
                inventory[i].sold);
    }
    fclose(fp);
    printf("Data saved successfully!\n");
}

// ---------------- LOAD FROM FILE ----------------
void loadFromFile() {
    FILE *fp = fopen("Inventory.txt", "r");
    if(fp == NULL) return;

    while(fscanf(fp, "%d %s %f %d %d",
          &inventory[itemCount].id,
          inventory[itemCount].name,
          &inventory[itemCount].price,
          &inventory[itemCount].quantity,
          &inventory[itemCount].sold) != EOF) {
        itemCount++;
    }
    fclose(fp);
}

// ---------------- SUMMARY REPORT ----------------
void showSummaryReport() {
    int i, maxIndex = 0;
    int totalItemsSold = 0;

    for(i = 0; i < itemCount; i++) {
        totalItemsSold += inventory[i].sold;
        if(inventory[i].sold > inventory[maxIndex].sold)
            maxIndex = i;
    }

    printf("\n===== END OF DAY REPORT =====\n");
    printf("Total Customers: %d\n", totalCustomers);
    printf("Total Items Sold: %d\n", totalItemsSold);
    printf("Total Revenue: %.2f\n", totalRevenue);

    if(itemCount > 0)
        printf("Highest Selling Item: %s\n", inventory[maxIndex].name);

    if(totalCustomers > 0)
        printf("Average Bill: %.2f\n", totalRevenue / totalCustomers);
}
