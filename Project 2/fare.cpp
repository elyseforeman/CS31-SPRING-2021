//
//  fare.cpp

// Including packages for strings

#include <iostream>
#include <string>
using namespace std;

int main() {

// Declaring all of my variables
    int riderAge;
    string studentStatus;
    string riderDestination;
    int zoneBoundaries;
    double farePrice;
    
// Ensuring that there are two digits for the ride fare
    cout.setf(ios::fixed);
    cout.precision(2);
    
// Collecting Input
// Using cin.ignore in case of error from following an integer input with a getline
    cout << "Age of rider: ";
    cin >> riderAge;
    cin.ignore(10000, '\n');
    
    cout << "Student? (y/n): ";
    getline(cin, studentStatus);
        
    cout << "Destination: ";
    getline(cin, riderDestination);
    
    cout << "Number of zone boundaries crossed: ";
    cin >> zoneBoundaries;

// No discount
    farePrice = 1.45 + 0.55*zoneBoundaries;
    
// Youth discount for short trips
    if (riderAge < 18 && (zoneBoundaries == 0 || zoneBoundaries == 1))
        farePrice = 0.65;
    
// Student discount for short trips
    if (riderAge >= 18 && studentStatus == "y" && (zoneBoundaries == 0 || zoneBoundaries == 1))
        farePrice = 0.65;
        
// Senior discount
// Nested if-else statement that checks if the senior is crossing 0 zones
    if (riderAge >= 65)
    { if (zoneBoundaries == 0)
            farePrice = 0.45;
        else
            farePrice = 0.55 + 0.35*zoneBoundaries; }

// Three hyphens
    cout << "---" << endl;
    
// Error Codes
// Ladder of if-else statements that checks each input for error
    if (riderAge < 0)
        cout << "The age must not be negative" << endl;
    else if (studentStatus != "y" && studentStatus != "n")
        cout << "You must enter y or n" << endl;
    else if (riderDestination == "")
        cout << "You must enter a destination" << endl;
    else if (zoneBoundaries < 0)
        cout << "The number of zone boundaries crossed must not be negative" << endl;
    
// Correct Output
// If all output is correct, then "The fare to" statement is printed with the correct fare
    if (riderAge >= 0 && (studentStatus == "y" || studentStatus == "n") && riderDestination != "" && zoneBoundaries >= 0)
        cout << "The fare to " << riderDestination << " is $" << farePrice << endl;
    
}
