#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

// Declare Functions

int fill(int lineLength, istream& inf, ostream& outf);
int fillWord(istream& inf, char word[], bool &hasPunctuation);

// Main Function
int main()
{
    int lineLength = 9;
    ifstream infile("data.txt");
    ofstream outfile("results.txt");
    cout << fill(lineLength, infile, outfile);
    
    return 0;
}

// Function definitions

int fill(int lineLength, istream& inf, ostream& outf)
{
    // If the desired maximum line length is less than 1, then return 2
    
    if (lineLength < 1 || lineLength > 350)
        return 2;
    
    // Declaring variables
    
    char word[160];
    int wordCount = 0;                                               // Tracks the number of words on each line
    int charCount = 0;                                               // Tracks the number of characters currently on each line
    char longWord[160];                                              // Used for splitting long word across two lines
    bool LONG_WORD = false;                                          // = true when the input contains a word longer than lineLength
    bool hasPunctuation = false;                                     // Set to true when a word ends in '.' or '?'
    int storePunctuation = 0;                                        // Set to 1 when the previous word ends in '.' or '?'
    bool isParagraph = false;                                        // Set to true when a word is #P#
    int storeParagraph = 0;                                          // Set to 1 when the previous word is #P#
    char newParagraph[4] = "#P#";                                    // Used to compare a word to #P#

    for (;;)
    {

        int indicator = fillWord(inf, word, hasPunctuation);
                
        if (indicator == 1)
            {
                // If this word is #P#, then isParagraph = true, and storeParagraph will be set to 1 for future use
                if (strcmp(word, newParagraph) == 0)
                {
                    isParagraph = true;
                }
                // If this word is either the first word of input OR following a word that filled a whole line, output it with no space (or an empty line when following a word that fills an entire line)
                else if (strlen(word) <= lineLength && (wordCount == 0 || wordCount == 100))
                {
                    // Output an empty line if the previous word was #P# and the word before that filled the line
                    if (wordCount == 100 && storeParagraph == 1)
                        outf << endl;
                    outf << word;
                    charCount = static_cast<int>(strlen(word));
                    wordCount = 1;
                }
                // If this word is longer than the line length, split it into multiple lines by filling an array with lineLength characters
                else if (strlen(word) > lineLength)
                {
                    LONG_WORD = true;
                    // Output a newline unless this is the first input word OR the previous word filled up the entire line
                    if (wordCount != 0 && wordCount != 100)
                    {
                        if (storeParagraph == 1)
                            outf << endl << endl;
                        else
                            outf << endl;
                    }
                    // Copy lineLength characters to the array longWord[] and output longWord for as many times as lineLength fits into this word
                    for (int i = 0; i < strlen(word)/lineLength; ++i)
                    {
                        strncpy(longWord, word+(lineLength*i), lineLength);
                        longWord[lineLength] = '\0';
                        outf << longWord << endl;
                    }
                    // Copy the remainder of word into longWord and output it on the next line
                    strncpy(longWord, word+(lineLength*(strlen(word)/lineLength)), strlen(word)-lineLength*(strlen(word)/lineLength));
                    longWord[strlen(word)-lineLength*(strlen(word)/lineLength)] = '\0';
                    outf << longWord;
                    // If there is no remainder, set charCount to 0; else, set charCount to the length of the remainder
                    if (strlen(word)%lineLength == 0)
                    {
                        wordCount = 100;
                        charCount = 0;
                    }
                    else
                    {
                        wordCount = 1;
                        charCount = static_cast<int>(strlen(longWord));
                    }
                }
                // If the previous word ended in '.' or '?' and this word will fit on the current line, output it with two spaces
                else if (storePunctuation == 1 && 2 + strlen(word) + charCount <= lineLength)
                {
                    outf << "  " << word;
                    ++wordCount;
                    charCount += 2 + strlen(word);
                }
                // If the previous word ended in '.' or '?' and this word will NOT fit on the current line, output a newline followed by the word
                else if (storePunctuation == 1)
                {
                    outf << endl << word;
                    wordCount = 1;
                    charCount = static_cast<int>(strlen(word));
                }
                // If this word will fit on the current line, output it with one space
                else if (1 + strlen(word) + charCount <= lineLength)
                {
                    if (storeParagraph == 1)
                    {
                        outf << endl << endl << word;
                        charCount = static_cast<int>(strlen(word));
                        wordCount = 1;
                    }
                    else
                    {
                    outf << " " << word;
                    charCount += 1 + static_cast<int>(strlen(word));
                    ++wordCount;
                    }
                }
                // If this word will NOT fit on the current line, output a newline followed by the word
                else
                {
                    if (storeParagraph == 1)
                        outf << endl << endl << word;
                    else
                        outf << endl << word;
                    charCount = static_cast<int>(strlen(word));
                    wordCount = 1;
                }
                // This if-else will set storeParagraph to 1, so the next word knows if the previous word was #P#
                if (isParagraph)
                {
                    storeParagraph = 1;
                    isParagraph = false;
                }
                else
                    storeParagraph = 0;
                // This if-else will set storePunctuation to 1, so the next word knows if the previous word contained punctuation
                if (hasPunctuation)
                {
                    storePunctuation = 1;
                    hasPunctuation = false;
                }
                else
                    storePunctuation = 0;
            }
            // Outputs a newline and breaks out of the loop if fillWord function hits the end of the input
            else if (indicator == 2)
            {
                // If the input doc is empty, don't output a newline at the end of output
                if (wordCount == 0)
                    break;
                else
                {
                    outf << "\n";
                    break;
                }
            }
        }
    
    // Return 1 if the input contained a word longer than lineLength; Return 0 if everything ran correctly
    if (LONG_WORD)
        return 1;
    else
        return 0;
}

// Function fillWord uses a while loop to fill a char array word[160] with input character-by-character until it hits whitespace
int fillWord(istream& inf, char word[], bool &hasPunctuation)
{
    char c;
    int i = 0;
    
    while (inf.get(c))
    {
        if (!isspace(c))
        {
            word[i] = c;
            word[i+1] = '\0';
            ++i;
        }
        else if (i == 0)                    // return 0 if c is whitespace
            return 0;
        else if (word[i-1] == '.' || word[i-1] == '?')
        {
            hasPunctuation = true;
            return 1;
        }
        else
            return 1;                       // return 1 if c is the start of a word
    }
    if (i > 0)
    {
        return 1;                           // run word printing a final time if the file ends directly after the final word
    }
    else
        return 2;                           // return 2 if there is a space after the final word end of file
}
