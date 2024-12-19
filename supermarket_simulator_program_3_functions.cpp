#include "supermarket_simulator_program_3.h"
/*********************************************************************
File name: supermarket_simulator_v3.cpp
Author:Pranaya Poudel(ddf85)
Date: 12/03/2024

Purpose:
     simulating operations in a supermarket and lets users browse aisles, buy products, control employees, and generate reports.
    Customer transaction record keeping, inventory handling, payroll processing, reporting generating and store functionality such as memberships are managed by the program.

Command Parameters:
    None

Input:
    - Users pick aisles, Items and quantities of items to purchase.
    - To Load a store data  (aisles, items, employees).
    - Others can either apply to join the store membership and get discounts.
    Employee salaries, restock amounts and customer details are provided by store managers.

Results:
    - The aisles are browsable and customers can interactively make purchases and checkout.
    Financials, pay employees, display aisles, generate sales and inventory reports … Store managers can view everything.
    
Notes:
    - Tracked aisles, inventory, employees and memberships.
    It updates store funds based on transactions, payroll, restocking.
    Membership discounts are taken into account and sales tax is applied.
    - Full supermarket simulation, with detailed reports and import of external data.
*****************************************************/



/*********************************************************************
string promptForFilename()
Purpose:
    Helper function ask the user for a valid file path
Parameters:
    -
Return Value:
    Valid filepath string
Notes:
    -
***********************************/
string promptForFilename()
{
    string szFilename;

    do
    {
        cout << "Please enter file path: ";
        getline(cin, szFilename);

        // Check for exit command
        if (szFilename == "EXIT" || szFilename == "exit")
        {
            return "EXIT";
        }

        // Check if the filename ends with .txt and has no spaces
        bool isValidFile = szFilename.length() >= 4 &&
                           szFilename.substr(szFilename.length() - 4) == ".txt" &&
                           szFilename.find(' ') == string::npos;

        if (isValidFile)
        {
            return szFilename;
        }

        cout << "Please enter a valid filepath" << endl;

    } while (true);
}

/*********************************************************************
void processSupermarketInformation(fstream& fileInput, Supermarket& myStore)
Purpose:
    Function to read supermarket text file and process the information
    into a supermarket structure
Parameters:
    I/O fstream& fileInput       File stream to read supermarket info
    I/O Supermarket& myStore     Supermarket structure to populate
Return Value:
    -
Notes:
    This function does not validate the file structure it is provided
*********************************************************************/
void processSupermarketInformation(fstream &fileInput, Supermarket &myStore) {
    // Prompt for file
    string szFilename = promptForFilename();
    if (szFilename == "EXIT") {
        cout << "Exiting function due to early exiting of file prompt." << endl;
        return;
    }

    // Open file
    fileInput.open(szFilename, ios::in);
    if (!fileInput.is_open()) {
        cout << "File could not be opened. Please try again." << endl;
        return;
    }

    // Resetting supermarket structure
    myStore = Supermarket();

    // Parse general supermarket information
    do {
        getline(fileInput, myStore.szName);       // Name
        getline(fileInput, myStore.szStoreHours); // Store Hours
        fileInput >> myStore.dTotalFunds;         // Total Funds
        fileInput.ignore();                       // Clear newline
        fileInput >> myStore.dMembershipFee;      // Membership Fee
        fileInput.ignore();                       // Clear newline
        string szLine;
        getline(fileInput, szLine);               // Skipping "Aisle Information"
    } while (false); // Ensures this executes once while maintaining a `do-while` structure

    // Parse aisles and items
    string szLine;
    int iAisleIndex = 0;
    do {
        while (getline(fileInput, szLine) && szLine != "*Employee Information*") {
            if (szLine.find("Aisle") != string::npos) {
                myStore.aislesArr[iAisleIndex].szName = szLine.substr(szLine.find(":") + 2);
                int iItemIndex = 0;

                while (getline(fileInput, szLine) && szLine != "############################") {
                    istringstream issItem(szLine);
                    Item &currentItem = myStore.aislesArr[iAisleIndex].itemArr[iItemIndex];
                    issItem >> currentItem.szName >> currentItem.dWholesale >> currentItem.dRegularPrice >> currentItem.dMembersPrice;

                    // Replace underscores with spaces
                    for (char &c : currentItem.szName) {
                        switch (c) {
                            case '_':
                                c = ' ';
                                break;
                            default:
                                break;
                        }
                    }
                    iItemIndex++;
                }
                iAisleIndex++;
            }
        }
    } while (false); // Single execution block for do-while structure

    // Parse employees
    int iEmployeeIndex = 0;
    while (getline(fileInput, szLine)) {
        istringstream issEmployee(szLine);
        Employee &currentEmployee = myStore.employeesArr[iEmployeeIndex];
        issEmployee >> currentEmployee.szName >> currentEmployee.szID >> currentEmployee.dSalary;

        // Replace underscores with spaces
        for (char &c : currentEmployee.szName) {
            switch (c) {
                case '_':
                    c = ' ';
                    break;
                default:
                    break;
            }
        }
        iEmployeeIndex++;
    }

    // Updating employee count
    myStore.iCurrentEmployees = iEmployeeIndex;

    // Close the file
    fileInput.close();
}

/*********************************************************************
void displayMenu(string szMenuName, string szChoicesArr[], int iChoices)
Purpose:
    Function to display the menu choices of a provided menu
Parameters:
    I   string szMenuName       Title of the displayed menu
    I   string szChoicesArr     Menu choices to be displayed
    I   int iChoices            Number of menu choices    
Return Value:
    -
Notes:
    Menu options are displayed starting at 1
    The last menu option should always be displayed as -1
*********************************************************************/
void displayMenu(string szMenuName, string szChoicesArr[], int iChoices)
{
    // Print top border
    cout << szMenuName << endl;
    cout << szBreakMessage;
    
    // Display menu choices
    for (int i = 0; i < iChoices - 1; i++)
    {
        cout << i + 1 << ". " << szChoicesArr[i] << endl;
    }
    
    // Always display the exit option as -1
    if (szMenuName == "\nEmployee Menu") 
    { 
        cout << "-1. Exit admin menu" << endl; } 
    else { 
        cout << "-1. Exit program" << endl;
    }
    
    
    // Print bottom border
    cout << szBreakMessage;
}

/*********************************************************************
void displaySupermarketInfo(const Supermarket myStore)
Purpose:
    Function to display basic supermarket information
Parameters:
    I   Supermarket myStore   Populated Supermarket info   
Return Value:
    -
Notes:
    -
*********************************************************************/
void displaySupermarketInfo(const Supermarket myStore) {
    // Display supermarket name and general information
    cout << myStore.szName << "'s Information" << endl;
    cout << szBreakMessage;

    // Display store hours
    cout << "Hours: " << myStore.szStoreHours << endl;

    // Display membership fee (formatted correctly with consistent spacing)
    cout << "Membership fee: " << endl;
    cout << "$" << fixed << setprecision(2) << myStore.dMembershipFee << endl;

    // Display total number of employees (with additional formatting for alignment)
    cout << "Total employees: " ;
    cout << myStore.iCurrentEmployees << endl;

    cout << szBreakMessage;
}

/*********************************************************************
void displayAisles(const Supermarket myStore)
Purpose:
    Function to display all aisles in the supermarket
Parameters:
    I   Supermarket myStore   Populated Supermarket info   
Return Value:
    -
Notes:
    Movies are displayed starting at 0
*********************************************************************/
void displayAisles(const Supermarket myStore)
{
    cout << myStore.szName << "'s Aisles" << endl;
    cout << szBreakMessage;
    for (int i = 0; i < 40; i++)
    {
        if (!myStore.aislesArr[i].szName.empty())
        {
            cout << "Aisle " << i << ": " << myStore.aislesArr[i].szName << endl;
        }
    }
    cout << szBreakMessage;
}


/*********************************************************************
void displayItems(const Supermarket myStore, int iAisleIndex)
Purpose:
    Function to display all aisles in the supermarket
Parameters:
    I   Supermarket myStore     Populated Supermarket info   
    I   int iAisleIndex         Index of the aisle to display items
Return Value:
    -
Notes:
    Movies are displayed starting at 0
*********************************************************************/
void displayItems(const Supermarket myStore, int iAisleIndex)
{
    if (iAisleIndex < 0 || iAisleIndex >= 40 || myStore.aislesArr[iAisleIndex].szName.empty())
    {
        cout << "Invalid Aisle Index." << endl;
        return;
    }

    Aisle aisle = myStore.aislesArr[iAisleIndex];
    cout << "Aisle " << iAisleIndex << ": " << aisle.szName << endl;
    cout << szBreakMessage;

    for (int i = 0; i < 100; i++)
    {
        if (!aisle.itemArr[i].szName.empty())
        {
            Item item = aisle.itemArr[i];
            cout << endl;
            cout << item.szName << endl;
            cout << "Item Quantity: " << item.iQuantity << endl;
            cout << "Regular Price: " << item.dRegularPrice << endl;
            cout << "Member  Price: " << item.dMembersPrice << endl;
        }
    }
    cout << szBreakMessage;
}

/*********************************************************************
CustomerPurchase checkout(Supermarket& myStore)
Purpose:
    Function to handle customer buying store items
Parameters:
    I/O Supermarket& myStore   Populated Supermarket info 
Return Value:
    Populated CustomerPurchase if transaction was successful
    Empty CustomerPurchase if transaction was unsuccessful
Notes:
    -
*********************************************************************/
CustomerPurchase checkout(Supermarket& myStore)
{
    CustomerPurchase tempCust;

    // Step 1: Ask for the customer’s name.
    cout << "Please enter your name: ";
    cin.ignore();  // Clear the newline character left in the buffer
    getline(cin, tempCust.szName);

    // Step 2: Check membership status.
    bool bisMember = false;
    for (int i = 0; i < myStore.iCurrentMembers; i++)
    {
        if (myStore.membersArr[i].szName == tempCust.szName)
        {
            bisMember = true;
            cout << "Welcome back " << tempCust.szName << endl;
            break;
        }
    }

    double dcartTotal = 0.0;

    if (!bisMember)
    {
        cout << "Would you like to become a member (y/n)? ";
        char response;
        cin >> response;
        if (response == 'y')
        {
            tempCust.bBoughtMembership = true;
            myStore.membersArr[myStore.iCurrentMembers++] = {tempCust.szName};
            bisMember = true;

            // Add membership fee to cart total
            dcartTotal += myStore.dMembershipFee;
        }
    }

    // Step 3: Shopping process.
    while (true)
    {
        cout << "\nCurrent cart total: " << fixed << setprecision(2) << dcartTotal << endl << endl;
        displayAisles(myStore);
        cout << "Please enter aisle index (-1 to exit): ";
        int iaisleIndex;
        cin >> iaisleIndex;

        switch (iaisleIndex)
        {
            case -1:
            {
                double dtotalCostWithTax = dcartTotal * (1 + dSALES_TAX);
                tempCust.dPurchaseCost = dtotalCostWithTax;

                cout << "\nYour total is $" << fixed << setprecision(2) << dtotalCostWithTax << endl;
                return tempCust;
            }

            default:
                if (iaisleIndex < 0 || iaisleIndex >= 40 || myStore.aislesArr[iaisleIndex].szName.empty())
                {
                    cout << "Invalid Aisle Index. Please try again." << endl;
                    break;
                }

                displayItems(myStore, iaisleIndex);
                cout << "Enter item to buy: ";
                cin.ignore();  // Clear the newline character left in the buffer
                string szitemName;
                getline(cin, szitemName);

                // Replace underscores with spaces in item names manually
                for (size_t i = 0; i < szitemName.length(); ++i)
                {
                    if (szitemName[i] == '_')
                    {
                        szitemName[i] = ' ';
                    }
                }

                bool bitemFound = false;
                for (int i = 0; i < 100; i++)
                {
                    Item& item = myStore.aislesArr[iaisleIndex].itemArr[i];
                    if (item.szName == szitemName)
                    {
                        bitemFound = true;
                        if (item.iQuantity > 0)
                        {
                            cout << "Please enter valid quantity: ";
                            int iquantity;
                            cin >> iquantity;

                            if (iquantity > 0 && iquantity <= item.iQuantity)
                            {
                                double dprice = bisMember ? item.dMembersPrice : item.dRegularPrice;
                                dcartTotal += dprice * iquantity;
                                item.iQuantity -= iquantity;
                                tempCust.iNumberItems += iquantity;
                                tempCust.dPurchaseCost += dprice * iquantity;

                                // Calculate and add profit to the supermarket's funds
                                double dprofitPerItem = dprice - item.dWholesale; // Profit per item sold
                                double dtotalProfit = dcartTotal - item.dWholesale * iquantity;
                                myStore.dTotalFunds += dtotalProfit; // Add the profit to the supermarket's total funds
                            }
                            else
                            {
                                cout << "Invalid Quantity." << endl;
                            }
                        }
                        else
                        {
                            cout << szitemName << " out of stock" << endl;
                        }
                        break;
                    }
                }

                if (!bitemFound)
                {
                    cout << "No item " << szitemName << " found." << endl;
                }
                break;
        }
    }

    // This return will only be reached if there's an error
    return tempCust;
}


/*********************************************************************
void displayTotalFunds(const Supermarket& myStore)
Purpose:
    Function to display the total funds of the store
Parameters:
    I   Supermarket myStore   Populated Supermarket info   
Return Value:
    -
Notes:
    This function should only be called by an employee
*********************************************************************/
void displayTotalFunds(const Supermarket& myStore)
{
    cout << myStore.szName << "'s Total Funds" << endl;
    cout << szBreakMessage;
    cout << "Funds: $" << fixed << setprecision(2) << myStore.dTotalFunds << endl;
    cout << szBreakMessage;
}


/*********************************************************************
double calculatedTotalSales(const Supermarket& myStore)
Purpose:
    Function to calculate the total sales of the supermarket
Parameters:
    I   Supermarket myStore   Populated Supermarket info   
Return Value:
    Total sales
Notes:
    This function should only be called by an employee
*********************************************************************/
double calculateTotalSales(const Supermarket& myStore)
{
    double dtotalSales = 0.0;

    for (int i = 0; i < myStore.iCurrentCustomers; i++)
    {
        double purchaseCost = myStore.customersArr[i].dPurchaseCost;
        if (purchaseCost > 0) {
            dtotalSales += purchaseCost / (1 + dSALES_TAX);
        }
    }

    return dtotalSales;
}


/*********************************************************************
bool payEmployees(Supermarket& myStore)
Purpose:
    Function to calculate the total sales of the supermarket
Parameters:
    I/O   Supermarket myStore   Populated Supermarket info   
Return Value:
    True if funds were successfully subtracted, else false. 
Notes:
    This function should only be called by an employee
*********************************************************************/
bool payEmployees(Supermarket& myStore)
{
    double dtotalSalaries = 0.0;

    for (int i = 0; i < myStore.iCurrentEmployees; i++)
    {
        dtotalSalaries = dtotalSalaries + myStore.employeesArr[i].dSalary;
    }

    if (myStore.dTotalFunds < dtotalSalaries)
    {
        return false;
    }

    myStore.dTotalFunds = myStore.dTotalFunds - dtotalSalaries;
    return true;
}

/*********************************************************************
void clearMembers(Supermarket& myStore);
Purpose:
    Function to clear the members from the supermarket
Parameters:
    I/O   Supermarket myStore   Populated Supermarket info   
Return Value:
    -
Notes:
    A cleared member should set back to a default state.
    This function should only be called by an employee.
    This does not clear the stored text file.
*********************************************************************/
void clearMembers(Supermarket& myStore)
{
    myStore.iCurrentMembers = 0;
    for (int i = 0; i < 200; i++)
    {
        myStore.membersArr[i] = Member();
    }
}

void extraCredit(Supermarket& myStore)
{
    cout << "Not implemented yet" << endl;
}