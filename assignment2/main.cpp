/*
Skeleton code for storage and buffer management
*/

#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <ios>
#include <stdexcept>
#include <limits> // for numeric_limits
#include <iostream>
#include "classes.h"
using namespace std;

int main(int argc, char* const argv[]) {
    // Create the EmployeeRelation file from Employee.csv
    StorageBufferManager manager("EmployeeRelation");
    manager.createFromFile("Employee.csv");

    // Loop to lookup IDs until the user is ready to quit
    int id;
    while (true) {
        cout << "\nEnter employee ID (or -1 to quit): ";
         // Input validation
        if (!(cin >> id)) {
            // If the input is not an integer
            cout << "Error: Please enter a valid integer." << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            continue;  // Restart the loop
        }

        if (id == -1) {
            break;
        }

        try {
            Record foundRecord = manager.findRecordById(id);
            foundRecord.print();
        } catch (const std::runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}