// witches.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>

using namespace std;
///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;      // max number of rows in a city
const int MAXCOLS = 40;      // max number of columns in a city
const int MAXWITCHES = 160;  // max number of witches allowed

const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class City;  // This is needed to let the compiler know that City is a
             // type name, since it's mentioned in the Witch declaration.

class Witch
{
  public:
        // Constructor
    Witch(City* cp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();

  private:
    City* m_city;
    int   m_row;
    int   m_col;
};

class Player
{
  public:
        // Constructor
    Player(City* cp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

        // Mutators
    void stand();
    void move(int dir);
    bool tossBalloon(int dir);
    void setDead();

  private:
    City* m_city;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class City
{
  public:
        // Constructor/destructor
    City(int nRows, int nCols);
    ~City();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     witchCount() const;
    int     numWitchesAt(int r, int c) const;
    void    display(string msg) const;

        // Mutators
    bool    addWitch(int r, int c);
    bool    addPlayer(int r, int c);
    bool    meltWitch(int r, int c);
    bool    moveWitches();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Witch*  m_witches[MAXWITCHES];
    int     m_nWitches;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nWitches);
    ~Game();

        // Mutators
    void play();

  private:
    City* m_city;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Witch implementation
///////////////////////////////////////////////////////////////////////////

Witch::Witch(City* cp, int r, int c)
{
    if (cp == nullptr)
    {
        cout << "***** A witch must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > cp->rows()  ||  c < 1  ||  c > cp->cols())
    {
        cout << "***** Witch created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_city = cp;
    m_row = r;
    m_col = c;
}

int Witch::row() const
{
    return m_row;
}

int Witch::col() const
{
    return m_col;
}

void Witch::move()
{
    // Attempt to move in a random direction;  if we can't move, don't move.
    switch (randInt(0, NUMDIRS-1))
    {
      case UP:
            if (m_row - 1 > 0)
                --m_row;
            break;
      case DOWN:
            if (m_row + 1 <= m_city->rows())
                ++m_row;
            break;
      case LEFT:
            if (m_col-1 > 0)
                --m_col;
            break;
      case RIGHT:
            if (m_col+1 <= m_city->cols())
                ++m_col;
            break;
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(City* cp, int r, int c)
{
    if (cp == nullptr)
    {
        cout << "***** The player must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > cp->rows()  ||  c < 1  ||  c > cp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_city = cp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    m_age++;
    
    switch (dir)
    {
      case UP:
            if (m_row - 1 > 0)
                --m_row;
            break;
      case DOWN:
            if (m_row + 1 < m_city->rows()+1)
                ++m_row;
            break;
      case LEFT:
            if (m_col-1 > 0)
                --m_col;
            break;
      case RIGHT:
            if (m_col+1 < m_city->cols()+1)
                ++m_col;
            break;
    }
}

bool Player::tossBalloon(int dir)
{
    m_age++;

    if (randInt(1, 3) > 1)  // miss with 2/3 probability
        return false;
    
    switch (dir)
    {
      case UP:
            for (int i = 0; i < m_row; ++i)
            {
                if(m_city->meltWitch(m_row-i, m_col))
                return true;
            }
            break;
      case DOWN:
            for (int i = 0; i <= m_city->rows()-m_row; ++i)
            {
                if(m_city->meltWitch(m_row+i, m_col))
                return true;
            }
            break;
      case LEFT:
            for (int i = 0; i < m_col; ++i)
            {
                if (m_city->meltWitch(m_row, m_col-i))
                return true;
            }
            break;
      case RIGHT:
            for (int i = 0; i <= m_city->cols()-m_col; ++i)
            {
                if (m_city->meltWitch(m_row, m_col+i))
                return true;
            }
            break;
    }
    return false;
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  City implementations
///////////////////////////////////////////////////////////////////////////

City::City(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** City created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nWitches = 0;
}

City::~City()
{
    delete m_player;
    for (int i = 0; i < m_nWitches; ++i)
    {
        delete m_witches[i];
    }
}

int City::rows() const
{
    return m_rows;
}

int City::cols() const
{
    return m_cols;
}

Player* City::player() const
{
    return m_player;
}

int City::witchCount() const
{
    return m_nWitches;
}

int City::numWitchesAt(int r, int c) const
{
    int numWitches = 0;
    for (int i = 0; i < m_nWitches; ++i)
    {
        if (m_witches[i]->row() == r && m_witches[i]->col() == c)
        {
            ++numWitches;
        }
    }
    return numWitches;
}

void City::display(string msg) const
{
      // Position (row,col) in the city coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    // Indicate each witch's position
    //        If one witch is at some position, set the char to 'W'.
    //        If it's 2 through 8, set it to '2' through '8'.
    //        For 9 or more, set it to '9'.
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
        {
            int numWitch = numWitchesAt(r+1,c+1);
            if (numWitch == 1)
                grid[r][c] = 'W';
            else if (numWitch > 1 && numWitch < 9)
                grid[r][c] = '0' + numWitch;
            else if (numWitch >= 9)
                grid[r][c] = '9';
        }
    }

        // Indicate player's position
    if (m_player != nullptr)
    {
          // Set the char to '@', unless there's also a witch there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, witch, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << witchCount() << " witches remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool City::addWitch(int r, int c)
{
      // If MAXWITCHES have already been added, return false.  Otherwise,
      // dynamically allocate a new witch at coordinates (r,c).  Save the
      // pointer to the newly allocated witch and return true.
    if (m_nWitches >= MAXWITCHES)
        return false;
    
    m_witches[m_nWitches] = new Witch(this, r, c);
    ++m_nWitches;
    return true;
}

bool City::addPlayer(int r, int c)
{
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically allocate a new Player and add it to the city
    m_player = new Player(this, r, c);
    return true;
}

bool City::meltWitch(int r, int c)
{
    //        Melt one witch at row r, column c, and return true.
    //        Return false is there is no witch there to destroy.
    for (int i = 0; i < m_nWitches; ++i)
    {
        if (m_witches[i]->row() == r && m_witches[i]->col() == c)
        {
            delete m_witches[i];
            for (int k = i; k < m_nWitches-1; ++k)
            {
                m_witches[k] = m_witches[k+1];
            }
            -- m_nWitches;
            return true;
        }
    }
   return false;
}

bool City::moveWitches()
{
    for (int k = 0; k < m_nWitches; k++)
    {
        m_witches[k]->move();
        if (m_player->row() == m_witches[k]->row() && m_player->col() == m_witches[k]->col())
        {
            m_player->setDead();
        }
    }
      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nWitches)
{
    if (nWitches < 0)
    {
        cout << "***** Cannot create Game with negative number of witches!" << endl;
        exit(1);
    }
    if (nWitches > MAXWITCHES)
    {
        cout << "***** Trying to create Game with " << nWitches
             << " witches; only " << MAXWITCHES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nWitches > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the witches!" << endl;
        exit(1);
    }

        // Create city
    m_city = new City(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_city->addPlayer(rPlayer, cPlayer);

      // Populate with witches
    while (nWitches > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a witch where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_city->addWitch(r, c);
        nWitches--;
    }
}

Game::~Game()
{
    delete m_city;
}

void Game::play()
{
    string msg = "";
    m_city->display(msg);
    Player* player = m_city->player();
    if (player == nullptr)
        return;

    while ( ! player->isDead()  &&  m_city->witchCount() > 0)
    {
        cout << "Move (u/d/l/r/tu/td/tl/tr/q or nothing): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)
            player->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                player->move(decodeDirection(action[0]));
                break;
              case 't':
                if (action.size() < 2)  // if no direction, nobody moves
                {
                    cout << '\a' << endl;  // beep
                    continue;
                }
                switch (action[1])
                {
                  default:   // if bad direction, nobody moves
                    cout << '\a' << endl;  // beep
                    continue;
                  case 'u':
                  case 'd':
                  case 'l':
                  case 'r':
                    if (player->tossBalloon(decodeDirection(action[1])))
                        msg = "Hit!";
                    else
                        msg = "Missed!";
                    break;
                }
                break;
            }
        }
        if ( ! player->isDead())
            m_city->moveWitches();
        m_city->display(msg);
        msg = "";
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////
int main () {
        // Create a game
        // Use this instead to create a mini-game:   Game g(3, 3, 2);
      Game g(15, 18, 100);

        // Play the game
      g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
