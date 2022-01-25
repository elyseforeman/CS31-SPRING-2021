//
//  main.cpp
//  Project 4
#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

// Declaring all functions

int countMatches(const string a[], int n, string target);
int detectMatch(const string a[], int n, string target);
bool detectSequence(const string a[], int n, string target, int& begin, int& end);
int detectMin(const string a[], int n);
int moveToBack(string a[], int n, int pos);
int moveToFront(string a[], int n, int pos);
int detectDifference(const string a1[], int n1, const string a2[], int n2);
int deleteDups(string a[], int n);
bool contains(const string a1[], int n1, const string a2[], int n2);
int detectMax(const string a[], int n);
int meld(const string a1[], int n1, const string a2[], int n2,
string result[], int max);
int split(string a[], int n, string splitter);

// Main program

int main()
{

    return 0;
}

// Function countMatches

int countMatches(const string a[], int n, string target)
{
    if (n < 0)
        return -1;
    int matches = 0;
    for (int i = 0; i < n; ++i)     // Iterates through interesting elements to count
    {
        if (a[i] == target)
        {
            matches += 1;
        }
    }
    return matches;
}

// Function detectMatch

int detectMatch(const string a[], int n, string target)
{
    for (int i = 0; i < n; ++i)
    {
        if (a[i] == target)
        {
            return i;
            break;                // Loop breaks so only the first match is returned
        }
    }
    return -1;                    // returns -1 if (n < 0) or a target is not found
}

// Function detectSequence
bool detectSequence(const string a[], int n, string target, int& begin, int& end)
{
    if (n < 0)
        return false;
    
    for (int i = 0; i < n; ++i)
    {
        if (a[i] == target)
        {
            begin = i;                   // Sets beginning and end within loop
            end = i;                     // so remain unhchanged in other cases
            for (int j = begin + 1; j < n; ++j)
            {
                if (a[j] == target)
                    end = j;            // Sets end to last target element
                else
                    return true;
            }
            return true;
        }
    }
    return false;
}

// Function detectMin

int detectMin(const string a[], int n)
{
    if (n <= 0)
        return -1;

    string min = a[0];
    int minPosition = 0;
    for (int i = 0; i < n; ++i)
    {
        if (a[i] < min)
        {
            min = a[i];
        minPosition = i;
        }
    }
    
    return minPosition;
}

// Function detectMax for use in Meld
int detectMax(const string a[], int n)
{
    if (n <= 0)
        return -1;

    string max = a[0];
    int maxPosition = 0;
    for (int i = 0; i < n; ++i)
    {
        if (a[i] > max)
        {
            max = a[i];
        maxPosition = i;
        }
    }
    
    return maxPosition;
}


// Function moveToBack


int moveToBack(string a[], int n, int pos)
{
    if (n < 0)
        return -1;
    if (pos >= n || pos < 0)
        return -1;
    
    string hold = a[pos];
    for (int i = pos; i < n -1; ++i)
    {
        a[i] = a[i+1];
    }
    a[n-1] = hold;
    
    return pos;
}

// Function moveToFront

int moveToFront(string a[], int n, int pos)
{
    if (n < 0)
        return -1;
    if (pos >= n || pos < 0)
        return -1;
    
    string hold = a[pos];
    for (int i = pos; i >= 1; --i)
    {
        a[i] = a[i-1];
    }
    a[0] = hold;
    
    return pos;
}

// Function detectDifference

int detectDifference(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0)
        return -1;
    
    for (int i = 0; i < n1; ++i)
    {
        if (a1[i] != a2[i])
            return i;
    }
    if (n1 <= n2)
        return n1;
    else
        return n2;
}

// Function deleteDups
int deleteDups(string a[], int n)
{
    if (n < 0)
        return -1;
    
    // Using counters numMatch and totalMatch to reset the conditions for the for loops in order to prevent the loops running after all of the interesting elements have been dealt with
    // Count numMatch allows me to adjust the number of spaces that the elements are shifted whenever trying to eliminate a repeated element (ex. if there is a sequence of 3 matching elements in a row starting with a[i], then numMatch = 2, and all of the elements after a[i] will be replaced with the element 2 ahead of them, so that the repeated elements are all eliminated.
    int numMatch = 0;
    int totalMatch = 0;
    
    for (int i = 0; i < n-1-totalMatch; ++i)
    {
        if (a[i] == a[i+1])
        {
            numMatch = 1;
            for (int j = i+1; j < n-1-totalMatch; ++j)
            {
                if (a[j] == a[j+1])
                {
                    numMatch += 1;
                }
                else
                    break;
            }
            for (int k = i+1; k < n-numMatch; ++k)
            {
                a[k] = a[k+numMatch];
            }
        }
        totalMatch += numMatch;
    }
    return n-totalMatch;
}


// Function Contains
/*



 */
bool contains(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0)
        return false;
    if (n2 > n1)
        return false;
    
    int count = 0;
    int index = 0;
    
    for (int i = 0; i < n2; i++)
    {
        for (int j = index; j < n1; j++)
        {
            if (a2[i] == a1[j])
            {
                index = j+1;        // Index sets for loop to checks elements in order
                ++count;
                break;
            }
        }
    }
    
    if (count == n2)
        return true;
    else
        return false;
}

// Function Meld

int meld(const string a1[], int n1, const string a2[], int n2,
string result[], int max)
{
    if ((n1+n2) > max)
        return -1;
    
    // Confirming that both strings a1 and a2 are in nondecreasing order
    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i < n1-1; i++)
    {
        if(a1[i] <= a1[i+1])
            ++count1;
    }
    
    for (int i = 0; i < n2-1; i++)
    {
        if(a2[i] <= a2[i+1])
            ++count2;
    }
    
    // Filling result with all the elements from a1 and a2
    const int ARRAY_SIZE = n1+n2;

    if (ARRAY_SIZE > max)
        return -1;
    
    for (int i = 0; i < n1; i++)
        result[i] = a1[i];
    for (int i = 0; i < n2; i++)
        result[n1+i] = a2[i];
    
    
    // Moving all of the maximum values to the back of array result[]
    // This will create an array that has all the values of a1 and a2 going in descending order
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        moveToBack(result, ARRAY_SIZE, detectMax(result, ARRAY_SIZE-i));
    }
    
    // Reversing array result[] so that it is in nondescending order
    
    for (int i = 0; i < ARRAY_SIZE/2; i++)
    {
        string temp = result[i];
        result[i] = result[ARRAY_SIZE-1-i];
        result[ARRAY_SIZE-1-i] = temp;
    }

    // Return the number of elements in result[] if count1 and count2 confirm that a1 and a2 are in nondecreasing order
    if (count1 == n1-1 && count2 == n2-1)
        return n1+n2;
    else
        return -1;
    
}

// Function splitter

int split(string a[], int n, string splitter)
{
    if (n < 0)
        return -1;
    
    // Count how many elements < splitter there are in a[]
    int countLess = 0;
    int countMore = 0;
    int countSplit = 0;
    for (int i = 0; i < n; ++i)
    {
        if (a[i] < splitter)
            ++countLess;
        else if (a[i] == splitter)
            ++countSplit;
        else if (a[i] > splitter)
            ++countMore;
    }

    // Rearrange string a[]
    // Move all the minimums (<splitter) to the front
    for (int i = 0; i < n; ++i)
    {
        if (a[i] < splitter)
        {
            moveToFront(a, n, i);
        }
    }
    // Move to back everything in splitter's place
    
    if (countSplit >= 0)
    {
        for(int i = 1; i <= countSplit; i++)
        {
            while(a[countLess] != splitter)
            {
                moveToBack(a, n, countLess);
                break;
            }
        }
    }
    
    for (int i = 0; i < n; ++i)
    {
        if (a[i] >= splitter)
            return i;
    }
    return n;
}
