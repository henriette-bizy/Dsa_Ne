#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;

// addLocation
void addLocation(string filename, string location)
{
    ofstream file;
    file.open(filename, ios::app);

    // cheking if the file is not open
    if (!file.is_open())
    {
        cout << "Error: file is not open" << endl;
    }
    // adding location in uppercase
    for (char &c : location)
    {
        c = toupper(c);
    }
    file << location << endl;
    file.close();
}

// addRecord
void addRecord(string filename, string data)
{
    // Opening the file in append mode
    ofstream file;
    file.open(filename, ios::app);

    // Checking if the file is not open
    if (!file.is_open())
    {
        cout << "Error: file is not open" << endl;
        return; // Exit the function if file opening failed
    }

    // Converting the data to uppercase
    for (char &c : data)
    {
        c = toupper(c);
    }

    cout << data << endl;

    // Getting the first parameter of data which is location
    string location = data.substr(0, data.find_first_of(" "));
    cout << location << endl;

    // Reading the file
    ifstream file1;
    file1.open(filename);
    string line;
    string updatedData = "";

    // Finding if the location doesn't exist already in the file
    bool found = false;
    while (getline(file1, line))
    {
        // If it finds the location and there is no data in the line
        if (line.compare(location) == 0 && line.find_first_of(" ") == string::npos)
        {
            found = true;
            cout << line << endl;

            // Appending the rest of the parameters except location to that line
            line.append(" ").append(data.substr(data.find_first_of(" ") + 1));
            cout << line << endl;
            updatedData += line + "\n"; // Store the updated line with appended data
        }
        else
        {
            updatedData += line + "\n"; // Store other lines as they are
        }
    }

    if (!found)
    {
        // If the location was not found, append the new data to the file
        updatedData += data + "\n";
    }

    // Close the input file before writing to it
    file1.close();

    // Open the file in write mode to update its contents
    ofstream file2;
    file2.open(filename, ios::trunc);
    file2 << updatedData; // Write the updated contents to the file

    // Close both files
    file2.close();
    file.close();
}

void printData(const vector<string> &data);

// listing the locations

void listLocations(string filename)
{
    ifstream file(filename);
    string line;
    set<string> uniqueLocations;

    if (!file.is_open())
    {
        cout << "Error: Unable to open the file" << endl;
    }

    while (getline(file, line))
    {
        // Insert the first parameter of the line into the set
        string location = line.substr(0, line.find_first_of(" "));
        uniqueLocations.insert(location);
    }
    file.close();

    // Convert the set to a vector for easier sorting
    vector<string> sortedLocations(uniqueLocations.begin(), uniqueLocations.end());
    sort(sortedLocations.begin(), sortedLocations.end());

    printData(sortedLocations);
}
void printData(const vector<string> &data)
{
    for (const string &data : data)
    {
        cout << data << endl;
    }
}


//listing the diseases
void listDiseases(string filename)
{
    ifstream file(filename);
    string line;
    set<string> uniqueDiseases;

    if (!file.is_open())
    {
        cout << "Error: Unable to open the file" << endl;
    }

    while (getline(file, line))
    {
        // Find the position of the first and last spaces
        size_t firstSpacePos = line.find_first_of(" ");
        size_t lastSpacePos = line.find_last_of(" ");

        // Check if both spaces exist and the first space is before the last space
        if (firstSpacePos != string::npos && lastSpacePos != string::npos && firstSpacePos < lastSpacePos)
        {
            // Extract the second parameter (disease) from the line
            string disease = line.substr(firstSpacePos + 1, lastSpacePos - firstSpacePos - 1);
            
            // Insert the disease into the set uniqueDiseases
            uniqueDiseases.insert(disease);
        }
    }
    file.close();

    // Convert the set to a vector for easier sorting
    vector<string> sortedDiseases(uniqueDiseases.begin(), uniqueDiseases.end());
    sort(sortedDiseases.begin(), sortedDiseases.end());

    
    printData(sortedDiseases);
}

//searchin the location by disease

void searchLocation(string disease){
    ifstream file("data.csv");
    string line;
    vector<string> locations;

    if (!file.is_open())
    {
        cout << "Error: Unable to open the file" << endl;
    }


   for(char& c: disease){
          c = toupper(c);
   }


    while (getline(file, line))

    //find where the second parameter is equal to the disease given
    {
        size_t firstSpacePos = line.find_first_of(" ");
        size_t lastSpacePos = line.find_last_of(" ");

        if (firstSpacePos != string::npos && lastSpacePos != string::npos && firstSpacePos < lastSpacePos)
        {
            string disease1 = line.substr(firstSpacePos + 1, lastSpacePos - firstSpacePos - 1);
            if (disease1.compare(disease) == 0)
            {
                string location = line.substr(0, firstSpacePos);
                locations.push_back(location);
            }
        }
    }


    //printing the locations
    if (locations.size() == 0)
    {
        cout << "No locations found for this disease"<< endl;
    }

    else
    {
        for (const string &loc : locations)
        {
            cout << "["<<loc<<"]" << endl;
        }
 
    }

    file.close();

    
}

//returning total number of cases by disease and location

void totalCases(string location, string disease){
    ifstream file("data.csv");
    string line;
    int totalCases = 0;

    if (!file.is_open())
    {
        cout << "Error: Unable to open the file" << endl;
    }

    while (getline(file, line))
    {
        size_t firstSpacePos = line.find_first_of(" ");
        size_t lastSpacePos = line.find_last_of(" ");

        if (firstSpacePos != string::npos && lastSpacePos != string::npos && firstSpacePos < lastSpacePos)
        {
            string location1 = line.substr(0, firstSpacePos);
            string disease1 = line.substr(firstSpacePos + 1, lastSpacePos - firstSpacePos - 1);

            // converting location and disease to uppercase
            for (char &c : location1)
            {
                c = toupper(c);
            }
            for (char &c : disease1)
            {
                c = toupper(c);
            }

            if (location1.compare(location) == 0 && disease1.compare(disease) == 0)
            {
                string cases = line.substr(lastSpacePos + 1);
                totalCases += stoi(cases);
                cout << "it reached here" <<endl;
            }
        }
    }

    cout << "Total cases of " << disease << " in " << location << " is " << totalCases << endl;

    file.close();
}


int app()
{
    cout << "Need a help?, type 'help' then press Enter key" << endl;

    vector<string> commands;
    string command;

    do
    {
        cout << "Console> ";

        string input;
        getline(cin, input);

        for (char &c : input)
        {
        c:
            _tolower;
        }

        
        istringstream iss(input);

        while (iss >> command)
        {
            commands.push_back(command);
        }
        // if(commands.at(0).compare("help") == 0)
        if (commands[0] == "help")
        {

            cout << "=========================================================================" << endl;
            cout << "" << endl;
            cout << "*                                H E L P  M E N U                         " << endl;
            cout << "" << endl;
            cout << "=========================================================================" << endl;
            cout << "" << endl;
            cout << "add <Location>                             :Add new location" << endl;
            cout << "" << endl;
            cout << "delete <Location>                          :Delete an existing location" << endl;
            cout << "" << endl;
            cout << "record <Location>  <disease> <case>        :Record a disease and its cases" << endl;
            cout << "" << endl;
            cout << "list locations                             :List all existing locations" << endl;
            cout << "" << endl;
            cout << "list diseases                              :List existing diseases Diseases in locations" << endl;
            cout << "" << endl;
            cout << "where <disease>                            :Find where disease exists" << endl;
            cout << "" << endl;
            cout << "cases <locations> <diseases>                :Find cases of disease in location" << endl;
            cout << "" << endl;
            cout << "cases <diseases>                           :Find total cases of a given disease" << endl;
            cout << "" << endl;
            cout << "help                                       :Prints  user manual" << endl;
            cout << endl;
            cout << "Exit                                       :Exit the program" << endl;
        }
        else if (commands[0] == "add")
        {

            addLocation("data.csv", commands[1]);
            cout << commands[1] + " is successfully added" << endl;
        }

        else if (commands[0] == "delete")
        {
        }
        else if (commands[0] == "record")
        {
            string data = commands.at(1).append(" ").append(commands.at(2)).append(" ").append(commands.at(3));
            // add("cases.csv",data);
            addRecord("data.csv", data);
        }

        else if (commands[0] == "list")
        {

            if (commands[1] == "locations")
            {
                listLocations("data.csv");
            }
            else if (commands[1] == "diseases")
            {
                listDiseases("data.csv");
            }
        }

        else if (commands[0] == "where")
        {
            searchLocation(commands[1]);
        }

        else if (commands[0] == "cases")
        {
            totalCases(commands[1], commands[2]);
        }
       
        else
        {
            cout << "please  type help to see what command can help you" << endl;
        }
        commands.clear();
    }

    while (commands[0].compare("exit") != 0);

    return 0;
}

int main()
{

    cout << "=========================================================================" << endl;
    cout << "*         Welcome to Disease Cases Reporting System                     *" << endl;
    cout << "*                              *" << endl;
    cout << "*         It is developed by Biziyaremye Henriette                      *" << endl;
    cout << "*         evaluation for the end of Y3.                                 *" << endl;
    cout << "=========================================================================" << endl;
    cout << "          Starting Time: Thu 01.01.2023" << endl;

    app();

    return 0;
}