#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;


//to pascal case
string toPascalCase(const string& str)
{
    stringstream ss(str);
    string word, result;

    while (getline(ss, word, ' ')) // Split the string by spaces
    {
        if (!word.empty())
        {
            word[0] = toupper(word[0]); // Capitalize the first character
            result += word;
        }
    }

    return result;
}



bool validateItemAddCommand(const vector<string> &commands);

// adding items to the file
void addItem(string filename, string data)
{
    // Check if the item ID already exists in the file
    bool itemExists = false;
    ifstream file(filename);
    if (file)
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string itemIdStr;
            ss >> itemIdStr;

            // Check if the item ID matches the one being added
            if (itemIdStr == data.substr(0, data.find(' ')))
            {
                itemExists = true;
                break;
            }
        }
        file.close();
    }

    // If the item already exists, update its quantity
    if (itemExists)
    {
        // Read the existing lines and update the quantity if necessary
        vector<string> lines;
        ifstream inFile(filename);
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string itemIdStr, restOfLine;
            ss >> itemIdStr;
            if (itemIdStr == data.substr(0, data.find(' ')))
            {
                // Split the line and update the quantity
                string itemId, itemName, quantityStr, registrationDate;
                ss >> itemName >> quantityStr >> registrationDate;
                int existingQuantity = stoi(quantityStr);
                int newQuantity = existingQuantity + 5;
                stringstream updatedLine;
                updatedLine << itemIdStr << ' ' << itemName << ' ' << newQuantity << ' ' << registrationDate;
                line = updatedLine.str();
            }
            lines.push_back(line);
        }
        inFile.close();

        // Rewrite the file with the updated lines
        ofstream outFile(filename);
        for (const string &updatedLine : lines)
        {
            outFile << updatedLine << endl;
        }
        outFile.close();

        return;
    }

    // If the item doesn't exist, append it to the file
    ofstream outFile;
    outFile.open(filename, ios::app);

    // Handling file errors
    if (!outFile)
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    outFile << data << endl;
    outFile.close();
}
// function to list items in the file

void listItems(string filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error opening file" << endl;
        return;
    }

    vector<string> items;
    string line;
    while (getline(file, line))
    {
        items.push_back(line);
    }
    file.close();

    sort(items.begin(), items.end(), [](const string &a, const string &b)
         {
        stringstream ss1(a);
        stringstream ss2(b);
        int id1, id2;
        string itemName1, itemName2;
        ss1 >> id1 >> itemName1;
        ss2 >> id2 >> itemName2;
        return itemName1 < itemName2; });

    for (const string &item : items)
    {
        stringstream ss(item);
        int id, quantity;
        string itemName, registrationDate;
        ss >> id >> itemName >> quantity >> registrationDate;
        itemName = toPascalCase(itemName);
        cout << "Item ID: " << id << "\tItem Name: " << itemName << "\tQuantity: " << quantity << "\tReg Date: " << registrationDate << endl;

    }
}

int app()
{
    vector<string> commands;
    string command;

    do
    {

        cout << "RCA> ";

        string input;
        getline(cin, input);

        for (char &c : input)
        {
            c = tolower(c);
        }

        istringstream iss(input);

        while (iss >> command)
        {
            commands.push_back(command);
        }

        if (commands[0] == "help")
        {
            cout << "=========================================================================" << endl;
            cout << "" << endl;
            cout << "*                                Commands Syntaxes                       " << endl;
            cout << "" << endl;
            cout << "=========================================================================" << endl;
            cout << "" << endl;
            cout << "        itemadd <item_id> <item_name> <quantity> <registration_date>    " << endl;
            cout << "" << endl;
            cout << "        itemslist                                                        " << endl;
        }
        else if (commands[0] == "itemadd")
        {

            if (!validateItemAddCommand(commands))

            {
                cout << commands.size() << endl;
                cout << "Invalid itemadd command. Please check the parameters and try again." << endl;
                commands.clear();
                continue;
                // Continue with the program execution
            }

            // appending the rest of the commands in the string data separted by space
            string data = commands[1] + " " + commands[2] + " " + commands[3] + " " + commands[4];
            addItem("inventory.csv", data);
        }

        // listing items of the file

        // checking if the command is itemslist and nothing else is added after it
        else if (commands[0] == "itemslist" && commands.size() == 1)
        {

            // calling function to list items
            listItems("inventory.csv");
        }

        else if (commands[0] == "exit")
        {
            // Break the loop and terminate the program
            break;
        }
        else
        {
            cout << "Please type 'help' to see the available commands." << endl;
        }

        commands.clear();
    } while (true);

    return 0;
}

bool validateItemAddCommand(const vector<string> &commands)
{
    if (commands.size() != 5)
    {
        cout << "Invalid number of parameters for 'itemadd' command." << endl;
        return false;
    }

    // Validate item ID
    const string &itemIdStr = commands[1];
    if (itemIdStr.find_first_not_of("0123456789") != string::npos)
    {
        cout << "Invalid item ID. Please provide a valid number." << endl;
        return false;
    }

    // Validate quantity
    const string &quantityStr = commands[3];
    if (quantityStr.find_first_not_of("0123456789") != string::npos)
    {
        cout << "Invalid quantity. Please provide a valid number." << endl;
        return false;
    }

    // Validate registration date
    const string &dateStr = commands[4];
    if (dateStr.size() != 10 || dateStr[4] != '-' || dateStr[7] != '-')
    {
        cout << "Invalid registration date format. Please provide a date in the format 'yyyy-mm-dd'." << endl;
        return false;
    }

    // Additional checks for year, month, and day can be added here if needed

    return true;
}
int main()
{
    cout << "=========================================================================" << endl;
    cout << "*              Welcome Rwanda Coding Academy                            *" << endl;
    cout << "=========================================================================" << endl;
    cout << "                   RCA  Inventory System                                  " << endl;

    app();

    cout << "=========================================================================" << endl;
    cout << "*              Logged out of the system                                 *" << endl;
    cout << "=========================================================================" << endl;

    return 0;
}
