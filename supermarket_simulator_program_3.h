#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

inline string szBreakMessage = "*****************************\n";
inline double dSALES_TAX = 0.08; 

struct Item
{
    string szName;          // Name of the item
    int iQuantity = 30;     // Number of items left in stock
    double dWholesale;      // Supermarket's cost
    double dRegularPrice;   // Price for regular customers
    double dMembersPrice;   // Price for store members
};

struct Aisle
{
    string szName = "";          // Name of store aisle
    Item itemArr[100];      // Array of items on that aisle
};

struct CustomerPurchase
{
    string szName = "";             // Name of person who made purchase
    bool bBoughtMembership = false; // Bought membership on the purchase
    int iNumberItems = 0;           // Number of items they purchased 
    double dPurchaseCost = 0.0;     // How much did the purchase cost including tax
    
};

struct Member
{
    string szName = "";         // Name of person who bought membership
};

struct Employee
{
    string szID;                // ID of the employee
    string szName;              // Name of the employee
    double dSalary;             // Salary of the employee
};

struct Supermarket
{
    string szName;                      // Name of the supermarket
    string szStoreHours;                // Hold store hours info
    int iCurrentCustomers = 0;          // Current number of customers
    int iCurrentMembers = 0;            // Current number of members
    int iCurrentEmployees = 0;          // Current number of employees
    double dMembershipFee;              // Fee to become a store member
    double dTotalFunds;                 // Total funds of the store
    Aisle aislesArr[40];                // Array of supermarket aisles
    Employee employeesArr[50];          // Array of employees
    CustomerPurchase customersArr[200]; // Array of customers
    Member membersArr[200];             // Array of members
};

// Functions to Implement
string promptForFilename();
void processSupermarketInformation(fstream& fileInput, Supermarket& myStore);

void displayMenu(string szMenuName, string szChoicesArr[], int iChoices);
void displaySupermarketInfo(const Supermarket myStore);
void displayAisles(const Supermarket myStore);
void displayItems(const Supermarket myStore, int iAisleIndex);
CustomerPurchase checkout(Supermarket& myStore);

void displayTotalFunds(const Supermarket& myStore);
double calculateTotalSales(const Supermarket& myStore);
bool payEmployees(Supermarket& myStore);
void clearMembers(Supermarket& myStore);

void extraCredit(Supermarket& myStore);