#include <iostream>
using namespace std;

int currentPlayer;
char currentMarker;
char board[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
void drawBoard()
{
    cout<<" "<<board[0][0]<<" | "<<board[0][1]<<" | "<<board[0][2]<<" \n";
    cout<<"---|---|---\n";
    cout<<" "<<board[1][0]<<" | "<<board[1][1]<<" | "<<board[1][2]<<" \n";
    cout<<"---|---|---\n";
    cout<<" "<<board[2][0]<<" | "<<board[2][1]<<" | "<<board[2][2]<<" \n";
}

bool placemarker(int slot)
{
    int row = (slot - 1)/3;
    int col = (slot - 1)%3;
}

// diagonals;
if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
{
    return currentPlayer;
}
if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
{
    return currentPlayer;
}

void swapPlayerandMarker()
{
    if (currentPlayer== 'X')
    {
        
    }
    
}

void game()
{
    cout<<"Player1 choose your marker: X or O"; 
    char markerP1;
    cin>>markerP1;

    currentPlayer = 1;
    currentMarker = markerP1;

    drawBoard();
    int playerwon;
    for (int i = 0; i < 9; i++)
    {
        cout<<"its player"<<"'s turn Enter your slot:";
        int slot;
        cin>>slot;

        if (slot< 1 || slot> 9)
        {
            cout<<"It is not a vaild slot, you may try again";
            i--;
            continue;
        }

        if (!placemarker(slot))
        {
            cout<<"slot occupied! try again \n";
            i--;
            continue;
        }
        drawBoard();
        playerwon = winner();

        if (playerwon == 1)
        {
            cout<<"player 1 wins";
            break;
        }
        if (playerwon == 2)
        {
            cout<<"player 2 wins";
            break;
        }
        swapPlayerandmarker();
        
    }
    
}

int main()
{
    game();
    return 0;
}