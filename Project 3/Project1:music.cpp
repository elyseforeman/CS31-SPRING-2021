//
//  music.cpp

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

    // Main Functions
bool hasCorrectForm(string s);
int convertSong(string s, string& instructions, int& badTick);

    // Secondary Functions ( in order of appearance )

char convertNote(int octave, char noteLetter, char accidentalSign);
bool isAthroughG(char c);
bool checkNote(string s, int i);
bool checkNoteDigit(string s, int i);
bool checkNoteAccidentalDigit(string s, int i);
bool checkNoteAccidental(string s, int i);

    // Int Main
int main()
{

    return 0;
}

int convertSong(string s, string& instructions, int& badTick)
{
    int i = 0;
    int numTick = 0;
    int totNote = 0;
    string convertedSong ="";
    
    if (hasCorrectForm(s))
    {
        while (i!=s.size())
        {
                if (s=="")
                {
                    convertedSong = "";
                    instructions = convertedSong;
                    badTick = badTick;
                    return 0;
                }
                else if(s=="/")                                 // Converts '/' into ' '
                {
                    convertedSong = " ";
                    instructions = convertedSong;
                    badTick = badTick;
                    return 0;
                }
                else if(checkNote(s, i))                        // Checks if a letter A-G is a standalone note
                {
                    convertedSong += convertNote(4, s.at(i), ' ');
                    totNote+=1;
                    i+=1;
                }
                else if (checkNoteDigit(s, i))                  // Checks if a letter A-G is a note followed
                {                                               // by a digit
                    convertedSong += convertNote((s.at(i+1)-48), s.at(i), ' ');
                    totNote+=1;
                    i+=2;
                }
                else if (checkNoteAccidentalDigit(s, i))
                {
                    convertedSong += convertNote((s.at(i+2)-48), s.at(i), s.at(i+1));
                    totNote+=1;
                    i+=3;
                }
                else if (checkNoteAccidental(s, i))
                {
                    convertedSong += convertNote(4, s.at(i), s.at(i+1));
                    totNote+=1;
                    i+=2;
                }
                else if (s.at(i) == '/' && i == 0)                  // Converts '/' to a space if its the
                {                                                   // first character
                    convertedSong+= ' ';
                    numTick+=1;
                    i+=1;
                }
                else if (s.at(i) == '/' && totNote == 1)            // Increments numTick if it is a tick
                {                                                   // with a single note
                    i+=1;
                    numTick+=1;
                    totNote=0;
                }
                else if (s.at(i) == '/' && totNote > 1 && numTick == 1) // Adds brackets around first group of
                {                                                       // notes
                    convertedSong.insert(0, "[");
                    convertedSong += ']';
                    i+=1;
                    totNote=0;
                    numTick+=1;
                }
                else if (s.at(i) == '/' && totNote == 0)
                {
                    convertedSong+= ' ';
                    i+=1;
                    numTick+=1;
                    totNote=0;
                }
                else if (s.at(i) == '/' && totNote > 1 && numTick != 1)
                {
                    convertedSong.insert(convertedSong.size()-totNote, "[");    // Subtracts by number of
                    convertedSong+= ']';             // notes to find where to place a bracket around a chord
                    i+=1;
                    totNote=0;
                    numTick+=1;
                }
                else
                {
                    instructions = instructions;
                    badTick = numTick+1;
                    return 2;
                }
        }
        instructions = convertedSong;
        badTick = badTick;
        return 0;
    }
    else
    {
        instructions = instructions;
        badTick = badTick;
        return 1;
    }
}

bool hasCorrectForm(string s)
{

    int isNote = 0;
    int isNoteNote = 0;
    int isNoteAccidental = 0;
    int isNoteDigit = 0;
    int isNoteAccidentalDigit = 0;
    int i;
    string noSlash = "";
    
    // Return true if empty string
    
    if (s == "")
    {
        return true;
    }
    
    // Return false if string doesn't end in '/'
    
    if (s.at(s.size()-1) != '/')
    {
        return false;
    }
    
    // Return false if string doesn't start with A-G or '/'
    
    if (!isAthroughG(s.at(0)) && s.at(0) != '/')
    {
        return false;
    }
  
    // Check if note - note
    
    for (i=0; i!=s.size()-1; ++i)
    {
        if(isAthroughG(s.at(i)) && isAthroughG(s.at(i+1)))
        {
            isNoteNote += 1;
        }
    }

    // Check if note - '/'
    
    for (i=0; i!=s.size()-1; ++i)
    {
        if(isAthroughG(s.at(i)) && s.at(i+1) == '/')
        {
            isNote += 1;
        }
    }
        
    // Check if note letter - accidental sign
    
    for (i=0; i!=s.size()-1; ++i)
    {
        if(isAthroughG(s.at(i)) && (s.at(i+1) == '#' || s.at(i+1) == 'b'))
        {
                isNoteAccidental += 1;
        }
    }
            
    // Check if note letter - digit
    for (i=0; i!=s.size()-1; ++i)
    {
        if(isAthroughG(s.at(i)) && isdigit(s.at(i+1)))
        {
            isNoteDigit += 1;
        }
    }
        
    // Check if note letter - accidental sign - digit
    
    if (s.size() >= 2)
    {
    for (i=0; i!=s.size()-2; ++i)
    {
        if(isAthroughG(s.at(i)) && (s.at(i+1) == '#' || s.at(i+1) == 'b') && isdigit(s.at(i+2)))
        {
            isNoteAccidentalDigit += 1;
            isNoteAccidental -= 1;
        }
    }
    }
    
    // Remove Slashes
    
    for (i=0; i!=s.size(); ++i)
    {
        if(s.at(i) != '/')
        {
            noSlash += s.at(i);
        }
    }
    
    if ((isNote + isNoteNote + isNoteAccidental*2 + isNoteDigit*2 + isNoteAccidentalDigit*3) == noSlash.size())
        return true;
    else
        return false;
}


// Checking Functions For convertSong

bool checkNote(string s, int i)
{
    if (isAthroughG(s.at(i)))
    {
    if (isAthroughG(s.at(i+1)) || s.at(i+1) == '/')
        return true;
    else
        return false;
    }
    else
        return false;
}

// Check note-digit function
bool checkNoteDigit(string s, int i)
{
    if (isAthroughG(s.at(i)))
    {
    if(isdigit(s.at(i+1)) && ((s.at(i+1)-48) > 1 && (s.at(i+1)-48) < 7))
        return true;
    else
        if (s.at(i) == 'C' && (s.at(i+1)-48) == 7)
            return true;
        else
            return false;
    }
    else
        return false;
}

// Check note-accidental-digit function
bool checkNoteAccidentalDigit(string s, int i)
{
    if (s.at(i) == 'C' && s.at(i+1) == 'b' && s.at(i+2) == '2')
        return false;
    if (isAthroughG(s.at(i)))
    {
    if((s.at(i+1) == 'b' || s.at(i+1) == '#') && ((s.at(i+2)-48) > 1 && (s.at(i+2)-48) < 7))
        return true;
    else if (s.at(i) == 'C' && s.at(i+1) == 'b' && s.at(i+2) == '7')
        return true;
    else if (s.at(i) == 'B' && s.at(i+1) == '#' && s.at(i+2) == '1')
        return true;
    else
        return false;
    }
    else
        return false;
}

// Check note-accidental function

bool checkNoteAccidental(string s, int i)
{
    if (isAthroughG(s.at(i)))
    {
    if((s.at(i+1) == 'b' || s.at(i+1) == '#'))
        return true;
    else
        return false;
    }
    else
        return false;
}
   
// Check if A - G
bool isAthroughG(char c)
{
    bool tf = false;
    
    if(isalpha(c) && (c == toupper(c)))
    {
        if(((c + '0') >= 113) && (c + '0')<= 119)
        {
            return true;
        }
    }
    return tf;
}

// Given convertNote function
char convertNote(int octave, char noteLetter, char accidentalSign)
{
      // This check is here solely to report a common CS 31 student error.
    if (octave > 9)
    {
        cerr << "********** convertNote was called with first argument = "
             << octave << endl;
    }

      // Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
      //      to -1, 0,   1,   2,   3, ...,  11, 12

    int note;
    switch (noteLetter)
    {
      case 'C':  note =  0; break;
      case 'D':  note =  2; break;
      case 'E':  note =  4; break;
      case 'F':  note =  5; break;
      case 'G':  note =  7; break;
      case 'A':  note =  9; break;
      case 'B':  note = 11; break;
      default:   return ' ';
    }
    switch (accidentalSign)
    {
      case '#':  note++; break;
      case 'b':  note--; break;
      case ' ':  break;
      default:   return ' ';
    }

      // Convert ..., A#1, B1, C2, C#2, D2, ... to
      //         ..., -2,  -1, 0,   1,  2, ...

    int sequenceNumber = 12 * (octave - 2) + note;

    string keymap = "1!2@34$5%6^78*9(0qQwWeErtTyYuiIoOpPasSdDfgGhHjJklLzZxcCvVbBnm";
    if (sequenceNumber < 0  ||  sequenceNumber >= keymap.size())
        return ' ';
    return keymap[sequenceNumber];
}
