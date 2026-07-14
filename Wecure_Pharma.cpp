#include <iostream>
#include <string>
using namespace std;

// Abstract base class for common product interface
class Product
{
protected:
    int id;
    string name, company;
    float price;

public:
    virtual void input() = 0;         // Pure virtual function to input product data
    virtual void display() const = 0; // Pure virtual function to display product info
    virtual int getId() const = 0;    // Pure virtual function to return product ID
    virtual ~Product() {}             // Virtual destructor
};

// Derived class for Medicine
class Medicine : public Product
{
private:
    string expiry;
    int quantity;

public:
    // Input medicine details
    void input() override
    {
        cout << "\nEnter ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin >> ws;
        getline(cin, name);
        cout << "Enter Company: ";
        getline(cin, company);
        cout << "Enter Expiry (MM/YYYY): ";
        cin >> expiry;
        cout << "Enter Quantity: ";
        cin >> quantity;
        cout << "Enter Price (per unit): ";
        cin >> price;
    }

    
    void display() const override
    {
        cout << id;
        if (id < 10)
            cout << "       ";
        else if (id < 100)
            cout << "      ";
        else
            cout << "     ";

        cout << name;
        for (int i = 0; i < 16 - name.length(); i++)
            cout << " ";

        cout << company;
        for (int i = 0; i < 16 - company.length(); i++)
            cout << " ";

        cout << expiry << "     ";

        if (quantity < 10)
            cout << quantity << "       ";
        else if (quantity < 100)
            cout << quantity << "      ";
        else
            cout << quantity << "     ";

        cout << price << "     ";

        float total = price * quantity;
        cout << total << "\n";
    }

    // Check if the medicine is expired based on current date
    bool isExpired(const string &currentDate) const
    {
        int cm = (currentDate[0] - '0') * 10 + (currentDate[1] - '0');
        int cy = stoi(currentDate.substr(3));
        int em = (expiry[0] - '0') * 10 + (expiry[1] - '0');
        int ey = stoi(expiry.substr(3));
        return (cy > ey) || (cy == ey && cm >= em);
    }

    // Return the ID of the medicine
    int getId() const override
    {
        return id;
    }

    // Update medicine quantity (+ve to add, -ve to reduce)
    void updateQuantity(int change)
    {
        quantity += change;
        if (quantity < 0)
            quantity = 0; 
    }
};

// Main system class to manage medicines
class PharmacySystem
{
private:
    Product **products; // Array of pointers to Product (polymorphic base)
    int count, capacity;

public:
    // Constructor to allocate memory
    PharmacySystem(int size)
    {
        capacity = size;
        count = 0;
        products = new Product *[capacity];
    }

    // Destructor to free allocated memory
    ~PharmacySystem()
    {
        for (int i = 0; i < count; i++)
            delete products[i];
        delete[] products;
    }

    // Add a new medicine to inventory
    void addMedicine()
    {
        if (count < capacity)
        {
            products[count] = new Medicine(); // Create new Medicine
            products[count++]->input();       // Get input and increment count
            cout << "\nMedicine added successfully!\n";
        }
        else
        {
            cout << "\nInventory is full.\n";
        }
    }

    // Display all stored medicines
    void displayAll() const
    {
        if (count == 0)
        {
            cout << "\nNo medicines in inventory.\n";
            return;
        }
        cout << "\n--- Wecure Pharmastatic - All Medicines ---\n";
        cout << "ID      Name            Company         Expiry     Qty     Price   Total\n";
        for (int i = 0; i < count; i++)
            products[i]->display();
    }

    // Show expired medicines based on current date
    void checkExpiry(const string &currentDate) const
    {
        bool foundExpired = false;
        cout << "\n--- Wecure Pharmastatic - Expired Medicines ---\n";
        cout << "ID      Name            Company         Expiry     Qty     Price   Total\n";
        for (int i = 0; i < count; i++)
        {
            Medicine *med = dynamic_cast<Medicine *>(products[i]); // Cast to Medicine
            if (med && med->isExpired(currentDate))
            {
                med->display();
                foundExpired = true;
            }
        }
        if (!foundExpired)
            cout << "No expired medicines found.\n";
    }

    // Edit quantity of a specific medicine
    void editQuantity()
    {
        int id, change;
        cout << "\nEnter Medicine ID: ";
        cin >> id;
        for (int i = 0; i < count; i++)
        {
            Medicine *med = dynamic_cast<Medicine *>(products[i]);
            if (med && med->getId() == id)
            {
                cout << "Enter quantity change (+ to add, - to remove): ";
                cin >> change;
                med->updateQuantity(change);
                cout << "Quantity updated.\n";
                return;
            }
        }
        cout << "Medicine not found.\n";
    }

    // Delete a medicine from inventory
    void deleteMedicine()
    {
        int id;
        cout << "\nEnter Medicine ID to delete: ";
        cin >> id;
        for (int i = 0; i < count; i++)
        {
            if (products[i]->getId() == id)
            {
                delete products[i]; // Free memory
                // Shift elements to fill the gap
                for (int j = i; j < count - 1; j++)
                    products[j] = products[j + 1];
                count--;
                cout << "Medicine deleted.\n";
                return;
            }
        }
        cout << "Medicine not found.\n";
    }
};

// Main menu loop
int main()
{
    // Welcome message from "Wecure Pharmastatic"
    cout << "\n--- Welcome to Wecure Pharmastatic Pharmacy Management System ---\n";
    cout << "Your trusted partner for quality medicines and healthcare.\n\n";

    PharmacySystem system(100); // System with capacity of 100 medicines
    int choice;
    string currentDate;

    do
    {
        cout << "\n========== Wecure Pharmastatic - Pharmacy Management ==========\n";
        cout << "1. Add Medicine\n2. Display All\n3. Edit Quantity\n";
        cout << "4. Delete Medicine\n5. Check Expiry\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.addMedicine();
            break;
        case 2:
            system.displayAll();
            break;
        case 3:
            system.editQuantity();
            break;
        case 4:
            system.deleteMedicine();
            break;
        case 5:
            cout << "Enter current date (MM/YYYY): ";
            cin >> currentDate;
            system.checkExpiry(currentDate);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    // Thank you message when exiting
    cout << "\nThank you for using Wecure Pharmastatic Pharmacy Management System.\n";
    cout << "We hope to continue serving you with quality medicines!\n";

    return 0;
}