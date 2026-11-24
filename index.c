#include <stdio.h>
#include <string.h>
// define name of items 
char itemnames[50][30] = {
    "potato", "tomato", "onion", "carrot",
    "cabbage", "cauliflower", "sugar", "rice",
    "beans", "lentils", "flour", "oil"
};
float itemprices[50] = {
    30.0, 40.0, 50.0, 60.0,
    70.0, 80.0, 90.0, 100.0,
    110.0, 120.0, 130.0, 140.0
};
float itemtaxes[50] = {
    0.05, 0.04, 0.03, 0.02,
    0.01, 0.02, 0.01, 0.02,
    0.03, 0.04, 0.03, 0.02
};
int totalitems = 12;

// function informations
void displaymenu();
void addnewitem();
int getitemdetails(int itemno, float *price, float *tax);
void applydiscount(char code, float *billtotal, float *taxtotal);
void savelog(float billtotal, float taxtotal, char code);

int main() {
    float weight, priceperkg, taxrate;
    float itemtotal, itemtax;
    float grandtotalprice = 0.0;
    float grandtotaltax = 0.0;
    int itemno;
    char choice, hasdiscount, addmore;
    char discountcode = 'N';

    printf("======================================\n");
    printf("        GROCERY SCALING SYSTEM        \n");
    printf("======================================\n\n");

    // ask if user wants to add new items to the list
    printf("Do you want to add new items to the menu? (y/n): ");
    scanf(" %c", &addmore);

    while (addmore == 'y' || addmore == 'Y') {
        addnewitem();
        printf("Do you want to add another item? (y/n): ");
        scanf(" %c", &addmore);
    }

    printf("\n");
    displaymenu();

    // main shopping loop
    do {
        printf("Enter the item number to buy: ");
        scanf("%d", &itemno);

        if (getitemdetails(itemno, &priceperkg, &taxrate) == 0) {
            printf("Invalid item number. Please choose from the menu.\n\n");
            choice = 'y';
            continue;
        }

        printf("Enter weight in kg: ");
        scanf("%f", &weight);
        if (weight <= 0) {
            printf("Weight must be greater than 0. Try again.\n\n");
            choice = 'y';
            continue;
        }

        // calculate cost and tax for this item
        itemtotal = weight * priceperkg;
        itemtax = itemtotal * taxrate;

        printf("\nItem: %s\n", itemnames[itemno - 1]);
        printf("Weight: %.2f kg\n", weight);
        printf("Price per kg: %.2f\n", priceperkg);
        printf("Item price: %.2f\n", itemtotal);
        printf("Item tax: %.2f\n", itemtax);
        printf("--------------------------------------\n");

        grandtotalprice += itemtotal;
        grandtotaltax += itemtax;

        printf("Want to buy another item? (y/n): ");
        scanf(" %c", &choice);
        printf("\n");

    } while (choice == 'y' || choice == 'Y');

    // check for discount code
    printf("======================================\n");
    printf("Do you have a discount code? (y/n): ");
    scanf(" %c", &hasdiscount);

    if (hasdiscount == 'y' || hasdiscount == 'Y') {
        printf("Enter discount code (A or B): ");
        scanf(" %c", &discountcode);
        applydiscount(discountcode, &grandtotalprice, &grandtotaltax);
    }

    // print final receipt
    printf("\n============ FINAL RECEIPT ===========\n");
    printf("Total price: %.2f\n", grandtotalprice);
    printf("Total tax: %.2f\n", grandtotaltax);
    printf("Final amount to pay: %.2f\n", grandtotalprice + grandtotaltax);
    printf("\n------ Thank you for shopping! ------\n");

    // save details to file
    savelog(grandtotalprice, grandtotaltax, discountcode);
    return 0;
}

// prints the menu with 3 items per line
void displaymenu() {
    int i;
    printf("================ MENU ================\n\n");

    for (i = 0; i < totalitems; i++) {
        printf("%d. %s (%.1f/kg)", i + 1, itemnames[i], itemprices[i]);

        if ((i + 1) % 3 == 0 || i == totalitems - 1) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
    printf("\n");
}

// adds a new item at the end of the arrays
void addnewitem() {
    if (totalitems >= 50) {
        printf("Menu is full. Cannot add more items.\n");
        return;
    }

    printf("\nEnter item name: ");
    scanf("%s", itemnames[totalitems]);

    printf("Enter price per kg: ");
    scanf("%f", &itemprices[totalitems]);

    printf("Enter tax rate (e.g. 0.05 for 5%%): ");
    scanf("%f", &itemtaxes[totalitems]);

    totalitems++;
    printf("Item added! Its menu number is %d.\n\n", totalitems);
}

// gets price and tax for the selected item number
int getitemdetails(int itemno, float *price, float *tax) {
    if (itemno >= 1 && itemno <= totalitems) {
        *price = itemprices[itemno - 1];
        *tax = itemtaxes[itemno - 1];
        return 1;
    }
    return 0;
}
// applies 10% off for A and 20% off for B
void applydiscount(char code, float *billtotal, float *taxtotal) {
    if (code == 'a' || code == 'A') {
        printf("Code A applied: 10%% discount!\n");
        *billtotal = *billtotal * 0.90;
        *taxtotal = *taxtotal * 0.90;
    } 
    else if (code == 'b' || code == 'B') {
        printf("Code B applied: 20%% discount!\n");
        *billtotal = *billtotal * 0.80;
        *taxtotal = *taxtotal * 0.80;
    } 
    else {
        printf("Invalid discount code. No discount given.\n");
    }
}
// saves bill summary 
void savelog(float billtotal, float taxtotal, char code) {
    FILE *fp = fopen("transactionlog.txt", "a");
    if (fp == NULL) {
        printf("Could not open log file.\n");
        return;
    }
    fprintf(fp, "--- Transaction Log ---\n");
    fprintf(fp, "Discount Code: %c\n", code);
    fprintf(fp, "Total Price: %.2f\n", billtotal);
    fprintf(fp, "Total Tax: %.2f\n", taxtotal);
    fprintf(fp, "-----------------------\n\n");
    fclose(fp);
    printf("Bill saved to transaction log.txt\n");
}