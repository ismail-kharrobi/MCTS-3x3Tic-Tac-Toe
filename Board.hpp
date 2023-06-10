#pragma once 
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

enum
{
    EMPTY = -1,
    ME, 
    ENEMY,

};

typedef struct s_cords
{
    int x;
    int y;
} cords;
class Board;
void printBoard(Board board);

class Board
{
    public :
        int board[3][3];
        Board(){
            for (int i = 0 ; i < 3 ; i++)
            {
                for (int j = 0 ; j < 3; j++)
                {
                    this->board[i][j] = EMPTY;
                }
            }
        }
        Board( const int board[3][3])
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    this->board[i][j] = board[i][j];
                }
            }
        }

        Board(const Board *b)
        {
            for (int i = 0 ; i < 3 ; i++)
            {
                for (int j = 0 ; j < 3; j++)
                {
                    this->board[i][j] = b->board[i][j];
                }
            }
        }
        Board(const Board& other)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    this->board[i][j] = other.board[i][j];
                }
            }
        }

        bool isEqual(int a , int b, int c)
        {
            if (a == b && b == c && c != EMPTY)
                return true;
            return false;
        }

        int isEnd()
        {
            for (int i = 0 ; i < 3 ; i++)
            {
                if (isEqual(board[0][i],board[1][i],board[2][i]))
                    return board[0][i];
                if (isEqual (board[i][0],board[i][1],board[i][2]))
                    return board[i][0];
            }
            if (isEqual(board[0][0],board[1][1],board[2][2]) || isEqual(board[0][2],board[1][1],board[2][0]))
                return board[1][1];
            return EMPTY;
        }
        bool isFull()
        {
            for (int i = 0 ; i < 3; i++)
            {
                for (int j = 0 ; j < 3 ; j++)
                {
                    if (board[i][j] == EMPTY)
                        return false;
                }
            }
            return true;
        }
        vector<cords> possibleMoves()
        {
            vector<cords> positions;
            for (int i = 0 ; i < 3 ; i++)
            {
                for (int j = 0 ; j < 3 ; j++)
                {
                    if (this->board[i][j] == EMPTY)
                    {
                        cords tmp;
                        tmp.x = i;
                        tmp.y = j;
                        positions.push_back(tmp);
                    }
                }
            }
            return positions;
        }
        
        int simulate(int player)
        {
            vector<cords> moves = possibleMoves();
            if (moves.empty())
                return EMPTY;
            if (moves.size() > 1)
            {
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(moves.begin() , moves.end(),g);
            }
            size_t i = 0;
            while (i < moves.size())
            {
                player = 1 - player;
                this->board[moves[i].x][moves[i].y] = player;
                if (this->isEnd() != EMPTY)
                    return this->isEnd();
                if (this->isFull() && this->isEnd() != EMPTY)
                    return EMPTY;
                i++;
            }
            
            return this->isEnd();
        }
};

void player(int i)
{
    if (i == ME)
        cout << "X";
    else 
        cout << "O";
}
cords convertXY(int play)
{
    cords cor ;
    cor.y = (play - 1) % 3;
    if (play > 0 && play <= 3)
        cor.x = 0;
    else if (play > 3 && play <=6)
        cor.x = 1;
    else if (play > 6 && play <= 9)
        cor.x = 2;

    return cor;
}

void printBoard(Board board)
{
    (void) board;
    int row = 0;
    for (int i = 0 ; i < 3; i++)
    {
        for (int j = 0 ; j < 3;j++)
        {
            cout << "|";
            if (board.board[i][j] != EMPTY)
                player(board.board[i][j]);
            else
                cout << i + j + 1 + row;
            cout << "|";
        }
        row += 2;
        cout << endl;
    }  
    cout << "\n-------------\n";
}